/* /////////////////////////////////////////////////////////////////////////////
 * File:        winstl_systemtime_counter.h (formerly MWSysTmr.h, ::SynesisWin)
 *
 * Purpose:     WinSTL system-time performance counter class.
 *
 * Created:     22nd March 2002
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


/// \file winstl_systemtime_counter.h
///
/// WinSTL system-time performance counter class.

#ifndef WINSTL_INCL_H_WINSTL_SYSTEMTIME_COUNTER
#define WINSTL_INCL_H_WINSTL_SYSTEMTIME_COUNTER

#ifndef __STLSOFT_DOCUMENTATION_SKIP_SECTION
# define WINSTL_VER_H_WINSTL_SYSTEMTIME_COUNTER_MAJOR       2
# define WINSTL_VER_H_WINSTL_SYSTEMTIME_COUNTER_MINOR       0
# define WINSTL_VER_H_WINSTL_SYSTEMTIME_COUNTER_REVISION    1
# define WINSTL_VER_H_WINSTL_SYSTEMTIME_COUNTER_EDIT        26
#endif /* !__STLSOFT_DOCUMENTATION_SKIP_SECTION */

/* /////////////////////////////////////////////////////////////////////////////
 * Includes
 */

#ifndef WINSTL_INCL_H_WINSTL
#include "winstl.h" // Include the WinSTL root header
#endif /* !WINSTL_INCL_H_WINSTL */

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

/// \weakgroup libraries_performance Performance Library
/// \ingroup libraries
/// \brief This library provides performance measuring facilities

/// \weakgroup winstl_perf_library Performance Library (WinSTL)
/// \ingroup WinSTL libraries_performance
/// \brief This library provides performance measuring facilities for the Win32 API
/// @{

/* /////////////////////////////////////////////////////////////////////////////
 * Classes
 */

/// Performance counter that uses the Windows <code>GetSystemTime()</code> function
class systemtime_counter
{
public:
    typedef systemtime_counter  class_type;

    typedef ws_sint64_t         epoch_type;
    typedef ws_sint64_t         interval_type;

// Construction
public:
    systemtime_counter();

// Operations
public:
    void        start();
    void        stop();

// Attributes
public:
    interval_type   get_period_count() const;
    interval_type   get_seconds() const;
    interval_type   get_milliseconds() const;
    interval_type   get_microseconds() const;

// Members
protected:
    epoch_type  m_start;
    epoch_type  m_end;
};

////////////////////////////////////////////////////////////////////////////////
// Unit-testing

#ifdef STLSOFT_UNITTEST

namespace unittest
{
    ss_bool_t test_winstl_systemtime_counter(unittest_reporter *r)
    {
        using stlsoft::unittest::unittest_initialiser;

        ss_bool_t               bSuccess    =   true;

        unittest_initialiser    init(r, "WinSTL", "systemtime_counter", __FILE__);

        systemtime_counter counter_1;
        systemtime_counter counter_2;

        /* Check that the periods of two enclosing measured intervals are enclosed */
        counter_1.start();
        counter_2.start();
        counter_2.stop();
        counter_1.stop();

        if(counter_1.get_period_count() < counter_2.get_period_count())
        {
            r->report("Enclosed interval is reported longer than enclosing interval", __LINE__);
            bSuccess = false;
        }

        /* Check that the relationships between s, ms & us are valid */

        if( counter_1.get_seconds()      * 1000 != counter_1.get_milliseconds() &&
            counter_1.get_milliseconds() / 1000 != counter_1.get_seconds())
        {
            r->report("get_seconds() and get_milliseconds() report contradictory values", __LINE__);
            bSuccess = false;
        }

        if( counter_1.get_milliseconds() * 1000 != counter_1.get_microseconds() &&
            counter_1.get_microseconds() / 1000 != counter_1.get_milliseconds())
        {
            r->report("get_milliseconds() and get_microseconds() report contradictory values", __LINE__);
            bSuccess = false;
        }

        return bSuccess;
    }

    unittest_registrar    unittest_winstl_systemtime_counter(test_winstl_systemtime_counter);

} // namespace unittest

#endif /* STLSOFT_UNITTEST */

////////////////////////////////////////////////////////////////////////////////
// Implementation

inline systemtime_counter::systemtime_counter()
{
    // Note that the constructor does nothing, for performance reasons. Calling
    // any of the Attribute methods before having gone through a start()-stop()
    // cycle will yield undefined results.
}

// Operations
inline void systemtime_counter::start()
{
    ::GetSystemTimeAsFileTime(reinterpret_cast<LPFILETIME>(&m_start));
}

inline void systemtime_counter::stop()
{
    ::GetSystemTimeAsFileTime(reinterpret_cast<LPFILETIME>(&m_end));
}

// Attributes
inline systemtime_counter::interval_type systemtime_counter::get_period_count() const
{
    return static_cast<interval_type>(m_end - m_start);
}

inline systemtime_counter::interval_type systemtime_counter::get_seconds() const
{
    return get_period_count() / interval_type(10000000);
}

inline systemtime_counter::interval_type systemtime_counter::get_milliseconds() const
{
    return get_period_count() / interval_type(10000);
}

inline systemtime_counter::interval_type systemtime_counter::get_microseconds() const
{
    return get_period_count() / interval_type(10);
}

/* ////////////////////////////////////////////////////////////////////////// */

/// @} // end of group winstl_perf_library

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

#endif /* !WINSTL_INCL_H_WINSTL_SYSTEMTIME_COUNTER */

/* ////////////////////////////////////////////////////////////////////////// */
