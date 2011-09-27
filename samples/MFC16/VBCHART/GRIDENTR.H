// gridentr.h : CGridEntry form view
//
// This is a part of the Microsoft Foundation Classes C++ library.
// Copyright (C) 1992 Microsoft Corporation
// All rights reserved.
//
// This source code is only intended as a supplement to the
// Microsoft Foundation Classes Reference and Microsoft
// QuickHelp and/or WinHelp documentation provided with the library.
// See these sources for detailed information regarding the
// Microsoft Foundation Classes product.



/////////////////////////////////////////////////////////////////////////////
// CMyEdit window

class CMyEdit : public CEdit
{
// Construction
public:
	CMyEdit();

// Attributes
public:

// Operations
public:

// Implementation
public:
	virtual ~CMyEdit();
	BOOL ProcessKeys(UINT nChar);

	// Generated message map functions
protected:
	//{{AFX_MSG(CMyEdit)
	afx_msg UINT OnGetDlgCode();
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

class CGridEntry : public CFormView
{
	DECLARE_DYNCREATE(CGridEntry)
protected:
	CGridEntry();           // protected constructor used by dynamic creation

// Form Data
public:
	//{{AFX_DATA(CGridEntry)
	enum { IDD = IDD_GRIDENTRY };
	CVBControl* m_pGrid;
	//}}AFX_DATA

// Attributes
public:
	BOOL    m_bEventLockout;
	BOOL    m_bSelChange;
	BOOL    m_bInitialized;

	CMyEdit m_edit;

// Operations
public:

// Implementation
protected:
	void SizeToFit();
	void PositionEdit();
	CPoint FindCellPosition(int nRow, int nCol);
	void DoUpdates();


	virtual ~CGridEntry();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	virtual void OnInitialUpdate();     // first time after construct
	// Generated message map functions
	//{{AFX_MSG(CGridEntry)
	afx_msg void OnRowColChangeGrid(UINT, int, CWnd*, LPVOID);
	afx_msg void OnSelChangeGrid(UINT, int, CWnd*, LPVOID);
	afx_msg void OnGotFocusGrid(UINT, int, CWnd*, LPVOID);
	afx_msg void OnChangeGridEdit();
	afx_msg void OnClickGrid(UINT, int, CWnd*, LPVOID);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////
// CGridEnter hint

class CGridEntryHint : public CObject
{
	DECLARE_DYNAMIC(CGridEntryHint)
public:
	CString     m_str;
};

/////////////////////////////////////////////////////////////////////////////
