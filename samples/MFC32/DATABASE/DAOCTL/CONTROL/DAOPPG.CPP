// DaoEditPpg.cpp : Implementation of the CDaoEditPropPage property page class.

#include "stdafx.h"
#include "DaoEdit.h"
#include "DaoPpg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


IMPLEMENT_DYNCREATE(CDaoEditPropPage, COlePropertyPage)


/////////////////////////////////////////////////////////////////////////////
// Message map

BEGIN_MESSAGE_MAP(CDaoEditPropPage, COlePropertyPage)
	//{{AFX_MSG_MAP(CDaoEditPropPage)
	// NOTE - ClassWizard will add and remove message map entries
	//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// Initialize class factory and guid

IMPLEMENT_OLECREATE_EX(CDaoEditPropPage, "DAOEDIT.DaoEditPropPage.1",
	0x5fdfc3e4, 0xc360, 0x11ce, 0xa9, 0x87, 0, 0xaa, 0, 0x6e, 0xa4, 0xda)


/////////////////////////////////////////////////////////////////////////////
// CDaoEditPropPage::CDaoEditPropPageFactory::UpdateRegistry -
// Adds or removes system registry entries for CDaoEditPropPage

BOOL CDaoEditPropPage::CDaoEditPropPageFactory::UpdateRegistry(BOOL bRegister)
{
	if (bRegister)
		return AfxOleRegisterPropertyPageClass(AfxGetInstanceHandle(),
			m_clsid, IDS_DAOEDIT_PPG);
	else
		return AfxOleUnregisterClass(m_clsid, NULL);
}


/////////////////////////////////////////////////////////////////////////////
// CDaoEditPropPage::CDaoEditPropPage - Constructor

CDaoEditPropPage::CDaoEditPropPage() :
	COlePropertyPage(IDD, IDS_DAOEDIT_PPG_CAPTION)
{
	//{{AFX_DATA_INIT(CDaoEditPropPage)
	// NOTE: ClassWizard will add member initialization here
	//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_DATA_INIT
}


/////////////////////////////////////////////////////////////////////////////
// CDaoEditPropPage::DoDataExchange - Moves data between page and properties

void CDaoEditPropPage::DoDataExchange(CDataExchange* pDX)
{
	//{{AFX_DATA_MAP(CDaoEditPropPage)
	// NOTE: ClassWizard will add DDP, DDX, and DDV calls here
	//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_DATA_MAP
	DDP_PostProcessing(pDX);
}


/////////////////////////////////////////////////////////////////////////////
// CDaoEditPropPage message handlers
