/* /////////////////////////////////////////////////////////////////////////////
 * File:        comstl_string_access.h
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


/// \file comstl_string_access.h
///
/// Contains classes and functions for dealing with OLE/COM strings.

#ifndef COMSTL_INCL_H_COMSTL_STRING_ACCESS
#define COMSTL_INCL_H_COMSTL_STRING_ACCESS

#ifndef __STLSOFT_DOCUMENTATION_SKIP_SECTION
# define COMSTL_VER_H_COMSTL_STRING_ACCESS_MAJOR     2
# define COMSTL_VER_H_COMSTL_STRING_ACCESS_MINOR     0
# define COMSTL_VER_H_COMSTL_STRING_ACCESS_REVISION  1
# define COMSTL_VER_H_COMSTL_STRING_ACCESS_EDIT      64
#endif /* !__STLSOFT_DOCUMENTATION_SKIP_SECTION */

/* ////////////////////////////////////////////////////////////////////////////
 * Includes
 */

#ifndef COMSTL_INCL_H_COMSTL
# include "comstl.h"    // Include the COMSTL root header
#endif /* !COMSTL_INCL_H_COMSTL */
#ifndef COMSTL_INCL_H_COMSTL_BSTR_FUNCTIONS
# include "comstl_bstr_functions.h"
#endif /* !COMSTL_INCL_H_COMSTL_BSTR_FUNCTIONS */
#ifndef STLSOFT_INCL_H_STLSOFT_STRING_ACCESS
# include "stlsoft_string_access.h"
#endif /* !STLSOFT_INCL_H_STLSOFT_STRING_ACCESS */
#include <wchar.h>

/* /////////////////////////////////////////////////////////////////////////////
 * Namespace
 *
 * The COMSTL components are contained within the comstl namespace. This is
 * actually an alias for stlsoft::comstl_project,
 *
 * The definition matrix is as follows:
 *
 * _STLSOFT_NO_NAMESPACE    _COMSTL_NO_NAMESPACE    comstl definition
 * ---------------------    --------------------    -----------------
 *  not defined              not defined             = stlsoft::comstl_project
 *  not defined              defined                 not defined
 *  defined                  not defined             comstl
 *  defined                  defined                 not defined
 *
 */

#ifndef _COMSTL_NO_NAMESPACE
# if defined(_STLSOFT_NO_NAMESPACE) || \
     defined(__STLSOFT_DOCUMENTATION_SKIP_SECTION)
/* There is no stlsoft namespace, so must define ::comstl */
namespace comstl
{
# else
/* Define stlsoft::comstl_project */

namespace stlsoft
{

namespace comstl_project
{

# endif /* _STLSOFT_NO_NAMESPACE */
#endif /* !_COMSTL_NO_NAMESPACE */

/* ////////////////////////////////////////////////////////////////////////// */

/// \weakgroup concepts STLSoft Concepts

/// \weakgroup concepts_shims Shims
/// \ingroup concepts

/// \weakgroup concepts_shims_string_access String Access Shims
/// \ingroup concepts_shims
/// \brief These \ref concepts_shims "shims" retrieve the C-string for arbitrary types

/// \defgroup comstl_string_access_shims String Access Shims (COMSTL)
/// \ingroup COMSTL concepts_shims_string_access
/// \brief These \ref concepts_shims "shims" retrieve the C-string for arbitrary types
/// @{

/* /////////////////////////////////////////////////////////////////////////////
 * Constants
 */

const cs_size_t COMSTL_CCH_GUID =   38;

/* /////////////////////////////////////////////////////////////////////////////
 * Functions
 */

inline cs_size_t guid2string_w(GUID const &guid, cs_char_w_t buff[1 + COMSTL_CCH_GUID])
{
    return static_cast<cs_size_t>(StringFromGUID2(guid, buff, 1 + COMSTL_CCH_GUID));
}

inline cs_size_t guid2string_a(GUID const &guid, cs_char_a_t buff[1 + COMSTL_CCH_GUID])
{
    const cs_size_t COMSTL_CCH_GUID_AND_NULL    =   COMSTL_CCH_GUID + 1;

    /* Don't ask! */
#ifdef __STLSOFT_COMPILER_IS_BORLAND
    int         __buff[COMSTL_CCH_GUID_AND_NULL];
    cs_char_w_t *_buff  =   (wchar_t *)__buff;
#else
    cs_char_w_t _buff[COMSTL_CCH_GUID_AND_NULL];
#endif /* __STLSOFT_COMPILER_IS_BORLAND */
    cs_size_t   cch =   guid2string_w(guid, _buff);

    ::WideCharToMultiByte(0, 0, _buff, COMSTL_CCH_GUID_AND_NULL, buff, COMSTL_CCH_GUID_AND_NULL, 0, 0);

    return cch;
}

/* /////////////////////////////////////////////////////////////////////////////
 * Classes
 */

/* GUID */

/// This class provides an intermediary object that may be returned by the
/// c_str_ptr_null() function, such that the text of a given GUID
/// may be accessed as a null-terminated string.
template <ss_typename_param_k C>
class c_str_ptr_GUID_proxy
{
public:
    typedef C                       char_type;
    typedef c_str_ptr_GUID_proxy<C> class_type;

// Construction
public:
    /// Constructs an instance of the proxy from the given GUID instance
    ///
    /// \param guid The GUID instance from which the text will be retrieved
    ss_explicit_k c_str_ptr_GUID_proxy(GUID const &guid);

    /// Copy constructor
    c_str_ptr_GUID_proxy(class_type const &rhs)
        : m_(m_buffer)
    {
        for(int i = 0; i < comstl_num_elements(m_buffer); ++i)
        {
            m_buffer[i] = rhs.m_buffer[i];
        }
    }

// Accessors
public:
    /// Returns a null-terminated string representing the GUID contents
    operator char_type const *() const
    {
        return m_buffer;
    }

// Members
private:
    char_type const *const  m_;
    char_type               m_buffer[1 + COMSTL_CCH_GUID];

// Not to be implemented
private:
    void operator =(class_type const &rhs);
};

STLSOFT_TEMPLATE_SPECIALISATION
inline c_str_ptr_GUID_proxy<cs_char_a_t>::c_str_ptr_GUID_proxy(GUID const &guid)
    : m_(m_buffer)
{
#ifndef __STLSOFT_COMPILER_IS_BORLAND
    stlsoft_static_assert(comstl_num_elements(m_buffer) > COMSTL_CCH_GUID);
#endif /* !__STLSOFT_COMPILER_IS_BORLAND */

    guid2string_a(guid, m_buffer);
}

STLSOFT_TEMPLATE_SPECIALISATION
inline c_str_ptr_GUID_proxy<cs_char_w_t>::c_str_ptr_GUID_proxy(GUID const &guid)
    : m_(m_buffer)
{
#ifndef __STLSOFT_COMPILER_IS_BORLAND
    stlsoft_static_assert(comstl_num_elements(m_buffer) > COMSTL_CCH_GUID);
#endif /* !__STLSOFT_COMPILER_IS_BORLAND */

    guid2string_w(guid, m_buffer);
}


/* VARIANT */

/// This class provides an intermediary object that may be returned by the
/// c_str_ptr_null() function, such that the text of a given variant
/// may be accessed as a null-terminated string.
class c_str_null_VARIANT_proxy
{
public:
    typedef c_str_null_VARIANT_proxy    class_type;

// Construction
public:
    /// Constructs an instance of the proxy from the given BSTR
    ///
    /// \param s The BSTR from which the text will be retrieved
    ss_explicit_k c_str_null_VARIANT_proxy(const BSTR s)
        : m_bstr(s)
        , m_own(false)
    {}

    /// Constructs an instance of the proxy from the given BSTR
    ///
    /// \param ps Pointer to the BSTR from which the text will be retrieved
    ss_explicit_k c_str_null_VARIANT_proxy(BSTR *ps)
        : m_bstr(*ps)
        , m_own(true)
    {
        if(m_own)
        {
            *ps = NULL;
        }
    }

    /// Default constructor
    c_str_null_VARIANT_proxy()
        : m_bstr(NULL)
        , m_own(false)
    {}

#ifdef __STLSOFT_CF_MOVE_CONSTRUCTOR_SUPPORT
    /// Move constructor
    ///
    /// This <a href = "http://synesis.com.au/resources/articles/cpp/movectors.pdf">move constructor</a>
    /// is for circumstances when the compiler does not, or cannot, apply the
    /// return value optimisation. It causes the contents of \c rhs to be
    /// transferred into the constructing instance. This is completely safe
    /// because the \c rhs instance will never be accessed in its own right, so
    /// does not need to maintain ownership of its contents.
    c_str_null_VARIANT_proxy(class_type &rhs)
        : m_bstr(rhs.m_bstr)
        , m_own(rhs.m_own)
    {
        move_lhs_from_rhs(rhs).m_bstr  =   NULL;
        move_lhs_from_rhs(rhs).m_own   =   false;
    }
#else /* ? __STLSOFT_CF_MOVE_CONSTRUCTOR_SUPPORT */
    // Copy constructor
    c_str_null_VARIANT_proxy(class_type const &rhs)
        : m_bstr(bstr_dup(rhs.m_bstr))
    {}
#endif /* __STLSOFT_CF_MOVE_CONSTRUCTOR_SUPPORT */

    /// Releases any storage aquired by the proxy
    ~c_str_null_VARIANT_proxy()
    {
        if(m_own)
        {
            ::SysFreeString(m_bstr);
        }
    }

// Accessors
public:
    /// Returns a null-terminated string representing the VARIANT contents, or
    /// NULL if the VARIANT contents cannot be converted to text.
    operator LPCOLESTR () const
    {
        return m_bstr;
    }

// Members
private:
    BSTR        m_bstr;
    cs_bool_t   m_own;

// Not to be implemented
private:
    void operator =(class_type const &rhs);
};

/// This class provides an intermediary object that may be returned by the
/// c_str_ptr_w() function, such that the text of a given variant
/// may be accessed as a null-terminated string.
class c_str_VARIANT_proxy_w
{
public:
    typedef c_str_VARIANT_proxy_w   class_type;

// Construction
public:
    /// Constructs an instance of the proxy from the given BSTR
    ///
    /// \param s The BSTR from which the text will be retrieved
    ss_explicit_k c_str_VARIANT_proxy_w(BSTR &s)
        : m_bstr(s)
    {
        s = NULL;
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
    c_str_VARIANT_proxy_w(class_type &rhs)
        : m_bstr(rhs.m_bstr)
    {
        move_lhs_from_rhs(rhs).m_bstr = NULL;
    }
#else /* ? __STLSOFT_CF_MOVE_CONSTRUCTOR_SUPPORT */
    // Copy constructor
    c_str_VARIANT_proxy_w(class_type const &rhs)
        : m_bstr(bstr_dup(rhs.m_bstr))
    {}
#endif /* __STLSOFT_CF_MOVE_CONSTRUCTOR_SUPPORT */

    /// Releases any storage aquired by the proxy
    ~c_str_VARIANT_proxy_w()
    {
        ::SysFreeString(m_bstr);
    }

// Accessors
public:
    /// Returns a null-terminated string representing the VARIANT contents.
    operator LPCOLESTR () const
    {
        return (m_bstr == NULL) ? L"" : m_bstr;
    }

// Members
private:
    BSTR    m_bstr;

// Not to be implemented
private:
    void operator =(class_type const &rhs);
};

/// This class provides an intermediary object that may be returned by the
/// c_str_ptr_a() function, such that the text of a given variant
/// may be accessed as a null-terminated string.
class c_str_VARIANT_proxy_a
{
public:
    typedef c_str_VARIANT_proxy_a   class_type;
    typedef c_str_VARIANT_proxy_w   class_w_type;

// Construction
public:
    /// Constructs an instance of the proxy from the given c_str_VARIANT_proxy_w
    ///
    /// \param rhs The c_str_VARIANT_proxy_w from which the text will be retrieved
    ss_explicit_k c_str_VARIANT_proxy_a(class_w_type rhs)
        : m_proxyw(rhs)
        , m_buffer(0)
    {}

#ifdef __STLSOFT_CF_MOVE_CONSTRUCTOR_SUPPORT
    /// Move constructor
    ///
    /// This <a href = "http://synesis.com.au/resources/articles/cpp/movectors.pdf">move constructor</a>
    /// is for circumstances when the compiler does not, or cannot, apply the
    /// return value optimisation. It causes the contents of \c rhs to be
    /// transferred into the constructing instance. This is completely safe
    /// because the \c rhs instance will never be accessed in its own right, so
    /// does not need to maintain ownership of its contents.
    c_str_VARIANT_proxy_a(class_type &rhs)
        : m_proxyw(rhs.m_proxyw)
        , m_buffer(rhs.m_buffer)
    {
        move_lhs_from_rhs(rhs).m_buffer = NULL;
    }
#else /* ? __STLSOFT_CF_MOVE_CONSTRUCTOR_SUPPORT */
    // Copy constructor
    c_str_VARIANT_proxy_a(class_type const &rhs)
        : m_proxyw(rhs.m_proxyw)
        , m_buffer(NULL)
    {}
#endif /* __STLSOFT_CF_MOVE_CONSTRUCTOR_SUPPORT */

    /// Releases any storage aquired by the proxy
    ~c_str_VARIANT_proxy_a()
    {
        if(m_buffer != empty_string_())
        {
            ::CoTaskMemFree(m_buffer);
        }
    }

// Accessors
public:
    /// Returns a null-terminated string representing the VARIANT contents.
    operator cs_char_a_t const *() const
    {
        if(NULL == m_buffer)
        {
            LPCOLESTR   w_value     =   m_proxyw;
            cs_char_a_t *&buffer_   =   const_cast<class_type*>(this)->m_buffer;

            if( NULL == w_value ||
                L'\0' == *w_value)
            {
                buffer_ = empty_string_();
            }
            else
            {
                cs_size_t   cch =   ::SysStringLen((BSTR)w_value);

                buffer_ = static_cast<cs_char_a_t *>(::CoTaskMemAlloc((1 + cch) * sizeof(cs_char_a_t)));

                if(NULL == buffer_)
                {
                    buffer_ = empty_string_();
                }
                else
                {
#ifdef WIN32
                    ::WideCharToMultiByte(0, 0, w_value, -1, buffer_, cch + 1, NULL, NULL);
#else
#error Not currently implemented for operating systems other than Win32
#endif /* __SYNSOFT_DBS_DEBUG */
                }
            }
        }

        return m_buffer;
    }

// Implementation
private:
    static cs_char_a_t *empty_string_()
    {
        // This character array is initialised to 0, which conveniently happens to
        // be the empty string, by the module/application load, so it is
        // guaranteed to be valid, and there are no threading/race conditions
        static cs_char_a_t  s_empty[1];

        comstl_assert(s_empty[0] == '\0'); // Paranoid check

        return s_empty;
    }

// Members
private:
    c_str_VARIANT_proxy_w   m_proxyw;
    cs_char_a_t             *m_buffer;

// Not to be implemented
private:
    void operator =(class_type const &rhs);
};

/* /////////////////////////////////////////////////////////////////////////////
 * Equivalence testing
 */

// c_str_ptr_GUID_proxy
template <ss_typename_param_k C>
inline cs_bool_t operator ==(C const *lhs, c_str_ptr_GUID_proxy<C> const &rhs)
{
    return lhs == static_cast<C const *>(rhs);
}

template <ss_typename_param_k C>
inline cs_bool_t operator ==(c_str_ptr_GUID_proxy<C> const &lhs, C const *rhs)
{
    return static_cast<C const *>(lhs) == rhs;
}

template <ss_typename_param_k C>
inline cs_bool_t operator !=(C const *lhs, c_str_ptr_GUID_proxy<C> const &rhs)
{
    return lhs != static_cast<C const*>(rhs);
}

template <ss_typename_param_k C>
inline cs_bool_t operator !=(c_str_ptr_GUID_proxy<C> const &lhs, C const *rhs)
{
    return static_cast<C const*>(lhs) != rhs;
}

// c_str_null_VARIANT_proxy
inline cs_bool_t operator ==(LPCOLESTR lhs, c_str_null_VARIANT_proxy const &rhs)
{
    return lhs == static_cast<LPCOLESTR>(rhs);
}

inline cs_bool_t operator ==(c_str_null_VARIANT_proxy const &lhs, LPCOLESTR rhs)
{
    return static_cast<LPCOLESTR>(lhs) == rhs;
}

inline cs_bool_t operator !=(LPCOLESTR lhs, c_str_null_VARIANT_proxy const &rhs)
{
    return lhs != static_cast<LPCOLESTR>(rhs);
}

inline cs_bool_t operator !=(c_str_null_VARIANT_proxy const &lhs, LPCOLESTR rhs)
{
    return static_cast<LPCOLESTR>(lhs) != rhs;
}

// c_str_VARIANT_proxy_a
inline cs_bool_t operator ==(LPCSTR lhs, c_str_VARIANT_proxy_a const &rhs)
{
    return lhs == static_cast<LPCSTR>(rhs);
}

inline cs_bool_t operator ==(c_str_VARIANT_proxy_a const &lhs, LPCSTR rhs)
{
    return static_cast<LPCSTR>(lhs) == rhs;
}

inline cs_bool_t operator !=(LPCSTR lhs, c_str_VARIANT_proxy_a const &rhs)
{
    return lhs != static_cast<LPCSTR>(rhs);
}

inline cs_bool_t operator !=(c_str_VARIANT_proxy_a const &lhs, LPCSTR rhs)
{
    return static_cast<LPCSTR>(lhs) != rhs;
}

// c_str_VARIANT_proxy_w
inline cs_bool_t operator ==(LPCOLESTR lhs, c_str_VARIANT_proxy_w const &rhs)
{
    return lhs == static_cast<LPCOLESTR>(rhs);
}

inline cs_bool_t operator ==(c_str_VARIANT_proxy_w const &lhs, LPCOLESTR rhs)
{
    return static_cast<LPCOLESTR>(lhs) == rhs;
}

inline cs_bool_t operator !=(LPCOLESTR lhs, c_str_VARIANT_proxy_w const &rhs)
{
    return lhs != static_cast<LPCOLESTR>(rhs);
}

inline cs_bool_t operator !=(c_str_VARIANT_proxy_w const &lhs, LPCOLESTR rhs)
{
    return static_cast<LPCOLESTR>(lhs) != rhs;
}

/* /////////////////////////////////////////////////////////////////////////////
 * IOStream compatibility
 */

template<   ss_typename_param_k C
        ,   ss_typename_param_k S
        >
inline S &operator <<(S & s, c_str_ptr_GUID_proxy<C> const &shim)
{
    s << static_cast<C const*>(shim);

    return s;
}

template <ss_typename_param_k S>
inline S &operator <<(S & s, c_str_null_VARIANT_proxy const &shim)
{
    s << static_cast<LPCOLESTR>(shim);

    return s;
}

template <ss_typename_param_k S>
inline S &operator <<(S & s, c_str_VARIANT_proxy_w const &shim)
{
    s << static_cast<LPCOLESTR>(shim);

    return s;
}

template <ss_typename_param_k S>
inline S &operator <<(S & s, c_str_VARIANT_proxy_a const &shim)
{
    s << static_cast<cs_char_a_t const*>(shim);

    return s;
}

/* /////////////////////////////////////////////////////////////////////////////
 * c_str_ptr_null
 *
 * This can be applied to an expression, and the return value is either a
 * pointer to the character string or NULL.
 */

/* GUID */
/// \brief Returns the corresponding ANSI C-string pointer of the GUID \c guid
inline c_str_ptr_GUID_proxy<cs_char_a_t> c_str_ptr_null_a(GUID const &guid)
{
    return c_str_ptr_GUID_proxy<cs_char_a_t>(guid);
}

/// \brief Returns the corresponding Unicode C-string pointer of the GUID \c guid
inline c_str_ptr_GUID_proxy<cs_char_w_t> c_str_ptr_null_w(GUID const &guid)
{
    return c_str_ptr_GUID_proxy<cs_char_w_t>(guid);
}

/// \brief Returns the corresponding C-string pointer of the GUID \c guid
inline c_str_ptr_GUID_proxy<cs_char_o_t> c_str_ptr_null_o(GUID const &guid)
{
    return c_str_ptr_GUID_proxy<cs_char_o_t>(guid);
}

/// \brief Returns the corresponding C-string pointer of the GUID \c guid
inline c_str_ptr_GUID_proxy<cs_char_o_t> c_str_ptr_null(GUID const &guid)
{
    return c_str_ptr_null_o(guid);
}

/* VARIANT */
/// \brief Returns the corresponding ANSI C-string pointer of the VARIANT \c v, or a null pointer
inline c_str_null_VARIANT_proxy c_str_ptr_null(VARIANT const &v)
{
    if(v.vt == VT_BSTR)
    {
        return c_str_null_VARIANT_proxy(v.bstrVal);
    }
    else if(v.vt == VT_NULL ||
            v.vt == VT_EMPTY)
    {
        return c_str_null_VARIANT_proxy();
    }
    else
    {
        VARIANT vs;
        HRESULT hr;

        ::VariantInit(&vs);

        hr  =   ::VariantChangeTypeEx(&vs, const_cast<VARIANT *>(&v), LOCALE_USER_DEFAULT, 0, VT_BSTR);

        if(FAILED(hr))
        {
            vs.bstrVal = NULL;
        }

        return c_str_null_VARIANT_proxy(&vs.bstrVal);
    }
}

/* /////////////////////////////////////////////////////////////////////////////
 * c_str_ptr
 *
 * This can be applied to an expression, and the return value is either a
 * pointer to the character string or to an empty string.
 */

/* GUID */
/// \brief Returns the corresponding ANSI C-string pointer of the GUID \c guid
inline c_str_ptr_GUID_proxy<cs_char_a_t> c_str_ptr_a(GUID const &guid)
{
    return c_str_ptr_GUID_proxy<cs_char_a_t>(guid);
}

/// \brief Returns the corresponding Unicode C-string pointer of the GUID \c guid
inline c_str_ptr_GUID_proxy<cs_char_w_t> c_str_ptr_w(GUID const &guid)
{
    return c_str_ptr_GUID_proxy<cs_char_w_t>(guid);
}

/// \brief Returns the corresponding C-string pointer of the GUID \c guid
inline c_str_ptr_GUID_proxy<cs_char_o_t> c_str_ptr_o(GUID const &guid)
{
    return c_str_ptr_GUID_proxy<cs_char_o_t>(guid);
}

/// \brief Returns the corresponding C-string pointer of the GUID \c guid
inline c_str_ptr_GUID_proxy<cs_char_o_t> c_str_ptr(GUID const &guid)
{
    return c_str_ptr_o(guid);
}

/* VARIANT */
/// \brief Returns the corresponding ANSI C-string pointer of the VARIANT \c v
inline c_str_VARIANT_proxy_a c_str_ptr_a(VARIANT const &v)
{
    VARIANT vs;
    HRESULT hr;

    ::VariantInit(&vs);

    hr  =   ::VariantChangeTypeEx(&vs, const_cast<VARIANT *>(&v), LOCALE_USER_DEFAULT, 0, VT_BSTR);

    if(FAILED(hr))
    {
        vs.bstrVal = NULL;
    }

    return c_str_VARIANT_proxy_a(c_str_VARIANT_proxy_w(vs.bstrVal));
}

/// \brief Returns the corresponding Unicode C-string pointer of the VARIANT \c v
inline c_str_VARIANT_proxy_w c_str_ptr_w(VARIANT const &v)
{
    VARIANT vs;
    HRESULT hr;

    ::VariantInit(&vs);

    hr  =   ::VariantChangeTypeEx(&vs, const_cast<VARIANT *>(&v), LOCALE_USER_DEFAULT, VARIANT_ALPHABOOL, VT_BSTR);

    if(FAILED(hr))
    {
        vs.bstrVal = NULL;
    }

    return c_str_VARIANT_proxy_w(vs.bstrVal);
}

/// \brief Returns the corresponding C-string pointer of the VARIANT \c v
inline c_str_VARIANT_proxy_w c_str_ptr(VARIANT const &v)
{
    return c_str_ptr_w(v);
}

/* /////////////////////////////////////////////////////////////////////////////
 * c_str_len
 *
 * This can be applied to an expression, and the return value is the number of
 * characters in the character string in the expression.
 */

/* GUID */
/// \brief Returns the length (in characters) of the GUID \c guid, <b><i>not</i></b> including the null-terminating character
inline cs_size_t c_str_len_a(GUID const &/* guid */)
{
    return COMSTL_CCH_GUID;
}

/// \brief Returns the length (in characters) of the GUID \c guid, <b><i>not</i></b> including the null-terminating character
inline cs_size_t c_str_len_w(GUID const &/* guid */)
{
    return COMSTL_CCH_GUID;
}

/// \brief Returns the length (in characters) of the GUID \c guid, <b><i>not</i></b> including the null-terminating character
inline cs_size_t c_str_len_o(GUID const &/* guid */)
{
    return COMSTL_CCH_GUID;
}

/// \brief Returns the length (in characters) of the GUID \c guid, <b><i>not</i></b> including the null-terminating character
inline cs_size_t c_str_len(GUID const &/* guid */)
{
    return COMSTL_CCH_GUID;
}

/* VARIANT */
/// \brief Returns the length (in characters) of the VARIANT \c v, <b><i>not</i></b> including the null-terminating character
inline cs_size_t c_str_len(VARIANT const &v)
{
    cs_size_t   len;

    if(v.vt == VT_BSTR)
    {
        len = v.bstrVal != NULL ? ::SysStringLen(v.bstrVal) : 0;
    }
    else if(v.vt == VT_NULL ||
            v.vt == VT_EMPTY)
    {
        len = 0;
    }
    else
    {
        VARIANT     vs;
        HRESULT     hr;

        ::VariantInit(&vs);

        hr  =   ::VariantChangeTypeEx(&vs, const_cast<VARIANT *>(&v), LOCALE_USER_DEFAULT, 0, VT_BSTR);

        if(FAILED(hr))
        {
            len = 0;
        }
        else
        {
            len = vs.bstrVal ? ::SysStringLen(vs.bstrVal) : 0;

            ::VariantClear(&vs);
        }
    }

    return len;
}

/// \brief Returns the length (in characters) of the VARIANT \c v, <b><i>not</i></b> including the null-terminating character
inline cs_size_t c_str_len_a(VARIANT const &v)
{
    return c_str_len(v);
}

/// \brief Returns the length (in characters) of the VARIANT \c v, <b><i>not</i></b> including the null-terminating character
inline cs_size_t c_str_len_w(VARIANT const &v)
{
    return c_str_len(v);
}

/* /////////////////////////////////////////////////////////////////////////////
 * c_str_size
 *
 * This can be applied to an expression, and the return value is the number of
 * bytes required to store the character string in the expression, NOT including
 * the null-terminating character.
 */

/* GUID */
/// \brief Returns the size (in bytes) of the GUID \c guid, <b><i>not</i></b> including the null-terminating character
inline cs_size_t c_str_size_a(GUID const &guid)
{
    return c_str_len_a(guid) *sizeof(cs_char_a_t);
}

/// \brief Returns the size (in bytes) of the GUID \c guid, <b><i>not</i></b> including the null-terminating character
inline cs_size_t c_str_size_w(GUID const &guid)
{
    return c_str_len_w(guid) * sizeof(cs_char_w_t);
}

/// \brief Returns the size (in bytes) of the GUID \c guid, <b><i>not</i></b> including the null-terminating character
inline cs_size_t c_str_size_o(GUID const &guid)
{
    return c_str_len_o(guid) * sizeof(cs_char_o_t);
}

/// \brief Returns the size (in bytes) of the GUID \c guid, <b><i>not</i></b> including the null-terminating character
inline cs_size_t c_str_size(GUID const &guid)
{
    return c_str_size_o(guid);
}

/* VARIANT */
/// \brief Returns the size (in bytes) of the VARIANT \c v, <b><i>not</i></b> including the null-terminating character
inline cs_size_t c_str_size_a(VARIANT const &v)
{
    return c_str_len(v) * sizeof(cs_char_a_t);
}

/// \brief Returns the size (in bytes) of the VARIANT \c v, <b><i>not</i></b> including the null-terminating character
inline cs_size_t c_str_size_w(VARIANT const &v)
{
    return c_str_len(v) * sizeof(OLECHAR);
}

/// \brief Returns the size (in bytes) of the VARIANT \c v, <b><i>not</i></b> including the null-terminating character
inline cs_size_t c_str_size(VARIANT const &v)
{
    return c_str_size_w(v);
}

////////////////////////////////////////////////////////////////////////////////
// Unit-testing

#ifdef STLSOFT_UNITTEST

namespace unittest
{
    namespace
    {
        ss_bool_t test_comstl_string_access_GUID(unittest_reporter *r)
        {
            ss_bool_t   bSuccess = true;

            /* Check GUID conversion */
            GUID const  guid    =   GUID_NULL;

            if(0 != wcscmp(c_str_ptr(guid), L"{00000000-0000-0000-0000-000000000000}"))
            {
                r->report("c_str_ptr(GUID) failed ", __LINE__);
                bSuccess = false;
            }

            if(0 != wcscmp(c_str_ptr_null(guid), L"{00000000-0000-0000-0000-000000000000}"))
            {
                r->report("c_str_ptr_null(GUID) failed ", __LINE__);
                bSuccess = false;
            }

            if(COMSTL_CCH_GUID != c_str_len(guid))
            {
                r->report("c_str_len(GUID) failed ", __LINE__);
                bSuccess = false;
            }

            return bSuccess;
        }

        ss_bool_t test_comstl_string_access_VARIANT(unittest_reporter *r)
        {
            ss_bool_t   bSuccess = true;

            /* Check VARIANT conversion */
            VARIANT     var1;

            ::VariantInit(&var1);
            var1.vt     =   VT_I4;
            var1.lVal   =   1968;

            if(0 != wcscmp(L"1968", c_str_ptr(var1)))
            {
                r->report("c_str_ptr(VARIANT) failed ", __LINE__);
                bSuccess = false;
            }

            if(0 != wcscmp(L"1968", c_str_ptr_null(var1)))
            {
                r->report("c_str_ptr_null(VARIANT) failed ", __LINE__);
                bSuccess = false;
            }

            VARIANT     var2;

            ::VariantInit(&var1);

            if(0 != wcscmp(L"", c_str_ptr(var2)))
            {
                r->report("c_str_ptr(VARIANT) failed ", __LINE__);
                bSuccess = false;
            }

            if(NULL != c_str_ptr_null(var2))
            {
                r->report("c_str_ptr_null(VARIANT) failed ", __LINE__);
                bSuccess = false;
            }

            return bSuccess;
        }

    } // anonymous namespace

    ss_bool_t test_comstl_string_access(unittest_reporter *r)
    {
        using stlsoft::unittest::unittest_initialiser;

        ss_bool_t               bSuccess    =   true;

        unittest_initialiser    init(r, "COMSTL", "string_access", __FILE__);

        // 1. Test GUID
        if(bSuccess)
        {
            bSuccess = test_comstl_string_access_GUID(r);
        }

        // 1. Test CWindow
        if(bSuccess)
        {
            bSuccess = test_comstl_string_access_VARIANT(r);
        }

        return bSuccess;
    }

    unittest_registrar    unittest_comstl_string_access(test_comstl_string_access);

} // namespace unittest

#endif /* STLSOFT_UNITTEST */

/* ////////////////////////////////////////////////////////////////////////// */

/// @} // end of group comstl_string_access_shims

/* ////////////////////////////////////////////////////////////////////////// */

#ifndef _COMSTL_NO_NAMESPACE
# if defined(_STLSOFT_NO_NAMESPACE) || \
     defined(__STLSOFT_DOCUMENTATION_SKIP_SECTION)
} // namespace comstl
# else
} // namespace stlsoft::comstl_project
} // namespace stlsoft
# endif /* _STLSOFT_NO_NAMESPACE */
#endif /* !_COMSTL_NO_NAMESPACE */

/* /////////////////////////////////////////////////////////////////////////////
 * Namespace
 *
 * The string access shims exist either in the stlsoft namespace, or in the
 * global namespace. This is required by the lookup rules.
 *
 */

#ifndef _COMSTL_NO_NAMESPACE
# if !defined(_STLSOFT_NO_NAMESPACE) && \
     !defined(__STLSOFT_DOCUMENTATION_SKIP_SECTION)
namespace stlsoft
{
# else /* ? _STLSOFT_NO_NAMESPACE */
/* There is no stlsoft namespace, so must define in the global namespace */
# endif /* !_STLSOFT_NO_NAMESPACE */

using ::comstl::c_str_ptr_null_a;
using ::comstl::c_str_ptr_null_w;
using ::comstl::c_str_ptr_null_o;
using ::comstl::c_str_ptr_null;

using ::comstl::c_str_ptr_a;
using ::comstl::c_str_ptr_w;
using ::comstl::c_str_ptr_o;
using ::comstl::c_str_ptr;

using ::comstl::c_str_len_a;
using ::comstl::c_str_len_w;
using ::comstl::c_str_len_o;
using ::comstl::c_str_len;

using ::comstl::c_str_size_a;
using ::comstl::c_str_size_w;
using ::comstl::c_str_size_o;
using ::comstl::c_str_size;

# if !defined(_STLSOFT_NO_NAMESPACE) && \
     !defined(__STLSOFT_DOCUMENTATION_SKIP_SECTION)
} // namespace stlsoft
# else /* ? _STLSOFT_NO_NAMESPACE */
/* There is no stlsoft namespace, so must define in the global namespace */
# endif /* !_STLSOFT_NO_NAMESPACE */
#endif /* !_COMSTL_NO_NAMESPACE */

/* ////////////////////////////////////////////////////////////////////////// */

#endif /* !COMSTL_INCL_H_COMSTL_STRING_ACCESS */

/* ////////////////////////////////////////////////////////////////////////// */
