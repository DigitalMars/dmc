/*_ random.c   Mon Jan 30 1989 */
/*
 * This file contains the command processing functions for a number of random
 * commands. There is no functional grouping here, for sure.
 */

#include        <stdio.h>
#include        "ed.h"

int     tabsize;                        /* Tab size (0: use real tabs)  */

/*****************************
 * Set fill column to n if n was given, otherwise set to current column. 
 */

random_setfillcol(f, n)
{
    fillcol = f ? n : getcol(curwp->w_dotp,curwp->w_doto);
    return TRUE;
}

/*
 * Display the current position of the cursor, in origin 1 X-Y coordinates,
 * the character that is under the cursor (in octal), and the fraction of the
 * text that is before the cursor. The displayed column is not the current
 * column, but the column that would be used on an infinite width display.
 * Normally this is bound to "C-X =".
 */
random_showcpos(f, n)
{
        register LINE   *clp;
	register long   numchars;
	register int    cbo;
	register long   thischar;
	register int    charatdot;
	register int    ratio;
	register int    col;
	register int	thisline;
	register int	numlines;

        clp = lforw(curbp->b_linep);            /* Grovel the data.     */
        cbo = 0;
        numchars = 0;
	numlines = 1;
        for (;;) {
		/* if on the current dot, save the character at dot	*/
		/* and the character and line position of the dot	*/
                if (clp==curwp->w_dotp && cbo==curwp->w_doto) {
			thisline = numlines;
                        thischar = numchars;
                        if (cbo == llength(clp))
                                charatdot = '\n';
                        else
                                charatdot = lgetc(clp, cbo);
                }
                if (cbo == llength(clp)) {
                        if (clp == curbp->b_linep)
                                break;
                        clp = lforw(clp);
                        cbo = 0;
			numlines++;
                } else
                        ++cbo;
                ++numchars;
        }
	col = getcol(curwp->w_dotp,curwp->w_doto); /* Get real column	*/
        ratio = 0;                              /* Ratio before dot.    */
        if (numchars != 0)
                ratio = (100L*thischar) / numchars;
        mlwrite("row=%d col=%d CH=0x%x .=%ld (%d%% of %ld) line %d of %d",
                currow+1, col+1, charatdot, thischar,
		ratio, numchars, thisline, numlines - 1);
        return (TRUE);
}

/*
 * Twiddle the two characters on either side of dot. If dot is at the end of
 * the line twiddle the two characters before it. Return with an error if dot
 * is at the beginning of line; it seems to be a bit pointless to make this
 * work. This fixes up a very common typo with a single stroke. Normally bound
 * to "C-T". This always works within a line, so "WFEDIT" is good enough.
 */
random_twiddle(f, n)
{
        register LINE   *dotp;
        register int    doto;
        register int    cl;
        register int    cr;

        dotp = curwp->w_dotp;
        doto = curwp->w_doto;
        if (doto==llength(dotp) && --doto<0)
                return (FALSE);
        cr = lgetc(dotp, doto);
        if (--doto < 0)
                return (FALSE);
        cl = lgetc(dotp, doto);
        lputc(dotp, doto+0, cr);
        lputc(dotp, doto+1, cl);
        line_change(WFEDIT);
        return (TRUE);
}

/*
 * Quote the next character, and insert it into the buffer. All the characters
 * are taken literally, with the exception of the newline, which always has
 * its line splitting meaning. The character is always read, even if it is
 * inserted 0 times, for regularity. Bound to "M-Q" (for me) and "C-Q" (for
 * Rich, and only on terminals that don't need XON-XOFF).
 */
random_quote(f, n)
{
        register int    s;
        register int    c;

        c = (*term.t_getchar)();
        if (c & ~0xFF || n < 0)
                return (FALSE);
        if (n == 0)
                return (TRUE);
        if (c == '\n') {
                do {
                        s = line_newline();
                } while (s==TRUE && --n);
                return (s);
        }
        return (line_insert(n, c));
}

/*
 * Set tab size if given non-default argument (n <> 1).  Otherwise, insert a
 * tab into file.  If given argument, n, of zero, change to true tabs.
 * If n > 1, simulate tab stop every n-characters using spaces. This has to be
 * done in this slightly funny way because the tab (in ASCII) has been turned
 * into "C-I" (in 10 bit code) already. Bound to "C-I".
 */
random_tab(f, n)
{
        if (n < 0)
                return (FALSE);
        if (n == 0 || n > 1) {
                tabsize = n;
                return(TRUE);
        }
        if (! tabsize)
                return(line_insert(1, '\t'));
        return(line_insert(tabsize - (getcol(curwp->w_dotp,curwp->w_doto) % tabsize), ' '));
}

/*
 * Set hardware tab size if given non-default argument (n <> 1).
 */
random_hardtab(f, n)
{
        if (n < 0)
                return (FALSE);
        if (n == 0 || n > 1)
	    hardtabsize = n;
	else if (hardtabsize == 8)
	    hardtabsize = 4;
	else if (hardtabsize == 4)
	    hardtabsize = 8;
	display_recalc();
	return TRUE;
}

/*
 * Open up some blank space. The basic plan is to insert a bunch of newlines,
 * and then back up over them. Everything is done by the subcommand
 * procerssors. They even handle the looping. Normally this is bound to "C-O".
 */
random_openline(f, n)
{
        register int    i;
        register int    s;

        if (n < 0)
                return (FALSE);
        if (n == 0)
                return (TRUE);
        i = n;                                  /* Insert newlines.     */
        do {
                s = line_newline();
	} while (s==TRUE && --i);
        if (s == TRUE)                          /* Then back up overtop */
                s = backchar(f, n);             /* of them all.         */
        return (s);
}

/*
 * Insert a newline. Bound to "C-M". If you are at the end of the line and the
 * next line is a blank line, just move into the blank line. This makes "C-O"
 * and "C-X C-O" work nicely, and reduces the ammount of screen update that
 * has to be done. This would not be as critical if screen update were a lot
 * more efficient.
 */
random_newline(f, n)
{
        int nicol;
        register LINE   *lp;
        register int    s;

        if (n < 0)
                return (FALSE);
        while (n--) {
#if 0
                lp = curwp->w_dotp;
                if (llength(lp) == curwp->w_doto /* if at end of line	*/
                && lp != curbp->b_linep		 /* and not start of buffer */
                && llength(lforw(lp)) == 0) {	/* and next line is blank */
                        if ((s=forwchar(FALSE, 1)) != TRUE)
                                return (s);
                } else
#endif
		    if ((s=line_newline()) != TRUE)
                        return (s);
        }
        return (TRUE);
}

/*
 * Delete blank lines around dot. What this command does depends if dot is
 * sitting on a blank line. If dot is sitting on a blank line, this command
 * deletes all the blank lines above and below the current line. If it is
 * sitting on a non blank line then it deletes all of the blank lines after
 * the line. Normally this command is bound to "C-X C-O". Any argument is
 * ignored.
 */
random_deblank(f, n)
{
        register LINE   *lp1;
        register LINE   *lp2;
        register int    nld;

        lp1 = curwp->w_dotp;
        while (llength(lp1)==0 && (lp2=lback(lp1))!=curbp->b_linep)
                lp1 = lp2;
        lp2 = lp1;
        nld = 0;
        while ((lp2=lforw(lp2))!=curbp->b_linep && llength(lp2)==0)
                ++nld;
        if (nld == 0)
                return (TRUE);
        curwp->w_dotp = lforw(lp1);
        curwp->w_doto = 0;
        return (line_delete(nld,TRUE));
}

/*
 * Insert a newline, then enough tabs and spaces to duplicate the indentation
 * of the previous line. Assumes tabs are every eight characters. Quite simple.
 * Figure out the indentation of the current line. Insert a newline by calling
 * the standard routine. Insert the indentation by inserting the right number
 * of tabs and spaces. Return TRUE if all ok. Return FALSE if one of the
 * subcomands failed. Normally bound to "C-J".
 */

#if 0
random_indent(f, n)
{
        register int    nicol;
        register int    c;
        register int    i;

        if (n < 0)
                return (FALSE);
        while (n--) {
                nicol = 0;
                for (i=0; i<llength(curwp->w_dotp); ++i) {
                        c = lgetc(curwp->w_dotp, i);
                        if (c!=' ' && c!='\t')
                                break;
                        if (c == '\t')
                                nicol |= 0x07;
                        ++nicol;
                }
                if (line_newline() == FALSE
                || ((i=nicol/8)!=0 && line_insert(i, '\t')==FALSE)
                || ((i=nicol%8)!=0 && line_insert(i,  ' ')==FALSE))
                        return (FALSE);
        }
        return (TRUE);
}
#endif

/*********************************
 * Determine indentation level of current line.
 * Increase it by 4.
 * Step to next line.
 */

random_incindent(f, n)	{ return changeindent(f,n,4); }
random_decindent(f, n)	{ return changeindent(f,n,-4); }

static changeindent(f, n, val)
{
        register int    nicol;
        register int    c;
        register int    i;
	struct LINE *dotpsave;
	int dotosave;

        if (n < 0)
	    goto err;
	if (window_marking(curwp))
	{   REGION region;
	    int s;

	    if ((s = getregion(&region)) != TRUE)
		return s;
	    dotpsave = curwp->w_dotp;
	    dotosave = curwp->w_doto;
	    curwp->w_dotp = region.r_linep;
	    curwp->w_doto = region.r_offset;
	    n = region.r_nlines;
	}
        while (n--)
	{   int len;

	    len = llength(curwp->w_dotp);
	    if (len)
	    {
                nicol = 0;
		for (i=0; i < len; ++i) {
		    c = lgetc(curwp->w_dotp, i);
		    if (c == '{' && i + 1 < len) /* } to balance things out */
		    {	int j;

			/* Next non-white char after bracket is indented by 3 */
			for (j = i + 1; j < len; j++)
			{   c = lgetc(curwp->w_dotp,j);
			    if (c != ' ' && c != '\t')
				break;
			}
			curwp->w_doto = i + 1;
			line_delete(j - (i + 1),FALSE);
			line_insert(abs(val) - 1,' ');
			break;
		    }
		    if (c!=' ' && c!='\t')
			break;
		    if (c == '\t')
			nicol |= 0x07;
		    ++nicol;
                }
		curwp->w_doto = 0;	/* move to beginning of line	*/
		line_delete(i,FALSE);	/* delete existing blanks	*/
		nicol += val;
		if (nicol < 0)
		    nicol = 0;
		if (((i=nicol/8)!=0 && line_insert(i, '\t')==FALSE)
		 || ((i=nicol%8)!=0 && line_insert(i,  ' ')==FALSE))
		    goto err;
	    }
	    if (forwline(FALSE,1) == FALSE)
		goto err;
        }
	if (window_marking(curwp))
	{
	    if (dotosave > llength(dotpsave))
		dotosave = llength(dotpsave);
	    curwp->w_dotp = dotpsave;
	    curwp->w_doto = dotosave;
	}
        return (TRUE);

err:
	return FALSE;
}

/*********************************
 * Optimally tab a line or region.
 * Step to next line.
 */

random_opttab(f, n)
{   
    register int    c;
    register int    i;
    struct LINE *dotpsave;
    int dotosave;

    if (n < 0)
	goto err;
    if (window_marking(curwp))
    {   REGION region;
	int s;

	if ((s = getregion(&region)) != TRUE)
	    return s;
	dotpsave = curwp->w_dotp;
	dotosave = curwp->w_doto;
	curwp->w_dotp = region.r_linep;
	curwp->w_doto = region.r_offset;
	n = region.r_nlines;
    }
    while (n--)
    {   
	int nspaces = 0;
	int nwhite = 0;
	int nicol = 0;			/* column number		*/

	for (i=0; i < llength(curwp->w_dotp); i++)
	{   
	    c = lgetc(curwp->w_dotp, i);
	    switch (c)
	    {   
		case '\t':
		    nwhite++;
		    if (nspaces)
		    {	int ntabs;

			i -= nspaces;
			nwhite -= nspaces;
			curwp->w_doto = i;
			line_delete(nspaces,FALSE);
			ntabs = nspaces >> 3;
			line_insert(ntabs,'\t');
			i += ntabs;
			nwhite += ntabs;
			nspaces = 0;
		    }
		    nicol = (nicol | 7) + 1;
		    break;
		default:
		    if (nspaces >= 2 && (nicol & 7) == 0)
		    {   i -= nspaces;
			curwp->w_doto = i;
			line_delete(nspaces,FALSE);
			line_insert((nspaces + 7) >> 3,'\t');
			i += (nspaces + 7) >> 3;
			nspaces = 0;
		    }
		    if (c == ' ')
		    {	nwhite++;
			nspaces++;
		    }
		    else
		    {	nwhite = 0;
			nspaces = 0;
		    }
		    nicol++;
		    break;
	    }
	}

	/* Truncate any trailing spaces or tabs	*/
	if (nwhite)
	{   curwp->w_doto = i - nwhite;
	    line_delete(nwhite,FALSE);
	}

	if (forwline(FALSE,1) == FALSE)		/* proceed to next line	*/
	    goto err;
    }
    if (window_marking(curwp))
    {   
	if (dotosave > llength(dotpsave))
	    dotosave = llength(dotpsave);
	curwp->w_dotp = dotpsave;
	curwp->w_doto = dotosave;
    }
    return (TRUE);

err:
    return FALSE;
}

/*
 * Delete forward. This is real easy, because the basic delete routine does
 * all of the work. Watches for negative arguments, and does the right thing.
 * If any argument is present, it kills rather than deletes, to prevent loss
 * of text if typed with a big argument. Normally bound to "C-D".
 */

static char undelch;

random_forwdel(f,n)
{
        if (n < 0)
                return (random_backdel(f, -n));
	if (f) {                       /* Really a random_kill.       */
                if ((lastflag&CFKILL) == 0)
                        kill_freebuffer();
                thisflag |= CFKILL;
        }
	else
		undelch = lgetc(curwp->w_dotp,curwp->w_doto);
        return (line_delete(n, f));
}

random_undelchar(f,n)
{
	return line_insert(n,undelch);
}

/*
 * Delete backwards. This is quite easy too, because it's all done with other
 * functions.
 * If marking, then cut the marked region.
 * Just move the cursor back, and delete forwards. Like delete
 * forward, this actually does a kill if presented with an argument.
 * Bound to both "RUBOUT" and "C-H".
 */
random_backdel(f, n)
{
        register int    s;

        if (n < 0)
                return (random_forwdel(f, -n));
	if (curwp->w_markp)			/* if marking		*/
		return region_kill(f,n);
        if (f != FALSE) {                       /* Really a kill.       */
                if ((lastflag&CFKILL) == 0)
                        kill_freebuffer();
                thisflag |= CFKILL;
        }
        if ((s=backchar(f, n)) == TRUE)
                s = line_delete(n, f);
        return (s);
}

/*
 * Kill text. If called without an argument, it kills from dot to the end of
 * the line, unless it is at the end of the line, when it kills the newline.
 * If called with an argument of 0, it kills from the start of the line to dot.
 * If called with a positive argument, it kills from dot forward over that
 * number of newlines. If called with a negative argument it kills backwards
 * that number of newlines. Normally bound to "C-K".
 */
random_kill(f, n)
{
        register int    chunk;
        register LINE   *nextp;

        if ((lastflag&CFKILL) == 0)             /* Clear kill buffer if */
                kill_freebuffer();                      /* last wasn't a kill.  */
        thisflag |= CFKILL;
        if (f == FALSE) {
                chunk = llength(curwp->w_dotp)-curwp->w_doto;
                if (chunk == 0)
                        chunk = 1;
        } else if (n == 0) {
                chunk = curwp->w_doto;
                curwp->w_doto = 0;
        } else if (n > 0) {
                chunk = llength(curwp->w_dotp)-curwp->w_doto+1;
                nextp = lforw(curwp->w_dotp);
                while (--n) {
                        if (nextp == curbp->b_linep)
                                return (FALSE);
                        chunk += llength(nextp)+1;
                        nextp = lforw(nextp);
                }
        } else {
                mlwrite("neg kill");
                return (FALSE);
        }
        return (line_delete(chunk, TRUE));
}

/*
 * Yank text back from the kill buffer. This is really easy. All of the work
 * is done by the standard insert routines. All you do is run the loop, and
 * check for errors. Bound to "C-Y". The blank lines are inserted with a call
 * to "newline" instead of a call to "line_newline" so that the magic stuff that
 * happens when you type a carriage return also happens when a carriage return
 * is yanked back from the kill buffer.
 */
random_yank(f, n)
{
        register int    c;
        register int    i;
        extern   int    kused;

        if (n < 0)
	    goto err;
        while (n--)
	{
	    i = 0;
	    if (column_mode)
	    {	int col;

		col = getcol(curwp->w_dotp,curwp->w_doto);
		while ((c=kill_remove(i)) >= 0)
		{
		    if (c == '\n')
		    {	int nspaces;

			if (curwp->w_dotp == curbp->b_linep)
			{   if (random_newline(FALSE, 1) == FALSE)
				goto err;
			}
			else
			    curwp->w_dotp = lforw(curwp->w_dotp);
			curwp->w_flag |= WFHARD;
			curwp->w_doto = coltodoto(curwp->w_dotp,col);
			if (kill_remove(i + 1) >= 0)
			{
			    nspaces = col - getcol(curwp->w_dotp,curwp->w_doto);
			    while (nspaces--)
				if (!line_insert(1,' '))
				    goto err;
			}
		    }
		    else
		    {
			if (line_insert(1, c) == FALSE)
			    goto err;
		    }
		    ++i;
		}
	    }
	    else
		while ((c=kill_remove(i)) >= 0)
		{
		    if (c == '\n') {
			if (random_newline(FALSE, 1) == FALSE)
			    goto err;
		    } else {
			if (line_insert(1, c) == FALSE)
			    goto err;
		    }
		    ++i;
		}
        }
        return (TRUE);

err:
    return FALSE;
}

