/* /////////////////////////////////////////////////////////////////////////////
 * File:        winstl_acl_sequence.h
 *
 * Purpose:     Helper for accessing token information.
 *
 * Created:     26th June 2003
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


/// \file winstl_acl_sequence.h
///
/// Helper for accessing token information.

#ifndef WINSTL_INCL_H_WINSTL_ACL_SEQUENCE
#define WINSTL_INCL_H_WINSTL_ACL_SEQUENCE

#ifndef __STLSOFT_DOCUMENTATION_SKIP_SECTION
# define WINSTL_VER_H_WINSTL_ACL_SEQUENCE_MAJOR     2
# define WINSTL_VER_H_WINSTL_ACL_SEQUENCE_MINOR     0
# define WINSTL_VER_H_WINSTL_ACL_SEQUENCE_REVISION  1
# define WINSTL_VER_H_WINSTL_ACL_SEQUENCE_EDIT      11
#endif /* !__STLSOFT_DOCUMENTATION_SKIP_SECTION */

/* /////////////////////////////////////////////////////////////////////////////
 * Includes
 */

#ifndef WINSTL_INCL_H_WINSTL
# include "winstl.h"                        // Include the WinSTL root header
#endif /* WINSTL_INCL_H_WINSTL */

/* /////////////////////////////////////////////////////////////////////////////
 * Namespace
 */

#ifndef _WINSTL_NO_NAMESPACE
# if defined(_STLSOFT_NO_NAMESPACE) || \
     defined(__STLSOFT_DOCUMENTATION_SKIP_SECTION)
/* There is no stlsoft namespace, so must define ::winstl */
namespace winstl
{
# else
/* Define stlsoft::winstl_project */

namespace stlsoft
{

namespace winstl_project
{

# endif /* _STLSOFT_NO_NAMESPACE */
#endif /* !_WINSTL_NO_NAMESPACE */

/* ////////////////////////////////////////////////////////////////////////// */

/// \weakgroup libraries STLSoft Libraries
/// \brief The individual libraries

/// \weakgroup libraries_security Security Library
/// \ingroup libraries
/// \brief This library provides facilities for manipulating security APIs

/// \defgroup winstl_security_library Security Library (WinSTL)
/// \ingroup WinSTL libraries_security
/// \brief This library provides facilities for manipulating the Win32 security API
/// @{

/* /////////////////////////////////////////////////////////////////////////////
 * Classes
 */

/// \brief Provides an iterable sequence of ACEs in an ACL
///
/// This class provides an iterable sequence of Access Control Entries (ACEs) in
/// an Access Control List (ACL).
class acl_sequence
{
public:
    typedef acl_sequence    class_type;

public:
    ss_explicit_k acl_sequence(PACL pacl)
        : m_pacl(pacl)
    {}

public:
    /// The const_iterator for the acl_sequence class
    class const_iterator
    {
    protected:
        friend class acl_sequence;

        const_iterator(ws_uint32_t count, ACE_HEADER *header)
            : m_index(0)
            , m_count(count)
            , m_header(header)
        {}
    public:
        const_iterator()
            : m_index(0)
            , m_count(0)
            , m_header(0)
        {}

        const_iterator(const_iterator const &rhs)
            : m_index(rhs.m_index)
            , m_count(rhs.m_count)
            , m_header(rhs.m_header)
        {}

        const_iterator &operator =(const_iterator const &rhs)
        {
            m_index     =   rhs.m_index;
            m_count     =   rhs.m_count;
            m_header    =   rhs.m_header;

            return *this;
        }

        const_iterator &operator ++()
        {
            stlsoft_message_assert("Incrementing an invalid iterator", m_header != 0 && m_index < m_count);

            if(++m_index == m_count)
            {
                m_header = 0;
            }
            else
            {
                m_header = (ACE_HEADER*)ptr_byte_offset(m_header, m_header->AceSize);
            }

            return *this;
        }
        const_iterator operator ++(int)
        {
            const_iterator  ret(*this);

            operator ++();

            return ret;
        }

        ACE_HEADER *operator *() const
        {
            stlsoft_message_assert("Dereferencing an invalid iterator", m_header != 0);

            return m_header;
        }

        ws_bool_t operator == (const_iterator const &rhs)
        {
            return m_header == rhs.m_header;
        }
        ws_bool_t operator != (const_iterator const &rhs)
        {
            return !operator == (rhs);
        }

    // Members
    private:
        ws_uint32_t m_index;
        ws_uint32_t m_count;
        ACE_HEADER  *m_header;
    };

    const_iterator begin() const
    {
        return const_iterator(m_pacl->AceCount, (ACE_HEADER*)(m_pacl + 1));
    }
    const_iterator end() const
    {
        return const_iterator();
    }

public:
    ws_size_t   size() const
    {
        return m_pacl->AceCount;
    }
    ws_bool_t   empty() const
    {
        return 0 == size();
    }

// Members
private:
    PACL    m_pacl;

// Not to be implemented
private:
    acl_sequence(class_type const &);
    acl_sequence &operator =(class_type const &);
};

/* ////////////////////////////////////////////////////////////////////////// */

/// @} // end of group winstl_security_library

/* ////////////////////////////////////////////////////////////////////////// */

#ifndef _WINSTL_NO_NAMESPACE
# if defined(_STLSOFT_NO_NAMESPACE) || \
     defined(__STLSOFT_DOCUMENTATION_SKIP_SECTION)
} // namespace winstl
# else
} // namespace winstl_project
} // namespace stlsoft
# endif /* _STLSOFT_NO_NAMESPACE */
#endif /* !_WINSTL_NO_NAMESPACE */

/* ////////////////////////////////////////////////////////////////////////// */

#endif /* WINSTL_INCL_H_WINSTL_ACL_SEQUENCE */

/* ////////////////////////////////////////////////////////////////////////// */
