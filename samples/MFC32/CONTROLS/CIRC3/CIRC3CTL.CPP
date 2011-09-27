// circ3ctl.cpp : Implementation of the CCirc3Ctrl OLE control class.
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
#include "circ3.h"
#include "circ3ctl.h"
#include "circ3ppg.h"


#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif


IMPLEMENT_DYNCREATE(CCirc3Ctrl, COleControl)


/////////////////////////////////////////////////////////////////////////////
// Message map

BEGIN_MESSAGE_MAP(CCirc3Ctrl, COleControl)
	//{{AFX_MSG_MAP(CCirc3Ctrl)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_LBUTTONUP()
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
	ON_OLEVERB(AFX_IDS_VERB_PROPERTIES, OnProperties)
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// Dispatch map

BEGIN_DISPATCH_MAP(CCirc3Ctrl, COleControl)
	//{{AFX_DISPATCH_MAP(CCirc3Ctrl)
	DISP_PROPERTY(CCirc3Ctrl, "FlashColor", m_flashColor, VT_COLOR)
	DISP_PROPERTY_NOTIFY(CCirc3Ctrl, "CircleShape", m_circleShape, OnCircleShapeChanged, VT_BOOL)
	DISP_PROPERTY_EX(CCirc3Ctrl, "CircleOffset", GetCircleOffset, SetCircleOffset, VT_I2)
	DISP_PROPERTY_EX(CCirc3Ctrl, "Note", GetNote, SetNote, VT_BSTR)
	DISP_STOCKPROP_BACKCOLOR()
	DISP_STOCKPROP_CAPTION()
	DISP_STOCKPROP_FONT()
	DISP_STOCKPROP_FORECOLOR()
	//}}AFX_DISPATCH_MAP
	DISP_FUNCTION_ID(CCirc3Ctrl, "AboutBox", DISPID_ABOUTBOX, AboutBox, VT_EMPTY, VTS_NONE)
END_DISPATCH_MAP()


/////////////////////////////////////////////////////////////////////////////
// Event map

BEGIN_EVENT_MAP(CCirc3Ctrl, COleControl)
	//{{AFX_EVENT_MAP(CCirc3Ctrl)
	EVENT_CUSTOM("ClickIn", FireClickIn, VTS_XPOS_PIXELS  VTS_YPOS_PIXELS)
	EVENT_CUSTOM("ClickOut", FireClickOut, VTS_NONE)
	//}}AFX_EVENT_MAP
END_EVENT_MAP()


/////////////////////////////////////////////////////////////////////////////
// Property pages

// TODO: Add more property pages as needed.  Remember to increase the count!
BEGIN_PROPPAGEIDS(CCirc3Ctrl, 3)
	PROPPAGEID(CCirc3PropPage::guid)
	PROPPAGEID(CLSID_CColorPropPage)
	PROPPAGEID(CLSID_CFontPropPage)
END_PROPPAGEIDS(CCirc3Ctrl)


/////////////////////////////////////////////////////////////////////////////
// Initialize class factory and guid

IMPLEMENT_OLECREATE_EX(CCirc3Ctrl, "CIRC3.Circ3Ctrl.1",
	0x9dbafccf, 0x592f, 0x101b, 0x85, 0xce, 0x0, 0x60, 0x8c, 0xec, 0x29, 0x7b)


/////////////////////////////////////////////////////////////////////////////
// Type library ID and version

IMPLEMENT_OLETYPELIB(CCirc3Ctrl, _tlid, _wVerMajor, _wVerMinor)


/////////////////////////////////////////////////////////////////////////////
// Interface IDs

const IID BASED_CODE IID_DCirc3 =
		{ 0x9dbafcd0, 0x592f, 0x101b, { 0x85, 0xce, 0x0, 0x60, 0x8c, 0xec, 0x29, 0x7b } };
const IID BASED_CODE IID_DCirc3Events =
		{ 0x9dbafcd1, 0x592f, 0x101b, { 0x85, 0xce, 0x0, 0x60, 0x8c, 0xec, 0x29, 0x7b } };


/////////////////////////////////////////////////////////////////////////////
// Control type information

static const DWORD BASED_CODE _dwCirc3OleMisc =
	OLEMISC_ACTIVATEWHENVISIBLE |
	OLEMISC_SETCLIENTSITEFIRST |
	OLEMISC_INSIDEOUT |
	OLEMISC_CANTLINKINSIDE |
	OLEMISC_RECOMPOSEONRESIZE;

IMPLEMENT_OLECTLTYPE(CCirc3Ctrl, IDS_CIRC3, _dwCirc3OleMisc)


/////////////////////////////////////////////////////////////////////////////
// CCirc3Ctrl::CCirc3CtrlFactory::UpdateRegistry -
// Adds or removes system registry entries for CCirc3Ctrl

BOOL CCirc3Ctrl::CCirc3CtrlFactory::UpdateRegistry(BOOL bRegister)
{
	if (bRegister)
		return AfxOleRegisterControlClass(
			AfxGetInstanceHandle(),
			m_clsid,
			m_lpszProgID,
			IDS_CIRC3,
			IDB_CIRC3,
			FALSE,                      //  Not insertable
			_dwCirc3OleMisc,
			_tlid,
			_wVerMajor,
			_wVerMinor);
	else
		return AfxOleUnregisterClass(m_clsid, m_lpszProgID);
}


/////////////////////////////////////////////////////////////////////////////
// CCirc3Ctrl::CCirc3Ctrl - Constructor

CCirc3Ctrl::CCirc3Ctrl()
{
	InitializeIIDs(&IID_DCirc3, &IID_DCirc3Events);

	// TODO: Initialize your control's instance data here.
}


/////////////////////////////////////////////////////////////////////////////
// CCirc3Ctrl::~CCirc3Ctrl - Destructor

CCirc3Ctrl::~CCirc3Ctrl()
{
	// TODO: Cleanup your control's instance data here.
}


/////////////////////////////////////////////////////////////////////////////
// CCirc3Ctrl::OnDraw - Drawing function

void CCirc3Ctrl::OnDraw(
			CDC* pdc, const CRect& rcBounds, const CRect&)
{
	CBrush* pOldBrush;
	CBrush bkBrush(TranslateColor(GetBackColor()));
	CPen* pOldPen;
	CRect rc = rcBounds;
	CFont* pOldFont;
	TEXTMETRIC tm;
	const CString& strCaption = InternalGetText();

	// Set the ForeColor property color and transparent background mode into the device context
	pdc->SetTextColor(TranslateColor(GetForeColor()));
	pdc->SetBkMode(TRANSPARENT);

	// Paint the background using the BackColor property
	pdc->FillRect(rcBounds, &bkBrush);

	// Draw the ellipse using the BackColor property and a black pen
	GetDrawRect(&rc);
	pOldBrush = pdc->SelectObject(&bkBrush);
	pOldPen = (CPen*)pdc->SelectStockObject(BLACK_PEN);
	pdc->Ellipse(rc);

	// Draw the caption and note using the stock Font and ForeColor properties
	pOldFont = SelectStockFont(pdc);
	GetStockTextMetrics(&tm);

	pdc->SetTextAlign(TA_CENTER | TA_TOP);
	pdc->ExtTextOut((rc.left + rc.right) / 2, (rc.top + rc.bottom - tm.tmHeight) / 2,
		ETO_CLIPPED, rc, strCaption, strCaption.GetLength(), NULL);
	pdc->SetTextAlign(TA_LEFT | TA_TOP);
	pdc->ExtTextOut(rcBounds.left, rcBounds.top,
		ETO_CLIPPED, rcBounds, m_note, m_note.GetLength(), NULL);
	pdc->SelectObject(pOldFont);

	pdc->SelectObject(pOldPen);
	pdc->SelectObject(pOldBrush);
}


/////////////////////////////////////////////////////////////////////////////
// CCirc3Ctrl::DoPropExchange - Persistence support

void CCirc3Ctrl::DoPropExchange(CPropExchange* pPX)
{
	ExchangeVersion(pPX, MAKELONG(_wVerMinor, _wVerMajor));
	COleControl::DoPropExchange(pPX);

	if (pPX->GetVersion() == (DWORD)MAKELONG(_wVerMinor, _wVerMajor))
	{
		PX_Bool(pPX, _T("CircleShape"), m_circleShape, TRUE);
		PX_Short(pPX, _T("CircleOffset"), m_circleOffset, 0);
		PX_Long(pPX, _T("FlashColor"), (long &)m_flashColor, RGB(0xFF, 0x00, 0x00));
		PX_String(pPX, _T("Note"), m_note, _T(""));
	}
	else if (pPX->IsLoading())
	{
		// Skip over persistent data
		BOOL bDummy;
		short sDummy;
		long lDummy;
		CString strDummy;
		PX_Bool(pPX, _T("CircleShape"), bDummy, FALSE);
		PX_Short(pPX, _T("CircleOffset"), sDummy, 0);
		PX_Long(pPX, _T("FlashColor"), lDummy, 0);
		PX_String(pPX, _T("Note"), strDummy, _T(""));

		// Force property values to these defaults
		m_circleShape = TRUE;
		m_circleOffset = 0;
		m_flashColor = RGB(0xFF, 0x00, 0x00);
		m_note = _T("");
	}
}


/////////////////////////////////////////////////////////////////////////////
// CCirc3Ctrl::OnResetState - Reset control to default state

void CCirc3Ctrl::OnResetState()
{
	COleControl::OnResetState();  // Resets defaults found in DoPropExchange

	// TODO: Reset any other control state here.
}


/////////////////////////////////////////////////////////////////////////////
// CCirc3Ctrl::AboutBox - Display an "About" box to the user

void CCirc3Ctrl::AboutBox()
{
	CDialog dlgAbout(IDD_ABOUTBOX_CIRC3);
	dlgAbout.DoModal();
}


/////////////////////////////////////////////////////////////////////////////
// CCirc3Ctrl message handlers


void CCirc3Ctrl::OnCircleShapeChanged()
{
	SetModifiedFlag();

	// force the control to redraw itself
	InvalidateControl();

	// reset the circle offset, if necessary
	if (m_circleShape)
		SetCircleOffset(0);
}


void CCirc3Ctrl::GetDrawRect(CRect* rc)
{
	if (m_circleShape)
	{
		int cx = rc->right - rc->left;
		int cy = rc->bottom - rc->top;

		if (cx > cy)
		{
			rc->left += (cx - cy) / 2;
			rc->right = rc->left + cy;

			// offset circle in bounding rect
			rc->left += m_circleOffset;
			rc->right += m_circleOffset;
		}
		else
		{
			rc->top += (cy - cx) / 2;
			rc->bottom = rc->top + cx;

			// offset circle in bounding rect
			rc->bottom -= m_circleOffset;
			rc->top -= m_circleOffset;
		}
	}
}


short CCirc3Ctrl::GetCircleOffset()
{
	return m_circleOffset;
}


void CCirc3Ctrl::SetCircleOffset(short nNewValue)
{
	// Validate the specified offset value
	if ((m_circleOffset != nNewValue) && m_circleShape && InBounds(nNewValue))
	{
		m_circleOffset = nNewValue;
		SetModifiedFlag();
		InvalidateControl();
	}
}


BOOL CCirc3Ctrl::InBounds(short nOffset)
{
	int diameter;
	int length;
	int cx;
	int cy;

	GetControlSize(&cx, &cy);

	if (cx > cy)
	{
		length = cx;
		diameter = cy;
	}
	else
	{
		length = cy;
		diameter = cx;
	}
	if (nOffset < 0)
		nOffset = (short) -nOffset;
	return (diameter / 2 + nOffset) <= (length / 2);
}


void CCirc3Ctrl::OnLButtonDown(UINT nFlags, CPoint point)
{
	CDC* pdc;

	// Flash the color of the control if within the ellipse.
	if (InCircle(point))
	{
		pdc = GetDC();
		FlashColor(pdc);
		ReleaseDC(pdc);

		// Fire the ClickIn event
		FireClickIn(point.x, point.y);
	}
	else
		// Fire the ClickOut event
		FireClickOut();

	COleControl::OnLButtonDown(nFlags, point);
}


void CCirc3Ctrl::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	CDC* pdc;

	// Flash the color of the control if within the ellipse.
	if (InCircle(point))
	{
		pdc = GetDC();
		FlashColor(pdc);
		ReleaseDC(pdc);
	}

	COleControl::OnLButtonDblClk(nFlags, point);
}


void CCirc3Ctrl::OnLButtonUp(UINT nFlags, CPoint point)
{
	// Redraw the control.
	if (InCircle(point))
		InvalidateControl();

	COleControl::OnLButtonUp(nFlags, point);
}


BOOL CCirc3Ctrl::InCircle(CPoint& point)
{
	CRect rc;
	GetClientRect(rc);
	GetDrawRect(&rc);

	// Determine radii
	double a = (rc.right - rc.left) / 2;
	double b = (rc.bottom - rc.top) / 2;

	// Determine x, y
	double x = point.x - (rc.left + rc.right) / 2;
	double y = point.y - (rc.top + rc.bottom) / 2;

	// Apply ellipse formula
	return ((x * x) / (a * a) + (y * y) / (b * b) <= 1);
}


void CCirc3Ctrl::FlashColor(CDC* pdc)
{
	CBrush* pOldBrush;
	CBrush flashBrush(TranslateColor(m_flashColor));
	CPen* pOldPen;
	CRect rc;

	GetClientRect(rc);
	GetDrawRect(&rc);
	pOldBrush = pdc->SelectObject(&flashBrush);
	pOldPen = (CPen*)pdc->SelectStockObject(BLACK_PEN);
	pdc->Ellipse(rc);
	pdc->SelectObject(pOldPen);
	pdc->SelectObject(pOldBrush);
}


BOOL CCirc3Ctrl::OnMapPropertyToPage(DISPID dispid, LPCLSID lpclsid,
	BOOL* pbPageOptional)
{
	switch(dispid)
	{
		case dispidFlashColor:
			*lpclsid = CLSID_CColorPropPage;
			*pbPageOptional = TRUE;
			return TRUE;
	}
	return COleControl::OnMapPropertyToPage(dispid, lpclsid, pbPageOptional);
}


BSTR CCirc3Ctrl::GetNote()
{
	return m_note.AllocSysString();
}


void CCirc3Ctrl::SetNote(LPCTSTR lpszNewValue)
{
	if (m_note != lpszNewValue)
	{
		m_note = lpszNewValue;
		SetModifiedFlag();
		InvalidateControl();
		BoundPropertyChanged(dispidNote);
	}
}

void CCirc3Ctrl::OnSize(UINT nType, int cx, int cy) 
{
	COleControl::OnSize(nType, cx, cy);
	
	// If circle shape is true & cicrle does not fit in new size, reset the offset
	if (m_circleShape && !InBounds(GetCircleOffset()))
		SetCircleOffset(0);
	
}
