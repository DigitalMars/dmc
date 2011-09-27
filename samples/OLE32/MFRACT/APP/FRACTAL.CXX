//+---------------------------------------------------------------------------
//
//  Microsoft Windows
//  Copyright (C) Microsoft Corporation, 1992 - 1995.
//
//  File:       fractal.cxx
//
//  Contents:   main windows engine
//
//  Classes:
//
//  Functions:  InitApplication
//              WinMain
//              Exists
//
//  History:    4-11-94   stevebl   Created
//
//----------------------------------------------------------------------------

#include <windows.h>
#include <ole2.h>
#include <ole2ver.h>
#include "fractal.h"
#include "fclass.h"

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
//  History:    4-11-94   stevebl   Created
//
//----------------------------------------------------------------------------

BOOL InitApplication(HINSTANCE hInstance)
{
    WNDCLASS wc;

    wc.style = CS_DBLCLKS;
    wc.lpfnWndProc = &WindowProc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = hInstance;
    wc.hIcon = LoadIcon(hInstance, TEXT("AppIcon"));
    wc.hCursor = (HCURSOR) LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH) GetStockObject(WHITE_BRUSH);
    wc.lpszMenuName = TEXT("CFractalWindowMenu");
    wc.lpszClassName = TEXT("CFractalWindow");
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
//  History:    4-11-94   stevebl   Created
//
//  Notes:      initializes application and starts message loop
//
//----------------------------------------------------------------------------

extern "C" int PASCAL WinMain(HINSTANCE hInstance,
            HINSTANCE hPrevInstance,
            LPSTR lpCmdLine,
            int nCmdShow)
{
    DWORD dwBuildVersion = OleBuildVersion();
    if (HIWORD(dwBuildVersion) != rmm || LOWORD(dwBuildVersion) < rup)
    {
        // alert the caller that the OLE version is incompatible
        // with this build.
        TCHAR szTitle[MAX_STRING_LENGTH];
        TCHAR szText[MAX_STRING_LENGTH];
        if (LoadString(hInstance, IDS_ERROR, szTitle, MAX_STRING_LENGTH))
        {
            if (LoadString(hInstance, IDS_OLEINCOMPATIBLE, szText, MAX_STRING_LENGTH))
            {
                MessageBox(
                    NULL,
                    szText,
                    szTitle,
                    MB_OK | MB_ICONEXCLAMATION);
            }
        }
        return(FALSE);
    }
    if (FAILED(OleInitialize(NULL)))
    {
        // alert the caller that OLE couldn't be initialized
        TCHAR szTitle[MAX_STRING_LENGTH];
        TCHAR szText[MAX_STRING_LENGTH];
        if (LoadString(hInstance, IDS_ERROR, szTitle, MAX_STRING_LENGTH))
        {
            if (LoadString(hInstance, IDS_OLEINITFAILED, szText, MAX_STRING_LENGTH))
            {
                MessageBox(
                    NULL,
                    szText,
                    szTitle,
                    MB_OK | MB_ICONEXCLAMATION);
            }
        }
        return(FALSE);
    }
    if (!hPrevInstance)
    {
        if (!InitApplication(hInstance))
        {
            OleUninitialize();
            return(FALSE);
        }
    }
    CFractalWindow * pw = new CFractalWindow;
    if (pw == NULL)
    {
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
        OleUninitialize();
        return(FALSE);
    }
    MSG msg;
    HACCEL haccel = LoadAccelerators(hInstance, TEXT("AppAccel"));
    if (haccel == NULL)
    {
        OleUninitialize();
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
    OleUninitialize();
    return(msg.wParam);
}

//+---------------------------------------------------------------------------
//
//  Function:   Exists
//
//  Synopsis:   simple function to test for the existance of a file
//
//  History:    6-16-93   stevebl   Created
//
//----------------------------------------------------------------------------

int Exists(TCHAR *sz)
{
    HANDLE h;
    h = CreateFile(sz,
        GENERIC_READ,
        FILE_SHARE_READ | FILE_SHARE_WRITE,
        NULL,
        OPEN_EXISTING,
        0,
        0);
    if (h != INVALID_HANDLE_VALUE)
    {
        CloseHandle(h);
        return(1);
    }
    return (0);
}


