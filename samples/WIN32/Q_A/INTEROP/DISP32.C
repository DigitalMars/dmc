/********************************************************************\
*                                                                    *
*  File:      disp32.c                                               *
*                                                                    *
*  Comments:  Dispatches function calls made through WM_COPYDATA on  *
*             both Windows NT and Win32s.                            *
*                                                                    *
*  Functions: WinMain                                                *
*             MainWndProc                                            *
*                                                                    *
\********************************************************************/

/*********************  Header Files  *********************/

#include <windows.h>
#include "myut.h"

/*********************  Definitions ***********************/

typedef struct tagMYREC
{
   char  s1[80];
   char  s2[80];
   DWORD n;
} MYREC;

/*********************  Prototypes  ***********************/

LRESULT CALLBACK MainWndProc( HWND, UINT, WPARAM, LPARAM );

void WINAPI MyPrint( LPSTR, LPSTR, DWORD );

/********************************************************************\
*  Function: int PASCAL WinMain(HINSTANCE, HINSTANCE, LPSTR, int)    *
*                                                                    *
*   Purpose: Initializes Application                                 *
*                                                                    *
*  Comments: Standard template                                       *
*                                                                    *
*                                                                    *
\********************************************************************/


int PASCAL WinMain( HINSTANCE hInstance,
                    HINSTANCE hPrevInstance,
                    LPSTR lpszCmdLine,
                    int nCmdShow )
{
   WNDCLASS wc;
   MSG msg;
   HWND hWnd;

   if( !hPrevInstance ) {
      wc.lpszClassName = "Disp32Class";
      wc.lpfnWndProc = MainWndProc;
      wc.style = CS_OWNDC | CS_VREDRAW | CS_HREDRAW;
      wc.hInstance = hInstance;
      wc.hIcon = LoadIcon( NULL, IDI_APPLICATION );
      wc.hCursor = LoadCursor( NULL, IDC_ARROW );
      wc.hbrBackground = (HBRUSH)( COLOR_WINDOW+1 );
      wc.lpszMenuName = NULL;
      wc.cbClsExtra = 0;
      wc.cbWndExtra = 0;

      RegisterClass( &wc );
   }

   hWnd = CreateWindow( "Disp32Class",
                        "Hidden Window",
                        WS_OVERLAPPEDWINDOW,
                        0,
                        0,
                        GetSystemMetrics(SM_CXMIN),
                        GetSystemMetrics(SM_CYMIN),
                        NULL,
                        NULL,
                        hInstance,
                        NULL
          );

   ShowWindow( hWnd, SW_HIDE );

   while( GetMessage( &msg, NULL, 0, 0 ) ) {
      TranslateMessage( &msg );
      DispatchMessage( &msg );
   }

   return msg.wParam;
}


/********************************************************************\
* Function: LRESULT CALLBACK MainWndProc(HWND, UINT, WPARAM, LPARAM) *
*                                                                    *
*  Purpose: Processes Application Messages                           *
*                                                                    *
* Comments: The following messages are processed                     *
*                                                                    *
*           WM_DESTROY                                               *
*                                                                    *
*                                                                    *
\********************************************************************/


LRESULT CALLBACK MainWndProc( HWND hWnd, UINT msg, WPARAM wParam,
   LPARAM lParam )
{
   PCOPYDATASTRUCT pMyCDS;

   switch( msg ) {

/**************************************************************\
*     WM_COPYDATA: Dispatch calls to dll32.dll. The switch     *
*        statement is not strictly necessary here, since there *
*        is only one function thunked, but it is used to       *
*        demonstrate how you would dispatch more than one      *
*        function call.                                        *
\**************************************************************/

      case WM_COPYDATA:
         pMyCDS = (PCOPYDATASTRUCT) lParam;

         switch( pMyCDS->dwData )
         {
            case MYPRINT:
               MyPrint( (LPSTR) ((MYREC *)(pMyCDS->lpData))->s1,
                        (LPSTR) ((MYREC *)(pMyCDS->lpData))->s2,
                        (DWORD) ((MYREC *)(pMyCDS->lpData))->n );
         }
         break;

/**************************************************************\
*     WM_DESTROY: PostQuitMessage() is called                  *
\**************************************************************/

      case WM_DESTROY:
         PostQuitMessage( 0 );
         break;

/**************************************************************\
*     Let the default window proc handle all other messages    *
\**************************************************************/

      default:
         return( DefWindowProc( hWnd, msg, wParam, lParam ));
   }

   return 0;
}
