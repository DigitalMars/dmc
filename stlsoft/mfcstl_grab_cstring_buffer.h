/* ////////////////////////////////////////////////////////////////////////////
 * File:        mfcstl_grab_cstring_buffer.h (Formerly MFGrbStr.h: ::SynesisMfc)
 *
 * Purpose:     CString Get/ReleaseBuffer scoping class.
 *
 * Created:     12th February 1999
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


/// \file mfcstl_grab_cstring_buffer.h
///
/// CString Get/ReleaseBuffer scoping class.

#ifndef MFCSTL_INCL_H_MFCSTL_GRAB_CSTRING_BUFFER
#define MFCSTL_INCL_H_MFCSTL_GRAB_CSTRING_BUFFER

#ifndef __STLSOFT_DOCUMENTATION_SKIP_SECTION
# define MFCSTL_VER_H_MFCSTL_GRAB_CSTRING_BUFFER_MAJOR      2
# define MFCSTL_VER_H_MFCSTL_GRAB_CSTRING_BUFFER_MINOR      0
# define MFCSTL_VER_H_MFCSTL_GRAB_CSTRING_BUFFER_REVISION   1
# define MFCSTL_VER_H_MFCSTL_GRAB_CSTRING_BUFFER_EDIT       37
#endif /* !__STLSOFT_DOCUMENTATION_SKIP_SECTION */

/* /////////////////////////////////////////////////////////////////////////////
 * Includes
 */

#ifndef MFCSTL_INCL_H_MFCSTL
# include <mfcstl.h> // Include the MFCSTL root header
#endif /* !MFCSTL_INCL_H_MFCSTL */

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

/* ////////////////////////////////////////////////////////////////////////////
 * grab_cstring_buffer
 *
 * This class is used to scope the aquisition and release of a CString buffer
 * ensuring that ReleaseBuffer() is called on the given CString object.
 */

/// \brief Scopes the aquisition and release of a CString buffer
///
/// This class is used to scope the aquisition and release of a CString buffer
/// ensuring that ReleaseBuffer() is called on the given CString object.
class grab_cstring_buffer
{
private:
    typedef grab_cstring_buffer   class_type;

// Construction
public:
    /// \brief Acquires the requested length on the given managed string
    ///
    /// Calls \c GetBuffer(length) on the given string, after recording the
    /// original length. If acquisition fails then a CMemoryException is thrown.
    grab_cstring_buffer(CString &str, int length) stlsoft_throw_1(CMemoryException *);
    /// Releases the managed string.
    ~grab_cstring_buffer() stlsoft_throw_0();

// Conversion operators
public:
    /// Returns a pointer to a nul-terminated string
    LPCTSTR c_str() const;

    /// Provides mutating access to the managed string's internal buffer
    operator LPTSTR();
#if !defined(__STLSOFT_COMPILER_IS_BORLAND)
    /// Provides non-mutating access to the managed string's internal buffer
    operator LPCTSTR() const;
#endif /* compiler */

// Attributes
public:
    /// Returns the length of the managed string
    int length() const;
    /// Returns the original length of the managed string
    int original_length() const;

// Members
private:
    CString         &m_str;
    const int       m_len;
    const int       m_originalLen;
    const LPTSTR    m_psz;

// Not implemented
private:
    grab_cstring_buffer(class_type const &rhs);
    const grab_cstring_buffer &operator =(class_type const &rhs);
};

///////////////////////////////////////////////////////////////////////////////
// Unit-testing

#ifdef STLSOFT_UNITTEST

namespace unittest
{
    ss_bool_t test_mfcstl_grab_cstring_buffer(unittest_reporter *r)
    {
        using stlsoft::unittest::unittest_initialiser;

        ss_bool_t               bSuccess    =   true;

        unittest_initialiser    init(r, "MFCSTL", "grab_cstring_buffer", __FILE__);

        CString             str("Str");

        try
        {
            grab_cstring_buffer gcs(str, 6);

            gcs[3] = 'i';
            gcs[4] = 'n';
            gcs[5] = 'g';
        }
        catch(CMemoryException *pe)
        {
            pe->ReportError();
        }
        catch(CException *pe)
        {
            pe->ReportError();
        }

        if(str != "String")
        {
            r->report("grab_cstring_buffer failed ", __LINE__);
            bSuccess = false;
        }

        return bSuccess;
    }

    unittest_registrar    unittest_mfcstl_grab_cstring_buffer(test_mfcstl_grab_cstring_buffer);

} // namespace unittest

#endif /* STLSOFT_UNITTEST */

////////////////////////////////////////////////////////////////////////////////
// Implementation

inline grab_cstring_buffer::grab_cstring_buffer(CString &str, int length) stlsoft_throw_1(CMemoryException *)
    : m_str(str)
    , m_len(length)
    , m_originalLen(str.GetLength())
    , m_psz(str.GetBuffer(length))
{
    // Fortunately, objects of this class contain no resources of their own, and so
    // throwing an exception from the constructor is somewhat benign.

    // Newer versions of MFC do not append a nul character to the end of the reallocated
    // area, so we do that now
    m_psz[m_len] = '\0';
}

inline grab_cstring_buffer::~grab_cstring_buffer() stlsoft_throw_0()
{
    // Best to check that the end character has not been overwritten
    mfcstl_message_assert("The client code has overwritten the managed area of the grab_cstring_buffer instance", '\0' == m_psz[m_len]);

    // Dtor will never be called if GetBufferSetLength throws,
    // so we can assume we have a valid reference to an open
    // string which needs to be released here.
    m_str.ReleaseBuffer();
}

LPCTSTR grab_cstring_buffer::c_str() const
{
    return m_psz;
}

inline grab_cstring_buffer::operator LPTSTR()
{
    return m_psz;
}

#if !defined(__STLSOFT_COMPILER_IS_BORLAND)
inline grab_cstring_buffer::operator LPCTSTR() const
{
    return m_psz;
}
#endif /* compiler */

inline int grab_cstring_buffer::length() const
{
    return m_len;
}

inline int grab_cstring_buffer::original_length() const
{
    return m_originalLen;
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

/* ////////////////////////////////////////////////////////////////////////// */

#endif /* !MFCSTL_INCL_H_MFCSTL_GRAB_CSTRING_BUFFER */

/* ////////////////////////////////////////////////////////////////////////// */
