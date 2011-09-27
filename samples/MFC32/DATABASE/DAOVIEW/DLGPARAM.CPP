// DlgParam.cpp : implementation file
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
#include "DaoView.h"
#include "DlgParam.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgParams dialog


CDlgParams::CDlgParams(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgParams::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgParams)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CDlgParams::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgParams)
	DDX_Control(pDX, IDC_LISTVIEW1, m_ctlList);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgParams, CDialog)
	//{{AFX_MSG_MAP(CDlgParams)
	ON_NOTIFY(LVN_ENDLABELEDIT, IDC_LISTVIEW1, OnEndlabeleditListview1)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgParams message handlers

void CDlgParams::SetInfo(CDaoDatabase* pDB,LPCTSTR lpszQueryDef)
{
	m_pDB = pDB;
	m_strQueryDef = lpszQueryDef;
}


BOOL CDlgParams::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	m_ctlList.AddColumn(_T("Value    "),0);
	m_ctlList.AddColumn(_T("Parameter Name"),1);

	CDaoQueryDef qd(m_pDB);
	CDaoParameterInfo paramInfo;
	int nParams;
	try{
		qd.Open(m_strQueryDef);
		nParams = qd.GetParameterCount();
		for (int i=0;i < nParams;i++){
			qd.GetParameterInfo(i,paramInfo);
			COleVariant var = qd.GetParamValue(i);
			m_ctlList.AddItem(i,0,CCrack::strVARIANT(var));
			m_ctlList.AddItem(i,1,paramInfo.m_strName);
		}
	}
	catch(CDaoException* e){
		// ... Do nothing.  Used to catch security violations opening tables.
		e->Delete();
	}
	qd.Close();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgParams::OnEndlabeleditListview1(NMHDR* pNMHDR, LRESULT* pResult) 
{
	LV_DISPINFO* pDispInfo = (LV_DISPINFO*)pNMHDR;

	m_ctlList.SetItemText(pDispInfo->item.iItem,pDispInfo->item.iSubItem,pDispInfo->item.pszText);
	CDaoQueryDef qd(m_pDB);
	try{
		qd.Open(m_strQueryDef);
		qd.SetParamValue(pDispInfo->item.iItem,COleVariant(pDispInfo->item.pszText, VT_BSTRT));
	}
	catch(CDaoException* e){
		// ... Do nothing.  Used to catch security violations opening tables.
		e->Delete();
	}
	qd.Close();
	*pResult = 0;
}
