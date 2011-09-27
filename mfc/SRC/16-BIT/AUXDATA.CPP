// This is a part of the Microsoft Foundation Classes C++ library.
// Copyright (C) 1992-1993 Microsoft Corporation
// All rights reserved.
//
// This source code is only intended as a supplement to the
// Microsoft Foundation Classes Reference and Microsoft
// QuickHelp and/or WinHelp documentation provided with the library.
// See these sources for detailed information regarding the
// Microsoft Foundation Classes product.

#include "stdafx.h"
#include <malloc.h>

#ifdef AFX_INIT_SEG
#pragma code_seg(AFX_INIT_SEG)
#endif

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// System metrics etc
/////////////////////////////////////////////////////////////////////////////

AUX_DATA NEAR afxData;

// Initialization code
AUX_DATA::AUX_DATA()
{
	HDC hDCScreen = GetDC(NULL);
	ASSERT(hDCScreen != NULL);

	// System metrics
	cxVScroll = GetSystemMetrics(SM_CXVSCROLL);
	cyHScroll = GetSystemMetrics(SM_CYHSCROLL);
	cxIcon = GetSystemMetrics(SM_CXICON);
	cyIcon = GetSystemMetrics(SM_CYICON);

	// Device metrics for screen
	cxPixelsPerInch = GetDeviceCaps(hDCScreen, LOGPIXELSX);
	cyPixelsPerInch = GetDeviceCaps(hDCScreen, LOGPIXELSY);
	cySysFont = HIWORD(GetTextExtent(hDCScreen, "M", 1));

	ReleaseDC(NULL, hDCScreen);

	// Border attributes
	hbrLtGray = ::CreateSolidBrush(RGB(192, 192, 192));
	hbrDkGray = ::CreateSolidBrush(RGB(128, 128, 128));
	ASSERT(hbrLtGray != NULL);
	ASSERT(hbrDkGray != NULL);

	// Cached system values (updated in CFrameWnd::OnSysColorChange)
	hbrBtnFace = NULL;
	hbrBtnShadow = NULL;
	hbrBtnHilite = NULL;
	hpenBtnShadow = NULL;
	hpenBtnHilite = NULL;
	hpenBtnText = NULL;
	UpdateSysColors();

	// Standard cursors
	hcurWait = ::LoadCursor(NULL, IDC_WAIT);
	hcurArrow = ::LoadCursor(NULL, IDC_ARROW);
	ASSERT(hcurWait != NULL);
	ASSERT(hcurArrow != NULL);
	hcurHelp = NULL;    // loaded on demand

	WORD nVersion = LOWORD(::GetVersion());
	bWin31 = ((LOBYTE(nVersion) << 8) + HIBYTE(nVersion)) >= 0x030a;
	bDBCS  = GetSystemMetrics(SM_DBCSENABLED);

	// allocated on demand
	hStatusFont = NULL;
	hbmMenuDot = NULL;
	pfnFreeToolBar = NULL;
}

// Termination code
AUX_DATA::~AUX_DATA()
{
	// cleanup
	_AfxExitDelete(hbrLtGray);
	_AfxExitDelete(hbrDkGray);
	_AfxExitDelete(hbrBtnFace);
	_AfxExitDelete(hbrBtnShadow);
	_AfxExitDelete(hbrBtnHilite);
	_AfxExitDelete(afxDlgBkBrush);

	// cleanup standard pens
	_AfxExitDelete(hpenBtnShadow);
	_AfxExitDelete(hpenBtnHilite);
	_AfxExitDelete(hpenBtnText);

	// clean up objects we don't actually create
	_AfxExitDelete(hStatusFont);
	_AfxExitDelete(hbmMenuDot);
	if (pfnFreeToolBar != NULL)
		(*pfnFreeToolBar)();        // toolbar cleanup uses _AfxExitDelete
}

void AUX_DATA::UpdateSysColors()
{
	clrBtnFace = ::GetSysColor(COLOR_BTNFACE);
	clrBtnShadow = ::GetSysColor(COLOR_BTNSHADOW);
	if (bWin31)
		clrBtnHilite = ::GetSysColor(COLOR_BTNHIGHLIGHT);
	else
		clrBtnHilite = RGB(255,255,255);
	clrBtnText = ::GetSysColor(COLOR_BTNTEXT);
	clrWindowFrame = ::GetSysColor(COLOR_WINDOWFRAME);

	if (hbrBtnFace != NULL)
		::DeleteObject(hbrBtnFace);
	if (hbrBtnShadow != NULL)
		::DeleteObject(hbrBtnShadow);
	if (hbrBtnHilite != NULL)
		::DeleteObject(hbrBtnHilite);
	hbrBtnFace = ::CreateSolidBrush(clrBtnFace);
	hbrBtnShadow = ::CreateSolidBrush(clrBtnShadow);
	hbrBtnHilite = ::CreateSolidBrush(clrBtnHilite);
	ASSERT(hbrBtnFace != NULL);
	ASSERT(hbrBtnShadow != NULL);
	ASSERT(hbrBtnHilite != NULL);

	if (hpenBtnShadow != NULL)
		::DeleteObject(hpenBtnShadow);
	if (hpenBtnHilite != NULL)
		::DeleteObject(hpenBtnHilite);
	if (hpenBtnText != NULL)
		::DeleteObject(hpenBtnText);
	hpenBtnShadow = ::CreatePen(PS_SOLID, 0, clrBtnShadow);
	hpenBtnHilite = ::CreatePen(PS_SOLID, 0, clrBtnHilite);
	hpenBtnText = ::CreatePen(PS_SOLID, 0, clrBtnText);
	ASSERT(hpenBtnText != NULL);
	ASSERT(hpenBtnShadow != NULL);
	ASSERT(hpenBtnHilite != NULL);
}

/////////////////////////////////////////////////////////////////////////////
// Other compatibility helpers for different versions of Windows

void PASCAL _AfxExitDelete(HGDIOBJ hObject)
{
	if (hObject == NULL)
		return;

	::DeleteObject(hObject);

#ifdef _WINDLL      // any DLL
#ifdef _DEBUG
	// Debug Kernel warns about these not being deleted but they really are
	// deleted when the DLL is implicitly loaded by Windows.

	// NOTE: you may wish to output a similar message in your application
	// if you rely on Windows to implicitly load and free your DLL.
	// If you explicitly load/free your DLL, this message is not needed.

	if (afxData.bWin31 && ::GetSystemMetrics(SM_DEBUG))
	{
		// Windows 3.0 does not always allow OutputDebugString in retail

		static char NEAR szMsg[64]; // try not to use stack space
#ifdef _AFXCTL
		static char BASED_CODE szFormat[] =
			"Info OC25: object has been safely deleted: %04X\r\n";
#else
		static char BASED_CODE szFormat[] =
			"Info MFC250: object has been safely deleted: %04X\r\n";
#endif

		ASSERT(::wsprintf(szMsg, szFormat, hObject) < sizeof(szMsg));
		AfxOutputDebugString(szMsg);
	}
#endif // _DEBUG
#endif // _WINDLL
}

#ifdef _DEBUG
#ifdef _WINDLL
/////////////////////////////////////////////////////////////////////////////
// Slow diagnostic output for times when Trace is not properly initialized.

static char BASED_CODE szIniFile[] = "AFX.INI";
static char BASED_CODE szDiagSection[] = "Diagnostics";
static char BASED_CODE szTraceEnabled[] = "TraceEnabled";

void AFXAPI AfxOutputDebugString(LPCSTR lpszOutput)
{
	if (::GetPrivateProfileInt(szDiagSection, szTraceEnabled,
			FALSE, szIniFile))
		OutputDebugString(lpszOutput);
}
#endif
#endif

/////////////////////////////////////////////////////////////////////////////
