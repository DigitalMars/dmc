// NwindDlg.h : header file
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

/////////////////////////////////////////////////////////////////////////////
// CNwindDialog dialog
#include "msdgrid.h"
#include "msacal70.h"
#include "msmask.h"

class CNwindDialog : public CDialog
{
// Construction
public:
	CNwindDialog(CWnd* pParent);   // standard constructor
    BOOL Create();

// Dialog Data
	//{{AFX_DATA(CNwindDialog)
	enum { IDD = IDD_NWIND };
	int		m_ShowDate;
	int		m_ShowName;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CNwindDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

// Implementation
protected:

	CWnd* m_pParent;
	int m_nID;
	CMsDgridCtrl m_pGrid;
	CMsacal70 m_date;
	CMsmask m_lName;

	void BindCalendar();

	// Generated message map functions
	//{{AFX_MSG(CNwindDialog)
	virtual void OnOK();
	virtual void OnCancel();
	virtual BOOL OnInitDialog();
	afx_msg void OnChangeDate();
	afx_msg void OnChangeName();
	afx_msg void OnDestroy();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	// Event Handler functions
	afx_msg BOOL OnRequestEdit(UINT nCtl, BOOL* pBool);
	afx_msg BOOL OnChanged(UINT nCtl);
	DECLARE_EVENTSINK_MAP()
};
