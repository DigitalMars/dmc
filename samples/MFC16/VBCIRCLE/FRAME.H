// frame.h : interface of the CVBCircleFrame class
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




class CVBCircleFrame : public CFrameWnd
{
// Construction
public:
	CVBCircleFrame();

// Attributes
protected:
	CVBControl m_circle;

// Implementation
public:
	virtual ~CVBCircleFrame();

// Generated message map functions
protected:
	//{{AFX_MSG(CVBCircleFrame)
	afx_msg void OnEditCircle();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnPaint();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
