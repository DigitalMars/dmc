// chartvw.cpp : implementation of the CChartView class
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

#include "resource.h"
#include "chartdoc.h"
#include "chartvw.h"
#include "gridentr.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CVbChartFrame


IMPLEMENT_DYNCREATE(CVbChartFrame, CMDIChildWnd)

BOOL CVbChartFrame::OnCreateClient(LPCREATESTRUCT, CCreateContext* pContext)
{
	CRect rect;
	GetClientRect(&rect);

	CSize size = rect.Size();
	size.cx /= 2;

	BOOL bSuccess;
	bSuccess  = m_Splitter.CreateStatic(this, 1, 2, WS_CHILD|WS_VISIBLE, AFX_IDW_PANE_FIRST);
	bSuccess &= m_Splitter.CreateView(0, 0, RUNTIME_CLASS(CGridEntry), size, pContext);
	bSuccess &= m_Splitter.CreateView(0, 1, RUNTIME_CLASS(CChartView), size, pContext);

	return TRUE;
}

BEGIN_MESSAGE_MAP(CVbChartFrame, CMDIChildWnd)
	//{{AFX_MSG_MAP(CVbChartFrame)
	ON_COMMAND(ID_FILE_PRINT, OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, OnFilePrintPreview)
	ON_UPDATE_COMMAND_UI(IDC_CHARTLIST, OnUpdateChartList)
	ON_CBN_SELCHANGE(IDC_CHARTLIST, OnChartTypeChange)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CVbChartFrame Commands

void CVbChartFrame::OnFilePrint()
{
	((CChartView*)m_Splitter.GetPane(0, 1))->Print();   // Tell Chart to print
}

void CVbChartFrame::OnFilePrintPreview()
{
	((CChartView*)m_Splitter.GetPane(0, 1))->Preview(); // Tell Chart to preview
}

void CVbChartFrame::OnChartTypeChange()
{
	CComboBox* pDrop = (CComboBox*)CWnd::FromHandle(
							(HWND)LOWORD(GetCurrentMessage()->lParam));

	ASSERT(pDrop != NULL);
	ASSERT(pDrop->IsKindOf(RUNTIME_CLASS(CWnd)));

	CChartView* pChart = (CChartView*)m_Splitter.GetPane(0, 1);
	ASSERT(pChart->IsKindOf(RUNTIME_CLASS(CChartView)));

	pChart->m_nChartType = (int)pDrop->GetItemData(pDrop->GetCurSel());
	pChart->Invalidate();
}

void CVbChartFrame::OnUpdateChartList(CCmdUI* pCmdUI)
{
	ASSERT(pCmdUI->m_pOther != NULL);

	CChartView* pChart = (CChartView*)m_Splitter.GetPane(0, 1);
	ASSERT(pChart->IsKindOf(RUNTIME_CLASS(CChartView)));

	// pCmdUI->m_pOther is not the real CWnd, even if it has been attached
	// to a permanent CWnd.  Therefore, compare hWnds, not pointers

	CComboBox* pCombo = (CComboBox*)pCmdUI->m_pOther;
	if (pCombo->m_hWnd == ::GetFocus())
		return;

	for (int i = 0; i < pCombo->GetCount(); i++)
	{
		if ((int)pCombo->GetItemData(i) == pChart->m_nChartType)
		{
			if (pCombo->GetCurSel() != i)
				pCombo->SetCurSel(i);
			return;
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
// CChartView

IMPLEMENT_DYNCREATE(CChartView, CView)

BEGIN_MESSAGE_MAP(CChartView, CView)
	//{{AFX_MSG_MAP(CChartView)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CChartView construction/destruction

CChartView::CChartView()
{
	m_nChartType = CHART_3DVBAR;
	m_pnData = NULL;
}

CChartView::~CChartView()
{
}


// do not update graph if a GridEntry hint.
void CChartView::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
{
	delete [] m_pnData; // invalidate cache
	m_pnData = NULL;

	if (pHint == NULL)
	{
		CView::OnUpdate(pSender, lHint, pHint);
	}
}

/////////////////////////////////////////////////////////////////////////////
// CChartView drawing

void CChartView::OnDraw(CDC* pDC)
{
	CChartDoc* pDoc = GetDocument();

	if (m_pnData == NULL)
		GetDataFromGrid();

	if (m_pnData == NULL)
	{
		pDC->TextOut(10, 10, "No Selection");
		return;
	}

	switch (m_nChartType)
	{
		case CHART_LINE:
			if (m_nDataCols > 1)
				PlotLineChart(pDC);
			else
				pDC->TextOut(10, 10, "No Selection");
			break;

		case CHART_VBAR:
			PlotBarChart(pDC, TRUE);
			break;

		case CHART_HBAR:
			PlotBarChart(pDC, FALSE);
			break;

		case CHART_3DVBAR:
			Plot3DBarChart(pDC, TRUE);
			break;

		case CHART_3DHBAR:
			Plot3DBarChart(pDC, FALSE);
			break;

		case CHART_VGANTT:
			PlotGanttChart(pDC, TRUE);
			break;

		case CHART_HGANTT:
			PlotGanttChart(pDC, FALSE);
			break;

	}
}

void CChartView::GetDataFromGrid()
{
	CSplitterWnd* pParent = (CSplitterWnd*) GetParent();
	ASSERT(pParent->IsKindOf(RUNTIME_CLASS(CSplitterWnd)));

	CGridEntry* pGridView = (CGridEntry*) pParent->GetPane(0, 0);
	ASSERT(pGridView->IsKindOf(RUNTIME_CLASS(CGridEntry)));

	CVBControl* pGrid = pGridView->m_pGrid;

	m_nStartRow    = (int)pGrid->GetNumProperty("SelStartRow");
	m_nStartCol    = (int)pGrid->GetNumProperty("SelStartCol");
	int nSelEndRow = (int)pGrid->GetNumProperty("SelEndRow");
	int nSelEndCol = (int)pGrid->GetNumProperty("SelEndCol");

	CString data = pGrid->GetStrProperty("Clip");
	char *pData = data.GetBuffer(1);    // do not expand buffer unnecessarily

	// Allocate data matrix

	delete [] m_pnData;     // delete old data if extant
	m_pnData = NULL;

	m_nDataRows = nSelEndRow - m_nStartRow + 1;
	m_nDataCols = nSelEndCol - m_nStartCol + 1;

	if (m_nDataRows * m_nDataCols < 2)
		return;

	m_pnData = new int[m_nDataRows * m_nDataCols];

	int* pnData = m_pnData;

	// Convert all of the data in selected region
	for (int row = m_nStartRow; row <= nSelEndRow; row++)
	{
		for (int col = m_nStartCol; col <= nSelEndCol; col++)
		{
			char* pToken = strtok(pData, " \t\r\n");
			pData = NULL;   // continue with same string on future calls

			if (pToken != NULL)
				*pnData = (int) strtol(pToken, NULL, 0);
			else
				*pnData = 0;        // This is bad because data will be skipped

			if (*pnData < 0)        // do not allow negative numbers
				*pnData = 0;        // They mess up the graphs

			pnData++;
		}
	}
}

static int nValidGraduations[] =
{
	1, 2, 5, 10, 20, 50, 100, 200, 500, 1000, 2000, 5000, 0
};


CRect CChartView::GetDisplayRect(CDC* pDC)
{
	CRect rect;

	if (pDC->IsPrinting())
	{
		rect.left = rect.top = 0;
		rect.right  = pDC->GetDeviceCaps(HORZRES);
		rect.bottom = pDC->GetDeviceCaps(VERTRES);
	}
	else
	{
		GetClientRect(&rect);
	}
	return rect;
}

static COLORREF colorTable[] =
{
	RGB(255,   0,   0),     // Red
	RGB(  0, 255,   0),     // Green
	RGB(  0,   0, 255),     // Blue
	RGB(255, 255,   0),     // Yellow
	RGB(255,   0, 255),     // Magenta
	RGB(  0, 255, 255),     // Cyan
	RGB(  0,   0,   0),     // Black
	RGB(128, 128, 128),     // Grey
	RGB(128, 128,   0),     // Brown
	RGB(128,   0, 128),     // Purple
	RGB(  0, 128, 128)      // Aqua
};

int CChartView::GetTickValue(int nMaxValue, int nNumDiv)
{
	// Determine values for vertical ticks
	int nValuePerTick = (nMaxValue + nNumDiv - 1) / nNumDiv;
	for (int i = 0; nValidGraduations[i] > 0; i++)
	{
		if (nValidGraduations[i] >= nValuePerTick)
		{
			return nValidGraduations[i];
		}
	}
	return nValidGraduations[i-1];
}

//
// GetChartMetrics determines several things:
// Returns Character Height
// &sizeGraph contains overall dimensions of the graph (may not be window size)
// &sizePixelsPerTick contains pixels per tick in both directions
// &ptOrigin contains the origin point
//
// Parameters:
//  pDC   - DC for display device
//  nVDiv - Number of vertical Divisions
//  nHDiv - Number of horizontal Divisions
//
int CChartView::GetChartMetrics(CDC* pDC, int nHDiv, int nVDiv,
	CSize& sizeGraph, CSize& sizePixelsPerTick, CPoint& ptOrigin)
{
	CSize sizeText = pDC->GetTextExtent("0", 1);

	// Determine minimum size for graph
	sizeGraph = sizeText;

	// Minimum size of a Division == 5 chars
	// Allow 5 chars for Vertical Captions and 2.5 chars on each side of chart

	sizeGraph.cx *= (5 * nHDiv) + 10;
	sizeGraph.cy *= nVDiv + 3;

	// Grow size if permitted by display area
	CRect rect = GetDisplayRect(pDC);
	if (rect.Width() > sizeGraph.cx)
		sizeGraph.cx = rect.Width();
	if (rect.Height() > sizeGraph.cy)
		sizeGraph.cy = rect.Height();

	sizePixelsPerTick.cx = (sizeGraph.cx - 10 * sizeText.cx) / nHDiv;
	sizePixelsPerTick.cy = (sizeGraph.cy -  3 * sizeText.cy) / nVDiv;

	ptOrigin.x = sizeText.cx * 15 / 2;
	ptOrigin.y = sizePixelsPerTick.cy * nVDiv + sizeText.cy * 3 / 2;

	return sizeText.cy;
}

void CChartView::PlotCaptions(CDC* pDC, CPoint ptOrigin,
			CSize sizePixelsPerTick, int nValue, int nValuePerTick,
			int nNumTicks, int nCharHeight, BOOL bVert, BOOL bAlpha)
{
	char buf[80];

	int nTickSize = (bVert ? sizePixelsPerTick.cx : sizePixelsPerTick.cy) / 30;
	if (nTickSize < 2)
		nTickSize = 2;

	int nPos  = bVert ? ptOrigin.y : ptOrigin.x;
	int nStep = bVert ? -sizePixelsPerTick.cy : sizePixelsPerTick.cx;

	pDC->SetTextAlign(bVert ? TA_RIGHT : TA_CENTER);

	for (int i = 0; i < nNumTicks; i++)
	{
		if (bAlpha)
			wsprintf(buf, "%c", nValue);
		else
			wsprintf(buf, "%d", nValue);

		if (bVert)
			pDC->TextOut(ptOrigin.x - 2 * nTickSize,
										nPos - nCharHeight / 2, buf);
		else
			pDC->TextOut(nPos, ptOrigin.y + nCharHeight / 2, buf);

		nValue += nValuePerTick;
		nPos   += nStep;
	}
}


void CChartView::PlotAxes(CDC* pDC, CPoint ptOrigin, CSize sizePixelsPerTick,
				int nHorzTicks, int nVertTicks, int nDrawTicks)
{
	int nHeight = sizePixelsPerTick.cy * nVertTicks;
	int nWidth  = sizePixelsPerTick.cx * nHorzTicks;

	// Draw Axes
	pDC->MoveTo(ptOrigin);
	pDC->LineTo(ptOrigin.x, ptOrigin.y - nHeight);
	pDC->MoveTo(ptOrigin);
	pDC->LineTo(ptOrigin.x + nWidth, ptOrigin.y);

	if (nDrawTicks & TICKS_VERT)
	{
		// Draw Vertical Ticks
		int nTickSize = (sizePixelsPerTick.cx / 30);
		if (nTickSize < 2)
			nTickSize = 2;

		int nVPos = ptOrigin.y;
		int xLeft  = ptOrigin.x - nTickSize;
		int xRight = ptOrigin.x + nTickSize + 1;

		for (int i = 0; i <= nVertTicks; i++)
		{
			pDC->MoveTo(xLeft, nVPos);
			pDC->LineTo(xRight, nVPos);
			nVPos -= sizePixelsPerTick.cy;
		}
	}

	if (nDrawTicks & TICKS_HORZ)
	{
		// Draw Horizontal Ticks
		int nTickSize = (sizePixelsPerTick.cy / 30);
		if (nTickSize < 2)
			nTickSize = 2;

		int nHPos = ptOrigin.x;
		int yTop    = ptOrigin.y - nTickSize;
		int yBottom = ptOrigin.y + nTickSize + 1;

		for (int i = 0; i <= nHorzTicks; i++)
		{
			pDC->MoveTo(nHPos, yTop);
			pDC->LineTo(nHPos, yBottom);
			nHPos += sizePixelsPerTick.cx;
		}
	}
}

void CChartView::PlotLineChart(CDC* pDC)
{
	int nMaxValue = GetMaxValue();

	int nValuePerTick = GetTickValue(nMaxValue, 5);

	CSize sizeGraph;
	CSize sizePixelsPerTick;
	CPoint ptOrigin;
	int nCharHeight = GetChartMetrics(pDC, m_nDataCols - 1, 5,
								sizeGraph, sizePixelsPerTick, ptOrigin);


	PlotAxes(pDC, ptOrigin, sizePixelsPerTick, m_nDataCols - 1, 5, TICKS_BOTH);

	PlotCaptions(pDC, ptOrigin, sizePixelsPerTick, 0, nValuePerTick,
						6, nCharHeight, TRUE, FALSE);

	PlotCaptions(pDC, ptOrigin, sizePixelsPerTick, 'A' + m_nStartCol - 1, 1,
						m_nDataCols, nCharHeight, FALSE, TRUE);

	int* pnData = m_pnData;
	for (int nRow = 0; nRow < m_nDataRows; nRow++)
	{
		CPen pen(PS_SOLID, 3, colorTable[nRow]);
		CPen* pOldPen = pDC->SelectObject(&pen);

		int nHPos = ptOrigin.x;
		for (int nCol = 0; nCol < m_nDataCols; nCol++)
		{
			int nVPos = MulDiv(*pnData++, sizePixelsPerTick.cy,
												nValuePerTick);
			if (nCol == 0)
				pDC->MoveTo(nHPos, ptOrigin.y - nVPos);
			else
				pDC->LineTo(nHPos, ptOrigin.y - nVPos);
			nHPos += sizePixelsPerTick.cx;
		}
		pDC->SelectObject(pOldPen);
	}
}

void CChartView::PlotBarChart(CDC* pDC, BOOL bVert)
{
	int nMaxValue = GetMaxValue();

	int nValuePerTick = GetTickValue(nMaxValue, 5);

	int nVTicks = bVert ? 5 : m_nDataCols;
	int nHTicks = bVert ? m_nDataCols : 5;

	CSize sizeGraph;
	CSize sizePixelsPerTick;
	CPoint ptOrigin;
	int nCharHeight = GetChartMetrics(pDC, nHTicks, nVTicks,
								sizeGraph, sizePixelsPerTick, ptOrigin);

	PlotAxes(pDC, ptOrigin, sizePixelsPerTick, nHTicks, nVTicks,
							bVert ? TICKS_VERT : TICKS_HORZ);

	PlotCaptions(pDC, ptOrigin, sizePixelsPerTick, 0, nValuePerTick,
						6, nCharHeight, bVert, FALSE);

	// offset Origin to plot column letters in middle of Division
	CPoint ptTemp = ptOrigin;
	if (!bVert)
		ptTemp.y -= sizePixelsPerTick.cy / 2;
	else
		ptTemp.x += sizePixelsPerTick.cx / 2;

	PlotCaptions(pDC, ptTemp, sizePixelsPerTick, 'A' + m_nStartCol - 1, 1,
						m_nDataCols, nCharHeight, !bVert, TRUE);

	// m_nDataRows bars per division, plus 1 bar width for space
	int nDataSetWidth = bVert ? sizePixelsPerTick.cx : sizePixelsPerTick.cy;
	int nBarWidth     = nDataSetWidth / (m_nDataRows + 1);
	int nSpaceWidth   = (nDataSetWidth - (m_nDataRows * nBarWidth)) / 2;

	// in the loop, xPos means distance from origin along Data Set axis
	// not necessarily horizontal position

	for (int nCol = 0; nCol < m_nDataCols; nCol++)
	{
		int xPos = nDataSetWidth * nCol + nSpaceWidth;

		for (int nRow = 0; nRow < m_nDataRows; nRow++)
		{
			int nValue = m_pnData[nRow * m_nDataCols + nCol];
			int yPos = MulDiv(nValue,
						bVert ? sizePixelsPerTick.cy : sizePixelsPerTick.cx,
						nValuePerTick);
			CBrush brush(colorTable[nRow]);
			CBrush* pOldBrush = pDC->SelectObject(&brush);

			if (bVert)
				pDC->Rectangle(ptOrigin.x + xPos, ptOrigin.y - yPos,
							ptOrigin.x + xPos + nBarWidth, ptOrigin.y);
			else
				pDC->Rectangle(ptOrigin.x, ptOrigin.y - xPos - nBarWidth,
							   ptOrigin.x + yPos, ptOrigin.y - xPos);

			pDC->SelectObject(pOldBrush);
			xPos += nBarWidth;
		}
	}
}

void CChartView::Plot3DBarChart(CDC* pDC, BOOL bVert)
{
	int nMaxValue = GetMaxValue();

	int nValuePerTick = GetTickValue(nMaxValue, 5);

	int nVTicks = bVert ? 5 : m_nDataCols;
	int nHTicks = bVert ? m_nDataCols : 5;

	// Tell Chart Metrics that there is an extra division in both directions
	CSize sizeGraph;
	CSize sizePixelsPerTick;
	CPoint ptOrigin;
	int nCharHeight = GetChartMetrics(pDC, nHTicks + 1, nVTicks + 1,
								sizeGraph, sizePixelsPerTick, ptOrigin);

	int nColWidth  = bVert ? sizePixelsPerTick.cx : sizePixelsPerTick.cy;
	int nDivHeight = bVert ? sizePixelsPerTick.cy : sizePixelsPerTick.cx;

	// Determine offsets of each column
	int deltaX = sizePixelsPerTick.cx / m_nDataRows;
	int deltaY = sizePixelsPerTick.cy / m_nDataRows;

	PlotAxes(pDC, ptOrigin, sizePixelsPerTick, nHTicks, nVTicks, TICKS_NONE);
	CPoint ptOffset = ptOrigin;
	ptOffset.x += m_nDataRows * deltaX;
	ptOffset.y -= m_nDataRows * deltaY;
	PlotAxes(pDC, ptOffset, sizePixelsPerTick, nHTicks, nVTicks, TICKS_NONE);

	// Draw top and right edges of back of grid
	int nTop   = ptOffset.y - sizePixelsPerTick.cy * nVTicks;
	int nRight = ptOffset.x + sizePixelsPerTick.cx * nHTicks;
	pDC->MoveTo(ptOrigin.x, nTop + m_nDataRows * deltaY);
	pDC->LineTo(ptOffset.x, nTop);
	pDC->LineTo(nRight, nTop);
	pDC->LineTo(nRight, ptOffset.y);
	pDC->LineTo(nRight - m_nDataRows * deltaX, ptOrigin.y);

	// Draw Value lines
	if (bVert)
	{           // Horizontal lines for vertical bars
		int yPos = 0;
		for (int nTick = 0; nTick < nVTicks; nTick++)
		{
			pDC->MoveTo(ptOrigin.x, ptOrigin.y - yPos);
			pDC->LineTo(ptOffset.x, ptOffset.y - yPos);
			pDC->LineTo(nRight,     ptOffset.y - yPos);
			yPos += sizePixelsPerTick.cy;
		}
	}
	else        // Vertical Lines for Horizontal Bars
	{
		int xPos = 0;
		for (int nTick = 0; nTick < nHTicks; nTick++)
		{
			pDC->MoveTo(ptOrigin.x + xPos, ptOrigin.y);
			pDC->LineTo(ptOffset.x + xPos, ptOffset.y);
			pDC->LineTo(ptOffset.x + xPos, nTop);
			xPos += sizePixelsPerTick.cx;
		}
	}

	PlotCaptions(pDC, ptOrigin, sizePixelsPerTick, 0, nValuePerTick,
						6, nCharHeight, bVert, FALSE);

	// offset Origin to plot column letters in middle of Division
	CPoint ptTemp = ptOrigin;
	if (!bVert)
		ptTemp.y -= sizePixelsPerTick.cy / 2;
	else
		ptTemp.x += sizePixelsPerTick.cx / 2;

	PlotCaptions(pDC, ptTemp, sizePixelsPerTick, 'A' + m_nStartCol - 1, 1,
						m_nDataCols, nCharHeight, !bVert, TRUE);

	// 2/3 of division used for bar--centered in division
	int nDivWidth     = bVert ? sizePixelsPerTick.cx : sizePixelsPerTick.cy;
	int nBarWidth     = nDivWidth * 2 / 3;
	int nSpaceWidth   = nDivWidth / 6;

	// in the loop, xPos means distance from origin along Column axis
	// not necessarily horizontal position

	for (int nRow = m_nDataRows; nRow--;)
	{
		int xPos    = (bVert ? deltaX : deltaY) * nRow + nSpaceWidth;
		int yOffset = (bVert ? deltaY : deltaX) * nRow;

		CBrush brush(colorTable[nRow]);
		CBrush* pOldBrush = pDC->SelectObject(&brush);

		for (int nCol = 0; nCol < m_nDataCols; nCol++)
		{
			int nValue = m_pnData[nRow * m_nDataCols + nCol];
			int yPos = MulDiv(nValue,
						bVert ? sizePixelsPerTick.cy : sizePixelsPerTick.cx,
						nValuePerTick) + yOffset;

			CRect rcFace;

			if (bVert)
			{
				rcFace.left   = ptOrigin.x + xPos;
				rcFace.top    = ptOrigin.y - yPos;
				rcFace.right  = rcFace.left + nBarWidth + 1;
				rcFace.bottom = ptOrigin.y - yOffset;
			}
			else
			{
				rcFace.left   = ptOrigin.x + yOffset;
				rcFace.bottom = ptOrigin.y - xPos;
				rcFace.right  = ptOrigin.x + yPos + 1;
				rcFace.top    = rcFace.bottom - nBarWidth;
			}

			// Paint front face
			pDC->Rectangle(&rcFace);
			rcFace.right--;

			// Paint top face
			CPoint ptArray[4];
			ptArray[0].x = rcFace.left;
			ptArray[0].y = rcFace.top;
			ptArray[1].x = ptArray[0].x + deltaX;
			ptArray[1].y = ptArray[0].y - deltaY;
			ptArray[3].x = rcFace.right;
			ptArray[3].y = rcFace.top;
			ptArray[2].x = ptArray[3].x + deltaX;
			ptArray[2].y = ptArray[3].y - deltaY;
			pDC->Polygon(ptArray, 4);

			// Leave points 2 & 3 the same
			// for right face
			ptArray[0].x = rcFace.right;
			ptArray[0].y = rcFace.bottom;
			ptArray[1].x = ptArray[0].x + deltaX;
			ptArray[1].y = ptArray[0].y - deltaY;
			pDC->Polygon(ptArray, 4);

			xPos += nDivWidth;
		}
		pDC->SelectObject(pOldBrush);
	}
}

void CChartView::PlotGanttChart(CDC* pDC, BOOL bVert)
{
	int nMaxValue = -32768;
	int* pnData = m_pnData;
	for (int nRow = 0; nRow < m_nDataRows; nRow++)
	{
		int nSum = 0;
		for (int nCol = 0; nCol < m_nDataCols; nCol++)
			nSum += *pnData++;
		if (nSum > nMaxValue)
			nMaxValue = nSum;
	}

	int nValuePerTick = GetTickValue(nMaxValue, 5);

	int nVTicks = bVert ? 5 : m_nDataRows;
	int nHTicks = bVert ? m_nDataRows : 5;

	CSize sizeGraph;
	CSize sizePixelsPerTick;
	CPoint ptOrigin;
	int nCharHeight = GetChartMetrics(pDC, nHTicks, nVTicks,
								sizeGraph, sizePixelsPerTick, ptOrigin);

	PlotAxes(pDC, ptOrigin, sizePixelsPerTick, nHTicks, nVTicks,
							bVert ? TICKS_VERT : TICKS_HORZ);

	PlotCaptions(pDC, ptOrigin, sizePixelsPerTick, 0, nValuePerTick,
						6, nCharHeight, bVert, FALSE);

	// offset Origin to plot row numbers in middle of Division
	CPoint ptTemp = ptOrigin;
	if (!bVert)
		ptTemp.y -= sizePixelsPerTick.cy / 2;
	else
		ptTemp.x += sizePixelsPerTick.cx / 2;

	PlotCaptions(pDC, ptTemp, sizePixelsPerTick, m_nStartRow, 1,
						m_nDataRows, nCharHeight, !bVert, FALSE);

	// 2/3 of division used for bar--centered in division
	int nDivWidth     = bVert ? sizePixelsPerTick.cx : sizePixelsPerTick.cy;
	int nBarWidth     = nDivWidth * 2 / 3;
	int nSpaceWidth   = nDivWidth / 6;

	// in the loop, xPos means distance from origin along Data Set axis
	// not necessarily horizontal position

	pnData = m_pnData;
	for (nRow = 0; nRow < m_nDataRows; nRow++)
	{
		int xPos = nDivWidth * nRow + nSpaceWidth;

		int yPos = 0;
		for (int nCol = 0; nCol < m_nDataCols; nCol++)
		{
			int nValue = MulDiv(*pnData++,
						bVert ? sizePixelsPerTick.cy : sizePixelsPerTick.cx,
						nValuePerTick);
			CBrush brush(colorTable[nCol]);
			CBrush* pOldBrush = pDC->SelectObject(&brush);

			if (bVert)
				pDC->Rectangle(ptOrigin.x + xPos, ptOrigin.y - yPos - nValue,
					ptOrigin.x + xPos + nBarWidth, ptOrigin.y - yPos + 1);
			else
				pDC->Rectangle(ptOrigin.x + yPos, ptOrigin.y - xPos - nBarWidth,
					ptOrigin.x + yPos + nValue + 1, ptOrigin.y - xPos);

			pDC->SelectObject(pOldBrush);
			yPos += nValue;
		}
	}
}


int CChartView::GetMaxValue()
{
	// Scan data to determine max value
	int *pnData = m_pnData;
	int nMax    = *pnData;

	for (int i = m_nDataRows*m_nDataCols; i--;)
	{
		if (*pnData > nMax)
			nMax = *pnData;
		pnData++;
	}
	return nMax;
}


/////////////////////////////////////////////////////////////////////////////
// CChartView printing

BOOL CChartView::OnPreparePrinting(CPrintInfo* pInfo)
{
	pInfo->SetMaxPage(1);       // one page printing/preview
	return DoPreparePrinting(pInfo);
}

void CChartView::Print()
{
	OnFilePrint();
}

void CChartView::Preview()
{
	OnFilePrintPreview();
}


/////////////////////////////////////////////////////////////////////////////

void CChartView::OnActivateView(BOOL bActivate, CView*, CView*)
{
	if (bActivate)
	{               // activate sibling
		GetParentFrame()->SetActiveView(
				(CView*)((CSplitterWnd*)GetParent())->GetPane(0, 0));
	}
}

/////////////////////////////////////////////////////////////////////////////
