// circ1ctl.h : Declaration of the CCirc1Ctrl OLE control class.
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

/////////////////////////////////////////////////////////////////////////////
// CCirc1Ctrl : See circ1ctl.cpp for implementation.

class CCirc1Ctrl : public COleControl
{
	DECLARE_DYNCREATE(CCirc1Ctrl)

// Constructor
public:
	CCirc1Ctrl();

// Overrides

	// Drawing function
	virtual void OnDraw(
				CDC* pdc, const CRect& rcBounds, const CRect& rcInvalid);

	// Persistence
	virtual void DoPropExchange(CPropExchange* pPX);

	// Reset control state
	virtual void OnResetState();

// Implementation
protected:
	~CCirc1Ctrl();

	DECLARE_OLECREATE_EX(CCirc1Ctrl)    // Class factory and guid
	DECLARE_OLETYPELIB(CCirc1Ctrl)      // GetTypeInfo
	DECLARE_PROPPAGEIDS(CCirc1Ctrl)     // Property page IDs
	DECLARE_OLECTLTYPE(CCirc1Ctrl)		// Type name and misc status

// Message maps
	//{{AFX_MSG(CCirc1Ctrl)
		// NOTE - ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

// Dispatch maps
	//{{AFX_DISPATCH(CCirc1Ctrl)
		// NOTE - ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_DISPATCH
	DECLARE_DISPATCH_MAP()

	afx_msg void AboutBox();

// Event maps
	//{{AFX_EVENT(CCirc1Ctrl)
		// NOTE - ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_EVENT
	DECLARE_EVENT_MAP()

// Dispatch and event IDs
public:
	enum {
	//{{AFX_DISP_ID(CCirc1Ctrl)
		// NOTE: ClassWizard will add and remove enumeration elements here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_DISP_ID
	};
};
