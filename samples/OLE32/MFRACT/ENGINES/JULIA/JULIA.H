//+---------------------------------------------------------------------------
//
//  Microsoft Windows
//  Copyright (C) Microsoft Corporation, 1992 - 1995.
//
//  File:       julia.h
//
//  Contents:   class definitions for the Julia Set Fractal engine
//
//  Classes:    CJuliaCF
//              CJulia
//
//  Functions:
//
//  History:    4-14-94   stevebl   Created
//
//----------------------------------------------------------------------------

#ifndef __JULIA_H__
#define __JULIA_H__

#define PROPERTIES 100
#define IDC_LOWREAL         1001
#define IDC_LOWIMAGINARY    1002
#define IDC_HIGHREAL        1003
#define IDC_HIGHIMAGINARY   1004
#define IDC_CREAL           1005
#define IDC_CIMAGINARY      1006
#define IDC_LIMIT           1007
#define IDC_ABOUT           1008
#define IDC_USEBOUNDINGBOXES 1009
#define IDS_ABOUT_TITLE     1100
#define IDS_ABOUT_TEXT      1101

#ifdef __cplusplus

#include <frcngn.h>
#include <frhost.h>
#include <qudcln.h>
#include <qudngn.h>
#include <cdialog.h>

//+---------------------------------------------------------------------------
//
//  Class:      CJuliaCF
//
//  Purpose:    class factory for the Julia set engine
//
//  Interface:  CJuliaCF       -- constructor
//              ~CJuliaCF      -- destructor
//              QueryInterface -- requests an interface
//              AddRef         -- increments refcount
//              Release        -- decrements refcount
//              CreateInstance -- creates a Mandelbrot engine
//              LockServer     -- keeps cf around if released
//
//  History:    4-14-94   stevebl   Created
//
//----------------------------------------------------------------------------

class CJuliaCF : public IClassFactory
{
public:
    CJuliaCF();
    ~CJuliaCF();

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
//  Class:      CJulia
//
//  Purpose:    implements the Julia set engine
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
//              ComputePoint       -- returns the graph color at a given point
//              DoneDrawingGraph   -- called when the graph engine is finished
//              DialogProc         -- dialog proc for the property dialog box
//              Initialize         -- private initialization function
//              CJulia             -- constructor
//              ~CJulia            -- destructor
//
//  History:    4-14-94   stevebl   Created
//              7-07-94   stevebl   Changed definition of ComputePoint
//
//----------------------------------------------------------------------------

class CJulia : public IFractalEngine, IPersistStream, IQuadrantClient, CHlprDialog
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

    // IQuadrantClient methods
    HRESULT STDMETHODCALLTYPE ComputePoint(
        unsigned *puColor,
        int x,
        int y);

    HRESULT STDMETHODCALLTYPE DoneDrawingGraph(void);

    // Methods from CHlprDialog
    BOOL DialogProc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);

    // Methods that aren't part of any interface
    BOOL Initialize(void);
    CJulia();
    ~CJulia();
private:
    ULONG _cRef;
    double _dLeft, _dTop, _dRight, _dBottom;
    double _dCr, _dCi;
    unsigned _uWidth, _uHeight;
    BOOL _fRunning;
    IFractalHost * _pfh;
    IQuadrantEngine * _pqe;
    unsigned _uLimit;
    BOOL _fDirty;
    BOOL _fUseBoundingBoxes;
};

extern HINSTANCE ghinst;
extern ULONG gcRef, gcLock;

#endif //__cplusplus
#endif //__JULIA_H__
