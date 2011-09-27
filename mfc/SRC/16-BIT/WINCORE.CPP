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

#include <stddef.h>     // for offsetof

#ifdef AFX_CORE1_SEG
#pragma code_seg(AFX_CORE1_SEG)
#endif

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// Globals

// CWnds for setting z-order with SetWindowPos's pWndInsertAfter parameter
const CWnd AFXAPI_DATA CWnd::wndTop(HWND_TOP);
const CWnd AFXAPI_DATA CWnd::wndBottom(HWND_BOTTOM);
const CWnd AFXAPI_DATA CWnd::wndTopMost(HWND_TOPMOST);
const CWnd AFXAPI_DATA CWnd::wndNoTopMost(HWND_NOTOPMOST);

const char BASED_CODE _afxWnd[] = "AfxWnd";
const char BASED_CODE _afxWndControlBar[] = "AfxControlBar";
const char BASED_CODE _afxWndMDIFrame[] = "AfxMDIFrame";
const char BASED_CODE _afxWndFrameOrView[] = "AfxFrameOrView";

// Special window creation
static CWnd* NEAR pWndInit = NULL;         // shared global with modal usage
static HHOOK NEAR hHookOldSendMsg = NULL;  // shared global with modal usage

// global for last state of call to 'WindowProc'
MSG NEAR _afxLastMsg;       // shared global with modal usage

/////////////////////////////////////////////////////////////////////////////
// CWnd construction

CWnd::CWnd()
{
	m_hWnd = NULL;
	m_hWndOwner = NULL;
	m_pDropTarget = NULL;
}

CWnd::CWnd(HWND hWnd)
{
	m_hWnd = hWnd;
	m_hWndOwner = NULL;
	m_pDropTarget = NULL;
}

/////////////////////////////////////////////////////////////////////////////
// Stack Checking routine.  Returns FALSE if stack is in danger of overflow

#ifndef _PORTABLE
#pragma optimize("qgel", off)
BOOL _AfxIsStackOK(UINT nStackNeeded)
{
	UINT nStackBottom;
	UINT nCurrentStack;

	_asm
	{
			MOV     AX,SS:[0x000A]
			MOV     nStackBottom,AX
			MOV     nCurrentStack,SP
	}
	return (nCurrentStack - nStackBottom) > nStackNeeded;
}
#pragma optimize("", on)
#endif

/////////////////////////////////////////////////////////////////////////////
// Official way to send message to a CWnd

LRESULT PASCAL _AfxCallWndProc(CWnd* pWnd, HWND hWnd, UINT message,
	WPARAM wParam, LPARAM lParam)
{
	LRESULT lResult;
	MSG oldState = _afxLastMsg;    // save for nesting

	_afxLastMsg.hwnd = hWnd;
	_afxLastMsg.message = message;
	_afxLastMsg.wParam = wParam;
	_afxLastMsg.lParam = lParam;

#ifdef _DEBUG
	if (afxTraceFlags & 4)
		_AfxTraceMsg("WndProc", &_afxLastMsg);
#endif

	// Catch exceptions thrown outside the scope of a callback
	// in debug builds and warn the user.
	TRY
	{
#ifndef _PORTABLE
		// throw exception if insufficient stack space available
		UINT nStackRequired = AfxGetApp()->m_nMsgStack;
		if (message == WM_COMMAND && (HWND)LOWORD(lParam) == NULL)
			nStackRequired = AfxGetApp()->m_nCmdStack;
		if (nStackRequired != 0 && !_AfxIsStackOK(nStackRequired))
			AfxThrowMemoryException();
#endif

		// delagate to object's WindowProc
		lResult = pWnd->WindowProc(message, wParam, lParam);
	}
	CATCH_ALL(e)
	{
		lResult = AfxGetApp()->ProcessWndProcException(e, &_afxLastMsg);
		TRACE1("Warning: Uncaught exception in WindowProc (returning %ld)\n",
			lResult);
	}
	END_CATCH_ALL

	_afxLastMsg = oldState;
	return lResult;
}

const MSG* PASCAL CWnd::GetCurrentMessage()
{
	// fill in time and position when asked for
	_afxLastMsg.time = ::GetMessageTime();
	*((DWORD*)&_afxLastMsg.pt) = ::GetMessagePos();
	return &_afxLastMsg;
}

LRESULT CWnd::Default()
	// call DefWindowProc with the last message
{
	return DefWindowProc(_afxLastMsg.message,
			_afxLastMsg.wParam, _afxLastMsg.lParam);
}

/////////////////////////////////////////////////////////////////////////////
// Map from HWND to CWnd*

#ifndef _AFXDLL
static CHandleMap NEAR _afxMapHWND(RUNTIME_CLASS(CWnd),
	offsetof(CWnd, m_hWnd));
#else
#define _afxMapHWND (*_AfxGetAppData()->appMapHWND)
#endif

void PASCAL CWnd::DeleteTempMap()
{
	_afxMapHWND.DeleteTemp();
}

CWnd* PASCAL CWnd::FromHandle(HWND hWnd)
{
	CWnd* pWnd = (CWnd*)_afxMapHWND.FromHandle(hWnd);
	ASSERT(pWnd == NULL || pWnd->m_hWnd == hWnd);
	return pWnd;
}

CWnd* PASCAL CWnd::FromHandlePermanent(HWND hWnd)
{
	// only look in the permanent map - does no allocations
	CWnd* pWnd;
	if (!_afxMapHWND.LookupPermanent(hWnd, (CObject*&)pWnd))
		return NULL;
	ASSERT(pWnd->m_hWnd == hWnd);
	return pWnd;
}

BOOL CWnd::Attach(HWND hWndNew)
{
	ASSERT(m_hWnd == NULL);     // only attach once, detach on destroy
	ASSERT(FromHandlePermanent(hWndNew) == NULL);
					// must not already be in permanent map

	if (hWndNew == NULL)
		return FALSE;
	_afxMapHWND.SetPermanent(m_hWnd = hWndNew, this);
	return TRUE;
}

HWND CWnd::Detach()
{
	HWND hWnd;
	if ((hWnd = m_hWnd) != NULL)
		_afxMapHWND.RemoveHandle(m_hWnd);
	m_hWnd = NULL;
	return hWnd;
}


/////////////////////////////////////////////////////////////////////////////
// One main WndProc for all CWnd's and derived classes

LRESULT CALLBACK AFX_EXPORT
AfxWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	CWnd* pWnd;

	pWnd = CWnd::FromHandlePermanent(hWnd);
	ASSERT(pWnd != NULL);
	ASSERT(pWnd->m_hWnd == hWnd);

	LRESULT lResult = _AfxCallWndProc(pWnd, hWnd, message, wParam, lParam);

	return lResult;
}

/////////////////////////////////////////////////////////////////////////////
// Window creation hook

#pragma optimize("q", off)
	// disable pcode optimization (for Win 3.0 compatibility)

LRESULT CALLBACK AFX_EXPORT
_AfxSendMsgHook(int code, WPARAM wParam, LPARAM lParam)
{
	struct HOOKINFO     // Hook info struct passed by send message hook
	{
		LPARAM lParam;
		WPARAM wParam;
		UINT msg;
		HWND hWnd;
	};
	HOOKINFO FAR* hookInfo;

	if (code < 0)
	{
		if (!afxData.bWin31)
		{
#ifdef STRICT
			return ::DefHookProc(code, wParam, lParam,
					&hHookOldSendMsg);
#else
			return ::DefHookProc(code, wParam, lParam,
					(HOOKPROC FAR*)&hHookOldSendMsg);
#endif
		}
		else
		{
			ASSERT(hHookOldSendMsg != NULL);
			return ::CallNextHookEx(hHookOldSendMsg, code, wParam, lParam);
		}
	}

	ASSERT(pWndInit != NULL);
	hookInfo = (HOOKINFO FAR*)lParam;
	HWND hWnd = hookInfo->hWnd;

	// ignore non-creation messages
	if (hookInfo->msg != WM_GETMINMAXINFO && hookInfo->msg != WM_NCCREATE)
	{
		// not being constructed
		return 0L;
	}

	if (CWnd::FromHandlePermanent(hWnd) != NULL)
	{
		// already constructed
		ASSERT(pWndInit != CWnd::FromHandlePermanent(hWnd));
		return 0L;
	}

	// Connect the HWND to pWndInit...
	pWndInit->Attach(hWnd);

	// Subclass the window by replacing its window proc addr...
	WNDPROC oldWndProc = (WNDPROC)::SetWindowLong(hWnd, GWL_WNDPROC,
		(DWORD)AfxWndProc);
	if (oldWndProc != (WNDPROC)AfxWndProc)
	{
		*(pWndInit->GetSuperWndProcAddr()) = oldWndProc; // save if not default
	}

	// Unhook the send message hook since we don't need it any more
	if (!afxData.bWin31)
		::UnhookWindowsHook(WH_CALLWNDPROC, (HOOKPROC)_AfxSendMsgHook);
	else
		::UnhookWindowsHookEx(hHookOldSendMsg);
	pWndInit = NULL;
	return 0L;
}
#pragma optimize("", on)    // return to default optimizations

void PASCAL _AfxHookWindowCreate(CWnd* pWnd)
{
	if (!afxData.bWin31)
	{
		hHookOldSendMsg = (HHOOK)::SetWindowsHook(WH_CALLWNDPROC,
			(HOOKPROC)_AfxSendMsgHook);
	}
	else
	{
#if !defined(_USRDLL) && !defined(_AFXCTL)
		// Just set the hook for this task
		hHookOldSendMsg = ::SetWindowsHookEx(WH_CALLWNDPROC,
			(HOOKPROC)_AfxSendMsgHook, _AfxGetHookHandle(),
			::GetCurrentTask());
#else
		// DLL hook must be set for everyone
		hHookOldSendMsg = ::SetWindowsHookEx(WH_CALLWNDPROC,
			(HOOKPROC)_AfxSendMsgHook, _AfxGetHookHandle(), NULL);
#endif
	}


	ASSERT(pWnd != NULL);
	ASSERT(pWnd->m_hWnd == NULL);   // only do once

	ASSERT(pWndInit == NULL);       // hook not already in progress
	pWndInit = pWnd;
}

BOOL PASCAL _AfxUnhookWindowCreate()
	// return TRUE if already unhooked
{
	if (pWndInit == NULL)
		return TRUE;        // already unhooked => window create success
	if (!afxData.bWin31)
		::UnhookWindowsHook(WH_CALLWNDPROC, (HOOKPROC)_AfxSendMsgHook);
	else
		::UnhookWindowsHookEx(hHookOldSendMsg);
	pWndInit = NULL;
	return FALSE;
}

/////////////////////////////////////////////////////////////////////////////
// CWnd creation

BOOL CWnd::CreateEx(DWORD dwExStyle, LPCSTR lpszClassName,
		LPCSTR lpszWindowName, DWORD dwStyle,
		int x, int y, int nWidth, int nHeight,
		HWND hWndParent, HMENU nIDorHMenu, LPSTR lpParam /* = NULL*/)
{
	// allow modification of several common create parameters
	CREATESTRUCT cs;
	cs.dwExStyle = dwExStyle;
	cs.lpszClass = lpszClassName;
	cs.lpszName = lpszWindowName;
	cs.style = dwStyle;
	cs.x = x;
	cs.y = y;
	cs.cx = nWidth;
	cs.cy = nHeight;
	cs.hwndParent = hWndParent;
	cs.hMenu = nIDorHMenu;
	cs.hInstance = AfxGetInstanceHandle();
	cs.lpCreateParams = lpParam;

	if (!PreCreateWindow(cs))
	{
		PostNcDestroy();
		return FALSE;
	}

	_AfxHookWindowCreate(this);
	HWND hWnd = ::CreateWindowEx(cs.dwExStyle, cs.lpszClass,
			cs.lpszName, cs.style, cs.x, cs.y, cs.cx, cs.cy,
			cs.hwndParent, cs.hMenu, cs.hInstance, cs.lpCreateParams);
	if (!_AfxUnhookWindowCreate())
		PostNcDestroy();        // cleanup if CreateWindowEx fails too soon

	if (hWnd == NULL)
		return FALSE;
	ASSERT(hWnd == m_hWnd); // should have been set in send msg hook
	return TRUE;
}

// for child windows
BOOL CWnd::PreCreateWindow(CREATESTRUCT& cs)
{
	if (cs.lpszClass == NULL)
	{
		// no WNDCLASS provided - use child window default
		ASSERT(cs.style & WS_CHILD);
		cs.lpszClass = _afxWnd;
	}
	return TRUE;
}

BOOL CWnd::Create(LPCSTR lpszClassName,
	LPCSTR lpszWindowName, DWORD dwStyle,
	const RECT& rect,
	CWnd* pParentWnd, UINT nID,
	CCreateContext* pContext /* = NULL */)
{
	// can't use for desktop or pop-up windows (use CreateEx instead)
	ASSERT(pParentWnd != NULL);
	ASSERT((dwStyle & WS_POPUP) == 0);

	return CreateEx(0, lpszClassName, lpszWindowName,
		dwStyle | WS_CHILD,
		rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top,
		pParentWnd->GetSafeHwnd(), (HMENU)nID, (LPSTR)pContext);
}

CWnd::~CWnd()
{
	if (m_hWnd != NULL &&
		this != (CWnd*)&wndTop && this != (CWnd*)&wndBottom &&
		this != (CWnd*)&wndTopMost && this != (CWnd*)&wndNoTopMost)
	{
		TRACE0("Warning: calling DestroyWindow in CWnd::~CWnd\n");
		TRACE0("\tOnDestroy or PostNcDestroy in derived class will not be called\n");
		DestroyWindow();
	}
}

void CWnd::OnNcDestroy()
{
	// WM_NCDESTROY is the absolute LAST message sent.
	if (AfxGetApp()->m_pMainWnd == this)
		AfxGetApp()->m_pMainWnd = NULL;

	if (AfxGetApp()->m_pActiveWnd == this)
		AfxGetApp()->m_pActiveWnd = NULL;

	// cleanup OLE 2.0 drop target interface
	if (m_pDropTarget != NULL)
	{
		m_pDropTarget->Revoke();
		m_pDropTarget = NULL;
	}

	Default();
	Detach();
	ASSERT(m_hWnd == NULL);
	// call special post-cleanup routine
	PostNcDestroy();
}

void CWnd::PostNcDestroy()
{
	// default to nothing
}

void CWnd::OnFinalRelease()
{
	if (m_hWnd != NULL)
		DestroyWindow();    // will call PostNcDestroy
	else
		PostNcDestroy();
}

#ifdef _DEBUG
void CWnd::AssertValid() const
{
	if (m_hWnd == NULL)
		return;     // null (unattached) windows are valid

	// check for special wnd??? values
	ASSERT(HWND_TOP == NULL);       // same as desktop
	if (m_hWnd == HWND_BOTTOM)
		ASSERT(this == &CWnd::wndBottom);
	else if (m_hWnd == HWND_TOPMOST)
		ASSERT(this == &CWnd::wndTopMost);
	else if (m_hWnd == HWND_NOTOPMOST)
		ASSERT(this == &CWnd::wndNoTopMost);
	else
	{
		// should be a normal window
		ASSERT(::IsWindow(m_hWnd));
		// should also be in the permanent or temporary handle map
		CObject* p;
		ASSERT(_afxMapHWND.LookupPermanent(m_hWnd, p) ||
			_afxMapHWND.LookupTemporary(m_hWnd, p));
		ASSERT((CWnd*)p == this);   // must be us
	}
}

void CWnd::Dump(CDumpContext& dc) const
{
	CObject::Dump(dc);

	AFX_DUMP0(dc, "with window information:\n");
	AFX_DUMP1(dc, "m_hWnd = ", (UINT)m_hWnd);

	if (m_hWnd == NULL || m_hWnd == HWND_BOTTOM ||
		m_hWnd == HWND_TOPMOST || m_hWnd == HWND_NOTOPMOST)
	{
		// not a normal window - nothing more to dump
		return;
	}

	if (!::IsWindow(m_hWnd))
	{
		// not a valid window
		AFX_DUMP0(dc, " = ILLEGAL HWND");
		return; // don't do anything more
	}

	if (CWnd::FromHandlePermanent(m_hWnd) != this)
	{
		AFX_DUMP0(dc, " - Detached or temporary window");
		return; // don't do anything more
	}

	// dump out window specific statistics
	char szBuf [64];
	GetWindowText(szBuf, sizeof (szBuf));
	AFX_DUMP1(dc, "\ncaption = \"", szBuf);
	AFX_DUMP0(dc, "\"");

	::GetClassName(m_hWnd, szBuf, sizeof (szBuf));
	AFX_DUMP1(dc, "\nclass name = \"", szBuf);
	AFX_DUMP0(dc, "\"");

	CRect rect;
	GetWindowRect(&rect);
	AFX_DUMP1(dc, "\nrect = ", rect);
	AFX_DUMP1(dc, "\nparent CWnd* = ", (void*)GetParent());

	AFX_DUMP1(dc, "\nstyle = ", (void FAR*)::GetWindowLong(m_hWnd, GWL_STYLE));
	if (::GetWindowLong(m_hWnd, GWL_STYLE) & WS_CHILD)
		AFX_DUMP1(dc, "\nid = ", _AfxGetDlgCtrlID(m_hWnd));
}
#endif

BOOL CWnd::DestroyWindow()
{
	if (m_hWnd == NULL)
		return FALSE;

	CObject* p;
	BOOL bInPermanentMap = _afxMapHWND.LookupPermanent(m_hWnd, p);
#ifdef _DEBUG
	HWND hWndOrig = m_hWnd;
#endif
	BOOL bRet = ::DestroyWindow(m_hWnd);
	// Note that 'this' may have been deleted at this point.
	if (bInPermanentMap)
	{
		// Should have been detached by OnNcDestroy
		ASSERT(!_afxMapHWND.LookupPermanent(hWndOrig, p));
	}
	else
	{
		ASSERT(m_hWnd == hWndOrig);
		// Detach after DestroyWindow called just in case
		Detach();
	}
	return bRet;
}

/////////////////////////////////////////////////////////////////////////////
// Default CWnd implementation

LRESULT CWnd::DefWindowProc(UINT nMsg, WPARAM wParam, LPARAM lParam)
{
	WNDPROC pfnWndProc;

	if ((pfnWndProc = *GetSuperWndProcAddr()) == NULL)
		return ::DefWindowProc(m_hWnd, nMsg, wParam, lParam);
	else
#ifdef STRICT
		return ::CallWindowProc(pfnWndProc, m_hWnd, nMsg, wParam, lParam);
#else
		return ::CallWindowProc((FARPROC)pfnWndProc, m_hWnd, nMsg, wParam, lParam);
#endif
}

WNDPROC* CWnd::GetSuperWndProcAddr()
{
	static WNDPROC NEAR pfnSuper = NULL;
	ASSERT(pfnSuper == NULL);       // should never be changed !!!
					// if this is non-NULL, then a derived class of CWnd
					//  forgot to override 'superWndProc' as well as 'className'
	return &pfnSuper;
}

BOOL CWnd::PreTranslateMessage(MSG*)
{
	// no default processing
	return FALSE;
}

/////////////////////////////////////////////////////////////////////////////
// CWnd will delegate owner draw messages to self drawing controls

// Drawing: for all 4 control types
void CWnd::OnDrawItem(int /*nIDCtl*/, LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	if (lpDrawItemStruct->CtlType == ODT_MENU)
	{
		CMenu* pMenu = CMenu::FromHandle((HMENU)lpDrawItemStruct->hwndItem);
		if (pMenu != NULL)
		{
			pMenu->DrawItem(lpDrawItemStruct);
			return; // eat it
		}
	}
	else
	{
		CWnd* pChild = CWnd::FromHandlePermanent(lpDrawItemStruct->hwndItem);
		if (pChild != NULL && pChild->SendChildNotifyLastMsg())
			return;     // eat it
	}
	// not handled - do default
	Default();
}

// Drawing: for all 4 control types
int CWnd::OnCompareItem(int /*nIDCtl*/, LPCOMPAREITEMSTRUCT lpCompareItemStruct)
{
	CWnd* pChild = CWnd::FromHandlePermanent(lpCompareItemStruct->hwndItem);
	if (pChild != NULL)
	{
		LRESULT lResult;
		if (pChild->SendChildNotifyLastMsg(&lResult))
			return (int)lResult;        // eat it
	}
	// not handled - do default
	return (int)Default();
}

void CWnd::OnDeleteItem(int /*nIDCtl*/, LPDELETEITEMSTRUCT lpDeleteItemStruct)
{
	CWnd* pChild = CWnd::FromHandlePermanent(lpDeleteItemStruct->hwndItem);
	if (pChild != NULL)
	{
		if (pChild->SendChildNotifyLastMsg())
			return;     // eat it
	}
	// not handled - do default
	Default();
}

/////////////////////////////////////////////////////////////////////////////
// Self drawing menus are a little trickier

static HWND _afxTrackingWindow = NULL;
HMENU _afxTrackingMenu = NULL;  // public for CFrameWnd access

BOOL CMenu::TrackPopupMenu(UINT nFlags, int x, int y,
		CWnd* pWnd, LPCRECT lpRect)
{
	ASSERT(m_hMenu != NULL);

	HWND hWndOld = _afxTrackingWindow;
	HMENU hMenuOld = _afxTrackingMenu;
	_afxTrackingWindow = pWnd->GetSafeHwnd();
	_afxTrackingMenu = m_hMenu;
	BOOL bOK = ::TrackPopupMenu(m_hMenu, nFlags, x, y, 0,
			_afxTrackingWindow, lpRect);
	_afxTrackingWindow = hWndOld;
	_afxTrackingMenu = hMenuOld;

	return bOK;
}

static CMenu* FindPopupMenuFromID(CMenu* pMenu, UINT nID)
{
	ASSERT_VALID(pMenu);
	// walk through all items, looking for ID match
	UINT nItems = pMenu->GetMenuItemCount();
	for (int iItem = 0; iItem < (int)nItems; iItem++)
	{
		CMenu* pPopup = pMenu->GetSubMenu(iItem);
		if (pPopup != NULL)
		{
			// recurse to child popup
			pPopup = FindPopupMenuFromID(pPopup, nID);
			// try recursing
			if (pPopup != NULL)
				return pPopup;
		}
		else if (pMenu->GetMenuItemID(iItem) == nID)
		{
			// it is a normal item inside our popup
			return pMenu;
		}
	}
	// not found
	return NULL;
}

// Measure item implementation relies on unique control/menu IDs
void CWnd::OnMeasureItem(int /*nIDCtl*/, LPMEASUREITEMSTRUCT lpMeasureItemStruct)
{
	if (lpMeasureItemStruct->CtlType == ODT_MENU)
	{
		ASSERT(lpMeasureItemStruct->CtlID == 0);
		CMenu* pMenu;

		if (_afxTrackingWindow == m_hWnd)
			pMenu = CMenu::FromHandle(_afxTrackingMenu);    // start from popup
		else
			pMenu = GetMenu();      // start from menubar

		pMenu = FindPopupMenuFromID(pMenu, lpMeasureItemStruct->itemID);
		if (pMenu != NULL)
			pMenu->MeasureItem(lpMeasureItemStruct);
		else
			TRACE1("Warning: unknown WM_MEASUREITEM request for"
				" menu item 0x%04X\n", lpMeasureItemStruct->itemID);
	}
	else
	{
		CWnd* pChild = GetDescendantWindow(lpMeasureItemStruct->CtlID, TRUE);
		if (pChild != NULL && pChild->SendChildNotifyLastMsg())
			return;     // eaten by child
	}
	// not handled - do default
	Default();
}

/////////////////////////////////////////////////////////////////////////////
// Additional helpers for WNDCLASS init

const char* AFXAPI AfxRegisterWndClass(UINT nClassStyle,
	HCURSOR hCursor, HBRUSH hbrBackground, HICON hIcon)
{
	// Returns a temporary string name for the class
	//  Save in a CString if you want to use it for a long time
	WNDCLASS wndcls;
	static char NEAR szName[64];     // 1 global string (modal usage)

	// generate a synthetic name for this class
	if (hCursor == NULL && hbrBackground == NULL && hIcon == NULL)
		wsprintf(szName, "Afx:%x", nClassStyle);
	else
		wsprintf(szName, "Afx:%x:%x:%x:%x", nClassStyle,
			(UINT) hCursor, (UINT) hbrBackground, (UINT) hIcon);

	// see if the class already exists
	if (::GetClassInfo(AfxGetInstanceHandle(), szName, &wndcls))
	{
		// already registered, assert everything is good
		ASSERT(wndcls.style == nClassStyle);

		// NOTE: We have to trust that the hIcon, hbrBackground, and the
		//  hCursor are semantically the same, because sometimes Windows does
		//  some internal translation or copying of those handles before
		//  storing them in the internal WNDCLASS retrieved by GetClassInfo.
		return szName;
	}

	// otherwise we need to register a new class
	wndcls.style = nClassStyle;
	wndcls.lpfnWndProc = AfxWndProc;
	wndcls.cbClsExtra = wndcls.cbWndExtra = 0;
	wndcls.hInstance = AfxGetInstanceHandle();
	wndcls.hIcon = hIcon;
	wndcls.hCursor = hCursor;
	wndcls.hbrBackground = hbrBackground;
	wndcls.lpszMenuName = NULL;
	wndcls.lpszClassName = szName;
	if (!::RegisterClass(&wndcls))
		AfxThrowResourceException();

	return szName;
}

/////////////////////////////////////////////////////////////////////////////
// CWnd extensions for help support

void CWnd::WinHelp(DWORD dwData, UINT nCmd /* = HELP_CONTEXT */)
{
	ASSERT_VALID(AfxGetApp());
	ASSERT(AfxGetApp()->m_pszHelpFilePath != NULL);

	BeginWaitCursor();
	if (IsFrameWnd())
	{
		// CFrameWnd windows should be allowed to exit help mode first
		CFrameWnd* pFrameWnd = (CFrameWnd*)this;
		pFrameWnd->ExitHelpMode();
	}

	// cancel any tracking modes
	SendMessage(WM_CANCELMODE);
	SendMessageToDescendants(WM_CANCELMODE, 0, 0, TRUE, TRUE);

	// need to use top level parent (for the case where m_hWnd is in DLL)
	CWnd* pWnd = GetTopLevelParent();
	pWnd->SendMessage(WM_CANCELMODE);
	pWnd->SendMessageToDescendants(WM_CANCELMODE, 0, 0, TRUE, TRUE);

	// attempt to cancel capture
	HWND hWndCapture = ::GetCapture();
	if (hWndCapture != NULL)
		::SendMessage(hWndCapture, WM_CANCELMODE, 0, 0);

	TRACE3("WinHelp: pszHelpFile = '%s', dwData: $%lx, fuCommand: %d\n",
		AfxGetApp()->m_pszHelpFilePath, dwData, nCmd);

	// finally, run the Windows Help engine
	if (!::WinHelp(pWnd->m_hWnd, AfxGetApp()->m_pszHelpFilePath, nCmd, dwData))
		AfxMessageBox(AFX_IDP_FAILED_TO_LAUNCH_HELP);
	EndWaitCursor();
}

/////////////////////////////////////////////////////////////////////////////
// Message table implementation

IMPLEMENT_DYNCREATE(CWnd, CCmdTarget)

BEGIN_MESSAGE_MAP(CWnd, CCmdTarget)
	//{{AFX_MSG_MAP(CWnd)
	ON_WM_COMPAREITEM()
	ON_WM_MEASUREITEM()
	ON_WM_DRAWITEM()
	ON_WM_DELETEITEM()
	ON_WM_CTLCOLOR()
	ON_WM_NCDESTROY()

	// VBX support and control reflection
	ON_MESSAGE(WM_VBXEVENT, OnVBXEvent)

	// VBX support
	ON_WM_HSCROLL()
	ON_WM_VSCROLL()
	ON_WM_PARENTNOTIFY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

union MessageMapFunctions
{
	AFX_PMSG pfn;   // generic member function pointer

	// specific type safe variants
	BOOL    (AFX_MSG_CALL CWnd::*pfn_bD)(CDC*);
	BOOL    (AFX_MSG_CALL CWnd::*pfn_bb)(BOOL);
	BOOL    (AFX_MSG_CALL CWnd::*pfn_bWww)(CWnd*, UINT, UINT);
	HBRUSH  (AFX_MSG_CALL CWnd::*pfn_hDWw)(CDC*, CWnd*, UINT);
	int     (AFX_MSG_CALL CWnd::*pfn_iwWw)(UINT, CWnd*, UINT);
	int     (AFX_MSG_CALL CWnd::*pfn_iWww)(CWnd*, UINT, UINT);
	int     (AFX_MSG_CALL CWnd::*pfn_is)(LPSTR);
	LRESULT (AFX_MSG_CALL CWnd::*pfn_lwl)(WPARAM, LPARAM);
	LRESULT (AFX_MSG_CALL CWnd::*pfn_lwwM)(UINT, UINT, CMenu*);
	void    (AFX_MSG_CALL CWnd::*pfn_vv)(void);

	void    (AFX_MSG_CALL CWnd::*pfn_vw)(UINT);
	void    (AFX_MSG_CALL CWnd::*pfn_vww)(UINT, UINT);
	void    (AFX_MSG_CALL CWnd::*pfn_vvii)(int, int);
	void    (AFX_MSG_CALL CWnd::*pfn_vwww)(UINT, UINT, UINT);
	void    (AFX_MSG_CALL CWnd::*pfn_vwii)(UINT, int, int);
	void    (AFX_MSG_CALL CWnd::*pfn_vwl)(WPARAM, LPARAM);
	void    (AFX_MSG_CALL CWnd::*pfn_vbWW)(BOOL, CWnd*, CWnd*);
	void    (AFX_MSG_CALL CWnd::*pfn_vD)(CDC*);
	void    (AFX_MSG_CALL CWnd::*pfn_vM)(CMenu*);
	void    (AFX_MSG_CALL CWnd::*pfn_vMwb)(CMenu*, UINT, BOOL);

	void    (AFX_MSG_CALL CWnd::*pfn_vW)(CWnd*);
	void    (AFX_MSG_CALL CWnd::*pfn_vWww)(CWnd*, UINT, UINT);
	void    (AFX_MSG_CALL CWnd::*pfn_vWh)(CWnd*, HANDLE);
	void    (AFX_MSG_CALL CWnd::*pfn_vwW)(UINT, CWnd*);
	void    (AFX_MSG_CALL CWnd::*pfn_vwWb)(UINT, CWnd*, BOOL);
	void    (AFX_MSG_CALL CWnd::*pfn_vwwW)(UINT, UINT, CWnd*);
	void    (AFX_MSG_CALL CWnd::*pfn_vs)(LPSTR);
	void    (AFX_MSG_CALL CWnd::*pfn_vOWNER)(int, LPSTR);   // force return TRUE
	int     (AFX_MSG_CALL CWnd::*pfn_iis)(int, LPSTR);
	UINT    (AFX_MSG_CALL CWnd::*pfn_wp)(CPoint);
	UINT    (AFX_MSG_CALL CWnd::*pfn_wv)(void);
	void    (AFX_MSG_CALL CWnd::*pfn_vPOS)(WINDOWPOS FAR*);
	void    (AFX_MSG_CALL CWnd::*pfn_vCALC)(BOOL, NCCALCSIZE_PARAMS FAR*);
};

/////////////////////////////////////////////////////////////////////////////
// Routines for fast search of message maps

#ifdef _PORTABLE
// C versions of search routines
AFX_MSGMAP_ENTRY FAR* PASCAL
_AfxFindMessageEntry(AFX_MSGMAP_ENTRY FAR* lpEntry, UINT nMsg, UINT nID)
{
	while (lpEntry->nSig != AfxSig_end)
	{
		if (lpEntry->nMessage == nMsg && lpEntry->nID == nID)
			return lpEntry;
		lpEntry++;
	}
	return NULL;    // not found
}
#else
// Hand tuned routine

#pragma optimize("qgel", off) // assembler cannot be globally optimized

AFX_MSGMAP_ENTRY FAR* PASCAL
_AfxFindMessageEntry(AFX_MSGMAP_ENTRY FAR* lpEntry, UINT nMsg, UINT nID)
{
// 16-bit Intel x86 version.

	ASSERT(offsetof(AFX_MSGMAP_ENTRY, nMessage) == 0);
	ASSERT(offsetof(AFX_MSGMAP_ENTRY, nID) == 2);
	ASSERT(offsetof(AFX_MSGMAP_ENTRY, nSig) == 4);

	_asm
	{
			LES     BX,lpEntry
			MOV     AX,nMsg
			MOV     DX,nID
	__loop:
			MOV     CX,WORD PTR ES:[BX+4]       ; nSig (0 => end)
			JCXZ    __failed
			CMP     AX,WORD PTR ES:[BX]         ; nMessage
			JE      __found_1
	__next:
			ADD     BX,SIZE AFX_MSGMAP_ENTRY
			JMP     short __loop
	__found_1:
			CMP     DX,WORD PTR ES:[BX+2]       ; nID
			JNE     __next
	   // found a match
			MOV     WORD PTR lpEntry,BX         ; return ES:BX
			JMP     short __end
	__failed:
			XOR     AX,AX                       ; return NULL
			MOV     WORD PTR lpEntry,AX
			MOV     WORD PTR lpEntry+2,AX
	__end:
	}
	return lpEntry;
}

#pragma optimize("", on)    // return to default optimizations
#endif  // !_PORTABLE

/////////////////////////////////////////////////////////////////////////////
// Cache of most recently sent messages

#ifndef iHashMax
// iHashMax must be a power of two
	#ifdef _NEARDATA
		#define iHashMax 64
	#else
		#define iHashMax 256
	#endif
#endif

struct AFX_MSG_CACHE
{
	UINT nMsg;
	AFX_MSGMAP_ENTRY FAR* lpEntry;
	AFX_MSGMAP* pMessageMap;
};

AFX_MSG_CACHE _afxMsgCache[iHashMax];

void AFXAPI AfxResetMsgCache()
{
	memset(_afxMsgCache, 0, sizeof(_afxMsgCache));
}

LRESULT CWnd::WindowProc(UINT nMsg, WPARAM wParam, LPARAM lParam)
{
	AFX_MSGMAP* pMessageMap;
	AFX_MSGMAP_ENTRY FAR* lpEntry;

	if (nMsg == WM_COMMAND) // special case for commands
	{
		if (OnCommand(wParam, lParam))
			return 1L; // command handled
		else
			return (LRESULT)DefWindowProc(nMsg, wParam, lParam);
	}

	pMessageMap = GetMessageMap();
	UINT iHash = (_AFX_FP_OFF(pMessageMap) ^ nMsg) & (iHashMax-1);
	AFX_MSG_CACHE& msgCache = _afxMsgCache[iHash];

	if (nMsg == msgCache.nMsg && pMessageMap == msgCache.pMessageMap)
	{
		// Cache hit
		lpEntry = msgCache.lpEntry;
		if (lpEntry == NULL)
			return (LRESULT)DefWindowProc(nMsg, wParam, lParam);
		else if (nMsg < 0xC000)
			goto LDispatch;
		else
			goto LDispatchRegistered;
	}
	else
	{
		// not in cache, look for it
		msgCache.nMsg = nMsg;
		msgCache.pMessageMap = pMessageMap;

		for (/* pMessageMap already init'ed */; pMessageMap != NULL;
			pMessageMap = pMessageMap->pBaseMessageMap)
		{
			// This may loop forever if the message maps are not properly
			// chained together.  Make sure each window class's message map
			// points to the base window class's message map.

			if (nMsg < 0xC000)
			{
				// constant window message
				if ((lpEntry = _AfxFindMessageEntry(pMessageMap->lpEntries,
					nMsg, 0)) != NULL)
				{
					msgCache.lpEntry = lpEntry;
					goto LDispatch;
				}
			}
			else
			{
				// registered windows message
				lpEntry = pMessageMap->lpEntries;

				while ((lpEntry = _AfxFindMessageEntry(lpEntry, 0xC000, 0))
					  != NULL)
				{
#ifndef _AFXDLL
					UINT NEAR* pnID = (UINT NEAR*)(lpEntry->nSig);
#else
					// if SS!=DS, then REGISTERED message must be
					//   in same data segment as the message map
					UINT FAR* pnID = (UINT FAR*)MAKELONG(lpEntry->nSig,
							_AFX_FP_SEG(pMessageMap));
#endif
					ASSERT(*pnID >= 0xC000);
						// must be successfully registered
					if (*pnID == nMsg)
					{
						msgCache.lpEntry = lpEntry;
						goto LDispatchRegistered;
					}
					lpEntry++;      // keep looking past this one
				}
			}
		}

		msgCache.lpEntry = NULL;
		return DefWindowProc(nMsg, wParam, lParam);
	}
	ASSERT(FALSE);      // not reached

LDispatch:
	ASSERT(nMsg < 0xC000);
	union MessageMapFunctions mmf;
	mmf.pfn = lpEntry->pfn;

	switch (lpEntry->nSig)
	{
	default:
		ASSERT(FALSE);
		return 0;

	case AfxSig_bD:
		return (this->*mmf.pfn_bD)(CDC::FromHandle((HDC)wParam));

	case AfxSig_bb:     // AfxSig_bb, AfxSig_bw, AfxSig_bh
		return (this->*mmf.pfn_bb)((BOOL)wParam);

	case AfxSig_bWww:   // really AfxSig_bWiw
		return (this->*mmf.pfn_bWww)(CWnd::FromHandle((HWND)wParam),
			(short)LOWORD(lParam), HIWORD(lParam));

	case AfxSig_hDWw:
		{
			// special case for OnCtlColor to avoid too many temporary objects
			CDC dcTemp;
			CWnd wndTemp;
			UINT nCtlType;

			dcTemp.m_hDC = (HDC)wParam;
			wndTemp.m_hWnd = (HWND)LOWORD(lParam);
			nCtlType = HIWORD(lParam);
			CWnd* pWnd = CWnd::FromHandlePermanent(wndTemp.m_hWnd);
			// if not coming from a permanent window, use stack temporary
			if (pWnd == NULL)
				pWnd = &wndTemp;
			HBRUSH hbr = (this->*mmf.pfn_hDWw)(&dcTemp, pWnd, nCtlType);
			// fast detach of temporary objects
			dcTemp.m_hDC = NULL;
			wndTemp.m_hWnd = NULL;
			return (LRESULT)(UINT)hbr;
		}

	case AfxSig_iwWw:
		return (this->*mmf.pfn_iwWw)(wParam,
			CWnd::FromHandle((HWND)LOWORD(lParam)), HIWORD(lParam));

	case AfxSig_iWww:   // really AfxSig_iWiw
		return (this->*mmf.pfn_iWww)(CWnd::FromHandle((HWND)wParam),
			(short)LOWORD(lParam), HIWORD(lParam));

	case AfxSig_is:
		return (this->*mmf.pfn_is)((LPSTR)lParam);

	case AfxSig_lwl:
		return (this->*mmf.pfn_lwl)(wParam, lParam);

	case AfxSig_lwwM:
		return (this->*mmf.pfn_lwwM)(wParam, LOWORD(lParam),
			CMenu::FromHandle((HMENU)HIWORD(lParam)));

	case AfxSig_vv:
		(this->*mmf.pfn_vv)();
		return 0;

	case AfxSig_vw: // AfxSig_vb, AfxSig_vh
		(this->*mmf.pfn_vw)(wParam);
		return 0;

	case AfxSig_vww:
		(this->*mmf.pfn_vww)(wParam, LOWORD(lParam));
		return 0;

	case AfxSig_vvii:
		(this->*mmf.pfn_vvii)(LOWORD(lParam), HIWORD(lParam));
		return 0;

	case AfxSig_vwww:
		(this->*mmf.pfn_vwww)(wParam, LOWORD(lParam), HIWORD(lParam));
		return 0;

	case AfxSig_vwii:
		(this->*mmf.pfn_vwii)(wParam, LOWORD(lParam), HIWORD(lParam));
		return 0;

	case AfxSig_vwl:
		(this->*mmf.pfn_vwl)(wParam, lParam);
		return 0;

	case AfxSig_vbWW:
		(this->*mmf.pfn_vbWW)((BOOL)wParam,
			CWnd::FromHandle((HWND)LOWORD(lParam)),
			CWnd::FromHandle((HWND)HIWORD(lParam)));
		return 0;

	case AfxSig_vD:
		(this->*mmf.pfn_vD)(CDC::FromHandle((HDC)wParam));
		return 0;

	case AfxSig_vM:
		(this->*mmf.pfn_vM)(CMenu::FromHandle((HMENU)wParam));
		return 0;

	case AfxSig_vMwb:
		(this->*mmf.pfn_vMwb)(CMenu::FromHandle((HMENU)wParam),
			LOWORD(lParam), (BOOL)HIWORD(lParam));
		return 0;

	case AfxSig_vW:
		(this->*mmf.pfn_vW)(CWnd::FromHandle((HWND)wParam));
		return 0;

	case AfxSig_vWww:
		(this->*mmf.pfn_vWww)(CWnd::FromHandle((HWND)wParam), LOWORD(lParam),
			HIWORD(lParam));
		return 0;

	case AfxSig_vWh:
		(this->*mmf.pfn_vWh)(CWnd::FromHandle((HWND)wParam),
				(HANDLE)LOWORD(lParam));
		return 0;

	case AfxSig_vwW:
		(this->*mmf.pfn_vwW)(wParam, CWnd::FromHandle((HWND)LOWORD(lParam)));
		return 0;

	case AfxSig_vwWb:
		(this->*mmf.pfn_vwWb)(wParam, CWnd::FromHandle((HWND)LOWORD(lParam)),
			(BOOL)HIWORD(lParam));
		return 0;

	case AfxSig_vwwW:
		(this->*mmf.pfn_vwwW)(wParam, LOWORD(lParam),
			CWnd::FromHandle((HWND)HIWORD(lParam)));
		return 0;

	case AfxSig_vs:
		(this->*mmf.pfn_vs)((LPSTR)lParam);
		return 0;

	case AfxSig_vOWNER:
		(this->*mmf.pfn_vOWNER)((int)wParam, (LPSTR)lParam);
		return TRUE;

	case AfxSig_iis:
		return (this->*mmf.pfn_iis)((int)wParam, (LPSTR)lParam);

	case AfxSig_wp: // really AfxSig_ip
		return (short)(this->*mmf.pfn_wp)(*(CPoint*)&lParam);

	case AfxSig_wv: // AfxSig_bv, AfxSig_wv
		return (this->*mmf.pfn_wv)();

	case AfxSig_vCALC:
		(this->*mmf.pfn_vCALC)((BOOL)wParam, (NCCALCSIZE_PARAMS FAR*)lParam);
		return 0;

	case AfxSig_vPOS:
		(this->*mmf.pfn_vPOS)((WINDOWPOS FAR*)lParam);
		return 0;

	}
	ASSERT(FALSE);      // not reached

LDispatchRegistered:    // for registered windows messages
	ASSERT(nMsg >= 0xC000);
	mmf.pfn = lpEntry->pfn;
	return (this->*mmf.pfn_lwl)(wParam, lParam);
}


/////////////////////////////////////////////////////////////////////////////

static HWND _afxLockoutNotificationWindow = NULL;

BOOL CWnd::OnCommand(WPARAM wParam, LPARAM lParam)
	// return TRUE if command invocation was attempted
{
	UINT nID = LOWORD(wParam);
	HWND hWndCtrl = (HWND)LOWORD(lParam);
	int nCode = HIWORD(lParam);

	// default routing for command messages (through closure table)

	if (nID == 0)
		return FALSE;       // 0 control IDs are not allowed!

	if (hWndCtrl == NULL)
	{
		// menu or accelerator
		nCode = CN_COMMAND;
	}
	else
	{
		// control notification
		ASSERT(::IsWindow(hWndCtrl));

		if (_afxLockoutNotificationWindow == m_hWnd)
			return TRUE;        // locked out - ignore control notification

		// Reflect notification to child window control
		CWnd* pChild = CWnd::FromHandlePermanent(hWndCtrl);
		if (pChild != NULL && pChild->SendChildNotifyLastMsg())
			return TRUE;        // eaten by child

		// handle in parent
	}

#ifdef _DEBUG
	if (nCode < 0 && nCode != (int)0x8000)
		TRACE1("Implementation Warning: control notification = $%x\n", nCode);
#endif

	return OnCmdMsg(nID, nCode, NULL, NULL);
}

/////////////////////////////////////////////////////////////////////////////
// CWnd extensions

BOOL PASCAL _AfxIsDescendant(HWND hWndParent, HWND hWndChild)
	// helper for detecting whether child descendent of parent
	//  (works with owned popups as well)
{
	ASSERT(::IsWindow(hWndParent));
	ASSERT(::IsWindow(hWndChild));

	do
	{
		if (hWndParent == hWndChild)
			return TRUE;

		hWndChild = _AfxGetParentOwner(hWndChild);
	} while (hWndChild != NULL);

	return FALSE;
}

CFrameWnd* CWnd::GetParentFrame() const
{
	ASSERT_VALID(this);

	if (m_hWnd == NULL) // no Window attached
		return NULL;

	CWnd* p = GetOwner();  // start with one parent up
	while (p != NULL)
	{
		if (p->IsFrameWnd())
			return (CFrameWnd*)p;
		p = p->GetOwner();
	}
	return NULL;
}

CWnd* CWnd::GetTopLevelParent() const
{
	if (this == NULL || m_hWnd == NULL) // no Window attached
		return NULL;

	ASSERT_VALID(this);

	HWND hWndParent = m_hWnd;
	HWND hWndT;
	while ((hWndT = _AfxGetParentOwner(hWndParent)) != NULL)
		hWndParent = hWndT;

	return CWnd::FromHandle(hWndParent);
}

CFrameWnd* CWnd::GetTopLevelFrame() const
{
	if (this == NULL || m_hWnd == NULL) // no Window attached
		return NULL;

	ASSERT_VALID(this);

	CFrameWnd* pFrameWnd = (CFrameWnd*)this;
	if (!IsFrameWnd())
		pFrameWnd = GetParentFrame();

	CFrameWnd* pTemp = pFrameWnd;
	if (pFrameWnd != NULL)
	{
		while ((pTemp = pFrameWnd->GetParentFrame()) != NULL)
			pFrameWnd = pTemp;
	}

	return pFrameWnd;
}

CWnd* PASCAL CWnd::GetDescendantWindow(HWND hWnd, int nID, BOOL bOnlyPerm)
{
	// GetDlgItem recursive (return first found)
	// breadth-first for 1 level, then depth-first for next level

	// use GetDlgItem since it is a fast USER function
	HWND hWndChild;
	CWnd* pWndChild;
	if ((hWndChild = ::GetDlgItem(hWnd, nID)) != NULL)
	{
		if (::GetTopWindow(hWndChild) != NULL)
		{
			// children with the same ID as their parent have priority
			pWndChild = GetDescendantWindow(hWndChild, nID, bOnlyPerm);
			if (pWndChild != NULL)
				return pWndChild;
		}
		// return temporary handle if allowed
		if (!bOnlyPerm)
			return CWnd::FromHandle(hWndChild);

		// return only permanent handle
		pWndChild = CWnd::FromHandlePermanent(hWndChild);
		if (pWndChild != NULL)
			return pWndChild;
	}

	// walk each child
	for (hWndChild = ::GetTopWindow(hWnd); hWndChild != NULL;
		hWndChild = ::GetNextWindow(hWndChild, GW_HWNDNEXT))
	{
		pWndChild = GetDescendantWindow(hWndChild, nID, bOnlyPerm);
		if (pWndChild != NULL)
			return pWndChild;
	}
	return NULL;    // not found
}

void PASCAL CWnd::SendMessageToDescendants(HWND hWnd, UINT message,
	WPARAM wParam, LPARAM lParam, BOOL bDeep, BOOL bOnlyPerm)
{
	// walk through HWNDs to avoid creating temporary CWnd objects
	// unless we need to call this function recursively
	for (HWND hWndChild = ::GetTopWindow(hWnd); hWndChild != NULL;
		hWndChild = ::GetNextWindow(hWndChild, GW_HWNDNEXT))
	{
		// if bOnlyPerm is TRUE, don't send to non-permanent windows
		if (bOnlyPerm)
		{
			CWnd* pWnd = CWnd::FromHandlePermanent(hWndChild);
			if (pWnd != NULL)
			{
				// call window proc directly since it is a C++ window
				_AfxCallWndProc(pWnd, pWnd->m_hWnd, message, wParam, lParam);
			}
		}
		else
		{
			// send message with Windows SendMessage API
			::SendMessage(hWndChild, message, wParam, lParam);
		}
		if (bDeep && ::GetTopWindow(hWndChild) != NULL)
		{
			// send to child windows after parent
			SendMessageToDescendants(hWndChild, message, wParam, lParam,
				bDeep, bOnlyPerm);
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
// Scroll bar helpers
//  hook for CWnd functions
//    only works for derived class (eg: CView) that override 'GetScrollBarCtrl'
// if the window doesn't have a _visible_ windows scrollbar - then
//   look for a sibling with the appropriate ID

CScrollBar* CWnd::GetScrollBarCtrl(int) const
{
	return NULL;        // no special scrollers supported
}


int CWnd::SetScrollPos(int nBar, int nPos, BOOL bRedraw)
{
	CScrollBar* pScrollBar;
	if ((pScrollBar = GetScrollBarCtrl(nBar)) != NULL)
		return pScrollBar->SetScrollPos(nPos, bRedraw);
	else
		return ::SetScrollPos(m_hWnd, nBar, nPos, bRedraw);
}


int CWnd::GetScrollPos(int nBar) const
{
	CScrollBar* pScrollBar;
	if ((pScrollBar = GetScrollBarCtrl(nBar)) != NULL)
		return pScrollBar->GetScrollPos();
	else
		return ::GetScrollPos(m_hWnd, nBar);
}


void CWnd::SetScrollRange(int nBar, int nMinPos, int nMaxPos, BOOL bRedraw)
{
	CScrollBar* pScrollBar;
	if ((pScrollBar = GetScrollBarCtrl(nBar)) != NULL)
		pScrollBar->SetScrollRange(nMinPos, nMaxPos, bRedraw);
	else
		::SetScrollRange(m_hWnd, nBar, nMinPos, nMaxPos, bRedraw);
}


void CWnd::GetScrollRange(int nBar, LPINT lpMinPos, LPINT lpMaxPos) const
{
	CScrollBar* pScrollBar;
	if ((pScrollBar = GetScrollBarCtrl(nBar)) != NULL)
		pScrollBar->GetScrollRange(lpMinPos, lpMaxPos);
	else
		::GetScrollRange(m_hWnd, nBar, lpMinPos, lpMaxPos);
}


// Turn on/off non-control scrollbars
//   for WS_?SCROLL scrollbars - show/hide them
//   for control scrollbar - enable/disable them
void CWnd::EnableScrollBarCtrl(int nBar, BOOL bEnable)
{
	CScrollBar* pScrollBar;
	if (nBar == SB_BOTH)
	{
		EnableScrollBarCtrl(SB_HORZ, bEnable);
		EnableScrollBarCtrl(SB_VERT, bEnable);
	}
	else if ((pScrollBar = GetScrollBarCtrl(nBar)) != NULL)
	{
		// control scrollbar - enable or disable
		pScrollBar->EnableWindow(bEnable);
	}
	else
	{
		// WS_?SCROLL scrollbar - show or hide
		ShowScrollBar(nBar, bEnable);
	}
}

/////////////////////////////////////////////////////////////////////////////
// minimal layout support

void CWnd::RepositionBars(UINT nIDFirst, UINT nIDLast, UINT nIDLeftOver,
	UINT nFlags, LPRECT lpRectParam, LPCRECT lpRectClient)
{
	ASSERT(nFlags == 0 || nFlags == reposQuery || nFlags == reposExtra);

	// walk kids in order, control bars get the resize notification
	//   which allow them to shrink the client area
	// remaining size goes to the 'nIDLeftOver' pane
	// NOTE: nIDFirst->nIDLast are usually 0->0xffff

	AFX_SIZEPARENTPARAMS layout;
	HWND hWndLeftOver = NULL;

	if (lpRectClient != NULL)
		layout.rect = *lpRectClient;    // starting rect comes from parameter
	else
		GetClientRect(&layout.rect);    // starting rect comes from client rect

	if (nFlags != reposQuery)
		layout.hDWP = ::BeginDeferWindowPos(8); // reasonable guess
	else
		layout.hDWP = NULL; // not actually doing layout

	for (HWND hWndChild = ::GetTopWindow(m_hWnd); hWndChild != NULL;
		hWndChild = ::GetNextWindow(hWndChild, GW_HWNDNEXT))
	{
		UINT nIDC = _AfxGetDlgCtrlID(hWndChild);
		if (nIDC == nIDLeftOver)
			hWndLeftOver = hWndChild;   // use it later
		else if (nIDC >= nIDFirst && nIDC <= nIDLast &&
					CWnd::FromHandlePermanent(hWndChild) != NULL)
			::SendMessage(hWndChild, WM_SIZEPARENT, 0, (LPARAM)(LPSTR)&layout);
		else
			::SetWindowLong(hWndChild, GWL_STYLE,
				::GetWindowLong(hWndChild, GWL_STYLE)|WS_CLIPSIBLINGS);
			// force WS_CLIPSIBLINGS for servers which may not include it
	}

	// if just getting the available rectangle, return it now...
	if (nFlags == reposQuery)
	{
		ASSERT(lpRectParam != NULL);
		::CopyRect(lpRectParam, &layout.rect);
		return;
	}

	// the rest is the client size of the left-over pane
	if (nIDLeftOver != 0 && hWndLeftOver != NULL)
	{
		CWnd* pLeftOver = CWnd::FromHandle(hWndLeftOver);
		// allow extra space as specified by lpRectBorder
		if (nFlags == reposExtra)
		{
			ASSERT(lpRectParam != NULL);
			layout.rect.left += lpRectParam->left;
			layout.rect.top += lpRectParam->top;
			layout.rect.right -= lpRectParam->right;
			layout.rect.bottom -= lpRectParam->bottom;
		}
		// reposition the window
		pLeftOver->CalcWindowRect(&layout.rect);
		_AfxRepositionWindow(&layout, hWndLeftOver, &layout.rect);
	}

	// move and resize all the windows at once!
	if (!::EndDeferWindowPos(layout.hDWP))
	{
#ifdef _DEBUG
		// Win3.1 and greater return success code, warn on failure
		// Don't warn when running under Windows NT/WOW -- it always
		//  returns failure.
		if (afxData.bWin31 && !(GetWinFlags() & 0x4000))
			TRACE0("Warning: DeferWindowPos failed - low system resources\n");
#endif
	}
}

void _AfxRepositionWindow(AFX_SIZEPARENTPARAMS FAR* lpLayout,
	HWND hWnd, LPCRECT lpRect)
{
	ASSERT(hWnd != NULL);
	ASSERT(lpRect != NULL);
	HWND hWndParent = ::GetParent(hWnd);
	ASSERT(hWndParent != NULL);

	// first check if the new rectangle is the same as the current
	CRect rectOld;
	::GetWindowRect(hWnd, rectOld);
	::ScreenToClient(hWndParent, &rectOld.TopLeft());
	::ScreenToClient(hWndParent, &rectOld.BottomRight());
	if (rectOld.EqualRect(lpRect))
		return;     // nothing to do

	if (lpLayout != NULL)
		lpLayout->hDWP = ::DeferWindowPos(lpLayout->hDWP, hWnd, NULL, lpRect->left,
			lpRect->top, lpRect->right - lpRect->left, lpRect->bottom - lpRect->top,
			SWP_NOACTIVATE|SWP_NOZORDER);
	else
		::SetWindowPos(hWnd, NULL, lpRect->left,
			lpRect->top, lpRect->right - lpRect->left, lpRect->bottom - lpRect->top,
			SWP_NOACTIVATE|SWP_NOZORDER);
}

void CWnd::CalcWindowRect(LPRECT lpClientRect, UINT nAdjustType)
{
	::AdjustWindowRect(lpClientRect, GetStyle(), FALSE);
}

/////////////////////////////////////////////////////////////////////////////

LRESULT CWnd::OnVBXEvent(WPARAM wParam, LPARAM lParam)
{
	ASSERT(wParam != 0);    // 0 control IDs are not allowed !

	AFX_VBXEVENTPARAMS FAR* lpEvent = (AFX_VBXEVENTPARAMS FAR*)lParam;

	UINT nNotifyCode = lpEvent->nNotifyCode;    // control specific

	return OnCmdMsg((UINT)wParam, nNotifyCode, &lpEvent, NULL);
}

BOOL CWnd::SendChildNotifyLastMsg(LRESULT* pLResult /*= NULL*/)
{
	return OnChildNotify(_afxLastMsg.message,
		_afxLastMsg.wParam, _afxLastMsg.lParam, pLResult);
}

BOOL CWnd::OnChildNotify(UINT, WPARAM, LPARAM, LRESULT*)
{
	return FALSE;       // let the parent have it
}

void CWnd::OnParentNotify(UINT message, LPARAM lParam)
{
	CWnd* pChild = CWnd::FromHandlePermanent((HWND)LOWORD(lParam));
	if (pChild != NULL &&
		(message == WM_CREATE || message == WM_DESTROY) &&
		pChild->SendChildNotifyLastMsg())
	{
		return;     // eat it
	}
	// not handled - do default
	Default();
}


void CWnd::OnHScroll(UINT, UINT, CScrollBar* pScrollBar)
{
	if (pScrollBar != NULL && pScrollBar->SendChildNotifyLastMsg())
		return;     // eat it
	Default();
}

void CWnd::OnVScroll(UINT, UINT, CScrollBar* pScrollBar)
{
	if (pScrollBar != NULL && pScrollBar->SendChildNotifyLastMsg())
		return;     // eat it
	Default();
}

HBRUSH CWnd::OnCtlColor(CDC*, CWnd* pWnd, UINT)
{
	ASSERT(pWnd != NULL && pWnd->m_hWnd != NULL);
	LRESULT lResult;
	if (pWnd->SendChildNotifyLastMsg(&lResult))
		return (HBRUSH)lResult;     // eat it
	return (HBRUSH)Default();
}

/////////////////////////////////////////////////////////////////////////////
// 'dialog data' support

BOOL CWnd::UpdateData(BOOL bSaveAndValidate)
{
	CDataExchange dx(this, bSaveAndValidate);

	// prevent control notifications from being dispatched during UpdateData
	HWND hWndOldLockout = _afxLockoutNotificationWindow;
	ASSERT(hWndOldLockout != m_hWnd);   // must not recurse
	_afxLockoutNotificationWindow = m_hWnd;

	BOOL bOK = FALSE;       // assume failure
	TRY
	{
		DoDataExchange(&dx);
		bOK = TRUE;         // it worked
	}
	CATCH(CUserException, e)
	{
		// validation failed - user already alerted, fall through
		ASSERT(bOK == FALSE);
	}
	AND_CATCH_ALL(e)
	{
		// validation failed due to OOM or other resource failure
		AfxMessageBox(AFX_IDP_INTERNAL_FAILURE, MB_ICONSTOP);
		ASSERT(bOK == FALSE);
	}
	END_CATCH_ALL

	_afxLockoutNotificationWindow = hWndOldLockout;
	return bOK;
}

CDataExchange::CDataExchange(CWnd* pDlgWnd, BOOL bSaveAndValidate)
{
	ASSERT_VALID(pDlgWnd);
	m_bSaveAndValidate = bSaveAndValidate;
	m_pDlgWnd = pDlgWnd;
	m_hWndLastControl = NULL;
}

/////////////////////////////////////////////////////////////////////////////
// Dialog initialization support

BOOL CWnd::ExecuteDlgInit(LPCSTR lpszResourceName)
{
	BOOL bSuccess = TRUE;
	HRSRC hDlgInit;
	if (lpszResourceName != NULL)
	{
		HINSTANCE hInst = AfxFindResourceHandle(lpszResourceName, RT_DLGINIT);
		if ((hDlgInit = ::FindResource(hInst, lpszResourceName,
			RT_DLGINIT)) != NULL)
		{
			HGLOBAL hRes = ::LoadResource(hInst, hDlgInit);
			if (hRes != NULL)
			{
				UINT FAR* lpnRes = (UINT FAR*)::LockResource(hRes);
				while (bSuccess && *lpnRes != 0)
				{
					UINT nIDC = *lpnRes++;
					UINT nMsg = *lpnRes++;
					DWORD dwLen = *((DWORD FAR*&)lpnRes)++;
					ASSERT(dwLen < 0xfffe);
					ASSERT(((LONG)LOWORD(lpnRes) + (UINT)dwLen) < 0xfffe);

#ifdef _DEBUG
					// For AddStrings, the count must exactly delimit the
					// string, including the NULL termination.  This check
					// will not catch all mal-formed ADDSTRINGs, but will
					// catch some.
					if (nMsg == LB_ADDSTRING || nMsg == CB_ADDSTRING)
						ASSERT(*((LPCSTR)lpnRes + (UINT)dwLen - 1) == 0);

#endif

					// List/Combobox returns -1 for error, as does
					// the VBX Form File initialization

					if (SendDlgItemMessage(nIDC, nMsg, 0, (LONG)lpnRes) == -1)
						bSuccess = FALSE;

					lpnRes = (UINT FAR*)((LPCSTR)lpnRes + (UINT)dwLen);
							// skip past data
				}
				::UnlockResource(hRes);
				::FreeResource(hRes);
			}
		}
	}

	// Send update message to all controls after all other siblings loaded
	if (bSuccess)
		SendMessageToDescendants(WM_INITIALUPDATE, 0, 0, FALSE, FALSE);

	return bSuccess;
}

void CWnd::UpdateDialogControls(CCmdTarget* pTarget, BOOL bDisableIfNoHndler)
{
	CCmdUI state;
	CWnd wndTemp;       // very temporary window just for CmdUI update

	// walk all the kids - assume the IDs are for buttons
	for (HWND hWndChild = ::GetTopWindow(m_hWnd); hWndChild != NULL;
			hWndChild = ::GetNextWindow(hWndChild, GW_HWNDNEXT))
	{
		// send to buttons
		wndTemp.m_hWnd = hWndChild; // quick and dirty attach
		state.m_nID = _AfxGetDlgCtrlID(hWndChild);
		state.m_pOther = &wndTemp;

		// determine whether to disable when no handler exists
		BOOL bDisableTemp = bDisableIfNoHndler;
		if (bDisableTemp)
		{
			if ((wndTemp.SendMessage(WM_GETDLGCODE) & DLGC_BUTTON) == 0)
			{
				// non-button controls don't get automagically disabled
				bDisableTemp = FALSE;
			}
			else
			{
				// only certain button controls get automagically disabled
				UINT nStyle = (UINT)(wndTemp.GetStyle() & 0x0F);
				if (nStyle == (UINT)BS_AUTOCHECKBOX ||
					nStyle == (UINT)BS_AUTO3STATE ||
					nStyle == (UINT)BS_GROUPBOX ||
					nStyle == (UINT)BS_AUTORADIOBUTTON)
				{
					bDisableTemp = FALSE;
				}
			}
		}
		state.DoUpdate(pTarget, bDisableTemp);
	}
	wndTemp.m_hWnd = NULL;      // quick and dirty detach
}

/////////////////////////////////////////////////////////////////////////////
// CFrameWnd (here for library granularity)

BOOL CWnd::IsFrameWnd() const
{
	return FALSE;
}

BOOL CFrameWnd::IsFrameWnd() const
{
	return TRUE;
}

BOOL CFrameWnd::IsTracking() const
{
	return m_nIDTracking != 0 &&
		m_nIDTracking != AFX_IDS_HELPMODEMESSAGE &&
		m_nIDTracking != AFX_IDS_IDLEMESSAGE;
}

/////////////////////////////////////////////////////////////////////////////
