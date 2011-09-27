//----------------------------------------------------------------------------
//
// (c) Copyright Microsoft Corp. 1993 All Rights Reserved
//
//  module:  classfct.h
//  date:
//  author:  tonykit
//
//  purpose:
//
//----------------------------------------------------------------------------
#ifndef __CLASSFCT_H__
#define __CLASSFCT_H__

#ifndef RC_INVOKED
#pragma message ("INCLUDING CLASSFCT.H from " __FILE__)
#endif  /* RC_INVOKED */

typedef LPVOID FAR* LPLPVOID;

#define WM_UPDATE_CONTAINER   WM_USER+1221

/////////////////////////////////////////////////////////////////////////////

class CClassFactory : public IClassFactory, public IAdviseSink2
{
public:
  CClassFactory();
  ~CClassFactory();

  HRESULT CreateClassObject(HWND hWndOwner, REFCLSID rclsid);
  HRESULT UpdateContainer(VOID);
  BOOL    CanRevokeNow(VOID);

  //*** IUnknown ***
  STDMETHOD(QueryInterface) (REFIID riid, LPVOID FAR* ppvObj);
  STDMETHOD_(ULONG,AddRef) (VOID);
  STDMETHOD_(ULONG,Release) (VOID);

  // *** IClassFactory methods ***
  STDMETHOD(CreateInstance) (IUnknown FAR* pUnkOuter, REFIID riid,
							 LPLPVOID ppunkObject);
  STDMETHOD(LockServer) (BOOL fLock);

  // *** IAdviseSink methods ***
  STDMETHOD_(void,OnDataChange)(FORMATETC FAR* petc, STGMEDIUM FAR* pmed);
  STDMETHOD_(void,OnViewChange)(DWORD dwAspect, LONG lindex);
  STDMETHOD_(void,OnRename)(LPMONIKER pmk);
  STDMETHOD_(void,OnSave)(VOID);
  STDMETHOD_(void,OnClose)(VOID);

  // *** IAdviseSink2 methods ***
  STDMETHOD_(void,OnLinkSrcChange)(LPMONIKER pmk);

//----------------------------------------------------------------------------
// shared features common to all implementations
//----------------------------------------------------------------------------
private:

  ULONG m_dwRefs;
  CLSID m_ClsId;
  LPCLASSFACTORY m_pClassFactory;
  LPOLEOBJECT m_pOleObject;
  DWORD m_dwOleAdvise, m_dwDataAdvise;
  HWND m_hWndOwner;

}; /* CClassFactory */


/////////////////////////////////////////////////////////////////////////////

#endif // __CLASSFCT_H__
