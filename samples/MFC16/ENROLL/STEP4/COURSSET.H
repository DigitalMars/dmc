// coursset.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CCourseSet recordset

class CCourseSet : public CRecordset
{
public:
	CCourseSet(CDatabase* pDatabase = NULL);

// Field/Param Data
	//{{AFX_FIELD(CCourseSet, CRecordset)
	CString m_CourseID;
	CString m_CourseTitle;
	float   m_Hours;
	//}}AFX_FIELD


// Implementation
protected:
	virtual CString GetDefaultSQL();    // Default SQL for Recordset
	virtual void DoFieldExchange(CFieldExchange* pFX);  // RFX support
	DECLARE_DYNAMIC(CCourseSet)
};
