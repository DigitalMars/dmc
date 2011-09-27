/* /////////////////////////////////////////////////////////////////////////////
 * File:        winstl_functionals.h
 *
 * Purpose:     A number of useful functionals .
 *
 * Created:     19th January 2002
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


/// \file winstl_functionals.h
///
/// A number of useful functionals .

#ifndef WINSTL_INCL_H_WINSTL_FUNCTIONALS
#define WINSTL_INCL_H_WINSTL_FUNCTIONALS

#ifndef __STLSOFT_DOCUMENTATION_SKIP_SECTION
# define WINSTL_VER_H_WINSTL_FUNCTIONALS_MAJOR      2
# define WINSTL_VER_H_WINSTL_FUNCTIONALS_MINOR      0
# define WINSTL_VER_H_WINSTL_FUNCTIONALS_REVISION   1
# define WINSTL_VER_H_WINSTL_FUNCTIONALS_EDIT       45
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
# error winstl_functionals.h is not compatible with Visual C++ 5.0 or earlier
#endif /* _MSC_VER < 1200 */

#ifndef STLSOFT_INCL_H_STLSOFT_STRING_ACCESS
#ifndef WINSTL_INCL_H_WINSTL_FILESYSTEM_TRAITS
# include "winstl_filesystem_traits.h"
#endif /* !WINSTL_INCL_H_WINSTL_FILESYSTEM_TRAITS */
#ifndef WINSTL_INCL_H_WINSTL_FILE_PATH_BUFFER
# include "winstl_file_path_buffer.h"   // basic_file_path_buffer
#endif /* !WINSTL_INCL_H_WINSTL_FILE_PATH_BUFFER */
# include "stlsoft_string_access.h" // c_str_ptr, etc.
#endif /* !STLSOFT_INCL_H_STLSOFT_STRING_ACCESS */
#ifndef WINSTL_INCL_H_WINSTL_STRING_ACCESS
# include "winstl_string_access.h"  // c_str_ptr, etc.
#endif /* !WINSTL_INCL_H_WINSTL_STRING_ACCESS */
#ifndef _WINSTL_FUNCTIONALS_NO_STD
# include <functional>
#endif /* _WINSTL_FUNCTIONALS_NO_STD */

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

#if !defined(__STLSOFT_COMPILER_IS_MWERKS)
stlsoft_ns_using(c_str_ptr)
#endif /* compiler */

/* /////////////////////////////////////////////////////////////////////////////
 * Classes
 */

/// Function object that compares two file-system paths
///
/// \param C The character type
template <ss_typename_param_k C>
struct compare_path
#ifndef _WINSTL_FUNCTIONALS_NO_STD
    : winstl_ns_qual_std(binary_function)<C const *, C const *, ws_bool_t>
#endif /* _WINSTL_FUNCTIONALS_NO_STD */
{
public:
    /// The character type
    typedef C                                                                       char_type;
#ifndef _WINSTL_FUNCTIONALS_NO_STD
private:
    typedef winstl_ns_qual_std(binary_function)<C const *, C const *, ws_bool_t>    parent_class_type;
public:
    /// The first argument type
    typedef ss_typename_type_k parent_class_type::first_argument_type               first_argument_type;
    /// The second argument type
    typedef ss_typename_type_k parent_class_type::second_argument_type              second_argument_type;
    /// The result type
    typedef ss_typename_type_k parent_class_type::result_type                       result_type;
#else
    /// The first argument type
    typedef const char_type                                                         *first_argument_type;
    /// The second argument type
    typedef const char_type                                                         *second_argument_type;
    /// The result type
    typedef ws_bool_t                                                               result_type;
#endif /* _WINSTL_FUNCTIONALS_NO_STD */
    /// The traits type
    typedef filesystem_traits<C>                                                    traits_type;
    /// The current parameterisation of the type
    typedef compare_path<C>                                                         class_type;

public:
    /// Function call, compares \c s1 with \c s2
    ///
    /// \note The comparison is determined by evaluation the full-paths of both \c s1 and \c s2
#ifdef __STLSOFT_CF_MEMBER_TEMPLATE_FUNCTION_SUPPORT
    template <ss_typename_param_k T1, ss_typename_param_k T2>
    result_type operator ()(T1 const &s1, T2 const &s2) const
    {
        return compare_(c_str_ptr(s1), c_str_ptr(s2));
    }
#else
    result_type operator ()(first_argument_type s1, second_argument_type s2) const
    {
        return compare_(s1, s2);
    }
#endif /* __STLSOFT_CF_MEMBER_TEMPLATE_FUNCTION_SUPPORT */

// Implementation
private:
    static result_type compare_(char_type const *s1, char_type const *s2)
    {
        basic_file_path_buffer<char_type>   path1;
        basic_file_path_buffer<char_type>   path2;
        char_type                           *dummy;
        result_type                         result;

        if(!traits_type::get_full_path_name(s1, path1.size(), &path1[0], &dummy))
        {
            result = false;
        }
        else if(!traits_type::get_full_path_name(s2, path2.size(), &path2[0], &dummy))
        {
            result = false;
        }
        else
        {
            traits_type::ensure_dir_end(&path1[0]);
            traits_type::ensure_dir_end(&path2[0]);

            result = traits_type::str_compare_no_case(stlsoft_ns_qual(c_str_ptr)(path1), stlsoft_ns_qual(c_str_ptr)(path2)) == 0;
        }

        return result;
    }
};

/// Predicate that indicates whether a given path exists
///
/// \param C The character type
template<   ss_typename_param_k C
        ,   ss_typename_param_k A = C const *
        >
struct path_exists
    : winstl_ns_qual_std(unary_function)<A, ws_bool_t>
{
public:
    /// The character type
    typedef C                                                                       char_type;
    /// The traits type
    typedef filesystem_traits<C>                                                    traits_type;
    /// The current parameterisation of the type
    typedef path_exists<C>                                                          class_type;

public:
//    ws_bool_t operator ()(char_type const *s) const
//    {
//        return exists_(s);
//    }

//#ifdef __STLSOFT_CF_MEMBER_TEMPLATE_FUNCTION_SUPPORT
    template <ss_typename_param_k S>
    ws_bool_t operator ()(S const &s) const
    {
        return exists_(c_str_ptr(s));
    }
//#endif /* __STLSOFT_CF_MEMBER_TEMPLATE_FUNCTION_SUPPORT */

private:
    static ws_bool_t exists_(char_type const *s)
    {
        basic_file_path_buffer<char_type>   sz0;
        basic_file_path_buffer<char_type>   sz1;
        ws_dword_t                          dw;

#ifdef __SYNSOFT_DBS_COMPILER_SUPPORTS_PRAGMA_MESSAGE
# pragma message(_sscomp_fileline_message("Might be an idea to check for, and handle, paths in quotes"))
#endif /* __SYNSOFT_DBS_COMPILER_SUPPORTS_PRAGMA_MESSAGE */

        if( !traits_type::expand_environment_strings(s, sz0, sz0.size()) ||
            !traits_type::get_full_path_name(sz0, sz1.size(), sz1))
        {
            dw = 0xFFFFFFFF;
        }
        else
        {
            dw = ::GetFileAttributes(sz1);
        }

        return 0xFFFFFFFF != dw;
    }
};

////////////////////////////////////////////////////////////////////////////////
// Unit-testing

#ifdef STLSOFT_UNITTEST

namespace unittest
{
    ss_bool_t test_winstl_functionals(unittest_reporter *r)
    {
        using stlsoft::unittest::unittest_initialiser;

        ss_bool_t               bSuccess    =   true;

        unittest_initialiser    init(r, "WinSTL", "functionals", __FILE__);

#if 0
        if(<<TODO>>)
        {
            r->report("<<TODO>> failed ", __LINE__);
            bSuccess = false;
        }
#endif /* 0 */

        return bSuccess;
    }

    unittest_registrar    unittest_winstl_functionals(test_winstl_functionals);

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

/* ////////////////////////////////////////////////////////////////////////// */

#endif /* WINSTL_INCL_H_WINSTL_FUNCTIONALS */

/* ////////////////////////////////////////////////////////////////////////// */
