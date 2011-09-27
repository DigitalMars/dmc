// ccvuiDlg.h : header file
//
// Updated: 21st August 2003
//
////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_CCVUIDLG_H__63533399_8F9C_493C_B25C_E290CC760ABF__INCLUDED_)
#define AFX_CCVUIDLG_H__63533399_8F9C_493C_B25C_E290CC760ABF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CCcvDlg dialog

class CCcvDlg : public CDialog
{
// Construction
public:
	CCcvDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CCcvDlg)
	enum { IDD = IDD_CCVUI_DIALOG };
	CTreeCtrl	m_tree;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCcvDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CCcvDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnQuery();
	afx_msg void OnAbout();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CCVUIDLG_H__63533399_8F9C_493C_B25C_E290CC760ABF__INCLUDED_)