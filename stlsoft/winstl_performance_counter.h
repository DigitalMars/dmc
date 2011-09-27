/* /////////////////////////////////////////////////////////////////////////////
 * File:        winstl_performance_counter.h
 *
 * Purpose:     WinSTL general performance counter class. This class attempts to
 *              use the built-in high-performance hardware counter if available,
 *              otherwise using the tick-count facilities.
 *
 * Created:     31st July 2002
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


/// \file winstl_performance_counter.h
///
/// WinSTL general performance counter class. This class attempts to use the built-in high-performance hardware counter if available, otherwise using the tick-count facilities.

#ifndef WINSTL_INCL_H_WINSTL_PERFORMANCE_COUNTER
#define WINSTL_INCL_H_WINSTL_PERFORMANCE_COUNTER

#ifndef __STLSOFT_DOCUMENTATION_SKIP_SECTION
# define WINSTL_VER_H_WINSTL_PERFORMANCE_COUNTER_MAJOR      2
# define WINSTL_VER_H_WINSTL_PERFORMANCE_COUNTER_MINOR      0
# define WINSTL_VER_H_WINSTL_PERFORMANCE_COUNTER_REVISION   1
# define WINSTL_VER_H_WINSTL_PERFORMANCE_COUNTER_EDIT       39
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
# error winstl_performance_counter.h is not compatible with Visual C++ 5.0 or earlier
#endif /* _MSC_VER < 1200 */

#ifndef STLSOFT_INCL_H_STLSOFT_LIMIT_TRAITS
# include "stlsoft_limit_traits.h"          // __STLSOFT_GEN_SINT64_SUFFIX
#endif /* !STLSOFT_INCL_H_STLSOFT_LIMIT_TRAITS */
#ifndef STLSOFT_INCL_H_STLSOFT_STATIC_INITIALISERS
# include "stlsoft_static_initialisers.h"   // class_constructor
#endif /* STLSOFT_INCL_H_STLSOFT_STATIC_INITIALISERS */
#ifndef STLSOFT_INCL_H_STLSOFT_SIGN_TRAITS
//# include "stlsoft_sign_traits.h"         // sign_traits
#endif /* STLSOFT_INCL_H_STLSOFT_SIGN_TRAITS */
#ifndef STLSOFT_CF_64BIT_INT_SUPPORT
# ifndef STLSOFT_INCL_H_STLSOFT_64BIT_INTEGERS
#  include "stlsoft_64bit_integers.h"       // sinteger64
# endif /* STLSOFT_INCL_H_STLSOFT_64BIT_INTEGERS */
#endif /* !STLSOFT_CF_64BIT_INT_SUPPORT */

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

// class performance_counter
/// \brief A performance counter that uses the most accurate measurement APIs available on the host machine
///
/// This class attempts to use the high performance hardware counter as its measurement resource, but failing
/// that it defaults to less accurate resources in order to guarantee that meaningful measurements are always
/// available to application code
class performance_counter
{
/// \name Member types
//@{
public:
    typedef performance_counter         class_type;
private:
#ifdef STLSOFT_CF_64BIT_INT_SUPPORT
    typedef ws_sint64_t                 epoch_type;
#else
    typedef sinteger64                  epoch_type;
#endif /* STLSOFT_CF_64BIT_INT_SUPPORT */
public:
    /// \brief The interval type
    ///
    /// The type of the interval measurement, a 64-bit signed integer
#ifdef STLSOFT_CF_64BIT_INT_SUPPORT
    typedef ws_sint64_t                 interval_type;
#else
    typedef sinteger64                  interval_type;
#endif /* STLSOFT_CF_64BIT_INT_SUPPORT */
//@}

/// \name Construction
//@{
public:
    static void class_init()
    {
        class_type  instance;

        instance.start();
    }
    static void class_uninit()
    {}
//@}

/// \name Operations
//@{
public:
    /// \brief Starts measurement
    ///
    /// Begins the measurement period
    void        start();
    /// \brief Ends measurement
    ///
    /// Ends the measurement period
    void        stop();
//@}

/// \name Attributes
//@{
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
//@}

/// \name Implementation
//@{
private:
    typedef void (*measure_fn_type)(epoch_type&);

    static interval_type    frequency_();
    static interval_type    query_frequency_();
    static void             qpc_(epoch_type &epoch);
    static void             gtc_(epoch_type &epoch);
    static measure_fn_type  get_measure_fn_();
    static void             measure_(epoch_type &epoch);
//@}

/// \name Members
//@{
private:
    epoch_type  m_start;    // start of measurement period
    epoch_type  m_end;      // End of measurement period
//@}
};

/* ////////////////////////////////////////////////////////////////////////// */

#if !defined(__STLSOFT_COMPILER_IS_DMC)
static stlsoft_ns_qual(class_constructor)<performance_counter>  s_performance_counter_class_constructor(&performance_counter::class_init, NULL);
#endif /* !__STLSOFT_COMPILER_IS_DMC */

/* /////////////////////////////////////////////////////////////////////////////
 * Unit-testing
 */

#ifdef STLSOFT_UNITTEST

namespace unittest
{
    ss_bool_t test_winstl_performance_counter(unittest_reporter *r)
    {
        using stlsoft::unittest::unittest_initialiser;

        ss_bool_t               bSuccess    =   true;

        unittest_initialiser    init(r, "WinSTL", "performance_counter", __FILE__);

        performance_counter counter_1;
        performance_counter counter_2;

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

    unittest_registrar    unittest_winstl_performance_counter(test_winstl_performance_counter);

} // namespace winstl_test

#endif /* STLSOFT_UNITTEST */

/* /////////////////////////////////////////////////////////////////////////////
 * Implementation
 */

inline /* static */ performance_counter::interval_type performance_counter::query_frequency_()
{
    interval_type   frequency;

    // If no high-performance counter is available ...
    if( !::QueryPerformanceFrequency(reinterpret_cast<LARGE_INTEGER*>(&frequency)) ||
        frequency == 0)
    {
        // ... then set the divisor to be the frequency for GetTickCount(), which is
        // 1000 since it returns intervals in milliseconds.
        frequency = 1000;
    }

    return frequency;
}

inline /* static */ performance_counter::interval_type performance_counter::frequency_()
{
#if defined(__STLSOFT_COMPILER_IS_BORLAND)
    interval_type           s_frequency = query_frequency_();
#else /* ? __STLSOFT_COMPILER_IS_BORLAND */
    static interval_type    s_frequency = query_frequency_();
#endif /* __STLSOFT_COMPILER_IS_BORLAND */

    stlsoft_assert(0 != s_frequency);

    return s_frequency;
}

inline /* static */ void performance_counter::qpc_(epoch_type &epoch)
{
    ::QueryPerformanceCounter(reinterpret_cast<LARGE_INTEGER*>(&epoch));
}

inline /* static */ void performance_counter::gtc_(epoch_type &epoch)
{
    epoch = static_cast<ws_sint32_t>(::GetTickCount());
}

inline /* static */ performance_counter::measure_fn_type performance_counter::get_measure_fn_()
{
    measure_fn_type fn;
    epoch_type      frequency;

    if(QueryPerformanceFrequency(reinterpret_cast<LARGE_INTEGER*>(&frequency)))
    {
        fn = qpc_;
    }
    else
    {
        fn = gtc_;
    }

    return fn;
}

inline /* static */ void performance_counter::measure_(epoch_type &epoch)
{
    static measure_fn_type  fn  =   get_measure_fn_();

    fn(epoch);
}

// Operations
inline void performance_counter::start()
{
    measure_(m_start);
}

inline void performance_counter::stop()
{
    measure_(m_end);
}

// Attributes
inline performance_counter::interval_type performance_counter::get_period_count() const
{
    return static_cast<interval_type>(m_end - m_start);
}

inline performance_counter::interval_type performance_counter::get_seconds() const
{
    return get_period_count() / frequency_();
}

inline performance_counter::interval_type performance_counter::get_milliseconds() const
{
    interval_type   result;
    interval_type   count   =   get_period_count();

#ifdef STLSOFT_CF_64BIT_INT_SUPPORT
    if(count < __STLSOFT_GEN_SINT64_SUFFIX(0x20C49BA5E353F7))
#else /* ? STLSOFT_CF_64BIT_INT_SUPPORT */
    if(count < interval_type(0x20C49B, 0xA5E353F7))
#endif /* !STLSOFT_CF_64BIT_INT_SUPPORT */
    {
        result = (count * interval_type(1000)) / frequency_();
    }
    else
    {
        result = (count / frequency_()) * interval_type(1000);
    }

    return result;
}

inline performance_counter::interval_type performance_counter::get_microseconds() const
{
    interval_type   result;
    interval_type   count   =   get_period_count();

#ifdef STLSOFT_CF_64BIT_INT_SUPPORT
    if(count < __STLSOFT_GEN_SINT64_SUFFIX(0x8637BD05AF6))
#else /* ? STLSOFT_CF_64BIT_INT_SUPPORT */
    if(count < interval_type(0x863, 0x7BD05AF6))
#endif /* !STLSOFT_CF_64BIT_INT_SUPPORT */
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

#endif /* !WINSTL_INCL_H_WINSTL_PERFORMANCE_COUNTER */

/* ////////////////////////////////////////////////////////////////////////// */
