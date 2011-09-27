//**********************************************************************
// File name: IAS.CPP
//
//      Implementation file of the IAdviseSink interface for CSimpleSite
//
//
// Functions:
//
//      See SITE.H for Class Definition
//
// Copyright (c) 1992-1995 Microsoft Corporation. All rights reserved.
//**********************************************************************

#include "precomp.h"
#include "app.h"            
#include "site.h"
#include "doc.h"                         

//**********************************************************************
//
// CSimpleSite::OnDataChange
// 
// Purpose:
//
//      Not Implemented (needs to be stubbed out)
//  
// Parameters:
//
//      Not Implemented (needs to be stubbed out)
//  
// Return Value:
//
//      Not Implemented (needs to be stubbed out)
//  
// Function Calls:
//      Function                    Location
//
//      OutputDebugString           Windows API
//
// Comments:
//
//      Not Implemented (needs to be stubbed out)
//  
//********************************************************************

STDMETHODIMP_(void) CSimpleSite::OnDataChange(LPFORMATETC pFormatetc, LPSTGMEDIUM pStgmed)
{
    OutputDebugString(TEXT("In IAS::OnDataChange\r\n"));
}

//**********************************************************************
//
// CSimpleSite::OnViewChange 
// 
// Purpose:
//                             
//      Notifies us that the view has changed and needs to be updated.
//  
// Parameters:
//                           
//      DWORD dwAspect  - Aspect that has changed
//
//      LONG lindex     - Index that has changed
// 
// Return Value:                                
//
//      None
//       
// Function Calls:
//      Function                    Location
//  
//      OutputDebugString           Windows API
//      InvalidateRect              Windows API 
//      IViewObject2::GetExtent     Object
//
// Comments:
//
//********************************************************************

STDMETHODIMP_(void) CSimpleSite::OnViewChange(DWORD dwAspect, LONG lindex)
{
    LPVIEWOBJECT2 lpViewObject2;
    OutputDebugString(TEXT("In IAS::OnViewChange\r\n"));

    // get a pointer to IViewObject2
    HRESULT hErr = m_lpOleObject->QueryInterface(IID_IViewObject2,
                                                 (LPVOID*)&lpViewObject2);

    if (hErr == NOERROR)
    {
        // get extent of the object
        // NOTE: this method will never be remoted; it can be called w/i this async method
        lpViewObject2->GetExtent(DVASPECT_CONTENT, -1 , NULL, &m_sizel);
        lpViewObject2->Release(); 
    }
    
    InvalidateRect(m_lpDoc->m_hDocWnd, NULL, TRUE);
}

//**********************************************************************
//
// CSimpleSite::OnRename
// 
// Purpose:
//
//      Not Implemented (needs to be stubbed out)
//  
// Parameters:
//
//      Not Implemented (needs to be stubbed out)
//  
// Return Value:
//
//      Not Implemented (needs to be stubbed out)
//  
// Function Calls:
//      Function                    Location
//
//      OutputDebugString           Windows API
//
// Comments:
//
//      Not Implemented (needs to be stubbed out)
//  
//********************************************************************

STDMETHODIMP_(void) CSimpleSite::OnRename(LPMONIKER pmk)
{
    OutputDebugString(TEXT("In IAS::OnRename\r\n"));
}

//**********************************************************************
//
// CSimpleSite::OnSave
// 
// Purpose:
//
//      Not Implemented (needs to be stubbed out)
//  
// Parameters:
//
//      Not Implemented (needs to be stubbed out)
//  
// Return Value:
//
//      Not Implemented (needs to be stubbed out)
//  
// Function Calls:
//      Function                    Location
//
//      OutputDebugString           Windows API
//
// Comments:
//
//      Not Implemented (needs to be stubbed out)
//  
//********************************************************************

STDMETHODIMP_(void) CSimpleSite::OnSave()
{
    OutputDebugString(TEXT("In IAS::OnSave\r\n"));
}

//**********************************************************************
//
// CSimpleSite::OnClose
// 
// Purpose:
//
//      Not Implemented (needs to be stubbed out)
//  
// Parameters:
//
//      Not Implemented (needs to be stubbed out)
//  
// Return Value:
//
//      Not Implemented (needs to be stubbed out)
//  
// Function Calls:
//      Function                    Location
//
//      OutputDebugString           Windows API
//
// Comments:
//
//      Not Implemented (needs to be stubbed out)
//  
//********************************************************************

STDMETHODIMP_(void) CSimpleSite::OnClose()      
{
    OutputDebugString(TEXT("In IAS::OnClose\r\n"));
}
