// idataobj.h
//
// Header file for the IDispatch interface viewer.
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
#ifndef _IDISP_H_
#define _IDISP_H_


/////////////////////////////////////////////////////////////////////////////
// IDispDlg dialog

extern "C"
BOOL CALLBACK __export fnIDispDlg( HWND hDlg, UINT uiMsg, WPARAM wParam, LPARAM lParam ) ;

class _far IDispDlg
{
friend BOOL CALLBACK __export fnIDispDlg( HWND hDlg, UINT uiMsg, WPARAM wParam, LPARAM lParam ) ;

// Construction
public:
	IDispDlg( HWND hwnd, LPUNKNOWN lpUnk, LPIID lpiid, LPSTR lpszName ) ;
	~IDispDlg() ;
	int DoModal( void ) ;

	// Input parameters (caller allocated)
	LPIID       m_piid ;
	LPSTR       m_pszName ;
	LPDISPATCH  m_pDispatch ;
	LPTYPEINFO  m_pTypeInfo ;
	LPTYPELIB   m_pTypeLib ;

	// Dialog management
	HWND        m_hWndParent ;
	HWND        m_hDlg ;

	HWND        m_hwndTypeInfoCount;
	HWND        m_hwndTypeInfo;
	HWND        m_hwndTypeAttr;
	HWND        m_hwndFunctionsLbl;
	HWND        m_hwndFunctions;
	HWND        m_hwndVariablesLbl;
	HWND        m_hwndVariables;
	HWND        m_hwndFuncProto ;
	HWND        m_hwndFuncProtoLbl ;
	HWND        m_hwndInfo;
	HWND        m_hwndInfoLbl;
	HWND        m_hwndTypeInfoInfo ;
	HWND        m_btnToFile ;

// Implementation
protected:
	void DoGetInfo( ) ;

	BOOL DoTypeAttr( LPTYPEINFO  pTI ) ;
	void DoGetFunctions( LPTYPEINFO lpTI, WORD cFuncs ) ;
	void DoGetVars( LPTYPEINFO lpTI, WORD cVars ) ;

	BOOL OnInitDialog();
	void OnSize(UINT nType, int cx, int cy);
	void OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct);
	void OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct);
	void OnDestroy();
	void OnSelChangeTypeInfo();
	void OnSelChangeFunctions();
	void OnSelChangeVariables();
	void OnToFile() ;
};

typedef IDispDlg FAR* LPIDISPDLG ;

#endif // _IDISP_H_
