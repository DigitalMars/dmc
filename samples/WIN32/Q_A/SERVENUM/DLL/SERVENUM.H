// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
// PARTICULAR PURPOSE.
//
// Copyright (C) 1992-1995  Microsoft Corporation.  All Rights Reserved.
//
//  MODULE:   enumdlg.h
//
//  PURPOSE:  contains enumdlg declarations
//
#ifndef _SERVENUM_H
#define _SERVENUM_H

// MSVC++ 1.1 for NT does not define this in WINERROR.H
#ifndef ERROR_CANCELLED
#define ERROR_CANCELLED 1223L
#endif

#ifdef __cplusplus
extern "C" {
#endif

//
// ServerEnumDialog(
//     HWND hwnd,              // Handle of calling window
//     LPWSTR lpszServer,      // Buffer to store chosen server name
//     LPDWORD lpcchServer,    // Pointer to size of buffer in bytes
//     FARPROC lpfnHelpProc    // User-defined help procedure
//     )
//
//  PURPOSE: Display a dialog box which allows a user to select
//           a computer on the network.  The user may also type
//           in a string.  The result is not guarenteed to be
//           a valid computer name
//
//  PARAMETERS:
//    hwnd - Handle of calling window
//
//    lpszServer - Buffer to store resultant server name in.
//                 This should be of length MAX_COMPUTERNAME_LENGTH+3
//
//    lpcchServer - Pointer to size of lpszServer, including the NULL
//                  terminating character.  The resultant length of the 
//                  string is stored here.  If ERROR_MORE_DATA is return,
//                  the value is the required size of the buffer.
//
//    lpfnHelpProc - Pointer to user-defined help procedure.
//                   The procedure should be prototyped as
//                   int HelpProc( VOID );
//
//  RETURN VALUE:
//    ERROR_SUCCESS - User selected or typed in a server name
//    ERROR_CANCELLED - Dialog box was cancelled
//    ERROR_NOT_ENOUGH_MEMORY - unable to initialize dialog box
//    ERROR_MORE_DATA - lpszServer not large enough
//
//


#ifdef UNICODE
#define ServerEnumDialog ServerEnumDialogW
#else
#define ServerEnumDialog ServerEnumDialogA
#endif



DWORD ServerEnumDialogW( HWND hwnd,
                         LPWSTR lpszServer,
                         LPDWORD lpcchServer,
                         FARPROC lpfnHelpProc
                        );

DWORD ServerEnumDialogA( HWND hwnd,
                         LPSTR lpszServer,
                         LPDWORD lpcchServer,
                         FARPROC lpfnHelpProc
                        );


#ifdef __cplusplus
}
#endif

#endif
