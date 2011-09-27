// circ3ppg.h : Declaration of the CCirc3PropPage property page class.
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

////////////////////////////////////////////////////////////////////////////
// CCirc3PropPage : See circ3ppg.cpp for implementation.

class CCirc3PropPage : public COlePropertyPage
{
	DECLARE_DYNCREATE(CCirc3PropPage)
	DECLARE_OLECREATE_EX(CCirc3PropPage)

// Constructor
public:
	CCirc3PropPage();

// Dialog Data
	//{{AFX_DATA(CCirc3PropPage)
	enum { IDD = IDD_PROPPAGE_CIRC3 };
	CString m_caption;
	int     m_circleOffset;
	BOOL    m_circleShape;
	CString m_note;
	//}}AFX_DATA

// Implementation
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Message maps
protected:
	//{{AFX_MSG(CCirc3PropPage)
		// NOTE - ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};
