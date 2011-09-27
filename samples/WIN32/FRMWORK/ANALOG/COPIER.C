// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
// PARTICULAR PURPOSE.
//
// Copyright (C) 1993-1995  Microsoft Corporation.  All Rights Reserved.
//
//  MODULE:   copier.c
//
//  PURPOSE:   Displays a dialog box which demonstrates the use of several
//    common controls, such as Progress Bar(Gas Gauge), Up/Down controls,
//    slider control and Status Bar control.
//
//  FUNCTIONS:
//    CopierProc - Processes messages for the Copier dialog box.
//
//    Copier Dialog Messages:
//       MsgDlgInit - Processing  of WM_INITDIALOG for the Copier dialog box.
//       MsgDlgCommand - Process WM_COMMAND message sent to Copier dialog box.
//       MsgDlgHScroll - Processing of WM_HSCROLL for the Copier dialog box.
//       MsgDlgTimer - Processing of WM_TIMER which is set by CmdStart()
//
//    Copier's WM_COMMAND Messages:
//       CmdnCopies - Validates amount entered in number of copies edit contorl
//       CmdStart - Starts the copy process.
//       CmdReset - Resets the dialog with its default values.
//       CmdStop - Stops/interupts the copy process.
//       CmdDone - Closes the dialog.
//
//    Copier's Helper functions:
//       InitStatusBar - Creates the Progress Bar Control
//       InitTBar - Creates and initializes the Track Bar Control
//       InitUDCtl - Creates the UpDown Control for the given Edit control
//       GetTBarPos - Calculates the Track Bar Control's dimensions
//       UpdateSB - Updates the status bar information about user selection
//       EnableControls - Enables and disables controls
//       StartCopying - Does the copying process and updates user
//       ResetDialog - Resets the dialog with default values
//       StopCopying - Stops the copying process by user's demand
//
//  COMMENTS:
//
//

#include <stdlib.h>
#include <windows.h>            // required for all Windows applications
#include <windowsx.h>
#include <commctrl.h>           // required for common controls
#include "globals.h"            // prototypes specific to this application
#include "copier.h"             // prototypes specific to the Copier dialog

// Functions for handling dialog window messages.  The message-dispatching
// mechanism expects all message-handling functions to have the following
// prototype:
//
//     LRESULT FunctionName(HWND, UINT, WPARAM, LPARAM);
LRESULT MsgDlgInit(HWND, UINT, WPARAM, LPARAM);
LRESULT MsgDlgCommand(HWND, UINT, WPARAM, LPARAM);
LRESULT MsgDlgHScroll(HWND, UINT, WPARAM, LPARAM);
LRESULT MsgDlgTimer(HWND, UINT, WPARAM, LPARAM);

// Functions for handling dialog window commands--ie. functions for
// processing WM_COMMAND messages based on the wParam value.
// The message-dispatching mechanism expects all command-handling
// functions to have the following prototype:
//
//     LRESULT FunctionName(HWND, WORD, WORD, HWND);
LRESULT CmdnCopies(HWND, WORD, WORD, HWND);
LRESULT CmdStart(HWND, WORD, WORD, HWND);
LRESULT CmdReset(HWND, WORD, WORD, HWND);
LRESULT CmdStop(HWND, WORD, WORD, HWND);
LRESULT CmdDone(HWND, WORD, WORD, HWND);

// Helper function prototypes used by the COPIER dialog.
HWND InitStatusBar(HWND);
HWND InitTBar(HWND, int, int, int, int, int, int, int, int);
HWND InitUDCtl(HWND, int, int, int, int, int);
void GetTBarPos(HWND, HWND, LPRECT);
void UpdateSB(HWND);
BOOL EnableControls(HWND ,BOOL);
BOOL StartCopying(HWND);
BOOL ResetDialog(HWND);
BOOL StopCopying(HWND, BOOL);

// Message and command dispatch infrastructure.  The following type
// definitions and functions are used by the message and command dispatching
// mechanism for the dialog.

    // This structure maps COPIER dialog messages to message handling 
    // functions.
MSD rgmsdDlg[] =
{
    {WM_INITDIALOG, MsgDlgInit},
    {WM_COMMAND,    MsgDlgCommand},
    {WM_HSCROLL,    MsgDlgHScroll},
    {WM_TIMER,      MsgDlgTimer} 

};

MSDI msdiDlg =
{
    sizeof(rgmsdDlg) / sizeof(MSD),
    rgmsdDlg,
    edwpNone
};

    // This structure maps COPIER dialogs command IDs to command handling 
    // functions.
CMD rgcmdCtrls[] =
{
    {IDD_NCOPIES,  CmdnCopies},     // displaying number of copies
    {IDOK,         CmdStart},       // Starts Copying
    {IDD_RESET,    CmdReset},       // Resets the copier
    {IDD_STOP,     CmdStop},        // Stops copying 
    {IDCANCEL,     CmdDone}         // Closes the dialog on ESC
};

CMDI cmdiCtrls =
{
    sizeof(rgcmdCtrls) / sizeof(CMD),
    rgcmdCtrls,
    edwpNone
};


// Global variable declarations.
UINT TimerID;                       // Timer ID for the Timer we set.
int iTimeCtr;                       // Timer tick counter
int nCopiesDone;                    // Number of copies completed copying.
int nCopies;                        // Number of copies selected
int iEnlargementPct;                // Enlargement percentage selected
int iShade;                         // Shading selected. 
                                    //       0 ==> lighter
                                    //       1 ==> Ave
                                    //       2 ==> Darker
BOOL fIsCopyState = FALSE;          // TRUE  ==> Copying State
                                    // FALSE ==> Idle or need clear State

//
//  FUNCTION: CopierProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for "Copier" dialog box.
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
//     Display a Copier Panel which demonstrated the use of 4 of the new 
//     Windows 95 Common Controls; Progress Bar, Up/Down Control, Slider Control
//     and Status Bar control.  System menu should be used to exit the dialog
//     box.
//

LRESULT CALLBACK CopierProc(HWND hdlg,
                            UINT uMessage,
                            WPARAM wparam,
                            LPARAM lparam)
{
    return DispMessage(&msdiDlg, hdlg, uMessage, wparam, lparam);
}

//
//  FUNCTION: MsgDlgInit(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: Performs initializations necessary for the Copier dialog box.
//
//  PARAMETERS:
//    hdlg - The window handing the message.
//    uMessage - The message number. (unused).
//    wparam - Message specific data (unused).
//    lparam - Message specific data (unused).
//
//  RETURN VALUE:
//    TRUE - message handled.
//
//  COMMENTS:
//     This function processed the code for WM_INITDIALOG for the Copier
//     dialog box.  The initialization steps are as follows:
//     1) Initializing of global variables used by the dialog.
//     2) default values are stored for use when resetting the dialog.  The
//        default values are stored using SetProp()
//     3) Initialize the state of the controls
//     4) Load the common control DLL.
//     5) Create and initialize the Status bar.
//     6) Create and initialize the Enlargement Track bar control.
//     7) Create and initialize the Shading Track bar control.
//     8) Create and initialize the Up/Down control.
//

LRESULT MsgDlgInit(HWND hdlg, UINT uMessage, WPARAM wparam, LPARAM lparam)
{
    HWND hCtl;
    RECT rc;

    // Initialize globals
    iTimeCtr = 0;
    nCopies = 1;
    iEnlargementPct = 100;
    iShade = 1;

    // Save the default values to be use by the resetting function.
    SetProp(hdlg, "defcopy", (HANDLE)nCopies);
    SetProp(hdlg, "defenlrg", (HANDLE)(iEnlargementPct / 10));
    SetProp(hdlg, "defshade", (HANDLE)iShade);

    // Disable the cancel button. This button is only used to cancel 
    // copying.
    hCtl = GetDlgItem(hdlg, IDD_STOP);
    EnableWindow(hCtl, FALSE);
    
    // Load the common control DLL
    InitCommonControls();

    // Create and initialize the Progress Bar
    if (!InitStatusBar(hdlg))
    {
        MessageBeep(0);
        return TRUE;
    }

    // Get the dimentions for the Track Bar
    GetTBarPos(hdlg, GetDlgItem(hdlg, IDD_ENLARGE), &rc);

    // Create and initialize the Track Bar
    if (!InitTBar(hdlg,                      
                  IDD_TBENLARGE,
                  rc.left, rc.top,           // x,y position of the track bar
                  (rc.right - rc.left),      // widht of the track bar
                  30,                        // height of the track bar
                  0, 20,                     // Min and Max of the Track bar
                  (iEnlargementPct / 10)))   // Initial position of Track bar
    {
        MessageBeep(0);
        return TRUE;
    }

    // Get the dimentions for the Track Bar
    GetTBarPos(hdlg, GetDlgItem(hdlg, IDD_DARK), &rc);

    // Create and initialize the Track Bar
    if (!InitTBar(hdlg,
                  IDD_TBDARK,
                  rc.left, rc.top,           // x,y position of the track bar
                  (rc.right - rc.left),      // widht of the track bar
                  30,                        // height of the track bar
                  0, 2,                      // Min and Max of the Track bar
                  iShade))                   // Initial position of Track bar
    {
        MessageBeep(0);
        return TRUE;
    }

    // Create and initialize the Up/Down for the Range Edit control
    if (!InitUDCtl(hdlg, IDD_NCOPIES, IDD_UDCOPIES, 50, 0, nCopies))
    {
        MessageBeep(0);
        return TRUE;
    }

    return TRUE;
}

//
//  FUNCTION: MsgDlgCommand(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: Process WM_COMMAND message sent to the "Copier" Dialog Box.
//
//  PARAMETERS:
//    hwnd - The window handing the message (handle of copier dialog window).
//    uMessage - The message number.
//    wparam - Message specific data.
//    lparam - Message specific data.
//
//  RETURN VALUE:
//    TRUE - message handled.
//
//  COMMENTS:
//    Uses this DispCommand function defined in wndproc.c combined
//    with the cmdiCtrls structure defined in COPIER.H file to handle
//    the command messages for the Copier dialog box.
//

LRESULT MsgDlgCommand (HWND hwnd, UINT uMessage, WPARAM wparam, LPARAM lparam)
{
    return DispCommand(&cmdiCtrls, hwnd, wparam, lparam);
}

//
//  FUNCTION: MsgDlgHScroll(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: Process WM_HSCROLL message sent to the "Copier" Dialog Box.
//
//  PARAMETERS:
//    hdlg - The window handing the message.
//    uMessage - The message number. (unused).
//    wparam - Message specific data.
//    lparam - Message specific data.
//
//  RETURN VALUE:
//    TRUE - message handled.
//
//  COMMENTS:
//    This message is processed for tracking the Track bar changes in the
//    dialog box.  The parent of a Track bar control is notified of a  
//    changes by user action, through WM_HSCROLL messages.
//          uMessage - WM_HSCROLL
//          wparam   - Low-order word of wparam contains the notification
//                     code and high-order word the new position. 
//                     NOTE:
//                     The position is only passed for TB_THUMBPOSITION and
//                     or SB_THUMBTRACK. 
//          lparam   - handle of the track bar control
//

LRESULT MsgDlgHScroll(HWND hdlg, UINT uMessage, WPARAM wparam, LPARAM lparam)
{
    int iCtrlID;
    HWND hCtl;
    int  iNotify;

    //  Only when the notification message is TB_THUMBPOSITION and the slider 
    //  thumb is dragged, the value of wparam is the position of the slider 
    //  thumb but not when clicking around on the slider control.  For all 
    //  other notification message the HIWORD(wparam) is 0.  TBM_GETPOS
    //  message can be send to retrieve the position of the slider thumb.
    iNotify = LOWORD(wparam);

    if (iNotify == TB_ENDTRACK)
    {
        hCtl = (HWND)lparam;
        iCtrlID = (int)GetDlgCtrlID(hCtl);
        if (iCtrlID == IDD_TBENLARGE)
            iEnlargementPct = 10 * (int)SendMessage(hCtl, TBM_GETPOS, 0, 0);
        else if (iCtrlID == IDD_TBDARK)
            iShade = (int)SendMessage(hCtl, TBM_GETPOS, 0, 0);

        UpdateSB(hdlg);
    }

    return TRUE;
}

//
//  FUNCTION: MsgDlgTimer(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: Process WM_TIMER message sent to the "Copier" Dialog Box.
//
//  PARAMETERS:
//    hdlg - The window handing the message.
//    uMessage - The message number. (unused).
//    wparam - Message specific data (unused).
//    lparam - Message specific data (unused).
//
//  RETURN VALUE:
//    TRUE - message handled.
//
//  COMMENTS:
//    This code pretends to copy documents.  The time to complete a copy
//    is arbitrarily set to 3.  With in this function the progress bar is
//    demonstrated and the status bar is updated to relay the copying 
//    information to the user. Progress bar is stepped every time one copy
//    is completed.  The amount used to step is calculated and set with in
//    the StartCopying() function below.
//

LRESULT MsgDlgTimer(HWND hdlg, UINT uMessage, WPARAM wparam, LPARAM lparam)
{
    static int iPctLeft;
    HWND hStatus;
    HWND hPGBar;
    HWND hCtl;

    if (!iTimeCtr)
        iPctLeft = 100;               // first timer tick initialise iPctLeft

    ++iTimeCtr;                       // increment the timer tick

    if ((iTimeCtr % 3) == 0)          // 3 timer ticks per copy
    {
        char szbuf[20];

        if (nCopies)                  // only increment if copies selected
            ++nCopiesDone;            // another copy completed

        // Update the status bar text with number of copies completed
        hStatus = GetDlgItem(hdlg, IDD_STATUSBAR);
        wsprintf(szbuf, "%i Copies Completed", nCopiesDone);
        SendMessage(hStatus, SB_SETTEXT, (WPARAM)1|0, (LPARAM)&szbuf);

        hPGBar = GetDlgItem(hdlg, IDD_PGBAR);

        // if this is the last one use DeltaPos to catch roundings that
        // may have occured.
        if (nCopiesDone == nCopies)
        {  
            SendMessage(hPGBar, PBM_DELTAPOS, (WPARAM)iPctLeft, 0);
            iPctLeft = 0;
            StopCopying(hdlg, TRUE);
        }
        else // just use one step per each completed copy.
        {
            int nStep = 100;

            SendMessage(hPGBar, PBM_STEPIT, 0, 0);

            nStep = 100;
            if (nCopies)
                nStep = nStep / nCopies;

            iPctLeft -= nStep;
            if (iPctLeft <= 0)
                iPctLeft = 0;
        }
    }

    // Set the focus back to the "Cancel" button
    hCtl = GetDlgItem(hdlg, IDD_STOP);
    SendMessage(hdlg, WM_NEXTDLGCTL, (WPARAM)hCtl, (LPARAM)TRUE);

    return TRUE;
}

//
//  FUNCTION: CmdnCopies(HWND, WORD, HWND)
//
//  PURPOSE: Checking the input for number of copies
//   
//  PARAMETERS:
//    hdlg - The window handling the command.
//    wCommand - The command to be handled (unused).
//    hwndCtrl - NULL (unused).
//
//  RETURN VALUE:
//    Always returns TRUE.
//
//  COMMENTS:
//    This is the code for WM_COMMAND with wParam == IDD_NCOPIES.  Processing
//    the EN_UPDATE notification message to check validity of input.  If the
//    input is invalid the status bar displays a message with the copy limits.
//    The global that holds the number of copies is not updated if the input
//    is invalid.
//

LRESULT CmdnCopies(HWND hdlg, WORD wCommand, WORD wNotify, HWND hwndCtrl)
{
    char szbuf[20];
    HWND hStatus;                // Handle to the Status Bar control
    int  iInCopies;
    

    if (wNotify == EN_UPDATE)
    {
        GetWindowText(hwndCtrl, szbuf, 4);
        iInCopies = atoi(szbuf);
        hStatus = GetDlgItem(hdlg, IDD_STATUSBAR);
        if ((iInCopies > 50) || (iInCopies < 0))
        { 
            MessageBeep(0);
            SendMessage(hStatus,
                        SB_SETTEXT,
                        (WPARAM)2|SBT_NOBORDERS,
                        (LPARAM)(LPSTR)"Num Copies 0-50");
       
            return FALSE;
        }
        else
        {
            nCopies = iInCopies;
            UpdateSB(hdlg);
            SendMessage(hStatus,
                        SB_SETTEXT,
                        (WPARAM)2|SBT_NOBORDERS,
                        (LPARAM)(LPSTR)"");
        }
    }


    return TRUE;
}

//
//  FUNCTION: CmdStart(HWND, WORD, HWND)
//
//  PURPOSE: Starts the copying process by calling StartCopying() function.
//   
//  PARAMETERS:
//    hdlg - The window handling the command.
//    wCommand - The command to be handled (unused).
//    hwndCtrl - NULL (unused).
//
//  RETURN VALUE:
//    Always returns TRUE.
//
//  COMMENTS:
//    This is the code for WM_COMMAND with wParam == IDOK.  
//

LRESULT CmdStart(HWND hdlg, WORD wCommand, WORD wNotify, HWND hwndCtrl)
{
    HWND hCtl;

    
    // Update the edit control and the status bar incase bad input was 
    // entered.
    hCtl = GetDlgItem(hdlg, IDD_UDCOPIES);
    SendMessage(hCtl, UDM_SETPOS, (WPARAM)0, (LPARAM)MAKELONG(nCopies, 0));

    // Start the actual copying.
    StartCopying(hdlg);

    return TRUE;
}

//
//  FUNCTION: CmdReset(HWND, WORD, HWND)
//
//  PURPOSE: Resets the Copier dialog box with the default values.
//
//  PARAMETERS:
//    hdlg - The window handling the command.
//    wCommand - The command to be handled (unused).
//    hwndCtrl - NULL (unused).
//
//  RETURN VALUE:
//    Always returns TRUE.
//
//  COMMENTS:
//    This is the code for WM_COMMAND with wParam == IDD_RESET.  
//

LRESULT CmdReset(HWND hdlg, WORD wCommand, WORD wNotify, HWND hwndCtrl)
{
    HWND hCtl;

    // Enable the controls and set the focus to the START button
    EnableControls(hdlg, TRUE);
    hCtl = GetDlgItem(hdlg, IDOK);
    SendMessage(hdlg, WM_NEXTDLGCTL, (WPARAM)hCtl, (LPARAM)TRUE);

    // Reset the dialog to its initial setup.
    return ResetDialog(hdlg);
}

//
//  FUNCTION: CmdStop(HWND, WORD, HWND)
//
//  PURPOSE: Stop/Interupts the copying process by calling StopCopying().
//           The dialog needs to be reset before other jobs could be 
//           processed.
//
//  PARAMETERS:
//    hdlg - The window handling the command.
//    wCommand - The command to be handled (unused).
//    hwndCtrl - NULL (unused).
//
//  RETURN VALUE:
//    Always returns TRUE.
//
//  COMMENTS:
//    This is the code for WM_COMMAND with wParam == IDD_STOP.  
//

LRESULT CmdStop(HWND hdlg, WORD wCommand, WORD wNotify, HWND hwndCtrl)
{
    return StopCopying(hdlg, FALSE);
}

//
//  FUNCTION: CmdDone(HWND, WORD, HWND)
//
//  PURPOSE: Close the Copier dialog box and free its related data.
//
//  PARAMETERS:
//    hdlg - The window handling the command.
//    wCommand - The command to be handled (unused).
//    hwndCtrl - NULL (unused).
//
//  RETURN VALUE:
//    Always returns TRUE.
//
//  COMMENTS:
//    This is the code for WM_COMMAND with wParam == IDCANCEL.  Calls 
//    EndDialog to finish the dialog session.
//

LRESULT CmdDone(HWND hdlg, WORD wCommand, WORD wNotify, HWND hwndCtrl)
{
    RemoveProp(hdlg, "defcopy");
    RemoveProp(hdlg, "defenlrg");
    RemoveProp(hdlg, "defshade");
    EndDialog(hdlg, TRUE);          // Exit the dialog

    return TRUE;
}

//
//  FUNCTION: InitStatusBar(HWND)
//
//  PURPOSE: Creates and initializes the Status Bar control and the Progress
//           Bar control
//
//  PARAMETERS:
//    hdlg - The dialogs window handle.
//
//  RETURN VALUE:
//    Handle of the Status Bar if successful
//
//  COMMENTS:
//

HWND InitStatusBar(HWND hdlg)
{
    #define SBP_COPYTXT   50            // 1st - "Copy:"
    #define SBP_COPYSTAT 250            // 2nd - Copying selected
    #define SBP_PGBAR    380            // 3rd - Progress bar or error message 
    #define SBP_DLGSTAT   -1            // 4th - Status of the dialog

    int ptSBParts[4] = {SBP_COPYTXT,
                        SBP_COPYSTAT,
                        SBP_PGBAR,
                        SBP_DLGSTAT} ;  // The number of parts/sections
    HWND hStatus;                       // Status Bar control handle
    HWND hPGBar;                        // Progress Bar control handle
    RECT rc;                            // Status bar Rect 
    int  sbBorders[3];                  // Status bar border information
    int  x, y, width, height;           // Progress bar co-ordinates

    hStatus = CreateWindow(STATUSCLASSNAME,
                           "Statusbar Sample",
                           WS_CHILD | WS_VISIBLE | WS_BORDER,
                           0, 0, 0, 0,                // these are ignored
                           hdlg,
                           (HMENU)IDD_STATUSBAR,
                           hInst,
                           NULL);

    if (hStatus)
    {
        // Set the parts for the status bar.  the number '4' indicated here 
        // matches the number of parts passed in ptSBParts.
        SendMessage(hStatus, SB_SETPARTS, 4, (LPARAM)(LPINT)ptSBParts);

        // Initialize each of the status bar parts.
        SendMessage(hStatus,
                    SB_SETTEXT,
                    (WPARAM)0|SBT_NOBORDERS,
                    (LPARAM)(LPSTR)"Copy:");
        UpdateSB(hdlg);
        SendMessage(hStatus,
                    SB_SETTEXT,
                    (WPARAM)2|SBT_NOBORDERS,
                    (LPARAM)(LPSTR)"");
        SendMessage(hStatus,
                    SB_SETTEXT,
                    (WPARAM)3|0,
                    (LPARAM)(LPSTR)"Idle");
    }
    else
        return NULL;

    // Calculate the dimensions of the progress bar with respect to the
    // Status bar by:
    //
    //  1) Get the client area of the status bar and convert its co-ordinates
    //        to the client co-ordinates of the dialog since progress bar is
    //        child of the dialog.
    //  2) Get the border dimentions of the status bar using SB_GETBORDERS
    //  3) Place the progress bar after where copying status is (SBP_COPYSTAT).
    //        Use the dimensions of the status bar and the border values 
    //        retrieved and calculate the x, y, width and height of the 
    //        progress bar.
    GetClientRect(hStatus, &rc);
    MapWindowPoints(hStatus, hdlg, (LPPOINT)&rc.left, 2);

    SendMessage(hStatus, SB_GETBORDERS, (WPARAM)0, (LPARAM)&sbBorders);

    x = rc.left + SBP_COPYSTAT + (sbBorders[2] / 2);
    y = rc.top + (2 * sbBorders[1]);
    width = SBP_PGBAR - SBP_COPYSTAT;
    height = rc.bottom - rc.top - (4 * sbBorders[1]);

    // Create the progress bar.  The Progress bar range 0-100 by default
    // and its step increment is 10 by default.  These default values are
    // displayed in the edit controls.
    hPGBar = CreateWindow(PROGRESS_CLASS,
                          "Progress Bar",
                          WS_VISIBLE | WS_CHILD,
                          x,
                          y,
                          width,
                          height,
                          hdlg,
                          (HMENU)IDD_PGBAR,
                          hInst,
                          NULL);
    if (hPGBar)
        ShowWindow(hPGBar, SW_HIDE);

    return hStatus;
}

//
//  FUNCTION: InitTBar(HWND)
//
//  PURPOSE: Creates and initializes the Track bar control
//
//  PARAMETERS:
//    hdlg - The dialogs window handle.
//    IDTB - ID of the Track bar to be created.
//    x - x position of the where the control is created.
//    y - y position of the where the control is created.
//    nWidth - Width of the Track bar control.
//    nHeight - Height of the Track bar control.
//    nMin - The minumum range of the Track bar.
//    nMax - The maximum range of the Track bar.
//    nPos - The initial thumb position of the Track bar.
//
//  RETURN VALUE:
//    Handle of the Track bar if successful
//
//  COMMENTS:
//

HWND InitTBar(HWND hdlg,
              int IDTB,
              int x,
              int y,
              int nWidth,
              int nHeight,
              int nMin,
              int nMax,
              int nPos)
{
    HWND hTBar;

    // Create the track bar window with TBS_AUTOTICKS which draws and updates
    // the ticks automatically.
    hTBar = CreateWindow(TRACKBAR_CLASS,
                         "Track Bar",
                         TBS_AUTOTICKS | WS_VISIBLE | WS_CHILD | WS_TABSTOP,
                         x,
                         y,
                         nWidth,
                         nHeight,
                         hdlg,
                         (HMENU)IDTB,
                         hInst,
                         NULL);

    if (hTBar)
    {
        // Set the minimum and maximum range of the track bar.
        SendMessage(hTBar,
                    TBM_SETRANGE,
                    (WPARAM)(BOOL)TRUE,
                    (LPARAM)MAKELONG(nMin, nMax));

        // Set the track bar thumb to the specified position.
        SendMessage(hTBar,
                    TBM_SETPOS,
                    (WPARAM)(BOOL)TRUE,
                    (LPARAM)(LONG)nPos);
    }

    return hTBar;
}

//
//  FUNCTION: InitUDCtl(HWND, int)
//
//  PURPOSE: Creates and initializes the Up/Down control
//
//  PARAMETERS:
//    hdlg - The dialogs window handle.
//    IDEdit - ID of the edit control to contain the Up/Down control
//    IDUD - ID of the Up/Down control
//    nUpper - The upper limit of the Up/Down control
//    nLower - The lower limit of the Up/Down control
//    nPos - The initial position of the the Up/Down control
//
//  RETURN VALUE:
//    Handle of the Up/Down Control if successful.
//
//  COMMENTS:
//

HWND InitUDCtl(HWND hdlg,
               int IDEdit,
               int IDUD,
               int nUpper,
               int nLower,
               int nPos)
{
    HWND hEdit;
    HWND hUDCtl;
    RECT rc;
    int  cx, cy, x, y;

    // Get the handle of the edit control which is used as the buddy of the
    // Up/Down control.  The dialog is still the parent of the control.
    // This UD control is using the right align style and which aligns the
    // UD control to the right border of the buddy and ignores the x,y,cx
    // and cy values.  x, y, cx and cy are place here to demonstrate how
    // the Up/Down control can be placed in the buddy manually!
    hEdit = GetDlgItem(hdlg, IDEdit);
    GetClientRect(hEdit, &rc);
    MapWindowPoints(hEdit, hdlg, (LPPOINT)&rc.left, 2);

    cx = 10;
    cy = (rc.bottom - rc.top) + 2;
    x  = rc.right;
    y  = rc.top - 1;

    // Create the Up/Down control. 
    // NOTE: Since UDS_ALIGNRIGHT is used, x, y, cx and cy are ignored.
    //       Remove UDS_ALIGNRIGHT and the UD control can be placed any where.
    hUDCtl = CreateWindow(UPDOWN_CLASS,
                          NULL,
                          WS_CHILD | WS_BORDER | WS_VISIBLE |
                          UDS_WRAP | UDS_ARROWKEYS |
                          UDS_ALIGNRIGHT | UDS_SETBUDDYINT, 
                          x, y,
                          cx, cy,
                          hdlg,              // handle of the parent
                          (HMENU)IDUD,       // ID of the UD control
                          hInst,
                          NULL);

    if (hUDCtl)
    {
        // Set the buddy for the Up/Down control and initialize the lower
        // and upper limit of the control and its initial position
        SendMessage(hUDCtl, UDM_SETBUDDY, (WPARAM)hEdit, 0L);
        SendMessage(hUDCtl, UDM_SETRANGE, 0, (LPARAM)MAKELONG(nUpper, nLower));
        SendMessage(hUDCtl, UDM_SETPOS, 0, (LPARAM)MAKELONG(nPos, 0));
    }

    return hUDCtl;
}

//
//  FUNCTION: GetTBarPos(HWND, HWND, LPRECT)
//
//  PURPOSE: Calculates the position of the Track bar control based on its
//    its static text title.
//
//  PARAMETERS:
//    hdlg - Handle of the dialog.
//    hStatic - The related Static control handle.
//    lprc - When returned contains the position of the Track Bar Control
//
//  RETURN VALUE:
//    None
//
//  COMMENTS:
//

void GetTBarPos(HWND hdlg, HWND hStatic, LPRECT lprc)
{
    RECT strc;          // Rectangle containing the static control

    // Get the dimentions of the static window containing the Track bar
    // header.
    GetWindowRect(hStatic,&strc);

    // Convert the rectangle from screen units to client units relative to
    // the dialog box.
    ScreenToClient(hdlg, (LPPOINT)&strc.left);
    ScreenToClient(hdlg, (LPPOINT)&strc.right);

    // return the calculated rect for the track bar.
    lprc->left   = strc.left;
    lprc->right  = strc.right;
    lprc->top    = strc.bottom + 2;
    lprc->bottom = lprc->top + (strc.bottom - strc.top);

    return;
}

//
//  FUNCTION: UpdateSB(HWND hdlg)
//
//  PURPOSE: Updates the status bar text for the copy information
//
//  PARAMETERS:
//    hdlg - Handle of the dialog
//
//  RETURN VALUE:
//    None
//
//  COMMENTS:
//

void UpdateSB(HWND hdlg)
{
    HWND hStatus;
    char szbuf[80];
    char Shade[20];

    // Get the text associated with the shadings
    if (!iShade)
        lstrcpy(Shade, "Lighter");
    else if (iShade == 2)
        lstrcpy(Shade, "Darker");
    else
        lstrcpy(Shade, "Auto");

    hStatus = GetDlgItem(hdlg, IDD_STATUSBAR);

    // Show users current selection on the copier panel
    wsprintf(szbuf, "%i at %i%% with %s Shading",
             nCopies,
             iEnlargementPct,
             Shade);
    SendMessage(hStatus, SB_SETTEXT, (WPARAM)1|0, (LPARAM)&szbuf);

    return;
}

//
//  FUNCTION: EnableControls(HWND, BOOL)
//
//  PURPOSE: Enables or Disables controls based on the passed parameter
//
//  PARAMETERS:
//    hdlg - The dialogs window handle.
//    fEnable - flag determining whether the window should be enabled or 
//       disabled.
//
//  RETURN VALUE:
//    Always returns TRUE.
//
//  COMMENTS:
//

BOOL EnableControls(HWND hdlg, BOOL fEnable)
{
    HWND hCtl;

    // Number of Copies edit control is disabled while copying
    hCtl = GetDlgItem(hdlg, IDD_NCOPYTXT);
    EnableWindow(hCtl, fEnable);
    hCtl = GetDlgItem(hdlg, IDD_NCOPIES);
    EnableWindow(hCtl, fEnable);

    // Start button is disabled while copying
    hCtl = GetDlgItem(hdlg, IDOK);
    EnableWindow(hCtl, fEnable);

    // Re-Set Button is disabled while copying
    hCtl = GetDlgItem(hdlg, IDD_RESET);
    EnableWindow(hCtl, fEnable);

    // Enlargement Tracker control is disabled while copying
    hCtl = GetDlgItem(hdlg, IDD_ENLARGE);
    EnableWindow(hCtl, fEnable);
    hCtl = GetDlgItem(hdlg, IDD_TBENLARGE);
    EnableWindow(hCtl, fEnable);

    // Darkness Tracker control is disabled while copying
    hCtl = GetDlgItem(hdlg, IDD_DARK);
    EnableWindow(hCtl, fEnable);
    hCtl = GetDlgItem(hdlg, IDD_TBDARK);
    EnableWindow(hCtl, fEnable);

    // This button is Cancel when copying and Clear when Idle
    hCtl = GetDlgItem(hdlg, IDD_STOP);
    EnableWindow(hCtl, !fEnable);

    return TRUE;
}

//
//  FUNCTION: StartCopying(HWND hdlg)
//
//  PURPOSE: Start the copying process
//
//  PARAMETERS:
//    hdlg - Handle of the dialog
//
//  RETURN VALUE:
//    TRUE always
//
//  COMMENTS:
//    The basic initialization for the copy process is done here.  The
//    Progress Bar steps are set here (the default is 10). Then a timer 
//    is set.
//

BOOL StartCopying(HWND hdlg)
{
    HWND hCtl;
    HWND hPGBar;
    int  nStep;
 
    // Set the copy flag.  It gets reset either in StopCopying.
    fIsCopyState = TRUE;

    // Disable the controls
    EnableControls(hdlg, FALSE);

    // Set the focus to the Cancel button
    hCtl = GetDlgItem(hdlg, IDD_STOP);
    SendMessage(hdlg, WM_NEXTDLGCTL, (WPARAM)hCtl, (LPARAM)TRUE);
    
    // Update the status bar with the new information
    UpdateSB(hdlg);
    hCtl = GetDlgItem(hdlg, IDD_STATUSBAR);
    SendMessage(hCtl, SB_SETTEXT, (WPARAM)3|0, (LPARAM)(LPSTR)"Copying");

    // display the progress bar
    hCtl = GetDlgItem(hdlg, IDD_PGBAR);
    ShowWindow(hCtl, SW_SHOW);

    // Set the Steps for Progress bar
    nStep = 100;
    if (nCopies)
        nStep = nStep / nCopies;
    hPGBar = GetDlgItem(hdlg, IDD_PGBAR);
    SendMessage(hPGBar, PBM_SETSTEP, (WPARAM)nStep, 0);

    // Start the timer for "pretend copying"
    iTimeCtr = 0;
    nCopiesDone = 0;
    TimerID = SetTimer(hdlg, 1, 250, NULL);

    return TRUE;
}

//
//  FUNCTION: ResetDialog(HWND hdlg)
//
//  PURPOSE: Resets the dialog to its default values
//
//  PARAMETERS:
//    hdlg - Handle of the dialog
//
//  RETURN VALUE:
//    TRUE always
//
//  COMMENTS:
//

BOOL ResetDialog(HWND hdlg)
{
    HWND hCtl;

    // Reset the Number of Copies edit control through its Up/Down control
    nCopies = (int)GetProp(hdlg, "defcopy");
    hCtl = GetDlgItem(hdlg, IDD_UDCOPIES);
    SendMessage(hCtl, UDM_SETPOS, (WPARAM)0, (LPARAM)MAKELONG(nCopies, 0));

    // Reset the Enlargement Percentage track bar
    iEnlargementPct = (int)GetProp(hdlg, "defenlrg");
    hCtl = GetDlgItem(hdlg, IDD_TBENLARGE);
    SendMessage(hCtl,
                TBM_SETPOS,
                (WPARAM)(BOOL)TRUE,
                (LPARAM)(LONG)iEnlargementPct);
    iEnlargementPct = iEnlargementPct * 10;

    // Reset the Shading density track bar
    iShade = (int)GetProp(hdlg, "defshade");
    hCtl = GetDlgItem(hdlg, IDD_TBDARK);
    SendMessage(hCtl,
                TBM_SETPOS,
                (WPARAM)(BOOL)TRUE,
                (LPARAM)(LONG)iShade);

    // Update the dialogs status to Idle
    hCtl = GetDlgItem(hdlg, IDD_STATUSBAR);
    SendMessage(hCtl, SB_SETTEXT, (WPARAM)3|0, (LPARAM)(LPSTR)"Idle");

    // Update the status bar
    UpdateSB(hdlg);

    // Hide and reset the progress bar
    hCtl = GetDlgItem(hdlg, IDD_PGBAR);
    ShowWindow(hCtl, SW_HIDE);
    SendMessage(hCtl, PBM_SETPOS, (WPARAM)0, (LPARAM)0);

    return TRUE;
}

//
//  FUNCTION: StopCopying(HWND hdlg,BOOL fIsComplete)
//
//  PURPOSE: Stops the copying process
//
//  PARAMETERS:
//    hdlg - Handle of the dialog
//
//  RETURN VALUE:
//    TRUE always
//
//  COMMENTS:
//    Basic clean up.  The Progress Bar is reset when the copying is complete.
//

BOOL StopCopying(HWND hdlg, BOOL fIsComplete)
{
    HWND hCtl;

    if (fIsComplete)
    {
        MessageBeep(0);             // Anounce completion

        // Update the status bar with the new information
        UpdateSB(hdlg);
        hCtl = GetDlgItem(hdlg, IDD_STATUSBAR);
        SendMessage(hCtl, SB_SETTEXT, (WPARAM)3|0, (LPARAM)(LPSTR)"Idle");
  
        // Hide and reset the progress bar
        hCtl = GetDlgItem(hdlg, IDD_PGBAR);
        ShowWindow(hCtl, SW_HIDE);
        SendMessage(hCtl, PBM_SETPOS, (WPARAM)0, (LPARAM)0);

        // Enable the controls
        EnableControls(hdlg, TRUE);

        // Set the input focus to the Start button
        hCtl = GetDlgItem(hdlg, IDOK);
        SendMessage(hdlg, WM_NEXTDLGCTL, (WPARAM)hCtl, (LPARAM)TRUE);
    }
    else
    {
        // Copying was interupted so the dialog needs to be reset via Re-Set
        // button.  So disable the Cancel button and enable the reset button
        // and give the focus to Re-Set Button.
        // Do not enable the controls until the Re-Set button is pushed.
        // Also the status of the dialog is set to Re-Set.

        hCtl = GetDlgItem(hdlg, IDD_STOP);
        EnableWindow(hCtl, FALSE);

        hCtl = GetDlgItem(hdlg, IDD_RESET);
        EnableWindow(hCtl, TRUE);
        SendMessage(hdlg, WM_NEXTDLGCTL, (WPARAM)hCtl, (LPARAM)TRUE);
  
        hCtl = GetDlgItem(hdlg, IDD_STATUSBAR);
        SendMessage(hCtl, SB_SETTEXT, (WPARAM)3|0, (LPARAM)(LPSTR)"Re-Set!");
    }
 
    // Set the copy flag to indicate end of copying.
    fIsCopyState = FALSE;

    KillTimer(hdlg, TimerID);

    return TRUE;
}
