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

#ifdef AFX_CORE4_SEG
#pragma code_seg(AFX_CORE4_SEG)
#endif

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// Global functions which handle app shutdown

BOOL AFXAPI AfxOleCanExitApp()
{
	return _afxOleState.nObjectCount == 0;
}

/////////////////////////////////////////////////////////////////////////////
// Access to "user-control" state

void AFXAPI AfxOleSetUserCtrl(BOOL bUserCtrl)
{
#ifdef _DEBUG
	if (bUserCtrl && !_afxOleState.bUserCtrl &&
		(AfxGetApp()->m_pMainWnd == NULL ||
		!AfxGetApp()->m_pMainWnd->IsWindowVisible()))
	{
		// If the user gets control while the application window is
		//  not visible.  The application may not shutdown when the object
		//  count reaches zero.
		TRACE0("Warning: "
			"AfxOleSetUserCtrl(TRUE) called with application window hidden.\n");
	}
#endif
	_afxOleState.bUserCtrl = bUserCtrl;
}

BOOL AFXAPI AfxOleGetUserCtrl()
{
	return _afxOleState.bUserCtrl;
}

/////////////////////////////////////////////////////////////////////////////
