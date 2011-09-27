// idisp.cpp
//
// Implementation file for the IDispatch interface viewer.
//
// Copyright (c) 1993-1995 Microsoft Corporation, All Rights Reserved.
//
// Charlie Kindel, Program Manager
// Microsoft Vertical Developer Relations
// August 7, 1993
//
// Internet   :  ckindel@microsoft.com
// CompuServe :  >INTERNET:ckindel@microsoft.com
//
// Revisions:
//  August 7, 1993  cek     First implementation.
//
//
// This module contains the Interface Viewer functions for the
// IDispatch, ITypeInfo, and ITypeLib interfaces.
// This implementation displays all of the
// type information that can be retrieved from an arbitrary IDispatch,
// ITypeInfo, or ITypeLib pointer.
//
// NOTE:  This module exports the following interface viewers:
//
//      DisplayIDispatch()
//      DisplayITypeInfo()
//      DisplayITypeLib()
//
// Each of these functions boils down to the same dialog and code.
//
// Note that attempting to 'look at' an arbitrary ITypeInfo pointer
// which was received from QueryInterface() is a somewhat bogus thing
// to do.   We provide DisplayITypeInfo here only for completeness.
//
//
// ANOTHER NOTE:
// Since this viewer can take an ITypeLib, it makes a great Type Library
// viewer.  That is why it is used by Ole2View for the File.View Type Libary
// command...
//

#include "precomp.h"
#include "defo2v.h"

#include "idisp.h"
#include "tofile.h"

#if defined(WIN32) && !defined(_UNICODE)
    #pragma message("Building ANSI version of " __FILE__)
#endif

static HFONT        g_hFont ;
static UINT         g_cyFont ;

// These functions take a hwnd of a listbox, an IDispatch related structure,
// and string and dump the contents of the structure to the listbox.
// All structure members are prefixed by szLabel in the listbox.
//
void DumpTYPEDESC( HWND hwnd, TYPEDESC FAR* lptdesc, LPTSTR szLabel ) ;
void DumpARRAYDESC( HWND hwnd, ARRAYDESC FAR* lpadesc, LPTSTR szLabel ) ;
void DumpIDLDESC( HWND hwnd, LPIDLDESC lp, LPTSTR szLabel ) ;
void DumpELEMDESC( HWND hwnd, LPELEMDESC lp, LPTSTR szLabel ) ;
void DumpFUNCDESC( HWND hwnd, LPFUNCDESC lpfndesc, LPTSTR szLabel ) ;
void DumpVARDESC( HWND hwnd, LPVARDESC lpvardesc, LPTSTR szLabel ) ;

static TCHAR * g_rgszTYPEKIND[] =
{
    _T("enum"),         /* TKIND_ENUM */
    _T("struct"),       /* TKIND_RECORD */
    _T("module"),       /* TKIND_MODULE */
    _T("interface"),    /* TKIND_INTERFACE */
    _T("dispinterface"),    /* TKIND_DISPATCH */
    _T("coclass"),      /* TKIND_COCLASS */
    _T("alias"),        /* TKIND_ALIAS */

    /* NOTE: the following aren't supported in typeinfo yet */
    _T("union"),        /* TKIND_UNION */
    _T("encap union"),   /* TKIND_ENCUNION */
    _T("class"),        // TKIND_Class
    _T("cotype"),       // TKIND_COOTYPE
#if WIN32==300
    _T("unused"),
    _T("propset"),
    _T("eventset"),
    _T("cmdset"),
#endif
    _T("<error!>"),     // dummy entries just in case the ole spec changes
    _T("<error!>"),     // and new enum's are added!
    _T("<error!>"),
    _T("<error!>"),
    _T("<error!>"),
};

LPTSTR TYPEKINDtoString( TYPEKIND tk )
{
    return (LPTSTR)g_rgszTYPEKIND[tk] ;
}

static TCHAR * g_rgszIDLFLAGS[] =
{
    _T(""),         //IDLFLAG_NONE = 0,
    _T("IN"),       //IDLFLAG_FIN = 1,
    _T("OUT"),      //IDLFLAG_FOUT = 2,
    _T("IN OUT"),   //IDLFLAF_FIN | IDLFLAG_FOUT
    _T("<error!>"),     // dummy entries just in case the ole spec changes
    _T("<error!>"),     // and new enum's are added!
    _T("<error!>"),
    _T("<error!>"),
    _T("<error!>"),
};

static TCHAR * g_rgszFUNCKIND[] =
{
    _T("VIRUTAL"),
    _T("PUREVIRTUAL"),
    _T("NONVIRTUAL"),
    _T("STATIC"),
    _T("DISPATCH"),
    _T("<error!>"),     // dummy entries just in case the ole spec changes
    _T("<error!>"),     // and new enum's are added!
    _T("<error!>"),
    _T("<error!>"),
    _T("<error!>"),
} ;

static TCHAR * g_rgszCALLCONV[] =
{
    _T("0"),
    _T("CDECL"),        // CC_CDECL, = 1,
    _T("PASCAL"),       // CC_MSCPASCAL and CC_PASCAL
    _T("MACPASCAL"),     // CC_MACPASCAL,
    _T("STDCALL"),       // CC_STDCALL,
    _T("THISCALL"),      // CC_THISCALL,
    _T("MAX"),           // end of enum marker
    _T("<error!>"),     // dummy entries just in case the ole spec changes
    _T("<error!>"),     // and new enum's are added!
    _T("<error!>"),
    _T("<error!>"),
    _T("<error!>"),
};

static TCHAR * g_rgszVARKIND[] =
{
    _T("PERINSTANCE"),  //VAR_PERINSTANCE,
    _T("STATIC"),       //VAR_STATIC
    _T("CONST"),        //VAR_CONST
    _T("DISPATCH"),      //VAR_DISPATCH
    _T("<error!>"),     // dummy entries just in case the ole spec changes
    _T("<error!>"),     // and new enum's are added!
    _T("<error!>"),
    _T("<error!>"),
    _T("<error!>"),
} ;

//ASSERTDATA
// DisplayIDispatch
//
// This
STDAPI DisplayIDispatch( HWND hwndParent, LPDISPATCH lpDisp, LPIID lpiid, LPTSTR lpszName )
{
    HRESULT hr = NULL ;

//    Assert( hwndParent ) ;
//    Assert( lpDisp ) ;
//    Assert( lpiid ) ;

    IDispDlg    dispdlg( hwndParent, (LPUNKNOWN)lpDisp, lpiid, lpszName ) ;

    dispdlg.DoModal() ;

    return hr ;
}


// DisplayITypeInfo
//
// This
STDAPI DisplayITypeInfo( HWND hwndParent, LPTYPEINFO lpTypeInfo, LPIID lpiid, LPTSTR lpszName )
{
    HRESULT hr = NULL ;

//    Assert( hwndParent ) ;
//    Assert( lpTypeInfo ) ;
//    Assert( lpiid ) ;

    IDispDlg    dispdlg( hwndParent, (LPUNKNOWN)lpTypeInfo,lpiid, lpszName ) ;
    dispdlg.DoModal() ;

    return hr ;
}

STDAPI DisplayITypeLib( HWND hwndParent, LPTYPELIB lpTypeLib, LPIID lpiid, LPTSTR lpszName )
{
    HRESULT hr = NULL ;
    int     rc;

//    Assert( hwndParent ) ;
//    Assert( lpTypeLib ) ;
//    Assert( lpiid ) ;

    IDispDlg    dispdlg( hwndParent, (LPUNKNOWN)lpTypeLib, lpiid, lpszName ) ;
    rc = dispdlg.DoModal() ;

    return hr ;
}


/////////////////////////////////////////////////////////////////////////////
// IDispDlg dialog
//
// The constructor is where we begin to descern between the three possible
// interfaces that could be passed in.
//
IDispDlg::IDispDlg(HWND hwnd, LPUNKNOWN lpUnk, LPIID lpiid, LPTSTR lpszName )
{
    m_hWndParent = hwnd ;
    m_piid = lpiid ;
    m_pszName = lpszName ;

    m_pDispatch = NULL ;
    m_pTypeInfo = NULL ;
    m_pTypeLib = NULL ;
    m_fImplType = FALSE;

    if (IsEqualIID( *m_piid, IID_IDispatch ))
    {
        m_pDispatch = (LPDISPATCH)lpUnk ;
        m_pDispatch->AddRef() ;
    }
#ifdef OB32BUG
    else if (IsEqualIID( *m_piid, IID_IMyTypeInfo ))
#else
    else if (IsEqualIID( *m_piid, IID_ITypeInfo ))
#endif

    {
        m_pTypeInfo = (LPTYPEINFO)lpUnk ;
        m_pTypeInfo->AddRef() ;
    }
#ifdef OB32BUG
    else if (IsEqualIID( *m_piid, IID_IMyTypeLib ))
#else
    else if (IsEqualIID( *m_piid, IID_ITypeLib ))
#endif

    {
        m_pTypeLib = (LPTYPELIB)lpUnk ;
        m_pTypeLib->AddRef() ;
    }
    else
    {
//        AssertSz(0, _T("Some Thing is wrong\n") ) ;
    }
}

IDispDlg::~IDispDlg( )
{
}

int IDispDlg::DoModal( void )
{
    return DialogBoxParam( g_hInst, MAKEINTRESOURCE( IDD_IDISPDLG ), m_hWndParent, (DLGPROC)fnIDispDlg, (LONG)this ) ;
}

extern "C"
BOOL EXPORT WINAPI fnIDispDlg( HWND hDlg, UINT uiMsg, WPARAM wParam, LPARAM lParam )
{
    IDispDlg FAR* pIDD =(IDispDlg FAR*)GetWindowLong( hDlg, DWL_USER ) ;

    switch (uiMsg)
    {
        case WM_INITDIALOG:
            pIDD=(IDispDlg FAR*)lParam ;
            if (pIDD==NULL)
            {
                EndDialog( hDlg, 0 ) ;
                return TRUE ;
            }
            pIDD->m_hDlg = hDlg ;
            SetWindowLong( hDlg, DWL_USER, (LONG)pIDD ) ;
            pIDD->OnInitDialog() ;
            PostMessage( hDlg, WM_USER+1, 0, 0 ) ;
        break ;

        // It's nice to let the dialog appear before we
        // do a lengthy operation.
        case WM_USER+1:
        {
            HCURSOR hcur = SetCursor( LoadCursor( NULL, IDC_WAIT ) ) ;
            pIDD->DoGetInfo() ;
            SetCursor( hcur ) ;
        }
        break ;

        case WM_DESTROY:
            if (pIDD)
                pIDD->OnDestroy() ;
        break ;

        case WM_SIZE:
            if (pIDD)
                pIDD->OnSize( (UINT)wParam, LOWORD( lParam ), HIWORD( lParam ) ) ;
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

                case IDC_TOFILE:
                    pIDD->OnToFile() ;
                break ;

                case IDC_TYPEINFO:
                    if (wNotifyCode == LBN_SELCHANGE)
                        pIDD->OnSelChangeTypeInfo() ;
                break ;

                case IDC_TYPEATTR:
                break ;

                case IDC_FUNCTIONS:
                    switch(wNotifyCode)
                    {
                        case LBN_SELCHANGE:
                            pIDD->OnSelChangeFunctions() ;
                        break;
                    }
                break ;

                case IDC_VARIABLES:
                    switch(wNotifyCode)
                    {
                        case LBN_SELCHANGE:
                            pIDD->OnSelChangeVariables() ;
                        break;
                    }
                break ;
            }
        }
        break ;

        default:
           return FALSE ;
    }
    return TRUE ;
}

void IDispDlg::SetImplFlag( BOOL fVal )
{
    m_fImplType= fVal;
}

BOOL IDispDlg::GetImplFlag( )
{
    return m_fImplType;
}

BOOL IDispDlg::OnInitDialog()
{
    // Setup member vars, so we have easy access to
    // dialog items.
    //
    m_hwndTypeInfoCount = GetDlgItem( m_hDlg, IDC_TYPEINFOCOUNT ) ;
    m_hwndTypeInfo = GetDlgItem( m_hDlg, IDC_TYPEINFO ) ;
    m_hwndTypeAttr = GetDlgItem( m_hDlg, IDC_TYPEATTR ) ;
    m_hwndFunctionsLbl = GetDlgItem( m_hDlg, IDC_FUNCTIONS_LBL ) ;
    m_hwndFunctions = GetDlgItem( m_hDlg, IDC_FUNCTIONS ) ;
    m_hwndVariablesLbl = GetDlgItem( m_hDlg, IDC_VARIABLES_LBL ) ;
    m_hwndVariables = GetDlgItem( m_hDlg, IDC_VARIABLES ) ;
    m_hwndFuncProtoLbl = GetDlgItem( m_hDlg, IDC_FUNCPROTO_LBL ) ;
    m_hwndFuncProto = GetDlgItem( m_hDlg, IDC_FUNCPROTO ) ;
    m_hwndInfo = GetDlgItem( m_hDlg, IDC_FUNCVARDESC ) ;
    m_hwndInfoLbl = GetDlgItem( m_hDlg, IDC_FUNCVARDESC_LBL ) ;
    m_hwndTypeInfoInfo = GetDlgItem( m_hDlg, IDC_TYPEINFOINFO ) ;
    m_btnToFile = GetDlgItem( m_hDlg, IDC_TOFILE ) ;

    // Create fonts we may use..
    //
    TEXTMETRIC  tm ;
    HDC hdc = GetDC(NULL);
    g_hFont = ReallyCreateFont( hdc, _T("MS Sans Serif"), _T("Regular"), 8, 0 ) ;
    g_hFont = (HFONT) SelectObject( hdc, g_hFont ) ;
    GetTextMetrics( hdc, &tm ) ;
    g_hFont = (HFONT) SelectObject( hdc, g_hFont ) ;
    ReleaseDC( NULL, hdc ) ;
    g_cyFont = tm.tmHeight + tm.tmExternalLeading ;

    SetWindowFont( m_hwndTypeInfoCount, g_hFont, TRUE ) ;
    SetWindowFont( m_hwndVariables, g_hFont, TRUE ) ;
    SetWindowFont( m_hwndTypeAttr, g_hFont, TRUE ) ;
    SetWindowFont( m_hwndInfo, g_hFont, TRUE ) ;
    SetWindowFont( m_hwndTypeInfoInfo, g_hFont, TRUE ) ;
    SetWindowFont( m_hwndFunctions, g_hFont, TRUE ) ;
    SetWindowFont( m_hwndFuncProto, g_hFont, TRUE ) ;

    DlgCenter( m_hDlg, m_hWndParent, FALSE ) ;
    RECT rc ;
    GetWindowRect( m_hDlg, &rc ) ;
    SetWindowPos( m_hDlg, NULL, rc.left, rc.top, rc.right - rc.left + 1,
                  rc.bottom - rc.top +1, SWP_NOMOVE|SWP_NOZORDER | SWP_NOACTIVATE ) ;

    SetWindowText( m_hDlg, m_pszName ) ;

    return TRUE;  // return TRUE  unless you set the focus to a control
}

void IDispDlg::OnDestroy()
{
    if (g_hFont)
        DeleteObject( g_hFont );

    if (m_pDispatch != NULL)
    {
        m_pDispatch->Release() ;
        m_pDispatch = NULL ;
    }

    if (m_pTypeInfo != NULL)
    {
        m_pTypeInfo->Release() ;
        m_pTypeInfo = NULL ;
    }

    if (m_pTypeLib != NULL)
    {
        m_pTypeLib->Release() ;
        m_pTypeLib = NULL ;
    }
}


void IDispDlg::OnSize(UINT nType, int cx, int cy)
{
//    if (m_hwndTypeAttr && !IsWindow( m_hwndTypeAttr ) )
        return ;
#if 0

    RECT    rc ;
    RECT    rcWnd ;
    GetClientRect( m_hDlg, &rcWnd ) ;
    GetWindowRect( m_hwndTypeAttr, &rc ) ;
    MapWindowPoints( NULL, m_hDlg, (POINT FAR*)&rc, 2 ) ;
    rc.left = rcWnd.left + 5 ;
    SetWindowPos( m_hwndTypeAttr, NULL, rcWnd.left + 5, rc.top, cx - 10, rc.bottom - rc.top,
                SWP_NOZORDER | SWP_NOACTIVATE ) ;
#endif
}

void IDispDlg::OnToFile()
{
    if (m_pTypeInfo == NULL && m_pTypeLib == NULL)
    {
        MessageBeep( 0 ) ;
        return ;
    }

    if (m_pTypeLib)
        DoTypeLibToFile( m_hDlg, m_pTypeLib ) ;
    else
        DoTypeInfoToFile( m_hDlg, m_pTypeInfo ) ;
}

// This starts our gathering of information.
// Here, we figure out which interface we were called for, get as
// much info as we can from it, including the other two
// interfaces if possible.   We fill the typeinfo listbox
// with all type infos found.
//
void IDispDlg::DoGetInfo()
{

    TCHAR        szTemp[128] ;
    UINT        uiTypeInfoCount = 0;
    HRESULT hr ;
    hr = ResultFromScode( E_FAIL ) ;

    ComboBox_ResetContent( m_hwndTypeInfo ) ;

    if (m_pDispatch != NULL)
    {
        // If we were called for IDispatch we need to get at least the typelib.
        //
        // This whole thing is done somewhat roundabout:
        //
        // 1) Call GetTypeInfo to get the typeinfo for the programmability interface.
        // 2) Try to get the "containing TypeLib" for the typeinfo.
        // 3a) If we get m_pTypeLib back then this object was implemented using a Type Library
        //     and will probably have more than one typeinfo.
        // 3b) If the call to GetContainingTypeLib fails then this object does not
        //     use typelibs and thus probably has only one typeinfo.
        // 4) In either case we eventually get a count of typeinfos and we
        //    use either IDispatch::GetTypeInfo or ITypeLib::GetTypeInfo to get the
        //    type information for the object.
        //
        hr = m_pDispatch->GetTypeInfoCount( &uiTypeInfoCount ) ;
        if (FAILED( hr ))
        {
            wsprintf( szTemp, _T("IDispatch::GetTypeInfoCount( ) FAILED: %s"), HRtoString( hr ) ) ;
            MessageBox( m_hDlg, szTemp, _T("Could not get type information"), MB_OK ) ;
            return ;
        }

        if (uiTypeInfoCount <= 0)
        {
            wsprintf( szTemp, _T("This object reports zero TypeInfos.  There is no programmability information available.") ) ;
            MessageBox( m_hDlg, szTemp, _T("Could not get type information"), MB_OK ) ;
            return ;
        }

        hr = m_pDispatch->GetTypeInfo( 0, LOCALE_SYSTEM_DEFAULT, &m_pTypeInfo ) ;
        if (FAILED( hr ))
        {
            wsprintf( szTemp, _T("IDispatch::GetTypeInfo( 0 ) FAILED: %s"), HRtoString( hr ) ) ;
            MessageBox( m_hDlg, szTemp, _T("Could not get type information"), MB_OK ) ;
            return ;
        }

        UINT    ui ;
        hr = m_pTypeInfo->GetContainingTypeLib( &m_pTypeLib, &ui ) ;
        if (SUCCEEDED( hr ))
        {
            // We no longer need the typeinfo ptr
            //
            m_pTypeInfo->Release() ;
            m_pTypeInfo = NULL ;
            uiTypeInfoCount = m_pTypeLib->GetTypeInfoCount( ) ;
            SetWindowText( m_hwndTypeInfoInfo, _T("This object uses a Type Library.") ) ;
        }
        else
        {
            // We no longer need the typeinfo ptr.
            //
            m_pTypeInfo->Release() ;
            m_pTypeInfo = NULL ;

            // Bogus.  The app does not support TypeLib (i.e. WinWord 6.0).   Therefore we must
            // use IDispatch::GetTypeInfoCount...
            //
            hr = m_pDispatch->GetTypeInfoCount( &uiTypeInfoCount ) ;
            if (FAILED( hr ))
            {
                wsprintf( szTemp, _T("IDispatch::GetTypeInfoCount FAILED: %s"), HRtoString( hr ) ) ;
                MessageBox(  m_hDlg, szTemp, _T("Could not get count of typeinfos"), MB_OK ) ;
                return ;
            }

            SetWindowText( m_hwndTypeInfoInfo, _T("This object does not use a Type Library.") ) ;
        }
    }
    else if (m_pTypeLib != NULL)
    {
        // If we were called with a typelib we are in great shape
        //
        uiTypeInfoCount = m_pTypeLib->GetTypeInfoCount() ;
        SetWindowText( m_hwndTypeInfoInfo, _T("Browsing a TypeLibrary." )) ;
    }
    else if (m_pTypeInfo != NULL)
    {
        // We were called with at TypeInfo.  This is the specialist of cases.
        UINT    ui ;
        hr = m_pTypeInfo->GetContainingTypeLib( &m_pTypeLib, &ui ) ;
        if (SUCCEEDED( hr ))
        {
            // We no longer need the typeinfo ptr
            //
            m_pTypeInfo->Release() ;
            m_pTypeInfo = NULL ;
            uiTypeInfoCount = m_pTypeLib->GetTypeInfoCount( ) ;
            SetWindowText( m_hwndTypeInfoInfo, _T("This object uses a Type Library.") ) ;
        }
        else
        {
            // If we get here then we assume that there is only one
            // typeinfo and we're looking at it.
            //
            uiTypeInfoCount = 1 ;
            SetWindowText( m_hwndTypeInfoInfo, _T("Browsing a TypeInfo") ) ;
        }
    }

    wsprintf( szTemp, _T("%u"), uiTypeInfoCount ) ;
    SetWindowText( m_hwndTypeInfoCount, szTemp ) ;

    // Fill combo box with all typeinfos
    //
    //
    for (UINT n = 1 ; n <= uiTypeInfoCount ; n++)
    {
        if (m_pTypeLib != NULL || m_pDispatch != NULL)
        {
            if (m_pTypeInfo != NULL)
            {
                m_pTypeInfo->Release() ;
                m_pTypeInfo = NULL ;
            }

            if (m_pTypeLib != NULL)
            {
                hr = m_pTypeLib->GetTypeInfo( n-1, &m_pTypeInfo ) ;
            }
            else if (m_pDispatch != NULL)
                hr = m_pDispatch->GetTypeInfo( n-1, LOCALE_SYSTEM_DEFAULT, &m_pTypeInfo ) ;
        }
        else if (m_pTypeInfo != NULL)
            hr = ResultFromScode( S_OK ) ;
        else
            hr = ResultFromScode( E_FAIL ) ;

        if (SUCCEEDED(hr))
        {
            // Note:  Early OLE2 samples (IDispatch Calculator #2)
            // would crash on a GetDocumentation( MEMBERID_NIL... ) call
            // it is commented out for now until we are sure that
            // it is working in OLE 2.01.
            //
#if 1
            BSTR    bstrName ;
            hr = m_pTypeInfo->GetDocumentation( MEMBERID_NIL, &bstrName, NULL, NULL, NULL ) ;
            if (SUCCEEDED(hr))
            {

                UINT ui ;
                wsprintf( szTemp, _T("%ws (%u)"), bstrName ? bstrName : OLESTR("(null name)"), n-1 ) ;
                ui = ComboBox_AddString( m_hwndTypeInfo, szTemp ) ;
                ComboBox_SetItemData( m_hwndTypeInfo, ui, (DWORD)n-1 ) ;
                SysFreeString( bstrName ) ;
            }
            else
#else
            BSTR            rgbstrNames[2] ;

            UINT            cNames ;
            hr = m_pTypeInfo->GetNames( MEMBERID_NIL, rgbstrNames, 1, &cNames );
            if (SUCCEEDED( hr ))
            {
                UINT ui ;
                wsprintf( szTemp, "%ws (%u)", rgbstrNames[0], n ) ;
                ui = ComboBox_AddString( m_hwndTypeInfo, szTemp ) ;
                ComboBox_SetItemData( m_hwndTypeInfo, ui, (DWORD)n ) ;
                SysFreeString( rgbstrNames[0] ) ;
            }
            else
#endif
            {
                UINT ui ;
                wsprintf( szTemp, _T("%u (no name)"), n-1 ) ;
                ui = ComboBox_AddString( m_hwndTypeInfo, szTemp ) ;
                ComboBox_SetItemData( m_hwndTypeInfo, ui, (DWORD)n-1 ) ;
            }
        }
        else
        {
            wsprintf(szTemp, _T("%s::GetTypeInfo(#%u) FAILED: %s"),
                        (LPTSTR)(m_pTypeLib == NULL ? _T("IDispatch") :
                            _T("ITypeLib") ), n-1, (LPTSTR)HRtoString( hr ) ) ;
            MessageBox( m_hDlg, szTemp, _T("Error"), MB_OK ) ;
        }

    }

    ComboBox_SetCurSel( m_hwndTypeInfo, 0 ) ;

    // kick start
    OnSelChangeTypeInfo() ;

    return ;
}

// When the selection changes in the combo, we first make sure
// our current TypeInfo pointer is Released.
// Then get the pTypeInfo for the newly selected item.
// Use that pointer to populate the typeinfo listbox (actually
// m_pTypeAttr).
//
void IDispDlg::OnSelChangeTypeInfo()
{
    HRESULT     hr ;
    UINT        uiTinfo = 0;
    int         uiItem = ComboBox_GetCurSel( m_hwndTypeInfo );
    TCHAR        szTemp[128] ;

    if (uiItem == CB_ERR)
        return ;

    ListBox_ResetContent( m_hwndTypeAttr ) ;
    ListBox_ResetContent( m_hwndFunctions ) ;
    ListBox_ResetContent( m_hwndVariables ) ;
    ListBox_ResetContent( m_hwndInfo ) ;
    SetWindowText( m_hwndFuncProto, _T("") ) ;
    SetWindowText( m_hwndInfoLbl, _T("F&UNCDESC/VARDESC:") ) ;

    hr = ResultFromScode( E_FAIL ) ;
    if ((m_pDispatch != NULL || m_pTypeLib != NULL) && m_pTypeInfo != NULL)
    {
        m_pTypeInfo->Release() ;
        m_pTypeInfo = NULL ;
        // Get memid from item
        uiTinfo = (UINT)ComboBox_GetItemData( m_hwndTypeInfo, uiItem ) ;

        // Get typeinfo pointer
        if (m_pTypeLib != NULL)
            hr = m_pTypeLib->GetTypeInfo( uiTinfo, &m_pTypeInfo ) ;
        else
            hr = m_pDispatch->GetTypeInfo( uiTinfo, LOCALE_SYSTEM_DEFAULT, &m_pTypeInfo ) ;
    }
    else if (m_pTypeInfo != NULL)
        hr = ResultFromScode( S_OK ) ;

    if (SUCCEEDED(hr))
    {

        DWORD   dwHelp ;
        BSTR    bstrName, bstrDoc, bstrHelpFile ;
        hr = m_pTypeInfo->GetDocumentation( MEMBERID_NIL, &bstrName, &bstrDoc,  &dwHelp, &bstrHelpFile ) ;
        if (SUCCEEDED(hr))
        {
            wsprintf( szTemp, _T("Name = %ws"), (bstrName == NULL ? OLESTR("n/a") : bstrName) ) ;
            ListBox_AddString( m_hwndTypeAttr, szTemp ) ;


            wsprintf( szTemp, _T("DocString = %ws"), (bstrDoc == NULL ? OLESTR("n/a") : bstrDoc) ) ;
            ListBox_AddString( m_hwndTypeAttr, szTemp ) ;

            wsprintf( szTemp, _T("Help Context ID = %#08lX"), (DWORD)dwHelp ) ;
            ListBox_AddString( m_hwndTypeAttr, szTemp ) ;
            wsprintf( szTemp, _T("Help File = %ws"), (bstrHelpFile == NULL ? OLESTR("n/a") : bstrHelpFile) ) ;
            ListBox_AddString( m_hwndTypeAttr, szTemp ) ;
            SysFreeString( bstrName ) ;
            SysFreeString( bstrDoc) ;
            SysFreeString( bstrHelpFile ) ;
        }
        else
        {
            wsprintf( szTemp, _T("GetDocumentation FAILED: %s"), HRtoString( hr ) ) ;
            ListBox_AddString( m_hwndTypeAttr, szTemp ) ;
        }

        // Get the containing typelib for this typeinfo.
        //
        UINT ui ;
        LPTYPELIB   pTL ;
        hr = m_pTypeInfo->GetContainingTypeLib( &pTL, &ui ) ;
        if (SUCCEEDED( hr ))
        {
            wsprintf( szTemp, _T("GetContainingTypeLib returned valid pTypelib (%#08lX, %u)"),
                        (LONG)pTL, ui ) ;
            pTL->Release() ;
        }
        else
            wsprintf( szTemp, _T("GetContainingTypeLib FAILED:  %s"), HRtoString( hr ) ) ;
        ListBox_AddString( m_hwndTypeAttr, szTemp ) ;

        // Call DoTypeAttr to get the TypeAttr info and to
        // fill the functions and variables listboxes.
        //
        DoTypeAttr( m_pTypeInfo ) ;

        // NEED: What other information can we put in this listbox?
        // what else is available, given ITypeInfo  pointer?
        //
        //
    }
    else
    {
        wsprintf( szTemp, _T("GetTypeInfo(#%u) FAILED: %s"), uiTinfo, HRtoString( hr ) ) ;
        ListBox_AddString( m_hwndTypeAttr, szTemp ) ;
    }
}

BOOL IDispDlg::DoTypeAttr( LPTYPEINFO  pTI )
{
    HRESULT hr ;
    TCHAR    szTemp[64] ;
    LPTYPEATTR  pTA = NULL ;

    hr = pTI->GetTypeAttr( &pTA ) ;
    if (FAILED( hr ))
    {
        wsprintf( szTemp, _T("GetTypeAttr FAILED: %s"), HRtoString( hr ) ) ;
        ListBox_AddString( m_hwndTypeAttr, szTemp ) ;
        return FALSE ;
    }


    if (pTA->typekind > TKIND_MAX)
        wsprintf( szTemp, _T("typekind = <invalid!> (%u)"), pTA->typekind ) ;
    else
        wsprintf( szTemp, _T("typekind = %s"), (LPSTR)g_rgszTYPEKIND[pTA->typekind] ) ;

    ListBox_AddString( m_hwndTypeAttr, szTemp ) ;
    wsprintf( szTemp, _T("Version =  %u.%03d"), pTA->wMajorVerNum, pTA->wMajorVerNum ) ;
    ListBox_AddString( m_hwndTypeAttr, szTemp ) ;
    wsprintf( szTemp, _T("lcid = %u"),  pTA->lcid ) ;
    ListBox_AddString( m_hwndTypeAttr, szTemp ) ;
    wsprintf( szTemp, _T("cFuncs = %d"), pTA->cFuncs ) ;
    ListBox_AddString( m_hwndTypeAttr, szTemp ) ;
    wsprintf( szTemp, _T("cVars = %u"), pTA->cVars ) ;
    ListBox_AddString( m_hwndTypeAttr, szTemp ) ;
    wsprintf( szTemp, _T("cImplTypes = %u"), pTA->cImplTypes ) ;
    ListBox_AddString( m_hwndTypeAttr, szTemp ) ;

    // DumpTYPEDESC will completely decode the tdescAlias for us...
    //
    DumpTYPEDESC( m_hwndTypeAttr, &pTA->tdescAlias, _T("tdescAlias." )) ;

    wsprintf(szTemp, _T("guid = {%08lX-%04X-%04X-%02X%02X-%02X%02X%02X%02X%02X%02X}"),
        pTA->guid.Data1, pTA->guid.Data2, pTA->guid.Data3,
        pTA->guid.Data4[0], pTA->guid.Data4[1],
        pTA->guid.Data4[2], pTA->guid.Data4[3],
        pTA->guid.Data4[4], pTA->guid.Data4[5],
        pTA->guid.Data4[6], pTA->guid.Data4[7]);

    ListBox_AddString( m_hwndTypeAttr, szTemp ) ;

    wsprintf(szTemp, _T("wTypeFlags = %04X"), pTA->wTypeFlags ) ;
    ListBox_AddString( m_hwndTypeAttr, szTemp ) ;

    wsprintf(szTemp, _T("dwReserved = %#08lX"), pTA->dwReserved ) ;
    ListBox_AddString( m_hwndTypeAttr, szTemp ) ;

    wsprintf(szTemp, _T("cbAlignment = %u"), pTA->cbAlignment ) ;
    ListBox_AddString( m_hwndTypeAttr, szTemp ) ;

    wsprintf(szTemp, _T("cbSizeInstance = %u"), pTA->cbSizeInstance ) ;
    ListBox_AddString( m_hwndTypeAttr, szTemp ) ;

    wsprintf(szTemp, _T("cbSizeVft = %u"), pTA->cbSizeVft ) ;
    ListBox_AddString( m_hwndTypeAttr, szTemp ) ;

    DumpIDLDESC( m_hwndTypeAttr, &pTA->idldescType, _T("idldescType.") ) ;

    wsprintf(szTemp, _T("memidConstructor = %#08lX"), (DWORD)pTA->memidConstructor ) ;
    ListBox_AddString( m_hwndTypeAttr, szTemp ) ;

    wsprintf(szTemp, _T("memidDestructor = %#08lX"), (DWORD)pTA->memidDestructor ) ;
    ListBox_AddString( m_hwndTypeAttr, szTemp ) ;

    // Get functions
    DoGetFunctions( pTI, pTA->cFuncs ) ;

    // Get variables
    DoGetVars( pTI, pTA->cVars ) ;
#if WIN32==300
    // GetImpl Types
    if ( pTA->typekind == TKIND_COTYPE)
    {
        DoGetImplTypes( pTI, pTA->cImplTypes );
    }
#endif

    if (pTI != NULL)
        pTI->ReleaseTypeAttr( pTA ) ;
    return TRUE ;
}



// Fill the function listbox with a list of all functions.
// The entries in the listbox show just the name of the function and
// when the selection changes, the static box below the list is
// updated to show the return value and parameters (OnFunctionsSelChange()).
//
void IDispDlg::DoGetFunctions( LPTYPEINFO lpTI, WORD cFuncs )
{
    TCHAR    szBuf[128] ;
    HRESULT hr ;
    TCHAR            szTemp[80] ;
    BSTR           rgbstrNames[2] ;

    UINT            cNames ;
    LPFUNCDESC      pFuncDesc ;
    WORD iIndex ;

    ListBox_ResetContent( m_hwndFunctions ) ;

    for ( iIndex = 0 ; iIndex < cFuncs ; iIndex++)
    {
        hr = lpTI->GetFuncDesc( iIndex, &pFuncDesc ) ;
        if (FAILED(hr))
        {
            wsprintf( szBuf, _T("GetFuncDesc FAILED for function #%u (%s)"), iIndex, (LPTSTR)HRtoString( hr ) ) ;
            ListBox_AddString( m_hwndFunctions, szBuf ) ;
            continue ;
        }

        // We could get the function name from either GetDocumentation or from
        // GetNames...we do it from GetNames...
        // (I had some problems getting GetDocumentation() to work with some
        // of the automation examples lying around)
        //

        // We AddString each function name into the list box, then
        // SetItemData 'i' so that we know the memberid of the function
        // for OnFunctionSelChange()
        //
        hr = lpTI->GetNames( pFuncDesc->memid, rgbstrNames, 1, &cNames );
        if (SUCCEEDED( hr ))
        {
            // rgbstrNames[0] is the name of the function
#if defined(WIN32) && !defined(UNICODE)
            TCHAR szTemp[128];
            wcstombs(szTemp, rgbstrNames[0], 128);
            ListBox_SetItemData( m_hwndFunctions,
                             ListBox_AddString( m_hwndFunctions, szTemp ),
                             (LONG)iIndex ) ;
#else
            ListBox_SetItemData( m_hwndFunctions,
                             ListBox_AddString( m_hwndFunctions, rgbstrNames[0] ),
                             (LONG)iIndex ) ;
#endif
            SysFreeString( rgbstrNames[0] ) ;
        }
        else
        {
            wsprintf( szTemp, _T("GetNames (%lu) FAILED: %s"), pFuncDesc->memid, HRtoString( hr ) ) ;
            ListBox_AddString( m_hwndFunctions, szTemp ) ;
        }

        lpTI->ReleaseFuncDesc( pFuncDesc ) ;
    }

    ListBox_SetCurSel( m_hwndFunctions, 0 ) ;
    OnSelChangeFunctions() ;
}

// Set the text of m_hwndFuncProto so that it looks like this:
//
//    RetType FuncName ( Type Param1, Type Param2, ... )
//
// where RetType is a string associated with a VARTYPE (VTtoString())
// FuncName is bstrName.
void IDispDlg::OnSelChangeFunctions()
{
    int     iIndex = ListBox_GetCurSel( m_hwndFunctions ) ;
    #define MAX_NAMES   100
    BSTR    rgbstrNames[MAX_NAMES] ;
    UINT    cNames ;

    if (iIndex == LB_ERR || m_pTypeInfo == NULL)
    {
        SetWindowText( m_hwndFuncProto, _T("") ) ;
        return ;
    }

    TCHAR            szRet[128] ;
    TCHAR            szFuncName[64] ;
    lstrcpy( szFuncName, _T("<???>") ) ;

    LPTSTR           szfn = NULL ;
    LPFUNCDESC      pFuncDesc = NULL;
    HRESULT         hr ;
    UINT            cParams ;

    // the index of the FUNCDESC is stored as itemdata
    iIndex = (int)ListBox_GetItemData( m_hwndFunctions, iIndex ) ;

    // Get the FUNCDESC for this function.
    // Save off cParams and get the return value type
    //
    hr = m_pTypeInfo->GetFuncDesc( iIndex, &pFuncDesc ) ;
    if (SUCCEEDED(hr))
    {
        cParams = pFuncDesc->cParams ;
        lstrcpy( szRet, TYPEDESCtoString( &pFuncDesc->elemdescFunc.tdesc ) ) ;
        lstrcat( szRet, _T(" ") ) ;
        // Get the names of the function and it's parameters into rgbstrNames.
        // cNames gets the number of parameters + 1.
        //
        // Note:  T3 (2004 - 8/5/93) is really weird here.  It sometimes
        // returns cNames - 1 == pFuncDesc->cParams, sometimes it's < and
        // sometimes is >.   There is no sense to it.
        //
        hr = m_pTypeInfo->GetNames( pFuncDesc->memid, rgbstrNames, MAX_NAMES, &cNames );
        if (SUCCEEDED( hr ))
        {
            // Allocate a string buffer that should be able to hold
            // all of our parameter types and names.
            //
            if ((szfn = (LPTSTR) GlobalAllocPtr( GPTR, max(cNames,cParams) * (64) )) == NULL )
            {
                MessageBox( m_hDlg, _T("GlobalAlloc Failed!"), _T("Yikes!"), MB_OK ) ;
                return ;
            }

            // put the return value first
            lstrcpy( szfn, szRet ) ;

            // rgbstrNames[0] is the name of the function
            if (cNames > 0)
            {
#if defined(WIN32) && !defined(UNICODE)
                wcstombs(szFuncName, rgbstrNames[0], 64);
                lstrcat( szfn, szFuncName) ;
                lstrcat( szfn, _T("(") ) ;
#else
                lstrcat( szfn, rgbstrNames[0] ) ;
                lstrcat( szfn, _T("(") ) ;
                lstrcpy( szFuncName, rgbstrNames[0] ) ;
#endif
                SysFreeString( rgbstrNames[0] ) ;
            }

            // For each parameter get the type and name.
            // The "max(cNames-1,cParams)" should handle the case
            // where a function has optional parameters (i.e. cParamsOpt is
            // non-zero).
            //
            for ( UINT n = 0 ; n < max(cNames-1,cParams) ; n++ )
            {
                if (n == 0)
                    lstrcat( szfn, _T("\r\n    ") ) ;

                if (n < cParams)    // safety check
                {
                    lstrcat( szfn, g_rgszIDLFLAGS[pFuncDesc->lprgelemdescParam[n].idldesc.wIDLFlags] ) ;
                    lstrcat( szfn, _T(" ") ) ;
                    lstrcat( szfn, TYPEDESCtoString( &pFuncDesc->lprgelemdescParam[n].tdesc ) ) ;
                }

                if (n+1 < cNames)   // saftey check
                {
                    if (n < cParams)
                        lstrcat( szfn, _T(" ") ) ;
#if defined(WIN32) && !defined(UNICODE)
                    TCHAR szTemp[64];
                    wcstombs(szTemp, rgbstrNames[n+1], 64);
                    lstrcat( szfn, szTemp) ;
#else
                    lstrcat( szfn, rgbstrNames[n+1] ) ;
#endif

                    SysFreeString( rgbstrNames[n+1] ) ;
                }
                if (n + 1 == max(cNames-1,cParams))
                    lstrcat( szfn, _T("\r\n    ") );
                else
                    lstrcat( szfn, _T(",\r\n    ") ) ;
            }
            lstrcat( szfn , _T(")")  );
        }
        else
        {
            lstrcat( szRet, _T("GetNames FAILED: ") ) ;
            lstrcat( szRet, HRtoString( hr ) ) ;
        }

        if (szfn)
        {
            SetWindowText( m_hwndFuncProto, szfn ) ;
            GlobalFreePtr( szfn );
        }
        else
            SetWindowText( m_hwndFuncProto, szRet ) ;
    }
    else
    {
        wsprintf( szRet, _T("GetFuncDesc FAILED: %s "), HRtoString( hr ) ) ;
        SetWindowText( m_hwndFuncProto, szRet ) ;
        pFuncDesc = NULL ;
    }

    if (pFuncDesc != NULL)
    {
        TCHAR szTemp[256] ;
        wsprintf( szTemp, _T("FUNC&DESC for %s:"), (LPSTR)szFuncName ) ;
        SetWindowText( m_hwndInfoLbl, szTemp ) ;
        ListBox_ResetContent( m_hwndInfo ) ;

        DWORD    dwHelp ;
        BSTR    bstrName, bstrDoc, bstrHelpFile ;

        hr = m_pTypeInfo->GetDocumentation( pFuncDesc->memid, &bstrName, &bstrDoc,  &dwHelp, &bstrHelpFile ) ;
        if (SUCCEEDED(hr))
        {

            wsprintf( szTemp, _T("Name = %ws"), (bstrName == NULL ? OLESTR("n/a") : bstrName ) ) ;
            ListBox_AddString( m_hwndInfo, szTemp ) ;

            wsprintf( szTemp, _T("DocString = %ws"), (bstrDoc == NULL ? OLESTR("n/a") : bstrDoc) ) ;
            ListBox_AddString( m_hwndInfo, szTemp ) ;

            wsprintf( szTemp, _T("Help Context ID = %#08lX"), (DWORD)dwHelp ) ;
            ListBox_AddString( m_hwndInfo, szTemp ) ;

            wsprintf( szTemp, _T("Help File = %ws"), (bstrHelpFile == NULL ? OLESTR("n/a") : bstrHelpFile) ) ;
            ListBox_AddString( m_hwndInfo, szTemp ) ;

            SysFreeString( bstrName ) ;
            SysFreeString( bstrDoc) ;
            SysFreeString( bstrHelpFile ) ;
        }
        else
        {
            wsprintf( szTemp, _T("GetDocumentation FAILED: %s"), HRtoString( hr ) ) ;
            ListBox_AddString( m_hwndInfo, szTemp ) ;
        }

        DumpFUNCDESC( m_hwndInfo, pFuncDesc, _T("") ) ;
        m_pTypeInfo->ReleaseFuncDesc( pFuncDesc ) ;
    }
}

void IDispDlg::DoGetImplTypes( LPTYPEINFO lpTI, WORD cImplTypes )
{
    HRESULT     hr;
    #ifdef WIN32
    ULONG       iIndex;
    #else
    UINT        iIndex ;
    #endif
    HREFTYPE    hRefType;
    ITypeInfo   *lpitihRef;
    BSTR       bstrName;

    ListBox_ResetContent( m_hwndVariables);
    SetImplFlag( TRUE );
    for ( iIndex  = 0 ; iIndex < cImplTypes ; iIndex++ )
    {
        hr = lpTI->GetRefTypeOfImplType( iIndex, &hRefType );
        if ( SUCCEEDED(hr))
        {
           hr = lpTI->GetRefTypeInfo( hRefType, &lpitihRef);
           if (SUCCEEDED(hr))
           {
                hr = lpitihRef->GetDocumentation( MEMBERID_NIL,
                                                  &bstrName,
                                                  NULL,
                                                  NULL,
                                                  NULL);
                if (SUCCEEDED(hr))
                {
#if defined(WIN32) && !defined(UNICODE)
                    TCHAR szTemp[128];
                    wcstombs(szTemp, bstrName, 128);
                    ListBox_AddString( m_hwndVariables, szTemp ) ;
#else
                    ListBox_AddString( m_hwndVariables, bstrName ) ;
#endif
                }
                SysFreeString( bstrName );

           }
           lpitihRef->Release();
        }
    }
}


void IDispDlg::DoGetVars( LPTYPEINFO lpTI, WORD cVars )
{
    TCHAR    szBuf[128] ;
    WORD    iIndex ;
    TCHAR    szTemp[80] ;
    HRESULT hr ;
    BSTR    rgbstrNames[2] ;
    UINT    cNames ;

    ListBox_ResetContent( m_hwndVariables) ;

    for (iIndex = 0 ; iIndex < cVars ; iIndex++)
    {
        LPVARDESC      pVarDesc ;
        hr = lpTI->GetVarDesc( iIndex, &pVarDesc ) ;
        if (FAILED(hr))
        {
            wsprintf( szBuf, _T("GetVarDesc FAILED for variable #%u (%s)"), iIndex, HRtoString( hr ) ) ;
            ListBox_AddString( m_hwndVariables, szBuf ) ;
            continue ;
        }
        else
        {
            // memid\tname
            hr = lpTI->GetNames( pVarDesc->memid, rgbstrNames, 1, &cNames );
            if (SUCCEEDED( hr ))
            {
                wsprintf( szBuf, _T("%ws (%s"), rgbstrNames[0],
                            TYPEDESCtoString( &pVarDesc->elemdescVar.tdesc ) ) ;
                lstrcat( szBuf, _T(")") ) ;
                SysFreeString( rgbstrNames[0] ) ;

                if (pVarDesc->varkind == VAR_CONST)
                {
                    // NEED:  This just prints out the type.  We can get the value
                    // too.  Need to write at VARIANTtoString() function.
                    //
                    VARIANT varValue ;
                    VariantInit( &varValue ) ;
                    hr = VariantChangeType( &varValue, pVarDesc->lpvarValue, 0, VT_BSTR ) ;
                    if (FAILED(hr))
                        wsprintf( szTemp, _T(" = %s"), HRtoString( hr ) ) ;
                    else
                    {
                        wsprintf( szTemp, _T(" = %ws"), varValue.bstrVal ) ;
                        SysFreeString( varValue.bstrVal ) ;
                    }

                    lstrcat( szBuf, szTemp ) ;
                }

                // rgbstrNames[0] is the name of the function
                ListBox_SetItemData( m_hwndVariables,
                                 ListBox_AddString( m_hwndVariables, szBuf ),
                                 (LONG)iIndex ) ;
            }
            else
            {
                wsprintf( szTemp, _T("GetNames (%lu) FAILED: %s"), pVarDesc->memid, HRtoString( hr ) ) ;
                ListBox_AddString( m_hwndVariables, szTemp ) ;
            }

            lpTI->ReleaseVarDesc( pVarDesc ) ;
        }
    }

    ListBox_SetCurSel( m_hwndVariables, 0 ) ;
    OnSelChangeVariables() ;
}


void IDispDlg::OnSelChangeVariables()
{
    // we are overloading the m_hwndVariables to have
    // contain impltype info, which don't have an list
    // box item data.

    if (GetImplFlag())
    {
        return;
    }
    int         iIndex = ListBox_GetCurSel( m_hwndVariables ) ;
    LPVARDESC   pVarDesc = NULL;
    HRESULT     hr ;
    BSTR        rgbstrNames[2] ;

    UINT        cNames ;
    TCHAR        szTemp[256] ;

    if (iIndex == LB_ERR || m_pTypeInfo == NULL)
        return ;

    ListBox_ResetContent( m_hwndInfo ) ;

    // the index of the VARDESC is stored as itemdata
    iIndex = (int)ListBox_GetItemData( m_hwndVariables, iIndex ) ;

    // Get the VARDESC for this Vartion.
    // Save off cParams and get the return value type
    //
    hr = m_pTypeInfo->GetVarDesc( iIndex, &pVarDesc ) ;
    if (SUCCEEDED(hr))
    {
        hr = m_pTypeInfo->GetNames( pVarDesc->memid, rgbstrNames, 1, &cNames );
        if (SUCCEEDED(hr))
        {
            wsprintf( szTemp, _T("Var&DESC for %ws:"), rgbstrNames[0] ) ;
            SysFreeString( rgbstrNames[0] ) ;

        }
        else
            lstrcpy( szTemp, _T("Var&DESC:") ) ;
        SetWindowText( m_hwndInfoLbl, szTemp ) ;

        DWORD   dwHelp ;
        BSTR    bstrName, bstrDoc, bstrHelpFile ;
        hr = m_pTypeInfo->GetDocumentation( pVarDesc->memid, &bstrName, &bstrDoc,  &dwHelp, &bstrHelpFile ) ;
        if (SUCCEEDED(hr))
        {
            wsprintf( szTemp, _T("Name = %ws"), (bstrName == NULL ? OLESTR("n/a") : bstrName ) ) ;
            ListBox_AddString( m_hwndInfo, szTemp ) ;

            wsprintf( szTemp, _T("DocString = %ws"), (bstrDoc == NULL ? OLESTR("n/a") : bstrDoc) ) ;
            ListBox_AddString( m_hwndInfo, szTemp ) ;

            wsprintf( szTemp, _T("Help Context ID = %#08lX"), (DWORD)dwHelp ) ;
            ListBox_AddString( m_hwndInfo, szTemp ) ;

            wsprintf( szTemp, _T("Help File = %ws"), (bstrHelpFile == NULL ? OLESTR("n/a") : bstrHelpFile) ) ;
            ListBox_AddString( m_hwndInfo, szTemp ) ;

            SysFreeString( bstrName ) ;
            SysFreeString( bstrDoc) ;
            SysFreeString( bstrHelpFile ) ;

        }
        else
        {
            wsprintf( szTemp, _T("GetDocumentation FAILED: %s"), HRtoString( hr ) ) ;
            ListBox_AddString( m_hwndInfo, szTemp ) ;
        }

        DumpVARDESC( m_hwndInfo, pVarDesc, _T("") ) ;
        m_pTypeInfo->ReleaseVarDesc( pVarDesc ) ;

    }
    else
    {
        wsprintf( szTemp, _T("GetVarDesc FAILED: %s "), (LPTSTR)HRtoString( hr ) ) ;
        ListBox_AddString( m_hwndInfo, szTemp ) ;
    }
}

void DumpTYPEDESC( HWND hwnd, TYPEDESC FAR* lptdesc, LPTSTR szLabel )
{
    TCHAR szTemp[128] ;
    if (lptdesc == NULL)
    {
        wsprintf( szTemp, _T("%slptdesc is NULL (this is an error)"), (LPTSTR)szLabel ) ;
        ListBox_AddString( hwnd, szTemp ) ;
        return ;
    }

    // Dump a TYPEDESC
    wsprintf( szTemp, _T("%svt = %s"), (LPTSTR)szLabel, (LPTSTR)VTtoString( lptdesc->vt ) ) ;
    ListBox_AddString( hwnd, szTemp ) ;

    if ((lptdesc->vt & 0x0FFF) == VT_PTR)
    {
        // lptdesc points to a TYPEDESC that specifies the thing pointed to
        wsprintf( szTemp, _T("%slptdesc->"), (LPTSTR)szLabel ) ;
        DumpTYPEDESC( hwnd, lptdesc->lptdesc, szTemp ) ;
        return ;
    }

    if ((lptdesc->vt & 0x0FFF) == VT_USERDEFINED)
    {
        wsprintf( szTemp, _T("%shreftype = %#08lX"), (LPTSTR)szLabel, (DWORD)lptdesc->hreftype ) ;
        ListBox_AddString( hwnd, szTemp ) ;
        return ;
    }

    if ((lptdesc->vt & 0x0FFF) == VT_CARRAY)
    {
        wsprintf( szTemp, _T("%slpadesc->"), (LPTSTR)szLabel ) ;
        DumpARRAYDESC( hwnd, lptdesc->lpadesc, szTemp ) ;
        return ;
    }

#ifdef _VT_FUNCPTR_DEFINED
    if ((lptdesc->vt & 0x0FFF) == VT_FUNCPTR)
    {
        wsprintf( szTemp, "%slpfdesc->", szLabel ) ;
        DumpFUNCDESC( hwnd, lptdesc->lpadesc, szTemp ) ;
        return ;
    }
#endif
}


void DumpARRAYDESC( HWND hwnd, ARRAYDESC FAR* lpadesc, LPTSTR szLabel )
{
    TCHAR szTemp[128] ;

    if (lpadesc == NULL)
    {
        wsprintf( szTemp, _T("%slpadesc is NULL (this is an error)"), szLabel ) ;
        ListBox_AddString( hwnd, szTemp ) ;
        return ;
    }

    wsprintf( szTemp, _T("%stdescElem."), szLabel ) ;
    DumpTYPEDESC( hwnd, &lpadesc->tdescElem, szTemp ) ;

    wsprintf( szTemp, _T("%scDims = %u"),  szLabel, lpadesc->cDims ) ;
    ListBox_AddString( hwnd, szTemp ) ;
}

void DumpIDLDESC( HWND hwnd, LPIDLDESC lpidldesc, LPTSTR szLabel )
{
    TCHAR szTemp[128] ;

    wsprintf(szTemp, _T("%swIDLFlags = %s"), szLabel,
            g_rgszIDLFLAGS[lpidldesc->wIDLFlags] ) ;
    ListBox_AddString( hwnd, szTemp ) ;

#ifndef WIN32
    if (lpidldesc->bstrIDLInfo != NULL)
        wsprintf( szTemp, "%sbstrIDLInfo = %s", szLabel,
                    lpidldesc->bstrIDLInfo ) ;
    else
        wsprintf( szTemp, "%sbstrIDLInfo = NULL", szLabel ) ;
#else
    wsprintf( szTemp, _T("%sdwReserved = %#08lX"), szLabel,
              lpidldesc->dwReserved ) ;
#endif
    ListBox_AddString( hwnd, szTemp ) ;
}

void DumpELEMDESC( HWND hwnd, LPELEMDESC lpelemdesc, LPTSTR szLabel )
{
    TCHAR szTemp[128] ;

    wsprintf( szTemp, _T("%stdesc."), szLabel ) ;
    DumpTYPEDESC( hwnd, &lpelemdesc->tdesc, szTemp ) ;

    wsprintf( szTemp, _T("%sidldesc."), szLabel ) ;
    DumpIDLDESC( hwnd, &lpelemdesc->idldesc, szTemp ) ;
}

void DumpFUNCDESC( HWND hwnd, LPFUNCDESC lpfndesc, LPTSTR szLabel )
{
    TCHAR szTemp[128] ;

    // MEMBERID  memid ;
    wsprintf( szTemp, _T("%smemid = %#08lX"), szLabel, (DWORD)lpfndesc->memid ) ;
    ListBox_AddString( hwnd, szTemp ) ;

    // FUNCKIND  funckind ;
    wsprintf( szTemp, _T("%sfunckind = %s"), szLabel, g_rgszFUNCKIND[lpfndesc->funckind] ) ;
    ListBox_AddString( hwnd, szTemp ) ;

    // INVOKEKIND   invkind ;
    if (lpfndesc->invkind == INVOKE_FUNC)
        wsprintf( szTemp, _T("%sinvkind = FUNC"), szLabel ) ;
    else if (lpfndesc->invkind == INVOKE_PROPERTYGET)
        wsprintf( szTemp, _T("%sinvkind = PROPERTYGET"), szLabel ) ;
    else if (lpfndesc->invkind == INVOKE_PROPERTYPUT)
        wsprintf( szTemp, _T("%sinvkind = PROPERTYPUT"), szLabel ) ;
    else if (lpfndesc->invkind == INVOKE_PROPERTYPUTREF)
        wsprintf( szTemp, _T("%sinvkind = PROPERTYPUTREF"), szLabel ) ;
    ListBox_AddString( hwnd, szTemp ) ;

    // CALLCONV callconv;
    wsprintf( szTemp, _T("%scallconv = %s"), szLabel, g_rgszCALLCONV[lpfndesc->callconv] ) ;
    ListBox_AddString( hwnd, szTemp ) ;

    // SHORT cParams;
    wsprintf( szTemp, _T("%scParams = %d"), szLabel, lpfndesc->cParams ) ;
    ListBox_AddString( hwnd, szTemp ) ;

    // SHORT cParamsOpt;
    wsprintf( szTemp, _T("%scParamsOpt = %d"), szLabel, lpfndesc->cParamsOpt ) ;
    ListBox_AddString( hwnd, szTemp ) ;

    // SHORT oVft;
    wsprintf( szTemp, _T("%soVft = %d"), (LPTSTR)szLabel, lpfndesc->oVft ) ;
    ListBox_AddString( hwnd, szTemp ) ;

    // WORD wFuncFlags;
    wsprintf( szTemp, _T("%swFuncFlags = %u"), szLabel, lpfndesc->wFuncFlags ) ;
    ListBox_AddString( hwnd, szTemp ) ;

    // ELEMDESC elemdescFunc;
    wsprintf( szTemp, _T("%selemdescFunc."), szLabel ) ;
    DumpELEMDESC( hwnd, &lpfndesc->elemdescFunc, szTemp ) ;

    // ELEMDESC FAR* lprgelemdescParam;  /* array of parameter types */
    for (SHORT s = 0 ; s < lpfndesc->cParams ; s++)
    {
        wsprintf( szTemp, _T("%slprgelemdescParam[%u]."), szLabel, s ) ;
        DumpELEMDESC( hwnd, &lpfndesc->lprgelemdescParam[s], szTemp ) ;
    }

    // SHORT cScodes;
    wsprintf( szTemp, _T("%scScodes = %d"), szLabel, lpfndesc->cScodes ) ;
    ListBox_AddString( hwnd, szTemp ) ;

#if 1
// BUGBUG:  Run Ole2View, invoke IDispatch Calc #2, double click on it's
// IDispatch, close the viewer.   Invoke Word Basic, double click on it's
// IDispatch.  All calls to GetFuncDesc return a FUNCDESC where cScodes is
// some bogus # like 0x0778.   Note that if we invoke Word Basic right
// after Ole2View starts up this problem does not occur.

    // SCODE FAR* lprgscode;
    if (lpfndesc->cScodes > 0)
    {
        //AssertSz( lpfndesc->lprgscode, "lpfndesc->sCodes > 0 when it shouldn't be" ) ;
        for (s = 0 ; s < lpfndesc->cScodes ; s++)
        {
            if (lpfndesc->lprgscode == NULL)
                wsprintf( szTemp, _T("%slprgscode = NULL (this is an error)"), szLabel ) ;
            else
                wsprintf( szTemp, _T("%slprgscode[%u] = %s"), szLabel, s,
                        HRtoString( ResultFromScode( lpfndesc->lprgscode[s] ) ) ) ;
            ListBox_AddString( hwnd, szTemp ) ;
        }
    }

    //AssertSz( !(lpfndesc->cScodes < -1), "lpfndesc->sCodes < -1 when it shouldn't be" ) ;

#endif
}

void DumpVARDESC( HWND hwnd, LPVARDESC lpvardesc, LPTSTR szLabel )
{
    TCHAR szTemp[128] ;

    // MEMBERID memid;
    wsprintf( szTemp, _T("%smemid = %#08lX"), szLabel, (DWORD)lpvardesc->memid ) ;
    ListBox_AddString( hwnd, szTemp ) ;

    // WORD wVarFlags;
    wsprintf( szTemp, _T("%swVarFlags = %s"), szLabel,
              lpvardesc->wVarFlags == VARFLAG_FREADONLY ? _T("READONLY") :  _T("NOT READONLY") ) ;
    ListBox_AddString( hwnd, szTemp ) ;

    // VARKIND varkind;
    wsprintf( szTemp, _T("%svarkind = %s"), szLabel, g_rgszVARKIND[lpvardesc->varkind] ) ;
    ListBox_AddString( hwnd, szTemp ) ;

    // ELEMDESC elemdescVar;
    wsprintf( szTemp, _T("%selemdescVar."), szLabel ) ;
    DumpELEMDESC( hwnd, &lpvardesc->elemdescVar, szTemp ) ;

    // union { ULONG oInst;  VARIANT FAR* lpvarValue;  }
    if (lpvardesc->varkind == VAR_PERINSTANCE)
    {
        wsprintf( szTemp, _T("%soInst = %lu"), szLabel, lpvardesc->oInst ) ;
    }
    else if (lpvardesc->varkind == VAR_CONST)
    {
        // NEED:  This just prints out the type.  We can get the value
        // too.  Need to write at VARIANTtoString() function.
        //
        //
        VARIANT varValue ;
        VariantInit( &varValue ) ;
        HRESULT hr = VariantChangeType( &varValue, lpvardesc->lpvarValue, 0, VT_BSTR ) ;
        if (FAILED(hr))
            wsprintf( szTemp, _T("%slpvarValue = %s = %s"), szLabel,
                        VTtoString( lpvardesc->lpvarValue->vt ), HRtoString( hr ) ) ;
        else
        {
            wsprintf( szTemp, _T("%slpvarValue = %s = %ws"),  szLabel,
                    VTtoString( lpvardesc->lpvarValue->vt ), varValue.bstrVal ) ;
            SysFreeString( varValue.bstrVal ) ;
        }

        ListBox_AddString( hwnd, szTemp ) ;
    }
}

// end of idisp.cpp

