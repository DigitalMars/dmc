// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
// PARTICULAR PURPOSE.
//
// Copyright (C) 1992-1995  Microsoft Corporation.  All Rights Reserved.
//
//  MODULE:   thrdutil.c
//
//  PURPOSE:  Maintain list of outstanding threads
//
//  FUNCTIONS:
//    WaitCloseOutstanding - Waits for all threads to finish,
//                           and closes thread handles
//    RefreshOutstanding - refreshs list, removing terminated threads
//    MarkOutstanding - adds a thread to the outstanding list
//
//  AUTHOR: Craig Link - Microsoft Developer Support
//

#include <windows.h>
#include <stdlib.h>
#include "thrdutil.h"
#include "mem.h"

// initialized in DllMain procedure
CRITICAL_SECTION g_csOutstanding;

// variables global to this module
HANDLE *g_lphOutstanding = NULL;
UINT    g_nOutstanding = 0;


//
//  FUNCTION: WaitCloseOutstanding()
//
//  PURPOSE:  Waits for all threads to finish, and closes thread handles
//
//  PARAMETERS:
//    none
//
//  RETURN VALUE:
//    none
//
//  COMMENTS:
//
VOID WaitCloseOutstanding()
{
    UINT ndx;

    EnterCriticalSection(&g_csOutstanding);

    for( ndx = 0; ndx < g_nOutstanding; ndx+=MAXIMUM_WAIT_OBJECTS )
    {
        WaitForMultipleObjects( g_nOutstanding-ndx,
                                g_lphOutstanding+ndx,
                                TRUE, INFINITE );

    }

    for( ndx = 0; ndx < g_nOutstanding; ndx++ )
        CloseHandle(g_lphOutstanding[ndx]);

    FreeMem((LPVOID*)&g_lphOutstanding);
    g_nOutstanding = 0;

    LeaveCriticalSection(&g_csOutstanding);

}

//
//  FUNCTION: RefreshOutstanding()
//
//  PURPOSE:  Refresh outstanding thread list, removing and threads
//            which have terminated
//
//  PARAMETERS:
//    none
//
//  RETURN VALUE:
//    none
//
//  COMMENTS:
//
VOID RefreshOutstanding()
{
    UINT ndx = 0;
    UINT ndx2 = 0;

    EnterCriticalSection(&g_csOutstanding);

    for( ndx = 0; ndx < g_nOutstanding; ndx++ )
    {
        if ( WaitForSingleObject( g_lphOutstanding[ndx], 0 ) == WAIT_OBJECT_0 )
        {
            CloseHandle( g_lphOutstanding[ndx] );
            g_lphOutstanding[ndx] = NULL;
        }
        else  // handle still outstanding
        {
            g_lphOutstanding[ndx2++] = g_lphOutstanding[ndx];
        }
    }

    if ( g_nOutstanding < ndx2 )
    {
        // realloc won't fail because we are shrinking
        g_nOutstanding = ndx2;
        ReallocMem( (LPVOID*)&g_lphOutstanding, sizeof(HANDLE)*g_nOutstanding );
    }

    LeaveCriticalSection(&g_csOutstanding);

}


//
//  FUNCTION: MarkOutstanding( HANDLE *lpHandle )
//
//  PURPOSE:  Adds a thread to the outstanding thread list
//
//  PARAMETERS:
//    lpHandle - pointer to thread to add
//
//  RETURN VALUE:
//    ERROR_NOT_ENOUGH_MEMORY
//    ERROR_SUCCESS
//
//  COMMENTS:
//    If thread is successfully added the passed in handle 
//    is set to NULL
//
DWORD MarkOutstanding( HANDLE *lpHandle )
{
    DWORD dwRet;

    EnterCriticalSection(&g_csOutstanding);

    RefreshOutstanding();

    if ( ERROR_NOT_ENOUGH_MEMORY == ReallocMem( (LPVOID*)&g_lphOutstanding, sizeof(HANDLE)*(g_nOutstanding+1) ) )
    {
        dwRet = ERROR_NOT_ENOUGH_MEMORY;
    }
    else
    {
        g_lphOutstanding[g_nOutstanding++] = *lpHandle;
        *lpHandle = NULL;
        dwRet = ERROR_SUCCESS;
    }

    LeaveCriticalSection(&g_csOutstanding);

    return dwRet;
}
