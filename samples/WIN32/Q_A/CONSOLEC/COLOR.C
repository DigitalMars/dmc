
/******************************************************************************\
*       This is a part of the Microsoft Source Code Samples. 
*       Copyright (C) 1992-1995 Microsoft Corporation.
*       All rights reserved. 
*       This source code is only intended as a supplement to 
*       Microsoft Development Tools and/or WinHelp documentation.
*       See these sources for detailed information regarding the 
*       Microsoft samples programs.
\******************************************************************************/

/************************************************************************\
*
* PROGRAM   : COLOR.C
*
* PURPOSE   : To demonstrate the use of the SetConsoleTextAttribute()
*             and GetConsoleScreenBufferInfo() APIs to set the console
*             text color attributes.
*
* FUNCTIONS : main()                     - Open the console, parse args,
*                                          and change console colors
*             Syntax()                   - Display the command syntax
*                                          and color table if no args.
*             GetConsoleTextAttribute()  - Get the previous console
*                                          color attributes
*
* COMMENTS  : The open handle to the console is obtained by specifying
*             CONOUT$ as the file name in the CreateFile() API call.
*             The OPEN_EXISTING create flag is used to get the current
*             console handle.
*
\************************************************************************/

#include <Windows.H>
#include <StdIO.H>
#include <StdLib.H>
#include <ConIO.H>
#include <String.H>

#include "Color.H"


/************************************************************************\
*
* FUNCTION    : main
*
* INPUTS      : __argc - argument count
*               __argv - array of argument strings
*
* RETURNS     : 0 on success or -1 on failure.
*
* GLOBAL VARS : Color - array of text color attributes
*
* LOCAL VARS  : ForeColor  - foreground text color
*               BackColor  - background text color
*               ColorIndex - index in the Color array
*               StrLen     - length of a command line argument string
*               hConsole   - handle to the console
*               ForeGround - string buffer containing the first argument
*               BackGround - string buffer containing the second argument
*
* COMMENTS    :
*
\************************************************************************/
int
main( int __argc, char** __argv )
{
  WORD    ForeColor;
  WORD    BackColor;
  int     ColorIndex;
  size_t  StrLen;
  HANDLE  hConsole;
  char    ForeGround[8];
  char    BackGround[8];

  // check if Win32s, if so, display notice and terminate
  if( GetVersion() & 0x80000000 && (GetVersion() & 0xFF) ==3)
  {
    MessageBoxA( NULL,
      "This application cannot run on Windows 3.1.\n"
      "This application will now terminate.",
      "Color",
      MB_OK | MB_ICONSTOP | MB_SETFOREGROUND );
      return( 1 );
  }

  /**********************************************************************\
   * Open the current console input buffer
  \**********************************************************************/
  if( ( hConsole = CreateFile(
                     "CONOUT$", GENERIC_WRITE | GENERIC_READ,
                     FILE_SHARE_READ | FILE_SHARE_WRITE,
                     0L, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0L) )
       == INVALID_HANDLE_VALUE ) {
    printf("\nError: Unable to open console.\n");
    return( -1 );
  }

  ForeColor = 0;
  BackColor = (WORD) (( GetConsoleTextAttribute(hConsole) & BACKGROUND_COLOR ) >> 4);

  if( __argc < 2 ) {
    Syntax( hConsole );
  }

  /**********************************************************************\
   * Get the first argument; the foreground text color
  \**********************************************************************/
  if( __argc > 1 ) {
    sprintf( ForeGround, "%s", __argv[1] );
    StrLen = strlen( ForeGround );
    for( ColorIndex = 0; ColorIndex < MAXCOLORS; ColorIndex++ ) {
      if( strnicmp( ForeGround, Color[ColorIndex], StrLen ) == 0 ) {
        ForeColor = (WORD) ColorIndex;
        break;
      }
    }
    if( ColorIndex == MAXCOLORS ) {
      printf("\nError: Invalid foreground color.\n");
      Syntax( hConsole );
    }
  }

  /**********************************************************************\
   * Get the second (optional) argument; the background text color
  \**********************************************************************/
  if( __argc > 2 ) {
    sprintf( BackGround, "%s", __argv[2] );
    StrLen = strlen( BackGround );
    for( ColorIndex = 0; ColorIndex < MAXCOLORS; ColorIndex++ ) {
      if( strnicmp( BackGround, Color[ColorIndex], StrLen ) == 0 ) {
        BackColor = (WORD) ColorIndex;
        break;
      }
    }
    if( ColorIndex == MAXCOLORS ) {
      printf("\nError: Invalid background color.\n");
      Syntax( hConsole );
    }
  }

  /**********************************************************************\
   * Check if foreground and backgroud colors are the same; issue a error
   * and exit if they are
  \**********************************************************************/
  if( BackColor == ForeColor) {
    printf("\nError: Foreground and background color cannot be the same.\n");
    Syntax( hConsole );
  }

  /**********************************************************************\
   * Set the new console text colors
  \**********************************************************************/
  SetConsoleTextAttribute( hConsole, (WORD) (ForeColor | (BackColor << 4) ) );

  return( 0 );
}


/************************************************************************\
*
* FUNCTION    : Syntax
*
* INPUTS      : hConsole - handle to an open console
*
* RETURNS     : 0 and exits the process.
*
* GLOBAL VARS : Color - array of text color attributes
*
* LOCAL VARS  : ForeColor      - foreground text color
*               BackColor      - background text color
*               wAttributesOld - previous text attributes
*
* COMMENTS    : Displays the command syntax, a text color table and
*               exits the process at completion.
*
\************************************************************************/
BOOL
Syntax( HANDLE hConsole )
{
  WORD ForeColor;
  WORD BackColor;
  WORD wAttributesOld;

  printf("Console Window Text Color Setter, Version 1.00");
  printf("\nMicrosoft Developer Support");
  printf("\nCopyright (c) 1992-1995 Microsoft Corp.");
  printf("\n\nSyntax: COLOR FOREGROUND [BACKGROUND]");
  printf("\n\nAvailable colors are:");
  printf("\n");

  wAttributesOld = GetConsoleTextAttribute( hConsole );

  printf("%36s BACKGROUND\n", " ");
  printf("FOREGROUND  ");
  for( BackColor = 0; BackColor < MAXCOLORS; BackColor++ ) {
    printf("%-8s", Color[BackColor] );
  }
  printf("\n");

  for( ForeColor = 0; ForeColor < MAXCOLORS; ForeColor++ ) {
    printf( "%-10s ",  Color[ForeColor] );
    for( BackColor = 0; BackColor < MAXCOLORS; BackColor++ ) {
      SetConsoleTextAttribute( hConsole, (WORD) ( (BackColor << 4) | ForeColor) );
      printf(" XxXxXx ");
    }
    SetConsoleTextAttribute( hConsole, wAttributesOld );
    printf("\n");
  }

  SetConsoleTextAttribute( hConsole, wAttributesOld );
  exit( 0 );

  return( TRUE );
}


/************************************************************************\
*
* FUNCTION    : GetConsoleTextAttribute
*
* INPUTS      : hConsole - handle to an open console
*
* RETURNS     : Current text attributes WORD.
*
* GLOBAL VARS : None.
*
* LOCAL VARS  : csbi - Console screen buffer information structure
*
* COMMENTS    : Gets the previous console color attributes
*
\************************************************************************/
WORD
GetConsoleTextAttribute( HANDLE hConsole )
{
  CONSOLE_SCREEN_BUFFER_INFO csbi;

  GetConsoleScreenBufferInfo( hConsole, &csbi );
  return( csbi.wAttributes );
}
