//**********************************************************************
// File name: Simple.cpp
//
//      Main source file for the Simple OLE 2.0 object container
//
// Functions:
//
//      WinMain         - Program entry point
//      MainWndProc     - Processes messages for the frame window
//      About           - Processes messages for the about dialog
//      DocWndProc      - Processes messages for the doc window
//
// Copyright (c) 1992 - 1993 Microsoft Corporation. All rights reserved.
//**********************************************************************

#include "pre.h"
#include "iocs.h"
#include "ias.h"
#include "ioipf.h"
#include "ioips.h"
#include "app.h"
#include "site.h"
#include "doc.h"

// This line is needed for the debug utilities in OLE2UI
extern "C" {
	OLEDBGDATA_MAIN("SIMPCNTR")
}

CSimpleApp FAR * lpCSimpleApp;

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

int PASCAL WinMain(HANDLE hInstance,HANDLE hPrevInstance,LPSTR lpCmdLine,int nCmdShow)

{
	MSG msg;

	// needed for LRPC to work properly...
	SetMessageQueue(96);

	lpCSimpleApp = new CSimpleApp;

	// we will add one ref count on our App. later when we want to destroy
	// the App object we will release this  ref count. when the App's ref
	// count goes to 0, it will be deleted.
	lpCSimpleApp->AddRef();

	// app initialization
	if (!hPrevInstance)
		if (!lpCSimpleApp->fInitApplication(hInstance))
			return (FALSE);

	// instance initialization
	if (!lpCSimpleApp->fInitInstance(hInstance, nCmdShow))
		return (FALSE);

	/* Initialization required for OLE 2 UI library.  This call is
	**    needed ONLY if we are using the static link version of the UI
	**    library. If we are using the DLL version, we should NOT call
	**    this function in our application.
	*/
	if (!OleUIInitialize(hInstance, hPrevInstance))
		{
		OleDbgOut("Could not initialize OLEUI library\n");
		return FALSE;
		}

	// message loop
	while (GetMessage(&msg, NULL, NULL, NULL))
		if (!lpCSimpleApp->HandleAccelerators(&msg))
			{
			TranslateMessage(&msg);    /* Translates virtual key codes */
			DispatchMessage(&msg);     /* Dispatches message to window */
			}

	// De-initialization for UI libraries.  Just like OleUIInitialize, this
	// funciton is needed ONLY if we are using the static link version of the
	// OLE UI library.
	OleUIUninitialize();

	// Release the ref count added on the App above. this will make
	// the App's ref count go to 0, and the App object will be deleted.
	lpCSimpleApp->Release();

	return (msg.wParam);          /* Returns the value from PostQuitMessage */
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

long FAR PASCAL _export MainWndProc(HWND hWnd,UINT message,WPARAM wParam,LPARAM lParam)

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
			if ( LOWORD(lParam) == 1)
				return lpCSimpleApp->m_lpDoc->lAddVerbs();

			break;

		// this code is needed for 256 color objects to work properly.
		case WM_QUERYNEWPALETTE:
			if (! lpCSimpleApp->m_fAppActive)
				return 0L;

			return lpCSimpleApp->QueryNewPalette();


		case WM_PALETTECHANGED:
		{
			HWND hWndPalChg = (HWND) wParam;

			if (hWnd != hWndPalChg)
				wSelectPalette(hWnd, lpCSimpleApp->m_hStdPal, TRUE/*fBackground*/);

			/* OLE2NOTE: always forward the WM_PALETTECHANGED message (via
			**    SendMessage) to any in-place objects that currently have
			**    their window visible. this gives these objects the chance
			**    to select their palettes. this is
			**    REQUIRED by all in-place containers independent of
			**    whether they use color palettes themselves--their objects
			**    may use color palettes.
			**    (see ContainerDoc_ForwardPaletteChangedMsg for more info)
			*/
			if (lpCSimpleApp->m_lpDoc && lpCSimpleApp->m_lpDoc->m_lpSite &&
				lpCSimpleApp->m_lpDoc->m_lpSite->m_hwndIPObj)
				SendMessage(lpCSimpleApp->m_lpDoc->m_lpSite->m_hwndIPObj,
						WM_PALETTECHANGED, wParam, lParam);

			return 0L;
		}

		case WM_ACTIVATEAPP:
			if ((lpCSimpleApp->m_fAppActive = (BOOL)wParam) == TRUE)
				lpCSimpleApp->QueryNewPalette();

			if (lpCSimpleApp->m_lpDoc->m_lpActiveObject) {
				lpCSimpleApp->m_lpDoc->m_lpActiveObject->OnFrameWindowActivate(
						(BOOL)wParam);
			}
			break;


		case WM_SIZE:
			return lpCSimpleApp->lSizeHandler(hWnd, message, wParam, lParam);

		default:                          // Passes it on if unproccessed
			return (DefWindowProc(hWnd, message, wParam, lParam));
		}
		return (NULL);
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

BOOL FAR PASCAL _export About(HWND hDlg,unsigned message,WORD wParam,LONG lParam)

{
	switch (message) {
	case WM_INITDIALOG:                /* message: initialize dialog box */
		return (TRUE);

	case WM_COMMAND:                      /* message: received a command */
		if (wParam == IDOK                /* "OK" box selected?          */
		|| wParam == IDCANCEL) {      /* System menu close command? */
		EndDialog(hDlg, TRUE);        /* Exits the dialog box        */
		return (TRUE);
		}
		break;
	}
	return (FALSE);                           /* Didn't process a message    */
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

long FAR PASCAL _export DocWndProc(HWND hWnd,UINT message,WPARAM wParam,LPARAM lParam)
{
	HDC hDC;
	PAINTSTRUCT ps;

	switch (message) {
		case WM_PAINT:

			hDC = BeginPaint(hWnd, &ps);

			if (lpCSimpleApp)
				lpCSimpleApp->PaintApp (hDC);

			EndPaint(hWnd, &ps);
			break;


		case WM_LBUTTONDBLCLK:
			{
			POINT pt;

			pt.x = (int)(short)LOWORD (lParam );
			pt.y = (int)(short)HIWORD (lParam );

			if (lpCSimpleApp->m_lpDoc->m_lpSite &&
				lpCSimpleApp->m_lpDoc->m_lpSite->m_lpOleObject)
				{
				RECT rect;

				lpCSimpleApp->m_lpDoc->m_lpSite->GetObjRect(&rect);

				if ( PtInRect(&rect, pt) )
					{
					// Execute object's default verb
					lpCSimpleApp->m_lpDoc->m_lpSite->m_lpOleObject->DoVerb(
							OLEIVERB_PRIMARY, (LPMSG)&message,
							&lpCSimpleApp->m_lpDoc->m_lpSite->m_OleClientSite,
							-1, hWnd, &rect);
					}
				}
			}
			break;

	// no code is added to WM_LBUTTONDOWN for context sensitive help, because
	// this app does not do context sensitive help.
	case WM_LBUTTONDOWN:

		LPOLEINPLACEOBJECT lpObject;

		if (lpCSimpleApp->m_lpDoc->m_fInPlaceActive)
			{
			lpCSimpleApp->m_lpDoc->m_lpSite->m_lpOleObject->QueryInterface(IID_IOleInPlaceObject, (LPVOID FAR *)&lpObject);
			lpObject->UIDeactivate();

			// this code is needed because we don't support inside out.
			RECT rect;
			lpCSimpleApp->m_lpDoc->m_lpSite->GetObjRect(&rect);
			lpCSimpleApp->m_lpDoc->m_lpSite->m_lpOleObject->DoVerb(OLEIVERB_HIDE, (LPMSG)&message, &lpCSimpleApp->m_lpDoc->m_lpSite->m_OleClientSite, -1, hWnd, &rect);

			lpObject->Release();
			}

		break;

		default:                          /* Passes it on if unproccessed */
			return (DefWindowProc(hWnd, message, wParam, lParam));
	}
	return (NULL);
}
