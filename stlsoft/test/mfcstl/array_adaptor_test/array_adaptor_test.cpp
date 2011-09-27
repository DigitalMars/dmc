/* /////////////////////////////////////////////////////////////////////////////
 * File:		array_adaptor_test.cpp
 *
 * Purpose:		Illustrates the MFCSTL array_adaptor class
 *
 * Created:		5th December 2002
 * Updated:		30th May 2004
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

#include <mfcstl_array_adaptor.h>

mfcstl_ns_using(array_adaptor)

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
	Create(NULL, "MFCSTL - array_adaptor_test");
}

void CMyFrame::OnPaint()
{
	typedef	CArray<CString, CString>								CStringArrayT;

#ifdef __STLSOFT_CF_TEMPLATE_PARTIAL_SPECIALISATION_SUPPORT
	typedef	array_adaptor<CObArray>									objarray_t;
	typedef	array_adaptor<CPtrArray>								ptrarray_t;
	typedef	array_adaptor<CStringArray>								strarray_t;
	typedef	array_adaptor<CStringArrayT>							strlistt_t;
#else
	typedef	array_adaptor<CObArray, make_value_type<CObject*> >		objarray_t;
	typedef	array_adaptor<CPtrArray, make_value_type<void*> >		ptrarray_t;
	typedef	array_adaptor<CStringArray, make_value_type<CString> >	strarray_t;
	typedef	array_adaptor<CStringArrayT, make_value_type<CString> >	strlistt_t;
#endif /* __STLSOFT_CF_TEMPLATE_PARTIAL_SPECIALISATION_SUPPORT */

	CPaintDC	dc( this );
	CPoint		origin(1, 1);
	dump_to_dc	d2dc(dc, origin);

	// Object
	{
		CObArray		objlist;
		objarray_t		c(objlist);

		objlist.Add(AfxGetApp());
		objlist.Add(AfxGetApp()->m_pMainWnd);
		objlist.Add(&dc);
		objlist.Add(this);

		d2dc("forward enumeration (via std::for_each) array_adaptor<CObjectArray>:");
		d2dc("");

		mfcstl_ns_qual_std(for_each)(c.begin(), c.end(), d2dc);

#ifdef __STLSOFT_CF_BIDIRECTIONAL_ITERATOR_SUPPORT
		d2dc("reverse enumeration (via std::for_each) array_adaptor<CObjectArray>:");
		d2dc("");

		mfcstl_ns_qual_std(for_each)(c.rbegin(), c.rend(), d2dc);
#endif /* __STLSOFT_CF_BIDIRECTIONAL_ITERATOR_SUPPORT */
	}

	// Ptr
	{
		CPtrArray		ptrlist;
		ptrarray_t		c(ptrlist);

		ptrlist.Add(&d2dc);
		ptrlist.Add(AfxGetApp());
		ptrlist.Add(static_cast<void*>(0));
		ptrlist.Add((void*)"a literal");

		d2dc("forward enumeration (via std::for_each) array_adaptor<CPtrArray>:");
		d2dc("");

		mfcstl_ns_qual_std(for_each)(c.begin(), c.end(), d2dc);

#ifdef __STLSOFT_CF_BIDIRECTIONAL_ITERATOR_SUPPORT
		d2dc("reverse enumeration (via std::for_each) array_adaptor<CPtrArray>:");
		d2dc("");

		mfcstl_ns_qual_std(for_each)(c.rbegin(), c.rend(), d2dc);
#endif /* __STLSOFT_CF_BIDIRECTIONAL_ITERATOR_SUPPORT */
	}

	// String
	{
		CStringArray	strlist;
		strarray_t		c(strlist);

		strlist.Add("String #1");
		strlist.Add("String #2");
		strlist.Add("String #3");
		strlist.Add("String #4");

		d2dc("forward enumeration (via std::for_each) array_adaptor<CStringArray>:");
		d2dc("");

		mfcstl_ns_qual_std(for_each)(c.begin(), c.end(), d2dc);

#ifdef __STLSOFT_CF_BIDIRECTIONAL_ITERATOR_SUPPORT
		d2dc("reverse enumeration (via std::for_each) array_adaptor<CStringArray>:");
		d2dc("");

		mfcstl_ns_qual_std(for_each)(c.rbegin(), c.rend(), d2dc);
#endif /* __STLSOFT_CF_BIDIRECTIONAL_ITERATOR_SUPPORT */
	}

	// String
	{
		CStringArrayT	strlist;
		strlistt_t		c(strlist);

		strlist.Add("String #1");
		strlist.Add("String #2");
		strlist.Add("String #3");
		strlist.Add("String #4");

		d2dc("forward enumeration (via std::for_each) array_adaptor<CArray<CString, CString> >:");
		d2dc("");

		mfcstl_ns_qual_std(for_each)(c.begin(), c.end(), d2dc);

#ifdef __STLSOFT_CF_BIDIRECTIONAL_ITERATOR_SUPPORT
		d2dc("reverse enumeration (via std::for_each) array_adaptor<CArray<CString, CString> >:");
		d2dc("");

		mfcstl_ns_qual_std(for_each)(c.rbegin(), c.rend(), d2dc);
#endif /* __STLSOFT_CF_BIDIRECTIONAL_ITERATOR_SUPPORT */
	}
}

////////////////////////////////////////////////////////////////////////////////

#if 0

	{
		TRACE("ptrarray_t\n");

		ptrarray_t	c(ptrarray);

		c.size();
		c.empty();
		c.max_size();

		ptrarray_t::const_iterator	it	=	c.begin();

#if 0
		dump_item()(*it);
		dump_item()(*++it);
		dump_item()(*--it);


		dump_item()(*--(++c.begin()));
		dump_item()(*c.begin());
		dump_item()(*++c.begin());
#endif /* 0 */

		mfcstl_ns_qual_std(for_each)(c.begin(), c.end(), dump_item());
	}

	{
		TRACE("strarrayt_t\n");

		strarrayt_t	c(strarrayt);

		c.size();
		c.empty();
		c.max_size();

		mfcstl_ns_qual_std(for_each)(c.begin(), c.end(), dump_item());

		if(!c.empty())
		{
			dump_item()(c.begin()[1]);

			dump_item()(c[2]);
		}

		strarrayt_t::size_type	size	=	c.end() - c.begin();

//		strarrayt_t::const_iterator	it2	=	10 + c.begin();
		strarrayt_t::const_iterator	it3	=	c.begin() + 2;

		dump_item()(it3[0]);
		dump_item()(it3[-1]);

		it3 < c.end();
	}
#endif /* 0 */
