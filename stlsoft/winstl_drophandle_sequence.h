/* /////////////////////////////////////////////////////////////////////////////
 * File:        winstl_drophandle_sequence.h
 *
 * Purpose:     Contains the basic_drophandle_sequence template class, and ANSI
 *              and Unicode specialisations thereof.
 *
 * Created:     13th November 2002
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


/// \file winstl_drophandle_sequence.h
///
/// Contains the basic_drophandle_sequence template class, and ANSI and Unicode specialisations thereof.

#ifndef WINSTL_INCL_H_WINSTL_DROPHANDLE_SEQUENCE
#define WINSTL_INCL_H_WINSTL_DROPHANDLE_SEQUENCE

#ifndef __STLSOFT_DOCUMENTATION_SKIP_SECTION
# define WINSTL_VER_H_WINSTL_DROPHANDLE_SEQUENCE_MAJOR        2
# define WINSTL_VER_H_WINSTL_DROPHANDLE_SEQUENCE_MINOR        0
# define WINSTL_VER_H_WINSTL_DROPHANDLE_SEQUENCE_REVISION     1
# define WINSTL_VER_H_WINSTL_DROPHANDLE_SEQUENCE_EDIT         45
#endif /* !__STLSOFT_DOCUMENTATION_SKIP_SECTION */

/* /////////////////////////////////////////////////////////////////////////////
 * Includes
 */

#ifndef WINSTL_INCL_H_WINSTL
# include "winstl.h"                        // Include the WinSTL root header
#endif /* !WINSTL_INCL_H_WINSTL */
#ifndef WINSTL_INCL_H_WINSTL_FILE_PATH_BUFFER
# include "winstl_file_path_buffer.h"       // basic_file_path_buffer
#endif /* !WINSTL_INCL_H_WINSTL_FILE_PATH_BUFFER */
#ifndef WINSTL_INCL_H_WINSTL_PROCESSHEAP_ALLOCATOR
# include "winstl_processheap_allocator.h"  // winstl::processheap_allocator
#endif /* !WINSTL_INCL_H_WINSTL_PROCESSHEAP_ALLOCATOR */
#ifndef STLSOFT_INCL_H_STLSOFT_AUTO_BUFFER
# include "stlsoft_auto_buffer.h"           // stlsoft::auto_buffer
#endif /* !STLSOFT_INCL_H_STLSOFT_AUTO_BUFFER */
#if defined(__STLSOFT_CF_FRAME_STRING_SHIM_NOT_SUPPORTED) || \
    (   defined(__STLSOFT_COMPILER_IS_MSVC) && \
        _MSC_VER < 1200)
# ifndef STLSOFT_INCL_H_STLSOFT_SIMPLE_STRING
#  include "stlsoft_simple_string.h"        // stlsoft::basic_simple_string
# endif /* !STLSOFT_INCL_H_STLSOFT_SIMPLE_STRING */
#else /* ? __STLSOFT_CF_FRAME_STRING_SHIM_NOT_SUPPORTED */
# ifndef STLSOFT_INCL_H_STLSOFT_STATIC_STRING
#  include "stlsoft_static_string.h"         // stlsoft::basic_static_string
# endif /* !STLSOFT_INCL_H_STLSOFT_STATIC_STRING */
#endif /* __STLSOFT_CF_FRAME_STRING_SHIM_NOT_SUPPORTED */
#ifndef STLSOFT_INCL_H_STLSOFT_ITERATOR
# include "stlsoft_iterator.h"              // stlsoft::iterator, stlsoft::reverse_iterator
#endif /* STLSOFT_INCL_H_STLSOFT_ITERATOR */

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

/* /////////////////////////////////////////////////////////////////////////////
 * Typedefs
 */

#ifdef __STLSOFT_DROPHANDLE_SEQUENCE_VALUE_TYPE
# undef __STLSOFT_DROPHANDLE_SEQUENCE_VALUE_TYPE
#endif /* __STLSOFT_DROPHANDLE_SEQUENCE_VALUE_TYPE */

#if defined(__STLSOFT_CF_FRAME_STRING_SHIM_NOT_SUPPORTED) || \
    (   defined(__STLSOFT_COMPILER_IS_MSVC) && \
        _MSC_VER < 1200)
# define __STLSOFT_DROPHANDLE_SEQUENCE_VALUE_TYPE(C)    stlsoft_ns_qual(basic_simple_string)<C, stlsoft_ns_qual(char_traits)<C>, processheap_allocator<C> >
#else /* ? __STLSOFT_CF_FRAME_STRING_SHIM_NOT_SUPPORTED */
# define __STLSOFT_DROPHANDLE_SEQUENCE_VALUE_TYPE(C)    stlsoft_ns_qual(basic_static_string)<C, _MAX_PATH>
#endif /* __STLSOFT_CF_FRAME_STRING_SHIM_NOT_SUPPORTED */

/* /////////////////////////////////////////////////////////////////////////////
 * Forward declarations
 */

#ifndef __STLSOFT_DOCUMENTATION_SKIP_SECTION

template <ss_typename_param_k C, ss_typename_param_k T>
class basic_drophandle_sequence;

#endif /* __STLSOFT_DOCUMENTATION_SKIP_SECTION */

/* /////////////////////////////////////////////////////////////////////////////
 * Classes
 */

#ifdef __STLSOFT_DOCUMENTATION_SKIP_SECTION
/// Traits for accessing the correct drop-handle functions for a given character type
///
/// drophandle_sequence_traits is a traits class for determining the correct
/// drop-handle structures and functions for a given character type.
///
/// \param C The character type
template <ss_typename_param_k C>
struct drophandle_sequence_traits
{
    /// Queries the information from the given drop handle
    ///
    /// \param hdrop The drop handle
    /// \param index The index of the requested item
    /// \param buffer The buffer within which to write the results
    /// \param cchBuffer The size of \c buffer in characters
    static ws_uint_t drag_query_file(HDROP hdrop, ws_uint_t index, C *buffer, ws_uint_t cchBuffer);
};

#else

template <ss_typename_param_k C>
struct drophandle_sequence_traits;

STLSOFT_TEMPLATE_SPECIALISATION

struct drophandle_sequence_traits<ws_char_a_t>
{
    static ws_uint_t drag_query_file(HDROP hdrop, ws_uint_t index, ws_char_a_t *buffer, ws_uint_t cchBuffer)
    {
        return DragQueryFileA(hdrop, index, buffer, cchBuffer);
    }
};

STLSOFT_TEMPLATE_SPECIALISATION
struct drophandle_sequence_traits<ws_char_w_t>
{
    static ws_uint_t drag_query_file(HDROP hdrop, ws_uint_t index, ws_char_w_t *buffer, ws_uint_t cchBuffer)
    {
        return DragQueryFileW(hdrop, index, buffer, cchBuffer);
    }
};

#endif /* __STLSOFT_DOCUMENTATION_SKIP_SECTION */


// class basic_drophandle_sequence_const_iterator
/// Iterator for the basic_drophandle_sequence class
///
/// \param C The character type
/// \param T The traits type. For translators that support default template arguments this defaults to drophandle_sequence_traits<C>
template<   ss_typename_param_k C
#ifdef __STLSOFT_CF_TEMPLATE_CLASS_DEFAULT_CLASS_ARGUMENT_SUPPORT
        ,   ss_typename_param_k T = drophandle_sequence_traits<C>
#else
        ,   ss_typename_param_k T /* = drophandle_sequence_traits<C> */
#endif /* __STLSOFT_CF_TEMPLATE_CLASS_DEFAULT_CLASS_ARGUMENT_SUPPORT */
        >
class basic_drophandle_sequence_const_iterator
    : public stlsoft_ns_qual(iterator_base)<winstl_ns_qual_std(bidirectional_iterator_tag), __STLSOFT_DROPHANDLE_SEQUENCE_VALUE_TYPE(C), ws_ptrdiff_t, void, __STLSOFT_DROPHANDLE_SEQUENCE_VALUE_TYPE(C)>
{
public:
    /// The character type
    typedef C                                                   char_type;
    /// The traits type
    typedef T                                                   traits_type;
    /// The string type
    typedef __STLSOFT_DROPHANDLE_SEQUENCE_VALUE_TYPE(C)         string_type;
    /// The current parameterisation of the type
    typedef basic_drophandle_sequence_const_iterator<C, T>      class_type;
    /// The value type
    typedef string_type                                         value_type;
    /// The index type
    typedef ws_uint_t                                           index_type;

// Construction
protected:
    /// Create an iterator based on the given drop handle
    ss_explicit_k basic_drophandle_sequence_const_iterator(HDROP hdrop, index_type index);
public:
    // Default constructor
    basic_drophandle_sequence_const_iterator();
    /// Copy constructor
    basic_drophandle_sequence_const_iterator(class_type const &rhs);

    /// Copy assignment operator
    basic_drophandle_sequence_const_iterator &operator =(class_type const &rhs);

// Operators
public:
    /// Pre-increment operator
    class_type &operator ++();
    /// Pre-decrement operator
    class_type &operator --();
    /// Post-increment operator
    class_type operator ++(int);
    /// Post-decrement operator
    class_type operator --(int);
    /// Dereference
    const value_type operator *() const;
    /// Evaluates whether \c this and \c rhs are equivalent
    ws_bool_t operator ==(class_type const &rhs) const;
    /// Evaluates whether \c this and \c rhs are not equivalent
    ws_bool_t operator !=(class_type const &rhs) const;

// Implementation
private:
    static index_type sentinel_();

// Members
private:
    friend class basic_drophandle_sequence<C, T>;

    HDROP       m_hdrop;
    index_type  m_index;
};


// class basic_drophandle_sequence
/// This class wraps an STL-like sequence interface over a Win32 drop handle
///
/// \param C The character type
/// \param T The traits type. For translators that support default template arguments this defaults to drophandle_sequence_traits<C>
template<   ss_typename_param_k C
#ifdef __STLSOFT_CF_TEMPLATE_CLASS_DEFAULT_CLASS_ARGUMENT_SUPPORT
        ,   ss_typename_param_k T = drophandle_sequence_traits<C>
#else
        ,   ss_typename_param_k T /* = drophandle_sequence_traits<C> */
#endif /* __STLSOFT_CF_TEMPLATE_CLASS_DEFAULT_CLASS_ARGUMENT_SUPPORT */
        >
class basic_drophandle_sequence
{
public:
    /// The character type
    typedef C                                                                               char_type;
    /// The traits type
    typedef T                                                                               traits_type;
    /// The current parameterisation of the type
    typedef basic_drophandle_sequence<C, T>                                                 class_type;
    /// The non-mutating (const) iterator type
    typedef basic_drophandle_sequence_const_iterator<C, T>                                  const_iterator;
    /// The value type
    typedef ss_typename_type_k const_iterator::value_type                                   value_type;
    /// The difference type
    typedef ws_ptrdiff_t                                                                    difference_type;
#if defined(__STLSOFT_CF_BIDIRECTIONAL_ITERATOR_SUPPORT)
    /// The non-mutating (const) reverse iterator type
    typedef stlsoft_ns_qual(const_reverse_bidirectional_iterator_base)< const_iterator,
                                                                        value_type,
                                                                        value_type, // Return by value!
                                                                        void*,
                                                                        difference_type>    const_reverse_iterator;
#endif /* __STLSOFT_CF_BIDIRECTIONAL_ITERATOR_SUPPORT */

// Construction
public:
    /// Construct a sequence based on the contents of the given drop handle
    ss_explicit_k basic_drophandle_sequence(HDROP hdrop);
    /// Release any resources aquired
    ~basic_drophandle_sequence() winstl_throw_0();

// Attributes
public:
    /// The number of elements in the sequence
    ws_size_t size() const;
    /// Indicates whether the sequence is empty
    ws_bool_t empty() const;

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

#if defined(__STLSOFT_CF_BIDIRECTIONAL_ITERATOR_SUPPORT)
    /// Begins the reverse iteration
    ///
    /// \return An iterator representing the start of the reverse sequence
    const_reverse_iterator  rbegin() const;
    /// Ends the reverse iteration
    ///
    /// \return An iterator representing the end of the reverse sequence
    const_reverse_iterator  rend() const;
#endif /* __STLSOFT_CF_BIDIRECTIONAL_ITERATOR_SUPPORT */

// Members
private:
    HDROP   m_hdrop;

// Implementation
private:

// Not to be implemented
private:
    basic_drophandle_sequence(class_type const &);
    const basic_drophandle_sequence &operator =(class_type const &);
};

/* Typedefs to commonly encountered types. */
/// Instantiation of the basic_drophandle_sequence template for the ANSI character type \c char
typedef basic_drophandle_sequence<ws_char_a_t, drophandle_sequence_traits<ws_char_a_t> >  drophandle_sequence_a;
/// Instantiation of the basic_drophandle_sequence template for the Unicode character type \c wchar_t
typedef basic_drophandle_sequence<ws_char_w_t, drophandle_sequence_traits<ws_char_w_t> >  drophandle_sequence_w;

///////////////////////////////////////////////////////////////////////////////

// basic_drophandle_sequence_const_iterator

template<   ss_typename_param_k C
        ,   ss_typename_param_k T
        >
inline /* static */ ss_typename_type_k basic_drophandle_sequence_const_iterator<C, T>::index_type basic_drophandle_sequence_const_iterator<C, T>::sentinel_()
{
    return 0x7fffffff;
}

template<   ss_typename_param_k C
        ,   ss_typename_param_k T
        >
inline /* ss_explicit_k */ basic_drophandle_sequence_const_iterator<C, T>::basic_drophandle_sequence_const_iterator(HDROP hdrop, ss_typename_type_k basic_drophandle_sequence_const_iterator<C, T>::index_type index)
    : m_hdrop(hdrop)
    , m_index(index)
{}

template<   ss_typename_param_k C
        ,   ss_typename_param_k T
        >
inline basic_drophandle_sequence_const_iterator<C, T>::basic_drophandle_sequence_const_iterator()
    : m_hdrop(0)
    , m_index(sentinel_())
{}

template<   ss_typename_param_k C
        ,   ss_typename_param_k T
        >
inline basic_drophandle_sequence_const_iterator<C, T>::basic_drophandle_sequence_const_iterator(ss_typename_type_k basic_drophandle_sequence_const_iterator<C, T>::class_type const &rhs)
    : m_hdrop(rhs.m_hdrop)
    , m_index(rhs.m_index)
{}

template<   ss_typename_param_k C
        ,   ss_typename_param_k T
        >
inline ss_typename_type_k basic_drophandle_sequence_const_iterator<C, T>::class_type &basic_drophandle_sequence_const_iterator<C, T>::operator =(ss_typename_type_k basic_drophandle_sequence_const_iterator<C, T>::class_type const &rhs)
{
    m_hdrop = rhs.m_hdrop;
    m_index = rhs.m_index;

    return *this;
}

template<   ss_typename_param_k C
        ,   ss_typename_param_k T
        >
inline ss_typename_type_k basic_drophandle_sequence_const_iterator<C, T>::class_type &basic_drophandle_sequence_const_iterator<C, T>::operator ++()
{
    stlsoft_message_assert("Incrementing the end iterator", m_index != sentinel_());

    char_type   ch;
    ws_uint_t   res =   traits_type::drag_query_file(m_hdrop, static_cast<ws_uint_t>(m_index + 1), &ch, 1);

    if(res == 0)
    {
        // Failed, so become 'end'
        m_index = sentinel_();
    }
    else
    {
        // Fine, so increment to next
        ++m_index;
    }

    return *this;
}

template<   ss_typename_param_k C
        ,   ss_typename_param_k T
        >
inline ss_typename_type_k basic_drophandle_sequence_const_iterator<C, T>::class_type &basic_drophandle_sequence_const_iterator<C, T>::operator --()
{
    // If currently at 'end' ....
    if(m_index == sentinel_())
    {
        // ... then determine the new end ...
        m_index = static_cast<index_type>(traits_type::drag_query_file(m_hdrop, 0xFFFFFFFF, NULL, 0));

        if(m_index == 0xFFFFFFFF)
        {
            // ... failed. Must set to end again!
            m_index = sentinel_();
        }
        else
        {
            // ... decrement from end
            --m_index;
        }
    }
    else
    {
        // ... decrement from current position
        --m_index;
    }

    return *this;
}

template<   ss_typename_param_k C
        ,   ss_typename_param_k T
        >
inline ss_typename_type_k basic_drophandle_sequence_const_iterator<C, T>::class_type basic_drophandle_sequence_const_iterator<C, T>::operator ++(int)
{
    class_type  ret(*this);

    operator ++();

    return ret;
}

template<   ss_typename_param_k C
        ,   ss_typename_param_k T
        >
inline ss_typename_type_k basic_drophandle_sequence_const_iterator<C, T>::class_type basic_drophandle_sequence_const_iterator<C, T>::operator --(int)
{
    class_type  ret(*this);

    operator --();

    return ret;
}

template<   ss_typename_param_k C
        ,   ss_typename_param_k T
        >
inline const ss_typename_type_k basic_drophandle_sequence_const_iterator<C, T>::value_type basic_drophandle_sequence_const_iterator<C, T>::operator *() const
{
    stlsoft_message_assert("Dereferencing the end iterator", m_index != sentinel_());

    basic_file_path_buffer<char_type>   sz;
    ws_uint_t                           res =   traits_type::drag_query_file(m_hdrop, static_cast<ws_uint_t>(m_index), &sz[0], sz.size());

    if(res == 0xFFFFFFFF)
    {
        sz[0] = '\0';
    }

    return value_type(stlsoft_ns_qual(c_str_ptr)(sz));
}

template<   ss_typename_param_k C
        ,   ss_typename_param_k T
        >
inline ws_bool_t basic_drophandle_sequence_const_iterator<C, T>::operator ==(ss_typename_type_k basic_drophandle_sequence_const_iterator<C, T>::class_type const &rhs) const
{
    stlsoft_message_assert("Comparing iterators from different sequences", m_hdrop == NULL || rhs.m_hdrop == NULL || rhs.m_hdrop);

    return m_index == rhs.m_index;
}

template<   ss_typename_param_k C
        ,   ss_typename_param_k T
        >
inline ws_bool_t basic_drophandle_sequence_const_iterator<C, T>::operator !=(ss_typename_type_k basic_drophandle_sequence_const_iterator<C, T>::class_type const &rhs) const
{
    return !operator ==(rhs);
}

// basic_drophandle_sequence

template <ss_typename_param_k C, ss_typename_param_k T>
inline basic_drophandle_sequence<C, T>::basic_drophandle_sequence(HDROP hdrop)
    : m_hdrop(hdrop)
{}

template <ss_typename_param_k C, ss_typename_param_k T>
inline basic_drophandle_sequence<C, T>::~basic_drophandle_sequence() winstl_throw_0()
{
    ::DragFinish(m_hdrop);
}

template <ss_typename_param_k C, ss_typename_param_k T>
inline ws_size_t basic_drophandle_sequence<C, T>::size() const
{
    return traits_type::drag_query_file(m_hdrop, static_cast<ws_uint_t>(-1), 0, 0);
}

template <ss_typename_param_k C, ss_typename_param_k T>
inline ws_bool_t basic_drophandle_sequence<C, T>::empty() const
{
    return size() == 0;
}

template <ss_typename_param_k C, ss_typename_param_k T>
inline ss_typename_type_k basic_drophandle_sequence<C, T>::const_iterator basic_drophandle_sequence<C, T>::begin() const
{
    char_type   ch;
    ws_uint_t   res =   traits_type::drag_query_file(m_hdrop, 0, &ch, 1);

    return const_iterator(m_hdrop, (res == 0) ? const_iterator::sentinel_() : 0);
}

template <ss_typename_param_k C, ss_typename_param_k T>
inline ss_typename_type_k basic_drophandle_sequence<C, T>::const_iterator basic_drophandle_sequence<C, T>::end() const
{
    return const_iterator(m_hdrop, const_iterator::sentinel_());
}

#if defined(__STLSOFT_CF_BIDIRECTIONAL_ITERATOR_SUPPORT)
template <ss_typename_param_k C, ss_typename_param_k T>
inline ss_typename_type_k basic_drophandle_sequence<C, T>::const_reverse_iterator basic_drophandle_sequence<C, T>::rbegin() const
{
    return const_reverse_iterator(end());
}

template <ss_typename_param_k C, ss_typename_param_k T>
inline ss_typename_type_k basic_drophandle_sequence<C, T>::const_reverse_iterator basic_drophandle_sequence<C, T>::rend() const
{
    return const_reverse_iterator(begin());
}
#endif /* __STLSOFT_CF_BIDIRECTIONAL_ITERATOR_SUPPORT */


////////////////////////////////////////////////////////////////////////////////
// Unit-testing

#ifdef STLSOFT_UNITTEST

namespace unittest
{
    ss_bool_t test_winstl_drophandle_sequence(unittest_reporter *r)
    {
        using stlsoft::unittest::unittest_initialiser;

        ss_bool_t               bSuccess    =   true;

        unittest_initialiser    init(r, "WinSTL", "drophandle_sequence", __FILE__);

#if 0
        if(<<TODO>>)
        {
            r->report("<<TODO>> failed ", __LINE__);
            bSuccess = false;
        }
#endif /* 0 */

        return bSuccess;
    }

    unittest_registrar    unittest_winstl_drophandle_sequence(test_winstl_drophandle_sequence);

} // namespace unittest

#endif /* STLSOFT_UNITTEST */

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

#endif /* WINSTL_INCL_H_WINSTL_DROPHANDLE_SEQUENCE */

/* ////////////////////////////////////////////////////////////////////////// */
