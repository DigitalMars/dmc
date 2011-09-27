// autocdoc.h : interface of the CClikDoc class
//
/////////////////////////////////////////////////////////////////////////////

class CClikDoc : public CDocument
{
protected: // create from serialization only
	CClikDoc();
	DECLARE_DYNCREATE(CClikDoc)

// Attributes
public:
	CPoint m_pt;
	CString m_str;

// Operations
public:
	void Refresh();

// Implementation
public:
	virtual ~CClikDoc();
	virtual void Serialize(CArchive& ar);   // overridden for document i/o
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	virtual BOOL OnNewDocument();

// Generated message map functions
protected:
	//{{AFX_MSG(CClikDoc)
	afx_msg void OnEditChangeText();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	// Generated OLE dispatch map functions
	//{{AFX_DISPATCH(CClikDoc)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_DISPATCH
	DECLARE_DISPATCH_MAP()
};

/////////////////////////////////////////////////////////////////////////////
