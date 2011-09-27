// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
// PARTICULAR PURPOSE.
//
// Copyright (C) 1993-1995  Microsoft Corporation.  All Rights Reserved.
//
//  MODULE:   subcls.c
//
//  PURPOSE:   Handles the message for the subclassed control(s).
//
//  FUNCTIONS:
//    WndProc        - Processes messages for the main window.
//    MsgCommand     - Handle the WM_COMMAND messages for the main window.
//    MsgDestroy     - Handles the WM_DESTROY message by calling 
//                     PostQuitMessage().
//    CmdExit        - Handles the file exit command by calling destory 
//                     window on the main window.
//    ChangeCaret    - Changes the caret in an edit control
//    SubClsControl  - Subclasses the given control
//    SubCls         - Handles the messages sent to the subclass procedure
//    SubClsEdit     - Dispatches messaeges for the 
//                   - Subclassed edit control 
//    SubclsList     - Dispatches messages for the 
//                   - Subclassed list box control
//    EditMsgChar    - Handles the WM_CHAR message for 
//                   - the subclassed control
//    EditKeydown    - Handles the WM_KEYDOWN message for 
//                   - the subclassed control
//    ListMsgKeydown - Handles the WM_KEYDOWN messages for the
//                   - subclassed list box
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
#include "resource.h"
#include "modal.h"
#include "subcls.h"
#include "listdlg.h"
#include "modal.h"

// Main window message table definition.
MSD rgmsd[] =
{
    {WM_COMMAND, MsgCommand},
    {WM_DESTROY, MsgDestroy}
};

MSDI msdiMain =
{
    sizeof(rgmsd) / sizeof(MSD),
    rgmsd,
    edwpWindow
};


// Main window command table definition.
CMD rgcmd[] =
{
    {IDM_EXIT,  CmdExit},
    {IDM_DEMO,  CmdModal},
    {IDM_ABOUT, CmdAbout}
};

CMDI cmdiMain =
{
    sizeof(rgcmd) / sizeof(CMD),
    rgcmd,
    edwpWindow
};


// Subclass window message table for edit

MSD rgmsdEditControl[] =
{
    {WM_CHAR,    EditMsgChar},
    {WM_KEYDOWN, EditMsgKeyDown}

};

// MSDI for the Subclassed control (edit)

MSDI msdiEditControl =
{
    sizeof(rgmsdEditControl) / sizeof(MSD),
    rgmsdEditControl,
    edwpChildWindow
};


// Subclass window message table for list box
MSD rgmsdListControl[] =
{
    {WM_KEYDOWN, ListMsgKeydown}
};

// MSDI for the Subclassed control (list)
MSDI msdiListControl =
{
    sizeof(rgmsdListControl) / sizeof(MSD),
    rgmsdListControl,
    edwpChildWindow
};


//Global variables for Module Subcls. 

// Subclass windowprocedures, new and old
WNDPROC   lpNewProcEdit, lpOldProcEdit,
          lpNewProcList, lpOldProcList ;
extern    bEditMode ; // Edit mode for the subclassed edit control


// Array of Structures for calling default control procedures. 

DCWP    defChildProcs[2] ;


//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
//  PARAMETERS:
//    hwnd     - window handle
//    uMessage - message number
//    wparam   - additional information (dependant on message number)
//    lparam   - additional information (dependant on message number)
//
//  RETURN VALUE:
//    The return value depends on the message number.  If the message
//    is implemented in the message dispatch table, the return value is
//    the value returned by the message handling function.  Otherwise,
//    the return value is the value returned by the default window procedure.
//
//  COMMENTS:
//    Call the DispMessage() function with the main window's message dispatch
//    information (msdiMain) and the message specific information.
//

LRESULT CALLBACK WndProc(HWND   hwnd, 
                         UINT   uMessage, 
                         WPARAM wparam, 
                         LPARAM lparam)
{
    return DispMessage(&msdiMain, hwnd, uMessage, wparam, lparam);
}


//
//  FUNCTION: MsgCommand(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: Handle the WM_COMMAND messages for the main window.
//
//  PARAMETERS:
//    hwnd     - window handle
//    uMessage - WM_COMMAND (Unused)
//    GET_WM_COMMAND_ID(wparam,lparam)   - Command identifier
//    GET_WM_COMMAND_HWND(wparam,lparam) - Control handle
//
//  RETURN VALUE:
//    The return value depends on the message number.  If the message
//    is implemented in the message dispatch table, the return value is
//    the value returned by the message handling function.  Otherwise,
//    the return value is the value returned by the default window procedure.
//
//  COMMENTS:
//    Call the DispCommand() function with the main window's command dispatch
//    information (cmdiMain) and the command specific information.
//

LRESULT MsgCommand(HWND hwnd, UINT uMessage, WPARAM wparam, LPARAM lparam)
{
    return DispCommand(&cmdiMain, hwnd, wparam, lparam);
}


//
//  FUNCTION: SubClsEdit(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
//  PARAMETERS:
//    hwnd     - window handle
//    uMessage - message number
//    wparam   - additional information (dependant on message number)
//    lparam   - additional information (dependant on message number)
//
//  RETURN VALUE:
//    The return value depends on the message number.  If the message
//    is implemented in the message dispatch table, the return value is
//    the value returned by the message handling function.  Otherwise,
//    the return value is the value returned by the default window procedure.
//
//  COMMENTS:
//    Call the DispMessage() function with the control's message dispatch
//    information (msdiMain) and the message specific information.
//


//
//  FUNCTION: MsgDestroy(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: Calls PostQuitMessage().
//
//  PARAMETERS:
//
//    hwnd      - Window handle  (Unused)
//    uMessage  - Message number (Unused)
//    wparam    - Extra data     (Unused)
//    lparam    - Extra data     (Unused)
//
//  RETURN VALUE:
//
//    Always returns 0 - Message handled
//
//  COMMENTS:
//
//

LRESULT MsgDestroy(HWND hwnd, UINT uMessage, WPARAM wparam, LPARAM lparam)
{
    PostQuitMessage(0);
    return 0;
}


//
//  FUNCTION: CmdModal(HWND, WORD, WORD, HWND)
//
//  PURPOSE: Displays the "Modal" dialog box
//
//  PARAMETERS:
//    hwnd      - Window handle
//    wCommand  - IDM_ABOUT (unused)
//    wNotify   - Notification number (unused)
//    hwndCtrl  - NULL (unused)
//
//  RETURN VALUE:
//
//    Always returns 0 - Message handled
//
//  COMMENTS:
//    To process the IDM_DEMO message, call MakeProcInstance() to get the
//    current instance address of the Modal() function.  Then call Dialog
//    box which will create the box according to the information in your
//    sample.rc file and turn control over to the Modal() function.  When
//    it returns, free the intance address.
//
//

LRESULT CmdModal(HWND hwnd, WORD wCommand, WORD wNotify, HWND hwndCtrl)
{
   DialogBox(hInst, MAKEINTRESOURCE(IDD_SUBCLSDLG), hwnd, (DLGPROC)Modal);
   return 0;
}



//
//  FUNCTION: CmdExit(HWND, WORD, WORD, HWND)
//
//  PURPOSE: Exit the application.
//
//  PARAMETERS:
//    hwnd     - The window.
//    wCommand - IDM_EXIT (unused)
//    wNotify  - Notification number (unused)
//    hwndCtrl - NULL (unused)
//
//  RETURN VALUE:
//    Always returns 0 - command handled.
//
//  COMMENTS:
//
//

LRESULT CmdExit(HWND hwnd, WORD wCommand, WORD wNotify, HWND hwndCtrl)
{
    DestroyWindow(hwnd);
    return 0;
}



LRESULT CALLBACK SubClsEdit (HWND   hwnd, 
                             UINT   uMessage, 
                             WPARAM wparam, 
                             LPARAM lparam)
{
    return DispMessage(&msdiEditControl, hwnd, uMessage, wparam, lparam);
}


//
//  FUNCTION: SubClsList(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
//  PARAMETERS:
//    hwnd     - window handle
//    uMessage - message number
//    wparam   - additional information (dependant on message number)
//    lparam   - additional information (dependant on message number)
//
//  RETURN VALUE:
//    The return value depends on the message number.  If the message
//    is implemented in the message dispatch table, the return value is
//    the value returned by the message handling function.  Otherwise,
//    the return value is the value returned by the default window procedure.
//
//  COMMENTS:
//    Call the DispMessage() function with the control's message dispatch
//    information (msdi) and the message specific information.
//

LRESULT CALLBACK SubClsList (HWND   hwnd, 
                             UINT   uMessage, 
                             WPARAM wparam, 
                             LPARAM lparam)
{
   return DispMessage(&msdiListControl, hwnd, uMessage, wparam, lparam);
}


/* Edit control subclass messages are processed here..*/


//
//  FUNCTION: EditMsgChar(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes Char messages for the Edit control window.
//
//  PARAMETERS:
//    hwnd     - window handle
//    uMessage - message number
//    wparam   - additional information (dependant on message number)
//    lparam   - additional information (dependant on message number)
//
//  RETURN VALUE:
//    The return value depends on the message number.  If the message
//    is implemented in the message dispatch table, the return value is
//    the value returned by the message handling function.  Otherwise,
//    the return value is the value returned by the default window procedure.
//
//  COMMENTS:
//    Process the WM_CHAR message to provide Addtional features for the
//    edit control. Pass the unprocessed messages onto the original
//    window procedure
//


LRESULT EditMsgChar(HWND hwnd, 
                    UINT uMessage, 
                    WPARAM wparam, 
                    LPARAM lparam)
{
   int  lPosition ;


   if (bEditMode == OVERRIDE)
      {
      lPosition = SendMessage (hwnd, EM_GETSEL, 0, 0L);
      SendMessage (hwnd, 
                   EM_SETSEL,  
                  (WPARAM)(INT)lPosition, 
                  (LPARAM)(INT)lPosition+1);
      SendMessage (hwnd, EM_REPLACESEL, 0, (LONG) (LPSTR) "");
     }

   // messages that are nor processed, pass them on..
   return CallWindowProc(lpOldProcEdit, 
                         hwnd, 
                         uMessage, 
                         wparam, 
                         lparam) ;
}


//
//  FUNCTION: EditMsgKeyDown(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes Keydown messages for the Edit control window.
//
//  PARAMETERS:
//    hwnd     - window handle
//    uMessage - message number
//    wparam   - additional information (dependant on message number)
//    lparam   - additional information (dependant on message number)
//
//  RETURN VALUE:
//    The return value depends on the message number.  If the message
//    is implemented in the message dispatch table, the return value is
//    the value returned by the message handling function.  Otherwise,
//    the return value is the value returned by the default window procedure.
//
//  COMMENTS:
//    Process the WM_KEYDOWN message to provide Addtional features for the
//    edit control.
//


LRESULT EditMsgKeyDown(HWND hwnd, 
                       UINT uMessage, 
                       WPARAM wparam, 
                       LPARAM lparam)
{

   if(wparam == VK_INSERT)
   {
      if(bEditMode == INSERT)
      {
         bEditMode = OVERRIDE;
         ChangeCaret(hwnd, OVERRIDE);
         EnableWindow(GetDlgItem(ghDlgList, IDM_OVERRIDE), FALSE);
         EnableWindow(GetDlgItem(ghDlgList, IDM_INSERT), TRUE);
     }
     else
     {
         bEditMode = INSERT ;
         ChangeCaret(hwnd, INSERT) ;
         EnableWindow(GetDlgItem(ghDlgList, IDM_OVERRIDE), TRUE);
         EnableWindow(GetDlgItem(ghDlgList, IDM_INSERT), FALSE);
     }
     return 0 ;
   }
   else
      return CallWindowProc(lpOldProcEdit, 
                            hwnd, 
                            uMessage, 
                            wparam, 
                            lparam);

}

/* List box subclass messages are processed here..*/

//
//  FUNCTION: ListMsgKeydown(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the Edit control window.
//
//  PARAMETERS:
//    hwnd     - window handle
//    uMessage - message number
//    wparam   - additional information (dependant on message number)
//    lparam   - additional information (dependant on message number)
//
//  RETURN VALUE:
//    The return value depends on the message number.  If the message
//    is implemented in the message dispatch table, the return value is
//    the value returned by the message handling function.  Otherwise,
//    the return value is the value returned by the default window procedure.
//
//  COMMENTS:
//    Process the WM_CHAR message to provide Addtional features for the
//    List control. Depending on the key pressed, either the INSERT or
//    the DELETE Dialogs will be displayed
//


LRESULT ListMsgKeydown(HWND hwnd, UINT uMessage, WPARAM wparam, LPARAM lparam)
{
   if((int)wparam == VK_INSERT) 
      DispInsDialog(ghDlgList, gIDControl) ;
    else
       if((int)wparam == VK_DELETE)
       DispDelDialog(ghDlgList, gIDControl) ;

       return CallWindowProc(lpOldProcList,  
                             hwnd, 
                             uMessage,
                             wparam, 
                             lparam) ;


}

//
//  FUNCTION: SubClsControl(HWND, WORD)
//
//  PURPOSE:  Subclasses the given control in a dialog box. 
//
//  PARAMETERS:
//     HWND    - window handle
//     WORD    - control ID. 
//
//  RETURN VALUE:
//    The return value is TRUE if the control was subclassed or else FALSE.
//
//  COMMENTS:
//    Calls the GetWindowLong() function to obtain the address of the 
//    window procedure and then calls SetWindowLong() to set the window
//    procedure to a new window procedure (subclass procedure) 
//    
//


BOOL SubClsControl(HWND hDlg, WORD idControl) 

{
   char    szBufClsName[50] ;
   HWND    hWndControl ;

     hWndControl = GetDlgItem(hDlg, idControl) ;
     GetClassName(hWndControl, (LPSTR)szBufClsName, sizeof(szBufClsName)) ;

     if(!lstrcmp((LPSTR)szBufClsName, (LPSTR)"Edit")) 
        {
        // Edit control needs to be subclassed.
        lpOldProcEdit = (WNDPROC) GetWindowLong(hWndControl, GWL_WNDPROC) ;
        lpNewProcEdit = MakeProcInstance((WNDPROC)SubClsEdit, hInst) ;        
        SetWindowLong(hWndControl, 
                      GWL_WNDPROC, 
                      (LONG)(WNDPROC)lpNewProcEdit) ;
        defChildProcs[0].hWndChild   = hWndControl ;
        defChildProcs[0].origWndProc = lpOldProcEdit ; 
        return TRUE ;
        }

     else
       if(!lstrcmp((LPSTR)szBufClsName, (LPSTR)"ListBox")) 
       {
        // Listbox control needs to be subclassed.
          lpOldProcList = (WNDPROC) GetWindowLong(hWndControl, GWL_WNDPROC) ;
          lpNewProcList = MakeProcInstance((WNDPROC)SubClsList, hInst) ;        
          SetWindowLong(hWndControl, 
                        GWL_WNDPROC, 
                        (LONG)(WNDPROC)lpNewProcList) ;
          defChildProcs[1].hWndChild   = hWndControl ;
          defChildProcs[1].origWndProc = lpOldProcList ; 
          return TRUE ;
       }

       return FALSE ;

 }



//
//  FUNCTION: DefChildWndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes Default messages for the  control windows
//            that are subclassed.
//
//  PARAMETERS:
//    hwnd     - window handle
//    uMessage - message number
//    wparam   - additional information (dependant on message number)
//    lparam   - additional information (dependant on message number)
//
//  RETURN VALUE:
//    The return value depends on the message number.  If the message
//    is implemented in the message dispatch table, the return value is
//    the value returned by the original message handling function.  
//    
//
//  COMMENTS:
//    This function decides which default child window procedure needs to be
//    called to process the default messages that are not handled by the 
//    Subclass procedure for that control.


LRESULT DefChildWindowProc(HWND   hwnd, 
                           UINT   uMessage, 
                           WPARAM wparam, 
                           LPARAM lparam)
{
   int i ;

   for (i = 0; i < IDM_NUMSUBCLS; i++) 
   {
     if(hwnd == defChildProcs[i].hWndChild)
     return CallWindowProc(defChildProcs[i].origWndProc, 
                           hwnd, 
                           uMessage, 
                           wparam, 
                           lparam) ;
    }

   return 0 ;
}



//
//  FUNCTION: ChangeCaret(HWND, BOOL)
//
//  PURPOSE:  Changes the Caret for the edit control. 
//            
//
//  PARAMETERS:
//    hdlg      - Control Handle
//    bEditMode - Mode of edit control
//
//  RETURN VALUE:
//    None
//
//  COMMENTS:
//    This function decides which caret needs to be displayed in the
//    edit window. It calls CreateCaret() and ShowCaret() to display
//    appropriate caret.


void ChangeCaret(HWND hEdit, BOOL bEditMode)
{
   HDC        hDC;
   TEXTMETRIC tm;
   int        cyHeight, cxWidth;

    hDC = GetDC (hEdit);
    GetTextMetrics (hDC, &tm);
    cyHeight = tm.tmHeight;
    cxWidth  = tm.tmAveCharWidth;
    ReleaseDC (hEdit, hDC);
    DestroyCaret ();

    if (bEditMode == INSERT)
       CreateCaret (hEdit, NULL, 2, cyHeight);
    else 
       if (bEditMode == OVERRIDE)
          CreateCaret (hEdit, NULL, cxWidth, cyHeight);

    ShowCaret (hEdit);

}
