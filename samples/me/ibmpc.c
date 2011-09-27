/*_ ibmpc.c   Tue Aug  4 1987 */
/*
 * The routines in this file provide support for computers with an IBM PC
 * compatible display.
 * It compiles into nothing if not an IBM PC device.
 */

#include        <stdio.h>
#include        "ed.h"

#if     IBMPC
#include	<disp.h>
#include	<msmouse.h>

#define BEL     0x07                    /* BEL character.               */
#define ESC     0x1B                    /* ESC character.               */

extern  int     ibmpcopen();            /* Forward references.          */
extern  int     ibmpcclose();
extern  int     ttgetc();
extern  int     ibmpcbeep();

static char set43 = FALSE;		/* TRUE if we set to 43 line mode */
					/* so we can set it back to 25	*/
					/* line mode upon exit		*/

/*
 * Standard terminal interface dispatch table. Most of the fields point into
 * "termio" code.
 */

typedef int (*TERMFP)();

TERM    term    = {
	0,
	0,
	(TERMFP) ibmpcopen,
	(TERMFP) ibmpcclose,
	(TERMFP) ttgetc,
	(TERMFP) disp_putc,
	(TERMFP) disp_flush,
	(TERMFP) disp_move,		/* (row,col)	*/
	(TERMFP) disp_eeol,
	(TERMFP) disp_eeop,
	(TERMFP) ibmpcbeep,
	(TERMFP) disp_startstand,
	(TERMFP) disp_endstand,
	(TERMFP) NULL,
	(TERMFP) NULL
};

ibmpcopen()
{
	ttopen();
	disp_open();
	disp_setcursortype(DISP_CURSORBLOCK);
	term.t_ncol = disp_numcols;
	term.t_nrow = disp_numrows;
	if (!(disp_mode == 1 || disp_mode == 3))
	{	config.modeattr = DISP_REVERSEVIDEO * 256;
		config.normattr = DISP_NORMAL * 256;
		config.eolattr  = config.normattr;
		config.markattr = (DISP_UNDERLINE | DISP_INTENSITY) * 256;
	}
#if MOUSE
	if (mouse)
	{   msm_setareay(0,(disp_numrows - 1) * 8);
	    msm_setareax(0,(disp_numcols - 1) *
		((disp_mode == 0 || disp_mode == 1) ? 16 : 8));
	    msm_showcursor();
	}
#endif
}

ibmpcclose()
{
#if MOUSE
    if (mouse)
	msm_hidecursor();
#endif
    if (set43)
	disp_reset43();		/* MS-DOS can't deal with 43 line mode	*/
    disp_close();
    ttclose();
}

/****************************
 * Toggle 43 line mode.
 */

ibmpctoggle43()
{   WINDOW *wp;
    int row;

L1:
    if (disp_ega)
    {
	if (disp_numrows == 25)
	{   set43 = TRUE;
	    vttidy();			/* release old video buffers	*/
	    disp_set43();
	}
	else
	{   vttidy();
	    disp_reset43();
	}
	term.t_nrow = disp_numrows;
	/* BUG: vtinit() can run out of memory, in which case the	*/
	/* program will gracelessly abort.				*/
	vtinit();			/* initialize display buffers	*/

	/* The windows all need to be readjusted			*/
	row = 0;
	for (wp = wheadp; wp; wp = wp->w_wndp)	/* for each window	*/
	{   wp->w_flag = WFMODE | WFFORCE;	/* force reformatting	*/
	    if (row >= term.t_nrow - 2)	/* if can't readjust window	*/
		goto L1;		/* switch it back		*/
	    wp->w_toprow = row;
	    if (row + wp->w_ntrows > term.t_nrow - 2 ||
		!wp->w_wndp && row + wp->w_ntrows != term.t_nrow - 2)
		wp->w_ntrows = term.t_nrow - 2 - row;
	    row += wp->w_ntrows + 1;	/* starting row of next window	*/
	}
	mlerase();
	window_refresh(FALSE,1);	/* and redraw the display	*/
    }
    return disp_ega;			/* FALSE if we failed		*/
}

ibmpcbeep()
{
        disp_putc(BEL);
        disp_flush();
}

updateline(int row,unsigned short *buffer,unsigned short *physical)
{
    int col;

#if 1
    if (__INTSIZE == 2 && disp_base && !disp_snowycga)
	pokebytes(disp_base,(row * disp_numcols) * 2,buffer,disp_numcols * 2);
    else
    {	for (col = 0; col < disp_numcols; col++)
	    if (buffer[col] != physical[col])
		disp_pokew(row,col,buffer[col]);
	memcpy(physical,buffer,sizeof(buffer[0]) * disp_numcols);
    }
#else
	char c;
	int inverse;

	inverse = FALSE;
	disp_endstand();
	disp_move(row,col);
	while ((c = *buffer++) != 0)
	{	if (c & 0x80)
		{	if (!inverse)
			{	disp_startstand();
				inverse = TRUE;
			}
		}
		else
		{	if (inverse)
			{	disp_endstand();
				inverse = FALSE;
			}
		}
		disp_putc(c & 0x7F);
	}
	disp_endstand();
#endif
}

#endif /* IBMPC */
