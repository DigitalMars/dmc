// circ3ctl.h : Declaration of the CCirc3Ctrl OLE control class.
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
// CCirc3Ctrl : See circ3ctl.cpp for implementation.

class CCirc3Ctrl : public COleControl
{
	DECLARE_DYNCREATE(CCirc3Ctrl)

// Constructor
public:
	CCirc3Ctrl();

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
	~CCirc3Ctrl();
	void GetDrawRect(CRect* rc);
	short m_circleOffset;
	BOOL InBounds(short nOffset);
	BOOL InCircle(CPoint& point);
	void FlashColor(CDC* pdc);
	CString m_note;

	DECLARE_OLECREATE_EX(CCirc3Ctrl)    // Class factory and guid
	DECLARE_OLETYPELIB(CCirc3Ctrl)      // GetTypeInfo
	DECLARE_PROPPAGEIDS(CCirc3Ctrl)     // Property page IDs
	DECLARE_OLECTLTYPE(CCirc3Ctrl)		// Type name and misc status

// Message maps
	//{{AFX_MSG(CCirc3Ctrl)
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

// Dispatch maps
	//{{AFX_DISPATCH(CCirc3Ctrl)
	OLE_COLOR m_flashColor;
	BOOL m_circleShape;
	afx_msg void OnCircleShapeChanged();
	afx_msg short GetCircleOffset();
	afx_msg void SetCircleOffset(short nNewValue);
	afx_msg BSTR GetNote();
	afx_msg void SetNote(LPCTSTR lpszNewValue);
	//}}AFX_DISPATCH
	DECLARE_DISPATCH_MAP()

	afx_msg void AboutBox();

// Event maps
	//{{AFX_EVENT(CCirc3Ctrl)
	void FireClickIn(OLE_XPOS_PIXELS x, OLE_YPOS_PIXELS y)
		{FireEvent(eventidClickIn,EVENT_PARAM(VTS_XPOS_PIXELS  VTS_YPOS_PIXELS), x, y);}
	void FireClickOut()
		{FireEvent(eventidClickOut,EVENT_PARAM(VTS_NONE));}
	//}}AFX_EVENT
	DECLARE_EVENT_MAP()

// Dispatch and event IDs
public:
	enum {
	//{{AFX_DISP_ID(CCirc3Ctrl)
	dispidCircleShape = 2L,
	dispidCircleOffset = 3L,
	dispidFlashColor = 1L,
	dispidNote = 4L,
	eventidClickIn = 1L,
	eventidClickOut = 2L,
	//}}AFX_DISP_ID
	};
};
