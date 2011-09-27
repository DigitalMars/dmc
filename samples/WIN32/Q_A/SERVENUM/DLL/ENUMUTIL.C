// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
// PARTICULAR PURPOSE.
//
// Copyright (C) 1992-1995  Microsoft Corporation.  All Rights Reserved.
//
//  MODULE:   enumutil.c
//
//  PURPOSE:   Various utility functions used by the enumdlg dialogbox
//
//  FUNCTIONS:
//    FreeEnumSync - frees ENUMSYNC object
//    FreeEnumChild - recusively frees children of an ENUMNODE
//    CompareEnumNode - compares lpRemoteNames of ENUMNODEs
//    EnumListRecurse - Recursively adds ENUMNODE entries to listbox
//    EnumListExpand - Creates a Thread to enumerate an ENUMNODEs children
//    EnumListCollapse - Removes ENUMNODE children from listbox
//    LoadBitmapArray - loads bitmaps used by owner draw listbox
//
//
//  AUTHOR: Craig Link - Microsoft Developer Support
//

#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <tchar.h>
#include <lm.h>
#include "enumutil.h"
#include "enumthrd.h"
#include "thrdutil.h"
#include "miscutil.h"
#include "mem.h"

extern HMODULE g_hMod;

//
//  FUNCTION: FreeEnumSync( LPENUMSYNC * lplpSync )
//
//  PURPOSE:  Decrements usage count and frees ENUMSYNC object
//
//  PARAMETERS:
//    lplpSync - address of pointer to ENUMSYNC object
//
//  RETURN VALUE:
//    none
//
//  COMMENTS:
//    Before the create of any enumeration threads, cThread should
//    be incremented.  Before the thread returns, it should call
//    FreeEnumSync to reduce the usage count
//
VOID FreeEnumSync( LPENUMSYNC * lplpSync )
{
    // if usage count is 0 than free object
    if ( InterlockedDecrement( &((LONG)(*lplpSync)->cThread) ) == 0 )
    {
        DeleteCriticalSection( &((*lplpSync)->cs) );

        if ( (*lplpSync)->lpRoot )
        {
            // this frees lpObject->lpNode as well
            FreeEnumChild( (*lplpSync)->lpRoot );
            FreeMem( &((*lplpSync)->lpRoot) );
        }

        if ( (*lplpSync)->cNetBuf )
        {
            while( (*lplpSync)->cNetBuf-- )
                FreeMem( (LPVOID *) ((*lplpSync)->lplpnetBuf + (*lplpSync)->cNetBuf) );

            FreeMem( (LPVOID *) &((*lplpSync)->lplpnetBuf) );
        }

        FreeMem( (LPVOID *) lplpSync );
    }
}


//
//  FUNCTION: FreeEnumChild( LPENUMNODE lpNode )
//
//  PURPOSE:  Frees all child nodes and the buffer containing their pointers
//
//  PARAMETERS:
//    lpNode - address of pointer to ENUMNODE object
//
//  RETURN VALUE:
//    none
//
//  COMMENTS:
//
VOID FreeEnumChild( LPENUMNODE lpNode )
{
    if ( lpNode->cChild )
    {
        while( lpNode->cChild-- )
            FreeEnumChild( lpNode->lpChild + lpNode->cChild );

    FreeMem( (LPVOID *) &(lpNode->lpChild) );
    }
}


//
//  FUNCTION: CompareEnumNode( const void *lpNode1, const void *lpNode2 )
//
//  PURPOSE:  Compares the lpRemoteName strings of the ENUMNODE objects
//
//  PARAMETERS:
//    lpNode1 - pointer to an ENUMNODE object
//    lpNode2 - pointer to an ENUMNODE object
//
//  RETURN VALUE:
//    value < 0    lpNode1 proceeds lpNode2
//    value = 0    lpNode1 is identicall to lpNode2
//    value > 0    lpNode2 proceeds lpNode1
//
//  COMMENTS:
//    This is used by the call to QSORT
//
int __cdecl CompareEnumNode( const void *lpNode1, const void *lpNode2 )
{
    return _tcsicmp( ((LPENUMNODE)lpNode1)->lpNet->lpRemoteName,
                     ((LPENUMNODE)lpNode2)->lpNet->lpRemoteName );
}


//
//  FUNCTION: EnumListRecurse( HWND hwnd, LPENUMNODE lpNode, LPTSTR lpServer )
//
//  PURPOSE:  Recursively add the ENUMNODE tree structure to a listbox
//
//  PARAMETERS:
//    hwnd - handle to listbox
//    lpNode - pointer to node from which to being recursion
//    lpServer - server name to match
//
//  RETURN VALUE:
//    listbox index of entry matching lpServer, otherwise 0
//
//  COMMENTS:
//
UINT EnumListRecurse( HWND hwnd, LPENUMNODE lpNode, LPTSTR lpServer )
{
    UINT ndx;
    UINT ndxTemp;
    UINT ndxComputer = 0;

    for ( ndx = 0; ndx < lpNode->cChild; ndx++ )
    {
        switch( lpNode->lpChild[ndx].lpNet->dwDisplayType )
        {
            case RESOURCEDISPLAYTYPE_GENERIC:  // network provider
                if ( lpNode->lpChild[ndx].cChild != 0 )
                    lpNode->lpChild[ndx].ndxBmp = PROVEX_NDX;
                else
                    lpNode->lpChild[ndx].ndxBmp = PROV_NDX;

                SendMessage( hwnd, LB_ADDSTRING, 0L, (LPARAM) &(lpNode->lpChild[ndx]) );
                if ( (ndxTemp = EnumListRecurse( hwnd, &(lpNode->lpChild[ndx]), lpServer ) ) != 0 )
                    ndxComputer = ndxTemp;
                break;

            case RESOURCEDISPLAYTYPE_DOMAIN:  // domain
                if ( lpNode->lpChild[ndx].cChild != 0 )
                    lpNode->lpChild[ndx].ndxBmp = DOMEX_NDX;
                else
                    lpNode->lpChild[ndx].ndxBmp = DOM_NDX;

                SendMessage( hwnd, LB_ADDSTRING, 0L, (LPARAM) &(lpNode->lpChild[ndx]) );
                if ( (ndxTemp = EnumListRecurse( hwnd, &(lpNode->lpChild[ndx]), lpServer ) ) != 0 )
                    ndxComputer = ndxTemp;
                break;

            case RESOURCEDISPLAYTYPE_SERVER:  // server
                lpNode->lpChild[ndx].ndxBmp = SERV_NDX;
                ndxTemp = SendMessage( hwnd, LB_ADDSTRING, 0L, (LPARAM) &(lpNode->lpChild[ndx]) );
                if ( _tcscmp( lpNode->lpChild[ndx].lpNet->lpRemoteName, lpServer ) == 0 )
                    ndxComputer = ndxTemp;
                break;
        }

    }

    return ndxComputer;
}


//
//  FUNCTION: EnumListExpand( HWND hwnd, LPENUMSYNC lpSync, LPENUMNODE lpNode, UINT ndx )
//
//  PURPOSE:  CreateThread to expand the selected network object
//            by one level 
//
//  PARAMETERS:
//    hwnd - handle to listbox
//    lpSync - pointer to syncronization object
//    lpNode - pointer to node fto expand
//    ndx - index at which to insert entries in the listbox
//
//  RETURN VALUE:
//    none
//
//  COMMENTS:
//
VOID EnumListExpand( HWND hwnd, LPENUMSYNC lpSync, LPENUMNODE lpNode, UINT ndx )
{
    HANDLE hThread;
    DWORD  tID;
    LPENUMLISTOBJECT lpListObject;

    if ( ERROR_SUCCESS == MallocMem( &lpListObject, sizeof( ENUMLISTOBJECT ) ) )
    {
        lpListObject->nItem = ndx;
        lpListObject->Object.lpSync = lpSync;
        lpListObject->Object.lpNode = lpNode;

        InterlockedIncrement( &((LONG)lpSync->cThread) );
        if ( ( hThread = CreateThread(NULL, 100,
                                     (LPTHREAD_START_ROUTINE)EnumExpandThread,
                                     (LPVOID) lpListObject,
                                     0, &tID) ) == NULL )
        {
            FreeMem( &lpListObject );
            InterlockedDecrement( &((LONG)lpSync->cThread) );
        }
        else
        {
            MarkOutstanding( &hThread ); // add to list of outstanding threads
        }
    }
}


//
//  FUNCTION: EnumListCollapse( HWND hwnd, LPENUMNODE lpNode, UINT ndx )
//
//  PURPOSE:  Collapses all children under selected network container
//
//  PARAMETERS:
//    hwnd - handle to listbox
//    lpNode - pointer to node fto expand
//    ndx - index of node to collapse
//
//  RETURN VALUE:
//    none
//
//  COMMENTS:
//    This deletes strings until we encounter LB_ERR or 
//    encounter a ENUMNODE of the same level
//
VOID EnumListCollapse( HWND hwnd, LPENUMNODE lpNode, UINT ndx )
{
    LPENUMNODE lpChild;
    SetCursor( LoadCursor( NULL, IDC_WAIT ) );
    SendMessage( hwnd, WM_SETREDRAW, FALSE, 0 );

    while ( LB_ERR != SendMessage( hwnd, LB_GETTEXT, (WPARAM) ndx+1, (LPARAM) &lpChild) )
    {
        if ( lpNode->lpNet->dwDisplayType == lpChild->lpNet->dwDisplayType )
            break;
        else
            SendMessage( hwnd, LB_DELETESTRING, ndx+1, 0 );
    }

    SendMessage( hwnd, WM_SETREDRAW, TRUE, 0 );
    SetCursor( LoadCursor( NULL, IDC_ARROW ) );
}


//
//  FUNCTION: LoadBitmapArray( HBITMAP * lphbmpArray)
//
//  PURPOSE:  Loads owner draw bitmaps into an array
//            creating highlighted and un hightlighted versions
//
//  PARAMETERS:
//    lphbmpArray - array of HBITMAPs, 2*HIGHLIGHT_NDX in length
//
//  RETURN VALUE:
//    none
//
//  COMMENTS:
//
VOID LoadBitmapArray( HBITMAP * lphbmpArray)
{
    UINT ndxBmp;

    for ( ndxBmp = START_NDX; ndxBmp < HIGHLIGHT_NDX; ndxBmp++ )
        lphbmpArray[ndxBmp] = SetBitmapBkgnd( g_hMod, MAKEINTRESOURCE(ndxBmp+BITMAP_START), GetSysColor(COLOR_WINDOW) );

    for ( ndxBmp = START_NDX; ndxBmp < HIGHLIGHT_NDX; ndxBmp++ )
        lphbmpArray[ndxBmp+HIGHLIGHT_NDX] = SetBitmapBkgnd( g_hMod, MAKEINTRESOURCE(ndxBmp+BITMAP_START), GetSysColor( COLOR_HIGHLIGHT ) );
}

