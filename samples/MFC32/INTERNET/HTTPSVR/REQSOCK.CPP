// ReqSock.cpp : implementation of the CRequestSocket class
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
#include "Http.h"
#include "ReqSock.h"
#include "Request.h"
#include "resource.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNCREATE(CRequestSocket, CAsyncSocket)

#define CRLF	"\x0d\x0a"

SECURITY_ATTRIBUTES g_sa = {sizeof(SECURITY_ATTRIBUTES),NULL,TRUE};

CRequestSocket::CRequestSocket( void )
{
}

CRequestSocket::CRequestSocket( CHttpSvrDoc* pDoc )
{
#ifdef IMPL_CGI
	m_pThread = NULL;
	m_pCancel = NULL;
#endif // IMPL_CGI
	m_bKilled = FALSE;
	m_nRefs = 1;
	m_reqStatus = REQ_REQUEST;
	m_buf.SetSize( 1024 );
	m_cbOut = 0;
	m_hFile = INVALID_HANDLE_VALUE;
	m_pRequest = NULL;
	m_pDoc = pDoc;
}

CRequestSocket::~CRequestSocket( void )
{
	// JIC....
#ifdef IMPL_CGI
	if ( m_pCancel )
	{
		if ( m_pThread )
		{
			DWORD dwCode;
			// signal a cancel if still running....
			if ( ::GetExitCodeThread( m_pThread->m_hThread, &dwCode )
				&& dwCode == STILL_ACTIVE )
			{
				// signal a cancel....
				m_pCancel->SetEvent();
				// wait for the thread to die....
				WaitForSingleObject( m_pThread->m_hThread, INFINITE );
			}
			// kill the object...
			delete m_pThread;
		}
		delete m_pCancel;
	}
#endif // IMPL_CGI

	if ( m_hFile )
		CloseHandle( m_hFile );

	if ( m_pRequest )
	{
		// release our hold on the request object....
		m_pRequest->m_bDone = TRUE;
		m_pRequest->Release();
	}
}

void CRequestSocket::OnReceive(int nErrorCode)
{
	if ( m_pRequest == NULL )
	{
		// new request....
		m_pRequest = new CRequest;
		m_bKeepOpen = m_bWantKeepOpen = FALSE;
	}
	if ( m_pRequest )
	{
		// get the bytes....
		int nBytes = Receive( m_buf.GetData(), m_buf.GetSize() );
		if ( nBytes != SOCKET_ERROR )
		{
			int ndx = 0;
			switch ( m_reqStatus )
			{
			case REQ_REQUEST:
			case REQ_HEADER:
				while( GetLine( m_buf, nBytes, ndx ) == TRUE )
				{
					if ( !m_strLine.IsEmpty() )
						ProcessLine();
					else
					{
						m_reqStatus = REQ_BODY;
						break;
					}
				}
				// break if we're not looking for the body....
				if ( m_reqStatus != REQ_BODY )
					break;
				// stop if no body sent....
				if ( !BodySent() )
				{
					m_reqStatus = REQ_DONE;
					break;
				}
				// else fall through....
			case REQ_BODY:
				AddToBody( nBytes, ndx );
				break;
			}
			if ( m_reqStatus == REQ_DONE )
			{
				m_buf.SetSize(0);
				if ( !StartResponse() )
					AsyncSelect( FD_WRITE | FD_CLOSE );
			}
		}
		else
			nBytes = GetLastError();
	}
	else
	{
		// couldn't allocate request object....
		ShutDown( both );
		m_bKilled = TRUE;
		Release();
	}
}

void CRequestSocket::OnSend(int nErrorCode)
{
	int nBytes = Send( m_buf.GetData(), m_cbOut );
	if ( nBytes == SOCKET_ERROR )
	{
		if ( GetLastError() != WSAEWOULDBLOCK )
		{
			ShutDown( both );
			m_bKilled = TRUE;
			Release();
		}
		else
			AsyncSelect( FD_WRITE | FD_CLOSE );
	}
	else if ( nBytes < m_cbOut )
	{
		// still got some left....
		m_buf.RemoveAt( 0, nBytes );
		m_cbOut -= nBytes;
		// adjust the bytes-sent value for the request....
		m_pRequest->m_cbSent += nBytes;
		// set up for next write....
		AsyncSelect( FD_WRITE | FD_CLOSE );
	}
	else
	{
		// adjust the bytes-sent value for the request....
		m_pRequest->m_cbSent += nBytes;
		// if we're not done with the file....
		if ( m_hFile != INVALID_HANDLE_VALUE )
		{
			DWORD dwRead = 0;
			// read next chunk....
			ReadFile( m_hFile, m_buf.GetData(), 
				m_buf.GetSize(), &dwRead, NULL );
			if ( dwRead > 0 )
				m_cbOut = dwRead;
			else
			{
				// no more data to send....
				CloseHandle( m_hFile );
				m_hFile = INVALID_HANDLE_VALUE;
			}
		}
		// see if we need to keep going....
		if ( m_hFile != INVALID_HANDLE_VALUE )
			AsyncSelect( FD_WRITE | FD_CLOSE );
		else
		{
			// eh, we're outta here....
			ShutDown( both );
			m_bKilled = TRUE;
			Release();
		}
	}
}

void CRequestSocket::OnClose(int nErrorCode)
{
	m_bKilled = TRUE;
	Release();
}

BOOL CRequestSocket::GetLine( const CByteArray& bytes, int nBytes, int& ndx )
{
	BOOL bLine = FALSE;
	while ( bLine == FALSE && ndx < nBytes )
	{
		char ch = (char)(bytes.GetAt( ndx ));
		switch( ch )
		{
		case '\r': // ignore
			break;
		case '\n': // end-of-line
			bLine = TRUE;
			break;
		default:   // other....
			m_strLine += ch;
			break;
		}
		++ndx;
	}
	return bLine;
}

void CRequestSocket::ProcessLine( void )
{
	int ndx;
	switch( m_reqStatus )
	{
	case REQ_REQUEST:
		ndx = m_strLine.Find( ' ' );
		if ( ndx != -1 )
		{
			m_pRequest->m_strMethod = m_strLine.Left( ndx );
			m_strLine = m_strLine.Mid( ndx+1 );
			m_strLine.TrimLeft();
			ndx = m_strLine.Find( ' ' );
			if ( ndx == -1 )
			{
				m_pRequest->m_strURL = m_strLine;
				m_pRequest->m_strURL.TrimRight();
				m_reqStatus = REQ_SIMPLE;
			}
			else
			{
				m_pRequest->m_strURL = m_strLine.Left( ndx );
				m_pRequest->m_strVersion = m_strLine.Mid( ndx+1 );
				m_pRequest->m_strVersion.TrimLeft();
			}
			// check for execution arguments....
			ndx = m_pRequest->m_strURL.Find( '?' );
			if ( ndx != -1 )
			{
				// yup; save the args....
				m_pRequest->m_strArgs = m_pRequest->m_strURL.Mid( ndx+1 );
				// strip from file name....
				m_pRequest->m_strURL = m_pRequest->m_strURL.Left( ndx );
				m_pRequest->m_dwExecute = CRequest::APP_EXECUTE;
			}

			// change any "%xx"s to the appropriate char....
			m_pRequest->m_strURL = Decode( m_pRequest->m_strURL );
		}
		m_reqStatus = REQ_HEADER;
		break;
	case REQ_HEADER:
		ndx = m_strLine.Find( ':' );
		if ( ndx != -1 )
		{
			CString strName = m_strLine.Left( ndx );
			CString strValue = m_strLine.Mid( ndx+1 );
			strName.MakeLower();
			strValue.TrimLeft();
			m_pRequest->m_mapHeaders.SetAt( strName, strValue );
		}
		break;
	};
	m_strLine.Empty();
}

BOOL CRequestSocket::BodySent( void )
{
	BOOL bSent = FALSE;
	CString strValue = m_pRequest->GetHeaderValue( "Content-Length" );
	if ( !strValue.IsEmpty() )
	{
		m_pRequest->m_cbBody = atoi( strValue );
		bSent = TRUE;
	}
	return bSent;
}

void CRequestSocket::AddToBody( int nBytes, int ndx )
{
	// save the buffer size....
	int nOldSize = m_buf.GetSize();
	// get rid of old stuff; append to body data....
	m_buf.RemoveAt( 0, ndx );
	m_buf.SetSize( nBytes - ndx );
	m_pRequest->m_baBody.Append( m_buf );
	// restore the buffer size....
	m_buf.SetSize( nOldSize );
	// see if we're done....
	if ( m_pRequest->m_baBody.GetSize() >= m_pRequest->m_cbBody )
	{
		m_pRequest->m_baBody.SetSize( m_pRequest->m_cbBody );
		m_reqStatus = REQ_DONE;
	}
}

BOOL CRequestSocket::StartResponse( void )
{
	BOOL bWait = FALSE;
	CString strFile;
	UINT uPort;
	// save the host address....
	GetPeerName( m_pRequest->m_strHost, uPort );
	// switch on the method....
	if ( m_pRequest->m_cbBody == 0 && 
		m_pRequest->m_strMethod.CompareNoCase( "GET" ) == 0 )
	{
		FindTarget( strFile );
		if( m_pRequest->m_uStatus == 0 )
		{
			if ( m_pRequest->m_dwExecute )
				bWait=StartSvrApp();
			else
			{
				if ( StuffHeading() )
					StartTargetStuff();
			}
		}
	}
	else if ( m_pRequest->m_cbBody == 0 && m_reqStatus == REQ_DONE &&
		m_pRequest->m_strMethod.CompareNoCase( "HEAD" ) == 0 )
	{
		FindTarget( strFile );
		if( m_pRequest->m_uStatus == 0 )
		{
			if ( m_pRequest->m_dwExecute )
				bWait=StartSvrApp();
			else
			{
				StuffHeading();
				// we don't send the file for HEAD reqs....
				if ( m_hFile != INVALID_HANDLE_VALUE)
				{
					CloseHandle( m_hFile );
					m_hFile = INVALID_HANDLE_VALUE;
				}
			}
		}
	}
	else if ( m_pRequest->m_cbBody > 0 && m_reqStatus == REQ_DONE &&
		m_pRequest->m_strMethod.CompareNoCase( "POST" ) == 0 )
	{
		// assume an executable....
		m_pRequest->m_dwExecute = CRequest::APP_EXECUTE;
		FindTarget( strFile );
		if ( m_pRequest->m_uStatus == 0 )
		{
			bWait=StartSvrApp();
		}
	}
	else
		StuffError( IDS_STATUS_NOTIMPL );

	// notify the active view of the hit....
	m_pDoc->DocHit( m_pRequest );
	return bWait;
}

BOOL CRequestSocket::FindTarget( CString& strFile )
{
	BOOL bFound = FALSE;
	strFile = m_pRequest->m_strURL;
	// change from URL to local file system path....
	if ( URLtoPath( strFile ) )
	{
		CString strExtra; // extra path info
		m_pRequest->m_dwAttr = GetFileAttributes( strFile );
		if ( m_pRequest->m_dwAttr != -1 )
			bFound = TRUE;
		else
		{
 			// rip off the last portion....
 			strExtra = StripLast( strFile );
 			while( !strFile.IsEmpty() )
 			{
 				// anything there?
 				m_pRequest->m_dwAttr = GetFileAttributes( strFile );
 				if ( m_pRequest->m_dwAttr != -1 )
 				{
 					// found something; better not be a folder....
 					if( (m_pRequest->m_dwAttr&FILE_ATTRIBUTE_DIRECTORY) == 0 )
 						bFound = TRUE;
 					break;
 				}
 				// rip off the next portion....
 				strExtra = StripLast( strFile ) + strExtra;
 			}
		}

		if ( bFound )
		{
			// strip any trailing SEPCHAR....
			if ( strFile.GetAt( strFile.GetLength()-1) == SEPCHAR )
				m_pRequest->m_strFullPath = strFile.Left( strFile.GetLength()-1 );
			else
				m_pRequest->m_strFullPath = strFile;

			// see if we need to set the extra path info....
			if ( !strExtra.IsEmpty() )
			{
				m_pRequest->m_strPathInfo = strExtra;
				if ( URLtoPath( strExtra ) )
 					m_pRequest->m_strPathTranslated = strExtra;
			}

			// if it's a folder, see if we can redirect to
			// on of the default docs or apps....
			if ( m_pRequest->m_dwAttr & FILE_ATTRIBUTE_DIRECTORY )
			{
				// check for existence of a default doc or app....
				if ( !CheckDefault( IDS_DEFAULTDOC, FALSE ) )
					CheckDefault( IDS_DEFAULTAPP, TRUE );
			}
			else if ( m_pRequest->m_dwExecute && !IsSvrApp() )
			{
				StuffError( IDS_STATUS_BADREQUEST );
			}
		}
		else
			StuffError( IDS_STATUS_NOTFOUND );
	}
	else
		StuffError( IDS_STATUS_BADREQUEST );

	return bFound;
}

BOOL CRequestSocket::URLtoPath( CString& strFile )
{
	BOOL bLegal = FALSE;
	CString& strRoot = m_pDoc->m_strRoot;

	// start with the root, append the abs path....
	CString strTemp = strRoot + strFile;
#ifdef _MAC
	MacifyPath( strTemp );
#endif // _MAC
	// now canonicalize it....
	DWORD dwSize = GetFullPathName( strTemp, MAX_PATH, strFile.GetBuffer(MAX_PATH+1), NULL );
	strFile.ReleaseBuffer();
	
	// get the full path okay?
	if ( dwSize )
	{
		int cchRoot = strRoot.GetLength();
		int cchFile = strFile.GetLength();
		// must be the same or longer than the root....
		if ( cchRoot == cchFile )
		{
			// must be exactly the same....
			if ( strRoot.Compare( strFile ) == 0 )
				bLegal = TRUE;
		}
		else if ( cchRoot < cchFile )
		{
			// must have the root as the base....
			if ( strRoot.Compare( strFile.Left(cchRoot) ) == 0
				&& strFile.GetAt( cchRoot ) == SEPCHAR )
				bLegal = TRUE;
		}
	}

	return bLegal;
}

BOOL CRequestSocket::PathToURL( CString& strFile )
{
	int ndx;
	// a local reference to the root....
	CString& strRoot = m_pDoc->m_strRoot;
	// change all SEPCHARs to forward slashes....
	while ( (ndx=strFile.Find( SEPCHAR )) != -1 )
		strFile = strFile.Left( ndx ) + '/' + strFile.Mid(ndx+1);
	// now add the prefix and server, and cut the root....
	CString strPort;
	UINT uPort = m_pDoc->m_uPort;
	if ( uPort != PORT_HTTP )
		strPort.Format( ":%d", uPort );

	strFile = CString("http://") 
		+ m_pDoc->m_strServer
		+ strPort
		+ strFile.Mid(strRoot.GetLength());

	return TRUE;
}

int CRequestSocket::StuffString( const CString& strData )
{
	int nLen = strData.GetLength()*sizeof(TCHAR);
	// make sure there's enough room....
	if ( m_cbOut + nLen > m_buf.GetSize() )
	{
		int nChunks = nLen/1024 + 1;
		m_buf.SetSize( m_cbOut + nChunks*1024 );
	}
	// copy the data....
	MoveMemory( m_buf.GetData() + m_cbOut, (LPCSTR)strData, nLen );
	m_cbOut += nLen;
	// return amount of space left....
	return (m_buf.GetSize() - m_cbOut);
}

int CRequestSocket::StuffString( UINT uId )
{
	CString str;
	str.LoadString( uId );
	return StuffString( str );
}

int CRequestSocket::StuffStatus( const CString& strStatus )
{
	CString strVer = "HTTP/1.0 ";
	StuffString( strVer );
	StuffString( strStatus );
	StuffString( CRLF );

	// stuff the server name....
	CString strServer;
	if ( strServer.LoadString( IDS_SERVER_NAME ) && !strServer.IsEmpty() )
		StuffHeader( "Server", strServer );

	// stuff the date....
	return StuffHeader( "Date", GetHttpDate() );
}

int CRequestSocket::StuffStatus( UINT uStatus )
{
	CString strStatus;
	strStatus.LoadString( uStatus );
	// save the status for this request....
	m_pRequest->m_uStatus = uStatus;
	// stuff the HTTP status line....
	return StuffStatus( strStatus );
}

int CRequestSocket::StuffError( UINT uMsg )
{
	StuffStatus( uMsg );
	return StuffString( CRLF );
}

int CRequestSocket::StuffHeader( CString strName, CString strValue )
{
	StuffString( strName );
	StuffString( ": " );
	StuffString( strValue );
	return StuffString( CRLF );
}

int CRequestSocket::StuffHeader( CString strName, int nValue )
{
	CString strValue;
	StuffString( strName );
	StuffString( ": " );
	strValue.Format( "%d", nValue );
	StuffString( strValue );
	return StuffString( CRLF );
}

BOOL CRequestSocket::StuffHeading( void )
{
	BOOL bContinue = FALSE;
	if ( m_pRequest->m_dwAttr & FILE_ATTRIBUTE_HIDDEN )
	{
		// never show hidden files....
		StuffError( IDS_STATUS_FORBIDDEN );
	}
	else if ( m_pRequest->m_dwAttr & FILE_ATTRIBUTE_DIRECTORY )
	{
#ifndef _MAC // Mac version doesn't support directory listings....
		if ( m_pDoc->m_bAllowListing )
		{
			// create a directory listing....
			StuffStatus( IDS_STATUS_OK );
			StuffString( CRLF );
			bContinue = TRUE;
		}
		else
#endif // !_MAC
			StuffError( IDS_STATUS_FORBIDDEN );
	}
#ifdef IMPL_CGI
	else if ( m_hFile != INVALID_HANDLE_VALUE )
	{
		// cgi's output file will be opened already....
		CString strStatus, strHeaders;
		// loop until we find a blank line....
		DWORD dwRead = 0;
		CByteArray baFile;
		baFile.SetSize( 1024 );
		// read next chunk....
		BOOL bRead = ReadFile( m_hFile, baFile.GetData(), 
			baFile.GetSize(), &dwRead, NULL );
		while ( dwRead > 0 )
		{
			int ndx = 0;
			while( GetLine( baFile, dwRead, ndx ) == TRUE )
			{
				BOOL bSave = TRUE;
				// stuff any non-empty lines.....
				if ( m_strLine.IsEmpty() )
				{
					// we found our empty line;
					// back up to where we left off....
					DWORD dwPos = SetFilePointer( m_hFile, 
						ndx - dwRead, 
						NULL, FILE_CURRENT );

					// and we're off....
					bContinue = TRUE;
					break;
				}
				else
				{
					int nPos = m_strLine.Find( ':' );
					if ( nPos != -1 )
					{
						CString strName = m_strLine.Left( nPos );
						strName.TrimLeft();
						strName.TrimRight();
						CString strVal  = m_strLine.Mid( nPos+1 );
						strVal.TrimLeft();
						strVal.TrimRight();
						if ( strName.CompareNoCase("Status") == 0 )
						{
							strStatus = strVal;
							bSave = FALSE;
						}
						else if ( strName.CompareNoCase("Location") == 0 )
						{
							strStatus.LoadString( IDS_STATUS_MOVEDTEMP );
						}
					}
				}

				// save the header (if we want to)....
				if ( bSave )
					strHeaders += m_strLine + CRLF;

				m_strLine.Empty();
			}
			// read next chunk if we're not done....
			if ( bContinue )
				break;
			else
				ReadFile( m_hFile, baFile.GetData(), 
					baFile.GetSize(), &dwRead, NULL );
		}
		if ( strStatus.IsEmpty() )
			StuffStatus( IDS_STATUS_OK );
		else
			StuffStatus( strStatus );

		// stuff the headers....
		StuffString( strHeaders );
		// stuff the blank line....
		StuffString( CRLF );
	}
#endif // IMPL_CGI
	else
	{
		// open the file....
		m_hFile = CreateFile( m_pRequest->m_strFullPath, 
			GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 
			FILE_ATTRIBUTE_NORMAL | FILE_FLAG_SEQUENTIAL_SCAN,
			NULL );
		if ( m_hFile != INVALID_HANDLE_VALUE )
		{
			if ( m_reqStatus != REQ_SIMPLE )
			{
				CTime timeIfMod;
				CString strIfMod = m_pRequest->GetHeaderValue( "If-Modified-Since" );
				if ( strIfMod.GetLength() > 0 &&
					FromHttpTime( strIfMod, timeIfMod ) &&
					!IfModSince( timeIfMod ) )
				{
					// eh, it hasn't been modified....
					StuffStatus( IDS_STATUS_NOTMODIFIED );
					// don't need it anymore....
					CloseHandle( m_hFile );
					m_hFile = INVALID_HANDLE_VALUE;
				}
				else
				{
					// send it off....
					StuffStatus( IDS_STATUS_OK );	
					// any other header info....
					StuffFileType();
					StuffHeader( "Content-length", GetFileSize( m_hFile, NULL ) );
					// get the last modified time....
					FILETIME ft;
					if ( GetFileTime( m_hFile, NULL, NULL, &ft ) )
					{
						StuffHeader( "Last-Modified", GetHttpDate( &ft ) );
					}
					bContinue = TRUE;
				}
				// blank line....
				StuffString( CRLF );
			}
			else
				bContinue = TRUE;
		}
		else
		{
			// couldn't open; try again later....
			StuffError( IDS_STATUS_SVCUNAVAIL );
		}
	}
	return bContinue;
}

void CRequestSocket::StartTargetStuff( void )
{
	if ( m_hFile != INVALID_HANDLE_VALUE)
	{
		DWORD dwRead = 0;
		// read the first chunk....
		ReadFile( m_hFile, m_buf.GetData() + m_cbOut,
			m_buf.GetSize()-m_cbOut, &dwRead, NULL );
		if ( dwRead > 0 )
			m_cbOut += dwRead;
		else
		{
			// nothing read.... close the file....
			CloseHandle( m_hFile );
			m_hFile = INVALID_HANDLE_VALUE;
		}
	}
#ifndef _MAC
	else if ( m_pRequest->m_dwAttr & FILE_ATTRIBUTE_DIRECTORY )
		StuffListing();
#endif // !_MAC
	else
		StuffString( CRLF );
}

#ifndef _MAC
void CRequestSocket::StuffListing( void )
{
	BOOL bRoot = FALSE;
	BOOL bIcons = m_pDoc->m_bListIcon;
	CString strIcon;
	CString strLine = CString("http://") 
		+ m_pDoc->m_strServer 
		+ m_pRequest->m_strURL;
	CString strDir = m_pRequest->m_strURL;
	CString strMask = m_pRequest->m_strFullPath;

	// make sure URL ends in a slash....
	if ( strDir.GetAt( strDir.GetLength()-1 ) != '/' )
		strDir += '/';
	// is this the server's root folder?
	else if ( strDir.Compare( "/" ) == 0 )
		bRoot = TRUE;

	// create the file search mask....
	AddFile( strMask, IDS_DIRMASK );
	StuffString( IDS_CONTENTS_PRE );
	StuffString( strLine );
	StuffString( IDS_CONTENTS_POST );
	if ( bRoot )
		StuffString( IDS_CONTENTS_DESC );

	if ( bIcons )
		strIcon.LoadString( IDS_ICON_BLANK );
	strLine.Format( IDS_CONTENTS_HEADING, strIcon );
	StuffString( strLine );

	int nFiles = 0;

	WIN32_FIND_DATA fd;
	// find the first file that matches the mask....
	HANDLE fh = FindFirstFile( strMask, &fd );
	if ( fh != INVALID_HANDLE_VALUE )
	{
		// create a line for the found file....
		nFiles += StuffListingFile( &fd, strDir, bIcons );
		// loop through all other files....
		while ( FindNextFile( fh, &fd ) )
			nFiles += StuffListingFile( &fd, strDir, bIcons );
	}

	if ( nFiles == 0 )
		StuffString( IDS_CONTENTS_EMPTY );

	StuffString( IDS_CONTENTS_FOOTER );
	// only add the parent link if there is one....
	if ( !bRoot )
	{
		if ( bIcons )
			strIcon.LoadString( IDS_ICON_PARENT );
		strLine.Format( IDS_CONTENTS_PARENT, strIcon );
		StuffString( strLine );
	}
	// add the note and end it....
	StuffString( IDS_CONTENTS_NOTE );
	StuffString( CRLF );
}

int CRequestSocket::StuffListingFile( WIN32_FIND_DATA* pfd, const CString& strDir, BOOL bIcons )
{
	int nFile = 0;
	// don't include '.', '..' or hidden files....
	if ( lstrcmp( pfd->cFileName, "." ) && lstrcmp( pfd->cFileName, ".." )
		&& (pfd->dwFileAttributes & FILE_ATTRIBUTE_HIDDEN) == 0 )
	{
		CString strSize, strIcon = "";
		CString strLine, strFile = pfd->cFileName;
		CTime timeFile( pfd->ftLastWriteTime );
		BOOL bFolder = ((pfd->dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0);
		if ( bIcons && bFolder )
			strIcon.LoadString( IDS_ICON_FOLDER );
		else if ( bIcons )
			strIcon.LoadString( IDS_ICON_FILE );

		// create the link string....
		CString strLink = strDir + strFile;
		// make sure spaces are replaced with '%20'...
		int ndx;
		while ( (ndx=strLink.Find(' ')) != -1 )
			strLink = strLink.Left(ndx) + "%20" + strLink.Mid( ndx+1 );

		// format the size string.... 
		if ( bFolder )
			strSize = "  Folder";
		else if ( pfd->nFileSizeHigh > 0 )
			strSize = "   &gt; 4GB"; // yeah, right.
		else if ( pfd->nFileSizeLow < 1024 )
			strSize = "    &lt; 1K";
		else
			strSize.Format( "%7dK", pfd->nFileSizeLow/1024 );

		strLine.Format( IDS_CONTENTS_FORMAT,
			timeFile.Format( IDS_FILETIMEFMT ),
			strSize, strLink, strIcon, strFile );
		StuffString( strLine );
		nFile = 1;
	}
	return nFile;
}
#endif // !_MAC

BOOL CRequestSocket::StartSvrApp( void )
{
#ifdef IMPL_CGI
	if ( m_pRequest->m_dwExecute != CRequest::APP_ISAPI )
		return CGIStart();
	else
	{
		StuffError( IDS_STATUS_NOTIMPL );
		return FALSE;
	}
#else //  IMPL_CGI
	StuffError( IDS_STATUS_NOTIMPL );
	return FALSE;
#endif // IMPL_CGI
}

#ifdef IMPL_CGI
BOOL CRequestSocket::CGIStart( void )
{
	BOOL bOk = FALSE;
	// get the temp path...
	CString strTempPath;
	GetTempPath( MAX_PATH, strTempPath.GetBuffer(MAX_PATH) );
	strTempPath.ReleaseBuffer();
	// create a temporary file for the output....
	CString strTempName;
	GetTempFileName( strTempPath, "CGI", 0, strTempName.GetBuffer(MAX_PATH) );
	strTempName.ReleaseBuffer();
	m_hFile = CreateFile( strTempName, GENERIC_READ|GENERIC_WRITE,
		FILE_SHARE_READ|FILE_SHARE_WRITE, &g_sa,
		CREATE_ALWAYS, FILE_ATTRIBUTE_TEMPORARY|FILE_FLAG_DELETE_ON_CLOSE, NULL );
	if ( m_hFile != INVALID_HANDLE_VALUE )
	{
		// create the cancel event....
		m_pCancel = new CEvent;
		if ( m_pCancel )
		{
			// make sure the event is reset....
			m_pCancel->ResetEvent();
			// create the CGI thread suspended....
			m_pThread = AfxBeginThread( (AFX_THREADPROC)CGIThread, 
				(LPVOID)this, THREAD_PRIORITY_NORMAL, 0,
				CREATE_SUSPENDED, NULL );
			if ( m_pThread )
			{
				// don't self-destruct (we must delete)....
				m_pThread->m_bAutoDelete = FALSE;
				// resume...
				m_pThread->ResumeThread();
				bOk = TRUE;
			}
		}
	}

	if ( bOk == FALSE )
	{
		StuffError( IDS_STATUS_SVRERROR );
		if( m_hFile != INVALID_HANDLE_VALUE )
		{ // JIC....
			CloseHandle( m_hFile );
			m_hFile = INVALID_HANDLE_VALUE;
		}
	}
	return bOk;
}

void AddEnvVar( CString& strEnv, CString strName, CString strVal )
{
	// add the name=val pair to the env in alphabetical order....
	strEnv += strName + '=' + strVal + '\a';
}

UINT CGIThread( LPVOID pvParam )
{
	CRequestSocket* pReqSock = (CRequestSocket*)pvParam;
	CRequest* pRequest = pReqSock->m_pRequest;
	BOOL bOk = FALSE;
	DWORD dwErr;
	HANDLE hWritePipe, hReadPipe;
	// create a pipe we'll use for STDIN....
	if ( CreatePipe( &hReadPipe, &hWritePipe, &g_sa, 0 ) )
	{
		// get the command line together....
		CString strCmdLine = pRequest->m_strFullPath 
			+ ' '
			+ Decode( pRequest->m_strArgs, TRUE );
		// get the directory....
		CString strDir = pRequest->m_strFullPath;
		int ndx = strDir.ReverseFind( SEPCHAR );
		// assume we found it....
		strDir = strDir.Left( ndx+1 );

		// create an environment for the CGI process....
		DWORD dwCreateFlags = 0;
#ifdef UNICODE
		dwCreateFlags = CREATE_UNICODE_ENVIRONMENT;
#endif // UNICODE
		CEnvironment cEnv;

		CString strValue;
		strValue.LoadString( IDS_SERVER_NAME );
		cEnv.Add( "SERVER_SOFTWARE", strValue );
		cEnv.Add( "SERVER_NAME", pReqSock->m_pDoc->m_strServer );
		cEnv.Add( "GATEWAY_INTERFACE", "CGI/1.1" );
		cEnv.Add( "SERVER_PROTOCOL", "HTTP/1.0" );
		strValue.Format( "%d", pReqSock->m_pDoc->m_uPort );
		cEnv.Add( "SERVER_PORT", strValue );

		cEnv.Add( "REQUEST_METHOD", pRequest->m_strMethod );
		cEnv.Add( "SCRIPT_NAME", pRequest->m_strURL );
		cEnv.Add( "QUERY_STRING", pRequest->m_strArgs );
		cEnv.Add( "REMOTE_ADDR", pRequest->m_strHost );
		if ( pRequest->m_cbBody > 0 )
		{
			cEnv.Add( "CONTENT_LENGTH", pRequest->GetHeaderValue("Content-Length") );
			cEnv.Add( "CONTENT_TYPE", pRequest->GetHeaderValue("Content-Type") );
		}
		if ( !pRequest->m_strPathInfo.IsEmpty() )
		{
			cEnv.Add( "PATH_INFO", pRequest->m_strPathInfo );
			cEnv.Add( "PATH_TRANSLATED", pRequest->m_strPathTranslated );
		}

		// all the passed headers prefixed with "HTTP_"....
		POSITION pos = pRequest->m_mapHeaders.GetStartPosition();
		while ( pos != NULL )
		{
			// get the name/value pair....
			CString strName, strValue;
			pRequest->m_mapHeaders.GetNextAssoc( pos, strName, strValue );
			HeaderToEnvVar( strName );
			// set the environment variable....
			cEnv.Add( strName, strValue );
		}

		// create the process....
		LPVOID pEnv = (LPVOID)cEnv.GetBlock();
		PROCESS_INFORMATION pi;
		STARTUPINFO si = {0};
		si.cb = sizeof(si);
		si.dwFlags = STARTF_USESHOWWINDOW | STARTF_USESTDHANDLES;
		si.wShowWindow = SW_HIDE;
		si.hStdInput = hReadPipe;
		si.hStdOutput = pReqSock->m_hFile;
		si.hStdError = pReqSock->m_hFile;
		bOk = CreateProcess( NULL, strCmdLine.GetBuffer(1), 
			NULL, NULL, TRUE,
			dwCreateFlags, pEnv, 
			strDir, &si, &pi );
		strCmdLine.ReleaseBuffer();
		// if created....
		if ( bOk )
		{
			// release our hold on the thread....
			CloseHandle( pi.hThread );
			// send the body of the post to the stdin....
			if ( pRequest->m_cbBody > 0 )
			{
				DWORD dwWritten = 0;
				WriteFile( hWritePipe, pRequest->m_baBody.GetData(), 
					pRequest->m_cbBody, &dwWritten, NULL );
			}
			// wait for either cancel or process done....
			HANDLE aHandles[2];
			aHandles[0] = pi.hProcess;
			aHandles[1] = pReqSock->m_pCancel->m_hObject;
			if ( WaitForMultipleObjects( 2, aHandles, FALSE, INFINITE ) == WAIT_OBJECT_0 )
			{
				// process finished; notify main thread....
				AfxGetApp()->m_pMainWnd->PostMessage( WSM_CGIDONE, 0, (LPARAM)pReqSock );
			}
			else
			{
				// canceled or some other error....
				bOk = FALSE;
			}
			// close our hold on it....
			CloseHandle( pi.hProcess );
		}
		else
			dwErr = GetLastError();

		// close the stdin pipe....
		CloseHandle( hWritePipe );
		CloseHandle( hReadPipe );
		delete pEnv;
	}
	if ( bOk == FALSE && pReqSock->m_hFile != INVALID_HANDLE_VALUE )
	{ // JIC....
		CloseHandle( pReqSock->m_hFile );
		pReqSock->m_hFile = INVALID_HANDLE_VALUE;
	}

	return (bOk?0:1);
}

void CRequestSocket::CGIDone( void )
{
	if ( !m_bKilled )
	{
		// flush the temp file's buffers....
		BOOL bSucceed = FlushFileBuffers( m_hFile );
		// go to start of file....
		DWORD dwPos = SetFilePointer( m_hFile, 0, NULL, FILE_BEGIN );
		// output the header....
		StuffHeading();
		if ( m_pRequest->m_strMethod.Compare( "HEAD" ) )
			StartTargetStuff();
		else
		{
			CloseHandle( m_hFile );
			m_hFile = INVALID_HANDLE_VALUE;
		}
		AsyncSelect( FD_WRITE | FD_CLOSE );
	}
	else
	{
		CloseHandle( m_hFile );
		m_hFile = INVALID_HANDLE_VALUE;
	}
}

void HeaderToEnvVar( CString& strVar )
{
	int ndx;
	// make upper case, change '-' to '_', and prefix....
	strVar.MakeUpper();
	while( (ndx = strVar.Find('-')) != -1 )
		strVar = strVar.Left(ndx) + '_' + strVar.Mid(ndx+1);
	strVar = "HTTP_" + strVar;
}

CEnvironment::CEnvironment( void )
{
	m_nSize = 2;
}

CEnvironment::~CEnvironment( void )
{
}

BOOL CEnvironment::Add( CString name, CString value )
{
	BOOL bOk = TRUE;
	// create the entry pair string....
	CString strPair = name + __TEXT('=') + value;
	m_nSize += strPair.GetLength() + 1;
	POSITION pos = m_list.GetHeadPosition();

	// find the first item bigger than this string....
	while( pos != NULL )
	{
		if ( m_list.GetAt(pos).CompareNoCase(strPair) > 0 )
		{
			m_list.InsertBefore( pos, strPair );
			break;
		}
		m_list.GetNext( pos );
	}
	if ( pos == NULL )
		m_list.AddTail( strPair );

	return bOk;
}

LPVOID CEnvironment::GetBlock( void )
{
	// allocate a block....
	PTCHAR pBlock = new TCHAR[m_nSize];
	if ( pBlock )
	{
		// iterate through the list....
		PTCHAR pPos = pBlock;
		POSITION pos = m_list.GetHeadPosition();
		while( pos != NULL )
		{
			CString& str = m_list.GetNext( pos );
			// copy the string....
			lstrcpy( pPos, str );
			pPos += str.GetLength() + 1;
		}
		// NULL for the whole list....
		*pPos = __TEXT('\0');
	}
	return pBlock;
}
#endif // IMPL_CGI

CString CRequestSocket::GetHttpDate( LPFILETIME pft )
{
	SYSTEMTIME st;
	if ( pft )
		FileTimeToSystemTime( pft, &st );
	else
		GetSystemTime( &st );

	CTime timeHttp( st );
	return timeHttp.Format( IDS_HTTPTIME );
}

BOOL CRequestSocket::IfModSince( const CTime& timeIfMod )
{
	// assume it has been modified....
	BOOL bOk = TRUE;
	FILETIME ft;
	if ( GetFileTime( m_hFile, NULL, NULL, &ft ) )
	{
		SYSTEMTIME st;
		if ( FileTimeToSystemTime( &ft, &st ) )
		{
			CTime timeFile( st );
			if ( timeFile <= timeIfMod )
				bOk = FALSE;
		}
	}
	return bOk;
}

static int IntVal( CString strVal )
{
	int nVal = 0;
	strVal.TrimLeft();
	for( int ndx = 0; ndx < strVal.GetLength(); ++ndx )
		nVal = nVal*10 + strVal.GetAt(ndx) - '0';

	return nVal;
}

static int MonthFromStr( const CString& str )
{
	LPSTR aMonths[] = { 
		"xxx", "jan", "feb", "mar", "apr", "may", "jun",
		"jul", "aug", "sep", "oct", "nov", "dec" };
	for( int nMonth=1; nMonth <= 12; ++nMonth )
	{
		if ( str.CompareNoCase( aMonths[nMonth] ) == 0 )
			break;
	}

	return nMonth;
}

// Dow, dd Mon year hh:mm:ss GMT
BOOL CRequestSocket::FromHttpTime( const CString& strHttp, CTime& timeHttp )
{
	// assume we couldn't get a good time conversion....
	BOOL bOk = FALSE;
	SYSTEMTIME st = {0};
	int ndx;
	switch( strHttp.GetAt(3) )
	{
	case ',':
		// read RFC-1123 (preferred)....
		st.wDay = IntVal( strHttp.Mid(5,2) );
		st.wMonth = MonthFromStr( strHttp.Mid(8,3) );
		st.wYear = IntVal( strHttp.Mid(12,4) );
		st.wHour = IntVal( strHttp.Mid(17,2) );
		st.wMinute = IntVal( strHttp.Mid(20,2) );
		st.wSecond = IntVal( strHttp.Mid(23,2) );
		break;
	case ' ':
		// read ANSI-C time format....
		st.wDay = IntVal( strHttp.Mid(8,2) );
		st.wMonth = MonthFromStr( strHttp.Mid(4,3) );
		st.wYear = IntVal( strHttp.Mid(20,4) );
		st.wHour = IntVal( strHttp.Mid(11,2) );
		st.wMinute = IntVal( strHttp.Mid(14,2) );
		st.wSecond = IntVal( strHttp.Mid(17,2) );
		break;
	default:
		if ( (ndx = strHttp.Find( ", " )) != -1 )
		{
			st.wDay = IntVal( strHttp.Mid(ndx+2,2) );
			st.wMonth = MonthFromStr( strHttp.Mid(ndx+5,3) );
			st.wYear = IntVal( strHttp.Mid(ndx+9,2) );
			st.wHour = IntVal( strHttp.Mid(ndx+12,2) );
			st.wMinute = IntVal( strHttp.Mid(ndx+15,2) );
			st.wSecond = IntVal( strHttp.Mid(ndx+18,2) );
			// add the correct century....
			st.wYear += (st.wYear > 50)?1900:2000;
		}
		break;
	}
	// if year not zero, we pulled the info out of the string....
	if ( st.wYear != 0 )
	{
		// assume GMT....
		CTime strTime( st );
		// check to see if the minutes are the same....
		if ( strTime.GetMinute() == st.wMinute )
		{
			// assume it worked....
			timeHttp = strTime;
			bOk = TRUE;
		}
	}
	return bOk;
}

int CRequestSocket::AddRef( void )
{
	return ++m_nRefs;
}

int CRequestSocket::Release( void )
{
	int nRefs = --m_nRefs;
	if ( nRefs == 0 )
		delete this;
	return nRefs;
}

#ifdef _MAC
void CRequestSocket::MacifyPath( CString& strPath )
{
	int ndx;
	// change all forward slashes to colons....
	while ( (ndx=strPath.Find( '/' )) != -1 )
		strPath = strPath.Left(ndx) + ':' + strPath.Mid(ndx+1);
	// change all back slashes to colons....
	while ( (ndx=strPath.Find( '\\' )) != -1 )
		strPath = strPath.Left(ndx) + ':' + strPath.Mid(ndx+1);

	// resolve any dos "previous dir" stuff....
	while( (ndx=strPath.Find(":..")) != -1 )
	{
		CString strPre = strPath.Left( ndx );
		int nPrev = strPre.ReverseFind( SEPCHAR );
		if ( nPrev != -1 )
			strPre = strPre.Left( nPrev );
		else
			strPre.Empty();
		strPath = strPre + strPath.Mid( ndx+3 );
	}
	// cut out any dos "current dir" stuff....
	while( (ndx=strPath.Find(":.")) != -1 )
		strPath = strPath.Left(ndx) + strPath.Mid(ndx+2);
	// resolve any apple "previous dir" stuff....
	while( (ndx=strPath.Find("::")) != -1 )
	{
		CString strPre = strPath.Left( ndx );
		int nPrev = strPre.ReverseFind( SEPCHAR );
		if ( nPrev != -1 )
			strPre = strPre.Left( nPrev );
		else
			strPre.Empty();
		strPath = strPre + strPath.Mid( ndx+2 );
	}
	// make sure it doesn't end in a SEPCHAR....
	if ( strPath[ strPath.GetLength()-1 ] == SEPCHAR )
		strPath = strPath.Left( strPath.GetLength()-1 );
}
#endif // _MAC


void CRequestSocket::StuffFileType( void )
{
#ifndef _MAC
	// get the extension....
	CString strExt = m_pRequest->m_strFullPath.Mid( 
		m_pRequest->m_strFullPath.ReverseFind('.') );
	// find it in the registry....
	HKEY hKey = NULL;
	if ( RegOpenKeyEx( HKEY_CLASSES_ROOT, strExt, 
		0, KEY_READ, &hKey ) == ERROR_SUCCESS )
	{
		DWORD dwSize = 0;
		// see how long the data is....
		if ( RegQueryValueEx( hKey, "Content Type", NULL, NULL,
			NULL, &dwSize ) == ERROR_SUCCESS )
		{
			CString strType;
			LONG lRet = RegQueryValueEx( hKey, "Content Type", NULL, NULL,
				(LPBYTE)strType.GetBuffer( dwSize ), &dwSize );
			strType.ReleaseBuffer();
			if ( lRet == ERROR_SUCCESS )
				StuffHeader( "Content-type", strType );
		}
		RegCloseKey( hKey );
	}
#endif // !_MAC
}

CString Decode( const CString& str, BOOL bQuery )
{
	int ndx;
	CString strDecoded = str;
	// special processing or query strings....
	if ( bQuery )
	{
		// change all '+' to ' '....
		while( (ndx=strDecoded.Find('+')) != -1 )
			strDecoded = strDecoded.Left(ndx) + ' ' + strDecoded.Mid(ndx+1);
	}

	// first see if there are any %s to decode....
	if ( strDecoded.Find( '%' ) != -1 )
	{
		// iterate through the string, changing %dd to special char....
		for( ndx=0; ndx < strDecoded.GetLength(); ndx++ )
		{
			char ch = strDecoded.GetAt( ndx );
			if ( ch == '%' )
			{
				if ( strDecoded.GetAt( ndx+1 ) == '%' )
				{
					// wanna keep one percent sign....
					strDecoded = strDecoded.Left(ndx) + strDecoded.Mid(ndx+1);
				}
				else
				{
					// assume we have a hex value....
					char ch1 = strDecoded.GetAt(ndx+1);
					char ch2 = strDecoded.GetAt(ndx+2);
					ch1 = ch1 >= 'A' ? (ch1&0xdf)-'A' : ch1-'0';
					ch2 = ch2 >= 'A' ? (ch2&0xdf)-'A' : ch2-'0';
					// replace the escape sequence with the char....
					strDecoded = strDecoded.Left(ndx) 
						+ (char)(ch1*16 + ch2)
						+ strDecoded.Mid( ndx+3 );
				}
			}
		}
	}
	return strDecoded;
}

CString CRequestSocket::StripLast( CString& strPath )
{
	CString strExtra;
	if ( !strPath.IsEmpty() )
	{
 		int ndx = strPath.ReverseFind( SEPCHAR );
 		if ( ndx < 0 )
 			ndx = 0;
 		strExtra = strPath.Mid( ndx );
 		strPath = strPath.Left( ndx );
	}
	return strExtra;
}

BOOL CRequestSocket::CheckDefault( UINT uList, BOOL bExecute )
{
	BOOL bFound = FALSE;
	DWORD dwAttr;
	CString strDefault, strDefList;
	strDefList.LoadString( uList );
	while ( !strDefList.IsEmpty() )
	{
		int ndx;
		strDefault = m_pRequest->m_strFullPath;
		if ( (ndx=strDefList.Find('\n')) == -1 )
		{
			AddFile( strDefault, strDefList );
			strDefList.Empty();
		}
		else
		{
			AddFile( strDefault, strDefList.Left(ndx) );
			strDefList = strDefList.Mid( ndx+1 );
		}
		if ( (dwAttr=GetFileAttributes(strDefault)) != -1 &&
			(dwAttr & FILE_ATTRIBUTE_DIRECTORY) == 0 )
		{
			bFound = TRUE;
			break;
		}
	}
	if ( bFound )
	{
		// redirect to the default file....
		PathToURL( strDefault );
		if ( bExecute )
			strDefault += '?';

		StuffStatus( IDS_STATUS_MOVEDTEMP );
		StuffHeader( "Location", strDefault );
		StuffString( CRLF );
	}
	return bFound;
}

BOOL CRequestSocket::IsSvrApp( void )
{
	BOOL bOk = FALSE;
	int ndx = m_pRequest->m_strFullPath.ReverseFind( '.' );
	if ( ndx != -1 )
	{
		CString strExt = m_pRequest->m_strFullPath.Mid( ndx+1 );
		CString strAvail;
		// check if CGI app....
		strAvail.LoadString( IDS_APP_CGI );
		bOk = CheckExt( strExt, strAvail, CRequest::APP_CGI );
		if ( !bOk )
		{
			strAvail.LoadString( IDS_APP_ISAPI );
			bOk = CheckExt( strExt, strAvail, CRequest::APP_ISAPI );
		}
	}

	return bOk;
}

BOOL CRequestSocket::CheckExt( const CString& strExt, CString& strAvail, DWORD dwType )
{
	BOOL bMatch = FALSE;
	CString strPossible;
	// loop through all possible exts....
	while( !strAvail.IsEmpty() )
	{
		int ndx = strAvail.ReverseFind('\n');
		if ( ndx == -1 )
		{
			strPossible = strAvail;
			strAvail.Empty();
		}
		else
		{
			strPossible = strAvail.Mid( ndx+1 );
			strAvail = strAvail.Left( ndx );
		}
		if ( strExt.CompareNoCase( strPossible ) == 0 )
		{
			m_pRequest->m_dwExecute = dwType;
			bMatch = TRUE;
			break;
		}
	}
	return bMatch;
}
