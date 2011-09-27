//**********************************************************************
// File name: doc.h
//
//      Definition of CSimpleDoc
//
// Copyright (c) 1992-1995 Microsoft Corporation. All rights reserved.
//**********************************************************************

#if !defined( _DOC_H_ )
#define _DOC_H_

class CSimpleSite;
class CSimpleApp;

class CSimpleDoc : public IUnknown
{
public:
    int             m_nCount;
    LPSTORAGE       m_lpStorage;
    LPOLEINPLACEACTIVEOBJECT m_lpActiveObject;
    BOOL            m_fInPlaceActive;
    BOOL            m_fAddMyUI;
    BOOL            m_fModifiedMenu;

    CSimpleSite    *m_lpSite;
    CSimpleApp     *m_lpApp;

    HWND m_hDocWnd;

    static CSimpleDoc* Create(CSimpleApp *lpApp, LPRECT lpRect, HWND hWnd);

    void Close();

    CSimpleDoc();
    CSimpleDoc(CSimpleApp *lpApp, HWND hWnd);
    ~CSimpleDoc();

    // IUnknown Interface
    STDMETHODIMP QueryInterface(REFIID riid, LPVOID* ppvObj);
    STDMETHODIMP_(ULONG) AddRef();
    STDMETHODIMP_(ULONG) Release();

    void InsertObject();
    void DisableInsertObject();
    long lResizeDoc(LPRECT lpRect);
    long lAddVerbs();
    void PaintDoc(HDC hDC);
};

#endif
