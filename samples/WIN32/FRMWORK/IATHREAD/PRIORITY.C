// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
// PARTICULAR PURPOSE.
//
// Copyright (C) 1993-1995  Microsoft Corporation.  All Rights Reserved.
//
//  MODULE:   priority.c
//
//  PURPOSE:   Displays the "Priority" dialog box
//
//  FUNCTIONS:
//    CmdMCThreadPriority  - Displays the "About" dialog box
//    PriorityDlgProc      - Processes messages for "Priority" dialog box.
//    MsgPriorityInit      - To initialize the about box with version info
//                           from resources.
//    MsgPriorityCommand   - Process WM_COMMAND message sent to the about box.
//    CmdPriorityOk        - Set priority/class user has selected.
//    CmdPriorityCancel    - Clean-up actions if user cancels prioity dialog.
//
//  COMMENTS:
//
//

#include <windows.h>            // required for all Windows applications
#include <windowsx.h>
#include "globals.h"            // prototypes specific to this application
#include "resource.h"


// Module-specific message- and command-handling functions
LRESULT CALLBACK PriorityDlgProc(HWND, UINT, WPARAM, LPARAM);
LRESULT MsgPriorityInit(HWND, UINT, WPARAM, LPARAM);
LRESULT MsgPriorityCommand(HWND, UINT, WPARAM, LPARAM);
LRESULT CmdPriorityOk(HWND, WORD, WORD, HWND);
LRESULT CmdPriorityCancel(HWND, WORD, WORD, HWND);


// Priority dialog message table definition.
static MSD rgmsdPriority[] =
{
    {WM_COMMAND,    MsgPriorityCommand},
    {WM_INITDIALOG, MsgPriorityInit}
};

static MSDI msdiPriority =
{
    sizeof(rgmsdPriority) / sizeof(MSD),
    rgmsdPriority,
    edwpNone
};

// Priority dialog command table definition.
static CMD rgcmdPriority[] =
{
    {IDOK,     CmdPriorityOk},
    {IDCANCEL, CmdPriorityCancel}
};

static CMDI cmdiPriority =
{
    sizeof(rgcmdPriority) / sizeof(CMD),
    rgcmdPriority,
    edwpNone
};


// Priority class listbox data.
static LISTBOXDATA rPriorityClassListData[] = {
    { "Real Time", REALTIME_PRIORITY_CLASS },
    { "High",      HIGH_PRIORITY_CLASS     },
    { "Normal",    NORMAL_PRIORITY_CLASS   },
    { "Idle",      IDLE_PRIORITY_CLASS     }
    };


// Thread priority listbox data.
static LISTBOXDATA rThreadPriorityListData[] = {
    { "Time Critical", THREAD_PRIORITY_TIME_CRITICAL },
    { "Highest",       THREAD_PRIORITY_HIGHEST       },
    { "Above Normal",  THREAD_PRIORITY_ABOVE_NORMAL  },
    { "Normal",        THREAD_PRIORITY_NORMAL        },
    { "Below Normal",  THREAD_PRIORITY_BELOW_NORMAL  },
    { "Lowest",        THREAD_PRIORITY_LOWEST        },
    { "Idle",          THREAD_PRIORITY_IDLE          }
    };


//
//  FUNCTION: CmdMCThreadPriority(HWND, WORD, WORD, HWND)
//
//  PURPOSE: Displays the "Priority" dialog box
//
//  PARAMETERS:
//    hwnd      - Window handle
//    wCommand  - IDM_THREAD_PRIORITY (unused)
//    wNotify   - Notification number (unused)
//    hwndCtrl  - NULL (unused)
//
//  RETURN VALUE:
//
//    Always returns 0 - Message handled
//
//  COMMENTS:
//    Call DialogBox() to display the priority dialog box.

LRESULT CmdMCThreadPriority(HWND hwnd,
                            WORD wCommand,
                            WORD wNotify,
                            HWND hwndCtrl
                           )
{
    DialogBox(hInst,
              MAKEINTRESOURCE(IDD_PRIORITY),
              hwnd,
              (DLGPROC)PriorityDlgProc);

    return 0;
}


//
//  FUNCTION: PriorityDlgProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for "Priority" dialog box.
//
//  PARAMETERS:
//    hdlg     - window handle of the dialog box
//    uMessage - type of message
//    wparam   - message-specific information
//    lparam   - message-specific information
//
//  RETURN VALUE:
//    TRUE     - message handled
//    FALSE    - message not handled
//
//  COMMENTS:
//

LRESULT CALLBACK PriorityDlgProc(HWND   hdlg,
                                 UINT   uMessage,
                                 WPARAM wparam,
                                 LPARAM lparam)
{
    return DispMessage(&msdiPriority, hdlg, uMessage, wparam, lparam);
}


//
//  FUNCTION: MsgPriorityInit(HWND, UINT, WPARAM, LPARAM)
////  PURPOSE: To initialize the priority dialog box.
//
//  PARAMETERS:
//    hwnd     - The window handing the message.
//    uMessage - WM_INITDIALOG         (unused).
//    wparam   - Message specific data (unused).
//    lparam   - Message specific data (unused).
//
//  RETURN VALUE:
//    Always returns 0 - message handled.
//
//  COMMENTS:
//    Initializes Priority Class and Thread Priority list boxes before the
//    dialog is displayed.
//

LRESULT MsgPriorityInit(HWND   hdlg,
                        UINT   uMessage,
                        WPARAM wparam,
                        LPARAM lparam)
{
    DWORD  dwPriorityClass = 0;
    int    nPriority       = 0;
    int    nIndex          = 0;
    int    cItems          = 0;
    int    i               = 0;


    // Initialize priority class listbox and set the default priority
    // class selection to the process's current priority class.

    cItems = sizeof(rPriorityClassListData) / sizeof(LISTBOXDATA);

    FillListBox(hdlg, 
                IDC_PRIORITY_CLASS_LIST, 
                rPriorityClassListData,
                cItems);

    dwPriorityClass = GetPriorityClass(GetCurrentProcess());

    for (i = 0; i < cItems; i++)
        if (dwPriorityClass == (DWORD)rPriorityClassListData[i].dwData)
            break;   //Found the priority class we're looking for.

    // This test should always fail because we should always have all of the
    // different priority classes in the listbox.  In case a new priority
    // is defined in a later version of the operating system, we want to know
    // about it in the debugging version, and handle it gracefully in the
    // release version.
#ifdef _DEBUG
    if (i >= cItems)
        MessageBox(ghwndFrame, 
                   "MsgPriorityInit(): Invalid priority class", 
                   "Priority.c", 
                   MB_OK);
#else
    if (i >= cItems)
        i = 0;      // Set i to a reasonable value.
#endif

    nIndex = SendDlgItemMessage(hdlg, 
                                IDC_PRIORITY_CLASS_LIST,
                                LB_FINDSTRINGEXACT, 
                                0,
                                (LPARAM)rPriorityClassListData[i].szString);
    SendDlgItemMessage(hdlg, 
                       IDC_PRIORITY_CLASS_LIST, 
                       LB_SETCURSEL,
                       nIndex, 
                       0);


    // Initialize the thread priority listbox and set the default thread
    // priority selection to the MDI child window's thread's current
    // priority.

    cItems = sizeof(rThreadPriorityListData) / sizeof(LISTBOXDATA);

    FillListBox(hdlg, 
                IDC_THREAD_PRIORITY_LIST, 
                rThreadPriorityListData,
                cItems);

    nPriority = GetThreadPriority(GetCurChildWindowThread());

    for (i = 0; i < cItems; i++)
        if (nPriority == (int)rThreadPriorityListData[i].dwData)
           break;   //Found the thread priority we're looking for.

    // This test should always fail because we should always have all of the
    // different thread priorities in the listbox.  In case a new priority
    // is defined in a later version of the operating system, we want to know
    // about it in the debugging version, and handle it gracefully in the
    // release version.
#ifdef _DEBUG
    if (i >= cItems)
        MessageBox(ghwndFrame, 
                   "MsgPriorityInit(): Invalid thread priority", 
                   "Priority.c", 
                   MB_OK);
#else
    if (i >= cItems)
        i = 0;      // Set i to a reasonable value.
#endif

    nIndex = SendDlgItemMessage(hdlg, 
                                IDC_THREAD_PRIORITY_LIST,
                                LB_FINDSTRINGEXACT, 
                                0,
                                (LPARAM)rThreadPriorityListData[i].szString);
    SendDlgItemMessage(hdlg, 
                       IDC_THREAD_PRIORITY_LIST, 
                       LB_SETCURSEL,
                       nIndex, 
                       0);


    // Center the dialog over the application window
    CenterWindow(hdlg, GetWindow(hdlg, GW_OWNER));
    return TRUE;
}

//
//  FUNCTION: MsgPriorityCommand(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: Process WM_COMMAND messages sent to the about box.
//
//  PARAMETERS:
//    hwnd     - The window handing the message.
//    uMessage - WM_COMMAND            (unused).
//    wparam   - Message specific data (unused).
//    lparam   - Message specific data (unused).
//
//  RETURN VALUE:
//    Always returns 0 - message handled.
//
//  COMMENTS:
//

LRESULT MsgPriorityCommand(HWND   hwnd,
                           UINT   uMessage,
                           WPARAM wparam,
                           LPARAM lparam)
{
    return DispCommand(&cmdiPriority, hwnd, wparam, lparam);
}

//
//  FUNCTION: CmdPriorityOk(HWND, WORD, HWND)
//
//  PURPOSE: To set thread priority and priority class according to user's
//           choice and then exit the dialog.
//
//  PARAMETERS:
//    hwnd     - The window handling the command.
//    wCommand - IDOK                 (unused).
//    wNotify  - Notification number  (unused)
//    hwndCtrl - NULL                 (unused).
//
//  RETURN VALUE:
//    Always returns TRUE.
//
//  COMMENTS:
//    Calls EndDialog to finish the dialog session.
//

LRESULT CmdPriorityOk(HWND hdlg,
                      WORD wCommand,
                      WORD wNotify,
                      HWND hwndCtrl)
{
    int    nIndex          = 0;
    int    nPriority       = 0;
    HANDLE hThread         = NULL;
    DWORD  dwPriorityClass = 0;

    // Set priority class and thread's priority based on what user has
    // selected in the priority class and thread priority list boxes.
    // If the user hasn't made a selection, then leave priority class
    // and thread priority alone.

    nIndex = SendDlgItemMessage(hdlg, 
                               IDC_PRIORITY_CLASS_LIST,
                               LB_GETCURSEL, 
                               0, 
                               0);
    if (nIndex != LB_ERR)
        {
        dwPriorityClass = SendDlgItemMessage(hdlg, 
                                             IDC_PRIORITY_CLASS_LIST,
                                             LB_GETITEMDATA, 
                                             (WPARAM)nIndex,
                                             0);
        SetPriorityClass(GetCurrentProcess(), dwPriorityClass);
        }

    nIndex = SendDlgItemMessage(hdlg, 
                                IDC_THREAD_PRIORITY_LIST,
                                LB_GETCURSEL, 
                                0, 
                                0);
    if (nIndex != LB_ERR)
        {
        hThread = GetCurChildWindowThread();
        nPriority = (int)SendDlgItemMessage(hdlg, 
                                            IDC_THREAD_PRIORITY_LIST,
                                            LB_GETITEMDATA, 
                                            (WPARAM)nIndex,
                                            0);

        SetThreadPriority(hThread, nPriority);
        }

    EndDialog(hdlg, TRUE);          // Exit the dialog
    return TRUE;
}


//
//  FUNCTION: CmdPriorityCancel(HWND, WORD, HWND)
//
//  PURPOSE: Exit the dialog box and cancel any selection user might have
//           made.
//
//  PARAMETERS:
//    hwnd     - The window handling the command.
//    wCommand - IDCANCEL             (unused).
//    wNotify  - Notification number  (unused)
//    hwndCtrl - NULL                 (unused).
//
//  RETURN VALUE:
//    Always returns TRUE.
//
//  COMMENTS:
//    Calls EndDialog to finish the dialog session.
//

LRESULT CmdPriorityCancel(HWND hdlg,
                          WORD wCommand,
                          WORD wNotify,
                          HWND hwndCtrl)
{
    // Leave thread's priority and class alone.

    EndDialog(hdlg, FALSE);          // Exit the dialog
    return TRUE;
}

