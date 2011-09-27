//----------------------------------------------------------------------------
//
// (c) Copyright Microsoft Corp. 1993 All Rights Reserved
//
//  module: classfct.cpp
//  date:
//  author: tonykit
//
//  purpose:
//
//----------------------------------------------------------------------------
#include "stdafx.h"
#include "classfct.h"


CClassFactory::CClassFactory()
//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
{
  m_dwRefs = 1;
  m_ClsId = GUID_NULL;
  m_pClassFactory = NULL;
  m_hWndOwner = NULL;
  m_dwOleAdvise = 0;
  m_dwDataAdvise = 0;

} /* CClassFactory()
   */


CClassFactory::~CClassFactory()
//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
{

} /* ~CClassFactory()
   */


HRESULT CClassFactory::
  CreateClassObject(HWND hWndOwner, REFCLSID rclsid)
//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
{
  HRESULT hRes;

  m_ClsId = rclsid;
  m_hWndOwner = hWndOwner;

  hRes = CoGetClassObject(m_ClsId, CLSCTX_INPROC_SERVER|CLSCTX_INPROC_HANDLER,
						  NULL, IID_IClassFactory, (LPLPVOID)&m_pClassFactory);

  return hRes;

} /* CreateClassObject()
   */


HRESULT CClassFactory::
  UpdateContainer(VOID)
//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
{
  HRESULT hRes = ResultFromScode(E_FAIL);
  LPOLECLIENTSITE pClientSite=NULL;

  if (m_pOleObject != NULL) {
	if ((hRes = m_pOleObject->GetClientSite(&pClientSite)) == NOERROR) {
	  if (pClientSite != NULL) {
		hRes = pClientSite->SaveObject();
		pClientSite->Release();
	  }
	}
  }

  return hRes;

} /* UpdateContainer()
   */


BOOL CClassFactory::
  CanRevokeNow(VOID)
//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
{
  DWORD dwRefs = 1;

  if (m_pOleObject != NULL) {
	dwRefs = m_pOleObject->AddRef();
	dwRefs = m_pOleObject->Release();

	if (dwRefs == 1) {
	  m_pOleObject->Release();
	  m_pOleObject == NULL;
	}
  }

  return (dwRefs == 1);

} /* CanRevokeNow()
   */


STDMETHODIMP CClassFactory::
  QueryInterface(REFIID riid, LPVOID FAR* ppobj)
//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
{
  ULONG dwRefs;
  HRESULT hRes = ResultFromScode(E_NOINTERFACE);

  *ppobj = NULL;

  if ((riid == IID_IUnknown) || (riid == IID_IClassFactory)) {
	*ppobj = (LPVOID)(LPCLASSFACTORY)this;

  }else if ((riid == IID_IAdviseSink) || (riid == IID_IAdviseSink2)) {
	*ppobj = (LPVOID)(LPADVISESINK2)this;
  }

  if (*ppobj != NULL) {
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
  LPDATAOBJECT pDataObject;
  HRESULT hRes;

  if (m_pClassFactory == NULL) {
	return ResultFromScode(E_FAIL);
  }

  hRes = m_pClassFactory->CreateInstance(pUnkOuter, riid, ppunkObject);

  if ((hRes == NOERROR) && (*ppunkObject != NULL)) {
	hRes = ((LPUNKNOWN)*ppunkObject)->QueryInterface(IID_IOleObject,
												  (LPVOID FAR*)&m_pOleObject);
	if (m_pOleObject != NULL) {
	  hRes = m_pOleObject->Advise((LPADVISESINK)this, &m_dwOleAdvise);
	}

	hRes = ((LPUNKNOWN)*ppunkObject)->QueryInterface(IID_IDataObject,
												  (LPVOID FAR*)&pDataObject);
	if (pDataObject != NULL) {
	  FORMATETC fetc;

	  fetc.cfFormat = NULL;
	  fetc.ptd = NULL;
	  fetc.lindex = -1;
	  fetc.dwAspect = -1L;
	  fetc.tymed = -1L;

	  hRes = pDataObject->DAdvise(&fetc, ADVF_PRIMEFIRST | ADVF_NODATA,
								  (LPADVISESINK)this,  &m_dwDataAdvise);
	  pDataObject->Release();
	}

  }

  return hRes;

} /* CreateInstance()
   */


STDMETHODIMP CClassFactory::
  LockServer(BOOL fLock)
//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
{
  if (m_pClassFactory == NULL) {
	return ResultFromScode(E_FAIL);
  }

  return m_pClassFactory->LockServer(fLock);

} /* LockServer()
   */


STDMETHODIMP_(void) CClassFactory::
  OnDataChange(FORMATETC FAR* /*petc*/, STGMEDIUM FAR* /*pmed*/)
//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
{
  ::PostMessage(m_hWndOwner, WM_UPDATE_CONTAINER, 0, 0L);

} /* OnDataChange()
   */


STDMETHODIMP_(void) CClassFactory::
  OnViewChange(DWORD /*dwAspect*/, LONG /*lindex*/)
//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
{
} /* OnViewChange()
   */


STDMETHODIMP_(void) CClassFactory::
  OnRename(LPMONIKER /*pmk*/)
//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
{
} /* OnRename()
   */


STDMETHODIMP_(void) CClassFactory::
  OnSave(VOID)
//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
{
} /* OnSave()
   */


STDMETHODIMP_(void) CClassFactory::
  OnClose(VOID)
//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
{
  LPDATAOBJECT pDataObject;
  HRESULT hRes;

  if (m_pOleObject != NULL) {
	if (m_dwDataAdvise != 0) {

	  hRes = m_pOleObject->QueryInterface(IID_IDataObject,
										  (LPVOID FAR*)&pDataObject);
	  if (pDataObject != NULL) {
		hRes = pDataObject->DUnadvise(m_dwDataAdvise);
		pDataObject->Release();
		m_dwDataAdvise = 0;
	  }
	}

	if (m_dwOleAdvise != 0) {
	  hRes = m_pOleObject->Unadvise(m_dwOleAdvise);
	  m_dwOleAdvise = 0;
	}
  }

  if (m_pClassFactory != NULL) {
	m_pClassFactory->Release();
	m_pClassFactory = NULL;
  }

  ::PostMessage(m_hWndOwner, WM_CLOSE, 0, 0L);

} /* OnClose()
   */


STDMETHODIMP_(void) CClassFactory::
  OnLinkSrcChange(LPMONIKER /*pmk*/)
//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
{
} /* OnLinkSrcChange()
   */
