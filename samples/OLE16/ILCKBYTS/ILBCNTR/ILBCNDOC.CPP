// ilbcndoc.cpp : implementation of the CIlbcntrDoc class
//

#include "stdafx.h"
#include "ilbcntr.h"

#include "ilbcndoc.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CIlbcntrDoc

IMPLEMENT_DYNCREATE(CIlbcntrDoc, CDocument)

BEGIN_MESSAGE_MAP(CIlbcntrDoc, CDocument)
	//{{AFX_MSG_MAP(CIlbcntrDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CIlbcntrDoc construction/destruction

CIlbcntrDoc::CIlbcntrDoc()
{
	// TODO: add one-time construction code here
}

CIlbcntrDoc::~CIlbcntrDoc()
{
}

BOOL CIlbcntrDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;
	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CIlbcntrDoc serialization

void CIlbcntrDoc::Serialize(CArchive& ar)
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
// CIlbcntrDoc diagnostics

#ifdef _DEBUG
void CIlbcntrDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CIlbcntrDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CIlbcntrDoc commands
