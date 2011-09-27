/**************************************************************************
 *
 *  THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF ANY
 *  KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
 *  IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A PARTICULAR
 *  PURPOSE.
 *
 *  Copyright (c) 1992 - 1995  Microsoft Corporation.  All Rights Reserved.
 *
 **************************************************************************/
/*--------------------------------------------------------------------
|
| MovPlay2.c - Sample Win app to play AVI movies using mciSendString
|
| Movie Functions supported:
|	Play/Pause
|	Home/End
|	Step/ReverseStep
|
+--------------------------------------------------------------------*/

#define  STRICT
#include <windows.h>
#include <windowsx.h>
#include <commdlg.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <direct.h>
#include <mmsystem.h>		
#include <digitalv.h>		
#include "movplay.h"

//----------------------------------------------------------------------------
#define BUFSIZE 260
#define MOVPLAY_CLASS "movplay"
#define AVI_VIDEO "avivideo"
#define OPEN_AVI_VIDEO "open avivideo"
#define CLOSE_AVI_VIDEO "close avivideo"
#define MOV_REALIZE "realize mov"
#define MOV_SEEK_TO_START "seek mov to start"
#define MOV_SEEK_TO_END "seek mov to end"
#define MOV_CLOSE "close mov"
#define MOV_SOURCE "where mov source"
#define MOV_OPEN_FORMAT "open %s alias mov style child parent %d"
#define MOV_SHOW "window mov state show"
#define MOV_HANDLE "status mov window handle"
#define MOV_REVERSE "play mov reverse notify"
#define MOV_FORWARD "play mov notify"
#define MOV_PAUSE "pause mov"
#define MOV_FORWARD_STEP "step mov by 1"
#define MOV_REVERSE_STEP "step mov reverse by 1"
//----------------------------------------------------------------------------
//function declarations

LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK    AboutDlgProc(HWND hwnd, UINT msg, WPARAM wParam,LPARAM lParam);

static BOOL initAVI(void);
static void termAVI(void);
static HWND initApp(HINSTANCE, HINSTANCE, int);
static void menubarUpdate(HWND);
static void titlebarUpdate(HWND hWnd, LPSTR lpstrMovie);
static void positionMovie(HWND);
static void fileCloseMovie(HWND);
static void fileOpenMovie(HWND);
static void playMovie(HWND, int);
static void seekMovie(HWND, int);
static void stepMovie(HWND, int);
static BOOL FormatFilterString(UINT, LPSTR);
static void MovPlay_OnInitMenuPopup(HWND, HMENU, int, BOOL);
static void MovPlay_OnCommand(HWND, int, HWND, UINT);
static void MovPlay_OnSize(HWND, UINT, int, int);
static void MovPlay_OnDestroy(HWND);


//----------------------------------------------------------------------------
//AVI stuff to keep around

HINSTANCE ghInst;
WORD gwMCIDeviceID = 0;	/* MCI Device ID for the AVI file */
HWND ghwndMovie;		/* window handle of the movie */
BOOL gfPlaying = FALSE;	/* Play flag: TRUE == playing, FALSE == paused */
BOOL gfMovieOpen = FALSE;/* Open flag: TRUE == movie open, FALSE = none */
HANDLE ghAccel = NULL;	/* accelerator table */
HMENU ghMenuBar = NULL;	/* menu bar handle */
char gszBuffer[BUFSIZE];

//----------------------------------------------------------------------------

/*--------------------------------------------------------------+
| initAVI - initialize avi libraries				|
|								|
+--------------------------------------------------------------*/
static BOOL initAVI(
void)
{
	return mciSendString(OPEN_AVI_VIDEO, NULL, 0, NULL) == 0;
}

/*--------------------------------------------------------------+
| termAVI - Closes the opened AVI file and the opened device    |
|           type.                                               |
|                                                               |
+--------------------------------------------------------------*/
void termAVI(
void)
{
	mciSendString(CLOSE_AVI_VIDEO, NULL, 0, NULL);
}
/*--------------------------------------------------------------+
| initApp - initialize the app overall.				|
|								|
| Returns the Window handle for the app on success, NULL if	|
| there is a failure.						|
|								|
+--------------------------------------------------------------*/
HWND initApp(
HINSTANCE hInstance,
HINSTANCE hPrevInstance,
int nCmdShow)
{
	HWND	hWnd;	/* window handle to return */
        char 	szAppName[BUFSIZE];

        ghInst = hInstance;

	if (!hPrevInstance){
		WNDCLASS    wndclass;

		wndclass.style		= CS_DBLCLKS | CS_HREDRAW | CS_VREDRAW;
		wndclass.lpfnWndProc	= WndProc;
		wndclass.cbClsExtra	= 0;
		wndclass.cbWndExtra	= 0;
		wndclass.hInstance	= hInstance;
		wndclass.hIcon		= LoadIcon (hInstance, "AppIcon");
		wndclass.hCursor	= LoadCursor (NULL, IDC_ARROW);
		wndclass.hbrBackground	= (HBRUSH) (COLOR_WINDOW + 1);
		wndclass.lpszMenuName	= MOVPLAY_CLASS;
		wndclass.lpszClassName	= MOVPLAY_CLASS;

		if (!RegisterClass(&wndclass)){
		        LoadString( ghInst, IDS_APPERR, szAppName, BUFSIZE );
			LoadString( ghInst, IDS_REGCLSFAILED, gszBuffer, BUFSIZE );
			MessageBox(NULL, gszBuffer, szAppName, MB_OK);
			return NULL;
			}
	}

        LoadString( ghInst, IDS_APPNAME, szAppName, BUFSIZE );

	/* create the main window for the app */
	hWnd = CreateWindow(MOVPLAY_CLASS, szAppName, WS_OVERLAPPEDWINDOW |
		WS_CLIPCHILDREN, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
		CW_USEDEFAULT, NULL, NULL, hInstance, NULL);

	if (hWnd == NULL){
	        LoadString( ghInst, IDS_APPERR, szAppName, BUFSIZE );
		LoadString( ghInst, IDS_CRTWNDFAILED, gszBuffer, BUFSIZE );
		MessageBox(NULL, gszBuffer, szAppName, MB_OK);
		return NULL;
		}

	ghMenuBar = GetMenu(hWnd);	/* get the menu bar handle */
	menubarUpdate(hWnd);		/* update menu bar to disable Movie menu */

	/* Show the main window */
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	/* load the accelerator table */
	ghAccel = LoadAccelerators(hInstance, MOVPLAY_CLASS);

	return hWnd;
}


/*--------------------------------------------------------------+
| WinMain - main routine.					|
|								|
+--------------------------------------------------------------*/
int PASCAL WinMain(
HINSTANCE hInstance,
HINSTANCE hPrevInstance,
LPSTR lpszCmdParam,
int nCmdShow)
{
	HWND        hWnd;
	MSG         msg;

	if (!initAVI())
		return 0;

	if ((hWnd = initApp(hInstance, hPrevInstance, nCmdShow)) == NULL)
		return 0;	/* died initializing, bail out */

	/* main message loop, be sure to handle accelerators */
	while (GetMessage(&msg, NULL, 0, 0)){
		if (!TranslateAccelerator(hWnd, ghAccel, &msg)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}
	return msg.wParam;
}

/*--------------------------------------------------------------+
| MovPlay_OnInitMenuPopup - Message handler for WM_INITMENUPOPUP|
|								|
+--------------------------------------------------------------*/
static void MovPlay_OnInitMenuPopup(
HWND hwnd,
HMENU hMenu,
int item,
BOOL fSystemMenu)
{
	UINT uEnable;
	/* be sure this isn't the system menu */
	if (fSystemMenu)
		return;

	switch (item){
		case 0: /* file menu */
			/* turn on/off CLOSE & PLAY */
			if(gfMovieOpen)
				uEnable = MF_ENABLED|MF_BYCOMMAND;
			else
				uEnable = MF_GRAYED|MF_BYCOMMAND;
			EnableMenuItem(hMenu, IDM_CLOSE, uEnable);
			break;
				
		case 1:	/* Movie menu */
			/* check or uncheck the PLAY menu item */
			if (gfPlaying)
				uEnable = MF_CHECKED|MF_BYCOMMAND;
			else
				uEnable = MF_UNCHECKED|MF_BYCOMMAND;
			CheckMenuItem(hMenu, IDM_PLAY, uEnable);
			break;
	} /* switch */
	return;
}
/*--------------------------------------------------------------+
| MovPlay_OnCommand - Message handler for WM_COMMAND		|
|                                                               |
+--------------------------------------------------------------*/
static void MovPlay_OnCommand(
HWND hWnd,
int id,
HWND hwndCtl,
UINT codeNotify)
{
	/* handle the menu commands */
	switch (id) {
                case IDM_ABOUT:
                        DialogBox(ghInst, MAKEINTRESOURCE(IDD_ABOUT), hWnd, AboutDlgProc);
                        break;
		/* File Menu */
		case IDM_OPEN:
			fileOpenMovie(hWnd);
			break;
		case IDM_CLOSE:
			fileCloseMovie(hWnd);
			break;
		case IDM_EXIT:
			PostMessage(hWnd, WM_CLOSE, 0, 0L);
			break;
		/* Movie Menu - note some of these are by */
		/* keyboard only, especially the REVERSE  */
		/* commands.				  */
		case IDM_PLAY:
		case IDM_RPLAY:
			playMovie(hWnd, id);
			break;
		case IDM_HOME:
		case IDM_END:
			seekMovie(hWnd, id);
			break;
		case IDM_STEP:
		case IDM_RSTEP:
			stepMovie(hWnd,id);
			break;
	}
	return;
}
/*--------------------------------------------------------------+
| MovPlay_OnSize - Message handler for WM_SIZE		|
|								|
+--------------------------------------------------------------*/
static void MovPlay_OnSize(
HWND hWnd,
UINT state,
int cx,
int cy)
{
	positionMovie(hWnd);	/* re-center the movie */
	return;
}
/*--------------------------------------------------------------+
| MovPlay_OnDestroy - Message handler for WM_DESTROY		|
|								|
+--------------------------------------------------------------*/
static void MovPlay_OnDestroy(
HWND hWnd)
{
	if (gfMovieOpen)
		fileCloseMovie(hWnd);
	termAVI();
	PostQuitMessage(0);
	return;
}
/*--------------------------------------------------------------+
| WndProc - window proc for the app				|
|								|
+--------------------------------------------------------------*/
LRESULT CALLBACK WndProc(
HWND hWnd,
UINT message,
WPARAM wParam,
LPARAM lParam)
{
	switch (message){
	case WM_INITMENUPOPUP:
		return HANDLE_WM_INITMENUPOPUP(hWnd, wParam, lParam, MovPlay_OnInitMenuPopup);
		break;
		
	case WM_COMMAND:
		return HANDLE_WM_COMMAND(hWnd, wParam, lParam, MovPlay_OnCommand);
		break;

	case WM_SIZE:
		return HANDLE_WM_SIZE(hWnd, wParam, lParam, MovPlay_OnSize);		
		break;
		
	case WM_DESTROY:
		return HANDLE_WM_DESTROY(hWnd, wParam, lParam, MovPlay_OnDestroy);		
		break;

        // We need to realize the AVI's palette
	case WM_QUERYNEWPALETTE:
	case WM_PALETTECHANGED:
	case WM_ACTIVATE:
            mciSendString(MOV_REALIZE, NULL, 0, NULL);
	    break;
		
	case MM_MCINOTIFY:
		/* This is where we check the status of an AVI	*/
		/* movie that might have been playing.  We do	*/
		/* the play with MCI_NOTIFY on so we should get	*/
		/* a MCI_NOTIFY_SUCCESSFUL if the play		*/
		/* completes on it's own.			*/
		switch(wParam){
			case MCI_NOTIFY_SUCCESSFUL:
				/* the play finished, let's rewind */
				/* and clear our flag.		   */
				gfPlaying = FALSE;
				mciSendString(MOV_SEEK_TO_START, NULL, 0, NULL);
				return 0;
		}
	} /* switch */
	return DefWindowProc(hWnd, message, wParam, lParam);
}

/*--------------------------------------------------------------+
| menubarUpdate - update the menu bar based on the <gfMovieOpen> |
|		  flag value.  This will turn on/off the	|
|		  Movie menu.					|
|								|
+--------------------------------------------------------------*/
static void menubarUpdate(
HWND hWnd)
{
	WORD w;

	if (gfMovieOpen){
		w = MF_ENABLED|MF_BYPOSITION;
	} else {
		w = MF_GRAYED|MF_BYPOSITION;
	}
	EnableMenuItem(ghMenuBar, 1, w);	/* change the Movie menu (#1) */
	DrawMenuBar(hWnd);	/* re-draw the menu bar */
}

/*--------------------------------------------------------------+
| titlebarUpdate - update the title bar to include the name	|
|		   of the movie playing.			|
|								|
+--------------------------------------------------------------*/
static void titlebarUpdate(
HWND hWnd,
LPSTR lpstrMovie)
{
	char szNewTitle[2*BUFSIZE];	// space for the title
	char szAppName[BUFSIZE];	

	LoadString( ghInst, IDS_APPNAME, szAppName, BUFSIZE );

	if (lpstrMovie != NULL)
		wsprintf(szNewTitle,"%s - %s", (LPSTR)szAppName,(LPSTR)lpstrMovie);
	else
		lstrcpy(szNewTitle, szAppName);
	SetWindowText(hWnd, szNewTitle);
}

/*--------------------------------------------------------------+
| positionMovie - sets the movie rectange <rcMovie> to be	|
|		centered within the app's window.		|
|								|
+--------------------------------------------------------------*/
static VOID positionMovie(
HWND hWnd)
{
	RECT rcMovie;
	RECT rcClient;
	RECT rcMovieBounds;
	char achRect[128];
	char *p;

	/* if there is no movie yet then just get out of here */
	if (!gfMovieOpen)
		return;

	GetClientRect(hWnd, &rcClient);	/* get the parent windows rect */
	
	/* get the original size of the movie */
	mciSendString(MOV_SOURCE, achRect, sizeof(achRect), NULL);

	SetRectEmpty(&rcMovieBounds);	// zero out movie rect
	p = achRect;	// point to rectangle string returned by where command
	while (*p == ' ') p++;	// skip over starting spaces
	while (*p != ' ') p++;	// skip over the x (which is 0)
	while (*p == ' ') p++;	// skip over spaces between x and y
	while (*p != ' ') p++;	// skip over the y (which is 0)
	while (*p == ' ') p++;	// skip over the spaces between y and width

	/* now find the width */
	for (; *p >= '0' && *p <= '9'; p++)
		rcMovieBounds.right = (10 * rcMovieBounds.right) + (*p - '0');
	while (*p == ' ') p++;	// skip spaces between width and height

	/* now find the height */
	for (; *p >= '0' && *p <= '9'; p++)
		rcMovieBounds.bottom = (10 * rcMovieBounds.bottom) + (*p - '0');

	/* figure out where to position the window at */
	rcMovie.left = (rcClient.right/2) - (rcMovieBounds.right / 2);
	rcMovie.top = (rcClient.bottom/2) - (rcMovieBounds.bottom / 2);
	rcMovie.right = rcMovie.left + rcMovieBounds.right;
	rcMovie.bottom = rcMovie.top + rcMovieBounds.bottom;

	/* reposition the playback (child) window */
	MoveWindow(ghwndMovie, rcMovie.left, rcMovie.top,
		rcMovieBounds.right, rcMovieBounds.bottom, TRUE);
}

/*--------------------------------------------------------------+
| fileCloseMovie - close the movie and anything associated	|
|		   with it.					|
|								|
| This function clears the <gfPlaying> and <gfMovieOpen> flags	|
|								|
+--------------------------------------------------------------*/
static void fileCloseMovie(HWND hWnd)
{
	mciSendString(MOV_CLOSE, NULL, 0, NULL);

	gfPlaying = FALSE;	// can't be playing any longer
	gfMovieOpen = FALSE;	// no more movies open
	
	titlebarUpdate(hWnd, NULL);	// title bar back to plain
	menubarUpdate(hWnd);		// update menu bar
	
	/* cause a total repaint to occur */
	InvalidateRect(hWnd, NULL, TRUE);
	UpdateWindow(hWnd);
}


/*--------------------------------------------------------------+
| fileOpenMovie - open an AVI movie. Use CommDlg open box to	|
|	        open and then handle the initialization to	|
|		show the movie and position it properly.  Keep	|
|		the movie paused when opened.			|
|								|
|		Sets <gfMovieOpened> on success.			|
+--------------------------------------------------------------*/
static void fileOpenMovie(
HWND hWnd)
{
	OPENFILENAME ofn;

	static char szFile [BUFSIZE];
	static char szFileTitle [BUFSIZE];
	static int  nLastFilter = 1;	  /* keep last file-type opened */
	static char szFilter [BUFSIZE];

	FormatFilterString( IDS_FILTERSTRING, szFilter );

	/* use the OpenFile dialog to get the filename */
	memset(&ofn, 0, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = hWnd;
	ofn.lpstrFilter = szFilter;
	ofn.nFilterIndex = nLastFilter;
	ofn.lpstrFile = szFile;
	ofn.nMaxFile = sizeof(szFile);
	ofn.lpstrFileTitle = szFileTitle;
	ofn.nMaxFileTitle = sizeof(szFileTitle);
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

	/* use CommDlg to get our filename */
	if (GetOpenFileName(&ofn)){
		char szCommand[BUFSIZE];
		DWORD  dw;

		/* we got a filename, now close any old movie and open */
		/* the new one.					*/
		if (gfMovieOpen)
			fileCloseMovie(hWnd);	
	
		/* we have a .AVI movie to open, use MCI */
		/*
		| build up the string -
		| open alias mov parent HWND FILENAME
		| to pass to mciSendString
		*/
		wsprintf(szCommand,MOV_OPEN_FORMAT, (LPSTR)ofn.lpstrFile, (int)hWnd);

		/* try to open the file */
		if (mciSendString(szCommand, NULL, 0, NULL) == 0){
			gfMovieOpen = TRUE;
			/* we opened the file o.k., now set up to */
			/* play it.				   */
			mciSendString(MOV_SHOW, NULL, 0, NULL);
			/* get the window handle */
			if ((dw = mciSendString(MOV_HANDLE, szCommand, sizeof(szCommand),
							NULL)) == 0L)
				ghwndMovie = (HWND)atoi(szCommand);
			else {
				mciGetErrorString(dw, szCommand, sizeof(szCommand));
				MessageBox(hWnd, szCommand, NULL, MB_ICONEXCLAMATION|MB_OK);
			}
			/* now get the movie centered */
			positionMovie(hWnd);
		} else {
			/* generic error for open */
			LoadString( ghInst, IDS_NOOPEN, gszBuffer, BUFSIZE );
			MessageBox(hWnd, gszBuffer, NULL, MB_ICONEXCLAMATION|MB_OK);
			gfMovieOpen = FALSE;
		}
	}
	/* update menu and title bars */
	if (gfMovieOpen)
		titlebarUpdate(hWnd, (LPSTR)ofn.lpstrFileTitle);
	else
		titlebarUpdate(hWnd, NULL);
	menubarUpdate(hWnd);

	/* cause an update to occur */
	InvalidateRect(hWnd, NULL, FALSE);
	UpdateWindow(hWnd);
}

/*--------------------------------------------------------------+
| playMovie - play/pause the movie depending on the state	|
|		of the <gfPlaying> flag.			|
|								|
| This function sets the <gfPlaying> flag appropriately when done|
|								|
+--------------------------------------------------------------*/
static void playMovie(
HWND hWnd,
int nDirection)
{
	gfPlaying = !gfPlaying;	/* swap the play flag */
	if(!nDirection)
		gfPlaying = FALSE;	/* nDirection == NULL means PAUSE */

	/* play/pause the AVI movie */
	if (gfPlaying){
		if (nDirection == IDM_RPLAY){
			mciSendString(MOV_REVERSE, NULL, 0, hWnd);
		} else {
			mciSendString(MOV_FORWARD, NULL, 0, hWnd);
		}
	} else {
		/* tell it to pause */
		mciSendString(MOV_PAUSE, NULL, 0, NULL);
	}
}

/*--------------------------------------------------------------+
| seekMovie - seek in the movie depending on the wAction.	|
|	      Possible actions are IDM_HOME (start of movie) or	|
|	      IDM_END (end of movie)				|
|								|
|	      Always stop the play before seeking.		|
|								|
+--------------------------------------------------------------*/
static void seekMovie(
HWND hWnd,
int nAction)
{
	/* first stop the movie from playing if it is playing */
	if (gfPlaying){
		playMovie(hWnd, 0);	
	}
	
	if (nAction == IDM_HOME){
		/* home the movie */
		mciSendString(MOV_SEEK_TO_START, NULL, 0, NULL);
	} else if (nAction == IDM_END){
		/* go to the end of the movie */
		mciSendString(MOV_SEEK_TO_END, NULL, 0, NULL);
	}
}

/*--------------------------------------------------------------+
| stepMovie - step forward or reverse in the movie.  wDirection	|
|		can be IDM_STEP (forward) or IDM_RSTEP (reverse)|
|								|
|		Again, stop the play if one is in progress.	|
|								|
+--------------------------------------------------------------*/
static void stepMovie(
HWND hWnd,
int nDirection)
{
	if (gfPlaying)
		playMovie(hWnd, 0);  /* turn off the movie */

	if (nDirection == IDM_STEP)
		mciSendString(MOV_FORWARD_STEP, NULL, 0, NULL);
	else
		mciSendString(MOV_REVERSE_STEP, NULL,0, NULL);
}
/*--------------------------------------------------------------+
| FormatFilterString 						|
|								|
+--------------------------------------------------------------*/
static BOOL FormatFilterString(
UINT uID,
LPSTR lpszString )
{
	int nLength, nCtr = 0;
	char chWildCard;
	
	*lpszString = 0;
	
	nLength = LoadString( ghInst, uID, lpszString, BUFSIZE );

	chWildCard = lpszString[nLength-1];

	while( lpszString[nCtr] ) {
		if( lpszString[nCtr] == chWildCard )
			lpszString[nCtr] = 0;
		nCtr++;
	}

	return TRUE;
}				

/* AboutDlgProc()
 *
 * Dialog Procedure for the "about" dialog box.
 *
 */

BOOL CALLBACK AboutDlgProc(
	HWND	hwnd,
	UINT	msg,
	WPARAM	wParam,
	LPARAM	lParam)
{
	switch (msg) {
	case WM_COMMAND:
		EndDialog(hwnd, TRUE);
		return TRUE;
	case WM_INITDIALOG:
		return TRUE;
	}
	return FALSE;
}
