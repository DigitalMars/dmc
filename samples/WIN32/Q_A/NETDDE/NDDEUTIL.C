// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
// PARTICULAR PURPOSE.
//
// Copyright (C) 1992-1995  Microsoft Corporation.  All Rights Reserved.
//
//  MODULE:   nddeutil.c
//
//  PURPOSE:  Handle NetDDE API calls
//
//  FUNCTIONS:
//    SetupNDDEShare - established NetDDE Share
//    ShowNDDEErrMsg - displayes message box of NetDDE error
//
//  COMMENTS:
//
//  AUTHOR: Craig Link - Microsoft Developer Support
//

#include <windows.h>            // required for all Windows applications
#include <windowsx.h>
#include <stdio.h>
#include <stdlib.h>

#ifdef WIN16
#include "win16ext.h"           // required only for win16 applications
#endif

#include "globals.h"            // prototypes specific to this application
#include <nddeapi.h>
#include "nddeutil.h"


// Module specific "globals"  Used when a variable needs to be
// accessed in more than on handler function.


//
//  FUNCTION: SetupNDDEShare ( HWND, LPSTR, LPSTR, LPSTR )
//
//  PURPOSE: Establish NetDDE Share
//
//  PARAMETERS:
//    hwnd - handle of calling window
//    lpszShareName - name of NDDE share
//    lpszApp - name of app supporting DDE conversation
//    lpszTopic - topic supported in DDE converstation
//
//  RETURN VALUE:
//    TRUE if share established
//
//  COMMENTS:
//    Clients are give READ/WRITE permissions to the share
//    to allow for advises and pokes
//
BOOL SetupNDDEShare(
    HWND  hwnd,
    LPSTR lpszShareName,
    LPSTR lpszApp,
    LPSTR lpszTopic )
{

#ifdef WIN16 // Win16 bimplementation
    UINT uRet = 0;
    NDDESHAREINFO ndsiInfo;

    lstrcpy( ndsiInfo.szShareName, lpszShareName );
    ndsiInfo.lpszTargetApp  = lpszApp;
    ndsiInfo.lpszTargetTopic= lpszTopic;
    ndsiInfo.lpbPassword1   = "";
    ndsiInfo.cbPassword1    = 0;
    ndsiInfo.dwPermissions1 = NDDEACCESS_POKE | NDDEACCESS_ADVISE | NDDEACCESS_START_APP;
    ndsiInfo.lpbPassword2   = "";
    ndsiInfo.cbPassword2    = 0;
    ndsiInfo.dwPermissions2 = NDDEACCESS_POKE | NDDEACCESS_ADVISE | NDDEACCESS_START_APP;
    ndsiInfo.lpszItem       = NULL;
    ndsiInfo.cAddItems      = 0;
    ndsiInfo.lpNDdeShareItemInfo = NULL;

    NDdeShareDel ( NULL, lpszShareName, 0 );

    uRet = NDdeShareAdd(NULL, 2, (LPBYTE) &ndsiInfo, sizeof(NDDESHAREINFO));

    if (uRet != NDDE_NO_ERROR )
    {
        ShowNDDEErrMsg (  hwnd, "Error Adding Share", uRet );
        return FALSE;
    }

    return TRUE;


#else  // Win32 implementation
    FARPROC lpfn = NULL;
    BOOL bRet = TRUE;
    UINT uRet = 0;
    NDDESHAREINFO ndsiInfo;
    PSECURITY_DESCRIPTOR pSD = NULL;
    long lShareType = 0;
    long len = 0;
    char szAppTopicList[3*(MAX_APPNAME+MAX_TOPICNAME+2)+1];
    // create app / topic list - this is 3 null terminated strings
    // terminated by another null

    wsprintf(szAppTopicList,
            "%s|%s",
            lpszApp,
            lpszTopic );

    len = lstrlen(szAppTopicList)+1;  // include NULL charater

    wsprintf(szAppTopicList+len,
            "%s|%s",
            lpszApp,
            lpszTopic );

    wsprintf(szAppTopicList+len*2,
            "%s|%s",
            lpszApp,
            lpszTopic );

    szAppTopicList[len*3] = 0;        // terminate with double null


    lShareType = SHARE_TYPE_OLD | SHARE_TYPE_NEW | SHARE_TYPE_STATIC;

    // Get a pointer to the NDdeIsValidAppTopicList() API
    lpfn = GetProcAddress(g_hInstNetDde, TEXT("NDdeIsValidAppTopicListA"));
    if (!lpfn)
    {
        MessageBox(NULL, TEXT("Error calling NetDDE API"), TEXT("NDDECHAT"),
                   MB_OK);
        return (FALSE);
    }

    if( (lpfn)(szAppTopicList)) {

        ndsiInfo.lRevision = 1;
        ndsiInfo.lpszShareName = lpszShareName;
        ndsiInfo.lShareType = lShareType;
        ndsiInfo.lpszAppTopicList = szAppTopicList;
        ndsiInfo.fSharedFlag = TRUE;
        ndsiInfo.fService = FALSE;
        ndsiInfo.fStartAppFlag = TRUE;
        ndsiInfo.nCmdShow = SW_SHOWNORMAL;
        ndsiInfo.cNumItems = 0;
        ndsiInfo.lpszItemList = "";

        // Delete possible old share
        lpfn = GetProcAddress(g_hInstNetDde, TEXT("NDdeShareDelA"));
        (lpfn)( NULL, lpszShareName, 0 );


        // Create a Security descriptor which allows write access
        // This enables pokes from client ( ie writes )

        // Allocate memory for the security descriptor.
        pSD = (PSECURITY_DESCRIPTOR) LocalAlloc(LPTR,
                                        SECURITY_DESCRIPTOR_MIN_LENGTH);

        // Initialize the new security descriptor.
        InitializeSecurityDescriptor(pSD, SECURITY_DESCRIPTOR_REVISION);

        // Add a NULL descriptor ACL to the security descriptor.
        SetSecurityDescriptorDacl(pSD, TRUE, (PACL) NULL, FALSE);

        // create the new share
        lpfn = GetProcAddress(g_hInstNetDde, TEXT("NDdeShareAddA"));
        uRet = (lpfn)( NULL, 2, pSD, (LPBYTE)&ndsiInfo, sizeof(NDDESHAREINFO) );
        if ( uRet == NDDE_NO_ERROR )
        {
            // trust the new share to allow remote access
            lpfn = GetProcAddress(g_hInstNetDde, TEXT("NDdeSetTrustedShareA"));
            uRet = (lpfn)( NULL, lpszShareName,
                           NDDE_TRUST_SHARE_INIT | NDDE_TRUST_SHARE_START);
            if ( uRet != NDDE_NO_ERROR )
            {
                ShowNDDEErrMsg ( hwnd, "Error Trusting Share", uRet );
                bRet = FALSE;
            }
        }
        else
        {
            ShowNDDEErrMsg (  hwnd, "Error Adding Share", uRet );
            bRet = FALSE;
        }

        // free security descriptor
        LocalFree( pSD );
    }
    else
    {
        MessageBox ( hwnd, "Invalid Topic List", szAppName, MB_ICONEXCLAMATION | MB_OK );
        bRet = FALSE;
    }

    return bRet;
#endif  // Win32

}


//
//  FUNCTION: ShowNDDEErrMsg ( HWND hwnd, LPSTR lpstr, UINT uCode )
//
//  PURPOSE: display messagebox containing NDDE error string
//
//  PARAMETERS:
//    hwnd - handle to messgebox owner window
//    lpstr - pointer to message box caption text
//    uCode - NDDE error code
//
//  RETURN VALUE:
//    none
//
//  COMMENTS:
//
VOID ShowNDDEErrMsg ( HWND hwnd, LPSTR lpstr, UINT uCode )
{
    char szBuf[256];
    FARPROC lpfn;

    if ( uCode == NDDE_NO_ERROR )
        return;

    MessageBeep(MB_ICONEXCLAMATION);

    wsprintf( (LPSTR)szBuf, "%s\n", lpstr );
    lpfn = GetProcAddress(g_hInstNetDde, TEXT("NDdeGetErrorStringA"));
    if ( (lpfn) ( uCode,
                  (LPSTR)szBuf+lstrlen((LPSTR)szBuf),
                  255-lstrlen((LPSTR)szBuf) ) == NDDE_NO_ERROR )
    {
        MessageBox (hwnd, szBuf, szAppName, MB_ICONEXCLAMATION | MB_OK );
    }
}

