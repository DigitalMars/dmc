// HttpSvrView.h : interface of the CHttpSvrView class
//
// This is a part of the Microsoft Foundation Classes C++ library.
// Copyright (C) 1996 Microsoft Corporation
// All rights reserved.
//
// This source code is only intended as a supplement to the
// Microsoft Foundation Classes Reference and related
// electronic documentation provided with the library.
// See these sources for detailed information regarding the
// Microsoft Foundation Classes product.
//
// Note that your filters probably won't have an #ifdef like this one.
// The project file for this sample allows you to build the sample as
// a statically linked regular MFC DLL (that is, with _AFXDLL defined)
// or as a DLL that doesn't use MFC classes aside from the ISAPI 
// support classes (that is, without _AFXDLL defined).

#define C_COLUMNS		6
#define COLUMN_FILE		0
#define COLUMN_PATH		1
#define COLUMN_HITS		2
#define COLUMN_LAST		3
#define COLUMN_CMD		4
#define COLUMN_URL		5

class CRequestSocket;

class CHttpSvrView : public CListView
{
protected: // create from serialization only
	BOOL InsertHitDoc( CHitDoc* pHitDoc );
	CHitDoc* m_phdPopup;
	CHttpSvrView();
	DECLARE_DYNCREATE(CHttpSvrView)

// Attributes
public:
	CHttpSvrDoc* GetDocument();

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CHttpSvrView)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext = NULL);
	protected:
	virtual void OnInitialUpdate(); // called first time after construct
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CHttpSvrView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	void SetListView( DWORD dwStyle );
	void RegisterHit( CListCtrl& list, CRequest* pRequest );
	int GetImage( CHitDoc* pHit );
	void DoContextMenu( const CPoint& point );

	CImageList m_ilLarge, m_ilSmall;
// Generated message map functions
protected:
	//{{AFX_MSG(CHttpSvrView)
	afx_msg void OnViewLarge();
	afx_msg void OnUpdateViewLarge(CCmdUI* pCmdUI);
	afx_msg void OnViewList();
	afx_msg void OnUpdateViewList(CCmdUI* pCmdUI);
	afx_msg void OnViewReport();
	afx_msg void OnUpdateViewReport(CCmdUI* pCmdUI);
	afx_msg void OnViewSmall();
	afx_msg void OnUpdateViewSmall(CCmdUI* pCmdUI);
	afx_msg void OnColumnclick(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnGetDispInfo(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDeleteItem(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnViewClear();
	//}}AFX_MSG
#ifndef _MAC
	afx_msg void OnDblclk(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnRclick(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnPopupClear();
	afx_msg void OnPopupEdit();
	afx_msg void OnPopupOpen();
#endif // !_MAC
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in HttpSvrView.cpp
inline CHttpSvrDoc* CHttpSvrView::GetDocument()
   { return (CHttpSvrDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////
