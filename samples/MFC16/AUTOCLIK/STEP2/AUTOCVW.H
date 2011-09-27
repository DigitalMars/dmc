// autocvw.h : interface of the CClikView class
//
/////////////////////////////////////////////////////////////////////////////

class CClikView : public CView
{
protected: // create from serialization only
	CClikView();
	DECLARE_DYNCREATE(CClikView)

// Attributes
public:
	CClikDoc* GetDocument();

// Operations
public:

// Implementation
public:
	virtual ~CClikView();
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

	// Printing support
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// Generated message map functions
protected:
	//{{AFX_MSG(CClikView)
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in autocvw.cpp
inline CClikDoc* CClikView::GetDocument()
   { return (CClikDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////
