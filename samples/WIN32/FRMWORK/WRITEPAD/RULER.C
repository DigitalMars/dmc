// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
// PARTICULAR PURPOSE.
//
// Copyright (C) 1993-1995  Microsoft Corporation.  All Rights Reserved.
//
//  MODULE:   ruler.c
//
//  PURPOSE:   
//
//  FUNCTIONS:
//    Ruler           - Processes messages for ruler box
//
//  COMMENTS:
//
//
 
#include <windows.h>            // required for all Windows applications
#include <windowsx.h>
#include <malloc.h>
#include <commctrl.h>
#include "globals.h"            // prototypes specific to this application
#include "ruler.h"            // prototypes specific to this application

// Ruler dialog message table definition.
MSD rgmsdRuler[] =
{
    {WM_CREATE,             MsgRulerCreate},
    {WM_DESTROY,            MsgRulerDestroy},
    {WM_PAINT,              MsgRulerPaint},
    {WM_LBUTTONDOWN,        MsgRulerLButtonDown},
    {WM_LBUTTONUP,          MsgRulerLButtonUp},
    {WM_MOUSEMOVE,          MsgRulerMouseMove},
    {WM_ERASEBKGND,         MsgRulerEraseBkgnd},
    {RM_GETMARGINS,         MsgRulerGetMargins},
    {RM_SETMARGINS,         MsgRulerSetMargins},
    {RM_GETTABCOUNT,        MsgRulerGetTabCount},
    {RM_GETTABS,            MsgRulerGetTabs},
    {RM_SETTABS,            MsgRulerSetTabs}
};

MSDI msdiRuler =
{
    sizeof(rgmsdRuler) / sizeof(MSD),
    rgmsdRuler,
    edwpWindow
};


// Module specific "globals"  Used when a variable needs to be
// accessed in more than on handler function.

char szRulerName[] = SZRULERNAME;

typedef struct tagRULER
    {
    UINT    RULER_iLeftMargin ;   // All measurements in Twips
    UINT    RULER_iRightMargin;
    UINT    RULER_iIndent     ;
    UINT    RULER_iLeftPage   ;
    UINT    RULER_iRightPage  ;
    LONG    RULER_lTabs[RULER_MAXTABS];
    }
RULER, FAR * LPRULER;

LPRULER    lpRuler;
HIMAGELIST hRulerImageList;
UINT       iRulerCount;
BOOL       bDragging;
int        iWhoToDrag;
int        dyHotSpot;

// Image list constants

#define RULER_LEFTMARGIN  0
#define RULER_RIGHTMARGIN 1
#define RULER_INDENT      2
#define RULER_LEFTTAB     3

// These macros are used to get at the ruler instance specific data

#define GETRULERPOINTER lpRuler = (LPRULER)GetWindowLong(hwnd,0)
#define SETRULERPOINTER(lpRuler)  SetWindowLong(hwnd, 0, (LONG)lpRuler)

#define iLeftMargin  (lpRuler->RULER_iLeftMargin )
#define iRightMargin (lpRuler->RULER_iRightMargin)
#define iIndent      (lpRuler->RULER_iIndent     )
#define iLeftPage    (lpRuler->RULER_iLeftPage   )
#define iRightPage   (lpRuler->RULER_iRightPage  )
#define lTabs        (lpRuler->RULER_lTabs       )

//
//  FUNCTION: Ruler(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for "Ruler" dialog box.
//
//  PARAMETERS:
//    hdlg - window handle of the dialog box
//    wMessage - type of message
//    wparam - message-specific information
//    lparam - message-specific information
//
//  RETURN VALUE:
//    TRUE - message handled
//    FALSE - message not handled
//
//  COMMENTS:
//
//     Display version information from the version section of the
//     application resource.
//
//     Wait for user to click on "Ok" button, then close the dialog box.
//

LRESULT CALLBACK Ruler(HWND hwnd, UINT uMessage, WPARAM wparam, LPARAM lparam)
{
    return DispMessage(&msdiRuler, hwnd, uMessage, wparam, lparam);
}

HWND CreateRuler(HWND hwndParent, UINT uID, int dx, int dy)
{
    HINSTANCE hInst = (HINSTANCE)GetWindowLong(hwndParent, GWL_HINSTANCE);

    return CreateWindow(szRulerName,
                        NULL,
                        WS_CHILD | WS_VISIBLE,
                        0,
                        0,
                        dx,
                        dy,
                        hwndParent,
                        (HMENU)uID,
                        hInst,
                        NULL);
}

//
//  FUNCTION: MsgRulerCreate(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: To initialize the Ruler box 
//
//  PARAMETERS:
//    hwnd - The window handing the message.
//    uMessage - The message number. (unused).
//    wparam - Message specific data (unused).
//    lparam - Message specific data (unused).
//
//  RETURN VALUE:
//    Always returns 0 - message handled.
//
//  COMMENTS:
//    Uses the version apis to retrieve version information for
//    each of the static text boxes in the Ruler box.
//

LRESULT MsgRulerCreate(HWND hwnd, UINT uMessage, WPARAM wparam, LPARAM lparam)
{
    int i;

    lpRuler = malloc(sizeof(RULER));
    if (lpRuler)
    {
        SETRULERPOINTER(lpRuler);

        iLeftMargin  = 1440;
        iRightMargin = 6*1440;
        iIndent      = 1440+720;
        iLeftPage    = 0;
        iRightPage   = 7*1440;

        for (i = 0; i < RULER_MAXTABS; i++) lTabs[i] = 0;

        if (!hRulerImageList)
        {
            hRulerImageList = ImageList_LoadBitmap(hInst,
                                                   MAKEINTRESOURCE(IDB_RULER),
                                                   8,
                                                   1,
                                                   RGB(255,0,0)
                                                  );
        }
        iRulerCount++;
    }
    return TRUE;
}

//
//  FUNCTION: MsgRulerCreate(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: To initialize the Ruler box 
//
//  PARAMETERS:
//    hwnd - The window handing the message.
//    uMessage - The message number. (unused).
//    wparam - Message specific data (unused).
//    lparam - Message specific data (unused).
//
//  RETURN VALUE:
//    Always returns 0 - message handled.
//
//  COMMENTS:
//    Uses the version apis to retrieve version information for
//    each of the static text boxes in the Ruler box.
//

LRESULT MsgRulerDestroy(HWND hwnd, UINT uMessage, WPARAM wparam, LPARAM lparam)
{
    GETRULERPOINTER;
    if (lpRuler) free(lpRuler);

    iRulerCount--;
    if (!iRulerCount)
    {
        ImageList_Destroy(hRulerImageList);
        hRulerImageList = NULL;
    }

    return 0;
}


//
//  FUNCTION: InitMDIChild(HINSTANCE)
//
//  PURPOSE: To register the MDI child window class.
//
//  PARAMETERS:
//    hinst - The instance of the application used to register the class.
//
//  RETURN VALUE:
//    TRUE - Succeeded in registering the class.
//    FALSE - Failed to register the class.
//
//  COMMENTS:
//
//

BOOL InitRuler(HINSTANCE hinst)
{
    WNDCLASS  wc = {0};

    wc.lpfnWndProc   = (WNDPROC)Ruler;
    wc.hIcon         = NULL;
    wc.hCursor       = LoadCursor(NULL, IDC_ARROW);
    wc.lpszMenuName  = NULL;
    wc.hInstance     = hinst;                      // Owner of this class
    wc.cbClsExtra    = 0;
    wc.cbWndExtra    = sizeof(LPRULER);
    wc.hbrBackground = GetStockObject(LTGRAY_BRUSH);
    wc.lpszClassName = szRulerName;

    return (BOOL)RegisterClass(&wc);
}

int TwipsToPixels (HDC hDC, int iTwips)
{
    POINT pt;
    int   iOldMode;

    iOldMode = SetMapMode(hDC, MM_TWIPS);

    pt.x = iTwips;
    pt.y = 0;

    LPtoDP(hDC, &pt, 1);

    SetMapMode(hDC, iOldMode);

    return pt.x;
}

int PixelsToTwips (HDC hDC, int iPixels)
{
    POINT pt;
    int   iOldMode;

    iOldMode = SetMapMode(hDC, MM_TWIPS);

    pt.x = iPixels;
    pt.y = 0;

    DPtoLP(hDC, &pt, 1);

    SetMapMode(hDC, iOldMode);

    return pt.x;
}

int InsertTab (HWND hwnd, int uPixelLocation)
{
    int     i;
    int     iTwipValue;
    HDC     hDC;

    GETRULERPOINTER;

    hDC = GetDC(hwnd);
    iTwipValue = PixelsToTwips(hDC, uPixelLocation);
    ReleaseDC(hwnd, hDC);

    for (i = 0; (lTabs[i]) && (i < RULER_MAXTABS); i++);
    if (RULER_MAXTABS-1 == i) 
        return -1; // no room!

    lTabs[i] = iTwipValue;

    return i;
}

BOOL DeleteTab (HWND hwnd, int iWhoToRemove)
{
    int i;

    GETRULERPOINTER;

    for (i = iWhoToRemove; i < RULER_MAXTABS-1; i++)
        lTabs[i] = lTabs[i+1];
    lTabs[RULER_MAXTABS-1] = 0;
    return TRUE;        
}

void SortTabs (HWND hwnd)
{
    int  i, j;
    LONG temp;

    GETRULERPOINTER;

    for (i = 0; i < RULER_MAXTABS-1; i++)
        for (j = 0; j < RULER_MAXTABS-1; j++)
            if ((!lTabs[j]) || ((lTabs[j] > lTabs[j+1])&&(lTabs[j+1])))
            {
                temp        = lTabs[j];
                lTabs[j]    = lTabs[j+1];
                lTabs[j+1]  = temp;
            }
}

LRESULT MsgRulerEraseBkgnd(HWND hwnd, UINT uMessage, WPARAM wparam, LPARAM lparam)
{
    return 0L;    
}


//
//  FUNCTION: MsgRulerCreate(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: To initialize the Ruler box 
//
//  PARAMETERS:
//    hwnd - The window handing the message.
//    uMessage - The message number. (unused).
//    wparam - Message specific data (unused).
//    lparam - Message specific data (unused).
//
//  RETURN VALUE:
//    Always returns 0 - message handled.
//
//  COMMENTS:
//    Uses the version apis to retrieve version information for
//    each of the static text boxes in the Ruler box.
//

LRESULT MsgRulerPaint(HWND hwnd, UINT uMessage, WPARAM wparam, LPARAM lparam)
{
    HDC           hDC, hMemDC;
    HBITMAP       hBitmap;
    PAINTSTRUCT   ps;
    int           iTickLeft;
    int           iTickRight;
    int           iTickIndent;
    int           iPixelsPerEigth;
    RECT          rc, rcWindow;
    int           i, iCount;
    char          sz[16];
    HPEN          hOldPen, hHighlight, hShadow;
    HBRUSH        hOldBrush;

    GETRULERPOINTER;

    hDC = BeginPaint(hwnd, &ps);   

    if ((lpRuler) && (hRulerImageList))
    {
        GetClientRect(hwnd, &rcWindow);

        hMemDC  = CreateCompatibleDC(hDC);
        hBitmap = CreateCompatibleBitmap(hDC, rcWindow.right, rcWindow.bottom);
        hBitmap = SelectObject(hMemDC,hBitmap);
        FillRect(hMemDC, 
                 &rcWindow, 
                 (HBRUSH)GetClassLong(hwnd, GCL_HBRBACKGROUND));

        // Step 1: Lay Down the gray and white sections

        GetClientRect(hwnd, &rc);
        rc.left  = TwipsToPixels (hMemDC, iLeftPage);
        rc.right = TwipsToPixels (hMemDC, iRightPage);
        rc.top   = 4;
        rc.bottom= 27;
        SetBkColor (hMemDC, RGB(255,255,255));
        ExtTextOut (hMemDC, rc.left, 0, ETO_OPAQUE, &rc, NULL, 0, NULL);

        // Step 2: Draw the highlights/shadows

        hHighlight = CreatePen(PS_SOLID, 1, RGB(255,255,255));
        hShadow    = CreatePen(PS_SOLID, 1, RGB(128,128,128));

        hOldPen   = SelectObject(hMemDC, hHighlight);
        hOldBrush = SelectObject(hMemDC, GetStockObject(NULL_BRUSH));
      
        SelectObject(hMemDC, hShadow);

        Rectangle(hMemDC, min(iLeftPage,4), 4, rc.right+1, 28);

        SelectObject(hMemDC, hHighlight);
        Rectangle(hMemDC, min (iLeftPage,4)+1, 5, rcWindow.right+1, 29);
        MoveToEx(hMemDC, min (iLeftPage,4), 28, NULL);
        LineTo(hMemDC, min (iLeftPage,4), 29);

        Rectangle (hMemDC, rc.right+1, 5, rcWindow.right+1, 29);

        SelectObject (hMemDC, hShadow);
        Rectangle (hMemDC, min (iLeftPage,4), 4, rc.left+1, 28);
        Rectangle (hMemDC, rc.right, 4, rcWindow.right+1, 28);

        SelectObject (hMemDC, GetStockObject (BLACK_PEN));
        MoveToEx(hMemDC, rc.left+1, 27, NULL);
        LineTo(hMemDC, rc.left+1, 5);
        LineTo(hMemDC, rc.right, 5);

        SelectObject(hMemDC, hOldPen);
        SelectObject(hMemDC, hOldBrush);

        // Step 3: Draw the line with the margins

        SetBkMode(hMemDC, TRANSPARENT);
        iPixelsPerEigth = TwipsToPixels(hMemDC, 1440/8);
        GetClientRect(hwnd, &rc);
        for (iCount = 0, i = 0; i <= rc.right; i+= iPixelsPerEigth, iCount++)
        {
            switch (iCount % 8)
            {
                case 0: // On the inch

                    if (i)
                    {
                        RECT rcNum;
                        rcNum.left = i-32;
                        rcNum.right = i+32;
                        rcNum.top = 0;
                        rcNum.bottom = 32;
                        wsprintf(sz, "%d", iCount/8);
                        DrawText(hMemDC, sz, lstrlen(sz), &rcNum, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
                    }
                    break;

                case 4: // On the half inch

                    MoveToEx(hMemDC, i, 12, NULL);
                    LineTo(hMemDC, i, 20);
                    break;

                default: // On the 8th inch

                    MoveToEx (hMemDC, i, 15, NULL);
                    LineTo(hMemDC, i, 17);
                    break;
            }
        }

        iTickLeft   = TwipsToPixels(hMemDC, iLeftMargin);
        iTickRight  = TwipsToPixels(hMemDC, iRightMargin);
        iTickIndent = TwipsToPixels(hMemDC, iIndent);

        ImageList_Draw(hRulerImageList,
                       RULER_LEFTMARGIN,
                       hMemDC,
                       iTickLeft-4,
                       16,
                       ILD_TRANSPARENT);
        ImageList_Draw(hRulerImageList,
                       RULER_RIGHTMARGIN,
                       hMemDC,
                       iTickRight-4,
                       16,
                       ILD_TRANSPARENT);
        ImageList_Draw(hRulerImageList,
                       RULER_INDENT,
                       hMemDC,
                       iTickIndent-4,
                       0,
                       ILD_TRANSPARENT);

        for (i = 0; i < RULER_MAXTABS; i++)
        {
            if (lTabs[i]) 
                ImageList_Draw(hRulerImageList,
                               RULER_LEFTTAB,
                               hMemDC,
                               TwipsToPixels (hMemDC, lTabs[i]),
                               11,
                               ILD_TRANSPARENT);
        }

        BitBlt(hDC, 0, 0, rcWindow.right, rcWindow.bottom, 
               hMemDC, 0, 0, 
               SRCCOPY);
        hBitmap = SelectObject(hMemDC,hBitmap);
		DeleteObject(hBitmap);
        DeleteObject(hHighlight);
        DeleteObject(hShadow);
        DeleteDC(hMemDC);
    }

    EndPaint(hwnd, &ps);   

    return 0;
}

LRESULT MsgRulerLButtonDown(HWND hwnd, UINT uMessage, WPARAM wparam, LPARAM lparam)
{
    RECT          rc;
    int           i;
    int           iTickLeft;
    int           iTickRight;
    int           iTickIndent;
    POINT         pt;
    HDC           hDC;
    int           iXPos, iYPos;
    int           dxHotSpot;

    GETRULERPOINTER;

    pt.x = LOWORD(lparam);
    pt.y = HIWORD(lparam);

    if (lpRuler)
    {
        iWhoToDrag = -1;

        hDC = GetDC(hwnd);

        iTickLeft   = TwipsToPixels(hDC, iLeftMargin);
        iTickRight  = TwipsToPixels(hDC, iRightMargin);
        iTickIndent = TwipsToPixels(hDC, iIndent);

        SetRect(&rc, iTickLeft-4, 16, iTickLeft+4, 32);
        if (PtInRect(&rc, pt)) 
        {
            iWhoToDrag = RULER_LEFTMARGIN;
            dxHotSpot  = pt.x - rc.left;
            dyHotSpot  = 16;
            iXPos = rc.left;
            iYPos = rc.top;
        }
        SetRect(&rc, iTickRight-4, 16, iTickRight+4, 32);
        if (PtInRect(&rc, pt)) 
        {
            iWhoToDrag = RULER_RIGHTMARGIN;
            dxHotSpot  = pt.x - rc.left;
            dyHotSpot  = 16;
            iXPos = rc.left;
            iYPos = rc.top;
        }
        SetRect(&rc, iTickIndent-4, 0, iTickIndent+4, 15);
        if (PtInRect(&rc, pt)) 
        {
            iWhoToDrag = RULER_INDENT;
            dxHotSpot  = pt.x - rc.left;
            dyHotSpot  = 0;
            iXPos = rc.left;
            iYPos = rc.top;
        }
        for (i = 0; i < RULER_MAXTABS && -1 == iWhoToDrag; i++)
        {
            SetRect(&rc, 
                    TwipsToPixels(hDC,lTabs[i]), 10, 
                    TwipsToPixels(hDC,lTabs[i])+6, 28);
            if (PtInRect (&rc, pt)) 
            {
                iWhoToDrag = RULER_LEFTTAB + i;
                dxHotSpot  = pt.x - rc.left;
                dyHotSpot  = 11;
                iXPos = rc.left;
                iYPos = rc.top;
            }
        }

        ReleaseDC(hwnd, hDC);

        if (-1 == iWhoToDrag)
        {
            iWhoToDrag = InsertTab(hwnd, pt.x);
            if (-1 != iWhoToDrag) 
            {
                iWhoToDrag += RULER_LEFTTAB;
                dxHotSpot  = 0;
                dyHotSpot  = 11;
                iXPos = pt.x;
                iYPos = rc.top;
            }
        }

        if (-1 != iWhoToDrag)
        {
            bDragging = TRUE;
            SetCapture (hwnd);

            ShowCursor ( FALSE );

            hDC = GetDC(hwnd);

            ImageList_Draw (hRulerImageList,
                            min (RULER_LEFTTAB, iWhoToDrag) + 4,
                            hDC,
                            iXPos,
                            dyHotSpot,
                            ILD_TRANSPARENT);

            ReleaseDC(hwnd, hDC);

            // Start the drag and drop procedure.
            ImageList_BeginDrag(hRulerImageList,
                                min (RULER_LEFTTAB, iWhoToDrag),
                                dxHotSpot,
                                0);

            // Lock the window for painting the drag and drop
            // image; this will cause the drag/drop image to
            // appear.
            ImageList_DragEnter(hwnd, iXPos+dxHotSpot, dyHotSpot);

        }
    }
    return 0;
}

LRESULT MsgRulerLButtonUp(HWND hwnd, UINT uMessage, WPARAM wparam, LPARAM lparam)
{
    int         iTabPos;
    HDC       hDC;
    NMRULER   nmruler;
    RECT      rc;

    GETRULERPOINTER;
    if (bDragging)
    {
        // Tell the Common Controls DD to stop the drag image
        ImageList_EndDrag();
        // Unlock any painting associated with the Image List's drag/drop
        ImageList_DragLeave(hwnd);

        ReleaseCapture ();
        ShowCursor(TRUE);
        bDragging = FALSE;

        hDC = GetDC(hwnd);

        iTabPos = PixelsToTwips (hDC, LOWORD(lparam));

        if (-1 != iWhoToDrag)
            switch (iWhoToDrag)
            {
                case RULER_LEFTMARGIN:  iLeftMargin  = iTabPos;                    break;
                case RULER_INDENT:      iIndent      = iTabPos;                    break;
                case RULER_RIGHTMARGIN: iRightMargin = iTabPos;                    break;
                default:             
                     if ((HIWORD(lparam)<0) || (HIWORD(lparam)>32)) // Out of bounds!
                     {
                         DeleteTab(hwnd, iWhoToDrag-RULER_LEFTTAB);
                     }
                     else
                     {
                         lTabs[iWhoToDrag-RULER_LEFTTAB] = iTabPos; 
                         SortTabs(hwnd);
                     }
                     break;

                }
        InvalidateRect (hwnd, NULL, FALSE);

        GetClientRect(GetParent(hwnd), &rc);

        nmruler.nmhdr.hwndFrom   = hwnd;
        nmruler.nmhdr.idFrom     = GetWindowLong(hwnd, GWL_ID);
        nmruler.nmhdr.code       = RN_CHANGEDSETTINGS;
        nmruler.iLeft            = iLeftMargin;
        nmruler.iRight           = iRightPage - iRightMargin;
        nmruler.iFirstLineIndent = iIndent;

        ReleaseDC(hwnd, hDC);

        // Notify the parent app the ruler's settings have changed
        SendMessage (GetParent(hwnd), 
                     WM_NOTIFY, 
                     nmruler.nmhdr.idFrom, 
                     (LPARAM)&nmruler);
    }
    return 0;
}

LRESULT MsgRulerMouseMove(HWND hwnd, UINT uMessage, WPARAM wparam, LPARAM lparam)
{
    if (bDragging) 
    {
        if (iWhoToDrag < RULER_LEFTTAB)
        {
            ImageList_DragMove(LOWORD(lparam), dyHotSpot);
        }
        else
        {
            if ((HIWORD(lparam)<0) || (HIWORD(lparam)>32))
                ImageList_DragMove(LOWORD(lparam), 
                                   dyHotSpot+GetSystemMetrics(SM_CYSCREEN));
            else
                ImageList_DragMove(LOWORD(lparam), dyHotSpot);
        }
    }
    return 0;
}

LRESULT MsgRulerGetMargins(HWND hwnd, UINT uMessage, WPARAM wparam, LPARAM lparam)
{
    HDC     hDC;
    RECT    rc;
    LPINT   iMargins = (LPINT)lparam;

    GETRULERPOINTER;

    GetClientRect(GetParent(hwnd), &rc);

    hDC = GetDC(hwnd); 
    
    iMargins[0] = iLeftMargin;
    iMargins[1] = iRightPage - iRightMargin;
    iMargins[2] = iIndent;
    iMargins[3] = iLeftPage;
    iMargins[4] = iRightPage;
        
    ReleaseDC(hwnd, hDC);
    
    return 0;
}

LRESULT MsgRulerSetMargins (HWND hwnd, UINT uMessage, WPARAM wparam, LPARAM lparam)
{
    HDC     hDC;
    RECT    rc;
    LPINT   iMargins = (LPINT)lparam;
    UINT    iTempLeft;
    UINT    iTempRight;
    UINT    iTempIndent;
    UINT    iTempLeftPage;
    UINT    iTempRightPage;

    GETRULERPOINTER;

    GetClientRect(GetParent(hwnd), &rc);

    hDC = GetDC(hwnd); 
    
    iTempLeft      = iMargins[0];
    iTempRight     = iMargins[4] - iMargins[1];
    iTempIndent    = iMargins[2];
    iTempLeftPage  = iMargins[3];
    iTempRightPage = iMargins[4];
        
    ReleaseDC(hwnd, hDC);

    if (
         (iTempLeft      != iLeftMargin)  ||
         (iTempRight     != iRightMargin) ||
         (iTempIndent    != iIndent)      ||
         (iTempLeftPage  != iLeftPage)    ||
         (iTempRightPage != iRightPage)
       )
       {
       iLeftMargin  = iTempLeft  ;
       iRightMargin = iTempRight ;
       iIndent      = iTempIndent; 
       iLeftPage    = iTempLeftPage;
       iRightPage   = iTempRightPage;
       InvalidateRect (hwnd,NULL,FALSE);
       }
    
    return 0;
}

LRESULT MsgRulerGetTabCount(HWND hwnd, UINT uMessage, WPARAM wparam, LPARAM lparam)
{
    int i, iCount;

    GETRULERPOINTER;

    iCount = 0;
    for (i = 0; i < RULER_MAXTABS; i++) if (lTabs[i]) iCount++;
    return (LPARAM)iCount;
}

LRESULT MsgRulerGetTabs(HWND hwnd, UINT uMessage, WPARAM wparam, LPARAM lparam)
{
    int i, iCount;
    GETRULERPOINTER;

    iCount = 0;
    for (i = 0; i < RULER_MAXTABS; i++) if (lTabs[i]) iCount++;
    for (i = 0; i < min(iCount, (int)wparam); i++) ((LPLONG)lparam)[i] = lTabs[i];
    return (LPARAM)iCount;
}

LRESULT MsgRulerSetTabs    (HWND hwnd, UINT uMessage, WPARAM wparam, LPARAM lparam)
{
    int     i;
    BOOL    bDiff;

    GETRULERPOINTER;

    bDiff = FALSE;
    for (i = 0; i < min(RULER_MAXTABS, (int)wparam); i++)
        if (lTabs[i] != ((LPLONG)lparam)[i]) bDiff = TRUE;
    if (wparam+1 < (RULER_MAXTABS-1))
        if (lTabs[wparam+1]) bDiff = TRUE;

    if (bDiff)
    {
        for (i = 0; i < RULER_MAXTABS; i++) lTabs[i] = 0;
        for (i = 0; i < min(RULER_MAXTABS, (int)wparam); i++)
            lTabs[i] = ((LPLONG)lparam)[i];
        InvalidateRect(hwnd, NULL, FALSE);
    }

    return 0L;
}
