// spin.cpp: Spin control - C++ class wrapping "MicroScroll" control in a DLL
//
// This is a part of the Microsoft Foundation Classes C++ library.
// Copyright (C) 1992 Microsoft Corporation
// All rights reserved.
//
// This source code is only intended as a supplement to the
// Microsoft Foundation Classes Reference and Microsoft
// WinHelp documentation provided with the library.
// See these sources for detailed information regarding the
// Microsoft Foundation Classes product.

#include "stdafx.h"
#include "ctrltest.h"

#include "spin.h"

IMPLEMENT_DYNAMIC(CSpinControl, CWnd)

BOOL CSpinControl::Create(DWORD dwStyle, const RECT& rect,
		CWnd* pParentWnd, UINT nID)
{
	return CWnd::Create("MicroScroll", NULL, dwStyle, rect, pParentWnd, nID);
}

WNDPROC* CSpinControl::GetSuperWndProcAddr()
{
	static WNDPROC pfnSuper;
	return &pfnSuper;
}
