// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
// PARTICULAR PURPOSE.
//
// Copyright (C) 1993-1995  Microsoft Corporation.  All Rights Reserved.
//
//  MODULE:   misc.c
//
//  PURPOSE:  Contains all helper functions "global" to the application.
//
//  FUNCTIONS:
//    CenterWindow -        Center one window over another.
//
//  COMMENTS:
//
//

#include <windows.h>            // required for all Windows applications
#include <windowsx.h>
#include "globals.h"            // prototypes specific to this application
#include <commdlg.h>
#include "hooks.h"

extern HWND hWndLBoxCtl;

COLORREF crLBoxCtl = RGB( 0, 0, 0 );  // initialize the color to black

//
//  FUNCTION: CenterWindow(HWND, HWND)
//
//  PURPOSE:  Center one window over another.
//
//  PARAMETERS:
//    hwndChild  - The handle of the window to be centered.
//    hwndParent - The handle of the window to center on.
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
    int     wChild, hChild, wParent, hParent;
    int     wScreen, hScreen, xNew, yNew;
    HDC     hdc;

    // Get the Height and Width of the child window
    GetWindowRect(hwndChild, &rcChild);
    wChild = rcChild.right - rcChild.left;
    hChild = rcChild.bottom - rcChild.top;

    // Get the Height and Width of the parent window
    GetWindowRect(hwndParent, &rcParent);
    wParent = rcParent.right - rcParent.left;
    hParent = rcParent.bottom - rcParent.top;

    // Get the display limits
    hdc = GetDC(hwndChild);
    wScreen = GetDeviceCaps(hdc, HORZRES);
    hScreen = GetDeviceCaps(hdc, VERTRES);
    ReleaseDC(hwndChild, hdc);

    // Calculate new X position, then adjust for screen
    xNew = rcParent.left + ((wParent - wChild) /2);
    if (xNew < 0)
    {
         xNew = 0;
    }
    else if ((xNew+wChild) > wScreen)
    {
        xNew = wScreen - wChild;
    }

    // Calculate new Y position, then adjust for screen
    yNew = rcParent.top  + ((hParent - hChild) /2);
    if (yNew < 0)
    {
        yNew = 0;
    }
    else if ((yNew+hChild) > hScreen)
    {
        yNew = hScreen - hChild;
    }

    // Set it, and return
    return SetWindowPos(hwndChild,
                        NULL,
                        xNew, yNew,
                        0, 0,
                        SWP_NOSIZE | SWP_NOZORDER);
}




//
//  FUNCTION: ChooseNewFont( HWND )
//
//  PURPOSE:  Allow the user to select a new font for the listbox and header
//
//  PARAMETERS:
//    HWND - hwnd  owner window of the dialog
//
//  RETURN VALUE:
//    NONE
//
//  COMMENTS:
//
//    This function sets up and calls the choose font common dialog box.  
//    When the user selects OK, the new font and color are inserted into
//    the GlobalOptions structure.
//

void ChooseNewFont(HWND hwnd)
{
    static CHOOSEFONT cf;          // common dialog box structure
    static LOGFONT    lf;          // logical-font structure
    static HFONT      hfont;       // new logical-font handle
  
    // Initialize the necessary members.
    cf.lStructSize = sizeof (CHOOSEFONT);
    cf.hwndOwner = hwnd;                    // owner window
    cf.lpLogFont = &lf;
    cf.Flags = CF_SCREENFONTS | CF_EFFECTS
               | CF_INITTOLOGFONTSTRUCT; // only screen fonts and effects
    cf.rgbColors = crLBoxCtl;     // default to black
    cf.nFontType = SCREEN_FONTTYPE;

    // Display the dialog box, allow the user to choose a font, and render
    // the text in the window with that selection
    if (ChooseFont (&cf))
    {
        // create the new font and store it and the new color in the 
        // global options
        hfont = CreateFontIndirect (cf.lpLogFont); 
        crLBoxCtl = cf.rgbColors;
        lf = * (cf.lpLogFont);

        SendMessage( hWndLBoxCtl, WM_SETFONT, (WPARAM)hfont, MAKELPARAM( TRUE, 0 ));  
    }
}
