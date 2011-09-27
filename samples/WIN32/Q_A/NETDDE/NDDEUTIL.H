// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
// PARTICULAR PURPOSE.
//
// Copyright (C) 1992-1995  Microsoft Corporation.  All Rights Reserved.
//
//  MODULE:   nddeutil.h
//
//  PURPOSE:  contains NetDDE Utility declarations
//

#ifndef _NDDEUTIL_H
#define _NDDEUTIL_H

BOOL SetupNDDEShare( HWND  hwnd, LPSTR pszShareName, LPSTR pszApp, LPSTR pszTopic );
VOID ShowNDDEErrMsg ( HWND hwnd, LPSTR lpstr, UINT uCode );

#endif

