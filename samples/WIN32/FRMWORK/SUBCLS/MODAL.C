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
//    CmdModal           - Displays the "Modal" dialog box
//    CenterWindow       - Center one window over another.
//    Modal              - Processes messages for "Modal" dialog box.
//    MsgModalInit       - To initialize the modal box with version 
//                         info from resources.
//    MsgModalCommand    - Process WM_COMMAND message sent to the modal box.
//    MsgModalInit       - Process the WM_INITDIALOG message for the dialog.
//    CmdModalDone       - Free the modal box and related data.
//    CmdModalOverRide   - Set the edit mode to Override. 
//    CmdModalInsert     - Set the edit mode to Insert. 
//    CmdModalDeleteItem - Bring up the delete item dialog
//    CmdModalInsertItem - Bring up the insert item dialog
//    CmdModalEdit       - Looks for the EN_SETFOCUS and 
//                         changes caret accordingly
//
//  COMMENTS:
//
//

#include <windows.h>            // required for all Windows applications
#include <windowsx.h>
#ifdef WIN16
#include "win16ext.h"           // required only for win16 applications
#endif
#include "globals.h"            
#include "resource.h"
#include "modal.h"
#include "subcls.h"
#include "listdlg.h"

// prototypes specific to this application
LRESULT CALLBACK Modal(HWND, UINT, WPARAM, LPARAM);
LRESULT MsgModalInit(HWND, UINT, WPARAM, LPARAM);
LRESULT MsgModalCommand(HWND, UINT, WPARAM, LPARAM);
LRESULT CmdModalDone(HWND, WORD, WORD, HWND);
LRESULT CmdModalInsert(HWND, WORD, WORD, HWND);
LRESULT CmdModalOverRide(HWND, WORD, WORD, HWND);
LRESULT CmdModalDeleteItem(HWND, WORD, WORD, HWND);
LRESULT CmdModalInsertItem(HWND, WORD, WORD, HWND);
LRESULT CmdModalEdit(HWND, WORD, WORD, HWND);



// Global variable for mode...

BOOL bEditMode  ;


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
    {IDOK,         CmdModalDone},
    {IDCANCEL,     CmdModalDone},
    {IDM_EDIT,     CmdModalEdit},
    {IDM_INSERT,   CmdModalInsert},
    {IDM_OVERRIDE, CmdModalOverRide},
    {IDM_INSITEM,  CmdModalInsertItem},
    {IDM_DELETE,   CmdModalDeleteItem},
 
};

CMDI cmdiModal =
{
    sizeof(rgcmdModal) / sizeof(CMD),
    rgcmdModal,
    edwpNone
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
//     Display the modal dialog box and dispatch messages to it. 
//     application resource.
//
//     Wait for user to click on "Ok" button, then close the dialog box.
//

LRESULT CALLBACK Modal(HWND hdlg, UINT uMessage, WPARAM wparam, LPARAM lparam)
{
   return DispMessage(&msdiModal, hdlg, uMessage, wparam, lparam);
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
  
   // Center the dialog over the application window
   CenterWindow(hdlg, GetWindow(hdlg, GW_OWNER));

   // Subclass the Edit control and The Listbox
   // and set the focus to the edit control
   SubClsControl(hdlg, IDM_EDIT) ;
   SubClsControl(hdlg, IDM_LIST) ;
   bEditMode = INSERT ;
   ghDlgList = hdlg ;
   gIDControl = IDM_LIST ;
   EnableWindow(GetDlgItem(hdlg, IDM_INSERT), FALSE) ;
   SetFocus(GetDlgItem(hdlg, IDM_EDIT)) ;
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





//
//  FUNCTION: CmdModalInsert(HWND, WORD, HWND)
//
//  PURPOSE:  Places the edit control in Insert Mode.
//
//  PARAMETERS:
//    hwnd - The window handling the command.
//    wCommand - The command to be handled (unused).
//    hwndCtrl - NULL (unused).
//
//  RETURN VALUE:
//    Return True if the edit control is placed in an insert mode. 
//
//  COMMENTS:
//    Sets the global variable bEditMode to INSERT. So that the next
//    time the user enters text, the edit control is in its insert mode. 
//

LRESULT CmdModalInsert(HWND hdlg, 
                       WORD wCommand, 
                       WORD wNotify, 
                       HWND hwndCtrl)
{
 
   bEditMode =  INSERT;
   EnableWindow (GetDlgItem(hdlg, IDM_OVERRIDE), TRUE);
   EnableWindow (GetDlgItem(hdlg, IDM_INSERT), FALSE);
   SetFocus (GetDlgItem(hdlg, IDM_EDIT));
   return 0 ;
 
}


//
//  FUNCTION: CmdModalOverRide(HWND, WORD, HWND)
//
//  PURPOSE:  Places edit in the Override text Mode.
//
//  PARAMETERS:
//    hwnd - The window handling the command.
//    wCommand - The command to be handled (unused).
//    hwndCtrl - NULL (unused).
//
//  RETURN VALUE:
//    Return True if successfully places the edit in an Override mode
//
//  COMMENTS:
//    Sets the global variable bEditMode to OVERRIDE. So that the next
//    time the user enters text, the edit control is in its insert mode. 
//

LRESULT CmdModalOverRide(HWND hdlg, 
                         WORD wCommand, 
                         WORD wNotify, 
                         HWND hwndCtrl)
{

   bEditMode = OVERRIDE;
   EnableWindow (GetDlgItem(hdlg, IDM_OVERRIDE), FALSE);
   EnableWindow (GetDlgItem(hdlg, IDM_INSERT), TRUE);
   SetFocus (GetDlgItem(hdlg, IDM_EDIT));
   return 0 ; 

}

//
//  FUNCTION: CmdModalEdit(HWND, WORD, HWND)
//
//  PURPOSE:  Desides which caret to display on the edit control.
//
//  PARAMETERS:
//    hwnd - The window handling the command.
//    wCommand - The command to be handled (unused).
//    hwndCtrl - NULL (unused).
//
//  RETURN VALUE:
//    Returns TRUE if caret change message was posted.
//
//  COMMENTS:
//    Depending on the edit mode (INSERT OR DELETE), this function calls
//    ChangeCaret() to change the caret. 
//

LRESULT CmdModalEdit(HWND hdlg, 
                     WORD wCommand, 
                     WORD wNotify, 
                     HWND hwndCtrl)
{
 
   if (wNotify == EN_SETFOCUS)
   {
      HideCaret (hwndCtrl);
      ChangeCaret(hwndCtrl, bEditMode);
      return TRUE ;
   }
   else // Did not process the message, pass it on..
      return FALSE ;
 
}


//
//  FUNCTION: CmdModalInsertItem(HWND, WORD, HWND)
//
//  PURPOSE:  Inserts items to the list box.
//
//  PARAMETERS:
//    hwnd - The window handling the command.
//    wCommand - The command to be handled (unused).
//    hwndCtrl - NULL (unused).
//
//  RETURN VALUE:
//    Returns True if an item was added to the list box
//
//  COMMENTS:
//    Calls DispInsItem() function to Insert an item
//

LRESULT CmdModalInsertItem(HWND hdlg, 
                           WORD wCommand, 
                           WORD wNotify, 
                           HWND hwndCtrl)
{
   DispInsDialog(hdlg, IDM_LIST) ;
   return 0 ; 

}


//
//  FUNCTION: CmdModalDeleteItem(HWND, WORD, HWND)
//
//  PURPOSE:  Deletes items to the list box.
//
//  PARAMETERS:
//    hwnd - The window handling the command.
//    wCommand - The command to be handled (unused).
//    hwndCtrl - NULL (unused).
//
//  RETURN VALUE:
//    Returns True if an item was deleted successfully from the list box
//
//  COMMENTS:
//    Calls DispDelDialog() function to delete an item
//

LRESULT CmdModalDeleteItem(HWND hdlg, 
                           WORD wCommand, 
                           WORD wNotify, 
                           HWND hwndCtrl)

{
   DispDelDialog(hdlg, IDM_LIST) ;
   return 0 ; 

}
