// firedlg.h : header file
//
// This is a part of the Microsoft Foundation Classes C++ library.
// Copyright (C) 1992-1995 Microsoft Corporation
// All rights reserved.
//
// This source code is only intended as a supplement to the
// Microsoft Foundation Classes Reference and related
// electronic documentation provided with the library.
// See these sources for detailed information regarding the
// Microsoft Foundation Classes product.

/////////////////////////////////////////////////////////////////////////////
// CFireDlg dialog

#include "FireWnd.h"

class CFireDlg : public CDialog
{
// Construction
public:
	CFireDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CFireDlg)
	enum { IDD = IDD_FIRE_DIALOG };
	CSpinButtonCtrl	m_SettingCtrl;
	CEdit	m_Setting;
	CButton	m_Apply;
	CTreeCtrl	m_Property;
	CSliderCtrl	m_Height;
	CTabCtrl	m_Color;
	CProgressCtrl	m_Burn;
	CFireWnd	m_FireWnd;
	int		m_nSetting;
	//}}AFX_DATA

	int m_nSettingMin;
	int m_nSettingMax;

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFireDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;
	UINT m_uTimerID;

	// Generated message map functions
	//{{AFX_MSG(CFireDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnApply();
	afx_msg void OnPropertySelchanged(NMHDR* pNMHDR, LRESULT* pResult); 
	afx_msg void OnColorSelchange(NMHDR* pNMHDR, LRESULT* pResult); 
	afx_msg void OnDestroy();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnSettingChange();
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
