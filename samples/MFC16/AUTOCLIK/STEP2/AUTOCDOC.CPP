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
	DISP_PROPERTY(CClikDoc, "text", m_str, VT_BSTR)
	DISP_FUNCTION(CClikDoc, "RefreshWindow", Refresh, VT_EMPTY, VTS_NONE)
	DISP_FUNCTION(CClikDoc, "SetAllProps", SetAllProps, VT_EMPTY, VTS_I2 VTS_I2 VTS_BSTR)
	DISP_PROPERTY_EX(CClikDoc, "x", GetX, SetX, VT_I2)
	DISP_PROPERTY_EX(CClikDoc, "y", GetY, SetY, VT_I2)
	DISP_FUNCTION(CClikDoc, "ShowWindow", ShowWindow, VT_EMPTY, VTS_NONE)
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

short CClikDoc::GetX()
{
	return m_pt.x;
}

void CClikDoc::SetX(short nNewValue)
{
	m_pt.x = nNewValue;
	Refresh();
}

short CClikDoc::GetY()
{
	return m_pt.y;
}

void CClikDoc::SetY(short nNewValue)
{
	m_pt.y = nNewValue;
	Refresh();
}


void CClikDoc::SetAllProps(short x, short y, LPCSTR text)
{
	m_pt.x = x;
	m_pt.y = y;
	m_str = text;
	Refresh();
}

void CClikDoc::ShowWindow()
{
	POSITION pos = GetFirstViewPosition();
	CView* pView = GetNextView(pos);
	if (pView != NULL)
	{
		CFrameWnd* pFrameWnd = pView->GetParentFrame();
		pFrameWnd->ActivateFrame(SW_SHOW);
		pFrameWnd = pFrameWnd->GetParentFrame();
		if (pFrameWnd != NULL)
			pFrameWnd->ActivateFrame(SW_SHOW);
	}
}
