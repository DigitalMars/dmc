// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
// PARTICULAR PURPOSE.
//
// Copyright (C) 1992-1995  Microsoft Corporation.  All Rights Reserved.
//
//  MODULE:   servenum.c
//
//  PURPOSE:   Implements ServerEnumDialog API
//
//  FUNCTIONS:
//    DllMain - handles DLL initialization
//    ServerEnumDialogW - Unicode version of API
//    ServerEnumDialogA - ASCII version of API - calls unicode version
//
//  COMMENTS:
//    Implements APIs for the ServerEnumDialog.
//
//  AUTHOR: Craig Link - Microsoft Developer Support
//

#include <windows.h>
#include "servenum.h"
#include "enumdlg.h"
#include "enumutil.h"
#include "thrdutil.h"
#include "miscutil.h"
#include "mem.h"

DWORD ndxTls;
HMODULE g_hMod;


//
//  FUNCTION: DllMain( HMODULE hMod, DWORD dwReason, LPVOID lpReserved )
//
//  PURPOSE:  Handle DLL issues
//
//  PARAMETERS:
//    hMod - module handle of DLL
//    dwReason - reason DllMain is being called
//    lpReserved - reserved
//
//  RETURN VALUE:
//    TRUE
//    FALSE
//
//  COMMENTS:
//
BOOL WINAPI DllMain( HMODULE hMod, DWORD dwReason, LPVOID lpReserved )
{
    BOOL bRet = TRUE;

    switch ( dwReason )
    {
        case DLL_PROCESS_ATTACH:  
            g_hMod = hMod;
            InitializeCriticalSection( &g_csOutstanding );
            if ( (DWORD)-1 == (ndxTls = TlsAlloc() ) )
            {
               DeleteCriticalSection( &g_csOutstanding );
               bRet = FALSE;
            }
            break;

        case DLL_THREAD_ATTACH:
            break;

        case DLL_THREAD_DETACH:
            break;

        case DLL_PROCESS_DETACH:
            WaitCloseOutstanding();
            DeleteCriticalSection( &g_csOutstanding );
            TlsFree( ndxTls );
            break;
    }

    return bRet;
}


//
//  FUNCTION: ServerEnumDialogW( HWND hwnd, LPWSTR lpszServer, LPDWORD lpcchServer, FARPROC lpfnHelpProc )
//
//  PURPOSE: Display dialogbox which allows a user to select a computer
//
//  PARAMETERS:
//    hwnd - Handle to calling window
//    lpszServer - Buffer to store chosen server name
//    lpcchServer - Size of buffer in bytes
//    lpfnHelpProc - User-defined help procedure
//
//  RETURN VALUE:
//    ERROR_SUCCESS
//    ERROR_CANCELLED
//    ERROR_NOT_ENOUGH_MEMORY
//    ERROR_MORE_DATA
//
//  COMMENTS:
//    Unicode version of the API
//
DWORD ServerEnumDialogW( HWND hwnd, LPWSTR lpszServer, LPDWORD lpcchServer, FARPROC lpfnHelpProc )
{
    HDC hdc;
    HFONT hfontOld;
    DWORD dwRet = ERROR_SUCCESS;
    LPTLSOBJECT lpTls;


    if ( ERROR_SUCCESS == ( dwRet = MallocMem( (LPVOID *) &lpTls, sizeof(TLSOBJECT) ) ) )
    {
        TlsSetValue( ndxTls, (LPVOID) lpTls );

        lpTls->lpszServer = lpszServer;
        lpTls->lpcchServer = lpcchServer;
        lpTls->lpfnHelpProc = lpfnHelpProc;

        lpTls->hfont = CreateStdFont( hwnd, 8 );
        hdc = GetDC( hwnd );
        hfontOld = SelectObject( hdc, lpTls->hfont );
        GetTextMetrics(hdc, &(lpTls->tm) );
        SelectObject( hdc, hfontOld );
        ReleaseDC( hwnd, hdc );

        dwRet = DialogBox(g_hMod, TEXT("EnumDlgBox"), hwnd, (DLGPROC)EnumDlg);

        DeleteObject( lpTls->hfont );

        FreeMem( &lpTls );
    }

    RefreshOutstanding(); // try cleaning up any outstanding threads

    return dwRet;

}


//
//  FUNCTION: ServerEnumDialogW( HWND hwnd, LPWSTR lpszServer, LPDWORD lpcchServer, FARPROC lpfnHelpProc )
//
//  PURPOSE: Display dialogbox which allows a user to select a computer
//
//  PARAMETERS:
//    hwnd - Handle to calling window
//    lpszServer - Buffer to store chosen server name
//    lpcchServer - Size of buffer in bytes
//    lpfnHelpProc - User-defined help procedure
//
//  RETURN VALUE:
//    ERROR_SUCCESS
//    ERROR_CANCELLED
//    ERROR_NOT_ENOUGH_MEMORY
//    ERROR_MORE_DATA
//
//  COMMENTS:
//    ASCII version of the API
//
DWORD ServerEnumDialogA( HWND hwnd, LPSTR lpszServer, LPDWORD lpcchServer, FARPROC lpfnHelpProc )
{
    DWORD dwRet;
    WCHAR szServer[MAX_COMPUTERNAME_LENGTH+3];

    if ( ERROR_SUCCESS == ( dwRet = ServerEnumDialogW( hwnd, szServer, lpcchServer, lpfnHelpProc ) ) )
        WideCharToMultiByte( CP_ACP, 0, szServer, *lpcchServer, lpszServer, *lpcchServer, NULL, NULL );
    return dwRet;
}
