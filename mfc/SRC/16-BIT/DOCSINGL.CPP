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
// CSingleDocTemplate

IMPLEMENT_DYNAMIC(CSingleDocTemplate, CDocTemplate)

/////////////////////////////////////////////////////////////////////////////
// CSingleDocTemplate construction/destruction

CSingleDocTemplate::CSingleDocTemplate(UINT nIDResource,
	CRuntimeClass* pDocClass, CRuntimeClass* pFrameClass,
	CRuntimeClass* pViewClass)
		: CDocTemplate(nIDResource, pDocClass, pFrameClass, pViewClass)
{
	m_pOnlyDoc = NULL;
}

CSingleDocTemplate::~CSingleDocTemplate()
{
#ifdef _DEBUG
	if (m_pOnlyDoc != NULL)
		TRACE0("Warning: destroying CSingleDocTemplate with live document\n");
#endif
}

/////////////////////////////////////////////////////////////////////////////
// CSingleDocTemplate attributes

POSITION CSingleDocTemplate::GetFirstDocPosition() const
{
	return (m_pOnlyDoc == NULL) ? NULL : BEFORE_START_POSITION;
}

CDocument* CSingleDocTemplate::GetNextDoc(POSITION& rPos) const
{
	CDocument* pDoc = NULL;
	if (rPos == BEFORE_START_POSITION)
	{
		// first time through, return a real document
		ASSERT(m_pOnlyDoc != NULL);
		pDoc = m_pOnlyDoc;
	}
	rPos = NULL;        // no more
	return pDoc;
}

/////////////////////////////////////////////////////////////////////////////
// CSingleDocTemplate document management (a list of currently open documents)

void CSingleDocTemplate::AddDocument(CDocument* pDoc)
{
	ASSERT(m_pOnlyDoc == NULL);     // one at a time please
	ASSERT_VALID(pDoc);

	CDocTemplate::AddDocument(pDoc);
	m_pOnlyDoc = pDoc;
}

void CSingleDocTemplate::RemoveDocument(CDocument* pDoc)
{
	ASSERT(m_pOnlyDoc == pDoc);     // must be this one
	ASSERT_VALID(pDoc);

	CDocTemplate::RemoveDocument(pDoc);
	m_pOnlyDoc = NULL;
}

/////////////////////////////////////////////////////////////////////////////
// CSingleDocTemplate commands

CDocument* CSingleDocTemplate::OpenDocumentFile(const char* pszPathName,
	BOOL bMakeVisible)
	// if pszPathName == NULL => create new file of this type
{
	CDocument* pDocument = NULL;
	CFrameWnd* pFrame = NULL;
	BOOL bCreated = FALSE;      // => doc and frame created
	BOOL bWasModified = FALSE;

	if (m_pOnlyDoc != NULL)
	{
		// already have a document - reinit it
		pDocument = m_pOnlyDoc;
		if (!pDocument->SaveModified())
			return NULL;        // leave the original one

		pFrame = (CFrameWnd*)AfxGetApp()->m_pMainWnd;
		ASSERT(pFrame != NULL);
		ASSERT(pFrame->IsKindOf(RUNTIME_CLASS(CFrameWnd)));
		ASSERT_VALID(pFrame);
	}
	else
	{
		// create a new document
		pDocument = CreateNewDocument();
		ASSERT(pFrame == NULL);     // will be created below
		bCreated = TRUE;
	}

	if (pDocument == NULL)
	{
		AfxMessageBox(AFX_IDP_FAILED_TO_CREATE_DOC);
		return NULL;
	}
	ASSERT(pDocument == m_pOnlyDoc);

	if (pFrame == NULL)
	{
		ASSERT(bCreated);

		// create frame - set as main document frame
		BOOL bAutoDelete = pDocument->m_bAutoDelete;
		pDocument->m_bAutoDelete = FALSE;
					// don't destroy if something goes wrong
		pFrame = CreateNewFrame(pDocument, NULL);
		pDocument->m_bAutoDelete = bAutoDelete;
		if (pFrame == NULL)
		{
			AfxMessageBox(AFX_IDP_FAILED_TO_CREATE_DOC);
			delete pDocument;       // explicit delete on error
			return NULL;
		}
	}

	if (pszPathName == NULL)
	{
		// create a new document
		CString strDocName;
		if (!GetDocString(strDocName, CDocTemplate::docName) ||
			strDocName.IsEmpty())
		{
			// use generic 'untitled'
			VERIFY(strDocName.LoadString(AFX_IDS_UNTITLED));
		}
		pDocument->SetTitle(strDocName);

		if (!pDocument->OnNewDocument())
		{
			// user has been alerted to what failed in OnNewDocument
			TRACE0("CDocument::OnNewDocument returned FALSE\n");
			if (bCreated)
				pFrame->DestroyWindow();    // will destroy document
			return NULL;
		}
	}
	else
	{
		BeginWaitCursor();

		// open an existing document
		bWasModified = pDocument->IsModified();
		pDocument->SetModifiedFlag(FALSE);  // not dirty for open

		if (!pDocument->OnOpenDocument(pszPathName))
		{
			// user has been alerted to what failed in OnOpenDocument
			TRACE0("CDocument::OnOpenDocument returned FALSE\n");
			if (bCreated)
			{
				pFrame->DestroyWindow();    // will destroy document
			}
			else if (!pDocument->IsModified())
			{
				// original document is untouched
				pDocument->SetModifiedFlag(bWasModified);
			}
			else
			{
				// we corrupted the original document
				CString strDocName;
				if (!GetDocString(strDocName, CDocTemplate::docName) ||
					strDocName.IsEmpty())
				{
					// use generic 'untitled'
					VERIFY(strDocName.LoadString(AFX_IDS_UNTITLED));
				}
				pDocument->SetTitle(strDocName);

				if (!pDocument->OnNewDocument())
				{
					TRACE0("Error: OnNewDocument failed after failing to"
						" open a document - trying to continue\n");
					// assume we can continue
				}
			}
			EndWaitCursor();
			return NULL;        // open failed
		}
		pDocument->SetPathName(pszPathName);
		EndWaitCursor();
	}

	if (bCreated && AfxGetApp()->m_pMainWnd == NULL)
	{
		// set as main frame
		AfxGetApp()->m_pMainWnd = pFrame;
		// InitialUpdateFrame will show the window in the correct mode
	}

	InitialUpdateFrame(pFrame, pDocument, bMakeVisible);
	return pDocument;
}

/////////////////////////////////////////////////////////////////////////////
// CSingleDocTemplate diagnostics

#ifdef _DEBUG
void CSingleDocTemplate::Dump(CDumpContext& dc) const
{
	CDocTemplate::Dump(dc);
	if (m_pOnlyDoc)
		AFX_DUMP1(dc, "\nwith document: ", (void*)m_pOnlyDoc);
	else
		AFX_DUMP0(dc, "\nwith no document");
}

void CSingleDocTemplate::AssertValid() const
{
	CDocTemplate::AssertValid();
	if (m_pOnlyDoc)
		ASSERT_VALID(m_pOnlyDoc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
