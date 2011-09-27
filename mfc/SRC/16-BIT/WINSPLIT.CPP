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
#include <limits.h>

#ifdef AFX_CORE3_SEG
#pragma code_seg(AFX_CORE3_SEG)
#endif

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// Visual attributes and other constants

// HitTest return values (values and spacing between values is important)
enum HitTestValue
{
	noHit                   = 0,
	vSplitterBox            = 1,
	hSplitterBox            = 2,
	bothSplitterBox         = 3,        // just for keyboard
	vSplitterBar1           = 101,
	vSplitterBar15          = 115,
	hSplitterBar1           = 201,
	hSplitterBar15          = 215,
	splitterIntersection1   = 301,
	splitterIntersection225 = 525
};

/////////////////////////////////////////////////////////////////////////////
// CSplitterWnd

BEGIN_MESSAGE_MAP(CSplitterWnd, CWnd)
	//{{AFX_MSG_MAP(CSplitterWnd)
	ON_WM_SETCURSOR()
	ON_WM_MOUSEMOVE()
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_LBUTTONUP()
	ON_WM_CANCELMODE()
	ON_WM_KEYDOWN()
	ON_WM_SIZE()
	ON_WM_HSCROLL()
	ON_WM_VSCROLL()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSplitterWnd construction/destruction

// size of splitter gaps
//  (all panes must have borders - even if client area of pane is 0 pixels wide)
#define CX_SPLITTER_GAP (CX_BORDER + m_cxSplitter + CX_BORDER)
#define CY_SPLITTER_GAP (CY_BORDER + m_cySplitter + CY_BORDER)

CSplitterWnd::CSplitterWnd()
{
	AFX_ZERO_INIT_OBJECT(CWnd);
	// default splitter box/bar size (includes border)
	m_cxSplitter = m_cySplitter = 4;
	if (GetSystemMetrics(SM_CXBORDER) != CX_BORDER ||
		GetSystemMetrics(SM_CYBORDER) != CY_BORDER)
	{
		TRACE0("Warning: CSplitterWnd assumes 1 pixel border\n");
			// will look ugly if borders are not 1 pixel wide and 1 pixel high
	}
}

CSplitterWnd::~CSplitterWnd()
{
	delete m_pRowInfo;
	delete m_pColInfo;
}

BOOL CSplitterWnd::Create(CWnd* pParentWnd,
		int nMaxRows, int nMaxCols, SIZE sizeMin,
		CCreateContext* pContext,
		DWORD dwStyle /* ... | SPLS_DYNAMIC_SPLIT */,
		UINT nID /* = AFX_IDW_PANE_FIRST */)
{
	ASSERT(pParentWnd != NULL);
	ASSERT(sizeMin.cx > 0 && sizeMin.cy > 0);   // minimum must be non-zero

	ASSERT(pContext != NULL);
	ASSERT(pContext->m_pNewViewClass != NULL);
	ASSERT(dwStyle & WS_CHILD);
	ASSERT(dwStyle & SPLS_DYNAMIC_SPLIT);   // must have dynamic split behavior

	// Dynamic splitters are limited to 2x2
	ASSERT(nMaxRows >= 1 && nMaxRows <= 2);
	ASSERT(nMaxCols >= 1 && nMaxCols <= 2);
	ASSERT(nMaxCols > 1 || nMaxRows > 1);       // 1x1 is not permitted

	m_nMaxRows = nMaxRows;
	m_nMaxCols = nMaxCols;
	ASSERT(m_nRows == 0 && m_nCols == 0);       // none yet
	m_nRows = m_nCols = 1;      // start off as 1x1
	if (!CreateCommon(pParentWnd, sizeMin, dwStyle, nID))
		return FALSE;
	ASSERT(m_nRows == 1 && m_nCols == 1);       // still 1x1

	m_pDynamicViewClass = pContext->m_pNewViewClass;
		// save for later dynamic creations

	// add the first initial pane
	if (!CreateView(0, 0, m_pDynamicViewClass, sizeMin, pContext))
	{
		DestroyWindow(); // will clean up child windows
		return FALSE;
	}
	m_pColInfo[0].nIdealSize = sizeMin.cx;
	m_pRowInfo[0].nIdealSize = sizeMin.cy;

	return TRUE;
}


// simple "wiper" splitter
BOOL CSplitterWnd::CreateStatic(CWnd* pParentWnd,
	int nRows, int nCols,
	DWORD dwStyle /* = WS_CHILD | WS_VISIBLE */,
	UINT nID /* = AFX_IDW_PANE_FIRST */)
{
	ASSERT(pParentWnd != NULL);
	ASSERT(nRows >= 1 && nRows <= 16);
	ASSERT(nCols >= 1 && nCols <= 16);
	ASSERT(nCols > 1 || nRows > 1);     // 1x1 is not permitted
	ASSERT(dwStyle & WS_CHILD);
	ASSERT(!(dwStyle & SPLS_DYNAMIC_SPLIT)); // can't have dynamic split

	ASSERT(m_nRows == 0 && m_nCols == 0);       // none yet
	m_nRows = m_nMaxRows = nRows;
	m_nCols = m_nMaxCols = nCols;

	// create with zero minimum pane size
	if (!CreateCommon(pParentWnd, CSize(0, 0), dwStyle, nID))
		return FALSE;

	// all panes must be created with explicit calls to CreateView
	return TRUE;
}

BOOL CSplitterWnd::CreateCommon(CWnd* pParentWnd,
	SIZE sizeMin, DWORD dwStyle, UINT nID)
{
	ASSERT(pParentWnd != NULL);
	ASSERT(sizeMin.cx >= 0 && sizeMin.cy >= 0);
	ASSERT(dwStyle & WS_CHILD);
	ASSERT(nID != 0);

	ASSERT(m_pColInfo == NULL && m_pRowInfo == NULL);   // only do once
	ASSERT(m_nMaxCols > 0 && m_nMaxRows > 0);

	// the Windows scroll bar styles bits turn no the smart scrollbars
	m_bHasHScroll = (dwStyle & WS_HSCROLL) != 0;
	m_bHasVScroll = (dwStyle & WS_VSCROLL) != 0;
	dwStyle &= ~(WS_HSCROLL|WS_VSCROLL);

	// create with the same wnd-class as MDI-Frame (no erase bkgnd)
	if (!CreateEx(0, _afxWndMDIFrame, NULL, dwStyle, 0, 0, 0, 0,
	  pParentWnd->m_hWnd, (HMENU)nID, NULL))
		return FALSE;       // create invisible

	// attach the initial splitter parts
	TRY
	{
		int row;
		int col;

		m_pColInfo = new CRowColInfo[m_nMaxCols];
		for (col = 0; col < m_nMaxCols; col++)
		{
			m_pColInfo[col].nMinSize = m_pColInfo[col].nIdealSize = sizeMin.cx;
			m_pColInfo[col].nCurSize = -1; // will be set in RecalcLayout
		}
		m_pRowInfo = new CRowColInfo[m_nMaxRows];
		for (row = 0; row < m_nMaxRows; row++)
		{
			m_pRowInfo[row].nMinSize = m_pRowInfo[row].nIdealSize = sizeMin.cy;
			m_pRowInfo[row].nCurSize = -1; // will be set in RecalcLayout
		}

		if (m_bHasHScroll)
			for (col = 0; col < m_nCols; col++)
				if (!CreateScrollBarCtrl(SBS_HORZ, AFX_IDW_HSCROLL_FIRST + col))
					AfxThrowResourceException();
		if (m_bHasVScroll)
			for (row = 0; row < m_nRows; row++)
				if (!CreateScrollBarCtrl(SBS_VERT, AFX_IDW_VSCROLL_FIRST + row))
					AfxThrowResourceException();

		if (m_bHasHScroll && m_bHasVScroll)
			if (!CreateScrollBarCtrl(SBS_SIZEBOX|WS_DISABLED, AFX_IDW_SIZE_BOX))
				AfxThrowResourceException();
	}
	CATCH_ALL(e)
	{
		DestroyWindow(); // will clean up child windows
		return FALSE;
	}
	END_CATCH_ALL

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CSplitterWnd default creation of parts

// You must create ALL panes unless DYNAMIC_SPLIT is defined !
//  Usually the splitter window is invisible when creating a pane
BOOL CSplitterWnd::CreateView(int row, int col,
	CRuntimeClass* pViewClass, SIZE sizeInit, CCreateContext* pContext)
{
#ifdef _DEBUG
	ASSERT_VALID(this);
	ASSERT(row >= 0 && row < m_nRows);
	ASSERT(col >= 0 && col < m_nCols);
	ASSERT(pViewClass != NULL);
	ASSERT(AfxIsValidAddress(pViewClass, sizeof(CRuntimeClass)));

	if (GetDlgItem(IdFromRowCol(row, col)) != NULL)
	{
		TRACE2("Error: CreateView - pane already exists for row %d, col %d\n",
			row, col);
		ASSERT(FALSE);
		return FALSE;
	}
#endif

	// set the initial size for that pane
	m_pColInfo[col].nIdealSize = sizeInit.cx;
	m_pRowInfo[row].nIdealSize = sizeInit.cy;

	BOOL bSendInitialUpdate = FALSE;

	CCreateContext contextT;
	if (pContext == NULL)
	{
		// if no context specified, generate one from the currently selected
		//  client if possible
		CView* pOldView = GetParentFrame()->GetActiveView();
		if (pOldView != NULL)
		{
			// set info about last pane
			ASSERT(contextT.m_pCurrentFrame == NULL);
			contextT.m_pLastView = pOldView;
			contextT.m_pCurrentDoc = pOldView->GetDocument();
			if (contextT.m_pCurrentDoc != NULL)
				contextT.m_pNewDocTemplate =
				  contextT.m_pCurrentDoc->GetDocTemplate();
		}
		pContext = &contextT;
		bSendInitialUpdate = TRUE;
	}

	CWnd* pWnd;
	TRY
	{
		pWnd = (CWnd*)pViewClass->CreateObject();
		if (pWnd == NULL)
			AfxThrowMemoryException();
	}
	CATCH_ALL(e)
	{
		TRACE0("Out of memory creating a splitter pane\n");
		return FALSE;
	}
	END_CATCH_ALL

	ASSERT(pWnd->IsKindOf(RUNTIME_CLASS(CWnd)));
	ASSERT(pWnd->m_hWnd == NULL);       // not yet created

	// Create with the right size (wrong position)
	CRect rect(CPoint(0,0), sizeInit);
	if (!pWnd->Create(NULL, NULL, AFX_WS_DEFAULT_VIEW,
		rect, this, IdFromRowCol(row, col), pContext))
	{
		TRACE0("Warning: couldn't create client pane for splitter\n");
			// pWnd will be cleaned up by PostNcDestroy
		return FALSE;
	}
	ASSERT((int)_AfxGetDlgCtrlID(pWnd->m_hWnd) == IdFromRowCol(row, col));

	// send initial notification message
	if (bSendInitialUpdate)
		pWnd->SendMessage(WM_INITIALUPDATE);
	return TRUE;
}


BOOL CSplitterWnd::CreateScrollBarCtrl(DWORD dwStyle, UINT nID)
{
	ASSERT_VALID(this);
	ASSERT(m_hWnd != NULL);

	return (::CreateWindow("SCROLLBAR", NULL, dwStyle | WS_VISIBLE | WS_CHILD,
		0, 0, 1, 1, m_hWnd, (HMENU)nID,
		AfxGetInstanceHandle(), NULL) != NULL);
}

int CSplitterWnd::IdFromRowCol(int row, int col) const
{
	ASSERT_VALID(this);
	ASSERT(row >= 0);
	ASSERT(row < m_nRows);
	ASSERT(col >= 0);
	ASSERT(col < m_nCols);

	return AFX_IDW_PANE_FIRST + row * 16 + col;
}

/////////////////////////////////////////////////////////////////////////////
// CSplitterWnd attributes

CWnd* CSplitterWnd::GetPane(int row, int col) const
{
	ASSERT_VALID(this);

	CWnd* pView = GetDlgItem(IdFromRowCol(row, col));
	ASSERT(pView != NULL);
		// panes can be a CWnd, but are usually CViews
	return pView;
}

BOOL CSplitterWnd::IsChildPane(CWnd* pWnd, int& row, int& col)
{
	ASSERT_VALID(this);
	ASSERT_VALID(pWnd);

	UINT nID = _AfxGetDlgCtrlID(pWnd->m_hWnd);
	if (IsChild(pWnd) && nID >= AFX_IDW_PANE_FIRST && nID <= AFX_IDW_PANE_LAST)
	{
		row = (nID - AFX_IDW_PANE_FIRST) / 16;
		col = (nID - AFX_IDW_PANE_FIRST) % 16;
		return TRUE;
	}
	else
	{
		row = col = -1;
		return FALSE;
	}
}

/////////////////////////////////////////////////////////////////////////////
// CSplitterWnd information access

// The get routines return the current size
// The set routines set the ideal size
//  RecalcLayout must be called to update current size

void CSplitterWnd::GetRowInfo(int row, int& cyCur, int& cyMin) const
{
	ASSERT_VALID(this);
	ASSERT(row >= 0 && row < m_nRows);

	cyCur = m_pRowInfo[row].nCurSize;
	cyMin = m_pRowInfo[row].nMinSize;
}

void CSplitterWnd::SetRowInfo(int row, int cyIdeal, int cyMin)
{
	ASSERT_VALID(this);
	ASSERT(row >= 0 && row < m_nRows);
	ASSERT(cyIdeal >= 0);
	ASSERT(cyMin >= 0);

	m_pRowInfo[row].nIdealSize = cyIdeal;
	m_pRowInfo[row].nMinSize = cyMin;
}

void CSplitterWnd::GetColumnInfo(int col, int& cxCur, int& cxMin) const
{
	ASSERT_VALID(this);
	ASSERT(col >= 0 && col < m_nCols);

	cxCur = m_pColInfo[col].nCurSize;
	cxMin = m_pColInfo[col].nMinSize;
}

void CSplitterWnd::SetColumnInfo(int col, int cxIdeal, int cxMin)
{
	ASSERT_VALID(this);
	ASSERT(col >= 0 && col < m_nCols);

	m_pColInfo[col].nIdealSize = cxIdeal;
	m_pColInfo[col].nMinSize = cxMin;
}

/////////////////////////////////////////////////////////////////////////////
// CSplitterWnd client operations/overridables

void CSplitterWnd::DeleteView(int row, int col)
{
	ASSERT_VALID(this);

	// if active child is being deleted - activate next
	CWnd* pPane = GetPane(row, col);
	ASSERT(pPane->IsKindOf(RUNTIME_CLASS(CView)));
	ASSERT(GetParentFrame() != NULL);
	if (GetParentFrame()->GetActiveView() == pPane)
		ActivateNext(FALSE);

	// default implementation assumes view will auto delete in PostNcDestroy
	pPane->DestroyWindow();
}

void CSplitterWnd::OnDrawSplitter(CDC* pDC, ESplitType nType,
		const CRect& rectArg)
	// if pDC == NULL then just invalidate
{
	if (pDC == NULL)
	{
		InvalidateRect(rectArg);
		return;
	}
	ASSERT_VALID(pDC);

	// otherwise actually draw
	CRect rect = rectArg;
	HGDIOBJ hOldBrush;
	if (nType == splitIntersection)
	{
		// prepare to fill the entire interesection
		hOldBrush = pDC->SelectObject(afxData.hbrLtGray);
	}
	else
	{
		// bar or box drawn the same
		// top or left hilite for 3D look
		pDC->PatBlt(rect.left, rect.top, rect.Width(), 1, WHITENESS);
		pDC->PatBlt(rect.left, rect.top, 1, rect.Height(), WHITENESS);

		// bottom or right shadow for 3D look
		hOldBrush = pDC->SelectObject(afxData.hbrDkGray);
		pDC->PatBlt(rect.right - 1, rect.top, 1, rect.Height(), PATCOPY);
		pDC->PatBlt(rect.left, rect.bottom - 1, rect.Width(), 1, PATCOPY);

		// prepare to fill the rest with gray
		pDC->SelectObject(afxData.hbrLtGray);
		rect.InflateRect(-1, -1);
	}
	// fill the middle
	pDC->PatBlt(rect.left, rect.top, rect.Width(), rect.Height(), PATCOPY);
	pDC->SelectObject(hOldBrush);
}

/////////////////////////////////////////////////////////////////////////////
// Dynamic row/col split etc

static int NEAR PASCAL CanSplitRowCol(CSplitterWnd::CRowColInfo* pInfoBefore,
		int nBeforeSize, int nSizeSplitter)
	// request to split Before row at point nBeforeSize
	// returns size of new pane (nBeforeSize will be new size of Before pane)
	// return -1 if not big enough
{
	ASSERT(pInfoBefore->nCurSize > 0);
	ASSERT(pInfoBefore->nMinSize > 0);
	ASSERT(nBeforeSize <= pInfoBefore->nCurSize);

	// space gets take from before pane (weird UI for > 2 splits)
	if (nBeforeSize < pInfoBefore->nMinSize)
	{
		TRACE0("Warning: split too small to fit in a new pane\n");
		return -1;
	}

	int nNewSize = pInfoBefore->nCurSize - nBeforeSize - nSizeSplitter;
	if (nBeforeSize < pInfoBefore->nMinSize)
	{
		TRACE0("Warning: split too small to shrink old pane\n");
		return -1;
	}
	if (nNewSize < (pInfoBefore+1)->nMinSize)
	{
		TRACE0("Warning: split too small to create new pane\n");
		return -1;
	}
	return nNewSize;
}


BOOL CSplitterWnd::SplitRow(int cyBefore)
{
	ASSERT_VALID(this);
	ASSERT(GetStyle() & SPLS_DYNAMIC_SPLIT);
	ASSERT(m_pDynamicViewClass != NULL);
	ASSERT(m_nRows < m_nMaxRows);

	int rowNew = m_nRows;
	int cyNew = CanSplitRowCol(&m_pRowInfo[rowNew-1], cyBefore, m_cySplitter);
	if (cyNew == -1)
		return FALSE;   // too small to split

	// create the scroll bar first (so new views can see that it is there)
	if (m_bHasVScroll &&
		!CreateScrollBarCtrl(SBS_VERT, AFX_IDW_VSCROLL_FIRST + rowNew))
	{
		TRACE0("Warning: SplitRow failed to create scroll bar\n");
		return FALSE;
	}

	m_nRows++;  // bump count during view creation

	// create new views to fill the new row (RecalcLayout will position)
	for (int col = 0; col < m_nCols; col++)
	{
		CSize size(m_pColInfo[col].nCurSize, cyNew);
		if (!CreateView(rowNew, col, m_pDynamicViewClass, size, NULL))
		{
			TRACE0("Warning: SplitRow failed to create new row\n");
			// delete anything we partially created 'col' = # columns created
			while (col > 0)
				DeleteView(rowNew, --col);
			if (m_bHasVScroll)
				GetDlgItem(AFX_IDW_VSCROLL_FIRST + rowNew)->DestroyWindow();
			m_nRows--;      // it didn't work out
			return FALSE;
		}
	}

	// new parts created - resize and re-layout
	m_pRowInfo[rowNew-1].nIdealSize = cyBefore;
	m_pRowInfo[rowNew].nIdealSize = cyNew;
	ASSERT(m_nRows == rowNew+1);
	RecalcLayout();
	return TRUE;
}

BOOL CSplitterWnd::SplitColumn(int cxBefore)
{
	ASSERT_VALID(this);
	ASSERT(GetStyle() & SPLS_DYNAMIC_SPLIT);
	ASSERT(m_pDynamicViewClass != NULL);
	ASSERT(m_nCols < m_nMaxCols);

	int colNew = m_nCols;
	int cxNew = CanSplitRowCol(&m_pColInfo[colNew-1], cxBefore, m_cxSplitter);
	if (cxNew == -1)
		return FALSE;   // too small to split

	// create the scroll bar first (so new views can see that it is there)
	if (m_bHasHScroll &&
		!CreateScrollBarCtrl(SBS_HORZ, AFX_IDW_HSCROLL_FIRST + colNew))
	{
		TRACE0("Warning: SplitRow failed to create scroll bar\n");
		return FALSE;
	}

	m_nCols++;  // bump count during view creation

	// create new views to fill the new column (RecalcLayout will position)
	for (int row = 0; row < m_nRows; row++)
	{
		CSize size(cxNew, m_pRowInfo[row].nCurSize);
		if (!CreateView(row, colNew, m_pDynamicViewClass, size, NULL))
		{
			TRACE0("Warning: SplitColumn failed to create new column\n");
			// delete anything we partially created 'col' = # columns created
			while (row > 0)
				DeleteView(--row, colNew);
			if (m_bHasHScroll)
				GetDlgItem(AFX_IDW_HSCROLL_FIRST + colNew)->DestroyWindow();
			m_nCols--;      // it didn't work out
			return FALSE;
		}
	}

	// new parts created - resize and re-layout
	m_pColInfo[colNew-1].nIdealSize = cxBefore;
	m_pColInfo[colNew].nIdealSize = cxNew;
	ASSERT(m_nCols == colNew+1);
	RecalcLayout();
	return TRUE;
}

void CSplitterWnd::DeleteRow(int row)
{
	ASSERT_VALID(this);
	ASSERT(GetStyle() & SPLS_DYNAMIC_SPLIT);

	// default implementation deletes last row only
	ASSERT(m_nRows > 1);
	row = m_nRows - 1;
	for (int col = 0; col < m_nCols; col++)
		DeleteView(row, col);
	if (m_bHasVScroll)
		GetDlgItem(AFX_IDW_VSCROLL_FIRST + row)->DestroyWindow();
	m_nRows--;
	RecalcLayout();     // re-assign the space
}

void CSplitterWnd::DeleteColumn(int col)
{
	ASSERT_VALID(this);
	ASSERT(GetStyle() & SPLS_DYNAMIC_SPLIT);

	// default implementation deletes last column only
	ASSERT(m_nCols > 1);
	col = m_nCols - 1;
	for (int row = 0; row < m_nRows; row++)
		DeleteView(row, col);
	if (m_bHasHScroll)
		GetDlgItem(AFX_IDW_HSCROLL_FIRST + col)->DestroyWindow();
	m_nCols--;
	RecalcLayout();     // re-assign the space
}

/////////////////////////////////////////////////////////////////////////////
// CSplitterWnd tracking support

// like GetClientRect but inset by shared scrollbars
void CSplitterWnd::GetInsideRect(CRect& rect) const
{
	ASSERT_VALID(this);

	GetClientRect(rect);
	ASSERT(rect.left == 0 && rect.top == 0);
	// subtract scrollbar clearance
	if (m_bHasVScroll)
		rect.right -= (afxData.cxVScroll - CX_BORDER);
	if (m_bHasHScroll)
		rect.bottom -= (afxData.cyHScroll - CY_BORDER);
}

void CSplitterWnd::StartTracking(int ht)
{
	ASSERT_VALID(this);
	if (ht == noHit)
		return;

	GetInsideRect(m_rectLimit);
		// GetHitRect will restrict 'm_rectLimit' as appropriate

	if (ht >= splitterIntersection1 && ht <= splitterIntersection225)
	{
		// split two directions (two tracking rectangles)
		int row = (ht - splitterIntersection1) / 15;
		int col = (ht - splitterIntersection1) % 15;

		GetHitRect(row + vSplitterBar1, m_rectTracker);
		m_bTracking2 = TRUE;
		GetHitRect(col + hSplitterBar1, m_rectTracker2);
		m_ptTrackOffset.x = -(m_cxSplitter/2);
		m_ptTrackOffset.y = -(m_cySplitter/2);
	}
	else if (ht == bothSplitterBox)
	{
		GetHitRect(vSplitterBox, m_rectTracker);
		m_bTracking2 = TRUE;
		GetHitRect(hSplitterBox, m_rectTracker2);
	}
	else
	{
		GetHitRect(ht, m_rectTracker);
	}

	// adjust for border size
	m_rectLimit.right -= CX_BORDER;
	m_rectLimit.bottom -= CY_BORDER;

	SetCapture();
	SetFocus();

	// make sure no updates are pending
	UpdateWindow();

	m_bTracking = TRUE;
	OnInvertTracker(m_rectTracker);
	if (m_bTracking2)
		OnInvertTracker(m_rectTracker2);
	m_htTrack = ht;
}

void CSplitterWnd::TrackRowSize(int y, int row)
{
	ASSERT_VALID(this);
	ASSERT(m_nRows > 1);

	CPoint pt(0, y);
	ClientToScreen(&pt);
	GetPane(row, 0)->ScreenToClient(&pt);
	m_pRowInfo[row].nIdealSize = pt.y;      // new size
	if (pt.y < m_pRowInfo[row].nMinSize)
	{
		// resized too small
		m_pRowInfo[row].nIdealSize = 0; // make it go away
		if (GetStyle() & SPLS_DYNAMIC_SPLIT)
			DeleteRow(row);
	}
	else if (m_pRowInfo[row].nCurSize + m_pRowInfo[row+1].nCurSize
			< pt.y + m_pRowInfo[row+1].nMinSize)
	{
		// not enough room for other pane
		if (GetStyle() & SPLS_DYNAMIC_SPLIT)
			DeleteRow(row);
	}
}


void CSplitterWnd::TrackColumnSize(int x, int col)
{
	ASSERT_VALID(this);
	ASSERT(m_nCols > 1);

	CPoint pt(x, 0);
	ClientToScreen(&pt);
	GetPane(0, col)->ScreenToClient(&pt);
	m_pColInfo[col].nIdealSize = pt.x;      // new size
	if (pt.x < m_pColInfo[col].nMinSize)
	{
		// resized too small
		m_pColInfo[col].nIdealSize = 0; // make it go away
		if (GetStyle() & SPLS_DYNAMIC_SPLIT)
			DeleteColumn(col);
	}
	else if (m_pColInfo[col].nCurSize + m_pColInfo[col+1].nCurSize
			< pt.x + m_pColInfo[col+1].nMinSize)
	{
		// not enough room for other pane
		if (GetStyle() & SPLS_DYNAMIC_SPLIT)
			DeleteColumn(col);
	}
}


void CSplitterWnd::StopTracking(BOOL bAccept)
{
	ASSERT_VALID(this);

	if (!m_bTracking)
		return;

	ReleaseCapture();
	// erase tracker rectangle
	OnInvertTracker(m_rectTracker);
	if (m_bTracking2)
		OnInvertTracker(m_rectTracker2);
	m_bTracking = m_bTracking2 = FALSE;

	// save old active view
	CFrameWnd* pParent = GetParentFrame();
	ASSERT(pParent != NULL);    // must be in a frame
	CView* pOldActiveView = pParent->GetActiveView();

	// m_rectTracker is set to the new splitter position (without border)
	m_rectTracker.left -= CX_BORDER;
	m_rectTracker.top -= CY_BORDER;
		// adjust relative to where the border will be

	if (bAccept)
	{
		if (m_htTrack == vSplitterBox)
		{
			SplitRow(m_rectTracker.top);
		}
		else if (m_htTrack >= vSplitterBar1 && m_htTrack <= vSplitterBar15)
		{
			// set row height
			TrackRowSize(m_rectTracker.top, m_htTrack - vSplitterBar1);
			RecalcLayout();
		}
		else if (m_htTrack == hSplitterBox)
		{
			SplitColumn(m_rectTracker.left);
		}
		else if (m_htTrack >= hSplitterBar1 && m_htTrack <= hSplitterBar15)
		{
			// set column width
			TrackColumnSize(m_rectTracker.left, m_htTrack - hSplitterBar1);
			RecalcLayout();
		}
		else if (m_htTrack >= splitterIntersection1 &&
			m_htTrack <= splitterIntersection225)
		{
			// set row height and column width
			int row = (m_htTrack - splitterIntersection1) / 15;
			int col = (m_htTrack - splitterIntersection1) % 15;

			TrackRowSize(m_rectTracker.top, row);
			TrackColumnSize(m_rectTracker2.left, col);
			RecalcLayout();
		}
		else if (m_htTrack == bothSplitterBox)
		{
			// rectTracker is vSplitter (splits rows)
			// rectTracker2 is hSplitter (splits cols)
			SplitRow(m_rectTracker.top);
			SplitColumn(m_rectTracker2.left);
		}
	}

	if (pOldActiveView == pParent->GetActiveView())
	{
		pParent->SetActiveView(pOldActiveView); // re-activate
		if (pOldActiveView != NULL)
			pOldActiveView->SetFocus(); // make sure focus is restored
	}
}


void CSplitterWnd::GetHitRect(int ht, CRect& rectHit)
{
	ASSERT_VALID(this);

	CRect rectClient;
	GetClientRect(&rectClient);
	ASSERT(rectClient.left == 0 && rectClient.top == 0);
	int cx = rectClient.right;
	int cy = rectClient.bottom;
	int x = 0;
	int y = 0;

	// hit rectangle does not include border
	// m_rectLimit will be limited to valid tracking rect
	// m_ptTrackOffset will be set to appropriate tracking offset
	m_ptTrackOffset.x = 0;
	m_ptTrackOffset.y = 0;

	if (ht == vSplitterBox)
	{
		cy = m_cySplitter;
		m_ptTrackOffset.y = -(cy/2);
		ASSERT(m_rectLimit.top == 0);
		ASSERT(m_pRowInfo[0].nCurSize > 0);
		m_rectLimit.bottom += m_ptTrackOffset.y - CY_BORDER;
	}
	else if (ht == hSplitterBox)
	{
		cx = m_cxSplitter;
		m_ptTrackOffset.x = -(cx/2);
		ASSERT(m_rectLimit.left == 0);
		ASSERT(m_pColInfo[0].nCurSize > 0);
		m_rectLimit.right += m_ptTrackOffset.x - CX_BORDER;
	}
	else if (ht >= vSplitterBar1 && ht <= vSplitterBar15)
	{
		cy = m_cySplitter;
		m_ptTrackOffset.y = -(cy/2);
		for (int row = 0; row < ht - vSplitterBar1; row++)
			y += m_pRowInfo[row].nCurSize + CY_SPLITTER_GAP;
		m_rectLimit.top = y;
		y += m_pRowInfo[row].nCurSize + CY_BORDER;
		m_rectLimit.bottom += m_ptTrackOffset.y - CY_BORDER;
	}
	else if (ht >= hSplitterBar1 && ht <= hSplitterBar15)
	{
		cx = m_cxSplitter;
		m_ptTrackOffset.x = -(cx/2);
		for (int col = 0; col < ht - hSplitterBar1; col++)
			x += m_pColInfo[col].nCurSize + CX_SPLITTER_GAP;
		m_rectLimit.left = x;
		x += m_pColInfo[col].nCurSize + CX_BORDER;
		m_rectLimit.right += m_ptTrackOffset.x - CX_BORDER;
	}
	else
	{
		TRACE1("Error: GetHitRect(%d): Not Found!\n", ht);
		ASSERT(FALSE);
	}

	rectHit.right = (rectHit.left = x) + cx;
	rectHit.bottom = (rectHit.top = y) + cy;
}


int CSplitterWnd::HitTest(CPoint pt) const
{
	ASSERT_VALID(this);

	CRect rectClient;
	GetClientRect(&rectClient);
	ASSERT(rectClient.left == 0 && rectClient.top == 0);

	if (m_bHasVScroll && m_nRows < m_nMaxRows &&
		CRect(rectClient.right - afxData.cxVScroll, 0,
		 rectClient.right, m_cySplitter + CY_BORDER).PtInRect(pt))
	{
		return vSplitterBox;
	}

	if (m_bHasHScroll && m_nCols < m_nMaxCols &&
		CRect(0, rectClient.bottom - afxData.cyHScroll,
		 m_cxSplitter + CX_BORDER, rectClient.bottom).PtInRect(pt))
	{
		return hSplitterBox;
	}

	// for hit detect, include the border of splitters
	CRect rect;
	rect = rectClient;
	rect.left = 0;
	for (int col = 0; col < m_nCols - 1; col++)
	{
		rect.left += m_pColInfo[col].nCurSize;
		rect.right = rect.left + CX_SPLITTER_GAP;
		if (rect.PtInRect(pt))
			break;
		rect.left = rect.right;
	}

	rect = rectClient;
	rect.top = 0;
	for (int row = 0; row < m_nRows - 1; row++)
	{
		rect.top += m_pRowInfo[row].nCurSize;
		rect.bottom = rect.top + CY_SPLITTER_GAP;
		if (rect.PtInRect(pt))
			break;
		rect.top = rect.bottom;
	}

	// row and col set for hit splitter (if not hit will be past end)
	if (col != m_nCols - 1)
	{
		if (row != m_nRows - 1)
			return splitterIntersection1 + row * 15 + col;
		return hSplitterBar1 + col;
	}

	if (row != m_nRows - 1)
		return vSplitterBar1 + row;

	return noHit;
}

void CSplitterWnd::OnInvertTracker(const CRect& rect)
{
	ASSERT_VALID(this);
	ASSERT(!rect.IsRectEmpty());
	ASSERT((GetStyle() & WS_CLIPCHILDREN) == 0);

	// pat-blt without clip children on
	CClientDC dc(this);
	dc.PatBlt(rect.left, rect.top, rect.Width(), rect.Height(), DSTINVERT);
}

/////////////////////////////////////////////////////////////////////////////
// CSplitterWnd commands

// Keyboard interface
BOOL CSplitterWnd::DoKeyboardSplit()
{
	ASSERT_VALID(this);

	int ht;
	if (m_nRows > 1 && m_nCols > 1)
		ht = splitterIntersection1; // split existing row+col
	else if (m_nRows > 1)
		ht = vSplitterBar1;         // split existing row
	else if (m_nCols > 1)
		ht = hSplitterBar1;         // split existing col
	else if (m_nMaxRows > 1 && m_nMaxCols > 1)
		ht = bothSplitterBox;       // we can split both
	else if (m_nMaxRows > 1)
		ht = vSplitterBox;          // we can split rows
	else if (m_nMaxCols > 1)
		ht = hSplitterBox;          // we can split columns
	else
		return FALSE;               // can't split

	// move the mouse cursor to the center of the client rect
	CRect rect;
	GetClientRect(&rect);
	ClientToScreen(&rect);
	::SetCursorPos(rect.left + rect.Width() / 2, rect.top + rect.Height() / 2);

	// start tracking
	StartTracking(ht);
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// Main drawing and layout

void CSplitterWnd::OnSize(UINT nType, int cx, int cy)
{
	if (nType != SIZE_MINIMIZED && cx > 0 && cy > 0)
		RecalcLayout();
	CWnd::OnSize(nType, cx, cy);
}

// Generic routine:
//  for X direction: pInfo = m_pColInfo, nMax = m_nMaxCols, nSize = cx
//  for Y direction: pInfo = m_pRowInfo, nMax = m_nMaxRows, nSize = cy
static void NEAR PASCAL LayoutRowCol(CSplitterWnd::CRowColInfo* pInfoArray,
		int nMax, int nSize, int nSizeSplitter)
{
	ASSERT(pInfoArray != NULL);
	ASSERT(nMax > 0);
	ASSERT(nSizeSplitter > 0);

	CSplitterWnd::CRowColInfo* pInfo;
	int i;

	if (nSize < 0)
		nSize = 0;  // if really too small layout as zero size

	// start with ideal sizes
	for (i = 0, pInfo = pInfoArray; i < nMax-1; i++, pInfo++)
	{
		if (pInfo->nIdealSize < pInfo->nMinSize)
			pInfo->nIdealSize = 0;      // too small to see
		pInfo->nCurSize = pInfo->nIdealSize;
	}
	pInfo->nCurSize = INT_MAX;  // last row/column takes the rest

	for (i = 0, pInfo = pInfoArray; i < nMax; i++, pInfo++)
	{
		ASSERT(nSize >= 0);
		if (nSize == 0)
		{
			// no more room (set pane to be invisible)
			pInfo->nCurSize = 0;
			continue;       // don't worry about splitters
		}
		else if (nSize < pInfo->nMinSize && i != 0)
		{
			// additional panes below the recommended minimum size
			//   aren't shown and the size goes to the previous pane
			pInfo->nCurSize = 0;

			// previous pane already has room for splitter + CX_BORDER
			//   add remaining size and remove the extra border
			ASSERT(CX_BORDER == CY_BORDER);
			(pInfo-1)->nCurSize += nSize + CX_BORDER;
			nSize = 0;
		}
		else
		{
			// otherwise we can add the second pane
			ASSERT(nSize > 0);
			if (pInfo->nCurSize == 0)
			{
				// too small to see
				if (i != 0)
					pInfo->nCurSize = 0;
			}
			else if (nSize < pInfo->nCurSize)
			{
				// this row/col won't fit completely - make as small as possible
				pInfo->nCurSize = nSize;
				nSize = 0;
			}
			else
			{
				// can fit everything
				nSize -= pInfo->nCurSize;
			}
		}

		// see if we should add a splitter
		ASSERT(nSize >= 0);
		if (i != nMax - 1)
		{
			// should have a splitter
			if (nSize > nSizeSplitter)
			{
				nSize -= nSizeSplitter; // leave room for splitter + border
				ASSERT(nSize > 0);
			}
			else
			{
				// not enough room - add left over less splitter size
				ASSERT(CX_BORDER == CY_BORDER);
				pInfo->nCurSize += nSize;
				if (pInfo->nCurSize > (nSizeSplitter-CX_BORDER))
					pInfo->nCurSize -= (nSizeSplitter-CX_BORDER);
				nSize = 0;
			}
		}
	}
	ASSERT(nSize == 0); // all space should be allocated
}

// repositions client area of specified window
// assumes everything has WS_BORDER or is inset like it does
//  (includes scroll bars)
static HDWP NEAR PASCAL DeferClientPos(HDWP hDWP, CWnd* pWnd,
		int x, int y, int cx, int cy, BOOL bScrollBar)
{
	ASSERT(pWnd != NULL);
	ASSERT(pWnd->m_hWnd != NULL);

	if (bScrollBar)
	{
		// if there is enough room, draw scroll bar without border
		// if there is not enough room, set the WS_BORDER bit so that
		//   we will at least get a proper border drawn
		DWORD dwStyle = pWnd->GetStyle();
		BOOL bNeedBorder = (cx <= CX_BORDER || cy <= CY_BORDER);
		if (bNeedBorder)
			dwStyle |= WS_BORDER;
		else
			dwStyle &= ~WS_BORDER;
		::SetWindowLong(pWnd->m_hWnd, GWL_STYLE, dwStyle);
	}

	// adjust for border size (even if zero client size)
	x -= CX_BORDER;
	y -= CY_BORDER;
	cx += 2 * CX_BORDER;
	cy += 2 * CY_BORDER;

	// first check if the new rectangle is the same as the current
	CRect rectOld;
	pWnd->GetWindowRect(rectOld);
	pWnd->GetParent()->ScreenToClient(&rectOld);
	if (rectOld.left == x && rectOld.top == y &&
		rectOld.Width() == cx && rectOld.Height() == cy)
	{
		return hDWP;        // nothing to do
	}

	return ::DeferWindowPos(hDWP, pWnd->m_hWnd, NULL,
		x, y, cx, cy, SWP_NOZORDER | SWP_NOACTIVATE);
}

void CSplitterWnd::RecalcLayout()
{
	ASSERT_VALID(this);
	ASSERT(m_nRows > 0 && m_nCols > 0);
			// must have at least one pane

	CRect rectInside;
	GetInsideRect(rectInside);

	// layout columns (restrict to possible sizes)
	LayoutRowCol(m_pColInfo, m_nCols, rectInside.right, CX_SPLITTER_GAP);
	LayoutRowCol(m_pRowInfo, m_nRows, rectInside.bottom, CY_SPLITTER_GAP);

	// adjust the panes (and optionally scroll bars)

	// give the hint for the maximum number of HWNDs
	HDWP hDWP = ::BeginDeferWindowPos((m_nCols + 1) * (m_nRows + 1));

	// reposition size box
	if (m_bHasHScroll && m_bHasVScroll)
	{
		CWnd* pScrollBar = GetDlgItem(AFX_IDW_SIZE_BOX);
		ASSERT(pScrollBar != NULL);
		hDWP = ::DeferWindowPos(hDWP,
			pScrollBar->m_hWnd, NULL,
			rectInside.right, rectInside.bottom,
			afxData.cxVScroll, afxData.cyHScroll,
			SWP_NOZORDER | SWP_NOACTIVATE);
	}

	// reposition scroll bars
	if (m_bHasHScroll)
	{
		int ySB = rectInside.bottom + CY_BORDER;
		int x = 0;
		for (int col = 0; col < m_nCols; col++)
		{
			CWnd* pScrollBar = GetDlgItem(AFX_IDW_HSCROLL_FIRST + col);
			ASSERT(pScrollBar != NULL);
			int cx = m_pColInfo[col].nCurSize;
			if (col == 0 && m_nCols < m_nMaxCols)
			{
				x += m_cxSplitter + CX_BORDER;
				cx -= m_cxSplitter + CX_BORDER;
			}
			hDWP = DeferClientPos(hDWP, pScrollBar, x, ySB,
				cx, afxData.cyHScroll - 2 * CY_BORDER, TRUE);
			x += cx + CX_SPLITTER_GAP;
		}
	}

	if (m_bHasVScroll)
	{
		int xSB = rectInside.right + CX_BORDER;
		int y = 0;
		for (int row = 0; row < m_nRows; row++)
		{
			CWnd* pScrollBar = GetDlgItem(AFX_IDW_VSCROLL_FIRST + row);
			ASSERT(pScrollBar != NULL);
			int cy = m_pRowInfo[row].nCurSize;
			if (row == 0 && m_nRows < m_nMaxRows)
			{
				y += m_cySplitter + CY_BORDER;
				cy -= m_cySplitter + CY_BORDER;
			}
			hDWP = DeferClientPos(hDWP, pScrollBar, xSB, y,
				afxData.cxVScroll - 2 * CX_BORDER, cy, TRUE);
			y += cy + CY_SPLITTER_GAP;
		}
	}

	//BLOCK: Reposition all the panes
	{
		int x = 0;
		for (int col = 0; col < m_nCols; col++)
		{
			int cx = m_pColInfo[col].nCurSize;
			int y = 0;
			for (int row = 0; row < m_nRows; row++)
			{
				int cy = m_pRowInfo[row].nCurSize;
				hDWP = DeferClientPos(hDWP, GetPane(row, col), x, y, cx, cy,
					FALSE);
				y += cy + CY_SPLITTER_GAP;
			}
			x += cx + CX_SPLITTER_GAP;
		}
	}

	// move them all
	if (hDWP == NULL)
	{
		TRACE0("Warning: low system resources, "
			"cannot reposition splitter panes\n");
	}
	::EndDeferWindowPos(hDWP);

	DrawAllSplitBars(NULL, rectInside.right, rectInside.bottom);
			// NULL pDC just invalidates
}

void CSplitterWnd::DrawAllSplitBars(CDC* pDC, int cxInside, int cyInside)
{
	ASSERT_VALID(this);

	CRect rect;
	GetClientRect(rect);
	for (int col = 0; col < m_nCols - 1; col++)
	{
		rect.left += m_pColInfo[col].nCurSize + CX_BORDER;
		rect.right = rect.left + m_cxSplitter;
		if (rect.right > cxInside)
			break;      // stop if not fully visible
		OnDrawSplitter(pDC, splitBar, rect);
		rect.left = rect.right + CX_BORDER;
	}

	GetClientRect(rect);
	for (int row = 0; row < m_nRows - 1; row++)
	{
		rect.top += m_pRowInfo[row].nCurSize + CY_BORDER;
		rect.bottom = rect.top + m_cySplitter;
		if (rect.bottom > cyInside)
			break;      // stop if not fully visible
		OnDrawSplitter(pDC, splitBar, rect);
		rect.top = rect.bottom + CY_BORDER;
	}
}

void CSplitterWnd::OnPaint()
{
	ASSERT_VALID(this);
	CPaintDC dc(this);

	CRect rectClient;
	GetClientRect(&rectClient);
	CRect rectInside;
	GetInsideRect(rectInside);

	// draw the splitter boxes
	if (m_bHasVScroll && m_nRows < m_nMaxRows)
	{
		OnDrawSplitter(&dc, splitBox,
			CRect(rectInside.right + CX_BORDER, 0,
				rectClient.right, m_cySplitter));
	}

	if (m_bHasHScroll && m_nCols < m_nMaxCols)
	{
		OnDrawSplitter(&dc, splitBox,
			CRect(0, rectInside.bottom + CY_BORDER,
				m_cxSplitter, rectClient.bottom));
	}

	DrawAllSplitBars(&dc, rectInside.right, rectInside.bottom);

	dc.IntersectClipRect(rectInside);
	// draw splitter intersections (inside only)
	CRect rect;
	rect.top = 0;
	for (int row = 0; row < m_nRows - 1; row++)
	{
		rect.top += m_pRowInfo[row].nCurSize + CX_BORDER;
		rect.bottom = rect.top + m_cySplitter;
		rect.left = 0;
		for (int col = 0; col < m_nCols - 1; col++)
		{
			rect.left += m_pColInfo[col].nCurSize + CY_BORDER;
			rect.right = rect.left + m_cxSplitter;
			OnDrawSplitter(&dc, splitIntersection, rect);
			rect.left = rect.right + CY_BORDER;
		}
		rect.top = rect.bottom + CX_BORDER;
	}
}

BOOL CSplitterWnd::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
	if (pWnd == this && !m_bTracking)
		return TRUE;    // we will handle it in the mouse move
	return CWnd::OnSetCursor(pWnd, nHitTest, message);
}

void CSplitterWnd::OnMouseMove(UINT /*nFlags*/, CPoint pt)
{
	// cache of last needed cursor
#ifndef _AFXDLL
	static HCURSOR NEAR hcurLast;
	static HCURSOR NEAR hcurDestroy;
	static UINT NEAR idcPrimaryLast;    // store the primary IDC
#else
	#define hcurLast    _AfxGetAppData()->hcurSplitLast
	#define hcurDestroy _AfxGetAppData()->hcurSplitDestroy
	#define idcPrimaryLast _AfxGetAppData()->idcSplitPrimaryLast
#endif

	if (m_bTracking)
	{
		pt.Offset(m_ptTrackOffset); // pt is the upper right of hit detect
		// limit the point to the valid split range
		if (pt.y < m_rectLimit.top)
			pt.y = m_rectLimit.top;
		else if (pt.y > m_rectLimit.bottom)
			pt.y = m_rectLimit.bottom;
		if (pt.x < m_rectLimit.left)
			pt.x = m_rectLimit.left;
		else if (pt.x > m_rectLimit.right)
			pt.x = m_rectLimit.right;

		if (m_htTrack == vSplitterBox ||
			m_htTrack >= vSplitterBar1 && m_htTrack <= vSplitterBar15)
		{
			OnInvertTracker(m_rectTracker);
			m_rectTracker.top = pt.y;
			m_rectTracker.bottom = pt.y + m_cySplitter;
			OnInvertTracker(m_rectTracker);
		}
		else if (m_htTrack == hSplitterBox ||
			m_htTrack >= hSplitterBar1 && m_htTrack <= hSplitterBar15)
		{
			OnInvertTracker(m_rectTracker);
			m_rectTracker.left = pt.x;
			m_rectTracker.right = pt.x + m_cxSplitter;
			OnInvertTracker(m_rectTracker);
		}
		else if (m_htTrack == bothSplitterBox ||
		   (m_htTrack >= splitterIntersection1 &&
			m_htTrack <= splitterIntersection225))
		{
			OnInvertTracker(m_rectTracker);
			m_rectTracker.top = pt.y;
			m_rectTracker.bottom = pt.y + m_cySplitter;
			OnInvertTracker(m_rectTracker);

			OnInvertTracker(m_rectTracker2);
			m_rectTracker2.left = pt.x;
			m_rectTracker2.right = pt.x + m_cxSplitter;
			OnInvertTracker(m_rectTracker2);
		}
	}
	else
	{
		int ht = HitTest(pt);
		UINT idcPrimary;        // app supplied cursor
		LPCSTR idcSecondary;    // system supplied cursor (MAKEINTRESOURCE)

		if (ht == vSplitterBox ||
			ht >= vSplitterBar1 && ht <= vSplitterBar15)
		{
			idcPrimary = AFX_IDC_VSPLITBAR;
			idcSecondary = IDC_SIZENS;
		}
		else if (ht == hSplitterBox ||
			ht >= hSplitterBar1 && ht <= hSplitterBar15)
		{
			idcPrimary = AFX_IDC_HSPLITBAR;
			idcSecondary = IDC_SIZEWE;
		}
		else if (ht == bothSplitterBox ||
			(ht >= splitterIntersection1 && ht <= splitterIntersection225))
		{
			idcPrimary = AFX_IDC_SMALLARROWS;
			idcSecondary = IDC_SIZE;
		}
		else
		{
			SetCursor(afxData.hcurArrow);
			idcPrimary = 0;     // don't use it
			idcSecondary = 0;   // don't use it
		}

		if (idcPrimary != 0)
		{
			HCURSOR hcurToDestroy = NULL;
			if (idcPrimary != idcPrimaryLast || hcurLast == NULL)
			{
				HINSTANCE hInst = AfxFindResourceHandle(
					MAKEINTRESOURCE(idcPrimary), RT_GROUP_CURSOR);

				// load in another cursor
				hcurToDestroy = hcurDestroy;
				if ((hcurDestroy = hcurLast =
				   ::LoadCursor(hInst, MAKEINTRESOURCE(idcPrimary))) == NULL)
				{
					// will not look as good
					TRACE0("Warning: Could not find splitter cursor -"
					  " using system provided alternative\n");

					ASSERT(hcurDestroy == NULL);    // will not get destroyed
					hcurLast = ::LoadCursor(NULL, idcSecondary);
					ASSERT(hcurLast != NULL);
				}
				idcPrimaryLast = idcPrimary;
			}
			ASSERT(hcurLast != NULL);
			::SetCursor(hcurLast);
			ASSERT(hcurLast != hcurToDestroy);
			if (hcurToDestroy != NULL)
				::DestroyCursor(hcurToDestroy); // destroy after being set
		}
	}
}

void CSplitterWnd::OnLButtonDown(UINT /*nFlags*/, CPoint pt)
{
	StartTracking(HitTest(pt));
}

void CSplitterWnd::OnLButtonDblClk(UINT /*nFlags*/, CPoint pt)
{
	int ht = HitTest(pt);
	CRect rect;

	StopTracking(FALSE);

	if ((GetStyle() & SPLS_DYNAMIC_SPLIT) == 0)
		return;     // do nothing if layout is static

	if (ht == vSplitterBox)
	{
		// half split
		SplitRow(m_pRowInfo[0].nCurSize / 2);
	}
	else if (ht == hSplitterBox)
	{
		// half split
		SplitColumn(m_pColInfo[0].nCurSize / 2);
	}
	else if (ht >= vSplitterBar1 && ht <= vSplitterBar15)
	{
		// delete the row
		DeleteRow(ht - vSplitterBar1);
	}
	else if (ht >= hSplitterBar1 && ht <= hSplitterBar15)
	{
		DeleteColumn(ht - hSplitterBar1);
	}
	else if (ht >= splitterIntersection1 && ht <= splitterIntersection225)
	{
		int row = (ht - splitterIntersection1) / 15;
		int col = (ht - splitterIntersection1) % 15;
		DeleteRow(row);
		DeleteColumn(col);
	}
}

void CSplitterWnd::OnLButtonUp(UINT /*nFlags*/, CPoint /*pt*/)
{
	StopTracking(TRUE);
}


void CSplitterWnd::OnCancelMode()
{
	StopTracking(FALSE);
}

void CSplitterWnd::OnKeyDown(UINT nChar, UINT /*nRepCnt*/, UINT /*nFlags*/)
{
	CPoint pt;
	int cz;

	GetCursorPos(&pt);
	cz = GetKeyState(VK_CONTROL) < 0 ? 1 : 16;

	switch (nChar)
	{
	case VK_ESCAPE:
		StopTracking(FALSE);
		break;

	case VK_RETURN:
		StopTracking(TRUE);
		break;

	case VK_LEFT:
		SetCursorPos(pt.x - cz, pt.y);
		break;

	case VK_RIGHT:
		SetCursorPos(pt.x + cz, pt.y);
		break;

	case VK_UP:
		SetCursorPos(pt.x, pt.y - cz);
		break;

	case VK_DOWN:
		SetCursorPos(pt.x, pt.y + cz);
		break;

	default:
		Default();  // pass other keys through
		break;
	}
}

/////////////////////////////////////////////////////////////////////////////
// Scroll messages

void CSplitterWnd::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	ASSERT(pScrollBar != NULL);
	int col = _AfxGetDlgCtrlID(pScrollBar->m_hWnd) - AFX_IDW_HSCROLL_FIRST;
	ASSERT(col >= 0 && col < m_nMaxCols);

	ASSERT(m_nRows > 0);
	int nOldPos = pScrollBar->GetScrollPos();
#ifdef _DEBUG
	int nNewPos;
#endif
	for (int row = 0; row < m_nRows; row++)
	{
		GetPane(row, col)->SendMessage(WM_HSCROLL, nSBCode,
			MAKELONG(nPos, pScrollBar->m_hWnd));
#ifdef _DEBUG
		if (row == 0)
			nNewPos = pScrollBar->GetScrollPos();
		if (pScrollBar->GetScrollPos() != nNewPos)
		{
			TRACE0("Warning: scroll panes setting different scroll positions\n");
			// stick with the last one set
		}
#endif //_DEBUG
		// set the scroll pos to the value it was originally for the next pane
		if (row < m_nRows - 1)
			pScrollBar->SetScrollPos(nOldPos, FALSE);
	}
}

void CSplitterWnd::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	ASSERT(pScrollBar != NULL);
	int row = _AfxGetDlgCtrlID(pScrollBar->m_hWnd) - AFX_IDW_VSCROLL_FIRST;
	ASSERT(row >= 0 && row < m_nMaxRows);

	ASSERT(m_nCols > 0);
	int nOldPos = pScrollBar->GetScrollPos();
#ifdef _DEBUG
	int nNewPos;
#endif
	for (int col = 0; col < m_nCols; col++)
	{
		GetPane(row, col)->SendMessage(WM_VSCROLL, nSBCode,
			MAKELONG(nPos, pScrollBar->m_hWnd));
#ifdef _DEBUG
		if (col == 0)
			nNewPos = pScrollBar->GetScrollPos();
		if (pScrollBar->GetScrollPos() != nNewPos)
		{
			TRACE0("Warning: scroll panes setting different scroll positions\n");
			// stick with the last one set
		}
#endif //_DEBUG
		// set the scroll pos to the value it was originally for the next pane
		if (col < m_nCols - 1)
			pScrollBar->SetScrollPos(nOldPos, FALSE);
	}
}

// syncronized scrolling
BOOL CSplitterWnd::DoScroll(CView* pViewFrom, UINT nScrollCode, BOOL bDoScroll)
{
	ASSERT_VALID(pViewFrom);

	int rowFrom, colFrom;
	if (!IsChildPane(pViewFrom, rowFrom, colFrom))
		return FALSE;

	BOOL bResult = FALSE;

	// save original positions
	int nOldVert;
	CScrollBar* pScrollVert = pViewFrom->GetScrollBarCtrl(SB_VERT);
	if (pScrollVert != NULL)
		nOldVert = pScrollVert->GetScrollPos();
	int nOldHorz;
	CScrollBar* pScrollHorz = pViewFrom->GetScrollBarCtrl(SB_HORZ);
	if (pScrollHorz != NULL)
		nOldHorz = pScrollHorz->GetScrollPos();

	// scroll the view from which the message is from
	if (pViewFrom->OnScroll(nScrollCode, 0, bDoScroll))
		bResult = TRUE;

	if (pScrollVert != NULL)
	{
#ifdef _DEBUG
		int nNewVert = pScrollVert->GetScrollPos();
#endif
		// scroll related columns
		for (int col = 0; col < m_nCols; col++)
		{
			if (col == colFrom)
				continue;

			// set the scroll pos to the value it was originally
			pScrollVert->SetScrollPos(nOldVert, FALSE);

			// scroll the pane
			CView* pView = (CView*)GetPane(rowFrom, col);
			ASSERT(pView->IsKindOf(RUNTIME_CLASS(CView)));
			ASSERT(pView != pViewFrom);
			if (pView->OnScroll(MAKEWORD(-1, HIBYTE(nScrollCode)), 0,
				bDoScroll))
			{
				bResult = TRUE;
			}

#ifdef _DEBUG
			if (pScrollVert->GetScrollPos() != nNewVert)
			{
				TRACE0("Warning: scroll panes setting different scroll positions\n");
				// stick with the last one set
			}
#endif
		}
	}

	if (pScrollHorz != NULL)
	{
#ifdef _DEBUG
		int nNewHorz = pScrollHorz->GetScrollPos();
#endif
		// scroll related rows
		for (int row = 0; row < m_nRows; row++)
		{
			if (row == rowFrom)
				continue;

			// set the scroll pos to the value it was originally
			pScrollHorz->SetScrollPos(nOldHorz, FALSE);

			// scroll the pane
			CView* pView = (CView*)GetPane(row, colFrom);
			ASSERT(pView->IsKindOf(RUNTIME_CLASS(CView)));
			ASSERT(pView != pViewFrom);
			if (pView->OnScroll(MAKEWORD(LOBYTE(nScrollCode), -1), 0,
				bDoScroll))
			{
				bResult = TRUE;
			}

#ifdef _DEBUG
			if (pScrollHorz->GetScrollPos() != nNewHorz)
			{
				TRACE0("Warning: scroll panes setting different scroll positions\n");
				// stick with the last one set
			}
#endif
		}
	}

	return bResult;
}

BOOL CSplitterWnd::DoScrollBy(CView* pViewFrom, CSize sizeScroll, BOOL bDoScroll)
{
	int rowFrom, colFrom;
	if (!IsChildPane(pViewFrom, rowFrom, colFrom))
		return FALSE;

	BOOL bResult = FALSE;

	// save original positions
	int nOldVert;
	CScrollBar* pScrollVert = pViewFrom->GetScrollBarCtrl(SB_VERT);
	if (pScrollVert != NULL)
		nOldVert = pScrollVert->GetScrollPos();
	int nOldHorz;
	CScrollBar* pScrollHorz = pViewFrom->GetScrollBarCtrl(SB_HORZ);
	if (pScrollHorz != NULL)
		nOldHorz = pScrollHorz->GetScrollPos();

	// scroll the view from which the message is from
	if (pViewFrom->OnScrollBy(sizeScroll, bDoScroll))
		bResult = TRUE;

	if (pScrollVert != NULL)
	{
#ifdef _DEBUG
		int nNewVert = pScrollVert->GetScrollPos();
#endif
		// scroll related columns
		for (int col = 0; col < m_nCols; col++)
		{
			if (col == colFrom)
				continue;

			// set the scroll pos to the value it was originally for the next pane
			pScrollVert->SetScrollPos(nOldVert, FALSE);

			// scroll the pane
			CView* pView = (CView*)GetPane(rowFrom, col);
			ASSERT(pView->IsKindOf(RUNTIME_CLASS(CView)));
			ASSERT(pView != pViewFrom);
			if (pView->OnScrollBy(CSize(0, sizeScroll.cy), bDoScroll))
				bResult = TRUE;

#ifdef _DEBUG
			if (pScrollVert->GetScrollPos() != nNewVert)
			{
				TRACE0("Warning: scroll panes setting different scroll positions\n");
				// stick with the last one set
			}
#endif
		}
	}

	if (pScrollHorz != NULL)
	{
#ifdef _DEBUG
	int nNewHorz = pScrollHorz->GetScrollPos();
#endif
		// scroll related rows
		for (int row = 0; row < m_nRows; row++)
		{
			if (row == rowFrom)
				continue;

			// set the scroll pos to the value it was originally for the next pane
			pScrollHorz->SetScrollPos(nOldHorz, FALSE);

			// scroll the pane
			CView* pView = (CView*)GetPane(row, colFrom);
			ASSERT(pView->IsKindOf(RUNTIME_CLASS(CView)));
			ASSERT(pView != pViewFrom);
			if (pView->OnScrollBy(CSize(sizeScroll.cx, 0), bDoScroll))
				bResult = TRUE;

#ifdef _DEBUG
			if (pScrollHorz->GetScrollPos() != nNewHorz)
			{
				TRACE0("Warning: scroll panes setting different scroll positions\n");
				// stick with the last one set
			}
#endif
		}
	}

	return bResult;
}

/////////////////////////////////////////////////////////////////////////////
// Focus control and control over the current pane/child

BOOL CSplitterWnd::CanActivateNext(BOOL)
{
	ASSERT_VALID(this);

	ASSERT(GetParentFrame() != NULL);
	if (GetParentFrame()->GetActiveView() == NULL)
	{
		TRACE0("Warning: Can't go to next pane - there is no current pane\n");
		return FALSE;
	}
	ASSERT(m_nRows != 0);
	ASSERT(m_nCols != 0);
	// if more than 1x1 we can go to the next or prev pane
	return (m_nRows > 1) || (m_nCols > 1);
}

void CSplitterWnd::ActivateNext(BOOL bPrev)
{
	ASSERT_VALID(this);

	CFrameWnd* pFrame = GetParentFrame();
	ASSERT(pFrame != NULL);

	CView* pActiveView = pFrame->GetActiveView();
	if (pActiveView == NULL)
	{
		TRACE0("Warning: Cannot go to next pane - there is no current view\n");
		return;
	}
	// find the coordinate of the current pane
	int row, col;
	if (!IsChildPane(pActiveView, row, col))
	{
		TRACE0("Warning: Cannot go to next pane - active view is not a pane\n");
		return;
	}
	ASSERT(row >= 0 && row < m_nRows);
	ASSERT(col >= 0 && col < m_nCols);

	if (bPrev)
	{
		// prev
		if (--col < 0)
		{
			col = m_nCols - 1;
			if (--row < 0)
				row = m_nRows - 1;
		}
	}
	else
	{
		// next
		if (++col >= m_nCols)
		{
			col = 0;
			if (++row >= m_nRows)
				row = 0;
		}
	}

	CWnd* pNextPane = GetPane(row, col);
	if (pNextPane->IsKindOf(RUNTIME_CLASS(CView)))
		pFrame->SetActiveView((CView*)pNextPane);
	else
		TRACE0("Warning: Cannot go to next pane - next pane is not a CView\n");
}

/////////////////////////////////////////////////////////////////////////////
// CSplitterWnd diagnostics

#ifdef _DEBUG
void CSplitterWnd::AssertValid() const
{
	CWnd::AssertValid();
	ASSERT(m_nMaxRows >= 1);
	ASSERT(m_nMaxCols >= 1);
	ASSERT(m_nMaxCols > 1 || m_nMaxRows > 1);       // 1x1 is not permitted
	ASSERT(m_nRows >= 1);
	ASSERT(m_nCols >= 1);
	ASSERT(m_nRows <= m_nMaxRows);
	ASSERT(m_nCols <= m_nMaxCols);
}

void CSplitterWnd::Dump(CDumpContext& dc) const
{
	ASSERT_VALID(this);

	CWnd::Dump(dc);
	if (m_pDynamicViewClass == NULL)
		AFX_DUMP0(dc, "\nm_pDynamicViewClass = NULL");
	else
		AFX_DUMP1(dc, "\nm_pDynamicViewClass = ", m_pDynamicViewClass->m_lpszClassName);
	AFX_DUMP1(dc, "\nm_nMaxRows = ", m_nMaxRows);
	AFX_DUMP1(dc, "\nm_nMaxCols = ", m_nMaxCols);
	AFX_DUMP1(dc, "\nm_nRows = ", m_nRows);
	AFX_DUMP1(dc, "\nm_nCols = ", m_nCols);
	AFX_DUMP1(dc, "\nm_bHasHScroll = ", m_bHasHScroll);
	AFX_DUMP1(dc, "\nm_bHasVScroll = ", m_bHasVScroll);
	AFX_DUMP1(dc, "\nm_cxSplitter = ", m_cxSplitter);
	AFX_DUMP1(dc, "\nm_cySplitter = ", m_cySplitter);
	if (m_bTracking)
	{
		AFX_DUMP1(dc, "\nTRACKING: m_htTrack = ", m_htTrack);
		AFX_DUMP1(dc, "\nm_rectLimit = ", m_rectLimit);
		AFX_DUMP1(dc, "\nm_ptTrackOffset = ", m_ptTrackOffset);
		AFX_DUMP1(dc, "\nm_rectTracker = ", m_rectTracker);
		if (m_bTracking2)
			AFX_DUMP1(dc, "\nm_rectTracker2 = ", m_rectTracker2);
	}
}
#endif

/////////////////////////////////////////////////////////////////////////////
