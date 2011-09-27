// mainfrm.cpp : implementation of the CMainFrame class
//
// This is a part of the Microsoft Foundation Classes C++ library.
// Copyright (C) 1992 Microsoft Corporation
// All rights reserved.
//
// This source code is only intended as a supplement to the
// Microsoft Foundation Classes Reference and Microsoft
// QuickHelp and/or WinHelp documentation provided with the library.
// See these sources for detailed information regarding the
// Microsoft Foundation Classes product.



#include "stdafx.h"
#include "vbchart.h"

#include "mainfrm.h"
#include "chartdoc.h"
#include "chartvw.h"    // for different chart types

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMainFrame

IMPLEMENT_DYNAMIC(CMainFrame, CMDIFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CMDIFrameWnd)
	//{{AFX_MSG_MAP(CMainFrame)
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// arrays of IDs used to initialize control bars

// toolbar buttons - IDs are command buttons
static UINT BASED_CODE buttons[] =
{
	// same order as in the bitmap 'toolbar.bmp'
	ID_FILE_NEW,
	ID_FILE_OPEN,
	ID_FILE_SAVE,
		ID_SEPARATOR,
	ID_FILE_PRINT,
	ID_APP_ABOUT,
		ID_SEPARATOR,
		ID_SEPARATOR        // Index 7, Will be location for DropList
};
#define INDEX_DROPLIST 7

struct CHART_TYPES
{
	char*   pType;
	int     nType;
};

static CHART_TYPES chartNames[] =
{
	"3D Vert Bar",      CHART_3DVBAR,
	"Line",             CHART_LINE,
	"Bar (Vertical)",   CHART_VBAR,
	"Bar (Horizontal)", CHART_HBAR,
	"3D Horiz Bar",     CHART_3DHBAR,
	"Gantt (Vertical)", CHART_VGANTT,
	"Gantt (Horizontal)", CHART_HGANTT,
	NULL,               0
};

static UINT BASED_CODE indicators[] =
{
	ID_SEPARATOR,           // status line indicator
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

/////////////////////////////////////////////////////////////////////////////
// CMainFrame construction/destruction

CMainFrame::CMainFrame()
{
}

CMainFrame::~CMainFrame()
{
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CMDIFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	if (!m_wndToolBar.Create(this) ||
		!m_wndToolBar.LoadBitmap(IDR_MAINFRAME) ||
		!m_wndToolBar.SetButtons(buttons,
		  sizeof(buttons)/sizeof(UINT)))
	{
		TRACE("Failed to create toolbar\n");
		return -1;      // fail to create
	}

	CRect rect;
	m_wndToolBar.GetItemRect(INDEX_DROPLIST, &rect);
	int cyFit = rect.Height();
	rect.top = 1;   // 1 pixel down from top of statusbar
	rect.right = rect.left + 150;
	rect.bottom = rect.top + 200;       // drop height

	if (!m_cboxDrop.Create(
		WS_CHILD|WS_BORDER|WS_VISIBLE|CBS_DROPDOWNLIST|CBS_SORT,
		rect, &m_wndToolBar, IDC_CHARTLIST))
	{
		TRACE("Failed to create combobox inside toolbar\n");
		return -1;      // fail to create
	}

	// load status bar font
	LOGFONT logfont;
	memset(&logfont, 0, sizeof(logfont));

	// 13 pixel high Sans Serif font
	logfont.lfHeight = -13;
	logfont.lfWeight = FW_NORMAL;
	logfont.lfPitchAndFamily = VARIABLE_PITCH | FF_SWISS;
	static char BASED_CODE szFaceName[] = "MS Sans Serif";
	lstrcpy(logfont.lfFaceName, szFaceName);

	if (m_fontDrop.CreateFontIndirect(&logfont))
	{
		m_cboxDrop.SetFont(&m_fontDrop, FALSE);
				// no need to redraw since invisible
	}

	for (CHART_TYPES* pType = chartNames; pType->pType != NULL; pType++)
	{
		int index = m_cboxDrop.AddString(pType->pType);
		m_cboxDrop.SetItemData(index, pType->nType);
	}

	m_cboxDrop.SelectString(-1, chartNames[0].pType);

	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		  sizeof(indicators)/sizeof(UINT)))
	{
		TRACE("Failed to create status bar\n");
		return -1;      // fail to create
	}

	return 0;
}


/////////////////////////////////////////////////////////////////////////////
// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CMDIFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CMDIFrameWnd::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
