

class CRealtimeItem :  public CControllingUnknown
{
public:
	CRealtimeItem( LPUNKNOWN pUnkOuter, CRTContainer FAR* pContainer );
	~CRealtimeItem( void );

	// *** IUnknown method overrides ***
	STDMETHOD(QueryInterface) (REFIID riid, LPVOID FAR* ppvObj);

	//  non-virtual methods
	BOOL Iterate(void);
	CString Eighths(void);

private:
	class CDataObject :  IDataObject
	{
	public:
		CDataObject( CRealtimeItem FAR * pRealtimeItem );
		STDMETHOD(QueryInterface) (REFIID riid, LPVOID FAR* ppvObj);
		STDMETHOD_(ULONG,AddRef) (THIS);
		STDMETHOD_(ULONG,Release) (THIS);

		// *** IDataObject methods ***
		STDMETHOD(GetData) (LPFORMATETC pformatetcIn,
							LPSTGMEDIUM pmedium );
		STDMETHOD(GetDataHere) (LPFORMATETC pformatetc,
							LPSTGMEDIUM pmedium );
		STDMETHOD(QueryGetData) (LPFORMATETC pformatetc );
		STDMETHOD(GetCanonicalFormatEtc) (LPFORMATETC pformatetc,
							LPFORMATETC pformatetcOut);
		STDMETHOD(SetData) (LPFORMATETC pformatetc, STGMEDIUM FAR * pmedium,
							BOOL fRelease);
		STDMETHOD(EnumFormatEtc) (DWORD dwDirection,
							LPENUMFORMATETC FAR* ppenumFormatEtc);

		STDMETHOD(DAdvise) (FORMATETC FAR* pFormatetc, DWORD advf,
					LPADVISESINK pAdvSink, DWORD FAR* pdwConnection);
		STDMETHOD(DUnadvise) (DWORD dwConnection);
		STDMETHOD(EnumDAdvise) (LPENUMSTATDATA FAR* ppenumAdvise);

		CRealtimeItem FAR * m_pRealtimeItem;
	};
	friend class CDataObject;
	CDataObject m_dataObject;


	IDataAdviseHolder FAR* m_pAdvHolder;
	CRTContainer FAR* m_pContainer;
	int m_nLastValue;       //  last value, in eighths
	int m_nJitter;          //  interval within which to choose random delta
	int m_nPeriod;          //  number of seconds per update
	int m_cSeconds;     //  count of seconds in this period.
};

struct RTItemData
{
	int nLastValue;
	int nJitter;
	int nPeriod;
};
