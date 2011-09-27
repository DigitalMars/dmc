/*
dvcache.cpp

	This module implements the COleCache methods of the DataView Ojbect.
COleCache internally holds an instance of a DataCache, an OLE-supplied
helper API which does most of the caching work for us.  The COleCache
mostly delegates to the DataCache.

	The DataCache is held internally and not aggregated into CDataView
for example purposes only.  Either technique is acceptable (although in
this instance, aggregation would result in less code, as most of the
interface methods are simply delegated to the DataCache).

(c) Copyright Microsoft Corp. 1992 - 1993 All Rights Reserved

*/

#include "dataview.h"
#include <olestd.h>         //for OleDbgAssert[Sz]

// This line is needed for the debug utilities in OLE2UI
extern "C" {
	OLEDBGDATA
}

/*
 * COleCache constructor -- here we grab a pointer to the top level
 * DataView object for reference.  The DataCache object is not created
 * until we need it (namely when the DataView object becomes running).
 */
CDataView::COleCache::COleCache( CDataView FAR *pDataView)
{
	static STATDATA z;  //zero'ed

	m_pDataView = pDataView;
	m_pDataCache = NULL;
	m_pViewObject = NULL;
	m_pDataObject = NULL;
	m_pPersistStorage = NULL;

	//now we initialize the STATDATA array to the formats we support
	//if this is modified, be sure to change the implementation of IOleCache::Cache
	m_rgStatData[0] = z;
	m_rgStatData[0].formatetc.cfFormat = CF_TEXT;
	m_rgStatData[0].formatetc.dwAspect = DVASPECT_CONTENT;
	m_rgStatData[1] = z;
	m_rgStatData[1].formatetc.cfFormat = CF_METAFILEPICT;
	m_rgStatData[1].formatetc.dwAspect = DVASPECT_CONTENT;
	m_rgStatData[2] = z;
	m_rgStatData[2].formatetc.dwAspect = DVASPECT_CONTENT;  //NULL format
	m_rgStatData[3] = z;
	m_rgStatData[3].formatetc.cfFormat = CF_METAFILEPICT;
	m_rgStatData[3].formatetc.dwAspect = DVASPECT_ICON;

}

CDataView::COleCache::~COleCache(void)
{
	if (m_pDataCache) m_pDataCache->Release();
	if (m_pViewObject) m_pViewObject->Release();
	if (m_pDataObject) m_pDataObject->Release();
	if (m_pPersistStorage) m_pPersistStorage->Release();
	if (m_pOleObject) m_pOleObject->Release();
}

/*
 * Setup -- create an instance of a DataCache and in general set things
 * up to use the cache
 */
HRESULT CDataView::COleCache::Setup(void)
{
	if( !m_pDataCache ){
		HRESULT hr = CreateDataCache(NULL, CLSID_CDataView, IID_IOleCache,
			(LPVOID FAR *)&m_pDataCache);
		if( hr == S_OK )
			hr = m_pDataCache->QueryInterface(IID_IViewObject2,
					(LPVOID FAR *)&m_pViewObject);
		if( hr == S_OK )
			hr = m_pDataCache->QueryInterface(IID_IDataObject,
					(LPVOID FAR *)&m_pDataObject);
		if( hr == S_OK )
			hr = m_pDataCache->QueryInterface(IID_IPersistStorage,
					(LPVOID FAR *)&m_pPersistStorage);
		if( hr == S_OK )
			hr = m_pDataCache->QueryInterface(IID_IOleObject,
					(LPVOID FAR *)&m_pOleObject );
		return hr;
	}else
		return NOERROR;
}

/*
 *COleCache implementations of IOleCache methods
 */

/*
 *QueryInterface -- simply delegate to the top level
 */

STDMETHODIMP CDataView::COleCache::QueryInterface(REFIID riid,
		LPVOID FAR *ppvObj)
{
	return m_pDataView->QueryInterface(riid, ppvObj);
}

/*
 *AddRef -- delegated to the top level.  This is necessary to keep
 *reference counts for the aggregate as a whole
 */

STDMETHODIMP_(ULONG) CDataView::COleCache::AddRef(void)
{
	return m_pDataView->AddRef();
}

/*
 *Release -- delegated to the top level.  See AddRef()
 */

STDMETHODIMP_(ULONG) CDataView::COleCache::Release(void)
{
	return m_pDataView->Release();
}

/*
 *Cache -- specify a data format that should be be cached.  This
 *function is delegate to the DataCache
 */

STDMETHODIMP CDataView::COleCache::Cache(LPFORMATETC lpFormatetc,
		DWORD advf, LPDWORD pdwConnection)
{
	HRESULT hresult;
	DWORD dwConn;

	if( !lpFormatetc)
		return ResultFromScode(E_INVALIDARG);

	if ( lpFormatetc->lindex != -1 )
		return ResultFromScode(DV_E_LINDEX);

	if ( (advf & ADVFCACHE_FORCEBUILTIN && lpFormatetc->dwAspect == DVASPECT_CONTENT) ||
		lpFormatetc->dwAspect == DVASPECT_ICON) {
		/*
		 * OLE2NOTE: DVASPECT_ICON and FORCEBUILTIN CONTENT are the two cases which force
		 * us to maintain an explicit cache.
		 *
		 * It is usually sufficient to simply delegate to the default
		 * cache implementation
		 */
		if( !m_pDataCache )
			Setup();
		hresult = m_pDataCache->Cache(lpFormatetc, advf, (LPDWORD)&dwConn);
		if( hresult == NOERROR ){
			//get these values from the constructor
			int index = (lpFormatetc->dwAspect == DVASPECT_ICON)? 3 : (lpFormatetc->cfFormat == CF_METAFILEPICT) ?
				1 : (lpFormatetc->cfFormat == CF_TEXT) ? 0 : 2;
			m_rgStatData[index].dwConnection = dwConn;

			if( pdwConnection ) //pdwConnection may be NULL
				*pdwConnection = dwConn;
		}
		return hresult;
	}


	//we will implicitly cache DVASPECT_CONTENT

	if ( lpFormatetc->dwAspect == DVASPECT_CONTENT) {
		if ( (lpFormatetc->cfFormat == NULL) || (lpFormatetc->cfFormat == CF_METAFILEPICT)) {
			if (pdwConnection)
				*pdwConnection = NULL;
			return ResultFromScode(CACHE_S_SAMECACHE);
		}

		return ResultFromScode(DV_E_CLIPFORMAT);
	}

	//if we get this far, then we have been asked to cache something we don't support.
	//(dataview is not a general purpose data depository! :)
	return ResultFromScode(DV_E_CLIPFORMAT);

}

/*
 *Uncache -- delete a cache connection previously established in a call
 *to Cache.  This function is delegated to the DataCache
 */

STDMETHODIMP CDataView::COleCache::Uncache(DWORD dwConnection)
{
	int i = 0;
	//The DataCache must be instantiated before we get here
	OleDbgAssertSz(m_pDataCache,
		"Attempting to uncache before COleCache created");

	if ( !m_pDataCache )
		return ResultFromScode(OLE_E_NOCONNECTION);

	//we must also remove the entry in our StatData array

	for(; i < DV_NUMFORMATS ; i++ )
		if( m_rgStatData[i].dwConnection == dwConnection )
			m_rgStatData[i].dwConnection = 0;


	return m_pDataCache->Uncache(dwConnection);
}

/*
 *EnumCache -- return an enumeration of all the current cache connections
 *This function is not delegated to the DataCache, as we keep track of both
 *implicit and explicit cache connections within our IOleCache::Cache implementation
 */

STDMETHODIMP CDataView::COleCache::EnumCache(
		LPENUMSTATDATA FAR *ppenumStatData )
{
	if (! ppenumStatData)
		return ResultFromScode(E_INVALIDARG);

	*ppenumStatData = OleStdEnumStatData_Create(
			DV_NUMFORMATS, (LPSTATDATA)m_rgStatData);
	return (*ppenumStatData ? NOERROR : ResultFromScode(E_OUTOFMEMORY));
}

/*
 *InitCache -- fill the cache with data from a Clipboard Data Object.
 *This function is delegated to the DataCache
 */

STDMETHODIMP CDataView::COleCache::InitCache(LPDATAOBJECT pDataObject)
{
	if ( !m_pDataCache )
		return NOERROR;

	return m_pDataCache->InitCache(pDataObject);
}

/*
 *SetData -- fill a cache with data from pmedium.
 *This function is delegated to the DataCache
 */

STDMETHODIMP CDataView::COleCache::SetData(LPFORMATETC pformatetc,
	STGMEDIUM FAR *pmedium, BOOL fRelease )
{
	if ( !m_pDataCache )
		return ResultFromScode(E_FAIL);

	return m_pDataCache->SetData(pformatetc, pmedium, fRelease);
}

/*
 * IOleCache2 Methods
 */

/*
 * UpdateCache
 * This function is simply delegated to the DataCache
 */

STDMETHODIMP CDataView::COleCache::UpdateCache( LPDATAOBJECT pDataObject,
	DWORD grfUpdf, LPVOID pReserved)
{
	if ( !m_pDataCache )
		return ResultFromScode(E_FAIL);

	return m_pDataCache->UpdateCache(pDataObject, grfUpdf, pReserved);
}

/*
 * UpdateCache
 * This function is simply delegated to the DataCache
 */

STDMETHODIMP CDataView::COleCache::DiscardCache(DWORD dwDiscardOptions)
{
	if ( !m_pDataCache )
		return ResultFromScode(E_FAIL);

	return m_pDataCache->DiscardCache(dwDiscardOptions);
}
