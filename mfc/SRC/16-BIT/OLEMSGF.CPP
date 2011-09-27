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
// COleMessageFilter::IMessageFilter implementation

COleMessageFilter::COleMessageFilter()
{
	// begin in not-busy state
	m_nBusyCount = 0;

	// dialogs are enabled by default
	m_bEnableBusy = TRUE;
	m_bEnableNotResponding = TRUE;

	m_nBusyReply = SERVERCALL_RETRYLATER;
		// effective only when m_nBusyCount != 0

	m_nRetryReply = OLESTDRETRYDELAY;   // default is 5000 (5 sec)
	m_nTimeout = OLESTDRETRYDELAY;      // default is 5000 (5 sec)

	m_bUnblocking = FALSE;
		// TRUE to avoid re-entrancy when busy dialog is up

	m_bRegistered = FALSE;

	ASSERT_VALID(this);
}

COleMessageFilter::~COleMessageFilter()
{
	ASSERT_VALID(this);

	Revoke();
}

/////////////////////////////////////////////////////////////////////////////
// Busy state management

void COleMessageFilter::BeginBusyState()
{
	ASSERT_VALID(this);
	++m_nBusyCount;
}

void COleMessageFilter::EndBusyState()
{
	ASSERT_VALID(this);
	if (m_nBusyCount != 0)
		--m_nBusyCount;
}

/////////////////////////////////////////////////////////////////////////////
// COleMessageFilter operations

BOOL COleMessageFilter::Register()
{
	ASSERT_VALID(this);
	ASSERT(!m_bRegistered); // calling Register twice?

	if (::CoRegisterMessageFilter(&m_xMessageFilter, NULL) == NOERROR)
	{
		m_bRegistered = TRUE;
		return TRUE;
	}
	return FALSE;
}

void COleMessageFilter::Revoke()
{
	ASSERT_VALID(this);

	if (m_bRegistered)
	{
		::CoRegisterMessageFilter(NULL, NULL);
		m_bRegistered = FALSE;
	}
}

/////////////////////////////////////////////////////////////////////////////
// COleMessageFilter standard implementation of callbacks

BOOL COleMessageFilter::OnMessagePending(const MSG* pMsg)
{
	// By default we rely on OLE's default message handling for every message
	//  except WM_PAINT messages.  WM_PAINT messages should not generate
	//  out-going calls.

	BOOL bEatMessage = FALSE;

	switch (pMsg->message)
	{
	case WM_PAINT:
		DispatchMessage(pMsg);
		bEatMessage = TRUE;
		break;
	}

	if (bEatMessage)
	{
		MSG msg;
		::PeekMessage(&msg, NULL, pMsg->message, pMsg->message,
			PM_REMOVE|PM_NOYIELD);
	}
	return bEatMessage;
}

BOOL COleMessageFilter::IsSignificantMessage(MSG* pMsg)
{
	// check for "significant" messages in the queue
	UINT rgnMsgs[] =
	{
		WM_LBUTTONDOWN, WM_RBUTTONDOWN, WM_MBUTTONDOWN, WM_KEYDOWN,
		WM_NCLBUTTONDOWN, WM_NCRBUTTONDOWN, WM_NCMBUTTONDOWN,
		WM_LBUTTONDBLCLK, WM_RBUTTONDBLCLK, WM_MBUTTONDBLCLK, WM_KEYDOWN,
		WM_NCLBUTTONDBLCLK, WM_NCRBUTTONDBLCLK, WM_NCMBUTTONDBLCLK
	};
	for (int i = 0; i < sizeof(rgnMsgs)/sizeof(UINT); i++)
	{
		if (::PeekMessage(pMsg, NULL, rgnMsgs[i], rgnMsgs[i],
			PM_NOREMOVE|PM_NOYIELD))
		{
			// "significant" message is waiting in the queue
			return TRUE;
		}
	}

	// no significant messages in the queue
	return FALSE;
}

int COleMessageFilter::OnBusyDialog(HTASK htaskBusy)
{
	COleBusyDialog dlg(htaskBusy, FALSE);

	int nResult = -1;
	TRY
	{
		if (dlg.DoModal() == IDOK)
			nResult = dlg.GetSelectionType();
	}
	END_TRY

	return nResult;
}

int COleMessageFilter::OnNotRespondingDialog(HTASK htaskBusy)
{
	COleBusyDialog dlg(htaskBusy, TRUE);

	int nResult = -1;
	TRY
	{
		if (dlg.DoModal() == IDOK)
			nResult = dlg.GetSelectionType();
	}
	END_TRY

	return nResult;
}

/////////////////////////////////////////////////////////////////////////////
// COleMessageFilter OLE interface implementation

BEGIN_INTERFACE_MAP(COleMessageFilter, CCmdTarget)
	INTERFACE_PART(COleMessageFilter, IID_IMessageFilter, MessageFilter)
END_INTERFACE_MAP()

STDMETHODIMP_(ULONG) COleMessageFilter::XMessageFilter::AddRef()
{
	METHOD_PROLOGUE(COleMessageFilter, MessageFilter)
	return (ULONG)pThis->ExternalAddRef();
}

STDMETHODIMP_(ULONG) COleMessageFilter::XMessageFilter::Release()
{
	METHOD_PROLOGUE(COleMessageFilter, MessageFilter)
	return (ULONG)pThis->ExternalRelease();
}

STDMETHODIMP COleMessageFilter::XMessageFilter::QueryInterface(
	REFIID iid, LPVOID far* ppvObj)
{
	METHOD_PROLOGUE(COleMessageFilter, MessageFilter)
	return (HRESULT)pThis->ExternalQueryInterface(&iid, ppvObj);
}

STDMETHODIMP_(DWORD) COleMessageFilter::XMessageFilter::HandleInComingCall(
	DWORD dwCallType, HTASK htaskCaller, DWORD dwTickCount, DWORD dwReserved )
{
	METHOD_PROLOGUE(COleMessageFilter, MessageFilter)
	ASSERT_VALID(pThis);

	// check for application busy first...
	if (pThis->m_nBusyCount == 0)
		return SERVERCALL_ISHANDLED;

	if (dwCallType == CALLTYPE_TOPLEVEL ||
		dwCallType == CALLTYPE_TOPLEVEL_CALLPENDING)
	{
		// application is busy and we have rejectable CALLTYPE
		return pThis->m_nBusyReply;
	}

	// application is busy, but CALLTYPE indicates that it *must* be handled
	return SERVERCALL_ISHANDLED;
}

STDMETHODIMP_(DWORD) COleMessageFilter::XMessageFilter::RetryRejectedCall(
	HTASK htaskCallee, DWORD dwTickCount, DWORD dwRejectType)
{
	METHOD_PROLOGUE(COleMessageFilter, MessageFilter)
	ASSERT_VALID(pThis);

	// if insignificant time has passed, don't panic -- just retry
	if (dwTickCount <= pThis->m_nTimeout)
		return 0;   // retry right away (0-100 are retry immediate)

	// too much time has passed, do something more drastic
	if (dwRejectType == SERVERCALL_RETRYLATER && pThis->m_bEnableBusy)
	{
		// show busy dialog
		int selType = pThis->OnBusyDialog(htaskCallee);

		// take action depending on selection
		switch (selType)
		{
		case -1:
			return OLESTDCANCELRETRY;   // cancel outgoing call

		case COleBusyDialog::retry:
			return 0;                   // retry immediately
		}
	}

	// start waiting longer each time
	return pThis->m_nRetryReply;    // use standard retry timeout
}

STDMETHODIMP_(DWORD) COleMessageFilter::XMessageFilter::MessagePending(
	HTASK htaskCallee, DWORD dwTickCount, DWORD dwPendingType)
{
	METHOD_PROLOGUE(COleMessageFilter, MessageFilter)
	ASSERT_VALID(pThis);

	MSG msg;
	if (dwTickCount > pThis->m_nTimeout && !pThis->m_bUnblocking &&
		pThis->IsSignificantMessage(&msg))
	{
		if (pThis->m_bEnableNotResponding)
		{
			pThis->m_bUnblocking = TRUE;    // avoid reentrant calls

			// eat all mouse messages in our queue
			while (PeekMessage(&msg, NULL, WM_MOUSEFIRST, WM_MOUSELAST,
				PM_REMOVE|PM_NOYIELD))
				;
			// eat all keyboard messages in our queue
			while (PeekMessage(&msg, NULL, WM_KEYFIRST, WM_KEYLAST,
				PM_REMOVE|PM_NOYIELD))
				;

			// show not responding dialog
			pThis->OnNotRespondingDialog(htaskCallee);
			pThis->m_bUnblocking = FALSE;

			return PENDINGMSG_WAITNOPROCESS;
		}
	}

	// don't process re-entrant messages
	if (pThis->m_bUnblocking)
		return PENDINGMSG_WAITDEFPROCESS;

	// allow application to process pending message
	if (::PeekMessage(&msg, NULL, NULL, NULL, PM_NOREMOVE|PM_NOYIELD) &&
		pThis->OnMessagePending(&msg))
	{
		// app has processed the message (and removed it from the queue)
		return PENDINGMSG_WAITNOPROCESS;
	}

	// by default we return pending MSG wait
	return PENDINGMSG_WAITNOPROCESS;
}

/////////////////////////////////////////////////////////////////////////////
// COleMessageFilter diagnostics

#ifdef _DEBUG
void COleMessageFilter::AssertValid() const
{
	CCmdTarget::AssertValid();
}

void COleMessageFilter::Dump(CDumpContext& dc) const
{
	CCmdTarget::Dump(dc);
	AFX_DUMP1(dc, "\nm_bRegistered = ", m_bRegistered);
	AFX_DUMP1(dc, "\nm_nBusyCount = ", m_nBusyCount);
	AFX_DUMP1(dc, "\nm_bEnableBusy = ", m_bEnableBusy);
	AFX_DUMP1(dc, "\nm_bEnableNotResponding = ", m_bEnableNotResponding);
	AFX_DUMP1(dc, "\nm_bUnblocking = ", m_bUnblocking);
	AFX_DUMP1(dc, "\nm_nRetryReply = ", m_nRetryReply);
	AFX_DUMP1(dc, "\nm_nBusyReply = ", m_nBusyReply);
	AFX_DUMP1(dc, "\nm_nTimeout = ", m_nTimeout);
}
#endif

/////////////////////////////////////////////////////////////////////////////
