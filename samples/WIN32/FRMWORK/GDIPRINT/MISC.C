// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
// PARTICULAR PURPOSE.
//
// Copyright (C) 1993 - 1995  Microsoft Corporation.  All Rights Reserved.
//
//  MODULE:   misc.c
//
//  PURPOSE:  Contains all helper functions "global" to the application.
//
//  FUNCTIONS:
//    CenterWindow     - Center one window over another.
//    SetWindowTitle   - Sets the title of the application window.
//    InitDIBSection   - Initializes the bitmap surface of the current DIB
//                       section and paints the display.
//    RemoveDIBSection - Removes the current DIB section.
//
//  COMMENTS:
//
//

#include <windows.h>       // required for all Windows applications
#include <commctrl.h>      // prototypes and structure for the common controls.

#include "globals.h"       // prototypes specific to this application
#include "toolbar.h"
#include "resource.h"
 
//
//  FUNCTION: CenterWindow(HWND, HWND)
//
//  PURPOSE:  Center one window over another.
//
//  PARAMETERS:
//    hwndChild - The handle of the window to be centered.
//    hwndParent- The handle of the window to center on.
//
//  RETURN VALUE:
//
//    TRUE  - Success
//    FALSE - Failure
//
//  COMMENTS:
//
//    Dialog boxes take on the screen position that they were designed
//    at, which is not always appropriate. Centering the dialog over a
//    particular window usually results in a better position.
//

BOOL CenterWindow(HWND hwndChild, HWND hwndParent)
{
    RECT    rcChild, rcParent;
    int     cxChild, cyChild, cxParent, cyParent;
    int     cxScreen, cyScreen, xNew, yNew;
    HDC     hdc;

    // Get the Height and Width of the child window
    GetWindowRect(hwndChild, &rcChild);
    cxChild = rcChild.right - rcChild.left;
    cyChild = rcChild.bottom - rcChild.top;

    // Get the Height and Width of the parent window
    GetWindowRect(hwndParent, &rcParent);
    cxParent = rcParent.right - rcParent.left;
    cyParent = rcParent.bottom - rcParent.top;

    // Get the display limits
    hdc = GetDC(hwndChild);
    cxScreen = GetDeviceCaps(hdc, HORZRES);
    cyScreen = GetDeviceCaps(hdc, VERTRES);
    ReleaseDC(hwndChild, hdc);

    // Calculate new X position, then adjust for screen
    xNew = rcParent.left + ((cxParent - cxChild) / 2);
    if (xNew < 0)
    {
        xNew = 0;
    }
    else if ((xNew + cxChild) > cxScreen)
    {
        xNew = cxScreen - cxChild;
    }

    // Calculate new Y position, then adjust for screen
    yNew = rcParent.top  + ((cyParent - cyChild) / 2);
    if (yNew < 0)
    {
        yNew = 0;
    }
    else if ((yNew + cyChild) > cyScreen)
    {
        yNew = cyScreen - cyChild;
    }

    // Set it, and return
    return SetWindowPos(hwndChild,
                        NULL,
                        xNew, yNew,
                        0, 0,
                        SWP_NOSIZE | SWP_NOZORDER);
}


//
//  FUNCTION: SetWindowTitle(HWND, LPSTR)
//
//  PURPOSE: Sets the title of the application window
//
//  PARAMETERS:
//    hwnd         - The window handle.
//    lpszFileName - Pointer to file name string.
//
//  RETURN VALUE:
//    none.
//
//  COMMENTS:
//
//

void SetWindowTitle(HWND hwnd, LPSTR lpszFileName)
{   
    int nChars;
    char szTitle[MAX_PATH];   
    LPBITMAPINFOHEADER lpbih;
    
    if (hDIBInfo)
        lpbih = (LPBITMAPINFOHEADER)GlobalLock(hDIBInfo);
               
    nChars = lstrlen(lpszFileName);
    
    if (nChars && lpbih)
    {   
        char szChanges[2] = { 0, 0 };
        
        if (fChanges)
            szChanges[0] = '*';
        
        // a DIB Section exists
        wsprintf(szTitle, 
                 "%s - %s (%ld x %ld x %d BPP)%s",
                 szAppName, 
                 lpszFileName,
                 lpbih->biWidth,
                 lpbih->biHeight,
                 lpbih->biBitCount,
                 szChanges);    
      
        // save current file name
        lstrcpy((LPSTR)szCurrentFile, lpszFileName);     
        
        GlobalUnlock(hDIBInfo);
    }    
    else
        // no DIB Section so just show application description text
        wsprintf(szTitle, "%s", SZDESCRIPTION);
                  
    SetWindowText(hwnd, szTitle);   
}     

 
//
//  FUNCTION: InitDIBSection(HWND)
//
//  PURPOSE: Initialize the bitmap surface and paint the display.
//
//  PARAMETERS:
//    hwnd  - The window displaying the bitmap.
//
//  RETURN VALUE:
//    none.
//
//  COMMENTS:
//
//
                        
void InitDIBSection(HWND hwnd)
{
    HDC hdc, hdcMem;
    HBITMAP hbmOld;
    HPALETTE hpalOld = NULL;
    HPALETTE hpalOldMem = NULL;
    LPBITMAPINFOHEADER lpbih;
    
    if (!hwnd || !hDIBInfo || !hBitmap)
        return;
        
    lpbih = (LPBITMAPINFOHEADER)GlobalLock(hDIBInfo);
    if (!lpbih)
        return;
          
    hdc = GetDC(hwnd);
    hdcMem = CreateCompatibleDC(hdc);  
    hbmOld = SelectObject(hdcMem, hBitmap);

    if (hPalette)
    {
        hpalOld = SelectPalette(hdc, hPalette, FALSE);
        RealizePalette(hdc);
        hpalOldMem = SelectPalette(hdcMem, hPalette, FALSE);
    }

    PatBlt(hdcMem, 0, 0, lpbih->biWidth, lpbih->biHeight, WHITENESS);    
    PatBlt(hdc,    0, 0, lpbih->biWidth, lpbih->biHeight, WHITENESS);   

    if (hpalOldMem)
        SelectPalette(hdcMem, hpalOldMem, FALSE);
    if (hpalOld)
        SelectPalette(hdc, hpalOld, FALSE);

    SelectObject(hdcMem, hbmOld);
    DeleteDC(hdcMem);
    ReleaseDC(hwnd, hdc);
    GlobalUnlock(hDIBInfo);
}

      
//
//  FUNCTION: RemoveDIBSection()
//
//  PURPOSE: Removes the current DIB section and resets global variables
//
//  PARAMETERS:
//    none.
//
//  RETURN VALUE:
//    none.
//
//  COMMENTS:
//
//

void RemoveDIBSection()
{
    if (hDIBInfo) 
    {
        // free old DIB header
        GlobalFree(hDIBInfo);
        hDIBInfo = NULL;
    }
  
    if (hBitmap)
    {
        // remove existing DIB section 
        DeleteObject(hBitmap); 
        hBitmap = NULL;
    }
    
    // bits are gone
    lpvBits = NULL;        
                        
    if (hPalette)                        
    {    
        // remove existing palette object
        DeleteObject(hPalette);
        hPalette = NULL;
    }
             
    // reset the drawing objects to defaults
    InitDrawObjects();
             
    // if there were edits, they are gone
    fChanges = FALSE;  
    
    // update status of menu and toolbar  
    EnableMenuItem(hMenu, IDM_FILESAVE, MF_DISABLED);
    SendMessage(hWndToolbar, TB_ENABLEBUTTON, IDM_FILESAVE, MAKELONG(FALSE, 0));  
    EnableMenuItem(hMenu, IDM_FILESAVEAS, MF_DISABLED);    
    EnableMenuItem(hMenu, IDM_FILECLOSE, MF_DISABLED);
    EnableMenuItem(hMenu, IDM_FILEPRINT, MF_DISABLED); 
    SendMessage(hWndToolbar, TB_ENABLEBUTTON, IDM_FILEPRINT, MAKELONG(FALSE, 0));  
    EnableMenuItem(hMenu, IDM_CLEAR, MF_DISABLED);
}
                              
