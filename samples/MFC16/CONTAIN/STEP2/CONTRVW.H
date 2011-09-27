// contrvw.h : interface of the CContainView class
//
/////////////////////////////////////////////////////////////////////////////

class CCntrItem;

class CContainView : public CView
{
protected: // create from serialization only
	CContainView();
	DECLARE_DYNCREATE(CContainView)

// Attributes
public:
	CContainDoc* GetDocument();
	// m_pSelection holds the selection to the current CCntrItem.
	// For many applications, such a member variable isn't adequate to
	//  represent a selection, such as a multiple selection or a selection
	//  of objects that are not CCntrItem objects.  This selection
	//  mechanism is provided just to help you get started.

	// TODO: replace this selection mechanism with one appropriate to your app.
	CCntrItem* m_pSelection;

// Operations
public:
	CCntrItem* HitTestItems(CPoint point);
	void SetSelection(CCntrItem* pItem);
	void SetupTracker(CCntrItem* pItem, CRectTracker* pTracker);

// Implementation
public:
	virtual ~CContainView();
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	virtual void OnInitialUpdate(); // called first time after construct
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);

	// Printing support
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

	// OLE Container support
	virtual BOOL IsSelected(const CObject* pDocItem) const;

// Generated message map functions
protected:
	//{{AFX_MSG(CContainView)
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnInsertObject();
	afx_msg void OnCancelEdit();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnEditClear();
	afx_msg void OnUpdateEditClear(CCmdUI* pCmdUI);
	afx_msg void OnEditCopy();
	afx_msg void OnUpdateEditCopy(CCmdUI* pCmdUI);
	afx_msg void OnEditPaste();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in contrvw.cpp
inline CContainDoc* CContainView::GetDocument()
   { return (CContainDoc*)m_pDocument; }
#endif



/////////////////////////////////////////////////////////////////////////////
