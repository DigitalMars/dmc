// tmlrdvw.h : interface of the CTMLReadView class
//
// Copyright (c) XYZ Corp., 1994. All Rights Reserved.
//
//

class CTMLReadView : public CView
{
protected: // create from serialization only
	CTMLReadView();
	DECLARE_DYNCREATE(CTMLReadView)

// Attributes
public:
	CTMLReadDoc* GetDocument();

// Operations
public:

// Implementation
public:
	virtual ~CTMLReadView();
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Printing support
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// Generated message map functions
protected:
	//{{AFX_MSG(CTMLReadView)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG	// debug version in tmlrdvw.cpp
inline CTMLReadDoc* CTMLReadView::GetDocument()
   { return (CTMLReadDoc*) m_pDocument; }
#endif
