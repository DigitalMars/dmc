/**************************************************************************
 *
 *  THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF ANY
 *  KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
 *  IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A PARTICULAR
 *  PURPOSE.
 *
 *  Copyright (c) 1993 - 1995  Microsoft Corporation.  All Rights Reserved.
 *
 **************************************************************************/

#define INC_OLE2
#include <windows.h>
#include <windowsx.h>
#include <mmsystem.h>
#ifndef WIN32
#include <ole2.h>
#endif
#include <string.h>
#include <vfw.h>
#include "aviview.h"
#include "regobjs.h"

/*
** The usage counter keeps track of the overall usage of objects based on
** implementations provided by the component. This allows one to determine
** when the implementation is no longer in use.
*/

UINT	uUseCount;
BOOL	fLocked;


IClassFactory FAR * gpCF;
DWORD		    gdwRegistered;

extern "C" BOOL RegisterObjects(void);
extern "C" void RevokeObjects(void);

BOOL RegisterObjects(void)
{
#if 0
    HRESULT hresult;
#endif

    // OleInitialize(NULL);
#if 0
    CAppCF::MakeInst(CLSID_ACMCmprsStream, IID_IClassFactory,
		   (void FAR * FAR *) &gpCF);

    hresult = CoRegisterClassObject(CLSID_ACMCmprsStream,
			  gpCF,
			  CLSCTX_INPROC_SERVER,
			  REGCLS_MULTIPLEUSE,
			  &gdwRegistered);
    if (FAILED(GetScode(hresult))) {
	delete gpCF;
	gpCF = NULL;
	return FALSE;
    }
#endif
	
    return TRUE;
}

void RevokeObjects(void)
{
#if 0
    CoRevokeClassObject(gdwRegistered);
#endif

    // OleUninitialize();
}
/*	-	-	-	-	-	-	-	-	*/

HRESULT CAppCF::MakeInst(
	const CLSID FAR&	rclsid,
	const IID FAR&	riid,
	void FAR* FAR*	ppv)
{
	CAppCF FAR*	pAppCF;
	IUnknown FAR*	pUnknown;
	HRESULT hresult;

	pAppCF = new FAR CAppCF(rclsid, &pUnknown);
	if (pAppCF == NULL)
		return ResultFromScode(E_OUTOFMEMORY);
	hresult = pUnknown->QueryInterface(riid, ppv);
	if (FAILED(GetScode(hresult)))
		delete pAppCF;
	return hresult;
}

/*	-	-	-	-	-	-	-	-	*/

CAppCF::CAppCF(
	const CLSID FAR&	rclsid,
	IUnknown FAR* FAR*	ppUnknown)
{
	m_clsid = rclsid;
	m_refs = 0;
	*ppUnknown = this;
}

/*	-	-	-	-	-	-	-	-	*/

STDMETHODIMP CAppCF::QueryInterface(
	const IID FAR&	iid,
	void FAR* FAR*	ppv)
{
	if (iid == IID_IUnknown)
		*ppv = this;
	else if (iid == IID_IClassFactory)
		*ppv = this;
	else
		return ResultFromScode(E_NOINTERFACE);
	AddRef();
	return NULL;
}

/*	-	-	-	-	-	-	-	-	*/

STDMETHODIMP_(ULONG) CAppCF::AddRef()
{
	return ++m_refs;
}

/*	-	-	-	-	-	-	-	-	*/

STDMETHODIMP_(ULONG) CAppCF::Release()
{
	if (!--m_refs) {
		delete this;
		return 0;
	}
	return m_refs;
}

/*	-	-	-	-	-	-	-	-	*/

STDMETHODIMP CAppCF::CreateInstance(
	IUnknown FAR*	pUnknownOuter,
	const IID FAR&	riid,
	void FAR* FAR*	ppv)
{
#if 0
	if (IsEqualCLSID(m_clsid, CLSID_ACMCmprsStream)) {
		return CACMCmpStream::MakeInst(pUnknownOuter, riid, ppv);
	}
	else
#endif
	{
		return ResultFromScode(CO_E_CANTDETERMINECLASS); // !!!
	}
}

/*	-	-	-	-	-	-	-	-	*/

STDMETHODIMP CAppCF::LockServer(
	BOOL	fLock)
{
	fLocked = fLock;
	return NULL;
}

/*	-	-	-	-	-	-	-	-	*/
