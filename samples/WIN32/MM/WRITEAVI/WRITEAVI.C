/**************************************************************************
 *
 *  THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF ANY
 *  KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
 *  IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A PARTICULAR
 *  PURPOSE.
 *
 *  Copyright (c) 1993, 1994  Microsoft Corporation.  All Rights Reserved.
 *
 **************************************************************************/
/****************************************************************************
 *
 *  WRITEAVI.C
 *
 *  Creates the file OUTPUT.AVI, an AVI file consisting of a rotating clock
 *  face.  This program demonstrates using the functions in AVIFILE.DLL
 *  to make writing AVI files simple.
 *
 *  This is a stripped-down example; a real application would have a user
 *  interface and check for errors.
 *
 ***************************************************************************/


#define  STRICT
#define  INC_OLE2
#include <windows.h>
#include <windowsx.h>
#include <memory.h>
#include <mmsystem.h>
#include <vfw.h>

#include "writeavi.h"
//---------------------------------------------------------------------------
// Defines
//---------------------------------------------------------------------------
// Our movie is 160x120 and 15 frames long
//
#define BITMAP_X	160
#define BITMAP_Y	120
#define N_FRAMES	15

#define TEXT_HEIGHT	20

#define AVIIF_KEYFRAME	0x00000010L // this frame is a key frame.

#define BUFSIZE 260

#define LPLPBI	LPBITMAPINFOHEADER *

//---------------------------------------------------------------------------
// Function declarations
//---------------------------------------------------------------------------
static void FreeFrames(LPLPBI) ;
static void MakeFrames(LPLPBI, UINT, UINT, UINT) ;
static HANDLE MakeDib(HBITMAP, UINT);

//---------------------------------------------------------------------------
// A quick lookup table for Sin and Cos values
//---------------------------------------------------------------------------
//
static int aSin[N_FRAMES] = {
	0,    40,    74,    95,    99,
	86,    58,    20,    -20,    -58,
	-86,    -99,    -95,    -74,    -40,
	} ;

static int aCos[N_FRAMES] = {
	100,    91,    66,    30,    -10,
	-49,    -80,    -97,    -97,    -80,
	-50,    -10,    30,    66,    91,
	} ;

//----------------------------------------------------------------------------
//
// We don't have a window, we just pop up a dialog
// box, write the file, and quit
//
int PASCAL WinMain(
HINSTANCE hInstance,
HINSTANCE hPrevInstance,
LPSTR szCmdLine,
int sw)
{
	LPBITMAPINFOHEADER alpbi[N_FRAMES];
	int i;
	AVISTREAMINFOW strhdr;
	PAVIFILE pfile = NULL;
	PAVISTREAM ps = NULL, psCompressed = NULL, psText = NULL;
	char szText[BUFSIZE];
	int iLen;
	AVICOMPRESSOPTIONS opts;
	HRESULT hr;
	DWORD dwTextFormat;
	WORD wVer;
	char szTitle[BUFSIZE];
	char szMessage[BUFSIZE];

	/* first let's make sure we are running on 1.1 */
	wVer = HIWORD(VideoForWindowsVersion());
	if (wVer < 0x010a){
		 /* oops, we are too old, blow out of here */
		LoadString(hInstance, IDS_APPERR, szTitle, BUFSIZE );
		LoadString(hInstance, IDS_VFWTOOOLD, szMessage, BUFSIZE );
		MessageBeep(MB_ICONHAND);
		MessageBox(NULL, szMessage, szTitle, MB_OK|MB_ICONSTOP);
		return FALSE;
	}

	alpbi[0] = NULL;

	LoadString(hInstance, IDS_APPNAME, szTitle, BUFSIZE );
	LoadString(hInstance, IDS_INTRO, szMessage, BUFSIZE );

	if (MessageBox(NULL, szMessage, szTitle, MB_OKCANCEL) == IDCANCEL)
		return 0;

	//
	// Set up the bitmaps for the file in an array
	//
	MakeFrames(alpbi, 8, BITMAP_X, BITMAP_Y);

	AVIFileInit();

	//
	// Open the movie file for writing....
	//
	LoadString(hInstance, IDS_FILENAME, szTitle, BUFSIZE );
	
	hr = AVIFileOpen(&pfile,		    // returned file pointer
		       szTitle,		            // file name
		       OF_WRITE | OF_CREATE,	    // mode to open file with
		       NULL);			    // use handler determined
						    // from file extension....
	if (hr != AVIERR_OK)
		goto error;

	// Fill in the header for the video stream....

	// The video stream will run in 15ths of a second....

	_fmemset(&strhdr, 0, sizeof(strhdr));
	strhdr.fccType                = streamtypeVIDEO;// stream type
	strhdr.fccHandler             = 0;
	strhdr.dwScale                = 1;
	strhdr.dwRate                 = 15;		    // 15 fps
	strhdr.dwSuggestedBufferSize  = alpbi[0]->biSizeImage;
	SetRect(&strhdr.rcFrame, 0, 0,		    // rectangle for stream
	    (int) alpbi[0]->biWidth,
	    (int) alpbi[0]->biHeight);

	// And create the stream;
	hr = AVIFileCreateStream(pfile,		    // file pointer
			         &ps,		    // returned stream pointer
			         &strhdr);	    // stream header
	if (hr != AVIERR_OK) {
		goto error;
	}

	 _fmemset(&opts, 0, sizeof(opts));

	opts.dwFlags = AVICOMPRESSF_VALID;
	opts.fccType = streamtypeVIDEO;
	opts.fccHandler = mmioFOURCC('M', 'S', 'V', 'C');
	opts.dwQuality = 6500;
	opts.dwKeyFrameEvery = 7;

	hr = AVIMakeCompressedStream(&psCompressed, ps, &opts, NULL);
	if (hr != AVIERR_OK) {
		goto error;
	}

	hr = AVIStreamSetFormat(psCompressed, 0,
			       alpbi[0],	    // stream format
			       alpbi[0]->biSize +   // format size
			       alpbi[0]->biClrUsed * sizeof(RGBQUAD));
	if (hr != AVIERR_OK) {
	goto error;
	}

	// Fill in the stream header for the text stream....

	// The text stream is in 60ths of a second....

	_fmemset(&strhdr, 0, sizeof(strhdr));
	strhdr.fccType                = streamtypeTEXT;
	strhdr.fccHandler             = mmioFOURCC('D', 'R', 'A', 'W');
	strhdr.dwScale                = 1;
	strhdr.dwRate                 = 60;
	strhdr.dwSuggestedBufferSize  = sizeof(szText);
	SetRect(&strhdr.rcFrame, 0, (int) alpbi[0]->biHeight,
	    (int) alpbi[0]->biWidth, (int) alpbi[0]->biHeight + TEXT_HEIGHT);

	// ....and create the stream.
	hr = AVIFileCreateStream(pfile, &psText, &strhdr);
	if (hr != AVIERR_OK) {
		goto error;
	}

	dwTextFormat = sizeof(dwTextFormat);
	hr = AVIStreamSetFormat(psText, 0, &dwTextFormat, sizeof(dwTextFormat));
	if (hr != AVIERR_OK) {
		goto error;
	}

	//
	// Now write out each video frame, along with a text label.
	// The video frames are 2/3 of a second apart, which is 10
	// in the video time scale and 40 in the text stream's time scale.
	//
	for (i = 0; i < N_FRAMES; i++) {
		hr = AVIStreamWrite(psCompressed,	// stream pointer
			i * 10,				// time of this frame
			1,				// number to write
			(LPBYTE) alpbi[i] +		// pointer to data
				alpbi[i]->biSize +
				alpbi[i]->biClrUsed * sizeof(RGBQUAD),
				alpbi[i]->biSizeImage,	// size of this frame
			AVIIF_KEYFRAME,			 // flags....
			NULL,
			NULL);
		if (hr != AVIERR_OK)
			break;
	
		// Make some text to put in the file ...
		LoadString(hInstance, IDS_TEXTFORMAT, szMessage, BUFSIZE );
		
		iLen = wsprintf(szText, szMessage, (int)(i + 1));

		// ... and write it as well.
		hr = AVIStreamWrite(psText,
				i * 40,
				1,
				szText,
				iLen + 1,
				AVIIF_KEYFRAME,
				NULL,
				NULL);
		if (hr != AVIERR_OK)
			break;
	}
	
error:
	//
	// Now close the file
	//
	if (ps)
		AVIStreamClose(ps);

	if (psCompressed)
		AVIStreamClose(psCompressed);

	if (psText)
		AVIStreamClose(psText);

	if (pfile)
		AVIFileClose(pfile);

	AVIFileExit();
	FreeFrames(alpbi);

	if (hr != NOERROR) {
		LoadString(hInstance, IDS_APPERR, szTitle, BUFSIZE );
		LoadString(hInstance, IDS_WRITEERR, szMessage, BUFSIZE );
		
		MessageBox(NULL, szMessage, szTitle, MB_OK);
	}
	return 0;
}




//
// Fill an array of LPBI's with the frames for this movie
//
static void MakeFrames(LPLPBI alpbi, UINT bits, UINT wXSize,UINT wYSize )
{
	HDC         hdc ;
	HDC         hdcMem ;
	HBITMAP     hbitmap,hbitmapOld ;
	HPEN        hpen3,hpen1,hpenwhite,hpenOld ;
	HFONT       hfont,hfontOld ;
	HBRUSH      hbrush,hbrushOld ;
	RECT        rc ;
	RECT        rcFrameNo ;
	int         wXCent,wYCent ;
	int         cxPixInch ;
	int         cyPixInch ;
	int         cxPixels ;
	int         cyPixels ;
	int         radius ;
	int         x0,y0,x1,y1 ;
	int         i,j ;
	char        szNumber[3] ;

	//
	// Make sure our resources are freed
	//
	FreeFrames(alpbi);

	//
	// Find the center of the movie
	//
	wXCent = wXSize/2 ;
	wYCent = wYSize/2 ;

	hdc = GetDC(NULL) ;
	hdcMem = CreateCompatibleDC(NULL) ;

	//
	// We need some gray and white brushes and pens, and a bitmap
	//
	hpen3 = CreatePen(PS_SOLID,3,RGB(128,128,128)) ;
	hpen1 = CreatePen(PS_SOLID,1,RGB(64,64,64));
	hpenwhite = CreatePen(PS_SOLID,1,RGB(255,255,255));
	hpenOld = SelectPen(hdcMem, hpen3);
	hbrush = CreateSolidBrush(RGB(192,192,192)) ;
	hbrushOld = SelectBrush(hdcMem,hbrush) ;
	hbitmap = CreateCompatibleBitmap(hdc,wXSize,wYSize) ;

	cxPixInch = GetDeviceCaps(hdc,LOGPIXELSX) ;
	cyPixInch = GetDeviceCaps(hdc,LOGPIXELSY) ;

	//
	// What radius of circle can we fit in this frame?  Make sure it's round
	// regardless of the aspect ratio
	//
	radius = ( wXSize < wYSize ) ? wXSize : (wYSize*cxPixInch)/cyPixInch ;
	radius = ( radius * 95 ) / 200 ;

	//
	// Make a Rectangle in the center where the number will go
	//
	/* x0 = radius / sqrt(2) */
	x0 = (radius*100)/141 ;
	y0 = (x0*cyPixInch)/cxPixInch ;
	x0 = (x0*9)/10 ;
	y0 = (y0*9)/10 ;
	SetRect( &rcFrameNo,wXCent-x0,wYCent-y0,wXCent+x0,wYCent+y0 ) ;

	//
	// Move the rectangle in a little and make a font big enough for it
	//
	x0 = (x0*9)/10 ;
	y0 = (y0*9)/10 ;

	hfont = CreateFont(
		y0*2,
		x0,
		0,
		0,
		FW_BOLD,
		0,
		0,
		0,
		ANSI_CHARSET,
		OUT_DEVICE_PRECIS,
		CLIP_DEFAULT_PRECIS,
		DEFAULT_QUALITY,
		DEFAULT_PITCH|FF_SWISS,
		NULL
	);

	hfontOld = SelectFont(hdcMem, hfont);

	//
	// Now walk through and make all the frames
	//
	for ( i=0; i<N_FRAMES; i++ ) {
		hbitmapOld = SelectBitmap(hdcMem, hbitmap);
	
		//
		// Fill the whole frame with white
		//
		SetRect(&rc,0,0,wXSize,wYSize) ;
		FillRect(hdcMem,&rc,GetStockBrush(WHITE_BRUSH)) ;

		//
		// Draw the circle inside the previously calculated radius
		//
		cxPixels = radius ;
		cyPixels = (cxPixels*cyPixInch)/cxPixInch ;

		SelectPen(hdcMem,hpen3) ;
		Ellipse(hdcMem,wXCent-cxPixels,wYCent-cyPixels,wXCent+cxPixels,
			wYCent+cyPixels) ;

		SelectPen(hdcMem,hpen1) ;

		//
		// Draw the number in the previously calculated area
		//
		wsprintf(szNumber,"%02u",i+1) ;

		SetBkColor(hdcMem,RGB(192,192,192)) ;
		SetTextColor(hdcMem,RGB(255,255,255)) ;
		ExtTextOut(
			hdcMem,
			rcFrameNo.left,
			rcFrameNo.top+(rcFrameNo.bottom-rcFrameNo.top)/20,
			ETO_CLIPPED,
			&rcFrameNo,
			szNumber,
			2,
			NULL);

		//
		// Draw tic marks around the inside of the circle in equal divisions
		//
		for ( j=0; j<N_FRAMES; j++ ) {
			x0 = (radius*aSin[j])/100 ;
			y0 = (radius*aCos[j])/100 ;
			x1 = (((radius*aSin[j])/100)*11)/12 ;
			y1 = (((radius*aCos[j])/100)*11)/12 ;

			y0 = -(y0*cyPixInch)/cxPixInch ;
			y1 = -(y1*cyPixInch)/cxPixInch ;

			MoveToEx(hdcMem,wXCent+x0,wYCent+y0,NULL) ;
			LineTo(hdcMem,wXCent+x1,wYCent+y1) ;
		}

		//
		// Now draw the hand of the clock in the appropriate position
		//
		x1 = (((radius*aSin[i])/100)*5)/8 ;
		y1 = (((radius*aCos[i])/100)*5)/8 ;
		y1 = -(y1*cyPixInch)/cxPixInch ;

		MoveToEx(hdcMem,wXCent,wYCent,NULL) ;
		LineTo(hdcMem,wXCent+x1,wYCent+y1) ;

		SelectBitmap(hdcMem, hbitmapOld);
		//
		// Make this into a DIB and stuff it into the array
		//
		alpbi[i] = (LPBITMAPINFOHEADER)GlobalLock(MakeDib(hbitmap, bits));

		//
		// For an error, just duplicate the last frame if we can
		//
		if (alpbi[i] == NULL && i )
			alpbi[i] = alpbi[i-1] ;
	}

	//
	// Select all the old objects back and delete resources
	//
	SelectPen(hdcMem, hpenOld);
	SelectBrush(hdcMem,hbrushOld) ;
	SelectFont(hdcMem,hfontOld) ;
	DeletePen(hpen1) ;
	DeletePen(hpen3) ;
	DeletePen(hpenwhite) ;
	DeleteBrush(hbrush) ;
	DeleteBitmap(hbitmap) ;
	DeleteFont(hfont) ;
	DeleteObject(hdcMem) ;
	ReleaseDC(NULL,hdc) ;
}

//
// Walk through our array of LPBI's and free them
//
static void FreeFrames(LPLPBI alpbi)
{
	UINT        w ;

	if (!alpbi[0])
		return ;
	//
	// Don't free a frame if it's a duplicate of the previous one
	//
	for (w=0; w<N_FRAMES; w++)
		if (alpbi[w] && alpbi[w] != alpbi[w-1])
			GlobalFreePtr(alpbi[w]);
	for (w=0; w<N_FRAMES; w++)
		alpbi[w] = NULL;
}

/*
** MakeDib(hbitmap)
**
** Take the given bitmap and transform it into a DIB with parameters:
**
** BitsPerPixel:    8
** Colors:          palette
**
*/
static HANDLE  MakeDib( HBITMAP hbitmap, UINT bits )
{
	HANDLE              hdib ;
	HDC                 hdc ;
	BITMAP              bitmap ;
	UINT                wLineLen ;
	DWORD               dwSize ;
	DWORD               wColSize ;
	LPBITMAPINFOHEADER  lpbi ;
	LPBYTE              lpBits ;
	
	GetObject(hbitmap,sizeof(BITMAP),&bitmap) ;

	//
	// DWORD align the width of the DIB
	// Figure out the size of the colour table
	// Calculate the size of the DIB
	//
	wLineLen = (bitmap.bmWidth*bits+31)/32 * 4;
	wColSize = sizeof(RGBQUAD)*((bits <= 8) ? 1<<bits : 0);
	dwSize = sizeof(BITMAPINFOHEADER) + wColSize +
		(DWORD)(UINT)wLineLen*(DWORD)(UINT)bitmap.bmHeight;

	//
	// Allocate room for a DIB and set the LPBI fields
	//
	hdib = GlobalAlloc(GHND,dwSize);
	if (!hdib)
		return hdib ;

	lpbi = (LPBITMAPINFOHEADER)GlobalLock(hdib) ;

	lpbi->biSize = sizeof(BITMAPINFOHEADER) ;
	lpbi->biWidth = bitmap.bmWidth ;
	lpbi->biHeight = bitmap.bmHeight ;
	lpbi->biPlanes = 1 ;
	lpbi->biBitCount = (WORD) bits ;
	lpbi->biCompression = BI_RGB ;
	lpbi->biSizeImage = dwSize - sizeof(BITMAPINFOHEADER) - wColSize ;
	lpbi->biXPelsPerMeter = 0 ;
	lpbi->biYPelsPerMeter = 0 ;
	lpbi->biClrUsed = (bits <= 8) ? 1<<bits : 0;
	lpbi->biClrImportant = 0 ;

	//
	// Get the bits from the bitmap and stuff them after the LPBI
	//
	lpBits = (LPBYTE)(lpbi+1)+wColSize ;

	hdc = CreateCompatibleDC(NULL) ;

	GetDIBits(hdc,hbitmap,0,bitmap.bmHeight,lpBits,(LPBITMAPINFO)lpbi, DIB_RGB_COLORS);

	// Fix this if GetDIBits messed it up....
	lpbi->biClrUsed = (bits <= 8) ? 1<<bits : 0;

	DeleteDC(hdc) ;

	return hdib ;
}
