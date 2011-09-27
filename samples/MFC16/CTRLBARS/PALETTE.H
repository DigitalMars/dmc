// palette.h : interface of the CPaletteBar class
//
// This is a part of the Microsoft Foundation Classes C++ library.
// Copyright (C) 1992 Microsoft Corporation
// All rights reserved.
//
// This source code is only intended as a supplement to the
// Microsoft Foundation Classes Reference and Microsoft
// WinHelp documentation provided with the library.
// See these sources for detailed information regarding the
// Microsoft Foundation Classes product.

#ifndef __AFXEXT_H__
#include <afxext.h>         // for access to CToolBar
#endif

class CPaletteBar : public CToolBar
{
// Constructor
public:
	CPaletteBar();
	BOOL Create(CWnd* pOwnerWnd, int x, int y);
	void SetSizes(SIZE sizeButton, SIZE sizeImage, UINT nColumns);
	BOOL SetButtons(const UINT FAR* lpIDArray, int nIDCount, UINT nColumns);

// Attributes
public:

// Operations
public:

// Implementation
public:
	virtual ~CPaletteBar();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	// overridden from CToolBar implementation
	virtual void GetItemRect(int nIndex, LPRECT lpRect) const;
	virtual int HitTest(CPoint point);
	virtual void DoPaint(CDC* pDC);

	void InvertTracker(CPoint point);
	void RecalcLayout(UINT nButtonCount);

protected:
	BOOL    m_bTrackMove;
	CPoint  m_ptMouse, m_ptLast;
	UINT    m_nColumns;
	CWnd*   m_pOwnerWnd;
	UINT    m_cxRightBorder;

// Generated message map functions
protected:
	//{{AFX_MSG(CPaletteBar)
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg int OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
