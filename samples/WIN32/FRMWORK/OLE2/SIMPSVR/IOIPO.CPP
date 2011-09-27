//**********************************************************************
// File name: IOIPO.CPP
//
//    Implementation file of IOleInPlaceObject for CSimpSvrObj
//
// Functions:
//
//    See ioipo.h for a list of member functions.
//
// Copyright (c) 1993 - 1995 Microsoft Corporation. All rights reserved.
//**********************************************************************

#include "precomp.h"
#include "obj.h"
#include "app.h"            
#include "doc.h"


//**********************************************************************
//
// CSimpSvrObj::InPlaceDeactivate
// 
// Purpose:
//                       
//      Called to deactivat the object
//  
// Parameters:
//                         
//      None       
// 
// Return Value:
//          
//
// 
// Function Calls:
//      Function                                Location
//                             
//      IOleClientSite::QueryInterface          Container
//      IOleInPlaceSite::OnInPlaceDeactivate    Container
//      IOleInPlaceSite::Release                Container
//
// Comments:
//
//
//********************************************************************

STDMETHODIMP CSimpSvrObj::InPlaceDeactivate()
{
     OutputDebugString(TEXT("In IOIPO::InPlaceDeactivate\r\n"));

     // if not inplace active, return NOERROR
     if (!m_fInPlaceActive)
         return NOERROR;

     // clear inplace flag
     m_fInPlaceActive = FALSE;

     // deactivate the UI
     UIDeactivate();
     DoInPlaceHide();

     // tell the container that we are deactivating.
     if (m_pIPSite)
     {
         m_pIPSite->OnInPlaceDeactivate();
         m_pIPSite->Release();
         m_pIPSite =NULL;
     }

    return S_OK;
}

//**********************************************************************
//
// CSimpSvrObj::UIDeactivate
// 
// Purpose:
//                       
//      Instructs us to remove our UI.
//  
// Parameters:
//                         
//      None       
// 
// Return Value:
//          
// 
// Function Calls:
//      Function                                Location
//                             
//      SetParent                               Windows API
//      IOleInPlaceUIWindow::SetActiveObject    Container
//      IOleInPlaceFrame::SetActiveObject       Container
//      IOleInPlaceSite::UIDeactivate           Container
//
// Comments:
//
//
//********************************************************************

STDMETHODIMP CSimpSvrObj::UIDeactivate()
{
    OutputDebugString(TEXT("In IOIPO::UIDeactivate\r\n"));

    // if not UI active, or no pointer to IOleInPlaceFrame, then
    // return NOERROR
    if (!(m_fUIActive || m_pFrame))
        return NOERROR;

    m_fUIActive = FALSE;

    // remove hatching
    SetParent(m_pDoc->GethHatchWnd(), m_pDoc->GethAppWnd());
    SetParent(m_pDoc->GethDocWnd(), m_pDoc->GethAppWnd());

    // if in an MDI container, call SetActiveObject on the DOC.
    if (m_pCntrDoc)
        m_pCntrDoc->SetActiveObject(NULL, NULL);

    if (m_pFrame)
        m_pFrame->SetActiveObject(NULL, NULL);

    // tell the container that our UI is going away.
    if (m_pIPSite)
        m_pIPSite->OnUIDeactivate(FALSE);

    return S_OK;
}

//**********************************************************************
//
// CSimpSvrObj::SetObjectRects
// 
// Purpose:
//                       
//      Called when the container clipping region or the object position
//      changes.
//  
// Parameters:
//                         
//      LPCRECT lprcPosRect     - New Position Rect.
//
//      LPCRECT lprcClipRect    - New Clipping Rect.
// 
// Return Value:
//          
//      S_OK
// 
// Function Calls:
//      Function                    Location
//                             
//      IntersectRect               Windows API
//      OffsetRect                  Windows API
//      CopyRect                    Windows API
//      MoveWindow                  Windows API
//      CSimpSvrDoc::GethHatchWnd   DOC.H
//      CSimpSvrDoc::gethDocWnd     DOC.h
//      SetHatchWindowSize          OLE2UI
//
// Comments:
//
//
//********************************************************************

STDMETHODIMP CSimpSvrObj::SetObjectRects(LPCRECT lprcPosRect, LPCRECT lprcClipRect)
{
    OutputDebugString(TEXT("In IOIPO::SetObjectRects\r\n"));

    RECT resRect;
    POINT pt;

    // Get the intersection of the clipping rect and the position rect.
    IntersectRect(&resRect, lprcPosRect, lprcClipRect);

// jms
//    m_ptOffset.x = abs(resRect.left - lprcPosRect->left);
//    m_ptOffset.y = abs(resRect.top  - lprcPosRect->top);
    m_ptOffset.x = resRect.left - lprcPosRect->left;
    m_ptOffset.y = resRect.top  - lprcPosRect->top;

    m_ptScale.x = MulDiv(lprcPosRect->right - lprcPosRect->left,
                         SCALE_BASE,
                         m_ptSize.x);
    m_ptScale.y = MulDiv(lprcPosRect->bottom - lprcPosRect->top,
                         SCALE_BASE,
                         m_ptSize.y);

    m_ptScale.x = max(SCALE_MIN, m_ptScale.x);
    m_ptScale.y = max(SCALE_MIN, m_ptScale.y);

    TCHAR szBuffer[255];
    wsprintf(szBuffer, TEXT("New Scale: %d%% x %d%%\r\n"),
             MulDiv(m_ptScale.x, 100, SCALE_BASE),
             MulDiv(m_ptScale.y, 100, SCALE_BASE));
    OutputDebugString(szBuffer);

    // Adjust the size of the Hatch Window.
    SetHatchWindowSize(m_pDoc->GethHatchWnd(),
                       (LPRECT)lprcPosRect,
                       (LPRECT)lprcClipRect,
                       &pt);

    // Calculate the actual object rect inside the hatchwnd.
    OffsetRect(&resRect, pt.x, pt.y);

    // Move the object window
    MoveWindow(m_pDoc->GethDocWnd(),
               resRect.left,
               resRect.top,
               resRect.right - resRect.left,
               resRect.bottom - resRect.top,
               TRUE /* fRepaint */);

    return S_OK;
}

//**********************************************************************
//
// CSimpSvrObj::ReactivateAndUndo
// 
// Purpose:
//                       
//      Called when the container wants to undo the last edit made in
//      the object.
//  
// Parameters:
//                         
//      None       
// 
// Return Value:
//      INPLACE_E_NOTUNDOABLE
// 
// Comments:                 
//
//      Since this server does not support undo, the value 
//      INPLACE_E_NOTUNDOABLE is always returned.
//
//********************************************************************

STDMETHODIMP CSimpSvrObj::ReactivateAndUndo()
{
    OutputDebugString(TEXT("In IOIPO::ReactivateAndUndo\r\n"));

    return INPLACE_E_NOTUNDOABLE;
}
