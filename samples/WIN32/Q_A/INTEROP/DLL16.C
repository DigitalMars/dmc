/********************************************************************\
*                                                                    *
*  File:      dll16.c                                                *
*                                                                    *
*  Comments:  16-bit side of the Universal/Generic Thunk             *
*                                                                    *
*  Functions: LibMain                                                *
*             MyPrint                                                *
*             UTInit                                                 *
*             UTProc                                                 *
*             WEP                                                    *
*                                                                    *
\********************************************************************/

/*********************  Header Files **********************/

#define W32SUT_16

#include <windows.h>
#include "w32sut.h"
#include "myut.h"

/*********************  Defines      **********************/

typedef void (FAR PASCAL *MYPROC)( LPSTR, LPSTR, DWORD );

typedef struct tagWIN32SINFO
{
   BYTE bMajor;
   BYTE bMinor;
   WORD wBuildNumber;
   BOOL fDebug;
} WIN32SINFO, FAR * LPWIN32SINFO;

/*********************  Globals      **********************/

BOOL fWin32s = FALSE;
UT16CBPROC gpUTCBack = NULL;

MYPROC Proc;
DWORD ghLib;

HWND ghWnd = NULL;
HINSTANCE ghInst;

/*********************  Prototypes   **********************/

BOOL FAR PASCAL IsWin32sLoaded( void );

/********************************************************************\
* Function: int FAR PASCAL LibMain( HINSTANCE, WORD, WORD, LPSTR )   *
*                                                                    *
*  Purpose: DLL entry point                                          *
*                                                                    *
* Comments: Loads the Win32 stub application                         *
*                                                                    *
*                                                                    *
\********************************************************************/

int FAR PASCAL LibMain( HINSTANCE hInst, WORD wDataSeg, WORD cbHeapSize, LPSTR lpszCmdLine )
{
   DWORD dwVersion;
   
   ghInst = hInst;

   WinExec( "disp32.exe", SW_SHOW );

   /* Check to see what version we're running on */
   dwVersion = GetVersion();
   if(( GetWinFlags() & WF_WINNT ) || ( HIBYTE(LOWORD(dwVersion)) == 95 )) 
   {
      /* This is either Windows NT or Windows 95 */
      OutputDebugString( "This is either Windows NT or Windows 95\r\n" );
      ghLib = LoadLibraryEx32W( "dll32.dll", NULL, 0 );
   }
   else
   {
      fWin32s = IsWin32sLoaded( );
      if( fWin32s )
         WinExec( "stub32.exe", SW_SHOW );
      else
         return FALSE;
   }
   return TRUE;
}

/********************************************************************\
* Function: DWORD FAR PASCAL UTInit( UT16CBPROC, LPVOID )            *
*                                                                    *
*  Purpose: Universal Thunk initialization function                  *
*                                                                    *
* Comments: Called when UTRegister() is called by utdll32.dll. The   *
*           window handle for the Win32 stub program is obtained and *
*           saved in a global for cleanup purposes later on.         *
*           FreeLibrary() is also called for cleanup purposes, by    *
*           reducing the reference count (the DLL is also referenced *
*           in the 32-bit side of the Universal Thunk through the    *
*           call to UTRegister().                                    *
*                                                                    *
*                                                                    *
\********************************************************************/

DWORD FAR PASCAL UTInit( UT16CBPROC pfnUT16CallBack, LPVOID lpBuff )
{
   gpUTCBack = pfnUT16CallBack;

   ghWnd = (HWND) (*gpUTCBack)( NULL, GETHWND, NULL );

   FreeLibrary( ghInst );

   return TRUE;
}

/********************************************************************\
* Function: DWORD FAR PASCAL UTProc( LPVOID, DWORD )                 *
*                                                                    *
*  Purpose: Universal Thunk 16-bit procedure                         *
*                                                                    *
* Comments: Unused                                                   *
*                                                                    *
*                                                                    *
\********************************************************************/

DWORD FAR PASCAL UTProc( LPVOID lpBuff, DWORD dwFunc )
{
   return FALSE;
}

/********************************************************************\
* Function: void WINAPI MyPrint( LPSTR, LPSTR, DWORD )               *
*                                                                    *
*  Purpose: Implementation of MyPrint()                              *
*                                                                    *
* Comments: Win32s: Implemented as a UT to the 32-bit side, where    *
*           the actual implementation resides. The translation list  *
*           is used to send multiple strings and have their          *
*           addresses translated.                                    *
*                                                                    *
*           Windows NT: Implemented as a GenThunk to the 32-bit side *
*           where the actual implementation resides. The address of  *
*           the function is obtained with GetProcAddress32W() and    *
*           the function is called via CallProc32W().                *
*                                                                    *
*                                                                    *
\********************************************************************/

void WINAPI MyPrint( LPSTR lpString1, LPSTR lpString2, DWORD dwValue )
{
   DWORD Args[3];
   LPVOID TransList[3];

   /*
      The arguments are packed into the Args array to be passed through
      the thunk. The TransList contains any pointers which must be
      translated. We wish to pass dwValue by value, so it is not placed
      in the translation list. The last entry in the translation list
      must be NULL.
   */

   if( fWin32s )
   {
      Args[0] = (DWORD) lpString1;
      Args[1] = (DWORD) lpString2;
      Args[2] = dwValue;

      TransList[0] = &Args[0];
      TransList[1] = &Args[1];
      TransList[2] = NULL;

      (*gpUTCBack)( Args, MYPRINT, TransList );
   }

   /*
      CallProc32W() takes a variable number of parameters. The last
      parameter here indicates that the first three arguments are to
      be passed through the thunk. The next parameter specifies Proc
      as the address of the function to be called. The second to the
      last parameter is a mask that indicates which parameters are
      addresses which should be translated. Note that the mask is
      specified in the opposite order that the parameters are listed,
      so 6 = 0110, indicating that lpString1 and lpString2 should be
      translated.
   */

   else
   {
      Proc = (MYPROC) GetProcAddress32W( ghLib, "MyPrint" );
      CallProc32W( (DWORD) lpString1, (DWORD) lpString2,
         (DWORD) dwValue, Proc, 6, 3 );
   }
}

/********************************************************************\
* Function: BOOL FAR PASCAL IsWin32sLoaded( );                       *
*                                                                    *
*  Purpose: Test if Win32s is loaded.                                *
*                                                                    *
* Comments: GetWin32sInfo.w32sys is used to verify if either Win32s  *
*           version 1.0 or 1.1 is loaded.                            *
*                                                                    *
*                                                                    *
\********************************************************************/

BOOL FAR PASCAL IsWin32sLoaded( )
{
   BOOL       fWin32sLoaded = FALSE;
   HANDLE     hWin32sys;
   FARPROC    pfnInfo;
   WIN32SINFO Info;

   hWin32sys = LoadLibrary( "w32sys.dll" );

   if( hWin32sys > HINSTANCE_ERROR )
   {
      pfnInfo = GetProcAddress( hWin32sys, "GETWIN32SINFO" );
      if( pfnInfo )
      {
         if(!(*pfnInfo)((LPWIN32SINFO) &Info))
            fWin32sLoaded = TRUE;
         else fWin32sLoaded = FALSE;
      }
      else fWin32sLoaded = TRUE;
      FreeLibrary( hWin32sys );
   }

   return fWin32sLoaded;
}

/********************************************************************\
* Function: int FAR PASCAL WEP ( int )                               *
*                                                                    *
*  Purpose: DLL exit procedure                                       *
*                                                                    *
* Comments: Win32s: Call into Win32 stub program to force cleanup of *
*           Win32 DLLs.                                              *
*                                                                    *
*           Windows NT: Free dll32.dll and terminate disp32.exe      *
*                                                                    *
*                                                                    *
\********************************************************************/

int FAR PASCAL WEP( int nParam )
{
   SendMessage( FindWindow("Disp32Class", "Hidden Window"),
                WM_CLOSE, 0, 0 );

   if( fWin32s )
   {
      PostMessage( ghWnd, WM_CLOSE, 0, 0 );
   }
   else
   {
      FreeLibrary32W( ghLib );
   }
   return TRUE;
}
