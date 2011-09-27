//+---------------------------------------------------------------------------
//
//  Microsoft Windows
//  Copyright (C) Microsoft Corporation, 1992 - 1995.
//
//  File:       RGenob.cxx
//
//  Contents:   Recursive Generator Fractal engine object
//
//  Classes:    CRGen
//
//  Functions:  StartGraphicsThread
//              Normalize
//
//  History:    4-23-94   stevebl   Created
//
//----------------------------------------------------------------------------

#include <windows.h>
#include <ole2.h>
#include <rgencid.h>
#include "RGen.h"
#include <stdio.h>
#include <strmhelp.h>
#include <palsize.h>
#include "polar.h"
#include <math.h>

#define MAX_RANDOM 50.0
#define MIN_RANDOM 0

//+---------------------------------------------------------------------------
//
//  Function:   StartGraphicsThread
//
//  Synopsis:   Global function used to bootstrap into the GraphicsThread
//              member of the CRGen class.
//
//  Arguments:  [lpdwParam] - not used
//
//  History:    4-15-94   stevebl   Created
//
//----------------------------------------------------------------------------

DWORD StartGraphicsThread(LPDWORD lpdwParam)
{
    return (((CRGen *)lpdwParam)->GraphicsThread());
}


//+---------------------------------------------------------------------------
//
//  Member:     CRGen::QueryInterface
//
//  Synopsis:   standard OLE method
//
//  Arguments:  [riid] - id of desired interface
//              [ppv]  - pointer to receive interface
//
//  Returns:    S_OK on success
//              E_NOINTERFACE if the requested interface isn't supported
//
//  History:    4-23-94   stevebl   Created
//
//----------------------------------------------------------------------------

HRESULT STDMETHODCALLTYPE CRGen::QueryInterface(REFIID riid, LPVOID * ppv)
{
    *ppv = NULL;
    if (IsEqualGUID(IID_IFractalEngine, riid))
    {
        *ppv = (IFractalEngine *)this;
    }
    else if (IsEqualGUID(IID_IPersist, riid))
    {
        *ppv = (IPersist *)this;
    }
    else if (IsEqualGUID(IID_IPersistStream, riid))
    {
        *ppv = (IPersistStream *)this;
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
//  Member:     CRGen::AddRef
//
//  Synopsis:   increments the reference count
//
//  Returns:    new reference count
//
//  History:    4-23-94   stevebl   Created
//
//----------------------------------------------------------------------------

ULONG STDMETHODCALLTYPE CRGen::AddRef(void)
{
    return(++_cRef);
}

//+---------------------------------------------------------------------------
//
//  Member:     CRGen::Release
//
//  Synopsis:   decrements the reference count
//
//  Returns:    new reference count
//
//  History:    4-23-94   stevebl   Created
//
//----------------------------------------------------------------------------

ULONG STDMETHODCALLTYPE CRGen::Release(void)
{
    ULONG cRef = --_cRef;
    if (0 == cRef)
    {
        delete this;
        gcRef--;
    }
    return(cRef);
}

// IPersist methods

//+---------------------------------------------------------------------------
//
//  Member:     CRGen::GetClassID
//
//  Synopsis:   returns the object's class ID
//
//  Arguments:  [pclsid] - pointer to recieve the class ID
//
//  Returns:    S_OK
//
//  History:    4-23-94   stevebl   Created
//
//----------------------------------------------------------------------------

HRESULT CRGen::GetClassID(LPCLSID pclsid)
{
    memcpy(pclsid, &CLSID_RGEN, sizeof(GUID));
    return(S_OK);
}

// IPersistStream methods

//+---------------------------------------------------------------------------
//
//  Member:     CRGen::IsDirty
//
//  Synopsis:   used to determine if the state of the object has changed
//
//  Returns:    S_OK if it is dirty
//              S_FALSE if it is not dirty
//
//  History:    4-23-94   stevebl   Created
//
//----------------------------------------------------------------------------

HRESULT CRGen::IsDirty(void)
{
    if (_fDirty)
    {
        return(S_OK);
    }
    else
    {
        return(S_FALSE);
    }
}

//+---------------------------------------------------------------------------
//
//  Member:     CRGen::Load
//
//  Synopsis:   loads the fractal properties from a stream
//
//  Arguments:  [pStm] - pointer to an IStream interface
//
//  Returns:    S_OK on success
//              E_FAIL on error
//              E_OUTOFMEMORY if there's not enough memory (can't happen here)
//
//  History:    4-23-94   stevebl   Created
//
//----------------------------------------------------------------------------

HRESULT CRGen::Load(LPSTREAM pStm)
{
    if (WAIT_OBJECT_0 == WaitForSingleObject(_hRunning, 0))
    {
        return(E_FAIL);
    }
    HRESULT hr;
    if (FAILED(hr = pStm->Read(&_nItterations, sizeof(_nItterations), NULL)))
    {
        return(hr);
    }
    if (FAILED(hr = pStm->Read(&_nSegments, sizeof(_nSegments), NULL)))
    {
        return(hr);
    }
    for (unsigned uIndex = 0; uIndex < _nSegments; uIndex++)
    {
        if (FAILED(hr = ReadDouble(pStm, &_rgRadius[uIndex], NULL)))
        {
            return(hr);
        }
        if (FAILED(hr = ReadDouble(pStm, &_rgTheta[uIndex], NULL)))
        {
            return(hr);
        }
    }
    if (FAILED(hr = ReadDouble(pStm, &_dLeft, NULL)))
    {
        return(hr);
    }
    if (FAILED(hr = ReadDouble(pStm, &_dTop, NULL)))
    {
        return(hr);
    }
    if (FAILED(hr = ReadDouble(pStm, &_dRight, NULL)))
    {
        return(hr);
    }
    if (FAILED(hr = ReadDouble(pStm, &_dBottom, NULL)))
    {
        return(hr);
    }
    _fDirty = FALSE;
    return(S_OK);
}

//+---------------------------------------------------------------------------
//
//  Member:     CRGen::Save
//
//  Synopsis:   saves the fractal properties to a stream
//
//  Arguments:  [pStm]        - pointer to an IStream interface
//              [fClearDirty] - TRUE if the dirty flag should be cleared
//
//  Returns:    S_OK on success
//
//  History:    4-23-94   stevebl   Created
//
//----------------------------------------------------------------------------

HRESULT CRGen::Save(LPSTREAM pStm, BOOL fClearDirty)
{
    if (WAIT_OBJECT_0 == WaitForSingleObject(_hRunning, 0))
    {
        return(E_FAIL);
    }
    HRESULT hr;
    if (FAILED(hr = pStm->Write(&_nItterations, sizeof(_nItterations), NULL)))
    {
        return(hr);
    }
    if (FAILED(hr = pStm->Write(&_nSegments, sizeof(_nSegments), NULL)))
    {
        return(hr);
    }
    for (unsigned uIndex = 0; uIndex < _nSegments; uIndex++)
    {
        if (FAILED(hr = WriteDouble(pStm, _rgRadius[uIndex], NULL)))
        {
            return(hr);
        }
        if (FAILED(hr = WriteDouble(pStm, _rgTheta[uIndex], NULL)))
        {
            return(hr);
        }
    }
    if (FAILED(hr = WriteDouble(pStm, _dLeft, NULL)))
    {
        return(hr);
    }
    if (FAILED(hr = WriteDouble(pStm, _dTop, NULL)))
    {
        return(hr);
    }
    if (FAILED(hr = WriteDouble(pStm, _dRight, NULL)))
    {
        return(hr);
    }
    if (FAILED(hr = WriteDouble(pStm, _dBottom, NULL)))
    {
        return(hr);
    }
    if (fClearDirty)
    {
        _fDirty = FALSE;
    }
    return(S_OK);
}

//+---------------------------------------------------------------------------
//
//  Member:     CRGen::GetSizeMax
//
//  Synopsis:   used to determine the maximum size of the object's data
//
//  Arguments:  [pcbSize] - pointer to recieve the max size
//
//  Returns:    S_OK
//
//  History:    4-23-94   stevebl   Created
//
//----------------------------------------------------------------------------

HRESULT CRGen::GetSizeMax(ULARGE_INTEGER * pcbSize)
{
    pcbSize->HighPart = 0;
    pcbSize->LowPart = SizeDouble();
    return(S_OK);
}

// IFractalEngine methods

//+---------------------------------------------------------------------------
//
//  Member:     CRGen::Init
//
//  Synopsis:   Initializes the graph engine and saves a pointer to the
//              IFractalHost interface that WinFract passes in as
//              the engine is initialized.
//
//  Arguments:  [pfh] - pointer to an IFractalHost interface
//
//  Returns:    S_OK
//
//  History:    4-23-94   stevebl   Created
//
//----------------------------------------------------------------------------

HRESULT STDMETHODCALLTYPE CRGen::Init(IFractalHost *pfh)
{
    _pfh = pfh;
    // NOTE that I don't addref this pointer!  That would create a circular
    // reference count paradox.
    return(S_OK);
}

//+---------------------------------------------------------------------------
//
//  Member:     CRGen::SetDefaultSegmentList
//
//  Synopsis:   sets the default segment list
//
//  Returns:    S_OK on success
//              E_OUTOFMEMORY if the list couldn't be created
//
//  History:    5-04-94   stevebl   Created
//
//  Notes:      The default segment list consists of four
//              segments that conform to the following shape:
//
//                   /\      .
//                  /  \     .
//              ___/    \___
//
//              This is the Triadic Koch Curve.
//
//              The distance from the first point to the last point in the
//              segment list (or path) is always 1.0 units.  The degree is
//              always 0.
//
//----------------------------------------------------------------------------

HRESULT CRGen::SetDefaultSegmentList(void)
{
    _rgTheta[0] = 0;

    // acos(0) == pi / 2
    // I'm using the trig functions instead of a constant
    // to ensure that the conversion functions use the
    // same values as the ones which I have computed.
    _rgTheta[1] = (acos(0) * 2.0) / 3.0;
    _rgTheta[2] = 0 - (acos(0) * 2.0) / 3.0;
    _rgTheta[3] = 0;
    _rgRadius[0] = _rgRadius[1] = _rgRadius[2] = _rgRadius[3] = 1.0 / 3.0;
    _nSegments = 4;
    return(S_OK);
}
//+---------------------------------------------------------------------------
//
//  Member:     CRGen::SetDefaults
//
//  Synopsis:   initializes the graph engine with its default values
//
//  Returns:    S_OK
//
//  History:    4-23-94   stevebl   Created
//
//----------------------------------------------------------------------------

HRESULT STDMETHODCALLTYPE CRGen::SetDefaults(void)
{
    if (_uWidth > _uHeight)
    {
        // image is wider than it is tall
        _dTop = 1.0;
        _dBottom = -1.0;
        _dLeft = (-1.0 * _uWidth) / _uHeight;
        _dRight = (1.0 * _uWidth) / _uHeight;
    }
    else
    {
        // image is taller than it is wide
        _dLeft = -1.0;
        _dRight = 1.0;
        _dTop = (1.0 * _uHeight) / _uWidth;
        _dBottom = (-1.0 * _uHeight) / _uWidth;
    }
    _nItterations = 6;
    _fDirty = TRUE;
    SetEvent(_hRestart);
    return(S_OK);
}

//+---------------------------------------------------------------------------
//
//  Member:     CRGen::SetProperties
//
//  Synopsis:   Displays the graph engine's property dialog box.
//
//  Arguments:  [hwnd] - handle to the parent window
//
//  Returns:    S_OK
//
//  History:    4-23-94   stevebl   Created
//
//----------------------------------------------------------------------------

HRESULT STDMETHODCALLTYPE CRGen::SetProperties(HWND hwnd)
{
    ShowDialog(ghinst, MAKEINTRESOURCE(PROPERTIES), hwnd);
    return(S_OK);
}

//+---------------------------------------------------------------------------
//
//  Member:     CRGen::GetExtent
//
//  Synopsis:   retrieves the extent of the graph
//
//  Arguments:  [pdLeft]   - x value at the left of the graph
//              [pdTop]    - y value at the top of the graph
//              [pdRight]  - x value at the right of the graph
//              [pdBottom] - y value at the bottom of the graph
//
//  Returns:    S_OK
//
//  History:    4-23-94   stevebl   Created
//
//----------------------------------------------------------------------------

HRESULT STDMETHODCALLTYPE CRGen::GetExtent(
    double *pdLeft,
    double *pdTop,
    double *pdRight,
    double *pdBottom)
{
    *pdLeft = _dLeft;
    *pdTop = _dTop;
    *pdRight = _dRight;
    *pdBottom = _dBottom;
    return(S_OK);
}

//+---------------------------------------------------------------------------
//
//  Member:     CRGen::SetExtent
//
//  Synopsis:   sets the graph's extent
//
//  Arguments:  [dLeft]   - x value at the left of the graph
//              [dTop]    - y value at the top of the graph
//              [dRight]  - x value at the right of the graph
//              [dBottom] - y value at the bottom of the graph
//
//  Returns:    S_OK - extents set
//              E_FAIL - the graph was running so the extents couldn't be set
//
//  History:    4-23-94   stevebl   Created
//
//----------------------------------------------------------------------------

HRESULT STDMETHODCALLTYPE CRGen::SetExtent(
    double dLeft,
    double dTop,
    double dRight,
    double dBottom)
{
    if (WAIT_OBJECT_0 == WaitForSingleObject(_hRunning, 0))
    {
        return(E_FAIL);
    }
    _dLeft = dLeft;
    _dTop = dTop;
    _dRight = dRight;
    _dBottom = dBottom;
    _fDirty = TRUE;
    SetEvent(_hRestart);
    return(S_OK);
}

//+---------------------------------------------------------------------------
//
//  Member:     CRGen::SetGraphSize
//
//  Synopsis:   Tells the graph engine how big the graph is
//
//  Arguments:  [uWidth]  - width of graph (in pixels)
//              [uHeight] - height of graph (in pixels)
//
//  Returns:    S_OK - success
//              E_FAIL - the graph was running or a value was illegal
//
//  History:    4-23-94   stevebl   Created
//
//----------------------------------------------------------------------------

HRESULT STDMETHODCALLTYPE CRGen::SetGraphSize(
    unsigned int uWidth,
    unsigned int uHeight)
{
    if (WAIT_OBJECT_0 == WaitForSingleObject(_hRunning, 0))
    {
        return(E_FAIL);
    }
    if (uWidth == 0 || uHeight == 0)
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
//  Member:     CRGen::Start
//
//  Synopsis:   starts the graph engine
//
//  Returns:    S_OK on success
//
//  History:    4-23-94   stevebl   Created
//
//----------------------------------------------------------------------------

HRESULT STDMETHODCALLTYPE CRGen::Start(void)
{
    SetEvent(_hRunning);
    return(S_OK);
}

//+---------------------------------------------------------------------------
//
//  Member:     CRGen::Stop
//
//  Synopsis:   stops the graph engine
//
//  Returns:    S_OK on success
//
//  History:    4-23-94   stevebl   Created
//
//----------------------------------------------------------------------------

HRESULT STDMETHODCALLTYPE CRGen::Stop(void)
{
    ResetEvent(_hRunning);
    return(S_OK);
}

// Methods that aren't part of any interface

//+---------------------------------------------------------------------------
//
//  Member:     CRGen::Initialize
//
//  Synopsis:   called by the class factory to initialize the object
//
//  Returns:    TRUE on success
//              FALSE if the object couldn't be initialized
//
//  History:    4-23-94   stevebl   Created
//
//----------------------------------------------------------------------------

BOOL CRGen::Initialize(void)
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
    SetDefaultSegmentList();
    return(TRUE);
}

//+---------------------------------------------------------------------------
//
//  Member:     CRGen::GraphicsThread
//
//  Synopsis:   main entry point for the engine's graphics thread
//
//  History:    4-23-94   stevebl   Created
//
//  Notes:      This routine enters into an eternal loop.  It will continue
//              looping until the class is destroyed at which point this
//              thread will be simply terminated.
//
//  Algorithm:
//
//----------------------------------------------------------------------------
DWORD CRGen::GraphicsThread(void)
{
    do
    {
        // wait here until someone starts the graphics engine
        WaitForSingleObject(_hRunning, INFINITE);

        // reset the _hRestart event so that we won't immediately fall out
        // of the engine
        ResetEvent(_hRestart);

        // Now begin drawing the graph

        // erase the background
        _pfh->Rect(0, 0, _uWidth - 1, _uHeight - 1, 0);

        // begin the recursion
        _uColor = 1;
        Recurse(-0.5, 0.0, 1.0, 0.0, 0);

        // We're either done drawing the graph now or we have been restarted
        if (WAIT_TIMEOUT == WaitForSingleObject(_hRestart, 0))
        {
            // Tell our clients that we're finished
            _pfh->DoneDrawingGraph();
            ResetEvent(_hRunning);
        }
    } while (TRUE);
    return(0);
}

//+---------------------------------------------------------------------------
//
//  Member:     CRGen::Recurse
//
//  Synopsis:   Plots one level of the recursion.
//
//  Arguments:  [x]           - the x coordinate of the start of the segment
//              [y]           - the y coordinate of the start of the segment
//              [dRadius]     - the length of the segment
//              [dTheta]      - the angle of the segment
//              [uItteration] - the current depth
//
//  History:    5-04-94   stevebl   Created
//
//  Notes:
//
//----------------------------------------------------------------------------

void CRGen::Recurse(double x, double y, double dRadius, double dTheta, unsigned uItteration)
{
    if (WAIT_TIMEOUT == WaitForSingleObject(_hRestart, 0))
    {
        WaitForSingleObject(_hRunning, INFINITE);
        if (uItteration < _nItterations)
        {
            double dOffsetX, dOffsetY;

            for (unsigned uIndex = 0; uIndex < _nSegments; uIndex++)
            {
                Recurse(
                    x,
                    y,
                    _rgRadius[uIndex] * dRadius,
                    _rgTheta[uIndex] + dTheta,
                    uItteration + 1);
                CartesianFromPolar(
                    dOffsetX,
                    dOffsetY,
                    _rgRadius[uIndex] * dRadius,
                    _rgTheta[uIndex] + dTheta);
                x += dOffsetX;
                y += dOffsetY;
            }
        }
        else
        {
            double dDeltaX, dDeltaY;

            CartesianFromPolar(dDeltaX, dDeltaY, dRadius, dTheta);

            double dWidth = _dRight - _dLeft;
            double dHeight = _dBottom - _dTop;

#define CONVERTX(x) ((((x) - _dLeft) * (_uWidth - 1)) / dWidth)
#define CONVERTY(y) ((((y) - _dTop) * (_uHeight - 1)) / dHeight)

            int x1 = (int) CONVERTX(x);
            int y1 = (int) CONVERTY(y);
            int x2 = (int) CONVERTX(x + dDeltaX);
            int y2 = (int) CONVERTY(y + dDeltaY);

            _pfh->Line(x1, y1, x2, y2, _uColor);

            // advance to the next color
            _uColor = 1 + (_uColor % (PALETTESIZE - 1));
        }
    }
}

//+---------------------------------------------------------------------------
//
//  Member:     CRGen::CRGen
//
//  Synopsis:   constructor
//
//  History:    4-23-94   stevebl   Created
//
//----------------------------------------------------------------------------

CRGen::CRGen()
{
    _uWidth = 160;
    _uHeight = 120;
    _nSegments = 0;
    _nItterations = 1;
    _hRestart = _hRunning = _hEngine = NULL;
    _dwThreadId = 0;
    _dLeft = -1;
    _dTop = 1;
    _dRight = 1;
    _dBottom = 1;
    _cRef = 0;
    _pfh = NULL;
    _fDirty = TRUE;
}

//+---------------------------------------------------------------------------
//
//  Member:     CRGen::~CRGen
//
//  Synopsis:   destructor
//
//  History:    4-23-94   stevebl   Created
//
//----------------------------------------------------------------------------

CRGen::~CRGen()
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

#define LENGTH 50
#define ABOUT_STRING_LENGTH 256

//+---------------------------------------------------------------------------
//
//  Member:     CRGen::DialogProc
//
//  Synopsis:   Dialog proc for the Properties dialog box
//
//  Arguments:  [hwndDlg] - the dialog's window handle
//              [uMsg]    - message
//              [wParam]  - first message parameter
//              [lParam]  - second message parameter
//
//  History:    4-23-94   stevebl   Created
//
//----------------------------------------------------------------------------

BOOL CRGen::DialogProc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
    case WM_INITDIALOG:
        {
            _pwinGeneratorWindow = new CGeneratorWindow;
            if (_pwinGeneratorWindow)
            {
                _pwinGeneratorWindow->InitializePoints(_rgRadius, _rgTheta, _nSegments);
                RECT rDim;
                rDim.left = 3;
                rDim.right = 100;
                rDim.top = 10;
                rDim.bottom = 100;
                MapDialogRect(hwndDlg, &rDim);
                _pwinGeneratorWindow->Create(
                    TEXT("RGen Generator"),
                    NULL,
                    WS_CHILD | WS_VISIBLE,
                    rDim.left,
                    rDim.top,
                    rDim.right,
                    rDim.bottom,
                    hwndDlg,
                    NULL,
                    ghinst);
            }
            SetDlgItemInt(hwndDlg, IDD_ITTERATIONS, _nItterations, FALSE);
        }
        return(TRUE);
    case WM_COMMAND:
        switch (LOWORD(wParam))
        {
        case IDOK:
            _nItterations = GetDlgItemInt(hwndDlg, IDD_ITTERATIONS, NULL, FALSE);
            if (_pwinGeneratorWindow)
            {
                _pwinGeneratorWindow->RetrievePoints(_rgRadius, _rgTheta, _nSegments);
            }
            _fDirty = TRUE;
            SetEvent(_hRestart);
            EndDialog(hwndDlg, TRUE);
            return(TRUE);
        case IDD_RESET:
            {
                SetDefaultSegmentList();
                if (_pwinGeneratorWindow)
                {
                    _pwinGeneratorWindow->InitializePoints(_rgRadius, _rgTheta, _nSegments);
                    _pwinGeneratorWindow->Redraw();
                }
                return(TRUE);
            }
        case IDD_ABOUT:
            {
                TCHAR szTitle[ABOUT_STRING_LENGTH];
                TCHAR szText[ABOUT_STRING_LENGTH];
                if (LoadString(ghinst, IDS_ABOUT_TITLE, szTitle, ABOUT_STRING_LENGTH))
                {
                    if (LoadString(ghinst, IDS_ABOUT_TEXT, szText, ABOUT_STRING_LENGTH))
                    {
                        MessageBox(
                            hwndDlg,
                            szText,
                            szTitle,
                            MB_OK | MB_ICONINFORMATION);
                    }
                }
            }
            return(TRUE);
        case IDCANCEL:
            EndDialog(hwndDlg, FALSE);
            return(TRUE);
        default:
            return(FALSE);
        }
    }
    return(FALSE);
}

