// firewnd.h : header file
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
// CFireWnd window

class CFireWnd : public CStatic
{
// Construction
public:
	CFireWnd();

// Attributes
public:

	// Fire Attributes
	int m_nDecay;
	int m_nFlammability;
	int m_nMaxHeat;
	int m_nSpreadRate;
	int m_nSize;
	int m_nSmoothness;
	int m_nDistribution;
	int m_nChaos;

protected:

	int m_MaxBurn;
	BYTE* m_Fire;
	static unsigned long m_RandSeed;
		// Seed used by faster Rand().

	// Device Context Attributes
	CDC* m_pMemDC;
	CWindowDC* m_pWinDC;

	// Palette Attributes
	RGBQUAD m_rgbPalette[256];
	CPalette m_Palette;
	CPalette* m_pOldPalette;

	// Bitmap Attributes
	CBitmap m_Bitmap;
	CBitmap* m_pOldBitmap;
	BYTE* m_pBits;

// Operations
public:
	enum { red = 1, green = 2, blue = 3 };
	void InitFire(int nColor);
	CPalette* GetPalette();
	void RenderFlame();
	void PaintFlame(CDC* pDC = NULL);
	void SetMaxBurn(int nMax);
	int GetMaxBurn();
	CSize GetBitmapSize();

protected:
	void CreatePalette(int nColor);
	void CreateBitmap();
	void BurnPoint(BYTE* pRow, BYTE* pNextRow);

	// This function replaces the crt lib rand() function.
	// The CRT lib function is very slow.  Since rand() is
	// one of the most frequently called functions it was
	// necessary to optimize it.  This function may be
	// inlined and is computationally simple.
	unsigned long Rand();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFireWnd)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CFireWnd();

	// Generated message map functions
protected:
	//{{AFX_MSG(CFireWnd)
	afx_msg void OnPaint();
	afx_msg void OnDestroy();
	afx_msg void OnPaletteChanged(CWnd* pFocusWnd);
	afx_msg BOOL OnQueryNewPalette();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

inline unsigned long CFireWnd::Rand()
{
	// Using the current seed, generate a new random value
	// and seed and return it.  The random value is shifted
	// to reduce some of the noise and produce a more
	// realistic flame.
	return (m_RandSeed = 1664525L * m_RandSeed + 1013904223L) >> 5;
}

inline void CFireWnd::BurnPoint(BYTE* pRow, BYTE* pNextRow)
{
	BYTE* pTarget;

	int off = Rand() % (m_nDistribution + 1);

	int val = m_nDecay + 1;
	val = Rand() % val;
	val = *pNextRow - val;

	if (Rand() & 1)
		pTarget = pRow + off;
	else
		pTarget = pRow - off;

	if (val > 16)
		*pTarget = (BYTE)val;
	else
		*pTarget = 16;
}

/////////////////////////////////////////////////////////////////////////////
