// mfcucase.cpp : implements the filter class
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

// Note that your filters probably won't have an #ifdef like this one.
// The project file for this sample allows you to build the sample as
// a statically linked regular MFC DLL (that is, with _AFXDLL defined)
// or as a DLL that doesn't use MFC classes aside from the ISAPI 
// support classes (that is, without _AFXDLL defined).

#ifdef _AFXDLL
#include <afx.h>
#include <afxwin.h>
#endif

#include <afxisapi.h>
#include "mfcucase.h"
#include "resource.h"


///////////////////////////////////////////////////////////////////////
// The one and only CUpCaseFilter object

CUpCaseFilter theFilter;


///////////////////////////////////////////////////////////////////////
// CUpCaseFilter implementation

CUpCaseFilter::CUpCaseFilter()
{
}

CUpCaseFilter::~CUpCaseFilter()
{
}


///////////////////////////////////////////////////////////////////////
// CUpCaseFilter object 

BOOL CUpCaseFilter::GetFilterVersion(PHTTP_FILTER_VERSION pVer)
{
	// call default implmentation to initialize
	CHttpFilter::GetFilterVersion(pVer);

	// set flags that interest us
	pVer->dwFlags |=
		(SF_NOTIFY_NONSECURE_PORT |
		 SF_NOTIFY_URL_MAP | SF_NOTIFY_SEND_RAW_DATA);

	// load description string
	TCHAR sz[SF_MAX_FILTER_DESC_LEN+1];
	ISAPIVERIFY(::LoadString(AfxGetResourceHandle(),
		IDS_MFCUCASE_FILTER, sz, SF_MAX_FILTER_DESC_LEN));
	_tcscpy(pVer->lpszFilterDesc, sz);
	return TRUE;
}
	
DWORD CUpCaseFilter::OnSendRawData(CHttpFilterContext* pCtxt,
	PHTTP_FILTER_RAW_DATA pRawData)
{
	LPTSTR	pstrIn;
	DWORD	cbBuffer;
	DWORD	cbTemp;

	// if we previously identified the data requested as being
	// interesting, let's try to parse it

	if (pCtxt->m_pFC->pFilterContext != NULL)
	{
		// gain a pointer to the actual data
		pstrIn = (LPTSTR) pRawData->pvInData;
		cbBuffer = 0;
		cbTemp = 0;

		// are we waiting for the header?

		if (pCtxt->m_pFC->pFilterContext == (VOID *) 1)
		{
			// rip through the header to the end
			while (cbBuffer < pRawData->cbInData)
			{
				if (pstrIn[cbBuffer] == '\n' &&
					pstrIn[cbBuffer+2] == '\n')
				{
					cbBuffer += 3;
					break;
				}
				cbBuffer++;
			}

			// does the header identify HTML content?
			while (cbTemp < cbBuffer)
			{
				if (pstrIn[cbTemp] == '/' && pstrIn[cbTemp+1] == 'h' &&
					pstrIn[cbTemp+2] == 't' && pstrIn[cbTemp+3] == 'm')
				{
					// yes, it's HTML, set our flag
					pCtxt->m_pFC->pFilterContext = (VOID*) 2;
					break;
				}

				cbTemp++;
			}

			// if it wasn't HTML--clear the flag
			if (cbTemp == cbBuffer)
				pCtxt->m_pFC->pFilterContext = NULL;
		}	

		// if there's anything else left in the buffer,
		// convert it all to upper case!
		if (pCtxt->m_pFC->pFilterContext != NULL)
		{
			while (cbBuffer < pRawData->cbInData)
			{
				if (isleadbyte(pstrIn[cbBuffer]))
					cbBuffer += 2;
				else
				{
					pstrIn[cbBuffer] = _totupper(pstrIn[cbBuffer]);
					cbBuffer++;
				}
			}
		}
	}

	return SF_STATUS_REQ_NEXT_NOTIFICATION;
}

DWORD CUpCaseFilter::OnUrlMap(CHttpFilterContext* pCtxt,
	PHTTP_FILTER_URL_MAP pUrlMap)
{
	LPTSTR pstrPhysPath = pUrlMap->pszPhysicalPath;
	pCtxt->m_pFC->pFilterContext = NULL;

	// if we get a URL, try to see if it has the string "\UC\"
	while (*pstrPhysPath != '\0')
	{
		if (pstrPhysPath[0] == '\\' &&
			(pstrPhysPath[1] == 'u' || pstrPhysPath[1] == 'U') &&
			(pstrPhysPath[2] == 'c' || pstrPhysPath[2] == 'C') &&
			pstrPhysPath[3] == '\\')
		{
			// we found a "\UC\", so the user wants
			// upper case.  remove the "\UC\" substring
			// so the server can find the real path

			while (pstrPhysPath[3] != '\0')
			{
				pstrPhysPath[0] = pstrPhysPath[3];
				pstrPhysPath = _tcsinc(pstrPhysPath);
			}
			pstrPhysPath[0] = '\0';

			// set a flag to remind us to try and uppercase
			pCtxt->m_pFC->pFilterContext = (VOID*) 1;
			break;
		}

		pstrPhysPath = _tcsinc(pstrPhysPath);
	}
	
	return SF_STATUS_REQ_NEXT_NOTIFICATION;
}

// If you're building a filter that doesn't use any MFC classes, you'll
// need to implement your own version of AfxGetResourceHandle() so that
// the MFC ISAPI support classes can find resources they may need.
// If you're using MFC classes besides the ISAPI support classes, you'll
// automatically get the normal MFC implementation of
// AfxGetResourceHandle() and won't need this code.

#ifndef _AFXDLL

static HINSTANCE g_hInstance;

HINSTANCE AFXISAPI AfxGetResourceHandle()
{
	return g_hInstance;
}

BOOL WINAPI DllMain(HINSTANCE hInst, ULONG ulReason,
					LPVOID lpReserved)
{
	if (ulReason == DLL_PROCESS_ATTACH)
		g_hInstance = hInst;

	return TRUE;
}

#endif

