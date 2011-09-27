//----------------------------------------------------------------------------
//
// (c) Copyright Microsoft Corp. 1992 All Rights Reserved
//
//  module: dbghndlr.cpp
//  date:
//  author:
//
//  purpose:
//
//----------------------------------------------------------------------------
#include "stdafx.h"


CHandlerObject::CHandlerObject(LPUNKNOWN pUnkOuter)
 : m_Unknown(this), m_PersistStorage(this), m_ViewObject2(this),
   m_OleObject(this), m_DataObject(this), m_OleCache2(this),
   m_RunnableObject(this), m_ExternalConnection(this), m_StdMarshalInfo(this)
//----------------------------------------------------------------------------
// Constructor:
//----------------------------------------------------------------------------
{
  m_dwRefs = 1;
  m_pStg = NULL;
  m_pDefHandler = NULL;
  m_pPersistStorage = NULL;
  m_pOleObject = NULL;
  m_pDataObject = NULL;
  m_pViewObject2 = NULL;
  m_pOleCache2 = NULL;
  m_pRunnableObject = NULL;
  m_pExternalConnection = NULL;
  m_pStdMarshalInfo = NULL;

  GetTempFileName(0, "OLE", 0, m_szFileName);

  CDebug::SetLogFileName(m_szFileName);

  m_pHandlerWindow = new CHandlerWindow(m_szFileName);
  m_pHandlerWindow->ShowWindow(SW_SHOWMINNOACTIVE);

  m_pListBox = m_pHandlerWindow->GetListBox();

  if (pUnkOuter == NULL) {
	m_pUnkOuter = &m_Unknown;
  }else{
	m_pUnkOuter = pUnkOuter;
  }

} /* CHandlerObject::CHandlerObject()
   */


CHandlerObject::~CHandlerObject()
//----------------------------------------------------------------------------
// Destructor:
//----------------------------------------------------------------------------
{
  ULONG dwRefs;

  if (m_pStg != NULL) {
	dwRefs = m_pStg->Release();
	m_pStg = NULL;
  }

  if (m_pDefHandler != NULL) {
	dwRefs = m_pDefHandler->Release();
	m_pDefHandler = NULL;
  }

  CDebug db;
  db << "***** \"And I'm outta here!\" *****" ;
  db.DebugOutput(NULL);

  CDebug::CloseLogFile();
  m_pHandlerWindow->DestroyWindow();

} /* CHandlerObject::~CHandlerObject()
   */


HRESULT CHandlerObject::
  CreateObject(REFCLSID rclsid, REFIID riid, LPVOID FAR* ppv)
//----------------------------------------------------------------------------
// CreateObject:
//----------------------------------------------------------------------------
{
  HRESULT hRes = ResultFromScode(E_FAIL);
  ULONG dwRefs;

  m_ClsId = rclsid;

  hRes = OleCreateDefaultHandler(m_ClsId, &m_Unknown, IID_IUnknown,
								 (LPVOID FAR*) &m_pDefHandler);
  if (hRes != NOERROR) {
	return hRes;
  }

  hRes = m_pDefHandler->QueryInterface(IID_IOleObject,
									   (LPVOID FAR*)&m_pOleObject);
  if (hRes == NOERROR) {
	dwRefs = m_pOleObject->Release();
  }

  hRes = m_pDefHandler->QueryInterface(IID_IDataObject,
									   (LPVOID FAR*)&m_pDataObject);
  if (hRes == NOERROR) {
	dwRefs = m_pDataObject->Release();
  }

  hRes = m_pDefHandler->QueryInterface(IID_IViewObject2,
									   (LPVOID FAR*)&m_pViewObject2);
  if (hRes == NOERROR) {
	dwRefs = m_pViewObject2->Release();
  }

  hRes = m_pDefHandler->QueryInterface(IID_IOleCache2,
									   (LPVOID FAR*)&m_pOleCache2);
  if (hRes == NOERROR) {
	dwRefs = m_pOleCache2->Release();
  }

  hRes = m_pDefHandler->QueryInterface(IID_IPersistStorage,
									   (LPVOID FAR*)&m_pPersistStorage);
  if (hRes == NOERROR) {
	dwRefs = m_pPersistStorage->Release();
  }

  hRes = m_pDefHandler->QueryInterface(IID_IRunnableObject,
									   (LPVOID FAR*)&m_pRunnableObject);
  if (hRes == NOERROR) {
	dwRefs = m_pRunnableObject->Release();
  }

  hRes = m_pDefHandler->QueryInterface(IID_IExternalConnection,
									   (LPVOID FAR*)&m_pExternalConnection);
  if (hRes == NOERROR) {
	dwRefs = m_pExternalConnection->Release();
  }

  hRes = m_pDefHandler->QueryInterface(IID_IStdMarshalInfo,
									   (LPVOID FAR*)&m_pStdMarshalInfo);
  if (hRes == NOERROR) {
	dwRefs = m_pStdMarshalInfo->Release();
  }

  if ((hRes =  m_Unknown.QueryInterface(riid, ppv)) == NOERROR) {
	ULONG dwRefs = ((LPUNKNOWN)*ppv)->Release();
  }

  return hRes;

} /* CreateObject()
   */


STDMETHODIMP NC(CHandlerObject, CUnknown)::
  QueryInterface(REFIID riid, LPVOID FAR* ppobj)
//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
{
  CDebug db;

  db << "IUnknown::QueryInterface(" << riid << (LPUNKNOWN)ppobj << ") Called" ;
  db.DebugOutput(m_pObject->m_pListBox);

  HRESULT hRes=NOERROR;
  BOOL bSupport=TRUE;

  *ppobj = NULL;

  if (riid == IID_IUnknown) {
	*ppobj = (LPVOID)&m_pObject->m_Unknown;

  }else if (riid == IID_IPersistStorage || riid == IID_IPersist) {
	*ppobj = (LPVOID)&m_pObject->m_PersistStorage;

  }else if (riid == IID_IOleObject) {
	*ppobj = (LPVOID)&m_pObject->m_OleObject;

  }else if (riid == IID_IDataObject) {
	*ppobj = (LPVOID)&m_pObject->m_DataObject;

  }else if (riid == IID_IViewObject || riid == IID_IViewObject2) {
	*ppobj = (LPVOID)&m_pObject->m_ViewObject2;

  }else if (riid == IID_IOleCache || riid == IID_IOleCache2) {
	*ppobj = (LPVOID)&m_pObject->m_OleCache2;

  }else if (riid == IID_IRunnableObject) {
	*ppobj = (LPVOID)&m_pObject->m_RunnableObject;

  }else if (riid == IID_IExternalConnection) {
	*ppobj = (LPVOID)&m_pObject->m_ExternalConnection;

  }else if (riid == IID_IStdMarshalInfo) {
	*ppobj = (LPVOID)&m_pObject->m_StdMarshalInfo;

  }else if (m_pObject->m_pDefHandler != NULL) {
	hRes = m_pObject->m_pDefHandler->QueryInterface(riid, ppobj);
	bSupport = FALSE;
  }

  if (bSupport) {
	if (*ppobj == NULL) {
	  hRes = ResultFromScode(E_NOINTERFACE);
	}else{
	  ((LPUNKNOWN)*ppobj)->AddRef();
	}
  }

  db << "IUnknown::QueryInterface(" << riid << (LPUNKNOWN)*ppobj
	 << ") Returning " << hRes ;
  db.DebugOutput(m_pObject->m_pListBox);

  return hRes;
} /* QueryInterface()
   */


STDMETHODIMP_(ULONG) NC(CHandlerObject, CUnknown)::
  AddRef(VOID)
//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
{
  CDebug db;

  db << "IUnknown::AddRef()  Called";
  db.DebugOutput(m_pObject->m_pListBox);

  ULONG dwRefs = ++m_pObject->m_dwRefs;

  db << "IUnknown::AddRef()  Returning " << dwRefs;
  db.DebugOutput(m_pObject->m_pListBox);

  return dwRefs;

} /* AddRef()
   */


STDMETHODIMP_(ULONG) NC(CHandlerObject, CUnknown)::
  Release(VOID)
//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
{
  CDebug db;

  db << "IUnknown::Release() Called";
  db.DebugOutput(m_pObject->m_pListBox);

  ULONG dwRefs = --m_pObject->m_dwRefs;
  CListBox* pListBox = m_pObject->m_pListBox;

  if (dwRefs == 0) {
	delete m_pObject;
  }

  db << "IUnknown::Release() Returning " << dwRefs;
  db.DebugOutput(pListBox);
  return dwRefs;

} /* Release()
   */


STDMETHODIMP NC(CHandlerObject, CPersistStorage)::
  GetClassID(LPCLSID lpClassID)
//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
{
  CDebug db;

  db << "IPersist::GetClassID() Called";
  db.DebugOutput(m_pObject->m_pListBox);

  HRESULT hRes = ResultFromScode(E_NOTIMPL);

  if (m_pObject->m_pPersistStorage != NULL) {
	hRes = m_pObject->m_pPersistStorage->GetClassID(lpClassID);
  }

  db << "IPersist::GetClassID(" << *lpClassID << ") Returning "
	 << hRes;
  db.DebugOutput(m_pObject->m_pListBox);

  return hRes;
} /* GetClassID()
   */


STDMETHODIMP NC(CHandlerObject, CPersistStorage)::
  IsDirty(VOID)
//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
{
  CDebug db;

  db << "IPersistStorage::IsDirty() Called";
  db.DebugOutput(m_pObject->m_pListBox);

  HRESULT hRes = ResultFromScode(E_NOTIMPL);

  if (m_pObject->m_pPersistStorage != NULL) {
	hRes = m_pObject->m_pPersistStorage->IsDirty();
  }

  db << "IPersistStorage::IsDirty() Returning " << hRes;
  db.DebugOutput(m_pObject->m_pListBox);

  return hRes;

} /* IsDirty()
   */


STDMETHODIMP NC(CHandlerObject, CPersistStorage)::
  InitNew(LPSTORAGE pStg)
//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
{
  CDebug db;

  db << "IPersistStorage::InitNew(" << (LPUNKNOWN)pStg << ") Called";
  db.DebugOutput(m_pObject->m_pListBox);

  HRESULT hRes = ResultFromScode(E_NOTIMPL);

  ULONG dwRefs;

  if (pStg != NULL) {
	m_pObject->m_pStg = pStg;
	dwRefs = m_pObject->m_pStg->AddRef();
  }

  if (m_pObject->m_pPersistStorage != NULL) {
	hRes = m_pObject->m_pPersistStorage->InitNew(pStg);
  }

  db << "IPersistStorage::InitNew() Returning " << hRes ;
  db.DebugOutput(m_pObject->m_pListBox);

  return hRes;
} /* InitNew()
   */


STDMETHODIMP NC(CHandlerObject, CPersistStorage)::
  Load(LPSTORAGE pStg)
//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
{
  CDebug db;

  db << "IPersistStorage::Load(" << (LPUNKNOWN)pStg << ") Called";
  db.DebugOutput(m_pObject->m_pListBox);

  HRESULT hRes = ResultFromScode(E_NOTIMPL);

  ULONG dwRefs;

  if (pStg != NULL) {
	m_pObject->m_pStg = pStg;
	dwRefs = m_pObject->m_pStg->AddRef();
  }

  if (m_pObject->m_pPersistStorage != NULL) {
	hRes = m_pObject->m_pPersistStorage->Load(pStg);
  }

  db << "IPersistStorage::Load() Returning " << hRes ;
  db.DebugOutput(m_pObject->m_pListBox);

  return hRes;
} /* Load()
   */


STDMETHODIMP NC(CHandlerObject, CPersistStorage)::
  Save(LPSTORAGE pStgSave, BOOL fSameAsLoad)
//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
{
  CDebug db;

  db << "IPersistStorage::Save(" << (LPUNKNOWN)pStgSave << fSameAsLoad
	 << ") Called";
  db.DebugOutput(m_pObject->m_pListBox);

  HRESULT hRes = ResultFromScode(E_NOTIMPL);

  if (m_pObject->m_pPersistStorage != NULL) {
	hRes = m_pObject->m_pPersistStorage->Save(pStgSave, fSameAsLoad);
  }

  db << "IPersistStorage::Save() Returning " << hRes ;
  db.DebugOutput(m_pObject->m_pListBox);

  return hRes;
} /* Save()
   */


STDMETHODIMP NC(CHandlerObject, CPersistStorage)::
  SaveCompleted(LPSTORAGE pStgNew)
//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
{
  CDebug db;

  db << "IPersistStorage::SaveCompleted(" << (LPUNKNOWN)pStgNew <<") Called";
  db.DebugOutput(m_pObject->m_pListBox);

  HRESULT hRes = ResultFromScode(E_NOTIMPL);

  ULONG dwRefs;

  if (pStgNew != NULL) {
	if (m_pObject->m_pStg != NULL) {
	  dwRefs = m_pObject->m_pStg->Release();
	}
	m_pObject->m_pStg = pStgNew;
	dwRefs = m_pObject->m_pStg->AddRef();
  }

  if (m_pObject->m_pPersistStorage != NULL) {
	hRes = m_pObject->m_pPersistStorage->SaveCompleted(pStgNew);
  }

  db << "IPersistStorage::SaveCompleted() Returning " << hRes ;
  db.DebugOutput(m_pObject->m_pListBox);

  return hRes;
} /* SaveCompleted()
   */


STDMETHODIMP NC(CHandlerObject, CPersistStorage)::
  HandsOffStorage(VOID)
//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
{
  CDebug db;

  db << "IPersistStorage::HandsOffStorage() Called" ;
  db.DebugOutput(m_pObject->m_pListBox);

  HRESULT hRes = ResultFromScode(E_NOTIMPL);

  if (m_pObject->m_pStg != NULL) {
	ULONG dwRefs = m_pObject->m_pStg->Release();
	m_pObject->m_pStg = NULL;
  }

  if (m_pObject->m_pPersistStorage != NULL) {
	hRes = m_pObject->m_pPersistStorage->HandsOffStorage();
  }

  db << "IPersistStorage::HandsOffStorage() Returning " << hRes ;
  db.DebugOutput(m_pObject->m_pListBox);

  return hRes;
} /* HandsOffStorage()
   */


STDMETHODIMP NC(CHandlerObject, COleObject)::
  SetClientSite(LPOLECLIENTSITE pClientSite)
//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
{
  CDebug db;

  db << "IOleObject::SetClientSite(" << (LPUNKNOWN)pClientSite << ") Called";
  db.DebugOutput(m_pObject->m_pListBox);

  HRESULT hRes = ResultFromScode(E_NOTIMPL);

  if (m_pObject->m_pOleObject != NULL) {
	hRes = m_pObject->m_pOleObject->SetClientSite(pClientSite);
  }

  db << "IOleObject::SetClientSite() Returning " << hRes ;
  db.DebugOutput(m_pObject->m_pListBox);

  return hRes;
} /* SetClientSite()
   */


STDMETHODIMP NC(CHandlerObject, COleObject)::
  GetClientSite(LPOLECLIENTSITE FAR* ppClientSite)
//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
{
  CDebug db;

  db << "IOleObject::GetClientSite(" << (LPUNKNOWN)ppClientSite << ") Called";
  db.DebugOutput(m_pObject->m_pListBox);

  HRESULT hRes = ResultFromScode(E_NOTIMPL);

  if (m_pObject->m_pOleObject != NULL) {
	hRes = m_pObject->m_pOleObject->GetClientSite(ppClientSite);
  }

  db << "IOleObject::GetClientSite(" << *ppClientSite << ") Returning "
	 << hRes ;
  db.DebugOutput(m_pObject->m_pListBox);

  return hRes;
} /* GetClientSite()
   */


STDMETHODIMP NC(CHandlerObject, COleObject)::
  SetHostNames(LPCSTR szContainerApp, LPCSTR szContainerObj)
//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
{
  CDebug db;

  db << "IOleObject::SetHostNames(" << szContainerApp << ", "
	 << szContainerObj << ") Called";
  db.DebugOutput(m_pObject->m_pListBox);

  HRESULT hRes = ResultFromScode(E_NOTIMPL);

  if (m_pObject->m_pOleObject != NULL) {
	hRes = m_pObject->m_pOleObject->SetHostNames(szContainerApp,
												 szContainerObj);
  }

  db << "IOleObject::SetHostNames() Returning " << hRes ;
  db.DebugOutput(m_pObject->m_pListBox);

  return hRes;
} /* SetHostNames()
   */


STDMETHODIMP NC(CHandlerObject, COleObject)::
  Close(DWORD dwSaveOption)
//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
{
  CDebug db;

  db << "IOleObject::Close(" << dwSaveOption << ") Called";
  db.DebugOutput(m_pObject->m_pListBox);

  HRESULT hRes = ResultFromScode(E_NOTIMPL);

  if (m_pObject->m_pOleObject != NULL) {
	hRes = m_pObject->m_pOleObject->Close(dwSaveOption);
  }

  db << "IOleObject::Close() Returning " << hRes ;
  db.DebugOutput(m_pObject->m_pListBox);

  return hRes;
} /* Close()
   */


STDMETHODIMP NC(CHandlerObject, COleObject)::
  SetMoniker(DWORD dwWhichMoniker, LPMONIKER pmk)
//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
{
  CDebug db;

  db << "IOleObject::SetMoniker(" << dwWhichMoniker << pmk << ") Called";
  db.DebugOutput(m_pObject->m_pListBox);

  HRESULT hRes = ResultFromScode(E_NOTIMPL);

  if (m_pObject->m_pOleObject != NULL) {
	hRes = m_pObject->m_pOleObject->SetMoniker(dwWhichMoniker, pmk);
  }

  db << "IOleObject::SetMoniker() Returning " << hRes ;
  db.DebugOutput(m_pObject->m_pListBox);

  return hRes;
} /* SetMoniker()
   */


STDMETHODIMP NC(CHandlerObject, COleObject)::
  GetMoniker(DWORD dwAssign, DWORD dwWhichMoniker, LPMONIKER FAR* ppmk)
//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
{
  CDebug db;

  db << "IOleObject::GetMoniker(" << dwAssign << dwWhichMoniker
	 << (LPUNKNOWN) ppmk << ") Called";
  db.DebugOutput(m_pObject->m_pListBox);

  HRESULT hRes = ResultFromScode(E_NOTIMPL);

  if (m_pObject->m_pOleObject != NULL) {
	hRes = m_pObject->m_pOleObject->GetMoniker(dwAssign, dwWhichMoniker, ppmk);
  }

  db << "IOleObject::GetMoniker(" << dwAssign << dwWhichMoniker
	 << *ppmk << ") Returning " << hRes ;
  db.DebugOutput(m_pObject->m_pListBox);

  return hRes;
} /* GetMoniker()
   */


STDMETHODIMP NC(CHandlerObject, COleObject)::
  InitFromData(LPDATAOBJECT pDataObject, BOOL fCreation, DWORD dwReserved)
//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
{
  CDebug db;

  db << "IOleObject::InitFromData(" << (LPUNKNOWN)pDataObject << fCreation
	 << dwReserved << ") Called";
  db.DebugOutput(m_pObject->m_pListBox);

  HRESULT hRes = ResultFromScode(E_NOTIMPL);

  if (m_pObject->m_pOleObject != NULL) {
	hRes = m_pObject->m_pOleObject->InitFromData(pDataObject, fCreation,
											  dwReserved);
  }

  db << "IOleObject::InitFromData() Returning " << hRes ;
  db.DebugOutput(m_pObject->m_pListBox);

  return hRes;
} /* InitFromData()
   */


STDMETHODIMP NC(CHandlerObject, COleObject)::
  GetClipboardData(DWORD dwReserved, LPDATAOBJECT FAR* ppDObj)
//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
{
  CDebug db;

  db << "IOleObject::GetClipboardData(" << dwReserved
	 << (LPUNKNOWN)ppDObj << ") Called";
  db.DebugOutput(m_pObject->m_pListBox);

  HRESULT hRes = ResultFromScode(E_NOTIMPL);

  if (m_pObject->m_pOleObject != NULL) {
	hRes = m_pObject->m_pOleObject->GetClipboardData(dwReserved, ppDObj);
  }

  db << "IOleObject::GetClipboardData() Returning " << hRes ;
  db.DebugOutput(m_pObject->m_pListBox);

  return hRes;
} /* GetClipboardData()
   */


STDMETHODIMP NC(CHandlerObject, COleObject)::
  DoVerb(LONG iVerb, LPMSG lpmsg, LPOLECLIENTSITE pActiveSite,
		 LONG lindex, HWND hwndParent, LPCRECT lprcPosRect)
//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
{
  CDebug db;

  db << "IOleObject::DoVerb(" << iVerb << (LPUNKNOWN)lpmsg
	 << (LPUNKNOWN)pActiveSite << lindex << (WORD)hwndParent
	 << lprcPosRect << ") Called";
  db.DebugOutput(m_pObject->m_pListBox);

  HRESULT hRes = ResultFromScode(E_NOTIMPL);

  if (m_pObject->m_pOleObject != NULL) {
	hRes = m_pObject->m_pOleObject->DoVerb(iVerb, lpmsg, pActiveSite, lindex,
										hwndParent, lprcPosRect);
  }

  db << "IOleObject::DoVerb() Returning " << hRes ;
  db.DebugOutput(m_pObject->m_pListBox);

  return hRes;
} /* DoVerb()
   */


STDMETHODIMP NC(CHandlerObject, COleObject)::
  EnumVerbs(IEnumOLEVERB FAR* FAR* ppenumOleVerb)
//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
{
  CDebug db;

  db << "IOleObject::EnumVerbs(" << (LPUNKNOWN) ppenumOleVerb << ") Called";
  db.DebugOutput(m_pObject->m_pListBox);

  HRESULT hRes = ResultFromScode(E_NOTIMPL);

  if (m_pObject->m_pOleObject != NULL) {
	hRes = m_pObject->m_pOleObject->EnumVerbs(ppenumOleVerb);
  }

  db << "IOleObject::EnumVerbs() Returning " << hRes ;
  db.DebugOutput(m_pObject->m_pListBox);

  return hRes;
} /* EnumVerbs()
   */


STDMETHODIMP NC(CHandlerObject, COleObject)::
  Update(VOID)
//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
{
  CDebug db;

  db << "IOleObject::Update() Called";
  db.DebugOutput(m_pObject->m_pListBox);

  HRESULT hRes = ResultFromScode(E_NOTIMPL);

  if (m_pObject->m_pOleObject != NULL) {
	hRes = m_pObject->m_pOleObject->Update();
  }

  db << "IOleObject::Update() Returning " << hRes ;
  db.DebugOutput(m_pObject->m_pListBox);

  return hRes;
} /* Update()
   */


STDMETHODIMP NC(CHandlerObject, COleObject)::
  IsUpToDate(VOID)
//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
{
  CDebug db;

  db << "IOleObject::IsUpToDate() Called";
  db.DebugOutput(m_pObject->m_pListBox);

  HRESULT hRes = ResultFromScode(E_NOTIMPL);

  if (m_pObject->m_pOleObject != NULL) {
	hRes = m_pObject->m_pOleObject->IsUpToDate();
  }

  db << "IOleObject::IsUpToDate() Returning " << hRes ;
  db.DebugOutput(m_pObject->m_pListBox);

  return hRes;
} /* IsUpToDate()
   */


STDMETHODIMP NC(CHandlerObject, COleObject)::
  GetUserClassID(CLSID FAR* pClsid)
//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
{
  CDebug db;

  db << "IOleObject::GetUserClassID(" << (LPUNKNOWN)pClsid << ") Called";
  db.DebugOutput(m_pObject->m_pListBox);

  HRESULT hRes = ResultFromScode(E_NOTIMPL);

  if (m_pObject->m_pOleObject != NULL) {
	hRes = m_pObject->m_pOleObject->GetUserClassID(pClsid);
  }

  db << "IOleObject::GetUserClassID(" << *pClsid << ") Returning " << hRes ;
  db.DebugOutput(m_pObject->m_pListBox);

  return hRes;
} /* GetUserClassID()
   */


STDMETHODIMP NC(CHandlerObject, COleObject)::
  GetUserType(DWORD dwFormOfType, LPSTR FAR* pszUserType)
//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
{
  CDebug db;

  db << "IOleObject::GetUserType(" << dwFormOfType << (LPUNKNOWN)pszUserType
	 << ") Called";
  db.DebugOutput(m_pObject->m_pListBox);

  HRESULT hRes = ResultFromScode(E_NOTIMPL);

  if (m_pObject->m_pOleObject != NULL) {
	hRes = m_pObject->m_pOleObject->GetUserType(dwFormOfType, pszUserType);
  }

  db << "IOleObject::GetUserType(" << dwFormOfType << ", " << *pszUserType
	 << ") Returning " << hRes ;
  db.DebugOutput(m_pObject->m_pListBox);

  return hRes;
} /* GetUserType()
   */


STDMETHODIMP NC(CHandlerObject, COleObject)::
  SetExtent(DWORD dwDrawAspect, LPSIZEL lpsizel)
//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
{
  CDebug db;

  db << "IOleObject::SetExtent(" << dwDrawAspect << lpsizel << ") Called";
  db.DebugOutput(m_pObject->m_pListBox);

  HRESULT hRes = ResultFromScode(E_NOTIMPL);

  if (m_pObject->m_pOleObject != NULL) {
	hRes = m_pObject->m_pOleObject->SetExtent(dwDrawAspect, lpsizel);
  }

  db << "IOleObject::SetExtent() Returning " << hRes ;
  db.DebugOutput(m_pObject->m_pListBox);

  return hRes;
} /* SetExtent()
   */


STDMETHODIMP NC(CHandlerObject, COleObject)::
  GetExtent(DWORD dwDrawAspect, LPSIZEL lpsizel)
//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
{
  CDebug db;

  db << "IOleObject::GetExtent(" << dwDrawAspect << lpsizel << ") Called" ;
  db.DebugOutput(m_pObject->m_pListBox);

  HRESULT hRes = ResultFromScode(E_NOTIMPL);

  if (m_pObject->m_pOleObject != NULL) {
	hRes = m_pObject->m_pOleObject->GetExtent(dwDrawAspect, lpsizel);
  }

  db << "IOleObject::GetExtent(" << dwDrawAspect << lpsizel << ") Returning "
	 << hRes ;
  db.DebugOutput(m_pObject->m_pListBox);

  return hRes;
} /* GetExtent()
   */


STDMETHODIMP NC(CHandlerObject, COleObject)::
  Advise(IAdviseSink FAR* pAdvSink, DWORD FAR* pdwConnection)
//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
{
  CDebug db;

  db << "IOleObject::Advise(" << (LPUNKNOWN)pAdvSink
	 << (LPUNKNOWN)pdwConnection << ") Called";
  db.DebugOutput(m_pObject->m_pListBox);

  HRESULT hRes = ResultFromScode(E_NOTIMPL);

  if (m_pObject->m_pOleObject != NULL) {
	hRes = m_pObject->m_pOleObject->Advise(pAdvSink, pdwConnection);
  }

  db << "IOleObject::Advise(" << (LPUNKNOWN)pAdvSink << *pdwConnection
	 << ") Returning " << hRes ;
  db.DebugOutput(m_pObject->m_pListBox);

  return hRes;
} /* Advise()
   */


STDMETHODIMP NC(CHandlerObject, COleObject)::
  Unadvise(DWORD dwConnection)
//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
{
  CDebug db;

  db << "IOleObject::Unadvise(" << dwConnection << ") Called";
  db.DebugOutput(m_pObject->m_pListBox);

  HRESULT hRes = ResultFromScode(E_NOTIMPL);

  if (m_pObject->m_pOleObject != NULL) {
	hRes = m_pObject->m_pOleObject->Unadvise(dwConnection);
  }

  db << "IOleObject::Unadvise() Returning " << hRes ;
  db.DebugOutput(m_pObject->m_pListBox);

  return hRes;
} /* Unadvise()
   */


STDMETHODIMP NC(CHandlerObject, COleObject)::
  EnumAdvise(LPENUMSTATDATA FAR* ppenumAdvise)
//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
{
  CDebug db;

  db << "IOleObject::EnumAdvise(" << (LPUNKNOWN)ppenumAdvise << ") Called";
  db.DebugOutput(m_pObject->m_pListBox);

  HRESULT hRes = ResultFromScode(E_NOTIMPL);

  if (m_pObject->m_pOleObject != NULL) {
	hRes = m_pObject->m_pOleObject->EnumAdvise(ppenumAdvise);
  }

  db << "IOleObject::EnumAdvise() Returning " << hRes ;
  db.DebugOutput(m_pObject->m_pListBox);

  return hRes;
} /* EnumAdvise()
   */


STDMETHODIMP NC(CHandlerObject, COleObject)::
  GetMiscStatus(DWORD dwAspect, DWORD FAR* pdwStatus)
//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
{
  CDebug db;

  db << "IOleObject::GetMiscStatus(" << dwAspect << (LPUNKNOWN)pdwStatus
	 << ") Called";
  db.DebugOutput(m_pObject->m_pListBox);

  HRESULT hRes = ResultFromScode(E_NOTIMPL);

  if (m_pObject->m_pOleObject != NULL) {
	hRes = m_pObject->m_pOleObject->GetMiscStatus(dwAspect, pdwStatus);
  }

  db << "IOleObject::GetMiscStatus(" << dwAspect << *pdwStatus
	 << ") Returning " << hRes ;
  db.DebugOutput(m_pObject->m_pListBox);

  return hRes;
} /* GetMiscStatus()
   */


STDMETHODIMP NC(CHandlerObject, COleObject)::
  SetColorScheme(LPLOGPALETTE lpLogpal)
//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
{
  CDebug db;

  db << "IOleObject::SetColorScheme(" << (LPUNKNOWN) lpLogpal << ") Called";
  db.DebugOutput(m_pObject->m_pListBox);

  HRESULT hRes = ResultFromScode(E_NOTIMPL);

  if (m_pObject->m_pOleObject != NULL) {
	hRes = m_pObject->m_pOleObject->SetColorScheme(lpLogpal);
  }

  db << "IOleObject::SetColorScheme() Returning " << hRes ;
  db.DebugOutput(m_pObject->m_pListBox);

  return hRes;
} /* SetColorScheme()
   */


STDMETHODIMP NC(CHandlerObject, CDataObject)::
  GetData(LPFORMATETC pformatetcIn, LPSTGMEDIUM pmedium)
//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
{
  CDebug db;

  db << "IDataObject::GetData(" << pformatetcIn << pmedium << ") Called";
  db.DebugOutput(m_pObject->m_pListBox);

  HRESULT hRes = ResultFromScode(E_NOTIMPL);

  if (m_pObject->m_pDataObject != NULL) {
	hRes = m_pObject->m_pDataObject->GetData(pformatetcIn, pmedium);
  }

  db << "IDataObject::GetData(" << pformatetcIn << pmedium << ") Returning "
	 << hRes;
  db.DebugOutput(m_pObject->m_pListBox);

  return hRes;
} /* GetData()
   */


STDMETHODIMP NC(CHandlerObject, CDataObject)::
  GetDataHere(LPFORMATETC pformatetcIn, LPSTGMEDIUM pmedium)
//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
{
  CDebug db;

  db << "IDataObject::GetDataHere(" << pformatetcIn << pmedium << ") Called";
  db.DebugOutput(m_pObject->m_pListBox);

  HRESULT hRes = ResultFromScode(E_NOTIMPL);

  if (m_pObject->m_pDataObject != NULL) {
	hRes = m_pObject->m_pDataObject->GetDataHere(pformatetcIn, pmedium);
  }

  db << "IDataObject::GetDataHere(" << pformatetcIn << pmedium
	 << ") Returning " << hRes;
  db.DebugOutput(m_pObject->m_pListBox);

  return hRes;
} /* GetDataHere()
   */


STDMETHODIMP NC(CHandlerObject, CDataObject)::
  QueryGetData(LPFORMATETC pformatetcIn)
//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
{
  CDebug db;

  db << "IDataObject::QueryGetData(" << pformatetcIn << ") Called";
  db.DebugOutput(m_pObject->m_pListBox);

  HRESULT hRes = ResultFromScode(E_NOTIMPL);

  if (m_pObject->m_pDataObject != NULL) {
	hRes = m_pObject->m_pDataObject->QueryGetData(pformatetcIn);
  }

  db << "IDataObject::QueryGetData(" << pformatetcIn << ") Returning " << hRes;
  db.DebugOutput(m_pObject->m_pListBox);

  return hRes;
} /* QueryGetData()
   */


STDMETHODIMP NC(CHandlerObject, CDataObject)::
  GetCanonicalFormatEtc(LPFORMATETC petcIn, LPFORMATETC petcOut)
//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
{
  CDebug db;

  db << "IDataObject::GetCanonicalFormatEtc(" << petcIn << petcOut
	 << ") Called";
  db.DebugOutput(m_pObject->m_pListBox);

  HRESULT hRes = ResultFromScode(E_NOTIMPL);

  if (m_pObject->m_pDataObject != NULL) {
	hRes = m_pObject->m_pDataObject->GetCanonicalFormatEtc(petcIn, petcOut);
  }

  db << "IDataObject::GetCanonicalFormatEtc(" << petcIn << petcOut
	 << ") Returning " << hRes;
  db.DebugOutput(m_pObject->m_pListBox);

  return hRes;
} /* GetCanonicalFormatEtc()
   */


STDMETHODIMP NC(CHandlerObject, CDataObject)::
  SetData(LPFORMATETC pformatetcIn, LPSTGMEDIUM pmedium, BOOL fRelease)
//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
{
  CDebug db;

  db << "IDataObject::SetData(" << pformatetcIn << pmedium << fRelease
	 <<") Called";
  db.DebugOutput(m_pObject->m_pListBox);

  HRESULT hRes = ResultFromScode(E_NOTIMPL);

  if (m_pObject->m_pDataObject != NULL) {
	hRes = m_pObject->m_pDataObject->SetData(pformatetcIn, pmedium, fRelease);
  }

  db << "IDataObject::SetData(" << pformatetcIn << pmedium << fRelease
	 << ") Returning " << hRes;
  db.DebugOutput(m_pObject->m_pListBox);

  return hRes;
} /* SetData()
   */


STDMETHODIMP NC(CHandlerObject, CDataObject)::
  EnumFormatEtc(DWORD dwDirection, LPENUMFORMATETC FAR* ppenum)
//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
{
  CDebug db;

  db << "IDataObject::EnumFormatEtc(" << dwDirection << (LPUNKNOWN)ppenum
	 << ") Called";
  db.DebugOutput(m_pObject->m_pListBox);

  HRESULT hRes = ResultFromScode(E_NOTIMPL);

  if (m_pObject->m_pDataObject != NULL) {
	hRes = m_pObject->m_pDataObject->EnumFormatEtc(dwDirection, ppenum);
  }

  db << "IDataObject::EnumFormatEtc() Returning " << hRes;
  db.DebugOutput(m_pObject->m_pListBox);

  return hRes;
} /* EnumFormatEtc()
   */


STDMETHODIMP NC(CHandlerObject, CDataObject)::
  DAdvise(LPFORMATETC pFormatetc, DWORD advf, LPADVISESINK pAdvSink,
		 LPDWORD pdwConnection)
//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
{
  CDebug db;

  db << "IDataObject::DAdvise(" << pFormatetc << advf << (LPUNKNOWN)pAdvSink
	 << (LPUNKNOWN)pdwConnection << ") Called";
  db.DebugOutput(m_pObject->m_pListBox);

  HRESULT hRes = ResultFromScode(E_NOTIMPL);

  if (m_pObject->m_pDataObject != NULL) {
	hRes = m_pObject->m_pDataObject->DAdvise(pFormatetc, advf, pAdvSink,
											 pdwConnection);
  }

  db << "IDataObject::DAdvise(" << pFormatetc << advf << (LPUNKNOWN)pAdvSink
	 << *pdwConnection << ") Returning " << hRes;
  db.DebugOutput(m_pObject->m_pListBox);

  return hRes;
} /* DAdvise()
   */


STDMETHODIMP NC(CHandlerObject, CDataObject)::
  DUnadvise(DWORD dwConnection)
//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
{
  CDebug db;

  db << "IDataObject::DUnadvise(" << dwConnection << ") Called";
  db.DebugOutput(m_pObject->m_pListBox);

  HRESULT hRes = ResultFromScode(E_NOTIMPL);

  if (m_pObject->m_pDataObject != NULL) {
	hRes = m_pObject->m_pDataObject->DUnadvise(dwConnection);
  }

  db << "IDataObject::DUnadvise() Returning " << hRes;
  db.DebugOutput(m_pObject->m_pListBox);

  return hRes;
} /* DUnadvise()
   */


STDMETHODIMP NC(CHandlerObject, CDataObject)::
  EnumDAdvise(LPENUMSTATDATA FAR* ppenumAdvise)
//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
{
  CDebug db;

  db << "IDataObject::EnumDAdvise(" << (LPUNKNOWN) ppenumAdvise << ") Called";
  db.DebugOutput(m_pObject->m_pListBox);


  HRESULT hRes = ResultFromScode(E_NOTIMPL);

  if (m_pObject->m_pDataObject != NULL) {
	hRes = m_pObject->m_pDataObject->EnumDAdvise(ppenumAdvise);
  }

  db << "IDataObject::EnumDAdvise() Returning " << hRes;
  db.DebugOutput(m_pObject->m_pListBox);

  return hRes;
} /* EnumDAdvise()
   */


STDMETHODIMP NC(CHandlerObject, CViewObject2)::
  Draw(DWORD dwDrawAspect, LONG lindex, void FAR* pvAspect,
	   DVTARGETDEVICE FAR * ptd, HDC hicTargetDev, HDC hdcDraw,
	   LPCRECTL lprcBounds, LPCRECTL lprcWBounds,
	   BOOL (CALLBACK * pfnContinue) (DWORD), DWORD dwContinue)
//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
{
  CDebug db;

  db << "IViewObject2::Draw(" << dwDrawAspect << lindex << (LPUNKNOWN)pvAspect
	 << (LPUNKNOWN)ptd << (WORD)hicTargetDev << (WORD)hdcDraw
	 << lprcBounds << lprcWBounds << (LPUNKNOWN)pfnContinue << dwContinue
	 << ") Called" ;
  db.DebugOutput(m_pObject->m_pListBox);


  HRESULT hRes = ResultFromScode(E_NOTIMPL);

  if (m_pObject->m_pViewObject2 != NULL) {
	hRes = m_pObject->m_pViewObject2->Draw(dwDrawAspect, lindex, pvAspect,
									 ptd, hicTargetDev, hdcDraw, lprcBounds,
									 lprcWBounds, pfnContinue, dwContinue);
  }

  db << "IViewObject2::Draw() Returning " << hRes ;
  db.DebugOutput(m_pObject->m_pListBox);

  return hRes;
} /* Draw()
   */


STDMETHODIMP NC(CHandlerObject, CViewObject2)::
  GetColorSet(DWORD dwDrawAspect, LONG lindex, void FAR* pvAspect,
			  DVTARGETDEVICE FAR * ptd, HDC hicTargetDev,
			  LPLOGPALETTE FAR* ppColorSet)
//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
{
  CDebug db;

  db << "IViewObject2::GetColorSet(" << dwDrawAspect << lindex
	 << (LPUNKNOWN)pvAspect << (LPUNKNOWN)ptd << (WORD) hicTargetDev
	 << (LPUNKNOWN)ppColorSet  << ") Called" ;
  db.DebugOutput(m_pObject->m_pListBox);


  HRESULT hRes = ResultFromScode(E_NOTIMPL);

  if (m_pObject->m_pViewObject2 != NULL) {
	hRes = m_pObject->m_pViewObject2->GetColorSet(dwDrawAspect, lindex,
												  pvAspect, ptd, hicTargetDev,
												  ppColorSet);
  }

  db << "IViewObject2::GetColorSet() Returning " << hRes ;
  db.DebugOutput(m_pObject->m_pListBox);

  return hRes;
} /* GetColorSet()
   */


STDMETHODIMP NC(CHandlerObject, CViewObject2)::
  Freeze(DWORD dwDrawAspect, LONG lindex, void FAR* pvAspect,
		 DWORD FAR* pdwFreeze)
//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
{
  CDebug db;

  db << "IViewObject2::Freeze(" << dwDrawAspect << lindex << (LPUNKNOWN)pvAspect
	 << (LPUNKNOWN)pdwFreeze  << ") Called" ;
  db.DebugOutput(m_pObject->m_pListBox);


  HRESULT hRes = ResultFromScode(E_NOTIMPL);

  if (m_pObject->m_pViewObject2 != NULL) {
	hRes = m_pObject->m_pViewObject2->Freeze(dwDrawAspect, lindex, pvAspect,
										 pdwFreeze);
  }

  db << "IViewObject2::Freeze() Returning " << hRes ;
  db.DebugOutput(m_pObject->m_pListBox);

  return hRes;
} /* Freeze()
   */


STDMETHODIMP NC(CHandlerObject, CViewObject2)::
  Unfreeze(DWORD dwFreeze)
//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
{
  CDebug db;

  db << "IViewObject2::Unfreeze(" << dwFreeze << ") Called" ;
  db.DebugOutput(m_pObject->m_pListBox);


  HRESULT hRes = ResultFromScode(E_NOTIMPL);

  if (m_pObject->m_pViewObject2 != NULL) {
	hRes = m_pObject->m_pViewObject2->Unfreeze(dwFreeze);
  }

  db << "IViewObject2::Unfreeze() Returning " << hRes ;
  db.DebugOutput(m_pObject->m_pListBox);

  return hRes;
} /* Unfreeze()
   */


STDMETHODIMP NC(CHandlerObject, CViewObject2)::
  SetAdvise(DWORD aspects, DWORD advf, LPADVISESINK pAdvSink)
//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
{
  CDebug db;

  db << "IViewObject2::SetAdvise(" << aspects << advf << (LPUNKNOWN) pAdvSink
	 << ") Called" ;
  db.DebugOutput(m_pObject->m_pListBox);


  HRESULT hRes = ResultFromScode(E_NOTIMPL);

  if (m_pObject->m_pViewObject2 != NULL) {
	hRes = m_pObject->m_pViewObject2->SetAdvise(aspects, advf, pAdvSink);
  }

  db << "IViewObject2::SetAdvise() Returning " << hRes ;
  db.DebugOutput(m_pObject->m_pListBox);

  return hRes;
} /* SetAdvise()
   */


STDMETHODIMP NC(CHandlerObject, CViewObject2)::
  GetAdvise(DWORD FAR* pAspects, DWORD FAR* pAdvf, LPADVISESINK FAR* ppAdvSink)
//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
{
  CDebug db;

  db << "IViewObject2::GetAdvise(" << (LPUNKNOWN)pAspects << (LPUNKNOWN) pAdvf
	 << (LPUNKNOWN) ppAdvSink << ") Called" ;
  db.DebugOutput(m_pObject->m_pListBox);


  HRESULT hRes = ResultFromScode(E_NOTIMPL);

  if (m_pObject->m_pViewObject2 != NULL) {
	hRes = m_pObject->m_pViewObject2->GetAdvise(pAspects, pAdvf, ppAdvSink);
  }

  db << "IViewObject2::GetAdvise(" << *pAspects << *pAdvf
	 << (LPUNKNOWN) *ppAdvSink << ") Returning " << hRes ;
  db.DebugOutput(m_pObject->m_pListBox);

  return hRes;
} /* GetAdvise()
   */


STDMETHODIMP NC(CHandlerObject, CViewObject2)::
  GetExtent(DWORD dwDrawAspect, LONG lindex, DVTARGETDEVICE FAR * ptd,
			LPSIZEL lpsizel)
//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
{
  CDebug db;

  db << "IViewObject2::GetExtent(" << dwDrawAspect << lindex << ptd << lpsizel
	 << ") Called" ;
  db.DebugOutput(m_pObject->m_pListBox);


  HRESULT hRes = ResultFromScode(E_NOTIMPL);

  if (m_pObject->m_pViewObject2 != NULL) {
	hRes = m_pObject->m_pViewObject2->GetExtent(dwDrawAspect, lindex, ptd,
												lpsizel);
  }

  db << "IViewObject2::GetExtent(" << dwDrawAspect << lindex << ptd << lpsizel
	 <<  ") Returning " << hRes ;
  db.DebugOutput(m_pObject->m_pListBox);

  return hRes;
} /* GetExtent()
   */


STDMETHODIMP NC(CHandlerObject, COleCache2)::
  Cache(LPFORMATETC lpFormatetc, DWORD advf, LPDWORD lpdwConnection)
//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
{
  CDebug db;

  db << "IOleCache2::Cache(" << lpFormatetc << advf
	 << (LPUNKNOWN)lpdwConnection << ") Called" ;
  db.DebugOutput(m_pObject->m_pListBox);


  HRESULT hRes = ResultFromScode(E_NOTIMPL);

  if (m_pObject->m_pOleCache2 != NULL) {
	hRes = m_pObject->m_pOleCache2->Cache(lpFormatetc, advf, lpdwConnection);
  }

  db << "IOleCache2::Cache(" << lpFormatetc << advf << *lpdwConnection
	 << ") Returning " << hRes ;
  db.DebugOutput(m_pObject->m_pListBox);

  return hRes;
} /* Cache()
   */


STDMETHODIMP NC(CHandlerObject, COleCache2)::
  Uncache(DWORD dwConnection)
//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
{
  CDebug db;

  db << "IOleCache2::Uncache(" << dwConnection << ") Called" ;
  db.DebugOutput(m_pObject->m_pListBox);


  HRESULT hRes = ResultFromScode(E_NOTIMPL);

  if (m_pObject->m_pOleCache2 != NULL) {
	hRes = m_pObject->m_pOleCache2->Uncache(dwConnection);
  }

  db << "IOleCache2::Uncache() Returning " << hRes ;
  db.DebugOutput(m_pObject->m_pListBox);

  return hRes;
} /* Uncache()
   */


STDMETHODIMP NC(CHandlerObject, COleCache2)::
  EnumCache(LPENUMSTATDATA FAR* ppenumStatData)
//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
{
  CDebug db;

  db << "IOleCache2::EnumCache(" << (LPUNKNOWN)ppenumStatData << ") Called" ;
  db.DebugOutput(m_pObject->m_pListBox);


  HRESULT hRes = ResultFromScode(E_NOTIMPL);

  if (m_pObject->m_pOleCache2 != NULL) {
	hRes = m_pObject->m_pOleCache2->EnumCache(ppenumStatData);
  }

  db << "IOleCache2::EnumCache(" << (LPUNKNOWN)*ppenumStatData
	 << ") Returning " << hRes ;
  db.DebugOutput(m_pObject->m_pListBox);

  return hRes;
} /* EnumCache()
   */


STDMETHODIMP NC(CHandlerObject, COleCache2)::
  InitCache(LPDATAOBJECT pDataObject)
//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
{
  CDebug db;

  db << "IOleCache2::InitCache(" << (LPUNKNOWN)pDataObject << ") Called" ;
  db.DebugOutput(m_pObject->m_pListBox);


  HRESULT hRes = ResultFromScode(E_NOTIMPL);

  if (m_pObject->m_pOleCache2 != NULL) {
	hRes = m_pObject->m_pOleCache2->InitCache(pDataObject);
  }

  db << "IOleCache2::InitCache() Returning " << hRes ;
  db.DebugOutput(m_pObject->m_pListBox);

  return hRes;
} /* InitCache()
   */


STDMETHODIMP NC(CHandlerObject, COleCache2)::
  SetData(LPFORMATETC pformatetc, STGMEDIUM FAR * pmedium, BOOL fRelease)
//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
{
  CDebug db;

  db << "IOleCache2::SetData(" << pformatetc << pmedium << fRelease
	 << ") Called" ;
  db.DebugOutput(m_pObject->m_pListBox);


  HRESULT hRes = ResultFromScode(E_NOTIMPL);

  if (m_pObject->m_pOleCache2 != NULL) {
	hRes = m_pObject->m_pOleCache2->SetData(pformatetc, pmedium, fRelease);
  }

  db << "IOleCache2::SetData() Returning " << hRes ;
  db.DebugOutput(m_pObject->m_pListBox);

  return hRes;
} /* SetData()
   */


STDMETHODIMP NC(CHandlerObject, COleCache2)::
  UpdateCache(LPDATAOBJECT pDataObject, DWORD grfUpdf, LPVOID pReserved)
//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
{
  CDebug db;

  db << "IOleCache2::UpdateCache(" << (LPUNKNOWN)pDataObject << grfUpdf
	 << (LPUNKNOWN)pReserved << ") Called" ;
  db.DebugOutput(m_pObject->m_pListBox);


  HRESULT hRes = ResultFromScode(E_NOTIMPL);

  if (m_pObject->m_pOleCache2 != NULL) {
	hRes = m_pObject->m_pOleCache2->UpdateCache(pDataObject, grfUpdf,
												pReserved);
  }

  db << "IOleCache2::UpdateCache() Returning " << hRes ;
  db.DebugOutput(m_pObject->m_pListBox);

  return hRes;
} /* UpdateCache()
   */


STDMETHODIMP NC(CHandlerObject, COleCache2)::
  DiscardCache(DWORD dwDiscardOptions)
//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
{
  CDebug db;

  db << "IOleCache2::DiscardCache(" << dwDiscardOptions << ") Called" ;
  db.DebugOutput(m_pObject->m_pListBox);

  HRESULT hRes = ResultFromScode(E_NOTIMPL);
  if (m_pObject->m_pOleCache2 != NULL) {
	hRes = m_pObject->m_pOleCache2->DiscardCache(dwDiscardOptions);
  }

  db << "IOleCache2::DiscardCache() Returning " << hRes ;
  db.DebugOutput(m_pObject->m_pListBox);

  return hRes;
} /* DiscardCache()
   */


STDMETHODIMP NC(CHandlerObject, CRunnableObject)::
  GetRunningClass(LPCLSID lpClsid)
//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
{
  CDebug db;

  db << "IRunnableObject::GetRunningClass(" << lpClsid << ") Called" ;
  db.DebugOutput(m_pObject->m_pListBox);

  HRESULT hRes = ResultFromScode(E_NOTIMPL);

  if (m_pObject->m_pRunnableObject != NULL) {
	hRes = m_pObject->m_pRunnableObject->GetRunningClass(lpClsid);
  }

  db << "IRunnableObject::GetRunningClass(" << lpClsid << ") Returning "
	 << hRes ;
  db.DebugOutput(m_pObject->m_pListBox);

  return hRes;
} /* GetRunningClass()
   */


STDMETHODIMP NC(CHandlerObject, CRunnableObject)::
  Run(LPBINDCTX pbc)
//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
{
  CDebug db;

  db << "IRunnableObject::Run(" << (LPUNKNOWN)pbc << ") Called" ;
  db.DebugOutput(m_pObject->m_pListBox);

  HRESULT hRes = ResultFromScode(E_NOTIMPL);

  if (m_pObject->m_pRunnableObject != NULL) {
	hRes = m_pObject->m_pRunnableObject->Run(pbc);
  }

  db << "IRunnableObject::Run() Returning " << hRes ;
  db.DebugOutput(m_pObject->m_pListBox);

  return hRes;
} /* Run()
   */


STDMETHODIMP_(BOOL) NC(CHandlerObject, CRunnableObject)::
  IsRunning()
//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
{
  CDebug db;

  db << "IRunnableObject::IsRunning() Called" ;
  db.DebugOutput(m_pObject->m_pListBox);

  BOOL bIsRunning = FALSE;
  if (m_pObject->m_pRunnableObject != NULL) {
	bIsRunning = m_pObject->m_pRunnableObject->IsRunning();
  }

  db << "IRunnableObject::IsRunning() Returning " << bIsRunning ;
  db.DebugOutput(m_pObject->m_pListBox);

  return bIsRunning;
} /* IsRunning()
   */


STDMETHODIMP NC(CHandlerObject, CRunnableObject)::
  LockRunning(BOOL fLock, BOOL fLastUnlockCloses)
//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
{
  CDebug db;

  db << "IRunnableObject::LockRunning(" << fLock << fLastUnlockCloses
	 << ") Called" ;
  db.DebugOutput(m_pObject->m_pListBox);

  HRESULT hRes = ResultFromScode(E_NOTIMPL);

  if (m_pObject->m_pRunnableObject != NULL) {
	hRes = m_pObject->m_pRunnableObject->LockRunning(fLock, fLastUnlockCloses);
  }

  db << "IRunnableObject::LockRunning() Returning " << hRes ;
  db.DebugOutput(m_pObject->m_pListBox);

  return hRes;
} /* LockRunning()
   */


STDMETHODIMP NC(CHandlerObject, CRunnableObject)::
  SetContainedObject(BOOL fContained)
//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
{
  CDebug db;

  db << "IRunnableObject::SetContainedObject(" << fContained << ") Called" ;
  db.DebugOutput(m_pObject->m_pListBox);

  HRESULT hRes = ResultFromScode(E_NOTIMPL);

  if (m_pObject->m_pRunnableObject != NULL) {
	hRes = m_pObject->m_pRunnableObject->SetContainedObject(fContained);
  }

  db << "IRunnableObject::SetContainedObject() Returning " << hRes ;
  db.DebugOutput(m_pObject->m_pListBox);

  return hRes;
} /* SetContainedObject()
   */


STDMETHODIMP_(DWORD) NC(CHandlerObject, CExternalConnection)::
  AddConnection(DWORD extconn, DWORD reserved)
//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
{
  CDebug db;

  db << "IExternalConnection::AddConnection(" << extconn << reserved
	 << ") Called" ;
  db.DebugOutput(m_pObject->m_pListBox);

  DWORD dw=0;
  if (m_pObject->m_pExternalConnection != NULL) {
	dw = m_pObject->m_pExternalConnection->AddConnection(extconn, reserved);
  }

  db << "IExternalConnection::AddConnection() Returning " << dw ;
  db.DebugOutput(m_pObject->m_pListBox);

  return dw;
} /* AddConnection()
   */


STDMETHODIMP_(DWORD) NC(CHandlerObject, CExternalConnection)::
  ReleaseConnection(DWORD extconn, DWORD reserved, BOOL fLastReleaseCloses)
//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
{
  CDebug db;

  db << "IExternalConnection::ReleaseConnection(" << extconn << reserved
	 << fLastReleaseCloses << ") Called" ;
  db.DebugOutput(m_pObject->m_pListBox);

  DWORD dw=0;
  if (m_pObject->m_pExternalConnection != NULL) {
	dw = m_pObject->m_pExternalConnection->ReleaseConnection(extconn, reserved,
														 fLastReleaseCloses);
  }

  db << "IExternalConnection::ReleaseConnection() Returning " << dw;
  db.DebugOutput(m_pObject->m_pListBox);

  return dw;
} /* ReleaseConnection()
   */


STDMETHODIMP NC(CHandlerObject, CStdMarshalInfo)::
  GetClassForHandler(DWORD dwDestContext, LPVOID pvDestContext, LPCLSID pClsid)
//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
{
  CDebug db;

  db << "IStdMarshalInfo::GetClassForHandler(" << dwDestContext
	 << (LPUNKNOWN)pvDestContext << pClsid << ") Called" ;
  db.DebugOutput(m_pObject->m_pListBox);

  HRESULT hRes = ResultFromScode(E_NOTIMPL);

  if (m_pObject->m_pStdMarshalInfo != NULL) {
	hRes = m_pObject->m_pStdMarshalInfo->GetClassForHandler(dwDestContext,
															pvDestContext,
															pClsid);
  }

  db << "IStdMarshalInfo::GetClassForHandler(" << dwDestContext
	 << (LPUNKNOWN)pvDestContext << *pClsid << ") Returning " << hRes ;
  db.DebugOutput(m_pObject->m_pListBox);

  return hRes;
} /* GetClassForHandler()
   */
