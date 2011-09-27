// RDCDlg.cpp : implementation file
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
#include "mcsdsc.h"
#include "MDIDoc.h"
#include "mainfrm.h"
#include "RDCFrm.h"
#include "RDCDlg.h"
#include "rdocols.h"
#include "rdocol.h"
#include "rdoReslt.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// COpenControlsDlg dialog


COpenControlsDlg::COpenControlsDlg(const CMDIBindDoc* pDocument,CWnd* pParent /*=NULL*/)
	: CDialog(COpenControlsDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(COpenControlsDlg)
	m_ColName = pDocument->m_boundCol;
	//}}AFX_DATA_INIT
	if(pDocument->m_pRDC!=NULL)
		m_pRDCView=(CRDCView*) pDocument->m_pRDC->GetParent(); 
		//you could change m_pRDCView to ponter to RDC itself
	else
		m_pRDCView=NULL;
	m_CurrentSel=-1;
}


void COpenControlsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(COpenControlsDlg)
	DDX_CBString(pDX, IDC_COLUMN, m_ColName);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(COpenControlsDlg, CDialog)
	//{{AFX_MSG_MAP(COpenControlsDlg)
	ON_CBN_DROPDOWN(IDC_COLUMN, OnDropdownColumns)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COpenControlsDlg message handlers

BOOL COpenControlsDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
   	CListBox* pListBox = (CListBox*) GetDlgItem(IDC_RDCLIST);

	CMainFrame* pMainFrame = (CMainFrame*) AfxGetApp()->m_pMainWnd;
	ASSERT(pMainFrame->IsKindOf(RUNTIME_CLASS(CMainFrame)));
	// iterate through all MDI windows
	CMDIChildWnd* pMDIChild = pMainFrame->MDIGetActive();
	if(pMDIChild != NULL)
		pMDIChild = (CRDCFrame*) pMDIChild->GetWindow(GW_HWNDLAST);
	while (pMDIChild != NULL)
	{
		CString strTitle;
		int nIndex;
		if(pMDIChild->IsKindOf(RUNTIME_CLASS(CRDCFrame)))
		{ // only RDC windows
			CRDCView* pRDCView = (CRDCView*)pMDIChild->GetActiveView();
			ASSERT(pRDCView->IsKindOf(RUNTIME_CLASS(CRDCView)));
			// get the window's title
			pMDIChild->GetWindowText(strTitle);
			strTitle=strTitle+_T(": ")+pRDCView->m_pRDCCtl->GetCaption();
			// add the window to the list
			nIndex = pListBox->AddString(strTitle);
			
			if(m_pRDCView!=NULL && 				
				pRDCView==m_pRDCView) // you'd compare RDC themselves as well
			{	// select nIndex item only if 
				// m_pRDCCtl==the control currenty assigned
				pListBox->SetCurSel(nIndex);
			}
			// store a pointer to the frame
			pListBox->SetItemData(nIndex, (DWORD)pMDIChild);
		}
		pMDIChild = (CRDCFrame*) pMDIChild->GetWindow(GW_HWNDPREV);
	} // while

	int cElements = pListBox->GetCount();
	pListBox->SetFocus();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void COpenControlsDlg::OnOK() 
{
   	CListBox* pListBox = (CListBox*) GetDlgItem(IDC_RDCLIST);
	int cElements = pListBox->GetCount();
	int nSelection= pListBox->GetCurSel(); 	// get the selected item

	if(cElements > 0 && nSelection >= 0) 
	{	// can only select a window if at least one item is selected
		// get the pointer to the frame
		m_pMDIChild = (CRDCFrame*) pListBox->GetItemData(nSelection);
		ASSERT(m_pMDIChild->IsKindOf(RUNTIME_CLASS(CRDCFrame)));
		// now use ActiveView() rather than MDIActivate()
		// get a pointer to the active view in the frame
		m_pRDCView = (CRDCView*) m_pMDIChild->GetActiveView();
		ASSERT(m_pRDCView->IsKindOf(RUNTIME_CLASS(CRDCView)));
	} //if
	
	CDialog::OnOK();
}

/////////////////////////////////////////////////////////////////////////////
// Refill the Combo List with columns names from currently selected RDC
void COpenControlsDlg::OnDropdownColumns() 
{
   	CListBox* pListBox = (CListBox*) GetDlgItem(IDC_RDCLIST);
   	CComboBox* pComboBox = (CComboBox*) GetDlgItem(IDC_COLUMN);
	
	int nSelection=pListBox->GetCurSel();
	if(nSelection<0 || nSelection==m_CurrentSel)
		return; // nothing to change in Combo box
	m_CurrentSel=nSelection;
	pComboBox->ResetContent( ); // clear old columns
  
   	m_pMDIChild = (CRDCFrame*) pListBox->GetItemData(nSelection);
	ASSERT(m_pMDIChild->IsKindOf(RUNTIME_CLASS(CRDCFrame)));
	// now use ActiveView() rather than MDIActivate()
	// get a pointer to the active view in the frame
	m_pRDCView = (CRDCView*) m_pMDIChild->GetActiveView();
	ASSERT(m_pRDCView->IsKindOf(RUNTIME_CLASS(CRDCView)));
		
	CMcsDsc* pRDC=m_pRDCView->m_pRDCCtl;
	ASSERT(pRDC!=NULL);
	
	CrdoResultset pResult;	// resultset from RDC
	CrdoColumns pColumns;	// columns from resultset

	pResult=pRDC->GetResultset();
	if(pResult!=NULL && (pColumns=pResult.GetRdoColumns())!=NULL)
	{
		long ncol=pColumns.GetCount();
		CrdoColumn pCol; // element from columns collection
		for(long icol=0; icol<ncol; icol++)
		{
			pCol=pColumns.GetItem(COleVariant(icol)); //get i-column 
		 	pComboBox->AddString(pCol.GetName());//add its name to the list
		}
		pComboBox->SetCurSel(0); // no old element valid if new RDC
	}
}
