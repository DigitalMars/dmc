/* ////////////////////////////////////////////////////////////////////////////
 * File:        mfcstl_resource_string.h (Formerly MWResStr.h: ::SynesisWin)
 *
 * Purpose:     resource_string class.
 *
 * Created:     1st November 1994
 * Updated:     11th September 2004
 *
 * Home:        http://stlsoft.org/
 *
 * Copyright (c) 1994-2004, Matthew Wilson and Synesis Software
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


/// \file mfcstl_resource_string.h
///
/// resource_string class.

#ifndef MFCSTL_INCL_H_MFCSTL_RESOURCE_STRING
#define MFCSTL_INCL_H_MFCSTL_RESOURCE_STRING

#ifndef __STLSOFT_DOCUMENTATION_SKIP_SECTION
# define MFCSTL_VER_H_MFCSTL_RESOURCE_STRING_MAJOR      2
# define MFCSTL_VER_H_MFCSTL_RESOURCE_STRING_MINOR      0
# define MFCSTL_VER_H_MFCSTL_RESOURCE_STRING_REVISION   1
# define MFCSTL_VER_H_MFCSTL_RESOURCE_STRING_EDIT       49
#endif /* !__STLSOFT_DOCUMENTATION_SKIP_SECTION */

/* /////////////////////////////////////////////////////////////////////////////
 * Includes
 */

#ifndef MFCSTL_INCL_H_MFCSTL
# include "mfcstl.h"    // Include the MFCSTL root header
#endif /* !MFCSTL_INCL_H_MFCSTL */

#if defined(__STLSOFT_COMPILER_IS_BORLAND)
# pragma warn -8022 /* Suppresses "'f()' hides virtual function 'g()'" */
# pragma warn -8084 /* Suppresses "Suggest parentheses to clarify precedence in function 'f()'" */
#endif /* compiler */

#include <afxwin.h>     // AfxThrowResourceException()

#if defined(__STLSOFT_COMPILER_IS_BORLAND)
# pragma warn .8022 /* Suppresses "'f()' hides virtual function 'g()'" */
# pragma warn .8084 /* Suppresses "Suggest parentheses to clarify precedence in function 'f()'" */
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

/// \weakgroup libraries_string String Library
/// \ingroup libraries
/// \brief This library provides facilities for defining and manipulating strings

/// \defgroup mfcstl_string_library String Library (MFCSTL)
/// \ingroup MFCSTL libraries_string
/// \brief This library provides facilities for defining and manipulating MFC string types
/// @{

/* ////////////////////////////////////////////////////////////////////////// */

// resource_string
/// Instances of this class represent Windows string resources, and are
/// constructed from instance identifiers.
class resource_string
    : public CString
{
private:
    typedef CString         parent_class_type;
    typedef resource_string class_type;
public:
    /// The type of the const (non-mutating) iterator
    typedef LPCTSTR         const_iterator;
    /// The size type
    typedef UINT            size_type;

// Construction
public:
    /// Constructs an around the string loaded from the given \c id
    ///
    /// \param id identifier of the string resource to load
    ss_explicit_k resource_string(ms_uint_t id) stlsoft_throw_2(CMemoryException*, CResourceException*);
    /// Constructs an around the string loaded from the given \c id and \c hinst
    ///
    /// \param hinst The module from which to load the string
    /// \param id identifier of the string resource to load
    resource_string(HINSTANCE hinst, ms_uint_t id) stlsoft_throw_2(CMemoryException*, CResourceException*);
    /// Copy constructor
    ///
    /// \param rhs The instance from which to copy-construct
    resource_string(resource_string const &rhs);
    /// Copy constructor
    ///
    /// \param rhs The instance from which to copy-construct
    resource_string(CString const &rhs);

    /// Copy assignment operator
    ///
    /// \param rhs The instance from which to copy-assign
    resource_string const &operator =(resource_string const &rhs);
    /// Copy assignment operator
    ///
    /// \param rhs The instance from which to copy-assign
    resource_string const &operator =(CString const &rhs);

// Iteration
public:
    /// Begins the iteration
    ///
    /// \return An iterator representing the start of the sequence
    const_iterator  begin() const;
    /// Ends the iteration
    ///
    /// \return An iterator representing the end of the sequence
    const_iterator  end() const;

// Attributes
public:
    /// Returns the number of elements in the sequence
    size_type size() const;
    /// Returns the number of elements in the sequence
    size_type length() const;
    /// Indicates whether the string is empty
    ms_bool_t empty() const;
    /// Returns a pointer to constant data representing the managed string
    LPCTSTR c_str() const;
};

/* /////////////////////////////////////////////////////////////////////////////
 * Shims
 */

inline LPCTSTR c_str_ptr_null(resource_string const &s)
{
    return s.empty() ? NULL : s.c_str();
}

inline LPCTSTR c_str_ptr(resource_string const &s)
{
    return s.c_str();
}

inline ms_size_t c_str_len(resource_string const &s)
{
    return s.length();
}

inline ms_size_t c_str_size(resource_string const &s)
{
    return c_str_len(s) * sizeof(TCHAR);
}

template<ss_typename_param_k S>
inline S &operator <<(S &s, resource_string const &str)
{
    s << str.c_str();

    return s;
}

///////////////////////////////////////////////////////////////////////////////
// Unit-testing

#ifdef STLSOFT_UNITTEST

namespace unittest
{
    ss_bool_t test_mfcstl_resource_string(unittest_reporter *r)
    {
        using stlsoft::unittest::unittest_initialiser;

        ss_bool_t               bSuccess    =   true;

        unittest_initialiser    init(r, "MFCSTL", "resource_string", __FILE__);

#if 0
        HINSTANCE       hinst   =   ::LoadLibrary("USER32");
        resource_string rs(hinst, 2);

        if(rs)
        {
            r->report("resource string load failed ", __LINE__);
            bSuccess = false;
        }
#endif /* 0 */

        return bSuccess;
    }

    unittest_registrar    unittest_mfcstl_resource_string(test_mfcstl_resource_string);

} // namespace unittest

#endif /* STLSOFT_UNITTEST */

////////////////////////////////////////////////////////////////////////////////
// Implementation

inline resource_string::resource_string(ms_uint_t id) stlsoft_throw_2(CMemoryException*, CResourceException*)
{
    if(!parent_class_type::LoadString(id))
    {
        AfxThrowResourceException();
    }
}

inline resource_string::resource_string(HINSTANCE hinst, ms_uint_t id) stlsoft_throw_2(CMemoryException*, CResourceException*)
{
    TCHAR sz[1024];

    if(0 == ::LoadString(hinst, id, sz, stlsoft_num_elements(sz)))
    {
        AfxThrowResourceException();
    }
    else
    {
        parent_class_type   *pThis = this;

        pThis->operator =(sz);
    }
}

inline resource_string::resource_string(resource_string const &rhs)
  : parent_class_type(rhs)
{}

inline resource_string::resource_string(CString const &rhs)
  : parent_class_type(rhs)
{}

inline resource_string const &resource_string::operator =(resource_string const &rhs)
{
    parent_class_type   *pThis = this;

    pThis->operator =(rhs);

    return *this;
}

inline resource_string const &resource_string::operator =(CString const &rhs)
{
    parent_class_type   *pThis = this;

    pThis->operator =(rhs);

    return *this;
}

inline resource_string::const_iterator resource_string::begin() const
{
    return *this;
}

inline resource_string::const_iterator resource_string::end() const
{
    return begin() + length();
}

inline resource_string::size_type resource_string::size() const
{
    return GetLength();
}

inline resource_string::size_type resource_string::length() const
{
    return GetLength();
}

inline ms_bool_t resource_string::empty() const
{
    return 0 == length();
}

inline LPCTSTR resource_string::c_str() const
{
    return *this;
}

/* ////////////////////////////////////////////////////////////////////////// */

/// @} // end of group mfcstl_string_library

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

/* /////////////////////////////////////////////////////////////////////////////
 * Namespace
 *
 * The string access shims exist either in the stlsoft namespace, or in the
 * global namespace. This is required by the lookup rules.
 *
 */

#ifndef _MFCSTL_NO_NAMESPACE
# if !defined(_STLSOFT_NO_NAMESPACE) && \
     !defined(__STLSOFT_DOCUMENTATION_SKIP_SECTION)
namespace stlsoft
{
# else /* ? _STLSOFT_NO_NAMESPACE */
/* There is no stlsoft namespace, so must define in the global namespace */
# endif /* !_STLSOFT_NO_NAMESPACE */

using ::mfcstl::c_str_ptr_null;

using ::mfcstl::c_str_ptr;

using ::mfcstl::c_str_len;

using ::mfcstl::c_str_size;

# if !defined(_STLSOFT_NO_NAMESPACE) && \
     !defined(__STLSOFT_DOCUMENTATION_SKIP_SECTION)
} // namespace stlsoft
# else /* ? _STLSOFT_NO_NAMESPACE */
/* There is no stlsoft namespace, so must define in the global namespace */
# endif /* !_STLSOFT_NO_NAMESPACE */
#endif /* !_MFCSTL_NO_NAMESPACE */

/* ////////////////////////////////////////////////////////////////////////// */

#endif /* !MFCSTL_INCL_H_MFCSTL_RESOURCE_STRING */

/* ////////////////////////////////////////////////////////////////////////// */
