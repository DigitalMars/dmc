//----------------------------------------------------------------------------
//
// (c) Copyright Microsoft Corp. 1993 All Rights Reserved
//
//  module:  classfct.h
//  date:
//  author:
//
//  purpose:
//
//----------------------------------------------------------------------------
#ifndef __CLASSFCT_H__
#define __CLASSFCT_H__

#ifndef RC_INVOKED
#pragma message ("INCLUDING CLASSFCT.H from " __FILE__)
#endif  /* RC_INVOKED */

extern DWORD g_dwObjectCount;

/* Initialize the DLL, register the classes etc
 */

STDAPI DllGetClassObject(REFCLSID rclsid, REFIID riid, LPVOID FAR* ppv);
STDAPI DllCanUnloadNow(void);

/////////////////////////////////////////////////////////////////////////////

class CClassFactory : public IClassFactory
{
public:
  CClassFactory(REFCLSID rclsid);
  ~CClassFactory();

  //*** IUnknown ***
  STDMETHOD(QueryInterface) (REFIID riid, LPVOID FAR* ppvObj);
  STDMETHOD_(ULONG,AddRef) (VOID);
  STDMETHOD_(ULONG,Release) (VOID);

  // *** IClassFactory methods ***
  STDMETHOD(CreateInstance) (IUnknown FAR* pUnkOuter, REFIID riid,
							 LPLPVOID ppunkObject);
  STDMETHOD(LockServer) (BOOL fLock);

private:
  ULONG m_dwRefs;
  CLSID m_ClsId;

}; /* CClassFactory */


/////////////////////////////////////////////////////////////////////////////

#endif // __CLASSFCT_H__
