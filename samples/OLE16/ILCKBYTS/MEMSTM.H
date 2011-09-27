#if !defined( _MEMSTM_H_ )
#define _MEMSTM_H_

#include <assert.h>

class FAR CMarshalMemBytes;

struct MEMSTM {             // Data in shared memory
	DWORD  cb;              // Size of hGlobal
	DWORD  cRef;            // See below
	HANDLE hGlobal;         // The data
	BOOL   fDeleteOnRelease;
};

STDAPI_(void) ReleaseMemStm (LPHANDLE phMem, BOOL fInternalOnly = FALSE);
STDAPI DllGetClassObject ( REFCLSID clsid, REFIID iid, LPVOID FAR *ppv );
STDAPI CreateMyILockBytes (BOOL fCustomMarshal, LPLOCKBYTES FAR* lplpBytes);



// CMemBytes is an ILockBytes implementation on top of global shared
// memory MEMSTM
//
// CMemBytes
// +---------+
// + pvtf    +    Shared  memory
// +---------+   +--------------+
// + m_pData +-->| cb           |
// +---------+   | cRef         |
//               | hGlobal      |--->+-------------+
//               +--------------+    | Actual data |
// CMemBytes         MEMSTM          +-------------+
//

// cRef counts all CMemBytes pointers to this MEMSTM.
// It and fDeleteOnRelease control the GlobalFree'ing of the hGlobal.

class FAR CMemBytes : public ILockBytes { // Shared memory lock bytes
public:
	STDMETHOD(QueryInterface) (REFIID iidInterface, void FAR* FAR* ppvObj);
	STDMETHOD_(ULONG,AddRef) (void);
	STDMETHOD_(ULONG,Release) (void);
	STDMETHOD(ReadAt) (ULARGE_INTEGER ulOffset, VOID HUGEP *pv, ULONG cb,
													ULONG FAR *pcbRead);
	STDMETHOD(WriteAt) (ULARGE_INTEGER ulOffset, VOID const HUGEP *pv, ULONG cb,
													ULONG FAR *pcbWritten);
	STDMETHOD(Flush) (void);
	STDMETHOD(SetSize) (ULARGE_INTEGER cb);
	STDMETHOD(LockRegion) (ULARGE_INTEGER libOffset, ULARGE_INTEGER cb, DWORD dwLockType);
	STDMETHOD(UnlockRegion) (ULARGE_INTEGER libOffset, ULARGE_INTEGER cb, DWORD dwLockType);
	STDMETHOD(Stat) (THIS_ STATSTG FAR *pstatstg, DWORD statflag);

	static CMemBytes FAR* Create(HANDLE hMem, BOOL fCustomMarshal = TRUE);

	CMemBytes() { m_hMem = NULL; m_pData = NULL; m_refs = 0; }
	~CMemBytes() { assert (m_refs == 0); }

private:
	ULONG m_refs;                   // Normal reference count
	HANDLE m_hMem;                  // Handle for bookeeping info (MEMSTM)
	MEMSTM FAR* m_pData;            // Pointer to that memory
	BOOL m_fCustomMarshal;

	friend class CMarshalMemBytes;
};




// CMarshalMemBytes can Marshal, Unmarshal CMemBytes.  It is impletented as
// a seperate object accessible from CMemBytes, CMemBytes: QueryInterface of
// IMarshal on CMemBytes's ILocBytes will return an IMarshal pointer to
// CMarshalMemBytes, but QueryInterface of ILockBytes on that IMarshal will
// fail.
// Also QueryInterface of IUnknown on IMarshal will not return the same value
// As QueryInterface of IUnkown on the original ILockBytes.
//
class FAR CMarshalMemBytes : public IMarshal {
public:
	STDMETHOD(QueryInterface) (REFIID riid, LPVOID FAR* ppvObj);
	STDMETHOD_(ULONG,AddRef) (void);
	STDMETHOD_(ULONG,Release) (void);

	STDMETHOD(GetUnmarshalClass)(THIS_ REFIID riid, LPVOID pv,
								 DWORD dwDestContext, LPVOID pvDestContext,
								 DWORD mshlflags, LPCLSID pCid);
	STDMETHOD(GetMarshalSizeMax)(THIS_ REFIID riid, LPVOID pv,
								 DWORD dwDestContext, LPVOID pvDestContext,
								 DWORD mshlflags, LPDWORD pSize);
	STDMETHOD(MarshalInterface)(THIS_ IStream FAR* pStm, REFIID riid,
								LPVOID pv, DWORD dwDestContext, LPVOID pvDestContext,
								DWORD mshlflags);
	STDMETHOD(UnmarshalInterface)(THIS_ IStream FAR* pStm, REFIID riid,
								  LPVOID FAR* ppv);
	STDMETHOD(ReleaseMarshalData)(THIS_ IStream FAR* pStm);
	STDMETHOD(DisconnectObject)(THIS_ DWORD dwReserved);

	static CMarshalMemBytes FAR* Create(CMemBytes FAR* pMemBytes);

	CMarshalMemBytes() { OutputDebugString("Creating CMarshalMemBytes\r\n");
						 m_pMemBytes = NULL; m_refs = 0; }
	~CMarshalMemBytes() { OutputDebugString("Destroying CMarshalMemBytes\r\n");
						  assert (m_refs == 0);}

private:
	ULONG m_refs;                   // Number of references to this CMemBytes
	CMemBytes FAR* m_pMemBytes;     // Pointer to object [Un]Marshalled
	CLSID m_clsid;                  // Class of object pointed by pUnk
};


// CClassFactory is the ClassFactory for the CMarshalMemBytes Class.  This
// is needed to unmarshal the data when process boundaries are crossed.
class FAR CClassFactory : public IClassFactory
{
public:
	// *** IUnknown methods ***
	STDMETHOD(QueryInterface) (THIS_ REFIID riid, LPVOID FAR* ppvObj);
	STDMETHOD_(ULONG,AddRef) (THIS);
	STDMETHOD_(ULONG,Release) (THIS);

	// *** IClassFactory methods ***
	STDMETHOD(CreateInstance) (THIS_ LPUNKNOWN pUnkOuter,
							  REFIID riid,
							  LPVOID FAR* ppvObject);
	STDMETHOD(LockServer) (THIS_ BOOL fLock);

	CClassFactory::CClassFactory() { m_refs = 0; }
	CClassFactory::~CClassFactory() {assert (m_refs == 0);}

private:
	ULONG m_refs;
};

#endif // _MemBytes_H
