/*_ win32.c   Wed May 24 1995 */
/*
 * The routines in this file provide support for computers with
 * WIN32 console I/O support.
 * It compiles into nothing if not a WIN32 device.
 */

#include        "ed.h"

#if     _WIN32
#include	<windows.h>
#include        <stdio.h>
#include        <stdlib.h>
#include	<msmouse.h>
#include	<disp.h>
#include	<malloc.h>

#define BEL     0x07                    /* BEL character.               */
#define ESC     0x1B                    /* ESC character.               */

extern  int     win32open();            /* Forward references.          */
extern  int     win32close();
extern  int     ttgetc();
extern	int	ttputc(int c);
extern  int     win32beep();

static char set43 = FALSE;		/* TRUE if we set to 43 line mode */
					/* so we can set it back to 25	*/
					/* line mode upon exit		*/

static HANDLE hStdin;			// console input handle
static DWORD fdwSaveOldMode;

static INPUT_RECORD lookaheadir;
static int lookahead;			// !=0 if data in lookaheadir

/*
 * Standard terminal interface dispatch table. Most of the fields point into
 * "termio" code.
 */

typedef int (*TERMFP)();

TERM    term    = {
	0,
	0,
	(TERMFP) win32open,
	(TERMFP) win32close,
	(TERMFP) ttgetc,
	(TERMFP) disp_putc,
	(TERMFP) disp_flush,
	(TERMFP) disp_move,		/* (row,col)	*/
	(TERMFP) disp_eeol,
	(TERMFP) disp_eeop,
	(TERMFP) win32beep,
	(TERMFP) disp_startstand,
	(TERMFP) disp_endstand,
	(TERMFP) NULL,
	(TERMFP) NULL
};

win32open()
{
    hStdin  = GetStdHandle(STD_INPUT_HANDLE);
    if (hStdin == INVALID_HANDLE_VALUE)
    {	printf("getstdhandle\n");
	exit(EXIT_FAILURE);
    }

    if (!GetConsoleMode(hStdin,&fdwSaveOldMode))
    {	printf("getconsolemode\n");
	exit(EXIT_FAILURE);
    }

    if (!SetConsoleMode(hStdin,ENABLE_WINDOW_INPUT | ENABLE_MOUSE_INPUT))
    {	printf("setconsolemode\n");
	exit(EXIT_FAILURE);
    }

    disp_open();
    disp_setcursortype(DISP_CURSORBLOCK);
    term.t_ncol = disp_numcols;
    term.t_nrow = disp_numrows;
}

win32close()
{
    if (set43)
    {	disp_reset43();		/* MS-DOS can't deal with 43 line mode	*/
	disp_move(0,0);
	disp_eeop();
    }
    disp_close();

    if (!SetConsoleMode(hStdin,fdwSaveOldMode))
    {	printf("restore console mode\n");
	exit(EXIT_FAILURE);
    }
}

/****************************
 * Toggle 43 line mode.
 */

win32toggle43()
{
#if 0
    return 0;
#else
    WINDOW *wp;
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
#endif
}

/********************************************
 */

win32beep()
{
	ttputc(BEL);
}

/********************************************
 */

updateline(int row,attchar_t *buffer,attchar_t *physical)
{
    int col;
    int numcols;
    CHAR_INFO *psb;
    CHAR_INFO sbbuf[256];
    CHAR_INFO *sb;
    COORD sbsize;
    static COORD sbcoord;
    SMALL_RECT sdrect;

    sbsize.X = disp_numcols;
    sbsize.Y = 1;
    sbcoord.X = 0;
    sbcoord.Y = 0;
    sdrect.Left = 0;
    sdrect.Top = row;
    sdrect.Right = disp_numcols - 1;
    sdrect.Bottom = row;
    numcols = disp_numcols;
    sb = sbbuf;
    if (numcols > arraysize(sbbuf))
    {
	sb = (CHAR_INFO *)alloca(numcols * sizeof(CHAR_INFO));
    }
    for (col = 0; col < numcols; col++)
    {
	sb[col].Char.AsciiChar = buffer[col] & 0xFF;
	sb[col].Attributes = (buffer[col] >> 8) & 0xFF;
	//printf("col = %2d, x%2x, '%c'\n",col,sb[col].Char.AsciiChar,sb[col].Char.AsciiChar);
    }
    if (!WriteConsoleOutput((HANDLE)disp_state.handle,sb,sbsize,sbcoord,&sdrect))
    {
	// error
    }
    memcpy(physical,buffer,sizeof(buffer[0]) * disp_numcols);
}

/*********************************
 */

int __cdecl msm_init(void)
{
    return GetSystemMetrics(SM_MOUSEPRESENT);
}

void	__cdecl msm_term(void) { }
void	__cdecl msm_showcursor(void) { }
void	__cdecl msm_hidecursor(void) { }

static struct msm_status		// current state of mouse
{
    unsigned row;
    unsigned col;
    int buttons;
} mstat;

/*************************
 * Fold MOUSE_EVENT into mstat.
 */

static void mstat_update(MOUSE_EVENT_RECORD *pme)
{
    mstat.row = pme->dwMousePosition.Y;
    mstat.col = pme->dwMousePosition.X;
    mstat.buttons = pme->dwButtonState & 3;
}

int __cdecl msm_getstatus(unsigned *pcol,unsigned *prow)
{
    INPUT_RECORD buf;
    DWORD cNumRead;

    if (lookahead)
    {	buf = lookaheadir;
	cNumRead = 1;
    }
    else if (!PeekConsoleInput(hStdin,&buf,1,&cNumRead))
	goto Lret;

    if (cNumRead)
	switch (buf.EventType)
	{
	    case MOUSE_EVENT:
		mstat_update(&buf.Event.MouseEvent);
	    default:
		if (lookahead)
		    lookahead = 0;
		else
		    ReadConsoleInput(hStdin,&buf,1,&cNumRead);	// discard
		break;

	    case KEY_EVENT:
		break;
	}

Lret:
    *prow = mstat.row;
    *pcol = mstat.col;
    return mstat.buttons;
}

/*************************************
 * Translate key from WIN32 to IBM PC style.
 * Returns:
 *	0 if ignore it
 */

static unsigned win32_keytran(KEY_EVENT_RECORD *pkey)
{   unsigned c;

    c = 0;
    if (!pkey->bKeyDown)
	goto Lret;				// ignore button up events
    c = pkey->uChar.AsciiChar & 0xFF;
    if (c == 0)
    {
	switch (pkey->wVirtualScanCode)
	{
	    case 0x1D:				// Ctrl
	    case 0x38:				// Alt
	    case 0x2A:				// Left Shift
	    case 0x36:				// Right Shift
		break;				// ignore
	    default:
		c = (pkey->wVirtualScanCode << 8) & 0xFF00;
		if (pkey->dwControlKeyState & (RIGHT_CTRL_PRESSED | LEFT_CTRL_PRESSED))
		{
		    switch (c)
		    {   case 0x4700:	c = 0x7700;	break;	// Home
			case 0x4F00:	c = 0x7500;	break;	// End
			case 0x4900:	c = 0x8400;	break;	// PgUp
			case 0x5100:	c = 0x7600;	break;	// PgDn
		    }
		}
		break;
	}
    }
    else if (pkey->dwControlKeyState & (RIGHT_ALT_PRESSED | LEFT_ALT_PRESSED))
    {
	c = (pkey->wVirtualScanCode << 8) & 0xFF00;
    }
Lret:
    return c;
}

/*************************************
 * Wait for any input (yield to other processes).
 */

ttyield()
{
    if (!lookahead)
    {
	DWORD cNumRead;

	if (!ReadConsoleInput(hStdin,&lookaheadir,1,&cNumRead))
	{   printf("readconsoleinput\n");
	    goto Lret;
	}
    }
    lookahead = 1;
Lret: ;
}

/*************************************
 */

int ttkeysininput()
{
    INPUT_RECORD buf;
    DWORD cNumRead;

    if (lookahead)
    {	buf = lookaheadir;
	cNumRead = 1;
    }
    else if (!PeekConsoleInput(hStdin,&buf,1,&cNumRead))
	goto Lret;

    if (cNumRead)
    {
	switch (buf.EventType)
	{
	    case MOUSE_EVENT:
		mstat_update(&buf.Event.MouseEvent);
	    default:
	    Ldiscard:
		if (lookahead)
		    lookahead = 0;
		else
		    ReadConsoleInput(hStdin,&buf,1,&cNumRead);	// discard
		cNumRead = 0;
		break;

	    case KEY_EVENT:
		if (!win32_keytran(&buf.Event.KeyEvent))
		    goto Ldiscard;
		break;
	}
    }

Lret:
    return cNumRead != 0;
}

/*************************************
 */

int ttputc(int c)
{
    disp_putc(c);
}

/*************************************
 */

int ttgetc()
{
    INPUT_RECORD buf;
    DWORD cNumRead;
    int c;

    while (1)
    {
	if (lookahead)
	{   buf = lookaheadir;
	    lookahead = 0;
	}
	else if (!ReadConsoleInput(hStdin,&buf,1,&cNumRead))
	{   c = 3;				// ^C
	    goto Lret;
	}

	switch (buf.EventType)
	{
	    case MOUSE_EVENT:
		mstat_update(&buf.Event.MouseEvent);
		continue;

	    default:
		continue;			// ignore

	    case KEY_EVENT:
		c = win32_keytran(&buf.Event.KeyEvent);
		if (!c)
		    continue;
		goto Lret;
	}
    }

Lret:
    return c;
}

#endif /* _WIN32 */
