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

#ifdef AFX_DBG1_SEG
#pragma code_seg(AFX_DBG1_SEG)
#endif

#ifdef _DEBUG       // entire file for debugging

#include "dde.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// Build data tables by including data file three times

#define DO(WM_FOO)  static char BASED_CODE sz##WM_FOO[] = #WM_FOO;
#include "tracedat.h"
#undef DO

static UINT BASED_CODE allMessages[] =
{
#define DO(WM_FOO)  WM_FOO,
#include "tracedat.h"
#undef DO
	0   // end of table
};

static LPCSTR BASED_CODE allMessageNames[] =
{
#define DO(WM_FOO)  sz##WM_FOO,
#include "tracedat.h"
#undef DO
	NULL    // end of table
};

/////////////////////////////////////////////////////////////////////////////
// DDE special case

static void PASCAL TraceDDE(LPCSTR lpszPrefix, const MSG* pMsg)
{
	if (pMsg->message == WM_DDE_EXECUTE)
	{
		HGLOBAL hCommands = (HGLOBAL)HIWORD(pMsg->lParam);
		ASSERT(hCommands != NULL);

		LPCSTR lpszCommands = (LPCSTR)::GlobalLock(hCommands);
		ASSERT(lpszCommands != NULL);
		TRACE2("%Fs: Execute '%Fs'\n", lpszPrefix, lpszCommands);
		::GlobalUnlock(hCommands);
	}
	else if (pMsg->message == WM_DDE_ADVISE)
	{
		ATOM aItem = HIWORD(pMsg->lParam);
		HGLOBAL hAdvise = (HGLOBAL)LOWORD(pMsg->lParam);
		ASSERT(aItem != NULL);
		ASSERT(hAdvise != NULL);

		DDEADVISE FAR* lpAdvise = (DDEADVISE FAR*)::GlobalLock(hAdvise);
		ASSERT(lpAdvise != NULL);
		char szItem[80];
		szItem[0] = '\0';

		if (aItem != 0)
			::GlobalGetAtomName(aItem, szItem, sizeof(szItem));

		char szFormat[80];
		szFormat[0] = '\0';
		if (((short)0xC000 <= lpAdvise->cfFormat) &&
				(lpAdvise->cfFormat <= (short)0xFFFF))
			::GetClipboardFormatName(lpAdvise->cfFormat,
				szFormat, sizeof(szFormat));

			// User defined clipboard formats have a range of 0xC000->0xFFFF
			// System clipboard formats have other ranges, but no printable
			// format names.

		static char BASED_DEBUG _sz[] =
			"%Fs: Advise item='%s', Format='%s', Ack=%d, Defer Update= %d\n";
		AfxTrace(_sz, lpszPrefix, szItem, szFormat, lpAdvise->fAckReq,
			lpAdvise->fDeferUpd);
		::GlobalUnlock(hAdvise);
	}
}

/////////////////////////////////////////////////////////////////////////////

void AFXAPI _AfxTraceMsg(LPCSTR lpszPrefix, const MSG* pMsg)
{
	ASSERT(lpszPrefix != NULL);
	ASSERT(pMsg != NULL);

	if (pMsg->message == WM_MOUSEMOVE || pMsg->message == WM_NCMOUSEMOVE ||
	   pMsg->message == WM_NCHITTEST ||
	   pMsg->message == WM_SETCURSOR ||
	   pMsg->message == WM_CTLCOLOR ||
	   pMsg->message == WM_ENTERIDLE ||
	   pMsg->message == WM_CANCELMODE)
	{
		// don't report very frequently sent messages
		return;
	}

	LPCSTR lpszMsgName = NULL;
	char szBuf[80];

	// find message name
	if (pMsg->message >= 0xC000)
	{
		// Window message registered with 'RegisterWindowMessage'
		//  (actually a USER atom)
		if (::GetClipboardFormatName(pMsg->message, szBuf, sizeof(szBuf)) != 0)
			lpszMsgName = szBuf;
	}
	else if (pMsg->message >= WM_USER)
	{
		// User message
		sprintf(szBuf, "WM_USER+0x%04X", pMsg->message - WM_USER);
		lpszMsgName = szBuf;
	}
	else
	{
		// a system windows message
		const UINT FAR* lpMessage;
		for (lpMessage = allMessages; *lpMessage != 0; lpMessage++)
		{
			if (*lpMessage == pMsg->message)
			{
				int iMsg = (int)(lpMessage - (const UINT FAR*)allMessages);
				lpszMsgName = allMessageNames[iMsg];
				break;
			}
		}
	}

	if (lpszMsgName != NULL)
	{
		static char BASED_DEBUG _sz[] =
			"%Fs: hwnd=0x%04X, msg = %Fs (0x%04X, 0x%08lX)\n";
		AfxTrace(_sz, lpszPrefix, (UINT)pMsg->hwnd, lpszMsgName,
			pMsg->wParam, pMsg->lParam);
	}
	else
	{
		static char BASED_DEBUG _sz[] =
			"%Fs: hwnd=0x%04X, msg = 0x%04X (0x%04X, 0x%08lX)\n";
		AfxTrace(_sz, lpszPrefix, (UINT)pMsg->hwnd, pMsg->message,
			pMsg->wParam, pMsg->lParam);
	}

	if (pMsg->message >= WM_DDE_FIRST && pMsg->message <= WM_DDE_LAST)
	{
		TraceDDE(lpszPrefix, pMsg);
	}
}

/////////////////////////////////////////////////////////////////////////////

#endif // _DEBUG (entire file)
