// regsvr.cpp : Program to invoke OLE self-registration on a DLL.
//
// This is a part of the Microsoft Foundation Classes C++ library.
// Copyright (C) 1992-1995 Microsoft Corporation
// All rights reserved.
//
// This source code is only intended as a supplement to the
// Microsoft Foundation Classes Reference and related
// electronic documentation provided with the library.
// See these sources for detailed information regarding the
// Microsoft Foundation Classes product.

#include <tchar.h>
#include <stdio.h>
#include <windows.h>
#include "resource.h"

#define FAIL_ARGS	1
#define FAIL_OLE	2
#define FAIL_LOAD	3
#define FAIL_ENTRY	4
#define FAIL_REG	5

static TCHAR _szAppName[] = _T("RegSvr32");
static char _szDllRegSvr[] = "DllRegisterServer";
static char _szDllUnregSvr[] = "DllUnregisterServer";
static HINSTANCE _hInstance = NULL;

static BOOL _bSilent = FALSE;
static BOOL _bConsole = FALSE;

#define SafePutts(string) ((stdout >= 0) ? _putts(string) : 0) 

void FormatString2(LPTSTR lpszOut, LPCTSTR lpszFormat, LPCTSTR lpsz1,
	LPCTSTR lpsz2)
{
	LPCTSTR pchSrc = lpszFormat;
	LPTSTR pchDest = lpszOut;
	while (*pchSrc != '\0')
	{
		if (pchSrc[0] == '%' && (pchSrc[1] >= '1' && pchSrc[1] <= '2'))
		{
			lstrcpy(pchDest, (LPCTSTR)(pchSrc[1] == '1' ? lpsz1 : lpsz2));
			pchDest += lstrlen(pchDest);
			pchSrc += 2;
		}
		else
		{
			if (_istlead(*pchSrc))
				*pchDest++ = *pchSrc++; // copy first of 2 bytes
			*pchDest++ = *pchSrc++;
		}
	}
	*pchDest = '\0';
}

#define MAX_STRING 1024

void DisplayMessage(UINT ids, LPCTSTR pszArg1 = NULL, LPCTSTR pszArg2 = NULL,
	BOOL bUsage = FALSE, BOOL bInfo = FALSE)
{
	if (_bSilent && !_bConsole)
		return;

	TCHAR szFmt[MAX_STRING];
	LoadString(_hInstance, ids, szFmt, MAX_STRING);

	TCHAR szText[MAX_STRING];
	FormatString2(szText, szFmt, pszArg1, pszArg2);
	if (bUsage)
	{
		int cch = _tcslen(szText);
		LoadString(_hInstance, IDS_USAGE, szText + cch, MAX_STRING - cch);
	}

	if (! _bSilent)
		MessageBox(NULL, szText, _szAppName,
			MB_TASKMODAL | (bInfo ? MB_ICONINFORMATION : MB_ICONEXCLAMATION));

	if (_bConsole)
	{
		TCHAR szMessage[MAX_STRING];
		FormatString2(szMessage, _T("%1: %2\n"), _szAppName, szText);
		SafePutts(szMessage);
	}
}

inline void Usage(UINT ids, LPCTSTR pszArg1 = NULL, LPCTSTR pszArg2 = NULL)
	{ DisplayMessage(ids, pszArg1, pszArg2, TRUE); }

inline void Info(UINT ids, LPCTSTR pszArg1 = NULL, LPCTSTR pszArg2 = NULL)
	{ DisplayMessage(ids, pszArg1, pszArg2, FALSE, TRUE); }

int PASCAL WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int)
{
	int iReturn = 0;
	HRESULT (FAR STDAPICALLTYPE * lpDllEntryPoint)(void);

	BOOL bVisualC = FALSE;
	BOOL bUnregister = FALSE;
	LPSTR pszDllEntryPoint = _szDllRegSvr;
	LPSTR pszDllName = NULL;    
	LPSTR pszTok;

	_hInstance = hInstance;

	// Parse command line arguments.
	int iTok;
	for (iTok = 1; iTok < __argc; iTok++)
	{                                
		pszTok = __argv[iTok];
		
		if ((pszTok[0] == '-') || (pszTok[0] == '/'))
		{
			switch (pszTok[1])
			{
			case 'v':
			case 'V':
				bVisualC = TRUE;
				break;
								
			case 's':
			case 'S':
				_bSilent = TRUE;
				break;
				
			case 'u':
			case 'U':
				bUnregister = TRUE;
				pszDllEntryPoint = _szDllUnregSvr;
				break;

			case 'c':
			case 'C':
				_bConsole = TRUE;
				break;
				
			default:
				Usage(IDS_UNRECOGNIZEDFLAG, pszTok);
				return FAIL_ARGS;
			}
		}
		else
		{
			if (pszDllName == NULL)
			{
				pszDllName = pszTok;
				break;
			}
			else
			{
				Usage(IDS_EXTRAARGUMENT, pszTok);
				return FAIL_ARGS;
			}
		}
	}

	if (pszDllName == NULL)
	{
		if (bVisualC)
			DisplayMessage(IDS_NOPROJECT);
		else
			Usage(IDS_NODLLNAME);

		return FAIL_ARGS;
	}

	// Initialize OLE.				
	if (FAILED(OleInitialize(NULL)))
	{
		DisplayMessage(IDS_OLEINITFAILED);
		return FAIL_OLE;
	}

	for (; iTok < __argc; iTok++)
	{
		pszDllName = __argv[iTok];

		// Load the library.	
		HINSTANCE hLib = LoadLibrary(pszDllName);

		if (hLib < (HINSTANCE)HINSTANCE_ERROR)
		{
			TCHAR szError[12];
			wsprintf(szError, _T("0x%08lx"), GetLastError());
			DisplayMessage(IDS_LOADLIBFAILED, pszDllName, szError);
			iReturn = FAIL_LOAD;
			goto CleanupOle;
		}

		// Find the entry point.		
		(FARPROC&)lpDllEntryPoint = GetProcAddress(hLib, pszDllEntryPoint);

		if (lpDllEntryPoint == NULL)
		{
			TCHAR szExt[_MAX_EXT];
			_tsplitpath(pszDllName, NULL, NULL, NULL, szExt);

			if ((stricmp(szExt, ".dll") != 0) && (stricmp(szExt, ".ocx") != 0))
				DisplayMessage(IDS_NOTDLLOROCX, pszDllName, pszDllEntryPoint);
			else
				DisplayMessage(IDS_NOENTRYPOINT, pszDllName, pszDllEntryPoint);

			iReturn = FAIL_ENTRY;
			goto CleanupLibrary;
		}

		// Call the entry point.	
		if (FAILED((*lpDllEntryPoint)()))
		{
			DisplayMessage(IDS_CALLFAILED, pszDllEntryPoint, pszDllName);
			iReturn = FAIL_REG;
			goto CleanupLibrary;
		}

		Info(IDS_CALLSUCCEEDED, pszDllEntryPoint, pszDllName);

CleanupLibrary:
		FreeLibrary(hLib);
	}

CleanupOle:
	OleUninitialize();
		
	return iReturn;
}			
