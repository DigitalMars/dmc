// circ3ppg.cpp : Implementation of the CCirc3PropPage property page class.
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
#include "circ3.h"
#include "circ3ppg.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif


IMPLEMENT_DYNCREATE(CCirc3PropPage, COlePropertyPage)


/////////////////////////////////////////////////////////////////////////////
// Message map

BEGIN_MESSAGE_MAP(CCirc3PropPage, COlePropertyPage)
	//{{AFX_MSG_MAP(CCirc3PropPage)
	// NOTE - ClassWizard will add and remove message map entries
	//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// Initialize class factory and guid

IMPLEMENT_OLECREATE_EX(CCirc3PropPage, "CIRC3.Circ3PropPage.1",
	0x9dbafcd3, 0x592f, 0x101b, 0x85, 0xce, 0x0, 0x60, 0x8c, 0xec, 0x29, 0x7b)


/////////////////////////////////////////////////////////////////////////////
// CCirc3PropPage::CCirc3PropPageFactory::UpdateRegistry -
// Adds or removes system registry entries for CCirc3PropPage

BOOL CCirc3PropPage::CCirc3PropPageFactory::UpdateRegistry(BOOL bRegister)
{
	if (bRegister)
		return AfxOleRegisterPropertyPageClass(AfxGetInstanceHandle(),
			m_clsid, IDS_CIRC3_PPG);
	else
		return AfxOleUnregisterClass(m_clsid, NULL);
}


/////////////////////////////////////////////////////////////////////////////
// CCirc3PropPage::CCirc3PropPage - Constructor

CCirc3PropPage::CCirc3PropPage() :
	COlePropertyPage(IDD, IDS_CIRC3_PPG_CAPTION)
{
	//{{AFX_DATA_INIT(CCirc3PropPage)
	m_caption = _T("");
	m_circleOffset = 0;
	m_circleShape = FALSE;
	m_note = _T("");
	//}}AFX_DATA_INIT
}


/////////////////////////////////////////////////////////////////////////////
// CCirc3PropPage::DoDataExchange - Moves data between page and properties

void CCirc3PropPage::DoDataExchange(CDataExchange* pDX)
{
	//{{AFX_DATA_MAP(CCirc3PropPage)
	DDP_Text(pDX, IDC_CAPTION, m_caption, _T("Caption") );
	DDX_Text(pDX, IDC_CAPTION, m_caption);
	DDP_Text(pDX, IDC_CIRCLEOFFSET, m_circleOffset, _T("CircleOffset") );
	DDX_Text(pDX, IDC_CIRCLEOFFSET, m_circleOffset);
	DDP_Check(pDX, IDC_CIRCLESHAPE, m_circleShape, _T("CircleShape") );
	DDX_Check(pDX, IDC_CIRCLESHAPE, m_circleShape);
	DDP_Text(pDX, IDC_NOTE, m_note, _T("Note") );
	DDX_Text(pDX, IDC_NOTE, m_note);
	//}}AFX_DATA_MAP
	DDP_PostProcessing(pDX);
}


/////////////////////////////////////////////////////////////////////////////
// CCirc3PropPage message handlers
