
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
   ddemain.c
*/

#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ddeinst.h"
#include "ddeglbl.h"


HMENU   hMenu;

/********************************************************************

   WinMain.
   
   Function that starts the app.

********************************************************************/

int APIENTRY WinMain (HINSTANCE hInstance, HINSTANCE hPrevInst,
      LPSTR lpCmdLine, int iShowCmd) {
   MSG   msg;

   ghModule = GetModuleHandle (NULL);


// Initialize the Application
   if (!InitializeApp ()) {
      MessageBox (ghwndMain, "memory: InitializeApp failure!", "Error", MB_OK);
      return (FALSE);
   }/*endIf*/


// Parse the command line
   if (!DecodeOptions (lpCmdLine)) {
      MessageBox (ghwndMain, "decode: Command line errors!", "Error", MB_OK);
      CleanUpApp ();
      return (FALSE);
   }/*endIf*/


// Load the accelerators
   if (!(ghAccel = LoadAccelerators (ghModule, MAKEINTRESOURCE(ACCEL_ID)))) {
      MessageBox(ghwndMain, "memory: Load Accel failure!", "Error", MB_OK);
   }/*endIf*/


// Test
   PostMessage (ghwndMain, WM_USER_GET_APPS, 0, 0L);

// Get the group names from the Program Manager
   PostMessage (ghwndMain, WM_USER_GET_GROUPS, 0, 0L);

// Main Event Loop
   while (GetMessage (&msg, NULL, 0, 0)) {
      if (!TranslateAccelerator (ghwndMain, ghAccel, &msg)) {
         TranslateMessage (&msg);
         DispatchMessage (&msg);
      }/*endIf*/
   }/*endWhile*/

// Clean up
   CleanUpApp ();
   return (TRUE);
}/* end WinMain */

/********************************************************************

   InitializeApp.
   
   Function that registers the window classes and creates the main
   window.

********************************************************************/

BOOL InitializeApp () {
   char      szErrorString[80];
   WNDCLASS wc;
   long      lError;

// Fill in the class info for the main window.
   wc.style         = CS_OWNDC;
   wc.lpfnWndProc   = (WNDPROC)MainWndProc;
   wc.cbClsExtra    = 0;
   wc.cbWndExtra    = sizeof(LONG);
   wc.hInstance     = ghModule;
   wc.hIcon         = LoadIcon(ghModule, MAKEINTRESOURCE(APPICON));
   wc.hCursor       = LoadCursor(NULL, IDC_ARROW);
   wc.hbrBackground = (HBRUSH)(COLOR_WINDOW+1);
   wc.lpszMenuName  = "MainMenu";
   wc.lpszClassName = "DDEMLInstaller";

   if (!RegisterClass(&wc))
      return FALSE;

// Fill in the class info for the status bar.
   wc.style         = CS_OWNDC | CS_HREDRAW | CS_VREDRAW;
   wc.lpfnWndProc   = (WNDPROC)StatusBarWndProc;
   wc.hIcon         = NULL;
   wc.hCursor       = LoadCursor(NULL, IDC_ARROW);
   wc.hbrBackground = (HBRUSH)(COLOR_BTNSHADOW);
   wc.lpszMenuName  = NULL;
   wc.lpszClassName = "StatusBar";

   if (!RegisterClass(&wc))
      return FALSE;

   hMenu = LoadMenu (ghModule, MAKEINTRESOURCE (MainMenu));
   if (!hMenu) {
      lError = GetLastError ();
      sprintf (szErrorString, "MainMenu load failed %ld", lError);
      MessageBox (GetDesktopWindow (), szErrorString, "Error", MB_OK);
   }/*endIf*/

   ghwndMain = CreateWindowEx (0L, "DDEMLInstaller", "Sample Installer",
         WS_OVERLAPPED   | WS_CAPTION     | WS_BORDER       |
         WS_THICKFRAME   | WS_MAXIMIZEBOX | WS_MINIMIZEBOX  |
         WS_CLIPCHILDREN | WS_SYSMENU,
         80, 70, 550, 250,
         NULL, hMenu, ghModule, NULL);

   if (ghwndMain == NULL)
      return FALSE;

   ShowWindow (ghwndMain, SW_SHOWDEFAULT);
   UpdateWindow (ghwndMain);

   SetWindowLong (ghwndMain, GWL_USERDATA, 0L);

// Set the initial focus to the main window
   SetFocus(ghwndMain);    /* set initial focus */

   return TRUE;
}/* end InitializeApp */

/********************************************************************

   CleanUpApp.
   
   Function that takes care of house cleaning when app terminates.

********************************************************************/

void CleanUpApp () {

// Destroy the menu loaded in InitializeApp.
   DestroyMenu (hMenu);

// If the DDEML conversation id is non-zero the Uninitialize the conversation
   if (lIdInst2) {
      DdeUninitialize (lIdInst2);
   }/*endIf*/

// If memory has been allocated for the user defined path then release it.
   if (szUserPath) {
      VirtualFree (szUserPath, MAX_PATH, MEM_DECOMMIT);
   }/*endIf*/
   if (szUserGroup) {
      VirtualFree (szUserGroup, MAX_PATH, MEM_DECOMMIT);
   }/*endIf*/
}/* end CleanUpApp */

/********************************************************************

   DecodeOptions.
   
   Function that decodes the command line looking for the user specified
   arguments.

********************************************************************/

BOOL DecodeOptions (LPSTR lpCmdLine) {
   LPSTR  szFirst;
   LPSTR  szToken;

// Allocate a block of memory to use for the user specified path.
   szUserPath = VirtualAlloc (NULL, MAX_PATH, MEM_COMMIT, PAGE_READWRITE);
   szUserGroup = VirtualAlloc (NULL, MAX_PATH, MEM_COMMIT, PAGE_READWRITE);
   if (!GetEnvironmentVariable ("MSTOOLS", szUserPath, MAX_PATH - 1)) {
      strcpy (szUserPath, "c:\\mstools\\samples");
   } else {
      strcat (szUserPath, "\\samples");
   }/*endIf*/
   strcpy (szUserGroup, "Sample Applications");
   fBatch = FALSE;
   if (!(szToken = strtok (lpCmdLine, "-/"))) {
      return (TRUE);
   }/*endIf*/
   do {
      switch (*szToken) {
         case 's':
         case 'S':{
            szFirst = szToken + 1;
            while (*szFirst == ' ') {
               szFirst++;
            }/*endWhile*/
            strcpy (szUserPath, szFirst);
            while (szFirst = strrchr (szUserPath, ' ')) {
               *szFirst = '\0';
            }/*endWhile*/
            break;
         }/*endCase*/
         case 'b':
         case 'B': {
            fBatch = TRUE;
            break;
         }/*endCase*/
         case 'i':
         case 'I': {
            fBatch = FALSE;
            break;
         }/*endCase*/
         case 'g':
         case 'G': {
            szFirst = szToken + 1;
            while (*szFirst == ' ') szFirst++;
            strcpy (szUserGroup, szFirst);
            while (szFirst = strrchr (szUserGroup, ' ')) {
               *szFirst = '\0';
            }/*endWhile*/
            break;
         }/*endCase*/
      }/*endSwitch*/
   } while (szToken = strtok (NULL, "-/"));
   return (TRUE);
}/* end DecodeOptions */
