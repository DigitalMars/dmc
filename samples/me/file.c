/*_ file.c   Mon May 12 1986 */
/*
 * The routines in this file
 * handle the reading and writing of
 * disk files. All of details about the
 * reading and writing of the disk are
 * in "fileio.c".
 */
#include        <stdio.h>
#include        "ed.h"

/**********************************
 * Save current file. Get next file and read it into the
 * current buffer. Next file is either:
 *	o next argument from command line
 *	o input from the user
 */

int filenext(f,n)
{	int s;
	extern int gargi,gargc;
	extern char **gargv;

	if (filesave(f,n) == FALSE)	/* save current file		*/
		return FALSE;
	if (gargi < gargc)		/* if more files on command line */
	{
		s = readin(gargv[gargi]);
		gargi++;
	}
	else				/* get file name from user	*/
		s = fileread(f,n);
	makename(curbp->b_bname,curbp->b_fname);
	return s;
}

#if MYIO
/**********************************
 * Insert a file into the current buffer.
 */

int Dinsertfile(f,n)
{
	int s,nline,nbytes;
	WINDOW *wp;
	char fname[NFILEN],*line;

	fname[0] = 0;
	if (mlreply("Insert file: ",fname,NFILEN) == FALSE)
		return FALSE;

	s = ffropen(fname);		/* open file for reading	*/
	switch (s)
	{
	    case FIOFNF:
		mlwrite("File not found");
	    case FIOERR:
		return FALSE;
	}
	mlwrite("[Reading file]");
	nline = 0;
	while ((s = ffgetline(&line,&nbytes)) == FIOSUC)
	{	register char *p;

		for (p = line; nbytes; p++,nbytes--)
		{
			if (line_insert(1,*p) == FALSE)
				return FALSE;
		}
		if (random_newline(FALSE,1) == FALSE)
			return FALSE;
		++nline;
	}
	ffclose();
        if (s == FIOEOF) {                      /* Don't zap message!   */
                if (nline == 1)
                        mlwrite("[Read 1 line]");
                else
                        mlwrite("[Read %d lines]", nline);
        }
        for (wp=wheadp; wp!=NULL; wp=wp->w_wndp) {
                if (wp->w_bufp == curbp) {
                        wp->w_flag |= WFMODE|WFHARD;
                }
        }
	return s != FIOERR;
}
#endif

/*
 * Read a file into the current
 * buffer. This is really easy; all you do it
 * find the name of the file, and call the standard
 * "read a file into the current buffer" code.
 * Bound to "C-X C-R".
 */
fileread(f, n)
{
        register int    s;
        char            fname[NFILEN];

	fname[0] = 0;
        if ((s=mlreply("Read file: ", fname, NFILEN)) != TRUE)
                return (s);
        return (readin(fname));
}

/*
 * Select a file for editing.
 * Look around to see if you can find the
 * file in another buffer; if you can find it
 * just switch to the buffer. If you cannot find
 * the file, create a new buffer, read in the
 * text, and switch to the new buffer.
 * Bound to GOLD E.
 */
filevisit(f, n)
{
        char            fname[NFILEN];

	fname[0] = 0;
        return	mlreply("Visit file: ", fname, NFILEN) &&
		window_split(f,n) &&
		file_readin(fname);
}

int file_readin(fname)
char *fname;
{
    register BUFFER *bp;
    register WINDOW *wp;
    register LINE   *lp;
    register int    i;
    register int    s;
    char            bname[NBUFN];

    /* If there is an existing buffer with the same file name, simply	*/
    /* switch to it instead of reading the file again.			*/
    for (bp=bheadp; bp!=NULL; bp=bp->b_bufp)
    {
	/* Always redo temporary buffers, check for filename match.	*/
	if ((bp->b_flag&BFTEMP)==0 && filcmp(bp->b_fname, fname)==0)
	{
	    /* If the current buffer now becomes undisplayed		*/
	    if (--curbp->b_nwnd == 0)
	    {   
		curbp->b_dotp  = curwp->w_dotp;
		curbp->b_doto  = curwp->w_doto;
		curbp->b_markp = curwp->w_markp;
		curbp->b_marko = curwp->w_marko;
	    }
	    curbp = bp;
	    curwp->w_bufp  = bp;
	    if (bp->b_nwnd++ == 0)	/* if buffer not already displayed */
	    {   
		curwp->w_dotp  = bp->b_dotp;
		curwp->w_doto  = bp->b_doto;
		curwp->w_markp = bp->b_markp;
		curwp->w_marko = bp->b_marko;
	    }
	    else
	    {
		/* Set dot to be at place where other window has it	*/
		for (wp = wheadp; wp != NULL; wp = wp->w_wndp)
		{   
		    if (wp!=curwp && wp->w_bufp==bp)
		    {   
			curwp->w_dotp  = wp->w_dotp;
			curwp->w_doto  = wp->w_doto;
			curwp->w_markp = wp->w_markp;
			curwp->w_marko = wp->w_marko;
			break;
		    }
		}
	    }

	    /* Adjust frame so dot is at center	*/
	    lp = curwp->w_dotp;
	    i = curwp->w_ntrows/2;
	    while (i-- && lback(lp)!=curbp->b_linep)
		lp = lback(lp);
	    curwp->w_linep = lp;

	    curwp->w_flag |= WFMODE|WFHARD;
	    mlwrite("[Old buffer]");
	    return TRUE;
	}
    }

    makename(bname, fname);                 /* New buffer name.     */
    while ((bp=buffer_find(bname, FALSE, 0)) != NULL)
    {   bname[0] = 0;
	s = mlreply("Buffer name: ", bname, NBUFN);
	if (s == ABORT)                 /* ^G to just quit      */
	    return (s);
	if (s == FALSE) {               /* CR to clobber it     */
	    makename(bname, fname);
	    break;
	}
    }
    if (bp==NULL && (bp=buffer_find(bname, TRUE, 0))==NULL)
    {	mlwrite("Cannot create buffer");
	return (FALSE);
    }
    if (--curbp->b_nwnd == 0)			/* Undisplay		*/
    {	curbp->b_dotp = curwp->w_dotp;
	curbp->b_doto = curwp->w_doto;
	curbp->b_markp = curwp->w_markp;
	curbp->b_marko = curwp->w_marko;
    }
    curbp = bp;                             /* Switch to it.        */
    curwp->w_bufp = bp;
    curbp->b_nwnd++;
    return (readin(fname));                 /* Read it in.          */
}

/*
 * Read file "fname" into the current
 * buffer, blowing away any text found there. Called
 * by both the read and visit commands. Return the final
 * status of the read. Also called by the mainline,
 * to read in a file specified on the command line as
 * an argument.
 */
readin(fname)
char    fname[];
{
        register LINE   *lp1;
        register LINE   *lp2;
        register int    i;
        register WINDOW *wp;
        register BUFFER *bp;
        register int    s;
        int    nbytes;
        register int    nline;
        char            *line;

        bp = curbp;                             /* Cheap.               */
        if ((s=buffer_clear(bp)) != TRUE)             /* Might be old.        */
                return (s);
        bp->b_flag &= ~(BFTEMP|BFCHG);
        strcpy(bp->b_fname, fname);

	/* Determine if file is read-only	*/
	if (ffreadonly(fname))			/* is file read-only?	*/
		bp->b_flag |= BFRDONLY;
	else
		bp->b_flag &= ~BFRDONLY;
        if ((s=ffropen(fname)) == FIOERR)       /* Hard file open.      */
	{	mlwrite("[Bad file]");
                goto out;
	}
        if (s == FIOFNF) {                      /* File not found.      */
                mlwrite("[New file]");
                goto out;
        }
        mlwrite("[Reading file]");

        nline = 0;
        while ((s=ffgetline(&line,&nbytes)) == FIOSUC) {
                if ((lp1=line_realloc(NULL,nbytes)) == NULL) {
                        s = FIOERR;             /* Keep message on the  */
                        break;                  /* display.             */
                }
                lp2 = lback(curbp->b_linep);
                lp2->l_fp = lp1;
                lp1->l_fp = curbp->b_linep;
                lp1->l_bp = lp2;
                curbp->b_linep->l_bp = lp1;
		memcpy(lp1->l_text,line,nbytes);
                ++nline;
        }
        ffclose();                              /* Ignore errors.       */
        if (s == FIOEOF) {                      /* Don't zap message!   */
                if (nline == 1)
                        mlwrite("[Read 1 line]");
                else
                        mlwrite("[Read %d lines]", nline);
        }
out:
        for (wp=wheadp; wp!=NULL; wp=wp->w_wndp) {
                if (wp->w_bufp == curbp) {
                        wp->w_linep = lforw(curbp->b_linep);
                        wp->w_dotp  = lforw(curbp->b_linep);
                        wp->w_doto  = 0;
                        wp->w_markp = NULL;
                        wp->w_marko = 0;
                        wp->w_flag |= WFMODE|WFHARD;
                }
        }
	return s != FIOERR;			/* FALSE if error	*/
}

/*
 * Take a file name, and from it
 * fabricate a buffer name. This routine knows
 * about the syntax of file names on the target system.
 * I suppose that this information could be put in
 * a better place than a line of code.
 */
makename(bname, fname)
char    bname[];
char    fname[];
{
#if 1
	strcpy(bname,fname);
#else
        register char   *cp1;
        register char   *cp2;

	cp1 = fname + strlen(fname);	/* point to terminating 0 of string */

#if     AMIGA
        while (cp1!=&fname[0] && cp1[-1]!=':' && cp1[-1]!='/')
                --cp1;
#endif
#if     VMS
        while (cp1!=&fname[0] && cp1[-1]!=':' && cp1[-1]!=']')
                --cp1;
#endif
#if     CPM
        while (cp1!=&fname[0] && cp1[-1]!=':')
                --cp1;
#endif
#if     MSDOS || _WIN32
        while (cp1!=&fname[0] && cp1[-1]!=':' && cp1[-1]!='\\')
                --cp1;
#endif
#if     BSDUNIX || linux
        while (cp1!=&fname[0] && cp1[-1]!='/')
                --cp1;
#endif
        cp2 = &bname[0];
        while (cp2!=&bname[NBUFN-1] && *cp1!=0 && *cp1!=';')
                *cp2++ = *cp1++;
        *cp2 = 0;
#endif
}

/*
 * Ask for a file name, and write the
 * contents of the current buffer or region to that file.
 * Update the remembered file name and clear the
 * buffer changed flag. This handling of file names
 * is different from the earlier versions, and
 * is more compatible with Gosling EMACS than
 * with ITS EMACS. Bound to "C-X C-W".
 */
filewrite(f, n)
{
    register int    s;
    char            fname[NFILEN];

    fname[0] = 0;
    if ((s=mlreply("Write file: ", fname, NFILEN)) != TRUE)
	return (s);
    if (curwp->w_markp)		/* if marking a region	*/
    {   REGION region;

	if (!getregion(&region))
	    return FALSE;
	return file_writeregion(fname,&region);
    }
    else
    {
        if ((s=writeout(fname)) == TRUE) {
	    strcpy(curbp->b_fname, fname);
	    fileunmodify(f,n);
        }
    }
    return (s);
}

/****************************
 * Mark a file as being unmodified.
 */

fileunmodify(f,n)
{
    register WINDOW *wp;

    curbp->b_flag &= ~BFCHG;

    /* Update mode lines.   */
    for (wp = wheadp; wp != NULL; wp = wp->w_wndp)
    {
	if (wp->w_bufp == curbp)
	    wp->w_flag |= WFMODE;
    }
    return TRUE;
}

/*
 * Save the contents of the current
 * buffer in its associated file. No nothing
 * if nothing has changed (this may be a bug, not a
 * feature). Error if there is no remembered file
 * name for the buffer. Bound to "C-X C-S". May
 * get called by "C-Z".
 */
filesave(f, n)
{
        register WINDOW *wp;
        register int    s;

        if ((curbp->b_flag&BFCHG) == 0)         /* Return, no changes.  */
                return (TRUE);
        if (curbp->b_fname[0] == 0) {           /* Must have a name.    */
                mlwrite("No file name");
                return (FALSE);
        }
        if ((s=writeout(curbp->b_fname)) == TRUE) {
		fileunmodify(f,n);
        }
        return (s);
}

/*
 * Save the contents of each and every modified
 * buffer.  Does nothing if the buffer is temporary
 * or has no filename.
 */
filemodify(f, n)
{
        register WINDOW *wp;
	register int s = TRUE;
	BUFFER *oldbp;

	oldbp = curbp;
	for (curbp = bheadp; curbp != NULL; curbp = curbp->b_bufp)
	{
		if((curbp->b_flag&BFCHG) == 0 || /* if no changes	*/
		   curbp->b_flag & BFTEMP ||	/* if temporary		*/
		   curbp->b_fname[0] == 0)	/* Must have a name	*/
			continue;
		if((s&=writeout(curbp->b_fname)) == TRUE )
			fileunmodify(f,n);
	}
	curbp = oldbp;
	return( s );
}

/*
 * This function performs the details of file
 * writing. Uses the file management routines in the
 * "fileio.c" package. The number of lines written is
 * displayed. Sadly, it looks inside a LINE; provide
 * a macro for this. Most of the grief is error
 * checking of some sort.
 */
writeout(fn)
char    *fn;
{
        register int    s;
        register LINE   *lp;
        register int    nline;

	static char backupname[NFILEN+4];
	/*
	 * This code has been added to supply backups
	 * when writing files.
	 */
	strcpy(backupname,fn);
	s = strlen(backupname);
#if MSDOS || _WIN32
	while( s 
	 && backupname[s] != '.' 
	 && backupname[s] != '\\'
	 && backupname[s] != '/'
	 && backupname[s] != ':' )
		s--;
	if( backupname[s] == '.' )
		strcpy(backupname+s, ".BAK");
	else
		strcat(backupname, ".BAK");
#endif
#if BSDUNIX || linux
	while( s
	 && backupname[s] != '/' )
		s--;
	if( backupname[s] == '/' ) s++;
	strcpy( backupname+s, ".B" );		/* UniPress EMACS style */
	strcat( backupname, fn+s );
#endif
	ffunlink( backupname );			/* Remove old backup file */
	if( ffrename( fn, backupname ) != FIOSUC )
		return( FALSE );		/* Make new backup file */

        if ((s=ffwopen(fn)) != FIOSUC)          /* Open writes message. */
                return (FALSE);
	if ( ffchmod( fn, backupname ) != FIOSUC ) /* Set protection	*/
		return( FALSE );

        lp = lforw(curbp->b_linep);             /* First line.          */
        nline = 0;                              /* Number of lines.     */
        while (lp != curbp->b_linep) {
                if ((s=ffputline(&lp->l_text[0], llength(lp))) != FIOSUC)
                        break;
                ++nline;
                lp = lforw(lp);
        }
	return file_finish(s,nline);
}

/*
 * The command allows the user
 * to modify the file name associated with
 * the current buffer. It is like the "f" command
 * in UNIX "ed". The operation is simple; just zap
 * the name in the BUFFER structure, and mark the windows
 * as needing an update. You can type a blank line at the
 * prompt if you wish.
 */
filename(f, n)
{
        register WINDOW *wp;
        register int    s;
        char            fname[NFILEN];

	fname[0] = 0;
        if ((s=mlreply("New File Name: ", fname, NFILEN)) == ABORT)
                return (s);
        if (s == FALSE)
                strcpy(curbp->b_fname, "");
        else
                strcpy(curbp->b_fname, fname);
        wp = wheadp;                            /* Update mode lines.   */
        while (wp != NULL) {
                if (wp->w_bufp == curbp)
                        wp->w_flag |= WFMODE;
                wp = wp->w_wndp;
        }
        return (TRUE);
}

/*******************************
 * Write region out to file.
 */

int file_writeregion(filename,region)
char *filename;
REGION *region;
{   int s;
    LINE   *lp;
    int    nline;
    int size;
    int loffs;

    if ((s=ffwopen(filename)) != FIOSUC)	/* open writes message	*/
	return (FALSE);

    lp = region->r_linep;		/* First line.          */
    loffs = region->r_offset;
    size = region->r_size;
    nline = 0;				/* Number of lines.     */
    while (size > 0)
    {	int nchars;

	nchars = llength(lp) - loffs;
	if (nchars > size)		/* if last line is not a full line */
	    nchars = size;
	if ((s=ffputline(&lp->l_text[loffs], nchars)) != FIOSUC)
	    break;
	size -= nchars + 1;
	++nline;
	lp = lforw(lp);
	loffs = 0;
    }
    return file_finish(s,nline);
}

/************************
 * Finish writing file.
 */

static int file_finish(s,nline)
{
    if (s == FIOSUC) {                      /* No write error.      */
	    s = ffclose();
	    if (s == FIOSUC) {              /* No close error.      */
		    if (nline == 1)
			    mlwrite("[Wrote 1 line]");
		    else
			    mlwrite("[Wrote %d lines]", nline);
	    }
    } else                                  /* Ignore close error   */
	    ffclose();                      /* if a write error.    */
    return s == FIOSUC;			/* TRUE if success	*/
}
