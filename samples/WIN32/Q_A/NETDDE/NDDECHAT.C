// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
// PARTICULAR PURPOSE.
//
// Copyright (C) 1992-1995  Microsoft Corporation.  All Rights Reserved.
//
//  MODULE:  nddechat.c
//
//  PURPOSE:  Implement the windows procedure for the main application
//            windows.
//
//  FUNCTIONS:
//    WndProc      - Processes messages for the main window.
//    MsgCreate    - Handle the WM_CREATE messages for the main window.
//    MsgSize      - Handle the WM_SIZE messages for the main window.
//    MsgCommand   - Handle the WM_COMMAND messages for the main window.
//    MsgDestroy   - Handles the WM_DESTROY message by calling
//                   PostQuitMessage().
//    CmdExit      - Handles the file exit command by calling destory
//                   wi\nndow on the main window.
//
//  COMMENTS:
//
//  AUTHOR: Craig Link - Microsoft Developer Support
//

#include <windows.h>            // required for all Windows applications
#include <windowsx.h>
#include <stdlib.h>
#include <nddeapi.h>
#ifdef WIN16
#include "win16ext.h"           // required only for win16 applications
#endif

#include "globals.h"            // prototypes specific to this application
#include "nddeutil.h"
#include "mem.h"
#include "connect.h"
#include "ddecback.h"

// Main window message table definition.
MSD rgmsd[] =
{
    {WM_CREATE,       MsgCreate},
    {WM_SIZE,         MsgSize},
    {WM_CONNECT,      MsgConnect},
    {WM_DISCONNECT,   MsgDisconnect},
    {WM_RECEIVE_TEXT, MsgReceiveText},
    {WM_COMMAND,      MsgCommand},
    {WM_DESTROY,      MsgDestroy}
};

MSDI msdiMain =
{
    sizeof(rgmsd) / sizeof(MSD),
    rgmsd,
    edwpWindow
};
// Main window command table definition.
CMD rgcmd[] =
{
    {IDM_CONNECT,    CmdConnectDlg},
    {IDM_DISCONNECT, CmdDisconnect},
    {IDE_SEND,       CmdSendNotify},
    {IDM_EXIT,       CmdExit},
    {IDM_ABOUT,      CmdAbout}
};

CMDI cmdiMain =
{
    sizeof(rgcmd) / sizeof(CMD),
    rgcmd,
    edwpWindow
};

// Application "globals"
HWND  g_hwnd;                     // handle of main window
char  g_szServer[MAX_COMPUTERNAME_LENGTH+3];  // name of server connected to
char  g_szTopic[MAX_TOPICNAME+1]; // netdde topic name
BOOL  g_fState = STATE_UNCONN;    // the app is currently a DDE server
DWORD g_idInst = 0;               // DDEML instance identifier

// Module specific "globals"  Used when a variable needs to be
// accessed in more than on handler function.

LPSTR pszLast = NULL;             // contents of send buffer at time of last send

char szShareName[MAX_NDDESHARENAME+1];  // netdde share name
char szItem[MAX_ITEMNAME+1];

HCONV hconv = 0;    // handle for DDEML conversation

HWND hwndRecv;      // handle of edit control for received data
HWND hwndSend;      // handle of edit control for sent data



//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
//  PARAMETERS:
//    hwnd     - window handle
//    uMessage - message number
//    wparam   - additional information (dependant on message number)
//    lparam   - additional information (dependant on message number)
//
//  RETURN VALUE:
//    The return value depends on the message number.  If the message
//    is implemented in the message dispatch table, the return value is
//    the value returned by the message handling function.  Otherwise,
//    the return value is the value returned by the default window procedure.
//
//  COMMENTS:
//    Call the DispMessage() function with the main window's message dispatch
//    information (msdiMain) and the message specific information.
//

LRESULT CALLBACK WndProc(HWND   hwnd,
                         UINT   uMessage,
                         WPARAM wparam,
                         LPARAM lparam)
{
    return DispMessage(&msdiMain, hwnd, uMessage, wparam, lparam);
}


//
//  FUNCTION: MsgCreate(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: Initialized main window
//
//  PARAMETERS:
//
//    hwnd      - Window handle
//    uMessage  - Message number WM_CREATE
//    wparam    - Extra data     (Unused)
//    lparam    - Extra data     (Unused)
//
//  RETURN VALUE:
//
//    Always returns 0 - Message handled
//
//  COMMENTS:
//    Creates NetDDE share, initializes ddeml, and creates
//    to edit controls in main window.
//

LRESULT MsgCreate(HWND hwnd, UINT uMessage, WPARAM wparam, LPARAM lparam)
{
    RECT rc;
    HSZ  hszService;

    g_hwnd = hwnd;
    EnableMenuItem( GetMenu( hwnd ), IDM_CONNECT, MF_GRAYED );
    EnableMenuItem( GetMenu( hwnd ), IDM_DISCONNECT, MF_GRAYED );

    // setup NetDDE Share ( require if acting as a server )
    LoadString(hInst, IDS_SHARENAME, (LPSTR)szShareName, MAX_NDDESHARENAME);
    LoadString(hInst, IDS_TOPIC, (LPSTR)g_szTopic, MAX_TOPICNAME);
    LoadString(hInst, IDS_ITEM, (LPSTR)szItem, MAX_ITEMNAME);

    if ( !SetupNDDEShare(hwnd, (LPSTR)szShareName, (LPSTR)szAppName, (LPSTR)g_szTopic) )
    {
        MessageBox( hwnd, (LPSTR)"Unable to create NetDDE Share.",
                    (LPSTR)szAppName, MB_ICONSTOP | MB_OK );
        return 0;
    }

    // Initialize DDEML conversation
    if (DdeInitialize (&g_idInst, (PFNCALLBACK) DdeCallback,
                       APPCLASS_STANDARD | CBF_FAIL_EXECUTES
                       | CBF_FAIL_REQUESTS | CBF_FAIL_SELFCONNECTIONS, 0L))
    {
        MessageBox (hwnd, (LPSTR)"Unable to initialize DDEML.",
                      szAppName, MB_ICONSTOP | MB_OK) ;
        return 0;
    }

    hszService = DdeCreateStringHandle (g_idInst, (LPSTR)szAppName, 0) ;
    DdeNameService (g_idInst, hszService, 0, DNS_REGISTER) ;
    DdeFreeStringHandle (g_idInst, hszService) ;

    // Initialize user interface
    GetClientRect( hwnd, &rc );

    hwndSend  = CreateWindow("EDIT",
                             NULL,
                             WS_CHILD | WS_VISIBLE | WS_VSCROLL | WS_BORDER
                              | ES_LEFT | ES_MULTILINE | ES_WANTRETURN,
                             0,
                             0,
                             rc.right,
                             rc.bottom/2,
                             hwnd,
                             (HMENU) IDE_SEND,
                             hInst,
                             NULL );

    hwndRecv  = CreateWindow("EDIT",
                             NULL,
                             WS_CHILD | WS_VISIBLE | WS_VSCROLL | WS_BORDER
                              | ES_LEFT | ES_MULTILINE | ES_READONLY | ES_WANTRETURN,
                             0,
                             rc.bottom/2,
                             rc.right,
                             rc.bottom/2,
                             hwnd,
                             (HMENU) IDE_RECV,
                             hInst,
                             NULL );

    EnableMenuItem( GetMenu( hwnd ), IDM_CONNECT, MF_ENABLED );

    // initialize variable for last contents of send data
    MallocMem( (LPVOID *) &pszLast, 1 );
    *pszLast = '\0';

    return 0;
}


//
//  FUNCTION: MsgSize(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: Resize edit controls to client area size
//
//  PARAMETERS:
//
//    hwnd      - Window handle
//    uMessage  - Message number WM_SIZE
//    wparam    - Extra data     (Unused)
//    lparam    - Extra data     (Unused)
//
//  RETURN VALUE:
//
//    Always returns 0 - Message handled
//
//  COMMENTS:
//    
//
LRESULT MsgSize(HWND hwnd, UINT uMessage, WPARAM wparam, LPARAM lparam)
{
    MoveWindow(hwndSend, 0, 0, LOWORD(lparam), HIWORD(lparam)/2, TRUE);
    MoveWindow(hwndRecv, 0, HIWORD(lparam)/2, LOWORD(lparam), HIWORD(lparam)/2, TRUE);
    return 0;
}


//
//  FUNCTION: MsgConnect(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: Message indicating that a connection has occured
//
//  PARAMETERS:
//    hwnd      - Window handle
//    uMessage  - Message number WM_CONNECT
//    wparam    - Extra data     (Unused)
//    lparam    - HCONV for DDEML conversation
//
//  RETURN VALUE:
//
//    Always returns 0 - Message handled
//
//  COMMENTS:
//
//
LRESULT MsgConnect(HWND hwnd, UINT uMessage, WPARAM wparam, LPARAM lparam)
{
    char szBuffer[128];
    DWORD cbBytes = 128;
    HSZ hszItem;

    if ( g_fState == STATE_PENDING ) // only clients pend
    {
        // get the name of the local ( client ) computer to poke to server
#ifdef WIN16
        NDdeGetNodeName( (LPSTR)szBuffer, cbBytes );
#else
        GetComputerName( (LPSTR)szBuffer, &cbBytes );
#endif
        // -1 signals computer name
        STR_DWORD((LPSTR)szBuffer) = (DWORD) -1;

        hszItem = DdeCreateStringHandle (g_idInst, (LPSTR)szItem, 0) ;
        DdeClientTransaction( (LPSTR)szBuffer, lstrlen((LPSTR)szBuffer)+1+sizeof(DWORD),
                              hconv, hszItem, CF_TEXT, XTYP_POKE,
                              (DWORD) TIMEOUT_ASYNC, NULL);

        DdeFreeStringHandle (g_idInst, hszItem) ;

        g_fState = STATE_CLIENT;

    }

    // save conversation handle for disconnect
    hconv = (HCONV) lparam;

    // set user interface
    if ( *g_szServer != '\0' )
    {
        wsprintf( (LPSTR)szBuffer, (LPSTR)"%s - Connected to %s", (LPSTR)SZDESCRIPTION, (LPSTR)g_szServer );
        SetWindowText( hwnd, (LPSTR)szBuffer );
    }

    *pszLast = '\0';

    SetWindowText( hwndRecv, (LPSTR)"" );
    SetWindowText( hwndSend, (LPSTR)"" );
    EnableMenuItem( GetMenu( g_hwnd ), IDM_DISCONNECT, MF_ENABLED );
    EnableMenuItem( GetMenu( g_hwnd ), IDM_CONNECT, MF_GRAYED );

    SetFocus( hwndSend );

    return 0;
}


//
//  FUNCTION: MsgDisconnect(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: Message indicating that the conversation have been disconnected
//
//  PARAMETERS:
//
//    hwnd      - Window handle
//    uMessage  - Message number WM_DISCONNECT
//    wparam    - Extra data     (Unused)
//    lparam    - Extra data     (Unused)
//
//  RETURN VALUE:
//
//    Always returns 0 - Message handled
//
//  COMMENTS:
//
//
LRESULT MsgDisconnect(HWND hwnd, UINT uMessage, WPARAM wparam, LPARAM lparam)
{
    char szBuffer[128];
                       
    wsprintf( (LPSTR)szBuffer, (LPSTR)"Conversation with %s disconnected", (LPSTR) g_szServer );                       
    g_fState = STATE_UNCONN;
    *g_szServer = '\0';
    SetWindowText(hwnd, SZDESCRIPTION );

    // reset menus
    EnableMenuItem(GetMenu(g_hwnd), IDM_CONNECT, MF_ENABLED );
    EnableMenuItem(GetMenu(g_hwnd), IDM_DISCONNECT, MF_GRAYED );
     
    MessageBox( hwnd, szBuffer, szAppName, MB_OK ); 
    return 0;
}



//
//  FUNCTION: MsgReceiveText(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: Updates received edit control with received text
//
//  PARAMETERS:
//    hwnd      - Window handle
//    uMessage  - Message number WM_RECEIVE_TEXT
//    wparam    - Size in bytes of data
//    lparam    - Pointer to data 
//
//  RETURN VALUE:
//
//    Always returns 0 - Message handled
//
//  COMMENTS:
//    Data has the following format
//    (LPSTR)String\0(DWORD)InsertionPrint
//
LRESULT MsgReceiveText(HWND hwnd, UINT uMessage, WPARAM wparam, LPARAM lparam)
{
    DWORD ndx = GetWindowTextLength( hwndRecv );

#ifdef WIN16
    SendMessage( hwndRecv, EM_SETSEL, 1, MAKELONG((WORD)STR_DWORD((LPSTR)lparam), ndx) );
#else
    SendMessage( hwndRecv, EM_SETSEL, (INT)STR_DWORD((LPSTR)lparam), (LPARAM)-1 );
#endif

    SendMessage( hwndRecv, EM_REPLACESEL, 0, lparam );

    FreeMem( (LPVOID *) &lparam );
    return 0;
}



//
//  FUNCTION: MsgCommand(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: Handle the WM_COMMAND messages for the main window.
//
//  PARAMETERS:
//    hwnd     - window handle
//    uMessage - WM_COMMAND 
//    GET_WM_COMMAND_ID(wparam, lparam)   - Command identifier
//    GET_WM_COMMAND_HWND(wparam, lparam) - Control handle
//
//  RETURN VALUE:
//    The return value depends on the message number.  If the message
//    is implemented in the message dispatch table, the return value is
//    the value returned by the message handling function.  Otherwise,
//    the return value is the value returned by the default window procedure.
//
//  COMMENTS:
//    Call the DispCommand() function with the main window's command dispatch
//    information (cmdiMain) and the command specific information.
//

LRESULT MsgCommand(HWND hwnd, UINT uMessage, WPARAM wparam, LPARAM lparam)
{
    return DispCommand(&cmdiMain, hwnd, wparam, lparam);
}


//
//  FUNCTION: MsgDestroy(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: Frees data and calls PostQuitMessage().
//
//  PARAMETERS:
//
//    hwnd      - Window handle  (Unused)
//    uMessage  - Message number (Unused)
//    wparam    - Extra data     (Unused)
//    lparam    - Extra data     (Unused)
//
//  RETURN VALUE:
//
//    Always returns 0 - Message handled
//
//  COMMENTS:
//
//
LRESULT MsgDestroy(HWND hwnd, UINT uMessage, WPARAM wparam, LPARAM lparam)
{
    HSZ hszService = DdeCreateStringHandle (g_idInst, (LPSTR)szAppName, 0);
    DdeNameService (g_idInst, hszService, 0, DNS_UNREGISTER);
    DdeFreeStringHandle (g_idInst, hszService);

    if ( pszLast )
        FreeMem( (LPVOID *) &pszLast );

    PostQuitMessage(0);
    return 0;
}



//
//  FUNCTION: CmdConnectDlg(HWND, WORD, WORD, HWND)
//
//  PURPOSE: Displays the "ConnectDlg" dialog box and initiates connection
//
//  PARAMETERS:
//    hwnd      - Window handle
//    wCommand  - IDM_CONNECT 
//    wNotify   - Notification number (unused)
//    hwndCtrl  - NULL (unused)
//
//  RETURN VALUE:
//
//    Always returns 0 - Message handled
//
//  COMMENTS:
//    Only clients will encounter this message
//
LRESULT CmdConnectDlg(HWND hwnd, WORD wCommand, WORD wNotify, HWND hwndCtrl)
{

    char szBuffer[128];
    char szService[MAX_COMPUTERNAME_LENGTH+9];  // extra for \\%s\NDDE$
    HSZ hszService;
    HSZ hszShare;
    HSZ hszItem;

    *g_szServer = '\0';

    if ( DialogBox(hInst, "ConnectDlgBox", hwnd, (DLGPROC)ConnectDlg) )
    {
        wsprintf( (LPSTR) szService, "%s\\NDDE$", (LPSTR) g_szServer );

        hszService = DdeCreateStringHandle (g_idInst, (LPSTR) szService, 0) ;
        hszShare   = DdeCreateStringHandle (g_idInst, (LPSTR) szShareName,   0) ;

        hconv = DdeConnect (g_idInst, hszService, hszShare, NULL) ;

        DdeFreeStringHandle (g_idInst, hszService) ;
        DdeFreeStringHandle (g_idInst, hszShare) ;

        if (!hconv)
        {
            MessageBox (hwnd, (LPSTR)"Cannot connect to server.",
                        (LPSTR)szAppName, MB_ICONEXCLAMATION | MB_OK) ;

            return 0 ;
        }

        // Request notification of item
        hszItem = DdeCreateStringHandle (g_idInst, szItem, 0) ;

        DdeClientTransaction (NULL, 0, hconv, hszItem, CF_TEXT,
                              XTYP_ADVSTART | XTYPF_ACKREQ,
                              (DWORD )TIMEOUT_ASYNC, NULL) ;

        DdeFreeStringHandle (g_idInst, hszItem) ;

        g_fState = STATE_PENDING;        
        wsprintf( (LPSTR)szBuffer, (LPSTR)"%s - Connecting to %s", (LPSTR)SZDESCRIPTION, (LPSTR)g_szServer );
        SetWindowText( hwnd, (LPSTR)szBuffer );

        EnableMenuItem( GetMenu( hwnd ), IDM_DISCONNECT, MF_GRAYED );
        EnableMenuItem( GetMenu( hwnd ), IDM_CONNECT, MF_GRAYED );
    }

    return 0;
}


//
//  FUNCTION: CmdDisconnect(HWND, WORD, WORD, HWND)
//
//  PURPOSE: Issues a disconnect for the current converstaion
//
//  PARAMETERS:
//    hwnd      - Window handle
//    wCommand  - IDM_DISCONNECT
//    wNotify   - Notification number (unused)
//    hwndCtrl  - NULL (unused)
//
//  RETURN VALUE:
//
//    Always returns 0 - Message handled
//
//  COMMENTS:
//    To process the IDM_ENTRY message, call DialogBox() to display the
//    phbkdlg dialog box.
//
LRESULT CmdDisconnect(HWND hwnd, WORD wCommand, WORD wNotify, HWND hwndCtrl)
{
    HSZ hszItem;

    if ( g_fState == STATE_CLIENT )
    {
        hszItem = DdeCreateStringHandle (g_idInst, (LPSTR)szItem, 0);

        DdeClientTransaction (NULL, 0, hconv, hszItem, CF_TEXT,
                              XTYP_ADVSTOP, (DWORD)TIMEOUT_ASYNC, NULL) ;

        DdeFreeStringHandle (g_idInst, hszItem) ;
    }

    // Disconnect the conversation
    DdeDisconnect (hconv) ;

    PostMessage( hwnd, WM_DISCONNECT, 0, 0 );

    return 0;
}


//
//  FUNCTION: CmdSendNotify(HWND, WORD, WORD, HWND)
//
//  PURPOSE: Handles notification messages for SEND edit control
//
//  PARAMETERS:
//    hwnd      - Window handle
//    wCommand  - IDM_DISCONNECT
//    wNotify   - Notification number (unused)
//    hwndCtrl  - NULL (unused)
//
//  RETURN VALUE:
//
//    Always returns 0 - Message handled
//
//  COMMENTS:
//    To process the IDM_ENTRY message, call DialogBox() to display the
//    phbkdlg dialog box.
//
LRESULT CmdSendNotify(HWND hwnd, WORD wCommand, WORD wNotify, HWND hwndCtrl)
{
    HSZ hszTopic;
    HSZ hszItem;
    HDDEDATA hdata;

    if ( wNotify == EN_CHANGE )
    {
        hszTopic = DdeCreateStringHandle (g_idInst, (LPSTR)g_szTopic, 0);
        hszItem = DdeCreateStringHandle (g_idInst, (LPSTR)szItem, 0);

        if ( g_fState == STATE_SERVER )
            DdePostAdvise (g_idInst, hszTopic, hszItem) ;
        else
        {
            if ( (hdata = GetDataToTrans()) != 0 )
            {
                DdeClientTransaction( (LPBYTE) hdata, (DWORD) -1, hconv,
                                      hszItem, CF_TEXT, XTYP_POKE,
                                      (DWORD)TIMEOUT_ASYNC, NULL);
            }
        }

        DdeFreeStringHandle (g_idInst, hszItem) ;
        DdeFreeStringHandle (g_idInst, hszTopic) ;

    }
    return 0;
}

//
//  FUNCTION: CmdExit(HWND, WORD, WORD, HWND)
//
//  PURPOSE: Exit the application.
//
//  PARAMETERS:
//    hwnd     - The window.
//    wCommand - IDM_EXIT
//    wNotify  - Notification number (unused)
//    hwndCtrl - NULL (unused)
//
//  RETURN VALUE:
//    Always returns 0 - command handled.
//
//  COMMENTS:
//
//
LRESULT CmdExit(HWND hwnd, WORD wCommand, WORD wNotify, HWND hwndCtrl)
{
    DestroyWindow(hwnd);
    return 0;
}


//
//  FUNCTION: GetDataToTrans( VOID )
//
//  PURPOSE: Determines what data to needs to be transmitted
//
//  PARAMETERS:
//    none
//
//  RETURN VALUE:
//    Always returns 0 - command handled.
//
//  COMMENTS:
//    Data has the following format
//    (LPSTR)String\0(DWORD)InsertionPrint
//
HDDEDATA GetDataToTrans( VOID )
{
    HDDEDATA hdata;
    HSZ hszItem;
    LPSTR pszNew;
    LPSTR pszN;
    LPSTR pszL;
    DWORD cbBytes = GetWindowTextLength( hwndSend );

    if ( MallocMem( (LPVOID *) &pszNew, cbBytes+1+sizeof(DWORD) ) == ERROR_SUCCESS )
    {
        GetWindowText( hwndSend, pszNew, (int)cbBytes+1 );

        pszN = pszNew;
        pszL = pszLast;

        // increment pointed while ...
        // 1) not at end of Last string
        // 2) not at end of New string
        // 3) New char is equal to Last char

        // compute the insertion point of changed text
        if( !(*pszL) || !(*pszN) )
            STR_DWORD(pszNew) = 0;
        else
        {
            while ( *pszN++ == *pszL++ ) ;
            pszN--;
            STR_DWORD(pszNew) = (DWORD) ( pszL - 1 - pszLast );
        }

        hszItem = DdeCreateStringHandle (g_idInst, szItem, 0);
        hdata = DdeCreateDataHandle (g_idInst, pszN, cbBytes+1+sizeof(DWORD)-STR_DWORD(pszNew),
                                     0, hszItem, CF_TEXT, 0);
        DdeFreeStringHandle (g_idInst, hszItem) ;

        FreeMem( &pszLast );
        pszLast = pszNew;
    }

    return hdata;
}
