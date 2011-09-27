//+---------------------------------------------------------------------------
//
//  Microsoft Windows
//  Copyright (C) Microsoft Corporation, 1992 - 1995.
//
//  File:       RGen.h
//
//  Contents:   class definitions for the Recursive Generator Fractal engine
//
//  Classes:    CRGenCF
//              CRGen
//
//  Functions:
//
//  History:    4-23-94   stevebl   Created
//
//----------------------------------------------------------------------------

#ifndef __RGen_H__
#define __RGen_H__

#define PROPERTIES 100

#define IDD_RESET                   102
#define IDD_ABOUT                   103
#define IDD_ITTERATIONS              105
#define IDD_GENERATOR               101

#define IDS_ABOUT_TITLE     200
#define IDS_ABOUT_TEXT      201

#define IDC_CHOOSE  300

#ifdef __cplusplus

#include <frcngn.h>
#include <frhost.h>
#include <cdialog.h>
#include "genwin.h"

#define MAX_SEGMENTS (MAX_POINTS - 1)
//+---------------------------------------------------------------------------
//
//  Class:      CRGenCF
//
//  Purpose:    class factory for the RGen engine
//
//  Interface:  CRGenCF      -- constructor
//              ~CRGenCF     -- destructor
//              QueryInterface -- requests an interface
//              AddRef         -- increments refcount
//              Release        -- decrements refcount
//              CreateInstance -- creates a Mandelbrot engine
//              LockServer     -- keeps cf around if released
//
//  History:    4-23-94   stevebl   Created
//
//----------------------------------------------------------------------------

class CRGenCF : public IClassFactory
{
public:
    CRGenCF();
    ~CRGenCF();

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
//  Class:      CRGen
//
//  Purpose:    implements the RGen engine
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
//              CRGen            -- constructor
//              ~CRGen           -- destructor
//
//  History:    4-23-94   stevebl   Created
//
//----------------------------------------------------------------------------

class CRGen : public IFractalEngine, IPersistStream, CHlprDialog
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
    DWORD GraphicsThread(void);
    HRESULT SetDefaultSegmentList(void);
    void Recurse(double x, double y, double dRadius, double dTheta, unsigned uItteration);

    CRGen();
    ~CRGen();
private:
    ULONG _cRef;
    HANDLE _hRestart, _hRunning, _hEngine;
    DWORD _dwThreadId;
    double _dLeft, _dRight, _dTop, _dBottom;
    unsigned _nSegments;
    unsigned _nItterations;
    double _rgTheta[MAX_SEGMENTS];
    double _rgRadius[MAX_SEGMENTS];
    unsigned _uWidth, _uHeight, _uColor;
    IFractalHost * _pfh;
    BOOL _fDirty;
    CGeneratorWindow  * _pwinGeneratorWindow;
};

extern HINSTANCE ghinst;
extern ULONG gcRef, gcLock;

#endif //__cplusplus
#endif //__RGen_H__
