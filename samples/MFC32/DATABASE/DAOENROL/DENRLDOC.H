// denrldoc.h : interface of the CDaoEnrolDoc class
//
/////////////////////////////////////////////////////////////////////////////

#define HINT_ADD_COURSE 1
#define HINT_DELETE_COURSE 2

class CUpdateHint : public CObject
{
	DECLARE_DYNAMIC(CUpdateHint);
	CUpdateHint();
	CString m_strCourse;
};

class CDaoEnrolDoc : public CDocument
{
protected: // create from serialization only
	CDaoEnrolDoc();
	DECLARE_DYNCREATE(CDaoEnrolDoc)

// Attributes
protected:
	CDaoDatabase m_database;
public:
	CsectionSet m_sectionSet;
	CCourseSet m_courseSet;
	CCourseSet m_courseSetForForm; // for CCourseForm
	CDaoDatabase* GetDatabase(){
		return &m_database;
	}

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDaoEnrolDoc)
	public:
	virtual BOOL OnNewDocument();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CDaoEnrolDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CDaoEnrolDoc)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
