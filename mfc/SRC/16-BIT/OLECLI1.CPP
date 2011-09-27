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

// IMPLEMENT_DYNAMIC for COleLinkingDoc here for better .OBJ granularity
IMPLEMENT_DYNAMIC(COleLinkingDoc, COleDocument)

/////////////////////////////////////////////////////////////////////////////
// COleClientItem - Container view of IOleObject and related interfaces

COleClientItem::COleClientItem(COleDocument* pContainerDoc)
{
	if (pContainerDoc != NULL)
		ASSERT_VALID(pContainerDoc);

	// initialize OLE client side view of IOleObject
	m_lpObject = NULL;
	m_dwConnection = 0;
	m_lpStorage = NULL;
	m_lpLockBytes = NULL;
	m_scLast = S_OK;
	m_pView = NULL;
	m_pInPlaceFrame = NULL;
	m_pInPlaceDoc = NULL;
	m_nItemState = emptyState;  // initially empty until OleLoad, OleCreate
	m_bMoniker = FALSE;
	m_nDrawAspect = DVASPECT_CONTENT;   // default draw aspect
	m_dwItemNumber = 0;
	m_bLinkUnavail = FALSE; // set to TRUE on failed DoVerb, or in links dialog
	m_nItemType = OT_UNKNOWN;       // type unknown so far
	m_hWndServer = NULL;
	m_bClosing = FALSE; // COleClientItem::Close in process
	m_bLocked = FALSE;  // need CoLockObjectExternal(..., FALSE, ...)

	// initialize compound file support
	m_lpNewStorage = NULL;
	m_bNeedCommit = FALSE;

	if (pContainerDoc != NULL)
		pContainerDoc->AddItem(this);

	ASSERT(m_pDocument == pContainerDoc);
	ASSERT_VALID(this);

	AfxOleLockApp();
}

COleClientItem::~COleClientItem()
{
	ASSERT_VALID(this);

	// release any references we may have to other objects
	Release();

	// only remove it from the associated document if it hasn't been detached
	//  from the document already!
	if (m_pDocument != NULL)
		m_pDocument->RemoveItem(this);

	// make sure all outside connections are disconnected
	ExternalDisconnect();
	AfxOleUnlockApp();
}

void COleClientItem::Delete(BOOL bAutoDelete)
{
	ASSERT_VALID(this);

	Release();      // first close it

	COleDocument* pDoc = GetDocument();
	if (pDoc != NULL && pDoc->m_bCompoundFile)
	{
		// cleanup docfile storage first
		COleDocument* pDoc = GetDocument();
		ASSERT_VALID(pDoc);

		if (pDoc->m_lpRootStg != NULL)
		{
			// get item name
			char szItemName[OLE_MAXITEMNAME];
			GetItemName(szItemName);

			// attempt to remove it from the storage, ignore errors
			pDoc->m_lpRootStg->DestroyElement(szItemName);
		}
	}

	if (bAutoDelete)
	{
		// remove item from document
		if (pDoc != NULL)
			pDoc->RemoveItem(this);

		InternalRelease();  // remove the item from memory
	}
}

void COleClientItem::Release(OLECLOSE dwCloseOption)
{
	ASSERT_VALID(this);

	m_scLast = S_OK;

	// cleanup the OLE object itself
	if (m_lpObject != NULL)
	{
		// cleanup view advise
		LPVIEWOBJECT lpViewObject = _AFXQUERYINTERFACE(m_lpObject, IViewObject);
		if (lpViewObject != NULL)
		{
			DWORD dwAspect;
			VERIFY(lpViewObject->GetAdvise(&dwAspect, NULL, NULL) == NOERROR);
			VERIFY(lpViewObject->SetAdvise(dwAspect, 0, NULL) == NOERROR);
			lpViewObject->Release();
		}

		// cleanup object advise
		if (m_dwConnection != 0)
		{
			VERIFY(m_lpObject->Unadvise(m_dwConnection) == NOERROR);
			m_dwConnection = 0;
		}

		// close object and save (except now when called from destructor)
		//  (NOTE: errors are _not_ reported as an exception)
		m_scLast = GetScode(m_lpObject->Close(dwCloseOption));
		_AfxRelease((LPUNKNOWN*)&m_lpObject);
	}

	// cleanup storage related data
	_AfxRelease((LPUNKNOWN*)&m_lpStorage);
	_AfxRelease((LPUNKNOWN*)&m_lpLockBytes);

	// cleanup in-place editing data
	if (m_pInPlaceFrame != NULL)
	{
		m_pInPlaceFrame->InternalRelease();
		m_pInPlaceFrame = NULL;
		if (m_pInPlaceDoc != NULL)
		{
			m_pInPlaceDoc->InternalRelease();
			m_pInPlaceDoc = NULL;
		}
	}
	ASSERT(m_pInPlaceFrame == NULL);
	ASSERT(m_pInPlaceDoc == NULL);
}

void COleClientItem::Close(OLECLOSE dwCloseOption)
{
	ASSERT_VALID(this);
	ASSERT(m_lpObject != NULL);

	// gaurd against re-entry
	if (m_bClosing)
		return;

	m_bClosing = TRUE;

	// attempt to close the object
	m_scLast = GetScode(m_lpObject->Close(dwCloseOption));

	// remove external lock placed on item during in-place activation
	if (m_bLocked)
	{
		OleLockRunning(m_lpObject, FALSE, TRUE);
		m_bLocked = FALSE;
	}

	// handle failure cases -- COleClientItem::Close can be used to
	//  robustly handle a server crashing (ie. something unexpected happens,
	//  we'll call COleClientItem::Close to attempt safe shutdown)
	if (GetItemState() != loadedState)
	{
		// We'll call COleClientItem::Close anywhere a catastrophy
		//  happens inside of other portions of COleClientItem.  We must
		//  completely exit from any in-place/open state.

		// force transition from activeUIState to activeState
		if (GetItemState() == activeUIState)
			OnDeactivateUI(FALSE);

		// force transition from activeState to loadedState
		if (GetItemState() == activeState)
			OnDeactivate();

		if (m_nItemState != loadedState)
		{
			// in case of extreme failure, force loadedState
			OnChange(OLE_CHANGED_STATE, (DWORD)loadedState);
			m_nItemState = loadedState; // force it to loaded state
		}
	}

	m_bClosing = FALSE; // now safe for further close calls
}

/////////////////////////////////////////////////////////////////////////////
// COleClientItem name management

DWORD COleClientItem::GetNewItemNumber()
{
	ASSERT_VALID(this);

	COleDocument* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	DWORD dwNextItemNumber = pDoc->m_dwNextItemNumber;

	for (;;)
	{
		// make sure that m_dwNextItemNumber is not used in another item first
		POSITION pos = pDoc->GetStartPosition();
		COleClientItem* pItem;
		while ((pItem = pDoc->GetNextClientItem(pos)) != NULL)
		{
			if (pItem->m_dwItemNumber == dwNextItemNumber)
				break;
		}
		if (pItem == NULL)
			break;  // no item using m_dwNextItemNumber

		// m_dwNextItemNumber is in use, try next one!
		++dwNextItemNumber;
	}

	pDoc->m_dwNextItemNumber = dwNextItemNumber + 1;
	return dwNextItemNumber;
}

void COleClientItem::GetItemName(char* pszItemName) const
{
	ASSERT_VALID(this);
	ASSERT(pszItemName != NULL);

	static char BASED_CODE szItemFormat[] = "Embedding %lu";
	wsprintf(pszItemName, szItemFormat, m_dwItemNumber);
	ASSERT(lstrlen(pszItemName) < OLE_MAXITEMNAME);
}

/////////////////////////////////////////////////////////////////////////////
// COleClientItem creation helpers

void COleClientItem::UpdateItemType()
{
	ASSERT_VALID(this);
	ASSERT(m_lpObject != NULL);

	// check for linked object
	LPOLELINK lpOleLink = _AFXQUERYINTERFACE(m_lpObject, IOleLink);
	if (lpOleLink != NULL)
	{
		ASSERT(lpOleLink != NULL);
		lpOleLink->Release();
		m_nItemType = OT_LINK;
		return;
	}

	// check for static object
	DWORD dwStatus;
	if (m_lpObject->GetMiscStatus(DVASPECT_CONTENT, &dwStatus) == NOERROR)
	{
		if (dwStatus & OLEMISC_STATIC)
		{
			m_nItemType = OT_STATIC;
			return;
		}
	}

	// not static, not link -- must be embedded
	m_nItemType = OT_EMBEDDED;
	return;
}

BOOL COleClientItem::FinishCreate(HRESULT hr)
{
	ASSERT_VALID(this);
	ASSERT(m_pView == NULL);

	TRY
	{
		// check return code from create function
		CheckGeneral(hr);

		UpdateItemType();
		if (GetType() != OT_STATIC)
		{
			// setup for advises; we assume that OLE cleans them up properly
			LPADVISESINK lpAdviseSink =
				(LPADVISESINK)GetInterface(&IID_IAdviseSink);
			ASSERT(lpAdviseSink != NULL);
			CheckGeneral(m_lpObject->Advise(lpAdviseSink, &m_dwConnection));
			ASSERT(m_dwConnection != 0);

			// setup for view advises
			LPVIEWOBJECT lpViewObject = NULL;
			CheckGeneral(m_lpObject->QueryInterface(IID_IViewObject,
				(LPLP)&lpViewObject));
			ASSERT(lpViewObject != NULL);
			VERIFY(lpViewObject->SetAdvise(DVASPECT_CONTENT, 0, lpAdviseSink) == NOERROR);
			lpViewObject->Release();

			// the server shows these in it's user-interface
			//  (as document title and in File Exit menu)
			m_lpObject->SetHostNames(AfxGetAppName(), m_pDocument->GetTitle());
		}

		// all items are "contained" -- this makes our reference to this object
		//  weak -- which is needed for links to embedding silent update.
		::OleSetContainedObject(m_lpObject, TRUE);

		// considered loaded at this point
		m_nItemState = loadedState;
	}
	CATCH_ALL(e)
	{
		Release();  // release the object just in case

		ASSERT_VALID(this);
		return FALSE;
	}
	END_CATCH_ALL

	// set state to loaded
	ASSERT(m_nItemState != emptyState);

	// otherwise no errors, return success!
	ASSERT_VALID(this);
	return TRUE;
}

//////////////////////////////////////////////////////////////////////////////
// COleClientItem create API variants

BOOL COleClientItem::CreateFromClipboard(
	OLERENDER render, CLIPFORMAT cfFormat, LPFORMATETC lpFormatEtc)
{
	ASSERT_VALID(this);
	ASSERT(m_lpObject == NULL);     // one time only
	ASSERT(m_pDocument != NULL);
	ASSERT(lpFormatEtc == NULL ||
		AfxIsValidAddress(lpFormatEtc, sizeof(FORMATETC), FALSE));

	// get clipboard contents
	COleDataObject dataObject;
	if (!dataObject.AttachClipboard())
		return FALSE;

	// create from IDataObject
	BOOL bResult = CreateFromData(&dataObject, render, cfFormat, lpFormatEtc);

	ASSERT_VALID(this);
	return bResult;
}

BOOL COleClientItem::CreateLinkFromClipboard(
	OLERENDER render, CLIPFORMAT cfFormat, LPFORMATETC lpFormatEtc)
{
	ASSERT_VALID(this);
	ASSERT(m_lpObject == NULL);     // one time only
	ASSERT(m_pDocument != NULL);
	ASSERT(lpFormatEtc == NULL ||
		AfxIsValidAddress(lpFormatEtc, sizeof(FORMATETC), FALSE));

	// get clipboard contents
	COleDataObject dataObject;
	if (!dataObject.AttachClipboard())
		return FALSE;

	// create from IDataObject
	BOOL bResult = CreateLinkFromData(&dataObject, render, cfFormat, lpFormatEtc);
	ASSERT_VALID(this);

	return bResult;
}

BOOL COleClientItem::CreateStaticFromClipboard(
	OLERENDER render, CLIPFORMAT cfFormat, LPFORMATETC lpFormatEtc)
{
	ASSERT_VALID(this);
	ASSERT(m_lpObject == NULL);     // one time only
	ASSERT(m_pDocument != NULL);
	ASSERT(lpFormatEtc == NULL ||
		AfxIsValidAddress(lpFormatEtc, sizeof(FORMATETC), FALSE));

	// get clipboard contents
	COleDataObject dataObject;
	if (!dataObject.AttachClipboard())
		return FALSE;

	// create from IDataObject
	BOOL bResult = CreateStaticFromData(&dataObject, render, cfFormat,
		lpFormatEtc);

	ASSERT_VALID(this);
	return bResult;
}

// Creation from IDataObject (used for drag-drop)
BOOL COleClientItem::CreateFromData(COleDataObject* pDataObject,
	OLERENDER render, CLIPFORMAT cfFormat, LPFORMATETC lpFormatEtc)
{
	ASSERT_VALID(this);
	ASSERT(m_lpObject == NULL);     // one time only
	ASSERT(m_pDocument != NULL);
	ASSERT(lpFormatEtc == NULL ||
		AfxIsValidAddress(lpFormatEtc, sizeof(FORMATETC), FALSE));

	// get storage for the object via virtual function call
	m_dwItemNumber = GetNewItemNumber();
	GetItemStorage();
	ASSERT(m_lpStorage != NULL);

	// fill in FORMATETC struct
	FORMATETC formatEtc;
	lpFormatEtc = _AfxFillFormatEtc(lpFormatEtc, cfFormat, &formatEtc);

	// attempt to create the object
	BOOL bResult = FinishCreate(::OleCreateFromData(
		pDataObject->m_lpDataObject, IID_IOleObject, render,
		lpFormatEtc, GetClientSite(), m_lpStorage, (LPLP)&m_lpObject));
	ASSERT_VALID(this);

	return bResult;
}

BOOL COleClientItem::CreateLinkFromData(COleDataObject* pDataObject,
	OLERENDER render, CLIPFORMAT cfFormat, LPFORMATETC lpFormatEtc)
{
	ASSERT_VALID(this);
	ASSERT(m_lpObject == NULL);     // one time only
	ASSERT(m_pDocument != NULL);
	ASSERT(lpFormatEtc == NULL ||
		AfxIsValidAddress(lpFormatEtc, sizeof(FORMATETC), FALSE));

	// get storage for the object via virtual function call
	m_dwItemNumber = GetNewItemNumber();
	GetItemStorage();
	ASSERT(m_lpStorage != NULL);

	// fill in FORMATETC struct
	FORMATETC formatEtc;
	lpFormatEtc = _AfxFillFormatEtc(lpFormatEtc, cfFormat, &formatEtc);

	// attempt to create the link
	BOOL bResult = FinishCreate(::OleCreateLinkFromData(
		pDataObject->m_lpDataObject, IID_IOleObject,
		render, lpFormatEtc, GetClientSite(), m_lpStorage, (LPLP)&m_lpObject));
	ASSERT_VALID(this);

	return bResult;
}

BOOL COleClientItem::CreateStaticFromData(COleDataObject* pDataObject,
	OLERENDER render, CLIPFORMAT cfFormat, LPFORMATETC lpFormatEtc)
{
	ASSERT_VALID(this);
	ASSERT(m_lpObject == NULL);     // one time only
	ASSERT(m_pDocument != NULL);
	ASSERT(lpFormatEtc == NULL ||
		AfxIsValidAddress(lpFormatEtc, sizeof(FORMATETC), FALSE));

	// get storage for the object via virtual function call
	m_dwItemNumber = GetNewItemNumber();
	GetItemStorage();
	ASSERT(m_lpStorage != NULL);

	// fill in FORMATETC struct
	FORMATETC formatEtc;
	lpFormatEtc = _AfxFillFormatEtc(lpFormatEtc, cfFormat, &formatEtc);

	// attempt to create the link
	BOOL bResult = FinishCreate(::OleCreateStaticFromData(
		pDataObject->m_lpDataObject, IID_IOleObject,
		render, lpFormatEtc, GetClientSite(), m_lpStorage, (LPLP)&m_lpObject));
	ASSERT_VALID(this);

	return bResult;
}

// Creation from files (in OLE 1.0, the user did this through packager)
BOOL COleClientItem::CreateFromFile(LPCSTR lpszFileName, REFCLSID clsid,
	OLERENDER render, CLIPFORMAT cfFormat, LPFORMATETC lpFormatEtc)
{
	ASSERT_VALID(this);
	ASSERT(m_lpObject == NULL);     // one time only
	ASSERT(m_pDocument != NULL);
	ASSERT(lpFormatEtc == NULL ||
		AfxIsValidAddress(lpFormatEtc, sizeof(FORMATETC), FALSE));

	// get storage for the object via virtual function call
	m_dwItemNumber = GetNewItemNumber();
	GetItemStorage();
	ASSERT(m_lpStorage != NULL);

	// fill in FORMATETC struct
	FORMATETC formatEtc;
	lpFormatEtc = _AfxFillFormatEtc(lpFormatEtc, cfFormat, &formatEtc);

	// attempt to create the object
	BOOL bResult = FinishCreate(::OleCreateFromFile(clsid, lpszFileName,
		IID_IOleObject, render, lpFormatEtc, GetClientSite(), m_lpStorage,
		(LPLP)&m_lpObject));
	ASSERT_VALID(this);

	return bResult;
}

BOOL COleClientItem::CreateLinkFromFile(LPCSTR lpszFileName,
	OLERENDER render, CLIPFORMAT cfFormat, LPFORMATETC lpFormatEtc)
{
	ASSERT_VALID(this);
	ASSERT(m_lpObject == NULL);     // one time only
	ASSERT(m_pDocument != NULL);
	ASSERT(lpFormatEtc == NULL ||
		AfxIsValidAddress(lpFormatEtc, sizeof(FORMATETC), FALSE));

	// get storage for the object via virtual function call
	m_dwItemNumber = GetNewItemNumber();
	GetItemStorage();
	ASSERT(m_lpStorage != NULL);

	// fill in FORMATETC struct
	FORMATETC formatEtc;
	lpFormatEtc = _AfxFillFormatEtc(lpFormatEtc, cfFormat, &formatEtc);

	// attempt to create the link
	BOOL bResult = FinishCreate(::OleCreateLinkToFile(lpszFileName,
		IID_IOleObject, render, lpFormatEtc, GetClientSite(), m_lpStorage,
		(LPLP)&m_lpObject));
	ASSERT_VALID(this);

	return bResult;
}

// create from class name (for insert item dialog)
BOOL COleClientItem::CreateNewItem(REFCLSID clsid,
	OLERENDER render, CLIPFORMAT cfFormat, LPFORMATETC lpFormatEtc)
{
	ASSERT_VALID(this);
	ASSERT(m_lpObject == NULL);     // one time only
	ASSERT(m_pDocument != NULL);
	ASSERT(lpFormatEtc == NULL ||
		AfxIsValidAddress(lpFormatEtc, sizeof(FORMATETC), FALSE));

	// get storage for the object via virtual function call
	m_dwItemNumber = GetNewItemNumber();
	GetItemStorage();
	ASSERT(m_lpStorage != NULL);

	// fill in FORMATETC struct
	FORMATETC formatEtc;
	lpFormatEtc = _AfxFillFormatEtc(lpFormatEtc, cfFormat, &formatEtc);

	// attempt to create the object
	BOOL bResult = FinishCreate(::OleCreate(clsid, IID_IOleObject,
		render, lpFormatEtc, GetClientSite(), m_lpStorage, (LPLP)&m_lpObject));
	ASSERT_VALID(this);

	return bResult;
}

/////////////////////////////////////////////////////////////////////////////
// More advanced creation

BOOL COleClientItem::CreateCloneFrom(COleClientItem* pSrcItem)
{
	ASSERT_VALID(this);
	ASSERT(m_lpObject == NULL);     // one time only
	ASSERT_VALID(pSrcItem);
	ASSERT(m_pDocument != NULL);

	// create storage for the item
	m_dwItemNumber = GetNewItemNumber();
	GetItemStorage();
	ASSERT(m_lpStorage != NULL);

	// save the object first
	LPPERSISTSTORAGE lpPersistStorage =
		_AFXQUERYINTERFACE(pSrcItem->m_lpObject, IPersistStorage);
	ASSERT(lpPersistStorage != NULL);
	HRESULT hr = ::OleSave(lpPersistStorage, m_lpStorage, FALSE);
	lpPersistStorage->SaveCompleted(NULL);
	lpPersistStorage->Release();
	if (hr != NOERROR)
	{
		// failed the save, do not attempt to create clone
		m_scLast = GetScode(hr);
		return FALSE;
	}

	// get information on the view advise type
	LPVIEWOBJECT lpViewObject =
		_AFXQUERYINTERFACE(pSrcItem->m_lpObject, IViewObject);
	ASSERT(lpViewObject != NULL);
	DWORD dwAspect;
	VERIFY(lpViewObject->GetAdvise(&dwAspect, NULL, NULL) == NOERROR);
	lpViewObject->Release();

	// then load the new object from the new storage
	BOOL bResult = FinishCreate(::OleLoad(m_lpStorage, IID_IOleObject,
		GetClientSite(), (LPLP)&m_lpObject));

	ASSERT_VALID(this);
	return bResult;
}

/////////////////////////////////////////////////////////////////////////////
// Storage for COleClientItem objects (memory and compound files)

BOOL COleClientItem::IsModified() const
{
	ASSERT_VALID(this);
	ASSERT(m_lpObject != NULL);

	// get IPersistStorage interface, and call IsDirty
	LPPERSISTSTORAGE lpPersistStorage =
		_AFXQUERYINTERFACE(m_lpObject, IPersistStorage);
	ASSERT(lpPersistStorage != NULL);
	HRESULT hr = lpPersistStorage->IsDirty();
	lpPersistStorage->Release();

	// NOERROR == S_OK == S_TRUE, therefore object dirty!
	return hr == NOERROR || FAILED(hr);
}

void COleClientItem::GetItemStorageFlat()
{
	ASSERT_VALID(this);
	ASSERT(m_lpStorage == NULL);
	ASSERT(m_lpLockBytes == NULL);

	HRESULT hr = ::CreateILockBytesOnHGlobal(NULL, TRUE, &m_lpLockBytes);
	if (hr != NOERROR)
		AfxThrowOleException(hr);
	ASSERT(m_lpLockBytes != NULL);

	hr = ::StgCreateDocfileOnILockBytes(m_lpLockBytes,
		STGM_SHARE_EXCLUSIVE|STGM_CREATE|STGM_READWRITE, 0, &m_lpStorage);
	if (hr != NOERROR)
	{
		VERIFY(m_lpLockBytes->Release() == 0);
		m_lpLockBytes = NULL;
		AfxThrowOleException(hr);
	}
	ASSERT(m_lpStorage != NULL);

	ASSERT_VALID(this);
}

void COleClientItem::ReadItemFlat(CArchive& ar)
{
	ASSERT_VALID(this);
	ASSERT(m_lpStorage == NULL);
	ASSERT(m_lpLockBytes == NULL);

	// read number of bytes in the ILockBytes
	DWORD dwBytes;
	ar >> dwBytes;

	// allocate enough memory to read entire block
	HGLOBAL hStorage = ::GlobalAlloc(GMEM_MOVEABLE|GMEM_SHARE, dwBytes);
	if (hStorage == NULL)
		AfxThrowMemoryException();

	LPVOID lpBuf = ::GlobalLock(hStorage);
	ASSERT(lpBuf != NULL);
	DWORD dwBytesRead;
	if (dwBytes >= 0x10000)
	{
		// more than 64K, use CFile interface for huge Read
		ar.Flush();
		CFile* pFile = ar.GetFile();
		ASSERT(pFile != NULL);
		dwBytesRead = pFile->ReadHuge(lpBuf, dwBytes);
	}
	else
	{
		// small block, use CArchive buffering
		dwBytesRead = ar.Read(lpBuf, (UINT)dwBytes);
	}
	::GlobalUnlock(hStorage);

	// throw exception in case of partial object
	if (dwBytesRead != dwBytes)
	{
		::GlobalFree(hStorage);
		AfxThrowArchiveException(CArchiveException::endOfFile);
	}

	HRESULT hr = CreateILockBytesOnHGlobal(hStorage, TRUE, &m_lpLockBytes);
	if (hr != NOERROR)
	{
		::GlobalFree(hStorage);
		AfxThrowOleException(hr);
	}
	ASSERT(m_lpLockBytes != NULL);
	ASSERT(::StgIsStorageILockBytes(m_lpLockBytes) == NOERROR);

	hr = ::StgOpenStorageOnILockBytes(m_lpLockBytes, NULL,
		STGM_SHARE_EXCLUSIVE|STGM_READWRITE, NULL, 0, &m_lpStorage);
	if (hr != NOERROR)
	{
		VERIFY(m_lpLockBytes->Release() == 0);
		m_lpLockBytes = NULL;
			// ILockBytes::Release will GlobalFree the hStorage
		AfxThrowOleException(hr);
	}

	// attempt to load the object from the storage
	hr = ::OleLoad(m_lpStorage, IID_IOleObject, GetClientSite(),
		(LPLP)&m_lpObject);
	CheckGeneral(hr);

	ASSERT_VALID(this);
}

void COleClientItem::WriteItemFlat(CArchive& ar)
{
	ASSERT_VALID(this);
	ASSERT(m_lpStorage != NULL);
	ASSERT(m_lpLockBytes != NULL);

	// save the OLE object to its storage first
	LPPERSISTSTORAGE lpPersistStorage =
		_AFXQUERYINTERFACE(m_lpObject, IPersistStorage);
	ASSERT(lpPersistStorage != NULL);
	HRESULT hr;
	if (GetDocument()->m_bCompoundFile || lpPersistStorage->IsDirty() == NOERROR)
	{
		hr = ::OleSave(lpPersistStorage, m_lpStorage,
			!GetDocument()->m_bCompoundFile);
		lpPersistStorage->SaveCompleted(NULL);
	}
	lpPersistStorage->Release();
	m_lpStorage->Commit(STGC_OVERWRITE);
	ASSERT(::StgIsStorageILockBytes(m_lpLockBytes) == NOERROR);

	// attempt to get the handle to the global memory
	HGLOBAL hStorage;
	hr = ::GetHGlobalFromILockBytes(m_lpLockBytes, &hStorage);
	if (hr != NOERROR)
		AfxThrowOleException(hr);

	// first write a byte count
	STATSTG statstg;
	hr = m_lpLockBytes->Stat(&statstg, STATFLAG_NONAME);
	if (hr != NOERROR)
		AfxThrowOleException(hr);
	ASSERT(statstg.cbSize.HighPart == 0);   // don't support >4GB objects
	DWORD dwBytes = statstg.cbSize.LowPart;
	ar << dwBytes;

	// write the contents of the block
	LPVOID lpBuf = GlobalLock(hStorage);
	ASSERT(lpBuf != NULL);
	if (dwBytes >= 0x10000)
	{
		// more than 64K, flush the archive and write via CFile
		ar.Flush();
		CFile* pFile = ar.GetFile();
		ASSERT(pFile != NULL);
		pFile->WriteHuge(lpBuf, dwBytes);
	}
	else
	{
		// small block use CArchive buffering
		ar.Write(lpBuf, (UINT)dwBytes);
	}
	::GlobalUnlock(hStorage);
}

void COleClientItem::GetItemStorageCompound()
{
	COleDocument* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	ASSERT(pDoc->m_lpRootStg != NULL);
	ASSERT(pDoc->m_bCompoundFile);

	// get item name
	char szItemName[OLE_MAXITEMNAME];
	GetItemName(szItemName);

	// create storage for this item
	LPSTORAGE lpStorage;
	HRESULT hr = pDoc->m_lpRootStg->CreateStorage(szItemName,
		STGM_CREATE|STGM_READWRITE|STGM_TRANSACTED|STGM_SHARE_EXCLUSIVE,
		0, 0, &lpStorage);
	if (hr != NOERROR)
	{
		TRACE1("Warning: unable to create child storage %s\n", (char*)szItemName);
		// upon failure throw file exception (item will be cleaned up)
		AfxThrowOleException(hr);
		ASSERT(FALSE);
	}
	ASSERT(lpStorage != NULL);

	// everything should have worked
	m_lpStorage = lpStorage;
	ASSERT(m_lpStorage != NULL);
}

void COleClientItem::ReadItemCompound(CArchive& ar)
{
	COleDocument* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	ASSERT(pDoc->m_lpRootStg != NULL);
	ASSERT(pDoc->m_bCompoundFile);
	ASSERT(m_lpStorage == NULL);
	ASSERT(m_lpLockBytes == NULL);

	if (ar.m_bForceFlat)
	{
		ReadItemFlat(ar);
		_AfxRelease((LPUNKNOWN*)&m_lpStorage);
		_AfxRelease((LPUNKNOWN*)&m_lpLockBytes);

		// change the number to something definitely unique
		m_dwItemNumber = GetNewItemNumber();

		// create new storage
		GetItemStorageCompound();
		LPPERSISTSTORAGE lpPersistStorage =
			_AFXQUERYINTERFACE(m_lpObject, IPersistStorage);
		ASSERT(lpPersistStorage != NULL);
		HRESULT hr = ::OleSave(lpPersistStorage, m_lpStorage, FALSE);
		if (FAILED(hr))
		{
			lpPersistStorage->Release();
			CheckGeneral(hr);
		}
		VERIFY(lpPersistStorage->SaveCompleted(m_lpStorage) == NOERROR);
		lpPersistStorage->Release();
	}
	else
	{
		// get item name
		char szItemName[OLE_MAXITEMNAME];
		GetItemName(szItemName);

		// open storage for this item
		LPSTORAGE lpStorage;
		HRESULT hr = pDoc->m_lpRootStg->OpenStorage(szItemName, NULL,
			STGM_READWRITE|STGM_TRANSACTED|STGM_SHARE_EXCLUSIVE,
			0, 0, &lpStorage);
		if (hr != NOERROR)
		{
			TRACE1("Warning: unable to open child storage %s\n", (char*)szItemName);
			// upon failure throw file exception (item will be cleaned up)
			AfxThrowOleException(hr);
		}
		ASSERT(lpStorage != NULL);

		// remember the storage
		m_lpStorage = lpStorage;
		ASSERT(m_lpStorage != NULL);

		// attempt to load the object from the storage
		hr = ::OleLoad(m_lpStorage, IID_IOleObject, GetClientSite(), (LPLP)&m_lpObject);
		CheckGeneral(hr);
	}
}

void COleClientItem::WriteItemCompound(CArchive& ar)
{
	COleDocument* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	ASSERT(pDoc->m_lpRootStg != NULL);
	ASSERT(pDoc->m_bCompoundFile);
	ASSERT(m_lpNewStorage == NULL);

	if (ar.m_bForceFlat)
	{
		LPSTORAGE pTempStorage = m_lpStorage;
		LPLOCKBYTES pTempLockBytes = m_lpLockBytes;
		m_lpStorage = NULL;
		m_lpLockBytes = NULL;
		GetItemStorageFlat();
		WriteItemFlat(ar);
		_AfxRelease((LPUNKNOWN*)&m_lpStorage);
		_AfxRelease((LPUNKNOWN*)&m_lpLockBytes);
		m_lpStorage = pTempStorage;
		m_lpLockBytes = pTempLockBytes;
		return;
	}

	// get item name
	char szItemName[OLE_MAXITEMNAME];
	GetItemName(szItemName);

	// determine destination storage
	ASSERT(m_lpStorage != NULL);
	LPSTORAGE lpStorage = m_lpStorage;
	if (!pDoc->m_bSameAsLoad)
	{
		// need to create new storage for this item
		HRESULT hr = pDoc->m_lpRootStg->CreateStorage(szItemName,
			STGM_CREATE|STGM_READWRITE|STGM_TRANSACTED|STGM_SHARE_EXCLUSIVE,
			0, 0, &lpStorage);
		if (hr != NOERROR)
		{
			TRACE1("Warning: unable to create child storage %s\n",
				(char*)szItemName);
			AfxThrowOleException(hr);
		}
		// remember the storage for CommitItem stage
		m_lpNewStorage = lpStorage;
		m_bNeedCommit = TRUE;
	}
	ASSERT(lpStorage != NULL);

	// save dirty object
	LPPERSISTSTORAGE lpPersistStorage =
		_AFXQUERYINTERFACE(m_lpObject, IPersistStorage);
	ASSERT(lpPersistStorage != NULL);
	HRESULT hr = NOERROR;
	if (!pDoc->m_bSameAsLoad || lpPersistStorage->IsDirty() == NOERROR)
	{
		// call OleSave now and IPersistStorage::Save later
		hr = ::OleSave(lpPersistStorage, lpStorage, pDoc->m_bSameAsLoad);
	}
	lpPersistStorage->Release();

	// if it fails, abort the save
	if (FAILED(hr))
		AfxThrowOleException(hr);

	// now will need to call CommitItem for this item
	m_bNeedCommit = TRUE;
	lpStorage->Commit(STGC_ONLYIFCURRENT);
}

void COleClientItem::GetItemStorage()
{
	if (GetDocument()->m_bCompoundFile)
		GetItemStorageCompound();
	else
		GetItemStorageFlat();
}

void COleClientItem::ReadItem(CArchive& ar)
{
	if (GetDocument()->m_bCompoundFile)
		ReadItemCompound(ar);
	else
		ReadItemFlat(ar);
}

void COleClientItem::WriteItem(CArchive& ar)
{
	if (GetDocument()->m_bCompoundFile)
		WriteItemCompound(ar);
	else
		WriteItemFlat(ar);
}

void COleClientItem::CommitItem(BOOL bSuccess)
{
	ASSERT_VALID(this);
	ASSERT(m_lpObject != NULL);

	if (!m_bNeedCommit)
		return;

	LPPERSISTSTORAGE lpPersistStorage =
		_AFXQUERYINTERFACE(m_lpObject, IPersistStorage);
	ASSERT(lpPersistStorage != NULL);

	// forget about new storage if save failed along the way...
	if (m_lpNewStorage != NULL && !bSuccess)
		_AfxRelease((LPUNKNOWN*)&m_lpNewStorage);

	// let the object remember the new storage
	VERIFY(lpPersistStorage->SaveCompleted(m_lpNewStorage) == NOERROR);
	lpPersistStorage->Release();

	// determine/remember new storage
	if (m_lpNewStorage != NULL)
	{
		m_lpStorage->Release();
		m_lpStorage = m_lpNewStorage;
		m_lpNewStorage = NULL;
	}

	m_bNeedCommit = FALSE;
}

/////////////////////////////////////////////////////////////////////////////
// COleClientItem serialization

void COleClientItem::Serialize(CArchive& ar)
{
	ASSERT_VALID(this);

	CDocItem::Serialize(ar);
	ASSERT(m_pDocument != NULL);  // must 'SetDocument' first

	if (ar.IsStoring())
	{
		ASSERT(m_lpObject != NULL);

		// first, save the type flag (this is used for versioning)
		ar << (DWORD)OT_OLE2;

		ar << m_dwItemNumber;   // save the item number

		// write the view advise type to storage
		LPVIEWOBJECT lpViewObject = _AFXQUERYINTERFACE(m_lpObject, IViewObject);
		ASSERT(lpViewObject != NULL);
		DWORD dwAspect;
		VERIFY(lpViewObject->GetAdvise(&dwAspect, NULL, NULL) == NOERROR);
		lpViewObject->Release();
		ar << dwAspect;         // save the display aspect

		// write flag indicating whether to create moniker upon load
		ar << (WORD)m_bMoniker;

		// save current default display aspect
		ar << (DWORD)m_nDrawAspect;

		// save object to storage (calls OleSave)
		WriteItem(ar);
	}
	else
	{
		ASSERT(m_lpObject == NULL);

		// first, get the type flag (for OLE 1.0 compatible reading)
		DWORD dwType;
		ar >> dwType;
		if (dwType != OT_OLE2)
		{
			// the item was not serialized correctly
			AfxThrowArchiveException(CArchiveException::generic);
		}
		ASSERT(dwType == OT_OLE2);

		ar >> m_dwItemNumber;   // get the item number

		DWORD dwAspect; // read the display aspect (aspects that are cached)
		ar >> dwAspect;

		WORD bMoniker;  // see if we should create & set the moniker
		ar >> bMoniker;

		DWORD nDrawAspect;  // read the default display aspect
		ar >> nDrawAspect;
		m_nDrawAspect = (DVASPECT)nDrawAspect;

		// read the OLE object from storage (calls OleLoad)
		ReadItem(ar);

		// finish OLE object creation process, setup advises, etc.
		if (!FinishCreate(NOERROR))
			AfxThrowArchiveException(CArchiveException::generic);

		if (bMoniker)
		{
			// force moniker creation by calling GetMoniker
			LPMONIKER lpMoniker;
			if (GetClientSite()->GetMoniker(OLEGETMONIKER_FORCEASSIGN,
				OLEWHICHMK_OBJREL, &lpMoniker) == NOERROR)
			{
				ASSERT(lpMoniker != NULL);
				lpMoniker->Release();
				ASSERT(m_bMoniker); // moniker should have been assigned
			}
		}

		// fix up the document's m_dwNextItemNumber
		if (m_dwItemNumber >= GetDocument()->m_dwNextItemNumber)
			GetDocument()->m_dwNextItemNumber = m_dwItemNumber + 1;
	}
}

/////////////////////////////////////////////////////////////////////////////
// default callback implementation

void COleClientItem::OnChange(OLE_NOTIFICATION nCode, DWORD dwParam)
{
	ASSERT_VALID(this);

	switch (nCode)
	{
	case OLE_CLOSED:
		break;  // no default implementation

	case OLE_CHANGED:
	case OLE_SAVED:
		ASSERT(m_pDocument != NULL);
		m_pDocument->SetModifiedFlag();
		break;

	case OLE_CHANGED_STATE:
	case OLE_CHANGED_ASPECT:
		break;  // no default implementation

	default:
		ASSERT(FALSE);
	}

	ASSERT_VALID(this);
}

void COleClientItem::OnDataChange(
	LPFORMATETC lpFormatEtc, LPSTGMEDIUM lpStgMedium)
{
	ASSERT(FALSE);  // derivative must override -- must not call base class
}

void COleClientItem::OnGetItemPosition(CRect& rPosition)
{
	// default does nothing
}

void COleClientItem::OnGetClipRect(CRect& rClipRect)
{
	ASSERT_VALID(this);
	ASSERT(AfxIsValidAddress(&rClipRect, sizeof(RECT)));

	// default clips rectClip to client area of the active view
	ASSERT_VALID(m_pView);
	m_pView->GetClientRect(&rClipRect);
}

void COleClientItem::OnShowItem()
{
	ASSERT_VALID(this);

	CDocument* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	// attempt to use m_pView set during activation
	CView* pView = m_pView;
	if (pView == NULL)
	{
		// otherwise, find the first view of this document
		POSITION pos = pDoc->GetFirstViewPosition();
		if (pos == NULL || (pView = pDoc->GetNextView(pos)) == NULL)
			return;
	}

	CFrameWnd* pFrameWnd = pView->GetParentFrame();
	if (pFrameWnd != NULL)
	{
		// activate frame holding view
		pFrameWnd->ActivateFrame();
		pFrameWnd->OnUpdateFrameTitle(TRUE);

		// activate app frame if necessary
		pFrameWnd = pFrameWnd->GetParentFrame();
		if (pFrameWnd != NULL)
		{
			ASSERT(pFrameWnd->IsKindOf(RUNTIME_CLASS(CFrameWnd)));
			pFrameWnd->ActivateFrame();
			pFrameWnd->OnUpdateFrameTitle(TRUE);
		}
	}

	if (!pDoc->GetPathName().IsEmpty())
	{
		// user is also in control of the application, when a file-based
		//  document becomes visible.
		AfxOleSetUserCtrl(TRUE);
	}
}

/////////////////////////////////////////////////////////////////////////////
// COleClientItem - attributes

void COleClientItem::GetClassID(CLSID FAR* pClassID) const
{
	ASSERT_VALID(this);
	ASSERT(m_lpObject != NULL);
	ASSERT(AfxIsValidAddress(pClassID, sizeof(CLSID)));

	if (m_lpObject->GetUserClassID(pClassID) != NOERROR)
		*pClassID = CLSID_NULL;
}

BOOL COleClientItem::GetExtent(LPSIZE lpSize, DVASPECT nDrawAspect)
{
	ASSERT_VALID(this);
	ASSERT(m_lpObject != NULL);
	ASSERT(AfxIsValidAddress(lpSize, sizeof(CSize)));

	// use current default aspect if specific one not specified
	if (nDrawAspect == -1)
		nDrawAspect = m_nDrawAspect;

	// get the extent
	SIZEL sizel;
	m_scLast = GetScode(m_lpObject->GetExtent(nDrawAspect, &sizel));
	lpSize->cx = (int)sizel.cx;
	lpSize->cy = (int)sizel.cy;

	return m_scLast == S_OK;
}

BOOL COleClientItem::SetIconicMetafile(HGLOBAL hMetaPict)
{
	ASSERT_VALID(this);
	ASSERT(m_lpObject != NULL);

	// get IOleCache interface
	LPOLECACHE lpOleCache = _AFXQUERYINTERFACE(m_lpObject, IOleCache);
	if (lpOleCache == NULL)
	{
		TRACE0("Warning: object does not support IOleCache interface.\n");
		return FALSE;
	}
	ASSERT(lpOleCache != NULL);

	// new cache is for CF_METAFILEPICT, DVASPECT_ICON
	FORMATETC formatEtc;
	formatEtc.cfFormat = CF_METAFILEPICT;
	formatEtc.ptd = NULL;
	formatEtc.dwAspect = DVASPECT_ICON;
	formatEtc.lindex = -1;
	formatEtc.tymed = TYMED_MFPICT;

	// setup the cache so iconic aspect is now included
	DWORD dwConnection;
	HRESULT hr = lpOleCache->Cache(&formatEtc,
		ADVF_NODATA|ADVF_PRIMEFIRST|ADVF_ONLYONCE, &dwConnection);
	if (FAILED(hr))
	{
		lpOleCache->Release();
		return FALSE;
	}

	// set data if iconic image provided
	if (hMetaPict != NULL)
	{
		STGMEDIUM stgMedium;
		stgMedium.tymed = TYMED_MFPICT;
		stgMedium.hGlobal = hMetaPict;
		stgMedium.pUnkForRelease = NULL;

		hr = lpOleCache->SetData(&formatEtc, &stgMedium, FALSE);
		if (FAILED(hr))
		{
			lpOleCache->Release();
			return FALSE;
		}
	}
	lpOleCache->Release();

	return TRUE;
}

HGLOBAL COleClientItem::GetIconicMetafile()
{
	ASSERT_VALID(this);
	ASSERT(m_lpObject != NULL);

	// get IDataObject interface
	LPDATAOBJECT lpDataObject = _AFXQUERYINTERFACE(m_lpObject, IDataObject);
	ASSERT(lpDataObject != NULL);

	// cache is for CF_METAFILEPICT, DVASPECT_ICON
	FORMATETC formatEtc;
	formatEtc.cfFormat = CF_METAFILEPICT;
	formatEtc.ptd = NULL;
	formatEtc.dwAspect = DVASPECT_ICON;
	formatEtc.lindex = -1;
	formatEtc.tymed = TYMED_MFPICT;

	// attempt to get the icon picture
	STGMEDIUM stgMedium;
	if (lpDataObject->GetData(&formatEtc, &stgMedium) != NOERROR)
	{
		lpDataObject->Release();

		// no current picture, attempt to get from class ID
		CLSID clsid;
		if (m_lpObject->GetUserClassID(&clsid) != NOERROR)
			return NULL;

		HGLOBAL hMetaPict = OleGetIconOfClass(clsid, NULL, TRUE);
		if (hMetaPict != NULL)
		{
			// cache it for later GetData (or drawing)
			SetIconicMetafile(hMetaPict);
			return hMetaPict;
		}
		return NULL;
	}
	lpDataObject->Release();

	// can't handle data where punkForRelease is set
	if (stgMedium.pUnkForRelease != NULL)
	{
		::ReleaseStgMedium(&stgMedium);
		return NULL;
	}
	ASSERT(stgMedium.tymed == TYMED_MFPICT);
	ASSERT(stgMedium.hGlobal != NULL);

	return stgMedium.hGlobal;   // return HGLOBAL to METAFILEPICT
}

void COleClientItem::SetDrawAspect(DVASPECT nDrawAspect)
{
	ASSERT_VALID(this);

	// prime iconic cache (in case object has never displayed iconic)
	if (nDrawAspect == DVASPECT_ICON)
	{
		SetIconicMetafile(NULL);    // allow object to provide own icon
		HGLOBAL hMetaPict = GetIconicMetafile();
		if (hMetaPict != NULL)
			OleUIMetafilePictIconFree(hMetaPict);
	}

	// Note: the aspect you are setting may be uncached and therefore blank.
	//  To make sure it is cached use SetIconPicture or use IOleCache to
	//  set the cache yourself.
	m_nDrawAspect = nDrawAspect;

	// mark document as dirty (m_nDrawAspect is part of persistent state)
	ASSERT_VALID(m_pDocument);
	m_pDocument->SetModifiedFlag();
}

// Helper used to get the DVTARGETDEVICE pointer for the first printer.
BOOL COleClientItem::GetPrintDeviceInfo(
	LPOLECACHE* plpOleCache, DVTARGETDEVICE FAR** pptd, DWORD* pdwConnection)
{
	*plpOleCache = NULL;
	*pptd = NULL;

	// get IOleCache interface
	LPOLECACHE lpOleCache = _AFXQUERYINTERFACE(m_lpObject, IOleCache);
	if (lpOleCache == NULL)
	{
		TRACE0("Warning: object does not support IOleCache interface.\n");
		return FALSE;   // no cache -- no possible print device
	}
	ASSERT(lpOleCache != NULL);

	// get enumerator for the cache
	LPENUMSTATDATA lpEnumSTATDATA;
	if (lpOleCache->EnumCache(&lpEnumSTATDATA) != NOERROR || lpEnumSTATDATA == NULL)
	{
		lpOleCache->Release();
		return FALSE;
	}
	// enumerate entries in the cache (look for one with ptd != NULL)
	STATDATA statData;
	while (lpEnumSTATDATA->Next(1, &statData, NULL) == NOERROR)
	{
		ASSERT(statData.pAdvSink == NULL);

		// return first non-NULL ptd (we assume this is a printer cache)
		if (statData.formatetc.ptd != NULL)
		{
			if (pdwConnection != NULL)
				*pdwConnection = statData.dwConnection;
			*pptd = statData.formatetc.ptd;
			lpEnumSTATDATA->Release();

			*plpOleCache = lpOleCache;
			return TRUE;    // Note: lpOleCache pointer is still alive
		}
	}
	// release interfaces
	lpEnumSTATDATA->Release();
	lpOleCache->Release();
	return FALSE;   // data not found
}

void COleClientItem::AttachDataObject(COleDataObject& rDataObject) const
{
	ASSERT_VALID(this);
	ASSERT(m_lpObject != NULL);

	// get the IDataObject interface for the item
	LPDATAOBJECT lpDataObject = _AFXQUERYINTERFACE(m_lpObject, IDataObject);
	ASSERT(lpDataObject != NULL);

	// return it by attaching it
	rDataObject.Attach(lpDataObject, TRUE);
}

/////////////////////////////////////////////////////////////////////////////
// COleClientItem - general operations

BOOL COleClientItem::Draw(CDC* pDC, LPCRECT lpBounds, DVASPECT nDrawAspect)
{
	ASSERT_VALID(this);
	ASSERT(AfxIsValidAddress(lpBounds, sizeof(RECT), FALSE));
	ASSERT_VALID(pDC);

	if (m_lpObject == NULL)
		return FALSE;   // partially created COleClientItem object

	// get IViewObject interface
	LPVIEWOBJECT lpViewObject = _AFXQUERYINTERFACE(m_lpObject, IViewObject);
	ASSERT(lpViewObject != NULL);

	// use current draw aspect if aspect is -1 (default)
	if (nDrawAspect == -1)
		nDrawAspect = m_nDrawAspect;

	// convert RECT lpBounds to RECTL rclBounds
	RECTL rclBounds;
	rclBounds.left = lpBounds->left;
	rclBounds.top = lpBounds->top;
	rclBounds.right = lpBounds->right;
	rclBounds.bottom = lpBounds->bottom;

	// get RECTL describing window extents and origin
	RECTL rclWBounds;
	CPoint ptOrg = pDC->GetWindowOrg();
	CSize sizeExt = pDC->GetWindowExt();
	rclWBounds.left = ptOrg.x;
	rclWBounds.top = ptOrg.y;
	rclWBounds.right = sizeExt.cx;
	rclWBounds.bottom = sizeExt.cy;

	// get target device to use for draw
	COleDocument* pDoc = GetDocument();
	const DVTARGETDEVICE FAR* ptd = NULL;
	HDC hdcTarget = NULL;
	if (pDC->IsPrinting() && pDoc->m_ptd != NULL)
	{
		ptd = pDoc->m_ptd;
		hdcTarget = pDC->m_hAttribDC;
	}

	// try with target device
	HRESULT hr = lpViewObject->Draw(nDrawAspect, -1, NULL,
		(DVTARGETDEVICE FAR*)ptd, hdcTarget, pDC->m_hDC,
		&rclBounds, &rclWBounds, NULL, 0);
	if (ptd != NULL && GetScode(hr) == OLE_E_BLANK)
	{
		// try without target device
		hr = lpViewObject->Draw(nDrawAspect, -1, NULL,
			NULL, NULL, pDC->m_hDC,
			&rclBounds, &rclWBounds, NULL, 0);
	}
	lpViewObject->Release();

	if (hr != NOERROR && GetScode(hr) == OLE_E_BLANK)
		return FALSE;   // return FALSE if the object is blank

	CheckGeneral(hr);   // otherwise, may throw exception on error
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// COleClientItem clipboard helpers

void COleClientItem::GetEmbeddedItemData(LPSTGMEDIUM lpStgMedium)
{
	ASSERT_VALID(this);
	ASSERT(AfxIsValidAddress(lpStgMedium, sizeof(STGMEDIUM)));

	LPLOCKBYTES lpLockBytes;
	HRESULT hr = ::CreateILockBytesOnHGlobal(NULL, TRUE, &lpLockBytes);
	if (hr != NOERROR)
		AfxThrowOleException(hr);
	ASSERT(lpLockBytes != NULL);

	LPSTORAGE lpStorage;
	hr = ::StgCreateDocfileOnILockBytes(lpLockBytes,
		STGM_SHARE_EXCLUSIVE|STGM_CREATE|STGM_READWRITE, 0, &lpStorage);
	if (hr != NOERROR)
	{
		VERIFY(lpLockBytes->Release() == 0);
		AfxThrowOleException(hr);
	}
	ASSERT(lpStorage != NULL);
	VERIFY(lpLockBytes->Release() != 0);

	// save the object into the storage
	LPPERSISTSTORAGE lpPersistStorage =
		_AFXQUERYINTERFACE(m_lpObject, IPersistStorage);
	ASSERT(lpPersistStorage != NULL);
	hr = ::OleSave(lpPersistStorage, lpStorage, FALSE);
	lpPersistStorage->SaveCompleted(NULL);
	lpPersistStorage->Release();
	if (hr != NOERROR)
	{
		VERIFY(lpStorage->Release() == 0);
		AfxThrowOleException(hr);
	}

	// add it to the data source
	lpStgMedium->tymed = TYMED_ISTORAGE;
	lpStgMedium->pstg = lpStorage;
	lpStgMedium->pUnkForRelease = NULL;
}

void COleClientItem::AddCachedData(COleDataSource* pDataSource)
{
	ASSERT_VALID(this);
	ASSERT_VALID(pDataSource);

	// get IOleCache interface
	LPOLECACHE lpOleCache = _AFXQUERYINTERFACE(m_lpObject, IOleCache);
	if (lpOleCache == NULL)
	{
		TRACE0("Warning: object does not support IOleCache interface.\n");
		return;
	}
	ASSERT(lpOleCache != NULL);

	// Get IEnumSTATDATA interface for IOleCache
	LPENUMSTATDATA lpEnumSTATDATA;
	if (lpOleCache->EnumCache(&lpEnumSTATDATA) != NOERROR || lpEnumSTATDATA == NULL)
	{
		lpOleCache->Release();
		return;
	}

	// get IDataObject interface
	LPDATAOBJECT lpDataObject = _AFXQUERYINTERFACE(m_lpObject, IDataObject);
	ASSERT(lpDataObject != NULL);

	// enumerate all of the cached formats
	STATDATA statData;
	while (lpEnumSTATDATA->Next(1, &statData, NULL) == NOERROR)
	{
		ASSERT(statData.pAdvSink == NULL);

		// for each format supported, try to get copy of the data
		STGMEDIUM stgMedium;
		if (lpDataObject->GetData(&statData.formatetc, &stgMedium) != NOERROR)
		{
			// data is not available
			::OleStdDeleteTargetDevice(statData.formatetc.ptd);
		}
		else if (stgMedium.pUnkForRelease != NULL)
		{
			// don't cache data with pUnkForRelease != NULL
			::ReleaseStgMedium(&stgMedium);
			::OleStdDeleteTargetDevice(statData.formatetc.ptd);
		}
		else
		{
			// format was acceptable -- add it to the clipboard
			pDataSource->CacheData(0, &stgMedium, &statData.formatetc);
		}
	}

	// release interfaces
	lpEnumSTATDATA->Release();
	lpDataObject->Release();
	lpOleCache->Release();
}

BOOL COleClientItem::GetLinkSourceData(LPSTGMEDIUM lpStgMedium)
{
	ASSERT_VALID(this);
	ASSERT(AfxIsValidAddress(lpStgMedium, sizeof(STGMEDIUM)));

	LPMONIKER lpMoniker = NULL;
	LPOLELINK lpOleLink = _AFXQUERYINTERFACE(m_lpObject, IOleLink);
	if (lpOleLink == NULL)
	{
		// get moniker from client site
		LPOLECLIENTSITE lpClientSite = GetClientSite();
		ASSERT(lpClientSite != NULL);
		HRESULT hr = lpClientSite->GetMoniker(OLEGETMONIKER_TEMPFORUSER,
			OLEWHICHMK_OBJFULL, &lpMoniker);
		if (hr != NOERROR)
		{
			TRACE0("Warning: unable to get moniker from client site.\n");
			return FALSE;
		}
		ASSERT(lpMoniker != NULL);
	}
	else
	{
		// get moniker from the link object itself
		HRESULT hr = lpOleLink->GetSourceMoniker(&lpMoniker);
		lpOleLink->Release();
		if (hr != NOERROR)
		{
			TRACE0("Warning: unable to get moniker from link source.\n");
			return FALSE;
		}
		ASSERT(lpMoniker != NULL);
	}

	// create a memory based stream to write the moniker to
	LPSTREAM lpStream;
	if (::CreateStreamOnHGlobal(NULL, TRUE, &lpStream) != NOERROR)
	{
		lpMoniker->Release();
		AfxThrowMemoryException();
	}
	ASSERT(lpStream != NULL);

	// write the moniker to the stream, and add it to the clipboard
	HRESULT hr = ::OleSaveToStream(lpMoniker, lpStream);
	lpMoniker->Release();
	if (hr != NOERROR)
	{
		lpStream->Release();
		AfxThrowOleException(hr);
	}

	// write the class ID of the document to the stream as well
	CLSID clsid;
	hr = m_lpObject->GetUserClassID(&clsid);
	if (hr != NOERROR)
	{
		lpStream->Release();
		AfxThrowOleException(hr);
	}
	hr = WriteClassStm(lpStream, clsid);
	if (hr != NOERROR)
	{
		lpStream->Release();
		AfxThrowOleException(hr);
	}

	// add it to the data source
	lpStgMedium->tymed = TYMED_ISTREAM;
	lpStgMedium->pstm = lpStream;
	lpStgMedium->pUnkForRelease = NULL;
	return TRUE;
}

void COleClientItem::GetObjectDescriptorData(
	LPPOINT lpOffset, LPSIZE lpSize, LPSTGMEDIUM lpStgMedium)
{
	ASSERT_VALID(this);
	ASSERT(AfxIsValidAddress(lpStgMedium, sizeof(STGMEDIUM)));
	ASSERT(lpOffset == NULL ||
		AfxIsValidAddress(lpOffset, sizeof(CPoint), FALSE));

	POINTL pointl = { 0, 0 };
	if (lpOffset != NULL)
	{
		CSize ptOffset(lpOffset->x, lpOffset->y);
		((CDC*)NULL)->DPtoHIMETRIC(&ptOffset);
		pointl.x = ptOffset.cx;
		pointl.y = ptOffset.cy;
	}
	SIZEL sizel = { 0, 0 };
	if (lpSize != NULL)
	{
		((CDC*)NULL)->DPtoHIMETRIC(lpSize);
		sizel.cx = lpSize->cx;
		sizel.cy = lpSize->cy;
	}

	COleDocument* pDoc = GetDocument();

	// get the object descriptor for the IOleObject
	++m_dwRef;
	HGLOBAL hGlobal = ::OleStdGetObjectDescriptorDataFromOleObject(
		m_lpObject, (LPSTR)(LPCSTR)pDoc->GetPathName(), m_nDrawAspect,
		pointl, &sizel);
	--m_dwRef;

	if (hGlobal == NULL)
		AfxThrowMemoryException();

	// setup the STGMEDIUM
	lpStgMedium->tymed = TYMED_HGLOBAL;
	lpStgMedium->hGlobal = hGlobal;
	lpStgMedium->pUnkForRelease = NULL;
}

/////////////////////////////////////////////////////////////////////////////
// Embedded COleClientItem operations

void COleClientItem::SetHostNames(LPCSTR lpszHost, LPCSTR lpszHostObj)
{
	ASSERT_VALID(this);
	ASSERT(m_lpObject != NULL);
	ASSERT(AfxIsValidString(lpszHost));
	ASSERT(AfxIsValidString(lpszHostObj));

	CheckGeneral(m_lpObject->SetHostNames(lpszHost, lpszHostObj));
}

void COleClientItem::SetExtent(const CSize& size, DVASPECT nDrawAspect)
{
	ASSERT_VALID(this);
	ASSERT(m_lpObject != NULL);

	SIZEL sizel = { size.cx, size.cy };
	CheckGeneral(m_lpObject->SetExtent(nDrawAspect, &sizel));
}

/////////////////////////////////////////////////////////////////////////////
// COleClientItem OLE interface implementation

BEGIN_INTERFACE_MAP(COleClientItem, CDocItem)
	INTERFACE_PART(COleClientItem, IID_IOleClientSite, OleClientSite)
	INTERFACE_PART(COleClientItem, IID_IAdviseSink, AdviseSink)
	INTERFACE_PART(COleClientItem, IID_IOleWindow, OleIPSite)
	INTERFACE_PART(COleClientItem, IID_IOleInPlaceSite, OleIPSite)
END_INTERFACE_MAP()

/////////////////////////////////////////////////////////////////////////////
// Implementation of IOleClientSite

STDMETHODIMP_(ULONG) COleClientItem::XOleClientSite::AddRef()
{
	METHOD_PROLOGUE(COleClientItem, OleClientSite)
	return (ULONG)pThis->ExternalAddRef();
}

STDMETHODIMP_(ULONG) COleClientItem::XOleClientSite::Release()
{
	METHOD_PROLOGUE(COleClientItem, OleClientSite)
	return (ULONG)pThis->ExternalRelease();
}

STDMETHODIMP COleClientItem::XOleClientSite::QueryInterface(
	REFIID iid, LPVOID far* ppvObj)
{
	METHOD_PROLOGUE(COleClientItem, OleClientSite)
	return (HRESULT)pThis->ExternalQueryInterface(&iid, ppvObj);
}

STDMETHODIMP COleClientItem::XOleClientSite::SaveObject()
{
	METHOD_PROLOGUE(COleClientItem, OleClientSite)
	ASSERT_VALID(pThis);

	LPPERSISTSTORAGE lpPersistStorage =
		_AFXQUERYINTERFACE(pThis->m_lpObject, IPersistStorage);
	ASSERT(lpPersistStorage != NULL);
	HRESULT hr = NOERROR;
	if (lpPersistStorage->IsDirty() == NOERROR)
	{
		// NOERROR == S_OK != S_FALSE, therefore object is dirty!
		hr = ::OleSave(lpPersistStorage, pThis->m_lpStorage, TRUE);
		if (hr != NOERROR)
			hr = lpPersistStorage->SaveCompleted(NULL);

		// mark the document as dirty, if save sucessful.
		pThis->m_pDocument->SetModifiedFlag();
	}
	lpPersistStorage->Release();
	return hr;
}

STDMETHODIMP COleClientItem::XOleClientSite::GetMoniker(
	DWORD dwAssign, DWORD dwWhichMoniker, LPMONIKER FAR* ppMoniker)
{
	METHOD_PROLOGUE(COleClientItem, OleClientSite)
	ASSERT_VALID(pThis);

	COleDocument* pDoc = pThis->GetDocument();
	ASSERT_VALID(pDoc);
	ASSERT(ppMoniker != NULL);
	*ppMoniker = NULL;

	switch (dwWhichMoniker)
	{
	case OLEWHICHMK_CONTAINER:
		// return the current moniker for the document
		*ppMoniker = pDoc->GetMoniker((OLEGETMONIKER)dwAssign);
		break;

	case OLEWHICHMK_OBJREL:
		{
			if (!pDoc->IsKindOf(RUNTIME_CLASS(COleLinkingDoc)))
				break;
			// don't return relative moniker if no document moniker
			LPMONIKER lpMoniker = pDoc->GetMoniker((OLEGETMONIKER)dwAssign);
			if (lpMoniker == NULL)
				break;
			lpMoniker->Release();

			// relative monikers have to handle assignment correctly
			switch (dwAssign)
			{
			case OLEGETMONIKER_ONLYIFTHERE:
				if (!pThis->m_bMoniker)
					break;  // no moniker assigned, don't return one
				// fall through...

			case OLEGETMONIKER_TEMPFORUSER:
			case OLEGETMONIKER_FORCEASSIGN:
				{
					// create item moniker from item name
					char szItemName[OLE_MAXITEMNAME];
					pThis->GetItemName(szItemName);
					::CreateItemMoniker(OLESTDDELIM, szItemName, ppMoniker);

					// notify the object of the assignment
					if (dwAssign != OLEGETMONIKER_TEMPFORUSER &&
						*ppMoniker != NULL && !pThis->m_bMoniker)
					{
						VERIFY(pThis->m_lpObject->SetMoniker(
							OLEWHICHMK_OBJREL, *ppMoniker) == NOERROR);
						pThis->m_bMoniker = TRUE;
						ASSERT_VALID(pThis->m_pDocument);
						pThis->m_pDocument->SetModifiedFlag();
					}
				}
				break;

			case OLEGETMONIKER_UNASSIGN:
				pThis->m_bMoniker = FALSE;
				break;
			}
		}
		break;

	case OLEWHICHMK_OBJFULL:
		{
			// get each sub-moniker: item & document
			LPMONIKER lpMoniker1, lpMoniker2;
			GetMoniker(dwAssign, OLEWHICHMK_CONTAINER, &lpMoniker1);
			GetMoniker(dwAssign, OLEWHICHMK_OBJREL, &lpMoniker2);

			// create composite moniker
			if (lpMoniker1 != NULL && lpMoniker2 != NULL)
				::CreateGenericComposite(lpMoniker1, lpMoniker2, ppMoniker);

			// release sub-monikers
			_AfxRelease((LPUNKNOWN*)&lpMoniker1);
			_AfxRelease((LPUNKNOWN*)&lpMoniker2);
		}
		break;
	}

	return *ppMoniker != NULL ? NOERROR : ResultFromScode(E_FAIL);
}

STDMETHODIMP COleClientItem::XOleClientSite::GetContainer(
	LPOLECONTAINER FAR* ppContainer)
{
	METHOD_PROLOGUE(COleClientItem, OleClientSite)
	ASSERT_VALID(pThis);

	// return the IOleItemContainer interface in the document
	COleDocument* pDoc = pThis->GetDocument();
	ASSERT_VALID(pDoc);
	*ppContainer = pDoc->GetContainer();

	return *ppContainer != NULL ? NOERROR : ResultFromScode(E_NOINTERFACE);
}

STDMETHODIMP COleClientItem::XOleClientSite::ShowObject()
{
	METHOD_PROLOGUE(COleClientItem, OleClientSite)
	ASSERT_VALID(pThis);

	TRY
	{
		pThis->OnShowItem();
	}
	END_TRY

	return NOERROR;
}

STDMETHODIMP COleClientItem::XOleClientSite::OnShowWindow(BOOL fShow)
{
	METHOD_PROLOGUE(COleClientItem, OleClientSite)
	ASSERT_VALID(pThis);

	// ignore this if the item is already in-place
	if (pThis->IsInPlaceActive())
		return NOERROR;

	TRY
	{
		// store new state of object -- determines how object may be drawn
		COleClientItem::ItemState uNewState;
		uNewState = fShow ? COleClientItem::openState :
			COleClientItem::loadedState;
		if (uNewState != pThis->m_nItemState)
		{
			pThis->OnChange(OLE_CHANGED_STATE, (DWORD)uNewState);
			pThis->m_nItemState = uNewState;
		}
	}
	END_TRY

	return NOERROR;
}

STDMETHODIMP COleClientItem::XOleClientSite::RequestNewObjectLayout()
{
	return ResultFromScode(E_NOTIMPL);
}

/////////////////////////////////////////////////////////////////////////////
// Implementation of IAdviseSink

STDMETHODIMP_(ULONG) COleClientItem::XAdviseSink::AddRef()
{
	METHOD_PROLOGUE(COleClientItem, AdviseSink)
	return (ULONG)pThis->ExternalAddRef();
}

STDMETHODIMP_(ULONG) COleClientItem::XAdviseSink::Release()
{
	METHOD_PROLOGUE(COleClientItem, AdviseSink)
	return (ULONG)pThis->ExternalRelease();
}

STDMETHODIMP COleClientItem::XAdviseSink::QueryInterface(
	REFIID iid, LPVOID far* ppvObj)
{
	METHOD_PROLOGUE(COleClientItem, AdviseSink)
	return (HRESULT)pThis->ExternalQueryInterface(&iid, ppvObj);
}

STDMETHODIMP_(void) COleClientItem::XAdviseSink::OnDataChange(
	LPFORMATETC lpFormatEtc, LPSTGMEDIUM lpStgMedium)
{
	METHOD_PROLOGUE(COleClientItem, AdviseSink)
	ASSERT_VALID(pThis);

	// Only interesting for advanced containers.  Forward it such that
	// containers do not have to implement the entire interface.
	pThis->OnDataChange(lpFormatEtc, lpStgMedium);
}

STDMETHODIMP_(void) COleClientItem::XAdviseSink::OnViewChange(
	DWORD aspects, LONG lindex)
{
	METHOD_PROLOGUE(COleClientItem, AdviseSink)
	ASSERT_VALID(pThis);

	pThis->OnChange(OLE_CHANGED, (DVASPECT)aspects);
}

STDMETHODIMP_(void) COleClientItem::XAdviseSink::OnRename(LPMONIKER lpMoniker)
{
	// only interesting to the OLE link object.  Containers ignore this.
}

STDMETHODIMP_(void) COleClientItem::XAdviseSink::OnSave()
{
	METHOD_PROLOGUE(COleClientItem, AdviseSink)
	ASSERT_VALID(pThis);

	pThis->OnChange(OLE_SAVED, (DVASPECT)0);
}

STDMETHODIMP_(void) COleClientItem::XAdviseSink::OnClose()
{
	METHOD_PROLOGUE(COleClientItem, AdviseSink)
	ASSERT_VALID(pThis);

	pThis->OnChange(OLE_CLOSED, (DVASPECT)0);
}

/////////////////////////////////////////////////////////////////////////////
// Diagnostics

#ifdef _DEBUG
void COleClientItem::AssertValid() const
{
	CDocItem::AssertValid();
	if (m_lpNewStorage != NULL)
		ASSERT(m_bNeedCommit);
	if (m_pView != NULL)
		m_pView->AssertValid();
	if (m_pInPlaceFrame != NULL)
		m_pInPlaceFrame->AssertValid();
	if (m_pInPlaceDoc != NULL)
		m_pInPlaceDoc->AssertValid();
}

void COleClientItem::Dump(CDumpContext& dc) const
{
	CDocItem::Dump(dc);

	// shallow dump
	AFX_DUMP1(dc, "\n\tm_lpObject = ", m_lpObject);
	AFX_DUMP1(dc, "\n\tm_dwItemNumber = ", m_dwItemNumber);
	AFX_DUMP1(dc, "\n\tm_nDrawAspect = ", (int)m_nDrawAspect);
	AFX_DUMP1(dc, "\n\tm_scLast = ", m_scLast);
	AFX_DUMP1(dc, "\n\tm_lpStorage = ", m_lpStorage);
	AFX_DUMP1(dc, "\n\tm_lpLockBytes = ", m_lpLockBytes);
	AFX_DUMP1(dc, "\n\tm_dwConnection = ", m_dwConnection);
	AFX_DUMP1(dc, "\n\tm_bLinkUnavail = ", m_bLinkUnavail);
	AFX_DUMP1(dc, "\n\tm_bMoniker = ", m_bMoniker);
	AFX_DUMP1(dc, "\n\tm_lpNewStorage = ", m_lpNewStorage);
	AFX_DUMP1(dc, "\n\tm_bNeedCommit = ", m_bNeedCommit);
	AFX_DUMP1(dc, "\n\tm_nItemState = ", (int)m_nItemState);
	AFX_DUMP1(dc, "\n\tm_pView = ", (void*)m_pView);
	AFX_DUMP1(dc, "\n\tm_dwContainerStyle = ", m_dwContainerStyle);
	AFX_DUMP1(dc, "\n\tm_pInPlaceFrame = ", (void*)m_pInPlaceFrame);
	AFX_DUMP1(dc, "\n\tm_hWndServer = ", m_hWndServer);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// Inline function declarations expanded out-of-line

#ifndef _AFX_ENABLE_INLINES

// expand inlines for OLE client APIs
static char BASED_CODE _szAfxOleInl[] = "afxole.inl";
#undef THIS_FILE
#define THIS_FILE _szAfxOleInl
#define _AFXOLECLI_INLINE
#include "afxole.inl"

#endif //!_AFX_ENABLE_INLINES

/////////////////////////////////////////////////////////////////////////////
