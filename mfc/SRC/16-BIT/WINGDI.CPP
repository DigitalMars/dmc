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
#include "winhand_.h"

#ifdef AFX_CORE2_SEG
#pragma code_seg(AFX_CORE2_SEG)
#endif

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// Standard exception processing

// resource failure
IMPLEMENT_DYNAMIC(CResourceException, CException)
static CResourceException NEAR simpleResourceException;
void AFXAPI AfxThrowResourceException()
	{ AfxThrow(&simpleResourceException, TRUE); }

// user alert
IMPLEMENT_DYNAMIC(CUserException, CException)
static CUserException NEAR simpleUserException;
void AFXAPI AfxThrowUserException()
	{ AfxThrow(&simpleUserException, TRUE); }

/////////////////////////////////////////////////////////////////////////////
// Diagnostic Output
#ifdef _DEBUG
CDumpContext& AFXAPI operator<<(CDumpContext& dc, SIZE size)
{
	return dc << "(" << size.cx << " x " << size.cy << ")";
}

CDumpContext& AFXAPI operator<<(CDumpContext& dc, POINT point)
{
	return dc << "(" << point.x << ", " << point.y << ")";
}

CDumpContext& AFXAPI operator<<(CDumpContext& dc, const RECT& rect)
{
	return dc << "(L " << rect.left << ", T " << rect.top << ", R " <<
		rect.right << ", B " << rect.bottom << ")";
}
#endif //_DEBUG



/////////////////////////////////////////////////////////////////////////////
// CDC

IMPLEMENT_DYNCREATE(CDC, CObject)

CDC::CDC()
{
	m_hDC = NULL;
	m_hAttribDC = NULL;
	m_bPrinting = FALSE;
}

#ifdef _DEBUG
void CDC::AssertValid() const
{
	CObject::AssertValid();
}

void CDC::Dump(CDumpContext& dc) const
{
	CObject::Dump(dc);
	AFX_DUMP1(dc, " m_hDC = ", (UINT)m_hDC);
	AFX_DUMP1(dc, "\nm_hAttribDC = ", (UINT)m_hAttribDC);
	AFX_DUMP1(dc, "\nm_bPrinting = ", m_bPrinting);
}
#endif //_DEBUG

#ifndef _AFXDLL
static CHandleMap NEAR _afxMapHDC(RUNTIME_CLASS(CDC), offsetof(CDC, m_hDC), 2);
#else
#define _afxMapHDC (*_AfxGetAppData()->appMapHDC)
#endif

void PASCAL CDC::DeleteTempMap()
{
	_afxMapHDC.DeleteTemp();
}


CDC* PASCAL CDC::FromHandle(HDC hDC)
{
	CDC* pDC = (CDC*)_afxMapHDC.FromHandle(hDC);
	ASSERT(pDC == NULL || pDC->m_hDC == hDC);
	return pDC;
}

BOOL CDC::Attach(HDC hDC)
{
	ASSERT(m_hDC == NULL);      // only attach once, detach on destroy
	ASSERT(m_hAttribDC == NULL);    // only attach to an empty DC

	if (hDC == NULL)
		return FALSE;
	_afxMapHDC.SetPermanent(m_hDC = hDC, this);

	SetAttribDC(m_hDC);     // Default to same as output
	return TRUE;
}

HDC CDC::Detach()
{
	HDC hDC;
	if ((hDC = m_hDC) != NULL)
		_afxMapHDC.RemoveHandle(m_hDC);

	ReleaseAttribDC();
	m_hDC = NULL;
	return hDC;
}

BOOL CDC::DeleteDC()
{
	if (m_hDC == NULL)
		return FALSE;

	return ::DeleteDC(Detach());
}

CDC::~CDC()
{
	if (m_hDC != NULL)
		::DeleteDC(Detach());
}


void CDC::SetAttribDC(HDC hDC)  // Set the Attribute DC
{
	m_hAttribDC = hDC;
}

void CDC::SetOutputDC(HDC hDC)  // Set the Output DC
{
#ifdef _DEBUG
	CObject* pObject;
	if (_afxMapHDC.LookupPermanent(m_hDC, pObject) && pObject == this)
	{
		TRACE0("Cannot Set Output hDC on Attached CDC\n");
		ASSERT(FALSE);
	}
#endif
	m_hDC = hDC;
}

void CDC::ReleaseAttribDC()     // Release the Attribute DC
{
	m_hAttribDC = NULL;
}

void CDC::ReleaseOutputDC()     // Release the Output DC
{
#ifdef _DEBUG
	CObject* pObject;
	if (_afxMapHDC.LookupPermanent(m_hDC, pObject) && pObject == this)
	{
		TRACE0("Cannot Release Output hDC on Attached CDC\n");
		ASSERT(FALSE);
	}
#endif
	m_hDC = NULL;
}

// Win 3.0 and 3.1 compatible printing routines

int CDC::StartDoc(LPCSTR lpszDocName)
{
	DOCINFO di;
	di.cbSize = sizeof(DOCINFO);
	di.lpszDocName = lpszDocName;
	di.lpszOutput = NULL;
	return StartDoc(&di);
}

int CDC::StartDoc(LPDOCINFO lpDocInfo)
{
	if (!afxData.bWin31)
	{
		// Output to file not supported in Win3.0
		ASSERT(lpDocInfo->lpszOutput == NULL);
		ASSERT(lstrlen(lpDocInfo->lpszDocName) < 32);
		return Escape(STARTDOC, lstrlen(lpDocInfo->lpszDocName),
			 lpDocInfo->lpszDocName, NULL);
	}
	else
	{
		return ::StartDoc(m_hDC, lpDocInfo);
	}
}

int CDC::StartPage()
{
	if (!afxData.bWin31)
		return TRUE;
	else
		return ::StartPage(m_hDC);
}

int CDC::EndPage()
{
	if (!afxData.bWin31)
		return ::Escape(m_hDC, NEWFRAME, 0, NULL, NULL);
	else
		return ::EndPage(m_hDC);
}

int CDC::SetAbortProc(BOOL (CALLBACK EXPORT* lpfn)(HDC, int))
{
	if (!afxData.bWin31)
		return ::Escape(m_hDC, SETABORTPROC, 0, (LPCSTR)lpfn, NULL);
	else
		return ::SetAbortProc(m_hDC, (ABORTPROC)lpfn);
}

int CDC::AbortDoc()
{
	if (!afxData.bWin31)
		return ::Escape(m_hDC, ABORTDOC, 0, NULL, NULL);
	else
		return ::AbortDoc(m_hDC);
}

int CDC::EndDoc()
{
	if (!afxData.bWin31)
		return ::Escape(m_hDC, ENDDOC, 0, NULL, NULL);
	else
		return ::EndDoc(m_hDC);
}

/////////////////////////////////////////////////////////////////////////////
// Out-of-line routines

int CDC::SaveDC()
{
	ASSERT(m_hDC != NULL);
	int nRetVal;
	if (m_hAttribDC != NULL)
		nRetVal = ::SaveDC(m_hAttribDC);
	if (m_hDC != m_hAttribDC && ::SaveDC(m_hDC) != 0)
		nRetVal = -1;   // -1 is the only valid restore value for complex DCs
	return nRetVal;
}

BOOL CDC::RestoreDC(int nSavedDC)
{
	// if two distinct DCs, nSavedDC can only be -1
	ASSERT(m_hDC != NULL);
	ASSERT(m_hDC == m_hAttribDC || nSavedDC == -1);

	BOOL bRetVal = TRUE;
	if (m_hDC != m_hAttribDC)
		bRetVal = ::RestoreDC(m_hDC, nSavedDC);
	if (m_hAttribDC != NULL)
		bRetVal = (bRetVal && ::RestoreDC(m_hAttribDC, nSavedDC));
	return bRetVal;
}

CGdiObject* PASCAL CDC::SelectGdiObject(HDC hDC, HGDIOBJ h)
{
	return CGdiObject::FromHandle(::SelectObject(hDC, h));
}

CGdiObject* CDC::SelectStockObject(int nIndex)
{
	ASSERT(m_hDC != NULL);

	HGDIOBJ hObject = ::GetStockObject(nIndex);
	HGDIOBJ hOldObj;

	ASSERT(hObject != NULL);
	if (m_hDC != m_hAttribDC)
		hOldObj = ::SelectObject(m_hDC, hObject);
	if (m_hAttribDC != NULL)
		hOldObj = ::SelectObject(m_hAttribDC, hObject);
	return CGdiObject::FromHandle(hOldObj);
}

CPen* CDC::SelectObject(CPen* pPen)
{
	ASSERT(m_hDC != NULL);
	HGDIOBJ hOldObj;
	if (m_hDC != m_hAttribDC)
		hOldObj = ::SelectObject(m_hDC, pPen->GetSafeHandle());
	if (m_hAttribDC != NULL)
		hOldObj = ::SelectObject(m_hAttribDC, pPen->GetSafeHandle());
	return (CPen*)CGdiObject::FromHandle(hOldObj);
}

CBrush* CDC::SelectObject(CBrush* pBrush)
{
	ASSERT(m_hDC != NULL);
	HGDIOBJ hOldObj;
	if (m_hDC != m_hAttribDC)
		hOldObj = ::SelectObject(m_hDC, pBrush->GetSafeHandle());
	if (m_hAttribDC != NULL)
		hOldObj = ::SelectObject(m_hAttribDC, pBrush->GetSafeHandle());
	return (CBrush*)CGdiObject::FromHandle(hOldObj);
}

CFont* CDC::SelectObject(CFont* pFont)
{
	ASSERT(m_hDC != NULL);
	HGDIOBJ hOldObj;
	if (m_hDC != m_hAttribDC)
		hOldObj = ::SelectObject(m_hDC, pFont->GetSafeHandle());
	if (m_hAttribDC != NULL)
		hOldObj = ::SelectObject(m_hAttribDC, pFont->GetSafeHandle());
	return (CFont*)CGdiObject::FromHandle(hOldObj);
}

int CDC::SelectObject(CRgn* pRgn)
{
	ASSERT(m_hDC != NULL);
	int nRetVal;
	if (m_hDC != m_hAttribDC)
		nRetVal = (int)::SelectObject(m_hDC, pRgn->GetSafeHandle());
	if (m_hAttribDC != NULL)
		nRetVal = (int)::SelectObject(m_hAttribDC, pRgn->GetSafeHandle());
	return nRetVal;
}

CPalette* CDC::SelectPalette(CPalette* pPalette, BOOL bForceBackground)
{
	ASSERT(m_hDC != NULL);

	return (CPalette*) CGdiObject::FromHandle(::SelectPalette(m_hDC,
		(HPALETTE)pPalette->GetSafeHandle(), bForceBackground));
}

COLORREF CDC::SetBkColor(COLORREF crColor)
{
	ASSERT(m_hDC != NULL);
	COLORREF crRetVal;
	if (m_hDC != m_hAttribDC)
		crRetVal = ::SetBkColor(m_hDC, crColor);
	if (m_hAttribDC != NULL)
		crRetVal = ::SetBkColor(m_hAttribDC, crColor);
	return crRetVal;
}

int CDC::SetBkMode(int nBkMode)
{
	ASSERT(m_hDC != NULL);
	int nRetVal;
	if (m_hDC != m_hAttribDC)
		nRetVal = ::SetBkMode(m_hDC, nBkMode);
	if (m_hAttribDC != NULL)
		nRetVal = ::SetBkMode(m_hAttribDC, nBkMode);
	return nRetVal;
}

int CDC::SetPolyFillMode(int nPolyFillMode)
{
	ASSERT(m_hDC != NULL);
	int nRetVal;
	if (m_hDC != m_hAttribDC)
		nRetVal = ::SetPolyFillMode(m_hDC, nPolyFillMode);
	if (m_hAttribDC != NULL)
		nRetVal = ::SetPolyFillMode(m_hAttribDC, nPolyFillMode);
	return nRetVal;
}

int CDC::SetROP2(int nDrawMode)
{
	ASSERT(m_hDC != NULL);
	int nRetVal;
	if (m_hDC != m_hAttribDC)
		nRetVal = ::SetROP2(m_hDC, nDrawMode);
	if (m_hAttribDC != NULL)
		nRetVal = ::SetROP2(m_hAttribDC, nDrawMode);
	return nRetVal;
}

int CDC::SetStretchBltMode(int nStretchMode)
{
	ASSERT(m_hDC != NULL);
	int nRetVal;
	if (m_hDC != m_hAttribDC)
		nRetVal = ::SetStretchBltMode(m_hDC, nStretchMode);
	if (m_hAttribDC != NULL)
		nRetVal = ::SetStretchBltMode(m_hAttribDC, nStretchMode);
	return nRetVal;
}

COLORREF CDC::SetTextColor(COLORREF crColor)
{
	ASSERT(m_hDC != NULL);
	COLORREF crRetVal;
	if (m_hDC != m_hAttribDC)
		crRetVal = ::SetTextColor(m_hDC, crColor);
	if (m_hAttribDC != NULL)
		crRetVal = ::SetTextColor(m_hAttribDC, crColor);
	return crRetVal;
}

int CDC::SetMapMode(int nMapMode)
{
	ASSERT(m_hDC != NULL);
	int nRetVal;
	if (m_hDC != m_hAttribDC)
		nRetVal = ::SetMapMode(m_hDC, nMapMode);
	if (m_hAttribDC != NULL)
		nRetVal = ::SetMapMode(m_hAttribDC, nMapMode);
	return nRetVal;
}

CPoint CDC::SetViewportOrg(int x, int y)
{
	ASSERT(m_hDC != NULL);
	CPoint point;
	if (m_hDC != m_hAttribDC)
		point = ::SetViewportOrg(m_hDC, x, y);
	if (m_hAttribDC != NULL)
		point = ::SetViewportOrg(m_hAttribDC, x, y);
	return point;
}

CPoint CDC::OffsetViewportOrg(int nWidth, int nHeight)
{
	ASSERT(m_hDC != NULL);
	CPoint point;
	if (m_hDC != m_hAttribDC)
		point = ::OffsetViewportOrg(m_hDC, nWidth, nHeight);
	if (m_hAttribDC != NULL)
		point = ::OffsetViewportOrg(m_hAttribDC, nWidth, nHeight);
	return point;
}

CSize CDC::SetViewportExt(int x, int y)
{
	ASSERT(m_hDC != NULL);
	CSize size;
	if (m_hDC != m_hAttribDC)
		size = ::SetViewportExt(m_hDC, x, y);
	if (m_hAttribDC != NULL)
		size = ::SetViewportExt(m_hAttribDC, x, y);
	return size;
}

CSize CDC::ScaleViewportExt(int xNum, int xDenom, int yNum, int yDenom)
{
	ASSERT(m_hDC != NULL);
	CSize size;
	if (m_hDC != m_hAttribDC)
		size = ::ScaleViewportExt(m_hDC, xNum, xDenom, yNum, yDenom);
	if (m_hAttribDC != NULL)
		size = ::ScaleViewportExt(m_hAttribDC, xNum, xDenom, yNum, yDenom);
	return size;
}

CPoint CDC::SetWindowOrg(int x, int y)
{
	ASSERT(m_hDC != NULL);
	CPoint point;
	if (m_hDC != m_hAttribDC)
		point = ::SetWindowOrg(m_hDC, x, y);
	if (m_hAttribDC != NULL)
		point = ::SetWindowOrg(m_hAttribDC, x, y);
	return point;
}

CPoint CDC::OffsetWindowOrg(int nWidth, int nHeight)
{
	ASSERT(m_hDC != NULL);
	CPoint point;
	if (m_hDC != m_hAttribDC)
		point = ::OffsetWindowOrg(m_hDC, nWidth, nHeight);
	if (m_hAttribDC != NULL)
		point = ::OffsetWindowOrg(m_hAttribDC, nWidth, nHeight);
	return point;
}

CSize CDC::ScaleWindowExt(int xNum, int xDenom, int yNum, int yDenom)
{
	ASSERT(m_hDC != NULL);
	CSize size;
	if (m_hDC != m_hAttribDC)
		size = ::ScaleWindowExt(m_hDC, xNum, xDenom, yNum, yDenom);
	if (m_hAttribDC != NULL)
		size = ::ScaleWindowExt(m_hAttribDC, xNum, xDenom, yNum, yDenom);
	return size;
}

CSize CDC::SetWindowExt(int x, int y)
{
	ASSERT(m_hDC != NULL);
	CSize size;
	if (m_hDC != m_hAttribDC)
		size = ::SetWindowExt(m_hDC, x, y);
	if (m_hAttribDC != NULL)
		size = ::SetWindowExt(m_hAttribDC, x, y);
	return size;
}


int CDC::GetClipBox(LPRECT lpRect) const
{
	ASSERT(m_hDC != NULL);
	return ::GetClipBox(m_hDC, lpRect);
}

int CDC::SelectClipRgn(CRgn* pRgn)
{
	ASSERT(m_hDC != NULL);
	int nRetVal;
	if (m_hDC != m_hAttribDC)
		nRetVal = ::SelectClipRgn(m_hDC, (HRGN)pRgn->GetSafeHandle());
	if (m_hAttribDC != NULL)
		nRetVal = ::SelectClipRgn(m_hAttribDC, (HRGN)pRgn->GetSafeHandle());
	return nRetVal;
}

int CDC::ExcludeClipRect(int x1, int y1, int x2, int y2)
{
	ASSERT(m_hDC != NULL);
	int nRetVal;
	if (m_hDC != m_hAttribDC)
		nRetVal = ::ExcludeClipRect(m_hDC, x1, y1, x2, y2);
	if (m_hAttribDC != NULL)
		nRetVal = ::ExcludeClipRect(m_hAttribDC, x1, y1, x2, y2);
	return nRetVal;
}

int CDC::ExcludeClipRect(LPCRECT lpRect)
{
	ASSERT(m_hDC != NULL);
	int nRetVal;
	if (m_hDC != m_hAttribDC)
		nRetVal = ::ExcludeClipRect(m_hDC, lpRect->left, lpRect->top,
			lpRect->right, lpRect->bottom);
	if (m_hAttribDC != NULL)
		nRetVal = ::ExcludeClipRect(m_hAttribDC, lpRect->left, lpRect->top,
			lpRect->right, lpRect->bottom);
	return nRetVal;
}

int CDC::IntersectClipRect(int x1, int y1, int x2, int y2)
{
	ASSERT(m_hDC != NULL);
	int nRetVal;
	if (m_hDC != m_hAttribDC)
		nRetVal = ::IntersectClipRect(m_hDC, x1, y1, x2, y2);
	if (m_hAttribDC != NULL)
		nRetVal = ::IntersectClipRect(m_hAttribDC, x1, y1, x2, y2);
	return nRetVal;
}

int CDC::IntersectClipRect(LPCRECT lpRect)
{
	ASSERT(m_hDC != NULL);
	int nRetVal;
	if (m_hDC != m_hAttribDC)
		nRetVal = ::IntersectClipRect(m_hDC, lpRect->left, lpRect->top,
			lpRect->right, lpRect->bottom);
	if (m_hAttribDC != NULL)
		nRetVal = ::IntersectClipRect(m_hAttribDC, lpRect->left, lpRect->top,
			lpRect->right, lpRect->bottom);
	return nRetVal;
}

int CDC::OffsetClipRgn(int x, int y)
{
	ASSERT(m_hDC != NULL);
	int nRetVal;
	if (m_hDC != m_hAttribDC)
		nRetVal = ::OffsetClipRgn(m_hDC, x, y);
	if (m_hAttribDC != NULL)
		nRetVal = ::OffsetClipRgn(m_hAttribDC, x, y);
	return nRetVal;
}

int CDC::OffsetClipRgn(SIZE size)
{
	ASSERT(m_hDC != NULL);
	int nRetVal;
	if (m_hDC != m_hAttribDC)
		nRetVal = ::OffsetClipRgn(m_hDC, size.cx, size.cy);
	if (m_hAttribDC != NULL)
		nRetVal = ::OffsetClipRgn(m_hAttribDC, size.cx, size.cy);
	return nRetVal;
}


CPoint CDC::MoveTo(int x, int y)
{
	ASSERT(m_hDC != NULL);
	CPoint point;
	if (m_hDC != m_hAttribDC)
		point = ::MoveTo(m_hDC, x, y);
	if (m_hAttribDC != NULL)
		point = ::MoveTo(m_hAttribDC, x, y);
	return point;
}


BOOL CDC::LineTo(int x, int y)
{
	ASSERT(m_hDC != NULL);
	if (m_hAttribDC != NULL && m_hDC != m_hAttribDC)
		::MoveTo(m_hAttribDC, x, y);
	return ::LineTo(m_hDC, x, y);
}

UINT CDC::SetTextAlign(UINT nFlags)
{
	ASSERT(m_hDC != NULL);
	UINT nRetVal;
	if (m_hDC != m_hAttribDC)
		::SetTextAlign(m_hDC, nFlags);
	if (m_hAttribDC != NULL)
		nRetVal = ::SetTextAlign(m_hAttribDC, nFlags);
	return nRetVal;
}

int CDC::SetTextJustification(int nBreakExtra, int nBreakCount)
{
	ASSERT(m_hDC != NULL);
	int nRetVal;
	if (m_hDC != m_hAttribDC)
		nRetVal = ::SetTextJustification(m_hDC, nBreakExtra, nBreakCount);
	if (m_hAttribDC != NULL)
		nRetVal = ::SetTextJustification(m_hAttribDC, nBreakExtra, nBreakCount);
	return nRetVal;
}

int CDC::SetTextCharacterExtra(int nCharExtra)
{
	ASSERT(m_hDC != NULL);
	int nRetVal;
	if (m_hDC != m_hAttribDC)
		nRetVal = ::SetTextCharacterExtra(m_hDC, nCharExtra);
	if (m_hAttribDC != NULL)
		nRetVal = ::SetTextCharacterExtra(m_hAttribDC, nCharExtra);
	return nRetVal;
}

DWORD CDC::SetMapperFlags(DWORD dwFlag)
{
	ASSERT(m_hDC != NULL);
	DWORD dwRetVal;
	if (m_hDC != m_hAttribDC)
		dwRetVal = ::SetMapperFlags(m_hDC, dwFlag);
	if (m_hAttribDC != NULL)
		dwRetVal = ::SetMapperFlags(m_hAttribDC, dwFlag);
	return dwRetVal;
}

/////////////////////////////////////////////////////////////////////////////
// Coordinate transforms

void CDC::LPtoDP(LPSIZE lpSize) const
{
	ASSERT(AfxIsValidAddress(lpSize, sizeof(SIZE)));

	CSize sizeWinExt = GetWindowExt();
	CSize sizeVpExt = GetViewportExt();
	lpSize->cx = MulDiv(lpSize->cx, abs(sizeVpExt.cx), abs(sizeWinExt.cx));
	lpSize->cy = MulDiv(lpSize->cy, abs(sizeVpExt.cy), abs(sizeWinExt.cy));
}

void CDC::DPtoLP(LPSIZE lpSize) const
{
	ASSERT(AfxIsValidAddress(lpSize, sizeof(SIZE)));

	CSize sizeWinExt = GetWindowExt();
	CSize sizeVpExt = GetViewportExt();
	lpSize->cx = MulDiv(lpSize->cx, abs(sizeWinExt.cx), abs(sizeVpExt.cx));
	lpSize->cy = MulDiv(lpSize->cy, abs(sizeWinExt.cy), abs(sizeVpExt.cy));
}

/////////////////////////////////////////////////////////////////////////////
// Helper DCs

IMPLEMENT_DYNAMIC(CClientDC, CDC)
IMPLEMENT_DYNAMIC(CWindowDC, CDC)
IMPLEMENT_DYNAMIC(CPaintDC, CDC)

#ifdef _DEBUG
void CClientDC::AssertValid() const
{
	CDC::AssertValid();
	ASSERT(m_hWnd == NULL || ::IsWindow(m_hWnd));
}

void CClientDC::Dump(CDumpContext& dc) const
{
	CDC::Dump(dc);
	AFX_DUMP1(dc, "\nm_hWnd = ", (UINT)m_hWnd);
}
#endif

CClientDC::CClientDC(CWnd* pWnd)
{
	if (!Attach(::GetDC(m_hWnd = pWnd->GetSafeHwnd())))
		AfxThrowResourceException();
}

CClientDC::~CClientDC()
{
	ASSERT(m_hDC != NULL);
	::ReleaseDC(m_hWnd, Detach());
}


#ifdef _DEBUG
void CWindowDC::AssertValid() const
{
	CDC::AssertValid();
	ASSERT(::IsWindow(m_hWnd));
}

void CWindowDC::Dump(CDumpContext& dc) const
{
	CDC::Dump(dc);
	AFX_DUMP1(dc, "\nm_hWnd = ", (UINT)m_hWnd);
}
#endif

CWindowDC::CWindowDC(CWnd* pWnd)
{
	if (!Attach(::GetWindowDC(m_hWnd = pWnd->GetSafeHwnd())))
		AfxThrowResourceException();
}

CWindowDC::~CWindowDC()
{
	ASSERT(m_hDC != NULL);
	::ReleaseDC(m_hWnd, Detach());
}

#ifdef _DEBUG
void CPaintDC::AssertValid() const
{
	CDC::AssertValid();
	ASSERT(::IsWindow(m_hWnd));
}

void CPaintDC::Dump(CDumpContext& dc) const
{
	CDC::Dump(dc);
	AFX_DUMP1(dc, "\nm_hWnd = ", (UINT)m_hWnd);
	AFX_DUMP1(dc, "\nm_ps.hdc = ", (UINT)m_ps.hdc);
	AFX_DUMP1(dc, "\nm_ps.fErase = ", m_ps.fErase);
	AFX_DUMP1(dc, "\nm_ps.rcPaint = ", (CRect)m_ps.rcPaint);
}
#endif

CPaintDC::CPaintDC(CWnd* pWnd)
{
	ASSERT_VALID(pWnd);

	if (!Attach(::BeginPaint(m_hWnd = pWnd->m_hWnd, &m_ps)))
		AfxThrowResourceException();
}

CPaintDC::~CPaintDC()
{
	ASSERT(m_hDC != NULL);
	::EndPaint(m_hWnd, &m_ps);
	Detach();
}

/////////////////////////////////////////////////////////////////////////////
// CGdiObject

IMPLEMENT_DYNCREATE(CGdiObject, CObject)

#ifdef _DEBUG
void CGdiObject::Dump(CDumpContext& dc) const
{
	CObject::Dump(dc);
	AFX_DUMP1(dc, " m_hObject = ", (UINT)m_hObject);
}

void CGdiObject::AssertValid() const
{
	CObject::AssertValid();
	if (afxData.bWin31)
	{
		ASSERT(m_hObject == NULL || ::IsGDIObject(m_hObject));
	}
}
#endif

#ifndef _AFXDLL
static CHandleMap NEAR _afxMapHGDIOBJ(RUNTIME_CLASS(CGdiObject),
	offsetof(CGdiObject, m_hObject));
#else
#define _afxMapHGDIOBJ (*_AfxGetAppData()->appMapHGDIOBJ)
#endif

void PASCAL CGdiObject::DeleteTempMap()
{
	_afxMapHGDIOBJ.DeleteTemp();
}


CGdiObject* PASCAL CGdiObject::FromHandle(HGDIOBJ h)
{
	CGdiObject* pObject = (CGdiObject*)_afxMapHGDIOBJ.FromHandle(h);
	ASSERT(pObject == NULL || pObject->m_hObject == h);
	return pObject;
}

BOOL CGdiObject::Attach(HGDIOBJ hObject)
{
	ASSERT(m_hObject == NULL);      // only attach once, detach on destroy
	if (hObject == NULL)
		return FALSE;
	_afxMapHGDIOBJ.SetPermanent(m_hObject = hObject, this);
	return TRUE;
}

HGDIOBJ CGdiObject::Detach()
{
	HGDIOBJ hObject;
	if ((hObject = m_hObject) != NULL)
		_afxMapHGDIOBJ.RemoveHandle(m_hObject);
	m_hObject = NULL;
	return hObject;
}

BOOL CGdiObject::DeleteObject()
{
	if (m_hObject == NULL)
		return FALSE;
	return ::DeleteObject(Detach());
}

CGdiObject::~CGdiObject()
{
	DeleteObject();
}

/////////////////////////////////////////////////////////////////////////////
// Standard GDI objects

IMPLEMENT_DYNAMIC(CPen, CGdiObject)
IMPLEMENT_DYNAMIC(CBrush, CGdiObject)
IMPLEMENT_DYNAMIC(CFont, CGdiObject)
IMPLEMENT_DYNAMIC(CBitmap, CGdiObject)
IMPLEMENT_DYNAMIC(CPalette, CGdiObject)
IMPLEMENT_DYNAMIC(CRgn, CGdiObject)

/////////////////////////////////////////////////////////////////////////////
// Out-of-line constructors

////////////////////////////////////////////
// CPen

CPen::CPen(int nPenStyle, int nWidth, COLORREF crColor)
{
	if (!Attach(::CreatePen(nPenStyle, nWidth, crColor)))
		AfxThrowResourceException();
}

#ifdef _DEBUG
void CPen::Dump(CDumpContext& dc) const
{
	LOGPEN lp;

	CGdiObject::Dump(dc);

	if (m_hObject == NULL)
		return;

	if (afxData.bWin31 && !::IsGDIObject(m_hObject))
	{
		// not a valid object
		AFX_DUMP0(dc, "  ILLEGAL HPEN");
		return; // don't do anything more
	}

	VERIFY(GetObject(sizeof(lp), &lp));

	AFX_DUMP1(dc, "\nlgpn.lopnStyle = ", lp.lopnStyle);
	AFX_DUMP1(dc, "\nlgpn.lopnWidth.x (width) = ", lp.lopnWidth.x);
	AFX_DUMP1(dc, "\nlgpn.lopnColor = ", (void FAR*)lp.lopnColor);
}
#endif
////////////////////////////////////////////
// CBrush

CBrush::CBrush(COLORREF crColor)
{
	if (!Attach(::CreateSolidBrush(crColor)))
		AfxThrowResourceException();
}

CBrush::CBrush(int nIndex, COLORREF crColor)
{
	if (!Attach(::CreateHatchBrush(nIndex, crColor)))
		AfxThrowResourceException();
}

CBrush::CBrush(CBitmap* pBitmap)
{
	ASSERT_VALID(pBitmap);

	if (!Attach(::CreatePatternBrush((HBITMAP)pBitmap->m_hObject)))
		AfxThrowResourceException();
}

#ifdef _DEBUG
void CBrush::Dump(CDumpContext& dc) const
{
	LOGBRUSH lb;

	CGdiObject::Dump(dc);

	if (m_hObject == NULL)
		return;

	if (afxData.bWin31 && !::IsGDIObject(m_hObject))
	{
		// not a valid window
		AFX_DUMP0(dc, "  ILLEGAL HBRUSH");
		return; // don't do anything more
	}

	VERIFY(GetObject(sizeof(lb), &lb));

	AFX_DUMP1(dc, "\nlb.lbStyle = ", lb.lbStyle);
	AFX_DUMP1(dc, "\nlb.lbHatch = ", lb.lbHatch);
	AFX_DUMP1(dc, "\nlb.lbColor = ", (void FAR*)lb.lbColor);
}
#endif

/////////////////////////////////////////////////////////////////////////////
#ifdef _DEBUG
void CFont::Dump(CDumpContext& dc) const
{
	LOGFONT lf;

	CGdiObject::Dump(dc);

	if (m_hObject == NULL)
		return;

	if (afxData.bWin31 && !::IsGDIObject(m_hObject))
	{
		// not a valid GDI object
		AFX_DUMP0(dc, "  ILLEGAL HFONT");
		return; // don't do anything more
	}

	VERIFY(GetObject(sizeof(lf), &lf));

	AFX_DUMP1(dc, "\nlf.lfHeight = ", lf.lfHeight);
	AFX_DUMP1(dc, "\nlf.lfWidth = ", lf.lfWidth);
	AFX_DUMP1(dc, "\nlf.lfEscapement = ", lf.lfEscapement);
	AFX_DUMP1(dc, "\nlf.lfOrientation = ", lf.lfOrientation);
	AFX_DUMP1(dc, "\nlf.lfWeight = ", lf.lfWeight);
	AFX_DUMP1(dc, "\nlf.lfItalic = ", (int)lf.lfItalic);
	AFX_DUMP1(dc, "\nlf.lfUnderline = ", (int)lf.lfUnderline);
	AFX_DUMP1(dc, "\nlf.lfStrikeOut = ", (int)lf.lfStrikeOut);
	AFX_DUMP1(dc, "\nlf.lfCharSet = ", (int)lf.lfCharSet);
	AFX_DUMP1(dc, "\nlf.lfOutPrecision = ", (int)lf.lfOutPrecision);
	AFX_DUMP1(dc, "\nlf.lfClipPrecision = ", (int)lf.lfClipPrecision);
	AFX_DUMP1(dc, "\nlf.lfQuality = ", (int)lf.lfQuality);
	AFX_DUMP1(dc, "\nlf.lfPitchAndFamily = ", (int)lf.lfPitchAndFamily);
	AFX_DUMP1(dc, "\nlf.lfFaceName = ", (char*)lf.lfFaceName);
}
#endif

/////////////////////////////////////////////////////////////////////////////

#ifdef _DEBUG
void CBitmap::Dump(CDumpContext& dc) const
{
	BITMAP bm;

	CGdiObject::Dump(dc);

	if (m_hObject == NULL)
		return;

	if (afxData.bWin31 && !::IsGDIObject(m_hObject))
	{
		// not a valid object
		AFX_DUMP0(dc, "  ILLEGAL HBITMAP");
		return; // don't do anything more
	}

	VERIFY(GetObject(sizeof(bm), &bm));

	AFX_DUMP1(dc, "\nbm.bmType = ", bm.bmType);
	AFX_DUMP1(dc, "\nbm.bmHeight = ", bm.bmHeight);
	AFX_DUMP1(dc, "\nbm.bmWidth = ", bm.bmWidth);
	AFX_DUMP1(dc, "\nbm.bmWidthBytes = ", bm.bmWidthBytes);
	AFX_DUMP1(dc, "\nbm.bmPlanes = ", bm.bmPlanes);
	AFX_DUMP1(dc, "\nbm.bmBitsPixel = ", bm.bmBitsPixel);
}
#endif

/////////////////////////////////////////////////////////////////////////////
