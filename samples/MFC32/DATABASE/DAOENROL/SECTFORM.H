// sectform.h : interface of the CSectionForm class
//
/////////////////////////////////////////////////////////////////////////////

class CsectionSet;

class CSectionForm : public CDaoRecordView
{
	DECLARE_DYNCREATE(CSectionForm)
public:
	CSectionForm();

public:
	//{{AFX_DATA(CSectionForm)
	enum { IDD = IDD_DAOENROL_FORM };
	CEdit	m_ctlSection;
	CComboBox	m_ctlCourseList;
	CsectionSet* m_pSet;
	//}}AFX_DATA

// Attributes
public:
	CDaoEnrolDoc* GetDocument();
protected:
	BOOL m_bAddMode;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSectionForm)
	public:
	virtual CDaoRecordset* OnGetRecordset();
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void OnInitialUpdate(); // called first time after construct
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual BOOL OnMove(UINT nIDMoveCommand);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CSectionForm();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CSectionForm)
	afx_msg void OnSelendokCourselist();
	afx_msg void OnRecordAdd();
	afx_msg void OnRecordDelete();
	afx_msg void OnRecordRefresh();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in sectform.cpp
inline CDaoEnrolDoc* CSectionForm::GetDocument()
   { return (CDaoEnrolDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////
