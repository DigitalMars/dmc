// HttpSvrDoc.cpp : implementation of the CHttpSvrDoc class
//
// This is a part of the Microsoft Foundation Classes C++ library.
// Copyright (C) 1996 Microsoft Corporation
// All rights reserved.
//
// This source code is only intended as a supplement to the
// Microsoft Foundation Classes Reference and related
// electronic documentation provided with the library.
// See these sources for detailed information regarding the
// Microsoft Foundation Classes product.
//
// Note that your filters probably won't have an #ifdef like this one.
// The project file for this sample allows you to build the sample as
// a statically linked regular MFC DLL (that is, with _AFXDLL defined)
// or as a DLL that doesn't use MFC classes aside from the ISAPI 
// support classes (that is, without _AFXDLL defined).

#include "stdafx.h"

#include "HttpSvr.h"
#include "HttpDoc.h"
#include "Request.h"
#include "GenPage.h"
#include "NamePage.h"
#include "RootPage.h"
#include "Listen.h"
#include "mainfrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CHttpSvrDoc

IMPLEMENT_DYNCREATE(CHttpSvrDoc, CDocument)

BEGIN_MESSAGE_MAP(CHttpSvrDoc, CDocument)
	//{{AFX_MSG_MAP(CHttpSvrDoc)
	ON_COMMAND(IDM_SVR_OPTIONS, OnSvrOptions)
	ON_COMMAND(ID_FILE_RESTART, OnFileRestart)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CHttpSvrDoc construction/destruction

CHttpSvrDoc::CHttpSvrDoc()
{
	m_pListen = NULL;
}

CHttpSvrDoc::~CHttpSvrDoc()
{
	// get rid of any lingering requests, JIC.....
	while ( !m_reqList.IsEmpty() )
	{
		CRequest* pRequest = (CRequest*)(m_reqList.RemoveTail());
		pRequest->Release();
	}
}

BOOL CHttpSvrDoc::OnNewDocument()
{
	CDocument::OnNewDocument();

	// zero the hits counter....
	m_nTotalHits = 0;
	// assign default values....
	m_strServer = ((CHttpSvrApp*)AfxGetApp())->m_strDefSvr;
	m_uPort = PORT_HTTP;
	m_nSvrName = 0;
	m_bLoggingOn = TRUE;
	m_bAllowListing = TRUE;
	m_bListIcon = TRUE;

	// get the default root path....
	m_strRoot.LoadString( IDS_DEF_ROOT );
	m_strRoot = SEPCHAR + m_strRoot;
	// ensure no final separator character....
	if ( m_strRoot[m_strRoot.GetLength()-1] == SEPCHAR )
		m_strRoot = m_strRoot.Left( m_strRoot.GetLength()-1 );
	// make into a full path string....
	GetFullPathName( m_strRoot, MAX_PATH, 
		m_strRoot.GetBuffer(MAX_PATH), NULL);
	m_strRoot.ReleaseBuffer();

	// see if we need to automatically disable the icons
	// in folder listings....
	CString strAdmin = m_strRoot + SEPCHAR + "SvrAdmin";
	DWORD dwAttr = GetFileAttributes( strAdmin );
	if ( dwAttr == (DWORD)(-1) || (dwAttr & FILE_ATTRIBUTE_DIRECTORY) == 0 )
		m_bListIcon = FALSE; // don't allow icons

	StartListening();
	SetTitle( NULL );
	return TRUE;
}

BOOL CHttpSvrDoc::OnOpenDocument(LPCTSTR lpszPathName) 
{
	BOOL bOk = FALSE;
	if ( CDocument::OnOpenDocument(lpszPathName) )
	{
		if ( (bOk = StartListening()) == FALSE )
			SetModifiedFlag( TRUE );
	}

	return bOk;
}

/////////////////////////////////////////////////////////////////////////////
// CHttpSvrDoc serialization

void CHttpSvrDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		ar << m_strRoot;
		ar << m_strServer;
		ar << m_timeStarted;
		ar << m_uPort;
		ar << m_nSvrName;
		ar << m_bLoggingOn;
		ar << m_bListIcon;
		ar << m_bAllowListing;
	}
	else
	{
		ar >> m_strRoot;
		ar >> m_strServer;
		ar >> m_timeStarted;
		ar >> m_uPort;
		ar >> m_nSvrName;
		ar >> m_bLoggingOn;
		ar >> m_bListIcon;
		ar >> m_bAllowListing;

		if ( m_nSvrName == 0 )
			m_strServer = ((CHttpSvrApp*)AfxGetApp())->m_strDefSvr;
	}
}

/////////////////////////////////////////////////////////////////////////////
// CHttpSvrDoc diagnostics

#ifdef _DEBUG
void CHttpSvrDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CHttpSvrDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CHttpSvrDoc commands

void CHttpSvrDoc::DocHit( CRequest* pRequest )
{
	// save the request object....
	pRequest->AddRef();
	m_reqList.AddTail( pRequest );
	// increment the total hit count....
	++m_nTotalHits; 
	// tell the view we got a new doc hit....
	UpdateAllViews( NULL, HINT_DOCHIT, pRequest );
}

BOOL CHttpSvrDoc::IdleProc( LONG lCount )
{
	BOOL bMore = FALSE;
	// if there's still requests in the list....
	if ( !m_reqList.IsEmpty() )
	{
		// pull off the first item....
		CRequest* pRequest = (CRequest*)(m_reqList.RemoveHead());
		// if it's done....
		if ( pRequest->m_bDone )
		{
			// process it for the stats....
			ExtractStats( pRequest );
			// done with it....
			pRequest->Release();
			// more idle needed if not empty....
			bMore = !m_reqList.IsEmpty();
		}
		else
		{
			// move to end of list....
			m_reqList.AddTail( pRequest );
			// still need to come back....
			bMore = TRUE;
		}
	}
	return bMore;
}

void CHttpSvrDoc::ExtractStats( CRequest* pRequest )
{
	// increment the status' group count....
	if ( pRequest->m_uStatus >= 100 && pRequest->m_uStatus < 600 )
		++m_aReStats[ pRequest->m_uStatus/100 - 1 ];
	else
		++m_aReStats[ STATUS_SERVERERR ]; // JIC

	// see if we want to write entry in log file....
	if ( m_bLoggingOn )
	{
		CString strLog;
		strLog.Format( IDS_STATUSFMT, pRequest->m_uStatus );
		strLog += pRequest->m_strHost + '\t'
			+ pRequest->m_timeReq.Format( IDS_TIMEFORMAT ) + '\t'
			+ pRequest->m_strURL + ' ';
		if ( !pRequest->m_strArgs.IsEmpty() )
			strLog += CString("? ") + pRequest->m_strArgs;

		strLog += '\n';
		CString strLogName = m_strTitleBase + pRequest->m_timeReq.Format(IDS_LOGFILEFMT);
		CStdioFile fileLog( strLogName,
			CFile::modeWrite | CFile::modeCreate |
			CFile::modeNoTruncate | CFile::shareExclusive );
		// see if file is new and we need to write the header....
		if ( fileLog.SeekToEnd() == 0 )
		{
			// write page header.....
			CString strHeader;
			strHeader.Format( IDS_LOG_HEADER, pRequest->m_timeReq.Format(IDS_LONG_DATE) );
			fileLog.Write( strHeader, strHeader.GetLength() );
		}
		fileLog.Write( strLog, strLog.GetLength() );
		fileLog.Close();
	}
}


void CHttpSvrDoc::OnCloseDocument() 
{
	StopListening();		
	CDocument::OnCloseDocument();
}

void CHttpSvrDoc::SetTitle(LPCTSTR lpszTitle) 
{
	CString strTitle;
	if ( lpszTitle != NULL )
		m_strTitleBase = lpszTitle;

	if ( m_strServer.IsEmpty() )
		strTitle = m_strTitleBase;
	else if ( m_pListen == NULL )
		strTitle.Format( IDS_INVALID, m_strTitleBase );
	else
	{
		if ( m_uPort != PORT_HTTP )
			strTitle.Format( IDS_DOCTITLE, m_strTitleBase, m_strServer, m_uPort );
		else
			strTitle.Format( IDS_DOCTITLE_NOPORT, m_strTitleBase, m_strServer );
	}

	CDocument::SetTitle( strTitle );
}

void CHttpSvrDoc::OnSvrOptions() 
{
	// add and initialize the general page....
	CPropertySheet propOptions( IDS_OPTIONS );
	CGenPage genPage( this );
	genPage.m_bListIcon = m_bListIcon;
	genPage.m_bAllowListing = m_bAllowListing;
	genPage.m_bLoggingOn = m_bLoggingOn;
	propOptions.AddPage( &genPage );

	// add and initialize the root dir page....
	CRootPage rootPage( this );
	rootPage.m_strRoot = m_strRoot;
	propOptions.AddPage( &rootPage );

	// add and initialize the name page....
	CNamePage namePage( this );
	namePage.m_nNameSetting = m_nSvrName;
	namePage.m_uPort = m_uPort;
	if ( m_nSvrName )
		namePage.m_strName = m_strServer;
	propOptions.AddPage( &namePage );

	m_bResetListen = FALSE;
	propOptions.DoModal();
	if ( m_bResetListen )
	{
		StartListening();
		SetTitle( NULL );
	}
}

void CHttpSvrDoc::StopListening( void )
{
	if ( m_pListen != NULL )
	{
		m_pListen->Close();
		delete m_pListen;
		m_pListen = NULL;
	}
}

BOOL CHttpSvrDoc::StartListening( void )
{
	BOOL bOk = FALSE;
	StopListening();
	m_pListen = new CListenSocket( this );
	if ( m_pListen )
	{
		if ( m_pListen->Create( m_uPort, SOCK_STREAM, FD_ACCEPT ) )
			bOk = m_pListen->Listen();

		if ( !bOk )
		{
			CString strMsg;
			int nErr = m_pListen->GetLastError();
			if ( nErr == WSAEADDRINUSE )
				strMsg.Format( IDS_LISTEN_INUSE, m_uPort );
			else
				strMsg.Format( IDS_LISTEN_ERROR, m_uPort );

			AfxMessageBox( strMsg, MB_OK|MB_ICONSTOP );
			m_pListen->Close();
			delete m_pListen;
			m_pListen = NULL;
		}
	}
	else
		AfxMessageBox( IDS_CANT_LISTEN, MB_OK|MB_ICONSTOP );

	m_timeStarted = CTime::GetCurrentTime();

	return bOk;
}



void CHttpSvrDoc::OnFileRestart() 
{
	// this will close the connection if it's already open
	// before starting again....
	StartListening();
}
