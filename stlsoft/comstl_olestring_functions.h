/* /////////////////////////////////////////////////////////////////////////////
 * File:        comstl_olestring_functions.h (originally MOOleStr.h, ::SynesisCom)
 *
 * Purpose:     Contains classes and functions for dealing with OLE/COM strings.
 *
 * Created:     2nd March 1996
 * Updated:     11th September 2004
 *
 * Home:        http://stlsoft.org/
 *
 * Copyright (c) 1996-2004, Matthew Wilson and Synesis Software
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


/// \file comstl_olestring_functions.h
///
/// Contains classes and functions for dealing with OLE/COM strings.

#ifndef COMSTL_INCL_H_COMSTL_OLESTRING_FUNCTIONS
#define COMSTL_INCL_H_COMSTL_OLESTRING_FUNCTIONS

#ifndef __STLSOFT_DOCUMENTATION_SKIP_SECTION
# define COMSTL_VER_H_COMSTL_OLESTRING_FUNCTIONS_MAJOR      3
# define COMSTL_VER_H_COMSTL_OLESTRING_FUNCTIONS_MINOR      0
# define COMSTL_VER_H_COMSTL_OLESTRING_FUNCTIONS_REVISION   1
# define COMSTL_VER_H_COMSTL_OLESTRING_FUNCTIONS_EDIT       125
#endif /* !__STLSOFT_DOCUMENTATION_SKIP_SECTION */

/* ////////////////////////////////////////////////////////////////////////////
 * Includes
 */

#ifndef COMSTL_INCL_H_COMSTL
# include "comstl.h"                     // Include the COMSTL root header
#endif /* !COMSTL_INCL_H_COMSTL */
#ifndef COMSTL_INCL_H_COMSTL_MEMORY_FUNCTIONS
# include "comstl_memory_functions.h"    //
#endif /* !COMSTL_INCL_H_COMSTL_MEMORY_FUNCTIONS */
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

/// \defgroup comstl_olestring_functions OLE String Functions
/// \ingroup COMSTL
/// \brief These functions create, manipulate and destroy instances of COM's OLE string type
/// @{

/* /////////////////////////////////////////////////////////////////////////////
 * Functions
 */

/// Creates an OLE string from an ANSI string
///
/// \param s The string from which to create the OLE string
/// \result The created OLE string
inline LPOLESTR olestring_create_a(cs_char_a_t const *s)
{
    LPOLESTR    posz;

    if(s == NULL)
    {
        posz = NULL;
    }
    else
    {
        cs_size_t   cch     =   ::MultiByteToWideChar(0, 0, s, -1, NULL, 0);

        posz    =   static_cast<LPOLESTR>(::CoTaskMemAlloc(sizeof(OLECHAR) * (cch + 1)));

        if(posz != NULL)
        {
            ::MultiByteToWideChar(0, 0, s, -1, posz, (cch + 1));
        }
    }

    return posz;
}

/// Creates an OLE string from a Unicode string
///
/// \param s The string from which to create the OLE string
/// \result The created OLE string
inline LPOLESTR olestring_create_w(cs_char_w_t const *s)
{
    LPOLESTR    posz;

    if(s == NULL)
    {
        posz = NULL;
    }
    else
    {
        cs_size_t   cch =   ::wcslen(s);

        posz    =   static_cast<LPOLESTR>(::CoTaskMemAlloc(sizeof(OLECHAR) * (cch + 1)));

        if(posz != NULL)
        {
            ::wcscpy(posz, s);
        }
    }

    return posz;
}

/// Creates an OLE string from an ANSI string
///
/// \param s The string from which to create the OLE string
/// \result The created OLE string
inline LPOLESTR olestring_create(cs_char_a_t const *s)
{
    return olestring_create_a(s);
}

/// Creates an OLE string from a Unicode string
///
/// \param s The string from which to create the OLE string
/// \result The created OLE string
inline LPOLESTR olestring_create(cs_char_w_t const *s)
{
    return olestring_create_w(s);
}

/// Destroys an OLE string
///
/// \param posz The OLE string to destroy
inline void olestring_destroy(LPOLESTR posz)
{
    comstl_message_assert("Invalid request to destroy non-COM string", CoTaskMemDidAlloc(posz) != 0);

    ::CoTaskMemFree(static_cast<LPVOID>(posz));
}

/// Duplicates an OLE string
///
/// \param posz The OLE string to duplicate
/// \return The copied OLE string
inline LPOLESTR olestring_dup(LPCOLESTR posz)
{
    return olestring_create(posz);
}

////////////////////////////////////////////////////////////////////////////////
// Unit-testing

#ifdef STLSOFT_UNITTEST

namespace unittest
{
    ss_bool_t test_comstl_olestring_functions(unittest_reporter *r)
    {
        using stlsoft::unittest::unittest_initialiser;

        ss_bool_t               bSuccess    =   true;

        unittest_initialiser    init(r, "COMSTL", "olestring_functions", __FILE__);

        LPOLESTR    str1   =   olestring_create("Hello, Sailor!");
        LPOLESTR    str2   =   olestring_create(L"Hello, Sailor!");

        if( NULL != str1 &&
            NULL != str2 &&
            0 != wcscmp(str1, str2))
        {
            r->report("OLE string (ANSI + Unicode) creation failed ", __LINE__);
            bSuccess = false;
        }

        LPOLESTR    str3   =   olestring_dup(str1);

        if( NULL != str1 &&
            NULL != str3 &&
            0 != wcscmp(str1, str3))
        {
            r->report("OLE string duplication failed ", __LINE__);
            bSuccess = false;
        }

        olestring_destroy(str3);
        olestring_destroy(str2);
        olestring_destroy(str1);

        return bSuccess;
    }

    unittest_registrar    unittest_comstl_olestring_functions(test_comstl_olestring_functions);

} // namespace unittest

#endif /* STLSOFT_UNITTEST */

/* ////////////////////////////////////////////////////////////////////////// */

/// @} // end of group comstl_olestring_functions

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

#endif /* !COMSTL_INCL_H_COMSTL_OLESTRING_FUNCTIONS */

/* ////////////////////////////////////////////////////////////////////////// */
