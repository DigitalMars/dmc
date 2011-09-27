//**********************************************************************
// File name: app.h
//
//      Definition of CSimpleApp
//
// Copyright (c) 1992-1995 Microsoft Corporation. All rights reserved.
//**********************************************************************

#if !defined( _APP_H_)
#define _APP_H_

class CSimpleDoc;

class CSimpleApp : public IOleInPlaceFrame
{
public:
    int         m_nCount;       // reference count
    HWND        m_hAppWnd;      // main window handle
    HINSTANCE   m_hInst;        // application instance
    HMENU       m_hMainMenu;
    HMENU       m_hFileMenu;
    HMENU       m_hEditMenu;
    HMENU       m_hHelpMenu;
    HMENU       m_hCascadeMenu;

    CSimpleDoc *m_lpDoc;        // pointer to document object
    BOOL        m_fInitialized; // OLE initialization flag
    BOOL        m_fCSHMode;
    BOOL        m_fMenuMode;
    HWND        m_hwndUIActiveObj;  // HWND of UIActive Object
    HPALETTE    m_hStdPal;      // Color palette used by container
    BOOL        m_fAppActive;   // TRUE if app is active

    CSimpleApp();               // Constructor
    ~CSimpleApp();              // Destructor
    RECT nullRect;

    // IUnknown methods
    STDMETHODIMP QueryInterface(REFIID riid, LPVOID* ppvObj);
    STDMETHODIMP_(ULONG) AddRef();
    STDMETHODIMP_(ULONG) Release();

    // IOleWindow methods
    STDMETHODIMP GetWindow(HWND *lphwnd);
    STDMETHODIMP ContextSensitiveHelp(BOOL fEnterMode);

    // IOleInPlaceUIWindow methods
    STDMETHODIMP GetBorder(LPRECT lprectBorder);
    STDMETHODIMP RequestBorderSpace(LPCBORDERWIDTHS lpborderwidths);
    STDMETHODIMP SetBorderSpace(LPCBORDERWIDTHS lpborderwidths);
    STDMETHODIMP SetActiveObject(LPOLEINPLACEACTIVEOBJECT lpActiveObject, LPCOLESTR lpszObjName);

    // IOleInPlaceFrame methods
    STDMETHODIMP InsertMenus(HMENU hmenuShared, LPOLEMENUGROUPWIDTHS lpMenuWidths);
    STDMETHODIMP SetMenu(HMENU hmenuShared, HOLEMENU holemenu, HWND hwndActiveObject);
    STDMETHODIMP RemoveMenus(HMENU hmenuShared);
    STDMETHODIMP SetStatusText(LPCOLESTR lpszStatusText);
    STDMETHODIMP EnableModeless(BOOL fEnable);
    STDMETHODIMP TranslateAccelerator(LPMSG lpmsg, WORD wID);

    // Initialization methods

    BOOL fInitApplication(HINSTANCE hInstance, int nCmdShow);

    // Message handling methods

    long lCommandHandler(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
    long lSizeHandler (HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
    long lCreateDoc (HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
    BOOL HandleAccelerators (LPMSG lpMsg);
    void PaintApp(HDC hDC);
    void DestroyDocs();

    // In-Place support functions
    void AddFrameLevelUI();
    void AddFrameLevelTools();
    LRESULT QueryNewPalette();
};

// helper function
LRESULT wSelectPalette(HWND hWnd, HPALETTE hPal, BOOL fBackground);

#endif
