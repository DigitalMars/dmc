/* /////////////////////////////////////////////////////////////////////////////
 * File:        winstl_performance_counter_scope.h
 *
 * Purpose:     WinSTL performance counter scoping class.
 *
 * Created:     20th July 2002
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


/// \file winstl_performance_counter_scope.h
///
/// WinSTL performance counter scoping class.

#ifndef WINSTL_INCL_H_WINSTL_PERFORMANCE_COUNTER_SCOPE
#define WINSTL_INCL_H_WINSTL_PERFORMANCE_COUNTER_SCOPE

#ifndef __STLSOFT_DOCUMENTATION_SKIP_SECTION
# define WINSTL_VER_H_WINSTL_PERFORMANCE_COUNTER_SCOPE_MAJOR    2
# define WINSTL_VER_H_WINSTL_PERFORMANCE_COUNTER_SCOPE_MINOR    0
# define WINSTL_VER_H_WINSTL_PERFORMANCE_COUNTER_SCOPE_REVISION 1
# define WINSTL_VER_H_WINSTL_PERFORMANCE_COUNTER_SCOPE_EDIT     26
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

// class performance_counter_scope
/// Scopes the measurement period for a performance counter
///
/// \param C The performance counter type
template <ss_typename_param_k C>
class performance_counter_scope
{
public:
    /// The counter type
    typedef C                               counter_type;
    /// The class type
    typedef performance_counter_scope<C>    class_type;

public:
    /// Constructs with the performance counter instance to manage, on which start() is called
    ss_explicit_k performance_counter_scope(counter_type &counter)
        : m_counter(counter)
    {
        m_counter.start();
    }
    /// Calls stop() on the managed counter
    ~performance_counter_scope()
    {
        m_counter.stop();
    }

    /// Calls stop() on the managed counter
    void stop()
    {
        m_counter.stop();
    }

    // This method is const, to ensure that only the stop operation (via
    // performance_counter_scope::stop()) is accessible on the managed counter.
    /// Returns a non-mutable (const) reference to the managed counter
    const counter_type &get_counter() const
    {
        return m_counter;
    }

// Members
private:
    C   &m_counter;

// Not to be implemented
private:
    performance_counter_scope(class_type const &rhs);
    class_type const &operator =(class_type const &rhs);
};

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

#endif /* !WINSTL_INCL_H_WINSTL_PERFORMANCE_COUNTER_SCOPE */

/* ////////////////////////////////////////////////////////////////////////// */
