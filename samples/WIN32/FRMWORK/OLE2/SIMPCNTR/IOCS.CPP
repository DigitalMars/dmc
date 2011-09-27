//**********************************************************************
// File name: IOCS.CPP
//
//      Implementation of IOleClientSite for CSimpleSite
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
// CSimpleSite::SaveObject
//
// Purpose:
//
//      Called by the object when it wants to be saved to persistant
//      storage
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
//      IOleObject::QueryInterface          Object
//      IPersistStorage::SaveCompleted      Object
//      IPersistStorage::Release            Object
//      OleSave                             OLE API
//
// Comments:
//
//********************************************************************

STDMETHODIMP CSimpleSite::SaveObject()
{
    LPPERSISTSTORAGE lpPS;

    OutputDebugString(TEXT("In IOCS::SaveObject\r\n"));

    // get a pointer to IPersistStorage
    HRESULT hr = m_lpOleObject->QueryInterface(IID_IPersistStorage,
                                               (LPVOID*)&lpPS);

    // save the object
    if (hr == NOERROR)
    {
        hr = OleSave(lpPS, m_lpObjStorage, TRUE);
        lpPS->SaveCompleted(NULL);
        lpPS->Release();
        return hr;
    }

    return E_FAIL;
}

//**********************************************************************
//
// CSimpleSite::GetMoniker
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
// Function Calls:
//      Function                    Location
//
//      OutputDebugString           Windows API
//
// Comments:
//
//      This function is not implemented because we don't support
//      linking.
//
//********************************************************************

STDMETHODIMP CSimpleSite::GetMoniker(DWORD dwAssign,
                                     DWORD dwWhichMoniker,
                                     LPMONIKER* ppmk)
{
    OutputDebugString(TEXT("In IOCS::GetMoniker\r\n"));

    // need to null the out pointer
    *ppmk = NULL;

    return E_NOTIMPL;
}

//**********************************************************************
//
// CSimpleSite::GetContainer
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

STDMETHODIMP CSimpleSite::GetContainer(LPOLECONTAINER* ppContainer)
{
    OutputDebugString(TEXT("In IOCS::GetContainer\r\n"));

    // NULL the out pointer
    *ppContainer = NULL;

    return E_NOTIMPL;
}

//**********************************************************************
//
// CSimpleSite::ShowObject
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
//      This function is not implemented because we don't support
//      linking.
//
//********************************************************************

STDMETHODIMP CSimpleSite::ShowObject()
{
    OutputDebugString(TEXT("In IOCS::ShowObject\r\n"));

    return NOERROR;
}

//**********************************************************************
//
// CSimpleSite::OnShowWindow
//
// Purpose:
//
//      Object calls this method when it is opening/closing non-InPlace
//      Window
//
// Parameters:
//
//      BOOL fShow  - TRUE if Window is opening, FALSE if closing
//
// Return Value:
//
//      S_OK
//
// Function Calls:
//      Function                    Location
//
//      OutputDebugString           Windows API
//      InvalidateRect              Windows API
//
// Comments:
//
//********************************************************************

STDMETHODIMP CSimpleSite::OnShowWindow(BOOL fShow)
{
    OutputDebugString(TEXT("In IOCS::OnShowWindow\r\n"));

    m_fObjectOpen = fShow;
    InvalidateRect(m_lpDoc->m_hDocWnd, NULL, TRUE);

    // if object window is closing, then bring container window to top
    if (!fShow)
    {
        BringWindowToTop(m_lpDoc->m_hDocWnd);
        SetFocus(m_lpDoc->m_hDocWnd);
    }
    return S_OK;
}

//**********************************************************************
//
// CSimpleSite::RequestNewObjectLayout
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

STDMETHODIMP CSimpleSite::RequestNewObjectLayout()
{
    OutputDebugString(TEXT("In IOCS::RequestNewObjectLayout\r\n"));

    return E_NOTIMPL;
}
