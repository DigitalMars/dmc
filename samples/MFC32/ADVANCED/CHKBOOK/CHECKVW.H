// checkvw.h : interface of the CCheckView class
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

// This class implements the check form, which looks something like
// an actual check.  The format of the form is defined by a dialog
// template that can be edited in App Studio.  The resource i.d. of this
// form is IDD_CHECK, which is specified in the form view's constructor.

class CCheckView : public CFormView
{
	DECLARE_DYNCREATE(CCheckView)
protected:
	CCheckView();           // protected constructor used by dynamic creation

// Form Data
protected:
	//{{AFX_DATA(CCheckView)
	enum { IDD = IDD_CHECK };
	UINT    m_nCheckNo;
	CString m_strDate;
	CString m_strMemo;
	CString m_strPayTo;
	DWORD   m_dwCents;
	//}}AFX_DATA

// Attributes
protected:
	CChkBookDoc* GetDocument()
		{
			ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CChkBookDoc)));
			return (CChkBookDoc*) m_pDocument;
		}

// Operations
public:
	// standard overrides of MFC classes
	void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	// called by the document
	BOOL MaybeCommitDirtyCheck();

// Implementation
protected:
	virtual ~CCheckView();

	// Generated message map functions
	//{{AFX_MSG(CCheckView)
	afx_msg void OnEditCommitCheck();
	afx_msg void OnAmountNumChange();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
