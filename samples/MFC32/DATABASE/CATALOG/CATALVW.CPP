// catalvw.cpp : implementation of the CCatalogView class
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
#include "catalog.h"

#include "tableset.h"
#include "columnst.h"
#include "cataldoc.h"
#include "catalvw.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCatalogView

IMPLEMENT_DYNCREATE(CCatalogView, CRecordView)

BEGIN_MESSAGE_MAP(CCatalogView, CRecordView)
	//{{AFX_MSG_MAP(CCatalogView)
	ON_UPDATE_COMMAND_UI(ID_APP_EXIT, OnUpdateAppExit)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCatalogView construction/destruction

CCatalogView::CCatalogView()
	: CRecordView(CCatalogView::IDD)
{
	//{{AFX_DATA_INIT(CCatalogView)
	m_pSet = NULL;
	//}}AFX_DATA_INIT
	// TODO: add construction code here
}

CCatalogView::~CCatalogView()
{
}

void CCatalogView::DoDataExchange(CDataExchange* pDX)
{
	CRecordView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCatalogView)
	DDX_Control(pDX, IDC_COLUMNS, m_lbColumns);
	DDX_FieldText(pDX, IDC_NAME, m_pSet->m_strName, m_pSet);
	DDX_FieldText(pDX, IDC_OWNER, m_pSet->m_strOwner, m_pSet);
	DDX_FieldText(pDX, IDC_TYPE, m_pSet->m_strType, m_pSet);
	DDX_FieldText(pDX, IDC_REMARK, m_pSet->m_strRemarks, m_pSet);
	//}}AFX_DATA_MAP
}

void CCatalogView::OnInitialUpdate()
{
	m_pSet = &GetDocument()->m_tableSet;
	m_pSet->m_strTypeParam = "'TABLE'";
	CRecordView::OnInitialUpdate();

	try
	{
		if (m_pSet->IsOpen())
		{
			// Flush pending results for drivers that don't
			// support multiple hstmt's
			m_pSet->MoveLast();
			m_pSet->MoveFirst();
			FillColumnsListBox();
		}
		else
			m_pSet->ThrowDBException(AFX_SQL_ERROR_CONNECT_FAIL);
	}
	
	catch( CDBException* e )
	{
		e->ReportError(MB_ICONEXCLAMATION);
	}
}


#ifdef _DEBUG
void CCatalogView::AssertValid() const
{
	CRecordView::AssertValid();
}

void CCatalogView::Dump(CDumpContext& dc) const
{
	CRecordView::Dump(dc);
}

CCatalogDoc* CCatalogView::GetDocument() // non-debug version is inline
{
	return STATIC_DOWNCAST(CCatalogDoc, m_pDocument);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CCatalogView database support

CRecordset* CCatalogView::OnGetRecordset()
{
	return m_pSet;
}

BOOL CCatalogView::OnMove(UINT nIDMoveCommand)
{
	if (!CRecordView::OnMove(nIDMoveCommand))
		return FALSE;

	FillColumnsListBox();
	return TRUE;
}

void CCatalogView::FillColumnsListBox()
{
	ASSERT(!m_Columns.IsOpen());

	m_Columns.m_strQualifierParam = m_pSet->m_strQualifier;
	m_Columns.m_strOwnerParam = m_pSet->m_strOwner;
	m_Columns.m_strTableNameParam = m_pSet->m_strName;

	if (m_Columns.m_pDatabase == NULL)
		m_Columns.m_pDatabase = m_pSet->m_pDatabase;

	if (!m_Columns.Open())
		return;

	m_lbColumns.ResetContent();
	while (!m_Columns.IsEOF())
	{
		m_lbColumns.AddString(m_Columns.m_strColumnName +
			" (" + m_Columns.m_strTypeName + ')');
		m_Columns.MoveNext();
	}

	m_Columns.Close();
}

/////////////////////////////////////////////////////////////////////////////
// CCatalogView message handlers

void CCatalogView::OnUpdateAppExit(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(TRUE);
}
