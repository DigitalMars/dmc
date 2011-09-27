// ilbsvdoc.h : interface of the CIlbsvrDoc class
//
/////////////////////////////////////////////////////////////////////////////

class CIlbsvrDoc : public CDocument
{
protected: // create from serialization only
	CIlbsvrDoc();
	DECLARE_DYNCREATE(CIlbsvrDoc)

// Attributes
public:

// Operations
public:

// Implementation
public:
	virtual ~CIlbsvrDoc();
	virtual void Serialize(CArchive& ar);   // overridden for document i/o
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
protected:
	virtual BOOL    OnNewDocument();

// Generated message map functions
protected:
	//{{AFX_MSG(CIlbsvrDoc)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
