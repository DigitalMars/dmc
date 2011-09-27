// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
// PARTICULAR PURPOSE.
//
// Copyright (C) 1992-1995  Microsoft Corporation.  All Rights Reserved.
//
// MODULE: enumthrd.c
//
// PURPOSE:
//    Contains thread procedures for enumeration.
//
//  FUNCTIONS:
//    InitialEnumThread - performs initial network enumeration
//    EnumExpandThread - expands a entry in list box by 1 level
//    EnumContainerThread - does actual container enumeration
//
//  COMMENTS:
//    cThread of the ENUMSYNC object should be incremented before calling
//    or creating threads on these procedures since they call FreeEnumSync()
//    befored returning
//
//  AUTHOR: Craig Link - Microsoft Developer Support
//



#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <tchar.h>
#include <lm.h>
#include "enumutil.h"
#include "enumthrd.h"
#include "enumdlg.h"
#include "mem.h"


//
//  FUNCTION: InitialEnumThread( LPENUMSYNC lpSync )
//
//  PURPOSE:  Perform initial enumeration of network
//
//  PARAMETERS:
//    lpSync - pointer to syncronization object for threads
//
//  RETURN VALUE:
//    ERROR_SUCCESS
//    ERROR_FAILURE
//    ERROR_NOT_ENOUGH_MEMORY
//
//  COMMENTS:
//
//    This functions performs the following actions in 3 threads
//    1) Enumerates Network providers - done by enumerating a NULL container
//
//    2) Enumerates Domains - Domains are only a concept of the LanMan
//       provider, so we go to the registry to get the actual provider 
//       string, and then enumerate the provider
//
//    3) Enumerates Servers in the Local Machine's Domain - The local
//       machine's domain is found by calling NetWkstaGetInfo ( this causes
//       the need for unicode ).  It is then enumerated.
//
//    As long as the 1st thread successfully completes, the function will
//    will notify the dialog box that the enumeration completed
//
DWORD WINAPI InitialEnumThread( LPENUMSYNC lpSync )
{
    DWORD dwRet = ERROR_SUCCESS;
    DWORD dwTemp = ERROR_SUCCESS;
    DWORD dwWait;
    LPWKSTA_INFO_100 lpWksta = NULL;
    HKEY  hkey;
    DWORD cbProv = 0;
    TCHAR *lpProv = NULL;
    HANDLE lpHandles[NUM_HANDLES] = { INVALID_HANDLE_VALUE,
                                      INVALID_HANDLE_VALUE,
                                      INVALID_HANDLE_VALUE };
    DWORD  tID;
    ENUMOBJECT eo_prov;
    ENUMOBJECT eo_dom;
    ENUMOBJECT eo_serv;
    NETRESOURCE netr_dom;
    NETRESOURCE netr_serv;
    ENUMNODE en_dom;
    ENUMNODE en_serv;
    DWORD ndx;

    //***  enumerate the network providers  ***//
    eo_prov.lpSync = lpSync;
    eo_prov.lpNode = lpSync->lpRoot;

    // start provider enumeration thread
    InterlockedIncrement( &((LONG)lpSync->cThread) );
    if ( ( lpHandles[HANDLE_PROV]=CreateThread(NULL, 100,
                                     (LPTHREAD_START_ROUTINE)EnumContainerThread,
                                     (LPVOID) &eo_prov,
                                     0, &tID) ) == NULL )
    {
        dwRet = ERROR_FAILURE;
        InterlockedDecrement( &((LONG)lpSync->cThread) );
        goto Cleanup;
    }


    //*** enumerate the domains ***//
    // get the LanmanWorkstation provider name from registry
    if ( ERROR_SUCCESS == ( dwRet = RegOpenKey( HKEY_LOCAL_MACHINE,
                        TEXT("System\\CurrentControlSet\\Services\\LanmanWorkstation\\NetworkProvider"),
                        &hkey ) ) )
    {
        if ( ERROR_SUCCESS == (dwRet = RegQueryValueEx( hkey, TEXT("Name"), NULL, NULL, NULL, &cbProv ) ) )
        {
            if ( ERROR_SUCCESS == ( dwRet = MallocMem( (LPVOID *) &lpProv, cbProv ) ) )
            {
                dwRet = RegQueryValueEx( hkey, TEXT("Name"), NULL, NULL, (LPBYTE) lpProv, &cbProv );
            }
        }

        RegCloseKey( hkey );
    }
    if ( dwRet != ERROR_SUCCESS )
    {
        goto Cleanup;
    }

    // setup netresource structure from domain enumeration
    netr_dom.lpRemoteName = lpProv;
    netr_dom.dwScope = RESOURCE_GLOBALNET;
    netr_dom.dwType = RESOURCETYPE_ANY;
    netr_dom.dwDisplayType = RESOURCEDISPLAYTYPE_GENERIC;
    netr_dom.dwUsage = RESOURCEUSAGE_CONTAINER | RESOURCEUSAGE_RESERVED;
    netr_dom.lpLocalName = NULL;
    netr_dom.lpComment = NULL;
    netr_dom.lpProvider = lpProv;

    en_dom.lpNet = &netr_dom;
    en_dom.cChild = 0;
    en_dom.lpChild = NULL;

    eo_dom.lpSync = lpSync;
    eo_dom.lpNode = &en_dom;

    // start domain enumeration thread
    InterlockedIncrement( &((LONG)lpSync->cThread) );
    if ( ( lpHandles[HANDLE_DOM]=CreateThread(NULL, 100,
                                     (LPTHREAD_START_ROUTINE)EnumContainerThread,
                                     (LPVOID) &eo_dom,
                                     0, &tID) )== NULL )
    {
        dwRet = ERROR_FAILURE;
        InterlockedDecrement( &((LONG)lpSync->cThread) );
        goto Cleanup;
    }


    //*** enumerate servers ***/
    // get the domain for this server, then enumerate to get servers
    if ( NERR_Success != NetWkstaGetInfo(NULL, 100, (LPBYTE *) &lpWksta) )
    {
        dwRet = ERROR_FAILURE;
        goto Cleanup;
    }

    // setup netresource structure
    netr_serv.lpRemoteName = lpWksta->wki100_langroup;
    netr_serv.dwScope = RESOURCE_GLOBALNET;
    netr_serv.dwType = RESOURCETYPE_ANY;
    netr_serv.dwDisplayType = RESOURCEDISPLAYTYPE_DOMAIN;
    netr_serv.dwUsage = RESOURCEUSAGE_CONTAINER;
    netr_serv.lpLocalName = NULL;
    netr_serv.lpComment = NULL;
    netr_serv.lpProvider = NULL;

    en_serv.lpNet = &netr_serv;
    en_serv.cChild = 0;
    en_serv.lpChild = NULL;

    eo_serv.lpSync = lpSync;
    eo_serv.lpNode = &en_serv;

    // start server enumeration thread
    InterlockedIncrement( &((LONG)lpSync->cThread) );
    if ( ( lpHandles[HANDLE_SERV]=CreateThread(NULL, 100,
                                     (LPTHREAD_START_ROUTINE)EnumContainerThread,
                                     (LPVOID) &eo_serv,
                                     0, &tID) )== NULL )
    {
        dwRet = ERROR_FAILURE;
        InterlockedDecrement( &((LONG)lpSync->cThread) );
        goto Cleanup;
    }


    //*** all threads started - now wait for them
    if ( (dwWait = WaitForMultipleObjects( NUM_HANDLES, lpHandles, TRUE, INFINITE ) ) == WAIT_FAILED )
    {
        dwRet = dwWait;
        goto Cleanup;
    }

    // if an error occuring during top level enum, there's
    // no hope, so shut down and signal error
    GetExitCodeThread( lpHandles[HANDLE_PROV], &dwRet );
    if ( dwRet == ERROR_SUCCESS )
    {
        GetExitCodeThread( lpHandles[HANDLE_DOM], &dwRet );
        if ( dwRet == ERROR_SUCCESS )
        {
            // now we just need to attach the domain list to the appropriate provider
            for ( ndx = 0; ndx < lpSync->lpRoot->cChild; ndx++ )
            {
                if ( _tcscmp( lpSync->lpRoot->lpChild[ndx].lpNet->lpRemoteName,
                              lpProv ) == 0 )
                {
                    lpSync->lpRoot->lpChild[ndx].lpChild = en_dom.lpChild;
                    lpSync->lpRoot->lpChild[ndx].cChild = en_dom.cChild;
                    break;
                }
            }

            GetExitCodeThread( lpHandles[HANDLE_SERV], &dwRet );
            if ( dwRet == ERROR_SUCCESS )
            {
                // now we just need to attach the server list to the appropriate domain
                for ( ndx = 0; ndx < eo_dom.lpNode->cChild; ndx++ )
                    if ( _tcscmp( eo_dom.lpNode->lpChild[ndx].lpNet->lpRemoteName,
                                 lpWksta->wki100_langroup ) == 0 )
                    {
                        eo_dom.lpNode->lpChild[ndx].lpChild = en_serv.lpChild;
                        eo_dom.lpNode->lpChild[ndx].cChild = en_serv.cChild;
                        break;
                    }
            }
        }
        else  // domain enumeration failed so free server memory
        {
            GetExitCodeThread( lpHandles[HANDLE_SERV], &dwTemp );
            if ( dwTemp == ERROR_SUCCESS )
                FreeEnumChild( &en_serv );
        }
    }
    else // provider enumeration failed
    {
        GetExitCodeThread( lpHandles[HANDLE_DOM], &dwTemp );
        if ( dwTemp == ERROR_SUCCESS )
            FreeEnumChild( &en_dom );

        GetExitCodeThread( lpHandles[HANDLE_SERV], &dwTemp );
        if ( dwTemp == ERROR_SUCCESS )
            FreeEnumChild( &en_serv );

    }


    Cleanup:
        if ( lpHandles[HANDLE_SERV] )
            CloseHandle( lpHandles[HANDLE_SERV] );

        if ( lpWksta )
            NetApiBufferFree(lpWksta);

        if ( lpHandles[HANDLE_DOM] )
            CloseHandle( lpHandles[HANDLE_DOM] );

        if ( lpProv )
            FreeMem( (LPVOID *) &lpProv );

        if ( lpHandles[HANDLE_PROV] )
            CloseHandle( lpHandles[HANDLE_PROV] );

        FreeEnumSync( &lpSync );
        if ( lpSync ) // if this is NULL, the dialog box doesn't exist anymore
            PostMessage( lpSync->hwnd, WM_INIT_ENUM, 0, 0 );

    return dwRet;

}


//
//  FUNCTION: EnumExpandThread( LPENUMLISTOBJECT lpListObject )
//
//  PURPOSE:  Expands a entries in listbox by 1 level
//
//  PARAMETERS:
//    lpListObject - pointer to entry to expand
//
//  RETURN VALUE:
//    ERROR_SUCCESS
//    ERROR_NOT_ENOUGH_MEMORY
//
//  COMMENTS:
//    Calls EnumContainerThread to expand the object by 1 level
//    and then post a message to the dialog
//
DWORD WINAPI EnumExpandThread( LPENUMLISTOBJECT lpListObject )
{
    DWORD dwRet;

    InterlockedIncrement( &((LONG)lpListObject->Object.lpSync->cThread) );
    dwRet = EnumContainerThread( &(lpListObject->Object) );

    FreeEnumSync( &(lpListObject->Object.lpSync) );

    if ( lpListObject->Object.lpSync )
        PostMessage( lpListObject->Object.lpSync->hwnd,
                     WM_ENUM_EXPAND,
                     (WPARAM) lpListObject->nItem,
                     (LPARAM) lpListObject->Object.lpNode );

    FreeMem( &lpListObject );

    return dwRet;
}


//
//  FUNCTION: EnumContainerThread( LPENUMOBJECT lpObject )
//
//  PURPOSE:  Expands a network container by 1 level
//
//  PARAMETERS:
//    lpObject - pointer to ENUMOBJECT to enumeration
//
//  RETURN VALUE:
//    ERROR_SUCCESS
//    ERROR_NOT_ENOUGH_MEMORY
//
//  COMMENTS:
//
DWORD WINAPI EnumContainerThread( LPENUMOBJECT lpObject )
{
    DWORD  dwRet = ERROR_SUCCESS;
    HANDLE hEnum;
    DWORD  cTotal = 0;

    DWORD       cChild = 0; // count of children
    LPENUMNODE  lpenChild = NULL;

    DWORD cNetBuf = 0; // count of net buffers
    LPNETRESOURCE * lplpnetBuf = NULL; // array of enumerated buffers

    if ( lpObject->lpNode->cChild == 0 )
    {
        dwRet  = WNetOpenEnum( RESOURCE_GLOBALNET,
                               RESOURCETYPE_ANY,
                               RESOURCEUSAGE_CONTAINER,
                               lpObject->lpNode->lpNet,
                               &hEnum );

        if ( dwRet == NO_ERROR )
        {
            while ( dwRet == NO_ERROR )
            {
                DWORD  cEntries =  (DWORD)-1;  // as many entries as will fit
                DWORD  cbBuf = 8192;

                // allocate pointer for buffer
                if ( ERROR_NOT_ENOUGH_MEMORY == ( dwRet = ReallocMem( (LPVOID *) &lplpnetBuf, ++cNetBuf*sizeof(LPNETRESOURCE) ) ) )
                    break;

                // allocate buffer
                if ( ERROR_NOT_ENOUGH_MEMORY == ( dwRet = MallocMem( (LPVOID *) (lplpnetBuf+cNetBuf-1), cbBuf ) ) )
                    break;

                if ( NO_ERROR == (dwRet = WNetEnumResource( hEnum, &cEntries, lplpnetBuf[cNetBuf-1], &cbBuf ) ) )
                {
                    // reallocate buffer to size needed
                    if ( ERROR_NOT_ENOUGH_MEMORY == ( dwRet = ReallocMem( (LPVOID *) (lplpnetBuf+cNetBuf-1), cbBuf) ) )
                        break;

                    // allocate array of ENUMNODEs
                    if ( ERROR_NOT_ENOUGH_MEMORY == ( dwRet = ReallocMem( (LPVOID *) &lpenChild, (cChild+cEntries)*sizeof(ENUMNODE) ) ) )
                        break;

                    while( cEntries-- )
                    {   // remote any leading \\'s
                        while( *((lplpnetBuf[cNetBuf-1]+cEntries)->lpRemoteName) == '\\' )
                            (lplpnetBuf[cNetBuf-1]+cEntries)->lpRemoteName++;

                        lpenChild[cChild].lpNet = lplpnetBuf[cNetBuf-1]+cEntries;
                        lpenChild[cChild].cChild = 0;
                        lpenChild[cChild].lpChild = NULL;
                        cChild++;
                    }
                }
                else
                {
                    cNetBuf--;
                    FreeMem( (LPVOID *) (lplpnetBuf+cNetBuf) );
                    // this will never fail because we shrink the buffer
                    ReallocMem( (LPVOID *) &lplpnetBuf, cNetBuf*sizeof(LPNETRESOURCE) );
                }

            }
            WNetCloseEnum( hEnum );

            if ( ( dwRet == ERROR_NO_MORE_ITEMS ) && cNetBuf )
            {
                qsort( (void *)lpenChild, (size_t)cChild, sizeof(ENUMNODE), CompareEnumNode );

                lpObject->lpNode->cChild = cChild;
                lpObject->lpNode->lpChild = lpenChild;

                EnterCriticalSection( &(lpObject->lpSync->cs) );
                    if ( ERROR_SUCCESS ==
                          ( dwRet = ReallocMem((LPVOID *) &(lpObject->lpSync->lplpnetBuf),
                                               (lpObject->lpSync->cNetBuf+cNetBuf)*sizeof(LPNETRESOURCE) ) ) )
                    {
                        while( cNetBuf-- )
                            lpObject->lpSync->lplpnetBuf[lpObject->lpSync->cNetBuf++] = lplpnetBuf[cNetBuf];
                    }
                LeaveCriticalSection( &(lpObject->lpSync->cs) );
            }

            if ( dwRet == ERROR_NOT_ENOUGH_MEMORY )
            {
                if ( lplpnetBuf != NULL )
                {
                    while( cNetBuf-- )
                        FreeMem( (LPVOID *) lplpnetBuf+cNetBuf );

                    FreeMem( (LPVOID *) &lpenChild );
                }
            }

            // because we made a copy of this buffer, we can free it
            FreeMem( (LPVOID *) &lplpnetBuf );

        }
    }

    FreeEnumSync( &(lpObject->lpSync) );

    return dwRet;
}
