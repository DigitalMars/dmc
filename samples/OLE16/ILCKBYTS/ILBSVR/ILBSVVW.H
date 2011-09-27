// ilbsvvw.h : interface of the CIlbsvrView class
//
/////////////////////////////////////////////////////////////////////////////

class CIlbsvrView : public CView
{
protected: // create from serialization only
	CIlbsvrView();
	DECLARE_DYNCREATE(CIlbsvrView)

// Attributes
public:
	CIlbsvrDoc* GetDocument();

// Operations
public:

// Implementation
public:
	virtual ~CIlbsvrView();
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif


// Generated message map functions
protected:
	//{{AFX_MSG(CIlbsvrView)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in ilbsvvw.cpp
inline CIlbsvrDoc* CIlbsvrView::GetDocument()
   { return (CIlbsvrDoc*) m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////
