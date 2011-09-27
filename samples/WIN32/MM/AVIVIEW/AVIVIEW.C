/**************************************************************************
 *
 *  THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF ANY
 *  KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
 *  IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A PARTICULAR
 *  PURPOSE.
 *
 *  Copyright (c) 1993 - 1995  Microsoft Corporation.  All Rights Reserved.
 *
 **************************************************************************/

#define INC_OLE2
#include <windows.h>
#include <windowsx.h>
#include <mmsystem.h>
#include <memory.h>
#include <commdlg.h>
#include <vfw.h>

#include "muldiv32.h"

#include <mmreg.h>
#include <msacm.h>
#include "aviview.h"
#include "audplay.h"
#include "aviball.h"

#define GlobalSizePtr(lp)   GlobalSize(GlobalPtrHandle(lp))

#define FIXCC(fcc)  if (fcc == 0)       fcc = mmioFOURCC('N', 'o', 'n', 'e'); \
                    if (fcc == BI_RLE8) fcc = mmioFOURCC('R', 'l', 'e', '8');

/*----------------------------------------------------------------------------*\
\*----------------------------------------------------------------------------*/
typedef LONG (PASCAL *LPWNDPROC)(HWND, UINT, WPARAM, LPARAM); // pointer to a window procedure

/*----------------------------------------------------------------------------*\
\*----------------------------------------------------------------------------*/
static  TCHAR   gszAppName[]=TEXT("AVIView");
static  TCHAR	gachFilter[512] = TEXT("");


static  HANDLE	ghInstApp;
static  HWND	ghwndApp;
static  HACCEL	ghAccel;
static  HANDLE  ghLib;                 // Handle to palmap32.dll
static  BOOL    gbCanPalMap = FALSE;   // Is palmap32.dll available?

#define SCROLLRANGE  10000

#define MAXNUMSTREAMS   50
PAVIFILE	    gpfile;			// the current file
PAVISTREAM          gapavi[MAXNUMSTREAMS];	// the current streams
PGETFRAME	    gapgf[MAXNUMSTREAMS];	// data for decompressing
						// video
HDRAWDIB	    ghdd[MAXNUMSTREAMS];	// drawdib handles
int		    gcpavi;			// # of streams

BOOL		    gfPlaying = FALSE;		// Are we playing right now?
LONG		    glPlayStartTime;		// When did we start playing?
LONG 		    glPlayStartPos;		// From what position?

PAVISTREAM          gpaviAudio;                 // 1st audio stream found
PAVISTREAM          gpaviVideo;                 // 1st video stream found
int                 giFirstVideo;                // index of gapavi for 1st Video stream

#define             gfVideoFound (gpaviVideo != NULL)
#define             gfAudioFound (gpaviAudio != NULL)

LONG                timeStart;			// cached start, end, length
LONG                timeEnd;
LONG                timeLength;
LONG		    timehscroll;                // how much arrows scroll HORZ bar
LONG		    vertSBLen;
LONG		    vertHeight;


DWORD		    gdwMicroSecPerPixel = 1000L;// scale for video

TCHAR               gachFileName[MAX_PATH] = TEXT("");
TCHAR               gachSaveFileName[MAX_PATH] = TEXT("");
UINT		    gwZoom = 2;			// one half zoom (divide by 4)
AVICOMPRESSOPTIONS  gaAVIOptions[MAXNUMSTREAMS];
LPAVICOMPRESSOPTIONS  galpAVIOptions[MAXNUMSTREAMS];

HFONT               hfontApp;
TEXTMETRIC          tm;
BYTE		    abFormat[1024];
LPVOID		    lpAudio;		// buffer for painting
int                 gnColours;          // No of colours to remap palette to

		                	// !!! constants for painting
#define VSPACE  8	                // some vertical spacing
#define HSPACE  4               	// space between frames for video streams
#define TSPACE  20	                // space for text area about each stream
#define AUDIOVSPACE  64	                // height of an audio stream at X1 zoom

/*----------------------------------------------------------------------------*\
\*----------------------------------------------------------------------------*/

#define GetScrollTime(hwnd) \
    (LONG)(timeStart + muldiv32(GetScrollPos(hwnd, SB_HORZ), timeLength, SCROLLRANGE))

#define SetScrollTime(hwnd, time) SetScrollPos(hwnd, SB_HORZ, \
    (int)muldiv32((time) - timeStart, SCROLLRANGE, timeLength), TRUE)

/*----------------------------------------------------------------------------*\
\*----------------------------------------------------------------------------*/
long             PaintStuff(HDC hdc, HWND hwnd, BOOL fDrawEverything);
LONG WINAPI      AppWndProc (HWND hwnd, UINT uiMessage, WPARAM wParam, LPARAM lParam);
int              ErrMsg (LPTSTR sz,...);
LONG PASCAL      AppCommand(HWND hwnd, unsigned msg, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK    AboutDlgProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK    GetNumberOfColorsDlgProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
LONG             GetNumberOfColors(PAVISTREAM ps);
/*----------------------------------------------------------------------------*\
\*----------------------------------------------------------------------------*/

HCURSOR hcurSave;
int     fWait = 0;

/*----------------------------------------------------------------------------*\
|    StartWait()
|
|    Start a wait operation... put up the hourglass if it's the first call.
\*----------------------------------------------------------------------------*/
void StartWait()
{
    if (fWait++ == 0) {
        SetCursor(LoadCursor(NULL,IDC_WAIT));
    }
}

/*----------------------------------------------------------------------------*\
|    EndWait()
|
|    Once every one who started a wait is finished, go back to regular cursor.
\*----------------------------------------------------------------------------*/
void EndWait()
{
    if (--fWait == 0) {
        SetCursor(LoadCursor(NULL,IDC_ARROW));
        InvalidateRect(ghwndApp, NULL, TRUE);
    }
}

/*----------------------------------------------------------------------------*\
|    WinYield()
|
|    Code to yield while we're not calling GetMessage.
|    Dispatch all messages.  Pressing ESC or closing aborts.
\*----------------------------------------------------------------------------*/
BOOL WinYield()
{
    MSG msg;
    BOOL fAbort=FALSE;

    while(fWait > 0 && PeekMessage(&msg,NULL,0,0,PM_REMOVE))
    {
	if (msg.message == WM_KEYDOWN && msg.wParam == VK_ESCAPE)
            fAbort = TRUE;
	if (msg.message == WM_SYSCOMMAND && (msg.wParam & 0xFFF0) == SC_CLOSE)
	    fAbort = TRUE;
	TranslateMessage(&msg);
	DispatchMessage(&msg);
    }
    return fAbort;
}


/*----------------------------------------------------------------------------*\
\*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*\
|    FixScrollbars()
|
|    When we load a file or zoom changes, we re-set the scrollbars.
\*----------------------------------------------------------------------------*/
void FixScrollbars(HWND hwnd)
{
    LONG                lHeight = 0;
    RECT                rc;
    HDC                 hdc;

    //
    // Pretend we're painting and count how many lines we needed
    //
    hdc = GetDC(NULL);
    ExcludeClipRect(hdc, 0, 0, 32767, 32767);   // don't actually draw
    lHeight = PaintStuff(hdc, hwnd, TRUE);
    ReleaseDC(NULL, hdc);

    //
    // Set vertical scrollbar for scrolling the visible area
    //
    GetClientRect(hwnd, &rc);
    vertHeight = lHeight;	// total height in pixels of entire display

    //
    // We won't fit in the window... need scrollbars
    //
    if (lHeight > rc.bottom) {
	vertSBLen = lHeight - rc.bottom;
	SetScrollRange(hwnd, SB_VERT, 0, (int)vertSBLen, TRUE);
	SetScrollPos(hwnd, SB_VERT, 0, TRUE);

    //
    // We will fit in the window!  No scrollbars necessary
    //
    } else {
	vertSBLen = 0;
	SetScrollRange(hwnd, SB_VERT, 0, 0, TRUE);
    }
}

/*----------------------------------------------------------------------------*\
|    InitStreams()
|
|    Initialize the streams of a loaded file -- the compression options, the
|    DrawDIB handles, and the scroll bars.
\*----------------------------------------------------------------------------*/
void InitStreams(HWND hwnd)
{
    AVISTREAMINFO     avis;
    LONG	lTemp;
    int		i;
    DWORD	dw;

    //
    // Start with bogus times
    //
    timeStart = 0x7FFFFFFF;
    timeEnd   = 0;

    //
    // Walk through and init all streams loaded
    //
    for (i = 0; i < gcpavi; i++) {

        AVIStreamInfo(gapavi[i], &avis, sizeof(avis));

	//
	// Save and SaveOptions code takes a pointer to our compression opts
	//
	galpAVIOptions[i] = &gaAVIOptions[i];

	//
	// clear options structure to zeroes
	//
	_fmemset(galpAVIOptions[i], 0, sizeof(AVICOMPRESSOPTIONS));

	//
 	// Initialize the compression options to some default stuff
	// !!! Pick something better
	//
	galpAVIOptions[i]->fccType = avis.fccType;

	switch(avis.fccType) {

	    case streamtypeVIDEO:		
		galpAVIOptions[i]->dwFlags = AVICOMPRESSF_VALID |
			AVICOMPRESSF_KEYFRAMES | AVICOMPRESSF_DATARATE;
		galpAVIOptions[i]->fccHandler = 0;
		galpAVIOptions[i]->dwQuality = (DWORD)ICQUALITY_DEFAULT;
		galpAVIOptions[i]->dwKeyFrameEvery = 7;	// !!! ask compressor?
		galpAVIOptions[i]->dwBytesPerSecond = 60000;
		break;

	    case streamtypeAUDIO:
		galpAVIOptions[i]->dwFlags |= AVICOMPRESSF_VALID;
		galpAVIOptions[i]->dwInterleaveEvery = 5;
                AVIStreamReadFormat(gapavi[i], AVIStreamStart(gapavi[i]),
                                    NULL, &lTemp);
                galpAVIOptions[i]->cbFormat = lTemp;
                if (lTemp)
                    galpAVIOptions[i]->lpFormat = GlobalAllocPtr(GHND, lTemp);
                // Use current format as default format
                if (galpAVIOptions[i]->lpFormat)
                    AVIStreamReadFormat(gapavi[i],
                                    AVIStreamStart(gapavi[i]),
				    galpAVIOptions[i]->lpFormat,
				    &lTemp);
		break;

	    default:
		break;
	}

	//
	// We're finding the earliest and latest start and end points for
	// our scrollbar.
	//
        timeStart = min(timeStart, AVIStreamStartTime(gapavi[i]));
        timeEnd   = max(timeEnd, AVIStreamEndTime(gapavi[i]));

	//
	// Initialize video streams for getting decompressed frames to display
	//
        if (avis.fccType == streamtypeVIDEO) {

	    gapgf[i] = AVIStreamGetFrameOpen(gapavi[i], NULL);
	    if (gapgf[i] == NULL)
		continue;
	
	    ghdd[i] = DrawDibOpen();
	    // !!! DrawDibBegin?
	
	    if (gpaviVideo == NULL) {

		//
		// Remember the first video stream --- treat it specially
		//
                gpaviVideo = gapavi[i];
                giFirstVideo = i;

                //
                // Set the horizontal scrollbar scale to show every frame
                // of the first video stream exactly once
                //
                dw = (avis.rcFrame.right - avis.rcFrame.left) * gwZoom / 4 + HSPACE;
                gdwMicroSecPerPixel = muldiv32(1000000,
                                               avis.dwScale,
                                               dw * avis.dwRate);

                // Move one frame on the top video screen for each HSCROLL
                timehscroll = muldiv32(1000, avis.dwScale, avis.dwRate);
            }

        } else if (avis.fccType == streamtypeAUDIO) {

            //
            // If there are no video streams, we base everything on this
            // audio stream.
            //
            if (gpaviAudio == NULL && gpaviVideo == NULL) {

                // Show one sample per pixel
                gdwMicroSecPerPixel = muldiv32(1000000,
                                               avis.dwScale,
                                               avis.dwRate);
                // Move one sample per HSCROLL
                // Move at least enough to show movement
                timehscroll = muldiv32(1000, avis.dwScale, avis.dwRate);
            }

	    //
	    // Remember the first audio stream --- treat it specially
	    //
	    if (gpaviAudio == NULL)
	        gpaviAudio = gapavi[i];

	}

    }

    timeLength = timeEnd - timeStart;

    if (timeLength == 0)
        timeLength = 1;

    // Make sure HSCROLL scrolls enough to be noticeable.
    timehscroll = max(timehscroll, timeLength / SCROLLRANGE + 2);

    SetScrollRange(hwnd, SB_HORZ, 0, SCROLLRANGE, TRUE);
    SetScrollTime(hwnd, timeStart);

    FixScrollbars(hwnd);
}

/*----------------------------------------------------------------------------*\
|    FixWindowTitle()
|
|    Update the window title to reflect what's loaded.
\*----------------------------------------------------------------------------*/
void FixWindowTitle(HWND hwnd)
{
    TCHAR ach[80];

    wsprintf(ach, TEXT("%s %s"),
            (LPTSTR)gszAppName,
            (LPTSTR)gachFileName);

    SetWindowText(hwnd, ach);

    InvalidateRect(hwnd, NULL, TRUE);
}

/*----------------------------------------------------------------------------*\
|    FreeDrawStuff()
|
| Free up the resources associated with DrawDIB
\*----------------------------------------------------------------------------*/
void FreeDrawStuff(HWND hwnd)
{
    int	i;

    aviaudioStop();

    for (i = 0; i < gcpavi; i++) {
	if (gapgf[i]) {
	    AVIStreamGetFrameClose(gapgf[i]);
	    gapgf[i] = NULL;
	}
	if (ghdd[i]) {
	    DrawDibClose(ghdd[i]);
	    ghdd[i] = 0;
	}
    }
    SetScrollRange(hwnd, SB_HORZ, 0, 0, TRUE);
    gpaviVideo = gpaviAudio = NULL;
}

/*----------------------------------------------------------------------------*\
|    FreeAvi()
|
|    Free the resources associated with an open file.
\*----------------------------------------------------------------------------*/
void FreeAvi(HWND hwnd)
{
    int	i;

    FreeDrawStuff(hwnd);

    AVISaveOptionsFree(gcpavi, galpAVIOptions);

    for (i = 0; i < gcpavi; i++) {
	AVIStreamRelease(gapavi[i]);
    }

    if (gpfile)
	AVIFileRelease(gpfile);

    gpfile = NULL;

    // Good a place as any to make sure audio data gets freed
    if (lpAudio)
        GlobalFreePtr(lpAudio);
    lpAudio = NULL;

    gcpavi = 0;
}

/*----------------------------------------------------------------------------*\
|    InitBall()
|
|    Open up our fake "ball" file as an installible stream hander
\*----------------------------------------------------------------------------*/
void InitBall(HWND hwnd)
{
    // close everything down
    FreeAvi(hwnd);

    // The NewBall() function creates a PAVISTREAM we can use as if it was
    // an AVI file.
    gapavi[0] = NewBall();

    if (gapavi[0])
	gcpavi = 1;

    lstrcpy(gachFileName, TEXT("BALL"));
    InitStreams(hwnd);
    FixWindowTitle(hwnd);
}

/*----------------------------------------------------------------------------*\
|    InsertAVIFile()
|
|    Does most of the work of opening an AVI file.
\*----------------------------------------------------------------------------*/
void InsertAVIFile(PAVIFILE pfile, HWND hwnd, LPTSTR lpszFile)
{
    int		i;
    PAVISTREAM	pavi;

    //
    // Get all the streams from the new file
    //
    for (i = gcpavi; i <= MAXNUMSTREAMS; i++) {
	if (AVIFileGetStream(pfile, &pavi, 0L, i - gcpavi) != AVIERR_OK)
	    break;
        if (i == MAXNUMSTREAMS) {
            AVIStreamRelease(pavi);
            ErrMsg("Exceeded maximum number of streams");
            break;
        }
	gapavi[i] = pavi;
    }

    //
    // Couldn't get any streams out of this file
    //
    if (gcpavi == i && i != MAXNUMSTREAMS)
    {
        ErrMsg(TEXT("Unable to open %s"), lpszFile);
	if (pfile)
	    AVIFileRelease(pfile);
	return;
    }

    gcpavi = i;

    if (gpfile) {
	AVIFileRelease(pfile);
    } else
	gpfile = pfile;

    FreeDrawStuff(hwnd);
    InitStreams(hwnd);
    FixWindowTitle(hwnd);
}

/*----------------------------------------------------------------------------*\
|    InitAvi()
|
|    Open up a file through the AVIFile handlers.
\*----------------------------------------------------------------------------*/
void InitAvi(HWND hwnd, LPTSTR szFile, UINT wMenu)
{
    HRESULT	hr;
    PAVIFILE	pfile;

    hr = AVIFileOpen(&pfile, szFile, 0, 0L);

    if (hr != 0)
    {
        ErrMsg(TEXT("Unable to open %s"), szFile);
        return;
    }

    if (wMenu == MENU_OPEN)
	FreeAvi(hwnd);

    InsertAVIFile(pfile, hwnd, szFile);
}

/*----------------------------------------------------------------------------*\
|   AppInit( hInst, hPrev)						       |
|									       |
|   Description:							       |
|	This is called when the application is first loaded into	       |
|	memory.  It performs all initialization that doesn't need to be done   |
|	once per instance.						       |
|									       |
|   Arguments:								       |
|	hInstance	instance handle of current instance		       |
|	hPrev		instance handle of previous instance		       |
|									       |
|   Returns:								       |
|	TRUE if successful, FALSE if not				       |
|									       |
\*----------------------------------------------------------------------------*/
BOOL AppInit(HINSTANCE hInst, HINSTANCE hPrev, int sw,LPSTR szCmdLine)
{
    WNDCLASS	cls;
    HDC		hdc;

#ifdef BIDI
    const DWORD  dwExStyle = WS_EX_BIDI_SCROLL  | WS_EX_BIDI_MENU |WS_EX_BIDI_NOICON;
#else
    const DWORD  dwExStyle = 0;
#endif

    /* Save instance handle for DialogBoxs */
    ghInstApp = hInst;

    ghAccel = LoadAccelerators(hInst, MAKEINTATOM(ID_APP));

    if (szCmdLine && szCmdLine[0]) {
#ifdef UNICODE
	// convert to unicode
	lstrcpy(gachFileName, GetCommandLine());
#else
    	lstrcpy(gachFileName, szCmdLine);
#endif
    }

    if (!hPrev) {
	/*
	 *  Register a class for the main application window
	 */
        cls.hCursor        = LoadCursor(NULL,IDC_ARROW);
        cls.hIcon          = LoadIcon(hInst,MAKEINTATOM(ID_APP));
        cls.lpszMenuName   = MAKEINTATOM(ID_APP);
        cls.lpszClassName  = MAKEINTATOM(ID_APP);
        cls.hbrBackground  = (HBRUSH)(COLOR_WINDOW + 1);
        cls.hInstance      = hInst;
        cls.style          = CS_BYTEALIGNCLIENT | CS_VREDRAW | CS_HREDRAW | CS_DBLCLKS;
        cls.lpfnWndProc    = (LPWNDPROC)AppWndProc;
        cls.cbWndExtra     = 0;
        cls.cbClsExtra     = 0;

        if (!RegisterClass(&cls))
	    return FALSE;
    }

    //
    // Must be called before using any of the AVIFile routines
    //
    AVIFileInit();

    hfontApp = GetStockObject(ANSI_VAR_FONT);
    hdc = GetDC(NULL);
    SelectObject(hdc, hfontApp);
    GetTextMetrics(hdc, &tm);
    ReleaseDC(NULL, hdc);

    ghwndApp=CreateWindowEx(dwExStyle,
			    MAKEINTATOM(ID_APP),    // Class name
                            gszAppName,             // Caption
                            WS_OVERLAPPEDWINDOW,    // Style bits
                            CW_USEDEFAULT, 0,       // Position
                            320,300,                // Size
                            (HWND)NULL,             // Parent window (no parent)
                            (HMENU)NULL,            // use class menu
                            (HANDLE)hInst,          // handle to window instance
                            (LPSTR)NULL             // no params to pass on
                           );
    ShowWindow(ghwndApp,sw);

    ghLib = LoadLibrary(TEXT("palmap32.dll"));
    if (ghLib == NULL)
        gbCanPalMap = FALSE;
    else
        gbCanPalMap = TRUE;

    return TRUE;
}

/*----------------------------------------------------------------------------*\
|   WinMain( hInst, hPrev, lpszCmdLine, cmdShow )			       |
|                                                                              |
|   Description:                                                               |
|       The main procedure for the App.  After initializing, it just goes      |
|       into a message-processing loop until it gets a WM_QUIT message         |
|       (meaning the app was closed).                                          |
|                                                                              |
|   Arguments:                                                                 |
|	hInst		instance handle of this instance of the app	       |
|	hPrev		instance handle of previous instance, NULL if first    |
|       szCmdLine       ->null-terminated command line                         |
|       cmdShow         specifies how the window is initially displayed        |
|                                                                              |
|   Returns:                                                                   |
|       The exit code as specified in the WM_QUIT message.                     |
|                                                                              |
\*----------------------------------------------------------------------------*/
int PASCAL WinMain(HINSTANCE hInst, HINSTANCE hPrev, LPSTR szCmdLine, int sw)
{
    MSG     msg;

    /* Call initialization procedure */
    if (!AppInit(hInst,hPrev,sw,szCmdLine))
        return FALSE;

    /*
     * Polling messages from event queue
     */
    for (;;)
    {
        while (PeekMessage(&msg, NULL, 0, 0,PM_REMOVE))
        {
            if (msg.message == WM_QUIT)
                return msg.wParam;

	    if (TranslateAccelerator(ghwndApp, ghAccel, &msg))
		continue;
	
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }

	//
	// If we have no messages to dispatch, we do our background task...
	// If we're playing a file, we set the scroll bar to show the video
	// frames corresponding with the current playing audio sample
	//
        if (gfPlaying) {
	    LONG    l;

	    //
	    // Use the audio clock to tell how long we've been playing.  To
	    // maintain sync, it's important we use this clock.
	    //
	    l = aviaudioTime(); 	// returns -1 if no audio playing

	    //
	    // If we can't use the audio clock to tell us how long we've been
	    // playing, calculate it ourself
	    //
	    if (l == -1)
		l = timeGetTime() - glPlayStartTime + glPlayStartPos;

	    if (l != GetScrollTime(ghwndApp)) {
	        if (l < timeStart)	// make sure number isn't out of bounds
		    l = timeStart;
	        if (l > timeEnd)	// looks like we're all done!
                    FORWARD_WM_COMMAND(ghwndApp, MENU_STOP, NULL, 0, SendMessage);

		SetScrollTime(ghwndApp, l);
		InvalidateRect(ghwndApp, NULL, FALSE);
		UpdateWindow(ghwndApp);
		continue;
	    }
	}
	
	WaitMessage();
    }

    // not reached
    return msg.wParam;
}

typedef BYTE * HPBYTE;
typedef UNALIGNED short * HPSHORT;

/*----------------------------------------------------------------------------*\
|    PaintVideo()
|
|    Draw a video frame in the specified rect.
\*----------------------------------------------------------------------------*/
void PaintVideo(HDC hdc, RECT rcFrame, int iStream, LPBITMAPINFOHEADER lpbi, LONG lCurFrame, LONG lPos)
{
    int		iLen;
    char	ach[200];
    RECT	rc;

    //
    // If we have a picture, draw it
    //
    if (lpbi)
    {
        //
        // use the palette of the first video stream
        //
        DrawDibDraw(ghdd[iStream],hdc,
	    rcFrame.left, rcFrame.top,
	    rcFrame.right - rcFrame.left,
	    rcFrame.bottom - rcFrame.top,
	    lpbi, NULL,
	    0, 0, -1, -1,
	    gapavi[iStream] == gpaviVideo ? 0 : DDF_BACKGROUNDPAL);

        iLen = wsprintf(ach, TEXT("%ld %ld.%03lds"),
	    lCurFrame, lPos/1000, lPos%1000);
    }

    //
    // Before or after the movie (or read error) draw GRAY
    //
    else {
        if (gapgf[iStream])
	    SelectObject(hdc,GetStockObject(DKGRAY_BRUSH));
        else
	    SelectObject(hdc,GetStockObject(LTGRAY_BRUSH));

        PatBlt(hdc,
	    rcFrame.left, rcFrame.top,
	    rcFrame.right - rcFrame.left,
	    rcFrame.bottom - rcFrame.top,
	    PATCOPY);
        iLen = 0;
        ach[0] = TEXT('\0');
    }

    //
    // print something meaningful under the frame
    //
    rc.left = rcFrame.left;
    rc.right = rcFrame.right + HSPACE;
    rc.top = rcFrame.bottom + VSPACE;
    rc.bottom = rc.top + TSPACE;
    ExtTextOut(hdc, rc.left, rc.top, ETO_OPAQUE,
	       &rc, ach, iLen, NULL);
}

/*----------------------------------------------------------------------------*\
|    PaintAudio()
|
|    Draw some samples of audio inside the given rectangle.
\*----------------------------------------------------------------------------*/
void PaintAudio(HDC hdc, PRECT prc, PAVISTREAM pavi, LONG lStart, LONG lLen)
{
    PCMWAVEFORMAT wf;
    int i;
    int x,y;
    int w,h;
    BYTE b;
    HBRUSH hbr;
    RECT rc = *prc;
    LONG    lBytes;
    LONG    l, lLenOrig = lLen;
    LONG    lWaveBeginTime = AVIStreamStartTime(pavi);
    LONG    lWaveEndTime   = AVIStreamEndTime(pavi);

    //
    // We can't draw before the beginning of the stream - adjust
    //
    if (lStart < lWaveBeginTime) {
	lLen -= lWaveBeginTime - lStart;
	lStart = lWaveBeginTime;
	// right justify the legal samples in the rectange - don't stretch
	rc.left = rc.right - (int)muldiv32(rc.right - rc.left, lLen, lLenOrig);
    }

    //
    // We can't draw past the end of the stream
    //
    if (lStart + lLen > lWaveEndTime) {
	lLenOrig = lLen;
	lLen = max(0, lWaveEndTime - lStart);	// maybe nothing to draw!
	// left justify the legal samples in the rectange - don't stretch
	rc.right = rc.left + (int)muldiv32(rc.right - rc.left, lLen, lLenOrig);
    }

    // Now change and work with samples, not time.
    l = lStart;
    lStart = AVIStreamTimeToSample(pavi, lStart);
    lLen = AVIStreamTimeToSample(pavi, l + lLen) - lStart;

    //
    // Get the format of the wave data
    //
    l = sizeof(wf);
    AVIStreamReadFormat(pavi, lStart, &wf, &l);
    if (!l)
        return;

    w = rc.right - rc.left;
    h = rc.bottom - rc.top;

    //
    // We were starting before the beginning or continuing past the end.
    // We're not painting in the whole original rect --- use a dark background
    //
    if (rc.left > prc->left) {
        SelectObject(hdc, GetStockObject(DKGRAY_BRUSH));
 	PatBlt(hdc, prc->left, rc.top, rc.left - prc->left,
		    rc.bottom - rc.top, PATCOPY);
    }

    if (rc.right < prc->right) {
        SelectObject(hdc, GetStockObject(DKGRAY_BRUSH));
 PatBlt(hdc, rc.right, rc.top, prc->right - rc.right,
		    rc.bottom - rc.top, PATCOPY);
    }

#define BACKBRUSH  (GetSysColor(COLOR_BTNFACE))
#define MONOBRUSH  (GetSysColor(COLOR_BTNSHADOW))
#define LEFTBRUSH  (RGB(0,0,255))
#define RIGHTBRUSH (RGB(0,255,0))
#define HPOSBRUSH  (RGB(255,0,0))

    //
    // Paint the background
    //
    hbr = SelectObject(hdc, CreateSolidBrush(BACKBRUSH));
    PatBlt(hdc, rc.left, rc.top, w, h, PATCOPY);
    DeleteObject(SelectObject(hdc, hbr));

    //
    // !!! we can only paint PCM data right now.  Sorry!
    //
    if (wf.wf.wFormatTag != WAVE_FORMAT_PCM)
        return;

    //
    // How many bytes are we painting? Alloc some space for them
    //
    lBytes = lLen * wf.wf.nChannels * wf.wBitsPerSample / 8;
    if (!lpAudio)
        lpAudio = GlobalAllocPtr (GHND, lBytes);
    else if ((LONG)GlobalSizePtr(lpAudio) < lBytes)
        lpAudio = GlobalReAllocPtr(lpAudio, lBytes, GMEM_MOVEABLE);

    if (!lpAudio)
        return;

    //
    // Read in the wave data
    //
    AVIStreamRead(pavi, lStart, lLen, lpAudio, lBytes, NULL, &l);

    if (l != lLen)
	return;

    //
    // !!! Flickers less painting it NOW or LATER?
    // First show the current position as a bar
    //
    hbr = SelectObject(hdc, CreateSolidBrush(HPOSBRUSH));
    PatBlt(hdc, prc->right / 2, prc->top, 1, prc->bottom - prc->top, PATCOPY);
    DeleteObject(SelectObject(hdc, hbr));

    //
    // Paint monochrome wave data
    //
    if (wf.wf.nChannels == 1) {

	//
	// Draw the x-axis
	//
        hbr = SelectObject(hdc, CreateSolidBrush(MONOBRUSH));
        y = rc.top + h/2;
        PatBlt(hdc, rc.left, y, w, 1, PATCOPY);

	//
	// 8 bit data is centred around 0x80
	//
        if (wf.wBitsPerSample == 8) {
            for (x=0; x<w; x++) {

		// which byte of audio data belongs at this pixel?
                b = *((HPBYTE)lpAudio + muldiv32(x,lLen,w));

                if (b > 0x80) {
                    i = y - (int)muldiv32(b-0x80,(h/2),128);
                    PatBlt(hdc, rc.left+x, i, 1, y-i, PATCOPY);
                }
                else {
                    i = y + (int)muldiv32(0x80-b,(h/2),128);
                    PatBlt(hdc, rc.left+x, y, 1, i-y, PATCOPY);
                }
            }
        }

	//
	// 16 bit data is centred around 0x00
	//
        else if (wf.wBitsPerSample == 16) {
            for (x=0; x<w; x++) {

		// Don't make any assumptions about INT size !
		// which byte of audio data belongs at this pixel?
                i = *((HPSHORT)lpAudio + muldiv32(x,lLen,w));
                if (i > 0) {
                   i = y - (int) ((LONG)i * (h/2) / 32768);
                   PatBlt(hdc, rc.left+x, i, 1, y-i, PATCOPY);
                }
                else {
                   i = (int) ((LONG)i * (h/2) / 32768);
                   PatBlt(hdc, rc.left+x, y, 1, -i, PATCOPY);
                }
            }
        }
        DeleteObject(SelectObject(hdc, hbr));
    } // endif mono

    //
    // Draw stereo waveform data
    //
    else if (wf.wf.nChannels == 2) {

	//
	// 8 bit data is centred around 0x80
	//
        if (wf.wBitsPerSample == 8) {

            // Left channel
            hbr = SelectObject(hdc, CreateSolidBrush(LEFTBRUSH));
            y = rc.top + h/4;
            PatBlt(hdc, rc.left, y, w, 1, PATCOPY);

            for (x=0; x<w; x++) {
                b = *((HPBYTE)lpAudio + muldiv32(x,lLen,w) * 2);

                if (b > 0x80) {
                    i = y - (int)muldiv32(b-0x80,(h/4),128);
                    PatBlt(hdc, rc.left+x, i, 1, y-i, PATCOPY);
                }
                else {
                    i = y + (int)muldiv32(0x80-b,(h/4),128);
                    PatBlt(hdc, rc.left+x, y, 1, i-y, PATCOPY);
                }
            }
            DeleteObject(SelectObject(hdc, hbr));

            // Right channel
            hbr = SelectObject(hdc, CreateSolidBrush(RIGHTBRUSH));
            y = rc.top + h * 3 / 4;
            PatBlt(hdc, rc.left, y, w, 1, PATCOPY);

            for (x=0; x<w; x++) {
                b = *((HPBYTE)lpAudio + muldiv32(x,lLen,w) * 2 + 1);

                if (b > 0x80) {
                    i = y - (int)muldiv32(b-0x80,(h/4),128);
                    PatBlt(hdc, rc.left+x, i, 1, y-i, PATCOPY);
                }
                else {
                    i = y + (int)muldiv32(0x80-b,(h/4),128);
                    PatBlt(hdc, rc.left+x, y, 1, i-y, PATCOPY);
                }
            }
            DeleteObject(SelectObject(hdc, hbr));
        }

	//
	// 16 bit data is centred around 0x00
	//
        else if (wf.wBitsPerSample == 16) {

            // Left channel
            hbr = SelectObject(hdc, CreateSolidBrush(LEFTBRUSH));
            y = rc.top + h/4;
            PatBlt(hdc, rc.left, y, w, 1, PATCOPY);

            for (x=0; x<w; x++) {
		// Don't make any assumptions about INT size !
                i = *((HPSHORT)lpAudio + muldiv32(x,lLen,w) * 2);
                if (i > 0) {
                    i = y - (int) ((LONG)i * (h/4) / 32768);
                    PatBlt(hdc, rc.left+x, i, 1, y-i, PATCOPY);
                }
                else {
                    i = (int) ((LONG)i * (h/4) / 32768);
                    PatBlt(hdc, rc.left+x, y, 1, -i, PATCOPY);
                }
            }
            DeleteObject(SelectObject(hdc, hbr));

            // Right channel
            hbr = SelectObject(hdc, CreateSolidBrush(RIGHTBRUSH));
            y = rc.top + h * 3 / 4;
            PatBlt(hdc, rc.left, y, w, 1, PATCOPY);

            for (x=0; x<w; x++) {
		// Don't make any assumptions about INT size !
                i = *((HPSHORT)lpAudio + muldiv32(x,lLen,w) * 2 + 1);
                if (i > 0) {
                   i = y - (int) ((LONG)i * (h/4) / 32768);
                   PatBlt(hdc, rc.left+x, i, 1, y-i, PATCOPY);
                }
                else {
                   i = (int) ((LONG)i * (h/4) / 32768);
                   PatBlt(hdc, rc.left+x, y, 1, -i, PATCOPY);
                }
            }
            DeleteObject(SelectObject(hdc, hbr));
        }
    } // endif stereo
}

/*----------------------------------------------------------------------------*\
|    PaintStuff()
|
|    Paint the screen with what we plan to show them.
\*----------------------------------------------------------------------------*/
long PaintStuff(HDC hdc, HWND hwnd, BOOL fDrawEverything)
{
    int         yStreamTop;
    TCHAR       ach[400];
    int         iFrameWidth, iLen;
    int         n;
    int         nFrames;
    LPBITMAPINFOHEADER lpbi;
    LONG        lTime;
    LONG        lSize = 0;
    LONG        lAudioStart;
    LONG        lAudioLen;
    RECT        rcFrame, rcC;
    int         i;
    LONG        lFrame;
    LONG        lCurFrame;
    HBRUSH      hbr;
    RECT        rc;

    SelectObject(hdc, hfontApp);

    #define PRINT(sz) \
	(TextOut(hdc, TSPACE, yStreamTop, sz, lstrlen(sz)), \
	yStreamTop += tm.tmHeight+1)

    #define PF1(sz,a)                   (wsprintf(ach, sz, a), PRINT(ach))
    #define PF2(sz,a,b)                 (wsprintf(ach, sz, a, b), PRINT(ach))
    #define PF3(sz,a,b,c)               (wsprintf(ach, sz, a, b, c), PRINT(ach))
    #define PF4(sz,a,b,c,d)             (wsprintf(ach, sz, a, b, c, d), PRINT(ach))
    #define PF5(sz,a,b,c,d,e)           (wsprintf(ach, sz, a, b, c, d, e), PRINT(ach))
    #define PF6(sz,a,b,c,d,e,f)         (wsprintf(ach, sz, a, b, c, d, e, f), PRINT(ach))
    #define PF7(sz,a,b,c,d,e,f,g)       (wsprintf(ach, sz, a, b, c, d, e, f, g), PRINT(ach))
    #define PF8(sz,a,b,c,d,e,f,g,h)     (wsprintf(ach, sz, a, b, c, d, e, f, g, h), PRINT(ach))
    #define PF9(sz,a,b,c,d,e,f,g,h,i)   (wsprintf(ach, sz, a, b, c, d, e, f, g, h, i), PRINT(ach))

    GetClientRect(hwnd, &rcC);

    //
    // Look at scrollbars to find current position
    //
    lTime = GetScrollTime(hwnd);
    yStreamTop = -GetScrollPos(hwnd, SB_VERT);

    //
    // Walk through all streams and draw something
    //
    for (i=0; i<gcpavi; i++) {
	AVISTREAMINFO	avis;
	LONG	lEnd, lEndTime, lNextFmt, lPrevFmt, l;
	LONG	lPos, lNextKey, lPrevKey, lNextAny, lPrevAny;

	AVIStreamInfo(gapavi[i], &avis, sizeof(avis));
	FIXCC(avis.fccHandler);
	FIXCC(avis.fccType);

	l = sizeof(abFormat);
	AVIStreamReadFormat(gapavi[i],0, &abFormat, &l);

	PF7(TEXT("Stream%d [%4.4hs/%4.4hs] Start: %ld Length: %ld (%ld.%03ld sec)             "),
		    i,
		    (LPSTR)&avis.fccType,
		    (LPSTR)&avis.fccHandler,
		    AVIStreamStart(gapavi[i]),
		    AVIStreamLength(gapavi[i]),
		    AVIStreamLengthTime(gapavi[i]) / 1000,
		    AVIStreamLengthTime(gapavi[i]) % 1000);

	lPos = AVIStreamTimeToSample(gapavi[i], lTime);
	AVIStreamSampleSize(gapavi[i], lPos, &lSize);

	lPrevKey = AVIStreamFindSample(gapavi[i], lPos, FIND_PREV|FIND_KEY);
	lPrevAny = AVIStreamFindSample(gapavi[i], lPos, FIND_PREV|FIND_ANY);
	lPrevFmt = AVIStreamFindSample(gapavi[i], lPos, FIND_PREV|FIND_FORMAT);

	lNextKey = AVIStreamFindSample(gapavi[i], lPos, FIND_NEXT|FIND_KEY);
	lNextAny = AVIStreamFindSample(gapavi[i], lPos, FIND_NEXT|FIND_ANY);
	lNextFmt = AVIStreamFindSample(gapavi[i], lPos, FIND_NEXT|FIND_FORMAT);

	PF5(TEXT("Pos:%ld Time:%ld.%03ld sec Size:%ld bytes %s                                 "),
		lPos, lTime/1000, lTime%1000, lSize,
		(LPTSTR)(lPos == lPrevKey ? TEXT("Key") : TEXT("")));

	PF6(TEXT("PrevKey=%ld NextKey=%ld, PrevAny=%ld NextAny=%ld, PrevFmt=%ld NextFmt=%ld                      "),
	    lPrevKey, lNextKey,
	    lPrevAny, lNextAny,
	    lPrevFmt, lNextFmt);

	//
	// Draw a VIDEO stream
	//
	if (avis.fccType == streamtypeVIDEO) {
	    if (gapgf[i] == NULL)
		continue;

	    lpbi = (LPBITMAPINFOHEADER)abFormat;
	    FIXCC(lpbi->biCompression);

	    //
	    // display video format
	    //
	    //  Video: 160x120x8 (cram)
	    //
	    PF4(TEXT("Format: %dx%dx%d (%4.4hs)"),
		(int)lpbi->biWidth,
		(int)lpbi->biHeight,
		(int)lpbi->biBitCount,
		(LPSTR)&lpbi->biCompression);

            //
	    // Which frame belongs at this time?
	    //
	    lEndTime = AVIStreamEndTime(gapavi[i]);
	    if (lTime <= lEndTime)
		lFrame = AVIStreamTimeToSample(gapavi[i], lTime);
	    else {	// we've scrolled past the end of this stream
		lEnd = AVIStreamEnd(gapavi[i]);
		lFrame = lEnd + AVIStreamTimeToSample(
			gapavi[i], lTime - lEndTime);
	    }

	    //
	    // how wide is each frame to paint?
	    //
	    iFrameWidth = (avis.rcFrame.right - avis.rcFrame.left) *
		gwZoom / 4 + HSPACE;

	    //
	    // how many frames can we fit on each half of the screen?
	    //
	    nFrames = (rcC.right - iFrameWidth) / (2 * iFrameWidth);
	    if (nFrames < 0)
		nFrames = 0;    // at least draw *something*

	    //
	    // Step through all the frames we'll draw
	    //
	    for (n=-nFrames; n<=nFrames; n++)
	    {

                //
                // Each video stream is drawn as a horizontal line of
                // frames, very close together.
                // The first video stream shows a different frame in
                // each square. Thus the scale of time is determined
                // by the first video stream.
                // Every other video stream shows whatever
                // frame belongs at the time corresponding to the mid-
                // point of each square.
                //
		if (gapavi[i] == gpaviVideo) {

                    //
                    // by definition, we know what frame we're drawing..
                    // (lFrame-n), (lFrame-(n-1)), ..., (lFrame), ...,
                    // (lFrame+(n-1)), (lFrame+n)
                    //
		    lCurFrame = lFrame + n;

		    //
		    // what time is it at that frame?  This number will be
		    // printed underneath the frame
		    //
		    l = AVIStreamSampleToTime(gapavi[i], lCurFrame);

		} else {	// NOT the first video stream

		    //
		    // What time is it at the left edge of the square
		    // we'll draw?  That's what frame we use.
		    //
                    if (n<0) {
                        l = lTime - muldiv32(-n * iFrameWidth + HSPACE,
					gdwMicroSecPerPixel, 1000);
                    }
                    else {
		        l = lTime + muldiv32(n * iFrameWidth,
					gdwMicroSecPerPixel, 1000);
                    }

                    //
		    // What frame belongs to that time?
		    //
		    lCurFrame = AVIStreamTimeToSample(gapavi[i], l);

                    //
                    // what time is it at that frame? This number will
                    // be printed underneath the frame
                    //
                    l = AVIStreamSampleToTime(gapavi[i], lCurFrame);
		}
		
		// !!!
		// Could actually return an LPBI for invalid frames
		// so we better force it to NULL.
		//
		if (gapgf[i] && lCurFrame >= AVIStreamStart(gapavi[i]))
		    lpbi = AVIStreamGetFrame(gapgf[i], lCurFrame);
		else
		    lpbi = NULL;

		//
		// Figure out where to draw this frame
		//
		rcFrame.left   = rcC.right / 2 -
		    ((avis.rcFrame.right - avis.rcFrame.left) * gwZoom / 4)
		    / 2 + (n * iFrameWidth);
		rcFrame.top    = yStreamTop;
		rcFrame.right  = rcFrame.left +
			(avis.rcFrame.right - avis.rcFrame.left)*gwZoom/4;
		rcFrame.bottom = rcFrame.top +
			(avis.rcFrame.bottom - avis.rcFrame.top)*gwZoom/4;

		//
		// draw a border around the current frame.  Make the
		// one around the centre frame a special colour.
		//
		if (n == 0)
		    hbr = CreateSolidBrush(RGB(255,0,0));
		else
		    hbr = CreateSolidBrush(RGB(255,255,255));

		InflateRect(&rcFrame, 1, 1);
		FrameRect(hdc, &rcFrame, hbr);
		InflateRect(&rcFrame, -1, -1);
		DeleteObject (hbr);

		//
		// Now draw the video frame in the computed rectangle
		//
		PaintVideo(hdc, rcFrame, i, lpbi, lCurFrame, l);

	    }
	
	    //
	    // Move down to where we can draw the next stream
	    //
	    yStreamTop += (rcFrame.bottom - rcFrame.top) +
			TSPACE;
	}

	//
	// Draw an AUDIO stream
	//
	else if (avis.fccType == streamtypeAUDIO) {
	    LPWAVEFORMAT pwf = (LPWAVEFORMAT)abFormat;
	    TCHAR *szFmt;

	    if (pwf->wFormatTag == 1) {  // PCM
		if (pwf->nChannels == 1)
		    szFmt = TEXT("Format: Mono %ldHz %dbit");
		else
		    szFmt = TEXT("Format: Stereo %ldHz %dbit");
	    }
	    else if (pwf->wFormatTag == 2) {  // ADPCM
		if (pwf->nChannels == 1)
		    szFmt = TEXT("Format: ADPCM Mono %ldHz %dbit");
		else
		    szFmt = TEXT("Format: ADPCM Stereo %ldHz %dbit");
	    }
	    else {
		if (pwf->nChannels == 1)
		    szFmt = TEXT("Format: Compressed Mono %ldHz %dbit");
		else
		    szFmt = TEXT("Format: Compressed Stereo %ldHz %dbit");
	    }

	    PF2(szFmt,(LONG)pwf->nSamplesPerSec,
		(int)((LONG)pwf->nAvgBytesPerSec / pwf->nSamplesPerSec /
		      pwf->nChannels) * 8);

	    //
	    // Figure out which samples are visible
	    //
	    lAudioStart = lTime - muldiv32(rcC.right / 2,
					gdwMicroSecPerPixel, 1000);
	    lAudioLen = 2 * (lTime - lAudioStart);

	    //
	    // Make rectangle to draw audio into
	    //
	    rc.left = rcC.left;
	    rc.right = rcC.right;
	    rc.top = yStreamTop;
	    rc.bottom = rc.top + AUDIOVSPACE * gwZoom / 4;

	    //
	    // Actually paint the audio
	    //
	    PaintAudio(hdc, &rc, gapavi[i], lAudioStart, lAudioLen);

            //
            // Move down to where we can draw the next stream
            //
            yStreamTop += AUDIOVSPACE * gwZoom / 4;

	}
	else if (avis.fccType == streamtypeTEXT) {
	    LONG    lPos;
	    int	    iLeft;

	    lPos = AVIStreamTimeToSample(gapavi[i],
					 lTime -
					 muldiv32((rcC.right - rcC.left),
						gdwMicroSecPerPixel,
						1000));

	    if (lPos < 0)
		lPos = 0;

	    PatBlt(hdc, rcC.left, yStreamTop,
		   rcC.right - rcC.left, TSPACE + TSPACE,
		   WHITENESS);
	
	    while (lPos < AVIStreamEnd(gapavi[i]) - 1) {

                LONG    lStreamTime = AVIStreamSampleToTime(gapavi[i], lPos);

                // What pixel is it at this time?

                if (lStreamTime < lTime) {              // pixel will be left of centre
                    iLeft = (rcC.right + rcC.left) / 2 -
			    (int) muldiv32(lTime - lStreamTime, 1000,  gdwMicroSecPerPixel);
                }
                else {                                  // pixel is at, or right of, centre
                    iLeft = (rcC.right + rcC.left) / 2 +
			    (int) muldiv32(lStreamTime - lTime, 1000,  gdwMicroSecPerPixel);
                }
		if (iLeft >= rcC.right)
		    break;

		AVIStreamRead(gapavi[i], lPos, 1, ach, sizeof(ach), &l, NULL);

		if (l)
		    TextOut(hdc, iLeft, yStreamTop, ach, (int) l - 1);

		iLen = wsprintf(ach, TEXT("%ld"), lPos);
		TextOut(hdc, iLeft, yStreamTop + TSPACE, ach, iLen);

		lPos += 1;
	    }

	    yStreamTop += TSPACE + TSPACE;
	}

	yStreamTop += VSPACE;

        //
        // Give up once we're painting below the bottom of the window
        //
        if (!fDrawEverything && yStreamTop >= rcC.bottom)
            break;
    }

    //
    // How many lines did we draw?
    //
    return yStreamTop + GetScrollPos(hwnd, SB_VERT);
}

/*----------------------------------------------------------------------------*\
|   AppWndProc( hwnd, uiMessage, wParam, lParam )			       |
|                                                                              |
|   Description:                                                               |
|       The window proc for the app's main (tiled) window.  This processes all |
|       of the parent window's messages.                                       |
|                                                                              |
|   Arguments:                                                                 |
|	hwnd		window handle for the window			       |
|       msg             message number                                         |
|       wParam          message-dependent                                      |
|       lParam          message-dependent                                      |
|                                                                              |
|   Returns:                                                                   |
|       0 if processed, nonzero if ignored                                     |
|                                                                              |
\*----------------------------------------------------------------------------*/
LONG WINAPI  AppWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    PAINTSTRUCT ps;
    BOOL        f;
    HDC		hdc;
    RECT	rc;
    LONG	l;

    switch (msg) {

        //
        // If we passed a command line filename, open it
        //
        case WM_CREATE:
            if (gachFileName[0])
                InitAvi(hwnd, gachFileName, MENU_OPEN);
	    break;

        case WM_COMMAND:
            return AppCommand(hwnd,msg,wParam,lParam);

        case WM_INITMENU:
            f = gfVideoFound || gfAudioFound;
            EnableMenuItem((HMENU)wParam, MENU_OPTIONS,f ? MF_ENABLED :
			MF_GRAYED);

            f = gcpavi > 0;
            EnableMenuItem((HMENU)wParam, MENU_SAVEAS, f ? MF_ENABLED :
			MF_GRAYED);
            EnableMenuItem((HMENU)wParam, MENU_NEW,    f ? MF_ENABLED :
			MF_GRAYED);
            EnableMenuItem((HMENU)wParam, MENU_ADD,    f ? MF_ENABLED :
			MF_GRAYED);
	
            f = (gpfile != 0) && gbCanPalMap && gfVideoFound;
            EnableMenuItem((HMENU)wParam, MENU_NEWPALETTE, f ? MF_ENABLED :
                        MF_GRAYED);

	    f = gfVideoFound || gfAudioFound;
            EnableMenuItem((HMENU)wParam, MENU_PLAY,
			(f && !gfPlaying) ? MF_ENABLED : MF_GRAYED);
            EnableMenuItem((HMENU)wParam, MENU_STOP,
                        (f && gfPlaying) ? MF_ENABLED : MF_GRAYED);

	    CheckMenuItem((HMENU)wParam, MENU_ZOOMQUARTER,
		    (gwZoom == 1) ? MF_CHECKED : MF_UNCHECKED);
	    CheckMenuItem((HMENU)wParam, MENU_ZOOMHALF,
		    (gwZoom == 2) ? MF_CHECKED : MF_UNCHECKED);
	    CheckMenuItem((HMENU)wParam, MENU_ZOOM1,
		    (gwZoom == 4) ? MF_CHECKED : MF_UNCHECKED);
	    CheckMenuItem((HMENU)wParam, MENU_ZOOM2,
		    (gwZoom == 8) ? MF_CHECKED : MF_UNCHECKED);
	    CheckMenuItem((HMENU)wParam, MENU_ZOOM4,
		    (gwZoom == 16) ? MF_CHECKED : MF_UNCHECKED);
	    	
            break;

        //
        // During a wait state (eg saving) don't let us choose any menus
        //
	case WM_NCHITTEST:
	    if (fWait) {

		// Let windows tell us where the cursor is
		lParam = DefWindowProc(hwnd,msg,wParam,lParam);

		// If it's over a menu, pretend it's in the client (force
		// an hourglass)
		if (lParam == HTMENU)
		    lParam = HTCLIENT;

		return lParam;
	    }
	    break;

	//
	// Set vertical scrollbar for scrolling streams
	//
	case WM_SIZE:
	    GetClientRect(hwnd, &rc);

            //
            // There is not enough vertical room to show all streams. Scrollbars
            // are required.
            //
	    if (vertHeight > rc.bottom) {
	        vertSBLen = vertHeight - rc.bottom;
	        SetScrollRange(hwnd, SB_VERT, 0, (int)vertSBLen, TRUE);

            //
            // Everything fits vertically. No scrollbar necessary.
            //
	    } else {
	        vertSBLen = 0;
	        SetScrollRange(hwnd, SB_VERT, 0, 0, TRUE);
	    }
	    break;

        //
        // During a wait state, show an hourglass over our client area
        //
        case WM_SETCURSOR:
            if (fWait && LOWORD(lParam) == HTCLIENT) {
                SetCursor(LoadCursor(NULL, IDC_WAIT));
                return TRUE;
            }
            break;

	//
	// We're out of here!
	//
        case WM_DESTROY:
            FreeAvi(hwnd);	// close all open streams
	    AVIFileExit();	// shuts down the AVIFile system
            FreeLibrary(ghLib);
	    PostQuitMessage(0);
	    break;

	//
	// Don't let us close ourselves in a wait state (eg saving)
	//
        case WM_CLOSE:
	    if (fWait)
		return 0;
            break;

	//
	// Block keyboard access to menus if waiting
	//
	case WM_SYSCOMMAND:
	    switch (wParam & 0xFFF0) {
		case SC_KEYMENU:
		    if (fWait)
			return 0;
		    break;
	    }
	    break;

        case WM_PALETTECHANGED:
	    // It came from us.  Ignore it
            if ((HWND)wParam == hwnd)
                break;

	case WM_QUERYNEWPALETTE:
            hdc = GetDC(hwnd);

            //
            // Realize the palette of the first video stream
            // !!! Assumes the first stream is video
            //
            if (ghdd[0] && (f = DrawDibRealize(ghdd[0], hdc, FALSE)))
                InvalidateRect(hwnd,NULL,TRUE);

            ReleaseDC(hwnd,hdc);

            return f;
	
        case WM_ERASEBKGND:
            break;

        case WM_PAINT:
            hdc = BeginPaint(hwnd,&ps);

	    PaintStuff(hdc, hwnd, FALSE);

            EndPaint(hwnd,&ps);
            break;

	//
	// handle the keyboard interface
	//
	case WM_KEYDOWN:
            switch (wParam)
            {
                case VK_UP:    PostMessage (hwnd,WM_VSCROLL,SB_LINEUP,0L);   break;
                case VK_DOWN:  PostMessage (hwnd,WM_VSCROLL,SB_LINEDOWN,0L); break;
                case VK_PRIOR: PostMessage (hwnd,WM_HSCROLL,SB_PAGEUP,0L);   break;
                case VK_NEXT:  PostMessage (hwnd,WM_HSCROLL,SB_PAGEDOWN,0L); break;
                case VK_HOME:  PostMessage (hwnd,WM_HSCROLL,SB_THUMBPOSITION,0L);     break;
                case VK_END:   PostMessage (hwnd,WM_HSCROLL,SB_THUMBPOSITION,0x7FFF); break;
                case VK_LEFT:  PostMessage (hwnd,WM_HSCROLL,SB_LINEUP,0L);   break;
                case VK_RIGHT: PostMessage (hwnd,WM_HSCROLL,SB_LINEDOWN,0L); break;
	    }
	    break;

        case WM_HSCROLL:
            l = GetScrollTime(hwnd);

            switch (GET_WM_HSCROLL_CODE(wParam, lParam)) {
                case SB_LINEDOWN:      l += timehscroll;  break;
                case SB_LINEUP:        l -= timehscroll;  break;
                case SB_PAGEDOWN:      l += timeLength/10; break;
                case SB_PAGEUP:        l -= timeLength/10; break;
                case SB_THUMBTRACK:
                case SB_THUMBPOSITION:
			l = GET_WM_HSCROLL_POS(wParam, lParam);
			l = timeStart + muldiv32(l, timeLength, SCROLLRANGE);
			break;
            }

	    if (l < timeStart)
		l = timeStart;

	    if (l > timeEnd)
		l = timeEnd;

	    if (l == (LONG)GetScrollTime(hwnd))
		break;
	
	    SetScrollTime(hwnd, l);
            InvalidateRect(hwnd, NULL, FALSE);
            UpdateWindow(hwnd);
            break;

        case WM_VSCROLL:
            l = GetScrollPos(hwnd, SB_VERT);
	    GetClientRect(hwnd, &rc);

            switch (GET_WM_VSCROLL_CODE(wParam, lParam)) {
                case SB_LINEDOWN:      l += 10;  break;
                case SB_LINEUP:        l -= 10;  break;
                case SB_PAGEDOWN:      l += rc.bottom; break;
                case SB_PAGEUP:        l -= rc.bottom; break;
                case SB_THUMBTRACK:
                case SB_THUMBPOSITION:
		    l = GET_WM_VSCROLL_POS(wParam, lParam);
		    break;
            }

	    if (l < 0)
		l = 0;

	    if (l > vertSBLen)
		l = vertSBLen;

	    if (l == GetScrollPos(hwnd, SB_VERT))
		break;
	
	    SetScrollPos(hwnd, SB_VERT, (int)l, TRUE);
            InvalidateRect(hwnd, NULL, TRUE);
            UpdateWindow(hwnd);
            break;

	//
	// Wave driver wants to tell us something.  Pass it on.
	//
	case MM_WOM_OPEN:
	case MM_WOM_DONE:
	case MM_WOM_CLOSE:
	    aviaudioMessage(hwnd, msg, wParam, lParam);
	    break;
    }
    return DefWindowProc(hwnd,msg,wParam,lParam);
}

/*----------------------------------------------------------------------------*\
|    SaveCallback()
|
|    Our save callback that prints our progress in our window title bar.
\*----------------------------------------------------------------------------*/
BOOL PASCAL  SaveCallback(int iProgress)
{
    TCHAR    ach[128];

    wsprintf(ach, TEXT("%s - Saving %s: %d%%"),
        (LPTSTR) gszAppName, (LPTSTR) gachSaveFileName, iProgress);

    SetWindowText(ghwndApp, ach);

    //
    // Give ourselves a chance to abort
    //
    return WinYield();
}

/*----------------------------------------------------------------------------*\
|    AppCommand()
|
|    Process all of our WM_COMMAND messages.
\*----------------------------------------------------------------------------*/
LONG PASCAL AppCommand (HWND hwnd, unsigned msg, WPARAM wParam, LPARAM lParam)
{
    switch(GET_WM_COMMAND_ID(wParam, lParam))
    {
	//
	// Our about box
	//
        case MENU_ABOUT:
            DialogBox(ghInstApp, MAKEINTRESOURCE(IDD_ABOUT), hwnd, AboutDlgProc);
            break;

	//
	// We want out of here!
	//
	case MENU_EXIT:
	    PostMessage(hwnd,WM_CLOSE,0,0L);
            break;

	//
	// Set the compression options for each stream - pass an array of
	// streams and an array of compression options structures
	//
        case MENU_OPTIONS:
            AVISaveOptions(hwnd, ICMF_CHOOSE_KEYFRAME | ICMF_CHOOSE_DATARATE
			| ICMF_CHOOSE_PREVIEW,
		gcpavi, gapavi, galpAVIOptions);
	    break;

        //
        // Use palmap to set the no of colours to save with
        //
        case MENU_NEWPALETTE:
            {
               PAVISTREAM     psMappedStream;
               PAVISTREAM     psCurrent;
               AVISTREAMINFO  avis;
               FARPROC        lpfnAVICreateMappedStream;
#define AVICreateMappedStream 11       //Cardinal for procedure in palmap32.dll

               lpfnAVICreateMappedStream = GetProcAddress(ghLib,MAKEINTRESOURCE(AVICreateMappedStream));
               if (lpfnAVICreateMappedStream == NULL) {
                   ErrMsg(TEXT("Unable to find AVICreateMappedStream in palmap32.dll"));
                   gbCanPalMap = FALSE;
               }

               psCurrent = gapavi[giFirstVideo];

               AVIStreamInfo(psCurrent, &avis, sizeof(avis));

               if (avis.fccType != streamtypeVIDEO) {
                   ErrMsg("Stream is not video!");
                   break;
               }

               if (DialogBoxParam(
                             ghInstApp,
                             MAKEINTRESOURCE(IDD_NCOLORS),
                             hwnd,
                             GetNumberOfColorsDlgProc,
                             (LPARAM)GetNumberOfColors(psCurrent))
                   ) {
                      StartWait();
                      //
                      //  Find out how many colours they want to map it to
                      //

                      if (0 != (*lpfnAVICreateMappedStream)(&psMappedStream,
                                              psCurrent,
                                              gnColours)) {
                           ErrMsg(TEXT("Can't map stream palette"));
                      }
                      else {
                          gapavi[giFirstVideo] = psMappedStream;
                      }
                      EndWait();
                   }

            }
            break;

	
	//
	// Save all the open streams into a file
	//
        case MENU_SAVEAS:
            {
                OPENFILENAME ofn;

                gachSaveFileName[0] = 0;

    	        //
                // prompt user for file to save
    	        //
                ofn.lStructSize = sizeof(OPENFILENAME);
                ofn.hwndOwner = hwnd;
                ofn.hInstance = NULL;

                AVIBuildFilter(gachFilter, sizeof(gachFilter)/sizeof(TCHAR), TRUE);

                ofn.lpstrFilter = gachFilter;
                ofn.lpstrCustomFilter = NULL;
                ofn.nMaxCustFilter = 0;
                ofn.nFilterIndex = 0;
                ofn.lpstrFile = gachSaveFileName;
                ofn.nMaxFile = sizeof(gachSaveFileName)/sizeof(TCHAR);
                ofn.lpstrFileTitle = NULL;
                ofn.nMaxFileTitle = 0;
                ofn.lpstrInitialDir = NULL;
                ofn.lpstrTitle = TEXT("Save AVI File");
                ofn.Flags = OFN_PATHMUSTEXIST | OFN_HIDEREADONLY |
    			    OFN_OVERWRITEPROMPT;
                ofn.nFileOffset = 0;
                ofn.nFileExtension = 0;
                ofn.lpstrDefExt = TEXT("avi");
                ofn.lCustData = 0;
                ofn.lpfnHook = NULL;
                ofn.lpTemplateName = NULL;

 	        //
 	        // If we get a filename, save it
 	        //
                if (GetSaveFileName(&ofn)) {
                    DWORD   fccHandler[MAXNUMSTREAMS];
        	    int	    i;
        	    HRESULT hr;
        	
        	    StartWait();

        	    for (i = 0; i < gcpavi; i++)
        	        fccHandler[i] = galpAVIOptions[i]->fccHandler;

        	    hr = AVISaveV(gachSaveFileName,
        	                  NULL,
        	                  (AVISAVECALLBACK) SaveCallback,
        	                  gcpavi,
        	                  gapavi,
        	                  galpAVIOptions);
        	    if (hr != AVIERR_OK) {
                        switch (hr) {
                        case AVIERR_FILEOPEN:
                            ErrMsg(TEXT("Overwriting an open AVI file is not possible"));
                            break;
                        default:
                            ErrMsg(TEXT("Error saving AVI file"));
                        }
                    }

        	    // Now put the video compressors back that we stole
        	    for (i = 0; i < gcpavi; i++)
        	         galpAVIOptions[i]->fccHandler = fccHandler[i];
        	
        	    EndWait();
        	    FixWindowTitle(hwnd);
                }
 	    }
	    break;

	//
	// Close everything
	//
	case MENU_NEW:
	    FreeAvi(hwnd);
	    gachFileName[0] = TEXT('\0');
	    FixWindowTitle(hwnd);
	    break;
	
	//
	// Open a new file, or merge streams with a new file
	//
        case MENU_OPEN:
        case MENU_ADD:
            {
                OPENFILENAME ofn;

                gachFileName[0] = 0;

    	        //
                // prompt user for file to open
    	        //
                ofn.lStructSize = sizeof(OPENFILENAME);
                ofn.hwndOwner   = hwnd;
                ofn.hInstance   = NULL;
    	        if (wParam == MENU_ADD)
    		    ofn.lpstrTitle = TEXT("Merge With");
    	        else
    	            ofn.lpstrTitle = TEXT("Open AVI");

    	        if (gachFilter[0] == TEXT('\0'))
    		    AVIBuildFilter(gachFilter, sizeof(gachFilter)/sizeof(TCHAR), FALSE);
    	
    	        ofn.lpstrFilter       = gachFilter;
                ofn.lpstrCustomFilter = NULL;
                ofn.nMaxCustFilter    = 0;
                ofn.nFilterIndex      = 0;
                ofn.lpstrFile         = gachFileName;
                ofn.nMaxFile          = sizeof(gachFileName)/sizeof(TCHAR);
                ofn.lpstrFileTitle    = NULL;
                ofn.nMaxFileTitle     = 0;
                ofn.lpstrInitialDir   = NULL;
                ofn.Flags             = OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST | OFN_HIDEREADONLY;
                ofn.nFileOffset       = 0;
                ofn.nFileExtension    = 0;
                ofn.lpstrDefExt       = NULL;
                ofn.lCustData         = 0;
                ofn.lpfnHook          = NULL;
                ofn.lpTemplateName    = NULL;

    	        //
    	        // If we've got a filename, go open it
    	        //
                if (GetOpenFileNamePreview(&ofn))
    		    InitAvi(hwnd, gachFileName, wParam);
            }
            break;

	//
	// Open the "fake" ball file as our current file
	//
	case MENU_BALL:
	    InitBall(hwnd);
	    break;
	
	case MENU_ZOOMQUARTER:
	    gwZoom = 1;
	    FixScrollbars(hwnd);
            InvalidateRect(hwnd, NULL, TRUE);
	    break;
	
	case MENU_ZOOMHALF:
	    gwZoom = 2;
	    FixScrollbars(hwnd);
            InvalidateRect(hwnd, NULL, TRUE);
	    break;
	
	case MENU_ZOOM1:
	    gwZoom = 4;
	    FixScrollbars(hwnd);
            InvalidateRect(hwnd, NULL, TRUE);
	    break;
	
	case MENU_ZOOM2:
	    gwZoom = 8;
	    FixScrollbars(hwnd);
            InvalidateRect(hwnd, NULL, TRUE);
	    break;
	
	case MENU_ZOOM4:
	    gwZoom = 16;
	    FixScrollbars(hwnd);
            InvalidateRect(hwnd, NULL, TRUE);
            break;

	//
	// Simulate playing the file.  We just play the 1st audio stream and let
	// our main message loop scroll the video by whenever it's bored.
	//
	case MENU_PLAY:
	    if (gfAudioFound)
	        aviaudioPlay(hwnd,
			 gpaviAudio,
			 AVIStreamTimeToSample(gpaviAudio, GetScrollTime(hwnd)),
			 AVIStreamEnd(gpaviAudio),
			 FALSE);
            gfPlaying = TRUE;
            glPlayStartTime = timeGetTime();
            glPlayStartPos = GetScrollTime(hwnd);
            break;

	//
	// Stop the play preview
	//
	case MENU_STOP:
	    if (gfAudioFound)
	        aviaudioStop();
	    gfPlaying = FALSE;
	    break;

    }
    return 0L;
}

/*----------------------------------------------------------------------------*\
|   ErrMsg - Opens a Message box with a error message in it.  The user can     |
|            select the OK button to continue                                  |
\*----------------------------------------------------------------------------*/
int ErrMsg (LPTSTR sz,...)
{
    static TCHAR ach[2000];
    va_list va;

    va_start(va, sz);
    wvsprintf (ach,sz, va);
    va_end(va);
    MessageBox(NULL,ach,NULL,
#ifdef BIDI
		MB_RTL_READING |
#endif
    MB_OK|MB_ICONEXCLAMATION|MB_TASKMODAL);
    return FALSE;
}


// Find the number of colours present in the stream palette
LONG GetNumberOfColors(PAVISTREAM ps)
{
    BITMAPINFOHEADER bmih;
    LONG             cbFormat = sizeof(bmih);

    bmih.biClrUsed = 0;

    AVIStreamReadFormat(ps, 0, (LPVOID)&bmih, &cbFormat);

    return (LONG)bmih.biClrUsed;
}

/* GetNumberOfColorsDlgProc()
 *
 * Dialog Procedure for getting the no of colours the user wants in the palette
 *
 */

BOOL CALLBACK GetNumberOfColorsDlgProc(
	HWND	hwnd,
	UINT	msg,
	WPARAM	wParam,
	LPARAM	lParam)
{
        BOOL Translated;
	switch (msg) {
	case WM_COMMAND:
            switch (GET_WM_COMMAND_ID(wParam, lParam)) {
            case IDOK:
		gnColours = GetDlgItemInt(hwnd, IDC_NCOLORS, &Translated, FALSE);
                EndDialog(hwnd, TRUE);
                return TRUE;
            case IDCANCEL:
                EndDialog(hwnd, FALSE);
            }
            break;

	case WM_INITDIALOG:
            {
                SetDlgItemInt(hwnd, IDC_NCOLORS, lParam, FALSE);
		return TRUE;
            }
	}
	return FALSE;
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
                if (gbCanPalMap)                                         // Provide the user with
                    ShowWindow(GetDlgItem(hwnd, IDS_PALETTE), SW_HIDE);  // some information about
                else                                                     // whether palmap32.dll was
                    ShowWindow(GetDlgItem(hwnd, IDS_PALETTE), SW_SHOW);  // found.
                return TRUE;
	}
	return FALSE;
}
