//**********************************************************************
// File name: SITE.CPP
//
//      Implementation file for CSimpleSite
//
// Functions:
//
//      See SITE.H for class definition
//
// Copyright (c) 1992-1995 Microsoft Corporation. All rights reserved.
//**********************************************************************

#include "precomp.h"
#include "app.h"
#include "site.h"
#include "doc.h"

//**********************************************************************
//
// CSimpleSite::Create
//
// Purpose:
//
//      Creation routine for CSimpleSite
//
// Parameters:
//
//      CSimpleDoc *lpDoc       - Pointer to CSimpleDoc
//
// Return Value:
//
//      None
//
// Function Calls:
//      Function                    Location
//
//      IStorage::CreateStorage     OLE API
//      assert                      C Runtime
//
// Comments:
//
//********************************************************************

CSimpleSite* CSimpleSite::Create(CSimpleDoc *lpDoc)
{
    CSimpleSite *lpTemp = new CSimpleSite(lpDoc);

    if (!lpTemp)
        return NULL;

    // create a sub-storage for the object
    HRESULT hErr = lpDoc->m_lpStorage->CreateStorage(OLESTR("Object"),
                STGM_READWRITE | STGM_TRANSACTED | STGM_SHARE_EXCLUSIVE,
                0,
                0,
                &lpTemp->m_lpObjStorage);

    assert(hErr == NOERROR);

    if (hErr != NOERROR)
    {
        delete lpTemp;
        return NULL;
    }

    // we will add one ref count on our Site. later when we want to destroy
    // the Site object we will release this  ref count. when the Site's ref
    // count goes to 0, it will be deleted.
    lpTemp->AddRef();

    return lpTemp;
}

//**********************************************************************
//
// CSimpleSite::CSimpleSite
//
// Purpose:
//
//      Constructor for CSimpleSite
//
// Parameters:
//
//      CSimpleDoc *lpDoc       - Pointer to CSimpleDoc
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

CSimpleSite::CSimpleSite(CSimpleDoc *lpDoc)
{
    // remember the pointer to the doc
    m_lpDoc = lpDoc;

    // clear the reference count
    m_nCount = 0;

    m_dwDrawAspect      = DVASPECT_CONTENT;
    m_lpOleObject       = NULL;
    m_lpInPlaceObject   = NULL;
    m_hwndIPObj         = NULL;
    m_fInPlaceActive    = FALSE;
    m_fObjectOpen       = FALSE;
}

//**********************************************************************
//
// CSimpleSite::~CSimpleSite
//
// Purpose:
//
//      Destructor for CSimpleSite
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
//      IOleObject::Release                     Object
//      IStorage::Release                       OLE API
//
// Comments:
//
//********************************************************************

CSimpleSite::~CSimpleSite()
{
    OutputDebugString(TEXT("In CSimpleSite's Destructor \r\n"));

    if (m_lpOleObject)
       m_lpOleObject->Release();

    if (m_lpObjStorage)
       m_lpObjStorage->Release();
}


//**********************************************************************
//
// CSimpleSite::QueryInterface
//
// Purpose:
//
//      Used for interface negotiation of the container Site.
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
//      S_OK    -   The interface is supported.
//      S_FALSE -   The interface is not supported
//
// Function Calls:
//      Function                    Location
//
//      OutputDebugString           Windows API
//      IsEqualIID                  OLE API
//      CSimpleSite::AddRef         SITE.CPP
//
// Comments:
//
//
//********************************************************************

STDMETHODIMP CSimpleSite::QueryInterface(REFIID riid, LPVOID* ppv)
{
    OutputDebugString(TEXT("In CSimpleSite::QueryInterface\r\n"));

    *ppv = NULL;

    if (IsEqualIID(riid, IID_IUnknown) || IsEqualIID(riid, IID_IOleClientSite))
        *ppv = GetOleClientSite();

    else if (IsEqualIID(riid, IID_IAdviseSink))
        *ppv = GetAdviseSink();

    else if (IsEqualIID(riid, IID_IOleInPlaceSite))
        *ppv = GetOleInPlaceSite();

    if (NULL != *ppv)
    {
        ((LPUNKNOWN)*ppv)->AddRef();
        return S_OK;
    }

    return E_NOINTERFACE;
}

//**********************************************************************
//
// CSimpleSite::AddRef
//
// Purpose:
//
//      Increments the reference count of the container Site.
//
// Parameters:
//
//      None
//
// Return Value:
//
//      ULONG   -   The new reference count of the site.
//
// Function Calls:
//      Function                    Location
//
//      OutputDebugString           Windows API
//
// Comments:
//
//********************************************************************

STDMETHODIMP_(ULONG) CSimpleSite::AddRef()
{
    OutputDebugString(TEXT("In CSimpleSite::AddRef\r\n"));

    return ++m_nCount;
}

//**********************************************************************
//
// CSimpleSite::Release
//
// Purpose:
//
//      Decrements the reference count of the container Site
//
// Parameters:
//
//      None
//
// Return Value:
//
//      ULONG   -   The new reference count of the Site.
//
// Function Calls:
//      Function                    Location
//
//      OutputDebugString           Windows API
//
// Comments:
//
//********************************************************************

STDMETHODIMP_(ULONG) CSimpleSite::Release()
{
    OutputDebugString(TEXT("In CSimpleSite::Release\r\n"));

    int nTmp = --m_nCount;

    if (0 == nTmp)
        delete this;

    return nTmp;
}

//**********************************************************************
//
// CSimpleSite::InitObject
//
// Purpose:
//
//      Used to initialize a newly create object (can't be done in the
//      constructor).
//
// Parameters:
//
//      BOOL fCreateNew -   TRUE if insert NEW object
//                          FALSE if create object FROM FILE
//
// Return Value:
//
//      None
//
// Function Calls:
//      Function                        Location
//
//      IOleObject::SetHostNames        Object
//      IOleObject::QueryInterface      Object
//      IViewObject2::GetExtent         Object
//      IOleObject::DoVerb              Object
//      IViewObject::SetAdvise          Object
//      IViewObject::Release            Object
//      GetClientRect                   Windows API
//      OleSetContainedObject           OLE API
//
// Comments:
//
//********************************************************************

void CSimpleSite::InitObject(BOOL fCreateNew)
{
    LPVIEWOBJECT2 lpViewObject2;
    RECT rect;

    // Set a View Advise
    m_lpOleObject->QueryInterface(IID_IViewObject2, (LPVOID*)&lpViewObject2);
    lpViewObject2->SetAdvise(m_dwDrawAspect, ADVF_PRIMEFIRST, GetAdviseSink());

    // get the initial size of the object
    lpViewObject2->GetExtent(m_dwDrawAspect, -1 /*lindex*/, NULL /*ptd*/, &m_sizel);
    GetObjRect(&rect);  // get the rectangle of the object in pixels
    lpViewObject2->Release();

    // give the object the name of the container app/document
    m_lpOleObject->SetHostNames(OLESTR("Simple Application"),
                                OLESTR("Simple OLE In-Place Container"));

    // inform object handler/DLL object that it is used in the embedding container's context
    OleSetContainedObject(m_lpOleObject, TRUE);

    if (fCreateNew)
    {
       // force new object to save to guarantee valid object in our storage.
       // OLE 1.0 objects may close w/o saving. This is NOT necessary if the
       // object is created FROM FILE; its data in storage is already valid.
       SaveObject();

       // we only want to DoVerb(SHOW) if this is an InsertNew object.
       // we should NOT DoVerb(SHOW) if the object is created FromFile.
       m_lpOleObject->DoVerb(OLEIVERB_SHOW,
                             NULL,
                             GetOleClientSite(),
                             -1,
                             m_lpDoc->m_hDocWnd,
                             &rect);
    }
}

//**********************************************************************
//
// CSimpleSite::CloseOleObject
//
// Purpose:
//
//      Call IOleObject::Close on the object of the CSimpleSite
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
//      IOleObject::QueryInterface              Object
//      IOleObject::Close                       Object
//      IOleInPlaceObject::UIDeactivate         Object
//      IOleInPlaceObject::InPlaceDeactivate    Object
//      IOleInPlaceObject::Release              Object
//
// Comments:
//
//********************************************************************

void CSimpleSite::CloseOleObject()
{
    LPOLEINPLACEOBJECT lpObject;
    LPVIEWOBJECT lpViewObject = NULL;

    OutputDebugString(TEXT("In CSimpleSite::CloseOleObject \r\n"));

    if (m_lpOleObject)
    {
        if (m_fInPlaceActive)
        {
            m_lpOleObject->QueryInterface(IID_IOleInPlaceObject, (LPVOID*)&lpObject);
            lpObject->UIDeactivate();
            // don't need to worry about inside-out because the object
            // is going away.
            lpObject->InPlaceDeactivate();
            lpObject->Release();
        }

        m_lpOleObject->Close(OLECLOSE_NOSAVE);
    }
}


//**********************************************************************
//
// CSimpleSite::UnloadOleObject
//
// Purpose:
//
//      Close and release all pointers to the object of the CSimpleSite
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
//      CSimpleSite::CloseOleObject             SITE.CPP
//      IOleObject::QueryInterface              Object
//      IViewObject::SetAdvise                  Object
//      IViewObject::Release                    Object
//      IStorage::Release                       OLE API
//
// Comments:
//
//********************************************************************

void CSimpleSite::UnloadOleObject (void)
{
    OutputDebugString(TEXT("In CSimpleSite::UnloadOleObject \r\n"));

    if (m_lpOleObject)
    {
        LPVIEWOBJECT lpViewObject;
        CloseOleObject();    // ensure object is closed; NOP if already closed

        m_lpOleObject->QueryInterface(IID_IViewObject, (LPVOID*)&lpViewObject);

        if (lpViewObject)
        {
            // Remove the view advise
            lpViewObject->SetAdvise(m_dwDrawAspect, 0, NULL);
            lpViewObject->Release();
        }

        m_lpOleObject->Release();
        m_lpOleObject = NULL;
    }
}


//**********************************************************************
//
// CSimpleSite::PaintObj
//
// Purpose:
//
//      Paints the object
//
// Parameters:
//
//      HDC hDC     - Device context of the document window
//
// Return Value:
//
// Function Calls:
//      Function                        Location
//
//      IOleObject::QueryInterface      Object
//      IViewObject::GetColorSet        Object
//      IViewObject::Release            Object
//      SetMapMode                      Windows API
//      LPtoDP                          Windows API
//      CreateHatchBrush                Windows API
//      SelectObject                    Windows API
//      DeleteObject                    Windows API
//      CreatePalette                   Windows API
//      SelectPalette                   Windows API
//      RealizePalette                  Windows API
//      OleDraw                         OLE API
//
// Comments:
//
//********************************************************************

void CSimpleSite::PaintObj(HDC hDC)
{
    RECT rect;

    // need to check to make sure there is a valid object
    // available.  This is needed if there is a paint msg
    // between the time that CSimpleSite is instantiated
    // and OleUIInsertObject returns.
    if (!m_lpOleObject)
        return;

    // convert it to pixels
    GetObjRect(&rect);

    LPLOGPALETTE pColorSet = NULL;
    LPVIEWOBJECT lpView = NULL;

    // get a pointer to IViewObject
    m_lpOleObject->QueryInterface(IID_IViewObject, (LPVOID*)&lpView);

    // if the QI succeeds, get the LOGPALETTE for the object
    if (lpView)
        lpView->GetColorSet(m_dwDrawAspect, -1, NULL, NULL, NULL, &pColorSet);

    HPALETTE hPal = NULL;
    HPALETTE hPalT = NULL;

    // if a LOGPALETTE was returned (not guarateed), create the palette and
    // realize it.  NOTE: A smarter application would want to get the LOGPALETTE
    // for each of its visible objects, and try to create a palette that
    // satisfies all of the visible objects.
    if ((pColorSet))
    {
        hPal = CreatePalette(pColorSet);

        LPMALLOC pMalloc;
        if (CoGetMalloc(MEMCTX_TASK, &pMalloc) == NOERROR && pMalloc != NULL)
        {
            pMalloc->Free(pColorSet);
            pMalloc->Release();
        }
    }
    else
        hPal = m_lpDoc->m_lpApp->m_hStdPal;

    if (hPal)
    {
        hPalT = SelectPalette(hDC, hPal, TRUE);
        RealizePalette(hDC);
    }

    // draw the object
    OleDraw(m_lpOleObject, m_dwDrawAspect, hDC, &rect);

    // if the object is open, draw a hatch rect.
    if (m_fObjectOpen)
    {
        HBRUSH hBrush = CreateHatchBrush(HS_BDIAGONAL, RGB(0,0,0) );
        HBRUSH hOldBrush = (HBRUSH)SelectObject(hDC, hBrush);
        SetROP2(hDC, R2_MASKPEN);
        Rectangle(hDC, rect.left, rect.top, rect.right, rect.bottom);
        SelectObject(hDC, hOldBrush);
        DeleteObject(hBrush);
    }

    // if we created a palette, restore the old one, and destroy
    // the object.
    if (hPalT)
        SelectPalette(hDC, hPalT, TRUE);

    if (hPal && hPal != m_lpDoc->m_lpApp->m_hStdPal)
        DeleteObject(hPal);

    // if a view pointer was successfully returned, it needs to be released.
    if (lpView)
        lpView->Release();
}

//**********************************************************************
//
// CSimpleSite::GetObjRect
//
// Purpose:
//
//      Retrieves the rect of the object in pixels
//
// Parameters:
//
//      LPRECT lpRect - Rect structure filled with object's rect in pixels
//
// Return Value:
//
// Function Calls:
//      Function                        Location
//
//
// Comments:
//
//********************************************************************

#define HIMETRIC_PER_INCH 2540

void CSimpleSite::GetObjRect(LPRECT lpRect)
{
    lpRect->left = lpRect->top = 0;

    // Convert from HIMETRIC to pixels
    HDC hdc = GetDC(NULL);
    lpRect->right  = MulDiv(m_sizel.cx, GetDeviceCaps(hdc, LOGPIXELSX), HIMETRIC_PER_INCH);
    lpRect->bottom = MulDiv(m_sizel.cy, GetDeviceCaps(hdc, LOGPIXELSY), HIMETRIC_PER_INCH);
    ReleaseDC(NULL, hdc);

    // Arbitrarily offset the object into the client area so that there is
    // a visible left/top margin.
    OffsetRect(lpRect, 10, 10);
}
