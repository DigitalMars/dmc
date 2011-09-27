// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
// PARTICULAR PURPOSE.
//
// Copyright (C) 1993-1995  Microsoft Corporation.  All Rights Reserved.
//
//  MODULE:   modal.c
//
//  PURPOSE:   Displays the "modal" dialog box
//
//  FUNCTIONS:
//    CmdModal        - Displays the "Modal" dialog box
//    Modal           - Processes messages for "Modal" dialog box.
//    MsgModalInit    - To initialize the modal box with version info
//                      from resources.
//    MsgModalCommand - Process WM_COMMAND message sent to the modal box.
//    CmdModalDone    - Free the modal box and related data.
//
//  COMMENTS:
//
//

#include <windows.h>            // required for all Windows applications
#include <windowsx.h>
#ifdef WIN16
#include "win16ext.h"           // required only for win16 applications
#endif
#include "globals.h"            // prototypes specific to this application
#include "modal.h"

LRESULT MsgModalInit(HWND, UINT, WPARAM, LPARAM);
LRESULT MsgModalCommand(HWND, UINT, WPARAM, LPARAM);
LRESULT CmdModalDone(HWND, WORD, WORD, HWND);
LRESULT CmdModalIncludeBoys(HWND, WORD, WORD, HWND);
LRESULT CmdModalStudents(HWND, WORD, WORD, HWND);    

// Modal dialog message table definition.
MSD rgmsdModal[] =
{
    {WM_COMMAND,    MsgModalCommand},
    {WM_INITDIALOG, MsgModalInit}
};

MSDI msdiModal =
{
    sizeof(rgmsdModal) / sizeof(MSD),
    rgmsdModal,
    edwpNone
};

// Modal dialog command table definition.
CMD rgcmdModal[] =
{
    {IDD_INCLUDEBOYS, CmdModalIncludeBoys},
    {IDD_STUDENTS,    CmdModalStudents},
    {IDOK,            CmdModalDone},
    {IDCANCEL,        CmdModalDone}
};

CMDI cmdiModal =
{
    sizeof(rgcmdModal) / sizeof(CMD),
    rgcmdModal,
    edwpNone
};

// Module specific constants
#define MAXNAMELEN  20
#define MAXTITLELEN 40
#define MAXPHONELEN 20

// Modal dialog student data structure
typedef struct tagSTUDENTINFO
{
    char szName[ MAXNAMELEN];
    char szTitle[MAXTITLELEN];
    char szPhone[MAXPHONELEN];
    BOOL bBoy;
}STUDENTINFO;

// Module specific globals

STUDENTINFO rgStudentInfo[]=
{
    {"Tom",    "Cool Dude",     "555-1212", TRUE},
    {"Dick",   "Tom's Friend",  "555-1212", TRUE},
    {"Harry",  "Harry who?",    "555-1212", TRUE},
    {"Jane",   "Tom's Friend",  "555-1212", FALSE},
    {"Hilary", "Bill's Friend", "555-1212", FALSE},
    {"Martha", "Nice girl",     "555-1212", FALSE}
};


//
//  FUNCTION: Modal(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for "Modal" dialog box.
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
//
//     Display version information from the version section of the
//     application resource.
//
//     Wait for user to click on "Ok" button, then close the dialog box.
//

LRESULT CALLBACK Modal(HWND hdlg, UINT uMessage, WPARAM wparam, LPARAM lparam)
{
    return DispMessage(&msdiModal, hdlg, uMessage, wparam, lparam);
}

//
//  FUNCTION: PopulateStudentListbox(HWND hListbox, BOOL bShowBoys)
//
//  PURPOSE: To initialize the student listbox
//
//  PARAMETERS:
//    hwnd - The window handle to the student listbox
//    BOOL - TRUE if we should include the Boys in this listbox
//
//  RETURN VALUE:
//    void
//
//  COMMENTS:
//    Clear out the current entries in the students listbox, and
//    re-populate with the new setting.  The item data will give us
//    the index into the rgStudentInfo array for this student.
//    See CmdModalStudent() for usage of the item data
//

void PopulateStudentListbox(HWND hListbox, BOOL bShowBoys)
{
    int  i, iIndex;
    BOOL bShowThisOne;

    SendMessage(hListbox, LB_RESETCONTENT, 0, 0L);

    for (i = 0; i < sizeof(rgStudentInfo) / sizeof(STUDENTINFO); i++)
    {
        if (bShowBoys)
            bShowThisOne = TRUE;
        else
           bShowThisOne = !(rgStudentInfo[i].bBoy);

        if (bShowThisOne)
        {
            iIndex = (int)SendMessage(hListbox, 
                                      LB_ADDSTRING,
                                      0, 
                                      (LONG)(LPSTR)rgStudentInfo[i].szName);
            SendMessage(hListbox, LB_SETITEMDATA, (WPARAM)iIndex, (LONG)i);
        }
    }

}

//
//  FUNCTION: UpdateControls(HWND, HWND)
//
//  PURPOSE: To update the edit controls with new information based on 
//           the current selection in the IDD_STUDENTS listbox.
//
//  PARAMETERS:
//    hdlg        - Handle to the parent dialog
//    hwndListbox - Handle to the IDD_STUDENTS listbox control
//
//  RETURN VALUE:
//    void
//
//  COMMENTS:
//

void UpdateControls(HWND hdlg, HWND hwndListbox)
{
    int   iIndex;
    DWORD wItemData;

    iIndex = (int)SendMessage(hwndListbox, LB_GETCURSEL, 0, 0L);

    if (LB_ERR == iIndex)
        iIndex=0;

    wItemData = (WORD)SendMessage(hwndListbox, 
                                  LB_GETITEMDATA, 
                                  (WPARAM)iIndex,
                                  0L);
    SendDlgItemMessage(hdlg, 
                       IDD_NAME, 
                       WM_SETTEXT, 
                       0, 
                       (LONG)(LPSTR)rgStudentInfo[wItemData].szName);

    SendDlgItemMessage(hdlg, 
                       IDD_TITLE,
                       WM_SETTEXT, 
                       0, 
                       (LONG)(LPSTR)rgStudentInfo[wItemData].szTitle);

    SendDlgItemMessage(hdlg, 
                       IDD_PHONE,
                       WM_SETTEXT, 
                       0, 
                       (LONG)(LPSTR)rgStudentInfo[wItemData].szPhone);
    return;
}

//
//  FUNCTION: MsgModalInit(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: To initialize the modal box with version info from resources.
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
//    each of the static text boxes in the modal box.
//

LRESULT MsgModalInit(HWND hdlg, UINT uMessage, WPARAM wparam, LPARAM lparam)
{
    HWND  hwndListbox;

    // Center the dialog over the application window
    CenterWindow(hdlg, GetWindow(hdlg, GW_OWNER));

    hwndListbox = GetDlgItem(hdlg, IDD_STUDENTS);

    SendDlgItemMessage(hdlg, IDD_INCLUDEBOYS, BM_SETCHECK, 1, 0L);

    PopulateStudentListbox(hwndListbox, TRUE);

    SendMessage(hwndListbox, LB_SETCURSEL, 0, 0L);

    UpdateControls(hdlg, hwndListbox);

    return FALSE;
}

//
//  FUNCTION: MsgModalCommand(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: Process WM_COMMAND message sent to the modal box.
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
//    with the cmdiModal structure defined in this file to handle
//    the command messages for the modal dialog box.
//

LRESULT MsgModalCommand
    (HWND hwnd, UINT uMessage, WPARAM wparam, LPARAM lparam)
{
    return DispCommand(&cmdiModal, hwnd, wparam, lparam);
}

//
//  FUNCTION: CmdModalIncludeBoys(HWND, WORD, WORD, HWND)
//
//  PURPOSE: Update the students listbox when this checkbox is clicked
//
//  PARAMETERS:
//    hdlg - The window handling the command.
//    wCommand - The command to be handled (unused).
//    wNotify  - Notification flag
//    hwndCtrl - NULL (unused).
//
//  RETURN VALUE:
//    Always returns TRUE.
//
//  COMMENTS:
//    
//

LRESULT CmdModalIncludeBoys(HWND hdlg,
                            WORD wCommand,
                            WORD wNotify,
                            HWND hwndCtrl)
{
    BOOL bChecked;

    bChecked = (BOOL)SendMessage(hwndCtrl, BM_GETCHECK, 0, 0L);
    PopulateStudentListbox(GetDlgItem(hdlg, IDD_STUDENTS), bChecked);
    SendDlgItemMessage(hdlg, IDD_STUDENTS, LB_SETCURSEL, 0, 0L);
    UpdateControls(hdlg, GetDlgItem(hdlg, IDD_STUDENTS));
    return TRUE;
}

//
//  FUNCTION: CmdModalStudents(HWND, WORD, WORD, HWND)
//
//  PURPOSE: Update the edit controls for the currently selected name
//
//  PARAMETERS:
//    hdlg - The window handling the command.
//    wCommand - The command to be handled (unused).
//    wNotify  - Notification flag
//    hwndCtrl - Window handle of student listbox
//
//  RETURN VALUE:
//    Always returns TRUE.
//
//  COMMENTS:
//    The itemdata for the listbox will give us the index into the
//    rgStudentInfo array.  This is necessary because not all the
//    Students are shown at all times--therefore, there's not a 1-1
//    mapping between the listbox index and the rgStudentInfo index.
//

LRESULT CmdModalStudents
    (HWND hdlg, WORD wCommand, WORD wNotify, HWND hwndCtrl)
{
    switch(wNotify)
    {
        case LBN_SELCHANGE:
            UpdateControls(hdlg, hwndCtrl);
            break;

        default:
            break;
    }

    return TRUE;
}

//
//  FUNCTION: CmdModalDone(HWND, WORD, HWND)
//
//  PURPOSE: Free the modal box and related data.
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

LRESULT CmdModalDone(HWND hdlg, WORD wCommand, WORD wNotify, HWND hwndCtrl)
{
    EndDialog(hdlg, TRUE);          // Exit the dialog
    return TRUE;
}
