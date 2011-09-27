/* /////////////////////////////////////////////////////////////////////////////
 * File:        xstl_bstr_t_veneer.h
 *
 * Purpose:     Contains the definition of the bstr_t_veneer template.
 *
 * Created:     1st October 2002
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


/// \file xstl_bstr_t_veneer.h
///
/// Contains the definition of the bstr_t_veneer template.

#ifndef XSTL_INCL_H_XSTL_BSTR_T_VENEER
#define XSTL_INCL_H_XSTL_BSTR_T_VENEER

#ifndef __STLSOFT_DOCUMENTATION_SKIP_SECTION
# define XSTL_VER_H_XSTL_BSTR_T_VENEER_MAJOR       2
# define XSTL_VER_H_XSTL_BSTR_T_VENEER_MINOR       0
# define XSTL_VER_H_XSTL_BSTR_T_VENEER_REVISION    1
# define XSTL_VER_H_XSTL_BSTR_T_VENEER_EDIT        11
#endif /* !__STLSOFT_DOCUMENTATION_SKIP_SECTION */

/* /////////////////////////////////////////////////////////////////////////////
 * Includes
 */

#ifndef XSTL_INCL_H_XSTL
//# include "xstl.h"    // Include the XSTL root header
#endif /* !XSTL_INCL_H_XSTL */
#ifndef STLSOFT_INCL_H_STLSOFT_ITERATOR
# include "stlsoft_iterator.h"
#endif /* !STLSOFT_INCL_H_STLSOFT_ITERATOR */
#ifndef STLSOFT_INCL_H_STLSOFT_CONSTRAINTS
# include "stlsoft_constraints.h"
#endif /* !STLSOFT_INCL_H_STLSOFT_CONSTRAINTS */
#ifndef STLSOFT_INCL_H_STLSOFT_AUTO_BUFFER
# include "stlsoft_auto_buffer.h"
#endif /* !STLSOFT_INCL_H_STLSOFT_AUTO_BUFFER */
#ifndef STLSOFT_INCL_H_STLSOFT_NEW_ALLOCATOR
# include "stlsoft_new_allocator.h"
#endif /* !STLSOFT_INCL_H_STLSOFT_NEW_ALLOCATOR */
#include <comdef.h> // _bstr_t
#include <wchar.h>  // _wcsnset()

/* /////////////////////////////////////////////////////////////////////////////
 * Namespace
 *
 * The XSTL components are contained within the xstl namespace. This is
 * actually an alias for stlsoft::xstl_project,
 *
 * The definition matrix is as follows:
 *
 * _STLSOFT_NO_NAMESPACE    _XSTL_NO_NAMESPACE    xstl definition
 * ---------------------    --------------------    -----------------
 *  not defined              not defined             = stlsoft::xstl_project
 *  not defined              defined                 not defined
 *  defined                  not defined             xstl
 *  defined                  defined                 not defined
 *
 */

#ifndef _XSTL_NO_NAMESPACE
# ifdef _STLSOFT_NO_NAMESPACE
/* There is no stlsoft namespace, so must define ::xstl */
namespace xstl
{
# else
/* Define stlsoft::xstl_project */

namespace stlsoft
{

namespace xstl_project
{

# endif /* _STLSOFT_NO_NAMESPACE */
#endif /* !_XSTL_NO_NAMESPACE */

/* ////////////////////////////////////////////////////////////////////////// */

/// \weakgroup libraries STLSoft Libraries
/// \brief The individual libraries

/// \weakgroup libraries_container Container Library
/// \ingroup libraries
/// \brief This library provides facilities for defining and manipulation containers

/// \defgroup xstl_container_library Container Library (XSTL)
/// \ingroup XSTL libraries_container
/// \brief This library provides facilities for defining and manipulation containers for ATL types
/// @{

/* /////////////////////////////////////////////////////////////////////////////
 * Classes
 */

            typedef ss_bool_t               xs_bool_t;
            typedef ss_size_t               xs_size_t;
#            define xstl_message_assert     stlsoft_message_assert

#    ifndef _XSTL_NO_NAMESPACE
#     ifdef _STLSOFT_NO_NAMESPACE
    } // namespace xstl
#     else
    } // namespace xstl_project
    } // namespace stlsoft
#     endif /* _STLSOFT_NO_NAMESPACE */
    namespace xstl = ::stlsoft::xstl_project;
#    endif /* !_XSTL_NO_NAMESPACE */


#    ifndef _XSTL_NO_NAMESPACE
#     ifdef _STLSOFT_NO_NAMESPACE
    /* There is no stlsoft namespace, so must define ::xstl */
    namespace xstl
    {
#     else
    /* Define stlsoft::xstl_project */

    namespace stlsoft
    {

    namespace xstl_project
    {

#     endif /* _STLSOFT_NO_NAMESPACE */
#    endif /* !_XSTL_NO_NAMESPACE */

/// \brief Applies standard string (std::basic_string) interface to the _bstr_t class
///
/// This class adapts the
class bstr_t_veneer
#ifdef _XSTL_BSTR_T_VENEER_INHERIT_AS_PUBLIC
    : public _bstr_t
#else
    : private _bstr_t
#endif /* _XSTL_BSTR_T_VENEER_INHERIT_AS_PUBLIC */
{
private:
    typedef _bstr_t         parent_class_type;
    typedef bstr_t_veneer  class_type;
public:
    /// The value type
    typedef OLECHAR         value_type;
    /// The mutating iterator type
    typedef LPOLESTR        iterator;
    /// The non-mutating (const) iterator type
    typedef LPCOLESTR       const_iterator;
    /// The pointer type
    typedef LPOLESTR        pointer;
    /// The pointer-to-const type
    typedef LPCOLESTR       const_pointer;
    /// The reference type
    typedef OLECHAR         &reference;
    /// The reference-to-const type
    typedef OLECHAR const   &const_reference;
    /// The size type
    typedef ss_size_t       size_type;

// Construction
public:
    /// Default constructor
    bstr_t_veneer();
    /// Copy constructor
    bstr_t_veneer(class_type const &rhs);
    /// Copy constructor
    ss_explicit_k bstr_t_veneer(_bstr_t const &rhs);
    /// Construct from an ANSI string
    ss_explicit_k bstr_t_veneer(LPCSTR s);
    /// Construct from a Unicode string
    ss_explicit_k bstr_t_veneer(LPCWSTR s);
    /// Construct from a range
    bstr_t_veneer(LPCOLESTR from, LPCOLESTR to);
    /// Construct from a range
    bstr_t_veneer(size_type length, LPCOLESTR from);
    ///
    bstr_t_veneer(ss_size_t cch, OLECHAR ch);

    /// Copy assignment operator
    class_type const &operator =(class_type const &rhs);
    /// Copy assignment operator
    class_type const &operator =(_bstr_t const &rhs);
    /// Assignment operator
    class_type const &operator =(LPCSTR s);
    /// Assignment operator
    class_type const &operator =(LPCWSTR s);

// Iteration
public:
    /// Begins the iteration
    ///
    /// \return An iterator representing the start of the sequence
    const_iterator begin() const;
    /// Ends the iteration
    ///
    /// \return An iterator representing the end of the sequence
    const_iterator end() const;

// Access
public:
    reference operator [](size_type index);
    const_reference operator [](size_type index) const;

// Operations
public:
    void swap(class_type &rhs);

// Attributes
public:
    /// Returns the number of elements in the sequence
    size_type       length() const;
    /// Returns the number of elements in the sequence
    size_type       size() const;
    /// \brief Indicates whether the sequence is empty
    xs_bool_t       empty() const;
    /// Returns a pointer to constant data representing the managed string
    const_pointer   c_str() const;
    ///
    const_pointer   data() const;

// Implementation
private:
    void assign_(size_type length, LPCOLESTR from);
};

////////////////////////////////////////////////////////////////////////////////
// Unit-testing

#ifdef STLSOFT_UNITTEST

namespace unittest
{
    ss_bool_t test_xstl_bstr_t_veneer(unittest_reporter *r)
    {
        using stlsoft::unittest::unittest_initialiser;

        ss_bool_t               bSuccess    =   true;

        unittest_initialiser    init(r, "XSTL", "bstr_t_veneer", __FILE__);

#if 0
        if(<<TODO>>)
        {
            r->report("<<TODO>> failed ", __LINE__);
            bSuccess = false;
        }
#endif /* 0 */

        return bSuccess;
    }

    unittest_registrar    unittest_xstl_bstr_t_veneer(test_xstl_bstr_t_veneer);

} // namespace unittest

#endif /* STLSOFT_UNITTEST */

/* /////////////////////////////////////////////////////////////////////////////
 * Implementation
 */

inline void bstr_t_veneer::assign_(bstr_t_veneer::size_type length, LPCOLESTR from)
{
    typedef stlsoft_ns_qual(auto_buffer)<OLECHAR, stlsoft_ns_qual(new_allocator)<OLECHAR> > buffer_t;

    buffer_t    buffer(1 + length);

    memcpy(buffer.data(), from, sizeof(value_type) * length);
    buffer[length] = L'\0';

    parent_class_type::operator =(buffer);
}


inline bstr_t_veneer::bstr_t_veneer()
    : parent_class_type()
{
    stlsoft_constraint_must_be_same_size(_bstr_t, class_type);
}

inline bstr_t_veneer::bstr_t_veneer(class_type const &rhs)
    : parent_class_type(rhs)
{
    stlsoft_constraint_must_be_same_size(_bstr_t, class_type);
}

inline bstr_t_veneer::bstr_t_veneer(_bstr_t const &rhs)
    : parent_class_type(rhs)
{
    stlsoft_constraint_must_be_same_size(_bstr_t, class_type);
}

inline bstr_t_veneer::bstr_t_veneer(LPCSTR s)
    : parent_class_type(s)
{
    stlsoft_constraint_must_be_same_size(_bstr_t, class_type);
}

inline bstr_t_veneer::bstr_t_veneer(LPCWSTR s)
    : parent_class_type(s)
{
    stlsoft_constraint_must_be_same_size(_bstr_t, class_type);
}

inline bstr_t_veneer::bstr_t_veneer(LPCOLESTR from, LPCOLESTR to)
{
    stlsoft_constraint_must_be_same_size(_bstr_t, class_type);

    assign_(to - from, from);
}

#if 0
inline void bstr_t_veneer::swap(bstr_t_veneer::class_type &rhs)
{
    BSTR    str         =   rhs.m_str;
            rhs.m_str   =   m_str;
            m_str       =   str;
}
#endif /* 0 */

inline bstr_t_veneer::bstr_t_veneer(bstr_t_veneer::size_type length, LPCOLESTR from)
{
    stlsoft_constraint_must_be_same_size(_bstr_t, class_type);

    assign_(length, from);
}

inline bstr_t_veneer::bstr_t_veneer(ss_size_t cch, OLECHAR ch)
{
    stlsoft_constraint_must_be_same_size(_bstr_t, class_type);

    typedef stlsoft_ns_qual(auto_buffer)<OLECHAR, stlsoft_ns_qual(new_allocator)<OLECHAR> > buffer_t;

    buffer_t    buffer(cch);

    _wcsnset(buffer, ch, cch);

    assign_(cch, buffer);
}

inline bstr_t_veneer::class_type const &bstr_t_veneer::operator =(bstr_t_veneer::class_type const &rhs)
{
    parent_class_type::operator =(rhs);

    return *this;
}

inline bstr_t_veneer::class_type const &bstr_t_veneer::operator =(_bstr_t const &rhs)
{
    parent_class_type::operator =(rhs);

    return *this;
}

inline bstr_t_veneer::class_type const &bstr_t_veneer::operator =(LPCSTR s)
{
    parent_class_type::operator =(s);

    return *this;
}

inline bstr_t_veneer::class_type const &bstr_t_veneer::operator =(LPCWSTR s)
{
    parent_class_type::operator =(s);

    return *this;
}

inline bstr_t_veneer::const_iterator bstr_t_veneer::begin() const
{
    return *this;
}

inline bstr_t_veneer::const_iterator bstr_t_veneer::end() const
{
    return begin() + length();
}

inline bstr_t_veneer::reference bstr_t_veneer::operator [](bstr_t_veneer::size_type index)
{
    xstl_message_assert("Index out of range", index < length());

    return const_cast<reference>(data()[index]);
}

inline bstr_t_veneer::const_reference bstr_t_veneer::operator [](bstr_t_veneer::size_type index) const
{
    xstl_message_assert("Index out of range", index < length());

    return data()[index];
}

inline bstr_t_veneer::size_type bstr_t_veneer::length() const
{
    return parent_class_type::length();
}

inline bstr_t_veneer::size_type bstr_t_veneer::size() const
{
    return length();
}

inline xs_bool_t bstr_t_veneer::empty() const
{
    return length() == 0;
}

inline bstr_t_veneer::const_pointer bstr_t_veneer::c_str() const
{
    return *this;
}

inline bstr_t_veneer::const_pointer bstr_t_veneer::data() const
{
    return *this;
}

/* /////////////////////////////////////////////////////////////////////////////
 * Shims
 */

inline LPCOLESTR c_str_ptr_null(bstr_t_veneer const &s)
{
    return s.empty() ? NULL : s.c_str();
}

inline LPCOLESTR c_str_ptr(bstr_t_veneer const &s)
{
    return s.c_str();
}

inline xs_size_t c_str_len(bstr_t_veneer const &s)
{
    return s.length();
}

inline xs_size_t c_str_size(bstr_t_veneer const &s)
{
    return c_str_len(s) * sizeof(OLECHAR);
}

/* ////////////////////////////////////////////////////////////////////////// */

/// @} // end of group xstl_container_library

/* ////////////////////////////////////////////////////////////////////////// */

#ifndef _XSTL_NO_NAMESPACE
# ifdef _STLSOFT_NO_NAMESPACE
} // namespace xstl
# else
} // namespace xstl_project
} // namespace stlsoft
# endif /* _STLSOFT_NO_NAMESPACE */
#endif /* !_XSTL_NO_NAMESPACE */

/* ////////////////////////////////////////////////////////////////////////// */

#endif /* !XSTL_INCL_H_XSTL_BSTR_T_VENEER */

/* ////////////////////////////////////////////////////////////////////////// */
