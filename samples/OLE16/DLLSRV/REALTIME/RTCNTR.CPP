
#include "stdafx.h"
#include "utils.h"
#include "realtdoc.h"
#include "rtcntr.h"
#include "rtitem.h"
#include "realtime.h"
#include "..\prgidmon.h"

CRTContainer::CRTContainer( LPUNKNOWN pUnkOuter, CRealtimeDoc* pDoc )
	: CControllingUnknown(pUnkOuter), m_ItemContainer(this)
{
	m_pDoc = pDoc;
	cObjs = 0;
	m_dwRegister = 0;
}

CRTContainer::~CRTContainer(void)
{
	OutputDebugString("FOO\n");
	if (m_dwRegister) Unregister();
	PostMessage(theApp.m_pMainWnd->m_hWnd, WM_CLOSE, 0, 0L);
}

STDMETHODIMP_(ULONG) CRTContainer::Release()
{
	char chBuf[80];
	ULONG refs = CControllingUnknown::Release();
	wsprintf (chBuf, "CRTContainer::Release, m_refs = %d\n", refs);
	OutputDebugString(chBuf);
	return refs;
}

STDMETHODIMP CRTContainer::QueryInterface( REFIID riid, LPVOID FAR* ppvObj)
{
	*ppvObj = NULL;
	if (m_pUnkOuter) return m_pUnkOuter->QueryInterface( riid, ppvObj );
	if (riid == IID_IUnknown)
	{
		*ppvObj = (LPVOID) this;
	}
	else if (riid == IID_IOleItemContainer || riid == IID_IOleContainer || riid == IID_IParseDisplayName)
	{
		*ppvObj = (LPVOID)&m_ItemContainer;
	}
	if (*ppvObj == NULL) return ResultFromScode( E_NOINTERFACE );
	((LPUNKNOWN)(*ppvObj))->AddRef();
	return NOERROR;
}


CRTContainer::CItemContainer::CItemContainer
( CRTContainer FAR * pRTContainer )
{
	m_pItemContainer = pRTContainer;
}

STDMETHODIMP CRTContainer::CItemContainer::QueryInterface (THIS_ REFIID riid, LPVOID FAR* ppvObj)
{
	return m_pItemContainer->QueryInterface(riid, ppvObj);
}

STDMETHODIMP_(ULONG) CRTContainer::CItemContainer::AddRef (THIS)
{
	return m_pItemContainer->AddRef();
}

STDMETHODIMP_(ULONG) CRTContainer::CItemContainer::Release (THIS)
{
	return m_pItemContainer->Release();
}


		// *** IParseDisplayName method ***
STDMETHODIMP CRTContainer::CItemContainer::ParseDisplayName (THIS_ LPBC pbc, LPSTR lpszDisplayName,
			ULONG FAR* pchEaten, LPMONIKER FAR* ppmkOut)
{
	LPSTR psz = lpszDisplayName;
	char szDelim[2];
	HRESULT hresult;

	if (ppmkOut == NULL)
		return ResultFromScode(E_INVALIDARG);

	*ppmkOut = NULL;

	if (lpszDisplayName == NULL)
		return ResultFromScode(E_INVALIDARG);

	// check for leading delimeter
	if (*psz != '\\' && *psz != '/' && *psz != '!')
		return ResultFromScode(MK_E_SYNTAX);

	// we accept any string as our item name
	szDelim[0] = *psz;
	szDelim[1] = '\0';
	psz = AnsiNext(psz);
	hresult = CreateItemMoniker(szDelim, psz, ppmkOut );
	if (hresult == NOERROR && pchEaten)
		*pchEaten = lstrlen(lpszDisplayName);
	return hresult;
}

		// *** IOleContainer methods ***
STDMETHODIMP CRTContainer::CItemContainer::EnumObjects (THIS_ DWORD grfFlags, LPENUMUNKNOWN FAR* ppenumUnknown)
{
	*ppenumUnknown = NULL;
	return ResultFromScode(E_NOTIMPL);
}


STDMETHODIMP CRTContainer::CItemContainer::LockContainer (THIS_ BOOL fLock)
{
	return NOERROR;
}

		// *** IOleItemContainer methods ***
STDMETHODIMP CRTContainer::CItemContainer::GetObject (THIS_ LPSTR lpszItem, DWORD dwSpeedNeeded,
			LPBINDCTX pbc, REFIID riid, LPVOID FAR* ppvObject)
{
	CMapStringToPtr * pmapList = &(m_pItemContainer->m_pDoc->m_mapList);
	CRealtimeItem * pRealtimeItem;
	HRESULT hres;
	char * psz = new char[1+lstrlen(lpszItem)];
	_fstrcpy( psz, lpszItem );
	if (pmapList->Lookup(psz, (LPVOID &)pRealtimeItem))
		hres = pRealtimeItem->QueryInterface(riid, ppvObject);
	//  if there is no such item, create one.
	else
	{
		pmapList->SetAt(psz, pRealtimeItem = new CRealtimeItem(NULL, m_pItemContainer));
		if (pRealtimeItem != NULL)
		{
			hres = pRealtimeItem->QueryInterface(riid, ppvObject);
			pRealtimeItem->Release();
			m_pItemContainer->m_pDoc->UpdateAllViews( NULL );
		}
	}
	delete psz;
	return hres;
}

STDMETHODIMP CRTContainer::CItemContainer::GetObjectStorage (THIS_ LPSTR lpszItem, LPBINDCTX pbc,
			REFIID riid, LPVOID FAR* ppvStorage)
{
	return ResultFromScode(MK_E_NOSTORAGE);

}

STDMETHODIMP CRTContainer::CItemContainer::IsRunning (THIS_ LPSTR lpszItem)
{
	return NOERROR;
	//  objects are always running
}


//  nonvirtual methods

UINT CRTContainer::Lock(void)
{
	char chBuf[30];
	cObjs++;
	wsprintf(chBuf, "Lock count inc to %d\n", cObjs);
	OutputDebugString(chBuf);
	CoLockObjectExternal(this, TRUE, TRUE);
//  AddRef();
	return cObjs;
}


UINT CRTContainer::Unlock(void)
{
	char chBuf[30];
	UINT n = --cObjs;
	wsprintf(chBuf, "Lock count dec to %d\n", cObjs);
	OutputDebugString(chBuf);
	CoLockObjectExternal(this, FALSE, TRUE);
//  Release();
	return n;
}


BOOL CRTContainer::Exitable(void)
{
	return cObjs == 0;
}

void CRTContainer::Register()
{
	LPMONIKER pmkProgId;
	LPRUNNINGOBJECTTABLE prot;

	CreateProgIdMoniker("Realtime", &pmkProgId);
	GetRunningObjectTable(0,&prot);
	prot->Register(0, this, pmkProgId, &m_dwRegister);
	prot->Release();
}


void CRTContainer::Unregister()
{
	LPRUNNINGOBJECTTABLE prot;
		GetRunningObjectTable(0,&prot);
		prot->Revoke(m_dwRegister);
		prot->Release();
}
