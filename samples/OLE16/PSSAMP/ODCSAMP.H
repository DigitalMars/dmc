// odcsamp.h - sample interface

#undef INTERFACE
#define INTERFACE   IOLE2DevConfSample

DECLARE_INTERFACE_(IOLE2DevConfSample, IUnknown)
{
	// *** IUnknown methods ***
	STDMETHOD(QueryInterface) (THIS_ REFIID riid, LPVOID FAR* ppvObj) PURE;
	STDMETHOD_(ULONG,AddRef) (THIS)  PURE;
	STDMETHOD_(ULONG,Release) (THIS) PURE;

	// *** IOLE2DevConfSample methods ***
	STDMETHOD(Method1) (THIS_ DWORD dw) PURE;
	STDMETHOD(PassUnk) (THIS_ LPUNKNOWN lpUnk) PURE;
	STDMETHOD(GetUnk) (THIS_ LPUNKNOWN FAR* lpUnk) PURE;
};
typedef         IOLE2DevConfSample FAR* LPOLE2DEVCONFSAMPLE;


DEFINE_GUID(IID_IOLE2DevConfSample, 0xD0B01C60, 0x9801,
	0x1068, 0xAC, 0x1D, 0x00, 0x80, 0xC7, 0x0E, 0x55, 0x04);
