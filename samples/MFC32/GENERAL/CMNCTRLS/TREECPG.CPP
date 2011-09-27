// TreeCtrlPage.cpp : implementation file
//

// This is a part of the Microsoft Foundation Classes C++ library.
// Copyright (C) 1992-1995 Microsoft Corporation
// All rights reserved.
//
// This source code is only intended as a supplement to the
// Microsoft Foundation Classes Reference and related
// electronic documentation provided with the library.
// See these sources for detailed information regarding the
// Microsoft Foundation Classes product.

#include "stdafx.h"
#include "ctrldemo.h"
#include "treecpg.h"
#include "mtreectl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTreeCtrlPage property page

IMPLEMENT_DYNCREATE(CTreeCtrlPage, CPropertyPage)

CTreeCtrlPage::CTreeCtrlPage() : CPropertyPage(CTreeCtrlPage::IDD)
{
	//{{AFX_DATA_INIT(CTreeCtrlPage)
	m_bDisableDragDrop = FALSE;
	m_bHasButtons = FALSE;
	m_bHasLines = FALSE;
	m_bLinesAtRoot = FALSE;
	m_bEditLabels = FALSE;
	m_bShowSelAlways = FALSE;
	m_cstrStatus = _T("");
	//}}AFX_DATA_INIT
}

CTreeCtrlPage::~CTreeCtrlPage()
{
}

void CTreeCtrlPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTreeCtrlPage)
	DDX_Control(pDX, IDC_TREESTATUS, m_notifshowcase);
	DDX_Control(pDX, IDC_TREEVIEW1, m_mytreectrl);
	DDX_Check(pDX, IDC_TVDISABLEDDRAGDROP, m_bDisableDragDrop);
	DDX_Check(pDX, IDC_TVHASBUTTONS, m_bHasButtons);
	DDX_Check(pDX, IDC_TVHASLINES, m_bHasLines);
	DDX_Check(pDX, IDC_TVLINESATROOT, m_bLinesAtRoot);
	DDX_Check(pDX, IDC_TVEDITLABELS, m_bEditLabels);
	DDX_Check(pDX, IDC_TVSHOWSELALWAYS, m_bShowSelAlways);
	DDX_Text(pDX, IDC_TREESTATUS, m_cstrStatus);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTreeCtrlPage, CPropertyPage)
	//{{AFX_MSG_MAP(CTreeCtrlPage)
	ON_BN_CLICKED(IDC_TVEDITLABELS, OnEditLabels)
	ON_BN_CLICKED(IDC_TVHASBUTTONS, OnHasButtons)
	ON_BN_CLICKED(IDC_TVHASLINES, OnHasLines)
	ON_BN_CLICKED(IDC_TVLINESATROOT, OnLinesAtRoot)
	ON_BN_CLICKED(IDC_TVDISABLEDDRAGDROP, OnDisabledDragDrop)
	ON_BN_CLICKED(IDC_TVSHOWSELALWAYS, OnShowSelAlways)
	ON_NOTIFY(TVN_BEGINLABELEDIT, IDC_TREEVIEW1, ShowNotification)
	ON_NOTIFY(TVN_SELCHANGING, IDC_TREEVIEW1, ShowNotification)
	ON_NOTIFY(TVN_SELCHANGED, IDC_TREEVIEW1, ShowNotification)
	ON_NOTIFY(TVN_GETDISPINFO, IDC_TREEVIEW1, ShowNotification)
	ON_NOTIFY(TVN_SETDISPINFO, IDC_TREEVIEW1, ShowNotification)
	ON_NOTIFY(TVN_ITEMEXPANDING, IDC_TREEVIEW1, ShowNotification)
	ON_NOTIFY(TVN_ITEMEXPANDED, IDC_TREEVIEW1, ShowNotification)
	ON_NOTIFY(TVN_BEGINDRAG, IDC_TREEVIEW1, ShowNotification)
	ON_NOTIFY(TVN_KEYDOWN, IDC_TREEVIEW1, ShowNotification)
	ON_NOTIFY(TVN_SELCHANGED, IDC_TREEVIEW1, ShowNotification)
	ON_NOTIFY(TVN_SELCHANGING, IDC_TREEVIEW1, ShowNotification)
	ON_NOTIFY(TVN_ITEMEXPANDING, IDC_TREEVIEW1, ShowNotification)
	ON_NOTIFY(TVN_ITEMEXPANDED, IDC_TREEVIEW1, ShowNotification)
	ON_NOTIFY(TVN_DELETEITEM, IDC_TREEVIEW1, ShowNotification)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTreeCtrlPage message handlers
BOOL CTreeCtrlPage::OnInitDialog()
{
	CImageList			*pimagelist;
	CBitmap				bitmap;
	CCtrldemoApp		*pApp;
	int					iItem;
	UINT				nID;
	TV_INSERTSTRUCT		tvstruct;
	TCHAR				rgszItems[][18] = {_T("Dogs"), _T("German Shepherd"), _T("Dalmatian"), _T("Great Dane"),
								_T("Birds"), _T("Hummingbird"), _T("Pigeon"), _T("Eagle"),
								_T("Fish"), _T("Snapper"), _T("Sole"), _T("Salmon")};

	pApp = (CCtrldemoApp *)AfxGetApp();
	CPropertyPage::OnInitDialog();  // let the base class do the default work
	pimagelist = new CImageList();
	pimagelist->Create(32, 16, TRUE/*bMask*/, 6, 4);

	for (nID = IDB_BMTREEFIRST; nID <= IDB_BMTREELAST; nID++)  // load bitmaps for dog, bird and fish
	{
		bitmap.LoadBitmap(nID);
		pimagelist->Add(&bitmap, (COLORREF)0xFFFFFF);
		bitmap.DeleteObject();
	}

	m_mytreectrl.SetImageList(pimagelist, TVSIL_NORMAL);
	for (iItem = 0; iItem < CELEMS(rgszItems); iItem ++)
	{
		tvstruct.hParent = (iItem % 4 == 0)? NULL : m_rghItem[iItem / 4 * 4];
		tvstruct.hInsertAfter = TVI_SORT;
		tvstruct.item.iImage = iItem / 4 * 2;
		tvstruct.item.iSelectedImage = tvstruct.item.iImage + 1;
		tvstruct.item.pszText = rgszItems[iItem];
		tvstruct.item.mask = TVIF_IMAGE | TVIF_SELECTEDIMAGE | TVIF_TEXT;
		m_rghItem[iItem] = m_mytreectrl.InsertItem(&tvstruct);
	}

	return FALSE;
}

void CTreeCtrlPage::OnEditLabels()
{
	UpdateData(TRUE/*bSaveAndValidate*/);  // get information from the dialog
	m_mytreectrl.SetNewStyle(TVS_EDITLABELS, m_bEditLabels);
}

void CTreeCtrlPage::OnHasButtons()
{
	UpdateData(TRUE);
	m_mytreectrl.SetNewStyle(TVS_HASBUTTONS, m_bHasButtons);
}

void CTreeCtrlPage::OnHasLines()
{
	UpdateData(TRUE);
	m_mytreectrl.SetNewStyle(TVS_HASLINES, m_bHasLines);
}

void CTreeCtrlPage::OnLinesAtRoot()
{
	UpdateData(TRUE);
	m_mytreectrl.SetNewStyle(TVS_LINESATROOT, m_bLinesAtRoot);
}

void CTreeCtrlPage::OnDisabledDragDrop()
{
	UpdateData(TRUE);
	m_mytreectrl.SetNewStyle(TVS_DISABLEDRAGDROP, m_bDisableDragDrop);
}

void CTreeCtrlPage::OnShowSelAlways()
{
	UpdateData(TRUE);
	m_mytreectrl.SetNewStyle(TVS_SHOWSELALWAYS, m_bShowSelAlways);
}

void CTreeCtrlPage::ShowNotification(LPNMHDR pnmhdr, LRESULT *pResult)
{
	((CNotifyShowCase *)GetDlgItem(IDC_TREESTATUS))->ShowNotification(pnmhdr->code);
}
