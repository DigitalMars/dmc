//**********************************************************************
// File name: simpsvr.cpp
//
//      Main source file for the simple OLE 2.0 server
//
// Functions:
//
//      WinMain         - Program entry point
//      MainWndProc     - Processes messages for the frame window
//      About           - Processes messages for the about dialog
//      DocWndProc      - Processes messages for the doc window
//
// Copyright (c) 1993 - 1995 Microsoft Corporation. All rights reserved.
//**********************************************************************

#include "precomp.h"
#include "obj.h"
#include "app.h"
#include "doc.h"
#include "icf.h"


CSimpSvrApp* lpCSimpSvrApp;

//**********************************************************************
//
// WinMain
//
// Purpose:
//
//      Program entry point
//
// Parameters:
//
//      HINSTANCE hInstance     - Instance handle for this instance
//
//      HINSTANCE hPrevInstance - Instance handle for the last instance
//
//      LPSTR lpCmdLine         - Pointer to the command line
//
//      int nCmdShow            - Window State
//
// Return Value:
//
//      msg.wParam
//
// Function Calls:
//      Function                        Location
//
//      CSimpSvrApp::CSimpSvrApp        APP.CPP
//      CSimpSvrApp::fInitApplication   APP.CPP
//      CSimpSvrApp::fInitInstance      APP.CPP
//      CSimpSvrApp::HandleAccelerators APP.CPP
//      CSimpSvrApp::~CSimpSvrApp       APP.CPP
//      OleUIInitialize                 OLE2UI
//      OleUIUninitialize               OLE2UI
//      GetMessage                      Windows API
//      TranslateMessage                Windows API
//      DispatchMessage                 Windows API
//
// Comments:
//
//********************************************************************

int PASCAL WinMain(HINSTANCE hInstance,
                   HINSTANCE hPrevInstance,
                   LPSTR     lpCmdLine,
                   int       nCmdShow)
{
    MSG msg;

#ifdef UNICODE
    if (GetVersion() & 0x80000000)  // Unicode version requires NT
    {
        MessageBoxA(NULL,
                    "This application requires Windows NT.",
                    "Simple OLE Server",
                    MB_OK | MB_ICONSTOP);
        return 0;
    }
#endif

    lpCSimpSvrApp = new CSimpSvrApp;

    lpCSimpSvrApp->AddRef();      // need the app ref. count at 1 to hold the
                                  // app alive.

    lpCSimpSvrApp->ParseCmdLine(lpCmdLine);

    // app/instance initialization
    if (!lpCSimpSvrApp->fInitApplication(hInstance, nCmdShow))
    {
        lpCSimpSvrApp->Release();
        return FALSE;
    }

    // message loop
    while (GetMessage(&msg, NULL, NULL, NULL))
    {
        if (lpCSimpSvrApp->IsInPlaceActive())

            // Only key messages need to be sent to OleTranslateAccelerator.  Any other message
            // would result in an extra FAR call to occur for that message processing...

            if ((msg.message >= WM_KEYFIRST) && (msg.message <= WM_KEYLAST))

                // OleTranslateAccelerator MUST be called, even though this application does
                // not have an accelerator table.  This has to be done in order for the
                // mneumonics for the top level menu items to work properly.

                if (OleTranslateAccelerator(lpCSimpSvrApp->GetDoc()->GetObj()->GetInPlaceFrame(),
                                            lpCSimpSvrApp->GetDoc()->GetObj()->GetFrameInfo(),
                                            &msg) == NOERROR)
                    continue;

        TranslateMessage(&msg);    // Translates virtual key codes
        DispatchMessage(&msg);     // Dispatches message to window
    }

    return msg.wParam;             // Returns the value from PostQuitMessage
}


//**********************************************************************
//
// MainWndProc
//
// Purpose:
//
//      Processes messages for the frame window
//
// Parameters:
//
//      HWND hWnd       - Window handle for frame window
//
//      UINT message    - Message value
//
//      WPARAM wParam   - Message info
//
//      LPARAM lParam   - Message info
//
// Return Value:
//
//      long
//
// Function Calls:
//      Function                        Location
//
//      CSimpSvrApp::lCommandHandler    APP.CPP
//      CSimpSvrApp::DestroyDocs        APP.CPP
//      CSimpSvrApp::lCreateDoc         APP.CPP
//      CSimpSvrApp::lSizeHandler       APP.CPP
//      CGameDoc::lAddVerbs             DOC.CPP
//      PostQuitMessage                 Windows API
//      DefWindowProc                   Windows API
//
// Comments:
//
//********************************************************************

LRESULT CALLBACK MainWndProc(HWND   hWnd,
                             UINT   message,
                             WPARAM wParam,
                             LPARAM lParam)
{
    switch (message)
    {
        case WM_CLOSE:
            OutputDebugString(TEXT("*** In WM_CLOSE *** \r\n"));

            // if there is still a document
            if (lpCSimpSvrApp->GetDoc())

                // if there is still an object within a document
                if (lpCSimpSvrApp->GetDoc()->GetObj())   // this case occurs if there is still
                                                         // an outstanding Ref count on the object
                                                         // when the app is trying to go away.
                                                         // typically this case will occur in
                                                         // the "open" editing mode.
                    //  Close the document
                    lpCSimpSvrApp->GetDoc()->Close();

            // hide the app window
            lpCSimpSvrApp->HideAppWnd();

            // if we were started by ole, unregister the class factory, otherwise
            // remove the ref count on our dummy OLE object
            if (lpCSimpSvrApp->IsStartedByOle())
                CoRevokeClassObject(lpCSimpSvrApp->GetRegisterClass());
            else
                lpCSimpSvrApp->GetOleObject()->Release();

            lpCSimpSvrApp->Release();  // This should close the app.

            break;

        case WM_COMMAND:
            return lpCSimpSvrApp->lCommandHandler(hWnd, message, wParam, lParam);
            break;

        case WM_CREATE:
            return lpCSimpSvrApp->lCreateDoc(hWnd, message, wParam, lParam);
            break;

        case WM_DESTROY:
            PostQuitMessage(0);
            break;

        case WM_SIZE:
            return lpCSimpSvrApp->lSizeHandler(hWnd, message, wParam, lParam);

        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0L;
}


//**********************************************************************
//
// About
//
// Purpose:
//
//      Processes dialog box messages
//
// Parameters:
//
//      HWND hWnd       - Window handle for dialog box
//
//      UINT message    - Message value
//
//      WPARAM wParam   - Message info
//
//      LPARAM lParam   - Message info
//
// Return Value:
//
// Function Calls:
//      Function                    Location
//
//      EndDialog                   Windows API
//
// Comments:
//
//********************************************************************

BOOL CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
        case WM_INITDIALOG:
            return TRUE;

        case WM_COMMAND:
            if (GET_WM_COMMAND_ID(wParam,lParam) == IDOK ||
                GET_WM_COMMAND_ID(wParam,lParam) == IDCANCEL)
            {
                EndDialog(hDlg, TRUE);
                return TRUE;
            }
            break;
    }
    return FALSE;
}

//**********************************************************************
//
// DocWndProc
//
// Purpose:
//
//      Processes dialog box messages
//
// Parameters:
//
//      HWND hWnd       - Window handle for doc window
//
//      UINT message    - Message value
//
//      WPARAM wParam   - Message info
//
//      LPARAM lParam   - Message info
//
// Return Value:
//
// Function Calls:
//      Function                            Location
//
//      CSimpSvrApp::PaintApp               APP.CPP
//      CSimpSvrApp::SetStatusText          APP.CPP
//      CSimpSvrApp::lCommandHandler        APP.CPP
//      BeginPaint                          Windows API
//      EndPaint                            Windows API
//      DefWindowProc                       Windows API
//
// Comments:
//
//********************************************************************

LRESULT CALLBACK DocWndProc(HWND   hWnd,
                            UINT   message,
                            WPARAM wParam,
                            LPARAM lParam)
{
    PAINTSTRUCT ps;

    switch (message)
    {
        case WM_COMMAND:
            return lpCSimpSvrApp->lCommandHandler(hWnd, message, wParam, lParam);
            break;

        case WM_PAINT:
            BeginPaint(hWnd, &ps);

            // tell the app class to paint itself
            if (lpCSimpSvrApp)
                lpCSimpSvrApp->PaintApp(ps.hdc);

            EndPaint(hWnd, &ps);
            break;

        case WM_MENUSELECT:
            lpCSimpSvrApp->SetStatusText();
            break;

        /* OLE2NOTE: Any window that is used during in-place activation
        **    must handle the WM_SETCURSOR message or else the cursor
        **    of the in-place parent will be used. if WM_SETCURSOR is
        **    not handled, then DefWindowProc sends the message to the
        **    window's parent.
        */
        case WM_SETCURSOR:
        {
            HCURSOR hcur = (HCURSOR)GetClassLong(hWnd, GCL_HCURSOR);
            if (NULL != hcur)
            {
                SetCursor(hcur);
                return (LRESULT)TRUE;
            }
            // else FALL THROUGH to DefWindowProc and use parent's cursor
        }

        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0L;
}
