// dialog.cpp : implementation file
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
#include "dialog.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CCircleDialog dialog

IMPLEMENT_DYNAMIC(CCircleDialog, CDialog)

// The constructor for the dialog box first calls the base
// class CDialog constructor, passing it the resource i.d. of
// the dialog.  Later, CDialog::DoModal() then knows to create
// this dialog from the specified resource.

CCircleDialog::CCircleDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CCircleDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCircleDialog)
	m_strCaption = "Hello!";
	m_bBorder = FALSE;
	m_circle = NULL;
	m_nShape = 1;
	//}}AFX_DATA_INIT
}

BOOL CCircleDialog::OnInitDialog()
{
	CDialog::OnInitDialog();

	return TRUE;
}

// App Studio and ClassWizard generate the following code to
// the mapping between controls in the dialog box and
// data in the dialog C++ object.  The ClassWizard user specifies
// this mapping via an easy-to-use interface.   The {{AFX_DATA_MAP
// and }}AFX_DATA_MAP comments are used by ClassWizard to
// locate the data map and update it, as instructed by the user.
//

void CCircleDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCircleDialog)
	DDX_Check(pDX, IDC_BORDER, m_bBorder);
	DDX_Text(pDX, IDC_CAPTION, m_strCaption);
	DDV_MaxChars(pDX, m_strCaption, 30);
	DDX_VBControl(pDX, IDC_CIRCLE, m_circle);
	DDX_Radio(pDX, IDC_SHAPE_CIRCLE, m_nShape);
	//}}AFX_DATA_MAP
}


// When you use App Studio and ClassWizard to relate user interface commands
// to command handlers in your application, ClassWizard will generate
// the message map entries such as those below.  The {{AFX_MSG_MAP
// and }}AFX_MSG_MAP comments are used by ClassWizard to later locate
// the message map and update it, as instructed by the user.
//
// The first entry maps the command from the "Update" button in the
// dialog to the OnUpdateCircle() handler.  The next two entries map
// the two "Circle3" VB control-specific events, ClickIn and ClickOut,
// to the functions that handle these events.

BEGIN_MESSAGE_MAP(CCircleDialog, CDialog)
	//{{AFX_MSG_MAP(CCircleDialog)
	ON_BN_CLICKED(IDC_UPDATE_CIRCLE, OnUpdateCircle)
	ON_VBXEVENT(VBN_CLICKIN, IDC_CIRCLE, OnClickinCircle)
	ON_VBXEVENT(VBN_CLICKOUT, IDC_CIRCLE, OnClickoutCircle)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// This function illustrates handling of a VB control event.
// This event is triggered when the user clicks the mouse on the
// circle (ClickIn event), as opposed to the portion of the
// rectangular control outside of the circle.
//
void CCircleDialog::OnClickinCircle(UINT, int, CWnd*, LPVOID)
{
	// Display a message below the circle, so that we
	// can see that this event has indeed been handled.
	GetDlgItem(IDC_HIT_MESSAGE)->SetWindowText(
		"Last hit was INSIDE the circle");
}

/////////////////////////////////////////////////////////////////////////////
// Just like the above, except for the ClickOut event.
//
void CCircleDialog::OnClickoutCircle(UINT, int, CWnd*, LPVOID)
{
	GetDlgItem(IDC_HIT_MESSAGE)->SetWindowText(
		"Last hit was OUTSIDE the circle");
}

/////////////////////////////////////////////////////////////////////////////
// This command handler is called when the user hits the "Update"
// pushbutton in the dialog.  The purpose of this command is to
// illustrate the setting of properties in the control.  Note,
// getting properties is illustrated in CVBCircleView::OnPaint().
//
void CCircleDialog::OnUpdateCircle()
{
	// First, retrieve the values out of the edit, checkbox, and radio
	// buttons out of the dialog box in to the dialog C++ object's
	// member variables.
	UpdateData(TRUE);  // bSave=TRUE means "save from screen to dlg model"

	// Now, set those property values in the VB control.
	// The in-line function Circle() provides a pointer to the
	// specific instance of the control in the dialog box, that is,
	// the control that has the i.d. ID_CIRCLE.
	//
	m_circle->SetNumProperty("BorderStyle", m_bBorder);
	m_circle->SetNumProperty("CircleShape", m_nShape);
	m_circle->SetStrProperty("Caption", m_strCaption);
}
