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

#ifdef AFX_OLE3_SEG
#pragma code_seg(AFX_OLE3_SEG)
#endif

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

#ifndef _AFXDLL
COleObjectFactory* NEAR _afxFirstFactory;
#endif

/////////////////////////////////////////////////////////////////////////////
// COleObjectFactory implementation

IMPLEMENT_DYNAMIC(COleObjectFactory, CCmdTarget)

BEGIN_INTERFACE_MAP(COleObjectFactory, CCmdTarget)
	INTERFACE_PART(COleObjectFactory, IID_IClassFactory, ClassFactory)
END_INTERFACE_MAP()

COleObjectFactory::COleObjectFactory(REFCLSID clsid,
	CRuntimeClass* pRuntimeClass, BOOL bMultiInstance, LPCSTR lpszProgID)
{
	ASSERT(AfxIsValidAddress(&clsid, sizeof(CLSID), FALSE));
	ASSERT(lpszProgID == NULL || AfxIsValidString(lpszProgID));

	// initialize to unregistered state
	m_dwRegister = 0;   // not registered yet
	m_clsid = clsid;
	m_pRuntimeClass = pRuntimeClass;
	m_bMultiInstance = bMultiInstance;
	m_lpszProgID = lpszProgID;

#ifdef _AFXDLL
	// need to allocate OLE state if not already allocated
	if (_AfxGetAppData()->appOleState == NULL)
	{
		_AfxGetAppData()->appOleState = new AFX_OLESTATE;
		if (_AfxGetAppData()->appOleState == NULL)
			return;
	}
#endif

	// add this factory to the list of factories
	m_pNextFactory = _afxFirstFactory;
	_afxFirstFactory = this;

	ASSERT_VALID(this);
}

COleObjectFactory::~COleObjectFactory()
{
	ASSERT_VALID(this);

	// deregister this class factory
	Revoke();

#ifdef _AFXDLL
	// don't attempt to remove from list if OLE state already deleted
	if (_AfxGetAppData()->appOleState == NULL)
		return;
#endif

	// remove this class factory from the list of active class factories
	COleObjectFactory* pFactory = _afxFirstFactory;
	ASSERT(pFactory != NULL);
	if (pFactory == this)
	{
		// special case for first factory in the list -- point first at next
		_afxFirstFactory = m_pNextFactory;
	}
	else
	{
		// find the link that points to the one we are removing
		while (pFactory->m_pNextFactory != this)
		{
			pFactory = pFactory->m_pNextFactory;
			ASSERT(pFactory != NULL);   // must find it before end
		}
		// and point it to the next one
		pFactory->m_pNextFactory = m_pNextFactory;
	}
}

BOOL COleObjectFactory::Register()
{
	ASSERT_VALID(this);
	ASSERT(m_dwRegister == 0);  // registering server/factory twice?
	ASSERT(m_clsid != CLSID_NULL);

#if !defined(_USRDLL) && !defined(_AFXCTL)
	// In the application variants, the IClassFactory is registered
	//  with the OLE DLLs.

	HRESULT hr;
	hr = ::CoRegisterClassObject(m_clsid, &m_xClassFactory, CLSCTX_LOCAL_SERVER,
		m_bMultiInstance ?  REGCLS_SINGLEUSE : REGCLS_MULTIPLEUSE,
		&m_dwRegister);
	if (hr != NOERROR)
	{
#ifdef _DEBUG
		TRACE1("Warning: CoRegisterClassObject failed scode = %Fs\n",
			::AfxGetFullScodeString(GetScode(hr)));
#endif
		// registration failed.
		return FALSE;
	}

	ASSERT(m_dwRegister != 0);
	return TRUE;
#else
	// In the _USRDLL variant, it is not necessary to register it.

	m_dwRegister = 1;   // simply indicate the factory as registered
	return TRUE;
#endif // !_USRDLL && !_AFXCTL
}

BOOL PASCAL COleObjectFactory::RegisterAll()
{
	BOOL bResult = TRUE;
	COleObjectFactory* pFactory = _afxFirstFactory;
	while (pFactory != NULL)
	{
		// register any non-registered, non-doctemplate factories
		if (!pFactory->IsRegistered() &&
			pFactory->m_clsid != CLSID_NULL && !pFactory->Register())
		{
			bResult = FALSE;
		}
		pFactory = pFactory->m_pNextFactory;
	}
	return bResult;
}

void COleObjectFactory::Revoke()
{
	ASSERT_VALID(this);

	if (m_dwRegister != 0)
	{
		// revoke the registration of the class itself
#if !defined(_USRDLL) && !defined(_AFXCTL)
		::CoRevokeClassObject(m_dwRegister);
#endif
		m_dwRegister = 0;
	}
}

void PASCAL COleObjectFactory::RevokeAll()
{
	COleObjectFactory* pFactory = _afxFirstFactory;
	while (pFactory != NULL)
	{
		pFactory->Revoke();
		pFactory = pFactory->m_pNextFactory;
	}
}

void COleObjectFactory::UpdateRegistry(LPCSTR lpszProgID)
{
#ifdef _AFXCTL
	ASSERT(FALSE);  // should use COleObjectFactoryEx
#else
	ASSERT_VALID(this);
	ASSERT(lpszProgID == NULL || AfxIsValidString(lpszProgID));

	// use default prog-id if specific prog-id not given
	if (lpszProgID == NULL)
	{
		lpszProgID = m_lpszProgID;
		if (lpszProgID == NULL) // still no valid progID?
			return;
	}

	// call global helper to modify system registry
	//  (progid, shortname, and long name are all equal in this case)
	AfxOleRegisterServerClass(m_clsid, lpszProgID, lpszProgID, lpszProgID,
		OAT_DISPATCH_OBJECT);
#endif
}

void PASCAL COleObjectFactory::UpdateRegistryAll()
{
#ifdef _AFXCTL
	ASSERT(FALSE);  // should use COleObjectFactoryEx
#else
	COleObjectFactory* pFactory = _afxFirstFactory;
	while (pFactory != NULL)
	{
		pFactory->UpdateRegistry(); // will register with default m_lpszProgID
		pFactory = pFactory->m_pNextFactory;
	}
#endif
}

CCmdTarget* COleObjectFactory::OnCreateObject()
{
	ASSERT_VALID(this);
	ASSERT(AfxIsValidAddress(m_pRuntimeClass, sizeof(CRuntimeClass)));
		// this implementation needs a runtime class

	// allocate object, throw exception on failure
	CCmdTarget* pTarget = (CCmdTarget*)m_pRuntimeClass->CreateObject();
	if (pTarget == NULL)
		AfxThrowMemoryException();

	// make sure it is a CCmdTarget
	ASSERT(pTarget->IsKindOf(RUNTIME_CLASS(CCmdTarget)));
	ASSERT_VALID(pTarget);

	// return the new CCmdTarget object
	return pTarget;
}

/////////////////////////////////////////////////////////////////////////////
// Implementation of COleObjectFactory::IClassFactory interface

STDMETHODIMP_(ULONG) COleObjectFactory::XClassFactory::AddRef()
{
	METHOD_PROLOGUE(COleObjectFactory, ClassFactory)
	return (ULONG)pThis->ExternalAddRef();
}

STDMETHODIMP_(ULONG) COleObjectFactory::XClassFactory::Release()
{
	METHOD_PROLOGUE(COleObjectFactory, ClassFactory)
	return (ULONG)pThis->ExternalRelease();
}

STDMETHODIMP COleObjectFactory::XClassFactory::QueryInterface(
	REFIID iid, LPVOID far* ppvObj)
{
	METHOD_PROLOGUE(COleObjectFactory, ClassFactory)
	return (HRESULT)pThis->ExternalQueryInterface(&iid, ppvObj);
}

STDMETHODIMP COleObjectFactory::XClassFactory::CreateInstance(
	IUnknown FAR* pUnkOuter, REFIID riid, LPVOID FAR* ppunkObject)
{
	METHOD_PROLOGUE(COleObjectFactory, ClassFactory)
	ASSERT_VALID(pThis);

	*ppunkObject = NULL;

	// outer objects must ask for IUnknown only
	ASSERT(pUnkOuter == NULL || riid == IID_IUnknown);

	// attempt to create the object
	CCmdTarget* pTarget = NULL;
	SCODE sc = E_OUTOFMEMORY;
	TRY
	{
		// attempt to create the object
		pTarget = pThis->OnCreateObject();
		if (pTarget != NULL)
		{
			// check for aggregation on object not supporting it
			sc = CLASS_E_NOAGGREGATION;
			if (pUnkOuter == NULL || pTarget->m_xInnerUnknown != 0)
			{
				// create aggregates used by the object
				pTarget->m_pOuterUnknown = pUnkOuter;
				sc = E_OUTOFMEMORY;
				if (pTarget->OnCreateAggregates())
					sc = S_OK;
			}
		}
	}
	END_TRY

	// finish creation
	if (sc == S_OK)
	{
		DWORD dwRef = 1;
		if (pUnkOuter != NULL)
		{
			// return inner unknown instead of IUnknown
			*ppunkObject = &pTarget->m_xInnerUnknown;
		}
		else
		{
			// query for requested interface
			sc = pTarget->InternalQueryInterface(&riid, ppunkObject);
			if (sc == S_OK)
			{
				dwRef = pTarget->InternalRelease();
				ASSERT(dwRef != 0);
			}
		}
		if (dwRef != 1)
			TRACE1("Warning: object created with reference of %ld", dwRef);
	}

	// cleanup in case of errors
	if (sc != S_OK)
		delete pTarget;

	return ResultFromScode(sc);
}

STDMETHODIMP COleObjectFactory::XClassFactory::LockServer(BOOL fLock)
{
	SCODE sc = E_UNEXPECTED;
	TRY
	{
		if (fLock)
			AfxOleLockApp();
		else
			AfxOleUnlockApp();
		sc = S_OK;
	}
	END_TRY

	return ResultFromScode(sc);
}

//////////////////////////////////////////////////////////////////////////////
// Diagnostics

#ifdef _DEBUG
void COleObjectFactory::AssertValid() const
{
	CCmdTarget::AssertValid();
	ASSERT(m_lpszProgID == NULL || AfxIsValidString(m_lpszProgID));
	ASSERT(m_pRuntimeClass == NULL ||
		AfxIsValidAddress(m_pRuntimeClass, sizeof(CRuntimeClass), FALSE));
	ASSERT(m_pNextFactory == NULL ||
		AfxIsValidAddress(m_pNextFactory, sizeof(COleObjectFactory)));
}

void COleObjectFactory::Dump(CDumpContext& dc) const
{
	CCmdTarget::Dump(dc);
	AFX_DUMP1(dc, "\nm_pNextFactory = ", (void*)m_pNextFactory);
	AFX_DUMP1(dc, "\nm_dwRegister = ", m_dwRegister);
	LPSTR lpszClassID = NULL;
	if (StringFromCLSID(m_clsid, &lpszClassID) == NOERROR)
	{
		AFX_DUMP1(dc, "\nm_clsid = ", lpszClassID);
		_AfxFreeTaskMem(lpszClassID);
	}
	AFX_DUMP1(dc, "\nm_pRuntimeClass = ", m_pRuntimeClass);
	AFX_DUMP1(dc, "\nm_bMultiInstance = ", m_bMultiInstance);
	AFX_DUMP1(dc, "\nm_lpszProgID = ", m_lpszProgID);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
