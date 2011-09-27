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
#include <shellapi.h>

#ifdef AFX_OLE4_SEG
#pragma code_seg(AFX_OLE4_SEG)
#endif

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

#define new DEBUG_NEW

//////////////////////////////////////////////////////////////////////////////
// COleServerDoc implementation

IMPLEMENT_DYNAMIC(COleServerDoc, COleLinkingDoc)

COleServerDoc::COleServerDoc()
{
	m_lpClientSite = NULL;  // will be non-NULL when document is embedding
	m_bCntrVisible = FALSE;
	m_pInPlaceFrame = NULL; // will be non-NULL when in-place active
	m_pOrigParent = NULL;
	m_dwOrigStyle = 0;
	m_bClosing = FALSE;
	m_pEmbeddedItem = NULL; // will be non-NULL if embedded item needed
}

COleServerDoc::~COleServerDoc()
{
	DeleteContents();   // Note: will not call derived class

	// disconnect (remove) all items from the document
	POSITION pos = GetStartPosition();
	COleServerItem* pItem;
	while ((pItem = GetNextServerItem(pos)) != NULL)
		RemoveItem(pItem);

	// should not be in-place active when doc is destroyed!
	ASSERT(m_pInPlaceFrame == NULL);

	// Note: this must be done before the client site is released
	_AfxRelease((LPUNKNOWN*)&m_lpRootStg);

	// release client-site pointer
	_AfxRelease((LPUNKNOWN*)&m_lpClientSite);
}

void COleServerDoc::DeleteContents()
{
	COleLinkingDoc::DeleteContents();

	// protect all server items with an extra reference count
	POSITION pos = GetStartPosition();
	COleServerItem* pItem;
	while ((pItem = GetNextServerItem(pos)) != NULL)
		pItem->InternalAddRef();

	// delete any autodelete server items
	pos = GetStartPosition();
	while ((pItem = GetNextServerItem(pos)) != NULL)
	{
		if (pItem->m_bAutoDelete)
			delete pItem;
	}

	// remove extra reference added above
	pos = GetStartPosition();
	while ((pItem = GetNextServerItem(pos)) != NULL)
		pItem->InternalRelease();
}

COleServerItem* COleServerDoc::GetEmbeddedItem()
{
	// allocate embedded item if necessary
	if (m_pEmbeddedItem == NULL)
		m_pEmbeddedItem = OnGetEmbeddedItem();

	ASSERT_VALID(m_pEmbeddedItem);
	return m_pEmbeddedItem;
}

/////////////////////////////////////////////////////////////////////////////
// COleServerDoc client notifications

void COleServerDoc::NotifyRename(LPCSTR lpszNewName)
{
	ASSERT_VALID(this);
	ASSERT(AfxIsValidString(lpszNewName));

	if (m_pFactory == NULL)
		return;

	// update running object table with new moniker
	Revoke();
	CString strFileName = lpszNewName;
	Register(m_pFactory, strFileName);

	// notify all items of the new moniker
	POSITION pos = GetStartPosition();
	COleServerItem* pItem;
	while ((pItem = GetNextServerItem(pos)) != NULL)
	{
		// get IOleObject interface for this item
		LPOLEOBJECT lpOleObject = pItem->GetOleObject();
		ASSERT(lpOleObject != NULL);

		// get moniker from OLE object
		LPMONIKER lpMoniker = NULL;
		lpOleObject->GetMoniker(OLEGETMONIKER_ONLYIFTHERE,
			OLEWHICHMK_OBJFULL, &lpMoniker);

		// notify client directly
		pItem->NotifyClient(OLE_RENAMED, (DWORD)lpMoniker);
		_AfxRelease((LPUNKNOWN*)&lpMoniker);
	}
}

void COleServerDoc::NotifyAllItems(OLE_NOTIFICATION nCode, DWORD dwParam)
{
	ASSERT_VALID(this);

	POSITION pos = GetStartPosition();
	COleServerItem* pItem;
	while ((pItem = GetNextServerItem(pos)) != NULL)
	{
		// notify client directly
		pItem->NotifyClient(nCode, dwParam);
	}
}

// UpdateAllItems is much like UpdateAllViews, but for server items
void COleServerDoc::UpdateAllItems(COleServerItem* pSender,
	LPARAM lHint, CObject* pHint, DVASPECT nDrawAspect)
{
	ASSERT_VALID(this);

	POSITION pos = GetStartPosition();
	COleServerItem* pItem;
	while ((pItem = GetNextServerItem(pos)) != NULL)
	{
		// notify client indirectly through OnUpdate
		if (pItem != pSender)
			pItem->OnUpdate(pSender, lHint, pHint, nDrawAspect);
	}
}

void COleServerItem::OnUpdate(COleServerItem* pSender,
	LPARAM lHint, CObject* pHint, DVASPECT nDrawAspect)
{
	// the default implementation always notifies the container, regardless
	//  of the specific hint or sender.

	NotifyChanged(nDrawAspect);
}

/////////////////////////////////////////////////////////////////////////////
// COleServerDoc attributes

BOOL COleServerDoc::GetZoomFactor(LPSIZE lpSizeNum, LPSIZE lpSizeDenom,
	LPCRECT lpPosRect) const
{
	ASSERT_VALID(this);
	ASSERT(lpSizeNum == NULL || AfxIsValidAddress(lpSizeNum, sizeof(SIZE)));
	ASSERT(lpSizeDenom == NULL || AfxIsValidAddress(lpSizeDenom, sizeof(SIZE)));
	ASSERT(lpPosRect == NULL ||
		AfxIsValidAddress(lpPosRect, sizeof(RECT), FALSE));

	if (!IsInPlaceActive())
	{
		if (lpSizeNum != NULL)
		{
			ASSERT(lpSizeDenom != NULL);
			lpSizeNum->cx = 1;
			lpSizeNum->cy = 1;
			*lpSizeDenom = *lpSizeNum;
		}
		return FALSE;
	}
	ASSERT_VALID(m_pInPlaceFrame);

	// the zoom factor is (Size(position-rect) / m_sizeExtent)
	CSize sizeNum;
	if (lpPosRect == NULL)
	{
		// use current position rect
		sizeNum = m_pInPlaceFrame->m_rectPos.Size();
	}
	else
	{
		// use new position rect
		sizeNum.cx = lpPosRect->right - lpPosRect->left;
		sizeNum.cy = lpPosRect->bottom - lpPosRect->top;
	}

	// m_sizeExtent is in HIMETRIC units -- need to convert to pixels.
	CSize sizeDenom(0, 0);
	COleServerItem* pItem = ((COleServerDoc*)this)->GetEmbeddedItem();
	ASSERT_VALID(pItem);
	ASSERT(pItem->IsKindOf(RUNTIME_CLASS(COleServerItem)));

	// get zoom denominator, which is based on the current extent
	((COleServerItem*)pItem)->OnGetExtent(DVASPECT_CONTENT, sizeDenom);
	if (sizeDenom.cx == 0 || sizeDenom.cy == 0)
	{
		// no extent from container available, so use natural extent instead
		pItem->OnGetExtent(DVASPECT_CONTENT, sizeDenom);
	}
	// convert extent to pixels first
	((CDC*)NULL)->HIMETRICtoDP(&sizeDenom);

	// might be bad to have zoom denominator of zero!
	if (sizeDenom.cy == 0 || sizeDenom.cx == 0)
	{
		TRACE0("Warning: zero 'zoom denominator', using 100%% zoom "
			"instead.\n");
		sizeDenom = sizeNum;
	}

	// store the results
	if (lpSizeNum != NULL)
	{
		ASSERT(lpSizeDenom != NULL);
		*lpSizeNum = sizeNum;
		*lpSizeDenom = sizeDenom;
	}

	// if 100% scaling, return FALSE
	return sizeNum != sizeDenom;
}

void COleServerDoc::GetItemPosition(LPRECT lpPosRect) const
{
	ASSERT_VALID(this);
	ASSERT(AfxIsValidAddress(lpPosRect, sizeof(RECT)));
	ASSERT(IsInPlaceActive());
	ASSERT_VALID(m_pInPlaceFrame);

	// copy the current rectangle
	*lpPosRect = m_pInPlaceFrame->m_rectPos;
}

void COleServerDoc::GetItemClipRect(LPRECT lpClipRect) const
{
	ASSERT_VALID(this);
	ASSERT(AfxIsValidAddress(lpClipRect, sizeof(RECT)));
	ASSERT(IsInPlaceActive());
	ASSERT_VALID(m_pInPlaceFrame);

	// copy the current rectangle
	*lpClipRect = m_pInPlaceFrame->m_rectClip;
}

/////////////////////////////////////////////////////////////////////////////
// COleServerDoc overrideables

COleServerItem* COleServerDoc::OnGetLinkedItem(LPCSTR lpszItemName)
{
	ASSERT_VALID(this);
	ASSERT(AfxIsValidString(lpszItemName));

	// default implementation walks list of server items looking for
	//  a case sensitive match

	POSITION pos = GetStartPosition();
	COleServerItem* pItem;
	while ((pItem = GetNextServerItem(pos)) != NULL)
	{
		// return item if name matches (case sensitive)
		if (lstrcmp(pItem->GetItemName(), lpszItemName) == 0)
			return pItem;
	}

	OLE_TRACE1("Warning: default COleServerDoc::OnGetLinkedItem implementation "
			"failed to find item '%Fs'\n", lpszItemName);
	return NULL;        // not found (no link found)
}

void COleServerDoc::OnClose(OLECLOSE dwCloseOption)
{
	ASSERT_VALID(this);

	// do nothing if already in the process of closing the document
	if (m_bClosing)
		return;

	// OLECLOSE_PROMPTSAVE is handled like OLECLOSE_SAVEIFDIRTY if invisible
	CFrameWnd* pFrameWnd = GetFirstFrame();
	if (pFrameWnd != NULL && pFrameWnd->IsWindowVisible())
		dwCloseOption = OLECLOSE_SAVEIFDIRTY;

	// handle modified document and special dwCloseOption flags
	TRY
	{
		if (IsModified())
		{
			switch (dwCloseOption)
			{
			case OLECLOSE_PROMPTSAVE:
				if (!SaveModifiedPrompt())
					AfxThrowOleException(OLE_E_PROMPTSAVECANCELLED);
				break;

			case OLECLOSE_SAVEIFDIRTY:
				SaveEmbedding();
				break;
			}
		}
	}
	END_TRY

	// deactivate in-place session
	if (m_pInPlaceFrame != NULL)
	{
		OnDeactivate();
		ASSERT(m_pInPlaceFrame == NULL);
	}

	// close the document
	BOOL bAutoDelete = m_bAutoDelete;
	m_bAutoDelete = FALSE;
	OnCloseDocument();
	m_bAutoDelete = bAutoDelete;
}

BOOL COleServerDoc::SaveModifiedPrompt()
{
	ASSERT_VALID(this);

	if (m_lpClientSite == NULL)
		return SaveModified();

	UpdateModifiedFlag();   // check for modified client items

	if (!IsModified())
		return TRUE;        // ok to continue

	CString prompt;
	AfxFormatString1(prompt, AFX_IDP_ASK_TO_UPDATE, m_strTitle);
	switch (AfxMessageBox(prompt, MB_YESNOCANCEL, AFX_IDP_ASK_TO_UPDATE))
	{
	case IDCANCEL:
		return FALSE;       // don't continue

	case IDYES:
		if (!OnUpdateDocument())
		{
			TRACE0("Warning: OnUpdateDocument failed to update\n");
			// keep going, close will flush it
		}
		break;
	}
	return TRUE;    // keep going
}

BOOL COleServerDoc::CanCloseFrame(CFrameWnd* pFrame)
{
	m_bClosing = TRUE;
	if (!COleLinkingDoc::CanCloseFrame(pFrame))
	{
		m_bClosing = FALSE;
		return FALSE;
	}
	return TRUE;
}

BOOL COleServerDoc::GetFileTypeString(CString& rString)
{
	ASSERT_VALID(this);

	CDocTemplate* pTemplate = GetDocTemplate();
	if (pTemplate == NULL)
		return FALSE;

	pTemplate->GetDocString(rString, CDocTemplate::fileNewName);
	return !rString.IsEmpty();
}

void COleServerDoc::OnSetHostNames(LPCSTR lpszHost, LPCSTR lpszHostObj)
{
	ASSERT_VALID(this);
	ASSERT(lpszHost == NULL || AfxIsValidString(lpszHost));
	ASSERT(lpszHostObj == NULL || AfxIsValidString(lpszHostObj));

	// only change the title for embedded documents
	if (m_lpClientSite != NULL)
	{
		// save name of document for File.Exit update
		if (lpszHostObj == NULL)
			m_strHostObj.LoadString(AFX_IDS_UNTITLED);
		else
			m_strHostObj = lpszHostObj;

		// try to get the document name from the document template
		CString strFileType;
		if (!GetFileTypeString(strFileType))
			return;

		// format the string into <server-name> in <docname>
		CString strTitle;
		AfxFormatString2(strTitle, AFX_IDS_APP_TITLE_EMBEDDING,
			strFileType, m_strHostObj);

		// change title of document to that for an embedded item
		//  (this call will update all of the frames)
		SetTitle(strTitle);
	}
}

void COleServerDoc::SaveEmbedding()
{
	ASSERT_VALID(this);

	// tell the client site to save the object
	if (m_lpClientSite != NULL && !::InSendMessage())
	{
		HRESULT hr = m_lpClientSite->SaveObject();
		if (hr != NOERROR)
			AfxThrowOleException(hr);
	}
	ASSERT_VALID(this);
}

BOOL COleServerDoc::OnUpdateDocument()
{
	ASSERT_VALID(this);

	// don't save if already up-to-date
	if (!IsModified())
		return TRUE;

	// save a server document -> update
	TRY
	{
		SaveEmbedding();
	}
	CATCH_ALL(e)
	{
		AfxMessageBox(AFX_IDP_FAILED_TO_UPDATE);
		return FALSE;
	}
	END_CATCH_ALL

	return TRUE;
}

LPMONIKER COleServerDoc::GetMoniker(OLEGETMONIKER nAssign)
{
	ASSERT_VALID(this);

	if (m_lpClientSite != NULL)
	{
		// get moniker from client site instead of from document
		LPMONIKER lpMoniker = NULL;
		m_lpClientSite->GetMoniker(nAssign, OLEWHICHMK_OBJFULL, &lpMoniker);
		return lpMoniker;
	}

	return COleLinkingDoc::GetMoniker(nAssign);
}

/////////////////////////////////////////////////////////////////////////////
// COleServerDoc document handling

BOOL COleServerDoc::SaveModified()
{
	ASSERT_VALID(this);

	if (m_lpClientSite != NULL)
	{
		if (m_pInPlaceFrame == NULL)
		{
			UpdateModifiedFlag();   // check for modified items
			OnUpdateDocument();
		}
		return TRUE;
	}
	return COleLinkingDoc::SaveModified();
}

HMENU COleServerDoc::GetDefaultMenu()
{
	ASSERT_VALID(this);

	CDocTemplate* pTemplate = GetDocTemplate();
	if (pTemplate == NULL)
		return NULL;    // no doc template -- use default

	ASSERT_VALID(pTemplate);
	if (m_pInPlaceFrame != NULL)
		return pTemplate->m_hMenuInPlaceServer; // return special in-place menu
	else if (m_lpClientSite != NULL)
		return pTemplate->m_hMenuEmbedding; // return special embedding menu

	return NULL;    // no special mode, use default menu
}

HACCEL COleServerDoc::GetDefaultAccelerator()
{
	ASSERT_VALID(this);

	CDocTemplate* pTemplate = GetDocTemplate();
	if (pTemplate == NULL)
		return NULL;    // no doc template -- use default

	ASSERT_VALID(pTemplate);
	if (m_pInPlaceFrame != NULL)
		return pTemplate->m_hAccelInPlaceServer;    // return special in-place accel
	else if (m_lpClientSite != NULL)
		return pTemplate->m_hAccelEmbedding;// return special embedding accel

	return NULL;    // no special mode, use default menu
}

/////////////////////////////////////////////////////////////////////////////
// COleServerDoc default command handling

BEGIN_MESSAGE_MAP(COleServerDoc, COleLinkingDoc)
	//{{AFX_MSG_MAP(COleServerDoc)
	ON_COMMAND(ID_FILE_UPDATE, OnFileUpdate)
	ON_COMMAND(ID_FILE_SAVE_COPY_AS, OnFileSaveCopyAs)
	ON_UPDATE_COMMAND_UI(ID_APP_EXIT, OnUpdateFileExit)
	ON_UPDATE_COMMAND_UI(ID_FILE_UPDATE, OnUpdateFileUpdate)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

void COleServerDoc::OnFileUpdate()
{
	ASSERT_VALID(this);
	ASSERT(m_lpClientSite != NULL);

	UpdateModifiedFlag();
	OnUpdateDocument();
}

void COleServerDoc::OnFileSaveCopyAs()
{
	ASSERT_VALID(this);
	ASSERT(m_bRemember);
	ASSERT(m_lpClientSite != NULL);

	LPSTORAGE lpOrigStg = m_lpRootStg;
	m_lpRootStg = NULL; // ignore embedded storage for now

	TRY
	{
		// call DoSave to perform Save Copy As...
		m_bRemember = FALSE;
		DoSave(NULL, FALSE);
	}
	END_TRY

	m_lpRootStg = lpOrigStg;
	m_bRemember = TRUE;
}

void COleServerDoc::UpdateUsingHostObj(UINT nIDS, CCmdUI* pCmdUI)
{
	ASSERT_VALID(this);
	ASSERT(pCmdUI != NULL);

	if (m_lpClientSite == NULL)
		return;

	// update menu item using m_strHostObj
	CString str;
	AfxFormatString1(str, nIDS, m_strHostObj);
	if (!str.IsEmpty())
		pCmdUI->SetText(str);
}

void COleServerDoc::OnUpdateFileExit(CCmdUI* pCmdUI)
{
	ASSERT_VALID(this);
	ASSERT(pCmdUI != NULL);

	UpdateUsingHostObj(AFX_IDS_EXIT_MENU, pCmdUI);
}

void COleServerDoc::OnUpdateFileUpdate(CCmdUI* pCmdUI)
{
	ASSERT_VALID(this);
	ASSERT(pCmdUI != NULL);

	UpdateUsingHostObj(AFX_IDS_UPDATE_MENU, pCmdUI);
}

BOOL COleServerDoc::OnSaveDocument(const char* pszPathName)
{
	ASSERT_VALID(this);
	ASSERT(pszPathName == NULL || AfxIsValidString(pszPathName));

	BOOL bRemember = m_bRemember;
	if (!COleLinkingDoc::OnSaveDocument(pszPathName))
		return FALSE;

	if (pszPathName != NULL && bRemember)
	{
		if (GetPathName() == pszPathName)
		{
			// saved to same file
			NotifySaved();
		}
		else
		{
			// saved to a different file
			NotifyRename(pszPathName);
		}
	}
	return TRUE;
}

void COleServerDoc::OnCloseDocument()
{
	ASSERT_VALID(this);

	// don't allow in-place active documents to be closed without first
	//  deactivating them!
	if (m_pInPlaceFrame != NULL)
	{
		if (GetFirstViewPosition() != NULL)
			return;

		// no views but currently in-place active indicates that
		//  a WM_ENDSESSION is being processed.
		m_pInPlaceFrame = NULL;
	}

	InternalAddRef();   // keep document stable during shutdown

	// first hide the document
	OnShowDocument(FALSE);

	// send some notifications to the container
	if (m_lpClientSite != NULL && m_bCntrVisible)
	{
		// allow the container to unshade the object appropriately
		m_lpClientSite->OnShowWindow(FALSE);
		m_bCntrVisible = FALSE;
	}

	// send close notification
	NotifyClosed();

	// finish closing the document (before m_lpClientSite->Release)
	BOOL bAutoDelete = m_bAutoDelete;
	m_bAutoDelete = FALSE;
	COleLinkingDoc::OnCloseDocument();
	ASSERT_VALID(this);

	// release client-site pointer
	_AfxRelease((LPUNKNOWN*)&m_lpClientSite);

	// disconnect the object
	LPUNKNOWN lpUnknown = (LPUNKNOWN)GetInterface(&IID_IUnknown);
	ASSERT(lpUnknown != NULL);
	CoDisconnectObject(lpUnknown, 0);

	// destroy the document if allowed
	--m_dwRef;              // remove InternalAddRef above
	if (bAutoDelete)
		delete this;
}

/////////////////////////////////////////////////////////////////////////////
// COleServerDoc show/hide

void COleServerDoc::OnShowDocument(BOOL bShow)
{
	ASSERT_VALID(this);

	if (bShow)
	{
		// deactivate in-place session if active
		if (m_pInPlaceFrame != NULL)
		{
			OnDeactivate();
			ASSERT(m_pInPlaceFrame == NULL);
		}

		// find the first view of this document
		CFrameWnd* pFrameWnd;
		if ((pFrameWnd = GetFirstFrame()) != NULL)
		{
			// allow container to show & scroll to the object
			if (!pFrameWnd->IsWindowVisible() && m_lpClientSite != NULL)
				m_lpClientSite->ShowObject();

			// activate frame holding view
			ASSERT_VALID(pFrameWnd);
			pFrameWnd->ActivateFrame();

			// activate application if necessary
			CFrameWnd* pAppFrame = pFrameWnd->GetParentFrame();
			if (pAppFrame != NULL)
			{
				pFrameWnd = pAppFrame;
				ASSERT_VALID(pFrameWnd);
				ASSERT(pFrameWnd->IsKindOf(RUNTIME_CLASS(CFrameWnd)));
				pFrameWnd->ActivateFrame();
			}

			// update the menu and title as appropriate for this document
			pFrameWnd->OnUpdateFrameMenu(NULL);
			pFrameWnd->OnUpdateFrameTitle(TRUE);
		}
		else if (AfxGetApp()->m_pMainWnd != NULL)
		{
			// otherwise, just show the main window (for simple servers)
			CWnd* pWnd = AfxGetMainWnd();

			// allow container to show & scroll to the object
			if (!pWnd->IsWindowVisible() && m_lpClientSite != NULL)
				m_lpClientSite->ShowObject();

			pWnd->ShowWindow(SW_SHOW);
			pWnd->SetActiveWindow();
		}

		// for file based documents, showing the document puts user in control.
		if (!m_strPathName.IsEmpty())
			AfxOleSetUserCtrl(TRUE);
	}
	else
	{
		if (m_pInPlaceFrame != NULL)
		{
			// hide has semantics of DeactivateUI if the item is active
			if (m_pInPlaceFrame->m_bUIActive)
				OnDeactivateUI(FALSE);

			// and then hide the frame itself
			m_pInPlaceFrame->ActivateFrame(SW_HIDE);
			return;
		}

		// find the first view of this document
		POSITION pos = GetFirstViewPosition();
		if (pos != NULL)
		{
			CFrameWnd* pDocFrame = GetFirstFrame();
			CFrameWnd* pActiveFrame = NULL;
			CFrameWnd* pMainFrame = NULL;
			CView* pView = GetNextView(pos);
			ASSERT_VALID(pView);

			// destroy or hide all the frames for this document
			//  (the main for the document is hidden, where the alternate
			//   frames are simply destroyed)
			do
			{
				// hide frame holding view
				CFrameWnd* pFrame = pView->GetParentFrame();
				ASSERT_VALID(pFrame);

				// determine next valid view before destroying the frame
				while ((pView = GetNextView(pos)) != NULL)
				{
					if (pView->GetParentFrame() != pFrame)
						break;
				}

				pMainFrame = pFrame->GetParentFrame();
				if (pMainFrame != NULL && pMainFrame->GetActiveFrame() == pFrame)
				{
					// don't destroy the active frame until later
					pActiveFrame = pFrame;
				}
				else
				{
					// not the active frame -- destroy/hide it now
					PreCloseFrame(pFrame);
					if (pDocFrame == pFrame)
						pFrame->ActivateFrame(SW_HIDE);
					else
						pFrame->DestroyWindow();
				}

			} while (pView != NULL);

			// hide the active frame last
			if (pActiveFrame != NULL)
			{
				PreCloseFrame(pActiveFrame);
				if (pDocFrame == pActiveFrame)
					pActiveFrame->ActivateFrame(SW_HIDE);
				else
					pActiveFrame->DestroyWindow();

				// should leave at least one frame
				ASSERT_VALID(this);
				ASSERT_VALID(GetFirstFrame());
			}
		}

		// hide the entire application if no visible documents left
		CFrameWnd* pMainFrame = (CFrameWnd*)AfxGetApp()->m_pMainWnd;
		if (!AfxOleGetUserCtrl() && AfxGetApp()->m_pMainWnd != NULL &&
			AfxGetApp()->m_pMainWnd->IsWindowEnabled() &&
			AfxGetApp()->m_pMainWnd->IsFrameWnd())
		{
			CFrameWnd* pMainFrame = (CFrameWnd*)AfxGetApp()->m_pMainWnd;
			if (pMainFrame->GetActiveFrame() == pMainFrame)
				AfxGetApp()->HideApplication();
		}
	}

	// send OnShowWindow notifications to the container
	if (m_lpClientSite != NULL && (bShow || m_bCntrVisible != bShow))
	{
		// allow the container to shade the object appropriately
		m_lpClientSite->OnShowWindow(bShow);
		m_bCntrVisible = bShow;
	}

	// force update visible lock
	UpdateVisibleLock(bShow, FALSE);
}

/////////////////////////////////////////////////////////////////////////////
// COleServerDoc storage implementation

void COleServerDoc::OnNewEmbedding(LPSTORAGE lpStorage)
{
	ASSERT_VALID(this);
	ASSERT(lpStorage != NULL);

	// save state
	BOOL bUserCtrl = AfxOleGetUserCtrl();
	BOOL bCompoundFile = m_bCompoundFile;

	TRY
	{
		// remember new storage
		DeleteContents();
		lpStorage->AddRef();
		_AfxRelease((LPUNKNOWN*)&m_lpRootStg);
		m_lpRootStg = lpStorage;
		m_strPathName.Empty();

		// do document initialization by calling OnNewDocument
		m_bCompoundFile = FALSE;
		if (!OnNewDocument())
			AfxThrowMemoryException();
	}
	CATCH_ALL(e)
	{
		// restore state
		AfxOleSetUserCtrl(bUserCtrl);
		m_bCompoundFile = bCompoundFile;

		THROW_LAST();
	}
	END_CATCH_ALL

	// restore state
	AfxOleSetUserCtrl(bUserCtrl);
	m_bCompoundFile = bCompoundFile;

	SetModifiedFlag(TRUE);  // new storage-based documents are dirty!
	SendInitialUpdate();
}

void COleServerDoc::OnOpenEmbedding(LPSTORAGE lpStorage)
{
	ASSERT_VALID(this);
	ASSERT(lpStorage != NULL);

	// save state
	BOOL bUserCtrl = AfxOleGetUserCtrl();
	BOOL bCompoundFile = m_bCompoundFile;

	TRY
	{
		// abort changes to current document
		DeleteContents();
		lpStorage->AddRef();
		_AfxRelease((LPUNKNOWN*)&m_lpRootStg);
		m_lpRootStg = lpStorage;
		m_strPathName.Empty();

		// open document from the sub-storage
		m_bCompoundFile = FALSE;
		if (!OnOpenDocument(NULL))
			AfxThrowMemoryException();
	}
	CATCH_ALL(e)
	{
		// restore state
		AfxOleSetUserCtrl(bUserCtrl);
		m_bCompoundFile = bCompoundFile;

		THROW_LAST();
	}
	END_CATCH_ALL

	// restore state
	AfxOleSetUserCtrl(bUserCtrl);
	m_bCompoundFile = bCompoundFile;

	SetModifiedFlag(FALSE); // start off with unmodified
	SendInitialUpdate();
}

void COleServerDoc::OnSaveEmbedding(LPSTORAGE lpStorage)
{
	ASSERT_VALID(this);
	ASSERT(lpStorage != NULL);

	// save state
	BOOL bUserCtrl = AfxOleGetUserCtrl();
	BOOL bCompoundFile = m_bCompoundFile;

	// check for save as
	LPSTORAGE lpOrigStg = m_lpRootStg;
	if (!m_bSameAsLoad)
	{
		// File Save[Copy] As (saving to different file)
		ASSERT(lpStorage != NULL);
		m_lpRootStg = lpStorage;
	}

	TRY
	{
		// save document to the sub-storage
		m_bCompoundFile = FALSE;
		if (!OnSaveDocument(NULL))
			AfxThrowMemoryException();
	}
	CATCH_ALL(e)
	{
		// restore state
		AfxOleSetUserCtrl(bUserCtrl);
		m_bCompoundFile = bCompoundFile;

		// re-attach original storage
		m_lpRootStg = lpOrigStg;

		THROW_LAST();
	}
	END_CATCH_ALL

	// restore state
	AfxOleSetUserCtrl(bUserCtrl);
	m_bCompoundFile = bCompoundFile;

	// re-attach original storage
	m_lpRootStg = lpOrigStg;
}

/////////////////////////////////////////////////////////////////////////////
// COleServerDoc in-place activation implementation

BOOL COleServerDoc::ActivateInPlace()
{
	ASSERT_VALID(this);

	if (m_lpClientSite == NULL)
		return FALSE;   // no client-side (may be a link)

	// activate already in-place window if currently in-place active
	if (m_pInPlaceFrame != NULL)
	{
		if (m_pInPlaceFrame->m_bUIActive)
		{
			m_lpClientSite->ShowObject();   // object should get focus
			return TRUE;
		}

		// deactivate in-place session entirely before continuing
		OnDeactivate();
	}

	// fail if already fully open
	if (GetFirstFrame()->IsWindowVisible())
		return FALSE;

	// build object title/name (the container may use this in its caption)
	CString strFileType, strTitle;
	if (!GetFileTypeString(strFileType))
		return FALSE;
	AfxFormatString2(strTitle, AFX_IDS_OBJ_TITLE_INPLACE,
		AfxGetAppName(), strFileType);

	// attempt to get in-place client-site interface
	LPOLEINPLACESITE lpInPlaceSite =
		_AFXQUERYINTERFACE(m_lpClientSite, IOleInPlaceSite);
	if (lpInPlaceSite == NULL)
	{
		// unable to get in-place client site interface
		return FALSE;
	}

	// see if the container wants to go in-place right now
	if (lpInPlaceSite->CanInPlaceActivate() != NOERROR)
		goto ReleaseAndFail;

	// start activation sequence...

	if (lpInPlaceSite->OnInPlaceActivate() != NOERROR)
		goto ReleaseAndFail;

	// we'll need the parent window to create the COleIPFrameWnd
	HWND hWnd;
	VERIFY(lpInPlaceSite->GetWindow(&hWnd) == NOERROR);
	CWnd* pParentWnd;
	pParentWnd = CWnd::FromHandle(hWnd);

	// create the inplace frame window
	COleIPFrameWnd* pFrameWnd;
	pFrameWnd = CreateInPlaceFrame(pParentWnd);
	if (pFrameWnd == NULL)
	{
		ASSERT(lpInPlaceSite != NULL);
		lpInPlaceSite->OnInPlaceDeactivate();
		goto ReleaseAndFail;
	}
	ASSERT(pFrameWnd->GetParent() == pParentWnd);
	m_pInPlaceFrame = pFrameWnd;

	// send activate notification
	if (lpInPlaceSite->OnUIActivate() != NOERROR)
		goto DestroyFrameAndFail;

	// need to get frame & doc window interfaces as well as other info
	RECT rcPosRect, rcClipRect;
	if (lpInPlaceSite->GetWindowContext(
		&pFrameWnd->m_lpFrame, &pFrameWnd->m_lpDocFrame,
		&rcPosRect, &rcClipRect, &pFrameWnd->m_frameInfo) != NOERROR)
	{
		goto DeactivateUIAndFail;
	}
	ASSERT(pFrameWnd->m_lpFrame != NULL);

	// setup the shared menu
	if (!pFrameWnd->BuildSharedMenu(this))
		goto DeactivateUIAndFail;

	// allow server to install frame controls in container
	CWnd* pWndFrame;
	CWnd* pWndDoc;
	pWndFrame = pWndDoc = NULL;
	VERIFY(pFrameWnd->m_lpFrame->GetWindow(&hWnd) == NOERROR);
	pWndFrame = CWnd::FromHandle(hWnd);
	pFrameWnd->m_hWndFrame = hWnd;
	if (pFrameWnd->m_lpDocFrame != NULL)
	{
		VERIFY(pFrameWnd->m_lpDocFrame->GetWindow(&hWnd) == NOERROR);
		pWndDoc = CWnd::FromHandle(hWnd);
		pFrameWnd->m_hWndDocFrame = hWnd;
	}
	// update zoom factor information before creating control bars
	pFrameWnd->m_rectPos.CopyRect(&rcPosRect);
	pFrameWnd->m_rectClip.CopyRect(&rcClipRect);
	if (!pFrameWnd->OnCreateControlBars(pWndFrame, pWndDoc))
		goto DeactivateUIAndFail;

	// resize the window to match the object
	OnSetItemRects(&rcPosRect, &rcClipRect);

	// set the active object
	ASSERT(pFrameWnd->m_lpFrame != NULL);
	LPOLEINPLACEACTIVEOBJECT lpActiveObject;
	lpActiveObject = (LPOLEINPLACEACTIVEOBJECT)
		GetInterface(&IID_IOleInPlaceActiveObject);
	pFrameWnd->m_lpFrame->SetActiveObject(lpActiveObject, strTitle);
	if (pFrameWnd->m_lpDocFrame != NULL)
		pFrameWnd->m_lpDocFrame->SetActiveObject(lpActiveObject, strTitle);

	// add frame & document level frame controls
	ASSERT(m_pInPlaceFrame->m_lpFrame != NULL);
	OnShowControlBars(m_pInPlaceFrame->m_lpFrame, TRUE);
	if (m_pInPlaceFrame->m_lpDocFrame != NULL)
		OnShowControlBars(m_pInPlaceFrame->m_lpDocFrame, TRUE);

	// show any hidden modeless dialogs as well...
	m_pInPlaceFrame->ShowOwnedWindows(TRUE);

	// attempt toolbar negotiation
	OnResizeBorder(NULL, pFrameWnd->m_lpFrame, TRUE);
	if (pFrameWnd->m_lpDocFrame != NULL)
		OnResizeBorder(NULL, pFrameWnd->m_lpDocFrame, FALSE);

	// install the menu (also installs a hook which forwards messages from
	//  the menu to the inplace frame window)
	pFrameWnd->m_lpFrame->SetMenu(pFrameWnd->m_hSharedMenu,
		pFrameWnd->m_hOleMenu, pFrameWnd->m_hWnd);

	// make sure object is scrolled into view
	m_lpClientSite->ShowObject();   // object should get focus

	// finally -- show the inplace frame window and set focus
	pFrameWnd->ShowWindow(SW_SHOW);
	pFrameWnd->SetFocus();
	pFrameWnd->UpdateWindow();

	// allow the main window to be set
	OnFrameWindowActivate(TRUE);
	pFrameWnd->m_bUIActive = TRUE;

	// cleanup and return
	lpInPlaceSite->Release();
	return TRUE;

DeactivateUIAndFail:
	ASSERT(lpInPlaceSite != NULL);
	lpInPlaceSite->OnUIDeactivate(FALSE);

DestroyFrameAndFail:
	if (m_pInPlaceFrame != NULL)
	{
		ASSERT(pFrameWnd != NULL);
		DestroyInPlaceFrame(pFrameWnd);
		m_pInPlaceFrame = NULL;

		// also need to send OnInPlaceDeactivate notification
		ASSERT(lpInPlaceSite != NULL);
		lpInPlaceSite->OnInPlaceDeactivate();
	}

ReleaseAndFail:
	ASSERT(lpInPlaceSite != NULL);
	lpInPlaceSite->Release();
	return FALSE;
}

COleIPFrameWnd* COleServerDoc::CreateInPlaceFrame(CWnd* pParentWnd)
{
	ASSERT_VALID(this);
	ASSERT_VALID(pParentWnd);

	// get runtime class from the doc template
	CDocTemplate* pTemplate = GetDocTemplate();
	ASSERT_VALID(pTemplate);

	// use existing view if possible
	CWnd* pViewParent = NULL;
	CView* pView = NULL;
	CFrameWnd* pFrame = GetFirstFrame();
	if (pFrame != NULL)
	{
		pView = (CView*)pFrame->GetDescendantWindow(AFX_IDW_PANE_FIRST, TRUE);
		if (pView != NULL)
		{
			ASSERT(pView->IsKindOf(RUNTIME_CLASS(CView)));
			pViewParent = pView->GetParent();
			m_dwOrigStyle = pView->GetStyle();
		}
	}

	// create the frame from the template
	COleIPFrameWnd* pFrameWnd = (COleIPFrameWnd*)
		pTemplate->CreateOleFrame(pParentWnd, this, pView == NULL);
	if (pFrameWnd == NULL)
		return NULL;

	// connect the view to the frame window, if necessary
	if (pView != NULL)
		ConnectView(pFrameWnd, pView);

	// remember original parent window for deactivate
	m_pOrigParent = pViewParent;

	// send OnInitialUpdate if new view was created
	if (pView == NULL)
		pTemplate->InitialUpdateFrame(pFrameWnd, this, FALSE);

	// verify the type
	ASSERT_VALID(pFrameWnd);
	ASSERT(pFrameWnd->IsKindOf(RUNTIME_CLASS(COleIPFrameWnd)));
	return pFrameWnd;
}

void COleServerDoc::DestroyInPlaceFrame(COleIPFrameWnd* pFrameWnd)
{
	ASSERT_VALID(this);
	ASSERT_VALID(pFrameWnd);

	// connect view to original, if existing view was used
	if (m_pOrigParent != NULL)
	{
		CView* pView = (CView*)pFrameWnd->GetDescendantWindow(
			AFX_IDW_PANE_FIRST, TRUE);
		ASSERT_VALID(pView);

		// leaving the focus on an MDI child or one of its child windows
		// causes Windows to get confused when the child window is
		// destroyed, not to mention the fact that the focus will be
		// out of sync with activation.
		if (::GetFocus() == pView->m_hWnd)
		{
			// move focus to somewhere safe
			HWND hWnd = ::GetParent(pFrameWnd->m_hWnd);
			if (hWnd != NULL)
				::SetFocus(hWnd);

			// check again
			if (::GetFocus() == pView->m_hWnd)
				SetFocus(NULL); // last ditch effort
		}

		ConnectView(m_pOrigParent, pView);
		m_pOrigParent = NULL;

		// remove any scrollbars added because of in-place activation
		if ((m_dwOrigStyle & (WS_HSCROLL|WS_VSCROLL)) == 0 &&
			(pView->GetStyle() & (WS_HSCROLL|WS_VSCROLL)) != 0)
		{
			::SetScrollRange(pView->m_hWnd, SB_HORZ, 0, 0, TRUE);
			::SetScrollRange(pView->m_hWnd, SB_VERT, 0, 0, TRUE);
		}

		// force recalc layout on splitter window
		CSplitterWnd* pSplitter = CView::GetParentSplitter(pView, TRUE);
		if (pSplitter != NULL)
			pSplitter->RecalcLayout();
	}

	// no active view or document during destroy
	pFrameWnd->SetActiveView(NULL);

	// destroy in-place frame window
	pFrameWnd->DestroyWindow();
}

void COleServerDoc::ConnectView(CWnd* pParentWnd, CView* pView)
{
	ASSERT_VALID(this);
	ASSERT_VALID(pParentWnd);
	ASSERT_VALID(pView);

	// move the view to the new parent
	pView->SetParent(pParentWnd);

	// Note: The currently active view on the original frame window is
	//  kept active, because some controls, especially Windows controls,
	//  continue to send notification messages to the original parent
	//  window of the control.  So, the original frame window is kept
	//  alive with the original active view pointer intact, such that
	//  these notification messages do not get lost.

	// set the active view of the new frame to newly moved view
	CFrameWnd* pFrameWnd = pParentWnd->IsFrameWnd() ?
		(CFrameWnd*)pParentWnd : pParentWnd->GetParentFrame();
	pFrameWnd->SetActiveView(pView, FALSE);
	pFrameWnd->RecalcLayout();
}

void COleServerDoc::OnFrameWindowActivate(BOOL bActivate)
{
	ASSERT_VALID(this);

	CFrameWnd* pFrameWnd = m_pInPlaceFrame;
	ASSERT_VALID(pFrameWnd);

	if (bActivate)
	{
		// activating -- so set the main window
		AfxGetApp()->m_pActiveWnd = pFrameWnd;

		// send activation notification messages
		pFrameWnd->SendMessage(WM_ACTIVATEAPP, (WPARAM)TRUE);
		pFrameWnd->SendMessage(WM_ACTIVATE, WA_ACTIVE);
	}
	else if (pFrameWnd == AfxGetApp()->m_pActiveWnd)
	{
		// send deactivation notification messages
		pFrameWnd->SendMessage(WM_ACTIVATEAPP, (WPARAM)FALSE);
		pFrameWnd->SendMessage(WM_ACTIVATE, WA_INACTIVE);

		// simulate deactivation notification messages
		CView* pActiveView = pFrameWnd->GetActiveView();
		if (pActiveView != NULL)
			pActiveView->OnActivateView(FALSE, pActiveView, pActiveView);

		// deactivating and was previously active -- reset the main window
		AfxGetApp()->m_pActiveWnd = NULL;
	}
}

void COleServerDoc::OnDocWindowActivate(BOOL bActivate)
{
	ASSERT_VALID(this);

	if (bActivate)
	{
		// set active main window
		AfxGetApp()->m_pActiveWnd = m_pInPlaceFrame;

		// show frame level controls
		OnShowControlBars(m_pInPlaceFrame->m_lpFrame, TRUE);
		m_pInPlaceFrame->ShowOwnedWindows(TRUE);

		// attempt toolbar negotiation
		OnResizeBorder(NULL, m_pInPlaceFrame->m_lpFrame, TRUE);
		if (m_pInPlaceFrame->m_lpDocFrame != NULL)
			OnResizeBorder(NULL, m_pInPlaceFrame->m_lpDocFrame, FALSE);

		// install the menu (also installs a hook which forwards messages from
		//  the menu to the inplace frame window)
		m_pInPlaceFrame->m_lpFrame->SetMenu(
			m_pInPlaceFrame->m_hSharedMenu, m_pInPlaceFrame->m_hOleMenu,
			m_pInPlaceFrame->m_hWnd);

		// set focus to the frame (it will probably set focus to the view)
		//  (by simulating normal application activate messages)
		m_pInPlaceFrame->SendMessage(WM_ACTIVATE, WA_ACTIVE);
	}
	else
	{
		// hide frame level controls -- this does not destroy them
		OnShowControlBars(m_pInPlaceFrame->m_lpFrame, FALSE);

		// also hide any frame modeless windows
		m_pInPlaceFrame->ShowOwnedWindows(FALSE);

		// simulate deactivation notification messages
		CView* pActiveView = m_pInPlaceFrame->GetActiveView();
		if (pActiveView != NULL)
			pActiveView->OnActivateView(FALSE, pActiveView, pActiveView);
		m_pInPlaceFrame->SendMessage(WM_ACTIVATE, WA_INACTIVE);

		// clear active window for app if this object is active
		if (AfxGetApp()->m_pActiveWnd == m_pInPlaceFrame)
			AfxGetApp()->m_pActiveWnd = NULL;
	}
}

void COleServerDoc::OnShowControlBars(LPOLEINPLACEUIWINDOW lpUIWindow,
	BOOL bShow)
{
	ASSERT(lpUIWindow != NULL);

	// get the window handle corresponding to lpUIWindow
	HWND hWnd;
	if (lpUIWindow->GetWindow(&hWnd) != NOERROR)
		return;

	// only control bars whose owner is the same as the inplace frame window
	//  are shown/hidden.
	CFrameWnd* pFrameWnd = m_pInPlaceFrame;

	// enumerate controls looking for control bars
	for (HWND hWndChild = ::GetTopWindow(hWnd); hWndChild != NULL;
		hWndChild = ::GetNextWindow(hWndChild, GW_HWNDNEXT))
	{
		CWnd* pWnd = CWnd::FromHandlePermanent(hWndChild);
		if (pWnd != NULL && pWnd->IsKindOf(RUNTIME_CLASS(CControlBar)) &&
			pWnd->GetOwner() == pFrameWnd)
		{
			// found a control bar owned by this application -- hide/show it
			UINT nFlags = SWP_NOZORDER|SWP_NOSIZE|SWP_NOMOVE;
			if (bShow)
				nFlags |= SWP_SHOWWINDOW;
			else
				nFlags |= SWP_HIDEWINDOW;
			pWnd->SetWindowPos(NULL, 0, 0, 0, 0, nFlags);

			// update the user-interface before showing
			if (bShow)
				pWnd->SendMessage(WM_IDLEUPDATECMDUI, (WPARAM)TRUE);
		}
	}
}

void COleServerDoc::OnResizeBorder(
	LPCRECT lpRectBorder, LPOLEINPLACEUIWINDOW lpUIWindow, BOOL bFrame)
{
	ASSERT_VALID(this);
	ASSERT(lpRectBorder == NULL ||
		AfxIsValidAddress(lpRectBorder, sizeof(RECT), FALSE));
	ASSERT(lpUIWindow != NULL);

	// use IOleInPlaceUIWindow::GetBorder if no border given
	CRect rectBorder;
	if (lpRectBorder != NULL)
		rectBorder.CopyRect(lpRectBorder);
	else
		lpUIWindow->GetBorder(&rectBorder);

	// get CWnd* for the OLE window
	HWND hwnd;
	VERIFY(lpUIWindow->GetWindow(&hwnd) == NOERROR);
	CWnd* pWnd = CWnd::FromHandle(hwnd);
	ASSERT(pWnd != NULL);

	// see how much space we need by calling reposition bars
	CRect rectNeeded = rectBorder;
	pWnd->RepositionBars(0, 0xFFFF, 0, CWnd::reposQuery, &rectNeeded, &rectBorder);
	CRect rectRequest;

	// request the border space from the container
	rectRequest.SetRect(
		rectNeeded.left - rectBorder.left,
		rectNeeded.top - rectBorder.top,
		rectBorder.right - rectNeeded.right,
		rectBorder.bottom - rectNeeded.bottom);
	CRect rectTemp;
	rectTemp = rectRequest;

	// if no border space, just call SetBorderSpace
	if (rectRequest.IsRectNull())
		goto RequestFailed;

	if (lpUIWindow->RequestBorderSpace(&rectTemp) != NOERROR)
		goto RequestFailed;

	// set the border space -- now this object owns it
	VERIFY(lpUIWindow->SetBorderSpace(&rectRequest) == NOERROR);
	pWnd->RepositionBars(0, 0xFFFF, 0, CWnd::reposDefault, NULL, &rectBorder);
	return;

RequestFailed:
	// hide any toolbars (since we couldn't get border space for any)
	OnShowControlBars(lpUIWindow, FALSE);

	// make sure border space is cleared
	lpUIWindow->SetBorderSpace(NULL);
}

void COleServerDoc::OnDeactivate()
{
	ASSERT_VALID(this);

	ASSERT(m_pInPlaceFrame != NULL);

	// do UI deactivate first -- this hides everything
	if (m_pInPlaceFrame->m_bUIActive)
	{
		OnDeactivateUI(FALSE);

		// some containers call OnDeactivate during OnDeactivateUI
		if (m_pInPlaceFrame == NULL)
			return;
	}
	ASSERT(m_pInPlaceFrame != NULL);
	ASSERT(!m_pInPlaceFrame->m_bUIActive);

	// now safe to destroy the shared menu
	m_pInPlaceFrame->DestroySharedMenu(this);

	// no longer need doc & frame window interfaces
	ASSERT(m_pInPlaceFrame->m_lpFrame != NULL);
	_AfxRelease((LPUNKNOWN*)&m_pInPlaceFrame->m_lpFrame);
	m_pInPlaceFrame->m_hWndFrame = NULL;
	_AfxRelease((LPUNKNOWN*)&m_pInPlaceFrame->m_lpDocFrame);
	m_pInPlaceFrame->m_hWndDocFrame = NULL;
	DestroyInPlaceFrame(m_pInPlaceFrame);
	m_pInPlaceFrame = NULL;
		// destructor for COleIPFrameWnd or derivative should cleanup any
		//   toolbars etc. created during in-place activation

	// last of all, call IOleClientSite::InPlaceDeactivate
	ASSERT(m_lpClientSite != NULL);
	LPOLEINPLACESITE lpInPlaceSite =
		_AFXQUERYINTERFACE(m_lpClientSite, IOleInPlaceSite);
	if (lpInPlaceSite != NULL)
	{
		lpInPlaceSite->OnInPlaceDeactivate();
		lpInPlaceSite->Release();
	}
}

void COleServerDoc::OnDeactivateUI(BOOL bUndoable)
{
	ASSERT_VALID(this);

	// reset active object pointers
	ASSERT(m_pInPlaceFrame->m_lpFrame != NULL);
	m_pInPlaceFrame->m_lpFrame->SetActiveObject(NULL, NULL);
	if (m_pInPlaceFrame->m_lpDocFrame != NULL)
		m_pInPlaceFrame->m_lpDocFrame->SetActiveObject(NULL, NULL);

	// remove frame & document level frame controls
	ASSERT(m_pInPlaceFrame->m_lpFrame != NULL);
	OnShowControlBars(m_pInPlaceFrame->m_lpFrame, FALSE);
	if (m_pInPlaceFrame->m_lpDocFrame != NULL)
		OnShowControlBars(m_pInPlaceFrame->m_lpDocFrame, FALSE);

	// hide the frame and any popups owned by the frame
	m_pInPlaceFrame->ShowOwnedWindows(FALSE);
	m_pInPlaceFrame->ShowWindow(SW_HIDE);

	// no longer UI active...
	m_pInPlaceFrame->m_bUIActive = FALSE;
	CWinApp* pApp = AfxGetApp();
	if (pApp->m_pActiveWnd == m_pInPlaceFrame)
		pApp->m_pActiveWnd = NULL;

	// call IOleClientSite::OnUIDeactivate
	ASSERT(m_lpClientSite != NULL);
	LPOLEINPLACESITE lpInPlaceSite =
		_AFXQUERYINTERFACE(m_lpClientSite, IOleInPlaceSite);
	if (lpInPlaceSite != NULL)
	{
		lpInPlaceSite->OnUIDeactivate(bUndoable);
		lpInPlaceSite->Release();
	}

	// for release of visible lock (but don't destroy the object)
	UpdateVisibleLock(FALSE, FALSE);
}

void COleServerDoc::OnSetItemRects(LPCRECT lpPosRect, LPCRECT lpClipRect)
{
	ASSERT_VALID(this);
	ASSERT(AfxIsValidAddress(lpPosRect, sizeof(RECT), FALSE));
	ASSERT(AfxIsValidAddress(lpClipRect, sizeof(RECT), FALSE));

	if (m_pInPlaceFrame == NULL)
		return;
	ASSERT_VALID(m_pInPlaceFrame);

	// tell the frame to position itself such that the view is at the given
	//  rectangle (relative to the frame's parent)
	m_pInPlaceFrame->RepositionFrame(lpPosRect, lpClipRect);
}

BOOL COleServerDoc::OnReactivateAndUndo()
{
	// default implementation doesn't support undo

	return FALSE;
}

/////////////////////////////////////////////////////////////////////////////
// COleServerDoc special APIs for in-place editing

void COleServerDoc::RequestPositionChange(LPCRECT lpPosRect)
{
	ASSERT_VALID(this);
	ASSERT(AfxIsValidAddress(lpPosRect, sizeof(RECT), FALSE));

	// get IOleInPlaceSite interface
	ASSERT(m_lpClientSite != NULL);
	LPOLEINPLACESITE lpInPlaceSite =
		_AFXQUERYINTERFACE(m_lpClientSite, IOleInPlaceSite);

	if (lpInPlaceSite != NULL)
	{
		// call IOleInPlaceSite::OnPosRectChange
		lpInPlaceSite->OnPosRectChange(lpPosRect);
		lpInPlaceSite->Release();
	}
}

BOOL COleServerDoc::ScrollContainerBy(CSize sizeScroll)
{
	ASSERT_VALID(this);

	// get IOleInPlaceSite interface
	ASSERT(m_lpClientSite != NULL);
	LPOLEINPLACESITE lpInPlaceSite =
		_AFXQUERYINTERFACE(m_lpClientSite, IOleInPlaceSite);
	if (lpInPlaceSite == NULL)
		return FALSE;

	// call IOleInPlaceSite::Scroll
	BOOL bResult = lpInPlaceSite->Scroll(sizeScroll) == NOERROR;
	lpInPlaceSite->Release();
	return bResult;
}

BOOL COleServerDoc::DeactivateAndUndo()
{
	ASSERT_VALID(this);

	// get IOleInPlaceSite interface
	ASSERT(m_lpClientSite != NULL);
	LPOLEINPLACESITE lpInPlaceSite =
		_AFXQUERYINTERFACE(m_lpClientSite, IOleInPlaceSite);
	if (lpInPlaceSite == NULL)
		return FALSE;

	// call IOleInPlaceSite::DeactivateAndUndo
	BOOL bResult = lpInPlaceSite->DeactivateAndUndo() == NOERROR;
	lpInPlaceSite->Release();
	return bResult;
}

BOOL COleServerDoc::DiscardUndoState()
{
	ASSERT_VALID(this);

	// get IOleInPlaceSite interface
	ASSERT(m_lpClientSite != NULL);
	LPOLEINPLACESITE lpInPlaceSite =
		_AFXQUERYINTERFACE(m_lpClientSite, IOleInPlaceSite);
	if (lpInPlaceSite == NULL)
		return FALSE;

	// call IOleInPlaceSite::DiscardUndoState
	BOOL bResult = lpInPlaceSite->DiscardUndoState() == NOERROR;
	lpInPlaceSite->Release();
	return bResult;
}

/////////////////////////////////////////////////////////////////////////////
// COleServerDoc OLE interface implementation

BEGIN_INTERFACE_MAP(COleServerDoc, COleLinkingDoc)
	INTERFACE_PART(COleServerDoc, IID_IPersistStorage, PersistStorage)
	INTERFACE_PART(COleServerDoc, IID_IOleObject, OleObject)
	INTERFACE_PART(COleServerDoc, IID_IDataObject, DataObject)
	INTERFACE_PART(COleServerDoc, IID_IOleWindow, OleInPlaceObject)
	INTERFACE_PART(COleServerDoc, IID_IOleInPlaceObject, OleInPlaceObject)
	INTERFACE_PART(COleServerDoc, IID_IOleInPlaceActiveObject, OleInPlaceActiveObject)
END_INTERFACE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COleServerDoc::CPersistStorage

STDMETHODIMP_(ULONG) COleServerDoc::XPersistStorage::AddRef()
{
	METHOD_PROLOGUE(COleServerDoc, PersistStorage)
	return (ULONG)pThis->ExternalAddRef();
}

STDMETHODIMP_(ULONG) COleServerDoc::XPersistStorage::Release()
{
	METHOD_PROLOGUE(COleServerDoc, PersistStorage)
	return (ULONG)pThis->ExternalRelease();
}

STDMETHODIMP COleServerDoc::XPersistStorage::QueryInterface(
	REFIID iid, LPVOID far* ppvObj)
{
	METHOD_PROLOGUE(COleServerDoc, PersistStorage)
	return (HRESULT)pThis->ExternalQueryInterface(&iid, ppvObj);
}

STDMETHODIMP COleServerDoc::XPersistStorage::GetClassID(LPCLSID lpClassID)
{
	METHOD_PROLOGUE(COleServerDoc, PersistStorage)
	ASSERT_VALID(pThis);

	return pThis->m_xPersistFile.GetClassID(lpClassID);
}

STDMETHODIMP COleServerDoc::XPersistStorage::IsDirty()
{
	METHOD_PROLOGUE(COleServerDoc, PersistStorage)
	ASSERT_VALID(pThis);

	return pThis->m_xPersistFile.IsDirty();
}

STDMETHODIMP COleServerDoc::XPersistStorage::InitNew(LPSTORAGE pStg)
{
	METHOD_PROLOGUE(COleServerDoc, PersistStorage)
	ASSERT_VALID(pThis);

	SCODE sc;
	TRY
	{
		// delegate to member function in the document
		pThis->OnNewEmbedding(pStg);
		sc = S_OK;
	}
	CATCH_ALL(e)
	{
		sc = COleException::Process(e);
	}
	END_CATCH_ALL

	return ResultFromScode(sc);
}

STDMETHODIMP COleServerDoc::XPersistStorage::Load(LPSTORAGE pStg)
{
	METHOD_PROLOGUE(COleServerDoc, PersistStorage)
	ASSERT_VALID(pThis);

	SCODE sc;
	TRY
	{
		// delegate to member function in the document
		pThis->OnOpenEmbedding(pStg);
		sc = S_OK;
	}
	CATCH_ALL(e)
	{
		sc = COleException::Process(e);
	}
	END_CATCH_ALL

	return ResultFromScode(sc);
}

STDMETHODIMP COleServerDoc::XPersistStorage::Save(
	LPSTORAGE pStgSave, BOOL fSameAsLoad)
{
	METHOD_PROLOGUE(COleServerDoc, PersistStorage)
	ASSERT_VALID(pThis);

	// don't bother saving if destination is up-to-date
	if (fSameAsLoad && !pThis->IsModified())
		return NOERROR;

	SCODE sc;
	TRY
	{
		// delegate through the document
		ASSERT(pThis->m_bRemember);
		pThis->m_bRemember = FALSE;
		pThis->m_bSameAsLoad = fSameAsLoad;
		pThis->OnSaveEmbedding(pStgSave);

		// clear dirty flag since save to same storage successful
		if (fSameAsLoad)
		{
			pThis->SetModifiedFlag(FALSE);

			// notify clients that object has been saved
			pThis->NotifySaved();
		}
		sc = S_OK;
	}
	CATCH_ALL(e)
	{
		sc = COleException::Process(e);
	}
	END_CATCH_ALL

	// restore default state
	pThis->m_bRemember = TRUE;

	return ResultFromScode(sc);
}

STDMETHODIMP COleServerDoc::XPersistStorage::SaveCompleted(LPSTORAGE pStgSaved)
{
	METHOD_PROLOGUE(COleServerDoc, PersistStorage)
	ASSERT_VALID(pThis);

	// call SaveCompleted on any embedded items
	pThis->CommitItems(pStgSaved != NULL);

	// update state to reflect new storage
	if (pStgSaved != NULL)
	{
		// attach new storage
		pStgSaved->AddRef();
		_AfxRelease((LPUNKNOWN*)&pThis->m_lpRootStg);
		pThis->m_lpRootStg = pStgSaved;

		// now this document is storage based
		pThis->m_strPathName.Empty();
		pThis->SetModifiedFlag(FALSE);

		// notify clients that object has been saved
		pThis->NotifySaved();
	}

	return NOERROR;
}

STDMETHODIMP COleServerDoc::XPersistStorage::HandsOffStorage()
{
	METHOD_PROLOGUE(COleServerDoc, PersistStorage)
	ASSERT_VALID(pThis);

	if (pThis->m_lpRootStg != NULL)
	{
		// first call HandsOffStorage for all the embedded client items
		POSITION pos = pThis->GetStartPosition();
		COleClientItem* pItem;
		while ((pItem = pThis->GetNextClientItem(pos)) != NULL)
		{
			ASSERT(pItem->m_lpObject != NULL);
			LPPERSISTSTORAGE lpPersistStorage =
				_AFXQUERYINTERFACE(pItem->m_lpObject, IPersistStorage);
			ASSERT(lpPersistStorage != NULL);
			lpPersistStorage->HandsOffStorage();
			lpPersistStorage->Release();
		}

		// for now, can't access the storage
		_AfxRelease((LPUNKNOWN*)&pThis->m_lpRootStg);
	}

	return NOERROR;
}

/////////////////////////////////////////////////////////////////////////////
// COleServerDoc::XOleObject

STDMETHODIMP_(ULONG) COleServerDoc::XOleObject::AddRef()
{
	METHOD_PROLOGUE(COleServerDoc, OleObject)
	return (ULONG)pThis->ExternalAddRef();
}

STDMETHODIMP_(ULONG) COleServerDoc::XOleObject::Release()
{
	METHOD_PROLOGUE(COleServerDoc, OleObject)
	return (ULONG)pThis->ExternalRelease();
}

STDMETHODIMP COleServerDoc::XOleObject::QueryInterface(
	REFIID iid, LPVOID far* ppvObj)
{
	METHOD_PROLOGUE(COleServerDoc, OleObject)
	return (HRESULT)pThis->ExternalQueryInterface(&iid, ppvObj);
}

STDMETHODIMP COleServerDoc::XOleObject::SetClientSite(
	LPOLECLIENTSITE pClientSite)
{
	METHOD_PROLOGUE(COleServerDoc, OleObject)
	ASSERT_VALID(pThis);

	// maintain reference counts
	if (pClientSite != NULL)
		pClientSite->AddRef();
	_AfxRelease((LPUNKNOWN*)&pThis->m_lpClientSite);
	pThis->m_lpClientSite = pClientSite;

	return NOERROR;
}

STDMETHODIMP COleServerDoc::XOleObject::GetClientSite(
	LPOLECLIENTSITE FAR* ppClientSite)
{
	METHOD_PROLOGUE(COleServerDoc, OleObject)
	ASSERT_VALID(pThis);

	if (pThis->m_lpClientSite == NULL)
	{
		*ppClientSite = NULL;
		return ResultFromScode(E_FAIL);
	}

	*ppClientSite = pThis->m_lpClientSite;
	pThis->m_lpClientSite->AddRef();
	return NOERROR;
}

STDMETHODIMP COleServerDoc::XOleObject::SetHostNames(
	LPCSTR szContainerApp, LPCSTR szContainerObj)
{
	METHOD_PROLOGUE(COleServerDoc, OleObject)
	ASSERT_VALID(pThis);

	TRY
	{
		pThis->OnSetHostNames(szContainerApp, szContainerObj);
	}
	END_TRY

	return NOERROR;
}

STDMETHODIMP COleServerDoc::XOleObject::Close(DWORD dwSaveOption)
{
	METHOD_PROLOGUE(COleServerDoc, OleObject)
	ASSERT_VALID(pThis);

	pThis->InternalAddRef();    // protect this object

	SCODE sc;
	TRY
	{
		// delegate through document for most of the work
		pThis->OnClose((OLECLOSE)dwSaveOption);
		sc = S_OK;
	}
	CATCH_ALL(e)
	{
		sc = COleException::Process(e);
	}
	END_CATCH_ALL

	pThis->InternalRelease();   // may 'delete this'

	return ResultFromScode(sc);
}

STDMETHODIMP COleServerDoc::XOleObject::SetMoniker(
	DWORD dwWhichMoniker, LPMONIKER pmk)
{
	METHOD_PROLOGUE(COleServerDoc, OleObject)
	ASSERT_VALID(pThis);

	if (pThis->m_lpClientSite == NULL)
		return ResultFromScode(E_FAIL);

	// get current full moniker from client-site
	LPMONIKER lpMoniker = NULL;
	if (pThis->m_lpClientSite->GetMoniker(OLEGETMONIKER_ONLYIFTHERE,
		OLEWHICHMK_OBJFULL, &lpMoniker) != NOERROR)
	{
		// just to make sure -- always set moniker to NULL on failure
		lpMoniker = NULL;
	}

	// update all embedded items with new moniker
	POSITION pos = pThis->GetStartPosition();
	COleClientItem* pItem;
	while ((pItem = pThis->GetNextClientItem(pos)) != NULL)
	{
		if (pItem->m_bMoniker)
			pItem->m_lpObject->SetMoniker(OLEWHICHMK_CONTAINER, lpMoniker);
	}

	// send Rename advises
	pThis->NotifyAllItems(OLE_RENAMED, (DWORD)lpMoniker);
	_AfxRelease((LPUNKNOWN*)&lpMoniker);

	return NOERROR;
}

STDMETHODIMP COleServerDoc::XOleObject::GetMoniker(
	DWORD dwAssign, DWORD dwWhichMoniker, LPMONIKER FAR* ppMoniker)
{
	METHOD_PROLOGUE(COleServerDoc, OleObject)
	ASSERT_VALID(pThis);

	*ppMoniker = pThis->GetMoniker((OLEGETMONIKER)dwAssign);
	return *ppMoniker == NULL ? ResultFromScode(E_FAIL) : NOERROR;
}

STDMETHODIMP COleServerDoc::XOleObject::InitFromData(
	LPDATAOBJECT pDataObject, BOOL fCreation, DWORD dwReserved)
{
	METHOD_PROLOGUE(COleServerDoc, OleObject)
	ASSERT_VALID(pThis);

	COleServerItem* pItem;
	SCODE sc;
	TRY
	{
		// delegate through item
		pItem = pThis->GetEmbeddedItem();
		ASSERT_VALID(pItem);
		ASSERT(pItem->IsKindOf(RUNTIME_CLASS(COleServerItem)));

		COleDataObject dataObject;
		dataObject.Attach(pDataObject,  FALSE);
		sc = pItem->OnInitFromData(&dataObject, fCreation) ? S_OK : S_FALSE;
	}
	CATCH_ALL(e)
	{
		sc = COleException::Process(e);
	}
	END_CATCH_ALL

	return ResultFromScode(sc);
}

STDMETHODIMP COleServerDoc::XOleObject::GetClipboardData(
	DWORD dwReserved, LPDATAOBJECT FAR* ppDataObject)
{
	METHOD_PROLOGUE(COleServerDoc, OleObject)
	ASSERT_VALID(pThis);

	*ppDataObject = NULL;

	COleServerItem* pItem;
	SCODE sc;
	TRY
	{
		// delegate through item
		pItem = pThis->GetEmbeddedItem();
		ASSERT_VALID(pItem);
		ASSERT(pItem->IsKindOf(RUNTIME_CLASS(COleServerItem)));

		COleDataSource* pDataSource =
			pItem->OnGetClipboardData(TRUE, NULL, NULL);
		ASSERT(pDataSource != NULL);

		*ppDataObject =
			(LPDATAOBJECT)pDataSource->GetInterface(&IID_IDataObject);
		ASSERT(*ppDataObject != NULL);
		sc = S_OK;
	}
	CATCH_ALL(e)
	{
		sc = COleException::Process(e);
	}
	END_CATCH_ALL

	return ResultFromScode(sc);
}

STDMETHODIMP COleServerDoc::XOleObject::DoVerb(
	LONG iVerb, LPMSG lpmsg, LPOLECLIENTSITE pActiveSite, LONG lindex,
	HWND hwndParent, LPCRECT lpPosRect)
{
	METHOD_PROLOGUE(COleServerDoc, OleObject)
	ASSERT_VALID(pThis);

	pThis->InternalAddRef();    // protect this object

	COleServerItem* pItem;
	SCODE sc;
	TRY
	{
		// delegate through item
		pItem = pThis->GetEmbeddedItem();
		ASSERT_VALID(pItem);
		ASSERT(pItem->IsKindOf(RUNTIME_CLASS(COleServerItem)));

		pItem->OnDoVerb(iVerb);
		sc = S_OK;
	}
	CATCH_ALL(e)
	{
		sc = COleException::Process(e);
	}
	END_CATCH_ALL

	pThis->InternalRelease();   // may 'delete this'

	return NOERROR;
}

STDMETHODIMP COleServerDoc::XOleObject::EnumVerbs(
	IEnumOLEVERB FAR* FAR* ppenumOleVerb)
{
	METHOD_PROLOGUE(COleServerDoc, OleObject)
	ASSERT_VALID(pThis);

	*ppenumOleVerb = NULL;

	LPOLEOBJECT lpObject = (LPOLEOBJECT)pThis->GetInterface(&IID_IOleObject);
	ASSERT(lpObject != NULL);
	CLSID clsid;
	lpObject->GetUserClassID(&clsid);

	return OleRegEnumVerbs(clsid, ppenumOleVerb);
}

STDMETHODIMP COleServerDoc::XOleObject::Update()
{
	METHOD_PROLOGUE(COleServerDoc, OleObject)
	ASSERT_VALID(pThis);

	COleServerItem* pItem;
	SCODE sc;
	TRY
	{
		// delegate through item
		pItem = pThis->GetEmbeddedItem();
		ASSERT_VALID(pItem);
		ASSERT(pItem->IsKindOf(RUNTIME_CLASS(COleServerItem)));

		pItem->OnUpdateItems();
		sc = S_OK;
	}
	CATCH_ALL(e)
	{
		sc = COleException::Process(e);
	}
	END_CATCH_ALL

	return ResultFromScode(sc);
}

STDMETHODIMP COleServerDoc::XOleObject::IsUpToDate()
{
	METHOD_PROLOGUE(COleServerDoc, OleObject)
	ASSERT_VALID(pThis);

	COleServerItem* pItem;
	SCODE sc;
	TRY
	{
		// delegate through item
		pItem = pThis->GetEmbeddedItem();
		ASSERT_VALID(pItem);
		ASSERT(pItem->IsKindOf(RUNTIME_CLASS(COleServerItem)));

		sc = pItem->OnQueryUpdateItems() ? S_FALSE : S_OK;
	}
	CATCH_ALL(e)
	{
		sc = COleException::Process(e);
	}
	END_CATCH_ALL

	return ResultFromScode(sc);
}

STDMETHODIMP COleServerDoc::XOleObject::GetUserClassID(CLSID FAR* pClsid)
{
	METHOD_PROLOGUE(COleServerDoc, OleObject)
	ASSERT_VALID(pThis);

	return pThis->m_xPersistFile.GetClassID(pClsid);
}

STDMETHODIMP COleServerDoc::XOleObject::GetUserType(
	DWORD dwFormOfType, LPSTR FAR* ppszUserType)
{
	METHOD_PROLOGUE(COleServerDoc, OleObject)
	ASSERT_VALID(pThis);

	*ppszUserType = NULL;

	LPOLEOBJECT lpObject = (LPOLEOBJECT)pThis->GetInterface(&IID_IOleObject);
	ASSERT(lpObject != NULL);
	CLSID clsid;
	lpObject->GetUserClassID(&clsid);

	return OleRegGetUserType(clsid, dwFormOfType, ppszUserType);
}

STDMETHODIMP COleServerDoc::XOleObject::SetExtent(
	DWORD dwDrawAspect, LPSIZEL lpsizel)
{
	METHOD_PROLOGUE(COleServerDoc, OleObject)
	ASSERT_VALID(pThis);

	COleServerItem* pItem;
	SCODE sc = E_FAIL;
	TRY
	{
		// convert rectangle to a CSize and call item OnSetExtent
		pItem = pThis->GetEmbeddedItem();
		ASSERT_VALID(pItem);
		ASSERT(pItem->IsKindOf(RUNTIME_CLASS(COleServerItem)));

		CSize size((int)lpsizel->cx, (int)lpsizel->cy);
		if (pItem->OnSetExtent((DVASPECT)dwDrawAspect, size))
			sc = S_OK;
	}
	CATCH_ALL(e)
	{
		sc = COleException::Process(e);
	}
	END_CATCH_ALL

	return ResultFromScode(sc);
}

STDMETHODIMP COleServerDoc::XOleObject::GetExtent(
	DWORD dwDrawAspect, LPSIZEL lpsizel)
{
	METHOD_PROLOGUE(COleServerDoc, OleObject)
	ASSERT_VALID(pThis);

	COleServerItem* pItem;
	SCODE sc = E_INVALIDARG;
	TRY
	{
		pItem = pThis->GetEmbeddedItem();
		ASSERT_VALID(pItem);
		ASSERT(pItem->IsKindOf(RUNTIME_CLASS(COleServerItem)));

		// call to get regular windows CSize
		CSize size;
		if (pItem->OnGetExtent((DVASPECT)dwDrawAspect, size))
		{
			if (size.cy < 0)
				size.cy = -size.cy; // extents are always positive
			lpsizel->cx = size.cx;
			lpsizel->cy = size.cy;

			sc = S_OK;
		}
	}
	CATCH_ALL(e)
	{
		sc = COleException::Process(e);
	}
	END_CATCH_ALL

	return ResultFromScode(sc);
}

STDMETHODIMP COleServerDoc::XOleObject::Advise(
	IAdviseSink FAR* pAdvSink, DWORD FAR* pdwConnection)
{
	METHOD_PROLOGUE(COleServerDoc, OleObject)
	ASSERT_VALID(pThis);

	COleServerItem* pItem;
	SCODE sc = E_OUTOFMEMORY;
	TRY
	{
		pItem = pThis->GetEmbeddedItem();
		ASSERT_VALID(pItem);
		ASSERT(pItem->IsKindOf(RUNTIME_CLASS(COleServerItem)));
		sc = S_OK;
	}
	END_TRY

	if (sc != S_OK)
		return ResultFromScode(sc);

	return pItem->GetOleObject()->Advise(pAdvSink, pdwConnection);
}

STDMETHODIMP COleServerDoc::XOleObject::Unadvise(DWORD dwConnection)
{
	METHOD_PROLOGUE(COleServerDoc, OleObject)
	ASSERT_VALID(pThis);

	COleServerItem* pItem;
	SCODE sc = E_OUTOFMEMORY;
	TRY
	{
		pItem = pThis->GetEmbeddedItem();
		ASSERT_VALID(pItem);
		ASSERT(pItem->IsKindOf(RUNTIME_CLASS(COleServerItem)));
		sc = S_OK;
	}
	END_TRY

	if (sc != S_OK)
		return ResultFromScode(sc);

	return pItem->GetOleObject()->Unadvise(dwConnection);
}

STDMETHODIMP COleServerDoc::XOleObject::EnumAdvise(
	LPENUMSTATDATA FAR* ppenumAdvise)
{
	METHOD_PROLOGUE(COleServerDoc, OleObject)
	ASSERT_VALID(pThis);

	COleServerItem* pItem;
	SCODE sc = E_OUTOFMEMORY;
	TRY
	{
		pItem = pThis->GetEmbeddedItem();
		ASSERT_VALID(pItem);
		ASSERT(pItem->IsKindOf(RUNTIME_CLASS(COleServerItem)));
		sc = S_OK;
	}
	END_TRY

	if (sc != S_OK)
		return ResultFromScode(sc);

	return pItem->GetOleObject()->EnumAdvise(ppenumAdvise);
}

STDMETHODIMP COleServerDoc::XOleObject::GetMiscStatus(
	DWORD dwAspect, DWORD FAR* pdwStatus)
{
	METHOD_PROLOGUE(COleServerDoc, OleObject)
	ASSERT_VALID(pThis);

	*pdwStatus = 0;

	LPOLEOBJECT lpObject = (LPOLEOBJECT)pThis->GetInterface(&IID_IOleObject);
	ASSERT(lpObject != NULL);
	CLSID clsid;
	lpObject->GetUserClassID(&clsid);

	return OleRegGetMiscStatus(clsid, dwAspect, pdwStatus);
}

STDMETHODIMP COleServerDoc::XOleObject::SetColorScheme(LPLOGPALETTE lpLogpal)
{
	METHOD_PROLOGUE(COleServerDoc, OleObject)
	ASSERT_VALID(pThis);

	COleServerItem* pItem;
	SCODE sc = E_NOTIMPL;
	TRY
	{
		pItem = pThis->GetEmbeddedItem();
		ASSERT_VALID(pItem);
		ASSERT(pItem->IsKindOf(RUNTIME_CLASS(COleServerItem)));

		// delegate to embedded item
		if (pItem->OnSetColorScheme(lpLogpal))
			sc = S_OK;
	}
	END_TRY

	return ResultFromScode(sc);
}

/////////////////////////////////////////////////////////////////////////////
// COleServerDoc::XDataObject

STDMETHODIMP_(ULONG) COleServerDoc::XDataObject::AddRef()
{
	METHOD_PROLOGUE(COleServerDoc, DataObject)
	return (ULONG)pThis->ExternalAddRef();
}

STDMETHODIMP_(ULONG) COleServerDoc::XDataObject::Release()
{
	METHOD_PROLOGUE(COleServerDoc, DataObject)
	return (ULONG)pThis->ExternalRelease();
}

STDMETHODIMP COleServerDoc::XDataObject::QueryInterface(
	REFIID iid, LPVOID far* ppvObj)
{
	METHOD_PROLOGUE(COleServerDoc, DataObject)
	return (HRESULT)pThis->ExternalQueryInterface(&iid, ppvObj);
}

STDMETHODIMP COleServerDoc::XDataObject::GetData(
	LPFORMATETC lpFormatEtc, LPSTGMEDIUM lpStgMedium)
{
	METHOD_PROLOGUE(COleServerDoc, DataObject)
	ASSERT_VALID(pThis);

	COleServerItem* pItem;
	SCODE sc = E_OUTOFMEMORY;
	TRY
	{
		pItem = pThis->GetEmbeddedItem();
		ASSERT_VALID(pItem);
		ASSERT(pItem->IsKindOf(RUNTIME_CLASS(COleServerItem)));
		sc = S_OK;
	}
	END_TRY

	if (sc != S_OK)
		return ResultFromScode(sc);

	return pItem->GetDataObject()->GetData(lpFormatEtc, lpStgMedium);
}

STDMETHODIMP COleServerDoc::XDataObject::GetDataHere(
	LPFORMATETC lpFormatEtc, LPSTGMEDIUM lpStgMedium)
{
	METHOD_PROLOGUE(COleServerDoc, DataObject)
	ASSERT_VALID(pThis);

	COleServerItem* pItem;
	SCODE sc = E_OUTOFMEMORY;
	TRY
	{
		pItem = pThis->GetEmbeddedItem();
		ASSERT_VALID(pItem);
		ASSERT(pItem->IsKindOf(RUNTIME_CLASS(COleServerItem)));
		sc = S_OK;
	}
	END_TRY

	if (sc != S_OK)
		return ResultFromScode(sc);

	return pItem->GetDataObject()->GetDataHere(lpFormatEtc, lpStgMedium);
}

STDMETHODIMP COleServerDoc::XDataObject::QueryGetData(
	LPFORMATETC lpFormatEtc)
{
	METHOD_PROLOGUE(COleServerDoc, DataObject)
	ASSERT_VALID(pThis);

	COleServerItem* pItem;
	SCODE sc = E_OUTOFMEMORY;
	TRY
	{
		pItem = pThis->GetEmbeddedItem();
		ASSERT_VALID(pItem);
		ASSERT(pItem->IsKindOf(RUNTIME_CLASS(COleServerItem)));
		sc = S_OK;
	}
	END_TRY

	if (sc != S_OK)
		return ResultFromScode(sc);

	return pItem->GetDataObject()->QueryGetData(lpFormatEtc);
}

STDMETHODIMP COleServerDoc::XDataObject::GetCanonicalFormatEtc(
	LPFORMATETC lpFormatEtcIn, LPFORMATETC lpFormatEtcOut)
{
	// because we support the target-device (ptd) for server metafile format,
	//  all members of the FORMATETC are significant.

	return ResultFromScode(DATA_S_SAMEFORMATETC);
}

STDMETHODIMP COleServerDoc::XDataObject::SetData(
	LPFORMATETC lpFormatEtc, LPSTGMEDIUM lpStgMedium, BOOL fRelease)
{
	METHOD_PROLOGUE(COleServerDoc, DataObject)
	ASSERT_VALID(pThis);

	COleServerItem* pItem;
	SCODE sc = E_OUTOFMEMORY;
	TRY
	{
		pItem = pThis->GetEmbeddedItem();
		ASSERT_VALID(pItem);
		ASSERT(pItem->IsKindOf(RUNTIME_CLASS(COleServerItem)));
		sc = S_OK;
	}
	END_TRY

	if (sc != S_OK)
		return ResultFromScode(sc);

	return pItem->GetDataObject()->SetData(lpFormatEtc, lpStgMedium, fRelease);
}

STDMETHODIMP COleServerDoc::XDataObject::EnumFormatEtc(
	DWORD dwDirection, LPENUMFORMATETC FAR* ppenumFormatEtc)
{
	METHOD_PROLOGUE(COleServerDoc, DataObject)
	ASSERT_VALID(pThis);

	COleServerItem* pItem;
	SCODE sc = E_OUTOFMEMORY;
	TRY
	{
		pItem = pThis->GetEmbeddedItem();
		ASSERT_VALID(pItem);
		ASSERT(pItem->IsKindOf(RUNTIME_CLASS(COleServerItem)));
		sc = S_OK;
	}
	END_TRY

	if (sc != S_OK)
		return ResultFromScode(sc);

	return pItem->GetDataObject()->EnumFormatEtc(dwDirection, ppenumFormatEtc);
}

STDMETHODIMP COleServerDoc::XDataObject::DAdvise(
	FORMATETC FAR* pFormatetc, DWORD advf,
	LPADVISESINK pAdvSink, DWORD FAR* pdwConnection)
{
	METHOD_PROLOGUE(COleServerDoc, DataObject)
	ASSERT_VALID(pThis);

	COleServerItem* pItem;
	SCODE sc = E_OUTOFMEMORY;
	TRY
	{
		pItem = pThis->GetEmbeddedItem();
		ASSERT_VALID(pItem);
		ASSERT(pItem->IsKindOf(RUNTIME_CLASS(COleServerItem)));
		sc = S_OK;
	}
	END_TRY

	if (sc != S_OK)
		return ResultFromScode(sc);

	return pItem->GetDataObject()->DAdvise(pFormatetc, advf, pAdvSink,
		pdwConnection);
}

STDMETHODIMP COleServerDoc::XDataObject::DUnadvise(DWORD dwConnection)
{
	METHOD_PROLOGUE(COleServerDoc, DataObject)
	ASSERT_VALID(pThis);

	COleServerItem* pItem;
	SCODE sc = E_OUTOFMEMORY;
	TRY
	{
		pItem = pThis->GetEmbeddedItem();
		ASSERT_VALID(pItem);
		ASSERT(pItem->IsKindOf(RUNTIME_CLASS(COleServerItem)));
		sc = S_OK;
	}
	END_TRY

	if (sc != S_OK)
		return ResultFromScode(sc);

	return pItem->GetDataObject()->DUnadvise(dwConnection);
}

STDMETHODIMP COleServerDoc::XDataObject::EnumDAdvise(
	LPENUMSTATDATA FAR* ppenumAdvise)
{
	METHOD_PROLOGUE(COleServerDoc, DataObject)
	ASSERT_VALID(pThis);

	COleServerItem* pItem;
	SCODE sc = E_OUTOFMEMORY;
	TRY
	{
		pItem = pThis->GetEmbeddedItem();
		ASSERT_VALID(pItem);
		ASSERT(pItem->IsKindOf(RUNTIME_CLASS(COleServerItem)));
		sc = S_OK;
	}
	END_TRY

	if (sc != S_OK)
		return ResultFromScode(sc);

	return pItem->GetDataObject()->EnumDAdvise(ppenumAdvise);
}

/////////////////////////////////////////////////////////////////////////////
// COleServerDoc::COleInPlaceObject

STDMETHODIMP_(ULONG) COleServerDoc::XOleInPlaceObject::AddRef()
{
	METHOD_PROLOGUE(COleServerDoc, OleInPlaceObject)
	return (ULONG)pThis->ExternalAddRef();
}

STDMETHODIMP_(ULONG) COleServerDoc::XOleInPlaceObject::Release()
{
	METHOD_PROLOGUE(COleServerDoc, OleInPlaceObject)
	return (ULONG)pThis->ExternalRelease();
}

STDMETHODIMP COleServerDoc::XOleInPlaceObject::QueryInterface(
	REFIID iid, LPVOID far* ppvObj)
{
	METHOD_PROLOGUE(COleServerDoc, OleInPlaceObject)
	return (HRESULT)pThis->ExternalQueryInterface(&iid, ppvObj);
}

STDMETHODIMP COleServerDoc::XOleInPlaceObject::GetWindow(HWND FAR* lphwnd)
{
	METHOD_PROLOGUE(COleServerDoc, OleInPlaceObject)
	ASSERT_VALID(pThis);

	LPOLEINPLACEACTIVEOBJECT lpActiveObject = (LPOLEINPLACEACTIVEOBJECT)
		pThis->GetInterface(&IID_IOleInPlaceActiveObject);
	return lpActiveObject->GetWindow(lphwnd);
}

STDMETHODIMP COleServerDoc::XOleInPlaceObject::ContextSensitiveHelp(
	BOOL fEnterMode)
{
	METHOD_PROLOGUE(COleServerDoc, OleInPlaceObject)
	ASSERT_VALID(pThis);

	LPOLEINPLACEACTIVEOBJECT lpActiveObject = (LPOLEINPLACEACTIVEOBJECT)
		pThis->GetInterface(&IID_IOleInPlaceActiveObject);
	return lpActiveObject->ContextSensitiveHelp(fEnterMode);
}

STDMETHODIMP COleServerDoc::XOleInPlaceObject::InPlaceDeactivate()
{
	METHOD_PROLOGUE(COleServerDoc, OleInPlaceObject)
	ASSERT_VALID(pThis);

	pThis->InternalAddRef();    // protect this object

	SCODE sc = E_UNEXPECTED;
	TRY
	{
		// only call deactivate if necessary
		if (pThis->m_pInPlaceFrame != NULL)
			pThis->OnDeactivate();

		// should be completely inactive
		ASSERT(pThis->m_pInPlaceFrame == NULL);
		sc = S_OK;
	}
	END_TRY

	pThis->InternalRelease();   // may 'delete this'
	return ResultFromScode(sc);
}

STDMETHODIMP COleServerDoc::XOleInPlaceObject::UIDeactivate()
{
	METHOD_PROLOGUE(COleServerDoc, OleInPlaceObject)
	ASSERT_VALID(pThis);

	pThis->InternalAddRef();    // protect this object

	SCODE sc = E_UNEXPECTED;
	TRY
	{
		// only call OnUIDeactivate if necessary
		if (pThis->m_pInPlaceFrame != NULL &&
			pThis->m_pInPlaceFrame->m_bUIActive)
		{
			pThis->OnDeactivateUI(FALSE);   // default to not undoable
		}

		// should not be ui active
		ASSERT(pThis->m_pInPlaceFrame == NULL ||
			!pThis->m_pInPlaceFrame->m_bUIActive);
		sc = S_OK;
	}
	END_TRY

	pThis->InternalRelease();   // may 'delete this'
	return ResultFromScode(sc);
}

STDMETHODIMP COleServerDoc::XOleInPlaceObject::SetObjectRects(
	LPCRECT lpPosRect, LPCRECT lpClipRect)
{
	METHOD_PROLOGUE(COleServerDoc, OleInPlaceObject)
	ASSERT_VALID(pThis);

	SCODE sc = E_UNEXPECTED;
	TRY
	{
		pThis->OnSetItemRects(lpPosRect, lpClipRect);
		sc = S_OK;
	}
	END_TRY

	return ResultFromScode(sc);
}

STDMETHODIMP COleServerDoc::XOleInPlaceObject::ReactivateAndUndo()
{
	METHOD_PROLOGUE(COleServerDoc, OleInPlaceObject)
	ASSERT_VALID(pThis);

	SCODE sc = E_UNEXPECTED;
	TRY
	{
		sc = pThis->OnReactivateAndUndo() ? S_OK : INPLACE_E_NOTUNDOABLE;
	}
	END_TRY

	return ResultFromScode(sc);
}

/////////////////////////////////////////////////////////////////////////////
// COleServerDoc::XOleInPlaceActiveObject

STDMETHODIMP_(ULONG) COleServerDoc::XOleInPlaceActiveObject::AddRef()
{
	METHOD_PROLOGUE(COleServerDoc, OleInPlaceActiveObject)
	return (ULONG)pThis->ExternalAddRef();
}

STDMETHODIMP_(ULONG) COleServerDoc::XOleInPlaceActiveObject::Release()
{
	METHOD_PROLOGUE(COleServerDoc, OleInPlaceActiveObject)
	return (ULONG)pThis->ExternalRelease();
}

STDMETHODIMP COleServerDoc::XOleInPlaceActiveObject::QueryInterface(
	REFIID iid, LPVOID far* ppvObj)
{
	METHOD_PROLOGUE(COleServerDoc, OleInPlaceActiveObject)
	return (HRESULT)pThis->ExternalQueryInterface(&iid, ppvObj);
}

STDMETHODIMP COleServerDoc::XOleInPlaceActiveObject::GetWindow(
	HWND FAR* lphwnd)
{
	METHOD_PROLOGUE(COleServerDoc, OleInPlaceActiveObject)
	ASSERT_VALID(pThis);

	*lphwnd = pThis->m_pInPlaceFrame->GetSafeHwnd();
	return *lphwnd != NULL ? NOERROR : ResultFromScode(E_FAIL);
}

STDMETHODIMP COleServerDoc::XOleInPlaceActiveObject::ContextSensitiveHelp(
	BOOL fEnterMode)
{
	METHOD_PROLOGUE(COleServerDoc, OleInPlaceActiveObject)
	ASSERT_VALID(pThis);

	if (fEnterMode)
	{
		if (!pThis->m_pInPlaceFrame->m_bHelpMode)
		{
			// check if help mode probable
			if (!pThis->m_pInPlaceFrame->CanEnterHelpMode())
				return ResultFromScode(E_UNEXPECTED);

			// attempt to enter context help
			if (!pThis->m_pInPlaceFrame->PostMessage(WM_COMMAND, ID_CONTEXT_HELP))
				return ResultFromScode(E_UNEXPECTED);
		}
	}
	else
	{
		// just exit help mode
		pThis->m_pInPlaceFrame->ExitHelpMode();
	}

	return NOERROR;
}

STDMETHODIMP COleServerDoc::XOleInPlaceActiveObject::TranslateAccelerator(
	LPMSG lpmsg)
{
	METHOD_PROLOGUE(COleServerDoc, OleInPlaceActiveObject)
	ASSERT_VALID(pThis);

	pThis->InternalAddRef();    // protect this object

	SCODE sc = E_UNEXPECTED;
	TRY
	{
		// get frame window for this document
		CFrameWnd* pFrameWnd = pThis->m_pInPlaceFrame;
		ASSERT_VALID(pFrameWnd);

		// attempt translate accelerator
		MSG msg = *lpmsg;
		sc = pFrameWnd->PreTranslateMessage(&msg) ? S_OK : S_FALSE;
		*lpmsg = msg;
	}
	END_TRY

	pThis->InternalRelease();   // may 'delete this'

	return ResultFromScode(sc);
}

STDMETHODIMP COleServerDoc::XOleInPlaceActiveObject::OnFrameWindowActivate(
	BOOL fActivate)
{
	METHOD_PROLOGUE(COleServerDoc, OleInPlaceActiveObject)
	ASSERT_VALID(pThis);

	SCODE sc = E_UNEXPECTED;
	TRY
	{
		pThis->OnFrameWindowActivate(fActivate);
		sc = S_OK;
	}
	END_TRY

	return ResultFromScode(sc);
}

STDMETHODIMP COleServerDoc::XOleInPlaceActiveObject::OnDocWindowActivate(
	BOOL fActivate)
{
	METHOD_PROLOGUE(COleServerDoc, OleInPlaceActiveObject)
	ASSERT_VALID(pThis);

	SCODE sc = E_UNEXPECTED;
	TRY
	{
		pThis->OnDocWindowActivate(fActivate);
		sc = S_OK;
	}
	END_TRY

	return ResultFromScode(sc);
}

STDMETHODIMP COleServerDoc::XOleInPlaceActiveObject::ResizeBorder(
	LPCRECT lprectBorder, LPOLEINPLACEUIWINDOW lpUIWindow, BOOL fFrameWindow)
{
	METHOD_PROLOGUE(COleServerDoc, OleInPlaceActiveObject)
	ASSERT_VALID(pThis);

	SCODE sc = E_UNEXPECTED;
	TRY
	{
		pThis->OnResizeBorder(lprectBorder, lpUIWindow, fFrameWindow);
		sc = S_OK;
	}
	END_TRY

	return ResultFromScode(sc);
}

STDMETHODIMP COleServerDoc::XOleInPlaceActiveObject::EnableModeless(
	BOOL fEnable)
{
	METHOD_PROLOGUE(COleServerDoc, OleInPlaceActiveObject)
	ASSERT_VALID(pThis);

	SCODE sc = E_UNEXPECTED;
	TRY
	{
		if (!fEnable)
		{
			// start modal state if not in modal state
			if (!pThis->m_pInPlaceFrame->InModalState())
				pThis->m_pInPlaceFrame->BeginModalState();
		}
		else
		{
			// end modal state if in modal state
			if (pThis->m_pInPlaceFrame->InModalState())
				pThis->m_pInPlaceFrame->EndModalState();
		}
		sc = S_OK;
	}
	END_TRY

	return ResultFromScode(sc);
}

//////////////////////////////////////////////////////////////////////////////
// Diagnostics

#ifdef _DEBUG
void COleServerDoc::AssertValid() const
{
	COleLinkingDoc::AssertValid();
	if (m_pInPlaceFrame != NULL)
		m_pInPlaceFrame->AssertValid();
	if (m_pOrigParent != NULL)
		m_pOrigParent->AssertValid();
}

void COleServerDoc::Dump(CDumpContext& dc) const
{
	COleLinkingDoc::Dump(dc);
	AFX_DUMP1(dc, "\nm_lpClientSite = ", m_lpClientSite);
	AFX_DUMP1(dc, "\nm_strHostObj = ", m_strHostObj);
	AFX_DUMP1(dc, "\nm_bCntrVisible = ", m_bCntrVisible);
	if (m_pInPlaceFrame != NULL)
		AFX_DUMP1(dc, "\nwith in-place frame ", m_pInPlaceFrame);
	else
		AFX_DUMP0(dc, "\nwith no in-place frame");
	if (m_pOrigParent != NULL)
		AFX_DUMP1(dc, "\nwith original parent ", m_pOrigParent);
	else
		AFX_DUMP0(dc, "\nwith no original parent");
	AFX_DUMP1(dc, "\nm_dwOrigStyle = ", m_dwOrigStyle);
}
#endif //_DEBUG

//////////////////////////////////////////////////////////////////////////////
// Inline function declarations expanded out-of-line

#ifndef _AFX_ENABLE_INLINES

// expand inlines for OLE server APIs
static char BASED_CODE _szAfxOleInl[] = "afxole.inl";
#undef THIS_FILE
#define THIS_FILE _szAfxOleInl
#define _AFXOLESVR_INLINE
#include "afxole.inl"

#endif //!_AFX_ENABLE_INLINES

/////////////////////////////////////////////////////////////////////////////
