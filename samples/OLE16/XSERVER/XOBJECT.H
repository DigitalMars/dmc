//----------------------------------------------------------------------------
//
// (c) Copyright Microsoft Corp. 1993 All Rights Reserved
//
//  module: xobject.h
//  date:
//  author: tonykit
//
//  purpose:
//
//----------------------------------------------------------------------------
#ifndef __XOBJECT_H__
#define __XOBJECT_H__

#ifndef RC_INVOKED
#pragma message ("INCLUDING XOBJECT.H from " __FILE__)
#endif  /* RC_INVOKED */


/////////////////////////////////////////////////////////////////////////////

class CXObject
{
public:
  CXObject(LPUNKNOWN pUnkOuter);
  ~CXObject();

  STDMETHOD(CreateObject) (REFCLSID rclsid, REFIID riid, LPVOID FAR* ppv);

  STDMETHOD_(VOID,SendViewChange) (DWORD aspects);
  STDMETHOD(HandsOffStorage) (VOID);
  STDMETHOD(RememberStorage) (LPSTORAGE pStg, BOOL bCheckInitial = TRUE);
  STDMETHOD(LoadNativeData) (LPSTORAGE pStg);
  STDMETHOD(SaveNativeData) (LPSTORAGE pStg, BOOL fSameAsLoad);
  STDMETHOD(SaveCompleted) (LPSTORAGE pStg);

  STDMETHOD(SetClientSite) (LPOLECLIENTSITE pClientSite);
  STDMETHOD(Close) (DWORD dwSaveOption);
  STDMETHOD(GetMoniker) (DWORD dwAssign, DWORD dwWhichMoniker,
						 LPMONIKER FAR* ppmk);
  STDMETHOD(SetRunningState) (BOOL bRun);
  STDMETHOD(DoVerb) (LONG iVerb, LPMSG lpmsg, LPOLECLIENTSITE pActiveSite,
					 LONG lindex, HWND hwndParent, LPCRECT lprcPosRect);
  STDMETHOD(Update) (VOID);
  STDMETHOD(SendChangeNotification) (VOID);

  STDMETHOD(SetExtent) (LPSIZEL psizel);
  STDMETHOD(GetExtent) (DWORD dwDrawAspect, LONG lindex,
						DVTARGETDEVICE FAR* ptd, LPSIZEL lpsizel);

  STDMETHOD(GetData) (LPFORMATETC petc, LPSTGMEDIUM pmed);
  STDMETHOD(QueryGetData) (LPFORMATETC petc);
  STDMETHOD(GetCanonicalFormatEtc)(LPFORMATETC petcIn, LPFORMATETC petcOut);
  STDMETHOD(Draw) (DWORD dwDrawAspect, LONG lindex, void FAR* pvAspect,
				   DVTARGETDEVICE FAR * ptd, HDC hicTargetDev, HDC hdcDraw,
				   LPCRECTL lprcBounds, LPCRECTL lprcWBounds,
				   BOOL (CALLBACK * pfnContinue) (DWORD), DWORD dwContinue);
  STDMETHOD(FastDraw) (HDC hDC, long left, long top, long right, long bottom);
  STDMETHOD(DAdvise)(LPFORMATETC pFormatetc, DWORD advf, LPADVISESINK pAdvSink,
					 LPDWORD pdwConnection);
  STDMETHOD(RegisterAsRunning) (VOID);
  STDMETHOD(Cache) (LPFORMATETC petc, DWORD advf, LPDWORD lpdwConnection);
  STDMETHOD(Uncache) (DWORD dwConnection);
  STDMETHOD(EnumCache) (LPENUMSTATDATA FAR* ppenumStatData);
  STDMETHOD(SetData) (LPFORMATETC pformatetc, STGMEDIUM FAR * pmedium,
					  BOOL fRelease);

private:
//----------------------------------------------------------------------------
// IUnknown implementation
//----------------------------------------------------------------------------
  struct CUnknown : IUnknown
  {
	CUnknown(CXObject *pObject) { m_pObject = pObject; }

	//*** IUnknown ***
	STDMETHOD(QueryInterface) (REFIID riid, LPVOID FAR* ppvObj);
	STDMETHOD_(ULONG,AddRef) (VOID);
	STDMETHOD_(ULONG,Release) (VOID);

  private:
	CXObject * m_pObject;
  };

  DECLARE_NC(CXObject, CUnknown)
  CUnknown m_Unknown;

//----------------------------------------------------------------------------
// IPersistStorage implementation
//----------------------------------------------------------------------------
  struct CPersistStorage : IPersistStorage
  {
	CPersistStorage(CXObject *pObject) { m_pObject = pObject; }

	//*** IUnknown ***
	STDMETHOD(QueryInterface) (REFIID riid, LPVOID FAR* ppvObj);
	STDMETHOD_(ULONG,AddRef) (VOID);
	STDMETHOD_(ULONG,Release) (VOID);

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
	CXObject * m_pObject;
  };

  DECLARE_NC(CXObject, CPersistStorage)
  CPersistStorage m_PersistStorage;

//----------------------------------------------------------------------------
// IOleObject implementation
//----------------------------------------------------------------------------
  struct COleObject : IOleObject
  {
	COleObject(CXObject *pObject) { m_pObject = pObject; }

	//*** IUnknown ***
	STDMETHOD(QueryInterface) (REFIID riid, LPVOID FAR* ppvObj);
	STDMETHOD_(ULONG,AddRef) (VOID);
	STDMETHOD_(ULONG,Release) (VOID);

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
	CXObject * m_pObject;
  };

  DECLARE_NC(CXObject, COleObject)
  COleObject m_OleObject;

//----------------------------------------------------------------------------
// IDataObject implementation
//----------------------------------------------------------------------------
  struct CDataObject : IDataObject
  {
	CDataObject(CXObject *pObject) { m_pObject = pObject; }

	//*** IUnknown ***
	STDMETHOD(QueryInterface) (REFIID riid, LPVOID FAR* ppvObj);
	STDMETHOD_(ULONG,AddRef) (VOID);
	STDMETHOD_(ULONG,Release) (VOID);

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
	CXObject * m_pObject;
  };

  DECLARE_NC(CXObject, CDataObject)
  CDataObject m_DataObject;

//----------------------------------------------------------------------------
// IViewObject implementation
//----------------------------------------------------------------------------
  struct CViewObject2 : IViewObject2
  {
	CViewObject2(CXObject *pObject) { m_pObject = pObject; }

	//*** IUnknown ***
	STDMETHOD(QueryInterface) (REFIID riid, LPVOID FAR* ppvObj);
	STDMETHOD_(ULONG,AddRef) (VOID);
	STDMETHOD_(ULONG,Release) (VOID);

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
	STDMETHOD(GetExtent) (DWORD dwDrawAspect, LONG lindex,
						  DVTARGETDEVICE FAR * ptd, LPSIZEL lpsizel);

  private:
	CXObject * m_pObject;
  };

  DECLARE_NC(CXObject, CViewObject2)
  CViewObject2 m_ViewObject2;

//----------------------------------------------------------------------------
// IOleCache2 implementation
//----------------------------------------------------------------------------
  struct COleCache2 : IOleCache2
  {
	COleCache2(CXObject *pObject) { m_pObject = pObject; }

	//*** IUnknown ***
	STDMETHOD(QueryInterface) (REFIID riid, LPVOID FAR* ppvObj);
	STDMETHOD_(ULONG,AddRef) (VOID);
	STDMETHOD_(ULONG,Release) (VOID);

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
	CXObject * m_pObject;
  };

  DECLARE_NC(CXObject, COleCache2)
  COleCache2 m_OleCache2;

//----------------------------------------------------------------------------
// IRunnableObject implementation
//----------------------------------------------------------------------------
  struct CRunnableObject : IRunnableObject
  {
	CRunnableObject(CXObject *pObject) { m_pObject = pObject; }

	//*** IUnknown ***
	STDMETHOD(QueryInterface) (REFIID riid, LPVOID FAR* ppvObj);
	STDMETHOD_(ULONG,AddRef) (VOID);
	STDMETHOD_(ULONG,Release) (VOID);

	// *** IRunnableObject methods ***
	STDMETHOD(GetRunningClass) (LPCLSID lpClsid);
	STDMETHOD(Run) (LPBINDCTX pbc);
	STDMETHOD_(BOOL, IsRunning) ();
	STDMETHOD(LockRunning)(BOOL fLock, BOOL fLastUnlockCloses);
	STDMETHOD(SetContainedObject)(BOOL fContained);

  private:
	CXObject * m_pObject;
  };

  DECLARE_NC(CXObject, CRunnableObject)
  CRunnableObject m_RunnableObject;

//----------------------------------------------------------------------------
// IExternalConnection implementation
//----------------------------------------------------------------------------
  struct CExternalConnection : IExternalConnection
  {
	CExternalConnection(CXObject *pObject) { m_pObject = pObject; }

	//*** IUnknown ***
	STDMETHOD(QueryInterface) (REFIID riid, LPVOID FAR* ppvObj);
	STDMETHOD_(ULONG,AddRef) (VOID);
	STDMETHOD_(ULONG,Release) (VOID);

	// *** IExternalConnection methods ***
	STDMETHOD_(DWORD, AddConnection) (DWORD extconn, DWORD reserved);
	STDMETHOD_(DWORD, ReleaseConnection) (DWORD extconn, DWORD reserved,
										  BOOL fLastReleaseCloses);

  private:
	CXObject * m_pObject;
  };

  DECLARE_NC(CXObject, CExternalConnection)
  CExternalConnection m_ExternalConnection;

//----------------------------------------------------------------------------
// IStdMarshalInfo implementation
//----------------------------------------------------------------------------
  struct CStdMarshalInfo : IStdMarshalInfo
  {
	CStdMarshalInfo(CXObject *pObject) { m_pObject = pObject; }

	//*** IUnknown ***
	STDMETHOD(QueryInterface) (REFIID riid, LPVOID FAR* ppvObj);
	STDMETHOD_(ULONG,AddRef) (VOID);
	STDMETHOD_(ULONG,Release) (VOID);

	// *** IStdMarshalInfo methods ***
	STDMETHOD(GetClassForHandler)(DWORD dwDestContext,
								  LPVOID pvDestContext, LPCLSID pClsid);
  private:
	CXObject * m_pObject;
  };

  DECLARE_NC(CXObject, CStdMarshalInfo)
  CStdMarshalInfo m_StdMarshalInfo;

//----------------------------------------------------------------------------
// IDispatch implementation
//----------------------------------------------------------------------------
  struct CDispatch : IDispatch
  {
	CDispatch(CXObject *pObject);
	~CDispatch();

	//*** IUnknown ***
	STDMETHOD(QueryInterface) (REFIID riid, LPVOID FAR* ppvObj);
	STDMETHOD_(ULONG,AddRef) (VOID);
	STDMETHOD_(ULONG,Release) (VOID);

	// *** IDispatch methods ***
	STDMETHOD(GetTypeInfoCount)(unsigned int FAR* pctinfo);
	STDMETHOD(GetTypeInfo)(unsigned int itinfo, LCID lcid,
						   ITypeInfo FAR* FAR* pptinfo);
	STDMETHOD(GetIDsOfNames)(REFIID riid, TCHAR FAR* FAR* rgszNames,
							 unsigned int cNames, LCID lcid,
							 DISPID FAR* rgdispid);
	STDMETHOD(Invoke)(DISPID dispidMember, REFIID riid, LCID lcid,
					  unsigned short wFlags, DISPPARAMS FAR* pdispparams,
					  VARIANT FAR* pvarResult, EXCEPINFO FAR* pexcepinfo,
					  unsigned int FAR* puArgErr);

	// *** exported IDispatch methods ***
	STDMETHOD(FastDraw)(HDC hDC, long left, long top, long right, long bottom);
	STDMETHOD(SetExtent)(long cx, long cy);
	STDMETHOD(GetExtent)(long FAR *pcx, long FAR *pcy);
	STDMETHOD(ShowObject)(BOOL fShow);

  private:
	ITypeInfo FAR* m_ptinfo;
	CXObject * m_pObject;
  };

  DECLARE_NC(CXObject, CDispatch)
  CDispatch m_Dispatch;

//----------------------------------------------------------------------------
// shared features common to all implementations
//----------------------------------------------------------------------------
private:
  ULONG m_dwRefs;
  LPUNKNOWN m_pUnkOuter;
  LPSTORAGE m_pStg;
  LPOLECLIENTSITE m_pClientSite;
  CLSID m_ClsId;

  LPVIEWOBJECT2 m_pViewObject2;
  LPOLECACHE2 m_pOleCache2;
  LPDATAOBJECT m_pDataObject;
  LPPERSISTSTORAGE m_pPersistStg;
  LPOLEADVISEHOLDER m_pOleAdviseHolder;
  LPDATAADVISEHOLDER m_pDataAdviseHolder;

  DWORD m_dwAspects;
  DWORD m_dwStrong, m_dwRegister;
  DWORD m_advf;
  LPADVISESINK m_pViewAdviseSink;
  BOOL m_bIsDirty, m_bIsRunning, m_bDebug, m_bIsLocked;
  BOOL m_fNoScribbleMode, m_fSaveWithSameAsLoad;
  CLIPFORMAT m_cfFormat;

  SIZEL m_Size;
  LOGFONT m_lf;

}; /* CXObject
	*/

/////////////////////////////////////////////////////////////////////////////

#endif // __XOBJECT_H__
