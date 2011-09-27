// addfield.cpp : implementation file
//
// This is a part of the Microsoft Foundation Classes C++ library.
// Copyright (C) 1992-1993 Microsoft Corporation
// All rights reserved.
//
// This source code is only intended as a supplement to the
// Microsoft Foundation Classes Reference and Microsoft
// QuickHelp and/or WinHelp documentation provided with the library.
// See these sources for detailed information regarding the
// Microsoft Foundation Classes product.


#include "stdafx.h"
#include "enroll.h"
#include "sectset.h"
#include "addfield.h"
#include "..\catalog\columnst.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAddField dialog

CAddField::CAddField(CWnd* pParent /*=NULL*/)
	: CDialog(CAddField::IDD, pParent)
{
	//{{AFX_DATA_INIT(CAddField)
	m_strField = "";
	//}}AFX_DATA_INIT
}

void CAddField::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAddField)
	DDX_Control(pDX, IDC_LIST1, m_lbFields);
	DDX_LBString(pDX, IDC_LIST1, m_strField);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAddField, CDialog)
	//{{AFX_MSG_MAP(CAddField)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CAddField message handlers

BOOL CAddField::OnInitDialog()
{
	CDialog::OnInitDialog();

	CColumns* pcols = new CColumns(m_pSet->m_pDatabase);
	pcols->OnSetOptions(pcols->m_hstmt);

	RETCODE nRetCode;
	AFX_SQL_ASYNC(pcols, ::SQLColumns(pcols->m_hstmt,
		(UCHAR *)NULL, SQL_NTS, (UCHAR *)NULL, SQL_NTS,
		(UCHAR *)(const char *)m_pSet->GetTableName(), SQL_NTS,
		(UCHAR *)NULL, SQL_NTS));
	if (!pcols->Check(nRetCode))
	{
		AfxMessageBox("SQLColumns failed\n", MB_OK | MB_ICONEXCLAMATION);
		return TRUE;
	}

	// Position on first record
	pcols->MoveNext();

	UINT ccols = 0;
	while (!pcols->IsEOF())
	{
		CFieldInfo Info;

		if ((pcols->m_nDataType == SQL_CHAR || pcols->m_nDataType == SQL_VARCHAR) &&
			!m_pSet->FindField(pcols->m_strColumnName))
		{
			ccols++;
			m_lbFields.AddString(pcols->m_strColumnName);
		}
		pcols->MoveNext();
	}

	m_lbFields.SetSel(0);

	if (ccols == 0)
		AfxMessageBox("No additional columns available to add.\n", MB_OK | MB_ICONEXCLAMATION);

	delete pcols;

	m_lbFields.SetCurSel(0);

	return TRUE;  // return TRUE  unless you set the focus to a control
}
