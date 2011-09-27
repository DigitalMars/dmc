// tmlreavw.h : interface of the CTMLReadView class
//
// Copyright (c) Toxidyne Corporation, 1995. All Rights Reserved.
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
		// NOTE - the ClassExpress will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG	// debug version in tmlreavw.cpp
inline CTMLReadDoc* CTMLReadView::GetDocument()
   { return (CTMLReadDoc*) m_pDocument; }
#endif
