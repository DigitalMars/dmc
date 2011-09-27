/* ////////////////////////////////////////////////////////////////////////////
 * File:        winstl_resource_string.h (Formerly MWResStr.h: ::SynesisWin)
 *
 * Purpose:     basic_resource_string class.
 *
 * Created:     1st November 1994
 * Updated:     11th September 2004
 *
 * Home:        http://stlsoft.org/
 *
 * Copyright (c) 1994-2004, Matthew Wilson and Synesis Software
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


/// \file winstl_resource_string.h
///
/// basic_resource_string class.

#ifndef WINSTL_INCL_H_WINSTL_RESOURCE_STRING
#define WINSTL_INCL_H_WINSTL_RESOURCE_STRING

#ifndef __STLSOFT_DOCUMENTATION_SKIP_SECTION
# define WINSTL_VER_H_WINSTL_RESOURCE_STRING_MAJOR      2
# define WINSTL_VER_H_WINSTL_RESOURCE_STRING_MINOR      0
# define WINSTL_VER_H_WINSTL_RESOURCE_STRING_REVISION   1
# define WINSTL_VER_H_WINSTL_RESOURCE_STRING_EDIT       41
#endif /* !__STLSOFT_DOCUMENTATION_SKIP_SECTION */

/* /////////////////////////////////////////////////////////////////////////////
 * Compatibility
 */

/*
[Incompatibilies-start]
__STLSOFT_COMPILER_IS_MSVC: _MSC_VER<1200
[Incompatibilies-end]
 */

/* /////////////////////////////////////////////////////////////////////////////
 * Includes
 */

#ifndef WINSTL_INCL_H_WINSTL
# include "winstl.h"                // Include the WinSTL root header
#endif /* !WINSTL_INCL_H_WINSTL */

#if defined(__STLSOFT_COMPILER_IS_MSVC) && \
    _MSC_VER < 1200
# error winstl_resource_string.h is not compatible with Visual C++ 5.0 or earlier
#endif /* _MSC_VER < 1200 */

#ifndef STLSOFT_INCL_H_STLSOFT_STRING_TRAITS
# include "stlsoft_string_traits.h" // stlsoft::string_traits
#endif /* !STLSOFT_INCL_H_STLSOFT_STRING_TRAITS */
#ifndef STLSOFT_INCL_H_STLSOFT_EXCEPTIONS
# include "stlsoft_exceptions.h"    // stlsoft::null_exception_policy
#endif /* !STLSOFT_INCL_H_STLSOFT_EXCEPTIONS */
#ifdef STLSOFT_UNITTEST
# include <string>                  // std::string, std::wstring
#endif /* STLSOFT_UNITTEST */
#include <exception>

/* /////////////////////////////////////////////////////////////////////////////
 * Namespace
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

/// \weakgroup libraries STLSoft Libraries
/// \brief The individual libraries

/// \weakgroup libraries_string String Library
/// \ingroup libraries
/// \brief This library provides facilities for defining and manipulating strings

/// \defgroup winstl_string_library String Library (WinSTL)
/// \ingroup WinSTL libraries_string
/// \brief This library provides facilities for defining and manipulating Win32 string types
/// @{

/* /////////////////////////////////////////////////////////////////////////////
 * Classes
 */

// class basic_resource_string
/// Instances of this class represent Windows string resources, and are
/// constructed from instance identifiers.
///
/// \param S The string class, e.g. std::string, stlsoft::simple_string, etc.
/// \param X The exception class
template<   ss_typename_param_k S
#ifdef __STLSOFT_CF_TEMPLATE_CLASS_DEFAULT_CLASS_ARGUMENT_SUPPORT
        ,   ss_typename_param_k X = stlsoft_ns_qual(null_exception_policy)
#else /* ? __STLSOFT_CF_TEMPLATE_CLASS_DEFAULT_CLASS_ARGUMENT_SUPPORT */
        ,   ss_typename_param_k X /* = stlsoft_ns_qual(null_exception_policy) */
#endif /* __STLSOFT_CF_TEMPLATE_CLASS_DEFAULT_CLASS_ARGUMENT_SUPPORT */
        >
class basic_resource_string
    : public S
    , protected X
{
private:
    typedef S                                          parent_class_type;
//    typedef C                                          value_type;
public:
    typedef S                                          string_type;
    typedef basic_resource_string<S, X>                class_type;
    typedef X                                          exception_type;
    typedef stlsoft_ns_qual(string_traits)<S>          string_traits_type;

    typedef ss_typename_type_k string_type::value_type value_type;

/// \name Construction
/// @{
public:
    /// Constructs an around the string loaded from the given \c id
    ss_explicit_k basic_resource_string(ws_int_t id) winstl_throw_1(ss_typename_type_k exception_type::thrown_type)
    {
        load(::GetModuleHandle(NULL), id);
    }

    /// Constructs an around the string loaded from the given \c id and \c hinst
    basic_resource_string(HINSTANCE hinst, ws_int_t id) winstl_throw_1(ss_typename_type_k exception_type::thrown_type)
    {
        load(hinst, id);
    }

    /// Copy constructor
    basic_resource_string(const basic_resource_string &rhs)
        : parent_class_type(rhs)
    {}

    /// Copy constructor
    basic_resource_string(const string_type &rhs)
        : parent_class_type(rhs)
    {}

    /// Copy assignment operator
    const basic_resource_string &operator =(const basic_resource_string &rhs)
    {
        parent_class_type::operator =(rhs);

        return *this;
    }
    /// Copy assignment operator
    const basic_resource_string &operator =(const string_type &rhs)
    {
        parent_class_type::operator =(rhs);

        return *this;
    }

/// @}

// Implementation
private:
    ws_int_t LoadString(HINSTANCE hinst, int uID, ws_char_a_t *buffer, ws_size_t cchBuffer)
    {
        return ::LoadStringA(hinst, uID, buffer, cchBuffer);
    }
    ws_int_t LoadString(HINSTANCE hinst, int uID, ws_char_w_t *buffer, ws_size_t cchBuffer)
    {
        return ::LoadStringW(hinst, uID, buffer, cchBuffer);
    }

    void load(HINSTANCE hinst, ws_int_t id) winstl_throw_1(ss_typename_type_k exception_type::thrown_type)
    {
        // TODO: Verify that it's not possible to load string resources of >256. If that's
        // wrong, then need to fix this to use auto_buffer
        value_type  sz[1024];

        if(0 == LoadString(hinst, id, sz, winstl_num_elements(sz)))
        {
            exception_type()(hinst, id);

#if 0
            parent_class_type::operator =(string_traits_type::empty_string());
#else
            parent_class_type::operator =(string_type());
#endif /* 0 */
        }
        else
        {
            parent_class_type::operator =(sz);
        }
    }
};

///////////////////////////////////////////////////////////////////////////////
// Unit-testing

#ifdef STLSOFT_UNITTEST

namespace unittest
{
    namespace
    {
        struct throw_exception
        {
        public:
            /// The exception type
            class thrown_type
                : public std::exception
            {
            public:
                thrown_type(char const *reason) throw()
                    : m_reason(reason)
                {}

                ~thrown_type() throw()
                {}

                char const *what() const throw()
                {
                    return m_reason.c_str();
                }

            private:
                stlsoft_ns_qual_std(string) m_reason;
            };

        public:
            void operator ()(HINSTANCE hinst, int id)
            {
                char    message[101];

                wsprintfA(message, "Failed to load resource %d from module 0x%084", id, hinst);

                throw thrown_type(message);
            }
        };
    }


    ss_bool_t test_mfcstl_resource_string(unittest_reporter *r)
    {
        using stlsoft::unittest::unittest_initialiser;
        using stlsoft::null_exception_policy;

        ss_bool_t               bSuccess    =   true;

        unittest_initialiser    init(r, "WinSTL", "resource_string", __FILE__);

        typedef basic_resource_string<stlsoft_ns_qual_std(string), null_exception_policy>  rsn_a_t;
        typedef basic_resource_string<stlsoft_ns_qual_std(wstring), null_exception_policy> rsn_w_t;

        HINSTANCE   hinst   =   ::LoadLibrary("USER32");
        int         iBad    =   -1;
        int         iGood   =   -1;

        for(int i = 0; i < 10000; ++i)
        {
            rsn_a_t     rsn_a_2(hinst, i);
            rsn_w_t     rsn_w_2(hinst, i);

            if(0 == rsn_a_2.length())
            {
                if(-1 == iBad)
                {
                    iBad = i;
                }
            }
            else
            {
                if(-1 == iGood)
                {
                    iGood = i;
                }
            }

            if(rsn_a_2.length() != rsn_w_2.length())
            {
                r->report("resource string (null exception) length mismatch between ANSI and Unicode instantiations", __LINE__);
                bSuccess = false;
            }
        }

        if(bSuccess)
        {
            typedef basic_resource_string<stlsoft_ns_qual_std(string), throw_exception>     rsx_a_t;
            typedef basic_resource_string<stlsoft_ns_qual_std(wstring), throw_exception>    rsx_w_t;

            if(-1 != iGood)
            {
                try
                {
                    rsx_a_t     rsn_a_2(hinst, iGood);
                    rsx_w_t     rsn_w_2(hinst, iGood);
                }
                catch(std::exception &/* x */)
                {
                    r->report("resource string (std::exception) failed to load known resource from USER32", __LINE__);
                    bSuccess = false;
                }
            }

            if(-1 != iBad)
            {
                try
                {
                    rsx_a_t     rsn_a_2(hinst, iBad);

                    r->report("resource string (std::exception) loaded known invalid resource from USER32", __LINE__);
                    bSuccess = false;
                }
                catch(std::exception &/* x */)
                {}

                try
                {
                    rsx_w_t     rsn_w_2(hinst, iBad);

                    r->report("resource string (std::exception) loaded known invalid resource from USER32", __LINE__);
                    bSuccess = false;
                }
                catch(std::exception &/* x */)
                {}
            }
        }

        return bSuccess;
    }

    unittest_registrar    unittest_mfcstl_resource_string(test_mfcstl_resource_string);

} // namespace unittest

#endif /* STLSOFT_UNITTEST */

/* /////////////////////////////////////////////////////////////////////////////
 * String access shims
 */

#if 0
/* c_str_ptr_null */

/// \brief Returns the corresponding C-string pointer of \c s, or a null pointer
template<   ss_typename_param_k S
        ,   ss_typename_param_k X
        >
inline C const *c_str_ptr_null(basic_resource_string<S, X> const &s)
{
    return (s.length() == 0) ? 0 : s.c_str();
}

/* c_str_ptr */

/// \brief Returns the corresponding C-string pointer of \c s
template<   ss_typename_param_k S
        ,   ss_typename_param_k X
        >
inline C const *c_str_ptr(basic_resource_string<S, X> const &s)
{
    return s.c_str();
}
#endif /* 0 */

/* c_str_ptr_len */

/// \brief Returns the length (in characters) of \c s, <b><i>not</i></b> including the null-terminating character
template<   ss_typename_param_k S
        ,   ss_typename_param_k X
        >
inline ss_size_t c_str_len(basic_resource_string<S, X> const &s)
{
    return s.length();
}

/* c_str_ptr_size */

/// \brief Returns the size (in bytes) of the contents of \c s, <b><i>not</i></b> including the null-terminating character
template<   ss_typename_param_k S
        ,   ss_typename_param_k X
        >
inline ss_size_t c_str_size(basic_resource_string<S, X> const &s)
{
    return c_str_len(s) * sizeof(ss_typename_type_k basic_resource_string<S, X>::value_type);
}

/* ////////////////////////////////////////////////////////////////////////// */

/// @} // end of group winstl_string_library

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

/* ////////////////////////////////////////////////////////////////////////// */

#endif /* !WINSTL_INCL_H_WINSTL_RESOURCE_STRING */

/* ////////////////////////////////////////////////////////////////////////// */
