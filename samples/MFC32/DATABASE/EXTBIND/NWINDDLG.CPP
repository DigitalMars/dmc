// NwindDlg.cpp : implementation file
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
#include "extbind.h"
#include "nwindDlg.h"
#include "modeldlg.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CNwindDialog dialog


CNwindDialog::CNwindDialog(CWnd* pParent)
	: CDialog(CNwindDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CNwindDialog)
	m_ShowDate = 0;
	m_ShowName = 0;
	//}}AFX_DATA_INIT
	ASSERT(m_pParent != NULL);

	m_pParent = pParent;
	m_nID = CNwindDialog::IDD;
}

void CNwindDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CNwindDialog)
	DDX_Radio(pDX, IDC_BIRTHDATE, m_ShowDate);
	DDX_Radio(pDX, IDC_FIRSTNAME, m_ShowName);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CNwindDialog, CDialog)
	//{{AFX_MSG_MAP(CNwindDialog)
	ON_BN_CLICKED(IDC_BIRTHDATE, OnChangeDate)
	ON_BN_CLICKED(IDC_FIRSTNAME, OnChangeName)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_HIREDATE, OnChangeDate)
	ON_BN_CLICKED(IDC_LASTNAME, OnChangeName)
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CNwindDialog message handlers

void CNwindDialog::OnOK() 
{
	CMsmask* pEdit = (CMsmask*)GetDlgItem(IDC_MASKEDBOX1);
	CListBox* pList = (CListBox*) (m_pParent->GetDlgItem(IDC_LIST));

	ASSERT(pList != NULL);
	ASSERT(pEdit != NULL);

	if (pList != NULL && pEdit != NULL)
	{
		CString str=pEdit->GetText();
		str=_T("NorthWind:") + str;
		pList->AddString(str);
	}
}

BOOL CNwindDialog::Create()
{
	return CDialog::Create(m_nID, m_pParent);
}

void CNwindDialog::OnCancel() 
{
	((CMainDlg*)m_pParent)->BoxDone();
	DestroyWindow();
}

void CNwindDialog::PostNcDestroy() 
{
	delete this;
}

BOOL CNwindDialog::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	RECT r;
	GetClientRect(&r);
	r.bottom=r.top+180;
	r.top+=40;  // top of the dialog (almost)
	r.left+=10;r.right-=10; // some margins to look better

	// get the cursor from IDC_RDCNWIND created in Pubs dialog
	// parent dialogRDC
	LPUNKNOWN pCursor=m_pParent->GetDlgItem(IDC_RDCNWIND)->GetDSCCursor(); 
	ASSERT(pCursor!=NULL);
	
	m_pGrid.Create(_T("Northwind Employees table"),WS_VISIBLE | WS_CHILD,
		r,this,IDC_GRIDNWIND);
	m_pGrid.SetDataSource(pCursor);
	m_pGrid.BindProperty(0x9,m_pParent->GetDlgItem(IDC_RDCNWIND));
	//	create the calendar control to display the Birth/Hire Date
	GetClientRect(&r);
	r.top=r.bottom-170; // the very bottom portion of the dialog
	m_date.Create(NULL,WS_VISIBLE | WS_CHILD,
		r,this,IDC_CALNWIND);
	BindCalendar();
	// bind the mased edit to IDC_RDCNWIND
	CWnd* pMasked=GetDlgItem(IDC_MASKEDBOX1);
	ASSERT(pMasked!=NULL);
	// bind to the correct field: HireDate or BirthDate
	pMasked->BindDefaultProperty(MASKDISPID_TEXT,VT_BSTR,
		m_ShowName==0?_T("FirstName"):_T("LastName"),
		m_pParent->GetDlgItem(IDC_RDCNWIND));

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

	
void CNwindDialog::BindCalendar() 
{
	// bind the calendar to the correct field: HireDate or BirthDate
	m_date.BindDefaultProperty(0xc,VT_VARIANT,m_ShowDate==0?_T("BirthDate"):_T("HireDate"),
		m_pParent->GetDlgItem(IDC_RDCNWIND)); // bind to the RDC in Pubs dialog	
}

void CNwindDialog::OnChangeDate() 
{
	if (!UpdateData())
		return;
	BindCalendar();
}


void CNwindDialog::OnChangeName() 
{
	if (!UpdateData())
		return;
	CWnd* p_maskedit;

	p_maskedit=GetDlgItem(IDC_MASKEDBOX1); // retrieve the OCX control with name
	ASSERT(p_maskedit!=NULL);
	if (p_maskedit==NULL)
		return;
	p_maskedit->BindDefaultProperty(MASKDISPID_TEXT,VT_BSTR,
		m_ShowName==0?_T("FirstName"):_T("LastName"),
		m_pParent->GetDlgItem(IDC_RDCNWIND));	
}

void CNwindDialog::OnDestroy() 
{
	CWnd* pMasked=GetDlgItem(IDC_MASKEDBOX1);
	ASSERT(pMasked!=NULL);
	pMasked->BindDefaultProperty(MASKDISPID_TEXT,VT_BSTR,NULL,NULL); // unbind the edit
	m_date.BindDefaultProperty(0xc,VT_VARIANT,NULL,NULL);	// unbind the calendar
	m_pGrid.SetDataSource(NULL);							// unbind the GRID
	m_pGrid.BindProperty(0x9,NULL);

	CDialog::OnDestroy();		
}

int CNwindDialog::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;
	// move the window down (do not cover the main dialog)
	MoveWindow(lpCreateStruct->x+50,lpCreateStruct->y+50,
		lpCreateStruct->cx,lpCreateStruct->cy,FALSE );
	
	return 0;
}

BEGIN_EVENTSINK_MAP(CNwindDialog, CDialog)
	ON_PROPNOTIFY_RANGE(CNwindDialog, IDC_MASKEDBOX1, IDC_MASKEDBOX1, MASKDISPID_TEXT, OnRequestEdit, OnChanged)
END_EVENTSINK_MAP()

BOOL CNwindDialog::OnRequestEdit(UINT nCtl, BOOL* pBool)
{	// this handler is never called in optimistic DataBinding architecture supplied by MFC
	// but is mapped as well as OnChanged to the events produced by DSC.
	// see ON_PROPNOTIFY_RANGE macro.
	// You might want to implement pessimistic (Access style) DataBinding
	// to make use of this function
	return TRUE;
}

BOOL CNwindDialog::OnChanged(UINT nCtl)
{
	// process the notification from nCtl control when data changed
	// in optimistic DataBinding
	return TRUE;
}

