// RDCFrm.cpp : implementation of the CRDCFrame class
//

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
#include "MDIBind.h"

#include "RDCFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRDCFrame

IMPLEMENT_DYNCREATE(CRDCFrame, CMDIChildWnd)

BEGIN_MESSAGE_MAP(CRDCFrame, CMDIChildWnd)
	//{{AFX_MSG_MAP(CRDCFrame)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRDCFrame construction/destruction

CRDCFrame::CRDCFrame()
{
	// TODO: add member initialization code here
	
}

CRDCFrame::~CRDCFrame()
{
}

BOOL CRDCFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	// the CREATESTRUCT cs
    cs.cy=50;cs.cx=160;

	return CMDIChildWnd::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CRDCFrame diagnostics

#ifdef _DEBUG
void CRDCFrame::AssertValid() const
{
	CMDIChildWnd::AssertValid();
}

void CRDCFrame::Dump(CDumpContext& dc) const
{
	CMDIChildWnd::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CRDCFrame message handlers
