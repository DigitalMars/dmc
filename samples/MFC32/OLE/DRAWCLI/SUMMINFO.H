// summinfo.h : interface of the CSummInfo classes
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

#if !defined(_MAC)

#include "propset.h"

extern const OLECHAR szSummInfo[];

DEFINE_GUID( FMTID_SummaryInformation, 0xF29F85E0, 0x4FF9, 0x1068, 0xAB, 0x91, 0x08, 0x00, 0x2B, 0x27, 0xB3, 0xD9 ) ;

#undef PID_DICTIONARY
#define PID_DICTIONARY 0X00000000

#undef PID_CODEPAGE
#define PID_CODEPAGE 0X00000001

#define PID_TITLE 0X00000002
#define PID_SUBJECT 0X00000003
#define PID_AUTHOR 0X00000004
#define PID_KEYWORDS 0X00000005
#define PID_COMMENTS 0X00000006
#define PID_TEMPLATE 0X00000007
#define PID_LASTAUTHOR 0X00000008
#define PID_REVNUMBER 0X00000009
#define PID_EDITTIME 0X0000000A
#define PID_LASTPRINTED 0X0000000B
#define PID_CREATE_DTM 0X0000000C
#define PID_LASTSAVE_DTM 0X0000000D
#define PID_PAGECOUNT 0X0000000E
#define PID_WORDCOUNT 0X0000000F
#define PID_CHARCOUNT 0X00000010
#define PID_THUMBNAIL 0X00000011
#define PID_APPNAME 0X00000012

#undef PID_SECURITY
#define PID_SECURITY 0X00000013

class CSummInfo
{
public:
	CSummInfo();
	BOOL SetTitle(LPCTSTR szTitle);
	CString GetTitle();
	BOOL SetSubject(LPCTSTR szSubject);
	CString GetSubject();
	BOOL SetAuthor(LPCTSTR szAuthor);
	CString GetAuthor();
	BOOL SetKeywords(LPCTSTR szKeywords);
	CString GetKeywords();
	BOOL SetComments(LPCTSTR szComments);
	CString GetComments();
	BOOL SetTemplate(LPCTSTR szTemplate);
	CString GetTemplate();
	BOOL SetLastAuthor(LPCTSTR szLastAuthor);
	CString GetLastAuthor();
	BOOL IncrRevNum();
	CString GetRevNum();
	void StartEditTimeCount();
	BOOL AddCountToEditTime();
	CString GetEditTime();
	BOOL RecordPrintDate();
	CString GetLastPrintDate();
	BOOL RecordCreateDate();
	CString GetCreateDate();
	BOOL RecordSaveDate();
	CString GetLastSaveDate();
	BOOL SetNumPages(ULONG nPages);
	CString GetNumPages();
	BOOL SetNumWords(ULONG nWords);
	CString GetNumWords();
	BOOL SetNumChars(ULONG nChars);
	CString GetNumChars();
	BOOL SetAppname(LPCTSTR szAppname);
	CString GetAppname();
	BOOL SetSecurity(ULONG nLevel);
	CString GetSecurity();
	BOOL WriteToStorage(LPSTORAGE lpRootStg);
	BOOL ReadFromStorage(LPSTORAGE lpRootStg);
protected:
	CPropertySet m_propSet;
	CPropertySection* m_pSection;
	__int64 startEdit;
};

#endif // !defined(_MAC)
