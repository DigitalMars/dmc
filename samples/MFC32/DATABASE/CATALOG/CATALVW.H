// catalvw.h : interface of the CCatalogView class
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

/////////////////////////////////////////////////////////////////////////////

class CTables;
class CColumns;

class CCatalogView : public CRecordView
{
protected: // create from serialization only
	CCatalogView();
	DECLARE_DYNCREATE(CCatalogView)

public:
	//{{AFX_DATA(CCatalogView)
	enum { IDD = IDD_CATALOG_FORM };
	CListBox    m_lbColumns;
	CTables* m_pSet;
	//}}AFX_DATA

// Attributes
public:
	CCatalogDoc* GetDocument();
	CColumns m_Columns;

// Operations
public:
	virtual CRecordset* OnGetRecordset();
	virtual BOOL OnMove(UINT nIDMoveCommand);
	void FillColumnsListBox();

// Implementation
public:
	virtual ~CCatalogView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void OnInitialUpdate(); // called first time after construct

// Generated message map functions
protected:
	//{{AFX_MSG(CCatalogView)
	afx_msg void OnUpdateAppExit(CCmdUI* pCmdUI);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in catalvw.cpp
inline CCatalogDoc* CCatalogView::GetDocument()
   { return (CCatalogDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////
