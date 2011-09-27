//**********************************************************************
// File name: app.cpp
//
//    Implementation file for the CSimpleApp Class
//
// Functions:
//
//    See app.h for a list of member functions.
//
// Copyright (c) 1992-1995 Microsoft Corporation. All rights reserved.
//**********************************************************************

#include "precomp.h"
#include "app.h"
#include "site.h"
#include "doc.h"


//**********************************************************************
//
// CSimpleApp::CSimpleApp()
//
// Purpose:
//
//      Constructor for CSimpleApp
//
// Parameters:
//
//      None
//
// Return Value:
//
//      None
//
// Function Calls:
//      Function                    Location
//
//      OutputDebugString           Windows API
//      SetRectEmpty                Windows API
//
// Comments:
//
//      CSimpleApp has a contained COleInPlaceFrame.  On construction
//      of CSimpleApp, we explicitly call the constructor of this
//      contained class and pass a copy of the this pointer, so that
//      COleInPlaceFrame can refer back to this class
//
//********************************************************************

CSimpleApp::CSimpleApp()
{
    OutputDebugString(TEXT("In CSimpleApp's Constructor\r\n"));

    // Set Ref Count
    m_nCount = 0;

    // clear members
    m_hAppWnd = NULL;
    m_hInst = NULL;
    m_lpDoc = NULL;
    m_hwndUIActiveObj = NULL;

    // clear flags
    m_fInitialized = FALSE;
    m_fCSHMode = FALSE;
    m_fMenuMode = FALSE;

    // used for inplace
    SetRectEmpty(&nullRect);
}

//**********************************************************************
//
// CSimpleApp::~CSimpleApp()
//
// Purpose:
//
//      Destructor for CSimpleApp Class.
//
// Parameters:
//
//      None
//
// Return Value:
//
//      None
//
// Function Calls:
//      Function                    Location
//
//      OutputDebugString           Windows API
//      OleUninitialize             OLE API
//
// Comments:
//
//********************************************************************

CSimpleApp::~CSimpleApp()
{
    OutputDebugString(TEXT("In CSimpleApp's Destructor\r\n"));

    if (m_hStdPal)
        DeleteObject(m_hStdPal);

    // need to uninit the library...
    if (m_fInitialized)
        OleUninitialize();
}

//**********************************************************************
//
// CSimpleApp::DestroyDocs()
//
// Purpose:
//
//      Destroys all of the open documents in the application (Only one
//      since this is an SDI app, but could easily be modified to
//      support MDI).
//
// Parameters:
//
//      None
//
// Return Value:
//
//      None
//
// Function Calls:
//      Function                    Location
//
// Comments:
//
//********************************************************************

void CSimpleApp::DestroyDocs()
{
    if (m_lpDoc)
        m_lpDoc->Close();   // we have only 1 document
}

//**********************************************************************
//
// CSimpleApp::QueryInterface
//
// Purpose:
//
//      Used for interface negotiation at the Frame level.
//
// Parameters:
//
//      REFIID riid         -   A reference to the interface that is
//                              being queried.
//
//      LPVOID* ppvObj      -   An out parameter to return a pointer to
//                              the interface.
//
// Return Value:
//
//      S_OK    -   The interface is supported.
//      S_FALSE -   The interface is not supported
//
// Function Calls:
//      Function                    Location
//
//      OutputDebugString           Windows API
//      IsEqualIID                  OLE API
//      COleInPlaceFrame::AddRef    IOIPF.CPP
//
// Comments:
//
//      Note that this QueryInterface is associated with the frame.
//      Since the application could potentially have multiple documents
//      and multiple objects, a lot of the interfaces are ambiguous.
//      (ie. which IOleObject is returned?).  For this reason, only
//      pointers to interfaces associated with the frame are returned.
//      In this implementation, Only IOleInPlaceFrame (or one of the
//      interfaces it is derived from) can be returned.
//
//********************************************************************

STDMETHODIMP CSimpleApp::QueryInterface(REFIID riid, LPVOID* ppv)
{
    OutputDebugString(TEXT("In CSimpleApp::QueryInterface\r\n"));

    *ppv = NULL;        // must set out pointer parameters to NULL

    // looking for IUnknown
    if (IsEqualIID(riid, IID_IUnknown))
        *ppv = (LPUNKNOWN)this;

    // looking for IOleWindow
    else if (IsEqualIID(riid, IID_IOleWindow))
        *ppv = (LPOLEWINDOW)this;

    // looking for IOleInPlaceUIWindow
    else if (IsEqualIID(riid, IID_IOleInPlaceUIWindow))
        *ppv = (LPOLEINPLACEUIWINDOW)this;

    // looking for IOleInPlaceFrame
    else if (IsEqualIID(riid, IID_IOleInPlaceFrame))
        *ppv = (LPOLEINPLACEFRAME)this;

    if (NULL != *ppv)
    {
        ((LPUNKNOWN)*ppv)->AddRef();
        return S_OK;
    }

    // Not a supported interface
    return E_NOINTERFACE;
}

//**********************************************************************
//
// CSimpleApp::AddRef
//
// Purpose:
//
//      Adds to the reference count at the Application level.
//
// Parameters:
//
//      None
//
// Return Value:
//
//      ULONG   -   The new reference count of the application.
//
// Function Calls:
//      Function                    Location
//
//      OutputDebugString           Windows API
//
// Comments:
//
//      Due to the reference counting model that is used in this
//      implementation, this reference count is the sum of the
//      reference counts on all interfaces of all objects open
//      in the application.
//
//********************************************************************

STDMETHODIMP_(ULONG) CSimpleApp::AddRef()
{
    OutputDebugString(TEXT("In CSimpleApp::AddRef\r\n"));

    return ++m_nCount;
}

//**********************************************************************
//
// CSimpleApp::Release
//
// Purpose:
//
//      Decrements the reference count at this level
//
// Parameters:
//
//      None
//
// Return Value:
//
//      ULONG   -   The new reference count of the application.
//
// Function Calls:
//      Function                    Location
//
//      OutputDebugString           Windows API
//
// Comments:
//
//********************************************************************

STDMETHODIMP_(ULONG) CSimpleApp::Release()
{
    OutputDebugString(TEXT("In CSimpleApp::Release\r\n"));

    int nTmp = --m_nCount;

    if (0 == nTmp)
        delete this;

    return nTmp;
}


//**********************************************************************
//
// CSimpleApp::fInitApplication
//
// Purpose:
//
//      Initializes the application
//
// Parameters:
//
//      HANDLE hInstance    -   App. Instance Handle.
//      int nCmdShow        -   Show parameter from WinMain
//
// Return Value:
//
//      TRUE    -   Initialization Successful
//      FALSE   -   Initialization Failed.
//
//
// Function Calls:
//      Function                    Location
//
//      LoadIcon                    Windows API
//      LoadCursor                  Windows API
//      GetStockObject              Windows API
//      RegisterClass               Windows API
//      CreateWindow                Windows API
//      ShowWindow                  Windows API
//      UpdateWindow                Windows API
//      OleBuildVersion             OLE API
//      OleInitialize               OLE API
//
// Comments:
//
//      Note that successful Initalization of the OLE libraries
//      is remembered so the UnInit is only called if needed.
//
//********************************************************************

BOOL CSimpleApp::fInitApplication(HINSTANCE hInstance, int nCmdShow)
{
    m_hInst = hInstance;

    // Register the main window class

    WNDCLASSEX wc;

    wc.cbSize        = sizeof(WNDCLASSEX);
    wc.style         = 0;
    wc.lpfnWndProc   = MainWndProc;
    wc.cbClsExtra    = 0;
    wc.cbWndExtra    = 0;
    wc.hInstance     = hInstance;
    wc.hIcon         = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_SIMPCNTRICON));
    wc.hCursor       = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wc.lpszMenuName  = TEXT("SIMPLEMENU");
    wc.lpszClassName = TEXT("SimpCntrAppWClass");
    wc.hIconSm       = (HICON)LoadImage(hInstance,      // Load small icon image
                                        MAKEINTRESOURCE(IDI_SIMPCNTRICON),
                                        IMAGE_ICON,
                                        16, 16,
                                        0);

    // Register the window class and return FALSE if unsuccesful.

    if (!RegisterClassEx(&wc))
    {
        //Assume we are running on NT where RegisterClassEx() is
        //not implemented, so let's try calling RegisterClass().

        if (!RegisterClass((LPWNDCLASS)&wc.style))
        	return FALSE;
    }

    // Register the doc window class

    wc.style         = CS_DBLCLKS;
    wc.lpfnWndProc   = DocWndProc;
    wc.hIcon         = NULL;
    wc.lpszMenuName  = NULL;
    wc.lpszClassName = TEXT("SimpCntrDocWClass");
    wc.hIconSm       = NULL;

    if (!RegisterClassEx(&wc))
    {
        if (!RegisterClass((LPWNDCLASS)&wc.style))
        	return FALSE;
    }

    // Initialize OLE libraries

    m_fInitialized = SUCCEEDED(OleInitialize(NULL));

    // Create the "application" windows
    m_hAppWnd = CreateWindow(TEXT("SimpCntrAppWClass"),
                             TEXT("Simple OLE In-Place Container"),
                             WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN,
                             CW_USEDEFAULT,
                             CW_USEDEFAULT,
                             CW_USEDEFAULT,
                             CW_USEDEFAULT,
                             NULL,
                             NULL,
                             hInstance,
                             NULL);

    if (!m_hAppWnd)
        return FALSE;

    HDC hdc = GetDC(NULL);
    m_hStdPal = CreateHalftonePalette(hdc);
    ReleaseDC(NULL, hdc);

    ShowWindow(m_hAppWnd, nCmdShow);
    UpdateWindow(m_hAppWnd);

    return m_fInitialized;
}

//**********************************************************************
//
// CSimpleApp::lCommandHandler
//
// Purpose:
//
//      Handles the processing of WM_COMMAND.
//
// Parameters:
//
//      HWND hWnd       -   Handle to the application Window
//
//      UINT message    -   message (always WM_COMMAND)
//
//      WPARAM wParam   -   Same as passed to the WndProc
//
//      LPARAM lParam   -   Same as passed to the WndProc
//
// Return Value:
//
//      NULL
//
// Function Calls:
//      Function                                    Location
//
//      IOleInPlaceActiveObject::QueryInterface     Object
//      IOleInPlaceObject::ContextSensitiveHelp     Object
//      IOleInPlaceObject::Release                  Object
//      IOleObject::DoVerb                          Object
//      GetClientRect                               Windows API
//      MessageBox                                  Windows API
//      DialogBox                                   Windows API
//      SendMessage                                 Windows API
//      DefWindowProc                               Windows API
//      CSimpleDoc::InsertObject                    DOC.CPP
//
// Comments:
//
//********************************************************************

long CSimpleApp::lCommandHandler(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    WORD wCommandID = GET_WM_COMMAND_ID(wParam, lParam);

    // context sensitive help...
    if (m_fMenuMode || m_fCSHMode)
    {
        if (m_fCSHMode)
        {
            // clear context sensitive help flag
            m_fCSHMode = FALSE;

            // if there is an InPlace active object, call its context sensitive help
            // method with the FALSE parameter to bring the object out of the
            // csh state.  See the technotes for details.
            if (m_lpDoc->m_lpActiveObject)
            {
                LPOLEINPLACEOBJECT lpInPlaceObject;
                m_lpDoc->m_lpActiveObject->QueryInterface(IID_IOleInPlaceObject,
                                                          (LPVOID*)&lpInPlaceObject);
                lpInPlaceObject->ContextSensitiveHelp(FALSE);
                lpInPlaceObject->Release();
            }
        }

        // see the technotes for details on implementing context sensitive
        // help
        if (m_fMenuMode)
        {
            m_fMenuMode = FALSE;

            if (m_lpDoc->m_lpActiveObject)
                m_lpDoc->m_lpActiveObject->ContextSensitiveHelp(FALSE);
        }
        // if we provided help, we would do it here...
        MessageBox(hWnd, TEXT("Help"), TEXT("Help"), MB_OK);

        return 0L;
    }

    // see if the command is a verb selections
    if (wCommandID >= IDM_VERB0)
    {
        RECT rect;

        // get the rectangle of the object
        m_lpDoc->m_lpSite->GetObjRect(&rect);

        // do the verb
        m_lpDoc->m_lpSite->m_lpOleObject->DoVerb(wCommandID - IDM_VERB0,
                                                 NULL,
                                                 m_lpDoc->m_lpSite->GetOleClientSite(),
                                                 0,
                                                 m_lpDoc->m_hDocWnd,
                                                 &rect);
    }
    else
    {
        switch (wCommandID)
        {
            // bring up the About box
            case IDM_ABOUT:
                DialogBox(m_hInst,               // current instance
                    TEXT("AboutBox"),            // resource to use
                    m_hAppWnd,                   // parent handle
                    About);                      // About() dialog proc
                break;

            // bring up the InsertObject Dialog
            case IDM_INSERTOBJECT:
                m_lpDoc->InsertObject();
                break;

            // exit the application
            case IDM_EXIT:
                SendMessage(hWnd, WM_SYSCOMMAND, SC_CLOSE, 0L);
                break;

            case IDM_NEW:
                m_lpDoc->Close();
                m_lpDoc = NULL;
                lCreateDoc(hWnd, 0, 0, 0);
                break;

            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
        }   // end of switch
    }  // end of else
    return 0L;
}

//**********************************************************************
//
// CSimpleApp::lSizeHandler
//
// Purpose:
//
//      Handles the WM_SIZE message
//
// Parameters:
//
//      HWND hWnd       -   Handle to the application Window
//
//      UINT message    -   message (always WM_SIZE)
//
//      WPARAM wParam   -   Same as passed to the WndProc
//
//      LPARAM lParam   -   Same as passed to the WndProc
//
// Return Value:
//
//      LONG    -   returned from the "document" resizing
//
// Function Calls:
//      Function                    Location
//
//      GetClientRect               Windows API
//      CSimpleDoc::lResizeDoc      DOC.CPP
//
// Comments:
//
//********************************************************************

long CSimpleApp::lSizeHandler(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    RECT rect;

    GetClientRect(m_hAppWnd, &rect);

    return m_lpDoc->lResizeDoc(&rect);
}

//**********************************************************************
//
// CSimpleApp::lCreateDoc
//
// Purpose:
//
//      Handles the creation of a document.
//
// Parameters:
//
//      HWND hWnd       -   Handle to the application Window
//
//      UINT message    -   message (always WM_CREATE)
//
//      WPARAM wParam   -   Same as passed to the WndProc
//
//      LPARAM lParam   -   Same as passed to the WndProc
//
// Return Value:
//
//      0 for success, -1 for failure
//
// Function Calls:
//      Function                    Location
//
//      GetClientRect               Windows API
//      CSimpleDoc::CSimpleDoc      DOC.CPP
//
// Comments:
//
//********************************************************************

long CSimpleApp::lCreateDoc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    RECT rect;

    GetClientRect(hWnd, &rect);

    m_lpDoc = CSimpleDoc::Create(this, &rect, hWnd);

    if (NULL == m_lpDoc)
    {
        MessageBox(NULL,
                   TEXT("Unable to create document\r\n"),
                   TEXT("Simple OLE Container"),
                   MB_OK | MB_ICONEXCLAMATION);
        OutputDebugString(TEXT("Unable to create document\r\n"));
        return -1L;     // fail the window creation
    }

    return 0L;
}

//**********************************************************************
//
// CSimpleApp::AddFrameLevelUI
//
// Purpose:
//
//      Used during InPlace negotiation.
//
// Parameters:
//
//      None
//
// Return Value:
//
//      None
//
// Function Calls:
//      Function                            Location
//
//      COleInPlaceFrame::SetMenu           IOIPF.CPP
//      CSimpleApp::AddFrameLevelTools      APP.CPP
//
// Comments:
//
//      Be sure to read the Technotes included in the OLE 2.0 toolkit
//
//********************************************************************

void CSimpleApp::AddFrameLevelUI()
{
    SetMenu(NULL, NULL, NULL);
    AddFrameLevelTools();
}

//**********************************************************************
//
// CSimpleApp::AddFrameLevelTools
//
// Purpose:
//
//      Used during InPlace negotiation.
//
// Parameters:
//
//      None
//
// Return Value:
//
//      None
//
// Function Calls:
//      Function                              Location
//
//      COleInPlaceFrame::SetBorderSpace      IOIPF.CPP
//      InvalidateRect                        Windows API
//
// Comments:
//
//      Be sure to read the Technotes included in the OLE 2.0 toolkit
//
//********************************************************************

void CSimpleApp::AddFrameLevelTools()
{
    SetBorderSpace(&nullRect);
    InvalidateRect(m_hAppWnd, NULL, TRUE);
}

//**********************************************************************
//
// CSimpleApp::HandleAccelerators
//
// Purpose:
//
//      To properly handle accelerators in the Message Loop
//
// Parameters:
//
//      LPMSG lpMsg -   A pointer to the message structure.
//
// Return Value:
//
//      TRUE    -   The accelerator was handled
//      FALSE   -   The accelerator was not handled
//
// Function Calls:
//      Function                                        Location
//
//      IOleInPlaceActiveObject::TranslateAccelerator   Object
//
// Comments:
//
//      If an object is InPlace active, it gets the first shot at
//      handling the accelerators.
//
//********************************************************************

BOOL CSimpleApp::HandleAccelerators(LPMSG lpMsg)
{
    BOOL retval = FALSE;

    // if we have an InPlace Active Object
    if (m_lpDoc->m_lpActiveObject)
    {
        // Pass the accelerator on...
        HRESULT hResult = m_lpDoc->m_lpActiveObject->TranslateAccelerator(lpMsg);
        if (hResult == NOERROR)
            retval = TRUE;
    }

    return retval;
}

//**********************************************************************
//
// CSimpleApp::PaintApp
//
// Purpose:
//
//      Handles the painting of the doc window.
//
//
// Parameters:
//
//      HDC hDC -   hDC to the Doc Window.
//
// Return Value:
//
//      None
//
// Function Calls:
//      Function                    Location
//
//      CSimpleDoc::PaintDoc        DOC.CPP
//
// Comments:
//
//      This is an app level function in case we want to do palette
//      management.
//
//********************************************************************

void CSimpleApp::PaintApp (HDC hDC)
{
    // at this level, we could enumerate through all of the
    // visible objects in the application, so that a palette
    // that best fits all of the objects can be built.

    // This app is designed to take on the same palette
    // functionality that was provided in OLE 1.0, the palette
    // of the last object drawn is realized.  Since we only
    // support one object at a time, it shouldn't be a big
    // deal.

    // if we supported multiple documents, we would enumerate
    // through each of the open documents and call paint.

    if (m_lpDoc)
        m_lpDoc->PaintDoc(hDC);
}


/* OLE2NOTE: forward the WM_QUERYNEWPALETTE message (via
**    SendMessage) to UIActive in-place object if there is one.
**    this gives the UIActive object the opportunity to select
**    and realize its color palette as the FOREGROUND palette.
**    This is optional for in-place containers. If a container
**    prefers to force its color palette as the foreground
**    palette then it should NOT forward the this message. Or
**    the container can give the UIActive object priority; if
**    the UIActive object returns 0 from the WM_QUERYNEWPALETTE
**    message (ie. it did not realize its own palette), then
**    the container can realize its palette.
**    (see ContainerDoc_ForwardPaletteChangedMsg for more info)
**
**    (It is a good idea for containers to use the standard
**    palette even if they do not use colors themselves. this
**    will allow embedded object to get a good distribution of
**    colors when they are being drawn by the container)
**
*/

LRESULT CSimpleApp::QueryNewPalette()
{
    if (m_hwndUIActiveObj)
    {
        if (SendMessage(m_hwndUIActiveObj, WM_QUERYNEWPALETTE, 0, 0L))
        {
            /* Object selected its palette as foreground palette */
            return 1L;
        }
    }

    return wSelectPalette(m_hAppWnd, m_hStdPal, FALSE/*fBackground*/);
}


/* This is just a helper routine */

LRESULT wSelectPalette(HWND hWnd, HPALETTE hPal, BOOL fBackground)
{
    HDC hdc;
    HPALETTE hOldPal;
    UINT iPalChg = 0;

    if (hPal == NULL)
        return 0L;

    hdc = GetDC(hWnd);
    hOldPal = SelectPalette(hdc, hPal, fBackground);
    iPalChg = RealizePalette(hdc);
    SelectPalette(hdc, hOldPal, TRUE /*fBackground*/);
    ReleaseDC(hWnd, hdc);

    if (iPalChg > 0 && hWnd != NULL)
        InvalidateRect(hWnd, NULL, TRUE);

    return 1L;
}
