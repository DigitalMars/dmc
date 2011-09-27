/*
	dataview.h

	This file contains the principle class declarations for the dataview sample.
	Note the use of nested classes to contain OLE2 interfaces within a single
	C++ object.

	(c) Copyright Microsoft Corp. 1992 - 1993 All Rights Reserved
*/

#ifndef __DATAVIEW_H
#define __DATAVIEW_H

#include <windows.h>
#include <ole2.h>
#ifdef __TURBOC__
#define classmodel _huge
#else
#define classmodel FAR
#endif

#define DV_APPNAME "Simulated Stock Watcher"
#ifndef RC_INVOKED
#pragma warning ( disable : 4355 )
/*
 *      Warning 4355 warns against using the *this* pointer in a
 *      constructor.  Since we do this frequently, and it is harmless,
 *      we disable the warning.
 */
#endif  /* RC_INVOKED */
#define UNREFERENCED(a) ((void)(a))

/*
 *      The DATAVIEW server is an example of a server implemented as a
 *      DLL.  An embedded DATAVIEW object displays a simulated stock
 *      name and price, and the display is in black text if the stock
 *      price is below some threshold value and in red if the price is
 *      above the threshold.  When the stock value increases past the
 *      threshold, the object optionally beeps.
 *
 *      Part of the data of a DATAVIEW object is a new moniker type which
 *      allows the DATAVIEW object to bind to a REALTIME object, which is an
 *      object that simulates a realtime stock service. The implementation of
 *      the REALTIME objects uses a random number generator, and is in a
 *      separate executable realtime.exe; the objects support the IDataObject
 *      interface but not the IOleObject or IViewObject interfaces, and so
 *      they are not strictly speaking OLE objects. The DATAVIEW objects get
 *      notifications of changes in the REALTIME objects through the OLE
 *      notification mechanism, and they get data from the REALTIME objects
 *      through IDataObject::GetData.
 *
 *      This example supports a division between view and content.  The
 *      view state includes the threshold value and whether there is a
 *      beep when the threshold value is crossed, and the data includes
 *      the value of the simulated stock price.  If there are two
 *      embedded data view objects that connect to the same stock item,
 *      then the threshold values may be different, and hence the
 *      display color may be different, but the underlying stock value
 *      will be the same for both objects.  In this case there are two
 *      independent view states and a common, shared stock value.
 */

/*
 *      DllGetClassObject must be exported by every DLL-based OLE
 *      server.  The OLE code finds the class factory by calling
 *      DllGetClassObject for DLLs.  The class factory does NOT have to
 *      be registered in the DLL case, since OLE does not look for the
 *      class factory in the global class factory table (as it does
 *      when a server is implemented as a separate .exe file).
 */

extern DWORD g_dwObjectCount;

STDAPI  DllGetClassObject(REFCLSID clsid, REFIID iid, void FAR* FAR* ppv);
STDAPI  DllCanUnloadNow(void);

// Default to enable use of static form of OLE2UI library
//  This can be overriden by setting STATIC_OLE2UI=0 in the MAKEFILE.
#if !defined( STATIC_OLE2UI )
#define STATIC_OLE2UI 1
#endif

/*
 *      This sample illustrates using inheritance to share the
 *      frequently repetitious code in the implementations of AddRef,
 *      Release, and QueryInterface.  Classes derived from this will
 *      generally not have to reimplement AddRef and Release.
 *
 *      In order for the implementation of Release in the base class
 *      CControllingUnknown to implement deletion of the object when
 *      the reference count goes to zero, the destructor must be
 *      virtual.
 *
 *      The class supports aggregation by accepting an LPUNKNOWN
 *      parameter in the constructor and delegating to it if
 *      appropriate.
 *
 *      Derived classes will generally not need to re-implement AddRef
 *      and Release, but will need to re-implement QueryInterface.  In
 *      most cases, the implementation of QueryInterface will be a call
 *      to QueryInterfaceFromData with a pointer to the MapIIDToIFace
 *      array for that class.
 *
 *      The Realtime code makes more use of this capability than dataview.
 */

/*
 *      Coding conventions:   m_ refers to a member variable
 */

class classmodel CControllingUnknown :  public IUnknown
{
protected:
	CControllingUnknown( LPUNKNOWN pUnkOuter );
public:
	STDMETHOD(QueryInterface) (REFIID riid, LPVOID FAR* ppvObj);
	STDMETHOD_(ULONG,AddRef) (void);
	STDMETHOD_(ULONG,Release) (void);
	virtual     ~CControllingUnknown() {};
private:
	ULONG           m_refs;
	LPUNKNOWN       m_pUnkOuter;
};


class classmodel CDataViewFactory :  public IClassFactory
{
	static LPCLASSFACTORY Create(void);
	CDataViewFactory() : m_refs(0) { g_dwObjectCount++ ; };
	~CDataViewFactory() { g_dwObjectCount-- ; };

	STDMETHOD(QueryInterface) (REFIID riid, LPVOID FAR* ppvObj);
	STDMETHOD_(ULONG,AddRef) (void);
	STDMETHOD_(ULONG,Release) (void);
	STDMETHOD(CreateInstance)(IUnknown FAR* pUnkOuter, REFIID iidInterface, void FAR* FAR* ppvObject);
	STDMETHOD(LockServer)( BOOL fLock );

	friend HRESULT STDAPICALLTYPE DllGetClassObject(REFCLSID, REFIID, void FAR* FAR*);

	ULONG m_refs;
};


class classmodel CDataView: public CControllingUnknown
{
	static CDataView FAR * Create( LPUNKNOWN pUnkOuter, LPMONIKER pmkDataService );
	CDataView( LPUNKNOWN pUnkOuter);
	virtual ~CDataView();
	STDMETHOD(QueryInterface) (REFIID riid, LPVOID FAR* ppvObj);

	//these methods are defined in dvstock.cpp

	struct StockInfo {
		StockInfo( CDataView FAR *pDataView );      //constructor
		~StockInfo( void );

		HRESULT Save( LPSTREAM );   //save the StockInfo to a stream
		HRESULT Load( LPSTREAM );   //load StockInfo data from a stream

		void    SetPriceLimit( long nLimit, BOOL fBeep );       //in 1/8ths dollars
		long    GetPriceLimit( void );                          //in 1/8ths dollars

		BOOL    SetStockName( LPSTR lpszName, BOOL fSetServiceName );
		LPSTR   GetStockName( void );

		LPSTR   GetServiceName( void );
		BOOL    SetServiceName( LPSTR lpszService );

		BOOL    SetRealTimeParam( int Jitter, int UpdInt );
		BOOL    GetRealTimeParam( LPINT pJitter, LPINT pUpdInt );

		BOOL    GetMode( void );            //0 is automatic, 1 manual
		BOOL    SetAutoMode( void );
		BOOL    SetManualMode( void );
		BOOL    GetBeep( void );            //0 no beep, 1 beep on alarm mode

		void    Draw( HDC hdcDraw, LPCRECTL lprcBounds );       //Draw ourselves into an HDC
		int     UpdateDisplayText( void );  //&&set bouding boxes, etc.

		LPSTR       m_lpszText;             //text of the stock *number*
		LPMONIKER   m_pmkDataSource;
		private:
			LPSTR m_lpszDisplayText;    //the string that gets displayed to the user
			struct SaveSet {
				char szService[128];        // usually "@Realtime"
				char szName[128];

				long nLimit;                //the price limit of the stock (alarm mode if exceeded)
				BOOL fManual;               //automatic or manual updates of stock data
				BOOL fAlarm;                //was the last stock price over the limit?
				BOOL fBeep;                 //beep upon entering alarm mode?
			} ss;
			CDataView FAR *m_pDataView;
	};

	friend struct StockInfo;
	//this must be accessible from the outside world (for the dialog)
	public:
		StockInfo m_stockInfo;
	private:

	struct COleObject :  IOleObject
	{
	COleObject( CDataView FAR * pDataView);

		STDMETHOD(QueryInterface) (REFIID riid, LPVOID FAR* ppvObj);
		STDMETHOD_(ULONG,AddRef) (void);
		STDMETHOD_(ULONG,Release) (void);

		STDMETHOD(SetClientSite) (LPOLECLIENTSITE pClientSite);
		STDMETHOD(GetClientSite) (LPOLECLIENTSITE FAR* ppClientSite);
		STDMETHOD(SetHostNames) (LPCSTR szContainerApp, LPCSTR szContainerObj);
		STDMETHOD(Close) (DWORD dwSaveOption);
		STDMETHOD(SetMoniker) (DWORD dwWhichMoniker, LPMONIKER pmk);
		STDMETHOD(GetMoniker) (DWORD dwAssign, DWORD dwWhichMoniker,
			   LPMONIKER FAR* ppmk);
		STDMETHOD(InitFromData) (LPDATAOBJECT pDataObject,
			BOOL fCreation,
			DWORD dwReserved);
		STDMETHOD(GetClipboardData) (DWORD dwReserved,
			LPDATAOBJECT FAR* ppDataObject);
		STDMETHOD(DoVerb) (LONG iVerb,
			LPMSG lpmsg,
			LPOLECLIENTSITE pActiveSite,
			LONG lindex,
			HWND hwndParent,
			LPCRECT lprcPosRect);
		STDMETHOD(EnumVerbs) (IEnumOLEVERB FAR* FAR* ppenumOleVerb);
		STDMETHOD(Update) (void);
		STDMETHOD(IsUpToDate) (void);
		STDMETHOD(GetUserClassID) (CLSID FAR* pClsid);
		STDMETHOD(GetUserType) (DWORD dwFormOfType, LPSTR FAR* pszUserType);
		STDMETHOD(SetExtent) (DWORD dwDrawAspect, LPSIZEL lpsizel);
		STDMETHOD(GetExtent) (DWORD dwDrawAspect, LPSIZEL lpsizel);

		STDMETHOD(Advise)(IAdviseSink FAR* pAdvSink, DWORD FAR* pdwConnection);
		STDMETHOD(Unadvise)(DWORD dwConnection);
		STDMETHOD(EnumAdvise) (LPENUMSTATDATA FAR* ppenumAdvise);
		STDMETHOD(GetMiscStatus) (DWORD dwAspect, DWORD FAR* pdwStatus);
		STDMETHOD(SetColorScheme) (LPLOGPALETTE lpLogpal);
	private:
		CDataView FAR * m_pDataView;
	};

	friend struct COleObject;
	COleObject m_oleObject;

	struct CPersistStorage : IPersistStorage
	{
	CPersistStorage( CDataView FAR * pDataView );

		STDMETHOD(QueryInterface) (REFIID riid, LPVOID FAR* ppvObj);
		STDMETHOD_(ULONG,AddRef) (void);
		STDMETHOD_(ULONG,Release) (void);

		STDMETHOD(GetClassID) (LPCLSID lpClassID);

		STDMETHOD(IsDirty) (void);
		STDMETHOD(InitNew) (LPSTORAGE pStg);
		STDMETHOD(Load) (LPSTORAGE pStg);
		STDMETHOD(Save) (LPSTORAGE pStgSave, BOOL fSameAsLoad);
		STDMETHOD(SaveCompleted) (LPSTORAGE pStgNew);
		STDMETHOD(HandsOffStorage) (void);
	private:
		CDataView FAR * m_pDataView;
		/*
		 *OLE2NOTE: the following states and flags are used to control
		 *save and save completed behavior.  It is important to keep
		 *track of this information so that save advises and low memory
		 *situations are handled appropriately.  See the OLE spec for a
		 *thorough discussion of the issues.
		 */
		 enum DVPS_State { DVPS_UNINITIALIZED, DVPS_NORMAL, DVPS_NOSCRIBBLE,
			DVPS_HANDSOFF };        //DVPS stands for DataView PersistStorage state

		 DVPS_State m_state;
		 BOOL m_fSaveWithSameAsLoad;
		 BOOL m_fNoScribbleMode;
	};

	friend struct CPersistStorage;
	CPersistStorage m_persistStorage;

	struct CDataObject : IDataObject
	{
		CDataObject( CDataView FAR * pDataView );

		STDMETHOD(QueryInterface) (REFIID riid, LPVOID FAR* ppvObj);
	STDMETHOD_(ULONG,AddRef) (void);
	STDMETHOD_(ULONG, Release) (void);
	STDMETHOD(GetData) (LPFORMATETC pformatetcIn,
				LPSTGMEDIUM pmedium );
	STDMETHOD(GetDataHere) (LPFORMATETC pformatetc,
				LPSTGMEDIUM pmedium );
	STDMETHOD(QueryGetData) (LPFORMATETC pformatetc );
	STDMETHOD(GetCanonicalFormatEtc) (LPFORMATETC pformatetc,
				LPFORMATETC pformatetcOut);
	STDMETHOD(SetData) (LPFORMATETC pformatetc, STGMEDIUM FAR * pmedium,
				BOOL fRelease);
	STDMETHOD(EnumFormatEtc) (DWORD dwDirection,
				LPENUMFORMATETC FAR* ppenumFormatEtc);

	STDMETHOD(DAdvise) (FORMATETC FAR* pFormatetc, DWORD advf,
			LPADVISESINK pAdvSink, DWORD FAR* pdwConnection);
	STDMETHOD(DUnadvise) (DWORD dwConnection);
	STDMETHOD(EnumDAdvise) (LPENUMSTATDATA FAR* ppenumAdvise);

		private:
		CDataView FAR * m_pDataView;
	};

	friend struct CDataObject;
	CDataObject m_dataObject;


	struct CViewObject : IViewObject2
	{
	CViewObject( CDataView FAR * pDataView );

	STDMETHOD(QueryInterface) (REFIID riid, LPVOID FAR* ppvObj);
	STDMETHOD_(ULONG,AddRef) (void);
	STDMETHOD_(ULONG,Release) (void);

	//IViewObject methods

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
	STDMETHOD(Freeze)(DWORD dwDrawAspect, LONG lindex,
			void FAR* pvAspect,
			DWORD FAR* pdwFreeze);
	STDMETHOD(Unfreeze) (DWORD dwFreeze);
	STDMETHOD(SetAdvise) (DWORD aspects, DWORD advf,
			LPADVISESINK pAdvSink);
	STDMETHOD(GetAdvise) (DWORD FAR* pAspects, DWORD FAR* pAdvf,
			LPADVISESINK FAR* ppAdvSink);

	//IViewObject2 methods

	STDMETHOD(GetExtent) (DWORD dwDrawAspect, LONG lindex, DVTARGETDEVICE
					FAR *ptd, LPSIZEL lpsizel);

	DWORD   m_aspects;
	DWORD   m_advf;

	private:
	CDataView FAR * m_pDataView;
	BOOL    m_fFrozen;

	};

	friend struct CViewObject;
	CViewObject m_viewObject;


	struct CAdvSink : IAdviseSink
	{
	CAdvSink( CDataView FAR * pDataView );
  ~CAdvSink( );

	STDMETHOD(QueryInterface) (REFIID riid, LPVOID FAR* ppvObj);
	STDMETHOD_(ULONG,AddRef) (void);
	STDMETHOD_(ULONG,Release) (void);

		// *** IAdviseSink methods ***
		STDMETHOD_(void,OnDataChange)(FORMATETC FAR* pFormatetc,
					STGMEDIUM FAR* pStgmed);
		STDMETHOD_(void,OnViewChange)(DWORD dwAspect, LONG lindex);
		STDMETHOD_(void,OnRename)(LPMONIKER pmk);
		STDMETHOD_(void,OnSave)(void);
		STDMETHOD_(void,OnClose)(void);

	private:
	ULONG m_refs;
	CDataView FAR * m_pDataView;

	public:
	void Disconnect(void);      // release m_pDataView back pointer
	};

	friend struct CAdvSink;

	// OLE2NOTE: this CAdvSink object is allocated as a SEPERATE object
	// our CDataView object itself. notice that we only have a pointer
	// to the CAdvSink object. this is VERY important because the
	// lifetime of this CAdvSink object needs to be managed seperately
	// from the CDataView. when our embedding container releases all
	// pointers to the CDataView object, it must destroy itself. the
	// CAdvSink object can not be destroyed at the same time; it must
	// wait till all of its ref counts have been released (by the
	// remoting stub holding it). we also want to keep this CAdvSink
	// as a seperate object in that it has its own identify (its IUnknown
	// is separate from that of CDataView). we do not want clients to
	// be able to QueryInterface the CDataView object for IAdviseSink.
	CAdvSink FAR* m_pMyAdvSink;


	//the definiton of these methods can be found in dvcache.cpp

	struct COleCache : IOleCache2 {
		COleCache( CDataView FAR * pDataView );
		 ~COleCache(void);
		//required interfaces

		STDMETHOD(QueryInterface) (REFIID riid, LPVOID FAR* ppvObj);
		STDMETHOD_(ULONG, AddRef) (void);
		STDMETHOD_(ULONG, Release) (void);

		//IOleCache methods

		STDMETHOD(Cache)(LPFORMATETC lpFormatetc, DWORD advf, LPDWORD lpdwConnection);
		STDMETHOD(Uncache)(DWORD dwConnection);
		STDMETHOD(EnumCache)(LPENUMSTATDATA FAR *ppenumStatData);
		STDMETHOD(InitCache)(LPDATAOBJECT pDataObject);
		STDMETHOD(SetData)(LPFORMATETC pformatetc, STGMEDIUM FAR *pmedium, BOOL fRelease);

		//IOleCache2 methods

		STDMETHOD(UpdateCache)(LPDATAOBJECT pDataObject, DWORD grfUpdf, LPVOID pReserved);
		STDMETHOD(DiscardCache)(DWORD dwDiscardOptions);

		//COleCache holds a DataCache object inside it to which it delegates
		//mosts tasks.

		IOleCache2 FAR *m_pDataCache;
		IViewObject2 FAR *m_pViewObject;
		IDataObject FAR *m_pDataObject;
		IPersistStorage FAR *m_pPersistStorage;
		IOleObject FAR *m_pOleObject;

		//method to instantiate our     DataCache item and in general set
		//things up when needed.

		HRESULT Setup(void);

		private:
			//OLE2NOTE: since we have a combination of implicit and explicit
			//caches, we will keep an array of STATDATA's for *all* the formats
			//we support.  EnumCache will build the enumerator from this array.
			#define DV_NUMFORMATS   4
			STATDATA m_rgStatData[4];
			CDataView FAR *m_pDataView;
	};

	friend struct COleCache;
	COleCache m_oleCache;

	friend class CDataViewFactory;

	//the definition of these methods can be found in dvrun.cpp

	struct CRunnableObject : IRunnableObject  {
		CRunnableObject( CDataView FAR * pDataView );

		// *** IUnknown methods ***
		STDMETHOD(QueryInterface) (REFIID riid, LPVOID FAR* ppvObj);
		STDMETHOD_(ULONG,AddRef)(void);
		STDMETHOD_(ULONG,Release)(void);

		// *** IRunnableObject methods ***
		STDMETHOD(GetRunningClass) (LPCLSID lpClsid);
		STDMETHOD(Run)(LPBINDCTX pbc);
		STDMETHOD_(BOOL, IsRunning)(void);
		STDMETHOD(LockRunning)(BOOL fLock, BOOL fLastUnlockCloses);
		STDMETHOD(SetContainedObject)(BOOL fContained);

		private:
			CDataView FAR *m_pDataView;
	};

	friend struct CRunnableObject;
	CRunnableObject m_runnableObject;


	//the definition of these methods can be found in dvrun.cpp

	struct CExternalConnection : IExternalConnection {
		CExternalConnection( CDataView FAR *pDataView );

		// *** IUnkown methods ***
		STDMETHOD(QueryInterface) (REFIID riid, LPVOID FAR* ppvObj);
		STDMETHOD_(ULONG,AddRef)(void);
		STDMETHOD_(ULONG,Release)(void);

		// *** IExternalConnection methods ***
		STDMETHOD_(DWORD, AddConnection)(DWORD extconn, DWORD reserved);
		STDMETHOD_(DWORD, ReleaseConnection)(DWORD extconn, DWORD reserved,
			BOOL fLastReleaseCloses);

		private:
			CDataView FAR *m_pDataView;
			DWORD m_dwStrong;
	};

	friend struct CExternalConnection;
	CExternalConnection m_externalConnection;

	public:
		//methods for interacting with RealTime
		BOOL UpdateData(void);
		BOOL Connect(void);

	private:
		LPOLECLIENTSITE     m_pClientSite;
		LPSTORAGE           m_pStg;
		LPSTREAM            m_pStream;          /*
												 *OLE2NOTE: we hold a pointer to our stream open
												 *at all times to handle low-memory situations (so
												 *the object can be saved and then exited)
												 */
		BOOL                m_fIsDirty;         //has the dataview object been modified?
		LPDATAOBJECT        m_pSrcDataObject;   //ptr to RealTime data source
		DWORD               m_dwSrcDataConn;    //DAdvise connection cookie
												//  with RealTime data source
		FORMATETC           m_SrcFmtEtc;            //format avail. from data src
												//  either CF_REALTIME/CF_TEXT
		LPDATAADVISEHOLDER  m_pAdvHolder;
		IOleAdviseHolder FAR* m_pOleAdviseHolder;
		LPADVISESINK        m_pViewAdvSink;
		SIZEL   m_lastSizeL;
		BOOL    m_fConnect;                     //have we tried to connect once??
												//used for GetRealTimeParam
		BOOL    m_fIsRunning;
		BOOL    m_fIsClosing;
		BOOL    m_fIsSelfLocked;
		DWORD   m_dwRegister;

		//function to take us into and out of the self-locked state
		//(see accompanying state diagram)
		void SelfLock( BOOL fLock );
};

/*
 *      DATAVIEW objects get and set data in REALTIME objects by using
 *      a special clipboard format, cfRealtimeItem.
 */

extern WORD cfRealtimeItem;

#ifdef DEFINE_DATAVIEW_GUID_HERE
#include <initguid.h>
#endif
DEFINE_GUID(CLSID_CDataView, 0xA11CC4E0, 0xC0E1, 0x1068, 0xB8, 0x7C, 0x00, 0xDD, 0x01, 0x07, 0xD0, 0xBD);
DEFINE_GUID(CLSID_CRealtime, 0x73F76E20, 0xB853, 0x1068, 0xB8, 0x7A, 0x00, 0xDD, 0x01, 0x07, 0xD0, 0xBD);

#endif
