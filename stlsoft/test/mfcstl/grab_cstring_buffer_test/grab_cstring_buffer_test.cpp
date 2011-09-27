/* /////////////////////////////////////////////////////////////////////////////
 * File:		grab_cstring_buffer_test.cpp
 *
 * Purpose:		Illustrates the MFCSTL grab_cstring_buffer class
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

#include <mfcstl_grab_cstring_buffer.h>

mfcstl_ns_using(grab_cstring_buffer)

#include <afxwin.h>

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
	Create(NULL, "MFCSTL - grab_cstring_buffer_test");
}

void CMyFrame::OnPaint()
{
	CPaintDC dc( this );
	CPoint	origin(1, 1);

	put_to_dc(dc, origin, "Getting the current directory in an atomic action");

	try
	{
		CString	strCurrDir;
		int		i	=	::GetCurrentDirectory(_MAX_PATH + 1, grab_cstring_buffer(strCurrDir, _MAX_PATH + 1));

		put_to_dc(dc, origin, "Current Directory:");
		put_to_dc(dc, origin, strCurrDir);
	}
	catch(CMemoryException *e)
	{
		put_to_dc(dc, origin, "Exception:");

		e->ReportError(MB_ICONEXCLAMATION);
	}
}

////////////////////////////////////////////////////////////////////////////////
