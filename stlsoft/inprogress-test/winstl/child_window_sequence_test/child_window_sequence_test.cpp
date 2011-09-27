/* /////////////////////////////////////////////////////////////////////////////
 * File:        child_window_sequence_test.cpp (contains extracts from a version
 *              of winstl_child_window_sequence_as_fiber.h)
 *
 * Purpose:     Illustration of enumeration model unification via the Win32
 *              Fiber() API. This source is derived from a work in progress
 *              for the WinSTL project.
 *
 * Created:     15th January 2002
 * Updated:     30th May 2004
 *
 * Author:      Matthew Wilson, Synesis Software Pty Ltd.
 *
 * License:     (Licensed under the Synesis Software Standard Source License)
 *
 *              Copyright (C) 2002, Synesis Software Pty Ltd.
 *
 *              All rights reserved.
 *
 *              www:        http://www.synesis.com.au/winstl
 *                          http://www.winstl.org/
 *
 *              email:      submissions@winstl.org  for submissions
 *                          admin@winstl.org        for other enquiries
 *
 *              Redistribution and use in source and binary forms, with or
 *              without modification, are permitted provided that the following
 *              conditions are met:
 *
 *              (i) Redistributions of source code must retain the above
 *              copyright notice and contact information, this list of
 *              conditions and the following disclaimer.
 *
 *              (ii) Any derived versions of this software (howsoever modified)
 *              remain the sole property of Synesis Software.
 *
 *              (iii) Any derived versions of this software (howsoever modified)
 *              remain subject to all these conditions.
 *
 *              (iv) Neither the name of Synesis Software nor the names of any
 *              subdivisions, employees or agents of Synesis Software, nor the
 *              names of any other contributors to this software may be used to
 *              endorse or promote products derived from this software without
 *              specific prior written permission.
 *
 *              This source code is provided by Synesis Software "as is" and any
 *              warranties, whether expressed or implied, including, but not
 *              limited to, the implied warranties of merchantability and
 *              fitness for a particular purpose are disclaimed. In no event
 *              shall the Synesis Software be liable for any direct, indirect,
 *              incidental, special, exemplary, or consequential damages
 *              (including, but not limited to, procurement of substitute goods
 *              or services; loss of use, data, or profits; or business
 *              interruption) however caused and on any theory of liability,
 *              whether in contract, strict liability, or tort (including
 *              negligence or otherwise) arising in any way out of the use of
 *              this software, even if advised of the possibility of such
 *              damage.
 *
 * ////////////////////////////////////////////////////////////////////////// */



#define _WIN32_WINNT 0x0400

#include <windows.h>
#include <stdio.h>

//#include <winstl.h>

#if defined(_MT) || \
	defined(__MT__) || \
	defined(_REENTRANT)
#error This implementation is NOT compatible with multithread compilation. The
#error implementation is for illustrative purposes only, in order to show how
#error Fibers may be used for unifying enumeration concepts. A serious
#error implementation must provide a fiber-local, rather than, static variable
#endif // multi-threaded

/* This structure holds the state of the enumeration and the relationship
 * between the main and worker fibers
 */
struct cws_fiber_data
{
	LPVOID	main_fiber;
	LPVOID	worker_fiber;
	HWND	hwndParent;
	HWND	hwndChild;

	cws_fiber_data()
		: main_fiber(NULL)
		, worker_fiber(NULL)
		, hwndParent(NULL)
		, hwndChild(NULL)
	{}
};

/* The iterator class */
class child_window_sequence_const_iterator
{
// Construction
public:
	child_window_sequence_const_iterator(cws_fiber_data *fiberData)
		: m_data(fiberData)
	{}

	child_window_sequence_const_iterator(child_window_sequence_const_iterator &rhs)
		: m_data(rhs.m_data)
	{
		// Since this is a simple illustrative implementation, this
		// copy-constructor modifies the rhs, and takes exclusive
		// ownership of the data. A full implementation would aim to
		// provide correctly Assignable (cloneable) iterators.
		rhs.m_data = NULL;
	}

	~child_window_sequence_const_iterator()
	{
		// Since this class is not assignable, it is always safe to
		// do this here (in case the enumeration was not completed).
		if(m_data != NULL)
		{
			::DeleteFiber(m_data->worker_fiber);
			delete m_data;
		}
	}

	HWND operator *() const
	{
		// Should never be called when m_data is NULL
		return m_data->hwndChild;
	}

	child_window_sequence_const_iterator &operator ++()
	{
		// Switch to the enumeration fiber
		::SwitchToFiber(m_data->worker_fiber);

		// Having switched back, the child window member will contain
		// the next child, or will be NULL, in which case enumeration
		// is complete.
		if(m_data->hwndChild == NULL)
		{
			::DeleteFiber(m_data->worker_fiber);
			delete m_data;
			m_data = NULL;
		}

		return *this;
	}

// Comparison
public:
	bool operator == (const child_window_sequence_const_iterator &rhs)
	{
		// Equal if either both are NULL, or both non-NULL and have same child
		// window.
		return	(	m_data == NULL &&
					rhs.m_data == NULL) ||
				(	m_data != NULL &&
					rhs.m_data != NULL &&
					m_data->hwndChild == rhs.m_data->hwndChild);
	}
	bool operator != (const child_window_sequence_const_iterator &rhs)
	{
		return !operator == (rhs);
	}


// Members
protected:
	cws_fiber_data	*m_data;

// Unimplemented
private:
	// The iterators are not truly Assignable
//	child_window_sequence_const_iterator(const child_window_sequence_const_iterator &rhs);
	child_window_sequence_const_iterator &operator =(const child_window_sequence_const_iterator &);
};

class child_window_sequence
{
public:
	typedef	child_window_sequence					class_type;
	typedef	child_window_sequence_const_iterator	const_iterator;

// Construction
public:
	child_window_sequence(HWND hwnd)
		: m_fiberMain(GetMainFiber())
		, m_hwnd(hwnd)
	{

	}

// Iteration
public:
	const_iterator begin()
	{
		// (i) Create a shared area
		cws_fiber_data	*data	=	new cws_fiber_data;

		// (ii) Create the callback fiber
		data->main_fiber		=	m_fiberMain;
		data->worker_fiber		=	::CreateFiber(0, WorkerProc, data);
		data->hwndParent		=	m_hwnd;

		// (iii) Switch to fiber and start the enumeration
		::SwitchToFiber(data->worker_fiber);

		// (iv) set the first window and the fiber data into the iterator

		if(data->hwndChild == NULL)
		{
			::DeleteFiber(data->worker_fiber);
			delete data;
			data = NULL;
		}

		return child_window_sequence_const_iterator(data);
	}

	const_iterator end()
	{
		return const_iterator(NULL);
	}

// Implementation
protected:
	// Use of this member reduces the fragility of using Fibers somewhat, by
	// ensuring that all instances of child_window_sequence class share a
	// particular main fiber.
	static LPVOID GetMainFiber()
	{
		static LPVOID	s_fiberMain	=	::ConvertThreadToFiber(0);

		return s_fiberMain;
	}

	static void WINAPI WorkerProc(PVOID lpParameter)
	{
		// This procedure makes the enumeration call, and then
		// scopes the enumeration via fiber exchanges between this fiber
		// and the main fiber (where the iterators are

		cws_fiber_data	&data	=	*(cws_fiber_data*)lpParameter;

		// Start the enumeration
		::EnumChildWindows(data.hwndParent, EnumChildProc, (LPARAM)lpParameter);

		// Set the child to NULL to indicate enumeration end to the
		// main fiber ...
		data.hwndChild = NULL;

		// ... and switch back for the last time (main will delete this
		// fiber).
		::SwitchToFiber(data.main_fiber);
	}

	static BOOL CALLBACK EnumChildProc(HWND hwnd, LPARAM lParam)
	{
		cws_fiber_data	&data	=	*(cws_fiber_data*)lParam;

		// Now we place this result in the data, and switch back
		data.hwndChild = hwnd;

		::SwitchToFiber(data.main_fiber);

		return true;
	}

// Members
protected:
	HWND	m_hwnd;			// Window whose children will be enumerated
	LPVOID	m_fiberMain;	// The main (this) fiber
};

#include <functional>
#include <algorithm>

struct is_visible
	: public std::unary_function<HWND, bool>
{
	bool operator ()(HWND hwnd)
	{
		return ::IsWindowVisible(hwnd) != false;
	}
};

struct trace_window_text
	: public std::unary_function<HWND, void>
{
	void operator ()(HWND hwnd)
	{
		//
		TCHAR	szText[260];

		::GetWindowText(hwnd, szText, sizeof(szText) / sizeof(szText[0]));

		printf("0x%08x: \"%s\"\n", hwnd, szText);
	}
};

template<class I, class F, class P>
inline F for_each_if(I first, I last, F function, P pred)
{
	for(; first != last; ++first)
	{
		if(pred(*first))
		{
			function(*first);
		}
	}

	return function;
}


int main(int /* argc */, char ** /*argv*/)
{
	child_window_sequence					desktop(::GetDesktopWindow());

	// (i) Use in long-hand

	child_window_sequence::const_iterator	begin	=	desktop.begin();
	child_window_sequence::const_iterator	end		=	desktop.end();

	for(; begin != end; ++begin)
	{
		if(is_visible()(*begin))
		{
			trace_window_text()(*begin);
		}
	}

	// (i) Use via algorithm
	for_each_if(desktop.begin(), desktop.end(), trace_window_text(), is_visible());

    return 0;
}
