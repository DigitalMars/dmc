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
// Operations
public:

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
public:
	//{{AFX_DISPATCH(CClikDoc)
	CString m_str;
	afx_msg void Refresh();
	afx_msg void SetAllProps(short x, short y, LPCSTR text);
	afx_msg short GetX();
	afx_msg void SetX(short nNewValue);
	afx_msg short GetY();
	afx_msg void SetY(short nNewValue);
	afx_msg void ShowWindow();
	afx_msg LPDISPATCH GetPosition();
	afx_msg void SetPosition(LPDISPATCH newValue);
	//}}AFX_DISPATCH
	DECLARE_DISPATCH_MAP()
};

/////////////////////////////////////////////////////////////////////////////
