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

IMPLEMENT_SERIAL(CDocItem, CCmdTarget, 0)

#define new DEBUG_NEW

// These IMPLEMENT_DYNAMICs here for .OBJ granularity reasons.
IMPLEMENT_DYNAMIC(COleClientItem, CDocItem)
IMPLEMENT_DYNAMIC(COleServerItem, CDocItem)

/////////////////////////////////////////////////////////////////////////////
// COleDocument - enables both server and client

IMPLEMENT_DYNAMIC(COleDocument, CDocument)

COleDocument::COleDocument()
{
	ASSERT(m_viewList.IsEmpty());
	ASSERT(m_docItemList.IsEmpty());

#ifdef _DEBUG
	// check for common mistake of not initializing OLE libraries before
	//  creating an OLE document.
	LPMALLOC lpMalloc = NULL;
	if (::CoGetMalloc(MEMCTX_TASK, &lpMalloc) != NOERROR)
	{
		TRACE0("Warning: CoGetMalloc(MEMCTX_TASK, ...) failed,\n"
			"\tperhaps AfxOleInit() has not been called.\n");
	}
	_AfxRelease((LPUNKNOWN*)&lpMalloc);
#endif

	m_dwNextItemNumber = 1; // item number for first item in document
	m_bRemember = TRUE;
	m_bSameAsLoad = TRUE;
	m_lpRootStg = NULL;
	m_ptd = NULL;       // default to screen target device
	m_bLastVisible = FALSE;
	m_bCompoundFile = FALSE;

	AfxOleLockApp();
}

COleDocument::~COleDocument()
{
	ASSERT_VALID(this);

#ifdef _DEBUG
	if (!m_docItemList.IsEmpty())
		TRACE1("Warning: destroying COleDocument with %d doc items\n",
			m_docItemList.GetCount());
#endif

	// remove all doc-items from the list before shutting down the storage
	POSITION pos = GetStartPosition();
	while (pos != NULL)
	{
		CDocItem* pItem = GetNextItem(pos);
		ASSERT(pItem != NULL);
		delete pItem;
	}

	// release the hold on the document storage
	_AfxRelease((LPUNKNOWN*)&m_lpRootStg);
	OleStdDeleteTargetDevice(m_ptd);

	AfxOleUnlockApp();
}

/////////////////////////////////////////////////////////////////////////////
// DocItem management

void COleDocument::AddItem(CDocItem* pItem)
{
	// don't do an ASSERT_VALID until after we've added it !
	ASSERT(pItem->IsKindOf(RUNTIME_CLASS(CDocItem)));

	ASSERT(pItem->m_pDocument == NULL);     // not yet initialized
	m_docItemList.AddTail(pItem);
	pItem->m_pDocument = this;

	ASSERT_VALID(pItem);    // now it must be valid
}

void COleDocument::RemoveItem(CDocItem* pItem)
{
	ASSERT_VALID(pItem);    // must be valid before detach
	ASSERT(pItem->IsKindOf(RUNTIME_CLASS(CDocItem)));
	ASSERT(pItem->m_pDocument == this);     // formerly attached

	ASSERT(m_docItemList.Find(pItem) != NULL);  // must be in list
	m_docItemList.RemoveAt(m_docItemList.Find(pItem));
	ASSERT(m_docItemList.Find(pItem) == NULL);  // must not be in list now
	pItem->m_pDocument = NULL;
}

POSITION COleDocument::GetStartPosition() const
{
	ASSERT_VALID(this);
	return m_docItemList.GetHeadPosition();
}

CDocItem* COleDocument::GetNextItem(POSITION& pos) const
{
	// handle special case of !pos -- makes enumeration code smaller
	if (pos == NULL)
		return NULL;

	// otherwise get next item from list
	ASSERT_VALID(this);
	CDocItem* pItem = (CDocItem*)m_docItemList.GetNext(pos);
	ASSERT(pItem != NULL);
	ASSERT(pItem->IsKindOf(RUNTIME_CLASS(CDocItem)));
	ASSERT(pItem->m_pDocument == this);     // must be ours
	return pItem;
}

CDocItem*
COleDocument::GetNextItemOfKind(POSITION& pos, CRuntimeClass* pClass) const
{
	while (pos != NULL)
	{
		CDocItem* pItem = GetNextItem(pos);
		ASSERT_VALID(pItem);
		if (pItem->IsKindOf(pClass))
			return pItem;
	}
	return NULL;    // no suitable item found
}

COleClientItem* COleDocument::GetNextClientItem(POSITION& pos) const
{
	COleClientItem *pItem =
		(COleClientItem*)GetNextItemOfKind(pos, RUNTIME_CLASS(COleClientItem));
	return pItem;
}

COleServerItem* COleDocument::GetNextServerItem(POSITION& pos) const
{
	COleServerItem *pItem =
		(COleServerItem*)GetNextItemOfKind(pos, RUNTIME_CLASS(COleServerItem));
	return pItem;
}

void COleDocument::DeleteContents()
{
	// deletes all COleClientItem objects in the doc item list
	//  (Note: doesn't touch server items or other docitems)

	POSITION pos = GetStartPosition();
	COleClientItem* pItem;
	while ((pItem = GetNextClientItem(pos)) != NULL)
	{
		if (pItem->m_lpObject != NULL)
		{
			pItem->Release(OLECLOSE_NOSAVE);    // release OLE object
			RemoveItem(pItem);  // disconnect from document
			pItem->InternalRelease();   // may 'delete pItem'
		}
	}
}

void COleDocument::SetPathName(const char* pszPathName, BOOL bAddToMRU)
{
	CDocument::SetPathName(pszPathName, bAddToMRU);

	// update all of the objects' host names
	POSITION pos = GetStartPosition();
	COleClientItem* pItem;
	while ((pItem = GetNextClientItem(pos)) != NULL)
	{
		// update that item's host names
		pItem->m_lpObject->SetHostNames(AfxGetAppName(), m_strTitle);
	}
}

void COleDocument::Serialize(CArchive& ar)
{
	ASSERT_VALID(this);

	// serialize all items in the doc item list
	if (ar.IsStoring())
	{
		DWORD dwCount = 0;
		POSITION pos = GetStartPosition();
		while (pos != NULL)
		{
			CDocItem* pDocItem = GetNextItem(pos);
			ASSERT_VALID(pDocItem);

			// only count non-blank ones
			if (!pDocItem->IsBlank())
				++dwCount;
		}
		ar << dwCount;  // write count of objects

		// serialize all the items in the list
		pos = GetStartPosition();
		while (pos != NULL)
		{
			CDocItem* pDocItem = GetNextItem(pos);
			ASSERT_VALID(pDocItem);

			// only write non-blank ones
			if (!pDocItem->IsBlank())
				ar << pDocItem;
		}
	}
	else
	{
		// read number of items in the file
		DWORD dwCount;
		ar >> dwCount;

		// read all of them into the list
		while (dwCount--)
		{
			CDocItem* pDocItem;
			ar >> pDocItem;     // as they are serialized, they are added!
		}
	}
}

void COleDocument::CommitItems(BOOL bSuccess)
{
	// special 'Commit' phase for COleClientItem items
	POSITION pos = GetStartPosition();
	COleClientItem* pItem;
	while ((pItem = GetNextClientItem(pos)) != NULL)
	{
		// calling CommitItem with FALSE causes the object to revert
		//  to the original storage.  Calling CommitItem TRUE causes
		//  the item to adopt the new storage created in the Serialize
		//  function.
		pItem->CommitItem(bSuccess);
	}
}

BOOL COleDocument::HasBlankItems() const
{
	ASSERT_VALID(this);

	POSITION pos = GetStartPosition();
	while (pos != NULL)
	{
		CDocItem* pDocItem = GetNextItem(pos);
		ASSERT_VALID(pDocItem);
		if (pDocItem->IsBlank())
			return TRUE;    // blank item found
	}
	return FALSE;   // no items found that were blank
}

void COleDocument::UpdateModifiedFlag()
{
	ASSERT_VALID(this);

	POSITION pos = GetStartPosition();
	COleClientItem* pItem;
	while ((pItem = GetNextClientItem(pos)) != NULL)
	{
		if (pItem->IsModified())
		{
			SetModifiedFlag();
			break;
		}
	}
}

void COleDocument::PreCloseFrame(CFrameWnd* pFrameArg)
{
	ASSERT_VALID(this);
	ASSERT_VALID(pFrameArg);

	// turn off redraw so the user doesn't see the deactivation happening
	BOOL bSetRedraw = FALSE;
	if (pFrameArg->GetStyle() & WS_VISIBLE)
	{
		pFrameArg->SendMessage(WM_SETREDRAW, (WPARAM)FALSE);
		bSetRedraw = TRUE;
	}

	// deactivate any inplace active items on this frame
	COleClientItem* pItem = GetInPlaceActiveItem(pFrameArg);
	if (pItem != NULL)
	{
		pItem->Deactivate();
		pItem->Close(OLECLOSE_NOSAVE);
	}

	// turn redraw back on
	if (bSetRedraw)
		pFrameArg->SendMessage(WM_SETREDRAW, (WPARAM)TRUE);

	// should not have any inplace active items
	ASSERT(GetInPlaceActiveItem(pFrameArg) == NULL);
}

BOOL COleDocument::SaveModified()
{
	// determine if necessary to discard changes
	if (::InSendMessage())
	{
		POSITION pos = GetStartPosition();
		COleClientItem* pItem;
		while ((pItem = GetNextClientItem(pos)) != NULL)
		{
			ASSERT(pItem->m_lpObject != NULL);
			SCODE sc = GetScode(pItem->m_lpObject->IsUpToDate());
			if (sc != OLE_E_NOTRUNNING && FAILED(sc))
			{
				// inside inter-app SendMessage limits the user's choices
				CString name = m_strPathName;
				if (name.IsEmpty())
					VERIFY(name.LoadString(AFX_IDS_UNTITLED));

				CString prompt;
				AfxFormatString1(prompt, AFX_IDP_ASK_TO_DISCARD, name);
				return AfxMessageBox(prompt, MB_OKCANCEL|MB_DEFBUTTON2,
					AFX_IDP_ASK_TO_DISCARD) == IDOK;
			}
		}
	}

	// sometimes items change without a notification, so we have to
	//  update the document's modified flag before calling
	//  CDocument::SaveModified.
	UpdateModifiedFlag();

	return CDocument::SaveModified();
}

void COleDocument::OnShowViews(BOOL bVisible)
{
	// no default implementation
}

void COleDocument::OnIdle()
{
	ASSERT_VALID(this);

	// determine if any visible views are on this document
	BOOL bVisible = FALSE;
	POSITION pos = GetFirstViewPosition();
	while (pos != NULL)
	{
		CView* pView = GetNextView(pos);
		ASSERT_VALID(pView);
		CFrameWnd* pFrameWnd = pView->GetParentFrame();
		ASSERT_VALID(pFrameWnd);
		if (pFrameWnd->GetStyle() & WS_VISIBLE)
		{
			bVisible = TRUE;
			break;
		}
	}

	// when state has changed, call OnVisibilityChagne
	if (bVisible != m_bLastVisible)
	{
		OnShowViews(bVisible);
		m_bLastVisible = bVisible;
	}
}

/////////////////////////////////////////////////////////////////////////////
// COleDocument -> window mapping

CFrameWnd* COleDocument::GetFirstFrame()
{
	ASSERT_VALID(this);

	// get position of first view in the document
	POSITION pos = GetFirstViewPosition();

	// get view at that position
	CView* pView = GetNextView(pos);
	if (pView == NULL)
		return NULL;
	ASSERT_VALID(pView);

	// return the first frame window that is a parent of that view
	CFrameWnd* pFrameWnd = (CFrameWnd*)pView->GetParentFrame();
	ASSERT_VALID(pFrameWnd);
	ASSERT(pFrameWnd->IsKindOf(RUNTIME_CLASS(CFrameWnd)));
	return pFrameWnd;
}

/////////////////////////////////////////////////////////////////////////////
// COleDocument helpers

LPMONIKER COleDocument::GetMoniker(OLEGETMONIKER nAssign)
{
	ASSERT_VALID(this);

	// no moniker for untitled documents
	if (m_strPathName.IsEmpty())
		return NULL;

	// return file moniker based on current path name
	LPMONIKER lpMoniker;
	::CreateFileMoniker((LPSTR)(LPCSTR)m_strPathName, &lpMoniker);
	return lpMoniker;
}

LPOLEITEMCONTAINER COleDocument::GetContainer()
{
	// COleDocument doesn't support IOleClientSite::GetContainer

	return NULL;
}

/////////////////////////////////////////////////////////////////////////////
// 'Compound File' enabling in COleDocument

BOOL COleDocument::OnNewDocument()
{
	// call base class, which destroys all items
	if (!CDocument::OnNewDocument())
		return FALSE;

	// for file-based compound files, need to create temporary file
	if (m_bCompoundFile)
	{
		// abort changes to the current docfile
		_AfxRelease((LPUNKNOWN*)&m_lpRootStg);

		// create new temporary docfile
		LPSTORAGE lpStorage;
		HRESULT hr = ::StgCreateDocfile(NULL, STGM_DELETEONRELEASE|
			STGM_READWRITE|STGM_TRANSACTED|STGM_SHARE_EXCLUSIVE|STGM_CREATE,
			0, &lpStorage);
		if (hr != NOERROR)
			return FALSE;

		ASSERT(lpStorage != NULL);
		m_lpRootStg = lpStorage;
	}

	return TRUE;
}

BOOL COleDocument::OnOpenDocument(const char* pszPathName)
{
	ASSERT(pszPathName == NULL || AfxIsValidString(pszPathName));

	// just use default implementation if 'docfile' not enabled
	if (!m_bCompoundFile && m_lpRootStg == NULL)
	{
		ASSERT(pszPathName != NULL);
		return CDocument::OnOpenDocument(pszPathName);
	}

	if (IsModified())
		TRACE0("Warning: OnOpenDocument replaces an unsaved document\n");

	// abort changes to current docfile
	DeleteContents();
	if (m_bCompoundFile)
		_AfxRelease((LPUNKNOWN*)&m_lpRootStg);
	SetModifiedFlag(TRUE);  // dirty during de-serialize

	BOOL bResult = FALSE;
	TRY
	{
		if (m_lpRootStg == NULL)
		{
			// open new storage file
			LPSTORAGE lpStorage;
			HRESULT hr = ::StgOpenStorage(pszPathName, NULL,
				STGM_READWRITE|STGM_TRANSACTED|STGM_SHARE_EXCLUSIVE,
				0, 0, &lpStorage);
			if (hr != NOERROR)
				hr = StgOpenStorage(pszPathName, NULL,
					STGM_READ|STGM_TRANSACTED|STGM_SHARE_EXCLUSIVE,
					0, 0, &lpStorage);
			if (hr != NOERROR)
				AfxThrowOleException(hr);
			ASSERT(lpStorage != NULL);
			m_lpRootStg = lpStorage;
		}

		// use helper to read document from storage
		LoadFromStorage();

		SetModifiedFlag(FALSE); // start off with unmodified
		bResult = TRUE;
	}
	CATCH_ALL(e)
	{
		DeleteContents();   // removed failed contents
		_AfxRelease((LPUNKNOWN*)&m_lpRootStg);

		// if not file-based load, return exceptions to the caller
		if (!m_bCompoundFile)
			THROW_LAST();

		TRY
			ReportSaveLoadException(pszPathName, e,
				FALSE, AFX_IDP_FAILED_TO_OPEN_DOC);
		END_TRY
	}
	END_CATCH_ALL

	return bResult;
}

BOOL COleDocument::OnSaveDocument(const char* pszPathName)
{
	ASSERT(pszPathName == NULL || AfxIsValidString(pszPathName));

	// use default implementation if 'docfile' not enabled
	if (!m_bCompoundFile && m_lpRootStg == NULL)
		return CDocument::OnSaveDocument(pszPathName);

	LPSTORAGE lpOrigStg = NULL;
	if (m_bCompoundFile)
		m_bSameAsLoad = (m_strPathName == pszPathName);

	BOOL bResult = FALSE;
	TRY
	{
		// open new root storage if necessary
		if (m_bCompoundFile && !m_bSameAsLoad)
		{
			// temporarily detach current storage
			lpOrigStg = m_lpRootStg;
			m_lpRootStg = NULL;

			LPSTORAGE lpStorage;
			HRESULT hr = ::StgCreateDocfile(pszPathName,
				STGM_READWRITE|STGM_TRANSACTED|STGM_SHARE_EXCLUSIVE|STGM_CREATE,
				0, &lpStorage);
			if (hr != NOERROR)
				AfxThrowOleException(hr);

			ASSERT(lpStorage != NULL);
			m_lpRootStg = lpStorage;
		}
		ASSERT(m_lpRootStg != NULL);

		// use helper to save to root storage
		SaveToStorage();

		if (m_bCompoundFile)
		{
			// commit each of the items
			CommitItems(m_bRemember && !m_bSameAsLoad);

			// mark document as clean if remembering the storage
			if (m_bRemember)
				SetModifiedFlag(FALSE);

			// remember correct storage or release save copy as storage
			if (!m_bSameAsLoad)
			{
				if (m_bRemember)
				{
					// Save As case -- m_stgRoot is new storage, forget old storage
					lpOrigStg->Release();
				}
				else
				{
					// Save Copy As case -- m_stgRoot should hook up to m_stgOrig.
					m_lpRootStg->Release();
					m_lpRootStg = lpOrigStg;
				}
			}
		}

		bResult = TRUE;
	}
	CATCH_ALL(e)
	{
		if (lpOrigStg != NULL)
		{
			// save as failed: abort new storage, and re-attach original
			_AfxRelease((LPUNKNOWN*)&m_lpRootStg);
			m_lpRootStg = lpOrigStg;
		}

		if (!m_bCompoundFile)
			THROW_LAST();

		TRY
			ReportSaveLoadException(pszPathName, e,
				TRUE, AFX_IDP_FAILED_TO_SAVE_DOC);
		END_TRY
	}
	END_CATCH_ALL

	// cleanup
	m_bSameAsLoad = TRUE;
	m_bRemember = TRUE;

	return bResult;
}

void COleDocument::OnCloseDocument()
{
	// close the document without deleting the memory
	BOOL bAutoDelete = m_bAutoDelete;
	m_bAutoDelete = FALSE;
	CDocument::OnCloseDocument();

	// release storage since document has been closed
	_AfxRelease((LPUNKNOWN*)&m_lpRootStg);

	// delete the document if necessary
	if (bAutoDelete)
		delete this;
}

/////////////////////////////////////////////////////////////////////////////
// Helpers for saving to IStorage based files
//  (these are used in the 'docfile' implementation as well as for servers)

static char BASED_CODE szContents[] = "Contents";

void COleDocument::SaveToStorage(CObject* pObject)
{
	ASSERT(m_lpRootStg != NULL);

	// create Contents stream
	COleStreamFile file;
	CFileException fe;
	if (!file.CreateStream(m_lpRootStg, szContents,
		CFile::modeWrite|CFile::shareExclusive|CFile::modeCreate, &fe))
	{
		AfxThrowFileException(fe.m_cause, fe.m_lOsError);
	}

	// save to Contents stream
	CArchive saveArchive(&file, CArchive::store | CArchive::bNoFlushOnDelete);
	saveArchive.m_pDocument = this;
	saveArchive.m_bForceFlat = FALSE;

	TRY
	{
		// save the contents
		if (pObject != NULL)
			pObject->Serialize(saveArchive);
		else
			Serialize(saveArchive);
		saveArchive.Close();
		file.Close();

		// commit the root storage
		m_lpRootStg->Commit(STGC_ONLYIFCURRENT);
	}
	CATCH_ALL(e)
	{
		file.Abort();   // will not throw an exception
		CommitItems(FALSE); // abort save in progress
		saveArchive.CArchive::~CArchive();

		THROW_LAST();
	}
	END_CATCH_ALL
}

void COleDocument::LoadFromStorage()
{
	ASSERT(m_lpRootStg != NULL);

	// open Contents stream
	COleStreamFile file;
	CFileException fe;
	if (!file.OpenStream(m_lpRootStg, szContents,
		CFile::modeReadWrite|CFile::shareExclusive, &fe))
	{
		AfxThrowFileException(fe.m_cause, fe.m_lOsError);
	}

	// load it with CArchive (loads from Contents stream)
	CArchive loadArchive(&file, CArchive::load | CArchive::bNoFlushOnDelete);
	loadArchive.m_pDocument = this;
	loadArchive.m_bForceFlat = FALSE;

	TRY
	{
		Serialize(loadArchive);     // load main contents
		loadArchive.Close();
		file.Close();
	}
	CATCH_ALL(e)
	{
		file.Abort();   // will not throw an exception
		DeleteContents();   // removed failed contents
		loadArchive.CArchive::~CArchive();

		THROW_LAST();
	}
	END_CATCH_ALL
}

/////////////////////////////////////////////////////////////////////////////
// COleDocument diagnostics

#ifdef _DEBUG
void COleDocument::AssertValid() const
{
	CDocument::AssertValid();
	ASSERT(m_ptd == NULL || AfxIsValidAddress(m_ptd, (size_t)m_ptd->tdSize, FALSE));
	m_docItemList.AssertValid();
}

void COleDocument::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
	AFX_DUMP1(dc, "\n\t ", m_docItemList.GetCount());
	AFX_DUMP0(dc, " doc items");
	AFX_DUMP1(dc, "\nm_dwNextItemNumber = ", m_dwNextItemNumber);
	AFX_DUMP1(dc, "\nm_lpRootStg = ", m_lpRootStg);
	AFX_DUMP1(dc, "\nm_bSameAsLoad = ", m_bSameAsLoad);
	AFX_DUMP1(dc, "\nm_bRemember = ", m_bRemember);
	AFX_DUMP1(dc, "\nm_ptd = ", m_ptd);
	AFX_DUMP1(dc, "\nm_bLastVisible = ", m_bLastVisible);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CDocItem

CDocItem::CDocItem()
{
	m_pDocument = NULL;
}

CDocItem::~CDocItem()
{
	ASSERT(m_pDocument == NULL);    // must be detached from document
}

void CDocItem::Serialize(CArchive& ar)
{

	if (ar.IsStoring())
	{
		ASSERT_VALID(m_pDocument);
		// nothing to do, there is no data
	}
	else
	{
		// if no document connected yet, attach it from the archive
		if (m_pDocument == NULL)
		{
			COleDocument* pContainerDoc = (COleDocument*)ar.m_pDocument;
			ASSERT_VALID(pContainerDoc);
			ASSERT(pContainerDoc->IsKindOf(RUNTIME_CLASS(COleDocument)));
			pContainerDoc->AddItem(this);
			ASSERT(pContainerDoc == m_pDocument);
		}
	}
	// perform ASSERT_VALID at the end because COleServerItem::AssertValid
	// checks the validity of the m_pDocument pointer
	ASSERT_VALID(this);
}

BOOL CDocItem::IsBlank() const
{
	// by default, a CDocItem is not blank. COleClientItem is sometimes blank!
	//  (a COleServerItem is blank by default)
	return FALSE;
}

/////////////////////////////////////////////////////////////////////////////
// CDocItem diagnostics

#ifdef _DEBUG
void CDocItem::AssertValid() const
{
	CObject::AssertValid();
	if (m_pDocument != NULL)
		m_pDocument->AssertValid();
}

void CDocItem::Dump(CDumpContext& dc) const
{
	AFX_DUMP1(dc, "\n\tm_pDocument = ", (void*)m_pDocument);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// Inline function declarations expanded out-of-line

#ifndef _AFX_ENABLE_INLINES

// expand inlines for OLE general APIs
static char BASED_CODE _szAfxOleInl[] = "afxole.inl";
#undef THIS_FILE
#define THIS_FILE _szAfxOleInl
#define _AFXOLE_INLINE
#include "afxole.inl"

#endif //!_AFX_ENABLE_INLINES

/////////////////////////////////////////////////////////////////////////////
