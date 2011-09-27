// AddTbDlg.h : header file
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

// maximum number of types of fields
#define MAX_TYPES 12

/////////////////////////////////////////////////////////////////////////////
// CDlgAddTable dialog

class CDlgAddTable : public CDialog
{
// Construction
public:
	void setTypeDependentStatesAndExisting(int theType, BOOL bExistingField);
	int m_nFieldIndex;
	BOOL m_bCheckDDV;	// if TRUE, execute DDV checks in DoDataExchange
	BOOL m_bFirstFieldAdded;	// TRUE after first field added to tabledef
	int m_nTypeMap[MAX_TYPES];	// combobox list to actual type constants
	CDaoTableDef * m_pTableDef;	// points to the tabledef used to create table
	CDaoDatabase * m_pDatabase; // incoming database pointer on which to base tabledef

	CDlgAddTable(CWnd* pParent = NULL);   // standard constructor
	// constructor that takes a database pointer--the one we really use
	CDlgAddTable(CDaoDatabase *pDatabase, CString strTableName, CWnd * pParent = NULL);
	// check if a type selection was made
	void DDV_NoSel(CDataExchange* pDX, CComboBox *theControl);
	// initialize all the dialog data, other members
	void initializer();
	// initialize the field info (ordinal position is conditional)
	void fieldInitializer(BOOL bInitOrdPos = TRUE);
	// create new field in table with error checks for duplicates/failure
	BOOL CreateNewField();	
	// create new tabledef with error checks for duplicates/failure
	BOOL CreateNewTableDef();	

	// this enum corresponds to order of strings in type combobox
	enum theTypes { Boolean, Byte, Currency, Date, Double, Long,
					LongBinary, Memo, Short, Single, Text };

// Dialog Data
	// controls on the dialog and data they transfer from/to
	//{{AFX_DATA(CDlgAddTable)
	enum { IDD = IDD_ADD_TABLE_DLG };
	CEdit	m_ValidationTextControl;
	CEdit	m_ValidationRuleControl;
	CButton	m_RequireControl;
	CEdit	m_OrdinalPositionControl;
	CEdit	m_TableNameControl;
	CEdit	m_NameControl;
	CEdit	m_SizeControl;
	CEdit	m_DefaultValueControl;
	CButton	m_AutoIncrFieldControl;
	CButton	m_FieldVariabilityControl;
	CComboBox	m_TypeControl;
	CString m_strTableName;
	CDaoFieldInfo m_FI;
	int		m_FieldVariability;
	BOOL	m_bAutoIncrField;
	CString	m_strValidationRule;
	CString	m_strValidationText;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgAddTable)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgAddTable)
	virtual BOOL OnInitDialog();
	afx_msg void OnNextField();	// user selected to add a field or move to next
	afx_msg void OnSelendokType();	// user selected a type for the field
	afx_msg void OnDone();	// user done adding fields, ready to append tabledef
	afx_msg void OnClose();	// user cancelled out of creating a table
	afx_msg void OnPreviousField();
	afx_msg void OnDeleteField();
	afx_msg void OnAddField();
	afx_msg void OnCloseupType();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
