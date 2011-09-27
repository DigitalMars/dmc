/*_ region.c   Mon Apr 17 1989 */
/*
 * The routines in this file
 * deal with the region, that magic space
 * between "." and mark. Some functions are
 * commands. Some functions are just for
 * internal use.
 */
#include        <stdio.h>
#include	<ctype.h>
#include        "ed.h"

/*********************************
 * Toggle column cut/paste mode.
 */

int region_togglemode(f, n)
{
    column_mode ^= 1;			/* toggle screen mode		*/
    line_change(WFHARD);
    mlwrite(column_mode ? "[Column mode on]" : "[Column mode off]");
    return TRUE;
}

/*
 * Kill the region. Ask "getregion"
 * to figure out the bounds of the region.
 * Move "." to the start, and kill the characters.
 */
region_kill(f, n)
{
        register int    s;
        REGION          region;

	if (curbp->b_flag & BFRDONLY)
	    return FALSE;
	if ((s=getregion(&region)) != TRUE)
		goto err;
	if (region.r_size == 0)
		goto err;			/* error if 0 length	*/
	if ((lastflag&CFKILL) == 0)             /* This is a kill type  */
		kill_freebuffer();		/* command, so do magic */
        thisflag |= CFKILL;                     /* kill buffer stuff.   */
        curwp->w_dotp = region.r_linep;
        curwp->w_doto = region.r_offset;
	curwp->w_markp = NULL;
	if (column_mode)
	{
	    if (!kill_setsize(region.r_size))
		return FALSE;
	    while (region.r_nlines--)
	    {	int lright;
		register LINE *linep = curwp->w_dotp;

		curwp->w_doto = coltodoto(linep,region.r_leftcol);
		lright = coltodoto(linep,region.r_rightcol);
		if (!line_delete(lright - curwp->w_doto, TRUE))
		    goto err;
		if (!kill_appendchar('\n'))
		    goto err;
		curwp->w_dotp = lforw(linep);
	    }
	    curwp->w_dotp = region.r_linep;
	    curwp->w_doto = coltodoto(region.r_linep,region.r_leftcol);
	    line_change(WFHARD);
	}
	else
	    s = line_delete(region.r_size, TRUE);
	return s;
err:
	return FALSE;
}

/*
 * Copy all of the characters in the
 * region to the kill buffer. Don't move dot
 * at all. This is a bit like a kill region followed
 * by a yank. Bound to "M-W".
 */
region_copy(f, n)
{
        register LINE   *linep;
        register int    loffs;
        register int    s;
        REGION          region;

        if ((s=getregion(&region)) != TRUE)
                return (s);
        if ((lastflag&CFKILL) == 0)             /* Kill type command.   */
                kill_freebuffer();

	/* Turn off marked region */
	curwp->w_markp = NULL;
	curwp->w_flag |= WFHARD;

        thisflag |= CFKILL;
        linep = region.r_linep;                 /* Current line.        */
        loffs = region.r_offset;                /* Current offset.      */
	if (!kill_setsize(region.r_size))
	    return FALSE;
	if (column_mode)
	{
	    while (region.r_nlines--)
	    {	int lright;
		int lleft;

		lleft  = coltodoto(linep,region.r_leftcol);
		lright = coltodoto(linep,region.r_rightcol);
		if (!kill_appendstring(linep->l_text + lleft,lright - lleft))
		    goto err;
		if (!kill_appendchar('\n'))
		    goto err;
		linep = lforw(linep);
	    }
	}
	else
	    while (region.r_size) {
                if (loffs == llength(linep)) {  /* End of line.         */
                        if (kill_appendchar('\n') != TRUE)
                                return FALSE;
                        linep = lforw(linep);
                        loffs = 0;
			region.r_size--; 
               } else {                        /* Middle of line.      */
			int i;

			i = llength(linep) - loffs;
			if (i > region.r_size)
			    i = region.r_size;
                        if (kill_appendstring(linep->l_text + loffs, i) != TRUE)
			    return FALSE;
                        loffs += i;
			region.r_size -= i;
                }
        }
        return (TRUE);

err:	return FALSE;
}

/*
 * Upper/lower case region. Zap all of the upper
 * case characters in the region to lower case. Use
 * the region code to set the limits. Scan the buffer,
 * doing the changes. Call "line_change" to ensure that
 * redisplay is done in all buffers.
 */
region_lower(f, n)
{
	return region_case(TRUE);
}

region_upper(f, n)
{
	return region_case(FALSE);
}

static int region_case(flag)
int flag;
{
        register LINE   *linep;
        register int    loffs;
        register int    c;
        register int    s;
        REGION          region;

        if ((s=getregion(&region)) != TRUE)
                return (s);
	line_change(WFHARD);
	/*curwp->w_markp = NULL;*/
        linep = region.r_linep;
	if (column_mode)
	{
	    while (region.r_nlines--)
	    {	int lright;

		loffs = coltodoto(linep,region.r_leftcol);
		lright = coltodoto(linep,region.r_rightcol);
		for (; loffs < lright; loffs++)
		{   c = lgetc(linep, loffs);
		    if (flag ? isupper(c) : islower(c))
			lputc(linep, loffs, c ^ 0x20);
		}
		linep = lforw(linep);
	    }
	}
	else
	{
	    loffs = region.r_offset;
	    while (region.r_size--) {
		if (loffs == llength(linep)) {
		    linep = lforw(linep);
		    loffs = 0;
		} else {
		    c = lgetc(linep, loffs);
		    if (flag ? isupper(c) : islower(c))
			lputc(linep, loffs, c ^ 0x20);
		    ++loffs;
		}
	    }
	}
        return (TRUE);
}

/*
 * This routine figures out the
 * bounds of the region in the current window, and
 * fills in the fields of the "REGION" structure pointed
 * to by "rp". Because the dot and mark are usually very
 * close together, we scan outward from dot looking for
 * mark. This should save time. Return a standard code.
 * Callers of this routine should be prepared to get
 * an "ABORT" status; we might make this have the
 * conform thing later.
 */
int getregion(rp)
register REGION *rp;
{
        register LINE   *flp;
        register LINE   *blp;
	register int	nlines;		/* number of lines in region	*/
	register long	fsize;
	register long	bsize;
	register long	size;

        if (!window_marking(curwp)) {
                mlwrite("No mark set in this window");
                return (FALSE);
        }

	/* Figure out left and right columns, this is valid only if	*/
	/* column cut mode is on.					*/
	if (markcol < curgoal)
	{   rp->r_leftcol  = markcol;
	    rp->r_rightcol = curgoal;
	}
	else
	{   rp->r_leftcol  = curgoal;
	    rp->r_rightcol = markcol;
	}

	rp->r_nlines = 1;		/* always at least 1 line	*/

	/* If region lies within one line	*/
        if (curwp->w_dotp == curwp->w_markp)
	{   rp->r_linep = curwp->w_dotp;
	    if (column_mode)
	    {
		rp->r_size = rp->r_rightcol - rp->r_leftcol;
	    }
	    else
	    {    
		if (curwp->w_doto < curwp->w_marko)
		{   rp->r_offset = curwp->w_doto;
		    rp->r_size = curwp->w_marko-curwp->w_doto;
                }
		else
		{   rp->r_offset = curwp->w_marko;
		    rp->r_size = curwp->w_doto-curwp->w_marko;
                }
	    }
	    return (TRUE);
        }

        blp = curwp->w_dotp;
        bsize = curwp->w_doto;
        flp = curwp->w_dotp;
        fsize = llength(flp)-curwp->w_doto+1;
        while (flp!=curbp->b_linep || lback(blp)!=curbp->b_linep)
	{
		rp->r_nlines++;
                if (flp != curbp->b_linep) {
                        flp = lforw(flp);
                        if (flp == curwp->w_markp) {
                                rp->r_linep = curwp->w_dotp;
                                rp->r_offset = curwp->w_doto;
                                size = fsize+curwp->w_marko;
				/* Don't count last line if it's at start */
				if (curwp->w_marko == 0)
				    rp->r_nlines--;
				goto done;
                        }
                        fsize += llength(flp)+1;
                }
                if (lback(blp) != curbp->b_linep) {
                        blp = lback(blp);
                        bsize += llength(blp)+1;
                        if (blp == curwp->w_markp) {
                                rp->r_linep = blp;
                                rp->r_offset = curwp->w_marko;
                                size = bsize - curwp->w_marko;
				/* Don't count last line if it's at start */
				if (curwp->w_doto == 0)
				    rp->r_nlines--;
				goto done;
                        }
                }
        }
        mlwrite("Bug: lost mark");
        return (FALSE);

done:
	if (column_mode)
	    size = (long) (rp->r_rightcol - rp->r_leftcol + 1) * rp->r_nlines;
#if IBMPC && __INTSIZE == 2
	if (size > 0xFE00)
	{   mlwrite("Too large");
	    return FALSE;
	}
#endif
	rp->r_size = size;
	return TRUE;
}
