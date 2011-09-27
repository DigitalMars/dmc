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
// CMultiDocTemplate

IMPLEMENT_DYNAMIC(CMultiDocTemplate, CDocTemplate)

/////////////////////////////////////////////////////////////////////////////
// CMultiDocTemplate construction/destruction

CMultiDocTemplate::CMultiDocTemplate(UINT nIDResource, CRuntimeClass* pDocClass,
	CRuntimeClass* pFrameClass, CRuntimeClass* pViewClass)
		: CDocTemplate(nIDResource, pDocClass, pFrameClass, pViewClass)
{
	ASSERT(m_docList.IsEmpty());
	ASSERT(pDocClass != NULL);

	HINSTANCE hInst = AfxFindResourceHandle(
		MAKEINTRESOURCE(nIDResource), RT_MENU);
	m_hMenuShared = ::LoadMenu(hInst, MAKEINTRESOURCE(nIDResource));
	m_hAccelTable = ::LoadAccelerators(hInst, MAKEINTRESOURCE(nIDResource));
	m_nUntitledCount = 0;   // start at 1

#ifdef _DEBUG
	// warnings about missing components (don't bother with accelerators)
	if (m_hMenuShared == NULL)
		TRACE1("Warning: no shared menu for document template #%d\n", nIDResource);
#endif //_DEBUG
}

CMultiDocTemplate::~CMultiDocTemplate()
{
#ifdef _DEBUG
	if (!m_docList.IsEmpty())
		TRACE1("Warning: destroying CMultiDocTemplate with %d documents alive\n",
			m_docList.GetCount());
#endif
	// delete shared components
	if (m_hMenuShared != NULL)
		::DestroyMenu(m_hMenuShared);
	if (m_hAccelTable != NULL)
		::FreeResource((HGLOBAL)m_hAccelTable);
}

/////////////////////////////////////////////////////////////////////////////
// CMultiDocTemplate attributes

POSITION CMultiDocTemplate::GetFirstDocPosition() const
{
	return m_docList.GetHeadPosition();
}

CDocument* CMultiDocTemplate::GetNextDoc(POSITION& rPos) const
{
	return (CDocument*)m_docList.GetNext(rPos);
}

/////////////////////////////////////////////////////////////////////////////
// CMultiDocTemplate document management (a list of currently open documents)

void CMultiDocTemplate::AddDocument(CDocument* pDoc)
{
	ASSERT_VALID(pDoc);

	CDocTemplate::AddDocument(pDoc);
	ASSERT(m_docList.Find(pDoc, NULL) == NULL); // must not be in list
	m_docList.AddTail(pDoc);
}


void CMultiDocTemplate::RemoveDocument(CDocument* pDoc)
{
	ASSERT_VALID(pDoc);

	CDocTemplate::RemoveDocument(pDoc);
	m_docList.RemoveAt(m_docList.Find(pDoc));
}

/////////////////////////////////////////////////////////////////////////////
// CMultiDocTemplate commands

CDocument* CMultiDocTemplate::OpenDocumentFile(const char* pszPathName,
	BOOL bMakeVisible)
{
	CDocument* pDocument = CreateNewDocument();
	if (pDocument == NULL)
	{
		TRACE0("CDocTemplate::CreateNewDocument returned NULL\n");
		AfxMessageBox(AFX_IDP_FAILED_TO_CREATE_DOC);
		return NULL;
	}
	ASSERT_VALID(pDocument);

	BOOL bAutoDelete = pDocument->m_bAutoDelete;
	pDocument->m_bAutoDelete = FALSE;   // don't destroy if something goes wrong
	CFrameWnd* pFrame = CreateNewFrame(pDocument, NULL);
	pDocument->m_bAutoDelete = bAutoDelete;
	if (pFrame == NULL)
	{
		AfxMessageBox(AFX_IDP_FAILED_TO_CREATE_DOC);
		delete pDocument;       // explicit delete on error
		return NULL;
	}
	ASSERT_VALID(pFrame);

	if (pszPathName == NULL)
	{
		// create a new document - with default document name
		UINT nUntitled = m_nUntitledCount + 1;

		CString strDocName;
		if (GetDocString(strDocName, CDocTemplate::docName) &&
			!strDocName.IsEmpty())
		{
			char szNum[8];
			wsprintf(szNum, "%d", nUntitled);
			strDocName += szNum;
		}
		else
		{
			// use generic 'untitled' - ignore untitled count
			VERIFY(strDocName.LoadString(AFX_IDS_UNTITLED));
		}
		pDocument->SetTitle(strDocName);

		if (bMakeVisible && !pDocument->OnNewDocument())
		{
			// user has be alerted to what failed in OnNewDocument
			TRACE0("CDocument::OnNewDocument returned FALSE\n");
			pFrame->DestroyWindow();
			return NULL;
		}

		// it worked, now bump untitled count
		m_nUntitledCount++;
	}
	else
	{
		// open an existing document
		BeginWaitCursor();
		if (!pDocument->OnOpenDocument(pszPathName))
		{
			// user has be alerted to what failed in OnOpenDocument
			TRACE0("CDocument::OnOpenDocument returned FALSE\n");
			pFrame->DestroyWindow();
			EndWaitCursor();
			return NULL;
		}
		pDocument->SetPathName(pszPathName);
		EndWaitCursor();
	}

	InitialUpdateFrame(pFrame, pDocument, bMakeVisible);
	return pDocument;
}

/////////////////////////////////////////////////////////////////////////////
// CMultiDocTemplate diagnostics

#ifdef _DEBUG
void CMultiDocTemplate::Dump(CDumpContext& dc) const
{
	CDocTemplate::Dump(dc);
	AFX_DUMP1(dc, "\nm_hMenuShared = ", (UINT)m_hMenuShared);
	AFX_DUMP1(dc, "\nm_hAccelTable = ", (UINT)m_hAccelTable);
	AFX_DUMP1(dc, "\nm_nUntitledCount = ", m_nUntitledCount);
	AFX_DUMP1(dc, "\nwith ", m_docList.GetCount());
	AFX_DUMP0(dc, " open documents");
	POSITION pos = GetFirstDocPosition();
	while (pos != NULL)
	{
		CDocument* pDoc = GetNextDoc(pos);
		AFX_DUMP1(dc, "\nwith document ", (void*)pDoc);
	}
}

void CMultiDocTemplate::AssertValid() const
{
	CDocTemplate::AssertValid();

	POSITION pos = GetFirstDocPosition();
	while (pos != NULL)
	{
		CDocument* pDoc = GetNextDoc(pos);
		ASSERT_VALID(pDoc);
	}
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
