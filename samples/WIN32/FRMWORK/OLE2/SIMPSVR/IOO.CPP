//**********************************************************************
// File name: IOO.CPP
//
//    Implementation of IOleObject for CSimpSvrObj
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

#define VERB_OPEN 1
                       
//**********************************************************************
//
// CSimpSvrObj::SetClientSite
// 
// Purpose:
//                       
//      Called to notify the object of it's client site.
//  
// Parameters:
//                         
//      LPOLECLIENTSITE pClientSite     - ptr to new client site
// 
// Return Value:
//          
//      S_OK
// 
// Function Calls:
//      Function                    Location
//                             
//      OutputDebugString           Windows API
//      IOleClientSite::Release     Container
//      IOleClientSite::AddRef      Container
//
// Comments:
//
//
//********************************************************************

STDMETHODIMP CSimpSvrObj::SetClientSite(LPOLECLIENTSITE pClientSite)
{
    OutputDebugString(TEXT("In IOO::SetClientSite\r\n"));

    // if we already have a client site, release it.
    if (m_pOleClientSite)
    {
        m_pOleClientSite->Release();
        m_pOleClientSite = NULL;
    }

    // store copy of the client site.
    m_pOleClientSite = pClientSite;

    // AddRef it so it doesn't go away.
    if (m_pOleClientSite)
        m_pOleClientSite->AddRef();

    return S_OK;
}

//**********************************************************************
//
// CSimpSvrObj::Advise
// 
// Purpose:
//                       
//      Called to set up an advise on the OLE object.
//  
// Parameters:
//                         
//      LPADVISESINK pAdvSink       - ptr to the Advise Sink for notification
//
//      LPDWORD pdwConnection       - place to return the connection ID.
// 
// Return Value:
//          
//      Passed back from IOleAdviseHolder::Advise.
// 
// Function Calls:
//      Function                    Location
//                             
//      OutputDebugString           Windows API   
//      CreateOleAdviseHolder       OLE API
//      IOleAdviseHolder::Advise    OLE
//
// Comments:
//
//
//********************************************************************

STDMETHODIMP CSimpSvrObj::Advise(LPADVISESINK pAdvSink, LPDWORD pdwConnection)
{
    OutputDebugString(TEXT("In IOO::Advise\r\n"));

    // if we haven't made an OleAdviseHolder yet, make one.
    if (!m_pOleAdviseHolder)
        CreateOleAdviseHolder(&m_pOleAdviseHolder);

    // pass this call onto the OleAdviseHolder.
    return m_pOleAdviseHolder->Advise(pAdvSink, pdwConnection);
}

//**********************************************************************
//
// CSimpSvrObj::SetHostNames
// 
// Purpose:
//                       
//      Called to pass strings for Window titles.
//  
// Parameters:
//                         
//      LPCSTR szContainerApp   -   ptr to string describing Container App
//
//      LPCSTR szContainerObj   -   ptr to string describing Object
// 
// Return Value:
//          
//      S_OK
// 
// Function Calls:
//      Function                    Location
//                             
//      WideCharToMultiByte         Windows API
//      SetWindowText               Windows API
//
// Comments:
//
//      This routine is called so that the server application can
//      set the window title appropriately.
//
//********************************************************************

STDMETHODIMP CSimpSvrObj::SetHostNames(LPCOLESTR szContainerApp, LPCOLESTR szContainerObj)
{
    OutputDebugString(TEXT("In IOO::SetHostNames\r\n"));

    TCHAR szTitle[256] = TEXT("Simple OLE Server");

    if (NULL != szContainerObj)
    {
        lstrcat(szTitle, TEXT(" in "));

#ifdef UNICODE
        lstrcat(szTitle, szContainerObj);       // Both strings are unicode
#else
        WideCharToMultiByte(CP_ACP,             // Convert szContainerObj to ANSI
                            0,
                            szContainerObj,
                            -1,
                            szTitle + lstrlen(szTitle),
                            sizeof(szTitle) - lstrlen(szTitle),
                            NULL,
                            NULL);
#endif
    }

    SetWindowText(m_pDoc->GethAppWnd(), szTitle);

    return S_OK;
}

//**********************************************************************
//
// CSimpSvrObj::DoVerb
// 
// Purpose:
//                       
//      Called by the container application to invoke a verb.
//  
// Parameters:
//                         
//      LONG iVerb                  - The value of the verb to be
//                                    invoked.
//
//      LPMSG lpmsg                 - The message that caused the
//                                    verb to be invoked.
//
//      LPOLECLIENTSITE pActiveSite - Ptr to the active client site.
//
//      LONG lindex                 - Used in extended layout
//
//      HWND hwndParent             - This should be the window handle of
//                                    the window in which we are contained.
//                                    This value could be used to "fake"
//                                    inplace activation in a manner similar
//                                    to Video for Windows in OLE 1.0.
//
//      LPCRECT lprcPosRect         - The rectangle that contains the object
//                                    within hwndParent.  Also used to
//                                    "fake" inplace activation.
// 
// Return Value:
//          
//      OLE_E_NOTINPLACEACTIVE      - Returned if attempted to undo while not
//                                    inplace active.
//      S_OK
// 
// Function Calls:
//      Function                            Location
//                             
//      OutputDebugString                   Windows API
//      ShowWindow                          Windows API
//      CSimpSvrObj::DoInPlaceActivate      OBJ.CPP
//      CSimpSvrObj::DoInPlaceHide          OBJ.CPP
//      CSimpSvrObj::OpenEdit               IOO.CPP
//      CSimpSvrDoc::GethDocWnd             DOC.H
//      COleInPlaceObj::InPlaceDeactivate   IOIPO.CPP
//
// Comments:
//
//      Be sure to look at TECHNOTES.WRI included with the OLE
//      SDK for a description of handling the inplace verbs
//      properly.
//
//********************************************************************

STDMETHODIMP CSimpSvrObj::DoVerb(LONG iVerb,
                                 LPMSG lpmsg,
                                 LPOLECLIENTSITE pActiveSite,
                                 LONG lindex,
                                 HWND hwndParent,
                                 LPCRECT lprcPosRect)
{
    OutputDebugString(TEXT("In IOO::DoVerb\r\n"));

    switch (iVerb)
    {
        case OLEIVERB_SHOW:
        case OLEIVERB_PRIMARY:
            if (m_fOpen)
                SetFocus(m_pDoc->GethAppWnd());
            else if (DoInPlaceActivate(iVerb) == FALSE)
                OpenEdit(pActiveSite);
            break;

        case OLEIVERB_UIACTIVATE:
            if (m_fOpen)
                return E_FAIL;

            // inplace activate
            if (!DoInPlaceActivate(iVerb))
                return E_FAIL;
            break;

        case OLEIVERB_DISCARDUNDOSTATE:
            // don't have to worry about this situation as we don't
            // support an undo state.
            if (!m_fInPlaceActive)
                return OLE_E_NOT_INPLACEACTIVE;
            break;

        case OLEIVERB_HIDE:
            // if inplace active, do an "inplace" hide, otherwise
            // just hide the app window.
            if (m_fInPlaceActive)
            {
                UIDeactivate();
                DoInPlaceHide();
            }
            else
                m_pDoc->GetApp()->HideAppWnd();
            break;

        case OLEIVERB_OPEN:
        case VERB_OPEN:
            // if inplace active, deactivate
            if (m_fInPlaceActive)
                InPlaceDeactivate();

            // open into another window.
            OpenEdit(pActiveSite);
            break;

        default:
            if (iVerb < 0)
                return E_FAIL;
    }

    return S_OK;
}

//**********************************************************************
//
// CSimpSvrObj::GetExtent
// 
// Purpose:
//                       
//      Returns the extent of the object.
//  
// Parameters:
//                         
//      DWORD dwDrawAspect  - The aspect in which to get the size.
//
//      LPSIZEL lpsizel     - Out ptr to return the size.
// 
// Return Value:
//          
//
// 
// Function Calls:
//      Function                        Location
//                             
//      OutputDebugString               Windows API
//      XformWidthInPixelsToHimetric    OLE2UI
//      XformHeightInPixelsToHimetric   OLE2UI
//
// Comments:
//
//
//********************************************************************

STDMETHODIMP CSimpSvrObj::GetExtent(DWORD dwDrawAspect, LPSIZEL lpsizel)
{
    OutputDebugString(TEXT("In IOO::GetExtent\r\n"));

    SCODE sc = E_FAIL;

    // Only DVASPECT_CONTENT is supported....
    if (dwDrawAspect == DVASPECT_CONTENT)
    {
        sc = S_OK;

        // return the correct size in HIMETRIC...
        HDC hdc = GetDC(NULL);
        lpsizel->cx = MulDiv(m_ptSize.x, HIMETRIC_PER_INCH, GetDeviceCaps(hdc, LOGPIXELSX));
        lpsizel->cy = MulDiv(m_ptSize.y, HIMETRIC_PER_INCH, GetDeviceCaps(hdc, LOGPIXELSY));
        ReleaseDC(NULL, hdc);
    }

    return sc;
}

//**********************************************************************
//
// CSimpSvrObj::Update
// 
// Purpose:
//                       
//      Called to get the most up to date data
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
//      Function                            Location
//                             
//      OutputDebugString                   Windows API
//      IDataAdviseHolder::SendOnDataChange OLE
//
// Comments:
//
//
//********************************************************************

STDMETHODIMP CSimpSvrObj::Update()
{
    OutputDebugString(TEXT("In IOO::Update\r\n"));

    // force an update
    SendOnDataChange();

    return S_OK;
}

//**********************************************************************
//
// CSimpSvrObj::Close
// 
// Purpose:
//                       
//      Called when the OLE object needs to be closed
//  
// Parameters:
//                         
//      DWORD dwSaveOption  - Flags to instruct the server how to prompt
//                            the user.
// 
// Return Value:
//          
//      S_OK
// 
// Function Calls:
//      Function                    Location
//                             
//      OutputDebugString           Windows API
//      CSimpSvrDoc::Close          DOC.CPP
//
// Comments:
//
//
//********************************************************************

STDMETHODIMP CSimpSvrObj::Close(DWORD dwSaveOption)
{
    OutputDebugString(TEXT("In IOO::Close\r\n"));

    // delegate to the document object.
    m_pDoc->Close();

    return S_OK;
}

//**********************************************************************
//
// CSimpSvrObj::Unadvise
// 
// Purpose:
//                       
//      Breaks down an OLE advise that has been set up on this object.
//  
// Parameters:
//                         
//      DWORD dwConnection  - Connection that needs to be broken down       
// 
// Return Value:
//          
//      Passed back from IOleAdviseHolder::Unadvise
// 
// Function Calls:
//      Function                    Location
//                             
//      OutputDebugString           Windows API    
//      IOleAdviseHolder::Unadvise  OLE
//
// Comments:
//
//
//********************************************************************

STDMETHODIMP CSimpSvrObj::Unadvise(DWORD dwConnection)
{
    OutputDebugString(TEXT("In IOO::Unadvise\r\n"));

    // pass on to OleAdviseHolder.
    return m_pOleAdviseHolder->Unadvise(dwConnection);
}

//**********************************************************************
//
// CSimpSvrObj::EnumVerbs
// 
// Purpose:
//                       
//      Enumerates the verbs associated with this object.
//  
// Parameters:
//                         
//      LPENUMOLEVERB* ppenumOleVerb        - Out ptr in which to return
//                                            the enumerator      
// 
// Return Value:
//          
//      OLE_S_USEREG    - Instructs OLE to use the verbs found in the 
//                        REG DB for this server.
// 
// Function Calls:
//      Function                    Location
//                             
//      OutputDebugString           Windows API
//
// Comments:
//
//      In a .DLL, an application cannot return OLE_S_USEREG.  This is
//      due to the fact that the default object handler is not being
//      used, and the container is really making direct function calls
//      into the server .DLL.
//
//********************************************************************

STDMETHODIMP CSimpSvrObj::EnumVerbs(LPENUMOLEVERB* ppenumOleVerb)
{
    OutputDebugString(TEXT("In IOO::EnumVerbs\r\n"));

    return OLE_S_USEREG;
}

//**********************************************************************
//
// CSimpSvrObj::GetClientSite
// 
// Purpose:
//                       
//      Called to get the current client site of the object.
//  
// Parameters:
//                         
//      LPOLECLIENTSITE* ppClientSite       - Out ptr in which to return the
//                                            client site.
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
//
//********************************************************************

STDMETHODIMP CSimpSvrObj::GetClientSite(LPOLECLIENTSITE* ppClientSite)
{
    OutputDebugString(TEXT("In IOO::GetClientSite\r\n"));

    *ppClientSite = m_pOleClientSite;

    return S_OK;
}

//**********************************************************************
//
// CSimpSvrObj::SetMoniker
// 
// Purpose:
//                       
//      Used to set the objects moniker
//  
// Parameters:
//                         
//      DWORD dwWhichMoniker    - Type of moniker being set
//
//      LPMONIKER pmk           - Pointer to the moniker
// 
// Return Value:
//          
// 
// Function Calls:
//      Function                    Location
//                             
//      OutputDebugString           Windows API
//
// Comments:
//
//
//********************************************************************

STDMETHODIMP CSimpSvrObj::SetMoniker(DWORD dwWhichMoniker, LPMONIKER pmk)
{
    OutputDebugString(TEXT("In IOO::SetMoniker\r\n"));

    LPMONIKER lpmk;

    if (!GetOleClientSite())
        return E_FAIL;

    if (GetOleClientSite()->GetMoniker(OLEGETMONIKER_ONLYIFTHERE,
                                       OLEWHICHMK_OBJFULL,
                                       &lpmk) != NOERROR)
        return E_FAIL;

    if (GetOleAdviseHolder())
        GetOleAdviseHolder()->SendOnRename(lpmk);

    LPRUNNINGOBJECTTABLE lpRot;

    if (GetRunningObjectTable(0, &lpRot) == NOERROR)
    {
        if (m_dwRegister)
            lpRot->Revoke(m_dwRegister);

        lpRot->Register(0, (LPOLEOBJECT)this, lpmk, &m_dwRegister);
        lpRot->Release();
    }

    return S_OK;
}

//**********************************************************************
//
// CSimpSvrObj::GetMoniker
// 
// Purpose:
//                       
////  
// Parameters:
//                         
//      DWORD dwAssign          - Assignment for the moniker 
//
//      DWORD dwWhichMoniker    - Which moniker to return
//
//      LPMONIKER* ppmk         - An out ptr to return the moniker
// 
// Return Value:
//          
// 
// Function Calls:
//      Function                    Location
//                             
//      OutputDebugString           Windows API
//
// Comments:
//
//
//********************************************************************

STDMETHODIMP CSimpSvrObj::GetMoniker(DWORD dwAssign,
                                     DWORD dwWhichMoniker,
                                     LPMONIKER* ppmk)
{
    OutputDebugString(TEXT("In IOO::GetMoniker\r\n"));

    // need to NULL the out parameter
    *ppmk = NULL;

    return GetOleClientSite()->GetMoniker(OLEGETMONIKER_ONLYIFTHERE,
                                          OLEWHICHMK_OBJFULL,
                                          ppmk);
}

//**********************************************************************
//
// CSimpSvrObj::InitFromData
// 
// Purpose:
//                       
//      Initialize the object from the passed pDataObject.
//  
// Parameters:
//                         
//      LPDATAOBJECT pDataObject    - Pointer to data transfer object
//                                    to be used in the initialization
//
//      BOOL fCreation              - TRUE if the object is currently being
//                                    created.
//
//      DWORD dwReserved            - Reserved
// 
// Return Value:
//          
//      S_FALSE     
// 
// Function Calls:
//      Function                    Location
//                             
//      OutputDebugString           Windows API
//
// Comments:
//                  
//      We don't support this functionality, so we will always return
//      error.
//
//********************************************************************

STDMETHODIMP CSimpSvrObj::InitFromData(LPDATAOBJECT pDataObject,
                                       BOOL fCreation,
                                       DWORD dwReserved)
{
    OutputDebugString(TEXT("In IOO::InitFromData\r\n"));

    return S_FALSE;
}

//**********************************************************************
//
// CSimpSvrObj::GetClipboardData
// 
// Purpose:
//                       
//      Returns an IDataObject that is the same as doing an OleSetClipboard
//  
// Parameters:
//                         
//      DWORD dwReserved                - Reserved
//
//      LPDATAOBJECT* ppDataObject      - Out ptr for the Data Object.
// 
// Return Value:
//          
//      OLE_E_NOTSUPPORTED
// 
// Function Calls:
//      Function                    Location
//                             
//      OutputDebugString           Windows API
//
// Comments:
//                                             
//      Support of this method is optional.
//
//********************************************************************

STDMETHODIMP CSimpSvrObj::GetClipboardData(DWORD dwReserved,
                                           LPDATAOBJECT* ppDataObject)
{
    OutputDebugString(TEXT("In IOO::GetClipboardData\r\n"));

    // NULL the out ptr
    *ppDataObject = NULL;

    return E_NOTIMPL;
}

//**********************************************************************
//
// CSimpSvrObj::IsUpToDate
// 
// Purpose:
//                       
//      Determines if an object is up to date
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
//      Our embedded object is always up to date.  This function is
//      particularly useful in linking situations.
//
//********************************************************************

STDMETHODIMP CSimpSvrObj::IsUpToDate()
{
    OutputDebugString(TEXT("In IOO::IsUpToDate\r\n"));

    return S_OK;
}

//**********************************************************************
//
// CSimpSvrObj::GetUserClassID
// 
// Purpose:
//                       
//      Returns the applications CLSID
//  
// Parameters:
//                         
//      LPCLSID pClsid      - Out ptr to return the CLSID
// 
// Return Value:
//          
//      S_OK
// 
// Function Calls:
//      Function                    Location
//                             
//      OutputDebugString           Windows API
//      CPersistStorage::GetClassID IPS.CPP
//
// Comments:
//
//      This function is just delegated to IPS::GetClassID.
//
//********************************************************************

STDMETHODIMP CSimpSvrObj::GetUserClassID(LPCLSID pClsid)
{
    OutputDebugString(TEXT("In IOO::GetUserClassID\r\n"));

    GetClassID(pClsid);

    return S_OK;
}

//**********************************************************************
//
// CSimpSvrObj::GetUserType
// 
// Purpose:
//                       
//      Used to get a user presentable id for this object
//  
// Parameters:
//                         
//      DWORD dwFormOfType      - The ID requested
//
//      LPOLESTR* pszUserType   - Out ptr to return the string
// 
// Return Value:
//          
//      OLE_S_USEREG    - Use the reg db to get these entries.
// 
// Function Calls:
//      Function                    Location
//                             
//      OutputDebugString           Windows API
//
// Comments:
//
//
//********************************************************************

STDMETHODIMP CSimpSvrObj::GetUserType(DWORD dwFormOfType, LPOLESTR* pszUserType)
{
    OutputDebugString(TEXT("In IOO::GetUserType\r\n"));

    // NULL the out pointer
    *pszUserType = NULL;

    // Delegate to default handler
    return OLE_S_USEREG;
}

//**********************************************************************
//
// CSimpSvrObj::SetExtent
// 
// Purpose:
//                       
//      Called to set the extent of the object.
//  
// Parameters:
//                         
//      DWORD dwDrawAspect  - Aspect to have its size set
//
//      LPSIZEL lpsizel     - New size of the object.
// 
// Return Value:
//          
//      E_NOTIMPL   - This function is not curently implemented.
// 
// Function Calls:
//      Function                    Location
//                             
//      OutputDebugString           Windows API
//
// Comments:
//
//
//********************************************************************

STDMETHODIMP CSimpSvrObj::SetExtent(DWORD dwDrawAspect, LPSIZEL lpsizel)
{
    OutputDebugString(TEXT("In IOO::SetExtent\r\n"));

    return E_NOTIMPL;
}

//**********************************************************************
//
// CSimpSvrObj::EnumAdvise
// 
// Purpose:
//                       
//      Returns an enumerate which enumerates the outstanding advises
//      associated with this OLE object.
//  
// Parameters:
//                         
//      LPENUMSTATDATA* ppenumAdvise    - Out ptr in which to return
//                                         the enumerator.
// 
// Return Value:
//          
//      Passed on from IOleAdviseHolder::EnumAdvise.
// 
// Function Calls:
//      Function                        Location
//                             
//      OutputDebugString               Windows API     
//      IOleAdviseHolder::EnumAdvise    OLE
//
// Comments:
//
//
//********************************************************************

STDMETHODIMP CSimpSvrObj::EnumAdvise(LPENUMSTATDATA* ppenumAdvise)
{
    OutputDebugString(TEXT("In IOO::EnumAdvise\r\n"));

    // need to NULL the out parameter
    *ppenumAdvise = NULL;

    // pass on to the OLE Advise holder.
    return m_pOleAdviseHolder->EnumAdvise(ppenumAdvise);
}

//**********************************************************************
//
// CSimpSvrObj::GetMiscStatus
// 
// Purpose:
//                       
//      Return status information about the object
//  
// Parameters:
//                         
//      DWORD dwAspect          - Aspect interested in.
//
//      LPDWORD pdwStatus       - Out ptr in which to return the bits.
// 
// Return Value:
//          
//      CO_E_READREGDB  - Tell the library to use the reg DB.
// 
// Function Calls:
//      Function                    Location
//                             
//      OutputDebugString           Windows API
//
// Comments:
//
//
//********************************************************************

STDMETHODIMP CSimpSvrObj::GetMiscStatus(DWORD dwAspect, LPDWORD pdwStatus)
{
    OutputDebugString(TEXT("In IOO::GetMiscStatus\r\n"));

    // need to NULL the out parameter
    *pdwStatus = NULL;

    return OLE_S_USEREG;
}

//**********************************************************************
//
// CSimpSvrObj::SetColorScheme
// 
// Purpose:
//                       
//      Used to set the palette for the object to use.
//  
// Parameters:
//                         
//      LPLOGPALETTE lpLogpal   - Pointer to the LOGPALETTE to be used.
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
//      This server ignores this method.
//
//********************************************************************

STDMETHODIMP CSimpSvrObj::SetColorScheme(LPLOGPALETTE lpLogpal)
{
    OutputDebugString(TEXT("In IOO::SetColorScheme\r\n"));

    return S_OK;
}

//**********************************************************************
//
// CSimpSvrObj::OpenEdit
// 
// Purpose:
//                       
//      Used to Open the object into a seperate window.
//  
// Parameters:
//                         
//      LPOLECLIENTSITE pActiveSite - Pointer to the Active clientsite.
// 
// Return Value:
//          
//      None.
// 
// Function Calls:
//      Function                        Location
//                             
//      IOleClientSite::OnShowWindow    Container
//      ShowWindow                      Windows API
//      UpdateWindow                    Windows API
//      OutputDebugString               Windows API
//      CSimpSvrDoc::GethAppWnd         DOC.H
//      CSimpSvrDoc::GethHatchWnd       DOC.H
//
// Comments:
//
//
//********************************************************************

void CSimpSvrObj::OpenEdit(LPOLECLIENTSITE pActiveSite)
{
    if (GetOleClientSite())
        GetOleClientSite()->ShowObject();

    m_fOpen = TRUE;

    // tell the site we are opening so the object can be hatched out.
    if (GetOleClientSite())
        GetOleClientSite()->OnShowWindow(TRUE);

    m_pDoc->ShowDocWnd();
    SetForegroundWindow(m_pDoc->GethDocWnd());  // Bring window to foreground
    m_pDoc->HideHatchWnd();

    // Show app window.
    m_pDoc->GetApp()->ShowAppWnd();
    SetFocus(m_pDoc->GethAppWnd());
}
