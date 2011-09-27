/* /////////////////////////////////////////////////////////////////////////////
 * File:        atlstl_window_access.h
 *
 * Purpose:     Contains access shims for windows.
 *
 * Created:     10th January 2003
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


/// \file atlstl_window_access.h
///
/// Contains access shims for windows.

#ifndef ATLSTL_INCL_H_ATLSTL_WINDOW_ACCESS
#define ATLSTL_INCL_H_ATLSTL_WINDOW_ACCESS

#ifndef __STLSOFT_DOCUMENTATION_SKIP_SECTION
# define ATLSTL_VER_H_ATLSTL_WINDOW_ACCESS_MAJOR     2
# define ATLSTL_VER_H_ATLSTL_WINDOW_ACCESS_MINOR     0
# define ATLSTL_VER_H_ATLSTL_WINDOW_ACCESS_REVISION  1
# define ATLSTL_VER_H_ATLSTL_WINDOW_ACCESS_EDIT      19
#endif /* !__STLSOFT_DOCUMENTATION_SKIP_SECTION */

/* ////////////////////////////////////////////////////////////////////////////
 * Includes
 */

#ifndef ATLSTL_INCL_H_ATLSTL
# include "atlstl.h"                // Include the ATLSTL root header
#endif /* !ATLSTL_INCL_H_ATLSTL */

#if defined(__STLSOFT_COMPILER_IS_BORLAND)
# pragma warn -8013 /* Suppresses "Possible use of 'f' before definition in '. . .'" */
# pragma warn -8084 /* Suppresses "Suggest parentheses to clarify precedence in function 'f()'" */
#endif /* compiler */

#include <atlwin.h>

#if defined(__STLSOFT_COMPILER_IS_BORLAND)
# pragma warn .8013 /* Suppresses "Possible use of 'f' before definition in '. . .'" */
# pragma warn .8084 /* Suppresses "Suggest parentheses to clarify precedence in function 'f()'" */
#endif /* compiler */

/* /////////////////////////////////////////////////////////////////////////////
 * Namespace
 */

#ifndef _ATLSTL_NO_NAMESPACE
# if defined(_STLSOFT_NO_NAMESPACE) || \
     defined(__STLSOFT_DOCUMENTATION_SKIP_SECTION)
/* There is no stlsoft namespace, so must define ::atlstl */
namespace atlstl
{
# else
/* Define stlsoft::atlstl_project */

namespace stlsoft
{

namespace atlstl_project
{

# endif /* _STLSOFT_NO_NAMESPACE */
#endif /* !_ATLSTL_NO_NAMESPACE */

/* ////////////////////////////////////////////////////////////////////////// */

/// \weakgroup concepts STLSoft Concepts

/// \weakgroup concepts_shims Shims
/// \ingroup concepts

/// \weakgroup concepts_shims_window_access Window Access Shims
/// \ingroup concepts_shims
/// \brief These \ref concepts_shims "shims" retrieve the HWND for arbitrary types

/// \defgroup atlstl_window_access_shims Window Access Shims (ATLSTL)
/// \ingroup ATLSTL concepts_shims_window_access
/// \brief These \ref concepts_shims "shims" retrieve the HWND for arbitrary types
/// @{

/* /////////////////////////////////////////////////////////////////////////////
 * get_hwnd
 *
 * This can be applied to an expression, and the return value is the
 * corresponding HWND.
 */

/* HWND */

/// \brief Access the HWND of the given CWindow instance
///
/// This access <a href = "http://stlsoft.org/white_papers.html#shims">shim</a> retrieves the
/// HWND window handle for the given CWindow instance.
///
/// \param w A reference to the CWindow instance whose HWND will be retrieved
/// \return The HWND corresponding to the given CWindow instance \c w
inline HWND get_hwnd(CWindow const &w)
{
    return w.m_hWnd;
}

////////////////////////////////////////////////////////////////////////////////
// Unit-testing

#ifdef STLSOFT_UNITTEST

namespace unittest
{
    ss_bool_t test_atlstl_window_access(unittest_reporter *r)
    {
        using stlsoft::unittest::unittest_initialiser;

        ss_bool_t               bSuccess    =   true;

        unittest_initialiser    init(r, "ATLSTL", "window_access", __FILE__);

        HWND    hwndDesktop =   ::GetDesktopWindow();
        CWindow wndDesktop;

        wndDesktop.Attach(hwndDesktop);

        if(get_hwnd(wndDesktop) != hwndDesktop)
        {
            r->report("get_hwnd(CWindow) failed ", __LINE__);
            bSuccess = false;
        }

        wndDesktop.Detach();

        return bSuccess;
    }

    unittest_registrar    unittest_atlstl_window_access(test_atlstl_window_access);

} // namespace unittest

#endif /* STLSOFT_UNITTEST */

////////////////////////////////////////////////////////////////////////////////
// Implementation

/* ////////////////////////////////////////////////////////////////////////// */

/// @} // end of group atlstl_window_access_shims

/* ////////////////////////////////////////////////////////////////////////// */

#ifndef _ATLSTL_NO_NAMESPACE
# if defined(_STLSOFT_NO_NAMESPACE) || \
     defined(__STLSOFT_DOCUMENTATION_SKIP_SECTION)
} // namespace atlstl
# else
} // namespace atlstl_project
} // namespace stlsoft
# endif /* _STLSOFT_NO_NAMESPACE */
#endif /* !_ATLSTL_NO_NAMESPACE */

/* ////////////////////////////////////////////////////////////////////////// */

#endif /* ATLSTL_INCL_H_ATLSTL_SYSTEM_INFO */

/* ////////////////////////////////////////////////////////////////////////// */
