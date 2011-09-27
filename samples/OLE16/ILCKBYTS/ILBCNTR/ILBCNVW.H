// ilbcnvw.h : interface of the CIlbcntrView class
//
/////////////////////////////////////////////////////////////////////////////

class CIlbcntrView : public CView
{
protected: // create from serialization only
	CIlbcntrView();
	DECLARE_DYNCREATE(CIlbcntrView)

// Attributes
public:
	CIlbcntrDoc* GetDocument();

// Operations
public:

// Implementation
public:
	virtual ~CIlbcntrView();
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif


// Generated message map functions
protected:
	//{{AFX_MSG(CIlbcntrView)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in ilbcnvw.cpp
inline CIlbcntrDoc* CIlbcntrView::GetDocument()
   { return (CIlbcntrDoc*) m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////
