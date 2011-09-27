// msg.h : interface of the CMsg class
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

class CMsg : public CObject
{
protected:
	DECLARE_DYNCREATE(CMsg)
public:
	CMsg();

// Attributes
public:
	CString m_strText;
	BOOL m_bClose;
	CStringList m_msgList;

// Operations
public:
	void Init();

// Implementation
public:
	virtual ~CMsg();
	virtual void Serialize(CArchive& ar);   // overridden for document i/o
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
};

/////////////////////////////////////////////////////////////////////////////
