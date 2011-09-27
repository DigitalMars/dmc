// This is a part of the Microsoft Foundation Classes C++ library.
// Copyright (C) 1993 Microsoft Corporation
// All rights reserved.
//
// This source code is only intended as a supplement to the
// Microsoft Foundation Classes Reference and Microsoft
// QuickHelp and/or WinHelp documentation provided with the library.
// See these sources for detailed information regarding the
// Microsoft Foundation Classes product.

// NOTE: Must include AFXOLE.H first

/////////////////////////////////////////////////////////////////////////////
// Special OLE_TRACEx macros that check afxTraceFlags

#ifdef _DEBUG
// special OLE traces
#define OLE_TRACE0(string) \
	do { if (afxTraceFlags & 0x10) TRACE0(string); } while (0)
#define OLE_TRACE1(string, p1) \
	do { if (afxTraceFlags & 0x10) TRACE1(string, p1); } while (0)
#define OLE_TRACE2(string, p1, p2) \
	do { if (afxTraceFlags & 0x10) TRACE2(string, p1, p2); } while (0)
#else
// traces expand to nothing
#define OLE_TRACE0  TRACE0
#define OLE_TRACE1  TRACE1
#define OLE_TRACE2  TRACE2
#endif

/////////////////////////////////////////////////////////////////////////////
// COleFrameHook - AFX_INTERNAL

class COleFrameHook : public CCmdTarget
{
// Construction & Destruction
public:
	COleFrameHook(CFrameWnd* pFrameWnd, COleClientItem* pItem);
	~COleFrameHook();

// Implementation
public:
	CFrameWnd* m_pFrameWnd;
	LPOLEINPLACEACTIVEOBJECT m_lpActiveObject;
	COleClientItem* m_pActiveItem;  // item this COleFrameHook is for
	HWND m_hWnd;            // actual HWND this hook is attached to
	BOOL m_bInModalState;   // TRUE if EnableModeless(FALSE) has been called
	BOOL m_bToolBarHidden;  // TRUE if toolbar needs to be shown OnUIDeactivate
	HACCEL m_hAccelTable;   // accelerator to be used while in-place object active
	UINT m_nModelessCount;  // !0 if server's EnableModeless has been called
	CString m_strObjName;   // name of the active in-place object

// Overrides for implementation
public:
	virtual void OnRecalcLayout();  // for border space re-negotiation
	virtual BOOL OnPreTranslateMessage(MSG* pMsg);
	virtual void OnActivate(BOOL bActive); // for OnFrameWindowActivate
	virtual BOOL OnDocActivate(BOOL bActive);   // for OnDocWindowActivate
	virtual BOOL OnContextHelp(BOOL bEnter);
	virtual void OnEnableModeless(BOOL bEnable);
	virtual BOOL OnUpdateFrameTitle();

	// implementation helpers
	BOOL NotifyAllInPlace(
		BOOL bParam, BOOL (COleFrameHook::*pNotifyFunc)(BOOL bParam));
	BOOL DoContextSensitiveHelp(BOOL bEnter);
	BOOL DoEnableModeless(BOOL bEnable);

// Interface Maps
public:
	BEGIN_INTERFACE_PART(OleInPlaceUIWindow, IOleInPlaceUIWindow)
		STDMETHOD(GetWindow)(HWND FAR*);
		STDMETHOD(ContextSensitiveHelp)(BOOL);
		STDMETHOD(GetBorder)(LPRECT);
		STDMETHOD(RequestBorderSpace)(LPCBORDERWIDTHS);
		STDMETHOD(SetBorderSpace)(LPCBORDERWIDTHS);
		STDMETHOD(SetActiveObject)(LPOLEINPLACEACTIVEOBJECT, LPCSTR);
	END_INTERFACE_PART(OleInPlaceUIWindow)

	BEGIN_INTERFACE_PART(OleInPlaceFrame, IOleInPlaceFrame)
		STDMETHOD(GetWindow)(HWND FAR*);
		STDMETHOD(ContextSensitiveHelp)(BOOL);
		STDMETHOD(GetBorder)(LPRECT);
		STDMETHOD(RequestBorderSpace)(LPCBORDERWIDTHS);
		STDMETHOD(SetBorderSpace)(LPCBORDERWIDTHS);
		STDMETHOD(SetActiveObject)(LPOLEINPLACEACTIVEOBJECT, LPCSTR);
		STDMETHOD(InsertMenus)(HMENU, LPOLEMENUGROUPWIDTHS);
		STDMETHOD(SetMenu)(HMENU, HOLEMENU, HWND);
		STDMETHOD(RemoveMenus)(HMENU);
		STDMETHOD(SetStatusText)(LPCSTR);
		STDMETHOD(EnableModeless)(BOOL);
		STDMETHOD(TranslateAccelerator)(LPMSG, WORD);
	END_INTERFACE_PART(OleInPlaceFrame)

	DECLARE_INTERFACE_MAP()

	friend COleClientItem;
};

/////////////////////////////////////////////////////////////////////////////
// Helper for implementing OLE enumerators

// Note: the following interface is not an actual OLE interface, but is useful
//  for describing an abstract (not typesafe) enumerator.

#undef  INTERFACE
#define INTERFACE   IEnumVOID

DECLARE_INTERFACE_(IEnumVOID, IUnknown)
{
	STDMETHOD(QueryInterface)(REFIID, LPVOID FAR*) PURE;
	STDMETHOD_(ULONG,AddRef)()  PURE;
	STDMETHOD_(ULONG,Release)() PURE;
	STDMETHOD(Next)(ULONG, void FAR*, ULONG FAR*) PURE;
	STDMETHOD(Skip)(ULONG) PURE;
	STDMETHOD(Reset)() PURE;
	STDMETHOD(Clone)(IEnumVOID FAR* FAR*) PURE;
};

class CEnumArray : public CCmdTarget
{
// Constructors
public:
	CEnumArray(size_t nSize, REFIID iidEnum,
		const void FAR* pvEnum, UINT nCount, BOOL bNeedFree = FALSE);

// Implementation
public:
	virtual ~CEnumArray();

protected:
	size_t m_nSizeElem;     // size of each item in the array
	REFIID m_iidEnum;       // enumerator IID (used in QueryInterface)
	CCmdTarget* m_pClonedFrom;  // used to keep original alive for clones

	char FAR* m_pvEnum; // pointer data to enumerate
	UINT m_nCurPos;     // current position in m_pvEnum
	UINT m_nSize;       // total number of items in m_pvEnum
	BOOL m_bNeedFree;   // free on release?

#ifdef _AFXCTL
	AFX_APPDATA_MODULE* m_pModuleState;
#endif

	virtual BOOL OnNext(void FAR* pv);
	virtual BOOL OnSkip();
	virtual void OnReset();
	virtual CEnumArray* OnClone();

// Interface Maps
public:
	BEGIN_INTERFACE_PART(EnumVOID, IEnumVOID)
		STDMETHOD(Next)(ULONG, void FAR*, ULONG FAR*);
		STDMETHOD(Skip)(ULONG);
		STDMETHOD(Reset)();
		STDMETHOD(Clone)(IEnumVOID FAR* FAR*);
	END_INTERFACE_PART(EnumVOID)
};

/////////////////////////////////////////////////////////////////////////////
// COleDispatchImpl - IDispatch implementation

// Note: This class is only designed to be used as a CCmdTarget member
//  (at the offset specified by CCmdTarget::m_xDispatch))
// It WILL NOT work in other classes or at different offsets!

class FAR COleDispatchImpl : public IDispatch
{
public:
	/* IUnknown methods */
	STDMETHOD_(ULONG, AddRef)();
	STDMETHOD_(ULONG, Release)();
	STDMETHOD(QueryInterface)(REFIID iid, LPVOID far* ppvObj);

	/* IDispatch methods */
	STDMETHOD(GetTypeInfoCount)(UINT FAR* pctinfo);
	STDMETHOD(GetTypeInfo)(UINT itinfo, LCID lcid,
		ITypeInfo FAR* FAR* pptinfo);
	STDMETHOD(GetIDsOfNames)(REFIID riid,
		char FAR* FAR* rgszNames, UINT cNames, LCID lcid,
		DISPID FAR* rgdispid);
	STDMETHOD(Invoke)(DISPID dispidMember, REFIID riid, LCID lcid,
		WORD wFlags, DISPPARAMS FAR* pdispparams, VARIANT FAR* pvarResult,
		EXCEPINFO FAR* pexcepinfo, UINT FAR* puArgErr);

	// special method for disconnect
	virtual void Disconnect();
};

/////////////////////////////////////////////////////////////////////////////
// OLE 2.0 data (like AUX_DATA)

#ifdef AFX_CLASS_MODEL
struct NEAR OLE_DATA
#else
struct OLE_DATA
#endif
{
	// OLE 1.0 clipboard formats
	UINT    cfNative, cfOwnerLink, cfObjectLink;

	// OLE 2.0 clipboard formats
	UINT    cfEmbeddedObject, cfEmbedSource, cfLinkSource;
	UINT    cfObjectDescriptor, cfLinkSourceDescriptor;

	OLE_DATA();
};

extern OLE_DATA NEAR _oleData;

/////////////////////////////////////////////////////////////////////////////
// Global helper functions

// menu merging/unmerging
void AFXAPI _AfxMergeMenus(CMenu* pMenuShared, CMenu* pMenuSource,
	LONG FAR* lpMenuWidths, int iWidthIndex);
void AFXAPI _AfxUnmergeMenus(CMenu* pMenuShared, CMenu* pMenuSource);

// helpers for exceptions
void AFXAPI _AfxFillOleFileException(CFileException*, HRESULT hr);
void AFXAPI _AfxThrowOleFileException(HRESULT hr);

// helper used during object creation
LPFORMATETC AFXAPI _AfxFillFormatEtc(LPFORMATETC lpFormatEtc,
	CLIPFORMAT cfFormat, LPFORMATETC lpFormatEtcFill);

// helper to copy clipboard data
BOOL AFXAPI _AfxCopyStgMedium(
	CLIPFORMAT cfFormat, LPSTGMEDIUM lpDest, LPSTGMEDIUM lpSource);

// helper for reliable and small QueryInterface calls
LPUNKNOWN AFXAPI _AfxQueryInterface(LPUNKNOWN lpUnknown, REFIID riid);
DWORD AFXAPI _AfxRelease(LPUNKNOWN* plpUnknown);

// use this macro to do QueryInterface and downcast all at once
#define _AFXQUERYINTERFACE(lpUnknown, iface) \
	(iface FAR*)_AfxQueryInterface(lpUnknown, IID_##iface)

void FAR* AFXAPI _AfxAllocTaskMem(size_t nSize);
void AFXAPI _AfxFreeTaskMem(void FAR* p);

/////////////////////////////////////////////////////////////////////////////
// language specific DLL based one OLE locale ID

#ifdef _AFXCTL
void AFXAPI _AfxSetResourceLocale(LCID lcid);
#endif

/////////////////////////////////////////////////////////////////////////////
// implementation types and constants

#define OLE_MAXITEMNAME (sizeof("Embedding ")+sizeof("4294967295")-sizeof(""))

typedef LPVOID FAR* LPLP;

/////////////////////////////////////////////////////////////////////////////
