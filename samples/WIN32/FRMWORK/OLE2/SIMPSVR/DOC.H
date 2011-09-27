//**********************************************************************
// File name: doc.h
//
//      Definition of CSimpSvrDoc
//
// Copyright (c) 1993 - 1995 Microsoft Corporation. All rights reserved.
//**********************************************************************

#if !defined( _DOC_H_ )
#define _DOC_H_

class CSimpSvrApp;
class CSimpSvrObj;

class CSimpSvrDoc : IUnknown 
{   
private:                  
    UINT m_cRef;  

    CSimpSvrApp* m_pApp;
    CSimpSvrObj* m_pObj;

    HWND m_hDocWnd;
    HWND m_hHatchWnd; 
    BOOL m_fClosing;

public:
    static CSimpSvrDoc* Create(CSimpSvrApp* lpApp, LPRECT lpRect, HWND hWnd);

    CSimpSvrDoc();
    CSimpSvrDoc(CSimpSvrApp* lpApp, HWND hWnd);
    ~CSimpSvrDoc();

    // IUnknown Interfaces    
    STDMETHODIMP QueryInterface(REFIID riid, LPVOID* ppv);
    STDMETHODIMP_(ULONG) AddRef();
    STDMETHODIMP_(ULONG) Release(); 

    long lResizeDoc(LPRECT lpRect);
    long lAddVerbs();

    BOOL Load(LPSTR lpszFileName);
    void PaintDoc(HDC hDC);
    void lButtonDown(WPARAM wParam, LPARAM lParam);

    HRESULT CreateObject(REFIID riid, LPVOID* ppv);

    void Close();
    void SetStatusText();
    void ShowDocWnd();
    void ShowHatchWnd();
    void HideDocWnd();
    void HideHatchWnd();

    // member access    
    inline HWND GethDocWnd() { return m_hDocWnd; }
    inline HWND GethHatchWnd() { return m_hHatchWnd; }
    inline HWND GethAppWnd() { return m_pApp->GethAppWnd(); }
    inline CSimpSvrApp* GetApp() { return m_pApp; }
    inline CSimpSvrObj* GetObj() { return m_pObj; }
    inline void ClearObj() { m_pObj = NULL; }
};

#endif
