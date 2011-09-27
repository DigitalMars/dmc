// This is a part of the Microsoft Foundation Classes C++ library.
// Copyright (C) 1992-1993 Microsoft Corporation
// All rights reserved.
//
// This source code is only intended as a supplement to the
// Microsoft Foundation Classes Reference and Microsoft
// QuickHelp and/or WinHelp documentation provided with the library.
// See these sources for detailed information regarding the
// Microsoft Foundation Classes product.

#include "stdafx.h"

#ifdef AFX_CORE2_SEG
#pragma code_seg(AFX_CORE2_SEG)
#endif

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDocTemplate

IMPLEMENT_DYNAMIC(CDocTemplate, CCmdTarget)

/////////////////////////////////////////////////////////////////////////////
// CDocTemplate construction/destruction

CDocTemplate::CDocTemplate(UINT nIDResource, CRuntimeClass* pDocClass,
	CRuntimeClass* pFrameClass, CRuntimeClass* pViewClass)
{
	ASSERT_VALID_IDR(nIDResource);
	ASSERT(pFrameClass != NULL);

	m_nIDResource = nIDResource;
	m_pDocClass = pDocClass;
	m_pFrameClass = pFrameClass;
	m_pViewClass = pViewClass;
	m_pOleFrameClass = NULL;
	m_pOleViewClass = NULL;

	m_pAttachedFactory = NULL;
	m_hMenuInPlace = NULL;
	m_hAccelInPlace = NULL;
	m_hMenuEmbedding = NULL;
	m_hAccelEmbedding = NULL;
	m_hMenuInPlaceServer = NULL;
	m_hAccelInPlaceServer = NULL;

	if (!m_strDocStrings.LoadString(m_nIDResource))
	{
		TRACE1("Warning: no document names in string for template #%d\n",
			nIDResource);
	}
}

void CDocTemplate::SetServerInfo(UINT nIDOleEmbedding, UINT nIDOleInPlaceServer,
	CRuntimeClass* pOleFrameClass, CRuntimeClass* pOleViewClass)
{
	if (nIDOleEmbedding != 0)
	{
		// load menu to be used while editing an embedding (as a server)
		HINSTANCE hInst = AfxFindResourceHandle(
			MAKEINTRESOURCE(nIDOleEmbedding), RT_MENU);
		m_hMenuEmbedding = ::LoadMenu(hInst, MAKEINTRESOURCE(nIDOleEmbedding));
		m_hAccelEmbedding = ::LoadAccelerators(hInst, MAKEINTRESOURCE(nIDOleEmbedding));
	}

	if (nIDOleInPlaceServer != 0)
	{
		// load menu to be used while editing in-place (as a server)
		HINSTANCE hInst = AfxFindResourceHandle(
			MAKEINTRESOURCE(nIDOleInPlaceServer), RT_MENU);
		m_hMenuInPlaceServer = ::LoadMenu(hInst,
			MAKEINTRESOURCE(nIDOleInPlaceServer));
		m_hAccelInPlaceServer = ::LoadAccelerators(hInst,
			MAKEINTRESOURCE(nIDOleInPlaceServer));
	}
	m_pOleFrameClass = pOleFrameClass;
	m_pOleViewClass = pOleViewClass;
	m_nIDServerResource = nIDOleInPlaceServer;
}

void CDocTemplate::SetContainerInfo(UINT nIDOleInPlaceContainer)
{
	ASSERT(nIDOleInPlaceContainer != 0);

	// load menu to be used while in-place editing session (as a container)
	HINSTANCE hInst = AfxFindResourceHandle(
		MAKEINTRESOURCE(nIDOleInPlaceContainer), RT_MENU);
	m_hMenuInPlace = ::LoadMenu(hInst,
		MAKEINTRESOURCE(nIDOleInPlaceContainer));
	m_hAccelInPlace = ::LoadAccelerators(hInst,
		MAKEINTRESOURCE(nIDOleInPlaceContainer));
}

CDocTemplate::~CDocTemplate()
{
	// delete OLE resources
	if (m_hMenuInPlace != NULL)
		::DestroyMenu(m_hMenuInPlace);
	if (m_hAccelInPlace != NULL)
		::FreeResource(m_hAccelInPlace);
	if (m_hMenuEmbedding != NULL)
		::DestroyMenu(m_hMenuEmbedding);
	if (m_hAccelEmbedding != NULL)
		::FreeResource(m_hAccelEmbedding);
	if (m_hMenuInPlaceServer != NULL)
		::DestroyMenu(m_hMenuInPlaceServer);
	if (m_hAccelInPlaceServer != NULL)
		::FreeResource(m_hAccelInPlaceServer);
}

/////////////////////////////////////////////////////////////////////////////
// CDocTemplate attributes

BOOL CDocTemplate::GetDocString(CString& rString, enum DocStringIndex i) const
{
	return AfxExtractSubString(rString, m_strDocStrings, (int)i);
}

/////////////////////////////////////////////////////////////////////////////
// Document management

void CDocTemplate::AddDocument(CDocument* pDoc)
{
	ASSERT_VALID(pDoc);
	ASSERT(pDoc->m_pDocTemplate == NULL);   // no template attached yet
	pDoc->m_pDocTemplate = this;
}

void CDocTemplate::RemoveDocument(CDocument* pDoc)
{
	ASSERT_VALID(pDoc);
	ASSERT(pDoc->m_pDocTemplate == this);   // must be attached to us
	pDoc->m_pDocTemplate = NULL;
}

CDocTemplate::Confidence CDocTemplate::MatchDocType(const char* pszPathName,
	CDocument*& rpDocMatch)
{
	ASSERT(pszPathName != NULL);
	rpDocMatch = NULL;

	// go through all documents
	POSITION pos = GetFirstDocPosition();
	while (pos != NULL)
	{
		CDocument* pDoc = GetNextDoc(pos);
		if (pDoc->GetPathName() == pszPathName)
		{
			// already open
			rpDocMatch = pDoc;
			return yesAlreadyOpen;
		}
	}

	// see if it matches our default suffix
	CString strFilterExt;
	if (GetDocString(strFilterExt, CDocTemplate::filterExt) &&
	  !strFilterExt.IsEmpty())
	{
		// see if extension matches
		ASSERT(strFilterExt[0] == '.');
		int nDot = CString(pszPathName).ReverseFind('.');
		const char* pszDot = nDot < 0 ? NULL : pszPathName + nDot;
		if (nDot >= 0 && lstrcmpi(pszPathName+nDot, strFilterExt) == 0)
			return yesAttemptNative; // extension matches, looks like ours
	}

	// otherwise we will guess it may work
	return yesAttemptForeign;
}


CDocument* CDocTemplate::CreateNewDocument()
{
	// default implementation constructs one from CRuntimeClass
	if (m_pDocClass == NULL)
	{
		TRACE0("Error: you must override CDocTemplate::CreateNewDocument\n");
		ASSERT(FALSE);
		return NULL;
	}
	CDocument* pDocument = (CDocument*)m_pDocClass->CreateObject();
	if (pDocument == NULL)
	{
		TRACE1("Warning: Dynamic create of document type %Fs failed\n",
			m_pDocClass->m_lpszClassName);
		return NULL;
	}
	ASSERT(pDocument->IsKindOf(RUNTIME_CLASS(CDocument)));
	AddDocument(pDocument);
	return pDocument;
}


/////////////////////////////////////////////////////////////////////////////
// Default frame creation

CFrameWnd* CDocTemplate::CreateNewFrame(CDocument* pDoc, CFrameWnd* pOther)
{
	if (pDoc != NULL)
		ASSERT_VALID(pDoc);
	// create a frame wired to the specified document

	ASSERT(m_nIDResource != 0); // must have a resource ID to load from
	CCreateContext context;
	context.m_pCurrentFrame = pOther;
	context.m_pCurrentDoc = pDoc;
	context.m_pNewViewClass = m_pViewClass;
	context.m_pNewDocTemplate = this;

	if (m_pFrameClass == NULL)
	{
		TRACE0("Error: you must override CDocTemplate::CreateNewFrame\n");
		ASSERT(FALSE);
		return NULL;
	}
	CFrameWnd* pFrame = (CFrameWnd*)m_pFrameClass->CreateObject();
	if (pFrame == NULL)
	{
		TRACE1("Warning: Dynamic create of frame %Fs failed\n",
			m_pFrameClass->m_lpszClassName);
		return NULL;
	}
	ASSERT(pFrame->IsKindOf(RUNTIME_CLASS(CFrameWnd)));

	if (context.m_pNewViewClass == NULL)
		TRACE0("Warning: creating frame with no default view\n");

	// create new from resource
	if (!pFrame->LoadFrame(m_nIDResource,
			WS_OVERLAPPEDWINDOW | FWS_ADDTOTITLE,   // default frame styles
			NULL, &context))
	{
		TRACE0("Warning: CDocTemplate couldn't create a frame\n");
		// frame will be deleted in PostNcDestroy cleanup
		return NULL;
	}

	// it worked !
	return pFrame;
}

CFrameWnd* CDocTemplate::CreateOleFrame(CWnd* pParentWnd, CDocument* pDoc,
	BOOL bCreateView)
{
	CCreateContext context;
	context.m_pCurrentFrame = NULL;
	context.m_pCurrentDoc = pDoc;
	context.m_pNewViewClass = bCreateView ? m_pOleViewClass : NULL;
	context.m_pNewDocTemplate = this;

	if (m_pOleFrameClass == NULL)
	{
		TRACE0("Warning: pOleFrameClass not specified for doc template.\n"
			   "\tPerhaps CDocTemplate::SetServerInfo was not called?\n");
		return NULL;
	}

	ASSERT(m_nIDServerResource != 0); // must have a resource ID to load from
	CFrameWnd* pFrame = (CFrameWnd*)m_pOleFrameClass->CreateObject();
	if (pFrame == NULL)
	{
		TRACE1("Warning: Dynamic create of frame %Fs failed\n",
			m_pOleFrameClass->m_lpszClassName);
		return NULL;
	}

	// create new from resource (OLE frames are created as child windows)
	if (!pFrame->LoadFrame(m_nIDServerResource,
		WS_CHILD|WS_CLIPSIBLINGS, pParentWnd, &context))
	{
		TRACE0("Warning: CDocTemplate couldn't create an OLE frame.\n");
		// frame will be deleted in PostNcDestroy cleanup
		return NULL;
	}

	// it worked !
	return pFrame;
}

void CDocTemplate::InitialUpdateFrame(CFrameWnd* pFrame, CDocument* pDoc,
	BOOL bMakeVisible)
{
	// if the frame does not have an active view, set to first pane
	CView* pView = NULL;
	if (pFrame->GetActiveView() == NULL)
	{
		CWnd* pWnd = pFrame->GetDescendantWindow(AFX_IDW_PANE_FIRST, TRUE);
		if (pWnd != NULL && pWnd->IsKindOf(RUNTIME_CLASS(CView)))
		{
			pView = (CView*)pWnd;
			pFrame->SetActiveView(pView, FALSE);
		}
	}

	if (bMakeVisible)
	{
		// send initial update to all views (and other controls) in the frame
		pFrame->SendMessageToDescendants(WM_INITIALUPDATE, 0, 0, TRUE, TRUE);

		// give view a chance to save the focus (CFormView needs this)
		if (pView != NULL)
			pView->OnActivateFrame(WA_INACTIVE, pFrame);

		// finally, activate the frame
		// (send the default show command unless the main desktop window)
		int nCmdShow = -1;      // default
		if (pFrame == AfxGetApp()->m_pMainWnd)
			nCmdShow = AfxGetApp()->m_nCmdShow; // use the parameter from WinMain
		pFrame->ActivateFrame(nCmdShow);
		if (pView != NULL)
			pView->OnActivateView(TRUE, pView, pView);
	}

	// update frame counts and frame title (may already have been visible)
	if (pDoc != NULL)
		pDoc->UpdateFrameCounts();
	pFrame->OnUpdateFrameTitle(TRUE);

	// at this point the frame should be properly linked to the document
	ASSERT(pFrame->GetActiveDocument() == pDoc);
}

/////////////////////////////////////////////////////////////////////////////
// CDocTemplate commands and command helpers

BOOL CDocTemplate::SaveAllModified()
{
	POSITION pos = GetFirstDocPosition();
	while (pos != NULL)
	{
		CDocument* pDoc = GetNextDoc(pos);
		if (!pDoc->SaveModified())
			return FALSE;
	}
	return TRUE;
}


void CDocTemplate::CloseAllDocuments(BOOL bEndSession)
{
	POSITION pos = GetFirstDocPosition();
	while (pos != NULL)
	{
		CDocument* pDoc = GetNextDoc(pos);
		if (bEndSession)
			pDoc->DisconnectViews();
		pDoc->OnCloseDocument();
	}
}

void CDocTemplate::OnIdle()
{
	POSITION pos = GetFirstDocPosition();
	while (pos != NULL)
	{
		CDocument* pDoc = GetNextDoc(pos);
		ASSERT_VALID(pDoc);
		ASSERT(pDoc->IsKindOf(RUNTIME_CLASS(CDocument)));
		pDoc->OnIdle();
	}
}

/////////////////////////////////////////////////////////////////////////////
// CDocTemplate diagnostics

#ifdef _DEBUG
void CDocTemplate::Dump(CDumpContext& dc) const
{
	CCmdTarget::Dump(dc);

	AFX_DUMP1(dc, "\nm_nIDResource = ", m_nIDResource);
	AFX_DUMP1(dc, "\nm_strDocStrings: ", m_strDocStrings);
	if (m_pDocClass)
		AFX_DUMP1(dc, "\nm_pDocClass = ", m_pDocClass->m_lpszClassName);
	else
		AFX_DUMP0(dc, "\nm_pDocClass = NULL");

	if (dc.GetDepth() > 0)
	{
		AFX_DUMP0(dc, "\ndocument list = {");
		POSITION pos = GetFirstDocPosition();
		while (pos != NULL)
		{
			CDocument* pDoc = GetNextDoc(pos);
			AFX_DUMP1(dc, "\ndocument ", pDoc);
		}
		AFX_DUMP0(dc, "\n}");
	}
}

void CDocTemplate::AssertValid() const
{
	CCmdTarget::AssertValid();

	POSITION pos = GetFirstDocPosition();
	while (pos != NULL)
	{
		CDocument* pDoc = GetNextDoc(pos);
		ASSERT_VALID(pDoc);
	}
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
