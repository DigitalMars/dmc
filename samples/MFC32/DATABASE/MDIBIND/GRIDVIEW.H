// GridView.h : interface of the CGridView class
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

#include "msdgrid.h"
#include "mcsdsc.h"

class CGridView : public CView
{
protected: // create from serialization only
	CGridView();
	DECLARE_DYNCREATE(CGridView)

// Attributes
public:
	CMDIBindDoc* GetDocument();

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGridView)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CGridView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	CMsDgridCtrl* m_pGridCtl;
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint );

// Generated message map functions
protected:
	//{{AFX_MSG(CGridView)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in GridView.cpp
inline CMDIBindDoc* CGridView::GetDocument()
   { return (CMDIBindDoc*) m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////
