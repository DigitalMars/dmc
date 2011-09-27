// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
// PARTICULAR PURPOSE.
//
// Copyright (C) 1993 - 1995  Microsoft Corporation.  All Rights Reserved.
//
//  MODULE:   filenew.c
//
//  PURPOSE:   Displays the "File New" dialog box
//
//  FUNCTIONS:
//    CmdNew               - Displays the "File New" dialog box
//    FileNew              - Processes messages for "File New" dialog box.
//    MsgFileNewInit       - To initialize the File New dialog box
//    MsgFileNewCommand    - Process WM_COMMAND message sent to the FileNew box. 
//    CmdFileNewEditNotify - Validate bitmap size entered in File New dialog box.
//    CmdFileNewDone       - Free the File New dialog box.
//                      
//  COMMENTS:
//
//

#include <windows.h>            // required for all Windows applications
#include <windowsx.h>   
#include <commctrl.h>           // prototypes and structure for the common controls.

#include "globals.h"            // prototypes specific to this application  
#include "resource.h"
#include "toolbar.h"
#include "statbar.h"  
#include "palette.h"
#include "dibutil.h"

LRESULT CALLBACK FileNew(HWND, UINT, WPARAM, LPARAM);
LRESULT MsgFileNewInit(HWND, UINT, WPARAM, LPARAM);
LRESULT MsgFileNewCommand(HWND, UINT, WPARAM, LPARAM);
LRESULT CmdFileNewEditNotify(HWND, WORD, WORD, HWND);
LRESULT CmdFileNewDone(HWND, WORD, WORD, HWND);

// FileNew dialog message table definition.
MSD rgmsdFileNew[] =
{
    {WM_COMMAND,    MsgFileNewCommand},
    {WM_INITDIALOG, MsgFileNewInit}
};

MSDI msdiFileNew =
{
    sizeof(rgmsdFileNew) / sizeof(MSD),
    rgmsdFileNew,
    edwpNone
};

// FileNew dialog command table definition.
CMD rgcmdFileNew[] =
{
    {IDD_WIDTH,  CmdFileNewEditNotify},
    {IDD_HEIGHT, CmdFileNewEditNotify},
    {IDOK,       CmdFileNewDone},
    {IDCANCEL,   CmdFileNewDone}
};

CMDI cmdiFileNew =
{
    sizeof(rgcmdFileNew) / sizeof(CMD),
    rgcmdFileNew,
    edwpNone
};

// Module specific "globals"  Used when a variable needs to be
// accessed in more than on handler function.
         
// some arbitrary bitmap sizes                           
#define DEFBITMAPSIZE 256                 // default bitmap width or height
#define MAXBITMAPSIZE 2048                // maximum allowable width or height
#define NOBITMAPSIZE  (MAXBITMAPSIZE + 1) // width or height edit control empty
         
// specification for new bitmap
UINT cxNewWidth;
UINT cyNewHeight;
UINT uNewBitsPerPixel;

// flag to prevent recursion in FileNewEditNotify
BOOL fInEditNotify = FALSE;

//
//  FUNCTION: CmdNew(HWND, WORD, WORD, HWND)
//
//  PURPOSE: Call the open common dialog and show its results.
//
//  PARAMETERS:
//    hwnd     - The window handle.
//    wCommand - IDM_FILENEW
//    wNotify  - Notification number (unused)
//    hwndCtrl - NULL (Unused)
//
//  RETURN VALUE:
//    Always returns 0 - command handled.
//
//  COMMENTS:
//    To process the IDM_FILENEW message, call DialogBox() to display the
//    FileNew dialog box.
//

LRESULT CmdNew(HWND hwnd, WORD wCommand, WORD wNotify, HWND hwndCtrl)
{   
    HDC hdc;    
    LPBITMAPINFOHEADER lpbih;    
    char szBuffer[50];   
    int  cbWritten = 0;
    int nResult;
    int nNumColors = 0;     // number of colors supposed to be in color table  
    int nColorsInTable;     // actual number of colors in color table
    HDIB hDIBNew;    
    LPRGBQUAD lpColorTable;    
    HPALETTE hPalNew = NULL;

    cbWritten = LoadString(hInst, wCommand, szBuffer, sizeof(szBuffer)); 
    if(cbWritten == 0)
        lstrcpy(szBuffer, "Unknown Command");        
    UpdateStatusBar(szBuffer, 0, 0);

    // prompt user if there is an existing bitmap with changes
    nResult = QuerySaveChanges(hwnd);
    if (nResult == IDCANCEL)
        goto ErrExit;
    
    // put up dialog asking for parameters    
    nResult = DialogBox(hInst, "FileNew", hwnd, (DLGPROC)FileNew);
    if (!nResult)
        goto ErrExit;  
                                         
    if (uNewBitsPerPixel > 8) 
        // no color table if greater than 8 bpp
        nNumColors = 0;
    else  
        // set number of colors in color table
        nNumColors = 1 << uNewBitsPerPixel;                
        
    // Allocate memory for header & color table (if needed)
    hDIBNew = GlobalAlloc(GHND, (DWORD)(sizeof(BITMAPINFOHEADER) +
        nNumColors * sizeof(RGBQUAD)));

    if (!hDIBNew)
    {    
        DIBError(ERR_MEMORY);
        goto ErrExit;
    }

    lpbih = (LPBITMAPINFOHEADER)GlobalLock(hDIBNew);
    if (!lpbih)
    {
        DIBError(ERR_LOCK);
        goto ErrExitFree;  
    }
          
    // initialize bitmap header
    lpbih->biSize = sizeof(BITMAPINFOHEADER);
    lpbih->biWidth = cxNewWidth;      
    lpbih->biHeight = cyNewHeight;       
    lpbih->biPlanes = 1;              // must be 1
    lpbih->biBitCount = uNewBitsPerPixel;
    lpbih->biCompression = BI_RGB;    
    lpbih->biSizeImage = WIDTHBYTES((DWORD)lpbih->biWidth * 
        (DWORD)lpbih->biBitCount) * lpbih->biHeight;
    lpbih->biXPelsPerMeter = 0;
    lpbih->biYPelsPerMeter = 0;
    lpbih->biClrUsed = nNumColors;        
    lpbih->biClrImportant = 0; 

    if (nNumColors)
    {     
        if (bPalDevice)
        {
            HDC hdc;

            hdc = GetDC(hwnd);
            hPalNew = CreateHalftonePalette(hdc);
            ReleaseDC(hwnd, hdc);
        }

        // fill the color table from the logical palette
        lpColorTable = (LPRGBQUAD)((LPSTR)(lpbih) + lpbih->biSize);
        nColorsInTable = FillColorTable(hwnd,
                                        lpColorTable,
                                        hPalNew,
                                        nNumColors);
    
        if (nColorsInTable == 0)
        {
            // unable to store any color values
            DIBError(ERR_FILLCOLORTABLE);
            goto ErrExitUnlock;  
        }
        else if (nColorsInTable != nNumColors)
            lpbih->biClrUsed = nColorsInTable;
    }
     
    // we've reached the point of no return (no undo)
    // remove the existing DIB section
    RemoveDIBSection(); 
        
    // set global variable to new DIB header    
    hDIBInfo = hDIBNew;
      
    // create new DIB section
    hdc = GetDC(hWndClient);  
    hBitmap = CreateDIBSection(hdc, (LPBITMAPINFO)lpbih, DIB_RGB_COLORS, 
        &lpvBits, 0, 0L);
    ReleaseDC(hWndClient, hdc);
    
    if (!hBitmap || !lpvBits)
    {   
        DIBError(ERR_CREATEDIBSECTION); 
         
        if (hBitmap)            
            DeleteObject(hBitmap); 
            
        // create default wash for palette
        if (bPalDevice)
        {
            HDC hdc;

            hdc = GetDC(hwnd);
            hPalNew = CreateHalftonePalette(hdc);
            ReleaseDC(hwnd, hdc);
        }
                        
        goto ErrExitUnlock;
    } 
    
    // set global variable for palette
    hPalette = hPalNew;
   
    // initialize the DIB section's bitmap surface and paint the display 
    InitDIBSection(hWndClient);
     
    LoadString(hInst, IDS_UNTITLED, szBuffer, sizeof(szBuffer));                        
    SetWindowTitle(hwnd, szBuffer);

    // adjust the size of the client window to the size of the bitmap
    SizeClientWindow(hwnd);
          
    // unlock the DIB header, but don't free it!
    GlobalUnlock(hDIBInfo);     
    
    // update status of menu and toolbar                                                        
    // note that it's possible to save a newly initialized bitmap
    EnableMenuItem(hMenu, IDM_FILESAVE, MF_ENABLED);
    SendMessage(hWndToolbar, TB_ENABLEBUTTON, IDM_FILESAVE, MAKELONG(TRUE, 0));  
    EnableMenuItem(hMenu, IDM_FILESAVEAS, MF_ENABLED);    
    EnableMenuItem(hMenu, IDM_FILECLOSE, MF_ENABLED);
    EnableMenuItem(hMenu, IDM_CLEAR, MF_ENABLED);
    
    // set the status bar text to original text     
    UpdateStatusBar(SZDESCRIPTION, 0, 0);    
    return 1;       

ErrExitUnlock:
    // note: hDIBNew == hDIBInfo if failure occurred after CreateDIBSection call
    GlobalUnlock(hDIBNew);    

ErrExitFree:
    // note: hDIBNew == hDIBInfo if failure occurred after CreateDIBSection call    
    GlobalFree(hDIBNew);
              
ErrExit:            
    // set the status bar text to original text     
    UpdateStatusBar(SZDESCRIPTION, 0, 0);
    
    return 0;
}

//
//  FUNCTION: FileNew(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for "FileNew" dialog box.
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
//     Display version information from the version section of the
//     application resource.
//
//     Wait for user to click on "Ok" button, then close the dialog box.
//

LRESULT CALLBACK FileNew(HWND hdlg, UINT uMessage, WPARAM wparam, LPARAM lparam)
{
    return DispMessage(&msdiFileNew, hdlg, uMessage, wparam, lparam);
}


//
//  FUNCTION: MsgFileNewInit(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: To initialize the FileNew box with version info from resources.
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
//    each of the static text boxes in the FileNew box.
//

LRESULT MsgFileNewInit(HWND hdlg, UINT uMessage, WPARAM wparam, LPARAM lparam)
{   
    HDC hdc;
    int nBitsPerPixel;      // number of color bits per pixel used by device
    int nPlanes;            // number of color planes used by device
    LONG lNumColors;        // number of colors displayed by device
    int nBitsPerPixelID;    // dialog control for color resolution setting
    
    // set the icon of this dialog to the class icon
    SendMessage(hdlg, WM_SETICON, (WPARAM)FALSE, (LPARAM)hIcon);
        
    // Center the dialog over the application window
    CenterWindow(hdlg, GetWindow(hdlg, GW_OWNER));
                           
    // Get the color resolution used by the device driver
    hdc = GetDC(hdlg);
    nBitsPerPixel = GetDeviceCaps(hdc, BITSPIXEL);
    nPlanes = GetDeviceCaps(hdc, PLANES);
    ReleaseDC(hdlg, hdc);
    
    // calculate number of colors displayed by device
    lNumColors = 1L << (nBitsPerPixel + nPlanes - 1);
    
    // Create default bitmap settings    
    cxNewWidth = DEFBITMAPSIZE;
    cyNewHeight = DEFBITMAPSIZE;   
    SetDlgItemInt(hdlg, IDD_WIDTH, DEFBITMAPSIZE, FALSE);
    SetDlgItemInt(hdlg, IDD_HEIGHT, DEFBITMAPSIZE, FALSE);    
            
    if (lNumColors < (1 << 4))
        // device displays less than 16 colors      
        nBitsPerPixelID = IDD_1;      
    else if (lNumColors < (1 << 8))
        // device displays less than 256 colors
        nBitsPerPixelID = IDD_4;
    else if (lNumColors < (1L << 16))
        // display device capability is less than 64K colors       
        nBitsPerPixelID = IDD_8;
    else if (lNumColors < (1L << 24))                  
        // display device capability is less than 4M colors         
        nBitsPerPixelID = IDD_16;          
    else                        
        // display device capability is at least 4M colors        
        nBitsPerPixelID = IDD_24; 
        
    SendDlgItemMessage(hdlg, nBitsPerPixelID, BM_SETCHECK, 1, 0L);    

    return TRUE;
}

//
//  FUNCTION: MsgFileNewCommand(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: Process WM_COMMAND message sent to the FileNew box.
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
//    Uses this DipsCommand function defined in wndproc.c combined
//    with the cmdiFileNew structure defined in this file to handle
//    the command messages for the FileNew dialog box.
//

LRESULT MsgFileNewCommand(HWND   hwnd, 
                        UINT   uMessage, 
                        WPARAM wparam, 
                        LPARAM lparam)
{
    return DispCommand(&cmdiFileNew, hwnd, wparam, lparam);
}

//
//  FUNCTION: CmdFileNewValidate(HWND, WORD, HWND)
//
//  PURPOSE: Validate characters for bitmap size entered in File New dialog
//
//  PARAMETERS:
//    hwnd - The window handling the command.
//    wCommand - The command to be handled (unused).
//    hwndCtrl - NULL (unused).
//
//  RETURN VALUE:
//    Always returns TRUE.
//
//  COMMENTS:
//    
//

LRESULT CmdFileNewEditNotify(HWND hdlg, WORD wCommand, WORD wNotify, HWND hwndCtrl)
{   
    int nNumChars, nValue;
    char szBuffer[5];
    BOOL fTranslated;    
 
    if (wNotify != EN_UPDATE || fInEditNotify)
        // ignore all other notifications except EN_UPDATE, and don't recurse!
        return TRUE;        
                      
    // validate characters input into edit control
    nNumChars = GetDlgItemText(hdlg, wCommand, szBuffer, 5);  
    nValue = GetDlgItemInt(hdlg, wCommand, &fTranslated, FALSE);
                                   
    switch (wCommand)
    {
        case IDD_WIDTH:
            if (nNumChars == 0)     
                // the Width edit control is empty
                cxNewWidth = NOBITMAPSIZE;     
            else if (fTranslated && nValue <= MAXBITMAPSIZE)       
                // the user input a value in accepted range
                cxNewWidth = nValue;     
            else 
            {                            
                // bad user input, so reset to previous value  
                
                // first set the flag to prevent recursion
                fInEditNotify = TRUE;
                
                if (cxNewWidth == NOBITMAPSIZE)
                    // previously the Width edit control was empty
                    SetDlgItemText(hdlg, wCommand, "");
                else
                    // previously the Width edit control had a value
                    SetDlgItemInt(hdlg, wCommand, cxNewWidth, FALSE);
                                     
                // clear the recursion flag
                fInEditNotify = FALSE;                    
                                                     
                // notify the user
                MessageBeep((UINT)-1);      
            }    
            
            break;       
            
        case IDD_HEIGHT:
            if (nNumChars == 0)          
                // the Height edit control is empty
                cyNewHeight = NOBITMAPSIZE;             
            else if (fTranslated && nValue <= MAXBITMAPSIZE)
                // accept the user input
                cyNewHeight = nValue;
            else
            {
                // bad user input, so reset to previous value    
       
                // first set the flag to prevent recursion
                fInEditNotify = TRUE;
                                            
                if (cyNewHeight == NOBITMAPSIZE)
                    // previously the Height edit control was empty
                    SetDlgItemText(hdlg, wCommand, "");
                else                               
                    // previously the Height edit control had a value
                    SetDlgItemInt(hdlg, wCommand, cyNewHeight, FALSE); 
                    
                // clear the recursion flag
                fInEditNotify = FALSE;                    
                
                // notify the user
                MessageBeep((UINT)-1);           
            }        
            
            break;
            
        default:
            break;
    }
        
    return TRUE;
}
       
//
//  FUNCTION: CmdFileNewDone(HWND, WORD, HWND)
//
//  PURPOSE: Free the FileNew box and related data.
//
//  PARAMETERS:
//    hwnd - The window handling the command.
//    wCommand - The command to be handled (unused).
//    hwndCtrl - NULL (unused).
//
//  RETURN VALUE:
//    Always returns TRUE.
//
//  COMMENTS:
//    Calls EndDialog to finish the dialog session.
//

LRESULT CmdFileNewDone(HWND hdlg, WORD wCommand, WORD wNotify, HWND hwndCtrl)
{
    switch (wCommand)
    {
        case IDOK:
        {       
            char szBuffer[40];
            
            // the edit controls are already validated, but could be empty  
            
            if (cxNewWidth == NOBITMAPSIZE)
            {   
                LoadString(hInst, ERR_WIDTH, szBuffer, sizeof(szBuffer)); 
                MessageBox(hdlg, szBuffer, szAppName, MB_ICONHAND);
                SetFocus(GetDlgItem(hdlg, IDD_WIDTH));
                return TRUE;
            }
            
            if (cyNewHeight == NOBITMAPSIZE)
            {   
                LoadString(hInst, ERR_HEIGHT, szBuffer, sizeof(szBuffer));  
                MessageBox(hdlg, szBuffer, szAppName, MB_ICONHAND);
                SetFocus(GetDlgItem(hdlg, IDD_HEIGHT));
                return TRUE;
            }                    
            
            // we have the bitmap size, now get the color resolution
            if (SendDlgItemMessage(hdlg, IDD_1, BM_GETCHECK, 0, 0L))
                uNewBitsPerPixel = 1;
            else if (SendDlgItemMessage(hdlg, IDD_4, BM_GETCHECK, 0, 0L))
                uNewBitsPerPixel = 4;            
            else if (SendDlgItemMessage(hdlg, IDD_8, BM_GETCHECK, 0, 0L))
                uNewBitsPerPixel = 8;                                     
            else if (SendDlgItemMessage(hdlg, IDD_16, BM_GETCHECK, 0, 0L))
                uNewBitsPerPixel = 16;                        
            else
                uNewBitsPerPixel = 24;
       
             // exit the dialog
            EndDialog(hdlg, TRUE);         
            break;
        }
                
        case IDCANCEL:
            // cancel the dialog
            EndDialog(hdlg, FALSE);
            break;
            
        default:
            break;
    }
    
    return TRUE;
}
       
 
