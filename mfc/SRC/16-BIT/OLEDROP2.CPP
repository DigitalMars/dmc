// This is a part of the Microsoft Foundation Classes C++ library.
// Copyright (C) 1993 Microsoft Corporation
// All rights reserved.
//
// This source code is only intended as a supplement to the
// Microsoft Foundation Classes Reference and Microsoft
// QuickHelp and/or WinHelp documentation provided with the library.
// See these sources for detailed information regarding the
// Microsoft Foundation Classes product.


#include "stdafx.h"

#ifdef AFX_OLE4_SEG
#pragma code_seg(AFX_OLE4_SEG)
#endif

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

#define new DEBUG_NEW

/////////////////////////////////////////////////////////////////////////////
// COleDropTarget implementation

int COleDropTarget::nScrollInset;
UINT COleDropTarget::nScrollDelay;
UINT COleDropTarget::nScrollInterval;

COleDropTarget::COleDropTarget()
{
	// initialize local state
	m_hWnd = NULL;
	m_lpDataObject = NULL;
	m_nTimerID = MAKEWORD(-1, -1);

	static BOOL bInitialized;
	if (!bInitialized)
	{
		// get scroll metrics from win.ini
		static char BASED_CODE szWindows[] = "windows";
		static char BASED_CODE szScrollDelay[] = "DragScrollDelay";
		static char BASED_CODE szScrollInset[] = "DragScrollInset";
		static char BASED_CODE szScrollInterval[] = "DragScrollInterval";

		nScrollInset = GetProfileInt(szWindows, szScrollInset, DD_DEFSCROLLINSET);
		nScrollDelay = GetProfileInt(szWindows, szScrollDelay, DD_DEFSCROLLDELAY);
		nScrollInterval = GetProfileInt(szWindows, szScrollInterval,
			DD_DEFSCROLLINTERVAL);

		// now initialized, no need to call Initialize any more
		bInitialized = TRUE;
	}

	ASSERT_VALID(this);
}

COleDropTarget::~COleDropTarget()
{
	ASSERT_VALID(this);

	if (m_hWnd != NULL)
	{
		TRACE0("Warning: COleDropTarget::Revoke not called before destructor "
			"called.\n\t -- May cause RIPs under debug Windows.\n");
		Revoke();
	}
}

BOOL COleDropTarget::Register(CWnd* pWnd)
{
	ASSERT_VALID(this);
	ASSERT(m_hWnd == NULL);     // registering drop target twice?
	ASSERT_VALID(pWnd);

	LPUNKNOWN lpUnknown = (LPUNKNOWN)GetInterface(&IID_IUnknown);
	ASSERT(lpUnknown != NULL);

	// the object must be locked externally to keep LRPC connections alive
	if (CoLockObjectExternal(lpUnknown, TRUE, FALSE) != NOERROR)
		return FALSE;

	// connect the HWND to the IDropTarget implementation
	if (RegisterDragDrop(pWnd->m_hWnd,
		(LPDROPTARGET)GetInterface(&IID_IDropTarget)) != NOERROR)
	{
		CoLockObjectExternal(lpUnknown, FALSE, FALSE);
		return FALSE;
	}

	// connect internal data
	m_hWnd = pWnd->m_hWnd;
	ASSERT(pWnd->m_pDropTarget == NULL);
	pWnd->m_pDropTarget = this;

	return TRUE;
}

void COleDropTarget::Revoke()
{
	ASSERT_VALID(this);
	ASSERT(m_lpDataObject == NULL);

	if (m_hWnd == NULL)
	{
		ASSERT(m_nTimerID == MAKEWORD(-1, -1));
		return;
	}

	// disconnect from OLE
	RevokeDragDrop(m_hWnd);
	CoLockObjectExternal((LPUNKNOWN)GetInterface(&IID_IUnknown), FALSE, TRUE);

	// disconnect internal data
	CWnd::FromHandle(m_hWnd)->m_pDropTarget = NULL;
	m_hWnd = NULL;
}

/////////////////////////////////////////////////////////////////////////////
// default implementation of drag/drop scrolling

BOOL COleDropTarget::OnDragScroll(CWnd* pWnd, DWORD dwKeyState, CPoint point)
{
	ASSERT_VALID(this);
	ASSERT_VALID(pWnd);

	// CWnds are allowed, but don't support autoscrolling
	if (!pWnd->IsKindOf(RUNTIME_CLASS(CView)))
		return FALSE;
	CView* pView = (CView*)pWnd;

	// get client rectangle of destination window
	CRect rectClient;
	pWnd->GetClientRect(&rectClient);
	CRect rect = rectClient;

	// hit-test against inset region
	UINT nTimerID = MAKEWORD(-1, -1);
	rect.InflateRect(-nScrollInset, -nScrollInset);
	CSplitterWnd* pSplitter;
	if (rectClient.PtInRect(point) && !rect.PtInRect(point))
	{
		// determine which way to scroll along both X & Y axis
		if (point.x < rect.left)
			nTimerID = MAKEWORD(SB_LINEUP, HIBYTE(nTimerID));
		else if (point.x >= rect.right)
			nTimerID = MAKEWORD(SB_LINEDOWN, HIBYTE(nTimerID));
		if (point.y < rect.top)
			nTimerID = MAKEWORD(LOBYTE(nTimerID), SB_LINEUP);
		else if (point.y >= rect.bottom)
			nTimerID = MAKEWORD(LOBYTE(nTimerID), SB_LINEDOWN);
		ASSERT(nTimerID != MAKEWORD(-1, -1));

		// check for valid scroll first
		pSplitter = CView::GetParentSplitter(pView, FALSE);
		BOOL bEnableScroll = FALSE;
		if (pSplitter != NULL)
			bEnableScroll = pSplitter->DoScroll(pView, nTimerID, FALSE);
		else
			bEnableScroll = pView->OnScroll(nTimerID, 0, FALSE);
		if (!bEnableScroll)
			nTimerID = MAKEWORD(-1, -1);
	}

	if (nTimerID == MAKEWORD(-1, -1))
	{
		if (m_nTimerID != MAKEWORD(-1, -1))
		{
			// send fake OnDragEnter when transition from scroll->normal
			COleDataObject dataObject;
			dataObject.Attach(m_lpDataObject, FALSE);
			OnDragEnter(pWnd, &dataObject, dwKeyState, point);
			m_nTimerID = MAKEWORD(-1, -1);
		}
		return FALSE;
	}

	// save tick count when timer ID changes
	DWORD dwTick = GetTickCount();
	if (nTimerID != m_nTimerID)
	{
		m_dwLastTick = dwTick;
		m_nScrollDelay = nScrollDelay;
	}

	// scroll if necessary
	if (dwTick - m_dwLastTick > m_nScrollDelay)
	{
		if (pSplitter != NULL)
			pSplitter->DoScroll(pView, nTimerID, TRUE);
		else
			pView->OnScroll(nTimerID, 0, TRUE);
		m_dwLastTick = dwTick;
		m_nScrollDelay = nScrollInterval;
	}
	if (m_nTimerID == MAKEWORD(-1, -1))
	{
		// send fake OnDragLeave when transitioning from normal->scroll
		OnDragLeave(pWnd);
	}

	m_nTimerID = nTimerID;
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// COleDropTarget drop/ drop query handling

DROPEFFECT COleDropTarget::OnDragEnter(CWnd* pWnd, COleDataObject* pDataObject,
	DWORD dwKeyState, CPoint point)
{
	ASSERT_VALID(this);

	if (!pWnd->IsKindOf(RUNTIME_CLASS(CView)))
		return DROPEFFECT_NONE;

	// default delagates to view
	CView* pView = (CView*)pWnd;
	ASSERT_VALID(pView);
	return pView->OnDragEnter(pDataObject, dwKeyState, point);
}

DROPEFFECT COleDropTarget::OnDragOver(CWnd* pWnd, COleDataObject* pDataObject,
	DWORD dwKeyState, CPoint point)
{
	ASSERT_VALID(this);

	if (!pWnd->IsKindOf(RUNTIME_CLASS(CView)))
		return DROPEFFECT_NONE;

	// default delagates to view
	CView* pView = (CView*)pWnd;
	ASSERT_VALID(pView);
	return pView->OnDragOver(pDataObject, dwKeyState, point);
}

BOOL COleDropTarget::OnDrop(CWnd* pWnd, COleDataObject* pDataObject,
	DROPEFFECT dropEffect, CPoint point)
{
	ASSERT_VALID(this);

	if (!pWnd->IsKindOf(RUNTIME_CLASS(CView)))
		return DROPEFFECT_NONE;

	// default delagates to view
	CView* pView = (CView*)pWnd;
	ASSERT_VALID(pView);
	return pView->OnDrop(pDataObject, dropEffect, point);
}

void COleDropTarget::OnDragLeave(CWnd* pWnd)
{
	ASSERT_VALID(this);

	if (!pWnd->IsKindOf(RUNTIME_CLASS(CView)))
		return;

	// default delagates to view
	CView* pView = (CView*)pWnd;
	ASSERT_VALID(pView);
	pView->OnDragLeave();
	return;
}

/////////////////////////////////////////////////////////////////////////////
// COleDropTarget::COleDropTarget implementation

BEGIN_INTERFACE_MAP(COleDropTarget, CCmdTarget)
	INTERFACE_PART(COleDropTarget, IID_IDropTarget, DropTarget)
END_INTERFACE_MAP()

STDMETHODIMP_(ULONG) COleDropTarget::XDropTarget::AddRef()
{
	METHOD_PROLOGUE(COleDropTarget, DropTarget)
	return (ULONG)pThis->ExternalAddRef();
}

STDMETHODIMP_(ULONG) COleDropTarget::XDropTarget::Release()
{
	METHOD_PROLOGUE(COleDropTarget, DropTarget)
	return (ULONG)pThis->ExternalRelease();
}

STDMETHODIMP COleDropTarget::XDropTarget::QueryInterface(
	REFIID iid, LPVOID far* ppvObj)
{
	METHOD_PROLOGUE(COleDropTarget, DropTarget)
	return (HRESULT)pThis->ExternalQueryInterface(&iid, ppvObj);
}

// helper to filter out invalid DROPEFFECTs
static DROPEFFECT NEAR PASCAL
FilterDropEffect(DROPEFFECT dropEffect, DROPEFFECT dwEffects)
{
	// return allowed dropEffect and DROPEFFECT_NONE
	if ((dropEffect & dwEffects) != 0)
		return dropEffect;

	// map common operations (copy/move) to alternates, but give negative
	//  feedback for DROPEFFECT_LINK.
	switch (dropEffect)
	{
	case DROPEFFECT_COPY:
		if (dwEffects & DROPEFFECT_MOVE)
			return DROPEFFECT_MOVE;
		else if (dwEffects & DROPEFFECT_LINK)
			return DROPEFFECT_LINK;
		break;
	case DROPEFFECT_MOVE:
		if (dwEffects & DROPEFFECT_COPY)
			return DROPEFFECT_COPY;
		else if (dwEffects & DROPEFFECT_LINK)
			return DROPEFFECT_LINK;
		break;
	case DROPEFFECT_LINK:
		break;
	}

	return DROPEFFECT_NONE;
}

STDMETHODIMP COleDropTarget::XDropTarget::DragEnter(THIS_ LPDATAOBJECT lpDataObject,
	DWORD dwKeyState, POINTL pt, LPDWORD pdwEffect)
{
	METHOD_PROLOGUE(COleDropTarget, DropTarget)
	ASSERT_VALID(pThis);

	ASSERT(pdwEffect != NULL);
	ASSERT(lpDataObject != NULL);

	SCODE sc = E_UNEXPECTED;
	TRY
	{
		// cache lpDataObject
		lpDataObject->AddRef();
		_AfxRelease((LPUNKNOWN*)&pThis->m_lpDataObject);
		pThis->m_lpDataObject = lpDataObject;

		CWnd* pWnd = CWnd::FromHandle(pThis->m_hWnd);
		ASSERT_VALID(pWnd);
		CPoint point((int)pt.x, (int)pt.y);
		pWnd->ScreenToClient(&point);

		// check first for entering scroll area
		DROPEFFECT dropEffect = DROPEFFECT_SCROLL;
		if (!pThis->OnDragScroll(pWnd, dwKeyState, point))
		{
			// funnel through OnDragEnter
			COleDataObject dataObject;
			dataObject.Attach(lpDataObject, FALSE);
			dropEffect = FilterDropEffect(pThis->OnDragEnter(pWnd, &dataObject,
				dwKeyState, point), *pdwEffect);
		}
		*pdwEffect = dropEffect;
		sc = S_OK;
	}
	END_TRY

	return ResultFromScode(sc);
}

STDMETHODIMP COleDropTarget::XDropTarget::DragOver(THIS_ DWORD dwKeyState,
	POINTL pt, LPDWORD pdwEffect)
{
	METHOD_PROLOGUE(COleDropTarget, DropTarget)
	ASSERT_VALID(pThis);

	ASSERT(pdwEffect != NULL);
	ASSERT(pThis->m_lpDataObject != NULL);

	SCODE sc = E_UNEXPECTED;
	TRY
	{
		CWnd* pWnd = CWnd::FromHandle(pThis->m_hWnd);
		ASSERT_VALID(pWnd);
		CPoint point((int)pt.x, (int)pt.y);
		pWnd->ScreenToClient(&point);

		// check first for entering scroll area
		DROPEFFECT dropEffect = DROPEFFECT_SCROLL;
		if (!pThis->OnDragScroll(pWnd, dwKeyState, point))
		{
			// funnel through OnDragOver
			COleDataObject dataObject;
			dataObject.Attach(pThis->m_lpDataObject, FALSE);
			dropEffect = FilterDropEffect(pThis->OnDragOver(pWnd, &dataObject,
				dwKeyState, point), *pdwEffect);
		}
		*pdwEffect = dropEffect;
		sc = S_OK;
	}
	END_TRY

	return ResultFromScode(sc);
}

STDMETHODIMP COleDropTarget::XDropTarget::DragLeave(THIS)
{
	METHOD_PROLOGUE(COleDropTarget, DropTarget)
	ASSERT_VALID(pThis);

	CWnd* pWnd = CWnd::FromHandle(pThis->m_hWnd);
	ASSERT_VALID(pWnd);

	// cancel drag scrolling
	pThis->m_nTimerID = MAKEWORD(-1, -1);

	// allow derivative to do own cleanup
	COleDataObject dataObject;
	dataObject.Attach(pThis->m_lpDataObject, FALSE);
	pThis->OnDragLeave(pWnd);

	// release cached data object
	_AfxRelease((LPUNKNOWN*)&pThis->m_lpDataObject);

	return NOERROR;
}

STDMETHODIMP COleDropTarget::XDropTarget::Drop(THIS_ LPDATAOBJECT lpDataObject,
	DWORD dwKeyState, POINTL pt, LPDWORD pdwEffect)
{
	METHOD_PROLOGUE(COleDropTarget, DropTarget)
	ASSERT_VALID(pThis);

	ASSERT(pdwEffect != NULL);
	ASSERT(lpDataObject != NULL);

	SCODE sc = E_UNEXPECTED;
	TRY
	{
		// cancel drag scrolling
		pThis->m_nTimerID = MAKEWORD(-1, -1);

		// prepare for call to OnDragOver
		CWnd* pWnd = CWnd::FromHandle(pThis->m_hWnd);
		ASSERT_VALID(pWnd);
		COleDataObject dataObject;
		dataObject.Attach(lpDataObject, FALSE);
		CPoint point((int)pt.x, (int)pt.y);
		pWnd->ScreenToClient(&point);

		// verify that drop is legal
		DROPEFFECT dropEffect = FilterDropEffect(pThis->OnDragOver(pWnd,
			&dataObject, dwKeyState, point), *pdwEffect);
		if (dropEffect != DROPEFFECT_NONE)
		{
			// execute the drop
			if (!pThis->OnDrop(pWnd, &dataObject, dropEffect, point))
				dropEffect = DROPEFFECT_NONE;
		}
		else
		{
			// drop not accepted, allow cleanup
			pThis->OnDragLeave(pWnd);
		}

		// release potentially cached data object
		_AfxRelease((LPUNKNOWN*)&pThis->m_lpDataObject);
		*pdwEffect = dropEffect;
		sc = S_OK;
	}
	END_TRY

	return ResultFromScode(sc);
}

/////////////////////////////////////////////////////////////////////////////
// COleDropTarget diagnostics

#ifdef _DEBUG
void COleDropTarget::AssertValid() const
{
	CCmdTarget::AssertValid();
	if (m_lpDataObject != NULL)
		CWnd::FromHandle(m_hWnd)->AssertValid();
}

void COleDropTarget::Dump(CDumpContext& dc) const
{
	CCmdTarget::Dump(dc);
	AFX_DUMP1(dc, "\nm_hWnd = ", m_hWnd);
	AFX_DUMP1(dc, "\nm_lpDataObject = ", m_lpDataObject);
	AFX_DUMP1(dc, "\nm_nTimerID = ", m_nTimerID);
	AFX_DUMP1(dc, "\nm_dwLastTick = ", m_dwLastTick);
}
#endif

/////////////////////////////////////////////////////////////////////////////
