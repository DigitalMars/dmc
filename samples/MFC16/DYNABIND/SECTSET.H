// sectset.h : interface of the CSectionSet class
//
/////////////////////////////////////////////////////////////////////////////

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
