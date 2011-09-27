// coursese.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CCourseSet DAO recordset

class CCourseSet : public CDaoRecordset
{
public:
	CCourseSet(CDaoDatabase* pDatabase = NULL);
	DECLARE_DYNAMIC(CCourseSet)

// Field/Param Data
	//{{AFX_FIELD(CCourseSet, CDaoRecordset)
	CString	m_CourseID;
	CString	m_CourseTitle;
	short	m_Hours;
	//}}AFX_FIELD

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCourseSet)
	public:
	virtual CString GetDefaultSQL();		// Default SQL for Recordset
	virtual void DoFieldExchange(CDaoFieldExchange* pFX);  // RFX support
	//}}AFX_VIRTUAL

// Implementation
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
};
