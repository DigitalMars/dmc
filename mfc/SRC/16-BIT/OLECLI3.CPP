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

CView* COleClientItem::pActivateView = NULL;

/////////////////////////////////////////////////////////////////////////////
// COleClientItem activation

// helper to get client site -- this is called from a number of places
LPOLECLIENTSITE COleClientItem::GetClientSite()
{
	ASSERT_VALID(this);

	LPOLECLIENTSITE lpClientSite =
		(LPOLECLIENTSITE)GetInterface(&IID_IOleClientSite);
	ASSERT(lpClientSite != NULL);
	return lpClientSite;
}

void COleClientItem::Activate(LONG nVerb, CView* pView, LPMSG lpMsg)
{
	ASSERT_VALID(this);
	ASSERT(m_lpObject != NULL);
	if (pView != NULL)
		ASSERT_VALID(pView);
	if (lpMsg != NULL)
		ASSERT(AfxIsValidAddress(lpMsg, sizeof(MSG), FALSE));

	// store the container HWND for in place activation then do the verb
	if (m_pView == NULL)
		m_pView = pView;

	CView* pViewSave = pActivateView;
	pActivateView = NULL;

	// get item rectangle for in-place players
	//  (that may not support in-place activation)
	LPCRECT lpPosRect = NULL;
	CRect rectPos;
	if (pView != NULL)
	{
		ASSERT_VALID(pView);

		rectPos.SetRectEmpty();
		OnGetItemPosition(rectPos);
		if (!rectPos.IsRectEmpty())
		{
			lpPosRect = &rectPos;
			pActivateView = pView;
		}
	}

	HRESULT hr = m_lpObject->DoVerb(nVerb, lpMsg, GetClientSite(), -1,
		pView->GetSafeHwnd(), lpPosRect);

	pActivateView = pViewSave;

	// clear out m_pView in case in-place activation only partially worked
	if (!IsInPlaceActive())
		m_pView = NULL;

	// update available status based on the results of DoVerb
	//  (this is used in the links dialog).
	m_bLinkUnavail = FAILED(hr);

	CheckGeneral(hr);
}

//////////////////////////////////////////////////////////////////////////////
// Create error handling

void COleClientItem::CheckGeneral(HRESULT hr)
	// set 'm_scLast'
	// throw exception if not ok to continue
{
	ASSERT_VALID(this);

	m_scLast = S_OK;    // assume things are ok

	// then, check for error
	if (hr != NOERROR)
	{
		m_scLast = GetScode(hr);
		if (!FAILED(m_scLast))
		{
#ifdef _DEBUG
			// warn about non-NULL success codes
			TRACE1("Warning: operation returned scode = %Fs\n",
				AfxGetFullScodeString(m_scLast));
#endif
			return;
		}
		// this error wasn't expected, so throw an exception
		AfxThrowOleException(hr);
	}
}

/////////////////////////////////////////////////////////////////////////////
// COleClientItem clipboard support

void COleClientItem::CopyToClipboard(BOOL bIncludeLink)
{
	ASSERT_VALID(this);
	ASSERT(m_lpObject != NULL);

	// get clipboard data for this item
	COleDataSource* pDataSource = OnGetClipboardData(bIncludeLink, NULL, NULL);
	TRY
	{
		// put it on the clipboard
		pDataSource->SetClipboard();
	}
	CATCH_ALL(e)
	{
		delete pDataSource;
		THROW_LAST();
	}
	END_CATCH_ALL
}

COleDataSource* COleClientItem::OnGetClipboardData(
	BOOL bIncludeLink, LPPOINT lpOffset, LPSIZE lpSize)
{
	ASSERT_VALID(this);

	COleDataSource* pDataSource = new COleDataSource;
	TRY
	{
		GetClipboardData(pDataSource, bIncludeLink, lpOffset, lpSize);
	}
	CATCH_ALL(e)
	{
		delete pDataSource;
		THROW_LAST();
	}
	END_CATCH_ALL

	ASSERT_VALID(pDataSource);
	return pDataSource;
}


DROPEFFECT COleClientItem::DoDragDrop(LPCRECT lpItemRect, CPoint ptOffset,
	BOOL bIncludeLink, DWORD dwEffects, LPCRECT lpRectStartDrag)
{
	ASSERT(AfxIsValidAddress(lpItemRect, sizeof(RECT)));
	ASSERT_VALID(this);
	ASSERT(m_lpObject != NULL);

	// get clipboard data object for the item
	CSize sizeItem(
		lpItemRect->right - lpItemRect->left,
		lpItemRect->bottom - lpItemRect->top);
	COleDataSource *pDataSource = OnGetClipboardData(
		bIncludeLink, &ptOffset, &sizeItem);

	// add DROPEFFECT_LINK only if link source is available
	LPDATAOBJECT lpDataObject = (LPDATAOBJECT)
		pDataSource->GetInterface(&IID_IDataObject);
	ASSERT(lpDataObject != NULL);
	FORMATETC formatEtc;
	formatEtc.cfFormat = _oleData.cfLinkSource;
	formatEtc.ptd = NULL;
	formatEtc.dwAspect = DVASPECT_CONTENT;
	formatEtc.lindex = -1;
	formatEtc.tymed = -1;
	if (lpDataObject->QueryGetData(&formatEtc) == NOERROR)
		dwEffects |= DROPEFFECT_LINK;

	// calculate default sensitivity rectangle
	CRect rectDrag;
	if (lpRectStartDrag == NULL)
	{
		rectDrag.SetRect(lpItemRect->left, lpItemRect->bottom,
			lpItemRect->left, lpItemRect->bottom);
		lpRectStartDrag = &rectDrag;
	}

	// do drag drop operation
	DROPEFFECT dropEffect = pDataSource->DoDragDrop(dwEffects, lpRectStartDrag);
	pDataSource->InternalRelease();
	return dropEffect;
}

void COleClientItem::GetClipboardData(COleDataSource* pDataSource,
	BOOL bIncludeLink, LPPOINT lpOffset, LPSIZE lpSize)
{
	ASSERT_VALID(this);
	ASSERT(m_lpObject != NULL);
	ASSERT_VALID(pDataSource);

	// add CF_EMBEDDEDOBJECT by creating memory storage copy of the object
	STGMEDIUM stgMedium;
	GetEmbeddedItemData(&stgMedium);
	pDataSource->CacheData(_oleData.cfEmbeddedObject, &stgMedium);

	// add CF_OBJECTDESCRIPTOR
	GetObjectDescriptorData(lpOffset, lpSize, &stgMedium);
	pDataSource->CacheData(_oleData.cfObjectDescriptor, &stgMedium);

	// add any presentation entries in the object's cache
	AddCachedData(pDataSource);

	// add CF_LINKSOURCE if supporting links to embeddings
	if (bIncludeLink && GetLinkSourceData(&stgMedium))
	{
		pDataSource->CacheData(_oleData.cfLinkSource, &stgMedium);

		// add CF_LINKSOURCEDESCRIPTOR
		GetObjectDescriptorData(lpOffset, lpSize, &stgMedium);
		pDataSource->CacheData(_oleData.cfLinkSourceDescriptor, &stgMedium);
	}
}

BOOL PASCAL COleClientItem::CanPaste()
{
	BOOL bCanPaste = FALSE;
	LPDATAOBJECT lpDataObject;
	if (::OleGetClipboard(&lpDataObject) == NOERROR)
	{
		ASSERT(lpDataObject != NULL);
		SCODE sc = GetScode(::OleQueryCreateFromData(lpDataObject));
		bCanPaste = !FAILED(sc) && sc != S_FALSE;
		lpDataObject->Release();
	}
	return bCanPaste;
}

BOOL PASCAL COleClientItem::CanPasteLink()
{
	BOOL bCanPasteLink = FALSE;
	LPDATAOBJECT lpDataObject;
	if (::OleGetClipboard(&lpDataObject) == NOERROR)
	{
		ASSERT(lpDataObject != NULL);
		SCODE sc = GetScode(::OleQueryLinkFromData(lpDataObject));
		bCanPasteLink = !FAILED(sc) && sc != S_FALSE;
		lpDataObject->Release();
	}
	return bCanPasteLink;
}

BOOL PASCAL
COleClientItem::CanCreateFromData(const COleDataObject* pDataObject)
{
	ASSERT(pDataObject != NULL);
	SCODE sc = GetScode(::OleQueryCreateFromData(pDataObject->m_lpDataObject));
	return !FAILED(sc) && sc != S_FALSE;
}

BOOL PASCAL
COleClientItem::CanCreateLinkFromData(const COleDataObject* pDataObject)
{
	ASSERT(pDataObject != NULL);
	SCODE sc = GetScode(::OleQueryLinkFromData(pDataObject->m_lpDataObject));
	return !FAILED(sc) && sc != S_FALSE;
}

/////////////////////////////////////////////////////////////////////////////
// Conversion & Activate As support

BOOL COleClientItem::ConvertTo(REFCLSID clsidNew)
{
	ASSERT_VALID(this);
	ASSERT(m_lpObject != NULL);

	// first, close the object
	Close();

	// release any pointers we have to the object
	VERIFY(m_lpObject->Release() == 0);
	m_lpObject = NULL;

	// convert it
	m_scLast = GetScode(::OleStdDoConvert(m_lpStorage, clsidNew));
	if (FAILED(m_scLast))
		return FALSE;

	// then load the new object from the new storage
	BOOL bResult = FinishCreate(::OleLoad(m_lpStorage, IID_IOleObject,
		NULL, (LPLP)&m_lpObject));

	ASSERT_VALID(this);
	return bResult;
}

BOOL COleClientItem::Reload()
{
	// first, close the object
	Close();

	// release any pointers we have to the object
	VERIFY(m_lpObject->Release() == 0);
	m_lpObject = NULL;

	// then reload the object with OleLoad and finish creation process
	BOOL bResult = FinishCreate(::OleLoad(m_lpStorage, IID_IOleObject,
		NULL, (LPLP)&m_lpObject));

	ASSERT_VALID(this);
	return bResult;
}

BOOL COleClientItem::ActivateAs(LPCSTR lpszUserType,
	REFCLSID clsidOld, REFCLSID clsidNew)
{
	ASSERT_VALID(this);
	ASSERT(lpszUserType == NULL || AfxIsValidString(lpszUserType));
	ASSERT(m_lpObject != NULL);

	// enable activate as
	m_scLast = GetScode(::OleStdDoTreatAsClass(
		(LPSTR)(LPCSTR)lpszUserType, clsidOld, clsidNew));
	if (FAILED(m_scLast))
		return FALSE;

	// reload all items in this doucment
	COleDocument* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	POSITION pos = pDoc->GetStartPosition();
	COleClientItem* pItem;
	while ((pItem = pDoc->GetNextClientItem(pos)) != NULL)
	{
		// reload it, so activate as works as appropriate
		pItem->Reload();
	}

	ASSERT_VALID(this);
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// COleClientItem printing support

BOOL COleClientItem::SetPrintDevice(const DVTARGETDEVICE FAR* ptd)
{
	ASSERT(ptd == NULL ||
		AfxIsValidAddress(ptd, sizeof(DVTARGETDEVICE), FALSE));

	// get printer device information from cache
	LPOLECACHE lpOleCache;
	DVTARGETDEVICE FAR* ptdCur;
	DWORD dwConnection;
	if (!GetPrintDeviceInfo(&lpOleCache, &ptdCur, &dwConnection))
		return FALSE;   // no print device info available
	ASSERT(lpOleCache != NULL);

	// both may have no target device (considered equal)
	if (ptd == NULL && ptdCur == NULL)
	{
		lpOleCache->Release();
		::OleStdDeleteTargetDevice(ptdCur);
		return TRUE;
	}

	if (ptd != NULL && ptdCur != NULL)
	{
		// should be non-NULL and valid addresses
		ASSERT(AfxIsValidAddress(ptd, (size_t)ptd->tdSize));
		ASSERT(AfxIsValidAddress(ptdCur, (size_t)ptdCur->tdSize));
		// see if they compare equal
		if (ptdCur->tdSize == ptd->tdSize &&
			_fmemcmp(ptdCur, ptd, (size_t)ptd->tdSize) == 0)
		{
			lpOleCache->Release();
			::OleStdDeleteTargetDevice(ptdCur);
			return TRUE;
		}
	}

	// calling this with NULL will just remove the prevous printer cache
	if (ptd != NULL)
	{
		// new cache is for CF_METAFILEPICT, DVASPECT_CONTENT
		FORMATETC formatEtc;
		formatEtc.cfFormat = CF_METAFILEPICT;
		formatEtc.ptd = (DVTARGETDEVICE FAR*)ptd;
		formatEtc.dwAspect = DVASPECT_CONTENT;
		formatEtc.lindex = -1;
		formatEtc.tymed = TYMED_MFPICT;

		// attempt to cache new format
		DWORD dwNewConnection;
		if (lpOleCache->Cache(&formatEtc, ADVFCACHE_ONSAVE,
			&dwNewConnection) != NOERROR)
		{
			lpOleCache->Release();
			::OleStdDeleteTargetDevice(ptdCur);
			return FALSE;
		}
	}
	// new format is cached successfully, uncache old format
	lpOleCache->Uncache(dwConnection);

	// cleanup & return
	lpOleCache->Release();
	::OleStdDeleteTargetDevice(ptdCur);
	return TRUE;
}

BOOL COleClientItem::SetPrintDevice(const PRINTDLG* ppd)
{
	ASSERT(ppd == NULL || AfxIsValidAddress(ppd, sizeof(*ppd), FALSE));
	DVTARGETDEVICE FAR* ptd = NULL;
	if (ppd != NULL)
		ptd = ::OleStdCreateTargetDevice((PRINTDLG*)ppd);

	BOOL bResult = SetPrintDevice(ptd);
	::OleStdDeleteTargetDevice(ptd);
	return bResult;
}

/////////////////////////////////////////////////////////////////////////////
// other advanced COleClientItem support

void COleClientItem::GetUserType(
	USERCLASSTYPE nUserClassType, CString& rString)
{
	ASSERT_VALID(this);
	ASSERT(m_lpObject != NULL);

	LPSTR lpszUserType;
	CheckGeneral(m_lpObject->GetUserType(nUserClassType, &lpszUserType));
	ASSERT(lpszUserType != NULL);
	ASSERT(AfxIsValidString(lpszUserType));
	rString = lpszUserType;
	_AfxFreeTaskMem(lpszUserType);
}

void COleClientItem::Run()
{
	ASSERT_VALID(this);
	ASSERT(m_lpObject != NULL);

	// is object already in running state?
	if (::OleIsRunning(m_lpObject))
		return;

	// run the object -- throw exception on errors
	HRESULT hr = ::OleRun(m_lpObject);
	CheckGeneral(hr);

	// should be running now
	ASSERT(::OleIsRunning(m_lpObject));
}

/////////////////////////////////////////////////////////////////////////////
// Linked COleClientItem operations

BOOL COleClientItem::UpdateLink()
{
	ASSERT_VALID(this);
	ASSERT(m_lpObject != NULL);

	m_scLast = GetScode(m_lpObject->Update());
	ASSERT_VALID(m_pDocument);
	m_pDocument->SetModifiedFlag();

	return m_scLast == S_OK;
}

BOOL COleClientItem::FreezeLink()
{
	ASSERT_VALID(this);
	ASSERT(m_lpObject != NULL);
	ASSERT(m_pDocument != NULL);
	ASSERT(GetType() == OT_LINK);

	// first save & close the item
	Close();

	// get IDataObject interface
	LPDATAOBJECT lpDataObject = _AFXQUERYINTERFACE(m_lpObject, IDataObject);
	ASSERT(lpDataObject != NULL);
	COleDataObject dataObject;
	dataObject.Attach(lpDataObject, TRUE);

	// save important state of original item
	LPOLEOBJECT lpObject = m_lpObject;
	LPSTORAGE lpStorage = m_lpStorage;
	LPLOCKBYTES lpLockBytes = m_lpLockBytes;
	DWORD dwConnection = m_dwConnection;
	DWORD dwItemNumber = m_dwItemNumber;
	m_lpObject = NULL;
	m_lpStorage = NULL;
	m_lpLockBytes = NULL;
	m_dwConnection = 0;

	// attempt to create new object from data
	if (!CreateStaticFromData(&dataObject))
	{
		m_lpObject = lpObject;
		m_lpStorage = lpStorage;
		m_lpLockBytes = lpLockBytes;
		m_dwConnection = dwConnection;
		return FALSE;
	}
#ifdef _DEBUG
	UpdateItemType();
	ASSERT(GetType() == OT_STATIC);
#endif

	// save new state of that item
	LPOLEOBJECT lpNewObject = m_lpObject;
	LPSTORAGE lpNewStorage = m_lpStorage;
	LPLOCKBYTES lpNewLockBytes = m_lpLockBytes;
	DWORD dwNewConnection = m_dwConnection;
	DWORD dwNewItemNumber = m_dwItemNumber;

	// shut down old item
	m_lpObject = lpObject;
	m_lpStorage = lpStorage;
	m_lpLockBytes = lpLockBytes;
	m_dwConnection = dwConnection;
	m_dwItemNumber = dwItemNumber;
#ifdef _DEBUG
	UpdateItemType();
	ASSERT(GetType() == OT_LINK);
#endif
	Delete(FALSE);  // revokes item & removes storage

	// switch to new item
	m_lpObject = lpNewObject;
	m_lpStorage = lpNewStorage;
	m_lpLockBytes = lpNewLockBytes;
	m_dwConnection = dwNewConnection;
	m_dwItemNumber = dwNewItemNumber;
	UpdateItemType();
	ASSERT(GetType() == OT_STATIC);

	// send an on changed with same state to invalidate the item
	OnChange(OLE_CHANGED_STATE, (DWORD)GetItemState());
	ASSERT_VALID(m_pDocument);
	m_pDocument->SetModifiedFlag();

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// Special link attributes

OLEUPDATE COleClientItem::GetLinkUpdateOptions()
{
	ASSERT_VALID(this);
	ASSERT(m_lpObject != NULL);

	LPOLELINK lpOleLink = _AFXQUERYINTERFACE(m_lpObject, IOleLink);
	ASSERT(lpOleLink != NULL);  // perhaps not a link?

	DWORD dwUpdateOpt;
	HRESULT hr = lpOleLink->GetUpdateOptions(&dwUpdateOpt);
	lpOleLink->Release();
	CheckGeneral(hr);   // may throw an exception

	return (OLEUPDATE)dwUpdateOpt;
}

void COleClientItem::SetLinkUpdateOptions(OLEUPDATE dwUpdateOpt)
{
	ASSERT_VALID(this);
	ASSERT(m_lpObject != NULL);

	LPOLELINK lpOleLink = _AFXQUERYINTERFACE(m_lpObject, IOleLink);
	ASSERT(lpOleLink != NULL);  // perhaps not a link?

	HRESULT hr = lpOleLink->SetUpdateOptions(dwUpdateOpt);
	lpOleLink->Release();
	CheckGeneral(hr);
}

/////////////////////////////////////////////////////////////////////////////
