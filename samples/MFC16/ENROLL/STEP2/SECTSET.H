// sectset.h : interface of the CSectionSet class
//
/////////////////////////////////////////////////////////////////////////////

class CSectionSet : public CRecordset
{
DECLARE_DYNAMIC(CSectionSet)

public:
	CSectionSet(CDatabase* pDatabase = NULL);

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

// Implementation
protected:
	virtual CString GetDefaultConnect();    // Default connection string
	virtual CString GetDefaultSQL();    // default SQL for Recordset
	virtual void DoFieldExchange(CFieldExchange* pFX);  // RFX support
};
