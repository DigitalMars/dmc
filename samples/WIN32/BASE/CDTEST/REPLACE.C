/************************************************************************

  File: replace.c

  Purpose:

    This file contains only one function that set the bDoFindDlg global
    variable to FALSE and then creates a dialog box with the same Callback
    function as the DoFindDialog() function does.

    Since FindText() and ReplaceText() both use the FINDREPLACE
    structure to create their dialogs, one dialog proc and dialog box
    can handle both.

  Functions:

    - DoReplaceDialog()    -- Creates CDTEST's Find/Replace dialog.

************************************************************************/

#include <windows.h>
#include <commdlg.h>
#include "cdtest.h"
#include "replace.h"
#include "find.h"



/************************************************************************

  Function: DoReplaceDialog(HWND)

  Purpose: To create CDTEST's find/replace dialog box.


  Returns: Nothing.

  Comments:

    FindText() and ReplaceText() are similiar enough so that
    the same dialog can be used to edit their creation structure elements,
    so a global variable "bDoFindDlg" keeps track of which one to create
    when the user clicks the OK or Multithread buttons...

************************************************************************/

void DoReplaceDialog(HWND hwnd)
{

  bDoFindDlg = FALSE ;


  /* in order for the keyboard to work, a hook has to be installed before
     the dialog is created.  See find.c for more detail on this. */

  hHook = SetWindowsHookEx(WH_MSGFILTER, lpfnFilterProc, hInst, GetCurrentThreadId()) ;

  if (!hHook) return ;

  DialogBox(hInst, MAKEINTRESOURCE(ID_FINDDIALOG), hwnd, FindProc) ;

  UnhookWindowsHookEx(hHook) ;

}
