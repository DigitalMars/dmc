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

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// Initialization of MFC extension DLL

#include "afxdllx.h"

AFX_EXTENSION_MODULE NEAR _afxExtensionDLL = { NULL, NULL };

extern "C" int CALLBACK LibMain(HINSTANCE hInstance, WORD, WORD, LPCSTR)
{
	AfxInitExtensionModule(_afxExtensionDLL, hInstance);
	return 1;
}


#define FORCE_VECTOR(className) { new className[1]; }

static void ForceVectorDelete()
{
	FORCE_VECTOR(COleDataObject)
	FORCE_VECTOR(COleDataSource)
	FORCE_VECTOR(COleDispatchDriver)
	FORCE_VECTOR(COleDropSource)
	FORCE_VECTOR(COleDropTarget)
	FORCE_VECTOR(COleIPFrameWnd)
	FORCE_VECTOR(COleResizeBar)
	FORCE_VECTOR(COleStreamFile)
	FORCE_VECTOR(COleTemplateServer)
}

#ifdef AFX_CORE1_SEG
#pragma code_seg("AFX_VECTDEL_TEXT")
#endif

/////////////////////////////////////////////////////////////////////////////
