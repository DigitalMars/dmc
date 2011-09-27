/* /////////////////////////////////////////////////////////////////////////////
 * File:		list_adaptor_test.cpp
 *
 * Purpose:		Illustrates the MFCSTL list_adaptor class
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

#include <afxtempl.h>

#include <stlsoft_simple_algorithms.h>

stlsoft_ns_using(for_each_postinc)

#include <mfcstl_list_adaptor.h>

mfcstl_ns_using(list_adaptor)

#include <stlsoft_string_access.h>

#include <mfcstl_string_access.h>

stlsoft_ns_using(c_str_ptr)
stlsoft_ns_using(c_str_len)

#include <stlsoft_type_helpers.h>

stlsoft_ns_using(make_value_type)

#include <algorithm>

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

template <ss_typename_param_k S>
void put_to_dc(CDC &dc, POINT &origin, S const &s)
{
	int		cch	=	c_str_len(s);
	SIZE	cxy	=	dc.GetTextExtent(c_str_ptr(s), cch);

	dc.TextOut(origin.x, origin.y, c_str_ptr(s));

	origin.y += 1 + cxy.cy;
}

////////////////////////////////////////////////////////////////////////////////

struct dump_to_dc
{
public:
	dump_to_dc(CDC &dc, POINT &origin)
		: m_dc(dc)
		, m_origin(origin)
	{}

	void operator ()(CString const &s)
	{
		put_to_dc(m_dc, m_origin, s);
	}

	void operator ()(char const *s)
	{
		put_to_dc(m_dc, m_origin, s);
	}

	void operator ()(CObject const *pobj)
	{
		CString	str;

		str.Format("0x%08x, %s", pobj, pobj->GetRuntimeClass()->m_lpszClassName);

		put_to_dc(m_dc, m_origin, str);
	}

	void operator ()(void const *pv)
	{
		CString	str;

		str.Format("0x%08x", pv);

		put_to_dc(m_dc, m_origin, str);
	}

// members
protected:
	CDC		&m_dc;
	POINT	&m_origin;
};

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
	Create(NULL, "MFCSTL - list_adaptor_test");
}

void CMyFrame::OnPaint()
{
	typedef	CList<CString, CString>									CStringListT;

#ifdef __STLSOFT_CF_TEMPLATE_PARTIAL_SPECIALISATION_SUPPORT
	typedef	list_adaptor<CObList>									objlist_t;
	typedef	list_adaptor<CPtrList>									ptrlist_t;
	typedef	list_adaptor<CStringList>								strlist_t;
	typedef	list_adaptor<CStringListT>								strlistt_t;
#else
	typedef	list_adaptor<CObList, make_value_type<CObject*> >		objlist_t;
	typedef	list_adaptor<CPtrList, make_value_type<void*> >			ptrlist_t;
	typedef	list_adaptor<CStringList, make_value_type<CString> >	strlist_t;
	typedef	list_adaptor<CStringListT, make_value_type<CString> >	strlistt_t;
#endif /* __STLSOFT_CF_TEMPLATE_PARTIAL_SPECIALISATION_SUPPORT */

	CPaintDC	dc( this );
	CPoint		origin(1, 1);
	dump_to_dc	d2dc(dc, origin);

	// Object
	{
		d2dc("enumerating (via std::for_each) list_adaptor<CObjectList>:");
		d2dc("");

		CObList			objlist;
		objlist_t		c(objlist);

		objlist.AddTail(AfxGetApp());
		objlist.AddTail(AfxGetApp()->m_pMainWnd);
		objlist.AddTail(&dc);
		objlist.AddTail(this);

		mfcstl_ns_qual_std(for_each)(c.begin(), c.end(), d2dc);
	}

	// Ptr
	{
		d2dc("enumerating (via std::for_each) list_adaptor<CPtrList>:");
		d2dc("");

		CPtrList		ptrlist;
		ptrlist_t		c(ptrlist);

		ptrlist.AddTail(&d2dc);
		ptrlist.AddTail(AfxGetApp());
		ptrlist.AddTail(static_cast<void*>(0));
		ptrlist.AddTail((void*)"a literal");

		mfcstl_ns_qual_std(for_each)(c.begin(), c.end(), d2dc);
	}

	// String
	{
		d2dc("enumerating (via std::for_each) list_adaptor<CStringList>:");
		d2dc("");

		CStringList		strlist;
		strlist_t		c(strlist);

		strlist.AddTail("String #1");
		strlist.AddTail("String #2");
		strlist.AddTail("String #3");
		strlist.AddTail("String #4");

		mfcstl_ns_qual_std(for_each)(c.begin(), c.end(), d2dc);
	}

	// String
	{
		d2dc("enumerating (via std::for_each) list_adaptor<CList<CString, CString> >:");
		d2dc("");

		CStringListT	strlist;
		strlistt_t		c(strlist);

		strlist.AddTail("String #1");
		strlist.AddTail("String #2");
		strlist.AddTail("String #3");
		strlist.AddTail("String #4");

		mfcstl_ns_qual_std(for_each)(c.begin(), c.end(), d2dc);
	}

	// String (post-inc)
	{
		d2dc("enumerating (via std::for_each) list_adaptor<CStringList> (post-increment):");
		d2dc("");

		CStringList		strlist;
		strlist_t		c(strlist);

		strlist.AddTail("String #1");
		strlist.AddTail("String #2");
		strlist.AddTail("String #3");
		strlist.AddTail("String #4");

		stlsoft_ns_qual(for_each_postinc)(c.begin(), c.end(), d2dc);
	}
}

////////////////////////////////////////////////////////////////////////////////
