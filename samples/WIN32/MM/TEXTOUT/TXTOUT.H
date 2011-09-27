/****************************************************************************
 *
 *   TxtOut.h
 *
 ***************************************************************************/
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
typedef struct {
	WORD w;
} ICSTATE;

#define BUFFER_SIZE 256

typedef struct {
	DWORD		dwFlags;	// flags from ICOPEN
	ICSTATE		CurrentState;	// current state of compressor.
	int		nDraw;		// count of DRAW_BEGIN calls
	HFONT		hfont;
	ICDRAWBEGIN	db;
	RECT		rc;
	int		iLen;
	char		achBuffer[BUFFER_SIZE];
} INSTINFO, *PINSTINFO;

BOOL		NEAR PASCAL Load(void);
void		NEAR PASCAL Free(void);
INSTINFO*	NEAR PASCAL Open(ICOPEN FAR *icinfo);
LRESULT		NEAR PASCAL Close(INSTINFO * pinst);
LRESULT		NEAR PASCAL GetState(INSTINFO * pinst, LPVOID pv, DWORD dwSize);
LRESULT		NEAR PASCAL SetState(INSTINFO * pinst, LPVOID pv, DWORD dwSize);
LRESULT		NEAR PASCAL GetInfo(INSTINFO * pinst, ICINFO FAR *icinfo, DWORD dwSize);

BOOL		NEAR PASCAL QueryAbout(INSTINFO * pinst);
LRESULT		NEAR PASCAL About(INSTINFO * pinst, HWND hwnd);
BOOL		NEAR PASCAL QueryConfigure(INSTINFO * pinst);
LRESULT		NEAR PASCAL Configure(INSTINFO * pinst, HWND hwnd);

LRESULT		NEAR PASCAL DrawQuery(INSTINFO * pinst, LPVOID FAR * lpFormat);
LRESULT		NEAR PASCAL DrawBegin(INSTINFO * pinst,ICDRAWBEGIN FAR *icinfo, DWORD dwSize);
LRESULT		NEAR PASCAL Draw(INSTINFO * pinst,ICDRAW FAR *icinfo, DWORD dwSize);
LRESULT		NEAR PASCAL DrawEnd(INSTINFO * pinst);
BOOL 		CALLBACK ConfigureDlgProc(
			HWND hdlg,
			UINT msg,
			WPARAM wParam,
			LPARAM lParam);

#ifdef DEBUG
	extern void FAR CDECL dprintf(LPSTR, ...);
	extern void FAR CDECL dprintf2(LPSTR, ...);
	#define DPF dprintf
	#define DPF2 dprintf2
#else
	#define DPF / ## /	// Pasting 2 slashes together comments out
	#define DPF2 / ## /	// the rest of the line.
#endif

// string IDs
#define IDS_DESCRIPTION		112	// 0x70, so WIN16 rc.exe can put 16
#define IDS_NAME		113	// strings in a segment
