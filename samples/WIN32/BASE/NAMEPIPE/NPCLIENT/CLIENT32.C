
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
*  PROGRAM: client32.c
*
*  PURPOSE:
*
*     To demonstrate the use of named pipes and the overlapped structure.
*     This code serves as the client side of the named pipe instances.
*     For more details on an overview of this codes designs or use, see
*     the README file.  For details on the implementation, see the comments
*     in this code.
*
*
\*************************************************************************/

#define  STRICT
#include <windows.h>
#include "client32.h"

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

HANDLE hInst;
HWND   hWndClient;

CHAR ShrName[LINE_LEN];                 // Global: net share name.
CHAR ClntName[NAME_SIZE];               // Global: user or pipe client name.

/*************************************************************************\
*
*  FUNCTION: WinMain(HANDLE, HANDLE, LPSTR, int)
*
*  PURPOSE: Launches the Client's dialog box.
*
*  COMMENTS:
*
\*************************************************************************/

int APIENTRY WinMain (HINSTANCE hInstance,
                      HINSTANCE hPrevInstance,
                      LPSTR     lpCmdLine,
                      int       nCmdShow)


{
  DWORD retCode;

  UNREFERENCED_PARAMETER( nCmdShow );
  UNREFERENCED_PARAMETER( lpCmdLine );
  UNREFERENCED_PARAMETER( hPrevInstance );

  hInst   = hInstance;
  retCode = DialogBox ((HANDLE)hInst, (LPCTSTR)"ClientDialog",
                       NULL, (DLGPROC)ClientDlgProc);
  return  (retCode);

}


/*************************************************************************\
*
*  PROCEDURE: InitDlgProc (HWND hDlg, WORD wMsg, LONG wParam, LONG lParam)
*
*  PURPOSE:  This dialog box prompts the user for a net share name and
*            a client or user name.  These values are placed into global
*            strings ShrName and ClntName.
*
*  CALLED BY:
*
*    ClientDlgProc();
*
\*************************************************************************/

LONG CALLBACK InitDlgProc (HWND hDlg, UINT wMsg, WPARAM wParam, LPARAM lParam)
  {

  UNREFERENCED_PARAMETER(lParam);

  switch (wMsg)
    {
    case WM_INITDIALOG:
      PostMessage (GetDlgItem (hDlg, IDD_SVREDIT),
                   EM_LIMITTEXT, LINE_LEN, 0);

      PostMessage (GetDlgItem (hDlg, IDD_CLNTEDIT),
                   EM_LIMITTEXT, NAME_SIZE, 0);
    case WM_COMMAND:
      switch (LOWORD(wParam))
        {                            // When the user clicks okay, get the
        case IDB_INITOK:             // share name and user name from the
                                     // edit fields.
          GetWindowText (GetDlgItem (hDlg, IDD_SVREDIT), ShrName, LINE_LEN);
          GetWindowText (GetDlgItem (hDlg, IDD_CLNTEDIT), ClntName, NAME_SIZE);
          EndDialog(hDlg, 0);
          return (0);

        default:
          return (0);
        }
    default:
      return (0);
    }
  return (0);
  }


/*************************************************************************\
*
*  PROCEDURE: ClientDlgProc (HWND hDlg, WORD wMsg, LONG wParam, LONG lParam)
*
*  PURPOSE:  This procedure services the dialog box that serves as an interface
*            to the named pipe server instance.  The larger edit field is used
*            to read messages from the server instance.  The smaller edit field
*            is used to type messages to the server instance.  This procedure
*            is responsible for connecting to the named pipe, creating a
*            seperate thread to read the pipe, and sending to the pipe.
*
*  CALLED BY:
*
*    WinMain();
*
*  CALLS TO:
*
*    InitDlgProc();
*    ReadPipe();
*
\**************************************************************************/

LONG CALLBACK ClientDlgProc (HWND hDlg, UINT wMsg, WPARAM wParam, LPARAM lParam)
  {
  DWORD  retCode;                      // Return code.
  CHAR   errorBuf[LINE_LEN] = "";      // Error message buffer.
  CHAR   outBuf[OUT_BUF_SIZE]  = "";   // Buffer trapping message to send.
  CHAR   sendBuf[OUT_BUF_SIZE] = "";   // Buffer used to modify message.
  DWORD  bytesWritten;                 // Used for WriteFile().
  DWORD  threadID;                     // Used for CreateThread().
  CHAR   fileName[LINE_LEN+NAME_SIZE+2]; // Used to modify pipe/file name.
  DWORD  lastError;                    // Used to get returns from GetLastError.

  static HANDLE hPipe;                 // File or Pipe handle.
  static OVERLAPPED OverLapWrt;        // Overlapped structure
  static HANDLE     hEventWrt;         // Event handle for overlapped writes.

  UNREFERENCED_PARAMETER( lParam );

  hWndClient = hDlg;

  switch (wMsg)
    {

    case WM_COMMAND:
      switch (LOWORD(wParam))
        {

        // When the user presses Send: capture the string from the edit
        // field, prepend it with the user name, and overlap write it to
        // the server instance of the named pipe.

        case IDB_SEND:                 // Get the text from the edit field.
          GetWindowText (GetDlgItem(hDlg,IDD_EDITWRITE),
                         outBuf, PLEASE_WRITE);

                                       // Prepend it with the user name, and
                                       // terminate it with a new line
                                       // character.

          wsprintf (sendBuf, "%s%s %s\n", ClntName, ":", outBuf);

                                       // Do the overlapped write.
          retCode = WriteFile (hPipe, sendBuf, PLEASE_WRITE,
                     &bytesWritten, &OverLapWrt);
          if (!retCode)
            {
            lastError = GetLastError();
                                       // If Error = IO_PENDING, wait til
                                       // the event signals success.
            if (lastError == ERROR_IO_PENDING)
              WaitForSingleObject (hEventWrt, (DWORD)-1);
            }

          return (0);

        default:
          return (0);

        }


    case WM_INITCLIENT:

       // On initialization, use the Init dialog box prompt the user for a
       // net share name and a client or user name.  A share name of "."
       // means that the named pipe is local to this machine.  Named pipe
       // names should have the form '\\.\PIPE\<pipename>', for a local machine
       // or '\\<machinename>\PIPE\<pipename>' for remote machines.
       // Once the share name is captured from the Init dialog box,
       // convert the name into the proper form; then do a CreateFile()
       // to connect to the pipe.  Handle any error from the CreateFile().
       // Then write the user name to the server instance of the named
       // pipe.  Finally, create a thread to read the named pipe.

                                       // Launch Init dialog box to capture
                                       // share name and user name.
       DialogBox ((HANDLE)GetModuleHandle(NULL),
                  (LPCTSTR)"InitDialog",
                  (HWND)hDlg,
                  (DLGPROC)InitDlgProc);

                                       // Put captured user name in window
                                       // caption.
       SetWindowText (hDlg, ClntName);

                                       // Construct file/pipe name.
       wsprintf (fileName, "%s%s%s", "\\\\", ShrName, "\\PIPE\\test");

                                       // Do CreateFile() to connect to the
                                       // named pipe.
       hPipe = CreateFile (fileName,              // Pipe name.
                           GENERIC_WRITE          // Generic access, read/write.
                           | GENERIC_READ,
                           FILE_SHARE_READ        // Share both read and write.
                           | FILE_SHARE_WRITE ,
                           NULL,                  // No security.
                           OPEN_EXISTING,         // Fail if not existing.
                           FILE_FLAG_OVERLAPPED,  // Use overlap.
                           NULL);                 // No template.

                                       // Do some error checking.
       if ((DWORD)hPipe == 0xFFFFFFFF)
         {
         retCode = GetLastError();

                                       // This error means pipe wasn't found.
         if ((retCode == ERROR_SEEK_ON_DEVICE) ||
             (retCode == ERROR_FILE_NOT_FOUND))
           MessageBox (hDlg,
                       "CANNOT FIND PIPE: Assure Server32 is started, check share name.",
                       "",
                       MB_OK);
         else
           {                           // Flagging unknown errors.
           wsprintf (errorBuf,
                     "CreateFile() on pipe failed, see winerror.h error #%d.",
                     retCode);
           MessageBox (hDlg, errorBuf, "Debug Window",
                       MB_ICONINFORMATION | MB_OK | MB_APPLMODAL);
           }

         EndDialog (hDlg, 0);          // Kill app if pipe didn't connect.
         };

                                       // Create and init overlapped structure
                                       // for writes.
       hEventWrt = CreateEvent (NULL, TRUE, FALSE, NULL);
       OverLapWrt.hEvent = hEventWrt;

                                       // Write the client name to server.
       retCode = WriteFile (hPipe, ClntName, PLEASE_WRITE,
                            &bytesWritten, &OverLapWrt);

       if (!retCode)                   // Wait on overlapped if need be.
         {
         lastError = GetLastError();
         if (lastError == ERROR_IO_PENDING)
           WaitForSingleObject (hEventWrt, (DWORD)-1);
         }
                                       // Create a thread to read the pipe.
       CreateThread (NULL,
                     0,
                     (LPTHREAD_START_ROUTINE)ReadPipe,
                     (LPVOID)&hPipe,
                     0,
                     &threadID);
       return (0);



     case WM_INITDIALOG:
                                       // PostMessage() give time for the
                                       // dialog box to be created.
       PostMessage (hDlg, WM_INITCLIENT, 0, 0);
       return (0);



     case WM_GO_AWAY:
       CloseHandle (hPipe);
       CloseHandle (hEventWrt);
       EndDialog (hDlg, TRUE);
       return TRUE;



     case WM_SYSCOMMAND:
       if (wParam == SC_CLOSE)
         {
         CloseHandle (hPipe);
         CloseHandle (hEventWrt);
         EndDialog(hDlg, TRUE);
         return TRUE;
         }
       break;
     }
    return (FALSE);

  }

/*************************************************************************\
*
*  PROCEDURE: ReadPipe (HANDLE *hRead)
*
*  PURPOSE:  This is a thread function which loops and reads the named pipe.
*
*  CALLED BY:
*
*    ClientDlgProc.
*
*
\*************************************************************************/

VOID ReadPipe (HANDLE *hPipe)
  {
    CHAR       inBuf[IN_BUF_SIZE] = "";// Input buffer.
    DWORD      bytesRead;              // Used for ReadFile()
    DWORD      retCode;                // Used to trap return codes.
    CHAR       Buf[80];                // Message box buffer.
    DWORD      lastError;              // Used to trap returns from GetLastError.

    HANDLE     hEventRd;               // Event handle for overlapped reads.
    OVERLAPPED OverLapRd;              // Overlapped structure.
    DWORD      bytesTrans;             // Bytes transferred in read.

                                       // Create and init overlap structure.
    hEventRd = CreateEvent (NULL, TRUE, FALSE, NULL);
    memset (&OverLapRd, 0, sizeof(OVERLAPPED));
    OverLapRd.hEvent = hEventRd;

    // Loop, reading the named pipe until it is broken.  The ReadFile() uses
    // an overlapped structure.  When the event handle signals a completed
    // read, this loop writes the message to the larger edit field.

    do{
                                       // Read the pipe handle.
      retCode = ReadFile (*hPipe, inBuf, IN_BUF_SIZE, &bytesRead, &OverLapRd);

      if (!retCode)                    // Do some error checking.
       {
        lastError = GetLastError();
                                       // Check for 3 kinds of errors:
                                       // IO_PENDING, BROKEN_PIPE, or
                                       // other.
                                       // If Error = IO_PENDING, wait for
                                       // event handle to signal success.
        if (lastError == ERROR_IO_PENDING)
          {
          WaitForSingleObject (hEventRd, (DWORD)-1);

          }
        else                           // If pipe is broken, tell user and
          {                            // break.
          if (lastError == (DWORD)ERROR_BROKEN_PIPE)
            MessageBox (hWndClient,
                        "The connection to this client has been broken.",
                        "", MB_OK);
          else                         // Or flag unknown errors, and break.
            {
             wsprintf (Buf, "ReadFile() on pipe failed, see winerror.h error #%d", GetLastError());
             MessageBox (hWndClient, Buf, "Client: Debug", MB_OK);
            }
          break;
          }
       }
                                       // NULL terminate string.
      GetOverlappedResult (*hPipe, &OverLapRd, &bytesTrans, FALSE);
      inBuf[bytesTrans] = '\0';

                                       // Write message to larger edit field.
      SendMessage (GetDlgItem (hWndClient, IDD_EDITREAD),
                   EM_REPLACESEL,
                   0, (LONG)inBuf);
                                       // Add a new line.
      SendMessage (GetDlgItem (hWndClient, IDD_EDITREAD),
                   EM_REPLACESEL,
                   0, (LONG)"\r\n");
      }while(1);

                                       // When pipe is broken, send quit
                                       // messages to Client dialog box.
    PostMessage (hWndClient, WM_GO_AWAY, 0,0);
    ExitThread(0);
  }
