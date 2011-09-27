// sectset.cpp : implementation of the CSectionSet class
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
#include "enroll.h"
#include "sectset.h"

/////////////////////////////////////////////////////////////////////////////
// CSectionSet implementation

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
	m_nParams = 1;
	m_strCourseIDParam = "";

	m_nDefaultType = dynaset;
}

CSectionSet::~CSectionSet()
{
	// delete list of dynamically added columns
	while (!m_listName.IsEmpty())
	{
		m_listName.RemoveHead();
		m_listValue.RemoveHead();
	}
}

CString CSectionSet::GetDefaultConnect()
{
	return "ODBC;DSN=Student Registration;";
}

CString CSectionSet::GetDefaultSQL()
{
	return "DYNABIND_SECTION";
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
	pFX->SetFieldType(CFieldExchange::param);
	RFX_Text(pFX, "CourseIDParam", m_strCourseIDParam);

	// Register dynamically added fields
	pFX->SetFieldType(CFieldExchange::outputColumn);
	POSITION posName = m_listName.GetHeadPosition();
	POSITION posValue = m_listValue.GetHeadPosition();
	while (posName != NULL)
	{
		RFX_Text(pFX, m_listName.GetNext(posName),
			m_listValue.GetNext(posValue));
	}
}


void CSectionSet::AddTextField(CString& strName)
{
	m_listValue.AddTail("");
	m_listName.AddTail(strName);
	m_nFields++;
}

BOOL CSectionSet::FindField(const char* szName)
{
	for (UINT nField = 0; nField != m_nFields; nField++)
	{
		CODBCFieldInfo fi;

		GetODBCFieldInfo(nField, fi);
		if (fi.m_strName.Compare(szName) == 0)
			return TRUE;
	}

	return FALSE;
}
