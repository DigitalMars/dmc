
/******************************************************************************\
*       This is a part of the Microsoft Source Code Samples. 
*       Copyright (C) 1993 Microsoft Corporation.
*       All rights reserved. 
*       This source code is only intended as a supplement to 
*       Microsoft Development Tools and/or WinHelp documentation.
*       See these sources for detailed information regarding the 
*       Microsoft samples programs.
\******************************************************************************/

#define NOMINMAX
#include <windows.h>

#include <stdlib.h> // For 'abs'

#if !defined (APIENTRY)
#define APIENTRY FAR PASCAL
#endif

HWND    hwndTools, hwndToolText, hwndToolCombo, hwndToolButton, hwndMain;
HFONT   hfontTools=0;
TEXTMETRIC tmToolFont;
int     dyTools = 0, cxToolBorder, cyToolBorder, cntToolCtrls = 0, dyCombo;
int     xCurrent = 10;
HBRUSH  hbrBtnFace=0, hbrWindow=0;
HANDLE  hInst=0;

#define MAXCTRLS 25
#define TC_SPACE 0
#define TC_LABEL 1
#define TC_COMBO 2
#define TC_BUTTON 3
typedef struct _tagTools {
                        HWND    hwnd;
                        WORD    wType;
                        int     iWidth, iHeight;
                        HICON   hIcon;
                } Tools;
Tools  toolCtrl[MAXCTRLS];

LONG APIENTRY ToolsProc       (HWND, UINT, UINT, LONG);
LONG APIENTRY MyComboProc     (HWND, UINT, UINT, LONG);

BOOL InitToolBar (HANDLE hInstance)
{
        WNDCLASS    wndclass;

        hbrBtnFace = CreateSolidBrush(GetSysColor(COLOR_BTNFACE));
        hbrWindow = CreateSolidBrush(GetSysColor(COLOR_WINDOW));

        wndclass.style         = CS_HREDRAW | CS_VREDRAW;
        wndclass.lpfnWndProc   = (WNDPROC)ToolsProc;
        wndclass.cbClsExtra    = 0;
        wndclass.cbWndExtra    = 0;
        wndclass.hInstance      = hInstance;
        wndclass.hIcon     = NULL;
        wndclass.hbrBackground = hbrBtnFace;
        wndclass.hCursor       = LoadCursor (NULL, IDC_ARROW);
        wndclass.lpszMenuName  = NULL;
        wndclass.lpszClassName = "SamplerTools";

        if (!RegisterClass (&wndclass))
                return FALSE;
}

BOOL CreateToolBar (HWND hwnd, HANDLE hInstance, int iId)
{
        HWND hwndTmp;
        RECT rect;

#if defined (WIN32)
        hInst = (HANDLE)GetWindowLong (hwnd, GWL_HINSTANCE);
#else
        hInst = GetWindowWord (hwnd, GWW_HINSTANCE);
#endif

        if (hbrBtnFace==0) {
                hbrBtnFace = CreateSolidBrush(GetSysColor(COLOR_BTNFACE));
        }
        if (hbrWindow==0) {
                hbrWindow = CreateSolidBrush(GetSysColor(COLOR_WINDOW));
        }


        cxToolBorder = GetSystemMetrics (SM_CXBORDER);
        cyToolBorder = GetSystemMetrics (SM_CYBORDER);

        hwndTools = CreateWindow ("SamplerTools", "SamplerTools",
                WS_CHILD | WS_CLIPSIBLINGS | WS_BORDER | WS_VISIBLE,
                0, 0, 0, 0,
        hwnd, (HMENU)iId, hInst, NULL);
        if (!hwndTools) {
                return FALSE;
        }

        /* Lets find out how big a combo box is... */
        hwndTmp = CreateWindow ("COMBOBOX", "Combo",
                        WS_CHILD | WS_VISIBLE | CBS_DROPDOWNLIST,
                        0, 0, 0, 0,
                        hwndTools, NULL, hInst, NULL);
        if (hwndTmp) {
                SendMessage (hwndTmp, WM_SETFONT, (UINT)hfontTools, MAKELONG (TRUE, 0));
                GetClientRect (hwndTmp, &rect);
                dyCombo = rect.bottom - rect.top;
                DestroyWindow (hwndTmp);
        } else {
                dyCombo = 30; // Just for a default value
        }

        hwndMain = hwnd; // So we can pass WM_CONTROL messages back to the master parent

        return TRUE;

        hInstance; // unreferenced formal parameter
}

int ToolBarHeight (HWND hwnd)
{
        RECT rect;
        GetClientRect (hwndTools, &rect);
        return rect.bottom-rect.top;

        hwnd; //unreferenced formal parameter
}

BOOL AdjustToolBar (HWND hwnd)
{
        RECT rect;

        GetClientRect (hwnd, &rect);
        MoveWindow (hwndTools,
                rect.left-cxToolBorder,
                rect.top - cyToolBorder,
                rect.right - rect.left + (cxToolBorder*2),
                dyTools,
                TRUE);
        return TRUE;
}

void UpdatePositions (void)
{
        int i, x, y, dx, dy, cnt;

        x = 10;
        for (i=0; i<cntToolCtrls; i++) {
                switch (toolCtrl[i].wType) {
                        case TC_SPACE:
                                dx = toolCtrl[i].iWidth;
                                break;
                        case TC_LABEL:
                                dy = toolCtrl[i].iHeight;
                                y = (dyTools/2) - (dy/2) - 1;
                                dx = toolCtrl[i].iWidth;
                                break;
                        case TC_COMBO:
                                dy = toolCtrl[i].iHeight;
                                y = (dyTools/2) - (dy/2) - 1;
                                dx = toolCtrl[i].iWidth;
                                cnt = (int)SendMessage (toolCtrl[i].hwnd, CB_GETCOUNT, (UINT)0, (LONG)0);
                                if (cnt > 5) cnt = 5;
                                dy = dy * cnt;
                                break;
                        case TC_BUTTON:
                                dy = toolCtrl[i].iHeight;
                                y = (dyTools/2) - (dy/2) - 1;
                                dx = toolCtrl[i].iWidth;
                                break;
                        default:
                                dy = toolCtrl[i].iHeight;
                                y = (dyTools/2) - (dy/2) - 1;
                                dx = toolCtrl[i].iWidth;
                                break;
                }
                if (toolCtrl[i].wType != TC_SPACE) {
                        MoveWindow (toolCtrl[i].hwnd, x, y, dx, dy, FALSE);
                }
                x += dx;
        }
        if (hwndTools) {
                UpdateWindow (hwndTools);
        }
}

BOOL AddToolSpace (int iWidth, int iHeight)
{
        if (cntToolCtrls >= MAXCTRLS) return FALSE;
        toolCtrl[cntToolCtrls].hwnd = 0;
        toolCtrl[cntToolCtrls].wType = TC_SPACE;
        toolCtrl[cntToolCtrls].iWidth = iWidth;
        toolCtrl[cntToolCtrls].iHeight = iHeight;
        if ((toolCtrl[cntToolCtrls].iHeight + (6*cyToolBorder)) > dyTools) {
                dyTools = (toolCtrl[cntToolCtrls].iHeight + (6*cyToolBorder));
        }
        UpdatePositions();
        cntToolCtrls++;
        return TRUE;
}

HWND AddToolLabel (HANDLE hInst, int iId, LPSTR szLabel, int iWidth, DWORD dwStyle)
{
        HDC hdc;

        if (cntToolCtrls >= MAXCTRLS) return (HWND)0; // No room left in our fixed array

        toolCtrl[cntToolCtrls].hwnd = CreateWindow ("STATIC", szLabel,
                WS_CHILD | WS_VISIBLE | dwStyle,
                0, 0, 0, 0,
                hwndTools, (HMENU)iId, hInst, NULL);

        if (!toolCtrl[cntToolCtrls].hwnd) return (HWND)0; // CreateWindow failed for some reason

        SendMessage (toolCtrl[cntToolCtrls].hwnd, WM_SETFONT, (UINT)hfontTools, MAKELONG (TRUE, 0));
        toolCtrl[cntToolCtrls].wType = TC_LABEL;

        hdc = GetDC (hwndTools);
        if (iWidth < 0) {
                toolCtrl[cntToolCtrls].iWidth = tmToolFont.tmAveCharWidth * abs(iWidth);
        } else if (iWidth == 0) {
#if defined (WIN32)
                SIZE size;
                GetTextExtentPoint (hdc, szLabel, lstrlen(szLabel), &size);
                toolCtrl[cntToolCtrls].iWidth = size.cx;
#else
                toolCtrl[cntToolCtrls].iWidth = LOWORD(GetTextExtent (hdc, szLabel, lstrlen(szLabel)));
#endif
        } else {
                toolCtrl[cntToolCtrls].iWidth = iWidth;
        }
        toolCtrl[cntToolCtrls].iHeight = tmToolFont.tmHeight;

        if ((toolCtrl[cntToolCtrls].iHeight + (6*cyToolBorder)) > dyTools) {
                dyTools = (toolCtrl[cntToolCtrls].iHeight + (6*cyToolBorder));
        }

        ReleaseDC (hwndTools, hdc);
        UpdatePositions();
        return toolCtrl[cntToolCtrls++].hwnd;
}

HWND AddToolCombo (HANDLE hInst, int iId, int iWidth, DWORD dwStyle)
{

        if (cntToolCtrls >= MAXCTRLS) return (HWND)0; // No room left in our fixed array

        if (dwStyle==0) dwStyle = CBS_DROPDOWNLIST;
        toolCtrl[cntToolCtrls].hwnd = CreateWindow ("COMBOBOX", "",
                WS_CHILD | WS_VISIBLE | dwStyle,
                0, 0, 0, 0,
                hwndTools, (HMENU)iId, hInst, NULL);

        if (!toolCtrl[cntToolCtrls].hwnd) return (HWND)0; // CreateWindow failed for some reason

        SendMessage (toolCtrl[cntToolCtrls].hwnd, WM_SETFONT, (UINT)hfontTools, MAKELONG (TRUE, 0));
        toolCtrl[cntToolCtrls].wType = TC_COMBO;

        if (iWidth < 0) {
                toolCtrl[cntToolCtrls].iWidth = tmToolFont.tmAveCharWidth * abs(iWidth);
        } else if (iWidth == 0) {
                toolCtrl[cntToolCtrls].iWidth = tmToolFont.tmAveCharWidth * 15; // just a default width
        } else {
                toolCtrl[cntToolCtrls].iWidth = iWidth;
        }
        toolCtrl[cntToolCtrls].iHeight = dyCombo;

        if ((toolCtrl[cntToolCtrls].iHeight + (6*cyToolBorder)) > dyTools) {
                dyTools = (toolCtrl[cntToolCtrls].iHeight + (6*cyToolBorder));
        }

        UpdatePositions();
        return toolCtrl[cntToolCtrls++].hwnd;
}


HWND AddToolButton (HANDLE hInst, int iId, LPSTR szLabel, int iWidth, int iHeight, DWORD dwStyle)
{
        HDC hdc;

        if (cntToolCtrls >= MAXCTRLS) return (HWND)0; // No room left in our fixed array

        if (dwStyle == 0) dwStyle = BS_PUSHBUTTON;
        toolCtrl[cntToolCtrls].hwnd = CreateWindow ("BUTTON", szLabel,
                WS_CHILD | WS_VISIBLE | dwStyle,
                0, 0, 0, 0,
                hwndTools, (HMENU)iId, hInst, NULL);

        if (!toolCtrl[cntToolCtrls].hwnd) return (HWND)0; // CreateWindow failed for some reason

        SendMessage (toolCtrl[cntToolCtrls].hwnd, WM_SETFONT, (UINT)hfontTools, MAKELONG (TRUE, 0));
        toolCtrl[cntToolCtrls].wType = TC_BUTTON;

        hdc = GetDC (hwndTools);
        SelectObject (hdc, hfontTools);
        if (iWidth < 0) {
                toolCtrl[cntToolCtrls].iWidth = tmToolFont.tmAveCharWidth * abs(iWidth);
                toolCtrl[cntToolCtrls].iWidth += (6*cxToolBorder);
        } else if (iWidth == 0) {
#if defined (WIN32)
                SIZE size;
                GetTextExtentPoint (hdc, szLabel, lstrlen(szLabel), &size);
                toolCtrl[cntToolCtrls].iWidth = size.cx;
#else
                toolCtrl[cntToolCtrls].iWidth = LOWORD(GetTextExtent (hdc, szLabel, lstrlen(szLabel)));
#endif
                toolCtrl[cntToolCtrls].iWidth += (6*cxToolBorder);
        } else {
                toolCtrl[cntToolCtrls].iWidth = iWidth;
        }
        if (iHeight < 0) {
                toolCtrl[cntToolCtrls].iHeight = tmToolFont.tmHeight;
                toolCtrl[cntToolCtrls].iHeight += (6*cyToolBorder);
        } else if (iHeight==0) {
                toolCtrl[cntToolCtrls].iHeight = dyTools - (6*cyToolBorder);
        } else {
                toolCtrl[cntToolCtrls].iHeight = iHeight;
        }

        if ((toolCtrl[cntToolCtrls].iHeight + (6*cyToolBorder)) > dyTools) {
                dyTools = (toolCtrl[cntToolCtrls].iHeight + (6*cyToolBorder));
        }

        if (dwStyle & BS_OWNERDRAW) {
                toolCtrl[cntToolCtrls].hIcon = LoadIcon (hInst, szLabel);
        } else {
                toolCtrl[cntToolCtrls].hIcon = NULL;
        }

        ReleaseDC (hwndTools, hdc);
        UpdatePositions();
        return toolCtrl[cntToolCtrls++].hwnd;
}


BOOL DestroyToolBar (void)
{
        return DeleteObject (hbrBtnFace);
}

void DrawButton (HDC hdc, RECT rect, BOOL bDown, HICON hIcon)
{
        HBRUSH  hBrush, hbrFrame, hbrFace, hbrHilite, hbrShadow;
        RECT    border;
        int     i;

        hbrFrame = CreateSolidBrush(GetSysColor(COLOR_WINDOWFRAME));
        hbrFace = CreateSolidBrush(GetSysColor(COLOR_BTNFACE));
        hbrHilite = CreateSolidBrush(GetSysColor(COLOR_WINDOW));
        hbrShadow = CreateSolidBrush(GetSysColor(COLOR_BTNSHADOW));

        FillRect (hdc, &rect, hbrFace);

        if (hIcon) {
                if (bDown) {
                        DrawIcon (hdc, rect.left + (4*cyToolBorder), rect.top + (4*cyToolBorder), hIcon);
                } else {
                        DrawIcon (hdc, rect.left + (3*cyToolBorder), rect.top + (3*cyToolBorder), hIcon);
                }
        }

        hBrush = hbrFrame;
        border = rect; border.bottom = border.top + cyToolBorder;
        FillRect (hdc, &border, hBrush);
        border = rect; border.right = border.left + cxToolBorder;
        FillRect (hdc, &border, hBrush);
        border = rect; border.top = border.bottom - cyToolBorder;
        FillRect (hdc, &border, hBrush);
        border = rect; border.left = border.right - cxToolBorder;
        FillRect (hdc, &border, hBrush);

        for (i= 0; i<2; i++) {
                InflateRect (&rect, -cxToolBorder, -cyToolBorder);
                hBrush = (bDown?hbrShadow:hbrHilite);
                border = rect; border.bottom = border.top + cyToolBorder;
                FillRect (hdc, &border, hBrush);
                border = rect; border.right = border.left + cxToolBorder;
                FillRect (hdc, &border, hBrush);
                if (!bDown) {
                        hBrush = hbrShadow;
                        border = rect; border.top = border.bottom - cyToolBorder;
                        FillRect (hdc, &border, hBrush);
                        border = rect; border.left = border.right - cxToolBorder;
                        FillRect (hdc, &border, hBrush);
                }
        }

        DeleteObject (hbrFrame);
        DeleteObject (hbrFace);
        DeleteObject (hbrHilite);
        DeleteObject (hbrShadow);

}


LONG APIENTRY ToolsProc (HWND hwnd, UINT msg, UINT wParam, LONG lParam)
{
        HDC          hdc;
        PAINTSTRUCT      ps;
        int          iType, idCtrl, msgCtrl, i;
        RECT         rect, border;
        COLORREF         clrColor;
        HWND         hwndCtl;
        LONG         lStyle;
        HBRUSH       hBrush;
        LPDRAWITEMSTRUCT lpdi;
        HICON        hIcon;
        LOGFONT     lfTmp ={0,0,0,0,0,0,0,0,0,0,0,0,0,0};

        switch (msg) {
                case WM_CREATE:
                        lfTmp.lfHeight = 14;
                        lfTmp.lfPitchAndFamily = VARIABLE_PITCH | FF_SWISS;
                        hfontTools = CreateFontIndirect (&lfTmp);
                        if (!hfontTools) {
                                MessageBox (GetFocus(),
                                        "Unable to get an unnamed variable pitch swiss font", 
                                        "Tool Bar CreateFont Error",
                                         MB_OK);
                                hfontTools = CreateFont(14, 0, 0, 0, 0, 0, 0, 0,
                                        0, 0, 0, 0,
                                        VARIABLE_PITCH | FF_SWISS, "Arial");
                        }

                        if (!hfontTools) {
                                MessageBox (GetFocus(), "Failed To Create Font", "StatusProc", MB_OK);
                        }
                        hdc = GetDC (hwnd);
                        SelectObject (hdc, hfontTools);
                        GetTextMetrics (hdc, &tmToolFont);
                        ReleaseDC (hwnd, hdc);
                        return DefWindowProc (hwnd, msg, wParam, lParam);

                case WM_DESTROY:
                        if (hfontTools) {
                                DeleteObject (hfontTools);
                        }
                        break;

                case WM_SIZE:
                        UpdatePositions();
                        break;

#if defined (WIN32)
                case WM_CTLCOLORLISTBOX:
                case WM_CTLCOLOREDIT:
                case WM_CTLCOLORSTATIC:
                case WM_CTLCOLORBTN:
                case WM_CTLCOLORDLG:
                case WM_CTLCOLORMSGBOX:
                case WM_CTLCOLORSCROLLBAR:
                        iType   = msg - WM_CTLCOLORMSGBOX;
                        hdc     = (HDC)wParam;
                        hwndCtl = (HWND)lParam;
#else
                case WM_CTLCOLOR:
                        hdc = wParam;
                        hwndCtl = LOWORD(lParam);
                        iType = HIWORD (lParam);
#endif
                        switch (iType) {
                                case CTLCOLOR_EDIT: //Edit control
                                        clrColor = GetSysColor (COLOR_BTNFACE);
                                        hBrush = hbrWindow;
                                        break;

                                case CTLCOLOR_LISTBOX: //List-box control
                                        lStyle = GetWindowLong (hwndCtl, GWL_STYLE);
                                        if (lStyle & CBS_SIMPLE) {
                                                clrColor = GetSysColor (COLOR_WINDOW);
                                                hBrush = hbrWindow;
                                        } else {
                                                clrColor = GetSysColor (COLOR_BTNFACE);
                                                hBrush = hbrBtnFace;
                                        }
                                        break;

                                case CTLCOLOR_STATIC:
                                        clrColor = GetSysColor (COLOR_BTNFACE);
                                        hBrush = hbrBtnFace;
                                        break;

                                case CTLCOLOR_BTN:
                                        clrColor = GetSysColor (COLOR_BTNFACE);
                                        hBrush = hbrBtnFace;
                                        break;

                                case CTLCOLOR_SCROLLBAR:
                                case CTLCOLOR_DLG:
                                case CTLCOLOR_MSGBOX:
                                default:
                                        return DefWindowProc (hwnd, msg, wParam, lParam);
                        }
                        SetBkColor(hdc, clrColor);
                return (LONG)hBrush;

                case WM_PAINT:
                        hdc = BeginPaint (hwnd, &ps);
                        GetClientRect (hwnd, &rect);

                        /* Shade the top of the bar white */
                        hBrush = CreateSolidBrush(GetSysColor(COLOR_WINDOW));
                        border = rect;
                        border.bottom = border.top + cyToolBorder;
                        FillRect (hdc, &border, hBrush);
                        DeleteObject (hBrush);

                        /* Shade the bottom of the bar dark gray */
                        hBrush = CreateSolidBrush(GetSysColor(COLOR_BTNSHADOW));
                        border = rect;
                        border.top = border.bottom - cyToolBorder;
                        FillRect (hdc, &border, hBrush);
                        DeleteObject (hBrush);

                        EndPaint (hwnd, &ps);

                        return DefWindowProc (hwnd, msg, wParam, lParam);

                case WM_DRAWITEM: // Indicates that an owner-draw control needs to be redrawn.
                        lpdi = (LPDRAWITEMSTRUCT)lParam;

                        switch (lpdi->itemAction) {
                                // handle normal drawing of button, but check if its selected or focus
                                case ODA_SELECT:
                                case ODA_DRAWENTIRE:
                                        // handle button pressed down select state -- button down bitmap
                                        //   text is right & down 2 pixels
                                                                                hIcon = NULL;
                                        for (i=0; i< cntToolCtrls; i++) {
                                                if (toolCtrl[i].hwnd == lpdi->hwndItem) {
                                                        hIcon = toolCtrl[i].hIcon;
                                                }
                                        }
                                        if (lpdi->itemState & ODS_SELECTED) {
                                                DrawButton (lpdi->hDC,lpdi->rcItem, TRUE, hIcon);
                                        } else { // not selected -- button up; text is in normal position
                                                DrawButton (lpdi->hDC,lpdi->rcItem, FALSE, hIcon);
                                        }
                                        return TRUE;
                        }
                        break;

                case WM_COMMAND:
#if defined (WIN32)
                        idCtrl = LOWORD(wParam);
                        msgCtrl = HIWORD(wParam);
                        hwndCtl = (HWND)lParam;
#else
                        idCtrl = wParam;
                        msgCtrl = HIWORD (lParam);
                        hwndCtl = LOWORD (lParam);
#endif

                        if (GetWindowLong (hwndCtl, GWL_STYLE) & BS_OWNERDRAW) {
                                if (msgCtrl == BN_DOUBLECLICKED) {
                                        PostMessage (hwndCtl, WM_LBUTTONDOWN, 0, 0);
                                        return TRUE;
                                }
                        }
                        PostMessage (hwndMain, msg, wParam, lParam);
                        return DefWindowProc (hwnd, msg, wParam, lParam);


                default:
                        return DefWindowProc (hwnd, msg, wParam, lParam);
        }
        return 0L;
}
