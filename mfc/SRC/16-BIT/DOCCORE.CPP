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
#include "io.h" // for access

#ifdef AFX_CORE2_SEG
#pragma code_seg(AFX_CORE2_SEG)
#endif

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDocument

IMPLEMENT_DYNAMIC(CDocument, CCmdTarget)

BEGIN_MESSAGE_MAP(CDocument, CCmdTarget)
	//{{AFX_MSG_MAP(CDocument)
	ON_COMMAND(ID_FILE_CLOSE, OnFileClose)
	ON_COMMAND(ID_FILE_SAVE, OnFileSave)
	ON_COMMAND(ID_FILE_SAVE_AS, OnFileSaveAs)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDocument construction/destruction

CDocument::CDocument()
{
	m_pDocTemplate = NULL;
	m_bModified = FALSE;
	m_bAutoDelete = TRUE;       // default to auto delete document
	ASSERT(m_viewList.IsEmpty());
}

CDocument::~CDocument()
{
	// do not call DeleteContents here !
#ifdef _DEBUG
	if (IsModified())
		TRACE0("Warning: destroying an unsaved document\n");
#endif

	// there should be no views left!
	DisconnectViews();
	ASSERT(m_viewList.IsEmpty());

	if (m_pDocTemplate != NULL)
		m_pDocTemplate->RemoveDocument(this);
	ASSERT(m_pDocTemplate == NULL);     // must be detached
}

void CDocument::OnFinalRelease()
{
	ASSERT_VALID(this);

	OnCloseDocument();  // may 'delete this'
}

void CDocument::DisconnectViews()
{
	while (!m_viewList.IsEmpty())
	{
		CView* pView = (CView*)m_viewList.RemoveHead();
		ASSERT_VALID(pView);
		ASSERT(pView->IsKindOf(RUNTIME_CLASS(CView)));
		pView->m_pDocument = NULL;
	}
}

/////////////////////////////////////////////////////////////////////////////
// CDocument attributes, general services

void CDocument::SetTitle(const char* pszTitle)
{
	m_strTitle = pszTitle;
	UpdateFrameCounts();        // will cause name change in views
}

void CDocument::DeleteContents()
{
}

/////////////////////////////////////////////////////////////////////////////
// Closing documents or views

void CDocument::OnChangedViewList()
{
	// if no more views on the document, delete ourself
	// not called if directly closing the document or terminating the app
	if (m_viewList.IsEmpty() && m_bAutoDelete)
	{
		OnCloseDocument();
		return;
	}

	// update the frame counts as needed
	UpdateFrameCounts();
}

void CDocument::UpdateFrameCounts()
	 // assumes 1 doc per frame
{
	// walk all frames of views (mark and sweep approach)
	POSITION pos = GetFirstViewPosition();
	while (pos != NULL)
	{
		CView* pView = GetNextView(pos);
		ASSERT_VALID(pView);
		if (pView->IsWindowVisible())   // Do not count invisible windows.
		{
			CFrameWnd* pFrame = pView->GetParentFrame();
			if (pFrame != NULL)
				pFrame->m_nWindow = -1;     // unknown
		}
	}

	// now do it again counting the unique ones
	int nFrames = 0;
	pos = GetFirstViewPosition();
	while (pos != NULL)
	{
		CView* pView = GetNextView(pos);
		ASSERT_VALID(pView);
		if (pView->IsWindowVisible())   // Do not count invisible windows.
		{
			CFrameWnd* pFrame = pView->GetParentFrame();
			if (pFrame != NULL && pFrame->m_nWindow == -1)
			{
				ASSERT_VALID(pFrame);
				// not yet counted (give it a 1 based number)
				pFrame->m_nWindow = ++nFrames;
			}
		}
	}

	// lastly walk the frames and update titles (assume same order)
	// go through frames updating the appropriate one
	int iFrame = 1;
	pos = GetFirstViewPosition();
	while (pos != NULL)
	{
		CView* pView = GetNextView(pos);
		ASSERT_VALID(pView);
		if (pView->IsWindowVisible())   // Do not count invisible windows.
		{
			CFrameWnd* pFrame = pView->GetParentFrame();
			if (pFrame != NULL && pFrame->m_nWindow == iFrame)
			{
				ASSERT_VALID(pFrame);
				if (nFrames == 1)
					pFrame->m_nWindow = 0;      // the only one of it's kind
				pFrame->OnUpdateFrameTitle(TRUE);
				iFrame++;
			}
		}
	}
	ASSERT(iFrame == nFrames + 1);
}

BOOL CDocument::CanCloseFrame(CFrameWnd* pFrameArg)
	// permission to close all views using this frame
	//  (at least one of our views must be in this frame)
{
	ASSERT_VALID(pFrameArg);

	POSITION pos = GetFirstViewPosition();
	while (pos != NULL)
	{
		CView* pView = GetNextView(pos);
		ASSERT_VALID(pView);
		CFrameWnd* pFrame = pView->GetParentFrame();
		// assume frameless views are ok to close
		if (pFrame != NULL)
		{
			// assumes 1 document per frame
			ASSERT_VALID(pFrame);
			if (pFrame->m_nWindow > 0)
				return TRUE;        // more than one frame refering to us
		}
	}

	// otherwise only one frame that we know about
	return SaveModified();
}

void CDocument::PreCloseFrame(CFrameWnd* pFrameArg)
{
	// default does nothing
}

/////////////////////////////////////////////////////////////////////////////
// File/Path commands

void CDocument::SetPathName(const char* pszPathName, BOOL bAddToMRU)
{
	m_strPathName = pszPathName;
	ASSERT(!m_strPathName.IsEmpty());       // must be set to something

	// Set the document title based on path name
	char szTitle[_MAX_FNAME];
	if (::GetFileTitle(m_strPathName, szTitle, _MAX_FNAME) == 0)
	{
		::AnsiUpper(szTitle);       // always upper case
		SetTitle(szTitle);
	}

	if (bAddToMRU)
		AfxGetApp()->AddToRecentFileList(pszPathName);
}

/////////////////////////////////////////////////////////////////////////////
// Standard file menu commands

void CDocument::OnFileClose()
{
	if (!SaveModified())
		return;

	// shut it down
	OnCloseDocument();
		// this should destroy the document
}


void CDocument::OnFileSave()
{
	if (_access(m_strPathName, 6) != 0)
	{
		// we do not have read-write access or the file does not (now) exist
		if (!DoSave(NULL))
			TRACE0("Warning: File save with new name failed\n");
	}
	else
	{
		if (!DoSave(m_strPathName))
			TRACE0("Warning: File save failed\n");
	}
}


void CDocument::OnFileSaveAs()
{
	if (!DoSave(NULL))
		TRACE0("Warning: File save-as failed\n");
}


BOOL CDocument::DoSave(const char* pszPathName, BOOL bReplace /*=TRUE*/)
	// Save the document data to a file
	// pszPathName = path name where to save document file
	// if pszPathName is NULL then the user will be prompted (SaveAs)
	// note: pszPathName can be different than 'm_strPathName'
	// if 'bReplace' is TRUE will change file name if successful (SaveAs)
	// if 'bReplace' is FALSE will not change path name (SaveCopyAs)
{
	CString newName = pszPathName;
	if (newName.IsEmpty())
	{
		CDocTemplate* pTemplate = GetDocTemplate();
		ASSERT(pTemplate != NULL);

		newName = m_strPathName;
		if (bReplace && newName.IsEmpty())
		{
			newName = m_strTitle;
			if (newName.GetLength() > 8)
				newName.ReleaseBuffer(8);
			int iBad = newName.FindOneOf(" #%;/\\");    // dubious filename
			if (iBad != -1)
				newName.ReleaseBuffer(iBad);

			// append the default suffix if there is one
			CString strExt;
			if (pTemplate->GetDocString(strExt, CDocTemplate::filterExt) &&
			  !strExt.IsEmpty())
			{
				ASSERT(strExt[0] == '.');
				newName += strExt;
			}
		}

		if (!AfxGetApp()->DoPromptFileName(newName,
		  bReplace ? AFX_IDS_SAVEFILE : AFX_IDS_SAVEFILECOPY,
		  OFN_HIDEREADONLY | OFN_PATHMUSTEXIST, FALSE, pTemplate))
			return FALSE;       // don't even try to save
	}

	BeginWaitCursor();
	if (!OnSaveDocument(newName))
	{
		if (pszPathName == NULL)
		{
			// be sure to delete the file
			TRY
			{
				CFile::Remove(newName);
			}
			CATCH_ALL(e)
			{
				TRACE0("Warning: failed to delete file after failed SaveAs\n");
			}
			END_CATCH_ALL
		}
		EndWaitCursor();
		return FALSE;
	}

	if (bReplace)
	{
		// Reset the title and change the document name
		SetPathName(newName);
		ASSERT(m_strPathName == newName);       // must be set
	}
	EndWaitCursor();
	return TRUE;        // success
}


BOOL CDocument::SaveModified()
{
	if (!IsModified())
		return TRUE;        // ok to continue

	// get name/title of document
	CString name = m_strPathName;
	if (name.IsEmpty())
	{
		name = m_strTitle;
		if (name.IsEmpty())
			VERIFY(name.LoadString(AFX_IDS_UNTITLED));
	}

	CString prompt;
	AfxFormatString1(prompt, AFX_IDP_ASK_TO_SAVE, name);
	switch (AfxMessageBox(prompt, MB_YESNOCANCEL, AFX_IDP_ASK_TO_SAVE))
	{
	case IDCANCEL:
		return FALSE;       // don't continue

	case IDYES:
		// If so, either Save or Update, as appropriate
		if (_access(m_strPathName, 6) != 0)
		{
			if (!DoSave(NULL))
				return FALSE;   // don't continue
		}
		else
		{
			if (!DoSave(m_strPathName))
				return FALSE;   // don't continue
		}
		break;

	case IDNO:
		// If not saving changes, revert the document
		break;

	default:
		ASSERT(FALSE);
		break;
	}
	return TRUE;    // keep going
}

HMENU CDocument::GetDefaultMenu()
{
	return NULL;    // just use original default
}

HACCEL CDocument::GetDefaultAccel()
{
	return NULL;    // just use original default
}

void CDocument::ReportSaveLoadException(const char* pszPathName, CException* e,
		BOOL bSaving, UINT nIDPDefault)
{
	ASSERT_VALID(e);

	UINT nIDP = nIDPDefault;

	if (e->IsKindOf(RUNTIME_CLASS(CUserException)))
		return; // already reported

	if (e->IsKindOf(RUNTIME_CLASS(CArchiveException)))
	{
		switch (((CArchiveException*)e)->m_cause)
		{
		case CArchiveException::badSchema:
		case CArchiveException::badClass:
		case CArchiveException::badIndex:
		case CArchiveException::endOfFile:
			nIDP = AFX_IDP_FAILED_INVALID_FORMAT;
			break;
		default:
			break;
		}
	}
	else if (e->IsKindOf(RUNTIME_CLASS(CFileException)))
	{
		TRACE1("Reporting file I/O exception on Save/Load"
			" with lOsError = $%lX\n", ((CFileException*)e)->m_lOsError);

		switch (((CFileException*)e)->m_cause)
		{
		case CFileException::fileNotFound:
		case CFileException::badPath:
			nIDP = AFX_IDP_FAILED_INVALID_PATH;
			break;
		case CFileException::diskFull:
			nIDP = AFX_IDP_FAILED_DISK_FULL;
			break;
		case CFileException::accessDenied:
			nIDP = bSaving ? AFX_IDP_FAILED_ACCESS_WRITE :
					AFX_IDP_FAILED_ACCESS_READ;
			break;

		case CFileException::badSeek:
		case CFileException::generic:
		case CFileException::tooManyOpenFiles:
		case CFileException::invalidFile:
		case CFileException::hardIO:
		case CFileException::directoryFull:
			nIDP = bSaving ? AFX_IDP_FAILED_IO_ERROR_WRITE :
					AFX_IDP_FAILED_IO_ERROR_READ;
			break;
		default:
			break;
		}
	}

	CString prompt;
	AfxFormatString1(prompt, nIDP, pszPathName);
	AfxMessageBox(prompt, MB_ICONEXCLAMATION, nIDP);
}

/////////////////////////////////////////////////////////////////////////////
// File operations (default uses CDocument::Serialize)

BOOL CDocument::OnNewDocument()
{
	if (IsModified())
		TRACE0("Warning: OnNewDocument replaces an unsaved document\n");

	DeleteContents();
	m_strPathName.Empty();      // no path name yet
	SetModifiedFlag(FALSE);     // make clean

	return TRUE;
}

BOOL CDocument::OnOpenDocument(const char* pszPathName)
{
	if (IsModified())
		TRACE0("Warning: OnOpenDocument replaces an unsaved document\n");

	CFile file;
	CFileException fe;
	if (!file.Open(pszPathName, CFile::modeRead | CFile::shareDenyWrite, &fe))
	{
		ReportSaveLoadException(pszPathName, &fe,
			FALSE, AFX_IDP_FAILED_TO_OPEN_DOC);
		return FALSE;
	}

	DeleteContents();
	SetModifiedFlag(TRUE);  // dirty during de-serialize

	CArchive loadArchive(&file, CArchive::load | CArchive::bNoFlushOnDelete);
	loadArchive.m_pDocument = this;
	loadArchive.m_bForceFlat = FALSE;
	TRY
	{
		BeginWaitCursor();
		Serialize(loadArchive);     // load me
		loadArchive.Close();
		file.Close();
	}
	CATCH_ALL(e)
	{
		file.Abort(); // will not throw an exception
		DeleteContents();   // remove failed contents
		EndWaitCursor();

		TRY
			ReportSaveLoadException(pszPathName, e,
				FALSE, AFX_IDP_FAILED_TO_OPEN_DOC);
		END_TRY
		return FALSE;
	}
	END_CATCH_ALL

	EndWaitCursor();
	SetModifiedFlag(FALSE);     // start off with unmodified
	return TRUE;
}

BOOL CDocument::OnSaveDocument(const char* pszPathName)
{
	CFile file;
	CFileException fe;

	if (!file.Open(pszPathName, CFile::modeCreate |
	  CFile::modeReadWrite | CFile::shareExclusive, &fe))
	{
		ReportSaveLoadException(pszPathName, &fe,
			TRUE, AFX_IDP_INVALID_FILENAME);
		return FALSE;
	}

	CArchive saveArchive(&file, CArchive::store | CArchive::bNoFlushOnDelete);
	saveArchive.m_pDocument = this;
	saveArchive.m_bForceFlat = FALSE;
	TRY
	{
		BeginWaitCursor();
		Serialize(saveArchive);     // save me
		saveArchive.Close();
		file.Close();
	}
	CATCH_ALL(e)
	{
		file.Abort(); // will not throw an exception
		EndWaitCursor();

		TRY
			ReportSaveLoadException(pszPathName, e,
				TRUE, AFX_IDP_FAILED_TO_SAVE_DOC);
		END_TRY
		return FALSE;
	}
	END_CATCH_ALL

	EndWaitCursor();
	SetModifiedFlag(FALSE);     // back to unmodified

	return TRUE;        // success
}


void CDocument::OnCloseDocument()
	// must close all views now (no prompting) - usually destroy's this
{
	// destroy all frames viewing this document
	// the last destroy may destroy us
	BOOL bAutoDelete = m_bAutoDelete;
	m_bAutoDelete = FALSE;  // don't destroy document while closing views
	while (!m_viewList.IsEmpty())
	{
		// get frame attached to the view
		CView* pView = (CView*)m_viewList.GetHead();
		ASSERT_VALID(pView);
		CFrameWnd* pFrame = pView->GetParentFrame();
		ASSERT_VALID(pFrame);

		// and close it
		PreCloseFrame(pFrame);
		pFrame->DestroyWindow();
			// will destroy the view as well
	}
	m_bAutoDelete = bAutoDelete;

	// clean up contents of document before destroying the document itself
	DeleteContents();

	// delete the document if necessary
	if (m_bAutoDelete)
		delete this;
}

void CDocument::OnIdle()
{
	// default does nothing
}

/////////////////////////////////////////////////////////////////////////////
// View operations

void CDocument::AddView(CView* pView)
{
	ASSERT_VALID(pView);
	ASSERT(pView->m_pDocument == NULL); // must not be already attached
	ASSERT(m_viewList.Find(pView, NULL) == NULL);   // must not be in list

	m_viewList.AddTail(pView);
	ASSERT(pView->m_pDocument == NULL); // must be un-attached
	pView->m_pDocument = this;

	OnChangedViewList();    // must be the last thing done to the document
}

void CDocument::RemoveView(CView* pView)
{
	ASSERT_VALID(pView);
	ASSERT(pView->m_pDocument == this); // must be attached to us

	m_viewList.RemoveAt(m_viewList.Find(pView));
	pView->m_pDocument = NULL;

	OnChangedViewList();    // must be the last thing done to the document
}

POSITION CDocument::GetFirstViewPosition() const
{
	return m_viewList.GetHeadPosition();
}

CView* CDocument::GetNextView(POSITION& rPosition) const
{
	ASSERT(rPosition != BEFORE_START_POSITION);
		// use CDocument::GetFirstViewPosition instead !
	if (rPosition == NULL)
		return NULL;    // nothing left
	CView* pView = (CView*)m_viewList.GetNext(rPosition);
	ASSERT(pView->IsKindOf(RUNTIME_CLASS(CView)));
	return pView;
}

void CDocument::UpdateAllViews(CView* pSender, LPARAM lHint, CObject* pHint)
	// walk through all views
{
	ASSERT(pSender == NULL || !m_viewList.IsEmpty());
		// must have views if sent by one of them

	POSITION pos = GetFirstViewPosition();
	while (pos != NULL)
	{
		CView* pView = GetNextView(pos);
		ASSERT_VALID(pView);
		if (pView != pSender)
			pView->OnUpdate(pSender, lHint, pHint);
	}
}

void CDocument::SendInitialUpdate()
	// walk through all views and call OnInitialUpdate
{
	POSITION pos = GetFirstViewPosition();
	while (pos != NULL)
	{
		CView* pView = GetNextView(pos);
		ASSERT_VALID(pView);
		pView->OnInitialUpdate();
	}
}

/////////////////////////////////////////////////////////////////////////////
// command routing

BOOL CDocument::OnCmdMsg(UINT nID, int nCode, void* pExtra,
	AFX_CMDHANDLERINFO* pHandlerInfo)
{
	if (CCmdTarget::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo))
		return TRUE;

	// otherwise try template
	if (m_pDocTemplate != NULL &&
	  m_pDocTemplate->OnCmdMsg(nID, nCode, pExtra, pHandlerInfo))
		return TRUE;

	return FALSE;
}

/////////////////////////////////////////////////////////////////////////////
// CDocument diagnostics

#ifdef _DEBUG
void CDocument::Dump(CDumpContext& dc) const
{
	CObject::Dump(dc);
	AFX_DUMP1(dc, "\nm_strTitle = ", m_strTitle);
	AFX_DUMP1(dc, "\nm_strPathName = ", m_strPathName);
	AFX_DUMP1(dc, "\nm_bModified = ", m_bModified);
	AFX_DUMP1(dc, "\nm_pDocTemplate = ", (void*)m_pDocTemplate);

	if (dc.GetDepth() > 0)
	{
		POSITION pos = GetFirstViewPosition();
		while (pos != NULL)
		{
			CView* pView = GetNextView(pos);
			ASSERT_VALID(pView);
			AFX_DUMP1(dc, "\nwith view ", (void*)pView);
		}
	}
}

void CDocument::AssertValid() const
{
	CObject::AssertValid();

	POSITION pos = GetFirstViewPosition();
	while (pos != NULL)
	{
		CView* pView = GetNextView(pos);
		ASSERT_VALID(pView);
	}
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
