/* /////////////////////////////////////////////////////////////////////////////
 * File:        comstl_bstr_functions.h (originally MOBStrFn.h, ::SynesisCom)
 *
 * Purpose:     Contains classes and functions for dealing with BSTR strings.
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


/// \file comstl_bstr_functions.h
///
/// Contains classes and functions for dealing with BSTR strings.

#ifndef COMSTL_INCL_H_COMSTL_BSTR_FUNCTIONS
#define COMSTL_INCL_H_COMSTL_BSTR_FUNCTIONS

#ifndef __STLSOFT_DOCUMENTATION_SKIP_SECTION
# define COMSTL_VER_H_COMSTL_BSTR_FUNCTIONS_MAJOR       2
# define COMSTL_VER_H_COMSTL_BSTR_FUNCTIONS_MINOR       0
# define COMSTL_VER_H_COMSTL_BSTR_FUNCTIONS_REVISION    1
# define COMSTL_VER_H_COMSTL_BSTR_FUNCTIONS_EDIT        42
#endif /* !__STLSOFT_DOCUMENTATION_SKIP_SECTION */

/* ////////////////////////////////////////////////////////////////////////////
 * Includes
 */

#ifndef COMSTL_INCL_H_COMSTL
# include "comstl.h"                // Include the COMSTL root header
#endif /* !COMSTL_INCL_H_COMSTL */
#ifndef STLSOFT_INCL_H_STLSOFT_AUTO_BUFFER
# include "stlsoft_auto_buffer.h"   // stlsoft::auto_buffer
#endif /* !STLSOFT_INCL_H_STLSOFT_AUTO_BUFFER */
#ifndef COMSTL_INCL_H_COMSTL_TASK_ALLOCATOR
# include "comstl_task_allocator.h" // task_allocator
#endif /* !COMSTL_INCL_H_COMSTL_TASK_ALLOCATOR */
#ifdef STLSOFT_UNITTEST
# include <wchar.h>
#endif /* STLSOFT_UNITTEST */

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

/// \defgroup comstl_bstr_functions BSTR Functions
/// \ingroup COMSTL
/// \brief These functions create, manipulate and destroy instances of COM's BSTR type
/// @{

/* /////////////////////////////////////////////////////////////////////////////
 * Functions
 */

/// \brief Creates a BSTR from a Unicode string
///
/// \param s The string from which to create the BSTR
/// \result The created BSTR
inline BSTR bstr_create_w(cs_char_w_t const *s)
{
    return ::SysAllocString(s);
}

/// \brief Creates a BSTR from an ANSI string
///
/// \param s The string from which to create the BSTR
/// \result The created BSTR
inline BSTR bstr_create_a(cs_char_a_t const *s)
{
    BSTR    bstr;

    if(s == NULL)
    {
        bstr = NULL;
    }
    else
    {
        stlsoft_ns_qual(auto_buffer)<cs_char_w_t, task_allocator<cs_char_w_t>, 512> buffer(1 + strlen(s));

        ::MultiByteToWideChar(0, 0, s, -1, buffer, buffer.size());

        bstr = bstr_create_w(buffer);
    }

    return bstr;
}

/// \brief Creates a BSTR from an ANSI string
///
/// \param s The string from which to create the BSTR
/// \result The created BSTR
inline BSTR bstr_create(cs_char_a_t const *s)
{
    return bstr_create_a(s);
}

/// \brief Creates a BSTR from a Unicode string
///
/// \param s The string from which to create the BSTR
/// \result The created BSTR
inline BSTR bstr_create(cs_char_w_t const *s)
{
    return bstr_create_w(s);
}

/// \brief Destroys a BSTR
///
/// \param bstr The BSTR to destroy
inline void bstr_destroy(BSTR bstr)
{
    ::SysFreeString(bstr);
}

/// \brief Duplicates a BSTR
///
/// \param bstr The BSTR to duplicate
/// \return The copied BSTR
inline BSTR bstr_dup(BSTR bstr)
{
    return bstr_create(bstr);
}

////////////////////////////////////////////////////////////////////////////////
// Unit-testing

#ifdef STLSOFT_UNITTEST

namespace unittest
{
    ss_bool_t test_comstl_bstr_functions(unittest_reporter *r)
    {
        using stlsoft::unittest::unittest_initialiser;

        ss_bool_t               bSuccess    =   true;

        unittest_initialiser    init(r, "COMSTL", "bstr_functions", __FILE__);

        BSTR    bstr1   =   bstr_create("Hello, Sailor!");
        BSTR    bstr2   =   bstr_create(L"Hello, Sailor!");

        if( NULL != bstr1 &&
            NULL != bstr2 &&
            0 != wcscmp(bstr1, bstr2))
        {
            r->report("BSTR (ANSI + Unicode) creation failed ", __LINE__);
            bSuccess = false;
        }

        bstr_destroy(bstr1);
        bstr_destroy(bstr2);

        return bSuccess;
    }

    unittest_registrar    unittest_comstl_bstr_functions(test_comstl_bstr_functions);

} // namespace unittest

#endif /* STLSOFT_UNITTEST */

/* ////////////////////////////////////////////////////////////////////////// */

/// @} // end of group comstl_bstr_functions

/* ////////////////////////////////////////////////////////////////////////// */

#ifndef _COMSTL_NO_NAMESPACE
# if defined(_STLSOFT_NO_NAMESPACE) || \
     defined(__STLSOFT_DOCUMENTATION_SKIP_SECTION)
} // namespace comstl
# else
} // namespace comstl_project
} // namespace stlsoft
# endif /* _STLSOFT_NO_NAMESPACE */
#endif /* !_COMSTL_NO_NAMESPACE */

/* ////////////////////////////////////////////////////////////////////////// */

#endif /* !COMSTL_INCL_H_COMSTL_BSTR_FUNCTIONS */

/* ////////////////////////////////////////////////////////////////////////// */
