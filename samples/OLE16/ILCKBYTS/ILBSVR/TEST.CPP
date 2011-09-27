#include "stdafx.h"
#include <windows.h>
#include "ole2.h"
#include "storage.h"

#include "test.h"

// Class Factory methods...
STDMETHODIMP CClassFactory::QueryInterface (REFIID riid, LPVOID FAR* ppvObj)
{
	*ppvObj = NULL;
	SCODE sc = E_NOINTERFACE;

	if ( (riid == IID_IClassFactory) ||
		 (riid == IID_IUnknown) )
		{
		*ppvObj = this;
		AddRef();
		sc = S_OK;
		}

	return ResultFromScode (sc);
}

STDMETHODIMP_(ULONG) CClassFactory::AddRef ()
{

	return ++m_nCount;
}

STDMETHODIMP_(ULONG) CClassFactory::Release ()
{
	if (--m_nCount == 0)
		delete this;

	return m_nCount;
}

STDMETHODIMP CClassFactory::CreateInstance (LPUNKNOWN pUnkOuter,
											REFIID riid,
											LPVOID FAR* ppvObject)
{
	CPersistStorage FAR *lpPS = new CPersistStorage;

	return lpPS->QueryInterface(riid,ppvObject);
}

STDMETHODIMP CClassFactory::LockServer (BOOL fLock)
{
	return NOERROR;
}


// IPersistStorage Methods...Only the IUnknown and Save methods are implemented
STDMETHODIMP CPersistStorage::QueryInterface (REFIID riid, LPVOID FAR* ppvObj)
{
	*ppvObj = NULL;
	SCODE sc = E_NOINTERFACE;

	if ( (riid == IID_IPersistStorage) ||
		 (riid == IID_IUnknown) )
		{
		*ppvObj = this;
		AddRef();
		sc = S_OK;
		}

	return ResultFromScode (sc);
}

STDMETHODIMP_(ULONG) CPersistStorage::AddRef ()
{

	return ++m_nCount;
}

STDMETHODIMP_(ULONG) CPersistStorage::Release ()
{
	if (--m_nCount == 0)
		delete this;

	return m_nCount;
}

	// *** IPersist methods ***
STDMETHODIMP CPersistStorage::GetClassID (LPCLSID lpClassID)
{
	return ResultFromScode(E_NOTIMPL);
}

	// *** IPersistStorage methods ***
STDMETHODIMP CPersistStorage::IsDirty ()
{
	return ResultFromScode(E_NOTIMPL);
}

STDMETHODIMP CPersistStorage::InitNew (LPSTORAGE pStg)
{
	return ResultFromScode(E_NOTIMPL);
}

STDMETHODIMP CPersistStorage::Load (LPSTORAGE pStg)
{
	return ResultFromScode(E_NOTIMPL);
}

// This method is the "Test" method for the ILockBytes.
STDMETHODIMP CPersistStorage::Save (LPSTORAGE pStgSave, BOOL fSameAsLoad)
{
	LPSTREAM lpStream;
	int nCount = 0;

	// open the "Number Of Bytes" stream.
	pStgSave->OpenStream ( "Number of Bytes",
						   0,
						   STGM_DIRECT | STGM_SHARE_EXCLUSIVE | STGM_READWRITE,
						   0,
						   &lpStream);

	// read the number of bytes to be written.
	if (lpStream)
		{
		lpStream->Read(&nCount,sizeof(nCount), NULL);
		lpStream->Release();
		}

	// create the "Test" stream
	pStgSave->CreateStream( "Test",
							STGM_DIRECT | STGM_SHARE_EXCLUSIVE | STGM_READWRITE,
							0,
							0,
							&lpStream);

	// write the number of bytes read in from above
	if (lpStream)
		{
		for (int i=0;i < nCount; i++)
			lpStream->Write("A",1,NULL);

		lpStream->Release();
		}

	return ResultFromScode(S_OK);
}

STDMETHODIMP CPersistStorage::SaveCompleted (LPSTORAGE pStgNew)
{
	return ResultFromScode(E_NOTIMPL);
}

STDMETHODIMP CPersistStorage::HandsOffStorage ()
{
	return ResultFromScode(E_NOTIMPL);
}
