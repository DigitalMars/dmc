//**********************************************************************
// File name: IOIPF.CPP
//
//      Implementation of COleInPlaceFrame for CSimpleApp
//
// Functions:
//
//      See APP.H for class definition
//
// Copyright (c) 1992-1995 Microsoft Corporation. All rights reserved.
//**********************************************************************

#include "precomp.h"
#include "app.h"            
#include "site.h"
#include "doc.h"   

                                                 
//**********************************************************************
//
// CSimpleApp::GetWindow 
// 
// Purpose:
//                  
//      Returns the frame window handle
//  
// Parameters:
//                             
//      HWND *lphwnd        - Location to return the window handle
// 
// Return Value:
//                                                                
//      S_OK
//
// Function Calls:
//      Function                    Location
//                   
//      OutputDebugString           Windows API
//
// Comments:
//
//********************************************************************

STDMETHODIMP CSimpleApp::GetWindow(HWND *lphwnd)
{
    OutputDebugString(TEXT("In IOIPF::GetWindow\r\n"));

    *lphwnd = m_hAppWnd;
    return S_OK;
}

//**********************************************************************
//
// CSimpleApp::ContextSensitiveHelp
// 
// Purpose:
//                  
//      Used in implementing Context sensitive help
//  
// Parameters:
//                                       
//      BOOL fEnterMode -   TRUE if starting Context Sensitive help mode
// 
// Return Value:
//               
//      S_OK
//
// Function Calls:
//      Function                                    Location
//  
//      OutputDebugString                           Windows API
//      IOleInPlaceActiveObject::QueryInterface     Object
//      IOleInPlaceObject::ContextSensitiveHelp     Object
//      IOleInPlaceObject::Release                  Object
//
// Comments:
//                                         
//      Be sure to read the technotes in the OLE toolkit.
//
//********************************************************************

STDMETHODIMP CSimpleApp::ContextSensitiveHelp(BOOL fEnterMode)
{
    OutputDebugString(TEXT("In IOIPF::ContextSensitiveHelp\r\n"));

    if (m_fCSHMode != fEnterMode)
    {
        m_fCSHMode = fEnterMode;

        // this code "trickles" the context sensitive help via shift+f1
        // to the inplace active object.  See the technotes for implementation
        // details.
        if (m_lpDoc->m_lpActiveObject)
        {
            LPOLEINPLACEOBJECT lpInPlaceObject;
            m_lpDoc->m_lpActiveObject->QueryInterface(IID_IOleInPlaceObject,
                                                      (LPVOID*)&lpInPlaceObject);
            lpInPlaceObject->ContextSensitiveHelp(fEnterMode);
            lpInPlaceObject->Release();
        }
    }
    return S_OK;
}

//**********************************************************************
//
// CSimpleApp::GetBorder
// 
// Purpose:
//                      
//      Returns the outermost border that frame adornments can be attached
//      during InPlace Activation.
//  
// Parameters:
//                            
//      LPRECT lprectBorder - return parameter to contain the outermost 
//                            rect for frame adornments
// 
// Return Value:                                       
//
//      S_OK
//       
// Function Calls:
//      Function                    Location
//  
//      OutputDebugString           Windows API
//      GetClientRect               Windows API
//      CopyRect                    Windows API
//
// Comments:
//
//********************************************************************

STDMETHODIMP CSimpleApp::GetBorder(LPRECT lprectBorder)
{   
    OutputDebugString(TEXT("In IOIPF::GetBorder\r\n"));

    // get the rect for the entire frame.
    GetClientRect(m_hAppWnd, lprectBorder);

    return S_OK;
}

//**********************************************************************
//
// CSimpleApp::RequestBorderSpace
// 
// Purpose:
//                                    
//      Approves/Denies requests for border space during InPlace 
//      negotiation.
//  
// Parameters:
//                                     
//      LPCBORDERWIDTHS lpborderwidths  - The width in pixels needed on 
//                                        each side of the frame.
// 
// Return Value:
//                                                               
//      S_OK
//
// Function Calls:
//      Function                    Location
//
//      OutputDebugString           Windows API
//
// Comments:
//                                                                  
//      This implementation doesn't care about how much border space
//      is used.  It always returns S_OK.
//
//********************************************************************

STDMETHODIMP CSimpleApp::RequestBorderSpace(LPCBORDERWIDTHS lpborderwidths)
{
    OutputDebugString(TEXT("In IOIPF::RequestBorderSpace\r\n"));
    
    // always approve the request
    return S_OK;
}

//**********************************************************************
//
// CSimpleApp::SetBorderSpace 
// 
// Purpose:
//                                                     
//      The object calls this method when it is actually going to 
//      start using the border space.
//  
// Parameters:
//                                  
//      LPCBORDERWIDTHS lpborderwidths  - Border space actually being used
//                                        by the object
// 
// Return Value:
//                                   
//      S_OK
//
// Function Calls:
//      Function                        Location
//
//      CSimpleApp::AddFrameLevelTools  APP.CPP
//      OutputDebugString               Windows API
//      GetClientRect                   Windows API
//      MoveWindow                      Windows API
//
// Comments:
//                        
//      This routine could be a little smarter and check to see if
//      the object is requesting the entire client area of the 
//      window.
//
//********************************************************************

STDMETHODIMP CSimpleApp::SetBorderSpace(LPCBORDERWIDTHS lpborderwidths)
{   
    OutputDebugString(TEXT("In IOIPF::SetBorderSpace\r\n"));
    
    if (lpborderwidths == NULL)
        AddFrameLevelTools();
    else
    {
        RECT rect;                                                           

        GetClientRect(m_hAppWnd, &rect);
    
        MoveWindow(m_lpDoc->m_hDocWnd,
                   rect.left + lpborderwidths->left,
                   rect.top + lpborderwidths->top,
                   rect.right - lpborderwidths->right - lpborderwidths->left,
                   rect.bottom - lpborderwidths->bottom - lpborderwidths->top,
                   TRUE);         
    }     

    return S_OK;
}

//**********************************************************************
//
// CSimpleApp::SetActiveObject 
// 
// Purpose:
//
//  
// Parameters:
// 
//      LPOLEINPLACEACTIVEOBJECT lpActiveObject     -   Pointer to the
//                                                      objects 
//                                                      IOleInPlaceActiveObject
//                                                      interface
//
//      LPCOLESTR lpszObjName                       -   Name of the object
// 
// Return Value:
//
//      S_OK
//       
// Function Calls:
//      Function                            Location
//
//      OutputDebugString                   Windows API                           
//      IOleInPlaceActiveObject::AddRef     Object
//      IOleInPlaceActiveObject::Release    Object
//
// Comments:
//
//********************************************************************

STDMETHODIMP CSimpleApp::SetActiveObject(LPOLEINPLACEACTIVEOBJECT lpActiveObject, LPCOLESTR lpszObjName)
{
    OutputDebugString(TEXT("In IOIPF::SetActiveObject\r\n"));

    // AddRef() it and save it...
    if (lpActiveObject)
    {
        lpActiveObject->AddRef();

        lpActiveObject->GetWindow(&m_hwndUIActiveObj);
        if (m_hwndUIActiveObj)
            SendMessage(m_hwndUIActiveObj, WM_QUERYNEWPALETTE, 0, 0L);
    }
    else 
    {
        if (m_lpDoc->m_lpActiveObject)
            m_lpDoc->m_lpActiveObject->Release();
        m_hwndUIActiveObj = NULL;
    }

    // in an MDI app, this method really shouldn't be called,
    // this method associated with the doc is called instead.

    m_lpDoc->m_lpActiveObject = lpActiveObject;
    // should set window title here

    return S_OK;
}

//**********************************************************************
//
// CSimpleApp::InsertMenus 
// 
// Purpose:
//
//      Inserts the container menu into the combined menu
//  
// Parameters:
//                               
//      HMENU hmenuShared                   -   Menu Handle to be set.
//      LPOLEMENUGROUPWIDTHS lpMenuWidths   -   Width of menus
// 
// Return Value:
//       
// Function Calls:
//      Function                    Location
//
//      OutputDebugString           Windows API
//      AppendMenu                  Windows API
//
// Comments:
//
//********************************************************************

STDMETHODIMP CSimpleApp::InsertMenus (HMENU hmenuShared, LPOLEMENUGROUPWIDTHS lpMenuWidths)
{   
    OutputDebugString(TEXT("In IOIPF::InsertMenus\r\n"));
    
    AppendMenu(hmenuShared, MF_BYPOSITION | MF_POPUP, (UINT)m_hFileMenu, TEXT("&File"));
    AppendMenu(hmenuShared, MF_BYPOSITION | MF_POPUP, (UINT)m_hHelpMenu, TEXT("&Other"));
    
    lpMenuWidths->width[0] = 1;
    lpMenuWidths->width[2] = 0;
    lpMenuWidths->width[4] = 1;
    
    return S_OK;
}


//**********************************************************************
//
// CSimpleApp::SetMenu
// 
// Purpose:
//                          
//      Sets the application menu to the combined menu
//  
// Parameters:
// 
//      HMENU hmenuShared       - The combined menu
//
//      HOLEMENU holemenu       - Used by OLE
//
//      HWND hwndActiveObject   - Used by OLE
// 
// Return Value:
//                                           
//      S_OK
//
// Function Calls:
//      Function                    Location
//
//      OutputDebugString           Windows API
//      SetMenu                     Windows API
//      OleSetMenuDescriptor        OLE API
//
// Comments:
//
//********************************************************************

STDMETHODIMP CSimpleApp::SetMenu(HMENU hmenuShared, HOLEMENU holemenu, HWND hwndActiveObject)
{                                                                                          
    OutputDebugString(TEXT("In IOIPF::SetMenu\r\n"));
    
    HMENU hMenu = m_hMainMenu;
    
    if (holemenu)
        hMenu = hmenuShared;
    
    // call the windows api, not this method
    ::SetMenu(m_hAppWnd, hMenu);
    
    OleSetMenuDescriptor(holemenu, m_hAppWnd, hwndActiveObject, this, m_lpDoc->m_lpActiveObject);
    
    return S_OK;
}


//**********************************************************************
//
// CSimpleApp::RemoveMenus 
// 
// Purpose:
//                   
//      Removes the container menus from the combined menu
//  
// Parameters:
//                               
//      HMENU hmenuShared   - Handle to the combined menu.
// 
// Return Value:
//                                                        
//      S_OK
//
// Function Calls:
//      Function                    Location
//
//      OutputDebugString           Windows API
//      GetMenuItemCount            Windows API
//      RemoveMenu                  Windows API
//
// Comments:
//
//********************************************************************

STDMETHODIMP CSimpleApp::RemoveMenus (HMENU hmenuShared)
{   
    int retval;
    
    OutputDebugString(TEXT("In IOIPF::RemoveMenus\r\n"));

    while ((retval = GetMenuItemCount(hmenuShared)) && (retval != -1))
        RemoveMenu(hmenuShared, 0, MF_BYPOSITION);

    return S_OK;
}

//**********************************************************************
//
// CSimpleApp::SetStatusText
// 
// Purpose:
//                                
//      Not Implemented
//  
// Parameters:
//                                
//      Not Implemented
//  
// Return Value:
//                                
//      Not Implemented
//  
// Function Calls:
//      Function                    Location
//
//      OutputDebugString           Windows API
//
// Comments:
//                                
//      This function is not implemented due to the fact
//      that this application does not have a status bar.
//  
//********************************************************************

STDMETHODIMP CSimpleApp::SetStatusText(LPCOLESTR lpszStatusText)
{
    TCHAR szMsg[256];

    wsprintf(szMsg, TEXT("In IOIPF::SetStatusText: %s\r\n"), lpszStatusText);
    OutputDebugString(szMsg);

    return E_FAIL;
}

//**********************************************************************
//
// CSimpleApp::EnableModeless
// 
// Purpose:
//                                
//      Enables/Disables container modeless dialogs
//  
// Parameters:
//                                
//      BOOL fEnable    - Enable/Disable
//  
// Return Value:
//                                
//      S_OK
//  
// Function Calls:
//      Function                    Location
//
//      OutputDebugString           Windows API
//
// Comments:
//                                
//      There are no modeless dialogs in this application, so the
//      implementation of this method is trivial.
//  
//********************************************************************

STDMETHODIMP CSimpleApp::EnableModeless(BOOL fEnable)
{
    OutputDebugString(TEXT("In IOIPF::EnableModeless\r\n"));

    return S_OK;
}

//**********************************************************************
//
// CSimpleApp::TranslateAccelerator
// 
// Purpose:
//                                
//      Not Implemented
//  
// Parameters:
//                                
//      Not Implemented
//  
// Return Value:
//                                
//      Not Implemented
//  
// Function Calls:
//      Function                    Location
//
//      OutputDebugString           Windows API
//
// Comments:
//                                
//      Not Implemented
//  
//********************************************************************

STDMETHODIMP CSimpleApp::TranslateAccelerator(LPMSG lpmsg, WORD wID)
{
    OutputDebugString(TEXT("In IOIPF::TranslateAccelerator\r\n"));

    return S_FALSE;
}
