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

#ifdef AFX_OLE3_SEG
#pragma code_seg(AFX_OLE3_SEG)
#endif

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

#define new DEBUG_NEW

/////////////////////////////////////////////////////////////////////////////
// COleDataSource implementation

struct AFX_DATACACHE_ENTRY
{
	FORMATETC m_formatEtc;
	STGMEDIUM m_stgMedium;
	DATADIR m_dataDir;
};

static COleDataSource* NEAR pClipboardSource;
	// last data source put on clipboard

/////////////////////////////////////////////////////////////////////////////
// COleDataSource construction & destruction

COleDataSource::COleDataSource()
{
	m_pDataCache = NULL;
	m_nMaxSize = 0;
	m_nSize = 0;
	m_nGrowBy = 10;
}

COleDataSource::~COleDataSource()
{
	// clear clipboard source if this object was on the clipboard
	if (this == pClipboardSource)
		pClipboardSource = NULL;

	// free the clipboard data cache
	Empty();
}

void COleDataSource::Empty()
{
	if (m_pDataCache != NULL)
	{
		ASSERT(m_nMaxSize != 0);
		ASSERT(m_nSize != 0);

		// release all of the STGMEDIUMs and FORMATETCs
		for (UINT nIndex = 0; nIndex < m_nSize; nIndex++)
		{
			::OleStdDeleteTargetDevice(m_pDataCache[nIndex].m_formatEtc.ptd);
			::ReleaseStgMedium(&m_pDataCache[nIndex].m_stgMedium);
		}

		// delete the cache
		delete m_pDataCache;
		m_pDataCache = NULL;
		m_nMaxSize = 0;
		m_nSize = 0;
	}
	ASSERT(m_pDataCache == NULL);
	ASSERT(m_nMaxSize == 0);
	ASSERT(m_nSize == 0);
}

/////////////////////////////////////////////////////////////////////////////
// COleDataSource clipboard API wrappers

void COleDataSource::SetClipboard()
{
	ASSERT_VALID(this);

	// attempt OLE set clipboard operation
	LPDATAOBJECT lpDataObject = (LPDATAOBJECT)GetInterface(&IID_IDataObject);
	HRESULT hr = ::OleSetClipboard(lpDataObject);
	if (hr != NOERROR)
		AfxThrowOleException(hr);

	// success - set as current clipboard source
	pClipboardSource = this;
	ASSERT(::OleIsCurrentClipboard(lpDataObject) == NOERROR);
	InternalRelease();
}

void PASCAL COleDataSource::FlushClipboard()
{
	if (GetClipboardOwner() != NULL)
	{
		// active clipboard source and it is on the clipboard - flush it
		::OleFlushClipboard();

		// shouldn't be clipboard owner any more...
		ASSERT(GetClipboardOwner() == NULL);
	}
}

COleDataSource* PASCAL COleDataSource::GetClipboardOwner()
{
	if (pClipboardSource == NULL)
		return NULL;    // can't own the clipboard if pClipboardSource isn't set

	ASSERT_VALID(pClipboardSource);
	LPDATAOBJECT lpDataObject = (LPDATAOBJECT)
		pClipboardSource->GetInterface(&IID_IDataObject);
	if (::OleIsCurrentClipboard(lpDataObject) != NOERROR)
	{
		pClipboardSource = NULL;
		return NULL;    // don't own the clipboard anymore
	}

	// return current clipboard source
	return pClipboardSource;
}

/////////////////////////////////////////////////////////////////////////////
// COleDataSource cache allocation

AFX_DATACACHE_ENTRY* COleDataSource::AllocSpace()
{
	// allocate space for item at m_nSize (at least room for 1 item)
	if (m_pDataCache == NULL || m_nSize == m_nMaxSize)
	{
		ASSERT(m_nGrowBy != 0);
		AFX_DATACACHE_ENTRY* pCache = new AFX_DATACACHE_ENTRY[m_nMaxSize+m_nGrowBy];
		m_nMaxSize += m_nGrowBy;
		if (m_pDataCache != NULL)
		{
			memcpy(pCache, m_pDataCache, m_nSize * sizeof(AFX_DATACACHE_ENTRY));
			delete[] m_pDataCache;
		}
		m_pDataCache = pCache;
	}
	ASSERT(m_pDataCache != NULL);
	ASSERT(m_nMaxSize != 0);

	// return next slot in cache table
	return &m_pDataCache[m_nSize];
}

/////////////////////////////////////////////////////////////////////////////
// COleDataSource operations

// for HGLOBAL based cached render
void COleDataSource::CacheGlobalData(CLIPFORMAT cfFormat, HGLOBAL hGlobal,
	LPFORMATETC lpFormatEtc)
{
	ASSERT(hGlobal != NULL);
	ASSERT(lpFormatEtc == NULL ||
		AfxIsValidAddress(lpFormatEtc, sizeof(FORMATETC), FALSE));

	// fill in FORMATETC struct
	FORMATETC formatEtc;
	lpFormatEtc = _AfxFillFormatEtc(lpFormatEtc, cfFormat, &formatEtc);
	lpFormatEtc->tymed = TYMED_HGLOBAL;

	// add it to the cache
	AFX_DATACACHE_ENTRY* pEntry = AllocSpace();
	pEntry->m_formatEtc = *lpFormatEtc;
	pEntry->m_stgMedium.tymed = TYMED_HGLOBAL;
	pEntry->m_stgMedium.hGlobal = hGlobal;
	pEntry->m_stgMedium.pUnkForRelease = NULL;
	pEntry->m_dataDir = DATADIR_GET;
	m_nSize++;
}

// for raw LPSTGMEDIUM cached render
void COleDataSource::CacheData(CLIPFORMAT cfFormat, LPSTGMEDIUM lpStgMedium,
	LPFORMATETC lpFormatEtc)
{
	ASSERT(lpStgMedium == NULL || lpStgMedium->tymed != TYMED_NULL);
	ASSERT(lpStgMedium == NULL ||
		AfxIsValidAddress(lpStgMedium, sizeof(STGMEDIUM), FALSE));
	ASSERT(lpFormatEtc == NULL ||
		AfxIsValidAddress(lpFormatEtc, sizeof(FORMATETC), FALSE));

	// fill in FORMATETC struct
	FORMATETC formatEtc;
	lpFormatEtc = _AfxFillFormatEtc(lpFormatEtc, cfFormat, &formatEtc);

	// add it to the cache
	AFX_DATACACHE_ENTRY* pEntry = AllocSpace();
	lpFormatEtc->tymed = lpStgMedium->tymed;
	pEntry->m_stgMedium = *lpStgMedium;
	pEntry->m_formatEtc = *lpFormatEtc;
	pEntry->m_dataDir = DATADIR_GET;
	m_nSize++;
}

// for CFile* based delayed render
void COleDataSource::DelayRenderFileData(CLIPFORMAT cfFormat,
	LPFORMATETC lpFormatEtc)
{
	ASSERT(lpFormatEtc == NULL ||
		AfxIsValidAddress(lpFormatEtc, sizeof(FORMATETC), FALSE));

	// fill in FORMATETC struct
	FORMATETC formatEtc;
	lpFormatEtc = _AfxFillFormatEtc(lpFormatEtc, cfFormat, &formatEtc);
	lpFormatEtc->tymed |= TYMED_ISTREAM|TYMED_HGLOBAL;

	// add it to the cache
	AFX_DATACACHE_ENTRY* pEntry = AllocSpace();
	pEntry->m_formatEtc = *lpFormatEtc;
	pEntry->m_stgMedium.tymed = TYMED_NULL;
	pEntry->m_stgMedium.hGlobal = NULL;
	pEntry->m_stgMedium.pUnkForRelease = NULL;
	pEntry->m_dataDir = DATADIR_GET;
	m_nSize++;
}

// for LPSTGMEDIUM or HGLOBAL based delayed render
void COleDataSource::DelayRenderData(CLIPFORMAT cfFormat, LPFORMATETC lpFormatEtc)
{
	ASSERT(lpFormatEtc == NULL ||
		AfxIsValidAddress(lpFormatEtc, sizeof(FORMATETC), FALSE));

	// fill in FORMATETC struct
	FORMATETC formatEtc;
	if (lpFormatEtc == NULL)
	{
		lpFormatEtc = _AfxFillFormatEtc(lpFormatEtc, cfFormat, &formatEtc);
		lpFormatEtc->tymed = TYMED_HGLOBAL;
	}
	// insure that cfFormat member is set
	if (cfFormat != 0)
		lpFormatEtc->cfFormat = cfFormat;

	// add it to the cache
	AFX_DATACACHE_ENTRY* pEntry = AllocSpace();
	memset(&pEntry->m_stgMedium, 0, sizeof pEntry->m_stgMedium);
	pEntry->m_formatEtc = *lpFormatEtc;
	pEntry->m_dataDir = DATADIR_GET;
	m_nSize++;
}

// DelaySetData -- used to allow SetData on given LPFORMATETC
void COleDataSource::DelaySetData(CLIPFORMAT cfFormat, LPFORMATETC lpFormatEtc)
{
	ASSERT(lpFormatEtc == NULL ||
		AfxIsValidAddress(lpFormatEtc, sizeof(FORMATETC), FALSE));

	// fill in FORMATETC struct
	FORMATETC formatEtc;
	lpFormatEtc = _AfxFillFormatEtc(lpFormatEtc, cfFormat, &formatEtc);

	// add it to the cache
	AFX_DATACACHE_ENTRY* pEntry = AllocSpace();
	pEntry->m_formatEtc = *lpFormatEtc;
	pEntry->m_stgMedium.tymed = TYMED_NULL;
	pEntry->m_stgMedium.hGlobal = NULL;
	pEntry->m_stgMedium.pUnkForRelease = NULL;
	pEntry->m_dataDir = DATADIR_SET;
	m_nSize++;
}

/////////////////////////////////////////////////////////////////////////////
// COleDataSource cache implementation

AFX_DATACACHE_ENTRY* COleDataSource::Lookup(
	LPFORMATETC lpFormatEtc, DATADIR dataDir) const
{
	// look for suitable match to lpFormatEtc in cache
	for (UINT nIndex = 0; nIndex < m_nSize; nIndex++)
	{
		// get entry from cache at nIndex
		AFX_DATACACHE_ENTRY* pCache = &m_pDataCache[nIndex];
		FORMATETC *pCacheFormat = &pCache->m_formatEtc;

		// check for match
		if (pCacheFormat->cfFormat == lpFormatEtc->cfFormat &&
			(pCacheFormat->tymed & lpFormatEtc->tymed) != 0 &&
			pCacheFormat->lindex == lpFormatEtc->lindex &&
			pCacheFormat->dwAspect == lpFormatEtc->dwAspect &&
			pCache->m_dataDir == dataDir)
		{
			// return that cache entry
			return pCache;
		}
	}

	return NULL;    // not found
}

/////////////////////////////////////////////////////////////////////////////
// COleDataSource overidable default implementation

BOOL COleDataSource::OnRenderGlobalData(LPFORMATETC lpFormatEtc,
	HGLOBAL* phGlobal)
{
	return FALSE;   // default does nothing
}

BOOL COleDataSource::OnRenderFileData(LPFORMATETC lpFormatEtc,
	CFile* pFile)
{
	return FALSE;   // default does nothing
}

BOOL COleDataSource::OnRenderData(LPFORMATETC lpFormatEtc,
	LPSTGMEDIUM lpStgMedium)
{
	// try TYMED_HGLOBAL as prefered format
	if (lpFormatEtc->tymed & TYMED_HGLOBAL)
	{
		// try HGLOBAL delay render hook
		HGLOBAL hGlobal = lpStgMedium->hGlobal;
		if (OnRenderGlobalData(lpFormatEtc, &hGlobal))
		{
			ASSERT(lpStgMedium->tymed != TYMED_HGLOBAL ||
				(lpStgMedium->hGlobal == hGlobal));
			ASSERT(hGlobal != NULL);
			lpStgMedium->tymed = TYMED_HGLOBAL;
			lpStgMedium->hGlobal = hGlobal;
			return TRUE;
		}

		// try CFile* based delay render hook
		CSharedFile file;
		if (lpStgMedium->tymed == TYMED_HGLOBAL)
		{
			ASSERT(lpStgMedium->hGlobal != NULL);
			file.SetHandle(lpStgMedium->hGlobal, FALSE);
		}
		if (OnRenderFileData(lpFormatEtc, &file))
		{
			lpStgMedium->tymed = TYMED_HGLOBAL;
			lpStgMedium->hGlobal = file.Detach();
			ASSERT(lpStgMedium->hGlobal != NULL);
			return TRUE;
		}
		if (lpStgMedium->tymed == TYMED_HGLOBAL)
			file.Detach();
	}

	// try TYMED_ISTREAM format
	if (lpFormatEtc->tymed & TYMED_ISTREAM)
	{
		COleStreamFile file;
		if (lpStgMedium->tymed == TYMED_ISTREAM)
		{
			ASSERT(lpStgMedium->pstm != NULL);
			file.Attach(lpStgMedium->pstm);
		}
		else
		{
			if (!file.CreateMemoryStream())
				AfxThrowMemoryException();
		}
		// get data into the stream
		if (OnRenderFileData(lpFormatEtc, &file))
		{
			lpStgMedium->tymed = TYMED_ISTREAM;
			lpStgMedium->pstm = file.Detach();
			return TRUE;
		}
		if (lpStgMedium->tymed == TYMED_ISTREAM)
			file.Detach();
	}

	return FALSE;   // default does nothing
}

#ifdef _AFXCTL
BOOL COleDataSource::OnSetData(LPFORMATETC lpFormatEtc, LPSTGMEDIUM lpStgMedium,
	BOOL fRelease)
#else
BOOL COleDataSource::OnSetData(LPFORMATETC lpFormatEtc, LPSTGMEDIUM lpStgMedium)
#endif
{
	return FALSE;   // default does nothing
}

/////////////////////////////////////////////////////////////////////////////
// CEnumFormatEtc - enumerator for array for FORMATETC structures

class CEnumFormatEtc : public CEnumArray
{
// Constructors
public:
	CEnumFormatEtc();

// Operations
	void AddFormat(const FORMATETC FAR* lpFormatEtc);

// Implementation
public:
	virtual ~CEnumFormatEtc();

protected:
	virtual BOOL OnNext(void FAR* pv);

	UINT m_nMaxSize;    // number of items allocated (>= m_nSize)
};

CEnumFormatEtc::CEnumFormatEtc()
	: CEnumArray(sizeof(FORMATETC), IID_IEnumFORMATETC, NULL, 0, TRUE)
{
	m_nMaxSize = 0;
}

CEnumFormatEtc::~CEnumFormatEtc()
{
	if (m_pClonedFrom == NULL)
	{
		// release all of the pointers to DVTARGETDEVICE
		LPFORMATETC lpFormatEtc = (LPFORMATETC)_AfxGetPtrFromFarPtr(m_pvEnum);
		for (UINT nIndex = 0; nIndex < m_nSize; nIndex++)
			::OleStdDeleteTargetDevice(lpFormatEtc[nIndex].ptd);
	}
	// destructor will free the actual array (if it was not a clone)
}

BOOL CEnumFormatEtc::OnNext(void FAR* pv)
{
	if (!CEnumArray::OnNext(pv))
		return FALSE;

	// any outgoing formatEtc may require the DVTARGETDEVICE to
	//  be copied (the caller has responsibility to free it)
	LPFORMATETC lpFormatEtc = (LPFORMATETC)pv;
	if (lpFormatEtc->ptd != NULL)
	{
		lpFormatEtc->ptd = ::OleStdCopyTargetDevice(lpFormatEtc->ptd);
		if (lpFormatEtc->ptd == NULL)
			AfxThrowMemoryException();
	}
	// otherwise, copying worked...
	return TRUE;
}

void CEnumFormatEtc::AddFormat(const FORMATETC FAR* lpFormatEtc)
{
	ASSERT(m_nSize <= m_nMaxSize);

	if (m_nSize == m_nMaxSize)
	{
		// not enough space for new item -- allocate more
		FORMATETC* pListNew = new FORMATETC[m_nSize+10];
		m_nMaxSize += 10;
		_fmemcpy(pListNew, m_pvEnum, m_nSize*sizeof(FORMATETC));
		delete _AfxGetPtrFromFarPtr(m_pvEnum);
		m_pvEnum = (char FAR*)pListNew;
	}

	// add this item to the list
	ASSERT(m_nSize < m_nMaxSize);
	FORMATETC* pFormat = &((FORMATETC*)
		_AfxGetPtrFromFarPtr(m_pvEnum))[m_nSize];
	pFormat->cfFormat = lpFormatEtc->cfFormat;
	pFormat->ptd = lpFormatEtc->ptd;
		// Note: ownership of lpFormatEtc->ptd is transfered with this call.
	pFormat->dwAspect = lpFormatEtc->dwAspect;
	pFormat->lindex = lpFormatEtc->lindex;
	pFormat->tymed = lpFormatEtc->tymed;
	++m_nSize;
}

/////////////////////////////////////////////////////////////////////////////
// COleDataSource::XDataObject

BEGIN_INTERFACE_MAP(COleDataSource, CCmdTarget)
	INTERFACE_PART(COleDataSource, IID_IDataObject, DataObject)
END_INTERFACE_MAP()

STDMETHODIMP_(ULONG) COleDataSource::XDataObject::AddRef()
{
	METHOD_PROLOGUE(COleDataSource, DataObject)
	return (ULONG)pThis->ExternalAddRef();
}

STDMETHODIMP_(ULONG) COleDataSource::XDataObject::Release()
{
	METHOD_PROLOGUE(COleDataSource, DataObject)
	return (ULONG)pThis->ExternalRelease();
}

STDMETHODIMP COleDataSource::XDataObject::QueryInterface(
	REFIID iid, LPVOID far* ppvObj)
{
	METHOD_PROLOGUE(COleDataSource, DataObject)
	return (HRESULT)pThis->ExternalQueryInterface(&iid, ppvObj);
}

STDMETHODIMP COleDataSource::XDataObject::GetData(
	LPFORMATETC lpFormatEtc, LPSTGMEDIUM lpStgMedium)
{
	METHOD_PROLOGUE(COleDataSource, DataObject)
	ASSERT_VALID(pThis);

	// try to find match in the cache
	AFX_DATACACHE_ENTRY* pCache = pThis->Lookup(lpFormatEtc, DATADIR_GET);
	if (pCache == NULL)
		return ResultFromScode(DATA_E_FORMATETC);

	// use cache if entry is not delay render
	_fmemset(lpStgMedium, 0, sizeof(STGMEDIUM));
	if (pCache->m_stgMedium.tymed != TYMED_NULL)
	{
		// Copy the medium in the cache into the lpStgMedium provided
		//  by the caller.
		if (!_AfxCopyStgMedium(lpFormatEtc->cfFormat, lpStgMedium,
		  &pCache->m_stgMedium))
			return ResultFromScode(DATA_E_FORMATETC);

		// format was supported for copying
		return NOERROR;
	}

	SCODE sc = DATA_E_FORMATETC;
	TRY
	{
		// try LPSTGMEDIUM based delay render
		if (pThis->OnRenderData(lpFormatEtc, lpStgMedium))
			sc = S_OK;
	}
	CATCH_ALL(e)
	{
		sc = COleException::Process(e);
	}
	END_CATCH_ALL

	return ResultFromScode(sc);
}

STDMETHODIMP COleDataSource::XDataObject::GetDataHere(
	LPFORMATETC lpFormatEtc, LPSTGMEDIUM lpStgMedium)
{
	METHOD_PROLOGUE(COleDataSource, DataObject)
	ASSERT_VALID(pThis);

	// these two must be the same
	ASSERT(lpFormatEtc->tymed == lpStgMedium->tymed);
	lpFormatEtc->tymed = lpStgMedium->tymed;    // but just in case...

	// try to find match in the cache
	AFX_DATACACHE_ENTRY* pCache = pThis->Lookup(lpFormatEtc, DATADIR_GET);
	if (pCache == NULL)
		return ResultFromScode(DATA_E_FORMATETC);

	// handle cached medium and copy
	if (pCache->m_stgMedium.tymed != TYMED_NULL)
	{
		// found a cached format -- copy it to dest medium
		ASSERT(pCache->m_stgMedium.tymed == lpStgMedium->tymed);
		if (!_AfxCopyStgMedium(lpFormatEtc->cfFormat, lpStgMedium,
		  &pCache->m_stgMedium))
			return ResultFromScode(DATA_E_FORMATETC);

		// else - copy successful
		return NOERROR;
	}

	SCODE sc = DATA_E_FORMATETC;
	TRY
	{
		// try LPSTGMEDIUM based delay render
		if (pThis->OnRenderData(lpFormatEtc, lpStgMedium))
			sc = S_OK;
	}
	CATCH_ALL(e)
	{
		sc = COleException::Process(e);
	}
	END_CATCH_ALL

	return ResultFromScode(sc);
}

STDMETHODIMP COleDataSource::XDataObject::QueryGetData(LPFORMATETC lpFormatEtc)
{
	METHOD_PROLOGUE(COleDataSource, DataObject)
	ASSERT_VALID(pThis);

	// try to find match in the cache
	AFX_DATACACHE_ENTRY* pCache = pThis->Lookup(lpFormatEtc, DATADIR_GET);
	if (pCache == NULL)
		return ResultFromScode(DATA_E_FORMATETC);

	// it was found in the cache or can be rendered -- success
	return NOERROR;
}

STDMETHODIMP COleDataSource::XDataObject::GetCanonicalFormatEtc(
	LPFORMATETC lpFormatEtcIn, LPFORMATETC lpFormatEtcOut)
{
	// because we support the target-device (ptd) for server metafile format,
	//  all members of the FORMATETC are significant.

	return ResultFromScode(DATA_S_SAMEFORMATETC);
}

STDMETHODIMP COleDataSource::XDataObject::SetData(
	LPFORMATETC lpFormatEtc, LPSTGMEDIUM lpStgMedium, BOOL fRelease)
{
	METHOD_PROLOGUE(COleDataSource, DataObject)
	ASSERT_VALID(pThis);

	ASSERT(lpFormatEtc->tymed == lpStgMedium->tymed);

	// try to find match in the cache
	AFX_DATACACHE_ENTRY* pCache = pThis->Lookup(lpFormatEtc, DATADIR_SET);
	if (pCache == NULL)
		return ResultFromScode(DATA_E_FORMATETC);

	ASSERT(pCache->m_stgMedium.tymed == TYMED_NULL);

	SCODE sc = E_UNEXPECTED;
	TRY
	{
		// try LPSTGMEDIUM based SetData
#ifdef _AFXCTL
		if (pThis->OnSetData(lpFormatEtc, lpStgMedium, fRelease))
#else
		if (pThis->OnSetData(lpFormatEtc, lpStgMedium))
#endif
			sc = S_OK;
	}
	CATCH_ALL(e)
	{
		sc = COleException::Process(e);
	}
	END_CATCH_ALL

	return ResultFromScode(sc);
}

STDMETHODIMP COleDataSource::XDataObject::EnumFormatEtc(
	DWORD dwDirection, LPENUMFORMATETC FAR* ppenumFormatEtc)
{
	METHOD_PROLOGUE(COleDataSource, DataObject)
	ASSERT_VALID(pThis);

	*ppenumFormatEtc = NULL;

	CEnumFormatEtc* pFormatList = NULL;
	SCODE sc = E_OUTOFMEMORY;
	TRY
	{
		// generate a format list from the cache
		pFormatList = new CEnumFormatEtc;
		for (UINT nIndex = 0; nIndex < pThis->m_nSize; nIndex++)
		{
			AFX_DATACACHE_ENTRY* pCache = &pThis->m_pDataCache[nIndex];
			if ((DWORD)pCache->m_dataDir & dwDirection)
			{
				// entry should be enumerated -- add it to the list
				FORMATETC formatEtc;
				if (!::OleStdCopyFormatEtc(&formatEtc, &pCache->m_formatEtc))
					AfxThrowMemoryException();
				pFormatList->AddFormat(&formatEtc);
			}
		}
		// give it away to OLE (ref count is already 1)
		*ppenumFormatEtc = (LPENUMFORMATETC)&pFormatList->m_xEnumVOID;
		sc = S_OK;
	}
	END_TRY

	return ResultFromScode(sc);
}

STDMETHODIMP COleDataSource::XDataObject::DAdvise(
	FORMATETC FAR* pFormatetc, DWORD advf,
	LPADVISESINK pAdvSink, DWORD FAR* pdwConnection)
{
	*pdwConnection = 0;
	return ResultFromScode(OLE_E_ADVISENOTSUPPORTED);
}

STDMETHODIMP COleDataSource::XDataObject::DUnadvise(DWORD dwConnection)
{
	return ResultFromScode(OLE_E_ADVISENOTSUPPORTED);
}

STDMETHODIMP COleDataSource::XDataObject::EnumDAdvise(
	LPENUMSTATDATA FAR* ppenumAdvise)
{
	*ppenumAdvise = NULL;
	return ResultFromScode(OLE_E_ADVISENOTSUPPORTED);
}

/////////////////////////////////////////////////////////////////////////////
// Special clipboard window implementation to work-around Windows NT bug.

extern WNDPROC NEAR _afxPrevClipboardWndProc;

LRESULT CALLBACK AFX_EXPORT
_AfxClipboardWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	// don't pass WM_DESTROYCLIPBOARD to OLE unless this window owns the
	//  clipboard.  The OLE code does not handle this and this happens
	//  under Windows NT.
	if (message == WM_DESTROYCLIPBOARD && ::GetClipboardOwner() != hWnd)
		return 0;

	// otherwise, pass message along to OLE clipboard window
	ASSERT(_afxPrevClipboardWndProc != NULL);
	return ::CallWindowProc(_afxPrevClipboardWndProc,
		hWnd, message, wParam, lParam);
}

/////////////////////////////////////////////////////////////////////////////
// COleDataSource diagnostics

#ifdef _DEBUG
void COleDataSource::AssertValid() const
{
	CCmdTarget::AssertValid();
	ASSERT(m_nSize <= m_nMaxSize);
	ASSERT(m_nMaxSize != 0 || m_pDataCache == NULL);
}

void COleDataSource::Dump(CDumpContext& dc) const
{
	CCmdTarget::Dump(dc);
	AFX_DUMP1(dc, "\nm_nMaxSize = ", m_nMaxSize);
	AFX_DUMP1(dc, "\nm_nSize = ", m_nSize);
	AFX_DUMP1(dc, "m_pDataCache = ", m_pDataCache);
	for (UINT n = 0; n < m_nSize; n++)
	{
		AFX_DUMP1(dc, "\n\tentry [", n); AFX_DUMP0(dc, "]");
		AFX_DUMP1(dc, "\n\t m_formatEtc.cfFormat = ",
			m_pDataCache[n].m_formatEtc.cfFormat);
		AFX_DUMP1(dc, "\n\t m_formatEtc.pdt = ", m_pDataCache[n].m_formatEtc.ptd);
		AFX_DUMP1(dc, "\n\t m_formatEtc.dwAspect = ",
			m_pDataCache[n].m_formatEtc.dwAspect);
		AFX_DUMP1(dc, "\n\t m_formatEtc.lindex = ",
			m_pDataCache[n].m_formatEtc.lindex);
		AFX_DUMP1(dc, "\n\t m_formatEtc.tymed = ",
			m_pDataCache[n].m_formatEtc.tymed);
		AFX_DUMP1(dc, "\n\t m_stgMedium.tymed = ",
			m_pDataCache[n].m_stgMedium.tymed);
		AFX_DUMP1(dc, "\n\t m_dataDir = ", (UINT)m_pDataCache[n].m_dataDir);
	}
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
