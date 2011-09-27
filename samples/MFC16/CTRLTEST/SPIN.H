// spin.h: C++ interface to spin control
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

#include "muscroll.h"       // message based API

/////////////////////////////////////////////////////////////////////////////

class CSpinControl : public CWnd
{
	DECLARE_DYNAMIC(CSpinControl)

// Constructors
public:
	CSpinControl();
	BOOL Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID);

// Attributes
	CWnd* GetAssociate();
	void SetAssociate(CWnd* pNew);
	void GetRange(UINT& iMin, UINT& iMax);
	void SetRange(UINT iMin, UINT iMax);
	UINT GetCurrentPos();
	void SetCurrentPos(UINT iPos);
	// there are more APIs in 'muscroll.h' not wrapped here

// Implementation
protected:
	virtual WNDPROC* GetSuperWndProcAddr();
};

/////////////////////////////////////////////////////////////////////////////
// inlines
inline CSpinControl::CSpinControl()
	{ }

inline CWnd* CSpinControl::GetAssociate()
	{ return CWnd::FromHandle((HWND)SendMessage(MSM_HWNDASSOCIATEGET)); }
inline void CSpinControl::SetAssociate(CWnd* pNew)
	{ SendMessage(MSM_HWNDASSOCIATESET, (UINT)pNew->GetSafeHwnd()); }
inline void CSpinControl::GetRange(UINT& iMin, UINT& iMax)
	{ DWORD dw = SendMessage(MSM_DWRANGEGET);
		iMin = LOWORD(dw); iMax = HIWORD(dw);
	}
inline void CSpinControl::SetRange(UINT iMin, UINT iMax)
	{ SendMessage(MSM_DWRANGESET, 0, MAKELONG(iMin, iMax)); }
inline UINT CSpinControl::GetCurrentPos()
	{ return (UINT)SendMessage(MSM_WCURRENTPOSGET); }
inline void CSpinControl::SetCurrentPos(UINT iPos)
	{ SendMessage(MSM_WCURRENTPOSSET, iPos); }

/////////////////////////////////////////////////////////////////////////////
