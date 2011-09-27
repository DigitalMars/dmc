/* /////////////////////////////////////////////////////////////////////////////
 * File:        stlsoft_placement_aid.h
 *
 * Purpose:     Contains the auto_destructor and auto_array_destructor template
 *              classes.
 *
 * Created:     9th January 2002
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


/// \file stlsoft_placement_aid.h
///
/// Contains the auto_destructor and auto_array_destructor template classes.

#ifndef STLSOFT_INCL_H_STLSOFT_PLACEMENT_AID
#define STLSOFT_INCL_H_STLSOFT_PLACEMENT_AID

#ifndef __STLSOFT_DOCUMENTATION_SKIP_SECTION
# define STLSOFT_VER_H_STLSOFT_PLACEMENT_AID_MAJOR      2
# define STLSOFT_VER_H_STLSOFT_PLACEMENT_AID_MINOR      0
# define STLSOFT_VER_H_STLSOFT_PLACEMENT_AID_REVISION   1
# define STLSOFT_VER_H_STLSOFT_PLACEMENT_AID_EDIT       15
#endif /* !__STLSOFT_DOCUMENTATION_SKIP_SECTION */

/* /////////////////////////////////////////////////////////////////////////////
 * Includes
 */

#ifndef STLSOFT_INCL_H_STLSOFT
# include "stlsoft.h"                // Include the STLSoft root header
#endif /* !STLSOFT_INCL_H_STLSOFT */
#ifndef STLSOFT_INCL_H_STLSOFT_CONSTRAINTS
# include "stlsoft_constraints.h"    // constraints
#endif /* !STLSOFT_INCL_H_STLSOFT_CONSTRAINTS */
#include <new>

/* /////////////////////////////////////////////////////////////////////////////
 * Namespace
 */

#ifndef _STLSOFT_NO_NAMESPACE
namespace stlsoft
{
#endif /* _STLSOFT_NO_NAMESPACE */

/* /////////////////////////////////////////////////////////////////////////////
 * Classes
 */

// class placement_aid

/// Scopes the lifetime of an in-place object
///
/// \param T The type of the in-place object
template <ss_typename_param_k T>
class placement_aid
{
public:
    /// The value type
    typedef T                   value_type;
    /// The type of the current parameterisation
    typedef placement_aid<T>    class_type;

// Construction
public:
    /// Create an instance of the \c value_type at the given location
    ///
    /// \param pv The location at which to in-place construct the object instance
    ss_explicit_k placement_aid(void *pv)
        : m_t(*new(pv) T())
    {}

    /// In-place destroy the object instance
    ~placement_aid()
    {
        m_t.~T();
    }

// Accessors
public:
    /// Implicit conversion operator to a reference to the in-place object instance
    operator T &()
    {
        return m_t;
    }
    /// Implicit conversion operator to a non-mutable (const) reference to the in-place object instance
    operator T const &() const
    {
        return m_t;
    }

    /// Address-of operator, providing pointer access to the in-place object instance
    T *operator &()
    {
        return &m_t;
    }
    /// Address-of operator, providing non-mutable (const) pointer access to the in-place object instance
    T const *operator &() const
    {
        return &m_t;
    }

// Members
private:
    T   &m_t;

// Not to be implemented
private:
    placement_aid(class_type const &);
    class_type const &operator =(class_type const &);
};

/* ////////////////////////////////////////////////////////////////////////// */

#ifndef _STLSOFT_NO_NAMESPACE
} // namespace stlsoft
#endif /* _STLSOFT_NO_NAMESPACE */

/* ////////////////////////////////////////////////////////////////////////// */

#endif /* !STLSOFT_INCL_H_STLSOFT_PLACEMENT_AID */

/* ////////////////////////////////////////////////////////////////////////// */
