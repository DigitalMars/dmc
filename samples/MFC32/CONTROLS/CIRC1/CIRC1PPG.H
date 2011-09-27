// circ1ppg.h : Declaration of the CCirc1PropPage property page class.
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
// CCirc1PropPage : See circ1ppg.cpp for implementation.

class CCirc1PropPage : public COlePropertyPage
{
	DECLARE_DYNCREATE(CCirc1PropPage)
	DECLARE_OLECREATE_EX(CCirc1PropPage)

// Constructor
public:
	CCirc1PropPage();

// Dialog Data
	//{{AFX_DATA(CCirc1PropPage)
	enum { IDD = IDD_PROPPAGE_CIRC1 };
		// NOTE - ClassWizard will add data members here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_DATA

// Implementation
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Message maps
protected:
	//{{AFX_MSG(CCirc1PropPage)
		// NOTE - ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};
