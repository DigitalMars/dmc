// tmlredoc.h : interface of the CTMLReadDoc class
//
// Copyright (c) Toxidyne Corporation, 1995. All Rights Reserved.
//
//

class CTMLReadDoc : public CDocument
{
protected: // create from serialization only
	CTMLReadDoc();
	DECLARE_DYNCREATE(CTMLReadDoc)

// Attributes
public:

// Operations
public:

// Implementation
public:
	virtual ~CTMLReadDoc();
	virtual void Serialize(CArchive& ar);	// overridden for document i/o
#ifdef _DEBUG
	virtual	void AssertValid() const;
	virtual	void Dump(CDumpContext& dc) const;
#endif
protected:
	virtual	BOOL	OnNewDocument();

// Generated message map functions
protected:
	//{{AFX_MSG(CTMLReadDoc)
		// NOTE - the ClassExpress will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
