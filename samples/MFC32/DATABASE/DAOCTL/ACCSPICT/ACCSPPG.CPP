// AccsPpg.cpp : Implementation of the CAccessPictPropPage property page class.

#include "stdafx.h"
#include "AccsPict.h"
#include "AccsPpg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


IMPLEMENT_DYNCREATE(CAccessPictPropPage, COlePropertyPage)


/////////////////////////////////////////////////////////////////////////////
// Message map

BEGIN_MESSAGE_MAP(CAccessPictPropPage, COlePropertyPage)
	//{{AFX_MSG_MAP(CAccessPictPropPage)
	ON_BN_CLICKED(IDC_STRETCH, OnStretch)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// Initialize class factory and guid

IMPLEMENT_OLECREATE_EX(CAccessPictPropPage, "ACCESSPICT.AccessPictPropPage.1",
	0x1f4e7c24, 0xc38f, 0x11ce, 0x96, 0x11, 0, 0xaa, 0, 0x4a, 0x75, 0xcf)


/////////////////////////////////////////////////////////////////////////////
// CAccessPictPropPage::CAccessPictPropPageFactory::UpdateRegistry -
// Adds or removes system registry entries for CAccessPictPropPage

BOOL CAccessPictPropPage::CAccessPictPropPageFactory::UpdateRegistry(BOOL bRegister)
{
	if (bRegister)
		return AfxOleRegisterPropertyPageClass(AfxGetInstanceHandle(),
			m_clsid, IDS_ACCESSPICT_PPG);
	else
		return AfxOleUnregisterClass(m_clsid, NULL);
}


/////////////////////////////////////////////////////////////////////////////
// CAccessPictPropPage::CAccessPictPropPage - Constructor

CAccessPictPropPage::CAccessPictPropPage() :
	COlePropertyPage(IDD, IDS_ACCESSPICT_PPG_CAPTION)
{
	//{{AFX_DATA_INIT(CAccessPictPropPage)
	m_bStretch = FALSE;
	m_bPreserve = FALSE;
	m_strCaption = _T("");
	//}}AFX_DATA_INIT
}


/////////////////////////////////////////////////////////////////////////////
// CAccessPictPropPage::DoDataExchange - Moves data between page and properties

void CAccessPictPropPage::DoDataExchange(CDataExchange* pDX)
{
	//{{AFX_DATA_MAP(CAccessPictPropPage)
	DDP_Check(pDX, IDC_STRETCH, m_bStretch, _T("StretchToFit") );
	DDX_Check(pDX, IDC_STRETCH, m_bStretch);
	DDP_Check(pDX, IDC_PRESERVE, m_bPreserve, _T("PreserveRatio") );
	DDX_Check(pDX, IDC_PRESERVE, m_bPreserve);
	DDP_Text(pDX, IDC_EDIT1, m_strCaption, _T("Caption") );
	DDX_Text(pDX, IDC_EDIT1, m_strCaption);
	//}}AFX_DATA_MAP
	DDP_PostProcessing(pDX);
}


/////////////////////////////////////////////////////////////////////////////
// CAccessPictPropPage message handlers

void CAccessPictPropPage::OnStretch() 
{
	// TODO: Add your control notification handler code here
	
}
