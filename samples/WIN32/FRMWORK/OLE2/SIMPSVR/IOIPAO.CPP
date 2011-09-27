//**********************************************************************
// File name: IOIPAO.CPP
//
//    Implementation of IOleInPlaceActiveObject for CSimpSvrObj
//
// Functions:
//
//    See obj.h for a list of member functions.
//
// Copyright (c) 1993 - 1995 Microsoft Corporation. All rights reserved.
//**********************************************************************

#include "precomp.h"
#include "obj.h"
#include "app.h"            
#include "doc.h"   

//**********************************************************************
//
// CSimpSvrObj::GetWindow
// 
// Purpose:
//                       
//      Gets the objects Window Handle.
//  
// Parameters:
//                         
//      HWND* lphwnd        - Location to return the window handle.
// 
// Return Value:
//          
//      S_OK
// 
// Function Calls:
//      Function                    Location
//                             
//      CSimpSvrDoc::GethDocWnd     DOC.H
//
//
// Comments:
//
//
//********************************************************************

STDMETHODIMP CSimpSvrObj::GetWindow(HWND* lphwnd)
{
    OutputDebugString(TEXT("In IOIPAO::GetWindow\r\n"));

    *lphwnd = m_pDoc->GethDocWnd();

    return S_OK;
}

//**********************************************************************
//
// CSimpSvrObj::ContextSensitiveHelp
// 
// Purpose:
//                       
//      Used to implement Context Sensitive help
//  
// Parameters:
//                         
//      None       
// 
// Return Value:
//          
//      E_NOTIMPL
// 
// Function Calls:
//      Function                    Location
//                             
// Comments:
//               
//      See TECHNOTES.WRI include with the OLE SDK for proper 
//      implementation of this function.
//
//********************************************************************

STDMETHODIMP CSimpSvrObj::ContextSensitiveHelp(BOOL fEnterMode)
{
    OutputDebugString(TEXT("In IOIPAO::ContextSensitiveHelp\r\n"));

    return E_NOTIMPL;
}

//**********************************************************************
//
// CSimpSvrObj::TranslateAccelerator
// 
// Purpose:
//                       
//      Used for translating accelerators in .DLL objects.
//  
// Parameters:
//                         
//      LPMSG lpmsg - Pointer to a message
// 
// Return Value:
//          
//      S_FALSE 
// 
// Function Calls:
//      Function                    Location
//                             
// Comments:
//              
//      This method should never be called since we are implemented
//      in an executable.
//
//********************************************************************

STDMETHODIMP CSimpSvrObj::TranslateAccelerator(LPMSG lpmsg)
{
    OutputDebugString(TEXT("In IOIPAO::TranslateAccelerator\r\n"));

    // no accelerator table, return FALSE
    return S_FALSE;
}

//**********************************************************************
//
// CSimpSvrObj::OnFrameWindowActivate
// 
// Purpose:
//                       
//      Called when the Frame window is (de)activating
//  
// Parameters:
//                         
//      BOOL fActivate  - TRUE if activating, FALSE if Deactivating
// 
// Return Value:
//          
//      S_OK
// 
// Function Calls:
//      Function                    Location
//                             
//      SetFocus                    Windows API
//
//
// Comments:
//
//
//********************************************************************

STDMETHODIMP CSimpSvrObj::OnFrameWindowActivate(BOOL fActivate)
{
    OutputDebugString(TEXT("In IOIPAO::OnFrameWindowActivate\r\n"));

// jms - kosher?
    // set the focus to the object window if we are activating.
    if (fActivate)
        SetFocus(m_pDoc->GethDocWnd());

    return S_OK;
}

//**********************************************************************
//
// CSimpSvrObj::OnDocWindowActivate
//                                             
// Purpose:
//                       
//      Called when the doc window (in an MDI App) is (de)activated.
//  
// Parameters:
//                         
//      BOOL fActivate  - TRUE if activating, FALSE if deactivating       
// 
// Return Value:
//          
//      S_OK
// 
// Function Calls:
//      Function                            Location
//                             
//      CSimpSvrObject::AddFrameLevelUI     OBJ.CPP
//
//
// Comments:
//
//
//********************************************************************

STDMETHODIMP CSimpSvrObj::OnDocWindowActivate(BOOL fActivate)
{
    OutputDebugString(TEXT("In IOIPAO::OnDocWindowActivate\r\n"));

    // Activating?
    if (fActivate)
        AddFrameLevelUI();

    // No frame level tools to remove...

    return S_OK;
}

//**********************************************************************
//
// CSimpSvrObj::ResizeBorder
// 
// Purpose:
//                       
//      Called when the border changes size.
//  
// Parameters:
//                         
//      LPCRECT lprectBorder                - New Border
//
//      LPOLEINPLACEUIWINDOW lpUIWindow     - Pointer to UIWindow
//
//      BOOL fFrameWindow                   - True if lpUIWindow is the
//                                            frame window.
// 
// Return Value:
//          
//      S_OK
// 
// Function Calls:
//      Function                    Location
//                             
// Comments:
//
//      Need to call SetBorderSpace again...
//
//********************************************************************

STDMETHODIMP CSimpSvrObj::ResizeBorder(LPCRECT lprectBorder,
                                       LPOLEINPLACEUIWINDOW lpUIWindow,
                                       BOOL fFrameWindow)
{
    OutputDebugString(TEXT("In IOIPAO::ResizeBorder\r\n"));

// jms
#if 0
    // should always have an inplace frame...
    GetInPlaceFrame()->SetBorderSpace(NULL);

    // There will only be a UIWindow if in an MDI container
    if (GetUIWindow())
        GetUIWindow()->SetBorderSpace(NULL);
#else
    lpUIWindow->SetBorderSpace(NULL);   // don't need any border space
#endif
    return S_OK;
}

//**********************************************************************
//
// CSimpSvrObj::EnableModeless
// 
// Purpose:
//                       
//      Called to enable/disable modeless dialogs.
//  
// Parameters:
//                         
//      BOOL fEnable    - TRUE to enable, FALSE to disable
// 
// Return Value:
//          
//      S_OK
// 
// Function Calls:
//      Function                    Location
//                             
// Comments:
//          
//      Called by the container when a model dialog box is added/removed
//      from the screen.  The appropriate action for a server application
//      is to disable/enable any modeless dialogs currently being displayed.
//      Since this application doesn't display any modeless dialogs,
//      this method is essentially ignored.
//
//********************************************************************

STDMETHODIMP CSimpSvrObj::EnableModeless(BOOL fEnable)
{
    OutputDebugString(TEXT("In IOIPAO::EnableModeless\r\n"));

    return S_OK;
}
