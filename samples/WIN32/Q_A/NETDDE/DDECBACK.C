// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
// PARTICULAR PURPOSE.
//
// Copyright (C) 1992-1995  Microsoft Corporation.  All Rights Reserved.
//
//  MODULE:   ddecback.c
//
//  PURPOSE:   Contains the DDEML Callback procedure
//
//  FUNCTIONS:
//    DdeCallback - callback routine for the ddeml conversation
//
//  COMMENTS:
//
//  AUTHOR:  Craig Link - Microsoft Developer Support
//

#include <windows.h>            // required for all Windows applications
#include <windowsx.h>
#ifdef WIN16
#include "win16ext.h"           // required only for win16 applications
#endif
#include "globals.h"            // prototypes specific to this application
#include "mem.h"
#include "ddecback.h"



//  FUNCTION: DdeCallback ( UINT, UINT, HCONV, HSZ, HSZ, HDDEDATA,
//                          DWORD, DWORD )
//
//  PURPOSE:  Process ddeml conversation messages
//
//  PARAMETERS:
//    lplpv - address of pointer to newly allocated memory
//    dwSize - number of bytes to allocate
//
//  RETURN VALUE:
//    ERROR_SUCCESS
//    ERROR_NOT_ENOUGH_MEMORY
//
//
HDDEDATA CALLBACK DdeCallback (UINT iType, UINT iFmt, HCONV hConv,
                               HSZ hsz1, HSZ hsz2, HDDEDATA hData,
                               DWORD dwData1, DWORD dwData2)
{
    char szBuffer[128];
    LPSTR psz;
    DWORD cbBytes;

    switch (iType)
    {

        //** DDEML messages handled by client side of the conversation **//
        case XTYP_ADVDATA:
            // Check for matching format and data item
            if (iFmt != CF_TEXT)
                return (HDDEDATA) DDE_FNOTPROCESSED ;

            if ( g_fState == STATE_PENDING )
            {
                PostMessage( g_hwnd, WM_CONNECT, 0, (LPARAM) hConv );
                return (HDDEDATA) DDE_FACK ;
            }
            
            // get data being advised
            cbBytes = DdeGetData (hData, NULL, 0, 0) ;
            if ( MallocMem( (LPVOID *) &psz, cbBytes ) == ERROR_SUCCESS )
            {
                DdeGetData( hData, psz, cbBytes, 0 );
                PostMessage( g_hwnd, WM_RECEIVE_TEXT, (WPARAM)cbBytes, (LPARAM)psz );
            }
            return (HDDEDATA) DDE_FACK ;
            break;


        //** DDEML messages handled by both sides of the conversation **//
        case XTYP_DISCONNECT:
            PostMessage( g_hwnd, WM_DISCONNECT, 0L, (LPARAM)hConv );
            return (HDDEDATA) TRUE ;
            break;


        //** DDEML messages handled by server side of the conversation **//
        case XTYP_CONNECT:
            // hsz1 = topic, hsz2 = service
            if ( g_fState  != STATE_UNCONN)
                return (HDDEDATA) FALSE;

            // validate service for connection
            DdeQueryString (g_idInst, hsz2, (LPSTR)szBuffer, 128, 0) ;

            if (0 != lstrcmp ( (LPSTR) szBuffer, (LPSTR)szAppName))
                return (HDDEDATA) FALSE ;

            // validate topic for connection
            DdeQueryString (g_idInst, hsz1, (LPSTR)szBuffer, 128, 0) ;

            if (0 != lstrcmp ( (LPSTR) szBuffer, (LPSTR)g_szTopic))
                return (HDDEDATA) FALSE ;

            g_fState = STATE_SERVER;
            return (HDDEDATA) TRUE;
            break;

        case XTYP_CONNECT_CONFIRM:
            PostMessage( g_hwnd, WM_CONNECT, 0, (LPARAM) hConv );
            return (HDDEDATA) TRUE;
            break;

        case XTYP_POKE:
            if (iFmt != CF_TEXT)
                return (HDDEDATA) DDE_FNOTPROCESSED;

            // get data being poked
            cbBytes = DdeGetData (hData, NULL, 0, 0) ;
            if ( MallocMem( (LPVOID *) &psz, cbBytes ) == ERROR_SUCCESS )
            {
                DdeGetData( hData, psz, cbBytes, 0 );

                if ( STR_DWORD(psz) == (DWORD) -1 ) // computer name poked
                {
                    wsprintf( (LPSTR)g_szServer, psz );
                    PostMessage( g_hwnd, WM_CONNECT, 0, (LPARAM)hConv );
                }
                else
                    PostMessage( g_hwnd, WM_RECEIVE_TEXT, (WPARAM) cbBytes, (LPARAM)psz );

                return (HDDEDATA) DDE_FACK;
            }
            break;

        case XTYP_ADVSTART:
#ifdef WIN16
            PostMessage( g_hwnd, WM_COMMAND, (WPARAM) IDE_SEND, MAKELONG( hwndSend, EN_CHANGE ) );
#else            
            PostMessage( g_hwnd, WM_COMMAND, MAKELONG( IDE_SEND, EN_CHANGE ), (LPARAM) hwndSend );
#endif
            return (HDDEDATA) TRUE;
            break;

        case XTYP_ADVREQ:  // send data
            return GetDataToTrans();
            break;

        case XTYP_ADVSTOP:  // stop advising client
            return (HDDEDATA) TRUE ;
            break;

     }

     return (HDDEDATA) 0 ;
}
