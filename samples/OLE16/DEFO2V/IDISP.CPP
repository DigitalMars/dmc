// idisp.cpp
//
// Implementation file for the IDispatch interface viewer.
//
// Copyright (c) 1993 Microsoft Corporation, All Rights Reserved.
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

#include "debug.h"

// SCODE_FIX determines whether the code in DEFO2V that interrogates the
// cScode and lprgscodes members of FUNCDESC is enabled or not.  This
// define should be defined if the bug relating to the cScode and
// lprgscodes members of FUNCDESC not being intialized and
// marshalled correctly by OLE2DISP.DLL has been fixed.
// As of the 10/7/93 build of the OLE2DISP.DLL (23.618) the bug
// has not been fixed.
//
// #define SCODE_FIX


BOOL DoTypeInfoToFile( HWND hwnd, LPTYPEINFO lpTypeInfo )  ;
BOOL DoTypeLibToFile( HWND hwnd, LPTYPELIB lpTypeLib ) ;

static HFONT        g_hFont ;
static UINT         g_cyFont ;

// These functions take a hwnd of a listbox, an IDispatch related structure,
// and string and dump the contents of the structure to the listbox.
// All structure members are prefixed by szLabel in the listbox.
//
void DumpTYPEDESC( HWND hwnd, TYPEDESC FAR* lptdesc, LPSTR szLabel ) ;
void DumpARRAYDESC( HWND hwnd, ARRAYDESC FAR* lpadesc, LPSTR szLabel ) ;
void DumpIDLDESC( HWND hwnd, LPIDLDESC lp, LPSTR szLabel ) ;
void DumpELEMDESC( HWND hwnd, LPELEMDESC lp, LPSTR szLabel ) ;
void DumpFUNCDESC( HWND hwnd, LPFUNCDESC lpfndesc, LPSTR szLabel ) ;
void DumpVARDESC( HWND hwnd, LPVARDESC lpvardesc, LPSTR szLabel ) ;

static char * g_rgszTYPEKIND[] =
{
	"enum",         /* TKIND_ENUM */
	"Struct",       /* TKIND_RECORD */
	"Module",       /* TKIND_MODULE */
	"Interface",    /* TKIND_INTERFACE */
	"Dispinterface",    /* TKIND_DISPATCH */
	"Coclass",      /* TKIND_COCLASS */
	"Typedef",      /* TKIND_ALIAS */

	/* NOTE: the following aren't supported in typeinfo yet */
	"Union",        /* TKIND_UNION */
	"Encap Union",   /* TKIND_ENCUNION */
	"Class",        // TKIND_Class
	"cotype",       // TKIND_COOTYPE
	"<error!>",     // dummy entries just in case the ole spec changes
	"<error!>",     // and new enum's are added!
	"<error!>",
	"<error!>",
	"<error!>"
};

static char * g_rgszIDLFLAGS[] =
{
	"",         //IDLFLAG_NONE = 0,
	"IN",       //IDLFLAG_FIN = 1,
	"OUT",      //IDLFLAG_FOUT = 2,
	"IN OUT",   //IDLFLAF_FIN | IDLFLAG_FOUT
	"<error!>",     // dummy entries just in case the ole spec changes
	"<error!>",     // and new enum's are added!
	"<error!>",
	"<error!>",
	"<error!>"
};

static char * g_rgszFUNCKIND[] =
{
	"VIRUTAL",
	"PUREVIRTUAL",
	"NONVIRTUAL",
	"STATIC",
	"DISPATCH",
	"<error!>",     // dummy entries just in case the ole spec changes
	"<error!>",     // and new enum's are added!
	"<error!>",
	"<error!>",
	"<error!>"
} ;

static char * g_rgszCALLCONV[] =
{
	"0",
	"CDECL",        // CC_CDECL, = 1,
	"PASCAL",       // CC_MSCPASCAL and CC_PASCAL
	"MACPASCAL",     // CC_MACPASCAL,
	"STDCALL",       // CC_STDCALL,
	"THISCALL",      // CC_THISCALL,
	"MAX",           // end of enum marker
	"<error!>",     // dummy entries just in case the ole spec changes
	"<error!>",     // and new enum's are added!
	"<error!>",
	"<error!>",
	"<error!>"
};

static char * g_rgszVARKIND[] =
{
	"PERINSTANCE",  //VAR_PERINSTANCE,
	"STATIC",       //VAR_STATIC
	"CONST",        //VAR_CONST
	"DISPATCH",      //VAR_DISPATCH
	"<error!>",     // dummy entries just in case the ole spec changes
	"<error!>",     // and new enum's are added!
	"<error!>",
	"<error!>",
	"<error!>"
} ;

ASSERTDATA
// DisplayIDispatch
//
// This
extern "C"
HRESULT WINAPI _export DisplayIDispatch( HWND hwndParent, LPDISPATCH lpDisp, LPIID lpiid, LPSTR lpszName )
{
	HRESULT hr = NULL ;

	Assert( hwndParent ) ;
	Assert( lpDisp ) ;
	Assert( lpiid ) ;

	#ifdef _DEBUG
	ULONG ulRefCount ;
	if (lpDisp) lpDisp->AddRef() ;
	if (lpDisp) ulRefCount = lpDisp->Release() ;
	#endif

	IDispDlg    dispdlg( hwndParent, (LPUNKNOWN)lpDisp, lpiid, lpszName ) ;
	dispdlg.DoModal() ;

	#ifdef _DEBUG
	if (lpDisp) lpDisp->AddRef() ;
	if (lpDisp)
		AssertSz( ulRefCount == lpDisp->Release(), "Ref counts don't match!" ) ;
	#endif

	return hr ;
}

// DisplayITypeInfo
//
// This
extern "C"
HRESULT WINAPI _export DisplayITypeInfo( HWND hwndParent, LPTYPEINFO lpTypeInfo, LPIID lpiid, LPSTR lpszName )
{
	HRESULT hr = NULL ;

	Assert( hwndParent ) ;
	Assert( lpTypeInfo ) ;
	Assert( lpiid ) ;

	#ifdef _DEBUG
	ULONG ulRefCount ;
	if (lpTypeInfo) lpTypeInfo->AddRef() ;
	if (lpTypeInfo) ulRefCount = lpTypeInfo->Release() ;
	#endif

	IDispDlg    dispdlg( hwndParent, (LPUNKNOWN)lpTypeInfo,lpiid, lpszName ) ;
	dispdlg.DoModal() ;

	#ifdef _DEBUG
	if (lpTypeInfo) lpTypeInfo->AddRef() ;
	if (lpTypeInfo)
		AssertSz( ulRefCount == lpTypeInfo->Release(), "Ref counts don't match!" ) ;
	#endif

	return hr ;
}

// DisplayITypeLib
//
// This
extern "C"
HRESULT WINAPI _export DisplayITypeLib( HWND hwndParent, LPTYPELIB lpTypeLib, LPIID lpiid, LPSTR lpszName )
{
	HRESULT hr = NULL ;

	Assert( hwndParent ) ;
	Assert( lpTypeLib ) ;
	Assert( lpiid ) ;

	#ifdef _DEBUG
	ULONG ulRefCount ;
	if (lpTypeLib) lpTypeLib->AddRef() ;
	if (lpTypeLib) ulRefCount = lpTypeLib->Release() ;
	#endif

	IDispDlg    dispdlg( hwndParent, (LPUNKNOWN)lpTypeLib, lpiid, lpszName ) ;
	dispdlg.DoModal() ;

	#ifdef _DEBUG
	if (lpTypeLib) lpTypeLib->AddRef() ;
	if (lpTypeLib)
		AssertSz( ulRefCount == lpTypeLib->Release(), "Ref counts don't match!" ) ;
	#endif

	return hr ;
}


/////////////////////////////////////////////////////////////////////////////
// IDispDlg dialog
//
// The constructor is where we begin to descern between the three possible
// interfaces that could be passed in.
//
IDispDlg::IDispDlg(HWND hwnd, LPUNKNOWN lpUnk, LPIID lpiid, LPSTR lpszName )
{
	m_hWndParent = hwnd ;
	m_piid = lpiid ;
	m_pszName = lpszName ;

	m_pDispatch = NULL ;
	m_pTypeInfo = NULL ;
	m_pTypeLib = NULL ;

	if (IsEqualIID( *m_piid, IID_IDispatch ))
	{
		m_pDispatch = (LPDISPATCH)lpUnk ;
		m_pDispatch->AddRef() ;
	}
	else if (IsEqualIID( *m_piid, IID_ITypeInfo ))
	{
		m_pTypeInfo = (LPTYPEINFO)lpUnk ;
		m_pTypeInfo->AddRef() ;
	}
	else if (IsEqualIID( *m_piid, IID_ITypeLib ))
	{
		m_pTypeLib = (LPTYPELIB)lpUnk ;
		m_pTypeLib->AddRef() ;
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
BOOL CALLBACK __export fnIDispDlg( HWND hDlg, UINT uiMsg, WPARAM wParam, LPARAM lParam )
{
	LPIDISPDLG pIDD =(LPIDISPDLG)GetWindowLong( hDlg, DWL_USER ) ;

	switch (uiMsg)
	{
		case WM_INITDIALOG:
			pIDD=(LPIDISPDLG)lParam ;
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
	g_hFont = ReallyCreateFont( hdc, "MS Sans Serif", "Regular", 8, 0 ) ;
	g_hFont = SelectObject( hdc, g_hFont ) ;
	GetTextMetrics( hdc, &tm ) ;
	g_hFont = SelectObject( hdc, g_hFont ) ;
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


	char szBuf[256] ;
	wsprintf( szBuf, "%s Viewer", (LPSTR)m_pszName ) ;
	SetWindowText( m_hDlg, szBuf ) ;

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

	RECT    rc ;
	RECT    rcWnd ;
	GetClientRect( m_hDlg, &rcWnd ) ;
	GetWindowRect( m_hwndTypeAttr, &rc ) ;
	MapWindowPoints( NULL, m_hDlg, (POINT FAR*)&rc, 2 ) ;
	rc.left = rcWnd.left + 5 ;
	SetWindowPos( m_hwndTypeAttr, NULL, rcWnd.left + 5, rc.top, cx - 10, rc.bottom - rc.top,
				SWP_NOZORDER | SWP_NOACTIVATE ) ;
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

	char        szTemp[128] ;
	UINT        uiTypeInfoCount = 0;
	HRESULT hr ;

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
			wsprintf( szTemp, "IDispatch::GetTypeInfoCount( ) FAILED: %s", (LPSTR)HRtoString( hr ) ) ;
			MessageBox( m_hDlg, szTemp, "Could not get type information", MB_OK ) ;
			return ;
		}

		if (uiTypeInfoCount <= 0)
		{
			wsprintf( szTemp, "This object reports zero TypeInfos.  There is no programmability information available." ) ;
			MessageBox( m_hDlg, szTemp, "Could not get type information", MB_OK ) ;
			return ;
		}

		hr = m_pDispatch->GetTypeInfo( 0, LOCALE_SYSTEM_DEFAULT, &m_pTypeInfo ) ;
		if (FAILED( hr ))
		{
			wsprintf( szTemp, "IDispatch::GetTypeInfo( 0 ) FAILED: %s", (LPSTR)HRtoString( hr ) ) ;
			MessageBox( m_hDlg, szTemp, "Could not get type information", MB_OK ) ;
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
			SetWindowText( m_hwndTypeInfoInfo, "This object uses a Type Library." ) ;
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
				wsprintf( szTemp, "IDispatch::GetTypeInfoCount FAILED: %s", (LPSTR)HRtoString( hr ) ) ;
				MessageBox(  m_hDlg, szTemp, "Could not get count of typeinfos", MB_OK ) ;
				return ;
			}

			SetWindowText( m_hwndTypeInfoInfo, "This object does not use a Type Library." ) ;
		}
	}
	else if (m_pTypeLib != NULL)
	{
		// If we were called with a typelib we are in great shape
		//
		uiTypeInfoCount = m_pTypeLib->GetTypeInfoCount() ;
		SetWindowText( m_hwndTypeInfoInfo, "Browsing a TypeLibrary." ) ;
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
			SetWindowText( m_hwndTypeInfoInfo, "This object uses a Type Library." ) ;
		}
		else
		{
			// If we get here then we assume that there is only one
			// typeinfo and we're looking at it.
			//
			uiTypeInfoCount = 1 ;
			SetWindowText( m_hwndTypeInfoInfo, "Browsing a TypeInfo" ) ;
		}
	}

	wsprintf( szTemp, "%u", (UINT)uiTypeInfoCount ) ;
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
				hr = m_pTypeLib->GetTypeInfo( n-1, &m_pTypeInfo ) ;
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
				wsprintf( szTemp, "%s (%u)", (LPSTR)bstrName, n-1 ) ;
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
				wsprintf( szTemp, "%s (%u)", (LPSTR)rgbstrNames[0], n ) ;
				ui = ComboBox_AddString( m_hwndTypeInfo, szTemp ) ;
				ComboBox_SetItemData( m_hwndTypeInfo, ui, (DWORD)n ) ;
				SysFreeString( rgbstrNames[0] ) ;
			}
			else
#endif
			{
				UINT ui ;
				wsprintf( szTemp, "%u (no name)", n-1 ) ;
				ui = ComboBox_AddString( m_hwndTypeInfo, szTemp ) ;
				ComboBox_SetItemData( m_hwndTypeInfo, ui, (DWORD)n-1 ) ;
			}
		}
		else
		{
			wsprintf(szTemp, "%s::GetTypeInfo(#%u) FAILED: %s",
						(LPSTR)(m_pTypeLib == NULL ? "IDispatch" :
							"ITypeLib" ), n-1, (LPSTR)HRtoString( hr ) ) ;
			MessageBox( m_hDlg, szTemp, "Error", MB_OK ) ;
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
	UINT        uiTinfo ;
	int         uiItem = ComboBox_GetCurSel( m_hwndTypeInfo );
	char        szTemp[128] ;

	if (uiItem == CB_ERR)
		return ;

	ListBox_ResetContent( m_hwndTypeAttr ) ;
	ListBox_ResetContent( m_hwndFunctions ) ;
	ListBox_ResetContent( m_hwndVariables ) ;
	ListBox_ResetContent( m_hwndInfo ) ;
	SetWindowText( m_hwndFuncProto, "" ) ;
	SetWindowText( m_hwndInfoLbl, "F&UNCDESC/VARDESC:" ) ;

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
		{
			// in the case where the viewer was called with a pIDispatch we need
			// to check to see if the typeinfo returned by GetTypeInfo is of
			// type CoClass.  If it is, then we need to use GetRefTypeInfo
			// to get the Interface TypeInfo.
			//
			// Note this is somewhat of a hack.  This viewer should be redesigned
			// to handle the heirarchy introduced by the CoClass types.
			//
			hr = m_pDispatch->GetTypeInfo( uiTinfo, LOCALE_SYSTEM_DEFAULT, &m_pTypeInfo ) ;
			if (SUCCEEDED( hr ))
			{
				LPTYPEATTR      pTA = NULL ;
				LPTYPEINFO      pTI = NULL ;

				hr = m_pTypeInfo->GetTypeAttr( &pTA ) ;
				if (FAILED( hr ))
				{
					wsprintf( szTemp, "GetTypeAttr FAILED: %s", (LPSTR)HRtoString( hr ) ) ;
					ListBox_AddString( m_hwndTypeAttr, szTemp ) ;
					hr = S_OK ;
				}
				else
				{
					if (pTA->typekind == TKIND_COCLASS)
					{
						m_pTypeInfo->ReleaseTypeAttr( pTA ) ;
						pTA = NULL ;

						wsprintf( szTemp, "This TypeInfo is of type TKIND_COCLASS" ) ;
						ListBox_AddString( m_hwndTypeAttr, szTemp ) ;

						wsprintf( szTemp, "  Using TypeInfo from GetRefTypeInfo instead" ) ;
						ListBox_AddString( m_hwndTypeAttr, szTemp ) ;
						hr = m_pTypeInfo->GetRefTypeInfo( 0, &pTI ) ;
						if (FAILED(hr))
						{
							wsprintf( szTemp, "    GetRefTypeInfo failed: %s", (LPSTR)HRtoString( hr )) ;
							ListBox_AddString( m_hwndTypeAttr, szTemp ) ;
						}
						else
						{

							m_pTypeInfo->Release() ;
							m_pTypeInfo = pTI ;
						}
					}
					else
					{
						m_pTypeInfo->ReleaseTypeAttr( pTA ) ;
						pTA = NULL ;
					}
				}
			}

		}
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
			wsprintf( szTemp, "Name = %s", (LPSTR)(bstrName == NULL ? "n/a" : bstrName) ) ;
			ListBox_AddString( m_hwndTypeAttr, szTemp ) ;

			wsprintf( szTemp, "DocString = %s", (LPSTR)(bstrDoc == NULL ? "n/a" : bstrDoc) ) ;
			ListBox_AddString( m_hwndTypeAttr, szTemp ) ;

			wsprintf( szTemp, "Help Context ID = %#08lX", (DWORD)dwHelp ) ;
			ListBox_AddString( m_hwndTypeAttr, szTemp ) ;

			wsprintf( szTemp, "Help File = %s", (LPSTR)(bstrHelpFile == NULL ? "n/a" : bstrHelpFile) ) ;
			ListBox_AddString( m_hwndTypeAttr, szTemp ) ;

			SysFreeString( bstrName ) ;
			SysFreeString( bstrDoc) ;
			SysFreeString( bstrHelpFile ) ;
		}
		else
		{
			wsprintf( szTemp, "GetDocumentation FAILED: %s", (LPSTR)HRtoString( hr ) ) ;
			ListBox_AddString( m_hwndTypeAttr, szTemp ) ;
		}

		// Get the containing typelib for this typeinfo.
		//
		UINT ui ;
		LPTYPELIB   pTL ;
		hr = m_pTypeInfo->GetContainingTypeLib( &pTL, &ui ) ;
		if (SUCCEEDED( hr ))
		{
			wsprintf( szTemp, "GetContainingTypeLib returned valid pTypelib (%#08lX, %u)",
						(LONG)pTL, ui ) ;
			pTL->Release() ;
		}
		else
			wsprintf( szTemp, "GetContainingTypeLib FAILED:  %s", (LPSTR)HRtoString( hr ) ) ;
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
		wsprintf( szTemp, "GetTypeInfo(#%u) FAILED: %s", uiTinfo, (LPSTR)HRtoString( hr ) ) ;
		ListBox_AddString( m_hwndTypeAttr, szTemp ) ;
	}
}

BOOL IDispDlg::DoTypeAttr( LPTYPEINFO  pTI )
{
	HRESULT hr ;
	char    szTemp[64] ;
	LPTYPEATTR  pTA = NULL ;
	LPTYPEINFO  pTItemp = NULL ;

	hr = pTI->GetTypeAttr( &pTA ) ;
	if (FAILED( hr ))
	{
		wsprintf( szTemp, "GetTypeAttr FAILED: %s", (LPSTR)HRtoString( hr ) ) ;
		ListBox_AddString( m_hwndTypeAttr, szTemp ) ;
		return FALSE ;
	}

	if (pTA->typekind > TKIND_MAX)
	{
		wsprintf( szTemp, "typekind = <invalid!> (%u)", pTA->typekind ) ;
		ListBox_AddString( m_hwndTypeAttr, szTemp ) ;
	}
	else
	{
		wsprintf( szTemp, "typekind = %s", (LPSTR)g_rgszTYPEKIND[pTA->typekind] ) ;
		ListBox_AddString( m_hwndTypeAttr, szTemp ) ;
	}

	wsprintf( szTemp, "Version =  %u.%03d", pTA->wMajorVerNum, pTA->wMajorVerNum ) ;
	ListBox_AddString( m_hwndTypeAttr, szTemp ) ;
	wsprintf( szTemp, "lcid = %u",  pTA->lcid ) ;
	ListBox_AddString( m_hwndTypeAttr, szTemp ) ;
	wsprintf( szTemp, "cFuncs = %d", pTA->cFuncs ) ;
	ListBox_AddString( m_hwndTypeAttr, szTemp ) ;
	wsprintf( szTemp, "cVars = %u", pTA->cVars ) ;
	ListBox_AddString( m_hwndTypeAttr, szTemp ) ;
	wsprintf( szTemp, "cImplTypes = %u", pTA->cImplTypes ) ;
	ListBox_AddString( m_hwndTypeAttr, szTemp ) ;

	// DumpTYPEDESC will completely decode the tdescAlias for us...
	//
	DumpTYPEDESC( m_hwndTypeAttr, &pTA->tdescAlias, "tdescAlias." ) ;

	wsprintf(szTemp, "guid = {%08lX-%04X-%04X-%02X%02X-%02X%02X%02X%02X%02X%02X}",
		pTA->guid.Data1, pTA->guid.Data2, pTA->guid.Data3,
		pTA->guid.Data4[0], pTA->guid.Data4[1],
		pTA->guid.Data4[2], pTA->guid.Data4[3],
		pTA->guid.Data4[4], pTA->guid.Data4[5],
		pTA->guid.Data4[6], pTA->guid.Data4[7]);
	ListBox_AddString( m_hwndTypeAttr, szTemp ) ;

	wsprintf(szTemp, "wTypeFlags = %04X", pTA->wTypeFlags ) ;
	ListBox_AddString( m_hwndTypeAttr, szTemp ) ;

	wsprintf(szTemp, "dwReserved = %#08lX", pTA->dwReserved ) ;
	ListBox_AddString( m_hwndTypeAttr, szTemp ) ;

	wsprintf(szTemp, "cbAlignment = %u", pTA->cbAlignment ) ;
	ListBox_AddString( m_hwndTypeAttr, szTemp ) ;

	wsprintf(szTemp, "cbSizeInstance = %u", pTA->cbSizeInstance ) ;
	ListBox_AddString( m_hwndTypeAttr, szTemp ) ;

	wsprintf(szTemp, "cbSizeVft = %u", pTA->cbSizeVft ) ;
	ListBox_AddString( m_hwndTypeAttr, szTemp ) ;

	DumpIDLDESC( m_hwndTypeAttr, &pTA->idldescType, "idldescType." ) ;

	wsprintf(szTemp, "memidConstructor = %#08lX", (DWORD)pTA->memidConstructor ) ;
	ListBox_AddString( m_hwndTypeAttr, szTemp ) ;

	wsprintf(szTemp, "memidDestructor = %#08lX", (DWORD)pTA->memidDestructor ) ;
	ListBox_AddString( m_hwndTypeAttr, szTemp ) ;

	// Get functions
	DoGetFunctions( pTI, pTA->cFuncs ) ;

	// Get variables
	DoGetVars( pTI, pTA->cVars ) ;

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
	char    szBuf[128] ;
	HRESULT hr ;
	char            szTemp[80] ;
	BSTR            rgbstrNames[2] ;
	UINT            cNames ;
	LPFUNCDESC      pFuncDesc ;
	WORD iIndex ;

	ListBox_ResetContent( m_hwndFunctions ) ;

	for ( iIndex = 0 ; iIndex < cFuncs ; iIndex++)
	{
		hr = lpTI->GetFuncDesc( iIndex, &pFuncDesc ) ;
		if (FAILED(hr))
		{
			wsprintf( szBuf, "GetFuncDesc FAILED for function #%u (%s)", iIndex, (LPSTR)HRtoString( hr ) ) ;
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
			ListBox_SetItemData( m_hwndFunctions,
							 ListBox_AddString( m_hwndFunctions, (LPSTR)rgbstrNames[0] ),
							 (LONG)iIndex ) ;
			SysFreeString( rgbstrNames[0] ) ;
		}
		else
		{
			wsprintf( szTemp, "GetNames (%lu) FAILED: %s", pFuncDesc->memid, (LPSTR)HRtoString( hr ) ) ;
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
		SetWindowText( m_hwndFuncProto, "" ) ;
		return ;
	}

	char            szRet[128] ;
	char            szFuncName[64] ;
	lstrcpy( szFuncName, "<???>" ) ;

	LPSTR           szfn = NULL ;
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
		lstrcat( szRet, " " ) ;
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
			if (NULL == (szfn = GlobalAllocPtr( GPTR, max(cNames,cParams) * (64) )))
			{
				MessageBox( m_hDlg, "GlobalAlloc Failed!", "Yikes!", MB_OK ) ;
				return ;
			}

			// put the return value first
			lstrcpy( szfn, szRet ) ;

			// rgbstrNames[0] is the name of the function
			if (cNames > 0)
			{
				lstrcat( szfn, (LPSTR)rgbstrNames[0] ) ;
				lstrcat( szfn, "(" ) ;
				lstrcpy( szFuncName, (LPSTR)rgbstrNames[0] ) ;
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
					lstrcat( szfn, "\r\n    " ) ;

				if (n < cParams)    // safety check
				{
					lstrcat( szfn, (LPSTR)g_rgszIDLFLAGS[pFuncDesc->lprgelemdescParam[n].idldesc.wIDLFlags] ) ;
					lstrcat( szfn, " " ) ;
					lstrcat( szfn, TYPEDESCtoString( &pFuncDesc->lprgelemdescParam[n].tdesc ) ) ;
				}

				if (n+1 < cNames)   // saftey check
				{
					if (n < cParams)
						lstrcat( szfn, " " ) ;
					lstrcat( szfn, (LPSTR)rgbstrNames[n+1] ) ;
					SysFreeString( rgbstrNames[n+1] ) ;
				}
				if (n + 1 == max(cNames-1,cParams))
					lstrcat( szfn, "\r\n    " );
				else
					lstrcat( szfn, ",\r\n    " ) ;
			}
			lstrcat( szfn , ")"  );
		}
		else
		{
			lstrcat( szRet, "GetNames FAILED: " ) ;
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
		wsprintf( szRet, "GetFuncDesc FAILED: %s ", (LPSTR)HRtoString( hr ) ) ;
		SetWindowText( m_hwndFuncProto, szRet ) ;
		pFuncDesc = NULL ;
	}

	if (pFuncDesc != NULL)
	{
		char szTemp[256] ;
		wsprintf( szTemp, "FUNC&DESC for %s:", (LPSTR)szFuncName ) ;
		SetWindowText( m_hwndInfoLbl, szTemp ) ;
		ListBox_ResetContent( m_hwndInfo ) ;

		DWORD   dwHelp ;
		BSTR    bstrName, bstrDoc, bstrHelpFile ;
		hr = m_pTypeInfo->GetDocumentation( pFuncDesc->memid, &bstrName, &bstrDoc,  &dwHelp, &bstrHelpFile ) ;
		if (SUCCEEDED(hr))
		{
			wsprintf( szTemp, "Name = %s", (LPSTR)(bstrName == NULL ? "n/a" : bstrName ) ) ;
			ListBox_AddString( m_hwndInfo, bstrName ) ;

			wsprintf( szTemp, "DocString = %s", (LPSTR)(bstrDoc == NULL ? "n/a" : bstrDoc) ) ;
			ListBox_AddString( m_hwndInfo, szTemp ) ;

			wsprintf( szTemp, "Help Context ID = %#08lX", (DWORD)dwHelp ) ;
			ListBox_AddString( m_hwndInfo, szTemp ) ;

			wsprintf( szTemp, "Help File = %s", (LPSTR)(bstrHelpFile == NULL ? "n/a" : bstrHelpFile) ) ;
			ListBox_AddString( m_hwndInfo, szTemp ) ;

			SysFreeString( bstrName ) ;
			SysFreeString( bstrDoc) ;
			SysFreeString( bstrHelpFile ) ;
		}
		else
		{
			wsprintf( szTemp, "GetDocumentation FAILED: %s", (LPSTR)HRtoString( hr ) ) ;
			ListBox_AddString( m_hwndInfo, szTemp ) ;
		}

		DumpFUNCDESC( m_hwndInfo, pFuncDesc, "" ) ;
		m_pTypeInfo->ReleaseFuncDesc( pFuncDesc ) ;
	}
}

void IDispDlg::DoGetVars( LPTYPEINFO lpTI, WORD cVars )
{
	char    szBuf[128] ;
	WORD    iIndex ;
	char    szTemp[80] ;
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
			wsprintf( szBuf, "GetVarDesc FAILED for variable #%u (%s)", iIndex, (LPSTR)HRtoString( hr ) ) ;
			ListBox_AddString( m_hwndVariables, szBuf ) ;
			continue ;
		}
		else
		{
			// memid\tname
			hr = lpTI->GetNames( pVarDesc->memid, rgbstrNames, 1, &cNames );
			if (SUCCEEDED( hr ))
			{
				wsprintf( szBuf, "%s (%s", (LPSTR)rgbstrNames[0],
							TYPEDESCtoString( &pVarDesc->elemdescVar.tdesc ) ) ;
				lstrcat( szBuf, ")" ) ;

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
						wsprintf( szTemp, " = %s", (LPSTR)HRtoString( hr ) ) ;
					else
					{
						wsprintf( szTemp, " = %s", (LPSTR)varValue.bstrVal ) ;
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
				wsprintf( szTemp, "GetNames (%lu) FAILED: %s", pVarDesc->memid, (LPSTR)HRtoString( hr ) ) ;
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
	int         iIndex = ListBox_GetCurSel( m_hwndVariables ) ;
	LPVARDESC   pVarDesc = NULL;
	HRESULT     hr ;
	BSTR        rgbstrNames[2] ;
	UINT        cNames ;
	char        szTemp[256] ;

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
			wsprintf( szTemp, "Var&DESC for %s:", (LPSTR)rgbstrNames[0] ) ;
			SysFreeString( rgbstrNames[0] ) ;
		}
		else
			lstrcpy( szTemp, "Var&DESC:" ) ;
		SetWindowText( m_hwndInfoLbl, szTemp ) ;

		DWORD   dwHelp ;
		BSTR    bstrName, bstrDoc, bstrHelpFile ;
		hr = m_pTypeInfo->GetDocumentation( pVarDesc->memid, &bstrName, &bstrDoc,  &dwHelp, &bstrHelpFile ) ;
		if (SUCCEEDED(hr))
		{
			wsprintf( szTemp, "Name = %s", (LPSTR)(bstrName == NULL ? "n/a" : bstrName ) ) ;
			ListBox_AddString( m_hwndInfo, bstrName ) ;

			wsprintf( szTemp, "DocString = %s", (LPSTR)(bstrDoc == NULL ? "n/a" : bstrDoc) ) ;
			ListBox_AddString( m_hwndInfo, szTemp ) ;

			wsprintf( szTemp, "Help Context ID = %#08lX", (DWORD)dwHelp ) ;
			ListBox_AddString( m_hwndInfo, szTemp ) ;

			wsprintf( szTemp, "Help File = %s", (LPSTR)(bstrHelpFile == NULL ? "n/a" : bstrHelpFile) ) ;
			ListBox_AddString( m_hwndInfo, szTemp ) ;

			SysFreeString( bstrName ) ;
			SysFreeString( bstrDoc) ;
			SysFreeString( bstrHelpFile ) ;
		}
		else
		{
			wsprintf( szTemp, "GetDocumentation FAILED: %s", (LPSTR)HRtoString( hr ) ) ;
			ListBox_AddString( m_hwndInfo, szTemp ) ;
		}

		DumpVARDESC( m_hwndInfo, pVarDesc, "" ) ;
		m_pTypeInfo->ReleaseVarDesc( pVarDesc ) ;

	}
	else
	{
		wsprintf( szTemp, "GetVarDesc FAILED: %s ", (LPSTR)HRtoString( hr ) ) ;
		ListBox_AddString( m_hwndInfo, szTemp ) ;
	}
}

void DumpTYPEDESC( HWND hwnd, TYPEDESC FAR* lptdesc, LPSTR szLabel )
{
	char szTemp[128] ;
	if (lptdesc == NULL)
	{
		wsprintf( szTemp, "%slptdesc is NULL (this is an error)", (LPSTR)szLabel ) ;
		ListBox_AddString( hwnd, szTemp ) ;
		return ;
	}

	// Dump a TYPEDESC
	wsprintf( szTemp, "%svt = %s", (LPSTR)szLabel, (LPSTR)VTtoString( lptdesc->vt ) ) ;
	ListBox_AddString( hwnd, szTemp ) ;

	if ((lptdesc->vt & 0x0FFF) == VT_PTR)
	{
		// lptdesc points to a TYPEDESC that specifies the thing pointed to
		wsprintf( szTemp, "%slptdesc->", (LPSTR)szLabel ) ;
		DumpTYPEDESC( hwnd, lptdesc->lptdesc, szTemp ) ;
		return ;
	}

	if ((lptdesc->vt & 0x0FFF) == VT_USERDEFINED)
	{
		wsprintf( szTemp, "%shreftype = %#08lX", (LPSTR)szLabel, (DWORD)lptdesc->hreftype ) ;
		ListBox_AddString( hwnd, szTemp ) ;
		return ;
	}

	if ((lptdesc->vt & 0x0FFF) == VT_CARRAY)
	{
		wsprintf( szTemp, "%slpadesc->", (LPSTR)szLabel ) ;
		DumpARRAYDESC( hwnd, lptdesc->lpadesc, szTemp ) ;
		return ;
	}

#ifdef _VT_FUNCPTR_DEFINED
	if ((lptdesc->vt & 0x0FFF) == VT_FUNCPTR)
	{
		wsprintf( szTemp, "%slpfdesc->", (LPSTR)szLabel ) ;
		DumpFUNCDESC( hwnd, lptdesc->lpadesc, szTemp ) ;
		return ;
	}
#endif
}

void DumpARRAYDESC( HWND hwnd, ARRAYDESC FAR* lpadesc, LPSTR szLabel )
{
	char szTemp[128] ;

	if (lpadesc == NULL)
	{
		wsprintf( szTemp, "%slpadesc is NULL (this is an error)", (LPSTR)szLabel ) ;
		ListBox_AddString( hwnd, szTemp ) ;
		return ;
	}

	wsprintf( szTemp, "%stdescElem.", (LPSTR)szLabel ) ;
	DumpTYPEDESC( hwnd, &lpadesc->tdescElem, szTemp ) ;

	wsprintf( szTemp, "%scDims = %u",  (LPSTR)szLabel, lpadesc->cDims ) ;
	ListBox_AddString( hwnd, szTemp ) ;
}

void DumpIDLDESC( HWND hwnd, LPIDLDESC lpidldesc, LPSTR szLabel )
{
	char szTemp[128] ;

	wsprintf(szTemp, "%swIDLFlags = %s", (LPSTR)szLabel,
			(LPSTR)g_rgszIDLFLAGS[lpidldesc->wIDLFlags] ) ;
	ListBox_AddString( hwnd, szTemp ) ;

	if (lpidldesc->bstrIDLInfo != NULL)
		wsprintf( szTemp, "%sbstrIDLInfo = %s", (LPSTR)szLabel,
				(LPSTR)lpidldesc->bstrIDLInfo ) ;
	else
		wsprintf( szTemp, "%sbstrIDLInfo = NULL", (LPSTR)szLabel ) ;
	ListBox_AddString( hwnd, szTemp ) ;
}

void DumpELEMDESC( HWND hwnd, LPELEMDESC lpelemdesc, LPSTR szLabel )
{
	char szTemp[128] ;

	wsprintf( szTemp, "%stdesc.", (LPSTR)szLabel ) ;
	DumpTYPEDESC( hwnd, &lpelemdesc->tdesc, szTemp ) ;

	wsprintf( szTemp, "%sidldesc.", (LPSTR)szLabel ) ;
	DumpIDLDESC( hwnd, &lpelemdesc->idldesc, szTemp ) ;
}

void DumpFUNCDESC( HWND hwnd, LPFUNCDESC lpfndesc, LPSTR szLabel )
{
	char szTemp[128] ;
	SHORT s ;

	// MEMBERID  memid ;
	wsprintf( szTemp, "%smemid = %#08lX", (LPSTR)szLabel, (DWORD)lpfndesc->memid ) ;
	ListBox_AddString( hwnd, szTemp ) ;

#ifdef SCODE_FIX
// BUGBUG:  As of the 10/7/93 build of the OLE 2.01 DLLs (23.618) the
// cScodes and lprgscode members of FUNCDESC are
//  (a) not initialized by the default implementation of ITypeInfo::GetFuncDesc
//      This causes them to contain bogus values when GetFuncDesc is called
//      on applications that use the default implementation (e.g. the Spoly2
//      sample.
//
//  (b) not marshalled at all in the proxy code for ITypeInfo.  Applications
//      which implement GetFuncDesc themselves still won't work because of this
//      bug.  (an example of this is WinWord 6.0).
//
// The code bracketed here by #ifdef SCODE_FIX is the only code in the world
// today that actually looks at these two members.  These two members are
// intended for use in future Microsoft operating systems (e.g. Cairo).
//

	// SCODE FAR* lprgscode;
	if (lpfndesc->cScodes > 0)
	{
		for (s = 0 ; s < lpfndesc->cScodes ; s++)
		{
			if (lpfndesc->lprgscode == NULL)
				wsprintf( szTemp, "%slprgscode = NULL (this is an error)", (LPSTR)szLabel ) ;
			else
				wsprintf( szTemp, "%slprgscode[%u] = %s", (LPSTR)szLabel, s,
					 (LPSTR)HRtoString( ResultFromScode( lpfndesc->lprgscode[s] ) ) ) ;
			ListBox_AddString( hwnd, szTemp ) ;
		}
	}

	AssertSz( !(lpfndesc->cScodes < -1), "lpfndesc->sCodes < -1 when it shouldn't be" ) ;

#endif

	// ELEMDESC FAR* lprgelemdescParam;  /* array of parameter types */
	for (s = 0 ; s < lpfndesc->cParams ; s++)
	{
		wsprintf( szTemp, "%slprgelemdescParam[%u].", (LPSTR)szLabel, s ) ;
		DumpELEMDESC( hwnd, &lpfndesc->lprgelemdescParam[s], szTemp ) ;
	}

	// FUNCKIND  funckind ;
	wsprintf( szTemp, "%sfunckind = %s", (LPSTR)szLabel, (LPSTR)g_rgszFUNCKIND[lpfndesc->funckind] ) ;
	ListBox_AddString( hwnd, szTemp ) ;

	// INVOKEKIND   invkind ;
	if (lpfndesc->invkind == INVOKE_FUNC)
		wsprintf( szTemp, "%sinvkind = FUNC", (LPSTR)szLabel ) ;
	else if (lpfndesc->invkind == INVOKE_PROPERTYGET)
		wsprintf( szTemp, "%sinvkind = PROPERTYGET", (LPSTR)szLabel ) ;
	else if (lpfndesc->invkind == INVOKE_PROPERTYPUT)
		wsprintf( szTemp, "%sinvkind = PROPERTYPUT", (LPSTR)szLabel ) ;
	else if (lpfndesc->invkind == INVOKE_PROPERTYPUTREF)
		wsprintf( szTemp, "%sinvkind = PROPERTYPUTREF", (LPSTR)szLabel ) ;
	ListBox_AddString( hwnd, szTemp ) ;

	// CALLCONV callconv;
	wsprintf( szTemp, "%scallconv = %s", (LPSTR)szLabel, (LPSTR)g_rgszCALLCONV[lpfndesc->callconv] ) ;
	ListBox_AddString( hwnd, szTemp ) ;

	// SHORT cParams;
	wsprintf( szTemp, "%scParams = %d", (LPSTR)szLabel, lpfndesc->cParams ) ;
	ListBox_AddString( hwnd, szTemp ) ;

	// SHORT cParamsOpt;
	wsprintf( szTemp, "%scParamsOpt = %d", (LPSTR)szLabel, lpfndesc->cParamsOpt ) ;
	ListBox_AddString( hwnd, szTemp ) ;

	// SHORT oVft;
	wsprintf( szTemp, "%soVft = %d", (LPSTR)szLabel, lpfndesc->oVft ) ;
	ListBox_AddString( hwnd, szTemp ) ;

	// SHORT cScodes;
	wsprintf( szTemp, "%scScodes = %d", (LPSTR)szLabel, lpfndesc->cScodes ) ;
	ListBox_AddString( hwnd, szTemp ) ;

	// ELEMDESC elemdescFunc;
	wsprintf( szTemp, "%selemdescFunc.", (LPSTR)szLabel ) ;
	DumpELEMDESC( hwnd, &lpfndesc->elemdescFunc, szTemp ) ;

	// WORD wFuncFlags;
	wsprintf( szTemp, "%swFuncFlags = %u", (LPSTR)szLabel, lpfndesc->wFuncFlags ) ;
	ListBox_AddString( hwnd, szTemp ) ;

}

void DumpVARDESC( HWND hwnd, LPVARDESC lpvardesc, LPSTR szLabel )
{
	char szTemp[128] ;

	// MEMBERID memid;
	wsprintf( szTemp, "%smemid = %#08lX", (LPSTR)szLabel, (DWORD)lpvardesc->memid ) ;
	ListBox_AddString( hwnd, szTemp ) ;

	// WORD wVarFlags;
	wsprintf( szTemp, "%swVarFlags = %s", (LPSTR)szLabel,
				(LPSTR)(lpvardesc->wVarFlags == VARFLAG_FREADONLY ? "READONLY" :
			"NOT READONLY") ) ;
	ListBox_AddString( hwnd, szTemp ) ;

	// VARKIND varkind;
	wsprintf( szTemp, "%svarkind = %s", (LPSTR)szLabel, (LPSTR)g_rgszVARKIND[lpvardesc->varkind] ) ;
	ListBox_AddString( hwnd, szTemp ) ;

	// ELEMDESC elemdescVar;
	wsprintf( szTemp, "%selemdescVar.", (LPSTR)szLabel ) ;
	DumpELEMDESC( hwnd, &lpvardesc->elemdescVar, szTemp ) ;

	// union { ULONG oInst;  VARIANT FAR* lpvarValue;  }
	if (lpvardesc->varkind == VAR_PERINSTANCE)
	{
		wsprintf( szTemp, "%soInst = %lu", (LPSTR)szLabel, lpvardesc->oInst ) ;
	}
	else if (lpvardesc->varkind == VAR_CONST)
	{
		// NEED:  This just prints out the type.  We can get the value
		// too.  Need to write at VARIANTtoString() function.
		//
		VARIANT varValue ;
		VariantInit( &varValue ) ;
		HRESULT hr = VariantChangeType( &varValue, lpvardesc->lpvarValue, 0, VT_BSTR ) ;
		if (FAILED(hr))
			wsprintf( szTemp, "%slpvarValue = %s = %s", (LPSTR)szLabel,
					(LPSTR)VTtoString( lpvardesc->lpvarValue->vt ), (LPSTR)HRtoString( hr ) ) ;
		else
		{
			wsprintf( szTemp, "%slpvarValue = %s = %s",  (LPSTR)szLabel,
					(LPSTR)VTtoString( lpvardesc->lpvarValue->vt ), (LPSTR)varValue.bstrVal ) ;
			SysFreeString( varValue.bstrVal ) ;
		}

		ListBox_AddString( hwnd, szTemp ) ;
	}
}

// end of idisp.cpp
