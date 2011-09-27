//**********************************************************************
// File name: icf.h
//
//      Definition of CClassFactory
//
// Copyright (c) 1993 - 1995 Microsoft Corporation. All rights reserved.
//**********************************************************************

#if !defined( _ICF_H_)
#define _ICF_H_

class CSimpSvrApp;

interface CClassFactory : IClassFactory
{
private:
    UINT m_cRef;                // reference count
    CSimpSvrApp* m_pApp;

public:
    CClassFactory::CClassFactory(CSimpSvrApp* pApp)
    {
        m_cRef = 0;
        m_pApp = pApp;
    }

    CClassFactory::~CClassFactory() {}

    // IUnknown Methods
    STDMETHODIMP QueryInterface(REFIID riid, LPVOID* ppv);
    STDMETHODIMP_(ULONG) AddRef();
    STDMETHODIMP_(ULONG) Release();

    STDMETHODIMP CreateInstance(LPUNKNOWN pUnkOuter, REFIID riid, LPVOID* ppvObject);
    STDMETHODIMP LockServer(BOOL fLock);
};

#endif
