/* /////////////////////////////////////////////////////////////////////////////
 * File:        atlstl_ccombstr_veneer.h
 *
 * Purpose:     Contains the definition of the ccombstr_veneer template.
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


/// \file atlstl_ccombstr_veneer.h
///
/// Contains the definition of the ccombstr_veneer template.

#ifndef ATLSTL_INCL_H_ATLSTL_CCOMBSTR_VENEER
#define ATLSTL_INCL_H_ATLSTL_CCOMBSTR_VENEER

#ifndef __STLSOFT_DOCUMENTATION_SKIP_SECTION
# define ATLSTL_VER_H_ATLSTL_CCOMBSTR_VENEER_MAJOR       2
# define ATLSTL_VER_H_ATLSTL_CCOMBSTR_VENEER_MINOR       0
# define ATLSTL_VER_H_ATLSTL_CCOMBSTR_VENEER_REVISION    1
# define ATLSTL_VER_H_ATLSTL_CCOMBSTR_VENEER_EDIT        20
#endif /* !__STLSOFT_DOCUMENTATION_SKIP_SECTION */

/* /////////////////////////////////////////////////////////////////////////////
 * Includes
 */

#ifndef ATLSTL_INCL_H_ATLSTL
# include "atlstl.h"    // Include the ATLSTL root header
#endif /* !ATLSTL_INCL_H_ATLSTL */
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
#if defined(__STLSOFT_COMPILER_IS_DMC)
#  include "stlsoft_sap_cast.h"
# ifndef STLSOFT_INCL_H_STLSOFT_SAP_CAST
# endif /* !STLSOFT_INCL_H_STLSOFT_SAP_CAST */
#endif /* compiler */
#include <wchar.h>  // _wcsnset()

/* /////////////////////////////////////////////////////////////////////////////
 * Namespace
 *
 * The ATLSTL components are contained within the atlstl namespace. This is
 * actually an alias for stlsoft::atlstl_project,
 *
 * The definition matrix is as follows:
 *
 * _STLSOFT_NO_NAMESPACE    _ATLSTL_NO_NAMESPACE    atlstl definition
 * ---------------------    --------------------    -----------------
 *  not defined              not defined             = stlsoft::atlstl_project
 *  not defined              defined                 not defined
 *  defined                  not defined             atlstl
 *  defined                  defined                 not defined
 *
 */

#ifndef _ATLSTL_NO_NAMESPACE
# if defined(_STLSOFT_NO_NAMESPACE) || \
     defined(__STLSOFT_DOCUMENTATION_SKIP_SECTION)
/* There is no stlsoft namespace, so must define ::atlstl */
namespace atlstl
{
# else
/* Define stlsoft::atlstl_project */

namespace stlsoft
{

namespace atlstl_project
{

# endif /* _STLSOFT_NO_NAMESPACE */
#endif /* !_ATLSTL_NO_NAMESPACE */

/* ////////////////////////////////////////////////////////////////////////// */

/// \weakgroup libraries STLSoft Libraries
/// \brief The individual libraries

/// \weakgroup libraries_container Container Library
/// \ingroup libraries
/// \brief This library provides facilities for defining and manipulation containers

/// \defgroup atlstl_container_library Container Library (ATLSTL)
/// \ingroup ATLSTL libraries_container
/// \brief This library provides facilities for defining and manipulation containers for ATL types
/// @{

/* /////////////////////////////////////////////////////////////////////////////
 * Classes
 */

/// \brief Applies standard string (std::basic_string) interface to the CComBSTR class
///
/// This class adapts the
class ccombstr_veneer
#ifdef _ATLSTL_CCOMBSTR_VENEER_INHERIT_AS_PUBLIC
    : public CComBSTR
#else
    : private CComBSTR
#endif /* _ATLSTL_CCOMBSTR_VENEER_INHERIT_AS_PUBLIC */
{
private:
    typedef CComBSTR        parent_class_type;
    typedef ccombstr_veneer class_type;
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
    typedef as_size_t       size_type;

// Construction
public:
    /// Default constructor
    ccombstr_veneer();
    /// Copy constructor
    ccombstr_veneer(class_type const &rhs);
    /// Copy constructor
    ss_explicit_k ccombstr_veneer(CComBSTR const &rhs);
    /// Construct from an ANSI string
    ss_explicit_k ccombstr_veneer(LPCSTR s);
    /// Construct from a Unicode string
    ss_explicit_k ccombstr_veneer(LPCWSTR s);
    /// Construct from a range
    ccombstr_veneer(LPCOLESTR from, LPCOLESTR to);
    /// Construct from a range
    ccombstr_veneer(size_type length, LPCOLESTR from);
    ///
    ccombstr_veneer(as_size_t cch, OLECHAR ch);

    /// Copy assignment operator
    class_type const &operator =(class_type const &rhs);
    /// Copy assignment operator
    class_type const &operator =(CComBSTR const &rhs);
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
    /// Provides access to the underlying CComBSTR type
    CComBSTR        &get_base_type();
    /// Provides access to the underlying CComBSTR type
    CComBSTR const  &get_base_type() const;
    /// Returns the number of elements in the sequence
    size_type       length() const;
    /// Returns the number of elements in the sequence
    size_type       size() const;
    /// \brief Indicates whether the sequence is empty
    as_bool_t       empty() const;
    /// Returns a pointer to constant data representing the managed string
    const_pointer   c_str() const;
    ///
    const_pointer   data() const;
};

/* /////////////////////////////////////////////////////////////////////////////
 * Operators
 */

#ifndef _ATLSTL_CCOMBSTR_VENEER_INHERIT_AS_PUBLIC
inline as_bool_t operator ==(ccombstr_veneer const &lhs, ccombstr_veneer const &rhs)
{
    CComBSTR const  &lhs_ = lhs.get_base_type();
    CComBSTR const  &rhs_ = rhs.get_base_type();

#if (_ATL_VER < 0x0300)
    return operator == (ccombstr_veneer(lhs), rhs);
#else /* ? _ATL_VER */
    return rhs_ == lhs_;
#endif /* _ATL_VER */
}
inline as_bool_t operator ==(LPCSTR lhs, ccombstr_veneer const &rhs)
{
    CComBSTR const  &rhs_ = rhs.get_base_type();

#if (_ATL_VER < 0x0300)
    return operator == (ccombstr_veneer(lhs), rhs);
#else /* ? _ATL_VER */
    return rhs_ == lhs;
#endif /* _ATL_VER */
}
inline as_bool_t operator ==(ccombstr_veneer const &lhs, LPCSTR rhs)
{
    CComBSTR const  &lhs_ = lhs.get_base_type();

#if (_ATL_VER < 0x0300)
    return operator == (lhs, ccombstr_veneer(rhs));
#else /* ? _ATL_VER */
    return lhs_ == rhs;
#endif /* _ATL_VER */
}
inline as_bool_t operator ==(LPCWSTR lhs, ccombstr_veneer const &rhs)
{
    CComBSTR const  &rhs_ = rhs.get_base_type();

    return lhs == rhs_;
}
inline as_bool_t operator ==(ccombstr_veneer const &lhs, LPCWSTR rhs)
{
    CComBSTR const  &lhs_   =   lhs.get_base_type();
#if _ATL_VER >= 0x0700

    return lhs_ == rhs;
#else /* ? _ATL_VER */

    if(NULL == lhs_.m_str)
    {
        return (NULL == rhs);
    }
    else if(NULL == rhs)
    {
        return (NULL == lhs_.m_str);
    }
    else
    {
        return 0 == wcscmp(lhs_.m_str, rhs);
    }
#endif /* _ATL_VER */
}

inline as_bool_t operator !=(ccombstr_veneer const &lhs, ccombstr_veneer const &rhs)
{
    return !operator ==(lhs, rhs);
}
inline as_bool_t operator !=(LPCSTR lhs, ccombstr_veneer const &rhs)
{
    return !operator ==(lhs, rhs);
}
inline as_bool_t operator !=(ccombstr_veneer const &lhs, LPCSTR rhs)
{
    return !operator ==(lhs, rhs);
}
inline as_bool_t operator !=(LPCWSTR lhs, ccombstr_veneer const &rhs)
{
    return !operator ==(lhs, rhs);
}
inline as_bool_t operator !=(ccombstr_veneer const &lhs, LPCWSTR rhs)
{
    return !operator ==(lhs, rhs);
}
#endif /* !_ATLSTL_CCOMBSTR_VENEER_INHERIT_AS_PUBLIC */

/* /////////////////////////////////////////////////////////////////////////////
 * Shims
 */

inline LPCOLESTR c_str_ptr_null(ccombstr_veneer const &str)
{
    return str.empty() ? NULL : str.c_str();
}

inline LPCOLESTR c_str_ptr(ccombstr_veneer const &str)
{
    return str.c_str();
}

inline as_size_t c_str_len(ccombstr_veneer const &str)
{
    return str.length();
}

inline as_size_t c_str_size(ccombstr_veneer const &str)
{
    return c_str_len(str) * sizeof(OLECHAR);
}

template<ss_typename_param_k S>
inline S &operator <<(S &s, ccombstr_veneer const &str)
{
    s << str.c_str();

    return s;
}

/* /////////////////////////////////////////////////////////////////////////////
 * Unit-testing
 */

#ifdef STLSOFT_UNITTEST

namespace unittest
{
    ss_bool_t test_atlstl_ccombstr_veneer(unittest_reporter *r)
    {
        using stlsoft::unittest::unittest_initialiser;

        ss_bool_t               bSuccess    =   true;

        unittest_initialiser    init(r, "ATLSTL", "ccombstr_veneer", __FILE__);

        ccombstr_veneer         bs1( "Hello");
        ccombstr_veneer         bs2(L"Hello");

        if(bs1 != L"Hello")
        {
            r->report("Construction (from ANSI) failed ", __LINE__);
            bSuccess = false;
        }

        if(bs2 != L"Hello")
        {
//          wprintf(L"bs2: %u, %s\n", bs2.length(), bs2);

            r->report("Construction (from Unicode) failed ", __LINE__);
            bSuccess = false;
        }

//      wprintf(L"bs1: %u, %s\nbs2: %u, %s\n", bs1.length(), bs1, bs2.length(), bs2);

        if(bs1 != bs2)
        {
//          wprintf(L"bs1: %u, %s\nbs2: %u, %s\n", bs1.length(), bs1, bs2.length(), bs2);

            r->report("Copy construction failed ", __LINE__);
            bSuccess = false;
        }

        ccombstr_veneer         bs3(bs1);

        if(bs1 != bs3)
        {
            r->report("Copy assignment failed ", __LINE__);
            bSuccess = false;
        }

        bs3 = bs1;

        if(bs3 != "Hello")
        {
            r->report("Comparison to LPCSTR failed ", __LINE__);
            bSuccess = false;
        }

        if(bs1 != L"Hello")
        {
            r->report("Comparison to LPCWSTR failed ", __LINE__);
            bSuccess = false;
        }

        bs3 = "Hello";

        if(bs3 != L"Hello")
        {
            r->report("Assigment to LPCSTR failed ", __LINE__);
            bSuccess = false;
        }

        return bSuccess;
    }

    unittest_registrar    unittest_atlstl_ccombstr_veneer(test_atlstl_ccombstr_veneer);

} // namespace unittest

#endif /* STLSOFT_UNITTEST */

/* /////////////////////////////////////////////////////////////////////////////
 * Implementation
 */

inline ccombstr_veneer::ccombstr_veneer()
    : parent_class_type()
{
    stlsoft_constraint_must_be_same_size(CComBSTR, class_type);
}

inline ccombstr_veneer::ccombstr_veneer(class_type const &rhs)
    : parent_class_type(rhs)
{
    stlsoft_constraint_must_be_same_size(CComBSTR, class_type);
}

inline ccombstr_veneer::ccombstr_veneer(CComBSTR const &rhs)
    : parent_class_type(rhs)
{
    stlsoft_constraint_must_be_same_size(CComBSTR, class_type);
}

inline ccombstr_veneer::ccombstr_veneer(LPCSTR s)
/* There's a bug in some implementations of ATL 3.x, so we go for the assigment,
 * doing the conversion ourselves
 */
#if _ATL_VER >= 0x0700
    : parent_class_type(s)
#endif /* _ATL_VER */
{
#if _ATL_VER < 0x0700
    USES_CONVERSION;
    *this = A2COLE(s);
#endif /* _ATL_VER */

    atlstl_assert(s == NULL || length() == strlen(s));

    stlsoft_constraint_must_be_same_size(CComBSTR, class_type);
}

inline ccombstr_veneer::ccombstr_veneer(LPCWSTR s)
#if _ATL_VER >= 0x0300
    : parent_class_type(s)
#endif /* _ATL_VER */
{
#if _ATL_VER < 0x0300
    USES_CONVERSION;
    *this = s;
#endif /* _ATL_VER */

    atlstl_assert(s == NULL || length() == wcslen(s));

    stlsoft_constraint_must_be_same_size(CComBSTR, class_type);
}

inline ccombstr_veneer::ccombstr_veneer(LPCOLESTR from, LPCOLESTR to)
    : parent_class_type(static_cast<int>(to - from), from)
{
    stlsoft_constraint_must_be_same_size(CComBSTR, class_type);
}

inline void ccombstr_veneer::swap(ccombstr_veneer::class_type &rhs)
{
    BSTR    str         =   rhs.m_str;
            rhs.m_str   =   m_str;
            m_str       =   str;
}

inline ccombstr_veneer::ccombstr_veneer(ccombstr_veneer::size_type length, LPCOLESTR from)
    : parent_class_type(static_cast<int>(length), from)
{
    stlsoft_constraint_must_be_same_size(CComBSTR, class_type);
}

inline ccombstr_veneer::ccombstr_veneer(as_size_t cch, OLECHAR ch)
{
    stlsoft_constraint_must_be_same_size(CComBSTR, class_type);

    typedef stlsoft_ns_qual(auto_buffer)<OLECHAR, stlsoft_ns_qual(new_allocator)<OLECHAR> > buffer_t;

    buffer_t    buffer(cch);

    _wcsnset(buffer, ch, cch);

    class_type(cch, buffer).swap(*this);
}

inline ccombstr_veneer::class_type const &ccombstr_veneer::operator =(ccombstr_veneer::class_type const &rhs)
{
    parent_class_type::operator =(rhs);

    return *this;
}

inline ccombstr_veneer::class_type const &ccombstr_veneer::operator =(CComBSTR const &rhs)
{
    parent_class_type::operator =(rhs);

    return *this;
}

inline ccombstr_veneer::class_type const &ccombstr_veneer::operator =(LPCSTR s)
{
/* There's a bug in some implementations of ATL 3.x, so we go for the assigment,
 * doing the conversion ourselves
 */
#if _ATL_VER >= 0x0700
    parent_class_type::operator =(s);
#else /* ? _ATL_VER */
    USES_CONVERSION;
    *this = A2COLE(s);
#endif /* _ATL_VER */

    return *this;
}

inline ccombstr_veneer::class_type const &ccombstr_veneer::operator =(LPCWSTR s)
{
    parent_class_type::operator =(s);

    return *this;
}

inline ccombstr_veneer::const_iterator ccombstr_veneer::begin() const
{
    return *this;
}

inline ccombstr_veneer::const_iterator ccombstr_veneer::end() const
{
    return begin() + Length();
}

inline ccombstr_veneer::reference ccombstr_veneer::operator [](ccombstr_veneer::size_type index)
{
    atlstl_message_assert("Index out of range", index < length());

    return const_cast<reference>(data()[index]);
}

inline ccombstr_veneer::const_reference ccombstr_veneer::operator [](ccombstr_veneer::size_type index) const
{
    atlstl_message_assert("Index out of range", index < length());

    return data()[index];
}

inline CComBSTR &ccombstr_veneer::get_base_type()
{
#if defined(__STLSOFT_COMPILER_IS_DMC)
    CComBSTR    *this_  =   stlsoft_ns_qual(sap_cast)<CComBSTR*>(this);
#else /* ? compiler */
    CComBSTR    *this_  =   this;
#endif /* compiler */

    return *this_;
}

inline CComBSTR const &ccombstr_veneer::get_base_type() const
{
#if defined(__STLSOFT_COMPILER_IS_DMC)
    CComBSTR const  *this_  =   stlsoft_ns_qual(sap_cast)<CComBSTR const*>(this);
#else /* ? compiler */
    CComBSTR const  *this_  =   this;
#endif /* compiler */

    return *this_;
}

inline ccombstr_veneer::size_type ccombstr_veneer::length() const
{
    return Length();
}

inline ccombstr_veneer::size_type ccombstr_veneer::size() const
{
    return length();
}

inline as_bool_t ccombstr_veneer::empty() const
{
    return length() == 0;
}

inline ccombstr_veneer::const_pointer ccombstr_veneer::c_str() const
{
    return *this;
}

inline ccombstr_veneer::const_pointer ccombstr_veneer::data() const
{
    return *this;
}

/* ////////////////////////////////////////////////////////////////////////// */

/// @} // end of group atlstl_container_library

/* ////////////////////////////////////////////////////////////////////////// */

#ifndef _ATLSTL_NO_NAMESPACE
# if defined(_STLSOFT_NO_NAMESPACE) || \
     defined(__STLSOFT_DOCUMENTATION_SKIP_SECTION)
} // namespace atlstl
# else
} // namespace atlstl_project
} // namespace stlsoft
# endif /* _STLSOFT_NO_NAMESPACE */
#endif /* !_ATLSTL_NO_NAMESPACE */

/* ////////////////////////////////////////////////////////////////////////// */

#endif /* !ATLSTL_INCL_H_ATLSTL_CCOMBSTR_VENEER */

/* ////////////////////////////////////////////////////////////////////////// */
