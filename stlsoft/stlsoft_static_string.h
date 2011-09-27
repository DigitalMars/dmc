/* /////////////////////////////////////////////////////////////////////////////
 * File:        stlsoft_static_string.h (formerly MStkStr.h, ::SynesisDev)
 *
 * Purpose:     static_string class.
 *
 * Created:     11th June 1994
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


/// \file stlsoft_static_string.h
///
/// static_string class.

#ifndef STLSOFT_INCL_H_STLSOFT_STATIC_STRING
#define STLSOFT_INCL_H_STLSOFT_STATIC_STRING

#ifndef __STLSOFT_DOCUMENTATION_SKIP_SECTION
# define STLSOFT_VER_H_STLSOFT_STATIC_STRING_MAJOR      2
# define STLSOFT_VER_H_STLSOFT_STATIC_STRING_MINOR      0
# define STLSOFT_VER_H_STLSOFT_STATIC_STRING_REVISION   1
# define STLSOFT_VER_H_STLSOFT_STATIC_STRING_EDIT       127
#endif /* !__STLSOFT_DOCUMENTATION_SKIP_SECTION */

/* /////////////////////////////////////////////////////////////////////////////
 * Compatibility
 */

/*
[Incompatibilies-start]
__STLSOFT_COMPILER_IS_DMC: __DMC__<0x0839
__STLSOFT_COMPILER_IS_MSVC: _MSC_VER<1200
[Incompatibilies-end]
 */

/* /////////////////////////////////////////////////////////////////////////////
 * Includes
 */

#ifndef STLSOFT_INCL_H_STLSOFT
# include "stlsoft.h"               // Include the STLSoft root header
#endif /* !STLSOFT_INCL_H_STLSOFT */

#if defined(__STLSOFT_COMPILER_IS_DMC) && \
    __DMC__ < 0x0839
# error stlsoft_static_string.h is not compatible with Digital Mars C/C++ 3.38 or earlier
#endif /* _MSC_VER < 1200 */
#if defined(__STLSOFT_COMPILER_IS_MSVC) && \
    _MSC_VER < 1200
# error stlsoft_static_string.h is not compatible with Visual C++ 5.0 or earlier
#endif /* _MSC_VER < 1200 */

#ifndef STLSOFT_INCL_H_STLSOFT_CHAR_TRAITS
# include "stlsoft_char_traits.h"  // stlsoft::char_traits
#endif /* !STLSOFT_INCL_H_STLSOFT_CHAR_TRAITS */
#ifndef STLSOFT_INCL_H_STLSOFT_ITERATOR
# include "stlsoft_iterator.h"       // stlsoft iterator bases
#endif /* !STLSOFT_INCL_H_STLSOFT_ITERATOR */
#ifndef STLSOFT_INCL_H_STLSOFT_AUTO_BUFFER
# include "stlsoft_auto_buffer.h"       // stlsoft::auto_buffer
#endif /* !STLSOFT_INCL_H_STLSOFT_AUTO_BUFFER */
#ifndef STLSOFT_INCL_H_STLSOFT_NEW_ALLOCATOR
# include "stlsoft_new_allocator.h"     // stlsoft::new_allocator
#endif /* !STLSOFT_INCL_H_STLSOFT_NEW_ALLOCATOR */

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

/// \weakgroup libraries_string String Library
/// \ingroup libraries
/// \brief This library provides facilities for defining and manipulating strings

/// \weakgroup stlsoft_string_library String Library (STLSoft)
/// \ingroup STLSoft libraries_string
/// \brief This library provides facilities for defining and manipulating strings
/// @{

/* /////////////////////////////////////////////////////////////////////////////
 * Classes
 */

// class static_string
/// Simple string class using fixed-size static-based storage
///
/// \param C The character type
/// \param CCH The number of characters in the fixed-side buffer, not including the null-terminator
/// \param T The traits type. On translators that support default template arguments this is defaulted to char_traits<C>
template<   ss_typename_param_k C
#if !defined(__STLSOFT_COMPILER_IS_BORLAND)
        ,   ss_size_t           CCH
#else /* ? compiler */
        ,   ss_size_t           cch
#endif /* compiler */
#ifdef __STLSOFT_CF_TEMPLATE_CLASS_DEFAULT_CLASS_ARGUMENT_SUPPORT
        ,   ss_typename_param_k T = char_traits<C>
#else
        ,   ss_typename_param_k T = /* char_traits<C> */
#endif /* __STLSOFT_CF_TEMPLATE_CLASS_DEFAULT_CLASS_ARGUMENT_SUPPORT */
        >
class basic_static_string
{
#if !defined(__STLSOFT_COMPILER_IS_BORLAND)
    enum { cch = CCH };
#endif /* compiler */

public:
    /// The value type
    typedef C                               value_type;
    /// The traits type
    typedef T                               traits_type;
    /// The current parameterisation of the type
    typedef basic_static_string<C, cch, T>   class_type;
    /// The character type
    typedef value_type                      char_type;
    /// The pointer type
    typedef value_type                      *pointer;
    /// The non-mutable (const) pointer type
    typedef value_type const                *const_pointer;
    /// The reference type
    typedef value_type                      &reference;
    /// The non-mutable (const) reference type
    typedef value_type const                &const_reference;
    /// The size type
    typedef ss_size_t                       size_type;
    /// The difference type
    typedef ss_ptrdiff_t                    difference_type;

    /// The iterator type
    typedef
#if !defined(__STLSOFT_COMPILER_IS_BORLAND)
         ss_typename_type_k
#endif /* __STLSOFT_COMPILER_IS_BORLAND */
                       pointer_iterator <   value_type
                                        ,   pointer
                                        ,   reference
                                        >::iterator_type    iterator;
    /// The non-mutating (const) iterator type
    typedef
#if !defined(__STLSOFT_COMPILER_IS_BORLAND)
         ss_typename_type_k
#endif /* __STLSOFT_COMPILER_IS_BORLAND */
                       pointer_iterator <   value_type const
                                        ,   const_pointer
                                        ,   const_reference
                                        >::iterator_type    const_iterator;

#if defined(__STLSOFT_CF_BIDIRECTIONAL_ITERATOR_SUPPORT)
    /// The reverse iterator type
    typedef reverse_iterator_base       <   iterator
                                        ,   value_type
                                        ,   reference
                                        ,   pointer
                                        ,   difference_type
                                        >                   reverse_iterator;

    /// The non-mutating (const) reverse iterator type
    typedef const_reverse_iterator_base <   const_iterator
                                        ,   value_type const
                                        ,   const_reference
                                        ,   const_pointer
                                        ,   difference_type
                                        >                   const_reverse_iterator;
#endif /* __STLSOFT_CF_BIDIRECTIONAL_ITERATOR_SUPPORT */

/// \name Construction
//@{
public:
    /// Default constructor
    basic_static_string();
    /// Copy constructor
    basic_static_string(class_type const &rhs);
    /// Construct from the given string at the specified position
    basic_static_string(class_type const &s, size_type pos);
    /// Construct with \c n characters from the given string at the specified position
    basic_static_string(class_type const &s, size_type pos, size_type n);
    /// Construct from the given character string
    basic_static_string(char_type const *s);
    /// Construct with \c n characters from the given character string
    basic_static_string(char_type const *s, size_type n);
    /// Construct with \c n characters each set to \c ch
    basic_static_string(size_type n, char_type ch);
    /// Construct from the range [first:last)
#if !defined(__STLSOFT_CF_MEMBER_TEMPLATE_RANGE_METHOD_SUPPORT)
    basic_static_string(char_type const *f, char_type const *t);
#else
    template <ss_typename_param_k II>
    basic_static_string(II first, II last)
        : m_length(stlsoft_ns_qual_std(distance)(first, last))
    {
        stlsoft_message_assert("incident string too large for static_string construction", !(cch < m_length));

        typedef auto_buffer<char_type, new_allocator<char_type> >  buffer_t;

        buffer_t    buffer(m_length);

        stlsoft_ns_qual_std(copy)(first, last, buffer.begin());

        traits_type::copy(m_buffer, buffer, buffer.size());

        m_buffer[m_length] = 0;
        m_buffer[size()] = 0;
    }
#endif /* __STLSOFT_CF_MEMBER_TEMPLATE_RANGE_METHOD_SUPPORT */
    /// Destructor
    ~basic_static_string();
//@}

/// \name Assignment
//@{
public:
    /// Assigns from the given character string
    class_type &assign(const char_type *s);
    /// Assigns with \c n characters from the given character string
    class_type &assign(const char_type *s, size_type n);
    /// Assigns with \c n characters from the given character string at the specified position
    class_type &assign(class_type const &str, size_type pos, size_type n);
    /// Assigns from the given string
    class_type &assign(class_type const &str);
    /// Assigns \c n characters with the value \c ch
    class_type &assign(size_type n, char_type c);
    /// Assigns from the range [first:last)
#if !defined(__STLSOFT_CF_MEMBER_TEMPLATE_RANGE_METHOD_SUPPORT)
    class_type &assign(const_iterator first, const_iterator last);
#else
    template <ss_typename_param_k II>
    class_type &assign(II first, II last)
    {
# if defined(__STLSOFT_COMPILER_IS_GCC) && \
     __GNUC__ < 3
        typedef ss_typename_type_k std::iterator_traits<II> traits_t;

        return assign(first, last, traits_t::iterator_category());
# else
        return assign(first, last, stlsoft_iterator_query_category(II, first));
# endif /* GCC < 3 */
    }
#endif /* __STLSOFT_CF_MEMBER_TEMPLATE_RANGE_METHOD_SUPPORT */

    /// Copy assignment operator
    class_type const &operator =(class_type const &rhs);
    /// Assignment operator
    class_type const &operator =(char_type const *s);
    /// Assignment operator
    class_type const &operator =(char_type c);
//@}

/// \name Appending
//@{
public:
    /// Appends the given character string
    class_type &append(char_type const *s);
    /// Appends \c cch characters from the given character string
    class_type &append(char_type const *s, size_type cch);
    /// Assigns \c cch characters from the given character string at the specified position
    class_type &append(class_type const &str, size_type pos, size_type cch);
    /// Appends the given string
    class_type &append(class_type const &str);
    /// Appends \c cch characters with the value \c ch
    class_type &append(size_type cch, char_type ch);
    /// Appends the range [first:last)
#if !defined(__STLSOFT_CF_MEMBER_TEMPLATE_RANGE_METHOD_SUPPORT)
    class_type &append(const_iterator first, const_iterator last);
#else
    template <ss_typename_param_k II>
    class_type &append(II first, II last)
    {
# if defined(__STLSOFT_COMPILER_IS_GCC) && \
     __GNUC__ < 3
        typedef ss_typename_type_k std::iterator_traits<II> traits_t;

        return append_(first, last, traits_t::iterator_category());
# else
        return append_(first, last, stlsoft_iterator_query_category(II, first));
# endif /* GCC < 3 */
    }
#endif /* __STLSOFT_CF_MEMBER_TEMPLATE_RANGE_METHOD_SUPPORT */

    /// Concatenation operator
    class_type &operator +=(char_type ch);
    /// Concatenation operator
    class_type &operator +=(char_type const *s);
    /// Concatenation operator
    class_type &operator +=(class_type const &rhs);

    /// Appends a single character
    void push_back(char_type ch);
//@}

/// \name Operations
//@{
public:
    /// Reserves at least n characters
    void reserve(size_type n);
    /// Swaps the contents between \c this and \c other
    void swap(class_type &other);

    /// Resizes the string
    ///
    /// \param n The new size of the string
    /// \param ch The value with which to initialise additional items if the string is expanded
    void resize(size_type n, value_type ch = value_type());

    /// Empties the string
    void clear();
//@}

/// \name Attributes
//@{
public:
    /// The number of elements in the string
    static size_type size();
    /// The maximum number of elements that can be stored in the string
    static size_type max_size();
    /// The number of elements in the string
    size_type length() const;
    /// The storage currently allocated by the string
    size_type capacity() const;
    /// Indicates whether the string is empty
    ss_bool_t empty() const;
//@}

/// \name Comparison
//@{
public:
    /// Compares \c this with the given string
    ss_sint_t compare(size_type pos, size_type n, value_type const *s, size_type cchRhs) const;
    /// Compares \c this with the given string
    ss_sint_t compare(size_type pos, size_type n, value_type const *s) const;
    /// Compares \c this with the given string
    ss_sint_t compare(value_type const *s) const;
    /// Compares \c this with the given string
    ss_sint_t compare(size_type pos, size_type n, class_type const &rhs, size_type posRhs, size_type cchRhs) const;
    /// Compares \c this with the given string
    ss_sint_t compare(size_type pos, size_type n, class_type const &rhs) const;
    /// Compares \c this with the given string
    ss_sint_t compare(class_type const &rhs) const;
//@}

/// \name Accessors
//@{
public:
    /// Returns mutable reference at the given index
    reference       operator [](size_type index);
    /// Returns non-mutable (const) reference at the given index
    const_reference operator [](size_type index) const;

    /// Returns null-terminated non-mutable (const) pointer to string data
    value_type const    *c_str() const;
    /// Returns non-mutable (const) pointer to string data
    value_type const    *data() const;

    /// Copies elements into the given destination
    size_type copy(value_type *dest, size_type cch_, size_type pos = 0) const;
//@}

/// \name Iteration
//@{
public:
    /// Begins the iteration
    ///
    /// \return A non-mutable (const) iterator representing the start of the sequence
    const_iterator          begin() const;
    /// Ends the iteration
    ///
    /// \return A non-mutable (const) iterator representing the end of the sequence
    const_iterator          end() const;
    /// Begins the iteration
    ///
    /// \return An iterator representing the start of the sequence
    iterator                begin();
    /// Ends the iteration
    ///
    /// \return An iterator representing the end of the sequence
    iterator                end();

#if defined(__STLSOFT_CF_BIDIRECTIONAL_ITERATOR_SUPPORT)
    /// Begins the reverse iteration
    ///
    /// \return A non-mutable (const) iterator representing the start of the reverse sequence
    const_reverse_iterator  rbegin() const;
    /// Ends the reverse iteration
    ///
    /// \return A non-mutable (const) iterator representing the end of the reverse sequence
    const_reverse_iterator  rend() const;
    /// Begins the reverse iteration
    ///
    /// \return An iterator representing the start of the reverse sequence
    reverse_iterator        rbegin();
    /// Ends the reverse iteration
    ///
    /// \return An iterator representing the end of the reverse sequence
    reverse_iterator        rend();
#endif /* __STLSOFT_CF_BIDIRECTIONAL_ITERATOR_SUPPORT */
//@}

/// \name Implementation
//@{
private:
    // Length
    size_type length_() const;

    // Comparison
    static ss_sint_t compare_(char_type const *lhs, size_type lhs_len, char_type const *rhs, size_type rhs_len);

    // Assignment
#if defined(__STLSOFT_CF_MEMBER_TEMPLATE_RANGE_METHOD_SUPPORT)
    template <ss_typename_param_k II>
    class_type &assign(II first, II last, stlsoft_ns_qual_std(input_iterator_tag))
    {
        stlsoft_ns_qual_std(copy)(first, last, stlsoft_ns_qual_std(back_inserter)(*this));

        return *this;
    }
    template <ss_typename_param_k II>
    class_type &assign(II first, II last, stlsoft_ns_qual_std(forward_iterator_tag))
    {
        typedef auto_buffer<char_type, new_allocator<char_type> >  buffer_t;

        buffer_t    buffer(stlsoft_ns_qual_std(distance)(first, last));

        stlsoft_ns_qual_std(copy)(first, last, buffer.begin());
        assign(buffer, buffer.size());

        return *this;
    }
#endif /* __STLSOFT_CF_MEMBER_TEMPLATE_RANGE_METHOD_SUPPORT */

//@}

/// \name Members
//@{
private:
    value_type  m_buffer[cch + 1];
    ss_size_t   m_length;
//@}
};

/* /////////////////////////////////////////////////////////////////////////////
 * Operators
 */

#ifndef __STLSOFT_DOCUMENTATION_SKIP_SECTION

// operator ==

template<   ss_typename_param_k C
        ,   ss_size_t           CCH
        ,   ss_typename_param_k T
        >
inline ss_bool_t operator ==(basic_static_string<C, CCH, T> const &lhs, basic_static_string<C, CCH, T> const &rhs)
{
    return lhs.compare(rhs) == 0;
}

template<   ss_typename_param_k C
        ,   ss_size_t           CCH
        ,   ss_typename_param_k T
        >
#ifdef __STLSOFT_CF_TEMPLATE_OUTOFCLASSFN_QUALIFIED_TYPE_SUPPORT
inline ss_bool_t operator ==(basic_static_string<C, CCH, T> const &lhs, ss_typename_type_k basic_static_string<C, CCH, T>::char_type const *rhs)
#else
inline ss_bool_t operator ==(basic_static_string<C, CCH, T> const &lhs, C const *rhs)
#endif /* __STLSOFT_CF_TEMPLATE_OUTOFCLASSFN_QUALIFIED_TYPE_SUPPORT */
{
    return lhs.compare(rhs) == 0;
}

template<   ss_typename_param_k C
        ,   ss_size_t           CCH
        ,   ss_typename_param_k T
        >
#ifdef __STLSOFT_CF_TEMPLATE_OUTOFCLASSFN_QUALIFIED_TYPE_SUPPORT
inline ss_bool_t operator ==(ss_typename_type_k basic_static_string<C, CCH, T>::char_type const *lhs, basic_static_string<C, CCH, T> const &rhs)
#else
inline ss_bool_t operator ==(C *lhs, basic_static_string<C, CCH, T> const &rhs)
#endif /* __STLSOFT_CF_TEMPLATE_OUTOFCLASSFN_QUALIFIED_TYPE_SUPPORT */
{
    return rhs.compare(lhs) == 0;
}

// operator !=

template<   ss_typename_param_k C
        ,   ss_size_t           CCH
        ,   ss_typename_param_k T
        >
inline ss_bool_t operator !=(basic_static_string<C, CCH, T> const &lhs, basic_static_string<C, CCH, T> const &rhs)
{
    return lhs.compare(rhs) != 0;
}
template<   ss_typename_param_k C
        ,   ss_size_t           CCH
        ,   ss_typename_param_k T
        >
#ifdef __STLSOFT_CF_TEMPLATE_OUTOFCLASSFN_QUALIFIED_TYPE_SUPPORT
inline ss_bool_t operator !=(basic_static_string<C, CCH, T> const &lhs, ss_typename_type_k basic_static_string<C, CCH, T>::char_type const *rhs)
#else
inline ss_bool_t operator !=(basic_static_string<C, CCH, T> const &lhs, C const *rhs)
#endif /* __STLSOFT_CF_TEMPLATE_OUTOFCLASSFN_QUALIFIED_TYPE_SUPPORT */
{
    return lhs.compare(rhs) != 0;
}
template<   ss_typename_param_k C
        ,   ss_size_t           CCH
        ,   ss_typename_param_k T
        >
#ifdef __STLSOFT_CF_TEMPLATE_OUTOFCLASSFN_QUALIFIED_TYPE_SUPPORT
inline ss_bool_t operator !=(ss_typename_type_k basic_static_string<C, CCH, T>::char_type const *lhs, basic_static_string<C, CCH, T> const &rhs)
#else
inline ss_bool_t operator !=(C const *lhs, basic_static_string<C, CCH, T> const &rhs)
#endif /* __STLSOFT_CF_TEMPLATE_OUTOFCLASSFN_QUALIFIED_TYPE_SUPPORT */
{
    return rhs.compare(lhs) != 0;
}

// operator <

template<   ss_typename_param_k C
        ,   ss_size_t           CCH
        ,   ss_typename_param_k T
        >
inline ss_bool_t operator <(basic_static_string<C, CCH, T> const &lhs, basic_static_string<C, CCH, T> const &rhs)
{
    return lhs.compare(rhs) < 0;
}

template<   ss_typename_param_k C
        ,   ss_size_t           CCH
        ,   ss_typename_param_k T
        >
#ifdef __STLSOFT_CF_TEMPLATE_OUTOFCLASSFN_QUALIFIED_TYPE_SUPPORT
inline ss_bool_t operator <(basic_static_string<C, CCH, T> const &lhs, ss_typename_type_k basic_static_string<C, CCH, T>::char_type const *rhs)
#else
inline ss_bool_t operator <(basic_static_string<C, CCH, T> const &lhs, C const *rhs)
#endif /* __STLSOFT_CF_TEMPLATE_OUTOFCLASSFN_QUALIFIED_TYPE_SUPPORT */
{
    return lhs.compare(rhs) < 0;
}

template<   ss_typename_param_k C
        ,   ss_size_t           CCH
        ,   ss_typename_param_k T
        >
#ifdef __STLSOFT_CF_TEMPLATE_OUTOFCLASSFN_QUALIFIED_TYPE_SUPPORT
inline ss_bool_t operator <(ss_typename_type_k basic_static_string<C, CCH, T>::char_type const *lhs, basic_static_string<C, CCH, T> const &rhs)
#else
inline ss_bool_t operator <(C const *lhs, basic_static_string<C, CCH, T> const &rhs)
#endif /* __STLSOFT_CF_TEMPLATE_OUTOFCLASSFN_QUALIFIED_TYPE_SUPPORT */
{
    return rhs.compare(lhs) > 0;
}

// operator <=

template<   ss_typename_param_k C
        ,   ss_size_t           CCH
        ,   ss_typename_param_k T
        >
inline ss_bool_t operator <=(basic_static_string<C, CCH, T> const &lhs, basic_static_string<C, CCH, T> const &rhs)
{
    return lhs.compare(rhs) <= 0;
}
template<   ss_typename_param_k C
        ,   ss_size_t           CCH
        ,   ss_typename_param_k T
        >
#ifdef __STLSOFT_CF_TEMPLATE_OUTOFCLASSFN_QUALIFIED_TYPE_SUPPORT
inline ss_bool_t operator <=(basic_static_string<C, CCH, T> const &lhs, ss_typename_type_k basic_static_string<C, CCH, T>::char_type const *rhs)
#else
inline ss_bool_t operator <=(basic_static_string<C, CCH, T> const &lhs, C const *rhs)
#endif /* __STLSOFT_CF_TEMPLATE_OUTOFCLASSFN_QUALIFIED_TYPE_SUPPORT */
{
    return lhs.compare(rhs) <= 0;
}
template<   ss_typename_param_k C
        ,   ss_size_t           CCH
        ,   ss_typename_param_k T
        >
#ifdef __STLSOFT_CF_TEMPLATE_OUTOFCLASSFN_QUALIFIED_TYPE_SUPPORT
inline ss_bool_t operator <=(ss_typename_type_k basic_static_string<C, CCH, T>::char_type const *lhs, basic_static_string<C, CCH, T> const &rhs)
#else
inline ss_bool_t operator <=(C const *lhs, basic_static_string<C, CCH, T> const &rhs)
#endif /* __STLSOFT_CF_TEMPLATE_OUTOFCLASSFN_QUALIFIED_TYPE_SUPPORT */
{
    return rhs.compare(lhs) >= 0;
}

// operator >

template<   ss_typename_param_k C
        ,   ss_size_t           CCH
        ,   ss_typename_param_k T
        >
inline ss_bool_t operator >(basic_static_string<C, CCH, T> const &lhs, basic_static_string<C, CCH, T> const &rhs)
{
    return lhs.compare(rhs) > 0;
}
template<   ss_typename_param_k C
        ,   ss_size_t           CCH
        ,   ss_typename_param_k T
        >
#ifdef __STLSOFT_CF_TEMPLATE_OUTOFCLASSFN_QUALIFIED_TYPE_SUPPORT
inline ss_bool_t operator >(basic_static_string<C, CCH, T> const &lhs, ss_typename_type_k basic_static_string<C, CCH, T>::char_type const *rhs)
#else
inline ss_bool_t operator >(basic_static_string<C, CCH, T> const &lhs, C const *rhs)
#endif /* __STLSOFT_CF_TEMPLATE_OUTOFCLASSFN_QUALIFIED_TYPE_SUPPORT */
{
    return lhs.compare(rhs) > 0;
}
template<   ss_typename_param_k C
        ,   ss_size_t           CCH
        ,   ss_typename_param_k T
        >
#ifdef __STLSOFT_CF_TEMPLATE_OUTOFCLASSFN_QUALIFIED_TYPE_SUPPORT
inline ss_bool_t operator >(ss_typename_type_k basic_static_string<C, CCH, T>::char_type const *lhs, basic_static_string<C, CCH, T> const &rhs)
#else
inline ss_bool_t operator >(C const *lhs, basic_static_string<C, CCH, T> const &rhs)
#endif /* __STLSOFT_CF_TEMPLATE_OUTOFCLASSFN_QUALIFIED_TYPE_SUPPORT */
{
    return rhs.compare(lhs) < 0;
}

// operator >=

template<   ss_typename_param_k C
        ,   ss_size_t           CCH
        ,   ss_typename_param_k T
        >
inline ss_bool_t operator >=(basic_static_string<C, CCH, T> const &lhs, basic_static_string<C, CCH, T> const &rhs)
{
    return lhs.compare(rhs) >= 0;
}
template<   ss_typename_param_k C
        ,   ss_size_t           CCH
        ,   ss_typename_param_k T
        >
#ifdef __STLSOFT_CF_TEMPLATE_OUTOFCLASSFN_QUALIFIED_TYPE_SUPPORT
inline ss_bool_t operator >=(basic_static_string<C, CCH, T> const &lhs, ss_typename_type_k basic_static_string<C, CCH, T>::char_type const *rhs)
#else
inline ss_bool_t operator >=(basic_static_string<C, CCH, T> const &lhs, C const *rhs)
#endif /* __STLSOFT_CF_TEMPLATE_OUTOFCLASSFN_QUALIFIED_TYPE_SUPPORT */
{
    return lhs.compare(rhs) >= 0;
}
template<   ss_typename_param_k C
        ,   ss_size_t           CCH
        ,   ss_typename_param_k T
        >
#ifdef __STLSOFT_CF_TEMPLATE_OUTOFCLASSFN_QUALIFIED_TYPE_SUPPORT
inline ss_bool_t operator >=(ss_typename_type_k basic_static_string<C, CCH, T>::char_type const *lhs, basic_static_string<C, CCH, T> const &rhs)
#else
inline ss_bool_t operator >=(C const *lhs, basic_static_string<C, CCH, T> const &rhs)
#endif /* __STLSOFT_CF_TEMPLATE_OUTOFCLASSFN_QUALIFIED_TYPE_SUPPORT */
{
    return rhs.compare(lhs) <= 0;
}

#endif /* !__STLSOFT_DOCUMENTATION_SKIP_SECTION */

/* /////////////////////////////////////////////////////////////////////////////
 * Shims
 */

#ifndef __STLSOFT_CF_TEMPLATE_SHIMS_NOT_SUPPORTED

/* c_str_ptr_null */

/// \brief Returns the corresponding C-string pointer of \c s, or a null pointer
template<   ss_typename_param_k C
        ,   ss_size_t           CCH
        ,   ss_typename_param_k T
        >
inline C const *c_str_ptr_null(basic_static_string<C, CCH, T> const &s)
{
    return (s.length() == 0) ? 0 : s.c_str();
}

/* c_str_ptr */

/// \brief Returns the corresponding C-string pointer of \c s
template<   ss_typename_param_k C
        ,   ss_size_t           CCH
        ,   ss_typename_param_k T
        >
inline C const *c_str_ptr(basic_static_string<C, CCH, T> const &s)
{
    return s.c_str();
}

/* c_str_ptr_len */

/// \brief Returns the length (in characters) of \c s, <b><i>not</i></b> including the null-terminating character
template<   ss_typename_param_k C
        ,   ss_size_t           CCH
        ,   ss_typename_param_k T
        >
inline ss_size_t c_str_len(basic_static_string<C, CCH, T> const &s)
{
    return s.length();
}

/* c_str_ptr_size */

/// \brief Returns the size (in bytes) of the contents of \c s, <b><i>not</i></b> including the null-terminating character
template<   ss_typename_param_k C
        ,   ss_size_t           CCH
        ,   ss_typename_param_k T
        >
inline ss_size_t c_str_size(basic_static_string<C, CCH, T> const &s)
{
    return c_str_len(s) * sizeof(C);
}

template<   ss_typename_param_k S
        ,   ss_typename_param_k C
        ,   ss_size_t           CCH
        ,   ss_typename_param_k T
        >
inline S &operator <<(S & s, basic_static_string<C, CCH, T> const &str)
{
    s << str.c_str();

    return s;
}

#endif /* !__STLSOFT_CF_TEMPLATE_SHIMS_NOT_SUPPORTED */

/* /////////////////////////////////////////////////////////////////////////////
 * Unit-testing
 */

#ifdef STLSOFT_UNITTEST

namespace unittest
{
    ss_bool_t test_stlsoft_static_string(unittest_reporter *r)
    {
        ss_bool_t               bSuccess    =   true;

        unittest_initialiser    init(r, "STLSoft", "static_string", __FILE__);

        typedef basic_static_string<char, 32>    sts_32_t;

        sts_32_t    s1("Hello, Pussycat!");
        sts_32_t    s2("Hello, Pussycat!");

#if !defined(__STLSOFT_COMPILER_IS_DMC) // TODO: Need to sort out DMC++
        if(s1 != s2)
        {
            r->report("Equivalent strings comparison failed ", __LINE__);
            bSuccess = false;
        }

        sts_32_t    s3(s1);

        if(s1 != s3)
        {
            r->report("Copy construction failed ", __LINE__);
            bSuccess = false;
        }

        sts_32_t    s4;

        s4 = s1;

        if(s1 != s4)
        {
            r->report("Copy assignment failed ", __LINE__);
            bSuccess = false;
        }
#endif /* compiler */

        return bSuccess;
    }

    unittest_registrar    unittest_stlsoft_static_string(test_stlsoft_static_string);

} // namespace unittest

#endif /* STLSOFT_UNITTEST */

/* /////////////////////////////////////////////////////////////////////////////
 * Implementation
 */

#ifndef __STLSOFT_DOCUMENTATION_SKIP_SECTION

template<   ss_typename_param_k C
        ,   ss_size_t           CCH
        ,   ss_typename_param_k T
        >
inline ss_typename_type_k basic_static_string<C, CCH, T>::size_type basic_static_string<C, CCH, T>::length_() const
{
    if(m_length == static_cast<ss_size_t>(-1))
    {
        ss_size_t   &_m_length  =   const_cast<class_type *>(this)->m_length;

        _m_length = traits_type::length(m_buffer);
    }

    return m_length;
}

template<   ss_typename_param_k C
        ,   ss_size_t           CCH
        ,   ss_typename_param_k T
        >
inline basic_static_string<C, CCH, T>::basic_static_string()
    : m_length(static_cast<ss_size_t>(-1))
{
    m_buffer[0] = 0;
    m_buffer[size()] = 0;
}

template<   ss_typename_param_k C
        ,   ss_size_t           CCH
        ,   ss_typename_param_k T
        >
inline basic_static_string<C, CCH, T>::basic_static_string(class_type const &rhs)
    : m_length(rhs.length())
{
    traits_type::copy(m_buffer, rhs.m_buffer, rhs.length());

    m_buffer[m_length] = 0;
    m_buffer[size()] = 0;
}

template<   ss_typename_param_k C
        ,   ss_size_t           CCH
        ,   ss_typename_param_k T
        >
inline basic_static_string<C, CCH, T>::basic_static_string(class_type const &rhs, size_type pos)
    : m_length(rhs.length() - pos)
{
    traits_type::copy(m_buffer, rhs.m_buffer + pos, m_length);

    m_buffer[m_length] = 0;
    m_buffer[size()] = 0;
}

template<   ss_typename_param_k C
        ,   ss_size_t           CCH
        ,   ss_typename_param_k T
        >
inline basic_static_string<C, CCH, T>::basic_static_string(class_type const &rhs, size_type pos, size_type n)
    : m_length(n)
{
    stlsoft_message_assert("incident string too large for static_string construction", !(cch < n));

    traits_type::copy(m_buffer, rhs.m_buffer + pos, n);

    m_buffer[m_length] = 0;
    m_buffer[size()] = 0;
}


template<   ss_typename_param_k C
        ,   ss_size_t           CCH
        ,   ss_typename_param_k T
        >
inline basic_static_string<C, CCH, T>::basic_static_string(char_type const *s)
    : m_length((s != 0) ? traits_type::length(s) : 0)
{
    traits_type::copy(m_buffer, s, m_length);

    m_buffer[m_length] = 0;
    m_buffer[size()] = 0;
}

template<   ss_typename_param_k C
        ,   ss_size_t           CCH
        ,   ss_typename_param_k T
        >
inline basic_static_string<C, CCH, T>::basic_static_string(char_type const *s, size_type n)
    : m_length(n)
{
    stlsoft_message_assert("incident string too large for static_string construction", !(cch < n));

    traits_type::copy(m_buffer, s, n);

    m_buffer[n] = 0;
    m_buffer[size()] = 0;
}

template<   ss_typename_param_k C
        ,   ss_size_t           CCH
        ,   ss_typename_param_k T
        >
inline basic_static_string<C, CCH, T>::basic_static_string(size_type n, char_type c)
    : m_length(n)
{
    traits_type::assign(m_buffer, n, c);

    m_buffer[m_length] = 0;
    m_buffer[size()] = 0;
}

#if !defined(__STLSOFT_CF_MEMBER_TEMPLATE_RANGE_METHOD_SUPPORT)
template<   ss_typename_param_k C
        ,   ss_size_t           CCH
        ,   ss_typename_param_k T
        >
inline basic_static_string<C, CCH, T>::basic_static_string(char_type const *f, char_type const *t)
    : m_length(t - f)
{
    stlsoft_message_assert("incident string too large for static_string construction", !(cch < m_length));

    traits_type::copy(m_buffer, f, m_length);

    m_buffer[m_length] = 0;
    m_buffer[size()] = 0;
}
#endif /* __STLSOFT_CF_MEMBER_TEMPLATE_RANGE_METHOD_SUPPORT */

template<   ss_typename_param_k C
        ,   ss_size_t           CCH
        ,   ss_typename_param_k T
        >
inline basic_static_string<C, CCH, T>::~basic_static_string()
{}


// Assignment

template<   ss_typename_param_k C
        ,   ss_size_t           CCH
        ,   ss_typename_param_k T
        >
inline ss_typename_type_k basic_static_string<C, CCH, T>::class_type &basic_static_string<C, CCH, T>::assign(const ss_typename_type_k basic_static_string<C, CCH, T>::char_type *s)
{
    m_length = static_cast<ss_size_t>(-1);
    traits_type::copy(m_buffer, s, CCH);
    m_buffer[size()] = 0;

    return *this;
}

template<   ss_typename_param_k C
        ,   ss_size_t           CCH
        ,   ss_typename_param_k T
        >
inline ss_typename_type_k basic_static_string<C, CCH, T>::class_type &basic_static_string<C, CCH, T>::assign(const ss_typename_type_k basic_static_string<C, CCH, T>::char_type *s, ss_typename_type_k basic_static_string<C, CCH, T>::size_type n)
{
    stlsoft_message_assert("incident string too large for static_string assignment", !(cch < n));

    traits_type::copy(m_buffer, s, n);

    m_length = n;
    m_buffer[m_length] = 0;
    m_buffer[size()] = 0;

    return *this;
}

template<   ss_typename_param_k C
        ,   ss_size_t           CCH
        ,   ss_typename_param_k T
        >
inline ss_typename_type_k basic_static_string<C, CCH, T>::class_type &basic_static_string<C, CCH, T>::assign(ss_typename_type_k basic_static_string<C, CCH, T>::class_type const &rhs, ss_typename_type_k basic_static_string<C, CCH, T>::size_type pos, ss_typename_type_k basic_static_string<C, CCH, T>::size_type n)
{
    stlsoft_message_assert("incident string too large for static_string assignment", !(cch < n));

    traits_type::copy(m_buffer, rhs.m_buffer + pos, n);

    m_length = n;
    m_buffer[m_length] = 0;
    m_buffer[size()] = 0;

    return *this;
}

template<   ss_typename_param_k C
        ,   ss_size_t           CCH
        ,   ss_typename_param_k T
        >
inline ss_typename_type_k basic_static_string<C, CCH, T>::class_type &basic_static_string<C, CCH, T>::assign(ss_typename_type_k basic_static_string<C, CCH, T>::class_type const &rhs)
{
    m_length = rhs.length();

    traits_type::copy(m_buffer, rhs.m_buffer, rhs.length());
    m_buffer[m_length] = 0;

    return *this;
}

template<   ss_typename_param_k C
        ,   ss_size_t           CCH
        ,   ss_typename_param_k T
        >
inline ss_typename_type_k basic_static_string<C, CCH, T>::class_type &basic_static_string<C, CCH, T>::assign(ss_typename_type_k basic_static_string<C, CCH, T>::size_type n, ss_typename_type_k basic_static_string<C, CCH, T>::char_type c)
{
    stlsoft_message_assert("incident string too large for static_string assignment", !(cch < n));

    traits_type::assign(m_buffer, n, c);

    m_length = n;
    m_buffer[m_length] = 0;
    m_buffer[size()] = 0;

    return *this;
}

#if !defined(__STLSOFT_CF_MEMBER_TEMPLATE_RANGE_METHOD_SUPPORT)
template<   ss_typename_param_k C
        ,   ss_size_t           CCH
        ,   ss_typename_param_k T
        >
inline ss_typename_type_k basic_static_string<C, CCH, T>::class_type &basic_static_string<C, CCH, T>::assign(ss_typename_type_k basic_static_string<C, CCH, T>::const_iterator first, ss_typename_type_k basic_static_string<C, CCH, T>::const_iterator last)
{
    // We have to use this strange appearing this, because of Visual C++ .NET's
    // disgusting STL swill. Sigh!
    return assign(&(*first), last - first);
}
#endif /* __STLSOFT_CF_MEMBER_TEMPLATE_RANGE_METHOD_SUPPORT */

template<   ss_typename_param_k C
        ,   ss_size_t           CCH
        ,   ss_typename_param_k T
        >
inline ss_typename_type_k basic_static_string<C, CCH, T>::class_type const &basic_static_string<C, CCH, T>::operator =(ss_typename_type_k basic_static_string<C, CCH, T>::class_type const &rhs)
{
    return assign(rhs);
}

template<   ss_typename_param_k C
        ,   ss_size_t           CCH
        ,   ss_typename_param_k T
        >
inline ss_typename_type_k basic_static_string<C, CCH, T>::class_type const &basic_static_string<C, CCH, T>::operator =(ss_typename_type_k basic_static_string<C, CCH, T>::char_type const *s)
{
    return assign(s);
}

template<   ss_typename_param_k C
        ,   ss_size_t           CCH
        ,   ss_typename_param_k T
        >
inline ss_typename_type_k basic_static_string<C, CCH, T>::class_type const &basic_static_string<C, CCH, T>::operator =(ss_typename_type_k basic_static_string<C, CCH, T>::char_type c)
{
    m_length = 1;
    m_buffer[0] = c;
    m_buffer[1] = c;

    return *this;
}


template<   ss_typename_param_k C
        ,   ss_size_t           CCH
        ,   ss_typename_param_k T
        >
inline /* static */ ss_typename_type_k basic_static_string<C, CCH, T>::size_type basic_static_string<C, CCH, T>::size()
{
    return max_size();
}

template<   ss_typename_param_k C
        ,   ss_size_t           CCH
        ,   ss_typename_param_k T
        >
inline /* static */ ss_typename_type_k basic_static_string<C, CCH, T>::size_type basic_static_string<C, CCH, T>::max_size()
{
#if defined(__STLSOFT_COMPILER_IS_BORLAND)
    return CCH;
#else /* ? compiler */
    return cch;
#endif /* compiler */
}

template<   ss_typename_param_k C
        ,   ss_size_t           CCH
        ,   ss_typename_param_k T
        >
inline ss_typename_type_k basic_static_string<C, CCH, T>::size_type basic_static_string<C, CCH, T>::length() const
{
    return length_();
}

template<   ss_typename_param_k C
        ,   ss_size_t           CCH
        ,   ss_typename_param_k T
        >
inline ss_typename_type_k basic_static_string<C, CCH, T>::size_type basic_static_string<C, CCH, T>::capacity() const
{
    return cch;
}

template<   ss_typename_param_k C
        ,   ss_size_t           CCH
        ,   ss_typename_param_k T
        >
inline ss_bool_t basic_static_string<C, CCH, T>::empty() const
{
    return m_buffer[0] == 0;
}

template<   ss_typename_param_k C
        ,   ss_size_t           CCH
        ,   ss_typename_param_k T
        >
inline /* static */ ss_sint_t basic_static_string<C, CCH, T>::compare_(ss_typename_type_k basic_static_string<C, CCH, T>::value_type const *lhs, ss_typename_type_k basic_static_string<C, CCH, T>::size_type lhs_len, ss_typename_type_k basic_static_string<C, CCH, T>::value_type const *rhs, ss_typename_type_k basic_static_string<C, CCH, T>::size_type rhs_len)
{
    size_type   cmp_len =   (lhs_len < rhs_len) ? lhs_len : rhs_len;
    ss_int_t    result  =   traits_type::compare(lhs, rhs, cmp_len);

    if(result == 0)
    {
        result = static_cast<ss_int_t>(lhs_len) - static_cast<ss_int_t>(rhs_len);
    }

    return result;
}

template<   ss_typename_param_k C
        ,   ss_size_t           CCH
        ,   ss_typename_param_k T
        >
inline ss_sint_t basic_static_string<C, CCH, T>::compare(ss_typename_type_k basic_static_string<C, CCH, T>::size_type pos, ss_typename_type_k basic_static_string<C, CCH, T>::size_type n, ss_typename_type_k basic_static_string<C, CCH, T>::value_type const *s, ss_typename_type_k basic_static_string<C, CCH, T>::size_type cchRhs) const
{
    size_type   lhs_len =   length();

    if(!(pos < lhs_len))
    {
        pos = lhs_len;
    }
    else
    {
        lhs_len -= pos;
    }

    if(cch < lhs_len)
    {
        lhs_len = cch;
    }

    size_type   rhs_len =   (s == 0) ? 0 : traits_type::length(s);

    if(cchRhs < rhs_len)
    {
        rhs_len = cchRhs;
    }

    return compare_(m_buffer + pos, lhs_len, s, rhs_len);
}

template<   ss_typename_param_k C
        ,   ss_size_t           CCH
        ,   ss_typename_param_k T
        >
inline ss_sint_t basic_static_string<C, CCH, T>::compare(ss_typename_type_k basic_static_string<C, CCH, T>::size_type pos, ss_typename_type_k basic_static_string<C, CCH, T>::size_type n, ss_typename_type_k basic_static_string<C, CCH, T>::value_type const *s) const
{
    size_type   lhs_len =   length();

    if(!(pos < lhs_len))
    {
        pos = lhs_len;
    }
    else
    {
        lhs_len -= pos;
    }

    if(cch < lhs_len)
    {
        lhs_len = cch;
    }

    size_type   rhs_len =   (s == 0) ? 0 : traits_type::length(s);

    return compare_(m_buffer + pos, lhs_len, s, rhs_len);
}

template<   ss_typename_param_k C
        ,   ss_size_t           CCH
        ,   ss_typename_param_k T
        >
inline ss_sint_t basic_static_string<C, CCH, T>::compare(ss_typename_type_k basic_static_string<C, CCH, T>::value_type const *s) const
{
    size_type   lhs_len =   length();
    size_type   rhs_len =   (s == 0) ? 0 : traits_type::length(s);

    return compare_(m_buffer, lhs_len, s, rhs_len);
}

template<   ss_typename_param_k C
        ,   ss_size_t           CCH
        ,   ss_typename_param_k T
        >
inline ss_sint_t basic_static_string<C, CCH, T>::compare(ss_typename_type_k basic_static_string<C, CCH, T>::size_type pos, ss_typename_type_k basic_static_string<C, CCH, T>::size_type n, ss_typename_type_k basic_static_string<C, CCH, T>::class_type const &rhs, ss_typename_type_k basic_static_string<C, CCH, T>::size_type posRhs, ss_typename_type_k basic_static_string<C, CCH, T>::size_type cchRhs) const
{
    size_type   lhs_len =   length();

    if(!(pos < lhs_len))
    {
        pos = lhs_len;
    }
    else
    {
        lhs_len -= pos;
    }

    if(cch < lhs_len)
    {
        lhs_len = cch;
    }

    size_type   rhs_len =   rhs.length();

    if(!(posRhs < rhs_len))
    {
        posRhs = rhs_len;
    }
    else
    {
        rhs_len -= posRhs;
    }

    if(cchRhs < rhs_len)
    {
        rhs_len = cchRhs;
    }

    return compare_(m_buffer + pos, lhs_len, rhs.m_buffer + posRhs, rhs_len);
}

template<   ss_typename_param_k C
        ,   ss_size_t           CCH
        ,   ss_typename_param_k T
        >
inline ss_sint_t basic_static_string<C, CCH, T>::compare(ss_typename_type_k basic_static_string<C, CCH, T>::size_type pos, ss_typename_type_k basic_static_string<C, CCH, T>::size_type n, ss_typename_type_k basic_static_string<C, CCH, T>::class_type const &rhs) const
{
    size_type   lhs_len =   length();

    if(!(pos < lhs_len))
    {
        pos = lhs_len;
    }
    else
    {
        lhs_len -= pos;
    }

    if(cch < lhs_len)
    {
        lhs_len = cch;
    }

    size_type   rhs_len =   rhs.length();

    return compare_(m_buffer + pos, lhs_len, rhs.m_buffer, rhs_len);
}

template<   ss_typename_param_k C
        ,   ss_size_t           CCH
        ,   ss_typename_param_k T
        >
inline ss_sint_t basic_static_string<C, CCH, T>::compare(ss_typename_type_k basic_static_string<C, CCH, T>::class_type const &rhs) const
{
    size_type   lhs_len =   length();
    size_type   rhs_len =   rhs.length();

    return compare_(m_buffer, lhs_len, rhs.m_buffer, rhs_len);
}


template<   ss_typename_param_k C
        ,   ss_size_t           CCH
        ,   ss_typename_param_k T
        >
inline void basic_static_string<C, CCH, T>::resize( ss_typename_type_k basic_static_string<C, CCH, T>::size_type    n
                                                ,   ss_typename_type_k basic_static_string<C, CCH, T>::value_type   ch /* = value_type() */)
{
    stlsoft_message_assert("resize request too large for static_string", !(cch < n));

    size_type const len =   length();

    if(len != n)
    {
        if(len < n)
        {
            traits_type::assign(m_buffer + len, n - len, ch);
        }

        m_length = n;
        m_buffer[n] = 0;
    }
}

template<   ss_typename_param_k C
        ,   ss_size_t           CCH
        ,   ss_typename_param_k T
        >
inline void basic_static_string<C, CCH, T>::clear()
{
    m_length = static_cast<ss_size_t>(-1);
    m_buffer[0] = 0;
}

template<   ss_typename_param_k C
        ,   ss_size_t           CCH
        ,   ss_typename_param_k T
        >
inline ss_typename_type_k basic_static_string<C, CCH, T>::reference basic_static_string<C, CCH, T>::operator [](ss_typename_type_k basic_static_string<C, CCH, T>::size_type index)
{
    stlsoft_message_assert("index access out of range in static_string", index < cch + 1);   // Has to be +1, since legitimate to take address of one-past-the-end

    return m_buffer[index];
}

template<   ss_typename_param_k C
        ,   ss_size_t           CCH
        ,   ss_typename_param_k T
        >
inline ss_typename_type_k basic_static_string<C, CCH, T>::const_reference basic_static_string<C, CCH, T>::operator [](ss_typename_type_k basic_static_string<C, CCH, T>::size_type index) const
{
    stlsoft_message_assert("index access out of range in static_string", index < cch + 1);   // Has to be +1, since legitimate to take address of one-past-the-end

    return m_buffer[index];
}


template<   ss_typename_param_k C
        ,   ss_size_t           CCH
        ,   ss_typename_param_k T
        >
inline ss_typename_type_k basic_static_string<C, CCH, T>::value_type const *basic_static_string<C, CCH, T>::c_str() const
{
    return m_buffer;
}


// Appending

template<   ss_typename_param_k C
        ,   ss_size_t           CCH
        ,   ss_typename_param_k T
        >
inline ss_typename_type_k basic_static_string<C, CCH, T>::class_type &basic_static_string<C, CCH, T>::append(   ss_typename_type_k basic_static_string<C, CCH, T>::char_type const  *s
                                                                                                            ,   ss_typename_type_k basic_static_string<C, CCH, T>::size_type        n)
{
    stlsoft_message_assert("resize request too large for static_string", !(cch < n + length()));

    if(0 != n)
    {
        traits_type::copy(m_buffer + m_length, s, n);

        m_length += n;
        m_buffer[m_length] = 0;
    }

    return *this;
}

template<   ss_typename_param_k C
        ,   ss_size_t           CCH
        ,   ss_typename_param_k T
        >
inline ss_typename_type_k basic_static_string<C, CCH, T>::class_type &basic_static_string<C, CCH, T>::append(ss_typename_type_k basic_static_string<C, CCH, T>::char_type const *s)
{
    return append(s, (NULL == s) ? 0 : traits_type::length(s));
}

template<   ss_typename_param_k C
        ,   ss_size_t           CCH
        ,   ss_typename_param_k T
        >
inline ss_typename_type_k basic_static_string<C, CCH, T>::class_type &basic_static_string<C, CCH, T>::append(   ss_typename_type_k basic_static_string<C, CCH, T>::class_type const &rhs
                                                                                                            ,   ss_typename_type_k basic_static_string<C, CCH, T>::size_type        pos
                                                                                                            ,   ss_typename_type_k basic_static_string<C, CCH, T>::size_type        cch)
{
    char_type const *s  =   rhs.m_buffer;
    size_type       len =   rhs.length();

    if(len < pos)
    {
        pos = len;
    }

    if(len - pos < cch)
    {
        cch = len - pos;
    }

    if(NULL != s)
    {
        s += pos;
    }
    else
    {
        cch = 0;
    }

    return append(s, cch);
}

template<   ss_typename_param_k C
        ,   ss_size_t           CCH
        ,   ss_typename_param_k T
        >
inline ss_typename_type_k basic_static_string<C, CCH, T>::class_type &basic_static_string<C, CCH, T>::append(ss_typename_type_k basic_static_string<C, CCH, T>::class_type const &s)
{
    return append(s.m_buffer, s.length());
}

template<   ss_typename_param_k C
        ,   ss_size_t           CCH
        ,   ss_typename_param_k T
        >
inline ss_typename_type_k basic_static_string<C, CCH, T>::class_type &basic_static_string<C, CCH, T>::append(   ss_typename_type_k basic_static_string<C, CCH, T>::size_type    n
                                                                                                            ,   ss_typename_type_k basic_static_string<C, CCH, T>::char_type    ch)
{
    stlsoft_message_assert("resize request too large for static_string", !(cch < n + length()));

    if(0 != n)
    {
        traits_type::assign(m_buffer + m_length, n, ch);

        m_length += n;
        m_buffer[m_length] = 0;
    }

    return *this;
}

#if !defined(__STLSOFT_CF_MEMBER_TEMPLATE_RANGE_METHOD_SUPPORT)
template<   ss_typename_param_k C
        ,   ss_size_t           CCH
        ,   ss_typename_param_k T
        >
inline ss_typename_type_k basic_static_string<C, CCH, T>::class_type &basic_static_string<C, CCH, T>::append(   ss_typename_type_k basic_static_string<C, CCH, T>::const_iterator first
                                                                                                            ,   ss_typename_type_k basic_static_string<C, CCH, T>::const_iterator last)
{
    // We have to use this strange appearing this, because of Visual C++ .NET's
    // disgusting STL swill. Sigh!
    return append(&(*first), last - first);
}
#endif /* __STLSOFT_CF_MEMBER_TEMPLATE_RANGE_METHOD_SUPPORT */

template<   ss_typename_param_k C
        ,   ss_size_t           CCH
        ,   ss_typename_param_k T
        >
inline ss_typename_type_k basic_static_string<C, CCH, T>::class_type &basic_static_string<C, CCH, T>::operator +=(ss_typename_type_k basic_static_string<C, CCH, T>::char_type ch)
{
    return append(1, ch);
}

template<   ss_typename_param_k C
        ,   ss_size_t           CCH
        ,   ss_typename_param_k T
        >
inline ss_typename_type_k basic_static_string<C, CCH, T>::class_type &basic_static_string<C, CCH, T>::operator +=(const ss_typename_type_k basic_static_string<C, CCH, T>::char_type *s)
{
    return append(s);
}

template<   ss_typename_param_k C
        ,   ss_size_t           CCH
        ,   ss_typename_param_k T
        >
inline ss_typename_type_k basic_static_string<C, CCH, T>::class_type &basic_static_string<C, CCH, T>::operator +=(const ss_typename_type_k basic_static_string<C, CCH, T>::class_type &rhs)
{
    return append(rhs);
}

template<   ss_typename_param_k C
        ,   ss_size_t           CCH
        ,   ss_typename_param_k T
        >
inline void basic_static_string<C, CCH, T>::push_back(ss_typename_type_k basic_static_string<C, CCH, T>::char_type ch)
{
    append(1, ch);
}

template<   ss_typename_param_k C
        ,   ss_size_t           CCH
        ,   ss_typename_param_k T
        >
inline void basic_static_string<C, CCH, T>::reserve(ss_typename_type_k basic_static_string<C, CCH, T>::size_type n)
{
    stlsoft_message_assert("reserve request out of range in static_string", !(cch < n));
}

template<   ss_typename_param_k C
        ,   ss_size_t           CCH
        ,   ss_typename_param_k T
        >
inline void basic_static_string<C, CCH, T>::swap(ss_typename_type_k basic_static_string<C, CCH, T>::class_type &other)
{
    value_type  buffer[CCH];

    traits_type::copy(        buffer, other.m_buffer, 1 + other.length());
    traits_type::copy(other.m_buffer,       m_buffer, 1 +       length());
    traits_type::copy(      m_buffer,         buffer, 1 + other.length());
}

template<   ss_typename_param_k C
        ,   ss_size_t           CCH
        ,   ss_typename_param_k T
        >
inline ss_typename_type_k basic_static_string<C, CCH, T>::value_type const *basic_static_string<C, CCH, T>::data() const
{
    return m_buffer;
}

template<   ss_typename_param_k C
        ,   ss_size_t           CCH
        ,   ss_typename_param_k T
        >
inline ss_typename_type_k basic_static_string<C, CCH, T>::size_type basic_static_string<C, CCH, T>::copy(ss_typename_type_k basic_static_string<C, CCH, T>::value_type *dest, ss_typename_type_k basic_static_string<C, CCH, T>::size_type cch_, ss_typename_type_k basic_static_string<C, CCH, T>::size_type pos /* = 0 */) const
{
    size_type   len =   length();

    if(pos < len)
    {
        if(len < pos + cch_)
        {
            cch_ = len - pos;
        }

        traits_type::copy(dest, data() + pos, cch_);
    }
    else
    {
        cch_ = 0;
    }

    return cch_;
}


template<   ss_typename_param_k C
        ,   ss_size_t           CCH
        ,   ss_typename_param_k T
        >
inline ss_typename_type_k basic_static_string<C, CCH, T>::const_iterator basic_static_string<C, CCH, T>::begin() const
{
    return m_buffer;
}

template<   ss_typename_param_k C
        ,   ss_size_t           CCH
        ,   ss_typename_param_k T
        >
inline ss_typename_type_k basic_static_string<C, CCH, T>::const_iterator basic_static_string<C, CCH, T>::end() const
{
    return m_buffer + length_();
}

template<   ss_typename_param_k C
        ,   ss_size_t           CCH
        ,   ss_typename_param_k T
        >
inline ss_typename_type_k basic_static_string<C, CCH, T>::iterator basic_static_string<C, CCH, T>::begin()
{
    return m_buffer;
}

template<   ss_typename_param_k C
        ,   ss_size_t           CCH
        ,   ss_typename_param_k T
        >
inline ss_typename_type_k basic_static_string<C, CCH, T>::iterator basic_static_string<C, CCH, T>::end()
{
    return m_buffer + length_();
}


#if defined(__STLSOFT_CF_BIDIRECTIONAL_ITERATOR_SUPPORT)
template<   ss_typename_param_k C
        ,   ss_size_t           CCH
        ,   ss_typename_param_k T
        >
inline ss_typename_type_k basic_static_string<C, CCH, T>::const_reverse_iterator basic_static_string<C, CCH, T>::rbegin() const
{
    return const_reverse_iterator(end());
}

template<   ss_typename_param_k C
        ,   ss_size_t           CCH
        ,   ss_typename_param_k T
        >
inline ss_typename_type_k basic_static_string<C, CCH, T>::const_reverse_iterator basic_static_string<C, CCH, T>::rend() const
{
    return const_reverse_iterator(begin());
}

template<   ss_typename_param_k C
        ,   ss_size_t           CCH
        ,   ss_typename_param_k T
        >
inline ss_typename_type_k basic_static_string<C, CCH, T>::reverse_iterator basic_static_string<C, CCH, T>::rbegin()
{
    return reverse_iterator(end());
}

template<   ss_typename_param_k C
        ,   ss_size_t           CCH
        ,   ss_typename_param_k T
        >
inline ss_typename_type_k basic_static_string<C, CCH, T>::reverse_iterator basic_static_string<C, CCH, T>::rend()
{
    return reverse_iterator(begin());
}
#endif /* __STLSOFT_CF_BIDIRECTIONAL_ITERATOR_SUPPORT */

#endif /* !__STLSOFT_DOCUMENTATION_SKIP_SECTION */

/* ////////////////////////////////////////////////////////////////////////// */

/// @} // end of group stlsoft_string_library

/* ////////////////////////////////////////////////////////////////////////// */

#ifndef _STLSOFT_NO_NAMESPACE
} // namespace stlsoft
#endif /* _STLSOFT_NO_NAMESPACE */

/* ////////////////////////////////////////////////////////////////////////// */

#endif /* !STLSOFT_INCL_H_STLSOFT_STATIC_STRING */

/* ////////////////////////////////////////////////////////////////////////// */
