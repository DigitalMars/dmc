/********************************************************************\
*  Julie Solon                                                       *
*  Microsoft Developer Support                                       *
*  Copyright (c) 1992-1995 Microsoft Corporation                          *
*                                                                    *
*  Comments:                                                         *
*     Demonstrates a Win16 app (app16.exe) calling routines in a     *
*     Win32 DLL (dll32.dll) on Win32s and Windows NT. On Win32s, the *
*     user has a choice of Reverse Universal Thunks or WM_COPYDATA.  *
*     On Windows NT, the user has a choice of Generic Thunks or      *
*     WM_COPYDATA.
*                                                                    *
*  Modules in sample:                                                *
*     app16.c    -  Win16 Application (this file)                    *
*     dll16.c    -  16-bit side of Universal Thunk/Generic Thunk     *
*     stub32.c   -  32-bit stub that loads the 32-bit DLLs on Win32s *
*     utdll32.c  -  32-bit side of the Universal Thunk               *
*     disp32.c   -  Dispatch calls sent through WM_COPYDATA (both    *
*                   platforms)                                       *
*     dll32.c    -  Win32 DLL                                        *
*                                                                    *
*  Functions in this module:                                         *
*     WinMain()                                                      *
*     MainWndProc()                                                  *
*     InfoDlgProc()                                                  *
*     AboutDlgProc()                                                 *
*                                                                    *
\********************************************************************/

/*********************  Header Files  *********************/

#include <windows.h>
#include <stdlib.h>
#include "app16.h"
#include "myut.h"

/*********************  Definitions   *********************/

typedef struct tagCOPYDATASTRUCT
{
   DWORD dwData;
   DWORD cbData;
   VOID FAR *lpData;
} COPYDATASTRUCT, FAR *LPCOPYDATASTRUCT;

#define WM_COPYDATA 0x004A

typedef struct tagMYREC
{
   char  s1[80];
   char  s2[80];
   DWORD n;
} MYREC;

/*********************  Globals       *********************/

HANDLE ghWnd;
HINSTANCE ghInstance;

char  szFirstName[80];
char  szLastName[80];
DWORD nAge;

/*********************  Prototypes    *********************/

LRESULT CALLBACK MainWndProc( HWND, UINT, WPARAM, LPARAM );
BOOL CALLBACK InfoDlgProc( HWND, UINT, WPARAM, LPARAM );
BOOL CALLBACK AboutDlgProc( HWND, UINT, WPARAM, LPARAM );

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
      wc.lpszClassName = "App16Class";
      wc.lpfnWndProc = MainWndProc;
      wc.style = CS_OWNDC | CS_VREDRAW | CS_HREDRAW;
      wc.hInstance = hInstance;
      wc.hIcon = LoadIcon( hInstance, "App16Icon" );
      wc.hCursor = LoadCursor( NULL, IDC_ARROW );
      wc.hbrBackground = (HBRUSH)( COLOR_WINDOW+1 );
      wc.lpszMenuName = "App16Menu";
      wc.cbClsExtra = 0;
      wc.cbWndExtra = 0;

      RegisterClass( &wc );
   }

   hWnd = CreateWindow( "App16Class",
                        "App16",
                        WS_OVERLAPPEDWINDOW|WS_HSCROLL|WS_VSCROLL,
                        0,
                        0,
                        CW_USEDEFAULT,
                        CW_USEDEFAULT,
                        NULL,
                        NULL,
                        hInstance,
                        NULL
          );

   ghWnd = hWnd;
   ghInstance = hInstance;

   ShowWindow( hWnd, nCmdShow );

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
*           WM_COMMAND                                               *
*           WM_DESTROY                                               *
*                                                                    *
*                                                                    *
\********************************************************************/

LRESULT CALLBACK MainWndProc( HWND hWnd, UINT msg, WPARAM wParam,
   LPARAM lParam )
{
   COPYDATASTRUCT MyCDS;
   MYREC MyRec;
   HWND hwDispatch;

   switch( msg ) {

/**************************************************************\
*     WM_COMMAND: Process menu commands                        *
\**************************************************************/

      case WM_COMMAND:
         switch( wParam ) {
            case IDM_THUNK:

            /* Get data from user (stored in globals) */

               DialogBox( ghInstance, "InfoDlg", hWnd, (DLGPROC)
                             InfoDlgProc );

            /* Call routine (through thunk) */

               MyPrint( (LPSTR) szFirstName,
                        (LPSTR) szLastName,
                        (DWORD) nAge );
               break;

            case IDM_COPYDATA:

            /* Get data from user (stored in globals) */

               DialogBox( ghInstance, "InfoDlg", hWnd, (DLGPROC)
                             InfoDlgProc );

            /* Copy data into structure to be passed via WM_COPYDATA */

               lstrcpy( MyRec.s1, szFirstName );
               lstrcpy( MyRec.s2, szLastName );
               MyRec.n = nAge;

            /* Fill the COPYDATA structure */

               MyCDS.dwData = MYPRINT;          // function identifier
               MyCDS.cbData = sizeof( MyRec );  // size of data
               MyCDS.lpData = &MyRec;           // data structure

            /* Call function through identifier, passing data in &MyCDS */

               hwDispatch = FindWindow( "Disp32Class", "Hidden Window" );
               if( hwDispatch != NULL )
                  SendMessage( hwDispatch,
                               WM_COPYDATA,
                               (WPARAM)(HWND) hWnd,
                               (LPARAM) (VOID FAR *) &MyCDS );
               else
                  MessageBox( hWnd, "Can't send WM_COPYDATA", "app16", 
                              MB_OK );
               break;
            case IDM_EXIT:
               SendMessage( hWnd, WM_CLOSE, 0, 0 );
               break;
            case IDM_ABOUT:
               DialogBox( ghInstance, "AboutDlg", hWnd, (DLGPROC)
                             AboutDlgProc );
               break;
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

/********************************************************************\
* Function: BOOL CALLBACK InfoDlgProc(HWND, UINT, WPARAM, LPARAM)    *
*                                                                    *
*  Purpose: Gets information for data record to be sent              *
*                                                                    *
* Comments: The dialog box is displayed when the user selects        *
*           Send Data.Thunk or Send Data.WM_COPYDATA.                *
*                                                                    *
\********************************************************************/


BOOL CALLBACK InfoDlgProc( HWND hDlg, UINT uMsg, WPARAM wParam,
                              LPARAM lParam )
{
   char szAge[5];

   switch( uMsg ) {
      case WM_INITDIALOG:
         szFirstName[0] = '\0';
         szLastName[0] = '\0';
         nAge = 0;
         return TRUE;
      case WM_COMMAND:
         switch( wParam ) {
            case ID_OK:
               SendMessage( GetDlgItem(hDlg, IDC_FIRSTNAME), WM_GETTEXT,
                  sizeof(szFirstName), (LPARAM) ((LPSTR) szFirstName) );
               SendMessage( GetDlgItem(hDlg, IDC_LASTNAME), WM_GETTEXT,
                  sizeof(szLastName), (LPARAM) ((LPSTR) szLastName) );
               SendMessage( GetDlgItem(hDlg, IDC_AGE), WM_GETTEXT,
                  sizeof(szAge), (LPARAM) ((LPSTR) szAge) );
               nAge = atoi( szAge );
               EndDialog( hDlg, TRUE );
               return TRUE;
            case ID_CANCEL:
               EndDialog( hDlg, FALSE );
               return TRUE;
         }
      break;
   }
   return FALSE;
}

/********************************************************************\
* Function: BOOL CALLBACK AboutDlgProc(HWND, UINT, WPARAM, LPARAM)   *
*                                                                    *
*  Purpose: Processes "About" Dialog Box Messages                    *
*                                                                    *
* Comments: The dialog box is displayed when the user selects        *
*           Help.About.                                              *
*                                                                    *
\********************************************************************/


BOOL CALLBACK AboutDlgProc( HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
   switch( uMsg ) {
      case WM_INITDIALOG:
         return TRUE;
      case WM_COMMAND:
         switch( wParam ) {
            case IDOK:
               EndDialog( hDlg, TRUE );
               return TRUE;
         }
      break;
   }
   return FALSE;
}
