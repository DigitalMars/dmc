// EnrolDoc.h : interface of the CEnrollDoc class
//
/////////////////////////////////////////////////////////////////////////////


class CEnrollDoc : public CDocument
{
protected: // create from serialization only
	CEnrollDoc();
	DECLARE_DYNCREATE(CEnrollDoc)

// Attributes
public:
	CSectionSet m_sectionSet;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CEnrollDoc)
	public:
	virtual BOOL OnNewDocument();
	//}}AFX_VIRTUAL


// Implementation
public:
	virtual ~CEnrollDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CEnrollDoc)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
