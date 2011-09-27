// sectset.cpp : implementation file
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
#include "sectset.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSectionSet

IMPLEMENT_DYNAMIC(CSectionSet, CRecordset)

CSectionSet::CSectionSet(CDatabase* pdb)
	: CRecordset(pdb)
{
	//{{AFX_FIELD_INIT(CSectionSet)
	m_CourseID = "";
	m_SectionNo = "";
	m_InstructorID = "";
	m_RoomNo = "";
	m_Schedule = "";
	m_Capacity = 0;
	m_nFields = 6;
	//}}AFX_FIELD_INIT
}


CString CSectionSet::GetDefaultConnect()
{
	return "ODBC;DSN=Student Registration;";
}

CString CSectionSet::GetDefaultSQL()
{
	return "SECTION";
}

void CSectionSet::DoFieldExchange(CFieldExchange* pFX)
{
	//{{AFX_FIELD_MAP(CSectionSet)
	pFX->SetFieldType(CFieldExchange::outputColumn);
	RFX_Text(pFX, "CourseID", m_CourseID);
	RFX_Text(pFX, "SectionNo", m_SectionNo);
	RFX_Text(pFX, "InstructorID", m_InstructorID);
	RFX_Text(pFX, "RoomNo", m_RoomNo);
	RFX_Text(pFX, "Schedule", m_Schedule);
	RFX_Int(pFX, "Capacity", m_Capacity);
	//}}AFX_FIELD_MAP
}

/////////////////////////////////////////////////////////////////////////////
// CSectionSet diagnostics

#ifdef _DEBUG
void CSectionSet::AssertValid() const
{
	CRecordset::AssertValid();
}

void CSectionSet::Dump(CDumpContext& dc) const
{
	CRecordset::Dump(dc);
}
#endif //_DEBUG
