// This is a part of the Microsoft Foundation Classes C++ library.
// Copyright (C) 1993 Microsoft Corporation
// All rights reserved.
//
// This source code is only intended as a supplement to the
// Microsoft Foundation Classes Reference and Microsoft
// QuickHelp and/or WinHelp documentation provided with the library.
// See these sources for detailed information regarding the
// Microsoft Foundation Classes product.


#include "stdafx.h"

#ifdef AFX_OLE_SEG
#pragma code_seg(AFX_OLE_SEG)
#endif

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

#define new DEBUG_NEW

/////////////////////////////////////////////////////////////////////////////
// Debug helpers

#ifdef _DEBUG
// Helper for converting IID into useful string.  Only for debugging.
LPCSTR AFXAPI AfxGetIIDString(REFIID iid)
{
	struct IID_ENTRY
	{
		const IID FAR* piid;
		char szName[32];
	};
	#define MAKE_IID_ENTRY(name) { &name, #name },
	static IID_ENTRY BASED_CODE iidNameTable[] =
	{
		MAKE_IID_ENTRY(IID_IAdviseSink)
		MAKE_IID_ENTRY(IID_IAdviseSink2)
		MAKE_IID_ENTRY(IID_IBindCtx)
		MAKE_IID_ENTRY(IID_IClassFactory)
		MAKE_IID_ENTRY(IID_IDataAdviseHolder)
		MAKE_IID_ENTRY(IID_IDataObject)
		MAKE_IID_ENTRY(IID_IDebug)
		MAKE_IID_ENTRY(IID_IDebugStream)
		MAKE_IID_ENTRY(IID_IDfReserved1)
		MAKE_IID_ENTRY(IID_IDfReserved2)
		MAKE_IID_ENTRY(IID_IDfReserved3)
		MAKE_IID_ENTRY(IID_IDispatch)
		MAKE_IID_ENTRY(IID_IDropSource)
		MAKE_IID_ENTRY(IID_IDropTarget)
		MAKE_IID_ENTRY(IID_IEnumCallback)
		MAKE_IID_ENTRY(IID_IEnumFORMATETC)
		MAKE_IID_ENTRY(IID_IEnumGeneric)
		MAKE_IID_ENTRY(IID_IEnumHolder)
		MAKE_IID_ENTRY(IID_IEnumMoniker)
		MAKE_IID_ENTRY(IID_IEnumOLEVERB)
		MAKE_IID_ENTRY(IID_IEnumSTATDATA)
		MAKE_IID_ENTRY(IID_IEnumSTATSTG)
		MAKE_IID_ENTRY(IID_IEnumString)
		MAKE_IID_ENTRY(IID_IEnumUnknown)
		MAKE_IID_ENTRY(IID_IEnumVARIANT)
		MAKE_IID_ENTRY(IID_IExternalConnection)
		MAKE_IID_ENTRY(IID_IInternalMoniker)
		MAKE_IID_ENTRY(IID_ILockBytes)
		MAKE_IID_ENTRY(IID_IMalloc)
		MAKE_IID_ENTRY(IID_IMarshal)
		MAKE_IID_ENTRY(IID_IMessageFilter)
		MAKE_IID_ENTRY(IID_IMoniker)
		MAKE_IID_ENTRY(IID_IOleAdviseHolder)
		MAKE_IID_ENTRY(IID_IOleCache)
		MAKE_IID_ENTRY(IID_IOleCache2)
		MAKE_IID_ENTRY(IID_IOleCacheControl)
		MAKE_IID_ENTRY(IID_IOleClientSite)
		MAKE_IID_ENTRY(IID_IOleContainer)
		MAKE_IID_ENTRY(IID_IOleInPlaceActiveObject)
		MAKE_IID_ENTRY(IID_IOleInPlaceFrame)
		MAKE_IID_ENTRY(IID_IOleInPlaceObject)
		MAKE_IID_ENTRY(IID_IOleInPlaceSite)
		MAKE_IID_ENTRY(IID_IOleInPlaceUIWindow)
		MAKE_IID_ENTRY(IID_IOleItemContainer)
		MAKE_IID_ENTRY(IID_IOleLink)
		MAKE_IID_ENTRY(IID_IOleManager)
		MAKE_IID_ENTRY(IID_IOleObject)
		MAKE_IID_ENTRY(IID_IOlePresObj)
		MAKE_IID_ENTRY(IID_IOleWindow)
		MAKE_IID_ENTRY(IID_IPSFactory)
		MAKE_IID_ENTRY(IID_IParseDisplayName)
		MAKE_IID_ENTRY(IID_IPersist)
		MAKE_IID_ENTRY(IID_IPersistFile)
		MAKE_IID_ENTRY(IID_IPersistStorage)
		MAKE_IID_ENTRY(IID_IPersistStream)
		MAKE_IID_ENTRY(IID_IProxyManager)
		MAKE_IID_ENTRY(IID_IRootStorage)
		MAKE_IID_ENTRY(IID_IRpcChannel)
		MAKE_IID_ENTRY(IID_IRpcProxy)
		MAKE_IID_ENTRY(IID_IRpcStub)
		MAKE_IID_ENTRY(IID_IRunnableObject)
		MAKE_IID_ENTRY(IID_IRunningObjectTable)
		MAKE_IID_ENTRY(IID_IStdMarshalInfo)
		MAKE_IID_ENTRY(IID_IStorage)
		MAKE_IID_ENTRY(IID_IStream)
		MAKE_IID_ENTRY(IID_IStubManager)
		MAKE_IID_ENTRY(IID_IUnknown)
		MAKE_IID_ENTRY(IID_IViewObject)
		MAKE_IID_ENTRY(IID_IViewObject2)
		MAKE_IID_ENTRY(IID_NULL)
	};
	#undef MAKE_IID_ENTRY

	// look for it in the table
	for (int i = 0; i < sizeof(iidNameTable)/sizeof(iidNameTable[0]); i++)
	{
		if (iid == *iidNameTable[i].piid)
			return iidNameTable[i].szName;
	}

	// if we get here, it is some IID_ we haven't heard of...
	static char BASED_CODE szUnknownIID[] = "*Unfamiliar IID*";
	return szUnknownIID;
}
#endif

/////////////////////////////////////////////////////////////////////////////
// Component object model helpers

LPUNKNOWN AFXAPI _AfxQueryInterface(LPUNKNOWN lpUnknown, REFIID iid)
{
	ASSERT(lpUnknown != NULL);

	LPUNKNOWN lpResult = NULL;  // always start with NULL
	if (lpUnknown->QueryInterface(iid, (LPLP)&lpResult) != NOERROR)
		return NULL;    // just to make sure

	return lpResult;
}

DWORD AFXAPI _AfxRelease(LPUNKNOWN* plpUnknown)
{
	ASSERT(plpUnknown != NULL);
	if (*plpUnknown != NULL)
	{
		DWORD dwRef = (*plpUnknown)->Release();
		*plpUnknown = NULL;
		return dwRef;
	}
	return 0;
}

#define GetInterfacePtr(pTarget, pEntry) \
	(LPUNKNOWN)((char*)pTarget + pEntry->nOffset)

#define GetAggregatePtr(pTarget, pEntry) \
	*(LPUNKNOWN*)((char*)this + pEntry->nOffset)

/////////////////////////////////////////////////////////////////////////////
// CCmdTarget interface map implementation

// support for aggregation
class FAR CInnerUnknown : public IUnknown
{
public:
	STDMETHOD_(ULONG, AddRef)();
	STDMETHOD_(ULONG, Release)();
	STDMETHOD(QueryInterface)(REFIID iid, LPVOID far* ppvObj);
};

// calling this function enables an object to be aggregatable
void CCmdTarget::EnableAggregation()
{
	// construct an CInnerUnknown just to get to the vtable
	CInnerUnknown innerUnknown;

	// copy the vtable & make sure initialized
	ASSERT(sizeof(m_xInnerUnknown) == sizeof(CInnerUnknown));
	m_xInnerUnknown = *(DWORD FAR*)&innerUnknown;
}

DWORD CCmdTarget::ExternalAddRef()
{
	// delegate to controlling unknown if aggregated
	if (m_pOuterUnknown != NULL)
		return m_pOuterUnknown->AddRef();

	return InternalAddRef();
}

DWORD CCmdTarget::InternalRelease()
{
	ASSERT(GetInterfaceMap() != NULL);

	if (m_dwRef == 0)
		return 0;

	if (--m_dwRef == 0)
	{
		OnFinalRelease();
		return 0;
	}
	return m_dwRef;
}

DWORD CCmdTarget::ExternalRelease()
{
	// delegate to controlling unknown if aggregated
	if (m_pOuterUnknown != NULL)
		return m_pOuterUnknown->Release();

	return InternalRelease();
}

// special QueryInterface used in implementation (does not AddRef)
LPUNKNOWN CCmdTarget::GetInterface(const void FAR* iid)
{
	// allow general hook first chance
	LPUNKNOWN lpUnk;
	if ((lpUnk = GetInterfaceHook(iid)) != NULL)
		return lpUnk;

	AFX_INTERFACEMAP FAR* pMap = GetInterfaceMap();
	ASSERT(pMap != NULL);

	// IUnknown is a special case since nobody really implements *only* it!
	BOOL bUnknown = (*(IID FAR*)iid == IID_IUnknown);

	// otherwise, walk the Interface map to find the IID
	do
	{
		AFX_INTERFACEMAP_ENTRY FAR* pEntry = pMap->pEntry;
		ASSERT(pEntry != NULL);
		while (pEntry->piid != NULL)
		{
			if (bUnknown || *(IID FAR*)pEntry->piid == *(IID FAR*)iid)
			{
				LPUNKNOWN lpUnk = GetInterfacePtr(this, pEntry);
				// check vtable pointer (can be NULL)
				if (*(DWORD FAR*)lpUnk != 0)
					return lpUnk;
			}

			// entry did not match -- keep looking
			++pEntry;
		}
	} while ((pMap = pMap->pMapBase) != NULL);

	// interface ID not found, fail the call
	return NULL;
}

LPUNKNOWN CCmdTarget::QueryAggregates(const void FAR* iid)
{
	AFX_INTERFACEMAP FAR* pMap = GetInterfaceMap();
	ASSERT(pMap != NULL);

	// walk the Interface map to call aggregates
	do
	{
		AFX_INTERFACEMAP_ENTRY FAR* pEntry = pMap->pEntry;
		// skip non-aggregate entries
		ASSERT(pEntry != NULL);
		while (pEntry->piid != NULL)
			++pEntry;

		// call QueryInterface for each aggregate entry
		while (pEntry->nOffset != (size_t)-1)
		{
			LPUNKNOWN lpQuery = GetAggregatePtr(this, pEntry);
			// it is ok to have aggregate but not created yet
			if (lpQuery != NULL)
			{
				LPUNKNOWN lpUnk = _AfxQueryInterface(lpQuery, *(IID FAR*)iid);
				if (lpUnk != NULL)
					return lpUnk;
			}

			// entry did not match -- keep looking
			++pEntry;
		}
	} while ((pMap = pMap->pMapBase) != NULL);

	// interface ID not found, fail the call
	return NULL;
}

// real implementation of QueryInterface
DWORD CCmdTarget::InternalQueryInterface(const void FAR* iid,
	LPVOID FAR* ppvObj)
{
	// try local interfaces
	if ((*ppvObj = GetInterface(iid)) != NULL)
	{
		// interface was found -- add a reference
		ExternalAddRef();
		return NOERROR;
	}

	// try aggregates
	if ((*ppvObj = QueryAggregates(iid)) != NULL)
		return NOERROR;

	// interface ID not found, fail the call
	return (DWORD)ResultFromScode(E_NOINTERFACE);
}

// QueryInterface that is exported to normal clients
DWORD CCmdTarget::ExternalQueryInterface(const void FAR* iid,
	LPVOID FAR* ppvObj)
{
	// delegate to controlling unknown if aggregated
	if (m_pOuterUnknown != NULL)
	{
		return (DWORD)m_pOuterUnknown->QueryInterface(
			*(IID FAR*)iid, ppvObj);
	}
	return InternalQueryInterface(iid, ppvObj);
}

/////////////////////////////////////////////////////////////////////////////
// Inner IUnknown implementation (for aggregation)

STDMETHODIMP_(ULONG) CInnerUnknown::AddRef()
{
	METHOD_PROLOGUE(CCmdTarget, InnerUnknown)
	return (ULONG)pThis->InternalAddRef();
}

STDMETHODIMP_(ULONG) CInnerUnknown::Release()
{
	METHOD_PROLOGUE(CCmdTarget, InnerUnknown)
	return (ULONG)pThis->InternalRelease();
}

STDMETHODIMP CInnerUnknown::QueryInterface(REFIID iid, LPVOID far* ppvObj)
{
	METHOD_PROLOGUE(CCmdTarget, InnerUnknown)

	if (iid == IID_IUnknown)
	{
		// QueryInterface on inner IUnknown for IID_IUnknown must
		//  return inner IUnknown.
		pThis->InternalAddRef();
		*ppvObj = this;
		return NOERROR;
	}
	return (HRESULT)pThis->InternalQueryInterface(&iid, ppvObj);
}

/////////////////////////////////////////////////////////////////////////////
// other helper functions

// ExternalDisconnect is used to remove RPC connections in destructors.  This
//  insures that no RPC calls will go to the object after it has been
//  deleted.
void CCmdTarget::ExternalDisconnect()
{
	if (m_dwRef == 0)   // already in disconnected state?
		return;

	// get IUnknown pointer for the object
	LPUNKNOWN lpUnknown = (LPUNKNOWN)GetInterface(&IID_IUnknown);
	ASSERT(lpUnknown != NULL);

	// disconnect the object
	++m_dwRef;  // protect object from destruction
	CoDisconnectObject(lpUnknown, 0);
	m_dwRef = 0;    // now in disconnected state
}

// GetControllingUnknown is used when creating aggregate objects,
//  usually from OnCreateAggregates.  The outer, or controlling, unknown
//  is one of the parameters to CoCreateInstance and other OLE creation
//  functions which support aggregation.
LPUNKNOWN CCmdTarget::GetControllingUnknown()
{
	if (m_pOuterUnknown != NULL)
		return m_pOuterUnknown; // aggregate of m_pOuterUnknown

	LPUNKNOWN lpUnknown = (LPUNKNOWN)GetInterface(&IID_IUnknown);
	return lpUnknown;   // return our own IUnknown implementation
}

/////////////////////////////////////////////////////////////////////////////
// Inline function declarations expanded out-of-line

#ifndef _AFX_ENABLE_INLINES

// expand inlines for OLE general APIs
static char BASED_CODE _szAfxComInl[] = "afxcom.inl";
#undef THIS_FILE
#define THIS_FILE _szAfxComInl
#define _AFXCOM_INLINE
#define _AFXDISP_INLINE
#include "afxole.inl"

#endif //!_AFX_ENABLE_INLINES

/////////////////////////////////////////////////////////////////////////////
