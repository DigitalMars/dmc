// dsectset.h : header file
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
// CDynabindSectionSet recordset

class CDynabindSectionSet : public CRecordset
{
public:
	CDynabindSectionSet(CDatabase* pDatabase);
	DECLARE_DYNAMIC(CDynabindSectionSet)

// Field/Param Data
	//{{AFX_FIELD(CDynabindSectionSet, CRecordset)
	CString m_CourseID;
	CString m_SectionNo;
	CString m_InstructorID;
	CString m_RoomNo;
	CString m_Schedule;
	int     m_Capacity;
	CString m_LabRoomNo;
	CString m_LabSchedule;
	//}}AFX_FIELD


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CDynabindSectionSet)
	public:
	virtual CString GetDefaultConnect();    // Default connection string
	virtual CString GetDefaultSQL();    // Default SQL for Recordset
	virtual void DoFieldExchange(CFieldExchange* pFX);  // RFX support
	//}}AFX_VIRTUAL

// Implementation
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
};
