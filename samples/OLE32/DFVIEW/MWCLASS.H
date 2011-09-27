//+---------------------------------------------------------------------------
//
//  Microsoft Windows
//  Copyright (C) Microsoft Corporation, 1992 - 1995.
//
//  File:       mwclass.h
//
//  Contents:   definition of the main window class
//
//  Classes:    CMainWindow
//
//  Functions:  Exists
//
//  History:    6-08-94   stevebl   Created
//
//----------------------------------------------------------------------------

#ifndef __MWCLASS_H__
#define __MWCLASS_H__

#include <cwindow.h>
#include <commdlg.h>

#ifdef __cplusplus

int Exists(TCHAR *sz);


// List Box Element Structure
struct TAG_LISTBOXELEMENT
{
#define LBE_STORAGE     STGTY_STORAGE
#define LBE_STREAM      STGTY_STREAM
    unsigned uType;
    BOOL fOpen;
    void * pInterface;
    union {
        ULONG nChildren;
        HWND hwndView;
    };
    unsigned uIndent;
    LPOLESTR pwcsName;
};
typedef struct TAG_LISTBOXELEMENT LISTBOXELEMENT;

//+---------------------------------------------------------------------------
//
//  Class:      CMainWindow
//
//  Purpose:    Code for the main docfile viewer window and the main menu.
//
//  Interface:  CMainWindow          -- constructor
//              InitInstance         -- instantiates the docfile viewer window
//              Toggle               -- toggles a selection
//              Expand               -- expands a selection
//              Collapse             -- collapses a selection
//              ExpandAll            -- expands an entire tree
//              OpenFile             -- trys to load the docfile in _szFileName
//
//  History:    6-27-94   stevebl   Created
//
//  Notes:      only the public interface is listed here
//
//----------------------------------------------------------------------------

class CMainWindow: public CHlprWindow
{
public:
    CMainWindow();
    BOOL InitInstance(HINSTANCE, int);
    void Toggle(long lIndex);
    void Expand(long lIndex);
    void Collapse(long lIndex);
    void ExpandAll(long lIndex);
    void OpenFile(void);
    TCHAR _szFileName[MAX_PATH];
protected:
    ~CMainWindow();
    LRESULT WindowProc(UINT uMsg, WPARAM wParam, LPARAM lParam);
private:
    LRESULT DoMenu(WPARAM wParam, LPARAM lParam);
    LISTBOXELEMENT * CreateStreamElement(IStream * pstm);
    LISTBOXELEMENT * CreateStorageElement(IStorage * pstg);
    void DeleteElement(LISTBOXELEMENT * plbe);
    BOOL LoadBitmaps(void);

    HWND _hlb;
    HBITMAP _hbmpStorage;
    HBITMAP _hbmpStream;
    COLORREF _crTextHigh;
    COLORREF _crBkHigh;
    COLORREF _crTextNorm;
    COLORREF _crBkNorm;
    WORD _wWidth;
    OPENFILENAME _ofnFile;
    TCHAR _szFileTitle[MAX_PATH];
    TEXTMETRIC _tm;
    UINT _cyBitmap;
    TCHAR _szHelpFile[MAX_PATH];
};

#endif // __cplusplus

#endif // __MWCLASS_H__
