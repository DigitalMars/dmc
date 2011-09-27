// MainFrm.h : interface of the CMainFrame class
//

// This is a part of the Microsoft Foundation Classes C++ library.
// Copyright (C) 1992-1996 Microsoft Corporation
// All rights reserved.
//
// This source code is only intended as a supplement to the
// Microsoft Foundation Classes Reference and related
// electronic documentation provided with the library.
// See these sources for detailed information regarding the
// Microsoft Foundation Classes product.


#include "termedit.h"
#include "commctrl.h"

#ifndef _MAINFRM_H_
#define _MAINFRM_H_

#define ID_TERMEDIT 2000  // Ctrl ID for edit control
#define ID_COMMCTRL 2001  // Ctrl ID for comm control

class CMainFrame : public CFrameWnd
{
public: 
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)

// Attributes
public:
	BOOL		m_bCancel;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMainFrame)
	public:
	virtual void RecalcLayout(BOOL bNotify = TRUE);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CMainFrame();

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  
	CStatusBar  m_wndStatusBar;
	CToolBar    m_wndToolBar;
	
	CTermEdit   m_edit;
	CCommCtrl   m_commctrl;
	CFile       m_file;
	CString     m_strLogFile;
	CString     m_strPhoneNum;

// Generated message map functions
protected:
	//{{AFX_MSG(CMainFrame)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
	afx_msg void OnClose();
	afx_msg void OnUpdateFile(CCmdUI* pCmdUI);
	afx_msg void OnFileOpenlog();
	afx_msg void OnFileCloselog();
	afx_msg void OnFileTransmit();
	afx_msg void OnUpdateCommPort(CCmdUI* pCmdUI);
	afx_msg void OnCommPort(UINT nID);
	afx_msg void OnUpdatePropDTREnable(CCmdUI* pCmdUI);
	afx_msg void OnProperties(UINT nID);
	//}}AFX_MSG
	afx_msg void OnCommEvent();

	DECLARE_EVENTSINK_MAP()
	DECLARE_MESSAGE_MAP()

public:
	CTermEdit* GetEditCtrl(){return &m_edit;}
	CCommCtrl* GetCommCtrl(){return &m_commctrl;}

	void ShowData(LPCTSTR strData);
	void DoEvents();
};

#endif // _MAINFRM_H_
/////////////////////////////////////////////////////////////////////////////
