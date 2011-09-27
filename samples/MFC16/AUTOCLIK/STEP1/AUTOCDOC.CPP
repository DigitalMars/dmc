// autocdoc.cpp : implementation of the CClikDoc class
//

#include "stdafx.h"
#include "autoclik.h"

#include "autocdoc.h"
#include "dialogs.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CClikDoc

IMPLEMENT_DYNCREATE(CClikDoc, CDocument)

BEGIN_MESSAGE_MAP(CClikDoc, CDocument)
	//{{AFX_MSG_MAP(CClikDoc)
	ON_COMMAND(ID_EDIT_CHANGETEXT, OnEditChangeText)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BEGIN_DISPATCH_MAP(CClikDoc, CDocument)
	//{{AFX_DISPATCH_MAP(CClikDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//      DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_DISPATCH_MAP
END_DISPATCH_MAP()

/////////////////////////////////////////////////////////////////////////////
// CClikDoc construction/destruction

CClikDoc::CClikDoc()
{
	EnableAutomation();

	m_pt = CPoint(10,10);
	m_str = "Automation!";

	AfxOleLockApp();
}

CClikDoc::~CClikDoc()
{
	AfxOleUnlockApp();
}

BOOL CClikDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}

void CClikDoc::Refresh()
{
	UpdateAllViews(NULL);
	SetModifiedFlag();
}
/////////////////////////////////////////////////////////////////////////////
// CClikDoc serialization

void CClikDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		ar << m_pt << m_str;
	}
	else
	{
		ar >> m_pt >> m_str;
	}
}

/////////////////////////////////////////////////////////////////////////////
// CClikDoc diagnostics

#ifdef _DEBUG
void CClikDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CClikDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CClikDoc commands

void CClikDoc::OnEditChangeText()
{
	CChangeText dlg;
	dlg.m_str = m_str;
	if (dlg.DoModal())
	{
		m_str = dlg.m_str;
		Refresh();
	}
}
