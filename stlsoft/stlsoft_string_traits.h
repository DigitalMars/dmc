/* /////////////////////////////////////////////////////////////////////////////
 * File:        stlsoft_string_traits.h
 *
 * Purpose:     Contains string_traits.
 *
 * Created:     16th January 2002
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


/// \file stlsoft_string_traits.h
///
/// Contains string_traits.

#ifndef STLSOFT_INCL_H_STLSOFT_STRING_TRAITS
#define STLSOFT_INCL_H_STLSOFT_STRING_TRAITS

#ifndef __STLSOFT_DOCUMENTATION_SKIP_SECTION
# define STLSOFT_VER_H_STLSOFT_STRING_TRAITS_MAJOR      2
# define STLSOFT_VER_H_STLSOFT_STRING_TRAITS_MINOR      0
# define STLSOFT_VER_H_STLSOFT_STRING_TRAITS_REVISION   1
# define STLSOFT_VER_H_STLSOFT_STRING_TRAITS_EDIT       40
#endif /* !__STLSOFT_DOCUMENTATION_SKIP_SECTION */

/* /////////////////////////////////////////////////////////////////////////////
 * Includes
 */

#ifndef STLSOFT_INCL_H_STLSOFT
# include "stlsoft.h"               // Include the STLSoft root header
#endif /* !STLSOFT_INCL_H_STLSOFT */
#ifndef STLSOFT_INCL_H_STLSOFT_CHAR_TRAITS
# include "stlsoft_char_traits.h"  // stlsoft::char_traits
#endif /* !STLSOFT_INCL_H_STLSOFT_CHAR_TRAITS */
#ifndef _STLSOFT_STRING_TRAITS_NO_STD_STRING
# include <string>                  // std::string classes
#endif /* _STLSOFT_STRING_TRAITS_NO_STD_STRING */

/* /////////////////////////////////////////////////////////////////////////////
 * Namespace
 */

#ifndef _STLSOFT_NO_NAMESPACE
namespace stlsoft
{
#endif /* _STLSOFT_NO_NAMESPACE */

/* ////////////////////////////////////////////////////////////////////////// */

/// \weakgroup libraries STLSoft Libraries
/// \brief The individual libraries

/// \weakgroup libraries_string String Library
/// \ingroup libraries
/// \brief This library provides facilities for defining and manipulating strings

/// \weakgroup stlsoft_string_library String Library (STLSoft)
/// \ingroup STLSoft libraries_string
/// \brief This library provides facilities for defining and manipulating strings
/// @{

/* /////////////////////////////////////////////////////////////////////////////
 * Classes
 */

#ifdef __STLSOFT_DOCUMENTATION_SKIP_SECTION
/// Traits for determining the characteristics of strings
///
/// string_traits is a traits class for determining various characteristics of
/// strings.
///
/// \param C The char type
template <class C>
struct string_traits
{
    typedef C               value_type;     //!< The value type
    typedef C               char_type;      //!< The char type
    typedef ss_size_t       size_type;      //!< The size type
    typedef ???             string_type;    //!< The string type
    enum
    {
            is_pointer                      //!< non-zero if C is a pointer type
        ,   is_pointer_to_const             //!< non-zero if C is a pointer-to-const type
        ,   char_type_size                  //!< The size of \c char_type
    };

    static string_type empty_string();
};
#else
template <ss_typename_param_k T>
struct string_traits;

/* C-style ANSI string */
STLSOFT_TEMPLATE_SPECIALISATION
struct string_traits<ss_char_a_t *>
{
    typedef ss_char_a_t             value_type;
    typedef ss_char_a_t             char_type;
    typedef ss_size_t               size_type;
    typedef char_type const         const_char_type;
    typedef value_type const        *string_type;
    enum
    {
            is_pointer          =   true
        ,   is_pointer_to_const =   false
        ,   char_type_size      =   sizeof(char_type)
    };

    static string_type empty_string()
    {
        // This character array is initialised to 0, which conveniently happens to
        // be the empty string, by the module/application load, so it is
        // guaranteed to be valid, and there are no threading/race conditions
        static char_type    s_empty[1];

        stlsoft_assert(s_empty[0] == '\0'); // Paranoid check

        return s_empty;
    }
};

STLSOFT_TEMPLATE_SPECIALISATION
struct string_traits<ss_char_a_t const *>
{
    typedef ss_char_a_t             value_type;
    typedef ss_char_a_t             char_type;
    typedef ss_size_t               size_type;
    typedef char_type const         const_char_type;
    typedef value_type const        *string_type;
    enum
    {
            is_pointer          =   true
        ,   is_pointer_to_const =   false
        ,   char_type_size      =   sizeof(char_type)
    };

    static string_type empty_string()
    {
        // This character array is initialised to 0, which conveniently happens to
        // be the empty string, by the module/application load, so it is
        // guaranteed to be valid, and there are no threading/race conditions
        static char_type    s_empty[1];

        stlsoft_assert(s_empty[0] == '\0'); // Paranoid check

        return s_empty;
    }
};

/* C-style Unicode string */
STLSOFT_TEMPLATE_SPECIALISATION
struct string_traits<ss_char_w_t *>
{
    typedef ss_char_w_t             value_type;
    typedef ss_char_w_t             char_type;
    typedef ss_size_t               size_type;
    typedef char_type const         const_char_type;
    typedef value_type const        *string_type;
    enum
    {
            is_pointer          =   true
        ,   is_pointer_to_const =   false
        ,   char_type_size      =   sizeof(char_type)
    };

    static string_type empty_string()
    {
        // This character array is initialised to 0, which conveniently happens to
        // be the empty string, by the module/application load, so it is
        // guaranteed to be valid, and there are no threading/race conditions
        static char_type    s_empty[1];

        stlsoft_assert(s_empty[0] == '\0'); // Paranoid check

        return s_empty;
    }
};

STLSOFT_TEMPLATE_SPECIALISATION
struct string_traits<ss_char_w_t const *>
{
    typedef ss_char_w_t             value_type;
    typedef ss_char_w_t             char_type;
    typedef ss_size_t               size_type;
    typedef char_type const         const_char_type;
    typedef value_type const        *string_type;
    enum
    {
            is_pointer          =   true
        ,   is_pointer_to_const =   false
        ,   char_type_size      =   sizeof(char_type)
    };

    static string_type empty_string()
    {
        // This character array is initialised to 0, which conveniently happens to
        // be the empty string, by the module/application load, so it is
        // guaranteed to be valid, and there are no threading/race conditions
        static char_type    s_empty[1];

        stlsoft_assert(s_empty[0] == '\0'); // Paranoid check

        return s_empty;
    }
};

/* std::basic_string */
#ifndef _STLSOFT_STRING_TRAITS_NO_STD_STRING
# ifdef __STLSOFT_CF_TEMPLATE_PARTIAL_SPECIALISATION_SUPPORT
template <ss_typename_param_k C>
struct string_traits<stlsoft_ns_qual_std(basic_string)<C> >
{
    typedef stlsoft_ns_qual_std(basic_string)<C>        value_type;
    typedef ss_typename_type_k value_type::value_type   char_type;
    typedef ss_typename_type_k value_type::size_type    size_type;
    typedef char_type const                             const_char_type;
    typedef value_type                                  string_type;
    enum
    {
            is_pointer          =   false
        ,   is_pointer_to_const =   false
        ,   char_type_size      =   sizeof(char_type)
    };

    static string_type empty_string()   { return string_type(); }
};
# else /* ? __STLSOFT_CF_TEMPLATE_PARTIAL_SPECIALISATION_SUPPORT */
STLSOFT_TEMPLATE_SPECIALISATION
struct string_traits<stlsoft_ns_qual_std(basic_string)<ss_char_a_t> >
{
    typedef stlsoft_ns_qual_std(basic_string)<ss_char_a_t>  value_type;
    typedef value_type::value_type                          char_type;
    typedef value_type::size_type                           size_type;
    typedef char_type const                                 const_char_type;
    typedef value_type                                      string_type;
    enum
    {
            is_pointer          =   false
        ,   is_pointer_to_const =   false
        ,   char_type_size      =   sizeof(char_type)
    };

    static string_type empty_string()   { return string_type(); }
};

STLSOFT_TEMPLATE_SPECIALISATION
struct string_traits<stlsoft_ns_qual_std(basic_string)<ss_char_w_t> >
{
    typedef stlsoft_ns_qual_std(basic_string)<ss_char_w_t>  value_type;
    typedef value_type::value_type                          char_type;
    typedef value_type::size_type                           size_type;
    typedef char_type const                                 const_char_type;
    typedef value_type                                      string_type;
    enum
    {
            is_pointer          =   false
        ,   is_pointer_to_const =   false
        ,   char_type_size      =   sizeof(char_type)
    };

    static string_type empty_string()   { return string_type(); }
};
# endif /* __STLSOFT_CF_TEMPLATE_PARTIAL_SPECIALISATION_SUPPORT */
#endif /* _STLSOFT_STRING_TRAITS_NO_STD_STRING */

#endif /* !__STLSOFT_DOCUMENTATION_SKIP_SECTION */

////////////////////////////////////////////////////////////////////////////////
// Unit-testing

#ifdef STLSOFT_UNITTEST

namespace unittest
{
    ss_bool_t test_stlsoft_string_traits(unittest_reporter *r)
    {
        ss_bool_t               bSuccess    =   true;

        unittest_initialiser    init(r, "STLSoft", "string_traits", __FILE__);

#if 0
        if(<<VOID>>)
        {
            r->report("<<VOID>> failed ", __LINE__);
            bSuccess = false;
        }
#endif /* 0 */

        return bSuccess;
    }

    unittest_registrar    unittest_stlsoft_string_traits(test_stlsoft_string_traits);

} // namespace unittest

#endif /* STLSOFT_UNITTEST */

/* ////////////////////////////////////////////////////////////////////////// */

/// @} // end of group stlsoft_string_library

/* ////////////////////////////////////////////////////////////////////////// */

#ifndef _STLSOFT_NO_NAMESPACE
} // namespace stlsoft
#endif /* _STLSOFT_NO_NAMESPACE */

/* ////////////////////////////////////////////////////////////////////////// */

#endif /* !STLSOFT_INCL_H_STLSOFT_STRING_TRAITS */

/* ////////////////////////////////////////////////////////////////////////// */
