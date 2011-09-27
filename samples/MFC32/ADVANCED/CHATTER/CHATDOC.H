// chatdoc.h : interface of the CChatDoc class
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

#include "chatsock.h"

class CChatDoc : public CDocument
{
protected: // create from serialization only
	CChatDoc();
	DECLARE_DYNCREATE(CChatDoc)

// Attributes
public:
	BOOL m_bAutoChat;
	CString m_strHandle;
	CChatSocket* m_pSocket;
	CSocketFile* m_pFile;
	CArchive* m_pArchiveIn;
	CArchive* m_pArchiveOut;

// Operations
public:
	BOOL ConnectSocket(LPCTSTR lpszHandle, LPCTSTR lpszAddress, UINT nPort);
	void ProcessPendingRead();
	void SendMsg(CString& strText);
	void ReceiveMsg();
	void DisplayMsg(LPCTSTR lpszText);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CChatDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void DeleteContents();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CChatDoc();
	virtual void Serialize(CArchive& ar);   // overridden for document i/o
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CChatDoc)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
