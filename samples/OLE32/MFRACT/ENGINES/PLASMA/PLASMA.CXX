//+---------------------------------------------------------------------------
//
//  Microsoft Windows
//  Copyright (C) Microsoft Corporation, 1992 - 1995.
//
//  File:       plasma.cxx
//
//  Contents:   implementation for Plasma Fractal engine
//
//  Classes:    CPlasmaCF
//
//  Functions:  DllEntryPoint
//              DllGetClassObject
//              DllCanUnloadNow
//
//  History:    4-23-94   stevebl   Created
//
//----------------------------------------------------------------------------

#include <windows.h>
#include <ole2.h>
#include "plasma.h"
#include <plasmcid.h>


ULONG gcRef, gcLock;
HINSTANCE ghinst;

extern "C"
{
    BOOL WINAPI DllMain(HINSTANCE, DWORD, LPVOID);
}

//+---------------------------------------------------------------------------
//
//  Function:   DllEntryPoint
//
//  Synopsis:   the DLL's entry point
//
//  Arguments:  [hinst]      - instance handle for the dll
//              [fdwReason]  - reason for calling the entry point
//              [lpReserved] - reserved
//
//  Returns:    TRUE if DLL_PROCESS_ATTACH initialization succeeds
//              FALSE otherwise
//
//  History:    4-23-94   stevebl   Created
//
//----------------------------------------------------------------------------

BOOL WINAPI DllMain(
    HINSTANCE hinst,
    DWORD fdwReason,
    LPVOID lpReserved)
{
    switch (fdwReason)
    {
    case DLL_PROCESS_ATTACH:
        ghinst = hinst;
        gcRef = gcLock = 0;
        return(TRUE); // initialization succeeded
    case DLL_THREAD_ATTACH:
        break;
    case DLL_THREAD_DETACH:
        break;
    case DLL_PROCESS_DETACH:
        break;
    }
    return(FALSE);  // initialization failed (ignored if not DLL_PROCESS_ATTACH)
}

//+---------------------------------------------------------------------------
//
//  Function:   DllGetClassObject
//
//  Synopsis:   function called by OLE to get a class object
//
//  Arguments:  [rclsid] - the class id of the desired class object
//              [riid]   - the id of the desired interface
//              [ppv]    - pointer to recieve the interface
//
//  History:    4-23-94   stevebl   Created
//
//----------------------------------------------------------------------------

HRESULT PASCAL DllGetClassObject(
    REFCLSID rclsid,
    REFIID riid,
    LPVOID * ppv)
{
    if (!IsEqualGUID(rclsid, CLSID_PLASMA))
    {
        return(E_FAIL);
    }
    if ((!IsEqualGUID(riid, IID_IUnknown)) &&
        (!IsEqualGUID(riid, IID_IClassFactory)))
    {
        return(E_NOINTERFACE);
    }
    *ppv = (LPVOID) new CPlasmaCF();
    if (NULL == *ppv)
    {
        return(E_OUTOFMEMORY);
    }
    ((IUnknown*)*ppv)->AddRef();
    return(S_OK);
}

//+---------------------------------------------------------------------------
//
//  Function:   DllCanUnloadNow
//
//  Synopsis:   determines if there are no outstanding references on this DLL
//
//  Returns:    S_OK if the DLL can be unloaded
//              S_FALSE if not
//
//  History:    4-23-94   stevebl   Created
//
//----------------------------------------------------------------------------

STDAPI DllCanUnloadNow(void)
{
    // return S_OK if there are no references on myself
    if (0 == gcRef && 0 == gcLock)
    {
        return(S_OK);
    }
    return(S_FALSE);
}

//+---------------------------------------------------------------------------
//
//  Member:     CPlasmaCF::CPlasmaCF
//
//  Synopsis:   constructor
//
//  History:    4-23-94   stevebl   Created
//
//----------------------------------------------------------------------------

CPlasmaCF::CPlasmaCF()
{
    _cRef = 0;
}

//+---------------------------------------------------------------------------
//
//  Member:     CPlasmaCF::~CPlasmaCF
//
//  Synopsis:   destructor
//
//  History:    4-23-94   stevebl   Created
//
//----------------------------------------------------------------------------

CPlasmaCF::~CPlasmaCF()
{
}

//+---------------------------------------------------------------------------
//
//  Member:     CPlasmaCF::QueryInterface
//
//  Synopsis:   Standard OLE interface
//
//  Arguments:  [riid] - id of the desired interface
//              [ppv]  - pointer to receive the interface
//
//  History:    4-23-94   stevebl   Created
//
//----------------------------------------------------------------------------

HRESULT STDMETHODCALLTYPE CPlasmaCF::QueryInterface(REFIID riid, LPVOID * ppv)
{
    *ppv = NULL;
    if ((!IsEqualGUID(riid, IID_IUnknown)) &&
        (!IsEqualGUID(riid, IID_IClassFactory)))
    {
        return(E_NOINTERFACE);
    }
    *ppv = (IClassFactory *) this;
    AddRef();
    return(S_OK);
}

//+---------------------------------------------------------------------------
//
//  Member:     CPlasmaCF::AddRef
//
//  Synopsis:   increment reference count
//
//  Returns:    new reference count
//
//  History:    4-23-94   stevebl   Created
//
//----------------------------------------------------------------------------

ULONG STDMETHODCALLTYPE CPlasmaCF::AddRef(void)
{
    return(++_cRef);
}

//+---------------------------------------------------------------------------
//
//  Member:     CPlasmaCF::Release
//
//  Synopsis:   decrement reference count
//
//  Returns:    new reference count
//
//  History:    4-23-94   stevebl   Created
//
//----------------------------------------------------------------------------

ULONG STDMETHODCALLTYPE CPlasmaCF::Release(void)
{
    ULONG cRef = --_cRef;
    if (0 == cRef)
    {
        delete(this);
    }
    return(cRef);
}

//+---------------------------------------------------------------------------
//
//  Member:     CPlasmaCF::CreateInstance
//
//  Synopsis:   creates an instance of a Plasma Engine
//
//  Arguments:  [pUnkOuter] - controlling unknown (must be NULL)
//              [riid]      - id of desired interface
//              [ppv]       - pointer to receive the interface
//
//  Returns:    S_OK - success
//              CLASS_E_NOAGGREATION - the caller tried to aggregate
//              CLASS_E_CLASSNOTAVAILABLE - couldn't initialize the class
//              E_OUTOFMEMORY - not enough memory to instantiate class
//
//  History:    4-23-94   stevebl   Created
//
//----------------------------------------------------------------------------

HRESULT STDMETHODCALLTYPE CPlasmaCF::CreateInstance(LPUNKNOWN pUnkOuter, REFIID riid, LPVOID * ppv)
{
    HRESULT hr = S_OK;
    *ppv = NULL;
    if (pUnkOuter != NULL)
    {
        return(CLASS_E_NOAGGREGATION);
    }

    CPlasma * pObj = new CPlasma;
    if (NULL == pObj)
    {
        return(E_OUTOFMEMORY);
    }

    if (!pObj->Initialize())
    {
        delete ppv;
        return(CLASS_E_CLASSNOTAVAILABLE);
    }

    hr = pObj->QueryInterface(riid, ppv);
    if (FAILED(hr))
    {
        delete ppv;
    }
    else
    {
        gcRef++;
    }

    return(hr);
}

//+---------------------------------------------------------------------------
//
//  Member:     CPlasmaCF::LockServer
//
//  Synopsis:   locks the server, preventing it from being unloaded
//
//  Arguments:  [fLock] - TRUE to lock, FALSE to unlock
//
//  Returns:    S_OK
//
//  Modifies:   gcLock
//
//  History:    4-23-94   stevebl   Created
//
//----------------------------------------------------------------------------

HRESULT STDMETHODCALLTYPE CPlasmaCF::LockServer(BOOL fLock)
{
    if (fLock)
    {
        gcLock++;
    }
    else
    {
        gcLock--;
    }
    return(S_OK);
}

