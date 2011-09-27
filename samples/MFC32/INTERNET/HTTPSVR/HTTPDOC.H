// HttpSvrDoc.h : interface of the CHttpSvrDoc class
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

class CRequest;
class CListenSocket;

class CHttpSvrDoc : public CDocument
{
protected: // create from serialization only
	CHttpSvrDoc();
	DECLARE_DYNCREATE(CHttpSvrDoc)

// Attributes
public:
	BOOL StartListening( void );
	void StopListening( void );
	BOOL m_bResetListen;
	CString m_strTitleBase;
	CString	m_strRoot;
	CString m_strServer;
	CTime	m_timeStarted;
	UINT	m_uPort;
	int		m_nSvrName;
	BOOL	m_bLoggingOn;
	BOOL	m_bListIcon;
	BOOL	m_bAllowListing;
	// indexes into m_aReStats array....
	enum { STATUS_INFORMATIONAL = 0,
		STATUS_SUCCESS, STATUS_REDIRECTION,
		STATUS_CLIENTERR, STATUS_SERVERERR };
	int		m_aReStats[5]; // array of status group counts
	int		m_nTotalHits; // total hits
	CObList	m_reqList; // working list of request objects
	CListenSocket* m_pListen;

// Operations
public:
	void	DocHit( CRequest* pRequest );
	BOOL	IdleProc( LONG lCount );
	void	ExtractStats( CRequest* pRequest );

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CHttpSvrDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	virtual void OnCloseDocument();
	virtual void SetTitle(LPCTSTR lpszTitle);
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CHttpSvrDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CHttpSvrDoc)
	afx_msg void OnSvrOptions();
	afx_msg void OnFileRestart();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
