/*_ line.c   Wed Apr  5 1989 */
/*
 * The functions in this file are a general set of line management utilities.
 * They are the only routines that touch the text. They also touch the buffer
 * and window structures, to make sure that the necessary updating gets done.
 * There are routines in this file that handle the kill buffer too. It isn't
 * here for any good reason.
 *
 * Note that this code only updates the dot and mark values in the window list.
 * Since all the code acts on the current window, the buffer that we are
 * editing must be being displayed, which means that "b_nwnd" is non zero,
 * which means that the dot and mark values in the buffer headers are nonsense.
 */

#include        <stdio.h>
#include	<string.h>
#include        "ed.h"

#define NBLOCK  16                      /* Line block chunk size        */

/*
 * This routine allocates a block of memory large enough to hold a LINE
 * containing "used" characters. The block is always rounded up a bit. Return
 * a pointer to the new block, or NULL if there isn't any memory left. Print a
 * message in the message line if no space.
 */

LINE    *
line_realloc(lpold,used)
register LINE *lpold;
register int    used;
{
        register LINE   *lp;
        register int    size;

        size = (used+NBLOCK-1) & ~(NBLOCK-1);
        if (size == 0)                          /* Assume that an empty */
                size = NBLOCK;                  /* line is for type-in. */
        if ((lp = (LINE *) realloc(lpold,sizeof(LINE)+size)) == NULL) {
                mlwrite("Cannot allocate %d bytes", size);
                return (NULL);
        }
        lp->l_size = size;
        lp->l_used = used;
        return (lp);
}

/*
 * Delete line "lp". Fix all of the links that might point at it (they are
 * moved to offset 0 of the next line). Unlink the line from whatever buffer it
 * might be in. Release the memory. The buffers are updated too; the magic
 * conditions described in the above comments don't hold here.
 */
line_free(lp)
register LINE   *lp;
{
        register BUFFER *bp;
        register WINDOW *wp;

	for (wp = wheadp; wp != NULL; wp = wp->w_wndp)
	{
	    if (wp->w_linep == lp)
		wp->w_linep = lp->l_fp;
	    if (wp->w_dotp  == lp) {
		wp->w_dotp  = lp->l_fp;
		wp->w_doto  = 0;
	    }
	    if (wp->w_markp == lp) {
		wp->w_markp = lp->l_fp;
		wp->w_marko = 0;
	    }
	}
	for (bp = bheadp; bp != NULL; bp = bp->b_bufp)
	{
	    /* If there are windows on this buffer, the dot and mark	*/
	    /* values are nonsense.					*/
	    if (bp->b_nwnd == 0)	/* if no windows on this buffer	*/
	    {
		/* update dot or mark in the buffer	*/
		if (bp->b_dotp  == lp) {
			bp->b_dotp = lp->l_fp;
			bp->b_doto = 0;
		}
		if (bp->b_markp == lp) {
			bp->b_markp = lp->l_fp;
			bp->b_marko = 0;
		}
	    }
        }
        lp->l_bp->l_fp = lp->l_fp;
        lp->l_fp->l_bp = lp->l_bp;
        free((char *) lp);
}

/*
 * This routine gets called when a character is changed in place in the current
 * buffer. It updates all of the required flags in the buffer and window
 * system. The flag used is passed as an argument; if the buffer is being
 * displayed in more than 1 window we change EDIT to HARD. Set MODE if the
 * mode line needs to be updated (the "*" has to be set).
 */
line_change(flag)
register int    flag;
{
        register WINDOW *wp;

#if 0
        if (curbp->b_nwnd != 1)                 /* Ensure hard.         */
                flag = WFHARD;
#endif
	if (curwp->w_markp)			/* if marking		*/
	    curwp->w_flag |= WFMOVE;		/* so highlighting is updated */
        if ((curbp->b_flag&(BFCHG|BFRDONLY)) == 0) /* First change, so     */
	{   flag |= WFMODE;			/* update mode lines	*/
	    curbp->b_flag |= BFCHG;
        }
        for (wp = wheadp; wp; wp = wp->w_wndp)
	{
                if (wp->w_bufp == curbp)
		{
		    wp->w_flag |= flag;
		    if (wp != curwp)
			wp->w_flag |= WFHARD;
		}
        }
}

/*
 * Insert "n" copies of the character "c" at the current location of dot. In
 * the easy case all that happens is the text is stored in the line. In the
 * hard case, the line has to be reallocated. When the window list is updated,
 * take special care; I screwed it up once. You always update dot in the
 * current window. You update mark, and a dot in another window, if it is
 * greater than the place where you did the insert. Return TRUE if all is
 * well, and FALSE on errors.
 */
line_insert(n, c)
{
        register char   *cp1;
        register char   *cp2;
        register LINE   *lp1;
        register LINE   *lp2;
        register LINE   *lp3;
        register int    doto;
        register int    i;
        register WINDOW *wp;

	if (curbp->b_flag & BFRDONLY)		/* if buffer is read-only */
	    return FALSE;			/* error		*/
        line_change(WFEDIT);
        lp1 = curwp->w_dotp;                    /* Current line         */
        if (lp1 == curbp->b_linep) {            /* At the end: special  */
                if (curwp->w_doto != 0) {
                        mlwrite("bug: line_insert");
                        return (FALSE);
                }
                if ((lp2=line_realloc(NULL,n)) == NULL)    /* Allocate new line    */
                        return (FALSE);
                lp3 = lp1->l_bp;                /* Previous line        */
                lp3->l_fp = lp2;                /* Link in              */
                lp2->l_fp = lp1;
                lp1->l_bp = lp2;
                lp2->l_bp = lp3;
		memset(lp2->l_text,c,n);
                curwp->w_dotp = lp2;
                curwp->w_doto = n;
                return (TRUE);
        }
        doto = curwp->w_doto;                   /* Save for later.      */
	/* The if is not necessary, we do it for efficiency		*/
        if (lp1->l_used+n > lp1->l_size)	/* Hard: reallocate	*/
	{   if ((lp2=line_realloc(lp1,lp1->l_used+n)) == NULL)
		return (FALSE);
	    lp2->l_bp->l_fp = lp2;
	    lp2->l_fp->l_bp = lp2;
	}
	else
	{
	    lp2 = lp1;
	    lp2->l_used += n;
	}
	memmove(&lp2->l_text[doto + n],&lp2->l_text[doto],
		lp2->l_used - n - doto);
	if (n == 1)
	    lputc(lp2,doto,c);
	else
	    memset(&lp2->l_text[doto],c,n);

	/* Update windows       */
	for (wp = wheadp; wp != NULL; wp = wp->w_wndp)
	{   if (wp->w_linep == lp1)
		    wp->w_linep = lp2;
	    if (wp->w_dotp == lp1) {
		    wp->w_dotp = lp2;
		    if (wp==curwp || wp->w_doto>doto)
			    wp->w_doto += n;
	    }
	    if (wp->w_markp == lp1) {
		    wp->w_markp = lp2;
		    if (wp->w_marko > doto)
			    wp->w_marko += n;
	    }
	}

        return (TRUE);
}

/***************************
 * Same as line_insert(), but for overwrite mode.
 */

line_overwrite(n,c)
{   int status = TRUE;

    while (n-- > 0)
    {	if (curwp->w_doto < llength(curwp->w_dotp))
	    status = random_forwdel(FALSE,1);
	if (status)
	    status = line_insert(1,c);
	if (!status)
	    break;
    }
    return status;
}

/********************************************
 * Insert a newline into the buffer at the current location of dot in the
 * current window. The funny ass-backwards way it does things is not a botch;
 * it just makes the last line in the file not a special case. Return TRUE if
 * everything works out and FALSE on error (memory allocation failure). The
 * update of dot and mark is a bit easier then in the above case, because the
 * split forces more updating.
 */
line_newline()
{
        register char   *cp1;
        register char   *cp2;
        register LINE   *lp1;
        register LINE   *lp2;
        register int    doto;
        register WINDOW *wp;

	if (curbp->b_flag & BFRDONLY)		/* if buffer is read-only */
	    return FALSE;			/* error		*/
        lp1  = curwp->w_dotp;                   /* Get the address and  */
        doto = curwp->w_doto;                   /* offset of "."        */
#if 0
	/* If "." is at end of buffer, and buffer has no terminating	*/
	/* CR, then just turn off the no CR flag			*/
	if (curbp->b_flag & BFNOCR &&
	    doto == llength(lp1) &&		/* dot is at end of line */
	    lp1 != curbp->b_linep &&		/* buffer is not empty	 */
	    lforw(lp1) == curbp->b_linep)	/* this is the last line */
	{   curbp->b_flag &= ~BFNOCR;
	    return forwchar(FALSE,1);
	}
#endif
	if ((lp2=line_realloc(NULL,doto)) == NULL) /* New first half line  */
                return (FALSE);
#if 1
        lp1->l_used -= doto;
	memmove(lp2->l_text,lp1->l_text,doto);
	memmove(lp1->l_text,lp1->l_text + doto,lp1->l_used);
#else
        cp1 = &lp1->l_text[0];                  /* Shuffle text around  */
        cp2 = &lp2->l_text[0];
        while (cp1 != &lp1->l_text[doto])
                *cp2++ = *cp1++;
        cp2 = &lp1->l_text[0];
        while (cp1 != &lp1->l_text[lp1->l_used])
                *cp2++ = *cp1++;
        lp1->l_used -= doto;
#endif
        lp2->l_bp = lp1->l_bp;
        lp1->l_bp = lp2;
        lp2->l_bp->l_fp = lp2;
        lp2->l_fp = lp1;
        wp = wheadp;                            /* Windows              */
        while (wp != NULL) {
                if (wp->w_linep == lp1)
                        wp->w_linep = lp2;
                if (wp->w_dotp == lp1) {
                        if (wp->w_doto < doto)
                                wp->w_dotp = lp2;
                        else
                                wp->w_doto -= doto;
                }
                if (wp->w_markp == lp1) {
                        if (wp->w_marko < doto)
                                wp->w_markp = lp2;
                        else
                                wp->w_marko -= doto;
                }
                wp = wp->w_wndp;
        }       
        line_change(WFHARD);
        return (TRUE);
}

/*
 * This function deletes "n" bytes, starting at dot. It understands how do deal
 * with end of lines, etc. It returns TRUE if all of the characters were
 * deleted, and FALSE if they were not (because dot ran into the end of the
 * buffer. The "kflag" is TRUE if the text should be put in the kill buffer.
 */
line_delete(n, kflag)
{
        register LINE   *dotp;
        register int    doto;
        register int    chunk;
        register WINDOW *wp;

	if (curbp->b_flag & BFRDONLY)		/* if buffer is read-only */
	    return FALSE;			/* error		*/
        while (n != 0) {
                dotp = curwp->w_dotp;
                doto = curwp->w_doto;
                if (dotp == curbp->b_linep)     /* Hit end of buffer.   */
                        return (FALSE);
                chunk = dotp->l_used-doto;      /* Size of chunk.       */
                if (chunk > n)
                        chunk = n;
                if (chunk == 0) {               /* End of line, merge.  */
                        line_change(WFHARD);
                        if (line_delnewline() == FALSE
                        || (kflag!=FALSE && kill_appendchar('\n')==FALSE))
                                return (FALSE);
                        --n;
                        continue;
                }
                line_change(WFEDIT);
                if (kflag != FALSE) {           /* Kill?                */
		    if (!kill_appendstring(&dotp->l_text[doto],chunk))
			return FALSE;
                }
                dotp->l_used -= chunk;
		memmove(&dotp->l_text[doto],&dotp->l_text[doto + chunk],
		    dotp->l_used - doto);
                wp = wheadp;                    /* Fix windows          */
                while (wp != NULL) {
                        if (wp->w_dotp==dotp && wp->w_doto>=doto) {
                                wp->w_doto -= chunk;
                                if (wp->w_doto < doto)
                                        wp->w_doto = doto;
                        }       
                        if (wp->w_markp==dotp && wp->w_marko>=doto) {
                                wp->w_marko -= chunk;
                                if (wp->w_marko < doto)
                                        wp->w_marko = doto;
                        }
                        wp = wp->w_wndp;
                }
                n -= chunk;
        }
        return (TRUE);
}

/*
 * Delete a newline. Join the current line with the next line. If the next line
 * is the magic header line always return TRUE; merging the last line with the
 * header line can be thought of as always being a successful operation, even
 * if nothing is done, and this makes the kill buffer work "right". Easy cases
 * can be done by shuffling data around. Hard cases require that lines be moved
 * about in memory. Return FALSE on error and TRUE if all looks ok. Called by
 * "line_delete" only.
 */
line_delnewline()
{
        register LINE   *lp1;
        register LINE   *lp2;
        register LINE   *lp3;
        register WINDOW *wp;
	register int	lp1used;

	if (curbp->b_flag & BFRDONLY)		/* if buffer is read-only */
	    return FALSE;			/* error		*/
        lp1 = curwp->w_dotp;
        lp2 = lp1->l_fp;
	lp1used = lp1->l_used;
        if (lp2 == curbp->b_linep) {            /* At the buffer end.   */
                if (lp1used == 0)           /* Blank line.          */
                        line_free(lp1);
                return (TRUE);
        }
#if 0
	/* If enough extra space in lp1 to accommodate text in lp2	*/
        if (lp2->l_used <= lp1->l_size-lp1used)
	{
	    lp3 = lp1;
	    lp3->l_used += lp2->l_used;
        }
	else
#endif
	{
	    if ((lp3=line_realloc(lp1,lp1used+lp2->l_used)) == NULL)
                return (FALSE);
	    lp3->l_bp->l_fp = lp3;
	}

	memmove(&lp3->l_text[lp1used],lp2->l_text,lp2->l_used);
        lp3->l_fp = lp2->l_fp;
        lp3->l_fp->l_bp = lp3;

        free((char *) lp2);

        wp = wheadp;
        while (wp != NULL) {
                if (wp->w_linep==lp1 || wp->w_linep==lp2)
                        wp->w_linep = lp3;
                if (wp->w_dotp == lp1)
                        wp->w_dotp  = lp3;
                else if (wp->w_dotp == lp2) {
                        wp->w_dotp  = lp3;
                        wp->w_doto += lp1used;
                }
                if (wp->w_markp == lp1)
                        wp->w_markp  = lp3;
                else if (wp->w_markp == lp2) {
                        wp->w_markp  = lp3;
                        wp->w_marko += lp1used;
                }
                wp = wp->w_wndp;
        }
        return (TRUE);
}

/********************** KILL BUFFER STUFF *********************/

#define KBLOCK  256                     /* Kill buffer block size       */

typedef struct
{
    char    *buf;			/* Kill buffer data		*/
    int     used;			/* # of bytes used in KB	*/
    int     size;			/* # of bytes allocated in KB	*/
} killbuf_t;

static killbuf_t killbuffer[4];
static killbuf_t near *kbp = &killbuffer[0];	/* current kill buffer	*/

/************************************
 * Set the current kill buffer to i.
 */

kill_setbuffer(i)
int i;
{
    kbp = &killbuffer[i];
}

/*
 * Delete all of the text saved in the kill buffer. Called by commands when a
 * new kill context is being created. The kill buffer array is released, just
 * in case the buffer has grown to immense size. No errors.
 */
kill_freebuffer()
{
        if (kbp->buf != NULL) {
                free((char *) kbp->buf);
		kbp->buf = NULL;
		kbp->used = 0;
		kbp->size = 0;
        }
}

/*
 * Append a character to the kill buffer, enlarging the buffer if there isn't
 * any room. Always grow the buffer in chunks, on the assumption that if you
 * put something in the kill buffer you are going to put more stuff there too
 * later. Return TRUE if all is well, and FALSE on errors.
 */
kill_appendchar(c)
int c;
{
    if (kbp->used == kbp->size && !kill_setsize(kbp->size + KBLOCK))
	return (FALSE);
    kbp->buf[kbp->used++] = c;
    return (TRUE);
}

/********************************
 * Append string to kill buffer.
 */

int kill_appendstring(p,n)
char *p;
int n;
{
    if (kbp->used + n > kbp->size && !kill_setsize(kbp->used + n))
	return (FALSE);
    memcpy(kbp->buf + kbp->used,p,n);
    kbp->used += n;
    return TRUE;
}

/*
 * This function gets characters from the kill buffer. If the character index
 * "n" is off the end, it returns "-1". This lets the caller just scan along
 * until it gets a "-1" back.
 */
int kill_remove(n)
{
	return (n >= kbp->used) ? -1 : (kbp->buf[n] & 0xFF);
}

/********************************
 * We're going to use at least size bytes, so make room for it.
 * Returns:
 *	FALSE	out of memory
 */

int kill_setsize(size)
int size;
{   char *nbufp;

#if BSDUNIX || LINUX
    if (kbp->buf == NULL)
	nbufp = malloc(size);
    else
	nbufp = realloc(kbp->buf,size);
    if (nbufp == NULL)
#else
    if ((nbufp=realloc(kbp->buf,size)) == NULL)
#endif
	return (FALSE);
    kbp->buf  = nbufp;
    kbp->size = size;
    return TRUE;
}
