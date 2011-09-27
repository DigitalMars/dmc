// datadlg.cpp : implementation file
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
#include "dbfetch.h"
#include "datadlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDataDialog dialog


CDataDialog::CDataDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CDataDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDataDialog)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_prs = NULL;
}


void CDataDialog::SetRecordset(CDynamicBulkSet* prs)
{
	ASSERT(prs != NULL);

	m_prs = prs;
}

int CDataDialog::DoModal()
{
	ASSERT(m_prs != NULL);

	return CDialog::DoModal();
}

BOOL CDataDialog::OnInitDialog()
{
	BOOL bReturn = CDialog::OnInitDialog();

	AddColumns();
	FillData();

	return bReturn;
}

void CDataDialog::AddColumns()
{
	ASSERT(m_prs->IsOpen());

	// declare a CODBCFieldInfo structure and get a count
	// of the number of columns in the database
	CODBCFieldInfo info;
	int nColumns = m_prs->GetODBCFieldCount();

	// for each column, retrieve the field name and 
	// insert the field name in the header
	for (int nNum = 0; nNum < nColumns; nNum++)
	{
		m_prs->GetODBCFieldInfo(nNum, info);
		if (m_listData.InsertColumn(nNum, info.m_strName,
			LVCFMT_LEFT, 80) != nNum)
		{
			ASSERT(FALSE);
			return;
		}
	}
}

void CDataDialog::FillData()
{
	ASSERT(m_prs->IsOpen());

	// Make sure there are no items
	m_listData.DeleteAllItems();

	// Validate that there is data
	if (m_prs->IsEOF() && m_prs->IsBOF())
	{
		// Disable all the controls
		GetDlgItem(IDC_FIRST)->EnableWindow(FALSE);
		GetDlgItem(IDC_LAST)->EnableWindow(FALSE);
		GetDlgItem(IDC_NEXT)->EnableWindow(FALSE);
		GetDlgItem(IDC_PREV)->EnableWindow(FALSE);

		// Put up a warning dialog and return
		CString strError;
		strError.LoadString(IDS_ERROR_NODATA);
		AfxMessageBox(strError);
		return;
	}
	else
	{
		// Enable all the controls
		GetDlgItem(IDC_FIRST)->EnableWindow(TRUE);
		GetDlgItem(IDC_LAST)->EnableWindow(TRUE);
		GetDlgItem(IDC_NEXT)->EnableWindow(TRUE);
		GetDlgItem(IDC_PREV)->EnableWindow(TRUE);
	}


	long* rgLength;
	LPSTR rgData;

	// Need to use this to convert LPSTR to UNICODE
	CString strData;

	int nFields = m_prs->GetODBCFieldCount();
	int nRowsFetched = m_prs->GetRowsFetched();

	// Display 1 rowset of data by field
	for (int nField = 0; nField < nFields; nField++)
	{
		// set up the correct data and length arrays
		rgData = (LPSTR)m_prs->m_ppvData[nField];
		rgLength = (long*)m_prs->m_ppvLengths[nField];

		for (int nRow = 0; nRow < nRowsFetched; nRow++)
		{
			int nStatus = m_prs->GetRowStatus(nRow + 1);

			// Get the string to display
			if (nStatus == SQL_ROW_DELETED)
				strData = _T("<DELETED>");
			else if (nStatus == SQL_ROW_NOROW)
				// Shouldn't get this since rows fetched is checked
				strData = _T("<NO_ROW>");
			else if (rgLength[nRow] == SQL_NULL_DATA)
				strData = _T("<NULL>");
			else
				strData = &rgData[nRow * MAX_TEXT_LEN];

			// Set the string (if first column must add)
			if (nField == 0)
				m_listData.InsertItem(nRow, strData);
			else
			{
				m_listData.SetItem(nRow, nField, LVIF_TEXT,
					strData, -1, 0, 0, 0);
			}
		}
	}
}

void CDataDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDataDialog)
	DDX_Control(pDX, IDC_DATALIST, m_listData);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDataDialog, CDialog)
	//{{AFX_MSG_MAP(CDataDialog)
	ON_BN_CLICKED(IDC_FIRST, OnFirst)
	ON_BN_CLICKED(IDC_LAST, OnLast)
	ON_BN_CLICKED(IDC_NEXT, OnNext)
	ON_BN_CLICKED(IDC_PREV, OnPrev)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDataDialog message handlers

void CDataDialog::OnFirst() 
{
	ASSERT(m_prs->IsOpen());

	m_prs->MoveFirst();

	FillData();

	// Disable the correct controls
	GetDlgItem(IDC_PREV)->EnableWindow(FALSE);
}

void CDataDialog::OnLast() 
{
	ASSERT(m_prs->IsOpen());

	m_prs->MoveLast();
	FillData();

	// Disable the correct controls
	GetDlgItem(IDC_NEXT)->EnableWindow(FALSE);
}

void CDataDialog::OnNext() 
{
	ASSERT(m_prs->IsOpen());

	m_prs->MoveNext();

	if (m_prs->IsEOF())
	{
		// Disable the correct controls
		GetDlgItem(IDC_NEXT)->EnableWindow(FALSE);

		CString strError;
		strError.LoadString(IDS_ERROR_EOF);
		AfxMessageBox(strError);
	}
	else
		FillData();
}

void CDataDialog::OnPrev() 
{
	ASSERT(m_prs->IsOpen());

	m_prs->MovePrev();

	if (m_prs->IsBOF())
	{
		// Disable the correct controls
		GetDlgItem(IDC_PREV)->EnableWindow(FALSE);

		CString strError;
		strError.LoadString(IDS_ERROR_BOF);
		AfxMessageBox(strError);
	}
	else
		FillData();
}
