// ProgCtrl.cpp : implementation file
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
#include "ProgCtrl.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CProgCtrlPage property page

IMPLEMENT_DYNCREATE(CProgCtrlPage, CPropertyPage)

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CProgCtrlPage::CProgCtrlPage() : CPropertyPage(CProgCtrlPage::IDD)
{
	//{{AFX_DATA_INIT(CProgCtrlPage)
	m_iDelta = 0;
	m_uiStep = 10;
	m_uiRangeFrom = 0;
	m_uiPos = 0;
	m_uiRangeTo = 100;
	//}}AFX_DATA_INIT
}

CProgCtrlPage::~CProgCtrlPage()
{
}

void CProgCtrlPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CProgCtrlPage)
	DDX_Control(pDX, IDC_PROGRESS, m_Progress);
	DDX_Text(pDX, IDC_PROGRESS_DELTA, m_iDelta);
	DDX_Text(pDX, IDC_PROGRESS_STEP, m_uiStep);
	DDV_MinMaxUInt(pDX, m_uiStep, 0, 65535);
	DDX_Text(pDX, IDC_PROGRESS_FROM, m_uiRangeFrom);
	DDV_MinMaxUInt(pDX, m_uiRangeFrom, 0, 65535);
	DDX_Text(pDX, IDC_PROGRESS_POS, m_uiPos);
	DDV_MinMaxUInt(pDX, m_uiPos, 0, 65535);
	DDX_Text(pDX, IDC_PROGRESS_TO, m_uiRangeTo);
	DDV_MinMaxUInt(pDX, m_uiRangeTo, 0, 65535);
	//}}AFX_DATA_MAP
}

BOOL CProgCtrlPage::OnInitDialog()
{
	CPropertyPage::OnInitDialog();
	m_Progress.SetRange( m_uiRangeFrom, m_uiRangeTo );
	return TRUE;
}

BEGIN_MESSAGE_MAP(CProgCtrlPage, CPropertyPage)
	//{{AFX_MSG_MAP(CProgCtrlPage)
	ON_BN_CLICKED(IDC_PROGRESS_STEPIT, OnProgressStepit)
	ON_BN_CLICKED(IDC_SETPOS, OnSetpos)
	ON_BN_CLICKED(IDC_PROGRESS_SETDELTA, OnProgressSetdelta)
	ON_EN_KILLFOCUS(IDC_PROGRESS_FROM, OnFrom)
	ON_EN_KILLFOCUS(IDC_PROGRESS_TO, OnTo)
	ON_EN_KILLFOCUS(IDC_PROGRESS_STEP, OnStep)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CProgCtrlPage message handlers

void CProgCtrlPage::OnProgressStepit() 
{
	m_Progress.StepIt();
}

void CProgCtrlPage::OnSetpos() 
{
	UpdateData();
	m_Progress.SetPos( m_uiPos );
}

void CProgCtrlPage::OnProgressSetdelta() 
{
	UpdateData();
	m_Progress.OffsetPos( m_iDelta );
}

void CProgCtrlPage::OnFrom() 
{
	UpdateData();
	m_Progress.SetRange( m_uiRangeFrom, m_uiRangeTo );
}

void CProgCtrlPage::OnTo() 
{
	UpdateData();
	m_Progress.SetRange( m_uiRangeFrom, m_uiRangeTo );
}

void CProgCtrlPage::OnStep() 
{
	UpdateData();
	m_Progress.SetStep( m_uiStep );
}
