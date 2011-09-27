// dialogs.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CChangeText dialog

class CChangeText : public CDialog
{
// Construction
public:
	CChangeText(CWnd* pParent = NULL);  // standard constructor

// Dialog Data
	//{{AFX_DATA(CChangeText)
	enum { IDD = IDD_CHANGE_TEXT };
	CString m_str;
	//}}AFX_DATA

// Implementation
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	// Generated message map functions
	//{{AFX_MSG(CChangeText)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
