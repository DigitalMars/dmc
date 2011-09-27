// MDIDoc.cpp : implementation of the CMDIBindDoc class
//

// This is a part of the Microsoft Foundation Classes C++ library.
// Copyright (C) 1992-1996 Microsoft Corporation
// All rights reserved.
//
// This source code is only intended as a supplement to the
// Microsoft Foundation Classes Reference and related
// electronic documentation provided with the library.
// See these sources for detailed information regarding the
// Microsoft Foundation Classes product.

#include "stdafx.h"
#include "MDIBind.h"

#include "MDIDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMDIBindDoc

IMPLEMENT_DYNCREATE(CMDIBindDoc, CDocument)

BEGIN_MESSAGE_MAP(CMDIBindDoc, CDocument)
	//{{AFX_MSG_MAP(CMDIBindDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMDIBindDoc construction/destruction

CMDIBindDoc::CMDIBindDoc()
{
	// TODO: add one-time construction code here
	m_pRDC=NULL;
}

CMDIBindDoc::~CMDIBindDoc()
{
}

BOOL CMDIBindDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CMDIBindDoc serialization

void CMDIBindDoc::Serialize(CArchive& ar)
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
// CMDIBindDoc diagnostics

#ifdef _DEBUG
void CMDIBindDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CMDIBindDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMDIBindDoc commands
