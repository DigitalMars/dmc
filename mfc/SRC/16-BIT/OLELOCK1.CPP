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

// Note: these functions can be replaced by replacing this module in its
//  entirety (although doing so would be rare).

void AFXAPI AfxOleOnReleaseAllObjects()
{
	// don't shut down the application if user is in control of the app
	if (AfxOleGetUserCtrl())
		return;

	AfxOleSetUserCtrl(TRUE);    // avoid re-entry

	// shut down the application
	ASSERT_VALID(AfxGetApp());
	if (AfxGetApp()->m_pMainWnd != NULL)
	{
		// destroy the main window (only if enabled)
		if (AfxGetApp()->m_pMainWnd->IsWindowEnabled())
		{
			// the main window will post WM_QUIT as part of its shutdown
			AfxGetApp()->m_pMainWnd->DestroyWindow();
		}
	}
	else
	{
		// no main window, so just post WM_QUIT.
		::PostQuitMessage(0);
	}
}

void AFXAPI AfxOleLockApp()
{
	++_afxOleState.nObjectCount;
}

void AFXAPI AfxOleUnlockApp()
{
	ASSERT(_afxOleState.nObjectCount != 0);

	if (--_afxOleState.nObjectCount == 0)
	{
		// allow application to shut down when all the objects have
		//  been released
		::AfxOleOnReleaseAllObjects();
	}
}

/////////////////////////////////////////////////////////////////////////////
