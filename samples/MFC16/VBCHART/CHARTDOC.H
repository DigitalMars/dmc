// chartdoc.h : interface of the CChartDoc class
//
// This is a part of the Microsoft Foundation Classes C++ library.
// Copyright (C) 1992 Microsoft Corporation
// All rights reserved.
//
// This source code is only intended as a supplement to the
// Microsoft Foundation Classes Reference and Microsoft
// QuickHelp and/or WinHelp documentation provided with the library.
// See these sources for detailed information regarding the
// Microsoft Foundation Classes product.



class CGridEntry;   // forward reference

class CChartDoc : public CDocument
{
protected: // create from serialization only
	CChartDoc();
	DECLARE_DYNCREATE(CChartDoc)

// Attributes
public:
	CGridEntry* m_pDocGrid;
	BOOL        m_bDirtyGraph;

// Operations
public:

// Implementation
public:
	virtual ~CChartDoc();
	virtual void Serialize(CArchive& ar);   // overridden for document i/o

// Generated message map functions
protected:
	//{{AFX_MSG(CChartDoc)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
