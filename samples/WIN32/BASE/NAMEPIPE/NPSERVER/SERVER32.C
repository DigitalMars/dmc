/******************************************************************************\
*       This is a part of the Microsoft Source Code Samples. 
*       Copyright (C) 1993 Microsoft Corporation.
*       All rights reserved. 
*       This source code is only intended as a supplement to 
*       Microsoft Development Tools and/or WinHelp documentation.
*       See these sources for detailed information regarding the 
*       Microsoft samples programs.
\******************************************************************************/

/*************************************************************************\
*  PROGRAM: server32.c
*
*  PURPOSE:
*
*     To demonstrate the use of named pipes and the overlapped structure.
*     This code serves as the server side of the named pipe instances.
*     For more details on an overview of this codes designs or use, see
*     the README file.  For details on the implementation, see the comments
*     in this code.
*
*
\*************************************************************************/

#define  STRICT
#include <windows.h>
#include "server32.h"

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

                                       // clients[] is a global array of
                                       // structures used to keep track
                                       // of the multiple instances of
                                       // the server side of the named
                                       // pipe.  As a client connects
                                       // to a given instance, a new
                                       // server thread is created and
                                       // added to the array.
WRTHANDLE clients[MAX_PIPE_INSTANCES];
DWORD     clientCount = 0;             // Global count of connected clients.

HWND   hWnd;
HANDLE hInst;


/*************************************************************************\
*
*  FUNCTION: WinMain(HANDLE, HANDLE, LPSTR, int)
*
*  PURPOSE: calls initialization function, processes message loop
*
*  COMMENTS:
*
\*************************************************************************/

int APIENTRY WinMain (HINSTANCE hInstance,
                      HINSTANCE hPrevInstance,
                      LPSTR  lpCmdLine,
                      int    nCmdShow)


{

  MSG  msg;
  WNDCLASS wc;


  UNREFERENCED_PARAMETER( lpCmdLine );
  UNREFERENCED_PARAMETER( hPrevInstance );

  hInst = hInstance;

  wc.style = 0;
  wc.lpfnWndProc = (WNDPROC)MainWndProc;
  wc.cbClsExtra = 0;
  wc.cbWndExtra = 0;
  wc.hInstance = hInstance;
  wc.hIcon = LoadIcon (hInstance, "npserver");
  wc.hCursor = LoadCursor (NULL, IDC_ARROW);
  wc.hbrBackground = GetStockObject (WHITE_BRUSH);
  wc.lpszMenuName = "PipeMenu";
  wc.lpszClassName = "PipeWClass";

  RegisterClass(&wc);

  hWnd = CreateWindow ("PipeWClass",
                       "Server32 Named Pipe Sample",
                       WS_OVERLAPPEDWINDOW,
                       CW_USEDEFAULT,
                       CW_USEDEFAULT,
                       CW_USEDEFAULT,
                       CW_USEDEFAULT,
                       NULL,
                       NULL,
                       hInstance,
                       NULL);


  ShowWindow (hWnd, nCmdShow);

  while (GetMessage (&msg, NULL, 0, 0))
    DispatchMessage (&msg);

  return (msg.wParam);

}

/*************************************************************************\
*
*  FUNCTION:  MainWndProc (HWND, UINT, WPARAM, LPARAM)
*
*  PURPOSE:   To process messages.  To launch client and server threads
*
\*************************************************************************/

LONG CALLBACK MainWndProc (HWND   hwnd,
                           UINT   message,
                           WPARAM wParam,
                           LPARAM lParam)
{

  LONG        lpServerThreadID;
  PAINTSTRUCT paintStruct;
  HDC         hDC;

  switch (message)
      {
        case WM_PAINT:
           // DrawBranch is used to paint the spools and text to the window.
           hDC = BeginPaint (hwnd, &paintStruct);
           DrawBranch (hDC);
           EndPaint (hwnd, &paintStruct);
           return(0);

        case WM_CREATE :
           // Create the first instance of a server side of the pipe.
           CreateThread ((LPSECURITY_ATTRIBUTES)NULL,       // No security.
                         (DWORD)0,                          // Same stack size.
                         (LPTHREAD_START_ROUTINE)ServerProc,// Thread procedure.
                         (LPVOID)&hwnd,                     // Parameter.
                         (DWORD)0,                          // Start immediatly.
                         (LPDWORD)&lpServerThreadID);       // Thread ID.
           return (0);

        case WM_DESTROY :
           PostQuitMessage (0);
           return (0);
       }
    return DefWindowProc (hwnd, message, wParam, lParam);
}



/*************************************************************************\
*
*  PROCEDURE: ServerProc (HWND *hWnd)
*
*  PURPOSE:
*
*    A thread procedure, which creates an instance of the server side of
*    the named pipe, and then blocks waiting for a client to connect.
*    Once the client connects, a global array is updated with the specific
*    clients information, and this procedure is called again
*    to launch another waiting server thread.  After launching the new
*    thread, this thread begins to loop, reading the named pipe.  When
*    a message comes from it's client, it uses TellAll() to broadcast
*    the message to the other clients in the array.
*
*  CALLED BY:
*
*    ServerProc();
*    WinMain();
*
*  CALLS TO:
*
*    TellAll();
*    ServerProc().
*
*  COMMENTS:
*
*    Clients is a global array which hold information on each client
*    connected to the named pipe.  This procedure recieves a buffer.
*    It then steps through this global array, and for each client it
*    writes the buffer.
*
\*************************************************************************/

VOID ServerProc(HWND *hWnd)
 {
   HANDLE hPipe;                       // Pipe handle.
   CHAR   inBuf[IN_BUF_SIZE] = "";     // Input buffer for pipe.
   DWORD  ServerThreadID;              // Used for CreateThread().

   CHAR   errorBuf[LINE_LEN] = "";     // Used for error messages.
   DWORD  bytesRead;                   // Used in ReadFile().
   DWORD  retCode;                     // Used to trap return codes.
   DWORD  clientIndex;                 // Index into global array, for this
                                       // instances client.
   DWORD  lastError;                   // Traps returns from GetLastError().
   BOOL   ExitLoop = FALSE;            // Boolean Flag to exit loop.

   OVERLAPPED OverLapWrt;              // Overlapped structure for writing.
   HANDLE     hEventWrt;               // Event handle for overlapped write.

   OVERLAPPED OverLapRd;               // Overlapped structure for reading.
   HANDLE     hEventRd;                // Event handle for overlapped reads.
   DWORD        bytesTransRd;          // Bytes transferred by overlapped.
   PSECURITY_DESCRIPTOR    pSD;
   SECURITY_ATTRIBUTES     sa;

                                       // create a security NULL security
                                       // descriptor, one that allows anyone
                                       // to write to the pipe... WARNING
                                       // entering NULL as the last attribute
                                       // of the CreateNamedPipe() will
                                       // indicate that you wish all
                                       // clients connecting to it to have
                                       // all of the same security attributes
                                       // as the user that started the
                                       // pipe server.

   pSD = (PSECURITY_DESCRIPTOR) LocalAlloc(LPTR,
               SECURITY_DESCRIPTOR_MIN_LENGTH);

   if (pSD == NULL)
     {
     MessageBox (*hWnd, "Error in LocalAlloc for pSD",
                 "Debug: ServerProc()", MB_OK);
     return;
     }

   if (!InitializeSecurityDescriptor(pSD, SECURITY_DESCRIPTOR_REVISION))
     {
     wsprintf (errorBuf, "Error: InitializeSecurityDescriptor() %d",
               GetLastError());
     MessageBox (*hWnd, errorBuf, "Debug: ServerProc()", MB_OK);
       LocalFree((HLOCAL)pSD);
       return;
     }

                                       // add a NULL disc. ACL to the
                                       // security descriptor.

   if (!SetSecurityDescriptorDacl(pSD, TRUE, (PACL) NULL, FALSE))
     {
     wsprintf (errorBuf, "Error: SetSecurityDescriptorDacl() %d",
                GetLastError());
     MessageBox (*hWnd, errorBuf, "Debug: ServerProc()", MB_OK);
     LocalFree((HLOCAL)pSD);
     return;
     }

   sa.nLength = sizeof(sa);
   sa.lpSecurityDescriptor = pSD;
   sa.bInheritHandle = TRUE;

                                       // Create a local named pipe with
                                       // the name '\\.\PIPE\test'.  The
                                       // '.' signifies local pipe.
   hPipe = CreateNamedPipe ("\\\\.\\PIPE\\test", // Pipe name = 'test'.
               PIPE_ACCESS_DUPLEX                // 2 way pipe.
               | FILE_FLAG_OVERLAPPED,           // Use overlapped structure.
               PIPE_WAIT                         // Wait on messages.
               | PIPE_READMODE_MESSAGE           // Specify message mode pipe.
               | PIPE_TYPE_MESSAGE,
               MAX_PIPE_INSTANCES,               // Maximum instance limit.
               OUT_BUF_SIZE,                     // Buffer sizes.
               IN_BUF_SIZE,
               TIME_OUT,                         // Specify time out.
               &sa);                             // Security attributes.

                                       // Check Errors.
    if ((DWORD)hPipe == 0xFFFFFFFF)
     {

     retCode = GetLastError();         // Report any error, it should always
     wsprintf (errorBuf,               // succeed.
               "Error return code from CreateNamedPipe = %li.",
                retCode);
     MessageBox (*hWnd, errorBuf, "Debug Window",
                 MB_ICONINFORMATION | MB_OK | MB_APPLMODAL);
     };

                                       // Block until a client connects.
   ConnectNamedPipe(hPipe, NULL);

                                       // Create and init overlap for writing.
   hEventWrt = CreateEvent (NULL, TRUE, FALSE, NULL);
   memset (&OverLapWrt, 0, sizeof(OVERLAPPED));
   OverLapWrt.hEvent = hEventWrt;

                                       // Set the clientIndex, then increment
                                       // the count.  Fill in the structure
                                       // for this client in the array.
   clientIndex = clientCount++;
   clients[clientIndex].hPipe   = hPipe;
   clients[clientIndex].live    = TRUE;
   clients[clientIndex].overLap = OverLapWrt;
   clients[clientIndex].hEvent  = hEventWrt;

                                       // Create and init overlap for reading.
   hEventRd = CreateEvent(NULL,TRUE,FALSE,NULL);
   memset (&OverLapRd, 0, sizeof(OVERLAPPED));
   OverLapRd.hEvent = hEventRd;

                                       // Read from the client, the first
                                       // first message should always be
                                       // the clients user name.
   retCode = ReadFile (hPipe, inBuf, PLEASE_READ, &bytesRead, &OverLapRd);

   if (!retCode)
    lastError = GetLastError();

   if (lastError == ERROR_IO_PENDING)  // Wait on read if need be.
     WaitForSingleObject (hEventRd, (DWORD)-1);

                                       // Put client's name in the array.
   strcpy (clients[clientIndex].Name, inBuf);

                                       // Create a thread which will make
                                       // another server instance of the
                                       // named pipe.
   CreateThread ((LPSECURITY_ATTRIBUTES)NULL,        // No security attributes.
                 (DWORD)0,                           // Use same stack size.
                 (LPTHREAD_START_ROUTINE)ServerProc, // Thread procedure.
                 (LPVOID)hWnd,                       // Parameter to pass.
                 (DWORD)0,                           // Run immediately.
                 (LPDWORD)&ServerThreadID);          // Thread identifier.

   TellAll("");                        // Forces a paint, draws a red spool
                                       // and name for this client.

                                       // Do loop which basically reads from
                                       // this specific client, and then
                                       // uses TellAll() to broadcast the
                                       // message to all the connected
                                       // clients.
   do{
                                       // Read the pipe.
      retCode = ReadFile (hPipe, inBuf, PLEASE_READ, &bytesRead, &OverLapRd);

                                       // Check for three kinds of errors:
                                       // If Error = IO_PENDING, wait til
                                       // the event handle signals success,
                                       // If BROKEN_PIPE, exit the do loop.
                                       // Any other error, flag it to the
                                       // user and exit the do loop.
      if (!retCode)
        {
        lastError = GetLastError();

        switch (lastError)
          {
                                       // IO_PENDING, wait on the event.
           case ERROR_IO_PENDING:
             WaitForSingleObject (hEventRd, (DWORD)-1);
             break;
                                       // Pipe is broken, exit the loop.
           case ERROR_BROKEN_PIPE:
             ExitLoop = TRUE;
             break;
                                       // Something else is wrong, exit the
                                       // the loop after telling the user.
           default:
             wsprintf (errorBuf, "ReadFile Error in ServerProc()= %d",
             lastError);
             MessageBox (*hWnd, errorBuf, "Debug Information", MB_OK);
             ExitLoop = TRUE;
             break;
          }
        }

      if (!ExitLoop)
        {
        GetOverlappedResult (hPipe, &OverLapRd, &bytesTransRd, FALSE);

                                       // Use TellAll to broadcast the message.
        if (bytesTransRd)
          TellAll(inBuf);
        else
          TellAll("");
        }

   }while(!ExitLoop);

   clients[clientIndex].live = FALSE;  // Turns spool gray.
   CloseHandle (hPipe);                // Close handles.
   CloseHandle (hEventRd);
   CloseHandle (hEventWrt);
   DisconnectNamedPipe (hPipe);        // Close pipe instance.
   ExitThread(0);                      // Clean up and die.
  }


/*************************************************************************\
*
*  PROCEDURE: TellAll (CHAR *buffer)
*
*  PURPOSE:
*
*    To write the buffer (input parameter) to all of the clients listed
*    in the global array "clients".
*
*  CALLED BY:
*
*    ServerProc();
*
*  COMMENTS:
*
*    Clients is a global array which hold information on each client
*    connected to the named pipe.  This procedure recieves a buffer.
*    It then steps through this global array, and for each client it
*    writes the buffer.
*
\*************************************************************************/

VOID TellAll( CHAR *buffer )
  {
    DWORD i;                           // Index through array.
    DWORD bytesWritten;                // Used in WriteFile().
    DWORD retCode;                     // Traps return codes.
    CHAR  Buf[LINE_LEN];               // Message Buffer.
    DWORD lastError;                   // Traps returns from GetLastError().

    for(i=0; i < clientCount; i++)     // For all clients in the array.
      {
                                       // If client isn't alive, don't waste
                                       // time writing to it.
      if (clients[i].live)
        {
        retCode = WriteFile (clients[i].hPipe, buffer, strlen(buffer),
                             &bytesWritten, &clients[i].overLap);

                                       // Check 3 kinds of errors: IO_PENDING,
                                       // NO_DATA, or other.  Wait on event
                                       // handle if IO_PENDING, else, if it's
                                       // anything other than NO_DATA (pipe
                                       // client disconnected), flag the user.
                                       // In any case, if it's not IO_PENDING,
                                       // clients[i].live = FALSE, spool turns
                                       // gray.
        if (!retCode)
          {
          lastError = GetLastError();

                                       // IO_PENDING, wait on event handle.
          if (lastError == ERROR_IO_PENDING)
            {
            WaitForSingleObject (clients[i].hEvent, (DWORD)-1);
            }
          else
            {
                                       // If not NO_DATA, flag user.
            if (lastError != ERROR_NO_DATA)
              {
              wsprintf (Buf, "%s = %d", buffer, GetLastError());
              MessageBox(hWnd, Buf, "Debug,TellAll:", MB_OK);
              }
            clients[i].live = FALSE;
            }
          }
        } //if client.live
      } // for loop
                                       // Paint window with new information.
    InvalidateRect(hWnd, NULL, TRUE);
  }



/*************************************************************************\
*
*  PROCEDURE: DrawBranch (HDC hDC)
*
*  PURPOSE:
*
*    To draw one of four bitmaps for each client, depending upon the clients
*    status (alive = red spool, dead or disconnected = gray), and location in
*    the array.  It also draws the clients user name beside the spool.
*    This procedure is executed when the WM_PAINT message is trapped.
*
*  CALLED BY:
*
*    WinMain();
*
*  COMMENTS:
*
\*************************************************************************/
VOID DrawBranch(HDC hDC)
{
                                       // Spool bitmaps.
  HBITMAP hEndLive, hEndDead, hMidLive, hMidDead, hBitMap;

  HDC hDCMem;
  int X, Y;
  BITMAP bm;
  POINT ptSize, ptOrg;
  DWORD index;

                                       // Load bitmaps: two red (live),
                                       // two dead (gray).  End = end
                                       // of tree (last client to connect),
                                       // mid means in the middle somewhere.
       hEndLive = LoadBitmap (hInst, "EndLive");
       hEndDead = LoadBitmap (hInst, "EndDead");

       hMidLive = LoadBitmap (hInst, "MidLive");
       hMidDead = LoadBitmap (hInst, "MidDead");

                                       // For each client, determine if
                                       // if alive or not, and position;
                                       // then blt appropriate map and
                                       // clients name.
    for (index = 0; index < clientCount; index++)
      {

      if (index < clientCount - 1)     // ClientCount - 1 = last (end) client.
       {
        if(clients[index].live)        // If live = red, else = gray.
         hBitMap = hMidLive;
        else
         hBitMap = hMidDead;
       }
      else
       {
        if(clients[index].live)        // If live = red, else = gray.
         hBitMap = hEndLive;
        else
         hBitMap = hEndDead;
       }
                                       // Calculate coordinates:
      X = BITMAP_X;                    // X position is constant.
      Y = index * BITMAP_Y;            // Y is based on index in the array.

                                       // Blt the chosen map.
      hDCMem = CreateCompatibleDC(hDC);
      SelectObject(hDCMem, hBitMap);
      SetMapMode(hDCMem, GetMapMode(hDC));

      GetObject(hBitMap, sizeof(BITMAP), &bm);

      ptSize.x = bm.bmWidth;
      ptSize.y = bm.bmHeight;
      DPtoLP (hDC, &ptSize, 1);

      ptOrg.x = 0;
      ptOrg.y = 0;
      DPtoLP (hDCMem, &ptOrg, 1);

      BitBlt(hDC, X, Y, ptSize.x, ptSize.y,
             hDCMem, ptOrg.x, ptOrg.y, SRCCOPY);


      X =  NAME_X;                     // Relocate X,Y for clients name.
      Y += NAME_Y;
                                       // Write name next to spool.
      TextOut (hDC, X, Y, clients[index].Name, strlen(clients[index].Name));
      DeleteDC(hDCMem);
      }


}
