// cataldoc.cpp : implementation of the CCatalogDoc class
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
#include "catalog.h"

#include "tableset.h"
#include "columnst.h"
#include "cataldoc.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCatalogDoc

IMPLEMENT_DYNCREATE(CCatalogDoc, CDocument)

BEGIN_MESSAGE_MAP(CCatalogDoc, CDocument)
	//{{AFX_MSG_MAP(CCatalogDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCatalogDoc construction/destruction

CCatalogDoc::CCatalogDoc()
{
	// TODO: add one-time construction code here
}

CCatalogDoc::~CCatalogDoc()
{
}

BOOL CCatalogDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CCatalogDoc diagnostics

#ifdef _DEBUG
void CCatalogDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CCatalogDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CCatalogDoc commands
