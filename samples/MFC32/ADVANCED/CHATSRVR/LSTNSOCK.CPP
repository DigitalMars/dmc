// lstnsock.cpp : implementation of the CListeningSocket class
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
#include "lstnsock.h"
#include "srvrdoc.h"

CListeningSocket::CListeningSocket(CServerDoc* pDoc)
{
	m_pDoc = pDoc;
}

/////////////////////////////////////////////////////////////////////////////
// CListeningSocket Overridable callbacks

void CListeningSocket::OnAccept(int nErrorCode)
{
	CSocket::OnAccept(nErrorCode);
	m_pDoc->ProcessPendingAccept();
}

/////////////////////////////////////////////////////////////////////////////
// CSocket Implementation

CListeningSocket::~CListeningSocket()
{
}

#ifdef _DEBUG
void CListeningSocket::AssertValid() const
{
	CSocket::AssertValid();
}

void CListeningSocket::Dump(CDumpContext& dc) const
{
	CSocket::Dump(dc);
}
#endif //_DEBUG

IMPLEMENT_DYNAMIC(CListeningSocket, CSocket)
