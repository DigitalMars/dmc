// ListCtrlPage.cpp : implementation file
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
#include "listcpg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define CELEMS(rgFoo) (sizeof(rgFoo) / sizeof(rgFoo[0]))

/////////////////////////////////////////////////////////////////////////////
// CListCtrlPage property page

IMPLEMENT_DYNCREATE(CListCtrlPage, CPropertyPage)

CListCtrlPage::CListCtrlPage() : CPropertyPage(CListCtrlPage::IDD)
{
	//{{AFX_DATA_INIT(CListCtrlPage)
	m_bAutoArrange = FALSE;
	m_bCallBacks = FALSE;
	m_bEditLabels = FALSE;
	m_bNoColumnHeader = FALSE;
	m_bNoLabelWrap = FALSE;
	m_bNoSortHeader = FALSE;
	m_bSingleSel = FALSE;
	m_cstrViewMode = _T("REPORT");  // report is the default mode
	m_cstrAlignMode = _T("ALIGNTOP");		// top alignment is the default mode.
	m_cstrSort = _T("None");
	m_cstrStatus = _T("");
	//}}AFX_DATA_INIT

	m_pimagelist = NULL;
	m_pimagelistSmall = NULL;
}

CListCtrlPage::~CListCtrlPage()
{
	delete m_pimagelist;
	delete m_pimagelistSmall;
}


void CListCtrlPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CListCtrlPage)
	DDX_Control(pDX, IDC_LISTVIEW1, m_listctrl);
	DDX_Check(pDX, IDC_AUTOARRANGE, m_bAutoArrange);
	DDX_Check(pDX, IDC_EDITLABELS, m_bEditLabels);
	DDX_Check(pDX, IDC_NOCOLUMNHEADER, m_bNoColumnHeader);
	DDX_Check(pDX, IDC_NOLABELWRAP, m_bNoLabelWrap);
	DDX_Check(pDX, IDC_NOSORTHEADER, m_bNoSortHeader);
	DDX_Check(pDX, IDC_SINGLESEL, m_bSingleSel);
	DDX_CBString(pDX, IDC_VIEWMODE, m_cstrViewMode);
	DDX_CBString(pDX, IDC_ALIGN, m_cstrAlignMode);
	DDX_CBString(pDX, IDC_SORT, m_cstrSort);
	DDX_Text(pDX, IDC_STATUS, m_cstrStatus);
	//}}AFX_DATA_MAP
}

//	ON_NOTIFY(LVN_BEGINDRAG, IDC_LISTVIEW1, OnBeginDrag)
//	ON_WM_MOUSEMOVE()
//	ON_WM_LBUTTONUP()
//	ON_WM_RBUTTONUP()

BEGIN_MESSAGE_MAP(CListCtrlPage, CPropertyPage)
	//{{AFX_MSG_MAP(CListCtrlPage)
	ON_NOTIFY(LVN_ITEMCHANGING, IDC_LISTVIEW1, ShowNotification)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LISTVIEW1, ShowNotification)
	ON_NOTIFY(LVN_INSERTITEM, IDC_LISTVIEW1, ShowNotification)
	ON_NOTIFY(LVN_DELETEITEM, IDC_LISTVIEW1, ShowNotification)
	ON_NOTIFY(LVN_DELETEALLITEMS, IDC_LISTVIEW1, ShowNotification)
	ON_NOTIFY(LVN_BEGINLABELEDIT, IDC_LISTVIEW1, ShowNotification)
	ON_NOTIFY(LVN_ENDLABELEDIT, IDC_LISTVIEW1, ShowNotification)
	ON_NOTIFY(LVN_COLUMNCLICK, IDC_LISTVIEW1, ShowNotification)
	ON_NOTIFY(LVN_GETDISPINFO, IDC_LISTVIEW1, ShowNotification)
	ON_NOTIFY(LVN_SETDISPINFO, IDC_LISTVIEW1, ShowNotification)
	ON_BN_CLICKED(IDC_AUTOARRANGE, OnAutoArrange)
	ON_BN_CLICKED(IDC_SINGLESEL, OnSingleSel)
	ON_BN_CLICKED(IDC_NOLABELWRAP, OnNoLabelWrap)
	ON_BN_CLICKED(IDC_EDITLABELS, OnEditLabels)
	ON_BN_CLICKED(IDC_NOCOLUMNHEADER, OnNoColumnHeader)
	ON_BN_CLICKED(IDC_NOSORTHEADER, OnNoSortHeader)
	ON_CBN_SELCHANGE(IDC_VIEWMODE, OnChangeViewMode)
	ON_CBN_SELCHANGE(IDC_ALIGN, OnChangeAlignMode)
	ON_CBN_SELCHANGE(IDC_SORT, OnChangeSortMode)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CListCtrlPage message handlers
void CListCtrlPage::ChangeListCtrlStyle(long lStyle, BOOL bSetBit)
{
	long	lStyleOld;
	CRect	rect;

	m_listctrl.GetWindowRect(&rect);
	ScreenToClient(&rect);
	ASSERT(lStyle != 0);  // watch out for LVS_foo DEFINITIONS WHICH ARE 0.
	lStyleOld = GetWindowLong(m_listctrl.m_hWnd, GWL_STYLE);
	lStyleOld &= ~lStyle;  // turn off bits specified by caller.
	if (bSetBit)
		lStyleOld |= lStyle;

	SetWindowLong(m_listctrl.m_hWnd, GWL_STYLE, lStyleOld);
	m_listctrl.SetWindowPos(NULL, rect.left, rect.top, rect.Width(), rect.Height(),	SWP_NOZORDER | SWP_SHOWWINDOW);

	UpdateData(FALSE/*bSaveAndValidate*/);  // send information back to the dialog
}

void CListCtrlPage::OnAutoArrange()
{
	UpdateData(TRUE);  // get the information from the dialog
	ChangeListCtrlStyle(LVS_AUTOARRANGE, m_bAutoArrange);
}

void CListCtrlPage::OnSingleSel()
{
	UpdateData(TRUE/*bSaveAndValidate*/);
	ChangeListCtrlStyle(LVS_SINGLESEL, m_bSingleSel);
}

void CListCtrlPage::OnNoLabelWrap()
{
	UpdateData(TRUE);
	ChangeListCtrlStyle(LVS_NOLABELWRAP, m_bNoLabelWrap);
}

void CListCtrlPage::OnEditLabels()
{
	UpdateData(TRUE);
	ChangeListCtrlStyle(LVS_EDITLABELS, m_bEditLabels);
}

void CListCtrlPage::OnNoColumnHeader()
{
	UpdateData(TRUE);
	RenewListCtrl(LVS_NOCOLUMNHEADER, m_bNoColumnHeader);
}

void CListCtrlPage::OnNoSortHeader()
{
	UpdateData(TRUE);
	RenewListCtrl(LVS_NOSORTHEADER, m_bNoSortHeader);
}

void CListCtrlPage::OnChangeViewMode()
{
	long		lStyle, lStyleOld;
	BOOL		bReport, bIconic;

	UpdateData(TRUE);
	if (m_cstrViewMode == _T("ICON"))
		lStyle = LVS_ICON;
	else if (m_cstrViewMode == _T("SMALL ICON"))
		lStyle = LVS_SMALLICON;
	else if (m_cstrViewMode == _T("REPORT"))
		lStyle = LVS_REPORT;
	else
	{
		ASSERT(m_cstrViewMode == _T("LIST"));
		lStyle = LVS_LIST;
	}

	bReport = lStyle == LVS_REPORT;
	bIconic = lStyle == LVS_ICON || lStyle == LVS_SMALLICON;
	GetDlgItem(IDC_NOSORTHEADER)->EnableWindow(bReport);
	GetDlgItem(IDC_NOCOLUMNHEADER)->EnableWindow(bReport);
	GetDlgItem(IDC_ALIGN)->EnableWindow(bIconic);
	GetDlgItem(IDC_AUTOARRANGE)->EnableWindow(bIconic);

	lStyleOld = GetWindowLong(m_listctrl.m_hWnd, GWL_STYLE);
	lStyleOld &= ~(LVS_TYPEMASK);  // turn off all the style (view mode) bits
	lStyleOld |= lStyle;		// Set the new Style for the control
	SetWindowLong(m_listctrl.m_hWnd, GWL_STYLE, lStyleOld);  // done!
}

void CListCtrlPage::OnChangeSortMode()
{
	long	lStyle;

	UpdateData(TRUE/*bSaveAndValidate*/);
	lStyle = GetWindowLong(m_listctrl.m_hWnd, GWL_STYLE);
	lStyle &= ~(LVS_SORTASCENDING | LVS_SORTDESCENDING);
	SetWindowLong(m_listctrl.m_hWnd, GWL_STYLE, lStyle);  // set style without sorting 
	if (m_cstrSort == _T("None"))
		RenewListCtrl(LVS_SORTASCENDING | LVS_SORTDESCENDING, FALSE/*bSetBits*/);
	else if (m_cstrSort == _T("ASCENDING"))
		RenewListCtrl(LVS_SORTASCENDING, TRUE);
	else
	{
		ASSERT(m_cstrSort == _T("DESCENDING"));
		RenewListCtrl(LVS_SORTDESCENDING, TRUE);
	}
}

void CListCtrlPage::OnChangeAlignMode()
{
	// this function takes into consideration that LVS_ALIGNTOP is defined as zero.
	UpdateData(TRUE);
	ASSERT(m_cstrAlignMode == _T("ALIGNTOP") || m_cstrAlignMode == _T("ALIGNLEFT"));
	ChangeListCtrlStyle(LVS_ALIGNLEFT, (m_cstrAlignMode == _T("ALIGNLEFT")));
}

void CListCtrlPage::FillListCtrl()
{
	CRect			rect;
	int				i, iIcon, iItem, iSubItem, iActualItem;
	LV_ITEM			lvitem;
	LV_COLUMN		lvcolumn;
	TCHAR			rgtsz[2][10] = {_T("MAIN ITEM"), _T("SUB_ITEM")};
	CCtrldemoApp	*pApp;
	TCHAR			rgtszIconDescrip[LISTICONCOUNT][50], rgtszIconShortDesc[LISTICONCOUNT][12];

	ASSERT(LISTICONCOUNT == 4);
	_tcscpy(rgtszIconDescrip[0], _T("Blue Ellipse, Yellow Triangle, Red Rectangle"));
	_tcscpy(rgtszIconDescrip[1], _T("Yellow Ellipse, Red Triangle, Blue Rectangle"));
	_tcscpy(rgtszIconDescrip[2], _T("Red Ellipse, Blue Triangle, Yellow Rectangle"));
	_tcscpy(rgtszIconDescrip[3], _T("Red Ellipse, Yellow Triangle, Blue Rectangle"));

	_tcscpy(rgtszIconShortDesc[0], _T("BE, YT, RR"));
	_tcscpy(rgtszIconShortDesc[1], _T("YE, RT, BR"));
	_tcscpy(rgtszIconShortDesc[2], _T("RE, BT, YR"));
	_tcscpy(rgtszIconShortDesc[3], _T("RE, YT, BR"));


	pApp = (CCtrldemoApp *)AfxGetApp();
	m_listctrl.SetImageList(m_pimagelist, LVSIL_NORMAL);
	m_listctrl.SetImageList(m_pimagelistSmall, LVSIL_SMALL);
	m_listctrl.GetWindowRect(&rect);

	for (i = 0; i < 2; i++)  // add the columns to the list control
	{
		lvcolumn.mask = LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH;
		lvcolumn.fmt = LVCFMT_LEFT;
		lvcolumn.pszText = rgtsz[i];
		lvcolumn.iSubItem = i;
		lvcolumn.cx = rect.Width() * (i + 1) / 3;  // SubItem is twice as large
		m_listctrl.InsertColumn(i, &lvcolumn);  // assumes return value is OK.
	}

	for (iItem = 0; iItem < 50; iItem++)  // will now insert the items and subitems into the list view.
		for (iSubItem = 0; iSubItem < 2; iSubItem++)
		{
			if (iSubItem == 0)
				iIcon = rand() % 4;  // choose the icon and legend for the entry

			lvitem.mask = LVIF_TEXT | (iSubItem == 0? LVIF_IMAGE : 0);
			lvitem.iItem = (iSubItem == 0)? iItem : iActualItem;
			lvitem.iSubItem = iSubItem;
			lvitem.pszText = iSubItem == 0? rgtszIconShortDesc[iIcon] : rgtszIconDescrip[iIcon];

			lvitem.iImage = iIcon;
			if (iSubItem == 0)
				iActualItem = m_listctrl.InsertItem(&lvitem);
			else
				m_listctrl.SetItem(&lvitem);
		}
}

void CListCtrlPage::RenewListCtrl(long lStyle, BOOL bSetBits)
{
	long	lStyleOld;
	CRect	rect;

	lStyleOld = GetWindowLong(m_listctrl.m_hWnd, GWL_STYLE);
	lStyleOld &= ~lStyle;
	if (bSetBits)
		lStyleOld |= lStyle;

	m_listctrl.GetWindowRect(&rect);
	ScreenToClient(&rect);
	m_listctrl.DestroyWindow();
	m_listctrl.Create(lStyleOld, rect, this, IDC_LISTVIEW1);
	m_listctrl.SetImageList(m_pimagelist, LVSIL_NORMAL);
	m_listctrl.SetImageList(m_pimagelistSmall, LVSIL_SMALL);
	m_listctrl.GetWindowRect(&rect);
	FillListCtrl();
}

BOOL CListCtrlPage::OnInitDialog()
{
	CCtrldemoApp	*pApp;
	CRect			rect;
	UINT			rgIndicators[] = {ID_SEPARATOR, ID_INDICATOR_CAPS, ID_INDICATOR_NUM};

	CPropertyPage::OnInitDialog();  // let the base class do the default work
	UpdateData(TRUE/*bSaveAndValidate*/);  // bring the information from the dialog.
	pApp = (CCtrldemoApp *)AfxGetApp();
	srand((unsigned) time(NULL));  // start the random number generator
	m_pimagelist = new CImageList();		
	m_pimagelistSmall = new CImageList();
	ASSERT(m_pimagelist != NULL && m_pimagelistSmall != NULL);    // no serious allocation failure checking
	m_pimagelist->Create(32, 32, TRUE/*bMask*/,	LISTICONCOUNT/*nInitial*/, 4/*nGrow*/);
	m_pimagelistSmall->Create(16, 16, TRUE/*bMask*/, LISTICONCOUNT, 4);
	m_pimagelist->Add(pApp->LoadIcon(IDI_ICONLIST1));
	m_pimagelist->Add(pApp->LoadIcon(IDI_ICONLIST2));
	m_pimagelist->Add(pApp->LoadIcon(IDI_ICONLIST3));
	m_pimagelist->Add(pApp->LoadIcon(IDI_ICONLIST4));
	m_pimagelistSmall->Add(pApp->LoadIcon(IDI_ICONLIST1));
	m_pimagelistSmall->Add(pApp->LoadIcon(IDI_ICONLIST2));
	m_pimagelistSmall->Add(pApp->LoadIcon(IDI_ICONLIST3));
	m_pimagelistSmall->Add(pApp->LoadIcon(IDI_ICONLIST4));

	FillListCtrl();
	GetDlgItem(IDC_ALIGN)->EnableWindow(FALSE);  // default is  report mode Does not support this
	GetDlgItem(IDC_AUTOARRANGE)->EnableWindow(FALSE);
	return FALSE;  // there is no change in any control focus stuff here.
}

void CListCtrlPage::ShowNotification(LPNMHDR pnmhdr, LRESULT *pResult)
{
	((CNotifyShowCase *)GetDlgItem(IDC_LISTSTATUS))->ShowNotification(pnmhdr->code);
}
