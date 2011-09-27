// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
// PARTICULAR PURPOSE.
//
// Copyright (C) 1993-1995  Microsoft Corporation.  All Rights Reserved.
//
//  MODULE:    DIALOG.C
//
//  PURPOSE:   Displays the "Insert/Delete" dialog box.
//
//  FUNCTIONS:
//    DispInsDialog()      - Displays the Insert Item Dialog box
//    DispDelDialog()      - Displays the Delete Item Dialog box
//    Insert               - This Function processes messages for 
//                           the Insert Dialog
//    Delete               - This Function processes messages for   
//                           the Delete Dialog
//    MsgListInsertCommand - Function to dispatch command messages 
//                         - for the insert dialog
//    MsgListDeleteCommand - Function to dispatch command messages 
//                         - for the Delete dialog
//    CmdListInsertDone    - Terminate the insert dialog
//    CmdListDeleteDone    - Terminate the Delete dialog
//    CmdListInsertOk      - Validates the entered string for insertion
//    CmdListDeleteOk      - Validates the entered string for deletion
//    CmdListInsertClear   - Clear the list box contents 
//    CmdListDletetClear   - Clear the list box contents 
//
//  COMMENTS:
//


#include <windows.h>            // required for all Windows applications
#include <windowsx.h>
#ifdef WIN16
#include "win16ext.h"           // required only for win16 applications
#endif
#include "globals.h"            // prototypes specific to this application
#include "modal.h"
#include "subcls.h"
#include "listdlg.h"

// Global variavble for list box

HWND ghDlgList ;     // Main dialog window handle.
WORD gIDControl;     // List Box Control ID

// Insert Item Dialog message table definition.
MSD rgmsdListInsert[] =
{
    {WM_COMMAND,  MsgListInsertCommand}
};

MSDI msdiListInsert =
{
    sizeof(rgmsdListInsert) / sizeof(MSD),
    rgmsdListInsert,
    edwpNone
};

// Insert item  dialog command table definition.
CMD rgcmdListInsert[] =
{
 {IDOK,         CmdListInsertOk},
 {IDD_INSCLEAR, CmdListInsertClear},
 {IDCANCEL,     CmdListInsertDone}
 
};

CMDI cmdiListInsert =
{
    sizeof(rgcmdListInsert) / sizeof(CMD),
    rgcmdListInsert,
    edwpNone
};

// Delete Item  Dialog message table definition.
MSD rgmsdListDelete[] =
{
    {WM_COMMAND,    MsgListDeleteCommand}
};

MSDI msdiListDelete =
{
    sizeof(rgmsdListDelete) / sizeof(MSD),
    rgmsdListDelete,
    edwpNone
};

// Modal dialog command table definition.
CMD rgcmdListDelete[] =
{
    {IDCANCEL,     CmdListDeleteDone},
    {IDD_DELCLEAR, CmdListInsertClear},
    {IDOK,         CmdListDeleteOk}

 
};

CMDI cmdiListDelete =
{
    sizeof(rgcmdListDelete) / sizeof(CMD),
    rgcmdListDelete,
    edwpNone
};



//
//  FUNCTION: DispInsDialog(HWND, WORD)
//
//  PURPOSE:  Brings up the Insert Item Dialog
//
//  PARAMETERS:
//    hwnd     - window handle of insert item modal dialog
//    WORD     - ID Of the list box
//
//  RETURN VALUE:
//    None
//
//  COMMENTS:
//    This function displays the Insert Item Dialog box, by calling
//    DialogBox().
//

void DispInsDialog(HWND hwnd, WORD idControl)
 {
    DLGPROC lpProcInsert = MakeProcInstance((FARPROC)Insert, hInst);
    DialogBox(hInst, MAKEINTRESOURCE(IDD_INSDIALOG), hwnd, lpProcInsert);
    FreeProcInstance(lpProcInsert) ;
    SetFocus(GetDlgItem(hwnd, gIDControl)) ;

 }



//
//  FUNCTION: DispDelDialog(HWND, WORD)
//
//  PURPOSE:  Brings up the Delete Item Dialog
//
//  PARAMETERS:
//    hwnd     - window handle of delte item modal dialog
//    WORD     - ID Of the list box
//
//  RETURN VALUE:
//    None
//
//  COMMENTS:
//    This function displays the Delete Item Dialog box by calling
///   the DialogBox() function.
//
//
void DispDelDialog(HWND hwnd, WORD idControl)
 {
   
    DLGPROC lpProcDelete = MakeProcInstance((FARPROC)Delete, hInst);
    DialogBox(hInst, MAKEINTRESOURCE(IDD_DELDIALOG), hwnd, lpProcDelete);
    FreeProcInstance(lpProcDelete);
    SetFocus(GetDlgItem(hwnd, gIDControl)) ;

 }



//
//  FUNCTION: Insert(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for "Insert" dialog box.
//
//  PARAMETERS:
//    hdlg     - window handle of the dialog box
//    wMessage - type of message
//    wparam   - message-specific information
//    lparam   - message-specific information
//
//  RETURN VALUE:
//    TRUE - message handled
//    FALSE - message not handled
//
//  COMMENTS:
//    Calls DispMessage() to dispatch the messages to the correct
//    message handler. This dialog procedure inserts items into the
//    list box
//

LRESULT CALLBACK Insert(HWND hdlg, UINT uMessage, WPARAM wparam, LPARAM lparam)
{
    return DispMessage(&msdiListInsert, hdlg, uMessage, wparam, lparam);
}

//
//  FUNCTION: Delete(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for "Delete" dialog box.
//
//  PARAMETERS:
//    hdlg     - window handle of the dialog box
//    wMessage - type of message
//    wparam   - message-specific information
//    lparam   - message-specific information
//
//  RETURN VALUE:
//    TRUE - message handled
//    FALSE - message not handled
//
//  COMMENTS:
//    Calls DispMessage() to dispatch the messages to the correct
//    message handler
//

LRESULT CALLBACK Delete(HWND hdlg, UINT uMessage, WPARAM wparam, LPARAM lparam)
{
    return DispMessage(&msdiListDelete, hdlg, uMessage, wparam, lparam);
}


//
//  FUNCTION: MsgListInsertCommand(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: Process WM_COMMAND message sent to the Insert Dialog box.
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
//    with the cmdiListInsert structure defined in this file to handle
//    the command messages for the insert  modal dialog box.
//

LRESULT MsgListInsertCommand
    (HWND hwnd, UINT uMessage, WPARAM wparam, LPARAM lparam)
{
    return DispCommand(&cmdiListInsert, hwnd, wparam, lparam);
}


//
//  FUNCTION: MsgListDeleteCommand(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: Process WM_COMMAND message sent to the Insert Dialog box.
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
//    with the cmdiListDelete structure defined in this file to handle
//    the command messages for the delete modal dialog box.
//

LRESULT MsgListDeleteCommand
    (HWND hwnd, UINT uMessage, WPARAM wparam, LPARAM lparam)
{
    return DispCommand(&cmdiListDelete, hwnd, wparam, lparam);
}




//
//  FUNCTION: CmdListInsertDone(HWND, WORD, WORD, HWND)
//
//  PURPOSE: Free the insert modal box and related data.
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
//    Calls EndDialog to terminate the insert dialog session.
//

LRESULT CmdListInsertDone(HWND hdlg, 
                          WORD wCommand, 
                          WORD wNotify, 
                          HWND hwndCtrl)
{
    EndDialog(hdlg, TRUE);          // Exit the dialog
    return TRUE;
}

//
//  FUNCTION: CmdListInsertOk(HWND, WORD, WORD, HWND)
//
//  PURPOSE: Check and Insert the typed string into the list box.
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
//    Determines whether the typed string in the edit control can be 
//    added to the list box or not.
//

LRESULT CmdListInsertOk(HWND hdlg, 
                        WORD wCommand, 
                        WORD wNotify,  
                        HWND hwndCtrl)
{
    char szBuffer[100];
    UINT retValue;

// retrieve the text from the edit control
    retValue = GetDlgItemText(hdlg, 
                              IDD_INSEDIT, 
                              (LPSTR)szBuffer, 
                              sizeof(szBuffer)) ;


    if(retValue < 1)  
    {
       MessageBox(NULL,
                  "NOPE, CANNOT INSERT A NULL STRING", 
                  "ERROR", 
                  MB_OK |MB_ICONEXCLAMATION) ;
       EndDialog(hdlg, TRUE);          // Exit the dialog
    }
    else 
    {
    // add the string into the list box, it is a valid string.
       SendDlgItemMessage(ghDlgList,
                          gIDControl, 
                          LB_ADDSTRING,  
                          0, 
                          (LPARAM)(LPSTR)szBuffer) ;
       EndDialog(hdlg, TRUE);          // Exit the dialog
    }

    return TRUE;
}


//
//  FUNCTION: CmdListInsertClear(HWND, WORD, WORD, HWND)
//
//  PURPOSE: Clear the list box contents. 
//
//  PARAMETERS:
//    hwnd - The window handling the command.
//    wCommand - The command to be handled (unused).
//    hwndCtrl - NULL (unused).
//
//  RETURN VALUE:
//    Always returns FALSE ;
//
//  COMMENTS:
//    Clears the items in the list box, by sending it a 
//    LB_RESETCONTENT message. 
//
//

LRESULT CmdListInsertClear(HWND hdlg, 
                           WORD wCommand, 
                           WORD wNotify, 
                           HWND hwndCtrl)
{
   SendDlgItemMessage(ghDlgList, 
                      gIDControl, 
                      LB_RESETCONTENT, 
                      0,
                      0) ;
   EndDialog(hdlg, TRUE) ;
   return 0 ;
}


//
//  FUNCTION: CmdListDeleteDone(HWND, WORD, HWND)
//
//  PURPOSE: Free the delete modal box and related data.
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
//    Calls EndDialog to terminate the  delete dialog session.
//

LRESULT CmdListDeleteDone(HWND hdlg, WORD wCommand, WORD wNotify, HWND hwndCtrl)
{
    EndDialog(hdlg, TRUE);          // Exit the dialog
    return TRUE;
}


//
//  FUNCTION: CmdListDeleteOk(HWND, WORD, WORD, HWND)
//
//  PURPOSE: Delete the typed string from the list box if valid
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
//    Determines whether the typed string in the edit control can be 
//    deleted from  the list box or not.by sending it a LB_DELETESTRING
//    message.
//

LRESULT CmdListDeleteOk(HWND hdlg, 
                        WORD wCommand, 
                        WORD wNotify, 
                        HWND hwndCtrl)
{
    char szBuffer[100];
    int  indexValue ;
    UINT retValue ;

    retValue = GetDlgItemText(hdlg, 
                              IDD_DELEDIT, 
                             (LPSTR)szBuffer, 
                              sizeof(szBuffer)) ;

    indexValue = SendDlgItemMessage(ghDlgList,
                                    gIDControl, 
                                    LB_FINDSTRING, 
                                    (WPARAM)-1,  
                                    (LPARAM)(LPSTR)szBuffer) ;
    if(indexValue == LB_ERR) 
    {
       MessageBox(NULL, 
                 "NOPE, COULD NOT MATCH STRING", 
                 "ERROR", 
                  MB_OK|MB_ICONHAND) ;
       EndDialog(hdlg, TRUE);
       return 0 ;
    }
    else                 // valid index, delete the string string...
       SendDlgItemMessage(ghDlgList, 
                          gIDControl, 
                          LB_DELETESTRING, 
                          indexValue,  
                          (LPARAM)0) ;
       EndDialog(hdlg, TRUE);
       return 0 ;
}
