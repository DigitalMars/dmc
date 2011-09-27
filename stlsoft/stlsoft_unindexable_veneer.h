/* /////////////////////////////////////////////////////////////////////////////
 * File:        stlsoft_unindexable_veneer.h
 *
 * Purpose:     Contains the unindexable_veneer template class.
 *
 * Created:     9th July 2004
 * Updated:     11th September 2004
 *
 * Home:        http://stlsoft.org/
 *
 * Copyright (c) 2004, Matthew Wilson and Synesis Software
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


/// \file stlsoft_unindexable_veneer.h
///
/// Contains the unindexable_veneer template class.

#ifndef STLSOFT_INCL_H_STLSOFT_UNINDEXABLE_VENEER
#define STLSOFT_INCL_H_STLSOFT_UNINDEXABLE_VENEER

#ifndef __STLSOFT_DOCUMENTATION_SKIP_SECTION
# define STLSOFT_VER_H_STLSOFT_UNINDEXABLE_VENEER_MAJOR     2
# define STLSOFT_VER_H_STLSOFT_UNINDEXABLE_VENEER_MINOR     0
# define STLSOFT_VER_H_STLSOFT_UNINDEXABLE_VENEER_REVISION  1
# define STLSOFT_VER_H_STLSOFT_UNINDEXABLE_VENEER_EDIT      3
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

// class unindexable_pointer

template<   ss_typename_param_k T
        >
class unindexable_pointer
{
public:
    typedef T           value_type;

public:
    unindexable_pointer(value_type *p)
        : m_p(p)
    {}

    operator value_type *()
    {
        return m_p;
    }

private:
    void operator [](size_t );
    void operator [](size_t ) const;

    void operator +(ptrdiff_t );
    void operator -(ptrdiff_t );

private:
    value_type  *m_p;
};

template<   ss_typename_param_k T
        >
class unindexable_const_pointer
{
public:
    typedef T           value_type;

public:
    unindexable_const_pointer(value_type const *p)
        : m_p(p)
    {}

    operator value_type const *() const
    {
        return m_p;
    }

private:
    void operator [](size_t );
    void operator [](size_t ) const;

    void operator +(ptrdiff_t );
    void operator -(ptrdiff_t );

private:
    value_type const    *m_p;
};

// class unindexable_veneer

template<   ss_typename_param_k T
        >
class unindexable_veneer
    : public T
{
public:
    /// The veneer type
    typedef T                               parent_class_type;
    /// The type of the current parameterisation
    typedef unindexable_veneer<T>           class_type;
    /// The dereferenced type
    typedef unindexable_pointer<T>          dereferenced_type;
    /// The dereferenced type
//  typedef unindexable_const_pointer<T>    dereferenced_const_type;
    typedef unindexable_pointer<const T>    dereferenced_const_type;

public:
    /// Default constructor
    unindexable_veneer()
    {}
    /// Copy constructor
    ss_explicit_k unindexable_veneer(class_type const &rhs)
        : parent_class_type(rhs)
    {}
    /// Conversion constructor
    ss_explicit_k unindexable_veneer(parent_class_type const &rhs)
        : parent_class_type(rhs)
    {}

    dereferenced_type operator &()
    {
        return dereferenced_type(this);
    }
    dereferenced_const_type operator &() const
    {
        return dereferenced_const_type(this);
    }
};

/* ////////////////////////////////////////////////////////////////////////// */

#ifndef _STLSOFT_NO_NAMESPACE
} // namespace stlsoft
#endif /* _STLSOFT_NO_NAMESPACE */

/* ////////////////////////////////////////////////////////////////////////// */

#endif /* !STLSOFT_INCL_H_STLSOFT_UNINDEXABLE_VENEER */

/* ////////////////////////////////////////////////////////////////////////// */
