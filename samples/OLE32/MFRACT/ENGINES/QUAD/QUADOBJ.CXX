//+---------------------------------------------------------------------------
//
//  Microsoft Windows
//  Copyright (C) Microsoft Corporation, 1992 - 1995.
//
//  File:       mandelob.cxx
//
//  Contents:   Quadrant Engine object
//
//  Classes:    CQuadrantEngine
//
//  Functions:  StartGraphicsThread
//
//  History:    4-14-94   stevebl   Created
//
//----------------------------------------------------------------------------

#include <windows.h>
#include <ole2.h>
#include "quad.h"

//+---------------------------------------------------------------------------
//
//  Function:   StartGraphicsThread
//
//  Synopsis:   Global function used to bootstrap into the GraphicsThread
//              member of the CQuadrantEngine class.
//
//  Arguments:  [lpdwParam] - this pointer to a CQuadrantEngine class object
//
//  History:    4-15-94   stevebl   Created
//
//----------------------------------------------------------------------------

DWORD StartGraphicsThread(LPDWORD lpdwParam)
{
    return (((CQuadrantEngine *)lpdwParam)->GraphicsThread());
}

//+---------------------------------------------------------------------------
//
//  Member:     CQuadrantEngine::QueryInterface
//
//  Synopsis:   standard OLE method
//
//  Arguments:  [riid] - reference to the requested interface id
//              [ppv]  - pointer to recieve the interface
//
//  Returns:    S_OK on success
//              E_NOINTERFACE if the requested interface isn't supported
//
//  History:    4-14-94   stevebl   Created
//
//----------------------------------------------------------------------------

HRESULT STDMETHODCALLTYPE CQuadrantEngine::QueryInterface(REFIID riid, LPVOID * ppv)
{
    *ppv = NULL;
    if (IsEqualGUID(IID_IQuadrantEngine, riid))
    {
        *ppv = (IQuadrantEngine *)this;
    }
    else if (IsEqualGUID(IID_IUnknown, riid))
    {
        *ppv = (IUnknown *)(IFractalEngine *)this;
    }
    if (*ppv)
    {
        ((IUnknown *)*ppv)->AddRef();
        return(S_OK);
    }
    return(E_NOINTERFACE);
}

//+---------------------------------------------------------------------------
//
//  Member:     CQuadrantEngine::AddRef
//
//  Synopsis:   increments the reference count
//
//  Returns:    new reference count
//
//  History:    4-14-94   stevebl   Created
//
//----------------------------------------------------------------------------

ULONG STDMETHODCALLTYPE CQuadrantEngine::AddRef(void)
{
    return(++_cRef);
}

//+---------------------------------------------------------------------------
//
//  Member:     CQuadrantEngine::Release
//
//  Synopsis:   decrements the reference count
//
//  Returns:    new reference count
//
//  History:    4-14-94   stevebl   Created
//
//----------------------------------------------------------------------------

ULONG STDMETHODCALLTYPE CQuadrantEngine::Release(void)
{
    ULONG cRef = --_cRef;
    if (0 == cRef)
    {
        delete this;
        gcRef--;
    }
    return(cRef);
}

// IQuadrantEngine methods

//+---------------------------------------------------------------------------
//
//  Member:     CQuadrantEngine::Init
//
//  Synopsis:   initializes the quadrant engine
//
//  Arguments:  [pqc] - pointer to an IQuadrantClient interface
//              [pfh] - pointer to an IFractalHost interface
//
//  Returns:    S_OK
//
//  History:    4-27-94   stevebl   Created
//
//----------------------------------------------------------------------------

HRESULT STDMETHODCALLTYPE CQuadrantEngine::Init(
    IQuadrantClient *pqc,
    IFractalHost *pfh)
{
    _pqc = pqc;
    _pfh = pfh;
    // NOTE that neither of these interface pointers are reference counted since
    // I am depending on their owners to tell me when I can be unloaded.
    return(S_OK);
}

//+---------------------------------------------------------------------------
//
//  Member:     CQuadrantEngine::UseBoundingBoxes
//
//  Synopsis:   tells the quadrant engine to use the bounding box optimization
//
//  Arguments:  [fFlag] - TRUE if the optimization is enabled (FALSE otherwise)
//
//  Returns:    S_OK
//
//  History:    4-27-94   stevebl   Created
//
//----------------------------------------------------------------------------

HRESULT STDMETHODCALLTYPE CQuadrantEngine::UseBoundingBoxes(BOOL fFlag)
{
    _fUseBoundingBoxes = fFlag;
    return(S_OK);
}

//+---------------------------------------------------------------------------
//
//  Member:     CQuadrantEngine::SetGraphSize
//
//  Synopsis:   tells the quadrant engine how big the graph is
//
//  Arguments:  [uWidth]  - width of the graph in pixels
//              [uHeight] - height of the graph in pixels
//
//  Returns:    S_OK on success
//              E_FAIL if the graph size can't be altered at this time
//
//  History:    4-27-94   stevebl   Created
//
//----------------------------------------------------------------------------

HRESULT STDMETHODCALLTYPE CQuadrantEngine::SetGraphSize(
    unsigned int uWidth,
    unsigned int uHeight)
{
    if (WAIT_OBJECT_0 == WaitForSingleObject(_hRunning, 0))
    {
        return(E_FAIL);
    }
    _uWidth = uWidth;
    _uHeight = uHeight;
    SetEvent(_hRestart);
    return(S_OK);
}

//+---------------------------------------------------------------------------
//
//  Member:     CQuadrantEngine::Start
//
//  Synopsis:   Starts the graph engine
//
//  Returns:    S_OK on success
//
//  History:    4-27-94   stevebl   Created
//
//----------------------------------------------------------------------------

HRESULT STDMETHODCALLTYPE CQuadrantEngine::Start(void)
{
    SetEvent(_hRunning);
    return(S_OK);
}

//+---------------------------------------------------------------------------
//
//  Member:     CQuadrantEngine::Stop
//
//  Synopsis:   stops the graph engine
//
//  Returns:    S_OK on success
//
//  History:    4-27-94   stevebl   Created
//
//----------------------------------------------------------------------------

HRESULT STDMETHODCALLTYPE CQuadrantEngine::Stop(void)
{
    ResetEvent(_hRunning);
    return(S_OK);
}

//+---------------------------------------------------------------------------
//
//  Member:     CQuadrantEngine::Reset
//
//  Synopsis:   tells the graph engine to quit drawing the current graph
//              (if it is drawing it) and start over again
//
//  Returns:    S_OK
//
//  History:    4-27-94   stevebl   Created
//
//----------------------------------------------------------------------------

HRESULT STDMETHODCALLTYPE CQuadrantEngine::Reset(void)
{
    SetEvent(_hRestart);
    return(S_OK);
}


//+---------------------------------------------------------------------------
//
//  Member:     CQuadrantEngine::Initialize
//
//  Synopsis:   private method used to initialize the c++ class object
//
//  History:    4-27-94   stevebl   Created
//
//----------------------------------------------------------------------------

BOOL CQuadrantEngine::Initialize(void)
{
    _hRestart = CreateEvent(
        NULL,
        TRUE,   // must be manually reset
        TRUE,   // initially signaled
        NULL);
    if (_hRestart == NULL)
    {
        return(FALSE);  // couldn't create an event
    }
    _hRunning = CreateEvent(
        NULL,
        TRUE,
        FALSE,
        NULL);
    if (_hRunning == NULL)
    {
        return(FALSE);
    }
    _hEngine = CreateThread(
        (LPSECURITY_ATTRIBUTES)NULL,
        0,
        (LPTHREAD_START_ROUTINE) StartGraphicsThread,
        this,
        0,
        &_dwThreadId);
    if (_hEngine == NULL)
    {
        return(FALSE);  // couldn't start the graphics thread
    }
    return(TRUE);
}

//+---------------------------------------------------------------------------
//
//  Member:     CQuadrantEngine::GraphicsThread
//
//  Synopsis:   main entry point for the engine's graphics thread
//
//  History:    4-27-94   stevebl   Created
//
//  Notes:      This routine enters into an eternal loop.  It will continue
//              looping until the class is destroyed at which point this
//              thread will be simply terminated.
//
//  Algorithm:  Wait for the engine to be started.
//              Clear the graph.
//              Plot the points at the graph's edge and also plot two lines
//              through the center of the graph, dividing it into fourths.
//              For each quadrant, call Subdivide to begin the recursion.
//              After falling out of the recursion, check to see if the
//              graph has been aborted.
//              If not, notify that the graph has been completed.
//              Loop back around to the beginning.
//
//----------------------------------------------------------------------------

DWORD CQuadrantEngine::GraphicsThread(void)
{
    do
    {
        // wait here until someone starts the graphics engine
        WaitForSingleObject(_hRunning, INFINITE);
        // reset the _hRestart event so that we won't immediately fall out
        // of the engine
        ResetEvent(_hRestart);
        // Erase the graph
        _pfh->Rect(0, 0, (int) _uWidth - 1, (int) _uHeight - 1, PALETTEINDEX(0));
        unsigned uColor;
        for (int x = 0;
            x < (int)_uWidth
            && WAIT_TIMEOUT == WaitForSingleObject(_hRestart, 0)
            ; x++)
        {
            _pqc->ComputePoint(&uColor, x, 0);
            _pfh->Point(x, 0, uColor);
            _pqc->ComputePoint(&uColor, x, ((int) _uHeight - 1) / 2);
            _pfh->Point(x, ((int) _uHeight - 1) / 2, uColor);
            _pqc->ComputePoint(&uColor, x, (int) _uHeight - 1);
            _pfh->Point(x, (int) _uHeight - 1, uColor);
            WaitForSingleObject(_hRunning, INFINITE);
        }
        int y;
        for (y = 1;
            y < ((int) _uHeight - 1) / 2
            && WAIT_TIMEOUT == WaitForSingleObject(_hRestart, 0)
            ; y++)
        {
            _pqc->ComputePoint(&uColor, 0, y);
            _pfh->Point(0, y, uColor);
            _pqc->ComputePoint(&uColor, ((int) _uWidth - 1) / 2, y);
            _pfh->Point(((int) _uWidth - 1) / 2, y, uColor);
            _pqc->ComputePoint(&uColor, (int) _uWidth - 1, y);
            _pfh->Point((int) _uWidth - 1, y, uColor);
            WaitForSingleObject(_hRunning, INFINITE);
        }
        for (y = ((int) _uHeight - 1) / 2 + 1;
            y < (int) _uHeight - 1
            && WAIT_TIMEOUT == WaitForSingleObject(_hRestart, 0)
            ; y++)
        {
            _pqc->ComputePoint(&uColor, 0, y);
            _pfh->Point(0, y, uColor);
            _pqc->ComputePoint(&uColor, ((int) _uWidth - 1) / 2, y);
            _pfh->Point(((int) _uWidth - 1) / 2, y, uColor);
            _pqc->ComputePoint(&uColor, (int) _uWidth - 1, y);
            _pfh->Point((int) _uWidth - 1, y, uColor);
            WaitForSingleObject(_hRunning, INFINITE);
        }
        Subdivide(0, 0, (int) (_uWidth - 1) / 2, (int) (_uHeight - 1) / 2);
        Subdivide((int) (_uWidth - 1) / 2, 0, (int) _uWidth - 1, (int) (_uHeight - 1) / 2);
        Subdivide(0, (int) (_uHeight - 1) / 2, (int) (_uWidth - 1) / 2, (int) _uHeight - 1);
        Subdivide((int) (_uWidth - 1) / 2, (int) (_uHeight - 1) / 2, (int) _uWidth - 1, (int) _uHeight - 1);
        if (WAIT_TIMEOUT == WaitForSingleObject(_hRestart, 0))
        {
            // Tell our clients that we're finished
            _pqc->DoneDrawingGraph();
            _pfh->DoneDrawingGraph();
            ResetEvent(_hRunning);
        }
    } while (TRUE);
    return(0);
}

//+---------------------------------------------------------------------------
//
//  Member:     CQuadrantEngine::PlotAreaSlowly
//
//  Synopsis:   plots each point in a rectangle by brute force: one pixel at a
//              time
//
//  Arguments:  [iXLow]  - left side of rectangle
//              [iYLow]  - top
//              [iXHigh] - right side
//              [iYHigh] - bottom
//
//  History:    4-27-94   stevebl   Created
//
//  Notes:      Used when a box is to small to be subdivided any farther.
//
//----------------------------------------------------------------------------

void CQuadrantEngine::PlotAreaSlowly(
    int iXLow,
    int iYLow,
    int iXHigh,
    int iYHigh)
{
    for (int y = iYLow ; y <= iYHigh
        && WAIT_TIMEOUT == WaitForSingleObject(_hRestart, 0)
        ; y++)
    {
        for (int x = iXLow; x <= iXHigh
            && WAIT_TIMEOUT == WaitForSingleObject(_hRestart, 0)
            ; x++)
        {
            unsigned uColor;
            _pqc->ComputePoint(&uColor, x, y);
            _pfh->Point(x, y, uColor);
            WaitForSingleObject(_hRunning, INFINITE);
        }
    }
}

// SUBDIVIDE determines at what point the Subdivide routine quits trying
// to draw ever-smaller boxes and just plots the pixels by brute force.
CONST unsigned SUBDIVIDE = 3;

//+---------------------------------------------------------------------------
//
//  Member:     CQuadrantEngine::Subdivide
//
//  Synopsis:   recursively plots all points within a rectangle
//
//  Arguments:  [iXLow]  - left side of rectangle
//              [iYLow]  - top
//              [iXHigh] - right side
//              [iYHigh] - bottom
//
//  History:    4-27-94   stevebl   Created
//
//  Algorithm:  If the bounding box optimization is enabled, check to see if
//              the perimiter is all the same color.
//              If the perimiter is all one color, fill the entire rectangle
//              with that color and fall out of the recursion.
//              Otherwise, divide the rectangle into four quadrants and
//              continue the recursion into each of them.
//
//----------------------------------------------------------------------------

void CQuadrantEngine::Subdivide(
    int iXLow,
    int iYLow,
    int iXHigh,
    int iYHigh)
{
    if (WAIT_TIMEOUT == WaitForSingleObject(_hRestart, 0))
    {
        BOOL fDescend = !_fUseBoundingBoxes;
        //
        // If the bounding box optimization is enabled then fDescend
        // will be FALSE until it finds two points on the perimiter
        // that don't match.
        //
        // If the bounding box optimization is disabled then fDescend
        // will be TRUE, causing the checking to be disabled altogether.
        //

        if (iXHigh - iXLow > SUBDIVIDE && iYHigh - iYLow > SUBDIVIDE)
        {
            unsigned uTest, uTemp;
            _pfh->GetPoint(&uTest, iXLow, iYLow);
            for (int x = iXLow;
                (!fDescend) && x <= iXHigh;
                x++)
            {
                _pfh->GetPoint(&uTemp, x, iYLow);
                if (uTest != uTemp)
                {
                    fDescend = TRUE;
                }
                else
                {
                    _pfh->GetPoint(&uTemp, x, iYHigh);
                    if (uTest != uTemp)
                    {
                        fDescend = TRUE;
                    }
                }
            }
            for (int y = iYLow + 1;
                (!fDescend) && y < iYHigh;
                y++)
            {
                _pfh->GetPoint(&uTemp, iXLow, y);
                if (uTest != uTemp)
                {
                    fDescend = TRUE;
                }
                else
                {
                    _pfh->GetPoint(&uTemp, iXHigh, y);
                    if (uTest != uTemp)
                    {
                        fDescend = TRUE;
                    }
                }
            }
            if (fDescend)
            {
                unsigned uColor;
                int iXMid = (iXLow + iXHigh) / 2;
                int iYMid = (iYLow + iYHigh) / 2;
                for (int x = iXLow + 1;
                    x < iXHigh
                    && WAIT_TIMEOUT == WaitForSingleObject(_hRestart, 0)
                    ; x++)
                {
                    _pqc->ComputePoint(&uColor, x, iYMid);
                    _pfh->Point(x, iYMid, uColor);
                    WaitForSingleObject(_hRunning, INFINITE);
                }
                int y;
                for (y = iYLow + 1;
                    y < iYMid
                    && WAIT_TIMEOUT == WaitForSingleObject(_hRestart, 0)
                    ; y++)
                {
                    _pqc->ComputePoint(&uColor, iXMid, y);
                    _pfh->Point(iXMid, y, uColor);
                    WaitForSingleObject(_hRunning, INFINITE);
                }
                for (y = iYMid + 1;
                    y < iYHigh
                    && WAIT_TIMEOUT == WaitForSingleObject(_hRestart, 0)
                    ; y++)
                {
                    _pqc->ComputePoint(&uColor, iXMid, y);
                    _pfh->Point(iXMid, y, uColor);
                    WaitForSingleObject(_hRunning, INFINITE);
                }
                Subdivide(iXLow, iYLow, iXMid, iYMid);
                Subdivide(iXMid, iYLow, iXHigh, iYMid);
                Subdivide(iXLow, iYMid, iXMid, iYHigh);
                Subdivide(iXMid, iYMid, iXHigh, iYHigh);
            }
            else
            {
                _pfh->Rect(iXLow + 1, iYLow + 1,
                    iXHigh - 1, iYHigh - 1,
                    uTest);
            }
        }
        else
        {
            // do a simple iteration to plot the graph
            PlotAreaSlowly(iXLow + 1, iYLow + 1, iXHigh - 1, iYHigh - 1);
        }
    }
}

//+---------------------------------------------------------------------------
//
//  Member:     CQuadrantEngine::CQuadrantEngine
//
//  Synopsis:   constructor
//
//  History:    4-14-94   stevebl   Created
//
//----------------------------------------------------------------------------

CQuadrantEngine::CQuadrantEngine()
{
    _uWidth = 160;
    _uHeight = 180;
    _cRef = 0;
    _pfh = NULL;
    _pqc = NULL;
    _hRunning = NULL;
    _hRestart = NULL;
    _hEngine = NULL;
    _dwThreadId = 0;
    _fUseBoundingBoxes = TRUE;
}

//+---------------------------------------------------------------------------
//
//  Member:     CQuadrantEngine::~CQuadrantEngine
//
//  Synopsis:   destructor
//
//  History:    4-14-94   stevebl   Created
//
//----------------------------------------------------------------------------

CQuadrantEngine::~CQuadrantEngine()
{
    if (_hRestart)
    {
        CloseHandle(_hRestart);
    }
    if (_hRunning)
    {
        CloseHandle(_hRunning);
    }
    if (_hEngine)
    {
        TerminateThread(_hEngine, 0);
        CloseHandle(_hEngine);
    }
}
