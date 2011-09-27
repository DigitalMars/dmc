// sectset.h : interface of the CsectionSet class
//
/////////////////////////////////////////////////////////////////////////////

class CsectionSet : public CDaoRecordset
{
public:
	CsectionSet(CDaoDatabase* pDatabase = NULL);
	DECLARE_DYNAMIC(CsectionSet)

// Field/Param Data
	//{{AFX_FIELD(CsectionSet, CDaoRecordset)
	CString	m_CourseID;
	CString	m_SectionNo;
	CString	m_InstructorID;
	CString	m_RoomNo;
	CString	m_Schedule;
	short	m_Capacity;
	//}}AFX_FIELD
	CString m_strCourseIDParam;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CsectionSet)
	public:
	virtual CString GetDefaultSQL(); 	// default SQL for Recordset
	virtual void DoFieldExchange(CDaoFieldExchange* pFX);	// RFX support
	//}}AFX_VIRTUAL

// Implementation
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

};
