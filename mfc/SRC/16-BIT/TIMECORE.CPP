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

#ifdef AFX_AUX_SEG
#pragma code_seg(AFX_AUX_SEG)
#endif

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

#ifdef _WINDOWS
#define sprintf wsprintf
#endif

/////////////////////////////////////////////////////////////////////////////
// CTime - absolute time

CTime::CTime(int nYear, int nMonth, int nDay, int nHour, int nMin, int nSec)
{
	struct tm atm;
	atm.tm_sec = nSec;
	atm.tm_min = nMin;
	atm.tm_hour = nHour;
	ASSERT(nDay >= 1 && nDay <= 31);
	atm.tm_mday = nDay;
	ASSERT(nMonth >= 1 && nMonth <= 12);
	atm.tm_mon = nMonth - 1;        // tm_mon is 0 based
	ASSERT(nYear >= 1900);
	atm.tm_year = nYear - 1900;     // tm_year is 1900 based
	atm.tm_isdst = -1;
	m_time = mktime(&atm);
	ASSERT(m_time != -1);       // indicates an illegal input time
}

CTime::CTime(WORD wDosDate, WORD wDosTime)
{
	struct tm atm;
	atm.tm_sec = (wDosTime & ~0xFFE0) << 1;;
	atm.tm_min = (wDosTime & ~0xF800) >> 5;
	atm.tm_hour = wDosTime >> 11;

	atm.tm_mday = wDosDate & ~0xFFE0;
	atm.tm_mon = ((wDosDate & ~0xFE00) >> 5) - 1;
	atm.tm_year = (wDosDate >> 9) + 80;
	atm.tm_isdst = -1;
	m_time = mktime(&atm);
	ASSERT(m_time != -1);       // indicates an illegal input time
}


CTime PASCAL CTime::GetCurrentTime()
// return the current system time
{
	return CTime(::time(NULL));
}

struct tm* CTime::GetGmtTm(struct tm* ptm /* = NULL */) const
// NOTE: uses global static buffer
{
	if (ptm != NULL)
	{
		*ptm = *gmtime(&m_time);
		return ptm;
	}
	else
		return gmtime(&m_time);
}

struct tm* CTime::GetLocalTm(struct tm* ptm /* = NULL */) const
// NOTE: uses global static buffer
{
	if (ptm != NULL)
	{
		struct tm* ptmTemp = localtime(&m_time);
		if (ptmTemp == NULL)
			return NULL;    // indicates the m_time was not initialized!

		*ptm = *ptmTemp;
		return ptm;
	}
	else
		return localtime(&m_time);
}

#ifdef _DEBUG
CDumpContext& AFXAPI operator <<(CDumpContext& dc, CTime time)
{
	char* psz = ctime(&time.m_time);
	if ((psz == NULL) || (time.m_time == 0))
		return dc << "CTime(invalid #" << time.m_time << ")";

	// format it
	psz[24] = '\0';         // nuke newline
	return dc << "CTime(\"" << psz << "\")";
}
#endif

CArchive& AFXAPI operator <<(CArchive& ar, CTime time)
{
	return ar << (DWORD) time.m_time;
}

CArchive& AFXAPI operator >>(CArchive& ar, CTime& rtime)
{
	return ar >> (DWORD&) rtime.m_time;
}

/////////////////////////////////////////////////////////////////////////////
// CTimeSpan - relative time

#ifdef _DEBUG
CDumpContext& AFXAPI operator <<(CDumpContext& dc, CTimeSpan timeSpan)
{
	return dc << "CTimeSpan(" << timeSpan.GetDays() << " days, " <<
		 timeSpan.GetHours() << " hours, " <<
		 timeSpan.GetMinutes() << " minutes and " <<
		 timeSpan.GetSeconds() << " seconds)";
}
#endif

CArchive& AFXAPI operator <<(CArchive& ar, CTimeSpan timeSpan)
{
	return ar << (DWORD) timeSpan.m_timeSpan;
}

CArchive& AFXAPI operator >>(CArchive& ar, CTimeSpan& rtimeSpan)
{
	return ar >> (DWORD&) rtimeSpan.m_timeSpan;
}


/////////////////////////////////////////////////////////////////////////////
// String formatting

#if !defined(_AFXDLL) && !defined(_USRDLL)

#define maxTimeBufferSize       128
	// Verifies will fail if the needed buffer size is too large

CString CTimeSpan::Format(const char* pFormat) const
// formatting timespans is a little trickier than formatting CTimes
//  * we are only interested in relative time formats, ie. it is illegal
//      to format anything dealing with absolute time (i.e. years, months,
//         day of week, day of year, timezones, ...)
//  * the only valid formats:
//      %D - # of days -- NEW !!!
//      %H - hour in 24 hour format
//      %M - minute (0-59)
//      %S - seconds (0-59)
//      %% - percent sign
{
	char szBuffer[maxTimeBufferSize];
	char ch;
	char* pch = szBuffer;

	while ((ch = *pFormat++) != '\0')
	{
		ASSERT(pch < &szBuffer[maxTimeBufferSize]);
		if (ch == '%')
		{
			int num;
			switch (ch = *pFormat++)
			{
			default:
				ASSERT(FALSE);      // probably a bad format character
			case '%':
				*pch++ = ch;
				break;
			case 'D':
				pch += sprintf(pch, "%ld", GetDays());
				break;
			case 'H':
				num = GetHours();
				goto format_num;
			case 'M':
				num = GetMinutes();
				goto format_num;
			case 'S':
				num = GetSeconds();
format_num:
				pch += sprintf(pch, "%02d", num);
				break;
			}
		}
		else
		{
			*pch++ = ch;
			if (_AfxIsDBCSLeadByte(ch))
			{
				ASSERT(pch < &szBuffer[maxTimeBufferSize]);
				*pch++ = *pFormat++;
			}
		}
	}
	*pch = '\0';

	return szBuffer;
}

CString CTime::Format(const char* pFormat) const
{
	char    szBuffer[maxTimeBufferSize];

	struct tm* ptmTemp = localtime(&m_time);
	ASSERT(ptmTemp != NULL); // make sure the time has been initialized!

	if (!strftime(szBuffer, sizeof(szBuffer), pFormat, ptmTemp))
		szBuffer[0] = '\0';

	return szBuffer;
}

CString CTime::FormatGmt(const char* pFormat) const
{
	char    szBuffer[maxTimeBufferSize];

	if (!strftime(szBuffer, sizeof(szBuffer), pFormat, gmtime(&m_time)))
		szBuffer[0] = '\0';

	return szBuffer;
}
#endif //! DLL variant

/////////////////////////////////////////////////////////////////////////////
