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
#include "afxpen.h"

#ifdef _AFXDLL
// For the MFCDLL build, non-header data is still 'AFXAPI_DATA'
#undef AFXAPP_DATA
#define AFXAPP_DATA     AFXAPI_DATA
#endif

#ifdef AFX_AUX_SEG
#pragma code_seg(AFX_AUX_SEG)
#endif

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

/////////////////////////////////////////////////////////////////////////////
// CHEdit

IMPLEMENT_DYNAMIC(CHEdit, CEdit)

WNDPROC* CHEdit::GetSuperWndProcAddr()
{
	static WNDPROC NEAR pfnSuper;
	return &pfnSuper;
}

BOOL CHEdit::Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID)
{
	return CWnd::Create("HEDIT", NULL, dwStyle, rect, pParentWnd, nID);
}

CHEdit::~CHEdit()
{
	DestroyWindow();
}

/////////////////////////////////////////////////////////////////////////////
// CBEdit

IMPLEMENT_DYNAMIC(CBEdit, CHEdit)

WNDPROC* CBEdit::GetSuperWndProcAddr()
{
	static WNDPROC NEAR pfnSuper;
	return &pfnSuper;
}

BOOL CBEdit::Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID)
{
	return CWnd::Create("BEDIT", NULL, dwStyle, rect, pParentWnd, nID);
}

CBEdit::~CBEdit()
{
	DestroyWindow();
}

/////////////////////////////////////////////////////////////////////////////
// Inline function declarations expanded out-of-line

#ifndef _AFX_ENABLE_INLINES

#ifdef AFX_DBG2_SEG
#pragma code_seg(AFX_DBG2_SEG)
#endif

static char BASED_CODE _szAfxPenInl[] = "afxpen.inl";
#undef THIS_FILE
#define THIS_FILE _szAfxPenInl
#define _AFXPEN_INLINE
#include "afxpen.inl"

#endif //!_AFX_ENABLE_INLINES


/////////////////////////////////////////////////////////////////////////////
