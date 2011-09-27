/********************************************************************\
*                                                                    *
*  File:      stub32.c                                               *
*                                                                    *
*  Comments:  Loads the 32-bit side of the Universal Thunk           *
*             (utdll32.dll) and calls the initialization routine     *
*                                                                    *
*  Functions: WinMain                                                *
*             MainWndProc                                            *
*                                                                    *
\********************************************************************/

/*********************  Header Files  *********************/

#include <windows.h>
#include "myut.h"

/*********************  Globals     ***********************/

BOOL fWin32s;
HINSTANCE ghDLL;

/*********************  Prototypes  ***********************/

LRESULT CALLBACK MainWndProc( HWND, UINT, WPARAM, LPARAM );

void WINAPI InitThunk( HWND );


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
      wc.lpszClassName = "Stub32Class";
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

   hWnd = CreateWindow( "Stub32Class",
			"Hidden Window",
			WS_OVERLAPPEDWINDOW,
			0,
			0,
			CW_USEDEFAULT,
			CW_USEDEFAULT,
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
*           WM_CREATE                                                *
*           WM_DESTROY                                               *
*                                                                    *
*                                                                    *
\********************************************************************/


LRESULT CALLBACK MainWndProc( HWND hWnd, UINT msg, WPARAM wParam,
   LPARAM lParam )
{
   switch( msg ) {

/**************************************************************\
*     WM_CREATE: Let the 32-bit UT DLL initialize the thunk    *
\**************************************************************/

      case WM_CREATE:
         InitThunk( hWnd );
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
