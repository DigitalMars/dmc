// chatdoc.cpp : implementation of the CChatDoc class
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

#include "stdafx.h"
#include "chatter.h"

#include "chatsock.h"
#include "chatdoc.h"
#include "chatvw.h"
#include "setupdlg.h"
#include "msg.h"

#ifdef _WIN32
#ifndef _UNICODE
#include <strstrea.h>
#endif
#endif

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CChatDoc

IMPLEMENT_DYNCREATE(CChatDoc, CDocument)

BEGIN_MESSAGE_MAP(CChatDoc, CDocument)
	//{{AFX_MSG_MAP(CChatDoc)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CChatDoc construction/destruction

CChatDoc::CChatDoc()
{
	m_bAutoChat = FALSE;
	m_pSocket = NULL;
	m_pFile = NULL;
	m_pArchiveIn = NULL;
	m_pArchiveOut = NULL;
}

CChatDoc::~CChatDoc()
{
}

BOOL CChatDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

#ifdef _WIN32
#ifndef _UNICODE
	if (AfxGetApp()->m_lpCmdLine[0] != '\0')
	{
		TCHAR strHandle[128];
		TCHAR strServer[128];
		int nChannel;

		istrstream(AfxGetApp()->m_lpCmdLine) >> strHandle >> strServer >> nChannel;
		return ConnectSocket(strHandle, strServer, nChannel);
	}
	else
#endif
#endif
	{
		CSetupDlg Dialog;

		Dialog.m_strHandle=m_strHandle;
		Dialog.m_strServer=_T("");
		Dialog.m_nChannel=0;

		while(TRUE)
		{
			if (IDOK != Dialog.DoModal())
				return FALSE;

			if (ConnectSocket(Dialog.m_strHandle, Dialog.m_strServer, Dialog.m_nChannel))
				return TRUE;

			if (AfxMessageBox(IDS_CHANGEADDRESS,MB_YESNO) == IDNO)
				return FALSE;
		}
	}
}

void CChatDoc::DeleteContents() 
{
	m_bAutoChat = FALSE;

	if ((m_pSocket != NULL) && (m_pFile != NULL) &&	(m_pArchiveOut != NULL))
	{
		CMsg msg;
		CString strTemp;

		if (strTemp.LoadString(IDS_DISCONNECT))
		{
			msg.m_bClose = TRUE;
			msg.m_strText = m_strHandle + strTemp;
			msg.Serialize(*m_pArchiveOut);
			m_pArchiveOut->Flush();
		}
	}

	delete m_pArchiveOut;
	m_pArchiveOut = NULL;
	delete m_pArchiveIn;
	m_pArchiveIn = NULL;
	delete m_pFile;
	m_pFile = NULL;

	if (m_pSocket != NULL)
	{
		BYTE Buffer[50];
		m_pSocket->ShutDown();

		while(m_pSocket->Receive(Buffer,50) > 0);
	}

	delete m_pSocket;
	m_pSocket = NULL;

	for(POSITION pos=GetFirstViewPosition();pos!=NULL;)
	{
		CView* pView = GetNextView(pos);
		
		if (pView->IsKindOf(RUNTIME_CLASS(CChatView)))
		{
			CChatView* pChatView = (CChatView*)pView;
			pChatView->GetEditCtrl().SetWindowText(_T(""));
		}	
	}
	CDocument::DeleteContents();
}

/////////////////////////////////////////////////////////////////////////////
// CChatDoc Operations

BOOL CChatDoc::ConnectSocket(LPCTSTR lpszHandle, LPCTSTR lpszAddress, UINT nPort)
{
	m_strHandle = lpszHandle;

	m_pSocket = new CChatSocket(this);

	if (!m_pSocket->Create())
	{
		delete m_pSocket;
		m_pSocket = NULL;
		AfxMessageBox(IDS_CREATEFAILED);
		return FALSE;
	}

	while (!m_pSocket->Connect(lpszAddress, nPort + 700))
	{
		if (AfxMessageBox(IDS_RETRYCONNECT,MB_YESNO) == IDNO)
		{
			delete m_pSocket;
			m_pSocket = NULL;
			return FALSE;
		}
	}

	m_pFile = new CSocketFile(m_pSocket);
	m_pArchiveIn = new CArchive(m_pFile,CArchive::load);
	m_pArchiveOut = new CArchive(m_pFile,CArchive::store);

	CString strTemp;
	if (strTemp.LoadString(IDS_CONNECT))
		SendMsg(strTemp);

	return TRUE;
}

void CChatDoc::ProcessPendingRead() 
{
	do
	{
		ReceiveMsg();
		if (m_pSocket == NULL)
			return;
	}
	while(!m_pArchiveIn->IsBufferEmpty());
}

void CChatDoc::SendMsg(CString& strText)
{
	if (m_pArchiveOut != NULL)
	{
		CMsg msg;

		msg.m_strText = m_strHandle + _T(": ") + strText;

		TRY
		{
			msg.Serialize(*m_pArchiveOut);
			m_pArchiveOut->Flush();
		}
		CATCH(CFileException, e)
		{
			m_bAutoChat = FALSE;
			m_pArchiveOut->Abort();
			delete m_pArchiveOut;
			m_pArchiveOut = NULL;
		
			CString strTemp;
			if (strTemp.LoadString(IDS_SERVERRESET))
				DisplayMsg(strTemp);
		}
		END_CATCH
	}
}

void CChatDoc::ReceiveMsg()
{
	CMsg msg;
	
	TRY
	{
		msg.Serialize(*m_pArchiveIn);

		while(!msg.m_msgList.IsEmpty())
		{
			CString temp = msg.m_msgList.RemoveHead();
			DisplayMsg(temp);
		}

	}
	CATCH(CFileException, e)
	{
		m_bAutoChat = FALSE;
		msg.m_bClose = TRUE;
		m_pArchiveOut->Abort();

		CString strTemp;
		if (strTemp.LoadString(IDS_SERVERRESET))
			DisplayMsg(strTemp);
		if (strTemp.LoadString(IDS_CONNECTIONCLOSED))
			DisplayMsg(strTemp);
	}
	END_CATCH

	if (msg.m_bClose)
	{
		delete m_pArchiveIn;
		m_pArchiveIn = NULL;
		delete m_pArchiveOut;
		m_pArchiveOut = NULL;
		delete m_pFile;
		m_pFile = NULL;
		delete m_pSocket;
		m_pSocket = NULL;
	}
}

void CChatDoc::DisplayMsg(LPCTSTR lpszText)
{

	for(POSITION pos=GetFirstViewPosition();pos!=NULL;)
	{
		CView* pView = GetNextView(pos);
		CChatView* pChatView = DYNAMIC_DOWNCAST(CChatView, pView);
		
		if (pChatView != NULL)
			pChatView->Message(lpszText);
	}
}

/////////////////////////////////////////////////////////////////////////////
// CChatDoc serialization

void CChatDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		for(POSITION pos=GetFirstViewPosition();pos!=NULL;)
		{
			CView* pView = GetNextView(pos);
			CChatView* pChatView = DYNAMIC_DOWNCAST(CChatView, pView);
		
			if (pChatView != NULL)
				pChatView->SerializeRaw(ar);
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
// CChatDoc diagnostics

#ifdef _DEBUG
void CChatDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CChatDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CChatDoc commands

