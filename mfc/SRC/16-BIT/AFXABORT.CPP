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

#ifdef AFX_AUX_SEG
#pragma code_seg(AFX_AUX_SEG)
#endif

//  NOTE: in separate module so if can be replaced if needed

#ifdef _AFXDLL
// Callback to abort application if something goes wrong in MFC250.DLL
extern "C"
void AFX_EXPORT CALLBACK AfxAppAbort()
#else
// Otherwise just app specific abort
void AFXAPI AfxAbort()
#endif
{
	TRACE0("AfxAbort called\n");
#ifdef _WINDOWS
	AfxWinTerm();
#endif
	abort();
}
