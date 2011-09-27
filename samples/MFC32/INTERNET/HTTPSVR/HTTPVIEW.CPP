// HttpSvrView.cpp : implementation of the CHttpSvrView class
//
// This is a part of the Microsoft Foundation Classes C++ library.
// Copyright (C) 1996 Microsoft Corporation
// All rights reserved.
//
// This source code is only intended as a supplement to the
// Microsoft Foundation Classes Reference and related
// electronic documentation provided with the library.
// See these sources for detailed information regarding the
// Microsoft Foundation Classes product.
//
// Note that your filters probably won't have an #ifdef like this one.
// The project file for this sample allows you to build the sample as
// a statically linked regular MFC DLL (that is, with _AFXDLL defined)
// or as a DLL that doesn't use MFC classes aside from the ISAPI 
// support classes (that is, without _AFXDLL defined).

#include "stdafx.h"

#include "HttpSvr.h"
#include "Http.h"
#include "HttpDoc.h"
#include "HttpView.h"
#include "RootDlg.h"
#include "NoRoot.h"
#include "Request.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CHttpSvrView

IMPLEMENT_DYNCREATE(CHttpSvrView, CListView)

BEGIN_MESSAGE_MAP(CHttpSvrView, CListView)
	//{{AFX_MSG_MAP(CHttpSvrView)
	ON_COMMAND(IDM_VIEW_LARGE, OnViewLarge)
	ON_UPDATE_COMMAND_UI(IDM_VIEW_LARGE, OnUpdateViewLarge)
	ON_COMMAND(IDM_VIEW_LIST, OnViewList)
	ON_UPDATE_COMMAND_UI(IDM_VIEW_LIST, OnUpdateViewList)
	ON_COMMAND(IDM_VIEW_REPORT, OnViewReport)
	ON_UPDATE_COMMAND_UI(IDM_VIEW_REPORT, OnUpdateViewReport)
	ON_COMMAND(IDM_VIEW_SMALL, OnViewSmall)
	ON_UPDATE_COMMAND_UI(IDM_VIEW_SMALL, OnUpdateViewSmall)
	ON_NOTIFY_REFLECT(LVN_COLUMNCLICK, OnColumnclick)
	ON_NOTIFY_REFLECT(LVN_GETDISPINFO, OnGetDispInfo)
	ON_NOTIFY_REFLECT(LVN_DELETEITEM, OnDeleteItem)
	ON_COMMAND(IDM_VIEW_CLEAR, OnViewClear)
	//}}AFX_MSG_MAP
#ifndef _MAC
	ON_NOTIFY_REFLECT(NM_DBLCLK, OnDblclk)
	ON_NOTIFY_REFLECT(NM_RCLICK, OnRclick)
	ON_COMMAND(IDM_POPUP_CLEAR, OnPopupClear)
	ON_COMMAND(IDM_POPUP_EDIT, OnPopupEdit)
	ON_COMMAND(IDM_POPUP_OPEN, OnPopupOpen)
#endif // !_MAC
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CHttpSvrView construction/destruction

CHttpSvrView::CHttpSvrView()
{
	m_phdPopup = NULL;
}

CHttpSvrView::~CHttpSvrView()
{
}

BOOL CHttpSvrView::PreCreateWindow(CREATESTRUCT& cs)
{
	cs.style = (cs.style & ~LVS_TYPEMASK) | LVS_REPORT;
	cs.style |= LVS_AUTOARRANGE;
	return CListView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CHttpSvrView drawing

void CHttpSvrView::OnDraw(CDC* pDC)
{
	CHttpSvrDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
}

void CHttpSvrView::OnInitialUpdate()
{
	CHttpSvrDoc* pDoc = GetDocument();
	CListView::OnInitialUpdate();
	// get the root directory....
	CString strRoot = pDoc->m_strRoot;
	// make sure it exists and it's a folder....
	BOOL bLoop = TRUE;
	while ( bLoop )
	{
		DWORD dwAttr = GetFileAttributes( strRoot );
#ifndef _MAC
		if ( dwAttr == -1 )
		{
			CNoRootDlg dlg;
			dlg.m_strRoot = strRoot;
			UINT uButton = dlg.DoModal();
			if ( uButton == IDOK )
			{
				strRoot = dlg.m_strRoot;
				dwAttr = GetFileAttributes( strRoot );
				if ( dwAttr == -1 )
				{
					if ( !CreateDirectory( strRoot, NULL ) )
					{
						MessageBox( "Create Directory failed",
							strRoot, MB_ICONEXCLAMATION|MB_OK );
					}
				}
			}
			else // no....
				bLoop = FALSE; // use it anyway
		}
		else if ( (dwAttr & FILE_ATTRIBUTE_DIRECTORY) == 0 )
#else  // !_MAC
		if ( dwAttr == -1 || (dwAttr & FILE_ATTRIBUTE_DIRECTORY) == 0 )
#endif // !_MAC
		{
			CBadRootDlg dlg;
			dlg.m_strRoot = strRoot;
			if ( dlg.DoModal() == IDCANCEL )
				bLoop = FALSE; // use it anyway
		}
		else
		{
			// root is okay, save it....
			if ( strRoot.CompareNoCase(pDoc->m_strRoot) != 0 )
			{
				pDoc->m_strRoot = strRoot;
				pDoc->SetModifiedFlag( TRUE );
			}
			bLoop = FALSE;  // okay!
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
// CHttpSvrView diagnostics

#ifdef _DEBUG
void CHttpSvrView::AssertValid() const
{
	CListView::AssertValid();
}

void CHttpSvrView::Dump(CDumpContext& dc) const
{
	CListView::Dump(dc);
}

CHttpSvrDoc* CHttpSvrView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CHttpSvrDoc)));
	return (CHttpSvrDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CHttpSvrView message handlers

BOOL CHttpSvrView::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext) 
{
	BOOL bCreated = CWnd::Create(lpszClassName, lpszWindowName, dwStyle, rect, pParentWnd, nID, pContext);
	if ( bCreated )
	{
		CListCtrl& listView = GetListCtrl();
		int aWidths[] = { 150, 200, 50, 140, 225, 200 };
		CString strHeading;

		for ( int iCol = 0; iCol < C_COLUMNS; iCol++) {
			strHeading.LoadString( IDS_COLUMN1 + iCol );
			listView.InsertColumn( iCol, strHeading, LVCFMT_LEFT,
				aWidths[iCol], iCol );
		}

		// Create the full-sized and small icon image lists.
		if ( m_ilLarge.Create( IDB_IMAGES, 32, 1, RGB(0,255,0) ) )
		{
			listView.SetImageList( &m_ilLarge, LVSIL_NORMAL );
		}
		if ( m_ilSmall.Create( IDB_SMALLIMAGES, 16, 1, RGB(0,255,0) ) )
		{
			listView.SetImageList( &m_ilSmall, LVSIL_SMALL);
		}
	}
	return bCreated;
}

void CHttpSvrView::SetListView( DWORD dwView ) 
{
	CListCtrl& list = GetListCtrl();
	DWORD dwStyle = GetWindowLong( list.m_hWnd, GWL_STYLE );
	if ( (dwStyle & LVS_TYPEMASK) != dwView )
		SetWindowLong( list.m_hWnd, GWL_STYLE,
			(dwStyle & ~LVS_TYPEMASK) | dwView );
}

void CHttpSvrView::OnViewLarge() 
{
	SetListView( LVS_ICON );
}

void CHttpSvrView::OnUpdateViewLarge(CCmdUI* pCmdUI) 
{
	CListCtrl& list = GetListCtrl();
	DWORD dwStyle = GetWindowLong( list.m_hWnd, GWL_STYLE ) & LVS_TYPEMASK;
	pCmdUI->SetCheck( dwStyle == LVS_ICON );
}

void CHttpSvrView::OnViewList() 
{
	SetListView( LVS_LIST );
}

void CHttpSvrView::OnUpdateViewList(CCmdUI* pCmdUI) 
{
	CListCtrl& list = GetListCtrl();
	DWORD dwStyle = GetWindowLong( list.m_hWnd, GWL_STYLE ) & LVS_TYPEMASK;
	pCmdUI->SetCheck( dwStyle == LVS_LIST );
}

void CHttpSvrView::OnViewReport() 
{
	SetListView( LVS_REPORT );
}

void CHttpSvrView::OnUpdateViewReport(CCmdUI* pCmdUI) 
{
	CListCtrl& list = GetListCtrl();
	DWORD dwStyle = GetWindowLong( list.m_hWnd, GWL_STYLE ) & LVS_TYPEMASK;
	pCmdUI->SetCheck( dwStyle == LVS_REPORT );
}

void CHttpSvrView::OnViewSmall() 
{
	SetListView( LVS_SMALLICON );
}

void CHttpSvrView::OnUpdateViewSmall(CCmdUI* pCmdUI) 
{
	CListCtrl& list = GetListCtrl();
	DWORD dwStyle = GetWindowLong( list.m_hWnd, GWL_STYLE ) & LVS_TYPEMASK;
	pCmdUI->SetCheck( dwStyle == LVS_SMALLICON );
}

int CALLBACK
CompareHitDocs( CHitDoc* doc1, CHitDoc* doc2, LPARAM lCol )
{
	int nCmp = 0;
	switch( lCol )
	{
	case COLUMN_PATH:
		nCmp = doc1->m_strFolder.CompareNoCase( doc2->m_strFolder );
		break;
	case COLUMN_HITS:
		if ( doc1->m_nHits > doc2->m_nHits )
			nCmp = -1;
		else if ( doc1->m_nHits < doc2->m_nHits )
			nCmp = 1;
		break;
	case COLUMN_LAST:
		if ( doc1->m_timeLastHit > doc2->m_timeLastHit )
			nCmp = -1;
		else if ( doc1->m_timeLastHit < doc2->m_timeLastHit )
			nCmp = 1;
		break;
	case COLUMN_CMD:
		nCmp = doc1->m_strCommand.CompareNoCase( doc2->m_strCommand );
		break;
	case COLUMN_URL:
		nCmp = doc1->m_strURL.CompareNoCase( doc2->m_strURL );
		break;
	default:
		// put folders ahead of everything....
		if ( doc1->m_bFolder && !doc2->m_bFolder )
			nCmp = -1;
		else if ( !doc1->m_bFolder && doc2->m_bFolder )
			nCmp = 1;
		// sort successfully hit docs....
		else if ( doc1->m_nStatus == 200 && doc2->m_nStatus == 200 )
		{
			nCmp = doc1->m_strFile.CompareNoCase( doc2->m_strFile );
			if ( nCmp == 0 )
				nCmp = doc1->m_strFolder.CompareNoCase( doc2->m_strFolder );
		}
		// put hit docs ahead of status items....
		else if ( doc1->m_nStatus == 200 )
			nCmp = -1;
		else if ( doc2->m_nStatus == 200 )
			nCmp = 1;
		// sort status items by status value....
		else if ( doc1->m_nStatus < doc2->m_nStatus )
			nCmp = -1;
		else if ( doc1->m_nStatus > doc2->m_nStatus )
			nCmp = 1;
		break;
	}
	return nCmp;
}

void CHttpSvrView::OnColumnclick(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	CListCtrl& list = GetListCtrl();
	list.SortItems( (PFNLVCOMPARE)CompareHitDocs, pNMListView->iSubItem );
	*pResult = 0;
}

void CHttpSvrView::OnGetDispInfo(NMHDR* pNMHDR, LRESULT* pResult) 
{
	LV_DISPINFO* pDispInfo = (LV_DISPINFO*)pNMHDR;
	if ( pDispInfo->item.mask & LVIF_TEXT )
	{
		CHitDoc* pHitDoc = (CHitDoc*)(pDispInfo->item.lParam);
		switch( pDispInfo->item.iSubItem )
		{
		case COLUMN_FILE:
			lstrcpy( pDispInfo->item.pszText, pHitDoc->m_strFile );
			break;
		case COLUMN_PATH:
			lstrcpy( pDispInfo->item.pszText, pHitDoc->m_strFolder );
			break;
		case COLUMN_HITS:
			wsprintf( pDispInfo->item.pszText, "%d", pHitDoc->m_nHits ); 
			break;
		case COLUMN_LAST:
			if ( pHitDoc->m_nHits > 0 )
				lstrcpy( pDispInfo->item.pszText, (pHitDoc->m_nHits) 
					? pHitDoc->m_timeLastHit.Format( IDS_TIMEFORMAT )
					: "" );
			break;
		case COLUMN_URL:
			lstrcpy( pDispInfo->item.pszText, pHitDoc->m_strURL );
			break;
		case COLUMN_CMD:
			lstrcpy( pDispInfo->item.pszText, pHitDoc->m_strCommand );
			break;
		}
	}
	*pResult = 0;
}

void CHttpSvrView::OnDeleteItem(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	CListCtrl& list = GetListCtrl();
	LV_ITEM lvi;
	lvi.mask = LVIF_PARAM;
	lvi.iItem = pNMListView->iItem;
	lvi.iSubItem = 0;
	if ( list.GetItem( &lvi ) == TRUE )
	{
		CHitDoc* pHit = (CHitDoc*)(lvi.lParam);
		delete pHit;
	}
	*pResult = 0;
}

int
CHttpSvrView::GetImage( CHitDoc* pHitDoc )
{
	// indexes into image maps for (status/100)....
	int aStatImg[] = { 2, 5, 5, 4, 3, 3 };
	int iImage;
	if ( pHitDoc->m_nStatus != 0 && pHitDoc->m_nStatus != 200 )
		iImage = aStatImg[ pHitDoc->m_nStatus/100]; // status image
	else if ( pHitDoc->m_dwExecute )
		iImage = 2; // CGI executables
	else if ( pHitDoc->m_nHits == 0 )
		iImage = 0; // no hits yet
	else if ( pHitDoc->m_bFolder )
		iImage = 1; // folder image
	else
		iImage = 0; // document

	return iImage;
}

void CHttpSvrView::RegisterHit( CListCtrl& list, CRequest* pRequest )
{
	CHitDoc* pHitDoc = new CHitDoc( pRequest );
	if ( pHitDoc )
		InsertHitDoc( pHitDoc );
}

void CHttpSvrView::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint) 
{
	if ( lHint == HINT_DOCHIT )
	{
		CRequest* pRequest = (CRequest*)pHint;
		CListCtrl& list = GetListCtrl();
		RegisterHit( list, pRequest );
	}
}

#ifndef _MAC
void CHttpSvrView::OnDblclk(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// Get the control....
	CListCtrl& list = GetListCtrl();
	int nSelected = list.GetSelectedCount();
	// only proceed if one item selected....
	if ( nSelected == 1 )
	{
		// find the selected item....
		int ndx = 0;
		int nItems = list.GetItemCount();
		while ( ndx < nItems )
		{
			if ( list.GetItemState( ndx, LVIS_SELECTED ) == LVIS_SELECTED )
			{
				LV_ITEM lvi;
				lvi.mask = LVIF_PARAM;
				lvi.iItem = ndx;
				lvi.iSubItem = 0;
				if ( list.GetItem( &lvi ) )
				{
					// only do something for OK and non-executable hits...
					CHitDoc* pHitDoc = (CHitDoc*)(lvi.lParam);
					if ( pHitDoc->m_nStatus == IDS_STATUS_OK &&
						pHitDoc->m_dwExecute == 0 )
					{
						ShellExecute( GetSafeHwnd(), "open",
							pHitDoc->m_strFile, NULL,
							pHitDoc->m_strFolder, SW_SHOW );
					}
				}
				break;
			}
			++ndx;
		}
	}
	*pResult = 0;
}

void CHttpSvrView::OnRclick(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// Get the control....
	CListCtrl& list = GetListCtrl();
	if ( list.GetSelectedCount() == 1 )
	{
		CPoint point;
		if ( GetCursorPos( &point ) )
			DoContextMenu( point );
	}

	*pResult = 0;
}

void CHttpSvrView::DoContextMenu( const CPoint& point )
{
	m_phdPopup = NULL;
	// Get the control....
	CListCtrl& list = GetListCtrl();
	CPoint ptList = point;
	list.ScreenToClient( &ptList );
	int ndx = list.HitTest( ptList );
	if ( ndx != -1 )
	{
		LV_ITEM lvi;
		lvi.mask = LVIF_PARAM;
		lvi.iItem = ndx;
		lvi.iSubItem = 0;
		if ( list.GetItem( &lvi ) )
		{
			CHitDoc* pHitDoc = (CHitDoc*)(lvi.lParam);
			// get the popup menu type index....
			int nType = 0;
			if ( pHitDoc->m_dwExecute )
				nType = 1;
			else if ( pHitDoc->m_nStatus != 200 )
				nType = 2;

			// load the menus....
			CMenu menuPopups;
			if ( menuPopups.LoadMenu( IDM_POPUPS ) )
			{
				// get the popup....
				CMenu* pMenu = menuPopups.GetSubMenu( nType );
				if ( pMenu != NULL )
				{
					m_phdPopup = pHitDoc;
					pMenu->TrackPopupMenu( TPM_LEFTALIGN|TPM_RIGHTBUTTON,
						point.x, point.y, this );
				}
			}
		}
	}
}

void CHttpSvrView::OnPopupClear() 
{
	if ( m_phdPopup != NULL )
	{
		// Get the control....
		CListCtrl& list = GetListCtrl();
		LV_FINDINFO lvfi;
		lvfi.flags = LVFI_PARAM;
		lvfi.lParam = (LPARAM)m_phdPopup;
		int ndx = list.FindItem( &lvfi );
		if ( ndx != -1 )
			list.DeleteItem( ndx );
	}
}

void CHttpSvrView::OnPopupEdit() 
{
	if ( m_phdPopup != NULL )
	{
		ShellExecute( GetSafeHwnd(), "edit",
			m_phdPopup->m_strFile, NULL,
			m_phdPopup->m_strFolder, SW_SHOW );
	}
}

void CHttpSvrView::OnPopupOpen() 
{
	if ( m_phdPopup != NULL )
	{
		ShellExecute( GetSafeHwnd(), "open",
			m_phdPopup->m_strFile, NULL,
			m_phdPopup->m_strFolder, SW_SHOW );
	}
}
#endif // !_MAC

BOOL CHttpSvrView::InsertHitDoc( CHitDoc* pHitDoc )
{
	CListCtrl& list = GetListCtrl();
	// look for a match on the file name....
	LV_FINDINFO lvfi;
	lvfi.flags = LVFI_STRING;
	lvfi.psz = pHitDoc->m_strFile;
	int ndx = list.FindItem( &lvfi );
	while ( ndx != -1 )
	{
		// match found; see if folder matches....
		CString strFolder = list.GetItemText( ndx, COLUMN_PATH );
		if ( strFolder.CompareNoCase( pHitDoc->m_strFolder ) == 0 )
		{
			// matched; get the old HitDoc....
			LV_ITEM lvi;
			lvi.mask = LVIF_PARAM;
			lvi.iItem = ndx;
			lvi.iSubItem = 0;
			if ( list.GetItem( &lvi ) )
			{
				// get the old count
				CHitDoc* pOldHit = (CHitDoc*)(lvi.lParam);
				// add in the previous hits....
				pHitDoc->m_nHits += pOldHit->m_nHits;
				// delete the old hit....
				delete pOldHit;
				// update with new HitDoc....
				lvi.mask = LVIF_IMAGE|LVIF_PARAM;
				lvi.iImage = GetImage( pHitDoc );
				lvi.lParam = (LPARAM)pHitDoc;
				list.SetItem( &lvi );
				// indicate we've made changes....
				list.Update( lvi.iItem );
				break;
			}
		}
		// get next item....
		ndx = list.FindItem( &lvfi, ndx );
	}
	// if we didn't find anything....
	if ( ndx == -1 )
	{
		// didn't find a match; add it to the list....
		LV_ITEM lvi;
		lvi.mask = LVIF_TEXT|LVIF_PARAM|LVIF_IMAGE;
		lvi.iItem = 0;
		lvi.iSubItem = 0;
		lvi.pszText = LPSTR_TEXTCALLBACK;
		lvi.lParam = (LPARAM)pHitDoc;
		lvi.iImage = GetImage( pHitDoc );
		if ( (ndx=list.InsertItem( &lvi )) != -1 )
		{
			// add all the callback sub items....
			lvi.mask = LVIF_TEXT;
			lvi.pszText = LPSTR_TEXTCALLBACK;
			lvi.iItem = ndx;
			for( int subNdx=1; subNdx < C_COLUMNS; ++subNdx )
			{
				lvi.iSubItem = subNdx;
				list.SetItem( &lvi );
			}
		}
		else 
		{
			// insert failed; kill it....
			delete pHitDoc;
			pHitDoc = NULL;
		}
	}
	return (pHitDoc != NULL);
}

void CHttpSvrView::OnViewClear() 
{
	CListCtrl& list = GetListCtrl();
	list.DeleteAllItems();	
}
