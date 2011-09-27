/* /////////////////////////////////////////////////////////////////////////////
 * File:		string_access_test.cpp
 *
 * Purpose:		Illustrates the string access components of the MFCSTL libraries
 *
 * Created:		5th December 2002
 * Updated:		12th March 2004
 *
 * Copyright:	Synesis Software Pty Ltd, (c) 2002. All rights reserved.
 *
 * Home:		http://www.winstl.org/
 *
 * ////////////////////////////////////////////////////////////////////////// */

// This will cause various compile-time messages to be emitted. When you get
// sick of them just comment out or remove this #define
#define	_STLSOFT_COMPILE_VERBOSE

#include <mfcstl.h>

#include <stlsoft_string_access.h>

#include <mfcstl_string_access.h>

stlsoft_ns_using(c_str_ptr)
stlsoft_ns_using(c_str_len)

#include "resource.h"

////////////////////////////////////////////////////////////////////////////////
// Classes

// class CMyApp
class CMyApp
	: public CWinApp
{
public:
	virtual BOOL InitInstance();
};

// class CMyFrame
class CMyFrame
	: public CFrameWnd
{
	typedef CFrameWnd	parent_class_type;
public:
	CMyFrame();

	CComboBox	m_cbox;
	CListBox	m_lbox;
	CListCtrl	m_list;

protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpcs);
	afx_msg void OnPaint();

	DECLARE_MESSAGE_MAP()
};

////////////////////////////////////////////////////////////////////////////////

template <ss_typename_param_k S>
void put_to_dc(CDC &dc, POINT &origin, S const &s)
{
	int		cch	=	c_str_len(s);
	SIZE	cxy	=	dc.GetTextExtent(c_str_ptr(s), cch);

	dc.TextOut(origin.x, origin.y, c_str_ptr(s), cch);

	origin.y += 1 + cxy.cy;
}

////////////////////////////////////////////////////////////////////////////////

CMyApp gMainApp;

////////////////////////////////////////////////////////////////////////////////

BOOL CMyApp::InitInstance()
{
	m_pMainWnd = new CMyFrame();
	m_pMainWnd->ShowWindow(m_nCmdShow);
	m_pMainWnd->UpdateWindow();

	return TRUE;
}

BEGIN_MESSAGE_MAP(CMyFrame, CFrameWnd)
	ON_WM_CREATE()
	ON_WM_PAINT()
END_MESSAGE_MAP()

CMyFrame::CMyFrame()
{
	Create(NULL, "MFCSTL - string_access_test");
}

int CMyFrame::OnCreate(LPCREATESTRUCT lpcs)
{
	if(	!m_cbox.Create(WS_VISIBLE | WS_CHILD | CBS_DROPDOWN/* CBS_DROPDOWNLIST */, CRect(30, 30, 130, 330), this, 1968) ||
		!m_lbox.Create(WS_VISIBLE | WS_CHILD | 0, CRect(130, 30, 230, 330), this, 1024) ||
		!m_list.Create(WS_VISIBLE | WS_CHILD | LVS_LIST, CRect(230, 30, 330, 330), this, 2000))
	{
		return -1;
	}

	m_lbox.AddString("String 1");
	m_lbox.AddString("String 2");
	m_lbox.AddString("String 3");
	m_lbox.AddString("String 4");

	m_cbox.AddString("String 1");
	m_cbox.AddString("String 2");
	m_cbox.AddString("String 3");
	m_cbox.AddString("String 4");

	m_list.InsertItem(0, "String 1");
	m_list.InsertItem(1, "String 2");
	m_list.InsertItem(2, "String 3");
	m_list.InsertItem(3, "String 4");

	return parent_class_type::OnCreate(lpcs);
}

void CMyFrame::OnPaint()
{
	CPaintDC dc( this );
	CPoint	origin(1, 1);

	// Here the usefulness of the string access shims is demonstrated
	put_to_dc(dc, origin, "A C-style literal string");

	put_to_dc(dc, origin, CString("A CString, made from a literal string"));

	CString	strText;
	strText.LoadString(IDS_RESOURCE);
	put_to_dc(dc, origin, strText);

//	put_to_dc(dc, origin, m_lbox);
//	lbox.SetCurSel(2);
	put_to_dc(dc, origin, m_lbox);
	put_to_dc(dc, origin, m_cbox);
	put_to_dc(dc, origin, m_list);
}

////////////////////////////////////////////////////////////////////////////////
