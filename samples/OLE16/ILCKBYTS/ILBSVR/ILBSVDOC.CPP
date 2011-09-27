// ilbsvdoc.cpp : implementation of the CIlbsvrDoc class
//

#include "stdafx.h"
#include "ilbsvr.h"

#include "ilbsvdoc.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CIlbsvrDoc

IMPLEMENT_DYNCREATE(CIlbsvrDoc, CDocument)

BEGIN_MESSAGE_MAP(CIlbsvrDoc, CDocument)
	//{{AFX_MSG_MAP(CIlbsvrDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CIlbsvrDoc construction/destruction

CIlbsvrDoc::CIlbsvrDoc()
{
	// TODO: add one-time construction code here
}

CIlbsvrDoc::~CIlbsvrDoc()
{
}

BOOL CIlbsvrDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;
	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CIlbsvrDoc serialization

void CIlbsvrDoc::Serialize(CArchive& ar)
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
// CIlbsvrDoc diagnostics

#ifdef _DEBUG
void CIlbsvrDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CIlbsvrDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CIlbsvrDoc commands
