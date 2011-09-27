// idataobj.cpp
//
// Implementation file for the IDataObject interface viewer.
//
// Copyright (c) 1993-1995 Microsoft Corporation, All Rights Reserved.
//
// Revisions:
//  August 7, 1993  cek     First implementation.
//  February 19, 1994 cek   Removed XRT stuff.  Fixed minor bugs.
//  April 9, 1994 cek       Pulled from XRTView and genericized again.
//
#include "precomp.h"
#include "defo2v.h"

#include "idataobj.h"
#include "iadvsink.h"

#if defined(WIN32) && !defined(_UNICODE)
    #pragma message("Building ANSI version of " __FILE__)
#endif

static HFONT       g_hFont ;
static HFONT       g_hFontBold ;
static UINT        g_cyFont ;

static WNDPROC     g_pfnEdit = NULL ;

extern "C"
LRESULT EXPORT CALLBACK fnEditSubclass( HWND hwnd, UINT uiMsg, WPARAM wP, LPARAM lP ) ;

LPCTSTR MyGetClipboardFormatName( UINT cf ) ;

// DisplayIDataObject
//
// This is the "DisplayInterface" or "Interface Viewer" function for the
// IDataObject interface.   This implementation simply pops up a dialog
// box that has an ownerdraw listbox in it.
//
// It is expected that this function will be called from the Ole2View
// tool.   See the Ole2View helpfile in the OLE 2.01 and MSVC++ 1.5 kits
// for details on Ole2View interface viewers.
//
// The listbox is filled with the FORMATETCs that are enumerated through
// the IEnumFORMATETC interface that is retrieved from IDataObject::EnumFormatEtc
//
extern "C"
HRESULT CALLBACK DisplayIDataObject( HWND hwndParent, LPDATAOBJECT lpDO, LPIID lpiid, LPTSTR lpszName )
{
    HRESULT hr = NULL ;

    CIDataObjectDlg FAR* pdlg ;
    pdlg = new FAR CIDataObjectDlg( hwndParent, lpDO, lpiid, lpszName ) ;

    if (pdlg)
    {
        pdlg->DoModal() ;
        delete pdlg ;
    }

    return hr ;
}

/////////////////////////////////////////////////////////////////////////////
// CIDataObjectDlg dialog
//
CIDataObjectDlg::CIDataObjectDlg( HWND hwnd, LPDATAOBJECT lpDO, LPIID lpiid, LPTSTR lpszName )
{
    m_lbFmtEtc = NULL ;
    m_edtAdvise = NULL ;
    m_MetaFile.hMF = NULL ;

    m_hWndParent = hwnd ;
    m_lpDO = lpDO ;
    m_lpiid = lpiid ;
    m_lpszName = lpszName ;

    m_pSink = NULL ;
    m_dwConn = 0 ;
    m_advf = ADVF_PRIMEFIRST ;

#ifdef _UNICODE
    m_fetc.cfFormat = CF_UNICODETEXT ;
#else
    m_fetc.cfFormat = CF_TEXT ;
#endif
    m_fetc.dwAspect = DVASPECT_CONTENT ;
    m_fetc.ptd = NULL ;
    m_fetc.tymed = TYMED_HGLOBAL ;
    m_fetc.lindex = - 1 ;

    m_fDoOnGetDataPosted = FALSE ;

    m_cchOutput = 0 ;
    m_cLinesOutput = 0 ;
}

CIDataObjectDlg::~CIDataObjectDlg()
{
    if (m_MetaFile.hMF != NULL)
        DeleteMetaFile( m_MetaFile.hMF ) ;
}

int CIDataObjectDlg::DoModal( void )
{
#ifdef WIN32
    if (-1 == DialogBoxParam( GetModuleHandle(_T("DEFO2V32.DLL")), MAKEINTRESOURCE( IDD_IDATAOBJDLG ), m_hWndParent, (DLGPROC)fnIDataObjectDlg, (LONG)this ))
    {
        DWORD dw = GetLastError() ;
        TCHAR sz[1024] ;
        LPVOID lpMessageBuffer ;
        if (FormatMessage(
              FORMAT_MESSAGE_ALLOCATE_BUFFER |
              FORMAT_MESSAGE_FROM_SYSTEM,
              NULL,
              dw,
              MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US),
              (LPTSTR) &lpMessageBuffer,
              0,
              NULL ))
        {
            wsprintf( sz, _T( "%s (0x%lx)" ), (LPTSTR)lpMessageBuffer, dw);
            LocalFree(lpMessageBuffer) ;
        }
        else
        {
            wsprintf( sz, _T( "Unknown Error (0x%lx)" ), dw);
        }

        MessageBox( m_hWndParent, sz, _T("Could not open dialog box"), MB_OK ) ;
        return -1 ;
    }
    else
        return 0 ;
#else
    return DialogBoxParam( g_hInst, MAKEINTRESOURCE( IDD_IDATAOBJDLG ), m_hWndParent, (DLGPROC)fnIDataObjectDlg, (LONG)this ) ;
#endif
}

extern "C"
BOOL EXPORT CALLBACK fnIDataObjectDlg( HWND hDlg, UINT uiMsg, WPARAM wParam, LPARAM lParam )
{
    static BOOL fInErrSpace = FALSE ;

    CIDataObjectDlg FAR* pIDOD =(CIDataObjectDlg FAR*)GetWindowLong( hDlg, DWL_USER ) ;

    switch (uiMsg)
    {
        case WM_INITDIALOG:
            pIDOD=(CIDataObjectDlg FAR*)lParam ;
            if (pIDOD==NULL)
            {
                EndDialog( hDlg, 0 ) ;
                return TRUE ;
            }
            SetWindowLong( hDlg, DWL_USER, (LONG)pIDOD ) ;
            pIDOD->m_hDlg = hDlg ;
            return pIDOD->OnInitDialog() ;
        break ;

        case WM_DESTROY:
            if (pIDOD)
                pIDOD->OnDestroy() ;
        break ;

        case WM_SIZE:
            if (pIDOD)
                pIDOD->OnSize( (UINT)wParam, LOWORD( lParam ), HIWORD( lParam ) ) ;
        break ;

        case WM_COMMAND:
        {
            #ifdef WIN32
            WORD wNotifyCode = HIWORD(wParam);
            WORD wID = LOWORD(wParam);
            HWND hwndCtl = (HWND) lParam;
            #else
            WORD wNotifyCode = HIWORD(lParam) ;
            WORD wID = wParam ;
            HWND hwndCtl = (HWND)LOWORD(lParam) ;
            #endif

            switch (wID)
            {
                case IDCANCEL:
                    EndDialog( hDlg, IDCANCEL ) ;
                break ;

                case IDC_DOGETDATA:
                    pIDOD->OnDoGetData() ;
                break ;

                case IDC_SETUPADVISE:
                    if (pIDOD->m_dwConn != 0)
                        pIDOD->OnKillAdvise() ;
                    else
                        pIDOD->OnSetupAdvise() ;
                break ;

                case IDC_CLEAROUTPUT:
                    if (pIDOD)
                    {
                        pIDOD->m_cchOutput = 0 ;
                        pIDOD->m_cLinesOutput = 0 ;
                        if (pIDOD->m_MetaFile.hMF != NULL)
                        {
                            DeleteMetaFile( pIDOD->m_MetaFile.hMF ) ;
                            pIDOD->m_MetaFile.hMF = NULL ;
                        }
                        SetWindowText( pIDOD->m_edtAdvise, _T("") ) ;
                        InvalidateRect( pIDOD->m_edtAdvise, NULL, TRUE ) ;
                        UpdateWindow( pIDOD->m_edtAdvise ) ;
                    }
                break ;

                case IDC_FORMATETC:
                    if (wNotifyCode == LBN_SELCHANGE)
                        pIDOD->OnSelChangeFormatEtc() ;
                break ;

                case IDC_UPDATEDISPLAY:
                    pIDOD->m_fUpdateDisplay = Button_GetCheck( pIDOD->m_chkUpdateDisplay ) ;
                break ;

                case IDC_PRIMEFIRST:
                    if (Button_GetCheck( pIDOD->m_chkPrimeFirst ))
                        pIDOD->m_advf = ADVF_PRIMEFIRST ;
                    else
                        pIDOD->m_advf = ADVF_NODATA ;
                break ;

                case IDC_ADVISEDATA:
                    if (wNotifyCode == EN_ERRSPACE)
                    {
                        #ifdef _DEBUG
                        OutputDebugString(_T("Output Edit Control reports EN_ERRSPACE\r\n")) ;
                        #endif
                    }
                break ;
            }
        }
        break ;

        case WM_DRAWITEM:
            if (pIDOD)
                pIDOD->OnDrawItem( wParam, (LPDRAWITEMSTRUCT)lParam ) ;
        break ;

        case WM_MEASUREITEM:
            if (pIDOD)
                pIDOD->OnMeasureItem( wParam, (LPMEASUREITEMSTRUCT)lParam ) ;
        break ;

        case WM_OUTPUTBUFFERHASDATA:
            if (pIDOD)
                pIDOD->OnOutputBufferHasData() ;
        break ;

        default:
           return FALSE ;
    }
    return TRUE ;
}


/////////////////////////////////////////////////////////////////////////////
// CIDataObjectDlg message handlers

BOOL CIDataObjectDlg::OnInitDialog()
{
    m_btnDoGetData = GetDlgItem( m_hDlg, IDC_DOGETDATA ) ;
    m_btnSetupAdvise = GetDlgItem( m_hDlg, IDC_SETUPADVISE ) ;

    m_lbGetData = GetDlgItem( m_hDlg, IDC_GETDATA ) ;
    m_lbFmtEtc = GetDlgItem( m_hDlg, IDC_FORMATETC ) ;

    m_edtAdvise = GetDlgItem( m_hDlg, IDC_ADVISEDATA ) ;

    // Sublcass the edit so we can override it's WM_PAINT and
    // draw a metafile if we want to...
    if (g_pfnEdit == NULL)
        g_pfnEdit = SubclassWindow( m_edtAdvise, fnEditSubclass) ;

    m_chkUpdateDisplay = GetDlgItem( m_hDlg, IDC_UPDATEDISPLAY );
    m_fUpdateDisplay = TRUE ;
    Button_SetCheck( m_chkUpdateDisplay, m_fUpdateDisplay ) ;

    m_chkPrimeFirst = GetDlgItem( m_hDlg, IDC_PRIMEFIRST );
    Button_SetCheck( m_chkPrimeFirst, m_advf == ADVF_PRIMEFIRST ) ;

    //m_chkDump = GetDlgItem( m_hDlg, IDC_DUMPTOFILE ) ;
    //EnableWindow( m_chkDump, FALSE ) ;

    TEXTMETRIC  tm ;
    HDC hdc = GetDC(NULL);
    g_hFont = ReallyCreateFont( hdc, _T( "MS Sans Serif" ), _T( "Regular" ), 8, 0 ) ;
    g_hFontBold = ReallyCreateFont( hdc, _T( "MS Sans Serif" ), _T( "Bold" ), 8, 0 ) ;
    g_hFont = (HFONT)SelectObject( hdc, g_hFont ) ;
    GetTextMetrics( hdc, &tm ) ;
    g_hFont = (HFONT)SelectObject( hdc, g_hFont ) ;
    ReleaseDC( NULL, hdc ) ;
    g_cyFont = tm.tmHeight + tm.tmExternalLeading ;

    SetWindowFont( m_lbFmtEtc, g_hFont, TRUE ) ;
    SetWindowFont( m_lbGetData, g_hFont, TRUE ) ;
    SetWindowFont( m_edtAdvise, g_hFont, TRUE ) ;

    if (IsEqualCLSID( IID_IEnumFORMATETC, *m_lpiid ))
        DoIEnumFormatEtc( 0, (LPENUMFORMATETC)m_lpDO) ;
    else if (IsEqualCLSID( IID_IDataObject, *m_lpiid ))
    {
        DoIDataObject( 0, (LPDATAOBJECT)m_lpDO ) ;
    }
    else
    {
        MessageBox( m_hDlg, _T( "Wrong IID" ), _T( "Error" ), MB_OK ) ;
    }

    DlgCenter( m_hDlg, m_hWndParent, FALSE ) ;
    RECT rc ;
    GetWindowRect( m_hDlg, &rc ) ;
    SetWindowPos( m_hDlg, NULL, rc.left, rc.top, rc.right - rc.left + 1,
                  rc.bottom - rc.top +1, SWP_NOMOVE|SWP_NOZORDER | SWP_NOACTIVATE ) ;

    SetWindowText( m_hDlg, m_lpszName ) ;

    return TRUE;  // return TRUE  unless you set the focus to a control
}

void CIDataObjectDlg::OnDestroy()
{
    OnKillAdvise() ;

    if (g_pfnEdit != NULL && m_edtAdvise && IsWindow(m_edtAdvise))
    {
        SubclassWindow(m_edtAdvise, g_pfnEdit) ;
        g_pfnEdit = NULL ;
    }

    int c = ListBox_GetCount( m_lbFmtEtc ) ;
    for (int i = 0 ; i < c ; i++)
    {
        LPITEMDATA lpID = (LPITEMDATA)ListBox_GetItemData( m_lbFmtEtc, i ) ;
        if (lpID)
        {
            if (lpID->uiType == I_COLUMNHEAD)
                delete lpID->rgCol ;

            if (lpID->lpData != NULL)
                delete (LPFORMATETC)lpID->lpData ;
            delete lpID ;
        }
    }

    if (g_hFont)
        DeleteObject( g_hFont );

    if (g_hFontBold)
        DeleteObject (g_hFontBold) ;
}

void CIDataObjectDlg::OnSize(UINT nType, int cx, int cy)
{
    if (m_edtAdvise && !IsWindow( m_edtAdvise ) )
        return ;

    RECT    rc ;
    RECT    rcWnd ;
    GetClientRect( m_hDlg, &rcWnd ) ;
    GetWindowRect( m_edtAdvise, &rc ) ;
    MapWindowPoints( NULL, m_hDlg, (POINT FAR*)&rc, 2 ) ;
    SetWindowPos( m_edtAdvise, NULL, -1, rc.top, cx+2, cy - rc.top + 1,
                SWP_NOZORDER | SWP_NOACTIVATE ) ;
}


void CIDataObjectDlg::OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDIS )
{
    if (lpDIS->itemID == LB_ERR)
        return ;

    LPITEMDATA      lpID ;
    COLORREF        rgbBack ;
    RECT            rcFocus ;
    BOOL            fSelected ;
    int             x, y, cy ;
    TCHAR           szItem[128] ;

    lpID = (LPITEMDATA)lpDIS->itemData ;
    rcFocus = lpDIS->rcItem ;
    ListBox_GetText( m_lbFmtEtc, lpDIS->itemID, szItem ) ;

    if (fSelected = (lpDIS->itemState & ODS_SELECTED) ? TRUE : FALSE)
    {
        SetTextColor( lpDIS->hDC, GetSysColor( COLOR_HIGHLIGHTTEXT ) ) ;
        SetBkColor( lpDIS->hDC, rgbBack = GetSysColor( COLOR_HIGHLIGHT ) ) ;
    }
    else
    {
        SetTextColor( lpDIS->hDC, GetSysColor( COLOR_WINDOWTEXT ) ) ;
        SetBkColor( lpDIS->hDC, rgbBack = GetSysColor( COLOR_WINDOW ) ) ;
    }

    // if we are loosing focus, remove the focus rect before
    // drawing.
    //
    if ((lpDIS->itemAction) & (ODA_FOCUS))
        if (!((lpDIS->itemState) & (ODS_FOCUS)))
            DrawFocusRect( lpDIS->hDC, &rcFocus ) ;

    y = lpDIS->rcItem.top ;
    x = lpDIS->rcItem.left ;

    int cxChar = GetTextMetricsCorrectly( lpDIS->hDC, NULL ) ;

    if (lpID && (lpID->uiType == I_COLUMNHEAD || lpID->uiType == I_LABEL))
        g_hFontBold = (HFONT)SelectObject( lpDIS->hDC, g_hFontBold ) ;

    cy = (rcFocus.bottom - rcFocus.top - g_cyFont) / 2 ;

    ExtTextOut( lpDIS->hDC, x+2, y + cy, ETO_OPAQUE, &lpDIS->rcItem, NULL, 0, NULL ) ;
    if (lpID)
        ColumnTextOut( lpDIS->hDC, x + 2 + ((cxChar*3) * lpID->nLevel), y + cy, szItem, lpID->cColumns, lpID->rgCol ) ;

    if (lpID && lpID->uiType == I_COLUMNHEAD )
    {
        COLORREF    rgb ;
        RECT        rc = rcFocus ;
        rgb = SetBkColor( lpDIS->hDC, GetTextColor( lpDIS->hDC ) ) ;
        rc.top = rc.bottom - 1 ;
        rc.left = x + 2 + ((cxChar*3) * lpID->nLevel) ;
        ExtTextOut( lpDIS->hDC, 0, 0, ETO_OPAQUE, &rc, NULL, 0, NULL ) ;
        SetBkColor( lpDIS->hDC, rgb ) ;
    }

    if (lpID && (lpID->uiType == I_COLUMNHEAD || lpID->uiType == I_LABEL))
        g_hFontBold = (HFONT)SelectObject( lpDIS->hDC, g_hFontBold ) ;

    // if we are gaining focus draw the focus rect after drawing
    // the text.
    if ((lpDIS->itemAction) & (ODA_FOCUS))
        if ((lpDIS->itemState) & (ODS_FOCUS))
             DrawFocusRect( lpDIS->hDC, &rcFocus ) ;

    if (fSelected)
    {
        SetTextColor( lpDIS->hDC, GetSysColor( COLOR_WINDOWTEXT ) ) ;
        SetBkColor( lpDIS->hDC, GetSysColor( COLOR_WINDOW ) ) ;
    }

}

void CIDataObjectDlg::OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct)
{
    lpMeasureItemStruct->itemHeight = g_cyFont ;
}

void CIDataObjectDlg::OnDblClkFormatEtc()
{
}

void CIDataObjectDlg::OnSelChangeFormatEtc()
{

    if (m_MetaFile.hMF != NULL)
    {
        DeleteMetaFile( m_MetaFile.hMF ) ;
        m_MetaFile.hMF = NULL ;
    }
    InvalidateRect( m_edtAdvise, NULL, TRUE ) ;
    SetWindowText( m_edtAdvise, _T("") ) ;

    int i = ListBox_GetCurSel( m_lbFmtEtc ) ;
    if (i != LB_ERR)
    {
        LPITEMDATA lpid = (LPITEMDATA)ListBox_GetItemData( m_lbFmtEtc, i ) ;
        if (lpid && lpid->lpData)
        {
            m_fetc = *(LPFORMATETC)lpid->lpData ;
            return ;
        }
    }

#ifdef UNICODE
    m_fetc.cfFormat = CF_UNICODETEXT ;
#else
    m_fetc.cfFormat = CF_TEXT ;
#endif
    m_fetc.dwAspect = DVASPECT_CONTENT ;
    m_fetc.ptd = NULL ;
    m_fetc.tymed = TYMED_HGLOBAL ;
    m_fetc.lindex = - 1 ;
}


void CIDataObjectDlg::OnDoGetData()
{
    HRESULT     hr ;
    STGMEDIUM   stm ;
    TCHAR       szBuf[256] ;

    if (m_lpDO == NULL)
    {
        ListBox_AddString( m_lbGetData, _T( "IDataObject viewer internal error: m_lpDO == NULL" ) ) ;
        return ;
    }

    wsprintf( szBuf, _T( "Calling GetData for %s" ), (LPCTSTR)MyGetClipboardFormatName( m_fetc.cfFormat ) ) ;
    ListBox_AddString( m_lbGetData, szBuf ) ;

    hr = m_lpDO->QueryGetData( &m_fetc ) ;
    if (FAILED( hr ))
    {
        wsprintf( szBuf, _T( "QueryGetData() failed:  %s" ),
                    (LPTSTR)HRtoString( hr ) ) ;
        ListBox_AddString( m_lbGetData, szBuf ) ;
        return ;
    }

    stm.tymed = m_fetc.tymed ;
    hr = m_lpDO->GetData( &m_fetc, &stm ) ;
    if (FAILED( hr ))
    {
        wsprintf( szBuf, _T( "GetData() failed:  %s" ),
                    (LPTSTR)HRtoString( hr ) ) ;
        ListBox_AddString( m_lbGetData, szBuf ) ;
        return ;
    }

    if (FAILED( hr = GotData( &m_fetc, &stm ) ))
    {
        wsprintf( szBuf, _T( "IDataDlg::GotData() failed:  %s" ),
                    (LPTSTR)HRtoString( hr ) ) ;
        ListBox_AddString( m_lbGetData, szBuf ) ;
        return ;
    }

    // If we're warm linked, then don't let the queue overfill.  Allow only one
    // message at a time.  This is similar to the way Excel handles it's DDE Links.
    //
    m_fDoOnGetDataPosted = FALSE ;
}


HRESULT CIDataObjectDlg::GotData( LPFORMATETC lpfetc, LPSTGMEDIUM lpstm )
{
    HRESULT hr = NOERROR ;
    BOOL    fHandled = FALSE ;

#ifdef WIN32
    if ((lpfetc->cfFormat == CF_TEXT  || lpfetc->cfFormat == CF_UNICODETEXT)
            && lpstm->tymed & TYMED_HGLOBAL)
#else
    if (lpfetc->cfFormat == CF_TEXT && lpstm->tymed & TYMED_HGLOBAL)
#endif
    {
        LPTSTR lpsz = (LPTSTR)GlobalLock( lpstm->hGlobal ) ;
        if (lpsz == NULL)
        {
            ListBox_AddString( m_lbGetData, _T( "hGlobal returned by GetData is NULL!" ) ) ;
            ReleaseStgMedium( lpstm );
            return ResultFromScode( E_FAIL ) ;
        }

        if (m_fUpdateDisplay == TRUE)
              {
              #ifdef WIN32
                      // BUGFIX: 9/14/94 CEK
                      // Need to convert to ANSI or UNICODE
                      #ifdef _UNICODE
                      if (lpfetc->cfFormat == CF_TEXT)
                      {
                UINT n = lstrlen( lpsz ) * 2 ;
                WCHAR* sztemp = new WCHAR[n+1] ;
                mbstowcs( sztemp, (LPCSTR)lpsz, n ) ;
                WriteToOutput( sztemp ) ;
                delete [] sztemp ;
                      }
                      #else
            if (lpfetc->cfFormat == CF_UNICODETEXT)
            {
                UINT n = wcslen( (LPWSTR)lpsz ) ;
                char* sztemp = new char[n+1] ;
                wcstombs( sztemp, (LPWSTR)lpsz, n ) ;
                WriteToOutput( sztemp ) ;
                delete [] sztemp ;
            }
            else
                WriteToOutput( lpsz ) ;
                      #endif
              #else
            WriteToOutput( lpsz ) ;
              #endif
        }

        GlobalUnlock( lpstm->hGlobal ) ;

        fHandled = TRUE ;
    }

    if (lpfetc->cfFormat == CF_METAFILEPICT && lpstm->tymed & TYMED_MFPICT)
    {
        LPMETAFILEPICT pMF = (LPMETAFILEPICT)GlobalLock(lpstm->hGlobal) ;
        if (pMF)
        {
            if (m_MetaFile.hMF)
            {
                DeleteMetaFile( m_MetaFile.hMF ) ;
                m_MetaFile.hMF = NULL ;
            }

            if (m_fUpdateDisplay)
            {
                m_MetaFile = *pMF ;
                m_MetaFile.hMF = CopyMetaFile( pMF->hMF, NULL ) ;
                InvalidateRect( m_edtAdvise, NULL, FALSE ) ;
            }
        }
        fHandled = TRUE ;
    }

    m_cOnDataChanges++ ;

    ReleaseStgMedium( lpstm );

    if (fHandled == FALSE)
    {
        ListBox_AddString( m_lbGetData, _T( "The IDataObject viewer does not know how to decode this FORMATETC" ) ) ;
    }

    return  hr;
}

void CIDataObjectDlg::OnSetupAdvise()
{
    TCHAR szBuf[128] ;
    OnKillAdvise() ;

    m_pSink = new CImpIAdviseSink( this ) ;

    if (m_pSink == NULL)
    {
        ListBox_AddString( m_lbGetData, _T( "CImpIAdviseSink constructor failed" ) ) ;
        return ;
    }

    SetWindowText( m_btnSetupAdvise, _T( "&Kill Advise" ) ) ;

    wsprintf( szBuf, _T( "Advise started with a cfFormat of %s" ),
                (LPCTSTR)MyGetClipboardFormatName( m_fetc.cfFormat ) ) ;
    ListBox_AddString( m_lbGetData, szBuf ) ;

    m_dwTime = GetTickCount() ;
    m_cOnDataChanges = 0 ;

    HRESULT hr = m_lpDO->DAdvise( &m_fetc, m_advf, m_pSink, &m_dwConn ) ;
    if (FAILED( hr ))
    {
        wsprintf( szBuf, _T( "DAdvise() failed:  %s" ), (LPTSTR)HRtoString( hr ) ) ;
        ListBox_AddString( m_lbGetData, szBuf ) ;
        SetWindowText( m_btnSetupAdvise, _T( "&DAdvise" ) ) ;
        return ;
    }

}

void CIDataObjectDlg::OnKillAdvise()
{
    if (m_dwConn != 0)
    {
        TCHAR szBuf[128] ;
        DWORD dwTime = max( 1000, GetTickCount() - m_dwTime ) ;    // avoid div by zero

        m_lpDO->DUnadvise( m_dwConn ) ;
        m_dwConn = 0 ;
        wsprintf( szBuf, _T( "  Advise Killed after %lu milliseconds, and %lu OnDataChanges;" ), dwTime, m_cOnDataChanges ) ;
        ListBox_AddString( m_lbGetData, szBuf ) ;
        wsprintf( szBuf, _T( "  That's %lu.%lu OnDataChange calls per second." ),
                        m_cOnDataChanges / (dwTime / 1000L),
                        m_cOnDataChanges % (dwTime / 1000L) ) ;
        ListBox_AddString( m_lbGetData, szBuf ) ;
    }

    if (m_pSink != NULL)
    {
        m_pSink->Release() ;
        m_pSink = NULL ;
    }

    SetWindowText( m_btnSetupAdvise, _T( "&DAdvise" ) ) ;
}

// DoIDataObject
//
// This is where we start doing IDataObject dirty work.
// This function calls pIDataObject->EnumFormatEtc twice, once
// each for DATADIR_GET and DATADIR_SET, to get a pIEnumFORMATETC.
//
// We then call DoIEnumFormatEtc() with this pointer to actually
// perform the enumaration.
//

BOOL CIDataObjectDlg::DoIDataObject( UINT nLevel, LPDATAOBJECT pIDataObject )
{
    LPENUMFORMATETC pEFE = NULL ;
    HRESULT     hr1 ;

    hr1 = pIDataObject->EnumFormatEtc( DATADIR_GET, &pEFE ) ;
    if (SUCCEEDED( hr1 ))
    {
        if (pEFE)
        {
            DoIEnumFormatEtc( nLevel, pEFE ) ;
            pEFE->Release() ;
        }
        else
        {
            AddItem( nLevel, _T( "<<EnumFormatEtc succeeded but returned a NULL pointer!>>" ), NULL, I_LABEL ) ;
        }
    }
    else
    {
        AddItem( nLevel, _T( "<<DATA_DIRGET failed>>" ), NULL, I_LABEL ) ;
    }

    if (SUCCEEDED(hr1) && pEFE)
    {
        ListBox_SetCurSel( m_lbFmtEtc, 0 ) ;
        OnSelChangeFormatEtc() ;
    }
    return TRUE ;
}

// This member takes a pointer to an IEnumFORMATETC and enumerates
// the FORMATETC structures avaialbe, inserting them into our
// listbox.
//
BOOL CIDataObjectDlg::DoIEnumFormatEtc( UINT nLevel, LPENUMFORMATETC pIFormatEtc )
{
    LPCOLUMNSTRUCT  rgCol = new COLUMNSTRUCT[5] ;
    UINT    cxChar ;
    HDC     hdc  = GetDC( NULL) ;

    // We use the "ColumnTextOut" code in util.c to create nice
    // columns in our list box.
    //
    g_hFontBold = (HFONT)SelectObject( hdc, g_hFontBold ) ;
    cxChar = GetTextMetricsCorrectly( hdc, NULL ) ;
    rgCol[0].nLeft = 0 ;
    rgCol[0].nRight = cxChar * lstrlen( _T( "CF_METAFILEPICT_" ) ) ;
    rgCol[0].uiFlags = DT_LEFT ;

    rgCol[1].nLeft = rgCol[0].nRight + cxChar ;
    rgCol[1].nRight = rgCol[1].nLeft + cxChar * lstrlen( _T( "0000:0000_" ) ) ;
    rgCol[1].uiFlags = DT_LEFT ;

    rgCol[2].nLeft = rgCol[1].nRight + cxChar ;
    rgCol[2].nRight = rgCol[2].nLeft + cxChar * lstrlen( _T( "THUMBNAIL_" ) ) ;
    rgCol[2].uiFlags = DT_LEFT ;

    rgCol[3].nLeft = rgCol[2].nRight + cxChar ;
    rgCol[3].nRight = rgCol[3].nLeft + cxChar * lstrlen( _T( "0000:0000_" ) ) ;
    rgCol[3].uiFlags = DT_LEFT ;

    rgCol[4].nLeft = rgCol[3].nRight + cxChar ;
    rgCol[4].nRight = rgCol[4].nLeft + cxChar * lstrlen( _T( "_HGLOBAL_" ) ) ;
    rgCol[4].uiFlags = DT_LEFT ;

    g_hFontBold = (HFONT)SelectObject( hdc, g_hFontBold ) ;
    ReleaseDC( NULL, hdc ) ;

    AddItem( nLevel, _T( "cfFormat\tptd\tdwAspect\tlindex\ttymed" ), NULL, I_COLUMNHEAD, 5, rgCol ) ;
    LPFORMATETC     pfetc ;
    FORMATETC       fetc ;
    TCHAR           sz[512] ;
    ULONG           ulFE ; // number returned

    while (pIFormatEtc->Next( 1, &fetc, &ulFE ) == S_OK)
    {
        // now insert into list
        pfetc = new FORMATETC ;
        *pfetc = fetc ;
        lstrcpy( sz, MyGetClipboardFormatName( (UINT)fetc.cfFormat ) ) ;
        lstrcat( sz, _T("\t") ) ;

        TCHAR szTemp[32] ;
        wsprintf( szTemp, _T( "%04lX:%04lX\t" ), (DWORD)HIWORD( (DWORD)fetc.ptd ), (DWORD)LOWORD( (DWORD)fetc.ptd ) ) ;
        lstrcat( sz, szTemp ) ;

        switch( fetc.dwAspect )
        {
            case DVASPECT_CONTENT: lstrcat( sz, _T( "CONTENT\t" ) ) ; break ;
            case DVASPECT_THUMBNAIL: lstrcat( sz, _T( "THUMBNAIL\t" ) ) ; break ;
            case DVASPECT_ICON: lstrcat( sz, _T( "ICON\t" ) ) ; break ;
            case DVASPECT_DOCPRINT: lstrcat( sz, _T( "DOCPRINT\t" ) ) ; break ;
            default:
                wsprintf( szTemp, _T( "%08lX\t" ), (DWORD)fetc.dwAspect ) ;
                lstrcat( sz, szTemp ) ;
            break ;
        }

        wsprintf( szTemp, _T( "%04lX:%04lX\t" ), (DWORD)HIWORD( fetc.lindex ), (DWORD)LOWORD( fetc.lindex ) ) ;
        lstrcat( sz, szTemp ) ;

        switch( fetc.tymed )
        {
            case TYMED_HGLOBAL: lstrcat( sz, _T( "HGLOBAL\t" ) ) ; break ;
            case TYMED_FILE: lstrcat( sz, _T( "FILE\t" ) ) ; break ;
            case TYMED_ISTREAM: lstrcat( sz, _T( "ISTREAM\t" ) ) ; break ;
            case TYMED_ISTORAGE: lstrcat( sz, _T( "ISTORAGE\t" ) ) ; break ;
            case TYMED_GDI: lstrcat( sz, _T( "GDI\t" ) ) ; break ;
            case TYMED_MFPICT: lstrcat( sz, _T( "MFPICT\t" ) ) ; break ;
            case TYMED_NULL: lstrcat( sz, _T( "NULL\t" ) ) ; break ;
            default:
                wsprintf( szTemp, _T( "%08lX\t" ), (DWORD)fetc.tymed ) ;
                lstrcat( sz, szTemp ) ;
            break ;
        }

        AddItem( nLevel, sz,
                 pfetc, I_COLUMNENTRY, 5, rgCol ) ;
    }

    return TRUE ;
}

LPCTSTR MyGetClipboardFormatName( UINT cf )
{
    static TCHAR sz[256] ;
    switch( cf )
    {

#ifdef WIN32
        case CF_UNICODETEXT: lstrcpy( sz, _T( "CF_UNICODETEXT" ) ) ; break ;
        case CF_ENHMETAFILE: lstrcpy( sz, _T( "CF_ENHMETAFILE" ) ) ; break ;
#endif
        case CF_TEXT: lstrcpy( sz, _T( "CF_TEXT" ) ) ; break ;
        case CF_BITMAP: lstrcpy( sz, _T( "CF_BITMAP" ) ) ; break ;
        case CF_METAFILEPICT: lstrcpy( sz, _T( "CF_METAFILEPICT" ) ) ; break ;
        case CF_SYLK: lstrcpy( sz, _T( "CF_SYLK" ) ) ; break ;
        case CF_DIF: lstrcpy( sz, _T( "CF_DIF" ) ) ; break ;
        case CF_TIFF: lstrcpy( sz, _T( "CF_TIFF" ) ) ; break ;
        case CF_OEMTEXT: lstrcpy( sz, _T( "CF_OEMTEXT" ) ) ; break ;
        case CF_DIB: lstrcpy( sz, _T( "CF_DIB" ) ) ; break ;
        case CF_PALETTE: lstrcpy( sz, _T( "CF_PALETTE" ) ) ; break ;
        case CF_PENDATA: lstrcpy( sz, _T( "CF_PENDATA" ) ) ; break ;
        case CF_RIFF: lstrcpy( sz, _T( "CF_RIFF" ) ) ; break ;
        case CF_WAVE: lstrcpy( sz, _T( "CF_WAVE" ) ) ; break ;
        case 0: lstrcpy( sz, _T( "\"Wildcard Advise\"" ) ) ; break ;
        default:
            if (!GetClipboardFormatName( (UINT)cf, sz, 254 ))
                wsprintf( sz, _T( "%#08lX" ), (DWORD)cf ) ;
        break ;
    }

    return sz ;
}


UINT DBStrCpy(LPTSTR pszDst, LPCTSTR pszSrc, int far* pcLines) ;

BOOL  CIDataObjectDlg::WriteToOutput( LPTSTR lpsz )
{
    UINT cch;
    int cLines;

    cch = DBStrCpy(NULL, lpsz, &cLines);

    if (cch > (CCHOUTPUTMAX - m_cchOutput))
    {
        MessageBeep(0);
        return FALSE;
    }

    DBStrCpy(&m_szOutput[m_cchOutput], lpsz, NULL);

    m_cchOutput += cch;
    m_cLinesOutput += cLines;

    // If buffer was originally empty, wake up the main loop.
    //
    if (m_cchOutput == cch)
        PostMessage( m_hDlg, WM_OUTPUTBUFFERHASDATA, 0, 0L);

    return TRUE;
}

// This function performs a string copy (or string length if pszDst is NULL)
// It also ensures the destination string is properly formatted for the
// edit controls: line terminators must be CR followed by LF, in that orderc.
// If pcLines != NULL, returns number of lines in the string in *pcLines
//
// (pulled from the dbwin sample)
//
UINT DBStrCpy(LPTSTR pszDst, LPCTSTR pszSrc, int far* pcLines)
{
    TCHAR ch;
    UINT cch = 0;
    int cLines = 0;

    while (ch = *pszSrc++)
    {
        // If we find a CR or LF, then store a CR/LF in
        // the output string.
        //
        if (ch == 0x0d || ch == 0x0a)
        {
            cch += 2;
            cLines++;
            if (pszDst)
            {
                *pszDst++ = 0x0d;
                *pszDst++ = 0x0a;
            }

            // Skip any other CRs or LFs we find.
            //
            while ((ch = *pszSrc) && (ch == 0x0d || ch == 0x0a))
                ++pszSrc;
        }
        else
        {
            cch++;
            if (pszDst)
                *pszDst++ = ch;
        }
    }
    if (pszDst)
        *pszDst = 0;

    if (pcLines)
        *pcLines = cLines;

    return cch;
}

// Whenever something happens that causes output, it calls the WriteToOutput function
// which puts the data into a buffer (m_szOutput) and posts a message.  This is
// the message handler.
//
void CIDataObjectDlg::OnOutputBufferHasData()
{
    if (m_szOutput != NULL && m_cchOutput != 0)
    {
        int cLines = Edit_GetLineCount(m_edtAdvise) + m_cLinesOutput ;

        if (cLines > CLINESMAX)
        {
            SetWindowRedraw( m_edtAdvise, FALSE ) ;
            // If the total # of lines is greater than our arbitrary max lines
            // remove some from the top.
            //
            Edit_SetSel( m_edtAdvise, 0, Edit_LineIndex( m_edtAdvise, cLines - CLINESMAX));
            Edit_ReplaceSel( m_edtAdvise, _T(""));
        }

        // Put current output buffer at the end of the edit control
        //
        Edit_SetSel( m_edtAdvise, (UINT)-1, (UINT)-1 )  ;   // select the end
        Edit_ReplaceSel( m_edtAdvise, m_szOutput ) ;
        Edit_SetSel( m_edtAdvise, (UINT)-1, (UINT)-1 )  ;   // select the end

        if (cLines > CLINESMAX)
            SetWindowRedraw( m_edtAdvise, TRUE ) ;

        // Reset the output buffer
        m_cLinesOutput = 0 ;
        m_cchOutput = 0 ;
    }
}


// AddItem does a ListBox_AddString() plus a ListBox_SetItemData().  The
// item data that is set tells our owner-draw code what 'level' this
// item is at (indentation) and whether it is a label (bold), column
// head (column info is stored), or column entry (no column info is
// stored, but previous column head is used in WM_DRAWITEM).
//
int CIDataObjectDlg::AddItem( UINT nLevel, LPTSTR sz, LPVOID lpData, UINT uiType, int cColumns, LPCOLUMNSTRUCT  rgCol )
{
    int i ;
    LPITEMDATA      lpID ;

    i = ListBox_AddString( m_lbFmtEtc,  sz ) ;
    lpID = new ITEMDATA ;
    lpID->uiType = uiType ;
    lpID->cColumns = cColumns ;
    lpID->rgCol = rgCol ;
    lpID->nLevel = nLevel ;
    lpID->lpData = lpData ;
    ListBox_SetItemData( m_lbFmtEtc, i, (DWORD)lpID ) ;
    return i ;
}

int CIDataObjectDlg::AddItem( UINT nLevel, LPTSTR sz, LPVOID lpData, UINT uiType )
{
    return AddItem( nLevel, sz, lpData, uiType, 0, NULL ) ;
}

int CIDataObjectDlg::AddItem( UINT nLevel, LPTSTR sz, LPVOID lpData )
{
    return AddItem( nLevel, sz, lpData, I_NORMAL, 0, NULL ) ;
}

extern "C"
LRESULT EXPORT CALLBACK fnEditSubclass( HWND hwnd, UINT uiMsg, WPARAM wP, LPARAM lP )
{
    if (uiMsg == WM_PAINT)
    {
        HWND hwndParent = GetParent(hwnd) ;
        CIDataObjectDlg FAR* pIDOD =(CIDataObjectDlg FAR*)GetWindowLong( hwndParent, DWL_USER ) ;
        if (pIDOD && pIDOD->m_MetaFile.hMF != NULL)
        {
            PAINTSTRUCT ps ;
            BeginPaint( hwnd, &ps ) ;

/*
            if (pIDOD->m_MetaFile.mm == MM_ISOTROPIC || pIDOD->m_MetaFile.mm == MM_ANISOTROPIC)
            {
                if (pIDOD->m_MetaFile.xExt != 0 && pIDOD->m_MetaFile.yExt != 0)
                {

                }
            }
            else
            {
                SetMapMode( ps.hdc, MM_ANISOTROPIC ) ;
                SetWindowOrg( ps.hdc, 0, 0 ) ;
                SetWindowExt( ps.hdc, pIDOD->m_MetaFile.xExt, pIDOD->m_MetaFile.yExt ) ;
                SetViewportExt( ps.hdc, pIDOD->m_MetaFile.xExt, pIDOD->m_MetaFile.yExt ) ;
            }
*/
            PlayMetaFile( ps.hdc, pIDOD->m_MetaFile.hMF ) ;

            EndPaint( hwnd, &ps ) ;
            return 0L ;
        }
    }

    return CallWindowProc( g_pfnEdit, hwnd, uiMsg, wP, lP ) ;
}


