// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
// PARTICULAR PURPOSE.
//
// Copyright (C) 1992-1995  Microsoft Corporation.  All Rights Reserved.
//
// PURPOSE:
//    Contains declarations for all globally scoped names in the program.
//


//-------------------------------------------------------------------------
// Product identifier string defines

//  **TODO** Change these strings to the name of your application.

#define APPNAME       nddechat
#define ICONFILE      nddechat.ico
#define SZAPPNAME     "nddechat"
#define SZDESCRIPTION "NetDDE Chat Sample"
#define SZABOUT       "About NetDDE Chat"
#define SZVERSION     "Version 1.0"


//-------------------------------------------------------------------------
// Functions for handling main window messages.  The message-dispatching
// mechanism expects all message-handling functions to have the following
// prototype:
//
//     LRESULT FunctionName(HWND, UINT, WPARAM, LPARAM);

// **TODO**  Add message-handling function prototypes here.
//           add the function names to the main window message table in
//           rasberry.c.

LRESULT MsgCreate(HWND, UINT, WPARAM, LPARAM);
LRESULT MsgSetFocus(HWND, UINT, WPARAM, LPARAM);
LRESULT MsgSize(HWND, UINT, WPARAM, LPARAM);
LRESULT MsgConnect(HWND, UINT, WPARAM, LPARAM);
LRESULT MsgDisconnect(HWND, UINT, WPARAM, LPARAM);
LRESULT MsgReceiveText(HWND, UINT, WPARAM, LPARAM);
LRESULT MsgCommand(HWND, UINT, WPARAM, LPARAM);
LRESULT MsgDestroy(HWND, UINT, WPARAM, LPARAM);


//-------------------------------------------------------------------------
// Functions for handling main window commands--ie. functions for
// processing WM_COMMAND messages based on the wParam value.
// The message-dispatching mechanism expects all command-handling
// functions to have the following prototype:
//
//     LRESULT FunctionName(HWND, WORD, WORD, HWND);

// **TODO**  Add message-handling function prototypes here.  Be sure to
//           add the function names to the main window command table in
//           rasberry.c.

LRESULT CmdConnectDlg(HWND, WORD, WORD, HWND);
LRESULT CmdDisconnect(HWND, WORD, WORD, HWND);
LRESULT CmdSendNotify(HWND, WORD, WORD, HWND);
LRESULT CmdAbout(HWND, WORD, WORD, HWND);
LRESULT CmdExit(HWND, WORD, WORD, HWND);


//-------------------------------------------------------------------------
// Global function prototypes.

// **TODO**  Add global function prototypes here.

#ifdef WIN16
#include <ddeml.h> // for HDDEDATA definition
#define MAX_COMPUTERNAME_LENGTH 15
#ifndef ERROR_SUCCESS
#define ERROR_SUCCESS 0
#endif
#endif

BOOL InitApplication(HINSTANCE);
BOOL InitInstance(HINSTANCE, int);
BOOL CenterWindow(HWND, HWND);
HDDEDATA GetDataToTrans( VOID );

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
// Command ID definitions.  These definitions are used to associate menu
// items with commands.

// **TODO**  Add IDs for all menu commands here.  Be sure that the IDs you
//           add are also added to the main window command table in rasberry.c
//           and the menu statements in the resource file.

// File menu
#define IDM_EXIT         1000
#define IDM_CONNECT      1001
#define IDM_DISCONNECT   1002

// Help menu
#define IDM_ABOUT        1200

//-------------------------------------------------------------------------
// String Table ID definitions.

#define IDS_APPNAME         1
#define IDS_DESCRIPTION     2
#define IDS_SHARENAME       3
#define IDS_TOPIC           4
#define IDS_ITEM            5

//-------------------------------------------------------------------------
//  Main window defines.

#define IDE_SEND        100
#define IDE_RECV        101
#define IDC_STATIC       -1

#define WM_CONNECT      WM_USER+100
#define WM_DISCONNECT   WM_USER+101
#define WM_RECEIVE_TEXT WM_USER+102

#define STR_DWORD(x)   (*((UNALIGNED DWORD FAR*)(x+lstrlen(x)+1)))

#define STATE_UNCONN     0
#define STATE_PENDING    1
#define STATE_SERVER     2
#define STATE_CLIENT     3

//-------------------------------------------------------------------------
//  About dialog defines.

#define IDD_VERFIRST    200
#define IDD_VERLAST     204



//-------------------------------------------------------------------------
// Global variable declarations.

extern HINSTANCE hInst;          // The current instance handle
extern char      szAppName[];    // The name of this application
extern char      szTitle[];      // The title bar text
extern HWND      g_hwnd;
extern DWORD     g_idInst;
extern char      g_szServer[];
extern char      g_szTopic[];
extern BOOL      g_fState;
extern HWND      hwndSend;
extern HINSTANCE g_hInstNetDde;  // Handle of the nddeapi.dll 

// **TODO**  For NON-MDI applications uncomment line 1 below and comment
//           line 2.  For MDI applications, uncomment line 2 below, comment
//           line 1, and then define hwndMDIClient as a global variable in
//           INIT.C
#define hwndMDIClient NULL        /* (1) Stub for NON-MDI applications. */
// extern HWND hwndMDIClient;     /* (2) For MDI applications.          */


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
} MSDI, FAR *LPMSDI;   // MeSsage Dipatch Information

    // This structure maps command IDs to command handling functions.
typedef struct _CMD
{
    WORD   wCommand;
    PFNCMD pfncmd;
} CMD;                 // CoMmand Dispatch structure

    // This structure contains all of the information that a command
    // message procedure passes to DispCommand in order to define the
    // command dispatching behavior for the window.
typedef struct _CMDI
{
    int  ccmd;          // Number of command dispatch structs in rgcmd
    CMD *rgcmd;         // Table of command dispatch structures
    EDWP edwp;          // Type of default window handler needed.
} CMDI, FAR *LPCMDI;   // CoMmand Dispatch Information

    // Message and command dispatching functions.  They look up messages
    // and commands in the dispatch tables and call the appropriate handler
    // function.
LRESULT DispMessage(LPMSDI, HWND, UINT, WPARAM, LPARAM);
LRESULT DispCommand(LPCMDI, HWND, WPARAM, LPARAM);

    // Message dispatch information for the main window
extern MSDI msdiMain;
    // Command dispatch information for the main window
extern CMDI cmdiMain;



//-------------------------------------------------------------------------
// Version string definitions--Leave these alone.

#define SZRCOMPANYNAME "CompanyName"
#define SZRDESCRIPTION "FileDescription"
#define SZRVERSION     "FileVersion"
#define SZRAPPNAME     "InternalName"
#define SZRCOPYRIGHT   "LegalCopyright"
#define SZRTRADEMARK   "LegalTrademarks"
#define SZRPRODNAME    "ProductName"
#define SZRPRODVER     "ProuctVersion"
