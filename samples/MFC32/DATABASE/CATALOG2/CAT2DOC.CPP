// catalog2Doc.cpp : implementation of the CCatalog2Doc class
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
#include "catalog2.h"

#include "catsets.h"
#include "cat2Doc.h"
#include "TabPage.h"
#include "ColPage.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCatalog2Doc

IMPLEMENT_DYNCREATE(CCatalog2Doc, CDocument)

BEGIN_MESSAGE_MAP(CCatalog2Doc, CDocument)
	//{{AFX_MSG_MAP(CCatalog2Doc)
	ON_COMMAND(ID_VIEW_SETTINGS, OnViewSettings)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCatalog2Doc construction/destruction

CCatalog2Doc::CCatalog2Doc()
{
	// initialize member recordset pointers
	m_pTableset = 0;
	m_pColumnset = 0;
}

CCatalog2Doc::~CCatalog2Doc()
{
}

BOOL CCatalog2Doc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// initialize current view level
	m_nLevel = levelNone;

	// initialize table settings
	m_bSystemTables	= GetProfileValue(_T("TableSettings"),_T("SystemTables"));
	m_bViews		= GetProfileValue(_T("TableSettings"),_T("Views"));
	m_bSynonyms		= GetProfileValue(_T("TableSettings"),_T("SystemTables"));

	// initialize column info settings
	m_bLength		= GetProfileValue(_T("ColumnInfoSettings"),_T("Length"));
	m_bPrecision	= GetProfileValue(_T("ColumnInfoSettings"),_T("Precision"));
	m_bNullability	= GetProfileValue(_T("ColumnInfoSettings"),_T("Nullability"));

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CCatalog2Doc serialization

void CCatalog2Doc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}

/////////////////////////////////////////////////////////////////////////////
// CCatalog2Doc diagnostics

#ifdef _DEBUG
void CCatalog2Doc::AssertValid() const
{
	CDocument::AssertValid();
}

void CCatalog2Doc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CCatalog2Doc commands

int CCatalog2Doc::GetProfileValue(LPCTSTR lpszSection,LPCTSTR lpszItem)
{
	int	nValue = AfxGetApp()->GetProfileInt(lpszSection,lpszItem,-1);
	if (nValue == -1)
	{
		nValue = 0;
		AfxGetApp()->WriteProfileInt(lpszSection,lpszItem,nValue);
	}
	return nValue;
}

void CCatalog2Doc::SetLevel(Level nLevel)
{
	m_nLevel = nLevel;
	UpdateAllViews(NULL);
}

CString CCatalog2Doc::GetDSN()
{
	if (!m_Database.IsOpen())
		return _T("[No Data Source Selected]");

	// pull DSN from database connect string
	CString	string = m_Database.GetConnect();
	string = string.Right(string.GetLength() - (string.Find(_T("DSN=")) + 4));
	string = string.Left(string.Find(_T(";")));
	return string;
}

BOOL CCatalog2Doc::OnOpenDocument()
{
	// close and delete any open recordsets
	if (m_pTableset)
	{
		if (m_pTableset->IsOpen())
			m_pTableset->Close();
		delete m_pTableset;
		m_pTableset = 0;
	}
	if (m_pColumnset)
	{
		if (m_pColumnset->IsOpen())
			m_pColumnset->Close();
		delete m_pColumnset;
		m_pColumnset = 0;
	}

	// close the database
	if (m_Database.IsOpen())
		m_Database.Close();

	// open the database
	if (m_Database.Open(NULL,FALSE,TRUE))
	{
		if (FetchTableInfo())
			return TRUE;
	}
	return FALSE;
}

void CCatalog2Doc::OnCloseDocument()
{
	if (m_pTableset)
	{
		if (m_pTableset->IsOpen())
			m_pTableset->Close();
		delete m_pTableset;
		m_pTableset = 0;
	}
	if (m_pColumnset)
	{
		if (m_pColumnset->IsOpen())
			m_pColumnset->Close();
		delete m_pColumnset;
		m_pColumnset = 0;
	}

	if (m_Database.IsOpen())
		m_Database.Close();

	CDocument::OnCloseDocument();
}

void CCatalog2Doc::FetchColumnInfo(LPCSTR lpszName)
{
	if (m_pColumnset)
	{
		if (m_pColumnset->IsOpen())
			m_pColumnset->Close();
		delete m_pColumnset;
		m_pColumnset = 0;
	}
	m_pColumnset = new CColumns(&m_Database);
	m_pColumnset->Open(NULL,NULL,lpszName,NULL,CRecordset::snapshot);
}

BOOL CCatalog2Doc::FetchTableInfo()
{
	m_pTableset = new CTables(&m_Database);
	
	// Must use char array for ODBC interface
	// (can simply hard code max size)
	char lpszType[64];
	
	strcpy(lpszType, "'TABLE'");
	if (m_bViews)
		strcat(lpszType, ",'VIEW'");
	if (m_bSystemTables)
		strcat(lpszType, ",'SYSTEM TABLE'");
	if (m_bSynonyms)
		strcat(lpszType, ",'ALIAS','SYNONYM'");
	
	if (!m_pTableset->Open(NULL,NULL,NULL,lpszType,CRecordset::snapshot))
	{
		delete m_pTableset;
		m_pTableset = NULL;
		m_Database.Close();
		return FALSE;
	}
	return TRUE;
}

void CCatalog2Doc::OnViewSettings()
{
	CPropertySheet	sheet(_T("Settings"));
	CTablePage		pageTable;
	CColumnPage		pageColumn;

	// initialize and add table settings page
	sheet.AddPage(&pageTable);
	pageTable.m_bSystemTables = m_bSystemTables;
	pageTable.m_bViews = m_bViews;
	pageTable.m_bSynonyms = m_bSynonyms;

	// initialize and add column info settings page
	sheet.AddPage(&pageColumn);
	pageColumn.m_bLength = m_bLength;
	pageColumn.m_bPrecision = m_bPrecision;
	pageColumn.m_bNullability = m_bNullability;

	// execte property sheet and update settings
	if (sheet.DoModal() == IDOK) {
		BOOL	bTableModified = FALSE;
		BOOL	bColumnModified = FALSE;

		if (m_bSystemTables != pageTable.m_bSystemTables)
		{
			m_bSystemTables = pageTable.m_bSystemTables;
			AfxGetApp()->WriteProfileInt(_T("TableSettings"),
				_T("SystemTables"),m_bSystemTables);
			bTableModified = TRUE;
		}
		if (m_bViews != pageTable.m_bViews)
		{
			m_bViews = pageTable.m_bViews;
			AfxGetApp()->WriteProfileInt(_T("TableSettings"),
				_T("Views"),m_bViews);
			bTableModified = TRUE;
		}
		if (m_bSynonyms != pageTable.m_bSynonyms)
		{
			m_bSynonyms = pageTable.m_bSynonyms;
			AfxGetApp()->WriteProfileInt(_T("TableSettings"),
				_T("Synonyms"),m_bSynonyms);
			bTableModified = TRUE;
		}
		if (m_bLength != pageColumn.m_bLength)
		{
			m_bLength = pageColumn.m_bLength;
			AfxGetApp()->WriteProfileInt(_T("ColumnInfoSettings"),
				_T("Length"),m_bLength);
			bColumnModified = TRUE;
		}
		if (m_bPrecision != pageColumn.m_bPrecision)
		{
			m_bPrecision = pageColumn.m_bPrecision;
			AfxGetApp()->WriteProfileInt(_T("ColumnInfoSettings"),
				_T("Precision"),m_bPrecision);
			bColumnModified = TRUE;
		}
		if (m_bNullability != pageColumn.m_bNullability)
		{
			m_bNullability = pageColumn.m_bNullability;
			AfxGetApp()->WriteProfileInt(_T("ColumnInfoSettings"),
				_T("Nullability"),m_bNullability);
			bColumnModified = TRUE;
		}

		// check for table modification first
		if (bTableModified)
		{
			// close and delete any open recordsets
			if (m_pTableset)
			{
				if (m_pTableset->IsOpen())
					m_pTableset->Close();
				delete m_pTableset;
				m_pTableset = 0;
			}
			if (m_pColumnset)
			{
				if (m_pColumnset->IsOpen())
					m_pColumnset->Close();
				delete m_pColumnset;
				m_pColumnset = 0;
			}

			// refresh table data
			FetchTableInfo();
			SetLevel(levelTable);
			UpdateAllViews(NULL);
		}

		// if table settings not modified, check column info
		else if (bColumnModified)
			UpdateAllViews(NULL);
	}
}
