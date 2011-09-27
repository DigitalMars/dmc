/****************************************************************************
 *
 *  AVIEDIT.C
 *
 *  Sample program using the AVIFile read/write routines
 *
 **************************************************************************/
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

#define STRICT
#define INC_OLE2
#include <windows.h>
#include <shellapi.h>
#include <windowsx.h>
#include <commdlg.h>
#include "muldiv32.h"
#include <vfw.h>

#include "aviedit.h"
#include "audio.h"

#include <limits.h>

#define GlobalSizePtr(lp)   GlobalSize(GlobalPtrHandle(lp))
#define LPPAVIFILE PAVIFILE *
typedef BYTE * HPBYTE;
typedef UNALIGNED short * HPSHORT;

/*----------------------------------------------------------------------------*\
\*----------------------------------------------------------------------------*/
static BOOL gfDefDlgEx = FALSE;         //the recursion flag for message crackers

#define BUFSIZE 260
static char gszBuffer[BUFSIZE];
static char gszFileName[BUFSIZE];
static char gszSaveFileName[BUFSIZE];

#define AVI_EDIT_CLASS  "edit"
static LPAVISTREAMINFO glpavisi;
static int gnSel;
int     gSelectedStream = -1;   // Highlight this text area when painting
RECT    grcSelectedStream;      // where to highlight

char    gszFilter[512];     // for AVIBuildFilter - more than one string!
static  HINSTANCE   ghInstApp;
static  HWND        ghwndApp;
static  HACCEL      ghAccel;
static  WNDPROC     gOldEditProc;
static  HWND        ghwndEdit;

#define SCROLLRANGE  10000
#define MAXNUMSTREAMS   25

int                 gcpavi;                     // # of streams
PAVISTREAM          gapavi[MAXNUMSTREAMS];      // the current streams
int                 gcpaviSel;                  // num of edit streams
PAVISTREAM          gapaviSel[MAXNUMSTREAMS];   // edit streams to put on clipbd
int                 gStreamTop[MAXNUMSTREAMS+1];// y position of each stream
AVICOMPRESSOPTIONS  gaAVIOptions[MAXNUMSTREAMS];// compression options
LPAVICOMPRESSOPTIONS  galpAVIOptions[MAXNUMSTREAMS];
PGETFRAME           gapgf[MAXNUMSTREAMS];       // data for decompressing video
HDRAWDIB            ghdd[MAXNUMSTREAMS];        // drawdib handles
LONG                galSelStart[MAXNUMSTREAMS];
LONG                galSelLen[MAXNUMSTREAMS];
int		    giFirstAudio = -1;          // 1st audio stream found
int		    giFirstVideo = -1;          // 1st video stream found

#define             gfVideoFound (giFirstVideo >= 0)
#define             gfAudioFound (giFirstAudio >= 0)

BOOL                gfPlaying;          // are we currently playing?
LONG                glPlayStartTime;    // When did we start playing?
LONG                glPlayStartPos;     // Where were we on the scrollbar?
LONG                timeStart;          // cached start, end, length
LONG                timeEnd;
LONG                timeLength;
LONG                timehscroll;        // how much arrows scroll HORZ bar
int                 nVertSBLen;         // vertical scroll bar
int                 nVertHeight;
DWORD               gdwMicroSecPerPixel = 1000L;        // scale for video
WORD                gwZoom = 2;         // one-half zoom (divide by 4)
HWND                ghwndMCI;

// buffer for wave data
LPVOID lpAudio;

// constants for painting
#define VSPACE  8       // some vertical spacing
#define SELECTVSPACE 4 // height of selection line
#define HSPACE  4       // space between frames for video stream
#define TSPACE  10      // space for text area about each stream
#define AUDIOVSPACE  64 // height of an audio stream at X1 zoom

#define HIGHLIGHT       (GetSysColor(COLOR_HIGHLIGHT) ? GetSysColor(COLOR_HIGHLIGHT) : GetSysColor(COLOR_ACTIVECAPTION))

/*----------------------------------------------------------------------------*\
\*----------------------------------------------------------------------------*/

// Macros to get and set the scroll bar to a given millisecond value in the
// movie.  Movie lengths can be DWORDS but we only have 16 bits of resolution.

#define GetScrollTime(hwnd) \
(timeStart + muldiv32(GetScrollPos(hwnd, SB_HORZ), timeLength, SCROLLRANGE))

#define SetScrollTime(hwnd, time) SetScrollPos(hwnd, SB_HORZ, \
(int)muldiv32((time) - timeStart, SCROLLRANGE, timeLength), TRUE)

/*----------------------------------------------------------------------------*\
\*----------------------------------------------------------------------------*/

LRESULT CALLBACK AppWndProc(HWND, UINT, WPARAM, LPARAM );
LRESULT CALLBACK NewEditProc(HWND, UINT, WPARAM, LPARAM );
BOOL CALLBACK    AboutDlgProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
int  ErrMsg (LPSTR sz,...);

BOOL MenuHandler( HWND, int );

void editPaste(HWND hwnd, PAVIFILE pfile);
void             FrameVideo(HDC hdc, RECT *rcFrame, HBRUSH hbr);

/*----------------------------------------------------------------------------*\
\*----------------------------------------------------------------------------*/
static int gfWait = 0;

/*----------------------------------------------------------------------------*\
|    StartWait()                                                                |
|    Start a wait operation... put up the hourglass if it's the first call      |
\*----------------------------------------------------------------------------*/
void StartWait()
{
    if (gfWait++ == 0) {
	SetCursor(LoadCursor(NULL,IDC_WAIT));
    }
}

/*----------------------------------------------------------------------------*\
|    EndWait()                                                                  |
|    Once every one who started a wait is finished, go back to regular cursor   |
\*----------------------------------------------------------------------------*/
void EndWait()
{
    if (--gfWait == 0) {
	SetCursor(LoadCursor(NULL,IDC_ARROW));
	InvalidateRect(ghwndApp, NULL, TRUE);
    }
}

/*----------------------------------------------------------------------------*\
|    WinYield()                                                                 |
|    Code to yield while we're not calling GetMessage.                          |
|    Dispatch all messages.  Pressing ESC or closing aborts.                    |
\*----------------------------------------------------------------------------*/

BOOL WinYield(void)
{
    MSG msg;
    BOOL fAbort=FALSE;

    while(gfWait > 0 && PeekMessage(&msg,NULL,0,0,PM_REMOVE))
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
|    PaintVideo()                                                               |
|    Draw a video frame in the specified rect                                   |
\*----------------------------------------------------------------------------*/

void PaintVideo(HDC hdc, RECT rcFrame, int iStream, LPBITMAPINFOHEADER lpbi, LONG lCurSamp, LONG lPos)
{
    int         iLen;
    char        szText[BUFSIZE];
    RECT        rc;

    //
    // If we have a picture, draw it
    //
    if (lpbi)
    {
	//
	// use the palette of the first video stream
	//
	DrawDibDraw(ghdd[iStream], hdc,
		    rcFrame.left, rcFrame.top,
		    rcFrame.right - rcFrame.left,
		    rcFrame.bottom - rcFrame.top,
		    lpbi, NULL,
		    0, 0, -1, -1,
		    (iStream == giFirstVideo) ? 0 :DDF_BACKGROUNDPAL);

	iLen = wsprintf(szText, "%ld %ld.%03lds",
			lCurSamp, lPos / 1000, lPos % 1000);
    }

    //
    // Before or after the movie (or read error) draw GRAY
    //
    else {
	SelectObject(hdc,GetStockObject(DKGRAY_BRUSH));

	PatBlt(hdc,
	       rcFrame.left, rcFrame.top,
	       rcFrame.right - rcFrame.left,
	       rcFrame.bottom - rcFrame.top,
	       PATCOPY);
	iLen = 0;
	szText[0] = '\0';
    }

    //
    // print something meaningful under the frame
    //
    rc.left = rcFrame.left;
    rc.right = rcFrame.right - 2 * HSPACE;      // don't overlap text areas
    rc.top = rcFrame.bottom + HSPACE;
    rc.bottom = rc.top + TSPACE + TSPACE;       // blank out enough space
    ExtTextOut(hdc, rc.left, rc.top, ETO_CLIPPED,
	       &rc, szText, iLen, NULL);
}


/*----------------------------------------------------------------------------*\
|    PaintAudio()                                                               |
|    Draw some samples of audio inside the given rectangle                      |
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
	// right justify the legal samples in the rectangle - don't stretch
	rc.left = rc.right - (int)muldiv32(rc.right - rc.left, lLen, lLenOrig);
    }

    //
    // We can't draw past the end of the stream
    //
    if (lStart + lLen > lWaveEndTime) {
	lLenOrig = lLen;
	lLen = max(0, lWaveEndTime - lStart);   // maybe nothing to draw!
	// left justify the legal samples in the rectangle - don't stretch
	rc.right = rc.left + (int)muldiv32(rc.right - rc.left, lLen, lLenOrig);
    }

    // Now start working with samples, not time
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

#define BACKBRUSH  (GetSysColor(COLOR_BTNFACE))         // background
#define MONOBRUSH  (GetSysColor(COLOR_BTNSHADOW))       // for mono audio
#define LEFTBRUSH  (RGB(0,0,255))                       // left channel
#define RIGHTBRUSH (RGB(0,255,0))                       // right channel
#define HPOSBRUSH  (RGB(255,0,0))                       // current position

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

#define MulDiv(a,b,c) (UINT)((DWORD)(UINT)(a) * (DWORD)(UINT)(b) / (UINT)(c))

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
		b = *((HPBYTE)lpAudio + muldiv32(x, lLen, w));

		if (b > 0x80) {
		    i = y - MulDiv(b - 0x80, (h / 2), 128);
		    PatBlt(hdc, rc.left+x, i, 1, y-i, PATCOPY);
		}
		else {
		    i = y + MulDiv(0x80 - b, (h / 2), 128);
		    PatBlt(hdc, rc.left + x, y, 1, i - y, PATCOPY);
		}
	    }
	}

	//
	// 16 bit data is centred around 0x00
	//
	else if (wf.wBitsPerSample == 16) {
	    for (x=0; x<w; x++) {

		// which byte of audio data belongs at this pixel?
		// Don't make any assumptions about INT size !
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
		    i = y - MulDiv(b-0x80,(h/4),128);
		    PatBlt(hdc, rc.left+x, i, 1, y-i, PATCOPY);
		}
		else {
		    i = y + MulDiv(0x80-b,(h/4),128);
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
		    i = y - MulDiv(b-0x80,(h/4),128);
		    PatBlt(hdc, rc.left+x, i, 1, y-i, PATCOPY);
		}
		else {
		    i = y + MulDiv(0x80-b,(h/4),128);
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
|    PaintStuff()                                                               |
|    Do our painting.  Return the height of everything painted so we know how   |
|    much room it took to set our scrollbars.  If fDrawEverything is TRUE,      |
|    we will not stop drawing just because we know we're drawing outside the    |
|    window. This is used to determine how much we would want to draw.          |
\*----------------------------------------------------------------------------*/

int PaintStuff(HDC hdc, HWND hwnd, BOOL fDrawEverything)
{
    int         yStreamTop;
    char        szText[BUFSIZE];
    int         iFrameWidth, iLen;
    LONG        lSamp, lCurSamp;
    int         n;
    int         nFrames;
    LPBITMAPINFOHEADER lpbi = NULL;
    LONG        l;
    LONG        lTime;
    LONG        lSize = 0;
    LONG        lAudioStart;
    LONG        lAudioLen;
    RECT        rcFrame, rcC;
    int         i;
    HBRUSH      hbr, hbrOld;
    RECT        rc;

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
	AVISTREAMINFO   avis;
	LONG            lEnd, lEndTime;
	int             nMode;

	//
	// Remember where this stream begins
	//
	gStreamTop[i] = yStreamTop + GetScrollPos(hwnd, SB_VERT);

	//
	// Get some info about this stream
	//
	AVIStreamInfo(gapavi[i], &avis, sizeof(avis));

	//
	// Highlight the stream name if we're supposed to
	//
	if (gSelectedStream == MAXNUMSTREAMS+i) {
	    hbr = CreateSolidBrush(HIGHLIGHT);
	    hbrOld = SelectObject(hdc, hbr);

	    PatBlt(hdc, 0, yStreamTop, rcC.right, TSPACE * 2, PATCOPY);
	    SelectObject(hdc, hbrOld);
	    DeleteObject(hbr);
	}

	// First we'll print out the stream name
	nMode = SetBkMode(hdc, TRANSPARENT);
	TextOut(hdc, HSPACE, yStreamTop, avis.szName, strlen(avis.szName));
	SetBkMode(hdc, nMode);

	// Skip to the next line
	yStreamTop += 2*TSPACE;

	if (galSelStart[i] == -1)
	{
	    LoadString( ghInstApp, IDS_FORMAT_1, gszBuffer, BUFSIZE );
	    iLen = wsprintf(szText,
			    gszBuffer,
			    i,
			    (LPSTR) &avis.fccType,      // compressor FOURCC
			    AVIStreamStart(gapavi[i]),
			    AVIStreamLength(gapavi[i]),
			    (AVIStreamEndTime(gapavi[i]) -
			     AVIStreamStartTime(gapavi[i])) / 1000);
	}
	else
	{
	    LoadString( ghInstApp, IDS_FORMAT_2, gszBuffer, BUFSIZE );
	    iLen = wsprintf(szText,
			    gszBuffer,
			    i,
			    (LPSTR) &avis.fccType,      // compressor FOURCC
			    AVIStreamStart(gapavi[i]),
			    AVIStreamLength(gapavi[i]),
			    (AVIStreamEndTime(gapavi[i]) -
			     AVIStreamStartTime(gapavi[i])) / 1000,
			    galSelStart[i], galSelStart[i] + galSelLen[i] - 1);
	}
	//
	// Highlight the stream info if we're supposed to
	//
	if (gSelectedStream == i) {
	    hbr = CreateSolidBrush(HIGHLIGHT);
	    hbrOld = SelectObject(hdc, hbr);

	    PatBlt(hdc, 0, yStreamTop, rcC.right, TSPACE * 2, PATCOPY);
	    SelectObject(hdc, hbrOld);
	    DeleteObject(hbr);
	}

	nMode = SetBkMode(hdc, TRANSPARENT);
	TextOut(hdc, HSPACE, yStreamTop, szText, iLen);
	SetBkMode(hdc, nMode);

	yStreamTop += TSPACE;

	//
	// Draw a VIDEO stream
	//
	if (avis.fccType == streamtypeVIDEO) {
	    if (gapgf[i] == NULL)
		continue;

	    //
	    // Which frame belongs at this time?
	    //
	    lEndTime = AVIStreamEndTime(gapavi[i]);
	    if (lTime <= lEndTime)
		lSamp = AVIStreamTimeToSample(gapavi[i], lTime);
	    else {      // we've scrolled past the end of this stream
		lEnd = AVIStreamEnd(gapavi[i]);
		lSamp = lEnd + AVIStreamTimeToSample(gapavi[i],
						     lTime - lEndTime);
	    }

	    //
	    // how wide is each frame to paint?
	    //
	    iFrameWidth = (avis.rcFrame.right - avis.rcFrame.left) *
			  gwZoom / 4 + HSPACE;

	    //
	    // how many frames can we fit on each half of the screen
	    // not counting the one we'll centre?
	    //
	    nFrames = (rcC.right - iFrameWidth) / (2 * iFrameWidth);
	    if (nFrames < 0)
		nFrames = 0;

	    //
	    // Step through all the frames we'll draw
	    //
	    for (n = -nFrames; n <= nFrames; n++) {

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
		if (i == giFirstVideo) {

		    //
		    // by definition, we know what frame we're drawing..
		    // (lSamp-n), (lSamp-(n-1)), ..., (lSamp), ...,
		    // (lSamp+n) (lSamp is the one in the centre)
		    //
		    lCurSamp = lSamp + n;

		    //
		    // what time is it at that frame? This number will
		    // be printed underneath the frame
		    //
		    l = AVIStreamSampleToTime(gapavi[i], lCurSamp);

		} else {        // NOT the first video stream

		    //
		    // What time is it at the left hand of the square
		    // we'll draw?  That's what frame we use.
                    // Does the rounding with MulDiv32 have better properties
                    // than muldiv32? It appears to give _slightly_ better performance

                    l = lTime + MulDiv32(n * (iFrameWidth+HSPACE),
					 gdwMicroSecPerPixel, 1000);
                    //if (n<=0) {  // calculate the time for a frame left of centre,
                    //             // don't forget the HSPACE offset.
                    //    l = lTime - muldiv32(-n * (iFrameWidth + HSPACE),
                    //                         gdwMicroSecPerPixel, 1000);
                    //
                    //}
                    //else { // frame is to right of centre.
                    //    l = lTime + muldiv32(n * (iFrameWidth +HSPACE),
                    //                           gdwMicroSecPerPixel, 1000);
                    //
                    //}

		    //
		    // What frame belongs to that time?
		    //
		    lCurSamp = AVIStreamTimeToSample(gapavi[i], l);

		    //
		    // Use the exact time of that frame when printing
		    //
		    l = AVIStreamSampleToTime(gapavi[i], lCurSamp);
		}

		// !!!
		// Could actually return an LPBI for invalid frames
		// so we better force it to NULL.
		//
		if (gapgf[i] && lCurSamp >= AVIStreamStart(gapavi[i])) // &&
		    //lCurSamp <= AVIStreamEnd(gapavi[i]))
		    lpbi = AVIStreamGetFrame(gapgf[i], lCurSamp);
		else
		    lpbi = NULL;

		//
		// Figure out where to draw this frame
		//
		rcFrame.left   = rcC.right / 2 -
				 ((avis.rcFrame.right - avis.rcFrame.left) * gwZoom / 4)
				 / 2 + (n * iFrameWidth);
		rcFrame.top    = yStreamTop + TSPACE;
		rcFrame.right  = rcFrame.left +
				 (avis.rcFrame.right - avis.rcFrame.left) * gwZoom / 4;
		rcFrame.bottom = rcFrame.top +
				 (avis.rcFrame.bottom - avis.rcFrame.top) * gwZoom / 4;

		//
		// If this frame is selected, highlight it
		//
		if (lCurSamp >= galSelStart[i] &&
		    lCurSamp < galSelStart[i] +
		    galSelLen[i]) {
		    hbr = CreateSolidBrush(HIGHLIGHT);
                }
                else { //not highlighted - but need to clear area around frame
                       // of selection
                    hbr = CreateSolidBrush(GetBkColor(hdc));
		}

		FrameVideo(hdc, &rcFrame, hbr);
		DeleteObject (hbr);

		//
		// draw a border around the centre frame.
		//
		if (n == 0) {
		    hbr = CreateSolidBrush(RGB(255,0,0));
		    InflateRect(&rcFrame, 1, 1);
		    FrameRect(hdc, &rcFrame, hbr);
		    InflateRect(&rcFrame, -1, -1);
		    DeleteObject (hbr);
		}

		//
		// Now draw the video frame in the computed rectangle
		//
		PaintVideo(hdc, rcFrame, i, lpbi, lCurSamp, l);
	    }

	    //
	    // Print a description of this stream
	    //
	    if (lpbi)
		AVIStreamSampleSize(gapavi[i], lSamp, &lSize);

	    //
	    // Move down to where we can draw the next stream
	    //
	    yStreamTop += TSPACE +
			  (rcFrame.bottom - rcFrame.top) +
			  TSPACE;
	}

	//
	// Draw an AUDIO stream
	//
	else if (avis.fccType == streamtypeAUDIO) {

	    //
	    // Figure out which samples are visible
	    //
	    lAudioStart = lTime - muldiv32(rcC.right / 2,
					   gdwMicroSecPerPixel, 1000);
	    lAudioLen = 2 * (lTime - lAudioStart);

            // clear the selection area

            hbr = CreateSolidBrush(GetBkColor(hdc));
            hbrOld = SelectObject(hdc, hbr);
            PatBlt(hdc,
                0,
                yStreamTop + TSPACE - SELECTVSPACE,
                rcC.right,
                SELECTVSPACE,
                PATCOPY);
            PatBlt(hdc,
                0,
                yStreamTop + TSPACE + (AUDIOVSPACE * gwZoom/4),
                rcC.right,
                SELECTVSPACE,
                PATCOPY);
            SelectObject(hdc, hbrOld);
            DeleteObject (hbr);

	    //
	    // We have a selection... Highlight it
	    //
	    if (galSelStart[i] != -1) {
                LONG lSelSt, lSelLen;

		//
		// What time is our selection?
		//
                lSelSt = AVIStreamSampleToTime(gapavi[i], galSelStart[i]);
                lSelLen = AVIStreamSampleToTime(gapavi[i], galSelLen[i]);

		//
		// At what pixels is our selection?
		//
                if (lSelSt < lTime) { //selecting to the left of the current position
		    rc.left = rcC.right /2 - (int)
                        muldiv32((lTime - lSelSt) , 1000, gdwMicroSecPerPixel);
		}
		else {
		    rc.left = rcC.right / 2 + (int)
                        muldiv32(lSelSt - lTime, 1000, gdwMicroSecPerPixel);
		}
		rc.right = rc.left + (int)
                        muldiv32(lSelLen, 1000, gdwMicroSecPerPixel);

                // Selection starts way past left side of screen
                if (lSelSt < lAudioStart)
		    rc.left = 0;
                // Selection is off, screen left
                if (lSelSt + lSelLen < lAudioStart)
                    rc.right = - SELECTVSPACE;

                // Selection is off, screen right
                if (lSelSt > lAudioStart + lAudioLen)
                    rc.left = rcC.right + SELECTVSPACE;

                // Selection ends past the right side of the screen
                if (lSelSt + lSelLen > lAudioStart + lAudioLen)
		    rc.right = rcC.right;
		if (rc.right == rc.left)        // draw SOMEthing.
		    rc.right++;
		rc.top = yStreamTop + TSPACE;
		rc.bottom = rc.top + AUDIOVSPACE * gwZoom / 4;

		//
		// Draw some indication
		//
		hbr = CreateSolidBrush(HIGHLIGHT);
		hbrOld = SelectObject(hdc, hbr);
		PatBlt(hdc,
		       rc.left - SELECTVSPACE,
		       rc.top - SELECTVSPACE,
		       rc.right - rc.left + 2 * SELECTVSPACE,
		       SELECTVSPACE,
		       PATCOPY);
		PatBlt(hdc,
		       rc.left - SELECTVSPACE,
		       rc.bottom,
		       rc.right - rc.left + 2 * SELECTVSPACE,
		       SELECTVSPACE,
		       PATCOPY);
		SelectObject(hdc, hbrOld);
		DeleteObject (hbr);
	    }

	    //
	    // Make the rectangle to draw audio into
	    //
	    rc.left = rcC.left;
	    rc.right = rcC.right;
	    rc.top = yStreamTop + TSPACE;
	    rc.bottom = rc.top + AUDIOVSPACE * gwZoom / 4;

	    //
	    // Actually paint the audio
	    //
	    PaintAudio(hdc, &rc, gapavi[i], lAudioStart, lAudioLen);

	    //
	    // Print the time at the centre of the audio stream
	    //
	    iLen = wsprintf(szText, "%ld.%03lds", lTime/1000, lTime%1000);
	    rc.left = (rc.right - rc.left) / 2 - 5 * HSPACE;
	    rc.right = (rc.right - rc.left) / 2;
	    rc.top = rc.bottom + HSPACE; rc.bottom = rc.top + VSPACE;
	    ExtTextOut(hdc, rc.left, rc.top, 0, &rc, szText, iLen, NULL);

	    //
	    // Move down to where we can draw the next stream
	    //
	    yStreamTop += TSPACE + AUDIOVSPACE * gwZoom / 4;

	}

	yStreamTop += TSPACE + TSPACE;

	//
	// Give up once we're painting below the bottom of the window
	//
	if (!fDrawEverything && yStreamTop >= rcC.bottom)
	    break;
    }

    // The bottom of all the streams;
    gStreamTop[gcpavi] = yStreamTop + GetScrollPos(hwnd, SB_VERT);

    //
    // How many lines did we draw?
    //
    return yStreamTop + GetScrollPos(hwnd, SB_VERT);
}

/*----------------------------------------------------------------------------*\
|    FixScrollBars()                                                            |
|    When we load a file or zoom changes, we re-set the scrollbars              |
\*----------------------------------------------------------------------------*/

void FixScrollbars(HWND hwnd)
{
    int                 nHeight = 0;
    RECT                rc;
    HDC                 hdc;

    //
    // Determine how tall our window needs to be to display everything.
    //
    hdc = GetDC(NULL);
    ExcludeClipRect(hdc, 0, 0, 32767, 32767);   // don't actually draw
    nHeight = PaintStuff(hdc, hwnd, TRUE);
    ReleaseDC(NULL, hdc);

    //
    // Set vertical scrollbar for scrolling the visible area
    //
    GetClientRect(hwnd, &rc);
    nVertHeight = nHeight;      // total height in pixels of entire display

    //
    // We won't fit in the window... need scrollbars
    //
    if (nHeight > rc.bottom)
    {
	nVertSBLen = nHeight - rc.bottom;
	SetScrollRange(hwnd, SB_VERT, 0, nVertSBLen, TRUE);
	SetScrollPos(hwnd, SB_VERT, 0, TRUE);

	//
	// We will fit in the window!  No scrollbars necessary
	//
    }
    else
    {
	nVertSBLen = 0;
	SetScrollRange(hwnd, SB_VERT, 0, 0, TRUE);
    }
    return;
}



/*----------------------------------------------------------------------------*\
|    InitStreams()                                                              |
|    Initialize the streams of a loaded file -- the compression options, the    |
|    DrawDIB handles, and the scroll bars.                                      |
|    !!! This clears the compression options right now every time it's called!  |
\*----------------------------------------------------------------------------*/

void InitStreams(HWND hwnd)
{
    AVISTREAMINFO     avis;
    LONG        lTemp;
    int         i;

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
		galpAVIOptions[i]->dwKeyFrameEvery = 7; // !!! ask compressor?
		galpAVIOptions[i]->dwBytesPerSecond = 60000;
		break;

	    case streamtypeAUDIO:
		galpAVIOptions[i]->dwFlags |= AVICOMPRESSF_VALID;
		galpAVIOptions[i]->dwInterleaveEvery = 5;
		AVIStreamReadFormat(gapavi[i],
				    AVIStreamStart(gapavi[i]),
				    NULL,
				    &lTemp);
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

	    if (!gfVideoFound) {
            DWORD        dw;

		//
		// Remember the first video stream --- treat it specially
		//
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

	    // These aren't used and better be NULL!
	    gapgf[i] = ghdd[i] = NULL;

	    //
	    // If there are no video streams, we base everything on this
	    // audio stream.
	    //
	    if (!gfAudioFound && !gfVideoFound) {

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
	    if (!gfAudioFound)
		giFirstAudio = i;

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
|    FixWindowTitle()                                                           |
|    Update the window title to reflect what's loaded                           |
\*----------------------------------------------------------------------------*/

void FixWindowTitle(HWND hwnd)
{
    char szTitle[2*BUFSIZE];

    LoadString( ghInstApp, IDS_APPNAME, gszBuffer, BUFSIZE );

    wsprintf(szTitle, "%s %s", (LPSTR)gszBuffer, (LPSTR)gszFileName);

    SetWindowText( hwnd, szTitle );

    InvalidateRect(hwnd, NULL, TRUE);
}

/*----------------------------------------------------------------------------*\
|    FreeDrawStuff()                                                            |
|    Free up the resources associated with DrawDIB.                             |
\*----------------------------------------------------------------------------*/

void FreeDrawStuff(HWND hwnd)
{
    int i;

    // Make sure we're not playing!
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
    giFirstVideo = giFirstAudio = -1;
}


/*----------------------------------------------------------------------------*\
|    NukeAVIStream()                                                            |
|    Get rid of a stream in our array and compact it.                           |
\*----------------------------------------------------------------------------*/

void NukeAVIStream(int i)
{
    int j;

    //
    // Make sure it's a real stream number
    //
    if (i < 0 || i >=gcpavi)
	return;

    //
    // Free all the resources associated with this stream
    //
    AVIStreamRelease(gapavi[i]);
    if (galpAVIOptions[i]->lpFormat) {
	GlobalFreePtr(galpAVIOptions[i]->lpFormat);
    }
    if (gapgf[i]) {
	AVIStreamGetFrameClose(gapgf[i]);
	gapgf[i] = NULL;
    }
    if (ghdd[i]) {
	DrawDibClose(ghdd[i]);
	ghdd[i] = 0;
    }

    //
    // Compact the arrays of junk
    //
    for (j = i; j < gcpavi - 1; j++) {
	gapavi[j] = gapavi[j+1];
	galpAVIOptions[j] = galpAVIOptions[j+1];
	gapgf[j] = gapgf[j+1];
	ghdd[j] = ghdd[j+1];
    }

    gcpavi--;
}

/*----------------------------------------------------------------------------*\
|    FreeAVI()                                                                  |
|    Free the resources associated with an open file.                           |
\*----------------------------------------------------------------------------*/

void FreeAvi(HWND hwnd)
{
    int i;

    FreeDrawStuff(hwnd);

    AVISaveOptionsFree(gcpavi, galpAVIOptions);

    for (i = 0; i < gcpavi; i++) {
	AVIStreamRelease(gapavi[i]);
    }

    // Good a place as any to make sure audio data gets freed
    if (lpAudio)
	GlobalFreePtr(lpAudio);
    lpAudio = NULL;

    gcpavi = 0;
}

/*----------------------------------------------------------------------------*\
|    InsertAVIFile()                                                            |
|    Put a new AVI file into our internal structures.                           |
\*----------------------------------------------------------------------------*/

void InsertAVIFile(PAVIFILE pfile, HWND hwnd, LPSTR lpszFile)
{
    int         i;
    PAVISTREAM  pavi;

    for (i = gcpavi; i <= MAXNUMSTREAMS; i++) {
	if (AVIFileGetStream(pfile, &pavi, 0L, i - gcpavi) != AVIERR_OK)
	    break;
	if (i == MAXNUMSTREAMS)
	{
	    AVIStreamRelease(pavi);
	    LoadString( ghInstApp, IDS_MAXSTREAMS, gszBuffer, BUFSIZE );
	    ErrMsg(gszBuffer);
	    break;
	}
	if (CreateEditableStream(&gapavi[i], pavi) != AVIERR_OK) {
	    AVIStreamRelease(pavi);
	    break;
	}
	AVIStreamRelease(pavi);
	galSelStart[i] = galSelLen[i] = -1;
    }

    AVIFileRelease(pfile);

    if (gcpavi == i && i != MAXNUMSTREAMS)
    {

	LoadString( ghInstApp, IDS_NOOPEN, gszBuffer, BUFSIZE );

	ErrMsg(gszBuffer, lpszFile);
	return;
    }

    FreeDrawStuff(hwnd);
    gcpavi = i;
    InitStreams(hwnd);
    FixWindowTitle(hwnd);
}


/*----------------------------------------------------------------------------*\
|    InitAVI()                                                                  |
|    Open up a file through the AVIFile handlers.                               |
\*----------------------------------------------------------------------------*/


void InitAvi(HWND hwnd, LPSTR szFile, int nMenu)
{
    HRESULT     hr;
    PAVIFILE    pfile;

    hr = AVIFileOpen(&pfile, szFile, 0, 0L);

    if (hr != 0)
    {
	LoadString( ghInstApp, IDS_NOOPEN, gszBuffer, BUFSIZE );
	ErrMsg(gszBuffer, szFile);
	return;
    }

    //
    // If we're opening something new, close other open files, otherwise
    // just close the draw stuff so we'll merge streams with the new file
    //
    if (nMenu == MENU_OPEN)
	FreeAvi(hwnd);

    InsertAVIFile(pfile, hwnd, szFile);
}

/*----------------------------------------------------------------------------*\
|    DropAvi()                                                                  |
|    Allow a drag/drop on AVIEdit.                                              |
\*----------------------------------------------------------------------------*/
void DropAvi(HWND hwnd, HDROP hDrop)
{
    char        szPath[BUFSIZE];
    UINT        nDropped, n;
    PAVIFILE    pfile;
    HRESULT     hr;

    // Get number of files dropped
    nDropped = DragQueryFile(hDrop,0xFFFF,NULL,0);

    if (nDropped) {
	SetActiveWindow(hwnd);

	// If we wanted to, we could simulate a click at the position
	// the drop took place....

	for (n = 0; n < nDropped; n++) {
	    // Get the file that was dropped....
	    DragQueryFile(hDrop, n, szPath, BUFSIZE);

	    hr = AVIFileOpen(&pfile, szPath, 0, 0L);

	    if (hr == 0) {
		// ... and paste it in.
		editPaste(hwnd, pfile);
	    }
	}
    }
    DragFinish(hDrop);     /* Delete structure alocated */
}

/*----------------------------------------------------------------------------*\
|   AppInit( hInst, hPrev)                                                     |
|                                                                              |
|   Description:                                                               |
|       This is called when the application is first loaded into               |
|       memory.  It performs all initialization that doesn't need to be done   |
|       once per instance.                                                     |
|                                                                              |
|   Arguments:                                                                 |
|       hInstance       instance handle of current instance                    |
|       hPrev           instance handle of previous instance                   |
|                                                                              |
|   Returns:                                                                   |
|       TRUE if successful, FALSE if not                                       |
|                                                                              |
\*----------------------------------------------------------------------------*/

BOOL AppInit(HINSTANCE hInst, HINSTANCE hPrev, int sw, LPSTR szCmdLine)
{
    WNDCLASS cls;
    WORD        wVer;

    /* first let's make sure we are running on 1.1 */
    wVer = HIWORD(VideoForWindowsVersion());
    if (wVer < 0x010a)
    {
	char szTitle[BUFSIZE];
	/* oops, we are too old, blow out of here */
	MessageBeep(MB_ICONHAND);
	LoadString( ghInstApp, IDS_APPERR, szTitle, BUFSIZE );
	LoadString( ghInstApp, IDS_OLDVFW, gszBuffer, BUFSIZE );

	MessageBox(NULL, gszBuffer, szTitle, MB_OK|MB_ICONSTOP);
	return FALSE;
    }

    //
    // Save instance handle for DialogBoxs
    //
    ghInstApp = hInst;

    ghAccel = LoadAccelerators(hInst, MAKEINTATOM(ID_APP));

    //
    // Did we get passed a filename on the command line? We'll open it at create
    // time.
    //
    if (szCmdLine && szCmdLine[0])
	lstrcpy(gszFileName, szCmdLine);
    else
	lstrcpy( gszFileName, "" );

    lstrcpy( gszSaveFileName, "" );

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
	cls.style          = CS_BYTEALIGNCLIENT | CS_VREDRAW | CS_HREDRAW |
			     CS_DBLCLKS;
	cls.lpfnWndProc    = (WNDPROC)AppWndProc;
	cls.cbWndExtra     = 0;
	cls.cbClsExtra     = 0;

	if (!RegisterClass(&cls))
	    return FALSE;
    }

    //
    // Must be called before using any of the AVIFile routines
    //
    AVIFileInit();

    LoadString( ghInstApp, IDS_APPNAME, gszBuffer, BUFSIZE );
    //
    // Create our main application window
    //
    ghwndApp = CreateWindow (
			     MAKEINTATOM(ID_APP),    // Class name
			     gszBuffer,             // Caption
			     WS_OVERLAPPEDWINDOW,    // Style bits
			     CW_USEDEFAULT, 0,       // Position
			     320,300,                // Size
			     (HWND)NULL,             // Parent window (no parent)
			     (HMENU)NULL,            // use class menu
			     hInst,                  // handle to window instance
			     (LPSTR)NULL             // no params to pass on
			     );
    ShowWindow(ghwndApp,sw);

    return TRUE;
}

/*----------------------------------------------------------------------------*\
|   WinMain( hInst, hPrev, lpszCmdLine, cmdShow )                              |
|                                                                              |
|   Description:                                                               |
|       The main procedure for the App.  After initializing, it just goes      |
|       into a message-processing loop until it gets a WM_QUIT message         |
|       (meaning the app was closed). If the preview is playing it adjusts     |
|       the scrollbar appropriately.                                           |
|                                                                              |
|   Arguments:                                                                 |
|       hInst           instance handle of this instance of the app            |
|       hPrev           instance handle of previous instance, NULL if first    |
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

    //
    // Call our initialization procedure
    //
    if (!AppInit(hInst, hPrev, sw, szCmdLine))
	return FALSE;

    /*
     * Polling messages from event queue
     */
    for (;;)
    {
	while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
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
	    l = aviaudioTime();         // returns -1 if no audio playing

	    //
	    // If we can't use the audio clock to tell us how long we've been
	    // playing, calculate it ourself
	    //
	    if (l == -1)
		l = timeGetTime() - glPlayStartTime + glPlayStartPos;

	    if (l != (LONG)GetScrollTime(ghwndApp)) {
		if (l < timeStart)      // make sure number isn't out of bounds
		    l = timeStart;
		if (l > timeEnd)        // looks like we're all done!
                    FORWARD_WM_COMMAND(ghwndApp, MENU_STOP, NULL, 0, SendMessage);

		SetScrollTime(ghwndApp, l);
		InvalidateRect(ghwndApp, NULL, FALSE);
		UpdateWindow(ghwndApp);

		continue;
	    }
	}

	WaitMessage();
    }

    /* NOT REACHED */
}

/*----------------------------------------------------------------------------*\
|    SelectStream()                                                             |
|                                                                               |
|    Selects a portion of a stream i:                                           |
|                                                                               |
|    i == -1 means clear all selections                                         |
|    start == -1 means clear that individual stream's selection                 |
|    fAdd == TRUE means extend that stream's selection to include the new       |
|            range (will otherwise just replace the selection)                  |
|    fAll == TRUE means select this range in every stream, not just i           |
\*----------------------------------------------------------------------------*/

void SelectStream(HWND hwnd, int i, LONG start, LONG length, BOOL fAdd, BOOL fAll)
{
    int n, j;
    LONG mystart, mylength;
    RECT rc;

    //
    // Clear all selections
    //
    if (i == -1) {
	for (n = 0; n < gcpavi; n++)
	    galSelStart[n] = galSelLen[n] = -1;

    } else if (i >= 0 && i < gcpavi) {  // valid stream number

	//
	// We've been told to clear this selection
	//
	if (start == -1 || length == -1)
	    galSelStart[i] = galSelLen[i] = -1;

	//
	// Is this a valid selection range?
	//
	if (start >=AVIStreamStart(gapavi[i]) &&
	    start < AVIStreamEnd(gapavi[i]) &&
	    length >= 1) {

	    //
	    // Do we select the same range in every stream or just one?
	    //
	    for (j = (fAll ? 0 : i); j < (fAll ? gcpavi : i+1); j++) {

		//
		// Translate for each stream the equivalent region to select
		//
		if (j == i) {
		    mystart = start; mylength = length;
		} else {
		    mystart = AVIStreamSampleToSample(gapavi[j],
						      gapavi[i], start);
		    mylength = max(1, AVIStreamSampleToSample(gapavi[j],
							      gapavi[i], length));    // at least 1
		    // !!! Better invalidate this entire stream since we're not
		    // sure what part needs repainting.
		    GetClientRect(hwnd, &rc);
		    rc.top = gStreamTop[j] - GetScrollPos(hwnd, SB_VERT);
		    rc.bottom = gStreamTop[j+1] - GetScrollPos(hwnd, SB_VERT);
		    InvalidateRect(hwnd, &rc, FALSE);
		}

		//
		// Verify we got good selection values
		//
		if (mystart < AVIStreamStart(gapavi[j])) {
		    mylength -= AVIStreamStart(gapavi[j]) - mystart;
		    mystart = AVIStreamStart(gapavi[j]);
		}
		if (mystart + mylength > AVIStreamEnd(gapavi[j]))
		    mylength -= mystart + mylength - AVIStreamEnd(gapavi[j]);
		if (mylength <= 0)
		    mystart = -1;
		if (mystart == -1)      // nothing to select in this stream
		    continue;

		//
		// Reset selection to new values
		//
		if (!fAdd || galSelStart[j] == -1) {
		    galSelStart[j] = mystart;
		    galSelLen[j] = mylength;

		    //
		    // extend selection to include this new range
		    //
		} else {
		    if (mystart < galSelStart[j]) {
			galSelLen[j] += galSelStart[j] - mystart;
			galSelStart[j] = mystart;
		    }
		    if (mystart + mylength > galSelStart[j] + galSelLen[j])
			galSelLen[j] = mystart + mylength - galSelStart[j];
		}
	    }
	}
    }
}

/*----------------------------------------------------------------------------*\
|    EditStreamName()                                                           |
|                                                                               |
|    We need to edit the name of a stream. Create the edit box.                 |
\*----------------------------------------------------------------------------*/

void EditStreamName(HWND hwndParent)

{
    AVISTREAMINFO avis;

    // Get the stream info so we can get the name
    AVIStreamInfo(gapavi[gSelectedStream-MAXNUMSTREAMS], &avis, sizeof(avis));

    // Create the window;
    InflateRect(&grcSelectedStream, 0, 2);
    ghwndEdit = CreateWindow(AVI_EDIT_CLASS, NULL,
			     WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL,
			     grcSelectedStream.left,
			     grcSelectedStream.top,
			     grcSelectedStream.right - grcSelectedStream.left,
			     grcSelectedStream.bottom - grcSelectedStream.top,
			     hwndParent, (HMENU)1, ghInstApp, NULL);

    // Subclass the window so we can trap <cr> hits.
    gOldEditProc = (WNDPROC)GetWindowLong(ghwndEdit, GWL_WNDPROC);
    SetWindowLong(ghwndEdit, GWL_WNDPROC, (long)NewEditProc);

    // Set the initial text of the edit window, give focus to the
    // window and select the text.
    SetWindowText(ghwndEdit, avis.szName);
    SetFocus(ghwndEdit);
    Edit_SetSel(ghwndEdit, 0, lstrlen(avis.szName));
}

/*----------------------------------------------------------------------------*\
|    EditDone()                                                                 |
|                                                                               |
|    Done with an edit. See if we take the changes.                             |
\*----------------------------------------------------------------------------*/

void EditDone(HWND hwndParent, BOOL bAcceptChange)
{
    // Update the stream name if we're supposed to.
    if (bAcceptChange) {
	char szBuff[BUFSIZE];
	int n;

	// Get the edited name and put into the stream header
	n = GetWindowText(ghwndEdit, szBuff, BUFSIZE);
	szBuff[n] = '\0';
	EditStreamSetName(gapavi[gSelectedStream-MAXNUMSTREAMS], szBuff);
    }

    // Turn the selection off.
    gSelectedStream = -1;

    // Nuke the edit window.
    SetWindowLong(ghwndEdit, GWL_WNDPROC, (long)gOldEditProc);
    DestroyWindow(ghwndEdit);
    ghwndEdit = NULL;

    // Paint where window used to be.
    InvalidateRect(hwndParent, &grcSelectedStream, TRUE);
    UpdateWindow(hwndParent);

    // Give the parent the focus back.
    SetFocus(hwndParent);
}

/*----------------------------------------------------------------------------*\
|    NewEditProc()                                                              |
|                                                                               |
|    Our own home-rolled window proc for the edit window giving                 |
|    notification when <cr> is hit.                                             |
|                                                                               |
|    We trap WM_CHAR, because WM_CHAR(wParam='\r') causes a beep, which         |
|    we don't want.                                                             |
\*----------------------------------------------------------------------------*/
#define _ANSI_R    (TCHAR)'\r'
LRESULT CALLBACK NewEditProc( HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
    switch (msg) {
	case WM_CHAR:
	    // Trap a keydown for <cr>
	    if (wParam == _ANSI_R) {
		// Send message to parent giving ID 2 (which doesn't
		// exist)
                        FORWARD_WM_COMMAND(GetParent(hwnd), 2, NULL, 0, PostMessage);
		return 0L;
	    }

	    // Need normal handling of characters otherwise
	    goto callDWP;

	case WM_KEYUP:
	    // Trap a keydown for <Esc>. This is how we get out without
	    // making a modification.
	    if (wParam == VK_ESCAPE) {
		// Send message to parent giving ID 3 (which doesn't
		// exist)
                        FORWARD_WM_COMMAND(GetParent(hwnd), 3, NULL, 0, PostMessage);
		return 0L;
	    }

	    // We want to fall through so what should happen on keydown
	    // does.

	default:
callDWP:
	    // Just call the old window proc
	    return CallWindowProc(gOldEditProc, hwnd, msg, wParam, lParam);
    }
}

// ******************************************************************************
//
//      Message Handler for WM_CREATE
//
// ******************************************************************************

BOOL App_OnCreate(HWND hwnd, LPCREATESTRUCT lpCreateStruct)
{
    DragAcceptFiles(hwnd, TRUE);

    if (gszFileName[0])
	InitAvi(hwnd, gszFileName, MENU_OPEN);

    return TRUE;
}

// ******************************************************************************
//
//      Message Handler for WM_COMMAND
//
// ******************************************************************************

void App_OnCommand(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify)
{
    if (id == 2)
	EditDone(hwnd, TRUE);
	// See if we are terminating editting.
    else if (id == 3)
	EditDone(hwnd, FALSE);
    else
	MenuHandler( hwnd, id );
}

// ******************************************************************************
//
//      Message Handler for WM_DROPFILES
//
// ******************************************************************************
//
void App_OnDropFiles(HWND hwnd, HDROP hdrop)
{
    DropAvi(hwnd, hdrop);

    return;
}

// ******************************************************************************
//
//      Message Handler for WM_INITMENU
//
// ******************************************************************************
void App_OnInitMenu(HWND hwnd, HMENU hMenu)
{
    int i;
    BOOL f;
    PAVIFILE pf;

    f = gcpavi > 0;
    EnableMenuItem(hMenu, MENU_SAVEAS, f ? MF_ENABLED : MF_GRAYED);
    EnableMenuItem(hMenu, MENU_OPTIONS,f ? MF_ENABLED : MF_GRAYED);

    EnableMenuItem(hMenu, MENU_CLOSE,  f ? MF_ENABLED : MF_GRAYED);
    EnableMenuItem(hMenu, MENU_MERGE,  f ? MF_ENABLED : MF_GRAYED);

    EnableMenuItem(hMenu, MENU_SETINFO,  f ? MF_ENABLED : MF_GRAYED);

    // !!! Why not provide UNDO while I'm at it?
    // Enable CUT/COPY/DELETE if there's something selected in a stream
    f = FALSE;
    for (i=0; i<gcpavi; i++)
	if (galSelStart[i] != -1)
	    f = TRUE;

    EnableMenuItem(hMenu, MENU_COPY,   f ? MF_ENABLED : MF_GRAYED);
    EnableMenuItem(hMenu, MENU_CUT,    f ? MF_ENABLED : MF_GRAYED);
    EnableMenuItem(hMenu, MENU_DELETE, f ? MF_ENABLED : MF_GRAYED);

    // If we haven't an edit window, we need to setup the "Name'
    //
    if (ghwndEdit == NULL)
    {
	LoadString( ghInstApp, IDS_NAME, gszBuffer, BUFSIZE );
	ModifyMenu(hMenu, MENU_NAME, MF_BYCOMMAND | MF_STRING, MENU_NAME, gszBuffer);
	EnableMenuItem(hMenu, MENU_NAME,
		       (gSelectedStream >= MAXNUMSTREAMS) ? MF_ENABLED : MF_GRAYED);
    }
    else
    {
	LoadString( ghInstApp, IDS_ABORTNAME, gszBuffer, BUFSIZE );
	ModifyMenu(hMenu, MENU_NAME, MF_BYCOMMAND | MF_STRING,MENU_NAME, gszBuffer);
	EnableMenuItem(hMenu, MENU_NAME,MF_ENABLED);
    }

    // See if there's anything to paste....
    f = FALSE;
    AVIGetFromClipboard(&pf);

    if (pf)
    {
	f = TRUE;
	AVIFileRelease(pf);
    }

    EnableMenuItem(hMenu, MENU_PASTE,  f ? MF_ENABLED : MF_GRAYED);

    f = gfAudioFound | gfVideoFound;
    EnableMenuItem(hMenu, MENU_PREVIEW, (f & !gfPlaying) ? MF_ENABLED : MF_GRAYED);
    EnableMenuItem(hMenu, MENU_STOP,    (f & gfPlaying)  ? MF_ENABLED : MF_GRAYED);

    CheckMenuItem(hMenu, MENU_ZOOMQUARTER, (gwZoom == 1)  ? MF_CHECKED : MF_UNCHECKED);
    CheckMenuItem(hMenu, MENU_ZOOMHALF,    (gwZoom == 2)  ? MF_CHECKED : MF_UNCHECKED);
    CheckMenuItem(hMenu, MENU_ZOOM1,       (gwZoom == 4)  ? MF_CHECKED : MF_UNCHECKED);
    CheckMenuItem(hMenu, MENU_ZOOM2,       (gwZoom == 8)  ? MF_CHECKED : MF_UNCHECKED);
    CheckMenuItem(hMenu, MENU_ZOOM4,       (gwZoom == 16) ? MF_CHECKED : MF_UNCHECKED);


    return;
}
// ******************************************************************************
//
//      Message Handler for WM_SIZE
//
// ******************************************************************************

void App_OnSize(HWND hwnd, UINT state, int cx, int cy)
{
    RECT rc;

    GetClientRect(hwnd, &rc);
    //
    // There is not enough vertical room to show all streams. Scrollbars
    // are required.
    //
    if (nVertHeight > rc.bottom)
    {
	nVertSBLen = nVertHeight - rc.bottom;
	SetScrollRange(hwnd, SB_VERT, 0, nVertSBLen, TRUE);
    }
    else// Everything fits vertically. No scrollbar necessary.
    {
	nVertSBLen = 0;
	SetScrollRange(hwnd, SB_VERT, 0, 0, TRUE);
    }
    return;
}
// ******************************************************************************
//
//      Message Handler for WM_DESTROY
//
// ******************************************************************************
//
void App_OnDestroy(HWND hwnd)
{
    AVIClearClipboard();
    FreeAvi(hwnd);      // close all open streams
    AVIFileExit();      // shuts down the AVIFile system
    PostQuitMessage(0);

    return;
}
// ******************************************************************************
//
//      Message Handler for WM_ENDSESSION
//
// ******************************************************************************
//
void App_OnEndSession(HWND hwnd, BOOL fEnding)
{
    if (fEnding)
    {
	if (GetClipboardOwner() == hwnd)
	{
	    if (OpenClipboard(hwnd))
	    {
		EmptyClipboard();
		CloseClipboard();
	    }
	}
	FreeAvi(hwnd);
    }
    return;
}
// ******************************************************************************
//
//      Message Handler for WM_PALETTECHANGED
//
// ******************************************************************************
//
void App_OnPaletteChanged(HWND hwnd, HWND hwndPaletteChange)
{
    // It came from us.  Ignore it
    if (hwndPaletteChange == hwnd)
	return;

    //if needed, insert any calls - such as RealizePallete - below

    return;
}
// ******************************************************************************
//
//      Message Handler for WM_KEYDOWN
//
// ******************************************************************************
//
void App_OnKey(HWND hwnd, UINT vk, BOOL fDown, int cRepeat, UINT flags)
{
    if(fDown)
    {
	switch (vk)
	{
	    case VK_UP:
		FORWARD_WM_VSCROLL(hwnd, NULL, SB_LINEUP, 0, PostMessage);
		break;
	    case VK_DOWN:
		FORWARD_WM_VSCROLL(hwnd, NULL, SB_LINEDOWN, 0, PostMessage);
		break;
	    case VK_PRIOR:
		FORWARD_WM_HSCROLL(hwnd, NULL, SB_PAGEUP, 0, PostMessage);
		break;
	    case VK_NEXT:
		FORWARD_WM_HSCROLL(hwnd, NULL, SB_PAGEDOWN, 0, PostMessage);
		break;
	    case VK_HOME:
		FORWARD_WM_HSCROLL(hwnd, NULL, SB_THUMBPOSITION, 0, PostMessage);
		break;
	    case VK_END:
		FORWARD_WM_HSCROLL(hwnd, NULL, SB_THUMBPOSITION, 0x7FFF, PostMessage);
		break;
	    case VK_LEFT:
		FORWARD_WM_HSCROLL(hwnd, NULL, SB_LINEUP, 0, PostMessage);
		break;
	    case VK_RIGHT:
		FORWARD_WM_HSCROLL(hwnd, NULL, SB_LINEDOWN, 0, PostMessage);
		break;
	}
    }
    return;
}
// ******************************************************************************
//
//      Message Handler for WM_HSCROLL
//
// ******************************************************************************
//
void App_OnHScroll(HWND hwnd, HWND hwndCtl, UINT code, int pos)
{
    LONG lScrollTime;

    lScrollTime = GetScrollTime(hwnd);

    switch (code)
    {
	case SB_LINEDOWN:
	    lScrollTime += timehscroll;
	    break;
	case SB_LINEUP:
	    lScrollTime -= timehscroll;
	    break;
	case SB_PAGEDOWN:
	    lScrollTime += timeLength/10;
	    break;
	case SB_PAGEUP:
	    lScrollTime -= timeLength/10;
	    break;
	case SB_THUMBTRACK:
	case SB_THUMBPOSITION:
	    lScrollTime = pos;
	    lScrollTime = timeStart + muldiv32(lScrollTime, timeLength, SCROLLRANGE);
	    break;
    }

    if (lScrollTime < timeStart)
	lScrollTime = timeStart;

    if (lScrollTime > timeEnd)
	lScrollTime = timeEnd;

    if (lScrollTime == (LONG)GetScrollTime(hwnd))
	return;

    SetScrollTime(hwnd, lScrollTime);
    InvalidateRect(hwnd, NULL, TRUE);
    UpdateWindow(hwnd);

    return;
}
// ******************************************************************************
//
//      Message Handler for WM_VSCROLL
//
// ******************************************************************************
//
void App_OnVScroll(HWND hwnd, HWND hwndCtl, UINT code, int pos)
{
    int nScrollPos;
    RECT rc;

    nScrollPos = GetScrollPos(hwnd, SB_VERT);

    GetClientRect(hwnd, &rc);

    switch (code)
    {
	case SB_LINEDOWN:
	    nScrollPos += 10;
	    break;
	case SB_LINEUP:
	    nScrollPos -= 10;
	    break;
	case SB_PAGEDOWN:
	    nScrollPos += rc.bottom;
	    break;
	case SB_PAGEUP:
	    nScrollPos -= rc.bottom;
	    break;
	case SB_THUMBTRACK:
	case SB_THUMBPOSITION:
	    nScrollPos = pos;
	    break;
    }

    if (nScrollPos < 0)
	nScrollPos = 0;

    if (nScrollPos > nVertSBLen)
	nScrollPos = nVertSBLen;

    if (nScrollPos == GetScrollPos(hwnd, SB_VERT))
	return;

    SetScrollPos(hwnd, SB_VERT, nScrollPos, TRUE);
    InvalidateRect(hwnd, NULL, TRUE);
    UpdateWindow(hwnd);

    return;
}
// ******************************************************************************
//
//      Message Handler for WM_RBUTTONDOWN
//
// ******************************************************************************
//
void App_OnRButtonDown(HWND hwnd, BOOL fDoubleClick, int x, int y, UINT keyFlags)
{
    RECT rc;
    int i;
    // Invalidate any stream that has something selected.  It needs
    // to redraw now.
    if (gSelectedStream >= MAXNUMSTREAMS) {
	if (ghwndEdit)
	    EditDone(hwnd, FALSE);
        else {
	    InvalidateRect(hwnd, &grcSelectedStream, TRUE); // needs to erase
	    gSelectedStream = -1;
	}
    }
    else {
	GetClientRect(hwnd, &rc);
        for (i = 0; i < gcpavi; i++) {
             if (galSelStart[i] != -1) {
		rc.top = gStreamTop[i] - GetScrollPos(hwnd, SB_VERT);
		rc.bottom =gStreamTop[i+1] -GetScrollPos(hwnd, SB_VERT);
		InvalidateRect(hwnd, &rc, TRUE); // needs to erase
	    }
	}
	
	// Deselect everything
	SelectStream(hwnd, -1, -1, -1, FALSE, FALSE);
    }
    return;
}
// ******************************************************************************
//
//      Message Handler for WM_LBUTTONDOWN
//
// ******************************************************************************
//
void App_OnLButtonDown(HWND hwnd, BOOL fDoubleClick, int x, int y, UINT keyFlags)
{
    int yTop, yBottom = 0, iFrameWidth, nFrames;
    int i,j,n;
    AVISTREAMINFO       avis;
    BOOL fShift = FALSE;
    BOOL fCtrl = FALSE;
    RECT rc, rcC;
    LONG        l, lTime, lSamp, lCurSamp;

    if( fDoubleClick ) {
	// See if we get to edit a stream name
	if (gSelectedStream >= MAXNUMSTREAMS)
	    EditStreamName(hwnd);
	
	return;
    }

    GetClientRect(hwnd, &rc);
    rcC = rc;
    yBottom = -GetScrollPos(hwnd, SB_VERT);     // offset for scrollbar

    // If we currently have a stream name selected, we must
    // deselect it.
    if (gSelectedStream >= MAXNUMSTREAMS) {
	// Erase the select marks
	if (ghwndEdit)
	    EditDone(hwnd, FALSE);
	else
	    InvalidateRect(hwnd, &grcSelectedStream, TRUE);
	
	// Stream no longer selected.
	gSelectedStream = -1;
    }
    else {
	//
	// Otherwise, if the shift key isn't down,
	// we deselect everything first
	//
	fShift = GetAsyncKeyState(VK_SHIFT) & 0x8000;
	fCtrl = GetAsyncKeyState(VK_CONTROL) & 0x8000;
        if (!fShift) {
	    // Invalidate any stream that has something selected.  It needs
	    // to redraw now.
            for (j = 0; j < gcpavi; j++) {
                if (galSelStart[j] != -1) {
		    rc.top = gStreamTop[j] - GetScrollPos(hwnd, SB_VERT);
		    rc.bottom =gStreamTop[j+1] -GetScrollPos(hwnd, SB_VERT);
		    InvalidateRect(hwnd, &rc, TRUE); // needs to erase
		}
	    }
	    // Deselect everything
	    SelectStream(hwnd, -1, -1, -1, FALSE, FALSE);
	}
    }


    //
    // Walk the streams and find out where we clicked
    //
    for (i=0; i<gcpavi; i++) {
	AVIStreamInfo(gapavi[i], &avis, sizeof(avis));
	
	// See if they clicked on the name
	yTop = gStreamTop[i] - GetScrollPos(hwnd, SB_VERT);
	yBottom = yTop + (2 * TSPACE);  // !!! size of stream header
	
        if (y >= yTop && y < yBottom) {
	
	    gSelectedStream = MAXNUMSTREAMS+i;  // which stream's header it is
	    rc.top = yTop;
	    rc.bottom = yBottom;
	    grcSelectedStream = rc;     // Invalidate this on button up
	
	    InvalidateRect(hwnd, &rc, FALSE);   // repaint whole strip
	
	    // No need to be here anymore
	    return;
	}
	
	
        //*******************************************************************
	// See if they clicked on the information Header
	yTop = yBottom;
	yBottom = yTop + (2 * TSPACE);  // !!! size of stream header
	
	//
	// If they've clicked on the header - select the whole stream
	//
        if (y >= yTop && y < yBottom) {
	
	    // Maybe select everything if Ctrl is held down
	    SelectStream(hwnd, i, AVIStreamStart(gapavi[i]),
			 AVIStreamEnd(gapavi[i]), FALSE, fCtrl);
	
	    // Tell paint code to highlight the text area.  Invalidate
	    // the whole stream area.  When they let go of the mouse,
	    // redraw the text area only.
	    gSelectedStream = i;        // which stream's header it is
	    rc.top = yTop;
	    rc.bottom = yBottom;
	    grcSelectedStream = rc;     // Invalidate this on button up
	    // Now get the area of the whole stream
	    rc.bottom = gStreamTop[i + 1] - GetScrollPos(hwnd, SB_VERT);
	    InvalidateRect(hwnd, &rc, FALSE);   // repaint whole strip
	
	    // Time to go
	    return;
	}
	
	// *******************************************************************
	
	//
	// Now get the area of the stream data, and...
	//
	yTop = yBottom;
	yBottom = gStreamTop[i + 1] - GetScrollPos(hwnd, SB_VERT);
	
	//
	// ... see if we clicked on a video frame, or...
	//
        if (avis.fccType == streamtypeVIDEO) {
	    if (gapgf[i] == NULL)
		continue;
	
	    //
	    // We're in the vertical range of the strip of video
	    //
            if (y >= yTop && y < yBottom) {
		
		rc.top = yTop; rc.bottom = yBottom;
		// Time at the centre of the strip
		lTime = GetScrollTime(hwnd);
		// What frame should appear in the centre? Times that
		// are too big will all return the last frame, so we
		// need to calculate the hypothetical frame number
                if (lTime <= AVIStreamEndTime(gapavi[i])) {
		    lSamp = AVIStreamTimeToSample(gapavi[i], lTime);
		}
                else {
		    lSamp = AVIStreamEnd(gapavi[i]) +
			    AVIStreamTimeToSample(gapavi[i],
						  lTime - AVIStreamEndTime(gapavi[i]));
		}
		// How wide is each frame?
		iFrameWidth = (avis.rcFrame.right - avis.rcFrame.left) *
			      gwZoom / 4 + HSPACE;    // !!! hacky constant
		// How many frames on each half of centre?
		nFrames = (rcC.right - iFrameWidth) / (2 * iFrameWidth);
		if (nFrames < 0)
		    nFrames = 0;        // at least show *something*
		
		//
		// Walk all frames and find which one we're on top of
		//
                for (n = -nFrames; n <= nFrames; n++) {
		
		    rc.left   = rcC.right / 2 -
				(avis.rcFrame.right * gwZoom / 4) / 2 +
				(n * iFrameWidth);
		    rc.right = rc.left + iFrameWidth;
		
		    //
		    // We're on top of this frame!
		    //
                    if (x >= rc.left && x < rc.right) {
			//
			// For the top video stream, it's easy to tell
			// which frame we're on... each frame is
			// displayed in order.
			//
			if (i == giFirstVideo)
			    SelectStream(hwnd, i, lSamp + n, 1, fShift, fCtrl);
			
			
			//
			// For other video streams, we need to calculate
			// the time of the spot we're on, and see which
			// frame is associated with it, because who
			// knows what scale we're using for time.
			//
                        else {
                            l = lTime + MulDiv32(n * iFrameWidth, gdwMicroSecPerPixel, 1000);
			    lCurSamp = AVIStreamTimeToSample( gapavi[i], l);
			    SelectStream(hwnd, i, lCurSamp, 1, fShift, fCtrl);
			}
			
			//
			// Invalidate what we'll be highlighting.
			// This includes the text area above.  If
			// we're adding to a selection, invalidate the
			// whole strip because other frames might
			// become selected by this, and the text
			// changes.
			//
			InflateRect(&rc, HSPACE / 2, VSPACE / 2);
                        if (fShift) {       // could select more than this
			    rc.left = 0;
			    rc.right = rcC.right;
			}
			InvalidateRect(hwnd, &rc, FALSE);
			// Now invalidate the text area
			rc.bottom = rc.top;
			rc.top -= (2 * TSPACE); // !!! text changes
			rc.left = 0;
			rc.right = rcC.right;
			// If we've got selection text already, erase
			InvalidateRect(hwnd, &rc, fShift);
			return;
		    }
		}//end of inner frame walk for statement
	    }
	}
	
	// ********************************************************************
	
	//
	// ... see if we clicked on an audio section
	//
        else if (avis.fccType == streamtypeAUDIO) {
	
	    //
	    // We clicked inside the wave!
	    //
            if (y >= yTop && y < yBottom) {
		
		//
		// Get the time we clicked on, and it's sample number
		//
		GetClientRect(hwnd, &rcC);
		lTime = GetScrollTime(hwnd);
		if (x < (rcC.right/2)) { //button down to the left of current position
                    l = lTime - muldiv32( (rcC.right / 2) - x, gdwMicroSecPerPixel, 1000);
		}
		else {
                    l = lTime + muldiv32(x - rcC.right / 2, gdwMicroSecPerPixel, 1000);
		}
		lCurSamp = AVIStreamTimeToSample(gapavi[i], l);
		
		SelectStream(hwnd, i, lCurSamp, 1, fShift, fCtrl);
		
		// Invalidate what we'll need to repaint to show it
		rc.left = x - 2;        // may not be exact right pixel
		rc.right = x + 3;
		rc.top = yTop;
		rc.bottom = yBottom;
		InflateRect(&rc, 0, VSPACE / 2);
		// If we're adding to a selection, we better invalidate
		// the whole strip.
                if (fShift) {
		    rc.left = 0;
		    rc.right = rcC.right;
		}
		InvalidateRect(hwnd, &rc, FALSE);
		// Now invalidate the header text
		rc.bottom = rc.top;
		rc.top -= (2 * TSPACE); // !!! text changes
		rc.left = 0;
		rc.right = rcC.right;
		// If we've got selection text already, erase
		InvalidateRect(hwnd, &rc, fShift);
	    }
	}
    }//end of stream walk for statement


    return;
}
// ****************************************************************************
//
//      Message Handler for WM_LBUTTONUP
//
// ****************************************************************************
//
void App_OnLButtonUp(HWND hwnd, int x, int y, UINT keyFlags)
{
    //
    // If we're selecting a whole stream, stop highlighting the text area
    //
    if ((gSelectedStream >= 0) && (gSelectedStream < MAXNUMSTREAMS))
    {
	InvalidateRect(hwnd, &grcSelectedStream, TRUE);
	gSelectedStream = -1;
    }
    return;
}
/*----------------------------------------------------------------------------*\
|   AppWndProc( hwnd, uiMessage, wParam, lParam )                              |
|                                                                              |
|   Description:                                                               |
|       The window proc for the app's main (tiled) window.  This processes all |
|       of the parent window's messages.                                       |
|                                                                              |
|   Arguments:                                                                 |
|       hwnd            window handle for the window                           |
|       uiMessage       message number                                         |
|       wParam          message-dependent                                      |
|       lParam          message-dependent                                      |
|                                                                              |
|   Returns:                                                                   |
|       0 if processed, nonzero if ignored                                     |
|                                                                              |
\*----------------------------------------------------------------------------*/
LRESULT CALLBACK AppWndProc( HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    PAINTSTRUCT ps;
    BOOL        f;
    HDC         hdc;

    switch (msg) {
	
	//
	// If we passed a command line filename, open it
	//
	case WM_CREATE:
	    return HANDLE_WM_CREATE(hwnd, wParam, lParam, App_OnCreate);
	
	case WM_COMMAND:
	    HANDLE_WM_COMMAND(hwnd, wParam, lParam, App_OnCommand);
	    break;
	
	case WM_DROPFILES:
	    HANDLE_WM_DROPFILES(hwnd, wParam, lParam, App_OnDropFiles);
	    break;
	
	case WM_INITMENU:
	    HANDLE_WM_INITMENU(hwnd, wParam, lParam, App_OnInitMenu);
	    break;
	
	    //
	    // During a wait state (eg saving) don't let us choose any menus
	    //
	case WM_NCHITTEST:
	
	    if (gfWait)
	    {
		// Let windows tell us where the cursor is
		lParam = DefWindowProc(hwnd,msg,wParam,lParam);
		
		// If it's over a menu, pretend it's in the client (force
		// hourglass)
		if (lParam == HTMENU)
		    lParam = HTCLIENT;
		
		return lParam;
	    }
	    break;
	    //
	    // Set vertical scrollbar for scrolling streams
	    //
	case WM_SIZE:
	    HANDLE_WM_SIZE(hwnd, wParam, lParam, App_OnSize);
	    break;
	
	    //
	    // During a wait state, show an hourglass over our client area
	    // !!! Is this necessary?
	    //
	case WM_SETCURSOR:
	    if (gfWait && LOWORD(lParam) == HTCLIENT)
	    {
		SetCursor(LoadCursor(NULL, IDC_WAIT));
		return TRUE;
	    }
	    break;
	
	    //
	    // We're out of here!
	    //
	case WM_DESTROY:
	    HANDLE_WM_DESTROY(hwnd, wParam, lParam, App_OnDestroy);\
		    break;
	
	case WM_ENDSESSION:
	    HANDLE_WM_ENDSESSION(hwnd, wParam, lParam, App_OnEndSession);
	    break;
	
	    //
	    // Don't let us close ourselves in a wait state (eg saving)
	    //
	case WM_CLOSE:
	    if (gfWait)
		return 0;
	    break;
	
	    //
	    // Block keyboard access to menus if waiting
	    //
	case WM_SYSCOMMAND:
	    switch (wParam & 0xFFF0) {
		case SC_KEYMENU:
		    if (gfWait)
			return 0;
		    break;
	    }
	    break;
	
	case WM_PALETTECHANGED:
	    HANDLE_WM_PALETTECHANGED(hwnd, wParam, lParam, App_OnPaletteChanged);
	    break;
		
	case WM_QUERYNEWPALETTE:

	    if (gfVideoFound) {
		hdc = GetDC(hwnd);
                //
		// Realize the palette of the first video stream
	        //
		if (f = DrawDibRealize(ghdd[giFirstVideo], hdc, FALSE))
		    InvalidateRect(hwnd,NULL,TRUE);

                ReleaseDC(hwnd,hdc);
                return f;
	    }
	    break;
		
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
	    HANDLE_WM_KEYDOWN(hwnd, wParam, lParam, App_OnKey);
	    break;
		
	case WM_HSCROLL:
	    HANDLE_WM_HSCROLL(hwnd, wParam, lParam, App_OnHScroll);
	    break;
		
	case WM_VSCROLL:
	    HANDLE_WM_VSCROLL(hwnd, wParam, lParam, App_OnVScroll);
	    break;
		
	//
	// Deselect everything
	//
	case WM_RBUTTONDOWN:
	    HANDLE_WM_RBUTTONDOWN(hwnd, wParam, lParam, App_OnRButtonDown);
	    break;
		
	//
	// Select something
	//
	case WM_LBUTTONDOWN:
	    HANDLE_WM_LBUTTONDOWN(hwnd, wParam, lParam, App_OnLButtonDown);
	    break;
		
	case WM_LBUTTONDBLCLK:
	    HANDLE_WM_LBUTTONDBLCLK(hwnd, wParam, lParam, App_OnLButtonDown);
	    break;
		
	case WM_LBUTTONUP:
	    HANDLE_WM_LBUTTONUP(hwnd, wParam, lParam, App_OnLButtonUp);
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
|    SaveCallback()                                                             |
|                                                                               |
|    Our save callback that prints our progress in our window title bar         |
\*----------------------------------------------------------------------------*/
BOOL CALLBACK SaveCallback(int iProgress)
{
    char    szText[3*BUFSIZE];
    char    szFormat[BUFSIZE];

    LoadString( ghInstApp, IDS_APPNAME, gszBuffer, BUFSIZE );
    LoadString( ghInstApp, IDS_SAVEFORMAT, szFormat, BUFSIZE );

    wsprintf(szText, szFormat, (LPSTR) gszBuffer, (LPSTR) gszSaveFileName, iProgress);

    SetWindowText(ghwndApp, szText);

    //
    // Give ourselves a chance to abort
    //
    return WinYield();
}


/*----------------------------------------------------------------------------*\
|    GetDlgItemLong()                                                           |
|                                                                               |
|    Get a long integer from a dialog item.                                     |
\*----------------------------------------------------------------------------*/

DWORD GetDlgItemLong (HWND hwnd,  int idCtl, LPINT lpfOK, BOOL fSigned)
{
    LONG        l;
    char        ch;
    BOOL        fNegative = FALSE;
    char        szBuf[64];
    LPSTR       pbuf = szBuf;
    BOOL        fOk;


    fOk = FALSE;
    if (lpfOK)
	*lpfOK = FALSE;

    if (!GetDlgItemText(hwnd, idCtl, (LPSTR)szBuf, sizeof(szBuf)-1))
	return(0);

    while (*pbuf == ' ') pbuf++;

    if (fSigned && *pbuf == '-')
    {
	pbuf++;
	fNegative = TRUE;
    }

    l = 0;
    while ((ch = *pbuf++) >= '0' && ch <= '9')
    {
	fOk = TRUE;
	if (l > (DWORD)(ULONG_MAX/10))
	    return(0);
	l = (l * 10) + ch - '0';
	if (fSigned && l > (DWORD)(ULONG_MAX/2))
	    return(0);
    }

    if (fNegative)
	l = -l;

    if (lpfOK)
	*lpfOK = (ch == 0 && fOk);

    return(l);
}

/*----------------------------------------------------------------------------*\
|    SetDlgItemLong()                                                           |
|                                                                               |
|    Put a long integer into a dialog item.                                     |
\*----------------------------------------------------------------------------*/

void SetDlgItemLong (HWND hwnd, int idCtl, DWORD dwValue, BOOL fSigned )
{
    char szBuf[64];

    wsprintf (szBuf, fSigned ? "%ld" : "%lu", dwValue);
    SetDlgItemText(hwnd, idCtl, szBuf);
}


/*----------------------------------------------------------------------------*\
|    DoDataExchange()                                                           |
|                                                                               |
|    Exchange data between our internal buffer and dialog controls              |
|    fDir = TRUE for DialogBox->Buffer                                          |
|         = FALSE for Buffer->DialogBox                                         |
\*----------------------------------------------------------------------------*/

void DoDataExchange (HWND hwnd, LPAVISTREAMINFO lpinfo, BOOL fDir)
{
    if (fDir) {
	lpinfo->wPriority       = (WORD)GetDlgItemInt (hwnd, IDC_PRIORITY,    NULL, FALSE);
	lpinfo->wLanguage       = (WORD)GetDlgItemInt (hwnd, IDC_LANGUAGE,    NULL, FALSE);
	lpinfo->dwScale         = GetDlgItemLong(hwnd, IDC_SCALE,       NULL, FALSE);
	lpinfo->dwRate          = GetDlgItemLong(hwnd, IDC_RATE,        NULL, FALSE);
	lpinfo->dwStart         = GetDlgItemLong(hwnd, IDC_START,       NULL, FALSE);
	lpinfo->dwQuality       = GetDlgItemLong(hwnd, IDC_QUALITY,     NULL, FALSE);
	lpinfo->rcFrame.top     = GetDlgItemInt (hwnd, IDC_FRAMETOP,    NULL, TRUE);
	lpinfo->rcFrame.bottom  = GetDlgItemInt (hwnd, IDC_FRAMEBOTTOM, NULL, TRUE);
	lpinfo->rcFrame.left    = GetDlgItemInt (hwnd, IDC_FRAMELEFT,   NULL, TRUE);
	lpinfo->rcFrame.right   = GetDlgItemInt (hwnd, IDC_FRAMERIGHT,  NULL, TRUE);
	GetDlgItemText(hwnd, IDC_NAME, lpinfo->szName, sizeof(lpinfo->szName)-1);
    }
    else
    {
	SetDlgItemInt (hwnd, IDC_PRIORITY,    lpinfo->wPriority,       FALSE);
	SetDlgItemInt (hwnd, IDC_LANGUAGE,    lpinfo->wLanguage,       FALSE);
	SetDlgItemLong(hwnd, IDC_SCALE,       lpinfo->dwScale,         FALSE);
	SetDlgItemLong(hwnd, IDC_RATE,        lpinfo->dwRate,          FALSE);
	SetDlgItemLong(hwnd, IDC_START,       lpinfo->dwStart,         FALSE);
	SetDlgItemLong(hwnd, IDC_QUALITY,     lpinfo->dwQuality,       FALSE);
	SetDlgItemLong(hwnd, IDC_FRAMETOP,    lpinfo->rcFrame.top,     TRUE);
	SetDlgItemLong(hwnd, IDC_FRAMEBOTTOM, lpinfo->rcFrame.bottom,  TRUE);
	SetDlgItemLong(hwnd, IDC_FRAMELEFT,   lpinfo->rcFrame.left,    TRUE);
	SetDlgItemLong(hwnd, IDC_FRAMERIGHT,  lpinfo->rcFrame.right,   TRUE);
	SetDlgItemText(hwnd, IDC_NAME, lpinfo->szName);
    }
}

// *****************************************************************************
//
//      FUNCTION: Dlg_DefProc(HWND, UINT, WPARAM, LPARAM)
//
//      PURPOSE:  Handles default messages for all dialog boxes
//
//
static LRESULT Dlg_DefProc( HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam )
{
    return DefDlgProcEx( hDlg, message, wParam, lParam, &gfDefDlgEx );
}
//
// *****************************************************************************
//
//      FUNCTION: InfoDlg_OnInitDialog(HWND, HWND, LPARAM)
//
//      PURPOSE:  Handles initialization for dialog box
//
//
static BOOL InfoDlg_OnInitDialog(HWND hDlg, HWND hwndFocus, LPARAM lParam)
{
    int i;
    glpavisi = (LPAVISTREAMINFO)
	       GlobalAllocPtr (GHND, sizeof(AVISTREAMINFO)*gcpavi);

    for ( i = 0; i < gcpavi; ++i)
    {
	AVIStreamInfo(gapavi[i], &glpavisi[i], sizeof(AVISTREAMINFO));
	SendDlgItemMessage(hDlg, IDC_STREAMS, CB_ADDSTRING, 0, (LPARAM)(LPSTR)glpavisi[i].szName);
    }
    SendDlgItemMessage(hDlg, IDC_STREAMS, CB_SETCURSEL, 0, 0);
    gnSel = 0;
    DoDataExchange(hDlg, &glpavisi[0], FALSE);

    return (FALSE);
}
//
// *****************************************************************************
//
//      FUNCTION: InfoDlg_OnCommand(HWND, HWND, UINT)
//
//      PURPOSE:  Handles the child controls for dialog box
//
//      DIALOGBOX ID'S
//
//              IDOK            - ok button
//              IDCANCEL        - cancel button
//      IDC_STREAMS             - the dropdown listbox
//
static void InfoDlg_OnCommand(HWND hDlg, int control_source, HWND control_handle, UINT control_action )
{

    switch (control_source)
    {
	case IDC_STREAMS:
	{
	    if (control_action != CBN_SELCHANGE)
		break;
	    if (gnSel != CB_ERR)
		DoDataExchange (hDlg, &glpavisi[gnSel], TRUE);
	    gnSel = (int)SendDlgItemMessage(hDlg, IDC_STREAMS, CB_GETCURSEL, 0, 0);
	    if (gnSel != CB_ERR)
		DoDataExchange(hDlg, &glpavisi[gnSel], FALSE);
	}
	break;
	
	case IDOK:
	case IDCANCEL:
	{
	    BOOL fOk = (control_source == IDOK);
	    int i,ix;
	
	    if (fOk)
	    {
		ix = (int)SendDlgItemMessage(hDlg, IDC_STREAMS, CB_GETCURSEL, 0, 0);
		if (ix != CB_ERR)
		    DoDataExchange(hDlg, &glpavisi[ix], TRUE);
		
		for (i = 0; i < gcpavi; ++i)
		    EditStreamSetInfo(gapavi[i], &glpavisi[i], sizeof(AVISTREAMINFO));
	    }
	    GlobalFreePtr(glpavisi);
	    EndDialog(hDlg, fOk);
	}
	break;
	
    }
    return;
}
//
// *****************************************************************************
//
//      FUNCTION: SetInfoNewDlgProc(HWND, UINT, WPARAM, LPARAM)
//
//      PURPOSE:  Processes messages for dialog box using message crackers
//
//      MESSAGES:
//
//              WM_INITDIALOG   - initialize dialog box
//              WM_COMMAND              - process user input
//
// *****************************************************************************
//
static LRESULT SetInfoNewDlgProc( HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
	HANDLE_MSG( hDlg, WM_INITDIALOG,            InfoDlg_OnInitDialog            );
	HANDLE_MSG( hDlg, WM_COMMAND,                       InfoDlg_OnCommand               );
	default:
	    return Dlg_DefProc( hDlg, message, wParam, lParam );
    }
}
//
//
// *****************************************************************************
//
//      FUNCTION: SetInfoDlgProc(HWND, UINT, WPARAM, LPARAM)
//
//      PURPOSE:  Processes messages for info dialog box
//
//
BOOL CALLBACK SetInfoDlgProc( HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    CheckDefDlgRecursion( &gfDefDlgEx );
    return SetDlgMsgResult( hDlg, message, SetInfoNewDlgProc( hDlg, message, wParam, lParam ) );
}


/*----------------------------------------------------------------------------*\
|    SetStreamInfo()                                                            |
|                                                                               |
|    Bring up the dialog to allow setting the stream info                       |
\*----------------------------------------------------------------------------*/

BOOL SetStreamInfo (HWND hwnd)
{
    return(DialogBox(ghInstApp, MAKEINTRESOURCE(IDD_STREAMINFO), hwnd, (DLGPROC)SetInfoDlgProc));
}



/*----------------------------------------------------------------------------*\
|    editPaste()                                                                |
|                                                                               |
|    PASTE the streams in this PFILE into our movie using the following logic:  |
|                                                                               |
|    Take a stream from the clipboard.  If you can find a similar type stream   |
|    in the app with a selection, paste it in before the selection.  If no      |
|    such stream exists, add it to the end.                                     |
\*----------------------------------------------------------------------------*/

void editPaste(HWND hwnd, PAVIFILE pfile)
{
    int         i, j, nVideo = 0, nAudio = 0, nStream;
    LONG        l;
    PAVISTREAM  pavi;
    AVISTREAMINFO avisClip, avis;

    FreeDrawStuff(hwnd);

    for (i=0; i<MAXNUMSTREAMS; i++) {
	if (AVIFileGetStream(pfile, &pavi, 0L, i) != AVIERR_OK)
	    break;
	AVIStreamInfo(pavi, &avisClip, sizeof(avisClip));
	nStream = (avisClip.fccType == streamtypeVIDEO) ? nVideo : nAudio;
	for (j=nStream; j<gcpavi; j++) {
	    AVIStreamInfo(gapavi[j], &avis, sizeof(avis));
	    if (avis.fccType == avisClip.fccType && galSelStart[j] != -1) {
		l = AVIStreamLength(pavi);
		if (EditStreamPaste(gapavi[j], &galSelStart[j], &l,
				    pavi, AVIStreamStart(pavi), AVIStreamLength(pavi)) !=
		    AVIERR_OK)
		{
		    LoadString( ghInstApp, IDS_NOPASTE, gszBuffer, BUFSIZE );
		    ErrMsg(gszBuffer);
		}
		galSelLen[j] = AVIStreamLength(pavi);
		break;
	    }
	}
	if (j == gcpavi) {
	    galSelStart[j] = AVIStreamStart(pavi);
	    galSelLen[j] = AVIStreamLength(pavi);
	    if (CreateEditableStream(&gapavi[j], pavi) != AVIERR_OK)
	    {
		LoadString( ghInstApp, IDS_PASTEERROR, gszBuffer, BUFSIZE );
		ErrMsg(gszBuffer);
		AVIStreamRelease(pavi);
		break;
	    }
	    AVIStreamRelease(pavi);
	    gcpavi++;
	}
	if (avisClip.fccType == streamtypeVIDEO)
	    nVideo = ++j;
	else
	    nAudio = ++j;
    }
    AVIFileRelease(pfile);
    InitStreams(hwnd);
    FixWindowTitle(hwnd);
}
/*----------------------------------------------------------------------------*\
|    MenuHandler()                                                              |
|                                                                               |
|    Process all of our Menu messages.                                  |
\*----------------------------------------------------------------------------*/
BOOL MenuHandler( HWND hwnd, int nMenuID )
{
    OPENFILENAME ofn;

    switch(nMenuID)
    {
	case MENU_ABOUT:
	    //
	    // Display an informative dialog box.
	    //
            DialogBox(ghInstApp, MAKEINTRESOURCE(IDD_ABOUT), hwnd, (DLGPROC)AboutDlgProc);
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
	    AVISaveOptions(hwnd,
			   ICMF_CHOOSE_KEYFRAME | ICMF_CHOOSE_DATARATE |
			   ICMF_CHOOSE_PREVIEW,
			   gcpavi, gapavi, galpAVIOptions);
	    break;
	
	    //
	    // Save all the open streams into a file
	    //
	case MENU_SAVEAS:
	
	    gszSaveFileName[0] = 0;
	
	    //
	    // prompt user for file to save
	    //
	    ofn.lStructSize = sizeof(OPENFILENAME);
	    ofn.hwndOwner = hwnd;
	    ofn.hInstance = NULL;
	    AVIBuildFilter(gszFilter, sizeof(gszFilter), TRUE);
	    ofn.lpstrFilter = gszFilter;
	    ofn.lpstrCustomFilter = NULL;
	    ofn.nMaxCustFilter = 0;
	    ofn.nFilterIndex = 0;
	    ofn.lpstrFile = gszSaveFileName;
	    ofn.nMaxFile = sizeof(gszSaveFileName);
	    ofn.lpstrFileTitle = NULL;
	    ofn.nMaxFileTitle = 0;
	    ofn.lpstrInitialDir = NULL;
	    LoadString( ghInstApp, IDS_SAVETITLE, gszBuffer, BUFSIZE );
	    ofn.lpstrTitle = gszBuffer;
	    ofn.Flags = OFN_PATHMUSTEXIST | OFN_HIDEREADONLY |
			OFN_OVERWRITEPROMPT;
	    ofn.nFileOffset = 0;
	    ofn.nFileExtension = 0;
	    LoadString( ghInstApp, IDS_DEFEXT, gszBuffer, BUFSIZE );
	    ofn.lpstrDefExt = gszBuffer;
	    ofn.lCustData = 0;
	    ofn.lpfnHook = NULL;
	    ofn.lpTemplateName = NULL;
	
	    //
	    // If we get a filename, save it
	    //
	    if (GetSaveFileName(&ofn))
	    {
		    DWORD       fccHandler[MAXNUMSTREAMS];
		    int         i;
		    HRESULT     hr;
		
		    StartWait();
		
		    for (i = 0; i < gcpavi; i++)
			fccHandler[i] = galpAVIOptions[i]->fccHandler;
		
		    hr = AVISaveV(gszSaveFileName,
				  NULL,
                              (AVISAVECALLBACK) SaveCallback,
				  gcpavi,
				  gapavi,
				  galpAVIOptions);
                if (hr != AVIERR_OK) {
                    switch (hr) {
                    case AVIERR_FILEOPEN:
                        LoadString( ghInstApp, IDS_ERROVERWRITE, gszBuffer, BUFSIZE );
                        ErrMsg(gszBuffer);
                        break;
                    default:
			LoadString( ghInstApp, IDS_SAVEERROR, gszBuffer, BUFSIZE );
			ErrMsg(gszBuffer);
		    }
                }
		    // Now put the video compressors back that we stole
		    for (i = 0; i < gcpavi; i++)
			galpAVIOptions[i]->fccHandler = fccHandler[i];
		
		    EndWait();
		    FixWindowTitle(hwnd);
	    }
	    break;
	
	    //
	    // Close everything
	    //
	case MENU_CLOSE:
	    FreeAvi(hwnd);
	    gszFileName[0] = '\0';
	    FixWindowTitle(hwnd);
	    break;
	
	    //
	    // Open a new file, or merge streams with a new file
	    //
	case MENU_OPEN:
	case MENU_MERGE:
	    gszFileName[0] = 0;
	
	    //
	    // prompt user for file to open
	    //
	    ofn.lStructSize = sizeof(OPENFILENAME);
	    ofn.hwndOwner = hwnd;
	    ofn.hInstance = NULL;
	    if (nMenuID == MENU_MERGE)
		LoadString( ghInstApp, IDS_MERGETITLE, gszBuffer, BUFSIZE );
	    else
		LoadString( ghInstApp, IDS_OPENTITLE, gszBuffer, BUFSIZE );
	    ofn.lpstrTitle = gszBuffer;
	    AVIBuildFilter(gszFilter, sizeof(gszFilter), FALSE);
	    ofn.lpstrFilter = gszFilter;
	    ofn.lpstrCustomFilter = NULL;
	    ofn.nMaxCustFilter = 0;
	    ofn.nFilterIndex = 0;
	    ofn.lpstrFile = gszFileName;
	    ofn.nMaxFile = sizeof(gszFileName);
	    ofn.lpstrFileTitle = NULL;
	    ofn.nMaxFileTitle = 0;
	    ofn.lpstrInitialDir = NULL;
	    ofn.Flags = OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST |OFN_HIDEREADONLY;
	    ofn.nFileOffset = 0;
	    ofn.nFileExtension = 0;
	    ofn.lpstrDefExt = NULL;
	    ofn.lCustData = 0;
	    ofn.lpfnHook = NULL;
	    ofn.lpTemplateName = NULL;
	
	    //
	    // If we've got a filename, go open it
	    //
	    if (GetOpenFileNamePreview(&ofn))
		InitAvi(hwnd, gszFileName, nMenuID);
	
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
	case MENU_PREVIEW:
	    if (gfAudioFound)
		aviaudioPlay(hwnd,
			     gapavi[giFirstAudio],
			     AVIStreamTimeToSample(gapavi[giFirstAudio], GetScrollTime(hwnd)),
			     AVIStreamEnd(gapavi[giFirstAudio]),
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
	
	case MENU_SETINFO:
	    if (SetStreamInfo(hwnd))
	    {
		FreeDrawStuff(hwnd);        // !!! in order to call InitStreams
		InitStreams(hwnd);          // !!! Nukes COMP options
		InvalidateRect(hwnd, NULL, TRUE);
	    }
	    break;
	
	case MENU_CUT:
	case MENU_COPY:
	case MENU_DELETE:
	{
	    PAVIFILE    pf;
	    int         i;
	
	    //
	    // Walk our list of selections and make streams out of each section
	    //
	    gcpaviSel = 0;
	    for (i = 0; i < gcpavi; i++) {
		if (galSelStart[i] != -1) {
		    // !!! What if the start and length change?
		    if (nMenuID == MENU_COPY) {
			if (EditStreamCopy(gapavi[i], &galSelStart[i],
					   &galSelLen[i], &gapaviSel[gcpaviSel++]) != 0) {
			    --gcpaviSel;
			    LoadString( ghInstApp, IDS_STRCPYERR, gszBuffer, BUFSIZE );
			    ErrMsg(gszBuffer);
			}
		    } else {
			if (EditStreamCut(gapavi[i], &galSelStart[i],
					  &galSelLen[i], &gapaviSel[gcpaviSel++]) != 0) {
			    --gcpaviSel;
			    LoadString( ghInstApp, IDS_STRCUTERR, gszBuffer, BUFSIZE );
			    ErrMsg(gszBuffer);
			}
		    }
		}
	    }
	
	
	    for (i = gcpavi - 1; i >= 0; i--) {
		// Check to see if any stream is entirely gone now....
		if (AVIStreamLength(gapavi[i]) == 0) {
		    NukeAVIStream(i);
		}
	    }
	
	    //
	    // Put the selected stuff up on the clipboard
	    //
	    if (gcpaviSel && nMenuID != MENU_DELETE) {
		PAVISTREAM          gapaviTemp[MAXNUMSTREAMS];
		int i;
		
		//
		// Clone the edited streams, so that if the user does
		// more editing, the thing on the clipboard won't
		// suddenly change....
		//
		for (i = 0; i < gcpaviSel; i++) {
		    gapaviTemp[i] = NULL;
		    // !!! error check
		    EditStreamClone(gapaviSel[i], &gapaviTemp[i]);
		}
		
		AVIMakeFileFromStreams(&pf, gcpaviSel, gapaviTemp);
		if (AVIPutFileOnClipboard(pf) != AVIERR_OK)
		{
		    LoadString( ghInstApp, IDS_NOCLIP, gszBuffer, BUFSIZE );
		    ErrMsg(gszBuffer);
		}
		for (i = 0; i < gcpaviSel; i++) {
		    AVIStreamRelease(gapaviTemp[i]);
		}
		
		AVIFileRelease(pf);
	    }
	
	    for (i = 0; i < gcpaviSel; i++)
		AVIStreamRelease(gapaviSel[i]);
	
	    //
	    // If we cut out the selections, then they don't exist anymore.
	    //
	    if (gcpaviSel && (nMenuID == MENU_DELETE || nMenuID == MENU_CUT)) {
		SelectStream(hwnd, -1, -1, -1, FALSE, FALSE);
	    }
	
	    //
	    // We just changed the world!
	    //
	    FreeDrawStuff(hwnd);        // !!! in order to call InitStreams
	    InitStreams(hwnd);          // !!! Nukes COMP options
	    InvalidateRect(hwnd, NULL, TRUE);
	
	    break;
	}
	
	case MENU_PASTE:
	{
	    PAVIFILE pf;
	
	    AVIGetFromClipboard(&pf);
	
	    if (pf) {
		editPaste(hwnd, pf);
	    }
	    break;
	}
	case MENU_NAME:
	    if (ghwndEdit == NULL)
		EditStreamName(hwnd);
	    else
		EditDone(hwnd, FALSE);
	
	
	    break;
    }
    return TRUE;
}
/*-----------------------------------------------------------------------------
 * FrameVideo()
 *
 * Puts a border around a video frame, the size of a selection
 */

void FrameVideo(HDC hdc, RECT *rcFrame, HBRUSH hbr)
{
   RECT rcTop,rcBottom,rcLeft,rcRight;
   HBRUSH hbrOld;

   // Calculate 4 rectangles, which 'frame' rcFrame.
   rcTop.left = rcFrame->left - HSPACE/2;
   rcTop.top = rcFrame->top - SELECTVSPACE;
   rcTop.right = rcFrame->right + HSPACE/2;
   rcTop.bottom = rcFrame->top;
   rcLeft.left = rcFrame->left - HSPACE/2;
   rcLeft.top = rcFrame->top - SELECTVSPACE;
   rcLeft.right = rcFrame->left;
   rcLeft.bottom = rcFrame->bottom + SELECTVSPACE;
   rcRight.left = rcFrame->right;
   rcRight.top = rcFrame->top - SELECTVSPACE;
   rcRight.right = rcFrame->right + HSPACE/2;
   rcRight.bottom = rcFrame->bottom + SELECTVSPACE;
   rcBottom.left = rcFrame->left - HSPACE/2;
   rcBottom.top = rcFrame->bottom;
   rcBottom.right = rcFrame->right + HSPACE/2;
   rcBottom.bottom = rcFrame->bottom + SELECTVSPACE;

   // Now put each rectangle on screen
   hbrOld = SelectObject(hdc, hbr);
   PatBlt(hdc, rcTop.left, rcTop.top, rcTop.right - rcTop.left, rcTop.bottom - rcTop.top, PATCOPY);
   PatBlt(hdc, rcLeft.left, rcLeft.top, rcLeft.right - rcLeft.left, rcLeft.bottom - rcLeft.top, PATCOPY);
   PatBlt(hdc, rcRight.left, rcRight.top, rcRight.right - rcRight.left, rcRight.bottom - rcRight.top, PATCOPY);
   PatBlt(hdc, rcBottom.left, rcBottom.top, rcBottom.right - rcBottom.left, rcBottom.bottom - rcBottom.top, PATCOPY);
   SelectObject(hdc, hbrOld);

}

/*----------------------------------------------------------------------------*\
|   ErrMsg()                                                                    |
|                                                                               |
|   Opens a Message box with a error message in it.  The user can               |
|   select the OK button to continue                                            |
\*----------------------------------------------------------------------------*/

int ErrMsg (LPSTR sz,...)
{
    static char szOutput[4*BUFSIZE];

    va_list va;

    va_start(va, sz);
    wvsprintf (szOutput,sz,va);      /* Format the string */
    va_end(va);
    MessageBox(NULL,szOutput,NULL, MB_OK|MB_ICONEXCLAMATION|MB_TASKMODAL);
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
	    return TRUE;
    }
    return FALSE;
}
