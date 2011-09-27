// PrefDial.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPrefDialog dialog

class CPrefDialog : public CDialog
{
// Construction
public:
	CPrefDialog(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CPrefDialog)
	enum	{	IDD = IDD_PREFDIALOG};
	int		nParVSpace;
	int		nMargin;
	int		nIndent;
	//}}AFX_DATA

// Implementation
protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

	// Generated message map functions
	//{{AFX_MSG(CPrefDialog)
	afx_msg void OnDefault();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

