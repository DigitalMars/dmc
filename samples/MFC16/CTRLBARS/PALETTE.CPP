// palette.cpp : implementation of the Floating tool palette class
//
// This is a part of the Microsoft Foundation Classes C++ library.
// Copyright (C) 1992 Microsoft Corporation
// All rights reserved.
//
// This source code is only intended as a supplement to the
// Microsoft Foundation Classes Reference and Microsoft
// WinHelp documentation provided with the library.
// See these sources for detailed information regarding the
// Microsoft Foundation Classes product.

#include "stdafx.h"
#include "ctrlbars.h"

#include "palette.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

#define CYCAPTION 9     /* height of the caption */

/////////////////////////////////////////////////////////////////////////////
// CMainFrame

BEGIN_MESSAGE_MAP(CPaletteBar, CToolBar)
	//{{AFX_MSG_MAP(CPaletteBar)
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEACTIVATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPaletteBar construction/destruction

CPaletteBar::CPaletteBar()
{
	m_bTrackMove = FALSE;
	m_nColumns = 2;
	m_cyTopBorder = CYCAPTION+2;
	m_cxLeftBorder = 3;
	m_cxRightBorder = 3;
	m_cyBottomBorder = 3;
}

CPaletteBar::~CPaletteBar()
{
}

void CPaletteBar::SetSizes(SIZE sizeButton, SIZE sizeImage, UINT nColumns)
{
	m_nColumns = nColumns;
	RecalcLayout(m_nCount);
	CToolBar::SetSizes(sizeButton, sizeImage);
}

BOOL CPaletteBar::SetButtons(const UINT FAR* lpIDArray,
	int nIDCount, UINT nColumns)
{
	m_nColumns = nColumns;
	RecalcLayout(nIDCount);
	return CToolBar::SetButtons(lpIDArray, nIDCount);
}

BOOL CPaletteBar::Create(CWnd* pOwnerWnd, int x, int y)
{
	ASSERT(pOwnerWnd != NULL);
	m_pOwnerWnd = pOwnerWnd;

	return CreateEx(0, "AfxControlBar", NULL, WS_POPUP,
		x, y, 0, 0, pOwnerWnd->GetSafeHwnd(), NULL, NULL);
}

void CPaletteBar::RecalcLayout(UINT nButtonCount)
{
	SetWindowPos(NULL, 0, 0,
		m_cxLeftBorder + (m_sizeButton.cx-1) * m_nColumns + m_cxRightBorder + 1,
		m_cyTopBorder + m_cyBottomBorder + (m_sizeButton.cy-1) *
		((nButtonCount + m_nColumns - 1) / m_nColumns) + 1,
		SWP_NOZORDER|SWP_NOMOVE);
	Invalidate();
}

/////////////////////////////////////////////////////////////////////////////
// CPaletteBar diagnostics

#ifdef _DEBUG
void CPaletteBar::AssertValid() const
{
	CToolBar::AssertValid();
}

void CPaletteBar::Dump(CDumpContext& dc) const
{
	CToolBar::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CPaletteBar message handlers

void CPaletteBar::DoPaint(CDC* pDC)
{
	CControlBar::DoPaint(pDC);      // draws any borders

	CRect rect;
	GetClientRect(&rect);

	// Draw the frame border
	CBrush brBlack;
	brBlack.CreateSolidBrush(::GetSysColor(COLOR_WINDOWFRAME));
	pDC->FrameRect(rect,&brBlack);
	rect.bottom = CYCAPTION;
	pDC->FrameRect(rect,&brBlack);

	// Fill in the caption color
	CBrush brCaption;
	brCaption.CreateSolidBrush(::GetSysColor(COLOR_ACTIVECAPTION));
	rect.InflateRect(-1, -1);
	pDC->FillRect(rect, &brCaption);

	// We need to initialize the bitmap selection process.
	DrawState ds;
	if (!PrepareDrawButton(ds))
		return;     // something went wrong

	GetClientRect(&rect);
	rect.top = m_cyTopBorder;
	rect.bottom = rect.top + m_sizeButton.cy;

	// Now draw each visible button
	for (int iButton = 0; iButton < m_nCount; )
	{
		rect.left = m_cxLeftBorder;
		for (UINT nCol = 0; nCol < m_nColumns; nCol++, iButton++)
		{
			rect.right = rect.left + m_sizeButton.cx;
			if (pDC->RectVisible(&rect))
			{
				UINT nID, nStyle;
				int iImage;
				GetButtonInfo(iButton, nID, nStyle, iImage);
				DrawButton(pDC->m_hDC, rect.left, rect.top,
					iImage, nStyle);
			}
			rect.left = rect.right - 1; // prepare for overlap
		}
		rect.top = rect.bottom-1;
		rect.bottom = rect.top + m_sizeButton.cy;
	}

	EndDrawButton(ds);
}

void CPaletteBar::OnLButtonDown(UINT nFlags, CPoint point)
{
	if (point.y <= m_cyTopBorder)
	{
		m_bTrackMove = TRUE;
		m_ptMouse = point;
		SetCapture();
		ClientToScreen(&point);
		InvertTracker(point);
		m_ptLast = point;
	}
	else
		CToolBar::OnLButtonDown(nFlags, point);
}

void CPaletteBar::OnMouseMove(UINT nFlags, CPoint point)
{
	if (m_bTrackMove)
	{
		ClientToScreen(&point);
		InvertTracker(m_ptLast);
		m_ptLast = point;
		InvertTracker(m_ptLast);
	}
	else
		CToolBar::OnMouseMove(nFlags, point);
}

void CPaletteBar::InvertTracker(CPoint point)
{
	CRect rectFrame;
	GetWindowRect(&rectFrame);
	CDC dc;
	dc.Attach(::GetDC(NULL));

	dc.PatBlt(point.x-m_ptMouse.x, point.y-m_ptMouse.y, rectFrame.Width(),
			2, PATINVERT);
	dc.PatBlt(point.x-m_ptMouse.x+rectFrame.Width(), point.y-m_ptMouse.y,
			2, rectFrame.Height(), PATINVERT);
	dc.PatBlt(point.x-m_ptMouse.x, point.y-m_ptMouse.y+rectFrame.Height(),
			rectFrame.Width()+2, 2, PATINVERT);
	dc.PatBlt(point.x-m_ptMouse.x, point.y-m_ptMouse.y+2, 2,
			rectFrame.Height()-2, PATINVERT);
	::ReleaseDC(NULL,dc.Detach());
}

void CPaletteBar::OnLButtonUp(UINT nFlags, CPoint point)
{
	if (m_bTrackMove)
	{
		m_bTrackMove=FALSE;
		ReleaseCapture();
		InvertTracker(m_ptLast);
		ClientToScreen(&point);
		SetWindowPos(NULL, point.x-m_ptMouse.x, point.y-m_ptMouse.y,0,0,
				SWP_NOZORDER|SWP_NOSIZE);
		ShowWindow(SW_SHOW);
	}
	else
		CToolBar::OnLButtonUp(nFlags, point);
}

void CPaletteBar::GetItemRect(int nIndex, LPRECT lpRect) const
{
	ASSERT(nIndex >= 0 && nIndex < m_nCount);
	ASSERT(AfxIsValidAddress(lpRect, sizeof(RECT)));

	lpRect->left = m_cxLeftBorder +
			(nIndex - (nIndex / m_nColumns) * m_nColumns) * (m_sizeButton.cx-1);
	lpRect->right = lpRect->left + m_sizeButton.cx;

	lpRect->top = m_cyTopBorder + (nIndex / m_nColumns) * (m_sizeButton.cy-1);
	lpRect->bottom = lpRect->top + m_sizeButton.cy;
}

int CPaletteBar::HitTest(CPoint point)  // in window relative coords
{
	if (point.x < m_cxLeftBorder ||
			point.x >= (int)(m_cxLeftBorder + m_sizeButton.cx * m_nColumns))
		return -1;      // no X hit

	UINT nRows = (m_nCount + m_nColumns - 1) / m_nColumns;

	if (point.y < m_cyTopBorder ||
			point.y >= (int)(m_cyTopBorder + m_sizeButton.cy * nRows))
		return -1;      // no Y hit

	int iButton = ((point.y - m_cyTopBorder) / (m_sizeButton.cy-1) * m_nColumns +
			(point.x - m_cxLeftBorder) / (m_sizeButton.cx-1));
	return ( iButton < m_nCount ) ? iButton : -1;
}

int CPaletteBar::OnMouseActivate(CWnd*, UINT, UINT)
{
	return MA_NOACTIVATE;
}
