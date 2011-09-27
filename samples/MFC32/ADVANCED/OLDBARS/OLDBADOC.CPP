// oldbadoc.cpp : implementation of the COldbarsDoc class
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

#include "stdafx.h"
#include "oldbars.h"

#include "oldbadoc.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// COldbarsDoc

IMPLEMENT_DYNCREATE(COldbarsDoc, CDocument)

BEGIN_MESSAGE_MAP(COldbarsDoc, CDocument)
	//{{AFX_MSG_MAP(COldbarsDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COldbarsDoc construction/destruction

COldbarsDoc::COldbarsDoc()
{
	// TODO: add one-time construction code here

}

COldbarsDoc::~COldbarsDoc()
{
}

BOOL COldbarsDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// COldbarsDoc serialization

void COldbarsDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}

/////////////////////////////////////////////////////////////////////////////
// COldbarsDoc diagnostics

#ifdef _DEBUG
void COldbarsDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void COldbarsDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// COldbarsDoc commands
