/* /////////////////////////////////////////////////////////////////////////////
 * File:        mfcstl_cstring_veneer.h
 *
 * Purpose:     Contains the definition of the cstring_veneer template.
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


/// \file mfcstl_cstring_veneer.h
///
/// Contains the definition of the cstring_veneer template.

#ifndef MFCSTL_INCL_H_MFCSTL_CSTRING_VENEER
#define MFCSTL_INCL_H_MFCSTL_CSTRING_VENEER

#ifndef __STLSOFT_DOCUMENTATION_SKIP_SECTION
# define MFCSTL_VER_H_MFCSTL_CSTRING_VENEER_MAJOR       2
# define MFCSTL_VER_H_MFCSTL_CSTRING_VENEER_MINOR       0
# define MFCSTL_VER_H_MFCSTL_CSTRING_VENEER_REVISION    1
# define MFCSTL_VER_H_MFCSTL_CSTRING_VENEER_EDIT        35
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

#ifndef MFCSTL_INCL_H_MFCSTL
# include "mfcstl.h"    // Include the MFCSTL root header
#endif /* !MFCSTL_INCL_H_MFCSTL */

#if defined(__STLSOFT_COMPILER_IS_MSVC) && \
    _MSC_VER < 1200
# error mfcstl_cstring_veneer.h is not compatible with Visual C++ 5.0 or earlier
#endif /* _MSC_VER < 1200 */

#ifndef STLSOFT_INCL_H_STLSOFT_ITERATOR
# include "stlsoft_iterator.h"
#endif /* !STLSOFT_INCL_H_STLSOFT_ITERATOR */
#ifndef STLSOFT_INCL_H_STLSOFT_CONSTRAINTS
# include "stlsoft_constraints.h"
#endif /* !STLSOFT_INCL_H_STLSOFT_CONSTRAINTS */
#if defined(__STLSOFT_COMPILER_IS_DMC)
#  include "stlsoft_sap_cast.h"
# ifndef STLSOFT_INCL_H_STLSOFT_SAP_CAST
# endif /* !STLSOFT_INCL_H_STLSOFT_SAP_CAST */
#endif /* compiler */

/* /////////////////////////////////////////////////////////////////////////////
 * Namespace
 *
 * The MFCSTL components are contained within the mfcstl namespace. This is
 * actually an alias for stlsoft::mfcstl_project,
 *
 * The definition matrix is as follows:
 *
 * _STLSOFT_NO_NAMESPACE    _MFCSTL_NO_NAMESPACE    mfcstl definition
 * ---------------------    --------------------    -----------------
 *  not defined              not defined             = stlsoft::mfcstl_project
 *  not defined              defined                 not defined
 *  defined                  not defined             mfcstl
 *  defined                  defined                 not defined
 *
 */

#ifndef _MFCSTL_NO_NAMESPACE
# if defined(_STLSOFT_NO_NAMESPACE) || \
     defined(__STLSOFT_DOCUMENTATION_SKIP_SECTION)
/* There is no stlsoft namespace, so must define ::mfcstl */
namespace mfcstl
{
# else
/* Define stlsoft::mfcstl_project */

namespace stlsoft
{

namespace mfcstl_project
{

# endif /* _STLSOFT_NO_NAMESPACE */
#endif /* !_MFCSTL_NO_NAMESPACE */

/* ////////////////////////////////////////////////////////////////////////// */

/// \weakgroup libraries STLSoft Libraries
/// \brief The individual libraries

/// \weakgroup libraries_container Container Library
/// \ingroup libraries
/// \brief This library provides facilities for defining and manipulation containers

/// \defgroup mfcstl_container_library Container Library (MFCSTL)
/// \ingroup MFCSTL libraries_container
/// \brief This library provides facilities for defining and manipulation containers for MFC types
/// @{

/* /////////////////////////////////////////////////////////////////////////////
 * Classes
 */

/// \brief Applies standard string (std::basic_string) interface to the CString class
///
/// This class adapts the
class cstring_veneer
#ifdef _MFCSTL_CSTRING_VENEER_INHERIT_AS_PUBLIC
    : public CString
#else
    : private CString
#endif /* _MFCSTL_CSTRING_VENEER_INHERIT_AS_PUBLIC */
{
private:
    typedef CString         parent_class_type;
    typedef cstring_veneer  class_type;
public:
    /// The value type
    typedef TCHAR       value_type;
    /// The mutating iterator type
    typedef LPTSTR      iterator;
    /// The non-mutating (const) iterator type
    typedef LPCTSTR     const_iterator;
    /// The pointer type
    typedef LPTSTR      pointer;
    /// The pointer-to-const type
    typedef LPCTSTR     const_pointer;
    /// The reference type
    typedef TCHAR       &reference;
    /// The reference-to-const type
    typedef TCHAR const &const_reference;
    /// The size type
    typedef ms_size_t   size_type;

// Construction
public:
    /// Default constructor
    cstring_veneer();
    /// Copy constructor
    cstring_veneer(class_type const &rhs);
    /// Copy constructor
    ss_explicit_k cstring_veneer(CString const &rhs);
    /// Construct from an ANSI string
    ss_explicit_k cstring_veneer(LPCSTR s);
    /// Construct from a Unicode string
    ss_explicit_k cstring_veneer(LPCWSTR s);
    /// Construct from an MBCS string
    ss_explicit_k cstring_veneer(unsigned char const *s);
    /// Construct from a range
    cstring_veneer(LPCTSTR from, LPCTSTR to);
    /// Construct from a range
    cstring_veneer(LPCTSTR from, size_type length);
    ///
    cstring_veneer(ms_size_t cch, TCHAR ch);

    /// Copy assignment operator
    class_type const &operator =(class_type const &rhs);
    /// Copy assignment operator
    class_type const &operator =(CString const &rhs);
    /// Assignment operator
    class_type const &operator =(LPCSTR s);
    /// Assignment operator
    class_type const &operator =(LPCWSTR s);
    /// Assignment operator
    class_type const &operator =(unsigned char const *s);

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

// Attributes
public:
    /// Provides access to the underlying CString type
    CString         &get_base_type();
    /// Provides access to the underlying CString type
    CString const   &get_base_type() const;
    /// Returns the number of elements in the sequence
    size_type       length() const;
    /// Returns the number of elements in the sequence
    size_type       size() const;
    /// \brief Indicates whether the sequence is empty
    ms_bool_t       empty() const;
    /// Returns a pointer to constant data representing the managed string
    const_pointer   c_str() const;
    ///
    const_pointer   data() const;
};

/* /////////////////////////////////////////////////////////////////////////////
 * Operators
 */

#ifndef _MFCSTL_CSTRING_VENEER_INHERIT_AS_PUBLIC
inline ms_bool_t operator ==(cstring_veneer const &lhs, cstring_veneer const &rhs)
{
    CString const  &lhs_ = lhs.get_base_type();
    CString const  &rhs_ = rhs.get_base_type();

    return lhs_ == rhs_;
}
inline ms_bool_t operator ==(LPCSTR lhs, cstring_veneer const &rhs)
{
    CString const  &rhs_ = rhs.get_base_type();

    return rhs_ == lhs;
}
inline ms_bool_t operator ==(cstring_veneer const &lhs, LPCSTR rhs)
{
    CString const  &lhs_ = lhs.get_base_type();

    return lhs_ == rhs;
}
inline ms_bool_t operator ==(LPCWSTR lhs, cstring_veneer const &rhs)
{
    CString const  &rhs_ = rhs.get_base_type();

#if defined(__STLSOFT_COMPILER_IS_BORLAND) || \
    (   defined(__STLSOFT_COMPILER_IS_DMC) && \
        __DMC__ < 0x0835)
    return CString(lhs) == rhs_;
#else /* ? compiler */
    return lhs == rhs_;
#endif /* compiler */
}
inline ms_bool_t operator ==(cstring_veneer const &lhs, LPCWSTR rhs)
{
    CString const  &lhs_ = lhs.get_base_type();

#if defined(__STLSOFT_COMPILER_IS_BORLAND) || \
    (   defined(__STLSOFT_COMPILER_IS_DMC) && \
        __DMC__ < 0x0835)
    return lhs_ == CString(rhs);
#else /* ? compiler */
    return lhs_ == rhs;
#endif /* compiler */
}
inline ms_bool_t operator !=(cstring_veneer const &lhs, cstring_veneer const &rhs)
{
    return !operator ==(lhs, rhs);
}
inline ms_bool_t operator !=(LPCSTR lhs, cstring_veneer const &rhs)
{
    return !operator ==(lhs, rhs);
}
inline ms_bool_t operator !=(cstring_veneer const &lhs, LPCSTR rhs)
{
    return !operator ==(lhs, rhs);
}
inline ms_bool_t operator !=(LPCWSTR lhs, cstring_veneer const &rhs)
{
    return !operator ==(lhs, rhs);
}
inline ms_bool_t operator !=(cstring_veneer const &lhs, LPCWSTR rhs)
{
    return !operator ==(lhs, rhs);
}
#endif /* !_MFCSTL_CSTRING_VENEER_INHERIT_AS_PUBLIC */

/* /////////////////////////////////////////////////////////////////////////////
 * Shims
 */

inline LPCTSTR c_str_ptr_null(cstring_veneer const &s)
{
    return s.empty() ? NULL : s.c_str();
}

inline LPCTSTR c_str_ptr(cstring_veneer const &s)
{
    return s.c_str();
}

inline ms_size_t c_str_len(cstring_veneer const &s)
{
    return s.length();
}

inline ms_size_t c_str_size(cstring_veneer const &s)
{
    return c_str_len(s) * sizeof(TCHAR);
}

template<ss_typename_param_k S>
inline S &operator <<(S &s, cstring_veneer const &str)
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
    ss_bool_t test_mfcstl_cstring_veneer(unittest_reporter *r)
    {
        using stlsoft::unittest::unittest_initialiser;

        ss_bool_t               bSuccess    =   true;

        unittest_initialiser    init(r, "MFCSTL", "cstring_veneer", __FILE__);

        cstring_veneer  cs1("Hello");
        cstring_veneer  cs2(cs1);

        if(cs1 != cs2)
        {
            r->report("Copy construction failed ", __LINE__);
            bSuccess = false;
        }

        cstring_veneer  cs3;

        cs3 = cs1;

        if(cs1 != cs3)
        {
            r->report("Copy assignment failed ", __LINE__);
            bSuccess = false;
        }

        if(cs1 != "Hello")
        {
            r->report("Comparison to LPCSTR failed ", __LINE__);
            bSuccess = false;
        }

        if(cs1 != L"Hello")
        {
            r->report("Comparison to LPCWSTR failed ", __LINE__);
            bSuccess = false;
        }

        return bSuccess;
    }

    unittest_registrar    unittest_mfcstl_cstring_veneer(test_mfcstl_cstring_veneer);

} // namespace unittest

#endif /* STLSOFT_UNITTEST */

/* /////////////////////////////////////////////////////////////////////////////
 * Implementation
 */

inline cstring_veneer::cstring_veneer()
    : parent_class_type()
{
    stlsoft_constraint_must_be_same_size(CString, class_type);
}

inline cstring_veneer::cstring_veneer(class_type const &rhs)
    : parent_class_type(rhs.get_base_type())
{
    stlsoft_constraint_must_be_same_size(CString, class_type);
}

inline cstring_veneer::cstring_veneer(CString const &rhs)
    : parent_class_type(rhs)
{
    stlsoft_constraint_must_be_same_size(CString, class_type);
}

inline cstring_veneer::cstring_veneer(LPCSTR s)
    : parent_class_type(s)
{
    stlsoft_constraint_must_be_same_size(CString, class_type);
}

inline cstring_veneer::cstring_veneer(LPCWSTR s)
    : parent_class_type(s)
{
    stlsoft_constraint_must_be_same_size(CString, class_type);
}

inline cstring_veneer::cstring_veneer(unsigned char const *s)
    : parent_class_type(s)
{
    stlsoft_constraint_must_be_same_size(CString, class_type);
}

inline cstring_veneer::cstring_veneer(LPCTSTR from, LPCTSTR to)
    : parent_class_type(from, static_cast<int>(to - from))
{
    stlsoft_constraint_must_be_same_size(CString, class_type);
}

inline cstring_veneer::cstring_veneer(LPCTSTR from, cstring_veneer::size_type length)
    : parent_class_type(from, static_cast<int>(length))
{
    stlsoft_constraint_must_be_same_size(CString, class_type);
}

inline cstring_veneer::cstring_veneer(ms_size_t cch, TCHAR ch)
    : parent_class_type(ch, static_cast<int>(cch))
{
    stlsoft_constraint_must_be_same_size(CString, class_type);
}

inline cstring_veneer::class_type const &cstring_veneer::operator =(cstring_veneer::class_type const &rhs)
{
    parent_class_type::operator =(rhs.get_base_type());

    return *this;
}

inline cstring_veneer::class_type const &cstring_veneer::operator =(CString const &rhs)
{
    parent_class_type::operator =(rhs);

    return *this;
}

inline cstring_veneer::class_type const &cstring_veneer::operator =(LPCSTR s)
{
    parent_class_type::operator =(s);

    return *this;
}

inline cstring_veneer::class_type const &cstring_veneer::operator =(LPCWSTR s)
{
    parent_class_type::operator =(s);

    return *this;
}

inline cstring_veneer::class_type const &cstring_veneer::operator =(unsigned char const *s)
{
    parent_class_type::operator =(s);

    return *this;
}

inline cstring_veneer::const_iterator cstring_veneer::begin() const
{
    return *this;
}

inline cstring_veneer::const_iterator cstring_veneer::end() const
{
    return begin() + GetLength();
}

inline cstring_veneer::reference cstring_veneer::operator [](cstring_veneer::size_type index)
{
    mfcstl_message_assert("Index out of range", index < length());

    return const_cast<reference>(data()[index]);
}

inline cstring_veneer::const_reference cstring_veneer::operator [](cstring_veneer::size_type index) const
{
    mfcstl_message_assert("Index out of range", index < length());

    return data()[index];
}

inline CString &cstring_veneer::get_base_type()
{
#if defined(__STLSOFT_COMPILER_IS_DMC)
    CString *this_  =   stlsoft_ns_qual(sap_cast)<CString*>(this);
#else /* ? compiler */
    CString *this_  =   this;
#endif /* compiler */

    return *this_;
}

inline CString const &cstring_veneer::get_base_type() const
{
#if defined(__STLSOFT_COMPILER_IS_DMC)
    CString const   *this_  =   stlsoft_ns_qual(sap_cast)<CString const*>(this);
#else /* ? compiler */
    CString const   *this_  =   this;
#endif /* compiler */

    return *this_;
}

inline cstring_veneer::size_type cstring_veneer::length() const
{
    return GetLength();
}

inline cstring_veneer::size_type cstring_veneer::size() const
{
    return length();
}

inline ms_bool_t cstring_veneer::empty() const
{
    return length() == 0;
}

inline cstring_veneer::const_pointer cstring_veneer::c_str() const
{
    return *this;
}

inline cstring_veneer::const_pointer cstring_veneer::data() const
{
    return *this;
}

/* ////////////////////////////////////////////////////////////////////////// */

/// @} // end of group mfcstl_container_library

/* ////////////////////////////////////////////////////////////////////////// */

#ifndef _MFCSTL_NO_NAMESPACE
# if defined(_STLSOFT_NO_NAMESPACE) || \
     defined(__STLSOFT_DOCUMENTATION_SKIP_SECTION)
} // namespace mfcstl
# else
} // namespace mfcstl_project
} // namespace stlsoft
# endif /* _STLSOFT_NO_NAMESPACE */
#endif /* !_MFCSTL_NO_NAMESPACE */

/* ////////////////////////////////////////////////////////////////////////// */

#endif /* !MFCSTL_INCL_H_MFCSTL_CSTRING_VENEER */

/* ////////////////////////////////////////////////////////////////////////// */
