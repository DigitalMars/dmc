
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
**   MODULE:  DataBase                                                     **
**                                                                         **
**                                                                         **
**   PURPOSE: Demonstrates the basic concepts of one of the classical      **
**            synchronous problems. A derivative of the Reader/Writer      **
**            problem. In which one must allow access to each without      **
**            jeopardizing the Database.                                   **
**                                                                         **
**   INTERNAL FUNCTIONS:                                                   **
**                                                                         **
**          InitializeDataBase                                             **
**          CloseDataBase                                                  **
**          InitializeSubsystem                                            **
**          InitApp                                                        **
**          CleanupSubsystem                                               **
**          CleanupApp                                                     **
**          TestInitExitPoint                                              **
**          ErrorMsg                                                       **
**          DebugMsg                                                       **
**                                                                         **
**                                                                         **
**   EXPORTED FUNCTIONS                                                    **
**                                                                         **
**          ReadDataBase                                                   **
**          WriteDataBase                                                  **
**                                                                         **
**                                                                         **
**   COMMENTS:                                                             **
**                                                                         **
**          This also demonstrates how to have both global shared data     **
**          instance data. What I've done is named the shared section      **
**          with the #pragma data_seg( SEGNAME) statement and              **
**          referenced this in the DEF file as READ/WRITE/SHARED.          **
**                                                                         **
\***************************************************************************/




#define STRICT
#define NOMINMAX
#include <windows.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>


typedef HANDLE HMUTEX ;
typedef HANDLE HSEM ;
typedef HANDLE HEVENT ;

/* Names for synchronization objects */
#define SEM1    "semph1"
#define MUTX1   "mutx1"
#define EVENT1  "event1"
#define EVENT2  "event2"
#define EVENT3  "evnet3"
#define EVENT4  "event4"

#define MAX     10

#define SIGNALED    0
#define WAIT_ERROR  0xFFFFFFFF


/*global Variables */


/* Put gUserCount in a GLOBAL section. This is because first time
 * initialization is handled differently than others
 */

#pragma data_seg("MYSEG")

int    gUserCount = 0 ;

#pragma data_seg(".data")




HMUTEX ghMutex ;
HSEM   ghSemaphore ;
HEVENT ghReadEvent1, ghReadEvent2, ghWriteEvent1, ghWriteEvent2 ;
UINT   cReadQueue, cWriteQueue, cReadQueueEx ;
UINT   cRead, cWrite ;
BOOL   InWrite ;

/*
 * This is the data type which this
 *
 */

char   *DataBase[MAX] ;



/***** Private functions *****/

BOOL InitializeSubsystem ( VOID ) ;
BOOL InitApp ( VOID ) ;
BOOL CleanupSubsystem ( VOID ) ;
BOOL CleanupApp ( VOID ) ;
BOOL InitializeDataBase ( VOID ) ;
BOOL CloseDataBase ( VOID ) ;
BOOL GetData ( PSZ psz ) ;
BOOL PutData ( PSZ psz ) ;


/***** DLL Entry/Exit point *****/

BOOL WINAPI DllMain ( HINSTANCE hModule, DWORD dwReason, LPVOID lpReserved ) ;


/***** Debuging functions *****/

VOID ErrorMsg ( PSZ psz ) ;
VOID DebugMsg ( PSZ psz ) ;





/***************************************************************************\
**                                                                         **
**   FUNCTION: ErrorMsg(LPTSTR)                                            **
**                                                                         **
**   PURPOSE: Prints out an error message the return code of the last API  **
**            to fail.                                                     **
**                                                                         **
**   INPUT: psz: This string is a description of where the code failed in  **
**          the module                                                     **
**                                                                         **
**   OUTPUT: None                                                          **
**                                                                         **
\****************************************************************************/

VOID ErrorMsg ( PSZ psz )
{
    printf ("\n%s: return code = %d\n", psz, GetLastError()) ;
}


/***************************************************************************\
**                                                                         **
**   FUNCTION: DebugMsg(LPTSTR)                                            **
**                                                                         **
**   PURPOSE: Prints out an debug message                                  **
**                                                                         **
**   INPUT: psz: This string is used for informational purposes only       **
**                                                                         **
**   OUTPUT: None                                                          **
**                                                                         **
\****************************************************************************/


VOID DebugMsg ( PSZ psz )
{
    printf ( psz ) ;
}


/***************************************************************************\
**                                                                         **
**   FUNCTION: DllMain                                            **
**                                                                         **
**   PURPOSE: Handles both initialization and cleanup of DLL               **
**                                                                         **
**   INPUT: Handle to module calling this DLL. Flag which indicates        **
**          why the DLL entry routine is being called.                     **
**                                                                         **
**   OUTPUT: None                                                          **
**                                                                         **
**   ERRORS: Returns FALSE if an error occured while the process was being **
**           attached. Any other time the return value is ignored.         **
**                                                                         **
**   COMMENTS: This function takes place of the LIBMAIN/LIBENTRY and WEP   **
**                                                                         **
**             Even if no initialization is need this function must return **
**                                                                         **
**                                                                         **
\****************************************************************************/



BOOL WINAPI DllMain ( HINSTANCE hModule, DWORD dwReason, LPVOID lpReserved )
{

    BOOL bSuccess ;
    UNREFERENCED_PARAMETER ( hModule ) ;
    UNREFERENCED_PARAMETER ( lpReserved ) ;


    if ( dwReason == DLL_PROCESS_ATTACH ) {

        gUserCount++ ;

        printf ( "\nNumber of clients currently using this are %d\n", gUserCount ) ;

        if ( gUserCount == 1 )
            bSuccess = InitializeSubsystem ( ) ;
        else
            bSuccess = InitApp ( ) ;
    }

    else if ( dwReason == DLL_PROCESS_DETACH ) {

        printf ( "\nCleaning up subsystem ...\n" ) ;
        printf ( "Number of clients currently using this are %d\n", gUserCount ) ;

        if ( gUserCount ==  1 )
            bSuccess = CleanupSubsystem ( ) ;
        else
            bSuccess = CleanupApp ( ) ;

        gUserCount-- ;

    }


    return ( bSuccess ) ;
}


/***************************************************************************\
**                                                                         **
**   FUNCTION: InitializeSubsystem                                         **
**                                                                         **
**   PURPOSE: Sets up the required synchronization objects to insure both  **
**            system performance and data integrity. This is only done     **
**            once; by the first process that attaches itself to the DLL   **
**                                                                         **
**   INPUT: None                                                           **
**                                                                         **
**   OUTPUT: Returns TRUE if successfull otherwise FALSE                   **
**                                                                         **
\****************************************************************************/

BOOL InitializeSubsystem ( VOID )
{
    LONG cSemMax = 5;
    BOOL bSuccess ;

    DebugMsg ( "InitializeSubsystem\n" ) ;

    ghSemaphore = CreateSemaphore ( NULL,       // No security descriptor

                                   0,           // Initial count of zero so
                                                // access to database is blocked
                                                // till initialization completed
                                   cSemMax,
                                                // Maximun threads allowed to
                                                // access the database at one time
                                   SEM1) ;      // No name

    if ( !ghSemaphore )
    {
        ErrorMsg ( "Error Creating semaphore" ) ;
        return ( FALSE ) ;
    }


    ghMutex = CreateMutex ( NULL,               // No security descriptor
                            TRUE,               // Block access till database
                                                // is setup
                            MUTX1 ) ;           // No name
    if ( !ghMutex )
    {
        ErrorMsg ( "Error Creating Mutex" ) ;
        return ( FALSE ) ;
    }

    ghReadEvent1 = CreateEvent ( NULL,           // No security descriptor
                                FALSE,          // Use manual reset so all
                                                // waiting threads get signaled
                                FALSE,          // Set to non-signaled state
                                EVENT1 ) ;


    if ( !ghReadEvent1 )
    {
        ErrorMsg ( "Error Creating ghReadEvent1" ) ;
        return ( FALSE ) ;
    }

    ghWriteEvent1 = CreateEvent ( NULL,           // No security descriptor
                                FALSE,          // Use manual reset so all
                                                // waiting threads get signaled
                                FALSE,          // Set to non-signaled state
                                EVENT2 ) ;


    if ( !ghWriteEvent1 )
    {
        ErrorMsg ( "Error Creating ghWriteEvent1" ) ;
        return ( FALSE ) ;
    }

    ghReadEvent2 = CreateEvent ( NULL,           // No security descriptor
                                FALSE,          // Use manual reset so all
                                                // waiting threads get signaled
                                FALSE,          // Set to non-signaled state
                                EVENT3 ) ;


    if ( !ghReadEvent2 )
    {
        ErrorMsg ( "Error Creating ghReadEvent2" ) ;
        return ( FALSE ) ;
    }

    ghWriteEvent2 = CreateEvent ( NULL,           // No security descriptor
                                FALSE,          // Use manual reset so all
                                                // waiting threads get signaled
                                FALSE,          // Set to non-signaled state
                                EVENT4 ) ;

    if ( !ghWriteEvent2 )
    {
        ErrorMsg ( "Error Creating ghWriteEvent2" ) ;
        return ( FALSE ) ;
    }

    bSuccess = InitializeDataBase () ;


    /*
     * Now that all the initialization is done it's save to
     * release the semaphore and mutex.
     *
     */

    if ( !ReleaseSemaphore ( ghSemaphore,       // Handle to semaphore
                       cSemMax,                 // Size to increment
                       NULL ) )                 // Don't care about previous value
        {
            ErrorMsg ( "Error releasing semaphore" ) ;
            return ( FALSE ) ;
        }


    if ( !ReleaseMutex ( ghMutex ) )            // Handle to semaphore
        {
            ErrorMsg ( "Error releasing mutex" ) ;
            return ( FALSE ) ;
        }

    return ( bSuccess ) ;
}

/***************************************************************************\
**                                                                         **
**   FUNCTION: InitApp                                                     **
**                                                                         **
**   PURPOSE: Sets up the required synchronization objects to insure both  **
**            system performance and data integrity. This is done each     **
**            a process attaches itself to this DLL                        **
**                                                                         **
**   INPUT: None                                                           **
**                                                                         **
**   OUTPUT: Returns TRUE if successfull otherwise FALSE                   **
**                                                                         **
\****************************************************************************/

BOOL InitApp ( VOID )
{

    DebugMsg ( "InitApp\n" ) ;

    ghSemaphore = OpenSemaphore ( SEMAPHORE_ALL_ACCESS,  //
                                 FALSE,                  // No inheritance
                                 SEM1 ) ;                // No name

    if ( !ghSemaphore )
    {
        ErrorMsg ( "Error Opening semaphore" ) ;
        return ( FALSE ) ;
    }


    ghMutex = OpenMutex  ( MUTEX_ALL_ACCESS,    //
                           FALSE,               // No inheritance
                           MUTX1 ) ;            // No name
    if ( !ghMutex )
    {
        ErrorMsg ( "Error Opening Mutex" ) ;
        return ( FALSE ) ;
    }

    ghReadEvent1 = OpenEvent ( EVENT_ALL_ACCESS, //
                              FALSE,            // No inheritance
                              EVENT1 ) ;        //

    if ( !ghReadEvent1 )
    {
        ErrorMsg ( "Error Opening ghReadEvent1" ) ;
        return ( FALSE ) ;
    }


    ghWriteEvent1 = OpenEvent ( EVENT_ALL_ACCESS, //
                               FALSE,            // No inheritance
                               EVENT2 ) ;        //


    if ( !ghWriteEvent1 )
    {
        ErrorMsg ( "Error Opening ghWriteEvent1" ) ;
        return ( FALSE ) ;
    }

    ghReadEvent2 = OpenEvent ( EVENT_ALL_ACCESS, //
                              FALSE,            // No inheritance
                              EVENT3 ) ;        //

    if ( !ghReadEvent2 )
    {
        ErrorMsg ( "Error Opening ghReadEvent2" ) ;
        return ( FALSE ) ;
    }


    ghWriteEvent2 = OpenEvent ( EVENT_ALL_ACCESS, //
                               FALSE,            // No inheritance
                               EVENT4 ) ;        //


    if ( !ghWriteEvent2 )
    {
        ErrorMsg ( "Error Opening ghWriteEvent2" ) ;
        return ( FALSE ) ;
    }

    return ( TRUE ) ;
}



BOOL CleanupSubsystem ( VOID )
{
    DebugMsg ( "CleanupSubsystem\n" ) ;

    if ( !CloseHandle ( ghSemaphore ) )
        ErrorMsg ( "Error Closing Semaphore" ) ;

    if ( !CloseHandle ( ghMutex ) )
        ErrorMsg ( "Error Closing Mutex" ) ;

    if ( !CloseHandle ( ghReadEvent1 ) )
        ErrorMsg ( "Error Closing Event" ) ;

    if ( !CloseHandle ( ghWriteEvent1 ) )
        ErrorMsg ( "Error Closing Event" ) ;

    CloseDataBase ( ) ;

    return ( TRUE ) ;
}


/***************************************************************************\
**                                                                         **
**   FUNCTION: CleanupApp                                                  **
**                                                                         **
**   PURPOSE: Cleans up (closes) the synchronization objects.              **
**            This is done each a process attaches itself to this DLL      **
**                                                                         **
**   INPUT: None                                                           **
**                                                                         **
**   OUTPUT: Returns TRUE if successfull otherwise FALSE                   **
**                                                                         **
\****************************************************************************/

BOOL CleanupApp ( VOID )
{
    DebugMsg ( "CleanupApp\n" ) ;

    if ( !CloseHandle ( ghSemaphore ) )
        ErrorMsg ( "Error Closing Semaphore" ) ;

    if ( !CloseHandle ( ghMutex ) )
        ErrorMsg ( "Error Closing Mutex" ) ;

    if ( !CloseHandle ( ghReadEvent1 ) )
        ErrorMsg ( "Error Closing Event" ) ;

    if ( !CloseHandle ( ghWriteEvent1 ) )
        ErrorMsg ( "Error Closing Event" ) ;

    return ( TRUE ) ;
}


/***************************************************************************\
**                                                                         **
**   FUNCTION: InitializeDataBase                                          **
**                                                                         **
**   PURPOSE:
**
**                                                                         **
**   INPUT: None                                                           **
**                                                                         **
**   OUTPUT: Returns TRUE if successfull otherwise FALSE                   **
**                                                                         **
\****************************************************************************/

BOOL InitializeDataBase ( VOID )
{
    UINT i ;

    DebugMsg ( "Initializing database\n" ) ;

    for ( i=0; i< MAX; i++ )
        PutData ( "Unintialized" ) ;

    return ( TRUE ) ;
}


/***************************************************************************\
**                                                                         **
**   FUNCTION: CloseDataBase                                               **
**                                                                         **
**   PURPOSE:
**
**                                                                         **
**   INPUT: None                                                           **
**                                                                         **
**   OUTPUT: Returns TRUE if successfull otherwise FALSE                   **
**                                                                         **
\****************************************************************************/

BOOL CloseDataBase ( VOID )
{
    DebugMsg ( "Closing database\n" ) ;
    return ( TRUE ) ;
}


/***************************************************************************\
**                                                                         **
**   FUNCTION: ReadDataBase ( PSZ                                          **
**                                                                         **
**   PURPOSE: Fetches data out of the database                             **
**                                                                         **
**   INPUT: None                                                           **
**                                                                         **
**   OUTPUT: Pointer to data                                               **
**                                                                         **
**   ERROR: Returns False if it can not complete the request               **
**                                                                         **
**   COMMENTS:  A reader can not enter the database if                     **
**                                                                         **
**              1) A writer already is in the database                     **
**              2) The maximum number of readers are in the database       **
**                                                                         **
**              A new reader can not enter the databse if                  **
**                                                                         **
**              1) A writer is waiting                                     **
**                                                                         **
\****************************************************************************/


BOOL WINAPI ReadDataBase  ( PSZ psz1, PSZ psz2 )
{
   BOOL bSuccess ;
   DWORD dwSuccess ;
   UNREFERENCED_PARAMETER ( psz2 ) ;

//   DebugMsg ("In ReadDataBase\n" ) ;

   cReadQueueEx++ ;
   dwSuccess = SIGNALED ;

   if ( cWriteQueue )                               // Block if Writer waiting
         dwSuccess = WaitForSingleObject ( ghReadEvent1, 0xFFFFFFFF ) ;
//       dwSuccess = WaitForSingleObject ( ghReadEvent1, 5000 ) ;

   cReadQueue++ ;                                   // Reader entering queue
   cReadQueueEx-- ;

   if ( dwSuccess != SIGNALED )
   {
      ErrorMsg ( "ReadDatabase: Error Waiting for ghReadEvent1" ) ;
      bSuccess = FALSE ;
   }
   else
   {
   /*
    * Only let N readers in the queue at one time
    */
      switch ( WaitForSingleObject ( ghSemaphore, 0xFFFFFFFF ) )
//      switch ( WaitForSingleObject ( ghSemaphore, 3000 ) )
      {
          case SIGNALED:
              __try {
                   if ( InWrite )               // Block if Writer in database
                      dwSuccess = WaitForSingleObject ( ghReadEvent2, 0xFFFFFFFF ) ;
//                    dwSuccess = WaitForSingleObject ( ghReadEvent2, 1500 ) ;

                   cRead++ ;                    // Number readers in database
                   cReadQueue-- ;               // Reader leaving queue

                   /*
                    * Signal Writer if no Reader waiting
                    *
                    */
                   if ( !cReadQueue )
                      bSuccess = SetEvent ( ghWriteEvent1 ) ;

                   if ( dwSuccess == SIGNALED )
                   {
                      bSuccess = GetData ( psz1 ) ;   // Read data
                      cRead-- ;
                      /*
                       * Signal Writer if no Reader in database
                       *
                       */

                      if ( !cRead )
                         bSuccess = SetEvent ( ghWriteEvent2 ) ;

                      ReleaseSemaphore ( ghSemaphore, // Let next waiting
                                          1,          // reader in the queue
                                          NULL ) ;
                   }
                   else
                   {
                      cRead-- ;
                      /*
                       * Signal Writer if no Reader in database
                       *
                       */

                      if ( !cRead )
                         bSuccess = SetEvent ( ghWriteEvent2 ) ;

                      bSuccess = FALSE ;
                      DebugMsg ( "ReadDataBase: Error Waiting for ghReadEvent2\n" ) ;
                   }
              }
              __finally {
                 // what if these is already decremented above????
                   cRead-- ;
                   /*
                    * Signal Writer if no Reader in database
                    *
                    */

                   if ( !cRead )
                      bSuccess = SetEvent ( ghWriteEvent2 ) ;

                   /*
                    * Signal Writer if no Reader in queue
                    *
                    */
                   if ( !cReadQueue )
                      bSuccess = SetEvent ( ghWriteEvent1 ) ;

                   bSuccess = FALSE ;
                   ReleaseSemaphore ( ghSemaphore,     // Let next waiting
                                  1,                   // reader in the queue
                                  NULL ) ;
              }

              break ;

          case WAIT_TIMEOUT:
              DebugMsg ( "ReadDataBase: Error WAIT_TIMEOUT ghSemaphore\n" ) ;

              //where to put this?
              cReadQueue-- ;            // Reader leaving queue
              /*
               * Signal Writer if no Reader waiting
               *
               */
              if ( !cReadQueue )
                 bSuccess = SetEvent ( ghWriteEvent1 ) ;

              bSuccess = FALSE ;
              break ;

          case WAIT_ERROR:
              ErrorMsg ( "ReadDataBase: Error WAIT_ERROR ghSemaphore" ) ;

              //where to put this?
              cReadQueue-- ;            // Reader leaving queue
              /*
               * Signal Writer if no Reader waiting
               *
               */
              if ( !cReadQueue )
                 bSuccess = SetEvent ( ghWriteEvent1 ) ;

              bSuccess = FALSE ;
              break ;

          default:
              DebugMsg ( "ReadDataBase: Dropped through switch\n" ) ;

              //where to put this?
              cReadQueue-- ;            // Reader leaving queue
              /*
               * Signal Writer if no Reader waiting
               *
               */
              if ( !cReadQueue )
                 bSuccess = SetEvent ( ghWriteEvent1 ) ;

              bSuccess = FALSE ;
              break ;
      }
   } // END IF



   return ( bSuccess ) ;
}





/***************************************************************************\
**                                                                         **
**   FUNCTION: WriteDataBase ( PSZ                                         **
**                                                                         **
**   PURPOSE: Puts data into the database                                  **
**                                                                         **
**   INPUT: Pointer to data                                                **
**                                                                         **
**   OUTPUT: None                                                          **
**                                                                         **
**   ERROR: Returns False if it can not complete the request               **
**                                                                         **
**   COMMENTS:  A writer can not enter the database if                     **
**                                                                         **
**              1) A writer already is in the database                     **
**              2) Any readers are in the database                         **
**                                                                         **
**              All reader waiting at the end of a write operation         **
**              have priority over the next writer                         **
**                                                                         **
\****************************************************************************/

BOOL WINAPI WriteDataBase ( PSZ psz1, PSZ psz2 )
{
   BOOL bSuccess ;
   DWORD dwSuccess ;
   UNREFERENCED_PARAMETER ( psz2 ) ;

//   DebugMsg ( "In WriteDataBase\n" ) ;

   cWriteQueue++ ;                             // Writer entering queue
   dwSuccess = SIGNALED ;

                   // should the same event be used twice ???

   if ( cReadQueue )                           // Block if Reader waiting
      dwSuccess = WaitForSingleObject ( ghWriteEvent1, 0xFFFFFFFF ) ;
//      dwSuccess = WaitForSingleObject ( ghWriteEvent1, 5000 ) ;

   if ( dwSuccess != SIGNALED )
   {
      DebugMsg ( "WriteDataBase: Error Waiting for ghWriteEvent1\n" ) ;
      bSuccess = FALSE ;
   }
   else
   {
   /*
    * Only allow 1 writer at a time in the database
    *
    */
      switch ( WaitForSingleObject ( ghMutex, 0xFFFFFFFF ) ) {
//      switch ( WaitForSingleObject ( ghMutex, 3000 ) ) {
         case SIGNALED:
            __try {
               InWrite = TRUE ;
               cWriteQueue-- ;

               /*
                * Signal Reader if any waiting
                *
                */
               if ( cReadQueueEx )
                  bSuccess = SetEvent ( ghReadEvent1 ) ;

                               // Writer leaving queue
               if ( cRead )               // Block if Reader in database
                  dwSuccess = WaitForSingleObject ( ghWriteEvent2, 0xFFFFFFFF ) ;
//                dwSuccess = WaitForSingleObject ( ghWriteEvent2, 1500 ) ;

               if ( dwSuccess == SIGNALED )
               {
                  bSuccess = PutData ( psz1 ) ;
                  InWrite = FALSE ;
                  /*
                   * Signal Reader when no Writer waiting
                   *
                   */
                  bSuccess = SetEvent ( ghReadEvent2 ) ;
                  ReleaseMutex ( ghMutex ) ;    // Let next waiting writer in the queue
               }
               else
               {
                  bSuccess = FALSE ;
                  InWrite = FALSE ;
                  /*
                   * Signal Reader when no Writer waiting
                   *
                   */
                  bSuccess = SetEvent ( ghReadEvent2 ) ;
                  DebugMsg ( "WriteDataBase: Error Waiting on ghWriteEvent2\n" ) ;
               }
            }
            __finally {
               InWrite = FALSE ;
               /*
                * Signal Reader after leaving database
                *
                */
               bSuccess = SetEvent ( ghReadEvent2 ) ;

               /*
                * Signal Reader if no Writer in queue
                *
                */
               if ( !cWriteQueue )
                  bSuccess = SetEvent ( ghReadEvent1 ) ;

               bSuccess = FALSE ;
               ReleaseMutex ( ghMutex ) ;
            }
            break ;

         case WAIT_TIMEOUT:
            DebugMsg ( "WriteDataBase: Error WAIT_TIMEOUT ghMutex\n" ) ;
            cWriteQueue-- ;
            /*
             * Signal Reader if no Writer waiting
             *
             */
            if ( !cWriteQueue )
               bSuccess = SetEvent ( ghReadEvent1 ) ;

            bSuccess = FALSE ;
            break ;

         case WAIT_ABANDONED:
            DebugMsg ( "WriteDataBase: Error WAIT_ABANDONED ghMutex\n" ) ;
            cWriteQueue-- ;
            /*
             * Signal Reader if no Writer waiting
             *
             */
            if ( !cWriteQueue )
               bSuccess = SetEvent ( ghReadEvent1 ) ;

            bSuccess = FALSE ;
            break ;

         case WAIT_ERROR:
            ErrorMsg ( "WriteDataBase: Error WAIT_ERROR ghMutex" ) ;
            cWriteQueue-- ;
            /*
             * Signal Reader if no Writer waiting
             *
             */
            if ( !cWriteQueue )
               bSuccess = SetEvent ( ghReadEvent1 ) ;

            bSuccess = FALSE ;
            break ;

         default:
            DebugMsg ( "WriteDataBase: Dropped through switch\n" ) ;
            cWriteQueue-- ;
            /*
             * Signal Reader if no Writer waiting
             *
             */
            if ( !cWriteQueue )
               bSuccess = SetEvent ( ghReadEvent1 ) ;

            bSuccess = FALSE ;
            break ;
      }
   } // END IF


   if ( cReadQueue )
       bSuccess = SetEvent ( ghReadEvent1 ) ;          // Signal Reader
   else
       bSuccess = SetEvent ( ghWriteEvent1 ) ;         // Signal Writer

   return ( bSuccess ) ;
}



BOOL GetData ( PSZ psz )
{
    static UINT i = 0 ;

    strcpy ( psz, DataBase[i++ % MAX] ) ;
    return ( TRUE ) ;
}


BOOL PutData ( PSZ psz )
{
    static UINT i = 0 ;

    i++ ;
    DataBase[i % MAX] = strdup ( psz ) ;
    return ( TRUE ) ;
}
