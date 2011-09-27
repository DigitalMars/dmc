// tmlrddoc.cpp : implementation of the CTMLReadDoc class
//
// Copyright (c) XYZ Corp., 1994. All Rights Reserved.
//
//

#include "stdafx.h"
#include "tmlread.h"

#include "tmlrddoc.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTMLReadDoc

IMPLEMENT_DYNCREATE(CTMLReadDoc, CDocument)

BEGIN_MESSAGE_MAP(CTMLReadDoc, CDocument)
	//{{AFX_MSG_MAP(CTMLReadDoc)
		// NOTE - the ClassExpress will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTMLReadDoc construction/destruction

CTMLReadDoc::CTMLReadDoc()
{
  TRACE("CTMLReadDoc::CTMLReadDoc()\n");
	// TODO: add one-time construction code here
}

CTMLReadDoc::~CTMLReadDoc()
{
  TRACE("CTMLReadDoc::~CTMLReadDoc()\n");
}

BOOL CTMLReadDoc::OnNewDocument()
{
  TRACE("CTMLReadDoc::OnNewDocument()\n");
	if (!CDocument::OnNewDocument())
		return FALSE;
	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CTMLReadDoc serialization

void CTMLReadDoc::Serialize(CArchive& ar)
{
  TRACE("CTMLReadDoc::Serialize()\n");
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
// CTMLReadDoc diagnostics

#ifdef _DEBUG
void CTMLReadDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CTMLReadDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CTMLReadDoc commands
