// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
// PARTICULAR PURPOSE.
//
// Copyright (C) 1993 - 1995  Microsoft Corporation.  All Rights Reserved.
//
//  MODULE:   palette.c
//
//  PURPOSE:  Contains palette routines
//
//  FUNCTIONS:
//    ProcessPaletteChanged  - Process the WM_PALETTECHANGED message
//    ProcessQueryNewPalette - Process the WM_QUERYNEWPALETTE message
//    IsPaletteDevice        - Checks display for Windows palette
//                             capabilities.
//    PalEntriesOnDevice     - Get the number of palette entries on
//                             the specified device
//    GetSystemPalette       - Gets the current system palette
//    ColorsInPalette        - Given a handle to a palette, returns the
//                             # of colors in that palette.
//
//  COMMENTS:
//


#include <windows.h>            // required for all Windows applications
#include <windowsx.h> 
#include "globals.h"            // prototypes specific to this application
#include "palette.h" 


//
//  FUNCTION: ProcessPaletteChanged(HWND, WPARAM)
//
//  PURPOSE:  Process the WM_PALETTECHANGED message
//
//  PARAMETERS:
//    hwnd     -  Handle of window receiving the message.
//    wparam    - Handle of window that changed the palette.
//
//  RETURN VALUE:
//    TRUE if message processed, else FALSE.
//
//  COMMENTS:
//    The WM_PALETTECHANGED message informs all windows that the window with
//    input focus has realized its logical palette, thereby changing the
//    system palette. This message allows a window without input focus that
//    uses a color palette to realize its logical palettes and update its
//    client area.
//
//    This message is sent to all windows, including the one that changed
//    the system palette and caused this message to be sent. The wParam of
//    this message contains the handle of the window that caused the system
//    palette to change. To avoid an infinite loop, care must be taken to
//    check that the wParam of this message does not match the window's handle.
//            
LRESULT ProcessPaletteChanged(HWND hwnd, WPARAM wparam)
{
    HDC      hDC;      // Handle to device context
    HPALETTE hOldPal;  // Handle to previous logical palette

    // Before processing this message, make sure we are indeed using a palette
    if (hPalette)
    {
        // If this application did not change the palette, select
        // and realize this application's palette
        if ((HWND)wparam != hwnd)
        {
            // get a display context for the window
            hDC = GetDC(hwnd);

            // Select and realize our palette
            hOldPal = SelectPalette(hDC, hPalette, TRUE);
            RealizePalette(hDC);

            // When updating the colors for an inactive window,
            // UpdateColors can be called because it is faster than
            // redrawing the client area (even though the results are
            // not as good)
            UpdateColors(hDC);

            // Clean up
            if (hOldPal)
                SelectPalette(hDC, hOldPal, FALSE);   
                
            ReleaseDC(hwnd, hDC);
        } 
            
        // message processed
        return TRUE;  
    }
         
    // message not processed
    return FALSE;  
}


//
//  FUNCTION: ProcessQueryNewPalette(HWND)
//
//  PURPOSE:  Process the WM_QUERYNEWPALETTE message.
//
//  PARAMETERS:
//    hwnd     -  Handle of window receiving the message. 
//
//  RETURN VALUE:
//    TRUE if message processed, else FALSE.
//
//  COMMENTS:
//    The WM_QUERYNEWPALETTE message informs a window that it is about to
//    receive input focus. In response, the window receiving focus should
//    realize its palette as a foreground palette and update its client
//    area. If the window realizes its palette, it should return TRUE;
//    otherwise, it should return FALSE.
//
LRESULT ProcessQueryNewPalette(HWND hwnd)
{
    HDC      hDC;      // Handle to device context
    HPALETTE hOldPal;  // Handle to previous logical palette

    // Only process the message if a palette is in use
    if (hPalette)
    {
        // get a display context for the window       
        hDC = GetDC(hwnd);

        // Select and realize our palette
        hOldPal = SelectPalette(hDC, hPalette, FALSE);
        RealizePalette(hDC);

        // Redraw the entire client area
        InvalidateRect(hwnd, NULL, TRUE);
        UpdateWindow(hwnd);

        // Clean up
        if (hOldPal)
            SelectPalette(hDC, hOldPal, FALSE);
        ReleaseDC(hwnd, hDC);

        // Message processed
        return TRUE;
    }
    
    // Message not processed
    return FALSE;
}

//
//  FUNCTION: IsPaletteDevice()
//
//  PURPOSE:  Checks display for Windows palette capabilities.
//
//  PARAMETERS:
//    None.
//
//  RETURN VALUE:
//    TRUE if a palette device, FALSE otherwise
//
//  COMMENTS:
//
BOOL IsPaletteDevice()
{
    HDC hdc;
    int iRastercaps;
    
    hdc = GetDC(NULL);
    iRastercaps = GetDeviceCaps(hdc, RASTERCAPS);
    ReleaseDC(NULL, hdc);
    
    return ((iRastercaps & RC_PALETTE) ? TRUE : FALSE);
}


//
//  FUNCTION: PalEntriesOnDevice(HDC)
//
//  PURPOSE:  Get the number of palette entries on the specified device
//
//  PARAMETERS:
//    hDC          - device context
//
//  RETURN VALUE:
//    The number of palette entries on device
//
//  COMMENTS:
//
//
int PalEntriesOnDevice(HDC hDC)
{
    int nColors;  // number of colors

    // Find out the number of palette entries on this device.
    
    nColors = GetDeviceCaps(hDC, SIZEPALETTE);

    // For non-palette devices, we'll use the # of system
    // colors for our palette size.

    if (!nColors)
        nColors = GetDeviceCaps(hDC, NUMCOLORS);

    return nColors;
}


//
//  FUNCTION: GetSystemPalette(void)
//
//  PURPOSE:  This routine returns a handle to a palette which represents
//            the system palette (each entry is an offset into the system
//            palette instead of an RGB with a flag of PC_EXPLICIT).
//            Useful for dumping the system palette.
//
//  PARAMETERS:
//    None.
//
//  RETURN VALUE:
//    Handle to a palette consisting of the system palette.
//
//  COMMENTS:
//

HPALETTE GetSystemPalette(void)
{
    HDC          hDC;
    HPALETTE     hPal = NULL;
    HANDLE       hLogPal;
    LPLOGPALETTE lpLogPal;
    int          i, nColors;

    // Find out how many palette entries we want.
    
    hDC = GetDC(NULL);
    if (!hDC)
        return NULL;

    nColors = PalEntriesOnDevice(hDC);
    ReleaseDC(NULL, hDC);

    // Allocate room for the palette and lock it.
    
    hLogPal = GlobalAlloc(GHND, sizeof(LOGPALETTE) + 
                          nColors * sizeof(PALETTEENTRY));

    if (!hLogPal)
        return NULL;

    lpLogPal = (LPLOGPALETTE)GlobalLock(hLogPal);

    lpLogPal->palVersion    = 0x300;
    lpLogPal->palNumEntries = nColors;

    for (i = 0;  i < nColors;  i++)
    {
        lpLogPal->palPalEntry[i].peBlue = 0;
        *((LPWORD)(&lpLogPal->palPalEntry[i].peRed)) = i;
        lpLogPal->palPalEntry[i].peFlags = PC_EXPLICIT;
    }

    // Go ahead and create the palette.  Once it's created,
    //  we no longer need the LOGPALETTE, so free it.

    hPal = CreatePalette(lpLogPal);

    GlobalUnlock(hLogPal);
    GlobalFree(hLogPal);

    return hPal;
}


//
//  FUNCTION: ColorsInPalette(HPALETTE)
//
//  PURPOSE:  Given a handle to a palette, returns the # of colors
//            in that palette.
//
//  PARAMETERS:
//    hPal      - Handle to palette we want info on.
//
//  RETURN VALUE:
//    Number of colors in the given palette
//
//  COMMENTS:
//

int ColorsInPalette(HPALETTE hPal)
{
    int nColors=0;

    if (!hPal)
        return 0;

    GetObject(hPal, sizeof(nColors), (LPSTR)&nColors);

    return nColors;
}
