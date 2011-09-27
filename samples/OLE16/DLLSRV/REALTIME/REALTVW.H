// realtvw.h : interface of the CRealtimeView class
//
/////////////////////////////////////////////////////////////////////////////

class CRealtimeView : public CView
{
protected: // create from serialization only
	CRealtimeView();
	DECLARE_DYNCREATE(CRealtimeView)

// Attributes
public:
	CRealtimeDoc* GetDocument();

// Operations
public:

// Implementation
public:
	virtual ~CRealtimeView();
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif


// Generated message map functions
protected:
	//{{AFX_MSG(CRealtimeView)
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in realtvw.cpp
inline CRealtimeDoc* CRealtimeView::GetDocument()
   { return (CRealtimeDoc*) m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////
