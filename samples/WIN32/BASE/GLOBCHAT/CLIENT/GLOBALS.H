// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
// PARTICULAR PURPOSE.
//
// Copyright (C) 1993, 1994  Microsoft Corporation.  All Rights Reserved.
//
// PURPOSE:
//    Contains declarations for all globally scoped names in the program.
//

//-------------------------------------------------------------------------
// Product identifier string defines

//  **TODO** Change these strings to the name of your application.

#define APPNAME       GlobCl
#define ICONFILE      GLOBCL.ICO
#define SZAPPNAME     "GLobCL"
#define SZDESCRIPTION "Global Chat Client Application"
#define SZABOUT       "About Global Chat"
#define SZVERSION     "Version 4.0"


//-------------------------------------------------------------------------
// Functions for handling main window messages.  The message-dispatching
// mechanism expects all message-handling functions to have the following
// prototype:
//
//     LRESULT FunctionName(HWND, UINT, WPARAM, LPARAM);

// **TODO**  Add message-handling function prototypes here.  Be sure to
//           add the function names to the main window message table in
//           globcl.c.

LRESULT MsgCreate(HWND, UINT, WPARAM, LPARAM);
LRESULT MsgSize(HWND, UINT, WPARAM, LPARAM);
LRESULT MsgSetfocus(HWND, UINT, WPARAM, LPARAM);
LRESULT MsgCommand(HWND, UINT, WPARAM, LPARAM);
LRESULT MsgDataready(HWND, UINT, WPARAM, LPARAM);
LRESULT MsgRefreshdisplay(HWND, UINT, WPARAM, LPARAM);
LRESULT MsgDisconnected(HWND, UINT, WPARAM, LPARAM);
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
//           globcl.c.

LRESULT CmdConnect(HWND, WORD, WORD, HWND);
LRESULT CmdSelect(HWND, WORD, WORD, HWND);
LRESULT CmdExit(HWND, WORD, WORD, HWND);
LRESULT CmdAbout(HWND, WORD, WORD, HWND);
LRESULT CmdOutbox(HWND, WORD, WORD, HWND);
LRESULT CmdDisconnect(HWND, WORD, WORD, HWND);
LRESULT CmdEndChat(HWND, WORD, WORD, HWND);

// 
//   Socket Data transfer Message definitions
//
typedef struct _MSGHDR            // Message Header
{
    BYTE            signature;    // Identifies start of a message
    WORD            length;       // size of message
    BYTE            command;      // message command
} MSGHDR, *LPMSGHDR;


typedef struct _DATAMSG            // Message--contains header + data
{
    MSGHDR          hdr;
    BYTE            data[0x8000];  // data
} DATAMSG, *LPDATAMSG;

DATAMSG xferbuf;


// Message constants
#define HDRSIZE      sizeof(MSGHDR)    // Size of Message header
#define REALLEN(x)   lstrlen(x) + 1    // REALLEN will now count terminating NULL of x

//    Globchat socket data structure to hold all necessary socket information
//
typedef struct _SOCKDATA
{
    SOCKET           sock;         // socket handle
    int              status;       // Socket's status
    struct sockaddr  addr;         // Address structure
	char             reserved[10]; // Generic sockaddr struct is not big enough for all
	                               // addresses (namely netbios addresses) so adding some
								   // space at the end.

	int              addrlen;      // Address length
    int              type;         // Socket type (from socket() call)
    int              protocol;     // protocol (from socket() call)
    int              currconnects; // Number of current connections on this socket
    SOCKET           peer;         // Handle of server's peer socket we are connected to
    char             name[16];	   // Our name
} SOCKDATA, *LPSOCKDATA;

SOCKDATA MySock;

//  SOCKDATA structure status options
#define SOCKSTAT_INIT        1
#define SOCKSTAT_LISTENING   2
#define SOCKSTAT_ACCEPTING   3
#define SOCKSTAT_CLOSED      4
#define SOCKSTAT_CONNECTED   5
#define SOCKSTAT_AVAILABLE   6
#define SOCKSTAT_INSESSION   7
#define SOCKSTAT_REQSESSION  8

//-------------------------------------------------------------------------
// Global function prototypes.

// **TODO**  Add global function prototypes here.

BOOL InitApplication(HINSTANCE, int);
BOOL CenterWindow(HWND, HWND);
BOOL ReceiveInBox(HWND, WPARAM, LPARAM, char *, int);
void SendOutBox(char *, int);
void AtoH(char *, char *, int);
unsigned char BtoH(char);
void CleanUp(void);
void GetAddrString(PSOCKADDR_IPX, char *);
void HtoA(char *, char *, int);
char HtoB(UCHAR);
BOOL senddatamessage (SOCKET, LPDATAMSG);
BOOL recvdatamessage (LPSOCKDATA, LPDATAMSG);
SOCKADDR_IPX IPXAddr;

// Callback functions.  These are called by Windows.

// **TODO**  Add new callback function prototypes here.

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);


//-------------------------------------------------------------------------
// Command ID definitions.  These definitions are used to associate menu
// items with commands.

// **TODO**  Add IDs for all menu commands here.  Be sure that the IDs you
//           add are also added to the main window command table in globcl.c
//           and the menu statements in the resource file.

// Options menu
#define IDM_CONNECT           1000
#define IDM_SELECT            1001
#define IDM_END_CHAT          1002
#define IDM_DISCONNECT_SERVER 1003
#define IDM_EXIT              1004

// Help menu
#define IDM_ABOUT      1100

//-------------------------------------------------------------------------
// User Defined Messages.  These definitions are used for indicating
// network events.

#define MW_DATAREADY       501
#define MW_DISPLAYREFRESH  502
#define MW_DISCONNECTED    503
#define LDM_CONNECTED      504

//-------------------------------------------------------------------------
// String Table ID definitions.

#define IDS_APPNAME     1
#define IDS_DESCRIPTION 2

//-------------------------------------------------------------------------
//  Main Window Edit Control defines.

#define ID_OUTBOX          601
#define ID_INBOX           602

//-------------------------------------------------------------------------
//  About dialog defines.

#define IDD_VERFIRST    100
#define IDD_VERLAST     104

//-------------------------------------------------------------------------
//  Connect dialog defines.

#define CD_NAME        200
#define CD_PROTOCOL    201
#define CD_SERVER      202
#define CD_SERVER_TEXT 203
#define CD_HELP        204

//-------------------------------------------------------------------------
//  Select dialog defines.

#define SD_LIST        300

//-------------------------------------------------------------------------
//  Globchat socket message commands

#define REGISTER_NAME             1
#define XFER_DATA                 2
#define REQUEST_SESSION           3
#define SESSION_REQUEST_RESPONSE  4
#define SESSION_CLOSE             5
#define DEREGISTER_NAME           6

//-------------------------------------------------------------------------
//  Socket constants
#define MYSIGNATURE               0xCC        // Starts every globchat socket message header
#define NWCHATID                  0x0756      // 0x0756 (network order) was assigned by Novell
                                              // for this specific application
#define DNSCHATID                 0x555       // Well known socket for DNS GUID generation


//-------------------------------------------------------------------------
// Global variable declarations.

extern HINSTANCE hInst;          // The current instance handle
extern char      szAppName[];    // The name of this application
extern char      szTitle[];      // The title bar text
SOCKET sock, SrvSock;
SOCKADDR_IPX RemAddr;
PSOCKADDR_IPX pSockAddr, pRemAddr;
struct sockaddr addr;
int addrlen;
HWND hOutWnd, hInWnd;
char szRcvBuf[0x8000];
char szSndBuf[0x8000];
extern char szConnectName[];
extern char szConnectServer[];
CSADDR_INFO CSAInfo[10];
CSADDR_INFO CSABuf[20];
DWORD dwCSABufsize;
char peername[16];
LPPROTOCOL_INFO lpProtBuf;
int goodprots[30];
char lpServName[256];
SOCKADDR_NB NBAddr;


// **TODO**  For NON-MDI applications, uncomment line 1 below and comment
//           line 2.  For MDI applications, uncomment line 2 below, comment
//           line 1, and then define hwndMDIGlobCl as a global variable in
//           INIT.C
#define hwndMDIGlobCl NULL        /* (1) Stub for NON-MDI applications. */
// extern HWND hwndMDIGlobCl;     /* (2) For MDI applications.          */


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




