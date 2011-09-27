
/******************************************************************************\
*       This is a part of the Microsoft Source Code Samples. 
*       Copyright (C) 1993-1995 Microsoft Corporation.
*       All rights reserved. 
*       This source code is only intended as a supplement to 
*       Microsoft Development Tools and/or WinHelp documentation.
*       See these sources for detailed information regarding the 
*       Microsoft samples programs.
\******************************************************************************/

/***************************************************************************
 *                                                                         *
 *  MODULE      : infoctrl.c                                               *
 *                                                                         *
 *  PURPOSE     : Functions for the infoctrl control class                 *
 *                                                                         *
 ***************************************************************************/
/*
 * INFOCTRL.C
 *
 * This module implements a custom information display control which
 * can present up to 7 seperate strings of information at once and is
 * sizeable and moveable with the mouse.
 */

#include <windows.h>
#include <string.h>
#include <memory.h>
#include "infoctrl.h"
#include "track.h"

CHAR szClass[] = "InfoCtrl_class";
DWORD cCreated = 0;
CHAR szNULL[] = "";
INT cxMargin = 0;
INT cyMargin = 0;
HBRUSH hFocusBrush;


LONG  APIENTRY InfoCtrlWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
VOID MyDrawText(HDC hdc, LPRECT lprc, PSTR psz, DWORD wFormat);
VOID DrawFocus(HDC hdc, HWND hwnd, DWORD style);
INT CountWindows(HWND hwndParent);
VOID GetCascadeWindowPos(HWND hwndParent, INT  iWindow, LPRECT lprc);


/****************************************************************************
 *                                                                          *
 *  FUNCTION   :                                                            *
 *                                                                          *
 *  PURPOSE    :                                                            *
 *                                                                          *
 *  RETURNS    :                                                            *
 *                                                                          *
 ****************************************************************************/
HWND CreateInfoCtrl(
LPSTR pszCenter,              // NULL is ok.
INT x,
INT y,
INT cx,
INT cy,
HWND hwndParent,
HANDLE hInst,
LPSTR pszUL,                // NULLs here are fine.
LPSTR pszUC,
LPSTR pszUR,
LPSTR pszLL,
LPSTR pszLC,
LPSTR pszLR,
DWORD  style,
HMENU id,
DWORD dwUser)
{
    INFOCTRL_DATA *picd;
    HWND hwnd;

    if (!cCreated) {
        WNDCLASS wc;
        TEXTMETRIC metrics;
        HDC hdc;

        wc.style = CS_VREDRAW | CS_HREDRAW;
        wc.lpfnWndProc = InfoCtrlWndProc;
        wc.cbClsExtra = 0;
        wc.cbWndExtra = ICCBWNDEXTRA;
        wc.hInstance = hInst;
        wc.hIcon = NULL;
        wc.hCursor = NULL;
        wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
        wc.lpszMenuName =  NULL;
        wc.lpszClassName = szClass;

        RegisterClass(&wc);

        hdc = GetDC(hwndParent);
        GetTextMetrics(hdc, &metrics);
        cyMargin = metrics.tmHeight;
        cxMargin = metrics.tmAveCharWidth * 2;
        ReleaseDC(hwndParent, hdc);
        hFocusBrush = CreateSolidBrush(RGB(0, 0, 255));
    }

    if (!(picd = (INFOCTRL_DATA *)LocalAlloc(LPTR, sizeof(INFOCTRL_DATA))))
        return(FALSE);

    if (pszCenter) {
        picd->pszCenter = (PSTR)(PSTR)LocalAlloc(LPTR, strlen(pszCenter) + 1);
        strcpy(picd->pszCenter, pszCenter);
    } else {
        picd->pszCenter = NULL;
    }

    if (pszUL) {
        picd->pszUL = (PSTR)(PSTR)LocalAlloc(LPTR, strlen(pszUL) + 1);
        strcpy(picd->pszUL, pszUL);
    } else {
        picd->pszUL = NULL;
    }
    if (pszUC) {
        picd->pszUC = (PSTR)LocalAlloc(LPTR, strlen(pszUC) + 1);
        strcpy(picd->pszUC, pszUC);
    } else {
        picd->pszUC = NULL;
    }
    if (pszUR) {
        picd->pszUR = (PSTR)LocalAlloc(LPTR, strlen(pszUR) + 1);
        strcpy(picd->pszUR, pszUR);
    } else {
        picd->pszUR = NULL;
    }
    if (pszLL) {
        picd->pszLL = (PSTR)LocalAlloc(LPTR, strlen(pszLL) + 1);
        strcpy(picd->pszLL, pszLL);
    } else {
        picd->pszLL = NULL;
    }
    if (pszLC) {
        picd->pszLC = (PSTR)LocalAlloc(LPTR, strlen(pszLC) + 1);
        strcpy(picd->pszLC, pszLC);
    } else {
        picd->pszLC = NULL;
    }
    if (pszLR) {
        picd->pszLR = (PSTR)LocalAlloc(LPTR, strlen(pszLR) + 1);
        strcpy(picd->pszLR, pszLR);
    } else {
        picd->pszLR = NULL;
    }

    picd->style = style;
    picd->hInst = hInst;

    if (hwnd = CreateWindow(szClass, szNULL,
            WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS,
            x, y, cx, cy, hwndParent, id, hInst, (LPSTR)picd)) {
        cCreated++;
        SetWindowLong(hwnd, GWL_USER, dwUser);
        BringWindowToTop(hwnd);
        ShowWindow(hwnd, SW_SHOW);
        return(hwnd);
    }
    return(FALSE);
}




/****************************************************************************
 *                                                                          *
 *  FUNCTION   : MyDrawText                                                 *
 *                                                                          *
 *  PURPOSE    : Draws psz within lprc in hdc according to wFormat.         *
 *                                                                          *
 *  RETURNS    : Nothing.                                                   *
 *                                                                          *
 ****************************************************************************/
VOID MyDrawText(
HDC hdc,
LPRECT lprc,
PSTR psz,
DWORD wFormat)
{
    RECT rc;
    DWORD cx;

    if (psz == NULL || !*psz)
        return; // notin to draw dude.

    SetRect(&rc, 0, 0, 1, 0);
    DrawText(hdc, psz, -1, &rc, DT_CALCRECT | DT_NOCLIP | DT_SINGLELINE);
    cx = min(rc.right - rc.left, lprc->right - lprc->left);
    CopyRect(&rc, lprc);
    switch (wFormat & (DT_LEFT | DT_CENTER | DT_RIGHT)) {
    case DT_LEFT:
        rc.right = rc.left + cx;
        break;

    case DT_CENTER:
        cx = (rc.right - rc.left - cx) / 2;
        rc.right -= cx;
        rc.left += cx;
        break;

    case DT_RIGHT:
        rc.left = rc.right - cx;
        break;
    }
    DrawText(hdc, psz, -1, &rc, wFormat | DT_VCENTER);
}




/****************************************************************************
 *                                                                          *
 *  FUNCTION   : InfoCtrlWndProc                                            *
 *                                                                          *
 *  PURPOSE    : Main window proc for info controls                         *
 *                                                                          *
 *  RETURNS    : case dependent                                             *
 *                                                                          *
 ****************************************************************************/
LONG  APIENTRY InfoCtrlWndProc(
HWND hwnd,
UINT msg,
WPARAM wParam,
LPARAM lParam)
{
    INFOCTRL_DATA *picd;
    INT i;
    RECT rc;
    HDC hdc;

    switch (msg) {
    case WM_CREATE:
        /*
         * Info controls keep their information in the GWL_INFODATA window
         * word.
         */
        SetWindowLong(hwnd, GWL_INFODATA,
                (DWORD)(DWORD)(((LPCREATESTRUCT)lParam)->lpCreateParams));
        break;

    case WM_SIZE:
        /*
         * size the info control, updating the hittest rectangles.
         * The window is only allowed to get so small.
         */
        if ((short)LOWORD(lParam) < 2 * cxMargin || (short)HIWORD(lParam) < 2 * cyMargin) {
            MoveWindow(hwnd, 0, 0, max((short)LOWORD(lParam), 2 * cxMargin),
                max((short)HIWORD(lParam), 2 * cyMargin), TRUE);
        } else {
            picd = (INFOCTRL_DATA *)GetWindowLong(hwnd, GWL_INFODATA);
            SetRect(&picd->rcFocusUL, 0, 0, cxMargin, cyMargin);
            SetRect(&picd->rcFocusUR, (short)LOWORD(lParam) - cxMargin, 0,
                    (short)LOWORD(lParam), cyMargin);
            SetRect(&picd->rcFocusLL, 0, (short)HIWORD(lParam) - cyMargin,
                    cxMargin, (INT)HIWORD(lParam));
            SetRect(&picd->rcFocusLR, picd->rcFocusUR.left, picd->rcFocusLL.top,
                    picd->rcFocusUR.right, picd->rcFocusLL.bottom);
        }
        break;


    case WM_DESTROY:
        /*
         * Info control death:
         *
         * Inform out parent - last chance to access GWL_USER.
         * Free our information if it still exists.
         * Free all strings associated with this control.
         */
        {
            PSTR *ppsz;

            SendMessage(GetParent(hwnd), ICN_BYEBYE, (WPARAM)hwnd,
                    GetWindowLong(hwnd, GWL_USER));
            SetWindowLong(hwnd, GWL_USER, 0);

            picd = (INFOCTRL_DATA *)GetWindowLong(hwnd, GWL_INFODATA);
            if (picd) {
                ppsz = &picd->pszUL;
                for (i = 0; i < 5; i++, ppsz++) {
                    if (*ppsz) {
                        LocalUnlock((HANDLE)*ppsz);
                        *ppsz = (PSTR)LocalFree((HANDLE)*ppsz);
                    }
                }
                LocalUnlock((HANDLE)picd);
                LocalFree((HANDLE)picd);
                SetWindowLong(hwnd, GWL_INFODATA, 0);
            }
        }
        break;

    case WM_SETFOCUS:
    case WM_KILLFOCUS:
        /*
         * When focus changes:
         *
         * Alter our look apropriately
         * Bring ourselves to the top if necessary.
         * Inform our parent.
         * Repaint the focus portion of ourselves.
         * Call DefWindowProc()
         */
        picd = (INFOCTRL_DATA *)GetWindowLong(hwnd, GWL_INFODATA);
        if (picd != NULL) {
            if (picd->style & ICSTY_SHOWFOCUS) {
                if (msg == WM_SETFOCUS)
                    picd->style |= ICSTY_HASFOCUS;
                else
                    picd->style &= ~ICSTY_HASFOCUS;
                BringWindowToTop(hwnd);
                // notify parent
                SendMessage(GetParent(hwnd), ICN_HASFOCUS,
                        msg == WM_SETFOCUS, (LPARAM)hwnd);
            } else {
                picd->style &= ~ICSTY_HASFOCUS;
            }
            hdc = GetDC(hwnd);
            DrawFocus(hdc, hwnd, picd->style);
            ReleaseDC(hwnd, hdc);
        }
        goto DoDWP;
        break;

    case WM_MOUSEMOVE:
        /*
         * Keep the cursor updated to show sizing or moving state.
         */
        {
            LPSTR cursor;

            picd = (INFOCTRL_DATA *)GetWindowLong(hwnd, GWL_INFODATA);
            if (picd->style & ICSTY_SHOWFOCUS) {

                if ((INT)HIWORD(lParam) < cyMargin) {
                    if ((short)LOWORD(lParam) < cxMargin) {
                        cursor = IDC_SIZENWSE;
                    } else if ((short)LOWORD(lParam) > picd->rcFocusUR.left) {
                        cursor = IDC_SIZENESW;
                    } else {
                        cursor = IDC_SIZENS;
                    }
                } else if ((INT)HIWORD(lParam) > picd->rcFocusLL.top) {
                    if ((short)LOWORD(lParam) < cxMargin) {
                        cursor = IDC_SIZENESW;
                    } else if ((short)LOWORD(lParam) > picd->rcFocusUR.left) {
                        cursor = IDC_SIZENWSE;
                    } else {
                        cursor = IDC_SIZENS;
                    }
                } else {
                    if ((short)LOWORD(lParam) < cxMargin) {
                        cursor = IDC_SIZEWE;
                    } else if ((short)LOWORD(lParam) > picd->rcFocusUR.left) {
                        cursor = IDC_SIZEWE;
                    } else {
                        cursor = IDC_CROSS;
                    }
                }
            } else {
                cursor = IDC_ARROW;
            }
            SetCursor(LoadCursor(NULL, cursor));
        }
        break;

    case WM_LBUTTONDOWN:
        /*
         * Track window according do mouse location.
         */
        picd = (INFOCTRL_DATA *)GetWindowLong(hwnd, GWL_INFODATA);
        if (picd->style & ICSTY_SHOWFOCUS) {
            DWORD fs = 0;

            if (!(picd->style & ICSTY_HASFOCUS)) {
                SetFocus(hwnd);
            }

            if ((short)HIWORD(lParam) < cyMargin) {
                fs = TF_TOP;
            } else if ((INT)HIWORD(lParam) > picd->rcFocusLL.top) {
                fs = TF_BOTTOM;
            }
            if ((short)LOWORD(lParam) < cxMargin) {
                fs |= TF_LEFT;
            } else if ((short)LOWORD(lParam) > picd->rcFocusUR.left) {
                fs |= TF_RIGHT;
            } else if (fs == 0) {
                fs = TF_MOVE;
            }

            GetClientRect(hwnd, &rc);
            ClientToScreen(hwnd, (LPPOINT)&rc.left);
            ClientToScreen(hwnd, (LPPOINT)&rc.right);
            ScreenToClient(GetParent(hwnd), (LPPOINT)&rc.left);
            ScreenToClient(GetParent(hwnd), (LPPOINT)&rc.right);
            if (TrackRect(picd->hInst, GetParent(hwnd),
                    rc.left, rc.top, rc.right, rc.bottom,
                    2 * cxMargin, 2 * cyMargin,
                    fs | TF_ALLINBOUNDARY, &rc)) {

                MoveWindow(hwnd, rc.left, rc.top, rc.right - rc.left,
                        rc.bottom - rc.top, TRUE);
            }
        }
        break;

    case ICM_SETSTRING:
        /*
         * This message is sent when a info control string value is changeing.
         *
         * wParam = ICSID_ constant
         * lParam = new string.
         *
         * If new string is different from old, free old and allocate space
         * for new one and copy in.
         * Redraw invalidated part of info control.
         */
        {
            PSTR *ppsz;

            picd = (INFOCTRL_DATA *)GetWindowLong(hwnd, GWL_INFODATA);
            ppsz = (PSTR *)&picd->pszUL + wParam;

            if (lParam == 0)
                lParam = (DWORD)(LPSTR)szNULL;

            if (*ppsz) {
                if (!strcmp(*ppsz, (LPSTR)lParam)) {
                    return 0;
                }
                LocalUnlock((HANDLE)*ppsz);
                *ppsz = (PSTR)LocalFree((HANDLE)*ppsz);
            }
            if (lParam) {
                *ppsz = (PSTR)LocalAlloc(LPTR, strlen((LPSTR)lParam) + 1);
                strcpy((LPSTR)*ppsz, (LPSTR)lParam);
            }
            GetClientRect(hwnd, &rc);
            switch (wParam) {
            case ICSID_UL:
            case ICSID_UC:
            case ICSID_UR:
                rc.bottom = cyMargin;
                break;

            case ICSID_LL:
            case ICSID_LC:
            case ICSID_LR:
                rc.top = rc.bottom - cyMargin;
                break;

            case ICSID_CENTER:
                InflateRect(&rc, -cxMargin, -cyMargin);
                break;
            }
            InvalidateRect(hwnd, &rc, TRUE);
            UpdateWindow(hwnd);
        }
        break;

    case WM_PAINT:
        /*
         * Paint ourselves.
         *
         * Draw frame.
         * Draw info strings.
         * Send ownerdraw message to parent if ICSTY_OWNERDRAW.
         */
        {
            PAINTSTRUCT ps;
            HANDLE brush;

            picd = (INFOCTRL_DATA *)GetWindowLong(hwnd, GWL_INFODATA);
            BeginPaint(hwnd, &ps);
            // erasure should have already been done for us.
            GetClientRect(hwnd, &rc);
            brush = GetStockObject(BLACK_BRUSH);
            InflateRect(&rc, -cxMargin / 2, -cyMargin / 2);
            FrameRect(ps.hdc, &rc, brush);
            InflateRect(&rc, cxMargin / 2, cyMargin / 2);
            SetRect(&rc, picd->rcFocusUL.right, 0, picd->rcFocusUR.left,
                    cyMargin);
            MyDrawText(ps.hdc, &rc, picd->pszUR, DT_RIGHT);
            MyDrawText(ps.hdc, &rc, picd->pszUL, DT_LEFT);
            MyDrawText(ps.hdc, &rc, picd->pszUC, DT_CENTER);
            SetRect(&rc, picd->rcFocusLL.right, picd->rcFocusLL.top,
                    picd->rcFocusLR.left, picd->rcFocusLR.bottom);
            MyDrawText(ps.hdc, &rc, picd->pszLR, DT_RIGHT);
            MyDrawText(ps.hdc, &rc, picd->pszLL, DT_LEFT);
            MyDrawText(ps.hdc, &rc, picd->pszLC, DT_CENTER);

            GetClientRect(hwnd, &rc);
            InflateRect(&rc, -cxMargin, -cyMargin);
            if (picd->style & ICSTY_OWNERDRAW) {
                OWNERDRAWPS odps;

                if (IntersectRect(&odps.rcPaint, &rc, &ps.rcPaint)) {
                    if (IntersectClipRect(ps.hdc, rc.left, rc.top, rc.right,
                            rc.bottom) != NULLREGION) {
                        odps.rcBound = rc;
                        odps.hdc = ps.hdc;
                        odps.dwUser = GetWindowLong(hwnd, GWL_USER);
                        SendMessage(GetParent(hwnd), ICN_OWNERDRAW,
                                GetWindowLong(hwnd, GWL_ID), (DWORD)(LPSTR)&odps);
                    }
                }
            } else {
                MyDrawText(ps.hdc, &rc, picd->pszCenter, DT_LEFT | DT_WORDBREAK | DT_EXPANDTABS);
            }
            DrawFocus(ps.hdc, hwnd, picd->style);
            EndPaint(hwnd, &ps);
        }
        break;

DoDWP:
    default:
        return (DefWindowProc(hwnd, msg, wParam, lParam));
    }
    return (0);
}


/****************************************************************************
 *                                                                          *
 *  FUNCTION   : DrawFocus                                                  *
 *                                                                          *
 *  PURPOSE    : To draw focus part of info control.                        *
 *                                                                          *
 *  RETURNS    : nothing                                                    *
 *                                                                          *
 ****************************************************************************/
VOID DrawFocus(
HDC hdc,
HWND hwnd,
DWORD style)
{
    RECT rc;

    GetClientRect(hwnd, &rc);
    FrameRect(hdc, &rc, style & ICSTY_HASFOCUS ?
            hFocusBrush : GetStockObject(GRAY_BRUSH));
}




/****************************************************************************
 *                                                                          *
 *  FUNCTION   : CountWindows                                               *
 *                                                                          *
 *  PURPOSE    : Counts how many info controls the parent of this window has*
 *                                                                          *
 *  RETURNS    : the count.                                                 *
 *                                                                          *
 ****************************************************************************/
INT CountWindows(
register HWND hwndParent)
{
  INT cWindows = 0;
  register HWND hwnd;

  for (hwnd=GetWindow(hwndParent, GW_CHILD);
        hwnd;
        hwnd= GetWindow(hwnd, GW_HWNDNEXT)) {
      cWindows++;
  }
  return(cWindows);
}



/****************************************************************************
 *                                                                          *
 *  FUNCTION   : GetCascadeWindowPos                                        *
 *                                                                          *
 *  PURPOSE    : Based on a window index and the parent window size,        *
 *               calculates where to place a cascaded window.               *
 *                                                                          *
 *  RETURNS    : rectangle in lprc.                                         *
 *                                                                          *
 ****************************************************************************/
VOID GetCascadeWindowPos(
HWND hwndParent,
INT  iWindow,
LPRECT lprc)
{
  RECT      rc;
  INT       cStack;
  register INT dxClient, dyClient;

  /* Compute the width and breadth of the situation. */
  GetClientRect(hwndParent, (LPRECT)&rc);
  dxClient = rc.right - rc.left;
  dyClient = rc.bottom - rc.top;

  /* How many windows per stack? */
  cStack = dyClient / (3 * cyMargin);

  lprc->right = dxClient - (cStack * cxMargin);
  lprc->bottom = dyClient - (cStack * cyMargin);

  cStack++;             /* HACK!: Mod by cStack+1 */

  lprc->left = (iWindow % cStack) * cxMargin;
  lprc->top = (iWindow % cStack) * cyMargin;
}




/****************************************************************************
 *                                                                          *
 *  FUNCTION   : MyCascadeChildWindows                                      *
 *                                                                          *
 *  PURPOSE    : Cascades all children of a parent window                   *
 *                                                                          *
 *  RETURNS    : nothing                                                    *
 *                                                                          *
 ****************************************************************************/
VOID MyCascadeChildWindows(
register HWND hwndParent)
{
  INT       i;
  INT       cWindows;
  RECT      rc;
  DWORD      wFlags;
  register HWND hwndMove;
  HANDLE    hDefer;

  cWindows = CountWindows(hwndParent);

  if (!cWindows)
      return;

  hwndMove = GetWindow(hwndParent, GW_CHILD);

  hDefer = BeginDeferWindowPos(cWindows);

  for (i=0; i < cWindows; i++) {
      GetCascadeWindowPos(hwndParent, i, (LPRECT)&rc);

      wFlags = SWP_NOZORDER | SWP_NOACTIVATE | SWP_NOCOPYBITS;

      /* Size the window. */
      hDefer = DeferWindowPos(hDefer,
                 hwndMove, NULL,
                 rc.left, rc.top,
                 rc.right, rc.bottom,
                 wFlags);

      hwndMove = GetWindow(hwndMove, GW_HWNDNEXT);
  }

  EndDeferWindowPos(hDefer);
}


/****************************************************************************
 *                                                                          *
 *  FUNCTION   : TileChildWindows                                           *
 *                                                                          *
 *  PURPOSE    : Tiles all children of a parent window                      *
 *                                                                          *
 *  RETURNS    : nothing.                                                   *
 *                                                                          *
 ****************************************************************************/
VOID TileChildWindows(
register HWND hwndParent)
{
  INT       i;
  INT       dx;
  INT       dy;
  INT       xRes;
  INT       yRes;
  INT       iCol;
  INT       iRow;
  INT       cCols;
  INT       cRows;
  INT       cExtra;
  INT       cWindows;
  register HWND hwndMove;
  RECT      rcClient;
  HANDLE    hDefer;
  DWORD      wFlags;

  cWindows = CountWindows(hwndParent);

  if (!cWindows)
      return;

  /* Compute the smallest nearest square. */
  for (i=2; i * i <= cWindows; i++);

  cRows = i - 1;
  cCols = cWindows / cRows;
  cExtra = cWindows % cRows;

  GetClientRect(hwndParent, (LPRECT)&rcClient);
  xRes = rcClient.right - rcClient.left;
  yRes = rcClient.bottom - rcClient.top;

  if (xRes<=0 || yRes<=0)
      return;

  hwndMove = GetWindow(hwndParent, GW_CHILD);

  hDefer = BeginDeferWindowPos(cWindows);

  for (iCol=0; iCol < cCols; iCol++) {
      if ((cCols-iCol) <= cExtra)
      cRows++;

      for (iRow=0; iRow < cRows; iRow++) {
          dx = xRes / cCols;
          dy = yRes / cRows;

          wFlags = SWP_NOZORDER | SWP_NOACTIVATE | SWP_NOCOPYBITS;

          /* Position and size the window. */
          hDefer = DeferWindowPos(hDefer, hwndMove, NULL,
                     dx * iCol,
                     dy * iRow,
                     dx,
                     dy,
                     wFlags);

          hwndMove = GetWindow(hwndMove, GW_HWNDNEXT);
      }

      if ((cCols-iCol) <= cExtra) {
          cRows--;
          cExtra--;
      }
  }

  EndDeferWindowPos(hDefer);

}
