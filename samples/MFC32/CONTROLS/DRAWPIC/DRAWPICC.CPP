// DrawPicC.cpp : Implementation of the CDrawPicCtrl OLE control class.

// This is a part of the Microsoft Foundation Classes C++ library.
// Copyright (C) 1992-1996 Microsoft Corporation
// All rights reserved.
//
// This source code is only intended as a supplement to the
// Microsoft Foundation Classes Reference and related
// electronic documentation provided with the library.
// See these sources for detailed information regarding the
// Microsoft Foundation Classes product.

#include "stdafx.h"
#include "DrawPic.h"
#include "afxcmn.h"
#include "DrawPicC.h"
#include "DrawPicP.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


IMPLEMENT_DYNCREATE(CDrawPicCtrl, COleControl)


/////////////////////////////////////////////////////////////////////////////
// Message map

BEGIN_MESSAGE_MAP(CDrawPicCtrl, COleControl)
	//{{AFX_MSG_MAP(CDrawPicCtrl)
	// NOTE - ClassWizard will add and remove message map entries
	//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG_MAP
	ON_OLEVERB(AFX_IDS_VERB_PROPERTIES, OnProperties)
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// Dispatch map

BEGIN_DISPATCH_MAP(CDrawPicCtrl, COleControl)
	//{{AFX_DISPATCH_MAP(CDrawPicCtrl)
	DISP_FUNCTION(CDrawPicCtrl, "AddPicture", AddPicture, VT_BOOL, VTS_PICTURE)
	DISP_FUNCTION(CDrawPicCtrl, "ClearImageList", ClearImageList, VT_EMPTY, VTS_NONE)
	//}}AFX_DISPATCH_MAP
	DISP_FUNCTION_ID(CDrawPicCtrl, "AboutBox", DISPID_ABOUTBOX, AboutBox, VT_EMPTY, VTS_NONE)
END_DISPATCH_MAP()


/////////////////////////////////////////////////////////////////////////////
// Event map

BEGIN_EVENT_MAP(CDrawPicCtrl, COleControl)
	//{{AFX_EVENT_MAP(CDrawPicCtrl)
	// NOTE - ClassWizard will add and remove event map entries
	//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_EVENT_MAP
END_EVENT_MAP()


/////////////////////////////////////////////////////////////////////////////
// Property pages

// TODO: Add more property pages as needed.  Remember to increase the count!
BEGIN_PROPPAGEIDS(CDrawPicCtrl, 1)
	PROPPAGEID(CDrawPicPropPage::guid)
END_PROPPAGEIDS(CDrawPicCtrl)


/////////////////////////////////////////////////////////////////////////////
// Initialize class factory and guid

IMPLEMENT_OLECREATE_EX(CDrawPicCtrl, "DRAWPIC.DrawPicCtrl.1",
	0x57f5a423, 0xa324, 0x11cf, 0xb4, 0xa4, 0x44, 0x45, 0x53, 0x54, 0, 0)


/////////////////////////////////////////////////////////////////////////////
// Type library ID and version

IMPLEMENT_OLETYPELIB(CDrawPicCtrl, _tlid, _wVerMajor, _wVerMinor)


/////////////////////////////////////////////////////////////////////////////
// Interface IDs

const IID BASED_CODE IID_DDrawPic =
		{ 0x57f5a421, 0xa324, 0x11cf, { 0xb4, 0xa4, 0x44, 0x45, 0x53, 0x54, 0, 0 } };
const IID BASED_CODE IID_DDrawPicEvents =
		{ 0x57f5a422, 0xa324, 0x11cf, { 0xb4, 0xa4, 0x44, 0x45, 0x53, 0x54, 0, 0 } };


/////////////////////////////////////////////////////////////////////////////
// Control type information

static const DWORD BASED_CODE _dwDrawPicOleMisc =
	OLEMISC_ACTIVATEWHENVISIBLE |
	OLEMISC_SETCLIENTSITEFIRST |
	OLEMISC_INSIDEOUT |
	OLEMISC_CANTLINKINSIDE |
	OLEMISC_RECOMPOSEONRESIZE;

IMPLEMENT_OLECTLTYPE(CDrawPicCtrl, IDS_DRAWPIC, _dwDrawPicOleMisc)


/////////////////////////////////////////////////////////////////////////////
// CDrawPicCtrl::CDrawPicCtrlFactory::UpdateRegistry -
// Adds or removes system registry entries for CDrawPicCtrl

BOOL CDrawPicCtrl::CDrawPicCtrlFactory::UpdateRegistry(BOOL bRegister)
{
	if (bRegister)
		return AfxOleRegisterControlClass(
			AfxGetInstanceHandle(),
			m_clsid,
			m_lpszProgID,
			IDS_DRAWPIC,
			IDB_DRAWPIC,
			FALSE,                      //  Not insertable
			_dwDrawPicOleMisc,
			_tlid,
			_wVerMajor,
			_wVerMinor);
	else
		return AfxOleUnregisterClass(m_clsid, m_lpszProgID);
}


/////////////////////////////////////////////////////////////////////////////
// CDrawPicCtrl::CDrawPicCtrl - Constructor

CDrawPicCtrl::CDrawPicCtrl()
{
	InitializeIIDs(&IID_DDrawPic, &IID_DDrawPicEvents);

	m_ImageList.m_hImageList = NULL;
}


/////////////////////////////////////////////////////////////////////////////
// CDrawPicCtrl::~CDrawPicCtrl - Destructor

CDrawPicCtrl::~CDrawPicCtrl()
{
	// TODO: Cleanup your control's instance data here.
}


/////////////////////////////////////////////////////////////////////////////
// CDrawPicCtrl::OnDraw - Drawing function

void CDrawPicCtrl::OnDraw(
			CDC* pdc, const CRect& rcBounds, const CRect& rcInvalid)
{
    // clear the background
	pdc->FillRect(rcBounds, CBrush::FromHandle((HBRUSH)GetStockObject(WHITE_BRUSH)));

    // make sure we have an imagelist
    if (NULL == m_ImageList.m_hImageList)
    {
        return;
    }

    CPoint ptImagePt(rcBounds.left, rcBounds.top);

    // get number of images in imagelist
    int nImageCount = m_ImageList.GetImageCount();

    // for each image in list
    for (int nImageIndx = 0; nImageIndx < nImageCount; nImageIndx++)
    {
        // calc point to draw it at
        ptImagePt.x = (m_szImageSize.cx * nImageIndx) + rcBounds.left;

        // render the image
        m_ImageList.Draw(pdc, nImageIndx, ptImagePt, ILD_NORMAL);
    }
}


/////////////////////////////////////////////////////////////////////////////
// CDrawPicCtrl::DoPropExchange - Persistence support

void CDrawPicCtrl::DoPropExchange(CPropExchange* pPX)
{
	ExchangeVersion(pPX, MAKELONG(_wVerMinor, _wVerMajor));
	COleControl::DoPropExchange(pPX);

	// TODO: Call PX_ functions for each persistent custom property.

}


/////////////////////////////////////////////////////////////////////////////
// CDrawPicCtrl::OnResetState - Reset control to default state

void CDrawPicCtrl::OnResetState()
{
	COleControl::OnResetState();  // Resets defaults found in DoPropExchange

	// TODO: Reset any other control state here.
}


/////////////////////////////////////////////////////////////////////////////
// CDrawPicCtrl::AboutBox - Display an "About" box to the user

void CDrawPicCtrl::AboutBox()
{
	CDialog dlgAbout(IDD_ABOUTBOX_DRAWPIC);
	dlgAbout.DoModal();
}


/////////////////////////////////////////////////////////////////////////////
// CDrawPicCtrl message handlers

BOOL CDrawPicCtrl::AddPicture(LPPICTUREDISP NewPic) 
{
    BOOL bRetVal = TRUE;

    long lPicHeight = 0;
    long lPicWidth = 0;
    CDC * cdc = GetDC();
    CSize sizePic;
    int nRetVal;
    CPictureHolder NewPicture;

    // put picture into a picture holder
    NewPicture.SetPictureDispatch(NewPic);

    if (NULL == NewPicture.m_pPict)
    {
        return FALSE;
    }

    // if picture is a bitmap
    if (PICTYPE_BITMAP == NewPicture.GetType())
    {
        HBITMAP hBitmap = NULL;

        // get handle of the bitmap
        NewPicture.m_pPict->get_Handle((OLE_HANDLE FAR *) &hBitmap);

        // get dimensions of bitmap
        NewPicture.m_pPict->get_Width(&lPicWidth);
        NewPicture.m_pPict->get_Height(&lPicHeight);

        sizePic.cx = (int)lPicWidth;
        sizePic.cy = (int)lPicHeight;

        // convert coordinates from units to logical units
        cdc->HIMETRICtoLP(&sizePic);

        // if image list has not been created
        if (NULL == m_ImageList.m_hImageList)
        {
            m_szImageSize = sizePic;

            // create the image list
            bRetVal = m_ImageList.Create(m_szImageSize.cx, m_szImageSize.cy, FALSE, 1, 1);
        }
        // else make sure new picture is the same size
        else if (sizePic != m_szImageSize)
        {
            return FALSE;
        }

        // create a temp bitmap
        CBitmap * TempBmp = CBitmap::FromHandle(hBitmap);

        if (bRetVal)
        {
            // add bitmap to imagelist; mask is ignored in this sample
            nRetVal = m_ImageList.Add(TempBmp, RGB(0, 0, 0) ) ;
            bRetVal = (nRetVal != -1);
        }

        // redraw with new picture
        InvalidateControl();
    }
    // else if picture is an icon
    else if (PICTYPE_ICON == NewPicture.GetType())
    {
        HICON hIcon;

        // get handle of the icon
        NewPicture.m_pPict->get_Handle((OLE_HANDLE FAR *) &hIcon);

        // get dimensions of icon
        NewPicture.m_pPict->get_Width(&lPicWidth);
        NewPicture.m_pPict->get_Height(&lPicHeight);

        CDC * cdc = GetDC();

        sizePic.cx = (int)lPicWidth;
        sizePic.cy = (int)lPicHeight;

        // convert coordinates from units to logical units
        cdc->HIMETRICtoLP(&sizePic);

        // if image list has not been created
        if (NULL == m_ImageList.m_hImageList)
        {
            m_szImageSize = sizePic;

            // create the image list
            bRetVal = m_ImageList.Create(m_szImageSize.cx, m_szImageSize.cy, FALSE, 1, 1);
        }
        // else make sure new picture is the same size
        else if (sizePic != m_szImageSize)
        {
            return FALSE;
        }

        if (bRetVal)
        {
            // add icon to image list
            nRetVal = m_ImageList.Add(hIcon);
            bRetVal = (nRetVal != -1);
        }

        InvalidateControl();
    }
    else
    {
        bRetVal = FALSE;
    }

	return (bRetVal);
}

void CDrawPicCtrl::ClearImageList() 
{
    // delete all the images from the list
    m_ImageList.DeleteImageList();
    InvalidateControl();
}
