// circ2ctl.h : Declaration of the CCirc2Ctrl OLE control class.
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
// CCirc2Ctrl : See circ2ctl.cpp for implementation.

class CCirc2Ctrl : public COleControl
{
	DECLARE_DYNCREATE(CCirc2Ctrl)

// Constructor
public:
	CCirc2Ctrl();

// Overrides

	// Drawing function
	virtual void OnDraw(
				CDC* pdc, const CRect& rcBounds, const CRect& rcInvalid);

	// Persistence
	virtual void DoPropExchange(CPropExchange* pPX);

	// Reset control state
	virtual void OnResetState();

	virtual BOOL OnMapPropertyToPage(DISPID dispid, LPCLSID lpclsid,
		BOOL* pbPageOptional);

// Implementation
protected:
	~CCirc2Ctrl();
	void GetDrawRect(CRect* rc);
	short m_circleOffset;
	BOOL InBounds(short nOffset);
	BOOL InCircle(CPoint& point);
	void FlashColor(CDC* pdc);

	DECLARE_OLECREATE_EX(CCirc2Ctrl)    // Class factory and guid
	DECLARE_OLETYPELIB(CCirc2Ctrl)      // GetTypeInfo
	DECLARE_PROPPAGEIDS(CCirc2Ctrl)     // Property page IDs
	DECLARE_OLECTLTYPE(CCirc2Ctrl)		// Type name and misc status

// Message maps
	//{{AFX_MSG(CCirc2Ctrl)
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

// Dispatch maps
	//{{AFX_DISPATCH(CCirc2Ctrl)
	OLE_COLOR m_flashColor;
	BOOL m_circleShape;
	afx_msg void OnCircleShapeChanged();
	afx_msg short GetCircleOffset();
	afx_msg void SetCircleOffset(short nNewValue);
	//}}AFX_DISPATCH
	DECLARE_DISPATCH_MAP()

	afx_msg void AboutBox();

// Event maps
	//{{AFX_EVENT(CCirc2Ctrl)
	void FireClickIn(OLE_XPOS_PIXELS x, OLE_YPOS_PIXELS y)
		{FireEvent(eventidClickIn,EVENT_PARAM(VTS_XPOS_PIXELS  VTS_YPOS_PIXELS), x, y);}
	void FireClickOut()
		{FireEvent(eventidClickOut,EVENT_PARAM(VTS_NONE));}
	//}}AFX_EVENT
	DECLARE_EVENT_MAP()

// Dispatch and event IDs
public:
	enum {
	//{{AFX_DISP_ID(CCirc2Ctrl)
	dispidCircleShape = 2L,
	dispidCircleOffset = 3L,
	dispidFlashColor = 1L,
	eventidClickIn = 1L,
	eventidClickOut = 2L,
	//}}AFX_DISP_ID
	};
};
