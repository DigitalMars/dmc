// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
// PARTICULAR PURPOSE.
//
// Copyright (C) 1993-1995  Microsoft Corporation.  All Rights Reserved.
//
//  MODULE:   exports.c
//
//  PURPOSE:   Contains the DLL's exported functions.
//
//  FUNCTIONS:
//    RemoveMouseHook  - removes mouse hook
//    InstallMouseHook - installs mouse hook
//
//  COMMENTS:
//    These functions are simple and are meant to demonstrate different
//    calling conventions and parameters.
//
//    Since DLLs cannot control what types of applications call them, they
//    should be able to work properly with multithreaded applications.
//    This means using critical sections and mutexs as needed.  Do not
//    expect that an application will call DLL functions from within a
//    critical section or mutex!
//


#include <windows.h>
#include <stdio.h>
#include "dllglob.h"



// USER-Defined Message for the BrowseDlg to update its static control text.
// This is defined in globals.h too. It's up to you to modularize.
#define WM_UPDATETEXT      WM_USER+500
#define WM_PASSHWNDBROWSE  WM_USER+501


// Global variable definitions
extern  HANDLE  hMemMapped;
extern  PHSAVE  phSave;

HHOOK     ghMouseHook, ghCWPHook;
extern    HINSTANCE hInstDLL;


//
// Functions specific to this module.
//

LRESULT BrowseWindow(WPARAM, LPARAM);
void    HiliteWindow(HWND);


//
//  FUNCTION: RemoveMouseHook
//
//  PURPOSE:
//    Removes mouse hook installed
//
//  PARAMETERS:
//    hWnd       -  Handle of the window that is to be parent of message box
//    lpszMsg    -  Character string containing a message to display.
//
//  RETURN VALUE:
//    Always returns TRUE
//

DLLEXPORT BOOL WINAPI RemoveMouseHook (HWND hWnd)
{

    if(!UnhookWindowsHookEx(ghMouseHook))
        MessageBox(NULL, 
                   "MOUSE HOOK NOT REMOVED",
                   (LPSTR)NULL, 
                    MB_OK);


    return TRUE;
}


//
//  FUNCTION: InstallMouseHook(HWND)
//
//  PURPOSE:
//
//
//  PARAMETERS:
//    hWnd       -  Handle of the window that is to be parent of message box
//
//  RETURN VALUE:
//
//  COMMENTS:
//

DLLEXPORT BOOL WINAPI InstallMouseHook (HWND hWnd)
{   
    ghMouseHook =   SetWindowsHookEx(WH_MOUSE,
                                    (HOOKPROC)MouseHookCallback, 
                                     hInstDLL, 
                                    (DWORD)NULL);
    if(!ghMouseHook)
    {
        MessageBox(NULL, 
                   "HOOK NOT INSTALLED", 
                   "ERROR", 
                   MB_OK | MB_ICONSTOP);
        return FALSE;
    }
    return TRUE;
}


//
//  FUNCTION: MouseHookCallBack(int, WPARAM, LPARAM)
//
//  PURPOSE:
//
//  PARAMETERS:
//
//  RETURN VALUE:
//
//

LRESULT CALLBACK MouseHookCallback (int nCode,
                                    WPARAM wParam,
                                    LPARAM lParam)
{
    LPMOUSEHOOKSTRUCT MouseHookParam = (MOUSEHOOKSTRUCT FAR *)lParam;
    DWORD  dwResult;


    if (nCode < 0)
        return CallNextHookEx(ghMouseHook, nCode, wParam, lParam);

    if (nCode >= 0) 
    { 
        dwResult = BrowseWindow(wParam, lParam);

        if (!dwResult)
            return CallNextHookEx(ghMouseHook, nCode, wParam, lParam);
        else
            return dwResult;
    }

    return CallNextHookEx(ghMouseHook, nCode, wParam, lParam);

}


//
//  FUNCTION: BrowseWindow(WPARAM, LPARAM)
//
//  PURPOSE:
//
//  PARAMETERS:
//     wParam: wPAram received from MouseHookCallback above
//     lParam: lParam received from MouseHookCallback above
//
//  RETURN VALUE:
//     1       if message is to get discarded
//     0       if message is to be passed on to next hook in the chain.
//
//


LRESULT BrowseWindow(WPARAM wParam, LPARAM lParam)
{
    HWND    hWnd, hWndChild;
    POINT   point;
    LPMOUSEHOOKSTRUCT MouseHookParam = (MOUSEHOOKSTRUCT FAR *)lParam;
    LRESULT lResult = 1;
    static BOOL bMovingBrowseDlg = FALSE;
    HWND hWndMain; 


    // Get a handle to the main window
    hWndMain = GetWindow(phSave->hDlg, GW_OWNER);


    if (bMovingBrowseDlg)
    {
        lResult = 0;
        return lResult;
    }



    switch(wParam)
    {
        case WM_SETCURSOR:

            MessageBeep(MB_OK);
            return (LRESULT) TRUE;
            break;
        
        case WM_MOUSEMOVE:
  //      MessageBeep(MB_OK);
 
        case WM_NCMOUSEMOVE:
        case WM_LBUTTONDOWN:
        case WM_NCLBUTTONDOWN:
        case WM_LBUTTONUP:
        case WM_NCLBUTTONUP:
        

        case WM_RBUTTONDOWN:
        case WM_NCRBUTTONDOWN:
        
            // Since #pragma does not work,
            // we're storing our hWndBrowse in a MMF. 

//            SetCursor(LoadCursor(NULL, IDC_ARROW));
            point.x = (LONG)MouseHookParam->pt.x;
            point.y = (LONG)MouseHookParam->pt.y;
            hWnd = WindowFromPoint(point);

            // WindowFromPoint() returns handle to the parent
            // if currently browsing on a child control that is
            // hidden/disabled/transparent
            for(;;)
            {
                ScreenToClient(hWnd, &point);
                hWndChild = ChildWindowFromPoint(hWnd, point);
                ClientToScreen(hWnd, &point);            
                if(hWndChild && hWndChild != hWnd)
                    hWnd = hWndChild;
                else
                    break;
            }

            // Don't allow browsing of the Browse Dlg or
            // any of the Browse Dlg controls
            // NOR on our main window or any of its children
            if ((hWnd != phSave->hDlg) &&
                (GetParent (hWnd) != phSave->hDlg) &&
                (hWnd != hWndMain) &&
                (GetParent (hWnd) != hWndMain))
            {


                // Also, don't hilite NOR update Browse Dlg text
                // if message was for same window
                if (phSave->hWndBrowse != hWnd)
                {

                    // This code is here to De-Hilite the previous 
                    // hWndBrowse.
                    // Thus, if hWndBrowse is NULL (as initialized),
                    // we don't need to de-hilite anything!
                    if (phSave->hWndBrowse)
                        HiliteWindow (phSave->hWndBrowse);                     
  
                    phSave->hWndBrowse = hWnd;
                    HiliteWindow (phSave->hWndBrowse);                     

                    // Update the Browse Window text
                    // that contains information about the hWndBrowse
                    PostMessage (phSave->hDlg, 
                                 WM_UPDATETEXT, 
                                 (WPARAM)(HWND)phSave->hWndBrowse, 
                                 0);
//            SetWindowLong (ghDlg, DWL_USER, (LONG)hWndBrowse);


                }

                lResult = 1;

                // if a window has finally been selected 
                // ie, with a DBLCLK message, de-hilite the window
                // and dismiss the dialog.
                if ((wParam == WM_RBUTTONDOWN) ||
                    (wParam == WM_NCRBUTTONDOWN))
                {

                    // Post a message to main application to
                    // pass the hWndBrowse selected.
                    PostMessage (GetWindow(phSave->hDlg, GW_OWNER), 
                                 WM_PASSHWNDBROWSE,
                                 (WPARAM)phSave->hWndBrowse, 
                                 (LPARAM)0);

                    HiliteWindow (phSave->hWndBrowse);                     
                    PostMessage (phSave->hDlg, 
                                 WM_COMMAND, 
                                 (WPARAM)IDCANCEL, 
                                 (LPARAM)0);


                }



            }
            else 
            {
                // Generally, if any of the controls inside the Browse Dialog,
                // discard the messsage.
                if (GetParent (hWnd) == phSave->hDlg)
                {

                    lResult = 0;


                    // Unhilite the current hWndBrowse just before
                    // dismissing the Browse Dialog on an IDCANCEL.
                    // Pass the message on an IDCANCEL: DONT DISCARD!

                    // ALSO, set hWndBrowse to NULL.
                    // This helps us distinguish when an IDCANCEL is rec'd
                    // whether a window was selected for browsing or not.
                    if ((wParam == WM_LBUTTONDOWN) && 
                        (GetWindowLong (hWnd, GWL_ID) == IDCANCEL))
                    {
                        HiliteWindow (phSave->hWndBrowse);                     
                        phSave->hWndBrowse = 0;
                        lResult = 0;

                        // Post a message to main application to
                        // pass the NULL hWndBrowse 
                        // to indicate that no hWnd was selected.
                        PostMessage (GetWindow(phSave->hDlg, GW_OWNER), 
                                     WM_PASSHWNDBROWSE,
                                     (WPARAM)phSave->hWndBrowse, 
                                     (LPARAM)0);


                    }
                }


                // Generally, if message is for the Browse Dialog,
                // discard the message.
                if (hWnd == phSave->hDlg)
                {
                    lResult = 1;


                    // If the Browse Dialog got clicked on its caption bar,
                    // move the window, and discard the message.
                    // Make sure hWndBrowse is first de-hilited and
                    // then re-hilited back after the window is moved!
                    // Otherwise, it doesn't repaint properly over the area
                    // where the Browse Dialog was moved.

                    if ((wParam == WM_NCLBUTTONDOWN) &&
                       (MouseHookParam->wHitTestCode == HTCAPTION))
                    {
                        bMovingBrowseDlg = TRUE;

    
                        HiliteWindow (phSave->hWndBrowse);                     
                        SendMessage (phSave->hDlg, 
                                     WM_SYSCOMMAND, 
                                     SC_MOVE, 
                                     lParam);
                        HiliteWindow (phSave->hWndBrowse);                     
   
                        bMovingBrowseDlg = FALSE;
                    }
                }
            }
            break;

        default:
            lResult = 1;
            break;
     } //end of switch

    return lResult;
}



//
//  FUNCTION: HiliteWindow(WPARAM, LPARAM)
//
//  PURPOSE:
//
//  PARAMETERS:
//
//  RETURN VALUE:
//
//

void  HiliteWindow(HWND hWndToHilite)
{
#define DINV   3
    HDC hDC;
    RECT rect;

    // No need to hilite anything if hWndtoHilite == NULL
    if (!hWndToHilite)
        return;

    hDC = GetWindowDC (hWndToHilite);
    GetWindowRect (hWndToHilite, &rect);
    OffsetRect (&rect, -rect.left, -rect.top);

    if (!IsRectEmpty (&rect))
    {
        PatBlt (hDC, 
                rect.left, 
                rect.top, 
                rect.right - rect.left, 
                DINV,
                DSTINVERT);
        PatBlt (hDC, 
                rect.left, 
                rect.bottom-DINV, 
                DINV,
                -(rect.bottom-rect.top-2*DINV),
                DSTINVERT);
        PatBlt (hDC, 
                rect.right-DINV, 
                rect.top+DINV, 
                DINV,   
                rect.bottom-rect.top-2*DINV,
                DSTINVERT);
        PatBlt (hDC, 
                rect.right,   
                rect.bottom-DINV, 
                -(rect.right-rect.left), 
                DINV, 
                DSTINVERT);
    }
    ReleaseDC (hWndToHilite, hDC);
}


