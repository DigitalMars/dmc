//+---------------------------------------------------------------------------
//
//  Microsoft Windows
//  Copyright (C) Microsoft Corporation, 1992 - 1995.
//
//  File:       strmvwr.h
//
//  Contents:   Stream viewer window class definition.
//
//  Classes:    CStreamView
//
//  Functions:
//
//  History:    6-27-94   stevebl   Created
//
//----------------------------------------------------------------------------

#ifndef __STRMVWR_H__
#define __STRMVWR_H__
#include <cwindow.h>
#include "dfv.h"
#include "mwclass.h"

#ifdef __cplusplus

//+---------------------------------------------------------------------------
//
//  Class:      CStreamView
//
//  Purpose:    implements a stream viewer window
//
//  Interface:  CStreamView  -- constructor
//              InitInstance -- instantiates an instance of the viewer
//
//  History:    6-27-94   stevebl   Created
//
//  Notes:      only the public interface is described by this comment
//
//----------------------------------------------------------------------------

class CStreamView: public CHlprWindow
{
public:
    CStreamView(HWND hlb, LISTBOXELEMENT * ple);
    BOOL InitInstance(HINSTANCE hInst, int nCmdShow);
protected:
    ~CStreamView();
    LRESULT WindowProc(UINT uMsg, WPARAM wParam, LPARAM lParam);
private:
    HWND _hlbParent;
    HWND _hlb;
    LONG _lIndex;
    LISTBOXELEMENT * _ple;
    COLORREF _crTextHigh;
    COLORREF _crBkHigh;
    COLORREF _crTextNorm;
    COLORREF _crBkNorm;
    WORD _wWidth;
    DWORD _cbSize;
    TEXTMETRIC _tm;
    HFONT _hfListBox;
};

#endif // __cplusplus
#endif // __STRMVWR_H__
