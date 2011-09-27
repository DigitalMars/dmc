// inputvw.cpp : implementation file
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
#include "viewex.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CInputView

IMPLEMENT_DYNCREATE(CInputView, CFormView)

CInputView::CInputView()
	: CFormView(IDD_INPUTFORM)
{
	//{{AFX_DATA_INIT(CInputView)
	m_strData = "";
	m_iColor = -1;
	//}}AFX_DATA_INIT
}

CInputView::~CInputView()
{
}

void CInputView::OnUpdate(CView*, LPARAM, CObject*)
{
	CMainDoc* pDoc = GetDocument();
	m_strData = pDoc->m_strData;

	if (pDoc->m_colorData == RGB(255, 0, 0))
		m_iColor = 0;
	else if (pDoc->m_colorData == RGB(0, 255, 0))
		m_iColor = 1;
	else if (pDoc->m_colorData == RGB(0, 0, 255))
		m_iColor = 2;
	else
		m_iColor = -1;

TRACE("OnUpdate: m_iColor = %d ($%lx)\n", m_iColor, pDoc->m_colorData);

	UpdateData(FALSE);  // set the data into the controls
}

BOOL CInputView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}


void CInputView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CInputView)
	DDX_Text(pDX, IDC_EDIT1, m_strData);
	DDX_Radio(pDX, IDC_RADIO1, m_iColor);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CInputView, CFormView)
	//{{AFX_MSG_MAP(CInputView)
	ON_EN_CHANGE(IDC_EDIT1, OnDataChange)
	ON_BN_CLICKED(IDC_RADIO1, OnDataChange)
	ON_BN_CLICKED(IDC_RADIO2, OnDataChange)
	ON_BN_CLICKED(IDC_RADIO3, OnDataChange)
	//}}AFX_MSG_MAP
	ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CView::OnFilePrintPreview)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CInputView message handlers

void CInputView::OnDataChange()
{
	if (!UpdateData())
		return;

	CMainDoc* pDoc = GetDocument();
	COLORREF color = RGB(255 * (m_iColor == 0),
						255 * (m_iColor == 1),
						255 * (m_iColor == 2));

	BOOL bUpdate = FALSE;
	if (m_strData != pDoc->m_strData)
	{
		pDoc->m_strData = m_strData;
		bUpdate = TRUE;
	}
	if (color != pDoc->m_colorData)
	{
		pDoc->m_colorData = color;
		bUpdate = TRUE;
	}
	if (bUpdate)
	{
		// if the document stored data then we would call SetModifiedFlag here
		pDoc->UpdateAllViews(this);
	}
}

/////////////////////////////////////////////////////////////////////////////
// CInputView printing

// The following code shows the technique of setting up a CDC to do
//  the appropriate scaling for printing and print preview.
// The actual output HDC is then used to draw the individual controls.
// This technique sends the WM_PAINT message to the control passing
//  the HDC as the 'wParam'.  This is an alternative painting technique
//  that most Windows controls and all VB controls support.
// This technique has many caveats that make it inappropriate for some
//  cases.  If you find the general technique inappropriate, you can
//  add special case code to handle the printing of the troublesome
//  controls.
//  * If you are using non-VBX custom controls, that do not support
//     the WM_PAINT technique, they will not be printed
//  * Some controls may not result in the desired output format
//     since the controls print in the same fashion as they
//     are displayed on the screen.
//  * Some controls assume they are always being displayed on the
//     screen.  This sometimes results in incorrect fonts being
//     used and bitmaps drawn incorrectly.
//  * The implementation of control printing does not support
//     WS_VSCROLL and WS_HSCROLL styles correctly.
//  * Some printer drivers may fault when painting certain controls.

void CInputView::OnPrint(CDC* pDC, CPrintInfo*)
{
	//BLOCK: Set up scale mode
	{
		CClientDC dcScreen(NULL);
		pDC->SetMapMode(MM_ANISOTROPIC);
		// map 1 screen logical inch to 1 printer (/output) logical inch
		pDC->SetWindowExt(dcScreen.GetDeviceCaps(LOGPIXELSX),
				dcScreen.GetDeviceCaps(LOGPIXELSX));
		pDC->SetViewportExt(pDC->GetDeviceCaps(LOGPIXELSX),
				pDC->GetDeviceCaps(LOGPIXELSX));
	}
	// we must also offset the window positions relative to the scroll offset

	// We cheat here since some controls do not paint if they are invisible,
	//   so we temporary make set the appropriate visible bits during preview
	//   mode so the controls think they are visible even though they aren't.
	HWND hWndCheatVisible = NULL;
	if (!IsWindowVisible())
	{
		// walk up to the top until we find the invisible window
		for (HWND hWnd = m_hWnd;
			hWnd != NULL; hWnd = ::GetParent(hWnd))
		{
			ASSERT(hWnd != NULL);
			DWORD dwStyle = ::GetWindowLong(hWnd, GWL_STYLE);
			if ((dwStyle & WS_VISIBLE) == 0)
			{
				::SetWindowLong(hWnd, GWL_STYLE, dwStyle | WS_VISIBLE);
				hWndCheatVisible = hWnd;
				break;
			}
		}
		ASSERT(hWndCheatVisible != NULL);
	}

	CPen pen(PS_SOLID, 1, RGB(0,0,0));  // solid black pen
	CPen* pOldPen = pDC->SelectObject(&pen);
	ASSERT(pDC->GetWindowOrg() == CPoint(0,0));
	PaintChildWindows(m_hWnd, pDC, GetDeviceScrollPosition());
	ASSERT(pDC->GetWindowOrg() == CPoint(0,0));
	pDC->SelectObject(pOldPen);

	if (hWndCheatVisible != NULL)
		::SetWindowLong(hWndCheatVisible, GWL_STYLE,
			::GetWindowLong(hWndCheatVisible, GWL_STYLE) &~ WS_VISIBLE);
}

void CInputView::PaintChildWindows(HWND hWndParent, CDC* pDC, CPoint ptOffset)
{
	for (HWND hWndChild = ::GetTopWindow(hWndParent);
		hWndChild != NULL; hWndChild = ::GetNextWindow(hWndChild, GW_HWNDNEXT))
	{
		CRect rect;
		::GetWindowRect(hWndChild, rect);       // window rect in screen coords
		ScreenToClient(&rect);                  // relative to this view

		HDC hdcOut = pDC->m_hDC;
#ifdef _DEBUG
		CPoint pt = pDC->GetWindowOrg();
		ASSERT(pt.x == 0 && pt.y == 0);
#endif

		DWORD dwStyle = ::GetWindowLong(hWndChild, GWL_STYLE);
		if (dwStyle & (WS_HSCROLL|WS_VSCROLL))
		{
			TRACE("Warning: printing control with scrollbars not supported\n");
		}
		if (dwStyle & WS_BORDER)
		{
			// the only case we special case handle - manually drawn border
			::Rectangle(hdcOut, rect.left, rect.top, rect.right, rect.bottom);
			rect.InflateRect(-1,-1);        // 1 logical pixel
		}

		pDC->SaveDC();
		{
			CPoint pt(ptOffset.x + rect.left, ptOffset.y + rect.top);
			pDC->LPtoDP(&pt);
			pDC->OffsetViewportOrg(pt.x, pt.y);
				// set the viewport origin so that the window origin
				//  can be changed by the control

			// draw it using a non-virtual HDC
			::SendMessage(hWndChild, WM_PAINT, (WPARAM)hdcOut, 0L);
		}
		pDC->RestoreDC(-1);

		if (::GetTopWindow(hWndChild) != NULL)
			PaintChildWindows(hWndChild, pDC, ptOffset);
	}
}

/////////////////////////////////////////////////////////////////////////////
