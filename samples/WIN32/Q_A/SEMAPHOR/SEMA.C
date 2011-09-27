/*************************************************************************\
*  PROGRAM: Sema.c
*
*  PURPOSE:
*
*     To demonstrate controlling multiple threads through the use of a
*     semaphore.
*
*  FUNCTIONS:
*
*    WinMain()      - Initializes the window, and process the message loop.
*    MainWndProc()  - Process messages, creates semaphore and threads.
*
*  GLOBAL VARIABLES:
*
*    - hWnd:  Handle to the window.
*    - hSemaphore:
*             Handle of the semaphore used between the threads.
*
*  COMMENTS:
*
*    To Use:
*      Start the application.  You will see five rectangles: a dynamic
*      rectangle in the center (always changing color), and four static
*      rectangles surrounding it.  Each of the four rectagles has their
*      own color: red, blue, green, and gray.  The one in the center
*      alternates between these colors.
*
*      The four static rectangles represent four threads.  These four
*      threads compete for the rectangle in the middle, and their access
*      is controlled by the semaphore.  When the thread gains control of
*      the semaphore, it gets to draw it's color in the center rectangle.
*      (note: the threads do not actually draw any of the four static
*      rectangles.  To make the code simpler this is handled in the
*      WM_PAINT message in the MainWndProc().  They are only meant as
*      a visual representation of the threads.  The threads do however
*      draw the rectangle in the center with their specific color.)
*
*    What Happens:
*      The semaphore has a use count.  When it is set to zero, any thread
*      can access the semaphore and execute the code within its "semaphore
*      gate" by using WaitForSingleObject().  When the thread gains
*      control of the semaphore usint this call, the use count is
*      incremented by one.  When the thread is done executing its code,
*      it can call ReleaseSemaphore().  This will decrement the count
*      by what ever value you indicate (this code uses 1) signalling
*      to any other thread that it may gain control of the semaphore.
*
*      Note that any thread which has access to the semaphore may decrement
*      its use count with ReleaseSemaphore(); the thread does not have
*      to have control of the semaphore at the time.
*
*      In this code, the WM_CREATE message in MainWndProc creates a
*      semaphore.  The four threads are then created, each waiting
*      on the semaphore .
*
*      Each of the threads loop, blocking on a WaitForSingleObject()
*      call.  Once any thread has set the use count to 0, the thread
*      gets to draw draw the center rectagle with its color, and then
*      sleeps for half a second before freeing the semaphore again.
*      The thread then runs through the loop again.
*
\*************************************************************************/
#include <windows.h>
#include <stdlib.h>
#include "sema.h"


HANDLE hInst;
HANDLE hWnd;
HANDLE hSemaphore;

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
  DWORD dwVersion = GetVersion();

  UNREFERENCED_PARAMETER( lpCmdLine );
  UNREFERENCED_PARAMETER( hPrevInstance );

  // check if Win32s, if so, display notice and terminate
      if( !(dwVersion < 0x80000000 ) && (LOBYTE(LOWORD(dwVersion)) < 4) )
      {
        MessageBox( NULL,
           "This application cannot run on Windows 3.1.\n"
           "This application will now terminate.",
           "Semaphor",
           MB_OK | MB_ICONSTOP | MB_SETFOREGROUND );
        return( 1 );
      }

  hInst = hInstance;

  wc.style = CS_VREDRAW | CS_HREDRAW;
  wc.lpfnWndProc = (WNDPROC)MainWndProc;
  wc.cbClsExtra = 0;
  wc.cbWndExtra = 0;
  wc.hInstance = hInstance;
  wc.hIcon = LoadIcon (NULL, IDI_APPLICATION);
  wc.hCursor = LoadCursor (NULL, IDC_ARROW);
  wc.hbrBackground = GetStockObject (WHITE_BRUSH);
  wc.lpszMenuName = "";
  wc.lpszClassName = "SemaWClass";

  RegisterClass(&wc);


  hWnd = CreateWindow ("SemaWClass",
                       "Semaphore Sample",
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
*  PURPOSE:   To process the windows messages.  This procedure creates
*             the semaphore and the four threads.  It also draws the
*             four static rectangles.
*
*  VARIABLES USED:
*
*    - hThread?:
*             These four static varaibles are handles to the four
*             created threads.
*
*    - ThreadID?:
*             These four variables are local DWORDS used in the
*             CreateThread() call.
*
*    - pColor?:
*             These four variables are used as pointers to memory
*             which is allocated and filled with color values to
*             pass as a parameter to each of the threads (so the
*             threads know what color they are to paint the rectangle).
*
*    - Paint: A paint structure used in BeginPaint() and EndPaint().
*
*    - rect:  A RECT structure used to get the windows client area.
*             It's needed to figure out the new relative positions
*             of the four static rectangles when the user resizes
*             the window.
*
*    - Col?, Row?, width, height:
*             These local DWORDs are used to position the four static
*             rectangles and to determin their size.
*
*    - hDC:   Handle to the DC for the window.
*
*    - hBrush:
*             Handle to a brush, used to set the color of the four static
*             rectangles.
*
*  MESSAGES:
*
*    WM_CREATE:      - Creates a semaphore; allocates some
*                      memory and sets it with color values to be passed
*                      as parameter values to the threads.  It then
*                      creates four threads, giving each its own color.
*
*    WM_PAINT:       - Checks the size of the client space and then
*                      draws the four static rectangles to their relative
*                      positions.
*
*    WM_DESTROY:     - Terminates the threads, and posts a quit message.
*
*  CALLED BY:
*
*    WinMain();
*
*  CALLS TO:
*
*    ThreadProc();
*
\*************************************************************************/


LRESULT CALLBACK MainWndProc (HWND hwnd,
                              UINT message,
                              WPARAM wParam,
                              LPARAM lParam)
{
  DWORD  ThreadID1, ThreadID2, ThreadID3, ThreadID4;
  RECT   rect;
  DWORD  Col1, Col2, Col3, Row1, Row2, Row3, width, height;
  HDC    hDC;
  HANDLE hBrush;
  PAINTSTRUCT Paint;
  static HANDLE hThread1, hThread2, hThread3, hThread4;
  static DWORD  *pColor1,  *pColor2,  *pColor3,  *pColor4;
  CHAR Buf[80];


  switch (message)
      {

        case WM_PAINT:
          hDC = BeginPaint (hwnd, &Paint);
          GetClientRect( hWnd, &rect);       // Get window rectangle.

          width =  rect.right / 4;           // Calculate static rectangle's
          height = rect.bottom / 4;          // width and height.

          Col1 = (rect.right / 16)  * 1;     // Calculate x positions.
          Col2 = (rect.right / 16)  * 6;
          Col3 = (rect.right / 16)  * 11;
                                             // Calculate y positions.
          Row1 = (rect.bottom / 16) * 1;
          Row2 = (rect.bottom / 16) * 6;
          Row3 = (rect.bottom / 16) * 11;

          hBrush = CreateSolidBrush((COLORREF)RED);
          SelectObject (hDC, hBrush);
          Rectangle (hDC, Col2, Row1, Col2+width, Row1+height);
          DeleteObject (hBrush);
                                             // Draw the Blue rectangle.
          hBrush = CreateSolidBrush((COLORREF)BLUE);
          SelectObject (hDC, hBrush);
          Rectangle (hDC, Col1, Row2, Col1+width, Row2+height);
          DeleteObject (hBrush);
                                             // Draw the Green rectangle.
          hBrush = CreateSolidBrush((COLORREF)GREEN);
          SelectObject (hDC, hBrush);
          Rectangle (hDC, Col2, Row3, Col2+width, Row3+height);
          DeleteObject (hBrush);
                                             // Draw the Gray rectangle.
          hBrush = CreateSolidBrush((COLORREF)GRAY);
          SelectObject (hDC, hBrush);
          Rectangle (hDC, Col3, Row2, Col3+width, Row2+height);
          DeleteObject (hBrush);

          EndPaint (hwnd, &Paint);
          return (0);

        case WM_CREATE :
                                             // Create the semaphore.
          hSemaphore = CreateSemaphore (NULL, 0, 1, NULL);

          if (!hSemaphore)
            {
             wsprintf (Buf, "CreateSemaphore() Error = %d",
                       GetLastError());
             MessageBox(hwnd, Buf, "WM_CREATE", MB_OK);
             return (0);
            }

          pColor1  = malloc(sizeof(DWORD));
          *pColor1 = GREEN;                  // Create the Green thread.
          hThread1 = CreateThread (NULL, 0,
                                   (LPTHREAD_START_ROUTINE)ThreadProc,
				   (LPVOID)pColor1, 0,
                                   (LPDWORD)&ThreadID1);

          pColor2  = malloc(sizeof(DWORD));
          *pColor2 = RED;                    // Create the Red thread.
          hThread2 = CreateThread (NULL, 0,
                                   (LPTHREAD_START_ROUTINE)ThreadProc,
				   (LPVOID)pColor2, 0,
                                   (LPDWORD)&ThreadID2);

          pColor3  = malloc(sizeof(DWORD));
          *pColor3 = BLUE;                   // Create the Blue thread.
          hThread3 = CreateThread (NULL, 0,
                                   (LPTHREAD_START_ROUTINE)ThreadProc,
				   (LPVOID)pColor3, 0,
                                   (LPDWORD)&ThreadID3);

          pColor4  = malloc(sizeof(DWORD));
          *pColor4 = GRAY;                   // Create the Gray thread.
          hThread4 = CreateThread (NULL, 0,
                                   (LPTHREAD_START_ROUTINE)ThreadProc,
				   (LPVOID)pColor4, 0,
                                   (LPDWORD)&ThreadID4);

          ReleaseSemaphore (hSemaphore, 1, NULL);

          return (0);


        case WM_DESTROY :                    // Terminate the threads and
            TerminateThread(hThread1, 0);    // clean up.
            TerminateThread(hThread2, 0);
            TerminateThread(hThread3, 0);
            TerminateThread(hThread4, 0);
            free (pColor1);
            free (pColor2);
            free (pColor3);
            free (pColor4);

            PostQuitMessage (0);
            return (0);

       }
    return DefWindowProc (hwnd, message, wParam, lParam);
}


/*************************************************************************\
*
*  FUNCTION:  ThreadProc (LPVOID)
*
*  PURPOSE:   A thread procedure which requests a semaphore, and then
*             draws it's color to the rectangle in the middle of the
*             window.
*
*  VARIABLES USED:
*
*    - Color: An input parameter, a pointer to a DWORD with a color
*             value in it.
*
*    - rect:  The RECT rectangle of the client window, needed to know where
*             the center rectangle resides.
*
*    - hDC:   Handle to the DC of the window.
*
*    - hBrush:
*             Handle to a brush, used to set the rectangle's color to the
*             threads.
*
*    - width, height, Col, Row:
*             DWORDs use to position the rectangle.
*
*  CALLED BY:
*
*    MainWndProc();
*
\*************************************************************************/
VOID ThreadProc ( LPVOID *Color)
{
  RECT rect;
  HDC  hDC;
  HANDLE hBrush;
  DWORD width, height, Col, Row;

  hBrush = CreateSolidBrush((COLORREF)*Color);

  do
    {
     WaitForSingleObject (hSemaphore, INFINITE);
     GetClientRect( hWnd, &rect);

     width =  rect.right / 4;
     height = rect.bottom / 4;
     Col = (rect.right / 16)  * 6;
     Row = (rect.bottom / 16) * 6;

     hDC = GetDC(hWnd);
     SelectObject( hDC, hBrush);
     Rectangle (hDC, Col, Row, Col + width, Row + height);
     ReleaseDC(hWnd, hDC);
     Sleep (500);
     ReleaseSemaphore (hSemaphore, 1, NULL);

    }while(1);
}
