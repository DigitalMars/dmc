// Http.cpp : structures, functions and definitions for http service
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
#include "Http.h"
#include "Request.h"
#include "resource.h"

IMPLEMENT_DYNCREATE(CHitDoc, CObject)

CHitDoc::CHitDoc( void )
{
}

CHitDoc::CHitDoc(  CString strFile )
{
	DWORD dwAttr = GetFileAttributes(strFile);
	if ( dwAttr != (DWORD)(-1) && (dwAttr&FILE_ATTRIBUTE_DIRECTORY) != 0 )
		m_bFolder = TRUE;
	else
		m_bFolder = FALSE;

	m_nHits = 0;
	m_dwExecute = 0;
	m_nStatus = -1;
	ParseFileName( strFile );
}

CHitDoc::CHitDoc( CRequest* pRequest )
{
	m_nHits = 1; // at least one hit....
	// is it an executable?
	m_dwExecute = pRequest->m_dwExecute;
	// time it was hit....
	m_timeLastHit = pRequest->m_timeReq;
	// status....
	m_nStatus = pRequest->m_uStatus;
	// referring document....
	m_strCommand = pRequest->GetHeaderValue( "Referer" );
	// get the URL....
	m_strURL = pRequest->m_strURL;
	// determine file and folder names....
	if ( m_nStatus == 200 || (m_nStatus==0 && m_dwExecute) )
	{
		m_bFolder = ((pRequest->m_dwAttr & FILE_ATTRIBUTE_DIRECTORY) != 0);
		ParseFileName( pRequest->m_strFullPath );
	}
	else
	{
		m_bFolder = FALSE;
		m_strFile.LoadString( pRequest->m_uStatus );
	}
}

int CHitDoc::operator==( CHitDoc* pHit )
{
	int nCmp = m_strFile.CompareNoCase( pHit->m_strFile );
	if ( nCmp == 0 )
		nCmp = m_strFolder.CompareNoCase( pHit->m_strFolder );
	return nCmp;
}

void CHitDoc::ParseFileName( const CString& strFullPath )
{
	int nFile = strFullPath.ReverseFind( SEPCHAR );
	if ( nFile != -1 )
	{
		m_strFolder = strFullPath.Left( nFile+1 );
		m_strFile = strFullPath.Mid( nFile+1 );
	}
}

