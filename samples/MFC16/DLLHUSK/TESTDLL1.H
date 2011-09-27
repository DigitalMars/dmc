// TESTDLL1.H Public API exported from 'TESTDLL1.DLL'

// This is a part of the Microsoft Foundation Classes C++ library.
// Copyright (C) 1992 Microsoft Corporation
// All rights reserved.
//
// This source code is only intended as a supplement to the
// Microsoft Foundation Classes Reference and Microsoft
// QuickHelp and/or WinHelp documentation provided with the library.
// See these sources for detailed information regarding the
// Microsoft Foundation Classes product.


// Initialize the DLL, register the classes etc
extern "C" extern void WINAPI InitTestDLL1();

#undef AFXAPP_DATA
#define AFXAPP_DATA AFXAPI_DATA

/////////////////////////////////////////////////////////////////////////////
// Simple Text document

class CTextDoc : public CDocument
{
	DECLARE_DYNCREATE(CTextDoc)
protected:
	void Serialize(CArchive& ar);
	//{{AFX_MSG(CTextDoc)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
// Simple output view

class CHelloView : public CView
{
	DECLARE_DYNCREATE(CHelloView)
public:
	CHelloView();

// Implementation
protected:
	UINT m_nIDColor;        // last color command
	COLORREF m_clrText;

	virtual void OnDraw(CDC* pDC);
	// message handlers
	//{{AFX_MSG(CHelloView)
	afx_msg void OnColor();
	afx_msg void OnCustomColor();
	afx_msg void OnUpdateColor(CCmdUI* pCmdUI);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
// Simple do-nothing document

class CDummyDoc : public CDocument
{
	DECLARE_DYNCREATE(CDummyDoc)
protected:
	//{{AFX_MSG(CDummyDoc)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#undef AFXAPP_DATA
#define AFXAPP_DATA NEAR

/////////////////////////////////////////////////////////////////////////////
