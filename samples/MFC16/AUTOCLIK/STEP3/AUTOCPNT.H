// autocpnt.h : header file
//



/////////////////////////////////////////////////////////////////////////////
// CClikPoint command target

class CClikPoint : public CCmdTarget
{
	DECLARE_DYNCREATE(CClikPoint)
public:
	CClikPoint();

// Attributes
public:

// Operations
public:

// Implementation
public:
	virtual ~CClikPoint();
	virtual void OnFinalRelease();

protected:
	// Generated message map functions
	//{{AFX_MSG(CClikPoint)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	// Generated OLE dispatch map functions
	//{{AFX_DISPATCH(CClikPoint)
	short m_x;
	short m_y;
	//}}AFX_DISPATCH
	DECLARE_DISPATCH_MAP()
};

/////////////////////////////////////////////////////////////////////////////
