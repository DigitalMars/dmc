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
#include <malloc.h>

#ifdef AFX_CORE3_SEG
#pragma code_seg(AFX_CORE3_SEG)
#endif

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

/////////////////////////////////////////////////////////////////////////////

// the CBRS_ style is made up of an alignment style and a draw border style
//  the alignment styles are mutually exclusive
//  the draw border styles may be combined
#define ALIGN_ANY     0xF000
#define ALIGN_LEFT     0x1000
#define ALIGN_TOP      0x2000
#define ALIGN_RIGHT    0x4000
#define ALIGN_BOTTOM   0x8000
#define BORDER_ANY    0x0F00
#define BORDER_LEFT    0x0100
#define BORDER_TOP     0x0200
#define BORDER_RIGHT   0x0400
#define BORDER_BOTTOM  0x0800

/////////////////////////////////////////////////////////////////////////////
// CControlBar

// IMPLEMENT_DYNAMIC for CControlBar is in wincore.cpp for .OBJ granularity reasons

BEGIN_MESSAGE_MAP(CControlBar, CWnd)
	//{{AFX_MSG_MAP(CControlBar)
	ON_WM_PAINT()
	ON_WM_CTLCOLOR()
	ON_MESSAGE(WM_IDLEUPDATECMDUI, OnIdleUpdateCmdUI)
	ON_MESSAGE_VOID(WM_INITIALUPDATE, OnInitialUpdate)
	ON_MESSAGE(WM_SIZEPARENT, OnSizeParent)
	ON_MESSAGE(WM_HELPHITTEST, OnHelpHitTest)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

#ifdef AFX_INIT_SEG
#pragma code_seg(AFX_INIT_SEG)
#endif

CControlBar::CControlBar()
{
	m_sizeFixedLayout.cx = 32767;   // fill available space
	m_sizeFixedLayout.cy = 32767;   // fill available space
	m_nCount = 0;
	m_pData = NULL;
	// set up some default border spacings
	m_cxLeftBorder = 8;
	m_cxDefaultGap = 2;
	m_cyTopBorder = m_cyBottomBorder = 1;
	m_bAutoDelete = FALSE;
	m_hWndOwner = NULL;
	m_nStateFlags = 0;
}

BOOL CControlBar::AllocElements(int nElements, int cbElement)
{
	ASSERT_VALID(this);
	ASSERT(nElements > 0 && cbElement > 0);
	if (m_pData != NULL)
	{
		ASSERT(m_nCount != 0);
		free(m_pData);      // free old data
	}
	else
	{
		// no initialized yet
		ASSERT(m_nCount == 0);
	}

	if ((m_pData = calloc(nElements, cbElement)) == NULL)
		return FALSE;
	m_nCount = nElements;
	return TRUE;
}

#ifdef AFX_CORE3_SEG
#pragma code_seg(AFX_CORE3_SEG)
#endif

CControlBar::~CControlBar()
{
	ASSERT_VALID(this);
	DestroyWindow();    // avoid PostNcDestroy problems
	// free array
	if (m_pData != NULL)
	{
		ASSERT(m_nCount != 0);
		free(m_pData);
	}
}

void CControlBar::PostNcDestroy()
{
	if (m_bAutoDelete)      // Automatic cleanup?
		delete this;
}


/////////////////////////////////////////////////////////////////////////////
// Default control bar processing

BOOL CControlBar::PreTranslateMessage(MSG* pMsg)
{
	ASSERT_VALID(this);
	// act like a modeless dialog
	ASSERT(m_hWnd != NULL);

	// don't translate dialog messages when in Shift+F1 help mode
	CFrameWnd* pFrameWnd = GetTopLevelFrame();
	if (pFrameWnd != NULL && pFrameWnd->m_bHelpMode)
		return FALSE;

	// since 'IsDialogMessage' will eat frame window accelerators,
	//   we call the frame window's PreTranslateMessage first
	CWnd* pOwner = GetOwner();  // always try owner first
	if (pOwner != NULL || (pOwner = GetParentFrame()) != NULL)
	{
		if (pOwner->PreTranslateMessage(pMsg))
			return TRUE;        // eaten by frame accelerator
		// check for parent of the frame in case of MDI
		if ((pOwner = pOwner->GetParentFrame()) != NULL &&
		  pOwner->PreTranslateMessage(pMsg))
			return TRUE;        // eaten by frame accelerator
	}

	// filter both messages to dialog and from children
	if (pMsg->message >= WM_KEYFIRST && pMsg->message <= WM_KEYLAST)
		return ::IsDialogMessage(m_hWnd, pMsg);

	return FALSE;
}

LRESULT CControlBar::WindowProc(UINT nMsg, WPARAM wParam, LPARAM lParam)
{
	ASSERT_VALID(this);

	// parent notification messages are just passed to parent of control bar
	switch (nMsg)
	{
	case WM_COMMAND:
	case WM_DRAWITEM:
	case WM_MEASUREITEM:
	case WM_DELETEITEM:
	case WM_COMPAREITEM:
	case WM_VKEYTOITEM:
	case WM_CHARTOITEM:
	case WM_VBXEVENT:
		return GetOwner()->SendMessage(nMsg, wParam, lParam);
	}
	return CWnd::WindowProc(nMsg, wParam, lParam);
}

LRESULT CControlBar::OnHelpHitTest(WPARAM, LPARAM)
{
	ASSERT_VALID(this);

	UINT nID = _AfxGetDlgCtrlID(m_hWnd);
	if (nID != 0)
		return HID_BASE_CONTROL+nID;
	else
		return 0;
}


void CControlBar::OnPaint()
{
	// background is already filled in grey
	CPaintDC dc(this);
	DoPaint(&dc);       // delegate to paint helper
}

HBRUSH CControlBar::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	LRESULT lResult;
	if (pWnd->SendChildNotifyLastMsg(&lResult))
		return (HBRUSH)lResult;     // eat it

	// force black text on grey background all the time
	if (!GrayCtlColor(pDC->m_hDC, pWnd->GetSafeHwnd(), nCtlColor,
	   afxData.hbrBtnFace, afxData.clrBtnText))
		return (HBRUSH)Default();
	return afxData.hbrBtnFace;
}

LRESULT CControlBar::OnIdleUpdateCmdUI(WPARAM wParam, LPARAM)
{
	if (GetStyle() & WS_VISIBLE)        // ignore if child is invisible
	{
		CFrameWnd* pTarget = (CFrameWnd*)GetOwner();
		if (pTarget == NULL || !pTarget->IsFrameWnd())
			pTarget = GetParentFrame();
		if (pTarget != NULL)
			OnUpdateCmdUI(pTarget, (BOOL)wParam);
	}
	return 0L;
}

void CControlBar::OnInitialUpdate()
{
	// update the indicators before becoming visible
	OnIdleUpdateCmdUI((WPARAM)TRUE, 0L);
}

LRESULT CControlBar::OnSizeParent(WPARAM, LPARAM lParam)
{
	AFX_SIZEPARENTPARAMS FAR* lpLayout = (AFX_SIZEPARENTPARAMS FAR*)lParam;

	ASSERT(CBRS_NOALIGN == 0);
	ASSERT(CBRS_LEFT == (ALIGN_LEFT | BORDER_RIGHT));
	ASSERT(CBRS_RIGHT == (ALIGN_RIGHT | BORDER_LEFT));
	ASSERT(CBRS_TOP == (ALIGN_TOP | BORDER_BOTTOM));
	ASSERT(CBRS_BOTTOM == (ALIGN_BOTTOM | BORDER_TOP));

	// resize and reposition this control bar based on styles
	DWORD dwStyle = GetStyle();

	// handle delay hide/show
	if (m_nStateFlags & (delayHide|delayShow))
	{
		UINT swpFlags = 0;
		if (m_nStateFlags & delayHide)
		{
			ASSERT((m_nStateFlags & delayShow) == 0);
			if (dwStyle & WS_VISIBLE)
				swpFlags = SWP_HIDEWINDOW;
		}
		else
		{
			ASSERT(m_nStateFlags & delayShow);
			if ((dwStyle & WS_VISIBLE) == 0)
				swpFlags = SWP_SHOWWINDOW;
		}
		if (swpFlags != 0)
		{
			// make the window seem visible/hidden
			dwStyle ^= WS_VISIBLE;
			if (lpLayout->hDWP != NULL)
			{
				// clear delay flags
				m_nStateFlags &= ~(delayShow|delayHide);
				// hide/show the window if actually doing layout
				lpLayout->hDWP = ::DeferWindowPos(lpLayout->hDWP, m_hWnd, NULL,
					0, 0, 0, 0, swpFlags|
					SWP_NOMOVE|SWP_NOSIZE|SWP_NOZORDER|SWP_NOACTIVATE);
			}
		}
		else
		{
			// clear delay flags -- window is already in correct state
			m_nStateFlags &= ~(delayShow|delayHide);
		}
	}

	if ((dwStyle & WS_VISIBLE) && (dwStyle & ALIGN_ANY) != 0)
	{
		// align the control bar
		CRect rect;
		rect.CopyRect(&lpLayout->rect);

		CSize sizeAvail = rect.Size();  // maximum size available
		CSize size;     // maximum requested size
		size.cx = min(m_sizeFixedLayout.cx, sizeAvail.cx);
		size.cy = min(m_sizeFixedLayout.cy, sizeAvail.cy);

		if (dwStyle & ALIGN_LEFT)
		{
			// left align (stretch height)
			size.cy = sizeAvail.cy;
			lpLayout->rect.left += size.cx;
		}
		else if (dwStyle & ALIGN_TOP)
		{
			// top align (stretch width)
			size.cx = sizeAvail.cx;
			lpLayout->rect.top += size.cy;
		}
		else if (dwStyle & ALIGN_RIGHT)
		{
			// right align (stretch height)
			size.cy = sizeAvail.cy;
			rect.left = rect.right - size.cx;
			lpLayout->rect.right -= size.cx;
		}
		else if (dwStyle & ALIGN_BOTTOM)
		{
			// bottom align (stretch width)
			size.cx = sizeAvail.cx;
			rect.top = rect.bottom - size.cy;
			lpLayout->rect.bottom -= size.cy;
		}
		else
		{
			ASSERT(FALSE);      // can never happen
		}

		rect.right = rect.left + size.cx;
		rect.bottom = rect.top + size.cy;

		// only resize the window if doing layout and not just rect query
		if (lpLayout->hDWP != NULL)
			_AfxRepositionWindow(lpLayout, m_hWnd, &rect);
	}
	return 0;
}

void CControlBar::DelayHide()
{
	m_nStateFlags &= ~(delayShow|delayHide);
	if (GetStyle() & WS_VISIBLE)
		m_nStateFlags |= delayHide;
}

void CControlBar::DelayShow()
{
	m_nStateFlags &= ~(delayHide|delayShow);
	if ((GetStyle() & WS_VISIBLE) == 0)
		m_nStateFlags |= delayShow;
}

BOOL CControlBar::IsVisible() const
{
	if (m_nStateFlags & delayHide)
		return FALSE;

	if ((m_nStateFlags & delayShow) || (GetStyle() & WS_VISIBLE))
		return TRUE;

	return FALSE;
}

void CControlBar::DoPaint(CDC* pDC)
{
	ASSERT_VALID(this);
	ASSERT_VALID(pDC);

	DWORD dwStyle = GetStyle();
	if (dwStyle & BORDER_ANY)
	{
		// draw border line on sides
		CRect rect;
		GetClientRect(&rect);
		ASSERT(rect.top == 0 && rect.left == 0);
		HBRUSH hbrFrame = CreateSolidBrush(GetSysColor(COLOR_WINDOWFRAME));
		if (hbrFrame != NULL)
		{
			HGDIOBJ hOldBrush = pDC->SelectObject(hbrFrame);
			if (hOldBrush != NULL)
			{
				if (dwStyle & BORDER_LEFT)
					pDC->PatBlt(0, 0, CX_BORDER, rect.bottom, PATCOPY);
				if (dwStyle & BORDER_TOP)
					pDC->PatBlt(0, 0, rect.right, CY_BORDER, PATCOPY);
				if (dwStyle & BORDER_RIGHT)
					pDC->PatBlt(rect.right, 0, -CX_BORDER, rect.bottom,
						PATCOPY);
				if (dwStyle & BORDER_BOTTOM)
					pDC->PatBlt(0, rect.bottom, rect.right, -CY_BORDER,
						PATCOPY);
				pDC->SelectObject(hOldBrush);
			}
			DeleteObject(hbrFrame);
		}
	}
}

// input CRect should be client rectangle size
void CControlBar::CalcInsideRect(CRect& rect) const
{
	ASSERT_VALID(this);
	DWORD dwStyle = GetStyle();

	// adjust for border size
	if (dwStyle & BORDER_LEFT)
		rect.left += CX_BORDER;
	if (dwStyle & BORDER_TOP)
		rect.top += CY_BORDER;
	if (dwStyle & BORDER_RIGHT)
		rect.right -= CX_BORDER;
	if (dwStyle & BORDER_BOTTOM)
		rect.bottom -= CY_BORDER;

	// inset the top and bottom.
	rect.left += m_cxLeftBorder;
	rect.top += m_cyTopBorder;
	rect.bottom -= m_cyBottomBorder;
}


#ifdef _DEBUG
void CControlBar::AssertValid() const
{
	CWnd::AssertValid();
	ASSERT(m_nCount == 0 || m_pData != NULL);
}

void CControlBar::Dump(CDumpContext& dc) const
{
	CWnd::Dump(dc);
	AFX_DUMP1(dc, "\nborder offset = ", m_cxLeftBorder);
	AFX_DUMP1(dc, "\nm_cyTopBorder = ", m_cyTopBorder);
	AFX_DUMP1(dc, "\nm_cyBottomBorder = ", m_cyBottomBorder);
	AFX_DUMP1(dc, "\ndefault gap = ", m_cxDefaultGap);
	AFX_DUMP1(dc, "\nfixed layout size = ", m_sizeFixedLayout);
	AFX_DUMP1(dc, "\n# of non HWND elements = ", m_nCount);
	AFX_DUMP1(dc, "\nm_bAutoDelete = ", m_bAutoDelete);
}
#endif


/////////////////////////////////////////////////////////////////////////////
// CStatusBar creation etc

struct AFX_STATUSPANE
{
	UINT    nID;        // IDC of indicator: 0 => normal text area
	UINT    nStyle;     // style flags (SBPS_*)
	int     cxText;     // width of string area in pixels
						//   on both sides there is a 1 pixel gap and
						//    a one pixel border, making a pane 4 pixels wider
	LPCSTR  lpszText;   // always far strings
};

inline AFX_STATUSPANE* CStatusBar::_GetPanePtr(int nIndex) const
{
	ASSERT(nIndex >= 0 && nIndex < m_nCount);
	ASSERT(m_pData != NULL);
	return ((AFX_STATUSPANE*)m_pData) + nIndex;
}

IMPLEMENT_DYNAMIC(CStatusBar, CControlBar)

#ifdef AFX_INIT_SEG
#pragma code_seg(AFX_INIT_SEG)
#endif

CStatusBar::CStatusBar()
{
	m_hFont = NULL;
	m_cxRightBorder = m_cxDefaultGap;

	if (afxData.hStatusFont == NULL)
	{
		// load status bar font
		LOGFONT logfont;
		memset(&logfont, 0, sizeof(logfont));
		if (!afxData.bDBCS)
		{
			// 10 point height Sans Serif font
			logfont.lfHeight = -MulDiv(10, afxData.cyPixelsPerInch, 72);
			logfont.lfWeight = FW_NORMAL;
			logfont.lfPitchAndFamily = VARIABLE_PITCH | FF_SWISS;
			static char BASED_CODE szFaceName[] = "MS Sans Serif";
			lstrcpy(logfont.lfFaceName, szFaceName);
			afxData.hStatusFont = ::CreateFontIndirect(&logfont);
		}
		if (afxData.hStatusFont == NULL)
		{
			if (!afxData.bDBCS)
				TRACE0("Warning: Using system font for status font\n");
			afxData.hStatusFont = (HFONT)::GetStockObject(SYSTEM_FONT);
		}
	}
}

CStatusBar::~CStatusBar()
{
	// free strings before freeing array of elements
	for (int i = 0; i < m_nCount; i++)
		VERIFY(SetPaneText(i, NULL, FALSE));    // no update
}

BOOL CStatusBar::Create(CWnd* pParentWnd, DWORD dwStyle, UINT nID)
{
	ASSERT_VALID(pParentWnd);   // must have a parent

	dwStyle |= WS_CLIPSIBLINGS;
		// force WS_CLIPSIBLINGS (avoids SetWindowPos bugs)

	// create the HWND
	CRect rect;
	rect.SetRectEmpty();
	if (!CWnd::Create(_afxWndControlBar, NULL, dwStyle, rect, pParentWnd, nID))
		return FALSE;
		// NOTE: Parent must resize itself for control bar to be resized

	// set initial font and calculate bar height
	OnSetFont((WPARAM)afxData.hStatusFont, 0);  // initialize font height etc
	return TRUE;
}

BOOL CStatusBar::SetIndicators(const UINT FAR* lpIDArray, int nIDCount)
{
	ASSERT_VALID(this);
	ASSERT(nIDCount >= 1);  // must be at least one of them
	ASSERT(lpIDArray == NULL ||
		AfxIsValidAddress(lpIDArray, sizeof(UINT) * nIDCount, FALSE));

	// first allocate array for panes and copy initial data
	if (!AllocElements(nIDCount, sizeof(AFX_STATUSPANE)))
		return FALSE;
	ASSERT(nIDCount == m_nCount);

	BOOL bOK = TRUE;
	if (lpIDArray != NULL)
	{
		ASSERT(m_hFont != NULL);        // must have a font !
		CString strText;
		CClientDC dcScreen(NULL);
		HGDIOBJ hOldFont = dcScreen.SelectObject(m_hFont);
		for (int i = 0; i < nIDCount; i++)
		{
			AFX_STATUSPANE* pSBP = _GetPanePtr(i);
			if ((pSBP->nID = *lpIDArray++) != 0)
			{
				if (!strText.LoadString(pSBP->nID))
				{
					TRACE1("Warning: failed to load indicator string 0x%04X\n",
						pSBP->nID);
					bOK = FALSE;
					break;
				}
				pSBP->cxText = dcScreen.GetTextExtent(strText,
						strText.GetLength()).cx;
				ASSERT(pSBP->cxText >= 0);
				if (!SetPaneText(i, strText, FALSE))
				{
					bOK = FALSE;
					break;
				}
			}
			else
			{
				// no indicator (must access via index)
				// default to 1/4 the screen width (first pane is stretchy)
				pSBP->cxText = ::GetSystemMetrics(SM_CXSCREEN) / 4;
				if (i == 0)
					pSBP->nStyle |= (SBPS_STRETCH | SBPS_NOBORDERS);
			}
		}
		dcScreen.SelectObject(hOldFont);
	}
	return bOK;
}

#ifdef AFX_CORE3_SEG
#pragma code_seg(AFX_CORE3_SEG)
#endif

/////////////////////////////////////////////////////////////////////////////
// CStatusBar attribute access

int CStatusBar::CommandToIndex(UINT nIDFind) const
{
	ASSERT_VALID(this);
	AFX_STATUSPANE* pSBP = _GetPanePtr(0);
	for (int i = 0; i < m_nCount; i++, pSBP++)
		if (pSBP->nID == nIDFind)
			return i;
	return -1;
}

UINT CStatusBar::GetItemID(int nIndex) const
{
	ASSERT_VALID(this);
	return _GetPanePtr(nIndex)->nID;
}

void CStatusBar::GetItemRect(int nIndex, LPRECT lpRect) const
{
	ASSERT_VALID(this);
	ASSERT(AfxIsValidAddress(lpRect, sizeof(RECT)));

	// return rectangle containing inset size
	ASSERT(nIndex >= 0 && nIndex < m_nCount);
	AFX_STATUSPANE* pSBP;
	int i;

	CRect rect;
	GetClientRect(rect);
	CalcInsideRect(rect);

	int cxExtra = rect.Width() - m_cxRightBorder + m_cxDefaultGap;
	// first walk through to calculate extra space
	for (i = 0, pSBP = (AFX_STATUSPANE*)m_pData; i < m_nCount; i++, pSBP++)
		cxExtra -= (pSBP->cxText + CX_BORDER * 4 + m_cxDefaultGap);
	// if cxExtra <= 0 then we will not stretch but just clip

	for (i = 0, pSBP = (AFX_STATUSPANE*)m_pData; i < m_nCount; i++, pSBP++)
	{
		ASSERT(pSBP->cxText >= 0);
		int cxText = pSBP->cxText;
		if ((pSBP->nStyle & SBPS_STRETCH) && cxExtra > 0)
		{
			cxText += cxExtra;
			cxExtra = 0;
		}
		rect.right = rect.left + cxText + CX_BORDER * 4;
		if (i == nIndex)
			break;  // stop with correct rectangle (includes border)
		rect.left = rect.right + m_cxDefaultGap;
	}
	ASSERT(i == nIndex);
	*lpRect = rect;
}

inline UINT CStatusBar::_GetPaneStyle(int nIndex) const
{
	return _GetPanePtr(nIndex)->nStyle;
}

void CStatusBar::_SetPaneStyle(int nIndex, UINT nStyle)
{
	AFX_STATUSPANE* pSBP = _GetPanePtr(nIndex);
	if (pSBP->nStyle != nStyle)
	{
		// just change the style of 1 pane, and invalidate it
		pSBP->nStyle = nStyle;
		CRect rect;
		GetItemRect(nIndex, &rect);
		InvalidateRect(rect);
	}
}

void CStatusBar::GetPaneInfo(int nIndex, UINT& nID, UINT& nStyle,
	int& cxWidth) const
{
	ASSERT_VALID(this);

	AFX_STATUSPANE* pSBP = _GetPanePtr(nIndex);
	nID = pSBP->nID;
	nStyle = pSBP->nStyle;
	cxWidth = pSBP->cxText;
}

void CStatusBar::SetPaneInfo(int nIndex, UINT nID, UINT nStyle, int cxWidth)
{
	ASSERT_VALID(this);

	AFX_STATUSPANE* pSBP = _GetPanePtr(nIndex);
	pSBP->nID = nID;
	_SetPaneStyle(nIndex, nStyle);  // single pane invalidate
	if (cxWidth != pSBP->cxText)
	{
		// change width of one pane -> invalidate the entire status bar
		pSBP->cxText = cxWidth;
		Invalidate();
	}
}

void CStatusBar::GetPaneText(int nIndex, CString& s) const
{
	ASSERT_VALID(this);

	AFX_STATUSPANE* pSBP = _GetPanePtr(nIndex);
	s = pSBP->lpszText;
}

BOOL CStatusBar::SetPaneText(int nIndex, LPCSTR lpszNewText, BOOL bUpdate)
{
	ASSERT_VALID(this);

	AFX_STATUSPANE* pSBP = _GetPanePtr(nIndex);
	if (pSBP == NULL)
		return FALSE;

	if (pSBP->lpszText != NULL)
	{
		if (lpszNewText != NULL && lstrcmp(pSBP->lpszText, lpszNewText) == 0)
			return TRUE;        // nothing to change
		_ffree((LPVOID)pSBP->lpszText);
	}

	BOOL bOK = TRUE;
	if (lpszNewText == NULL || *lpszNewText == '\0')
	{
		pSBP->lpszText = NULL;
	}
	else
	{
		pSBP->lpszText = _fstrdup(lpszNewText);
		if (pSBP->lpszText == NULL)
			bOK = FALSE; // old text is lost and replaced by NULL
	}

	if (bUpdate)
	{
		// invalidate the text of the pane - but not the border
		CRect rect;
		GetItemRect(nIndex, &rect);
		rect.InflateRect(-CX_BORDER, -CY_BORDER);
		InvalidateRect(rect);
	}
	return bOK;
}


/////////////////////////////////////////////////////////////////////////////
// CStatusBar implementation

void CStatusBar::DoPaint(CDC* pDC)
{
	ASSERT_VALID(this);
	ASSERT_VALID(pDC);

	CControlBar::DoPaint(pDC);      // draw border
	CRect rect;
	GetClientRect(rect);
	CalcInsideRect(rect);

	AFX_STATUSPANE* pSBP;
	int i;

	ASSERT(m_hFont != NULL);        // must have a font !
	HGDIOBJ hOldFont = pDC->SelectObject(m_hFont);

	int cxExtra = rect.Width() - m_cxRightBorder + m_cxDefaultGap;
	// first walk through to calculate extra space
	for (i = 0, pSBP = (AFX_STATUSPANE*)m_pData; i < m_nCount; i++, pSBP++)
		cxExtra -= (pSBP->cxText + CX_BORDER * 4 + m_cxDefaultGap);
	// if cxExtra <= 0 then we will not stretch but just clip

	for (i = 0, pSBP = (AFX_STATUSPANE*)m_pData; i < m_nCount; i++, pSBP++)
	{
		ASSERT(pSBP->cxText >= 0);
		int cxText = pSBP->cxText;
		if ((pSBP->nStyle & SBPS_STRETCH) && cxExtra > 0)
		{
			cxText += cxExtra;
			cxExtra = 0;
		}
		rect.right = rect.left + cxText + CX_BORDER * 4;
		if (::RectVisible(pDC->m_hDC, &rect))
			DrawStatusText(pDC->m_hDC, rect, pSBP->lpszText, pSBP->nStyle);
		rect.left = rect.right + m_cxDefaultGap;
	}
	pDC->SelectObject(hOldFont);
}

void PASCAL CStatusBar::DrawStatusText(HDC hDC, CRect const& rect,
			LPCSTR lpszText, UINT nStyle)
{
	ASSERT(hDC != NULL);

	HBRUSH hbrHilite = NULL;
	HBRUSH hbrShadow = NULL;
	if (!(nStyle & SBPS_NOBORDERS))
	{
		if (nStyle & SBPS_POPOUT)
		{
			// reverse colors
			hbrHilite = afxData.hbrBtnShadow;
			hbrShadow = afxData.hbrBtnHilite;
		}
		else
		{
			// normal colors
			hbrHilite = afxData.hbrBtnHilite;
			hbrShadow = afxData.hbrBtnShadow;
		}
	}

	// background is already grey
	UINT nOpts = ETO_CLIPPED;
	int nOldMode = SetBkMode(hDC, TRANSPARENT);
	COLORREF crTextColor = SetTextColor(hDC, afxData.clrBtnText);
	COLORREF crBkColor = SetBkColor(hDC, afxData.clrBtnFace);

	// Draw the hilites
	if (hbrHilite != NULL)
	{
		HGDIOBJ hOldBrush = SelectObject(hDC, hbrHilite);
		if (hOldBrush)
		{
			PatBlt(hDC, rect.right, rect.bottom,
				-(rect.Width() - CX_BORDER),
				-CY_BORDER, PATCOPY);
			PatBlt(hDC, rect.right, rect.bottom,
				-CX_BORDER,
				-(rect.Height() - CY_BORDER), PATCOPY);
			SelectObject(hDC, hOldBrush);
		}
	}

	if (hbrShadow != NULL)
	{
		HGDIOBJ hOldBrush = SelectObject(hDC, hbrShadow);
		if (hOldBrush)
		{
			PatBlt(hDC, rect.left, rect.top,
				rect.Width(), CY_BORDER, PATCOPY);
			PatBlt(hDC, rect.left, rect.top,
				CX_BORDER, rect.Height(), PATCOPY);
			SelectObject(hDC, hOldBrush);
		}
	}

	// We need to adjust the rect for the ExtTextOut, and then adjust it back

	// just support left justified text
	if (lpszText != NULL && !(nStyle & SBPS_DISABLED))
	{
		CRect rectText(rect);
		rectText.InflateRect(-2*CX_BORDER, -CY_BORDER);

		// align on bottom (since descent is more important than ascent)
		SetTextAlign(hDC, TA_LEFT | TA_BOTTOM);
		ExtTextOut(hDC, rectText.left, rectText.bottom,
			nOpts, &rectText, lpszText, lstrlen(lpszText), NULL);
	}
}

/////////////////////////////////////////////////////////////////////////////
// CStatusBar message handlers

BEGIN_MESSAGE_MAP(CStatusBar, CControlBar)
	//{{AFX_MSG_MAP(CStatusBar)
	// control messages
	ON_WM_SIZE()
	ON_MESSAGE(WM_SETFONT, OnSetFont)
	ON_MESSAGE(WM_GETFONT, OnGetFont)
	ON_MESSAGE(WM_SETTEXT, OnSetText)
	ON_MESSAGE(WM_GETTEXT, OnGetText)
	ON_MESSAGE(WM_GETTEXTLENGTH, OnGetTextLength)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

void CStatusBar::OnSize(UINT, int, int)
{
	// force repaint on resize (recalculate stretchy)
	Invalidate();
}

LRESULT CStatusBar::OnSetFont(WPARAM wParam, LPARAM)
{
	m_hFont = (HFONT)wParam;
	ASSERT(m_hFont != NULL);

	// recalculate based on font height + borders
	TEXTMETRIC tm;
	// get text metrics of font
	{
		CClientDC dcScreen(NULL);
		HGDIOBJ hOldFont = dcScreen.SelectObject(m_hFont);
		VERIFY(dcScreen.GetTextMetrics(&tm));
		dcScreen.SelectObject(hOldFont);
	}
	CRect rectSize;
	rectSize.SetRectEmpty();
	CalcInsideRect(rectSize);       // will be negative size
	m_sizeFixedLayout.cy = (tm.tmHeight - tm.tmInternalLeading) +
			CY_BORDER*3 /* 1 extra on top, 2 on bottom */ - rectSize.Height();
	ASSERT(m_sizeFixedLayout.cx == 32767);  // max size

	return 0L;      // does not re-draw or invalidate - resize parent instead
}


LRESULT CStatusBar::OnGetFont(WPARAM, LPARAM)
{
	return (LRESULT)(UINT)m_hFont;
}

LRESULT CStatusBar::OnSetText(WPARAM, LPARAM lParam)
{
	return SetPaneText(0, (LPCSTR)lParam) ? 0 : -1;
}

LRESULT CStatusBar::OnGetText(WPARAM wParam, LPARAM lParam)
{
	int nMaxLen = (int)wParam;
	if (nMaxLen == 0)
		return 0;       // nothing copied
	LPSTR lpszDest = (LPSTR)lParam;

	AFX_STATUSPANE* pSBP = _GetPanePtr(0);
	int nLen = (pSBP != NULL && pSBP->lpszText != NULL) ?
		lstrlen(pSBP->lpszText) : 0;
	if (nLen > nMaxLen)
		nLen = nMaxLen - 1;     // number of characters to copy (less term.)
	_fmemcpy(lpszDest, pSBP->lpszText, nLen);
	lpszDest[nLen] = '\0';
	return nLen+1;      // number of bytes copied
}

LRESULT CStatusBar::OnGetTextLength(WPARAM, LPARAM)
{
	AFX_STATUSPANE* pSBP = _GetPanePtr(0);
	return (pSBP != NULL && pSBP->lpszText != NULL) ?
		lstrlen(pSBP->lpszText) : 0;
}

/////////////////////////////////////////////////////////////////////////////
// CStatusBar idle update through CStatusCmdUI class

class CStatusCmdUI : public CCmdUI      // class private to this file !
{
public: // re-implementations only
	virtual void Enable(BOOL bOn);
	virtual void SetCheck(int nCheck);
	virtual void SetText(LPCSTR lpszText);
};

void CStatusCmdUI::Enable(BOOL bOn)
{
	m_bEnableChanged = TRUE;
	CStatusBar* pStatusBar = (CStatusBar*)m_pOther;
	ASSERT(pStatusBar != NULL);
	ASSERT(pStatusBar->IsKindOf(RUNTIME_CLASS(CStatusBar)));
	ASSERT(m_nIndex < m_nIndexMax);

	UINT nNewStyle = pStatusBar->_GetPaneStyle(m_nIndex) & ~SBPS_DISABLED;
	if (!bOn)
		nNewStyle |= SBPS_DISABLED;
	pStatusBar->_SetPaneStyle(m_nIndex, nNewStyle);
}

void CStatusCmdUI::SetCheck(int nCheck) // "checking" will pop out the text
{
	CStatusBar* pStatusBar = (CStatusBar*)m_pOther;
	ASSERT(pStatusBar != NULL);
	ASSERT(pStatusBar->IsKindOf(RUNTIME_CLASS(CStatusBar)));
	ASSERT(m_nIndex < m_nIndexMax);

	UINT nNewStyle = pStatusBar->_GetPaneStyle(m_nIndex) & ~SBPS_POPOUT;
	if (nCheck != 0)
		nNewStyle |= SBPS_POPOUT;
	pStatusBar->_SetPaneStyle(m_nIndex, nNewStyle);
}

void CStatusCmdUI::SetText(LPCSTR lpszText)
{
	ASSERT(m_pOther != NULL);
	ASSERT(m_pOther->IsKindOf(RUNTIME_CLASS(CStatusBar)));
	ASSERT(m_nIndex < m_nIndexMax);

	((CStatusBar*)m_pOther)->SetPaneText(m_nIndex, lpszText);
}

void CStatusBar::OnUpdateCmdUI(CFrameWnd* pTarget, BOOL bDisableIfNoHndler)
{
	CStatusCmdUI state;
	state.m_pOther = this;
	state.m_nIndexMax = (UINT)m_nCount;
	for (state.m_nIndex = 0; state.m_nIndex < state.m_nIndexMax;
	  state.m_nIndex++)
	{
		state.m_nID = _GetPanePtr(state.m_nIndex)->nID;
		state.DoUpdate(pTarget, bDisableIfNoHndler);
	}

	// update the dialog controls added to the status bar
	UpdateDialogControls(pTarget, bDisableIfNoHndler);
}

/////////////////////////////////////////////////////////////////////////////
// CStatusBar diagnostics

#ifdef _DEBUG
void CStatusBar::AssertValid() const
{
	CControlBar::AssertValid();
}

void CStatusBar::Dump(CDumpContext& dc) const
{
	CControlBar::Dump(dc);

	AFX_DUMP1(dc, "\nm_cxRightBorder = ", m_cxRightBorder);
	AFX_DUMP1(dc, "\nm_hFont = ", (UINT)m_hFont);

	if (dc.GetDepth() > 0)
	{
		for (int i = 0; i < m_nCount; i++)
		{
			AFX_DUMP1(dc, "status pane[", i); AFX_DUMP0(dc, "] = { ");
			AFX_DUMP1(dc, "\n\tnID = ", _GetPanePtr(i)->nID);
			AFX_DUMP1(dc, "\n\tnStyle = ", _GetPanePtr(i)->nStyle);
			AFX_DUMP1(dc, "\n\tcxText = ", _GetPanePtr(i)->cxText);
			AFX_DUMP1(dc, "\n\tlpszText = ", _GetPanePtr(i)->lpszText);
			AFX_DUMP0(dc, "\n\t}");
		}
	}
}
#endif //_DEBUG


/////////////////////////////////////////////////////////////////////////////
