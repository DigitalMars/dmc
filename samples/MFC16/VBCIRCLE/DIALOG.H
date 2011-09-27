// dialog.h : header file
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
// CCircleDialog dialog

class CCircleDialog : public CDialog
{
	DECLARE_DYNAMIC(CCircleDialog)
// Construction
public:
	CCircleDialog(CWnd* pParent = NULL);    // standard constructor

// Dialog Data
	// The following is dialog data that we will copy to and from
	// the dialog box on the screen.  Each of these member variables
	// are used in exchange of data from the standard edit controls
	// in the dialog box.

	//{{AFX_DATA(CCircleDialog)
	enum { IDD = IDD_CIRCLE_DLG };
	BOOL    m_bBorder;
	CString m_strCaption;
	CVBControl* m_circle;
	int     m_nShape;
	//}}AFX_DATA

// Implementation
protected:
	virtual BOOL OnInitDialog();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	// Generated message map functions
	//{{AFX_MSG(CCircleDialog)
	afx_msg void OnUpdateCircle();
	afx_msg void OnClickinCircle(UINT, int, CWnd*, LPVOID);
	afx_msg void OnClickoutCircle(UINT, int, CWnd*, LPVOID);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
