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

#define OLE_MAXNAMESIZE     (256)

/////////////////////////////////////////////////////////////////////////////
// COleLinkingDoc - enables linking to embeddings (basis for server)

COleLinkingDoc::COleLinkingDoc()
{
	m_dwRegister = 0;
	m_pFactory = NULL;
	m_bVisibleLock = FALSE;

	ASSERT_VALID(this);
}

COleLinkingDoc::~COleLinkingDoc()
{
	ASSERT_VALID(this);

	ASSERT(!m_bVisibleLock);

	DisconnectViews();
	ASSERT(m_viewList.IsEmpty());

	Revoke();   // cleanup naming support
	ExternalDisconnect();
}

/////////////////////////////////////////////////////////////////////////////
// COleLinkingDoc moniker handling

BOOL COleLinkingDoc::Register(
	COleObjectFactory* pFactory, const char* pszPathName)
{
	ASSERT_VALID(this);
	ASSERT(pFactory == NULL ||
		AfxIsValidAddress(pFactory, sizeof(COleObjectFactory)));
	ASSERT(pszPathName == NULL || AfxIsValidString(pszPathName));
	ASSERT(m_dwRegister == 0);

	BOOL bResult = TRUE;

	// create file moniker based on path name
	LPMONIKER lpMoniker = NULL;
	if (pszPathName != NULL)
	{
		if (::CreateFileMoniker((LPSTR)(LPCSTR)pszPathName, &lpMoniker) != NOERROR)
			bResult = FALSE;
	}

	// register file moniker as running
	if (lpMoniker != NULL)
	{
		LPRUNNINGOBJECTTABLE lpROT = NULL;
		VERIFY(GetRunningObjectTable(0, &lpROT) == NOERROR);
		ASSERT(lpROT != NULL);
		HRESULT hr = lpROT->Register(NULL, (LPUNKNOWN)
			GetInterface(&IID_IUnknown), lpMoniker, &m_dwRegister);
		lpROT->Release();
		if (hr != NOERROR)
		{
			_AfxRelease((LPUNKNOWN*)&lpMoniker);
			bResult = FALSE;
		}
	}

	// update all objects with new moniker
	POSITION pos = GetStartPosition();
	COleClientItem* pItem;
	while ((pItem = GetNextClientItem(pos)) != NULL)
	{
		if (pItem->m_bMoniker)
		{
			ASSERT(pItem->m_lpObject != NULL);
			pItem->m_lpObject->SetMoniker(OLEWHICHMK_CONTAINER, lpMoniker);
		}
	}

	// release new moniker
	_AfxRelease((LPUNKNOWN*)&lpMoniker);

	// attach the document to the server
	ASSERT(m_pFactory == NULL || m_pFactory == pFactory);
	m_pFactory = pFactory;
	return bResult;
}

void COleLinkingDoc::Revoke()
{
	ASSERT_VALID(this);

	// revoke current registration
	if (m_dwRegister != 0)
	{
		LPRUNNINGOBJECTTABLE lpROT = NULL;
		GetRunningObjectTable(0, &lpROT);
		if (lpROT != NULL)
		{
			lpROT->Revoke(m_dwRegister);
			lpROT->Release();
		}
		m_dwRegister = 0;
	}
}

BOOL COleLinkingDoc::OnNewDocument()
{
	ASSERT_VALID(this);

	if (!COleDocument::OnNewDocument())
		return FALSE;

	Revoke();
	RegisterIfServerAttached(NULL);

	AfxOleSetUserCtrl(TRUE);

	return TRUE;
}

BOOL COleLinkingDoc::OnOpenDocument(const char* pszPathName)
{
	ASSERT_VALID(this);

	if (!COleDocument::OnOpenDocument(pszPathName))
		return FALSE;

	Revoke();
	RegisterIfServerAttached(pszPathName);

	AfxOleSetUserCtrl(TRUE);

	return TRUE;
}

BOOL COleLinkingDoc::OnSaveDocument(const char* pszPathName)
{
	ASSERT_VALID(this);

	BOOL bRemember = m_bRemember;
	if (!COleDocument::OnSaveDocument(pszPathName))
		return FALSE;

	if (bRemember)
	{
		// update the moniker since the name has changed
		Revoke();
		RegisterIfServerAttached(pszPathName);
	}
	return TRUE;
}

void COleLinkingDoc::OnCloseDocument()
{
	InternalAddRef();   // protect document during shutdown

	Revoke();   // cleanup naming support

	// remove visible lock if present
	if (m_bVisibleLock)
	{
		m_bVisibleLock = FALSE;
		LockExternal(FALSE, FALSE);
	}

	// cleanup the document but don't delete yet
	BOOL bAutoDelete = m_bAutoDelete;
	m_bAutoDelete = FALSE;
	COleDocument::OnCloseDocument();
	ASSERT_VALID(this);

	// remove extra reference count and destroy
	--m_dwRef;
	if (bAutoDelete)
		delete this;    // now safe to destroy document
}

void COleLinkingDoc::UpdateVisibleLock(BOOL bVisible, BOOL bRemoveRefs)
{
	ASSERT_VALID(this);

	if (bVisible != m_bVisibleLock)
	{
		InternalAddRef();   // make sure document is stable
		m_bVisibleLock = bVisible;
		LockExternal(bVisible, bRemoveRefs);
		InternalRelease();  // may Release the document!
	}
}

void COleLinkingDoc::OnShowViews(BOOL bVisible)
{
	UpdateVisibleLock(bVisible, TRUE);
}

void COleLinkingDoc::SaveToStorage(CObject* pObject)
{
	ASSERT_VALID(this);
	if (pObject != NULL)
		ASSERT_VALID(pObject);

	// write the classID of the application to the root storage
	if (m_pFactory != NULL)
	{
		ASSERT(m_lpRootStg != NULL);
		WriteClassStg(m_lpRootStg, m_pFactory->GetClassID());
	}
	COleDocument::SaveToStorage(pObject);
}

void COleLinkingDoc::RegisterIfServerAttached(const char* pszPathName)
{
	ASSERT_VALID(this);
	ASSERT(pszPathName == NULL || AfxIsValidString(pszPathName));

	CDocTemplate* pTemplate = GetDocTemplate();
	ASSERT_VALID(pTemplate);

	COleObjectFactory* pFactory =
		(COleObjectFactory*)pTemplate->m_pAttachedFactory;
	if (pFactory != NULL)
	{
		// always attach the document to the server at this time
		ASSERT(pFactory->IsKindOf(RUNTIME_CLASS(COleObjectFactory)));
		m_pFactory = pFactory;

		// register with OLE Server
		if (!Register(pFactory, pszPathName))
		{
			// not fatal
			AfxMessageBox(AFX_IDP_FAILED_TO_NOTIFY);
		}
	}
}

LPOLEITEMCONTAINER COleLinkingDoc::GetContainer()
{
	ASSERT_VALID(this);

	// get the IOleItemContainer interface via QueryInterface
	LPOLEITEMCONTAINER lpContainer;
	InternalQueryInterface(&IID_IOleItemContainer, (LPLP)&lpContainer);
	return lpContainer;
}

/////////////////////////////////////////////////////////////////////////////
// COleLinkingDoc default implementation

COleServerItem* COleLinkingDoc::OnGetLinkedItem(LPCSTR lpszItemName)
{
	ASSERT_VALID(this);

	// default implementation is in COleServerDoc
	return NULL;
}

COleClientItem* COleLinkingDoc::OnFindEmbeddedItem(LPCSTR lpszItemName)
{
	ASSERT_VALID(this);
	ASSERT(AfxIsValidString(lpszItemName));

	// default implementation walks list of client items looking for
	//  a case sensitive match

	POSITION pos = GetStartPosition();
	COleClientItem* pItem;
	while ((pItem = GetNextClientItem(pos)) != NULL)
	{
		// a client item is running if there is a match in name
		//  and the m_lpObject is also running.
		char szItemName[OLE_MAXITEMNAME];
		pItem->GetItemName(szItemName);
		if (lstrcmp(szItemName, lpszItemName) == 0)
			return pItem;
	}
	OLE_TRACE1("Warning: default COleLinkingDoc::OnFindEmbeddedItem "
		"implementation failed to find item '%Fs'\n", lpszItemName);
	return NULL;    // no matching item found
}

void COleLinkingDoc::LockExternal(BOOL bLock, BOOL bRemoveRefs)
{
	// when an item binding is successful, the original document
	//  is released.  To keep it alive and the RPC stubs that make
	//  it available to the external world (via the running object
	//  table), we need to place a lock on it.

	// a lock created with CoLockObjectExternal adds a reference
	//  to the object itself (with IUnknown::AddRef) as well
	//  as keeping the RPC stub alive.

	::CoLockObjectExternal((LPUNKNOWN)GetInterface(&IID_IUnknown),
		bLock, bRemoveRefs);

	if (bLock)
	{
		// avoid "dead" objects in the running object table, by
		//  reregistering this object in the running object table.
		if (!m_strPathName.IsEmpty())
		{
			Revoke();
			RegisterIfServerAttached(m_strPathName);
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
// COleLinkingDoc OLE interface implementation

BEGIN_INTERFACE_MAP(COleLinkingDoc, COleDocument)
	INTERFACE_PART(COleLinkingDoc, IID_IPersist, PersistFile)
	INTERFACE_PART(COleLinkingDoc, IID_IPersistFile, PersistFile)
	INTERFACE_PART(COleLinkingDoc, IID_IParseDisplayName, OleItemContainer)
	INTERFACE_PART(COleLinkingDoc, IID_IOleContainer, OleItemContainer)
	INTERFACE_PART(COleLinkingDoc, IID_IOleItemContainer, OleItemContainer)
END_INTERFACE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COleLinkingDoc::XPersistFile implementation

STDMETHODIMP_(ULONG) COleLinkingDoc::XPersistFile::AddRef()
{
	METHOD_PROLOGUE(COleLinkingDoc, PersistFile)
	return (ULONG)pThis->ExternalAddRef();
}

STDMETHODIMP_(ULONG) COleLinkingDoc::XPersistFile::Release()
{
	METHOD_PROLOGUE(COleLinkingDoc, PersistFile)
	return (ULONG)pThis->ExternalRelease();
}

STDMETHODIMP COleLinkingDoc::XPersistFile::QueryInterface(
	REFIID iid, LPVOID far* ppvObj)
{
	METHOD_PROLOGUE(COleLinkingDoc, PersistFile)
	return (HRESULT)pThis->ExternalQueryInterface(&iid, ppvObj);
}

STDMETHODIMP COleLinkingDoc::XPersistFile::GetClassID(LPCLSID lpClassID)
{
	METHOD_PROLOGUE(COleLinkingDoc, PersistFile)
	ASSERT_VALID(pThis);

	// this is sometimes called for documents not attached to servers!
	if (pThis->m_pFactory == NULL)
	{
		*lpClassID = CLSID_NULL;
		return ResultFromScode(E_FAIL);
	}

	// get the class ID from the connected server object
	ASSERT_VALID(pThis->m_pFactory);
	*lpClassID = pThis->m_pFactory->GetClassID();
	return NOERROR;
}

STDMETHODIMP COleLinkingDoc::XPersistFile::IsDirty()
{
	METHOD_PROLOGUE(COleLinkingDoc, PersistFile)
	ASSERT_VALID(pThis);

	// return NOERROR (S_OK) if IsModified, and S_FALSE otherwise
	return pThis->IsModified() ? NOERROR : ResultFromScode(S_FALSE);
}

STDMETHODIMP COleLinkingDoc::XPersistFile::Load(
	LPCSTR lpszFileName, DWORD dwMode)
{
	METHOD_PROLOGUE(COleLinkingDoc, PersistFile)
	ASSERT_VALID(pThis);

	CString strFileName;
	SCODE sc = E_FAIL;
	TRY
	{
		BOOL bUserCtrl = AfxOleGetUserCtrl();

		// delegate to file-based Open implementation
		strFileName = lpszFileName;
		strFileName.MakeUpper();
		if (!pThis->OnOpenDocument(strFileName))
		{
			AfxOleSetUserCtrl(bUserCtrl);
			return ResultFromScode(S_FALSE);
		}
		pThis->SendInitialUpdate();

		// set the path name, but don't add to MRU list
		pThis->SetPathName(strFileName, FALSE);
		AfxOleSetUserCtrl(bUserCtrl);

		sc = S_OK;
	}
	END_TRY

	return ResultFromScode(sc);
}

STDMETHODIMP COleLinkingDoc::XPersistFile::Save(
	LPCSTR lpszFileName, BOOL fRemember)
{
	METHOD_PROLOGUE(COleLinkingDoc, PersistFile)
	ASSERT_VALID(pThis);

	CString strFileName;
	SCODE sc = E_FAIL;
	TRY
	{
		// delegate to file-based Save/Save As implementation
		ASSERT(pThis->m_bRemember == TRUE);
		pThis->m_bRemember = fRemember;
		strFileName = lpszFileName;
		strFileName.MakeUpper();
		pThis->OnSaveDocument(strFileName);
		sc = S_OK;
	}
	END_TRY

	return ResultFromScode(sc);
}

STDMETHODIMP COleLinkingDoc::XPersistFile::SaveCompleted(LPCSTR lpszFileName)
{
	METHOD_PROLOGUE(COleLinkingDoc, PersistFile)
	ASSERT_VALID(pThis);

	TRY
	{
		// set the path name, but don't add to MRU list
		CString strFileName = lpszFileName;
		strFileName.MakeUpper();
		pThis->SetPathName(strFileName, FALSE);
	}
	END_TRY

	return NOERROR;
}

STDMETHODIMP COleLinkingDoc::XPersistFile::GetCurFile(LPSTR FAR* lplpszFileName)
{
	METHOD_PROLOGUE(COleLinkingDoc, PersistFile)
	ASSERT_VALID(pThis);

	*lplpszFileName = NULL;

	// use title if no document
	LPCSTR lpszResult;
	if (pThis->m_strPathName.IsEmpty())
		lpszResult = pThis->m_strTitle;
	else
		lpszResult = pThis->m_strPathName;
	ASSERT(lpszResult != NULL);

	// allocate memory for the file name
	int nLen = lstrlen(lpszResult)+1;
	LPSTR lpszPathName = (LPSTR)_AfxAllocTaskMem(nLen);
	if (lpszPathName == NULL)
		return ResultFromScode(E_OUTOFMEMORY);

	// copy the file name and return it
	_fmemcpy(lpszPathName, lpszResult, nLen);
	*lplpszFileName = lpszPathName;
	return NOERROR;
}

/////////////////////////////////////////////////////////////////////////////
// Implementation of IOleItemContainer
//  (supports linking to embeddings and linking to pseudo-objects)

STDMETHODIMP_(ULONG) COleLinkingDoc::XOleItemContainer::AddRef()
{
	METHOD_PROLOGUE(COleLinkingDoc, OleItemContainer)
	return (ULONG)pThis->ExternalAddRef();
}

STDMETHODIMP_(ULONG) COleLinkingDoc::XOleItemContainer::Release()
{
	METHOD_PROLOGUE(COleLinkingDoc, OleItemContainer)
	return (ULONG)pThis->ExternalRelease();
}

STDMETHODIMP COleLinkingDoc::XOleItemContainer::QueryInterface(
	REFIID iid, LPVOID far* ppvObj)
{
	METHOD_PROLOGUE(COleLinkingDoc, OleItemContainer)
	return (HRESULT)pThis->ExternalQueryInterface(&iid, ppvObj);
}

STDMETHODIMP COleLinkingDoc::XOleItemContainer::EnumObjects(
	DWORD grfFlags, LPENUMUNKNOWN FAR* ppEnumUnknown)
{
	return ResultFromScode(E_NOTIMPL);
}

STDMETHODIMP COleLinkingDoc::XOleItemContainer::ParseDisplayName(LPBC lpbc,
	LPSTR lpszDisplayName, ULONG FAR* cchEaten, LPMONIKER FAR* ppMoniker)
{
	METHOD_PROLOGUE(COleLinkingDoc, OleItemContainer)
	ASSERT_VALID(pThis);

	// reset all OUT parameters
	*ppMoniker = NULL;

	char szItemName[OLE_MAXNAMESIZE];
	char *pszDest = szItemName;
	LPCSTR lpszSrc = lpszDisplayName;

	// skip leading delimiters
	int cEaten = 0;
	while (*lpszSrc != '\0' && (*lpszSrc == '\\' || *lpszSrc == '/' ||
		*lpszSrc == ':' || *lpszSrc == '!' || *lpszSrc == '['))
	{
		if (_AfxIsDBCSLeadByte(*lpszSrc))
			++lpszSrc, ++cEaten;
		++lpszSrc;
		++cEaten;
	}

	// parse next token int szItemName
	while (*lpszSrc != '\0' && *lpszSrc != '\\' && *lpszSrc != '/' &&
		*lpszSrc != ':' && *lpszSrc != '!' && *lpszSrc != '[' &&
		cEaten < OLE_MAXNAMESIZE-1)
	{
		if (_AfxIsDBCSLeadByte(*lpszSrc))
			*pszDest++ = *lpszSrc++, ++cEaten;
		*pszDest++ = *lpszSrc++;
		++cEaten;
	}
	*cchEaten = cEaten;
	*pszDest = 0;

	// attempt to get the object
	LPUNKNOWN lpUnknown;
	HRESULT hr = GetObject(szItemName, BINDSPEED_INDEFINITE, lpbc,
		IID_IUnknown, (LPLP)&lpUnknown);
	if (hr != NOERROR)
		return hr;

	// item name found -- create item moniker for it
	lpUnknown->Release();
	return ::CreateItemMoniker(OLESTDDELIM, szItemName, ppMoniker);
}

STDMETHODIMP COleLinkingDoc::XOleItemContainer::LockContainer(BOOL fLock)
{
	METHOD_PROLOGUE(COleLinkingDoc, OleItemContainer)
	ASSERT_VALID(pThis);

	pThis->LockExternal(fLock, TRUE);
	return NOERROR;
}

STDMETHODIMP COleLinkingDoc::XOleItemContainer::GetObject(
	LPSTR lpszItem, DWORD dwSpeedNeeded, LPBINDCTX pbc, REFIID riid,
	LPVOID FAR* ppvObject)
{
	METHOD_PROLOGUE(COleLinkingDoc, OleItemContainer)
	ASSERT_VALID(pThis);

	*ppvObject = NULL;

	HRESULT hr = ResultFromScode(MK_E_NOOBJECT);
	TRY
	{
		// check for link to embedding
		COleClientItem* pClientItem = pThis->OnFindEmbeddedItem(lpszItem);
		if (pClientItem != NULL)
		{
			ASSERT_VALID(pClientItem);
			hr = NOERROR;

			// item found -- make sure it is running
			if (!::OleIsRunning(pClientItem->m_lpObject))
			{
				// should not run the object if bind-speed is immediate
				if (dwSpeedNeeded != BINDSPEED_INDEFINITE)
					hr = ResultFromScode(MK_E_EXCEEDEDDEADLINE);
				else
				{
					// bind speed is not immediate -- so run the object
					hr = OleRun(pClientItem->m_lpObject);
				}
			}

			if (hr == NOERROR)
			{
				// return the object with appropriate interface
				hr = pClientItem->m_lpObject->QueryInterface(riid, ppvObject);
			}
		}
		else
		{
			// check for link to pseudo object
			COleServerItem* pServerItem = pThis->OnGetLinkedItem(lpszItem);
			if (pServerItem != NULL)
			{
				if (!pServerItem->m_bNeedUnlock)
				{
					// when a link is bound, the document must be kept alive
					pThis->LockExternal(TRUE, FALSE);
					pServerItem->m_bNeedUnlock = TRUE;
				}

				// matching item found -- query for the requested interface
				hr = (HRESULT)pServerItem->ExternalQueryInterface(&riid,
					ppvObject);
			}
		}
	}
	END_TRY

	return hr;
}

STDMETHODIMP COleLinkingDoc::XOleItemContainer::GetObjectStorage(
	LPSTR lpszItem, LPBINDCTX pbc, REFIID riid, LPVOID FAR* ppvStorage)
{
	METHOD_PROLOGUE(COleLinkingDoc, OleItemContainer)
	ASSERT_VALID(pThis);

	*ppvStorage = NULL;

	// only IStorage is supported
	if (riid != IID_IStorage)
		return ResultFromScode(E_UNEXPECTED);

	// check for link to embedding
	COleClientItem* pClientItem = pThis->OnFindEmbeddedItem(lpszItem);
	if (pClientItem != NULL)
	{
		ASSERT_VALID(pClientItem);

		// if object has no storage, can't return it!
		if (pClientItem->m_lpStorage != NULL)
		{
			// found matching item -- return the storage
			*ppvStorage = pClientItem->m_lpStorage;
			pClientItem->m_lpStorage->AddRef();
			return NOERROR;
		}
	}
	return ResultFromScode(MK_E_NOSTORAGE);
}

STDMETHODIMP COleLinkingDoc::XOleItemContainer::IsRunning(LPSTR lpszItem)
{
	METHOD_PROLOGUE(COleLinkingDoc, OleItemContainer)
	ASSERT_VALID(pThis);

	// check for link to embedding
	COleClientItem* pClientItem = pThis->OnFindEmbeddedItem(lpszItem);
	if (pClientItem != NULL)
	{
		ASSERT_VALID(pClientItem);
		if (!::OleIsRunning(pClientItem->m_lpObject))
			return ResultFromScode(S_FALSE);

		return NOERROR; // item is in document and is running
	}

	// check for link to pseudo object
	SCODE sc = MK_E_NOOBJECT;
	TRY
	{
		COleServerItem* pServerItem = pThis->OnGetLinkedItem(lpszItem);
		if (pServerItem != NULL)
			sc = S_OK;
	}
	END_TRY

	return ResultFromScode(sc);
}

/////////////////////////////////////////////////////////////////////////////
// COleLinkingDoc diagnostics

#ifdef _DEBUG
void COleLinkingDoc::AssertValid() const
{
	COleDocument::AssertValid();
	if (m_pFactory != NULL)
		m_pFactory->AssertValid();
}

void COleLinkingDoc::Dump(CDumpContext& dc) const
{
	COleDocument::Dump(dc);
	AFX_DUMP1(dc, "\nm_dwRegister = ", m_dwRegister);
	AFX_DUMP1(dc, "\nm_bVisibleLock = ", m_bVisibleLock);
	if (m_pFactory != NULL)
		AFX_DUMP1(dc, "\nwith factory ", m_pFactory);
	else
		AFX_DUMP0(dc, "\nwith no factory");
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
