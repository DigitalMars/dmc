/**************************************************************************
 *
 *  THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF ANY
 *  KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
 *  IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A PARTICULAR
 *  PURPOSE.
 *
 *  Copyright (c) 1993 - 1995  Microsoft Corporation.  All Rights Reserved.
 *
 **************************************************************************/

/*
 * about.c - Show the "About" box.
 */

#include <windows.h>
#include "about.h"


/* About - Shows the "About MIDI Monitor" dialog.
 *
 * Params:  hWnd - The application's main window handle.
 *          hInstance - The application's instance handle.
 *
 * Returns: void
 */

void About(
  HANDLE  hInstance,
  HWND	  hWnd)
{
    DialogBox(hInstance, "About", hWnd, AboutDlgProc);
}


/* AboutDlgProc - The dialog procedure for the "About MIDI Monitor" dialog.
 *
 * Params:  hDlg - Specifies the associated dialog box.
 *          msg - Specifies the message from the dialog box.
 *	    wParam - 32 bits of message-dependent data.
 *          lParam - 32 bits of message-dependent data.
 *
 * Returns: Non-zero if the message is processed, zero otherwise.
 */

int FAR PASCAL AboutDlgProc(
  HWND	  hDlg,
  UINT	  msg,
  WPARAM  wParam,
  LPARAM  lParam)
{
    switch (msg) {
      case WM_INITDIALOG:
        break;

      case WM_COMMAND:
        EndDialog(hDlg, TRUE);
        break;

      default:
        return FALSE;
        break;
    }

    return (TRUE);
}
