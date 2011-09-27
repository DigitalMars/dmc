// SectForm.h : interface of the CSectionForm class
//
/////////////////////////////////////////////////////////////////////////////

class CSectionSet;

class CSectionForm : public CRecordView
{
protected: // create from serialization only
	CSectionForm();
	DECLARE_DYNCREATE(CSectionForm)

public:
	//{{AFX_DATA(CSectionForm)
	enum{ IDD = IDD_ENROLL_FORM };
	CSectionSet* m_pSet;
	//}}AFX_DATA

// Attributes
public:
	CEnrollDoc* GetDocument();

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSectionForm)
	public:
	virtual CRecordset* OnGetRecordset();
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void OnInitialUpdate(); // called first time after construct
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
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
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in SectForm.cpp
inline CEnrollDoc* CSectionForm::GetDocument()
   { return (CEnrollDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////
