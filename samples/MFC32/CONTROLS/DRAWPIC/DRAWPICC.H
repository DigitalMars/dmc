// DrawPicC.h : Declaration of the CDrawPicCtrl OLE control class.

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
// CDrawPicCtrl : See DrawPicC.cpp for implementation.

class CDrawPicCtrl : public COleControl
{
	DECLARE_DYNCREATE(CDrawPicCtrl)

// Constructor
public:
	CDrawPicCtrl();

// Overrides

	// Drawing function
	virtual void OnDraw(CDC* pdc, const CRect& rcBounds, 
		const CRect& rcInvalid);

	// Persistence
	virtual void DoPropExchange(CPropExchange* pPX);

	// Reset control state
	virtual void OnResetState();

// Implementation
protected:
	~CDrawPicCtrl();

	DECLARE_OLECREATE_EX(CDrawPicCtrl)    // Class factory and guid
	DECLARE_OLETYPELIB(CDrawPicCtrl)      // GetTypeInfo
	DECLARE_PROPPAGEIDS(CDrawPicCtrl)     // Property page IDs
	DECLARE_OLECTLTYPE(CDrawPicCtrl)		// Type name and misc status

    CImageList          m_ImageList;  
    CSize               m_szImageSize;

// Message maps
	//{{AFX_MSG(CDrawPicCtrl)
		// NOTE - ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

// Dispatch maps
	//{{AFX_DISPATCH(CDrawPicCtrl)
	afx_msg BOOL AddPicture(LPPICTUREDISP NewPic);
	afx_msg void ClearImageList();
	//}}AFX_DISPATCH
	DECLARE_DISPATCH_MAP()

	afx_msg void AboutBox();

// Event maps
	//{{AFX_EVENT(CDrawPicCtrl)
	//}}AFX_EVENT
	DECLARE_EVENT_MAP()

// Dispatch and event IDs
public:
	enum {
	//{{AFX_DISP_ID(CDrawPicCtrl)
	dispidAddPicture = 1L,
	dispidClearImageList = 2L,
	//}}AFX_DISP_ID
	};
};
