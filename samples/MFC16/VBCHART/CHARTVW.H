// chartvw.h : interface of the CChartView class
//
// This is a part of the Microsoft Foundation Classes C++ library.
// Copyright (C) 1992 Microsoft Corporation
// All rights reserved.
//
// This source code is only intended as a supplement to the
// Microsoft Foundation Classes Reference and Microsoft
// QuickHelp and/or WinHelp documentation provided with the library.
// See these sources for detailed information regarding the
// Microsoft Foundation Classes product.



class CVbChartFrame : public CMDIChildWnd
{
	DECLARE_DYNCREATE(CVbChartFrame)

protected:
	CVbChartFrame() {}

	CSplitterWnd    m_Splitter;

	virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);

protected:
	//{{AFX_MSG(CVbChartFrame)
	afx_msg void OnFilePrint();
	afx_msg void OnFilePrintPreview();
	afx_msg void OnUpdateChartList(CCmdUI* pCmdUI);
	//}}AFX_MSG
	void OnChartTypeChange();

	DECLARE_MESSAGE_MAP()
};

class CChartView : public CView
{
protected: // create from serialization only
	CChartView();
	DECLARE_DYNCREATE(CChartView)

// Attributes
public:
	CChartDoc* GetDocument()
		{
			ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CChartDoc)));
			return (CChartDoc*) m_pDocument;
		}


	int* m_pnData;
	int  m_nDataRows;
	int  m_nDataCols;
	int  m_nStartRow;
	int  m_nStartCol;
	int  m_nChartType;


// Operations
public:
	void Print();
	void Preview();

// Implementation
public:
	virtual ~CChartView();
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view

	CRect GetDisplayRect(CDC* pDC);
	void PlotLineChart(CDC* pDC);
	void PlotBarChart(CDC* pDC, BOOL bVert);
	void Plot3DBarChart(CDC* pDC, BOOL bVert);
	void PlotGanttChart(CDC* pDC, BOOL bVert);
	int GetMaxValue();
	void GetDataFromGrid();
	int GetTickValue(int nMaxValue, int nNumDiv);
	void PlotCaptions(CDC* pDC, CPoint ptOrigin, CSize sizePixelsPerTick,
					int nValue, int nValuePerTick, int nNumTicks,
					int nCharHeight, BOOL bVert, BOOL bAlpha);
	void PlotAxes(CDC* pDC, CPoint ptOrigin, CSize sizePixelsPerTick,
					int nHorzTicks, int nVertTicks, int nDrawTicks);
	int GetChartMetrics(CDC* pDC, int nHDiv, int nVDiv, CSize& sizeGraph,
					CSize& sizePixelsPerTick, CPoint& ptOrigin);

	virtual void OnActivateView(BOOL bActivate, CView* pActivateView,
					CView* pDeactiveView);

	// Printing support
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);

// Generated message map functions
protected:
	//{{AFX_MSG(CChartView)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


#define CHART_LINE      0
#define CHART_VBAR      1
#define CHART_HBAR      2
#define CHART_3DVBAR    3
#define CHART_3DHBAR    4
#define CHART_VGANTT    5
#define CHART_HGANTT    6

#define TICKS_NONE  0
#define TICKS_VERT  1
#define TICKS_HORZ  2
#define TICKS_BOTH  3


/////////////////////////////////////////////////////////////////////////////
