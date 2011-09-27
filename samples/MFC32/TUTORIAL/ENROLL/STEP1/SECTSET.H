// SectSet.h : interface of the CSectionSet class
//
/////////////////////////////////////////////////////////////////////////////

class CSectionSet : public CRecordset
{
public:
	CSectionSet(CDatabase* pDatabase = NULL);
	DECLARE_DYNAMIC(CSectionSet)

// Field/Param Data
	//{{AFX_FIELD(CSectionSet, CRecordset)
	CString	m_CourseID;
	CString	m_SectionNo;
	CString	m_InstructorID;
	CString	m_RoomNo;
	CString	m_Schedule;
	int	m_Capacity;
	//}}AFX_FIELD

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSectionSet)
	public:
	virtual CString GetDefaultConnect();	// Default connection string
	virtual CString GetDefaultSQL(); 	// default SQL for Recordset
	virtual void DoFieldExchange(CFieldExchange* pFX);	// RFX support
	//}}AFX_VIRTUAL

// Implementation
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

};
