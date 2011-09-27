// AddIndexDlg.cpp : implementation file--dialog to let user specify
// querydefs
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
#include "DAOTable.h"
#include "AddQyDlg.h"
#include "querydef.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAddQueryDlg dialog

// default constructor
CAddQueryDlg::CAddQueryDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CAddQueryDlg::IDD, pParent)
{
	// call centralized initialization function
	initializer();
}


// constructor that will generally be called to create the dialog
//
// IN: pDatabase--pointer to an open database object
// IN: strQueryName--name of query to add or view
// IN: pParent--pointer to parent of the dialog
//
CAddQueryDlg::CAddQueryDlg(CDaoDatabase *pDatabase, CString strQueryName, CWnd* pParent)
	: CDialog(CAddQueryDlg::IDD, pParent)
{
	// call centralized initialization function
	initializer();

	// initialize and set members to incoming parameters
	m_pDatabase = pDatabase;
	m_pQueryDef = NULL;
	m_QI.m_strName = strQueryName;
}

// initialize members of the class and the querydef info struct
void CAddQueryDlg::initializer()
{
	//{{AFX_DATA_INIT(CAddQueryDlg)
	//}}AFX_DATA_INIT

	// querydef info struct
	m_QI.m_strName = _T("");
	m_QI.m_bUpdatable = FALSE;
	m_QI.m_strSQL = _T("");
}


void CAddQueryDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAddQueryDlg)
	//}}AFX_DATA_MAP

	// since mapping directly to query info struct, must be
	// outside of wizard block
	DDX_Text(pDX, IDC_TABLE_NAME, m_QI.m_strName);
	DDX_Check(pDX, IDC_UPDATABLE, m_QI.m_bUpdatable);
	DDX_Text(pDX, IDC_SQL, m_QI.m_strSQL);
}


BEGIN_MESSAGE_MAP(CAddQueryDlg, CDialog)
	//{{AFX_MSG_MAP(CAddQueryDlg)
	ON_BN_CLICKED(IDC_ADD_QUERY_DEF, OnAddQueryDef)
	ON_BN_CLICKED(IDC_DELETE_QUERYDEF, OnDeleteQuerydef)
	ON_BN_CLICKED(IDOK, OnDone)
	ON_BN_CLICKED(IDC_MODIFY_QUERY_DEF, OnModifyQueryDef)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAddQueryDlg message handlers

BOOL CAddQueryDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// user either specified new query or existing query
	if(IsExistentQuery(m_pDatabase, m_QI.m_strName))
	{
		// user specified existing query, so populate
		// dialog with information from the query
		//
		// get querydef info by name
		getQueryInfo(m_pDatabase, &m_QI, -1, FALSE);

		// now try to open the querydef
		openQueryDef(m_pDatabase, &m_pQueryDef, m_QI.m_strName);
 	
		UpdateData(FALSE);
	}

	// set focus to the SQL edit box
	CEdit *pEdit = (CEdit *)GetDlgItem(IDC_SQL);
	pEdit->SetFocus();

	return FALSE;  // return TRUE unless you set the focus to a control
}

// user selected to add the specified querydef to the collection of
// querydefs
void CAddQueryDlg::OnAddQueryDef() 
{
	// get data from the dialog
	UpdateData(TRUE);

	// cleanup any existing querydef
	if (m_pQueryDef != NULL)
		delete m_pQueryDef;

	// construct the querydef
	m_pQueryDef = new CDaoQueryDef(m_pDatabase);

	// allcation failed!
	if (NULL == m_pQueryDef)
	{
		AfxMessageBox(_T("Failed to allocate querydef!"));
		return;
	}

	// create the querydef and append it to the collection
	createSetAndSaveNewQueryDef (m_pDatabase, &m_pQueryDef, &m_QI);

	// get the information on the new query and display	(-1 indicates use
	// name in m_QI to find querydef
	getQueryInfo(m_pDatabase, &m_QI, -1, FALSE);
	UpdateData(FALSE);
}

// user selected to delete the current query--prompt for acceptance
void CAddQueryDlg::OnDeleteQuerydef() 
{
	// get values from control 
	UpdateData(TRUE);

	// can only delete existing queries
	if (IsExistentQuery(m_pDatabase, m_QI.m_strName))
	{
		// is user sure?
		if (IDYES == AfxMessageBox(_T("Delete current query?"), MB_YESNO))
		{
			// only react if field is deleted!
			if (deleteQuery(m_pDatabase, m_QI.m_strName))
			{
				// set the initial state
				initializer();

				// update the dialog controls to erase deleted info
				UpdateData(FALSE);
			}
		}
	}

	// since query has been delete, cleanup and exit the dialog
	delete m_pQueryDef;

	EndDialog(0);	
}

// user is done viewing or specifying the query--prompt so new query
// that hasn't been added will not be lost--based solely on SQL string
// having changed
void CAddQueryDlg::OnDone() 
{
	// by default, simply exit
	int retCode = IDYES;

	// if user has entered a new sql string, then warn them they will lose
	// it if it is not explicitly added
	//
	// compare the current sql string with the original
	CString sql;
	CEdit *pEdit = (CEdit *)GetDlgItem(IDC_SQL);
	pEdit->GetWindowText(sql);
	// if sql in edit and in querydef info differ, then prompt
	if (sql != m_QI.m_strSQL)
	{
		CString prompt;

		// what to say in the prompt depends on if this is an existing
		// query or a new one
		if(IsExistentQuery(m_pDatabase, m_QI.m_strName))
		{
			// existing queries -- will lose any modifications
			prompt = _T("SQL modifications will be ignored unless you select Modify.  Continue anyway?");
		}
		else
		{
			// new queries -- will lose unless added
			prompt = _T("New query will be ignored unless you select Add.  Continue anyway?");
		}

		retCode = AfxMessageBox(prompt,	MB_YESNO);	
	}
	// either there never was a change in the sql or the user has
	// chosen not to record the change
	if (retCode == IDYES)
	{
		// cleanup
		delete m_pQueryDef;

		// end the dialog
		CDialog::EndDialog(0);
	}
}

// even after appending to a collection, querydef properties are updatable
// user has selected to update
void CAddQueryDlg::OnModifyQueryDef() 
{
	// cache old name (i.e. user may have modified name and we need to use
	// name to determine if this a new or existing query)
	CString oldName = m_QI.m_strName;

	// get new data from dialog	(possibly including new name)
	UpdateData(TRUE);

	// only proceed if this is an existing query (use old name to determine this)
	if (!IsExistentQuery(m_pDatabase, oldName))
	{
		AfxMessageBox(_T("Can not modify a query that has not been added to the collection."));

		// return since can't modify non-existent query
		return;
	}

	// attempt to modify the querydef with the new information
	// only if the modification succeeds do we allow the name to change,
	// else reset it to the old name to avoid "losing" this query
	// by setting its name to an invalid value
	if (!modifyQueryDef (m_pDatabase, m_pQueryDef, &m_QI))
		m_QI.m_strName = oldName;

}
