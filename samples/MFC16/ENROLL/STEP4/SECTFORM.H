// sectform.h : interface of the CSectionForm class
//
/////////////////////////////////////////////////////////////////////////////

class CSectionSet;

class CSectionForm : public CAddForm
{
public:
	CSectionForm();
	DECLARE_DYNCREATE(CSectionForm)

public:
	//{{AFX_DATA(CSectionForm)
	enum{ IDD = IDD_DIALOG_FORM };
	CEdit   m_ctlSection;
	CComboBox   m_ctlCourseList;
	CSectionSet* m_pSet;
	//}}AFX_DATA
// Attributes
public:
	CEnrollDoc* GetDocument();

// Operations
public:
	virtual CRecordset* OnGetRecordset();
	virtual BOOL OnMove(UINT nIDMoveCommand);

// Implementation
public:
	virtual ~CSectionForm();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void OnInitialUpdate(); // called first time after construct
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);

	// Printing support
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// Generated message map functions
protected:
	//{{AFX_MSG(CSectionForm)
	afx_msg void OnRecordAdd();
	afx_msg void OnRecordRefresh();
	afx_msg void OnSelendokCourselist();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in sectform.cpp
inline CEnrollDoc* CSectionForm::GetDocument()
   { return (CEnrollDoc*)m_pDocument; }
#endif



/////////////////////////////////////////////////////////////////////////////
