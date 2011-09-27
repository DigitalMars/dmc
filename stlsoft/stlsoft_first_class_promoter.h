/* /////////////////////////////////////////////////////////////////////////////
 * File:        stlsoft_first_class_promoter.h
 *
 * Purpose:     Class to convert built-in & aggregate types to 1st class types.
 *
 * Created:     8th September 2002
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


/// \file stlsoft_first_class_promoter.h
///
/// Class to convert built-in & aggregate types to 1st class types.

#ifndef STLSOFT_INCL_H_STLSOFT_FIRST_CLASS_PROMOTER
#define STLSOFT_INCL_H_STLSOFT_FIRST_CLASS_PROMOTER

#ifndef __STLSOFT_DOCUMENTATION_SKIP_SECTION
# define STLSOFT_VER_H_STLSOFT_FIRST_CLASS_PROMOTER_MAJOR       2
# define STLSOFT_VER_H_STLSOFT_FIRST_CLASS_PROMOTER_MINOR       0
# define STLSOFT_VER_H_STLSOFT_FIRST_CLASS_PROMOTER_REVISION    1
# define STLSOFT_VER_H_STLSOFT_FIRST_CLASS_PROMOTER_EDIT        26
#endif /* !__STLSOFT_DOCUMENTATION_SKIP_SECTION */

/* /////////////////////////////////////////////////////////////////////////////
 * Includes
 */

#ifndef STLSOFT_INCL_H_STLSOFT
# include "stlsoft.h"               // Include the STLSoft root header
#endif /* !STLSOFT_INCL_H_STLSOFT */
#ifndef STLSOFT_INCL_H_CONSTRAINTS
# include "stlsoft_constraints.h"   // stlsoft::must_be_pod
#endif /* !STLSOFT_INCL_H_CONSTRAINTS */

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

// class first_class_promoter

/// Promotes a basic type to a first-class type
///
/// This type can be used to promote a basic type (e.g. <code>int</code>) or a type that
/// cannot be used as the base of an inheritance relationship (such as a union)
/// to first class status.
///
/// \param T The basic type
template <ss_typename_param_k T>
class first_class_promoter
{
public:
    /// The value type
    typedef T                       value_type;
    /// The type of the current parameterisation
    typedef first_class_promoter<T> class_type;
    /// The pointer type
    typedef T                       *pointer;
    /// The non-mutating (const) pointer type
    typedef T const                 *const_pointer;
    /// The reference type
    typedef T                       &reference;
    /// The non-mutating (const) reference type
    typedef T const                 &const_reference;

// Construction
public:
    /// Default constructor
    ///
    /// \note The internal member of the \c value_type is <i><b>not</b></i> initialised, for efficiency
    first_class_promoter()
    {
        // Note that m_value is NOT initialised. (Efficiency)
    }

    /// Copy constructor
    first_class_promoter(class_type const &rhs)
        : m_value(rhs.m_value)
    {}

    /// Initialise an instance from an instance of the promoted type
#ifdef __STLSOFT_CF_MEMBER_TEMPLATE_CTOR_SUPPORT
    template <ss_typename_param_k U>
    ss_explicit_k first_class_promoter(U &value)
        : m_value(value)
    {}
#else
    ss_explicit_k first_class_promoter(value_type const &value)
        : m_value(value)
    {}
#endif // __STLSOFT_CF_MEMBER_TEMPLATE_CTOR_SUPPORT

    /// Destructor
    ~first_class_promoter() stlsoft_throw_0()
    {
        // This class shouldn't be used for 1st class types, so constrain to
        // non-class types, or trivial class types.
        //
        // It is put in a static assert merely to persuade recalcitrant
        // compilers to desist
        stlsoft_constraint_must_be_pod(value_type);

        // Check the assumption that this veneer is of zero size. The runtime
        // assert is included for those compilers that do not implement
        // compile-time asserts.
        stlsoft_static_assert(sizeof(class_type) == sizeof(value_type));
#if defined(__STLSOFT_COMPILER_IS_WATCOM)
        stlsoft_assert(sizeof(class_type) == sizeof(value_type));
#else
        stlsoft_message_assert("first_class_promoter used for inappropriate type", sizeof(class_type) == sizeof(value_type));
#endif /* !__STLSOFT_COMPILER_IS_WATCOM */
    }

    /// Copy assignment operator
    class_type & operator =(class_type const &rhs)
    {
        m_value = rhs.m_value;

        return *this;
    }

    /// Assignment operator, taking an instance of the promoted type
    class_type & operator =(value_type const &value)
    {
        m_value = value;

        return *this;
    }

// Accessors
public:
    /// Provides a non-mutable (const) reference to the promoted type member
    const_reference base_type_value() const
    {
        return m_value;
    }
    /// Provides a mutable reference to the promoted type member
    reference base_type_value()
    {
        return m_value;
    }

// Operators
public:
    /// Implicit conversion operator to a reference to the promoted type member
    operator reference()
    {
        return m_value;
    }

    /// Implicit conversion operator to a non-mutable (const) reference to the promoted type member
    operator const_reference() const
    {
        return m_value;
    }

    /// Address-of operator, providing pointer access to the promoted type member
    pointer operator &()
    {
        return &m_value;
    }

    /// Address-of operator, providing non-mutable (const) pointer access to the promoted type member
    const_pointer operator &() const
    {
        return &m_value;
    }

// Members
private:
    T   m_value;
};

/* ////////////////////////////////////////////////////////////////////////// */

#ifndef _STLSOFT_NO_NAMESPACE
} // namespace stlsoft
#endif /* _STLSOFT_NO_NAMESPACE */

/* ////////////////////////////////////////////////////////////////////////// */

#endif /* !STLSOFT_INCL_H_STLSOFT_FIRST_CLASS_PROMOTER */

/* ////////////////////////////////////////////////////////////////////////// */
