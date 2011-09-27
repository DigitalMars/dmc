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

#ifndef _ENUMDLG_H
#define _ENUMDLG_H

//-------------------------------------------------------------------------
//  enumdlg dialog defines.

#define IDE_SERVER       400
#define IDL_BROWSE       401
#define IDB_HELP         402
#define IDT_SELECT       403
#define IDC_STATIC       -1
#define WM_INIT_ENUM     WM_USER+100
#define WM_ENUM_EXPAND   WM_USER+101

#define SERVER_BMP       500
#define DOMAIN_BMP       501
#define DOMAINEX_BMP     502
#define PROVIDER_BMP     503
#define PROVIDEREX_BMP   504

#define WORKING          -1


//-----------------------------------------------------------------------
// enumdlg prototypes

LRESULT CALLBACK EnumDlg(HWND, UINT, WPARAM, LPARAM);
LRESULT APIENTRY ListSubProc(HWND, UINT, WPARAM, LPARAM);

#endif
