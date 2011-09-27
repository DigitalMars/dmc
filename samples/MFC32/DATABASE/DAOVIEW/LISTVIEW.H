// samplvw.h : interface of the CDaoListView class
//
/////////////////////////////////////////////////////////////////////////////

// This is a part of the Microsoft Foundation Classes C++ library.
// Copyright (C) 1992-1995 Microsoft Corporation
// All rights reserved.
//
// This source code is only intended as a supplement to the
// Microsoft Foundation Classes Reference and related
// electronic documentation provided with the library.
// See these sources for detailed information regarding the
// Microsoft Foundation Classes product.

class CDaoListView : public CListView
{
protected: // create from serialization only
	CDaoListView();
	DECLARE_DYNCREATE(CDaoListView)

// Attributes
public:
	CDaoViewDoc* GetDocument();
	CDaoDatabase* m_pDB;

// Operations
public:
	void EraseList();
	void ShowDatabase();
	void ShowIndexes(LPCTSTR strTableName,LPCTSTR strIndexName = NULL);
	void ShowRelations(LPCTSTR strRelationName = NULL);
	void ShowTableSchema(LPCTSTR strTableDefName = NULL);
	void ShowTableData(LPCTSTR strTableName);
	void ShowQuerySchema(LPCTSTR strQueryDefName = NULL);
	void RunQueryDef(LPCTSTR strQueryDefName);
	void ShowFields(LPCTSTR strTableName,LPCTSTR strFieldName = NULL);

protected:
	void ShowDatabaseInfo(int nItem,CDaoDatabaseInfo& Info);
	void ShowIndexInfo(int nItem,CDaoIndexInfo& Info);
	void ShowRelationInfo(int nItem,CDaoRelationInfo& Info);
	void ShowTableDefInfo(int nItem,CDaoTableDefInfo& tInfo);
	void ShowQueryDefInfo(int nItem,CDaoQueryDefInfo& qdInfo);
	void ShowFieldInfo(int nItem,CDaoFieldInfo& fInfo);
	void DisplayColumnHeadings(UINT nStringID);
	void AdjustColumnWidths();
	void AddItem(int nItem,int nSubItem,LPCTSTR strItem);

	LPCTSTR CrackBOOL(BOOL bFlag);
	LPCTSTR CrackFieldType(short sType);
	LPCTSTR CrackQueryDefType(short sType);
	CString CrackVARIANT(COleVariant& var);

	int	m_nColumns;
	BOOL m_bVertical;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDaoListView)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	protected:
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CDaoListView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CDaoListView)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/*
#ifndef _DEBUG  // debug version in samplvw.cpp
inline CSampleDoc* CDaoListView::GetDocument()
   { return (CSampleDoc*)m_pDocument; }
#endif
*/