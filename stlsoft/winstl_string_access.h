/* /////////////////////////////////////////////////////////////////////////////
 * File:        winstl_string_access.h
 *
 * Purpose:     Contains classes and functions for dealing with Win32 strings.
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


/// \file winstl_string_access.h
///
/// Contains classes and functions for dealing with Win32 strings.

#ifndef WINSTL_INCL_H_WINSTL_STRING_ACCESS
#define WINSTL_INCL_H_WINSTL_STRING_ACCESS

#ifndef __STLSOFT_DOCUMENTATION_SKIP_SECTION
# define WINSTL_VER_H_WINSTL_STRING_ACCESS_MAJOR     2
# define WINSTL_VER_H_WINSTL_STRING_ACCESS_MINOR     0
# define WINSTL_VER_H_WINSTL_STRING_ACCESS_REVISION  1
# define WINSTL_VER_H_WINSTL_STRING_ACCESS_EDIT      69
#endif /* !__STLSOFT_DOCUMENTATION_SKIP_SECTION */

/* ////////////////////////////////////////////////////////////////////////////
 * Includes
 */

#ifndef WINSTL_INCL_H_WINSTL
# include "winstl.h"                // Include the WinSTL root header
#endif /* !WINSTL_INCL_H_WINSTL */
#ifndef STLSOFT_INCL_H_STLSOFT_STRING_ACCESS
# include "stlsoft_string_access.h"
#endif /* !STLSOFT_INCL_H_STLSOFT_STRING_ACCESS */
#ifndef WINSTL_INCL_H_WINSTL_WINDOW_FUNCTIONS
# include "winstl_window_functions.h"
#endif /* !WINSTL_INCL_H_WINSTL_WINDOW_FUNCTIONS */
#ifndef WINSTL_INCL_H_WINSTL_WINDOWS_IDENT
# include "winstl_windows_ident.h"
#endif /* !WINSTL_INCL_H_WINSTL_WINDOWS_IDENT */
#ifndef STLSOFT_INCL_H_STLSOFT_CSTRING_MAKER
# include "stlsoft_cstring_maker.h"         //
#endif /* !STLSOFT_INCL_H_STLSOFT_CSTRING_MAKER */

/* /////////////////////////////////////////////////////////////////////////////
 * Namespace
 *
 * The WinSTL components are contained within the winstl namespace. This is
 * actually an alias for stlsoft::winstl_project,
 *
 * The definition matrix is as follows:
 *
 * _STLSOFT_NO_NAMESPACE    _WINSTL_NO_NAMESPACE    winstl definition
 * ---------------------    --------------------    -----------------
 *  not defined              not defined             = stlsoft::winstl_project
 *  not defined              defined                 not defined
 *  defined                  not defined             winstl
 *  defined                  defined                 not defined
 *
 */

#ifndef _WINSTL_NO_NAMESPACE
# if defined(_STLSOFT_NO_NAMESPACE) || \
     defined(__STLSOFT_DOCUMENTATION_SKIP_SECTION)
/* There is no stlsoft namespace, so must define ::winstl */
namespace winstl
{
# else
/* Define stlsoft::winstl_project */

namespace stlsoft
{

namespace winstl_project
{

# endif /* _STLSOFT_NO_NAMESPACE */
#endif /* !_WINSTL_NO_NAMESPACE */

/* ////////////////////////////////////////////////////////////////////////// */

/// \weakgroup concepts STLSoft Concepts

/// \weakgroup concepts_shims Shims
/// \ingroup concepts

/// \weakgroup concepts_shims_string_access String Access Shims
/// \ingroup concepts_shims
/// \brief These \ref concepts_shims "shims" retrieve the C-string for arbitrary types

/// \defgroup winstl_string_access_shims String Access Shims (WinSTL)
/// \ingroup WinSTL concepts_shims_string_access
/// \brief These \ref concepts_shims "shims" retrieve the C-string for arbitrary types
/// @{

/* /////////////////////////////////////////////////////////////////////////////
 * Functions
 */

#ifndef __STLSOFT_DOCUMENTATION_SKIP_SECTION

inline ws_size_t GetWindowTextLength__(HWND hwnd)
{
    WindowIdent ident   =   GetWindowIdent(hwnd);
    int         sel;

    switch(ident)
    {
        case    ListBox:
            if(0 == (GetStyle(hwnd) & (LBS_MULTIPLESEL | LBS_EXTENDEDSEL)))
            {
                sel = (int)::SendMessage(hwnd, LB_GETCURSEL, 0, 0l);

                if(LB_ERR != sel)
                {
                    return static_cast<ws_size_t>(::SendMessage(hwnd, LB_GETTEXTLEN, static_cast<WPARAM>(sel), 0L));
                }
                else
                {
                    return 0;
                }
            }
            break;
#if 0
        case    ListBox:
            if(1 == SendMessage(hwnd, LVM_GETSELECTEDCOUNT, 0, 0L))
            {
                sel =
            }
            break;
#endif /* 0 */
        default:
            break;
    }

    return ::GetWindowTextLength(hwnd);
}

inline ws_size_t GetWindowTextA__(HWND hwnd, ws_char_a_t *buffer, ws_size_t cchBuffer)
{
    WindowIdent ident   =   GetWindowIdent(hwnd);
    int         sel;
    ws_size_t      cch;

    switch(ident)
    {
        case    ListBox:
            if(0 == (GetStyle(hwnd) & (LBS_MULTIPLESEL | LBS_EXTENDEDSEL)))
            {
                sel = (int)::SendMessage(hwnd, LB_GETCURSEL, 0, 0l);

                if(LB_ERR != sel)
                {
                    cch =   static_cast<ws_size_t>(::SendMessage(hwnd, LB_GETTEXT, static_cast<WPARAM>(sel), reinterpret_cast<LPARAM>(buffer)));

                    // Some programs using list-boxes do not null-terminate - Visual
                    // SourceSafe Explorer, anyone? - so we must do so here.
                    buffer[cch] = '\0';
                }
                else
                {
                    buffer[0] = '\0';

                    cch = 0;
                }

                winstl_message_assert("Buffer overwrite", !(cchBuffer < cch));

                return cch;
            }
            break;
        default:
            break;
    }

    return ::GetWindowTextA(hwnd, buffer, static_cast<int>(cchBuffer));
}

inline ws_size_t GetWindowTextW__(HWND hwnd, ws_char_w_t *buffer, ws_size_t cchBuffer)
{
    WindowIdent ident   =   GetWindowIdent(hwnd);
    int         sel;

    switch(ident)
    {
        case    ListBox:
            if(0 == (GetStyle(hwnd) & (LBS_MULTIPLESEL | LBS_EXTENDEDSEL)))
            {
                ws_size_t  cch;

                sel = (int)::SendMessage(hwnd, LB_GETCURSEL, 0, 0l);

                if(LB_ERR != sel)
                {
                    cch =   static_cast<ws_size_t>(::SendMessage(hwnd, LB_GETTEXT, static_cast<WPARAM>(sel), reinterpret_cast<LPARAM>(buffer)));
                }
                else
                {
                    buffer[0] = '\0';

                    cch = 0;
                }

                winstl_message_assert("Buffer overwrite", !(cchBuffer < cch));

                return cch;
            }
            break;
        default:
            break;
    }

    return ::GetWindowTextW(hwnd, buffer, static_cast<int>(cchBuffer));
}
#endif /* !__STLSOFT_DOCUMENTATION_SKIP_SECTION */


/* /////////////////////////////////////////////////////////////////////////////
 * Classes
 */

/* HWND */
/// This class provides an intermediary object that may be returned by the
/// c_str_ptr_null() function, such that the window text of a given window may be
/// accessed as a null-terminated string.
template <ss_typename_param_k C>
class c_str_ptr_null_HWND_proxy
{
    typedef cstring_maker<C>                            string_maker_type;
public:
    typedef C                                           char_type;
    typedef c_str_ptr_null_HWND_proxy<C>                class_type;

// Construction
public:
    /// Constructs an instance of the proxy from the given HWND
    ///
    /// \param h The HWND from which the text will be retrieved
    ss_explicit_k c_str_ptr_null_HWND_proxy(HWND h)
    {
        ws_size_t length  =   GetWindowTextLength__(h);

        if(length == 0)
        {
            m_buffer = NULL;
        }
        else
        {
            m_buffer = string_maker_type::alloc(length);

            if(NULL != m_buffer)
            {
                get_window_text(h, m_buffer, length + 1);
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
    c_str_ptr_null_HWND_proxy(class_type &rhs)
        : m_buffer(rhs.m_buffer)
    {
        move_lhs_from_rhs(rhs).m_buffer = NULL;
    }
#else /* ? __STLSOFT_CF_MOVE_CONSTRUCTOR_SUPPORT */
    // Copy constructor
    c_str_ptr_null_HWND_proxy(class_type const &rhs)
        : m_buffer(string_maker_type::dup_null(rhs.m_buffer))
    {}
#endif /* __STLSOFT_CF_MOVE_CONSTRUCTOR_SUPPORT */

    /// Releases any storage aquired by the proxy
    ~c_str_ptr_null_HWND_proxy()
    {
        string_maker_type::free(m_buffer);
    }

// Accessors
public:
    /// Returns a null-terminated string representing the window contents, or
    /// the empty string "" if the window contains no text.
    operator char_type const *() const
    {
        return m_buffer;
    }

// Implementation
private:
    ws_size_t get_window_text(HWND h, char_type *buffer, ws_size_t cchBuffer);

// Members
private:
    char_type   *m_buffer;

// Not to be implemented
private:
    void operator =(class_type const &rhs);
};

#ifndef __STLSOFT_DOCUMENTATION_SKIP_SECTION

STLSOFT_TEMPLATE_SPECIALISATION
inline ws_size_t c_str_ptr_null_HWND_proxy<ws_char_a_t>::get_window_text(HWND h, ws_char_a_t *buffer, ws_size_t cchBuffer)
{
    return GetWindowTextA__(h, buffer, cchBuffer);
}

STLSOFT_TEMPLATE_SPECIALISATION
inline ws_size_t c_str_ptr_null_HWND_proxy<ws_char_w_t>::get_window_text(HWND h, ws_char_w_t *buffer, ws_size_t cchBuffer)
{
    return GetWindowTextW__(h, buffer, cchBuffer);
}

#endif /* !__STLSOFT_DOCUMENTATION_SKIP_SECTION */

/// This class provides an intermediary object that may be returned by the
/// c_str_ptr() function, such that the window text of a given window may be
/// accessed as a null-terminated string.
template <ss_typename_param_k C>
class c_str_ptr_HWND_proxy
{
    typedef cstring_maker<C>                            string_maker_type;
public:
    typedef C                                           char_type;
    typedef c_str_ptr_HWND_proxy<C>                     class_type;

// Construction
public:
    /// Constructs an instance of the proxy from the given HWND
    ///
    /// \param h The HWND from which the text will be retrieved
    ss_explicit_k c_str_ptr_HWND_proxy(HWND h)
    {
        ws_size_t length  =   GetWindowTextLength__(h);

        m_buffer = string_maker_type::alloc(length);

        if(NULL != m_buffer)
        {
            get_window_text(h, m_buffer, length + 1);
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
    c_str_ptr_HWND_proxy(class_type &rhs)
        : m_buffer(rhs.m_buffer)
    {
        move_lhs_from_rhs(rhs).m_buffer = NULL;
    }
#else /* ? __STLSOFT_CF_MOVE_CONSTRUCTOR_SUPPORT */
    // Copy constructor
    c_str_ptr_HWND_proxy(class_type const &rhs)
        : m_buffer(string_maker_type::dup_null(rhs.m_buffer))
    {}
#endif /* __STLSOFT_CF_MOVE_CONSTRUCTOR_SUPPORT */

    /// Releases any storage aquired by the proxy
    ~c_str_ptr_HWND_proxy()
    {
        string_maker_type::free(m_buffer);
    }

// Accessors
public:
    /// Returns a null-terminated string representing the window contents, or
    /// the empty string "" if the window contains no text.
    operator char_type const *() const
    {
        static char_type    s_ch[1] = { '\0' };

        return (NULL == m_buffer) ? s_ch : m_buffer;
    }

// Implementation
private:
    ws_size_t get_window_text(HWND h, char_type *buffer, ws_size_t cchBuffer);

// Members
private:
    char_type   *m_buffer;

// Not to be implemented
private:
    void operator =(class_type const &rhs);
};

#ifndef __STLSOFT_DOCUMENTATION_SKIP_SECTION

STLSOFT_TEMPLATE_SPECIALISATION
inline ws_size_t c_str_ptr_HWND_proxy<ws_char_a_t>::get_window_text(HWND h, ws_char_a_t *buffer, ws_size_t cchBuffer)
{
    return GetWindowTextA__(h, buffer, cchBuffer);
}

STLSOFT_TEMPLATE_SPECIALISATION
inline ws_size_t c_str_ptr_HWND_proxy<ws_char_w_t>::get_window_text(HWND h, ws_char_w_t *buffer, ws_size_t cchBuffer)
{
    return GetWindowTextW__(h, buffer, cchBuffer);
}

#endif /* !__STLSOFT_DOCUMENTATION_SKIP_SECTION */


/* Windows Policy Management LSA_UNICODE_STRING string */
#ifdef _NTSECAPI_
/// This class provides an intermediary object that may be returned by the
/// c_str_ptr_null() function, such that the text of a given LSA_UNICODE_STRING
/// string may be accessed as a null-terminated string.
class c_str_ptr_LSA_UNICODE_STRING_proxy
{
    typedef cstring_maker<WCHAR>                string_maker_type;
public:
    typedef c_str_ptr_LSA_UNICODE_STRING_proxy  class_type;

// Construction
public:
    /// Constructs an instance of the proxy from the given LSA_UNICODE_STRING instance
    ///
    /// \param s The LSA_UNICODE_STRING instance from which the text will be retrieved
    ss_explicit_k c_str_ptr_LSA_UNICODE_STRING_proxy(const LSA_UNICODE_STRING &s)
        : m_buffer(string_maker_type::alloc(s.Length))
    {
        if(NULL != m_buffer)
        {
            wcsncpy(m_buffer, s.Buffer, s.Length);
            m_buffer[s.Length] = L'\0';
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
    c_str_ptr_LSA_UNICODE_STRING_proxy(class_type &rhs)
        : m_buffer(rhs.m_buffer)
    {
        rhs.m_buffer = NULL;
    }
#else /* ? __STLSOFT_CF_MOVE_CONSTRUCTOR_SUPPORT */
    // Copy constructor
    c_str_ptr_LSA_UNICODE_STRING_proxy(class_type const &rhs)
        : m_buffer(string_maker_type::dup_null(rhs.m_buffer))
    {}
#endif /* __STLSOFT_CF_MOVE_CONSTRUCTOR_SUPPORT */

    /// Releases any storage aquired by the proxy
    ~c_str_ptr_LSA_UNICODE_STRING_proxy()
    {
        string_maker_type::free(m_buffer);
    }

// Accessors
public:
    /// Returns a null-terminated string representing the string contents, or
    /// the empty string "" if the string has no contents.
    operator LPCWSTR () const
    {
        return m_buffer;
    }

// Members
private:
    LPWSTR  m_buffer;

// Not to be implemented
private:
    void operator =(class_type const &rhs);
};
#endif /* _NTSECAPI_ */

#ifdef _NTSECAPI_
/// This class provides an intermediary object that may be returned by the
/// c_str_ptr_null() function, such that the text of a given LSA_UNICODE_STRING
/// string may be accessed as a null-terminated string.
class c_str_ptr_null_LSA_UNICODE_STRING_proxy
{
    typedef cstring_maker<WCHAR>                    string_maker_type;
public:
    typedef c_str_ptr_null_LSA_UNICODE_STRING_proxy class_type;

// Construction
public:
    /// Constructs an instance of the proxy from the given LSA_UNICODE_STRING instance
    ///
    /// \param s The LSA_UNICODE_STRING instance from which the text will be retrieved
    ss_explicit_k c_str_ptr_null_LSA_UNICODE_STRING_proxy(const LSA_UNICODE_STRING &s)
        : m_buffer((s.Length != 0) ? string_maker_type::alloc(s.Length) : NULL)
    {
        if(m_buffer != NULL)
        {
            wcsncpy(m_buffer, s.Buffer, s.Length);
            m_buffer[s.Length] = L'\0';
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
    c_str_ptr_null_LSA_UNICODE_STRING_proxy(class_type &rhs)
        : m_buffer(rhs.m_buffer)
    {
        rhs.m_buffer = NULL;
    }
#else /* ? __STLSOFT_CF_MOVE_CONSTRUCTOR_SUPPORT */
    // Copy constructor
    c_str_ptr_null_LSA_UNICODE_STRING_proxy(class_type const &rhs)
        : m_buffer(string_maker_type::dup_null(rhs.m_buffer))
    {}
#endif /* __STLSOFT_CF_MOVE_CONSTRUCTOR_SUPPORT */

    /// Releases any storage aquired by the proxy
    ~c_str_ptr_null_LSA_UNICODE_STRING_proxy()
    {
        string_maker_type::free(m_buffer);
    }

// Accessors
public:
    /// Returns a null-terminated string representing the string contents, or
    /// NULL if the string has no contents.
    operator LPCWSTR () const
    {
        return m_buffer;
    }

// Members
private:
    LPWSTR  m_buffer;

// Not to be implemented
private:
    void operator =(class_type const &rhs);
};
#endif /* _NTSECAPI_ */

/* /////////////////////////////////////////////////////////////////////////////
 * IOStream compatibility
 */

template<   ss_typename_param_k C
        ,   ss_typename_param_k S
        >
inline S &operator <<(S & s, c_str_ptr_null_HWND_proxy<C> const &shim)
{
    s << static_cast<C const *>(shim);

    return s;
}

template<   ss_typename_param_k C
        ,   ss_typename_param_k S
        >
inline S &operator <<(S & s, c_str_ptr_HWND_proxy<C> const &shim)
{
    s << static_cast<C const *>(shim);

    return s;
}

#ifdef _NTSECAPI_
template<ss_typename_param_k S>
inline S &operator <<(S & s, c_str_ptr_LSA_UNICODE_STRING_proxy const &shim)
{
    s << static_cast<LPCWSTR>(shim);

    return s;
}

template<ss_typename_param_k S>
inline S &operator <<(S & s, c_str_ptr_null_LSA_UNICODE_STRING_proxy const &shim)
{
    s << static_cast<LPCWSTR>(shim);

    return s;
}
#endif /* _NTSECAPI_ */

/* /////////////////////////////////////////////////////////////////////////////
 * c_str_ptr_null
 *
 * This can be applied to an expression, and the return value is either a
 * pointer to the character string or NULL.
 */

/* HWND */
/// \brief Returns the corresponding C-string pointer of the window \c h, or a null pointer
inline c_str_ptr_null_HWND_proxy<ws_char_a_t> c_str_ptr_null_a(HWND h)
{
    return c_str_ptr_null_HWND_proxy<ws_char_a_t>(h);
}

/// \brief Returns the corresponding C-string pointer of the window \c h, or a null pointer
inline c_str_ptr_null_HWND_proxy<ws_char_w_t> c_str_ptr_null_w(HWND h)
{
    return c_str_ptr_null_HWND_proxy<ws_char_w_t>(h);
}

/// \brief Returns the corresponding C-string pointer of the window \c h, or a null pointer
inline c_str_ptr_null_HWND_proxy<TCHAR> c_str_ptr_null(HWND h)
{
    return c_str_ptr_null_HWND_proxy<TCHAR>(h);
}

/* LSA_UNICODE_STRING */
#ifdef _NTSECAPI_
/// \brief Returns the corresponding C-string pointer of the LSA_UNICODE_STRING \c s, or a null pointer
inline c_str_ptr_null_LSA_UNICODE_STRING_proxy c_str_ptr_null(const LSA_UNICODE_STRING &s)
{
    return c_str_ptr_null_LSA_UNICODE_STRING_proxy(s);
}
#endif /* _NTSECAPI_ */

/* /////////////////////////////////////////////////////////////////////////////
 * c_str_ptr
 *
 * This can be applied to an expression, and the return value is either a
 * pointer to the character string or to an empty string.
 */

/* HWND */
/// \brief Returns the corresponding C-string pointer of the window \c h
inline c_str_ptr_HWND_proxy<ws_char_a_t> c_str_ptr_a(HWND h)
{
    return c_str_ptr_HWND_proxy<ws_char_a_t>(h);
}

/// \brief Returns the corresponding C-string pointer of the window \c h
inline c_str_ptr_HWND_proxy<ws_char_w_t> c_str_ptr_w(HWND h)
{
    return c_str_ptr_HWND_proxy<ws_char_w_t>(h);
}

/// \brief Returns the corresponding C-string pointer of the window \c h
inline c_str_ptr_HWND_proxy<TCHAR> c_str_ptr(HWND h)
{
    return c_str_ptr_HWND_proxy<TCHAR>(h);
}

/* LSA_UNICODE_STRING */
#ifdef _NTSECAPI_
/// \brief Returns the corresponding C-string pointer of the LSA_UNICODE_STRING \c s
inline c_str_ptr_LSA_UNICODE_STRING_proxy c_str_ptr(const LSA_UNICODE_STRING &s)
{
    return c_str_ptr_LSA_UNICODE_STRING_proxy(s);
}
#endif /* _NTSECAPI_ */

/* /////////////////////////////////////////////////////////////////////////////
 * c_str_len
 *
 * This can be applied to an expression, and the return value is the number of
 * characters in the character string in the expression.
 */

/* HWND */
/// \brief Returns the length (in characters) of the string of \c s, <b><i>not</i></b> including the null-terminating character
inline ws_size_t c_str_len(HWND h)
{
    return GetWindowTextLength__(h);
}

/// \brief Returns the length (in characters) of the string of \c s, <b><i>not</i></b> including the null-terminating character
inline ws_size_t c_str_len_a(HWND h)
{
    return c_str_len(h);
}

/// \brief Returns the length (in characters) of the string of \c s, <b><i>not</i></b> including the null-terminating character
inline ws_size_t c_str_len_w(HWND h)
{
    return c_str_len(h);
}

/* LSA_UNICODE_STRING */
#ifdef _NTSECAPI_
/// \brief Returns the length (in characters) of the LSA_UNICODE_STRING \c s, <b><i>not</i></b> including the null-terminating character
inline ws_size_t c_str_len(const LSA_UNICODE_STRING &s)
{
    return s.Length;
}
#endif /* _NTSECAPI_ */

/* /////////////////////////////////////////////////////////////////////////////
 * c_str_size
 *
 * This can be applied to an expression, and the return value is the number of
 * bytes required to store the character string in the expression, NOT including
 * the null-terminating character.
 */

/* HWND */
/// \brief Returns the size (in bytes) of the string of \c h, <b><i>not</i></b> including the null-terminating character
inline ws_size_t c_str_size_a(HWND h)
{
    return c_str_len(h) * sizeof(ws_char_a_t);
}

/// \brief Returns the size (in bytes) of the string of \c h, <b><i>not</i></b> including the null-terminating character
inline ws_size_t c_str_size_w(HWND h)
{
    return c_str_len(h) * sizeof(ws_char_w_t);
}

/// \brief Returns the size (in bytes) of the string of \c h, <b><i>not</i></b> including the null-terminating character
inline ws_size_t c_str_size(HWND h)
{
    return c_str_len(h) * sizeof(TCHAR);
}

/* LSA_UNICODE_STRING */
#ifdef _NTSECAPI_
/// \brief Returns the size (in bytes) of the LSA_UNICODE_STRING \c s, <b><i>not</i></b> including the null-terminating character
inline ws_size_t c_str_size(const LSA_UNICODE_STRING &s)
{
    return c_str_len(s) * sizeof(WCHAR);
}
#endif /* _NTSECAPI_ */

////////////////////////////////////////////////////////////////////////////////
// Unit-testing

#ifdef STLSOFT_UNITTEST

namespace unittest
{
    namespace
    {
        ss_bool_t test_winstl_string_access_HWND(unittest_reporter *r)
        {
            ss_bool_t   bSuccess = true;

            HWND        hwnd    =   ::CreateWindow("BUTTON", "Window #1", 0, 0, 0, 0, 0, NULL, NULL, NULL, NULL);

            if( NULL != hwnd &&
                0 != strcmp("Window #1", c_str_ptr_a(hwnd)))
            {
                r->report("c_str_ptr(HWND) failed ", __LINE__);
                bSuccess = false;
            }

            return bSuccess;
        }

        ss_bool_t test_winstl_string_access_LSA_UNICODE_STRING(unittest_reporter *r)
        {
            ss_bool_t   bSuccess = true;

#ifdef _NTSECAPI_
            WCHAR               buffer[100] = L"LSA-String #1";
            LSA_UNICODE_STRING  lsa1 =
            {
                    static_cast<USHORT>(wcslen(buffer))
                ,   stlsoft_num_elements(buffer)
                ,   buffer
            };

            if(0 != wcscmp(c_str_ptr(lsa1), L"LSA-String #1"))
            {
                r->report("c_str_ptr(LSA_UNICODE_STRING) failed ", __LINE__);
                bSuccess = false;
            }
#else  /* ?_NTSECAPI_ */
            STLSOFT_SUPPRESS_UNUSED(r);
#endif /* _NTSECAPI_ */

            return bSuccess;
        }

    } // anonymous namespace

    ss_bool_t test_winstl_string_access(unittest_reporter *r)
    {
        using stlsoft::unittest::unittest_initialiser;

        ss_bool_t               bSuccess    =   true;

        unittest_initialiser    init(r, "WinSTL", "string_access", __FILE__);

        return bSuccess;
    }

    unittest_registrar    unittest_winstl_string_access(test_winstl_string_access);

} // namespace unittest

#endif /* STLSOFT_UNITTEST */

/* ////////////////////////////////////////////////////////////////////////// */

/// @} // end of group winstl_string_access_shims

/* ////////////////////////////////////////////////////////////////////////// */

#ifndef _WINSTL_NO_NAMESPACE
# if defined(_STLSOFT_NO_NAMESPACE) || \
     defined(__STLSOFT_DOCUMENTATION_SKIP_SECTION)
} // namespace winstl
# else
} // namespace stlsoft::winstl_project
} // namespace stlsoft
# endif /* _STLSOFT_NO_NAMESPACE */
#endif /* !_WINSTL_NO_NAMESPACE */

/* /////////////////////////////////////////////////////////////////////////////
 * Namespace
 *
 * The string access shims exist either in the stlsoft namespace, or in the
 * global namespace. This is required by the lookup rules.
 *
 */

#ifndef _WINSTL_NO_NAMESPACE
# if !defined(_STLSOFT_NO_NAMESPACE) && \
     !defined(__STLSOFT_DOCUMENTATION_SKIP_SECTION)
namespace stlsoft
{
# else /* ? _STLSOFT_NO_NAMESPACE */
/* There is no stlsoft namespace, so must define in the global namespace */
# endif /* !_STLSOFT_NO_NAMESPACE */

using ::winstl::c_str_ptr_null_a;
using ::winstl::c_str_ptr_a;
using ::winstl::c_str_len_a;
using ::winstl::c_str_size_a;
using ::winstl::c_str_ptr_null_w;
using ::winstl::c_str_ptr_w;
using ::winstl::c_str_len_w;
using ::winstl::c_str_size_w;
using ::winstl::c_str_ptr_null;
using ::winstl::c_str_ptr;
using ::winstl::c_str_len;
using ::winstl::c_str_size;

# if !defined(_STLSOFT_NO_NAMESPACE) && \
     !defined(__STLSOFT_DOCUMENTATION_SKIP_SECTION)
} // namespace stlsoft
# else /* ? _STLSOFT_NO_NAMESPACE */
/* There is no stlsoft namespace, so must define in the global namespace */
# endif /* !_STLSOFT_NO_NAMESPACE */
#endif /* !_WINSTL_NO_NAMESPACE */

/* ////////////////////////////////////////////////////////////////////////// */

#endif /* WINSTL_INCL_H_WINSTL_SYSTEM_INFO */

/* ////////////////////////////////////////////////////////////////////////// */
