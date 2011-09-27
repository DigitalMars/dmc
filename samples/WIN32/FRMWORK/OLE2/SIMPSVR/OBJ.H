//**********************************************************************
// File name: obj.h
//
//      Definition of CSimpSvrObj
//
// Copyright (c) 1993 - 1995 Microsoft Corporation. All rights reserved.
//**********************************************************************

#if !defined( _OBJ_H_)
#define _OBJ_H_

// CSimpSvrObj's CLSID
DEFINE_GUID(GUID_SIMPLESVR32, 0xC53E3F21, 0xF05B, 0x1069, 0xBE, 0x3F, 0x00, 0xDD, 0x01, 0x10, 0x8B, 0x8C);


#define SCALE_BASE  128
#define SCALE_MIN   (SCALE_BASE/8)

class CSimpSvrDoc;

class CSimpSvrObj : public IOleObject,
                           IPersistStorage,
                           IDataObject,
                           IOleInPlaceActiveObject,
                           IOleInPlaceObject,
                           IExternalConnection
{
private:
    UINT m_cRef;                    // Reference count
    CSimpSvrDoc* m_pDoc;            // Back pointer
    BOOL m_fInPlaceActive;          // Used during InPlace Negotiation
    BOOL m_fInPlaceVisible;         // "  "  "  "   "   "   "   "   "
    BOOL m_fUIActive;               // "  "  "  "   "   "   "   "   "
    HMENU m_hmenuShared;            // "  "  "  "   "   "   "   "   "
    HOLEMENU m_hOleMenu;            // "  "  "  "   "   "   "   "   "
    OLEINPLACEFRAMEINFO m_FrameInfo;
    BOOL m_fSaveWithSameAsLoad;
    BOOL m_fNoScribbleMode;
    BOOL m_fOpen;
    DWORD m_dwStrong;               // IExternalConnection lock count
    DWORD m_dwRegister;             // Registered in ROT

    int m_red, m_green, m_blue;     // current color
    POINT m_ptSize;                 // current size
    POINT m_ptOffset;
    POINT m_ptScale;

    HWND m_hWndParent;              // parent window handle

    // interfaces used
    LPSTORAGE m_pStorage;
    LPSTREAM m_pColorStm, m_pSizeStm;
    LPOLECLIENTSITE m_pOleClientSite;          // IOleClientSite
    LPOLEADVISEHOLDER m_pOleAdviseHolder;      // IOleAdviseHolder
    LPDATAADVISEHOLDER m_pDataAdviseHolder;    // IDataAdviseHolder
    LPOLEINPLACEFRAME m_pFrame;                // IOleInPlaceFrame
    LPOLEINPLACEUIWINDOW m_pCntrDoc;           // IOleInPlaceUIWindow
    LPOLEINPLACESITE m_pIPSite;                // IOleInPlaceSite

public:
    // construction/destruction
    CSimpSvrObj(CSimpSvrDoc* pDoc);
    ~CSimpSvrObj();

    // IUnknown methods
    STDMETHODIMP QueryInterface(REFIID riid, LPVOID* ppv);
    STDMETHODIMP_(ULONG) AddRef();
    STDMETHODIMP_(ULONG) Release();

    // IOleObject methods
    STDMETHODIMP SetClientSite(LPOLECLIENTSITE pClientSite);
    STDMETHODIMP GetClientSite(LPOLECLIENTSITE* ppClientSite);
    STDMETHODIMP SetHostNames(LPCOLESTR szContainerApp,
                              LPCOLESTR szContainerObj);
    STDMETHODIMP Close(DWORD dwSaveOption);
    STDMETHODIMP SetMoniker(DWORD dwWhichMoniker, LPMONIKER pmk);
    STDMETHODIMP GetMoniker(DWORD dwAssign,
                            DWORD dwWhichMoniker,
                            LPMONIKER* ppmk);
    STDMETHODIMP InitFromData(LPDATAOBJECT pDataObject,
                              BOOL fCreation,
                              DWORD dwReserved);
    STDMETHODIMP GetClipboardData(DWORD dwReserved,
                                  LPDATAOBJECT* ppDataObject);
    STDMETHODIMP DoVerb(LONG iVerb,
                        LPMSG lpmsg,
                        LPOLECLIENTSITE pActiveSite,
                        LONG lindex,
                        HWND hwndParent,
                        LPCRECT lprcPosRect);
    STDMETHODIMP EnumVerbs(LPENUMOLEVERB* ppenumOleVerb);
    STDMETHODIMP Update();
    STDMETHODIMP IsUpToDate();
    STDMETHODIMP GetUserClassID(LPCLSID pClsid);
    STDMETHODIMP GetUserType(DWORD dwFormOfType, LPOLESTR* pszUserType);
    STDMETHODIMP SetExtent(DWORD dwDrawAspect, LPSIZEL lpsizel);
    STDMETHODIMP GetExtent(DWORD dwDrawAspect, LPSIZEL lpsizel);

    STDMETHODIMP Advise(LPADVISESINK pAdvSink, LPDWORD pdwConnection);
    STDMETHODIMP Unadvise(DWORD dwConnection);
    STDMETHODIMP EnumAdvise(LPENUMSTATDATA* ppenumAdvise);
    STDMETHODIMP GetMiscStatus(DWORD dwAspect, LPDWORD pdwStatus);
    STDMETHODIMP SetColorScheme(LPLOGPALETTE lpLogpal);

    void OpenEdit(LPOLECLIENTSITE pActiveSite);

    // IPersistStorage methods
    STDMETHODIMP InitNew(LPSTORAGE pStg);
    STDMETHODIMP GetClassID(LPCLSID lpClassID);
    STDMETHODIMP Save(LPSTORAGE pStg, BOOL fSameAsLoad);
    STDMETHODIMP SaveCompleted(LPSTORAGE pStgNew);
    STDMETHODIMP Load(LPSTORAGE pStg);
    STDMETHODIMP IsDirty();
    STDMETHODIMP HandsOffStorage();
    
    // IDataObject methods
    STDMETHODIMP GetData(LPFORMATETC pformatetcIn, LPSTGMEDIUM pmedium);
    STDMETHODIMP GetDataHere(LPFORMATETC pformatetc, LPSTGMEDIUM pmedium);
    STDMETHODIMP QueryGetData(LPFORMATETC pformatetc);
    STDMETHODIMP GetCanonicalFormatEtc(LPFORMATETC pformatetc,
                                       LPFORMATETC pformatetcOut);
    STDMETHODIMP SetData(LPFORMATETC pformatetc,
                         LPSTGMEDIUM pmedium,
                         BOOL fRelease);
    STDMETHODIMP EnumFormatEtc(DWORD dwDirection,
                               LPENUMFORMATETC* ppenumFormatEtc);
    STDMETHODIMP DAdvise(LPFORMATETC pFormatetc,
                         DWORD advf,
                         LPADVISESINK pAdvSink,
                         LPDWORD pdwConnection);
    STDMETHODIMP DUnadvise(DWORD dwConnection);
    STDMETHODIMP EnumDAdvise(LPENUMSTATDATA* ppenumAdvise);

    // IOleWindow methods (in IOIPAO.CPP)
    STDMETHODIMP GetWindow(HWND* lphwnd);
    STDMETHODIMP ContextSensitiveHelp(BOOL fEnterMode);

    // IOleInPlaceActiveObject methods
    STDMETHODIMP TranslateAccelerator(LPMSG lpmsg);
    STDMETHODIMP OnFrameWindowActivate(BOOL fActivate);
    STDMETHODIMP OnDocWindowActivate(BOOL fActivate);
    STDMETHODIMP ResizeBorder(LPCRECT lprectBorder,
                              LPOLEINPLACEUIWINDOW lpUIWindow,
                              BOOL fFrameWindow);
    STDMETHODIMP EnableModeless(BOOL fEnable);

    // IOleInPlaceObject methods
    STDMETHODIMP InPlaceDeactivate();
    STDMETHODIMP UIDeactivate();
    STDMETHODIMP SetObjectRects(LPCRECT lprcPosRect, LPCRECT lprcClipRect);
    STDMETHODIMP ReactivateAndUndo();

    // IExternalConnection methods
    STDMETHODIMP_(DWORD) AddConnection(DWORD extconn, DWORD reserved);
    STDMETHODIMP_(DWORD) ReleaseConnection(DWORD extconn, DWORD reserved, BOOL fLastReleaseCloses);

    // utility functions
    void Draw(HDC hDC, BOOL fMetaDC = TRUE);
    void PaintObj(HDC hDC);
    void lButtonDown(WPARAM wParam,LPARAM lParam);
    HMETAFILEPICT GetMetaFilePict();

    // visual editing helper functions
    BOOL DoInPlaceActivate(LONG lVerb);
    void AssembleMenus();
    void AddFrameLevelUI();
    void DoInPlaceHide();
    void DisassembleMenus();
    void SendOnDataChange();

    // member variable access
    BOOL IsInPlaceActive() { return m_fInPlaceActive; };
    BOOL IsInPlaceVisible() { return m_fInPlaceVisible; };
    BOOL IsUIActive() { return m_fUIActive; };
    HWND GetParent() { return m_hWndParent; };
    LPSTORAGE GetStorage() { return m_pStorage; };
    LPOLECLIENTSITE GetOleClientSite() { return m_pOleClientSite; };
    LPDATAADVISEHOLDER GetDataAdviseHolder() { return m_pDataAdviseHolder; };
    LPOLEADVISEHOLDER GetOleAdviseHolder() { return m_pOleAdviseHolder; };
    LPOLEINPLACEFRAME GetInPlaceFrame() { return m_pFrame; };
    LPOLEINPLACEUIWINDOW GetUIWindow() { return m_pCntrDoc; };
    LPOLEINPLACESITE GetInPlaceSite() { return m_pIPSite; };
    LPOLEOBJECT GetOleObject() { return (LPOLEOBJECT)this; };
    LPPERSISTSTORAGE GetPersistStorage() { return (LPPERSISTSTORAGE)this; };
    LPDATAOBJECT GetDataObject() { return (LPDATAOBJECT)this; };
    LPOLEINPLACEACTIVEOBJECT GetOleInPlaceActiveObject() { return (LPOLEINPLACEACTIVEOBJECT)this; };
    LPOLEINPLACEOBJECT GetOleInPlaceObject() { return (LPOLEINPLACEOBJECT)this; };
    void ClearOleClientSite() { m_pOleClientSite = NULL; };
    void ClearDataAdviseHolder() { m_pDataAdviseHolder = NULL; };
    void ClearOleAdviseHolder() { m_pOleAdviseHolder = NULL; };
    LPPOINT GetSize() { return &m_ptSize; };
    LPOLEINPLACEFRAMEINFO GetFrameInfo() { return &m_FrameInfo; };
    DWORD GetRotRegister() { return m_dwRegister; };

    // member manipulation
    void SetColor(int nRed, int nGreen, int nBlue)
        { m_red = nRed; m_green = nGreen; m_blue = nBlue; };

    void RotateColor()
        { m_red += 10; m_green += 10; m_blue += 10;};
};

#endif // _OBJ_H_
