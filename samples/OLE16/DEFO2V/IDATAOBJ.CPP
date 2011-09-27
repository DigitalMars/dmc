// idataobj.cpp
//
// Implementation file for the IDataObject interface viewer.
//
// Copyright (c) 1993 Microsoft Corporation, All Rights Reserved.
//
// Charlie Kindel, Program Manager
// Microsoft Vertical Developer Relations
// August 7, 1993
//
// InterNet   :  ckindel@microsoft.com
// CompuServe :  >INTERNET:ckindel@microsoft.com
//
// Revisions:
//  August 7, 1993  cek     First implementation.
//

#include "precomp.h"
#include "defo2v.h"
#include "idataobj.h"

static HFONT       g_hFont ;
static HFONT       g_hFontBold ;
static UINT        g_cyFont ;

// DisplayIDataObject
//
// This is the "DisplayInterface" or "Interface Viewer" function for the
// IDataObject interface.   This implementation simply pops up a dialog
// box that has an ownerdraw listbox in it.
//
// The listbox is filled with the FORMATETCs that are enumerated through
// the IEnumFORMATETC interface that is retrieved from IDataObject::EnumFormatEtc
//
extern "C"
HRESULT WINAPI _export DisplayIDataObject( HWND hwndParent, LPDATAOBJECT lpDO, LPIID lpiid, LPSTR lpszName )
{
	HRESULT hr = NULL ;

	IDataObjDlg dlg( hwndParent, lpDO, lpiid, lpszName ) ;
	dlg.DoModal() ;

	return hr ;
}

/////////////////////////////////////////////////////////////////////////////
// IDataObjDlg dialog

IDataObjDlg::IDataObjDlg(HWND hwnd, LPDATAOBJECT lpDO,LPIID lpiid, LPSTR lpszName )
{
	m_lbGet = NULL ;
	m_hWndParent = hwnd ;
	m_lpDO = lpDO ;
	m_lpiid = lpiid ;
	m_lpszName = lpszName ;
}

IDataObjDlg::~IDataObjDlg()
{
}

int IDataObjDlg::DoModal( void )
{
	return DialogBoxParam( g_hInst, MAKEINTRESOURCE( IDD_IDATAOBJDLG ), m_hWndParent, (DLGPROC)fnIDataObjDlg, (LONG)this ) ;
}

BOOL WINAPI __export fnIDataObjDlg( HWND hDlg, UINT uiMsg, WPARAM wParam, LPARAM lParam )
{
	LPIDATAOBJDLG pIDOD =(LPIDATAOBJDLG)GetWindowLong( hDlg, DWL_USER ) ;

	switch (uiMsg)
	{
		case WM_INITDIALOG:
			pIDOD=(LPIDATAOBJDLG)lParam ;
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

		default:
		   return FALSE ;
	}
	return TRUE ;
}


/////////////////////////////////////////////////////////////////////////////
// IDataObjDlg message handlers

BOOL IDataObjDlg::OnInitDialog()
{
	m_lbGet = GetDlgItem( m_hDlg, IDC_IFACE_PROPS ) ;

	TEXTMETRIC  tm ;
	HDC hdc = GetDC(NULL);
	g_hFont = ReallyCreateFont( hdc, "MS Sans Serif", "Regular", 8, 0 ) ;
	g_hFontBold = ReallyCreateFont( hdc, "MS Sans Serif", "Bold", 8, 0 ) ;
	g_hFont = SelectObject( hdc, g_hFont ) ;
	GetTextMetrics( hdc, &tm ) ;
	g_hFont = SelectObject( hdc, g_hFont ) ;
	ReleaseDC( NULL, hdc ) ;
	g_cyFont = tm.tmHeight + tm.tmExternalLeading ;

	SetWindowFont( m_lbGet, g_hFont, TRUE ) ;

	if (IsEqualCLSID( IID_IEnumFORMATETC, *m_lpiid ))
		DoIEnumFormatEtc( 0, (LPENUMFORMATETC)m_lpDO) ;
	else if (IsEqualCLSID( IID_IDataObject, *m_lpiid ))
	{
		DoIDataObject( 0, (LPDATAOBJECT)m_lpDO ) ;
	}
	else
	{
		MessageBox( m_hDlg, "Wrong IID", "Error", MB_OK ) ;
	}

	DlgCenter( m_hDlg, m_hWndParent, FALSE ) ;
	RECT rc ;
	GetWindowRect( m_hDlg, &rc ) ;
	SetWindowPos( m_hDlg, NULL, rc.left, rc.top, rc.right - rc.left + 1,
				  rc.bottom - rc.top +1, SWP_NOMOVE|SWP_NOZORDER | SWP_NOACTIVATE ) ;
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void IDataObjDlg::OnDestroy()
{
	int c = ListBox_GetCount( m_lbGet ) ;
	for (int i = 0 ; i < c ; i++)
	{
		LPITEMDATA lpID = (LPITEMDATA)ListBox_GetItemData( m_lbGet, i ) ;
		if (lpID)
		{
			if (lpID->uiType == I_COLUMNHEAD)
				delete lpID->rgCol ;
			delete lpID ;
		}
	}

	if (g_hFont)
		DeleteObject( g_hFont );

	if (g_hFontBold)
		DeleteObject (g_hFontBold) ;
}

void IDataObjDlg::OnSize(UINT nType, int cx, int cy)
{
	if (m_lbGet && !IsWindow( m_lbGet ) )
		return ;

	RECT    rc ;
	RECT    rcWnd ;
	GetClientRect( m_hDlg, &rcWnd ) ;
	GetWindowRect( m_lbGet, &rc ) ;
	MapWindowPoints( NULL, m_hDlg, (POINT FAR*)&rc, 2 ) ;
	rc.left = rcWnd.left + 5 ;
	SetWindowPos( m_lbGet, NULL, rcWnd.left + 5, rc.top, cx - 10, cy - rc.top - 5,
				SWP_NOZORDER | SWP_NOACTIVATE ) ;

}


void IDataObjDlg::OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDIS )
{
	if (lpDIS->itemID == LB_ERR)
		return ;

	LPITEMDATA      lpID ;
	COLORREF        rgbBack ;
	RECT            rcFocus ;
	BOOL            fSelected ;
	int             x, y, cy ;
	char            szItem[128] ;

	lpID = (LPITEMDATA)lpDIS->itemData ;
	rcFocus = lpDIS->rcItem ;
	ListBox_GetText( m_lbGet, lpDIS->itemID, szItem ) ;

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
		g_hFontBold = SelectObject( lpDIS->hDC, g_hFontBold ) ;

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
		g_hFontBold = SelectObject( lpDIS->hDC, g_hFontBold ) ;

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

void IDataObjDlg::OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct)
{
	lpMeasureItemStruct->itemHeight = g_cyFont ;
}

void IDataObjDlg::OnDblclk()
{
}

void IDataObjDlg::OnSelchange()
{
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
BOOL IDataObjDlg::DoIDataObject( UINT nLevel, LPDATAOBJECT pIDataObject )
{
	LPENUMFORMATETC pEFE ;
	HRESULT     hr ;

	hr = pIDataObject->EnumFormatEtc( DATADIR_GET, &pEFE ) ;
	if (SUCCEEDED( hr ))
	{
		AddItem( nLevel, "EnumFormatEtc(DATA_DIRGET) returned a pIEnumFORMATETC:", I_LABEL ) ;
		DoIEnumFormatEtc( nLevel+1, pEFE ) ;
		pEFE->Release() ;
	}
	else
		AddItem( nLevel, "EnumFormatEtc(DATA_DIRGET) failed to return an enumerator.", I_LABEL ) ;

	AddItem( nLevel, " ", I_LABEL ) ;

	hr = pIDataObject->EnumFormatEtc( DATADIR_SET, &pEFE ) ;
	if (SUCCEEDED( hr ))
	{
		AddItem( nLevel, "EnumFormatEtc(DATA_DIRSET) returned a pIEnumFORMATETC:", I_LABEL ) ;
		DoIEnumFormatEtc( nLevel+1, pEFE ) ;
		pEFE->Release() ;
	}
	else
		AddItem( nLevel, "EnumFormatEtc(DATA_DIRSET) failed to return an enumerator.", I_LABEL ) ;

	return TRUE ;
}

// This member takes a pointer to an IEnumFORMATETC and enumerates
// the FORMATETC structures avaialbe, inserting them into our
// listbox.
//
BOOL IDataObjDlg::DoIEnumFormatEtc( UINT nLevel, LPENUMFORMATETC pIFormatEtc )
{
	LPCOLUMNSTRUCT  rgCol = new COLUMNSTRUCT[5] ;
	UINT    cxChar ;
	HDC     hdc  = GetDC( NULL) ;

	// We use the "ColumnTextOut" code in util.c to create nice
	// columns in our list box.
	//
	g_hFontBold = SelectObject( hdc, g_hFontBold ) ;
	cxChar = GetTextMetricsCorrectly( hdc, NULL ) ;
	rgCol[0].nLeft = 0 ;
	rgCol[0].nRight = cxChar * lstrlen( "CF_METAFILEPICT_" ) ;
	rgCol[0].uiFlags = DT_LEFT ;

	rgCol[1].nLeft = rgCol[0].nRight + cxChar ;
	rgCol[1].nRight = rgCol[1].nLeft + cxChar * lstrlen( "0000:0000_" ) ;
	rgCol[1].uiFlags = DT_LEFT ;

	rgCol[2].nLeft = rgCol[1].nRight + cxChar ;
	rgCol[2].nRight = rgCol[2].nLeft + cxChar * lstrlen( "THUMBNAIL_" ) ;
	rgCol[2].uiFlags = DT_LEFT ;

	rgCol[3].nLeft = rgCol[2].nRight + cxChar ;
	rgCol[3].nRight = rgCol[3].nLeft + cxChar * lstrlen( "0000:0000_" ) ;
	rgCol[3].uiFlags = DT_LEFT ;

	rgCol[4].nLeft = rgCol[3].nRight + cxChar ;
	rgCol[4].nRight = rgCol[4].nLeft + cxChar * lstrlen( "_HGLOBAL_" ) ;
	rgCol[4].uiFlags = DT_LEFT ;

	g_hFontBold = SelectObject( hdc, g_hFontBold ) ;
	ReleaseDC( NULL, hdc ) ;

	AddItem( nLevel, "cfFormat\tptd\tdwAspect\tlindex\ttymed", I_COLUMNHEAD, 5, rgCol ) ;
	FORMATETC       fetc ;
	char            sz[64] ;
	ULONG           ulFE ; // number returned

	while (pIFormatEtc->Next( 1, &fetc, &ulFE ) == S_OK)
	{
		switch( fetc.cfFormat )
		{
			case CF_TEXT: lstrcpy( sz, "CF_TEXT\t" ) ; break ;
			case CF_BITMAP: lstrcpy( sz, "CF_BITMAP\t" ) ; break ;
			case CF_METAFILEPICT: lstrcpy( sz, "CF_METAFILEPICT\t" ) ; break ;
			case CF_SYLK: lstrcpy( sz, "CF_SYLK\t" ) ; break ;
			case CF_DIF: lstrcpy( sz, "CF_DIF\t" ) ; break ;
			case CF_TIFF: lstrcpy( sz, "CF_TIFF\t" ) ; break ;
			case CF_OEMTEXT: lstrcpy( sz, "CF_OEMTEXT\t" ) ; break ;
			case CF_DIB: lstrcpy( sz, "CF_DIB\t" ) ; break ;
			case CF_PALETTE: lstrcpy( sz, "CF_PALETTE\t" ) ; break ;
			case CF_PENDATA: lstrcpy( sz, "CF_PENDATA\t" ) ; break ;
			case CF_RIFF: lstrcpy( sz, "CF_RIFF\t" ) ; break ;
			case CF_WAVE: lstrcpy( sz, "CF_WAVE\t" ) ; break ;
			default:
				if (!GetClipboardFormatName( (UINT)fetc.cfFormat, sz, 63 ))
					wsprintf( sz, "%#08lX", (DWORD)fetc.cfFormat ) ;
				lstrcat( sz, "\t" ) ;
			break ;
		}

		char szTemp[32] ;
		wsprintf( szTemp, "%04lX:%04lX\t", (DWORD)HIWORD( fetc.ptd ), (DWORD)LOWORD( fetc.ptd ) ) ;
		lstrcat( sz, szTemp ) ;

		switch( fetc.dwAspect )
		{
			case DVASPECT_CONTENT: lstrcat( sz, "CONTENT\t" ) ; break ;
			case DVASPECT_THUMBNAIL: lstrcat( sz, "THUMBNAIL\t" ) ; break ;
			case DVASPECT_ICON: lstrcat( sz, "ICON\t" ) ; break ;
			case DVASPECT_DOCPRINT: lstrcat( sz, "DOCPRINT\t" ) ; break ;
			default:
				wsprintf( szTemp, "%08lX\t", (DWORD)fetc.dwAspect ) ;
				lstrcat( sz, szTemp ) ;
			break ;
		}

		wsprintf( szTemp, "%04lX:%04lX\t", (DWORD)HIWORD( fetc.lindex ), (DWORD)LOWORD( fetc.lindex ) ) ;
		lstrcat( sz, szTemp ) ;

		switch( fetc.tymed )
		{
			case TYMED_HGLOBAL: lstrcat( sz, "HGLOBAL\t" ) ; break ;
			case TYMED_FILE: lstrcat( sz, "FILE\t" ) ; break ;
			case TYMED_ISTREAM: lstrcat( sz, "ISTREAM\t" ) ; break ;
			case TYMED_ISTORAGE: lstrcat( sz, "ISTORAGE\t" ) ; break ;
			case TYMED_GDI: lstrcat( sz, "GDI\t" ) ; break ;
			case TYMED_MFPICT: lstrcat( sz, "MFPICT\t" ) ; break ;
			case TYMED_NULL: lstrcat( sz, "NULL\t" ) ; break ;
			default:
				wsprintf( szTemp, "%08lX\t", (DWORD)fetc.tymed ) ;
				lstrcat( sz, szTemp ) ;
			break ;
		}

		// now insert into list
		AddItem( nLevel, sz, I_COLUMNENTRY, 5, rgCol ) ;
	}

	return TRUE ;
}

// AddItem does a ListBox_AddString() plus a ListBox_SetItemData().  The
// item data that is set tells our owner-draw code what 'level' this
// item is at (indentation) and whether it is a label (bold), column
// head (column info is stored), or column entry (no column info is
// stored, but previous column head is used in WM_DRAWITEM).
//
int IDataObjDlg::AddItem( UINT nLevel, LPSTR sz, UINT uiType, int cColumns, LPCOLUMNSTRUCT  rgCol )
{
	int i ;
	LPITEMDATA      lpID ;
/*
	{
		LPSTR p = sz.GetBuffer(256) ;
		if (p)
		{
			TRACE( "AddItem: %s\n", (LPSTR)p ) ;
			sz.ReleaseBuffer() ;
		}
		else
			TRACE( "AddItem: STRING IS NULL!\n" ) ;
	}
*/
	i = ListBox_AddString( m_lbGet,  sz ) ;
	lpID = new ITEMDATA ;
	lpID->uiType = uiType ;
	lpID->cColumns = cColumns ;
	lpID->rgCol = rgCol ;
	lpID->nLevel = nLevel ;
	ListBox_SetItemData( m_lbGet, i, (DWORD)lpID ) ;
	return i ;
}

int IDataObjDlg::AddItem( UINT nLevel, LPSTR sz, UINT uiType )
{
	return AddItem( nLevel, sz, uiType, 0, NULL ) ;
}

int IDataObjDlg::AddItem( UINT nLevel, LPSTR sz )
{
	return AddItem( nLevel, sz, I_NORMAL, 0, NULL ) ;
}
