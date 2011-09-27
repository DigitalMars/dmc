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

#include <nspapi.h>
#include <wsipx.h>

//-------------------------------------------------------------------------
// Product identifier string defines

//  **TODO** Change these strings to the name of your application.

#define APPNAME       GlobChat
#define ICONFILE      GLOBCHAT.ICO
#define SZAPPNAME     "GlobChat"
#define SZDESCRIPTION "Global Chat Example Application"
#define SZABOUT       "About GlobChat"
#define SZVERSION     "Version 4.0"


//-------------------------------------------------------------------------
// Functions for handling main window messages.  The message-dispatching
// mechanism expects all message-handling functions to have the following
// prototype:
//
//     LRESULT FunctionName(HWND, UINT, WPARAM, LPARAM);

// **TODO**  Add message-handling function prototypes here.  Be sure to
//           add the function names to the main window message table in
//           generic.c.

LRESULT MsgCreate(HWND, UINT, WPARAM, LPARAM);
LRESULT MsgSize(HWND, UINT, WPARAM, LPARAM);
LRESULT MsgTimer(HWND, UINT, WPARAM, LPARAM);
LRESULT MsgCommand(HWND, UINT, WPARAM, LPARAM);
LRESULT MsgDestroy(HWND, UINT, WPARAM, LPARAM);
LRESULT MsgConnected(HWND, UINT, WPARAM, LPARAM);
LRESULT MsgDataready(HWND, UINT, WPARAM, LPARAM);


//-------------------------------------------------------------------------
// Functions for handling main window commands--ie. functions for
// processing WM_COMMAND messages based on the wParam value.
// The message-dispatching mechanism expects all command-handling
// functions to have the following prototype:
//
//     LRESULT FunctionName(HWND, WORD, WORD, HWND);

// **TODO**  Add message-handling function prototypes here.  Be sure to
//           add the function names to the main window command table in
//           generic.c.

LRESULT CmdExit(HWND, WORD, WORD, HWND);
LRESULT CmdAbout(HWND, WORD, WORD, HWND);

//-------------------------------------------------------------------------
//  Globchat socket message definitions
//
typedef struct _MSGHDR            // Message header
{
    BYTE            signature;    // Identifies start of a message
    WORD            length;       // size of message
    BYTE            command;      // message command
} MSGHDR, *LPMSGHDR;


typedef struct _DATAMSG            // Message = message header + data
{
    MSGHDR          hdr;
    BYTE            data[0x8000];  // data
} DATAMSG, *LPDATAMSG;

DATAMSG xferbuf;

#define HDRSIZE      sizeof(MSGHDR)    // Message header size

#define REALLEN(x)   lstrlen(x) + 1    // REALLEN will now count terminating NULL of x

//-------------------------------------------------------------------------
// My personal SOCKDATA structure for storing info on my sockets

typedef struct _SOCKDATA
{
    SOCKET           sock;           // socket handle
    int              status;         // Socket's status
    struct sockaddr  addr;           // Address structure
	char             reserved[10];   // Generic sockaddr struct is not big enough for all
	                                 // addresses (namely netbios addresses) so adding some
								     // space at the end.
	int              addrlen;        // Address length
    int              type;           // Socket type (from socket() call)
    int              protocol;       // protocol (from socket() call)
    int              currconnects;   // Number of current connections on this socket
    LPTSTR           lpProtocolName; // Pointer to protocol name socket is running on
    int              servsockindex;  // Index to array of server sockets
    SOCKET           peer;           // Associated peer socket which this socket is in session with
    char             name[16];       // User name
} SOCKDATA, *LPSOCKDATA;

LPSOCKDATA ServerSockets;        // Listening (server) sockets
LPSOCKDATA ConnectedSockets;	 // Accepted (client) sockets

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
void AtoH(char *, char *, int);
unsigned char BtoH(char);
void deregistername(char *);
BOOL senddatamessage(SOCKET, LPDATAMSG);
BOOL recvdatamessage (LPSOCKDATA, LPDATAMSG);
void UpdateClientList(char *, int, char *);
BOOL MakeServSock(HWND, LPSOCKDATA, LPPROTOCOL_INFO);


// Callback functions.  These are called by Windows.

// **TODO**  Add new callback function prototypes here.

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);


//-------------------------------------------------------------------------
// Command ID definitions.  These definitions are used to associate menu
// items with commands.

// **TODO**  Add IDs for all menu commands here.  Be sure that the IDs you
//           add are also added to the main window command table in generic.c
//           and the menu statements in the resource file.

// File menu
#define IDM_EXIT    1000

// Help menu
#define IDM_ABOUT   1100

// Status List boxes
#define ID_PROTOCOLBOX 400
#define ID_CLIENTBOX   401

//-------------------------------------------------------------------------
// String Table ID definitions.

#define IDS_APPNAME     1
#define IDS_DESCRIPTION 2

//-------------------------------------------------------------------------
//  About dialog defines.

#define IDD_VERFIRST    100
#define IDD_VERLAST     104

//-------------------------------------------------------------------------
// Application Specific Messages.

#define MW_CONNECTED    200
#define MW_DATAREADY    201

#define SAPTIMER        300

//--------------------------------------------------------------------------
//  Other constants

#define NWCHATID        0x5607      // 0x0756 (network order) was assigned by Novell
                                    // for this specific application
#define DNSCHATID       0x555       // TCP well known port

// Globchat socket message commands
#define REGISTER_NAME             1
#define XFER_DATA                 2
#define REQUEST_SESSION           3
#define SESSION_REQUEST_RESPONSE  4
#define SESSION_CLOSE             5
#define DEREGISTER_NAME           6

#define MYSIGNATURE               0xCC   // First byte in every message header

//-------------------------------------------------------------------------
// Global variable declarations.

extern HINSTANCE hInst;          // The current instance handle
extern char      szAppName[];    // The name of this application
extern char      szTitle[];      // The title bar text
HANDLE ConnectHeap;
LPPROTOCOL_INFO lpProtBuf;
SOCKET SAPSocket;
SOCKADDR_IPX SAPSockAddr, SAPDestSockAddr;
int NextFree, MaxConnects;
char aliasbuf[512];
int sizealiasbuf;
HWND hwndProtocolList;
HWND hwndClientList;




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

//-------------------------------------------------------------------------
// SAP structure definition for a single entry SAP packet.

typedef struct _SAPHDR
{
    WORD     operation;    // 1 = Req., 2 = Resp., 3 = Get Nearest Req, 4 = Get Nearest Resp.
    WORD     servicetype;  // kind of service
    char     name[48];         // Name of Server
    char     network[4];      // network number
    char     node[6];         // node number
    char     socket[2];       // socket number
    WORD     hops;         // Number of hops to server
} SAPHDR, *LPSAPHDR;  // SAP structure, pointer to SAP structure

SAPHDR SAPData;

