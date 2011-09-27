/*** 
*statbar.cpp
*
*  This is a part of the Microsoft Source Code Samples.
*
*  Copyright (C) 1992-1995 Microsoft Corporation. All rights reserved.
*
*  This source code is only intended as a supplement to Microsoft Development
*  Tools and/or WinHelp documentation.  See these sources for detailed
*  information regarding the Microsoft samples programs.
*
*Purpose:
*
*Implementation Notes:
*
*****************************************************************************/

#include <stdarg.h>

#include "hostenv.h"
#include "statbar.h"


extern "C" long FAR PASCAL StatBarWndProc(HWND, unsigned int, WPARAM, LPARAM);


TCHAR FAR* CStatBar::m_szWndClass = TSTR("StatBarWndClass");


CStatBar::CStatBar()
{
    m_refs = 0;

    m_x = 0;
    m_y = 0;
    m_width = 0;
    m_height = 0;

    m_bstrMsg = NULL;

    m_hfont = (HFONT)0;
}

CStatBar::~CStatBar()
{
    SysFreeString(m_bstrMsg);
}


/***
*PUBLIC CStatBar FAR* CStatBar::Create(HINSTANCE, HWND)
*
*Purpose:
*
*Entry:
*
*Exit:
*
***********************************************************************/
CStatBar FAR*
CStatBar::Create(HINSTANCE hinst, HWND hwndFrame)
{
    CStatBar FAR* psb;

    psb = new FAR CStatBar();
    if(psb == NULL)
      return NULL;
    psb->AddRef();

    if(!psb->Register(hinst))
      goto LFail;

    psb->m_hwnd = CreateWindow(
      CStatBar::m_szWndClass,
      NULL,
      WS_CHILD | WS_CLIPSIBLINGS,
      0, 0, 0, 0,
      hwndFrame,
      0,
      hinst,
      NULL);

    if(!psb->m_hwnd)
      goto LFail;

    // Stash the newly created CStatBar* in the extra bytes of the
    // associated window so we can get at the instance in the message
    // proc.
    //
    // Note: we do not AddRef for this reference. We make sure that the
    // window is destroyed when the refcnt goes to 0.
    //
    SetWindowLong(psb->m_hwnd, 0, (long)psb);

    return psb;

LFail:;
    delete psb;
    return NULL;
}


//---------------------------------------------------------------------
//                     IUnknown Methods
//---------------------------------------------------------------------


STDMETHODIMP
CStatBar::QueryInterface(REFIID riid, void FAR* FAR* ppv)
{
    if(IsEqualIID(riid,IID_IUnknown)){
      *ppv = this;
      AddRef();
      return NOERROR;
    }
    *ppv = (void FAR*)NULL;
    return ResultFromScode(E_NOINTERFACE);
}


STDMETHODIMP_(unsigned long)
CStatBar::AddRef(void)
{
    return ++m_refs;
}


STDMETHODIMP_(unsigned long)
CStatBar::Release(void)
{
    if(--m_refs == 0){

      // destroy the status bar window.
      //
      SendMessage(m_hwnd, WM_DESTROY, 0, 0L);

      delete this;
      return 0;
    }

    return m_refs;
}


//---------------------------------------------------------------------
//                     Introduced Methods
//---------------------------------------------------------------------


/***
*PRIVATE BOOL CStatBar::Register(HINSTANCE)
*
*Purpose:
*  Register the status bar window class.
*
*Entry:
*  None
*
*Exit:
*  return value = BOOL, TRUE if successful, FALSE if not.
*
***********************************************************************/
BOOL
CStatBar::Register(HINSTANCE hinst)
{
    WNDCLASS  wc;

    // register the class, unless already registered.
    if(GetClassInfo(hinst, m_szWndClass, &wc) == 0){
      wc.style         = 0;
      wc.lpfnWndProc   = StatBarWndProc;
      wc.cbClsExtra    = 0;
      wc.cbWndExtra    = sizeof(CStatBar FAR*);
      wc.hInstance     = hinst;
      wc.hIcon         = 0;
      wc.hCursor       = LoadCursor(NULL, IDC_ARROW);
      wc.hbrBackground = (HBRUSH)GetStockObject(LTGRAY_BRUSH); 
      wc.lpszMenuName  = 0;
      wc.lpszClassName = CStatBar::m_szWndClass;
      if(!RegisterClass(&wc))
        return FALSE;
    }
    return TRUE;
}


/***
*PUBLIC void CStatBar::Show(void)
*
*Purpose:
*  Show the status bar window associated with this CStatBar instance.
*
*Entry:
*  None
*
*Exit:
*  None
*
***********************************************************************/
void
CStatBar::Show()
{
    ShowWindow(m_hwnd, SW_SHOW);
}

void
CStatBar::SetFont(HFONT hfont)
{
    HDC hdc;
    TEXTMETRIC tm;
    HFONT hfontOld;

    // compute the character sizes given this new font.
    //
    hdc = GetDC(m_hwnd);
    hfontOld = (HFONT)SelectObject(hdc, hfont);
    GetTextMetrics(hdc, &tm);
    m_dxFont = tm.tmAveCharWidth;
    m_dyFont = tm.tmHeight + tm.tmExternalLeading;
    SelectObject(hdc, hfontOld);
    ReleaseDC(m_hwnd, hdc);

    m_hfont = hfont;
}

/***
*PRIVATE CStatBar::WMPaint(void)
*
*Purpose:
*  This method is responsible for drawing the status bar, and is called
*  in response to a WM_PAINT message.
*
*Entry:
*  None
*
*Exit:
*  None
*
***********************************************************************/
void
CStatBar::WMPaint()
{
    HDC hdc;
    RECT rcMsg;
    HRGN hrgn;
    HFONT hfontOld;
    PAINTSTRUCT ps;
    HPEN hpenBlack, hpenWhite, hpenGray, hpenOld;

    hdc = BeginPaint(m_hwnd, &ps);

    // compute the message box rect
    //
    rcMsg.top	= 3;
    rcMsg.bottom= m_height - 3;
    rcMsg.left	= m_dxFont;
    rcMsg.right	= m_width - m_dxFont;

    // prepare the pens
    //
    hpenWhite	= (HPEN)GetStockObject(WHITE_PEN);
    hpenBlack	= (HPEN)GetStockObject(BLACK_PEN);
    hpenGray	= CreatePen(PS_SOLID, 1, GetSysColor(COLOR_BTNSHADOW));

    // draw a top gray line
    //
    hpenOld = (HPEN)SelectObject(hdc, hpenGray);
#if   defined(WIN16)	
    MoveTo(hdc, ps.rcPaint.left, 0);
#elif defined(WIN32)	
    MoveToEx(hdc, ps.rcPaint.left, 0, NULL);
#endif
    LineTo(hdc, ps.rcPaint.right, 0);

    // draw a white line just under
    //
    SelectObject(hdc, hpenWhite);
#if   defined(WIN16)	
    MoveTo(hdc, ps.rcPaint.left, 1);
#elif defined(WIN32)	
    MoveToEx(hdc, ps.rcPaint.left, 1, NULL);
#endif
    LineTo(hdc, ps.rcPaint.right, 1);

    // do not overwrite the background color
    //
    SetBkMode(hdc, TRANSPARENT);

    // message area
    //
    SelectObject(hdc, hpenBlack);
#if   defined(WIN16)	
    MoveTo(hdc, rcMsg.left,  rcMsg.bottom);
#elif defined(WIN32)	
    MoveToEx(hdc, rcMsg.left,  rcMsg.bottom, NULL);
#endif	
    LineTo(hdc, rcMsg.left,  rcMsg.top);
    LineTo(hdc, rcMsg.right, rcMsg.top);

    SelectObject(hdc, hpenWhite);
    LineTo(hdc, rcMsg.right, rcMsg.bottom);
    LineTo(hdc, rcMsg.left,  rcMsg.bottom);

    // select the black pen for writing
    //
    SelectObject(hdc, hpenBlack);

    // select the status bar font to write in
    //
    hfontOld = (HFONT)SelectObject(hdc, m_hfont);

    // set the clipping region
    //
    hrgn = CreateRectRgn(
      rcMsg.left, rcMsg.top, rcMsg.right, rcMsg.bottom);

    SelectClipRgn(hdc, hrgn);

    // draw the status message
    //
    TextOut(
      hdc,
      rcMsg.left + (m_dxFont / 2),
      rcMsg.top + ((rcMsg.bottom - rcMsg.top - m_dyFont) / 2),
      STRING(m_bstrMsg), (SysStringLen(m_bstrMsg)));

    // cleanup
    //
    SelectObject(hdc, hpenOld);
    SelectObject(hdc, hfontOld);

    DeleteObject(hrgn);
    DeleteObject(hpenGray);

    EndPaint(m_hwnd, &ps);
}

extern "C" long FAR PASCAL
StatBarWndProc(
    HWND hwnd,
    unsigned int message,
    WPARAM wParam,
    LPARAM lParam)
{
    CStatBar FAR* psb;

    switch(message){
    case WM_SIZE:
      return 0;
    case WM_PAINT:
      psb = (CStatBar FAR*)GetWindowLong(hwnd, 0);
      psb->WMPaint();
      return 0;
    }
    return(DefWindowProc(hwnd, message, wParam, lParam));
}


//---------------------------------------------------------------------
//                    Status Bar Utilities
//---------------------------------------------------------------------

extern "C" void
SBprintf(CStatBar FAR* psb, TCHAR FAR* szFmt, ...)
{
    va_list args;
static TCHAR buf[256];

    va_start(args, szFmt);
    wvsprintf(buf, szFmt, args);
    psb->SetText(WIDESTRING(buf));
    psb->Update();
}
