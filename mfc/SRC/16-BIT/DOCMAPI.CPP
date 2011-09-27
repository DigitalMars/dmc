// This is a part of the Microsoft Foundation Classes C++ library.
// Copyright (C) 1992 Microsoft Corporation
// All rights reserved.
//
// This source code is only intended as a supplement to the
// Microsoft Foundation Classes Reference and Microsoft
// QuickHelp and/or WinHelp documentation provided with the library.
// See these sources for detailed information regarding the
// Microsoft Foundation Classes product.

#include "stdafx.h"
#include <mapi.h>

#ifdef AFX_AUX2_SEG
#pragma code_seg(AFX_AUX2_SEG)
#endif

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// MAPI implementation helpers and globals

static BOOL _afxIsMailAvail = (BOOL)-1;    // start out not determined

#ifndef _AFXDLL
static HINSTANCE _afxInstMail;
struct _AFX_DOCMAPI_TERM
{
	~_AFX_DOCMAPI_TERM()
	{
		if (_afxInstMail != NULL)
		{
			::FreeLibrary(_afxInstMail);
			_afxInstMail = NULL;
		}
	}
};

static const _AFX_DOCMAPI_TERM docmapiTerm;
#else
#define _afxInstMail _AfxGetAppData()->appInstMail
#endif

/////////////////////////////////////////////////////////////////////////////
// CDocument MAPI support

void CDocument::OnFileSendMail()
{
	ASSERT_VALID(this);
	ASSERT(_afxIsMailAvail);   // update handler always gets called first

	BeginWaitCursor();

	if (_afxInstMail == NULL)
		_afxInstMail = ::LoadLibrary(_T("MAPI.DLL"));

	if (_afxInstMail == NULL)
	{
		EndWaitCursor();
		AfxMessageBox(AFX_IDP_FAILED_MAPI_LOAD);
		return;
	}
	ASSERT(_afxInstMail != NULL);

	ULONG (FAR PASCAL *lpfnSendMail)(LHANDLE lhSession, ULONG ulUIParam,
		lpMapiMessage lpMessage, FLAGS flFlags, ULONG ulReserved);
	(FARPROC&)lpfnSendMail = GetProcAddress(_afxInstMail, "MAPISendMail");
	if (lpfnSendMail == NULL)
	{
		EndWaitCursor();
		AfxMessageBox(AFX_IDP_INVALID_MAPI_DLL);
		return;
	}
	ASSERT(lpfnSendMail != NULL);

	TCHAR szTempName[_MAX_PATH];
	BOOL bRemoveTemp = FALSE;
	if (m_strPathName.IsEmpty() || IsModified())
	{
		// save to temporary path
		VERIFY(GetTempFileName(GetTempDrive(0), _T("afx"), 0, szTempName) != 0);
		BOOL bModified = IsModified();
		BOOL bResult = DoSave(szTempName, FALSE);
		SetModifiedFlag(bModified);
		if (!bResult)
		{
			EndWaitCursor();
			TRACE0("Warning: file save failed during File.Send Mail.\n");
			return;
		}
		bRemoveTemp = TRUE;
	}
	else
	{
		// use actual file since it isn't modified
		lstrcpyn(szTempName, m_strPathName, sizeof(szTempName));
	}

	// build an appropriate title for the attachment
	char szTitle[_MAX_PATH];
	lstrcpyn(szTitle, m_strTitle, sizeof(szTitle));
	if (m_strTitle.Find('.') == -1)
	{
		// append the default suffix if there is one
		CString strExt;
		CDocTemplate* pTemplate = GetDocTemplate();
		if (pTemplate != NULL && 
			pTemplate->GetDocString(strExt, CDocTemplate::filterExt) &&
		  	!strExt.IsEmpty())
		{
			ASSERT(strExt[0] == '.');
			int nLen = lstrlen(szTitle);
			lstrcpyn(szTitle+nLen, strExt, sizeof(szTitle)-nLen);
		}
	}

	// prepare the file description (for the attachment)
	MapiFileDesc fileDesc;
	memset(&fileDesc, 0, sizeof(fileDesc));
	fileDesc.nPosition = (ULONG)-1;
	fileDesc.lpszPathName = szTempName;
	fileDesc.lpszFileName = szTitle;

	// prepare the message (empty with 1 attachment)
	MapiMessage message;
	memset(&message, 0, sizeof(message));
	message.nFileCount = 1;
	message.lpFiles = &fileDesc;

	// allow MAPI to send the mail message
	HWND hWndParent = _AfxGetSafeOwner(NULL);

	// some extra precautions are required to use MAPISendMail as it
	// tends to enable the parent window in between dialogs (after
	// the login dialog, but before the send note dialog).
	::SetCapture(hWndParent);
	::SetFocus(NULL);
	::SetProp(hWndParent, "StayDisabled", (HANDLE)1);

	ULONG nError = lpfnSendMail(NULL, MAKELONG(hWndParent, 0), &message,
		MAPI_LOGON_UI|MAPI_DIALOG, 0);

	// after returning from the MAPISendMail call, the window must
	// be re-enabled and focus returned to the frame to undo the workaround
	// done before the MAPI call.
	::ReleaseCapture();
	::RemoveProp(hWndParent, "StayDisabled");
	::EnableWindow(hWndParent, TRUE);
	::SetFocus(NULL);
	::SetFocus(hWndParent);

	if (nError != SUCCESS_SUCCESS && 
		nError != MAPI_USER_ABORT && nError != MAPI_E_LOGIN_FAILURE)
	{
		AfxMessageBox(AFX_IDP_FAILED_MAPI_SEND);
	}

	// remove temporary file, if temporary file was used
	if (bRemoveTemp)
		CFile::Remove(szTempName);

	EndWaitCursor();
}

void CDocument::OnUpdateFileSendMail(CCmdUI* pCmdUI)
{
	ASSERT_VALID(this);

	if (_afxIsMailAvail == (BOOL)-1)
	{
		OFSTRUCT of;
		_afxIsMailAvail = ::GetProfileInt(_T("MAIL"), _T("MAPI"), 0) != 0 &&
			OpenFile(_T("MAPI.DLL"), &of, OF_EXIST) != HFILE_ERROR;
	}

	// enable the Send... menu item if available
	pCmdUI->Enable(_afxIsMailAvail);
	CMenu* pMenu = pCmdUI->m_pMenu;
	if (!_afxIsMailAvail && pMenu != NULL)
	{
		// remove the Send... menu and surrounding separators
		UINT nStateAbove = pMenu->GetMenuState(pCmdUI->m_nIndex-1, MF_BYPOSITION);
		UINT nStateBelow = pMenu->GetMenuState(pCmdUI->m_nIndex+1, MF_BYPOSITION);
		pMenu->RemoveMenu(pCmdUI->m_nIndex, MF_BYPOSITION);
		if (nStateAbove & nStateBelow & MF_SEPARATOR)
		{
			// a separator must be removed since the Send... is gone
			if (nStateAbove != (UINT)-1)
				pMenu->RemoveMenu(pCmdUI->m_nIndex-1, MF_BYPOSITION);
			else if (nStateBelow != (UINT)-1)
				pMenu->RemoveMenu(pCmdUI->m_nIndex, MF_BYPOSITION);
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
// COleDocument MAPI support

void COleDocument::OnFileSendMail()
{
	ASSERT_VALID(this);
	ASSERT(m_bRemember);

	LPSTORAGE lpOrigStg = m_lpRootStg;
	m_lpRootStg = NULL;

	TRY
	{
		m_bRemember = FALSE;
		CDocument::OnFileSendMail();
	}
	CATCH_ALL(e)
	{
		m_lpRootStg = lpOrigStg;
		m_bRemember = TRUE;
		THROW_LAST();
	}
	END_CATCH_ALL

	m_lpRootStg = lpOrigStg;
	m_bRemember = TRUE;
}

/////////////////////////////////////////////////////////////////////////////
