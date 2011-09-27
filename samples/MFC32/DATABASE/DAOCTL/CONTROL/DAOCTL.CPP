// DaoEditCtl.cpp : Implementation of the CDaoEditCtrl OLE control class.

#include "stdafx.h"
#include "DaoEdit.h"
#include "DaoCtl.h"
#include "DaoPpg.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


IMPLEMENT_DYNCREATE(CDaoEditCtrl, COleControl)


/////////////////////////////////////////////////////////////////////////////
// Message map

BEGIN_MESSAGE_MAP(CDaoEditCtrl, COleControl)
	//{{AFX_MSG_MAP(CDaoEditCtrl)
	ON_WM_KILLFOCUS()
	ON_WM_CHAR()
	ON_WM_KEYDOWN()
	//}}AFX_MSG_MAP
	ON_MESSAGE(OCM_COMMAND, OnOcmCommand)
	ON_OLEVERB(AFX_IDS_VERB_PROPERTIES, OnProperties)
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// Dispatch map

BEGIN_DISPATCH_MAP(CDaoEditCtrl, COleControl)
	//{{AFX_DISPATCH_MAP(CDaoEditCtrl)
	DISP_PROPERTY(CDaoEditCtrl, "FieldName", m_strFieldName, VT_BSTR)
	DISP_DEFVALUE(CDaoEditCtrl, "Text")
	DISP_STOCKPROP_TEXT()
	DISP_STOCKPROP_BORDERSTYLE()
	//}}AFX_DISPATCH_MAP
	DISP_FUNCTION_ID(CDaoEditCtrl, "AboutBox", DISPID_ABOUTBOX, AboutBox, VT_EMPTY, VTS_NONE)
END_DISPATCH_MAP()


/////////////////////////////////////////////////////////////////////////////
// Event map

BEGIN_EVENT_MAP(CDaoEditCtrl, COleControl)
	//{{AFX_EVENT_MAP(CDaoEditCtrl)
	//}}AFX_EVENT_MAP
END_EVENT_MAP()


/////////////////////////////////////////////////////////////////////////////
// Property pages

// TODO: Add more property pages as needed.  Remember to increase the count!
BEGIN_PROPPAGEIDS(CDaoEditCtrl, 1)
	PROPPAGEID(CDaoEditPropPage::guid)
END_PROPPAGEIDS(CDaoEditCtrl)


/////////////////////////////////////////////////////////////////////////////
// Initialize class factory and guid

IMPLEMENT_OLECREATE_EX(CDaoEditCtrl, "DAOEDIT.DaoEditCtrl.1",
	0x5fdfc3e3, 0xc360, 0x11ce, 0xa9, 0x87, 0, 0xaa, 0, 0x6e, 0xa4, 0xda)


/////////////////////////////////////////////////////////////////////////////
// Type library ID and version

IMPLEMENT_OLETYPELIB(CDaoEditCtrl, _tlid, _wVerMajor, _wVerMinor)


/////////////////////////////////////////////////////////////////////////////
// Interface IDs

const IID BASED_CODE IID_DDaoEdit =
		{ 0x5fdfc3e1, 0xc360, 0x11ce, { 0xa9, 0x87, 0, 0xaa, 0, 0x6e, 0xa4, 0xda } };
const IID BASED_CODE IID_DDaoEditEvents =
		{ 0x5fdfc3e2, 0xc360, 0x11ce, { 0xa9, 0x87, 0, 0xaa, 0, 0x6e, 0xa4, 0xda } };


/////////////////////////////////////////////////////////////////////////////
// Control type information

static const DWORD BASED_CODE _dwDaoEditOleMisc =
	OLEMISC_ACTIVATEWHENVISIBLE |
	OLEMISC_SETCLIENTSITEFIRST |
	OLEMISC_INSIDEOUT |
	OLEMISC_CANTLINKINSIDE |
	OLEMISC_RECOMPOSEONRESIZE;

IMPLEMENT_OLECTLTYPE(CDaoEditCtrl, IDS_DAOEDIT, _dwDaoEditOleMisc)


/////////////////////////////////////////////////////////////////////////////
// CDaoEditCtrl::CDaoEditCtrlFactory::UpdateRegistry -
// Adds or removes system registry entries for CDaoEditCtrl

BOOL CDaoEditCtrl::CDaoEditCtrlFactory::UpdateRegistry(BOOL bRegister)
{
	if (bRegister)
		return AfxOleRegisterControlClass(
			AfxGetInstanceHandle(),
			m_clsid,
			m_lpszProgID,
			IDS_DAOEDIT,
			IDB_DAOEDIT,
			FALSE,                      //  Not insertable
			_dwDaoEditOleMisc,
			_tlid,
			_wVerMajor,
			_wVerMinor);
	else
		return AfxOleUnregisterClass(m_clsid, m_lpszProgID);
}


/////////////////////////////////////////////////////////////////////////////
// CDaoEditCtrl::CDaoEditCtrl - Constructor

CDaoEditCtrl::CDaoEditCtrl()
{
	InitializeIIDs(&IID_DDaoEdit, &IID_DDaoEditEvents);

	m_bDirty = FALSE;
}


/////////////////////////////////////////////////////////////////////////////
// CDaoEditCtrl::~CDaoEditCtrl - Destructor

CDaoEditCtrl::~CDaoEditCtrl()
{
}


/////////////////////////////////////////////////////////////////////////////
// CDaoEditCtrl::OnDraw - Drawing function

void CDaoEditCtrl::OnDraw(
			CDC* pdc, const CRect& rcBounds, const CRect& rcInvalid)
{
	DoSuperclassPaint(pdc, rcBounds);
}


/////////////////////////////////////////////////////////////////////////////
// CDaoEditCtrl::DoPropExchange - Persistence support

void CDaoEditCtrl::DoPropExchange(CPropExchange* pPX)
{
	ExchangeVersion(pPX, MAKELONG(_wVerMinor, _wVerMajor));
	COleControl::DoPropExchange(pPX);

	// TODO: Call PX_ functions for each persistent custom property.

}


/////////////////////////////////////////////////////////////////////////////
// CDaoEditCtrl::OnResetState - Reset control to default state

void CDaoEditCtrl::OnResetState()
{
	COleControl::OnResetState();  // Resets defaults found in DoPropExchange

	// TODO: Reset any other control state here.
}


/////////////////////////////////////////////////////////////////////////////
// CDaoEditCtrl::AboutBox - Display an "About" box to the user

void CDaoEditCtrl::AboutBox()
{
	CDialog dlgAbout(IDD_ABOUTBOX_DAOEDIT);
	dlgAbout.DoModal();
}


/////////////////////////////////////////////////////////////////////////////
// CDaoEditCtrl::PreCreateWindow - Modify parameters for CreateWindowEx

BOOL CDaoEditCtrl::PreCreateWindow(CREATESTRUCT& cs)
{
	cs.lpszClass = _T("EDIT");
	cs.style |= ES_AUTOHSCROLL;
	return COleControl::PreCreateWindow(cs);
}


/////////////////////////////////////////////////////////////////////////////
// CDaoEditCtrl::IsSubclassedControl - This is a subclassed control

BOOL CDaoEditCtrl::IsSubclassedControl()
{
	return TRUE;
}


/////////////////////////////////////////////////////////////////////////////
// CDaoEditCtrl::OnOcmCommand - Handle command messages

LRESULT CDaoEditCtrl::OnOcmCommand(WPARAM wParam, LPARAM lParam)
{
#ifdef _WIN32
	WORD wNotifyCode = HIWORD(wParam);
#else
	WORD wNotifyCode = HIWORD(lParam);
#endif

	// TODO: Switch on wNotifyCode here.

	return 0;
}


/////////////////////////////////////////////////////////////////////////////


void CDaoEditCtrl::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	if (nChar != VK_DELETE)
	{
		COleControl::OnKeyDown(nChar, nRepCnt, nFlags);
		return;
	}
	if (BoundPropertyRequestEdit(DISPID_TEXT))
	{
		m_bDirty = TRUE;
		COleControl::OnKeyDown(nChar, nRepCnt, nFlags);
	}
}

void CDaoEditCtrl::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	if (BoundPropertyRequestEdit(DISPID_TEXT))
	{
		m_bDirty = TRUE;
		COleControl::OnChar(nChar, nRepCnt, nFlags);
	}
}

void CDaoEditCtrl::OnKillFocus(CWnd* pNewWnd) 
{
	COleControl::OnKillFocus(pNewWnd);
	
	if (m_bDirty)
	{
		BoundPropertyChanged(DISPID_TEXT);
		m_bDirty = FALSE;
	}
}
