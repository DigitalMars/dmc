//+---------------------------------------------------------------------------
//
//  Microsoft Windows
//  Copyright (C) Microsoft Corporation, 1992 - 1995.
//
//  File:       chngdim.h
//
//  Contents:   definition for the Change Graph Dimension dialog box class
//
//  Classes:    CChangeDim
//
//  Functions:
//
//  History:    4-20-94   stevebl   Created
//
//----------------------------------------------------------------------------

#ifndef __CHNGDIM_H__
#define __CHNGDIM_H__

#include "cdialog.h"
#include "fclass.h"

#ifdef __cplusplus

//+---------------------------------------------------------------------------
//
//  Class:      CChangeDim
//
//  Purpose:    implements the Change Graph Size dialog box
//
//  Interface:  CChangeDim    -- constructor
//              DialogProc    -- dialog proc
//              CheckButtons  -- updates the radio button state
//
//  History:    4-20-94   stevebl   Created
//
//----------------------------------------------------------------------------

class CChangeDim: public CHlprDialog
{
public:
    CChangeDim(CFractalWindow * pwf)
    {
        _pwf = pwf;
    };
    BOOL DialogProc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);
    void CheckButtons(HWND hwndDlg, int iWidth, int iHeight);

protected:
    CFractalWindow * _pwf;
    unsigned _uImageWidth, _uImageHeight;
};

#endif //__cplusplus

#endif //__CHNGDIM_H__


