// DaoViewDoc.cpp : implementation of the CDaoViewDoc class
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
#include "OptionsD.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDaoViewDoc

IMPLEMENT_DYNCREATE(CDaoViewDoc, CDocument)

BEGIN_MESSAGE_MAP(CDaoViewDoc, CDocument)
	//{{AFX_MSG_MAP(CDaoViewDoc)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDaoViewDoc construction/destruction

CDaoViewDoc::CDaoViewDoc()
{
	m_pDB = NULL;
	m_pTreeView = NULL;
	m_pListView = NULL;
}

CDaoViewDoc::~CDaoViewDoc()
{
	if(m_pDB)
	{
		if(m_pDB->IsOpen())
			m_pDB->Close();
		delete m_pDB;
	}
}

BOOL CDaoViewDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CDaoViewDoc serialization

void CDaoViewDoc::Serialize(CArchive& ar)
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
// CDaoViewDoc diagnostics

#ifdef _DEBUG
void CDaoViewDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CDaoViewDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CDaoViewDoc commands

BOOL CDaoViewDoc::OnOpenDocument(LPCTSTR lpszPathName) 
{
	if (!CDocument::OnOpenDocument(lpszPathName))
		return FALSE;
	
	CDaoDatabase* tmpDB = new CDaoDatabase;

	// Attempt to open the new database before replacing our ptr
	try
	{
		tmpDB->Open(lpszPathName);
	}
	catch (CDaoException* e)
	{
		DisplayDaoException(e);
		delete tmpDB;
		e->Delete();
		return FALSE;
	}


	if (m_pDB)
	{
		if (m_pDB->IsOpen())
			m_pDB->Close();
		delete m_pDB;
	}

	m_pDB = tmpDB;
	m_pTreeView->m_pDB = m_pDB;
	m_pListView->m_pDB = m_pDB;

	RefreshViews();

	return TRUE;
}


void CDaoViewDoc::RefreshViews() 
{
	if (m_pDB)
	{
		if (m_pListView)
			m_pListView->EraseList();

		if (m_pTreeView)
			m_pTreeView->PopulateTree();
	}
}

