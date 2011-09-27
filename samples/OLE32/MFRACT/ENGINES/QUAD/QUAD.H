//+---------------------------------------------------------------------------
//
//  Microsoft Windows
//  Copyright (C) Microsoft Corporation, 1992 - 1995.
//
//  File:       quad.h
//
//  Contents:   class definitions for the Quadrant Engine
//
//  Classes:    CQuadrantEngineCF
//              CQuadrantEngine
//
//  Functions:
//
//  History:    4-14-94   stevebl   Created
//
//----------------------------------------------------------------------------

#ifndef __QUAD_H__
#define __QUAD_H__
#ifdef __cplusplus

#include <frcngn.h>
#include <frhost.h>
#include <qudcln.h>
#include <qudngn.h>

//+---------------------------------------------------------------------------
//
//  Class:      CQuadrantEngineCF
//
//  Purpose:    the class factory for the Quadrant Engine
//
//  Interface:  CQuadrantEngineCF  -- constructor
//              ~CQuadrantEngineCF -- destructor
//              QueryInterface     -- retrieves an interface on the cf
//              AddRef             -- incrementes referencec count
//              Release            -- decrements reference count
//              CreateInstance     -- creates a Quadrant Engine object
//              LockServer         -- keeps cf from being deleted on release
//
//  History:    4-14-94   stevebl   Created
//
//----------------------------------------------------------------------------

class CQuadrantEngineCF : public IClassFactory
{
public:
    CQuadrantEngineCF();
    ~CQuadrantEngineCF();

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
//  Class:      CQuadrantEngine
//
//  Purpose:    implementation for the quadrant engine object
//
//  Interface:  QueryInterface     -- retrieves an interface from the engine
//              AddRef             -- increments refcount
//              Release            -- decrements refcount
//              Init               -- initializes quadrant engine
//              UseBoundingBoxes   -- tells engine to use bounding boxes
//              SetGraphSize       -- tells engine the size of its graph
//              Start              -- starts engine
//              Stop               -- stops engine
//              Reset              -- resets engine
//              GraphicsThread     -- entry point for the graphics thread
//              PlotAreaSlowly     -- plots a rectangle
//              Subdivide          -- recursively plots a quadrant
//              Initialize         -- private method to initialize c++ object
//              CQuadrantEngine    -- constructor
//              ~CQuadrantEngine   -- destructor
//
//  History:    4-14-94   stevebl   Created
//
//----------------------------------------------------------------------------

class CQuadrantEngine : public IQuadrantEngine
{
public:
    // IUnknown methods
    HRESULT STDMETHODCALLTYPE QueryInterface(REFIID, LPVOID *);
    ULONG STDMETHODCALLTYPE AddRef(void);
    ULONG STDMETHODCALLTYPE Release(void);

    // IQuadrantEngine methods
    HRESULT STDMETHODCALLTYPE Init(
        IQuadrantClient *pqc,
        IFractalHost *pfh);

    HRESULT STDMETHODCALLTYPE UseBoundingBoxes(BOOL fFlag);

    HRESULT STDMETHODCALLTYPE SetGraphSize(
        unsigned int uWidth,
        unsigned int uHeight);

    HRESULT STDMETHODCALLTYPE Start(void);

    HRESULT STDMETHODCALLTYPE Stop(void);

    HRESULT STDMETHODCALLTYPE Reset(void);

    // Methods that aren't part of any interface
    DWORD GraphicsThread(void);
    void PlotAreaSlowly(
        int iXLow,
        int iYLow,
        int iXHigh,
        int iYHigh);
    void Subdivide(
        int iXLow,
        int iYLow,
        int iXHigh,
        int iYHigh);
    BOOL Initialize(void);
    CQuadrantEngine();
    ~CQuadrantEngine();
private:
    BOOL _fUseBoundingBoxes;
    ULONG _cRef;
    unsigned _uWidth, _uHeight;
    HANDLE _hRestart, _hRunning;
    HANDLE _hEngine;
    DWORD _dwThreadId;
    IFractalHost * _pfh;
    IQuadrantClient * _pqc;
};

extern HINSTANCE ghinst;
extern ULONG gcRef, gcLock;

#endif //__cplusplus
#endif //__QUAD_H__
