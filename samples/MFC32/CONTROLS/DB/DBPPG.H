// dbppg.h : Declaration of the CDbPropPage property page class.
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

////////////////////////////////////////////////////////////////////////////
// CDbPropPage : See dbppg.cpp for implementation.

class CDbPropPage : public COlePropertyPage
{
	DECLARE_DYNCREATE(CDbPropPage)
	DECLARE_OLECREATE_EX(CDbPropPage)

// Constructor
public:
	CDbPropPage();

// Dialog Data
	//{{AFX_DATA(CDbPropPage)
	enum { IDD = IDD_PROPPAGE_DB };
	CString	m_dataSource;
	CString	m_columnName;
	CString	m_tableName;
	//}}AFX_DATA
	
// Implementation
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Message maps
protected:
	//{{AFX_MSG(CDbPropPage)
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeDatasource();
	afx_msg void OnSelchangeColumns();
	afx_msg void OnSelchangeTables();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};
