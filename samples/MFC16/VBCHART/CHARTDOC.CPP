// chartdoc.cpp : implementation of the CChartDoc class
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
#include "vbchart.h"

#include "chartdoc.h"
#include "gridentr.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CChartDoc

IMPLEMENT_DYNCREATE(CChartDoc, CDocument)

BEGIN_MESSAGE_MAP(CChartDoc, CDocument)
	//{{AFX_MSG_MAP(CChartDoc)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CChartDoc construction/destruction

CChartDoc::CChartDoc()
{
	m_pDocGrid = NULL;
	m_bDirtyGraph = FALSE;
}

CChartDoc::~CChartDoc()
{
}


/////////////////////////////////////////////////////////////////////////////
// CChartDoc serialization

void CChartDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		CVBControl* pGrid = m_pDocGrid->m_pGrid;
		m_pDocGrid->m_bEventLockout = TRUE;

		// Get and Save current selection state
		int nSelStartRowSave = (int)pGrid->GetNumProperty("SelStartRow");
		int nSelStartColSave = (int)pGrid->GetNumProperty("SelStartCol");
		int nSelEndRowSave   = (int)pGrid->GetNumProperty("SelEndRow");
		int nSelEndColSave   = (int)pGrid->GetNumProperty("SelEndCol");
		int nFixedRowsSave   = (int)pGrid->GetNumProperty("FixedRows");
		int nFixedColsSave   = (int)pGrid->GetNumProperty("FixedCols");

		int nRows = (int)pGrid->GetNumProperty("Rows");
		int nCols = (int)pGrid->GetNumProperty("Cols");

		// Select entire grid
		pGrid->SetNumProperty("FixedRows", 0);
		pGrid->SetNumProperty("FixedCols", 0);
		pGrid->SetNumProperty("SelStartRow", 0);
		pGrid->SetNumProperty("SelStartCol", 0);
		pGrid->SetNumProperty("SelEndRow", nRows - 1);
		pGrid->SetNumProperty("SelEndCol", nCols - 1);

		// Save Everything
		ar << (WORD&)nFixedRowsSave << (WORD&)nFixedColsSave
		   << (WORD&)nRows << (WORD&)nCols;

		ar << pGrid->GetStrProperty("Clip");

		// Reselect original selection to Doc grid
		pGrid->SetNumProperty("FixedRows",   nFixedRowsSave);
		pGrid->SetNumProperty("FixedCols",   nFixedColsSave);
		pGrid->SetNumProperty("SelStartRow", nSelStartRowSave);
		pGrid->SetNumProperty("SelStartCol", nSelStartColSave);
		pGrid->SetNumProperty("SelEndRow",   nSelEndRowSave);
		pGrid->SetNumProperty("SelEndCol",   nSelEndColSave);
		m_pDocGrid->m_bEventLockout = FALSE;
	}
	else
	{
		// Find the first Grid View (first view)
		POSITION pos = GetFirstViewPosition();
		CGridEntry* pGridView = (CGridEntry*) GetNextView(pos);

		ASSERT(pGridView != NULL);
		ASSERT(pGridView->IsKindOf(RUNTIME_CLASS(CGridEntry)));

		m_pDocGrid = pGridView;     // connect Doc to View

		CVBControl* pGrid = (CVBControl*)m_pDocGrid->GetDlgItem(IDC_GRID);
		ASSERT(pGrid != NULL);
		ASSERT(pGrid->IsKindOf(RUNTIME_CLASS(CVBControl)));

		m_pDocGrid->m_bEventLockout = TRUE;
		m_pDocGrid->m_bInitialized = TRUE;

		int nFixedRowsSave;
		int nFixedColsSave;
		int nRows;
		int nCols;
		CString s;

		ar >> (WORD&)nFixedRowsSave >> (WORD&)nFixedColsSave
		   >> (WORD&)nRows >> (WORD&)nCols;
		ar >> s;

		// Set up grid to proper dimensions
		pGrid->SetNumProperty("Rows", nRows);
		pGrid->SetNumProperty("Cols", nCols);
		pGrid->SetNumProperty("FixedRows", 0);
		pGrid->SetNumProperty("FixedCols", 0);
		pGrid->SetNumProperty("SelStartRow", 0);
		pGrid->SetNumProperty("SelStartCol", 0);
		pGrid->SetNumProperty("SelEndRow", nRows - 1);
		pGrid->SetNumProperty("SelEndCol", nCols - 1);

		// Fill all Cells
		pGrid->SetStrProperty("Clip", s);

		// Select first cell
		pGrid->SetNumProperty("FixedRows",   nFixedRowsSave);
		pGrid->SetNumProperty("FixedCols",   nFixedColsSave);
		pGrid->SetNumProperty("SelStartRow", nFixedRowsSave);
		pGrid->SetNumProperty("SelStartCol", nFixedColsSave);
		pGrid->SetNumProperty("SelEndRow",   nFixedRowsSave);
		pGrid->SetNumProperty("SelEndCol",   nFixedColsSave);
		m_pDocGrid->m_bEventLockout = FALSE;
	}
}


/////////////////////////////////////////////////////////////////////////////
