/* /////////////////////////////////////////////////////////////////////////////
 * File:        unixstl_string_access.h
 *
 * Purpose:     Contains classes and functions for dealing with OLE/COM strings.
 *
 * Created:     11th January 2003
 * Updated:     11th September 2004
 *
 * Home:        http://stlsoft.org/
 *
 * Copyright (c) 2003-2004, Matthew Wilson and Synesis Software
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


/// \file unixstl_string_access.h
///
/// Contains classes and functions for dealing with OLE/COM strings.

#ifndef UNIXSTL_INCL_H_UNIXSTL_STRING_ACCESS
#define UNIXSTL_INCL_H_UNIXSTL_STRING_ACCESS

#ifndef __STLSOFT_DOCUMENTATION_SKIP_SECTION
# define UNIXSTL_VER_H_UNIXSTL_STRING_ACCESS_MAJOR      2
# define UNIXSTL_VER_H_UNIXSTL_STRING_ACCESS_MINOR      0
# define UNIXSTL_VER_H_UNIXSTL_STRING_ACCESS_REVISION   1
# define UNIXSTL_VER_H_UNIXSTL_STRING_ACCESS_EDIT       22
#endif /* !__STLSOFT_DOCUMENTATION_SKIP_SECTION */

/* /////////////////////////////////////////////////////////////////////////////
 * Includes
 */

#ifndef UNIXSTL_INCL_H_UNIXSTL
# include "unixstl.h"   // Include the UNIXSTL root header
#endif /* !UNIXSTL_INCL_H_UNIXSTL */
#ifndef STLSOFT_INCL_H_STLSOFT_STRING_ACCESS
# include "stlsoft_string_access.h"
#endif /* !STLSOFT_INCL_H_STLSOFT_STRING_ACCESS */
#include <dirent.h>

/* /////////////////////////////////////////////////////////////////////////////
 * Namespace
 *
 * The UNIXSTL components are contained within the unixstl namespace. This is
 * actually an alias for stlsoft::unixstl_project,
 *
 * The definition matrix is as follows:
 *
 * _STLSOFT_NO_NAMESPACE    _UNIXSTL_NO_NAMESPACE    unixstl definition
 * ---------------------    --------------------    -----------------
 *  not defined              not defined             = stlsoft::unixstl_project
 *  not defined              defined                 not defined
 *  defined                  not defined             unixstl
 *  defined                  defined                 not defined
 *
 */

#ifndef _UNIXSTL_NO_NAMESPACE
# if defined(_STLSOFT_NO_NAMESPACE) || \
     defined(__STLSOFT_DOCUMENTATION_SKIP_SECTION)
/* There is no stlsoft namespace, so must define ::unixstl */
namespace unixstl
{
# else
/* Define stlsoft::unixstl_project */

namespace stlsoft
{

namespace unixstl_project
{

# endif /* _STLSOFT_NO_NAMESPACE */
#endif /* !_UNIXSTL_NO_NAMESPACE */

/* ////////////////////////////////////////////////////////////////////////// */

/// \weakgroup concepts STLSoft Concepts

/// \weakgroup concepts_shims Shims
/// \ingroup concepts

/// \weakgroup concepts_shims_string_access String Access Shims
/// \ingroup concepts_shims
/// \brief These \ref concepts_shims "shims" retrieve the C-string for arbitrary types

/// \defgroup unixstl_string_access_shims String Access Shims (UNIXSTL)
/// \ingroup UNIXSTL concepts_shims_string_access
/// \brief These \ref concepts_shims "shims" retrieve the C-string for arbitrary types
/// @{

/* /////////////////////////////////////////////////////////////////////////////
 * c_str_ptr_null
 *
 * This can be applied to an expression, and the return value is either a
 * pointer to the character string or NULL.
 */

/// \brief Returns the corresponding C-string pointer of the dirent structure \c d, or NULL if \c d is empty
inline us_char_a_t const *c_str_ptr_null(struct dirent const *d)
{
    return (NULL == d || 0 == d->d_name[0]) ? NULL : d->d_name;
}

/// \brief Returns the corresponding C-string pointer of the dirent structure \c d, or NULL if \c d is empty
inline us_char_a_t const *c_str_ptr_null(struct dirent const &d)
{
    return 0 == d.d_name[0] ? NULL : d.d_name;
}

/* /////////////////////////////////////////////////////////////////////////////
 * c_str_ptr
 *
 * This can be applied to an expression, and the return value is either a
 * pointer to the character string or to an empty string.
 */

/// \brief Returns the corresponding C-string pointer of the dirent structure \c d
inline us_char_a_t const *c_str_ptr(struct dirent const *d)
{
    return (NULL == d) ? "" : d->d_name;
}

/// \brief Returns the corresponding C-string pointer of the dirent structure \c d
inline us_char_a_t const *c_str_ptr(struct dirent const &d)
{
    return d.d_name;
}

/* /////////////////////////////////////////////////////////////////////////////
 * c_str_len
 *
 * This can be applied to an expression, and the return value is the number of
 * characters in the character string in the expression.
 */

/// \brief Returns the length (in characters) of the dirent structure \c d, <b><i>not</i></b> including the null-terminating character
inline us_size_t c_str_len(struct dirent const *d)
{
    return stlsoft_ns_qual(c_str_len)(c_str_ptr(d));
}

/// \brief Returns the length (in characters) of the dirent structure \c d, <b><i>not</i></b> including the null-terminating character
inline us_size_t c_str_len(struct dirent const &d)
{
    return stlsoft_ns_qual(c_str_len)(c_str_ptr(d));
}

/* /////////////////////////////////////////////////////////////////////////////
 * c_str_size
 *
 * This can be applied to an expression, and the return value is the number of
 * bytes required to store the character string in the expression, NOT including
 * the null-terminating character.
 */

/// \brief Returns the size (in bytes) of the dirent structure \c d, <b><i>not</i></b> including the null-terminating character
inline us_size_t c_str_size(struct dirent const *d)
{
    return stlsoft_ns_qual(c_str_size)(c_str_ptr(d));
}

/// \brief Returns the size (in bytes) of the dirent structure \c d, <b><i>not</i></b> including the null-terminating character
inline us_size_t c_str_size(struct dirent const &d)
{
    return stlsoft_ns_qual(c_str_size)(c_str_ptr(d));
}

////////////////////////////////////////////////////////////////////////////////
// Unit-testing

#ifdef STLSOFT_UNITTEST

namespace unittest
{
    ss_bool_t test_unixstl_string_access(unittest_reporter *r)
    {
        using stlsoft::unittest::unittest_initialiser;

        ss_bool_t               bSuccess    =   true;

        unittest_initialiser    init(r, "UNIXSTL", "string_access", __FILE__);

        struct dirent   de;

        strcpy(de.d_name, "/home/stlsoft");

        if(0 != strcmp("/home/stlsoft", c_str_ptr(de)))
        {
            r->report("c_str_ptr(dirent &) failed ", __LINE__);
            bSuccess = false;
        }

        if(0 != strcmp("/home/stlsoft", c_str_ptr(&de)))
        {
            r->report("c_str_ptr(dirent *) failed ", __LINE__);
            bSuccess = false;
        }

        return bSuccess;
    }

    unittest_registrar    unittest_unixstl_string_access(test_unixstl_string_access);

} // namespace unittest

#endif /* STLSOFT_UNITTEST */

/* ////////////////////////////////////////////////////////////////////////// */

/// @} // end of group unixstl_string_access_shims

/* ////////////////////////////////////////////////////////////////////////// */

#ifndef _UNIXSTL_NO_NAMESPACE
# if defined(_STLSOFT_NO_NAMESPACE) || \
     defined(__STLSOFT_DOCUMENTATION_SKIP_SECTION)
} // namespace unixstl
# else
} // namespace stlsoft::unixstl_project
} // namespace stlsoft
# endif /* _STLSOFT_NO_NAMESPACE */
#endif /* !_UNIXSTL_NO_NAMESPACE */

/* /////////////////////////////////////////////////////////////////////////////
 * Namespace
 *
 * The string access shims exist either in the stlsoft namespace, or in the
 * global namespace. This is required by the lookup rules.
 *
 */

#ifndef _UNIXSTL_NO_NAMESPACE
# if !defined(_STLSOFT_NO_NAMESPACE) && \
     !defined(__STLSOFT_DOCUMENTATION_SKIP_SECTION)
namespace stlsoft
{
# else /* ? _STLSOFT_NO_NAMESPACE */
/* There is no stlsoft namespace, so must define in the global namespace */
# endif /* !_STLSOFT_NO_NAMESPACE */

using ::unixstl::c_str_ptr_null;

using ::unixstl::c_str_ptr;

using ::unixstl::c_str_len;

using ::unixstl::c_str_size;

# if !defined(_STLSOFT_NO_NAMESPACE) && \
     !defined(__STLSOFT_DOCUMENTATION_SKIP_SECTION)
} // namespace stlsoft
# else /* ? _STLSOFT_NO_NAMESPACE */
/* There is no stlsoft namespace, so must define in the global namespace */
# endif /* !_STLSOFT_NO_NAMESPACE */
#endif /* !_UNIXSTL_NO_NAMESPACE */

/* ////////////////////////////////////////////////////////////////////////// */

#endif /* !UNIXSTL_INCL_H_UNIXSTL_STRING_ACCESS */

/* ////////////////////////////////////////////////////////////////////////// */
