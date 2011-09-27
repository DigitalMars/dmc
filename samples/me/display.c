/*_ display.c   Fri Apr 14 1989 */
/*
 * The functions in this file handle redisplay. There are two halves, the
 * ones that update the virtual display screen, and the ones that make the
 * physical display screen the same as the virtual display screen. These
 * functions use hints that are left in the windows by the commands.
 *
 * REVISION HISTORY:
 *
 * ?    Steve Wilhite, 1-Dec-85
 *      - massive cleanup on code.
 */

#include        <stdio.h>
#include        <stdlib.h>
#include        <string.h>
#include	<stdarg.h>

#include        "ed.h"

#ifndef max
#define max(a,b)	((a) > (b) ? (a) : (b))
#endif

#define SHOWCONTROL	1

char column_mode = FALSE;

extern char *EMACSREV;
#define WFDEBUG 0                       /* Window flag debug. */
#if !(IBMPC || _WIN32)
static char *blnk_ln;
#endif

typedef struct  VIDEO {
        short   v_flag;                 /* Flags */
#if IBMPC || _WIN32
	unsigned short	v_text[1];	/* Screen data. */
#else
	char	v_text[1];		/* Screen data. */
#endif
}       VIDEO;

#define VFCHG   0x0001                  /* Changed. */

int display_recalc()
{
    register WINDOW *wp;

    for (wp = wheadp; wp; wp = wp->w_wndp)	/* for each window	*/
	wp->w_flag |= WFHARD | WFMODE;
    return TRUE;
}

#if IBMPC || _WIN32

/* Attribute flags	*/
#define MODEATTR config.modeattr	/* for mode line		*/
#define NORMATTR config.normattr	/* for normal text		*/
#define EOLATTR  config.eolattr		/* for end of line		*/
#define MARKATTR config.markattr	/* for selected text		*/

display_eol_bg()
{
    EOLATTR += 0x1000;
    return display_recalc();
}

display_norm_bg()
{
    NORMATTR += 0x1000;
    return display_recalc();
}

display_norm_fg()
{
    NORMATTR = (NORMATTR & 0xF0FF) + ((NORMATTR + 0x100) & 0xF00);
    return display_recalc();
}

display_mode_bg()
{
    MODEATTR += 0x1000;
    return display_recalc();
}

display_mode_fg()
{
    MODEATTR = (MODEATTR & 0xF0FF) + ((MODEATTR + 0x100) & 0xF00);
    return display_recalc();
}

display_mark_bg()
{
    MARKATTR += 0x1000;
    return display_recalc();
}

display_mark_fg()
{
    MARKATTR = (MARKATTR & 0xF0FF) + ((MARKATTR + 0x100) & 0xF00);
    return display_recalc();
}
#else
#define STANDATTR 0x80			/* Standout mode bit flag, or'ed in */
#define NORMATTR 0x00			/* Attribute for normal characters */
#define MODEATTR STANDATTR
#define EOLATTR  NORMATTR
#define MARKATTR STANDATTR
#endif

int	sgarbf	= TRUE;			/* TRUE if screen is garbage */
int	mpresf	= FALSE;		/* TRUE if message in last line */
int	vtrow	= 0;			/* Row location of SW cursor */
int	vtcol	= 0;			/* Column location of SW cursor */
int	ttrow	= HUGE;			/* Row location of HW cursor */
int	ttcol	= HUGE;			/* Column location of HW cursor */
int	attr;				/* Attribute for chars to vtputc() */
int	hardtabsize = 8;		// hardware tab size

VIDEO   **vscreen;                      /* Virtual screen. */
VIDEO   **pscreen;                      /* Physical screen. */

/*
 * Initialize the data structures used by the display code. The edge vectors
 * used to access the screens are set up. The operating system's terminal I/O
 * channel is set up. All the other things get initialized at compile time.
 * The original window has "WFCHG" set, so that it will get completely
 * redrawn on the first call to "update".
 */
vtinit()
{
    register int i;
    register VIDEO *vp;

    (*term.t_open)();
    vscreen = (VIDEO **) malloc(term.t_nrow*sizeof(VIDEO *));

    if (vscreen == NULL)
        exit(1);

    pscreen = (VIDEO **) malloc(term.t_nrow*sizeof(VIDEO *));
    if (pscreen == NULL)
        exit(1);

#if !(IBMPC || _WIN32)
    blnk_ln = malloc(term.t_ncol+1);
    if( blnk_ln == NULL )
	exit(1);
    memset(blnk_ln,' ',term.t_ncol);
    blnk_ln[term.t_ncol] = '\0';
#endif

    for (i = 0; i < term.t_nrow; ++i)
        {
	vp = (VIDEO *)
	    malloc(sizeof(VIDEO) + (term.t_ncol+1) * sizeof(vp->v_text[0]));

        if (vp == NULL)
            exit(1);

        vscreen[i] = vp;
	vp->v_text[term.t_ncol] = '\0';

	vp = (VIDEO *)
	    malloc(sizeof(VIDEO) + (term.t_ncol+1) * sizeof(vp->v_text[0]));
        if (vp == NULL)
            exit(1);
        pscreen[i] = vp;
	vp->v_text[term.t_ncol] = '\0';
        }
}

/*
 * Clean up the virtual terminal system, in anticipation for a return to the
 * operating system. Move down to the last line and clear it out (the next
 * system prompt will be written in the line). Shut down the channel to the
 * terminal.
 */
vttidy()
{   unsigned i;

    for (i = 0; i < term.t_nrow; ++i)
    {
	free(vscreen[i]);
	free(pscreen[i]);
    }
    free(vscreen);
    free(pscreen);

#if IBMPC
    movecursor(term.t_nrow - 1, 0);
    (*term.t_close)();
    printf("\n");			/* scroll up one line		*/
#else
    movecursor(term.t_nrow - 1, 0);
    (*term.t_putchar)('\n');		/* scroll up one line		*/
    (*term.t_close)();
#endif
}

/*
 * Set the virtual cursor to the specified row and column on the virtual
 * screen. There is no checking for nonsense values; this might be a good
 * idea during the early stages.
 */
vtmove(row, col)
{
    vtrow = row;
    vtcol = col;
}

/*
 * Write a character to the virtual screen. The virtual row and column are
 * updated. If the line is too long put a "+" in the last column. This routine
 * only puts printing characters into the virtual terminal buffers. Only
 * column overflow is checked.
 * Startcol is the starting column on the screen.
 */
vtputc(int c, int startcol)
{
    register VIDEO *vp;

    vp = vscreen[vtrow];

    if (vtcol - startcol >= term.t_ncol)
        vp->v_text[term.t_ncol - 1] = '+' | MODEATTR;
    else if (c == '\t')
    {   int i;

	i = hardtabsize - (vtcol % hardtabsize);
	do
            vtputc(config.tabchar,startcol);
	while (--i);
    }
#if SHOWCONTROL
    else if (c < 0x20 || c == 0x7F)
    {
        vtputc('^',startcol);
        vtputc(c ^ 0x40,startcol);
    }
#endif
    else
    {
	if (vtcol - startcol == 0 && startcol != 0)
            vp->v_text[0] = '+' | MODEATTR;
	else if (vtcol - startcol >= 0)
	    vp->v_text[vtcol - startcol] = c | attr;
	vtcol++;
    }
}

/****************************
 * Compute column number of line given index into that line.
 */

int getcol(LINE *dotp, int doto)
{
    return getcol2(dotp->l_text, doto);
}

int getcol2(char *dotp, int doto)
{   register int curcol,i;
    char c;

    curcol = 0;
    i = 0;

    while (i < doto)
    {
	c = dotp[i] & 0xFF;
	i++;

	if (c == '\t')
	{
	    if (hardtabsize == 8)
		curcol |= 7;
	    else
	    {
		curcol = ((curcol + hardtabsize) / hardtabsize) * hardtabsize - 1;
	    }
	}
#if SHOWCONTROL
	else if (c < 0x20 || c == 0x7F)
	    ++curcol;
#endif
	++curcol;
    }
    return curcol;
}

/******************************************
 * Inverse of getcol(), i.e. find offset into line that is closest
 * to or past column col.
 */

int coltodoto(lp,col)
struct LINE *lp;
int col;
{   int i;

    for (i = 0; i < llength(lp); i++)
	if (getcol(lp,i) >= col)
	    break;
    return i;
}

/***********************
 * Write a string to vtputc().
 */

static vtputs(p,startcol)
char *p;
int startcol;
{
	while (*p)
		vtputc(*p++,startcol);
}

/*
 * Erase from the end of the software cursor to the end of the line on which
 * the software cursor is located.
 */
vteeol(startcol)
int startcol;
{
#if IBMPC || _WIN32
    int i;

    for (i = max(vtcol - startcol,0); i < term.t_ncol; i++)
	vscreen[vtrow]->v_text[i] = EOLATTR | ' ';
#else
    memset(&(vscreen[vtrow]->v_text[max(vtcol - startcol,0)]),' ',term.t_ncol -
	max(vtcol - startcol,0));
#endif
    vtcol = startcol + term.t_ncol;
}

/*
 * Make sure that the display is right. This is a three part process. First,
 * scan through all of the windows looking for dirty ones. Check the framing,
 * and refresh the screen. Second, make sure that "currow" and "curcol" are
 * correct for the current window. Third, make the virtual and physical
 * screens the same.
 */

update()
{
    register LINE *lp;
    register WINDOW *wp;
    register VIDEO *vp1;
    register VIDEO *vp2;
    register int i;
    register int j;
    register int c;
    register int k;
    int l_first,l_last;
    int scroll_done_flag;
    int wcol;
    int curcol;				/* cursor column from left of text */
    char inmark;			/* if column marking, and in region */
#if MOUSE
    char hidden;
#endif

    if (ttkeysininput())		/* if more user input		*/
	return;				/* skip updating till caught up	*/

    curcol = getcol(curwp->w_dotp,curwp->w_doto);
    if ((lastflag&CFCPCN) == 0)		/* Reset goal if last		*/
	curgoal = curcol;		/* not backline() or forwline()	*/

    /* If cursor is off left or right side, set update bit so it'll scroll */
    if (curwp->w_startcol && curcol <= curwp->w_startcol ||
	curcol > curwp->w_startcol + term.t_ncol - 2)
	curwp->w_flag |= WFHARD;

    for (wp = wheadp; wp; wp = wp->w_wndp)	/* for each window	*/
    {
        /* Look at any window with update flags set on. */
        if (wp->w_flag != 0)
	{   char marking = wp->w_markp != NULL;
	    int col_left,col_right;		/* for column marking	*/

            /* If not force reframe, check the framing. */
            if ((wp->w_flag & WFFORCE) == 0)
	    {
                lp = wp->w_linep;	/* top line on screen		*/

                for (i = 0; i < wp->w_ntrows; ++i)
                    {
                    if (lp == wp->w_dotp)	/* if dot is on screen	*/
                        goto out;		/* no reframe necessary	*/

                    if (lp == wp->w_bufp->b_linep)
                        break;			/* reached end of buffer */

                    lp = lforw(lp);
                    }

		    /* if not on screen	*/
		    if (i == wp->w_ntrows)
		    {	if (lp == wp->w_dotp ||
			    wp->w_dotp == wp->w_bufp->b_linep)
			    wp->w_force = -1;	/* one up from bottom	*/
			else if (wp->w_dotp == lback(wp->w_linep))
			    wp->w_force = 1;	/* one before top	*/
		    }
            } /* if not force reframe */

	    /* A reframe is necessary.
             * Compute a new value for the line at the
             * top of the window. Then set the "WFHARD" flag to force full
             * redraw.
             */
            i = wp->w_force;

            if (i > 0)		/* if set dot to be on ith window line	*/
                {
                --i;

                if (i >= wp->w_ntrows)
                  i = wp->w_ntrows-1;	/* clip i to size of window	*/
                }
            else if (i < 0)	/* if set dot to be -ith line from bottom */
                {
                i += wp->w_ntrows;

                if (i < 0)
                    i = 0;	/* clip to top of screen		*/
                }
            else		/* set to center of screen		*/
                i = wp->w_ntrows/2;

            lp = wp->w_dotp;
            while (i != 0 && lback(lp) != wp->w_bufp->b_linep)
	    {
                --i;
                lp = lback(lp);
	    }

            wp->w_linep = lp;
            wp->w_flag |= WFHARD;       /* Force full. */

out:
	    /* Determine cursor column. If cursor is off the left or the  */
	    /* right, readjust starting column and do a WFHARD update	  */
	    wcol = getcol(wp->w_dotp,wp->w_doto);
	    if (wp->w_startcol && wcol <= wp->w_startcol)
	    {	wp->w_startcol = wcol ? wcol - 1 : 0;
		wp->w_flag |= WFHARD;
	    }
	    else if (wp->w_startcol < wcol - term.t_ncol + 2)
	    {	wp->w_startcol = wcol - term.t_ncol + 2;
		wp->w_flag |= WFHARD;
	    }

	    /* Determine if we should start out with a standout		  */
	    /* attribute or not, depends on if mark is before the window. */
	    attr = NORMATTR;
	    if (marking)
	    {
		inmark = FALSE;
		for (lp = lforw(wp->w_bufp->b_linep);
		     lp != wp->w_linep;
		     lp = lforw(lp))
			if (lp == wp->w_markp)
			{   inmark = TRUE;
			    break;
			}
		if (column_mode)
		{
		    /* Calculate left and right column numbers of region */
		    col_right = markcol;/*getcol(wp->w_markp,wp->w_marko);*/
		    if (curgoal <= col_right)
			col_left = curgoal;
		    else
		    {	col_left = col_right;
			col_right = curgoal;
		    }
		}
		else
		{   if (inmark)
		    {	attr = MARKATTR;
			inmark = FALSE;
		    }
		}
	    }

	    /* The window is framed properly now.
             * Try to use reduced update. Mode line update has its own special
             * flag. The fast update is used if the only thing to do is within
             * the line editing.
             */
            lp = wp->w_linep;		/* line of top of window	  */
            i = wp->w_toprow;		/* display row # of top of window */

	    if ((wp->w_flag & (WFFORCE | WFEDIT | WFHARD | WFMOVE)) == WFEDIT)
            {	/* Only need to update the line that the cursor is on	*/

		/* Determine row number and line pointer for cursor line */
                while (lp != wp->w_dotp)
                {   ++i;
                    lp = lforw(lp);
                }

                vscreen[i]->v_flag |= VFCHG;	/* assume this line will change */
                vtmove(i, 0);			/* start at beg of line	*/
                for (j = 0; 1; ++j)
		{
#if 0
		    if (marking)
		    {	if (wp->w_markp == lp && wp->w_marko == j)
				attr ^= NORMATTR ^ MARKATTR;
			if (wp->w_dotp == lp && wp->w_doto == j)
				attr ^= NORMATTR ^ MARKATTR;
		    }
#endif
		    if (j >= llength(lp))
			break;
                    vtputc(lgetc(lp, j),wp->w_startcol);
		}
                vteeol(wp->w_startcol);		/* clear remainder of line */
             }
             else if ((wp->w_flag & (WFEDIT | WFHARD)) != 0 ||
		      marking && wp->w_flag & WFMOVE)
	     {	/* update every line in the window	*/
                while (i < wp->w_toprow+wp->w_ntrows)
                {
                    vscreen[i]->v_flag |= VFCHG;
                    vtmove(i, 0);
                    if (lp != wp->w_bufp->b_linep) /* if not end of buffer */
                    {
			if (marking && column_mode)
			{   if (wp->w_markp == lp)
				inmark++;
			    if (wp->w_dotp == lp)
				inmark++;
			    attr = NORMATTR;
			}
                        for (j = 0; 1; ++j)
			{   if (marking)
			    {	if (column_mode)
				{
				    if (inmark && col_left <= vtcol)
					attr = MARKATTR;
				    if (col_right <= vtcol)
					attr = NORMATTR;
				}
				else
				{
				    if (wp->w_markp == lp && wp->w_marko == j)
					attr ^= NORMATTR ^ MARKATTR;
				    if (wp->w_dotp == lp && wp->w_doto == j)
					attr ^= NORMATTR ^ MARKATTR;
				}
			    }
			    if (j >= llength(lp))
				break;
                            vtputc(lgetc(lp, j),wp->w_startcol);
			}
			if (inmark == 2)
			    inmark = 0;
                        lp = lforw(lp);
                    }
                    vteeol(wp->w_startcol);
                    ++i;
                }
            }
#if !WFDEBUG
            if ((wp->w_flag&WFMODE) != 0)	/* if mode line is modified */
                modeline(wp);
            wp->w_flag  = 0;
            wp->w_force = 0;
#endif
        } /* if any update flags on */           
#if WFDEBUG
        modeline(wp);
        wp->w_flag =  0;
        wp->w_force = 0;
#endif
    } /* for each window */

    /* Always recompute the row and column number of the hardware cursor. This
     * is the only update for simple moves.
     */
    lp = curwp->w_linep;
    currow = curwp->w_toprow;

    while (lp != curwp->w_dotp)
        {
        ++currow;
        lp = lforw(lp);
        }

    /* Special hacking if the screen is garbage. Clear the hardware screen,
     * and update your copy to agree with it. Set all the virtual screen
     * change bits, to force a full update.
     */
#if MOUSE
    hidden = 0;
#endif

    if (sgarbf != FALSE)
    {
        for (i = 0; i < term.t_nrow; ++i)
	{   int j;

            vscreen[i]->v_flag |= VFCHG;
	    for (j = 0; j < term.t_ncol; j++)
		pscreen[i]->v_text[j] = NORMATTR * 256 + ' ';
	}

#if MOUSE
	if (!hidden && mouse)
	{   msm_hidecursor();
	    hidden++;
	}
#endif
	ttrow = HUGE;
	ttcol = HUGE;			// don't know where they are
        movecursor(0, 0);               /* Erase the screen. */
        (*term.t_eeop)();
        sgarbf = FALSE;                 /* Erase-page clears */
        mpresf = FALSE;                 /* the message area. */
    }

#if !(IBMPC || _WIN32)
    /* Here we check to see if we can scroll any of the lines.
     * This silly routine just checks for possibilites of scrolling
     * lines one line in either direction, but not multiple lines.
     */
    if( !term.t_scrollup ) goto no_scroll_possible;
    scroll_done_flag = 0;
    for (i = 0; i < term.t_nrow; i++)
    {
	if( (vscreen[i]->v_flag&VFCHG) != 0 )
	{
		/* if not first line					*/
		/* and current line is identical to previous line	*/
		/* and previous line is not blank			*/
		if( i > 0
		&& vscreen[i - 1]->v_flag & VFCHG
		&& strcmp(vscreen[i]->v_text,pscreen[i-1]->v_text) == 0
		&& strcmp(pscreen[i-1]->v_text,blnk_ln) != 0 )
		{
			/* Scroll screen down	*/
			l_first = i-1;	/* first line of scrolling region */
			while( i<term.t_nrow
			&& vscreen[i - 1]->v_flag & VFCHG
			&& strcmp(vscreen[i]->v_text,pscreen[i-1]->v_text)==0 )
				i++;
			l_last = i-1;	/* last line of scrolling region */
			(*term.t_scrolldn)( l_first, l_last );
			scroll_done_flag++;
			for( j = l_first+1; j < l_last+1; j++ )
				vscreen[j]->v_flag &= ~VFCHG;
			for( j = l_last; j > l_first; j-- )
				memcpy(pscreen[j]->v_text,
				       pscreen[j - 1]->v_text,term.t_ncol);
			memset(pscreen[l_first]->v_text,' ',term.t_ncol);

			/* Set change flag on last line to get rid of	*/
			/* bug that caused lines to 'vanish'.		*/
			vscreen[l_first]->v_flag |= VFCHG;
		}
		else if( i < term.t_nrow-1
		&& vscreen[i + 1]->v_flag & VFCHG
		&& strcmp(vscreen[i]->v_text,pscreen[i+1]->v_text) == 0
		&& strcmp(pscreen[i+1]->v_text,blnk_ln) != 0 )
		{
			l_first = i;
			while( i<term.t_nrow-1
			&& vscreen[i + 1]->v_flag & VFCHG
			&& strcmp(vscreen[i]->v_text,pscreen[i+1]->v_text)==0 )
				i++;
			l_last = i;
			(*term.t_scrollup)( l_first, l_last );
			scroll_done_flag++;
			for( j = l_first; j < l_last; j++ )
				vscreen[j]->v_flag &= ~VFCHG;
			for( j = l_first; j < l_last; j++ )
				memcpy(pscreen[j]->v_text,
				       pscreen[j + 1]->v_text,term.t_ncol);
			memset(pscreen[l_last]->v_text,' ',term.t_ncol);

			/* Set change flag on last line to get rid of	*/
			/* bug that caused lines to 'vanish'.		*/
			vscreen[l_last]->v_flag |= VFCHG;
		}
	}
    }
    if( scroll_done_flag )
    {
	ttrow = ttrow-1;	/* force a change */
	movecursor(currow, curcol - curwp->w_startcol);
    }

  no_scroll_possible:
#endif
    /* Make sure that the physical and virtual displays agree. Unlike before,
     * the "updateline" code is only called with a line that has been updated
     * for sure.
     */
    for (i = 0; i < term.t_nrow; ++i)
    {
        vp1 = vscreen[i];

        if ((vp1->v_flag&VFCHG) != 0)
	{
#if MOUSE
	    if (!hidden && mouse)
	    {	msm_hidecursor();
		hidden++;
	    }
#endif
            vp1->v_flag &= ~VFCHG;
	    vp2 = pscreen[i];
	    updateline(i, &vp1->v_text[0], &vp2->v_text[0]);
	}
    }

    /* Finally, update the hardware cursor and flush out buffers. */

#if IBMPC || _WIN32
    (*term.t_move)(currow,curcol - curwp->w_startcol);	/* putline() trashed the cursor pos */
    ttrow = currow;
    ttcol = curcol - curwp->w_startcol;
#else
    movecursor(currow, curcol - curwp->w_startcol);
#endif
    (*term.t_flush)();
#if MOUSE
    if (hidden && mouse)
	msm_showcursor();
#endif
}

/*
 * Update a single line. This does not know how to use insert or delete
 * character sequences; we are using VT52 functionality. Update the physical
 * row and column variables. It does try an exploit erase to end of line. The
 * RAINBOW version of this routine uses fast video.
 */
#if !IBMPC && !_WIN32
updateline(row, vline, pline)
    char vline[];
    char pline[];
{
#if RAINBOW
    register char *cp1;
    register char *cp2;
    register int nch;
    int n;

    cp1 = &vline[0];                    /* Use fast video. */
    cp2 = &pline[0];
    n = strlen(cp1);
    if (n > term.t_ncol)
        n = term.t_ncol;
    Put_Data(row+1, 1, n, cp1);

    nch = term.t_ncol;
    do
        {
        *cp2 = *cp1;
        ++cp2;
        ++cp1;
        }
    while (--nch);
#else
    register char *cp1;
    register char *cp2;
    register char *cp3;
    register char *cp4;
    register char *cp5;
    register int nbflag;
    static int tstand = FALSE;		/* TRUE if standout mode is active */

    cp1 = &vline[0];                    /* Compute left match.  */
    cp2 = &pline[0];

    while (cp1!=&vline[term.t_ncol] && cp1[0]==cp2[0])
        {
        ++cp1;
        ++cp2;
        }

    /* This can still happen, even though we only call this routine on changed
     * lines. A hard update is always done when a line splits, a massive
     * change is done, or a buffer is displayed twice. This optimizes out most
     * of the excess updating. A lot of computes are used, but these tend to
     * be hard operations that do a lot of update, so I don't really care.
     */
    if (cp1 == &vline[term.t_ncol])             /* All equal. */
        return;

    nbflag = FALSE;
    cp3 = &vline[term.t_ncol];          /* Compute right match. */
    cp4 = &pline[term.t_ncol];

    while (cp3[-1] == cp4[-1])
        {
        --cp3;
        --cp4;
        if (cp3[0] != ' ')              /* Note if any nonblank */
            nbflag = TRUE;              /* in right match. */
        }

    cp5 = cp3;

    if (nbflag == FALSE)                /* Erase to EOL ? */
        {
        while (cp5!=cp1 && cp5[-1]==' ')
            --cp5;

        if (cp3-cp5 <= 3)               /* Use only if erase is */
            cp5 = cp3;                  /* fewer characters. */
        }

    movecursor(row, cp1-&vline[0]);     /* Go to start of line. */

    while (cp1 != cp5)                  /* Ordinary. */
    {
	if( *cp1 & STANDATTR )
	{	if( !tstand ) {	(*term.t_standout)(); tstand = TRUE;	} }
	else
	{	if(  tstand ) {	(*term.t_standend)(); tstand = FALSE;	} }
        (*term.t_putchar)(*cp1 & ~STANDATTR);
        ++ttcol;
        *cp2++ = *cp1++;
    }

    if (cp5 != cp3)                     /* Erase. */
        {
        (*term.t_eeol)();
        while (cp1 != cp3)
            *cp2++ = *cp1++;
        }
	if( tstand )
	{	(*term.t_standend)();
		tstand = FALSE;
	}
#endif
}
#endif /* !IBMPC */

/*
 * Redisplay the mode line for the window pointed to by the "wp". This is the
 * only routine that has any idea of how the modeline is formatted. You can
 * change the modeline format by hacking at this routine. Called by "update"
 * any time there is a dirty window.
 */
modeline(wp)
    WINDOW *wp;
{
    register char *cp;
    register int c;
    register int n;
    register BUFFER *bp;

    n = wp->w_toprow+wp->w_ntrows;              /* Location. */
    vscreen[n]->v_flag |= VFCHG;                /* Redraw next time. */
    vtmove(n, 0);                               /* Seek to right line. */
    attr = MODEATTR;
    bp = wp->w_bufp;

    if (bp->b_flag & BFRDONLY)
	vtputc('R',0);
    else if ((bp->b_flag&BFCHG) != 0)                /* "*" if changed. */
        vtputc('*',0);
    else
        vtputc('-',0);

    vtputs(EMACSREV,0);
    vtputc(' ', 0);

    if (filcmp(bp->b_bname,bp->b_fname))
    {	vtputs("-- Buffer: ",0);
	vtputs(bp->b_bname,0);
	vtputc(' ',0);
    }
    if (bp->b_fname[0] != 0)            /* File name. */
    {
	vtputs("-- File: ",0);
	vtputs(bp->b_fname,0);
        vtputc(' ',0);
    }

#if WFDEBUG
    vtputc('-',0);
    vtputc((wp->w_flag&WFMODE)!=0  ? 'M' : '-',0);
    vtputc((wp->w_flag&WFHARD)!=0  ? 'H' : '-',0);
    vtputc((wp->w_flag&WFEDIT)!=0  ? 'E' : '-',0);
    vtputc((wp->w_flag&WFMOVE)!=0  ? 'V' : '-',0);
    vtputc((wp->w_flag&WFFORCE)!=0 ? 'F' : '-',0);
#endif

    while (vtcol < term.t_ncol)             /* Pad to full width. */
        vtputc('-',0);
}

/*
 * Send a command to the terminal to move the hardware cursor to row "row"
 * and column "col". The row and column arguments are origin 0. Optimize out
 * random calls. Update "ttrow" and "ttcol".
 */
movecursor(row, col)
    {
    if (row!=ttrow || col!=ttcol)
        {
        ttrow = row;
        ttcol = col;
        (*term.t_move)(row, col);
        }
    }


/*
 * Erase the message line. This is a special routine because the message line
 * is not considered to be part of the virtual screen. It always works
 * immediately; the terminal buffer is flushed via a call to the flusher.
 */
mlerase()
{
    int i;
    register VIDEO *vp;

    vp = vscreen[term.t_nrow - 1];
    for (i = 0; i < term.t_ncol; i++)
	vp->v_text[i] = EOLATTR | ' ';
    vp->v_flag |= VFCHG;
    mpresf = FALSE;
}

/*
 * Ask a yes or no question in the message line. Return either TRUE, FALSE, or
 * ABORT. The ABORT status is returned if the user bumps out of the question
 * with a ^G. Used any time a confirmation is required.
 */
mlyesno(prompt)
    char *prompt;
    {
    register int s;
    char buf[64];

    for (;;)
        {
	buf[0] = 0;
        s = mlreply(prompt, buf, sizeof(buf));

        if (s == ABORT)
            return (ABORT);

        if (s != FALSE)
            {
            if (buf[0]=='y' || buf[0]=='Y')
                return (TRUE);

            if (buf[0]=='n' || buf[0]=='N')
                return (FALSE);
            }
        }
    }


/***********************************
 * Simple circular history buffer for message line.
 */

#define HISTORY_MAX 10
char *history[HISTORY_MAX];
int history_top;

#define HDEC(hi)	((hi == 0) ? HISTORY_MAX - 1 : hi - 1)
#define HINC(hi)	((hi == HISTORY_MAX - 1) ? 0 : hi + 1)

/*
 * Write a prompt into the message line, then read back a response. Keep
 * track of the physical position of the cursor. If we are in a keyboard
 * macro throw the prompt away, and return the remembered response. This
 * lets macros run at full speed. The reply is always terminated by a carriage
 * return. Handle erase, kill, and abort keys.
 */
mlreply(char *prompt, char *buf, unsigned maxchars)
{
    int dot;		// insertion point in buffer
    int buflen;		// number of characters in buffer
    int promptlen;
    int startcol;
    int changes;
    int hi;

    int i;
    int c;

    if (kbdmop != NULL)
    {
	strcpy(buf,(char *)kbdmop);
	kbdmop += strlen(buf) + 1;
	return (buf[0] != 0);
    }

    hi = history_top;
    startcol = 0;
    attr = NORMATTR;
    changes = 1;

    mpresf = TRUE;

    promptlen = strlen(prompt);
    buflen = strlen(buf);
    dot = buflen;

    for (;;)
    {
	if (changes)
	{
	    int col;

	    buf[buflen] = 0;
	    col = promptlen + getcol2(buf, dot);
	    if (col >= startcol + term.t_ncol - 2)
		startcol = col - term.t_ncol + 2;
	    if (col < startcol + promptlen)
		startcol = col - promptlen;

	    vtmove(term.t_nrow - 1, 0);
	    vtputs(prompt,startcol);
	    vtputs(buf,startcol);
	    vteeol(startcol);
	    mlchange();
	    update();
	    attr = NORMATTR;
	    vtmove(term.t_nrow - 1, col);

	    changes = 0;
	}

	movecursor(vtrow, vtcol - startcol);
	(*term.t_flush)();
        c = (*term.t_getchar)();

        switch (c)
	{   case 0x0D:                  /* Return, end of line */
		buf[buflen] = 0;
                if (kbdmip != NULL)
		{
                    if (kbdmip + buflen + 1 > &kbdm[NKBDM-3])
			goto err;	/* error	*/

		    memcpy(kbdmip, buf, buflen + 1);
		    kbdmip += buflen + 1;
		}
		if (buflen != 0)
		{
		    hi = HDEC(history_top);
		    if (!history[hi] || strcmp(buf, history[hi]) != 0)
		    {
			// Store in history buffer
			history[history_top] = strdup(buf);
			history_top = HINC(history_top);
			if (history[history_top] != NULL)
			{   free(history[history_top]);
			    history[history_top] = NULL;
			}
		    }
		    return 1;
		}
		return 0;

            case 0x07:                  /* Bell, abort */
                vtputc(7, startcol);
		mlchange();
		goto err;		/* error	*/

	    case 0x01:			// ^A, beginning of line
	    case HOMEKEY:
                if (dot != 0)
		{
		    dot = 0;
		    startcol = 0;
		    changes = 1;
		}
                break;

	    case 0x05:			// ^E, beginning of line
	    case ENDKEY:
                if (dot != buflen)
		{
		    dot = buflen;
		    changes = 1;
		}
                break;

	    case 0x0B:			// ^K, delete to end of line
		if (dot != buflen)
		{
		    buflen = dot;
		    changes = 1;
		}
		break;

            case 0x7F:                  /* Rubout, erase */
            case 0x08:                  /* Backspace, erase */
                if (dot != 0)
		{
		    memmove(buf + dot - 1, buf + dot, buflen - dot);
		    --dot;
		    --buflen;
		    changes = 1;
		}
                break;

	    case DelKEY:
                if (dot < buflen)
		{
		    memmove(buf + dot, buf + dot + 1, buflen - dot - 1);
		    --buflen;
		    changes = 1;
		}
                break;


            case 0x15:                  // ^U means delete line
		dot = 0;
		buflen = 0;
		startcol = 0;
		changes = 1;
                break;

	    case 'Y' - 0x40:		/* ^Y means yank		*/
		{   int n;

		    for (n = 0; (c = kill_remove(n)) != -1; n++)
		    {
	                if (buflen < maxchars-1)
			{
			    memmove(buf + dot + 1, buf + dot, buflen - dot);
			    buflen++;
	                    buf[dot++] = c;
			}
		    }
		    changes = 1;
		}
		break;

	    case LTKEY:
		if (dot != 0)
		{
		    dot--;
		    changes = 1;
		}
                break;

	    case RTKEY:
		if (dot < buflen)
		{
		    dot++;
		    changes = 1;
		}
                break;

	    case UPKEY:
		i = HDEC(hi);
		if (hi == history_top && history[i] && strcmp(buf, history[i]) == 0)
		    i = HDEC(i);
		goto L1;

	    case DNKEY:
		i = HINC(hi);
	    L1:
		if (history[i])
		{   int len = strlen(history[i]);
		    if (len >= maxchars)
			len = maxchars - 1;
		    memcpy(buf, history[i], len);
		    buflen = len;
		    dot = len;
		    startcol = 0;
		    changes = 1;
		    hi = i;
		}
		else
		    ctrlg(FALSE, 0);
		break;

#if 0
	    case InsKEY:
#endif

	    case 0x11:			/* ^Q, quote next		*/
	        c = (*term.t_getchar)();
            default:
		if (c < 0 || c >= 0x7F)
		{   // Error
		    ctrlg(FALSE, 0);
		}
                else if (buflen < maxchars-1)
		{
		    memmove(buf + dot + 1, buf + dot, buflen - dot);
		    buflen++;
                    buf[dot++] = c;
		    changes = 1;
		}
		break;
            }
    }

err:
    ctrlg(FALSE, 0);
    return (ABORT);
}

/*
 * Write a message into the message line. Keep track of the physical cursor
 * position. printf like format items are handled.
 * Set the "message line" flag TRUE.
 */
void mlwrite(const char *fmt, ...)
{
    int c;
    char buffer[80+1];
    char *p;
    va_list ap;
    int savecol;

    va_start(ap, fmt);
    vsprintf(buffer, fmt, ap);
    va_end(ap);

    vtmove(term.t_nrow - 1, 0);
    attr = NORMATTR;
    vtputs(buffer,0);

    savecol = vtcol;
    vteeol(0);
    vtcol = savecol;
    mlchange();
    mpresf = TRUE;
}


mlchange()
{
    VIDEO *vp;

    vp = vscreen[term.t_nrow - 1];
    vp->v_flag |= VFCHG;
}
