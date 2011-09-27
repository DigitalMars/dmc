/* /////////////////////////////////////////////////////////////////////////////
 * File:        mfcstl_string_access.h
 *
 * Purpose:     Contains classes and functions for dealing with MFC strings.
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


/// \file mfcstl_string_access.h
///
/// Contains classes and functions for dealing with MFC strings.

#ifndef MFCSTL_INCL_H_MFCSTL_STRING_ACCESS
#define MFCSTL_INCL_H_MFCSTL_STRING_ACCESS

#ifndef __STLSOFT_DOCUMENTATION_SKIP_SECTION
# define MFCSTL_VER_H_MFCSTL_STRING_ACCESS_MAJOR     2
# define MFCSTL_VER_H_MFCSTL_STRING_ACCESS_MINOR     0
# define MFCSTL_VER_H_MFCSTL_STRING_ACCESS_REVISION  1
# define MFCSTL_VER_H_MFCSTL_STRING_ACCESS_EDIT      50
#endif /* !__STLSOFT_DOCUMENTATION_SKIP_SECTION */

/* /////////////////////////////////////////////////////////////////////////////
 * Includes
 */

#ifndef MFCSTL_INCL_H_MFCSTL
# include "mfcstl.h"    // Include the MFCSTL root header
#endif /* !MFCSTL_INCL_H_MFCSTL */
#ifndef STLSOFT_INCL_H_STLSOFT_STRING_ACCESS
# include "stlsoft_string_access.h"
#endif /* !STLSOFT_INCL_H_STLSOFT_STRING_ACCESS */
#ifndef STLSOFT_INCL_H_STLSOFT_CONSTRAINTS
# include "stlsoft_constraints.h"
#endif /* !STLSOFT_INCL_H_STLSOFT_CONSTRAINTS */
#ifndef STLSOFT_INCL_H_STLSOFT_CSTRING_MAKER
# include "stlsoft_cstring_maker.h"         //
#endif /* !STLSOFT_INCL_H_STLSOFT_CSTRING_MAKER */

#if defined(__STLSOFT_COMPILER_IS_BORLAND)
# pragma warn -8022 /* Suppresses "'f()' hides virtual function 'g()'" */
# pragma warn -8084 /* Suppresses "Suggest parentheses to clarify precedence in function 'f()'" */
#endif /* compiler */

#include <afxwin.h>     // CWnd, CListBox
#if !defined(__AFXCMN_H__) && \
    !defined(MFCSTL_STRING_ACCESS_NO_INCLUDE_AFXCMN)
# include <afxcmn.h>    // CListCtrl
#endif /* !__AFXCMN_H__ && !MFCSTL_STRING_ACCESS_NO_INCLUDE_AFXCMN */
#if !defined(__AFXCVIEW_H__) && \
    !defined(MFCSTL_STRING_ACCESS_NO_INCLUDE_AFXCVIEW)
# include <afxcview.h> // CListView
#endif /* !__AFXCVIEW_H__ && !MFCSTL_STRING_ACCESS_NO_INCLUDE_AFXCMN */

#if defined(__STLSOFT_COMPILER_IS_BORLAND)
# pragma warn .8022 /* Suppresses "'f()' hides virtual function 'g()'" */
# pragma warn .8084 /* Suppresses "Suggest parentheses to clarify precedence in function 'f()'" */
#endif /* compiler */

/* /////////////////////////////////////////////////////////////////////////////
 * Namespace
 *
 * The MFCSTL components are contained within the mfcstl namespace. This is
 * actually an alias for stlsoft::mfcstl_project,
 *
 * The definition matrix is as follows:
 *
 * _STLSOFT_NO_NAMESPACE    _MFCSTL_NO_NAMESPACE    mfcstl definition
 * ---------------------    --------------------    -----------------
 *  not defined              not defined             = stlsoft::mfcstl_project
 *  not defined              defined                 not defined
 *  defined                  not defined             mfcstl
 *  defined                  defined                 not defined
 *
 */

#ifndef _MFCSTL_NO_NAMESPACE
# if defined(_STLSOFT_NO_NAMESPACE) || \
     defined(__STLSOFT_DOCUMENTATION_SKIP_SECTION)
/* There is no stlsoft namespace, so must define ::mfcstl */
namespace mfcstl
{
# else
/* Define stlsoft::mfcstl_project */

namespace stlsoft
{

namespace mfcstl_project
{

# endif /* _STLSOFT_NO_NAMESPACE */
#endif /* !_MFCSTL_NO_NAMESPACE */

/* ////////////////////////////////////////////////////////////////////////// */

/// \weakgroup concepts STLSoft Concepts

/// \weakgroup concepts_shims Shims
/// \ingroup concepts

/// \weakgroup concepts_shims_string_access String Access Shims
/// \ingroup concepts_shims
/// \brief These \ref concepts_shims "shims" retrieve the C-string for arbitrary types

/// \defgroup mfcstl_string_access_shims String Access Shims (MFCSTL)
/// \ingroup MFCSTL concepts_shims_string_access
/// \brief These \ref concepts_shims "shims" retrieve the C-string for arbitrary types
/// @{

/* /////////////////////////////////////////////////////////////////////////////
 * Functions
 */

#ifndef __STLSOFT_DOCUMENTATION_SKIP_SECTION

inline ms_size_t GetWindowTextLength__(CWnd const &w)
{
    return (NULL != w.m_hWnd) ? static_cast<ms_size_t>(w.GetWindowTextLength()) : 0;
}

inline ms_size_t GetWindowText__(CWnd const &w, LPTSTR buffer, ms_size_t cchBuffer)
{
    mfcstl_assert(NULL != w.m_hWnd);

    return static_cast<ms_size_t>(w.GetWindowText(buffer, cchBuffer));
}


inline ms_size_t GetWindowTextLength__(CListBox const &w)
{
    int sel;

    if( 0 == (w.GetStyle() & (LBS_MULTIPLESEL | LBS_EXTENDEDSEL)) &&
        LB_ERR != (sel = w.GetCurSel()))
    {
        return static_cast<ms_size_t>(w.GetTextLen(sel));
    }
    else
    {
        return static_cast<ms_size_t>(w.GetWindowTextLength());
    }
}

inline ms_size_t GetWindowText__(CListBox const &w, LPTSTR buffer, ms_size_t cchBuffer)
{
    int sel = -1;

    if( 0 == (w.GetStyle() & (LBS_MULTIPLESEL | LBS_EXTENDEDSEL)) &&
        LB_ERR != (sel = w.GetCurSel()))
    {
        ms_size_t cch = static_cast<ms_size_t>(w.GetText(sel, buffer));

        mfcstl_message_assert("Invalid item", LB_ERR != cch);
        mfcstl_message_assert("Buffer overwrite", !(cchBuffer < cch));

        return static_cast<ms_size_t>(cch);
    }
    else
    {
        return w.GetWindowText(buffer, cchBuffer);
    }
}

#ifdef __AFXCMN_H__
# if _MFC_VER >= 0x0600
inline ms_size_t GetWindowTextLength__(CListCtrl const &w)
{
    if(1 == w.GetSelectedCount())
    {
        POSITION    pos =   w.GetFirstSelectedItemPosition();
        int         sel =   w.GetNextSelectedItem(pos);

        return static_cast<ms_size_t>(w.GetItemText(sel, 0).GetLength());
    }
    else
    {
        return static_cast<ms_size_t>(w.GetWindowTextLength());
    }
}

inline ms_size_t GetWindowText__(CListCtrl const &w, LPTSTR buffer, ms_size_t cchBuffer)
{
    if(1 == w.GetSelectedCount())
    {
        POSITION    pos =   w.GetFirstSelectedItemPosition();
        int         sel =   w.GetNextSelectedItem(pos);

        return static_cast<ms_size_t>(w.GetItemText(sel, 0, buffer, cchBuffer));
    }
    else
    {
        return w.GetWindowText(buffer, cchBuffer);
    }
}

#  ifdef __AFXCVIEW_H__
inline ms_size_t GetWindowTextLength__(CListView const &w)
{
    return GetWindowTextLength__(w.GetListCtrl());
}

inline ms_size_t GetWindowText__(CListView const &w, LPTSTR buffer, ms_size_t cchBuffer)
{
    return GetWindowText__(w.GetListCtrl(), buffer, cchBuffer);
}
#  endif /* __AFXCVIEW_H__ */
# endif /* _MFC_VER >= 0x0600 */
#endif /* __AFXCMN_H__ */

#endif /* !__STLSOFT_DOCUMENTATION_SKIP_SECTION */


/* /////////////////////////////////////////////////////////////////////////////
 * Classes
 */

/* CWnd */

/// This class provides an intermediary object that may be returned by the
/// c_str_ptr_null() function, such that the window text of a given window
/// may be accessed as a null-terminated string.
class c_str_ptr_null_CWnd_proxy
{
    typedef TCHAR                       char_type;
    typedef cstring_maker<TCHAR>        string_maker_type;
public:
    typedef c_str_ptr_null_CWnd_proxy   class_type;

// Construction
public:
    /// Constructs an instance of the proxy from the given CWnd instance
    ///
    /// \param w The CWnd instance from which the text will be retrieved
    template<ss_typename_param_k W>
    c_str_ptr_null_CWnd_proxy(const W &w)
    {
        stlsoft_constraint_must_have_base(W, CWnd);

        ms_size_t length = GetWindowTextLength__(w);

        if(length == 0)
        {
            m_buffer = NULL;
        }
        else
        {
            m_buffer = string_maker_type::alloc(length);

            if(NULL != m_buffer)
            {
                GetWindowText__(w, m_buffer, length + 1);
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
    c_str_ptr_null_CWnd_proxy(class_type &rhs)
        : m_buffer(rhs.m_buffer)
    {
        rhs.m_buffer = NULL;
    }
#else /* ? __STLSOFT_CF_MOVE_CONSTRUCTOR_SUPPORT */
    // Copy constructor
    c_str_ptr_null_CWnd_proxy(class_type const &rhs)
        : m_buffer(string_maker_type::dup_null(rhs.m_buffer))
    {}
#endif /* __STLSOFT_CF_MOVE_CONSTRUCTOR_SUPPORT */

    /// Releases any storage aquired by the proxy
    ~c_str_ptr_null_CWnd_proxy()
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
class c_str_ptr_CWnd_proxy
{
    typedef TCHAR                   char_type;
    typedef cstring_maker<TCHAR>    string_maker_type;
private:
    typedef c_str_ptr_CWnd_proxy    class_type;

// Construction
public:
    /// Constructs an instance of the proxy from the given CWnd instance
    ///
    /// \param w The CWnd instance from which the text will be retrieved
    template<ss_typename_param_k W>
    c_str_ptr_CWnd_proxy(const W &w)
    {
        stlsoft_constraint_must_have_base(W, CWnd);

        ms_size_t length = GetWindowTextLength__(w);

        if(length == 0)
        {
            m_buffer = string_maker_type::dup(_T(""));
        }
        else
        {
            m_buffer = string_maker_type::alloc(length);

            if(NULL != m_buffer)
            {
                GetWindowText__(w, m_buffer, length + 1);
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
    c_str_ptr_CWnd_proxy(class_type &rhs)
        : m_buffer(rhs.m_buffer)
    {
        rhs.m_buffer = NULL;
    }
#else /* ? __STLSOFT_CF_MOVE_CONSTRUCTOR_SUPPORT */
    // Copy constructor
    c_str_ptr_CWnd_proxy(class_type const &rhs)
        : m_buffer(string_maker_type::dup_null(rhs.m_buffer))
    {}
#endif /* __STLSOFT_CF_MOVE_CONSTRUCTOR_SUPPORT */

    /// Releases any storage aquired by the proxy
    ~c_str_ptr_CWnd_proxy()
    {
        string_maker_type::free(m_buffer);
    }

// Accessors
public:
    /// Returns a null-terminated string representing the window contents, or
    /// the empty string "" if the window contains no text.
    operator LPCTSTR () const
    {
        static char_type    s_ch[1] = { '\0' };

        return (NULL == m_buffer) ? s_ch : m_buffer;
    }

// Implementation
private:
    // \note Does not need to worry about operator new() returning NULL
    static LPTSTR dup_(LPCTSTR s)
    {
        return lstrcpy(new TCHAR[1 + lstrlen(s)], s);
    }

// Members
private:
    LPTSTR  m_buffer;

// Not to be implemented
private:
    void operator =(class_type const &rhs);
};

/* /////////////////////////////////////////////////////////////////////////////
 * Equivalence testing
 */

inline ms_bool_t operator ==(LPCTSTR lhs, c_str_ptr_null_CWnd_proxy const &rhs)
{
    return lhs == static_cast<LPCTSTR>(rhs);
}

inline ms_bool_t operator ==(c_str_ptr_null_CWnd_proxy const &lhs, LPCTSTR rhs)
{
    return static_cast<LPCTSTR>(lhs) == rhs;
}

inline ms_bool_t operator !=(LPCTSTR lhs, c_str_ptr_null_CWnd_proxy const &rhs)
{
    return lhs != static_cast<LPCTSTR>(rhs);
}

inline ms_bool_t operator !=(c_str_ptr_null_CWnd_proxy const &lhs, LPCTSTR rhs)
{
    return static_cast<LPCTSTR>(lhs) != rhs;
}

/* /////////////////////////////////////////////////////////////////////////////
 * IOStream compatibility
 */

template<ss_typename_param_k S>
inline S &operator <<(S & s, c_str_ptr_null_CWnd_proxy const &shim)
{
    s << static_cast<LPCTSTR>(shim);

    return s;
}

template <ss_typename_param_k S>
inline S &operator <<(S & s, c_str_ptr_CWnd_proxy const &shim)
{
    s << static_cast<LPCTSTR>(shim);

    return s;
}

/* /////////////////////////////////////////////////////////////////////////////
 * c_str_ptr_null
 *
 * This can be applied to an expression, and the return value is either a
 * pointer to the character string or NULL.
 */

/* CWnd */
/// \brief Returns the corresponding C-string pointer of the CWnd \c w, or a null pointer
inline c_str_ptr_null_CWnd_proxy c_str_ptr_null(const CWnd &w)
{
    return c_str_ptr_null_CWnd_proxy(w);
}

inline c_str_ptr_null_CWnd_proxy c_str_ptr_null(const CListBox &w)
{
    return c_str_ptr_null_CWnd_proxy(w);
}

#ifdef __AFXCMN_H__
inline c_str_ptr_null_CWnd_proxy c_str_ptr_null(const CListCtrl &w)
{
    return c_str_ptr_null_CWnd_proxy(w);
}

# ifdef __AFXCVIEW_H__
inline c_str_ptr_null_CWnd_proxy c_str_ptr_null(const CListView &w)
{
    return c_str_ptr_null(w.GetListCtrl());
}
# endif /* __AFXCVIEW_H__ */
#endif /* __AFXCMN_H__ */

/* CString */
/// \brief Returns the corresponding C-string pointer of the CString \c s, or a null pointer
inline LPCTSTR c_str_ptr_null(const CString &s)
{
    /* CString always points to valid memory, whether its own
     * CStringData or afxEmptyString.m_pchData
     */
    return s.IsEmpty() ? NULL : (LPCTSTR)s;
}

/* /////////////////////////////////////////////////////////////////////////////
 * c_str_ptr
 *
 * This can be applied to an expression, and the return value is either a
 * pointer to the character string or to an empty string.
 */

/* CWnd */
/// \brief Returns the corresponding C-string pointer of the CWnd \c w
inline c_str_ptr_CWnd_proxy c_str_ptr(const CWnd &w)
{
    return c_str_ptr_CWnd_proxy(w);
}

inline c_str_ptr_CWnd_proxy c_str_ptr(const CListBox &w)
{
    return c_str_ptr_CWnd_proxy(w);
}

#ifdef __AFXCMN_H__
inline c_str_ptr_CWnd_proxy c_str_ptr(const CListCtrl &w)
{
    return c_str_ptr_CWnd_proxy(w);
}

# ifdef __AFXCVIEW_H__
inline c_str_ptr_CWnd_proxy c_str_ptr(const CListView &w)
{
    return c_str_ptr(w.GetListCtrl());
}
# endif /* __AFXCVIEW_H__ */
#endif /* __AFXCMN_H__ */

/* CString */
/// \brief Returns the corresponding C-string pointer of the CString \c s
inline LPCTSTR c_str_ptr(const CString &s)
{
    /* CString always points to valid memory, whether its own
     * CStringData or afxEmptyString.m_pchData
     */
    return s;
}

/* /////////////////////////////////////////////////////////////////////////////
 * c_str_len
 *
 * This can be applied to an expression, and the return value is the number of
 * characters in the character string in the expression.
 */

/* CWnd */
/// \brief Returns the length (in characters) of the contents of the window \c w, <b><i>not</i></b> including the null-terminating character
inline ms_size_t c_str_len(const CWnd &w)
{
    return GetWindowTextLength__(w);
}

inline ms_size_t c_str_len(const CListBox &w)
{
    return GetWindowTextLength__(w);
}

#ifdef __AFXCMN_H__
inline ms_size_t c_str_len(const CListCtrl &w)
{
    return GetWindowTextLength__(w);
}

# ifdef __AFXCVIEW_H__
inline ms_size_t c_str_len(const CListView &w)
{
    return c_str_len(w.GetListCtrl());
}
# endif /* __AFXCVIEW_H__ */
#endif /* __AFXCMN_H__ */

/* CString */
/// \brief Returns the length (in characters) of the CString \c s, <b><i>not</i></b> including the null-terminating character
inline ms_size_t c_str_len(const CString &s)
{
    return s.GetLength();
}

/* /////////////////////////////////////////////////////////////////////////////
 * c_str_size
 *
 * This can be applied to an expression, and the return value is the number of
 * bytes required to store the character string in the expression, NOT including
 * the null-terminating character.
 */

/* CWnd */
/// \brief Returns the size (in bytes) of the contents of the window \c w, <b><i>not</i></b> including the null-terminating character
inline ms_size_t c_str_size(const CWnd &w)
{
    return c_str_len(w) * sizeof(TCHAR);
}

inline ms_size_t c_str_size(const CListBox &w)
{
    return c_str_len(w) * sizeof(TCHAR);
}

#ifdef __AFXCMN_H__
inline ms_size_t c_str_size(const CListCtrl &w)
{
    return c_str_len(w) * sizeof(TCHAR);
}

# ifdef __AFXCVIEW_H__
inline ms_size_t c_str_size(const CListView &w)
{
    return c_str_size(w.GetListCtrl());
}
# endif /* __AFXCVIEW_H__ */
#endif /* __AFXCMN_H__ */

/* CString */
/// \brief Returns the size (in bytes) of the CString \c s, <b><i>not</i></b> including the null-terminating character
inline ms_size_t c_str_size(const CString &s)
{
    return c_str_len(s) * sizeof(TCHAR);
}

////////////////////////////////////////////////////////////////////////////////
// Unit-testing

#ifdef STLSOFT_UNITTEST

namespace unittest
{
    namespace
    {
        ss_bool_t test_mfcstl_string_access_CString(unittest_reporter *r)
        {
            ss_bool_t   bSuccess = true;

            CString     str1("String #1");

            if(0 != _tcscmp(_T("String #1"), c_str_ptr(str1)))
            {
                r->report("c_str_ptr(CString) failed ", __LINE__);
                bSuccess = false;
            }

            if(0 != _tcscmp(_T("String #1"), c_str_ptr_null(str1)))
            {
                r->report("c_str_ptr_null(CString) failed ", __LINE__);
                bSuccess = false;
            }

            CString     str2;

            if(NULL != c_str_ptr_null(str2))
            {
                r->report("c_str_ptr_null(CString) failed ", __LINE__);
                bSuccess = false;
            }

            return bSuccess;
        }

        ss_bool_t test_mfcstl_string_access_CWnd(unittest_reporter *r)
        {
            ss_bool_t   bSuccess = true;

            CWnd    wnd1;
            CWnd    *pwndDesktopWindow  =   CWnd::GetDesktopWindow();
            RECT    rc                  =   { 0, 0, 10, 10 };

            if( wnd1.Create("BUTTON", "Window #1", NULL, rc, pwndDesktopWindow, 0) &&
                0 != strcmp("Window #1", c_str_ptr(wnd1)))
            {
                r->report("c_str_ptr(CWnd) failed ", __LINE__);
                bSuccess = false;
            }

            wnd1.DestroyWindow();

            CWnd    wnd2;

            if(0 != strcmp("", c_str_ptr(wnd2)))
            {
                r->report("c_str_ptr(CWnd) failed ", __LINE__);
                bSuccess = false;
            }

            if(NULL != c_str_ptr_null(wnd2))
            {
                r->report("c_str_ptr_null(CWnd) failed ", __LINE__);
                bSuccess = false;
            }

            return bSuccess;
        }

    } // anonymous namespace

    ss_bool_t test_mfcstl_string_access(unittest_reporter *r)
    {
        using stlsoft::unittest::unittest_initialiser;

        ss_bool_t               bSuccess    =   true;

        unittest_initialiser    init(r, "MFCSTL", "string_access", __FILE__);

        // 1. Test CString
        if(bSuccess)
        {
            bSuccess = test_mfcstl_string_access_CString(r);
        }

        // 1. Test CWnd
        if(bSuccess)
        {
            bSuccess = test_mfcstl_string_access_CWnd(r);
        }

        return bSuccess;
    }

    unittest_registrar    unittest_mfcstl_string_access(test_mfcstl_string_access);

} // namespace unittest

#endif /* STLSOFT_UNITTEST */

/* ////////////////////////////////////////////////////////////////////////// */

/// @} // end of group mfcstl_string_access_shims

/* ////////////////////////////////////////////////////////////////////////// */

#ifndef _MFCSTL_NO_NAMESPACE
# if defined(_STLSOFT_NO_NAMESPACE) || \
     defined(__STLSOFT_DOCUMENTATION_SKIP_SECTION)
} // namespace mfcstl
# else
} // namespace stlsoft::mfcstl_project
} // namespace stlsoft
# endif /* _STLSOFT_NO_NAMESPACE */
#endif /* !_MFCSTL_NO_NAMESPACE */

/* /////////////////////////////////////////////////////////////////////////////
 * Namespace
 *
 * The string access shims exist either in the stlsoft namespace, or in the
 * global namespace. This is required by the lookup rules.
 *
 */

#ifndef _MFCSTL_NO_NAMESPACE
# if !defined(_STLSOFT_NO_NAMESPACE) && \
     !defined(__STLSOFT_DOCUMENTATION_SKIP_SECTION)
namespace stlsoft
{
# else /* ? _STLSOFT_NO_NAMESPACE */
/* There is no stlsoft namespace, so must define in the global namespace */
# endif /* !_STLSOFT_NO_NAMESPACE */

using ::mfcstl::c_str_ptr_null;

using ::mfcstl::c_str_ptr;

using ::mfcstl::c_str_len;

using ::mfcstl::c_str_size;

# if !defined(_STLSOFT_NO_NAMESPACE) && \
     !defined(__STLSOFT_DOCUMENTATION_SKIP_SECTION)
} // namespace stlsoft
# else /* ? _STLSOFT_NO_NAMESPACE */
/* There is no stlsoft namespace, so must define in the global namespace */
# endif /* !_STLSOFT_NO_NAMESPACE */
#endif /* !_MFCSTL_NO_NAMESPACE */

/* ////////////////////////////////////////////////////////////////////////// */

#endif /* !MFCSTL_INCL_H_MFCSTL_STRING_ACCESS */

/* ////////////////////////////////////////////////////////////////////////// */
