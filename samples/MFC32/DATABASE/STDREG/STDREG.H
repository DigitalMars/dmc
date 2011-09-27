// stdreg.h : main header file for the STDREG application
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

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

typedef CMap<SWORD,SWORD,CString,LPCSTR> CMapSQLTypeToSyntax;

/////////////////////////////////////////////////////////////////////////////
// CStdRegSetupApp:
// See stdreg.cpp for the implementation of this class
//

class CStdRegSetupApp : public CWinApp
{
public:
	CStdRegSetupApp();
	~CStdRegSetupApp();

// Attributes
protected:
	CMapSQLTypeToSyntax m_mapSQLTypeToSyntax;
public:
	CDatabase m_db;

// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CStdRegSetupApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Operations
public:
	void AddDataSource();
	BOOL GetColumnSyntax();
	void InitializeData();

// Implementation
protected:
	void ShowProgress(int nTablesDone);
	BOOL ExecuteSQLAndReportFailure(const CString& strSQL);
	BOOL DropThenAddTable(const CString& strTableName, const CString& strColumns);
	void AddColumn(CString& strColumns, LPCSTR lpszColumnName, SWORD fSqlType,
		LPCSTR lpszColLength = NULL);
	BOOL AddCourseTable();
	BOOL AddStudentTable();
	BOOL AddInstructorTable();
	BOOL AddSectionTable();
	BOOL AddDynabindSectionTable();
	BOOL AddEnrollmentTable();

	//{{AFX_MSG(CStdRegSetupApp)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////
