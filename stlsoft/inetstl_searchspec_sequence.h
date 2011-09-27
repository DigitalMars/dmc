/* /////////////////////////////////////////////////////////////////////////////
 * File:        inetstl_searchspec_sequence.h
 *
 * Purpose:     Contains the searchspec_sequence template class, and ANSI
 *              and Unicode specialisations thereof.
 *
 * Notes:       1. The original implementation of the class had the const_iterator
 *              and value_type as nested classes. Unfortunately, Visual C++ 5 &
 *              6 both had either compilation or linking problems so these are
 *              regretably now implemented as independent classes.
 *
 *              2. This class was described in detail in the article
 *              "Adapting Windows Enumeration Models to STL Iterator Concepts"
 *              (http://www.windevnet.com/documents/win0303a/), in the March
 *              2003 issue of Windows Developer Network (http://windevnet.com).
 *
 * Created:     1st May 2004
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


/// \file inetstl_searchspec_sequence.h
///
/// Contains the searchspec_sequence template class, and ANSI and Unicode specialisations thereof.

#ifndef INETSTL_INCL_H_INETSTL_SEARCHSPEC_SEQUENCE
#define INETSTL_INCL_H_INETSTL_SEARCHSPEC_SEQUENCE

#ifndef __STLSOFT_DOCUMENTATION_SKIP_SECTION
# define STLSOFT_VER_H_INETSTL_SEARCHSPEC_SEQUENCE_MAJOR        2
# define STLSOFT_VER_H_INETSTL_SEARCHSPEC_SEQUENCE_MINOR        0
# define STLSOFT_VER_H_INETSTL_SEARCHSPEC_SEQUENCE_REVISION     1
# define STLSOFT_VER_H_INETSTL_SEARCHSPEC_SEQUENCE_EDIT         11
#endif /* !__STLSOFT_DOCUMENTATION_SKIP_SECTION */

/* /////////////////////////////////////////////////////////////////////////////
 * Includes
 */

#ifndef INETSTL_INCL_H_INETSTL
# include "inetstl.h"                       // Include the InetSTL root header
#endif /* !INETSTL_INCL_H_INETSTL */
#ifndef STLSOFT_INCL_H_STLSOFT_SIMPLE_STRING
# include "stlsoft_simple_string.h"         // stlsoft::basic_simple_string
#endif /* !STLSOFT_INCL_H_STLSOFT_SIMPLE_STRING */
#ifndef STLSOFT_INCL_H_STLSOFT_STRING_TOKENISER
# include "stlsoft_string_tokeniser.h"
#endif /* !STLSOFT_INCL_H_STLSOFT_STRING_TOKENISER */

//#include <inetstl_string_access.h>


#ifndef __STLSOFT_COMPILER_IS_WATCOM
# ifndef STLSOFT_INCL_H_STLSOFT_ITERATOR
#  include "stlsoft_iterator.h"          // iterator_base
# endif /* !STLSOFT_INCL_H_STLSOFT_ITERATOR */
#endif /* __STLSOFT_COMPILER_IS_WATCOM */

/* /////////////////////////////////////////////////////////////////////////////
 * Pre-processor
 *
 * Definition of the
 */

/* /////////////////////////////////////////////////////////////////////////////
 * Namespace
 */

#ifndef _INETSTL_NO_NAMESPACE
# if defined(_STLSOFT_NO_NAMESPACE) || \
     defined(__STLSOFT_DOCUMENTATION_SKIP_SECTION)
/* There is no stlsoft namespace, so must define ::inetstl */
namespace inetstl
{
# else
/* Define stlsoft::inetstl_project */

namespace stlsoft
{

namespace inetstl_project
{

# endif /* _STLSOFT_NO_NAMESPACE */
#endif /* !_INETSTL_NO_NAMESPACE */

/* ////////////////////////////////////////////////////////////////////////// */

/// \weakgroup libraries STLSoft Libraries
/// \brief The individual libraries

/// \weakgroup libraries_filesystem File-System Library
/// \ingroup libraries
/// \brief This library provides facilities for defining and manipulating file-system objects

/// \weakgroup inetstl_filesystem_library File-System Library (InetSTL)
/// \ingroup STLSOFT libraries_filesystem
/// \brief This library provides facilities for defining and manipulating file-system objects for the Win32 API
/// @{

/* /////////////////////////////////////////////////////////////////////////////
 * Functions
 */

template <ss_typename_param_k T>
inline void call_set_null(T *&pt, void (T::*F)())
{
    inetstl_assert(NULL != pt);

    (pt->*F)();

    pt = NULL;
}

/* /////////////////////////////////////////////////////////////////////////////
 * Classes
 */

/// Provides multi-pattern functionality over a file-system search sequence class
template <ss_typename_param_k S>
class searchspec_sequence
{
public:
    /// The underlying find sequence type
    typedef S                                                           find_sequence_type;
    /// The current parameterisation of the type
    typedef searchspec_sequence<S>                                      class_type;
private:
    typedef searchspec_sequence<S>                                      outer_class_type;
    typedef ss_typename_type_k find_sequence_type::traits_type          traits_type;
public:
    /// The character type
    typedef ss_typename_type_k find_sequence_type::char_type            char_type;
    /// The value type
    typedef ss_typename_type_k find_sequence_type::value_type           value_type;
    /// The size type
    typedef ss_typename_type_k find_sequence_type::size_type            size_type;
    /// The non-mutable (const) reference type
    typedef ss_typename_type_k find_sequence_type::const_reference      const_reference;
    /// The non-mutating (const) iterator type
    class                                                               const_iterator;
private:
    typedef basic_simple_string<char_type>                              string_type;
    typedef string_tokeniser<string_type, char>                         tokeniser_type;

public:
    /// Default constructor
    searchspec_sequence()
        : m_hconn(NULL)
        , m_rootDir(null_str_())
        , m_searchSpec(null_str_())
        , m_delimiter('\0')
        , m_flags(0)
    {}
    /// Creates a search sequence for the given connection, search specification and delimiter
    ///
    /// \note Assumes the current directory
    searchspec_sequence(HINTERNET hconn, char_type const *searchSpec, char_type delimiter)
        : m_hconn(hconn)
        , m_rootDir(dot1_str_())
        , m_searchSpec(searchSpec)
        , m_delimiter(delimiter)
        , m_flags(0)
    {}
    /// Creates a search sequence for the given connection, search specification, delimiter and flags
    ///
    /// \note Assumes the current directory
    searchspec_sequence(HINTERNET hconn, char_type const *searchSpec, char_type delimiter, ss_int_t flags)
        : m_hconn(hconn)
        , m_rootDir(dot1_str_())
        , m_searchSpec(searchSpec)
        , m_delimiter(delimiter)
        , m_flags(flags)
    {}
    /// Creates a search sequence for the given connection, root directory, search specification and delimiter
    searchspec_sequence(HINTERNET hconn, char_type const *rootDir, char_type const *searchSpec, char_type delimiter)
        : m_hconn(hconn)
        , m_rootDir(rootDir)
        , m_searchSpec(searchSpec)
        , m_delimiter(delimiter)
        , m_flags(0)
    {}
    /// Creates a search sequence for the given connection, root directory, search specification, delimiter and flags
    searchspec_sequence(HINTERNET hconn, char_type const *rootDir, char_type const *searchSpec, char_type delimiter, ss_int_t flags)
        : m_hconn(hconn)
        , m_rootDir(rootDir)
        , m_searchSpec(searchSpec)
        , m_delimiter(delimiter)
        , m_flags(flags)
    {}

private:
    struct search_state
    {
        HINTERNET                                               m_hconn;
        string_type                                             m_rootDir;
        ss_int_t                                                m_flags;
        tokeniser_type                                          m_tokens;
        ss_typename_type_k tokeniser_type::const_iterator       m_tokensNext;
        ss_typename_type_k tokeniser_type::const_iterator       m_tokensEnd;
        find_sequence_type                                      *m_entries;
        ss_typename_type_k find_sequence_type::const_iterator   m_entriesNext;
        ss_typename_type_k find_sequence_type::const_iterator   m_entriesEnd;
        ss_sint32_t                                             m_cRefs;

    private:
        search_state(HINTERNET hconn, char_type const *rootDir, char_type const *searchSpec, char_type delimiter, ss_int_t flags)
            : m_hconn(hconn)
            , m_rootDir(rootDir)
            , m_flags(flags)
            , m_tokens(searchSpec, delimiter)
            , m_tokensNext(m_tokens.begin())
            , m_tokensEnd(m_tokens.end())
            , m_entries(new find_sequence_type(hconn, stlsoft::c_str_ptr(m_rootDir), stlsoft::c_str_ptr(*m_tokensNext), m_flags))
            , m_entriesNext(m_entries->begin())
            , m_entriesEnd(m_entries->end())
            , m_cRefs(1)
        {
            while(m_entriesNext == m_entriesEnd)
            {
                ++m_tokensNext;

                if(m_tokensNext == m_tokensEnd)
                {
                    break;
                }
                else
                {
                    inetstl_assert(NULL != m_entries);

                    stlsoft_destroy_instance_fn(m_entries);
                    m_entries = new(m_entries) find_sequence_type(m_hconn, stlsoft::c_str_ptr(m_rootDir), stlsoft::c_str_ptr(*m_tokensNext), m_flags);

                    m_entriesEnd    =   m_entries->end();
                    m_entriesNext   =   m_entries->begin();
                }
            }
        }

#if defined(__STLSOFT_COMPILER_IS_MSVC) && \
_MSC_VER < 1300
        friend class const_iterator;
#endif /* compiler */

    public:
        static search_state *create(HINTERNET hconn, char_type const *rootDir, char_type const *searchSpec, char_type delimiter, ss_int_t flags)
        {
            search_state *ss = new search_state(hconn, rootDir, searchSpec, delimiter, flags);

            if(ss->m_tokensNext == ss->m_tokensEnd)
            {
                delete ss;

                ss = NULL;
            }

            return ss;
        }

    public:
        bool next()
        {
            if(m_tokensNext == m_tokensEnd)
            {
                return false;
            }

            inetstl_assert(m_tokensNext != m_tokensEnd);
            inetstl_assert(NULL != m_entries || m_tokensNext == m_tokensEnd);

            // We are doing two enumerations here.
            //
            // The outer enumeration is over the tokens, the
            // inner is over the file entries.

            ++m_entriesNext;

            while(m_entriesNext == m_entriesEnd)
            {
                ++m_tokensNext;

                if(m_tokensNext == m_tokensEnd)
                {
                    return false;
                }
                else
                {
                    inetstl_assert(NULL != m_entries);

                    stlsoft_destroy_instance_fn(m_entries);
                    m_entries = new(m_entries) find_sequence_type(m_hconn, stlsoft::c_str_ptr(m_rootDir), stlsoft::c_str_ptr(*m_tokensNext), m_flags);

                    m_entriesEnd    =   m_entries->end();
                    m_entriesNext   =   m_entries->begin();
                }
            }

            return true;
        }

        void Release()
        {
            if(0 == --m_cRefs)
            {
                delete this;
            }
        }
#if defined(__STLSOFT_COMPILER_IS_GCC)
protected:
#else /* ? __STLSOFT_COMPILER_IS_GCC */
private:
#endif /* __STLSOFT_COMPILER_IS_GCC */
        ~search_state()
        {
            delete m_entries;
        }
    };

public:
    /// The const_iterator type for the searchspec_sequence
    class const_iterator
    {
    private:
        typedef const_iterator      class_type;

    private:
        friend class searchspec_sequence<S>;

        const_iterator(HINTERNET hconn, char_type const *rootDir, char_type const *searchSpec, char_type delimiter, ss_int_t flags)
            : m_searchState(search_state::create(hconn, rootDir, searchSpec, delimiter, flags))
        {}

    public:
        /// Default constructor
        const_iterator()
            : m_searchState(NULL)
        {}
        /// Destructor
        ~const_iterator()
        {
            if(NULL != m_searchState)
            {
                m_searchState->Release();
            }
        }

        /// Copy constructor
        const_iterator(class_type const &rhs)
            : m_searchState(rhs.m_searchState)
        {
            if(NULL != m_searchState)
            {
                ++m_searchState->m_cRefs;
            }
        }

        class_type &operator =(class_type const &rhs)
        {
            if(NULL != m_searchState)
            {
                m_searchState->Release();
            }

            m_searchState = rhs.m_searchState;

            if(NULL != m_searchState)
            {
                ++m_searchState->m_cRefs;
            }

            return *this;
        }

    public:
        class_type &operator ++()
        {
            inetstl_assert(NULL != m_searchState);

            if(!m_searchState->next())
            {
#if defined(__STLSOFT_COMPILER_IS_MSVC) && \
_MSC_VER < 1300
                m_searchState->Release();

                m_searchState = NULL;
#else /* ? compiler */
                call_set_null(m_searchState, &search_state::Release);
#endif /* compiler */
            }

            return *this;
        }

        class_type operator ++(int)
        {
            class_type  ret(*this);

            operator ++();

            return ret;
        }

        /// Dereference to return the value at the current position
        const value_type operator *() const
        {
            inetstl_assert(NULL != m_searchState);

            return *m_searchState->m_entriesNext;
        }

        ss_bool_t operator ==(class_type const &rhs) const
        {
            // Only evaluate the same when both are at their ends
            return (NULL == m_searchState) && (NULL == rhs.m_searchState);
        }
        ss_bool_t operator !=(class_type const &rhs) const
        {
            return !operator ==(rhs);
        }

    private:
        search_state    *m_searchState;
    };

public:
    /// Begins the iteration
    ///
    /// \return An iterator representing the start of the sequence
    const_iterator begin() const
    {
        return const_iterator(m_hconn, stlsoft::c_str_ptr(m_rootDir), stlsoft::c_str_ptr(m_searchSpec), m_delimiter, m_flags);
    }
    /// Ends the iteration
    ///
    /// \return An iterator representing the end of the sequence
    const_iterator end() const
    {
        return const_iterator();
    }

public:
    /// \brief Indicates whether the sequence is empty
    ss_bool_t empty() const
    {
        return begin() == end();
    }

// Implementation
private:
    static char_type const *null_str_()
    {
        static char_type    s_null[1] = { '\0' };

        return s_null;
    }
    static char_type const *dot1_str_()
    {
        static char_type    s_dot1[2] = { '.', '\0' };

        return s_dot1;
    }

private:
    HINTERNET   m_hconn;
    string_type m_rootDir;
    string_type m_searchSpec;
    char_type   m_delimiter;
    ss_int_t    m_flags;
};

///////////////////////////////////////////////////////////////////////////////
// Shims

template <ss_typename_param_k S>
inline ss_bool_t is_empty(searchspec_sequence<S> const &s)
{
    return s.empty();
}

////////////////////////////////////////////////////////////////////////////////
// Unit-testing

#ifdef STLSOFT_UNITTEST

namespace unittest
{
    ss_bool_t test_inetstl_searchspec_sequence(unittest_reporter *r)
    {
        ss_bool_t               bSuccess    =   true;

        unittest_initialiser    init(r, "STLSoft", "searchspec_sequence", __FILE__);

#if 0
        if(<<TODO>>)
        {
            r->report("<<TODO>> failed ", __LINE__);
            bSuccess = false;
        }
#endif /* 0 */

        return bSuccess;
    }

    unittest_registrar    unittest_inetstl_searchspec_sequence(test_inetstl_searchspec_sequence);

} // namespace unittest

#endif /* STLSOFT_UNITTEST */

///////////////////////////////////////////////////////////////////////////////
// Implementation

#ifndef __STLSOFT_DOCUMENTATION_SKIP_SECTION


#endif /* !__STLSOFT_DOCUMENTATION_SKIP_SECTION */

/* ////////////////////////////////////////////////////////////////////////// */

/// @} // end of group inetstl_filesystem_library

/* ////////////////////////////////////////////////////////////////////////// */

#ifndef _INETSTL_NO_NAMESPACE
# if defined(_STLSOFT_NO_NAMESPACE) || \
     defined(__STLSOFT_DOCUMENTATION_SKIP_SECTION)
} // namespace inetstl
# else
} // namespace inetstl_project
} // namespace stlsoft
# endif /* _STLSOFT_NO_NAMESPACE */
#endif /* !_INETSTL_NO_NAMESPACE */

/* ////////////////////////////////////////////////////////////////////////// */

#endif /* INETSTL_INCL_H_INETSTL_SEARCHSPEC_SEQUENCE */

/* ////////////////////////////////////////////////////////////////////////// */
