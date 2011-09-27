//+---------------------------------------------------------------------------
//
//  Microsoft Windows
//  Copyright (C) Microsoft Corporation, 1992 - 1995.
//
//  File:       mwclass.cxx
//
//  Contents:   implementation for the main window class
//
//  Classes:    CMainWindow
//
//  Functions:  Exists
//
//  History:    6-08-94   stevebl   Created
//
//----------------------------------------------------------------------------

#include "dfv.h"
#include "mwclass.h"
#include "about.h"
#include "bitmaps.h"
#include "strmvwr.h"
#include <assert.h>

#define BITMAPWIDTH 16
#define BITMAPHEIGHT 16
#define INDENT BITMAPWIDTH
#define ROOTSTGSTR OLESTR(" \\ - ")

//+---------------------------------------------------------------------------
//
//  Member:     CMainWindow::CMainWindow
//
//  Synopsis:   constructor
//
//  History:    6-27-94   stevebl   Created
//
//----------------------------------------------------------------------------

CMainWindow::CMainWindow()
{
    _hlb = NULL;
    _hbmpStorage = _hbmpStream = NULL;
}

//+---------------------------------------------------------------------------
//
//  Member:     CMainWindow::~CMainWindow
//
//  Synopsis:   destructor
//
//  History:    6-27-94   stevebl   Created
//
//  Notes:      Destruction of the main window indicates that the app should
//              quit.
//
//----------------------------------------------------------------------------

CMainWindow::~CMainWindow()
{
    DeleteObject(_hbmpStorage);
    DeleteObject(_hbmpStream);
    PostQuitMessage(0);
}

//+---------------------------------------------------------------------------
//
//  Member:     CMainWindow::InitInstance
//
//  Synopsis:   Instantiates an instance of the docfile viewer window.
//
//  Arguments:  [hInstance] - instance of the app
//              [nCmdShow]  - command to pass to ShowWindow
//
//  Returns:    TRUE on success
//              FALSE on failure
//
//  History:    6-27-94   stevebl   Created
//
//  Notes:      This method must be called only once, immediately after
//              class construction.
//
//----------------------------------------------------------------------------

BOOL CMainWindow::InitInstance(HINSTANCE hInstance, int nCmdShow)
{
    // Note, the Create method sets the _hwnd member for me so I don't
    // need to set it myself.
    if (!Create(
        TEXT(MAIN_WINDOW_CLASS_NAME),
        TEXT(VER_INTERNALNAME_STR),
        WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX
            | WS_MAXIMIZEBOX | WS_THICKFRAME,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        NULL,
        NULL,
        hInstance))
    {
        return(FALSE);
    }

    ShowWindow(_hwnd, nCmdShow);
    UpdateWindow(_hwnd);

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
    return(TRUE);
}

//+---------------------------------------------------------------------------
//
//  Member:     CMainWindow::WindowProc
//
//  Synopsis:   main window procedure
//
//  Arguments:  [uMsg]   - Windows message
//              [wParam] - first message parameter
//              [lParam] - second message parameter
//
//  History:    6-27-94   stevebl   Created
//
//  Notes:      See CHlprWindow for a description of how this method gets
//              called by the global WinProc.
//
//----------------------------------------------------------------------------

LRESULT CMainWindow::WindowProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
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
            GetTextMetrics(hdc, &_tm);
            ReleaseDC(_hwnd, hdc);

            // create the child listbox
            _hlb = CreateWindow(
                TEXT("listbox"),
                TEXT(""),
                LBS_OWNERDRAWFIXED |
                    WS_CHILD |
                    WS_VISIBLE |
                    WS_VSCROLL |
                    LBS_NOTIFY |
                    LBS_NOINTEGRALHEIGHT |
                    LBS_WANTKEYBOARDINPUT,
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

            if (!LoadBitmaps())
            {
                MessageBoxFromStringIds(
                    _hwnd,
                    _hInstance,
                    IDS_LOADBITMAPSFAILED,
                    IDS_ERROR,
                    MB_OK | MB_ICONEXCLAMATION);
                return(-1);
            }
            _crTextHigh = GetSysColor(COLOR_HIGHLIGHTTEXT);
            _crTextNorm = GetSysColor(COLOR_WINDOWTEXT);
            _crBkHigh = GetSysColor(COLOR_HIGHLIGHT);
            _crBkNorm = GetSysColor(COLOR_WINDOW);

            // Fill in the File's OPENFILENAME structure
            _szFileName[0] = 0;
            _szFileTitle[0] = 0;
            _ofnFile.lStructSize = sizeof(OPENFILENAME);
            _ofnFile.hwndOwner = _hwnd;
            _ofnFile.lpstrFilter = TEXT("All Files (*.*)\0*.*\0Doc Files (*.DFL;*.OL?)\0*.DFL;*.OL;\0");
            _ofnFile.lpstrCustomFilter = NULL;
            _ofnFile.nMaxCustFilter = 0;
            _ofnFile.nFilterIndex = 1;
            _ofnFile.lpstrFile = _szFileName;
            _ofnFile.nMaxFile = MAX_PATH;
            _ofnFile.lpstrInitialDir = NULL;
            _ofnFile.lpstrFileTitle = _szFileTitle;
            _ofnFile.nMaxFileTitle = MAX_PATH;
            _ofnFile.lpstrTitle = TEXT("Open DocFile");
            _ofnFile.lpstrDefExt = NULL;
            _ofnFile.Flags = OFN_HIDEREADONLY;
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
            if (BITMAPHEIGHT < _tm.tmHeight + _tm.tmExternalLeading)
            {
                lpmis->itemHeight = _tm.tmHeight + _tm.tmExternalLeading;
            }
            else
            {
                lpmis->itemHeight = BITMAPHEIGHT;
            }
            lpmis->itemWidth = _wWidth;
            return(TRUE);
        }
    case WM_DELETEITEM:
        if (IDC_LISTBOX == wParam)
        {
            DeleteElement((LISTBOXELEMENT *)((LPDELETEITEMSTRUCT)lParam)->itemData);
        }
        break;
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

                    LISTBOXELEMENT * plbe = (LISTBOXELEMENT *) lpdis->itemData;
                    assert(plbe);

                    // compute the starting point
                    int x = lpdis->rcItem.left + plbe->uIndent * INDENT;
                    int y = lpdis->rcItem.top;

                    // paint the bitmap
                    HDC hMemDC = CreateCompatibleDC(lpdis->hDC);
                    HBITMAP hbmp;
                    switch (plbe->uType)
                    {
                    case LBE_STORAGE:
                        hbmp = _hbmpStorage;
                        break;
                    case LBE_STREAM:
                        hbmp = _hbmpStream;
                        break;
                    default:
                        assert(!"Unsupported type");
                        hbmp = _hbmpStream;
                        break;
                    }
                    HBITMAP hbmpOld = (HBITMAP) SelectObject(hMemDC, hbmp);
                    int nBitmap = ((lpdis->itemState & ODS_SELECTED) ? 0 : 2) +
                        plbe->fOpen;
                    BitBlt(
                        lpdis->hDC,
                        x,
                        y,
                        BITMAPWIDTH,
                        _cyBitmap,
                        hMemDC,
                        nBitmap * BITMAPWIDTH, 0, SRCCOPY);
                    SelectObject(hMemDC, hbmpOld);
                    DeleteDC(hMemDC);
                    x += BITMAPWIDTH;

                    // paint the text
                    SIZE sizeText;
                    RECT rcItem = lpdis->rcItem;
                    rcItem.left = x;

                    // Note that pwcsName is an OLEStr and
                    // is therefore always wide under WIN32.
                    GetTextExtentPoint32W(
                        lpdis->hDC,
                        plbe->pwcsName,
                        wcslen(plbe->pwcsName),
                        &sizeText);
                    rcItem.right = x + sizeText.cx;
                    COLORREF crOldText = SetTextColor(
                        lpdis->hDC,
                        (lpdis->itemState & ODS_SELECTED) ? _crTextHigh : _crTextNorm);
                    COLORREF crOldBack = SetBkColor(
                        lpdis->hDC,
                        (lpdis->itemState & ODS_SELECTED) ? _crBkHigh : _crBkNorm);
                    ExtTextOutW(
                        lpdis->hDC,
                        x,
                        y,
                        ETO_OPAQUE | ETO_CLIPPED,
                        &rcItem,
                        plbe->pwcsName,
                        wcslen(plbe->pwcsName),
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
    case WM_VKEYTOITEM:
        if ((HWND)lParam == _hlb)
        {
            if (LOWORD(wParam) == VK_RETURN)
            {
                if(SendMessage(_hlb, LB_GETCOUNT, 0, 0))
                {
                    Toggle(SendMessage(_hlb, LB_GETCURSEL, 0, 0));
                    InvalidateRect(_hlb, NULL, FALSE);
                }
                return(-2);
            }
            return(-1);
        }
        break;
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
        if ((LOWORD(wParam) == IDC_LISTBOX) && ((HWND) lParam == _hlb))
        {
            switch (HIWORD(wParam))
            {
                case LBN_DBLCLK:
                    {
                        if(SendMessage(_hlb, LB_GETCOUNT, 0, 0))
                        {
                            Toggle(SendMessage(_hlb, LB_GETCURSEL, 0, 0));
                            InvalidateRect(_hlb, NULL, FALSE);
                        }
                    }
                    break;
                default:
                    break;
            }
        }
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
//  Member:     CMainWindow::DoMenu
//
//  Synopsis:   implements the main menu commands
//
//  Arguments:  [wParam] - first window parameter
//              [lParam] - second window parameter
//
//  History:    6-23-94   stevebl   Created
//
//----------------------------------------------------------------------------

LRESULT CMainWindow::DoMenu(WPARAM wParam, LPARAM lParam)
{
    switch (LOWORD(wParam))
    {
    case IDM_OPEN:
        if (!GetOpenFileName((LPOPENFILENAME)&_ofnFile))
        {
            return(FALSE);
        }
        OpenFile();
        break;
    case IDM_CLOSE:
        SendMessage(_hlb, LB_RESETCONTENT, 0, 0);
        break;
    case IDM_EXPAND:
        if(SendMessage(_hlb, LB_GETCOUNT, 0, 0))
        {
            Expand(SendMessage(_hlb, LB_GETCURSEL, 0, 0));
            InvalidateRect(_hlb, NULL, FALSE);
        }
        break;
    case IDM_EXPANDBRANCH:
        if(SendMessage(_hlb, LB_GETCOUNT, 0, 0))
        {
            ExpandAll(SendMessage(_hlb, LB_GETCURSEL, 0, 0));
            InvalidateRect(_hlb, NULL, FALSE);
        }
        break;
    case IDM_EXPANDALL:
        if(SendMessage(_hlb, LB_GETCOUNT, 0, 0))
        {
            ExpandAll(0);
            InvalidateRect(_hlb, NULL, FALSE);
        }
        break;
    case IDM_COLLAPSE:
        if(SendMessage(_hlb, LB_GETCOUNT, 0, 0))
        {
            Collapse(SendMessage(_hlb, LB_GETCURSEL, 0, 0));
            InvalidateRect(_hlb, NULL, FALSE);
        }
        break;
    case IDM_COLLAPSEALL:
        if(SendMessage(_hlb, LB_GETCOUNT, 0, 0))
        {
            Collapse(0);
            SendMessage(_hlb, LB_SETCURSEL, 0, 0);
            InvalidateRect(_hlb, NULL, FALSE);
        }
        break;
    case IDM_TOGGLE:
        if (SendMessage(_hlb, LB_GETCOUNT, 0, 0))
        {
            Toggle(SendMessage(_hlb, LB_GETCURSEL, 0, 0));
            InvalidateRect(_hlb, NULL, FALSE);
        }
    case IDM_EXIT:
        SendMessage(_hwnd, WM_CLOSE, 0, 0);
        break;
    case IDM_HELP:
        if (!Exists(_szHelpFile))
        {
            MessageBoxFromStringIds(
                _hwnd,
                _hInstance,
                IDS_NOHELPFILE,
                IDS_ERROR,
                MB_OK | MB_ICONEXCLAMATION);
        }
        else
        {
            WinHelp(_hwnd, _szHelpFile, HELP_CONTEXT, 3);
        }
        break;
    case IDM_ABOUT:
        {
            CAbout dlgAbout;
            dlgAbout.ShowDialog(_hInstance, MAKEINTRESOURCE(IDM_ABOUT), _hwnd);
        }
        break;
    default:
        return(DefWindowProc(_hwnd, WM_COMMAND, wParam, lParam));
    }
    return(FALSE);
}

//+---------------------------------------------------------------------------
//
//  Member:     CMainWindow::Expand
//
//  Synopsis:   Expands a level of the tree.
//
//  Arguments:  [lIndex] - index of the item that needs to be expanded
//
//  History:    6-23-94   stevebl   Created
//
//----------------------------------------------------------------------------

void CMainWindow::Expand(long lIndex)
{
    LISTBOXELEMENT * pElement = (LISTBOXELEMENT *) SendMessage(_hlb, LB_GETITEMDATA, lIndex, 0);
    if (pElement->fOpen)
    {
        // this element has already been expanded
        return;
    }
    SendMessage(_hlb, WM_SETREDRAW, FALSE, 0);
    switch (pElement->uType)
    {
    case LBE_STORAGE:
        {
            // create entries for all of its children
            IStorage * pstg = (IStorage *) pElement->pInterface;
            IEnumSTATSTG * penumStatStg;
            STATSTG statstg;
            HRESULT hr = pstg->EnumElements(0, NULL, 0, &penumStatStg);
            if (SUCCEEDED(hr))
            {
                pElement->fOpen = TRUE;
                pElement->nChildren = 0;
                while (hr != S_FALSE)
                {
                    hr = penumStatStg->Next(1, &statstg, NULL);
                    if (FAILED(hr))
                    {
                        MessageBoxFromStringIdsAndArgs(
                            _hwnd,
                            _hInstance,
                            IDS_ENUMSTATSTGFAILED,
                            IDS_ERROR,
                            MB_OK | MB_ICONEXCLAMATION,
                            hr);
                    }
                    else if (S_OK == hr)
                    {
                        // retrieved another member from the list
                        LISTBOXELEMENT * plbe;
                        switch (statstg.type)
                        {
                        case STGTY_STORAGE:
                            {
                                IStorage * pstg2;
                                hr = pstg->OpenStorage(
                                    statstg.pwcsName,
                                    NULL,
                                    STGM_READ | STGM_SHARE_EXCLUSIVE,
                                    NULL,
                                    0,
                                    &pstg2);
                                if (SUCCEEDED(hr))
                                {
                                    plbe = CreateStorageElement(pstg2);
                                }
                                else
                                {
                                    MessageBoxFromStringIdsAndArgs(
                                        _hwnd,
                                        _hInstance,
                                        IDS_OPENSTORAGEFAILED,
                                        IDS_ERROR,
                                        MB_OK | MB_ICONEXCLAMATION,
                                        hr);
                                    plbe = NULL;
                                }
                            }
                            break;
                        case STGTY_STREAM:
                            {
                                IStream * pstm;
                                hr = pstg->OpenStream(
                                    statstg.pwcsName,
                                    0,
                                    STGM_READ | STGM_SHARE_EXCLUSIVE,
                                    0,
                                    &pstm);
                                if (SUCCEEDED(hr))
                                {
                                    plbe = CreateStreamElement(pstm);
                                }
                                else
                                {
                                    MessageBoxFromStringIdsAndArgs(
                                        _hwnd,
                                        _hInstance,
                                        IDS_OPENSTREAMFAILED,
                                        IDS_ERROR,
                                        MB_OK | MB_ICONEXCLAMATION,
                                        hr);
                                    plbe = NULL;
                                }
                            }
                            break;
                        }
                        if (plbe)
                        {
                            plbe->pwcsName = statstg.pwcsName;
                            plbe->uIndent = pElement->uIndent + 1;
                            pElement->nChildren++;
                            if (SendMessage(_hlb, LB_INSERTSTRING, lIndex + pElement->nChildren, (LPARAM) plbe) == LB_ERRSPACE)
                            {
                                MessageBoxFromStringIds(
                                    _hwnd,
                                    _hInstance,
                                    IDS_INSERTSTRINGFAILED,
                                    IDS_ERROR,
                                    MB_OK | MB_ICONEXCLAMATION);
                                DeleteElement(plbe);
                                pElement->nChildren--;
                            }
                        }
                    }
                }
                penumStatStg->Release();
            }
            else
            {
                MessageBoxFromStringIdsAndArgs(
                    _hwnd,
                    _hInstance,
                    IDS_ENUMELEMENTSFAILED,
                    IDS_ERROR,
                    MB_OK | MB_ICONEXCLAMATION,
                    hr);
            }
        }
        break;
    case LBE_STREAM:
        // streams are only expanded if they are the currently selected list item
        if (SendMessage(_hlb, LB_GETCURSEL, 0, 0) == lIndex)
        {
            // open stream
            CStreamView * psv = new CStreamView(_hlb, pElement);
            if (psv)
            {
                pElement->fOpen = psv->InitInstance(_hInstance, SW_SHOWNORMAL);
                if (pElement->fOpen)
                {
                    pElement->hwndView = psv->GetHwnd();
                    SetFocus(pElement->hwndView);
                }
            }
        }
        break;
    }
    SendMessage(_hlb, WM_SETREDRAW, TRUE, 0);
}

//+---------------------------------------------------------------------------
//
//  Member:     CMainWindow::Collapse
//
//  Synopsis:   collapses a level of the tree
//
//  Arguments:  [lIndex] - index of the item to be collapsed
//
//  History:    6-23-94   stevebl   Created
//
//----------------------------------------------------------------------------

void CMainWindow::Collapse(long lIndex)
{
    LISTBOXELEMENT * pElement = (LISTBOXELEMENT *) SendMessage(_hlb, LB_GETITEMDATA, lIndex, 0);
    if (pElement->fOpen)
    {
        switch (pElement->uType)
        {
        case LBE_STORAGE:
            while (pElement->nChildren--)
            {
                Collapse(lIndex + 1);
                SendMessage(_hlb, LB_DELETESTRING, lIndex + 1, 0);
            }
            break;
        case LBE_STREAM:
            if (pElement->fOpen)
            {
                DestroyWindow(pElement->hwndView);
            }
            break;
        }
        pElement->fOpen = FALSE;
    }
}

//+---------------------------------------------------------------------------
//
//  Member:     CMainWindow::ExpandAll
//
//  Synopsis:   expands the current item and all its children
//
//  Arguments:  [lIndex] - index of the item to be expanded
//
//  History:    6-23-94   stevebl   Created
//
//----------------------------------------------------------------------------

void CMainWindow::ExpandAll(long lIndex)
{
    LISTBOXELEMENT * pElement = (LISTBOXELEMENT *) SendMessage(_hlb, LB_GETITEMDATA, lIndex, 0);
    Expand(lIndex);
    if (pElement->uType == LBE_STORAGE)
    {
        unsigned nChildren = pElement->nChildren;
        while (nChildren--)
        {
            ExpandAll(lIndex + 1 + nChildren);
        }
    }
}

//+---------------------------------------------------------------------------
//
//  Member:     CMainWindow::Toggle
//
//  Synopsis:   toggles a node - expanding or collapsing as appropriate
//
//  Arguments:  [lIndex] - index of the node
//
//  History:    6-25-94   stevebl   Created
//
//----------------------------------------------------------------------------

void CMainWindow::Toggle(long lIndex)
{
    LISTBOXELEMENT * pElement = (LISTBOXELEMENT *) SendMessage(_hlb, LB_GETITEMDATA, lIndex, 0);
    if (pElement->fOpen)
    {
        Collapse(lIndex);
    }
    else
    {
        Expand(lIndex);
    }
}

//+---------------------------------------------------------------------------
//
//  Member:     CMainWindow::CreateStreamElement
//
//  Synopsis:   turns an IStream pointer into a listbox element
//
//  Arguments:  [pstm] - IStream pointer
//
//  Returns:    pointer to the newly created element
//              (NULL on failure)
//
//  History:    6-23-94   stevebl   Created
//
//----------------------------------------------------------------------------

LISTBOXELEMENT * CMainWindow::CreateStreamElement(IStream * pstm)
{
    LISTBOXELEMENT * plbe = new LISTBOXELEMENT;
    if (plbe)
    {
        plbe->uType = LBE_STREAM;
        plbe->fOpen = FALSE;
        plbe->pInterface = (void *) pstm;
        plbe->uIndent = 0;
        plbe->hwndView = NULL;
    }
    else
    {
        MessageBoxFromStringIds(
            _hwnd,
            _hInstance,
            IDS_OUTOFMEMORY,
            IDS_ERROR,
            MB_OK | MB_ICONEXCLAMATION);
    }
    return(plbe);
}

//+---------------------------------------------------------------------------
//
//  Member:     CMainWindow::CreateStorageElement
//
//  Synopsis:   turns an IStorage pointer into a listbox element
//
//  Arguments:  [pstg] - IStorage pointer
//
//  Returns:    pointer to the newly created element
//              (NULL on failure)
//
//  History:    6-23-94   stevebl   Created
//
//----------------------------------------------------------------------------

LISTBOXELEMENT * CMainWindow::CreateStorageElement(IStorage * pstg)
{
    LISTBOXELEMENT * plbe = new LISTBOXELEMENT;
    if (plbe)
    {
        plbe->uType = LBE_STORAGE;
        plbe->fOpen = FALSE;
        plbe->pInterface = (void *) pstg;
        plbe->uIndent = 0;
        plbe->nChildren = 0;
    }
    else
    {
        MessageBoxFromStringIds(
            _hwnd,
            _hInstance,
            IDS_OUTOFMEMORY,
            IDS_ERROR,
            MB_OK | MB_ICONEXCLAMATION);
    }
    return(plbe);
}

//+---------------------------------------------------------------------------
//
//  Member:     CMainWindow::DeleteElement
//
//  Synopsis:   deletes a listbox element and it's associated data
//
//  Arguments:  [plbe] - pointer to the element
//
//  History:    6-23-94   stevebl   Created
//
//  Notes:      This doesn't remove it from the list, it just frees up
//              the LISTBOXELEMENT pointer and it's data.
//
//----------------------------------------------------------------------------

void CMainWindow::DeleteElement(LISTBOXELEMENT * plbe)
{

    switch (plbe->uType)
    {
    case LBE_STORAGE:
        ((IStorage *) plbe->pInterface)->Release();
        break;
    case LBE_STREAM:
        if (plbe->fOpen)
        {
            DestroyWindow(plbe->hwndView);
        }
        ((IStream *) plbe->pInterface)->Release();
        break;
    }
    LPMALLOC pMalloc;
    CoGetMalloc(MEMCTX_TASK, &pMalloc);
    pMalloc->Free(plbe->pwcsName);
    pMalloc->Release();
    delete(plbe);
}

//+---------------------------------------------------------------------------
//
//  Member:     CMainWindow::LoadBitmaps
//
//  Synopsis:   loads the Storage and Stream bitmap arrays
//
//  Returns:    TRUE on success
//              FALSE on failure
//
//  Modifies:   _hbmpStorage and _hbmpStream
//
//  History:    6-24-94   stevebl   Created from code used in original DFView
//
//----------------------------------------------------------------------------

BOOL CMainWindow::LoadBitmaps(void)
{
    //
    // Get some metrics/info
    //
    UINT cxBitmap = BITMAPWIDTH;
    _cyBitmap = BITMAPHEIGHT;
    if (_cyBitmap < (UINT) _tm.tmHeight + _tm.tmExternalLeading)
    {
        _cyBitmap = _tm.tmHeight + _tm.tmExternalLeading;
    }
    UINT crHigh   = (COLORREF) GetSysColor(COLOR_HIGHLIGHT);
    UINT crNorm   = (COLORREF) GetSysColor(COLOR_WINDOW);

    HDC hdc = CreateCompatibleDC(NULL);
    if (!hdc)
    {
        goto ERRLOADBITMAP;
    }
    _hbmpStorage = CreateBitmap(
        4 * cxBitmap,
        _cyBitmap,
        GetDeviceCaps(hdc, PLANES),
        GetDeviceCaps(hdc, BITSPIXEL),
        NULL);
    _hbmpStream = CreateBitmap(
        4 * cxBitmap,
        _cyBitmap,
        GetDeviceCaps(hdc, PLANES),
        GetDeviceCaps (hdc, BITSPIXEL),
        NULL);
   DeleteDC(hdc);
   if (!_hbmpStorage || !_hbmpStream)
   {
        goto ERRLOADBITMAP;
   }

    //
    //  Load our bitmaps, and munge them to create
    //  our 4-bitmap "arrays".  Anything RED in the loaded
    //  bitmap will be considered clear.
    //
    if (!LoadAndStretch(
        _hInstance,
        _hbmpStorage,
        MAKEINTRESOURCE(BMP_STORAGE),
        cxBitmap,
        _cyBitmap,
        crHigh,
        crNorm))
    {
        goto ERRLOADBITMAP;
    }
    if (!LoadAndStretch(
        _hInstance,
        _hbmpStream,
        MAKEINTRESOURCE(BMP_STREAM),
        cxBitmap,
        _cyBitmap,
        crHigh,
        crNorm))
    {
        goto ERRLOADBITMAP;
    }
    return TRUE;

ERRLOADBITMAP:
    //
    // Bitmap loading error.
    //
    DeleteObject(_hbmpStorage);
    DeleteObject(_hbmpStream);
    _hbmpStorage = NULL;
    _hbmpStream = NULL;

    return FALSE;
}

//+---------------------------------------------------------------------------
//
//  Member:     CMainWindow::OpenFile
//
//  Synopsis:   attempts to open whatever docfile is named by _szFileName;
//
//  History:    6-24-94   stevebl   Created
//
//----------------------------------------------------------------------------

void CMainWindow::OpenFile(void)
{
    SendMessage(_hlb, LB_RESETCONTENT, 0, 0);
    IStorage * pstg;
#ifndef UNICODE
    // convert file name to a wide string
    OLECHAR wszFileName[MAX_PATH];
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
        STGM_TRANSACTED | STGM_READ | STGM_SHARE_DENY_WRITE,
        NULL,
        0,
        &pstg);
    if (FAILED(hr))
    {
        MessageBoxFromStringIdsAndArgs(
            _hwnd,
            _hInstance,
            IDS_STGOPENSTORAGEFAILED,
            IDS_ERROR,
            MB_OK | MB_ICONEXCLAMATION,
#ifdef UNICODE
            _szFileName
#else
            wszFileName
#endif
            );
        return;
    }
    // Storage was opened successfully.
    LISTBOXELEMENT * plbe = CreateStorageElement(pstg);
    if (plbe)
    {
        IMalloc * pMalloc;
        CoGetMalloc(MEMCTX_TASK, &pMalloc);
        plbe->pwcsName = (LPOLESTR) pMalloc->Alloc(sizeof(OLECHAR) *  (lstrlen(_szFileName) + wcslen(ROOTSTGSTR) + 1));
        pMalloc->Release();
        if (plbe->pwcsName)
        {
            wcscpy(plbe->pwcsName, ROOTSTGSTR);
#ifdef UNICODE
            wcscpy(&plbe->pwcsName[wcslen(ROOTSTGSTR)], _szFileName);
#else
            mbstowcs(&plbe->pwcsName[wcslen(ROOTSTGSTR)], _szFileName, strlen(_szFileName) + 1);
#endif
        }
        if (SendMessage(_hlb, LB_INSERTSTRING, 0, (LPARAM) plbe) == LB_ERRSPACE)
        {
            MessageBoxFromStringIds(
                _hwnd,
                _hInstance,
                IDS_INSERTSTRINGFAILED,
                IDS_ERROR,
                MB_OK | MB_ICONEXCLAMATION);
            DeleteElement(plbe);
        }
        else
        {
            SendMessage(_hlb, LB_SETCURSEL, 0, 0);
        }
    }
}

//+---------------------------------------------------------------------------
//
//  Function:   Exists
//
//  Synopsis:   simple function to test for the existance of a file
//
//  History:    6-16-93   stevebl   Created
//
//----------------------------------------------------------------------------

int Exists(TCHAR *sz)
{
    HANDLE h;
    h = CreateFile(sz,
        GENERIC_READ,
        FILE_SHARE_READ | FILE_SHARE_WRITE,
        NULL,
        OPEN_EXISTING,
        0,
        0);
    if (h != INVALID_HANDLE_VALUE)
    {
        CloseHandle(h);
        return(1);
    }
    return (0);
}

