// contrdoc.h : interface of the CContainDoc class
//
/////////////////////////////////////////////////////////////////////////////

#define HINT_UPDATE_WINDOW  0
#define HINT_UPDATE_ITEM    1

class CContainDoc : public COleDocument
{
protected: // create from serialization only
	CContainDoc();
	DECLARE_DYNCREATE(CContainDoc)

// Attributes
public:

// Operations
public:

// Implementation
public:
	virtual ~CContainDoc();
	virtual void Serialize(CArchive& ar);   // overridden for document i/o
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	virtual BOOL OnNewDocument();

// Generated message map functions
protected:
	//{{AFX_MSG(CContainDoc)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
