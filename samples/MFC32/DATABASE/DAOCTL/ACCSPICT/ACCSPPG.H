// AccsPpg.h : Declaration of the CAccessPictPropPage property page class.

////////////////////////////////////////////////////////////////////////////
// CAccessPictPropPage : See AccsPpg.cpp.cpp for implementation.

class CAccessPictPropPage : public COlePropertyPage
{
	DECLARE_DYNCREATE(CAccessPictPropPage)
	DECLARE_OLECREATE_EX(CAccessPictPropPage)

// Constructor
public:
	CAccessPictPropPage();

// Dialog Data
	//{{AFX_DATA(CAccessPictPropPage)
	enum { IDD = IDD_PROPPAGE_ACCESSPICT };
	BOOL	m_bStretch;
	BOOL	m_bPreserve;
	CString	m_strCaption;
	//}}AFX_DATA

// Implementation
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Message maps
protected:
	//{{AFX_MSG(CAccessPictPropPage)
	afx_msg void OnStretch();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};
