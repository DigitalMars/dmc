/********************************************************************\
*                                                                    *
*  File:      dll32.c                                                *
*                                                                    *
*  Comments:  Win32 DLL                                              *
*                                                                    *
*  Functions: DllMain                                                *
*             MyPrint                                                *
*                                                                    *
*                                                                    *
\********************************************************************/

/*********************  Header Files  *********************/

#include <windows.h>
#include <stdio.h>

/********************************************************************\
* Function: BOOL WINAPI DllMain( HANDLE, DWORD, LPVOID )             *
*                                                                    *
*  Purpose: DLL entry point                                          *
*                                                                    *
* Comments: Standard template                                        *
*                                                                    *
*                                                                    *
\********************************************************************/

BOOL WINAPI DllMain( HANDLE hDLL, DWORD dwReason, LPVOID lpReserved )
{
   switch (dwReason)
   {
      case DLL_PROCESS_ATTACH:
         break;

      case DLL_THREAD_ATTACH:
         break;

      case DLL_THREAD_DETACH:
         break;

      case DLL_PROCESS_DETACH:
         break;
   }

   return TRUE;
}

/********************************************************************\
* Function: void WINAPI MyPrint( LPSTR, LPSTR, DWORD )               *
*                                                                    *
*  Purpose: Print the strings passed in as arguments                 *
*                                                                    *
* Comments: This simple routine is called by app16.exe in order to   *
*           demonstrate calling a routine in a Win32 DLL from a      *
*           Win16 application.                                       *
*                                                                    *
*                                                                    *
\********************************************************************/

void WINAPI MyPrint( LPSTR lpString1, LPSTR lpString2, DWORD dwValue )
{
   char buf[200];

   wsprintf( buf, "Data received:\n\n%s\n%s\n%d\n", lpString1, lpString2,
                 dwValue );
   MessageBox( NULL, buf, "Dll32", MB_OK|MB_SETFOREGROUND );
}
