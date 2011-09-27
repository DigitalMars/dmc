// DaoEditPpg.h : Declaration of the CDaoEditPropPage property page class.

////////////////////////////////////////////////////////////////////////////
// CDaoEditPropPage : See DaoEditPpg.cpp.cpp for implementation.

class CDaoEditPropPage : public COlePropertyPage
{
	DECLARE_DYNCREATE(CDaoEditPropPage)
	DECLARE_OLECREATE_EX(CDaoEditPropPage)

// Constructor
public:
	CDaoEditPropPage();

// Dialog Data
	//{{AFX_DATA(CDaoEditPropPage)
	enum { IDD = IDD_PROPPAGE_DAOEDIT };
		// NOTE - ClassWizard will add data members here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_DATA

// Implementation
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Message maps
protected:
	//{{AFX_MSG(CDaoEditPropPage)
		// NOTE - ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};
