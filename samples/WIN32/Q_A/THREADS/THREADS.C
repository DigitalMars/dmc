
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
*  PROGRAM: Threads.c
*
*  PURPOSE:
*
*     To demonstrate suspending, resuming, and setting the priorities
*     of threads.
*
*  FUNCTIONS:
*
*    WinMain()      - Initializes the window, and process the message loop.
*    MainWndProc()  - Process messages, launches server & client threads.
*    ThreadProc()   - Draws rectangles to the window, demonstrating the
*                     threads performance.
*
*  COMMENTS:
*
*    To Use:
*      When starting this application, two threads are created.  The
*      first draws a green box, the second a red.  Both boxes are moved
*      about the screen as their individual threads calculate a new
*      position and redraws the box.  The user can Suspend either thread,
*      Resume them, or change their priority through the menu selections.
*      The colored boxes will respond accordingly.
*
*      Note through out the sample "red thread" or "green thread" are
*      referred to.  This simply indicates the thread which draws either the
*      red or green rectangle.
*
\*************************************************************************/


#include <windows.h>
#include <stdlib.h>
#include <time.h>
#include "threads.h"


HANDLE hInst;
HANDLE hWnd;

/*************************************************************************\
*
*  FUNCTION: WinMain(HANDLE, HANDLE, LPSTR, int)
*
*  PURPOSE: calls initialization function, processes message loop
*
*  COMMENTS:
*
\*************************************************************************/
int PASCAL WinMain ( HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR  lpCmdLine,
                     int    nCmdShow)
{
  MSG  msg;
  WNDCLASS wc;

  UNREFERENCED_PARAMETER( lpCmdLine );
  UNREFERENCED_PARAMETER( hPrevInstance );

  // check if Win32s, if so, display notice and terminate
      if( GetVersion() & 0x80000000 && (GetVersion() & 0xFF) ==3)
      {
        MessageBox( NULL,
           "This application cannot run on Windows 3.1.\n"
           "This application will now terminate.",
           "Threads",
           MB_OK | MB_ICONSTOP | MB_SETFOREGROUND );
        return( 1 );
      }

  hInst = hInstance;

  wc.style = 0;                          // Replaces CS_SIZEREDRAW.
  wc.lpfnWndProc = (WNDPROC)MainWndProc; // The client window procedure.
  wc.cbClsExtra = 0;                     // No room reserved for extra data.
  wc.cbWndExtra = 0;
  wc.hInstance = hInstance;
  wc.hIcon = LoadIcon (NULL, IDI_APPLICATION);
  wc.hCursor = LoadCursor (NULL, IDC_ARROW);
  wc.hbrBackground = GetStockObject (WHITE_BRUSH);
  wc.lpszMenuName = "Thread_Menu";
  wc.lpszClassName = "ThreadsWClass";

  RegisterClass(&wc);


  hWnd = CreateWindow ("ThreadsWClass",
                       "Threads Sample",
                       WS_OVERLAPPEDWINDOW,
                       CW_USEDEFAULT,
                       CW_USEDEFAULT,
                       CW_USEDEFAULT,
                       CW_USEDEFAULT,
                       NULL,
                       NULL,
                       hInstance,
                       NULL);


  ShowWindow(hWnd, nCmdShow);
  while (GetMessage (&msg, NULL, 0, 0))
    DispatchMessage (&msg);   // Dispatch message to window.

  return (msg.wParam);           // Returns value from PostQuitMessage.

}

/*************************************************************************\
*
*  FUNCTION:  MainWndProc (HWND, UINT, WPARAM, LPARAM)
*
*  PURPOSE:   To process the windows messages.  This procedure totally
*             controls the priority and suspension/resumption of the
*             thread.
*
*  VARIABLES USED:
*
*    - hThread1, hThread2:
*             Static handles to the two created threads.
*
*    - ThreadID1 ThreadID2:
*             DWORDs used in the CreateThread call.
*
*    - pColor1, pColor2:
*             DWORDs used to allocate some memory to use as a parameter
*             to pass color values to the threads.  This memory was
*             allocated so that the threads wouldn't have to rely on this
*             procedures stack for the values.
*
*    - Buf[80]:
*             Character buffer used to write messages to the user.
*
*    - RedSuspendCnt, GreenSuspendCnt:
*             Static DWORDs used to track the count of suspension put
*             on the specific thread.
*
*  MESSAGES:
*
*    WM_DESTROY:     - Terminates the threads and post the quit message.
*    WM_CREATE:      - Allocates memory to hold some color values, and
*                      creates the two threads.
*    WM_COMMAND
*
*      IDM_SUSPEND***:
*               Suspends the specified thread, updates the globally kept
*               count (number of times the thread has been suspended),
*               and then produces a message box telling the user the
*               number of suspensions.
*
*      IDM_RESUME***:
*               Resumes the specified thread, updates the gobally kept
*               count (number of times the thread has been suspended),
*               and then produces a message box telling the user the
*               number of suspensions.
*
*      IDM_G***:
*               These 7 messages use SetThreadPriority to set the
*               priority of the Green Thread.
*
*      IDM_R***:
*               These 7 messages use SetThreadPriority to set the
*               priority of the Red Thread.
*
*  CALLED BY:
*
*    WinMain();
*
*  CALLS TO:
*
*    ThreadProc();
*
*  COMMENTS:
*
*
\*************************************************************************/


LRESULT CALLBACK MainWndProc (HWND hwnd,
                              UINT message,
                              WPARAM wParam,
                              LPARAM lParam)
{
  static HANDLE hThread1, hThread2;
  DWORD  ThreadID1, ThreadID2;
  CHAR Buf[80];
  static DWORD RedSuspendCnt = 0;
  static DWORD GreenSuspendCnt = 0;
  static DWORD *pColor1, *pColor2;

  switch (message)
      {

        case WM_CREATE :

          pColor1 = malloc(sizeof(DWORD));
          *pColor1 = GREEN;
          hThread1 = CreateThread (NULL, 0,
                                   (LPTHREAD_START_ROUTINE)ThreadProc,
                                   (LPVOID)pColor1, 0,
                                   (LPDWORD)&ThreadID1);

          if (!hThread1)
            {
             wsprintf(Buf, "Error in creating Green thread: %d",
                      GetLastError());
             MessageBox (hwnd, Buf, "WM_CREATE", MB_OK);
            }

          Sleep (500);             // Allow some time/distance between the
                                   // thread boxes.

          pColor2 = malloc(sizeof(DWORD));
          *pColor2 = RED;
          hThread2 = CreateThread (NULL, 0,
                                   (LPTHREAD_START_ROUTINE)ThreadProc,
                                   (LPVOID)pColor2, 0,
                                   (LPDWORD)&ThreadID2);
          if (!hThread2)
            {
             wsprintf(Buf, "Error in creating Red thread: %d",
                      GetLastError());
             MessageBox (hwnd, Buf, "WM_CREATE", MB_OK);
            }


          return (0);

        case WM_COMMAND:
          switch (LOWORD(wParam))
            {
            case IDM_SUSPENDGREEN:           // Suspends green thread.
              SuspendThread (hThread1);
              GreenSuspendCnt++;
              wsprintf(Buf, "The suspension count for the green thread is now %d",
                       GreenSuspendCnt);
              MessageBox(hWnd, Buf, "Suspension Count", MB_OK);
              return (0);

            case IDM_SUSPENDRED:             // Suspends red thread.
              SuspendThread (hThread2);
              RedSuspendCnt++;
              wsprintf(Buf, "The suspension count for the red thread is now %d",
                       RedSuspendCnt);
              MessageBox(hWnd, Buf, "Suspension Count", MB_OK);
              return (0);

            case IDM_RESUMEGREEN:            // Resumes green thread.
              ResumeThread (hThread1);
              if (GreenSuspendCnt > 0)
                GreenSuspendCnt--;
              wsprintf(Buf, "The suspension count for the green thread is now %d",
                       GreenSuspendCnt);
              MessageBox(hWnd, Buf, "Suspension Count", MB_OK);
              return (0);

            case IDM_RESUMERED:              // Resumes red thread.
              ResumeThread (hThread2);
              if (RedSuspendCnt > 0)
                RedSuspendCnt--;
              wsprintf(Buf, "The suspension count for the red thread is now %d",
                       RedSuspendCnt);
              MessageBox(hWnd, Buf, "Suspension Count", MB_OK);
              return (0);


            case IDM_GIDLE:                   // Sets green idle.
              SetThreadPriority (hThread1, THREAD_PRIORITY_IDLE);
              return (0);


            case IDM_GLOW:                   // Sets green lowest.
              SetThreadPriority (hThread1, THREAD_PRIORITY_LOWEST);
              return (0);

            case IDM_GBNORM:                 // Sets green below normal.
              SetThreadPriority (hThread1, THREAD_PRIORITY_BELOW_NORMAL);
              return (0);

            case IDM_GNORM:                  // Sets green to normal.
              SetThreadPriority (hThread1, THREAD_PRIORITY_NORMAL);
              return (0);

            case IDM_GANORM:                 // Sets green above normal.
              SetThreadPriority (hThread1, THREAD_PRIORITY_ABOVE_NORMAL);
              return (0);

            case IDM_GHIGH:                  // Sets green to highest.
              SetThreadPriority (hThread1, THREAD_PRIORITY_HIGHEST);
              return (0);

            case IDM_GTC:                   // Sets green time critical.
              SetThreadPriority (hThread1, THREAD_PRIORITY_TIME_CRITICAL);
              return (0);




            case IDM_RIDLE:                   // Sets red idle.
              SetThreadPriority (hThread1, THREAD_PRIORITY_IDLE);
              return (0);

            case IDM_RLOW:                   // Sets red to lowest possible
              SetThreadPriority (hThread2, THREAD_PRIORITY_LOWEST);
              return (0);

            case IDM_RBNORM:                 // Sets red below normal.
              SetThreadPriority (hThread2, THREAD_PRIORITY_BELOW_NORMAL);
              return (0);

            case IDM_RNORM:                  // Sets red to normal.
              SetThreadPriority (hThread2, THREAD_PRIORITY_NORMAL);
              return (0);

            case IDM_RANORM:                 // Sets red above normal.
              SetThreadPriority (hThread2, THREAD_PRIORITY_ABOVE_NORMAL);
              return (0);

            case IDM_RHIGH:                  // Sets red to highest.
              SetThreadPriority (hThread2, THREAD_PRIORITY_HIGHEST);
              return (0);

            case IDM_RTC:                   // Sets green time critical.
              SetThreadPriority (hThread1, THREAD_PRIORITY_TIME_CRITICAL);
              return (0);

            default:
              return (0);


            }

        case WM_DESTROY :
            TerminateThread(hThread1, 0);
            TerminateThread(hThread2, 0);
            free (pColor1);
            free (pColor2);
            PostQuitMessage (0);
            return (0);

       }
    return DefWindowProc (hwnd, message, wParam, lParam);
}

/*************************************************************************\
*
*  FUNCTION:  ThreadProc (LPVOID)
*
*  PURPOSE:   A thread procedure which calculates position on the window
*             and draws a colored rectangle.  The color of the rectangle
*             is determined by the input parameter, it's border is
*             always black (or whatever the pen color is); and since
*             the window does not paint between rectangle draws, the
*             rectangle leaves a black path in it's wake.
*
*  VARIABLES USED:
*
*    - horizontal, vertical:
*             Local int used to indicate the next directional move the
*             rectangle will make.
*
*    - ulx, uly:
*             Local DWORD used for the Upper Left X corner and Upper
*             Upper Left Y position of the rectangle.
*
*    - rect:  A RECT structure used to determin the current size of the
*             window (in case the user resizes it).
*
*    - hdc:   HDC of the rectangle.
*
*    - Time:  A SYSTEMTIME structure.  It's milli-second field is used
*             to create an apparent random starting point for the
*             rectangles.
*
*    -hBrush: A handle to a Brush object, used to set the color of the
*             rectangle.
*
*    -width, height:
*             Local DWORDs used for the width and height of the rectangles.
*
*  CALLED BY:
*
*    MainWndProc();
*
\*************************************************************************/


VOID ThreadProc ( LPVOID *Color)
{
  int  horizontal, vertical;
  DWORD ulx, uly;
  RECT rect;
  HDC  hDC;
  SYSTEMTIME Time;
  HANDLE hBrush;
  DWORD width, height;

  width =  10;
  height = 10;

  GetSystemTime (&Time);                     // Get the time.

  do{}while(!GetClientRect(hWnd, &rect));    // Loop, making sure window
                                             // exists.

  ulx = (Time.wMilliseconds % rect.right);   // Use MOD to get a random
  uly = (Time.wMilliseconds % rect.bottom);  // position.

  if(Time.wMilliseconds % 2 == 0)            // Use MOD to pick random
    {                                        // directions.
     horizontal = 1;
     vertical = 1;
    }
  else
    {
     horizontal = 1;
     vertical = -1;
    }
                                             // Set color as per input
                                             // parameter.
  hBrush = CreateSolidBrush((COLORREF)*Color);

  do
    {                                        // do forever ...
     GetClientRect( hWnd, &rect);

     if ( (ulx+width) > (DWORD)rect.right)   // ... check for right edge,
      {
      ulx = rect.right - width;
      horizontal = -1;                       //   ... if so change direction;
      }

     if ((uly+height) > (DWORD)rect.bottom)  // ... check for bottom edge,
      {
      uly = rect.bottom - height;            //   ... if so change dir.
      vertical = -1;
      }

     if (uly <= 1)                           // ... check for right edge,
      {
      uly = 1;
      vertical = 1;
      }

     if (ulx <= 1)
      {                                      // ... check for top edge;
      ulx = 1;
      horizontal = 1;
      }

     hDC = GetDC(hWnd);                      // ... Get DC,
     SelectObject( hDC, hBrush);             // ... Set brush color,
                                             // ... Draw rectangle,
     Rectangle (hDC, ulx, uly, ulx+width, uly+height);
     ReleaseDC(hWnd, hDC);                   // ... Release DC
     ulx += horizontal;                      // ... Increment/decrement
     uly += vertical;                        // ... position.

    }while(1);
}
