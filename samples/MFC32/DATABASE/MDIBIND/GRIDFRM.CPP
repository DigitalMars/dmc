// GridFrm.cpp : implementation of the CGridFrame class
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

#include "GridFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CGridFrame

IMPLEMENT_DYNCREATE(CGridFrame, CMDIChildWnd)

BEGIN_MESSAGE_MAP(CGridFrame, CMDIChildWnd)
	//{{AFX_MSG_MAP(CGridFrame)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGridFrame construction/destruction

CGridFrame::CGridFrame()
{
	// TODO: add member initialization code here
	
}

CGridFrame::~CGridFrame()
{
}

BOOL CGridFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs
    cs.cy=200;cs.cx=300;

	return CMDIChildWnd::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CGridFrame diagnostics

#ifdef _DEBUG
void CGridFrame::AssertValid() const
{
	CMDIChildWnd::AssertValid();
}

void CGridFrame::Dump(CDumpContext& dc) const
{
	CMDIChildWnd::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CGridFrame message handlers
