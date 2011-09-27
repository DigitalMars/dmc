
/******************************************************************************\
*       This is a part of the Microsoft Source Code Samples. 
*       Copyright (C) 1993 Microsoft Corporation.
*       All rights reserved. 
*       This source code is only intended as a supplement to 
*       Microsoft Development Tools and/or WinHelp documentation.
*       See these sources for detailed information regarding the 
*       Microsoft samples programs.
\******************************************************************************/

/*
   ddedlg.c
*/

#include <windows.h>
#include "ddeinst.h"
#include "ddextrn.h"

/********************************************************************

   AboutBoxWndProc

   Function to process messages to the about box.

********************************************************************/

LRESULT APIENTRY AboutBoxWndProc (HWND hwnd, UINT uiMsg, WPARAM wParam,
      LPARAM lParam) {
   switch (uiMsg) {
      case WM_COMMAND: {
         EndDialog (hwnd, TRUE);
         return (TRUE);
      }/*endCase*/
   }/*endSwitch*/
   return (FALSE);
}/* end AboutBoxWndProc */

/********************************************************************

   ProgressDlgWndProc

   Function to process messages to the about box.

********************************************************************/

LRESULT APIENTRY ProgressDlgWndProc (HWND hwnd, UINT uiMsg, WPARAM wParam,
      LPARAM lParam) {
   switch (uiMsg) {
      case WM_DESTROY: {
         EndDialog (hwnd, TRUE);
         return (TRUE);
      }/*endCase*/
   }/*endSwitch*/
   return (FALSE);
}/* end ProgressDlgWndProc */
