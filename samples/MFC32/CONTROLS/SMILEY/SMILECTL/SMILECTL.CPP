// SmileCtl.cpp : Implementation of the CSmileCtrl OLE control class.

#include "stdafx.h"
#include "Smile.h"
#include "SmileCtl.h"
#include "SmilePpg.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


IMPLEMENT_DYNCREATE(CSmileCtrl, COleControl)


/////////////////////////////////////////////////////////////////////////////
// Message map

BEGIN_MESSAGE_MAP(CSmileCtrl, COleControl)
	//{{AFX_MSG_MAP(CSmileCtrl)
	ON_WM_LBUTTONDOWN()
	//}}AFX_MSG_MAP
	ON_OLEVERB(AFX_IDS_VERB_PROPERTIES, OnProperties)
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// Dispatch map

BEGIN_DISPATCH_MAP(CSmileCtrl, COleControl)
	//{{AFX_DISPATCH_MAP(CSmileCtrl)
	DISP_PROPERTY_NOTIFY(CSmileCtrl, "Sad", m_bSad, OnSadChanged, VT_BOOL)
	DISP_FUNCTION(CSmileCtrl, "Beep", Beep, VT_EMPTY, VTS_NONE)
	DISP_FUNCTION(CSmileCtrl, "Wink", Wink, VT_EMPTY, VTS_BOOL)
	DISP_STOCKPROP_BACKCOLOR()
	DISP_STOCKPROP_FONT()
	DISP_STOCKPROP_FORECOLOR()
	DISP_STOCKPROP_CAPTION()
	DISP_STOCKPROP_BORDERSTYLE()
	//}}AFX_DISPATCH_MAP
	DISP_FUNCTION_ID(CSmileCtrl, "AboutBox", DISPID_ABOUTBOX, AboutBox, VT_EMPTY, VTS_NONE)
END_DISPATCH_MAP()


/////////////////////////////////////////////////////////////////////////////
// Event map

BEGIN_EVENT_MAP(CSmileCtrl, COleControl)
	//{{AFX_EVENT_MAP(CSmileCtrl)
	EVENT_CUSTOM("Outside", FireOutside, VTS_NONE)
	EVENT_CUSTOM("Inside", FireInside, VTS_XPOS_PIXELS  VTS_YPOS_PIXELS)
	//}}AFX_EVENT_MAP
END_EVENT_MAP()


/////////////////////////////////////////////////////////////////////////////
// Property pages

// TODO: Add more property pages as needed.  Remember to increase the count!
BEGIN_PROPPAGEIDS(CSmileCtrl, 3)
	PROPPAGEID(CSmilePropPage::guid)
	PROPPAGEID(CLSID_CColorPropPage)
	PROPPAGEID(CLSID_CFontPropPage)
END_PROPPAGEIDS(CSmileCtrl)


/////////////////////////////////////////////////////////////////////////////
// Initialize class factory and guid

IMPLEMENT_OLECREATE_EX(CSmileCtrl, "SMILE.SmileCtrl.1",
	0x175cb003, 0xbeed, 0x11ce, 0x96, 0x11, 0, 0xaa, 0, 0x4a, 0x75, 0xcf)


/////////////////////////////////////////////////////////////////////////////
// Type library ID and version

IMPLEMENT_OLETYPELIB(CSmileCtrl, _tlid, _wVerMajor, _wVerMinor)


/////////////////////////////////////////////////////////////////////////////
// Interface IDs

const IID BASED_CODE IID_DSmile =
		{ 0x175cb001, 0xbeed, 0x11ce, { 0x96, 0x11, 0, 0xaa, 0, 0x4a, 0x75, 0xcf } };
const IID BASED_CODE IID_DSmileEvents =
		{ 0x175cb002, 0xbeed, 0x11ce, { 0x96, 0x11, 0, 0xaa, 0, 0x4a, 0x75, 0xcf } };


/////////////////////////////////////////////////////////////////////////////
// Control type information

static const DWORD BASED_CODE _dwSmileOleMisc =
	OLEMISC_ACTIVATEWHENVISIBLE |
	OLEMISC_SETCLIENTSITEFIRST |
	OLEMISC_INSIDEOUT |
	OLEMISC_CANTLINKINSIDE |
	OLEMISC_RECOMPOSEONRESIZE;

IMPLEMENT_OLECTLTYPE(CSmileCtrl, IDS_Smile, _dwSmileOleMisc)


/////////////////////////////////////////////////////////////////////////////
// CSmileCtrl::CSmileCtrlFactory::UpdateRegistry -
// Adds or removes system registry entries for CSmileCtrl

BOOL CSmileCtrl::CSmileCtrlFactory::UpdateRegistry(BOOL bRegister)
{
	if (bRegister)
		return AfxOleRegisterControlClass(
			AfxGetInstanceHandle(),
			m_clsid,
			m_lpszProgID,
			IDS_Smile,
			IDB_Smile,
			FALSE,                      //  Not insertable
			_dwSmileOleMisc,
			_tlid,
			_wVerMajor,
			_wVerMinor);
	else
		return AfxOleUnregisterClass(m_clsid, m_lpszProgID);
}


/////////////////////////////////////////////////////////////////////////////
// CSmileCtrl::CSmileCtrl - Constructor

CSmileCtrl::CSmileCtrl()
{
	InitializeIIDs(&IID_DSmile, &IID_DSmileEvents);

	m_bWink = FALSE;
}


/////////////////////////////////////////////////////////////////////////////
// CSmileCtrl::~CSmileCtrl - Destructor

CSmileCtrl::~CSmileCtrl()
{
	// TODO: Cleanup your control's instance data here.
}


/////////////////////////////////////////////////////////////////////////////
// CSmileCtrl::OnDraw - Drawing function

#define X(x) (int)(xLeft + (x)*xScale/100)	// Scaling macros
#define Y(y) (int)(yTop + (y)*yScale/100)	// so scale is 0 - 100
#define CX(x) (int)((x)*xScale/100)
#define CY(y) (int)((y)*yScale/100)

void CSmileCtrl::OnDraw(
			CDC* pdc, const CRect& rcBounds, const CRect& rcInvalid)
{
	long xLeft = rcBounds.left;			// Use with scaling macros
	long yTop = rcBounds.top;	
	long xScale = rcBounds.Width();
	long yScale = rcBounds.Height();	

	int iPenWidth = max(CX(5), CY(5)); // Pen width based on control size
	CBrush brushBack;				   // Background brush
	CBrush brushBlack;
	CBrush brushYellow;
	CPen penBlack(PS_SOLID, iPenWidth, RGB(0x00,0x00,0x00));
	CPen penNull(PS_NULL, 0, (COLORREF)0);	// Null pen for drawing filled ellipses
	COLORREF crBack = TranslateColor(GetBackColor());	//Use BackColor
	brushBack.CreateSolidBrush(crBack);                 //Stock Property
	brushBlack.CreateSolidBrush(RGB(0x00,0x00,0x00));
	brushYellow.CreateSolidBrush(RGB(0xff,0xff,0x00));

	pdc->FillRect(rcBounds, &brushBack);				// Clear background

	CPen* pPenSave = pdc->SelectObject(&penBlack);
	CBrush* pBrushSave = pdc->SelectObject(&brushYellow);
	pdc->Ellipse(X(10), Y(15), X(90), Y(95));			// Head

	if (m_bSad)											// Use Sad
	{													// Custom Property
		pdc->Arc(X(25), Y(70), X(75), Y(140),			// Sad Mouth
			   X(65), Y(75), X(35), Y(75));
    } 
	else 
	{
		pdc->Arc(X(25), Y(10), X(75), Y(80),			// Smile mouth
			   X(35), Y(70), X(65), Y(70));
	}


	pdc->SelectObject(&penNull);					    // No draw width
	pdc->SelectObject(&brushBlack);

	if (m_bWink)										// Left Eye
	{
		iPenWidth = max(CX(1), CY(1));
		CPen penThin(PS_SOLID, iPenWidth, RGB(0x00,0x00,0x00));
		CPen* pThick = pdc->SelectObject(&penThin);

		pdc->MoveTo(X(57), Y(35));
		pdc->LineTo(X(65), Y(50));
		pdc->MoveTo(X(57), Y(50));
		pdc->LineTo(X(65), Y(35));
		pdc->SelectObject(pThick);
	}
	else
	{
		pdc->Ellipse(X(57), Y(35), X(65), Y(50));
	}

	pdc->Ellipse(X(35), Y(35), X(43), Y(50));			// Right eye
	pdc->Ellipse(X(46), Y(43), X(54), Y(58));			// Nose
	
	pdc->SetBkMode(TRANSPARENT);						// Use ForeColor
	pdc->SetTextColor(TranslateColor(GetForeColor()));	// Stock Property

	SelectStockFont(pdc);								// Use Font 
														// Stock Property
	CRect rect = rcBounds; 
	pdc->DrawText(InternalGetText(), -1, rect,			// Use Caption 
		DT_SINGLELINE | DT_CENTER | DT_TOP);			// Stock Propery

	pdc->SelectObject(pBrushSave);
	pdc->SelectObject(pPenSave);
}


/////////////////////////////////////////////////////////////////////////////
// CSmileCtrl::DoPropExchange - Persistence support

void CSmileCtrl::DoPropExchange(CPropExchange* pPX)
{
	ExchangeVersion(pPX, MAKELONG(_wVerMinor, _wVerMajor));
	COleControl::DoPropExchange(pPX);

	PX_Bool(pPX, _T("Sad"), m_bSad, FALSE);

}


/////////////////////////////////////////////////////////////////////////////
// CSmileCtrl::OnResetState - Reset control to default state

void CSmileCtrl::OnResetState()
{
	COleControl::OnResetState();  // Resets defaults found in DoPropExchange

	// TODO: Reset any other control state here.
}


/////////////////////////////////////////////////////////////////////////////
// CSmileCtrl::AboutBox - Display an "About" box to the user

void CSmileCtrl::AboutBox()
{
	CDialog dlgAbout(IDD_ABOUTBOX_Smile);
	dlgAbout.DoModal();
}


/////////////////////////////////////////////////////////////////////////////
// CSmileCtrl message handlers

void CSmileCtrl::OnLButtonDown(UINT nFlags, CPoint point) 
{
	CRect rcBounds;
	GetClientRect(&rcBounds);

	long xLeft = rcBounds.left;	//Required by X() and Y() macros
	long yTop = rcBounds.top;	
	long xScale = rcBounds.Width();
	long yScale = rcBounds.Height();

	if (InEllipse(point, X(10), Y(15), X(90), Y(95)))
		FireInside((short)point.x, (short)point.y);
	else
		FireOutside();
	
	COleControl::OnLButtonDown(nFlags, point);
}

//Simple Ellipse Hit-Testing
BOOL CSmileCtrl::InEllipse(const CPoint& pt,
	int x1, int y1, int x2, int y2)
{
	// Determine radii                       
	double a = (x2 - x1) / 2;
	double b = (y2 - y1) / 2;
	
	// Determine x, y
	double x = pt.x - (x1 + x2) / 2;
	double y = pt.y - (y1 + y2) / 2;
	
	// Apply ellipse formula
	return ((x * x) / (a * a) + (y * y) / (b * b) <= 1);
}

void CSmileCtrl::OnSadChanged() 
{
	InvalidateControl();
}

void CSmileCtrl::Beep() 
{
	MessageBeep(0);
}

void CSmileCtrl::Wink(BOOL bWink) 
{
	m_bWink = bWink;
	InvalidateControl();
}
