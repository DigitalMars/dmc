// TreeView.h : interface of the CDaoTreeView class
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

class CDaoTreeView : public CTreeView
{
protected: // create from serialization only
	CDaoTreeView();
	DECLARE_DYNCREATE(CDaoTreeView)

// Attributes
public:
	CDaoViewDoc* GetDocument();
	CDaoDatabase* m_pDB;
	UINT m_nIDClipFormat;
	CTreeCursor m_ItemSel;
	BOOL m_bNoNotifications;

// Operations
public:
	enum ImageIDS{
		IID_DATABASE = 0,IID_DATABASES,
		IID_TABLE,IID_TABLES,
		IID_FIELD,IID_FIELDS,
		IID_INDEX,IID_INDEXES,
		IID_RELATION,IID_RELATIONS,
		IID_QUERYDEF,IID_QUERYDEFS
	};

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDaoTreeView)
	public:
	virtual DROPEFFECT OnDragEnter(COleDataObject* pDataObject, DWORD dwKeyState, CPoint point);
	virtual BOOL OnDrop(COleDataObject* pDataObject, DROPEFFECT dropEffect, CPoint point);
	virtual DROPEFFECT OnDragOver(COleDataObject* pDataObject, DWORD dwKeyState, CPoint point);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CDaoTreeView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	void PopulateTree();
	void AddItem(WORD nItemType, LPCTSTR lpszName);
	void DeleteItem(CTreeCursor& itemDelete);

protected:

	CImageList	m_ctlImage;

	// Some bookmarks to places in the tree
	CTreeCursor tDatabases, tDatabase;
	CTreeCursor tTables, tTable;
	CTreeCursor tIndexes, tFields;
	CTreeCursor tRelations, tQueryDefs;

	COleDropTarget m_dropTarget;

// Generated message map functions
protected:
	//{{AFX_MSG(CDaoTreeView)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSysColorChange();
	afx_msg void OnNewQuery();
	afx_msg void OnQueryEdit();
	//}}AFX_MSG
	afx_msg void OnPopupCommand(UINT nMenuID);
	void OnNodeSelect(NMHDR *pNotifyStruct,LRESULT *result);
	void OnBeginDrag(NMHDR *pNotifyStruct,LRESULT *result);
	void OnKeyDown(NMHDR *pNotifyStruct,LRESULT *result);
	void OnRightClick(NMHDR *pNotifyStruct,LRESULT *result);
	void DoPopupMenu(UINT nMenuID);

	DECLARE_MESSAGE_MAP()
};
/*
#ifndef _DEBUG  // debug version in TreeView.cpp
inline CDaoViewDoc* CDaoTreeView::GetDocument()
   { return (CDaoViewDoc*)m_pDocument; }
#endif
*/
/////////////////////////////////////////////////////////////////////////////
