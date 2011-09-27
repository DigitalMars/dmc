/* /////////////////////////////////////////////////////////////////////////////
 * File:        atlstl_string_access.h
 *
 * Purpose:     Contains classes and functions for dealing with OLE/COM strings.
 *
 * Created:
 * Updated:     11th September 2004
 *
 * Home:        http://stlsoft.org/
 *
 * Copyright (c) 2002-2004, Matthew Wilson and Synesis Software
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * - Redistributions of source code must retain the above copyright notice, this
 *   list of conditions and the following disclaimer.
 * - Redistributions in binary form must reproduce the above copyright notice,
 *   this list of conditions and the following disclaimer in the documentation
 *   and/or other materials provided with the distribution.
 * - Neither the name(s) of Matthew Wilson and Synesis Software nor the names of
 *   any contributors may be used to endorse or promote products derived from
 *   this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 * ////////////////////////////////////////////////////////////////////////// */


/// \file atlstl_string_access.h
///
/// Contains classes and functions for dealing with OLE/COM strings.

#ifndef ATLSTL_INCL_H_ATLSTL_STRING_ACCESS
#define ATLSTL_INCL_H_ATLSTL_STRING_ACCESS

#ifndef __STLSOFT_DOCUMENTATION_SKIP_SECTION
# define ATLSTL_VER_H_ATLSTL_STRING_ACCESS_MAJOR     2
# define ATLSTL_VER_H_ATLSTL_STRING_ACCESS_MINOR     0
# define ATLSTL_VER_H_ATLSTL_STRING_ACCESS_REVISION  1
# define ATLSTL_VER_H_ATLSTL_STRING_ACCESS_EDIT      56
#endif /* !__STLSOFT_DOCUMENTATION_SKIP_SECTION */

/* /////////////////////////////////////////////////////////////////////////////
 * Includes
 */

#ifndef ATLSTL_INCL_H_ATLSTL
# include "atlstl.h"                        // Include the ATLSTL root header
#endif /* !ATLSTL_INCL_H_ATLSTL */
#ifndef STLSOFT_INCL_H_STLSOFT_STRING_ACCESS
# include "stlsoft_string_access.h"         //
#endif /* !STLSOFT_INCL_H_STLSOFT_STRING_ACCESS */
#ifndef STLSOFT_INCL_H_STLSOFT_CSTRING_MAKER
# include "stlsoft_cstring_maker.h"         //
#endif /* !STLSOFT_INCL_H_STLSOFT_CSTRING_MAKER */
#ifdef STLSOFT_UNITTEST
# include <wchar.h>
#endif /* STLSOFT_UNITTEST */

//#ifndef STLSOFT_CF_ALLOCATOR_REBIND_SUPPORT
//# include "winstl_processheap_allocator.h"    // winstl::processheap_allocator
//#endif /* STLSOFT_CF_ALLOCATOR_REBIND_SUPPORT */

/* /////////////////////////////////////////////////////////////////////////////
 * Namespace
 *
 * The ATLSTL components are contained within the atlstl namespace. This is
 * actually an alias for stlsoft::atlstl_project,
 *
 * The definition matrix is as follows:
 *
 * _STLSOFT_NO_NAMESPACE    _ATLSTL_NO_NAMESPACE    atlstl definition
 * ---------------------    --------------------    -----------------
 *  not defined              not defined             = stlsoft::atlstl_project
 *  not defined              defined                 not defined
 *  defined                  not defined             atlstl
 *  defined                  defined                 not defined
 *
 */

#ifndef _ATLSTL_NO_NAMESPACE
# if defined(_STLSOFT_NO_NAMESPACE) || \
     defined(__STLSOFT_DOCUMENTATION_SKIP_SECTION)
/* There is no stlsoft namespace, so must define ::atlstl */
namespace atlstl
{
# else
/* Define stlsoft::atlstl_project */

namespace stlsoft
{

namespace atlstl_project
{

# endif /* _STLSOFT_NO_NAMESPACE */
#endif /* !_ATLSTL_NO_NAMESPACE */

/* ////////////////////////////////////////////////////////////////////////// */

/// \weakgroup concepts STLSoft Concepts

/// \weakgroup concepts_shims Shims
/// \ingroup concepts

/// \weakgroup concepts_shims_string_access String Access Shims
/// \ingroup concepts_shims
/// \brief These \ref concepts_shims "shims" retrieve the C-string for arbitrary types

/// \defgroup atlstl_string_access_shims String Access Shims (ATLSTL)
/// \ingroup ATLSTL concepts_shims_string_access
/// \brief These \ref concepts_shims "shims" retrieve the C-string for arbitrary types
/// @{

/* /////////////////////////////////////////////////////////////////////////////
 * Classes
 */

/* CWindow */

#ifdef __ATLWIN_H__
/// This class provides an intermediary object that may be returned by the
/// c_str_ptr_null() function, such that the window text of a given window
/// may be accessed as a null-terminated string.
class c_str_ptr_null_CWindow_proxy
{
private:
    typedef cstring_maker<TCHAR>            string_maker_type;
    typedef c_str_ptr_null_CWindow_proxy    class_type;

// Construction
public:
    /// Constructs an instance of the proxy from the given CWindow instance
    ///
    /// \param w The CWindow instance from which the text will be retrieved
    ss_explicit_k c_str_ptr_null_CWindow_proxy(CWindow const &w)
    {
        int length  =   (NULL == w.m_hWnd) ? 0 : w.GetWindowTextLength();

        if(length == 0)
        {
            m_buffer = NULL;
        }
        else
        {
            m_buffer = string_maker_type::alloc(length);

            if(NULL != m_buffer)
            {
                w.GetWindowText(m_buffer, length + 1);
            }
        }
    }

#ifdef __STLSOFT_CF_MOVE_CONSTRUCTOR_SUPPORT
    /// Move constructor
    ///
    /// This <a href = "http://synesis.com.au/resources/articles/cpp/movectors.pdf">move constructor</a>
    /// is for circumstances when the compiler does not, or cannot, apply the
    /// return value optimisation. It causes the contents of \c rhs to be
    /// transferred into the constructing instance. This is completely safe
    /// because the \c rhs instance will never be accessed in its own right, so
    /// does not need to maintain ownership of its contents.
    c_str_ptr_null_CWindow_proxy(class_type &rhs)
        : m_buffer(rhs.m_buffer)
    {
        rhs.m_buffer = 0;
    }
#else /* ? __STLSOFT_CF_MOVE_CONSTRUCTOR_SUPPORT */
    // Copy constructor
    c_str_ptr_null_CWindow_proxy(class_type const &rhs)
        : m_buffer(string_maker_type::dup_null(rhs.m_buffer))
    {}
#endif /* __STLSOFT_CF_MOVE_CONSTRUCTOR_SUPPORT */

    /// Releases any storage aquired by the proxy
    ~c_str_ptr_null_CWindow_proxy()
    {
        string_maker_type::free(m_buffer);
    }

// Accessors
public:
    /// Returns a null-terminated string representing the window contents, or
    /// NULL if the window contains no text.
    operator LPCTSTR () const
    {
        return m_buffer;
    }

// Members
private:
    LPTSTR  m_buffer;

// Not to be implemented
private:
    void operator =(class_type const &rhs);
};

/// This class provides an intermediary object that may be returned by the
/// c_str_ptr() function, such that the window text of a given window may be
/// accessed as a null-terminated string.
class c_str_ptr_CWindow_proxy
{
private:
    typedef cstring_maker<TCHAR>        string_maker_type;
    typedef c_str_ptr_CWindow_proxy     class_type;

// Construction
public:
    /// Constructs an instance of the proxy from the given CWindow instance
    ///
    /// \param w The CWindow instance from which the text will be retrieved
    ss_explicit_k c_str_ptr_CWindow_proxy(CWindow const &w)
    {
        int length  =   (NULL == w.m_hWnd) ? 0 : w.GetWindowTextLength();

        if(length == 0)
        {
            m_buffer = string_maker_type::dup("");
        }
        else
        {
            m_buffer = string_maker_type::alloc(length);

            if(NULL != m_buffer)
            {
                w.GetWindowText(m_buffer, length + 1);
            }
        }
    }

#ifdef __STLSOFT_CF_MOVE_CONSTRUCTOR_SUPPORT
    /// Move constructor
    ///
    /// This <a href = "http://synesis.com.au/resources/articles/cpp/movectors.pdf">move constructor</a>
    /// is for circumstances when the compiler does not, or cannot, apply the
    /// return value optimisation. It causes the contents of \c rhs to be
    /// transferred into the constructing instance. This is completely safe
    /// because the \c rhs instance will never be accessed in its own right, so
    /// does not need to maintain ownership of its contents.
    c_str_ptr_CWindow_proxy(class_type &rhs)
        : m_buffer(rhs.m_buffer)
    {
        rhs.m_buffer = 0;
    }
#else /* ? __STLSOFT_CF_MOVE_CONSTRUCTOR_SUPPORT */
    // Copy constructor
    c_str_ptr_CWindow_proxy(class_type const &rhs)
        : m_buffer(string_maker_type::dup_null(rhs.m_buffer))
    {}
#endif /* __STLSOFT_CF_MOVE_CONSTRUCTOR_SUPPORT */

    /// Releases any storage aquired by the proxy
    ~c_str_ptr_CWindow_proxy()
    {
        string_maker_type::free(m_buffer);
    }

// Accessors
public:
    /// Returns a null-terminated string representing the window contents, or
    /// the empty string "" if the window contains no text.
    operator LPCTSTR () const
    {
        return (NULL == m_buffer) ? "" : m_buffer;
    }

// Members
protected:
    LPTSTR  m_buffer;

// Not to be implemented
protected:
    void operator =(class_type const &rhs);
};
#endif /* __ATLWIN_H__ */

/* /////////////////////////////////////////////////////////////////////////////
 * Equivalence testing
 */

#ifdef __ATLWIN_H__
inline as_bool_t operator ==(LPCTSTR lhs, c_str_ptr_null_CWindow_proxy const &rhs)
{
    return lhs == static_cast<LPCTSTR>(rhs);
}

inline as_bool_t operator ==(c_str_ptr_null_CWindow_proxy const &lhs, LPCTSTR rhs)
{
    return static_cast<LPCTSTR>(lhs) == rhs;
}

inline as_bool_t operator !=(LPCTSTR lhs, c_str_ptr_null_CWindow_proxy const &rhs)
{
    return lhs != static_cast<LPCTSTR>(rhs);
}

inline as_bool_t operator !=(c_str_ptr_null_CWindow_proxy const &lhs, LPCTSTR rhs)
{
    return static_cast<LPCTSTR>(lhs) != rhs;
}
#endif /* __ATLWIN_H__ */

/* /////////////////////////////////////////////////////////////////////////////
 * IOStream compatibility
 */

#ifdef __ATLWIN_H__
template<ss_typename_param_k S>
inline S &operator <<(S & s, c_str_ptr_null_CWindow_proxy const &shim)
{
    s << static_cast<LPCTSTR>(shim);

    return s;
}

template <ss_typename_param_k S>
inline S &operator <<(S & s, c_str_ptr_CWindow_proxy const &shim)
{
    s << static_cast<LPCTSTR>(shim);

    return s;
}
#endif /* __ATLWIN_H__ */

/* /////////////////////////////////////////////////////////////////////////////
 * c_str_ptr_null
 *
 * This can be applied to an expression, and the return value is either a
 * pointer to the character string or NULL.
 */

/* CWindow */
#ifdef __ATLWIN_H__
/// \brief Returns the corresponding C-string pointer of the CWindow \c w, or a null pointer
inline c_str_ptr_null_CWindow_proxy c_str_ptr_null(atlstl_ns_qual_atl(CWindow) const &w)
{
    return c_str_ptr_null_CWindow_proxy(w);
}
#endif /* __ATLWIN_H__ */

/* CComBSTR */
/// \brief Returns the corresponding C-string pointer of the CComBSTR \c s, or a null pointer
inline LPCOLESTR c_str_ptr_null(atlstl_ns_qual_atl(CComBSTR) const &s)
{
    /* NULL is a valid BSTR value, so may return that */
    return s.m_str;
}

/* /////////////////////////////////////////////////////////////////////////////
 * c_str_ptr
 *
 * This can be applied to an expression, and the return value is either a
 * pointer to the character string or to an empty string.
 */

/* CWindow */
#ifdef __ATLWIN_H__
/// \brief Returns the corresponding C-string pointer of the CWindow \c w
inline c_str_ptr_CWindow_proxy c_str_ptr(atlstl_ns_qual_atl(CWindow) const &w)
{
    return c_str_ptr_CWindow_proxy(w);
}
#endif /* __ATLWIN_H__ */

/* CComBSTR */
/// \brief Returns the corresponding C-string pointer of the CComBSTR \c s
inline LPCOLESTR c_str_ptr(atlstl_ns_qual_atl(CComBSTR) const &s)
{
    /* NULL is a valid BSTR value, so check for that */
    return (s.m_str != 0) ? s.m_str : L"";
}

/* /////////////////////////////////////////////////////////////////////////////
 * c_str_len
 *
 * This can be applied to an expression, and the return value is the number of
 * characters in the character string in the expression.
 */

/* CWindow */
#ifdef __ATLWIN_H__
/// \brief Returns the length (in characters) of the contents of the window \c w, <b><i>not</i></b> including the null-terminating character
inline as_size_t c_str_len(atlstl_ns_qual_atl(CWindow) const &w)
{
    return static_cast<as_size_t>(w.GetWindowTextLength());
}
#endif /* __ATLWIN_H__ */

/* CComBSTR */
/// \brief Returns the length (in characters) of the CComBSTR \c s, <b><i>not</i></b> including the null-terminating character
inline as_size_t c_str_len(atlstl_ns_qual_atl(CComBSTR) const &s)
{
    return s.Length();
}

/* /////////////////////////////////////////////////////////////////////////////
 * c_str_size
 *
 * This can be applied to an expression, and the return value is the number of
 * bytes required to store the character string in the expression, NOT including
 * the null-terminating character.
 */

/* CWindow */
#ifdef __ATLWIN_H__
/// \brief Returns the size (in bytes) of the contents of the window \c w, <b><i>not</i></b> including the null-terminating character
inline as_size_t c_str_size(atlstl_ns_qual_atl(CWindow) const &w)
{
    return c_str_len(w) * sizeof(TCHAR);
}
#endif /* __ATLWIN_H__ */

/* CComBSTR */
/// \brief Returns the size (in bytes) of the CComBSTR \c s, <b><i>not</i></b> including the null-terminating character
inline as_size_t c_str_size(atlstl_ns_qual_atl(CComBSTR) const &s)
{
    return c_str_len(s) * sizeof(OLECHAR);
}

////////////////////////////////////////////////////////////////////////////////
// Unit-testing

#ifdef STLSOFT_UNITTEST

namespace unittest
{
    namespace
    {
        ss_bool_t test_atlstl_string_access_CComBSTR(unittest_reporter *r)
        {
            ss_bool_t   bSuccess = true;
#if defined(__STLSOFT_COMPILER_IS_BORLAND)
            CComBSTR    bstr1(L"String #1");
#else /* ? Borland */
            CComBSTR    bstr1("String #1"); // Borland's CComBSTR(LPCSTR) leaves a character off the end!!
#endif /* Borland */

            if(0 != wcscmp(L"String #1", c_str_ptr(bstr1)))
            {
                r->report("c_str_ptr(CComBSTR) failed ", __LINE__);
                bSuccess = false;
            }

            if(0 != wcscmp(L"String #1", c_str_ptr_null(bstr1)))
            {
                r->report("c_str_ptr_null(CComBSTR) failed ", __LINE__);
                bSuccess = false;
            }

            CComBSTR    bstr2;

            if(0 != wcscmp(L"", c_str_ptr(bstr2)))
            {
                r->report("c_str_ptr(CComBSTR) failed with empty string", __LINE__);
                bSuccess = false;
            }

            if(NULL != c_str_ptr_null(bstr2))
            {
                r->report("c_str_ptr_null(CComBSTR) failed with empty string", __LINE__);
                bSuccess = false;
            }

            return bSuccess;
        }

        ss_bool_t test_atlstl_string_access_CWindow(unittest_reporter *r)
        {
            ss_bool_t   bSuccess = true;
            RECT        rc      =   { 0, 0, 10, 10 };
#if (_ATL_VER & 0xff00) < 0x0300
            HWND        hwnd1   =   ::CreateWindowEx(0, "BUTTON", "Window #1", 0, rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top, NULL, (HMENU)0, _Module.GetModuleInstance(), NULL);
            CWindow     wnd1(hwnd1);
#else /* ? _ATL_VER */
            CWindow     wnd1;
            HWND        hwnd1   =   wnd1.Create("BUTTON", NULL, rc, "Window #1");
#endif /* _ATL_VER */

            if( NULL != hwnd1 &&
                0 != strcmp("Window #1", c_str_ptr(wnd1)))
            {
                r->report("c_str_ptr(CWindow) failed ", __LINE__);
                bSuccess = false;
            }

            CWindow     wnd2;

            if(0 != lstrcmp(_T(""), c_str_ptr(wnd2)))
            {
                r->report("c_str_ptr(CWindow) failed with empty window", __LINE__);
                bSuccess = false;
            }

            if(NULL != c_str_ptr_null(wnd2))
            {
                r->report("c_str_ptr_null(CWindow) failed with empty window", __LINE__);
                bSuccess = false;
            }

            return bSuccess;
        }

    } // anonymous namespace

    ss_bool_t test_atlstl_string_access(unittest_reporter *r)
    {
        using stlsoft::unittest::unittest_initialiser;

        ss_bool_t               bSuccess    =   true;

        unittest_initialiser    init(r, "ATLSTL", "string_access", __FILE__);

        // 1. Test CComBSTR
        if(bSuccess)
        {
            bSuccess = test_atlstl_string_access_CComBSTR(r);
        }

        // 1. Test CWindow
        if(bSuccess)
        {
            bSuccess = test_atlstl_string_access_CWindow(r);
        }

        return bSuccess;
    }

    unittest_registrar    unittest_atlstl_string_access(test_atlstl_string_access);

} // namespace unittest

#endif /* STLSOFT_UNITTEST */

/* ////////////////////////////////////////////////////////////////////////// */

/// @} // end of group atlstl_string_access_shims

/* ////////////////////////////////////////////////////////////////////////// */

#ifndef _ATLSTL_NO_NAMESPACE
# if defined(_STLSOFT_NO_NAMESPACE) || \
     defined(__STLSOFT_DOCUMENTATION_SKIP_SECTION)
} // namespace atlstl
# else
} // namespace stlsoft::atlstl_project
} // namespace stlsoft
# endif /* _STLSOFT_NO_NAMESPACE */
#endif /* !_ATLSTL_NO_NAMESPACE */

/* /////////////////////////////////////////////////////////////////////////////
 * Namespace
 *
 * The string access shims exist either in the stlsoft namespace, or in the
 * global namespace. This is required by the lookup rules.
 *
 */

#ifndef _ATLSTL_NO_NAMESPACE
# if !defined(_STLSOFT_NO_NAMESPACE) && \
     !defined(__STLSOFT_DOCUMENTATION_SKIP_SECTION)
namespace stlsoft
{
# else /* ? _STLSOFT_NO_NAMESPACE */
/* There is no stlsoft namespace, so must define in the global namespace */
# endif /* !_STLSOFT_NO_NAMESPACE */

using ::atlstl::c_str_ptr_null;

using ::atlstl::c_str_ptr;

using ::atlstl::c_str_len;

using ::atlstl::c_str_size;

# if !defined(_STLSOFT_NO_NAMESPACE) && \
     !defined(__STLSOFT_DOCUMENTATION_SKIP_SECTION)
} // namespace stlsoft
# else /* ? _STLSOFT_NO_NAMESPACE */
/* There is no stlsoft namespace, so must define in the global namespace */
# endif /* !_STLSOFT_NO_NAMESPACE */
#endif /* !_ATLSTL_NO_NAMESPACE */

/* ////////////////////////////////////////////////////////////////////////// */

#endif /* !ATLSTL_INCL_H_ATLSTL_STRING_ACCESS */

/* ////////////////////////////////////////////////////////////////////////// */
