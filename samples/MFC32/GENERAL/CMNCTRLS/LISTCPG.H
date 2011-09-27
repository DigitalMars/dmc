// ListCtrlPage.h : header file
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
// CListCtrlPage dialog
#ifndef INC_LISTCTRLPAGE_H
#define INC_LISTCTRLPAGE_H

#include "mlistctl.h"
#include "notifwdw.h"

class CListCtrlPage : public CPropertyPage
{
	DECLARE_DYNCREATE(CListCtrlPage)

// Construction
public:
	CListCtrlPage();
	~CListCtrlPage();

// Dialog Data
	//{{AFX_DATA(CListCtrlPage)
	enum { IDD = IDD_LISTCTRL };
	CMyListCtrl	m_listctrl;
	CNotifyShowCase m_notifshowcase;
	BOOL	m_bAutoArrange;
	BOOL	m_bCallBacks;
	BOOL	m_bEditLabels;
	BOOL	m_bNoColumnHeader;
	BOOL	m_bNoLabelWrap;
	BOOL	m_bNoSortHeader;
	BOOL	m_bSingleSel;
	CString	m_cstrViewMode;
	CString	m_cstrAlignMode;
	CString	m_cstrSort;
	CString	m_cstrStatus;
	//}}AFX_DATA


	CImageList	*m_pimagelist;
	CImageList	*m_pimagelistSmall;
	CPoint		m_ptHotSpot;


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CListCtrlPage)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog(void);
	//}}AFX_VIRTUAL

// Implementation
protected:

	void ChangeListCtrlStyle(long lStyle, BOOL bSetBit);
	void FillListCtrl(void);
	void RenewListCtrl(long lStyle, BOOL bSetBit);
	void OnButtonUp(CPoint point);
	void OnButtonDown(CPoint point);

public:

	// Generated message map functions
	//{{AFX_MSG(CListCtrlPage)
	afx_msg void OnAutoArrange();
	afx_msg void OnSingleSel();
	afx_msg void OnNoLabelWrap();
	afx_msg void OnEditLabels();
	afx_msg void OnNoColumnHeader();
	afx_msg void OnNoSortHeader();
	afx_msg void OnChangeViewMode();
	afx_msg void OnChangeAlignMode();
	afx_msg void OnChangeSortMode();
    afx_msg void ShowNotification(LPNMHDR pnmhdr, LRESULT *plResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
//	afx_msg void OnBeginDrag(LPNMHDR pnmhdr, LRESULT *pResult);
};


#endif
