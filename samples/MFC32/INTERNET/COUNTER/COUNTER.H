/////////////////////////////////////////////////////////////////////////////
// counter.h : Defines the initialization routines for the DLL.
//

// This is a part of the Microsoft Foundation Classes C++ library.
// Copyright (C) 1992-1996 Microsoft Corporation
// All rights reserved.
//
// This source code is only intended as a supplement to the
// Microsoft Foundation Classes Reference and related
// electronic documentation provided with the library.
// See these sources for detailed information regarding the
// Microsoft Foundation Classes product.

/////////////////////////////////////////////////////////////////////////////

class CCounterExtension : public CHttpServer
{
public:
	CCounterExtension();
	~CCounterExtension();

	BOOL GetExtensionVersion(HSE_VERSION_INFO* pVer);

	void Default(CHttpServerContext* pCtxt);
	void Clock(CHttpServerContext* pCtxt);

	DECLARE_PARSE_MAP()

private:
	CMapStringToString m_Paths;
	int GetPageCount(CString& szPage);
	void OutputXBM(CHttpServerContext* pCtxt, CString& szDigits);
};

