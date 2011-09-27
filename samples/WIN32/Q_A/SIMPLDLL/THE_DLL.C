
/******************************************************************************\
*       This is a part of the Microsoft Source Code Samples. 
*       Copyright (C) 1992-1995 Microsoft Corporation.
*       All rights reserved. 
*       This source code is only intended as a supplement to 
*       Microsoft Development Tools and/or WinHelp documentation.
*       See these sources for detailed information regarding the 
*       Microsoft samples programs.
\******************************************************************************/

/******************************************************************************\
*
*  MODULE:      THE_DLL.C
*
*  PURPOSE:     To provide a simple DLL template. This module contains a
*               generic DLL entry point (replacing the LIbMain & WEP of
*               Win16), four exported skeleton functions with varying
*               parameter lists, and one exported "DLLDialogBox" function
*               which loads a dialog template from the attached resource
*               file.
*
*  FUNCTIONS:   DllMain() - DLL entry point
*               DLLFunction1()  - skeleton function with no inputs
*               DLLFunction2()  - skeleton function with int input
*               DLLFunction3()  - skeleton function with HANDLE input
*               DLLFunction4()  - skeleton function with HWND input
*               DLLDialogBox()  - calls DialogBox using hndParent param
*               DLLDlgProc()    - dialog window procedure
*
*  COMMENTS:    Another name may be used for the entry point
*               ("DllMain" is only an example). If no initialization
*               or notification is required in a DLL, the DLL entry point
*               may be omitted. In either case, make sure to modify the
*               "-entry:DllMain" part of the DLL's link line in the
*               makefile; either substitute the new entry point name, or
*               delete the line altogether (if omitting the entry point).
*
\******************************************************************************/

#include <windows.h>
#include "the_dll.h"



/******************************************************************************\
*
*  FUNCTION:    DllMain
*
*  INPUTS:      hDLL       - handle of DLL
*               dwReason   - indicates why DLL called
*               lpReserved - reserved
*
*  RETURNS:     TRUE (always, in this example.)
*
*               Note that the retuRn value is used only when
*               dwReason = DLL_PROCESS_ATTACH.
*
*               Normally the function would return TRUE if DLL initial-
*               ization succeeded, or FALSE it it failed.
*
*  GLOBAL VARS: ghMod - handle of DLL (initialized when PROCESS_ATTACHes)
*
*  COMMENTS:    The function will display a dialog box informing user of
*               each notification message & the name of the attaching/
*               detaching process/thread. For more information see
*               "DllMain" in the Win32 API reference.
*
\******************************************************************************/

BOOL WINAPI DllMain (HINSTANCE hDLL, DWORD dwReason, LPVOID lpReserved)
{
  switch (dwReason)
  {
    case DLL_PROCESS_ATTACH:
    {
      char buf[BUFSIZE+1];

      //
      // DLL is attaching to the address space of the current process.
      //

      ghMod = hDLL;
      GetModuleFileName (NULL, (LPTSTR) buf, BUFSIZE);
      MessageBox ( GetFocus(),
                  (LPCTSTR) buf,
                  (LPCTSTR) "THE_DLL: Process attaching",
                  MB_OK | MB_SYSTEMMODAL);
      break;
    }

    case DLL_THREAD_ATTACH:

      //
      // A new thread is being created in the current process.
      //

      MessageBox ( GetFocus(),
                  (LPCTSTR) "THE_DLL: Thread attaching",
                  (LPCTSTR) "",
                  MB_OK | MB_SYSTEMMODAL);
      break;

    case DLL_THREAD_DETACH:

      //
      // A thread is exiting cleanly.
      //

      MessageBox ( GetFocus(),
                  (LPCTSTR) "THE_DLL: Thread detaching",
                  (LPCTSTR) "",
                  MB_OK | MB_SYSTEMMODAL);
      break;

    case DLL_PROCESS_DETACH:

      //
      // The calling process is detaching the DLL from its address space.
      //
      MessageBox ( GetFocus(),
                  (LPCTSTR) "THE_DLL: Process detaching",
                  (LPCTSTR) "",
                  MB_OK | MB_SYSTEMMODAL );
      break;
  }

return TRUE;
}



/******************************************************************************\
*
*  FUNCTION: DLLFunction1
*
*  RETURNS:  1
*
\******************************************************************************/

INT DLLFunction1 ()
{
  MessageBeep (0);
  return 1;
}



/******************************************************************************\
*
*  FUNCTION: DLLFunction2
*
*  INPUTS:   i - a dummy int param
*
*  RETURNS:  1
*
\******************************************************************************/

INT DLLFunction2 (int i)
{
  MessageBeep (0);
  return 1;
}



/******************************************************************************\
*
*  FUNCTION: DLLFunction3
*
*  INPUTS:   h - a dummy HANDLE param
*
*  RETURNS:  1
*
\******************************************************************************/

INT DLLFunction3 (HANDLE h)
{
  MessageBeep (0);
  return 1;
}



/******************************************************************************\
*
*  FUNCTION: DLLFunction4
*
*  INPUTS:   i - a dummy HWND param
*
*  RETURNS:  1
*
\******************************************************************************/

INT DLLFunction4 (HWND hwnd)
{
  MessageBeep (0);
  return 1;
}



/******************************************************************************\
*
*  FUNCTION: DLLDialogBox
*
*  INPUTS:   hwndPArent - parent of the dialog box to display
*
*  RETURNS:  1
*
\******************************************************************************/

INT DLLDialogBox (HWND hwndParent)
{
  DialogBox (ghMod, (LPCTSTR) "DLLDlg", hwndParent, (DLGPROC) DLLDlgProc);
  return 1;
}



/******************************************************************************\
*
*  FUNCTION: DLLDlgProc (standard dialog procedure INPUTS/RETURNS)
*
\******************************************************************************/

BOOL CALLBACK DLLDlgProc (HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam)
{
  switch (msg)
  {
    case WM_COMMAND:

      EndDialog(hDlg, TRUE);
      return (TRUE);
  }
  return (FALSE);
}
