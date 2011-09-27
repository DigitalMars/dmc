//+---------------------------------------------------------------------------
//
//  Microsoft Windows
//  Copyright (C) Microsoft Corporation, 1992 - 1995.
//
//  File:       stmvwr.cxx
//
//  Contents:   implementation for the stream viewer window class
//
//  Classes:    CStreamView
//
//  Functions:
//
//  History:    6-27-94   stevebl   Created
//
//----------------------------------------------------------------------------

#include <windows.h>
#include <ole2.h>
#include "strmvwr.h"

#define BYTESPERLINE 16
#define DIVISIONPOINTS 8

//+---------------------------------------------------------------------------
//
//  Member:     CStreamView::CStreamView
//
//  Synopsis:   constructor
//
//  Arguments:  [hlb]    - handle to CMainWindow's listbox
//              [ple]    - pointer to the LISTBOXELEMENT structure for this
//                         stream
//
//  History:    6-27-94   stevebl   Created
//
//----------------------------------------------------------------------------

CStreamView::CStreamView(HWND hlb, LISTBOXELEMENT * ple)
{
    _hlbParent = hlb;
    _ple = ple;
    _hfListBox = NULL;
}

//+---------------------------------------------------------------------------
//
//  Member:     CStreamView::~CStreamView
//
//  Synopsis:   destructor
//
//  History:    6-27-94   stevebl   Created
//
//----------------------------------------------------------------------------

CStreamView::~CStreamView()
{
    _ple->fOpen = FALSE;
    if (_hfListBox)
    {
        DeleteObject(_hfListBox);
    }
    InvalidateRect(_hlbParent, NULL, FALSE);
}

//+---------------------------------------------------------------------------
//
//  Member:     CStreamView::InitInstance
//
//  Synopsis:   creates and shows the stream viewer window
//
//  Arguments:  [hInstance] - the application's instance handle
//              [nCmdShow]  - command to pass to ShowWindow
//
//  Returns:    TRUE on success
//              FALSE on failure
//
//  History:    6-27-94   stevebl   Created
//
//  Notes:      must only be called once immediately after class construction
//
//----------------------------------------------------------------------------

BOOL CStreamView::InitInstance(HINSTANCE hInstance, int nCmdShow)
{
    IStream * pstm = (IStream * ) _ple->pInterface;
    STATSTG statstg;
    HRESULT hr = pstm->Stat(&statstg, STATFLAG_NONAME);
    if (FAILED(hr))
    {
        MessageBoxFromStringIdsAndArgs(
            _hwnd,
            _hInstance,
            IDS_STMSTATFAILED,
            IDS_ERROR,
            MB_OK | MB_ICONEXCLAMATION,
            hr
            );
        return(FALSE);
    }
    _cbSize = statstg.cbSize.LowPart;

    TCHAR szStreamTitleText[MAX_STRING_LENGTH];
    if (!LoadString(_hInstance, IDS_STMTITLETEXT, szStreamTitleText, MAX_STRING_LENGTH))
    {
        lstrcpy(szStreamTitleText, TEXT(""));
    }

    TCHAR * szTitle = new TCHAR[lstrlen(TEXT("  [0x00000000 bytes]"))
        + lstrlen(szStreamTitleText) + wcslen(_ple->pwcsName) + 1];
    if (szTitle)
    {
        wsprintf(szTitle, TEXT("%s %ws [0x%08X bytes]"), szStreamTitleText, _ple->pwcsName, _cbSize);
    }
    // Note, the Create method sets the _hwnd member for me so I don't
    // need to set it myself.
    HWND hwnd = Create(
        TEXT(STREAM_VIEW_CLASS_NAME),
        szTitle,
        WS_OVERLAPPED
            | WS_MINIMIZEBOX
            | WS_MAXIMIZEBOX
            | WS_SYSMENU
            | WS_THICKFRAME,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        NULL,
        NULL,
        hInstance);
    delete[] szTitle;
    if (!hwnd)
    {
        return(FALSE);
    }
    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);
    return(TRUE);
}

//+---------------------------------------------------------------------------
//
//  Member:     CStreamView::WindowProc
//
//  Synopsis:   window procedure for the stream viewer window
//
//  Arguments:  [uMsg]   - Window's message
//              [wParam] - first message parameter
//              [lParam] - second message parameter
//
//  History:    6-27-94   stevebl   Created
//
//----------------------------------------------------------------------------

LRESULT CStreamView::WindowProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
    case WM_CREATE:
        {
            RECT rc;
            GetClientRect(_hwnd, &rc);
            // record the width before we create the list box
            _wWidth = (WORD) rc.right;

            HDC hdc = GetDC(_hwnd);
            LOGFONT lf;
            memset(&lf, 0, sizeof(LOGFONT));

            // Get an 8 point Courier font.
            lf.lfHeight = -MulDiv(8, GetDeviceCaps(hdc, LOGPIXELSY), 72);
            lstrcpy(lf.lfFaceName, TEXT("Courier"));
            _hfListBox = CreateFontIndirect(&lf);
            HFONT hfOld = (HFONT) SelectObject(hdc, _hfListBox);
            GetTextMetrics(hdc, &_tm);
            SelectObject(hdc, hfOld);
            ReleaseDC(_hwnd, hdc);

            // create the child listbox
            _hlb = CreateWindow(
                TEXT("listbox"),
                TEXT(""),
                LBS_OWNERDRAWFIXED |
                    WS_CHILD |
                    WS_VISIBLE |
                    WS_VSCROLL |
                    LBS_NOINTEGRALHEIGHT |
                    LBS_NODATA,
                0,
                0,
                rc.right,
                rc.bottom,
                _hwnd,
                (HMENU) IDC_LISTBOX,
                _hInstance,
                NULL);
            if (NULL == _hlb)
            {
                // abort window creation
                return(-1);
            }
            SetFocus(_hlb);

            SendMessage(
                _hlb,
                LB_SETCOUNT,
                (WPARAM)((_cbSize / BYTESPERLINE) + (_cbSize % BYTESPERLINE ? 1 : 0)),
                0);
            SendMessage(_hlb, WM_SETFONT, (WPARAM) _hfListBox, 0);
            SendMessage(_hlb, LB_SETCURSEL, 0, 0);
            _crTextHigh = GetSysColor(COLOR_HIGHLIGHTTEXT);
            _crTextNorm = GetSysColor(COLOR_WINDOWTEXT);
            _crBkHigh = GetSysColor(COLOR_HIGHLIGHT);
            _crBkNorm = GetSysColor(COLOR_WINDOW);
        }
        break;
    case WM_SETFOCUS:
        {
            if (_hlb)
            {
                SetFocus(_hlb);
            }
        break;
        }
    case WM_MEASUREITEM:
        {
            LPMEASUREITEMSTRUCT lpmis = (LPMEASUREITEMSTRUCT) lParam;
            lpmis->itemHeight = _tm.tmHeight + _tm.tmExternalLeading;
            lpmis->itemWidth = _wWidth;
            return(TRUE);
        }
    case WM_DRAWITEM:
        {
            LPDRAWITEMSTRUCT lpdis = (LPDRAWITEMSTRUCT) lParam;
            if (-1 == lpdis->itemID)
            {
                // no items in the listbox
                break;
            }
            // draw the item
            switch (lpdis->itemAction)
            {
            case ODA_SELECT:
            case ODA_DRAWENTIRE:
                {
                    // draw the listbox item
                    LONG lStartByte = lpdis->itemID * BYTESPERLINE;

                    // NOTE:  The following string is actually a bit larger
                    // than it has to be.  The minimum size for this string
                    // is acutally somewhere between 5 and 6 bytes for each
                    // byte (plus 12) in the stream that is to be displayed.
                    // Doing it this way wastes a little space but it's much
                    // simpler, it's faster, and it's less prone to error
                    // than dynamically computing it each time.
                    TCHAR szLine[BYTESPERLINE * 6 + 12];

                    unsigned cch;
                    wsprintf(szLine, TEXT("0x%08X:"), lStartByte);
                    for (cch = BYTESPERLINE * 6 + 12; (unsigned) lstrlen(TEXT("0x12345678:")) < cch--; )
                    {
                        szLine[cch] = TEXT(' ');
                    }

                    IStream * pstm = (IStream *) _ple->pInterface;
                    BYTE rgb[BYTESPERLINE];
                    LARGE_INTEGER dlibMove;
                    dlibMove.HighPart = 0;
                    // If we ever get a stream that's big enough to require
                    // that the HiPart get set, then nobody would want to
                    // browse that much data anyway!
                    dlibMove.LowPart = lStartByte;

                    HRESULT hr = pstm->Seek(dlibMove, STREAM_SEEK_SET, NULL);
                    if (FAILED(hr))
                    {
                        MessageBoxFromStringIdsAndArgs(
                            _hwnd,
                            _hInstance,
                            IDS_STMSEEKFAILED,
                            IDS_ERROR,
                            MB_OK | MB_ICONEXCLAMATION,
                            hr
                            );
                        return(FALSE);
                    }
                    ULONG cbRead;
                    hr = pstm->Read(rgb, BYTESPERLINE, &cbRead);
                    if (FAILED(hr))
                    {
                        MessageBoxFromStringIdsAndArgs(
                            _hwnd,
                            _hInstance,
                            IDS_STMREADFAILED,
                            IDS_ERROR,
                            MB_OK | MB_ICONEXCLAMATION,
                            hr
                            );
                        return(FALSE);
                    }

                    for (cch = 0; cch < cbRead; cch++)
                    {
                        TCHAR szTemp[3];
                        wsprintf(szTemp, TEXT("%02X"), rgb[cch]);
                        szLine[cch * 3 + cch / DIVISIONPOINTS + 12] = szTemp[0];
                        szLine[cch * 3 + cch / DIVISIONPOINTS + 13] = szTemp[1];
                        char ch = rgb[cch];
                        if (ch < ' ')
                        {
                            ch = '.';
                        }
                        // this is a little trick to get the character in the
                        // correct type of string (UNICODE or not)
                        wsprintf(szTemp, TEXT("%c"), ch);
                        szLine[cch + 13 + cch / DIVISIONPOINTS + 3 * BYTESPERLINE + BYTESPERLINE / DIVISIONPOINTS] = szTemp[0];
                    }
                    szLine[cch + 13 + cch / DIVISIONPOINTS + 3 * BYTESPERLINE + BYTESPERLINE / DIVISIONPOINTS] = 0;

                    SIZE sizeText;
                    RECT rcItem = lpdis->rcItem;

                    GetTextExtentPoint32(
                        lpdis->hDC,
                        szLine,
                        lstrlen(szLine),
                        &sizeText);
                    rcItem.right = rcItem.left + sizeText.cx;
                    COLORREF crOldText = SetTextColor(
                        lpdis->hDC,
                        (lpdis->itemState & ODS_SELECTED) ? _crTextHigh : _crTextNorm);
                    COLORREF crOldBack = SetBkColor(
                        lpdis->hDC,
                        (lpdis->itemState & ODS_SELECTED) ? _crBkHigh : _crBkNorm);
                    ExtTextOut(
                        lpdis->hDC,
                        rcItem.left,
                        rcItem.top,
                        ETO_OPAQUE | ETO_CLIPPED,
                        &rcItem,
                        szLine,
                        lstrlen(szLine),
                        NULL);
                    SetTextColor(lpdis->hDC, crOldText);
                    SetBkColor(lpdis->hDC, crOldBack);
                }
                break;
            case ODA_FOCUS:
                break;
            }
            return(TRUE);
        }
    case WM_SIZE:
        {
            _wWidth = LOWORD(lParam);
            WORD wHeight = HIWORD(lParam);
            if (_hlb)
            {
                SetWindowPos(_hlb, NULL, 0, 0, _wWidth, wHeight, SWP_NOMOVE | SWP_NOZORDER);
            }
            break;
        }
    case WM_COMMAND:
    case WM_QUIT:
    case WM_CLOSE:
    default:
        return(DefWindowProc(_hwnd, uMsg, wParam, lParam));
    }
    return(FALSE);
}

