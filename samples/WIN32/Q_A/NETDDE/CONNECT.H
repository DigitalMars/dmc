// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
// PARTICULAR PURPOSE.
//
// Copyright (C) 1992-1995  Microsoft Corporation.  All Rights Reserved.
//
//  MODULE:   connect.h
//
//  PURPOSE:  contains ConnectDlg declarations
//

#ifndef _CONNECT_H
#define _CONNECT_H

//-------------------------------------------------------------------------
//  Simple connection dialog defines.

#define IDE_SERVERNAME 100

#ifdef WIN16
// Dialog box procedures must be exported in 16-bit applications for Windows.
LRESULT CALLBACK __export ConnectDlg(HWND, UINT, WPARAM, LPARAM);
#else
LRESULT CALLBACK ConnectDlg(HWND, UINT, WPARAM, LPARAM);
#endif

#endif
