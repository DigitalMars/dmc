// frame.cpp : implementation of the CVBCircleFrame class
//
// This is a part of the Microsoft Foundation Classes C++ library.
// Copyright (C) 1992 Microsoft Corporation
// All rights reserved.
//
// This source code is only intended as a supplement to the
// Microsoft Foundation Classes Reference and Microsoft
// QuickHelp and/or WinHelp documentation provided with the library.
// See these sources for detailed information regarding the
// Microsoft Foundation Classes product.



#include "stdafx.h"
#include "vbcircle.h"
#include "frame.h"

#include "dialog.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CVBCircleFrame

BEGIN_MESSAGE_MAP(CVBCircleFrame, CFrameWnd)
	//{{AFX_MSG_MAP(CVBCircleFrame)
	ON_COMMAND(ID_EDIT_CIRCLE, OnEditCircle)
	ON_WM_CREATE()
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CVBCircleFrame construction/destruction

CVBCircleFrame::CVBCircleFrame()
{
}

CVBCircleFrame::~CVBCircleFrame()
{
}

/////////////////////////////////////////////////////////////////////////////

int CVBCircleFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	// Call base class OnCreate() to create the main frame window.
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	// Create the VB control, referring to its name and .VBX file.
	m_circle.Create("circ3.vbx;Circ3;text from Create( ) call",
			// The 'window text'
			// pararmeter for a VB control must be specified according
			// to the following syntax:
			// <vbx_file_name>;<vb_control_name>;<window text>
		WS_CHILD | WS_VISIBLE,
		CRect(10, 30, 210, 180), // initial position of the circle control
		this,
		1);  // arbitrary child window i.d., not needed in this application

	return 0;       // create ok
}


/////////////////////////////////////////////////////////////////////////////
void CVBCircleFrame::OnPaint()
{
	// This demonstrates getting a property from a VB control.

	CPaintDC dc(this);
	CString strCaption = m_circle.GetStrProperty("Caption");
	dc.TextOut(10, 10, strCaption);
}

void CVBCircleFrame::OnEditCircle()
{
	// Create the dialog object.  This doesn't actually put up
	// the dialog; it just creates the C++ object for the dialog.
	CCircleDialog  dlg;

	// DoModal() actually puts up the dialog box on the screen.
	// It will stay there until the user hits the OK button
	// or uses the system menu Close item.  Usually an application
	// is interested in the return value from DoModal(), which
	// indicates whether the user hit the OK or Cancel button.
	if (dlg.DoModal() != IDOK)
		return;

	// Use the Circle3 properties entered by the user to
	// update the circle in the main window.

	m_circle.SetNumProperty("BorderStyle", dlg.m_bBorder);
	m_circle.SetNumProperty("CircleShape", dlg.m_nShape);
	m_circle.SetStrProperty("Caption", dlg.m_strCaption);

	Invalidate();  // to force TextOut() in the main window
}
