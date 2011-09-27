// SmileCtl.h : Declaration of the CSmileCtrl OLE control class.

/////////////////////////////////////////////////////////////////////////////
// CSmileCtrl : See SmileCtl.cpp for implementation.

class CSmileCtrl : public COleControl
{
	DECLARE_DYNCREATE(CSmileCtrl)

// Constructor
public:
	CSmileCtrl();


// Attributes
protected:
	BOOL	m_bWink;		// Determines if Eye should 'blink'

// Overrides
public:
	// Drawing function
	virtual void OnDraw(
				CDC* pdc, const CRect& rcBounds, const CRect& rcInvalid);

	// Persistence
	virtual void DoPropExchange(CPropExchange* pPX);

	// Reset control state
	virtual void OnResetState();

// Implementation
protected:
	~CSmileCtrl();

	// Hit Testing Helper
	BOOL InEllipse(const CPoint& pt, int x1, int y1, int x2, int y2);


	DECLARE_OLECREATE_EX(CSmileCtrl)    // Class factory and guid
	DECLARE_OLETYPELIB(CSmileCtrl)      // GetTypeInfo
	DECLARE_PROPPAGEIDS(CSmileCtrl)     // Property page IDs
	DECLARE_OLECTLTYPE(CSmileCtrl)		// Type name and misc status

// Message maps
	//{{AFX_MSG(CSmileCtrl)
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

// Dispatch maps
	//{{AFX_DISPATCH(CSmileCtrl)
	BOOL m_bSad;
	afx_msg void OnSadChanged();
	afx_msg void Beep();
	afx_msg void Wink(BOOL bWink);
	//}}AFX_DISPATCH
	DECLARE_DISPATCH_MAP()

	afx_msg void AboutBox();

// Event maps
	//{{AFX_EVENT(CSmileCtrl)
	void FireOutside()
		{FireEvent(eventidOutside,EVENT_PARAM(VTS_NONE));}
	void FireInside(OLE_XPOS_PIXELS x, OLE_YPOS_PIXELS y)
		{FireEvent(eventidInside,EVENT_PARAM(VTS_XPOS_PIXELS  VTS_YPOS_PIXELS), x, y);}
	//}}AFX_EVENT
	DECLARE_EVENT_MAP()

// Dispatch and event IDs
public:
	enum {
	//{{AFX_DISP_ID(CSmileCtrl)
	dispidSad = 1L,
	dispidBeep = 2L,
	dispidWink = 3L,
	eventidOutside = 1L,
	eventidInside = 2L,
	//}}AFX_DISP_ID
	};
};
