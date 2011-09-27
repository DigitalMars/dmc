
//-----------------------------------------------------------------------------
// This is a part of the Microsoft Source Code Samples. 
// Copyright (C) 1993 Microsoft Corporation.
// All rights reserved. 
//  
// This source code is only intended as a supplement to 
// Microsoft Development Tools and/or WinHelp documentation.
// See these sources for detailed information regarding the 
// Microsoft samples programs.
//-----------------------------------------------------------------------------

/****************************************************************************\
*  wsock.c -- sample program demonstrating Windows Sockets APIs.
*
*  Demonstrates basic sockets programming with the Windows Sockets API.
*  Allows two occurances of the application to connect.  Also, displays
*  information about a host computer.
*
****************************************************************************/

#include <windows.h>       /* required for all Windows applications */
#include <winsock.h>
#include <stdio.h>         /* for sprintf                           */
#include <string.h>        /* for strlen                            */
#include <memory.h>
#include <process.h>       /* for _beginthread                      */
#include "wsock.h"         /* specific to this program              */

HANDLE hInst;              /* current instance                      */

SOCKET sock;
u_short portno;            /* Which tcp port are we going to use?   */

char szBuff[ 80 ];         /* Temp buffer - used to pass strings    */
                           /* to and from dialog boxes, etc         */

#define MAX_PENDING_CONNECTS 4  /* The backlog allowed for listen() */
#define NO_FLAGS_SET         0  /* Used with recv()/send()          */
#define MY_MSG_LENGTH       80  /* msg buffer sent back and forth   */

/****************************************************************************
*
*    FUNCTION: WinMain(HANDLE, HANDLE, LPSTR, int)
*
*    PURPOSE: calls initialization function, processes message loop
*
*\***************************************************************************/

WINAPI WinMain(
    HINSTANCE hInstance,
    HINSTANCE hPrevInstance,
    LPSTR lpCmdLine,
    int nCmdShow
    )
{

    MSG msg;

    UNREFERENCED_PARAMETER( lpCmdLine );

    if (!hPrevInstance)                  /* Other instances of app running? */
        if (!InitApplication(hInstance)) /* Initialize shared things        */
            return (FALSE);              /* Exits if unable to initialize   */

    /*
    *   Perform initializations that apply to a specific instance
    */
    if (!InitInstance(hInstance, nCmdShow))
        return (FALSE);

    /*
    *   Acquire and dispatch messages until a WM_QUIT message is received.
    */
    while (GetMessage(&msg,        /* message structure                      */
            NULL,                  /* handle of window receiving the message */
            0,             /* lowest message to examine              */
            0))            /* highest message to examine             */
        {
        TranslateMessage(&msg);    /* Translates virtual key codes           */
        DispatchMessage(&msg);     /* Dispatches message to window           */
   }
    return (msg.wParam);           /* Returns the value from PostQuitMessage */
}


/****************************************************************************
*
*    FUNCTION: InitApplication(HANDLE)
*
*    PURPOSE: Initializes window data and registers window class
*
*\***************************************************************************/

BOOL InitApplication(HANDLE hInstance)       /* current instance             */
{
    WNDCLASS  wc;

    /* Fill in window class structure with parameters that describe the       */
    /* main window.                                                           */

    wc.style = 0;                    /* Class style(s).                    */
    wc.lpfnWndProc = (WNDPROC)MainWndProc;       /* Function to retrieve messages for  */
                                        /* windows of this class.             */
    wc.cbClsExtra = 0;                  /* No per-class extra data.           */
    wc.cbWndExtra = 0;                  /* No per-window extra data.          */
    wc.hIcon = LoadIcon (hInstance, "wsockicon"); /* Icon name from .RC        */
    wc.hInstance = hInstance;          /* Application that owns the class.   */
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = GetStockObject(WHITE_BRUSH);
    wc.lpszMenuName =  "WSockMenu";   /* Name of menu resource in .RC file. */
    wc.lpszClassName = "WSockWClass"; /* Name used in call to CreateWindow. */

    /* Register the window class and return success/failure code. */

    return (RegisterClass(&wc));

}


/****************************************************************************\
*
*    FUNCTION:  InitInstance(HANDLE, int)
*
*    PURPOSE:  Saves instance handle and creates main window
*
*\***************************************************************************/

BOOL InitInstance(
    HANDLE          hInstance,          /* Current instance identifier.       */
    int             nCmdShow)           /* Param for first ShowWindow() call. */
{
    HWND            hWnd;               /* Main window handle.                */

    /* Save the instance handle in static variable, which will be used in  */
    /* many subsequence calls from this application to Windows.            */

    hInst = hInstance;

    /* Create a main window for this application instance.  */

    hWnd = CreateWindow(
        "WSockWClass",                  /* See RegisterClass() call.          */
        "Windows Sockets Sample Application",   /* Text for window title bar.         */
        WS_OVERLAPPEDWINDOW,            /* Window style.                      */
        CW_USEDEFAULT,                  /* Default horizontal position.       */
        CW_USEDEFAULT,                  /* Default vertical position.         */
        CW_USEDEFAULT,                  /* Default width.                     */
        CW_USEDEFAULT,                  /* Default height.                    */
        NULL,                           /* Overlapped windows have no parent. */
        NULL,                           /* Use the window class menu.         */
        hInstance,                      /* This instance owns this window.    */
        NULL                            /* Pointer not needed.                */
    );

    /* If window could not be created, return "failure" */

    if (!hWnd)
        return (FALSE);

    /* Make the window visible; update its client area; and return "success" */

    ShowWindow(hWnd, nCmdShow);  /* Show the window                        */
    UpdateWindow(hWnd);          /* Sends WM_PAINT message                 */
    return (TRUE);               /* Returns the value from PostQuitMessage */

}

/****************************************************************************\
*
*    FUNCTION: AcceptThreadProc(PTHREADPACK tp)
*
*    PURPOSE:  Use blocking accept() calls and display a message box when
*              a connection is made.
*
*\***************************************************************************/

void AcceptThreadProc( PTHREADPACK ptp )
{
   SOCKADDR_IN acc_sin;    /* Accept socket address - internet style */
   int acc_sin_len;        /* Accept socket address length */
   int status;
   char szMsg[ MY_MSG_LENGTH ];


   acc_sin_len = sizeof(acc_sin);

   wsprintf( szBuff, "thread #%d created.", ptp->nThread);
   MessageBox(ptp->hWnd, szBuff, "FYI", MB_OK);

   sock = accept( sock,(struct sockaddr FAR *) &acc_sin,
            (int FAR *) &acc_sin_len );

   if (sock < 0) {
      sprintf(szBuff, "%d is the error", WSAGetLastError());

      MessageBox(ptp->hWnd, szBuff, "accept(sock) failed", MB_OK);

   }

   wsprintf( szBuff, "Thread #%d accepted something\n\nCheck for incoming messages?", ptp->nThread);

   /*
   *   Now have a connection --
   *   SetConnectMenus() grays/enables proper menu items
   */
   SetConnectMenus( ptp->hWnd );


   while (1) {

      /*
      *   By default sockets are created in blocking mode.
      *   Just keep reading until process destroyed.
      */
      status = recv( sock, szMsg, MY_MSG_LENGTH, NO_FLAGS_SET );

      if (status == SOCKET_ERROR) {
         wsprintf( szMsg, "Error %d", WSAGetLastError() );
         MessageBox( ptp->hWnd, szMsg, "Error with recv()", MB_OK);
         _endthread();
      }
          szMsg[status] = '\0';  /* NULL-terminate the string */

      if (status)
         MessageBox( ptp->hWnd, szMsg, "From thread", MB_OK);
      else  {
         MessageBox( ptp->hWnd, "Connection broken", "Error", MB_OK);
         _endthread();
      }

   }    /* while (forever) */
}

/****************************************************************************\
*
*    FUNCTION:  FillAddr(HWND, PSOCKADDR_IN, BOOL)
*
*    PURPOSE:  Retrieves the IP address and port number.
*
*    COMMENTS:
*        This function is called in two conditions.
*            1.) When a client is preparing to call connect(), or
*            2.) When a server host is going to call bind(), listen() and
*                accept().
*        In both situations, a SOCKADDR_IN structure is filled.
*        However, different fields are filled depending on the condition.
*
*   ASSUMPTION:
*      szBuff is a global variable that contains the remote host name or NULL
*      if local.
*      bClient determines if this is being called by a client ( will be
*         performing a connect ) or a server ( will be listening )
*
*
*\***************************************************************************/

BOOL FillAddr(
        HWND hWnd,
        PSOCKADDR_IN psin,
        BOOL bClient)
{
   DWORD dwSize;
   PHOSTENT phe;
   PSERVENT pse;
   char szTemp[200];
   int status;


   psin->sin_family = AF_INET;


   /*
   *   If we are setting up for a listen() call (bConnect = FALSE),
   *   fill servent with our address.
   */
   if (bClient) {
      phe = gethostbyname(szBuff);
      if (phe == NULL) {
         sprintf(szTemp, "%d is the error. Make sure '%s' is listed in the hosts file.", WSAGetLastError(), szBuff);

         MessageBox(hWnd, szTemp, "gethostbyname() failed.", MB_OK);
         return FALSE;
      }
      memcpy((char FAR *)&(psin->sin_addr), phe->h_addr,
         phe->h_length);

      }
   else { // server

      /*
      *   Retrieve my ip address.  Assuming the hosts file in
      *   in %systemroot%/system/drivers/etc/hosts contains my computer name.
      */

      dwSize = sizeof(szBuff);
      gethostname(szBuff, dwSize);

      psin->sin_addr.s_addr = INADDR_ANY;
      }


   /*
   *   Retrieve the Port number
   */
   status = DialogBox(hInst,             /* current instance         */
      "TCPPORTNUM",                      /* resource to use          */
      hWnd,                              /* parent handle            */
      GetTcpPort);                       /* instance address         */

   switch(status) {
      case 0:               /* User cancelled request from prev. dialog box */
         return FALSE;

      case 1:               /* actual port number entered */
         psin->sin_port = htons(portno);        /* Convert to network ordering */
         break;

      case 2:               /* service name entereted */
         /*
         *   Find the service name, szBuff, which is a type tcp protocol in
         *   the "services" file.
         */
         pse = getservbyname(szBuff, "tcp");
         if (pse == NULL)  {
            sprintf(szBuff, "%d is the error. Make sure this is a valid TCP service.", WSAGetLastError());
            MessageBox(hWnd, szBuff, "getservbyname(sock) failed", MB_OK);
            return FALSE;
         }
         psin->sin_port = pse->s_port;
         break;

      default:
         return FALSE;
   }
   return TRUE;
}

/****************************************************************************
*
*    FUNCTION: SetConnectMenus( HWND )
*
*    PURPOSE: Gray/Enable the proper menu items after a connection has been
*             established.
*
*\***************************************************************************/

void SetConnectMenus( HWND hWnd )
{
   /*
   *   Disable/enable proper menu items.
   */
   EnableMenuItem(GetMenu( hWnd ), IDM_HOSTNAME, MF_ENABLED );
   EnableMenuItem(GetMenu( hWnd ), IDM_LISTEN, MF_GRAYED );
   EnableMenuItem(GetMenu( hWnd ), IDM_CONNECT, MF_GRAYED );
   EnableMenuItem(GetMenu( hWnd ), IDM_ALISTEN, MF_GRAYED );
   EnableMenuItem(GetMenu( hWnd ), IDM_TLISTEN, MF_GRAYED );
   EnableMenuItem(GetMenu( hWnd ), IDM_CANCEL, MF_GRAYED );
   EnableMenuItem(GetMenu( hWnd ), IDM_SENDTCP, MF_ENABLED );

   /*
   *   Reflect socket connection in title bar.
   */
   SetWindowText( hWnd, "Connected");
}

/****************************************************************************\
*
*    FUNCTION: MainWndProc(HWND, unsigned, WORD, LONG)
*
*    PURPOSE:  Processes main window messages
*
* MESSAGES:
*  WM_CREATE   - call WSAStartUp() and display description message
*  WSA_ACCEPT  - User-defined message used with WSAAsyncSelect().  Sent
*                by the Windows Sockets DLL when a socket connection is
*                pending.
*
*  WM_COMMAND
*  IDM_CONNECT - Connect to a remote host.
*  IDM_LISTEN  - Use the BSD-Style accept().
*  IDM_ALISTEN - Use the Windows Sockets Asynchronous APIs to detect when
*                a connection is made.
*  IDM_CANCEL  - Cancel the Asynchronous call above.
*  IDM_TLISTEN - Uses two threads to accept network connections (using the
*                BSD-Style accept().
*  IDM_HOSTNAME- Display information about a host.
*  IDM_ABOUT   - About box.
*
*  WM_DESTROY  - destroy window and call the WSACleanUp()
*
*\***************************************************************************/

LONG APIENTRY MainWndProc(
        HWND hWnd,                /* window handle                   */
        UINT message,             /* type of message                 */
        UINT wParam,              /* additional information          */
        LONG lParam)              /* additional information          */
{
   int status;             /* Status Code */
   SOCKADDR_IN local_sin;  /* Local socket - internet style */
   SOCKADDR_IN acc_sin;    /* Accept socket address - internet style */
   int acc_sin_len;        /* Accept socket address length */


   switch (message) {
   case WM_CREATE:
   {
      WSADATA WSAData;
      char szTemp[80];

      if ((status = WSAStartup(MAKEWORD(1,1), &WSAData)) == 0) {
         MessageBox( hWnd, WSAData.szDescription, WSAData.szSystemStatus, MB_OK);
      }
      else {
         sprintf(szTemp, "%d is the err", status);
         MessageBox( hWnd, szTemp, "Error", MB_OK);
      }
   }
   break;   /* WM_CREATE */

   /*
   *    Notification if data is waiting on a socket.  This comes
   *    from Windows Sockets (via WSAAsyncSelect()).
   */
   case WSA_READ:
   {
      char szTemp[ MY_MSG_LENGTH ];

      if (WSAGETSELECTEVENT(lParam) == FD_READ) {
         status = recv((SOCKET)wParam, szTemp, MY_MSG_LENGTH, NO_FLAGS_SET );

         if (status) {
            szTemp[ status ] = '\0';
            MessageBox( hWnd, szTemp, "WSA_READ", MB_OK);
         }
         else
            MessageBox( hWnd, "Connection broken", "Error", MB_OK);
      }
      else {    /* FD_CLOSE -- connection dropped */
         MessageBox( hWnd, "Connection lost", "WSA_READ", MB_OK);
         EnableMenuItem(GetMenu( hWnd ), IDM_HOSTNAME, MF_ENABLED);
         EnableMenuItem(GetMenu( hWnd ), IDM_LISTEN, MF_ENABLED);
         EnableMenuItem(GetMenu( hWnd ), IDM_CONNECT, MF_ENABLED);
         EnableMenuItem(GetMenu( hWnd ), IDM_ALISTEN, MF_ENABLED);
         EnableMenuItem(GetMenu( hWnd ), IDM_TLISTEN, MF_ENABLED);
         EnableMenuItem(GetMenu( hWnd ), IDM_CANCEL, MF_GRAYED);
         EnableMenuItem(GetMenu( hWnd ), IDM_SENDTCP, MF_GRAYED);
      }


   }
   break;       /* WSA_READ*/

   case WSA_ACCEPT: /* Notification if a socket connection is pending. */
      /*
      *   Disable/enable proper menu items.
      */
      EnableMenuItem(GetMenu( hWnd ), IDM_HOSTNAME, MF_ENABLED);
      EnableMenuItem(GetMenu( hWnd ), IDM_LISTEN, MF_ENABLED);
      EnableMenuItem(GetMenu( hWnd ), IDM_CONNECT, MF_ENABLED);
      EnableMenuItem(GetMenu( hWnd ), IDM_ALISTEN, MF_ENABLED);
      EnableMenuItem(GetMenu( hWnd ), IDM_TLISTEN, MF_ENABLED);
      EnableMenuItem(GetMenu( hWnd ), IDM_CANCEL, MF_GRAYED);

      if (WSAGETSELECTERROR( lParam ) == 0) {   /* Success */

         /*
         *   Accept the incoming connection.
         */
         acc_sin_len = sizeof( acc_sin );
         sock = accept( sock,(struct sockaddr FAR *) &acc_sin,
            (int FAR *) &acc_sin_len );

         if (sock < 0) {
            sprintf(szBuff, "%d is the error", WSAGetLastError());

            MessageBox(hWnd, szBuff, "accept(sock) failed", MB_OK);
            break;
         }

         MessageBox(hWnd, "accept()", "Accepted a connection!", MB_OK);

         /*
         *   Now have a connection --
         *   SetConnectMenus() grays/enables proper menu items
         */
         SetConnectMenus( hWnd );

         /*
         *   Send main window a WSA_READ when either data is pending on
         *   the socket (FD_READ) or the connection is closed (FD_CLOSE)
         */
         if ((status = WSAAsyncSelect( sock, hWnd, WSA_READ, FD_READ | FD_CLOSE )) > 0) {
            wsprintf(szBuff, "%d (0x%x)", status, status);
            MessageBox( hWnd, "Error on WSAAsyncSelect()", szBuff, MB_OK);
            closesocket( sock );
         }
      }
      else {
         MessageBox(hWnd, "accept()", "Error occured!", MB_OK);

         /*
         *   Cancel any further notifications.
         */
         WSAAsyncSelect( sock, hWnd, 0, 0);
         SetWindowText( hWnd, "Async Listen call canceled");
      }
      break;   /* WSA_ACCEPT */



   case WM_COMMAND:        /* message: command from application menu */
      switch(LOWORD(wParam)) {
      case IDM_CONNECT: /* Client - connect to remote host */
      {
         /*

         When a network client wants to connect to a server,
         it must have:
            1.) a TCP port number (gotten via getservbyname())
            and
            2.) an IP address of the remote host (gotten via gethostbyname()).

         The following summarizes the steps used to connect.
         Make a dialog box (HostName)
         Get the name of the remote host computer in which
          to connect from the user (store string in "szBuff" global var)
       * Check to see if the hosts file knows the computer (gethostbyname)
       * Get the host information (hostent structure filled)
       * Fill in the address of the remote host into the servent structure (memcpy)
       * Make a dialog box (TCPPORTNUM)
       * Get the NAME of the port to connect to on the remote host from the
         user.
       * Get the port number (getservbyname)
       * Fill in the port number of the servent structure
         Establish a connection (connect)

         The * prefixed steps are done in the FillAddr() procedure.


         */
         SOCKADDR_IN dest_sin;  /* DESTination Socket INternet */



         /* Get the name of the remote host. Store the string in szBuff. */

         status = DialogBox(hInst,
            "HOSTNAME",
            hWnd,
            GetHostName);

         if (!status)   /* User cancelled request from prev. dialog box */
            break;

         sock = socket( AF_INET, SOCK_STREAM, 0);
         if (sock == INVALID_SOCKET) {
            MessageBox(hWnd, "socket() failed", "Error", MB_OK);
            break;
         }

         /*
         *    Retrieve the IP address and TCP Port number
         *    Global variable szBuff contains the remote host name.
         */
         if (!FillAddr( hWnd, &dest_sin, TRUE)) {
            closesocket( sock );
            break;
         }


         if (connect( sock, (PSOCKADDR) &dest_sin, sizeof( dest_sin)) < 0) {
            closesocket( sock );
            MessageBox(hWnd, "connect() failed", "Error", MB_OK);
            break;
         }
         MessageBox(hWnd, "connect() worked!", "Success!", MB_OK);

         /*
         *   Now have a connection --
         *   SetConnectMenus() grays/enables proper menu items
         */
         SetConnectMenus( hWnd );

         /*
         *   Send main window a WSA_READ when either data is pending on
         *   the socket (FD_READ) or the connection is closed (FD_CLOSE)
         */
         if ((status = WSAAsyncSelect( sock, hWnd, WSA_READ, FD_READ | FD_CLOSE )) > 0) {
            wsprintf(szBuff, "%d (0x%x)");
            MessageBox( hWnd, "Error on WSAAsyncSelect()", szBuff, MB_OK);
            closesocket( sock );
         }

      }
      break;   /* IDM_CONNECT */

      case IDM_LISTEN:
      {
         sock = socket( AF_INET, SOCK_STREAM, 0);
         if (sock == INVALID_SOCKET) {
            MessageBox(hWnd, "socket() failed", "Error", MB_OK);
            closesocket(sock);
            break;
         }

         /*
         *   Retrieve the IP address and TCP Port number
         */

         if (!FillAddr(hWnd, &local_sin, FALSE ))
            break;

         /*
         *   Disable/enable proper menu items.
         */
         EnableMenuItem(GetMenu( hWnd ), IDM_HOSTNAME, MF_GRAYED);
         EnableMenuItem(GetMenu( hWnd ), IDM_LISTEN, MF_GRAYED);
         EnableMenuItem(GetMenu( hWnd ), IDM_ALISTEN, MF_GRAYED);
         EnableMenuItem(GetMenu( hWnd ), IDM_TLISTEN, MF_GRAYED);
         EnableMenuItem(GetMenu( hWnd ), IDM_CONNECT, MF_GRAYED);

         SetWindowText( hWnd, "Waiting for connection..");


         /*
         *   Associate an address with a socket. (bind)
         */
         if (bind( sock, (struct sockaddr FAR *) &local_sin, sizeof(local_sin)) == SOCKET_ERROR) {
            sprintf(szBuff, "%d is the error", WSAGetLastError());

            MessageBox(hWnd, szBuff, "bind(sock) failed", MB_OK);
            break;
         }

         if (listen( sock, MAX_PENDING_CONNECTS ) < 0) {
            sprintf(szBuff, "%d is the error", WSAGetLastError());

            MessageBox(hWnd, szBuff, "listen(sock) failed", MB_OK);
            break;
         }

         acc_sin_len = sizeof(acc_sin);


         sock = accept( sock,(struct sockaddr FAR *) &acc_sin,
            (int FAR *) &acc_sin_len );
         if (sock < 0) {
            sprintf(szBuff, "%d is the error", WSAGetLastError());

            MessageBox(hWnd, szBuff, "accept(sock) failed", MB_OK);
            break;
         }

         MessageBox(hWnd, "accept()", "Accepted a connection!", MB_OK);

         /*
         *   Now have a connection --
         *   SetConnectMenus() grays/enables proper menu items
         */
         SetConnectMenus( hWnd );

         /*
         *   Send main window a WSA_READ when either data is pending on
         *   the socket (FD_READ) or the connection is closed (FD_CLOSE)
         */
         if ((status = WSAAsyncSelect( sock, hWnd, WSA_READ, FD_READ | FD_CLOSE )) > 0) {
            wsprintf(szBuff, "%d (0x%x)");
            MessageBox( hWnd, "Error on WSAAsyncSelect()", szBuff, MB_OK);
            closesocket( sock );
         }

      }
      break;   /* IDM_LISTEN */

      /*
      *   Asynchronous Listen - Using WSA extensions.
      */
      case IDM_ALISTEN:
      {

         sock = socket( AF_INET, SOCK_STREAM, 0);
         if (sock == INVALID_SOCKET) {
            MessageBox(hWnd, "socket() failed", "Error", MB_OK);
            break;
         }
         /*
         *   Retrieve the IP address and TCP Port number
         */

         if (!FillAddr( hWnd, &local_sin, FALSE)) {
            closesocket( sock );
            break;
         }

         /*
         *   Disable/enable proper menu items.
         */
         EnableMenuItem(GetMenu( hWnd ), IDM_HOSTNAME, MF_GRAYED);
         EnableMenuItem(GetMenu( hWnd ), IDM_LISTEN, MF_GRAYED);
         EnableMenuItem(GetMenu( hWnd ), IDM_CONNECT, MF_GRAYED);
         EnableMenuItem(GetMenu( hWnd ), IDM_ALISTEN, MF_GRAYED);
         EnableMenuItem(GetMenu( hWnd ), IDM_TLISTEN, MF_GRAYED);
         EnableMenuItem(GetMenu( hWnd ), IDM_CANCEL, MF_ENABLED);

         SetWindowText( hWnd, "Waiting for connection.. (Async)");


         /*
         *   Associate an address with a socket. (bind)
         */
         if (bind( sock, (struct sockaddr FAR *) &local_sin, sizeof(local_sin)) == SOCKET_ERROR) {
            sprintf(szBuff, "%d is the error", WSAGetLastError());

            MessageBox(hWnd, szBuff, "bind(sock) failed", MB_OK);
            closesocket( sock );
            break;
         }

         if (listen( sock, MAX_PENDING_CONNECTS ) < 0) {
            sprintf(szBuff, "%d is the error", WSAGetLastError());

            MessageBox(hWnd, szBuff, "listen(sock) failed", MB_OK);
            break;
         }

         /*
         *   Send window a WSA_ACCEPT when something is trying to connect.
         */
         if ((status = WSAAsyncSelect( sock, hWnd, WSA_ACCEPT, FD_ACCEPT)) > 0) {
            wsprintf( szBuff, "%d (0x%x)");
            MessageBox( hWnd, "Error on WSAAsyncSelect()", szBuff, MB_OK);
            SetWindowText( hWnd, "Async listen cancelled");
            closesocket( sock );
         }

      }
      break;   /* IDM_ALISTEN */


      /*
      *   Cancel an asynchronous call.
      */
      case IDM_CANCEL:
         WSAAsyncSelect( sock, hWnd, 0, 0);
         SetWindowText( hWnd, "Async Listen cancelled..");

         /*
         *   Disable/enable proper menu items.
         */
         EnableMenuItem(GetMenu( hWnd ), IDM_HOSTNAME, MF_ENABLED);
         EnableMenuItem(GetMenu( hWnd ), IDM_LISTEN, MF_ENABLED);
         EnableMenuItem(GetMenu( hWnd ), IDM_CONNECT, MF_ENABLED);
         EnableMenuItem(GetMenu( hWnd ), IDM_ALISTEN, MF_ENABLED);
         EnableMenuItem(GetMenu( hWnd ), IDM_TLISTEN, MF_ENABLED);
         EnableMenuItem(GetMenu( hWnd ), IDM_CANCEL, MF_GRAYED);
         EnableMenuItem(GetMenu( hWnd ), IDM_SENDTCP, MF_GRAYED);

         break;   /* IDM_CANCEL */

      /*
      * Listen in the main thread -- spawn and accept two network
      *  connections inside two threads.
      */
      case IDM_TLISTEN:
      {
         static THREADPACK tp;

         sock = socket( AF_INET, SOCK_STREAM, 0);
         if (sock == INVALID_SOCKET) {
            MessageBox(hWnd, "socket() failed", "Error", MB_OK);
            closesocket(sock);
            break;
         }

         /*
         *   Retrieve the IP address and TCP Port number
         */

         if (!FillAddr(hWnd, &local_sin, FALSE ))
            break;

         /*
         *   Disable/enable proper menu items.
         */
         EnableMenuItem(GetMenu( hWnd ), IDM_HOSTNAME, MF_GRAYED);
         EnableMenuItem(GetMenu( hWnd ), IDM_LISTEN, MF_GRAYED);
         EnableMenuItem(GetMenu( hWnd ), IDM_ALISTEN, MF_GRAYED);
         EnableMenuItem(GetMenu( hWnd ), IDM_TLISTEN, MF_GRAYED);
         EnableMenuItem(GetMenu( hWnd ), IDM_CONNECT, MF_GRAYED);
         EnableMenuItem(GetMenu( hWnd ), IDM_SENDTCP, MF_GRAYED);

         SetWindowText( hWnd, "Waiting for connection..");


         /*
         *   Associate an address with a socket. (bind)
         */
         if (bind( sock, (struct sockaddr FAR *) &local_sin, sizeof(local_sin)) == SOCKET_ERROR) {
            sprintf(szBuff, "%d is the error", WSAGetLastError());

            MessageBox(hWnd, szBuff, "bind(sock) failed", MB_OK);
            break;
         }

         if (listen( sock, MAX_PENDING_CONNECTS ) < 0) {
            sprintf(szBuff, "%d is the error", WSAGetLastError());

            MessageBox(hWnd, szBuff, "listen(sock) failed", MB_OK);
            break;
         }

         tp.nThread = 0;
         tp.hWnd = hWnd;

         _beginthread(AcceptThreadProc, 0, &tp);

         }
         break;   /* IDM_TLISTEN */


      /*
      *   Display host information.
      */
      case IDM_HOSTNAME:

         /*
         *  Prompt the user and retrieve the text name of the host.
         */
         status = DialogBox(hInst,
            "HOSTNAME",
            hWnd,
            GetHostName);


         if (status == TRUE) {   /* If user hit "OK" .. */

         /*
         *   Get the host information
         */

         if ((phe = gethostbyname( szBuff )) == NULL) {
            MessageBox(hWnd, "gethostbyname() failed", "Error", MB_OK);
            break;
         }
         else {

         /*
         *   Display the host information ..
         */
            DialogBox(hInst,
               "DISPLAYHOST",
               hWnd,
               DisplayHostEnt);
         }
      }
      break;   /* IDM_HOSTNAME */
      /*
      *   Send a message to (via TCP connection) to remote host.
      */
      case IDM_SENDTCP:
         DialogBox(hInst,                /* current instance         */
            "GetString",                 /* resource to use          */
            hWnd,                        /* parent handle            */
            GetSendString);              /* instance address         */

        /*
        *   Assumption -- The GetString dialog box proc fills the global
        *   string buffer, szBuff, with the desired string to send.
        */
        send(sock, szBuff, strlen(szBuff), NO_FLAGS_SET );

        break;   /* IDM_SENDTCP */



      case IDM_ABOUT:
         DialogBox(hInst,                /* current instance         */
            "AboutBox",                  /* resource to use          */
            hWnd,                        /* parent handle            */
            About);                      /* About() instance address */

         break;   /* IDM_ABOUT */

      default:
         /* Lets Windows process it          */
         return (DefWindowProc(hWnd, message, wParam, lParam));
         break;
      }
   break;


   /*
   *   Clean up.  Takes care of any open socket descriptors.
   */
   case WM_DESTROY:
      WSACleanup();
      PostQuitMessage(0);
      break;

   default:                       /* Passes it on if unproccessed    */
      return (DefWindowProc(hWnd, message, wParam, lParam));

   }
   return (0);

}
