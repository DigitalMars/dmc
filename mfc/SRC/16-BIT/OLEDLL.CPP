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

#ifndef _AFXCTL
#ifndef _USRDLL
	#error OLEDLL.CPP is to be used only for the _USRDLL and _AFXCTL variants.
#endif
#endif

#ifdef AFX_OLE3_SEG
#pragma code_seg(AFX_OLE3_SEG)
#endif

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// Support for MFC/COM in DLLs

#ifdef _AFXCTL
HRESULT AfxDllGetClassObject(REFCLSID rclsid, REFIID riid, LPVOID FAR* ppv)
#else
extern "C"
STDAPI DllGetClassObject(REFCLSID rclsid, REFIID riid, LPVOID FAR* ppv)
#endif
{
	// find class factory with same class ID

	COleObjectFactory* pFactory = _afxFirstFactory;
	while (pFactory != NULL)
	{
		if (pFactory->m_dwRegister != 0 && pFactory->m_clsid == rclsid)
		{
			// found suitable class factory -- attempt to create the instance
			VERIFY(pFactory->ExternalQueryInterface(&riid, ppv)
				== NOERROR);
			return NOERROR;
		}
		pFactory = pFactory->m_pNextFactory;
	}

	// factory not registered -- return error
	return ResultFromScode(CLASS_E_CLASSNOTAVAILABLE);
}

#ifdef _AFXCTL
HRESULT AfxDllCanUnloadNow(void)
#else
extern "C"
STDAPI DllCanUnloadNow(void)
#endif
{
	// return S_TRUE only if no outstanding objects active
	return AfxOleCanExitApp() ? NOERROR : ResultFromScode(S_FALSE);
}

/////////////////////////////////////////////////////////////////////////////
