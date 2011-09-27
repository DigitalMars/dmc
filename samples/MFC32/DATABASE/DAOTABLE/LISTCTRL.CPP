// FieldListCtrl.cpp : implementation file
//
// this class is an override of the default clistctrl
// this version provides handlers to allow modifying the
// subitem on repeated selection of the item in the list

// This is a part of the Microsoft Foundation Classes C++ library.
// Copyright (C) 1992-1996 Microsoft Corporation
// All rights reserved.
//
// This source code is only intended as a supplement to the
// Microsoft Foundation Classes Reference and related
// electronic documentation provided with the library.
// See these sources for detailed information regarding the
// Microsoft Foundation Classes product.

#include "stdafx.h"
#include "DAOTable.h"
#include "ListCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFieldListCtrl

CFieldListCtrl::CFieldListCtrl()
{
}

CFieldListCtrl::~CFieldListCtrl()
{
}


BEGIN_MESSAGE_MAP(CFieldListCtrl, CListCtrl)
	//{{AFX_MSG_MAP(CFieldListCtrl)
	ON_WM_LBUTTONDOWN()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFieldListCtrl message handlers

// if you press the button over an item, then toggle the subitem
// text between "", "ascending", and "descending" for index sort order
// specification
void CFieldListCtrl::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// what item did we select
	UINT flags;
	int item = HitTest(point, &flags);

	// if an item is actually selected, then process
	if (item != -1)
	{
		// get the item's current selection state based on subitem text
		CString text = GetItemText(item, 1);

		// if selected, then modify the sub item text
		if (text != "")
		{
			// if already ascending, then descending is next
			if (text == _T("ascending"))
			{
				SetItemText(item, 1, _T("descending"));
				SetItemState(item, 1, LVIS_SELECTED);	
				CListCtrl::OnLButtonDown(nFlags, point);
			}
			else // descending goes to no selection or text
			{
				SetItemText(item, 1, _T(""));
				SetItemState(item, 0, LVIS_SELECTED);
			}
		}
		else // if no selection at all, then 1st state is ascending
		{
			SetItemText(item, 1, _T("ascending"));	
			SetItemState(item, 1, LVIS_SELECTED);	
			CListCtrl::OnLButtonDown(nFlags, point);
		}
	}
}
