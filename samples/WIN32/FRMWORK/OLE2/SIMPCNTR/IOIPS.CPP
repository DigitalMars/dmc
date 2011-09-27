//**********************************************************************
// File name: IOIPS.CPP
//
//      Implementation of IOleInPlaceSite for CSimpleSite
//
// Functions:
//
//      See SITE.H for class Definition
//
// Copyright (c) 1992-1995 Microsoft Corporation. All rights reserved.
//**********************************************************************

#include "precomp.h"
#include "app.h"            
#include "site.h"
#include "doc.h"   
                                                        
//**********************************************************************
//
// CSimpleSite::GetWindow  
// 
// Purpose:
//                  
//      Returns the Window Handle of the client site
//  
// Parameters:
// 
//      HWND *lphwnd        - place to return the handle
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

STDMETHODIMP CSimpleSite::GetWindow(HWND *lphwnd) 
{
    OutputDebugString(TEXT("In IOIPS::GetWindow\r\n"));
                               
    // return the handle to our editing window.                               
    *lphwnd = m_lpDoc->m_hDocWnd;
    
    return S_OK;
}

//**********************************************************************
//
// CSimpleSite::ContextSensitiveHelp
// 
// Purpose:
//
//  
// Parameters:
//                                      
//      BOOL fEnterMode - TRUE for entering Context Sensitive help mode
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
//      Be sure to read the technotes included with the OLE toolkit.
//
//********************************************************************

STDMETHODIMP CSimpleSite::ContextSensitiveHelp(BOOL fEnterMode)
{
    OutputDebugString(TEXT("In IOIPS::ContextSensitiveHelp\r\n"));

    m_lpDoc->m_lpApp->m_fCSHMode = fEnterMode;
                               
    return S_OK;
}

//**********************************************************************
//
// CSimpleSite::CanInPlaceActivate
// 
// Purpose:
//                                    
//      Object calls to find out if the container can InPlace activate
//  
// Parameters:                                                        
//
//      None 
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

STDMETHODIMP CSimpleSite::CanInPlaceActivate()
{
    OutputDebugString(TEXT("In IOIPS::CanInPlaceActivate\r\n"));
    
    // return S_OK to indicate we can in-place activate
    return S_OK;
}

//**********************************************************************
//
// CSimpleSite::OnInPlaceActivate
// 
// Purpose:
//
//      Called by the object on InPlace Activation
//  
// Parameters:                                    
//
//      None 
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

STDMETHODIMP CSimpleSite::OnInPlaceActivate()
{
    OutputDebugString(TEXT("In IOIPS::OnInPlaceActivate\r\n"));
    
    HRESULT hrErr = m_lpOleObject->QueryInterface(
        IID_IOleInPlaceObject, (LPVOID*)&m_lpInPlaceObject);
    if (hrErr != NOERROR)
        return E_FAIL;

    // return S_OK to indicate we can in-place activate.
    return S_OK;
}

//**********************************************************************
//
// CSimpleSite::OnUIActivate
// 
// Purpose:
//                              
//      Object calls this method when it displays it's UI.
//  
// Parameters:                                            
//
//      None. 
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

STDMETHODIMP CSimpleSite::OnUIActivate()
{
    OutputDebugString(TEXT("In IOIPS::OnUIActivate\r\n"));
    
    m_lpDoc->m_fAddMyUI=FALSE;
    m_lpDoc->m_fInPlaceActive = TRUE;
    m_fInPlaceActive = TRUE;

    m_lpInPlaceObject->GetWindow((HWND*)&m_hwndIPObj);

    // return S_OK to continue in-place activation    
    return S_OK;
}

//**********************************************************************
//
// CSimpleSite::GetWindowContext 
// 
// Purpose:
//
//      Called by the object to get information for InPlace Negotiation.
//  
// Parameters:
// 
//      LPOLEINPLACEFRAME* lplpFrame        - Location to return a pointer
//                                            to IOleInPlaceFrame.
//      
//      LPOLEINPLACEUIWINDOW* lplpDoc       - Location to return a pointer
//                                            to IOleInPlaceUIWindow.
//
//      LPRECT lprcPosRect                  - The rect that the object 
//                                            occupies
//
//      LPRECT lprcClipRect                 - The clipping rect
//
//      LPOLEINPLACEFRAMEINFO lpFrameInfo   - Pointer to FRAMEINFO
//                                   
//
// Return Value:
//                                                                
//      S_OK
//
// Function Calls:
//      Function                    Location
//
//      COleInPlaceFrame::AddRef    IOIPF.CPP      
//      CSimpleSite::GetObjRect     SITE.CPP
//      OutputDebugString           Windows API       
//      SetMapMode                  Windows API
//      GetDC                       Windows API
//      ReleaseDC                   Windows API
//      CopyRect                    Windows API
//      GetClientRect               Windows API
//
// Comments:
//
//********************************************************************

STDMETHODIMP CSimpleSite::GetWindowContext(LPOLEINPLACEFRAME* lplpFrame,
                               LPOLEINPLACEUIWINDOW* lplpDoc,
                               LPRECT lprcPosRect,
                               LPRECT lprcClipRect,
                               LPOLEINPLACEFRAMEINFO lpFrameInfo)
{
    OutputDebugString(TEXT("In IOIPS::GetWindowContext\r\n"));

    // Get the frame pointer from the application object.
    m_lpDoc->m_lpApp->QueryInterface(IID_IOleInPlaceFrame, (LPVOID*)lplpFrame);
    *lplpDoc = NULL;  // must be NULL, cause we're SDI.

    // get the size of the object in pixels
    GetObjRect(lprcPosRect);

    // fill the clipping region
    GetClientRect(m_lpDoc->m_hDocWnd, lprcClipRect);

    // fill the FRAMEINFO
    lpFrameInfo->fMDIApp = FALSE;
    lpFrameInfo->hwndFrame = m_lpDoc->m_lpApp->m_hAppWnd;
    lpFrameInfo->haccel = NULL;
    lpFrameInfo->cAccelEntries = 0;

    return S_OK;
}

//**********************************************************************
//
// CSimpleSite::Scroll
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

STDMETHODIMP CSimpleSite::Scroll(SIZE scrollExtent)
{
    OutputDebugString(TEXT("In IOIPS::Scroll\r\n"));

    return E_FAIL;
}

//**********************************************************************
//
// CSimpleSite::OnUIDeactivate
// 
// Purpose:
//                     
//      Called by the object when its UI goes away
//  
// Parameters:
//                                
//       BOOL fUndoable
// 
// Return Value:
//                                                
//      S_OK
//
// Function Calls:
//      Function                    Location
//
//      OutputDebugString           Windows API
//      CSimpleAPP::AddFrameLevelUI APP.CPP
//
// Comments:
//
//********************************************************************

STDMETHODIMP CSimpleSite::OnUIDeactivate(BOOL fUndoable)
{
    OutputDebugString(TEXT("In IOIPS::OnUIDeactivate\r\n"));

    // need to clear this flag first
    m_lpDoc->m_fInPlaceActive = FALSE; 
    m_fInPlaceActive = FALSE;

    m_lpDoc->m_lpApp->QueryNewPalette();
    m_lpDoc->m_lpApp->AddFrameLevelUI();

    return S_OK;
}

//**********************************************************************
//
// CSimpleSite::OnInPlaceDeactivate
// 
// Purpose:
//                                     
//      Called when the inplace session is over
//  
// Parameters:                                 
//
//      None 
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

STDMETHODIMP CSimpleSite::OnInPlaceDeactivate()
{
    OutputDebugString(TEXT("In IOIPS::OnInPlaceDeactivate\r\n"));
    
    if (m_lpInPlaceObject)
    {
        m_lpInPlaceObject->Release();
        m_lpInPlaceObject = NULL;
    }
    return S_OK;
}

//**********************************************************************
//
// CSimpleSite::DiscardUndoState
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

STDMETHODIMP CSimpleSite::DiscardUndoState()
{
    OutputDebugString(TEXT("In IOIPS::DiscardUndoState\r\n"));

    return E_FAIL;
}

//**********************************************************************
//
// CSimpleSite::DeactivateAndUndo
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

STDMETHODIMP CSimpleSite::DeactivateAndUndo()
{
    OutputDebugString(TEXT("In IOIPS::DeactivateAndUndo\r\n"));

    return E_FAIL;
}

//**********************************************************************
//
// CSimpleSite::OnPosRectChange
// 
// Purpose:                
//
//      The object calls this method when it's size changes during an
//      InPlace Session
//  
// Parameters:
// 
//      LPCRECT lprcPosRect -   The new object rect
// 
// Return Value:
//                                                 
//      S_OK
//
// Function Calls:
//      Function                            Location
//
//      OutputDebugString                   Windows API
//      GetClientRect                       Windows API
//      IOleObject::GetExtent               Object
//      IOleObject::QueryInterface          Object
//      IOleInPlaceObject::SetObjectRects   Object
//      IOleInPlaceObject::Release          Object
//
// Comments:
//
//********************************************************************

STDMETHODIMP CSimpleSite::OnPosRectChange(LPCRECT lprcPosRect)
{
    OutputDebugString(TEXT("In IOIPS::OnPosRectChange\r\n"));

    // update the size in the document object
    // NOTE: here we must call IOleObject::GetExtent to get actual extents
    //       of the running object. IViewObject2::GetExtent returns the
    //       last cached extents.
    m_lpOleObject->GetExtent(DVASPECT_CONTENT, &m_sizel);

    RECT rect;
    GetClientRect(m_lpDoc->m_hDocWnd, &rect);

    // tell the object its new size
    m_lpInPlaceObject->SetObjectRects(lprcPosRect, &rect);

    return S_OK;
}
