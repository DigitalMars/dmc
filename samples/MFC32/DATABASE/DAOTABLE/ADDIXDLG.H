// AddIxDlg.h : header file
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
// CAddIndexDlg dialog

class CAddIndexDlg : public CDialog
{
// Construction
public:
	void setFieldListSelections(BOOL bJustClear = FALSE);
	void initializer();
	void indexInitializer();
	CAddIndexDlg(CDaoDatabase *pDatabase, CString strTableName,
				 CWnd* pParent = NULL);
	CAddIndexDlg(CWnd* pParent = NULL);   // standard constructor
	CDaoTableDef *m_pTableDef;
	CDaoDatabase *m_pDatabase;
	BOOL m_bCheckDDV;
	CDaoIndexInfo m_II;
	int m_nIndexIndex;
	void disableControlsForExisting(BOOL bDisable = TRUE);
	void DDV_NoSel(CDataExchange* pDX, CListCtrl *theControl);
	void populateFieldList();
	void createFieldArray(CDaoIndexFieldInfo **pFields, short *nFields);

	// Dialog Data
	//{{AFX_DATA(CAddIndexDlg)
	enum { IDD = IDD_ADD_INDEX_DLG };
	CFieldListCtrl	m_FieldListListControl;
	CButton	m_DescendingControl;
	CButton	m_UniqueControl;
	CButton	m_RequiredControl;
	CButton	m_IgnoreNullControl;
	CButton	m_PrimaryControl;
	CString	m_strTableName;
	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAddIndexDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CAddIndexDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnDone();
	afx_msg void OnClose();
	afx_msg void OnAddIndex();
	afx_msg void OnDeleteIndex();
	afx_msg void OnNextIndex();
	afx_msg void OnPreviousIndex();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
