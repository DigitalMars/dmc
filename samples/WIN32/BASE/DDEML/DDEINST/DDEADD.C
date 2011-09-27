
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

   ddeadd.c
   DDEML Execute functions to add selected items
   and create new group if specified.

*/

#include <windows.h>
#include <stdio.h>
#include <string.h>

#include "ddeinst.h"
#include "ddextrn.h" 
#include "dialogs.h"

CONVCONTEXT   CCFilter = { sizeof (CONVCONTEXT), 0, 0, 0, 0L, 0L };


/********************************************************************

   StartAddThread

   Function to Start the program item addition thread.

********************************************************************/

BOOL StartAddThread () {
   HANDLE hThread;
   LONG   lThreadId;

   hThread = CreateThread (NULL, 0, (LPTHREAD_START_ROUTINE) AddProgramItems,
         "Dummy", CREATE_SUSPENDED | STANDARD_RIGHTS_REQUIRED, &lThreadId);
   if (hThread) {
      SetThreadPriority (hThread, THREAD_PRIORITY_BELOW_NORMAL);
      ResumeThread (hThread);

/* Close the handle since we don't need it anymore */

      CloseHandle (hThread);
      return (TRUE);
   }/*endIf*/
   return (FALSE);
}/* end StartAddThread */

/********************************************************************

   CreateGroup

   Function that creates a group in the Program Manager. If the specified
   name already exists the group is activated.

********************************************************************/

BOOL CreateGroup () {
   HDDEDATA   hData;
   LPSTR      szText;
   LPSTR      szCommand;
   HCONV      hConv;
   HSZ        szProgMan;
   LONG       lResult;

// Allocate a block of memory for the group name that the user has selected.
   szText = VirtualAlloc (NULL, 64, MEM_COMMIT, PAGE_READWRITE);

// Allocate a block of memory for the command to send to the Program Manager.
   szCommand = VirtualAlloc (NULL, 128, MEM_COMMIT, PAGE_READWRITE);
   if (szText) {
   // We got the memory so we can proceed.

   // Initialize the DDE conversation here.

      if (DdeInitialize (&lIdInst, (PFNCALLBACK) GroupDDECallback,
            (DWORD) APPCMD_CLIENTONLY, 0L)) {
         MessageBox (ghwndMain, "DDEML Initialization Failure", "Error", MB_OK);
         VirtualFree (szText, 128, MEM_DECOMMIT);
         return (FALSE);
      }/*endIf*/

   // Create a DDEML string handle for the Program Manager.
      szProgMan = DdeCreateStringHandle (lIdInst, "PROGMAN", CP_WINANSI);
      if (szProgMan) {

      // String handle created so continue.

// Establish a conversation with the Program Manager.
         hConv = DdeConnect (lIdInst, szProgMan, szProgMan, &CCFilter);

         PostMessage (hwndStatus, WM_USER_UPDATE_STATUS, 0,
               ID_DDEML_CONNECT);

// Retrieve the text of the combo control. This will return back what the
// user has typed in if nothing has been selected.
         GetWindowText (hwndCombo, szText, 64);

// Find out if this string is in the list or not. If not create a group
// Otherwise just activate the group.
         lResult = (LONG) SendMessage (hwndCombo, CB_FINDSTRING, (WPARAM) -1,
               (LPARAM) (LPCSTR) szText);
         if (lResult == CB_ERR) {

// Create the group by creating the command line.
// The program manager will activate an existing group if the name matches
// a DDE attempt to create a new group so the create versus show is somewhat
// redundant.
            sprintf (szCommand, "[CreateGroup(%s)]", szText);

// Create a DDEML data handle for the command.
            hData = DdeCreateDataHandle (lIdInst, szCommand,
                  strlen (szCommand) + 1, 0, (HSZ) NULL, CF_TEXT, 0L);

// Send the transaction to the server waiting a maximum of 10 seconds.
// The server will release the data handle.
            if (!DdeClientTransaction ((LPBYTE) hData, 0xFFFFFFFF, hConv,
                  (HSZ) NULL, 0, XTYP_EXECUTE, 10000, &lResult)) {

// If it fails get the error code.
               lResult = DdeGetLastError (lIdInst);
               MessageBox (NULL, "DdeClientTransaction Failed", "Error",
                  MB_OK);
            }/*endIf*/

            PostMessage (hwndStatus, WM_USER_UPDATE_STATUS, 0,
                  ID_DDEML_CREATE);

         } else {

// Group already exists so activate it.
            sprintf (szCommand, "[ShowGroup(%s,1)]", szText);

// Create a DDEML data handle for the command.
            hData = DdeCreateDataHandle (lIdInst, szCommand, strlen (szCommand),
                  0, (HSZ) NULL, CF_TEXT, 0L);

// Send the transaction to the server waiting a maximum of 10 seconds.
// The server will release the data handle.
            if (!DdeClientTransaction ((LPBYTE) hData, 0xFFFFFFFF, hConv,
                  (HSZ) NULL, 0, XTYP_EXECUTE, 10000, &lResult)) {

// If it fails get the error code.
               lResult = DdeGetLastError (lIdInst);
            }/*endIf*/

            PostMessage (hwndStatus, WM_USER_UPDATE_STATUS, 0,
                  ID_DDEML_ACTIVATE);

         }/*endIf*/

// Release the Program Manager string handle.
         DdeFreeStringHandle (lIdInst, szProgMan);

// Disconnect from the server.
         DdeDisconnect (hConv);
         PostMessage (hwndStatus, WM_USER_UPDATE_STATUS, 0,
                  ID_DDEML_COMPLETE);
      } else {
         lResult = DdeGetLastError (lIdInst);
      }/*endIf*/

// Free the two blocks of memory that were allocated.
      VirtualFree (szText, 64, MEM_DECOMMIT);
      VirtualFree (szCommand, 64, MEM_DECOMMIT);
      EnableWindow (hwndAddButton, TRUE);

   // Uninitialize the conversation here so that resources are freed.

      DdeUninitialize (lIdInst);
      lIdInst = 0L;
      return (TRUE);
   } else {
      MessageBox (NULL, "Memory Allocation failure", "Error", MB_OK);
   }/*endIf*/
   EnableWindow (hwndAddButton, FALSE);
   return (FALSE);
}/* end CreateGroup */


// Local function
void UpdateProgressBar (int, int);

// Flag for progress bar update
BOOL   fFirst;

/********************************************************************

   AddProgramItems

   Function that uses DDEML to add program items to group in Program
   Manager.

********************************************************************/

BOOL AddProgramItems (LPSTR szDummy) {
   char      szPercent[8];
   HDDEDATA  hData;
   HCONV     hConv;
   HSZ       szProgMan;
   int       lSelCount;
   LONG      lResult;
   LPLONG    lpSelection;
   LPSTR     szProgName;
   LPSTR     szExePath;
   LPSTR     szExecuteString;
   int       iIndex;
   int       iGroupCount;

   fFirst = FALSE;
   iGroupCount = 2;
// Retrieve the number of selected items from the file list.
   lSelCount = (int) SendMessage (hwndFileList, LB_GETSELCOUNT, 0, 0L);

// Allocate a block of memory to hold the indexes of the list selection.
   lpSelection = VirtualAlloc (NULL, lSelCount * sizeof (int), MEM_COMMIT,
         PAGE_READWRITE);
   if (lpSelection) {

// Establish a new conversation.
      if (DdeInitialize (&lIdInst2, (PFNCALLBACK) GroupDDECallback,
            (DWORD) APPCMD_CLIENTONLY, 0L)) {
         VirtualFree (lpSelection, lSelCount * sizeof (int), MEM_DECOMMIT);
         return (FALSE);
      }/*endIf*/

// Create a DDEML string handle for the Program Manager.
      szProgMan = DdeCreateStringHandle (lIdInst2, "PROGMAN", CP_WINANSI);

// Establish the conversation.
      hConv = DdeConnect (lIdInst2, szProgMan, szProgMan, &CCFilter);

      PostMessage (hwndStatus, WM_USER_UPDATE_STATUS, 0,
           ID_DDEML_CONNECT);


// Release the string handle that we created.
      DdeFreeStringHandle (lIdInst2, szProgMan);


// Allocate a block of memory to hold the path and name of the item.
      szProgName = VirtualAlloc (NULL, MAX_PATH * 2, MEM_COMMIT,
            PAGE_READWRITE);
      szExePath = szProgName + MAX_PATH;

// Allocate some memory to hold the command string.
      szExecuteString = VirtualAlloc (NULL, MAX_PATH * 4, MEM_COMMIT,
            PAGE_READWRITE);

// Retrieve the array of list selections.
      SendMessage (hwndFileList, LB_GETSELITEMS, (WPARAM) lSelCount,
            (LPARAM) lpSelection);

// Loop through the array of selections.
      for (iIndex = 0; iIndex < lSelCount; iIndex++) {

// Work around for limitation for in ProgMan (no more than 50 items per group)
         if (!(iIndex % 50) && (iIndex > 0)) {
            LPSTR      szText;
            LPSTR      szText2;

         // Allocate a block of memory for the group name 
            szText = VirtualAlloc (NULL, 64, MEM_COMMIT, PAGE_READWRITE);
            szText2 = VirtualAlloc (NULL, 256, MEM_COMMIT, PAGE_READWRITE);
            if (szText && szText2) {
               GetWindowText (hwndCombo, szText, 64);
               sprintf (szText2, "[CreateGroup(%s Part %ld )]", szText,
                     iGroupCount++);

         // Create a DDEML data handle for the command.
               hData = DdeCreateDataHandle (lIdInst2, szText2,
                  strlen (szText2) + 1, 0, (HSZ) NULL, CF_TEXT, 0L);

         // Send the transaction to the server waiting a maximum of 10 seconds.
         // The server will release the data handle.
               if (!DdeClientTransaction ((LPBYTE) hData, 0xFFFFFFFF, hConv,
                     (HSZ) NULL, 0, XTYP_EXECUTE, 10000, &lResult)) {
         // If it fails get the error code.
                  lResult = DdeGetLastError (lIdInst);
                  MessageBox (NULL, "DdeClientTransaction Failed",
                        "Error", MB_OK);
               }/*endIf*/
               VirtualFree (szText, 64, MEM_DECOMMIT);
               VirtualFree (szText2, 256, MEM_DECOMMIT);
            }
         }/*endIf*/

// Retrieve the Name of the Program Item to add.
         SendMessage (hwndFileList2, LB_GETTEXT, (WPARAM) lpSelection[iIndex],
               (LPARAM) szProgName);

// Set the name in the progress dialog
         SetDlgItemText (hwndDialog, IDL_ITEMNAME, szProgName);

// Retrieve the absolute path of the item to add.
         SendMessage (hwndPathList, LB_GETTEXT, (WPARAM) lpSelection[iIndex],
               (LPARAM) szExePath);

// Set the path in the progress dialog
         SetDlgItemText (hwndDialog, IDL_EXEPATH, szExePath);

// Set the percentage in the progress dialog
         sprintf (szPercent, "%d%%", (iIndex * 100) / lSelCount);
         SetDlgItemText (hwndDialog, IDL_PERCENTAGE, szPercent);
         UpdateProgressBar (iIndex + 1, lSelCount);

// Create the command string to add the item.
         sprintf (szExecuteString, "[AddItem(%s,%s)]", szExePath,
               (LPARAM) szProgName);

// Create a DDEML Data handle for the command string.
         hData = DdeCreateDataHandle (lIdInst2, szExecuteString,
               strlen (szExecuteString) + 1, 0, (HSZ) NULL, CF_TEXT, 0L);

// Send the command over to the program manager.
         if (!DdeClientTransaction ((LPBYTE) hData, 0xFFFFFFFF,
               hConv, (HSZ) NULL, 0, XTYP_EXECUTE, 1000, &lResult)) {
            lResult = DdeGetLastError (lIdInst2);
         }/*endIf*/
      }/*endFor*/

// Release the memory allocated for path and name retrieval.
      VirtualFree (szProgName, MAX_PATH * 2, MEM_DECOMMIT);

// Release the command line memory.
      VirtualFree (szExecuteString, MAX_PATH * 4, MEM_DECOMMIT);

// Disoconnect the DDEML Conversation
      DdeDisconnect (hConv);

// Release the memory allocate for the list selections.
      VirtualFree (lpSelection, lSelCount * sizeof (int), MEM_DECOMMIT);
   }/*endIf*/

// Clear the selection in the lists.

   SendMessage (hwndFileList, LB_SETSEL, (WPARAM) FALSE, (LPARAM) -1);
   SendMessage (hwndFileList2, LB_SETSEL, (WPARAM) FALSE, (LPARAM) -1);
   EnableWindow (hwndAddAll, FALSE);

// Uninitialize the local conversation.
   DdeUninitialize (lIdInst2);
   
   if (hwndDialog) {
      PostMessage (ghwndMain, WM_USER_CLOSE_DIALOG, 0, 0L);
   }/*endIf*/
   lIdInst2 = 0L;
   if (fBatch) {
      PostMessage (ghwndMain, WM_COMMAND, (WPARAM)
            (WPARAM) MAKELONG (ID_EXITBUTTON, BN_CLICKED),
            (LPARAM) hwndExitButton);
   }/*endIf*/
   return (TRUE);
}/* end AddProgramItems */


HWND   hwndBar;
RECT   rc;
double iStep;
HBRUSH hBrush;

/********************************************************************

   UpdateProgressBar

   Function to update the progress bar in the dialog while items are
   added to the Program Manager.

********************************************************************/

void UpdateProgressBar (int iIndex, int iCount) {
   HDC  hdc;

   if (!fFirst) {
      hwndBar = GetDlgItem (hwndDialog, IDL_PROGRESSBAR);
      if (hwndBar) {
         GetClientRect (hwndBar, &rc);
         fFirst = TRUE;
      } else {
         return;
      }/*endIf*/
      iStep = (double) rc.right / (double) iCount;
      hBrush = GetStockObject (BLACK_BRUSH);
   }/*endIf*/
   hdc = GetDC (hwndBar);
   rc.right = (int) (iStep * (double) iIndex);
   FillRect (hdc, &rc, hBrush);
   ReleaseDC (hwndBar, hdc);

   PostMessage (hwndStatus, WM_USER_UPDATE_STATUS, iIndex, ID_DDEML_ADD);
}/* end UpdateProgressBar */
