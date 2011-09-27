/*
 *  dvrun.cpp
 *
 *      This file contains dataview's implementations of the IRunnableObject
 *      and IExternalConnection interfaces.
 *
 *  (c) Copyright Microsoft Corp. 1992 - 1993 All Rights Reserved
 *
 */

#include "dataview.h"
#include <olestd.h>

extern FORMATETC g_formatetc;

/*
 * CRunnableObject constructor -- here we take a pointer to the top level
 * DataView object for reference (thus allowing us to access it as needed).
 */

CDataView::CRunnableObject::CRunnableObject( CDataView FAR *pDataView )
{
	m_pDataView = pDataView;
}

/*
 * IRunnableObject interface implementations
 */

/*
 * QueryInterface -- simply delegate to the top level (DataView's controlling
 * unknown)
 */
STDMETHODIMP CDataView::CRunnableObject::QueryInterface( REFIID riid,
	LPVOID FAR *ppvObj)
{
	return m_pDataView->QueryInterface(riid, ppvObj);
}

/*
 * AddRef -- delegated to the top level to ensure that reference counts are
 * kept for the object as a whole and not just the specific interfaces
 */
STDMETHODIMP_(ULONG) CDataView::CRunnableObject::AddRef(void)
{
	return m_pDataView->AddRef();
}

/*
 * Release -- delegated to the top level (see AddRef)
 */
STDMETHODIMP_(ULONG) CDataView::CRunnableObject::Release()
{
	return m_pDataView->Release();
}

/*
 * GetRunningClass -- return our class id (found in dataview.h)
 */
STDMETHODIMP CDataView::CRunnableObject::GetRunningClass( LPCLSID lpClsid )
{
	if ( !lpClsid )
		return ResultFromScode(E_INVALIDARG);
	*lpClsid =  CLSID_CDataView;

	return ResultFromScode(S_OK);
}

/*
 * Run -- a DataView object needs both the dll *and* the
 * realtime exe to be considered as running.  Therefore, we
 * check to see if the realtime server is running, and if not,
 * then we start it up
 */
STDMETHODIMP CDataView::CRunnableObject::Run( LPBINDCTX pbc )
{
	LPOLECONTAINER pContainer;
	LPMONIKER pMoniker;

	(void) pbc; //eat up unused arguments

	if( !m_pDataView->m_fIsRunning ){

		/*
		 * OLE2NOTE: Since we are a dll server, we must manually do the things
		 * that an exe's default handler would do, such as locking our container.
		 */

		//we don't hang on to the container because of the following problem: when
		//do we get rid of it (in the ExitRunning method)?  Instead of keeping reference
		//counts and such, it is much easier just to get it as it's needed.
		if( m_pDataView->m_pClientSite )
			if( m_pDataView->m_pClientSite->GetContainer( &pContainer ) == NOERROR ){
				pContainer->LockContainer(TRUE);
				pContainer->Release();
			}


		/*
		 * The following is a Running Object Table optimization hack.  The basic
		 * idea is that we don't want to enter *every* embedded object into the ROT; that
		 * could get very expensive.  The only time that we need to be in there is if
		 * somebody can link or already has a link to us.  To determine this, we
		 * simply ask our container if we have a moniker assigned to us.
		 */

		if( m_pDataView->m_oleObject.GetMoniker(OLEGETMONIKER_ONLYIFTHERE,
			OLEWHICHMK_OBJFULL, &pMoniker) == NOERROR ) {
			OleStdRegisterAsRunning(&(m_pDataView->m_oleObject), pMoniker, &m_pDataView->m_dwRegister);
			pMoniker->Release();
		}

		// Make sure we are connected to the RealTime data source
		if (! m_pDataView->m_pSrcDataObject)
			m_pDataView->Connect();
	}

	m_pDataView->m_fIsRunning = TRUE;

	return NOERROR;
}

/*
 * IsRunning-- return whether or not somebody has set us running via Run
 */
STDMETHODIMP_(BOOL) CDataView::CRunnableObject::IsRunning( void )
{
	static FORMATETC formatetc;     //static to zero it out
	HRESULT hresult = NOERROR;

	//if we are think we are running, check to see if the realtime server is
	//active (in case it has been shut down),  If it is shut down, then restart
	//and reconnect to it.

	if (m_pDataView->m_fIsRunning && m_pDataView->m_pSrcDataObject)
	{
		hresult = m_pDataView->m_pSrcDataObject->QueryGetData(&g_formatetc);
		if( hresult != NOERROR )
			//if this fails were are still running, but in a "dead" state
			m_pDataView->Connect();
	}
	return m_pDataView->m_fIsRunning;
}

/*
 * LockRunning -- lock ourselves as running
 */
STDMETHODIMP CDataView::CRunnableObject::LockRunning ( BOOL fLock,
	BOOL fLastUnlockCloses )
{
	return CoLockObjectExternal(&(m_pDataView->m_oleObject), fLock, fLastUnlockCloses);
}

/*
 * SetContainedObject
 */
STDMETHODIMP CDataView::CRunnableObject::SetContainedObject(
	BOOL fContained )
{
	return NOERROR;
}


/*
 * IExternalConnection methods
 */

CDataView::CExternalConnection::CExternalConnection( CDataView FAR *pDataView )
{
	m_pDataView = pDataView;
	m_dwStrong = 0;
}

/*
 * QueryInterface -- simply delegate to the top level (DataView's controlling
 * unknown)
 */
STDMETHODIMP CDataView::CExternalConnection::QueryInterface( REFIID riid,
	LPVOID FAR *ppvObj)
{
	return m_pDataView->QueryInterface(riid, ppvObj);
}

/*
 * AddRef -- delegated to the top level to ensure that reference counts are
 * kept for the object as a whole and not just the specific interfaces
 */
STDMETHODIMP_(ULONG) CDataView::CExternalConnection::AddRef(void)
{
	return m_pDataView->AddRef();
}

/*
 * Release -- delegated to the top level (see AddRef)
 */
STDMETHODIMP_(ULONG) CDataView::CExternalConnection::Release()
{
	return m_pDataView->Release();
}

/*
 *OLE2NOTE:  IExternalConnection keeps track of external connections to an
 *object (such as moniker bindings, calls to CoLockObjectExternal, etc).
 *When the external connection count goes to zero, the object should be closed.  Ole2
 *dll's (and possibly apps) use this interface to control object shutdown.
 */

/*
 * AddConnection
 */
STDMETHODIMP_(DWORD) CDataView::CExternalConnection::AddConnection(
	DWORD extconn, DWORD reserved)
{
	(void)reserved;     //use up the argument
	if( extconn & EXTCONN_STRONG )
		return ++m_dwStrong;
	else
		return 0;
}

/*
 * ReleaseConnection
 */
STDMETHODIMP_(DWORD) CDataView::CExternalConnection::ReleaseConnection(
	DWORD extconn, DWORD reserved, BOOL fLastReleaseCloses )
{
	(void)reserved;     //use up the argument
	if( extconn & EXTCONN_STRONG ){
		DWORD dwStrong = --m_dwStrong;
		if( !m_dwStrong && fLastReleaseCloses )
			m_pDataView->m_oleObject.Close(OLECLOSE_SAVEIFDIRTY);
		return dwStrong;    //we can't return m_dwStrong because the
							//this pointer may be invalid after Close returns
	} else
		return 0;
}
