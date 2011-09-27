/* /////////////////////////////////////////////////////////////////////////////
 * File:        stlsoft_explicit_inheritance_veneer.h
 *
 * Purpose:     Explicit inheritance veneer class.
 *
 * Created:     13th January 2003
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


/// \file stlsoft_explicit_inheritance_veneer.h
///
/// Explicit inheritance veneer class.

#ifndef STLSOFT_INCL_H_STLSOFT_EXPLICIT_INHERITANCE_VENEER
#define STLSOFT_INCL_H_STLSOFT_EXPLICIT_INHERITANCE_VENEER

#ifndef __STLSOFT_DOCUMENTATION_SKIP_SECTION
# define STLSOFT_VER_H_STLSOFT_EXPLICIT_INHERITANCE_VENEER_MAJOR        2
# define STLSOFT_VER_H_STLSOFT_EXPLICIT_INHERITANCE_VENEER_MINOR        0
# define STLSOFT_VER_H_STLSOFT_EXPLICIT_INHERITANCE_VENEER_REVISION     1
# define STLSOFT_VER_H_STLSOFT_EXPLICIT_INHERITANCE_VENEER_EDIT         17
#endif /* !__STLSOFT_DOCUMENTATION_SKIP_SECTION */

/* /////////////////////////////////////////////////////////////////////////////
 * Includes
 */

#ifndef STLSOFT_INCL_H_STLSOFT
# include "stlsoft.h"               // Include the STLSoft root header
#endif /* !STLSOFT_INCL_H_STLSOFT */
#ifndef STLSOFT_INCL_H_STLSOFT_CONSTRAINTS
# include "stlsoft_constraints.h"   // must_be_same_size
#endif /* !STLSOFT_INCL_H_STLSOFT_CONSTRAINTS */

/* /////////////////////////////////////////////////////////////////////////////
 * Namespace
 */

#ifndef _STLSOFT_NO_NAMESPACE
namespace stlsoft
{
#endif /* _STLSOFT_NO_NAMESPACE */

/* ////////////////////////////////////////////////////////////////////////////
 * Classes
 */


// class explicit_inheritance_veneer

/// Applies a \c protected veneer over a type, which can be accessed only
/// through conversions to the veneered type
///
/// \param T The type to be subject to the veneer
template <ss_typename_param_k T>
struct explicit_inheritance_veneer
    : protected T
{
public:
    /// The base class type
    typedef T   base_type;

// Conversion
public:
    /// The conversion operator that provides access to the base type
    base_type const *operator &() const
    {
        stlsoft_constraint_must_be_same_size(T, explicit_inheritance_veneer<T>);

        return this;
    }

// Implementation
protected:
    /// Access a mutable reference to the base type
    base_type &base_type_value()
    {
        return *this;
    }
    /// Access a non-mutable reference to the base type
    base_type const &base_type_value() const
    {
        return *this;
    }
};

////////////////////////////////////////////////////////////////////////////////
// Unit-testing

#ifdef STLSOFT_UNITTEST

namespace unittest
{
    ss_bool_t test_stlsoft_explicit_inheritance_veneer(unittest_reporter *r)
    {
        ss_bool_t               bSuccess    =   true;

        unittest_initialiser    init(r, "STLSoft", "explicit_inheritance_veneer", __FILE__);

#if 0
        if(<<TODO>>)
        {
            r->report("<<TODO>> failed ", __LINE__);
            bSuccess = false;
        }
#endif /* 0 */

        return bSuccess;
    }

    unittest_registrar    unittest_stlsoft_explicit_inheritance_veneer(test_stlsoft_explicit_inheritance_veneer);

} // namespace unittest

#endif /* STLSOFT_UNITTEST */

/* ////////////////////////////////////////////////////////////////////////// */

#ifndef _STLSOFT_NO_NAMESPACE
} // namespace stlsoft
#endif /* _STLSOFT_NO_NAMESPACE */

/* ////////////////////////////////////////////////////////////////////////// */

#endif /* !STLSOFT_INCL_H_STLSOFT_EXPLICIT_INHERITANCE_VENEER */

/* ////////////////////////////////////////////////////////////////////////// */
