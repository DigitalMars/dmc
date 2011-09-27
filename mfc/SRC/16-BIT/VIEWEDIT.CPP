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
#include <ctype.h>

#ifdef AFX_CORE4_SEG
#pragma code_seg(AFX_CORE4_SEG)
#endif

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// AFX_FRSTATE : last find/replace state

AFX_FRSTATE::AFX_FRSTATE()
{
	pFindReplaceDlg = NULL;
	bCase = FALSE;
	bNext = TRUE;
}

#ifndef _AFXDLL
static AFX_FRSTATE NEAR _afxLastFRState;
#else
#define _afxLastFRState (*_AfxGetAppData()->appLastFRState)
#endif

/////////////////////////////////////////////////////////////////////////////
// CEditView

IMPLEMENT_DYNCREATE(CEditView, CView)

#define new DEBUG_NEW

static char BASED_CODE szFINDMSGSTRING[] = FINDMSGSTRING;
static const UINT NEAR nMsgFindReplace =
	::RegisterWindowMessage(szFINDMSGSTRING);

BEGIN_MESSAGE_MAP(CEditView, CView)
	//{{AFX_MSG_MAP(CEditView)
	ON_WM_CREATE()
	ON_WM_PAINT()
	ON_MESSAGE(WM_SETFONT, OnSetFont)
	ON_EN_CHANGE(AFX_IDW_PANE_FIRST, OnEditChange)
	ON_UPDATE_COMMAND_UI(ID_EDIT_CUT, OnUpdateNeedSel)
	ON_UPDATE_COMMAND_UI(ID_EDIT_PASTE, OnUpdateNeedClip)
	ON_UPDATE_COMMAND_UI(ID_EDIT_SELECT_ALL, OnUpdateNeedText)
	ON_UPDATE_COMMAND_UI(ID_EDIT_UNDO, OnUpdateEditUndo)
	ON_COMMAND(ID_EDIT_CUT, OnEditCut)
	ON_COMMAND(ID_EDIT_COPY, OnEditCopy)
	ON_COMMAND(ID_EDIT_PASTE, OnEditPaste)
	ON_COMMAND(ID_EDIT_CLEAR, OnEditClear)
	ON_COMMAND(ID_EDIT_UNDO, OnEditUndo)
	ON_COMMAND(ID_EDIT_SELECT_ALL, OnEditSelectAll)
	ON_UPDATE_COMMAND_UI(ID_EDIT_FIND, OnUpdateNeedText)
	ON_UPDATE_COMMAND_UI(ID_EDIT_REPLACE, OnUpdateNeedText)
	ON_COMMAND(ID_EDIT_FIND, OnEditFind)
	ON_COMMAND(ID_EDIT_REPLACE, OnEditReplace)
	ON_UPDATE_COMMAND_UI(ID_EDIT_REPEAT, OnUpdateNeedFind)
	ON_COMMAND(ID_EDIT_REPEAT, OnEditRepeat)
	ON_UPDATE_COMMAND_UI(ID_EDIT_COPY, OnUpdateNeedSel)
	ON_UPDATE_COMMAND_UI(ID_EDIT_CLEAR, OnUpdateNeedSel)
	//}}AFX_MSG_MAP
	ON_REGISTERED_MESSAGE(nMsgFindReplace, OnFindReplaceCmd)

	// Standard Print commands (print only - not preview)
	ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
END_MESSAGE_MAP()

const DWORD CEditView::dwStyleDefault =
	AFX_WS_DEFAULT_VIEW |
	WS_HSCROLL | WS_VSCROLL |
	ES_AUTOHSCROLL | ES_AUTOVSCROLL |
	ES_MULTILINE | ES_NOHIDESEL;

// Operating system specific maximum buffer limit
const UINT CEditView::nMaxSize = 63U*1024U+1022U;

// class name for control creation
static char BASED_CODE szClassName[] = "EDIT";

/////////////////////////////////////////////////////////////////////////////
// CEditView construction/destruction

CEditView::CEditView()
{
	m_segText = NULL;
	m_nTabStops = 8*4;  // default 8 character positions
	m_hPrinterFont = NULL;
	m_hMirrorFont = NULL;
}

CEditView::~CEditView()
{
	ASSERT(m_hWnd == NULL);
	if (m_segText != NULL)
	{
		HGLOBAL hText = (HGLOBAL)GlobalHandle((UINT)m_segText);
		ASSERT(hText != NULL);
		GlobalUnlock(hText);
		GlobalFree(hText);
	}
}

WNDPROC* CEditView::GetSuperWndProcAddr()
{
	static WNDPROC NEAR pfnSuper;
	return &pfnSuper;
}

BOOL CEditView::PreCreateWindow(CREATESTRUCT& cs)
{
	ASSERT(cs.lpszClass == NULL);
	cs.lpszClass = szClassName;

	if (m_segText == NULL)
	{
		// global alloc segment for edit control to use
		HGLOBAL hText = GlobalAlloc(GMEM_MOVEABLE|GMEM_ZEROINIT, 512);
		if (hText == NULL)
			return FALSE;
		void FAR* p = GlobalLock(hText);
		ASSERT(LOWORD(p) == 0);
		m_segText = _AFX_FP_SEG(p);
	}
	ASSERT(m_segText != NULL);
	cs.hInstance = (HINSTANCE)m_segText;

	// map default CView style to default CEditView style
	if (cs.style == AFX_WS_DEFAULT_VIEW)
		cs.style = dwStyleDefault;

	return TRUE;
}

int CEditView::OnCreate(LPCREATESTRUCT lpcs)
{
	if (CView::OnCreate(lpcs) != 0)
		return -1;
	GetEditCtrl().LimitText(nMaxSize);
	GetEditCtrl().SetTabStops(m_nTabStops);
	return 0;
}

// EDIT controls always turn off WS_BORDER and draw it themselves
void CEditView::CalcWindowRect(LPRECT lpClientRect, UINT nAdjustType)
{
	if (nAdjustType != 0)
	{
		// default behavior for in-place editing handles scrollbars
		DWORD dwStyle = GetStyle();
		if (dwStyle & WS_VSCROLL)
			lpClientRect->right += afxData.cxVScroll - CX_BORDER;
		if (dwStyle & WS_HSCROLL)
			lpClientRect->bottom += afxData.cyHScroll - CY_BORDER;
		return;
	}

	::AdjustWindowRect(lpClientRect, GetStyle() | WS_BORDER, FALSE);
}

/////////////////////////////////////////////////////////////////////////////
// CEditView document like functions

void CEditView::DeleteContents()
{
	ASSERT_VALID(this);
	ASSERT(m_hWnd != NULL);
	SetWindowText(NULL);
	ASSERT_VALID(this);
}

void CEditView::Serialize(CArchive& ar)
	// Read and write CEditView object to archive, with length prefix.
{
	ASSERT_VALID(this);
	ASSERT(m_hWnd != NULL);
	if (ar.IsStoring())
	{
		UINT nLen = GetBufferLength();
		ar << (DWORD)nLen;
		WriteToArchive(ar);
	}
	else
	{
		DWORD dwLen;
		ar >> dwLen;
		if (dwLen > nMaxSize)
		{
			AfxThrowArchiveException(CArchiveException::badIndex);
			ASSERT(FALSE);
		}
		UINT nLen = (UINT)dwLen;
		ReadFromArchive(ar, nLen);
	}
	ASSERT_VALID(this);
}

void CEditView::ReadFromArchive(CArchive& ar, UINT nLen)
	// Read certain amount of text from the file, assume at least nLen
	// bytes are in the file.
{
	ASSERT_VALID(this);
	LPVOID hText = _AfxLocalAlloc(m_segText, LMEM_MOVEABLE, nLen+1);
	if (hText == NULL)
	{
		AfxThrowMemoryException();
		ASSERT(FALSE);
	}
	LPSTR lpszText = (LPSTR)_AfxLocalLock(hText);
	ASSERT(lpszText != NULL);
	if (ar.Read(lpszText, nLen) != nLen)
	{
		_AfxLocalUnlock(hText);
		_AfxLocalFree(hText);
		AfxThrowArchiveException(CArchiveException::endOfFile);
		ASSERT(FALSE);
	}
	// Replace the editing edit buffer with the newly loaded data
	lpszText[nLen] = '\0';
	_AfxLocalUnlock(hText);
	HLOCAL hOldText = GetEditCtrl().GetHandle();
	ASSERT(hOldText != NULL);
	_AfxLocalFree((LPVOID)MAKELONG(hOldText, m_segText));
	GetEditCtrl().SetHandle((HLOCAL)(UINT)(DWORD)hText);
	Invalidate();
	ASSERT_VALID(this);
}

void CEditView::WriteToArchive(CArchive& ar)
	// Write just the text to an archive, no length prefix.
{
	ASSERT_VALID(this);
	LPCSTR lpszText = LockBuffer();
	ASSERT(lpszText != NULL);
	UINT nLen = GetBufferLength();
	TRY
	{
		ar.Write(lpszText, nLen);
	}
	CATCH_ALL(e)
	{
		UnlockBuffer();
		THROW_LAST();
		ASSERT(FALSE);
	}
	END_CATCH_ALL
	UnlockBuffer();
	ASSERT_VALID(this);
}

void CEditView::SerializeRaw(CArchive& ar)
	// Read/Write object as stand-alone file.
{
	ASSERT_VALID(this);
	if (ar.IsStoring())
	{
		WriteToArchive(ar);
	}
	else
	{
		CFile* pFile = ar.GetFile();
		ASSERT(pFile->GetPosition() == 0);
		DWORD nFileSize = pFile->GetLength();
		if (nFileSize > nMaxSize)
		{
			AfxMessageBox(AFX_IDP_FILE_TOO_LARGE);
			AfxThrowUserException();
			ASSERT(FALSE);
		}
		ReadFromArchive(ar, (UINT)nFileSize);
	}
	ASSERT_VALID(this);
}

/////////////////////////////////////////////////////////////////////////////
// CEditView drawing

void CEditView::OnPaint()
{
	// do not call CView::OnPaint since it will call OnDraw
	CWnd::OnPaint();
}

void CEditView::OnDraw(CDC*)
{
	// do nothing here since CWnd::OnPaint() will repaint the EDIT control
}

/////////////////////////////////////////////////////////////////////////////
// CEditView Printing Helpers

static UINT NEAR PASCAL
EndOfLine(LPCSTR lpszText, UINT nLen, UINT nIndex)
{
	ASSERT(AfxIsValidAddress(lpszText, nLen, FALSE));
	LPCSTR lpsz = lpszText + nIndex;
	LPCSTR lpszStop = lpszText + nLen;
	while (lpsz < lpszStop && *lpsz != '\r')
		++lpsz;
	return lpsz - lpszText;
}

static UINT NEAR PASCAL
NextLine(LPCSTR lpszText, UINT nLen, UINT nIndex)
{
	ASSERT(AfxIsValidAddress(lpszText, nLen, FALSE));
	LPCSTR lpsz = lpszText + nIndex;
	LPCSTR lpszStop = lpszText + nLen;
	while (lpsz < lpszStop && *lpsz == '\r')
		++lpsz;
	if (lpsz < lpszStop && *lpsz == '\n')
		++lpsz;
	return lpsz - lpszText;
}

static UINT NEAR PASCAL
ClipLine(CDC* pDC, int aCharWidths[256], int cxLine, int nTabStop,
	LPCSTR lpszText, UINT nIndex, UINT nIndexEnd)
{
	ASSERT_VALID(pDC);
	ASSERT(nIndex < nIndexEnd);
	ASSERT(AfxIsValidAddress(lpszText, nIndexEnd, FALSE));

	TEXTMETRIC tm;
	pDC->GetTextMetrics(&tm);

	// make an initial guess on the number of characters that will fit
	int cx = 0;
	LPCSTR lpszStart = lpszText + nIndex;
	LPCSTR lpszStop = lpszText + nIndexEnd;
	LPCSTR lpsz = lpszStart;
	while (lpsz < lpszStop)
	{
		if (*lpsz == '\t')
			cx += nTabStop - (cx % nTabStop);
		else
		{
			if (afxData.bDBCS && _AfxIsDBCSLeadByte(*lpsz))
			{
				++lpsz;
				cx += tm.tmAveCharWidth;
			}
			else
				cx += aCharWidths[(BYTE)*lpsz];
		}
		++lpsz;
		if (cx > cxLine)
			break;
	}

	// adjust for errors in the guess
	cx = pDC->GetTabbedTextExtent(lpszStart, lpsz-lpszStart, 1, &nTabStop).cx;
	if (cx > cxLine)
	{
		// remove characters until it fits
		do
		{
			ASSERT(lpsz != lpszStart);
			if (afxData.bDBCS)
				lpsz = AnsiPrev(lpszStart, lpsz);
			else
				--lpsz;
			cx = pDC->GetTabbedTextExtent(lpszStart, lpsz-lpszStart, 1, &nTabStop).cx;
		} while (cx > cxLine);
	}
	else if (cx < cxLine)
	{
		// add characters until it doesn't fit
		while (lpsz < lpszStop)
		{
			lpsz = AnsiNext(lpsz);
			ASSERT(lpsz <= lpszStop);
			cx = pDC->GetTabbedTextExtent(lpszStart, lpsz-lpszStart, 1, &nTabStop).cx;
			if (cx > cxLine)
			{
				if (afxData.bDBCS)
					lpsz = AnsiPrev(lpszStart, lpsz);
				else
					--lpsz;
				break;
			}
		}
	}

	// return index of character just past the last that would fit
	return lpsz - lpszText;
}

/////////////////////////////////////////////////////////////////////////////
// CEditView Printing support

BOOL CEditView::OnPreparePrinting(CPrintInfo* pInfo)
{
	return DoPreparePrinting(pInfo);
}

void CEditView::OnBeginPrinting(CDC* pDC, CPrintInfo*)
{
	ASSERT_VALID(this);
	ASSERT_VALID(pDC);
	// initialize page start vector
	ASSERT(m_aPageStart.GetSize() == 0);
	m_aPageStart.Add(0);
	ASSERT(m_aPageStart.GetSize() > 0);

	if (m_hPrinterFont == NULL)
	{
		// get current screen font object metrics
		CFont* pFont = GetFont();
		LOGFONT lf;
		if (pFont == NULL)
			return;
		pFont->GetObject(sizeof(LOGFONT), &lf);
		static char BASED_CODE szSystem[] = "system";
		if (lstrcmpi((LPCSTR)lf.lfFaceName, szSystem) == 0)
			return;

		// map to printer font metrics
		HDC hDCFrom = ::GetDC(NULL);
		lf.lfHeight = ::MulDiv(lf.lfHeight, pDC->GetDeviceCaps(LOGPIXELSY),
			::GetDeviceCaps(hDCFrom, LOGPIXELSY));
		lf.lfWidth = ::MulDiv(lf.lfWidth, pDC->GetDeviceCaps(LOGPIXELSX),
			::GetDeviceCaps(hDCFrom, LOGPIXELSX));
		::ReleaseDC(NULL, hDCFrom);

		// create it, if it fails we just the the printer's default.
		m_hMirrorFont = ::CreateFontIndirect(&lf);
		m_hPrinterFont = m_hMirrorFont;
	}
	ASSERT_VALID(this);
}

BOOL CEditView::PaginateTo(CDC* pDC, CPrintInfo* pInfo)
	// attempts pagination to pInfo->m_nCurPage, TRUE == success
{
	ASSERT_VALID(this);
	ASSERT_VALID(pDC);

	CRect rectSave = pInfo->m_rectDraw;
	UINT nPageSave = pInfo->m_nCurPage;
	ASSERT(nPageSave > 1);
	ASSERT(nPageSave >= (UINT)m_aPageStart.GetSize());
	VERIFY(pDC->SaveDC() != 0);
	pDC->IntersectClipRect(0, 0, 0, 0);
	pInfo->m_nCurPage = m_aPageStart.GetSize();
	while (pInfo->m_nCurPage < nPageSave)
	{
		ASSERT(pInfo->m_nCurPage == (UINT)m_aPageStart.GetSize());
		OnPrepareDC(pDC, pInfo);
		ASSERT(pInfo->m_bContinuePrinting);
		pInfo->m_rectDraw.SetRect(0, 0,
			pDC->GetDeviceCaps(HORZRES), pDC->GetDeviceCaps(VERTRES));
		pDC->DPtoLP(&pInfo->m_rectDraw);
		OnPrint(pDC, pInfo);
		if (pInfo->m_nCurPage == (UINT)m_aPageStart.GetSize())
			break;
		++pInfo->m_nCurPage;
	}
	BOOL bResult = pInfo->m_nCurPage == nPageSave;
	VERIFY(pDC->RestoreDC(-1));
	pInfo->m_nCurPage = nPageSave;
	pInfo->m_rectDraw = rectSave;
	ASSERT_VALID(this);
	return bResult;
}

void CEditView::OnPrepareDC(CDC* pDC, CPrintInfo* pInfo)
{
	ASSERT_VALID(this);
	ASSERT_VALID(pDC);
	ASSERT(pInfo != NULL);  // overriding OnPaint -- never get this.

	if (pInfo->m_nCurPage > (UINT)m_aPageStart.GetSize() &&
		!PaginateTo(pDC, pInfo))
	{
		// can't paginate to that page, thus cannot print it.
		pInfo->m_bContinuePrinting = FALSE;
	}
	ASSERT_VALID(this);
}

UINT CEditView::PrintInsideRect(CDC* pDC, RECT& rectLayout,
	UINT nIndexStart, UINT nIndexStop)
	// worker function for laying out text in a rectangle.
{
	ASSERT_VALID(this);
	ASSERT_VALID(pDC);
	BOOL bWordWrap = (GetStyle() & ES_AUTOHSCROLL) == 0;

	// get buffer and real starting and ending postions
	UINT nLen = GetBufferLength();
	if (nIndexStart >= nLen)
		return nLen;
	LPCSTR lpszText = LockBuffer();
	if (nIndexStop > nLen)
		nIndexStop = nLen;
	ASSERT(nIndexStart < nLen);

	// calculate text & tab metrics
	TEXTMETRIC tm;
	pDC->GetTextMetrics(&tm);
	int cyChar = tm.tmHeight;
	int nTabStop = m_nTabStops *
		pDC->GetTabbedTextExtent("\t", 1, 0, NULL).cx / 8 / 4;
	int aCharWidths[256];
	pDC->GetCharWidth(0, 255, aCharWidths);

	int y = rectLayout.top;
	UINT cx = rectLayout.right - rectLayout.left;
	UINT nIndex = nIndexStart;

	VERIFY(pDC->SaveDC() != 0);
	BOOL bLayoutOnly = pDC->IntersectClipRect(&rectLayout) == NULLREGION;

	do
	{
		UINT nIndexEnd = EndOfLine(lpszText, nIndexStop, nIndex);
		if (nIndex == nIndexEnd)
		{
			y += cyChar;
		}
		else if (bWordWrap)
		{
			// word-wrap printing
			do
			{
				UINT nIndexWrap = ClipLine(pDC, aCharWidths,
					cx, nTabStop, lpszText, nIndex, nIndexEnd);
				UINT nIndexWord = nIndexWrap;
				if (nIndexWord != nIndexEnd)
				{
					while (nIndexWord > nIndex &&
					  !isspace(lpszText[nIndexWord]))
					{
						nIndexWord--;
					}
					if (nIndexWord == nIndex)
						nIndexWord = nIndexWrap;
				}
				CRect rect(rectLayout.left, y, rectLayout.right, y+cyChar);
				if (!bLayoutOnly && pDC->RectVisible(rect))
				{
					pDC->TabbedTextOut(rect.left, y,
						(LPCSTR)(lpszText+nIndex), nIndexWord-nIndex, 1,
						&nTabStop, rect.left);
				}
				y += cyChar;
				nIndex = nIndexWord;
				while (nIndex < nIndexEnd && isspace(lpszText[nIndex]))
					nIndex++;
			} while (nIndex < nIndexEnd && y+cyChar <= rectLayout.bottom);

			nIndexEnd = nIndex;
		}
		else
		{
			// non-word wrap printing (much easier and faster)
			CRect rect(rectLayout.left, y, rectLayout.right, y+cyChar);
			if (!bLayoutOnly && pDC->RectVisible(rect))
			{
				UINT nIndexClip = ClipLine(pDC, aCharWidths, cx, nTabStop,
					lpszText, nIndex, nIndexEnd);
				if (nIndexClip < nIndexEnd)
				{
					if (_AfxIsDBCSLeadByte(*(lpszText+nIndexClip)))
						nIndexClip++;
					nIndexClip++;
				}
				pDC->TabbedTextOut(rect.left, y,
					(LPCSTR)(lpszText+nIndex), nIndexClip-nIndex, 1,
					&nTabStop, rect.left);
			}
			y += cyChar;
		}
		nIndex = NextLine(lpszText, nIndexStop, nIndexEnd);
	}
	while (nIndex < nIndexStop && y+cyChar <= rectLayout.bottom);

	VERIFY(pDC->RestoreDC(-1));
	UnlockBuffer();
	ASSERT_VALID(this);

	rectLayout.bottom = y;
	return nIndex;
}

void CEditView::OnPrint(CDC* pDC, CPrintInfo* pInfo)
{
	ASSERT_VALID(this);
	ASSERT_VALID(pDC);
	ASSERT(pInfo != NULL);
	ASSERT(pInfo->m_bContinuePrinting);

	CFont* pOldFont = NULL;
	if (m_hPrinterFont != NULL)
		pOldFont = pDC->SelectObject(CFont::FromHandle(m_hPrinterFont));
	pDC->SetBkMode(TRANSPARENT);

	UINT nPage = pInfo->m_nCurPage;
	ASSERT(nPage <= (UINT)m_aPageStart.GetSize());
	UINT nIndex = m_aPageStart[nPage-1];

	// print as much as possible in the current page.
	nIndex = PrintInsideRect(pDC, pInfo->m_rectDraw, nIndex, 0xFFFF);

	if (pOldFont != NULL)
		pDC->SelectObject(pOldFont);

	// update pagination information for page just printed
	if (nPage == (UINT)m_aPageStart.GetSize())
	{
		if (nIndex < GetBufferLength())
			m_aPageStart.Add(nIndex);
	}
	else
	{
		ASSERT(nPage+1 <= (UINT)m_aPageStart.GetSize());
		ASSERT(nIndex == m_aPageStart[nPage+1-1]);
	}
}

void CEditView::OnEndPrinting(CDC*, CPrintInfo*)
{
	ASSERT_VALID(this);

	m_aPageStart.RemoveAll();
	if (m_hMirrorFont != NULL && m_hPrinterFont == m_hMirrorFont)
	{
		::DeleteObject(m_hMirrorFont);
		m_hMirrorFont = NULL;
		m_hPrinterFont = NULL;
	}
}

/////////////////////////////////////////////////////////////////////////////
// CEditView commands

void CEditView::OnUpdateNeedSel(CCmdUI* pCmdUI)
{
	ASSERT_VALID(this);
	int nStartChar, nEndChar;
	GetEditCtrl().GetSel(nStartChar, nEndChar);
	pCmdUI->Enable(nStartChar != nEndChar);
	ASSERT_VALID(this);
}

void CEditView::OnUpdateNeedClip(CCmdUI* pCmdUI)
{
	ASSERT_VALID(this);
	pCmdUI->Enable(::IsClipboardFormatAvailable(CF_TEXT));
	ASSERT_VALID(this);
}

void CEditView::OnUpdateNeedText(CCmdUI* pCmdUI)
{
	ASSERT_VALID(this);
	pCmdUI->Enable(GetBufferLength() != 0);
	ASSERT_VALID(this);
}

void CEditView::OnUpdateNeedFind(CCmdUI* pCmdUI)
{
	ASSERT_VALID(this);
	pCmdUI->Enable(GetBufferLength() != 0 &&
		!_afxLastFRState.strFind.IsEmpty());
	ASSERT_VALID(this);
}

void CEditView::OnUpdateEditUndo(CCmdUI* pCmdUI)
{
	ASSERT_VALID(this);
	pCmdUI->Enable(GetEditCtrl().CanUndo());
	ASSERT_VALID(this);
}

void CEditView::OnEditChange()
{
	ASSERT_VALID(this);
	GetDocument()->SetModifiedFlag();
	ASSERT_VALID(this);
}

void CEditView::OnEditCut()
{
	ASSERT_VALID(this);
	GetEditCtrl().Cut();
	ASSERT_VALID(this);
}

void CEditView::OnEditCopy()
{
	ASSERT_VALID(this);
	GetEditCtrl().Copy();
	ASSERT_VALID(this);
}

void CEditView::OnEditPaste()
{
	ASSERT_VALID(this);
	GetEditCtrl().Paste();
	ASSERT_VALID(this);
}

void CEditView::OnEditClear()
{
	ASSERT_VALID(this);
	GetEditCtrl().Clear();
	ASSERT_VALID(this);
}

void CEditView::OnEditUndo()
{
	ASSERT_VALID(this);
	GetEditCtrl().Undo();
	ASSERT_VALID(this);
}

void CEditView::OnEditSelectAll()
{
	ASSERT_VALID(this);
	GetEditCtrl().SetSel(0, -1);
	ASSERT_VALID(this);
}

/////////////////////////////////////////////////////////////////////////////
// CEditView Font Handling

LRESULT CEditView::OnSetFont(WPARAM, LPARAM)
{
	ASSERT_VALID(this);
	Default();
	GetEditCtrl().SetTabStops(m_nTabStops);
	ASSERT_VALID(this);
	return 0;
}

void CEditView::SetPrinterFont(CFont* pFont)
{
	ASSERT_VALID(this);
	m_hPrinterFont = (HFONT)pFont->GetSafeHandle();
	ASSERT_VALID(this);
}

CFont* CEditView::GetPrinterFont() const
{
	ASSERT_VALID(this);
	return CFont::FromHandle(m_hPrinterFont);
}

/////////////////////////////////////////////////////////////////////////////
// CEditView attributes

LPCSTR CEditView::LockBuffer() const
{
	ASSERT_VALID(this);
	ASSERT(m_hWnd != NULL);
	HLOCAL hLocal = GetEditCtrl().GetHandle();
	ASSERT(hLocal != NULL);
	LPCSTR lpszText = _AfxLocalLock((LPVOID)MAKELONG(hLocal, m_segText));
	ASSERT(_AFX_FP_SEG(lpszText) == m_segText);
	ASSERT_VALID(this);
	return lpszText;
}

void CEditView::UnlockBuffer() const
{
	ASSERT_VALID(this);
	ASSERT(m_hWnd != NULL);
	HLOCAL hLocal = GetEditCtrl().GetHandle();
	ASSERT(hLocal != NULL);
	_AfxLocalUnlock((LPVOID)MAKELONG(hLocal, m_segText));
}

UINT CEditView::GetBufferLength() const
{
	ASSERT_VALID(this);
	ASSERT(m_hWnd != NULL);
	UINT nLen = GetWindowTextLength();
	return nLen;
}

void CEditView::GetSelectedText(CString& strResult) const
{
	ASSERT_VALID(this);
	int nStartChar, nEndChar;
	GetEditCtrl().GetSel(nStartChar, nEndChar);
	ASSERT((UINT)nEndChar <= GetBufferLength());
	LPCSTR lpszText = ((CEditView*)this)->LockBuffer();
	UINT nLen = EndOfLine(lpszText, nEndChar, nStartChar) - nStartChar;
	_fmemcpy(strResult.GetBuffer(nLen), lpszText + nStartChar, nLen);
	strResult.ReleaseBuffer(nLen);
	UnlockBuffer();
	ASSERT_VALID(this);
}

/////////////////////////////////////////////////////////////////////////////
// CEditView Find & Replace

void CEditView::OnEditFind()
{
	ASSERT_VALID(this);
	OnEditFindReplace(TRUE);
	ASSERT_VALID(this);
}

void CEditView::OnEditReplace()
{
	ASSERT_VALID(this);
	OnEditFindReplace(FALSE);
	ASSERT_VALID(this);
}

void CEditView::OnEditRepeat()
{
	ASSERT_VALID(this);
	if (!FindText(_afxLastFRState.strFind, _afxLastFRState.bNext,
		_afxLastFRState.bCase))
	{
		OnTextNotFound(_afxLastFRState.strFind);
	}
	ASSERT_VALID(this);
}

void CEditView::OnEditFindReplace(BOOL bFindOnly)
{
	ASSERT_VALID(this);
	if (_afxLastFRState.pFindReplaceDlg != NULL)
	{
		if (_afxLastFRState.bFindOnly == bFindOnly)
		{
			_afxLastFRState.pFindReplaceDlg->SetActiveWindow();
			_afxLastFRState.pFindReplaceDlg->ShowWindow(SW_SHOW);
			return;
		}
		else
		{
			ASSERT(_afxLastFRState.bFindOnly != bFindOnly);
			_afxLastFRState.pFindReplaceDlg->SendMessage(WM_CLOSE);
			ASSERT(_afxLastFRState.pFindReplaceDlg == NULL);
			ASSERT_VALID(this);
		}
	}
	CString strFind;
	GetSelectedText(strFind);
	if (strFind.IsEmpty())
		strFind = _afxLastFRState.strFind;
	CString strReplace = _afxLastFRState.strReplace;
	_afxLastFRState.pFindReplaceDlg = new CFindReplaceDialog;
	ASSERT(_afxLastFRState.pFindReplaceDlg != NULL);
	DWORD dwFlags = FR_HIDEWHOLEWORD;
	if (_afxLastFRState.bNext)
		dwFlags |= FR_DOWN;
	if (_afxLastFRState.bCase)
		dwFlags |= FR_MATCHCASE;
	if (!_afxLastFRState.pFindReplaceDlg->Create(bFindOnly, strFind,
		strReplace, dwFlags, this))
	{
		_afxLastFRState.pFindReplaceDlg = NULL;
		ASSERT_VALID(this);
		return;
	}
	ASSERT(_afxLastFRState.pFindReplaceDlg != NULL);
	_afxLastFRState.bFindOnly = bFindOnly;
	ASSERT_VALID(this);
}

void CEditView::OnFindNext(LPCSTR lpszFind, BOOL bNext, BOOL bCase)
{
	ASSERT_VALID(this);
	_afxLastFRState.strFind = lpszFind;
	_afxLastFRState.bCase = bCase;
	_afxLastFRState.bNext = bNext;

	if (!FindText(_afxLastFRState.strFind, bNext, bCase))
		OnTextNotFound(_afxLastFRState.strFind);
	ASSERT_VALID(this);
}

void CEditView::OnReplaceSel(LPCSTR lpszFind, BOOL bNext, BOOL bCase,
	LPCSTR lpszReplace)
{
	ASSERT_VALID(this);
	_afxLastFRState.strFind = lpszFind;
	_afxLastFRState.strReplace = lpszReplace;
	_afxLastFRState.bCase = bCase;
	_afxLastFRState.bNext = bNext;

	if (!InitializeReplace())
		return;

	GetEditCtrl().ReplaceSel(_afxLastFRState.strReplace);
	FindText(_afxLastFRState.strFind, bNext, bCase);
	ASSERT_VALID(this);
}

void CEditView::OnReplaceAll(LPCSTR lpszFind, LPCSTR lpszReplace, BOOL bCase)
{
	ASSERT_VALID(this);
	_afxLastFRState.strFind = lpszFind;
	_afxLastFRState.strReplace = lpszReplace;
	_afxLastFRState.bCase = bCase;
	_afxLastFRState.bNext = TRUE;

	if (!InitializeReplace() &&
		!SameAsSelected(_afxLastFRState.strFind, _afxLastFRState.bCase))
	{
		// initial find was not successful
		return;
	}

	do
	{
		GetEditCtrl().ReplaceSel(_afxLastFRState.strReplace);
	} while (FindText(_afxLastFRState.strFind, 1, bCase));

	ASSERT_VALID(this);
}

BOOL CEditView::InitializeReplace()
	// helper to do find first if no selection
{
	ASSERT_VALID(this);

	// do find next if no selection
	int nStartChar, nEndChar;
	GetEditCtrl().GetSel(nStartChar, nEndChar);
	if (nStartChar == nEndChar)
	{
		if (!FindText(_afxLastFRState.strFind,
		  _afxLastFRState.bNext, _afxLastFRState.bCase))
			OnTextNotFound(_afxLastFRState.strFind);
		return FALSE;
	}

	if (!SameAsSelected(_afxLastFRState.strFind, _afxLastFRState.bCase))
	{
		if (!FindText(_afxLastFRState.strFind,
		  _afxLastFRState.bNext, _afxLastFRState.bCase))
			OnTextNotFound(_afxLastFRState.strFind);
		return FALSE;
	}

	ASSERT_VALID(this);
	return TRUE;
}

LRESULT CEditView::OnFindReplaceCmd(WPARAM, LPARAM lParam)
{
	ASSERT_VALID(this);
	CFindReplaceDialog* pDialog = CFindReplaceDialog::GetNotifier(lParam);
	ASSERT(pDialog != NULL);
	ASSERT(pDialog == _afxLastFRState.pFindReplaceDlg);
	if (pDialog->IsTerminating())
	{
		_afxLastFRState.pFindReplaceDlg = NULL;
	}
	else if (pDialog->FindNext())
	{
		OnFindNext(pDialog->GetFindString(),
			pDialog->SearchDown(), pDialog->MatchCase());
	}
	else if (pDialog->ReplaceCurrent())
	{
		ASSERT(!_afxLastFRState.bFindOnly);
		OnReplaceSel(pDialog->GetFindString(),
			pDialog->SearchDown(), pDialog->MatchCase(),
			pDialog->GetReplaceString());
	}
	else if (pDialog->ReplaceAll())
	{
		ASSERT(!_afxLastFRState.bFindOnly);
		OnReplaceAll(pDialog->GetFindString(), pDialog->GetReplaceString(),
			pDialog->MatchCase());
	}
	ASSERT_VALID(this);
	return 0;
}

typedef int (WINAPI* AFX_COMPARE_PROC)(LPCSTR str1, LPCSTR str2);

BOOL CEditView::SameAsSelected(LPCSTR lpszCompare, BOOL bCase)
{
	// check length first
	size_t nLen = lstrlen(lpszCompare);
	int nStartChar, nEndChar;
	GetEditCtrl().GetSel(nStartChar, nEndChar);
	if (nLen != (size_t)(nEndChar - nStartChar))
		return FALSE;

	// length is the same, check contents
	CString strSelect;
	GetSelectedText(strSelect);
	return (bCase && lstrcmp(lpszCompare, strSelect) == 0) ||
		(!bCase && lstrcmpi(lpszCompare, strSelect) == 0);
}

BOOL CEditView::FindText(LPCSTR lpszFind, BOOL bNext, BOOL bCase)
{
	ASSERT_VALID(this);
	ASSERT(lpszFind != NULL);
	ASSERT(*lpszFind != '\0');

	UINT nLen = GetBufferLength();
	int nStartChar, nEndChar;
	GetEditCtrl().GetSel(nStartChar, nEndChar);
	UINT nStart = nStartChar;
	int iDir = bNext ? +1 : -1;

	// can't find a match before the first character
	if (nStart == 0 && iDir < 0)
		return FALSE;

	BeginWaitCursor();
	LPCSTR lpszText = LockBuffer();

	if (iDir < 0)
	{
		// always go back one for search backwards
		nStart -= (lpszText+nStart) -
			AnsiPrev(lpszText, lpszText+nStart);
	}
	else if (nStartChar != nEndChar && SameAsSelected(lpszFind, bCase))
	{
		// easy to go backward/forward with SBCS
		if (_AfxIsDBCSLeadByte(lpszText[nStart]))
			nStart++;
		nStart += iDir;
	}

	// handle search with nStart past end of buffer
	size_t nLenFind = lstrlen(lpszFind);
	if (nStart+nLenFind-1 >= nLen)
	{
		if (iDir < 0 && nLen >= nLenFind)
		{
			if (afxData.bDBCS)
			{
				// walk back to previous character n times
				nStart = nLen;
				int n = nLenFind;
				while (n--)
				{
					nStart -= (lpszText+nStart) -
						AnsiPrev(lpszText, lpszText+nStart);
				}
			}
			else
			{
				// single-byte character set is easy and fast
				nStart = nLen - nLenFind;
			}
			ASSERT(nStart+nLenFind-1 <= nLen);
		}
		else
		{
			UnlockBuffer();
			EndWaitCursor();
			return FALSE;
		}
	}

	// start the search at nStart
	LPCSTR lpsz = lpszText + nStart;
	AFX_COMPARE_PROC pfnCompare = bCase ? lstrcmp : lstrcmpi;

	if (afxData.bDBCS)
	{
		// double-byte string search
		LPCSTR lpszStop;
		if (iDir > 0)
		{
			// start at current and find _first_ occurance
			lpszStop = lpszText + nLen - nLenFind;
		}
		else
		{
			// start at top and find _last_ occurance
			lpszStop = lpsz;
			lpsz = lpszText;
		}

		LPCSTR lpszFound = NULL;
		while (lpsz <= lpszStop)
		{
			if (!bCase || (*lpsz == *lpszFind &&
				(!_AfxIsDBCSLeadByte(*lpsz) || lpsz[1] == lpszFind[1])))
			{
				LPSTR lpch = (LPSTR)(lpsz + nLenFind);
				char chSave = *lpch;
				*lpch = '\0';
				int nResult = (*pfnCompare)(lpsz, lpszFind);
				*lpch = chSave;
				if (nResult == 0)
				{
					lpszFound = lpsz;
					if (iDir > 0)
						break;
				}
			}
			lpsz = AnsiNext(lpsz);
		}
		UnlockBuffer();

		if (lpszFound != NULL)
		{
			int n = (int)(lpszFound - lpszText);
			GetEditCtrl().SetSel(n, n+nLenFind);
			EndWaitCursor();
			return TRUE;
		}
	}
	else
	{
		// single-byte string search
		UINT nCompare;
		if (iDir < 0)
			nCompare = (UINT)(lpsz - lpszText) + 1;
		else
			nCompare = nLen - (UINT)(lpsz - lpszText) - nLenFind + 1;

		while (nCompare > 0)
		{
			ASSERT(lpsz >= lpszText);
			ASSERT(lpsz+nLenFind-1 <= lpszText+nLen-1);

			LPSTR lpch = (LPSTR)(lpsz + nLenFind);
			char chSave = *lpch;
			*lpch = '\0';
			int nResult = (*pfnCompare)(lpsz, lpszFind);
			*lpch = chSave;
			if (nResult == 0)
			{
				UnlockBuffer();
				int n = (int)(lpsz - lpszText);
				GetEditCtrl().SetSel(n, n+nLenFind);
				ASSERT_VALID(this);
				EndWaitCursor();
				return TRUE;
			}

			// restore character at end of search
			*lpch = chSave;

			// move on to next substring
			nCompare--;
			lpsz += iDir;
		}
		UnlockBuffer();
	}

	ASSERT_VALID(this);
	EndWaitCursor();
	return FALSE;
}

void CEditView::OnTextNotFound(LPCSTR)
{
	ASSERT_VALID(this);
	MessageBeep(0);
}

/////////////////////////////////////////////////////////////////////////////
// CEditView Tab Stops

void CEditView::SetTabStops(int nTabStops)
{
	ASSERT_VALID(this);
	m_nTabStops = nTabStops;
	GetEditCtrl().SetTabStops(m_nTabStops);
	Invalidate();
	ASSERT_VALID(this);
}

/////////////////////////////////////////////////////////////////////////////
// CEditView diagnostics

#ifdef _DEBUG
void CEditView::AssertValid() const
{
	CView::AssertValid();
	ASSERT_VALID(&m_aPageStart);
	if (m_hPrinterFont != NULL)
		ASSERT_VALID(CFont::FromHandle(m_hPrinterFont));
	if (m_hMirrorFont != NULL)
		ASSERT_VALID(CFont::FromHandle(m_hMirrorFont));
	if (_afxLastFRState.pFindReplaceDlg != NULL)
		ASSERT_VALID(_afxLastFRState.pFindReplaceDlg);
	if (m_hWnd != NULL)
		ASSERT(m_segText != NULL);
}

void CEditView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
	AFX_DUMP1(dc, "\nm_nTabStops = ", m_nTabStops);
	if (m_hPrinterFont != NULL)
		AFX_DUMP1(dc, "\nm_hPrinterFont ", (UINT)m_hPrinterFont);
	if (m_hMirrorFont != NULL)
		AFX_DUMP1(dc, "\nm_hMirrorFont ", (UINT)m_hMirrorFont);
	AFX_DUMP1(dc, "\nm_aPageStart ", &m_aPageStart);
	AFX_DUMP0(dc, "\n Static Member Data:");
	if (_afxLastFRState.pFindReplaceDlg != NULL)
	{
		AFX_DUMP1(dc, "\npFindReplaceDlg = ",
			(void*)_afxLastFRState.pFindReplaceDlg);
		AFX_DUMP1(dc, "\nbFindOnly = ", _afxLastFRState.bFindOnly);
	}
	AFX_DUMP1(dc, "\nstrFind = ", _afxLastFRState.strFind);
	AFX_DUMP1(dc, "\nstrReplace = ", _afxLastFRState.strReplace);
	AFX_DUMP1(dc, "\nbCase = ", _afxLastFRState.bCase);
	AFX_DUMP1(dc, "\nbNext = ", _afxLastFRState.bNext);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
