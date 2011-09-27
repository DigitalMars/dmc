// tear.cpp : implements the TEAR console application
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

#include <afx.h>
#include <afxinet.h>
#include "tear.h"

#include <iostream.h>
#include <stdlib.h>

/////////////////////////////////////////////////////////////////////////////
// Globals

LPCTSTR pszURL = NULL;
BOOL    bStripMode = FALSE;
BOOL    bProgressMode = FALSE;
DWORD   dwAccessType = PRE_CONFIG_INTERNET_ACCESS;

DWORD dwHttpRequestFlags =
	INTERNET_FLAG_EXISTING_CONNECT | INTERNET_FLAG_NO_AUTO_REDIRECT;

const TCHAR szHeaders[] =
	_T("Accept: text/*\r\nUser-Agent: MFC_Tear_Sample\r\n");

/////////////////////////////////////////////////////////////////////////////
// CTearSession object

// TEAR wants to use its own derivative of the CInternetSession class
// just so it can implement an OnStatusCallback() override.

CTearSession::CTearSession(LPCTSTR pszAppName, int nMethod)
	: CInternetSession(pszAppName, 1, nMethod)
{
}

void CTearSession::OnStatusCallback(DWORD /* dwContext */, DWORD dwInternetStatus,
	LPVOID /* lpvStatusInfomration */, DWORD /* dwStatusInformationLen */)
{
	if (!bProgressMode)
		return;

	if (dwInternetStatus == INTERNET_STATUS_CONNECTED_TO_SERVER)
		cerr << _T("Connection made!") << endl;
}

/////////////////////////////////////////////////////////////////////////////
// CTearException -- used if something goes wrong for us

// TEAR will throw its own exception type to handle problems it might
// encounter while fulfilling the user's request.

IMPLEMENT_DYNCREATE(CTearException, CException)

CTearException::CTearException(int nCode)
	: m_nErrorCode(nCode)
{
}

void ThrowTearException(int nCode)
{
	CTearException* pEx = new CTearException(nCode);
	throw pEx;
}

/////////////////////////////////////////////////////////////////////////////
// Routines

void ShowBanner()
{
	cerr << _T("TEAR - Tear a Page Off the Internet!") << endl;
	cerr << _T("Version 4.2 - Copyright (C) Microsoft Corp 1996") << endl;
	cerr << endl;
}

void ShowUsage()
{
	cerr << _T("Usage:  TEAR [options] <URL>") << endl << endl;
	cerr << _T("\t<URL> points at a HTTP resource") << endl;
	cerr << _T("\t[options] are any of:") << endl;
	cerr << _T("\t\t/F force reload of requested page") << endl;
	cerr << _T("\t\t/P show detailed progress information") << endl;
	cerr << _T("\t\t/S strip HTML tags from stream") << endl << endl;
	cerr << _T("\t\t/L use local Internet access") << endl;
	cerr << _T("\t\t/D use pre-configured Internet access (default)") << endl;
	cerr << _T("\t\t/G use gateway Internet access") << endl;

	cerr << endl;
	exit(1);
}

// ParseOptions() looks for options on the command line and sets global
// flags so the rest of the program knows about them.  ParseOptions()
// also initializes pszURL to point at the URL the user wanted.

BOOL ParseOptions(int argc, char* argv[])
{
	int nIndex;
	for (nIndex = 1; nIndex < argc; nIndex++)
	{
		// an option or a URL?

		if (*argv[nIndex] == '-' || *argv[nIndex] == '/')
		{
			if (argv[nIndex][1] == 'D' || argv[nIndex][1] == 'd')
				dwAccessType = PRE_CONFIG_INTERNET_ACCESS;
			else if (argv[nIndex][1] == 'L' || argv[nIndex][1] == 'l')
				dwAccessType = LOCAL_INTERNET_ACCESS;
			else if (argv[nIndex][1] == 'G' || argv[nIndex][1] == 'g')
				dwAccessType = GATEWAY_INTERNET_ACCESS;
			else if (argv[nIndex][1] == 'S' || argv[nIndex][1] == 's')
				bStripMode = TRUE;
			else if (argv[nIndex][1] == 'P' || argv[nIndex][1] == 'p')
				bProgressMode = TRUE;
			else if (argv[nIndex][1] == 'F' || argv[nIndex][1] == 'f')
				dwHttpRequestFlags |= INTERNET_FLAG_RELOAD;
			else
			{
				cerr << _T("Error: unrecognized option: ") << argv[nIndex] << endl;
				return FALSE;
			}
		}
		else
		{
		// can't have too many URLs

		if (pszURL != NULL)
		{
			cerr << _T("Error: can only specify one URL!") << endl;
			return FALSE;
		}
		else
			pszURL = argv[nIndex];
		}
	}

	return TRUE;
}

// StripTags() rips through a buffer and removes HTML tags from it.
// The function uses a static variable to remember its state in case
// a HTML tag spans a buffer boundary.

void StripTags(LPTSTR pszBuffer)
{
	static BOOL bInTag = FALSE;
	LPTSTR pszSource = pszBuffer;
	LPTSTR pszDest = pszBuffer;

	while (*pszSource != '\0')
	{
		if (bInTag)
		{
			if (*pszSource == '>')
				bInTag = FALSE;
			pszSource++;
		}
		else
		{
			if (*pszSource == '<')
				bInTag = TRUE;
			else
			{
				*pszDest = *pszSource;
				pszDest++;
			}
			pszSource++;
		}
	}
	*pszDest = '\0';
}

/////////////////////////////////////////////////////////////////////////////
// The main() Thang

int main(int argc, char* argv[])
{
	int nRetCode = 0;

	ShowBanner();

	if (argc < 2 || !ParseOptions(argc, argv) || pszURL == NULL)
		ShowUsage();

	CTearSession session(_T("TEAR - MFC Sample App"), dwAccessType);
	CHttpConnection* pServer = NULL;
	CHttpFile* pFile = NULL;
	try
	{
		// check to see if this is a reasonable URL

		CString strServerName;
		CString strObject;
		INTERNET_PORT nPort;
		DWORD dwServiceType;

		if (!AfxParseURL(pszURL, dwServiceType, strServerName, strObject, nPort) ||
			dwServiceType != INTERNET_SERVICE_HTTP)
		{
			cerr << _T("Error: can only use URLs beginning with http://") << endl;
			ThrowTearException(1);
		}

		if (bProgressMode)
		{
			cerr << _T("Opening Internet...");
			VERIFY(session.EnableStatusCallback(TRUE));
		}

		pServer = session.GetHttpConnection(strServerName, nPort);

		pFile = pServer->OpenRequest(CHttpConnection::HTTP_VERB_GET,
			strObject, NULL, 1, NULL, NULL, dwHttpRequestFlags);
		pFile->AddRequestHeaders(szHeaders);
		pFile->SendRequest();

		DWORD dwRet;
		pFile->QueryInfoStatusCode(dwRet);

		// if access was denied, prompt the user for the password

		if (dwRet == HTTP_STATUS_DENIED)
		{
			DWORD dwPrompt;
			dwPrompt = pFile->ErrorDlg(NULL, ERROR_INTERNET_INCORRECT_PASSWORD,
				FLAGS_ERROR_UI_FLAGS_GENERATE_DATA | FLAGS_ERROR_UI_FLAGS_CHANGE_OPTIONS, NULL);

			// if the user cancelled the dialog, bail out

			if (dwPrompt != ERROR_INTERNET_FORCE_RETRY)
			{
				cerr << _T("Access denied: Invalid password\n");
				ThrowTearException(1);
			}

			pFile->SendRequest();
			pFile->QueryInfoStatusCode(dwRet);
		}

		CString strNewLocation;
		pFile->QueryInfo(HTTP_QUERY_RAW_HEADERS_CRLF, strNewLocation);

		// were we redirected?
		// these response status codes come from WININET.H

		if (dwRet == HTTP_STATUS_MOVED ||
			dwRet == HTTP_STATUS_REDIRECT ||
			dwRet == HTTP_STATUS_REDIRECT_METHOD)
		{
			CString strNewLocation;
			pFile->QueryInfo(HTTP_QUERY_RAW_HEADERS_CRLF, strNewLocation);

			int nPlace = strNewLocation.Find(_T("Location: "));
			if (nPlace == -1)
			{
				cerr << _T("Error: Site redirects with no new location") << endl;
				ThrowTearException(2);
			}

			strNewLocation = strNewLocation.Mid(nPlace + 10);
			nPlace = strNewLocation.Find('\n');
			if (nPlace > 0)
				strNewLocation = strNewLocation.Left(nPlace);

			// close up the redirected site

			pFile->Close();
			delete pFile;
			pServer->Close();

			if (bProgressMode)
			{
				cerr << _T("Caution: redirected to ");
				cerr << (LPCTSTR) strNewLocation << endl;
			}

			// figure out what the old place was
			if (!AfxParseURL(strNewLocation, dwServiceType, strServerName, strObject, nPort))
			{
				cerr << _T("Error: the redirected URL could not be parsed.") << endl;
				ThrowTearException(2);
			}

			if (dwServiceType != INTERNET_SERVICE_HTTP)
			{
				cerr << _T("Error: the redirected URL does not reference a HTTP resource.") << endl;
				ThrowTearException(2);
			}

			// try again at the new location
			pServer = session.GetHttpConnection(strServerName, nPort);
			pFile = pServer->OpenRequest(CHttpConnection::HTTP_VERB_GET,
				strObject, NULL, 1, NULL, NULL, dwHttpRequestFlags);
			pFile->AddRequestHeaders(szHeaders);
			pFile->SendRequest();

			pFile->QueryInfoStatusCode(dwRet);
			if (dwRet != HTTP_STATUS_OK)
			{
				cerr << _T("Error: Got status code ") << dwRet << endl;
				ThrowTearException(2);
			}
		}

		cerr << _T("Status Code is ") << dwRet << endl;

		TCHAR sz[1024];
		while (pFile->ReadString(sz, 1023))
		{
			if (bStripMode)
				StripTags(sz);
			cout << sz;
		}

	// NOTE: Since HTTP servers normally spit back plain text, the
	// above code (which reads line by line) is just fine.  However,
	// other data sources (eg, FTP servers) might provide binary data
	// which should be handled a buffer at a time, like this:

#if 0
		while (nRead > 0)
		{
			sz[nRead] = '\0';
			if (bStripMode)
				StripTags(sz);
			cout << sz;
			nRead = pFile->Read(sz, 1023);
		}
#endif

		pFile->Close();
		pServer->Close();
	}
	catch (CInternetException* pEx)
	{
		// catch errors from WinINet

		TCHAR szErr[1024];
		pEx->GetErrorMessage(szErr, 1024);

		cerr << _T("Error: (") << pEx->m_dwError << _T(") ");
		cerr << szErr << endl;

		nRetCode = 2;
		pEx->Delete();
	}
	catch (CTearException* pEx)
	{
		// catch things wrong with parameters, etc

		nRetCode = pEx->m_nErrorCode;
		TRACE1("Error: Exiting with CTearException(%1)\n", nRetCode);
		pEx->Delete();
	}

	if (pFile != NULL)
		delete pFile;
	if (pServer != NULL)
		delete pServer;
	session.Close();

	return nRetCode;
}
