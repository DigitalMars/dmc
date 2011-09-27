// columdlg.h : header file
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
// CColSyntaxDlg dialog

class CColSyntaxDlg : public CDialog
{
// Construction
public:
	CColSyntaxDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CColSyntaxDlg)
	enum { IDD = IDD_COLUMN_SYNTAX_DLG };
	CButton m_ctlOK;
	CListBox    m_ctlTypeInfoList;
	CString m_strSyntax;
	CString m_strListBoxInstruction;
	CString m_strColTypeInstruction;
	//}}AFX_DATA

	CMapSQLTypeToSyntax* m_pMapSQLTypeToSyntax;
	POSITION m_posMapSQLTypeToSyntax;
	SWORD m_swCurrentSQLType;

// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CColSyntaxDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void OnOK();
	//}}AFX_VIRTUAL

// Implementation
protected:
	void GetNextSQLTypeAndUpdateControls();
	void FillTypeInfoListbox();

	// Generated message map functions
	//{{AFX_MSG(CColSyntaxDlg)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
