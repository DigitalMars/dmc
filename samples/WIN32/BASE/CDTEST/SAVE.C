/************************************************************************

  File: save.c

  Purpose:

    This file contains only one function that set the bDoOpenDlg global
    variable to FALSE and then creates a dialog box with the same Callback
    function as the DoOpenDialog() function does.

    Since GetOpenFileName() and GetSaveFileName() both use the
    OPENFILENAME structure to create their dialogs, one dialog
    proc and dialog box can handle both.

  Functions:

    - DoSaveDialog()    -- Creates CDTEST's Open/Save dialog.

************************************************************************/


#include <windows.h>
#include <commdlg.h>
#include "cdtest.h"
#include "save.h"
#include "open.h"




/************************************************************************

  Function: DoSaveDialog(HWND)

  Purpose: To create the GetOpenFileName() and GetSaveFileName()
           creation dialog.

  Returns: Nothing.

  Comments:

    GetOpenFileName() and GetSaveFileName() are similiar enough so that
    the same dialog can be used to edit their creation structure elements,
    so a global variable "bDoOpenDlg" keeps track of which one to create
    when the user clicks the OK or Multithread buttons...

************************************************************************/



void DoSaveDialog(HWND hwnd)
{
  bDoOpenDlg = FALSE ;

  DialogBox(hInst, MAKEINTRESOURCE(ID_OPENDIALOG), hwnd, OpenFunc) ;

}
