/* /////////////////////////////////////////////////////////////////////////////
 * File:        winstl_system_info.h
 *
 * Purpose:     Contains the basic_system_info class, which provides information
 *              regarding the host system, such as number of processors and
 *              page size.
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


/// \file winstl_system_info.h
///
/// Contains the basic_system_info class, which provides information regarding the host system, such as number of processors and page size.

#ifndef WINSTL_INCL_H_WINSTL_SYSTEM_INFO
#define WINSTL_INCL_H_WINSTL_SYSTEM_INFO

#ifndef __STLSOFT_DOCUMENTATION_SKIP_SECTION
# define WINSTL_VER_H_WINSTL_SYSTEM_INFO_MAJOR      2
# define WINSTL_VER_H_WINSTL_SYSTEM_INFO_MINOR      0
# define WINSTL_VER_H_WINSTL_SYSTEM_INFO_REVISION   1
# define WINSTL_VER_H_WINSTL_SYSTEM_INFO_EDIT       33
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

/* ////////////////////////////////////////////////////////////////////////// */

/// \weakgroup libraries STLSoft Libraries
/// \brief The individual libraries

/// \weakgroup libraries_system System Library
/// \ingroup libraries
/// \brief This library provides facilities for accessing system attributes

/// \defgroup winstl_system_library System Library (WinSTL)
/// \ingroup WinSTL libraries_system
/// \brief This library provides facilities for accessing Win32 system attributes
/// @{

/* /////////////////////////////////////////////////////////////////////////////
 * Classes
 */

/// Provides system information
///
/// This class wraps the GetSystemInfo() API function. Since the information that
/// this function provides is constant for any particular active system for its
/// lifetime, the function is called only once, as implemented via the
/// _get_systeminfo() method.
class system_info
{
public:
    typedef system_info class_type;

// Construction
private:
    system_info();
    ~system_info();

// Operations
public:
    /// Returns the number of processes on the host machine
    static ws_uint32_t number_of_processors()
    {
        return _get_systeminfo().dwNumberOfProcessors;
    }

    /// Returns the page size of the host machine
    static ws_size_t page_size()
    {
        return _get_systeminfo().dwPageSize;
    }

    /// Returns the allocation granularity of the host machine
    static ws_size_t allocation_granularity()
    {
        return _get_systeminfo().dwAllocationGranularity;
    }

    /// Provides a non-mutable (const) reference to the \c SYSTEM_INFO instance
    static const SYSTEM_INFO &get_systeminfo()
    {
        return _get_systeminfo();
    }

// Implementation
private:
    /// Unfortunately, something in this technique scares the Borland compilers (5.5
    /// and 5.51) into Internal compiler errors so the s_init variable in
    /// _get_systeminfo() is int rather than bool when compiling for borland.
    static SYSTEM_INFO &_get_systeminfo()
    {
        static SYSTEM_INFO  s_systeminfo;
#ifdef __STLSOFT_COMPILER_IS_BORLAND
        /* WSCB: Borland has an internal compiler error if use ws_bool_t */
        static ws_int_t     s_init = (::GetSystemInfo(&s_systeminfo), ws_true_v);
#else
        static ws_bool_t    s_init = (::GetSystemInfo(&s_systeminfo), ws_true_v);
#endif /* __STLSOFT_COMPILER_IS_BORLAND */

        STLSOFT_SUPPRESS_UNUSED(s_init);

        return s_systeminfo;
    }
};

////////////////////////////////////////////////////////////////////////////////
// Unit-testing

#ifdef STLSOFT_UNITTEST

namespace unittest
{
    ss_bool_t test_winstl_system_info(unittest_reporter *r)
    {
        using stlsoft::unittest::unittest_initialiser;

        ss_bool_t               bSuccess    =   true;

        unittest_initialiser    init(r, "WinSTL", "system_info", __FILE__);

#if 0
        if(<<TODO>>)
        {
            r->report("<<TODO>> failed ", __LINE__);
            bSuccess = false;
        }
#endif /* 0 */

        return bSuccess;
    }

    unittest_registrar    unittest_winstl_system_info(test_winstl_system_info);

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

#endif /* WINSTL_INCL_H_WINSTL_SYSTEM_INFO */

/* ////////////////////////////////////////////////////////////////////////// */
