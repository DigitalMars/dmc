// statpage.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CStatPage dialog

class CStatPage : public CPropertyPage
{
        DECLARE_DYNCREATE(CStatPage)

// Construction
public:
        CStatPage();
        ~CStatPage();

// Dialog Data
        //{{AFX_DATA(CStatPage)
	enum { IDD = IDD_STAT_PAGE };
	CString	m_strSavedBy;
	CString	m_strRevNum;
	CString	m_strEditTime;
	CString	m_strLastPrint;
	CString	m_strCreateDate;
	CString	m_strLastSave;
	CString	m_strNumPages;
	CString	m_strNumWords;
	CString	m_strNumChars;
	CString	m_strSecurity;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
        //{{AFX_VIRTUAL(CStatPage)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
        //{{AFX_MSG(CStatPage)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};
