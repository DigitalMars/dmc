// MaskView.h : interface of the CMaskView class
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

#include "msmask.h"
#include "mcsdsc.h"

class CMaskView : public CView
{
protected: // create from serialization only
	CMaskView();
	DECLARE_DYNCREATE(CMaskView)

// Attributes
public:
	CString m_boundCol;
	CMDIBindDoc* GetDocument();

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMaskView)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CMaskView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	CMsmask* m_pMaskCtl;
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint );

// Generated message map functions
protected:
	//{{AFX_MSG(CMaskView)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in MaskView.cpp
inline CMDIBindDoc* CMaskView::GetDocument()
   { return (CMDIBindDoc*) m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////
