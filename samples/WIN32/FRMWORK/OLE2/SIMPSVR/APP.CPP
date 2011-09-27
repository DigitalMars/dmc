//**********************************************************************
// File name: app.cpp
//
//    Implementation file for the CSimpSvrApp Class
//
// Functions:
//
//    See app.h for a list of member functions.
//
// Copyright (c) 1993 - 1995 Microsoft Corporation. All rights reserved.
//**********************************************************************

#include "precomp.h"
#include "initguid.h"
#include "obj.h"
#include "app.h"            
#include "doc.h"   
#include "icf.h"


//**********************************************************************
//
// CSimpSvrApp::CSimpSvrApp()
// 
// Purpose:
// 
//      Constructor for CSimpSvrApp
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
//
//********************************************************************

CSimpSvrApp::CSimpSvrApp() 
{
    OutputDebugString(TEXT("In CSimpSvrApp's Constructor \r\n"));
		  
    // Set Ref Count              
    m_cRef = 0;   
    
    // clear members
    m_hAppWnd = NULL;
    m_hInst = NULL;
    m_pDoc = NULL;    
    
    // clear flags
    m_fInitialized = FALSE;
    
    // used for inplace
    SetRectEmpty(&nullRect);
}

//**********************************************************************
//
// CSimpSvrApp::~CSimpSvrApp()
// 
// Purpose:
//                          
//      Destructor for CSimpSvrApp Class.
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
//      DestroyWindow               Windows API
//      CSimpSvrApp::IsInitialized  APP.H           
//      OleUninitialize             OLE API
//
// Comments:
//
//********************************************************************

CSimpSvrApp::~CSimpSvrApp()
{
    OutputDebugString(TEXT("In CSimpSvrApp's Destructor\r\n"));

    // need to uninit the library...
    if (IsInitialized())
        OleUninitialize();

    if (m_hAppWnd)
        DestroyWindow(m_hAppWnd);
}


//**********************************************************************
//
// CSimpSvrApp::QueryInterface
// 
// Purpose:
//                                  
//      Used for interface negotiation at the Application level.
//  
// Parameters:
//                           
//      REFIID  riid        -   A reference to the interface that is
//                              being queried.
//
//      LPVOID* ppv         -   An out parameter to return a pointer to
//                              the interface.
// 
// Return Value:                              
//
//      S_OK          -   The interface is supported.
//      E_NOINTERFACE -   The interface is not supported
//
// Function Calls:
//      Function                    Location
//                   
//      OutputDebugString           Windows API
//      IUnknown::AddRef            APP.CPP
//
// Comments:                                      
//
//
//********************************************************************

STDMETHODIMP CSimpSvrApp::QueryInterface(REFIID riid, LPVOID* ppv)
{
    OutputDebugString(TEXT("In CSimpSvrApp::QueryInterface\r\n"));
	 
    *ppv = NULL;
	     
    if (IsEqualIID(riid, IID_IUnknown))
        *ppv = (LPUNKNOWN)this;
	
    if (NULL != *ppv)
    {
        ((LPUNKNOWN)*ppv)->AddRef();
        return S_OK;
    }
    
    return E_NOINTERFACE;
}

//**********************************************************************
//
// CSimpSvrApp::AddRef
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
    
STDMETHODIMP_(ULONG) CSimpSvrApp::AddRef()
{
    OutputDebugString(TEXT("In CSimpSvrApp::AddRef\r\n"));

    return ++m_cRef;
}

//**********************************************************************
//
// CSimpSvrApp::Release
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
//      Due to the reference counting model that is used in this
//      implementation, this reference count is the sum of the
//      reference counts on all interfaces of all objects open
//      in the application.
//
//********************************************************************

STDMETHODIMP_(ULONG) CSimpSvrApp::Release()
{
    OutputDebugString(TEXT("In CSimpSvrApp::Release\r\n"));

    int nTmp = --m_cRef;

    if (0 == nTmp)
        delete this;

    return nTmp;
}                   


//**********************************************************************
//
// CSimpSvrApp::fInitApplication
// 
// Purpose:
//                                       
//      Initializes the application
//  
// Parameters:
//                          
//      HINSTANCE hInstance -   App. Instance Handle.
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
//      InvalidateRect              Windows API
//      ShowWindow                  Windows API
//      UpdateWindow                Windows API
//      CoRegisterClassObject       OLE API
//      OleBuildVersion             OLE API
//      OleInitialize               OLE API                                        
//      RegisterHatchWindowClass    OLE2UI
//      CSimpSvrDoc::CreateObject   DOC.CPP
//
// Comments:                              
//
//      Note that successful Initalization of the OLE libraries
//      is remembered so the UnInit is only called if needed.
//
//********************************************************************

BOOL CSimpSvrApp::fInitApplication(HINSTANCE hInstance, int nCmdShow)
{
    m_hInst = hInstance;  // save the app instance handle

    // Register the main window class

    WNDCLASSEX wc;

    wc.cbSize        = sizeof(WNDCLASSEX);
    wc.style         = 0;
    wc.lpfnWndProc   = MainWndProc;
    wc.cbClsExtra    = 0;
    wc.cbWndExtra    = 0;
    wc.hInstance     = hInstance;
    wc.hIcon         = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_SIMPSVRICON));
    wc.hCursor       = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wc.lpszMenuName  = TEXT("SimpSvrMENU");
    wc.lpszClassName = TEXT("SimpSvrWClass");
    wc.hIconSm       = (HICON)LoadImage(hInstance,      // Load small icon image
                                        MAKEINTRESOURCE(IDI_SIMPSVRICON),
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

    wc.style         = CS_VREDRAW | CS_HREDRAW;
    wc.lpfnWndProc   = DocWndProc;
    wc.hIcon         = NULL;
    wc.lpszMenuName  = NULL;   
    wc.lpszClassName = TEXT("DocWClass");
    wc.hIconSm       = NULL;
    
    if (!RegisterClassEx(&wc))
    {
        if (!RegisterClass((LPWNDCLASS)&wc.style))
        	return FALSE;
    }

    // Register the hatch window class

    if (!RegisterHatchWindowClass(hInstance))
        return FALSE;

    // Initialize the OLE libraries

    m_fInitialized = SUCCEEDED(OleInitialize(NULL));

    // Create the main application window

    m_hAppWnd = CreateWindow(TEXT("SimpSvrWClass"),
                             TEXT("Simple OLE In-Place Server"),
                             WS_OVERLAPPEDWINDOW,
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

    // if not started by OLE, then show the Window, and create a "fake" object, else
    // Register a pointer to IClassFactory so that OLE can instruct us to make an
    // object at the appropriate time.
    if (!m_fStartByOle)
    {
        ShowAppWnd(nCmdShow);
        m_pDoc->CreateObject(IID_IOleObject, (LPVOID*)&m_pOleObject);
        InvalidateRect(m_pDoc->GethDocWnd(), NULL, TRUE);
    }
    else
    {
        LPCLASSFACTORY lpClassFactory = new CClassFactory(this);

        // shouldn't pass an API an object with a zero ref count
        lpClassFactory->AddRef();

        CoRegisterClassObject(GUID_SIMPLESVR32,
                              (LPUNKNOWN)lpClassFactory,
                              CLSCTX_LOCAL_SERVER,
                              REGCLS_SINGLEUSE,
                              &m_dwRegisterClass);

        // remove artificial Ref. count
        lpClassFactory->Release();
    }

    m_hMainMenu  = GetMenu(m_hAppWnd);
    m_hColorMenu = GetSubMenu(m_hMainMenu, 1);
    m_hHelpMenu  = GetSubMenu(m_hMainMenu, 2);

    return m_fInitialized;
}

//**********************************************************************
//
// CSimpSvrApp::lCommandHandler 
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
//      GetClientRect                               Windows API
//      MessageBox                                  Windows API
//      DialogBox                                   Windows API
//      MakeProcInstance                            Windows API
//      FreeProcInstance                            Windows API
//      SendMessage                                 Windows API
//      DefWindowProc                               Windows API
//      InvalidateRect                              Windows API
//      CSimpSvrDoc::InsertObject                   DOC.CPP
//      CSimpSvrObj::SetColor                       OBJ.CPP    
//      CSimpSvrObj::RotateColor                    OBJ.CPP
//
// Comments:
//
//********************************************************************

long CSimpSvrApp::lCommandHandler(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (GET_WM_COMMAND_ID(wParam,lParam))
    {
        // bring up the About box
        case IDM_ABOUT:
            DialogBox(m_hInst, TEXT("AboutBox"), m_hAppWnd, About);
            break;

        // exit the application
        case IDM_EXIT:
            SendMessage(hWnd, WM_SYSCOMMAND, SC_CLOSE, 0L);
            break;

        case IDM_RED:
            m_pDoc->GetObj()->SetColor(192, 0, 0);
            InvalidateRect(m_pDoc->GethDocWnd(), NULL, TRUE);
            break;

        case IDM_GREEN:
            m_pDoc->GetObj()->SetColor(0,192, 0);
            InvalidateRect(m_pDoc->GethDocWnd(), NULL, TRUE);
            break;

        case IDM_BLUE:
            m_pDoc->GetObj()->SetColor(0, 0, 192);
            InvalidateRect(m_pDoc->GethDocWnd(), NULL, TRUE);
            break;

        case IDM_ROTATE:
            m_pDoc->GetObj()->RotateColor();
            InvalidateRect(m_pDoc->GethDocWnd(), NULL, TRUE);
            break;

        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
    }   // end of switch
    return 0L;
}        

//**********************************************************************
//
// CSimpSvrApp::lSizeHandler
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
//      CSimpSvrDoc::lResizeDoc      DOC.CPP
//
// Comments:
//
//********************************************************************

long CSimpSvrApp::lSizeHandler (HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    RECT rect;

    GetClientRect(m_hAppWnd, &rect);
    return m_pDoc->lResizeDoc(&rect);
}

//**********************************************************************
//
// CSimpSvrApp::lCreateDoc
//
// Purpose:
//
//      Handles the WM_CREATE message for the main app window and
//      creates a document.
//
// Parameters:
//
//      HWND hWnd       -   Handle to the application Window
//      UINT message    -   message (always WM_CREATE)
//      WPARAM wParam   -   Same as passed to the WndProc
//      LPARAM lParam   -   Same as passed to the WndProc
//
// Return Value:
//
//      NULL for success; -1 for failure (aborts window creation)
//
// Function Calls:
//      Function                    Location
//
//      GetClientRect               Windows API
//      CSimpSvrDoc::Create         DOC.CPP
//
// Comments:
//
//********************************************************************

long CSimpSvrApp::lCreateDoc (HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    RECT rect;

    GetClientRect(hWnd, &rect);

    m_pDoc = CSimpSvrDoc::Create(this, &rect, hWnd);

    return (m_pDoc != NULL ? 0 : -1);
}


//**********************************************************************
//
// CSimpSvrApp::PaintApp 
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
//      CSimpSvrDoc::PaintDoc        DOC.CPP
//
// Comments:
//
//
//********************************************************************

void CSimpSvrApp::PaintApp (HDC hDC)
{
    // if we supported multiple documents, we would enumerate
    // through each of the open documents and call paint.

    if (m_pDoc)
        m_pDoc->PaintDoc(hDC);
}

//**********************************************************************
//
// CSimpSvrApp::ParseCmdLine
// 
// Purpose:
//
//      Determines if the app was started by OLE
//      
//  
// Parameters:          
//
//      LPSTR lpCmdLine -   Pointer to the command line
// 
// Return Value:                               
//
//      None
//
// Function Calls:
//      Function                    Location
//                       
//      lstrlen                     Windows API
//      lstrcmp                     Windows API
//
//
// Comments:
//
//      Parses the command line looking for the -Embedding or /Embedding
//      flag.
//
//********************************************************************

void CSimpSvrApp::ParseCmdLine(LPSTR lpCmdLine)
{
    m_fStartByOle = TRUE;

    ::ParseCmdLine(lpCmdLine, &m_fStartByOle, NULL, 0);
}

//**********************************************************************
//
// CSimpSvrApp::SetStatusText
// 
// Purpose:
//
//      Blanks out the text in the status bar
//      
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
//      CSimpSvrDoc::SetStatusText  DOC.CPP
//
//
// Comments:
//
//
//********************************************************************

void CSimpSvrApp::SetStatusText()                          
{
    m_pDoc->SetStatusText();
}
	
			  
//**********************************************************************
//
// CSimpSvrApp::IsInPlaceActive
// 
// Purpose:
//
//      Safely determines from the app level if currently inplace active.
//      
//  
// Parameters:          
//
//      None
// 
// Return Value:                               
//
//      TRUE    - Inplace active
//      FALSE   - Not Inplace active
//
// Function Calls:
//      Function                    Location
//                       
//      CSimpSvrDoc::GetObject      OBJ.H
//      CSimpSvrObj:IsInPlaceActive OBJ.H
//
//
// Comments:
//
//
//********************************************************************

BOOL CSimpSvrApp::IsInPlaceActive()
{
    BOOL retval = FALSE;
    
    if (m_pDoc && m_pDoc->GetObj())
        retval = m_pDoc->GetObj()->IsInPlaceActive();
	    
    return retval;
}    

//**********************************************************************
//
// CSimpSvrApp::ShowAppWnd
// 
// Purpose:
//                     
//      Shows the Application Window
//  
// Parameters:                              
//
//      int nCmdShow    - Window State 
// 
// Return Value:                              
//
//      None
//       
// Function Calls:
//      Function                        Location
//          
//      ShowWindow                      Windows API
//      UpdateWindow                    Windows API
//      CoLockObjectExternal            OLE API
//
// Comments:                                     
//
//********************************************************************

void CSimpSvrApp::ShowAppWnd(int nCmdShow)
{
    CoLockObjectExternal(this, TRUE, FALSE);
    ShowWindow(m_hAppWnd, nCmdShow);
    UpdateWindow(m_hAppWnd);
}

//**********************************************************************
//
// CSimpSvrApp::ShowAppWnd
// 
// Purpose:
//                     
//      Hides the Application Window
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
//      Function                        Location
//          
//      ShowWindow                      Windows API
//      CoLockObjectExternal            OLE API
//
// Comments:                                     
//
//********************************************************************

void CSimpSvrApp::HideAppWnd()
{
    CoLockObjectExternal(this, FALSE, TRUE);
    ShowWindow(m_hAppWnd, SW_HIDE);
}
