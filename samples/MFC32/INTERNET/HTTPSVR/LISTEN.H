// Listen.h : interface of the CListenSocket class
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

class CHttpSvrDoc;

class CListenSocket : public CAsyncSocket
{
	DECLARE_DYNCREATE(CListenSocket)
	CListenSocket( void );
public:
	CListenSocket( CHttpSvrDoc* pDoc );
protected:
	CHttpSvrDoc* m_pDoc;
	virtual void OnAccept( int nErrorCode );
};


