/* /////////////////////////////////////////////////////////////////////////////
 * File:        stlsoft_environment_block.h
 *
 * Purpose:     Contains the basic_environment_block class.
 *
 * Created:     25th June 2004
 * Updated:     12th September 2004
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


/// \file stlsoft_environment_block.h
///
/// Contains the basic_environment_block class.

#ifndef STLSOFT_INCL_H_STLSOFT_ENVIRONMENT_BLOCK
#define STLSOFT_INCL_H_STLSOFT_ENVIRONMENT_BLOCK

#ifndef __STLSOFT_DOCUMENTATION_SKIP_SECTION
# define STLSOFT_VER_H_STLSOFT_ENVIRONMENT_BLOCK_MAJOR    2
# define STLSOFT_VER_H_STLSOFT_ENVIRONMENT_BLOCK_MINOR    0
# define STLSOFT_VER_H_STLSOFT_ENVIRONMENT_BLOCK_REVISION 1
# define STLSOFT_VER_H_STLSOFT_ENVIRONMENT_BLOCK_EDIT     3
#endif /* !__STLSOFT_DOCUMENTATION_SKIP_SECTION */

/* /////////////////////////////////////////////////////////////////////////////
 * Compatibility
 */

/* /////////////////////////////////////////////////////////////////////////////
 * Includes
 */

#ifndef STLSOFT_INCL_H_STLSOFT
# include "stlsoft.h"                   // Include the STLSoft root header
#endif /* !STLSOFT_INCL_H_STLSOFT */
#ifndef STLSOFT_INCL_H_STLSOFT_AUTO_BUFFER
# include "stlsoft_auto_buffer.h"       // auto_buffer
#endif /* !STLSOFT_INCL_H_STLSOFT_AUTO_BUFFER */
#ifndef STLSOFT_INCL_H_STLSOFT_PROCESSHEAP_ALLOCATOR
# include "stlsoft_malloc_allocator.h"  // malloc_allocator
#endif /* !STLSOFT_INCL_H_STLSOFT_PROCESSHEAP_ALLOCATOR */
#ifndef STLSOFT_INCL_H_STLSOFT_CHAR_TRAITS
# include "stlsoft_char_traits.h"       // char_traits
#endif /* !STLSOFT_INCL_H_STLSOFT_CHAR_TRAITS */
#ifndef STLSOFT_INCL_H_STLSOFT_STRING_ACCESS
# include "stlsoft_string_access.h"     // c_str_ptr
#endif /* !STLSOFT_INCL_H_STLSOFT_STRING_ACCESS */

/* /////////////////////////////////////////////////////////////////////////////
 * Namespace
 */

#ifndef _STLSOFT_NO_NAMESPACE
namespace stlsoft
{
#endif /* _STLSOFT_NO_NAMESPACE */

/* ////////////////////////////////////////////////////////////////////////// */

/// \weakgroup libraries STLSoft Libraries
/// \brief The individual libraries

/// \weakgroup libraries_system System Library
/// \ingroup libraries
/// \brief This library provides facilities for defining and manipulating operating-system specific constructs
/// @{

/* /////////////////////////////////////////////////////////////////////////////
 * Classes
 */

/// Class used for preparing environment blocks
template<   ss_typename_param_k C
#ifdef __STLSOFT_CF_TEMPLATE_CLASS_DEFAULT_CLASS_ARGUMENT_SUPPORT
        ,   ss_typename_param_k T = char_traits<C>
        ,   ss_typename_param_k A = malloc_allocator<C>
#else /* ? __STLSOFT_CF_TEMPLATE_CLASS_DEFAULT_CLASS_ARGUMENT_SUPPORT */
        ,   ss_typename_param_k T /* = char_traits<C> */
        ,   ss_typename_param_k A /* = malloc_allocator<C> */
#endif /* __STLSOFT_CF_TEMPLATE_CLASS_DEFAULT_CLASS_ARGUMENT_SUPPORT */
        >
class basic_environment_block
{
/// \name Types
/// @{
public:
    /// The value type
    typedef C                                   value_type;
    /// The char type
    typedef C                                   char_type;
    /// The traits type
    typedef T                                   traits_type;
    /// The allocator type
    typedef A                                   allocator_type;
    /// The current parameterisation of the type
    typedef basic_environment_block<C, T, A>    class_type;
    /// The mutating (non-const) pointer type
    typedef char_type                           *pointer;
    /// The non-mutating (const) pointer type
    typedef char_type const                     *const_pointer;
    /// The size type
    typedef ss_size_t                           size_type;
/// @}

/// \name Construction
/// @{
public:
    basic_environment_block()
        : m_chars(1)
        , m_offsets(1)
        , m_pointers(0)
    {
        m_chars[0]      =   '\0';
        m_offsets[0]    =   0;
    }
/// @}

/// \name Operations
/// @{
public:
    /// \brief Append a full NAME=VALUE environment pair
    void push_back(char_type const *s, size_t cch)
    {
        stlsoft_assert(NULL != s);
        stlsoft_assert(cch >= 3);
        stlsoft_assert(NULL != traits_type::find(s, cch, '='));

        stlsoft_assert(m_chars.size() > 0);
        stlsoft_assert('\0' == m_chars[m_chars.size() - 1]);

        const size_type   numChars_     =   m_chars.size();
        const size_type   numOffsets_       =   m_offsets.size();
        const size_type   numPointers_  =   m_pointers.size();

        m_chars.resize(numChars_ + cch + 1);
        // The next item
        traits_type::copy(&m_chars[numChars_ - 1], s, cch);
        m_chars[numChars_ + cch - 1] = '\0';
        // The terminating item
        m_chars[numChars_ + cch] = '\0';

        m_offsets.resize(numOffsets_ + 1);
        m_offsets[numOffsets_] = numChars_ + cch;

        m_pointers.resize(0);

        stlsoft_assert('\0' == m_chars[m_chars.size() - 1]);
    }
    template <typename S>
    void push_back(S const &s)
    {
        push_back(stlsoft_ns_qual(c_str_ptr)(s), stlsoft_ns_qual(c_str_len)(s));
    }
    void push_back(char_type const *name, size_t cchName, char_type const *value, size_t cchValue)
    {
        stlsoft_assert(NULL != name);
        stlsoft_assert(NULL != value);
        stlsoft_assert(cchName > 0);
        stlsoft_assert(cchValue > 0);
        stlsoft_assert(NULL == traits_type::find(name, cchName, '='));

        stlsoft_assert(m_chars.size() > 0);
        stlsoft_assert('\0' == m_chars[m_chars.size() - 1]);

        const size_type   numChars_     =   m_chars.size();
        const size_type   numOffsets_       =   m_offsets.size();
        const size_type   numPointers_  =   m_pointers.size();

        m_chars.resize(numChars_ + cchName + 1 + cchValue + 1);
        // The next item

        traits_type::copy(&m_chars[numChars_ - 1], name, cchName);
        m_chars[numChars_ - 1 + cchName] = '=';
        traits_type::copy(&m_chars[numChars_ - 1 + cchName + 1], value, cchValue);
        m_chars[numChars_ - 1 + cchName + 1 + cchValue] = '\0';
        // The terminating item
        m_chars[numChars_ + cchName + 1 + cchValue] = '\0';

        m_offsets.resize(numOffsets_ + 1);
        m_offsets[numOffsets_] = numChars_ + cchName + 1 + cchValue;

        m_pointers.resize(0);

        stlsoft_assert('\0' == m_chars[m_chars.size() - 1]);
    }
    template <typename S1, typename S2>
    void push_back(S1 const &name, S2 const &value)
    {
        push_back(stlsoft_ns_qual(c_str_ptr)(name), stlsoft_ns_qual(c_str_len)(name), stlsoft_ns_qual(c_str_ptr)(value), stlsoft_ns_qual(c_str_len)(value));
    }

    void clear()
    {
        m_chars.resize(1);
        m_offsets.resize(1);
        m_pointers.resize(0);

        m_chars[0]      =   '\0';
        m_offsets[0]    =   0;
    }
/// @}

/// \name Accessors
/// @{
public:
    char_type const *const *base() const
    {
        if(m_pointers.size() != m_offsets.size())
        {
            set_pointers();
        }

        return m_pointers.data();
    }
    size_type size() const
    {
        stlsoft_assert(m_offsets.size() >= 1);

        return m_offsets.size() - 1;
    }
/// @}

// Implementation
private:
    void set_pointers()
    {
        if(m_pointers.resize(m_offsets.size()))
        {
            for(size_type i = 0; i < m_offsets.size(); ++i)
            {
                m_pointers[i] = &m_chars[m_offsets[i]];
            }
        }
    }

    void set_pointers() const
    {
        const_cast<class_type*>(this)->set_pointers();
    }

// Members
private:
    typedef stlsoft_ns_qual(auto_buffer)<char_type, allocator_type, 1024>                           char_buffer_type;
#if !defined(__STLSOFT_COMPILER_IS_MSVC) || \
    _MSC_VER >= 1300
    typedef stlsoft_ns_qual(auto_buffer)<size_type, allocator_type::rebind<size_type>::other, 32>   offset_buffer_type;
    typedef stlsoft_ns_qual(auto_buffer)<const_pointer, allocator_type::rebind<pointer>::other, 32> pointer_buffer_type;
#else /* ? compiler */
    typedef stlsoft_ns_qual(auto_buffer)<size_type, malloc_allocator<size_type>, 32>                offset_buffer_type;
    typedef stlsoft_ns_qual(auto_buffer)<const_pointer, malloc_allocator<pointer>, 32>              pointer_buffer_type;
#endif /* ? compiler */

    char_buffer_type        m_chars;
    offset_buffer_type      m_offsets;
    pointer_buffer_type     m_pointers;
};

/* /////////////////////////////////////////////////////////////////////////////
 * Typedefs for commonly encountered types
 */

#ifdef __STLSOFT_CF_TEMPLATE_CLASS_DEFAULT_CLASS_ARGUMENT_SUPPORT

 /// Instantiation of the basic_path template for the ANSI character type \c char
typedef basic_environment_block<ss_char_a_t>    environment_block_a;
/// Instantiation of the basic_environment_block template for the Unicode character type \c wchar_t
typedef basic_environment_block<ss_char_w_t>    environment_block_w;

#endif /* __STLSOFT_CF_TEMPLATE_CLASS_DEFAULT_CLASS_ARGUMENT_SUPPORT */

/* /////////////////////////////////////////////////////////////////////////////
 * Unit-testing
 */

#ifdef STLSOFT_UNITTEST

namespace unittest
{
    ss_bool_t test_stlsoft_environment_block(unittest_reporter *r)
    {
        using stlsoft::unittest::unittest_initialiser;

        ss_bool_t               bSuccess    =   true;

        unittest_initialiser    init(r, "STLSoft", "environment_block", __FILE__);

        return bSuccess;
    }

    unittest_registrar    unittest_stlsoft_environment_block(test_stlsoft_environment_block);

} // namespace unittest

#endif /* STLSOFT_UNITTEST */

/* /////////////////////////////////////////////////////////////////////////////
 * Implementation
 */

/* ////////////////////////////////////////////////////////////////////////// */

/// @} // end of group stlsoft_system_library

/* ////////////////////////////////////////////////////////////////////////// */

#ifndef _STLSOFT_NO_NAMESPACE
} // namespace stlsoft
#endif /* _STLSOFT_NO_NAMESPACE */

/* ////////////////////////////////////////////////////////////////////////// */

#endif /* STLSOFT_INCL_H_STLSOFT_ENVIRONMENT_BLOCK */

/* ////////////////////////////////////////////////////////////////////////// */
