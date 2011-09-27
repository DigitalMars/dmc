
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
   ddeinst.c
*/

#include <windows.h>
#include "ddeinst.h"
#include "ddextrn.h"

#include <stdio.h>
#include <string.h>

extern CONVCONTEXT CCFilter;

/********************************************************************

   StartTraverseThread

   Function that starts thread to traverse the specified directory.

********************************************************************/
BOOL StartTraverseThread (LPSTR lpArg) {
   HANDLE   hThread;
   LONG     lThreadId;

   hThread = CreateThread (NULL, 0, (LPTHREAD_START_ROUTINE) TraversePath,
      lpArg, CREATE_SUSPENDED | STANDARD_RIGHTS_REQUIRED, &lThreadId);
   if (hThread) {
      // SetThreadPriority (hThread, THREAD_PRIORITY_BELOW_NORMAL);
      ResumeThread(hThread);

/* Close the handle since we don't need it anymore */

      CloseHandle (hThread);
      return (TRUE);
   }/*endIf*/
   return (FALSE);
}/* end StartTraverseThread */


int   iRecurse = 0;

/********************************************************************

   TraversePath

   Function that recursively descends a specified directory looking
   for files that have the .exe extension.

********************************************************************/

BOOL TraversePath (LPSTR szPath) {
   HANDLE            hSearch;
   WIN32_FIND_DATA   findData;
   LPSTR             szTemp;
   LPSTR             szNewPath;
   LPSTR             szTempName;
   LPSTR             szFullPath;
   int               iLen;
   long              lError;
   long              lPos;
   long              lTemp;
   long              lNewPath;

// If there is no path then we need to exit.
   if (!szPath || (strlen (szPath) > (MAX_PATH - 5))) {
      return (FALSE);
   }/*endIf*/

// Increment the recursion level.
   iRecurse++;

// Get the length of the path plus room for the wildcard and nul.
   lTemp = strlen (szPath) + 5;

// allocate some memory
   szTemp = VirtualAlloc (NULL, lTemp, MEM_COMMIT, PAGE_READWRITE);

// Append the wildcard
   sprintf (szTemp, "%s\\*.*", szPath);

// Look for a file
   hSearch = FindFirstFile (szTemp, &findData);

// If nothing found then return.
   if (!hSearch || hSearch == (HANDLE) -1) {
      VirtualFree (szTemp, lTemp, MEM_DECOMMIT);
      iRecurse--;
      return (FALSE);
   }/*endIf*/

// Loop through the current directory looking for exectuables.
   do {
      if (strcmp (findData.cFileName, ".") &&
            strcmp (findData.cFileName, "..")) {

      // Ignore '.' and '..'
         if (findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {

         // If it is a directory then recurse.

            lNewPath = strlen (szPath) + strlen (findData.cFileName) + 1;
            szNewPath = VirtualAlloc (NULL, lNewPath, MEM_COMMIT,
                  PAGE_READWRITE);
            sprintf (szNewPath, "%s\\%s", szPath, findData.cFileName);
            TraversePath (szNewPath);
            VirtualFree (szNewPath, lNewPath, MEM_DECOMMIT);
         } else {

         // See if it ends in .exe

            iLen = strlen (findData.cFileName) - 4;
            szTempName = findData.cFileName + iLen;
            if (!strnicmp (szTempName, ".exe", 4)) {

            // Add the file name to the list of files.

               if (!(lPos = SendMessage (hwndFileList, LB_ADDSTRING,
                  (WPARAM) NULL, (LPARAM) findData.cFileName))) {

               // Debugging code

                  lError = GetLastError ();
               }/*endIf*/
               szTempName = VirtualAlloc (NULL, iLen + 4, MEM_COMMIT,
                     PAGE_READWRITE);
               strncpy (szTempName, findData.cFileName, iLen);

            // Add the constructed file name to the list of names

               SendMessage (hwndFileList2, LB_INSERTSTRING, (WPARAM) lPos,
                     (LPARAM) szTempName);
               VirtualFree (szTempName, iLen + 4, MEM_DECOMMIT);
               iLen = strlen (szPath) + iLen + 4;

            // Construct a full pathname to the file.

               szFullPath = VirtualAlloc (NULL, iLen, MEM_COMMIT,
                     PAGE_READWRITE);
               sprintf (szFullPath, "%s\\%s", szPath, findData.cFileName);

            // Add the full path to the hidden list.

               SendMessage (hwndPathList, LB_INSERTSTRING, (WPARAM) lPos,
                     (LPARAM) szFullPath);
               VirtualFree (szFullPath, iLen, MEM_DECOMMIT);
            }/*endIf*/
         }/*endIf*/
      }/*endIf*/

   // Keep looping until no more files.

   } while (FindNextFile (hSearch, &findData));

// End the search.

   FindClose (hSearch);
   VirtualFree (szTemp, lTemp, MEM_DECOMMIT);

// Decrement the recursion count.

   iRecurse--;
   if (!iRecurse) {

   // If recursion count == 0 then we are done so inform the primary thread.

      PostMessage (ghwndMain, WM_USER_THREAD_DONE, 0, 0L);
   }/*endIf*/
   return (TRUE);
}/* end TraversePath */

/********************************************************************

   StartGroupRetrievalThread

   Function that starts thread to retrieve the names of existing groups
   from the Program Manager.

********************************************************************/


BOOL StartGroupRetrievalThread () {
   HANDLE   hThread;
   LONG     lThreadId;

   hThread = CreateThread (NULL, 0, (LPTHREAD_START_ROUTINE) GroupRetrieval,
      "test", CREATE_SUSPENDED | STANDARD_RIGHTS_REQUIRED, &lThreadId);
   if (hThread) {
      SetThreadPriority (hThread, THREAD_PRIORITY_BELOW_NORMAL);
      ResumeThread (hThread);

/* Close the handle since we don't need it anymore */

      CloseHandle (hThread);
      return (TRUE);
   }/*endIf*/
   return (FALSE);
}/* end StartGroupRetrievalThread */


// Prototype for DDECallback function.
HDDEDATA CALLBACK GroupDDECallback (UINT, UINT, HANDLE, HSZ, HSZ, HDDEDATA,
      LONG, LONG);


/********************************************************************

   GroupRetrieval
   Function to obtain the names of all of the defined program groups in
   the Program Manager.

********************************************************************/

BOOL GroupRetrieval (LPSTR lpDummy) {
   LPBYTE     lpByte;
   LPSTR      szGroups;
   LPSTR      szToken;
   LPSTR      szMem;
   LPSTR      szToken2;
   long       lResult;
   LONG       lIdLocal;
   CRITICAL_SECTION lpCritical;
   HCONV      hConv;
   HDDEDATA   hDdeData;
   HSZ        szProgMan;
   HSZ        szTopic;


// Initialize the Dde id to 0

   lIdLocal = 0L;
   szGroups = NULL;

// Attempt to initialize a conversation.

   if (DdeInitialize (&lIdLocal, (PFNCALLBACK) GroupDDECallback,
         (DWORD) APPCMD_CLIENTONLY, 0L)) { 
      return (FALSE);
   }/*endIf*/

// Start a critical section. This fixes a problem where the DDEML
// Can hang under threaded conditions.

   InitializeCriticalSection (&lpCritical);
   EnterCriticalSection (&lpCritical);

// Create a string handle for the Dde conversation.

   szProgMan = DdeCreateStringHandle (lIdLocal, "PROGMAN", CP_WINANSI);
   
// Connect to the program manager.

   hConv = DdeConnect (lIdLocal, szProgMan, szProgMan, &CCFilter);

   PostMessage (hwndStatus, WM_USER_UPDATE_STATUS, 0, ID_DDEML_CONNECT);

// Create a handle for the Group topic.

   szTopic = DdeCreateStringHandle (lIdLocal, "Groups", CP_WINANSI);

// Execute a request for the group names.

   hDdeData = DdeClientTransaction (NULL, 0L, hConv, szTopic, CF_TEXT,
         XTYP_REQUEST, 1000, &lResult);

   PostMessage (hwndStatus, WM_USER_UPDATE_STATUS, 0, ID_DDEML_RETRIEVING);
// Release the two string handles.

   DdeFreeStringHandle (lIdLocal, szProgMan);
   DdeFreeStringHandle (lIdLocal, szTopic);

// Disconnect the conversation.

   DdeDisconnect (hConv);

   PostMessage (hwndStatus, WM_USER_UPDATE_STATUS, 0, ID_DDEML_COMPLETE);

// Exit and delete the critical section.

   LeaveCriticalSection (&lpCritical);
   DeleteCriticalSection (&lpCritical);

// Verify that we got something back.

   if (hDdeData && (hDdeData != TRUE)) {

   // Access the data to get a character pointer.

      lpByte = DdeAccessData (hDdeData, &lResult);

   // allocate a block of memory to use.

      szGroups = szMem = VirtualAlloc (NULL, lResult, MEM_COMMIT,
         PAGE_READWRITE);
      if (!szMem) {
         lResult = GetLastError ();
      } else {

      // Copy the Dde data to our own block.

         memcpy (szGroups, lpByte, lResult);
      }/*endIf*/

   // Free the Dde data that is there. This is our responsibility.

      DdeUnaccessData (hDdeData);
      DdeFreeDataHandle (hDdeData);
   }/*endIf*/

// If we have a memory block then parse it.

   if (szMem != NULL) {

   // Search for 'cr'.

      szToken = strtok (szGroups, "\n");

   // While we have a token use it.

      while (szToken) {

      // We have to strip out line feeds.

         if (szToken2 = strrchr (szToken, 0x0d)) {
            *szToken2 = '\0';
         }/*endIf*/
      
      // Add the resulting string to the combo box of groups.

         SendMessage (hwndCombo, CB_ADDSTRING, (WPARAM) 0, (LPARAM) szToken);

      // Find the next 'cr'.

         szToken = strtok (NULL, "\n");
      }/*endWhile*/

   // Release the memory block.

      VirtualFree (szMem, lResult, MEM_DECOMMIT);
   }/*endIf*/

// If there is a default name defined then use it. And enable the AddGroup
// Button.

   if (strlen (szUserGroup)) {
      SetWindowText (hwndCombo, szUserGroup);
      EnableWindow (hwndAddGroupButton, TRUE);
   } else {

   // There is no default group so disable the button.

      EnableWindow (hwndAddGroupButton, FALSE);
   }/*endIf*/

// Release the DDEML.

   DdeUninitialize (lIdLocal);

// Inform the primary thread that the group retrieval is completed.

   PostMessage (ghwndMain, WM_USER_THREAD_DONE, 0, 0L);
   return (TRUE);
}/* end GroupRetrieval */


/********************************************************************

   GroupDDECallback.
   
   Function exists for compatibility. Does nothing.

********************************************************************/

HDDEDATA CALLBACK GroupDDECallback (UINT uiType, UINT uiFmt, HANDLE hConv,
      HSZ sz1, HSZ sz2, HDDEDATA hData, LONG lData1, LONG lData2) {
   switch (uiType) {

   }/*endSwitch*/
   return ((HDDEDATA) NULL);
}/* end GroupDDECallback */
