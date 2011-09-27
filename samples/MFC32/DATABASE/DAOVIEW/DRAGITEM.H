// DragItem.h : header file
//

// This is a part of the Microsoft Foundation Classes C++ library.
// Copyright (C) 1992-1995 Microsoft Corporation
// All rights reserved.
//
// This source code is only intended as a supplement to the
// Microsoft Foundation Classes Reference and related
// electronic documentation provided with the library.
// See these sources for detailed information regarding the
// Microsoft Foundation Classes product.

/////////////////////////////////////////////////////////////////////////////
// CDragItem window

class CDragItem : public CObject
{
public:
// Flag values
	enum Mode { itemNull = 0,itemTable = 1,itemQuery = 2,itemRelation = 3 };
	 
// Construction
	CDragItem();
	CDragItem(WORD nItemType, LPCTSTR lpszDBName, LPCTSTR lpszDBConnect, 
		LPCTSTR lpszItemName );

// Attributes
public:
	WORD m_nItemType;
	CString m_strDBName;
	CString m_strDBConnect;
	CString m_strItemName;

	virtual void Serialize( CArchive& ar );

// Operations
public:
	BOOL Transfer(CDaoTreeView* pTreeView,CDaoDatabase* pDBTarget);
	BOOL CopyTable(CDaoDatabase* pDBSource,CDaoDatabase* pDBTarget);
	BOOL CopyQueryDef(CDaoDatabase* pDBSource,CDaoDatabase* pDBTarget);

// Overrides

// Implementation
public:
	virtual ~CDragItem();

	// Generated message map functions
protected:
};

/////////////////////////////////////////////////////////////////////////////
