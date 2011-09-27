// RDCView.h : interface of the CRDCView class
//
/////////////////////////////////////////////////////////////////////////////

// This is a part of the Microsoft Foundation Classes C++ library.
// Copyright (C) 1992-1996 Microsoft Corporation
// All rights reserved.
//
// This source code is only intended as a supplement to the
// Microsoft Foundation Classes Reference and related
// electronic documentation provided with the library.
// See these sources for detailed information regarding the
// Microsoft Foundation Classes product.

#ifndef __RDCVIEW_H__
#define __RDCVIEW_H__

class CRDCView : public CView
{
protected: // create from serialization only
	CRDCView();
	DECLARE_DYNCREATE(CRDCView)

// Attributes
public:
	CMDIBindDoc* GetDocument();
	CMcsDsc* m_pRDCCtl;	//the RDC control in that view

	// Operations
public:
	void Bind(CString &DSN,CString &Sql,CString &User,CString &Pw);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRDCView)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CRDCView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:


// Generated message map functions
protected:
	//{{AFX_MSG(CRDCView)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in RDCView.cpp
inline CMDIBindDoc* CRDCView::GetDocument()
   { return (CMDIBindDoc*)m_pDocument; }
#endif

#endif
/////////////////////////////////////////////////////////////////////////////
