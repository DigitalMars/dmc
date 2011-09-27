// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
// PARTICULAR PURPOSE.
//
// Copyright (C) 1993-1995  Microsoft Corporation.  All Rights Reserved.
//
//  MODULE: optdlg.c
//
//  PURPOSE: To show the use of the fonts and colors common dialogs.
//
//
//  FUNCTIONS:
//    CmdFonts  - Change the current font using the fonts dialog box.
//    CmdColors - Change the current color using the colors dialog box.
//
//  COMMENTS:
//
//
//
//  SPECIAL INSTRUCTIONS: N/A
//

#include <windows.h>            // required for all Windows applications
#ifdef WIN16
#include "win16ext.h"           // required only for win16 applications
#include "commdlg.h"           
#endif
#include "globals.h"            // prototypes specific to this application

//
//  FUNCTION: CmdFonts(HWND, WORD, WORD, HWND)
//
//  PURPOSE: Change the current font using the fonts dialog box.
//
//  PARAMETERS:
//    hwnd     - The window handle.
//    wCommand - IDM_FONTS (Unused)
//    wNotify   - Notification number (unused)
//    hwndCtrl - NULL (Unused)
//
//  RETURN VALUE:
//    Always returns 0 - Command handled.
//
//  COMMENTS:
//
//

LRESULT CmdFonts(HWND hwnd, WORD wCommand, WORD wNotify, HWND hwndCtrl)
{
    CHOOSEFONT cf = {0};/* common dialog box structure */

    static LOGFONT lf = {0};         /* logical-font structure */
    static HFONT hfont = NULL;       /* new logical-font handle */
    static DWORD rgbColors = RGB(0, 0, 0);
    static BOOL  fInitLogFont = FALSE;

    // Initialize the ChooseFont structure.

    cf.lStructSize = sizeof (CHOOSEFONT);
    cf.hwndOwner = hwnd;
    cf.Flags =
        CF_SCREENFONTS |
        CF_EFFECTS |
        (fInitLogFont ? CF_INITTOLOGFONTSTRUCT : 0);
    cf.lpLogFont = &lf;
    cf.rgbColors = rgbColors;
    cf.nFontType = SCREEN_FONTTYPE;

    fInitLogFont = TRUE;

    // Display the dialog box, allow the user to choose a font, and
    //   render the text in the window with that selection.

    if (ChooseFont(&cf))
    {
        rgbColors = cf.rgbColors;
        SetCurrentFont(&lf, rgbColors);
        InvalidateRect(hwnd, NULL, TRUE);
    }
    return 0;
}


//
//  FUNCTION: CmdColors(HWND, WORD, WORD, HWND)
//
//  PURPOSE: Change the current color using the colors dialog box.
//
//  PARAMETERS:
//    hwnd     - The window handle.
//    wCommand - IDM_COLORS (Unused)
//    wNotify   - Notification number (unused)
//    hwndCtrl - NULL (Unused)
//
//  RETURN VALUE:
//    Always returns 0 - Command handled.
//
//  COMMENTS:
//
//

LRESULT CmdColors(HWND hwnd, WORD wCommand, WORD wNotify, HWND hwndCtrl)
{
    CHOOSECOLOR cc = {0};     /* common dialog box structure */
    static DWORD rgbCurrent = RGB(0,0,0);

    static COLORREF acrCustClr[16] =
    {
        RGB(255, 255, 255), RGB(239, 239, 239),
        RGB(223, 223, 223), RGB(207, 207, 207),
        RGB(191, 191, 191), RGB(175, 175, 175),
        RGB(159, 159, 159), RGB(143, 143, 143),
        RGB(127, 127, 127), RGB(111, 111, 111),
        RGB( 95,  95,  95), RGB( 79,  79,  79),
        RGB( 63,  63,  63), RGB( 47,  47,  47),
        RGB( 31,  31,  31), RGB( 15,  15,  15)
    };

    // Initialize necessary members.

    cc.lStructSize = sizeof(CHOOSECOLOR);
    cc.hwndOwner = hwnd;
    cc.lpCustColors = (LPDWORD) acrCustClr;
    cc.rgbResult = rgbCurrent;
    cc.Flags = CC_RGBINIT | CC_SHOWHELP;

    /* Call the ChooseColor function. */

    if (ChooseColor(&cc))
    {
        rgbCurrent = cc.rgbResult;
        SetCurrentColor(cc.rgbResult);
        InvalidateRect(hwnd,NULL,TRUE);
    }
    return 0;
}
