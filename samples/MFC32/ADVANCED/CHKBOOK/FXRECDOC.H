// fxrecdoc.h : interface of the CFixedLenRecDoc and CFixedLenRecHint classes
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

// CFixedLenRecDoc implements the behavior of a transaction-based
// document in which each record has a fixed size.  The file has
// a fixed-length header.  The derived class can specify the length
// of an extra header, that is, a header in addition to that required
// by CFixedLenRecDoc.  This is an abstract class that must be
// derived from.

class CFixedLenRecHint : public CObject
{
	DECLARE_DYNAMIC(CFixedLenRecHint)
public:
	CFixedLenRecHint();
};


class CFixedLenRecDoc : public CDocument
{
	DECLARE_DYNAMIC(CFixedLenRecDoc)
public:
	CFixedLenRecDoc();

protected:

	CFile m_file;       // The file is kept open to do read/writes on per
						// transaction basis.

	// The following header, and optional extra header implemented
	// in the derived class, are updated each time a new record is
	// added, or WriteHeader() is called.

	struct
	{
		UINT nRecordCount;          // count of records in the file
		UINT nRecordLength;         // length of fixed-length records
		UINT nExtraHeaderLength;    // length of additional header written
									// by derived class
	} m_header;

// Overridables
	virtual void* OnCreateNewRecord(int nNewRecordIndex) = 0;
									// returns pointer to new record
	virtual BOOL OnReadExtraHeader(CFile* pFile);
	virtual void OnWriteExtraHeader(CFile* pFile, BOOL bNewHeader);

// Operations
public:
	virtual void WriteHeader(CFile* pFile, BOOL bNewHeader);
	virtual BOOL ReadHeader(CFile* pFile);
								// returns FALSE if file can't be interpreted

	// Operations used by views
	virtual UINT CreateNewRecord(); // creates a new record
	UINT GetRecordCount()
		{ return m_header.nRecordCount; }
	virtual void GetRecord(UINT nRecordIndex, void* pRecord);
	virtual void UpdateRecord(CView* pSourceView, UINT nRecordIndex,
		void* pRecord);
	virtual void UpdateAllViewsWithRecord(CView* pSourceView,
		UINT nRecordIndex);

// Implementation
protected:
	// Overrides of CDocument member functions
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
									// reopen file using m_file
	virtual void DeleteContents();  // closes the m_file

	virtual void FileSeekRecord(UINT nRecord);

public:
	virtual ~CFixedLenRecDoc();
	virtual void Serialize(CArchive& ar);
};
