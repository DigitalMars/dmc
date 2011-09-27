// DrawPicP.h : Declaration of the CDrawPicPropPage property page class.

// This is a part of the Microsoft Foundation Classes C++ library.
// Copyright (C) 1992-1996 Microsoft Corporation
// All rights reserved.
//
// This source code is only intended as a supplement to the
// Microsoft Foundation Classes Reference and related
// electronic documentation provided with the library.
// See these sources for detailed information regarding the
// Microsoft Foundation Classes product.

////////////////////////////////////////////////////////////////////////////
// CDrawPicPropPage : See DrawPicP.cpp for implementation.

class CDrawPicPropPage : public COlePropertyPage
{
	DECLARE_DYNCREATE(CDrawPicPropPage)
	DECLARE_OLECREATE_EX(CDrawPicPropPage)

// Constructor
public:
	CDrawPicPropPage();

// Dialog Data
	//{{AFX_DATA(CDrawPicPropPage)
	enum { IDD = IDD_PROPPAGE_DRAWPIC };
		// NOTE - ClassWizard will add data members here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_DATA

// Implementation
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Message maps
protected:
	//{{AFX_MSG(CDrawPicPropPage)
		// NOTE - ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};
