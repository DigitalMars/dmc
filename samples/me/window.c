/*_ window.c   Wed Dec 18 1985 */
/*
 * Window management. Some of the functions are internal, and some are
 * attached to keys that the user actually types.
 */

#include        <stdio.h>
#include        "ed.h"

/*************************************
 * Reposition dot in the current window to line "n". If the argument is
 * positive, it is that line. If it is negative it is that line from the
 * bottom. If it is 0 the window is centered (this is what the standard
 * redisplay code does). With no argument it defaults to 1. Bound to M-!.
 * Because of the default, it works like in Gosling.
 */

window_reposition(f, n)
    {
    curwp->w_force = n;
    curwp->w_flag |= WFFORCE;
    return (TRUE);
    }

/*
 * Refresh the screen. With no argument, it just does the refresh. With an
 * argument it recenters "." in the current window. Bound to "C-L".
 */
window_refresh(f, n)
    {
    if (f == FALSE)
        sgarbf = TRUE;
    else
        {
        curwp->w_force = 0;             /* Center dot. */
        curwp->w_flag |= WFFORCE;
        }

    return (TRUE);
    }

/*
 * The command make the next window (next => down the screen) the current
 * window. There are no real errors, although the command does nothing if
 * there is only 1 window on the screen. Bound to "C-X C-N".
 */
window_next(f, n)
    {
    register WINDOW *wp;

    if ((wp = curwp->w_wndp) == NULL)
        wp = wheadp;

    curwp = wp;
    curbp = wp->w_bufp;
    return (TRUE);
    }

/*
 * This command makes the previous window (previous => up the screen) the
 * current window. There arn't any errors, although the command does not do a
 * lot if there is 1 window.
 */
window_prev(f, n)
    {
    register WINDOW *wp1;
    register WINDOW *wp2;

    wp1 = wheadp;
    wp2 = curwp;

    if (wp1 == wp2)
        wp2 = NULL;

    while (wp1->w_wndp != wp2)
        wp1 = wp1->w_wndp;

    curwp = wp1;
    curbp = wp1->w_bufp;
    return (TRUE);
    }

/*
 * This command moves the current window down by "arg" lines. Recompute the
 * top line in the window. The move up and move down code is almost completely
 * the same; most of the work has to do with reframing the window, and picking
 * a new dot. We share the code by having "move down" just be an interface to
 * "move up". Magic. Bound to "C-X C-N".
 */
window_mvdn(f, n)
    int n;
    {
    return window_mvup(f, -n);
    }

/*
 * Move the current window up by "arg" lines. Recompute the new top line of
 * the window. Look to see if "." is still on the screen. If it is, you win.
 * If it isn't, then move "." to center it in the new framing of the window
 * (this command does not really move "."; it moves the frame). Bound to
 * "C-X C-P".
 */
window_mvup(f, n)
    int n;
    {
    register LINE *lp;
    register int i;

    lp = curwp->w_linep;

    if (n < 0)
        {
        while (n++ && lp!=curbp->b_linep)
            lp = lforw(lp);
        }
    else
        {
        while (n-- && lback(lp)!=curbp->b_linep)
            lp = lback(lp);
        }

    curwp->w_linep = lp;		/* new top line of window	*/
    curwp->w_flag |= WFHARD;            /* Mode line is OK. */

    for (i = 0; i < curwp->w_ntrows; ++i)
        {
        if (lp == curwp->w_dotp)
            return (TRUE);
        if (lp == curbp->b_linep)
            break;
        lp = lforw(lp);
        }

    lp = curwp->w_linep;
    i  = curwp->w_ntrows/2;

    while (i-- && lp != curbp->b_linep)
        lp = lforw(lp);

    curwp->w_dotp  = lp;
    curwp->w_doto  = 0;
    return (TRUE);
    }

/*
 * This command makes the current window the only window on the screen. Bound
 * to "C-X 1". Try to set the framing so that "." does not have to move on the
 * display. Some care has to be taken to keep the values of dot and mark in
 * the buffer structures right if the destruction of a window makes a buffer
 * become undisplayed.
 */
window_only(f, n)
{
        register WINDOW *wp;
        register LINE   *lp;
        register int    i;

        while (wheadp != curwp) {
                wp = wheadp;
                wheadp = wp->w_wndp;
                if (--wp->w_bufp->b_nwnd == 0) {
                        wp->w_bufp->b_dotp  = wp->w_dotp;
                        wp->w_bufp->b_doto  = wp->w_doto;
                        wp->w_bufp->b_markp = wp->w_markp;
                        wp->w_bufp->b_marko = wp->w_marko;
                }
                free((char *) wp);
        }
        while (curwp->w_wndp != NULL) {
                wp = curwp->w_wndp;
                curwp->w_wndp = wp->w_wndp;
                if (--wp->w_bufp->b_nwnd == 0) {
                        wp->w_bufp->b_dotp  = wp->w_dotp;
                        wp->w_bufp->b_doto  = wp->w_doto;
                        wp->w_bufp->b_markp = wp->w_markp;
                        wp->w_bufp->b_marko = wp->w_marko;
                }
                free((char *) wp);
        }
        lp = curwp->w_linep;
        i  = curwp->w_toprow;
        while (i!=0 && lback(lp)!=curbp->b_linep) {
                --i;
                lp = lback(lp);
        }
        curwp->w_toprow = 0;
        curwp->w_ntrows = term.t_nrow - 2;
        curwp->w_linep  = lp;
        curwp->w_flag  |= WFMODE|WFHARD;
        return (TRUE);
}

/*
 * Split the current window. A window smaller than 3 lines cannot be split.
 * The only other error that is possible is a "malloc" failure allocating the
 * structure for the new window. Bound to "C-X 2".
 */
window_split(f, n)
{
        register WINDOW *wp;
        register LINE   *lp;
        register int    ntru;
        register int    ntrl;
        register int    ntrd;
        register WINDOW *wp1;
        register WINDOW *wp2;

        if (curwp->w_ntrows < 3) {
                mlwrite("Cannot split a %d line window", curwp->w_ntrows);
                return (FALSE);
        }
        if ((wp = (WINDOW *) calloc(sizeof(WINDOW),1)) == NULL) {
                mlwrite("Cannot allocate WINDOW block");
                return (FALSE);
        }
        ++curbp->b_nwnd;                        /* Displayed twice.     */
        wp->w_bufp  = curbp;
        wp->w_dotp  = curwp->w_dotp;
        wp->w_doto  = curwp->w_doto;
        wp->w_markp = curwp->w_markp;
        wp->w_marko = curwp->w_marko;
        ntru = (curwp->w_ntrows-1) / 2;         /* Upper size           */
        ntrl = (curwp->w_ntrows-1) - ntru;      /* Lower size           */
        lp = curwp->w_linep;
        ntrd = 0;
        while (lp != curwp->w_dotp) {
                ++ntrd;
                lp = lforw(lp);
        }
        lp = curwp->w_linep;
        if (ntrd <= ntru) {                     /* Old is upper window. */
                if (ntrd == ntru)               /* Hit mode line.       */
                        lp = lforw(lp);
                curwp->w_ntrows = ntru;
                wp->w_wndp = curwp->w_wndp;
                curwp->w_wndp = wp;
                wp->w_toprow = curwp->w_toprow+ntru+1;
                wp->w_ntrows = ntrl;
        } else {                                /* Old is lower window  */
                wp1 = NULL;
                wp2 = wheadp;
                while (wp2 != curwp) {
                        wp1 = wp2;
                        wp2 = wp2->w_wndp;
                }
                if (wp1 == NULL)
                        wheadp = wp;
                else
                        wp1->w_wndp = wp;
                wp->w_wndp   = curwp;
                wp->w_toprow = curwp->w_toprow;
                wp->w_ntrows = ntru;
                ++ntru;                         /* Mode line.           */
                curwp->w_toprow += ntru;
                curwp->w_ntrows  = ntrl;
                while (ntru--)
                        lp = lforw(lp);
        }
        curwp->w_linep = lp;                    /* Adjust the top lines */
        wp->w_linep = lp;                       /* if necessary.        */
        curwp->w_flag |= WFMODE|WFHARD;
        wp->w_flag |= WFMODE|WFHARD;
        return (TRUE);
}

/*
 * Enlarge the current window. Find the window that loses space. Make sure it
 * is big enough. If so, hack the window descriptions, and ask redisplay to do
 * all the hard work. You don't just set "force reframe" because dot would
 * move. Bound to "C-X Z".
 */
window_enlarge(f, n)
{
        register WINDOW *adjwp;
        register LINE   *lp;
        register int    i;

        if (n < 0)
                return (window_shrink(f, -n));
        if (wheadp->w_wndp == NULL) {
                mlwrite("Only one window");
                return (FALSE);
        }
        if ((adjwp=curwp->w_wndp) == NULL) {
                adjwp = wheadp;
                while (adjwp->w_wndp != curwp)
                        adjwp = adjwp->w_wndp;
        }
        if (adjwp->w_ntrows <= n) {
                mlwrite("Impossible change");
                return (FALSE);
        }
        if (curwp->w_wndp == adjwp) {           /* Shrink below.        */
                lp = adjwp->w_linep;
                for (i=0; i<n && lp!=adjwp->w_bufp->b_linep; ++i)
                        lp = lforw(lp);
                adjwp->w_linep  = lp;
                adjwp->w_toprow += n;
        } else {                                /* Shrink above.        */
                lp = curwp->w_linep;
                for (i=0; i<n && lback(lp)!=curbp->b_linep; ++i)
                        lp = lback(lp);
                curwp->w_linep  = lp;
                curwp->w_toprow -= n;
        }
        curwp->w_ntrows += n;
        adjwp->w_ntrows -= n;
        curwp->w_flag |= WFMODE|WFHARD;
        adjwp->w_flag |= WFMODE|WFHARD;
        return (TRUE);
}

/*
 * Shrink the current window. Find the window that gains space. Hack at the
 * window descriptions. Ask the redisplay to do all the hard work. Bound to
 * "C-X C-Z".
 */
window_shrink(f, n)
{
        register WINDOW *adjwp;
        register LINE   *lp;
        register int    i;

        if (n < 0)
                return (window_enlarge(f, -n));
        if (wheadp->w_wndp == NULL) {
                mlwrite("Only one window");
                return (FALSE);
        }
        if ((adjwp=curwp->w_wndp) == NULL) {
                adjwp = wheadp;
                while (adjwp->w_wndp != curwp)
                        adjwp = adjwp->w_wndp;
        }
        if (curwp->w_ntrows <= n) {
                mlwrite("Impossible change");
                return (FALSE);
        }
        if (curwp->w_wndp == adjwp) {           /* Grow below.          */
                lp = adjwp->w_linep;
                for (i=0; i<n && lback(lp)!=adjwp->w_bufp->b_linep; ++i)
                        lp = lback(lp);
                adjwp->w_linep  = lp;
                adjwp->w_toprow -= n;
        } else {                                /* Grow above.          */
                lp = curwp->w_linep;
                for (i=0; i<n && lp!=curbp->b_linep; ++i)
                        lp = lforw(lp);
                curwp->w_linep  = lp;
                curwp->w_toprow += n;
        }
        curwp->w_ntrows -= n;
        adjwp->w_ntrows += n;
        curwp->w_flag |= WFMODE|WFHARD;
        adjwp->w_flag |= WFMODE|WFHARD;
        return (TRUE);
}

/*
 * Pick a window for a pop-up. Split the screen if there is only one window.
 * Pick the uppermost window that isn't the current window. An LRU algorithm
 * might be better. Return a pointer, or NULL on error.
 */
WINDOW  *
wpopup()
{
        register WINDOW *wp;

        if (wheadp->w_wndp == NULL              /* Only 1 window        */
        && window_split(FALSE, 0) == FALSE)        /* and it won't split   */
                return (NULL);
        wp = wheadp;                            /* Find window to use   */
        while (wp!=NULL && wp==curwp)
                wp = wp->w_wndp;
        return (wp);
}

/*
 * Delete the current window. Does nothing if this window is last the
 * one on the screen.  Otherwise, it moves to the next window and
 * deletes the previous one.
 */
delwind(f, n)
{
	register WINDOW *delwp;
	register LINE *lp;
	register int i;

	if( wheadp->w_wndp == NULL )	return( TRUE );	/* Only 1 window */

	delwp = curwp;
	if( wheadp == delwp )		/* Pick which window to be in next */
	{
		curwp = curwp->w_wndp;
		wheadp = curwp;
		curbp = curwp->w_bufp;
		lp = curwp->w_linep;
		i  = curwp->w_toprow;
		while( i!=0 && lback(lp)!=curbp->b_linep )
		{
			i--;
			lp = lback(lp);
		}
		curwp->w_toprow = delwp->w_toprow;
	}
	else
	{
		curwp = wheadp;
		while( curwp->w_wndp != delwp )
			curwp = curwp->w_wndp;
		curwp->w_wndp = delwp->w_wndp;
		curbp = curwp->w_bufp;
	}

	curwp->w_ntrows += delwp->w_ntrows+1;
	curwp->w_flag |= WFMODE|WFHARD;

	delwp->w_bufp->b_dotp  = delwp->w_dotp;
	delwp->w_bufp->b_doto  = delwp->w_doto;
	delwp->w_bufp->b_markp = delwp->w_markp;
	delwp->w_bufp->b_marko = delwp->w_marko;
	delwp->w_bufp->b_nwnd--;
	free((char *) delwp);

	return( TRUE );
}
