// sectform.h : interface of the CSectionForm class
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

class CSectionSet;

class CSectionForm : public CRecordView
{
protected: // create from serialization only
	CSectionForm();
	DECLARE_DYNCREATE(CSectionForm)

public:
	//{{AFX_DATA(CSectionForm)
	enum { IDD = IDD_ENROLL_FORM };
	CStatic m_staticLast;
	CEdit   m_editLast;
	CEdit   m_ctlSection;
	CComboBox   m_ctlCourseList;
	CSectionSet* m_pSet;
	//}}AFX_DATA
// Attributes
public:
	CEnrollDoc* GetDocument();
protected:
	BOOL m_bAddMode;

	// For dynamically added columns
	CObList m_listEdit;
	CObList m_listStatic;

	// For edit control placement
	CRect m_rc;
	UINT m_nOffset;

	// For static control (label) placement
	CRect m_rcStatic;

// Operations
public:
	virtual CRecordset* OnGetRecordset();
	virtual BOOL OnMove(UINT nIDMoveCommand);

// Implementation
public:
	virtual ~CSectionForm();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void OnInitialUpdate(); // called first time after construct

	// Printing support
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// Generated message map functions
protected:
	//{{AFX_MSG(CSectionForm)
	afx_msg void OnSelendokCourselist();
	afx_msg void OnRecordAdd();
	afx_msg void OnRecordRefresh();
	afx_msg void OnRecordDelete();
	//}}AFX_MSG
	afx_msg void OnOptionsAddfields();
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in sectform.cpp
inline CEnrollDoc* CSectionForm::GetDocument()
   { return (CEnrollDoc*)m_pDocument; }
#endif



/////////////////////////////////////////////////////////////////////////////
