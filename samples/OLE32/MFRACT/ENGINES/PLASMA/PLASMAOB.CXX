//+---------------------------------------------------------------------------
//
//  Microsoft Windows
//  Copyright (C) Microsoft Corporation, 1992 - 1995.
//
//  File:       plasmaob.cxx
//
//  Contents:   Plasma Fractal engine object
//
//  Classes:    CPlasma
//
//  Functions:  StartGraphicsThread
//              Normalize
//
//  History:    4-23-94   stevebl   Created
//
//----------------------------------------------------------------------------

#include <windows.h>
#include <ole2.h>
#include <plasmcid.h>
#include "plasma.h"
#include <stdio.h>
#include <strmhelp.h>
#include <palsize.h>
#include <math.h>

#define MAX_RANDOM 50.0
#define MIN_RANDOM 0

//+---------------------------------------------------------------------------
//
//  Function:   StartGraphicsThread
//
//  Synopsis:   Global function used to bootstrap into the GraphicsThread
//              member of the CPlasma class.
//
//  Arguments:  [lpdwParam] - not used
//
//  History:    4-15-94   stevebl   Created
//
//----------------------------------------------------------------------------

DWORD StartGraphicsThread(LPDWORD lpdwParam)
{
    return (((CPlasma *)lpdwParam)->GraphicsThread());
}


//+---------------------------------------------------------------------------
//
//  Member:     CPlasma::QueryInterface
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

HRESULT STDMETHODCALLTYPE CPlasma::QueryInterface(REFIID riid, LPVOID * ppv)
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
//  Member:     CPlasma::AddRef
//
//  Synopsis:   increments the reference count
//
//  Returns:    new reference count
//
//  History:    4-23-94   stevebl   Created
//
//----------------------------------------------------------------------------

ULONG STDMETHODCALLTYPE CPlasma::AddRef(void)
{
    return(++_cRef);
}

//+---------------------------------------------------------------------------
//
//  Member:     CPlasma::Release
//
//  Synopsis:   decrements the reference count
//
//  Returns:    new reference count
//
//  History:    4-23-94   stevebl   Created
//
//----------------------------------------------------------------------------

ULONG STDMETHODCALLTYPE CPlasma::Release(void)
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
//  Member:     CPlasma::GetClassID
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

HRESULT CPlasma::GetClassID(LPCLSID pclsid)
{
    memcpy(pclsid, &CLSID_PLASMA, sizeof(GUID));
    return(S_OK);
}

// IPersistStream methods

//+---------------------------------------------------------------------------
//
//  Member:     CPlasma::IsDirty
//
//  Synopsis:   used to determine if the state of the object has changed
//
//  Returns:    S_OK if it is dirty
//              S_FALSE if it is not dirty
//
//  History:    4-23-94   stevebl   Created
//
//----------------------------------------------------------------------------

HRESULT CPlasma::IsDirty(void)
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
//  Member:     CPlasma::Load
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

HRESULT CPlasma::Load(LPSTREAM pStm)
{
    if (WAIT_OBJECT_0 == WaitForSingleObject(_hRunning, 0))
    {
        return(E_FAIL);
    }
    HRESULT hr;
    if (FAILED(hr = ReadDouble(pStm, &_dRandomnessFactor, NULL)))
    {
        return(hr);
    }
    _fDirty = FALSE;
    return(S_OK);
}

//+---------------------------------------------------------------------------
//
//  Member:     CPlasma::Save
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

HRESULT CPlasma::Save(LPSTREAM pStm, BOOL fClearDirty)
{
    if (WAIT_OBJECT_0 == WaitForSingleObject(_hRunning, 0))
    {
        return(E_FAIL);
    }
    HRESULT hr;
    if (FAILED(hr = WriteDouble(pStm, _dRandomnessFactor, NULL)))
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
//  Member:     CPlasma::GetSizeMax
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

HRESULT CPlasma::GetSizeMax(ULARGE_INTEGER * pcbSize)
{
    pcbSize->HighPart = 0;
    pcbSize->LowPart = SizeDouble();
    return(S_OK);
}

// IFractalEngine methods

//+---------------------------------------------------------------------------
//
//  Member:     CPlasma::Init
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

HRESULT STDMETHODCALLTYPE CPlasma::Init(IFractalHost *pfh)
{
    _pfh = pfh;
    // NOTE that I don't addref this pointer!  That would create a circular
    // reference count paradox.
    return(S_OK);
}

//+---------------------------------------------------------------------------
//
//  Member:     CPlasma::SetDefaults
//
//  Synopsis:   initializes the graph engine with its default values
//
//  Returns:    S_OK
//
//  History:    4-23-94   stevebl   Created
//
//----------------------------------------------------------------------------

HRESULT STDMETHODCALLTYPE CPlasma::SetDefaults(void)
{
    _dRandomnessFactor = 2.0;
    _fDirty = TRUE;
    SetEvent(_hRestart);
    return(S_OK);
}

//+---------------------------------------------------------------------------
//
//  Member:     CPlasma::SetProperties
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

HRESULT STDMETHODCALLTYPE CPlasma::SetProperties(HWND hwnd)
{
    ShowDialog(ghinst, MAKEINTRESOURCE(PROPERTIES), hwnd);
    return(S_OK);
}

//+---------------------------------------------------------------------------
//
//  Member:     CPlasma::GetExtent
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

HRESULT STDMETHODCALLTYPE CPlasma::GetExtent(
    double *pdLeft,
    double *pdTop,
    double *pdRight,
    double *pdBottom)
{
    *pdLeft = -1.0;
    *pdTop = 1.0;
    *pdRight = 1.0;
    *pdBottom = -1.0;
    return(S_OK);
}

//+---------------------------------------------------------------------------
//
//  Member:     CPlasma::SetExtent
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

HRESULT STDMETHODCALLTYPE CPlasma::SetExtent(
    double dLeft,
    double dTop,
    double dRight,
    double dBottom)
{
    if (WAIT_OBJECT_0 == WaitForSingleObject(_hRunning, 0))
    {
        return(E_FAIL);
    }
    _fDirty = TRUE;
    SetEvent(_hRestart);
    return(S_OK);
}

//+---------------------------------------------------------------------------
//
//  Member:     CPlasma::SetGraphSize
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

HRESULT STDMETHODCALLTYPE CPlasma::SetGraphSize(
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
//  Member:     CPlasma::Start
//
//  Synopsis:   starts the graph engine
//
//  Returns:    S_OK on success
//
//  History:    4-23-94   stevebl   Created
//
//----------------------------------------------------------------------------

HRESULT STDMETHODCALLTYPE CPlasma::Start(void)
{
    SetEvent(_hRunning);
    return(S_OK);
}

//+---------------------------------------------------------------------------
//
//  Member:     CPlasma::Stop
//
//  Synopsis:   stops the graph engine
//
//  Returns:    S_OK on success
//
//  History:    4-23-94   stevebl   Created
//
//----------------------------------------------------------------------------

HRESULT STDMETHODCALLTYPE CPlasma::Stop(void)
{
    ResetEvent(_hRunning);
    return(S_OK);
}

// Methods that aren't part of any interface

//+---------------------------------------------------------------------------
//
//  Member:     CPlasma::Initialize
//
//  Synopsis:   called by the clas factory to initialize the object
//
//  Returns:    TRUE on success
//              FALSE if the object couldn't be initialized
//
//  History:    4-23-94   stevebl   Created
//
//----------------------------------------------------------------------------

BOOL CPlasma::Initialize(void)
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
//  Member:     CPlasma::GraphicsThread
//
//  Synopsis:   main entry point for the engine's graphics thread
//
//  History:    4-23-94   stevebl   Created
//
//  Notes:      This routine enters into an eternal loop.  It will continue
//              looping until the class is destroyed at which point this
//              thread will be simply terminated.
//
//  Algorithm:  Wait for the engine to be started.
//              Clear the graph.
//              Plot the points at the graph's four corners.
//              Divide it into four quadrants and call Subdivide for each.
//              After falling out of the recursion, check to see if the
//              graph has been aborted.
//              If not, notify that the graph has been completed.
//              Loop back around to the beginning.
//
//----------------------------------------------------------------------------
DWORD CPlasma::GraphicsThread(void)
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
        _pfh->Rect(0, 0, (int) _uWidth - 1, (int) _uHeight - 1, 0);

        // Seed the random number generator
        srand((UINT)GetTickCount());

        // plot the upper corner
        _pfh->Point(0, 0, rand() % (PALETTESIZE -1 ));

        // begin the recursion
        Subdivide(0, 0, (int) _uWidth, (int) _uHeight);

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
//  Function:   Normalize
//
//  Synopsis:   Makes sure that the value is within the legal color set.
//
//  Arguments:  [iTemp] - color index value
//
//  Returns:    0 <= iTemp < (PALETTESIZE - 1)
//
//  History:    4-28-94   stevebl   Created
//
//  Notes:      There are a couple ways this could be achieved, most of them
//              are undesirable.
//
//              For instance, we could just perform a modulus operation on
//              the value:
//
//                  iTemp = iTemp % (PALETTESIZE - 1)
//
//              This would have the effect of causing numbers to wrap
//              around (PALETTESIZE - 1 would become 0).  This would be bad because
//              in order for the graph to look good we want all the points to
//              transition as smoothley as possible.  Doing it this way
//              would cause the graph to have an area where the elevation
//              suddenly changed dramaticaly when it should have been very
//              smooth.
//
//              Another option would be to just truncate the value:
//
//              if (iTemp) > PALETTESIZE - 2) iTemp = PALETTESIZE - 2
//
//              This would cause our graph to have areas where it is
//              artificially flat.
//
//              Instead, this routine reflects the value back down into a
//              range of values.  For every unit the value is over
//              PALETTESIZE - 2, it is made to be that many units under
//              PALETTESIZE - 1.  This avoids both sudden elevation changes
//              and flat spots.
//
//----------------------------------------------------------------------------

inline void Normalize(int & iTemp)
{
    while (iTemp < 0 || iTemp >= PALETTESIZE - 1)
    {
        if (iTemp < 0)
        {
            iTemp = 0 - iTemp;
        }
        else
        {
            iTemp = ((PALETTESIZE - 1) * 2) - (iTemp + 1);
        }
    }
}

//+---------------------------------------------------------------------------
//
//  Member:     CPlasma::Subdivide
//
//  Synopsis:   recurses into  a rectangle
//
//  Arguments:  [xl] - left side
//              [yl] - top side
//              [xh] - right side
//              [yh] - bottom side
//
//  Returns:    nothing
//
//  History:    4-25-94   stevebl   Created
//
//  Notes:      Each ittertation is responsible for drawing the
//              points marked X below if the haven't already been drawn.
//              The O's are points drawn by previous ittertations.
//              The .'s represent points to be drawn by future itterations.
//
//              O . . . . . X . . . . . O
//              . . . . . . . . . . . . .
//              . . . . . . . . . . . . .
//              . . . . . . . . . . . . .
//              . . . . . . . . . . . . .
//              . . . . . . . . . . . . .
//              X . . . . . X . . . . . X
//              . . . . . . . . . . . . .
//              . . . . . . . . . . . . .
//              . . . . . . . . . . . . .
//              . . . . . . . . . . . . .
//              . . . . . . . . . . . . .
//              O . . . . . X . . . . . O
//
//              Once the routine has plotted its points, it divides
//              the graph into quarters and calls itself
//              for each quarter.
//
//              Since the graph is erased to color 0 before itterations are
//              begun, testing if a point has already been plotted is simple:
//              if the point contians color 0 then it hasn't been plotted yet.
//
//----------------------------------------------------------------------------

void CPlasma::Subdivide(int xl, int yl, int xh, int yh)
{
    if (WAIT_TIMEOUT == WaitForSingleObject(_hRestart, 0))
    {
        WaitForSingleObject(_hRunning, INFINITE);
        // find the midpoints
        int iMidX = (xh + xl) / 2;
        int iMidY = (yh + yl) / 2;

        // find the four corner colors
        unsigned char cTL, cTR, cBL, cBR;
        unsigned uTemp;
        _pfh->GetPoint(&uTemp, xl, yl);
        cTL = uTemp - 1;
        _pfh->GetPoint(&uTemp, xh % _uWidth, yl);
        cTR = uTemp - 1;
        _pfh->GetPoint(&uTemp, xl, yh % _uHeight);
        cBL = uTemp - 1;
        _pfh->GetPoint(&uTemp, xh % _uWidth, yh % _uHeight);
        cBR = uTemp - 1;
        int cTM, cLM, cRM, cBM;

        unsigned uSpread = (unsigned)((xh - xl) * _dRandomnessFactor);

        // choose the top point
        _pfh->GetPoint(&uTemp, iMidX, yl);
        if (0 == uTemp)
        {
            cTM = (rand() % (uSpread * 2 + 1)) - uSpread;
            cTM += (cTL + cTR) / 2;
            Normalize(cTM);
            _pfh->Point(iMidX, yl, cTM + 1);
        }

        // choose the bottom point
        _pfh->GetPoint(&uTemp, iMidX, yh % _uHeight);
        if (0 == uTemp)
        {
            cBM = (rand() % (uSpread * 2 + 1)) - uSpread;
            cBM += (cBL + cBR) / 2;
            Normalize(cBM);
            // Don't need to use yh % _uHeight here because the only
            // way we can get here is if yh < _uHeight.
            _pfh->Point(iMidX, yh, cBM + 1);
        }

        uSpread = (unsigned)((yh - yl) * _dRandomnessFactor);

        // choose the left point
        _pfh->GetPoint(&uTemp,xl, iMidY);
        if (0 == uTemp)
        {
            cLM = (rand() % (uSpread * 2 + 1)) - uSpread;
            cLM += (cTL + cBL) / 2;
            Normalize(cLM);
            _pfh->Point(xl, iMidY, cLM + 1);
        }

        // choose the right point
        _pfh->GetPoint(&uTemp, xh % _uWidth, iMidY);
        if (0 == uTemp)
        {
            cRM = (rand() % (uSpread * 2 + 1)) - uSpread;
            cRM += (cTR + cBR) / 2;
            Normalize(cRM);
            // Don't need to use xh % _uWidth here because the only
            // way we can get here is if xh < _uWidth.
            _pfh->Point(xh, iMidY, cRM + 1);
        }

        _pfh->GetPoint(&uTemp, iMidX, iMidY);
        if (0 == uTemp)
        {
            // choose the center point
            uSpread = (unsigned)((sqrt((xh - xl) * (xh - xl) + (yh - yl) * (yh - yl)) / 2) * _dRandomnessFactor);
            int cC = (rand() % (uSpread * 2 + 1)) - uSpread;
            cC += (cTL + cBR + cTR + cBL) / 4;
            Normalize(cC);
            _pfh->Point(iMidX, iMidY, cC + 1);
        }

        // now recurse
        if (xh - xl > 2 || yh - yl > 2)
        {
            Subdivide(xl, yl, iMidX, iMidY);
            Subdivide(iMidX, yl, xh, iMidY);
            Subdivide(xl, iMidY, iMidX, yh);
            Subdivide(iMidX, iMidY, xh, yh);
        }
    }
}

//+---------------------------------------------------------------------------
//
//  Member:     CPlasma::CPlasma
//
//  Synopsis:   constructor
//
//  History:    4-23-94   stevebl   Created
//
//----------------------------------------------------------------------------

CPlasma::CPlasma()
{
    _uWidth = 160;
    _uHeight = 120;
    _dRandomnessFactor = 2.0;
    _hRestart = _hRunning = _hEngine = NULL;
    _dwThreadId = 0;
    _cRef = 0;
    _pfh = NULL;
    _fDirty = TRUE;
}

//+---------------------------------------------------------------------------
//
//  Member:     CPlasma::~CPlasma
//
//  Synopsis:   destructor
//
//  History:    4-23-94   stevebl   Created
//
//----------------------------------------------------------------------------

CPlasma::~CPlasma()
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
//  Member:     CPlasma::DialogProc
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

BOOL CPlasma::DialogProc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    char mbs[LENGTH + 1];

    switch (uMsg)
    {
    case WM_INITDIALOG:
        sprintf(mbs, "%.13g", _dRandomnessFactor);
        SetDlgItemTextA(hwndDlg, IDM_RANDOMNESS, mbs);
        return(TRUE);
    case WM_COMMAND:
        switch (LOWORD(wParam))
        {
        case IDOK:
            GetDlgItemTextA(hwndDlg, IDM_RANDOMNESS, mbs, LENGTH);
            sscanf(mbs, "%lg", &_dRandomnessFactor);
            if (_dRandomnessFactor < MIN_RANDOM)
            {
                _dRandomnessFactor = MIN_RANDOM;
            }
            if (_dRandomnessFactor > MAX_RANDOM)
            {
                _dRandomnessFactor = MAX_RANDOM;
            }
            _fDirty = TRUE;
            SetEvent(_hRestart);
            EndDialog(hwndDlg, TRUE);
            return(TRUE);
        case IDM_ABOUT:
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

