
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
   ddeprocs.c
*/

#include <windows.h>
#include <stdlib.h>
#include <stdio.h>
#include "ddeinst.h"
#include "ddextrn.h"
#include "dialogs.h"

// function prototype for List synchronization
void   SynchronizeLists (HWND);
void   SynchronizeListSelection (HWND);

// function prototype for child window creation 
void   CreateChildren (HWND);

// internal thread counter
int    iThreadCount = 0;

/********************************************************************

   MainWndProc.
   
   Function that processes the messages sent to the main window.

********************************************************************/

LRESULT APIENTRY MainWndProc (HWND hwnd, UINT uiMsg, WPARAM wParam,
      LPARAM lParam) {
   BOOL        fFlag;

   switch (uiMsg) {
      case WM_CREATE: {

// Create all of the child windows that are desired.
         CreateChildren (hwnd);
         break;
      }/*endCase*/
      case WM_DESTROY: {
         PostQuitMessage (0);
         return (0L);
      }/*endCase*/
      case WM_USER_CLOSE_DIALOG: {
         if (hwndDialog) {
            DestroyWindow (hwndDialog);
            hwndDialog = (HWND) NULL;
            return (TRUE);
         }/*endIf*/
         break;
      }/*endCase*/
      case WM_SIZE: {
      /* Need to resize the StatusBar */
         if (hwndStatus) {
            long lHeight;
            int  iWidth;
            int  iHeight;

            iWidth = LOWORD (lParam);
            iHeight = HIWORD (lParam);
            lHeight = GetWindowLong (hwndStatus, GWL_USERDATA);
            MoveWindow (hwndStatus, -1, iHeight - (int) lHeight,
                  iWidth + 2, lHeight + 2, TRUE);
         }/*endIf*/
         break;
      }/*endCase*/
      case WM_USER_GET_APPS: {
         StartTraverseThread (szUserPath);
         return (1L);
      }/*endCase*/
      case WM_USER_GET_GROUPS: {
         StartGroupRetrievalThread ();
         return (1L);
      }/*endCase*/
      case WM_USER_THREAD_DONE: {
         iThreadCount++;
         if (!fBatch) {
            break;
         }/*endIf*/
         if (iThreadCount == 2) {
            PostMessage (ghwndMain, WM_COMMAND,
                  (WPARAM) MAKELONG (ID_ADDALLBUTTON, BN_CLICKED),
                  (LPARAM) hwndAddAll);
            PostMessage (ghwndMain, WM_COMMAND,
                  (WPARAM) MAKELONG (ID_ADDGROUP, BN_CLICKED),
                  (LPARAM) hwndAddGroupButton);
            PostMessage (ghwndMain, WM_COMMAND,
                  (WPARAM) MAKELONG (ID_ADDBUTTON, BN_CLICKED), 
                  (LPARAM) hwndAddButton);
            iThreadCount = 0;
         }/*endIf*/
         break;
      }/*endCase*/
      case WM_COMMAND: {
         switch (HIWORD (wParam)) {
            case LBN_SELCHANGE: {
               SynchronizeListSelection ((HWND) lParam);
               if ((HWND) lParam == hwndCombo) {
                  EnableWindow (hwndAddGroupButton, TRUE);
               }/*endIf*/
               break;
            }/*endCase*/
            case CBN_EDITCHANGE: {
               fFlag = (BOOL) GetWindowTextLength (hwndCombo);
               EnableWindow (hwndAddGroupButton, fFlag);
               break;
            }/*endCase*/
            case BN_CLICKED: {
               switch (LOWORD (wParam)) {
                  case ID_ADDALLBUTTON: {

               // Select all elements in the two lists.
               // Under Win 3.x LPARAM of LB_SETSEL is MAKELPARAM (-1, 0)
               // Under Win32 it is -1
               
                     SendMessage (hwndFileList, LB_SETSEL, (WPARAM) TRUE,
                           (LPARAM) -1);

               // Under Win 3.x LPARAM of LB_SETSEL is MAKELPARAM (-1, 0)
               // Under Win32 it is -1
               
                     SendMessage (hwndFileList2, LB_SETSEL, (WPARAM) TRUE,
                           (LPARAM) -1);
                     break;
                  }/*endCase*/
                  case ID_EXITBUTTON: {
                     PostQuitMessage (0);
                     break;
                  }/*endCase*/
                  case ID_ADDBUTTON: {
                  // Create the Progress Dialog
                     hwndDialog = CreateDialog (ghModule,
                           "ProgressDialog", hwnd, ProgressDlgWndProc);

                  // Start a thread to add the items 
                     StartAddThread ();
                     break;
                  }/*endCase*/
                  case ID_ADDGROUP: {

                  // Add the user defined group.
                     CreateGroup ();
                     break;
                  }/*endCase*/
               }/*endSwitch*/
               break;
            }/*endCase*/
         }/*endSwitch*/

     // Process menu events now.

         switch (LOWORD (wParam)) {
            case DI_EXIT: {
               PostQuitMessage (0);
               return (1L);
            }/*endCase*/
            case DI_ABOUT: {
               DialogBox (ghModule, "AboutBox", hwnd,
                     (DLGPROC) AboutBoxWndProc);
               break;
            }/*endCase*/
         }/*endSwitch*/
         break;
      }/*endCase*/
   }/*endSwitch*/
   return (DefWindowProc (hwnd, uiMsg, wParam, lParam));
}/* end MainWndProc */

/********************************************************************

   StatusBarWndProc.
   
   Function that manages the Status bar at the bottom of the main 
   window.

********************************************************************/

LRESULT APIENTRY StatusBarWndProc (HWND hwnd, UINT uiMsg, WPARAM wParam,
      LPARAM lParam) {
   HDC             hdc;
   long            lHeight;
   RECT            rc;
   PAINTSTRUCT     ps;
   TEXTMETRIC      tm;
   POINT           pts[6];

   switch (uiMsg) {
      case WM_CTLCOLORSTATIC: {
         SetBkMode ((HDC) wParam, TRANSPARENT);
         return ((LRESULT) GetStockObject (WHITE_BRUSH));
      }/*endCase*/
      case WM_CREATE: {
         hdc = GetDC (hwnd); // Get the DC for text metrics

         GetClientRect (GetParent (hwnd), &rc);
         GetTextMetrics (hdc, &tm);

      // Calculate the height of the window
         lHeight = tm.tmHeight + 10 * GetSystemMetrics (SM_CYBORDER) + 2;

      // Save away the calculated height
         SetWindowLong (hwnd, GWL_USERDATA, lHeight);

      //* resize the window
         SetWindowPos (hwnd, NULL, -1, rc.bottom - lHeight,
            rc.right + 2, lHeight + 2, SWP_NOZORDER | SWP_NOMOVE);

         ReleaseDC (hwnd, hdc);
         break;
      }/*endCase*/
      case WM_USER_UPDATE_STATUS: {
         switch (lParam) {
            case ID_DDEML_CONNECT: {
               SetWindowText (hwndStatusText, "DDEML Connection");
               break;
            }/*endCase*/
            case ID_DDEML_RETRIEVING: {
               SetWindowText (hwndStatusText, "DDEML Retrieving");
               break;
            }/*endCase*/
            case ID_DDEML_DISCONNECT: {
               SetWindowText (hwndStatusText, "DDEML Disconnect");
               break;
            }/*endCase*/
            case ID_DDEML_COMPLETE: {
               SetWindowText (hwndStatusText, "DDEML Complete");
               break;
            }/*endCase*/
            case ID_DDEML_ACTIVATE: {
               SetWindowText (hwndStatusText, "DDEML Activate Group");
               break;
            }/*endCase*/
            case ID_DDEML_CREATE: {
               SetWindowText (hwndStatusText, "DDEML Create Group");
               break;
            }/*endCase*/
            case ID_DDEML_ADD: {
               TCHAR   szText[32];

               sprintf (szText, "DDEML Add Item %ld", wParam);
               SetWindowText (hwndStatusText, szText);
               break;
            }/*endCase*/
         }/*endSwitch*/
         break;
      }/*endCase*/
      case WM_PAINT: {
         HBRUSH   hBrush;
         HBRUSH   hBrushTemp;
         HPEN     hPen;
         HPEN     hPenTemp;

         hdc = BeginPaint (hwnd, &ps);
         GetClientRect (hwnd, &rc);

         lHeight = GetWindowLong (hwnd, GWL_USERDATA);
         pts[0].x = 3;
         pts[0].y = pts[5].y = lHeight - 4;
         pts[1].x = 6;
         pts[1].y = pts[2].y = lHeight - 6;
         pts[2].x = pts[3].x = (rc.right / 2) - 4;
         pts[3].y = 6;
         pts[4].y = 3;
         pts[4].x = pts[5].x = rc.right / 2;
         hBrush = GetStockObject (WHITE_BRUSH);
         hPen = GetStockObject (WHITE_PEN);
         hBrushTemp = SelectObject (hdc, hBrush);
         hPenTemp = SelectObject (hdc, hPen);
         SetPolyFillMode (hdc, WINDING);
         Polygon (hdc, pts, 6);
         hBrush = CreateSolidBrush (GetSysColor (COLOR_BTNSHADOW));
         pts[2].x = 6;
         hPen = CreatePen (PS_SOLID, 1, GetSysColor (COLOR_BTNSHADOW));
         pts[2].y = 6;
         pts[5].x = pts[5].y = 3;
         SelectObject (hdc, hBrush);
         SelectObject (hdc, hPen);
         Polygon (hdc, pts, 6);
         rc.left = rc.top = 7;
         rc.right = (rc.right / 2) - 4;
         rc.bottom = lHeight - 7;
         SelectObject (hdc, hBrushTemp);
         SelectObject (hdc, hPenTemp);
         DeleteObject (hBrush);
         DeleteObject (hPen);
         hBrush = GetStockObject (WHITE_BRUSH);
         FillRect (hdc, &rc, hBrush);
         DeleteObject (hBrush);
         EndPaint (hwnd, &ps);
         return (0L);
      }/*endCase*/
   }/*endSwitch*/
   return (DefWindowProc (hwnd, uiMsg, wParam, lParam));
}/* end StatusBarWndProc */

/********************************************************************

   ListBoxWndProc.
   
   Function that manages the list boxes that are on the screen. Subclassed
   to provide additional functionality.

********************************************************************/

LRESULT APIENTRY ListBoxWndProc (HWND hwnd, UINT uiMsg, WPARAM wParam,
      LRESULT lParam) {

   switch (uiMsg) {
      case WM_KEYUP: {
         SynchronizeLists (hwnd);
         break;
      }/*endCase*/
      case WM_VSCROLL: {
         SynchronizeLists (hwnd);
         break;
      }/*endCase*/
      break;
   }/*endSwitch*/
   return (CallWindowProc (DefListBoxWndProc, hwnd, uiMsg, wParam, lParam));
}/* end ListBoxWndProc */

/********************************************************************

   SynchronizeLists.
   
   Function that makes sure that the two lists are synchronized and 
   always have the same selection.

********************************************************************/

void SynchronizeLists (HWND hwnd) {
   HWND   hwndOther;
   long   lSourceTop;
   long   lTargetTop;

// If this is from the combo list then return.
   if (hwnd == hwndCombo) {
      return;
   }/*endIf*/

// Figure out which list caused the event and update the other.
   if (hwnd == hwndFileList) {
      hwndOther = hwndFileList2;
   } else {
      hwndOther = hwndFileList;
   }/*endIf*/

// Get the top index of the source list.
   lSourceTop = SendMessage (hwnd, LB_GETTOPINDEX, 0, 0L);

// Get the top index of the target list.
   lTargetTop = SendMessage (hwndOther, LB_GETTOPINDEX, 0, 0L);

// If the top indexes are different set the top of the target to the
// same value as the source.
   if (lSourceTop != lTargetTop) {
      SendMessage (hwndOther, LB_SETTOPINDEX, lSourceTop, 0L);
   }/*endIf*/
}/* end SynchronizeLists */

/********************************************************************

   SynchronizeListSelection.
   
   Function that actually manages the list selection.

********************************************************************/

void SynchronizeListSelection (HWND hwnd) {
   HWND   hwndOther;
   long   lNumSel;
   int   * lpSelection;
   long   lIndex;

// If this is from the combo list then return.

   if (hwnd == hwndCombo) {
      return;
   }/*endIf*/

// Figure out which list caused the event and update the other.

   if (hwnd == hwndFileList) {
      hwndOther = hwndFileList2;
   } else {
      hwndOther = hwndFileList;
   }/*endIf*/

// Find out how many items are selected in the source list.

   lNumSel = SendMessage (hwnd, LB_GETSELCOUNT, 0, 0L);
   if (lNumSel != LB_ERR && lNumSel != 0) {

   // Allocate a block of memory to hold the selection indexes.
      lpSelection = (int *) GlobalAlloc (GMEM_FIXED | GMEM_ZEROINIT,
         lNumSel * sizeof (int));
      if (lpSelection) {

   // Clear the selection in the list before reselecting.
   // Under Win 3.x LPARAM of LB_SETSEL is MAKELPARAM (-1, 0)
   // Under Win32 it is -1
               
         SendMessage (hwndOther, LB_SETSEL, (WPARAM) FALSE, (LPARAM) -1);

   // Retrieve the selection indexes.

         SendMessage (hwnd, LB_GETSELITEMS, lNumSel, (LPARAM) lpSelection);

   // Loop the indexes selecting the same values in the target list.

         for (lIndex = 0; lIndex < lNumSel; lIndex++) {
            SendMessage (hwndOther, LB_SETSEL, (WPARAM) TRUE,
               MAKELPARAM (lpSelection[lIndex], 0));
         }/*endFor*/

   // Release the memory that was allocated.

         GlobalFree (lpSelection);
      }/*endIf*/
   }/*endIf*/
}/* end SynchronizeListSelection */

/********************************************************************

   CreateChildren.
   
   Function that creates all of the child windows for the app.

********************************************************************/

void CreateChildren (HWND hwnd) {
   int              iListHeight;
   long             lHeight;
   RECT             rc;

// Create the status bar at the bottom of the window.
   hwndStatus = CreateWindow ("StatusBar", NULL,
         WS_BORDER | SS_LEFT | WS_CHILD | WS_VISIBLE,
         0, 0, 0, 0, hwnd, (HMENU) ID_STATUSBAR, ghModule, NULL);

// Retrieve the height of the status bar.
   lHeight = GetWindowLong (hwndStatus, GWL_USERDATA);

// Retrieve the size of the client area of the status bar.
   GetClientRect (hwndStatus, &rc);

   hwndStatusText = CreateWindow ("Static", "Status Bar",
         WS_VISIBLE | WS_CHILD | SS_LEFT,
         7, 7, rc.right / 2 - 10, lHeight - 14, hwndStatus, (HMENU) 0, ghModule,
         NULL);

// Retrieve the size of the client area of the parent window
   GetClientRect (hwnd, &rc);

// Calculate the size of a list box.
   iListHeight = rc.bottom - lHeight * 2;

// Decrease the size of lHeight to compensate for size of status bar.
   lHeight -= 8;

// Create a Static window in the upper left corner.
   CreateWindow ("Static", "Available exe's",
         WS_VISIBLE | WS_CHILD | WS_CLIPSIBLINGS | SS_LEFT,
         0, 0, 200, lHeight - 2, hwnd, (HMENU) 0, ghModule, NULL);

// Create the first list to hold the file name.
   hwndFileList = CreateWindow ("ListBox", NULL, WS_CLIPSIBLINGS | LBS_NOTIFY |
         WS_BORDER | WS_VISIBLE | WS_CHILD | LBS_EXTENDEDSEL | LBS_HASSTRINGS,
         0, lHeight, 150, iListHeight, hwnd, (HMENU) ID_FILELIST, ghModule,
         NULL);

// Subclass the list box window proc so we can do the synchronization
   DefListBoxWndProc = (WNDPROC) GetWindowLong (hwndFileList, GWL_WNDPROC);
   SetWindowLong (hwndFileList, GWL_WNDPROC, (LONG) ListBoxWndProc);

// Create the second list box that holds the name that will be used in the
// Program Manager.
   hwndFileList2 = CreateWindow ("ListBox", NULL,
         WS_BORDER | WS_VISIBLE | WS_CHILD | WS_VSCROLL | LBS_EXTENDEDSEL |
         LBS_NOTIFY | LBS_HASSTRINGS | LBS_SORT | WS_CLIPSIBLINGS, 150,
         lHeight, 150, iListHeight, hwnd, (HMENU) ID_FILELIST2, ghModule,
         NULL);

// Subclass the list.
   SetWindowLong (hwndFileList2, GWL_WNDPROC, (LONG) ListBoxWndProc);

// Create a third list box that is not visible that contains the absolute
// path to the executable.
   hwndPathList = CreateWindow ("ListBox", NULL,
         WS_BORDER | WS_CHILD | WS_VSCROLL | LBS_EXTENDEDSEL |
         LBS_NOTIFY | LBS_HASSTRINGS | LBS_SORT | WS_CLIPSIBLINGS, 150, lHeight,
         150, iListHeight, hwnd, (HMENU) ID_PATHLIST, ghModule, NULL);

// Create a button that is used to add a group.
   hwndAddGroupButton = CreateWindow ("Button", "Add Group",
         WS_VISIBLE | WS_CHILD | WS_CLIPSIBLINGS,
         rc.right - 110, iListHeight - (int) (lHeight * 4),
         100, lHeight, hwnd, (HMENU) ID_ADDGROUP, ghModule, NULL);

// Create the button that is used to add the selected items.
   hwndAddButton = CreateWindow ("Button", "Add Items",
         WS_DISABLED | WS_VISIBLE | WS_CHILD | WS_CLIPSIBLINGS,
         rc.right - 110, iListHeight - (int) (lHeight * 3),
         100, lHeight, hwnd, (HMENU) ID_ADDBUTTON, ghModule, NULL);

// Create the button that is used to select all of the items in the list.
   hwndAddAll = CreateWindow ("Button", "Select All",
         WS_VISIBLE | WS_CHILD | WS_CLIPSIBLINGS,
         rc.right - 110, iListHeight - (int) (lHeight * 2),
         100, lHeight, hwnd, (HMENU) ID_ADDALLBUTTON, ghModule, NULL);

// Create a button that can be used to exit the application.
   hwndExitButton = CreateWindow ("Button", "Exit",
         WS_VISIBLE | WS_CHILD | WS_CLIPSIBLINGS,
         rc.right - 110, iListHeight - (int) lHeight, 100, lHeight, hwnd,
         (HMENU) ID_EXITBUTTON, ghModule, NULL);

// Create a drop down combo box for the group names from the Program Manager.
   hwndCombo = CreateWindow ("ComboBox", NULL,
         WS_VISIBLE | WS_CHILD | WS_CLIPSIBLINGS | CBS_DROPDOWN | WS_VSCROLL |
         CBS_AUTOHSCROLL | CBS_HASSTRINGS, rc.right - 200, lHeight,
         190, lHeight * 5, hwnd, (HMENU) ID_COMBOBOX, ghModule, NULL);

// Create a Static window to label the combo box.
   CreateWindow ("Static", "Available Groups",
         WS_VISIBLE | WS_CHILD | WS_CLIPSIBLINGS | SS_LEFT,
         rc.right - 200, 0, 190, lHeight - 2, hwnd, (HMENU) 0, ghModule, NULL);
}/* end CreateChildren */
