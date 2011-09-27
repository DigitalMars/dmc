// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
// PARTICULAR PURPOSE.
//
// Copyright (C) 1993 - 1995  Microsoft Corporation.  All Rights Reserved.
//
// PURPOSE:
//    Contains declarations for all globally scoped names in the program.
//

//-------------------------------------------------------------------------
// Product identifier string defines

//  **TODO** Change these strings to the name of your application.

    #define APPNAME       GDIDIB
    #define ICONFILE      GDIDIB.ICO
    #define SZAPPNAME     "GDIDIB"
    #define SZDESCRIPTION "GDIDIB Example Application"
    #define SZABOUT       "About GDIDIB"
    #define SZVERSION     "Version 4.0"


//-------------------------------------------------------------------------
// Functions for handling main window messages.  The message-dispatching
// mechanism expects all message-handling functions to have the following
// prototype:
//
//     LRESULT FunctionName(HWND, UINT, WPARAM, LPARAM);

// **TODO**  Add message-handling function prototypes here.  Be sure to
//           add the function names to the main window message table in
//           gdidib.c.

LRESULT MsgCommand            (HWND, UINT, WPARAM, LPARAM);  // gdidib.c   
LRESULT MsgClose              (HWND, UINT, WPARAM, LPARAM);
LRESULT MsgDestroy            (HWND, UINT, WPARAM, LPARAM);
LRESULT MsgCreate             (HWND, UINT, WPARAM, LPARAM);
LRESULT MsgSize               (HWND, UINT, WPARAM, LPARAM);
LRESULT MsgPaletteChanged     (HWND, UINT, WPARAM, LPARAM);
LRESULT MsgQueryNewPalette    (HWND, UINT, WPARAM, LPARAM);
LRESULT MsgQueryEndSession    (HWND, UINT, WPARAM, LPARAM);
LRESULT MsgNotify             (HWND, UINT, WPARAM, LPARAM);  // toolbar.c
LRESULT MsgTimer              (HWND, UINT, WPARAM, LPARAM);  // statbar.c
LRESULT MsgMenuSelect         (HWND, UINT, WPARAM, LPARAM);

//-------------------------------------------------------------------------
// Functions for handling main window commands--ie. functions for
// processing WM_COMMAND messages based on the wParam value.
// The message-dispatching mechanism expects all command-handling
// functions to have the following prototype:
//
//     LRESULT FunctionName(HWND, WORD, WORD, HWND);

// **TODO**  Add message-handling function prototypes here.  Be sure to
//           add the function names to the main window command table in
//           gdidib.c.

LRESULT CmdExit       (HWND, WORD, WORD, HWND);     // gdidib.c
LRESULT CmdAbout      (HWND, WORD, WORD, HWND);     // about.c 
LRESULT CmdInfo       (HWND, WORD, WORD, HWND);     // infodlg.c
LRESULT CmdNew        (HWND, WORD, WORD, HWND);     // filedlg.c
LRESULT CmdOpen       (HWND, WORD, WORD, HWND);
LRESULT CmdSave       (HWND, WORD, WORD, HWND);
LRESULT CmdClose      (HWND, WORD, WORD, HWND);
LRESULT CmdPrint      (HWND, WORD, WORD, HWND);     // print.c  
LRESULT CmdPageSetup  (HWND, WORD, WORD, HWND);
LRESULT CmdPrintSetup (HWND, WORD, WORD, HWND);

LRESULT CmdDrawMode   (HWND, WORD, WORD, HWND);     // client.c
LRESULT CmdFill       (HWND, WORD, WORD, HWND);
LRESULT CmdCreatePen  (HWND, WORD, WORD, HWND);
LRESULT CmdCreateBrush(HWND, WORD, WORD, HWND);
LRESULT CmdClear      (HWND, WORD, WORD, HWND);     // gdidib.c


//-------------------------------------------------------------------------
// Global function prototypes.

// **TODO**  Add global function prototypes here.

BOOL    InitApplication(HINSTANCE, int);     // init.c
BOOL    CenterWindow(HWND, HWND);            // misc.c
void    SetWindowTitle(HWND, LPSTR);         // misc.c 
void    InitDIBSection(HWND);                // misc.c
void    RemoveDIBSection(void);              // misc.c
HWND    CreateClientWindow(HWND);            // client.c
void    SizeClientWindow(HWND);              // client.c  
void    InitDrawObjects(void);               // client.c
int     QuerySaveChanges(HWND);              // filedlg.c
HBITMAP LoadDIBSection(HWND, LPSTR);         // fileio.c


// Callback functions.  These are called by Windows.

// **TODO**  Add new callback function prototypes here.  

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);       // gdidib.c
LRESULT CALLBACK ClientWndProc(HWND, UINT, WPARAM, LPARAM); // client.c

//-------------------------------------------------------------------------
// Global variable declarations.

extern HINSTANCE hInst;          // The current instance handle
extern char      szAppName[];    // The name of this application
extern HWND      hWndClient;     // The Client window
extern HMENU     hMenu;          // Application menu
extern HICON     hIcon;          // Application icon
                
// palette-related globals
extern HPALETTE  hPalette;       // App's logical palette
extern BOOL      bPalDevice;     // palette device flag

// drawing-related globals
extern LOGPEN    logPen;         // structure for pen attributes
extern LOGBRUSH  logBrush;       // structure for brush attributes

// DIB section-related globals
extern char      szCurrentFile[]; 
extern HBITMAP   hBitmap;
extern HANDLE    hDIBInfo;
extern LPVOID    lpvBits;
extern BOOL      fChanges;
                                    

// **TODO**  For NON-MDI applications, uncomment line 1 below and comment
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
typedef LRESULT (*PFNMSG)(HWND, UINT, WPARAM, LPARAM);

    // Function pointer prototype for command handling functions.
typedef LRESULT (*PFNCMD)(HWND, WORD, WORD, HWND);

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
