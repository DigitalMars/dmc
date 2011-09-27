

class CRealtimeItem :  public CControllingUnknown
{
public:
	CRealtimeItem( LPUNKNOWN pUnkOuter );
	~CRealtimeItem( void );

	// *** IUnknown method overrides ***
	STDMETHOD(QueryInterface) (THIS_ REFIID riid, LPVOID FAR* ppvObj);

	class CDataObject :  IDataObject
	{
	public:
		CDataObject( CRealtimeItem FAR * pRealtimeItem );
		STDMETHOD(QueryInterface) (THIS_ REFIID riid, LPVOID FAR* ppvObj);

		// *** IDataObject methods ***
		STDMETHOD(GetData) (THIS_ LPFORMATETC pformatetcIn,
							LPSTGMEDIUM pmedium );
		STDMETHOD(GetDataHere) (THIS_ LPFORMATETC pformatetc,
							LPSTGMEDIUM pmedium );
		STDMETHOD(QueryGetData) (THIS_ LPFORMATETC pformatetc );
		STDMETHOD(GetCanonicalFormatEtc) (THIS_ LPFORMATETC pformatetc,
							LPFORMATETC pformatetcOut);
		STDMETHOD(SetData) (THIS_ LPFORMATETC pformatetc, STGMEDIUM FAR * pmedium,
							BOOL fRelease);
		STDMETHOD(EnumFormatEtc) (THIS_ DWORD dwDirection,
							LPENUMFORMATETC FAR* ppenumFormatEtc);

		STDMETHOD(DAdvise) (THIS_ FORMATETC FAR* pFormatetc, DWORD advf,
					LPADVISESINK pAdvSink, DWORD FAR* pdwConnection);
		STDMETHOD(DUnadvise) (THIS_ DWORD dwConnection);
		STDMETHOD(EnumDAdvise) (THIS_ LPENUMSTATDATA FAR* ppenumAdvise);

		CRealtimeItem FAR * m_pRealtimeItem;
	};
	friend class CDataObject;
	CDataObject m_dataObject;

	IDataAdviseHolder FAR* m_pAdvHolder;
	int m_nLastValue;
	int m_nJitter;
};
