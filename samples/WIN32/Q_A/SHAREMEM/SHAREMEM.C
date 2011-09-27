
/******************************************************************************\
*       This is a part of the Microsoft Source Code Samples. 
*       Copyright (C) 1992-1995 Microsoft Corporation.
*       All rights reserved. 
*       This source code is only intended as a supplement to 
*       Microsoft Development Tools and/or WinHelp documentation.
*       See these sources for detailed information regarding the 
*       Microsoft samples programs.
\******************************************************************************/

/*************************************************************************\
*  PROGRAM: sharemem.c
*
*  PURPOSE:
*
*    To demonstrate the use of named shared memory between two independent
*    processes.
*
*  GLOBAL VARIABLES:
*
*    HANDLE hInst:  - Instance handle.
*
*    HWND hWnd:     - Client window handle.
*
*    HWND hWndDlg:  - Handle to the dialog box which is imposed over the
*                     parent window.
*
*    LONG *MapView: - Address of where the file will be mapped to.
*
*    HANDLE hMapObject:
*                   - Handle to the file mapping object.
*
*
*  FUNCTIONS:
*
*    WinMain()      - Initializes the window, and process the message loop.
*    MainWndProc()  - To handle the messages to the main window.
*
*  COMMENTS:
*
*    Overview:
*      This sample is accompanied by the Othrproc (Other Procedure)
*      application.  Basically this code traps WM_MOUSEMOVE messages
*      and writes them to the shared memory.  The Otherproc code
*      reads the share memory and blts a small bitmap in the same
*      relative coordinates.  The effect is to have the mouse move
*      in one window, while the bitmap moves in the same relative
*      position in the other window.  The mouse coordinates will appear
*      in the edit fields of the dialog box at the top of the parent
*      windows.
*
*    To Use:
*      First start Sharemem, then start Otherproc.  The visual effect
*      is better if the focus remains with Otherproc, while the mouse
*      moves with the Sharemem window.
*
\*************************************************************************/

#include <windows.h>
#include <stdlib.h>
#include <stdio.h>
#include "sharemem.h"

#define WERR(who,where)    {sprintf(Buf,"ERROR: %s returned %u, line: %u", who, GetLastError(), __LINE__);\
                            sprintf(Buf2,"From within %s", where);\
                            MessageBox(hwnd, Buf, Buf2, MB_OK);}

CHAR Buf[80];
CHAR Buf2[80];


HANDLE hInst;
HANDLE hMapObject;
HWND   hWnd, hWndDlg;
LONG   *MapView;



/*************************************************************************\
*
*  FUNCTION: WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
*
*  PURPOSE:  Initializes the window, processes message loop.
*
*  COMMENTS: A fairly standard WinMain except that it creates a dialog
*            box which is imposed on the upper part of the window.  This
*            dialog box is used to show the mouse coordinates.
*
\*************************************************************************/

int PASCAL WinMain ( HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR  lpCmdLine,
                     int    nCmdShow)


{

  MSG  msg;
  WNDCLASS wc;
  RECT rect;
  DWORD dwVersion = GetVersion();

  UNREFERENCED_PARAMETER( lpCmdLine );
  UNREFERENCED_PARAMETER( hPrevInstance );

// check if running on Windows NT, if not, display notice and terminate
    if( !(dwVersion < 0x80000000) && (LOBYTE(LOWORD(dwVersion)) < 4) )
    {
       MessageBoxA( NULL,
           "This sample application cannot run on Win32s.\n"
           "This application will now terminate.",
           "OthrProc",
           MB_OK | MB_ICONSTOP | MB_SETFOREGROUND );
       return( 1 );
    }
  
  hInst = hInstance;

  wc.style = 0;
  wc.lpfnWndProc = (WNDPROC)MainWndProc;
  wc.cbClsExtra = 0;
  wc.cbWndExtra = 0;
  wc.hInstance = hInstance;
  wc.hIcon = LoadIcon (NULL, IDI_APPLICATION);
  wc.hCursor = LoadCursor (NULL, IDC_ARROW);
  wc.hbrBackground = GetStockObject (WHITE_BRUSH);
  wc.lpszMenuName = "";
  wc.lpszClassName = "ShareMemWClass";

  RegisterClass(&wc);

  hWnd = CreateWindow ("ShareMemWClass",
                       "Shared Memory Sample",
                       WS_OVERLAPPEDWINDOW,
                       CW_USEDEFAULT,
                       CW_USEDEFAULT,
                       CW_USEDEFAULT,
                       CW_USEDEFAULT,
                       NULL,
                       NULL,
                       hInstance,
                       NULL);

  hWndDlg     = CreateDialog (hInst, "StatusDlg", hWnd, NULL);

  GetClientRect (hWnd, &rect);
  SendMessage (hWnd, WM_SIZE, 0, (rect.right - rect.left));
  ShowWindow  (hWndDlg,  SW_SHOW);
  ShowWindow  (hWnd, nCmdShow);

  while (GetMessage (&msg, NULL, 0, 0))
    if (!IsDialogMessage (hWndDlg, &msg))
      {
       DispatchMessage (&msg);   // Dispatch message to window.
      }

  return (msg.wParam);           // Returns value from PostQuitMessage.

}

/*************************************************************************\
*
*  FUNCTION:  MainWndProc (HWND, UINT, WPARAM, LPARAM)
*
*  PURPOSE:   To process messages.  To create the shared memory.
*
*  MESSAGES:
*
*    WM_CREATE       - Create the shared memory by calling
*                      CreateFileMapping().  This memory is the size
*                      of a DWORD, all the size that is needed to pass
*                      mouse coordinates.  If the CreateFileMapping()
*                      call is successful, it then calls MapViewOfFile()
*                      to get the address of the memory.
*
*    WM_MOUSEMOVE    - Traps the mouse move messages, and uses structured
*                      exception handling to write the mouse coordinates
*                      to the share memory.  It also sends the coordinates
*                      to the edit fields of the dialog box.
*
*    WM_DESTROY      - Closes the memory handle, and destroys the window.
*
*    WM_SIZE         - Updates the size of the dialog box to maintain
*                      its relative size to the client window.
*
*  CALLED BY:
*
*    WinMain();
*
\*************************************************************************/

LRESULT CALLBACK MainWndProc (HWND hwnd,
                              UINT message,
                              WPARAM wParam,
                              LPARAM lParam)
{

  switch (message)
      {

        case WM_CREATE:
          hMapObject = CreateFileMapping ((HANDLE) 0xFFFFFFFF,
                                          NULL,
                                          PAGE_READWRITE,
                                          0,
                                          sizeof(DWORD),
                                          "shared_memory");
          if (!hMapObject)
            {
             WERR("CreateFileMapping","WM_CREATE");
            }
	  MapView = (LONG *) MapViewOfFile (hMapObject,
                                           FILE_MAP_WRITE,
                                           0, 0, 0);
          if (!MapView)
            MessageBox (hwnd, "ERROR: MapViewOfFile was not successful.",
                        "From within WM_CREATE", MB_OK);
          return (0);


        case WM_SIZE :
          SetWindowPos (hWndDlg, NULL, 0,0, LOWORD(lParam), DIALOGHEIGHT, 0);
          return (0);


        case WM_MOUSEMOVE:

          __try
            {
             MapView[0] = lParam;
            }
            __except(EXCEPTION_EXECUTE_HANDLER)
            {
              MessageBox( hwnd, "Trapped an exception when writing to memory.",
                          "From within WM_MOUSEMOVE", MB_OK);
            }

          SetDlgItemInt (hWndDlg, IDE_XCOORD, LOWORD(lParam), FALSE);
          SetDlgItemInt (hWndDlg, IDE_YCOORD, HIWORD(lParam), FALSE);
          return (0);

        case WM_DESTROY :
            CloseHandle (hMapObject);
            PostQuitMessage (0);
            return (0);

       }
    return DefWindowProc (hwnd, message, wParam, lParam);
}
