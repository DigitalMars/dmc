// This is a part of the Microsoft Foundation Classes C++ library.
// Copyright (C) 1993 Microsoft Corporation
// All rights reserved.
//
// This source code is only intended as a supplement to the
// Microsoft Foundation Classes Reference and Microsoft
// QuickHelp and/or WinHelp documentation provided with the library.
// See these sources for detailed information regarding the
// Microsoft Foundation Classes product.


#include "stdafx.h"
#include <shellapi.h>

#ifdef AFX_OLE4_SEG
#pragma code_seg(AFX_OLE4_SEG)
#endif

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

#define new DEBUG_NEW

//////////////////////////////////////////////////////////////////////////////
// data for UpdateRegistry functionality

static char BASED_CODE sz00[] = "%2\0" "%5";
static char BASED_CODE sz01[] = "%2\\CLSID\0" "%1";
static char BASED_CODE sz02[] = "%2\\Insertable\0" "";
static char BASED_CODE sz03[] = "%2\\protocol\\StdFileEditing\\verb\\0\0" "&Edit";
static char BASED_CODE sz04[] = "%2\\protocol\\StdFileEditing\\server\0" "%3";
static char BASED_CODE sz05[] = "CLSID\\%1\0" "%5";
static char BASED_CODE sz06[] = "CLSID\\%1\\ProgID\0" "%2";
#ifndef _USRDLL
static char BASED_CODE sz07[] = "CLSID\\%1\\InprocHandler\0" "ole2.dll";
static char BASED_CODE sz08[] = "CLSID\\%1\\LocalServer\0" "%3";
#else
static char BASED_CODE sz07[] = "\0" "";
static char BASED_CODE sz08[] = "CLSID\\%1\\InProcServer\0" "%3";
#endif
static char BASED_CODE sz09[] = "CLSID\\%1\\Verb\\0\0" "&Edit,0,2";
static char BASED_CODE sz10[] = "CLSID\\%1\\Verb\\1\0" "&Open,0,2";
static char BASED_CODE sz11[] = "CLSID\\%1\\Insertable\0" "";
static char BASED_CODE sz12[] = "CLSID\\%1\\AuxUserType\\2\0" "%4";
static char BASED_CODE sz13[] = "CLSID\\%1\\AuxUserType\\3\0" "%5";
static char BASED_CODE sz14[] = "CLSID\\%1\\DefaultIcon\0" "%3,0";
static char BASED_CODE sz15[] = "CLSID\\%1\\MiscStatus\0" "32";

// registration for OAT_FULLSERVER
static LPCSTR BASED_CODE rglpszInPlaceRegister[] =
{
	sz00, sz02, sz03, sz05, sz07, sz09, sz10, sz11, sz12,
	sz13, sz15, NULL
};

// registration for OAT_MINISERVER
static LPCSTR BASED_CODE rglpszServerRegister[] =
{
	sz00, sz02, sz03, sz05, sz07, sz09, sz11, sz12,
	sz13, sz15, NULL
};
// overwrite entries for OAT_MINISERVER & OAT_FULLSERVER
static LPCSTR BASED_CODE rglpszServerOverwrite[] =
{
	sz01, sz04, sz06, sz08, sz14, NULL
};

// registration for OAT_CONTAINER
static LPCSTR BASED_CODE rglpszContainerRegister[] =
{
	sz00, sz05, sz07, NULL
};
// overwrite entries for OAT_CONTAINER
static LPCSTR BASED_CODE rglpszContainerOverwrite[] =
{
	sz01, sz06, sz08, sz14, NULL
};

// registration for OAT_DISPATCH_OBJECT
static LPCSTR BASED_CODE rglpszDispatchRegister[] =
{
	sz00, sz05, NULL
};
// overwrite entries for OAT_CONTAINER
static LPCSTR BASED_CODE rglpszDispatchOverwrite[] =
{
	sz01, sz06, sz08, NULL
};

struct STANDARD_ENTRY
{
	LPCSTR FAR* rglpszRegister;
	LPCSTR FAR* rglpszOverwrite;
};

static STANDARD_ENTRY BASED_CODE rgStdEntries[] =
{
	{ rglpszInPlaceRegister, rglpszServerOverwrite },
	{ rglpszServerRegister, rglpszServerOverwrite },
	{ rglpszContainerRegister, rglpszContainerOverwrite },
	{ rglpszDispatchRegister, rglpszDispatchOverwrite }
};

/////////////////////////////////////////////////////////////////////////////
// Special registration for apps that wish not to use REGLOAD

BOOL AFXAPI AfxOleRegisterServerClass(
	REFCLSID clsid, LPCSTR lpszClassName,
	LPCSTR lpszShortTypeName, LPCSTR lpszLongTypeName,
	OLE_APPTYPE nAppType,
	LPCSTR FAR* rglpszRegister, LPCSTR FAR* rglpszOverwrite)
{
	ASSERT(AfxIsValidString(lpszClassName));
	ASSERT(AfxIsValidString(lpszShortTypeName));
	ASSERT(*lpszShortTypeName != 0);
	ASSERT(AfxIsValidString(lpszLongTypeName));
	ASSERT(*lpszLongTypeName != 0);
	ASSERT(nAppType == OAT_INPLACE_SERVER || nAppType == OAT_SERVER ||
		nAppType == OAT_CONTAINER || nAppType == OAT_DISPATCH_OBJECT);

	// use standard registration entries if non given
	if (rglpszRegister == NULL)
		rglpszRegister = rgStdEntries[nAppType].rglpszRegister;
	if (rglpszOverwrite == NULL)
		rglpszOverwrite = rgStdEntries[nAppType].rglpszOverwrite;

	LPCSTR rglpszSymbols[5];
		// 0 - class ID
		// 1 - class name
		// 2 - executable path
		// 3 - short type name
		// 4 - long type name

	// convert the CLSID to a string
	LPSTR lpszClassID;
	::StringFromCLSID(clsid, &lpszClassID);
	if (lpszClassID == NULL)
	{
		TRACE0("Warning: StringFromCLSID failed in AfxOleRegisterServerName\n"
			"-- perhaps AfxOleInit() has not been called.\n");
		return FALSE;
	}
	rglpszSymbols[0] = lpszClassID;
	rglpszSymbols[1] = lpszClassName;

	// get path name to server
	char szPathName[_MAX_PATH];
	::GetModuleFileName(AfxGetInstanceHandle(), szPathName, _MAX_PATH);
	rglpszSymbols[2] = szPathName;

	// fill in rest of symbols
	rglpszSymbols[3] = lpszShortTypeName;
	rglpszSymbols[4] = lpszLongTypeName;

	BOOL bResult;
	bResult = AfxOleRegisterHelper(rglpszRegister, rglpszSymbols, 5, FALSE);
	if (bResult && rglpszOverwrite != NULL)
		bResult = AfxOleRegisterHelper(rglpszOverwrite, rglpszSymbols, 5, TRUE);

	// free memory for class ID
	ASSERT(lpszClassID != NULL);
	_AfxFreeTaskMem(lpszClassID);

	return bResult;
}

// writes key/value pairs to system registry
BOOL AFXAPI AfxOleRegisterHelper(LPCSTR FAR* rglpszRegister,
	LPCSTR FAR* rglpszSymbols, int nSymbols, BOOL bReplace)
{
	ASSERT(rglpszRegister != NULL);
	ASSERT(nSymbols == 0 || rglpszSymbols != NULL);

	CString strKey;
	CString strValue;

	// keeping a key open makes this go a bit faster
	static char BASED_CODE szCLSID[] = "CLSID";
	HKEY hKeyTemp = NULL;
	RegOpenKey(HKEY_CLASSES_ROOT, szCLSID, &hKeyTemp);

	BOOL bResult = TRUE;
	while (*rglpszRegister != NULL)
	{
		LPCSTR lpszKey = *rglpszRegister++;
		if (*lpszKey == '\0')
			continue;

		LPCSTR lpszValue = lpszKey + lstrlen(lpszKey) + 1;

		AfxFormatStrings(strKey, lpszKey, rglpszSymbols, nSymbols);
		AfxFormatStrings(strValue, lpszValue, rglpszSymbols, nSymbols);

		if (strKey.IsEmpty())
		{
			TRACE1("Warning: skipping empty key '%Fs'\n", lpszKey);
			continue;
		}

		if (!bReplace)
		{
			char szBuffer[256];
			LONG lSize = sizeof(szBuffer);
			if (::RegQueryValue(HKEY_CLASSES_ROOT, strKey, szBuffer, &lSize) ==
				ERROR_SUCCESS)
			{
#ifdef _DEBUG
				if (strValue != szBuffer)
				{
					TRACE3("Warning: Leaving value '%Fs' for key '%Fs' in "
						"registry\n\tintended value was '%Fs'\n",
						(LPCSTR)szBuffer, (LPCSTR)strKey, (LPCSTR)strValue);
				}
#endif
				continue;
			}
		}

		if (::RegSetValue(HKEY_CLASSES_ROOT, strKey, REG_SZ, strValue, 0)
			!= ERROR_SUCCESS)
		{
			TRACE2("Error: failed setting key '%Fs' to value '%Fs'\n",
				(LPCSTR)strKey, (LPCSTR)strValue);
			bResult = FALSE;
			break;
		}
	}

	if (hKeyTemp != NULL)
		RegCloseKey(hKeyTemp);

	return bResult;
}

/////////////////////////////////////////////////////////////////////////////
