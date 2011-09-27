
/******************************************************************************\
*       This is a part of the Microsoft Source Code Samples. 
*       Copyright (C) 1992-1995 Microsoft Corporation.
*       All rights reserved. 
*       This source code is only intended as a supplement to 
*       Microsoft Development Tools and/or WinHelp documentation.
*       See these sources for detailed information regarding the 
*       Microsoft samples programs.
\******************************************************************************/

/********************************************************************\
*  world.c -- Sample program demonstrating scaling and translating   *
*             an image from a metafile with World Coordinate         *
*             Transforms.                                            *
*                                                                    *
*  Comments:                                                         *
*                                                                    *
*  The application loads the specified metafile. The user can        *
*  scale the image through a menu option and translate it via        *
*  the scrollbars and arrow keys.                                    *
*                                                                    *
*  Functions:                                                        *
*                                                                    *
*  WinMain()         - Initializes Application                       *
*  MainWndProc()     - Processes Application Messages                *
*  ScaleDlgProc()    - Processes "Scale Image" Dialog Box Messages   *
*  AboutDlgProc()    - Processes "About" Dialog Box Messages         *
*  OpenMetaFile()    - Gets Metafile name and opens the Metafile     *
*  SetUnityXform()   - Helper routine which sets the unity transform *
*                                                                    *
*                                                                    *
\********************************************************************/


/*********************  Header Files  *********************/

#include <windows.h>
#include <memory.h>
#include <stdlib.h>
#include "world.h"


/**********************  Defines  *************************/

#define VSCROLLMAX  50
#define HSCROLLMAX  50
#define VERTSCALE   VSCROLLMAX/3
#define HORZSCALE   HSCROLLMAX/3

#define LINEINC     1
#define PAGEINC     4

#define FIRSTSTRING 0
#define LASTSTRING  10
#define MAXINPUT    10

/*********************  Prototypes  ***********************/

LRESULT CALLBACK MainWndProc( HWND, UINT, WPARAM, LPARAM );
BOOL CALLBACK AboutDlgProc( HWND, UINT, WPARAM, LPARAM );
BOOL CALLBACK ScaleDlgProc( HWND, UINT, WPARAM, LPARAM );
BOOL OpenMetafile( HWND );
void SetUnityXform( VOID );

/*******************  Global Variables ********************/

HANDLE ghInstance;

HENHMETAFILE hEMF;  /* Handle of metafile opened                  */
XFORM xForm;        /* Transform to apply in SetWorldTransform()  */
BOOL bFileOpen;     /* Logs if a metafile has been opened or not  */
BOOL bFileJustOpen; /* Used to re-initialize "Scale Image" values */

/* Used for putting up File Open common dialog */

OPENFILENAME OpenFileName;
CHAR szFile[MAX_PATH];
CHAR szFileTitle[MAX_PATH];
CHAR szFilter[] = "Metafile (*.EMF)\0*.emf\0All Files (*.*)\0*.*\0";


/********************************************************************\
*  Function: int APIENTRY WinMain(HINSTANCE, HINSTANCE, LPSTR, int)  *
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

   // check if running on Windows NT, if not, display notice and terminate
   if( GetVersion() & 0x80000000 )
   {
      MessageBoxA( NULL,
         "This sample application can only be run on Windows NT.\n"
         "This application will now terminate.",
         "World",
         MB_OK | MB_ICONSTOP | MB_SETFOREGROUND );
      return( 1 );
   }

   if( !hPrevInstance ) {
      wc.lpszClassName = "WorldClass";
      wc.lpfnWndProc = MainWndProc;
      wc.style = CS_OWNDC | CS_VREDRAW | CS_HREDRAW;
      wc.hInstance = hInstance;
      wc.hIcon = LoadIcon( hInstance, "WorldIcon" );
      wc.hCursor = LoadCursor( NULL, IDC_ARROW );
      wc.hbrBackground = (HBRUSH)( COLOR_WINDOW+1 );
      wc.lpszMenuName = "WorldMenu";
      wc.cbClsExtra = 0;
      wc.cbWndExtra = 0;

      RegisterClass( &wc );
   }

   ghInstance = hInstance;

   hWnd = CreateWindow( "WorldClass",
                        "World Coordinate Transforms",
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
*           WM_CREATE                                                *
*           WM_HSCROLL                                               *
*           WM_VSCROLL                                               *
*           WM_KEYDOWN                                               *
*           WM_PAINT                                                 *
*           WM_COMMAND                                               *
*           WM_DESTROY                                               *
*                                                                    *
*                                                                    *
\********************************************************************/


LRESULT CALLBACK MainWndProc( HWND hWnd, UINT msg, WPARAM wParam,
   LPARAM lParam )
{
   static int nVscrollPos, nHscrollPos;
   int nVscrollInc, nHscrollInc;
   RECT rect;
   PAINTSTRUCT ps;
   HDC hDC;

   switch( msg ) {

/**************************************************************\
*     WM_CREATE: The scrollbar range is set. Global variables  *
*        are initialized. The new API SetGraphicsMode() must   *
*        be called before SetWorldCoordinates() can be called  *
*        in the WM_PAINT case.                                 *
\**************************************************************/

      case WM_CREATE:

         SetScrollRange( hWnd, SB_HORZ, 0, HSCROLLMAX, FALSE );
         SetScrollRange( hWnd, SB_VERT, 0, VSCROLLMAX, FALSE );

         bFileOpen = FALSE;
         bFileJustOpen = FALSE;

         hDC = GetDC( hWnd );
         SetGraphicsMode( hDC, GM_ADVANCED );
         ReleaseDC( hWnd, hDC );

         break;

/**************************************************************\
*     WM_HSCROLL: Standard template, except that instead of    *
*        calling ScrollWindow(), the transform is updated.     *
\**************************************************************/

      case WM_HSCROLL:
         switch( wParam ) {
            case SB_LINEUP:
               nHscrollInc = -LINEINC;
               break;
            case SB_LINEDOWN:
               nHscrollInc = LINEINC;
               break;
            case SB_PAGEUP:
               nHscrollInc = -PAGEINC;
               break;
            case SB_PAGEDOWN:
               nHscrollInc = PAGEINC;
               break;
            case SB_THUMBPOSITION:
               nHscrollInc = HIWORD(wParam) - nHscrollPos;
               break;
            default:
               nHscrollInc = 0;
         }
         if(nHscrollInc=max(-nHscrollPos,min(nHscrollInc,HSCROLLMAX-
               nHscrollPos))) {
            nHscrollPos += nHscrollInc;
            SetScrollPos( hWnd, SB_HORZ, nHscrollPos, TRUE );

            xForm.eDx += (FLOAT) (nHscrollInc * HORZSCALE);
            InvalidateRect( hWnd, NULL, TRUE );
         }
         return 0;

/**************************************************************\
*     WM_VSCROLL: Standard template, except that instead of    *
*        calling ScrollWindow(), the transform is updated.*    *
\**************************************************************/

      case WM_VSCROLL:
         switch( wParam ) {
            case SB_TOP:
               nVscrollInc = -nVscrollPos;
               break;
            case SB_BOTTOM:
               nVscrollInc = VSCROLLMAX - nVscrollPos;
               break;
            case SB_LINEUP:
               nVscrollInc = -LINEINC;
               break;
            case SB_LINEDOWN:
               nVscrollInc = LINEINC;
               break;
            case SB_PAGEUP:
               nVscrollInc = -PAGEINC;
               break;
            case SB_PAGEDOWN:
               nVscrollInc = PAGEINC;
               break;
            case SB_THUMBTRACK:
               nVscrollInc = HIWORD(wParam) - nVscrollPos;
               break;
            default:
               nVscrollInc = 0;
         }
         if(nVscrollInc=max(-nVscrollPos,min(nVscrollInc,VSCROLLMAX-
               nVscrollPos))) {
            nVscrollPos += nVscrollInc;
            SetScrollPos( hWnd, SB_VERT, nVscrollPos, TRUE );

            xForm.eDy += (FLOAT) (nVscrollInc * VERTSCALE);
            InvalidateRect( hWnd, NULL, TRUE );
         }
         return 0;

/**************************************************************\
*     WM_KEYDOWN: Standard template                            *
\**************************************************************/

      case WM_KEYDOWN:
         switch( wParam ) {
            case VK_UP:
               SendMessage( hWnd, WM_VSCROLL, SB_LINEUP, 0L );
               break;
            case VK_DOWN:
               SendMessage( hWnd, WM_VSCROLL, SB_LINEDOWN, 0L );
               break;
            case VK_LEFT:
               SendMessage( hWnd, WM_HSCROLL, SB_LINEUP, 0L );
               break;
            case VK_RIGHT:
               SendMessage( hWnd, WM_HSCROLL, SB_LINEDOWN, 0L );
               break;
            case VK_PRIOR:
               SendMessage( hWnd, WM_VSCROLL, SB_PAGEUP, 0L );
               break;
            case VK_NEXT:
               SendMessage( hWnd, WM_VSCROLL, SB_PAGEDOWN, 0L );
               break;
         }
         return 0;

/**************************************************************\
*     WM_PAINT: If a Metafile has been opened, the transform   *
*        is applied using SetWorldTransform() and the Metafile *
*        is displayed using PlayEnhMetaFile().                 *
\**************************************************************/

      case WM_PAINT:
         hDC = BeginPaint( hWnd, &ps );

         if( bFileOpen ) {
            GetClientRect( hWnd, &rect );
            SetWorldTransform( hDC, &xForm );
            PlayEnhMetaFile( hDC, hEMF, &rect );
         }

         EndPaint( hWnd, &ps );
         break;

/**************************************************************\
*     WM_COMMAND: If Metafile.Open is selected, OpenMetafile   *
*        and InvalidateRect() are called and the thumb         *
*        position is centered. If Metafile.Exit is selected,   *
*        a WM_CLOSE message is sent. If Scale is chosen, then  *
*        the "Scale Image" dialog is displayed; if the user    *
*        choses OK, then InvalidateRect() is called. If        *
*        Help.About is chosen, then the "About" dialog box is  *
*        displayed.                                            *
\**************************************************************/

      case WM_COMMAND:
         switch( wParam ) {
            case IDM_OPEN:
               if( OpenMetafile( hWnd ) ) {
                  SetWindowText( hWnd, OpenFileName.lpstrFile );
                  SetUnityXform( ); 
                  nHscrollPos = HSCROLLMAX/2;
                  nVscrollPos = VSCROLLMAX/2;
                  SetScrollPos( hWnd, SB_HORZ, nHscrollPos, TRUE );
                  SetScrollPos( hWnd, SB_VERT, nVscrollPos, TRUE );

                  InvalidateRect( hWnd, NULL, TRUE );
               }
               break;
            case IDM_EXIT:
               SendMessage( hWnd, WM_CLOSE, 0, 0 );
               break;
            case IDM_SCALE:
               if( DialogBox( ghInstance, "ScaleDlg", hWnd,                
                     (DLGPROC) ScaleDlgProc ) )
                  InvalidateRect( hWnd, NULL, TRUE );
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
* Function: BOOL CALLBACK ScaleDlgProc(HWND, UINT, WPARAM, LPARAM)   *
*                                                                    *
*  Purpose: Processes "Scale Image" Dialog Box Messages.             *
*                                                                    *
* Comments: The following messages are processed:                    *
*                                                                    *
*           WM_INITDIALOG                                            *
*           WM_COMMAND                                               *
*                                                                    *
\********************************************************************/


BOOL CALLBACK ScaleDlgProc( HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
   char buffer[MAXINPUT];
   static char HorzInputBuf[MAXINPUT];
   static char VertInputBuf[MAXINPUT];
   int index;

   switch( uMsg ) {

/**************************************************************\
*     WM_INITDIALOG: Strings representing the possible scale   *
*        factors are loaded into the comboboxes. The current   *
*        selection is indicated.                               *
\**************************************************************/

      case WM_INITDIALOG:
         for( index=FIRSTSTRING; index <= LASTSTRING; index++ ) {
            LoadString( GetModuleHandle(NULL), index, buffer, MAXINPUT);
            SendDlgItemMessage( hDlg, IDC_HORZ, CB_ADDSTRING, 0, 
                                 (LPARAM)buffer );
            SendDlgItemMessage( hDlg, IDC_VERT, CB_ADDSTRING, 0, 
                                 (LPARAM)buffer );
         }

         if( bFileJustOpen || !bFileOpen ) {
            lstrcpy( HorzInputBuf, " 1.0" );
            lstrcpy( VertInputBuf, " 1.0" );
            bFileJustOpen = FALSE;
         } 

         SendDlgItemMessage( hDlg, IDC_HORZ, CB_SELECTSTRING, 0,  
                              (LPARAM)HorzInputBuf );
         SendDlgItemMessage( hDlg, IDC_VERT, CB_SELECTSTRING, 0, 
                              (LPARAM)VertInputBuf );
         return TRUE;

/**************************************************************\
*     WM_COMMAND: If the OK button is selected, the current    *
*        horizontal and vertical scaling factors are obtained  *
*        from the ComboBox and the transform is updated. If    *
*        the CANCEL button is selected, EndDialog() is called. *
\**************************************************************/

      case WM_COMMAND:
         switch( wParam ) {
            case IDOK:
               index = SendDlgItemMessage( hDlg, IDC_HORZ, CB_GETCURSEL, 
                                            0, 0 );
               LoadString( GetModuleHandle(NULL), index, HorzInputBuf, 
                            MAXINPUT );
               xForm.eM11 = (FLOAT) atof( HorzInputBuf );

               index = SendDlgItemMessage( hDlg, IDC_VERT, CB_GETCURSEL, 
                                            0, 0 );
               LoadString( GetModuleHandle(NULL), index, VertInputBuf, 
                            MAXINPUT );
               xForm.eM22 = (FLOAT) atof( VertInputBuf );

               EndDialog( hDlg, TRUE );
               return TRUE;
            case IDCANCEL:
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
* Comments: The Dialog Box is displayed when the user selects        *
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


/********************************************************************\
* Function: BOOL OpenMetafile( HWND )                                *
*                                                                    *
*  Purpose: Gets Metafile name and opens the Metafile                *
*                                                                    *
* Comments: Uses the Common Dialog GetOpenFileName() to prompt user  *
*           for the name of the Metafile to be opened.  If the name  *
*           is successfully retrieved, GetEnhMetaFile() is used to   *
*           open the Metafile.                                       *
*                                                                    *
\********************************************************************/


BOOL OpenMetafile( HWND hWnd )
{
   szFile[0] = '\0';
   szFileTitle[0] = '\0';
   memset( (LPVOID) &OpenFileName, 0, sizeof( OPENFILENAME ));

   OpenFileName.lStructSize = sizeof( OPENFILENAME );
   OpenFileName.hwndOwner = hWnd;
   OpenFileName.lpstrFilter = szFilter;
   OpenFileName.lpstrCustomFilter = (LPSTR) NULL;
   OpenFileName.nMaxCustFilter = 0L;
   OpenFileName.nFilterIndex = 1L;
   OpenFileName.lpstrFile = szFile;
   OpenFileName.nMaxFile = sizeof( szFile );
   OpenFileName.lpstrFileTitle = szFileTitle;
   OpenFileName.nMaxFileTitle = sizeof( szFileTitle );
   OpenFileName.lpstrInitialDir = NULL;
   OpenFileName.lpstrTitle = "Open Metafile";
   OpenFileName.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
   OpenFileName.nFileOffset = 0;
   OpenFileName.nFileExtension = 0;
   OpenFileName.lpstrDefExt = "*.EMF";

   if( !GetOpenFileName( &OpenFileName ) )
      return FALSE;

   hEMF = GetEnhMetaFile( OpenFileName.lpstrFile );
   bFileOpen = TRUE;
   bFileJustOpen = TRUE;

   return TRUE;
}


/********************************************************************\
* Function: void SetUnityXform( VOID )                               *
*                                                                    *
*  Purpose: Helper routine which sets the unity transform            *
*                                                                    *
* Comments: These fields have the following use in this application: *
*                                                                    *
*                  eM11: Horizontal Scaling Factor                   *
*                  eM12: Not Used                                    *
*                  eM21: Not Used                                    *
*                  eM22: Vertical Scaling Factor                     *
*                  eDx: Horizontal Translation                       *
*                  eDy: Vertical Translation                         *
*                                                                    *
\********************************************************************/


void SetUnityXform( )
{
   xForm.eM11 = (FLOAT) 1.0;
   xForm.eM12 = (FLOAT) 0.0;
   xForm.eM21 = (FLOAT) 0.0;
   xForm.eM22 = (FLOAT) 1.0;
   xForm.eDx  = (FLOAT) 0.0;
   xForm.eDy  = (FLOAT) 0.0;
}
