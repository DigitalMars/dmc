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
#include "penwin.h"     // MFC Apps are PenAware by default

#ifdef AFX_INIT_SEG
#pragma code_seg(AFX_INIT_SEG)
#endif

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// other globals (internal library use)

#if !defined(_USRDLL) && !defined(_AFXCTL)
void (CALLBACK* _afxRegisterPenAppProc)(UINT, BOOL);
#endif //!_USRDLL

#if !defined(_USRDLL) && !defined(_AFXDLL)
HHOOK NEAR _afxHHookOldCbtFilter = NULL;
HHOOK NEAR _afxHHookOldMsgFilter = NULL;
#endif

#ifdef _AFXDLL
extern void WINAPI _AfxAppDataTerm();       // from 'dllinit.cpp'
#endif

/////////////////////////////////////////////////////////////////////////////
// Message Filter processing (WH_MSGFILTER)

#ifndef MSGF_DDEMGR
#define MSGF_DDEMGR (-32767)
#endif

#if !defined(_USRDLL) && !defined(_AFXCTL)
LRESULT CALLBACK AFX_EXPORT
_AfxMsgFilterHook(int code, WPARAM wParam, LPARAM lParam)
{
	if (code < 0 && code != MSGF_DDEMGR)
	{
		if (!afxData.bWin31)
		{
#ifdef STRICT
			return ::DefHookProc(code, wParam, lParam, &_afxHHookOldMsgFilter);
#else
			return ::DefHookProc(code, wParam, lParam,
				(HOOKPROC FAR*)&_afxHHookOldMsgFilter);
#endif
		}
		else
		{
			return ::CallNextHookEx(_afxHHookOldMsgFilter, code, wParam, lParam);
		}
	}

	ASSERT(wParam == 0);
	return (LRESULT)AfxGetApp()->ProcessMessageFilter(code, (LPMSG)lParam);
}
#endif //!_USRDLL && !_AFXCTL

/////////////////////////////////////////////////////////////////////////////
// Standard cleanup called by WinMain and AfxAbort

extern "C"
void AFXAPI AfxWinTerm(void)
{
	// cleanup OLE 2.0 libraries
	if (AfxGetApp() != NULL && AfxGetApp()->m_lpfnOleTerm != NULL)
		(*AfxGetApp()->m_lpfnOleTerm)();

#ifdef _AFXDLL
	_AfxAppDataTerm();
#endif //_AFXDLL

#if !defined(_USRDLL) && !defined(_AFXCTL)
	// if we registered ourself with PenWin, deregister now
	if (_afxRegisterPenAppProc != NULL)
	{
		(*_afxRegisterPenAppProc)(RPA_DEFAULT, FALSE);
		_afxRegisterPenAppProc = NULL;
	}

	if (afxDlgBkBrush != NULL)
	{
		::DeleteObject(afxDlgBkBrush);
		afxDlgBkBrush = NULL;
	}

	if (_afxHHookOldMsgFilter != NULL)
	{
		if (!afxData.bWin31)
			::UnhookWindowsHook(WH_MSGFILTER, (HOOKPROC)_AfxMsgFilterHook);
		else
			::UnhookWindowsHookEx(_afxHHookOldMsgFilter);
		_afxHHookOldMsgFilter = NULL;
	}

	if (_afxHHookOldCbtFilter != NULL)
	{
		ASSERT(afxData.bWin31);
		::UnhookWindowsHookEx(_afxHHookOldCbtFilter);
		_afxHHookOldCbtFilter = NULL;
	}
#endif //!_USRDLL && !_AFXCTL

	AfxResetMsgCache();
}

/////////////////////////////////////////////////////////////////////////////
