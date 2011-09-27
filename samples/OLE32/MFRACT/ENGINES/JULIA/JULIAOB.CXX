//+---------------------------------------------------------------------------
//
//  Microsoft Windows
//  Copyright (C) Microsoft Corporation, 1992 - 1995.
//
//  File:       juliaob.cxx
//
//  Contents:   Julia Fractal engine object
//
//  Classes:    CJulia
//
//  Functions:
//
//  History:    4-14-94   stevebl   Created
//
//----------------------------------------------------------------------------

#include <windows.h>
#include <ole2.h>
#include <quadcid.h>
#include <juliacid.h>
#include "julia.h"
#include <stdio.h>
#include <strmhelp.h>
#include <palsize.h>

//+---------------------------------------------------------------------------
//
//  Member:     CJulia::QueryInterface
//
//  Synopsis:   standard OLE method
//
//  Arguments:  [riid] - id of the desired interface
//              [ppv]  - pointer to receive the interface
//
//  Returns:    S_OK on success
//              E_NOINTERFACE if the requested interface isn't supported
//
//  History:    4-14-94   stevebl   Created
//
//----------------------------------------------------------------------------

HRESULT STDMETHODCALLTYPE CJulia::QueryInterface(REFIID riid, LPVOID * ppv)
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
    else if (IsEqualGUID(IID_IQuadrantClient, riid))
    {
        *ppv = (IQuadrantClient *)this;
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
//  Member:     CJulia::AddRef
//
//  Synopsis:   increments the reference count
//
//  Returns:    new reference count
//
//  History:    4-14-94   stevebl   Created
//
//----------------------------------------------------------------------------

ULONG STDMETHODCALLTYPE CJulia::AddRef(void)
{
    return(++_cRef);
}

//+---------------------------------------------------------------------------
//
//  Member:     CJulia::Release
//
//  Synopsis:   decrements the reference count
//
//  Returns:    new reference count
//
//  History:    4-14-94   stevebl   Created
//
//----------------------------------------------------------------------------

ULONG STDMETHODCALLTYPE CJulia::Release(void)
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
//  Member:     CJulia::GetClassID
//
//  Synopsis:   returns the object's class ID
//
//  Arguments:  [pclsid] - pointer to recieve the class ID
//
//  Returns:    S_OK
//
//  History:    4-21-94   stevebl   Created
//
//----------------------------------------------------------------------------

HRESULT CJulia::GetClassID(LPCLSID pclsid)
{
    memcpy(pclsid, &CLSID_JULIASET, sizeof(GUID));
    return(S_OK);
}

// IPersistStream methods

//+---------------------------------------------------------------------------
//
//  Member:     CJulia::IsDirty
//
//  Synopsis:   used to determine if the state of the object has changed
//
//  Returns:    S_OK if it is dirty
//              S_FALSE if it is not dirty
//
//  History:    4-21-94   stevebl   Created
//
//----------------------------------------------------------------------------

HRESULT CJulia::IsDirty(void)
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
//  Member:     CJulia::Load
//
//  Synopsis:   loads the fractal properties from a stream
//
//  Arguments:  [pStm] - pointer to an IStream interface
//
//  Returns:    S_OK on success
//              E_FAIL on error
//              E_OUTOFMEMORY if there's not enough memory (can't happen here)
//
//  History:    4-21-94   stevebl   Created
//
//----------------------------------------------------------------------------

HRESULT CJulia::Load(LPSTREAM pStm)
{
    HRESULT hr;
    if (_fRunning)
    {
        return(E_FAIL);
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
    if (FAILED(hr = pStm->Read(&_uLimit, sizeof(_uLimit), NULL)))
    {
        return(hr);
    }
    if (FAILED(hr = ReadDouble(pStm, &_dCr, NULL)))
    {
        return(hr);
    }
    if (FAILED(hr = ReadDouble(pStm, &_dCi, NULL)))
    {
        return(hr);
    }
    if (FAILED(hr = pStm->Read(&_fUseBoundingBoxes, sizeof(_fUseBoundingBoxes), NULL)))
    {
        return(hr);
    }
    _fDirty = FALSE;
    return(S_OK);
}

//+---------------------------------------------------------------------------
//
//  Member:     CJulia::Save
//
//  Synopsis:   saves the fractal properties to a stream
//
//  Arguments:  [pStm]        - pointer to an IStream interface
//              [fClearDirty] - TRUE if the dirty flag should be cleared
//
//  Returns:    S_OK on success
//
//  History:    4-21-94   stevebl   Created
//
//----------------------------------------------------------------------------

HRESULT CJulia::Save(LPSTREAM pStm, BOOL fClearDirty)
{
    if (_fRunning)
    {
        return(STG_E_CANTSAVE);
    }
    HRESULT hr;
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
    if (FAILED(hr = pStm->Write(&_uLimit, sizeof(_uLimit), NULL)))
    {
        return(hr);
    }
    if (FAILED(hr = WriteDouble(pStm, _dCr, NULL)))
    {
        return(hr);
    }
    if (FAILED(hr = WriteDouble(pStm, _dCi, NULL)))
    {
        return(hr);
    }
    if (FAILED(hr = pStm->Write(&_fUseBoundingBoxes, sizeof(_fUseBoundingBoxes), NULL)))
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
//  Member:     CJulia::GetSizeMax
//
//  Synopsis:   used to determine the maximum size of the object's data
//
//  Arguments:  [pcbSize] - pointer to recieve the max size
//
//  Returns:    S_OK
//
//  History:    4-21-94   stevebl   Created
//
//----------------------------------------------------------------------------

HRESULT CJulia::GetSizeMax(ULARGE_INTEGER * pcbSize)
{
    pcbSize->HighPart = 0;
    pcbSize->LowPart = SizeDouble() * 6 + sizeof(_uLimit);
    return(S_OK);
}

// IFractalEngine methods

//+---------------------------------------------------------------------------
//
//  Member:     CJulia::Init
//
//  Synopsis:   Initializes the graph engine and saves a pointer to the
//              IFractalHost interface that WinFract passes in as
//              the engine is initialized.
//
//  Arguments:  [pfh] - pointer to an IFractalHost interface
//
//  Returns:    S_OK
//
//  History:    4-14-94   stevebl   Created
//
//----------------------------------------------------------------------------

HRESULT STDMETHODCALLTYPE CJulia::Init(IFractalHost *pfh)
{
    _pfh = pfh;
    // NOTE that I don't addref this pointer!  That would create a circular
    // reference count paradox.

    _pqe->Init((IQuadrantClient *)this, pfh);
    _pqe->UseBoundingBoxes(_fUseBoundingBoxes);

    return(S_OK);
}

//+---------------------------------------------------------------------------
//
//  Member:     CJulia::SetDefaults
//
//  Synopsis:   sets default values for the Julia Set
//
//  History:    4-22-94   stevebl   Created
//
//----------------------------------------------------------------------------

HRESULT STDMETHODCALLTYPE CJulia::SetDefaults(void)
{
    if (_uWidth > _uHeight)
    {
        // image is wider than it is tall
        _dTop = 2.0;
        _dBottom = -2.0;
        _dLeft = (-2.0 * _uWidth) / _uHeight;
        _dRight = (2.0 * _uWidth) / _uHeight;
    }
    else
    {
        // image is taller than it is wide
        _dLeft = -2.0;
        _dRight = 2.0;
        _dTop = (2.0 * _uHeight) / _uWidth;
        _dBottom = (-2.0 * _uHeight) / _uWidth;
    }
    _uLimit = 200;
    _pqe->Reset();
    return(S_OK);
}

//+---------------------------------------------------------------------------
//
//  Member:     CJulia::SetProperties
//
//  Synopsis:   displays the properties dialog box
//
//  Arguments:  [hwnd] - parent window
//
//  History:    4-22-94   stevebl   Created
//
//----------------------------------------------------------------------------

HRESULT STDMETHODCALLTYPE CJulia::SetProperties(HWND hwnd)
{
    ShowDialog(ghinst, MAKEINTRESOURCE(PROPERTIES), hwnd);
    return(S_OK);
}

//+---------------------------------------------------------------------------
//
//  Member:     CJulia::GetExtent
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
//  History:    4-14-94   stevebl   Created
//
//----------------------------------------------------------------------------

HRESULT STDMETHODCALLTYPE CJulia::GetExtent(
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
//  Member:     CJulia::SetExtent
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
//  History:    4-14-94   stevebl   Created
//
//----------------------------------------------------------------------------

HRESULT STDMETHODCALLTYPE CJulia::SetExtent(
    double dLeft,
    double dTop,
    double dRight,
    double dBottom)
{
    if (_fRunning)
    {
        return(E_FAIL);
    }
    _dLeft = dLeft;
    _dTop = dTop;
    _dRight = dRight;
    _dBottom = dBottom;
    _pqe->Reset();
    return(S_OK);
}

//+---------------------------------------------------------------------------
//
//  Member:     CJulia::SetGraphSize
//
//  Synopsis:   Tells the graph engine how big the graph is
//
//  Arguments:  [uWidth]  - width of the graph (in pixels)
//              [uHeight] - height of the graph (in pixels)
//
//  Returns:    S_OK - success
//              E_FAIL - the graph was running or a value was illegal
//
//  History:    4-14-94   stevebl   Created
//
//----------------------------------------------------------------------------

HRESULT STDMETHODCALLTYPE CJulia::SetGraphSize(
    unsigned int uWidth,
    unsigned int uHeight)
{
    if (_fRunning || uWidth == 0 || uHeight == 0)
    {
        return(E_FAIL);
    }
    _uWidth = uWidth;
    _uHeight = uHeight;
    return(_pqe->SetGraphSize(uWidth, uHeight));
}

//+---------------------------------------------------------------------------
//
//  Member:     CJulia::Start
//
//  Synopsis:   starts the graph engine
//
//  History:    4-22-94   stevebl   Created
//
//----------------------------------------------------------------------------

HRESULT STDMETHODCALLTYPE CJulia::Start(void)
{
    _fRunning = TRUE;
    return(_pqe->Start());
}

//+---------------------------------------------------------------------------
//
//  Member:     CJulia::Stop
//
//  Synopsis:   Stops the graph engine
//
//  History:    4-22-94   stevebl   Created
//
//----------------------------------------------------------------------------

HRESULT STDMETHODCALLTYPE CJulia::Stop(void)
{
    _fRunning = FALSE;
    return(_pqe->Stop());
}

// IQuadrantClient methods

//+---------------------------------------------------------------------------
//
//  Member:     CJulia::ComputePoint
//
//  Synopsis:   Returns what color should be painted at this particular point
//              on the graph.
//
//  Arguments:  [puColor] - pointer to variable to recieve the color
//              [x]       - x position of the pixel
//              [y]       - y position of the pixel
//
//  Returns:    S_OK
//
//  Notes:      Assume there are PALETTESIZE colors in the palette.
//              Colors 1 to PALETTESIZE-1 can cycle; 0 cannot.
//
//  History:    4-14-94   stevebl   Created
//
//----------------------------------------------------------------------------

HRESULT STDMETHODCALLTYPE CJulia::ComputePoint(
    unsigned * puColor,
    int x,
    int y)
{
    unsigned c = 1;

    //  The Julia Set formula is: Z' <= Z^2 + C

    double
        dZr = 0.0,          // real component of Z
        dZi = 0.0,          // imaginary component of Z
        dZiSqr = 0.0,       // Zi squared
        dZrSqr = 0.0,       // Zr squared
        dZr1;               // temporary holder for Zr

    dZr = ((x * (_dRight - _dLeft)) / _uWidth) + _dLeft;
    dZi = ((((_uHeight - 1) - y) * (_dTop - _dBottom)) / _uHeight) + _dBottom;
    if (dZr < -2 || dZr > 2 || dZi < -2 || dZi > 2)
    {
        *puColor = 1;
        return(S_OK);
    }
    dZiSqr = dZi * dZi;
    dZrSqr = dZr * dZr;
    while (c < _uLimit && dZiSqr + dZrSqr < 4)
    {
        dZr1 = dZrSqr - dZiSqr + _dCr;
        dZi = 2 * dZr * dZi + _dCi;
        dZr = dZr1;
        dZiSqr = dZi * dZi;
        dZrSqr = dZr * dZr;
        ++c;
    }
    if (c >= _uLimit)
    {
        *puColor = 0;
        return(S_OK);
    }
    *puColor = ((c - 1) % (PALETTESIZE - 1)) + 1;
    return(S_OK);
}

//+---------------------------------------------------------------------------
//
//  Member:     CJulia::DoneDrawingGraph
//
//  Synopsis:   called by the quad helper engine when the graph is done
//
//  History:    4-22-94   stevebl   Created
//
//----------------------------------------------------------------------------

HRESULT STDMETHODCALLTYPE CJulia::DoneDrawingGraph(void)
{
    _fRunning = FALSE;
    return(S_OK);
}

// Methods that aren't part of any interface

//+---------------------------------------------------------------------------
//
//  Member:     CJulia::Initialize
//
//  Synopsis:   initialize the object
//
//  Returns:    TRUE on success, FALSE on failure
//
//  History:    4-22-94   stevebl   Created
//
//----------------------------------------------------------------------------

BOOL CJulia::Initialize(void)
{
    HRESULT hr = CoCreateInstance(
        CLSID_QUADRANTENGINE,
        NULL,
        CLSCTX_INPROC_SERVER | CLSCTX_INPROC_HANDLER,
        IID_IQuadrantEngine,
        (void **)&_pqe);
    if (FAILED(hr))
    {
        // couldn't get an IQuadrantEngine interface
        return(FALSE);
    }
    return(TRUE);
}

//+---------------------------------------------------------------------------
//
//  Member:     CJulia::CJulia
//
//  Synopsis:   constructor
//
//  History:    4-14-94   stevebl   Created
//
//----------------------------------------------------------------------------

CJulia::CJulia()
{
    _dLeft = -2.0;
    _dTop = 2.0;
    _dRight = 2.0;
    _dBottom = -2.0;
    _uWidth = 160;
    _uHeight = 120;
    _dCr = -1.25;
    _dCi = 0.1;
    _fRunning = FALSE;
    _cRef = 0;
    _pfh = NULL;
    _pqe = NULL;
    _uLimit = 200;  // default limit
    _fDirty = TRUE;
    _fUseBoundingBoxes = TRUE;
}

//+---------------------------------------------------------------------------
//
//  Member:     CJulia::~CJulia
//
//  Synopsis:   destructor
//
//  History:    4-14-94   stevebl   Created
//
//----------------------------------------------------------------------------

CJulia::~CJulia()
{
    if (_pqe)
    {
        _pqe->Release();
    }
}

#define LENGTH 20
#define ABOUT_STRING_LENGTH 256

//+---------------------------------------------------------------------------
//
//  Member:     CJulia::DialogProc
//
//  Synopsis:   Dialog proc for the Properties dialog box
//
//  Arguments:  [hwndDlg] - handle to the dialog
//              [uMsg]    - message
//              [wParam]  - first message parameter
//              [lParam]  - second message parameter
//
//  History:    4-18-94   stevebl   Created
//
//----------------------------------------------------------------------------

BOOL CJulia::DialogProc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    char mbs[LENGTH + 1];

    switch (uMsg)
    {
    case WM_INITDIALOG:
        CheckDlgButton(
            hwndDlg,
            IDC_USEBOUNDINGBOXES,
            _fUseBoundingBoxes);
        sprintf(mbs, "%.13lg", _dLeft);
        SetDlgItemTextA(hwndDlg, IDC_LOWREAL, mbs);
        sprintf(mbs, "%.13lg", _dBottom);
        SetDlgItemTextA(hwndDlg, IDC_LOWIMAGINARY, mbs);
        sprintf(mbs, "%.13lg", _dRight);
        SetDlgItemTextA(hwndDlg, IDC_HIGHREAL, mbs);
        sprintf(mbs, "%.13lg", _dTop);
        SetDlgItemTextA(hwndDlg, IDC_HIGHIMAGINARY, mbs);
        sprintf(mbs, "%.13lg", _dCr);
        SetDlgItemTextA(hwndDlg, IDC_CREAL, mbs);
        sprintf(mbs, "%.13lg", _dCi);
        SetDlgItemTextA(hwndDlg, IDC_CIMAGINARY, mbs);
        sprintf(mbs, "%d", _uLimit);
        SetDlgItemTextA(hwndDlg, IDC_LIMIT, mbs);
        return(TRUE);
    case WM_COMMAND:
        switch (LOWORD(wParam))
        {
        case IDOK:
            _fUseBoundingBoxes = IsDlgButtonChecked(hwndDlg, IDC_USEBOUNDINGBOXES);
            _pqe->UseBoundingBoxes(_fUseBoundingBoxes);
            GetDlgItemTextA(hwndDlg, IDC_LOWREAL, mbs, LENGTH);
            sscanf(mbs, "%lf", &_dLeft);
            GetDlgItemTextA(hwndDlg, IDC_LOWIMAGINARY, mbs, LENGTH);
            sscanf(mbs, "%lf", &_dBottom);
            GetDlgItemTextA(hwndDlg, IDC_HIGHREAL, mbs, LENGTH);
            sscanf(mbs, "%lf", &_dRight);
            GetDlgItemTextA(hwndDlg, IDC_HIGHIMAGINARY, mbs, LENGTH);
            sscanf(mbs, "%lf", &_dTop);
            GetDlgItemTextA(hwndDlg, IDC_CREAL, mbs, LENGTH);
            sscanf(mbs, "%lf", &_dCr);
            GetDlgItemTextA(hwndDlg, IDC_CIMAGINARY, mbs, LENGTH);
            sscanf(mbs, "%lf", &_dCi);
            GetDlgItemTextA(hwndDlg, IDC_LIMIT, mbs, LENGTH);
            sscanf(mbs, "%u", &_uLimit);
            if (_uLimit < 1)
            {
                _uLimit = 1;
            }
            _pqe->Reset();
            EndDialog(hwndDlg, TRUE);
            return(TRUE);
        case IDC_ABOUT:
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

