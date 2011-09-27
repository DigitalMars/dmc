// DrawPicP.cpp : Implementation of the CDrawPicPropPage property page class.

// This is a part of the Microsoft Foundation Classes C++ library.
// Copyright (C) 1992-1996 Microsoft Corporation
// All rights reserved.
//
// This source code is only intended as a supplement to the
// Microsoft Foundation Classes Reference and related
// electronic documentation provided with the library.
// See these sources for detailed information regarding the
// Microsoft Foundation Classes product.

#include "stdafx.h"
#include "DrawPic.h"
#include "DrawPicP.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


IMPLEMENT_DYNCREATE(CDrawPicPropPage, COlePropertyPage)


/////////////////////////////////////////////////////////////////////////////
// Message map

BEGIN_MESSAGE_MAP(CDrawPicPropPage, COlePropertyPage)
	//{{AFX_MSG_MAP(CDrawPicPropPage)
	// NOTE - ClassWizard will add and remove message map entries
	//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// Initialize class factory and guid

IMPLEMENT_OLECREATE_EX(CDrawPicPropPage, "DRAWPIC.DrawPicPropPage.1",
	0x57f5a424, 0xa324, 0x11cf, 0xb4, 0xa4, 0x44, 0x45, 0x53, 0x54, 0, 0)


/////////////////////////////////////////////////////////////////////////////
// CDrawPicPropPage::CDrawPicPropPageFactory::UpdateRegistry -
// Adds or removes system registry entries for CDrawPicPropPage

BOOL CDrawPicPropPage::CDrawPicPropPageFactory::UpdateRegistry(BOOL bRegister)
{
	if (bRegister)
		return AfxOleRegisterPropertyPageClass(AfxGetInstanceHandle(),
			m_clsid, IDS_DRAWPIC_PPG);
	else
		return AfxOleUnregisterClass(m_clsid, NULL);
}


/////////////////////////////////////////////////////////////////////////////
// CDrawPicPropPage::CDrawPicPropPage - Constructor

CDrawPicPropPage::CDrawPicPropPage() :
	COlePropertyPage(IDD, IDS_DRAWPIC_PPG_CAPTION)
{
	//{{AFX_DATA_INIT(CDrawPicPropPage)
	// NOTE: ClassWizard will add member initialization here
	//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_DATA_INIT
}


/////////////////////////////////////////////////////////////////////////////
// CDrawPicPropPage::DoDataExchange - Moves data between page and properties

void CDrawPicPropPage::DoDataExchange(CDataExchange* pDX)
{
	//{{AFX_DATA_MAP(CDrawPicPropPage)
	// NOTE: ClassWizard will add DDP, DDX, and DDV calls here
	//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_DATA_MAP
	DDP_PostProcessing(pDX);
}


/////////////////////////////////////////////////////////////////////////////
// CDrawPicPropPage message handlers
