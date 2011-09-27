/* /////////////////////////////////////////////////////////////////////////////
 * File:        stlsoft_proxy_ptr.h (originally MLBrwPtr.h, ::SynesisStd)
 *
 * Purpose:     Contains the proxy_ptr template class.
 *
 * Created:     17th January 1999
 * Updated:     11th September 2004
 *
 * Home:        http://stlsoft.org/
 *
 * Copyright (c) 1999-2004, Matthew Wilson and Synesis Software
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


/// \file stlsoft_proxy_ptr.h
///
/// Contains the proxy_ptr template class.

#ifndef STLSOFT_INCL_H_STLSOFT_PROXY_PTR
#define STLSOFT_INCL_H_STLSOFT_PROXY_PTR

#ifndef __STLSOFT_DOCUMENTATION_SKIP_SECTION
# define STLSOFT_VER_H_STLSOFT_PROXY_PTR_MAJOR      2
# define STLSOFT_VER_H_STLSOFT_PROXY_PTR_MINOR      0
# define STLSOFT_VER_H_STLSOFT_PROXY_PTR_REVISION   1
# define STLSOFT_VER_H_STLSOFT_PROXY_PTR_EDIT       48
#endif /* !__STLSOFT_DOCUMENTATION_SKIP_SECTION */

/* /////////////////////////////////////////////////////////////////////////////
 * Includes
 */

#ifndef STLSOFT_INCL_H_STLSOFT
# include "stlsoft.h"   // Include the STLSoft root header
#endif /* !STLSOFT_INCL_H_STLSOFT */

/* /////////////////////////////////////////////////////////////////////////////
 * Namespace
 */

#ifndef _STLSOFT_NO_NAMESPACE
namespace stlsoft
{
#endif /* _STLSOFT_NO_NAMESPACE */

/* /////////////////////////////////////////////////////////////////////////////
 * Forward declarations
 */

#ifndef __STLSOFT_DOCUMENTATION_SKIP_SECTION

template <ss_typename_param_k T>
class proxy_ptr;

#endif /* !__STLSOFT_DOCUMENTATION_SKIP_SECTION */

/* /////////////////////////////////////////////////////////////////////////////
 * Classes
 */

// class proxy_ptr
/// This class emulates a pointer in all respects, and is simply an aid to
/// self-documentation.
///
/// \param T The value type
template <ss_typename_param_k T>
class proxy_ptr
{
public:
    /// The value type
    typedef T                       value_type;
    /// The current parameterisation of the type
    typedef proxy_ptr<T>            class_type;

// Construction
public:
    /// Construct from a pointer to "borrow"
    ss_explicit_k proxy_ptr(value_type *t)
        : m_value(t)
    {}
    /// Assignment from a new pointer
    proxy_ptr &operator =(value_type *t)
    {
        m_value = t;

        return *this;
    }

// Conversion
public:
    /// Implicit conversion to pointer to the underlying pointer
    operator value_type *()
    {
        return m_value;
    }
    /// Implicit conversion to pointer-to-const to the underlying pointer
    operator const value_type *() const
    {
        return m_value;
    }

    /// Indirection operator
    ///
    /// \note This method does a debug-time assertion that the underlying pointer is non-null
    value_type &operator *() // indirection
    {
        stlsoft_message_assert("Dereferencing a null pointer!", NULL != m_value);

        return *m_value;
    }
    /// Indirection operator
    ///
    /// \note This method does a debug-time assertion that the underlying pointer is non-null
    value_type const &operator *() const // indirection
    {
        stlsoft_message_assert("Dereferencing a null pointer!", NULL != m_value);

        return *m_value;
    }
    /// Member-selection operator
    ///
    /// \note This method does a debug-time assertion that the underlying pointer is non-null
    value_type *operator ->() // member-selection
    {
        stlsoft_message_assert("Dereferencing a null pointer!", NULL != m_value);

        return m_value;
    }
    /// Member-selection operator
    ///
    /// \note This method does a debug-time assertion that the underlying pointer is non-null
    value_type const *operator ->() const // member-selection
    {
        stlsoft_message_assert("Dereferencing a null pointer!", NULL != m_value);

        return m_value;
    }

    /// Returns the underlying pointer value
    value_type *get_ptr()
    {
        return m_value;
    }
    /// Returns the underlying pointer value
    value_type const *get_ptr() const
    {
        return m_value;
    }

    /// Returns the underlying pointer value
    ///
    /// \deprecated
    value_type *GetPointer()
    {
        return m_value;
    }
    /// Returns the underlying pointer value
    ///
    /// \deprecated
    value_type const *GetPointer() const
    {
        return m_value;
    }

    /// Sets the underlying pointer value to null
    void clear()
    {
        m_value = NULL;
    }

// Members
private:
    value_type  *m_value;
};

/* ////////////////////////////////////////////////////////////////////////// */

#ifndef _STLSOFT_NO_NAMESPACE
} // namespace stlsoft
#endif /* _STLSOFT_NO_NAMESPACE */

/* ////////////////////////////////////////////////////////////////////////// */

#endif /* !STLSOFT_INCL_H_STLSOFT_PROXY_PTR */

/* ////////////////////////////////////////////////////////////////////////// */
