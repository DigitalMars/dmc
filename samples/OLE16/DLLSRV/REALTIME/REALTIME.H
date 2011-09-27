// realtime.h : main header file for the REALTIME application
//

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including void file for PCH
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CRealtimeApp:
// See realtime.cpp for the implementation of void class
//

class CRTClassFactory;

class CRealtimeApp : public CWinApp
{
public:
	CRealtimeApp();
	 virtual ~CRealtimeApp(void);
// Overrides
	virtual BOOL InitApplication();
	virtual BOOL InitInstance();
	void Dump(CDumpContext& dc) const;

// Implementation

	//{{AFX_MSG(CRealtimeApp)
	afx_msg void OnAppAbout();
	afx_msg void OnUpdateAppExit(CCmdUI* pCmdUI);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	CRealtimeDoc * m_pDoc;
	BOOL    m_fUser;
	BOOL    Exitable(void);

	DWORD   m_dwRegCF;
	CRTClassFactory FAR* m_pClassFactory;
	BOOL    m_fInitialized;
};

extern CRealtimeApp NEAR theApp;
class CRTContainer;

class CRTClassFactory: public IClassFactory
{
public:
	STDMETHOD(QueryInterface) (REFIID riid, LPVOID FAR * ppv);
	STDMETHOD_(ULONG, AddRef)(void);
	STDMETHOD_(ULONG, Release)(void);

	STDMETHOD (CreateInstance) ( LPUNKNOWN punkOuter,
		REFIID riid, LPVOID FAR * ppvObj);
	STDMETHOD (LockServer) (BOOL fLock);
	CRTClassFactory(void);
private:
	class CParseDisplayName : public IParseDisplayName
	//  good comment here
	{
	public:
		CParseDisplayName(CRTClassFactory FAR * pClassFactory);
	   STDMETHOD(QueryInterface) ( REFIID riid, LPVOID FAR* ppvObj);
	   STDMETHOD_(ULONG,AddRef) (void);
	   STDMETHOD_(ULONG,Release) (void);

	   // *** IParseDisplayName method ***
	   STDMETHOD(ParseDisplayName) ( LPBC pbc, LPSTR lpszDisplayName,
		  ULONG FAR* pchEaten, LPMONIKER FAR* ppmkOut);

	private:
		CRTClassFactory FAR* m_pClassFactory;
	};

	friend class CParseDisplayName;
	CParseDisplayName m_parseDisplayName;

	ULONG m_refs;
public:
	CRTContainer FAR* m_pContainer;
};

#ifdef INITGUIDS
#include <initguid.h>
#endif
DEFINE_GUID(CLSID_CRealtime, 0x73F76E20, 0xB853, 0x1068,
0xB8, 0x7A, 0x00, 0xDD, 0x01, 0x07, 0xD0, 0xBD);

/////////////////////////////////////////////////////////////////////////////
