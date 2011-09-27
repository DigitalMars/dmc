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

    #define APPNAME       GDIPal
    #define ICONFILE      GDIPal.ICO
    #define SZAPPNAME     "GDIPal"
    #define SZDESCRIPTION "GDI Palette Example Application"
    #define SZABOUT       "About GDI Palette"
    #define SZVERSION     "Version 4.0"

//-------------------------------------------------------------------------
// Functions for handling main window messages.  The message-dispatching
// mechanism expects all message-handling functions to have the following
// prototype:
//
//     LRESULT FunctionName(HWND, UINT, WPARAM, LPARAM);

// **TODO**  Add message-handling function prototypes here.  Be sure to
//           add the function names to the main window message table in
//           gdipal.c.

LRESULT MsgCommand         (HWND, UINT, WPARAM, LPARAM); // gdipal.c
LRESULT MsgDestroy         (HWND, UINT, WPARAM, LPARAM);
LRESULT MsgCreate          (HWND, UINT, WPARAM, LPARAM);
LRESULT MsgSize            (HWND, UINT, WPARAM, LPARAM);
LRESULT MsgPaletteChanged  (HWND, UINT, WPARAM, LPARAM);
LRESULT MsgQueryNewPalette (HWND, UINT, WPARAM, LPARAM);
LRESULT MsgNotify          (HWND, UINT, WPARAM, LPARAM); // toolbar.c
LRESULT MsgTimer           (HWND, UINT, WPARAM, LPARAM); // statbar.c
LRESULT MsgMenuSelect      (HWND, UINT, WPARAM, LPARAM);


//-------------------------------------------------------------------------
// Functions for handling main window commands--ie. functions for
// processing WM_COMMAND messages based on the wParam value.
// The message-dispatching mechanism expects all command-handling
// functions to have the following prototype:
//
//     LRESULT FunctionName(HWND, WORD, WORD, HWND);

// **TODO**  Add message-handling function prototypes here.  Be sure to
//           add the function names to the main window command table in
//           gdipal.c.

LRESULT CmdFileNew    (HWND, WORD, WORD, HWND);     // gdipal.c
LRESULT CmdRefresh    (HWND, WORD, WORD, HWND);
LRESULT CmdExit       (HWND, WORD, WORD, HWND);

LRESULT CmdAbout      (HWND, WORD, WORD, HWND);     // about.c

LRESULT CmdInfo       (HWND, WORD, WORD, HWND);     // infodlg.c

LRESULT CmdDrawMode   (HWND, WORD, WORD, HWND);     // client.c
LRESULT CmdFill       (HWND, WORD, WORD, HWND);
LRESULT CmdCreatePen  (HWND, WORD, WORD, HWND);
LRESULT CmdCreateBrush(HWND, WORD, WORD, HWND);


//-------------------------------------------------------------------------
// Global function prototypes.

// **TODO**  Add global function prototypes here.

BOOL    InitApplication(HINSTANCE, int);
BOOL    CenterWindow(HWND, HWND);

HWND    CreateClientWindow(HWND);
VOID    ClientNewDrawing(VOID);

    // Callback functions.  These are called by Windows.

// **TODO**  Add new callback function prototypes here.

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK ClientWndProc(HWND, UINT, WPARAM, LPARAM);


//-------------------------------------------------------------------------
// Command ID definitions.  These definitions are used to associate menu
// items with commands.

// **TODO**  Add IDs for all menu commands here.  Be sure that the IDs you
//           add are also added to the main window command table in gdipal.c
//           and the menu statements in the resource file.

// File menu
#define IDM_FILEMENU    1000
#define IDM_FILENEW     1001
#define IDM_FILEOPEN    1002
#define IDM_FILESAVE    1003
#define IDM_FILESAVEAS  1004
#define IDM_FILECLOSE   1005
#define IDM_FILEPRINT   1006
#define IDM_FILEPAGESU  1007
#define IDM_FILEPRINTSU 1008
#define IDM_EXIT        1009

// Info menu
#define IDM_INFO        1050

// Edit menu
#define IDM_EDITMENU    1100
#define IDM_EDITUNDO    1101
#define IDM_EDITCUT     1102
#define IDM_EDITCOPY    1103
#define IDM_EDITPASTE   1104
#define IDM_EDITCLEAR   1105

// Draw menu
#define IDM_DRAWMENU    1200
#define IDM_PIXEL       1201
#define IDM_LINE        1202
#define IDM_RECT        1203
#define IDM_ELLIPSE     1204
#define IDM_BEZIER      1205
#define IDM_FILL        1206
#define IDM_NOFILL      1207
#define IDM_CREATEPEN   1208
#define IDM_CREATEBRUSH 1209
#define IDM_REFRESH     1210

// Help menu
#define IDM_HELPMENU    1300
#define IDM_ABOUT       1301


//-------------------------------------------------------------------------
// String Table ID definitions.

#define IDS_APPNAME     1
#define IDS_DESCRIPTION 2

//-------------------------------------------------------------------------
//  About dialog defines.

#define IDD_VERFIRST    100
#define IDD_VERLAST     104


//-------------------------------------------------------------------------
// Global variable declarations.

extern HINSTANCE hInst;          // The current instance handle
extern char      szAppName[];    // The name of this application
extern char      szTitle[];      // The title bar text
extern HWND      hWndClient;     // The Client window

// palette-related globals
extern HPALETTE  hPalette;       // App's logical palette
extern BOOL      bPalDevice;     // palette device flag

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
