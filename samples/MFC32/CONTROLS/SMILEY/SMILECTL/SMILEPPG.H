// SmilePpg.h : Declaration of the CSmilePropPage property page class.

////////////////////////////////////////////////////////////////////////////
// CSmilePropPage : See SmilePpg.cpp.cpp for implementation.

class CSmilePropPage : public COlePropertyPage
{
	DECLARE_DYNCREATE(CSmilePropPage)
	DECLARE_OLECREATE_EX(CSmilePropPage)

// Constructor
public:
	CSmilePropPage();

// Dialog Data
	//{{AFX_DATA(CSmilePropPage)
	enum { IDD = IDD_PROPPAGE_Smile };
	CString	m_strCaption;
	BOOL m_bSad;
	//}}AFX_DATA

// Implementation
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Message maps
protected:
	//{{AFX_MSG(CSmilePropPage)
		// NOTE - ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};
