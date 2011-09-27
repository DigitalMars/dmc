/* /////////////////////////////////////////////////////////////////////////////
 * File:        inetstl_functionals.h
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


/// \file inetstl_functionals.h
///
/// A number of useful functionals .

#ifndef INETSTL_INCL_H_INETSTL_FUNCTIONALS
#define INETSTL_INCL_H_INETSTL_FUNCTIONALS

#ifndef __STLSOFT_DOCUMENTATION_SKIP_SECTION
# define INETSTL_VER_H_INETSTL_FUNCTIONALS_MAJOR    2
# define INETSTL_VER_H_INETSTL_FUNCTIONALS_MINOR    0
# define INETSTL_VER_H_INETSTL_FUNCTIONALS_REVISION 1
# define INETSTL_VER_H_INETSTL_FUNCTIONALS_EDIT     7
#endif /* !__STLSOFT_DOCUMENTATION_SKIP_SECTION */

/* /////////////////////////////////////////////////////////////////////////////
 * Includes
 */

#ifndef INETSTL_INCL_H_INETSTL
# include "inetstl.h"                // Include the InetSTL root header
#endif /* !INETSTL_INCL_H_INETSTL */
#ifndef STLSOFT_INCL_H_STLSOFT_STRING_ACCESS
# include "stlsoft_string_access.h" // c_str_ptr, etc.
#endif /* !STLSOFT_INCL_H_STLSOFT_STRING_ACCESS */
#ifndef INETSTL_INCL_H_INETSTL_STRING_ACCESS
//# include "inetstl_string_access.h"  // c_str_ptr, etc.
#endif /* !INETSTL_INCL_H_INETSTL_STRING_ACCESS */
#ifndef INETSTL_INCL_H_INETSTL_FILESYSTEM_TRAITS
# include "inetstl_filesystem_traits.h"
#endif /* !INETSTL_INCL_H_INETSTL_FILESYSTEM_TRAITS */
#ifndef _INETSTL_FUNCTIONALS_NO_STD
# include <functional>
#endif /* _INETSTL_FUNCTIONALS_NO_STD */

/* /////////////////////////////////////////////////////////////////////////////
 * Namespace
 */

#ifndef _INETSTL_NO_NAMESPACE
# if defined(_STLSOFT_NO_NAMESPACE) || \
     defined(__STLSOFT_DOCUMENTATION_SKIP_SECTION)
/* There is no stlsoft namespace, so must define ::inetstl */
namespace inetstl
{
# else
/* Define stlsoft::inetstl_project */

namespace stlsoft
{

namespace inetstl_project
{

# endif /* _STLSOFT_NO_NAMESPACE */
#endif /* !_INETSTL_NO_NAMESPACE */

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
#ifndef _INETSTL_FUNCTIONALS_NO_STD
    : inetstl_ns_qual_std(binary_function)<C const *, C const *, is_bool_t>
#endif /* _INETSTL_FUNCTIONALS_NO_STD */
{
public:
    /// The character type
    typedef C                                                                       char_type;
#ifndef _INETSTL_FUNCTIONALS_NO_STD
private:
    typedef inetstl_ns_qual_std(binary_function)<C const *, C const *, is_bool_t>    parent_class_type;
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
    typedef is_bool_t                                                               result_type;
#endif /* _INETSTL_FUNCTIONALS_NO_STD */
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
        char_type   path1[_MAX_PATH + 1];
        char_type   path2[_MAX_PATH + 1];
        result_type result;
#if 0
        char_type   *dummy;

        if(!traits_type::get_full_path_name(s1, inetstl_num_elements(path1), path1, &dummy))
        {
            result = false;
        }
        else if(!traits_type::get_full_path_name(s2, inetstl_num_elements(path2), path2, &dummy))
        {
            result = false;
        }
        else
#else
            traits_type::str_copy(path1, s1);
            traits_type::str_copy(path2, s2);
#endif /* 0 */
        {
            traits_type::ensure_dir_end(path1);
            traits_type::ensure_dir_end(path2);

            result = 0 == traits_type::str_compare_no_case(path1, path2);
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
    : inetstl_ns_qual_std(unary_function)<A, is_bool_t>
{
public:
    /// The character type
    typedef C                                                                       char_type;
    /// The traits type
    typedef filesystem_traits<C>                                                    traits_type;
    /// The current parameterisation of the type
    typedef path_exists<C>                                                          class_type;

public:
//    is_bool_t operator ()(char_type const *s) const
//    {
//        return exists_(s);
//    }

//#ifdef __STLSOFT_CF_MEMBER_TEMPLATE_FUNCTION_SUPPORT
    template <ss_typename_param_k S>
    is_bool_t operator ()(S const &s) const
    {
        return exists_(c_str_ptr(s));
    }
//#endif /* __STLSOFT_CF_MEMBER_TEMPLATE_FUNCTION_SUPPORT */

private:
    static is_bool_t exists_(char_type const *s)
    {
        char_type   sz0[1 + _MAX_PATH];
        char_type   sz1[1 + _MAX_PATH];
        is_dword_t  dw;

#ifdef __SYNSOFT_DBS_COMPILER_SUPPORTS_PRAGMA_MESSAGE
# pragma message(_sscomp_fileline_message("Might be an idea to check for, and handle, paths in quotes"))
#endif /* __SYNSOFT_DBS_COMPILER_SUPPORTS_PRAGMA_MESSAGE */

        if( !traits_type::expand_environment_strings(s, sz0, stlsoft_num_elements(sz0)) ||
            !traits_type::get_full_path_name(sz0, stlsoft_num_elements(sz1), sz1))
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
    ss_bool_t test_inetstl_functionals(unittest_reporter *r)
    {
        using stlsoft::unittest::unittest_initialiser;

        ss_bool_t               bSuccess    =   true;

        unittest_initialiser    init(r, "InetSTL", "functionals", __FILE__);

#if 0
        if(<<TODO>>)
        {
            r->report("<<TODO>> failed ", __LINE__);
            bSuccess = false;
        }
#endif /* 0 */

        return bSuccess;
    }

    unittest_registrar    unittest_inetstl_functionals(test_inetstl_functionals);

} // namespace unittest

#endif /* STLSOFT_UNITTEST */

/* ////////////////////////////////////////////////////////////////////////// */

#ifndef _INETSTL_NO_NAMESPACE
# if defined(_STLSOFT_NO_NAMESPACE) || \
     defined(__STLSOFT_DOCUMENTATION_SKIP_SECTION)
} // namespace inetstl
# else
} // namespace inetstl_project
} // namespace stlsoft
# endif /* _STLSOFT_NO_NAMESPACE */
#endif /* !_INETSTL_NO_NAMESPACE */

/* ////////////////////////////////////////////////////////////////////////// */

#endif /* INETSTL_INCL_H_INETSTL_FUNCTIONALS */

/* ////////////////////////////////////////////////////////////////////////// */
