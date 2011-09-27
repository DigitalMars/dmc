// gridentr.cpp : implementation file
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
#include "chartvw.h"
#include "gridentr.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CGridEntry

IMPLEMENT_DYNCREATE(CGridEntry, CFormView)

CGridEntry::CGridEntry()
	: CFormView(CGridEntry::IDD)
{
	//{{AFX_DATA_INIT(CGridEntry)
	m_pGrid = NULL;
	//}}AFX_DATA_INIT
	m_bEventLockout = FALSE;
	m_bSelChange = FALSE;
	m_bInitialized = FALSE;
}

CGridEntry::~CGridEntry()
{
	CChartDoc* pDoc = (CChartDoc*) GetDocument();
	ASSERT(pDoc->IsKindOf(RUNTIME_CLASS(CChartDoc)));

	if (pDoc->m_pDocGrid == this)
	{           // hand doc responsibility to another grid
		// Do not allow event processing for this view
		m_bEventLockout  = TRUE;
		pDoc->m_pDocGrid = NULL;
		pDoc->UpdateAllViews(this, 0, NULL);
	}
}

void CGridEntry::OnInitialUpdate()     // first time after construct
{
	// Subclass the Edit Control
	m_edit.SubclassDlgItem(IDC_GRIDEDIT, this);

	CFormView::OnInitialUpdate();       // tie into Document

	HWND hWndGrid = m_pGrid->GetSafeHwnd();
	DWORD dwStyles = ::GetWindowLong(hWndGrid, GWL_STYLE);
	::SetWindowLong(hWndGrid, GWL_STYLE, dwStyles | WS_CLIPSIBLINGS);

	CChartDoc* pDoc = (CChartDoc*) GetDocument();
	ASSERT(pDoc->IsKindOf(RUNTIME_CLASS(CChartDoc)));

	if (!m_bInitialized)
	{
		m_bEventLockout = TRUE;

		int nRows = (int)m_pGrid->GetNumProperty("Rows");
		int nCols = (int)m_pGrid->GetNumProperty("Cols");
		char buf[80];

		// Initialize row headings
		m_pGrid->SetNumProperty("Col", 0);
		for (int nRow = 1; nRow < nRows; nRow++)
		{
			wsprintf(buf, "%d", nRow);
			m_pGrid->SetNumProperty("Row", nRow);
			m_pGrid->SetStrProperty("Text", buf);
		}

		// Initialize column headings
		m_pGrid->SetNumProperty("Row", 0);
		for (int nCol = 1; nCol < nCols; nCol++)
		{
			wsprintf(buf, "%c", 'A' + nCol - 1);
			m_pGrid->SetNumProperty("Col", nCol);
			m_pGrid->SetStrProperty("Text", buf);
		}

		// Initalize Cell Contents (Random)
		for (nRow = 1; nRow < nRows; nRow++)
		{
			m_pGrid->SetNumProperty("Row", nRow);
			for (nCol = 1; nCol < nCols; nCol++)
			{
				long lRand = rand();
				lRand *= 100;
				lRand /= (LONG)(RAND_MAX) + 1;

				m_pGrid->SetNumProperty("Col", nCol);
				wsprintf(buf, "%ld", lRand);
				m_pGrid->SetStrProperty("Text", buf);
			}
		}
		m_pGrid->SetNumProperty("Row", 1);
		m_pGrid->SetNumProperty("Col", 1);      // force edit to reposition
		m_bEventLockout = FALSE;
	}
	SizeToFit();
	PositionEdit();
}

void CGridEntry::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CGridEntry)
	DDX_VBControl(pDX, IDC_GRID, m_pGrid);
	//}}AFX_DATA_MAP

}

void CGridEntry::OnUpdate(CView*, LPARAM lHint, CObject* pHint)
{
	if (m_bEventLockout)
		return;

	CChartDoc* pDoc = (CChartDoc*) GetDocument();
	ASSERT(pDoc->IsKindOf(RUNTIME_CLASS(CChartDoc)));

	if (pDoc->m_pDocGrid == NULL)   // if no other grid supplying document
		pDoc->m_pDocGrid = this;

	// Do not allow spurious events to be processed
	pDoc->m_pDocGrid->m_bEventLockout = TRUE;
	m_bEventLockout = TRUE;

	// lHint of -1 is "Draw Graph", not update grid
	if (lHint != -1 && pDoc->m_pDocGrid != this)
	{               // update this grid from document grid control
		if (pHint != NULL && pHint->IsKindOf(RUNTIME_CLASS(CGridEntryHint)))
		{           // Hint has all necessary info for changed cell
			int nRow = LOWORD(lHint);
			int nCol = HIWORD(lHint);

			m_pGrid->SetNumProperty("Row", nRow);
			m_pGrid->SetNumProperty("Col", nCol);
			m_pGrid->SetStrProperty("Text", (LPCSTR)((CGridEntryHint*)pHint)->m_str);
		}
		else if (pHint == NULL)     // must update entire grid
		{
			CVBControl* pDocGrid = pDoc->m_pDocGrid->m_pGrid;
			int nSelStartRowSave = (int)pDocGrid->GetNumProperty("SelStartRow");
			int nSelStartColSave = (int)pDocGrid->GetNumProperty("SelStartCol");
			int nSelEndRowSave   = (int)pDocGrid->GetNumProperty("SelEndRow");
			int nSelEndColSave   = (int)pDocGrid->GetNumProperty("SelEndCol");
			int nFixedRowsSave   = (int)pDocGrid->GetNumProperty("FixedRows");
			int nFixedColsSave   = (int)pDocGrid->GetNumProperty("FixedCols");

			int nRows = (int)pDocGrid->GetNumProperty("Rows");
			int nCols = (int)pDocGrid->GetNumProperty("Cols");

			m_pGrid->SetNumProperty("Rows", nRows);
			m_pGrid->SetNumProperty("Cols", nCols);

			// Select entire grid of both controls
			pDocGrid->SetNumProperty("FixedRows", 0);
			pDocGrid->SetNumProperty("FixedCols", 0);
			pDocGrid->SetNumProperty("SelStartRow", 0);
			pDocGrid->SetNumProperty("SelStartCol", 0);
			pDocGrid->SetNumProperty("SelEndRow", nRows - 1);
			pDocGrid->SetNumProperty("SelEndCol", nCols - 1);

			m_pGrid->SetNumProperty("FixedRows", 0);
			m_pGrid->SetNumProperty("FixedCols", 0);
			m_pGrid->SetNumProperty("SelStartRow", 0);
			m_pGrid->SetNumProperty("SelStartCol", 0);
			m_pGrid->SetNumProperty("SelEndRow", nRows - 1);
			m_pGrid->SetNumProperty("SelEndCol", nCols - 1);

			// Copy Everything
			m_pGrid->SetStrProperty("Clip", pDocGrid->GetStrProperty("Clip"));

			m_pGrid->SetNumProperty("FixedRows",   nFixedRowsSave);
			m_pGrid->SetNumProperty("FixedCols",   nFixedColsSave);
			m_pGrid->SetNumProperty("SelStartRow", nSelStartRowSave);
			m_pGrid->SetNumProperty("SelStartCol", nSelStartColSave);
			m_pGrid->SetNumProperty("SelEndRow",   nSelEndRowSave);
			m_pGrid->SetNumProperty("SelEndCol",   nSelEndColSave);

			// Reselect original selection to Doc grid
			pDocGrid->SetNumProperty("FixedRows",   nFixedRowsSave);
			pDocGrid->SetNumProperty("FixedCols",   nFixedColsSave);
			pDocGrid->SetNumProperty("SelStartRow", nSelStartRowSave);
			pDocGrid->SetNumProperty("SelStartCol", nSelStartColSave);
			pDocGrid->SetNumProperty("SelEndRow",   nSelEndRowSave);
			pDocGrid->SetNumProperty("SelEndCol",   nSelEndColSave);
		}
		m_bInitialized = TRUE;
	}

	SizeToFit();        // set up grid size
	PositionEdit();     // and edit position


//  m_edit.SetWindowPos(&wndTop, 0, 0, 0, 0,
//                      SWP_NOACTIVATE|SWP_NOMOVE|SWP_NOSIZE);

	pDoc->m_pDocGrid->m_bEventLockout = FALSE;
	m_bEventLockout = FALSE;
}

BEGIN_MESSAGE_MAP(CGridEntry, CFormView)
	//{{AFX_MSG_MAP(CGridEntry)
	ON_VBXEVENT(VBN_ROWCOLCHANGE, IDC_GRID, OnRowColChangeGrid)
	ON_VBXEVENT(VBN_SELCHANGE, IDC_GRID, OnSelChangeGrid)
	ON_VBXEVENT(VBN_GOTFOCUS, IDC_GRID, OnGotFocusGrid)
	ON_EN_CHANGE(IDC_GRIDEDIT, OnChangeGridEdit)
	ON_VBXEVENT(VBN_CLICK, IDC_GRID, OnClickGrid)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGridEntry message handlers


void CGridEntry::OnRowColChangeGrid(UINT, int, CWnd*, LPVOID)
{
	if (m_bEventLockout)
		return;

	PositionEdit();
	DoUpdates();
}

void CGridEntry::OnSelChangeGrid(UINT, int, CWnd*, LPVOID)
{
	if (m_bEventLockout)
		return;

	m_bSelChange = TRUE;
}

#define TWIPS_PER_INCH 1440

void CGridEntry::SizeToFit()
{
	// Do not let control stay scrolled
	m_pGrid->SetNumProperty("LeftCol", m_pGrid->GetNumProperty("FixedCols"));
	m_pGrid->SetNumProperty("TopRow",  m_pGrid->GetNumProperty("FixedRows"));

	CPoint ptSize = FindCellPosition((int)m_pGrid->GetNumProperty("Rows"),
									 (int)m_pGrid->GetNumProperty("Cols"));

	if (ptSize.x != (int)m_pGrid->GetNumProperty("Width") ||
		ptSize.y != (int)m_pGrid->GetNumProperty("Height"))
	{
		CRect rect;
		rect.left   = (int)m_pGrid->GetNumProperty("Left");
		rect.top    = (int)m_pGrid->GetNumProperty("Top");
		rect.right  = rect.left + ptSize.x;
		rect.bottom = rect.top  + ptSize.y;

		m_pGrid->Move(rect);

		ptSize.x += 2 * rect.left;
		ptSize.y += 2 * rect.top;

		SetScrollSizes(MM_TEXT, (CSize)ptSize);
	}
}


//
// Returns the position of the top-left corner of a cell.  Asking for
// nRow == maxRow and nCol == maxCol will give the extent of the grid
//
CPoint CGridEntry::FindCellPosition(int nRow, int nCol)
{
	ASSERT(nRow >= 0 && nRow <= m_pGrid->GetNumProperty("Rows"));
	ASSERT(nCol >= 0 && nCol <= m_pGrid->GetNumProperty("Cols"));

	CPoint ptPos(0, 0);

	// Get left edge of requested cell by summing widths of previous cells
	int nLeftCol = (int)m_pGrid->GetNumProperty("LeftCol");
	int nLeftFix = (int)m_pGrid->GetNumProperty("FixedCols");

	for (int i = 0; i < nCol; i++)
	{
		if (i < nLeftFix || i >= nLeftCol)  // only count displayed cells
			ptPos.x += (int)m_pGrid->GetNumProperty("ColWidth", i);
	}

	// Get top edge of requested cell by summing Heights of previous cells
	int nTopRow = (int)m_pGrid->GetNumProperty("TopRow");
	int nTopFix = (int)m_pGrid->GetNumProperty("FixedRows");
	for (i = 0; i < nRow; i++)
	{
		if (i < nTopFix || i >= nTopRow)    // only count displayed cells
			ptPos.y += (int)m_pGrid->GetNumProperty("RowHeight", i);
	}

	// ptPos is now in LOGICAL TWIPs
	// Convert to pixels

	CClientDC dc(this);

	ptPos.x = MulDiv(ptPos.x, dc.GetDeviceCaps(LOGPIXELSX), TWIPS_PER_INCH);
	ptPos.y = MulDiv(ptPos.y, dc.GetDeviceCaps(LOGPIXELSY), TWIPS_PER_INCH);

	ptPos.x += nCol;        // add one pixel per column for gap
	ptPos.y += nRow;        // add one pixel per row    for gap

	return ptPos;
}

void CGridEntry::PositionEdit()
{
	// First determine if the edit control should be visible
	CChartDoc* pDoc = (CChartDoc*) GetDocument();
	ASSERT(pDoc->IsKindOf(RUNTIME_CLASS(CChartDoc)));

	BOOL bVisible = pDoc->m_pDocGrid == this;       // editable?

	if (bVisible)
	{
		int nRow = (int)m_pGrid->GetNumProperty("Row");
		int nCol = (int)m_pGrid->GetNumProperty("Col");

		CPoint ptPos = FindCellPosition(nRow, nCol);

		CClientDC dc(this);
		int nWidth  = MulDiv((int)m_pGrid->GetNumProperty("ColWidth", nCol),
						(int)dc.GetDeviceCaps(LOGPIXELSX), TWIPS_PER_INCH) + 2;

		int nHeight = MulDiv((int)m_pGrid->GetNumProperty("RowHeight", nRow),
						(int)dc.GetDeviceCaps(LOGPIXELSY), TWIPS_PER_INCH) + 2;

		// Adjust for Grid position and scroll position

		ptPos.x += (int)m_pGrid->GetNumProperty("Left");
		ptPos.y += (int)m_pGrid->GetNumProperty("Top");

		m_edit.MoveWindow(ptPos.x, ptPos.y, nWidth, nHeight, TRUE);

		m_bEventLockout = TRUE;
		CString s = m_pGrid->GetStrProperty("Text");
		m_edit.SetWindowText(s);        // Set to reflect cell's data
		m_bEventLockout = FALSE;

		m_edit.ShowWindow(SW_SHOW);
		m_edit.SetSel(0, -1);
		m_edit.Invalidate();
	}
	else
	{
		m_edit.ShowWindow(SW_HIDE);
	}
}

void CGridEntry::OnGotFocusGrid(UINT, int, CWnd*, LPVOID)
{
	if (m_bEventLockout)
		return;

	CChartDoc* pDoc = (CChartDoc*) GetDocument();
	ASSERT(pDoc->IsKindOf(RUNTIME_CLASS(CChartDoc)));

	if (pDoc->m_pDocGrid == this)       // editable?
	{
		m_edit.SetFocus();      // Give the focus to the edit control
		m_edit.SetSel(0, -1);
	}
}

void CGridEntry::OnChangeGridEdit()
{
	if (m_bEventLockout)
		return;

	CGridEntryHint Hint;

	BOOL bValid;
	int nValue = GetDlgItemInt(IDC_GRIDEDIT, &bValid, TRUE);
	if (!bValid)
		return;     // must be a valid int in order to copy.

	m_edit.GetWindowText(Hint.m_str);

	m_pGrid->SetStrProperty("Text", Hint.m_str);

	int nRow = (int)m_pGrid->GetNumProperty("Row");
	int nCol = (int)m_pGrid->GetNumProperty("Col");

	CChartDoc* pDoc = (CChartDoc*) GetDocument();
	ASSERT(pDoc->IsKindOf(RUNTIME_CLASS(CChartDoc)));

	pDoc->UpdateAllViews(this, MAKELONG(nRow, nCol), &Hint);
	pDoc->m_bDirtyGraph = TRUE;     // graphs are now dirty
	pDoc->SetModifiedFlag(TRUE);
}

void CGridEntry::OnClickGrid(UINT, int, CWnd*, LPVOID)
{
	if (m_bEventLockout)
		return;

	SizeToFit();
	PositionEdit();

	DoUpdates();
}

void CGridEntry::DoUpdates()
{
	CChartDoc* pDoc = (CChartDoc*) GetDocument();
	ASSERT(pDoc->IsKindOf(RUNTIME_CLASS(CChartDoc)));

	if (pDoc->m_bDirtyGraph)
	{
		pDoc->UpdateAllViews(NULL, -1, NULL);       // update all of the graphs
	}
	else if (m_bSelChange)
	{
		CSplitterWnd* pParent = (CSplitterWnd*) GetParent();
		ASSERT(pParent->IsKindOf(RUNTIME_CLASS(CSplitterWnd)));

		// update sibling view
		((CChartView*)pParent->GetPane(0, 1))->OnUpdate(NULL, 0, NULL);
	}
	m_bSelChange = FALSE;
	pDoc->m_bDirtyGraph = FALSE;
}


/////////////////////////////////////////////////////////////////////////////
// Hint for CGridEntry

IMPLEMENT_DYNAMIC(CGridEntryHint, CObject)

/////////////////////////////////////////////////////////////////////////////
// CMyEdit

CMyEdit::CMyEdit()
{
}

CMyEdit::~CMyEdit()
{
}


BEGIN_MESSAGE_MAP(CMyEdit, CWnd)
	//{{AFX_MSG_MAP(CMyEdit)
	ON_WM_GETDLGCODE()
	ON_WM_KEYDOWN()
	ON_WM_CHAR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMyEdit message handlers


UINT CMyEdit::OnGetDlgCode()
{
	return CEdit::OnGetDlgCode() | DLGC_WANTALLKEYS;
}

void CMyEdit::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	if (nChar == VK_UP || nChar == VK_DOWN)
		ProcessKeys(nChar);
	else
		CEdit::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CMyEdit::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	if (!ProcessKeys(nChar))
		CEdit::OnChar(nChar, nRepCnt, nFlags);
}

BOOL CMyEdit::ProcessKeys(UINT nChar)
{
	CGridEntry* pParent = (CGridEntry*) GetParent();
	CVBControl* pGrid = pParent->m_pGrid;

	int nRow  = (int)pGrid->GetNumProperty("Row");
	int nCol  = (int)pGrid->GetNumProperty("Col");
	int nRows = (int)pGrid->GetNumProperty("Rows");
	int nCols = (int)pGrid->GetNumProperty("Cols");
	int nFirstRow = (int)pGrid->GetNumProperty("FixedRows");
	int nFirstCol = (int)pGrid->GetNumProperty("FixedCols");

	int nOldCol = nCol;

	switch (nChar)
	{
		case VK_TAB:
			if (GetKeyState(VK_SHIFT) & 0x8000)     // is it a back-tab?
				nCol--;
			else
				nCol++;
			break;

		case VK_RETURN:
			nCol++;
			break;

		case VK_UP:
			nRow--;
			break;

		case VK_DOWN:
			nRow++;
			break;

		default:
			return FALSE;
	}
	if (nCol >= nCols)      // moved off Right edge
	{
		nCol = nFirstCol;
		nRow++;
	}
	if (nCol < nFirstCol)   // moved off left edge
	{
		nCol = nCols - 1;
		nRow--;
	}
	if (nRow < nFirstRow)
		nRow = nRows - 1;
	if (nRow >= nRows)
		nRow = nFirstRow;

	pParent->m_bEventLockout = (nCol != nOldCol);   // only allow one change event
	pGrid->SetNumProperty("Row", nRow);
	pParent->m_bEventLockout = FALSE;
	pGrid->SetNumProperty("Col", nCol);     // generate Row/Col change event
	return TRUE;
}
