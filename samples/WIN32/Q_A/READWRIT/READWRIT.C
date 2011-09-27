
/******************************************************************************\
*       This is a part of the Microsoft Source Code Samples. 
*       Copyright (C) 1992-1995 Microsoft Corporation.
*       All rights reserved. 
*       This source code is only intended as a supplement to 
*       Microsoft Development Tools and/or WinHelp documentation.
*       See these sources for detailed information regarding the 
*       Microsoft samples programs.
\******************************************************************************/

/****************************************************************************\
**   MODULE:  Test Engine                                                  **
**                                                                         **
**                                                                         **
**   PURPOSE: This is a simple driver to test my database dll's            **
**            ability to handle multiple read's and writes without         **
**            corrupting the database.                                     **
**                                                                         **
**                                                                         **
**                                                                         **
**                                                                         **
**   COMMENTS:                                                             **
**                                                                         **
**                                                                         **
**                                                                         **
\***************************************************************************/



#define STRICT
#define NOMINMAX
#include <windows.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "DataBase.h"

VOID Read ( PSZ psz ) ;
VOID Write ( PSZ psz ) ;

#define MAX 100


/*
 * This is a simple driver to test my database dll's ability to handle
 * multiple read's and writes without corrupting the database.
 *
 */



void main( void )
{
    char psz[20], psztmp[10] ;
    int  i ;
    DWORD idThread ;
    DWORD dwVersion = GetVersion();

  // check if Win32s, if so, display notice and terminate
   if( !(dwVersion < 0x80000000 ) && (LOBYTE(LOWORD(dwVersion)) < 4) )
        {
          MessageBox( NULL,
             "This application cannot run on Windows 3.1.\n"
             "This application will now terminate.",
             "ReadWrit",
             MB_OK | MB_ICONSTOP | MB_SETFOREGROUND );
          return;
        }
  
    printf ( "\nStart of program\n" ) ;

/*
 * Create the reader/writers to stress the database
 *
 */

    for ( i=1; i<=MAX; i++ )
    {
        strcat (strcpy ( psz,"In Thread #" ),itoa( i,psztmp, 10)) ;
        CreateThread ( NULL, 0, (LPTHREAD_START_ROUTINE)Write,psz, 0,
                 &idThread ) ;

        CreateThread ( NULL, 0, (LPTHREAD_START_ROUTINE)Read, psz, 0,
                 &idThread ) ;

        Sleep( 0 ) ;                        // Relinquish timeslice
        printf ( "psz = %s\n", psz ) ;      // Print result of the test
    }

    printf ( "\nEnd of program\n" ) ;

    exit ( EXIT_SUCCESS ) ;
}


VOID Read ( PSZ psz )
{
  ReadDataBase ( psz, NULL ) ;
}

VOID Write ( PSZ psz )
{

  WriteDataBase ( psz, NULL ) ;
}
