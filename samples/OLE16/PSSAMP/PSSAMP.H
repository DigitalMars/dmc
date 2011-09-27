// pssamp.h - sample proxy / stub header

#define IMETHOD_Method1 0
#define IMETHOD_PassUnk 1
#define IMETHOD_GetUnk 2

// interface proxy for IOLE2DevConfSample; always aggregated
class FAR CPrxSample : public IOLE2DevConfSample
{
public:
	static IRpcProxy FAR* Create(IUnknown FAR* pUnkOuter);

	// IUnknown methods (for external interface(s))
	STDMETHOD(QueryInterface) ( REFIID iid, LPVOID FAR* ppvObj);
	STDMETHOD_(ULONG,AddRef) (void);
	STDMETHOD_(ULONG,Release) (void);

	// IOLE2DevConfSample methods
	STDMETHOD(Method1) (DWORD dw);
	STDMETHOD(PassUnk) (THIS_ LPUNKNOWN lpUnk);
	STDMETHOD(GetUnk) (THIS_ LPUNKNOWN FAR* lpUnk);

private:
	CPrxSample(IUnknown FAR* pUnkOuter);
	~CPrxSample();


	// IRpcProxy which is also the controlling unknown
	struct CProxyImpl : IRpcProxy
	{
	public:
		CProxyImpl(CPrxSample FAR* pPrxSample)
		{ m_pPrxSample = pPrxSample; }

		STDMETHOD(QueryInterface)(REFIID iid, LPVOID FAR* ppv);
		STDMETHOD_(ULONG,AddRef)(void);
		STDMETHOD_(ULONG,Release)(void);

		STDMETHOD(Connect)(IRpcChannel FAR* pRpcChannel);
		STDMETHOD_(void, Disconnect)(void);

	private:
		CPrxSample FAR* m_pPrxSample;
	};
	friend CProxyImpl;

	CProxyImpl m_Proxy;


// private state:
	ULONG               m_refs;
	IUnknown FAR*       m_pUnkOuter;
	IRpcChannel FAR*    m_pRpcChannel;
};




// interface stub for IOLE2DevConfSample
class FAR CStubSample : public IRpcStub
{
public:
	static HRESULT Create(IUnknown FAR* pUnkObject, IRpcStub FAR* FAR* ppStub);

	STDMETHOD(QueryInterface) (REFIID iid, LPVOID FAR* ppv);
	STDMETHOD_(ULONG,AddRef) (void);
	STDMETHOD_(ULONG,Release) (void);

	STDMETHOD(Connect)(IUnknown FAR* pUnkObject);
	STDMETHOD_(void, Disconnect)(void);
	STDMETHOD(Invoke)(REFIID iid, int iMethod, IStream FAR* pStream,
			DWORD dwDestCtx, LPVOID lpvDestCtx);
	STDMETHOD_(BOOL, IsIIDSupported)(REFIID iid);
	STDMETHOD_(ULONG, CountRefs)(void);

private:
	CStubSample(void);
	~CStubSample(void);

	ULONG               m_refs;

	IOLE2DevConfSample FAR* m_pODCSample;
};



// Proxy/Stub Factory for pssamp.dll: supports IPSFactory only.
class FAR CPSFactory : public IPSFactory
{
public:
	CPSFactory();

	STDMETHOD(QueryInterface)(REFIID iid, LPVOID FAR* ppv);
	STDMETHOD_(ULONG,AddRef)(void);
	STDMETHOD_(ULONG,Release)(void);

	STDMETHOD(CreateProxy)(IUnknown FAR* pUnkOuter, REFIID iid,
		IRpcProxy FAR* FAR* ppProxy, LPVOID FAR* ppv);
	STDMETHOD(CreateStub)(REFIID iid, IUnknown FAR* pUnkServer,
		IRpcStub FAR* FAR* ppStub);

private:
	ULONG   m_refs;
};
