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
#include "winhand_.h"

#ifdef AFX_CORE1_SEG
#pragma code_seg(AFX_CORE1_SEG)
#endif

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// Map from HMENU to CMenu *

#ifndef _AFXDLL
static CHandleMap NEAR _afxMapHMENU(RUNTIME_CLASS(CMenu),
	offsetof(CMenu, m_hMenu));
#else
#define _afxMapHMENU (*_AfxGetAppData()->appMapHMENU)
#endif

void PASCAL CMenu::DeleteTempMap()
{
	_afxMapHMENU.DeleteTemp();
}

CMenu* PASCAL CMenu::FromHandle(HMENU hMenu)
{
	CMenu* pMenu = (CMenu*)_afxMapHMENU.FromHandle(hMenu);
	ASSERT(pMenu == NULL || pMenu->m_hMenu == hMenu);
	return pMenu;
}

/////////////////////////////////////////////////////////////////////////////
// CMenu

IMPLEMENT_DYNCREATE(CMenu, CObject)

#ifdef _DEBUG
void CMenu::AssertValid() const
{
	CObject::AssertValid();
	if (afxData.bWin31)
	{
		ASSERT(m_hMenu == NULL || ::IsMenu(m_hMenu));
	}
}

void CMenu::Dump(CDumpContext& dc) const
{
	CObject::Dump(dc);
	AFX_DUMP1(dc, "\nm_hMenu = ", (UINT)m_hMenu);
}
#endif

BOOL CMenu::Attach(HMENU hMenu)
{
	ASSERT(m_hMenu == NULL);        // only attach once, detach on destroy
	if (hMenu == NULL)
		return FALSE;
	_afxMapHMENU.SetPermanent(m_hMenu = hMenu, this);
	return TRUE;
}

HMENU CMenu::Detach()
{
	HMENU hMenu;
	if ((hMenu = m_hMenu) != NULL)
		_afxMapHMENU.RemoveHandle(m_hMenu);
	m_hMenu = NULL;
	return hMenu;
}

CMenu::~CMenu()
{
	DestroyMenu();
}

BOOL CMenu::DestroyMenu()
{
	if (m_hMenu == NULL)
		return FALSE;
	return ::DestroyMenu(Detach());
}

/////////////////////////////////////////////////////////////////////////////
// Self-drawing menu items

void CMenu::DrawItem(LPDRAWITEMSTRUCT /* lpDrawItemStruct */)
{
	// default drawing does nothing
}

void CMenu::MeasureItem(LPMEASUREITEMSTRUCT /* lpMeasureItemStruct */)
{
	// default drawing does nothing
}

/////////////////////////////////////////////////////////////////////////////
