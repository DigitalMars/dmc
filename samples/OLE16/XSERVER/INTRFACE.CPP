//----------------------------------------------------------------------------
//
// (c) Copyright Microsoft Corp. 1993 All Rights Reserved
//
//  module: interfac.cpp
//  date:
//  author: tonykit
//
//  purpose:
//
//----------------------------------------------------------------------------
#include "xserver.h"


STDMETHODIMP NC(CXObject, CUnknown)::
  QueryInterface(REFIID riid, LPVOID FAR* ppobj)
//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
{
  CDebug db(m_pObject->m_bDebug);

  db << "IUnknown::QueryInterface(" << riid << (LPUNKNOWN)ppobj << ") Called" ;
  db.DebugOutput();

  HRESULT hRes = ResultFromScode(E_NOINTERFACE);

  *ppobj = NULL;

  if (riid == IID_IUnknown) {
	*ppobj = (LPVOID)&m_pObject->m_Unknown;

  }else if (riid == IID_IPersistStorage || riid == IID_IPersist) {
	*ppobj = (LPVOID)&m_pObject->m_PersistStorage;

  }else if (riid == IID_IOleObject) {
	*ppobj = (LPVOID)&m_pObject->m_OleObject;

  }else if (riid == IID_IDataObject) {
	*ppobj = (LPVOID)&m_pObject->m_DataObject;

  }else if ((riid == IID_IViewObject)||(riid == IID_IViewObject2)) {
	*ppobj = (LPVOID)&m_pObject->m_ViewObject2;

  }else if ((riid == IID_IOleCache)||(riid == IID_IOleCache2)) {
	*ppobj = (LPVOID)&m_pObject->m_OleCache2;

  }else if (riid == IID_IRunnableObject) {
	*ppobj = (LPVOID)&m_pObject->m_RunnableObject;

  }else if (riid == IID_IExternalConnection) {
	*ppobj = (LPVOID)&m_pObject->m_ExternalConnection;

  }else if (riid == IID_IStdMarshalInfo) {
	*ppobj = (LPVOID)&m_pObject->m_StdMarshalInfo;

  }else if (riid == IID_IDispatch) {
	*ppobj = (LPVOID)&m_pObject->m_Dispatch;
  }

  if (*ppobj != NULL) {
	((LPUNKNOWN)*ppobj)->AddRef();
	hRes = NOERROR;
  }

  if (hRes == NOERROR) {
	db << "IUnknown::QueryInterface(" << riid << (LPUNKNOWN)*ppobj
	   << ") Returning " << hRes ;
  }else{
	db << "IUnknown::QueryInterface() Returning " << hRes ;
  }

  db.DebugOutput();

  return hRes;
} /* QueryInterface()
   */


STDMETHODIMP_(ULONG) NC(CXObject, CUnknown)::
  AddRef(VOID)
//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
{
  CDebug db(m_pObject->m_bDebug);

  db << "IUnknown::AddRef()  Called";
  db.DebugOutput();

  ULONG dwRefs = ++m_pObject->m_dwRefs;

  db << "IUnknown::AddRef()  Returning " << dwRefs;
  db.DebugOutput();

  return dwRefs;

} /* AddRef()
   */


STDMETHODIMP_(ULONG) NC(CXObject, CUnknown)::
  Release(VOID)
//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
{
  CDebug db(m_pObject->m_bDebug);

  db << "IUnknown::Release() Called";
  db.DebugOutput();

  ULONG dwRefs = --m_pObject->m_dwRefs;

  if (dwRefs == 0) {
	delete m_pObject;
  }

  db << "IUnknown::Release() Returning " << dwRefs;
  db.DebugOutput();
  return dwRefs;

} /* Release()
   */


STDMETHODIMP NC(CXObject, CPersistStorage)::
  QueryInterface(REFIID riid, LPVOID FAR* ppvObj)
//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
{
  return m_pObject->m_pUnkOuter->QueryInterface(riid, ppvObj);
} /* QueryInterface()
   */


STDMETHODIMP_(ULONG) NC(CXObject, CPersistStorage)::
  AddRef(VOID)
//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
{
  return m_pObject->m_pUnkOuter->AddRef();
} /* AddRef()
   */


STDMETHODIMP_(ULONG) NC(CXObject, CPersistStorage)::
  Release(VOID)
//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
{
  return m_pObject->m_pUnkOuter->Release();
} /* Release()
   */


STDMETHODIMP NC(CXObject, CPersistStorage)::
  GetClassID(LPCLSID lpClassID)
//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
{
  CDebug db(m_pObject->m_bDebug);

  db << "IPersist::GetClassID(" << (LPUNKNOWN)lpClassID << ") Called";
  db.DebugOutput();

  HRESULT hRes = ResultFromScode(E_INVALIDARG);

  if (lpClassID != NULL) {
	*lpClassID = m_pObject->m_ClsId;
	hRes = NOERROR;
  }

  db << "IPersist::GetClassID(" << lpClassID << ") Returning "
	 << hRes;
  db.DebugOutput();

  return hRes;
} /* GetClassID()
   */


STDMETHODIMP NC(CXObject, CPersistStorage)::
  IsDirty(VOID)
//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
{
  CDebug db(m_pObject->m_bDebug);

  db << "IPersistStorage::IsDirty() Called";
  db.DebugOutput();

  HRESULT hRes = NOERROR;
  if (m_pObject->m_bIsDirty == FALSE) {
	hRes = ResultFromScode(S_FALSE);
  }

  db << "IPersistStorage::IsDirty() Returning " << hRes;
  db.DebugOutput();

  return hRes;

} /* IsDirty()
   */


STDMETHODIMP NC(CXObject, CPersistStorage)::
  InitNew(LPSTORAGE pStg)
//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
{
  CDebug db(m_pObject->m_bDebug);

  db << "IPersistStorage::InitNew(" << (LPUNKNOWN)pStg << ") Called";
  db.DebugOutput();

  HRESULT hRes = m_pObject->RememberStorage(pStg);
  if (hRes == NOERROR) {
	hRes = m_pObject->m_pPersistStg->InitNew(pStg);
  }

  db << "IPersistStorage::InitNew() Returning " << hRes ;
  db.DebugOutput();

  return hRes;
} /* InitNew()
   */


STDMETHODIMP NC(CXObject, CPersistStorage)::
  Load(LPSTORAGE pStg)
//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
{
  CDebug db(m_pObject->m_bDebug);

  db << "IPersistStorage::Load(" << (LPUNKNOWN)pStg << ") Called";
  db.DebugOutput();

  HRESULT hRes = m_pObject->LoadNativeData(pStg);
  if (hRes == NOERROR) {
	hRes = m_pObject->m_pPersistStg->Load(pStg);
  }

  db << "IPersistStorage::Load() Returning " << hRes ;
  db.DebugOutput();

  return hRes;
} /* Load()
   */


STDMETHODIMP NC(CXObject, CPersistStorage)::
  Save(LPSTORAGE pStgSave, BOOL fSameAsLoad)
//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
{
  CDebug db(m_pObject->m_bDebug);

  db << "IPersistStorage::Save(" << (LPUNKNOWN)pStgSave << fSameAsLoad
	 << ") Called";
  db.DebugOutput();

  HRESULT hRes = m_pObject->SaveNativeData(pStgSave, fSameAsLoad);
  if (hRes == NOERROR) {
	hRes = m_pObject->m_pPersistStg->Save(pStgSave, fSameAsLoad);
  }

  db << "IPersistStorage::Save() Returning " << hRes ;
  db.DebugOutput();

  return hRes;
} /* Save()
   */


STDMETHODIMP NC(CXObject, CPersistStorage)::
  SaveCompleted(LPSTORAGE pStgNew)
//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
{
  CDebug db(m_pObject->m_bDebug);

  db << "IPersistStorage::SaveCompleted(" << (LPUNKNOWN)pStgNew <<") Called";
  db.DebugOutput();

  HRESULT hRes = m_pObject->SaveCompleted(pStgNew);
  if (hRes == NOERROR) {
	hRes = m_pObject->m_pPersistStg->SaveCompleted(pStgNew);
  }

  db << "IPersistStorage::SaveCompleted() Returning " << hRes ;
  db.DebugOutput();

  return hRes;
} /* SaveCompleted()
   */


STDMETHODIMP NC(CXObject, CPersistStorage)::
  HandsOffStorage(VOID)
//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
{
  CDebug db(m_pObject->m_bDebug);

  db << "IPersistStorage::HandsOffStorage() Called" ;
  db.DebugOutput();

  HRESULT hRes = m_pObject->HandsOffStorage();
  if (hRes == NOERROR) {
	hRes = m_pObject->m_pPersistStg->HandsOffStorage();
  }

  db << "IPersistStorage::HandsOffStorage() Returning " << hRes ;
  db.DebugOutput();

  return hRes;
} /* HandsOffStorage()
   */


STDMETHODIMP NC(CXObject, COleObject)::
  QueryInterface(REFIID riid, LPVOID FAR* ppvObj)
//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
{
  return m_pObject->m_pUnkOuter->QueryInterface(riid, ppvObj);
} /* QueryInterface()
   */


STDMETHODIMP_(ULONG) NC(CXObject, COleObject)::
  AddRef(VOID)
//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
{
  return m_pObject->m_pUnkOuter->AddRef();
} /* AddRef()
   */


STDMETHODIMP_(ULONG) NC(CXObject, COleObject)::
  Release(VOID)
//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
{
  return m_pObject->m_pUnkOuter->Release();
} /* Release()
   */


STDMETHODIMP NC(CXObject, COleObject)::
  SetClientSite(LPOLECLIENTSITE pClientSite)
//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
{
  CDebug db(m_pObject->m_bDebug);

  db << "IOleObject::SetClientSite(" << (LPUNKNOWN)pClientSite << ") Called";
  db.DebugOutput();

  HRESULT hRes = m_pObject->SetClientSite(pClientSite);

  db << "IOleObject::SetClientSite() Returning " << hRes ;
  db.DebugOutput();

  return hRes;
} /* SetClientSite()
   */


STDMETHODIMP NC(CXObject, COleObject)::
  GetClientSite(LPOLECLIENTSITE FAR* ppClientSite)
//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
{
  CDebug db(m_pObject->m_bDebug);

  db << "IOleObject::GetClientSite(" << (LPUNKNOWN)ppClientSite << ") Called";
  db.DebugOutput();

  HRESULT hRes = ResultFromScode(E_INVALIDARG);

  if (ppClientSite != NULL) {
	if ((*ppClientSite = m_pObject->m_pClientSite) != NULL) {
	  (*ppClientSite)->AddRef();
	}
	hRes = NOERROR;

	db << "IOleObject::GetClientSite(" << *ppClientSite << ") Returning "
	   << hRes ;

  }else{
	db << "IOleObject::GetClientSite() Returning " << hRes ;
  }

  db.DebugOutput();

  return hRes;
} /* GetClientSite()
   */


STDMETHODIMP NC(CXObject, COleObject)::
  SetHostNames(LPCSTR szContainerApp, LPCSTR szContainerObj)
//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
{
  CDebug db(m_pObject->m_bDebug);

  db << "IOleObject::SetHostNames(" << szContainerApp << ", "
	 << szContainerObj << ") Called";
  db.DebugOutput();

  HRESULT hRes = NOERROR;

  db << "IOleObject::SetHostNames() Returning " << hRes ;
  db.DebugOutput();

  return hRes;
} /* SetHostNames()
   */


STDMETHODIMP NC(CXObject, COleObject)::
  Close(DWORD dwSaveOption)
//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
{
  CDebug db(m_pObject->m_bDebug);

  db << "IOleObject::Close(" << dwSaveOption << ") Called";
  db.DebugOutput();

  HRESULT hRes = m_pObject->Close(dwSaveOption);

  db << "IOleObject::Close() Returning " << hRes ;
  db.DebugOutput();

  return hRes;
} /* Close()
   */


STDMETHODIMP NC(CXObject, COleObject)::
  SetMoniker(DWORD dwWhichMoniker, LPMONIKER pmk)
//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
{
  CDebug db(m_pObject->m_bDebug);

  db << "IOleObject::SetMoniker(" << dwWhichMoniker << pmk << ") Called";
  db.DebugOutput();

  HRESULT hRes = NOERROR;

  if (m_pObject->m_bIsRunning) {
	hRes = m_pObject->RegisterAsRunning();
  }

  db << "IOleObject::SetMoniker() Returning " << hRes ;
  db.DebugOutput();

  return hRes;
} /* SetMoniker()
   */


STDMETHODIMP NC(CXObject, COleObject)::
  GetMoniker(DWORD dwAssign, DWORD dwWhichMoniker, LPMONIKER FAR* ppmk)
//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
{
  CDebug db(m_pObject->m_bDebug);

  db << "IOleObject::GetMoniker(" << dwAssign << dwWhichMoniker
	 << (LPUNKNOWN) ppmk << ") Called";
  db.DebugOutput();

  HRESULT hRes = m_pObject->GetMoniker(dwAssign, dwWhichMoniker, ppmk);

  if (hRes == NOERROR) {
	db << "IOleObject::GetMoniker(" << dwAssign << dwWhichMoniker
	   << *ppmk << ") Returning " << hRes ;

  }else{
	db << "IOleObject::GetMoniker() Returning " << hRes ;
  }

  db.DebugOutput();

  return hRes;
} /* GetMoniker()
   */


STDMETHODIMP NC(CXObject, COleObject)::
  InitFromData(LPDATAOBJECT pDataObject, BOOL fCreation, DWORD dwReserved)
//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
{
  CDebug db(m_pObject->m_bDebug);

  db << "IOleObject::InitFromData(" << (LPUNKNOWN)pDataObject << fCreation
	 << dwReserved << ") Called";
  db.DebugOutput();

  HRESULT hRes = ResultFromScode(S_FALSE);

  db << "IOleObject::InitFromData() Returning " << hRes ;
  db.DebugOutput();

  return hRes;
} /* InitFromData()
   */


STDMETHODIMP NC(CXObject, COleObject)::
  GetClipboardData(DWORD dwReserved, LPDATAOBJECT FAR* ppDObj)
//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
{
  CDebug db(m_pObject->m_bDebug);

  db << "IOleObject::GetClipboardData(" << dwReserved
	 << (LPUNKNOWN)ppDObj << ") Called";
  db.DebugOutput();

  HRESULT hRes = ResultFromScode(E_INVALIDARG);

  if (ppDObj != NULL) {
	*ppDObj = NULL;
	hRes = ResultFromScode(E_NOTIMPL);
  }

  db << "IOleObject::GetClipboardData() Returning " << hRes ;
  db.DebugOutput();

  return hRes;
} /* GetClipboardData()
   */


STDMETHODIMP NC(CXObject, COleObject)::
  DoVerb(LONG iVerb, LPMSG lpmsg, LPOLECLIENTSITE pActiveSite,
		 LONG lindex, HWND hwndParent, LPCRECT lprcPosRect)
//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
{
  CDebug db(m_pObject->m_bDebug);

  db << "IOleObject::DoVerb(" << iVerb << (LPUNKNOWN)lpmsg
	 << (LPUNKNOWN)pActiveSite << lindex << (WORD)hwndParent
	 << lprcPosRect << ") Called";
  db.DebugOutput();

  HRESULT hRes = m_pObject->DoVerb(iVerb, lpmsg, pActiveSite, lindex,
								   hwndParent, lprcPosRect);

  db << "IOleObject::DoVerb() Returning " << hRes ;
  db.DebugOutput();

  return hRes;
} /* DoVerb()
   */


STDMETHODIMP NC(CXObject, COleObject)::
  EnumVerbs(IEnumOLEVERB FAR* FAR* ppenumOleVerb)
//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
{
  CDebug db(m_pObject->m_bDebug);

  db << "IOleObject::EnumVerbs(" << (LPUNKNOWN) ppenumOleVerb << ") Called";
  db.DebugOutput();

  HRESULT hRes = OleRegEnumVerbs(m_pObject->m_ClsId, ppenumOleVerb);

  if ((hRes == NOERROR) && (ppenumOleVerb != NULL)) {
	db << "IOleObject::EnumVerbs(" << (LPUNKNOWN) *ppenumOleVerb
	   << ") Returning " << hRes ;

  }else{
	db << "IOleObject::EnumVerbs() Returning " << hRes ;
  }

  db.DebugOutput();

  return hRes;
} /* EnumVerbs()
   */


STDMETHODIMP NC(CXObject, COleObject)::
  Update(VOID)
//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
{
  CDebug db(m_pObject->m_bDebug);

  db << "IOleObject::Update() Called";
  db.DebugOutput();

  HRESULT hRes = m_pObject->Update();

  db << "IOleObject::Update() Returning " << hRes ;
  db.DebugOutput();

  return hRes;
} /* Update()
   */


STDMETHODIMP NC(CXObject, COleObject)::
  IsUpToDate(VOID)
//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
{
  CDebug db(m_pObject->m_bDebug);

  db << "IOleObject::IsUpToDate() Called";
  db.DebugOutput();

  HRESULT hRes = NOERROR;
  if (m_pObject->m_bIsDirty == TRUE) {
	hRes = ResultFromScode(S_FALSE);
  }

  db << "IOleObject::IsUpToDate() Returning " << hRes ;
  db.DebugOutput();

  return hRes;
} /* IsUpToDate()
   */


STDMETHODIMP NC(CXObject, COleObject)::
  GetUserClassID(CLSID FAR* pClsid)
//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
{
  CDebug db(m_pObject->m_bDebug);

  db << "IOleObject::GetUserClassID(" << (LPUNKNOWN)pClsid << ") Called";
  db.DebugOutput();

  HRESULT hRes = ResultFromScode(E_INVALIDARG);

  if (pClsid != NULL) {
	*pClsid = m_pObject->m_ClsId;
	hRes = NOERROR;

	db << "IOleObject::GetUserClassID(" << *pClsid << ") Returning " << hRes ;

  }else{
	db << "IOleObject::GetUserClassID() Returning " << hRes ;
  }

  db.DebugOutput();

  return hRes;
} /* GetUserClassID()
   */


STDMETHODIMP NC(CXObject, COleObject)::
  GetUserType(DWORD dwFormOfType, LPSTR FAR* pszUserType)
//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
{
  CDebug db(m_pObject->m_bDebug);

  db << "IOleObject::GetUserType(" << dwFormOfType << (LPUNKNOWN)pszUserType
	 << ") Called";
  db.DebugOutput();

  HRESULT hRes = OleRegGetUserType(m_pObject->m_ClsId, dwFormOfType,
								   pszUserType);

  if (hRes == NOERROR) {
	db << "IOleObject::GetUserType(" << dwFormOfType << ", " << *pszUserType
	   << ") Returning " << hRes ;

  }else{
	db << "IOleObject::GetUserType() Returning " << hRes ;
  }

  db.DebugOutput();

  return hRes;
} /* GetUserType()
   */


STDMETHODIMP NC(CXObject, COleObject)::
  SetExtent(DWORD dwDrawAspect, LPSIZEL lpsizel)
//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
{
  CDebug db(m_pObject->m_bDebug);

  db << "IOleObject::SetExtent(" << dwDrawAspect << lpsizel << ") Called";
  db.DebugOutput();

  HRESULT hRes = m_pObject->SetExtent(lpsizel);

  db << "IOleObject::SetExtent() Returning " << hRes ;
  db.DebugOutput();

  return hRes;
} /* SetExtent()
   */


STDMETHODIMP NC(CXObject, COleObject)::
  GetExtent(DWORD dwDrawAspect, LPSIZEL lpsizel)
//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
{
  CDebug db(m_pObject->m_bDebug);

  db << "IOleObject::GetExtent(" << dwDrawAspect << lpsizel << ") Called" ;
  db.DebugOutput();

  HRESULT hRes = m_pObject->GetExtent(dwDrawAspect, -1, NULL, lpsizel);

  db << "IOleObject::GetExtent(" << dwDrawAspect << lpsizel << ") Returning "
	 << hRes ;
  db.DebugOutput();

  return hRes;
} /* GetExtent()
   */


STDMETHODIMP NC(CXObject, COleObject)::
  Advise(IAdviseSink FAR* pAdvSink, DWORD FAR* pdwConnection)
//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
{
  CDebug db(m_pObject->m_bDebug);

  db << "IOleObject::Advise("<< (LPUNKNOWN)pAdvSink << (LPUNKNOWN)pdwConnection
	 << ") Called";
  db.DebugOutput();

  HRESULT hRes = ResultFromScode(E_NOTIMPL);
  if (m_pObject->m_pOleAdviseHolder != NULL) {
	hRes = m_pObject->m_pOleAdviseHolder->Advise(pAdvSink, pdwConnection);
  }

  if (hRes == NOERROR) {
	db << "IOleObject::Advise(" << (LPUNKNOWN)pAdvSink << *pdwConnection
	   << ") Returning " << hRes ;
  }else{
	db << "IOleObject::Advise() Returning " << hRes ;
  }

  db.DebugOutput();

  return hRes;
} /* Advise()
   */


STDMETHODIMP NC(CXObject, COleObject)::
  Unadvise(DWORD dwConnection)
//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
{
  CDebug db(m_pObject->m_bDebug);

  db << "IOleObject::Unadvise(" << dwConnection << ") Called";
  db.DebugOutput();

  HRESULT hRes = ResultFromScode(E_NOTIMPL);
  if (m_pObject->m_pOleAdviseHolder != NULL) {
	hRes = m_pObject->m_pOleAdviseHolder->Unadvise(dwConnection);
  }

  db << "IOleObject::Unadvise() Returning " << hRes ;
  db.DebugOutput();

  return hRes;
} /* Unadvise()
   */


STDMETHODIMP NC(CXObject, COleObject)::
  EnumAdvise(LPENUMSTATDATA FAR* ppenumAdvise)
//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
{
  CDebug db(m_pObject->m_bDebug);

  db << "IOleObject::EnumAdvise(" << (LPUNKNOWN)ppenumAdvise << ") Called";
  db.DebugOutput();

  HRESULT hRes = ResultFromScode(E_NOTIMPL);
  if (m_pObject->m_pOleAdviseHolder != NULL) {
	hRes = m_pObject->m_pOleAdviseHolder->EnumAdvise(ppenumAdvise);
  }

  if (hRes == NOERROR) {
	db << "IOleObject::EnumAdvise(" << (LPUNKNOWN)*ppenumAdvise
	   << ") Returning " << hRes ;

  }else{
	db << "IOleObject::EnumAdvise() Returning " << hRes ;
  }

  db.DebugOutput();

  return hRes;
} /* EnumAdvise()
   */


STDMETHODIMP NC(CXObject, COleObject)::
  GetMiscStatus(DWORD dwAspect, DWORD FAR* pdwStatus)
//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
{
  CDebug db(m_pObject->m_bDebug);

  db << "IOleObject::GetMiscStatus(" << dwAspect << (LPUNKNOWN)pdwStatus
	 << ") Called";
  db.DebugOutput();

  HRESULT hRes = OleRegGetMiscStatus(m_pObject->m_ClsId, dwAspect, pdwStatus);

  if (hRes == NOERROR) {
	db << "IOleObject::GetMiscStatus(" << dwAspect << *pdwStatus
	   << ") Returning " << hRes ;
  }else{
	db << "IOleObject::GetMiscStatus() Returning " << hRes ;
  }

  db.DebugOutput();

  return hRes;
} /* GetMiscStatus()
   */


STDMETHODIMP NC(CXObject, COleObject)::
  SetColorScheme(LPLOGPALETTE lpLogpal)
//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
{
  CDebug db(m_pObject->m_bDebug);

  db << "IOleObject::SetColorScheme(" << (LPUNKNOWN)lpLogpal << ") Called";
  db.DebugOutput();

  HRESULT hRes = ResultFromScode(E_NOTIMPL);

  db << "IOleObject::SetColorScheme() Returning " << hRes ;
  db.DebugOutput();

  return hRes;
} /* SetColorScheme()
   */


STDMETHODIMP NC(CXObject, CDataObject)::
  QueryInterface(REFIID riid, LPVOID FAR* ppvObj)
//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
{
  return m_pObject->m_pUnkOuter->QueryInterface(riid, ppvObj);
} /* QueryInterface()
   */


STDMETHODIMP_(ULONG) NC(CXObject, CDataObject)::
  AddRef(VOID)
//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
{
  return m_pObject->m_pUnkOuter->AddRef();
} /* AddRef()
   */


STDMETHODIMP_(ULONG) NC(CXObject, CDataObject)::
  Release(VOID)
//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
{
  return m_pObject->m_pUnkOuter->Release();
} /* Release()
   */


STDMETHODIMP NC(CXObject, CDataObject)::
  GetData(LPFORMATETC pformatetcIn, LPSTGMEDIUM pmedium)
//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
{
  CDebug db(m_pObject->m_bDebug);

  db << "IDataObject::GetData(" << pformatetcIn << pmedium << ") Called";
  db.DebugOutput();

  HRESULT hRes = m_pObject->GetData(pformatetcIn, pmedium);

  db << "IDataObject::GetData(" << pformatetcIn << pmedium << ") Returning "
	 << hRes;
  db.DebugOutput();

  return hRes;
} /* GetData()
   */


STDMETHODIMP NC(CXObject, CDataObject)::
  GetDataHere(LPFORMATETC pformatetcIn, LPSTGMEDIUM pmedium)
//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
{
  CDebug db(m_pObject->m_bDebug);

  db << "IDataObject::GetDataHere(" << pformatetcIn << pmedium << ") Called";
  db.DebugOutput();

  HRESULT hRes = ResultFromScode(DV_E_FORMATETC);

  db << "IDataObject::GetDataHere(" << pformatetcIn << pmedium
	 << ") Returning " << hRes;
  db.DebugOutput();

  return hRes;
} /* GetDataHere()
   */


STDMETHODIMP NC(CXObject, CDataObject)::
  QueryGetData(LPFORMATETC pformatetcIn)
//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
{
  CDebug db(m_pObject->m_bDebug);

  db << "IDataObject::QueryGetData(" << pformatetcIn << ") Called";
  db.DebugOutput();

  HRESULT hRes = m_pObject->QueryGetData(pformatetcIn);

  db << "IDataObject::QueryGetData(" << pformatetcIn << ") Returning " << hRes;
  db.DebugOutput();

  return hRes;
} /* QueryGetData()
   */


STDMETHODIMP NC(CXObject, CDataObject)::
  GetCanonicalFormatEtc(LPFORMATETC petcIn, LPFORMATETC petcOut)
//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
{
  CDebug db(m_pObject->m_bDebug);

  db << "IDataObject::GetCanonicalFormatEtc(" << petcIn << petcOut
	 << ") Called";
  db.DebugOutput();

  HRESULT hRes = m_pObject->GetCanonicalFormatEtc(petcIn, petcOut);

  db << "IDataObject::GetCanonicalFormatEtc(" << petcIn << petcOut
	 << ") Returning " << hRes;
  db.DebugOutput();

  return hRes;
} /* GetCanonicalFormatEtc()
   */


STDMETHODIMP NC(CXObject, CDataObject)::
  SetData(LPFORMATETC pformatetcIn, LPSTGMEDIUM pmedium, BOOL fRelease)
//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
{
  CDebug db(m_pObject->m_bDebug);

  db << "IDataObject::SetData(" << pformatetcIn << pmedium << fRelease
	 <<") Called";
  db.DebugOutput();

  HRESULT hRes = ResultFromScode(E_NOTIMPL);

  db << "IDataObject::SetData(" << pformatetcIn << pmedium << fRelease
	 << ") Returning " << hRes;
  db.DebugOutput();

  return hRes;
} /* SetData()
   */


STDMETHODIMP NC(CXObject, CDataObject)::
  EnumFormatEtc(DWORD dwDirection, LPENUMFORMATETC FAR* ppenum)
//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
{
  CDebug db(m_pObject->m_bDebug);

  db << "IDataObject::EnumFormatEtc(" << dwDirection << (LPUNKNOWN)ppenum
	 << ") Called";
  db.DebugOutput();

  HRESULT hRes = OleRegEnumFormatEtc(m_pObject->m_ClsId, dwDirection, ppenum);

  if (hRes == NOERROR) {
	db << "IDataObject::EnumFormatEtc(" << dwDirection << (LPUNKNOWN)*ppenum
	   << ") Returning " << hRes;
  }else{
	db << "IDataObject::EnumFormatEtc() Returning " << hRes;
  }

  db.DebugOutput();

  return hRes;
} /* EnumFormatEtc()
   */


STDMETHODIMP NC(CXObject, CDataObject)::
  DAdvise(LPFORMATETC pFormatetc, DWORD advf, LPADVISESINK pAdvSink,
		  LPDWORD pdwConnection)
//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
{
  CDebug db(m_pObject->m_bDebug);

  db << "IDataObject::DAdvise(" << pFormatetc << advf << (LPUNKNOWN)pAdvSink
	 << (LPUNKNOWN)pdwConnection << ") Called";
  db.DebugOutput();

  HRESULT hRes = m_pObject->DAdvise(pFormatetc, advf,pAdvSink, pdwConnection);

  if (hRes == NOERROR) {
	db << "IDataObject::DAdvise(" << pFormatetc << advf << (LPUNKNOWN)pAdvSink
	   << *pdwConnection << ") Returning " << hRes;
  }else{
	db << "IDataObject::DAdvise() Returning " << hRes;
  }

  db.DebugOutput();

  return hRes;
} /* DAdvise()
   */


STDMETHODIMP NC(CXObject, CDataObject)::
  DUnadvise(DWORD dwConnection)
//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
{
  CDebug db(m_pObject->m_bDebug);

  db << "IDataObject::DUnadvise(" << dwConnection << ") Called";
  db.DebugOutput();

  HRESULT hRes = ResultFromScode(E_NOTIMPL);
  if (m_pObject->m_pDataAdviseHolder != NULL) {
	hRes = m_pObject->m_pDataAdviseHolder->Unadvise(dwConnection);
  }

  db << "IDataObject::DUnadvise() Returning " << hRes;
  db.DebugOutput();

  return hRes;
} /* DUnadvise()
   */


STDMETHODIMP NC(CXObject, CDataObject)::
  EnumDAdvise(LPENUMSTATDATA FAR* ppenumAdvise)
//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
{
  CDebug db(m_pObject->m_bDebug);

  db << "IDataObject::EnumDAdvise(" << (LPUNKNOWN)ppenumAdvise << ") Called";
  db.DebugOutput();

  HRESULT hRes = ResultFromScode(E_NOTIMPL);
  if (m_pObject->m_pDataAdviseHolder != NULL) {
	hRes = m_pObject->m_pDataAdviseHolder->EnumAdvise(ppenumAdvise);
  }

  if (hRes == NOERROR) {
	db << "IDataObject::EnumDAdvise(" << (LPUNKNOWN)*ppenumAdvise
	   << ") Returning " << hRes;
  }else{
	db << "IDataObject::EnumDAdvise() Returning " << hRes;
  }

  db.DebugOutput();

  return hRes;
} /* EnumDAdvise()
   */


STDMETHODIMP NC(CXObject, CViewObject2)::
  QueryInterface(REFIID riid, LPVOID FAR* ppvObj)
//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
{
  return m_pObject->m_pUnkOuter->QueryInterface(riid, ppvObj);
} /* QueryInterface()
   */


STDMETHODIMP_(ULONG) NC(CXObject, CViewObject2)::
  AddRef(VOID)
//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
{
  return m_pObject->m_pUnkOuter->AddRef();
} /* AddRef()
   */


STDMETHODIMP_(ULONG) NC(CXObject, CViewObject2)::
  Release(VOID)
//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
{
  return m_pObject->m_pUnkOuter->Release();
} /* Release()
   */


STDMETHODIMP NC(CXObject, CViewObject2)::
  Draw(DWORD dwDrawAspect, LONG lindex, void FAR* pvAspect,
	   DVTARGETDEVICE FAR * ptd, HDC hicTargetDev, HDC hdcDraw,
	   LPCRECTL lprcBounds, LPCRECTL lprcWBounds,
	   BOOL (CALLBACK * pfnContinue) (DWORD), DWORD dwContinue)
//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
{
  CDebug db(m_pObject->m_bDebug);

  db << "IViewObject2::Draw(" << dwDrawAspect << lindex << (LPUNKNOWN)pvAspect
	 << (LPUNKNOWN)ptd << (WORD)hicTargetDev << (WORD)hdcDraw
	 << lprcBounds << lprcWBounds << (LPUNKNOWN)pfnContinue << dwContinue
	 << ") Called" ;
  db.DebugOutput();

  HRESULT hRes = m_pObject->Draw(dwDrawAspect, lindex, pvAspect, ptd,
								 hicTargetDev, hdcDraw, lprcBounds,
								 lprcWBounds, pfnContinue, dwContinue);

  db << "IViewObject2::Draw() Returning " << hRes ;
  db.DebugOutput();

  return hRes;
} /* Draw()
   */


STDMETHODIMP NC(CXObject, CViewObject2)::
  GetColorSet(DWORD dwDrawAspect, LONG lindex, void FAR* pvAspect,
			  DVTARGETDEVICE FAR * ptd, HDC hicTargetDev,
			  LPLOGPALETTE FAR* ppColorSet)
//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
{
  CDebug db(m_pObject->m_bDebug);

  db << "IViewObject2::GetColorSet(" << dwDrawAspect << lindex
	 << (LPUNKNOWN)pvAspect << (LPUNKNOWN)ptd << (WORD) hicTargetDev
	 << (LPUNKNOWN)ppColorSet  << ") Called" ;
  db.DebugOutput();

  HRESULT hRes = ResultFromScode(S_FALSE);

  db << "IViewObject2::GetColorSet() Returning " << hRes ;
  db.DebugOutput();

  return hRes;
} /* GetColorSet()
   */


STDMETHODIMP NC(CXObject, CViewObject2)::
  Freeze(DWORD dwDrawAspect, LONG lindex, void FAR* pvAspect,
		 DWORD FAR* pdwFreeze)
//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
{
  CDebug db(m_pObject->m_bDebug);

  db << "IViewObject2::Freeze(" << dwDrawAspect << lindex << pvAspect
	 << (LPUNKNOWN)pdwFreeze  << ") Called" ;
  db.DebugOutput();

  HRESULT hRes = ResultFromScode(E_INVALIDARG);
  if (pdwFreeze != NULL) {
	*pdwFreeze = 0;
	hRes = NOERROR;
  }

  db << "IViewObject2::Freeze() Returning " << hRes ;
  db.DebugOutput();

  return hRes;
} /* Freeze()
   */


STDMETHODIMP NC(CXObject, CViewObject2)::
  Unfreeze(DWORD dwFreeze)
//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
{
  CDebug db(m_pObject->m_bDebug);

  db << "IViewObject2::Unfreeze(" << dwFreeze << ") Called" ;
  db.DebugOutput();

  HRESULT hRes = ResultFromScode(OLE_E_NOCONNECTION);
  if (dwFreeze == 0) {
	hRes = NOERROR;
  }

  db << "IViewObject2::Unfreeze() Returning " << hRes ;
  db.DebugOutput();

  return hRes;
} /* Unfreeze()
   */


STDMETHODIMP NC(CXObject, CViewObject2)::
  SetAdvise(DWORD aspects, DWORD advf, LPADVISESINK pAdvSink)
//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
{
  CDebug db(m_pObject->m_bDebug);

  db << "IViewObject2::SetAdvise(" << aspects << advf << (LPUNKNOWN) pAdvSink
	 << ") Called" ;
  db.DebugOutput();

  HRESULT hRes = ResultFromScode(E_INVALIDARG);

  if (advf & ADVF_NODATA) {
	goto errReturn;
  }

  if (m_pObject->m_pViewAdviseSink != NULL) {
	m_pObject->m_pViewAdviseSink->Release();
  }

  if (pAdvSink == NULL) {
	m_pObject->m_pViewAdviseSink = NULL;
	m_pObject->m_advf = NULL;
	m_pObject->m_dwAspects = NULL;

  }else{
	pAdvSink->AddRef();

	m_pObject->m_pViewAdviseSink = pAdvSink;
	m_pObject->m_advf = advf;
	m_pObject->m_dwAspects = aspects;

	if (advf & ADVF_PRIMEFIRST) {
	  m_pObject->SendViewChange(aspects);
	}
  }

  hRes = NOERROR;

errReturn:
  db << "IViewObject2::SetAdvise() Returning " << hRes ;
  db.DebugOutput();

  return hRes;
} /* SetAdvise()
   */


STDMETHODIMP NC(CXObject, CViewObject2)::
  GetAdvise(DWORD FAR* pAspects, DWORD FAR* pAdvf, LPADVISESINK FAR* ppAdvSink)
//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
{
  CDebug db(m_pObject->m_bDebug);

  db << "IViewObject2::GetAdvise(" << (LPUNKNOWN)pAspects << (LPUNKNOWN) pAdvf
	 << (LPUNKNOWN) ppAdvSink << ") Called" ;
  db.DebugOutput();

  HRESULT hRes = NOERROR ;

	if (m_pObject->m_pViewAdviseSink != NULL) {
		if (pAspects != NULL) {
			*pAspects = m_pObject->m_dwAspects;
	}

		if (pAdvf != NULL) {
			*pAdvf = m_pObject->m_advf;
	}

		if (ppAdvSink != NULL) {
			(*ppAdvSink = m_pObject->m_pViewAdviseSink)->AddRef();
	}

	}else{
		if (pAspects != NULL) {
			*pAspects = NULL;
	}

		if (pAdvf != NULL) {
			*pAdvf = NULL;
	}

		if (ppAdvSink != NULL) {
			*ppAdvSink = NULL;
	}
	}

  db << "IViewObject2::GetAdvise(" ;

  (pAspects == NULL) ? db << "NULL" : db << *pAspects ;
  (pAdvf == NULL) ? db << ", NULL" : db << *pAdvf;
  (ppAdvSink == NULL) ? db << ", NULL" : db << (LPUNKNOWN) *ppAdvSink;

  db << ") Returning " << hRes ;
  db.DebugOutput();

  return hRes;

} /* GetAdvise()
   */


STDMETHODIMP NC(CXObject, CViewObject2)::
  GetExtent(DWORD dwDrawAspect, LONG lindex, DVTARGETDEVICE FAR* ptd,
			LPSIZEL lpsizel)
//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
{
  CDebug db(m_pObject->m_bDebug);

  db << "IViewObject2::GetExtent(" << dwDrawAspect << lindex << ptd
	 << lpsizel << ") Called" ;
  db.DebugOutput();

  HRESULT hRes = m_pObject->GetExtent(dwDrawAspect, lindex, ptd, lpsizel);

  db << "IViewObject2::GetExtent(" << dwDrawAspect << lindex << ptd
	 << lpsizel << ") Returning " << hRes ;
  db.DebugOutput();

  return hRes;
} /* Freeze()
   */


STDMETHODIMP NC(CXObject, COleCache2)::
  QueryInterface(REFIID riid, LPVOID FAR* ppvObj)
//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
{
  return m_pObject->m_pUnkOuter->QueryInterface(riid, ppvObj);
} /* QueryInterface()
   */


STDMETHODIMP_(ULONG) NC(CXObject, COleCache2)::
  AddRef(VOID)
//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
{
  return m_pObject->m_pUnkOuter->AddRef();
} /* AddRef()
   */


STDMETHODIMP_(ULONG) NC(CXObject, COleCache2)::
  Release(VOID)
//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
{
  return m_pObject->m_pUnkOuter->Release();
} /* Release()
   */


STDMETHODIMP NC(CXObject, COleCache2)::
  Cache(LPFORMATETC petc, DWORD advf, LPDWORD lpdwConnection)
//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
{
  CDebug db(m_pObject->m_bDebug);

  db << "IOleCache2::Cache(" << petc << advf << (LPUNKNOWN)lpdwConnection
	 << ") Called" ;
  db.DebugOutput();

  HRESULT hRes = m_pObject->Cache(petc, advf, lpdwConnection);

  if (hRes == NOERROR) {
	db << "IOleCache2::Cache(" << petc << advf << *lpdwConnection
	   << ") Returning " << hRes ;
  }else{
	db << "IOleCache2::Cache() Returning " << hRes ;
  }

  db.DebugOutput();

  return hRes;
} /* Cache()
   */


STDMETHODIMP NC(CXObject, COleCache2)::
  Uncache(DWORD dwConnection)
//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
{
  CDebug db(m_pObject->m_bDebug);

  db << "IOleCache2::Uncache(" << dwConnection << ") Called" ;
  db.DebugOutput();

  HRESULT hRes = m_pObject->Uncache(dwConnection);

  db << "IOleCache2::Uncache() Returning " << hRes ;
  db.DebugOutput();

  return hRes;
} /* Uncache()
   */


STDMETHODIMP NC(CXObject, COleCache2)::
  EnumCache(LPENUMSTATDATA FAR* ppenumStatData)
//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
{
  CDebug db(m_pObject->m_bDebug);

  db << "IOleCache2::EnumCache(" << (LPUNKNOWN)ppenumStatData << ") Called" ;
  db.DebugOutput();

  HRESULT hRes = m_pObject->EnumCache(ppenumStatData);

  if (hRes == NOERROR) {
	db << "IOleCache2::EnumCache(" << (LPUNKNOWN)*ppenumStatData
	   << ") Returning " << hRes ;
  }else{
	db << "IOleCache2::EnumCache() Returning " << hRes ;
  }

  db.DebugOutput();

  return hRes;
} /* EnumCache()
   */


STDMETHODIMP NC(CXObject, COleCache2)::
  InitCache(LPDATAOBJECT pDataObject)
//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
{
  CDebug db(m_pObject->m_bDebug);

  db << "IOleCache2::InitCache(" << (LPUNKNOWN)pDataObject << ") Called" ;
  db.DebugOutput();

  HRESULT hRes = ResultFromScode(E_NOTIMPL);
  if (m_pObject->m_pOleCache2 != NULL) {
	hRes = m_pObject->m_pOleCache2->InitCache(pDataObject);
  }

  db << "IOleCache2::InitCache() Returning " << hRes ;
  db.DebugOutput();

  return hRes;
} /* InitCache()
   */


STDMETHODIMP NC(CXObject, COleCache2)::
  SetData(LPFORMATETC pformatetc, STGMEDIUM FAR * pmedium, BOOL fRelease)
//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
{
  CDebug db(m_pObject->m_bDebug);

  db << "IOleCache2::SetData(" << pformatetc << pmedium << fRelease
	 << ") Called" ;
  db.DebugOutput();

  HRESULT hRes = m_pObject->SetData(pformatetc, pmedium, fRelease);

  db << "IOleCache2::SetData() Returning " << hRes ;
  db.DebugOutput();

  return hRes;
} /* SetData()
   */


STDMETHODIMP NC(CXObject, COleCache2)::
  UpdateCache(LPDATAOBJECT pDataObject, DWORD grfUpdf, LPVOID pReserved)
//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
{
  CDebug db(m_pObject->m_bDebug);

  db << "IOleCache2::UpdateCache(" << (LPUNKNOWN)pDataObject << grfUpdf
	 << (LPUNKNOWN)pReserved << ") Called" ;
  db.DebugOutput();

  HRESULT hRes = ResultFromScode(E_NOTIMPL);
  if (m_pObject->m_pOleCache2 != NULL) {
	hRes = m_pObject->m_pOleCache2->UpdateCache(pDataObject, grfUpdf,
											   pReserved);
  }

  db << "IOleCache2::UpdateCache() Returning " << hRes ;
  db.DebugOutput();

  return hRes;
} /* UpdateCache()
   */


STDMETHODIMP NC(CXObject, COleCache2)::
  DiscardCache(DWORD dwDiscardOptions)
//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
{
  CDebug db(m_pObject->m_bDebug);

  db << "IOleCache2::DiscardCache(" << dwDiscardOptions << ") Called" ;
  db.DebugOutput();

  HRESULT hRes = ResultFromScode(E_NOTIMPL);
  if (m_pObject->m_pOleCache2 != NULL) {
	hRes = m_pObject->m_pOleCache2->DiscardCache(dwDiscardOptions);
  }

  db << "IOleCache2::DiscardCache() Returning " << hRes ;
  db.DebugOutput();

  return hRes;
} /* DiscardCache()
   */


STDMETHODIMP NC(CXObject, CRunnableObject)::
  QueryInterface(REFIID riid, LPVOID FAR* ppvObj)
//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
{
  return m_pObject->m_pUnkOuter->QueryInterface(riid, ppvObj);
} /* QueryInterface()
   */


STDMETHODIMP_(ULONG) NC(CXObject, CRunnableObject)::
  AddRef(VOID)
//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
{
  return m_pObject->m_pUnkOuter->AddRef();
} /* AddRef()
   */


STDMETHODIMP_(ULONG) NC(CXObject, CRunnableObject)::
  Release(VOID)
//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
{
  return m_pObject->m_pUnkOuter->Release();
} /* Release()
   */


STDMETHODIMP NC(CXObject, CRunnableObject)::
  GetRunningClass(LPCLSID lpClsid)
//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
{
  CDebug db(m_pObject->m_bDebug);

  db << "IRunnableObject::GetRunningClass(" << (LPUNKNOWN)lpClsid
	 << ") Called" ;
  db.DebugOutput();

  HRESULT hRes = ResultFromScode(E_NOTIMPL);
  if (lpClsid != NULL) {
	*lpClsid = m_pObject->m_ClsId;
  }

  db << "IRunnableObject::GetRunningClass(" << lpClsid << ") Returning "
	 << hRes ;
  db.DebugOutput();

  return hRes;
} /* GetRunningClass()
   */


STDMETHODIMP NC(CXObject, CRunnableObject)::
  Run(LPBINDCTX pbc)
//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
{
  CDebug db(m_pObject->m_bDebug);

  db << "IRunnableObject::Run(" << (LPUNKNOWN)pbc << ") Called" ;
  db.DebugOutput();

  HRESULT hRes = m_pObject->SetRunningState(TRUE);

  db << "IRunnableObject::Run() Returning " << hRes ;
  db.DebugOutput();

  return hRes;
} /* Run()
   */


STDMETHODIMP_(BOOL) NC(CXObject, CRunnableObject)::
  IsRunning()
//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
{
  CDebug db(m_pObject->m_bDebug);

  db << "IRunnableObject::IsRunning() Called" ;
  db.DebugOutput();

  BOOL bIsRunning = m_pObject->m_bIsRunning;

  db << "IRunnableObject::IsRunning() Returning " << bIsRunning ;
  db.DebugOutput();

  return bIsRunning;
} /* IsRunning()
   */


STDMETHODIMP NC(CXObject, CRunnableObject)::
  LockRunning(BOOL fLock, BOOL fLastUnlockCloses)
//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
{
  CDebug db(m_pObject->m_bDebug);

  db << "IRunnableObject::LockRunning(" << fLock << fLastUnlockCloses
	 << ") Called" ;
  db.DebugOutput();

  HRESULT hRes = CoLockObjectExternal(this, fLock, fLastUnlockCloses);

  db << "IRunnableObject::LockRunning() Returning " << hRes ;
  db.DebugOutput();

  return hRes;
} /* LockRunning()
   */


STDMETHODIMP NC(CXObject, CRunnableObject)::
  SetContainedObject(BOOL fContained)
//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
{
  CDebug db(m_pObject->m_bDebug);

  db << "IRunnableObject::SetContainedObject(" << fContained << ") Called" ;
  db.DebugOutput();

  HRESULT hRes = NOERROR;

  db << "IRunnableObject::SetContainedObject() Returning " << hRes ;
  db.DebugOutput();

  return hRes;
} /* SetContainedObject()
   */


STDMETHODIMP NC(CXObject, CExternalConnection)::
  QueryInterface(REFIID riid, LPVOID FAR* ppvObj)
//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
{
  return m_pObject->m_pUnkOuter->QueryInterface(riid, ppvObj);
} /* QueryInterface()
   */


STDMETHODIMP_(ULONG) NC(CXObject, CExternalConnection)::
  AddRef(VOID)
//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
{
  return m_pObject->m_pUnkOuter->AddRef();
} /* AddRef()
   */


STDMETHODIMP_(ULONG) NC(CXObject, CExternalConnection)::
  Release(VOID)
//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
{
  return m_pObject->m_pUnkOuter->Release();
} /* Release()
   */


STDMETHODIMP_(DWORD) NC(CXObject, CExternalConnection)::
  AddConnection(DWORD extconn, DWORD reserved)
//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
{
  CDebug db(m_pObject->m_bDebug);

  db << "IExternalConnection::AddConnection(" << extconn << reserved
	 << ") Called" ;
  db.DebugOutput();

  DWORD dwCount = (extconn & EXTCONN_STRONG)? ++m_pObject->m_dwStrong : 0;

  db << "IExternalConnection::AddConnection() Returning " << dwCount ;
  db.DebugOutput();

  return dwCount;
} /* AddConnection()
   */


STDMETHODIMP_(DWORD) NC(CXObject, CExternalConnection)::
  ReleaseConnection(DWORD extconn, DWORD reserved, BOOL fLastReleaseCloses)
//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
{
  CDebug db(m_pObject->m_bDebug);

  db << "IExternalConnection::ReleaseConnection(" << extconn << reserved
	 << fLastReleaseCloses << ") Called" ;
  db.DebugOutput();

  DWORD dwCount = 0 ;

  if (extconn & EXTCONN_STRONG) {
	dwCount = -- m_pObject->m_dwStrong;

	if ((dwCount == 0) && fLastReleaseCloses) {
	  m_pObject->m_OleObject.Close(OLECLOSE_SAVEIFDIRTY);
	}
  }

  db << "IExternalConnection::ReleaseConnection() Returning " << dwCount ;
  db.DebugOutput();

  return dwCount;
} /* ReleaseConnection()
   */


STDMETHODIMP NC(CXObject, CStdMarshalInfo)::
  QueryInterface(REFIID riid, LPVOID FAR* ppvObj)
//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
{
  return m_pObject->m_pUnkOuter->QueryInterface(riid, ppvObj);
} /* QueryInterface()
   */


STDMETHODIMP_(ULONG) NC(CXObject, CStdMarshalInfo)::
  AddRef(VOID)
//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
{
  return m_pObject->m_pUnkOuter->AddRef();
} /* AddRef()
   */


STDMETHODIMP_(ULONG) NC(CXObject, CStdMarshalInfo)::
  Release(VOID)
//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
{
  return m_pObject->m_pUnkOuter->Release();
} /* Release()
   */


STDMETHODIMP NC(CXObject, CStdMarshalInfo)::
  GetClassForHandler(DWORD dwDestContext, LPVOID pvDestContext, LPCLSID pClsid)
//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
{
  CDebug db(m_pObject->m_bDebug);

  db << "IStdMarshalInfo::GetClassForHandler(" << dwDestContext
	 << (LPUNKNOWN)pvDestContext << (LPUNKNOWN)pClsid << ") Called" ;
  db.DebugOutput();

  HRESULT hRes = ResultFromScode(E_INVALIDARG);

  if (pClsid != NULL) {
	*pClsid = m_pObject->m_ClsId;
	hRes = NOERROR;
  }

  db << "IStdMarshalInfo::GetClassForHandler(" << dwDestContext
	 << (LPUNKNOWN)pvDestContext << pClsid << ") Returning " << hRes ;
  db.DebugOutput();

  return hRes;
} /* GetClassForHandler()
   */
