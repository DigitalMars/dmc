// ************************************************************************
//
//                      Microsoft Developer Support
//             Copyright (c) 1992-1995 Microsoft Corporation
//
// ************************************************************************
// MODULE    : Reboot.C
// PURPOSE   : A Reboot Win32 Console Application Source File
// FUNCTIONS :
//   main            - application-defined entry point
//   DisplayVersion  - displays the application's version
//   DisplaySyntax   - displays the application's command line syntax
//   DisplayError    - displays an API error message
// COMMENTS  : none.
// ************************************************************************
#define   STRICT               // enable strict typing
#include <Windows.H>           // required for all Windows applications
#include <StdIO.H>             // printf()

// internal data
// --------------------------------------------------------------------------
static TCHAR szSourceFileName[] = TEXT(__FILE__);
static TCHAR szApiFailed[]      = TEXT("A Windows API Failed!");

// internal function prototypes
// --------------------------------------------------------------------------
BOOL DisplayError( LPCTSTR, LPCTSTR, LPCTSTR, INT );
BOOL DisplayVersion();
BOOL DisplaySyntax();


// ************************************************************************
// FUNCTION : main( int, char** )
// PURPOSE  : main is called by the true application entry point
//            mainCRTStartup right after the C Run-Time libaries are
//            initialized when the executable is first loaded.  That is,
//            main is the application-defined entry point.
// COMMENTS : none.
// ************************************************************************
int
main( int __argc, char** __argv )
{
  // check if Win32s, if so, display notice and terminate
  if( GetVersion() & 0x80000000 ) {
    MessageBoxA( NULL,
      "This application cannot run on Windows 3.1 or Windows 95.\n"
      "This application will now terminate.",
      "Reboot",
      MB_OK | MB_ICONSTOP | MB_SETFOREGROUND );
    return( 1 );
  }

  if( __argc != 2  || ( __argv[1][0] != '\\' && __argv[1][1] != '\\' )  ) {
    DisplayVersion();
    DisplaySyntax();
    return( 2 );
  }

  printf( "Attempting to locate %s on the network...\n", __argv[1] );

  if( !InitiateSystemShutdown( __argv[1], NULL, 0, TRUE, TRUE ) ) {
    DWORD dwGetLastError;

    switch( dwGetLastError = GetLastError() ) {

      case ERROR_ACCESS_DENIED:
        printf( "You do not have permission to reboot %s.\n", __argv[1] );
        return( dwGetLastError );

      case RPC_S_SERVER_UNAVAILABLE:
        printf( "The machine named %s was not found.\n", __argv[1] );
        return( dwGetLastError );

      default:
        DisplayError( TEXT("InitiateSystemShutdown()"),
          szApiFailed, szSourceFileName, __LINE__ );
        return( dwGetLastError );

    }

  }
  else
    printf( "Found.  Rebooting %s.\n", __argv[1] );

  return( 0 );
}


// ************************************************************************
// FUNCTION : DisplayVersion( )
// PURPOSE  : displays the application's version.
// COMMENTS : none.
// ************************************************************************
BOOL
DisplayVersion( )
{
  printf("Remote Reboot Program, Version 1.00\n");
  printf("Microsoft Developer Support, Developed by Paul Tissue\n");
  printf("Copyright (c) 1992-1995 Microsoft Corp.\n");

  return( TRUE );
}


// ************************************************************************
// FUNCTION : DisplaySyntax( )
// PURPOSE  : displays the application's command line syntax.
// COMMENTS : none.
// ************************************************************************
BOOL
DisplaySyntax( )
{
  printf("\nSyntax: REBOOT \\\\MachineName\n");

  return( TRUE );
}


// ************************************************************************
// FUNCTION : DisplayError( LPCTSTR, LPCTSTR, LPCTSTR, INT )
// PURPOSE  : Displays an error message box with various error information
//            and allows the user to terminate or continue the process.
//            For a Win32 Application, GetLastError and FormatMessage are
//            user to retrieve the last API error code and error message.
// COMMENTS : none.
// ************************************************************************
BOOL
DisplayError( LPCTSTR lpszText, LPCTSTR lpszTitle, LPCTSTR lpszFile,
  INT Line )
{
  #define ERROR_BUFFER_SIZE 512

  static TCHAR Format[] =
    TEXT( "%s\n\n"                    )
    TEXT( "-- Error Information --\n" )
    TEXT( "File : %s\n"               )
    TEXT( "Line : %d\n"               )
    TEXT( "Error Number  : %d\n"      )
    TEXT( "Error Message : %s\n"      );

  LPTSTR lpFormatMessageBuffer;
  DWORD  dwFormatMessage;
  DWORD  dwGetLastError;
  HLOCAL hMessageBuffer;
  LPVOID lpMessageBuffer;

  //-- perform a Reboot check on the needed buffer size
  if( lstrlen(lpszText) > (ERROR_BUFFER_SIZE - lstrlen(Format)) )
    return( FALSE );

  //-- allocate the message box buffer
  hMessageBuffer  = LocalAlloc( LMEM_FIXED, ERROR_BUFFER_SIZE );
  lpMessageBuffer = LocalLock( hMessageBuffer );

  //-- get the system error and system error message
  dwGetLastError = GetLastError();
  dwFormatMessage = FormatMessage(
                      FORMAT_MESSAGE_ALLOCATE_BUFFER
                      | FORMAT_MESSAGE_FROM_SYSTEM,
                      NULL, dwGetLastError, LANG_NEUTRAL,
                      (LPTSTR) &lpFormatMessageBuffer, 0, NULL );
  if( !dwFormatMessage )
    lpFormatMessageBuffer = TEXT("FormatMessage() failed!");

  //-- format the error messge box string
  wsprintf( lpMessageBuffer, Format, lpszText, lpszFile, Line,
     dwGetLastError, lpFormatMessageBuffer );

  // -- display the error and allow the user to terminate or continue
  printf(
    "\n"
    "----------------------------------------\n"
    "\n%s\n%s"
    "----------------------------------------\n",
    lpszTitle, lpMessageBuffer );

  //-- free all buffers
  if( dwFormatMessage )
    LocalFree( (HLOCAL) lpFormatMessageBuffer );
  LocalFree( (HLOCAL) hMessageBuffer );

  return( TRUE );
}
