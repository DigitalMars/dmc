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
// Product identifier string defines

//  **TODO** Change these strings to the name of your application.

#define APPNAME       Input


//-------------------------------------------------------------------------
// Functions for handling main window messages.  The message-dispatching
// mechanism expects all message-handling functions to have the following
// prototype:
//
//     LRESULT FunctionName(HWND, UINT, WPARAM, LPARAM);

// **TODO**  Add message-handling function prototypes here.  Be sure to
//           add the function names to the main window message table in
//           input.c.

LRESULT MsgCommand(HWND, UINT, WPARAM, LPARAM);
LRESULT MsgCreate(HWND, UINT, WPARAM, LPARAM);
LRESULT MsgDestroy(HWND, UINT, WPARAM, LPARAM);
LRESULT MsgMouseMove(HWND, UINT, WPARAM, LPARAM);
LRESULT MsgLButtonDown(HWND, UINT, WPARAM, LPARAM);
LRESULT MsgLButtonUp(HWND, UINT, WPARAM, LPARAM);
LRESULT MsgLButtonDoubleClick(HWND, UINT, WPARAM, LPARAM);
LRESULT MsgRButtonDown(HWND, UINT, WPARAM, LPARAM);
LRESULT MsgRButtonUp(HWND, UINT, WPARAM, LPARAM);
LRESULT MsgRButtonDoubleClick(HWND, UINT, WPARAM, LPARAM);
LRESULT MsgKeyDown(HWND, UINT, WPARAM, LPARAM);
LRESULT MsgKeyUp(HWND, UINT, WPARAM, LPARAM);
LRESULT MsgChar(HWND, UINT, WPARAM, LPARAM);
LRESULT MsgTimer(HWND, UINT, WPARAM, LPARAM);
LRESULT MsgScroll(HWND, UINT, WPARAM, LPARAM);
LRESULT MsgScroll(HWND, UINT, WPARAM, LPARAM);
LRESULT MsgPaint(HWND, UINT, WPARAM, LPARAM);


//-------------------------------------------------------------------------
// Global function prototypes.

// **TODO**  Add global function prototypes here.

BOOL InitApplication(HINSTANCE);
BOOL InitInstance(HINSTANCE, int);
BOOL InitInput(HWND);

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

// **TODO**  For NON-MDI applications, uncomment line 1 below and comment
//           line 2.  For MDI applications, uncomment line 2 below, comment
//           line 1, and then define hwndMDIClient as a global variable in
//           INIT.C
#define hwndMDIClient NULL        /* (1) Stub for NON-MDI applications. */
// extern HWND hwndMDIClient;     /* (2) For MDI applications.          */


//-------------------------------------------------------------------------
// Message dispatch infrastructure.  The following type
// definitions and functions are used by the message dispatching
// mechanism and do not need to be changed.

    // Function pointer prototype for message handling functions.
typedef LRESULT (*PFNMSG)(HWND,UINT,WPARAM,LPARAM);

    // Enumerated type used to determine which default window procedure
    // should be called by the message- and command-dispatching mechanism
    // if a message or command is not handled explicitly.
typedef enum
{
   edwpNone,            // Do not call any default procedure.
   edwpWindow,          // Call DefWindowProc.
   edwpDialog,          // Call DefDlgProc (This should be used only for
                        // custom dialogs - standard dialog use edwpNone).
   edwpMDIChild,        // Call DefMDIChildProc.
   edwpMDIFrame         // Call DefFrameProc.
} EDWP;                // Enumeration for Default Window Procedures

    // This structure maps messages to message handling functions.
typedef struct _MSD
{
    UINT   uMessage;
    PFNMSG pfnmsg;
} MSD;                 // MeSsage Dispatch structure

    // This structure contains all of the information that a window
    // procedure passes to DispMessage in order to define the message
    // dispatching behavior for the window.
typedef struct _MSDI
{
    int  cmsd;          // Number of message dispatch structs in rgmsd
    MSD *rgmsd;         // Table of message dispatch structures
    EDWP edwp;          // Type of default window handler needed.
} MSDI, FAR *LPMSDI;   // MeSsage Dipatch Information

    // Message dispatching function.  This looks up messages in the
    // dispatch table and call the appropriate handler function.
LRESULT DispMessage(LPMSDI, HWND, UINT, WPARAM, LPARAM);

    // Message dispatch information for the main window
extern MSDI msdiMain;
