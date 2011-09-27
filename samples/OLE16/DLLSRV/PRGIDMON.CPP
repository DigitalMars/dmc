/*
	prgidmon.cpp

	This file contains the CProgIdMoniker object implementation
	and the CProgIdMonikerFactory object implementation.
	This sample demonstrates how to write a new leading moniker
	(instead of using a FileMoniker).

	A ProgIdMoniker stores a ProgId. Its BindToObject methods
	operates by calling CoCreateInstance on the class associated
	with the stored ProgId.

	(c) Copyright Microsoft Corp. 1992 - 1993 All Rights Reserved
*/

#define DEFINE_PRGIDMON_GUID_HERE
#ifdef __TURBOC__
#define classmodel _huge
#else
#define classmodel FAR
#endif
#include "prgidmon.h"


DWORD g_dwObjectCount = 0; // count of objects given out by this DLL


/*
 *      Since we are a InProcServer object, we may be running in the context
 *      of any process that chooses to instantiate our object.  This
 *      means that we should use the memory allocator that that process
 *      gave in its OleInitialize/CoInitialize call.  We get this by calling
 *      CoGetMalloc.
 *
 *      We can encapsulate this by redefining the global new operator
 *      and using new for all memory allocations.
 */

void FAR* operator new( size_t size)
{
	IMalloc FAR* pMalloc;
	LPVOID lpv;

	if (CoGetMalloc(MEMCTX_TASK, &pMalloc) == NOERROR)
	{
		lpv = pMalloc->Alloc(size);
		pMalloc->Release();
		return lpv;
	}
	return NULL;
}

/*
 *      We need to redefine delete to match our redefinition of new.
 */

void operator delete( void FAR* lpv)
{
	IMalloc FAR* pMalloc;

	if (lpv == NULL) return;

  if( CoGetMalloc(MEMCTX_TASK, &pMalloc) == NOERROR) {
	if (pMalloc->DidAlloc(lpv))
	  pMalloc->Free(lpv);
	pMalloc->Release();
  }
}


class classmodel CProgIdMonikerFactory :  public IClassFactory
{
	CProgIdMonikerFactory(void) : m_refs(0) { g_dwObjectCount++ ; };
	~CProgIdMonikerFactory() { g_dwObjectCount-- ; };

	STDMETHOD(QueryInterface) (REFIID riid, LPVOID FAR* ppvObj);
	STDMETHOD_(ULONG,AddRef) (void);
	STDMETHOD_(ULONG,Release) (void);
	STDMETHOD(CreateInstance)(IUnknown FAR* pUnkOuter, REFIID iidInterface, void FAR* FAR* ppvObject);
	STDMETHOD(LockServer)( BOOL fLock );

	friend HRESULT STDAPICALLTYPE DllGetClassObject(REFCLSID, REFIID, void FAR* FAR*);

private:
	ULONG m_refs;
};



class classmodel CProgIdMoniker :  public IMoniker
{
public:
	CProgIdMoniker( void );
	CProgIdMoniker( LPSTR szProgId );
	~CProgIdMoniker( void );

	// *** IUnknown methods ***
	STDMETHOD(QueryInterface) (REFIID riid, LPVOID FAR* ppvObj);
	STDMETHOD_(ULONG,AddRef) (void);
	STDMETHOD_(ULONG,Release) (void);

	// *** IPersist methods ***
	STDMETHOD(GetClassID) (LPCLSID lpClassID);

	// *** IPersistStream methods ***
	STDMETHOD(IsDirty) (void);
	STDMETHOD(Load) (LPSTREAM pStm);
	STDMETHOD(Save) (LPSTREAM pStm,
					BOOL fClearDirty);
	STDMETHOD(GetSizeMax) (ULARGE_INTEGER FAR * pcbSize);

	// *** IMoniker methods ***
	STDMETHOD(BindToObject) (LPBC pbc, LPMONIKER pmkToLeft,
		REFIID riidResult, LPVOID FAR* ppvResult);
	STDMETHOD(BindToStorage) (LPBC pbc, LPMONIKER pmkToLeft,
		REFIID riid, LPVOID FAR* ppvObj);
	STDMETHOD(Reduce) (LPBC pbc, DWORD dwReduceHowFar, LPMONIKER FAR*
		ppmkToLeft, LPMONIKER FAR * ppmkReduced);
	STDMETHOD(ComposeWith) (LPMONIKER pmkRight, BOOL fOnlyIfNotGeneric,
		LPMONIKER FAR* ppmkComposite);
	STDMETHOD(Enum) (BOOL fForward, LPENUMMONIKER FAR* ppenumMoniker);
	STDMETHOD(IsEqual) (LPMONIKER pmkOtherMoniker);
	STDMETHOD(Hash) (LPDWORD pdwHash);
	STDMETHOD(IsRunning) (LPBC pbc, LPMONIKER pmkToLeft, LPMONIKER
		pmkNewlyRunning);
	STDMETHOD(GetTimeOfLastChange) (LPBC pbc, LPMONIKER pmkToLeft,
		FILETIME FAR* pfiletime);
	STDMETHOD(Inverse) (LPMONIKER FAR* ppmk);
	STDMETHOD(CommonPrefixWith) (LPMONIKER pmkOther, LPMONIKER FAR*
		ppmkPrefix);
	STDMETHOD(RelativePathTo) (LPMONIKER pmkOther, LPMONIKER FAR*
		ppmkRelPath);
	STDMETHOD(GetDisplayName) (LPBC pbc, LPMONIKER pmkToLeft,
		LPSTR FAR* lplpszDisplayName);
	STDMETHOD(ParseDisplayName) (LPBC pbc, LPMONIKER pmkToLeft,
		LPSTR lpszDisplayName, ULONG FAR* pchEaten,
		LPMONIKER FAR* ppmkOut);
	STDMETHOD(IsSystemMoniker) (LPDWORD pdwMksys);

class CMarshal : public IUnknown
{
public:
	CMarshal ( CProgIdMoniker FAR* pprogidmon );
	// *** IUnknown methods ***
	STDMETHOD(QueryInterface) ( REFIID riid, LPVOID FAR* ppvObj);
	STDMETHOD_(ULONG,AddRef) (void);
	STDMETHOD_(ULONG,Release) (void);

	// *** IMarshal methods ***
	STDMETHOD(GetUnmarshalClass)( REFIID riid, LPVOID pv,
						DWORD dwDestContext, LPVOID pvDestContext,
						DWORD mshlflags, LPCLSID pCid);
	STDMETHOD(GetMarshalSizeMax)( REFIID riid, LPVOID pv,
						DWORD dwDestContext, LPVOID pvDestContext,
						DWORD mshlflags, LPDWORD pSize);
	STDMETHOD(MarshalInterface)( LPSTREAM pStm, REFIID riid,
						LPVOID pv, DWORD dwDestContext, LPVOID pvDestContext,
						DWORD mshlflags);
	STDMETHOD(UnmarshalInterface)( LPSTREAM pStm, REFIID riid,
						LPVOID FAR* ppv);
	STDMETHOD(ReleaseMarshalData)( LPSTREAM pStm);
	STDMETHOD(DisconnectObject)( DWORD dwReserved);
private:
	CProgIdMoniker FAR* m_pprogidmon;
};

friend class CMarshal;
CMarshal m_marshal;

private:
	ULONG m_refs;
	LPSTR m_pszService;
};


STDMETHODIMP CProgIdMonikerFactory::QueryInterface (REFIID riid, LPVOID FAR* ppvObj)
{
	*ppvObj = NULL;
	if (riid == IID_IUnknown || riid == IID_IClassFactory)
		*ppvObj = this;
	if (*ppvObj != NULL)
	{
		AddRef();
		return NOERROR;
	}
	return ResultFromScode(E_NOINTERFACE);
}


STDMETHODIMP_(ULONG) CProgIdMonikerFactory::AddRef (void)
{
	return ++m_refs;
}

STDMETHODIMP_(ULONG) CProgIdMonikerFactory::Release (void)
{
	if (--m_refs == 0)
	{
		delete this;
		return 0;
	}
	return m_refs;
}


STDMETHODIMP CProgIdMonikerFactory::CreateInstance(IUnknown FAR* pUnkOuter,
	REFIID iidInterface, void FAR* FAR* ppvObject)
{
	CProgIdMoniker FAR * pPIM;
	HRESULT hresult;
	*ppvObject = NULL;

	pPIM = new CProgIdMoniker;  //  ignore aggregation
	if (pPIM == NULL) return ResultFromScode(E_OUTOFMEMORY);
	hresult = pPIM->QueryInterface( iidInterface, ppvObject );
/*
 *  The next line destroys pPIM in the case the QueryInterface
 *  failed (and so pPIM->AddRef was not called).
 */
	pPIM->Release();
/*
 *  We could assert here that the value returned by Release is 1
 *  if hresult == NOERROR, otherwise 0.
 */

	return hresult;
}

STDMETHODIMP CProgIdMonikerFactory::LockServer( BOOL fLock )
{
  if (fLock) {
	g_dwObjectCount ++ ;
  }else{
	g_dwObjectCount -- ;
  }

  return NOERROR;
}



STDAPI  DllGetClassObject(REFCLSID clsid, REFIID iid, void FAR* FAR* ppv)
{
	IClassFactory FAR * pCF;
	HRESULT hresult;

	*ppv = NULL;
	if (clsid == CLSID_ProgIdMoniker)
	{
		pCF = new CProgIdMonikerFactory;
		if (pCF == NULL) return ResultFromScode(E_OUTOFMEMORY);
		pCF->AddRef();  // ref count should now be 1
		hresult = pCF->QueryInterface( iid, ppv ); // now should be 2
		pCF->Release(); // now should be 1 again unless QI failed
		return hresult;
	}
	return ResultFromScode(E_FAIL);
}


STDAPI DllCanUnloadNow(void)
{
  if (g_dwObjectCount == 0) {
	return NOERROR;
  }

  return ResultFromScode(S_FALSE);
}


STDAPI  CreateProgIdMoniker( LPSTR szProgId, LPMONIKER FAR * ppmk)
{
	CProgIdMoniker FAR * pmk = new CProgIdMoniker( szProgId );
	*ppmk = pmk;
	if (pmk != NULL) return NOERROR;
	return ResultFromScode(E_OUTOFMEMORY);
}


CProgIdMoniker::CProgIdMoniker( void )
	:  m_refs(1), m_marshal(this), m_pszService(NULL)
{
  g_dwObjectCount ++ ;
}

CProgIdMoniker::CProgIdMoniker( LPSTR szProgId )
	:  m_refs(1), m_marshal(this)
{
	m_pszService = new char[ 1 + lstrlen(szProgId)];
	lstrcpy( m_pszService, szProgId );
	g_dwObjectCount ++ ;
}

CProgIdMoniker::~CProgIdMoniker( void )
{
	if (m_pszService)
		delete m_pszService;
	g_dwObjectCount -- ;
}

STDMETHODIMP CProgIdMoniker::QueryInterface(REFIID riid, LPVOID FAR* ppvObj)
{
	*ppvObj = NULL;
	if ((riid == IID_IMoniker) || (riid == IID_IUnknown) ||
		(riid == IID_IPersistStream))
	{
		*ppvObj = this;
		++m_refs;
		return NOERROR;
	}
	if (riid == IID_IMarshal)
	{
		*ppvObj = &m_marshal;
		++m_refs;
		return NOERROR;
	}
	return ResultFromScode(E_NOINTERFACE);
}


STDMETHODIMP_(ULONG) CProgIdMoniker::AddRef(void)
{
	return ++m_refs;
}

STDMETHODIMP_(ULONG) CProgIdMoniker::Release(void)
{
	if (--m_refs == 0)
	{
		delete this;
		return 0;
	}
	return m_refs;
}

	// *** IPersist methods ***
STDMETHODIMP CProgIdMoniker::GetClassID(LPCLSID lpClassID)
{
	*lpClassID = CLSID_ProgIdMoniker;
	return NOERROR;
}

	// *** IPersistStream methods ***
STDMETHODIMP CProgIdMoniker::IsDirty(void)
{
	return ResultFromScode(S_FALSE);
}


STDMETHODIMP CProgIdMoniker::Load(LPSTREAM pStm)
{
	ULONG cbRead;
	ULONG cbStrLen;
	HRESULT hresult;

	hresult = pStm->Read( &cbStrLen, sizeof(ULONG), &cbRead);
	if (cbRead == sizeof(ULONG) && hresult == NOERROR)
	{
		m_pszService = new char[cbStrLen];
		hresult = pStm->Read( m_pszService, cbStrLen, &cbRead);
		if (cbRead != cbStrLen) //  error
		{
			delete m_pszService;
			m_pszService = NULL;
			return ResultFromScode(E_FAIL);
		}

	}
	return hresult;
}


STDMETHODIMP CProgIdMoniker::Save(LPSTREAM pStm,
					BOOL fClearDirty)
{
	ULONG cbWritten;
	ULONG cbStrLen = 1 + lstrlen(m_pszService);
	HRESULT hresult;

	hresult = pStm->Write( &cbStrLen, sizeof(ULONG), &cbWritten);
	if (cbWritten == sizeof(ULONG) && hresult == NOERROR)
	{
		hresult = pStm->Write( m_pszService, cbStrLen, &cbWritten);
		if (cbWritten != cbStrLen)  //  error
		{
			return ResultFromScode(E_FAIL);
		}

	}
	return hresult;
}


STDMETHODIMP CProgIdMoniker::GetSizeMax(ULARGE_INTEGER FAR * pcbSize)
{
	ULISet32( *pcbSize,
			  lstrlen(m_pszService) +
			  sizeof(CLSID) +
			  1
	);
	return NOERROR;
}


	// *** IMoniker methods ***
STDMETHODIMP CProgIdMoniker::BindToObject(LPBC pbc, LPMONIKER pmkToLeft,
		REFIID riidResult, LPVOID FAR* ppvResult)
{
	HRESULT hresult;
	CLSID   cid;

	hresult = CLSIDFromProgID(m_pszService, &cid);
	if (hresult == NOERROR)
	{
		hresult = CoCreateInstance(cid, NULL, CLSCTX_SERVER, riidResult,
				ppvResult);
		if (hresult == NOERROR)
		{
			pbc->RegisterObjectBound( (LPUNKNOWN)*ppvResult);
		}
	}
	return hresult;
}


STDMETHODIMP CProgIdMoniker::BindToStorage(LPBC pbc, LPMONIKER pmkToLeft,
		REFIID riid, LPVOID FAR* ppvObj)
{
	return ResultFromScode( MK_E_NOSTORAGE );
}


STDMETHODIMP CProgIdMoniker::Reduce(LPBC pbc, DWORD dwReduceHowFar, LPMONIKER FAR*
		ppmkToLeft, LPMONIKER FAR * ppmkReduced)
{
	*ppmkReduced = this;
	AddRef();
	return ResultFromScode(MK_S_REDUCED_TO_SELF);
}


STDMETHODIMP CProgIdMoniker::ComposeWith(LPMONIKER pmkRight, BOOL fOnlyIfNotGeneric,
		LPMONIKER FAR* ppmkComposite)
{
	HRESULT hresult;
	if (!fOnlyIfNotGeneric)
		hresult = CreateGenericComposite( this, pmkRight, ppmkComposite );
	else
	{
		hresult = ResultFromScode(MK_E_NEEDGENERIC);
		*ppmkComposite = NULL;
	}
	return hresult;
}


STDMETHODIMP CProgIdMoniker::Enum(BOOL fForward, LPENUMMONIKER FAR* ppenumMoniker)
{
	//  These monikers do not have an internal structure
	*ppenumMoniker = NULL;
	return NOERROR;
}


STDMETHODIMP CProgIdMoniker::IsEqual(LPMONIKER pmkOtherMoniker)
{
	CLSID cid;
	CProgIdMoniker FAR* pCPIMOther;

	GetClassID(&cid);

	if (cid == CLSID_ProgIdMoniker)
	{
		pCPIMOther = (CProgIdMoniker FAR*)pmkOtherMoniker;
		if (_fstrcmp(m_pszService, pCPIMOther->m_pszService) == 0)
			return NOERROR;
	}
	return ResultFromScode(S_FALSE);
}


STDMETHODIMP CProgIdMoniker::Hash(LPDWORD pdwHash)
{
	LPSTR psz = m_pszService;
	DWORD dw = 0;
	char ch;

	if (psz)
	while (*psz)
	{
		dw *= 3;
		ch = (char)(DWORD)AnsiUpper((char FAR *)*psz);
		dw ^= ch;
		psz = AnsiNext(psz);
	}
	*pdwHash = dw;
	return NOERROR;
}


STDMETHODIMP CProgIdMoniker::IsRunning(LPBC pbc, LPMONIKER pmkToLeft, LPMONIKER
		pmkNewlyRunning)
{
	return ResultFromScode(S_FALSE);    //  REVIEW
}


STDMETHODIMP CProgIdMoniker::GetTimeOfLastChange(LPBC pbc, LPMONIKER pmkToLeft,
		FILETIME FAR* pfiletime)
{
	return ResultFromScode(MK_E_UNAVAILABLE);
}


STDMETHODIMP CProgIdMoniker::Inverse(LPMONIKER FAR* ppmk)
{
	*ppmk = NULL;
	return ResultFromScode(MK_E_NOINVERSE);
}


STDMETHODIMP CProgIdMoniker::CommonPrefixWith(LPMONIKER pmkOther, LPMONIKER FAR*
		ppmkPrefix)
{
	//  This can have no prefix in common with any moniker except a composite
	//  moniker beginning with an identical CProgIdMoniker
	DWORD dwType;
	HRESULT hresult;
	SCODE scode;

	hresult = pmkOther->IsSystemMoniker(&dwType);
	if (hresult == NOERROR && dwType == MKSYS_GENERICCOMPOSITE)
	{
		hresult = pmkOther->CommonPrefixWith(this, ppmkPrefix);
		scode = GetScode( hresult );
		if (MK_S_HIM == scode) hresult = ResultFromScode(MK_S_ME);
		else if (MK_S_ME == scode) hresult = ResultFromScode(MK_S_HIM);
		return hresult;
	}
	hresult = IsEqual(pmkOther);
	if (hresult == NOERROR) return ResultFromScode( MK_S_US);
	return ResultFromScode(MK_E_NOPREFIX);
}


STDMETHODIMP CProgIdMoniker::RelativePathTo(LPMONIKER pmkOther, LPMONIKER FAR*
		ppmkRelPath)
{
	return MonikerRelativePathTo(this, pmkOther, ppmkRelPath, TRUE);
}


STDMETHODIMP CProgIdMoniker::GetDisplayName(LPBC pbc, LPMONIKER pmkToLeft,
		LPSTR FAR* lplpszDisplayName)
{
	LPSTR psz;
	//  Assert pmkToLeft is null; these monikers are always at the beginning
	//  of composites.

	psz = new FAR char [2 + lstrlen(m_pszService)];
	*lplpszDisplayName = psz;
	if (psz == NULL) return ResultFromScode(E_OUTOFMEMORY);
	//  We do not store the '@'
	*psz = '@';
	psz = AnsiNext(psz);
	lstrcpy(psz, m_pszService);
	return NOERROR;
}


STDMETHODIMP CProgIdMoniker::ParseDisplayName(LPBC pbc, LPMONIKER pmkToLeft,
		LPSTR lpszDisplayName, ULONG FAR* pchEaten,
		LPMONIKER FAR* ppmkOut)
{
	HRESULT hresult;
	IParseDisplayName FAR * pPDN;

	if (ppmkOut == NULL || pbc == NULL)
		return ResultFromScode(E_INVALIDARG);

	*ppmkOut = NULL;
	if (pchEaten)
		*pchEaten = 0;

//  OleDbgAssertSz( pmkToLeft == NULL,
//      "ProgIdMonikers cannot have anything to the left");

	hresult = BindToObject( pbc, pmkToLeft, IID_IParseDisplayName,
			(VOID FAR * FAR *)&pPDN );
	if (hresult) goto errRet;
	hresult = pbc->RegisterObjectBound(pPDN);
	if (hresult) goto errRet;
	hresult = pPDN->ParseDisplayName(pbc, lpszDisplayName, pchEaten,
		ppmkOut);

errRet:
	if (pPDN) pPDN->Release();
	return hresult;
}


STDMETHODIMP CProgIdMoniker::IsSystemMoniker(LPDWORD pdwMksys)
{
	*pdwMksys = MKSYS_NONE;
	return NOERROR;
}


CProgIdMoniker::CMarshal::CMarshal( CProgIdMoniker FAR * pprogidmon )
	:  m_pprogidmon(pprogidmon)
{
}

STDMETHODIMP CProgIdMoniker::CMarshal::QueryInterface( REFIID riid, LPVOID FAR* ppvObj)
{
	return m_pprogidmon->QueryInterface(riid, ppvObj);
}



STDMETHODIMP_(ULONG) CProgIdMoniker::CMarshal::AddRef(void)
{
	return m_pprogidmon->AddRef();
}



STDMETHODIMP_(ULONG) CProgIdMoniker::CMarshal::Release(void)
{
	return m_pprogidmon->Release();
}



	// *** IMarshal methods ***
STDMETHODIMP CProgIdMoniker::CMarshal::GetUnmarshalClass ( REFIID riid, LPVOID pv,
						DWORD dwDestContext, LPVOID pvDestContext,
						DWORD mshlflags, LPCLSID pCid)
{
	return m_pprogidmon->GetClassID(pCid);
}



STDMETHODIMP CProgIdMoniker::CMarshal::GetMarshalSizeMax ( REFIID riid, LPVOID pv,
						DWORD dwDestContext, LPVOID pvDestContext,
						DWORD mshlflags, LPDWORD pSize)
{
	HRESULT hresult;
	ULARGE_INTEGER ularge_integer;

	hresult = m_pprogidmon->GetSizeMax( &ularge_integer );
	if (hresult == NOERROR)
		*pSize = ularge_integer.LowPart;
	return hresult;
}



STDMETHODIMP CProgIdMoniker::CMarshal::MarshalInterface( LPSTREAM pStm, REFIID riid,
						LPVOID pv, DWORD dwDestContext, LPVOID pvDestContext,
						DWORD mshlflags)
{
	return m_pprogidmon->Save(pStm, FALSE);
}



STDMETHODIMP CProgIdMoniker::CMarshal::UnmarshalInterface( LPSTREAM pStm, REFIID riid,
						LPVOID FAR* ppv)
{
	HRESULT hresult;
	hresult = m_pprogidmon->Load(pStm);
	if (hresult == NOERROR)
		hresult = m_pprogidmon->QueryInterface(riid, ppv);
	return hresult;
}



STDMETHODIMP CProgIdMoniker::CMarshal::ReleaseMarshalData (LPSTREAM pStm)
{
	return NOERROR;
}



STDMETHODIMP CProgIdMoniker::CMarshal::DisconnectObject (DWORD dwReserved)
{
	return NOERROR;
}
