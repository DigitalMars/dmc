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
#include "xserver.h"

DWORD g_dwObjectCount = 0;


STDAPI
  DllGetClassObject(REFCLSID rclsid, REFIID riid, LPVOID FAR* ppv)
//----------------------------------------------------------------------------
// Exported DLL initialization is run in context of running application
//----------------------------------------------------------------------------
{
  HRESULT hRes = ResultFromScode(E_OUTOFMEMORY);

  CClassFactory *pClassFactory =
	new CClassFactory(rclsid);

  if (pClassFactory != NULL) {
	hRes = pClassFactory->QueryInterface(riid, ppv);
	pClassFactory->Release();
  }

  return hRes;

} /* DllGetClassObject()
   */


STDAPI
  DllCanUnloadNow(void)
//----------------------------------------------------------------------------
// Exported DLL initialization is run in context of running application
//----------------------------------------------------------------------------
{
  if (g_dwObjectCount == 0) {
	return NOERROR;
  }

  return ResultFromScode(S_FALSE);

} /* DllCanUnloadNow()
   */


CClassFactory::CClassFactory(REFCLSID rclsid)
//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
{
  g_dwObjectCount++ ;

  m_dwRefs = 1;
  m_ClsId = rclsid;

} /* CClassFactory()
   */


CClassFactory::~CClassFactory()
//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
{
  g_dwObjectCount-- ;

} /* ~CClassFactory()
   */


STDMETHODIMP CClassFactory::
  QueryInterface(REFIID riid, LPVOID FAR* ppobj)
//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
{
  CDebug db;

  db << "IClassFactory::QueryInterface(" << riid << (LPVOID)ppobj
	 << ") Called" ;
  db.DebugOutput();

  ULONG dwRefs;
  HRESULT hRes = ResultFromScode(E_NOINTERFACE);

  *ppobj = NULL;

  if ((riid == IID_IUnknown) || (riid == IID_IClassFactory)) {
	*ppobj = (LPVOID)this;
  }

  if (*ppobj != NULL) {
	dwRefs = ((LPUNKNOWN)*ppobj)->AddRef();
	hRes = NOERROR;
  }

  if (hRes == NOERROR) {
	db << "IClassFactory::QueryInterface(" << riid << (LPUNKNOWN)*ppobj
	   << ") Returning " << hRes ;
  }else{
	db << "IClassFactory::QueryInterface() Returning " << hRes ;
  }

  db.DebugOutput();

  return hRes;

} /* QueryInterface()
   */


STDMETHODIMP_(ULONG) CClassFactory::
  AddRef(VOID)
//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
{
  CDebug db;

  db << "IClassFactory::AddRef()  Called";
  db.DebugOutput();

  ULONG dwRefs = ++m_dwRefs;

  db << "IClassFactory::AddRef()  Returning " << dwRefs;
  db.DebugOutput();

  return dwRefs;

} /* AddRef()
   */


STDMETHODIMP_(ULONG) CClassFactory::
  Release(VOID)
//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
{
  CDebug db;

  db << "IClassFactory::Release() Called";
  db.DebugOutput();

  ULONG dwRefs = --m_dwRefs;

  if (dwRefs == 0) {
	delete this;
  }

  db << "IClassFactory::Release() Returning " << dwRefs;
  db.DebugOutput();
  return dwRefs;

} /* Release()
   */


STDMETHODIMP CClassFactory::
  CreateInstance(IUnknown FAR* pUnkOuter, REFIID riid, LPLPVOID ppunkObject)
//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
{
  CDebug db;

  db << "IClassFactory::CreateInstance(" << pUnkOuter << riid
	 << (LPVOID)ppunkObject << ") Called" ;
  db.DebugOutput();

  HRESULT hRes = ResultFromScode(E_INVALIDARG);
  CXObject *pObject;

	if (ppunkObject) {
		*ppunkObject = NULL;
	}

  hRes = ResultFromScode(CLASS_E_NOAGGREGATION);

	if (pUnkOuter == NULL) {
	pObject = new CXObject(pUnkOuter);
	hRes = pObject->CreateObject(m_ClsId, riid, ppunkObject);
	if (hRes != NOERROR) {
	  delete pObject;
	}
  }

  if (hRes == NOERROR) {
	db << "IClassFactory::CreateInstance(" << pUnkOuter << riid
	   << (LPVOID)*ppunkObject << ") Returning " << hRes ;
  }else{
	db << "IClassFactory::CreateInstance() Returning " << hRes ;
  }

  db.DebugOutput();

  return hRes;
} /* CreateInstance()
   */


STDMETHODIMP CClassFactory::
  LockServer(BOOL fLock)
//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
{
  CDebug db;

  db << "IClassFactory::LockServer(" << fLock << ") Called" ;
  db.DebugOutput();

  HRESULT hRes = NOERROR;

  if (fLock) {
	g_dwObjectCount++ ;

  }else{
	g_dwObjectCount-- ;
  }

  db << "IClassFactory::LockServer() Returning " << hRes;
  db.DebugOutput();
  return hRes;

} /* LockServer()
   */
