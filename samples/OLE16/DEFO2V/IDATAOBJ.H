// idataobj.h
//
// Header file for the IDataObject interface viewer.
//
// Copyright (c) 1993 Microsoft Corporation, All Rights Reserved.
//
// Charlie Kindel, Program Manager
// Microsoft Vertical Developer Relations
// August 6, 1993
//
// InterNet   :  ckindel@microsoft.com
// CompuServe :  >INTERNET:ckindel@microsoft.com
//
// Revisions:
//  August 6, 1993  cek     First implementation.
//

#ifndef _IDATAOBJ_H_
#define _IDATAOBJ_H_

/////////////////////////////////////////////////////////////////////////////
// IDataObjDlg dialog
//
extern "C"
BOOL WINAPI __export fnIDataObjDlg( HWND hDlg, UINT uiMsg, WPARAM wParam, LPARAM lParam ) ;

class _far IDataObjDlg
{
friend BOOL WINAPI __export fnIDataObjDlg( HWND hDlg, UINT uiMsg, WPARAM wParam, LPARAM lParam ) ;

// Construction                    friend
public:
	IDataObjDlg( HWND hwnd, LPDATAOBJECT lpDO, LPIID lpiid, LPSTR lpszName ) ;
	~IDataObjDlg() ;
	int DoModal( void ) ;

	LPDATAOBJECT    m_lpDO ;
	LPIID           m_lpiid ;
	LPSTR           m_lpszName ;

	HWND        m_hWndParent ;
	HWND        m_hDlg ;

	HWND        m_lbGet ;

// Implementation
protected:
	BOOL DoIDataObject( UINT nLevel, LPDATAOBJECT pI ) ;
	BOOL DoIEnumFormatEtc( UINT nLevel, LPENUMFORMATETC pI ) ;

	int AddItem( UINT nLevel, LPSTR sz, UINT uiType, int cColumns, LPCOLUMNSTRUCT  rgCol ) ;
	int AddItem( UINT nLevel, LPSTR sz, UINT uiType ) ;
	int AddItem( UINT nLevel, LPSTR sz ) ;

	BOOL OnInitDialog();
	void OnSize(UINT nType, int cx, int cy);
	void OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct);
	void OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct);
	void OnDblclk();
	void OnSelchange();
	void OnDestroy();
};

typedef IDataObjDlg FAR * LPIDATAOBJDLG ;

#endif // _IDATAOBJ_H_
