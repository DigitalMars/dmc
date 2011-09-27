// idataobj.h
//
// Header file for the IDataObject interface viewer.
//
// Copyright (c) 1993-1995 Microsoft Corporation, All Rights Reserved.
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

//#if defined(WIN32) && !defined(_UNICODE)
//    #define CImpIAdviseSink CImpIAdviseSinkA
//    #define fnIDataObjectDlg fnIDataObjectDlgA
//    #define fnEditSubclass fnEditSubclassA
//    #define CIDataObjectDlg CIDataObjectDlgA
//    #define DisplayIDataObject DisplayIDataObjectA
//#endif

STDAPI DisplayIDataObject( HWND hwndParent, LPDATAOBJECT lpDO, LPIID lpiid, LPTSTR lpszName ) ;

class CImpIAdviseSink ;

#define CCHOUTPUTMAX        8192
#define CLINESMAX           500
#define WM_OUTPUTBUFFERHASDATA      (WM_USER+1000)

/////////////////////////////////////////////////////////////////////////////
// CIDataObjectDlg dialog
//
extern "C"
BOOL CALLBACK fnIDataObjectDlg( HWND hDlg, UINT uiMsg, WPARAM wParam, LPARAM lParam ) ;

class FAR CIDataObjectDlg
{
    friend BOOL CALLBACK fnIDataObjectDlg( HWND hDlg, UINT uiMsg, WPARAM wParam, LPARAM lParam ) ;
    friend class CImpIAdviseSink ;

public:

    CIDataObjectDlg( HWND hwnd, LPDATAOBJECT lpDO, LPIID lpiid, LPTSTR lpszName ) ;
    ~CIDataObjectDlg() ;

    int DoModal( void ) ;

    LPDATAOBJECT    m_lpDO ;
    LPIID           m_lpiid ;
    LPTSTR          m_lpszName ;

    FORMATETC       m_fetc ;
    DWORD           m_advf ;
    BOOL            m_fDoOnGetDataPosted ;

    HWND        m_hWndParent ;
    HWND        m_hDlg ;

    HWND        m_btnDoGetData ;
    HWND        m_btnSetupAdvise ;
    HWND        m_lbGetData ;
    HWND        m_edtAdvise ;
    TCHAR       m_szOutput[CCHOUTPUTMAX] ;
    METAFILEPICT m_MetaFile ;

    UINT        m_cchOutput ;
    UINT        m_cLinesOutput ;

    HWND        m_chkUpdateDisplay ;
    BOOL        m_fUpdateDisplay ;
    HWND        m_chkPrimeFirst ;

    HWND        m_lbFmtEtc ;

    HWND        m_chkDump ;

    DWORD       m_dwTime ;
    DWORD       m_cOnDataChanges ;

    CImpIAdviseSink FAR* m_pSink ;
    DWORD               m_dwConn ;

// Implementation
protected:
    BOOL DoIDataObject( UINT nLevel, LPDATAOBJECT pI ) ;
    BOOL DoIEnumFormatEtc( UINT nLevel, LPENUMFORMATETC pI ) ;

    int AddItem( UINT nLevel, LPTSTR sz, LPVOID lpData, UINT uiType, int cColumns, LPCOLUMNSTRUCT  rgCol ) ;
    int AddItem( UINT nLevel, LPTSTR sz, LPVOID lpData, UINT uiType ) ;
    int AddItem( UINT nLevel, LPTSTR sz, LPVOID lpData ) ;

    BOOL OnInitDialog();

    void OnDoGetData() ;
    HRESULT GotData( LPFORMATETC lpfetc, LPSTGMEDIUM lpstm ) ;
    void OnSetupAdvise() ;
    void OnKillAdvise() ;

    void OnSize(UINT nType, int cx, int cy);
    void OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct);
    void OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct);
    void OnDblClkFormatEtc();
    void OnSelChangeFormatEtc();
    void OnDestroy();
    BOOL  WriteToOutput( LPTSTR lpsz ) ;
    void OnOutputBufferHasData() ;
};


#endif // _IDATAOBJ_H_
