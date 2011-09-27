/**************************************************************************
 *
 *  THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF ANY
 *  KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
 *  IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A PARTICULAR
 *  PURPOSE.
 *
 *  Copyright (c) 1992 - 1995  Microsoft Corporation.  All Rights Reserved.
 *
 **************************************************************************/

#define STRICT
#include <windows.h>
#include <windowsx.h>
#include "pro.h"

//----------------------------------------------------------------------------

#define BUFSIZE         260

#define BAR_RANGE       0
#define BAR_POS         4
#define BAR_SETRANGE    WM_USER+BAR_RANGE
#define BAR_SETPOS      WM_USER+BAR_POS
#define BAR_DELTAPOS    WM_USER+8
#define PRO_CLASS       "PRO"

//----------------------------------------------------------------------------

BOOL    CALLBACK ProDlgProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK ProBarProc(HWND, UINT, WPARAM, LPARAM);
static  BOOL     Bar_OnCreate(HWND, LPCREATESTRUCT);
static  void     Bar_OnPaint(HWND);
static  LRESULT  Bar_OnSet(HWND, UINT, LPARAM);
static  BOOL     Dlg_OnInit(HWND);

//----------------------------------------------------------------------------

static HWND     ghWnd = NULL;
static int      iCnt;
static DWORD    rgbFG;
static DWORD    rgbBG;

//----------------------------------------------------------------------------

#define COLORBG  rgbBG
#define COLORFG  rgbFG

/*----------------------------------------------------------------------------*\
|   ProInit( hPrev,hInst )                                                     |
|                                                                              |
|   Description:                                                               |
|       This is called when the application is first loaded into               |
|       memory.  It performs all initialization.                               |
|                                                                              |
|   Arguments:                                                                 |
|       hPrev      instance handle of previous instance                        |
|       hInst      instance handle of current instance                         |
|                                                                              |
|   Returns:                                                                   |
|       TRUE if successful, FALSE if not                                       |
|                                                                              |
\*----------------------------------------------------------------------------*/

BOOL CALLBACK ProInit(
HANDLE hPrev,
HANDLE hInst )
{
        WNDCLASS   rClass;

        if (!hPrev) {
                rClass.hCursor          = LoadCursor(NULL,IDC_ARROW);
                rClass.hIcon            = NULL;
                rClass.lpszMenuName     = NULL;
                rClass.lpszClassName    = PRO_CLASS;
                rClass.hbrBackground    = (HBRUSH)COLOR_WINDOW+1;
                rClass.hInstance        = hInst;
                rClass.style            = CS_HREDRAW | CS_VREDRAW;
                rClass.lpfnWndProc      = (WNDPROC)ProBarProc;
                rClass.cbClsExtra       = 0;
                rClass.cbWndExtra       = 2*sizeof(LONG);

        if (!RegisterClass(&rClass))
                return FALSE;
        }

        rgbBG = RGB(0,0,255);
        rgbFG = RGB(255,255,255);

        return TRUE;
}

/*----------------------------------------------------------------------------*\
|   ProOpen ()                                                                 |
|                                                                              |
|   Description:                                                               |
|                                                                              |
|   Arguments:                                                                 |
|                                                                              |
|   Returns:                                                                   |
|       0 if processed, nonzero if ignored                                     |
|                                                                              |
\*----------------------------------------------------------------------------*/
HWND CALLBACK ProOpen(
HWND hWnd)
{

        HINSTANCE hInst = (HINSTANCE)GetWindowLong(hWnd,GWL_HINSTANCE);

        iCnt++;
        if (!ghWnd) {
                ghWnd = CreateDialog(hInst,"DLG_PROGRESS",hWnd,(DLGPROC)ProDlgProc);
                if (ghWnd) {
                        ShowWindow (ghWnd,SHOW_OPENWINDOW);
                        UpdateWindow(ghWnd);
                        EnableWindow(hWnd, FALSE);
                }
        }
        SendDlgItemMessage(ghWnd,ID_BAR,BAR_SETRANGE,0,100);
        ProSetBarPos(0);
        return ghWnd;
}
/*----------------------------------------------------------------------------*\
|   ProClose ()                                                                |
|                                                                              |
|   Description:                                                               |
|                                                                              |
|   Arguments:                                                                 |
|                                                                              |
|   Returns:                                                                   |
|       0 if processed, nonzero if ignored                                     |
|                                                                              |
\*----------------------------------------------------------------------------*/
BOOL CALLBACK ProClose(
)
{
        iCnt--;
        if (ghWnd && iCnt == 0) {
                EnableWindow(GetParent(ghWnd), TRUE);
                DestroyWindow (ghWnd);
                ghWnd = NULL;
        }
        return TRUE;
}
/*----------------------------------------------------------------------------*\
|   ProSetText (int,LPSTR)                                                     |
|                                                                              |
|   Description:                                                               |
|                                                                              |
|   Arguments:                                                                 |
|                                                                              |
|   Returns:                                                                   |
|       0 if processed, nonzero if ignored                                     |
|                                                                              |
\*----------------------------------------------------------------------------*/
BOOL CALLBACK ProSetText(
int i,
LPSTR lpsz)
{
        if (ghWnd) {
                SetDlgItemText(ghWnd,i,lpsz);
                return TRUE;
        }
        return FALSE;
}
/*----------------------------------------------------------------------------*\
|   ProSetBarPos (UINT)                                                        |
|                                                                              |
|   Description:                                                               |
|                                                                              |
|   Arguments:                                                                 |
|                                                                              |
|   Returns:                                                                   |
|       0 if processed, nonzero if ignored                                     |
|                                                                              |
\*----------------------------------------------------------------------------*/
BOOL CALLBACK ProSetBarPos(
UINT uPos)
{
        if (ghWnd) {
                SendDlgItemMessage(ghWnd,ID_BAR,BAR_SETPOS,0,uPos);
                return TRUE;
        }
        return FALSE;
}
/*----------------------------------------------------------------------------*\
|   ProDlgProc( hWnd, uiMessage, wParam, lParam )                              |
|                                                                              |
|   Description:                                                               |
|       The window proc for the Progress dialog box                            |
|                                                                              |
|   Arguments:                                                                 |
|       hWnd            window handle for the dialog                           |
|       uiMessage       message number                                         |
|       wParam          message-dependent                                      |
|       lParam          message-dependent                                      |
|                                                                              |
|   Returns:                                                                   |
|       0 if processed, nonzero if ignored                                     |
|                                                                              |
\*----------------------------------------------------------------------------*/
BOOL CALLBACK ProDlgProc(
HWND hDlg,
UINT uiMessage,
WPARAM wParam,
LPARAM lParam)
{
        switch (uiMessage) {
                case WM_INITDIALOG:
                        Dlg_OnInit(hDlg);
                        break;
        }
        return FALSE;
}
/*----------------------------------------------------------------------------*\
|   ProBarProc( hWnd, uiMessage, wParam, lParam )                              |
|                                                                              |
|   Description:                                                               |
|       The window proc for the Progress Bar chart                             |
|                                                                              |
|   Arguments:                                                                 |
|       hWnd            window handle for the dialog                           |
|       uiMessage       message number                                         |
|       wParam          message-dependent                                      |
|       lParam          message-dependent                                      |
|                                                                              |
|   Returns:                                                                   |
|       0 if processed, nonzero if ignored                                     |
|                                                                              |
\*----------------------------------------------------------------------------*/

LRESULT CALLBACK ProBarProc(
HWND hwnd,
UINT uiMessage,
WPARAM wParam,
LPARAM lParam)
{
        switch (uiMessage)
        {
                case WM_CREATE:
                        return HANDLE_WM_CREATE(hwnd, wParam, lParam, Bar_OnCreate);
                        break;

                case BAR_SETRANGE:
                case BAR_SETPOS:
                        return Bar_OnSet(hwnd, uiMessage, lParam);
                        break;

                case WM_PAINT:
                        return HANDLE_WM_PAINT(hwnd, wParam, lParam, Bar_OnPaint);
                        break;
        }
        return DefWindowProc(hwnd,uiMessage,wParam,lParam);
}

//----------------------------------------------------------------------------
//
//      Message Handler for WM_CREATE
//
//----------------------------------------------------------------------------
static BOOL Bar_OnCreate(
HWND hwnd,
LPCREATESTRUCT lpCreateStruct)
{
        SetWindowLong (hwnd,BAR_RANGE,100);
        SetWindowLong (hwnd,BAR_POS,0);
        return TRUE;
}

//----------------------------------------------------------------------------
//
//      Message Handler for WM_PAINT
//
//----------------------------------------------------------------------------
static void Bar_OnPaint(
HWND hwnd)
{
        PAINTSTRUCT     rPS;
        RECT            rc1,rc2;
        char            szBuffer[BUFSIZE];
        LONG            lRange,lPos;
        int             nRight,nBottom,nMiddle,nPercent;
        int             nStrLength, nLeft, nTop;
        SIZE            sizeText;

        BeginPaint(hwnd,&rPS);
        GetClientRect (hwnd,&rc1);
        FrameRect(rPS.hdc,&rc1,GetStockObject(BLACK_BRUSH));
        InflateRect(&rc1,-1,-1);
        CopyRect( &rc2, &rc1 );
        lRange = GetWindowLong(hwnd,BAR_RANGE);
        lPos   = GetWindowLong(hwnd,BAR_POS);

        if (lRange <= 0)
                lRange = 1;

        if (lPos > lRange)      // make sure we don't go past 100%
                lPos = lRange;

        nRight  = rc1.right;
        nBottom = rc1.bottom;
        nMiddle = MulDiv( (int)lPos, nRight, (int)lRange ) + 1;
        nPercent= MulDiv( (int)lPos, 100, (int)lRange );

        wsprintf (szBuffer,"%3i%%",nPercent);
        nStrLength = lstrlen(szBuffer);
        GetTextExtentPoint(rPS.hdc, szBuffer, nStrLength, &sizeText );

        rc1.right = nMiddle;
        rc2.left  = nMiddle;

        nLeft = (nRight  - ((int)sizeText.cx))/2;
        nTop  = (nBottom - ((int)sizeText.cy))/2;

        SetBkColor(rPS.hdc,COLORBG);
        SetTextColor(rPS.hdc,COLORFG);
        ExtTextOut (rPS.hdc, nLeft, nTop, ETO_OPAQUE | ETO_CLIPPED,
                &rc1, szBuffer, nStrLength, NULL);
        SetBkColor(rPS.hdc,COLORFG);
        SetTextColor(rPS.hdc,COLORBG);
        ExtTextOut (rPS.hdc, nLeft, nTop, ETO_OPAQUE | ETO_CLIPPED,
                &rc2, szBuffer, nStrLength, NULL);
        EndPaint(hwnd,(LPPAINTSTRUCT)&rPS);
        return;
}

//----------------------------------------------------------------------------
//
//      Message Handler for BAR_SETRANGE and BAR_SETPOS
//
//----------------------------------------------------------------------------
static LRESULT Bar_OnSet(
HWND hwnd,
UINT uiMessage,
LPARAM lpSet)
{
        SetWindowLong (hwnd,(uiMessage-WM_USER),lpSet);
        InvalidateRect (hwnd,NULL,FALSE);
        UpdateWindow(hwnd);
        return 0L;
}
//----------------------------------------------------------------------------
//
//      Message Handler for WM_INITDIALOG
//
//----------------------------------------------------------------------------
static  BOOL Dlg_OnInit(
HWND hDlg)
{
        SetDlgItemText (hDlg, ID_STATUS1, "");
        SetDlgItemText (hDlg, ID_STATUS2, "");
        SetDlgItemText (hDlg, ID_STATUS3, "");
        SetDlgItemText (hDlg, ID_STATUS4, "");
        return TRUE;
}
