// This is a part of the Microsoft Foundation Classes C++ library.
// Copyright (C) 1992-1993 Microsoft Corporation
// All rights reserved.
//
// This source code is only intended as a supplement to the
// Microsoft Foundation Classes Reference and Microsoft
// QuickHelp and/or WinHelp documentation provided with the library.
// See these sources for detailed information regarding the
// Microsoft Foundation Classes product.

#include "stdafx.h"

#ifdef AFX_DBG1_SEG
#pragma code_seg(AFX_DBG1_SEG)
#endif

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// Diagnostic Stream output

void CDumpContext::OutputString(LPCSTR lpsz)
{
#ifdef _DEBUG // all CDumpContext output is controlled by afxTraceEnabled
	if (!afxTraceEnabled)
		return;
#endif //_DEBUG
#ifdef _WINDOWS
	if (m_pFile == NULL)
		::OutputDebugString(lpsz);
	else
#endif
		m_pFile->Write(lpsz, lstrlen(lpsz));
}

CDumpContext::CDumpContext(CFile* pFile)
{
	if (pFile)
		ASSERT_VALID(pFile);

	m_pFile = pFile;
	m_nDepth = 0;
}

void CDumpContext::Flush()
{
	if (m_pFile)
		m_pFile->Flush();
}

CDumpContext&
CDumpContext::operator<<(LPCSTR lpsz)
{
	if (lpsz == NULL)
		return *this;

#ifdef _DEBUG // all CDumpContext output is controlled by afxTraceEnabled
	if (!afxTraceEnabled)
		return *this;
#endif //_DEBUG

#ifdef _WINDOWS
	if (m_pFile == NULL)
	{
#ifndef _WINDLL
		char szBuffer[512];
#else
		static char szBuffer[512];
#endif
		LPSTR lpBuf = szBuffer;

		while (*lpsz != '\0')
		{
			if (lpBuf > szBuffer + sizeof(szBuffer) - 3)
			{
				*lpBuf = '\0';
				OutputString(szBuffer);
				lpBuf = szBuffer;
			}
			if (*lpsz == '\n')
				*lpBuf++ = '\r';
			*lpBuf++ = *lpsz++;
		}
		*lpBuf = '\0';
		OutputString(szBuffer);
		return *this;
	}
#endif
	m_pFile->Write(lpsz, lstrlen(lpsz));
	return *this;
}

CDumpContext& CDumpContext::operator<<(BYTE by)
{
	char szBuffer[32];

	sprintf(szBuffer, "%d", (int)by);
	OutputString(szBuffer);

	return *this;
}

CDumpContext& CDumpContext::operator<<(WORD w)
{
	char szBuffer[32];

	sprintf(szBuffer, "%u", (UINT) w);
	OutputString(szBuffer);

	return *this;
}

CDumpContext& CDumpContext::operator<<(UINT u)
{
	char szBuffer[32];

	sprintf(szBuffer, "0x%X", u);
	OutputString(szBuffer);

	return *this;
}

CDumpContext& CDumpContext::operator<<(LONG l)
{
	char szBuffer[32];

	sprintf(szBuffer, "%ld", l);
	OutputString(szBuffer);

	return *this;
}

CDumpContext& CDumpContext::operator<<(DWORD dw)
{
	char szBuffer[32];

	sprintf(szBuffer, "%lu", dw);
	OutputString(szBuffer);

	return *this;
}

CDumpContext& CDumpContext::operator<<(int n)
{
	char szBuffer[32];

	sprintf(szBuffer, "%d", n);
	OutputString(szBuffer);

	return *this;
}

CDumpContext& CDumpContext::operator<<(const CObject* pOb)
{
#ifdef _DEBUG // all CDumpContext output is controlled by afxTraceEnabled
	if (!afxTraceEnabled)
		return *this;
#endif //_DEBUG

	if (pOb == NULL)
		*this << "NULL";
	else
	{
		ASSERT_VALID(pOb);
		pOb->Dump(*this);
	}
	return *this;
}

CDumpContext& CDumpContext::operator<<(const CObject& ob)
{
	return *this << &ob;
}

#ifdef _NEARDATA
CDumpContext& CDumpContext::operator<<(const void NEAR* np)
{
	char szBuffer[32];

	// prefix a pointer with "$" and print in hex
	sprintf(szBuffer, "$%X", (WORD)np);
	OutputString(szBuffer);

	return *this;
}
#endif //_NEARDATA

CDumpContext& CDumpContext::operator<<(const void FAR* lp)
{
	char szBuffer[32];

	// prefix a pointer with "$" and print in hex
	sprintf(szBuffer, "$%lX", (LONG)lp);
	OutputString(szBuffer);

	return *this;
}

/////////////////////////////////////////////////////////////////////////////
// Formatted output

void CDumpContext::HexDump(const char* pszLine, BYTE* pby,
	int nBytes, int nWidth)
// do a simple hex-dump (8 per line) to a CDumpContext
//  the "pszLine" is a string to print at the start of each line
//    (%lx should be used to expand the current address)
{
	ASSERT(nBytes > 0);
	ASSERT(nWidth > 0);
	ASSERT(AfxIsValidString(pszLine));
	ASSERT(AfxIsValidAddress(pby, nBytes, FALSE));

#ifdef _DEBUG // all CDumpContext output is controlled by afxTraceEnabled
	if (!afxTraceEnabled)
		return;
#endif //_DEBUG

	int nRow = 0;
	char szBuffer[32];

	while (nBytes--)
	{
		if (nRow == 0)
		{
			sprintf(szBuffer, pszLine, pby);
			*this << szBuffer;
		}

		sprintf(szBuffer, " %02X", *pby++);
		*this << szBuffer;

		if (++nRow >= nWidth)
		{
			*this << "\n";
			nRow = 0;
		}
	}
	if (nRow != 0)
		*this << "\n";
}

/////////////////////////////////////////////////////////////////////////////
