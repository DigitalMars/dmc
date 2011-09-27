/* /////////////////////////////////////////////////////////////////////////////
 * File:        winstl_scale_functions.h
 *
 * Purpose:     Contains scale functions.
 *
 * Created:     7th February 2002
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


/// \file winstl_scale_functions.h
///
/// Contains scale functions.

#ifndef WINSTL_INCL_H_WINSTL_SCALE_FUNCTIONS
#define WINSTL_INCL_H_WINSTL_SCALE_FUNCTIONS

#ifndef __STLSOFT_DOCUMENTATION_SKIP_SECTION
# define WINSTL_VER_H_WINSTL_SCALE_FUNCTIONS_MAJOR      2
# define WINSTL_VER_H_WINSTL_SCALE_FUNCTIONS_MINOR      0
# define WINSTL_VER_H_WINSTL_SCALE_FUNCTIONS_REVISION   1
# define WINSTL_VER_H_WINSTL_SCALE_FUNCTIONS_EDIT       21
#endif /* !__STLSOFT_DOCUMENTATION_SKIP_SECTION */

/* /////////////////////////////////////////////////////////////////////////////
 * Includes
 */

#include <winstl.h>             // Include the WinSTL root header

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

/* /////////////////////////////////////////////////////////////////////////////
 * calc_width()
 *
 * This attribute shim calculates the width of an entity.
 */

/// Calculates the width of a \c SIZE instance
inline ws_sint_t calc_width(SIZE const &size)
{
    return size.cx;
}

/// Calculates the width of a \c RECT instance
inline ws_sint_t calc_width(RECT const &rc)
{
    return rc.right - rc.left;
}

/* /////////////////////////////////////////////////////////////////////////////
 * calc_height()
 *
 * This attribute shim calculates the height of an entity.
 */

/// Calculates the height of a \c SIZE instance
inline ws_sint_t calc_height(SIZE const &size)
{
    return size.cy;
}

/// Calculates the height of a \c RECT instance
inline ws_sint_t calc_height(RECT const &rc)
{
    return rc.bottom - rc.top;
}

////////////////////////////////////////////////////////////////////////////////
// Unit-testing

#ifdef STLSOFT_UNITTEST

namespace unittest
{
    ss_bool_t test_winstl_scale_functions(unittest_reporter *r)
    {
        using stlsoft::unittest::unittest_initialiser;

        ss_bool_t               bSuccess    =   true;

        unittest_initialiser    init(r, "WinSTL", "scale_functions", __FILE__);

        SIZE    size    =   { 100, 100 };
        RECT    rect    =   { 0, 0, 100, 100 };

        if(calc_width(size) != 100)
        {
            r->report("calc_width(SIZE) failed ", __LINE__);
            bSuccess = false;
        }

        if(calc_width(rect) != 100)
        {
            r->report("calc_width(RECT) failed ", __LINE__);
            bSuccess = false;
        }

        if(calc_height(size) != 100)
        {
            r->report("calc_height(SIZE) failed ", __LINE__);
            bSuccess = false;
        }

        if(calc_height(rect) != 100)
        {
            r->report("calc_height(RECT) failed ", __LINE__);
            bSuccess = false;
        }

        return bSuccess;
    }

    unittest_registrar    unittest_winstl_scale_functions(test_winstl_scale_functions);

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

#endif /* WINSTL_INCL_H_WINSTL_SCALE_FUNCTIONS */

/* ////////////////////////////////////////////////////////////////////////// */
