/**************************************************************************
 *
 *  THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF ANY
 *  KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
 *  IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A PARTICULAR
 *  PURPOSE.
 *
 *  Copyright (c) 1993 - 1995  Microsoft Corporation.  All Rights Reserved.
 * 
 **************************************************************************/

/*
** _StdClassImplementations
** Defines the standard implementations for a class object.
*/

#define	_StdClassImplementations(Impl)	\
	STDMETHODIMP QueryInterface(const IID FAR& riid, void FAR* FAR* ppv);	\
	STDMETHODIMP_(ULONG) AddRef();	\
	STDMETHODIMP_(ULONG) Release()


/*	-	-	-	-	-	-	-	-	*/

class FAR CAppCF : IClassFactory {
public:
    static HRESULT MakeInst(const CLSID FAR& rclsid, const IID FAR& riid, void FAR* FAR* ppv);
private:
    CAppCF(const CLSID FAR& rclsid, IUnknown FAR* FAR* ppUnknown);
    ULONG	m_refs;
    
public:
    _StdClassImplementations(CAppCF);
    STDMETHODIMP CreateInstance(IUnknown FAR* pUnknownOuter, const IID FAR& riid, void FAR* FAR* ppv);
    STDMETHODIMP LockServer(BOOL fLock);
public:
    CLSID	m_clsid;
};

/*
** The usage counter keeps track of the overall usage of objects based on
** implementations provided by the component. This allows one to determine
** when the implementation is no longer in use.
*/

extern UINT	uUseCount;
extern BOOL	fLocked;
