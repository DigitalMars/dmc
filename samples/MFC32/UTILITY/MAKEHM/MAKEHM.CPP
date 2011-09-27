// makehm.cpp : A simple utility to create .HM (Help Map) files from
//              .H (resource header files).
//
//  This utility creates a .HM file from a header file (usually
//  resource.h) which allows one to maintain the help mappings
//  without a lot of manual effort on the part of the programmer
//  or help author.
//
//  This is done by naming convention, the naming convention used
//  being that normally accepted as standard Windows and MFC
//  programming practices.
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

#include <ctype.h>
#include <afxcoll.h>	// also includes <afx.h>

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

#define SIGNON_VER 1
#define SIGNON_REV 0

/////////////////////////////////////////////////////////////////////////////

void UsageErr(const char* szErrorMessage = NULL,
			  const char* szErrorParam = NULL)
{
	fprintf(stderr,
		"\nMicrosoft (R) Help Maintainence Utility   Version %d.%02d\n"
		"Copyright (c) Microsoft Corp. 1992-1995. All rights reserved.\n\n",
		SIGNON_VER, SIGNON_REV);

	if (szErrorMessage != NULL)
	{
		fprintf(stderr, "makehm: error: ");
		fprintf(stderr, szErrorMessage, szErrorParam);
		fprintf(stderr, ".\n\n");
	}

	fprintf(stderr, "makehm usage:\n\n"
	"  makehm <from>,<to>,<add>... <resource.h> [output.hm]\n"
	"\n"
	"    <from>,<to>,<add> fields must appear as one argument and\n"
	"       are separated by commas.  A set of these arguments may\n"
	"       appear more than once.\n"
	"    <from> - identifies the symbol prefix to map from (ie. ID_)\n"
	"    <to>   - identifies the symbol prefix to map to (ie. HID_)\n"
	"    <add>  - identifies a numeric offset to be used (ie. 0x10000)\n"
	"\n"
	"    <resource.h> - identifies the input header file (ie. resource.h).\n"
	"    [output.hm] - identifies the output help map file.  If not\n"
	"       supplied, output is written to stdout.\n");

	exit(1);
}

/////////////////////////////////////////////////////////////////////////////

class CLineFile : public CStdioFile
{
public:
	BOOL ReadLine(CString& stringLine);
	void WriteLine(const CString& stringLine);
	void SafeOpen(const CString& string, UINT nStyleFlags);
};

BOOL CLineFile::ReadLine(CString& str)
{
	UINT nMax = 512;
	for (;;)
	{
		LONG pos = GetPosition();
		if (!ReadString(str.GetBuffer(nMax), nMax))
			return FALSE;
		str.ReleaseBuffer();
		if (str.GetLength() < (int)nMax-1)
			return TRUE;
		nMax += 128;
		Seek(pos, CFile::begin);
	}
	ASSERT(FALSE);
}

void CLineFile::WriteLine(const CString& str)
{
	ASSERT(str[str.GetLength()-1] == '\n');
	WriteString(str);
}

void CLineFile::SafeOpen(const CString& name, UINT nStyleFlags)
{
	BOOL fSuccess = Open(name, nStyleFlags, 0);
	if (!fSuccess)
		UsageErr("unable to open file \"%s\"", name);
}

/////////////////////////////////////////////////////////////////////////////

BOOL IsValidSymbol(const char* psz)
{
	if (!__iscsymf(*psz))
		return FALSE;

	ASSERT(*psz != 0);
	++psz;
	while (*psz)
	{
		if (!__iscsym(*psz))
			return FALSE;
		++psz;
	}
	return TRUE;
}

#define isodigit(i) ('0' <= (i) && (i) <= '7')

BOOL IsValidValue(const char* psz, DWORD& dwValue)
{
	if (*psz == 0 || !isdigit(*psz))
		return FALSE;

	DWORD dw = 0;
	if (psz[0] == '0' && (psz[1] == 'x' || psz[1] == 'X'))
	{
		if (psz[1] == 0)
			return FALSE;
		psz += 2;
		while (isxdigit(*psz))
		{
			dw *= 16;
			dw += isdigit(*psz) ? *psz - '0' : 10 + (*psz|0x20) - 'a';
			++psz;
		}
	}
	else if (psz[0] == '0')
	{
		while (isodigit(*psz))
		{
			dw *= 8;
			dw += *psz - '0';
			++psz;
		}
	}
	else
	{
		while (isdigit(*psz))
		{
			dw *= 10;
			dw += *psz - '0';
			++psz;
		}
	}

	if (*psz != 0)
		return FALSE;

	dwValue = dw;
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////

CPtrArray aMap;

struct CMapInfo
{
	char* pszPrefixFrom;
	char* pszPrefixTo;
	DWORD dwAddTo;
};

void AddToMap(char* pszArg)
{
	ASSERT(pszArg != NULL);
	char* psz = _strdup(pszArg);
	if (psz == NULL)
	{
		AfxThrowMemoryException();
		ASSERT(FALSE);
	}
	ASSERT(strchr(psz, ',') != NULL);

	char* pszPrefixFrom;
	char* pszPrefixTo;
	char* pszAddTo;
	DWORD dwAddTo;
	CMapInfo* pInfo;

	// parse each field out of the argument.
	pszPrefixFrom = strtok(psz, ",");
	pszPrefixTo = strtok(NULL, ",");
	if (pszPrefixTo == NULL)
		goto ParmError;
	pszAddTo = strtok(NULL, ",");
	if (pszAddTo == NULL)
		goto ParmError;

	// make sure they are valid symbols/values.
	if (!IsValidSymbol(pszPrefixFrom) || !IsValidSymbol(pszPrefixTo))
		goto ParmError;
	if (!IsValidValue(pszAddTo, dwAddTo))
		goto ParmError;

	// add them to the map.
	pInfo = new CMapInfo;
	ASSERT(pInfo);
	pInfo->pszPrefixFrom = pszPrefixFrom;
	pInfo->pszPrefixTo = pszPrefixTo;
	pInfo->dwAddTo = dwAddTo;
	aMap.Add(pInfo);
	return;

ParmError:
	UsageErr("parameter \"%s\" not correctly formed.", pszArg);
	ASSERT(FALSE);
}

CMapInfo* FindInMap(const char* psz)
{
	ASSERT(psz != NULL);
	ASSERT(*psz != 0);
	int nMax = aMap.GetSize();
	for (int i = 0; i < nMax; i++)
	{
		CMapInfo* pInfo = (CMapInfo*)aMap.GetAt(i);
		size_t nLen = strlen(pInfo->pszPrefixFrom);
		if (strncmp(pInfo->pszPrefixFrom, psz, nLen) == 0)
			return pInfo;
	}
	return NULL;
}

/////////////////////////////////////////////////////////////////////////////

BOOL
MapNameValue(const char* pszName, CString& strNewName, DWORD& dwValue)
{
	CMapInfo* pInfo = FindInMap(pszName);
	if (pInfo == NULL)
		return FALSE;

	CString strName = pszName;
	strNewName = (CString)pInfo->pszPrefixTo +
		strName.Right(strName.GetLength()-strlen(pInfo->pszPrefixFrom));
	dwValue += pInfo->dwAddTo;
	return TRUE;
}

CString StringFromDword(DWORD dwValue)
{
	char buf[sizeof "0x12345678"];
	sprintf(buf, "0x%lX", dwValue);
	return CString(buf);
}

BOOL MapLine(CString& strLine)
{
	static char szWhiteSpace1[] = "\t ";
	static char szWhiteSpace2[] = "\t\n ";
	static char szDefine[] = "#define";

	char* pszCopy = _strdup(strLine);
	if (pszCopy == NULL)
	{
		AfxThrowMemoryException();
		ASSERT(FALSE);
	}

	char* psz;
	char* pszSymbol;
	char* pszValue;
	DWORD dwValue;
	CString strNewName;

	// '//{{NOHELP}}' can be placed on the line and it will not be included
	if (strstr(strLine, "//{{NOHELP}}") != NULL)
		goto RetFalse;

	psz = strtok(pszCopy, szWhiteSpace1);
	if (psz == NULL)
		goto RetFalse;
	if (strcmp(psz, szDefine) != 0)
		goto RetFalse;
	pszSymbol = strtok(NULL, szWhiteSpace1);
	if (pszSymbol == NULL)
		goto RetFalse;
	pszValue = strtok(NULL, szWhiteSpace2);
	if (pszValue == NULL)
		goto RetFalse;

	if (!IsValidSymbol(pszSymbol))
		goto RetFalse;
	if (!IsValidValue(pszValue, dwValue))
		goto RetFalse;
	if (!MapNameValue(pszSymbol, strNewName, dwValue))
		goto RetFalse;

	//BLOCK: format output line
	{
		CString strPad(' ', 40-strNewName.GetLength());
		if (strPad.IsEmpty())
			strPad = '\t';
		strLine = strNewName + strPad + StringFromDword(dwValue) + "\n";
	}

	ASSERT(pszCopy != NULL);
	free(pszCopy);
	return TRUE;

RetFalse:
	ASSERT(pszCopy != NULL);
	free(pszCopy);
	return FALSE;
}

/////////////////////////////////////////////////////////////////////////////

int main(int argc, char** argv)
{
	// add symbol mappings to the map.
	BOOL fAddedToMap = FALSE;
	for (int i = 1; i < argc && strchr(argv[i], ',') != NULL; i++)
	{
		AddToMap(argv[i]);
		fAddedToMap = TRUE;
	}

	// must only have 1-2 parms left on command line.
	if (!fAddedToMap || i < argc-2 || i > argc-1)
	{
		UsageErr(NULL, NULL);
		ASSERT(FALSE);
	}

	// open input file.
	CLineFile fileIn;
	fileIn.SafeOpen(argv[i], CLineFile::modeRead);

	// open/hook up output file.
	CLineFile fileOut;
	if (i+1 < argc)
		fileOut.SafeOpen(argv[i+1], CLineFile::modeWrite | CLineFile::modeCreate);
	else
		fileOut.m_pStream = stdout;

	// process the file.
	CString strLine;
	while (fileIn.ReadLine(strLine))
	{
		if (MapLine(strLine))
		{
			fileOut.WriteLine(strLine);
		}
	}

	// close files.
	fileIn.Close();
	fileOut.Close();

	return 0;
}

/////////////////////////////////////////////////////////////////////////////
