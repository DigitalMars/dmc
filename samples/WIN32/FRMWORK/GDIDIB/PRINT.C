// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
// PARTICULAR PURPOSE.
//
// Copyright (C) 1993 - 1995  Microsoft Corporation.  All Rights Reserved.
//
//  MODULE: print.c
//
//  PURPOSE: Handle the application specific printing commands based on
//    parameters gathered from the common printer dialog box.
//
//  FUNCTIONS:  
//    CmdPrint      - Handles the File Print command (disabled).
//    CmdPageSetup  - Handles the File PageSetup command (disabled).                
//    CmdPrintSetup - Handles the File PrintSetup command (disabled).
//
//
//  COMMENTS:
//
//  SPECIAL INSTRUCTIONS: N/A
//


#include <windows.h>            // required for all Windows applications 
#include <windowsx.h> 
           
#include <string.h>
           
#include "globals.h"            // prototypes specific to this application
#include "resource.h"
#include "statbar.h"

// buffer for string resources
char szBuffer[50];              // watch out for recursive use of this buffer!
   
  
//  FUNCTION: CmdPrint(HWND, WORD, WORD, HWND)
//
//  PURPOSE: Display statusbar updates by calling SendMessage(..SB_SETTEXT..).
//
//  PARAMETERS:
//    hwnd     - The window.
//    wCommand - WM_COMMAND 
//    wNotify  - Notification number (unused)
//    hwndCtrl - NULL (unused)
//
//  RETURN VALUE:
//    Always returns 0 - command handled.
//
//  COMMENTS:
//   
//

LRESULT CmdPrint(HWND hwnd, WORD wCommand, WORD wNotify, HWND hwndCtrl) 
{  
    int cbWritten = 0; 
   
    cbWritten = LoadString(hInst, wCommand, szBuffer, sizeof(szBuffer)); 
    if(cbWritten == 0)
        lstrcpy(szBuffer, "Unknown Command");
        
    UpdateStatusBar(szBuffer, 0, 0);
 
    // implementation code will go here

    // once command is executed, set the statusbar text to original
    UpdateStatusBar(SZDESCRIPTION, 0, 0);  
    
    return 0;
}
   
//  FUNCTION: CmdPageSetup(HWND, WORD, WORD, HWND)
//
//  PURPOSE: Display statusbar updates by calling SendMessage(..SB_SETTEXT..).
//
//  PARAMETERS:
//    hwnd     - The window.
//    wCommand - WM_COMMAND
//    wNotify  - Notification number (unused)
//    hwndCtrl - NULL (unused)
//
//  RETURN VALUE:
//    Always returns 0 - command handled.
//
//  COMMENTS:
//   
//

LRESULT CmdPageSetup(HWND hwnd, WORD wCommand, WORD wNotify, HWND hwndCtrl) 
{  
    int  cbWritten = 0;

    cbWritten = LoadString(hInst, wCommand, szBuffer, sizeof(szBuffer)); 
    if(cbWritten == 0)
        lstrcpy(szBuffer, "Unknown Command");
        
    UpdateStatusBar(szBuffer, 0, 0);
   
    // implementation code will go here

    // once command is executed, set the statusbar text to original
    UpdateStatusBar(SZDESCRIPTION, 0, 0);
    return 0;
}

   
//  FUNCTION: CmdPrintSetup(HWND, WORD, WORD, HWND)
//
//  PURPOSE:  Display statusbar updates by calling SendMessage(..SB_SETTEXT..).
//
//  PARAMETERS:
//    hwnd     - The window.
//    wCommand - WM_COMMAND
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


LRESULT CmdPrintSetup(HWND hwnd, WORD wCommand, WORD wNotify, HWND hwndCtrl) 
{   
    int  cbWritten = 0;

    cbWritten = LoadString(hInst, wCommand, szBuffer, sizeof(szBuffer)); 
    if(cbWritten == 0)
        lstrcpy(szBuffer, "Unknown Command");
        
    UpdateStatusBar(szBuffer, 0, 0); 
    
    // implementation code will go here

    // once command is executed, set the statusbar text to original
    UpdateStatusBar(SZDESCRIPTION, 0, 0);
    
    return 0;
}


