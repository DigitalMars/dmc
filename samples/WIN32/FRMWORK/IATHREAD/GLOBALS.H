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

#define APPNAME       IAThread
#define SZAPPNAME     "IAThread"
#define SZCHILDNAME   "MdiChild"
#define SZDESCRIPTION "IAThread Example Application"
#define SZABOUT       "About IAThread"
#define SZVERSION     "Version 4.0"


//-------------------------------------------------------------------------
// Functions for handling main window messages.  The message-dispatching
// mechanism expects all message-handling functions to have the following
// prototype:
//
//     LRESULT FunctionName(HWND, UINT, WPARAM, LPARAM);

// Frame window messages
LRESULT MsgCommand(HWND, UINT, WPARAM, LPARAM);
LRESULT MsgClose(HWND, UINT, WPARAM, LPARAM);
LRESULT MsgDestroy(HWND, UINT, WPARAM, LPARAM);
LRESULT MsgCreate(HWND, UINT, WPARAM, LPARAM);


// MDI child window messages
LRESULT MsgMCCreate(HWND, UINT, WPARAM, LPARAM);
LRESULT MsgMCDestroy(HWND, UINT, WPARAM, LPARAM);
LRESULT MsgMCCommand(HWND, UINT, WPARAM, LPARAM);
LRESULT MsgMCMdiActivate(HWND hwnd, UINT uMessage, WPARAM wparam,
                         LPARAM lparam);


//-------------------------------------------------------------------------
// Functions for handling main window commands--ie. functions for
// processing WM_COMMAND messages based on the wParam value.
// The message-dispatching mechanism expects all command-handling
// functions to have the following prototype:
//
//     LRESULT FunctionName(HWND, WORD, WORD, HWND);

//
// Special command handler to forward a command to the active MDI child
//
LRESULT CmdForwardToActiveChild(HWND, WORD, WORD, HWND);

//
// File Commands
//
LRESULT CmdFileNew(HWND, WORD, WORD, HWND);
LRESULT CmdFileClose(HWND, WORD, WORD, HWND);
LRESULT CmdFileExit(HWND, WORD, WORD, HWND);

//
// Window Commands
//
LRESULT CmdWindowTile(HWND, WORD, WORD, HWND);
LRESULT CmdWindowCascade(HWND, WORD, WORD, HWND);
LRESULT CmdWindowIcons(HWND, WORD, WORD, HWND);
LRESULT CmdWindowCloseAll(HWND, WORD, WORD, HWND);

//
// Help Commands
//
LRESULT CmdAbout(HWND, WORD, WORD, HWND);


//
// Commands specific to MDI child windows
//
LRESULT CmdMCThreadPriority(HWND, WORD, WORD, HWND);
LRESULT CmdMCThreadSuspend(HWND, WORD, WORD, HWND);
LRESULT CmdMCThreadResume(HWND, WORD, WORD, HWND);


//-------------------------------------------------------------------------
// Global manifest constants

#define DEFAULT_STR_LEN        50       // Default length for strings
#define PRIORITY_TIMEOUT_VALUE 7000     // Initial thread priority timer
                                        // count

//-------------------------------------------------------------------------
// Global data structure types

// Data structure to contain strings and item data for listboxes.
typedef struct _LISTBOXDATA
{
    char   szString[DEFAULT_STR_LEN];
    LPARAM dwData;
} LISTBOXDATA, *LPLISTBOXDATA;

// Data associated with each child window.  This data will be shared between
// the child window its worker thread.
typedef struct _THREAD_DATA
{
    HWND              hWindow;
    HANDLE            hThread;
    DWORD             dwThreadID;
    BOOL              bKill;
    int               nPriorityTimer;
    CRITICAL_SECTION  csCritSec;
} THREAD_DATA, *PTHREAD_DATA;


//-------------------------------------------------------------------------
// Thread procedures

DWORD WINAPI ChildThreadProc (LPDWORD lpData);


//-------------------------------------------------------------------------
// Global function prototypes.

HWND InitFrameWindow(HINSTANCE, int, HMENU);
BOOL RegisterMDIChildClass(HINSTANCE);
BOOL CenterWindow(HWND, HWND);
BOOL FillListBox(HWND          hdlg,
                 int           idListBox,
                 LPLISTBOXDATA lpData,
                 int           cItems);

HANDLE GetCurChildWindowThread(void);
PTHREAD_DATA AllocThreadData (void);
BOOL FreeThreadData (PTHREAD_DATA pData);
BOOL ReducePriority (PTHREAD_DATA pThreadData);

//-------------------------------------------------------------------------
// Callback functions.  These are called by Windows.

LRESULT CALLBACK FrameWndProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK MDIChildWndProc(HWND, UINT, WPARAM, LPARAM);


// Position of window popup menu
#define INITWINDOWMENU        0         // Position in initial menu
#define DOCWINDOWMENU         2         // Position in document menu


//-------------------------------------------------------------------------
// Global variable declarations.

extern HINSTANCE hInst;            // The current instance handle
extern HWND      ghwndFrame;       // The MDI Frame window
extern HWND      ghwndMDIClient;   // The MDI Client window
extern UINT      gcOpenChildWnds;  // Count of all MDI child windows
extern HMENU     ghInitMenu;       // Frame window's initial menu
extern HMENU     ghDocMenu;        // Frame window's document menu


//-------------------------------------------------------------------------
// Message and command dispatch infrastructure.  The following type
// definitions and functions are used by the message and command dispatching
// mechanism and do not need to be changed.

    // Function pointer prototype for message handling functions.
typedef LRESULT (*PFNMSG)(HWND,UINT,WPARAM,LPARAM);

    // Function pointer prototype for command handling functions.
typedef LRESULT (*PFNCMD)(HWND,WORD,WORD,HWND);

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
} MSDI, FAR *LPMSDI;    // MeSsage Dipatch Information

    // This structure maps command IDs to command handling functions.
typedef struct _CMD
{
    WORD   wCommand;
    PFNCMD pfncmd;
} CMD;                  // CoMmand Dispatch structure

    // This structure contains all of the information that a command
    // message procedure passes to DispCommand in order to define the
    // command dispatching behavior for the window.
typedef struct _CMDI
{
    int  ccmd;          // Number of command dispatch structs in rgcmd
    CMD *rgcmd;         // Table of command dispatch structures
    EDWP edwp;          // Type of default window handler needed.
} CMDI, FAR *LPCMDI;    // CoMmand Dispatch Information

    // Message and command dispatching functions.  They look up messages
    // and commands in the dispatch tables and call the appropriate handler
    // function.
LRESULT DispMessage(LPMSDI, HWND, UINT, WPARAM, LPARAM);
LRESULT DispCommand(LPCMDI, HWND, WPARAM, LPARAM);

    // Message dispatch information for the main window
extern MSDI msdiMain;
    // Command dispatch information for the main window
extern CMDI cmdiMain;
