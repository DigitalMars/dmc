// SmilePpg.cpp : Implementation of the CSmilePropPage property page class.

#include "stdafx.h"
#include "Smile.h"
#include "SmilePpg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


IMPLEMENT_DYNCREATE(CSmilePropPage, COlePropertyPage)


/////////////////////////////////////////////////////////////////////////////
// Message map

BEGIN_MESSAGE_MAP(CSmilePropPage, COlePropertyPage)
	//{{AFX_MSG_MAP(CSmilePropPage)
	// NOTE - ClassWizard will add and remove message map entries
	//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// Initialize class factory and guid

IMPLEMENT_OLECREATE_EX(CSmilePropPage, "SMILE.SmilePropPage.1",
	0x175cb004, 0xbeed, 0x11ce, 0x96, 0x11, 0, 0xaa, 0, 0x4a, 0x75, 0xcf)


/////////////////////////////////////////////////////////////////////////////
// CSmilePropPage::CSmilePropPageFactory::UpdateRegistry -
// Adds or removes system registry entries for CSmilePropPage

BOOL CSmilePropPage::CSmilePropPageFactory::UpdateRegistry(BOOL bRegister)
{
	if (bRegister)
		return AfxOleRegisterPropertyPageClass(AfxGetInstanceHandle(),
			m_clsid, IDS_Smile_PPG);
	else
		return AfxOleUnregisterClass(m_clsid, NULL);
}


/////////////////////////////////////////////////////////////////////////////
// CSmilePropPage::CSmilePropPage - Constructor

CSmilePropPage::CSmilePropPage() :
	COlePropertyPage(IDD, IDS_Smile_PPG_CAPTION)
{
	//{{AFX_DATA_INIT(CSmilePropPage)
	m_strCaption = _T("");
	m_bSad = FALSE;
	//}}AFX_DATA_INIT
}


/////////////////////////////////////////////////////////////////////////////
// CSmilePropPage::DoDataExchange - Moves data between page and properties

void CSmilePropPage::DoDataExchange(CDataExchange* pDX)
{
	//{{AFX_DATA_MAP(CSmilePropPage)
	DDP_Text(pDX, IDC_EDIT1, m_strCaption, _T("Caption") );
	DDX_Text(pDX, IDC_EDIT1, m_strCaption);
	DDP_Check(pDX, IDC_CHECK1, m_bSad, _T("Sad") );
	DDX_Check(pDX, IDC_CHECK1, m_bSad);
	//}}AFX_DATA_MAP
	DDP_PostProcessing(pDX);
}


/////////////////////////////////////////////////////////////////////////////
// CSmilePropPage message handlers
