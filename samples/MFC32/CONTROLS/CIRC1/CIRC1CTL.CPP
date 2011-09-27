// circ1ctl.cpp : Implementation of the CCirc1Ctrl OLE control class.
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
#include "circ1.h"
#include "circ1ctl.h"
#include "circ1ppg.h"


#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif


IMPLEMENT_DYNCREATE(CCirc1Ctrl, COleControl)


/////////////////////////////////////////////////////////////////////////////
// Message map

BEGIN_MESSAGE_MAP(CCirc1Ctrl, COleControl)
	//{{AFX_MSG_MAP(CCirc1Ctrl)
	// NOTE - ClassWizard will add and remove message map entries
	//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG_MAP
	ON_OLEVERB(AFX_IDS_VERB_PROPERTIES, OnProperties)
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// Dispatch map

BEGIN_DISPATCH_MAP(CCirc1Ctrl, COleControl)
	//{{AFX_DISPATCH_MAP(CCirc1Ctrl)
	// NOTE - ClassWizard will add and remove dispatch map entries
	//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_DISPATCH_MAP
	DISP_FUNCTION_ID(CCirc1Ctrl, "AboutBox", DISPID_ABOUTBOX, AboutBox, VT_EMPTY, VTS_NONE)
END_DISPATCH_MAP()


/////////////////////////////////////////////////////////////////////////////
// Event map

BEGIN_EVENT_MAP(CCirc1Ctrl, COleControl)
	//{{AFX_EVENT_MAP(CCirc1Ctrl)
	// NOTE - ClassWizard will add and remove event map entries
	//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_EVENT_MAP
END_EVENT_MAP()


/////////////////////////////////////////////////////////////////////////////
// Property pages

// TODO: Add more property pages as needed.  Remember to increase the count!
BEGIN_PROPPAGEIDS(CCirc1Ctrl, 1)
	PROPPAGEID(CCirc1PropPage::guid)
END_PROPPAGEIDS(CCirc1Ctrl)


/////////////////////////////////////////////////////////////////////////////
// Initialize class factory and guid

IMPLEMENT_OLECREATE_EX(CCirc1Ctrl, "CIRC1.Circ1Ctrl.1",
	0x9dbafcc5, 0x592f, 0x101b, 0x85, 0xce, 0x0, 0x60, 0x8c, 0xec, 0x29, 0x7b)


/////////////////////////////////////////////////////////////////////////////
// Type library ID and version

IMPLEMENT_OLETYPELIB(CCirc1Ctrl, _tlid, _wVerMajor, _wVerMinor)


/////////////////////////////////////////////////////////////////////////////
// Interface IDs

const IID BASED_CODE IID_DCirc1 =
		{ 0x9dbafcc6, 0x592f, 0x101b, { 0x85, 0xce, 0x0, 0x60, 0x8c, 0xec, 0x29, 0x7b } };
const IID BASED_CODE IID_DCirc1Events =
		{ 0x9dbafcc7, 0x592f, 0x101b, { 0x85, 0xce, 0x0, 0x60, 0x8c, 0xec, 0x29, 0x7b } };


/////////////////////////////////////////////////////////////////////////////
// Control type information

static const DWORD BASED_CODE _dwCirc1OleMisc =
	OLEMISC_ACTIVATEWHENVISIBLE |
	OLEMISC_SETCLIENTSITEFIRST |
	OLEMISC_INSIDEOUT |
	OLEMISC_CANTLINKINSIDE |
	OLEMISC_RECOMPOSEONRESIZE;

IMPLEMENT_OLECTLTYPE(CCirc1Ctrl, IDS_CIRC1, _dwCirc1OleMisc)


/////////////////////////////////////////////////////////////////////////////
// CCirc1Ctrl::CCirc1CtrlFactory::UpdateRegistry -
// Adds or removes system registry entries for CCirc1Ctrl

BOOL CCirc1Ctrl::CCirc1CtrlFactory::UpdateRegistry(BOOL bRegister)
{
	if (bRegister)
		return AfxOleRegisterControlClass(
			AfxGetInstanceHandle(),
			m_clsid,
			m_lpszProgID,
			IDS_CIRC1,
			IDB_CIRC1,
			FALSE,                      //  Not insertable
			_dwCirc1OleMisc,
			_tlid,
			_wVerMajor,
			_wVerMinor);
	else
		return AfxOleUnregisterClass(m_clsid, m_lpszProgID);
}


/////////////////////////////////////////////////////////////////////////////
// CCirc1Ctrl::CCirc1Ctrl - Constructor

CCirc1Ctrl::CCirc1Ctrl()
{
	InitializeIIDs(&IID_DCirc1, &IID_DCirc1Events);

	// TODO: Initialize your control's instance data here.
}


/////////////////////////////////////////////////////////////////////////////
// CCirc1Ctrl::~CCirc1Ctrl - Destructor

CCirc1Ctrl::~CCirc1Ctrl()
{
	// TODO: Cleanup your control's instance data here.
}


/////////////////////////////////////////////////////////////////////////////
// CCirc1Ctrl::OnDraw - Drawing function

void CCirc1Ctrl::OnDraw(
			CDC* pdc, const CRect& rcBounds, const CRect&)
{
	// TODO: Replace the following code with your own drawing code.
	pdc->FillRect(rcBounds, CBrush::FromHandle((HBRUSH)GetStockObject(WHITE_BRUSH)));
	pdc->Ellipse(rcBounds);
}


/////////////////////////////////////////////////////////////////////////////
// CCirc1Ctrl::DoPropExchange - Persistence support

void CCirc1Ctrl::DoPropExchange(CPropExchange* pPX)
{
	ExchangeVersion(pPX, MAKELONG(_wVerMinor, _wVerMajor));
	COleControl::DoPropExchange(pPX);

	// TODO: Call PX_ functions for each persistent custom property.

}


/////////////////////////////////////////////////////////////////////////////
// CCirc1Ctrl::OnResetState - Reset control to default state

void CCirc1Ctrl::OnResetState()
{
	COleControl::OnResetState();  // Resets defaults found in DoPropExchange

	// TODO: Reset any other control state here.
}


/////////////////////////////////////////////////////////////////////////////
// CCirc1Ctrl::AboutBox - Display an "About" box to the user

void CCirc1Ctrl::AboutBox()
{
	CDialog dlgAbout(IDD_ABOUTBOX_CIRC1);
	dlgAbout.DoModal();
}


/////////////////////////////////////////////////////////////////////////////
// CCirc1Ctrl message handlers
