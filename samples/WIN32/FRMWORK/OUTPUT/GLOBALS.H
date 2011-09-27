// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
// PARTICULAR PURPOSE.
//
// Copyright (C) 1993-1995  Microsoft Corporation.  All Rights Reserved.
//
// PURPOSE:
//    Contains declarations for all globally scoped names in the program.
//


//-------------------------------------------------------------------------
// Global function prototypes.

// **TODO**  Add global function prototypes here.

BOOL InitApplication(HINSTANCE);
BOOL InitInstance(HINSTANCE, int);
BOOL InitOutput(void);
void FreeOutput(void);
void DrawOutput(HWND);


    // Callback functions.  These are called by Windows.

// **TODO**  Add new callback function prototypes here.  Win16 compiles
//           require the __export keyword to generate proper prolog
//           and epilog code for exported functions.

#ifdef WIN16

LRESULT CALLBACK __export WndProc(HWND, UINT, WPARAM, LPARAM);

#else

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

#endif


//-------------------------------------------------------------------------
// Global variable declarations.

extern HINSTANCE hInst;          // The current instance handle
extern char      szAppName[];    // The name of this application
extern char      szTitle[];      // The title bar text
