// mfcucase.h : defines the filter class
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

class CUpCaseFilter : public CHttpFilter
{
public:
	CUpCaseFilter();
	~CUpCaseFilter();

public:
	virtual BOOL GetFilterVersion(PHTTP_FILTER_VERSION pVer);
	
	virtual DWORD OnSendRawData(CHttpFilterContext* pCtxt,
		PHTTP_FILTER_RAW_DATA pRawData);
	virtual DWORD OnUrlMap(CHttpFilterContext* pCtxt,
		PHTTP_FILTER_URL_MAP pUrlMap);
};

