// srvrdoc.cpp : implementation of the CServerDoc class
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
#include "chatsrvr.h"

#include "srvrdoc.h"
#include "srvrvw.h"

#include "msg.h"
#include "dialogs.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CServerDoc

IMPLEMENT_DYNCREATE(CServerDoc, CDocument)

BEGIN_MESSAGE_MAP(CServerDoc, CDocument)
	//{{AFX_MSG_MAP(CServerDoc)
	//}}AFX_MSG_MAP
	ON_UPDATE_COMMAND_UI(ID_MESSAGES, OnUpdateMessages)
	ON_UPDATE_COMMAND_UI(ID_CONNECTIONS, OnUpdateConnections)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CServerDoc construction/destruction

CServerDoc::CServerDoc()
{
	m_pSocket = NULL;
}

CServerDoc::~CServerDoc()
{
	delete m_pSocket;
}

BOOL CServerDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	CDiscussionDlg Dialog;
	
	if (Dialog.DoModal() == IDOK)
	{
		m_pSocket = new CListeningSocket(this);
		if (m_pSocket->Create(Dialog.m_nPort+700))
		{
			if (m_pSocket->Listen())
				return TRUE;
		}
	}
	return FALSE;
}

void CServerDoc::DeleteContents() 
{
	delete m_pSocket;
	m_pSocket = NULL;

	CString temp;
	if (temp.LoadString(IDS_SERVERSHUTDOWN))
		m_msgList.AddTail(temp);

	while(!m_connectionList.IsEmpty())
	{
		CClientSocket* pSocket = (CClientSocket*)m_connectionList.RemoveHead();
		CMsg* pMsg = AssembleMsg(pSocket);
		pMsg->m_bClose = TRUE;

		SendMsg(pSocket, pMsg);

		if (!pSocket->IsAborted())
		{
			pSocket->ShutDown();

			BYTE Buffer[50];

			while (pSocket->Receive(Buffer,50) > 0);

			delete pSocket;
		}
	}

	m_msgList.RemoveAll();
		
	if (!m_viewList.IsEmpty())
		((CEditView*)m_viewList.GetHead())->SetWindowText(_T(""));

	CDocument::DeleteContents();
}

/////////////////////////////////////////////////////////////////////////////
// CServerDoc serialization

void CServerDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// CEditView contains an edit control which handles all serialization
		((CEditView*)m_viewList.GetHead())->SerializeRaw(ar);
	}
}

/////////////////////////////////////////////////////////////////////////////
// CServerDoc diagnostics

#ifdef _DEBUG
void CServerDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CServerDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CServerDoc Operations

void CServerDoc::UpdateClients()
{
	for(POSITION pos = m_connectionList.GetHeadPosition(); pos != NULL;)
	{
		CClientSocket* pSocket = (CClientSocket*)m_connectionList.GetNext(pos);
		CMsg* pMsg = AssembleMsg(pSocket);

		if (pMsg != NULL)
			SendMsg(pSocket, pMsg);
	}
}

void CServerDoc::ProcessPendingAccept() 
{
	CClientSocket* pSocket = new CClientSocket(this);

	if (m_pSocket->Accept(*pSocket))
	{
		pSocket->Init();
		m_connectionList.AddTail(pSocket);
	}
	else
		delete pSocket;
}

void CServerDoc::ProcessPendingRead(CClientSocket* pSocket)
{
	do
	{
		CMsg* pMsg = ReadMsg(pSocket);

		if (pMsg->m_bClose)	
		{
			CloseSocket(pSocket);
			break;
		}
	}
	while (!pSocket->m_pArchiveIn->IsBufferEmpty());
	
	UpdateClients();
}

CMsg* CServerDoc::AssembleMsg(CClientSocket* pSocket)
{
	static CMsg msg;

	msg.Init();

	if (pSocket->m_nMsgCount >= m_msgList.GetCount())
		return NULL;

	for (POSITION pos1 = m_msgList.FindIndex(pSocket->m_nMsgCount); pos1 != NULL;)
	{
		CString temp = m_msgList.GetNext(pos1);
		msg.m_msgList.AddTail(temp);
	}
	pSocket->m_nMsgCount = m_msgList.GetCount();
	return &msg;
}

CMsg* CServerDoc::ReadMsg(CClientSocket* pSocket)
{
	static CMsg msg;

	TRY
	{
		pSocket->ReceiveMsg(&msg);

		Message(msg.m_strText);

		m_msgList.AddTail(msg.m_strText);
	}
	CATCH(CFileException, e)
	{
		CString strTemp;
		if (strTemp.LoadString(IDS_READERROR))
			Message(strTemp);

		msg.m_bClose = TRUE;
		pSocket->Abort();
	}
	END_CATCH

	return &msg;
}

void CServerDoc::SendMsg(CClientSocket* pSocket, CMsg* pMsg)
{
	TRY
	{
		pSocket->SendMsg(pMsg);
	}
	CATCH(CFileException, e)
	{
		pSocket->Abort();

		CString strTemp;
		if (strTemp.LoadString(IDS_SENDERROR))
			Message(strTemp);
	}
	END_CATCH
}

void CServerDoc::CloseSocket(CClientSocket* pSocket)
{
	pSocket->Close();

	POSITION pos,temp;
	for(pos = m_connectionList.GetHeadPosition(); pos != NULL;)
	{
		temp = pos;
		CClientSocket* pSock = (CClientSocket*)m_connectionList.GetNext(pos);
		if (pSock == pSocket)
		{
			m_connectionList.RemoveAt(temp);
			break;
		}
	}

	delete pSocket;
}

void CServerDoc::Message(LPCTSTR lpszMessage)
{
	((CServerView*)m_viewList.GetHead())->Message(lpszMessage);
}

/////////////////////////////////////////////////////////////////////////////
// CServerDoc Handlers

void CServerDoc::OnUpdateMessages(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(TRUE);

	CString strFmt;
	if (strFmt.LoadString(IDS_MESSAGESFMT))
	{
		CString strTemp;
		wsprintf(strTemp.GetBuffer(50),strFmt,m_msgList.GetCount());
        strTemp.ReleaseBuffer();
		pCmdUI->SetText(strTemp);
	}
}

void CServerDoc::OnUpdateConnections(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(TRUE);

	CString strFmt;
	if (strFmt.LoadString(IDS_CONNECTIONSFMT))
	{
		CString strTemp;
		wsprintf(strTemp.GetBuffer(50),strFmt,m_connectionList.GetCount());
		strTemp.ReleaseBuffer();
		pCmdUI->SetText(strTemp);
	}
}

