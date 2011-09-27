/* /////////////////////////////////////////////////////////////////////////////
 * File:		resource_string_test.cpp
 *
 * Purpose:		Illustrates the MFCSTL resource_string class
 *
 * Created:		5th December 2002
 * Updated:		22nd April 2004
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

#include <mfcstl_resource_string.h>

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
public:
	CMyFrame();

protected:
	afx_msg void OnPaint();

	DECLARE_MESSAGE_MAP()
};

////////////////////////////////////////////////////////////////////////////////

CMyApp gMainApp;

////////////////////////////////////////////////////////////////////////////////

void put_to_dc(CDC &dc, POINT &origin, CString const &s)
{
	SIZE	cxy	=	dc.GetTextExtent(s);

	dc.TextOut(origin.x, origin.y, s);

	origin.y += 1 + cxy.cy;
}

////////////////////////////////////////////////////////////////////////////////

BOOL CMyApp::InitInstance()
{
	m_pMainWnd = new CMyFrame();
	m_pMainWnd->ShowWindow(m_nCmdShow);
	m_pMainWnd->UpdateWindow();

	return TRUE;
}

BEGIN_MESSAGE_MAP(CMyFrame, CFrameWnd)
	ON_WM_PAINT()
END_MESSAGE_MAP()

CMyFrame::CMyFrame()
{
	Create(NULL, "MFCSTL - resource_string_test");
}

void CMyFrame::OnPaint()
{
	typedef mfcstl_ns_qual(resource_string) r_str_t;

	CPaintDC dc( this );

	HINSTANCE	hinst		=	::GetModuleHandle(NULL);
	bool		bRunning	=	(::GetTickCount() & 0x02) != 0;
	CPoint		origin(1, 1);
	r_str_t		message		=	r_str_t(hinst, IDS_PREFIX)
							+	r_str_t(bRunning
											? IDS_RUNNING
											: IDS_STOPPED);

	put_to_dc(dc, origin, message);
	put_to_dc(dc, origin, r_str_t(IDS_ANOTHER));
}

////////////////////////////////////////////////////////////////////////////////
