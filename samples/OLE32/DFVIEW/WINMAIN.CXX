//+---------------------------------------------------------------------------
//
//  Microsoft Windows
//  Copyright (C) Microsoft Corporation, 1992 - 1995.
//
//  File:       winmain.cxx
//
//  Contents:   main entry point for Stereo Shop
//
//  Classes:
//
//  Functions:  WinMain
//
//  History:    6-08-94   stevebl   Created
//
//----------------------------------------------------------------------------

#include "dfv.h"
#include "mwclass.h"
#include <ole2ver.h>

//+---------------------------------------------------------------------------
//
//  Function:   InitApplication
//
//  Synopsis:   initializes the application and registers its window class
//              (called once for all instances)
//
//  Arguments:  [hInstance] - handle to the first instance
//
//  Returns:    TRUE on success
//
//  History:    4-11-94   stevebl   Created for MFract
//              6-08-94   stevebl   Stolen from MFract
//
//----------------------------------------------------------------------------

BOOL InitApplication(HINSTANCE hInstance)
{
    WNDCLASS wc;

    wc.style = 0;
    wc.lpfnWndProc = &WindowProc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = hInstance;
    wc.hIcon = LoadIcon(hInstance, TEXT("StreamIcon"));
    wc.hCursor = (HCURSOR) LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH) GetStockObject(WHITE_BRUSH);
    wc.lpszMenuName = NULL;
    wc.lpszClassName = TEXT(STREAM_VIEW_CLASS_NAME);
    ATOM aReturn = RegisterClass(&wc);
    if (!aReturn)
    {
        return(aReturn);
    }
    wc.style = CS_DBLCLKS;
    wc.hIcon = LoadIcon(hInstance, TEXT("AppIcon"));
    wc.lpszMenuName = TEXT(MAIN_WINDOW_CLASS_MENU_STR);
    wc.lpszClassName = TEXT(MAIN_WINDOW_CLASS_NAME);
    return(RegisterClass(&wc));
}

//+---------------------------------------------------------------------------
//
//  Function:   WinMain
//
//  Synopsis:   main window proceedure
//
//  Arguments:  [hInstance]     - instance handle
//              [hPrevInstance] - handle of the previous instance (if any)
//              [lpCmdLine]     - pointer to the command line
//              [nCmdShow]      - show state
//
//  History:    4-11-94   stevebl   Created for MFract
//              6-08-94   stevebl   Stolen from MFract
//
//  Notes:      initializes application and starts message loop
//
//----------------------------------------------------------------------------

extern "C" int PASCAL WinMain(HINSTANCE hInstance,
            HINSTANCE hPrevInstance,
            LPSTR lpCmdLine,
            int nCmdShow)
{
    DWORD dwBuildVersion = CoBuildVersion();
    if (HIWORD(dwBuildVersion) != rmm || LOWORD(dwBuildVersion) < rup)
    {
        // alert the caller that the OLE version is incompatible
        // with this build.
        MessageBoxFromStringIdsAndArgs(
            NULL,
            hInstance,
            IDS_OLEINCOMPATIBLE,
            IDS_ERROR,
            MB_OK | MB_ICONEXCLAMATION,
            rmm, rup);
        return(FALSE);
    }
    if (FAILED(CoInitialize(NULL)))
    {
        // alert the caller that OLE couldn't be initialized
        MessageBoxFromStringIds(
            NULL,
            hInstance,
            IDS_OLEINITFAILED,
            IDS_ERROR,
            MB_OK | MB_ICONEXCLAMATION);
        return(FALSE);
    }
    if (!hPrevInstance)
    {
        if (!InitApplication(hInstance))
        {
            CoUninitialize();
            return(FALSE);
        }
    }
    CMainWindow * pw = new CMainWindow;
    if (pw == NULL)
    {
        CoUninitialize();
        return(FALSE);
    }
    if (!pw->InitInstance(hInstance, nCmdShow))
    {
        // Note, if InitInstance has failed then it would have
        // already deleted pw for me so I don't delete it here.
        // This is because when WM_CREATE returns -1 (failure)
        // Windows sends the WM_DESTROY message to the window
        // and the the CHlprWindow class destroys itself whenever
        // it receives this message.
        CoUninitialize();
        return(FALSE);
    }

    // check command line and try and load as a file

    if (strlen(lpCmdLine) > 0)
    {
#ifdef UNICODE
        mbstowcs(pw->_szFileName, lpCmdLine, MAX_PATH - 1);
#else
        strncpy(pw->_szFileName, lpCmdLine, MAX_PATH - 1);
#endif
        pw->OpenFile();
    }

    MSG msg;
    HACCEL haccel = LoadAccelerators(hInstance, TEXT("AppAccel"));
    if (haccel == NULL)
    {
        CoUninitialize();
        return(FALSE);
    }

    while (GetMessage(&msg, NULL, 0, 0))
    {
        if (!TranslateAccelerator(
            pw->GetHwnd(),
            haccel,
            &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }
    CoUninitialize();
    return(msg.wParam);
}


