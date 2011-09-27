// sectset.h : interface of the CSectionSet class
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

class CSectionSet : public CRecordset
{
DECLARE_DYNAMIC(CSectionSet)

public:
	CSectionSet(CDatabase* pDatabase = NULL);
	CSectionSet::~CSectionSet();

// Field/Param Data
	//{{AFX_FIELD(CSectionSet, CRecordset)
	CString m_CourseID;
	CString m_SectionNo;
	CString m_InstructorID;
	CString m_RoomNo;
	CString m_Schedule;
	int m_Capacity;
	//}}AFX_FIELD
	CString m_strCourseIDParam;

// Attributes
	// Late bound field info
	CStringList m_listName;
	CStringList m_listValue;

// Operations
	// Late bind to a text field
	void AddTextField(CString& strName);

	// Find an output field by name
	BOOL FindField(const char *);

// Implementation
protected:
	virtual CString GetDefaultConnect();    // Default connection string
	virtual CString GetDefaultSQL();    // default SQL for Recordset
	virtual void DoFieldExchange(CFieldExchange* pFX);  // RFX support
};
