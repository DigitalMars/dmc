// checkdoc.h : interface of the CChkBookDoc class
//
// This is a part of the Microsoft Foundation Classes C++ library.
// Copyright (C) 1992 Microsoft Corporation
// All rights reserved.
//
// This source code is only intended as a supplement to the
// Microsoft Foundation Classes Reference and Microsoft
// QuickHelp and/or WinHelp documentation provided with the library.
// See these sources for detailed information regarding the
// Microsoft Foundation Classes product.


// This class implements all the details about the transaction-based,
// fixed-length record document that are specific to the check book
// document.  Generic transaction-based, fixed-length record document
// behavior is implemented in its based class, CFixedLenRecDoc.
/////////////////////////////////////////////////////////////////////////////


class CChkBookDoc : public CFixedLenRecDoc
{
protected:  // create from dyunamic creation only
	CChkBookDoc();
	DECLARE_DYNCREATE(CChkBookDoc)

// Attributes
	UINT m_nActiveRecord;

	// Extra header (in addition to that of CFixedLenRecDoc)
	struct
	{
		DWORD dwFileSignature;
		UINT nFirstCheckNo;
	} m_extraHeader;

	// Each check is written in a fixed length record.
	struct
	{
		DWORD   dwCents;
		char    szPayTo[40];
		char    szDate[10];
		char    szMemo[40];
	} m_record;

// Overrides of CFixedLenRecDoc and CDocument
	BOOL OnOpenDocument(const char* pszPathName);
	BOOL OnSaveDocument(const char* pszPathName);
	BOOL SaveModified();
	void* OnCreateNewRecord(int nNewRecordIndex);
	BOOL OnReadExtraHeader();
	void OnWriteExtraHeader(BOOL bNewHeader);

// Operations, called by book view and check view
	void GetCheck(UINT nCheckNo, DWORD& dwCents, CString& strPayTo,
			CString& strDate, CString& strMemo);
	void UpdateCheck(CView* pSourceView, UINT nCheckNo, DWORD dwCents,
			const char* szPayTo, const char* szDate,
			const char* szMemo);
	void ChangeSelectionNextCheckNo(BOOL bNext);
	void ChangeSelectionToCheckNo(UINT nCheckNo);
	UINT GetActiveCheckNo();
	UINT GetFirstCheckNo();
	UINT GetLastCheckNo();

// Implementation
	void UpdateIniFileWithDocPath(const char* pszPathName);
	BOOL MaybeCommitDirtyCheck();
	void PackRecord(DWORD dwCents, const char* szPayTo, const char* szDate,
		const char* szMemo);
	void ParseRecord(DWORD& dwCents, CString& strPayTo,
			CString& strDate, CString& strMemo);
	UINT CheckNoToRecordIndex(UINT nCheckNo);
	UINT RecordIndexToCheckNo(UINT nRecordIndex);

public:
	virtual ~CChkBookDoc();

// Generated message map functions
protected:
	//{{AFX_MSG(CChkBookDoc)
	afx_msg void NewCheck();
	afx_msg void OnNextCheck();
	afx_msg void OnUpdateNextCheck(CCmdUI* pCmdUI);
	afx_msg void OnPrevCheck();
	afx_msg void OnUpdatePrevCheck(CCmdUI* pCmdUI);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
