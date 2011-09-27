//**********************************************************************
// File name: ICF.CPP
//
//    Implementation file for the CClassFactory Class
//
// Functions:
//
//    See icf.h for a list of member functions.
//
// Copyright (c) 1993 - 1995 Microsoft Corporation. All rights reserved.
//**********************************************************************

#include "precomp.h"
#include "app.h"            
#include "doc.h"   
#include "icf.h"

//**********************************************************************
//
// CClassFactory::QueryInterface
// 
// Purpose:
// 
//  
// Parameters:
//                              
//      REFIID riid         -   Interface being queried for.
//
//      LPVOID* ppvObj      -   Out pointer for the interface.  
// 
// Return Value:
//                              
//      S_OK            - Success
//      E_NOINTERFACE   - Failure
// 
// Function Calls:
//      Function                    Location
//                               
//      CSimpSvrApp::QueryInterface APP.CPP
//
// Comments:
//
//
//********************************************************************

STDMETHODIMP CClassFactory::QueryInterface (REFIID riid, LPVOID* ppv)
{   
    OutputDebugString(TEXT("In CClassFactory::QueryInterface\r\n"));

    *ppv = NULL;

    if (IsEqualIID(riid, IID_IUnknown) ||
        IsEqualIID(riid, IID_IClassFactory))
        *ppv = (LPCLASSFACTORY)this;

    if (*ppv)
    {
        ((LPUNKNOWN)*ppv)->AddRef();
        return S_OK;
    }

    return E_NOINTERFACE;
}

//**********************************************************************
//
// CClassFactory::AddRef
// 
// Purpose:
//                      
//      Increments the reference count on CClassFactory and the application
//      object.
//  
// Parameters:
//             
//      None                   
// 
// Return Value:
//          
//      The Reference count on CClassFactory    
// 
// Function Calls:
//      Function                    Location
//                                          
//      OuputDebugString            Windows API
//
// Comments:
//
//
//********************************************************************


STDMETHODIMP_(ULONG) CClassFactory::AddRef()
{ 
    OutputDebugString(TEXT("In CClassFactory::AddRef\r\n"));

    return ++m_cRef; 
}

//**********************************************************************
//
// CClassFactory::Release
// 
// Purpose:
//                       
//      Decrements the reference count of CClassFactory and the 
//      application object.
//  
// Parameters:
//                         
//      None       
// 
// Return Value:
//          
//      The new reference count    
// 
// Function Calls:
//      Function                    Location
//                             
// Comments:
//
//
//********************************************************************

STDMETHODIMP_(ULONG) CClassFactory::Release()
{
    OutputDebugString(TEXT("In CClassFactory::Release\r\n"));

    int nTmp = --m_cRef;

    if (0 == nTmp)
        delete this;

    return nTmp;
}


//**********************************************************************
//
// CClassFactory::CreateInstance
// 
// Purpose:
//                              
//      Instantiates a new OLE object
//  
// Parameters:
//                                   
//      LPUNKNOWN pUnkOuter     - Pointer to the controlling unknown
//
//      REFIID riid             - The interface type to fill in ppvObject
//
//      LPVOID* ppvObject       - Out pointer for the object
// 
// Return Value:
//    
//      S_OK                    - Creation was successful
//      CLASS_E_NOAGGREGATION   - Tried to be created as part of an aggregate
//                
// 
// Function Calls:
//      Function                    Location
//                                    
//      CSimpSvrDoc::CreateObject   DOC.CPP
//
// Comments:
//
//
//********************************************************************

STDMETHODIMP CClassFactory::CreateInstance(LPUNKNOWN pUnkOuter,
                                           REFIID riid,
                                           LPVOID* ppvObject)
{
    OutputDebugString(TEXT("In CClassFactory::CreateInstance\r\n"));

    // need to NULL the out parameter
    *ppvObject = NULL;

    // Don't allow aggregation
    if (NULL != pUnkOuter)
        return CLASS_E_NOAGGREGATION;

    return m_pApp->m_pDoc->CreateObject(riid, ppvObject);
}

//**********************************************************************
//
// CClassFactory::LockServer
// 
// Purpose:
// 
//  
// Parameters:
//                          
//      BOOL fLock      - TRUE to lock the server, FALSE to unlock it      
// 
// Return Value:                                                     
//
//      S_OK              
// 
// Function Calls:
//      Function                    Location
//          
//      CoLockObjectExternal        OLE API
//
// Comments:
//
//
//********************************************************************

STDMETHODIMP CClassFactory::LockServer(BOOL fLock)
{
    OutputDebugString(TEXT("In CClassFactory::LockServer\r\n"));

    return CoLockObjectExternal(m_pApp, fLock, TRUE);
}
