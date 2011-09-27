// This is a part of the Microsoft Foundation Classes C++ library.
// Copyright (C) 1992-1993 Microsoft Corporation
// All rights reserved.
//
// This source code is only intended as a supplement to the
// Microsoft Foundation Classes Reference and Microsoft
// QuickHelp documentation provided with the library.
// See these sources for detailed information regarding the
// Microsoft Foundation Classes product.

/////////////////////////////////////////////////////////////////////////////
// Auxiliary System/Screen metrics

#ifdef AFX_CLASS_MODEL
struct NEAR AUX_DATA
#else
struct AUX_DATA
#endif
{
	// System metrics
	int     cxVScroll, cyHScroll;
	int     cxIcon, cyIcon;

	// Device metrics for screen
	int     cxPixelsPerInch, cyPixelsPerInch;
	int     cySysFont;

	// Solid brushes with convenient gray colors and system colors
	HBRUSH  hbrLtGray, hbrDkGray;
	HBRUSH	hbrBtnHilite, hbrBtnFace, hbrBtnShadow;
	HPEN	hpenBtnHilite, hpenBtnShadow, hpenBtnText;

	// Color values of system colors used for CToolBar
	COLORREF    clrBtnFace, clrBtnShadow, clrBtnHilite;
	COLORREF    clrBtnText, clrWindowFrame;

	// Standard cursors
	HCURSOR     hcurWait;
	HCURSOR     hcurArrow;
	HCURSOR     hcurHelp;       // cursor used in Shift+F1 help

	// Special GDI objects allocated on demand
	HFONT   hStatusFont;
	HBITMAP hbmMenuDot;
	void (PASCAL* pfnFreeToolBar)();    // cleanup procedure

	BOOL    bWin31;             // TRUE if 3.1 or higher
	BOOL    bDBCS;              // TRUE if system is DBCS enabled

// Implementation
	AUX_DATA();
	~AUX_DATA();
	void UpdateSysColors();
};

extern AUX_DATA NEAR afxData;

// NOTE: we don't use afxData.cxBorder and afxData.cyBorder anymore
#define CX_BORDER   1
#define CY_BORDER   1

// states for Shift+F1 hep mode
#define HELP_INACTIVE   0   // not in Shift+F1 help mode (must be 0)
#define HELP_ACTIVE     1   // in Shift+F1 help mode (non-zero)
#define HELP_ENTERING   2   // entering Shift+F1 help mode (non-zero)

/////////////////////////////////////////////////////////////////////////////
// Window class names and other window creation support

// from wincore.cpp
extern const char BASED_CODE _afxWnd[];           // simple child windows/controls
extern const char BASED_CODE _afxWndControlBar[]; // controls with grey backgrounds
extern const char BASED_CODE _afxWndMDIFrame[];
extern const char BASED_CODE _afxWndFrameOrView[];
extern MSG NEAR _afxLastMsg;

/////////////////////////////////////////////////////////////////////////////
// Define global state in ordinary "C" globals

#ifdef _AFXDLL
// use macros for inline access to stack globals
#define afxCurrentWinApp (_AfxGetAppData()->appCurrentWinApp)
#define afxCurrentInstanceHandle (_AfxGetAppData()->appCurrentInstanceHandle)
#define afxCurrentResourceHandle (_AfxGetAppData()->appCurrentResourceHandle)
#define afxCurrentAppName (_AfxGetAppData()->appCurrentAppName)
#define _afxFirstFrameWnd (_AfxGetAppData()->appFirstFrameWnd)
#else //_AFXDLL
extern CFrameWnd* NEAR _afxFirstFrameWnd;
#endif

#ifndef _AFXDLL
#ifndef _USRDLL
extern HHOOK NEAR _afxHHookOldMsgFilter;
#endif
#else
#define _afxHHookOldMsgFilter _AfxGetAppData()->appHHookOldMsgFilter
#endif

#ifndef _AFXDLL
extern HBRUSH NEAR afxDlgBkBrush; // dialog and message box background brush
extern COLORREF NEAR afxDlgTextClr;
#else
#define afxDlgBkBrush   (_AfxGetAppData()->appDlgBkBrush)
#define afxDlgTextClr   (_AfxGetAppData()->appDlgTextClr)
#endif //_AFXDLL

void PASCAL _AfxHookWindowCreate(CWnd* pWnd);
BOOL PASCAL _AfxUnhookWindowCreate();

LRESULT CALLBACK AFX_EXPORT _AfxDlgProc(HWND, UINT, WPARAM, LPARAM);
UINT CALLBACK AFX_EXPORT _AfxCommDlgProc(HWND hWnd, UINT, WPARAM, LPARAM);

// Support for standard dialogs
extern const UINT NEAR _afxNMsgSETRGB;
typedef UINT (CALLBACK* COMMDLGPROC)(HWND, UINT, UINT, LONG);

// Special mode helpers
HWND PASCAL _AfxGetSafeOwner(CWnd* pParent);
BOOL PASCAL _AfxIsComboBoxControl(HWND hWnd, UINT nStyle);
void PASCAL _AfxCancelModes(HWND hWndRcvr);
BOOL PASCAL _AfxHelpEnabled();  // determine if ID_HELP handler exists
HWND PASCAL _AfxGetParentOwner(HWND hWnd);
BOOL PASCAL _AfxIsDescendant(HWND hWndParent, HWND hWndChild);

// String helpers
void PASCAL _AfxSmartSetWindowText(HWND hWndCtrl, LPCSTR lpszNew);
int  PASCAL _AfxLoadString(UINT nIDS, char* pszBuf);    // 255 char buffer

#ifndef _AfxStrChr
LPSTR PASCAL _AfxStrChr(LPCSTR lpszIn, char chFind);
#endif
#ifndef _AfxIsDBCSLeadByte
BOOL PASCAL _AfxIsDBCSLeadByte(BYTE b);
#endif

// ambient AnsiNext helper macro
#define _AfxAnsiNext(psz)   ((char*)_AfxGetPtrFromFarPtr(AnsiNext((LPCSTR)psz)))

BOOL _AfxIsStackOK(UINT nStackNeeded);

/////////////////////////////////////////////////////////////////////////////
// Sub-Segment Allocation

LPVOID  PASCAL _AfxLocalAlloc(UINT sgmnt, UINT wFlags, UINT wBytes);
void    PASCAL _AfxLocalFree(LPVOID lhBlock);
BOOL    PASCAL _AfxLocalUnlock(LPVOID lhBlock);
LPSTR   PASCAL _AfxLocalLock(LPVOID lhBlock);

/////////////////////////////////////////////////////////////////////////////
// Portability abstractions

#define _AfxSetDlgCtrlID(hWnd, nID)     SetWindowWord(hWnd, GWW_ID, (WORD)nID)
#define _AfxGetHookHandle() \
	GetModuleHandle((LPCSTR)MAKELONG(AfxGetInstanceHandle(),0))

#define _AfxGetDlgCtrlID(hWnd)          ((UINT)(WORD)::GetDlgCtrlID(hWnd))

// misc helpers
void PASCAL _AfxStrCpy(LPSTR lpszDest, LPCSTR lpszSrc, size_t nSizeDest);
BOOL PASCAL _AfxFullPath(LPSTR lpszPathOut, LPCSTR lpszFileIn);

AFX_MSGMAP_ENTRY FAR* PASCAL _AfxFindMessageEntry(
	AFX_MSGMAP_ENTRY FAR* lpEntry, UINT nMsg, UINT nID);

#ifndef _AFXDLL
// In non-AFXDLL, the resource handle is always AfxGetResourceHandle
#define AfxFindResourceHandle(lpszResource, lpszType) AfxGetResourceHandle()
#endif

/////////////////////////////////////////////////////////////////////////////
// Debugging/Tracing helpers

#ifdef _DEBUG
void AFXAPI AfxOutputDebugString(LPCSTR lpszOutput);
void AFXAPI _AfxTraceMsg(LPCSTR lpszPrefix, const MSG* pMsg);
BOOL AFXAPI _AfxCheckDialogTemplate(LPCSTR lpszResource, BOOL bInvisibleChild);
#endif

/////////////////////////////////////////////////////////////////////////////
// Delete on exit for MFC allocated objects (emits debug message)

void PASCAL _AfxExitDelete(HGDIOBJ hObject);

/////////////////////////////////////////////////////////////////////////////
// Memory Allocation Failure Handler

#include <new.h>

#ifdef _AFXDLL
_PNH AFXAPI _AfxSetNewHandler(_PNH);
#else
#define _AfxSetNewHandler(pnh)  _set_new_handler(pnh)
#endif

/////////////////////////////////////////////////////////////////////////////
