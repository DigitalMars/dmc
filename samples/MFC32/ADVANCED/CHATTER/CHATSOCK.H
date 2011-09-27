// chatsock.h : interface of the CChatSocket class
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

#ifndef __CHATSOCK_H__
#define __CHATSOCK_H__

class CChatDoc;

class CChatSocket : public CSocket
{
	DECLARE_DYNAMIC(CChatSocket);

// Construction
public:
	CChatSocket(CChatDoc* pDoc);

// Operations
public:
	CChatDoc* m_pDoc;

// Implementation
protected:
	virtual void OnReceive(int nErrorCode);
};

#endif // __CHATSOCK_H__
