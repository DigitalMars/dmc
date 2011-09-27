// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
// PARTICULAR PURPOSE.
//
// Copyright (C) 1993 - 1995  Microsoft Corporation.  All Rights Reserved.
//
//  MODULE: filedlg.c
//
//  PURPOSE: Shows basic use of "Open" and "Save As" common dialogs.
//
//  FUNCTIONS:
//    CmdOpen - Call the open common dialog and show its results.
//    CmdSave - Performs file save or save as operations.
//    SavingDlgProc - Modeless dialog box which is displayed while saving a file.
//    CmdClose - Close the file unless user decides to cancel.
//    QuerySaveChanges - Query the user if bitmap has changed. 
//
//  COMMENTS:
//
//
//
//  SPECIAL INSTRUCTIONS: N/A
//

#include <windows.h>            // required for all Windows applications
#include <windowsx.h>  
#include <commctrl.h>           // prototypes and structure for the common controls.
#include <commdlg.h>            // For common dialogs   

#include "globals.h"            // prototypes specific to this application 
#include "statbar.h"
#include "toolbar.h"
#include "palette.h"
#include "dibutil.h"
#include "resource.h"
        
BOOL CALLBACK SavingDlgProc(HWND, UINT, WPARAM, LPARAM);

// constant for CmdSave                    
#define MAXFILENAMELEN 144

// buffer for string resources
char szBuffer[50];              // watch out for recursive use of this buffer!

//
//  FUNCTION: CmdOpen(HWND, WORD, WORD, HWND)
//
//  PURPOSE: Call the open common dialog and show its results.
//
//  PARAMETERS:
//    hwnd     - The window handle.
//    wCommand - IDM_FILEOPEN
//    wNotify  - Notification number (unused)
//    hwndCtrl - NULL (Unused)
//
//  RETURN VALUE:
//    Always returns 0 - command handled.
//
//  COMMENTS:
//    Assumes there is a resource string describing this command with the
//    same ID as the command ID.  Loads the string and calls UpdateStatusBar
//    to put the string into main pane of the status bar.
//

LRESULT CmdOpen(HWND hwnd, WORD wCommand, WORD wNotify, HWND hwndCtrl)
{
    OPENFILENAME ofn = {0}; // common dialog box structure   
    char szFile[256];       // filename string
    char szFileTitle[256];  // file-title string
    char szFilter[256];     // filter string
    char chReplace;         // separator for szFilter
    int i, cbString;        // integer count variables    
    int  cbWritten = 0;   
    int nResult;    

    // prompt user if there is an existing bitmap with changes
    nResult = QuerySaveChanges(hwnd);
    if (nResult == IDCANCEL)
        goto ErrExit;
                                  
    cbWritten = LoadString(hInst, wCommand, szBuffer, sizeof(szBuffer)); 
    if(cbWritten == 0)
        lstrcpy(szBuffer, "Unknown Command");        
    UpdateStatusBar(szBuffer, 0, 0);                  
   
    // Place the terminating null character in the szFile.
    szFile[0] = '\0';

    // Load the filter string from the resource file.
    cbString = LoadString(hInst, IDS_FILTERSTRING, szFilter, sizeof(szFilter));

    // Add a terminating null character to the filter string.
    chReplace = szFilter[cbString - 1];
    for (i = 0; szFilter[i] != '\0'; i++)
    {
        if (szFilter[i] == chReplace)
            szFilter[i] = '\0';
    }

    // Set the members of the OPENFILENAME structure.
    ofn.lStructSize     = sizeof(OPENFILENAME);
    ofn.hwndOwner       = hwnd;
    ofn.lpstrFilter     = szFilter;
    ofn.nFilterIndex    = 1;
    ofn.lpstrFile       = szFile;
    ofn.nMaxFile        = sizeof(szFile);
    ofn.lpstrFileTitle  = szFileTitle;
    ofn.nMaxFileTitle   = sizeof(szFileTitle);
    ofn.lpstrInitialDir = NULL;
    ofn.Flags           = OFN_SHOWHELP | OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

    // Display the Open dialog box.
    if (GetOpenFileName(&ofn))
    {
        // successfully obtained filename so open the file
        hBitmap = LoadDIBSection(hWndClient, ofn.lpstrFile);
        
        if (!hBitmap)        
            goto ErrExit;           
       
        SizeClientWindow(hwnd);
          
        // display the bitmap
        InvalidateRect(hWndClient, NULL, TRUE);  

        SetWindowTitle(hwnd, ofn.lpstrFile);      
    } 
            
ErrExit:            
    // set the status bar text to original text     
    UpdateStatusBar(SZDESCRIPTION, 0, 0);
    return 0;
}


//  FUNCTION: CmdSave(HWND, WORD, WORD, HWND)
//
//  PURPOSE:  Performs file save or save as operations
//
//  PARAMETERS:
//    hwnd     - The window.
//    wCommand - IDM_FILESAVE or IDM_FILESAVEAS
//    wNotify  - Notification number (unused)
//    hwndCtrl - NULL (unused)
//
//  RETURN VALUE:
//    Always returns 0 - command handled.
//
//  COMMENTS:
//    Assumes there is a resource string describing this command with the
//    same ID as the command ID.  Loads the string and calls UpdateStatusBar
//    to put the string into main pane of the status bar.
//

LRESULT CmdSave(HWND hwnd, WORD wCommand, WORD wNotify, HWND hwndCtrl) 
{  
    char    szFileBuf[255];      // Buffer to hold returned file name
    OPENFILENAME ofn = {0};      // structure for GetSaveFileName common dialog
    char    szTitle[50];         // common dialog box title 
    char    szFilter[256];       // edit control filter string
    char    chReplace;           // separator for szFilter
    int i,  cbString;            // integer count variables   
    int     nResult;             // Result of SaveDIB function    
    HWND    hModelessDlg = NULL; // modeless dialog displayed while saving file
    int     cbWritten = 0;

    cbWritten = LoadString(hInst, wCommand, szBuffer, sizeof(szBuffer)); 
    if(cbWritten == 0)
        lstrcpy(szBuffer, "Unknown Command");        
    UpdateStatusBar(szBuffer, 0, 0);
                
    // make a local copy of the current file name and "Untitled"
    _fstrcpy((LPSTR)szFileBuf, (LPSTR)szCurrentFile);    
    LoadString(hInst, IDS_UNTITLED, szBuffer, sizeof(szBuffer));

    if (wCommand == IDM_FILESAVEAS || 
            !_fstrcmp((LPSTR)szFileBuf, (LPSTR)szBuffer))
    { 
        // performing save as or saving an untitled file; use common dialog
        // to get filename, compression and color resolution.       

        // Initialize the OPENFILENAME members
   
        LoadString(hInst, IDS_SAVEASTITLE, szTitle, sizeof(szTitle));        
                                 
        // Load the filter string from the resource file.
        cbString = LoadString(hInst, IDS_FILTERSTRING, szFilter, sizeof(szFilter));

        // Add a terminating null character to the filter string.
        chReplace = szFilter[cbString - 1];
        for (i = 0; szFilter[i] != '\0'; i++)
        {
            if (szFilter[i] == chReplace)
                szFilter[i] = '\0';
        }
                                     
        ofn.lStructSize       = sizeof(OPENFILENAME);
        ofn.hwndOwner         = hwnd;
        ofn.hInstance         = hInst;
        ofn.lpstrFilter       = szFilter;
        ofn.lpstrCustomFilter = NULL;
        ofn.nMaxCustFilter    = 0L;
        ofn.nFilterIndex      = 1L;
  
        if (_fstrcmp(szFileBuf, (LPSTR)szBuffer)) 
            // bitmap already has a name
            ofn.lpstrFile = szFileBuf;
        else
            // untitled bitmap needs to have a name
            ofn.lpstrFile = (LPSTR)"*.BMP";   

        ofn.nMaxFile          = MAXFILENAMELEN;
        ofn.lpstrFileTitle    = NULL;
        ofn.nMaxFileTitle     = 0;
        ofn.lpstrInitialDir   = NULL;
        ofn.lpstrTitle        = szTitle; 
        ofn.Flags             = OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT;
        ofn.nFileOffset       = 0;
        ofn.nFileExtension    = 0;
        ofn.lpstrDefExt       = "bmp";      

        // Call the GetSaveFilename function
        if (GetSaveFileName(&ofn))
            // copy filename specification given by user
            _fstrcpy((LPSTR)szFileBuf, (LPSTR)ofn.lpstrFile);       
        else
            // user cancelled the operation
            goto ErrExit;
    }
               
    // display modeless dialog while saving to file   
    hModelessDlg = CreateDialogParam(hInst, 
                                     (LPSTR)"Saving",
                                     hwnd, 
                                     SavingDlgProc, 
                                     (DWORD)(LPSTR)szFileBuf);                                             

    // save the DIB to disk file
    nResult = SaveDIB(hDIBInfo, lpvBits, szFileBuf);
    if (nResult)
    {
        DIBError(nResult);
        goto ErrExitModeless;
    }
  
    // clear global flag for changes
    fChanges = FALSE;  
                            
    // update the title bar information
    SetWindowTitle(hwnd, szFileBuf);
                                    
    // update status of menu and toolbar
    EnableMenuItem(hMenu, IDM_FILESAVE, MF_DISABLED);
    SendMessage(hWndToolbar, TB_ENABLEBUTTON, IDM_FILESAVE, MAKELONG(FALSE, 0));
    
ErrExitModeless:
    DestroyWindow(hModelessDlg);
    hModelessDlg = NULL;       
         
ErrExit:         
    // set the status bar text to original text     
    UpdateStatusBar(SZDESCRIPTION, 0, 0);

    return 0;    
}
 

//
//  FUNCTION: SavingDlgProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: This is a modeless dialog box which is called when the bitmap is
//           saved to a file (so the user dosen't think the machine is hung).
//
//  PARAMETERS:
//    hdlg      - The window handle.
//    uMessage  - The message to process
//    wparam    - extra data
//    lparam    - extra data
//
//  RETURN VALUE:
//    TRUE if message was processed.
//    FALSE if the system needs to process the message.
//
//  COMMENTS:
//
//
//
BOOL CALLBACK SavingDlgProc(HWND hdlg, UINT uMessage, WPARAM wparam, 
    LPARAM lparam)
{
    switch (uMessage)
    {
        case WM_SETFOCUS:
            MessageBeep(0);
            break;

        case WM_INITDIALOG:
            // Center the dialog over the application window
            CenterWindow(hdlg, GetWindow(hdlg, GW_OWNER));
 
            // set the icon of this dialog to the class icon
            SendMessage(hdlg, WM_SETICON, (WPARAM)FALSE, (LPARAM)hIcon);
 
            // Set the text of the filename in the dialog box.  This dialog
            // should be called with DialogBoxParam, and the parameter should
            // be a pointer to the filename.  It shows up as the lParam here.
            SetDlgItemText(hdlg, IDD_FILETEXT, (LPSTR)lparam);
            return TRUE;
            break;

        case WM_DESTROY:
            return TRUE;
            break;

        default:
            return FALSE;
    }
}


//
//  FUNCTION: CmdClose(HWND, WORD, WORD, HWND)
//
//  PURPOSE: Processes the IDM_FILECLOSE message, queries user if there are 
//           changes.
//
//  PARAMETERS:
//    hwnd     - The window handle.
//    wCommand - IDM_FILECLOSE (Unused)
//    wNotify  - Notification number (unused)
//    hwndCtrl - NULL (Unused)
//
//  RETURN VALUE:
//    Always returns 0 - command handled.
//
//  COMMENTS:
//    
//

LRESULT CmdClose(HWND hwnd, WORD wCommand, WORD wNotify, HWND hwndCtrl)
{   
    int  cbWritten = 0;
    int nResult;
  
    cbWritten = LoadString(hInst, wCommand, szBuffer, sizeof(szBuffer)); 
    if(cbWritten == 0)
        lstrcpy(szBuffer, "Unknown Command");        
    UpdateStatusBar(szBuffer, 0, 0);
    
    // prompt user first in case the existing bitmap has unsaved changes
    nResult = QuerySaveChanges(hwnd);
    if (nResult == IDCANCEL)
        return 0;
   
    // remove the current DIB section and palette
    RemoveDIBSection();
    
    // create default wash for application palette
    if (bPalDevice)
    {
        HDC hdc;

        hdc = GetDC(hwnd);
        hPalette = CreateHalftonePalette(hdc);
        ReleaseDC(hwnd, hdc);
    }
                              
    // reset the window title
    SetWindowTitle(hwnd, "");
     
    // update the client window
    SizeClientWindow(hwnd);
                     
    // set the status bar text to original text     
    UpdateStatusBar(SZDESCRIPTION, 0, 0);
    
    return 1;
}


//
//  FUNCTION: QuerySaveChanges(HWND)
//
//  PURPOSE: Queries user to save changes in bitmap
//
//  PARAMETERS:
//    hwnd      - The window handle.
//
//  RETURN VALUE:
//    IDYES if user elects to save changes
//    IDNO if user elects to abandon changes or there were no changes
//    IDCANCEL if user wants to abandon calling process
//
//  COMMENTS:
//
//
// 
int QuerySaveChanges(HWND hwnd)
{   
    int nResult = IDNO;
    
    if (hBitmap && fChanges)
    {   
        LoadString(hInst, IDS_SAVECHANGES, szBuffer, sizeof(szBuffer));
        
        nResult = MessageBox(hwnd, szBuffer, szAppName, MB_YESNOCANCEL);
        
        if (nResult == IDYES)
            // save the existing bitmap
            CmdSave(hwnd, IDM_FILESAVE, 0, NULL);
        else if (nResult == IDCANCEL)
            // user has decided to cancel File New
            return IDCANCEL;
            
        // no action needed if IDNO is returned
    }
 
    return nResult;
} 


            
