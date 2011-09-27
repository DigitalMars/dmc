//+---------------------------------------------------------------------------
//
//  Microsoft Windows
//  Copyright (C) Microsoft Corporation, 1992 - 1995.
//
//  File:       plasma.h
//
//  Contents:   class definitions for the Plasma Fractal engine
//
//  Classes:    CPlasmaCF
//              CPlasma
//
//  Functions:
//
//  History:    4-23-94   stevebl   Created
//
//----------------------------------------------------------------------------

#ifndef __PLASMA_H__
#define __PLASMA_H__

#define PROPERTIES 100

#define IDM_RANDOMNESS      1000
#define IDM_ABOUT           1006
#define IDS_ABOUT_TITLE     1100
#define IDS_ABOUT_TEXT      1101

#ifdef __cplusplus

#include <frcngn.h>
#include <frhost.h>
#include <cdialog.h>

//+---------------------------------------------------------------------------
//
//  Class:      CPlasmaCF
//
//  Purpose:    class factory for the Plasma engine
//
//  Interface:  CPlasmaCF      -- constructor
//              ~CPlasmaCF     -- destructor
//              QueryInterface -- requests an interface
//              AddRef         -- increments refcount
//              Release        -- decrements refcount
//              CreateInstance -- creates a Mandelbrot engine
//              LockServer     -- keeps cf around if released
//
//  History:    4-23-94   stevebl   Created
//
//----------------------------------------------------------------------------

class CPlasmaCF : public IClassFactory
{
public:
    CPlasmaCF();
    ~CPlasmaCF();

    HRESULT STDMETHODCALLTYPE QueryInterface(REFIID, LPVOID *);
    ULONG STDMETHODCALLTYPE AddRef(void);
    ULONG STDMETHODCALLTYPE Release(void);

    HRESULT STDMETHODCALLTYPE CreateInstance(LPUNKNOWN, REFIID, LPVOID*);
    HRESULT STDMETHODCALLTYPE LockServer(BOOL);

protected:
    ULONG _cRef;
};

//+---------------------------------------------------------------------------
//
//  Class:      CPlasma
//
//  Purpose:    implements the plasma engine
//
//  Interface:  QueryInterface     -- retrieves an interface
//              AddRef             -- increments refcount
//              Release            -- decrements refcount
//              GetClassID         -- gets the CLSID
//              IsDirty            -- tests if data should be saved
//              Load               -- loads properties
//              Save               -- saves properties
//              GetSizeMax         -- returns max size of property stream
//              Init               -- initializes engine
//              SetDefaults        -- sets default property values
//              SetProperties      -- displays the property dialog box
//              GetExtent          -- gets size of the graph (in graph units)
//              SetExtent          -- sets size of the graph (in graph units)
//              SetGraphSize       -- sets size of the graph (in pixels)
//              Start              -- starts the graph engine
//              Stop               -- stops the graph engine
//              DialogProc         -- dialog proc for the property dialog box
//              Initialize         -- private initialization function
//              GraphicsThread     -- graphics thread entry point
//              Subdivide          -- graph routine for each quadrant
//              CPlasma            -- constructor
//              ~CPlasma           -- destructor
//
//  History:    4-23-94   stevebl   Created
//
//----------------------------------------------------------------------------

class CPlasma : public IFractalEngine, IPersistStream, CHlprDialog
{
public:
    // IUnknown methods
    HRESULT STDMETHODCALLTYPE QueryInterface(REFIID, LPVOID *);
    ULONG STDMETHODCALLTYPE AddRef(void);
    ULONG STDMETHODCALLTYPE Release(void);

    // IPersist methods
    HRESULT STDMETHODCALLTYPE GetClassID(LPCLSID pclsid);

    // IPersistStream methods
    HRESULT STDMETHODCALLTYPE IsDirty(void);
    HRESULT STDMETHODCALLTYPE Load(LPSTREAM pStm);
    HRESULT STDMETHODCALLTYPE Save(LPSTREAM pStm, BOOL fClearDirty);
    HRESULT STDMETHODCALLTYPE GetSizeMax(ULARGE_INTEGER * pcbSize);

    // IFractalEngine methods
    HRESULT STDMETHODCALLTYPE Init(IFractalHost *pfh);

    HRESULT STDMETHODCALLTYPE SetDefaults(void);

    HRESULT STDMETHODCALLTYPE SetProperties(HWND hwnd);

    HRESULT STDMETHODCALLTYPE GetExtent(
        double *pdLeft,
        double *pdTop,
        double *pdRight,
        double *pdBottom);

    HRESULT STDMETHODCALLTYPE SetExtent(
        double dLeft,
        double dTop,
        double dRight,
        double dBottom);

    HRESULT STDMETHODCALLTYPE SetGraphSize(
        unsigned int uWidth,
        unsigned int uHeight);

    HRESULT STDMETHODCALLTYPE Start(void);

    HRESULT STDMETHODCALLTYPE Stop(void);

    // Methods from CHlprDialog
    BOOL DialogProc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);

    // Methods that aren't part of any interface
    BOOL Initialize(void);
    DWORD CPlasma::GraphicsThread(void);
    void Subdivide(int xl, int yl, int xh, int yh);

    CPlasma();
    ~CPlasma();
private:
    ULONG _cRef;
    HANDLE _hRestart, _hRunning, _hEngine;
    DWORD _dwThreadId;
    double _dRandomnessFactor;
    unsigned _uWidth, _uHeight;
    IFractalHost * _pfh;
    BOOL _fDirty;
};

extern HINSTANCE ghinst;
extern ULONG gcRef, gcLock;

#endif //__cplusplus
#endif //__PLASMA_H__
