/*****************************************************************************
Programmer: Glenn Anderson
Project:    C++ Runtime Library, Win32

This is the implementation of the 32 bit serialization code.
*****************************************************************************/

#include <windows.h>
#include <string.h>

#include "semlock.h"

/*==========================================================================*/
/* Global data for the semaphore system                                     */
/*==========================================================================*/

#define _SIZE_SEM_PREFIX 9

short _iSemLockCtrs[_MAX_SEMAPHORES];
int _iSemThreadIds[_MAX_SEMAPHORES];
int _iSemNestCount[_MAX_SEMAPHORES];
HANDLE _hdlSemHandles[_MAX_SEMAPHORES];

char _szSemPrefix[_SIZE_SEM_PREFIX];
int _szSemPrefixLen;

/*==========================================================================*/
/* Semaphore routines                                                       */
/*==========================================================================*/

/*--------------------------------------------------------------------------*/
/* _InitSemaphoreSys is called by the startup code to initialize the        */
/* semaphore system.  It must be called before any of the routines that use */
/* semaphores is called.                                                    */

void _InitSemaphoreSys()
{
    static int sem_inited = 0;
    
    if (!sem_inited)
    {
        /* Fill arrays */
        memset(_iSemLockCtrs, -1, _MAX_SEMAPHORES * sizeof(short int));

	// These are already set to 0 because they are in BSS
	//memset(_iSemThreadIds, 0, _MAX_SEMAPHORES * sizeof(int));
	//memset(_iSemNestCount, 0, _MAX_SEMAPHORES * sizeof(int));
	//memset(_hdlSemHandles, 0, _MAX_SEMAPHORES * sizeof(HANDLE));

        /* Assign prefix value */
        itoa(GetTickCount(), _szSemPrefix, 16);
	_szSemPrefixLen = strlen(_szSemPrefix);

        sem_inited++;
    }
}

/*--------------------------------------------------------------------------*/
/* _DestroySemaphore is called whenever a semaphore that may have been      */
/* created can be deleted                                                   */

void _DestroySemaphore(int iSemaphore)
{
    LockSemaphore(_semDestroySem)

    if (_hdlSemHandles[iSemaphore])
        CloseHandle(_hdlSemHandles[iSemaphore]);
    _hdlSemHandles[iSemaphore] = 0;

    UnlockSemaphore(_semDestroySem)
}

/*--------------------------------------------------------------------------*/
/* _DestroyAllSemaphores is called when exiting the application to cleanly  */
/* destroy any existing semaphores                                          */

void _DestroyAllSemaphores()
{
    int iIdx;

    /* This is not destroying the destroy semaphore semaphore */
    for (iIdx = 1; iIdx < _MAX_SEMAPHORES; iIdx++)
        _DestroySemaphore(iIdx);

    CloseHandle(_hdlSemHandles[_semDestroySem]);
    _hdlSemHandles[_semDestroySem] = 0;
}

/***********************************
 * Ensure that semaphore exists.
 */

static void _EnsureSemaphore(int iSemaphore)
{
    char szSemName[25];
    HANDLE hdlNewSem;

    memcpy(szSemName, _szSemPrefix, _szSemPrefixLen);
    itoa(iSemaphore, szSemName + _szSemPrefixLen, 16);

    while (!_hdlSemHandles[iSemaphore])
    {
        hdlNewSem = CreateSemaphore(NULL, 0, 1, szSemName);
        if (hdlNewSem)
            _hdlSemHandles[iSemaphore] = hdlNewSem;
    }

}

/*--------------------------------------------------------------------------*/
/* _WaitSemaphore is called when it has been determined that another thread */
/* has already gained access to a particular function.  It will create a    */
/* semaphore in an unsignaled state and then wait for it to become signaled */

void _WaitSemaphore(int iSemaphore)
{
    _EnsureSemaphore(iSemaphore);
    WaitForSingleObject(_hdlSemHandles[iSemaphore], INFINITE);
}

/*--------------------------------------------------------------------------*/
/* _ReleaseSemaphore is called when it has been determined that another     */
/* thread is waiting for the semaphore to be signaled.  Because of timing   */
/* issues the semaphore may need to be created.                             */

void _ReleaseSemaphore(int iSemaphore)
{
    _EnsureSemaphore(iSemaphore);
    ReleaseSemaphore(_hdlSemHandles[iSemaphore], 1, 0);
}

/*==========================================================================*/
/* The following routines can be called instead of using the macros         */
/*==========================================================================*/

/*--------------------------------------------------------------------------*/
/* _LockSemaphore will wait if the semaphore has already been locked,       */
/* otherwise it will exit cleanly                                           */

void _LockSemaphore(int iSemaphore)
{
    LockSemaphore(iSemaphore)
}

/*--------------------------------------------------------------------------*/
/* _UnlockSemaphore will release the lock on the semaphore, and allow       */
/* another lock to take place                                               */

void _UnlockSemaphore(int iSemaphore)
{
    UnlockSemaphore(iSemaphore)
}
