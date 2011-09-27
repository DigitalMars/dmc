//**********************************************************************
// File name: DOC.CPP
//
//      Implementation file for CSimpSvrDoc.
//
// Functions:
//
//      See DOC.H for Class Definition
//
// Copyright (c) 1993 - 1995 Microsoft Corporation. All rights reserved.
//**********************************************************************

#include "precomp.h"
#include "obj.h"
#include "app.h"
#include "doc.h"

//**********************************************************************
//
// CSimpSvrDoc::Create
// 
// Purpose:
//                       
//      Creation for the CSimpSvrDoc Class
//  
// Parameters:
//                                                                     
//      CSimpSvrApp* lpApp      -   Pointer to the CSimpSvrApp Class
//
//      LPRECT lpRect           -   Client area rect of "frame" window
//
//      HWND hWnd               -   Window Handle of "frame" window
// 
// Return Value:
//
//      None
//       
// Function Calls:
//      Function                    Location
//                  
//      StgCreateDocfile            OLE API
//      CreateWindow                Windows API
//      ShowWindow                  Windows API
//      UpdateWindow                Windows API
//      CSimpSvrDoc::CSimpSvrDoc    DOC.CPP
//      CreateHatchWindow           OLE2UI
//
// Comments:
//               
//      This routine was added so that failure could be returned
//      from object creation.
//
//********************************************************************

CSimpSvrDoc* CSimpSvrDoc::Create(CSimpSvrApp* lpApp, LPRECT lpRect, HWND hWnd)
{
    CSimpSvrDoc* lpTemp = new CSimpSvrDoc(lpApp, hWnd);

    if (!lpTemp)
        return NULL;

    // create the document Window
    lpTemp->m_hDocWnd = CreateWindow(TEXT("DocWClass"),
                                     NULL,
                                     WS_CHILD | WS_CLIPSIBLINGS,
                                     lpRect->left,
                                     lpRect->top,
                                     lpRect->right,
                                     lpRect->bottom,
                                     hWnd,
                                     NULL,
                                     lpApp->GethInst(),
                                     NULL);

    if (!lpTemp->m_hDocWnd)
        goto error;

    lpTemp->ShowDocWnd();
    lpTemp->m_hHatchWnd = CreateHatchWindow(hWnd, lpApp->GethInst());
    lpTemp->HideHatchWnd();

    return lpTemp;

error:
    delete lpTemp;
    return NULL;
}                             
    
//**********************************************************************
//
// CSimpSvrDoc::CSimpSvrDoc
// 
// Purpose:
//                       
//      Constructor for the CSimpSvrDoc Class
//  
// Parameters:
//                                                                     
//      CSimpSvrApp* lpApp      -   Pointer to the CSimpSvrApp Class
//
//      HWND hWnd               -   Window Handle of "frame" window
// 
// Return Value:
//
//      None
//       
// Function Calls:
//      Function                    Location
//                  
//      OutputDebugString           Windows API
//      GetMenu                     Windows API
//      GetSubMenu                  Windows API
//
// Comments:
//
//********************************************************************

CSimpSvrDoc::CSimpSvrDoc(CSimpSvrApp* pApp, HWND hWnd)
{
    OutputDebugString(TEXT("In CSimpSvrDoc's Constructor\r\n"));

    m_cRef = 0;
    m_pApp = pApp;
    m_pObj = NULL;
    m_fClosing = FALSE;
}

//**********************************************************************
//
// CSimpSvrDoc::~CSimpSvrDoc
// 
// Purpose:
//                        
//      Destructor for CSimpSvrDoc
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
//      CSimpSvrApp::ClearDoc       APP.CPP
//
// Comments:
//
//********************************************************************

CSimpSvrDoc::~CSimpSvrDoc()
{      
    OutputDebugString(TEXT("In CSimpSvrDoc's Destructor\r\n"));

    m_pApp->ClearDoc();
    DestroyWindow(m_hHatchWnd);
}                     


//**********************************************************************
//
// CSimpSvrDoc::QueryInterface
// 
// Purpose:
//                                 
//      Return a pointer to a requested interface
//  
// Parameters:
//                           
//      REFIID riid         -   ID of interface to be returned
//      LPVOID* ppv         -   Location to return the interface
// 
// Return Value:
//                                                              
//      E_NOINTERFACE -   Always
//
// Function Calls:
//      Function                    Location
//
//      OutputDebugString           Windows API
//      CSimpSvrApp::QueryInterface APP.CPP
//
// Comments:
//
//      Since the document could contain multiple objects, all 
//      interfaces except those associated with the document should
//      be returned.  In this implementation, there are no doc level
//      interfaces.
//
//********************************************************************

STDMETHODIMP CSimpSvrDoc::QueryInterface(REFIID riid, LPVOID* ppv)
{
    OutputDebugString(TEXT("In CSimpSvrDoc::QueryInterface\r\n"));
    
    *ppv = NULL;

    if (IsEqualIID(riid, IID_IUnknown))
        *ppv = this;

    if (*ppv)
    {
        ((LPUNKNOWN)*ppv)->AddRef();
        return S_OK;
    }
       
    return E_NOINTERFACE;
}
    
//**********************************************************************
//
// CSimpSvrDoc::AddRef
// 
// Purpose:
//                   
//      Increments the document level reference count
//  
// Parameters:                                       
//
//      None 
// 
// Return Value:
//          
//      UINT    -   The current reference count on the document
//
// Function Calls:
//      Function                    Location
//
//      OutputDebugString           Windows API 
//      CSimpSvrApp::AddRef         APP.CPP
//
// Comments:
//
//      The reference count at this level reflects the total ref.
//      count of all interfaces on all objects contained within
//      this document.  Note that it also "trickles up" the 
//      ref count to the app level.
//
//********************************************************************

STDMETHODIMP_(ULONG) CSimpSvrDoc::AddRef()
{
    OutputDebugString(TEXT("In CSimpSvrDoc::AddRef\r\n"));

    // AddRef the app, but return the doc count
    m_pApp->AddRef();              
    
    return ++m_cRef;
}

//**********************************************************************
//
// CSimpSvrDoc::Release
// 
// Purpose:
//                   
//      Decrements the document level reference count
//  
// Parameters:                                       
//
//      None 
// 
// Return Value:
//          
//      UINT    -   The current reference count on the document
//
// Function Calls:
//      Function                    Location
//
//      OutputDebugString           Windows API 
//      CSimpSvrApp::Release         APP.CPP
//
// Comments:
//
//      The reference count at this level reflects the total ref.
//      count of all interfaces on all objects contained within
//      this document.  Note that it also "trickles up" the 
//      ref count to the app level.
//
//********************************************************************

STDMETHODIMP_(ULONG) CSimpSvrDoc::Release()
{
    OutputDebugString(TEXT("In CSimpSvrDoc::Release\r\n"));

    // Release the app, but return the doc count
    m_pApp->Release();

    int nTmp = --m_cRef;

    if (0 == nTmp)
        delete this;

    return nTmp;
}

//**********************************************************************
//
// CSimpSvrDoc::lResizeDoc
// 
// Purpose:
//                       
//      Resizes the document
//  
// Parameters:
//                          
//      LPRECT lpRect   -   The size of the client are of the "frame"
//                          Window.
// 
// Return Value:
//                                 
//      NULL
//
// Function Calls:
//      Function                                Location
//                                    
//      MoveWindow                              Windows API
//
// Comments:
//
//********************************************************************

long CSimpSvrDoc::lResizeDoc(LPRECT lpRect)
{ 
    MoveWindow(m_hDocWnd, lpRect->left, lpRect->top, lpRect->right, lpRect->bottom, TRUE);
    return 0L;
}                    


//**********************************************************************
//
// CSimpSvrDoc::PaintDoc
// 
// Purpose:
//                     
//      Paints the Document
//  
// Parameters:
//                         
//      HDC hDC -   hDC of the document Window
// 
// Return Value:                              
//
//      None
//       
// Function Calls:
//      Function                    Location
//          
//      CSimpSvrObj::Draw                   OBJ.CPP
//      CSimpSvrObj::GetDataAdviseHolder    OBJ.H
//      CSimpSvrObj::GetDataObject          OBJ.H
//      CSimpAppObj::IsStartedByOle         APP.CPP
//      IDataAdviseHolder::SendOnDataChange OLE API
//
// Comments:
//
//********************************************************************

void CSimpSvrDoc::PaintDoc(HDC hDC)
{
    // if the object hasn't been created yet, then don't draw
    if (m_pObj)
        m_pObj->Draw(hDC, FALSE);
    else
        return;

    // Sending a data change every time we paint, but only if we
    // were started by OLE
    if (m_pApp->IsStartedByOle())
            m_pObj->SendOnDataChange();
}



//**********************************************************************
//
// CSimpSvrDoc::CreateObject
// 
// Purpose:
//                     
//  
// Parameters:
//                         
// 
// Return Value:                              
//
//      NOERROR if the function succeeds, otherwise E_FAIL
//       
// Function Calls:
//      Function                    Location
//          
//      CSimpSvrObj::CSimpSvrObj    OBJ.CPP
//      CSimpSvrOjb::QueryInterface OBJ.CPP
//
// Comments:
//
//********************************************************************

HRESULT CSimpSvrDoc::CreateObject(REFIID riid, LPVOID* ppv)
{        
    m_pObj = new CSimpSvrObj(this);
    
    if (m_pObj)
        return m_pObj->QueryInterface(riid, ppv);
    
    return E_FAIL;
}    

//**********************************************************************
//
// CSimpSvrDoc::Close
// 
// Purpose:
//                   
//      Closes the object  
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
//      Function                                Location
//          
//      OutputDebugString                       Windows API
//      CSimpSvrObj::AddRef                     OBJ.CPP
//      CSimpSvrObj::Release                    OBJ.CPP
//      CSimpSvrObj::IsInPlaceActive            OBJ.H
//      CSimpSvrObj::GetOleInPlaceObject        OBJ.H
//      CSimpSvrObj::ClearOleClientSite         OBJ.H
//      CSimpSvrObj::GetDataAdviseHolder        OBJ.H
//      CSimpSvrObj::GetOleClientSite           OBJ.H
//      CSimpSvrObj::ClearDataAdviseHolder      OBJ.H
//      CSimpSvrObj::GetOleAdviseHolder         OBJ.H
//      CSimpSvrObj::ClearOleAdviseHolder       OBJ.H
//      IOleInPlaceObject::InPlaceDeactivate    Container
//      IOleClientSite::SaveObject              Container
//      IOleClientSite::OnShowWindow            Container
//      IOleClientSite::Release                 Container
//      IDataAdviseHolder::SendOnDataChange     OLE
//      IDataAdviseHolder::Release              OLE
//      IOleAdviseHolder::SendOnClose           OLE
//      IOleAdviseHolder::Release               OLE
//
// Comments:
//
//********************************************************************

void CSimpSvrDoc::Close()
{   
    OutputDebugString(TEXT("In CSimpSvrDoc::Close() \r\n"));
    
    m_pObj->AddRef(); // hold object alive
    
    if (m_fClosing)
        return;
        
    m_fClosing = TRUE;
    
    // if the object is currently inplace active, then deactivate                                                       
    if (m_pObj->IsInPlaceActive())
        m_pObj->GetOleInPlaceObject()->InPlaceDeactivate();
        
    // unregister from the ROT...
    if (m_pObj->GetRotRegister())
    {
        LPRUNNINGOBJECTTABLE lpRot;
        
        if (GetRunningObjectTable (0, &lpRot) == NOERROR )
        {
            lpRot->Revoke(m_pObj->GetRotRegister());
            lpRot->Release();
        }
    }
        
    // if we have a clientsite, instruct it to save the object                                                                 
    if (m_pObj->GetOleClientSite())
    {     
        m_pObj->GetOleClientSite()->SaveObject();
        m_pObj->GetOleClientSite()->OnShowWindow(FALSE); 
    }
                                                              
    // Do a final SendOnDataChange for those containers that have specified the 
    // ADF_DATAONSTOP flag.                                                              
    if (m_pObj->GetDataAdviseHolder())
    {     
        m_pObj->GetDataAdviseHolder()->SendOnDataChange(m_pObj->GetDataObject(), 0, ADVF_DATAONSTOP);
        m_pObj->GetDataAdviseHolder()->Release();
        m_pObj->ClearDataAdviseHolder();
    }                              

    // Tell the container that we are shutting down.    
    if (m_pObj->GetOleAdviseHolder())
    {
        m_pObj->GetOleAdviseHolder()->SendOnClose();
        m_pObj->GetOleAdviseHolder()->Release();
        m_pObj->ClearOleAdviseHolder();
    }              
        
    if (m_pObj->GetOleClientSite())
    {
        m_pObj->GetOleClientSite()->Release();  
        m_pObj->ClearOleClientSite();
    }

    // release our streams and storage
    m_pObj->HandsOffStorage();
    
    // Disconnect the object.  NOTE: This call should not do anything
    // unless the container has cause a GP Fault or some other problem
    // has occured...
    OutputDebugString(TEXT("*** Before CoDisconnectObject *** \r\n"));
    CoDisconnectObject((LPOLEOBJECT)m_pObj, 0); 
    OutputDebugString(TEXT("*** After CoDisconnectObject *** \r\n"));
    
    m_pObj->Release(); // let object close
}


//**********************************************************************
//
// CSimpSvrDoc::SetStatusText
// 
// Purpose:
//                     
//      Sets the Container's status bar text
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
//      CSimpSvrObj::IsInPlaceActive    OBJ.CPP
//      IOleInPlaceFrame::SetStatusText Container
//
// Comments:                                     
//
//      Even though there is no status line in this sample, this 
//      method must be called on WM_MENUSELECT to clear the last
//      message in the status line.
//
//********************************************************************

void CSimpSvrDoc::SetStatusText()
{
    if (m_pObj->IsInPlaceActive())
        m_pObj->GetInPlaceFrame()->SetStatusText(OLESTR("\0"));
}        

//**********************************************************************
//
// CSimpSvrDoc::ShowDocWnd
// 
// Purpose:
//                     
//      Shows the Document Window
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
//      UpdateWindow                    Windows API
//
// Comments:                                     
//
//********************************************************************

void CSimpSvrDoc::ShowDocWnd()
{
    ShowWindow(m_hDocWnd, SW_SHOWNORMAL);  // Show the window                        
    UpdateWindow(m_hDocWnd);               // Sends WM_PAINT message                 
}

//**********************************************************************
//
// CSimpSvrDoc::ShowHatchWnd
// 
// Purpose:
//                     
//      Shows the hatch Window
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
//
// Comments:                                     
//
//********************************************************************

void CSimpSvrDoc::ShowHatchWnd()
{
    ShowWindow(m_hHatchWnd, SW_SHOW); 
}  

//**********************************************************************
//
// CSimpSvrDoc::HideDocWnd
// 
// Purpose:
//                     
//      Hides the DocumentWindow
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
//
// Comments:                                     
//
//********************************************************************

void CSimpSvrDoc::HideDocWnd()
{
    ShowWindow(m_hDocWnd, SW_HIDE);
}

//**********************************************************************
//
// CSimpSvrDoc::HideHatchWnd
// 
// Purpose:
//                     
//      Hides the Hatch Window
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
//
// Comments:                                     
//
//********************************************************************

void CSimpSvrDoc::HideHatchWnd()
{
    ShowWindow(m_hHatchWnd, SW_HIDE);
}
