/*
	dataview.cpp

	This file contains much of the DataView object implementation.
	DataView is a OLE2.0 server dll which displays a stock quote gained
	from some external data source (such as realtime.exe).

	This sample code is fully functional and supports loading, saving,
	linking, and embedding.  There is no support for OLE Automation or
	in-place activation (as there is no user interface).

	(c) Copyright Microsoft Corp. 1992 - 1993 All Rights Reserved
*/

#define DEFINE_DATAVIEW_GUID_HERE
#include "dataview.h"
#include "stockdlg.h"
#include "stockutl.h"
#include <string.h>
#include <stdlib.h>
#include "..\prgidmon.h"
#include <olestd.h>

// This routine is in OLEUI.DLL and since we didn't include "ole2ui.h"
// we need to define it here.
STDAPI OleUICanUnloadNow(void);

FORMATETC g_formatetc = {CF_TEXT, NULL, DVASPECT_CONTENT, -1, TYMED_HGLOBAL};
DWORD g_dwObjectCount = 0; // count of objects given out by this DLL

// This line is needed for the debug utilities in OLE2UI
extern "C" {
	OLEDBGDATA
}

/*
 *      Since we are a DLL server, we may be running in the context of
 *      any process that chooses to embed a DATAVIEW object.  This
 *      means that we should use the memory allocator that that process
 *      gave in its OleInitialize call.  We get this by calling
 *      CoGetMalloc.
 *
 *      We can encapsulate this by redefining the global new operator
 *      and using new for all memory allocations.
 */

void FAR* operator new( size_t size)
{
	IMalloc FAR* pMalloc;
	LPVOID lpv;

	if (CoGetMalloc(MEMCTX_TASK, &pMalloc) == NOERROR)
	{
		lpv = pMalloc->Alloc(size);
		pMalloc->Release();
		return lpv;
	}
	return NULL;
}

/*
 *      We need to redefine delete to match our redefinition of new.
 */

void operator delete( void FAR* lpv)
{
	IMalloc FAR* pMalloc;

  if (lpv == NULL) return;

  if( CoGetMalloc(MEMCTX_TASK, &pMalloc) == NOERROR){
	if (pMalloc->DidAlloc(lpv) != 0)
	  pMalloc->Free(lpv);
	pMalloc->Release();
  }

}


/*
 *      Implementation of CControllingUnknown class.
 */
CControllingUnknown::CControllingUnknown( LPUNKNOWN pUnkOuter ) : m_refs(0)
{
		m_pUnkOuter = pUnkOuter;
		if (pUnkOuter) pUnkOuter->AddRef();
}


STDMETHODIMP CControllingUnknown::QueryInterface (REFIID riid, LPVOID FAR* ppvObj)
{
	//      Assert
	*ppvObj = NULL;
	return ResultFromScode(E_NOINTERFACE);
}


STDMETHODIMP_(ULONG) CControllingUnknown::AddRef (void)
{
	if (m_pUnkOuter) return m_pUnkOuter->AddRef();
	return ++m_refs;
}

STDMETHODIMP_(ULONG) CControllingUnknown::Release (void)
{
	if (m_pUnkOuter) return m_pUnkOuter->Release();
	if (--m_refs == 0)
	{
		delete this;
		return 0;
	}
	return m_refs;
}


/*
 *      Implementation of CDataViewFactory and the related function,
 *      DllGetClassObject.
 */


LPCLASSFACTORY CDataViewFactory::Create(void)
{
	CDataViewFactory FAR * pSF = new FAR CDataViewFactory;
	return (LPCLASSFACTORY) pSF;  // ref count should be 0
}

STDAPI  DllGetClassObject(REFCLSID clsid, REFIID iid, void FAR* FAR* ppv)
{
	IClassFactory FAR * pCF;
	HRESULT hr;

	*ppv = NULL;
	if (clsid == CLSID_CDataView)
	{
		pCF = CDataViewFactory::Create();
		if (pCF == NULL) return ResultFromScode(E_OUTOFMEMORY);
		pCF->AddRef();      // ref count should be 1
		hr = pCF->QueryInterface( iid, ppv ); // now it should be 2
		pCF->Release();     // now it should be 1 unless QI failed
		return hr;
	}
	return ResultFromScode(E_FAIL);
}


STDAPI DllCanUnloadNow(void)
{
	/* OLE2NOTE: if the OLE2UI library is used as a static library,
	**    then we need to include a check whether the OLE2UI code is
	**    able to be unloaded along with our own check. this means if
	**    the OLE2UI code, for example, has passed out a pointer to a
	**    OleStdEnumFmtEtc object that has not yet been released, then
	**    the OLE2UI code may not be unloaded. however, because OLE2UI
	**    code is statically linked with our DLL, this really means OUR
	**    DLL can not be unloaded. Thus it is necessary to call
	**    OleUICanUnloadNow from within our own DllCanUnloadNow function.
	**
	**    if we had used the OLE2UI library in DLL form then we must
	**    call OleUILockLibrary(TRUE) during our LibMain processing and
	**    call OleUILockLibrary(FALSE) in our WEP. this is done INSTEAD of
	**    calling OleUICanUnloadNow from our DllCanUnloadNow function.
	**    the idea here is that the OLE2UI DLL would be held locked for
	**    the duration that our DLL is in use. after our DLL is
	**    unloaded then the OLE2UI DLL can be unloaded if it is not
	**    being used (eg. all object instances created by the OLE2UI
	**    DLL have been released). if, for an example, an
	**    OleStdEnumFmtEtc object had not been released, then the
	**    OLE2UI DLL would remain loaded even after our DLL was
	**    unloaded. this is achieved because the OleUILockLibrary calls
	**    the OLE API "CoLoadLibrary" which tells OLE to manage the
	**    library.
	*/
	if ((g_dwObjectCount == 0)
#if STATIC_OLE2UI == 1
		&& (OleUICanUnloadNow() == NOERROR)
#endif
	) {
		return NOERROR;
	}

	return ResultFromScode(S_FALSE);
}


STDMETHODIMP CDataViewFactory::QueryInterface (REFIID riid, LPVOID FAR* ppvObj)
{
	*ppvObj = NULL;
	if (riid == IID_IUnknown || riid == IID_IClassFactory)
	{
		*ppvObj = (LPVOID)this;
		AddRef();
		return NOERROR;
	}
	return ResultFromScode(E_NOINTERFACE);
}


STDMETHODIMP_(ULONG) CDataViewFactory::AddRef (void)
{
	return ++m_refs;
}

STDMETHODIMP_(ULONG) CDataViewFactory::Release (void)
{
	if (--m_refs == 0)
	{
		delete this;
		return 0;
	}
	return m_refs;
}


STDMETHODIMP CDataViewFactory::CreateInstance(IUnknown FAR* pUnkOuter,
	REFIID iidInterface, void FAR* FAR* ppvObject)
{
	CDataView FAR * pDV;
	HRESULT hresult;
	*ppvObject = NULL;

	pDV = CDataView::Create(pUnkOuter, NULL);
	if (pDV == NULL) return ResultFromScode(E_OUTOFMEMORY);
	pDV->AddRef();      // ref count should be 1
	hresult = pDV->QueryInterface( iidInterface, ppvObject ); //now should be 2
	pDV->Release();     // now it should be 1 unless QI failed
	return hresult;
}

STDMETHODIMP CDataViewFactory::LockServer( BOOL fLock )
{
  if (fLock) {
	g_dwObjectCount ++ ;
  }else{
	g_dwObjectCount -- ;
  }

  return NOERROR;
}



/*
 *      The implementation of the CDataView constructor, and the IUnknown
 *      methods for CDataView.
 */

CDataView::CDataView( IUnknown FAR * pUnk ): CControllingUnknown(pUnk),
	m_oleObject(this), m_persistStorage(this),
	m_viewObject(this), m_dataObject(this), m_oleCache(this),
	m_runnableObject(this), m_externalConnection(this), m_stockInfo(this)
{
  g_dwObjectCount++ ; // increment the object count

	m_pClientSite = NULL;
	m_pStg = NULL;
	m_pStream = NULL;
	m_fIsDirty = TRUE;      //we start out being dirty until somebody saves us

	m_pSrcDataObject = NULL;
	m_dwSrcDataConn = 0L;
	m_SrcFmtEtc.cfFormat = CF_TEXT;
	m_SrcFmtEtc.ptd = NULL;
	m_SrcFmtEtc.dwAspect = DVASPECT_CONTENT;
	m_SrcFmtEtc.lindex = -1;
	m_SrcFmtEtc.tymed = TYMED_HGLOBAL;

	m_pOleAdviseHolder = NULL;
	m_pAdvHolder = NULL;
	m_pViewAdvSink = NULL;
	m_lastSizeL.cx = 0;
	m_lastSizeL.cy = 0;
	m_fConnect = FALSE;
	m_fIsRunning = FALSE;
	m_fIsClosing = FALSE;
	m_fIsSelfLocked = FALSE;
	m_dwRegister = 0L;
	m_pMyAdvSink = NULL;
}


/*
 * DataView destructor
 */

CDataView::~CDataView(void)
{
	if (m_pClientSite) m_pClientSite->Release();
	if (m_pStream) m_pStream->Release();
	if (m_pStg) m_pStg->Release();
	if (m_dwSrcDataConn) m_pSrcDataObject->DUnadvise(m_dwSrcDataConn);
	if (m_pSrcDataObject) m_pSrcDataObject->Release();
	if (m_pOleAdviseHolder) m_pOleAdviseHolder->Release();
	if( m_pAdvHolder ) m_pAdvHolder->Release();
	if( m_pMyAdvSink ) {
		m_pMyAdvSink->Disconnect();
		m_pMyAdvSink->Release();
	}

  g_dwObjectCount-- ; //decrement the object count
}


CDataView FAR * CDataView::Create( LPUNKNOWN pUnkOuter, LPMONIKER pmkDataService )
{
	CDataView FAR * pDataView = new FAR CDataView(pUnkOuter);

	if (!pDataView) return NULL;    // E_OUTOFMEMORY

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
	// the CAdvSink object holds a non-addref'ed pointer to the CDataView
	// object.
	pDataView->m_pMyAdvSink = new FAR CAdvSink(pDataView);
	if ( !pDataView->m_pMyAdvSink ) {
		delete pDataView;               // clean up on error
		return NULL;                    // E_OUTOFMEMORY
	}
	pDataView->m_pMyAdvSink->AddRef();  // ref count should now be 1

	return pDataView;                   // ref count should be 0
}


STDMETHODIMP CDataView::QueryInterface (REFIID riid, LPVOID FAR* ppvObj)
{
	*ppvObj = NULL;
	if (riid == IID_IUnknown)
		*ppvObj = (LPVOID)this;
	else if (riid == IID_IOleObject)
		*ppvObj = (LPVOID)&m_oleObject;
	else if (riid == IID_IDataObject)
		*ppvObj = (LPVOID)&m_dataObject;
	else if (riid == IID_IViewObject || riid == IID_IViewObject2)
		*ppvObj = (LPVOID)&m_viewObject;
	else if (riid == IID_IPersistStorage)
		*ppvObj = (LPVOID)&m_persistStorage;
	else if (riid == IID_IOleCache || riid == IID_IOleCache2)
		*ppvObj = (LPVOID)&m_oleCache;
	else if (riid == IID_IRunnableObject)
		*ppvObj = (LPVOID)&m_runnableObject;
	else if (riid == IID_IExternalConnection)
		*ppvObj = (LPVOID)&m_externalConnection;
	if (*ppvObj != NULL)
	{
		AddRef();
		return NOERROR;
	}
	return ResultFromScode(E_NOINTERFACE);
}


CDataView::COleObject::COleObject( CDataView FAR * pDataView )
{
	m_pDataView = pDataView;
}


STDMETHODIMP CDataView::COleObject::QueryInterface (REFIID riid, LPVOID FAR* ppvObj)
{
	return m_pDataView->QueryInterface(riid, ppvObj);
}


STDMETHODIMP_(ULONG) CDataView::COleObject::AddRef (void)
{
	return m_pDataView->AddRef();
}


STDMETHODIMP_(ULONG) CDataView::COleObject::Release (void)
{
	return m_pDataView->Release();
}



STDMETHODIMP CDataView::COleObject::SetClientSite (LPOLECLIENTSITE pClientSite)
{
	if (m_pDataView->m_pClientSite)
		m_pDataView->m_pClientSite->Release();

	m_pDataView->m_pClientSite = pClientSite;

	if (m_pDataView->m_pClientSite)
		m_pDataView->m_pClientSite->AddRef();
	return NOERROR;
}


STDMETHODIMP CDataView::COleObject::GetClientSite (LPOLECLIENTSITE FAR* ppClientSite)
{
	*ppClientSite = m_pDataView->m_pClientSite;
	if (*ppClientSite)
		(*ppClientSite)->AddRef();
	return NOERROR;
}


STDMETHODIMP CDataView::COleObject::SetHostNames (LPCSTR szContainerApp, LPCSTR szContainerObj)
{
	// we do not need to use the Host name strings because we do not have
	// any menus.
	return NOERROR;
}


STDMETHODIMP CDataView::COleObject::Close (DWORD dwSaveOption)
{

	if( !m_pDataView->m_fIsClosing ) {
		m_pDataView->m_fIsClosing = TRUE;

		if( dwSaveOption == OLECLOSE_SAVEIFDIRTY ||
			dwSaveOption == OLECLOSE_PROMPTSAVE )
			if( m_pDataView->m_pClientSite )
				m_pDataView->m_pClientSite->SaveObject();

		if (m_pDataView->m_pViewAdvSink != NULL)
		{
			m_pDataView->m_pViewAdvSink->Release();
			m_pDataView->m_pViewAdvSink = NULL;
		}

		//get us out of locked
		m_pDataView->SelfLock(FALSE);

		//if we are running (registered in ROT), then cease to be
		if( m_pDataView->m_fIsRunning ) {
			m_pDataView->m_fIsRunning = FALSE;
			OleStdRevokeAsRunning( (DWORD FAR *)&m_pDataView->m_dwRegister);

			/*
			 * OLE2NOTE: Since we are a dll server, we must manually do the things
			 * that an exe's default handler would do, such as unlocking our container.
			 */

			if( m_pDataView->m_pClientSite ) {
				LPOLECONTAINER pContainer;
				if( m_pDataView->m_pClientSite->GetContainer( &pContainer ) == NOERROR ){
					pContainer->LockContainer(FALSE);
					pContainer->Release();
				}
			}
		}

		//disconnect ourselves from the realtime exe

		if( m_pDataView->m_pSrcDataObject) {
			if (m_pDataView->m_dwSrcDataConn){
				m_pDataView->m_pSrcDataObject->DUnadvise(
						m_pDataView->m_dwSrcDataConn);
				m_pDataView->m_dwSrcDataConn = 0L;
			}
			m_pDataView->m_pSrcDataObject->Release();
			m_pDataView->m_pSrcDataObject = NULL;
		}

		if (m_pDataView->m_pOleAdviseHolder)
			m_pDataView->m_pOleAdviseHolder->SendOnClose();

		m_pDataView->m_fIsClosing = FALSE;
	}
	return NOERROR;
}


STDMETHODIMP CDataView::COleObject::SetMoniker (DWORD dwWhichMoniker, LPMONIKER pmk)
{
	LPMONIKER lpmkFull;
	HRESULT hrErr;

	if( m_pDataView->m_pClientSite == NULL )
		return ResultFromScode(E_FAIL);

	if( (hrErr = m_pDataView->m_pClientSite->GetMoniker(
			OLEGETMONIKER_ONLYIFTHERE,
			OLEWHICHMK_OBJFULL,
			&lpmkFull) ) != NOERROR )
		return hrErr;

	//if we have advise holders, tell them that we've been renamed
	if( lpmkFull && m_pDataView->m_pOleAdviseHolder )
		m_pDataView->m_pOleAdviseHolder->SendOnRename(lpmkFull);

	if( lpmkFull )
		lpmkFull->Release();

	return NOERROR;
}


STDMETHODIMP CDataView::COleObject::GetMoniker (DWORD dwAssign, DWORD dwWhichMoniker,
			   LPMONIKER FAR* ppmk)
{
	*ppmk = NULL;

	if( !m_pDataView->m_pClientSite )
		return ResultFromScode(E_FAIL);

	return m_pDataView->m_pClientSite->GetMoniker(dwAssign, dwWhichMoniker, ppmk);

}


STDMETHODIMP CDataView::COleObject::InitFromData (LPDATAOBJECT pDataObject,
			BOOL fCreation,
			DWORD dwReserved)
{
	return NOERROR;
}


STDMETHODIMP CDataView::COleObject::GetClipboardData (DWORD dwReserved,
			LPDATAOBJECT FAR* ppDataObject)
{
	return ResultFromScode(E_NOTIMPL);
}


STDMETHODIMP CDataView::COleObject::DoVerb (LONG iVerb,
			LPMSG lpmsg,
			LPOLECLIENTSITE pActiveSite,
			LONG lindex,
			HWND hwndParent,
			LPCRECT lprcPosRect)
{
	static HWND oldhwndParent = 0;
	/*
	 if dialog up, then don't create new one

	 if hide, then goto running state
	 */
	StockDlg *sd;

	if( iVerb == OLEIVERB_PRIMARY || iVerb == OLEIVERB_OPEN || iVerb == OLEIVERB_SHOW ){
		m_pDataView->SelfLock(TRUE);

		// tell embedding container to go visible and scroll us into view.
		m_pDataView->m_pClientSite->ShowObject();

		// do not show dialog if DoVerb is delegated from a linking client
		if (pActiveSite && m_pDataView->m_pClientSite &&
			(pActiveSite != m_pDataView->m_pClientSite))
			return NOERROR;

		if( !oldhwndParent ) {
			oldhwndParent = hwndParent;
			sd = new FAR StockDlg( hwndParent, m_pDataView );
			oldhwndParent = NULL;
			delete sd;

			//since the user may have changed the stock name, recalculate the
			//size of the drawing box

			m_pDataView->m_stockInfo.UpdateDisplayText();
		} else
			//just make the one we have visible
			BringWindowToTop( oldhwndParent );

	} else if ( iVerb == OLEIVERB_HIDE ){
		/*
		 * OLE2NOTE:  If we are not currently running, HIDE forces
		 * us into the running state
		 */
		if( !m_pDataView->m_fIsRunning )
			m_pDataView->m_runnableObject.Run(NULL);

		m_pDataView->SelfLock(FALSE);
	}


	return NOERROR;
}


STDMETHODIMP CDataView::COleObject::EnumVerbs (IEnumOLEVERB FAR* FAR* ppenumOleVerb)
{
	*ppenumOleVerb = NULL;
	//OLE2NOTE: use the OleReg helper API's
	return OleRegEnumVerbs(CLSID_CDataView, ppenumOleVerb);
}


STDMETHODIMP CDataView::COleObject::Update (void)
{
	//tell the cache to update itself, if we have one

	if( m_pDataView->m_oleCache.m_pOleObject )
		return m_pDataView->m_oleCache.m_pOleObject->Update();

	return NOERROR;
}


STDMETHODIMP CDataView::COleObject::IsUpToDate (void)
{
	return NOERROR;    // TRUE
}


STDMETHODIMP CDataView::COleObject::GetUserClassID (CLSID FAR* pClsid)
{
	*pClsid = CLSID_CDataView;
	return NOERROR;
}


STDMETHODIMP CDataView::COleObject::GetUserType (DWORD dwFormOfType, LPSTR FAR* pszUserType)
{
	*pszUserType = NULL;
	//OLE2NOTE: use OleReg helper API's
	return OleRegGetUserType(CLSID_CDataView, dwFormOfType, pszUserType);
}


STDMETHODIMP CDataView::COleObject::SetExtent (DWORD dwDrawAspect, LPSIZEL lpsizel)
{
	if( dwDrawAspect == DVASPECT_CONTENT)
		return ResultFromScode(S_FALSE);
	else {
		//delegate to the cache if we have one
		if( m_pDataView->m_oleCache.m_pOleObject )
			return m_pDataView->m_oleCache.m_pOleObject->SetExtent(dwDrawAspect, lpsizel);
		return ResultFromScode(S_FALSE);
	}
}


STDMETHODIMP CDataView::COleObject::GetExtent (DWORD dwDrawAspect, LPSIZEL lpsizel)
{
	if (dwDrawAspect == DVASPECT_CONTENT) {
		*lpsizel = m_pDataView->m_lastSizeL;
		return NOERROR;
	} else {
		//delegate to the cache (to handle user-defined icons, etc)
		if( m_pDataView->m_oleCache.m_pViewObject)
			return m_pDataView->m_oleCache.m_pViewObject->GetExtent(dwDrawAspect,
				-1, NULL, lpsizel);
		return ResultFromScode(E_FAIL);
	}
}



STDMETHODIMP CDataView::COleObject::Advise (IAdviseSink FAR* pAdvSink, DWORD FAR* pdwConnection)
{
	if (m_pDataView->m_pOleAdviseHolder == NULL)
		CreateOleAdviseHolder(&(m_pDataView->m_pOleAdviseHolder));
	if (m_pDataView->m_pOleAdviseHolder != NULL)
			return m_pDataView->m_pOleAdviseHolder->Advise(pAdvSink, pdwConnection);
	return ResultFromScode(E_FAIL);
}


STDMETHODIMP CDataView::COleObject::Unadvise(DWORD dwConnection)
{
	if (m_pDataView->m_pOleAdviseHolder != NULL)
			return m_pDataView->m_pOleAdviseHolder->Unadvise(dwConnection);
	return ResultFromScode(E_FAIL);
}


STDMETHODIMP CDataView::COleObject::EnumAdvise (LPENUMSTATDATA FAR* ppenumAdvise)
{
	*ppenumAdvise = NULL;
	return ResultFromScode(E_NOTIMPL);
}


STDMETHODIMP CDataView::COleObject::GetMiscStatus (DWORD dwAspect, DWORD FAR* pdwStatus)
{
	*pdwStatus = 0;
	//OLE2NOTE: use OleReg helper API's
	return OleRegGetMiscStatus(CLSID_CDataView, dwAspect, pdwStatus);
}


STDMETHODIMP CDataView::COleObject::SetColorScheme (LPLOGPALETTE lpLogpal)
{
	return NOERROR;         //we don't let folks change our colors
}



CDataView::CPersistStorage::CPersistStorage( CDataView FAR * pDataView )
{
	m_pDataView = pDataView;
	m_state = DVPS_UNINITIALIZED;
	m_fSaveWithSameAsLoad = FALSE;
	m_fNoScribbleMode = FALSE;
}


STDMETHODIMP CDataView::CPersistStorage::QueryInterface (REFIID riid, LPVOID FAR* ppvObj)
{
	return m_pDataView->QueryInterface(riid, ppvObj);
}


STDMETHODIMP_(ULONG) CDataView::CPersistStorage::AddRef (void)
{
	return m_pDataView->AddRef();
}


STDMETHODIMP_(ULONG) CDataView::CPersistStorage::Release (void)
{
	return m_pDataView->Release();
}



STDMETHODIMP CDataView::CPersistStorage::GetClassID (LPCLSID lpClassID)
{
	if( m_state == DVPS_UNINITIALIZED )
		return ResultFromScode(E_UNEXPECTED);
	else
		*lpClassID = CLSID_CDataView;
		return NOERROR;
}


STDMETHODIMP CDataView::CPersistStorage::IsDirty (void)
{
	if( m_pDataView->m_fIsDirty )
		return ResultFromScode(S_OK);
	else
		return ResultFromScode(S_FALSE);
}


STDMETHODIMP CDataView::CPersistStorage::InitNew (LPSTORAGE pStg)
{
	HRESULT hresult;

	if( m_state != DVPS_UNINITIALIZED)
		return ResultFromScode(E_UNEXPECTED);

	if(!pStg )
		return ResultFromScode(E_INVALIDARG);

	m_pDataView->m_pStg = pStg;
	pStg->AddRef();

	//now create the new stream
	/*
	 *OLE2NOTE:  the stream is created and held open to handle low memory situations
	 *(so the user can save and exit without needing to allocate more memory)
	 */

	if( (hresult = pStg->CreateStream("DataViewContents",
		STGM_READWRITE  | STGM_SHARE_EXCLUSIVE  | STGM_CREATE, 0, 0, &(m_pDataView->m_pStream))) != NOERROR) {
			pStg->Release();
			m_pDataView->m_pStg = NULL;
			return hresult;
	}

	//delegate to the cache to initialize cache storage
	if( !m_pDataView->m_oleCache.m_pDataCache)
		m_pDataView->m_oleCache.Setup();
	hresult = m_pDataView->m_oleCache.m_pPersistStorage->InitNew(pStg);

	//set ourselves to the normal state
	if( hresult == NOERROR) {
		m_state = DVPS_NORMAL;
		m_fSaveWithSameAsLoad = FALSE;      //see SaveCompleted() for more details
		m_fNoScribbleMode = FALSE;
		m_pDataView->m_fIsDirty = TRUE;
	} else {
		//clean things up
		m_pDataView->m_pStream->Release();
		m_pDataView->m_pStream = NULL;
		pStg->Release();
		m_pDataView->m_pStg = NULL;
	}
	return hresult;
}


STDMETHODIMP CDataView::CPersistStorage::Load (LPSTORAGE pStg)
{
	HRESULT hresult;
	CLSID cid;

	if( m_state != DVPS_UNINITIALIZED )
		return ResultFromScode(E_UNEXPECTED);

	if( !pStg )
		return ResultFromScode(E_INVALIDARG);

	m_pDataView->m_pStg = pStg;
	pStg->AddRef();

//Stupid Macro Trick #113: getting around repetitious code :)
#define xx(a) if( (hresult = (a)) != NOERROR ) goto err_rtn;

	//open the stream
	//OLE2NOTE: we hang onto this stream for low memory situations
	xx( pStg->OpenStream("DataViewContents",NULL, STGM_READWRITE | STGM_SHARE_EXCLUSIVE,
		0, &(m_pDataView->m_pStream)))

	//get the class id of the data source moniker and create it
	xx( ReadClassStm(m_pDataView->m_pStream, &cid));

	if (cid != CLSID_NULL) {
		xx( CoCreateInstance(  cid, NULL, CLSCTX_SERVER, IID_IMoniker,
		(LPVOID FAR*)&(m_pDataView->m_stockInfo.m_pmkDataSource)));

		//load our data
		xx( m_pDataView->m_stockInfo.m_pmkDataSource->Load(m_pDataView->m_pStream));
	} else {
		// we have no data source moniker
		m_pDataView->m_stockInfo.m_pmkDataSource = NULL;
	}

	xx( m_pDataView->m_stockInfo.Load(m_pDataView->m_pStream));

	//delegate to the cache in case it needs to do anything
	if( !m_pDataView->m_oleCache.m_pDataCache)
		m_pDataView->m_oleCache.Setup();
	xx( m_pDataView->m_oleCache.m_pPersistStorage->Load(pStg));

#undef xx

	//if we got here, then everything went OK; enter the normal state
	m_state = DVPS_NORMAL;
	m_pDataView->m_fIsDirty = FALSE;

	return NOERROR;

err_rtn:
	if( m_pDataView->m_pStream ){
		m_pDataView->m_pStream->Release();
		m_pDataView->m_pStream = NULL;
	}
	if( pStg )
		pStg->Release();

	return hresult;     //set in the xx macro
}


STDMETHODIMP CDataView::CPersistStorage::Save (LPSTORAGE pStgSave, BOOL fSameAsLoad)
{
	HRESULT hresult = NOERROR;
	LPSTREAM pStm;
	CLSID clsid;

	if( m_state != DVPS_NORMAL )
		return ResultFromScode(E_UNEXPECTED);

	if( !pStgSave )
		return ResultFromScode(E_INVALIDARG);

	if( fSameAsLoad ) {
		LARGE_INTEGER libzero;
		ULARGE_INTEGER ulibzero;
		LISet32(libzero, 0);
		LISet32(ulibzero, 0);

		//OLE2NOTE: we reuse streams previously opened, useful for low-memory situations
		pStm = m_pDataView->m_pStream;
		pStm->AddRef();

		//truncate and reset the stream to the beginning

		pStm->SetSize(ulibzero);
		pStm->Seek(libzero, STREAM_SEEK_SET, NULL);
	} else
		hresult = pStgSave->CreateStream("DataViewContents",
			STGM_READWRITE  | STGM_SHARE_EXCLUSIVE  | STGM_CREATE, 0, 0, &pStm);

	if (hresult == NOERROR) {

#define xx(a)   if( (hresult = (a)) != NOERROR ) goto err_rtn;

		//get the clsid of our moniker for saving (so we can create it on return)
		if (m_pDataView->m_stockInfo.m_pmkDataSource) {
			xx( m_pDataView->m_stockInfo.m_pmkDataSource->GetClassID((LPCLSID)&clsid) );
			xx( WriteClassStm(pStm, clsid) );

			//tell the moniker to save itself in the stream

			xx( m_pDataView->m_stockInfo.m_pmkDataSource->Save(pStm, FALSE) );
		} else {
			// we have no moniker so write out CLSID_NULL
			xx( WriteClassStm(pStm, CLSID_NULL) );
		}

		xx( m_pDataView->m_stockInfo.Save(pStm));

		//if we have a cache, tell it to update itself and then save

		if( m_pDataView->m_oleCache.m_pOleObject )
			xx( m_pDataView->m_oleCache.m_pOleObject->Update());

		if( m_pDataView->m_oleCache.m_pPersistStorage )
			xx( hresult = m_pDataView->m_oleCache.m_pPersistStorage->Save( pStgSave, fSameAsLoad));

#undef xx

		//upon success, enter the NOSCRIBBLE state.  SaveCompleted will
		//take us out of this state.
		m_state = DVPS_NOSCRIBBLE;
		m_fSaveWithSameAsLoad = fSameAsLoad;
		m_fNoScribbleMode = TRUE;
	}


err_rtn:
	if (pStm)
		pStm->Release();

	return hresult;
}


STDMETHODIMP CDataView::CPersistStorage::SaveCompleted (LPSTORAGE pStgSaveNew)
{
	HRESULT hresult;

	if( m_state == DVPS_UNINITIALIZED || m_state == DVPS_NORMAL)
		return ResultFromScode(E_UNEXPECTED);

	//reopen storages and streams that we need
	if( pStgSaveNew){
		if( m_pDataView->m_pStream);
			m_pDataView->m_pStream->Release();
		if( m_pDataView->m_pStg)
			m_pDataView->m_pStg->Release();


		m_pDataView->m_pStg = pStgSaveNew;
		m_pDataView->m_pStg->AddRef();

		/*
		 *OLE2NOTE:  in some state transitions, pStgSaveNew is guaranteed to
		 *contain the same bits as the last save.  In this case, an app may want
		 *to Open the stream, instead of creating a new one in its place.
		 *In DataView's case, once a stream is open, it will only be written to;
		 *thus, it is sufficient to always create a new stream and destroy whatever
		 *data may have been there.
		 *
		 *Real apps may want the more complicated behavior, refer to the OLE2.0
		 *spec for more details.
		 */
		hresult = pStgSaveNew->OpenStream("DataViewContents", NULL,
			STGM_READWRITE  | STGM_SHARE_EXCLUSIVE , 0,
			&(m_pDataView->m_pStream));
		if (hresult != NOERROR ){
			m_pDataView->m_pStg->Release();
			m_pDataView->m_pStg = NULL;
			return hresult;
		}
	}

	/*
	 *OLE2NOTE:  we do NOT clear the dirty bit in these kinds of situations:
	 *  1. a save copy as operation was performed
	 *  2. SaveCompleted was called without a prior call to Save.
	 *     (ie. HandsOffStorage was called without Save being called)
	 *A further discussion of this issue can be found in the spec.
	 */
	if( pStgSaveNew || m_fSaveWithSameAsLoad) {
		if (m_fNoScribbleMode) {
			m_pDataView->m_fIsDirty = FALSE;
			if( m_pDataView->m_pAdvHolder )
				m_pDataView->m_pOleAdviseHolder->SendOnSave();
		}
		m_fSaveWithSameAsLoad = FALSE;
	}
	m_fNoScribbleMode = FALSE;

	//enter back into the normal state
	m_state = DVPS_NORMAL;

	//if we have a cache, tell it save completed as well

	if( m_pDataView->m_oleCache.m_pPersistStorage )
		return m_pDataView->m_oleCache.m_pPersistStorage->SaveCompleted( pStgSaveNew );
	return NOERROR;
}


STDMETHODIMP CDataView::CPersistStorage::HandsOffStorage (void)
{
	if( m_state != DVPS_NORMAL || m_state != DVPS_NOSCRIBBLE )
		return ResultFromScode(E_UNEXPECTED);

	if( m_pDataView->m_pStg ){
		m_pDataView->m_pStg->Release();
		m_pDataView->m_pStg = NULL;
		if( m_pDataView->m_pStream ){
			m_pDataView->m_pStream->Release();
			m_pDataView->m_pStream = NULL;
		}

	}

	m_state = DVPS_HANDSOFF;

	return NOERROR;
}


CDataView::CDataObject::CDataObject( CDataView FAR * pDataView )
{
	m_pDataView = pDataView;
}

STDMETHODIMP CDataView::CDataObject::QueryInterface (THIS_ REFIID riid, LPVOID FAR* ppvObj)
{
	return m_pDataView->QueryInterface(riid, ppvObj );
}

STDMETHODIMP_(ULONG) CDataView::CDataObject::AddRef (THIS)
{
	return m_pDataView->AddRef();
}

STDMETHODIMP_(ULONG) CDataView::CDataObject::Release (THIS)
{
	return m_pDataView->Release();
}

STDMETHODIMP CDataView::CDataObject::GetData (THIS_ LPFORMATETC pformatetcIn,
			LPSTGMEDIUM pmedium )
{
	pmedium->pUnkForRelease = NULL;
	pmedium->tymed = NULL;
	pmedium->hGlobal = NULL;

	if( pformatetcIn->cfFormat == CF_TEXT && (pformatetcIn->tymed & TYMED_HGLOBAL) ){
		LPSTR lpsz;
		int len = (m_pDataView->m_stockInfo.m_lpszText ?
						_fstrlen(m_pDataView->m_stockInfo.m_lpszText ) : 0);
		pmedium->hGlobal = GlobalAlloc(GMEM_SHARE | GMEM_MOVEABLE, 1 + len);

		lpsz = (LPSTR)GlobalLock(pmedium->hGlobal);
		if (! lpsz) {
			if (pmedium->hGlobal) {
				GlobalFree(pmedium->hGlobal);
				pmedium->hGlobal = NULL;
			}
			return ResultFromScode(E_OUTOFMEMORY);
		}

		if (len)
			_fstrcpy(lpsz, m_pDataView->m_stockInfo.m_lpszText);
		else
			*lpsz = '\0';

		GlobalUnlock(pmedium->hGlobal);
		pmedium->tymed = TYMED_HGLOBAL;
		return NOERROR;
	} else if( pformatetcIn->cfFormat == CF_METAFILEPICT &&
		(pformatetcIn->tymed & TYMED_MFPICT ) ) {

		if( pformatetcIn->dwAspect == DVASPECT_CONTENT ){
			/*
			 *OLE2NOTE: If we get here, we've been asked to provide data
			 *in a metafile format.  The OLE sample library (outlui) provides the
			 *function below to help us implement this case.  It will create the
			 *metafile and then call our ViewObject->Draw function to fill it.
			 */
			pmedium->hGlobal =
				OleStdGetMetafilePictFromOleObject(&(m_pDataView->m_oleObject),
				pformatetcIn->dwAspect, NULL, pformatetcIn->ptd );
			pmedium->tymed = TYMED_MFPICT;
			return NOERROR;
		} else if ( pformatetcIn->dwAspect == DVASPECT_ICON ){
			//if we have a cache, ask it first
			HRESULT hresult = ResultFromScode(E_FAIL);
			if( m_pDataView->m_oleCache.m_pDataObject )
				hresult = m_pDataView->m_oleCache.m_pDataObject->GetData(
					pformatetcIn, pmedium);

			if( hresult != NOERROR ) {
				//the cache did not already have an icon stuffed into it
				//or we do not have a cache
				//return our default icon
				HGLOBAL hglobal = OleGetIconOfClass(CLSID_CDataView, NULL, TRUE);
				if( hglobal ) {
					pmedium->hGlobal = hglobal;
					pmedium->tymed = TYMED_MFPICT;
					return NOERROR;
				} else
					return ResultFromScode(E_FAIL);
			}
		}
	}

	//if we get this far, then the caller has asked for a data format that we
	//don't support.

	return ResultFromScode( DATA_E_FORMATETC );
}

STDMETHODIMP CDataView::CDataObject::GetDataHere (THIS_ LPFORMATETC pformatetc,
			LPSTGMEDIUM pmedium )
{
	if( pformatetc->cfFormat == CF_TEXT && (pformatetc->tymed & TYMED_HGLOBAL) ){
		LPSTR lpsz;

		if( GlobalSize(pmedium->hGlobal) <= _fstrlen(m_pDataView->m_stockInfo.m_lpszText) )
			return ResultFromScode( STG_E_MEDIUMFULL );

		lpsz = (LPSTR)GlobalLock(pmedium->hGlobal);
		_fstrcpy(lpsz, m_pDataView->m_stockInfo.m_lpszText);
		GlobalUnlock(pmedium->hGlobal);

		return NOERROR;
	}
	//for simplicity, we don't allow GetDataHere for Icon aspects.
	//the implementation, however, would be similar that for GetData

	return ResultFromScode( DATA_E_FORMATETC );
}

STDMETHODIMP CDataView::CDataObject::QueryGetData (THIS_ LPFORMATETC pformatetc )
{

	if (pformatetc == NULL) {
		return ResultFromScode(E_INVALIDARG);
	}

	if (pformatetc->lindex != -1) {
		return ResultFromScode(DV_E_LINDEX);
	}

	if( pformatetc->cfFormat == CF_TEXT && (pformatetc->tymed & TYMED_HGLOBAL) )
		return NOERROR;
	else if( pformatetc->cfFormat == CF_METAFILEPICT &&
		(pformatetc->tymed & TYMED_MFPICT ) )
		return NOERROR;
	/* this is a check for Wildcard formatetc only used for DAdvise
	*/
	else if ((pformatetc->cfFormat == NULL) && (pformatetc->ptd == NULL) &&
		(pformatetc->dwAspect == -1L) && (pformatetc->tymed == -1L))
		return NOERROR;
	else if( m_pDataView->m_oleCache.m_pDataObject )
		return m_pDataView->m_oleCache.m_pDataObject->QueryGetData(pformatetc);

	return ResultFromScode( DATA_E_FORMATETC );
}

STDMETHODIMP CDataView::CDataObject::GetCanonicalFormatEtc (THIS_ LPFORMATETC lpformatetc,
			LPFORMATETC lpformatetcOut)
{
	HRESULT hresult;

	if (!lpformatetcOut)
		return ResultFromScode(E_INVALIDARG);

	/* OLE2NOTE: we must make sure to set all out parameters to NULL. */
	lpformatetcOut->ptd = NULL;

	if (!lpformatetc)
		return ResultFromScode(E_INVALIDARG);

	// OLE2NOTE: we must validate that the format requested is supported
	if ((hresult=QueryGetData(lpformatetc)) != NOERROR)
		return hresult;

	/* OLE2NOTE: an app that is insensitive to target device (as
	**    DataView is) should fill in the lpformatOut parameter
	**    but NULL out the "ptd" field; it should return NOERROR if the
	**    input formatetc->ptd what non-NULL. this tells the caller
	**    that it is NOT necessary to maintain a separate screen
	**    rendering and printer rendering. if should return
	**    DATA_S_SAMEFORMATETC if the input and output formatetc's are
	**    identical.
	*/

	*lpformatetcOut = *lpformatetc;
	if (lpformatetc->ptd == NULL)
		return ResultFromScode(DATA_S_SAMEFORMATETC);
	else {
		lpformatetcOut->ptd = NULL;
		return NOERROR;
	}
}


STDMETHODIMP CDataView::CDataObject::SetData (THIS_ LPFORMATETC pformatetc, STGMEDIUM FAR * pmedium,
			BOOL fRelease)
{
	return NOERROR;         //we have no data that can be set! ;)
}

STDMETHODIMP CDataView::CDataObject::EnumFormatEtc (THIS_ DWORD dwDirection,
			LPENUMFORMATETC FAR* ppenumFormatEtc)
{
	return OleRegEnumFormatEtc(CLSID_CDataView, dwDirection, ppenumFormatEtc);
}


STDMETHODIMP CDataView::CDataObject::DAdvise (THIS_ FORMATETC FAR* pFormatetc, DWORD advf,
		LPADVISESINK pAdvSink, DWORD FAR* pdwConnection)
{
	HRESULT hRes;

	if( !m_pDataView->m_pAdvHolder )
		CreateDataAdviseHolder(&(m_pDataView->m_pAdvHolder));

	if (!m_pDataView->m_pAdvHolder)
		return ResultFromScode(E_OUTOFMEMORY);

	if ((hRes = QueryGetData(pFormatetc)) == NOERROR) {
		hRes = m_pDataView->m_pAdvHolder->Advise(
				this, pFormatetc, advf, pAdvSink, pdwConnection );
	}
	return hRes;
}

STDMETHODIMP CDataView::CDataObject::DUnadvise (THIS_ DWORD dwConnection)
{
	return m_pDataView->m_pAdvHolder->Unadvise(dwConnection);
}

STDMETHODIMP CDataView::CDataObject::EnumDAdvise (THIS_ LPENUMSTATDATA FAR* ppenumAdvise)
{
	return m_pDataView->m_pAdvHolder->EnumAdvise(ppenumAdvise);
}


//  CViewObject implementations

CDataView::CViewObject::CViewObject( CDataView FAR * pDataView )
{
	m_pDataView = pDataView;
	m_fFrozen = FALSE;
}

STDMETHODIMP CDataView::CViewObject::QueryInterface (REFIID riid, LPVOID FAR* ppvObj)
{
	return m_pDataView->QueryInterface(riid, ppvObj);
}

STDMETHODIMP_(ULONG) CDataView::CViewObject::AddRef (void)
{
	return m_pDataView->AddRef();
}

STDMETHODIMP_(ULONG) CDataView::CViewObject::Release (void)
{
	return m_pDataView->Release();
}

STDMETHODIMP CDataView::CViewObject::Draw (DWORD dwDrawAspect, LONG lindex,
		void FAR* pvAspect, DVTARGETDEVICE FAR * ptd,
		HDC hicTargetDev,
		HDC hdcDraw,
		LPCRECTL lprcBounds,
		LPCRECTL lprcWBounds,
		BOOL (CALLBACK * pfnContinue) (DWORD),
		DWORD dwContinue)
{
	/*
	 *OLE2NOTE: If we are not asked to draw our content (the "normal"
	 *thing to do), then delegate the call to our cache handler so that it
	 *can draw the user-supplied icon (or whatever is needed).
	 */
	if( dwDrawAspect != DVASPECT_CONTENT ){
		if(m_pDataView->m_oleCache.m_pDataCache)
			return m_pDataView->m_oleCache.m_pViewObject->Draw(dwDrawAspect, lindex,
				pvAspect, ptd, hicTargetDev, hdcDraw, lprcBounds, lprcWBounds,
				pfnContinue, dwContinue);
		else
			return ResultFromScode(DV_E_DVASPECT);
	}

	// if we are drawing to the screen, then auto run (connect to RealTime)
	if (GetDeviceCaps(hdcDraw, TECHNOLOGY) == DT_RASDISPLAY)
		m_pDataView->SelfLock(TRUE);

	m_pDataView->m_stockInfo.Draw(hdcDraw, lprcBounds);

	return NOERROR;
}

STDMETHODIMP CDataView::CViewObject::GetColorSet (DWORD dwDrawAspect, LONG lindex,
		void FAR* pvAspect, DVTARGETDEVICE FAR * ptd,
		HDC hicTargetDev,
		LPLOGPALETTE FAR* ppColorSet)
{
	if( ppColorSet )
		*ppColorSet = NULL;

	// if DVASPECT_ICON is cached, then delegate to cache;
	// else we must handle it ourselves. in our case we do not
	// require a special pallet thus we return S_FALSE.
	if( (dwDrawAspect & DVASPECT_ICON) && m_pDataView->m_oleCache.m_pViewObject)
		return m_pDataView->m_oleCache.m_pViewObject->GetColorSet(dwDrawAspect,
			lindex, pvAspect, ptd, hicTargetDev, ppColorSet);

	return ResultFromScode(S_FALSE);
}

STDMETHODIMP CDataView::CViewObject::Freeze(DWORD dwDrawAspect, LONG lindex,
		void FAR* pvAspect,
		DWORD FAR* pdwFreeze)
{
	if( m_fFrozen )
		return ResultFromScode(VIEW_S_ALREADY_FROZEN);

	m_fFrozen = TRUE;

	//turn off our advises from realtime (but stay in whatever mode we were in)
	if( m_pDataView->m_pSrcDataObject && m_pDataView->m_dwSrcDataConn ) {
		m_pDataView->m_pSrcDataObject->DUnadvise(m_pDataView->m_dwSrcDataConn);
		m_pDataView->m_dwSrcDataConn = 0l;
	}

	if( pdwFreeze )
		*pdwFreeze = 1l;
	return NOERROR;
}

STDMETHODIMP CDataView::CViewObject::Unfreeze (DWORD dwFreeze)
{
	if( !m_fFrozen )
		return NOERROR;

	m_fFrozen = FALSE;

	if( !m_pDataView->m_stockInfo.GetMode() ) //if automatic mode
		if( m_pDataView->m_dwSrcDataConn == 0 && m_pDataView->m_pSrcDataObject ){
			m_pDataView->m_SrcFmtEtc.cfFormat = CF_TEXT;
			m_pDataView->m_pSrcDataObject->DAdvise(&(m_pDataView->m_SrcFmtEtc),0
				, m_pDataView->m_pMyAdvSink, &(m_pDataView->m_dwSrcDataConn));
		}
	return NOERROR;
}

STDMETHODIMP CDataView::CViewObject::SetAdvise (DWORD aspects, DWORD advf,
		LPADVISESINK pAdvSink)
{
	if( ! (aspects & (DVASPECT_CONTENT | DVASPECT_ICON)) )
		return ResultFromScode(DV_E_DVASPECT);

	if (m_pDataView->m_pViewAdvSink != NULL){
		m_pDataView->m_pViewAdvSink->Release();
		m_pDataView->m_pViewAdvSink = NULL;
	}
	m_aspects = aspects;
	m_advf = advf;

	if( pAdvSink != NULL ){
		pAdvSink->AddRef();             //we are now holding on this pointer
		m_pDataView->m_pViewAdvSink = pAdvSink;
		/*
		 *OLE2NOTE: if the advise flag specifies PRIMEFIRST, we must send an
		 *immediate on view change advise
		 *The other advise flags do not apply here.
		 */
		if (advf & ADVF_PRIMEFIRST)
			m_pDataView->m_pViewAdvSink->OnViewChange(DVASPECT_CONTENT, -1);
		return NOERROR;
	} else
		return ResultFromScode(E_FAIL);
}

STDMETHODIMP CDataView::CViewObject::GetAdvise (DWORD FAR* pAspects, DWORD FAR* pAdvf,
		LPADVISESINK FAR* ppAdvSink)
{
	if( pAspects )
		*pAspects = m_aspects;
	if( pAdvf )
		*pAdvf = m_advf;
	if( ppAdvSink ){
		*ppAdvSink = m_pDataView->m_pViewAdvSink;
		if( m_pDataView->m_pViewAdvSink)
			m_pDataView->m_pViewAdvSink->AddRef();
	}
	return NOERROR;
}

/*
 *GetExtent -- IViewObject2 method
 *If the requested aspect is CONTENT (i.e., our own data), then return the size of our
 *text output (using OleObject::GetExtent to avoid code duplication).
 *Otherwise, we do not know the size of the requested format--if a cache has been created,
 *delegate to it in case an icon or other format has been stored there (possibly by the
 *user).
 */
STDMETHODIMP CDataView::CViewObject::GetExtent( DWORD dwDrawAspect, LONG lindex,
				DVTARGETDEVICE FAR *ptd, LPSIZEL  lpsizel)
{
	if( dwDrawAspect == DVASPECT_CONTENT )
		return m_pDataView->m_oleObject.GetExtent(dwDrawAspect, lpsizel);
	else if (m_pDataView->m_oleCache.m_pViewObject) //delegate to the cache (if it exists)
		return m_pDataView->m_oleCache.m_pViewObject->GetExtent(dwDrawAspect, lindex,
				ptd, lpsizel);
	return ResultFromScode(E_FAIL);
}

CDataView::CAdvSink::CAdvSink( CDataView FAR * pDataView )
{
  g_dwObjectCount ++;
	m_pDataView = pDataView;
	m_refs = 0;
}


CDataView::CAdvSink::~CAdvSink()
{
  g_dwObjectCount --;
}

// release back pointer to CDataView object. this is called from
// the CDataView object's destructor.
void CDataView::CAdvSink::Disconnect( void )
{
	m_pDataView = NULL;
}

STDMETHODIMP CDataView::CAdvSink::QueryInterface (REFIID riid, LPVOID FAR* ppvObj)
{
	*ppvObj = NULL;
	if (riid == IID_IUnknown || riid == IID_IAdviseSink)
		*ppvObj = (LPVOID) this;
	if (*ppvObj)
	{
		AddRef();
		return NOERROR;
	}
	return ResultFromScode(E_NOINTERFACE);
}


STDMETHODIMP_(ULONG) CDataView::CAdvSink::AddRef (void)
{
	return ++m_refs;
}


STDMETHODIMP_(ULONG) CDataView::CAdvSink::Release (void)
{
	if (--m_refs == 0)
	{
		delete this;
		return 0;
	}
	return m_refs;
}



		// *** IAdviseSink methods ***
STDMETHODIMP_(void) CDataView::CAdvSink::OnDataChange(FORMATETC FAR* pFormatetc,
					STGMEDIUM FAR* pStgmed)
{
	LPSTR lpsz;

	/*
	 * OLE2NOTE:
	 * since our AdviseSink is a separate memory entity from DataView, be sure
	 * we don't access it if it has been deleted.
	 */

	if( !m_pDataView )
		return;

	if (pFormatetc->cfFormat == CF_TEXT)
	{
		lpsz = (LPSTR)GlobalLock(pStgmed->hGlobal);
		if (m_pDataView->m_stockInfo.m_lpszText)
			delete m_pDataView->m_stockInfo.m_lpszText;

		m_pDataView->m_stockInfo.m_lpszText = new FAR char[ 1 + _fstrlen(lpsz) ];
		_fstrcpy(m_pDataView->m_stockInfo.m_lpszText, lpsz);

		m_pDataView->m_stockInfo.UpdateDisplayText();   //set the display text and bounding
													//box

		GlobalUnlock(pStgmed->hGlobal);
		ReleaseStgMedium(pStgmed);

		/*
		 * OLE2NOTE: it is possible (although unlikely) that OnViewChange will
		 * call back around and unadvise us (thus releasing the memory the this
		 * pointer refers to).  This is an example of the more general issue of
		 * concurrency and re-entrant code.
		 * The AddRef ensures that both the memory for dataview and the advise
		 * sink (this) are stable.  (since dataview holds a pointer to the advise
		 * sink, we do not need to separately addref the advise sink (this)
		 */
		m_pDataView->AddRef();

		if (m_pDataView->m_pViewAdvSink){
			//OLE2NOTE: don't view advise if the container does not care about our content
			//(see ViewObject->SetAdvise)
			if( m_pDataView->m_viewObject.m_aspects & DVASPECT_CONTENT )
				m_pDataView->m_pViewAdvSink->OnViewChange(DVASPECT_CONTENT, -1);
		}

		//notify any advises that our data has changed
		if( m_pDataView->m_pAdvHolder)
			m_pDataView->m_pAdvHolder->SendOnDataChange(&(m_pDataView->m_dataObject), 0, 0);

		//take us out of self-stabilizing mode
		m_pDataView->Release();

	}
}


STDMETHODIMP_(void) CDataView::CAdvSink::OnViewChange(DWORD dwAspect, LONG lindex)
{
//since we only care about data changes from our data source (realtime), this
//function can be empty
}


STDMETHODIMP_(void) CDataView::CAdvSink::OnRename(LPMONIKER pmk)
{
//since we only care about data changes from our data source (realtime), this
//function can be empty
}


STDMETHODIMP_(void) CDataView::CAdvSink::OnSave(void)
{
//since we only care about data changes from our data source (realtime), this
//function can be empty
}


STDMETHODIMP_(void) CDataView::CAdvSink::OnClose(void)
{
//since we only care about data changes from our data source (realtime), this
//function can be empty
}


BOOL CDataView::Connect(void)
{
	LPBINDCTX pbc;
	HRESULT hresult = ResultFromScode(E_FAIL);
	LPDATAOBJECT pOldSrcObj = NULL;

	// Disconnect from current data source
	if (m_pSrcDataObject) {
		if( m_dwSrcDataConn ){
			m_pSrcDataObject->DUnadvise(m_dwSrcDataConn);
			m_dwSrcDataConn = 0;
		}

		if (m_stockInfo.m_lpszText) {
			delete m_stockInfo.m_lpszText;
			m_stockInfo.m_lpszText = NULL;
		}

		pOldSrcObj = m_pSrcDataObject;  // save so it can be released later
		m_pSrcDataObject = NULL;
	}

	if (m_stockInfo.m_pmkDataSource == NULL) {
		if (pOldSrcObj != NULL) pOldSrcObj->Release();
		return FALSE;
	}

	CreateBindCtx(0, &pbc);
	hresult = m_stockInfo.m_pmkDataSource->
		BindToObject(pbc, NULL, IID_IDataObject, (LPVOID FAR*)&(m_pSrcDataObject));

	if (hresult == NOERROR && m_pSrcDataObject)
	{
		m_SrcFmtEtc.cfFormat = CF_TEXT;

		OleDbgAssert(m_pMyAdvSink);

		if( !m_stockInfo.GetMode() /*automatic mode*/ && m_pMyAdvSink )
		{
			m_pSrcDataObject->DAdvise(&m_SrcFmtEtc,0, m_pMyAdvSink, &m_dwSrcDataConn);
			UpdateData();
		}
	}
	pbc->Release();

	if (pOldSrcObj != NULL) pOldSrcObj->Release();

	m_fConnect = TRUE;  //regardless of the error status, we have tried to
						//connect. (see GetRealTimeParam)
	return (hresult == NOERROR);
}

/*
 * UpdateData -- manualy fetch data from the realtime server
 */
BOOL CDataView::UpdateData(void)
{
	LPSTR lpsz;
	STGMEDIUM stgMed;

	m_SrcFmtEtc.cfFormat = CF_TEXT;
	if (!m_pSrcDataObject)
		return FALSE;
	HRESULT hrErr = m_pSrcDataObject->GetData(&m_SrcFmtEtc, &stgMed);
	if (SUCCEEDED(hrErr))
	{
		lpsz = (LPSTR)GlobalLock(stgMed.hGlobal);
		if (m_stockInfo.m_lpszText)
			delete m_stockInfo.m_lpszText;
		m_stockInfo.m_lpszText = new FAR char[ 1 + _fstrlen(lpsz)];
		_fstrcpy(m_stockInfo.m_lpszText, lpsz);
		GlobalUnlock(stgMed.hGlobal);
		ReleaseStgMedium(&stgMed);
		m_stockInfo.UpdateDisplayText();

		if (m_pViewAdvSink)
			m_pViewAdvSink->OnViewChange(DVASPECT_CONTENT, -1);
		if( m_pAdvHolder)
			m_pAdvHolder->SendOnDataChange(&(m_dataObject), 0, 0);
	}
	return TRUE;
}

/*
 * SelfLock -- take us into/out of the self-locked state
 */
void CDataView::SelfLock( BOOL fLock )
{
	if( fLock == TRUE ) {
		if( !m_fIsSelfLocked ) {
			m_fIsSelfLocked = TRUE;
			m_runnableObject.Run(NULL);
			m_runnableObject.LockRunning(TRUE, 0);
		}
	} else {
		if( m_fIsSelfLocked ) {
			m_fIsSelfLocked = FALSE;
			m_runnableObject.LockRunning(FALSE, TRUE);
		}
	}
}
