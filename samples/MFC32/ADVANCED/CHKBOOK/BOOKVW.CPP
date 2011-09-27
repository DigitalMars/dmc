// bookvw.cpp : implementation of the CBookView class
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
#include "chkbook.h"
#include <stdlib.h>

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNCREATE(CBookView, CRowView)

/////////////////////////////////////////////////////////////////////////////
CBookView::CBookView()
{
}

/////////////////////////////////////////////////////////////////////////////
// Overrides of CView and CRowView

void CBookView::OnUpdate(CView*, LPARAM lHint, CObject* pHint)
{
	// OnUpdate() is called whenever the document has changed and,
	// therefore, the view needs to redisplay some or all of itself.

	if (DYNAMIC_DOWNCAST(CFixedLenRecHint, pHint) != NULL)
	{
		int nRow = LOWORD(lHint);
		UpdateRow(nRow);
	}
	else
	{
		Invalidate();
	}
}

void CBookView::GetRowWidthHeight(CDC* pDC, int& nRowWidth, int& nRowHeight)
{
	TEXTMETRIC tm;
	pDC->GetTextMetrics(&tm);
	nRowWidth = tm.tmAveCharWidth * ROW_WIDTH;
	nRowHeight = tm.tmHeight * 2; // 2 lines of text
}

int CBookView::GetActiveRow()
{
	CChkBookDoc* pDoc = GetDocument();
	return (pDoc->GetActiveCheckNo() - pDoc->GetFirstCheckNo());
}

int CBookView::GetRowCount()
{
	CChkBookDoc* pDoc = GetDocument();
	return (pDoc->GetLastCheckNo() - pDoc->GetFirstCheckNo() + 1);
}

void CBookView::ChangeSelectionNextRow(BOOL bNext)
{
	GetDocument()->ChangeSelectionNextCheckNo(bNext);
}

void CBookView::ChangeSelectionToRow(int nRow)
{
	CChkBookDoc* pDoc = GetDocument();
	pDoc->ChangeSelectionToCheckNo(nRow + pDoc->GetFirstCheckNo());
}

void CBookView::OnDrawRow(CDC* pDC, int nRow, int y, BOOL bSelected)
{
	// Prepare for highlighting or un-highlighting the check, depending
	// on whether it is the currently selected check or not.  And
	// paint the background (behind the text) accordingly.
	CBrush brushBackground;

#ifndef _MAC
	// save colors for drawing selected item on the screen
	COLORREF crOldText = 0;
	COLORREF crOldBackground = 0;
#endif

	if (!pDC->IsPrinting())
	{
#ifndef _MAC
		if (bSelected)
		{
			brushBackground.CreateSolidBrush(::GetSysColor(COLOR_HIGHLIGHT));
			crOldBackground = pDC->SetBkColor(::GetSysColor(COLOR_HIGHLIGHT));
			crOldText = pDC->SetTextColor(::GetSysColor(COLOR_HIGHLIGHTTEXT));
		}
		else
#endif
		{
			brushBackground.CreateSolidBrush(::GetSysColor(COLOR_WINDOW));
		}

		CRect rectSelection;
		pDC->GetClipBox(&rectSelection);
		rectSelection.top = y;
		rectSelection.bottom = y + m_nRowHeight;
		pDC->FillRect(&rectSelection, &brushBackground);
	}

	// Get the data for the specific check.
	DWORD dwCents;
	CString strDate, strPayTo, strMemo;
	int nCheckNo = nRow + GetDocument()->GetFirstCheckNo();

	GetDocument()->GetCheck(nCheckNo, dwCents, strPayTo, strDate, strMemo);

	TEXTMETRIC tm;
	pDC->GetTextMetrics(&tm);

	// Display the check in two lines of text.
	TCHAR num[10];
	// line 1
	wsprintf(num, _T("%u"), nCheckNo);
	pDC->TextOut(CHECKNO_COL*tm.tmAveCharWidth, y, num, _tcslen(num));
	pDC->TextOut(DATE_COL*tm.tmAveCharWidth, y, strDate);
	pDC->TextOut(PAYTO_COL*tm.tmAveCharWidth, y, strPayTo);
	CString strDollarCents;
	strDollarCents = GetDollarsCentsFormatted(dwCents);
	// right justify the check dollar/cent amount
	pDC->TextOut(
		(AMOUNT_COL + AMOUNT_LEN - strDollarCents.GetLength()) * tm.tmAveCharWidth,
		y,
		strDollarCents);
	// line 2
	pDC->TextOut(MEMO_COL*tm.tmAveCharWidth, y + tm.tmHeight, strMemo);

#ifdef _MAC
	// Highlight the selection
	if (!pDC->IsPrinting() && bSelected)
	{
		CRect rectSelection;
		pDC->GetClipBox(&rectSelection);
		rectSelection.top = y;
		rectSelection.bottom = y + m_nRowHeight;
		::HilightRect(pDC->m_hDC, &rectSelection);
	}
#else
	// Restore the DC.
	if (!pDC->IsPrinting() && bSelected)
	{
		pDC->SetBkColor(crOldBackground);
		pDC->SetTextColor(crOldText);
	}
#endif
}
