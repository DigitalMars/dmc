
class CRTContainer :  public CControllingUnknown
{
public:
	CRTContainer( LPUNKNOWN pUnkOuter, CRealtimeDoc * pDoc );
	~CRTContainer( void );

	// *** IUnknown method override ***
	STDMETHOD(QueryInterface) (THIS_ REFIID riid, LPVOID FAR* ppvObj);
	STDMETHOD_(ULONG,Release) (THIS);

	class CItemContainer :  IOleItemContainer
	{
	public:
		CItemContainer( CRTContainer FAR * pRTContainer );
		STDMETHOD(QueryInterface) (THIS_ REFIID riid, LPVOID FAR* ppvObj);
		STDMETHOD_(ULONG,AddRef) (THIS);
		STDMETHOD_(ULONG,Release) (THIS);

		// *** IParseDisplayName method ***
		STDMETHOD(ParseDisplayName) (THIS_ LPBC pbc, LPSTR lpszDisplayName,
			ULONG FAR* pchEaten, LPMONIKER FAR* ppmkOut);

		// *** IOleContainer methods ***
		STDMETHOD(EnumObjects) (THIS_ DWORD grfFlags, LPENUMUNKNOWN FAR* ppenumUnknown);
		STDMETHOD(LockContainer) (THIS_ BOOL fLock);

		// *** IOleItemContainer methods ***
		STDMETHOD(GetObject) (THIS_ LPSTR lpszItem, DWORD dwSpeedNeeded,
			LPBINDCTX pbc, REFIID riid, LPVOID FAR* ppvObject);
		STDMETHOD(GetObjectStorage) (THIS_ LPSTR lpszItem, LPBINDCTX pbc,
			REFIID riid, LPVOID FAR* ppvStorage);
		STDMETHOD(IsRunning) (THIS_ LPSTR lpszItem);

	private:
		CRTContainer * m_pItemContainer;
	};
	friend class CItemContainer;
	CItemContainer m_ItemContainer;

	CRealtimeDoc * m_pDoc;
	UINT    cObjs;
	DWORD m_dwRegister;
public:
		//  non-virtual methods
		UINT Lock(void);
		UINT Unlock(void);
		BOOL Exitable(void);
		void Register(void);
		void Unregister(void);

};
