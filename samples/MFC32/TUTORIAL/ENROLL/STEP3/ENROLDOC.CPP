// EnrolDoc.cpp : implementation of the CEnrollDoc class
//

#include "Stdafx.h"
#include "Enroll.h"
#include "SectSet.h"
#include "CoursSet.h"
#include "EnrolDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CEnrollDoc

IMPLEMENT_DYNCREATE(CEnrollDoc, CDocument)

BEGIN_MESSAGE_MAP(CEnrollDoc, CDocument)
	//{{AFX_MSG_MAP(CEnrollDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEnrollDoc construction/destruction

CEnrollDoc::CEnrollDoc()
{
	// TODO: add one-time construction code here

}

CEnrollDoc::~CEnrollDoc()
{
}

BOOL CEnrollDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CEnrollDoc diagnostics

#ifdef _DEBUG
void CEnrollDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CEnrollDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CEnrollDoc commands
