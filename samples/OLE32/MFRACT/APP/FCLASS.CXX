//+---------------------------------------------------------------------------
//
//  Microsoft Windows
//  Copyright (C) Microsoft Corporation, 1992 - 1995.
//
//  File:       fclass.cxx
//
//  Contents:   implementation for CFractalWindow
//
//  Classes:    CFractalWindow
//
//  Functions:
//
//  History:    4-11-94   stevebl   Created
//
//----------------------------------------------------------------------------

#include <windows.h>
#include <ole2.h>
#include "fractal.h"
#include "fclass.h"
#include "about.h"
#include "chngdim.h"
#include "dib.h"

#define START_IN_QUICKDRAWMODE

#define WM_REFRESHCOLORS    WM_USER

#define SCROLL_STEP 10

//+---------------------------------------------------------------------------
//
//  Member:     CFractalWindow::DrawRect
//
//  Synopsis:   Draws the zoom rectangle
//
//  Arguments:  [hdc]  - handle to the display context that contains the graph
//              [lprc] - rectangle containing the zoom rect's dimensions
//
//  Returns:    TRUE on success
//
//  History:    5-02-94   stevebl   Created
//
//----------------------------------------------------------------------------

BOOL CFractalWindow::DrawRect(HDC hdc, const RECT * lprc)
{
   HPALETTE hOldPal =  SelectPalette(hdc, _hpal, FALSE);
   RealizePalette(hdc);
   HBRUSH hbr = CreateSolidBrush(WHITENESS);
   BOOL fReturn = FrameRect(hdc, lprc, hbr);
   DeleteObject(hbr);
   SelectPalette(hdc, hOldPal, TRUE);
   RealizePalette(hdc);
   return(fReturn);
}

//+---------------------------------------------------------------------------
//
//  Member:     CFractalWindow::EraseRect
//
//  Synopsis:   Erases the zoom rectangle
//
//  Arguments:  [hdc]  - handle to a display context
//              [lprc] - rectangle containing the dimensions of the zoom rect
//
//  Returns:    TRUE on success
//
//  History:    5-02-94   stevebl   Created
//
//----------------------------------------------------------------------------

BOOL CFractalWindow::EraseRect(HDC hdc, const RECT * lprc)
{
   HPALETTE hOldPal =  SelectPalette(hdc, _hpal, FALSE);
   RealizePalette(hdc);
   BOOL fReturn;
   fReturn = BitBlt(hdc,
      lprc->left,
      lprc->top,
      lprc->right - lprc->left,
      1,
      _hdcMem,
      lprc->left + _uScrollPosX,
      lprc->top + _uScrollPosY,
      SRCCOPY);
   fReturn &= BitBlt(hdc,
      lprc->left,
      lprc->top,
      1,
      lprc->bottom - lprc->top,
      _hdcMem,
      lprc->left + _uScrollPosX,
      lprc->top + _uScrollPosY,
      SRCCOPY);
   fReturn &= BitBlt(hdc,
      lprc->right - 1,
      lprc->top,
      1,
      lprc->bottom - lprc->top,
      _hdcMem,
      (lprc->right - 1) + _uScrollPosX,
      lprc->top + _uScrollPosY,
      SRCCOPY);
   fReturn &= BitBlt(hdc,
      lprc->left,
      lprc->bottom - 1,
      lprc->right - lprc->left,
      1,
      _hdcMem,
      lprc->left + _uScrollPosX,
      (lprc->bottom - 1) + _uScrollPosY,
      SRCCOPY);
   SelectPalette(hdc, hOldPal, TRUE);
   RealizePalette(hdc);
   return(fReturn);
}

//+---------------------------------------------------------------------------
//
//  Member:     CFractalWindow::GetHwnd
//
//  Synopsis:   simple method to retrieve the HWND of the window
//
//  Returns:    the window handle to the window
//
//  History:    4-26-94   stevebl   Created
//
//----------------------------------------------------------------------------

HWND CFractalWindow::GetHwnd(void)
{
    return(_hwnd);
}

//+---------------------------------------------------------------------------
//
//  Member:     CFractalWindow::InitInstance
//
//  Synopsis:   initializes an instance of a window object belonging to the
//              CFractalWindow class
//
//  Arguments:  [hInstance] - instance handle
//              [nCmdShow]  - show command
//
//  History:    4-11-94   stevebl   Created
//
//----------------------------------------------------------------------------

BOOL CFractalWindow::InitInstance(HINSTANCE hInstance, int nCmdShow)
{
    HWND hwnd = Create(
        TEXT("CFractalWindow"),
        TEXT(APP_TITLE),
        WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX
            | WS_MAXIMIZEBOX | WS_THICKFRAME | WS_VSCROLL | WS_HSCROLL,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        NULL,
        NULL,
        hInstance);
    if (!hwnd)
    {
        return(FALSE);
    }

    if (!SetTimer(hwnd, REFRESH_TIMER, _uRefreshRate, NULL))
    {
        TCHAR szTitle[MAX_STRING_LENGTH];
        TCHAR szText[MAX_STRING_LENGTH];
        if (LoadString(_hInstance, IDS_ERROR, szTitle, MAX_STRING_LENGTH))
        {
            if (LoadString(_hInstance, IDS_TOOMANYTIMERS, szText, MAX_STRING_LENGTH))
            {
                MessageBox(
                    hwnd,
                    szText,
                    szTitle,
                    MB_OK | MB_ICONEXCLAMATION);
            }
        }
        return(FALSE);
    }


    _hGraphStartedEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
    if (!_hGraphStartedEvent)
    {
        TCHAR szTitle[MAX_STRING_LENGTH];
        TCHAR szText[MAX_STRING_LENGTH];
        if (LoadString(_hInstance, IDS_ERROR, szTitle, MAX_STRING_LENGTH))
        {
            if (LoadString(_hInstance, IDS_GRAPHICSEVENTFAILED, szText, MAX_STRING_LENGTH))
            {
                MessageBox(
                    hwnd,
                    szText,
                    szTitle,
                    MB_OK | MB_ICONEXCLAMATION);
            }
        }
        return(FALSE);
    }

    _hTitleEvent = CreateEvent(NULL, TRUE, TRUE, NULL);
    if (!_hTitleEvent)
    {
        TCHAR szTitle[MAX_STRING_LENGTH];
        TCHAR szText[MAX_STRING_LENGTH];
        if (LoadString(_hInstance, IDS_ERROR, szTitle, MAX_STRING_LENGTH))
        {
            if (LoadString(_hInstance, IDS_TITLEEVENTFAILED, szText, MAX_STRING_LENGTH))
            {
                MessageBox(
                    hwnd,
                    szText,
                    szTitle,
                    MB_OK | MB_ICONEXCLAMATION);
            }
        }
        return(FALSE);
    }

    // build a path to where the help file should be
    // (it should be in the same directory as the .EXE)
    // first get the exe's path
    DWORD cch = GetModuleFileName(hInstance, _szHelpFile, MAX_PATH);
    // then back up past the name of the application
    while (cch > 0 &&
        _szHelpFile[cch - 1] != TEXT('\\') &&
        _szHelpFile[cch - 1] != TEXT('/') &&
        _szHelpFile[cch - 1] != TEXT(':'))
    {
        cch--;
    }
    // finally copy the help file's name at the end of the path
    lstrcpy(&_szHelpFile[cch], TEXT(HELPFILE_STR));

    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);
    return(TRUE);
}

//+---------------------------------------------------------------------------
//
//  Member:     CFractalWindow::NewWindowSize
//
//  Synopsis:   updates the scroll bars every time the window's size changes
//
//  Arguments:  [uWidth]  - window width
//              [uHeight] - window height
//
//  Modifies:   _uWindowWidth, _uWindowHeight
//
//  History:    4-12-94   stevebl   Created
//
//----------------------------------------------------------------------------

void CFractalWindow::NewWindowSize(unsigned uWidth, unsigned uHeight)
{
    BOOL fRedraw = FALSE;
    _uWindowWidth = uWidth;
    _uWindowHeight = uHeight;
    unsigned uVRange = (uHeight > _uImageHeight ? 0 : _uImageHeight-uHeight);
    unsigned uHRange = (uWidth > _uImageWidth ? 0 : _uImageWidth-uWidth);
    SetScrollRange(_hwnd, SB_HORZ, 0, uHRange, FALSE);
    SetScrollRange(_hwnd, SB_VERT, 0, uVRange, FALSE);
    if (_uScrollPosX > uHRange)
    {
        fRedraw = TRUE;
        _uScrollPosX = uHRange;
    }
    if (_uScrollPosY > uVRange)
    {
        fRedraw = TRUE;
        _uScrollPosY = uVRange;
    }
    if (fRedraw)
    {
        InvalidateRect(_hwnd, NULL, FALSE);
    }
    SetScrollPos(_hwnd, SB_HORZ, _uScrollPosX, TRUE);
    SetScrollPos(_hwnd, SB_VERT, _uScrollPosY, TRUE);
}

//+---------------------------------------------------------------------------
//
//  Member:     CFractalWindow::OnCreate
//
//  Synopsis:   called when the window is first created (on WM_CREATE)
//              to initialize all relevent member variables
//
//  Modifies:   _hdcMem, _uScreenWidth, _uScreenHeight, etc.
//
//  History:    4-12-94   stevebl   Created
//
//  Notes:      Calls SetGraphSize to initialize the graph.
//
//----------------------------------------------------------------------------

void CFractalWindow::OnCreate(void)
{
    HDC hdc = GetDC(_hwnd);
    _hdcMem = CreateCompatibleDC(hdc);
    _uScreenWidth = GetDeviceCaps(hdc, HORZRES);
    _uScreenHeight = GetDeviceCaps(hdc, VERTRES);

    // If we don't have an animating palette with 256 colors and
    // exactly 20 static colors then we're going to disable our
    // animate palette features.

    _fCanAnimate = (GetDeviceCaps(hdc, RASTERCAPS) & RC_PALETTE) != 0
        && GetDeviceCaps(hdc, SIZEPALETTE) == 256
        && GetDeviceCaps(hdc, NUMRESERVED) == 20;

    ReleaseDC(_hwnd, hdc);
    SetUpColorTable();
#ifdef START_IN_QUICKDRAWMODE
    SetGraphSize(QUICKWIDTH, QUICKHEIGHT);
#else
    SetGraphSize(_uScreenWidth, _uScreenHeight);
#endif
    SetDefaultColors();
    // Fill in the Palette's open file name structure
    _szPalFileName[0] = 0;
    _szPalFileTitle[0] = 0;
    _ofnPalette.lStructSize = sizeof(OPENFILENAME);
    _ofnPalette.hwndOwner = _hwnd;
    _ofnPalette.lpstrFilter = TEXT("Palette Files (*.PAL)\0*.PAL\0All Files (*.*)\0\\*.*\0");
    _ofnPalette.lpstrCustomFilter = NULL;
    _ofnPalette.nMaxCustFilter = 0;
    _ofnPalette.nFilterIndex = 1;
    _ofnPalette.lpstrFile = _szPalFileName;
    _ofnPalette.nMaxFile = MAX_PATH;
    _ofnPalette.lpstrInitialDir = NULL;
    _ofnPalette.lpstrFileTitle = _szPalFileTitle;
    _ofnPalette.nMaxFileTitle = MAX_PATH;
    _ofnPalette.lpstrTitle = NULL;
    _ofnPalette.lpstrDefExt = TEXT("PAL");
    _ofnPalette.Flags = OFN_HIDEREADONLY;

    // Fill in the BMP's open file name structure
    _szBmpFileName[0] = 0;
    _szBmpFileTitle[0] = 0;
    _ofnBitmap.lStructSize = sizeof(OPENFILENAME);
    _ofnBitmap.hwndOwner = _hwnd;
    _ofnBitmap.lpstrFilter = TEXT("Bitmaps (*.BMP)\0*.BMP\0All Files (*.*)\0\\*.*\0");
    _ofnBitmap.lpstrCustomFilter = NULL;
    _ofnBitmap.nMaxCustFilter = 0;
    _ofnBitmap.nFilterIndex = 1;
    _ofnBitmap.lpstrFile = _szBmpFileName;
    _ofnBitmap.nMaxFile = MAX_PATH;
    _ofnBitmap.lpstrInitialDir = NULL;
    _ofnBitmap.lpstrFileTitle = _szBmpFileTitle;
    _ofnBitmap.nMaxFileTitle = MAX_PATH;
    _ofnBitmap.lpstrTitle = NULL;
    _ofnBitmap.lpstrDefExt = TEXT("BMP");
    _ofnBitmap.Flags = OFN_HIDEREADONLY;

    // Fill in the File's open file name structure
    _szFileName[0] = 0;
    _szFileTitle[0] = 0;
    _ofnFile.lStructSize = sizeof(OPENFILENAME);
    _ofnFile.hwndOwner = _hwnd;
    _ofnFile.lpstrFilter = TEXT("Fractal Files (*.MFR)\0*.MFR\0All Files (*.*)\0\\*.*\0");
    _ofnFile.lpstrCustomFilter = NULL;
    _ofnFile.nMaxCustFilter = 0;
    _ofnFile.nFilterIndex = 1;
    _ofnFile.lpstrFile = _szFileName;
    _ofnFile.nMaxFile = MAX_PATH;
    _ofnFile.lpstrInitialDir = NULL;
    _ofnFile.lpstrFileTitle = _szFileTitle;
    _ofnFile.nMaxFileTitle = MAX_PATH;
    _ofnFile.lpstrTitle = NULL;
    _ofnFile.lpstrDefExt = TEXT("MFR");
    _ofnFile.Flags = OFN_HIDEREADONLY;
}

#include <stdio.h>

//+---------------------------------------------------------------------------
//
//  Member:     CFractalWindow::ChangeFractalEngines
//
//  Synopsis:   releases the old fractal engine and binds to the new one
//
//  History:    4-15-94   stevebl   Created
//
//----------------------------------------------------------------------------

void CFractalWindow::ChangeFractalEngines(void)
{
    if(_pFractalEngine)
    {
        _pFractalEngine->Release();
    }
    HRESULT hr = CoCreateInstance(
        _pEngineList->GetEntry(_uFractalType),
        NULL,
        CLSCTX_INPROC_SERVER | CLSCTX_INPROC_HANDLER,
        IID_IFractalEngine,
        (void **)&_pFractalEngine);
    if (SUCCEEDED(hr))
    {
        hr = _pFractalEngine->Init((IFractalHost *)this);
    }
    else
    {
        _pFractalEngine = NULL;
    }
    if (SUCCEEDED(hr))
    {
        hr = _pFractalEngine->SetGraphSize(_uImageWidth, _uImageHeight);
    }
    if (SUCCEEDED(hr))
    {
        hr = _pFractalEngine->SetDefaults();
    }
    if (SUCCEEDED(hr))
    {
        hr = _pFractalEngine->GetExtent(&_dLeft, &_dTop, &_dRight, &_dBottom);
    }
    Rect(0, 0, (int)_uImageWidth - 1, (int)_uImageHeight - 1, 0);
    if (FAILED(hr))
    {
        if (_pFractalEngine)
        {
            _pFractalEngine->Release();
        }
        TCHAR szTitle[MAX_STRING_LENGTH];
        TCHAR szText[MAX_STRING_LENGTH];
        if (LoadString(_hInstance, IDS_ERROR, szTitle, MAX_STRING_LENGTH))
        {
            if (LoadString(_hInstance, IDS_NOFRACTALENGINE, szText, MAX_STRING_LENGTH))
            {
                MessageBox(
                    _hwnd,
                    szText,
                    szTitle,
                    MB_OK | MB_ICONEXCLAMATION);
            }
        }
        _pFractalEngine = NULL;
    }
}

//+---------------------------------------------------------------------------
//
//  Member:     CFractalWindow::WindowProc
//
//  Synopsis:   main message proc for CFractalWindow
//
//  Arguments:  [uMsg]   - message
//              [wParam] - first message parameter
//              [lParam] - second message parameter
//
//  History:    4-11-94   stevebl   Created
//
//----------------------------------------------------------------------------

LRESULT CFractalWindow::WindowProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    if (_fZooming)
    {
        switch (uMsg)
        {
        // this first set of messages fall through to the normal message
        // handler during a zoom
        case WM_PALETTECHANGED:
        case WM_QUERYNEWPALETTE:
        case WM_PAINT:
            break;

        // The next set of messages are handled differently during a zoom

        case WM_LBUTTONDBLCLK:
            // zoom in
            {
                HDC hdc = GetDC(_hwnd);
                EraseRect(hdc, &_rectZoom);
                ReleaseDC(_hwnd, hdc);
                OffsetRect(&_rectZoom, _uScrollPosX, _uScrollPosY);
                double dScaler = (_dRight - _dLeft) / (double)_uImageWidth;
                double dLeft = _dLeft
                    + (double)_rectZoom.left * dScaler;
                double dRight = _dLeft
                    + (double)_rectZoom.right * dScaler;
                double dBottom = _dBottom
                    + (double)(_uImageHeight - _rectZoom.bottom) * dScaler;
                double dTop = dBottom +
                     (_dTop - _dBottom) * (dRight - dLeft) / (_dRight - _dLeft);

                if (_pFractalEngine)
                {
                    _pFractalEngine->SetExtent(dLeft, dTop, dRight, dBottom);
                }
            }
            ReleaseCapture();
            ShowCursor(TRUE);
            _fZooming = FALSE;
            return 0;
        case WM_RBUTTONDBLCLK:
            // zoom out
            {
                HDC hdc = GetDC(_hwnd);
                EraseRect(hdc, &_rectZoom);
                ReleaseDC(_hwnd, hdc);
                OffsetRect(&_rectZoom, _uScrollPosX, _uScrollPosY);
                double dScaler = (_dRight - _dLeft) /
                    (double)(_rectZoom.right - _rectZoom.left);
                double dLeft = _dLeft
                    - (double)_rectZoom.left * dScaler;
                double dRight = _dRight +
                    (double)(_uImageWidth - _rectZoom.right) * dScaler;
                double dBottom = _dBottom -
                    (double)(_uImageHeight - _rectZoom.bottom) * dScaler;
                double dTop = _dTop +
                    (double)_rectZoom.top * dScaler;
                if (_pFractalEngine)
                {
                    _pFractalEngine->SetExtent(dLeft, dTop, dRight, dBottom);
                }
            }
            ReleaseCapture();
            ShowCursor(TRUE);
            _fZooming = FALSE;
            return 0;
        case WM_MOUSEMOVE:
            {
                HDC hdc = GetDC(_hwnd);
                EraseRect(hdc, &_rectZoom);
                POINT p;
                p.x = LOWORD(lParam);
                p.y = HIWORD(lParam);
                if (p.x > (int) _uImageWidth)
                {
                    p.x = _uImageWidth;
                }
                else if (p.x < 0)
                {
                    p.x = 0;
                }
                if (p.y > (int) _uImageHeight)
                {
                    p.y = _uImageHeight;
                }
                else if (p.y < 0)
                {
                    p.y = 0;
                }
                if (MK_RBUTTON == wParam || MK_LBUTTON == wParam)
                {
                    // change size of the zoom window
                    int dx, dy;
                    BOOL fChangeX;
                    fChangeX = abs(p.y - _rectZoom.bottom) >
                        abs(p.x - _rectZoom.right);
                    _rectZoom.bottom = p.y;
                    _rectZoom.right = p.x;
                    if (_rectZoom.right <= _rectZoom.left)
                    {
                        _rectZoom.right = _rectZoom.left + 1;
                    }
                    if (_rectZoom.bottom <= _rectZoom.top)
                    {
                        _rectZoom.bottom = _rectZoom.top + 1;
                    }
                    dx = _rectZoom.right - _rectZoom.left;
                    dy = _rectZoom.bottom - _rectZoom.top;
                    if (dx < 4)
                    {
                        dx = 4;
                        _rectZoom.right = _rectZoom.left + 4;
                    }
                    if (dy < 4)
                    {
                        dy = 4;
                        _rectZoom.bottom = _rectZoom.top + 4;
                    }
                    // normalize the zoom window
                    if (fChangeX)
                    {
                        _rectZoom.right = _rectZoom.left +
                            MulDiv(dy, _uImageWidth, _uImageHeight);
                    }
                    else
                    {
                        _rectZoom.bottom = _rectZoom.top +
                            MulDiv(dx, _uImageHeight, _uImageWidth);
                    }
                    p.x = _rectZoom.right;
                    p.y = _rectZoom.bottom;
                    ClientToScreen(_hwnd, &p);
                    SetCursorPos(p.x, p.y);
                }
                else
                {
                    // move the zoom window
                    OffsetRect(
                        &_rectZoom,
                        p.x - _rectZoom.right,
                        p.y - _rectZoom.bottom);
                    OffsetRect(
                        &_rectZoom,
                        _rectZoom.left < 0 ? 0 - _rectZoom.left : 0,
                        _rectZoom.top < 0 ? 0 - _rectZoom.top : 0);
                    OffsetRect(
                        &_rectZoom,
                        _rectZoom.right > (int) _uImageWidth ? _uImageWidth - _rectZoom.right : 0,
                        _rectZoom.bottom > (int) _uImageHeight ? _uImageHeight - _rectZoom.bottom : 0);
                }
                DrawRect(hdc, &_rectZoom);
                ReleaseDC(_hwnd, hdc);
                return 0;
            }
            // change zoom rectangle
            break;
        case WM_KEYUP:
            if (wParam == VK_ESCAPE)
            {
                // abort the zoom window
                HDC hdc = GetDC(_hwnd);
                EraseRect(hdc, &_rectZoom);
                ReleaseDC(_hwnd, hdc);
                ReleaseCapture();
                ShowCursor(TRUE);
                _fZooming = FALSE;
            }
            else
                return(DefWindowProc(_hwnd, uMsg, wParam, lParam));
            return 0;

        // All other messages are swallowed up and sent
        // to the DefWindowProc during a zoom.
        default:
            return(DefWindowProc(_hwnd, uMsg, wParam, lParam));
        }
    }
    switch (uMsg)
    {
    case WM_CREATE:
        OnCreate();
        break;
    case WM_SHOWWINDOW:
        if (!_pEngineList)
        {
            // this is the first time the window is being shown

            HMENU hmenu = GetMenu(_hwnd);
            if (!_fCanAnimate)
            {
                EnableMenuItem(
                    hmenu,
                    IDM_CYCLEIN,
                    MF_BYCOMMAND | MF_GRAYED);
                EnableMenuItem(
                    hmenu,
                    IDM_CYCLEOUT,
                    MF_BYCOMMAND | MF_GRAYED);
                EnableMenuItem(
                    hmenu,
                    IDM_CYCLESTOP,
                    MF_BYCOMMAND | MF_GRAYED);
            }

            _pEngineList = new CEngineList(hmenu);
            if (_pEngineList)
            {
                if SUCCEEDED(_pEngineList->Read())
                {
                    _uFractalType = 0;
                    ChangeFractalEngines();
                    if (_pFractalEngine)
                    {
                        SetDefaultColors();
                        if (SUCCEEDED(_pFractalEngine->Start()))
                        {
                            OnStart();
                        }
                        else
                        {
                            OnStop();
                        }
                    }
                }
                else
                {
                    // couldn't prime the engine list
                    TCHAR szTitle[MAX_STRING_LENGTH];
                    TCHAR szText[MAX_STRING_LENGTH];
                    if (LoadString(_hInstance, IDS_ERROR, szTitle, MAX_STRING_LENGTH))
                    {
                        if (LoadString(_hInstance, IDS_REGISTRYERROR, szText, MAX_STRING_LENGTH))
                        {
                            MessageBox(
                                _hwnd,
                                szText,
                                szTitle,
                                MB_OK | MB_ICONEXCLAMATION);
                        }
                    }
                    SendMessage(_hwnd, WM_CLOSE, 0, 0);
                }
            }
        }
        PostMessage(_hwnd, WM_REFRESHCOLORS, 0, 0);
        break;
    case WM_REFRESHCOLORS:
        {
            PALETTEENTRY rgColors[PALETTESIZE];
            memcpy(rgColors, _pEntry1, sizeof(PALETTEENTRY) * PALETTESIZE);
            Wash(PALETTESIZE, rgColors);
        }
        break;
    case WM_SIZE:
        NewWindowSize(LOWORD(lParam), HIWORD(lParam));
        break;
    case WM_HSCROLL:
        switch (LOWORD(wParam))
        {
        case SB_PAGEUP:
            _uScrollPosX = (_uScrollPosX > _uWindowWidth ?
                _uScrollPosX - _uWindowWidth :
                0);
            break;
        case SB_PAGEDOWN:
            _uScrollPosX = (_uScrollPosX + _uWindowWidth * 2 < _uImageWidth ?
                _uScrollPosX + _uWindowWidth :
                _uImageWidth - _uWindowWidth);
            break;
        case SB_LINEUP:
            _uScrollPosX = (_uScrollPosX > SCROLL_STEP ?
                _uScrollPosX - SCROLL_STEP :
                0);
            break;
        case SB_LINEDOWN:
            _uScrollPosX = (_uScrollPosX + _uWindowWidth + SCROLL_STEP < _uImageWidth ?
                _uScrollPosX + SCROLL_STEP :
                _uImageWidth - _uWindowWidth);
            break;
        case SB_THUMBTRACK:
            _uScrollPosX = HIWORD(wParam);
            break;
        }
        SetScrollPos(_hwnd, SB_HORZ, _uScrollPosX, TRUE);
        InvalidateRect(_hwnd, NULL, FALSE);
        break;
    case WM_VSCROLL:
        switch (LOWORD(wParam))
        {
        case SB_PAGEUP:
            _uScrollPosY = (_uScrollPosY > _uWindowHeight ?
                _uScrollPosY - _uWindowHeight :
                0);
            break;
        case SB_PAGEDOWN:
            _uScrollPosY = (_uScrollPosY + _uWindowHeight * 2 < _uImageHeight ?
                _uScrollPosY + _uWindowHeight :
                _uImageHeight - _uWindowHeight);
            break;
        case SB_LINEUP:
            _uScrollPosY = (_uScrollPosY > SCROLL_STEP ?
                _uScrollPosY - SCROLL_STEP :
                0);
            break;
        case SB_LINEDOWN:
            _uScrollPosY = (_uScrollPosY + _uWindowHeight + SCROLL_STEP < _uImageHeight ?
                _uScrollPosY + SCROLL_STEP :
                _uImageHeight - _uWindowHeight);
            break;
        case SB_THUMBTRACK:
            _uScrollPosY = HIWORD(wParam);
            break;
        }
        SetScrollPos(_hwnd, SB_VERT, _uScrollPosY, TRUE);
        InvalidateRect(_hwnd, NULL, FALSE);
        break;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            RECT rc;
              EnterCriticalSection(&_csGraph);
            HDC hdc = BeginPaint(_hwnd, &ps);
            HPALETTE hOldPal =  SelectPalette(hdc, _hpal, FALSE);
            RealizePalette(hdc);
            if (GetClipBox(ps.hdc, &rc) != NULLREGION)
            {
                BitBlt(ps.hdc,
                    rc.left,
                    rc.top,
                    rc.right - rc.left,
                    rc.bottom - rc.top,
                    _hdcMem,
                    rc.left + _uScrollPosX,
                    rc.top + _uScrollPosY,
                    SRCCOPY);
            }
            SelectPalette(hdc, hOldPal, TRUE);
            RealizePalette(hdc);
            EndPaint(_hwnd, &ps);
            LeaveCriticalSection(&_csGraph);
            break;
        }
    case WM_PALETTECHANGED:
        if ((HWND)wParam == _hwnd)
        {
            break;
        }
    case WM_QUERYNEWPALETTE:
        {
            if (_fReRealizePalette && GetFocus() == _hwnd)
            {
                // The first time we realized this
                // palette, it wasn't in the forground.  This
                // means that we must realize it again to ensure
                // that we get the entire palette to ourselves.

                UnrealizeObject(_hpal);
                _fReRealizePalette = FALSE;
            }
            HDC hdc = GetDC(_hwnd);
            HPALETTE hOldPal = SelectPalette(hdc, _hpal, FALSE);
            int iTemp = RealizePalette(hdc);

            // this next step is really unnecessary since
            SelectPalette(hdc, hOldPal, TRUE);
            RealizePalette(hdc);
            ReleaseDC(_hwnd, hdc);
            if (iTemp)
            {
                InvalidateRect(_hwnd, NULL, TRUE);
            }
            return(iTemp);
        }

    // When the title is not shown and mouse is clicked in client area,
    // we fool the system into thinking that the mouse has been clicked
    // on the caption (see WM_NCHITTEST message handling).  But this means
    // that the system thinks that a left button double click in the client
    // area when the title is not displayed is a non-client double click.
    // Therefore when the title is not displayed, we get WM_NCLBUTTONDBLCLK
    // messages instead of WM_LBUTTONDBLCLK messages, but need to handle
    // them the same way.

    case WM_NCLBUTTONDBLCLK:
        if (WaitForSingleObject(_hTitleEvent,0) == WAIT_OBJECT_0)
        {
            break;
        }
        // else FALL THROUGH to WM_LBUTTONDBLCLK case
        //

    case WM_LBUTTONDBLCLK:
        ToggleTitle();
        return(0);

    case WM_NCRBUTTONDOWN:
        if (WaitForSingleObject(_hTitleEvent,0) == WAIT_OBJECT_0)
        {
            break;
        }
    case WM_RBUTTONDOWN:
        _fRButtonDown = TRUE;
        break;

    case WM_NCRBUTTONUP:
        if (WaitForSingleObject(_hTitleEvent,0) == WAIT_OBJECT_0)
        {
            break;
        }
    case WM_RBUTTONUP:
        if (_fRButtonDown && WaitForSingleObject(_hGraphStartedEvent,0) == WAIT_TIMEOUT)
        {
            SendMessage(_hwnd, WM_COMMAND, IDM_EDIT_PROPERTIES, 0);
            _fRButtonDown = FALSE;
        }
        break;

    case WM_NCHITTEST:  // stolen from clock program
        wParam = DefWindowProc(_hwnd, uMsg, wParam, lParam);
        if (WaitForSingleObject(_hTitleEvent,0) != WAIT_OBJECT_0 && (wParam == HTCLIENT) && !IsZoomed(_hwnd))
        {
            return HTCAPTION;
        }
        return wParam;
        case WM_TIMER:
        if (wParam == CYCLE_TIMER)
        {
            // Cycle the colors
            RGBQUAD rgb[PALETTESIZE];
            for (int c = PALETTESIZE - 1; c--;)
            {
                int iTemp = c + _iCycleVelocity;
                while (iTemp < 0)
                {
                    iTemp += PALETTESIZE - 1;
                }
                while (iTemp >= PALETTESIZE - 1)
                {
                    iTemp -= PALETTESIZE - 1;
                }
                _pEntry2[c + 1] = _pEntry1[iTemp + 1];
                rgb[c].rgbRed = _pEntry2[c + 1].peRed;
                rgb[c].rgbBlue = _pEntry2[c + 1].peBlue;
                rgb[c].rgbGreen = _pEntry2[c + 1].peGreen;
            }
            _pEntry2[0] = _pEntry1[0];
            PALETTEENTRY * pEntryTemp = _pEntry1;
            _pEntry1 = _pEntry2;
            _pEntry2 = pEntryTemp;
            EnterCriticalSection(&_csGraph);
            SetDIBColorTable(_hdcMem, 11, PALETTESIZE - 1, rgb);
            AnimatePalette(_hpal, 11, PALETTESIZE - 1, &_pEntry1[1]);
            LeaveCriticalSection(&_csGraph);
        }
        if (wParam == REFRESH_TIMER)
        {
            if (WaitForSingleObject(_hGraphStartedEvent, 0) == WAIT_OBJECT_0)
            {
                InvalidateRect(_hwnd, &_rectPainted, FALSE);
                SetRectEmpty(&_rectPainted);
            }
        }
        return(0);
    case WM_COMMAND:
        return DoMenu(wParam, lParam);
    case WM_QUIT:
    case WM_CLOSE:
    default:
        return(DefWindowProc(_hwnd, uMsg, wParam, lParam));
    }
    return(FALSE);
}

//+---------------------------------------------------------------------------
//
//  Member:     CFractalWindow::DoMenu
//
//  Synopsis:   processes menu commands
//
//  Arguments:  [wParam] - menu command
//              [lParam] - command parameter
//
//  History:    4-12-94   stevebl   Created
//
//----------------------------------------------------------------------------

LRESULT CFractalWindow::DoMenu(WPARAM wParam, LPARAM lParam)
{
    switch (LOWORD(wParam))
    {
    case IDM_ABOUT:
        {
            CAbout dlgAbout;
            dlgAbout.ShowDialog(_hInstance, MAKEINTRESOURCE(IDM_ABOUT), _hwnd);
        }
        break;
    case IDM_CHANGEDIM:
        if (_pFractalEngine)
        {
            unsigned uSavedImageWidth = _uImageWidth;
            unsigned uSavedImageHeight = _uImageHeight;
            CChangeDim dlgChangeDim(this);
            if (dlgChangeDim.ShowDialog(_hInstance, MAKEINTRESOURCE(IDM_CHANGEDIM), _hwnd))
            {
                if (SUCCEEDED(_pFractalEngine->SetGraphSize(_uImageWidth, _uImageHeight)))
                {
                    _pFractalEngine->SetDefaults();
                }
                else
                {
                    _uImageWidth = uSavedImageWidth;
                    _uImageHeight = uSavedImageHeight;
                }
            }
            else
            {
                if (FAILED(_pFractalEngine->SetGraphSize(_uImageWidth, _uImageHeight)))
                {
                    _uImageWidth = uSavedImageWidth;
                    _uImageHeight = uSavedImageHeight;
                }
            }
        }
        break;
    case IDM_START:
        if (_pFractalEngine)
        {
            if (SUCCEEDED(_pFractalEngine->Start()))
            {
                _pFractalEngine->GetExtent(&_dLeft, &_dTop, &_dRight, &_dBottom);
                OnStart();
            }
        }
        break;
    case IDM_STOP:
        if (_pFractalEngine)
        {
            if (SUCCEEDED(_pFractalEngine->Stop()))
            {
                OnStop();
            }
        }
        break;
    case IDM_EDIT_PROPERTIES:
        if (_pFractalEngine)
        {
            _pFractalEngine->SetProperties(_hwnd);
        }
        break;
    case IDM_BRING_FORWARD:
        ToggleTitle();
        break;
    case IDM_EXIT:
        SendMessage(_hwnd, WM_CLOSE, 0, 0);
        break;
    case IDM_ZOOM:
        if (WaitForSingleObject(_hGraphStartedEvent,0) == WAIT_TIMEOUT)
        {
            SetCapture(_hwnd);
            ShowCursor(FALSE);
            SetRect(
                &_rectZoom,
                2 * _uImageWidth / 5,
                2 * _uImageHeight / 5,
                3 * _uImageWidth / 5,
                3 * _uImageHeight / 5);
            HDC hdc = GetDC(_hwnd);
            DrawRect(hdc, &_rectZoom);
            ReleaseDC(_hwnd, hdc);
            _fZooming = TRUE;
        }
        break;
    case IDM_HELP:
        if (!Exists(_szHelpFile))
        {
            TCHAR szTitle[MAX_STRING_LENGTH];
            TCHAR szText[MAX_STRING_LENGTH];
            if (LoadString(_hInstance, IDS_ERROR, szTitle, MAX_STRING_LENGTH))
            {
                if (LoadString(_hInstance, IDS_NOHELPFILE, szText, MAX_STRING_LENGTH))
                {
                    MessageBox(
                        _hwnd,
                        szText,
                        szTitle,
                        MB_OK | MB_ICONEXCLAMATION);
                }
            }
        }
        else
        {
            WinHelp(_hwnd, _szHelpFile, HELP_INDEX, 0);
        }
        break;
    case IDM_DEFAULTCOLORS:
        SetDefaultColors();
        break;
    case IDM_LOADPAL:
        if (!GetOpenFileName((LPOPENFILENAME)&_ofnPalette))
        {
            return(FALSE);
        }
        LoadPalette();
        break;
    case IDM_SAVEPAL:
        if (!GetSaveFileName((LPOPENFILENAME)&_ofnPalette))
        {
            return(FALSE);
        }
        SavePalette();
        break;
    case IDM_SAVEBMP:
        if (!GetSaveFileName((LPOPENFILENAME)&_ofnBitmap))
        {
            return(FALSE);
        }
        SaveBitmap();
        break;
    case IDM_LOAD:
        if (!GetOpenFileName((LPOPENFILENAME)&_ofnFile))
        {
            return(FALSE);
        }
        LoadFile();
        break;
    case IDM_SAVE:
        if (!GetSaveFileName((LPOPENFILENAME)&_ofnFile))
        {
            return(FALSE);
        }
        SaveFile();
        break;
    case IDM_CYCLEIN:
        if (_fCanAnimate)
        {
            Faster(-1);
        }
        break;
    case IDM_CYCLEOUT:
        if (_fCanAnimate)
        {
            Faster(1);
        }
        break;
    case IDM_CYCLESTOP:
        if (_fCanAnimate)
        {
            CycleStop();
        }
        break;
    default:
        if (WaitForSingleObject(_hGraphStartedEvent,0) == WAIT_TIMEOUT &&
            LOWORD(wParam) >= IDM_FRACTAL_TYPE &&
            LOWORD(wParam) < IDM_FRACTAL_TYPE + MAX_FRACTAL_TYPES &&
            LOWORD(wParam) - IDM_FRACTAL_TYPE != (int)_uFractalType)
        {
            // changing fractal types
            HMENU hmenu = GetMenu(_hwnd);
            CheckMenuItem(
                hmenu,
                _uFractalType + IDM_FRACTAL_TYPE,
                MF_BYCOMMAND | MF_UNCHECKED);
            CheckMenuItem(
                hmenu,
                LOWORD(wParam),
                MF_BYCOMMAND | MF_CHECKED);
            _uFractalType = LOWORD(wParam) - IDM_FRACTAL_TYPE;
            ChangeFractalEngines();
        }
        else
        {
            return(DefWindowProc(_hwnd, WM_COMMAND, wParam, lParam));
        }
    }
    return(FALSE);
}

//+---------------------------------------------------------------------------
//
//  Member:     CFractalWindow::CFractalWindow
//
//  Synopsis:   constructor
//
//  History:    4-11-94   stevebl   Created
//
//----------------------------------------------------------------------------

CFractalWindow::CFractalWindow()
{
    _pstg = NULL;
    _rgPixels = NULL;
    _hInstance = NULL;
    _hwnd = NULL;
    _hdcMem = NULL;
    _uScreenWidth = 0;
    _uScreenHeight = 0;
    _uImageWidth = 0;
    _uImageHeight = 0;
    _uWindowWidth = 0;
    _uWindowHeight = 0;
    _uScrollPosX = 0;
    _uScrollPosY = 0;
    _iCycleVelocity = 0;
    _iCurrentDirection = 0;
    _uRefreshRate = 1000;
    _hTitleEvent = NULL;
    _hGraphStartedEvent = NULL;
    _hbmSavedBitmap = NULL;
    _hbmMainWindow = NULL;
    _hpal = NULL;
    _pLogPal = NULL;
    _pSecondLogPal = NULL;
    _pEntry1 = NULL;
    _pEntry2 = NULL;
    _uRefCount = 0;
    _pEngineList = 0;
    _uFractalType = 0;
    _pFractalEngine = NULL;
    _fZooming = FALSE;
    _fRButtonDown = FALSE;
    InitializeCriticalSection(&_csGraph);
}

//+---------------------------------------------------------------------------
//
//  Member:     CFractalWindow::~CFractalWindow
//
//  Synopsis:   destructor
//
//  History:    4-11-94   stevebl   Created
//
//  Notes:      Frees up all resources allocated by the window.
//
//----------------------------------------------------------------------------

CFractalWindow::~CFractalWindow()
{
    if (_pFractalEngine)
    {
        _pFractalEngine->Stop();
    }
    WinHelp(_hwnd, _szHelpFile, HELP_QUIT, 0);
    if (_pFractalEngine)
    {
        _pFractalEngine->Release();
    }
    if (_hTitleEvent)
    {
        CloseHandle(_hTitleEvent);
    }
    if (_hGraphStartedEvent)
    {
        CloseHandle(_hGraphStartedEvent);
    }
    if (_hbmSavedBitmap)
    {
        HDC hdc = GetDC(_hwnd);
        HBITMAP hbm = (HBITMAP) SelectObject(hdc,_hbmSavedBitmap);
        ReleaseDC(_hwnd, hdc);
        DeleteObject(hbm);
    }
    if (_pLogPal)
    {
        LocalFree(_pLogPal);
    }
    if (_pSecondLogPal)
    {
        LocalFree(_pSecondLogPal);
    }
    if (_pEntry1)
    {
        LocalFree(_pEntry1);
    }
    if (_pEntry2)
    {
        LocalFree(_pEntry2);
    }
    if (_pEngineList)
    {
        delete _pEngineList;
    }
    DeleteCriticalSection(&_csGraph);
    PostQuitMessage(0);
}

//+---------------------------------------------------------------------------
//
//  Member:     CFractalWindow::SetUpColorTable
//
//  Synopsis:   initializes the color palette
//
//  Modifies:   _pEntry1, _pEntry2, _pLogPal
//
//  History:    4-12-94   stevebl   Created
//
//----------------------------------------------------------------------------

void CFractalWindow::SetUpColorTable(void)
{
    _pLogPal = (LOGPALETTE *) LocalAlloc(LMEM_FIXED,
        (sizeof(LOGPALETTE)+
        (sizeof(PALETTEENTRY)*(255))));

    _pEntry1 = (PALETTEENTRY *) LocalAlloc(LMEM_FIXED, sizeof(PALETTEENTRY)*(PALETTESIZE));
    _pEntry2 = (PALETTEENTRY *) LocalAlloc(LMEM_FIXED, sizeof(PALETTEENTRY)*(PALETTESIZE));
}

//+---------------------------------------------------------------------------
//
//  Member:     CFractalWindow::SetDefaultColors
//
//  Synopsis:   sets the default colors in the color palette wich are:
//                  black - background
//                  Then red
//                  washing to green
//                  washing to blue
//                  and washing back to red again.
//
//  History:    4-12-94   stevebl   Created
//
//----------------------------------------------------------------------------

void CFractalWindow::SetDefaultColors(void)
{
    PALETTEENTRY rgColors[4];

    rgColors[0].peRed = 0;
    rgColors[0].peGreen = 0;
    rgColors[0].peBlue = 0;

    rgColors[1].peRed = 255;
    rgColors[1].peGreen = 0;
    rgColors[1].peBlue = 0;

    rgColors[2].peRed = 0;
    rgColors[2].peGreen = 255;
    rgColors[2].peBlue = 0;

    rgColors[3].peRed = 0;
    rgColors[3].peGreen = 0;
    rgColors[3].peBlue = 255;

    Wash(4,rgColors);
}

//+---------------------------------------------------------------------------
//
//  Member:     CFractalWindow::Faster
//
//  Synopsis:   increses or decreses the palette's cycle velocity
//
//  Arguments:  [iDirection] - direction to cycle the palette
//                             -1 is in, 1 is out
//
//  History:    4-20-94   stevebl   Created
//
//----------------------------------------------------------------------------

void CFractalWindow::Faster(int iDirection)
{
    if (_iCycleVelocity == 0)
    {
        SetTimer(_hwnd, CYCLE_TIMER, CYCLE_RATE, NULL);
        _iCurrentDirection = _iCycleVelocity = iDirection;
    }
    else if (_iCycleVelocity == -iDirection)
    {
        CycleStop();
    }
    else if (iDirection == _iCurrentDirection)
    {
        _iCycleVelocity *= 2;
    }
    else
    {
        _iCycleVelocity /= 2;
    }
}

//+---------------------------------------------------------------------------
//
//  Member:     CFractalWindow::CycleStop
//
//  Synopsis:   stops color cycling
//
//  History:    4-20-94   stevebl   Created
//
//----------------------------------------------------------------------------

void CFractalWindow::CycleStop(void)
{
    if (_iCycleVelocity)
    {
        KillTimer(_hwnd, CYCLE_TIMER);
        _iCycleVelocity = 0;
    }
}

//+---------------------------------------------------------------------------
//
//  Member:     CFractalWindow::Wash
//
//  Synopsis:   "washes" a set of colors over the last n-1 colors in the
//              color palette, smoothly cycling the colors over the entire
//              range
//
//  Arguments:  [i]        - number of colors to wash
//              [rgColors] - array of colors
//
//  Modifies:   _pEntry1, _pEntry2, _hpal, _pLogPal
//
//  History:    4-12-94   stevebl   Created
//
//  Notes:      Color 0 is the graph background (or points inside the graph
//              set where appropriate).  It is always the first palette entry.
//              The other colors are cycled or washed over the remaining
//              palette entries so that they smoothly transition from one
//              to the next.
//
//----------------------------------------------------------------------------

void CFractalWindow::Wash(int i, PALETTEENTRY rgColors[])
{
    HPALETTE hPalOld;

    int c;
    int iFirst, iNext, iSource, iDestination;
    int iSpread, iIndex;

    EnterCriticalSection(&_csGraph);

    // Remember if we were in the foreground when we realized
    // our main window's palette.  If we're not in the foreground then we
    // will have to do it again when we get the WM_QUERYNEWPALETTE message.

    _fReRealizePalette = GetFocus() != _hwnd;

    // Now create an all black palette and realize it to rudely bump
    // everyone elese's colors out of the system palette.

    _pLogPal->palVersion    = 0x300;
    _pLogPal->palNumEntries = 256;

    for (c = 256; c--; )
    {
        _pLogPal->palPalEntry[c].peRed = 0;
        _pLogPal->palPalEntry[c].peGreen = 0;
        _pLogPal->palPalEntry[c].peBlue = 0;
        _pLogPal->palPalEntry[c].peFlags = PC_NOCOLLAPSE;
    }
    HPALETTE hPalTemp = CreatePalette((LPLOGPALETTE)_pLogPal);
    HDC hdc = GetDC(_hwnd);
    hPalOld = SelectPalette(hdc, hPalTemp, 0);
    RealizePalette(hdc);
    DeleteObject(hPalOld);

    // Now we're ready to create the palette that we're really interested in.

    _pLogPal->palVersion    = 0x300;
    _pLogPal->palNumEntries = 256;

    // First we'll get the system palette and initialize our palette to it.

    GetSystemPaletteEntries(hdc, 0, 256, &_pLogPal->palPalEntry[0]);

    // Here we'll make sure that everything in the palette (other than the
    // system colors) gets marked at least PC_NOCOLLAPSE.  This way
    // we can rudely be sure that no other app will be able to slow our
    // app down by destroying our one to one identity mapping with the
    // system palette.

    for (c = 245; c > PALETTESIZE + 10; c--)
    {
        _pLogPal->palPalEntry[c].peFlags = PC_NOCOLLAPSE;
    }

    // Now we'll fill in the rest of the colors that we're interested in,
    // marking them PC_RESERVED so that they can be animated.

    _pEntry1[0].peRed = _pLogPal->palPalEntry[10].peRed = rgColors[0].peRed;
    _pEntry1[0].peGreen = _pLogPal->palPalEntry[10].peGreen = rgColors[0].peGreen;
    _pEntry1[0].peBlue = _pLogPal->palPalEntry[10].peBlue = rgColors[0].peBlue;
    _pEntry1[0].peFlags = _pLogPal->palPalEntry[10].peFlags =
        _fCanAnimate ? PC_RESERVED : PC_NOCOLLAPSE;
    for (c = 0; c < i - 1 ; c++)
    {
        iSource = c + 1;
        iDestination = (c+1)%(i-1)+1;
        iFirst = 1 + MulDiv(c, PALETTESIZE - 1, i - 1);
        iNext = 1 + MulDiv(c + 1, PALETTESIZE - 1, i - 1);
        iSpread = iNext-iFirst;
        for (iIndex = iSpread; iIndex--; )
        {
            _pEntry1[iIndex + iFirst].peRed =
                _pLogPal->palPalEntry[iIndex + iFirst + 10].peRed =
                    (rgColors[iSource].peRed * (iSpread-iIndex)) / iSpread +
                    (rgColors[iDestination].peRed * iIndex) / iSpread;
            _pEntry1[iIndex + iFirst].peGreen =
                _pLogPal->palPalEntry[iIndex + iFirst + 10].peGreen =
                    (rgColors[iSource].peGreen * (iSpread-iIndex)) / iSpread +
                    (rgColors[iDestination].peGreen * iIndex) / iSpread;
            _pEntry1[iIndex + iFirst].peBlue =
                _pLogPal->palPalEntry[iIndex + iFirst + 10].peBlue =
                    (rgColors[iSource].peBlue * (iSpread-iIndex)) / iSpread +
                    (rgColors[iDestination].peBlue * iIndex) / iSpread;
            _pEntry1[iIndex + iFirst].peFlags =
                _pLogPal->palPalEntry[iIndex + iFirst + 10].peFlags =
                _fCanAnimate ? PC_RESERVED : PC_NOCOLLAPSE;
        }
    }

    _hpal = CreatePalette((LPLOGPALETTE) _pLogPal);
    hPalOld = SelectPalette(hdc, _hpal, 0);
    RealizePalette(hdc);
    ReleaseDC(_hwnd, hdc);
    DeleteObject(hPalOld);

    // now select the same palette into the memory DC

    hPalOld = SelectPalette (_hdcMem, _hpal, 0);
    RealizePalette(_hdcMem);
    DeleteObject(hPalOld);

    // prepare the dibsection's color table to exactly match the palette

    RGBQUAD rgb[256];
    for (int x = 256; x--;)
    {
        rgb[x].rgbRed = _pLogPal->palPalEntry[x].peRed;
        rgb[x].rgbGreen = _pLogPal->palPalEntry[x].peGreen;
        rgb[x].rgbBlue = _pLogPal->palPalEntry[x].peBlue;
        rgb[x].rgbReserved = 0;
    }

    SetDIBColorTable(_hdcMem, 0, 256, rgb);

    // force the bitmap to be repainted
    InvalidateRect(_hwnd, NULL, FALSE);
    LeaveCriticalSection(&_csGraph);
}

//+---------------------------------------------------------------------------
//
//  Member:     CFractalWindow::LoadPalette
//
//  Synopsis:   Loads the palette from a text file.
//
//  History:    4-21-94   stevebl   Created
//
//----------------------------------------------------------------------------

void CFractalWindow::LoadPalette(void)
{
    HANDLE h;
    PALETTEENTRY rgColors[PALETTESIZE];
    int c;
    char sz[256];
    DWORD dwCount;
    h = CreateFile(_szPalFileName,
        GENERIC_READ,
        FILE_SHARE_READ | FILE_SHARE_WRITE,
        NULL,
        OPEN_EXISTING,
        0,
        0);
    if (h != INVALID_HANDLE_VALUE)
    {
        for (c = 0; c < PALETTESIZE; c++)
        {
            int n = 0;
            do
            {
                ReadFile(h,&sz[n++], 1, &dwCount, NULL);
            } while (sz[n-1] != '\n' && dwCount && n < 255);
            if (n != 1)
            {
                unsigned u1, u2, u3;
                if(sscanf(sz, "%u %u %u",
                    &u1,
                    &u2,
                    &u3) != 3)
                {
                    break;
                }
                rgColors[c].peRed = u1;
                rgColors[c].peGreen = u2;
                rgColors[c].peBlue = u3;
            }
            else break;
        }
        CloseHandle(h);
        Wash(c,rgColors);
    }
}

//+---------------------------------------------------------------------------
//
//  Member:     CFractalWindow::SavePalette
//
//  Synopsis:   Saves the palette to a text file.
//
//  History:    4-21-94   stevebl   Created
//
//----------------------------------------------------------------------------

void CFractalWindow::SavePalette(void)
{
    HANDLE h;
    int c;
    char sz[256];
    DWORD dwCount;
    h = CreateFile(_szPalFileName,
        GENERIC_WRITE,
        0,
        NULL,
        CREATE_ALWAYS,
        FILE_ATTRIBUTE_NORMAL,
        0);
    if (h != INVALID_HANDLE_VALUE)
    {
        for (c = 0; c < PALETTESIZE; c++)
        {
            sprintf(sz, "%u %u %u\n",
                _pEntry1[c].peRed,
                _pEntry1[c].peGreen,
                _pEntry1[c].peBlue);
            WriteFile(h, sz, strlen(sz), &dwCount, NULL);
        }
        CloseHandle(h);
    }
}

//+---------------------------------------------------------------------------
//
//  Member:     CFractalWindow::SaveBitmap
//
//  Synopsis:   saves the graph as a standard device independent bitmap
//
//  History:    4-21-94   stevebl   Created
//
//----------------------------------------------------------------------------

void CFractalWindow::SaveBitmap(void)
{
    HANDLE hdibCurrent = DibFromBitmap(_hbmMainWindow, BI_RGB, 0, _hpal);
    StartWait();
#ifndef UNICODE
    // convert file name to a wide string
    WCHAR wszBmpFileName[MAX_PATH];
    mbstowcs(wszBmpFileName, _szBmpFileName, MAX_PATH);
    if (!WriteDIB(wszBmpFileName, hdibCurrent))
#else
    if (!WriteDIB(_szBmpFileName, hdibCurrent))
#endif
    {
        EndWait();
        TCHAR szTitle[MAX_STRING_LENGTH];
        TCHAR szText[MAX_STRING_LENGTH];
        if (LoadString(_hInstance, IDS_ERROR, szTitle, MAX_STRING_LENGTH))
        {
            if (LoadString(_hInstance, IDS_SAVEBMPFAILED, szText, MAX_STRING_LENGTH))
            {
                MessageBox(
                    _hwnd,
                    szText,
                    szTitle,
                    MB_OK | MB_ICONEXCLAMATION);
            }
        }
    }
    else
    {
        EndWait();
        TCHAR szText[MAX_STRING_LENGTH];
        if (LoadString(_hInstance, IDS_SAVEBMPSUCCEEDED, szText, MAX_STRING_LENGTH))
        {
            MessageBox(
                _hwnd,
                szText,
                TEXT(APP_TITLE),
                MB_OK | MB_ICONEXCLAMATION);
        }
    }
    GlobalFree(hdibCurrent);
}

//+---------------------------------------------------------------------------
//
//  Member:     CFractalWindow::LoadFile
//
//  Synopsis:   loads a fractal file
//
//  History:    4-22-94   stevebl   Created
//
//----------------------------------------------------------------------------

void CFractalWindow::LoadFile(void)
{
    StartWait();

#ifndef UNICODE
    // convert file name to a wide string
    WCHAR wszFileName[MAX_PATH];
    mbstowcs(wszFileName, _szFileName, MAX_PATH);
#endif
    // get IStorage
    HRESULT hr = StgOpenStorage(
#ifdef UNICODE
        _szFileName,
#else
        wszFileName,
#endif
        NULL,
        STGM_TRANSACTED | STGM_READWRITE | STGM_SHARE_EXCLUSIVE,
        NULL,
        0,
        &_pstg);
    if (SUCCEEDED(hr))
    {
        IStream * pstm;

        // load properies from "Properties" stream using OleLoadFromStream

        hr = _pstg->OpenStream(
            OLESTR("Properties"),
            0,
            STGM_READWRITE | STGM_SHARE_EXCLUSIVE,
            0,
            &pstm);
        if (SUCCEEDED(hr))
        {
            IFractalEngine * pFractalEngine;
            IPersistStream * pFractalEngineStream;
            hr = OleLoadFromStream(pstm, IID_IFractalEngine, (void **)&pFractalEngine);
            if (SUCCEEDED(hr))
            {
                if (_pFractalEngine)
                {
                    _pFractalEngine->Release();
                }
                _pFractalEngine = pFractalEngine;
                hr = pFractalEngine->QueryInterface( IID_IPersistStream, (void**)&pFractalEngineStream);
                if (SUCCEEDED(hr) )
                {
                    // Tell the new fractal engine where our IFractalHost pointer is.

                    _pFractalEngine->Init((IFractalHost *)this);
                    HMENU hmenu = GetMenu(_hwnd);

                    // Clear our check mark by our old fractal type.

                    CheckMenuItem(
                        hmenu,
                        _uFractalType + IDM_FRACTAL_TYPE,
                        MF_BYCOMMAND | MF_UNCHECKED);

                    // And check our new fractal type.

                    CLSID cid;
                    pFractalEngineStream->GetClassID(&cid);
                    pFractalEngineStream->Release();
                    int iIndex = _pEngineList->FindEntry(cid);
                    if (iIndex >= 0)
                    {
                        _uFractalType = (unsigned) iIndex;
                        CheckMenuItem(
                            hmenu,
                            _uFractalType + IDM_FRACTAL_TYPE,
                            MF_BYCOMMAND | MF_CHECKED);
                    }
                    else
                    {
                        // This is a previously unknown fractal type.
                        // HOWEVER it could be loaded and so there is
                        // nothing keeping us from adding it to our
                        // list of known fractal engines.

                        // At this time there is no mechanism for adding
                        // fractal engines to the Engine List object.
                        // This would be an easy thing to add at some time
                        // in the future.

                        // For now, we'll allow this condition (since there
                        // is no reason not to) but we'll do
                        // nothing about it.
                    }
                    _pFractalEngine->GetExtent(&_dLeft, &_dTop, &_dRight, &_dBottom);
                }
            }
            pstm->Release();

        }

        // Load image from "Image" stream

        if (SUCCEEDED(hr))
        {
            hr = _pstg->OpenStream(
                OLESTR("Image"),
                0,
                STGM_READWRITE | STGM_SHARE_EXCLUSIVE,
                0,
                &pstm);
            if (SUCCEEDED(hr))
            {
                unsigned uWidth, uHeight;
                hr = pstm->Read(&uWidth, sizeof(unsigned), NULL);
                if (SUCCEEDED(hr))
                {
                    hr = pstm->Read(&uHeight, sizeof(unsigned), NULL);
                }
                if (SUCCEEDED(hr))
                {
                    hr = SetGraphSize(uWidth, uHeight);
                    if (SUCCEEDED(hr))
                    {
                        _pFractalEngine->SetGraphSize(uWidth, uHeight);
                        hr = pstm->Read(_rgPixels, ((uWidth  + 3) / 4) * 4 * uHeight, NULL);
                    }
                }
                pstm->Release();
            }
        }

        // Load palette from "Palette" stream

        if (SUCCEEDED(hr))
        {
            hr = _pstg->OpenStream(
                OLESTR("Palette"),
                0,
                STGM_READWRITE | STGM_SHARE_EXCLUSIVE,
                0,
                &pstm);
            if (SUCCEEDED(hr))
            {
                PALETTEENTRY rgColors[PALETTESIZE];
                hr = pstm->Read(rgColors, sizeof(PALETTEENTRY) * (PALETTESIZE), NULL);

                // select new palette
                if (SUCCEEDED(hr))
                {
                    Wash(PALETTESIZE, rgColors);
                }
                pstm->Release();
            }
        }

        _pstg->Release();
        _pstg = NULL;
    }
    EndWait();
    if (FAILED(hr))
    {
        TCHAR szTitle[MAX_STRING_LENGTH];
        TCHAR szText[MAX_STRING_LENGTH];
        if (LoadString(_hInstance, IDS_ERROR, szTitle, MAX_STRING_LENGTH))
        {
            if (LoadString(_hInstance, IDS_LOADFRACTALFAILED, szText, MAX_STRING_LENGTH))
            {
                MessageBox(
                    _hwnd,
                    szText,
                    szTitle,
                    MB_OK | MB_ICONEXCLAMATION);
            }
        }
    }
}

//+---------------------------------------------------------------------------
//
//  Member:     CFractalWindow::SaveFile
//
//  Synopsis:   saves a fractal file
//
//  History:    4-22-94   stevebl   Created
//
//----------------------------------------------------------------------------

void CFractalWindow::SaveFile(void)
{
    if (!_pFractalEngine)
    {
        TCHAR szTitle[MAX_STRING_LENGTH];
        TCHAR szText[MAX_STRING_LENGTH];
        if (LoadString(_hInstance, IDS_ERROR, szTitle, MAX_STRING_LENGTH))
        {
            if (LoadString(_hInstance, IDS_SAVEFRACTALFAILED, szText, MAX_STRING_LENGTH))
            {
                MessageBox(
                    _hwnd,
                    szText,
                    szTitle,
                    MB_OK | MB_ICONEXCLAMATION);
            }
        }
        return;
    }
    StartWait();
#ifndef UNICODE
    // convert file name to a wide string
    WCHAR wszFileName[MAX_PATH];
    mbstowcs(wszFileName, _szFileName, MAX_PATH);
#endif
    HRESULT hr = StgCreateDocfile(
#ifdef UNICODE
        _szFileName,
#else
        wszFileName,
#endif
        STGM_CREATE | STGM_READWRITE | STGM_SHARE_EXCLUSIVE,
        0,
        &_pstg);
    if (SUCCEEDED(hr))
    {
        IStream * pstm;
        hr = _pstg->CreateStream(
            OLESTR("Palette"),
            STGM_CREATE | STGM_READWRITE | STGM_SHARE_EXCLUSIVE,
            0,
            0,
            &pstm);
        if (SUCCEEDED(hr))
        {
            hr = pstm->Write(_pEntry1, sizeof(PALETTEENTRY) * (PALETTESIZE), NULL);
            if (SUCCEEDED(hr))
            {
                hr = pstm->Commit(STGC_DEFAULT);
            }
            pstm->Release();
            if (SUCCEEDED(hr))
            {
                hr = _pstg->CreateStream(
                    OLESTR("Image"),
                    STGM_CREATE | STGM_READWRITE | STGM_SHARE_EXCLUSIVE,
                    0,
                    0,
                    &pstm);
                if (SUCCEEDED(hr))
                {
                    hr = pstm->Write(&_uImageWidth, sizeof(unsigned), NULL);
                    if (SUCCEEDED(hr))
                    {
                        hr = pstm->Write(&_uImageHeight, sizeof(unsigned), NULL);
                    }
                    if (SUCCEEDED(hr))
                    {
                        hr = pstm->Write(_rgPixels, ((_uImageWidth  + 3) / 4) * 4 * _uImageHeight, NULL);
                    }
                    pstm->Release();

                    hr = _pstg->CreateStream(
                        OLESTR("Properties"),
                        STGM_CREATE | STGM_READWRITE | STGM_SHARE_EXCLUSIVE,
                        0,
                        0,
                        &pstm);
                    if (SUCCEEDED(hr))
                    {
                        IPersistStream * pPStm;
                        hr = _pFractalEngine->QueryInterface(IID_IPersistStream, (void **)&pPStm);
                        if (SUCCEEDED(hr))
                        {
                            hr = OleSaveToStream(pPStm, pstm);
                            if (SUCCEEDED(hr))
                            {
                                hr = pstm->Commit(STGC_DEFAULT);
                            }
                            pstm->Release();
                            pPStm->Release();
                        }
                    }
                }
            }
        }
        if (SUCCEEDED(hr))
        {
            hr = _pstg->Commit(STGC_DEFAULT);
        }
        _pstg->Release();
        _pstg = NULL;
    }
    EndWait();
    if (SUCCEEDED(hr))
    {
        TCHAR szText[MAX_STRING_LENGTH];
        if (LoadString(_hInstance, IDS_SAVEFRACTALSUCCEEDED, szText, MAX_STRING_LENGTH))
        {
            MessageBox(
                _hwnd,
                szText,
                TEXT(APP_TITLE),
                MB_OK | MB_ICONEXCLAMATION);
        }
    }
    else
    {
        TCHAR szTitle[MAX_STRING_LENGTH];
        TCHAR szText[MAX_STRING_LENGTH];
        if (LoadString(_hInstance, IDS_ERROR, szTitle, MAX_STRING_LENGTH))
        {
            if (LoadString(_hInstance, IDS_SAVEFRACTALFAILED, szText, MAX_STRING_LENGTH))
            {
                MessageBox(
                    _hwnd,
                    szText,
                    szTitle,
                    MB_OK | MB_ICONEXCLAMATION);
            }
        }
    }
}

//+---------------------------------------------------------------------------
//
//  Member:     CFractalWindow::ToggleTitle
//
//  Synopsis:   toggles full screen mode on/off
//
//  History:    4-22-94   stevebl   Created
//
//----------------------------------------------------------------------------

void CFractalWindow::ToggleTitle(void)
{
    static ULONG ulID;
    static RECT OldRec;
    RECT rectTemp;
    static int iNoTitleTop = 0;
    static int iNoTitleLeft = 0;
    ULONG  ulStyleBits = GetWindowLong(_hwnd, GWL_STYLE);

    if (WaitForSingleObject(_hTitleEvent,0) != WAIT_OBJECT_0)
    {
        GetWindowRect(_hwnd,&rectTemp);
        iNoTitleTop = rectTemp.top;
        iNoTitleLeft = rectTemp.left;
        ulStyleBits |= WS_TILEDWINDOW;
        SetWindowLong(_hwnd, GWL_ID, ulID);
        MoveWindow(_hwnd,OldRec.left,OldRec.top,
            OldRec.right-OldRec.left,OldRec.bottom-OldRec.top,TRUE);
        SetWindowPos(_hwnd,HWND_NOTOPMOST,0,0,0,0,SWP_NOMOVE|SWP_NOSIZE|SWP_NOACTIVATE);
        SetEvent(_hTitleEvent);
        if (WaitForSingleObject(_hGraphStartedEvent, 0) != WAIT_OBJECT_0)
        {
            OnStop();
        }
        else
        {
            OnStart();
        }
    }
    else
    {
        int iWidth, iHeight;
        ResetEvent(_hTitleEvent);
        GetWindowRect(_hwnd,&OldRec);
        ulStyleBits &= ~(WS_DLGFRAME | WS_SYSMENU |
            WS_MINIMIZEBOX | WS_MAXIMIZEBOX |
            WS_THICKFRAME | WS_VSCROLL | WS_HSCROLL);
        ulID = SetWindowLong(_hwnd, GWL_ID, 0);
        iWidth = _uImageWidth + GetSystemMetrics(SM_CXBORDER) * 2;
        iHeight = _uImageHeight + GetSystemMetrics(SM_CYBORDER) * 2;
        if (iNoTitleTop == 0 && iNoTitleLeft == 0)
        {
            iNoTitleTop = 0-GetSystemMetrics(SM_CYBORDER);
            iNoTitleLeft = 0-GetSystemMetrics(SM_CXBORDER);
        }
        MoveWindow(_hwnd
            ,iNoTitleLeft
            ,iNoTitleTop
            ,iWidth
            ,iHeight
            ,TRUE);
    }

    SetWindowLong(_hwnd, GWL_STYLE, ulStyleBits);
    SetWindowPos( _hwnd, NULL, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE |
    SWP_NOZORDER | SWP_FRAMECHANGED );
    ShowWindow(_hwnd, SW_SHOW);
}


//+---------------------------------------------------------------------------
//
//  Member:     CFractalWindow::OnStart
//
//  Synopsis:   Called whenever the graphics engine has been
//              successfully started.
//
//  History:    4-22-94   stevebl   Created
//
//  Notes:      Disables the following menu items:
//                  IDM_LOAD
//                  IDM_SAVE
//                  IDM_SAVEBMP
//                  IDM_START
//                  IDM_SELECT_TYPE
//                  IDM_EDIT_PROPERTIES
//                  IDM_CHANGEDIM
//                  IDM_ZOOM
//
//              Enables the following menu items:
//                  IDM_STOP
//
//----------------------------------------------------------------------------

void CFractalWindow::OnStart(void)
{
    SetEvent(_hGraphStartedEvent);
    HMENU hmenu = GetMenu(_hwnd);
    UINT rgCommandList[] =
        {
            IDM_LOAD,
            IDM_SAVE,
            IDM_SAVEBMP,
            IDM_START,
            IDM_SELECT_TYPE,
            IDM_EDIT_PROPERTIES,
            IDM_CHANGEDIM,
            IDM_ZOOM
        };
    for (int iIndex = sizeof(rgCommandList)/sizeof(UINT); iIndex--;)
    {
        EnableMenuItem(
            hmenu,
            rgCommandList[iIndex],
            MF_BYCOMMAND | MF_GRAYED);
    }
    EnableMenuItem(
        hmenu,
        IDM_STOP,
        MF_BYCOMMAND | MF_ENABLED);
}

//+---------------------------------------------------------------------------
//
//  Member:     CFractalWindow::OnStop
//
//  Synopsis:   Called whenever the graphics engine has been stopped.
//
//  History:    4-22-94   stevebl   Created
//
//  Notes:      Disables the following menu items:
//                  IDM_STOP
//
//              Enables the following menu items:
//                  IDM_LOAD
//                  IDM_SAVE
//                  IDM_SAVEBMP
//                  IDM_START
//                  IDM_SELECT_TYPE
//                  IDM_EDIT_PROPERTIES
//                  IDM_CHANGEDIM
//                  IDM_ZOOM
//
//----------------------------------------------------------------------------

void CFractalWindow::OnStop(void)
{
    ResetEvent(_hGraphStartedEvent);
    HMENU hmenu = GetMenu(_hwnd);
    UINT rgCommandList[] =
        {
            IDM_LOAD,
            IDM_SAVE,
            IDM_SAVEBMP,
            IDM_START,
            IDM_SELECT_TYPE,
            IDM_EDIT_PROPERTIES,
            IDM_CHANGEDIM,
            IDM_ZOOM
        };
    for (int iIndex = sizeof(rgCommandList)/sizeof(UINT); iIndex--;)
    {
        EnableMenuItem(
            hmenu,
            rgCommandList[iIndex],
            MF_BYCOMMAND | MF_ENABLED);
    }
    EnableMenuItem(
        hmenu,
        IDM_STOP,
        MF_BYCOMMAND | MF_GRAYED);
    InvalidateRect(_hwnd, NULL, FALSE);
}

//+---------------------------------------------------------------------------
//
//  Member:     CFractalWindow::SetGraphSize
//
//  Synopsis:   called every time the graph's size changes
//
//  Arguments:  [uWidth]  - width of the graph in pixels
//              [uHeight] - height of the graph in pixels
//
//  Modifies:   _uImageWidth, _uImageHeight
//
//  History:    4-12-94   stevebl   Created
//
//  Notes:      calls NewWindowSize to update the scroll bars
//
//----------------------------------------------------------------------------

STDMETHODIMP CFractalWindow::SetGraphSize(unsigned uWidth, unsigned uHeight)
{
    HDC hdc = GetDC(_hwnd);
    BITMAPINFO bmi;

    bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    bmi.bmiHeader.biWidth = uWidth;
    bmi.bmiHeader.biHeight= uHeight;
    bmi.bmiHeader.biPlanes = 1;
    bmi.bmiHeader.biBitCount = 8;
    bmi.bmiHeader.biCompression = BI_RGB;
    bmi.bmiHeader.biSizeImage = 0;
    bmi.bmiHeader.biXPelsPerMeter = 0;
    bmi.bmiHeader.biYPelsPerMeter = 0;
    bmi.bmiHeader.biClrUsed = 0;
    bmi.bmiHeader.biClrImportant = 0;
    char * rgPixels;
    HBITMAP hbmMainWindow = CreateDIBSection(hdc, &bmi, DIB_PAL_COLORS, (void **)&rgPixels, NULL, NULL);
    ReleaseDC(_hwnd, hdc);
    if (hbmMainWindow == NULL)
    {
        return(E_OUTOFMEMORY);
    }
    _uImageWidth = uWidth;
    _uImageHeight = uHeight;
    _hbmMainWindow = hbmMainWindow;
    _rgPixels = rgPixels;

    if (!_hbmSavedBitmap)
    {
        // this is the very first time this has been called.
        EnterCriticalSection(&_csGraph);
        _hbmSavedBitmap = (HBITMAP) SelectObject(_hdcMem, _hbmMainWindow);
        LeaveCriticalSection(&_csGraph);
    }
    else
    {
        EnterCriticalSection(&_csGraph);
        HBITMAP hbm = (HBITMAP) SelectObject(_hdcMem, _hbmMainWindow);
        LeaveCriticalSection(&_csGraph);
        DeleteObject(hbm);
    }

    NewWindowSize(_uWindowWidth, _uWindowHeight);

    Rect(0, 0, (int)uWidth-1, (int)uHeight-1, 0);
    InvalidateRect(_hwnd, NULL, TRUE);

    PostMessage(_hwnd, WM_REFRESHCOLORS, 0, 0);

    return(S_OK);
}

//+---------------------------------------------------------------------------
//
//  Member:     CFractalWindow::GetGraphSize
//
//  Synopsis:   returns the size of the graph's bitmap
//
//  Arguments:  [puWidth]  - width in pixels
//              [puHeight] - height in pixels
//
//  History:    4-13-94   stevebl   Created
//
//----------------------------------------------------------------------------

STDMETHODIMP CFractalWindow::GetGraphSize(unsigned * puWidth, unsigned * puHeight)
{
    *puWidth = _uImageWidth;
    *puHeight = _uImageHeight;
    return(S_OK);
}

// IUnknown methods:

//+---------------------------------------------------------------------------
//
//  Member:     CFractalWindow::QueryInterface
//
//  Synopsis:   returns an interface supported by this object
//
//  Arguments:  [riid] - id of desired interface
//              [ppv]  - pointer to recieve interface
//
//  Returns:    S_OK if the interface is supported
//
//  History:    4-14-94   stevebl   Created
//
//----------------------------------------------------------------------------

STDMETHODIMP CFractalWindow::QueryInterface (REFIID riid, void ** ppv)
{
    HRESULT hr = S_OK;
    if (IsEqualGUID(riid, IID_IUnknown))
    {
        *ppv = (LPVOID)(IUnknown *) this;
    }
    else if (IsEqualGUID(riid, IID_IFractalHost))
    {
        *ppv = (LPVOID)(IFractalHost *) this;
    }
    else
    {
        *ppv = NULL;
        hr = E_NOINTERFACE;
    }
    return(hr);
}

//+---------------------------------------------------------------------------
//
//  Member:     CFractalWindow::AddRef
//
//  Synopsis:   increments refcount
//
//  History:    4-14-94   stevebl   Created
//
//  Notes:      Since this is an application, the refcount isn't acutally
//              used for anything.  The app will remain around until the
//              user closes it.
//
//----------------------------------------------------------------------------

STDMETHODIMP_(ULONG) CFractalWindow::AddRef(void)
{
    return(++_uRefCount);
}

//+---------------------------------------------------------------------------
//
//  Member:     CFractalWindow::Release
//
//  Synopsis:   decrements the refcount
//
//  Returns:    new refcount
//
//  History:    4-14-94   stevebl   Created
//
//  Notes:      Since this is an application the refcount isn't actually used
//              for anything.  If the refcount reaches zero, the app will
//              still remain open until the user closes it.
//
//----------------------------------------------------------------------------

STDMETHODIMP_(ULONG) CFractalWindow::Release(void)
{
    if (_uRefCount > 0)
    {
        _uRefCount--;
    }
    else
    {
        // This object is _NOT_ destroyed if it's reference count reaches zero.
    }
    return(_uRefCount);
}


// IFractalHost methods:

//+---------------------------------------------------------------------------
//
//  Member:     CFractalWindow::GetPoint
//
//  Synopsis:   gets the color at the given point
//
//  Arguments:  [puColor] - pointer to variable to recieve the color
//              [x]       - x position of the pixel
//              [y]       - y position of the pixel
//
//  Returns:    S_OK if the point is on the graph
//              S_FALSE if the point is off the graph (and *puColor is set to 0)
//
//  History:    4-12-94   stevebl   Created
//              7-07-94   stevebl   changed parameters and return value
//
//----------------------------------------------------------------------------

STDMETHODIMP CFractalWindow::GetPoint(unsigned * puColor, int x, int y)
{
    if ((unsigned) x >= _uImageWidth || (unsigned) y >= _uImageHeight)
    {
        *puColor = 0;
        return(S_FALSE);
    }
    EnterCriticalSection(&_csGraph);
    * puColor = (unsigned)_rgPixels[x + (_uImageHeight - (y + 1)) * (((_uImageWidth + 3) / 4) * 4)] - 10;
    LeaveCriticalSection(&_csGraph);
    return(S_OK);
}

//+---------------------------------------------------------------------------
//
//  Member:     CFractalWindow::Rect
//
//  Synopsis:   paints a filled rectangle
//
//  Arguments:  [iLeft]   - left side
//              [iTop]    - top
//              [iRight]  - right side
//              [iBottom] - bottom
//              [uColor]  - color index
//
//  Returns:    S_OK if any part of the rectangle is painted
//              S_FALSE if the rectangle is entirely off the graph
//
//  History:    4-12-94   stevebl   Created
//
//----------------------------------------------------------------------------

STDMETHODIMP CFractalWindow::Rect(
    int iLeft,
    int iTop,
    int iRight,
    int iBottom,
    unsigned uColor)
{
    if (iRight < iLeft)
    {
        int i = iRight;
        iRight = iLeft;
        iLeft = i;
    }
    if (iBottom < iTop)
    {
        int i = iBottom;
        iBottom = iTop;
        iTop = i;
    }
    if (iRight < 0 || iBottom < 0 || iLeft >= (int) _uImageWidth || iTop >= (int) _uImageHeight)
    {
        // Rectangle is entirely outside of the boundaries
        return(S_FALSE);
    }
    if (iLeft < 0)
    {
        iLeft = 0;
    }
    if (iTop < 0)
    {
        iTop = 0;
    }
    if (iRight >= (int) _uImageWidth)
    {
        iRight = _uImageWidth - 1;
    }
    if (iBottom >= (int) _uImageHeight)
    {
        iBottom = _uImageHeight - 1;
    }
    EnterCriticalSection(&_csGraph);
    char chColor = char (uColor + 10);
    if (0 == iLeft &&
        0 == iTop &&
        (int) _uImageWidth - 1 == iRight &&
        (int) _uImageHeight - 1 == iBottom)
    {
        memset(_rgPixels, chColor, ((_uImageWidth + 3) / 4) * 4 * _uImageHeight);

        // This little trick causes the screen to always repaint if the
        // calling process paints the entire window (which always happens
        // when a graph engine is restarted).

        InvalidateRect(_hwnd, NULL, TRUE);
    }
    else
    {
        RECT rect;
        rect.left = iLeft;
        rect.top = iTop;
        // add one to the right and bottom to
        // make sure that we never get a null region
        rect.right = iRight + 1;
        rect.bottom = iBottom + 1;
        UnionRect(&_rectPainted, &_rectPainted, &rect);
        for (unsigned y = (iBottom - iTop) + 1; y--;)
        {
            unsigned uBase = (_uImageHeight - (y + iTop + 1)) * ((_uImageWidth + 3) / 4) * 4 + iLeft;
            for (unsigned x = (iRight - iLeft) + 1; x--;)
            {
                _rgPixels[uBase + x] = chColor;
            }
        }
    }
    LeaveCriticalSection(&_csGraph);
    return(S_OK);
}

//+---------------------------------------------------------------------------
//
//  Member:     CFractalWindow::ClipLine
//
//  Synopsis:   clips a line to the graph boundaries
//
//  Arguments:  [x1] - x coordinate of point 1
//              [y1] - y coordinate of point 1
//              [x2] - x coordinate of point 2
//              [y2] - y coordinate of point 2
//
//  Returns:    TRUE if the line is completely outside the graph (rejected)
//              FALSE if any part of the line should be drawn
//
//  Modifies:   the endpoints are clipped to be within the graph
//
//  History:    5-03-94   stevebl   Created
//
//----------------------------------------------------------------------------

BOOL CFractalWindow::ClipLine(int &x1, int &y1, int &x2, int &y2)
{
    // Test for trivial rejections
    if (
        (x1 < 0 && x2 < 0)
        || (y1 < 0 && y2 < 0)
        || (x1 >= (int) _uImageWidth && x2 >= (int) _uImageWidth)
        || (y1 >= (int) _uImageHeight && y2 >= (int) _uImageHeight)
       )
    {
        return(TRUE);   // line has been rejected
    }

    // Now adjust any points that are outside the bounding box to be cut off
    // at and edge of the bounding box.

    int dx = x2 - x1;
    int dy = y2 - y1;

    if (x1 < 0)
    {
        y1 = ((0 - x1) * dy) / dx + y1;
        x1 = 0;
    }
    else if (x2 < 0)
    {
        y2 = ((0 - x2) * dy) / dx + y2;
        x2 = 0;
    }
    if ((int) y1 < 0)
    {
        x1 = ((0 - y1) * dx) / dy + x1;
        y1 = 0;
    }
    else if (y2 < 0)
    {
        x2 = ((0 - y2) * dx) / dy + x2;
        y2 = 0;
    }
    if (x1 >= (int) _uImageWidth)
    {
        y1 = (((int) _uImageWidth - x1) * dy) / dx + y1;
        x1 = _uImageWidth - 1;
    }
    else if (x2 >= (int) _uImageWidth)
    {
        y2 = (((int) _uImageWidth - x2) * dy) / dx + y2;
        x2 = _uImageWidth - 1;
    }
    if (y1 >= (int) _uImageHeight)
    {
        x1 = (((int) _uImageHeight - y1) * dx) / dy + x1;
        y1 = _uImageHeight - 1;
    }
    else if (y2 >= (int) _uImageHeight)
    {
        x2 = (((int) _uImageHeight - y2) * dx) / dy + x2;
        y2 = _uImageHeight - 1;
    }

    // Test again for rejection
    if (
        x1 < 0
        || x2 < 0
        || y1 < 0
        || y2 < 0
        || x1 >= (int) _uImageWidth
        || x2 >= (int) _uImageWidth
        || y1 >= (int) _uImageHeight
        || y2 >= (int) _uImageHeight
       )
    {
        return(TRUE);   // line has been rejected
    }

    return(FALSE);
}

//+---------------------------------------------------------------------------
//
//  Member:     CFractalWindow::Line
//
//  Synopsis:   draws a line on the graph
//
//  Arguments:  [x1]     - starting point
//              [y1]
//              [x2]     - ending point
//              [y2]
//              [uColor] - color
//
//  Returns:    S_OK if any part of the line could be drawn
//              S_FALSE if all of the line was outside the graph (rejected)
//
//  History:    4-12-94   stevebl   Created
//
//----------------------------------------------------------------------------

STDMETHODIMP CFractalWindow::Line(
    int x1,
    int y1,
    int x2,
    int y2,
    unsigned uColor)
{
    // clip the line
    if (ClipLine(x1, y1, x2, y2))
    {
        // line was rejected (it's not on the graph)
        return(S_FALSE);
    }

    EnterCriticalSection(&_csGraph);
    RECT rect;

    if (x1 < x2)
    {
        rect.left = x1;
        rect.right = x2;
    }
    else
    {
        rect.left = x2;
        rect.right = x1;
    }
    if (y1 < y2)
    {
        rect.top = y1;
        rect.bottom = y2;
    }
    else
    {
        rect.top = y2;
        rect.bottom = y1;
    }
    int dx = rect.right - rect.left;
    int dy = rect.bottom - rect.top;
    char chColor = (char) uColor + 10;
    int p, c1, c2, iNextLine;
    unsigned uCurrent, uLast;
    if (dx > dy)
    {
        // step along x axis
        p = 2 * dy - dx;
        c1 = 2 * dy;
        c2 = 2 * (dy - dx);
        if (x1 > x2)
        {
            // last point to first point
            uCurrent = x2 + (_uImageHeight - (y2 + 1)) * (((_uImageWidth + 3) / 4) * 4);
            uLast = x1 + (_uImageHeight - (y1 + 1)) * (((_uImageWidth + 3) / 4) * 4);
            if (y2 < y1)
            {
                // posative slope
                iNextLine = (0 - (((_uImageWidth + 3) / 4) * 4)) + 1;
            }
            else
            {
                // negative slope
                iNextLine = (((_uImageWidth + 3) / 4) * 4) + 1;
            }
        }
        else
        {
            // first point to last point
            uCurrent = x1 + (_uImageHeight - (y1 + 1)) * (((_uImageWidth + 3) / 4) * 4);
            uLast = x2 + (_uImageHeight - (y2 + 1)) * (((_uImageWidth + 3) / 4) * 4);
            if (y1 < y2)
            {
                // posative slope
                iNextLine = (0 - (((_uImageWidth + 3) / 4) * 4)) + 1;
            }
            else
            {
                // negative slope
                iNextLine = (((_uImageWidth + 3) / 4) * 4) + 1;
            }
        }
        _rgPixels[uCurrent] = chColor;
        while (uCurrent != uLast)
        {
            if (p < 0)
            {
                uCurrent++;
                p += c1;
            }
            else
            {
                uCurrent += iNextLine;
                p += c2;
            }
            _rgPixels[uCurrent] = chColor;
        }
    }
    else
    {
        // step along y axis
        p = 2 * dx - dy;
        c1 = 2 * dx;
        c2 = 2 * (dx - dy);
        if (y1 > y2)
        {
            // last point to first point
            uCurrent = x2 + (_uImageHeight - (y2 + 1)) * (((_uImageWidth + 3) / 4) * 4);
            uLast = x1 + (_uImageHeight - (y1 + 1)) * (((_uImageWidth + 3) / 4) * 4);
            if (x2 > x1)
            {
                // negative slope
                iNextLine = (0 - (((_uImageWidth + 3) / 4) * 4)) - 1;
            }
            else
            {
                // posative slope
                iNextLine = (0 - (((_uImageWidth + 3) / 4) * 4)) + 1;
            }
        }
        else
        {
            // first point to last point
            uCurrent = x1 + (_uImageHeight - (y1 + 1)) * (((_uImageWidth + 3) / 4) * 4);
            uLast = x2 + (_uImageHeight - (y2 + 1)) * (((_uImageWidth + 3) / 4) * 4);
            if (x1 > x2)
            {
                // negative slope
                iNextLine = (0 - (((_uImageWidth + 3) / 4) * 4)) - 1;
            }
            else
            {
                // posative slope
                iNextLine = (0 - (((_uImageWidth + 3) / 4) * 4)) + 1;
            }
        }
        _rgPixels[uCurrent] = chColor;
        while (uCurrent != uLast)
        {
            if (p < 0)
            {
                uCurrent += (0 - (((_uImageWidth + 3) / 4) * 4));
                p += c1;
            }
            else
            {
                uCurrent += iNextLine;
                p += c2;
            }
            _rgPixels[uCurrent] = chColor;
        }
    }
    // add one to the right and bottom to
    // make sure that we never get a null region
    rect.bottom++;
    rect.right++;
    UnionRect(&_rectPainted, &_rectPainted, &rect);
    LeaveCriticalSection(&_csGraph);
    return(S_OK);
}

//+---------------------------------------------------------------------------
//
//  Member:     CFractalWindow::Point
//
//  Synopsis:   plots a pixel on the graph
//
//  Arguments:  [x]      - x coordinate
//              [y]      - y coordinate
//              [uColor] - color
//
//  Returns:    S_OK if the point was plotted
//              S_FALSE if the point is outside the graph
//
//  History:    4-12-94   stevebl   Created
//
//----------------------------------------------------------------------------

STDMETHODIMP CFractalWindow::Point(
    int x,
    int y,
    unsigned uColor)
{
    // clip the point
    if ((unsigned) x >= _uImageWidth || (unsigned) y >= _uImageHeight)
    {
        return(S_FALSE);
    }
    RECT rectPt;
    EnterCriticalSection(&_csGraph);
    rectPt.left = x;
    rectPt.top = y;
    // Have to have a rectangle with at least one pixel or UnionRect won't
    // do anything.
    rectPt.right = x + 1;
    rectPt.bottom = y + 1;
    UnionRect(&_rectPainted, &_rectPainted, &rectPt);
    _rgPixels[x + (_uImageHeight - (y + 1)) * (((_uImageWidth + 3) / 4) * 4)] = (char)(uColor + 10);
    LeaveCriticalSection(&_csGraph);
    return(S_OK);
}

//+---------------------------------------------------------------------------
//
//  Member:     CFractalWindow::DoneDrawingGraph
//
//  Synopsis:   called by the fractal engine when its done drawing a graph
//
//  Returns:    S_OK
//
//  History:    4-12-94   stevebl   Created
//
//----------------------------------------------------------------------------

STDMETHODIMP CFractalWindow::DoneDrawingGraph(void)
{
    MessageBeep(0xffffffff);
    OnStop();
    return(S_OK);
}

