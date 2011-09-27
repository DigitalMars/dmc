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
// CWinApp User Interface Extensions
/////////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CWinApp, CCmdTarget)
	//{{AFX_MSG_MAP(CWinApp)
	// Global File commands
	ON_COMMAND(ID_APP_EXIT, OnAppExit)
	// MRU - most recently used file menu
	ON_UPDATE_COMMAND_UI(ID_FILE_MRU_FILE1, OnUpdateRecentFileMenu)
	ON_UPDATE_COMMAND_UI(ID_FILE_MRU_FILE2, OnUpdateRecentFileMenu)
	ON_UPDATE_COMMAND_UI(ID_FILE_MRU_FILE3, OnUpdateRecentFileMenu)
	ON_UPDATE_COMMAND_UI(ID_FILE_MRU_FILE4, OnUpdateRecentFileMenu)
	ON_COMMAND_EX(ID_FILE_MRU_FILE1, OnOpenRecentFile)
	ON_COMMAND_EX(ID_FILE_MRU_FILE2, OnOpenRecentFile)
	ON_COMMAND_EX(ID_FILE_MRU_FILE3, OnOpenRecentFile)
	ON_COMMAND_EX(ID_FILE_MRU_FILE4, OnOpenRecentFile)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


void CWinApp::OnAppExit()
{
	// same as double-clicking on main window close box
	ASSERT(m_pMainWnd != NULL);
	m_pMainWnd->SendMessage(WM_CLOSE);
}

CDocument* CWinApp::OpenDocumentFile(LPCSTR lpszFileName)
{
	// find the highest confidence
	POSITION pos = m_templateList.GetHeadPosition();
	CDocTemplate::Confidence bestMatch = CDocTemplate::noAttempt;
	CDocTemplate* pBestTemplate = NULL;
	CDocument* pOpenDocument = NULL;

	char szPath[_MAX_PATH];
	_AfxFullPath(szPath, lpszFileName);

	while (pos != NULL)
	{
		CDocTemplate* pTemplate = (CDocTemplate*)m_templateList.GetNext(pos);
		ASSERT(pTemplate->IsKindOf(RUNTIME_CLASS(CDocTemplate)));

		CDocTemplate::Confidence match;
		ASSERT(pOpenDocument == NULL);
		match = pTemplate->MatchDocType(szPath, pOpenDocument);
		if (match > bestMatch)
		{
			bestMatch = match;
			pBestTemplate = pTemplate;
		}
		if (match == CDocTemplate::yesAlreadyOpen)
			break;      // stop here
	}

	if (pOpenDocument != NULL)
	{
		POSITION pos = pOpenDocument->GetFirstViewPosition();
		if (pos != NULL)
		{
			CView* pView = pOpenDocument->GetNextView(pos); // get first one
			ASSERT_VALID(pView);
			CFrameWnd* pFrame = pView->GetParentFrame();
			if (pFrame != NULL)
				pFrame->ActivateFrame();
			else
				TRACE0("Error: Can not find a frame for document to activate");
			CFrameWnd* pAppFrame;
			if (pFrame != (pAppFrame = (CFrameWnd*)AfxGetApp()->m_pMainWnd))
			{
				ASSERT(pAppFrame->IsKindOf(RUNTIME_CLASS(CFrameWnd)));
				pAppFrame->ActivateFrame();
			}
		}
		else
		{
			TRACE0("Error: Can not find a view for document to activate");
		}
		return pOpenDocument;
			// file already open (even if we can't activate it)
	}

	if (pBestTemplate == NULL)
	{
		TRACE0("Error: can't open document\n");
		return FALSE;
	}

	return pBestTemplate->OpenDocumentFile(szPath);
}

int CWinApp::GetOpenDocumentCount()
{
	int nOpen = 0;
	POSITION pos = m_templateList.GetHeadPosition();
	while (pos != NULL)
	{
		CDocTemplate* pTemplate = (CDocTemplate*)m_templateList.GetNext(pos);
		POSITION pos2 = pTemplate->GetFirstDocPosition();
		while (pos2)
		{
			if (pTemplate->GetNextDoc(pos2) != NULL)
				nOpen++;
		}
	}
	return nOpen;
}

/////////////////////////////////////////////////////////////////////////////
// DDE and ShellExecute support

#ifdef AFX_CORE3_SEG
#pragma code_seg(AFX_CORE3_SEG)
#endif

BOOL CWinApp::OnDDECommand(char* pszCommand)
{
	// open format is "[open("%s")]" - no whitespace allowed, one per line
	static char BASED_CODE szOpenStart[] = "[open(\"";  // 7 characters
	if (_fstrncmp(pszCommand, szOpenStart, 7) == 0)
		pszCommand += 7;
	else
		return FALSE;   // not the Open command

	LPSTR lpszEnd = _AfxStrChr(pszCommand, '"');
	if (lpszEnd == NULL)
		return FALSE;       // illegally terminated

	// trim the string, and open the file
	*lpszEnd = '\0';
	OpenDocumentFile(pszCommand);

	// show the application window
	if (!AfxGetApp()->m_pMainWnd->IsWindowVisible())
	{
		AfxGetApp()->m_pMainWnd->ShowWindow(AfxGetApp()->m_nCmdShow);
		AfxGetApp()->m_pMainWnd->UpdateWindow();
	}
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// Doc template support

void CWinApp::AddDocTemplate(CDocTemplate* pTemplate)
{
	ASSERT_VALID(pTemplate);
	ASSERT(m_templateList.Find(pTemplate, NULL) == NULL);// must not be in list
	m_templateList.AddTail(pTemplate);
}

/////////////////////////////////////////////////////////////////////////////
// Standard command helpers

BOOL CWinApp::SaveAllModified()
{
	POSITION pos = m_templateList.GetHeadPosition();
	while (pos != NULL)
	{
		CDocTemplate* pTemplate = (CDocTemplate*)m_templateList.GetNext(pos);
		ASSERT(pTemplate->IsKindOf(RUNTIME_CLASS(CDocTemplate)));
		if (!pTemplate->SaveAllModified())
			return FALSE;
	}
	return TRUE;
}

void CWinApp::CloseAllDocuments(BOOL bEndSession)
{
	POSITION pos = m_templateList.GetHeadPosition();
	while (pos != NULL)
	{
		CDocTemplate* pTemplate = (CDocTemplate*)m_templateList.GetNext(pos);
		ASSERT(pTemplate->IsKindOf(RUNTIME_CLASS(CDocTemplate)));
		pTemplate->CloseAllDocuments(bEndSession);
	}
}

void CWinApp::HideApplication()
{
	ASSERT_VALID(m_pMainWnd);

	// hide the application's windows before closing all the documents
	m_pMainWnd->ShowWindow(SW_HIDE);
	m_pMainWnd->ShowOwnedPopups(FALSE);

	// put the window at the bottom of zorder, so it isn't activated
	m_pMainWnd->SetWindowPos(&CWnd::wndBottom, 0, 0, 0, 0,
		SWP_NOMOVE|SWP_NOSIZE|SWP_NOACTIVATE);
}

void CWinApp::DoWaitCursor(int nCode)
{
	// 0 => restore, 1=> begin, -1=> end
	ASSERT(nCode == 0 || nCode == 1 || nCode == -1);
	ASSERT(afxData.hcurWait != NULL);
	m_nWaitCursorCount += nCode;

	if (m_nWaitCursorCount > 0)
	{
		HCURSOR hcurPrev;
		hcurPrev = ::SetCursor(afxData.hcurWait);
		if (hcurPrev != NULL && hcurPrev != afxData.hcurWait)
			m_hcurWaitCursorRestore = hcurPrev;
	}
	else
	{
		// turn everything off
		m_nWaitCursorCount = 0;     // prevent underflow
		::SetCursor(m_hcurWaitCursorRestore);
	}
}

// get parent window for modal dialogs and message boxes.
HWND PASCAL _AfxGetSafeOwner(CWnd* pParent)
{
	if (pParent != NULL)
	{
		ASSERT_VALID(pParent);
		return pParent->m_hWnd;
	}

	pParent = AfxGetMainWnd();
	if (pParent == NULL || pParent->m_hWnd == NULL)
		return NULL;
	ASSERT_VALID(pParent);
	return ::GetLastActivePopup(pParent->GetTopLevelParent()->m_hWnd);
}

void CWinApp::EnableModeless(BOOL bEnable)
{
	// no-op if main window is NULL or not a CFrameWnd
	CWnd* pMainWnd = GetMainWnd();
	if (pMainWnd == NULL || !pMainWnd->IsFrameWnd())
		return;

	// check if notify hook installed
	ASSERT(pMainWnd->IsKindOf(RUNTIME_CLASS(CFrameWnd)));
	CFrameWnd* pFrameWnd = (CFrameWnd*)pMainWnd;
	if (pFrameWnd->m_pNotifyHook == NULL)
		return;
	pFrameWnd->m_pNotifyHook->OnEnableModeless(bEnable);
}

int CWinApp::DoMessageBox(LPCSTR lpszPrompt, UINT nType, UINT nIDPrompt)
{
	ASSERT_VALID(this);

	CWnd* pWnd = CWnd::FromHandle(_AfxGetSafeOwner(NULL));
	EnableModeless(FALSE);

	DWORD* pdwContext = &m_dwPromptContext;
	if (pWnd != NULL)
	{
		// use app-level context or frame level context
		ASSERT_VALID(pWnd);
		CWnd* pMainWnd = pWnd->GetTopLevelParent();
		ASSERT_VALID(pMainWnd);
		if (pMainWnd->IsFrameWnd())
			pdwContext = &((CFrameWnd*)pMainWnd)->m_dwPromptContext;
	}

	ASSERT(pdwContext != NULL);
	DWORD dwOldPromptContext = *pdwContext;
	if (nIDPrompt != 0)
		*pdwContext = HID_BASE_PROMPT+nIDPrompt;

	if ((nType & MB_ICONMASK) == 0)
	{
		switch (nType & MB_TYPEMASK)
		{
		case MB_OK:
		case MB_OKCANCEL:
			nType |= MB_ICONEXCLAMATION;
			break;

		case MB_YESNO:
		case MB_YESNOCANCEL:
			nType |= MB_ICONQUESTION;
			break;

		case MB_ABORTRETRYIGNORE:
		case MB_RETRYCANCEL:
			// No default icon for these types, since they are rarely used.
			// The caller should specify the icon.
			break;
		}
	}

#ifdef _DEBUG
	if ((nType & MB_ICONMASK) == 0)
		TRACE0("Warning: no icon specified for message box.\n");
#endif

	int nRet = ::MessageBox(pWnd->GetSafeHwnd(), lpszPrompt, m_pszAppName, nType);
	*pdwContext = dwOldPromptContext;
	EnableModeless(TRUE);

	return nRet;
}

int AFXAPI AfxMessageBox(LPCSTR lpszText, UINT nType, UINT nIDHelp)
{
	return AfxGetApp()->DoMessageBox(lpszText, nType, nIDHelp);
}

int AFXAPI AfxMessageBox(UINT nIDPrompt, UINT nType, UINT nIDHelp)
{
	CString string;
	if (!string.LoadString(nIDPrompt))
	{
		TRACE1("Error: failed to load message box prompt string 0x%04x\n",
			nIDPrompt);
		ASSERT(FALSE);
	}
	if (nIDHelp == (UINT)-1)
		nIDHelp = nIDPrompt;
	return AfxGetApp()->DoMessageBox(string, nType, nIDHelp);
}

int CWnd::MessageBox(LPCSTR lpszText, LPCSTR lpszCaption /* = NULL */,
	UINT nType /* = MB_OK */)
{
	if (lpszCaption == NULL)
		lpszCaption = AfxGetAppName();
	return ::MessageBox(GetSafeHwnd(), lpszText, lpszCaption, nType);
}

/////////////////////////////////////////////////////////////////////////////
// MRU file list default implementation

BOOL CWinApp::OnOpenRecentFile(UINT nID)
{
	ASSERT_VALID(this);

	ASSERT(nID >= ID_FILE_MRU_FILE1);
	ASSERT(nID < ID_FILE_MRU_FILE1 + _AFX_MRU_COUNT);
	ASSERT(m_strRecentFiles[nID - ID_FILE_MRU_FILE1].GetLength() != 0);

	TRACE2("MRU: open file (%d) %s\n", (nID - ID_FILE_MRU_FILE1) + 1,
			(const char*)m_strRecentFiles[nID - ID_FILE_MRU_FILE1]);

	OpenDocumentFile(m_strRecentFiles[nID - ID_FILE_MRU_FILE1]);
	return TRUE;
}

void CWinApp::AddToRecentFileList(const char* pszPathName)
	// pszPathName must be a full path in upper-case ANSI character set
{
	ASSERT_VALID(this);
	ASSERT(pszPathName != NULL);
	ASSERT(AfxIsValidString(pszPathName));

	// update the MRU list
	int iMRU;
	int iMRULast;

	// if an existing MRU string matches file name
	for (iMRU = 0; iMRU < _AFX_MRU_COUNT - 1; iMRU++)
	{
		if (m_strRecentFiles[iMRU] == pszPathName)
			break;      // iMRU will point to matching entry
	}

	// move MRU strings after this one down
	for (iMRULast = iMRU; iMRULast > 0; iMRULast--)
	{
		ASSERT(iMRULast > 0);
		ASSERT(iMRULast < _AFX_MRU_COUNT);

		m_strRecentFiles[iMRULast] = m_strRecentFiles[iMRULast - 1];
	}

	// add our file name to the top
	m_strRecentFiles[0] = pszPathName;
}

void CWinApp::OnUpdateRecentFileMenu(CCmdUI* pCmdUI)
{
	ASSERT_VALID(this);
	ASSERT(pCmdUI->m_pMenu != NULL);

	if (m_strRecentFiles[0].IsEmpty())
	{
		// no MRU files
		pCmdUI->Enable(FALSE);
		return;
	}

	// when we get the update for the first entry, refill the list
	ASSERT(pCmdUI->m_nID == ID_FILE_MRU_FILE1);

	for (int iMRU = 0; iMRU < _AFX_MRU_COUNT; iMRU++)
		pCmdUI->m_pMenu->DeleteMenu(pCmdUI->m_nID + iMRU, MF_BYCOMMAND);

	char szCurDir[_MAX_PATH];
	_AfxFullPath(szCurDir, "A");
	int nCurDir = strlen(szCurDir) - 1;     // skip "A"

	for (iMRU = 0; iMRU < _AFX_MRU_COUNT; iMRU++)
	{
		char szBuff[_MAX_PATH];
		char* pch;

		if (m_strRecentFiles[iMRU].IsEmpty())
			break;

		pch = szBuff;
		*pch++ = '&';
		*pch++ = (char)('1' + iMRU);
		*pch++ = ' ';
		lstrcpy(pch, m_strRecentFiles[iMRU]);
		if (_fmemcmp(szCurDir, m_strRecentFiles[iMRU], nCurDir) == 0)
			lstrcpy(pch, pch + nCurDir);

		pCmdUI->m_pMenu->InsertMenu(pCmdUI->m_nIndex++,
			MF_STRING | MF_BYPOSITION, pCmdUI->m_nID++, szBuff);
	}
	// update end menu count
	pCmdUI->m_nIndex--; // point to last menu added
	pCmdUI->m_nIndexMax = pCmdUI->m_pMenu->GetMenuItemCount();

	pCmdUI->m_bEnableChanged = TRUE;    // all the added items are enabled
}

#ifdef AFX_INIT_SEG
#pragma code_seg(AFX_INIT_SEG)
#endif

// INI strings are not localized
static char BASED_CODE szFileSection[] = "Recent File List";
static char BASED_CODE szFileEntry[] = "File%d";
static char BASED_CODE szPreviewSection[] = "Settings";
static char BASED_CODE szPreviewEntry[] = "PreviewPages";

void CWinApp::SaveStdProfileSettings()
{
	ASSERT_VALID(this);

	for (int iMRU = 0; iMRU < _AFX_MRU_COUNT; iMRU++)
	{
		if (m_strRecentFiles[iMRU].IsEmpty())
			break;  // all done

		char szEntry[16];
		wsprintf(szEntry, szFileEntry, iMRU + 1);
		::WritePrivateProfileString(szFileSection, szEntry,
			m_strRecentFiles[iMRU], m_pszProfileName);
	}

	if (m_nNumPreviewPages != 0)
		WriteProfileInt(szPreviewSection, szPreviewEntry, m_nNumPreviewPages);
}

#ifdef AFX_CORE3_SEG
#pragma code_seg(AFX_CORE3_SEG)
#endif

void CWinApp::LoadStdProfileSettings()
{
	ASSERT_VALID(this);

	for (int iMRU = 0; iMRU < _AFX_MRU_COUNT; iMRU++)
	{
		char szEntry[16];
		wsprintf(szEntry, szFileEntry, iMRU + 1);
		m_strRecentFiles[iMRU] = GetProfileString(szFileSection, szEntry);
	}
	m_nNumPreviewPages = ::GetPrivateProfileInt(szPreviewSection,
		szPreviewEntry, 0, m_pszProfileName);
		// 0 by default means not set
}

// Profile API helpers
UINT CWinApp::GetProfileInt(LPCSTR lpszSection, LPCSTR lpszEntry, int nDefault)
{
	ASSERT(lpszSection != NULL);
	ASSERT(lpszEntry != NULL);
	ASSERT(m_pszProfileName != NULL);

	return ::GetPrivateProfileInt(lpszSection, lpszEntry, nDefault,
		m_pszProfileName);
}

BOOL CWinApp::WriteProfileInt(LPCSTR lpszSection, LPCSTR lpszEntry, int nValue)
{
	ASSERT(lpszSection != NULL);
	ASSERT(lpszEntry != NULL);
	ASSERT(m_pszProfileName != NULL);

	char szT[16];
	wsprintf(szT, "%d", nValue);
	return ::WritePrivateProfileString(lpszSection, lpszEntry, szT,
		m_pszProfileName);
}

CString CWinApp::GetProfileString(LPCSTR lpszSection, LPCSTR lpszEntry,
			LPCSTR lpszDefault /*= NULL */)
{
	ASSERT(lpszSection != NULL);
	ASSERT(lpszEntry != NULL);
	ASSERT(m_pszProfileName != NULL);

	if (lpszDefault == NULL)
		lpszDefault = "";       // don't pass in NULL
	char szT[_MAX_PATH];
	::GetPrivateProfileString(lpszSection, lpszEntry, lpszDefault,
		szT, sizeof(szT), m_pszProfileName);
	return szT;
}

BOOL CWinApp::WriteProfileString(LPCSTR lpszSection, LPCSTR lpszEntry,
			LPCSTR lpszValue)
{
	ASSERT(lpszSection != NULL);
	ASSERT(m_pszProfileName != NULL);

	return ::WritePrivateProfileString(lpszSection, lpszEntry, lpszValue,
		m_pszProfileName);
}

/////////////////////////////////////////////////////////////////////////////
