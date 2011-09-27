/* /////////////////////////////////////////////////////////////////////////////
 * File:        winstl_char_conversions.h (originally MLStrCnv.h, ::SynesisStd)
 *
 * Purpose:     Type conversions for Windows.
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


/// \file winstl_char_conversions.h
///
/// Type conversions for Windows.

#ifndef WINSTL_INCL_H_WINSTL_CHAR_CONVERSIONS
#define WINSTL_INCL_H_WINSTL_CHAR_CONVERSIONS

#ifndef __STLSOFT_DOCUMENTATION_SKIP_SECTION
# define WINSTL_VER_H_WINSTL_CHAR_CONVERSIONS_MAJOR       2
# define WINSTL_VER_H_WINSTL_CHAR_CONVERSIONS_MINOR       0
# define WINSTL_VER_H_WINSTL_CHAR_CONVERSIONS_REVISION    1
# define WINSTL_VER_H_WINSTL_CHAR_CONVERSIONS_EDIT        34
#endif /* !__STLSOFT_DOCUMENTATION_SKIP_SECTION */

/* /////////////////////////////////////////////////////////////////////////////
 * Compatibility
 */

/*
[Incompatibilies-start]
__STLSOFT_COMPILER_IS_GCC: __GNUC__<3
__STLSOFT_COMPILER_IS_MSVC: _MSC_VER<1200
[Incompatibilies-end]
 */

/* /////////////////////////////////////////////////////////////////////////////
 * Includes
 */

#ifndef WINSTL_INCL_H_WINSTL
# include "winstl.h"                        // Include the WinSTL root header
#endif /* !WINSTL_INCL_H_WINSTL */

#if defined(__STLSOFT_COMPILER_IS_GCC) && \
    __GNUC__ < 3
# error winstl_char_conversions.h is not compatible with GNU C++ prior to 3.0
#endif /* compiler */
#if defined(__STLSOFT_COMPILER_IS_MSVC) && \
    _MSC_VER < 1100
# error winstl_char_conversions.h is not compatible with Visual C++ 5.0 or earlier
#endif /* _MSC_VER < 1200 */


#ifndef WINSTL_INCL_H_WINSTL_PROCESSHEAP_ALLOCATOR
# include "winstl_processheap_allocator.h"  // winstl::processheap_allocator
#endif /* !WINSTL_INCL_H_WINSTL_PROCESSHEAP_ALLOCATOR */
#ifndef STLSOFT_INCL_H_STLSOFT_STRING_ACCESS
# include "stlsoft_string_access.h"         // stlsoft::c_str_ptr
#endif /* !STLSOFT_INCL_H_STLSOFT_STRING_ACCESS */
#ifndef WINSTL_INCL_H_WINSTL_STRING_ACCESS
# include "winstl_string_access.h"          // winstl::c_str_ptr
#endif /* !WINSTL_INCL_H_WINSTL_STRING_ACCESS */
#ifndef STLSOFT_INCL_H_STLSOFT_AUTO_BUFFER
# include "stlsoft_auto_buffer.h"           // stlsoft::auto_buffer
#endif /* !STLSOFT_INCL_H_STLSOFT_AUTO_BUFFER */
#ifdef STLSOFT_UNITTEST
# include <wchar.h>
#endif /* STLSOFT_UNITTEST */

/* ////////////////////////////////////////////////////////////////////////// */

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

#if !defined(__STLSOFT_COMPILER_IS_MWERKS)
stlsoft_ns_using(c_str_ptr)
stlsoft_ns_using(c_str_len)
#endif /* compiler */

/* /////////////////////////////////////////////////////////////////////////////
 * Classes
 */

/// Converts an ANST (\c char-based) string to a Unicode (\c whar_t-based) string
template <ws_size_t CCH>
class ansi2Unicode
    : public auto_buffer<ws_char_w_t, processheap_allocator<ws_char_w_t>, CCH>
{
private:
    typedef auto_buffer<ws_char_w_t, processheap_allocator<ws_char_w_t>, CCH>   parent_class_type;
public:
    typedef ws_char_w_t                                                         char_type;
    typedef ss_typename_type_k parent_class_type::size_type                     size_type;
    typedef ss_typename_type_k parent_class_type::pointer                       pointer;

public:
#ifdef __STLSOFT_CF_MEMBER_TEMPLATE_CTOR_SUPPORT
    template <ss_typename_param_k S>
    ss_explicit_k ansi2Unicode(S const &s)
#else
    ss_explicit_k ansi2Unicode(ws_char_a_t const *s)
#endif // __STLSOFT_CF_MEMBER_TEMPLATE_CTOR_SUPPORT
        : parent_class_type(stlsoft_ns_qual(c_str_len)(s) + 1)
    {
        prepare_(s);
    }

#ifdef __STLSOFT_CF_MEMBER_TEMPLATE_CTOR_SUPPORT
    template <ss_typename_param_k S>
    ansi2Unicode(S const &s, size_type cch)
#else
    ansi2Unicode(ws_char_a_t const *s, size_type cch)
#endif // __STLSOFT_CF_MEMBER_TEMPLATE_CTOR_SUPPORT
        : parent_class_type(cch + 1)
    {
        prepare_(s);
    }

// Implementation
private:
#ifdef __STLSOFT_CF_MEMBER_TEMPLATE_CTOR_SUPPORT
    template <ss_typename_param_k S>
    void prepare_(S const &s)
#else
    void prepare_(ws_char_a_t const *s)
#endif // __STLSOFT_CF_MEMBER_TEMPLATE_CTOR_SUPPORT
    {
        // If the auto_buffer failed to allocate the required memory, and
        // we're not in an exception-environment, then size() will be zero
        if(0 == parent_class_type::size())
        {
            // Since we know that auto_buffer's parameterising size must
            // always be greater that 0, then
            parent_class_type::data()[0] = '\0';
        }
        else
        {
            size_type   size    =   parent_class_type::size();
            pointer     p       =   parent_class_type::data();

            // Note: cannot use -1 for length, since s might be a type that
            // could change length
            p[(0 == ::MultiByteToWideChar(0, 0, stlsoft_ns_qual(c_str_ptr)(s), static_cast<int>(size), p, static_cast<int>(size))) ? 0 : size - 1] = '\0';
        }
    }

/// \name Accessors
/// @{
public:
    char_type const *c_str() const
    {
        return parent_class_type::data();
    }
/// @}

// Not to be implemented
private:
    ansi2Unicode(ansi2Unicode const &);
    ansi2Unicode &operator =(ansi2Unicode const &);
};

/// Converts a Unicode (\c whar_t-based) string to an ANST (\c char-based) string
template <ws_size_t CCH>
class unicode2Ansi
    : public auto_buffer<ws_char_a_t, processheap_allocator<ws_char_a_t>, CCH>
{
private:
    typedef auto_buffer<ws_char_a_t, processheap_allocator<ws_char_a_t>, CCH>   parent_class_type;
public:
    typedef ws_char_a_t                                                         char_type;
    typedef ss_typename_type_k parent_class_type::size_type                     size_type;
    typedef ss_typename_type_k parent_class_type::pointer                       pointer;

public:
#ifdef __STLSOFT_CF_MEMBER_TEMPLATE_CTOR_SUPPORT
    template <ss_typename_param_k S>
    ss_explicit_k unicode2Ansi(S const &s)
#else
    ss_explicit_k unicode2Ansi(ws_char_w_t const *s)
#endif // __STLSOFT_CF_MEMBER_TEMPLATE_CTOR_SUPPORT
        : parent_class_type(stlsoft_ns_qual(c_str_len)(s) + 1)
    {
        prepare_(s);
    }

#ifdef __STLSOFT_CF_MEMBER_TEMPLATE_CTOR_SUPPORT
    template <ss_typename_param_k S>
    ss_explicit_k unicode2Ansi(S const &s, size_type cch)
#else
    ss_explicit_k unicode2Ansi(ws_char_w_t const *s, size_type cch)
#endif // __STLSOFT_CF_MEMBER_TEMPLATE_CTOR_SUPPORT
        : parent_class_type(cch + 1)
    {
        prepare_(s);
    }

// Implementation
private:
#ifdef __STLSOFT_CF_MEMBER_TEMPLATE_CTOR_SUPPORT
    template <ss_typename_param_k S>
    void prepare_(S const &s)
#else
    void prepare_(ws_char_a_t const *s)
#endif // __STLSOFT_CF_MEMBER_TEMPLATE_CTOR_SUPPORT
    {
        // If the auto_buffer failed to allocate the required memory, and
        // we're not in an exception-environment, then size() will be zero
        if(0 == parent_class_type::size())
        {
            // Since we know that auto_buffer's parameterising size must
            // always be greater that 0, then
            parent_class_type::data()[0] = '\0';
        }
        else
        {
            size_type   size    =   parent_class_type::size();
            pointer     p       =   parent_class_type::data();

            // Note: cannot use -1 for length, since s might be a type that
            // could change length
            p[(0 == ::WideCharToMultiByte(0, 0, stlsoft_ns_qual(c_str_ptr)(s), static_cast<int>(size), p, static_cast<int>(size), NULL, NULL)) ? 0 : size - 1] = '\0';
        }
    }

/// \name Accessors
/// @{
public:
    char_type const *c_str() const
    {
        return parent_class_type::data();
    }
/// @}

// Not to be implemented
private:
    unicode2Ansi(unicode2Ansi const &);
    unicode2Ansi &operator =(unicode2Ansi const &);
};

/* ////////////////////////////////////////////////////////////////////////// */

typedef ansi2Unicode<256>     a2w;
typedef unicode2Ansi<256>     w2a;

/* /////////////////////////////////////////////////////////////////////////////
 * Shims
 */

template<   ws_size_t           CCH
        >
inline ws_char_w_t const *c_str_ptr_null(ansi2Unicode<CCH> const &b)
{
    return stlsoft_ns_qual(c_str_ptr_null)(b.c_str());
}

template<   ws_size_t           CCH
        >
inline ws_char_w_t const *c_str_ptr(ansi2Unicode<CCH> const &b)
{
    return stlsoft_ns_qual(c_str_ptr)(b.c_str());
}

template<   ws_size_t           CCH
        >
inline ws_size_t c_str_len(ansi2Unicode<CCH> const &b)
{
    return stlsoft_ns_qual(c_str_len)(b.c_str());
}

template<   ws_size_t           CCH
        >
inline ws_size_t c_str_size(ansi2Unicode<CCH> const &b)
{
    return stlsoft_ns_qual(c_str_size)(b.c_str());
}

template<   ss_typename_param_k S
        ,   ws_size_t           CCH
        >
inline S &operator <<(S & s, ansi2Unicode<CCH> const &b)
{
    s << b.c_str();

    return s;
}

template<   ws_size_t           CCH
        >
inline ws_char_a_t const *c_str_ptr_null(unicode2Ansi<CCH> const &b)
{
    return stlsoft_ns_qual(c_str_ptr_null)(b.c_str());
}

template<   ws_size_t           CCH
        >
inline ws_char_a_t const *c_str_ptr(unicode2Ansi<CCH> const &b)
{
    return stlsoft_ns_qual(c_str_ptr)(b.c_str());
}

template<   ws_size_t           CCH
        >
inline ws_size_t c_str_len(unicode2Ansi<CCH> const &b)
{
    return stlsoft_ns_qual(c_str_len)(b.c_str());
}

template<   ws_size_t           CCH
        >
inline ws_size_t c_str_size(unicode2Ansi<CCH> const &b)
{
    return stlsoft_ns_qual(c_str_size)(b.c_str());
}

template<   ss_typename_param_k S
        ,   ws_size_t           CCH
        >
inline S &operator <<(S & s, unicode2Ansi<CCH> const &b)
{
    s << b.c_str();

    return s;
}

////////////////////////////////////////////////////////////////////////////////
// Unit-testing

#ifdef STLSOFT_UNITTEST

namespace unittest
{
    ss_bool_t test_winstl_char_conversions(unittest_reporter *r)
    {
        using stlsoft::unittest::unittest_initialiser;

        ss_bool_t               bSuccess    =   true;

        unittest_initialiser    init(r, "WinSTL", "char_conversion", __FILE__);

        if(0 != strcmp("String #1", w2a(L"String #1")))
        {
            r->report("w2a failed ", __LINE__);
            bSuccess = false;
        }

        if(0 != wcscmp(L"String #1", a2w("String #1")))
        {
            r->report("a2w failed ", __LINE__);
            bSuccess = false;
        }

        return bSuccess;
    }

    unittest_registrar    unittest_winstl_char_conversions(test_winstl_char_conversions);

} // namespace unittest

#endif /* STLSOFT_UNITTEST */

/* ////////////////////////////////////////////////////////////////////////// */

#ifndef _WINSTL_NO_NAMESPACE
# if defined(_STLSOFT_NO_NAMESPACE) || \
     defined(__STLSOFT_DOCUMENTATION_SKIP_SECTION)
} // namespace winstl
# else
} // namespace winstl_project
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

#endif /* WINSTL_INCL_H_WINSTL_CHAR_CONVERSIONS */

/* ////////////////////////////////////////////////////////////////////////// */
