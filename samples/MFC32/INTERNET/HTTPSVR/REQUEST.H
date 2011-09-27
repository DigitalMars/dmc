// Request.h : interface of the CRequest class
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


class CRequest : public CObject
{
	DECLARE_DYNCREATE(CRequest)

public:
	// Construction....
	CRequest();
	~CRequest();

	// Attributes....
	CString				m_strPathTranslated;
	CString				m_strPathInfo;
	CString				m_strHost; // host's address
	CString				m_strMethod; // GET, HEAD or POST
	CString				m_strURL;
	CString				m_strVersion; // HTTP/1.0
	CMapStringToString	m_mapHeaders;
	CByteArray			m_baBody;
	int					m_cbBody;
	CString				m_strFullPath;
	DWORD				m_dwAttr;
	DWORD				m_dwExecute; // executable type
	CString				m_strArgs; // string after '?'
	UINT				m_uStatus;
	BOOL				m_bDone;
	UINT				m_cbSent;
	CTime				m_timeReq; // time of request

	// access routines....
	CString	GetHeaderValue( CString strName );
	int		AddRef( void );
	int		Release( void );

	enum AppFlags { 
		APP_NONE	= 0x0000, 
		APP_EXECUTE	= 0x0001,
		APP_CGI		= 0x0002, 
		APP_ISAPI	= 0x0004 };

protected:
	int		m_nRefs;
};

