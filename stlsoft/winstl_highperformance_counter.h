/* /////////////////////////////////////////////////////////////////////////////
 * File:        winstl_highperformance_counter.h (formerly MWPrfCnt.h, ::SynesisWin)
 *
 * Purpose:     WinSTL high performance counter class.
 *
 * Created:     19th October 1998
 * Updated:     11th September 2004
 *
 * Home:        http://stlsoft.org/
 *
 * Copyright (c) 1998-2004, Matthew Wilson and Synesis Software
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


/// \file winstl_highperformance_counter.h
///
/// WinSTL high performance counter class.

#ifndef WINSTL_INCL_H_WINSTL_HIGHPERFORMANCE_COUNTER
#define WINSTL_INCL_H_WINSTL_HIGHPERFORMANCE_COUNTER

#ifndef __STLSOFT_DOCUMENTATION_SKIP_SECTION
# define WINSTL_VER_H_WINSTL_HIGHPERFORMANCE_COUNTER_MAJOR      2
# define WINSTL_VER_H_WINSTL_HIGHPERFORMANCE_COUNTER_MINOR      0
# define WINSTL_VER_H_WINSTL_HIGHPERFORMANCE_COUNTER_REVISION   1
# define WINSTL_VER_H_WINSTL_HIGHPERFORMANCE_COUNTER_EDIT       50
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
# include "winstl.h"                        // Include the WinSTL root header
#endif /* !WINSTL_INCL_H_WINSTL */

#if defined(__STLSOFT_COMPILER_IS_MSVC) && \
    _MSC_VER < 1200
# error winstl_highperformance_counter.h is not compatible with Visual C++ 5.0 or earlier
#endif /* _MSC_VER < 1200 */

#ifndef STLSOFT_INCL_H_STLSOFT_LIMIT_TRAITS
# include "stlsoft_limit_traits.h"          // stlsoft::limit_traits<ss_uint64_t>::maximum()
#endif /* !STLSOFT_INCL_H_STLSOFT_LIMIT_TRAITS */
#ifndef STLSOFT_INCL_H_STLSOFT_STATIC_INITIALISERS
# include "stlsoft_static_initialisers.h"   // class_constructor
#endif /* STLSOFT_INCL_H_STLSOFT_STATIC_INITIALISERS */
#ifndef STLSOFT_INCL_H_STLSOFT_SAP_CAST
# include "stlsoft_sap_cast.h"              // sap_cast
#endif /* STLSOFT_INCL_H_STLSOFT_SAP_CAST */

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

// class highperformance_counter
/// \brief A performance counter that uses the high performance hardware counter on the host machine
///
/// This class provides high-resolution performance monitoring using the host machine's high performance
/// hardware counter. This class does not provide meaningful timing information on operating systems
/// that do not provide a high performance hardware counter.
class highperformance_counter
{
public:
    typedef highperformance_counter     class_type;

private:
    typedef ws_sint64_t                 epoch_type;
public:
    /// \brief The interval type
    ///
    /// The type of the interval measurement, a 64-bit signed integer
    typedef ws_sint64_t                 interval_type;

/// Construction
//@{
public:
    static void class_init()
    {
        class_type().start();
    }
    static void class_uninit()
    {}
//@}

// Operations
public:
    /// \brief Starts measurement
    ///
    /// Begins the measurement period
    void        start();
    /// \brief Ends measurement
    ///
    /// Ends the measurement period
    void        stop();

// Attributes
public:
    /// \brief The elapsed count in the measurement period
    ///
    /// This represents the extent, in machine-specific increments, of the measurement period
    interval_type   get_period_count() const;
    /// \brief The number of whole seconds in the measurement period
    ///
    /// This represents the extent, in whole seconds, of the measurement period
    interval_type   get_seconds() const;
    /// \brief The number of whole milliseconds in the measurement period
    ///
    /// This represents the extent, in whole milliseconds, of the measurement period
    interval_type   get_milliseconds() const;
    /// \brief The number of whole microseconds in the measurement period
    ///
    /// This represents the extent, in whole microseconds, of the measurement period
    interval_type   get_microseconds() const;

// Implementation
private:
    static interval_type    frequency_();
    static interval_type    query_frequency_();

// Members
private:
    epoch_type  m_start;    // start of measurement period
    epoch_type  m_end;      // End of measurement period
};

/* ////////////////////////////////////////////////////////////////////////// */

#if !defined(__STLSOFT_COMPILER_IS_DMC)
static stlsoft_ns_qual(class_constructor)<highperformance_counter>  s_highperformance_counter_class_constructor(&highperformance_counter::class_init, NULL);
#endif /* !__STLSOFT_COMPILER_IS_DMC */

/* /////////////////////////////////////////////////////////////////////////////
 * Unit-testing
 */

#ifdef STLSOFT_UNITTEST

namespace unittest
{
    ss_bool_t test_winstl_highperformance_counter(unittest_reporter *r)
    {
        using stlsoft::unittest::unittest_initialiser;

        ss_bool_t               bSuccess    =   true;

        unittest_initialiser    init(r, "WinSTL", "highperformance_counter", __FILE__);

        highperformance_counter counter_1;
        highperformance_counter counter_2;

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

    unittest_registrar    unittest_winstl_highperformance_counter(test_winstl_highperformance_counter);

} // namespace winstl_test

#endif /* STLSOFT_UNITTEST */

/* /////////////////////////////////////////////////////////////////////////////
 * Implementation
 */

inline /* static */ highperformance_counter::interval_type highperformance_counter::query_frequency_()
{
    interval_type   frequency;

    // If no high-performance counter is available ...
    if( !::QueryPerformanceFrequency(sap_cast<LARGE_INTEGER*>(&frequency)) ||
        frequency == 0)
    {
        // ... then set the divisor to be the maximum value, guaranteeing that
        // the timed periods will always evaluate to 0.
        frequency = stlsoft_ns_qual(limit_traits)<interval_type>::maximum();
    }

    return frequency;
}

inline /* static */ highperformance_counter::interval_type highperformance_counter::frequency_()
{
#if defined(__STLSOFT_COMPILER_IS_BORLAND)
    interval_type           s_frequency = query_frequency_();
#else /* ? __STLSOFT_COMPILER_IS_BORLAND */
    static interval_type    s_frequency = query_frequency_();
#endif /* __STLSOFT_COMPILER_IS_BORLAND */

    stlsoft_assert(0 != s_frequency);

    return s_frequency;
}

// Operations
inline void highperformance_counter::start()
{
    ::QueryPerformanceCounter(sap_cast<LARGE_INTEGER*>(&m_start));
}

inline void highperformance_counter::stop()
{
    ::QueryPerformanceCounter(sap_cast<LARGE_INTEGER*>(&m_end));
}

// Attributes
inline highperformance_counter::interval_type highperformance_counter::get_period_count() const
{
    return static_cast<interval_type>(m_end - m_start);
}

inline highperformance_counter::interval_type highperformance_counter::get_seconds() const
{
    return get_period_count() / frequency_();
}

inline highperformance_counter::interval_type highperformance_counter::get_milliseconds() const
{
    highperformance_counter::interval_type  result;
    highperformance_counter::interval_type  count   =   get_period_count();

    if(count < __STLSOFT_GEN_SINT64_SUFFIX(0x20C49BA5E353F7))
    {
        result = (count * interval_type(1000)) / frequency_();
    }
    else
    {
        result = (count / frequency_()) * interval_type(1000);
    }

    return result;
}

inline highperformance_counter::interval_type highperformance_counter::get_microseconds() const
{
    highperformance_counter::interval_type  result;
    highperformance_counter::interval_type  count   =   get_period_count();

    if(count < __STLSOFT_GEN_SINT64_SUFFIX(0x8637BD05AF6))
    {
        result = (count * interval_type(1000000)) / frequency_();
    }
    else
    {
        result = (count / frequency_()) * interval_type(1000000);
    }

    return result;
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

#endif /* !WINSTL_INCL_H_WINSTL_HIGHPERFORMANCE_COUNTER */

/* ////////////////////////////////////////////////////////////////////////// */
