/////////////////////////////////////////////////////////////////////////////
// counter.cpp : Defines the initialization routines for the DLL.
//

// This is a part of the Microsoft Foundation Classes C++ library.
// Copyright (C) 1992-1996 Microsoft Corporation
// All rights reserved.
//
// This source code is only intended as a supplement to the
// Microsoft Foundation Classes Reference and related
// electronic documentation provided with the library.
// See these sources for detailed information regarding the
// Microsoft Foundation Classes product.

// Written by Jeff Miller, Microsoft Product Support Services

/////////////////////////////////////////////////////////////////////////////

// This ISAPI DLL can be called in three ways:

// <IMG SRC="/scripts/counter.dll?clock">
// will return an x-bitmap image containing the current system time

// <IMG SRC="/scripts/counter.dll/mydir/mypage.htm">
// will return an x-bitmap image containing the accumulated
// count for the identifier /mydir/mypage.htm

// <IMG SRC="/scripts/counter.dll">
// will return an x-bitmap image containing the accumulated
// count for the page which called the DLL.

#include <afx.h>
#include <afxwin.h>
#include <afxmt.h>
#include <afxisapi.h>
#include "resource.h"
#include "counter.h"

#include "charset.h"

// This program sends back xbitmap images rather than HTML.

static const TCHAR szContentType[] = _T("Content-Type: image/x-xbitmap\r\n");

// The following defines the location and filename for the log file.
// This should be placed in a location that can be read and written to.

static const TCHAR szLogFile[] = _T("C:\\counter.log");


// The following two headers will tell the client never to cache
// this information, as it is dynamic.

static const TCHAR szExpires[] = _T("Expires: Thu, 01 Dec 1994 16:00:00 GMT\r\n");
static const TCHAR szNoCache[] = _T("Pragma: no-cache\r\n");

///////////////////////////////////////////////////////////////////////
// command-parsing map

BEGIN_PARSE_MAP(CCounterExtension, CHttpServer)
	ON_PARSE_COMMAND(Clock, CCounterExtension, ITS_EMPTY)
	ON_PARSE_COMMAND(Default, CCounterExtension, ITS_EMPTY)
	DEFAULT_PARSE_COMMAND(Default, CCounterExtension)
END_PARSE_MAP(CCounterExtension)


///////////////////////////////////////////////////////////////////////
// The one and only CCounterExtension object

CCounterExtension theExtension;

///////////////////////////////////////////////////////////////////////
// CCounterExtension implementation

CCounterExtension::CCounterExtension()
{
}

CCounterExtension::~CCounterExtension()
{
}

BOOL CCounterExtension::GetExtensionVersion(HSE_VERSION_INFO* pVer)
{
	// Call default implementation for initialization
	CHttpServer::GetExtensionVersion(pVer);

	// Load description string
	TCHAR sz[HSE_MAX_EXT_DLL_NAME_LEN+1];
	ISAPIVERIFY(::LoadString(AfxGetResourceHandle(),
			IDS_SERVER, sz, HSE_MAX_EXT_DLL_NAME_LEN));
	_tcscpy(pVer->lpszExtensionDesc, sz);
	return TRUE;
}

///////////////////////////////////////////////////////////////////////
// CCounterExtension command handlers

void CCounterExtension::Default(CHttpServerContext* pCtxt)
{
	// We don't call StartContent() or WriteTitle() here due to the
	// fact that those will send back tags appropriate only to
	// HTML pages.  We will be sending back an image.

	// Don't allow these pages to be cached

	AddHeader(pCtxt, szExpires);
	AddHeader(pCtxt, szNoCache);

	TCHAR pstrBuffer[1024];

	// Check to see if a path was given after the DLL's name,
	// such as <IMG SRC="/scripts/counter.dll/mydir/mypage.htm">
	// If so, use the given path.  Otherwise, use the URL of
	// the page which called this DLL.

	if (_tcsclen(pCtxt->m_pECB->lpszPathInfo) != 0)
	{
		_tcscpy(pstrBuffer, pCtxt->m_pECB->lpszPathInfo);
	}
	else
	{
		// HTTP_REFERER contains the full URL of the page which
		// called this DLL.

		DWORD dwSize = 1024;
		pCtxt->GetServerVariable(_T("HTTP_REFERER"), pstrBuffer, &dwSize);
	}

	// Call member function to see how many times the requested
	// page has been accessed and update that count.

	CString szPath(pstrBuffer);
	int nCount = GetPageCount(szPath);

	// Call member function to output an xbitmap image 
	// containing the digits.

	CString szCount;
	szCount.Format(_T("%d"), nCount);
	OutputXBM(pCtxt, szCount);

	// EndContent() is only appropriate for HTML files, so we
	// don't call it here.
}

void CCounterExtension::Clock(CHttpServerContext* pCtxt)
{
	// Clock
	// Called when DLL is accessed using the format
	// <IMG SRC="/scripts/counter.dll?Clock">.

	// We don't call StartContent() or WriteTitle() here due to the
	// fact that those will send back tags appropriate only to
	// HTML pages.  We will be sending back an image.

	// Don't allow these pages to be cached

	AddHeader(pCtxt, szExpires);
	AddHeader(pCtxt, szNoCache);

	// Get the current system time, and put it into a form
	// which our function will accept.

	CTime time = CTime::GetCurrentTime();
	CString szTime = time.Format(_T("%H%%%M"));

	// call member function to output an xbitmap image
	// containing the digits

	OutputXBM(pCtxt, szTime);

	// EndContent() is only appropriate for HTML files, so we
	// don't call it here.
}

void CCounterExtension::OutputXBM(CHttpServerContext* pCtxt, CString& szDigits)
{
	// Function to take in a string containing the digits 0..9 and the character
	// ':' and output an xbitmap image of those digits to the stream

	// Start by writing the proper content type to the client

	AddHeader(pCtxt, szContentType);

	// NOTE: this code as is will only work properly with image data 
	// that has a width = 8

	int nFinalWidth = char_width * szDigits.GetLength();
	int nFinalHeight = char_height;

	// write out the XBM header.  We cast to long int because there is
	// no CHttpServerContext << operator overload that accepts an
	// integer.

	*pCtxt << _T("#define counter_width ") << (long int)nFinalWidth << _T("\r\n");
	*pCtxt << _T("#define counter_height ") << (long int)nFinalHeight << _T("\r\n");
	*pCtxt << _T("static unsigned char counter_bits[] = {\r\n");

	// Now for each horizontal line of output, get the bitmap for each
	// character for that line and output it.

	for (int nLine=0; nLine<nFinalHeight; nLine++)
	{
		for (int nChar=0; nChar<szDigits.GetLength(); nChar++)
		{
			int nDigitOffset;
			if (szDigits[nChar] >= __TEXT('0') && szDigits[nChar] <= __TEXT('9'))
				nDigitOffset = szDigits[nChar] - __TEXT('0');
			else

				// colon is in index 10 in the bitmap array
				nDigitOffset = 10;

			CString szHex;
			szHex.Format(_T("0x%02X, "), char_bits[nDigitOffset][nLine]);
			*pCtxt << szHex;
		}
	}

	*pCtxt << _T("};\r\n");
}

int CCounterExtension::GetPageCount(CString& szPage)
{
	// Given a unique page identifier (szPage), check our "database"
	// to see how many times this page has been accessed, then
	// update the count.

	// Attempt to open an existing file, or create a new one if none
	// exists.


	CFile file;
	CFileException e;

	// Only allow one thread through here at a time.

	CCriticalSection cs;
	cs.Lock();

	if ( !file.Open(szLogFile,
		CFile::modeRead | CFile::shareExclusive
		| CFile::modeCreate | CFile::modeNoTruncate, &e) )
	{
		TCHAR szCause[255];
		e.GetErrorMessage(szCause, 255);		
		ISAPITRACE0("Counter file could not be opened for reading\n");
		ISAPITRACE1("-> Cause: %s\n", szCause);
		cs.Unlock(); // IMPORTANT
		return 0;
	}

	CArchive archive(&file, CArchive::load);
	
	// Serialize in the data from the file
	try
	{
		m_Paths.Serialize(archive);
	}
	catch (CArchiveException* e)
	{
		TCHAR szCause[255];
		e->GetErrorMessage(szCause, 255);		
		ISAPITRACE0("Couldn't serialize from counter file\n");
		ISAPITRACE1("->Cause: %s\n", szCause);

		m_Paths.RemoveAll();
	}
	
	archive.Close();
	file.Close();

	// Try to find the page identifier in the log

	int nVal = 0;
	CString szCount;
	if (m_Paths.Lookup(szPage, szCount))
		nVal = atoi(szCount);

	// Increment the count.

	nVal++;

	// Set the new value.

	szCount.Format(_T("%d"), nVal);
	m_Paths.SetAt(szPage, szCount);

	// Write the updated log

	if ( !file.Open(szLogFile,
		CFile::modeWrite | CFile::shareExclusive
		| CFile::modeCreate, &e) )
	{
		TCHAR szCause[255];
		e.GetErrorMessage(szCause, 255);		
		ISAPITRACE0("Counter file could not be opened for writing\n");
		ISAPITRACE1("->Cause: %s", szCause);
		cs.Unlock(); // IMPORTANT
		return 0;
	}

	CArchive archiveStore(&file, CArchive::store);

	try
	{
		m_Paths.Serialize(archiveStore);
	}
	catch (CArchiveException* e)
	{
		TCHAR szCause[255];
		e->GetErrorMessage(szCause, 255);		
		ISAPITRACE0("Couldn't serialize to counter file\n");
		ISAPITRACE1("->Cause: %s\n", szCause);
	}

	archiveStore.Close();
	file.Close();

	cs.Unlock();

	// Return the count we received
	return nVal;
}
