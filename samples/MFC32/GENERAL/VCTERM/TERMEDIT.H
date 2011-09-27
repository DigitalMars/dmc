// termedit.h : interface of the CTermEdit class
//

// This is a part of the Microsoft Foundation Classes C++ library.
// Copyright (C) 1992-1996 Microsoft Corporation
// All rights reserved.
//
// This source code is only intended as a supplement to the
// Microsoft Foundation Classes Reference and related
// electronic documentation provided with the library.
// See these sources for detailed information regarding the
// Microsoft Foundation Classes product.

#include "commctrl.h"

#ifndef _TERMEDIT_H_
#define _TERMEDIT_H_

/////////////////////////////////////////////////////////////////////////////
// CTermEdit window

class CTermEdit : public CEdit
{
// Construction
public:
	CTermEdit();
	BOOL m_bEcho;
	CCommCtrl* m_pCommCtrl;

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTermEdit)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CTermEdit();

	// Generated message map functions
protected:
	//{{AFX_MSG(CTermEdit)
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

#endif // _TERMEDIT_H_
/////////////////////////////////////////////////////////////////////////////
