class CClassFactory : public IClassFactory
{
public:
	CClassFactory() { m_nCount = 0; }

	STDMETHODIMP QueryInterface (REFIID riid, LPVOID FAR* ppvObj);
	STDMETHODIMP_(ULONG) AddRef ();
	STDMETHODIMP_(ULONG) Release ();
	STDMETHODIMP CreateInstance (LPUNKNOWN pUnkOuter,
								 REFIID riid,
								 LPVOID FAR* ppvObject);
	STDMETHODIMP LockServer (BOOL fLock);

private:
	int m_nCount;
};

class CPersistStorage : public IPersistStorage {
public:
	CPersistStorage(){ m_nCount = 0;}

	STDMETHODIMP QueryInterface (REFIID riid, LPVOID FAR* ppvObj);
	STDMETHODIMP_(ULONG) AddRef ();
	STDMETHODIMP_(ULONG) Release ();
	STDMETHODIMP GetClassID (LPCLSID lpClassID);
	STDMETHODIMP IsDirty ();
	STDMETHODIMP InitNew (LPSTORAGE pStg);
	STDMETHODIMP Load (LPSTORAGE pStg);
	STDMETHODIMP Save (LPSTORAGE pStgSave, BOOL fSameAsLoad);
	STDMETHODIMP SaveCompleted (LPSTORAGE pStgNew);
	STDMETHODIMP HandsOffStorage ();

private:
	int m_nCount;

};
