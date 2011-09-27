// checkdoc.cpp : implementation of the CChkBookDoc class
//
// This is a part of the Microsoft Foundation Classes C++ library.
// Copyright (C) 1992-1995 Microsoft Corporation
// All rights reserved.
//
// This source code is only intended as a supplement to the
// Microsoft Foundation Classes Reference and related
// electronic documentation provided with the library.
// See these sources for detailed information regarding the
// Microsoft Foundation Classes product.


#include "stdafx.h"
#include "chkbook.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

#define FIRST_CHECK_NO 101
#define CHECK_BOOK_FILE_SIGNATURE 0xd6f7e471

/////////////////////////////////////////////////////////////////////////////
// CChkBookDoc
//

IMPLEMENT_DYNCREATE(CChkBookDoc, CFixedLenRecDoc)

BEGIN_MESSAGE_MAP(CChkBookDoc, CFixedLenRecDoc)
	//{{AFX_MSG_MAP(CChkBookDoc)
	ON_COMMAND(ID_EDIT_NEW_CHECK, NewCheck)
	ON_COMMAND(ID_NEXT_CHECK, OnNextCheck)
	ON_UPDATE_COMMAND_UI(ID_NEXT_CHECK, OnUpdateNextCheck)
	ON_COMMAND(ID_PREV_CHECK, OnPrevCheck)
	ON_UPDATE_COMMAND_UI(ID_PREV_CHECK, OnUpdatePrevCheck)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CChkBookDoc construction/destruction

CChkBookDoc::CChkBookDoc()
{
	m_header.nRecordLength = sizeof(m_record);
	m_header.nExtraHeaderLength = sizeof(m_extraHeader);
	m_header.nRecordCount = 0;
	m_nActiveRecord = 0;
	m_extraHeader.nFirstCheckNo = FIRST_CHECK_NO;
	m_extraHeader.dwFileSignature = CHECK_BOOK_FILE_SIGNATURE;
#ifdef _UNICODE
	m_extraHeader.bUnicode = TRUE;
#else
	m_extraHeader.bUnicode = FALSE;
#endif
}

CChkBookDoc::~CChkBookDoc()
{
}

/////////////////////////////////////////////////////////////////////////////
// Overrides of CFixedLenRecDoc and CDocument

BOOL CChkBookDoc::OnOpenDocument(LPCTSTR lpszPathName)
{
	// Upon opening the document, tell the application object
	// to save the path name in the private INI file.

	if (!CFixedLenRecDoc::OnOpenDocument(lpszPathName))
		return FALSE;
	UpdateIniFileWithDocPath(lpszPathName);
	m_nActiveRecord = 0;
	return TRUE;
}

BOOL CChkBookDoc::OnSaveDocument(LPCTSTR lpszPathName)
{
	// Upon saving the document, tell the application object
	// to save the path name in the private INI file.

	if (!CFixedLenRecDoc::OnSaveDocument(lpszPathName))
		return FALSE;
	UpdateIniFileWithDocPath(lpszPathName);
	return TRUE;
}

BOOL CChkBookDoc::SaveModified()
{
	// If the user has been editing a check in the check view
	// but hasn't commited it yet, ask her whether she wants to
	// commit the check.  She might response with 'Cancel', in
	// which case MaybeCommitDirtyCheck() returns FALSE, which
	// SaveModified() in turn returns to cancel the File Close.

	return MaybeCommitDirtyCheck();
}

void CChkBookDoc::UpdateIniFileWithDocPath(LPCTSTR lpszPathName)
{
	theApp.UpdateIniFileWithDocPath(lpszPathName);
}

void* CChkBookDoc::OnCreateNewRecord(int nNewRecordIndex)
{
	// The base class CFixedLenRecDoc calls this override to
	// format a new record (in memory).

	DWORD dwCents = 0L;
	TCHAR date[9];
	_tstrdate(date);
	CString strDate(date);
	CString strPayTo;
	CString strMemo;
	PackRecord(dwCents, strPayTo, strDate, strMemo);
	m_nActiveRecord = nNewRecordIndex;
	return &m_record;
}

BOOL CChkBookDoc::OnReadExtraHeader(CFile* pFile)
{
	// Read the ChkBook-specific portion of the file header,
	// and verify the file signature to make sure we're not
	// reading a non-ChkBook file.

	if (pFile->Read(&m_extraHeader, sizeof(m_extraHeader))
		< sizeof(m_extraHeader))
		return FALSE;
#ifdef _UNICODE
	if (!m_extraHeader.bUnicode)
	{
		AfxMessageBox(IDS_CANNOT_READ_UNICODE_FILE);
		return FALSE;
	}
#else
	if (m_extraHeader.bUnicode)
	{
		AfxMessageBox(IDS_CANNOT_READ_ANSI_FILE);
		return FALSE;
	}
#endif
	return (m_extraHeader.dwFileSignature == CHECK_BOOK_FILE_SIGNATURE);
}

void CChkBookDoc::OnWriteExtraHeader(CFile* pFile, BOOL bNewHeader)
{
	pFile->Write(&m_extraHeader, sizeof(m_extraHeader));

	// If this is a new header (that is, if the first is first being
	// created), then create the first record.

	if (bNewHeader)
	{
		ASSERT(pFile == &m_file);
		CreateNewRecord();
	}
}

/////////////////////////////////////////////////////////////////////////////
// Operations, called by book view and check view

void CChkBookDoc::GetCheck(UINT nCheckNo, DWORD& dwCents, CString& strPayTo,
		CString& strDate, CString& strMemo)
{
	UINT nRecord = CheckNoToRecordIndex(nCheckNo);
	GetRecord(nRecord, &m_record);
	ParseRecord(dwCents, strPayTo, strDate, strMemo);
}


void CChkBookDoc::UpdateCheck(CView* pSourceView, UINT nCheckNo,
		DWORD dwCents, LPCTSTR lpszPayTo, LPCTSTR lpszDate,
		LPCTSTR lpszMemo)
{
	UINT nRecord = CheckNoToRecordIndex(nCheckNo);
	PackRecord(dwCents, lpszPayTo, lpszDate, lpszMemo);
	UpdateRecord(pSourceView, nRecord, &m_record);
	ASSERT_VALID(GetDocTemplate());
}


void CChkBookDoc::ChangeSelectionNextCheckNo(BOOL bNext)
{
	if (bNext)
	{
		if (m_nActiveRecord < (GetRecordCount() - 1))
		{
			if (!MaybeCommitDirtyCheck())
				return;
			UpdateAllViewsWithRecord(NULL, ++m_nActiveRecord);
		}
	}
	else
	{
		if (m_nActiveRecord > 0)
		{
			if (!MaybeCommitDirtyCheck())
				return;
			UpdateAllViewsWithRecord(NULL, --m_nActiveRecord);
		}
	}
}

void CChkBookDoc::ChangeSelectionToCheckNo(UINT nNewActiveCheckNo)
{
	if (!MaybeCommitDirtyCheck())
		return;
	m_nActiveRecord = CheckNoToRecordIndex(nNewActiveCheckNo);
	UpdateAllViewsWithRecord(NULL, m_nActiveRecord);
}

BOOL CChkBookDoc::MaybeCommitDirtyCheck()
{
	CView* pView;
	POSITION pos = GetFirstViewPosition();
	while (pos != NULL)
	{
		pView = GetNextView(pos);
		CCheckView* pCheckView = DYNAMIC_DOWNCAST(CCheckView, pView);
		if (pCheckView != NULL)
			return pCheckView->MaybeCommitDirtyCheck();
	}
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// Implementation

void CChkBookDoc::PackRecord(DWORD dwCents, LPCTSTR lpszPayTo,
			LPCTSTR lpszDate, LPCTSTR lpszMemo)
{
	m_record.dwCents = dwCents;
	_tcsncpy(m_record.szPayTo, lpszPayTo, sizeof(m_record.szPayTo)/sizeof(TCHAR) - 1);
	m_record.szPayTo[sizeof(m_record.szPayTo)/sizeof(TCHAR) - 1] = 0;
	_tcsncpy(m_record.szDate, lpszDate, sizeof(m_record.szDate)/sizeof(TCHAR) - 1);
	m_record.szDate[sizeof(m_record.szDate)/sizeof(TCHAR) - 1] = 0;
	_tcsncpy(m_record.szMemo, lpszMemo, sizeof(m_record.szMemo)/sizeof(TCHAR) - 1);
	m_record.szMemo[sizeof(m_record.szMemo)/sizeof(TCHAR) - 1] = 0;

}

void CChkBookDoc::ParseRecord(DWORD& dwCents, CString& strPayTo,
			CString& strDate, CString& strMemo)
{
	dwCents = m_record.dwCents;
	strPayTo = m_record.szPayTo;
	strDate = m_record.szDate;
	strMemo = m_record.szMemo;
}

UINT CChkBookDoc::CheckNoToRecordIndex(UINT nCheckNo)
{
	return (nCheckNo - m_extraHeader.nFirstCheckNo);
}

UINT CChkBookDoc::RecordIndexToCheckNo(UINT nRecordIndex)
{
	return (nRecordIndex + m_extraHeader.nFirstCheckNo);
}

UINT CChkBookDoc::GetActiveCheckNo()
{
	return (m_nActiveRecord + m_extraHeader.nFirstCheckNo);
}

UINT CChkBookDoc::GetFirstCheckNo()
{
	return m_extraHeader.nFirstCheckNo;
}

UINT CChkBookDoc::GetLastCheckNo()
{
	return (m_extraHeader.nFirstCheckNo + GetRecordCount() - 1);
}

/////////////////////////////////////////////////////////////////////////////
// CChkBookDoc commands

void CChkBookDoc::NewCheck()
{
	// Before creating a new record, which will become the new selection,
	// ask the user whether she wants to commit data entered in the
	// check view for the previously selected check.

	if (!MaybeCommitDirtyCheck())
		return;

	m_nActiveRecord = CreateNewRecord();
}

void CChkBookDoc::OnNextCheck()
{
	ChangeSelectionNextCheckNo(TRUE);
}

void CChkBookDoc::OnUpdateNextCheck(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(m_nActiveRecord < (GetRecordCount() - 1));
}

void CChkBookDoc::OnPrevCheck()
{
	ChangeSelectionNextCheckNo(FALSE);
}

void CChkBookDoc::OnUpdatePrevCheck(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(m_nActiveRecord > 0);

}
