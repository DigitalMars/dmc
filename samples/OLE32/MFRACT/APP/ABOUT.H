//+---------------------------------------------------------------------------
//
//  Microsoft Windows
//  Copyright (C) Microsoft Corporation, 1992 - 1995.
//
//  File:       about.h
//
//  Contents:   definition for the About dialog box class
//
//  Classes:    CAbout
//
//  Functions:
//
//  History:    4-12-94   stevebl   Created
//
//----------------------------------------------------------------------------

#ifndef __ABOUT_H__
#define __ABOUT_H__

#include <cdialog.h>

#ifdef __cplusplus

//+---------------------------------------------------------------------------
//
//  Class:      CAbout
//
//  Purpose:    implements the about dialog box
//
//  Interface:  DialogProc -- dialog procedure
//
//  History:    4-28-94   stevebl   Created
//
//----------------------------------------------------------------------------

class CAbout: public CHlprDialog
{
public:
    BOOL DialogProc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);
};

#endif //__cplusplus

#endif //__ABOUT_H__

