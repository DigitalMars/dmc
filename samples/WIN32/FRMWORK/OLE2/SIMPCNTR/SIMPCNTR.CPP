//**********************************************************************
// File name: Simpcntr.cpp
//
//      Main source file for the Simple OLE object container
//
// Functions:
//
//      WinMain         - Program entry point 
//      MainWndProc     - Processes messages for the frame window
//      About           - Processes messages for the about dialog
//      DocWndProc      - Processes messages for the doc window
//
// Copyright (c) 1992-1995 Microsoft Corporation. All rights reserved.
//**********************************************************************

#include "precomp.h"
#include "app.h"            
#include "site.h"
#include "doc.h"   


CSimpleApp *lpCSimpleApp;

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
//      HANDLE hInstance        - Instance handle for this instance
//
//      HANDLE hPrevInstance    - Instance handle for the last instance
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
//      CSimpleApp::CSimpleApp          APP.CPP
//      CSimpleApp::fInitApplication    APP.CPP
//      CSimpleApp::fInitInstance       APP.CPP
//      CSimpleApp::HandleAccelerators  APP.CPP
//      CSimpleApp::~CSimpleApp         APP.CPP
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
                    "Simple OLE Container",
                    MB_OK | MB_ICONSTOP);
        return 0;
    }
#endif

    lpCSimpleApp = new CSimpleApp;

    // We will add one ref count on our App. later when we want to destroy
    // the App object we will release this  ref count. when the App's ref
    // count goes to 0, it will be deleted.
    lpCSimpleApp->AddRef();


    // App/instance initialization
    if (!lpCSimpleApp->fInitApplication(hInstance, nCmdShow))
    {
        lpCSimpleApp->Release();
        return FALSE;
    }


    // Message loop
    while (GetMessage(&msg, NULL, NULL, NULL))
        if (!lpCSimpleApp->HandleAccelerators(&msg))
        {
            TranslateMessage(&msg);    /* Translates virtual key codes */
            DispatchMessage(&msg);     /* Dispatches message to window */
        }                     


    // Release the ref count added on the App above. this will make
    // the App's ref count go to 0, and the App object will be deleted.
    lpCSimpleApp->Release();
    
    return msg.wParam;          /* Returns the value from PostQuitMessage */
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
//      CSimpleApp::lCommandHandler     APP.CPP
//      CSimpleApp::DestroyDocs         APP.CPP
//      CSimpleApp::lCreateDoc          APP.CPP
//      CSimpleApp::lSizeHandler        APP.CPP
//      CSimpleDoc::lAddVerbs           DOC.CPP
//      PostQuitMessage                 Windows API                     
//      DefWindowProc                   Windows API
//
// Comments:
//
//********************************************************************

LRESULT CALLBACK MainWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
        case WM_COMMAND:           // message: command from application menu
            return lpCSimpleApp->lCommandHandler(hWnd, message, wParam, lParam);
            break;
        
        case WM_CREATE:
            return lpCSimpleApp->lCreateDoc(hWnd, message, wParam, lParam);
            break;

        case WM_DESTROY:                  // message: window being destroyed
            lpCSimpleApp->DestroyDocs();  // need to destroy the doc...
            PostQuitMessage(0);
            break;

        case WM_INITMENUPOPUP:
            // is this the edit menu?
            if (LOWORD(lParam) == 1)
                return lpCSimpleApp->m_lpDoc->lAddVerbs();
            break;
                  
        // this code is needed for 256 color objects to work properly.
        case WM_QUERYNEWPALETTE:
            if (lpCSimpleApp->m_fAppActive)
                return lpCSimpleApp->QueryNewPalette();
            return 0L;

        case WM_PALETTECHANGED:
            if (hWnd != (HWND)wParam)
                wSelectPalette(hWnd, lpCSimpleApp->m_hStdPal, TRUE/*fBackground*/);

            // OLE2NOTE: always forward the WM_PALETTECHANGED message (via
            //    SendMessage) to any in-place objects that currently have
            //    their window visible. this gives these objects the chance
            //    to select their palettes. This is REQUIRED by all in-place
            //    containers independent of whether they use color palettes
            //    themselves (their objects may use color palettes).
            //    (see ContainerDoc_ForwardPaletteChangedMsg for more info)

            if (lpCSimpleApp->m_lpDoc && lpCSimpleApp->m_lpDoc->m_lpSite &&
                lpCSimpleApp->m_lpDoc->m_lpSite->m_hwndIPObj)
                SendMessage(lpCSimpleApp->m_lpDoc->m_lpSite->m_hwndIPObj,
                            WM_PALETTECHANGED,
                            wParam,
                            lParam);
            return 0L;

        case WM_ACTIVATEAPP:
            lpCSimpleApp->m_fAppActive = (BOOL)wParam;
            if (lpCSimpleApp->m_fAppActive)
                lpCSimpleApp->QueryNewPalette();

            if (lpCSimpleApp->m_lpDoc->m_lpActiveObject)
                lpCSimpleApp->m_lpDoc->m_lpActiveObject->OnFrameWindowActivate((BOOL)wParam);
            break;

        case WM_SIZE:
            return lpCSimpleApp->lSizeHandler(hWnd, message, wParam, lParam);
        
        default:                          // Passes it on if unproccessed    
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
        case WM_INITDIALOG:               /* message: initialize dialog box */
            return TRUE;

        case WM_COMMAND:                  /* message: received a command */
            if (GET_WM_COMMAND_ID(wParam,lParam) == IDOK ||     /* "OK" box selected?         */
                GET_WM_COMMAND_ID(wParam,lParam) == IDCANCEL)   /* System menu close command? */
            {
                EndDialog(hDlg, TRUE);    /* Exits the dialog box        */
                return TRUE;
            }
            break;
    }
    return FALSE;                         /* Didn't process a message    */
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
//      CSimpleApp::PaintApp                APP.CPP
//      BeginPaint                          Windows API
//      EndPaint                            Windows API
//      DefWindowProc                       Windows API
//      IOleObject::QueryInterface          Object
//      IOleInPlaceObject::UIDeactivate     Object
//      IOleObject::DoVerb                  Object
//      IOleInPlaceObject::Release          Object
//
// Comments:
//
//********************************************************************

LRESULT CALLBACK DocWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    RECT rect;
    POINT pt;

    switch (message)
    {
        case WM_PAINT:
            PAINTSTRUCT ps;

            BeginPaint(hWnd, &ps);                                

            if (lpCSimpleApp)
                lpCSimpleApp->PaintApp(ps.hdc);

            EndPaint(hWnd, &ps);
            break;

        case WM_LBUTTONDBLCLK:
            if (!(lpCSimpleApp->m_lpDoc->m_lpSite && 
                lpCSimpleApp->m_lpDoc->m_lpSite->m_lpOleObject))
            break;

            pt.x = (int)(short)LOWORD(lParam);
            pt.y = (int)(short)HIWORD(lParam);

            lpCSimpleApp->m_lpDoc->m_lpSite->GetObjRect(&rect);

            if (PtInRect(&rect, pt))
            {
                // Execute object's default verb
                lpCSimpleApp->m_lpDoc->m_lpSite->m_lpOleObject->DoVerb(
                        OLEIVERB_PRIMARY,
                        (LPMSG)&message,
                        lpCSimpleApp->m_lpDoc->m_lpSite->GetOleClientSite(),
                        -1,
                        hWnd,
                        &rect);
            }
            break;

        // no code is added to WM_LBUTTONDOWN for context sensitive help, because
        // this app does not do context sensitive help.
        case WM_LBUTTONDOWN:
            if (!(lpCSimpleApp->m_lpDoc->m_lpSite && 
                lpCSimpleApp->m_lpDoc->m_lpSite->m_lpOleObject))
                break;

            pt.x = (int)(short)LOWORD(lParam);
            pt.y = (int)(short)HIWORD(lParam);
            lpCSimpleApp->m_lpDoc->m_lpSite->GetObjRect(&rect);

            if (!PtInRect(&rect, pt))                           // mouse clicked outside
            {
                if (lpCSimpleApp->m_lpDoc->m_fInPlaceActive)    // active ==> deactivate
                {
                    LPOLEINPLACEOBJECT lpObject;

                    lpCSimpleApp->m_lpDoc->m_lpSite->m_lpOleObject->QueryInterface(IID_IOleInPlaceObject, (LPVOID*)&lpObject);
            
                    // this code is needed because we don't support inside out.
                    MSG msg;
                    msg.hwnd = hWnd;
                    msg.message = message;
                    msg.wParam = wParam;
                    msg.lParam = lParam;
                    msg.time = GetMessageTime();
                    DWORD dwPos = GetMessagePos();
                    msg.pt.x = (int)LOWORD(dwPos);
                    msg.pt.y = (int)HIWORD(dwPos);

                    lpObject->UIDeactivate();
                    lpCSimpleApp->m_lpDoc->m_lpSite->m_lpOleObject->DoVerb(
                            OLEIVERB_HIDE,
                            &msg,
                            lpCSimpleApp->m_lpDoc->m_lpSite->GetOleClientSite(),
                            0,
                            hWnd,
                            &rect);
                    lpObject->Release();
                }
            }
            else    // mouse clicked inside
            {
                if (!lpCSimpleApp->m_lpDoc->m_fInPlaceActive)    // inactive ==> select
                {
                    ;   // unimplemented
                }
            }
            break;

        default:                          /* Passes it on if unproccessed */
            return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0L;
}
