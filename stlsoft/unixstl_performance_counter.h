/* /////////////////////////////////////////////////////////////////////////////
 * File:        unixstl_performance_counter.h
 *
 * Purpose:     performance_counter class.
 *
 * Created:     16th January 2002
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


/// \file unixstl_performance_counter.h
///
/// performance_counter class.

#ifndef UNIXSTL_INCL_H_UNIXSTL_PERFORMANCE_COUNTER
#define UNIXSTL_INCL_H_UNIXSTL_PERFORMANCE_COUNTER

#ifndef __STLSOFT_DOCUMENTATION_SKIP_SECTION
# define UNIXSTL_VER_H_UNIXSTL_PERFORMANCE_COUNTER_MAJOR    2
# define UNIXSTL_VER_H_UNIXSTL_PERFORMANCE_COUNTER_MINOR    0
# define UNIXSTL_VER_H_UNIXSTL_PERFORMANCE_COUNTER_REVISION 1
# define UNIXSTL_VER_H_UNIXSTL_PERFORMANCE_COUNTER_EDIT     34
#endif /* !__STLSOFT_DOCUMENTATION_SKIP_SECTION */

/* /////////////////////////////////////////////////////////////////////////////
 * Includes
 */

#ifndef UNIXSTL_INCL_H_UNIXSTL
# include "unixstl.h"   // Include the UNIXSTL root header
#endif /* !UNIXSTL_INCL_H_UNIXSTL */

#include <sys/time.h>

/* /////////////////////////////////////////////////////////////////////////////
 * Namespace
 *
 * The UNIXSTL components are contained within the unixstl namespace. This is
 * actually an alias for stlsoft::unixstl_project,
 *
 * The definition matrix is as follows:
 *
 * _STLSOFT_NO_NAMESPACE    _UNIXSTL_NO_NAMESPACE   unixstl definition
 * ---------------------    ---------------------   -----------------
 *  not defined              not defined             = stlsoft::unixstl_project
 *  not defined              defined                 not defined
 *  defined                  not defined             unixstl
 *  defined                  defined                 not defined
 *
 */

/* No STLSoft namespaces means no UNIXSTL namespaces */
#ifdef _STLSOFT_NO_NAMESPACES
# define _UNIXSTL_NO_NAMESPACES
#endif /* _STLSOFT_NO_NAMESPACES */

/* No UNIXSTL namespaces means no unixstl namespace */
#ifdef _UNIXSTL_NO_NAMESPACES
# define _UNIXSTL_NO_NAMESPACE
#endif /* _UNIXSTL_NO_NAMESPACES */

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

/// \weakgroup libraries STLSoft Libraries
/// \brief The individual libraries

/// \weakgroup libraries_performance Performance Library
/// \ingroup libraries
/// \brief This library provides performance measuring facilities

/// \weakgroup unixstl_perf_library Performance Library (UNIXSTL)
/// \ingroup UNIXSTL libraries_performance
/// \brief This library provides performance measuring facilities for UNIX
/// @{

/* /////////////////////////////////////////////////////////////////////////////
 * Classes
 */

/// \brief A performance counter using \c gettimeofday()
///
/// This class provides performance monitoring functionality based around the
/// UNIX \c gettimeofday() API.

class performance_counter
{
private:
    typedef us_sint64_t         epoch_type;
public:
    typedef performance_counter class_type;
    /// \brief The interval type
    ///
    /// The type of the interval measurement, a 64-bit signed integer
    typedef us_sint64_t         interval_type;

// Construction
public:

// Operations
public:
    /// \brief Starts measurement
    ///
    /// Begins the measurement period
    void start();
    /// \brief Ends measurement
    ///
    /// Ends the measurement period
    void stop();

// Attributes
public:
    /// \brief The elapsed count in the measurement period
    ///
    /// This represents the extent, in arbitrary units, of the measurement period
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

// Members
private:
    typedef struct timeval timeval_t;

    timeval_t   m_start;
    timeval_t   m_end;
};

////////////////////////////////////////////////////////////////////////////////
// Unit-testing

#ifdef STLSOFT_UNITTEST

namespace unittest
{
    ss_bool_t test_unixstl_performance_counter(unittest_reporter *r)
    {
        using stlsoft::unittest::unittest_initialiser;

        ss_bool_t               bSuccess    =   true;

        unittest_initialiser    init(r, "UNIXSTL", "performance_counter", __FILE__);

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

    unittest_registrar    unittest_unixstl_performance_counter(test_unixstl_performance_counter);

} // namespace unittest

#endif /* STLSOFT_UNITTEST */

////////////////////////////////////////////////////////////////////////////////
// Implementation

inline void performance_counter::start()
{
    struct timezone tz;

    gettimeofday(&m_start, &tz);

    m_end   =   m_start;
}

inline void performance_counter::stop()
{
    struct timezone tz;

    gettimeofday(&m_end, &tz);
}

inline performance_counter::interval_type performance_counter::get_period_count() const
{
    return get_microseconds();
}

inline performance_counter::interval_type performance_counter::get_seconds() const
{
    interval_type   start   =   m_start.tv_sec + m_start.tv_usec / (1000 * 1000);
    interval_type   end     =   m_end.tv_sec   + m_end.tv_usec   / (1000 * 1000);

    return end - start;
}

inline performance_counter::interval_type performance_counter::get_milliseconds() const
{
    interval_type   start   =   m_start.tv_sec * 1000 + m_start.tv_usec / 1000;
    interval_type   end     =   m_end.tv_sec   * 1000 + m_end.tv_usec   / 1000;

    return end - start;
}

inline performance_counter::interval_type performance_counter::get_microseconds() const
{
    interval_type   start   =   m_start.tv_sec * 1000 * 1000 + m_start.tv_usec;
    interval_type   end     =   m_end.tv_sec   * 1000 * 1000 + m_end.tv_usec;

    return end - start;
}

/* ////////////////////////////////////////////////////////////////////////// */

/// @} // end of group unixstl_perf_library

/* ////////////////////////////////////////////////////////////////////////// */

#ifndef _UNIXSTL_NO_NAMESPACE
# if defined(_STLSOFT_NO_NAMESPACE) || \
     defined(__STLSOFT_DOCUMENTATION_SKIP_SECTION)
} // namespace unixstl
# else
} // namespace unixstl_project
} // namespace stlsoft
# endif /* _STLSOFT_NO_NAMESPACE */
#endif /* !_UNIXSTL_NO_NAMESPACE */

/* ////////////////////////////////////////////////////////////////////////// */

#endif /* !UNIXSTL_INCL_H_UNIXSTL_PERFORMANCE_COUNTER */

/* ////////////////////////////////////////////////////////////////////////// */
