// stdset.cpp : implementation file
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

#include "stdafx.h"
#include "stdreg.h"
#include "stdset.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CStudentSet

IMPLEMENT_DYNAMIC(CStudentSet, CRecordset)

CStudentSet::CStudentSet(CDatabase* pdb)
	: CRecordset(pdb)
{
	//{{AFX_FIELD_INIT(CStudentSet)
	m_StudentID = 0;
	m_Name = "";
	m_GradYear = 0;
	m_nFields = 3;
	//}}AFX_FIELD_INIT
}


CString CStudentSet::GetDefaultConnect()
{
	return "ODBC;DSN=Student Registration;";
}

CString CStudentSet::GetDefaultSQL()
{
	return "STUDENT";
}

void CStudentSet::DoFieldExchange(CFieldExchange* pFX)
{
	//{{AFX_FIELD_MAP(CStudentSet)
	pFX->SetFieldType(CFieldExchange::outputColumn);
	RFX_Long(pFX, "StudentID", m_StudentID);
	RFX_Text(pFX, "Name", m_Name);
	RFX_Int(pFX, "GradYear", m_GradYear);
	//}}AFX_FIELD_MAP
}

/////////////////////////////////////////////////////////////////////////////
// CStudentSet diagnostics

#ifdef _DEBUG
void CStudentSet::AssertValid() const
{
	CRecordset::AssertValid();
}

void CStudentSet::Dump(CDumpContext& dc) const
{
	CRecordset::Dump(dc);
}
#endif //_DEBUG
