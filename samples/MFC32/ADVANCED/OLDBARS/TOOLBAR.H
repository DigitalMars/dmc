// toolbar.h : definition of old backward compatible CToolBar
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

#ifndef _TOOLBAR_H_
#define _TOOLBAR_H_

struct AFX_TBBUTTON;        // private to implementation

#ifndef _MAC
HBITMAP AFXAPI AfxLoadSysColorBitmap(HINSTANCE hInst, HRSRC hRsrc);
#else
HBITMAP AFXAPI AfxLoadSysColorBitmap(HINSTANCE hInst, HRSRC hRsrc,
	HDC hDCGlyphs, BOOL bMonochrome);
#endif

class COldToolBar : public CControlBar
{
	DECLARE_DYNAMIC(COldToolBar)

// Construction
public:
	COldToolBar();
	BOOL Create(CWnd* pParentWnd,
			DWORD dwStyle = WS_CHILD | WS_VISIBLE | CBRS_TOP,
			UINT nID = AFX_IDW_TOOLBAR);

	void SetSizes(SIZE sizeButton, SIZE sizeImage);
				// button size should be bigger than image
	void SetHeight(int cyHeight);
				// call after SetSizes, height overrides bitmap size
	BOOL LoadBitmap(LPCTSTR lpszResourceName);
	BOOL LoadBitmap(UINT nIDResource);
	BOOL SetButtons(const UINT* lpIDArray, int nIDCount);
				// lpIDArray can be NULL to allocate empty buttons

// Attributes
public: // standard control bar things
	int CommandToIndex(UINT nIDFind) const;
	UINT GetItemID(int nIndex) const;
	virtual void GetItemRect(int nIndex, LPRECT lpRect) const;
	UINT GetButtonStyle(int nIndex) const;
	void SetButtonStyle(int nIndex, UINT nStyle);

public:
	// for changing button info
	void GetButtonInfo(int nIndex, UINT& nID, UINT& nStyle, int& iImage) const;
	void SetButtonInfo(int nIndex, UINT nID, UINT nStyle, int iImage);

// Implementation
public:
	virtual ~COldToolBar();
	virtual CSize CalcFixedLayout(BOOL bStretch, BOOL bHorz);

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	inline AFX_TBBUTTON* _GetButtonPtr(int nIndex) const;
	void InvalidateButton(int nIndex);
	void UpdateButton(int nIndex);
	void CreateMask(int iImage, CPoint offset, 
		BOOL bHilite, BOOL bHiliteShadow);

	// for custom drawing
	struct DrawState
	{
		HBITMAP hbmMono;
		HBITMAP hbmMonoOld;
		HBITMAP hbmOldGlyphs;
	};
	BOOL PrepareDrawButton(DrawState& ds);
	BOOL DrawButton(CDC* pDC, int x, int y, int iImage, UINT nStyle);
#ifdef _MAC
	BOOL DrawMonoButton(CDC* pDC, int x, int y, int dx, int dy,
		int iImage, UINT nStyle);
#endif
	void EndDrawButton(DrawState& ds);

protected:
	CSize m_sizeButton;         // size of button
	CSize m_sizeImage;          // size of glyph
	int m_cxSharedBorder;       // shared x border between buttons
	int m_cySharedBorder;       // shared y border between buttons
	HBITMAP m_hbmImageWell;     // glyphs only
	int m_iButtonCapture;       // index of button with capture (-1 => none)
	HRSRC m_hRsrcImageWell;     // handle to loaded resource for image well
	HINSTANCE m_hInstImageWell; // instance handle to load image well from

#ifdef _MAC
	// Macintosh toolbars need per-toolbar DCs in order to
	// work correctly in multiple-monitor environments

	HDC m_hDCGlyphs;            // per-toolbar DC for glyph images
	HDC m_hDCMono;              // per-toolbar DC for mono glyph masks
#endif
	virtual void DoPaint(CDC* pDC);
	virtual void OnUpdateCmdUI(CFrameWnd* pTarget, BOOL bDisableIfNoHndler);
	virtual int HitTest(CPoint point);
	virtual int OnToolHitTest(CPoint point, TOOLINFO* pTI) const;

	//{{AFX_MSG(COldToolBar)
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnCancelMode();
	afx_msg void OnSysColorChange();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

// define CToolBar to COldToolBar for convenience
#define CToolBar COldToolBar

#endif //!_TOOLBAR_H_
