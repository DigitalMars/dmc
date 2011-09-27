// rtformvi.cpp : implementation file
//

#include "stdafx.h"
#include "utils.h"
#include "realtdoc.h"
#include "realtime.h"
#include "rtformvi.h"
#include "rtitem.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRTFormView

IMPLEMENT_DYNCREATE(CRTFormView, CFormView)

CRTFormView::CRTFormView()
	: CFormView(CRTFormView::IDD)
{
	//{{AFX_DATA_INIT(CRTFormView)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CRTFormView::~CRTFormView()
{
}

void CRTFormView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CRTFormView)
	DDX_Control(pDX, IDC_LIST1, m_list);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CRTFormView, CFormView)
	//{{AFX_MSG_MAP(CRTFormView)
	ON_LBN_DBLCLK(IDC_LIST1, OnDblclkList1)
	ON_WM_LBUTTONDOWN()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRTFormView message handlers


void CRTFormView::OnDblclkList1()
{
	// my code
}

void CRTFormView::OnUpdate( CView * pSender, LPARAM lHint, CObject * pHint )
{
	POSITION pos;
	CString key;
	CString S;
	CRealtimeItem * pRealtimeItem;
	CRealtimeDoc * pDoc = (CRealtimeDoc *) GetDocument();
	int nSel = m_list.GetCurSel();
//  m_list.LockWindowUpdate();
	m_list.ResetContent();
	for( pos = pDoc->m_mapList.GetStartPosition(); pos != NULL; )
	{
		pDoc->m_mapList.GetNextAssoc( pos, key, (LPVOID&)pRealtimeItem );
		S = key+"\t"+ pRealtimeItem->Eighths();
		m_list.AddString(S);
	}
	m_list.SetCurSel(nSel);
//  m_list.LockWindowUpdate();
}

BOOL CRTFormView::UpdateData( BOOL bSaveAndValidate)
{
	return TRUE;
};
void CRTFormView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default

	CFormView::OnLButtonDown(nFlags, point);
	CRealtimeDoc * pDoc = (CRealtimeDoc *) GetDocument();
	pDoc->Iterate();

}
