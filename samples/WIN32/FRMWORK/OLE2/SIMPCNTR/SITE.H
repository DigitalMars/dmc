//**********************************************************************
// File name: SITE.H
//
//      Definition of CSimpleSite
//
// Copyright (c) 1992-1995 Microsoft Corporation. All rights reserved.
//**********************************************************************
#if !defined( _SITE_H_ )
#define _SITE_H_

class CSimpleDoc;

class CSimpleSite : public IAdviseSink, IOleInPlaceSite, IOleClientSite
{
public:
    int m_nCount;
    DWORD m_dwConnection;
    LPOLEOBJECT m_lpOleObject;
    LPOLEINPLACEOBJECT m_lpInPlaceObject;
    HWND m_hwndIPObj;
    DWORD m_dwDrawAspect;
    SIZEL m_sizel;
    BOOL m_fInPlaceActive;
    BOOL m_fObjectOpen;
    LPSTORAGE m_lpObjStorage;

    CSimpleDoc *m_lpDoc;

    // construction/initialization
    CSimpleSite(CSimpleDoc *lpDoc);
    ~CSimpleSite();

    static CSimpleSite* Create(CSimpleDoc *lpDoc);
    void InitObject(BOOL fCreateNew);

    // IUnknown methods
    STDMETHODIMP QueryInterface(REFIID riid, LPVOID* ppv);
    STDMETHODIMP_(ULONG) AddRef();
    STDMETHODIMP_(ULONG) Release();

    // IAdviseSink methods
    STDMETHODIMP_(void) OnDataChange(LPFORMATETC pFormatEtc, LPSTGMEDIUM pStgMed);
    STDMETHODIMP_(void) OnViewChange(DWORD dwAspect, LONG lIndex);
    STDMETHODIMP_(void) OnRename(LPMONIKER pmk);
    STDMETHODIMP_(void) OnSave();
    STDMETHODIMP_(void) OnClose();

    // IOleWindow methods (in ioips.cpp)
    STDMETHODIMP GetWindow(HWND *lphwnd);
    STDMETHODIMP ContextSensitiveHelp(BOOL fEnterMode);

    // IOleInPlaceSite methods
    STDMETHODIMP CanInPlaceActivate();
    STDMETHODIMP OnInPlaceActivate();
    STDMETHODIMP OnUIActivate();
    STDMETHODIMP GetWindowContext(LPOLEINPLACEFRAME *lplpFrame,
                                  LPOLEINPLACEUIWINDOW *lplpDoc,
                                  LPRECT lprcPosRect,
                                  LPRECT lprcClipRect,
                                  LPOLEINPLACEFRAMEINFO lpFrameInfo);
    STDMETHODIMP Scroll(SIZE scrollExtent);
    STDMETHODIMP OnUIDeactivate(BOOL fUndoable);
    STDMETHODIMP OnInPlaceDeactivate();
    STDMETHODIMP DiscardUndoState();
    STDMETHODIMP DeactivateAndUndo();
    STDMETHODIMP OnPosRectChange(LPCRECT lprcPosRect);

    // IOleClientSite methods
    STDMETHODIMP SaveObject();
    STDMETHODIMP GetMoniker(DWORD dwAssign, DWORD dwWhichMoniker, LPMONIKER *ppmk);
    STDMETHODIMP GetContainer(LPOLECONTAINER *ppContainer);
    STDMETHODIMP ShowObject();
    STDMETHODIMP OnShowWindow(BOOL fShow);
    STDMETHODIMP RequestNewObjectLayout();

    // Interface access
    LPADVISESINK     GetAdviseSink()     { return (LPADVISESINK)this; }
    LPOLEINPLACESITE GetOleInPlaceSite() { return (LPOLEINPLACESITE)this; }
    LPOLECLIENTSITE  GetOleClientSite()  { return (LPOLECLIENTSITE)this; }

    // site helper functions
    void PaintObj(HDC hDC);
    void GetObjRect(LPRECT lpRect);
    void CloseOleObject();
    void UnloadOleObject();
};

#endif
