//**********************************************************************
// File name: app.h
//
//      Definition of CSimpSvrApp
//
// Copyright (c) 1993 - 1995 Microsoft Corporation. All rights reserved.
//**********************************************************************

#ifndef _APP_H_
#define _APP_H_

class CSimpSvrDoc;                                                                        
interface CClassFactory;

class CSimpSvrApp : public IUnknown
{            
private:
    int m_cRef;                 // reference count
    HWND m_hAppWnd;             // main window handle
    HINSTANCE m_hInst;          // application instance
    BOOL m_fStartByOle;         // TRUE if app started by OLE
    DWORD m_dwRegisterClass;    // returned by RegisterClassFactory

    HMENU m_hMainMenu;
    HMENU m_hColorMenu;
    HMENU m_hHelpMenu;

    LPOLEOBJECT m_pOleObject;   // pointer to "dummy" object

    CSimpSvrDoc* m_pDoc;        // pointer to document object
    BOOL m_fInitialized;        // OLE initialization flag

    RECT nullRect;              // used in inplace negotiation        

public:    
    // IUnknown Interfaces    
    STDMETHODIMP QueryInterface(REFIID riid, LPVOID* ppvObj);
    STDMETHODIMP_(ULONG) AddRef();
    STDMETHODIMP_(ULONG) Release(); 

    // Initialization methods
    
    CSimpSvrApp();           // Constructor
    ~CSimpSvrApp();          // Destructor

    BOOL fInitApplication(HINSTANCE hInstance, int nCmdShow);

    // Message handling methods

    long lCommandHandler(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
    long lSizeHandler(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
    long lCreateDoc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
    void PaintApp(HDC hDC);                                    

    //  Utility functions    
    void ParseCmdLine(LPSTR lpCmdLine);
    void SetStatusText();
    BOOL IsInPlaceActive();
    void ShowAppWnd(int nCmdShow = SW_SHOWNORMAL);
    void HideAppWnd();

    // member variable access
    inline HWND GethAppWnd() { return m_hAppWnd; };
    inline HINSTANCE GethInst() { return m_hInst; };
    inline BOOL IsStartedByOle() { return m_fStartByOle; };
    inline BOOL IsInitialized() { return m_fInitialized; };
    inline DWORD GetRegisterClass() { return m_dwRegisterClass; };
    inline CSimpSvrDoc* GetDoc() { return m_pDoc; };
    inline void ClearDoc() { m_pDoc = NULL; };
    inline LPOLEOBJECT GetOleObject() { return m_pOleObject; };
    inline HMENU GetMainMenu() { return m_hMainMenu; };
    inline HMENU GetColorMenu() { return m_hColorMenu; };
    inline HMENU GetHelpMenu() { return m_hHelpMenu; } ;

    friend interface CClassFactory;  // make the contained class a friend
};

#endif
