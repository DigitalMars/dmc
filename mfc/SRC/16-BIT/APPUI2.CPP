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
// DDE and ShellExecute support

// Registration strings (not localized)
static char BASED_CODE szSystemTopic[] = "system";
static char BASED_CODE szShellOpenFmt[] = "%s\\shell\\open\\%s";
static char BASED_CODE szDDEExec[] = "ddeexec";
static char BASED_CODE szCommand[] = "command";
static char BASED_CODE szStdOpen[] = "[open(\"%1\")]";
static char BASED_CODE szStdArg[] = " %1";

#ifdef AFX_INIT_SEG
#pragma code_seg(AFX_INIT_SEG)
#endif

void CWinApp::EnableShellOpen()
{
	ASSERT(m_atomApp == NULL && m_atomSystemTopic == NULL); // do once

	m_atomApp = ::GlobalAddAtom(m_pszExeName);
	m_atomSystemTopic = ::GlobalAddAtom(szSystemTopic);
}


static BOOL NEAR PASCAL SetRegKey(LPCSTR lpszKey, LPCSTR lpszValue)
{
	if (::RegSetValue(HKEY_CLASSES_ROOT, lpszKey, REG_SZ,
		  lpszValue, lstrlen(lpszValue)) != ERROR_SUCCESS)
	{
		TRACE1("Warning: registration database update failed for key '%Fs'\n",
			lpszKey);
		return FALSE;
	}
	return TRUE;
}

void CWinApp::RegisterShellFileTypes()
{
	ASSERT(!m_templateList.IsEmpty());  // must have some doc templates

	char szPathName[_MAX_PATH+10];
	::GetModuleFileName(AfxGetInstanceHandle(), szPathName, _MAX_PATH);
	lstrcat(szPathName, szStdArg);      // "pathname %1"

	CString strFilterExt, strFileTypeId, strFileTypeName;
	CString strTemp;
	POSITION pos = m_templateList.GetHeadPosition();
	while (pos != NULL)
	{
		CDocTemplate* pTemplate = (CDocTemplate*)m_templateList.GetNext(pos);
		if (pTemplate->GetDocString(strFileTypeId,
		   CDocTemplate::regFileTypeId) && !strFileTypeId.IsEmpty())
		{
			// enough info to register it
			if (!pTemplate->GetDocString(strFileTypeName,
			   CDocTemplate::regFileTypeName))
				strFileTypeName = strFileTypeId;    // use id name

			ASSERT(strFileTypeId.Find(' ') == -1);  // no spaces allowed

			// first register the type ID with our server
			if (!SetRegKey(strFileTypeId, strFileTypeName))
				continue;       // just skip it

			// register open command
			char szBuff[_MAX_PATH*2];   // big buffer
			if (!pTemplate->GetDocString(strTemp, CDocTemplate::windowTitle) ||
				strTemp.IsEmpty())
			{
				// only register DDE commands for non-SDI apps
				wsprintf(szBuff, szShellOpenFmt, (LPCSTR)strFileTypeId,
					(LPCSTR)szDDEExec);
				if (!SetRegKey(szBuff, szStdOpen))
					continue;       // just skip it
			}

			// always register command line for all apps
			wsprintf(szBuff, szShellOpenFmt, (LPCSTR)strFileTypeId,
				(LPCSTR)szCommand);
			if (!SetRegKey(szBuff, szPathName))
				continue;       // just skip it

			pTemplate->GetDocString(strFilterExt, CDocTemplate::filterExt);
			if (!strFilterExt.IsEmpty())
			{
				ASSERT(strFilterExt[0] == '.');
				LONG lSize = sizeof(szBuff);

				if (::RegQueryValue(HKEY_CLASSES_ROOT, strFilterExt, szBuff,
					&lSize) != ERROR_SUCCESS || szBuff[0] == '\0')
				{
					// no association for that suffix
					(void)SetRegKey(strFilterExt, strFileTypeId);
				}
			}
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
