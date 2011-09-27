/*
	dvstock.cpp

	This file contains the method implementations of the StockInfo structure,
	which is used to keep track of dataview specific information (versus OLE2
	related information)

	(c) Copyright Microsoft Corp. 1992 - 1993 All Rights Reserved

*/

#include "dataview.h"
#include "stockutl.h"
#include <olestd.h>

// This line is needed for the debug utilities in OLE2UI
extern "C" {
	OLEDBGDATA
}

/*
 * The StockInfo constructor.  The StockInfo class is used to hold the stock data of
 * the dataview object
 */
CDataView::StockInfo::StockInfo( CDataView FAR *pDataView)
{
	m_pDataView = pDataView;
	m_lpszText = NULL;

	lstrcpy((LPSTR)ss.szName, (LPSTR)"MSFT");
	lstrcpy((LPSTR)ss.szService, "@Realtime");
	ss.nLimit = 800;
	m_lpszDisplayText = NULL;
	ss.fManual = FALSE;
	ss.fBeep = TRUE;
	ss.fAlarm = FALSE;
	m_pmkDataSource = NULL;
}

/*
 * The StockInfo destructor.
 */
CDataView::StockInfo::~StockInfo()
{
	if( m_lpszText )
		delete m_lpszText;
	if( m_lpszDisplayText )
		delete m_lpszDisplayText;
	if( m_pmkDataSource )
		m_pmkDataSource->Release();
}


/*
 * Save -- store the important data in Stockinfo to the given stream
 * A real app obviously needs a more robust write, checking for error conditions
 */
HRESULT CDataView::StockInfo::Save( LPSTREAM pStream)
{
	return pStream->Write( &ss, sizeof(StockInfo::SaveSet), NULL);
}

/*
 * Load -- retreive the important data from the Stream and put it into stock
 * info structure (this)
 */
HRESULT CDataView::StockInfo::Load( LPSTREAM pStream)
{
	return pStream->Read( &ss, sizeof(StockInfo::SaveSet) , NULL);
}

/*
 * UpdateDisplayText -- this function updates the text string that is displayed
 * by the Draw interface.  Size parameters are also recalcuted if necessary.
 *
 * BUGS:  The size of the text bounding box never decreases.  In a "real" app,
 * some heuristic should be used so that the bounding box is always of
 * "reasonable" size without too rapid size changes
 */

int CDataView::StockInfo::UpdateDisplayText()
{
	HDC hdc = GetDC(NULL);
	int mmOld = SetMapMode( hdc, MM_HIMETRIC), length;
	DWORD dw;

	//in a number of circumstances, m_lpszText will be NULL (e.g. if the realtime
	//server was not in the registration database.  In this case, we just use "N/A"
	//as the stock price

	if( !m_lpszText ){
		m_lpszText = new char[4];
		lstrcpy(m_lpszText, (LPSTR)"N/A");
	}

	if( m_lpszDisplayText )
		delete m_lpszDisplayText;

	m_lpszDisplayText = new FAR char[ (length = (3 + _fstrlen((LPSTR)ss.szName)+
								_fstrlen(m_lpszText))) ];

	wsprintf(m_lpszDisplayText, "%s: %s", ss.szName, m_lpszText);

	dw = GetTextExtent( hdc, m_lpszDisplayText, length);

	if( (int)(LOWORD(dw)) > m_pDataView->m_lastSizeL.cx )
		m_pDataView->m_lastSizeL.cx =  (int)(LOWORD(dw));
	if( (int)(HIWORD(dw)) > m_pDataView->m_lastSizeL.cy )
		m_pDataView->m_lastSizeL.cy = (int)(HIWORD(dw));

	SetMapMode( hdc, mmOld);
	ReleaseDC(NULL, hdc);

	return 1;               //this return value could be used to indicated whether the
					//bounding box changed size, etc.
}


/*
 * SetPriceLimit -- sets the value at which the display of the stock price should
 * turn red.  Must in integral 1/8ths of dollars (i.e. a stock price of 3 1/8 would
 * have a value of 25
 *
 * This value can also be set by the user in the DoVerb dialog
 */
void CDataView::StockInfo::SetPriceLimit( long nLimit, BOOL fBeep )
{
	if (ss.nLimit != nLimit || ss.fBeep != fBeep) {
		m_pDataView->m_fIsDirty = TRUE;
		ss.nLimit = nLimit;
		ss.fBeep = fBeep;
	}
}

/*
 * GetPriceLimit -- return the value of the stock price ceiling (see SetPriceLimit)
 */

long CDataView::StockInfo::GetPriceLimit( void )
{
	return ss.nLimit;
}

BOOL CDataView::StockInfo::SetServiceName( LPSTR lpszService)
{
	if( lpszService && lstrcmp(lpszService, ss.szService) != 0) {
		m_pDataView->m_fIsDirty = TRUE;
		lstrcpy((LPSTR)ss.szService, lpszService);
		return TRUE;
	}
	return FALSE;
}


/*
 * SetStockName -- set the name of the stock to be watched.  Success (TRUE) or
 * failure (FALSE) is returned.
 *
 */

BOOL CDataView::StockInfo::SetStockName( LPSTR lpszName, BOOL fSetServiceName)
{
	HRESULT hresult;
	LPSTR szBuff;
	ULONG cbEaten;
	LPBINDCTX pbc;

	if( lpszName &&
		(fSetServiceName || !m_pmkDataSource ||
		 lstrcmp(lpszName, ss.szName) != 0) ) {
		m_pDataView->m_fIsDirty = TRUE;
		lstrcpy((LPSTR)ss.szName, lpszName);
		if (m_pmkDataSource)
			m_pmkDataSource->Release(); //other releases (such as to advises) are done in Connect()

		szBuff = new FAR char[lstrlen((LPSTR)ss.szService) + lstrlen(lpszName) + 2];
		wsprintf(szBuff, "%s\\%s", (LPSTR)ss.szService, lpszName );
		hresult = CreateBindCtx(0, &pbc);
		if (hresult != NOERROR) {
			OleDbgOutHResult("CreateBindCtx returned", hresult);
			delete szBuff;
			return FALSE;
		}
		hresult = MkParseDisplayName( pbc, szBuff, &cbEaten, &m_pmkDataSource);
		delete szBuff;
		pbc->Release();
		m_pDataView->Connect(); // on error, this will disconnect old source
		if (FAILED(hresult))
		{
			OleDbgOutHResult("MkParseDisplayName returned", hresult);
			return FALSE;
		}
	}
	return TRUE;
}

/*
 * GetStockName -- return a pointer to the stock name string, or NULL on
 * failure.
 *
 */

LPSTR CDataView::StockInfo::GetStockName( void )
{
	return (ss.szName[0])? (LPSTR)ss.szName : NULL;
}

LPSTR CDataView::StockInfo::GetServiceName( void )
{
	return (ss.szService[0])? (LPSTR)ss.szService : NULL;
}


/*
 * SetRealTimeParam -- set the parameters of the realtime executable (which
 * generates stock data).  This function illustrates the use of inter-process
 * data communication.
 *
 * Jitter is the percentage variation in each generation of stock quotes
 * UpdInt is the UpdateInterval--the rate at which stock data should be sent to
 * the client object (this object)
 *
 * TRUE/FALSE returned upon success/failure
 */


struct RTItemData
{
	int nLastValue;
	int nJitter;
	int nPeriod;
};

BOOL CDataView::StockInfo::SetRealTimeParam( int Jitter, int UpdInt)
{
	FORMATETC formatetc;
	STGMEDIUM stgmed;
	HRESULT hresult = ResultFromScode(E_FAIL);

	formatetc.cfFormat = cfRealtimeItem;
	formatetc.ptd = NULL;
	formatetc.dwAspect = -1;
	formatetc.lindex = -1;
	formatetc.tymed = TYMED_HGLOBAL;

	stgmed.tymed = TYMED_HGLOBAL;
	stgmed.hGlobal = GlobalAlloc( GMEM_SHARE, sizeof( RTItemData ));
	stgmed.pUnkForRelease = NULL;

	RTItemData FAR* pItemData = (RTItemData FAR*)GlobalLock(stgmed.hGlobal);
	pItemData->nLastValue = 0;
	pItemData->nJitter = Jitter;
	pItemData->nPeriod = UpdInt;
	GlobalUnlock(stgmed.hGlobal);

	if( m_pDataView->m_pSrcDataObject )
		hresult = m_pDataView->m_pSrcDataObject->SetData(&formatetc, &stgmed, FALSE);

	//OLE2NOTE: ReleaseStgMedium will call GlobalFree on the hGlobal handle
	ReleaseStgMedium(&stgmed);

	return (hresult == NOERROR);
}

/*
 * GetRealTimeParam -- access the realtime executable and get the current
 * parameter settings.  (See SetRealTimeParam)
 */

BOOL CDataView::StockInfo::GetRealTimeParam( LPINT pJitter, LPINT pUpdInt )
{
		RTItemData FAR * pItemData;
		BOOL fRet = FALSE;
		STGMEDIUM stgMed;

		/*
		 *fConnect tells us if we have attempted a connection to the
		 *realtime server or not.  This is needed for the startup case,
		 *where we have created a new DataView object but not yet connected
		 *it to the server.  We want our dialog (see stockdlg.cpp) to not
		 *give us an error condition, but instead use default values.
		 *Note, however, that we can *not* be connected to the realtime server
		 *and still have fConnect == TRUE.  This case is actually an error
		 *(the realtime server not present on the machine, for instance, so it
		 *must be handled appropriately.  (here, we just return FALSE if that
		 *is the case)
		 */

		if (!m_pDataView->m_fConnect ){
			*pJitter = 10l;
			*pUpdInt = 5l;
			return TRUE;
		}
		if (m_pDataView->m_pSrcDataObject == NULL)
			return FALSE;

		m_pDataView->m_SrcFmtEtc.cfFormat = cfRealtimeItem;
		HRESULT hrErr = m_pDataView->m_pSrcDataObject->GetData(&m_pDataView->m_SrcFmtEtc, &stgMed);
		if (SUCCEEDED(hrErr))
		{
				pItemData = (RTItemData FAR*)GlobalLock(stgMed.hGlobal);
				if (pItemData)
				{
					if (pItemData->nJitter != 0 && pJitter) *pJitter = pItemData->nJitter;
					if (pItemData->nPeriod != 0 && pUpdInt) *pUpdInt = pItemData->nPeriod;
					GlobalUnlock(stgMed.hGlobal);
					fRet = TRUE;
				}
				ReleaseStgMedium(&stgMed);
		}
		m_pDataView->m_SrcFmtEtc.cfFormat = CF_TEXT;
		return fRet;
}

/*
 * GetMode -- returns the current mode, 0 for automatic update, 1 for manual
 */
BOOL CDataView::StockInfo::GetMode(void)
{
	return ss.fManual;
}

/*
 * GetBeep -- returns whether or not the beeps are activated for alarm mode
 */
BOOL CDataView::StockInfo::GetBeep(void)
{
	return ss.fBeep;
}

/*
 * SetAutoMode -- Set up the Advise sinks, etc. for automatic updating of data
 */
BOOL CDataView::StockInfo::SetAutoMode(void)
{
	if (ss.fManual)
	{
		m_pDataView->m_fIsDirty = TRUE;
		ss.fManual = FALSE;
		m_pDataView->m_SrcFmtEtc.cfFormat = CF_TEXT;
		if (m_pDataView->m_dwSrcDataConn == 0 && m_pDataView->m_pSrcDataObject)
			m_pDataView->m_pSrcDataObject->DAdvise(&(m_pDataView->m_SrcFmtEtc),
				0, m_pDataView->m_pMyAdvSink, &(m_pDataView->m_dwSrcDataConn));
	}
	return TRUE;
}

/*
 * SetManualMode -- Turn of advise sinks, etc.  Use UpdateData to refresh the
 * stock price
 */
BOOL CDataView::StockInfo::SetManualMode(void)
{
	if (!ss.fManual && m_pDataView->m_pSrcDataObject)
	{
		m_pDataView->m_pSrcDataObject->DUnadvise(m_pDataView->m_dwSrcDataConn);
		m_pDataView->m_dwSrcDataConn = 0L;
	}
	if( !ss.fManual ) {
		m_pDataView->m_fIsDirty = TRUE;
		ss.fManual = TRUE;
	}
	return TRUE;
}

/*
 * Draw -- Draw the dataview item into a given hdc
 * OLE2NOTE:  While sufficient for this sample app, the drawing code
 * below is not enough for more complicated apps.  See the function
 * FastDraw in xobject.cpp in the xserver sample app for a more
 * complete drawing function (which includes correct coordinate
 * normalization and so forth).
 */
void CDataView::StockInfo::Draw( HDC hdcDraw, LPCRECTL lprcBounds )
{
	DWORD dw = GetTextColor(hdcDraw);
	if ( Str2Stock(m_lpszText) > ss.nLimit){
		if( ss.fAlarm == FALSE )
			MessageBeep(-1);
		ss.fAlarm = TRUE;
		if( ss.fManual ){
			if( dw != RGB(175, 0, 0) )
			SetTextColor(hdcDraw, RGB(175, 0, 0)); //dark red
		}else{
			if( dw != RGB(255, 0, 0) )
				SetTextColor(hdcDraw, RGB(255, 0, 0));
		}
	}else {
		ss.fAlarm = FALSE;
		if( ss.fManual ) {
			if( dw != RGB(128, 128, 128) )
				SetTextColor(hdcDraw, RGB(128, 128, 128));
		}else {
			if( dw != RGB(0,0,0) )
				SetTextColor(hdcDraw, RGB(0,0,0));
		}
	}


	if ( m_lpszDisplayText)
		TextOut (hdcDraw, (int)(lprcBounds->left), (int)(lprcBounds->top), m_lpszDisplayText,
					_fstrlen(m_lpszDisplayText));

	//now restore the text color, so other objects are drawn correctly :)
	SetTextColor(hdcDraw, dw);
}
