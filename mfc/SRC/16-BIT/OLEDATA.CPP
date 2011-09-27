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

AFX_OLESTATE::AFX_OLESTATE()
{
	nObjectCount = 0;
	bUserCtrl = TRUE;
#ifdef _AFXDLL
	pFirstFactory = NULL;
	bExtensionDLL = FALSE;
#endif

	bNeedTerm = FALSE;
	bNeedTermCOM = FALSE;
	lpTaskMalloc = NULL;
	bNeedTermUI = FALSE;
}

#ifndef _AFXDLL
AFX_OLESTATE NEAR _afxOleState;
#endif

/////////////////////////////////////////////////////////////////////////////
