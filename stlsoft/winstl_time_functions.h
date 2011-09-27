/* /////////////////////////////////////////////////////////////////////////////
 * File:        winstl_time_functions.h
 *
 * Purpose:     Some simple functions for manipulating time.
 *
 * Created:     21st November 2003
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


/// \file winstl_time_functions.h
///
/// Some simple functions for manipulating time.

#ifndef WINSTL_INCL_H_WINSTL_TIME_FUNCTIONS
#define WINSTL_INCL_H_WINSTL_TIME_FUNCTIONS

#ifndef __STLSOFT_DOCUMENTATION_SKIP_SECTION
# define WINSTL_VER_H_WINSTL_TIME_FUNCTIONS_MAJOR       2
# define WINSTL_VER_H_WINSTL_TIME_FUNCTIONS_MINOR       0
# define WINSTL_VER_H_WINSTL_TIME_FUNCTIONS_REVISION    1
# define WINSTL_VER_H_WINSTL_TIME_FUNCTIONS_EDIT        12
#endif /* !__STLSOFT_DOCUMENTATION_SKIP_SECTION */

/* /////////////////////////////////////////////////////////////////////////////
 * Includes
 */

#ifndef WINSTL_INCL_H_WINSTL
# include "winstl.h"                        // Include the WinSTL root header
#endif /* !WINSTL_INCL_H_WINSTL */
#ifndef STLSOFT_INCL_H_STLSOFT_LIMIT_TRAITS
# include "stlsoft_limit_traits.h"
#endif /* !STLSOFT_INCL_H_STLSOFT_LIMIT_TRAITS */
#include <time.h>

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

/// \weakgroup libraries_time Time Library
/// \ingroup libraries
/// \brief This library provides facilities for representing and manipulating time

/// \weakgroup winstl_time_library Time Library (WinSTL)
/// \ingroup WinSTL libraries_time
/// \brief This library provides facilities for representing and manipulating time for the Win32 API
/// @{

/* /////////////////////////////////////////////////////////////////////////////
 * Functions
 */

#ifdef STLSOFT_CF_64BIT_INT_SUPPORT

/// Converts a time_t into a FILETIME
///
/// \note This follows the algorithm provided in Q167296
inline void UNIXTimeToFILETIME(time_t t, FILETIME &ft)
{
    ws_sint64_t i = Int32x32To64(t, 10000000) + __STLSOFT_GEN_UINT64_SUFFIX(116444736000000000);

    ft.dwLowDateTime = (DWORD)i;
    ft.dwHighDateTime = (DWORD)(i >> 32);
}

/// Converts a FILETIME into a time_t
inline time_t FILETIMEToUNIXTime(FILETIME const &ft, ws_sint32_t *microseconds = NULL)
{
    ws_sint64_t i;

    i = ft.dwHighDateTime;
    i <<= 32;
    i |= ft.dwLowDateTime;

    i -= __STLSOFT_GEN_UINT64_SUFFIX(116444736000000000);
    if(NULL != microseconds)
    {
        *microseconds = static_cast<ws_sint32_t>((i % 10000000) / 10);
    }
    i /= 10000000;

    return (time_t)i;
}

inline void FILETIMEToUNIXTime(FILETIME const &ft, time_t &t)
{
    t = FILETIMEToUNIXTime(ft);
}

#endif /* STLSOFT_CF_64BIT_INT_SUPPORT */


////////////////////////////////////////////////////////////////////////////////
// Unit-testing

#ifdef STLSOFT_UNITTEST

namespace unittest
{
    ss_bool_t test_winstl_time_functions(unittest_reporter *r)
    {
        using stlsoft::unittest::unittest_initialiser;

        ss_bool_t               bSuccess    =   true;

        unittest_initialiser    init(r, "WinSTL", "time_functions", __FILE__);

#if 0
        if(<<TODO>>)
        {
            r->report("<<TODO>> failed ", __LINE__);
            bSuccess = false;
        }
#endif /* 0 */

        return bSuccess;
    }

    unittest_registrar    unittest_winstl_time_functions(test_winstl_time_functions);

} // namespace unittest

#endif /* STLSOFT_UNITTEST */

/* ////////////////////////////////////////////////////////////////////////// */

/// @} // end of group winstl_time_library

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

#endif /* WINSTL_INCL_H_WINSTL_TIME_FUNCTIONS */

/* ////////////////////////////////////////////////////////////////////////// */
