// drawwnd.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDrawWnd window

class CDrawWnd : public CWnd
{
// Construction
public:
	CDrawWnd(BOOL bAutoDelete = TRUE);

// Attributes
public:
	CRgn m_rgnLast;
	int m_nWidth;
	int m_nSteps;
	int m_nPos;
	int m_nStep;
	int m_nScale;
	int m_nHeight;
	int m_nStyle;
	LOGBRUSH m_logbrush;
	LOGBRUSH m_logbrushBlack;
	static LPCTSTR m_lpszClassName;
	void SetSpeed(int nSpeed);
	void SetResolution(int nRes);
	void SetColor(COLORREF cr);
	void SetPenWidth(int nWidth);
	void SetLineStyle(int nStyle);

// Operations
public:
	void DrawLetter(CDC& dc, CPoint pt, int p1[][3], int p2[][3], int nLen);
	void Draw(CDC& dc, int nWidth);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDrawWnd)
	public:
	virtual BOOL Create(DWORD dwExStyle, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext = NULL);
	protected:
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CDrawWnd();

protected:
	BOOL m_bAutoDelete;

	// Generated message map functions
protected:
	//{{AFX_MSG(CDrawWnd)
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
