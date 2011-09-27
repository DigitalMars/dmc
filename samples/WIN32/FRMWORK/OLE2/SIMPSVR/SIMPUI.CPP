/*
 * SIMPUI.CPP
 *
 * Miscellaneous API's from the OLE 2.0 User Interface Support
 * Library sample code.  All code below comes from OBJFDBK.C,
 * HATCH.C, and OLEUTL.C.
 *
 * Copyright (c) 1995 Microsoft Corporation, All Right Reserved
 */

#include "precomp.h"
#include "simpui.h"


// Adapted from OBJFDBK.C

// DPa - combimes the Destination and Pattern (brush) with logical AND
#define ropDPa  0x00A000C9L


/*
 * OleUIDrawHandles
 *
 * Purpose:
 *  Draw handles or/and boundary around Container Object when selected
 *
 * Parameters:
 *  lpRect      Dimensions of Container Object
 *  hdc         HDC of Container Object (MM_TEXT mapping mode)
 *  dwFlags-
 *      Exclusive flags
 *          OLEUI_HANDLES_INSIDE    Draw handles on inside of rect
 *          OLEUI_HANDLES_OUTSIDE   Draw handles on outside of rect
 *      Optional flags
 *          OLEUI_HANDLES_NOBORDER  Draw handles only, no rect
 *          OLEUI_HANDLES_USEINVERSE
 *              use invert for handles and rect, o.t. use COLOR_WINDOWTEXT
 *  cSize       size of handle box
 *  fDraw       Draw if TRUE, erase if FALSE
 *
 * Return Value: null
 *
 */
STDAPI_(void) OleUIDrawHandles(LPRECT  lpRect,
                               HDC     hdc,
                               DWORD   dwFlags,
                               UINT    cSize,
                               BOOL    fDraw)
{
    RECT rc = *lpRect;

    if (dwFlags & OLEUI_HANDLES_OUTSIDE)
        InflateRect(&rc, cSize - 1, cSize - 1);

    DWORD dwRop = ((dwFlags & OLEUI_HANDLES_USEINVERSE) ? DSTINVERT
                   : (fDraw ? BLACKNESS : WHITENESS));

    // Draw the handles inside the rectangle boundary
    PatBlt(hdc, rc.left, rc.top, cSize, cSize, dwRop);
    PatBlt(hdc, rc.left, (rc.top + rc.bottom - cSize) / 2, cSize, cSize, dwRop);
    PatBlt(hdc, rc.left, rc.bottom - cSize, cSize, cSize, dwRop);
    PatBlt(hdc, (rc.right + rc.left - cSize) / 2, rc.top, cSize, cSize, dwRop);
    PatBlt(hdc, (rc.right + rc.left - cSize) / 2, rc.bottom - cSize, cSize, cSize, dwRop);
    PatBlt(hdc, rc.right - cSize, rc.top, cSize, cSize, dwRop);
    PatBlt(hdc, rc.right - cSize, (rc.top + rc.bottom - cSize) / 2, cSize, cSize, dwRop);
    PatBlt(hdc, rc.right - cSize, rc.bottom - cSize, cSize, cSize, dwRop);

    if (!(dwFlags & OLEUI_HANDLES_NOBORDER))
    {
        rc = *lpRect;
        rc.right--;
        rc.bottom--;

        PatBlt(hdc, rc.left, rc.top, rc.right - rc.left, 1, dwRop);
        PatBlt(hdc, rc.left, rc.top, 1, rc.bottom - rc.top, dwRop);
        PatBlt(hdc, rc.right, rc.top, 1, rc.bottom - rc.top, dwRop);
        PatBlt(hdc, rc.left, rc.bottom, rc.right - rc.left, 1, dwRop);
    }
}


/*
 * OleUIDrawShading
 *
 * Purpose:
 *  Shade the object when it is in in-place editing. Borders are drawn
 *  on the Object rectangle. The right and bottom edge of the rectangle
 *  are excluded in the drawing.
 *
 * Parameters:
 *  lpRect      Dimensions of Container Object
 *  hdc         HDC for drawing
 *  dwFlags-
 *      Exclusive flags
 *          OLEUI_SHADE_FULLRECT    Shade the whole rectangle
 *          OLEUI_SHADE_BORDERIN    Shade cWidth pixels inside rect
 *          OLEUI_SHADE_BORDEROUT   Shade cWidth pixels outside rect
 *      Optional flags
 *          OLEUI_SHADE_USEINVERSE
 *              use PATINVERT instead of the hex value
 *  cWidth      width of border in pixel
 *
 * Return Value: null
 *
 */
STDAPI_(void) OleUIDrawShading(LPRECT lpRect, HDC hdc, DWORD dwFlags, UINT cWidth)
{
    WORD wHatchBmp[] = {0x11, 0x22, 0x44, 0x88, 0x11, 0x22, 0x44, 0x88};

    HBITMAP hbm = CreateBitmap(8, 8, 1, 1, wHatchBmp);
    HBRUSH  hbr = CreatePatternBrush(hbm);
    HBRUSH  hbrOld = (HBRUSH)SelectObject(hdc, hbr);

    RECT rc = *lpRect;

    COLORREF cvText = SetTextColor(hdc, RGB(255, 255, 255));
    COLORREF cvBk = SetBkColor(hdc, RGB(0, 0, 0));

    if (dwFlags == OLEUI_SHADE_FULLRECT)
        PatBlt(hdc, rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top, ropDPa);
    else
    {    // either inside or outside rect

        if (dwFlags == OLEUI_SHADE_BORDEROUT)
            InflateRect(&rc, cWidth - 1, cWidth - 1);

        PatBlt(hdc, rc.left, rc.top, rc.right - rc.left, cWidth, ropDPa);
        PatBlt(hdc, rc.left, rc.top, cWidth, rc.bottom - rc.top, ropDPa);
        PatBlt(hdc, rc.right - cWidth, rc.top, cWidth, rc.bottom - rc.top, ropDPa);
        PatBlt(hdc, rc.left, rc.bottom - cWidth, rc.right - rc.left, cWidth, ropDPa);
    }

    SetTextColor(hdc, cvText);
    SetBkColor(hdc, cvBk);
    SelectObject(hdc, hbrOld);
    DeleteObject(hbr);
    DeleteObject(hbm);
}


/*
 * OleUIShowObject
 *
 * Purpose:
 *  Draw the ShowObject effect around the object
 *
 * Parameters:
 *  lprc        rectangle for drawing
 *  hdc         HDC for drawing
 *  fIsLink     linked object (TRUE) or embedding object (FALSE)
 *
 * Return Value: null
 *
 */
STDAPI_(void) OleUIShowObject(LPCRECT lprc, HDC hdc, BOOL fIsLink)
{
    HPEN    hpen;
    HPEN    hpenOld;
    HBRUSH  hbrOld;

    if (!lprc || !hdc)
        return;

    hpen = fIsLink ? CreatePen(PS_DASH, 1, RGB(0,0,0)) :
                     (HPEN)GetStockObject(BLACK_PEN);

    if (!hpen)
        return;

    hpenOld = (HPEN)  SelectObject(hdc, hpen);
    hbrOld  = (HBRUSH)SelectObject(hdc, (HBRUSH)GetStockObject(NULL_BRUSH));

    Rectangle(hdc, lprc->left, lprc->top, lprc->right, lprc->bottom);

    SelectObject(hdc, hpenOld);
    SelectObject(hdc, hbrOld);

    if (fIsLink)
        DeleteObject(hpen);
}



// Adapted from HATCH.C

// offsets in the extra bytes stored with the hatch window
#define EB_HATCHWIDTH       0
#define EB_HATCHRECT_LEFT   2
#define EB_HATCHRECT_TOP    4
#define EB_HATCHRECT_RIGHT  6
#define EB_HATCHRECT_BOTTOM 8

// class name of hatch window
#define CLASS_HATCH     TEXT("Hatch Window")

// local function prototypes
LRESULT CALLBACK HatchWndProc(HWND hWnd, UINT Message, WPARAM wParam, LPARAM lParam);


/*
 * RegisterHatchWindowClass
 *
 * Purpose:
 *  Register the hatch window
 *
 * Parameters:
 *  hInst           Process instance
 *
 * Return Value:
 *  TRUE            if successful
 *  FALSE           if failed
 *
 */
STDAPI_(BOOL) RegisterHatchWindowClass(HINSTANCE hInst)
{
    WNDCLASS wc;

    // Register Hatch Window Class
    wc.style = CS_BYTEALIGNWINDOW;
    wc.lpfnWndProc = HatchWndProc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 5 * sizeof(WORD);   // extra bytes stores
                                        //     uHatchWidth
                                        //     rcHatchRect
    wc.hInstance = hInst;
    wc.hIcon = NULL;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wc.lpszMenuName = NULL;
    wc.lpszClassName = CLASS_HATCH;

    return (BOOL)RegisterClass(&wc);
}


/*
 * CreateHatchWindow
 *
 * Purpose:
 *  Create the hatch window
 *
 * Parameters:
 *  hWndParent          parent of hatch window
 *  hInst               instance handle
 *
 * Return Value:
 *  pointer to hatch window         if successful
 *  NULL                            if failed
 *
 */
STDAPI_(HWND) CreateHatchWindow(HWND hWndParent, HINSTANCE hInst)
{
    if (!hWndParent || !hInst)
        return NULL;

    return CreateWindow(CLASS_HATCH,
                        TEXT("Hatch Window"),
                        WS_CHILDWINDOW | WS_CLIPCHILDREN | WS_CLIPSIBLINGS,
                        0, 0, 0, 0,
                        hWndParent,
                        (HMENU)NULL,
                        hInst,
                        0L);
}

/*
 *  GetHatchWidth
 *
 *  Purpose:
 *      Get width of hatch border
 *
 *  Parameters:
 *      hWndHatch       hatch window handle
 *
 *  Return Value:
 *      width of the hatch border
 */
STDAPI_(UINT) GetHatchWidth(HWND hWndHatch)
{
    if (!IsWindow(hWndHatch))
        return 0;

    return (UINT)GetWindowWord(hWndHatch, EB_HATCHWIDTH);
}

/*
 *  GetHatchRect
 *
 *  Purpose:
 *      Get hatch rect. this is the size of the hatch window if it were
 *      not clipped by the ClipRect.
 *
 *  Parameters:
 *      hWndHatch       hatch window handle
 *      lprcHatchRect   hatch rect
 *
 *  Return Value:
 *      none
 */
STDAPI_(void) GetHatchRect(HWND hWndHatch, LPRECT lprcHatchRect)
{
    if (!IsWindow(hWndHatch))
    {
        SetRect(lprcHatchRect, 0, 0, 0, 0);
        return;
    }

    lprcHatchRect->left   = (int)(short)GetWindowWord(hWndHatch, EB_HATCHRECT_LEFT);
    lprcHatchRect->top    = (int)(short)GetWindowWord(hWndHatch, EB_HATCHRECT_TOP);
    lprcHatchRect->right  = (int)(short)GetWindowWord(hWndHatch, EB_HATCHRECT_RIGHT);
    lprcHatchRect->bottom = (int)(short)GetWindowWord(hWndHatch, EB_HATCHRECT_BOTTOM);
}


/* SetHatchRect
 *
 *
 *  Purpose:
 *      Store hatch rect with HatchRect window.
 *      this rect is the size of the hatch window if it were
 *      not clipped by the ClipRect.
 *
 *  Parameters:
 *      hWndHatch       hatch window handle
 *      lprcHatchRect   hatch rect
 *
 *  Return Value:
 *      none
 */
STDAPI_(void) SetHatchRect(HWND hWndHatch, LPRECT lprcHatchRect)
{
    if (!IsWindow(hWndHatch))
        return;

    SetWindowWord(hWndHatch, EB_HATCHRECT_LEFT,  (WORD)(short)lprcHatchRect->left);
    SetWindowWord(hWndHatch, EB_HATCHRECT_TOP,   (WORD)(short)lprcHatchRect->top);
    SetWindowWord(hWndHatch, EB_HATCHRECT_RIGHT, (WORD)(short)lprcHatchRect->right);
    SetWindowWord(hWndHatch, EB_HATCHRECT_BOTTOM,(WORD)(short)lprcHatchRect->bottom);
}


/* SetHatchWindowSize
 *
 *
 *  Purpose:
 *      Move/size the HatchWindow correctly given the rect required by the
 *      in-place server object window and the lprcClipRect imposed by the
 *      in-place container. both rect's are expressed in the client coord.
 *      of the in-place container's window (which is the parent of the
 *      HatchWindow).
 *
 *      OLE2NOTE: the in-place server must honor the lprcClipRect specified
 *      by its in-place container. it must NOT draw outside of the ClipRect.
 *      In order to achieve this, the hatch window is sized to be
 *      exactly the size that should be visible (rcVisRect). the
 *      rcVisRect is defined as the intersection of the full size of
 *      the HatchRect window and the lprcClipRect.
 *      the ClipRect could infact clip the HatchRect on the
 *      right/bottom and/or on the top/left. if it is clipped on the
 *      right/bottom then it is sufficient to simply resize the hatch
 *      window. but if the HatchRect is clipped on the top/left then
 *      in-place server document window (child of HatchWindow) must be moved
 *      by the delta that was clipped. The window origin of the
 *      in-place server document window will then have negative coordinates
 *      relative to its parent HatchWindow.
 *
 *  Parameters:
 *      hWndHatch       hatch window handle
 *      lprcIPObjRect   full size of in-place server object window
 *      lprcClipRect    clipping rect imposed by in-place container
 *      lpptOffset      offset required to position in-place server object
 *                      window properly. caller should call:
 *                          OffsetRect(&rcObjRect,lpptOffset->x,lpptOffset->y)
 *
 *  Return Value:
 *      none
 */
STDAPI_(void) SetHatchWindowSize(HWND    hWndHatch,
                                 LPRECT  lprcIPObjRect,
                                 LPRECT  lprcClipRect,
                                 LPPOINT lpptOffset)
{
    RECT  rcHatchRect;
    RECT  rcVisRect;
    UINT  uHatchWidth;

    if (!IsWindow(hWndHatch))
        return;

    rcHatchRect = *lprcIPObjRect;
    uHatchWidth = GetHatchWidth(hWndHatch);
    InflateRect(&rcHatchRect, uHatchWidth + 1, uHatchWidth + 1);

    IntersectRect(&rcVisRect, &rcHatchRect, lprcClipRect);
    MoveWindow(hWndHatch,
               rcVisRect.left,
               rcVisRect.top,
               rcVisRect.right-rcVisRect.left,
               rcVisRect.bottom-rcVisRect.top,
               TRUE /* fRepaint */ );
    InvalidateRect(hWndHatch, NULL, TRUE);

    // calculate offset required to position server windows
    lpptOffset->x = -rcVisRect.left;
    lpptOffset->y = -rcVisRect.top;

    // Convert the rcHatchRect into the client coordinate system of the
    // HatchWindow itself and save the new rect.
    OffsetRect(&rcHatchRect, lpptOffset->x, lpptOffset->y);
    SetHatchRect(hWndHatch, &rcHatchRect);
}


/*
 *  HatchWndProc
 *
 *  Purpose:
 *      WndProc for hatch window
 *
 *  Parameters:
 *      hWnd
 *      Message
 *      wParam
 *      lParam
 *
 *  Return Value:
 *      message dependent
 */
LRESULT CALLBACK HatchWndProc(HWND hWnd, UINT Message, WPARAM wParam, LPARAM lParam)
{
    int nBorderWidth;

    switch (Message)
    {
        case WM_CREATE:
            nBorderWidth = GetProfileInt(TEXT("windows"),
                                         TEXT("oleinplaceborderwidth"),
                                         DEFAULT_HATCHBORDER_WIDTH);
            SetWindowWord(hWnd, EB_HATCHWIDTH, (WORD)nBorderWidth);
            break;

        case WM_PAINT:
        {
            PAINTSTRUCT ps;
            RECT rcHatchRect;

            BeginPaint(hWnd, &ps);
            nBorderWidth = GetHatchWidth(hWnd);
            GetHatchRect(hWnd, &rcHatchRect);
            OleUIDrawShading(&rcHatchRect,
                             ps.hdc,
                             OLEUI_SHADE_BORDERIN,
                             nBorderWidth);
            InflateRect(&rcHatchRect, -nBorderWidth, -nBorderWidth);
            OleUIDrawHandles(&rcHatchRect,
                             ps.hdc,
                             OLEUI_HANDLES_OUTSIDE,
                             nBorderWidth + 1,
                             TRUE);
            EndPaint(hWnd, &ps);
            break;
        }

        /* OLE2NOTE: Any window that is used during in-place activation
        **    must handle the WM_SETCURSOR message or else the cursor
        **    of the in-place parent will be used. if WM_SETCURSOR is
        **    not handled, then DefWindowProc sends the message to the
        **    window's parent.
        */
        case WM_SETCURSOR:
        {
            HCURSOR hcur = (HCURSOR)GetClassLong(hWnd, GCL_HCURSOR);
            if (NULL == hcur)
                hcur = LoadCursor(NULL, IDC_ARROW);
            SetCursor(hcur);
            return (LRESULT)TRUE;
        }

        default:
            return DefWindowProc(hWnd, Message, wParam, lParam);
    }

    return 0L;
}



// Adapted from OLEUTL.C

#include <ctype.h>

/* GetWord
 * -------
 *
 * LPSTR lpszSrc - Pointer to a source string
 * LPSTR lpszDst - Pointer to destination buffer
 *
 * Will copy one space-terminated or null-terminated word from the source
 * string to the destination buffer.
 * returns: pointer to next character following the word.
 *
 * This function works on ANSI strings only.
 *
 */
static LPSTR GetWord(LPSTR lpszSrc, LPSTR lpszDst)
{
    while (*lpszSrc && !isspace(*lpszSrc))
        *lpszDst++ = *lpszSrc++;

    *lpszDst = '\0';
    return lpszSrc;
}


/*
 * ParseCmdLine
 *
 * Parses the Windows command line which was passed to WinMain.
 * This function determines if the -Embedding switch has been given.
 *
 * Always assumes lpszLine is ANSI (from WinMain) but treats
 * szFileName as either ANSI or Unicode depending on the UNICODE symbol
 *
 */

STDAPI_(void) ParseCmdLine(LPSTR  lpszLine,
                           LPBOOL lpfEmbedFlag,
                           LPTSTR szFileName,
                           UINT   cchFileName)  // sizeof szFileName in characters
{
    if(lpfEmbedFlag)
        *lpfEmbedFlag = FALSE;

    if (szFileName && cchFileName > 0)
        szFileName[0] = '\0';         // NULL string

    if (!lpszLine)
        return;

    // skip blanks
    while(isspace(*lpszLine))
        lpszLine++;

    if(!*lpszLine)      // No filename or options, so start a fresh document.
        return;

    // Check for "-Embedding" or "/Embedding" and set fEmbedding.
    if(lpfEmbedFlag && (*lpszLine == '-' || *lpszLine == '/'))
    {
        char szBuf[256];

        lpszLine++;
        lpszLine = GetWord(lpszLine, szBuf);
        *lpfEmbedFlag = !lstrcmpiA(szBuf, EMBEDDINGFLAG);
    }

    // skip blanks
    while(isspace(*lpszLine))
        lpszLine++;

    // copy file name argument
    if (*lpszLine && szFileName && cchFileName > 0)
    {
#ifdef UNICODE      // copy and convert to Unicode at same time

        MultiByteToWideChar(CP_ACP,
                            MB_PRECOMPOSED,
                            lpszLine,
                            -1,
                            szFileName,
                            cchFileName);

#else               // ANSI, just copy

        lstrcpyn(szFileName, lpszLine, cchFileName - 1);

#endif
    }
}
