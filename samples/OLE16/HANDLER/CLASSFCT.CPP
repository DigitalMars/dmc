//----------------------------------------------------------------------------
//
// (c) Copyright Microsoft Corp. 1993 All Rights Reserved
//
//  module: classfct.cpp
//  date:
//  author:
//
//  purpose:
//
//----------------------------------------------------------------------------
#include "stdafx.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif


STDAPI
  DllGetClassObject(REFCLSID rclsid, REFIID riid, LPVOID FAR* ppv)
//----------------------------------------------------------------------------
// Exported DLL initialization is run in context of running application
//----------------------------------------------------------------------------
{
  HRESULT hRes;

  CClassFactory *pClassFactory =
	new CClassFactory(rclsid);

  if (pClassFactory != NULL) {
	hRes = pClassFactory->QueryInterface(riid, ppv);

	pClassFactory->Release();
  }

  return hRes;

} /* DLLGetClassObject()
   */


CClassFactory::CClassFactory(REFCLSID rclsid)
//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
{
  m_dwRefs = 1;
  m_ClsId = rclsid;

} /* CClassFactory()
   */


CClassFactory::~CClassFactory()
//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
{
} /* ~CClassFactory()
   */


STDMETHODIMP CClassFactory::
  QueryInterface(REFIID riid, LPVOID FAR* ppobj)
//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
{
  ULONG dwRefs;
  HRESULT hRes;

  *ppobj = NULL;

  if (riid == IID_IUnknown) {
	*ppobj = (LPVOID)this;

	}else if (riid == IID_IClassFactory) {
	*ppobj = (LPVOID)this;
  }

  if (*ppobj == NULL) hRes = ResultFromScode(E_NOINTERFACE);
  else{
	dwRefs = ((LPUNKNOWN)*ppobj)->AddRef();
	hRes = NOERROR;
  }

  return hRes;

} /* QueryInterface()
   */


STDMETHODIMP_(ULONG) CClassFactory::
  AddRef(VOID)
//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
{
  ULONG dwRefs = ++m_dwRefs;
  return dwRefs;

} /* AddRef()
   */


STDMETHODIMP_(ULONG) CClassFactory::
  Release(VOID)
//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
{
  ULONG dwRefs = --m_dwRefs;

  if (dwRefs == 0) {
	delete this;
  }
  return dwRefs;

} /* Release()
   */


STDMETHODIMP CClassFactory::
  CreateInstance(IUnknown FAR* pUnkOuter, REFIID riid, LPLPVOID ppunkObject)
//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
{
  HRESULT hRes;
  CHandlerObject* pObject;

	if (ppunkObject) {
		*ppunkObject = NULL;
	}

	if (pUnkOuter != NULL) {
		hRes = ResultFromScode(E_NOTIMPL);
		goto errReturn;
	}

  pObject = new CHandlerObject(NULL);

  if ((hRes = pObject->CreateObject(m_ClsId, riid, ppunkObject)) != NOERROR) {
	delete pObject;
  }

errReturn:

  return hRes;
} /* CreateInstance()
   */


STDMETHODIMP CClassFactory::
  LockServer(BOOL fLock)
//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
{
  HRESULT hRes = ResultFromScode(E_NOTIMPL);

  return hRes;
} /* LockServer()
   */
