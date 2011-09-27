// lstnsock.h : interface of the CListeningSocket class
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

#ifndef __LSTNSOCK_H__
#define __LSTNSOCK_H__

class CServerDoc;

class CListeningSocket : public CSocket
{
	DECLARE_DYNAMIC(CListeningSocket);
private:
	CListeningSocket(const CListeningSocket& rSrc);         // no implementation
	void operator=(const CListeningSocket& rSrc);  // no implementation

// Construction
public:
	CListeningSocket(CServerDoc* pDoc);

// Attributes
public:
	CServerDoc* m_pDoc;

// Overridable callbacks
protected:
	virtual void OnAccept(int nErrorCode);

// Implementation
public:
	virtual ~CListeningSocket();

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
};
#endif // __LSTNSOCK_H__
