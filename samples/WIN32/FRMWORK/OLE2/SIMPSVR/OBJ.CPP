//**********************************************************************
// File name: obj.cpp
//
//    Implementation file for the CSimpSvrApp Class
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
// CSimpSvrObj::QueryInterface
// 
// Purpose:
//                                  
//      Used for interface negotiation at the "Object" level.
//  
// Parameters:
//                           
//      REFIID riid         -   A reference to the interface that is
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
//      IsEqualIID                  OLE API
//      IUnknown::AddRef            OBJ.CPP
//
// Comments:                                      
//
//
//********************************************************************

STDMETHODIMP CSimpSvrObj::QueryInterface(REFIID riid, LPVOID* ppv)
{
    OutputDebugString(TEXT("In CSimpSvrObj::QueryInterface\r\n"));

    *ppv = NULL;

    if (IsEqualIID(riid, IID_IUnknown) || IsEqualIID(riid, IID_IOleObject))
        *ppv = (LPOLEOBJECT)this;
    else if (IsEqualIID(riid, IID_IPersistStorage) || IsEqualIID(riid, IID_IPersist))
        *ppv = (LPPERSISTSTORAGE)this;
    else if (IsEqualIID(riid, IID_IDataObject))
        *ppv = (LPDATAOBJECT)this;
    else if (IsEqualIID(riid, IID_IOleInPlaceActiveObject))
        *ppv = (LPOLEINPLACEACTIVEOBJECT)this;
    else if (IsEqualIID(riid, IID_IOleInPlaceObject))
        *ppv = (LPOLEINPLACEOBJECT)this;
    else if (IsEqualIID(riid, IID_IExternalConnection))
         *ppv = (LPEXTERNALCONNECTION)this;

    if (*ppv)
    {
        ((LPUNKNOWN)*ppv)->AddRef();
        return S_OK;
    }

    return E_NOINTERFACE;
}

//**********************************************************************
//
// CSimpSvrObj::AddRef
// 
// Purpose:
//                   
//      Adds to the reference count at the Object level.
//  
// Parameters:
//                                                           
//      None
// 
// Return Value:
//          
//      ULONG   -   The new reference count of the Object.
//
// Function Calls:
//      Function                    Location
//
// Comments:
//
//      Due to the reference counting model that is used in this
//      implementation, this reference count is the sum of the
//      reference counts on all interfaces 
//
//********************************************************************

STDMETHODIMP_(ULONG) CSimpSvrObj::AddRef()
{
    OutputDebugString(TEXT("In CSimpSvrObj::AddRef\r\n"));

    m_pDoc->AddRef();

    return ++m_cRef;
}

//**********************************************************************
//
// CSimpSvrObj::Release
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
//      ULONG   -   The new reference count of the object.
//
// Function Calls:
//      Function                    Location
//
// Comments:
//
//      Due to the reference counting model that is used in this
//      implementation, this reference count is the sum of the
//      reference counts on all interfaces
//
//      Because ~CSimpSvrObj() references m_pDoc, the CSimpSvrObj
//      object must be deleted before the CSimpSvrDoc is freed.  This
//      is the reason for the gyrations surrounding "delete this" below.
//
//********************************************************************

STDMETHODIMP_(ULONG) CSimpSvrObj::Release()
{
    OutputDebugString(TEXT("In CSimpSvrObj::Release\r\n"));

    int nTmp = --m_cRef;
    CSimpSvrDoc* pDoc = m_pDoc;     // save doc pointer

    if (0 == nTmp)
        delete this;        // obj must be deleted before doc
  
    pDoc->Release();        // release the doc
    return nTmp;
}

//**********************************************************************
//
// CSimpSvrObj::CSimpSvrObj
// 
// Purpose:
// 
//      Constructor for CSimpSvrObj
//  
// Parameters:                                      
//
//      CSimpSvrDoc* lpSimpSvrDoc   - ptr to the doc object
// 
// Return Value:
//          
//      None
//
// Function Calls:
//      Function                    Location
//
//
// Comments:
//
//
//********************************************************************

CSimpSvrObj::CSimpSvrObj(CSimpSvrDoc* pDoc)
{
    m_cRef                  = 0;
    m_pDoc                  = pDoc;
    m_fInPlaceActive        = FALSE;
    m_fInPlaceVisible       = FALSE;
    m_fUIActive             = FALSE;
    m_hmenuShared           = NULL;
    m_hOleMenu              = NULL;
    m_FrameInfo.cb          = sizeof(m_FrameInfo);
    m_fSaveWithSameAsLoad   = FALSE;
    m_fNoScribbleMode       = FALSE;
    m_fOpen                 = FALSE;
    m_dwStrong              = 0;
    m_dwRegister            = 0;

    m_red                   = 192;
    m_green                 = 0;
    m_blue                  = 0;

    m_ptSize.x              = 100;
    m_ptSize.y              = 100;

    m_ptOffset.x            = 0;
    m_ptOffset.y            = 0;

    m_ptScale.x             = SCALE_BASE;   // Scale at 100% initially
	m_ptScale.y             = SCALE_BASE;

    m_hWndParent            = NULL;

    m_pStorage              = NULL;
    m_pColorStm             = NULL;
    m_pSizeStm              = NULL;
    m_pOleClientSite        = NULL;
    m_pOleAdviseHolder      = NULL;
    m_pDataAdviseHolder     = NULL;
    m_pFrame                = NULL;
    m_pCntrDoc              = NULL;
    m_pIPSite               = NULL;
}

//**********************************************************************
//
// CSimpSvrObj::~CSimpSvrObj
// 
// Purpose:
// 
//      Destructor for CSimpSvrObj
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
//      Function                    Location
//
//      PostMessage                 Windows API
//      CSimpSvrDoc::GetApp         DOC.H
//      CSimpSvrDoc::GethAppWnd     DOC.H
//      CSimpSvrDoc::ClearObj       DOC.H
//      CSimpSvrApp::IsStartedByOle APP.CPP
//
// Comments:
//
//
//********************************************************************

CSimpSvrObj::~CSimpSvrObj()
{
    OutputDebugString(TEXT("In CSimpSvrObj's Destructor \r\n"));

    // if we were started by ole, post ourselves a close message
    if (m_pDoc->GetApp()->IsStartedByOle())
        PostMessage(m_pDoc->GethAppWnd(), WM_SYSCOMMAND, SC_CLOSE, 0L);

    // clear the OBJ ptr in the doc class
    m_pDoc->ClearObj();
}

//**********************************************************************
//
// CSimpSvrObj::Draw
// 
// Purpose:
// 
//      Draws the object into an arbitrary DC
//  
// Parameters:                                      
//
//      HDC hDC - DC to draw into
// 
// Return Value:
//          
//      NONE
//
// Function Calls:
//      Function                    Location
//
//      CreateBrushIndirect         Windows API
//      SelectObject                Windows API
//      Rectangle                   Windows API
//      DeleteObject                Windows API
//
// Comments:
//
//
//********************************************************************

void CSimpSvrObj::Draw(HDC hDC, BOOL fMeta)
{
    TCHAR szBuffer[255];
    wsprintf(szBuffer, TEXT("In CSimpSvrObj::Draw: %d%% x %d%%\r\n"),
             MulDiv(m_ptScale.x, 100, SCALE_BASE),
             MulDiv(m_ptScale.y, 100, SCALE_BASE));
    OutputDebugString(szBuffer);

    if (!fMeta)
    {
        SetMapMode(hDC, MM_ANISOTROPIC);
// jms
//        SetWindowOrgEx(hDC, (int)(m_ptOffset.x / m_ptScale.x), (int)(m_ptOffset.y / m_ptScale.y), NULL);
//        SetWindowExtEx(hDC, m_ptSize.x, m_ptSize.y, NULL);
        SetViewportOrgEx(hDC, 0, 0, NULL);
        SetViewportExtEx(hDC,
                         MulDiv(m_ptSize.x, m_ptScale.x, SCALE_BASE),
                         MulDiv(m_ptSize.y, m_ptScale.y, SCALE_BASE),
                         NULL);
    }

    SetWindowOrgEx(hDC, 0, 0, NULL);
    SetWindowExtEx(hDC, m_ptSize.x, m_ptSize.y, NULL);

    // Draw the object (a colored rectangle)
    HBRUSH hBrush = CreateSolidBrush(RGB(m_red, m_green, m_blue));
    RECT rc = { 0, 0, m_ptSize.x, m_ptSize.y };

    FillRect(hDC, &rc, hBrush);
 
    DeleteObject(hBrush);
}

//**********************************************************************
//
// CSimpSvrObj::GetMetaFilePict
// 
// Purpose:
// 
//      Returns a handle to a metafile representation of the object.
//  
// Parameters:                                      
//
//      None 
// 
// Return Value:
//          
//      Handle to the metafile.
//
// Function Calls:
//      Function                        Location
//
//      GlobalAlloc                     Windows API
//      GlobalLock                      Windows API
//      SetWindowOrg                    Windows API
//      SetWindowExt                    Windows API
//      CreateMetaFile                  Windows API
//      CloseMetaFile                   Windows API             
//      GlobalUnlock                    Windows API
//      XformWidthInPixelsToHimetric    OLE2UI
//      XformHeightInPixelsToHimetric   OLE2UI
//      CSimpSvrObj::Draw               OBJ.CPP
//
// Comments:
//
//
//********************************************************************

HMETAFILEPICT CSimpSvrObj::GetMetaFilePict()
{
    HMETAFILEPICT hMFP;
    LPMETAFILEPICT lpMFP;
    HDC hdc;

    OutputDebugString(TEXT("In CSimpSvrObj::GetMetaFilePict\r\n"));

    // Allocate the memory for the METAFILEPICT structure
    hMFP = (HMETAFILEPICT)GlobalAlloc(GMEM_SHARE | GHND, sizeof(METAFILEPICT));
    lpMFP = (LPMETAFILEPICT)GlobalLock(hMFP);

    // Fill out the METAFILEPICT structure
    lpMFP->mm = MM_ANISOTROPIC;

    // Get the size of the object in HIMETRIC
    hdc = GetDC(NULL);
    lpMFP->xExt = MulDiv(m_ptSize.x, HIMETRIC_PER_INCH, GetDeviceCaps(hdc, LOGPIXELSX));
    lpMFP->yExt = MulDiv(m_ptSize.y, HIMETRIC_PER_INCH, GetDeviceCaps(hdc, LOGPIXELSY));
    ReleaseDC(NULL, hdc);

    // Create the metafile
    hdc = CreateMetaFile(NULL);

    Draw(hdc);

    lpMFP->hMF = CloseMetaFile(hdc);

    // Unlock the metafilepict
    GlobalUnlock(hMFP);

    return hMFP;
}


//**********************************************************************
//
// CSimpSvrObj::DoInPlaceActivate
// 
// Purpose:
// 
//      Does the inplace activation for the object
//  
// Parameters:                                      
//
//      LONG lVerb  - Verb that caused this function to be called
// 
// Return Value:
//          
//      TRUE/FALSE depending on success or failure.
//
// Function Calls:
//      Function                                Location
//
//      IOleClientSite::QueryInterface          Container
//      IOleClientSite::ShowObject              Container
//      IOleInPlaceSite::CanInPlaceActivate     Container
//      IOleInPlaceSite::Release                Container
//      IOleInPlaceSite::OnInPlaceActivate      Container
//      IOleInPlaceSite::GetWindow              Container 
//      IOleInPlaceSite::GetWindowContext       Container
//      IOleInPlaceSite::OnUIActivate           Container                      
//      IOleInPlaceSite::Release                Container
//      IOleInPlaceFrame::SetActiveObject       Container
//      IOleInPlaceUIWindow::SetActiveObject    Container
//      ShowWindow                              Windows API
//      SetParent                               Windows API
//      IntersectRect                           Windows API
//      OffsetRect                              Windows API
//      MoveWindow                              Windows API
//      CopyRect                                Windows API
//      SetFocus                                Windows API
//      SetHatchWindowSize                      OLE2UI
//      CSimpSvrObj::AssembleMenus              OBJ.CPP
//      CSimpSvrObj::AddFrameLevelUI            OBJ.CPP
//
//
// Comments:
//
//      Be sure to read TECHNOTES.WRI included with the OLE SDK
//      for details on implementing inplace activation.
//
//********************************************************************

BOOL CSimpSvrObj::DoInPlaceActivate(LONG lVerb)
{
    BOOL retval = FALSE;
    RECT posRect, clipRect;


    OutputDebugString(TEXT("In CSimpSvrObj::DoInPlaceActivate\r\n"));

    // if not currently in place active
    if (!m_fInPlaceActive)
    {
        // get the inplace site
        if (m_pOleClientSite->QueryInterface(IID_IOleInPlaceSite,
                                             (LPVOID*)&m_pIPSite) != NOERROR)
            goto error;

        // if the inplace site could not be obtained, or refuses to inplace
        // activate then goto error.
        if (m_pIPSite == NULL || m_pIPSite->CanInPlaceActivate() != NOERROR)
        {
            if (m_pIPSite)
                m_pIPSite->Release();
            m_pIPSite == NULL;
            goto error;
        }

        // tell the site that we are activating.
        m_pIPSite->OnInPlaceActivate();
        m_fInPlaceActive = TRUE;
    }

    // if not currently inplace visible
    if (!m_fInPlaceVisible)
    {
        m_fInPlaceVisible = TRUE;

        // get the window handle of the site
        m_pIPSite->GetWindow(&m_hWndParent);

        // get window context from the container
        m_pIPSite->GetWindowContext(&m_pFrame,
                                    &m_pCntrDoc,
                                    &posRect,
                                    &clipRect,
                                    &m_FrameInfo);

        // Set the parenting
        SetParent(m_pDoc->GethHatchWnd(), m_hWndParent);
        SetParent(m_pDoc->GethDocWnd(), m_pDoc->GethHatchWnd());

        // Show the hatch window
        m_pDoc->ShowHatchWnd();

        // Tell the client site to show the object
        m_pOleClientSite->ShowObject();

        RECT resRect;

        // Figure out the "real" size of the object
        IntersectRect(&resRect, &posRect, &clipRect);

        POINT pt;

        // Adjust our hatch window size
        SetHatchWindowSize(m_pDoc->GethHatchWnd(),
                           &posRect,
                           &clipRect,
                           &pt);

        // Calculate the object rect inside the hatchwnd.
        OffsetRect(&resRect, pt.x, pt.y);

        // move the object window
        MoveWindow(m_pDoc->GethDocWnd(),
                   resRect.left,
                   resRect.top,
                   resRect.right - resRect.left,
                   resRect.bottom - resRect.top,
                   FALSE);

        // create the combined window
        AssembleMenus();
    }

    // if not UIActive
    if (!m_fUIActive)
    {
        m_fUIActive = TRUE;

        // tell the inplace site that we are activating
        m_pIPSite->OnUIActivate();

        // set the focus to our object window
        SetFocus(m_pDoc->GethDocWnd());

        // set the active object on the frame
        m_pFrame->SetActiveObject((LPOLEINPLACEACTIVEOBJECT)this, OLESTR("Simple OLE Server"));

        // set the active object on the Doc, if available.
        if (m_pCntrDoc)
            m_pCntrDoc->SetActiveObject((LPOLEINPLACEACTIVEOBJECT)this, OLESTR("Simple OLE Server"));

        // add the frame level UI.
        AddFrameLevelUI();
    }

    retval = TRUE;
error:
    return retval;
}

//**********************************************************************
//
// CSimpSvrObj::AssembleMenus
// 
// Purpose:
// 
//      Creates the combined menus used during inplace activation.
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
//      CreateMenu                      Windows API
//      IOleInPlaceFrame::InsertMenus   Container
//      InsertMenu                      Windows API
//      DestroyMenu                     Windows API
//      OleCreateMenuDescriptor         OLE API
//
// Comments:
//
//
//********************************************************************

void CSimpSvrObj::AssembleMenus()
{
    OutputDebugString(TEXT("In CSimpSvrObj::AssembleMenus\r\n"));

    OLEMENUGROUPWIDTHS menugroupwidths;

    //  Create the menu resource
    m_hmenuShared = CreateMenu();

    // have the contaner insert its menus
    if (m_pFrame->InsertMenus(m_hmenuShared, &menugroupwidths) == NOERROR)
    {
        int nFirstGroup = (int)menugroupwidths.width[0];

        // insert the server menus
        InsertMenu(m_hmenuShared,
                   nFirstGroup,
                   MF_BYPOSITION | MF_POPUP,
                   (UINT)m_pDoc->GetApp()->GetColorMenu(),
                   TEXT("&Color"));
        menugroupwidths.width[1] = 1;
        menugroupwidths.width[3] = 0;
        menugroupwidths.width[5] = 0;
    }
    else
    {
        // Destroy the menu resource
        DestroyMenu(m_hmenuShared);
        m_hmenuShared = NULL;
    }

    // tell OLE to create the menu descriptor
    m_hOleMenu = OleCreateMenuDescriptor(m_hmenuShared, &menugroupwidths);
}

//**********************************************************************
//
// CSimpSvrObj::AddFrameLevelUI
// 
// Purpose:
// 
//      Adds the Frame level user interface
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
//      IOleInPlaceFrame::SetMenu           Container
//      IOleInPlaceFrame::SetBorderSpace    Container
//      IOleInPlaceUIWindow::SetBorderSpace Container
//      CSimpSvrDoc::GethDocWnd             DOC.H
//
// Comments:
//
//
//********************************************************************

void CSimpSvrObj::AddFrameLevelUI()
{
    OutputDebugString(TEXT("In CSimpSvrObj::AddFrameLevelUI\r\n"));

    // add the combined menu
    m_pFrame->SetMenu(m_hmenuShared, m_hOleMenu, m_pDoc->GethDocWnd());

    // do hatched border
    SetParent(m_pDoc->GethHatchWnd(), m_hWndParent);
    SetParent(m_pDoc->GethDocWnd(), m_pDoc->GethHatchWnd());

    // set the border space.  Normally we would negotiate for toolbar
    // space at this point.  Since this server doesn't have a toolbar,
    // this isn't needed...
    if (m_pFrame)
        m_pFrame->SetBorderSpace(NULL);

    if (m_pCntrDoc)
        m_pCntrDoc->SetBorderSpace(NULL);
}

//**********************************************************************
//
// CSimpSvrObj::DoInPlaceHide
// 
// Purpose:
// 
//      Hides the object while inplace active
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
//      SetParent                       Windows API
//      CSimpSvrDoc::GethDocWnd         DOC.H
//      CSimpSvrDoc::GethAppWnd         DOC.H
//      CSimpSvrDoc::GethHatchWnd       DOC.H
//      CSimpSvrObj::DisassembleMenus   OBJ.CPP
//      IOleInPlaceFrame::Release       Container
//      IOleInPlaceUIWindow::Release    Container
//
//
// Comments:
//
//      Be sure to read TECHNOTES.WRI included with the OLE SDK
//      for details on implementing inplace activation.
//
//********************************************************************

void CSimpSvrObj::DoInPlaceHide()
{
    OutputDebugString(TEXT("In CSimpSvrObj::DoInPlaceHide\r\n"));

    // if we aren't inplace visible, then this routine is a NOP,
    if (!m_fInPlaceVisible)
        return;

    m_fInPlaceVisible = FALSE;

    // change the parenting
    SetParent(m_pDoc->GethHatchWnd(), m_pDoc->GethAppWnd());
    SetParent(m_pDoc->GethDocWnd(), m_pDoc->GethAppWnd());

    // rip down the combined menus
    DisassembleMenus();

    // release the inplace frame
    m_pFrame->Release();

    m_pFrame = NULL;  // only holding one ref. to frame.

    // release the UIWindow if it is there.
    if (m_pCntrDoc)
        m_pCntrDoc->Release();

    m_pCntrDoc = NULL;
}

//**********************************************************************
//
// CSimpSvrObj::DisassembleMenus
// 
// Purpose:
// 
//      Disassembles the combined menus used in inplace activation
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
//      OleDestroyMenuDescriptor        OLE API
//      RemoveMenu                      Windows API
//      IOleInPlaceFrame::RemoveMenus   Container
//      DestroyMenu                     Windows API
//
// Comments:
//
//      Be sure to read TECHNOTES.WRI included with the OLE SDK
//      for details on implementing inplace activation.
//
//********************************************************************

void CSimpSvrObj::DisassembleMenus()
{
    // destroy the menu descriptor
    OleDestroyMenuDescriptor(m_hOleMenu);

    if (m_hmenuShared)
    {
        // remove the menus that we added
        RemoveMenu(m_hmenuShared, 1, MF_BYPOSITION);

        // have the container remove its menus
        m_pFrame->RemoveMenus(m_hmenuShared);

        // Destroy the menu resource
        DestroyMenu(m_hmenuShared);

        m_hmenuShared = NULL;
    }
}

//**********************************************************************
//
// CSimpSvrObj::SendOnDataChange
// 
// Purpose:
// 
//      Uses the data advise holder to send a data change, then updates
//      the ROT to note the time of change.
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
//      IDataAdviseHolder::SendOnDataChange     OLE API
//      GetRunningObjectTable                   OLE API
//      CoFileTimeNow                           OLE API
//      IRunningObjectTable::NoteChangeTime     OLE API
//
// Comments:
//
//
//********************************************************************

void CSimpSvrObj::SendOnDataChange()
{
    if (m_pDataAdviseHolder)
        m_pDataAdviseHolder->SendOnDataChange((LPDATAOBJECT)this, 0, 0);

    LPRUNNINGOBJECTTABLE lpRot;

    GetRunningObjectTable(0, &lpRot);

    if (lpRot && m_dwRegister)
    {
        FILETIME ft;
        CoFileTimeNow(&ft);

        lpRot->NoteChangeTime(m_dwRegister, &ft);
        lpRot->Release();
    }
}
