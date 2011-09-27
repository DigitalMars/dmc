// realtdoc.cpp : implementation of the CRealtimeDoc class
//

#include "stdafx.h"
#include "utils.h"

#include "realtdoc.h"
#include "realtime.h"
#include <stdlib.h>

#include "rtitem.h"
#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRealtimeDoc

IMPLEMENT_DYNCREATE(CRealtimeDoc, CDocument)

BEGIN_MESSAGE_MAP(CRealtimeDoc, CDocument)
	//{{AFX_MSG_MAP(CRealtimeDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRealtimeDoc construction/destruction

CRealtimeDoc FAR * pRealtimeDoc;

CRealtimeDoc::CRealtimeDoc()
{
	theApp.m_pDoc = this;
	m_wndTimer.Create(NULL, "Timer", 0, CRect(0, 0, 1, 1), CWnd::GetDesktopWindow(), NULL, NULL );
	pRealtimeDoc = this;
	m_uTimer = m_wndTimer.SetTimer((UINT)(CRealtimeDoc FAR * __near *)&pRealtimeDoc, 1000, CRealtimeDoc::TimerProc);
}

CRealtimeDoc::~CRealtimeDoc()
{
	if (m_uTimer) m_wndTimer.KillTimer(m_uTimer);
	// delete time
}

void CRealtimeDoc::TimerProc( HWND hwnd, UINT nMsg, UINT nID, DWORD dwSysTime )
{
	pRealtimeDoc->Iterate();
}


BOOL CRealtimeDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;
	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)
	return TRUE;
}


void CRealtimeDoc::Iterate(void)
{
	POSITION pos;
	CString key;
	CRealtimeItem * pRealtimeItem;
	BOOL fUpdate = FALSE;

	for( pos = m_mapList.GetStartPosition(); pos != NULL; )
	{
		m_mapList.GetNextAssoc( pos, key, (LPVOID&)pRealtimeItem );
		fUpdate |= pRealtimeItem->Iterate();
	}
	if (fUpdate) UpdateAllViews( NULL );
}

/////////////////////////////////////////////////////////////////////////////
// CRealtimeDoc serialization

void CRealtimeDoc::Serialize(CArchive& ar)
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
// CRealtimeDoc diagnostics

#ifdef _DEBUG
void CRealtimeDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CRealtimeDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
	dc << "CRealtimeDoc";
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CRealtimeDoc commands
