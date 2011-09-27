// circ2ppg.cpp : Implementation of the CCirc2PropPage property page class.
//
// This is a part of the Microsoft Foundation Classes C++ library.
// Copyright (C) 1992-1995 Microsoft Corporation
// All rights reserved.
//
// This source code is only intended as a supplement to the
// Microsoft Foundation Classes Reference and related
// electronic documentation provided with the library.
// See these sources for detailed information regarding the
// Microsoft Foundation Classes product.

#include "stdafx.h"
#include "circ2.h"
#include "circ2ppg.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif


IMPLEMENT_DYNCREATE(CCirc2PropPage, COlePropertyPage)


/////////////////////////////////////////////////////////////////////////////
// Message map

BEGIN_MESSAGE_MAP(CCirc2PropPage, COlePropertyPage)
	//{{AFX_MSG_MAP(CCirc2PropPage)
	// NOTE - ClassWizard will add and remove message map entries
	//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// Initialize class factory and guid

IMPLEMENT_OLECREATE_EX(CCirc2PropPage, "CIRC2.Circ2PropPage.1",
	0x9dbafcce, 0x592f, 0x101b, 0x85, 0xce, 0x0, 0x60, 0x8c, 0xec, 0x29, 0x7b)


/////////////////////////////////////////////////////////////////////////////
// CCirc2PropPage::CCirc2PropPageFactory::UpdateRegistry -
// Adds or removes system registry entries for CCirc2PropPage

BOOL CCirc2PropPage::CCirc2PropPageFactory::UpdateRegistry(BOOL bRegister)
{
	if (bRegister)
		return AfxOleRegisterPropertyPageClass(AfxGetInstanceHandle(),
			m_clsid, IDS_CIRC2_PPG);
	else
		return AfxOleUnregisterClass(m_clsid, NULL);
}


/////////////////////////////////////////////////////////////////////////////
// CCirc2PropPage::CCirc2PropPage - Constructor

CCirc2PropPage::CCirc2PropPage() :
	COlePropertyPage(IDD, IDS_CIRC2_PPG_CAPTION)
{
	//{{AFX_DATA_INIT(CCirc2PropPage)
	// NOTE: ClassWizard will add member initialization here
	//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_DATA_INIT
}


/////////////////////////////////////////////////////////////////////////////
// CCirc2PropPage::DoDataExchange - Moves data between page and properties

void CCirc2PropPage::DoDataExchange(CDataExchange* pDX)
{
	//{{AFX_DATA_MAP(CCirc2PropPage)
	// NOTE: ClassWizard will add DDP, DDX, and DDV calls here
	//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_DATA_MAP
	DDP_PostProcessing(pDX);
}


/////////////////////////////////////////////////////////////////////////////
// CCirc2PropPage message handlers
