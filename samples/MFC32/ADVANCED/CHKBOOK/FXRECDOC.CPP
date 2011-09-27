// fxrecdoc.cpp : implementation of the CFixedLenRecDoc and
//                CFixedLenRecHint classes
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
#include "fxrecdoc.h"
#include <string.h>

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNAMIC(CFixedLenRecHint, CObject)
IMPLEMENT_DYNAMIC(CFixedLenRecDoc, CDocument)

///////////////////////////////////////////////////////////////////////
CFixedLenRecHint::CFixedLenRecHint()
{
}

CFixedLenRecDoc::CFixedLenRecDoc()
{
	m_header.nRecordCount = 0;
	m_header.nExtraHeaderLength = 0;
}

CFixedLenRecDoc::~CFixedLenRecDoc()
{
}

///////////////////////////////////////////////////////////////////////
// CDocument overrides

BOOL CFixedLenRecDoc::OnOpenDocument(LPCTSTR lpszPathName)
{
	// The standard CDocument::OnOpenDocument() opens the document,
	// deserializes it, and then closes the document.
	// CFixedLenRecDoc::Serialize() only reads the header.
	// We reopen the document's file here so that we can update
	// the file (via CFile::write's) on a per transaction basis.

	if (!CDocument::OnOpenDocument(lpszPathName))
		return FALSE;
	ASSERT(m_file.m_hFile == CFile::hFileNull);
	if (!m_file.Open(lpszPathName,
			CFile::modeReadWrite | CFile::shareExclusive))
	{
		return FALSE;
	}

	// If the file is empty, that is so because the application has
	// just created a new file in its override of CChkBook::OnFileNew.
	// It this case, we need to create a header for the new file.
	// We pass bNewHeader=TRUE to WriteHeader so that the CChkBookDoc
	// takes the opportunity to write the first record in its override
	// of OnWriteHeader.

	if (m_file.GetLength() == 0L)
	{
		WriteHeader(&m_file, TRUE);
	}
	else
	{
		return (ReadHeader(&m_file));
	}

	return TRUE;
}

void CFixedLenRecDoc::DeleteContents()
{
	// We reopened the file in the override of CDocument::OnOpenDocument,
	// after we deserialized the document, so we can update the file
	// on a per transaction basis.
	// Similarly, we now close the file when the framework wants to
	// clear out the CDocument object for potential reuse.

	if (m_file.m_hFile != CFile::hFileNull)
		m_file.Close();
}

///////////////////////////////////////////////////////////////////////
// Operations

void CFixedLenRecDoc::WriteHeader(CFile* pFile, BOOL bNewHeader)
{
	pFile->Seek(0L, CFile::begin);
	pFile->Write(&m_header, sizeof(m_header));
	if (m_header.nExtraHeaderLength > 0)
	{
		// OnWriteExtraHeader must be implemented by the derived class
		OnWriteExtraHeader(pFile, bNewHeader);
	}
}

BOOL CFixedLenRecDoc::OnReadExtraHeader(CFile*)
{
	TRACE0("Derived class has specified extra header but didn't override OnReadExtraHeader");
	ASSERT(FALSE);
	return FALSE;
}

void CFixedLenRecDoc::OnWriteExtraHeader(CFile*, BOOL)
{
	TRACE0("Derived class has specified extra header but didn't override OnWriteExtraHeader");
	ASSERT(FALSE);
}

BOOL CFixedLenRecDoc::ReadHeader(CFile* pFile)
{
	pFile->Seek(0L, CFile::begin);
	if (pFile->Read(&m_header, sizeof(m_header)) < sizeof(m_header))
		return FALSE;
	if (m_header.nExtraHeaderLength < 1)
		return TRUE;

	// OnReadExtraHeader is implemented by the derived class.
	return OnReadExtraHeader(pFile);
}

UINT CFixedLenRecDoc::CreateNewRecord()
{
	// CreateNewRecord is called by the view to create a new fixed
	// length record.  Increment the record count and update the
	// the header to reflect the addition of a new record in the file.
	// Notify all views about the new record.

	UINT nNewRecordIndex = m_header.nRecordCount++;
	WriteHeader(&m_file, FALSE);
	FileSeekRecord(nNewRecordIndex);
	void* pNewRecord = OnCreateNewRecord(nNewRecordIndex);
	m_file.Write(pNewRecord, m_header.nRecordLength);

#ifdef _DEBUG
	// Read back the record just written to see if were written correctly.
	void* pCompareRec = malloc(m_header.nRecordLength);
	FileSeekRecord(nNewRecordIndex);
	if (m_file.Read(pCompareRec, m_header.nRecordLength)
		< m_header.nRecordLength)
		ASSERT(FALSE);
	ASSERT(memcmp(pCompareRec, pNewRecord, m_header.nRecordLength) == 0);
	free(pCompareRec);
#endif

	UpdateAllViewsWithRecord(NULL, nNewRecordIndex);
	return nNewRecordIndex;
}

///////////////////////////////////////////////////////////////////////
// Implementation

void CFixedLenRecDoc::FileSeekRecord(UINT nRecord)
{
	m_file.Seek(sizeof(m_header) + m_header.nExtraHeaderLength
			+ nRecord * m_header.nRecordLength,
		CFile::begin);
}

void CFixedLenRecDoc::GetRecord(UINT nRecordIndex, void* pRecord)
{
	FileSeekRecord(nRecordIndex);
	if (m_file.Read(pRecord, m_header.nRecordLength)
		< m_header.nRecordLength)
			ASSERT(FALSE);
}


void CFixedLenRecDoc::UpdateRecord(CView* pSourceView, UINT nRecordIndex,
		void* pRecord)  // called by view
{
	FileSeekRecord(nRecordIndex);
	m_file.Write(pRecord, m_header.nRecordLength);
	UpdateAllViewsWithRecord(pSourceView, nRecordIndex);
}

void CFixedLenRecDoc::UpdateAllViewsWithRecord(CView* pSourceView,
	UINT nRecordIndex)
{
	CFixedLenRecHint hint;
	UpdateAllViews(pSourceView, MAKELONG(nRecordIndex, 0), &hint);
}

void CFixedLenRecDoc::Serialize(CArchive& ar)
{
	// It's not necessary to store anything in the file on File Save/As,
	// since we have been updating the file on every transaction.
	// We need only read the file header on File Open.

	if (ar.IsStoring())
	{
		if (!ReadHeader(ar.GetFile()))
		{
			// The header can't be interpreted.
			AfxThrowArchiveException(CArchiveException::badSchema);
		}
	}
}
