//----------------------------------------------------------------------------
//
// (c) Copyright Microsoft Corp. 1993 All Rights Reserved
//
//  module: object.h
//  date:
//  author:
//
//  purpose:
//
//----------------------------------------------------------------------------
#ifndef __OBJECT_H__
#define __OBJECT_H__

#ifndef RC_INVOKED
#pragma message ("INCLUDING OBJECT.H from " __FILE__)
#endif  /* RC_INVOKED */


/////////////////////////////////////////////////////////////////////////////

class CHandlerObject
{
public:
  CHandlerObject(LPUNKNOWN pUnkOuter);
  ~CHandlerObject();

  HRESULT CreateObject(REFCLSID rclsid, REFIID riid, LPVOID FAR* ppv);

private:
//----------------------------------------------------------------------------
// IUnknown implementation
//----------------------------------------------------------------------------
  struct CUnknown : IUnknown
  {
	CUnknown(CHandlerObject *pObject) { m_pObject = pObject; }

	//*** IUnknown ***
	STDMETHOD(QueryInterface) (REFIID riid, LPVOID FAR* ppvObj);
	STDMETHOD_(ULONG,AddRef) (VOID);
	STDMETHOD_(ULONG,Release) (VOID);

  private:
	CHandlerObject * m_pObject;
  };
  friend CUnknown;
  CUnknown m_Unknown;

//----------------------------------------------------------------------------
// IPersistStorage implementation
//----------------------------------------------------------------------------
  struct CPersistStorage : IPersistStorage
  {
	CPersistStorage(CHandlerObject *pObject) { m_pObject = pObject; }

	//*** IUnknown ***
	STDMETHOD(QueryInterface) (REFIID riid, LPVOID FAR* ppvObj)
	  { return m_pObject->m_pUnkOuter->QueryInterface(riid, ppvObj); }
	STDMETHOD_(ULONG,AddRef) (VOID)
	  { return m_pObject->m_pUnkOuter->AddRef(); }
	STDMETHOD_(ULONG,Release) (VOID)
	  { return m_pObject->m_pUnkOuter->Release(); }

	// *** IPersist methods ***
	STDMETHOD(GetClassID) (LPCLSID lpClassID);

	// *** IPersistStorage methods ***
	STDMETHOD(IsDirty) (VOID);
	STDMETHOD(InitNew) (LPSTORAGE pStg);
	STDMETHOD(Load) (LPSTORAGE pStg);
	STDMETHOD(Save) (LPSTORAGE pStgSave, BOOL fSameAsLoad);
	STDMETHOD(SaveCompleted) (LPSTORAGE pStgNew);
	STDMETHOD(HandsOffStorage) (VOID);

  private:
	CHandlerObject * m_pObject;
  };
  friend CPersistStorage;
  CPersistStorage m_PersistStorage;

//----------------------------------------------------------------------------
// IOleObject implementation
//----------------------------------------------------------------------------
  struct COleObject : IOleObject
  {
	COleObject(CHandlerObject *pObject) { m_pObject = pObject; }

	//*** IUnknown ***
	STDMETHOD(QueryInterface) (REFIID riid, LPVOID FAR* ppvObj)
	  { return m_pObject->m_pUnkOuter->QueryInterface(riid, ppvObj); }
	STDMETHOD_(ULONG,AddRef) (VOID)
	  { return m_pObject->m_pUnkOuter->AddRef(); }
	STDMETHOD_(ULONG,Release) (VOID)
	  { return m_pObject->m_pUnkOuter->Release(); }

	// *** IOleObject methods ***
	STDMETHOD(SetClientSite) (LPOLECLIENTSITE pClientSite);
	STDMETHOD(GetClientSite) (LPOLECLIENTSITE FAR* ppClientSite);
	STDMETHOD(SetHostNames) (LPCSTR szContainerApp, LPCSTR szContainerObj);
	STDMETHOD(Close) (DWORD dwSaveOption);
	STDMETHOD(SetMoniker) (DWORD dwWhichMoniker, LPMONIKER pmk);
	STDMETHOD(GetMoniker) (DWORD dwAssign, DWORD dwWhichMoniker,
						   LPMONIKER FAR* ppmk);
	STDMETHOD(InitFromData) (LPDATAOBJECT pDataObject, BOOL fCreation,
							 DWORD dwReserved);
	STDMETHOD(GetClipboardData) (DWORD dwReserved, LPDATAOBJECT FAR* ppDObj);
	STDMETHOD(DoVerb) (LONG iVerb, LPMSG lpmsg, LPOLECLIENTSITE pActiveSite,
					   LONG lindex, HWND hwndParent, LPCRECT lprcPosRect);
	STDMETHOD(EnumVerbs) (IEnumOLEVERB FAR* FAR* ppenumOleVerb);
	STDMETHOD(Update) (VOID);
	STDMETHOD(IsUpToDate) (VOID);
	STDMETHOD(GetUserClassID) (CLSID FAR* pClsid);
	STDMETHOD(GetUserType) (DWORD dwFormOfType, LPSTR FAR* pszUserType);
	STDMETHOD(SetExtent) (DWORD dwDrawAspect, LPSIZEL lpsizel);
	STDMETHOD(GetExtent) (DWORD dwDrawAspect, LPSIZEL lpsizel);
	STDMETHOD(Advise) (IAdviseSink FAR* pAdvSink, DWORD FAR* pdwConnection);
	STDMETHOD(Unadvise)(DWORD dwConnection);
	STDMETHOD(EnumAdvise) (LPENUMSTATDATA FAR* ppenumAdvise);
	STDMETHOD(GetMiscStatus) (DWORD dwAspect, DWORD FAR* pdwStatus);
	STDMETHOD(SetColorScheme) (LPLOGPALETTE lpLogpal);

  private:
	CHandlerObject * m_pObject;
  };
  friend COleObject;
  COleObject m_OleObject;

//----------------------------------------------------------------------------
// IDataObject implementation
//----------------------------------------------------------------------------
  struct CDataObject : IDataObject
  {
	CDataObject(CHandlerObject *pObject) { m_pObject = pObject; }

	//*** IUnknown ***
	STDMETHOD(QueryInterface) (REFIID riid, LPVOID FAR* ppvObj)
	  { return m_pObject->m_pUnkOuter->QueryInterface(riid, ppvObj); }
	STDMETHOD_(ULONG,AddRef) (VOID)
	  { return m_pObject->m_pUnkOuter->AddRef(); }
	STDMETHOD_(ULONG,Release) (VOID)
	  { return m_pObject->m_pUnkOuter->Release(); }

	//*** IDataObject ***
	STDMETHOD(GetData) (LPFORMATETC pformatetcIn, LPSTGMEDIUM pmedium);
	STDMETHOD(GetDataHere) (LPFORMATETC pformatetc, LPSTGMEDIUM pmedium);
	STDMETHOD(QueryGetData) (LPFORMATETC pformatetc);
	STDMETHOD(GetCanonicalFormatEtc) (LPFORMATETC petcin, LPFORMATETC petcout);
	STDMETHOD(SetData) (LPFORMATETC pformatetc, STGMEDIUM FAR * pmedium,
						BOOL fRelease);
	STDMETHOD(EnumFormatEtc) (DWORD dwDirection, LPENUMFORMATETC FAR* ppenum);
	STDMETHOD(DAdvise) (LPFORMATETC pFormatetc, DWORD advf,
					   LPADVISESINK pAdvSink, LPDWORD pdwConnection);
	STDMETHOD(DUnadvise) (DWORD dwConnection);
	STDMETHOD(EnumDAdvise) (LPENUMSTATDATA FAR* ppenumAdvise);

  private:
	CHandlerObject * m_pObject;
  };
  friend CDataObject;
  CDataObject m_DataObject;

//----------------------------------------------------------------------------
// IViewObject2 implementation
//----------------------------------------------------------------------------
  struct CViewObject2 : IViewObject2
  {
	CViewObject2(CHandlerObject *pObject) { m_pObject = pObject; }

	//*** IUnknown ***
	STDMETHOD(QueryInterface) (REFIID riid, LPVOID FAR* ppvObj)
	  { return m_pObject->m_pUnkOuter->QueryInterface(riid, ppvObj); }
	STDMETHOD_(ULONG,AddRef) (VOID)
	  { return m_pObject->m_pUnkOuter->AddRef(); }
	STDMETHOD_(ULONG,Release) (VOID)
	  { return m_pObject->m_pUnkOuter->Release(); }

	STDMETHOD(Draw) (DWORD dwDrawAspect, LONG lindex,
					 void FAR* pvAspect, DVTARGETDEVICE FAR * ptd,
					 HDC hicTargetDev,
					 HDC hdcDraw,
					 LPCRECTL lprcBounds,
					 LPCRECTL lprcWBounds,
					 BOOL (CALLBACK * pfnContinue) (DWORD),
					 DWORD dwContinue);
	STDMETHOD(GetColorSet) (DWORD dwDrawAspect, LONG lindex,
							void FAR* pvAspect, DVTARGETDEVICE FAR * ptd,
							HDC hicTargetDev,
							LPLOGPALETTE FAR* ppColorSet);
	STDMETHOD(Freeze) (DWORD dwDrawAspect, LONG lindex, void FAR* pvAspect,
					   DWORD FAR* pdwFreeze);
	STDMETHOD(Unfreeze) (DWORD dwFreeze);
	STDMETHOD(SetAdvise) (DWORD aspects, DWORD advf, LPADVISESINK pAdvSink);
	STDMETHOD(GetAdvise) (DWORD FAR* pAspects, DWORD FAR* pAdvf,
						  LPADVISESINK FAR* ppAdvSink);

	// *** IViewObject2 methods ***
	STDMETHOD(GetExtent) (DWORD dwDrawAspect, LONG lindex,
						  DVTARGETDEVICE FAR * ptd, LPSIZEL lpsizel);
  private:
	CHandlerObject * m_pObject;
  };
  friend CViewObject2;
  CViewObject2 m_ViewObject2;

//----------------------------------------------------------------------------
// IOleCache2 implementation
//----------------------------------------------------------------------------
  struct COleCache2 : IOleCache2
  {
	COleCache2(CHandlerObject *pObject) { m_pObject = pObject; }

	//*** IUnknown ***
	STDMETHOD(QueryInterface) (REFIID riid, LPVOID FAR* ppvObj)
	  { return m_pObject->m_pUnkOuter->QueryInterface(riid, ppvObj); }
	STDMETHOD_(ULONG,AddRef) (VOID)
	  { return m_pObject->m_pUnkOuter->AddRef(); }
	STDMETHOD_(ULONG,Release) (VOID)
	  { return m_pObject->m_pUnkOuter->Release(); }

	// *** IOleCache methods ***
	STDMETHOD(Cache) (LPFORMATETC lpFormatetc, DWORD advf,
					  LPDWORD lpdwConnection);
	STDMETHOD(Uncache) (DWORD dwConnection);
	STDMETHOD(EnumCache) (LPENUMSTATDATA FAR* ppenumStatData);
	STDMETHOD(InitCache) (LPDATAOBJECT pDataObject);
	STDMETHOD(SetData) (LPFORMATETC pformatetc, STGMEDIUM FAR * pmedium,
						BOOL fRelease);

	// *** IOleCache2 methods ***
	STDMETHOD(UpdateCache) (LPDATAOBJECT pDataObject, DWORD grfUpdf,
										  LPVOID pReserved);
	STDMETHOD(DiscardCache) (DWORD dwDiscardOptions);

  private:
	CHandlerObject * m_pObject;
  };
  friend COleCache2;
  COleCache2 m_OleCache2;

//----------------------------------------------------------------------------
// IRunnableObject implementation
//----------------------------------------------------------------------------
  struct CRunnableObject : IRunnableObject
  {
	CRunnableObject(CHandlerObject *pObject) { m_pObject = pObject; }

	//*** IUnknown ***
	STDMETHOD(QueryInterface) (REFIID riid, LPVOID FAR* ppvObj)
	  { return m_pObject->m_pUnkOuter->QueryInterface(riid, ppvObj); }
	STDMETHOD_(ULONG,AddRef) (VOID)
	  { return m_pObject->m_pUnkOuter->AddRef(); }
	STDMETHOD_(ULONG,Release) (VOID)
	  { return m_pObject->m_pUnkOuter->Release(); }

	// *** IRunnableObject methods ***
	STDMETHOD(GetRunningClass) (LPCLSID lpClsid);
	STDMETHOD(Run) (LPBINDCTX pbc);
	STDMETHOD_(BOOL, IsRunning) ();
	STDMETHOD(LockRunning)(BOOL fLock, BOOL fLastUnlockCloses);
	STDMETHOD(SetContainedObject)(BOOL fContained);

  private:
	CHandlerObject * m_pObject;
  };
  friend CRunnableObject;
  CRunnableObject m_RunnableObject;

//----------------------------------------------------------------------------
// IExternalConnection implementation
//----------------------------------------------------------------------------
  struct CExternalConnection : IExternalConnection
  {
	CExternalConnection(CHandlerObject *pObject) { m_pObject = pObject; }

	//*** IUnknown ***
	STDMETHOD(QueryInterface) (REFIID riid, LPVOID FAR* ppvObj)
	  { return m_pObject->m_pUnkOuter->QueryInterface(riid, ppvObj); }
	STDMETHOD_(ULONG,AddRef) (VOID)
	  { return m_pObject->m_pUnkOuter->AddRef(); }
	STDMETHOD_(ULONG,Release) (VOID)
	  { return m_pObject->m_pUnkOuter->Release(); }

	// *** IExternalConnection methods ***
	STDMETHOD_(DWORD, AddConnection) (DWORD extconn, DWORD reserved);
	STDMETHOD_(DWORD, ReleaseConnection) (DWORD extconn, DWORD reserved,
										  BOOL fLastReleaseCloses);

  private:
	CHandlerObject * m_pObject;
  };
  friend CExternalConnection;
  CExternalConnection m_ExternalConnection;

//----------------------------------------------------------------------------
// IStdMarshalInfo implementation
//----------------------------------------------------------------------------
  struct CStdMarshalInfo : IStdMarshalInfo
  {
	CStdMarshalInfo(CHandlerObject *pObject) { m_pObject = pObject; }

	//*** IUnknown ***
	STDMETHOD(QueryInterface) (REFIID riid, LPVOID FAR* ppvObj)
	  { return m_pObject->m_pUnkOuter->QueryInterface(riid, ppvObj); }
	STDMETHOD_(ULONG,AddRef) (VOID)
	  { return m_pObject->m_pUnkOuter->AddRef(); }
	STDMETHOD_(ULONG,Release) (VOID)
	  { return m_pObject->m_pUnkOuter->Release(); }

	// *** IStdMarshalInfo methods ***
	STDMETHOD(GetClassForHandler)(DWORD dwDestContext,
								  LPVOID pvDestContext, LPCLSID pClsid);
  private:
	CHandlerObject * m_pObject;
  };
  friend CStdMarshalInfo;
  CStdMarshalInfo m_StdMarshalInfo;

//----------------------------------------------------------------------------
// shared features common to all implementations
//----------------------------------------------------------------------------
private:
  ULONG m_dwRefs;
  LPUNKNOWN m_pDefHandler, m_pUnkOuter;
  LPSTORAGE m_pStg;
  LPPERSISTSTORAGE m_pPersistStorage;
  LPOLEOBJECT m_pOleObject;
  LPDATAOBJECT m_pDataObject;
  LPVIEWOBJECT2 m_pViewObject2;
  LPOLECACHE2 m_pOleCache2;
  LPRUNNABLEOBJECT m_pRunnableObject;
  LPEXTERNALCONNECTION m_pExternalConnection;
  LPSTDMARSHALINFO m_pStdMarshalInfo;

  CLSID m_ClsId;
  CString m_szClassString;
  CHandlerWindow* m_pHandlerWindow;
  char m_szFileName[144];
  CListBox* m_pListBox;

}; /* CHandlerObject */

/////////////////////////////////////////////////////////////////////////////

#endif // __DBGHNDLR_H__
