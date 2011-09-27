#include "stdafx.h"
#include "realtdoc.h"
#include "utils.h"
#include "rtcntr.h"
#include "rtitem.h"
#include "realtime.h"

#pragma warning( disable : 4355 )

WORD cfRealtimeItem;

CRealtimeItem::CRealtimeItem( LPUNKNOWN pUnkOuter, CRTContainer FAR* pContainer )
	: CControllingUnknown(pUnkOuter), m_dataObject(this)
{
	m_nLastValue = 800;
	m_nJitter = 8;
	m_nPeriod = 2;
	m_cSeconds = 0;
	m_pAdvHolder = NULL;
	m_pContainer = pContainer;
	pContainer->Lock();
}

CRealtimeItem::~CRealtimeItem( void )
{
	POSITION pos;
	CString key;
	CRealtimeItem * pRealtimeItem;

	m_pContainer->Unlock();
	for( pos = theApp.m_pDoc->m_mapList.GetStartPosition(); pos != NULL; )
	{
		theApp.m_pDoc->m_mapList.GetNextAssoc( pos, key, (LPVOID&)pRealtimeItem );
		if (pRealtimeItem == this)
			theApp.m_pDoc->m_mapList.RemoveKey(key);
	}
}


STDMETHODIMP CRealtimeItem::QueryInterface( REFIID riid, LPVOID FAR* ppvObj )
{
	*ppvObj = NULL;
	if (m_pUnkOuter) return m_pUnkOuter->QueryInterface(riid, ppvObj);
	if ( riid == IID_IUnknown )
		*ppvObj = (LPVOID)this;
	else if ( riid == IID_IDataObject )
		*ppvObj = (LPVOID)(&m_dataObject);
	if (NULL != *ppvObj)
	{
		((LPUNKNOWN)(*ppvObj))->AddRef();
		return NOERROR;
	}
	return ResultFromScode(E_NOINTERFACE);
}


CRealtimeItem::CDataObject::CDataObject( CRealtimeItem FAR* pRealtimeItem)
{
	m_pRealtimeItem = pRealtimeItem;
}


STDMETHODIMP CRealtimeItem::CDataObject::QueryInterface( REFIID riid, LPVOID FAR* ppvObj)
{
	return m_pRealtimeItem->QueryInterface(riid, ppvObj);
}

STDMETHODIMP_(ULONG) CRealtimeItem::CDataObject::AddRef(void)
{
	return m_pRealtimeItem->AddRef();
}

STDMETHODIMP_(ULONG) CRealtimeItem::CDataObject::Release(void)
{
	return m_pRealtimeItem->Release();
}




STDMETHODIMP CRealtimeItem::CDataObject::GetData(THIS_ LPFORMATETC pformatetcIn,
							LPSTGMEDIUM pmedium )
{
	pmedium->pUnkForRelease = NULL;
	pmedium->tymed = NULL;
	pmedium->hGlobal = NULL;

	if (pformatetcIn->cfFormat == CF_TEXT)
	{
		if (!(pformatetcIn->tymed & TYMED_HGLOBAL))
			return ResultFromScode( DATA_E_FORMATETC);
		CString str =  m_pRealtimeItem->Eighths();
		LPSTR psz;
		pmedium->hGlobal = GlobalAlloc( GMEM_SHARE | GMEM_MOVEABLE, 1 + str.GetLength());
		psz = (LPSTR)GlobalLock(pmedium->hGlobal);
		_fstrcpy( psz, str );
		GlobalUnlock(pmedium->hGlobal);
		pmedium->tymed = TYMED_HGLOBAL;
		return NOERROR;
	}
	else if (pformatetcIn->cfFormat == cfRealtimeItem)
	{
		RTItemData FAR* pItemData;
		pmedium->hGlobal = GlobalAlloc( GMEM_SHARE, sizeof(RTItemData));
		pItemData = (RTItemData FAR* )GlobalLock(pmedium->hGlobal);
		pItemData->nLastValue = m_pRealtimeItem->m_nLastValue;
		pItemData->nJitter = m_pRealtimeItem->m_nJitter;
		pItemData->nPeriod = m_pRealtimeItem->m_nPeriod;
		GlobalUnlock(pmedium->hGlobal);
		pmedium->tymed = TYMED_HGLOBAL;
		return NOERROR;
	}
	return ResultFromScode( DATA_E_FORMATETC);
}


STDMETHODIMP CRealtimeItem::CDataObject::GetDataHere(THIS_ LPFORMATETC pformatetc,
							LPSTGMEDIUM pmedium )
{
	return NOERROR;
}


STDMETHODIMP CRealtimeItem::CDataObject::QueryGetData(THIS_ LPFORMATETC pformatetc )
{
	return NOERROR;
}




STDMETHODIMP CRealtimeItem::CDataObject::GetCanonicalFormatEtc(THIS_ LPFORMATETC pformatetc,
							LPFORMATETC pformatetcOut)
{
	return NOERROR;
}


STDMETHODIMP CRealtimeItem::CDataObject::SetData(THIS_ LPFORMATETC pformatetc, STGMEDIUM FAR * pmedium,
							BOOL fRelease)
{
	RTItemData FAR * pItemData;
	if (pformatetc->cfFormat == cfRealtimeItem)
	{
		pItemData = (RTItemData FAR*)GlobalLock(pmedium->hGlobal);
		if (pItemData != NULL)
		{
			if (pItemData->nJitter != 0) m_pRealtimeItem->m_nJitter = pItemData->nJitter;
			if (pItemData->nPeriod != 0) m_pRealtimeItem->m_nPeriod = pItemData->nPeriod;
			if (pItemData->nLastValue != 0) m_pRealtimeItem->m_nLastValue = pItemData->nLastValue;
			GlobalUnlock(pmedium->hGlobal);
			return NOERROR;
		}
	}
	return ResultFromScode( DATA_E_FORMATETC ); //  separate code, perhaps, for bad hGlobal
}


STDMETHODIMP CRealtimeItem::CDataObject::EnumFormatEtc(THIS_ DWORD dwDirection,
							LPENUMFORMATETC FAR* ppenumFormatEtc)
{
	return NOERROR;
}


STDMETHODIMP CRealtimeItem::CDataObject::DAdvise(THIS_ FORMATETC FAR* pFormatetc, DWORD advf,
					LPADVISESINK pAdvSink, DWORD FAR* pdwConnection)
{
	if (m_pRealtimeItem->m_pAdvHolder == NULL)
		CreateDataAdviseHolder(&(m_pRealtimeItem->m_pAdvHolder));
	return m_pRealtimeItem->m_pAdvHolder->Advise( this, pFormatetc, advf, pAdvSink, pdwConnection);
}


STDMETHODIMP CRealtimeItem::CDataObject::DUnadvise(THIS_ DWORD dwConnection)
{
	if (m_pRealtimeItem->m_pAdvHolder == NULL) return ResultFromScode(E_FAIL);
	return m_pRealtimeItem->m_pAdvHolder->Unadvise(dwConnection);
}




STDMETHODIMP CRealtimeItem::CDataObject::EnumDAdvise(THIS_ LPENUMSTATDATA FAR* ppenumAdvise)
{
	return NOERROR;
}


BOOL CRealtimeItem::Iterate(void)       //  Returns TRUE if value changed.
{
	LPDATAOBJECT pDataObject;
	LONG lLastValue = m_nLastValue;
	LONG lJitter = m_nJitter;
	LONG lRand;

	if (++m_cSeconds >= m_nPeriod)
	{
		lRand = rand();

		m_nLastValue = m_nLastValue + (int)(lJitter * (lRand  - RAND_MAX/2)/RAND_MAX);
		if (m_nLastValue < 2) m_nLastValue = 2;
		if (m_nLastValue > RAND_MAX) m_nLastValue = RAND_MAX;
		//  possible wrap-around problems here
		if (m_pAdvHolder)
		{
				if (NOERROR == QueryInterface(IID_IDataObject, (LPVOID FAR*)&pDataObject))
				{
					m_pAdvHolder->SendOnDataChange(pDataObject, 0, 0);
					pDataObject->Release();
				}
		}
		m_cSeconds = 0;
		return TRUE;
	}
	return FALSE;
}


CString CRealtimeItem::Eighths(void)
{
	CString strReturn;
	char buf[20];
	sprintf(buf, "%d", m_nLastValue >> 3);
	strReturn = CString(buf);
	if (m_nLastValue & 0x0007 != 0)
	{
		sprintf(buf, " %d/8", m_nLastValue & 7);
		strReturn += buf;
	}
	return strReturn;
}
