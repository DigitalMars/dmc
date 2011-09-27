/* /////////////////////////////////////////////////////////////////////////////
 * File:        stlsoft_simple_string.h (formerly MSString.h, ::SynesisDev)
 *
 * Purpose:     simple_string class.
 *
 * Created:     19th March 1993
 * Updated:     11th September 2004
 *
 * Home:        http://stlsoft.org/
 *
 * Copyright (c) 1993-2004, Matthew Wilson and Synesis Software
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


/// \file stlsoft_simple_string.h
///
/// simple_string class.

#ifndef STLSOFT_INCL_H_STLSOFT_SIMPLE_STRING
#define STLSOFT_INCL_H_STLSOFT_SIMPLE_STRING

#ifndef __STLSOFT_DOCUMENTATION_SKIP_SECTION
# define STLSOFT_VER_H_STLSOFT_SIMPLE_STRING_MAJOR     2
# define STLSOFT_VER_H_STLSOFT_SIMPLE_STRING_MINOR     0
# define STLSOFT_VER_H_STLSOFT_SIMPLE_STRING_REVISION  1
# define STLSOFT_VER_H_STLSOFT_SIMPLE_STRING_EDIT      152
#endif /* !__STLSOFT_DOCUMENTATION_SKIP_SECTION */

/* /////////////////////////////////////////////////////////////////////////////
 * Includes
 */

#ifndef STLSOFT_INCL_H_STLSOFT
# include "stlsoft.h"                   // Include the STLSoft root header
#endif /* !STLSOFT_INCL_H_STLSOFT */
#ifndef STLSOFT_INCL_H_STLSOFT_NEW_ALLOCATOR
# include "stlsoft_new_allocator.h"     // stlsoft::new_allocator
#endif /* !STLSOFT_INCL_H_STLSOFT_NEW_ALLOCATOR */
#ifndef STLSOFT_INCL_H_STLSOFT_CHAR_TRAITS
# include "stlsoft_char_traits.h"      // stlsoft::char_traits
#endif /* !STLSOFT_INCL_H_STLSOFT_CHAR_TRAITS */
#ifndef STLSOFT_INCL_H_STLSOFT_ITERATOR
# include "stlsoft_iterator.h"          // stlsoft iterator bases
#endif /* !STLSOFT_INCL_H_STLSOFT_ITERATOR */
#ifndef STLSOFT_INCL_H_STLSOFT_AUTO_BUFFER
# include "stlsoft_auto_buffer.h"       // stlsoft::auto_buffer
#endif /* !STLSOFT_INCL_H_STLSOFT_AUTO_BUFFER */
#ifndef STLSOFT_INCL_H_STLSOFT_SIMPLE_ALGORITHMS
# include "stlsoft_simple_algorithms.h" // stlsoft::for_each_set_value
#endif /* !STLSOFT_INCL_H_STLSOFT_SIMPLE_ALGORITHMS */
#ifndef STLSOFT_INCL_H_STLSOFT_SAP_CAST
# include "stlsoft_sap_cast.h"          // stlsoft::sap_cast
#endif /* !STLSOFT_INCL_H_STLSOFT_SAP_CAST */

/* /////////////////////////////////////////////////////////////////////////////
 *
 */

#if defined(__STLSOFT_COMPILER_IS_MSVC) && \
    _MSC_VER < 1200
# define STLSOFT_SIMPLE_STRING_ITERATOR_METHODS_INLINE
#endif /* compiler */

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

// class basic_simple_string
/// Simple string class
///
/// \param C The character type
/// \param T The traits type. On translators that support default template arguments this is defaulted to char_traits<C>
/// \param A The allocator type. On translators that support default template arguments this is defaulted to new_allocator<C>
template<   ss_typename_param_k C
#ifdef __STLSOFT_CF_TEMPLATE_CLASS_DEFAULT_CLASS_ARGUMENT_SUPPORT
        ,   ss_typename_param_k T = stlsoft_char_traits<C>
        ,   ss_typename_param_k A = new_allocator<C>
#else
        ,   ss_typename_param_k T /* = stlsoft_char_traits<C> */
        ,   ss_typename_param_k A /* = new_allocator<C> */
#endif /* __STLSOFT_CF_TEMPLATE_CLASS_DEFAULT_CLASS_ARGUMENT_SUPPORT */
        >
class basic_simple_string
{
public:
    /// The value type
    typedef C                               value_type;
    /// The traits type
    typedef T                               traits_type;
    /// The allocator type
    typedef A                               allocator_type;
    /// The current parameterisation of the type
    typedef basic_simple_string<C, T, A>    class_type;
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
    basic_simple_string();
    /// Copy constructor
    basic_simple_string(class_type const &rhs);
    /// Construct from the given string at the specified position
    basic_simple_string(class_type const &s, size_type pos);
    /// Construct with \c cch characters from the given string at the specified position
    basic_simple_string(class_type const &s, size_type pos, size_type cch);
    /// Construct from the given character string
    basic_simple_string(char_type const *s); // Not, not explicit. Sigh
    /// Construct with \c cch characters from the given character string
    basic_simple_string(char_type const *s, size_type cch);
    /// Construct with \c cch characters each set to \c ch
    basic_simple_string(size_type cch, char_type ch);
    /// Construct from the range [first:last)
#if !defined(__STLSOFT_CF_MEMBER_TEMPLATE_RANGE_METHOD_SUPPORT)
    basic_simple_string(char_type const *first, char_type const *last);
#else
    template <ss_typename_param_k II>
    basic_simple_string(II first, II last)
        : m_buffer(NULL)
    {
        assign(first, last);
    }
#endif /* __STLSOFT_CF_MEMBER_TEMPLATE_RANGE_METHOD_SUPPORT */
    /// Destructor
    ~basic_simple_string();
//@}

/// \name Assignment
//@{
public:
    /// Assigns from the given character string
    class_type &assign(char_type const *s);
    /// Assigns with \c cch characters from the given character string
    class_type &assign(char_type const *s, size_type cch);
    /// Assigns with \c cch characters from the given character string at the specified position
    class_type &assign(class_type const &str, size_type pos, size_type cch);
    /// Assigns from the given string
    class_type &assign(class_type const &str);
    /// Assigns \c cch characters with the value \c ch
    class_type &assign(size_type cch, char_type ch);
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

        return assign_(first, last, traits_t::iterator_category());
# else
        return assign_(first, last, stlsoft_iterator_query_category(II, first));
# endif /* GCC < 3 */
    }
#endif /* __STLSOFT_CF_MEMBER_TEMPLATE_RANGE_METHOD_SUPPORT */

    /// Copy assignment operator
    class_type const &operator =(class_type const &rhs);
    /// Assignment operator
    class_type const &operator =(char_type const *s);
    /// Assignment operator
    class_type const &operator =(char_type ch);
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
    /// Reserves at least cch characters
    void reserve(size_type cch);
    /// Swaps the contents between \c this and \c other
    void swap(class_type &other);

    /// Resizes the string
    ///
    /// \param cch The new size of the string
    /// \param ch The value with which to initialise additional items if the string is expanded
    void resize(size_type cch, value_type ch = value_type());

    /// Empties the string
    void clear();
//@}

/// \name Attributes
//@{
public:
    /// The number of elements in the string
    size_type size() const;
    /// The maximum number of elements that can be stored in the string
    size_type max_size() const;
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
    ss_sint_t compare(size_type pos, size_type cch, value_type const *s, size_type cchRhs) const;
    /// Compares \c this with the given string
    ss_sint_t compare(size_type pos, size_type cch, value_type const *s) const;
    /// Compares \c this with the given string
    ss_sint_t compare(value_type const *s) const;
    /// Compares \c this with the given string
    ss_sint_t compare(size_type pos, size_type cch, class_type const &rhs, size_type posRhs, size_type cchRhs) const;
    /// Compares \c this with the given string
    ss_sint_t compare(size_type pos, size_type cch, class_type const &rhs) const;
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

    /// Returns the first character in the string
    ///
    /// \note It is us to the user to ensure that the string is not empty
    reference               front();
    /// Returns the last character in the string
    ///
    /// \note It is us to the user to ensure that the string is not empty
    reference               back();
    /// Returns the first character in the string
    ///
    /// \note It is us to the user to ensure that the string is not empty
    const_reference         front() const;
    /// Returns the last character in the string
    ///
    /// \note It is us to the user to ensure that the string is not empty
    const_reference         back() const;

    /// Copies elements into the given destination
    size_type copy(value_type *dest, size_type cch, size_type pos = 0) const;
//@}

/// \name Iteration
//@{
public:
    /// Begins the iteration
    ///
    /// \return A non-mutable (const) iterator representing the start of the sequence
#ifdef STLSOFT_SIMPLE_STRING_ITERATOR_METHODS_INLINE
    const_iterator          begin() const
    {
        return const_cast<class_type*>(this)->begin_();
    }
#else /* ? STLSOFT_SIMPLE_STRING_ITERATOR_METHODS_INLINE */
    const_iterator          begin() const;
#endif /* STLSOFT_SIMPLE_STRING_ITERATOR_METHODS_INLINE */
    /// Ends the iteration
    ///
    /// \return A non-mutable (const) iterator representing the end of the sequence
#ifdef STLSOFT_SIMPLE_STRING_ITERATOR_METHODS_INLINE
    const_iterator          end() const
    {
        return const_cast<class_type*>(this)->end_();
    }
#else /* ? STLSOFT_SIMPLE_STRING_ITERATOR_METHODS_INLINE */
    const_iterator          end() const;
#endif /* STLSOFT_SIMPLE_STRING_ITERATOR_METHODS_INLINE */
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
    enum { alloc_quantum = 31 };    // Must be (2^n - 1)

    struct string_buffer
    {
        size_type   capacity;       // The number of char places in the buffer
        size_type   length;         // The number of chars in the string (<= capacity)
        char_type   contents[1];    // The first element in the array
    };

#ifdef STLSOFT_SIMPLE_STRING_NO_PTR_ADJUST
    typedef string_buffer       *member_pointer;
    typedef string_buffer const *member_const_pointer;
#else /* ? STLSOFT_SIMPLE_STRING_NO_PTR_ADJUST */
    typedef char_type           *member_pointer;
    typedef char_type const     *member_const_pointer;
#endif /* STLSOFT_SIMPLE_STRING_NO_PTR_ADJUST */

    // Conversion between member pointer and character pointer
    static char_type            *char_pointer_from_member_pointer_(member_pointer );

    // Conversion between pointer and buffer
    static string_buffer        *string_buffer_from_member_pointer_(member_pointer );
    static string_buffer const  *string_buffer_from_member_pointer_(member_const_pointer );

    // Conversion between buffer and pointer
    static member_pointer       member_pointer_from_string_buffer_(string_buffer *);

    // Creating buffer
    static member_pointer       alloc_buffer_(char_type const *s, size_type capacity, size_type length);
    static member_pointer       alloc_buffer_(char_type const *s, size_type cch);
    static member_pointer       alloc_buffer_(char_type const *s);

    // Copying a buffer
    static member_pointer       copy_buffer_(member_pointer );

    // Destroying buffer
    static void                 destroy_buffer_(string_buffer *);
    static void                 destroy_buffer_(char_type *);

    // Iteration
    pointer                     begin_();
    pointer                     end_();

    // Invariance
    ss_bool_t is_valid_() const;

    // Empty string
    static char_type const *empty_string_();

    // Comparison
    static ss_sint_t compare_(char_type const *lhs, size_type lhs_len, char_type const *rhs, size_type rhs_len);

    // Assignment
#if defined(__STLSOFT_CF_MEMBER_TEMPLATE_RANGE_METHOD_SUPPORT)
    template <ss_typename_param_k II>
    class_type &assign_(II first, II last, stlsoft_ns_qual_std(input_iterator_tag))
    {
        stlsoft_ns_qual_std(copy)(first, last, stlsoft_ns_qual_std(back_inserter)(*this));

        stlsoft_assert(is_valid_());
        return *this;
    }
    template <ss_typename_param_k II>
    class_type &assign_(II first, II last, stlsoft_ns_qual_std(forward_iterator_tag))
    {
        typedef auto_buffer<char_type, allocator_type>  buffer_t;

        buffer_t    buffer(stlsoft_ns_qual_std(distance)(first, last));

        stlsoft_ns_qual_std(copy)(first, last, buffer.begin());
        assign(buffer, buffer.size());

        stlsoft_assert(is_valid_());
        return *this;
    }
#endif /* __STLSOFT_CF_MEMBER_TEMPLATE_RANGE_METHOD_SUPPORT */

    // Appending
#if defined(__STLSOFT_CF_MEMBER_TEMPLATE_RANGE_METHOD_SUPPORT)
    template <ss_typename_param_k II>
    class_type &append_(II first, II last, stlsoft_ns_qual_std(input_iterator_tag))
    {
        stlsoft_ns_qual_std(copy)(first, last, stlsoft_ns_qual_std(back_inserter)(*this));

        stlsoft_assert(is_valid_());
        return *this;
    }
    template <ss_typename_param_k II>
    class_type &append_(II first, II last, stlsoft_ns_qual_std(forward_iterator_tag))
    {
        typedef auto_buffer<char_type, allocator_type>  buffer_t;

        buffer_t    buffer(stlsoft_ns_qual_std(distance)(first, last));

        stlsoft_ns_qual_std(copy)(first, last, buffer.begin());
        append(buffer, buffer.size());

        stlsoft_assert(is_valid_());
        return *this;
    }
#endif /* __STLSOFT_CF_MEMBER_TEMPLATE_RANGE_METHOD_SUPPORT */
//@}

/// \name Members
//@{
private:
    member_pointer  m_buffer;
//@}
};

/* /////////////////////////////////////////////////////////////////////////////
 * Operators
 */

#ifndef __STLSOFT_DOCUMENTATION_SKIP_SECTION

// operator ==

template<   ss_typename_param_k C
        ,   ss_typename_param_k T
        ,   ss_typename_param_k A
        >
inline ss_bool_t operator ==(basic_simple_string<C, T, A> const &lhs, basic_simple_string<C, T, A> const &rhs)
{
    return 0 == lhs.compare(rhs);
}

template<   ss_typename_param_k C
        ,   ss_typename_param_k T
        ,   ss_typename_param_k A
        >
#ifdef __STLSOFT_CF_TEMPLATE_OUTOFCLASSFN_QUALIFIED_TYPE_SUPPORT
inline ss_bool_t operator ==(basic_simple_string<C, T, A> const &lhs, ss_typename_type_k basic_simple_string<C, T, A>::char_type const *rhs)
#else
inline ss_bool_t operator ==(basic_simple_string<C, T, A> const &lhs, C const *rhs)
#endif /* __STLSOFT_CF_TEMPLATE_OUTOFCLASSFN_QUALIFIED_TYPE_SUPPORT */
{
    return 0 == lhs.compare(rhs);
}

template<   ss_typename_param_k C
        ,   ss_typename_param_k T
        ,   ss_typename_param_k A
        >
#ifdef __STLSOFT_CF_TEMPLATE_OUTOFCLASSFN_QUALIFIED_TYPE_SUPPORT
inline ss_bool_t operator ==(ss_typename_type_k basic_simple_string<C, T, A>::char_type const *lhs, basic_simple_string<C, T, A> const &rhs)
#else
inline ss_bool_t operator ==(C *lhs, basic_simple_string<C, T, A> const &rhs)
#endif /* __STLSOFT_CF_TEMPLATE_OUTOFCLASSFN_QUALIFIED_TYPE_SUPPORT */
{
    return 0 == rhs.compare(lhs);
}

// operator !=

template<   ss_typename_param_k C
        ,   ss_typename_param_k T
        ,   ss_typename_param_k A
        >
inline ss_bool_t operator !=(basic_simple_string<C, T, A> const &lhs, basic_simple_string<C, T, A> const &rhs)
{
    return 0 != lhs.compare(rhs);
}
template<   ss_typename_param_k C
        ,   ss_typename_param_k T
        ,   ss_typename_param_k A
        >
#ifdef __STLSOFT_CF_TEMPLATE_OUTOFCLASSFN_QUALIFIED_TYPE_SUPPORT
inline ss_bool_t operator !=(basic_simple_string<C, T, A> const &lhs, ss_typename_type_k basic_simple_string<C, T, A>::char_type const *rhs)
#else
inline ss_bool_t operator !=(basic_simple_string<C, T, A> const &lhs, C const *rhs)
#endif /* __STLSOFT_CF_TEMPLATE_OUTOFCLASSFN_QUALIFIED_TYPE_SUPPORT */
{
    return 0 != lhs.compare(rhs);
}
template<   ss_typename_param_k C
        ,   ss_typename_param_k T
        ,   ss_typename_param_k A
        >
#ifdef __STLSOFT_CF_TEMPLATE_OUTOFCLASSFN_QUALIFIED_TYPE_SUPPORT
inline ss_bool_t operator !=(ss_typename_type_k basic_simple_string<C, T, A>::char_type const *lhs, basic_simple_string<C, T, A> const &rhs)
#else
inline ss_bool_t operator !=(C const *lhs, basic_simple_string<C, T, A> const &rhs)
#endif /* __STLSOFT_CF_TEMPLATE_OUTOFCLASSFN_QUALIFIED_TYPE_SUPPORT */
{
    return 0 != rhs.compare(lhs);
}

// operator <

template<   ss_typename_param_k C
        ,   ss_typename_param_k T
        ,   ss_typename_param_k A
        >
inline ss_bool_t operator <(basic_simple_string<C, T, A> const &lhs, basic_simple_string<C, T, A> const &rhs)
{
    return lhs.compare(rhs) < 0;
}

template<   ss_typename_param_k C
        ,   ss_typename_param_k T
        ,   ss_typename_param_k A
        >
#ifdef __STLSOFT_CF_TEMPLATE_OUTOFCLASSFN_QUALIFIED_TYPE_SUPPORT
inline ss_bool_t operator <(basic_simple_string<C, T, A> const &lhs, ss_typename_type_k basic_simple_string<C, T, A>::char_type const *rhs)
#else
inline ss_bool_t operator <(basic_simple_string<C, T, A> const &lhs, C const *rhs)
#endif /* __STLSOFT_CF_TEMPLATE_OUTOFCLASSFN_QUALIFIED_TYPE_SUPPORT */
{
    return lhs.compare(rhs) < 0;
}

template<   ss_typename_param_k C
        ,   ss_typename_param_k T
        ,   ss_typename_param_k A
        >
#ifdef __STLSOFT_CF_TEMPLATE_OUTOFCLASSFN_QUALIFIED_TYPE_SUPPORT
inline ss_bool_t operator <(ss_typename_type_k basic_simple_string<C, T, A>::char_type const *lhs, basic_simple_string<C, T, A> const &rhs)
#else
inline ss_bool_t operator <(C const *lhs, basic_simple_string<C, T, A> const &rhs)
#endif /* __STLSOFT_CF_TEMPLATE_OUTOFCLASSFN_QUALIFIED_TYPE_SUPPORT */
{
    return rhs.compare(lhs) > 0;
}

// operator <=

template<   ss_typename_param_k C
        ,   ss_typename_param_k T
        ,   ss_typename_param_k A
        >
inline ss_bool_t operator <=(basic_simple_string<C, T, A> const &lhs, basic_simple_string<C, T, A> const &rhs)
{
    return lhs.compare(rhs) <= 0;
}
template<   ss_typename_param_k C
        ,   ss_typename_param_k T
        ,   ss_typename_param_k A
        >
#ifdef __STLSOFT_CF_TEMPLATE_OUTOFCLASSFN_QUALIFIED_TYPE_SUPPORT
inline ss_bool_t operator <=(basic_simple_string<C, T, A> const &lhs, ss_typename_type_k basic_simple_string<C, T, A>::char_type const *rhs)
#else
inline ss_bool_t operator <=(basic_simple_string<C, T, A> const &lhs, C const *rhs)
#endif /* __STLSOFT_CF_TEMPLATE_OUTOFCLASSFN_QUALIFIED_TYPE_SUPPORT */
{
    return lhs.compare(rhs) <= 0;
}
template<   ss_typename_param_k C
        ,   ss_typename_param_k T
        ,   ss_typename_param_k A
        >
#ifdef __STLSOFT_CF_TEMPLATE_OUTOFCLASSFN_QUALIFIED_TYPE_SUPPORT
inline ss_bool_t operator <=(ss_typename_type_k basic_simple_string<C, T, A>::char_type const *lhs, basic_simple_string<C, T, A> const &rhs)
#else
inline ss_bool_t operator <=(C const *lhs, basic_simple_string<C, T, A> const &rhs)
#endif /* __STLSOFT_CF_TEMPLATE_OUTOFCLASSFN_QUALIFIED_TYPE_SUPPORT */
{
    return rhs.compare(lhs) >= 0;
}

// operator >

template<   ss_typename_param_k C
        ,   ss_typename_param_k T
        ,   ss_typename_param_k A
        >
inline ss_bool_t operator >(basic_simple_string<C, T, A> const &lhs, basic_simple_string<C, T, A> const &rhs)
{
    return lhs.compare(rhs) > 0;
}
template<   ss_typename_param_k C
        ,   ss_typename_param_k T
        ,   ss_typename_param_k A
        >
#ifdef __STLSOFT_CF_TEMPLATE_OUTOFCLASSFN_QUALIFIED_TYPE_SUPPORT
inline ss_bool_t operator >(basic_simple_string<C, T, A> const &lhs, ss_typename_type_k basic_simple_string<C, T, A>::char_type const *rhs)
#else
inline ss_bool_t operator >(basic_simple_string<C, T, A> const &lhs, C const *rhs)
#endif /* __STLSOFT_CF_TEMPLATE_OUTOFCLASSFN_QUALIFIED_TYPE_SUPPORT */
{
    return lhs.compare(rhs) > 0;
}
template<   ss_typename_param_k C
        ,   ss_typename_param_k T
        ,   ss_typename_param_k A
        >
#ifdef __STLSOFT_CF_TEMPLATE_OUTOFCLASSFN_QUALIFIED_TYPE_SUPPORT
inline ss_bool_t operator >(ss_typename_type_k basic_simple_string<C, T, A>::char_type const *lhs, basic_simple_string<C, T, A> const &rhs)
#else
inline ss_bool_t operator >(C const *lhs, basic_simple_string<C, T, A> const &rhs)
#endif /* __STLSOFT_CF_TEMPLATE_OUTOFCLASSFN_QUALIFIED_TYPE_SUPPORT */
{
    return rhs.compare(lhs) < 0;
}

// operator >=

template<   ss_typename_param_k C
        ,   ss_typename_param_k T
        ,   ss_typename_param_k A
        >
inline ss_bool_t operator >=(basic_simple_string<C, T, A> const &lhs, basic_simple_string<C, T, A> const &rhs)
{
    return lhs.compare(rhs) >= 0;
}
template<   ss_typename_param_k C
        ,   ss_typename_param_k T
        ,   ss_typename_param_k A
        >
#ifdef __STLSOFT_CF_TEMPLATE_OUTOFCLASSFN_QUALIFIED_TYPE_SUPPORT
inline ss_bool_t operator >=(basic_simple_string<C, T, A> const &lhs, ss_typename_type_k basic_simple_string<C, T, A>::char_type const *rhs)
#else
inline ss_bool_t operator >=(basic_simple_string<C, T, A> const &lhs, C const *rhs)
#endif /* __STLSOFT_CF_TEMPLATE_OUTOFCLASSFN_QUALIFIED_TYPE_SUPPORT */
{
    return lhs.compare(rhs) >= 0;
}
template<   ss_typename_param_k C
        ,   ss_typename_param_k T
        ,   ss_typename_param_k A
        >
#ifdef __STLSOFT_CF_TEMPLATE_OUTOFCLASSFN_QUALIFIED_TYPE_SUPPORT
inline ss_bool_t operator >=(ss_typename_type_k basic_simple_string<C, T, A>::char_type const *lhs, basic_simple_string<C, T, A> const &rhs)
#else
inline ss_bool_t operator >=(C const *lhs, basic_simple_string<C, T, A> const &rhs)
#endif /* __STLSOFT_CF_TEMPLATE_OUTOFCLASSFN_QUALIFIED_TYPE_SUPPORT */
{
    return rhs.compare(lhs) <= 0;
}

// operator +

template<   ss_typename_param_k C
        ,   ss_typename_param_k T
        ,   ss_typename_param_k A
        >
inline basic_simple_string<C, T, A> operator +(basic_simple_string<C, T, A> const &lhs, basic_simple_string<C, T, A> const &rhs)
{
    return basic_simple_string<C, T, A>(lhs) += rhs;
}
template<   ss_typename_param_k C
        ,   ss_typename_param_k T
        ,   ss_typename_param_k A
        >
#ifdef __STLSOFT_CF_TEMPLATE_OUTOFCLASSFN_QUALIFIED_TYPE_SUPPORT
inline basic_simple_string<C, T, A> operator +(basic_simple_string<C, T, A> const &lhs, ss_typename_type_k basic_simple_string<C, T, A>::char_type const *rhs)
#else
inline basic_simple_string<C, T, A> operator +(basic_simple_string<C, T, A> const &lhs, C const *rhs)
#endif /* __STLSOFT_CF_TEMPLATE_OUTOFCLASSFN_QUALIFIED_TYPE_SUPPORT */
{
    return basic_simple_string<C, T, A>(lhs) += rhs;
}
template<   ss_typename_param_k C
        ,   ss_typename_param_k T
        ,   ss_typename_param_k A
        >
#ifdef __STLSOFT_CF_TEMPLATE_OUTOFCLASSFN_QUALIFIED_TYPE_SUPPORT
inline basic_simple_string<C, T, A> operator +(ss_typename_type_k basic_simple_string<C, T, A>::char_type const *lhs, basic_simple_string<C, T, A> const &rhs)
#else
inline basic_simple_string<C, T, A> operator +(C const *lhs, basic_simple_string<C, T, A> const &rhs)
#endif /* __STLSOFT_CF_TEMPLATE_OUTOFCLASSFN_QUALIFIED_TYPE_SUPPORT */
{
    return basic_simple_string<C, T, A>(lhs) += rhs;
}
template<   ss_typename_param_k C
        ,   ss_typename_param_k T
        ,   ss_typename_param_k A
        >
#ifdef __STLSOFT_CF_TEMPLATE_OUTOFCLASSFN_QUALIFIED_TYPE_SUPPORT
inline basic_simple_string<C, T, A> operator +(basic_simple_string<C, T, A> const &lhs, ss_typename_type_k basic_simple_string<C, T, A>::char_type rhs)
#else
inline basic_simple_string<C, T, A> operator +(basic_simple_string<C, T, A> const &lhs, C rhs)
#endif /* __STLSOFT_CF_TEMPLATE_OUTOFCLASSFN_QUALIFIED_TYPE_SUPPORT */
{
    return basic_simple_string<C, T, A>(lhs) += rhs;
}
template<   ss_typename_param_k C
        ,   ss_typename_param_k T
        ,   ss_typename_param_k A
        >
#ifdef __STLSOFT_CF_TEMPLATE_OUTOFCLASSFN_QUALIFIED_TYPE_SUPPORT
inline basic_simple_string<C, T, A> operator +(ss_typename_type_k basic_simple_string<C, T, A>::char_type lhs, basic_simple_string<C, T, A> const &rhs)
#else
inline basic_simple_string<C, T, A> operator +(C lhs, basic_simple_string<C, T, A> const &rhs)
#endif /* __STLSOFT_CF_TEMPLATE_OUTOFCLASSFN_QUALIFIED_TYPE_SUPPORT */
{
    return basic_simple_string<C, T, A>(1, lhs) += rhs;
}

#endif /* !__STLSOFT_DOCUMENTATION_SKIP_SECTION */

/* /////////////////////////////////////////////////////////////////////////////
 * Shims
 */

/* c_str_ptr_null */

/// \brief Returns the corresponding C-string pointer of \c s, or a null pointer
template<   ss_typename_param_k C
        ,   ss_typename_param_k T
        ,   ss_typename_param_k A
        >
inline C const *c_str_ptr_null(basic_simple_string<C, T, A> const &s)
{
    return (0 == s.length()) ? NULL : s.c_str();
}

/* c_str_ptr */

/// \brief Returns the corresponding C-string pointer of \c s
template<   ss_typename_param_k C
        ,   ss_typename_param_k T
        ,   ss_typename_param_k A
        >
inline C const *c_str_ptr(basic_simple_string<C, T, A> const &s)
{
    return s.c_str();
}

/// \brief Returns the corresponding C-string pointer of \c s
template<   ss_typename_param_k T
        ,   ss_typename_param_k A
        >
inline ss_char_a_t const *c_str_ptr_a(basic_simple_string<ss_char_a_t, T, A> const &s)
{
    return s.c_str();
}

/// \brief Returns the corresponding C-string pointer of \c s
template<   ss_typename_param_k T
        ,   ss_typename_param_k A
        >
inline ss_char_w_t const *c_str_ptr_w(basic_simple_string<ss_char_w_t, T, A> const &s)
{
    return s.c_str();
}

/* c_str_ptr_len */

/// \brief Returns the length (in characters) of \c s, <b><i>not</i></b> including the null-terminating character
template<   ss_typename_param_k C
        ,   ss_typename_param_k T
        ,   ss_typename_param_k A
        >
inline ss_size_t c_str_len(basic_simple_string<C, T, A> const &s)
{
    return s.length();
}

/* c_str_ptr_size */

/// \brief Returns the size (in bytes) of the contents of \c s, <b><i>not</i></b> including the null-terminating character
template<   ss_typename_param_k C
        ,   ss_typename_param_k T
        ,   ss_typename_param_k A
        >
inline ss_size_t c_str_size(basic_simple_string<C, T, A> const &s)
{
    return c_str_len(s) * sizeof(C);
}

/* operator << */
template<   ss_typename_param_k S
        ,   ss_typename_param_k C
        ,   ss_typename_param_k T
        ,   ss_typename_param_k A
        >
inline S &operator <<(S & s, basic_simple_string<C, T, A> const &str)
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
    ss_bool_t test_stlsoft_simple_string(unittest_reporter *r)
    {
        ss_bool_t               bSuccess    =   true;

        unittest_initialiser    init(r, "STLSoft", "basic_simple_string", __FILE__);

        typedef basic_simple_string<char
                                ,   stlsoft_char_traits<char>
                                ,   new_allocator<char>
                                >                               string_t;

        if(string_t("abc") != "abc")
        {
            r->report("construction failed ", __LINE__);
            bSuccess = false;
        }

        if(string_t("abc") >= "def")
        {
            r->report("comparison failed ", __LINE__);
            bSuccess = false;
        }

        // Self assignment
        string_t const  s3("abcdefghijklm");
        string_t        s4;

        s4 = s3;    // setup
        s4 = s4;    // action
        if(s3 != s4)
        {
            r->report("self-assignment (1) failed ", __LINE__);
            bSuccess = false;
        }

        s4 = s3;            // setup
        s4.assign(&s4[2]);  // action
        if("abcdefghijklm" + 2 != s4)
        {
            r->report("self-assignment (2) failed ", __LINE__);
            bSuccess = false;
        }

        s4 = s3;                // setup
        s4.append(&s4[2], 4);   // action
        if("abcdefghijklmcdef" != s4)
        {
            r->report("self-append (1) failed ", __LINE__);
            bSuccess = false;
        }

        s4 = s3;            // setup
        s4.append(&s4[1]);  // action
        if("abcdefghijklmbcdefghijklm" != s4)
        {
            r->report("self-append (2) failed ", __LINE__);
            bSuccess = false;
        }

        return bSuccess;
    }

    unittest_registrar    unittest_stlsoft_simple_string(test_stlsoft_simple_string);

} // namespace winstl_test

#endif /* STLSOFT_UNITTEST */

/* /////////////////////////////////////////////////////////////////////////////
 * Implementation
 */

#ifndef __STLSOFT_DOCUMENTATION_SKIP_SECTION

// Implementation

template<   ss_typename_param_k C
        ,   ss_typename_param_k T
        ,   ss_typename_param_k A
        >
inline /* static */ ss_typename_type_k basic_simple_string<C, T, A>::char_type *basic_simple_string<C, T, A>::char_pointer_from_member_pointer_(ss_typename_type_k basic_simple_string<C, T, A>::member_pointer m)
{
#ifdef STLSOFT_SIMPLE_STRING_NO_PTR_ADJUST
    return (NULL == m) ? NULL : m->contents;
#else /* ? STLSOFT_SIMPLE_STRING_NO_PTR_ADJUST */
    return m;
#endif /* STLSOFT_SIMPLE_STRING_NO_PTR_ADJUST */
}


template<   ss_typename_param_k C
        ,   ss_typename_param_k T
        ,   ss_typename_param_k A
        >
inline /* static */ ss_typename_type_k basic_simple_string<C, T, A>::string_buffer *basic_simple_string<C, T, A>::string_buffer_from_member_pointer_(ss_typename_type_k basic_simple_string<C, T, A>::member_pointer m)
{
    stlsoft_message_assert("Attempt to convert a null string_buffer in basic_simple_string", NULL != m);

#ifdef STLSOFT_SIMPLE_STRING_NO_PTR_ADJUST
    return m;
#else /* ? STLSOFT_SIMPLE_STRING_NO_PTR_ADJUST */
    return reinterpret_cast<string_buffer*>(const_cast<void*>(ptr_byte_offset(m, -static_cast<ss_ptrdiff_t>(stlsoft_raw_offsetof(string_buffer, contents)))));
#endif /* STLSOFT_SIMPLE_STRING_NO_PTR_ADJUST */
}

template<   ss_typename_param_k C
        ,   ss_typename_param_k T
        ,   ss_typename_param_k A
        >
inline /* static */ ss_typename_type_k basic_simple_string<C, T, A>::string_buffer const *basic_simple_string<C, T, A>::string_buffer_from_member_pointer_(ss_typename_type_k basic_simple_string<C, T, A>::member_const_pointer m)
{
    stlsoft_message_assert("Attempt to convert a null string_buffer in basic_simple_string", NULL != m);

#ifdef STLSOFT_SIMPLE_STRING_NO_PTR_ADJUST
    return m;
#else /* ? STLSOFT_SIMPLE_STRING_NO_PTR_ADJUST */
    return reinterpret_cast<string_buffer const*>(ptr_byte_offset(m, -static_cast<ss_ptrdiff_t>(stlsoft_raw_offsetof(string_buffer, contents))));
#endif /* STLSOFT_SIMPLE_STRING_NO_PTR_ADJUST */
}

template<   ss_typename_param_k C
        ,   ss_typename_param_k T
        ,   ss_typename_param_k A
        >
inline /* static */ ss_typename_type_k basic_simple_string<C, T, A>::member_pointer basic_simple_string<C, T, A>::member_pointer_from_string_buffer_(ss_typename_type_k basic_simple_string<C, T, A>::string_buffer *b)
{
#ifdef STLSOFT_SIMPLE_STRING_NO_PTR_ADJUST
    return b;
#else /* ? STLSOFT_SIMPLE_STRING_NO_PTR_ADJUST */
    return b->contents;
#endif /* STLSOFT_SIMPLE_STRING_NO_PTR_ADJUST */
}

template<   ss_typename_param_k C
        ,   ss_typename_param_k T
        ,   ss_typename_param_k A
        >
inline /* static */ ss_typename_type_k basic_simple_string<C, T, A>::member_pointer basic_simple_string<C, T, A>::alloc_buffer_(ss_typename_type_k basic_simple_string<C, T, A>::char_type const    *s
                                                                                                                            ,   ss_typename_type_k basic_simple_string<C, T, A>::size_type          capacity
                                                                                                                            ,   ss_typename_type_k basic_simple_string<C, T, A>::size_type          length)
{
    // Pre-conditions
    stlsoft_assert(length <= capacity);
    stlsoft_assert(length >= traits_type::length_max_null(s, length));

    const ss_size_t members = (stlsoft_raw_offsetof(string_buffer, contents) + (sizeof(char_type) - 1)) / sizeof(char_type);

    capacity += 1;                                              // For null terminator
    capacity += members;                                        // Include the internal members.
    capacity = (alloc_quantum + capacity) & ~alloc_quantum;     // Round up to (alloc_quantum + 1)


#ifdef STLSOFT_CF_ALLOCATOR_REBIND_SUPPORT
# ifdef STLSOFT_CF_TEMPLATE_QUALIFIER_KEYWORD_SUPPORT
    ss_typename_type_k allocator_type::template rebind<ss_byte_t>::other    byte_ator;
# else /* ? STLSOFT_CF_TEMPLATE_QUALIFIER_KEYWORD_SUPPORT */
    ss_typename_type_k allocator_type::rebind<ss_byte_t>::other             byte_ator;
# endif /* STLSOFT_CF_TEMPLATE_QUALIFIER_KEYWORD_SUPPORT */
#else /* ? STLSOFT_CF_ALLOCATOR_REBIND_SUPPORT */
    new_allocator<ss_byte_t>                                                byte_ator;
#endif /* STLSOFT_CF_ALLOCATOR_REBIND_SUPPORT */

    ss_byte_t       *raw_buffer =   byte_ator.allocate(capacity * sizeof(char_type));
    string_buffer   *buffer     =   sap_cast<string_buffer*>(raw_buffer);

    if(NULL != buffer)
    {
        if(NULL == s)
        {
            stlsoft_assert(0 == length);

            buffer->contents[0] = traits_type::to_char_type(0);
        }
        else
        {
            traits_type::copy(buffer->contents, s, length);
            buffer->contents[length] = traits_type::to_char_type(0);
        }

        buffer->length      =   length;
        buffer->capacity    =   capacity - members;

        return member_pointer_from_string_buffer_(buffer);
    }

    return NULL;
}

template<   ss_typename_param_k C
        ,   ss_typename_param_k T
        ,   ss_typename_param_k A
        >
inline /* static */ ss_typename_type_k basic_simple_string<C, T, A>::member_pointer basic_simple_string<C, T, A>::alloc_buffer_(ss_typename_type_k basic_simple_string<C, T, A>::char_type const    *s
                                                                                                                            ,   ss_typename_type_k basic_simple_string<C, T, A>::size_type          cch)
{
    size_type   length      =   traits_type::length_max_null(s, cch);
    size_type   capacity    =   cch;

    if(cch < length)
    {
        length = cch;
    }

    return alloc_buffer_(s, capacity, length);
}

template<   ss_typename_param_k C
        ,   ss_typename_param_k T
        ,   ss_typename_param_k A
        >
inline /* static */ ss_typename_type_k basic_simple_string<C, T, A>::member_pointer basic_simple_string<C, T, A>::alloc_buffer_(ss_typename_type_k basic_simple_string<C, T, A>::char_type const *s)
{
    member_pointer  res;

    if(NULL == s)
    {
        res = NULL;
    }
    else
    {
        size_type   len =   traits_type::length(s);

        res = alloc_buffer_(s, len, len);
    }

    return res;
}

template<   ss_typename_param_k C
        ,   ss_typename_param_k T
        ,   ss_typename_param_k A
        >
inline /* static */ ss_typename_type_k basic_simple_string<C, T, A>::member_pointer basic_simple_string<C, T, A>::copy_buffer_(ss_typename_type_k basic_simple_string<C, T, A>::member_pointer m)
{
    if(NULL != m)
    {
#ifdef STLSOFT_CF_ALLOCATOR_REBIND_SUPPORT
# ifdef STLSOFT_CF_TEMPLATE_QUALIFIER_KEYWORD_SUPPORT
        ss_typename_type_k allocator_type::template rebind<ss_byte_t>::other    byte_ator;
# else /* ? STLSOFT_CF_TEMPLATE_QUALIFIER_KEYWORD_SUPPORT */
        ss_typename_type_k allocator_type::rebind<ss_byte_t>::other             byte_ator;
# endif /* STLSOFT_CF_TEMPLATE_QUALIFIER_KEYWORD_SUPPORT */
#else /* ? STLSOFT_CF_ALLOCATOR_REBIND_SUPPORT */
        new_allocator<ss_byte_t>                                                byte_ator;
#endif /* STLSOFT_CF_ALLOCATOR_REBIND_SUPPORT */

        string_buffer   *buffer     =   string_buffer_from_member_pointer_(m);
        ss_size_t       cb          =   buffer->capacity * sizeof(char_type) + stlsoft_raw_offsetof(string_buffer, contents);
        ss_byte_t       *raw_buffer =   byte_ator.allocate(cb);
        string_buffer   *new_buffer =   sap_cast<string_buffer*>(raw_buffer);

        if(NULL != new_buffer)
        {
            memcpy(new_buffer, buffer, cb);

            return member_pointer_from_string_buffer_(new_buffer);
        }
    }

    return NULL;
}

template<   ss_typename_param_k C
        ,   ss_typename_param_k T
        ,   ss_typename_param_k A
        >
inline /* static */ void basic_simple_string<C, T, A>::destroy_buffer_(ss_typename_type_k basic_simple_string<C, T, A>::string_buffer *buffer)
{
#ifdef STLSOFT_CF_ALLOCATOR_REBIND_SUPPORT
# ifdef STLSOFT_CF_TEMPLATE_QUALIFIER_KEYWORD_SUPPORT
    ss_typename_type_k allocator_type::template rebind<ss_byte_t>::other    byte_ator;
# else /* ? STLSOFT_CF_TEMPLATE_QUALIFIER_KEYWORD_SUPPORT */
    ss_typename_type_k allocator_type::rebind<ss_byte_t>::other             byte_ator;
# endif /* STLSOFT_CF_TEMPLATE_QUALIFIER_KEYWORD_SUPPORT */
#else /* ? STLSOFT_CF_ALLOCATOR_REBIND_SUPPORT */
    new_allocator<ss_byte_t>                                                byte_ator;
#endif /* STLSOFT_CF_ALLOCATOR_REBIND_SUPPORT */

#ifdef STLSOFT_CF_TEMPLATE_QUALIFIER_KEYWORD_SUPPORT
#else /* ? STLSOFT_CF_TEMPLATE_QUALIFIER_KEYWORD_SUPPORT */
#endif /* STLSOFT_CF_TEMPLATE_QUALIFIER_KEYWORD_SUPPORT */

    byte_ator.deallocate(sap_cast<ss_byte_t*>(buffer), 0);
}

template<   ss_typename_param_k C
        ,   ss_typename_param_k T
        ,   ss_typename_param_k A
        >
inline /* static */ void basic_simple_string<C, T, A>::destroy_buffer_(ss_typename_type_k basic_simple_string<C, T, A>::char_type *s)
{
    destroy_buffer_(string_buffer_from_member_pointer_(s));
}

template<   ss_typename_param_k C
        ,   ss_typename_param_k T
        ,   ss_typename_param_k A
        >
inline ss_typename_type_k basic_simple_string<C, T, A>::pointer basic_simple_string<C, T, A>::begin_()
{
    return char_pointer_from_member_pointer_(m_buffer);
}

template<   ss_typename_param_k C
        ,   ss_typename_param_k T
        ,   ss_typename_param_k A
        >
inline ss_typename_type_k basic_simple_string<C, T, A>::pointer basic_simple_string<C, T, A>::end_()
{
    return begin_() + length();
}

template<   ss_typename_param_k C
        ,   ss_typename_param_k T
        ,   ss_typename_param_k A
        >
inline ss_bool_t basic_simple_string<C, T, A>::is_valid_() const
{
    if(NULL != m_buffer)
    {
        string_buffer const *buffer =   string_buffer_from_member_pointer_(m_buffer);

        if(buffer->capacity < 1)
        {
            return false;
        }
        else if(buffer->capacity < buffer->length)
        {
            return false;
        }
        else
        {
            size_type   len =   traits_type::length(buffer->contents);

            if(buffer->length != len)
            {
                return false;
            }
        }
    }

    return true;
}

template<   ss_typename_param_k C
        ,   ss_typename_param_k T
        ,   ss_typename_param_k A
        >
inline /* static */ ss_typename_type_k basic_simple_string<C, T, A>::char_type const *basic_simple_string<C, T, A>::empty_string_()
{
    // This character array is initialised to 0, which conveniently happens to
    // be the empty string, by the module/application load, so it is
    // guaranteed to be valid, and there are no threading/race conditions
    static char_type    s_empty[1];

    stlsoft_assert(s_empty[0] == '\0'); // Paranoid check

    return s_empty;
}

// Construction

template<   ss_typename_param_k C
        ,   ss_typename_param_k T
        ,   ss_typename_param_k A
        >
inline basic_simple_string<C, T, A>::basic_simple_string()
    : m_buffer(NULL)
{
    stlsoft_assert(is_valid_());
}

template<   ss_typename_param_k C
        ,   ss_typename_param_k T
        ,   ss_typename_param_k A
        >
inline basic_simple_string<C, T, A>::basic_simple_string(class_type const &rhs)
    : m_buffer(copy_buffer_(rhs.m_buffer))
{
    stlsoft_assert(is_valid_());
}

template<   ss_typename_param_k C
        ,   ss_typename_param_k T
        ,   ss_typename_param_k A
        >
inline basic_simple_string<C, T, A>::basic_simple_string(   ss_typename_type_k basic_simple_string<C, T, A>::class_type const   &rhs
                                                        ,   ss_typename_type_k basic_simple_string<C, T, A>::size_type          pos)
    : m_buffer(alloc_buffer_(&rhs[pos]))
{
    stlsoft_assert(is_valid_());
}

template<   ss_typename_param_k C
        ,   ss_typename_param_k T
        ,   ss_typename_param_k A
        >
inline basic_simple_string<C, T, A>::basic_simple_string(   ss_typename_type_k basic_simple_string<C, T, A>::class_type const   &rhs
                                                        ,   ss_typename_type_k basic_simple_string<C, T, A>::size_type          pos
                                                        ,   ss_typename_type_k basic_simple_string<C, T, A>::size_type          cch)
    : m_buffer(alloc_buffer_(&rhs[pos], cch, cch))
{
    stlsoft_assert(is_valid_());
}

template<   ss_typename_param_k C
        ,   ss_typename_param_k T
        ,   ss_typename_param_k A
        >
inline basic_simple_string<C, T, A>::basic_simple_string(ss_typename_type_k basic_simple_string<C, T, A>::char_type const *s) // No, not explicit. Sigh
    : m_buffer(alloc_buffer_(s))
{
    stlsoft_assert(is_valid_());
}

template<   ss_typename_param_k C
        ,   ss_typename_param_k T
        ,   ss_typename_param_k A
        >
inline basic_simple_string<C, T, A>::basic_simple_string(   ss_typename_type_k basic_simple_string<C, T, A>::char_type const    *s
                                                        ,   ss_typename_type_k basic_simple_string<C, T, A>::size_type          cch)
    : m_buffer(alloc_buffer_(s, cch))
{
    stlsoft_assert(is_valid_());
}

template<   ss_typename_param_k C
        ,   ss_typename_param_k T
        ,   ss_typename_param_k A
        >
inline basic_simple_string<C, T, A>::basic_simple_string(   ss_typename_type_k basic_simple_string<C, T, A>::size_type  cch
                                                        ,   ss_typename_type_k basic_simple_string<C, T, A>::char_type  ch)
    : m_buffer(NULL)
{
    stlsoft_assert(is_valid_());

    assign(cch, ch);
}

#if !defined(__STLSOFT_CF_MEMBER_TEMPLATE_RANGE_METHOD_SUPPORT)
template<   ss_typename_param_k C
        ,   ss_typename_param_k T
        ,   ss_typename_param_k A
        >
inline basic_simple_string<C, T, A>::basic_simple_string(   ss_typename_type_k basic_simple_string<C, T, A>::char_type const    *first
                                                        ,   ss_typename_type_k basic_simple_string<C, T, A>::char_type const    *last)
    : m_buffer(alloc_buffer_(first, last - first))
{
    stlsoft_assert(is_valid_());
}
#endif /* !__STLSOFT_CF_MEMBER_TEMPLATE_RANGE_METHOD_SUPPORT */

template<   ss_typename_param_k C
        ,   ss_typename_param_k T
        ,   ss_typename_param_k A
        >
inline basic_simple_string<C, T, A>::~basic_simple_string()
{
    stlsoft_assert(is_valid_());

    if(NULL != m_buffer)
    {
        destroy_buffer_(m_buffer);
    }
}

// Comparison

template<   ss_typename_param_k C
        ,   ss_typename_param_k T
        ,   ss_typename_param_k A
        >
inline /* static */ ss_sint_t basic_simple_string<C, T, A>::compare_(   ss_typename_type_k basic_simple_string<C, T, A>::value_type const   *lhs
                                                                    ,   ss_typename_type_k basic_simple_string<C, T, A>::size_type          lhs_len
                                                                    ,   ss_typename_type_k basic_simple_string<C, T, A>::value_type const   *rhs
                                                                    ,   ss_typename_type_k basic_simple_string<C, T, A>::size_type          rhs_len)
{
    size_type   cmp_len =   (lhs_len < rhs_len) ? lhs_len : rhs_len;
    ss_int_t    result  =   traits_type::compare(lhs, rhs, cmp_len);

    if(0 == result)
    {
        result = static_cast<ss_int_t>(lhs_len) - static_cast<ss_int_t>(rhs_len);
    }

    return result;
}

template<   ss_typename_param_k C
        ,   ss_typename_param_k T
        ,   ss_typename_param_k A
        >
inline ss_sint_t basic_simple_string<C, T, A>::compare( ss_typename_type_k basic_simple_string<C, T, A>::size_type          pos
                                                    ,   ss_typename_type_k basic_simple_string<C, T, A>::size_type          cch
                                                    ,   ss_typename_type_k basic_simple_string<C, T, A>::value_type const   *rhs
                                                    ,   ss_typename_type_k basic_simple_string<C, T, A>::size_type          cchRhs) const
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

    size_type   rhs_len =   (NULL == rhs) ? 0 : traits_type::length(rhs);

    if(cchRhs < rhs_len)
    {
        rhs_len = cchRhs;
    }

    return compare_(char_pointer_from_member_pointer_(m_buffer) + pos, lhs_len, rhs, rhs_len);
}

template<   ss_typename_param_k C
        ,   ss_typename_param_k T
        ,   ss_typename_param_k A
        >
inline ss_sint_t basic_simple_string<C, T, A>::compare( ss_typename_type_k basic_simple_string<C, T, A>::size_type          pos
                                                    ,   ss_typename_type_k basic_simple_string<C, T, A>::size_type          cch
                                                    ,   ss_typename_type_k basic_simple_string<C, T, A>::value_type const   *rhs) const
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

    size_type   rhs_len =   (NULL == rhs) ? 0 : traits_type::length(rhs);

    return compare_(char_pointer_from_member_pointer_(m_buffer) + pos, lhs_len, rhs, rhs_len);
}

template<   ss_typename_param_k C
        ,   ss_typename_param_k T
        ,   ss_typename_param_k A
        >
inline ss_sint_t basic_simple_string<C, T, A>::compare(ss_typename_type_k basic_simple_string<C, T, A>::value_type const *rhs) const
{
    size_type   lhs_len =   length();
    size_type   rhs_len =   (NULL == rhs) ? 0 : traits_type::length(rhs);

    return compare_(char_pointer_from_member_pointer_(m_buffer), lhs_len, rhs, rhs_len);
}

template<   ss_typename_param_k C
        ,   ss_typename_param_k T
        ,   ss_typename_param_k A
        >
inline ss_sint_t basic_simple_string<C, T, A>::compare( ss_typename_type_k basic_simple_string<C, T, A>::size_type          pos
                                                    ,   ss_typename_type_k basic_simple_string<C, T, A>::size_type          cch
                                                    ,   ss_typename_type_k basic_simple_string<C, T, A>::class_type const   &rhs
                                                    ,   ss_typename_type_k basic_simple_string<C, T, A>::size_type          posRhs
                                                    ,   ss_typename_type_k basic_simple_string<C, T, A>::size_type          cchRhs) const
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

    return compare_(char_pointer_from_member_pointer_(m_buffer) + pos, lhs_len, char_pointer_from_member_pointer_(rhs.m_buffer) + posRhs, rhs_len);
}

template<   ss_typename_param_k C
        ,   ss_typename_param_k T
        ,   ss_typename_param_k A
        >
inline ss_sint_t basic_simple_string<C, T, A>::compare( ss_typename_type_k basic_simple_string<C, T, A>::size_type          pos
                                                    ,   ss_typename_type_k basic_simple_string<C, T, A>::size_type          cch
                                                    ,   ss_typename_type_k basic_simple_string<C, T, A>::class_type const   &rhs) const
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

    return compare_(char_pointer_from_member_pointer_(m_buffer) + pos, lhs_len, char_pointer_from_member_pointer_(rhs.m_buffer), rhs_len);
}

template<   ss_typename_param_k C
        ,   ss_typename_param_k T
        ,   ss_typename_param_k A
        >
inline ss_sint_t basic_simple_string<C, T, A>::compare(ss_typename_type_k basic_simple_string<C, T, A>::class_type const &rhs) const
{
    size_type   lhs_len =   length();
    size_type   rhs_len =   rhs.length();

    return compare_(char_pointer_from_member_pointer_(m_buffer), lhs_len, char_pointer_from_member_pointer_(rhs.m_buffer), rhs_len);
}

// Accessors
template<   ss_typename_param_k C
        ,   ss_typename_param_k T
        ,   ss_typename_param_k A
        >
inline ss_typename_type_k basic_simple_string<C, T, A>::reference basic_simple_string<C, T, A>::operator [](ss_typename_type_k basic_simple_string<C, T, A>::size_type index)
{
    stlsoft_message_assert("index access out of range in frame_string", index < length() + 1);   // Has to be +1, since legitimate to take address of one-past-the-end

    stlsoft_assert(is_valid_());

    return char_pointer_from_member_pointer_(m_buffer)[index];
}

template<   ss_typename_param_k C
        ,   ss_typename_param_k T
        ,   ss_typename_param_k A
        >
inline ss_typename_type_k basic_simple_string<C, T, A>::const_reference basic_simple_string<C, T, A>::operator [](ss_typename_type_k basic_simple_string<C, T, A>::size_type index) const
{
    stlsoft_message_assert("index access out of range in frame_string", index < length() + 1);   // Has to be +1, since legitimate to take address of one-past-the-end

    stlsoft_assert(is_valid_());

    return char_pointer_from_member_pointer_(m_buffer)[index];
}


template<   ss_typename_param_k C
        ,   ss_typename_param_k T
        ,   ss_typename_param_k A
        >
inline ss_typename_type_k basic_simple_string<C, T, A>::value_type const *basic_simple_string<C, T, A>::c_str() const
{
    return (NULL == m_buffer) ? empty_string_() : char_pointer_from_member_pointer_(m_buffer);
}

template<   ss_typename_param_k C
        ,   ss_typename_param_k T
        ,   ss_typename_param_k A
        >
inline ss_typename_type_k basic_simple_string<C, T, A>::value_type const *basic_simple_string<C, T, A>::data() const
{
    return (NULL == m_buffer) ? empty_string_() : char_pointer_from_member_pointer_(m_buffer);
}

template<   ss_typename_param_k C
        ,   ss_typename_param_k T
        ,   ss_typename_param_k A
        >
inline ss_typename_type_k basic_simple_string<C, T, A>::reference basic_simple_string<C, T, A>::front()
{
    return (*this)[0];
}

template<   ss_typename_param_k C
        ,   ss_typename_param_k T
        ,   ss_typename_param_k A
        >
inline ss_typename_type_k basic_simple_string<C, T, A>::reference basic_simple_string<C, T, A>::back()
{
    return (*this)[length() - 1];
}

template<   ss_typename_param_k C
        ,   ss_typename_param_k T
        ,   ss_typename_param_k A
        >
inline ss_typename_type_k basic_simple_string<C, T, A>::const_reference basic_simple_string<C, T, A>::front() const
{
    return (*this)[0];
}

template<   ss_typename_param_k C
        ,   ss_typename_param_k T
        ,   ss_typename_param_k A
        >
inline ss_typename_type_k basic_simple_string<C, T, A>::const_reference basic_simple_string<C, T, A>::back() const
{
    return (*this)[length() - 1];
}

template<   ss_typename_param_k C
        ,   ss_typename_param_k T
        ,   ss_typename_param_k A
        >
inline ss_typename_type_k basic_simple_string<C, T, A>::size_type basic_simple_string<C, T, A>::copy(   ss_typename_type_k basic_simple_string<C, T, A>::value_type     *dest
                                                                                                    ,   ss_typename_type_k basic_simple_string<C, T, A>::size_type      cch
                                                                                                    ,   ss_typename_type_k basic_simple_string<C, T, A>::size_type      pos /* = 0 */) const
{
    size_type   len =   length();

    if(pos < len)
    {
        if(len < pos + cch)
        {
            cch = len - pos;
        }

        traits_type::copy(dest, data() + pos, cch);
    }
    else
    {
        cch = 0;
    }

    return cch;
}

// Iteration

#ifndef STLSOFT_SIMPLE_STRING_ITERATOR_METHODS_INLINE
template<   ss_typename_param_k C
        ,   ss_typename_param_k T
        ,   ss_typename_param_k A
        >
inline ss_typename_type_k basic_simple_string<C, T, A>::const_iterator basic_simple_string<C, T, A>::begin() const
{
    return const_cast<class_type*>(this)->begin_();
}

template<   ss_typename_param_k C
        ,   ss_typename_param_k T
        ,   ss_typename_param_k A
        >
inline ss_typename_type_k basic_simple_string<C, T, A>::const_iterator basic_simple_string<C, T, A>::end() const
{
    return const_cast<class_type*>(this)->end_();
}
#endif /* !STLSOFT_SIMPLE_STRING_ITERATOR_METHODS_INLINE */

template<   ss_typename_param_k C
        ,   ss_typename_param_k T
        ,   ss_typename_param_k A
        >
inline ss_typename_type_k basic_simple_string<C, T, A>::iterator basic_simple_string<C, T, A>::begin()
{
    return begin_();
}

template<   ss_typename_param_k C
        ,   ss_typename_param_k T
        ,   ss_typename_param_k A
        >
inline ss_typename_type_k basic_simple_string<C, T, A>::iterator basic_simple_string<C, T, A>::end()
{
    return end_();
}

#if defined(__STLSOFT_CF_BIDIRECTIONAL_ITERATOR_SUPPORT)
template<   ss_typename_param_k C
        ,   ss_typename_param_k T
        ,   ss_typename_param_k A
        >
inline ss_typename_type_k basic_simple_string<C, T, A>::const_reverse_iterator basic_simple_string<C, T, A>::rbegin() const
{
    return const_reverse_iterator(end());
}

template<   ss_typename_param_k C
        ,   ss_typename_param_k T
        ,   ss_typename_param_k A
        >
inline ss_typename_type_k basic_simple_string<C, T, A>::const_reverse_iterator basic_simple_string<C, T, A>::rend() const
{
    return const_reverse_iterator(begin());
}

template<   ss_typename_param_k C
        ,   ss_typename_param_k T
        ,   ss_typename_param_k A
        >
inline ss_typename_type_k basic_simple_string<C, T, A>::reverse_iterator basic_simple_string<C, T, A>::rbegin()
{
    return reverse_iterator(end());
}

template<   ss_typename_param_k C
        ,   ss_typename_param_k T
        ,   ss_typename_param_k A
        >
inline ss_typename_type_k basic_simple_string<C, T, A>::reverse_iterator basic_simple_string<C, T, A>::rend()
{
    return reverse_iterator(begin());
}
#endif /* __STLSOFT_CF_BIDIRECTIONAL_ITERATOR_SUPPORT */

// Assignment
template<   ss_typename_param_k C
        ,   ss_typename_param_k T
        ,   ss_typename_param_k A
        >
inline ss_typename_type_k basic_simple_string<C, T, A>::class_type &basic_simple_string<C, T, A>::assign(   ss_typename_type_k basic_simple_string<C, T, A>::char_type const    *s
                                                                                                        ,   ss_typename_type_k basic_simple_string<C, T, A>::size_type          cch)
{
    stlsoft_assert(is_valid_());

    if(NULL == m_buffer)
    {
        if(cch == 0)
        {
            // Nothing to do
        }
        else
        {
            m_buffer = alloc_buffer_(s, cch);
        }
    }
    else
    {
        if(NULL == s)
        {
            destroy_buffer_(m_buffer);
            m_buffer = NULL;
        }
        else
        {
            // Here is an opportunity to optimise a bit. We will do more so in a
            // later release, but for the moment we will reuse our existing
            // buffer if its capacity is sufficient for our purposes

            string_buffer   *buffer =   string_buffer_from_member_pointer_(m_buffer);
            size_type       len     =   traits_type::length(s);

            if(len < cch)
            {
                cch = len;
            }

            if( cch < buffer->capacity &&
                (   s < &buffer->contents[0] ||
                    s > &buffer->contents[len]))
            {
                traits_type::copy(buffer->contents, s, cch);
                buffer->contents[cch] = 0;
                buffer->length = cch;
            }
            else
            {
                member_pointer  new_buffer =   alloc_buffer_(s, cch, cch);

                destroy_buffer_(m_buffer);
                m_buffer = new_buffer;
            }
        }
    }

    stlsoft_assert(is_valid_());
    return *this;
}

template<   ss_typename_param_k C
        ,   ss_typename_param_k T
        ,   ss_typename_param_k A
        >
inline ss_typename_type_k basic_simple_string<C, T, A>::class_type &basic_simple_string<C, T, A>::assign(ss_typename_type_k basic_simple_string<C, T, A>::char_type const *s)
{
    return assign(s, (NULL == s) ? 0 : traits_type::length(s));
}

template<   ss_typename_param_k C
        ,   ss_typename_param_k T
        ,   ss_typename_param_k A
        >
inline ss_typename_type_k basic_simple_string<C, T, A>::class_type &basic_simple_string<C, T, A>::assign(   ss_typename_type_k basic_simple_string<C, T, A>::class_type const   &rhs
                                                                                                        ,   ss_typename_type_k basic_simple_string<C, T, A>::size_type          pos
                                                                                                        ,   ss_typename_type_k basic_simple_string<C, T, A>::size_type          cch)
{
    char_type   *s  =   char_pointer_from_member_pointer_(rhs.m_buffer);
    size_type   len =   rhs.length();

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

    return assign(s, cch);
}

template<   ss_typename_param_k C
        ,   ss_typename_param_k T
        ,   ss_typename_param_k A
        >
inline ss_typename_type_k basic_simple_string<C, T, A>::class_type &basic_simple_string<C, T, A>::assign(ss_typename_type_k basic_simple_string<C, T, A>::class_type const &rhs)
{
    return assign(char_pointer_from_member_pointer_(rhs.m_buffer), rhs.length());
}

template<   ss_typename_param_k C
        ,   ss_typename_param_k T
        ,   ss_typename_param_k A
        >
inline ss_typename_type_k basic_simple_string<C, T, A>::class_type &basic_simple_string<C, T, A>::assign(   ss_typename_type_k basic_simple_string<C, T, A>::size_type  cch
                                                                                                        ,   ss_typename_type_k basic_simple_string<C, T, A>::char_type  ch)
{
    typedef auto_buffer<char_type, allocator_type>  buffer_t;

    buffer_t    buffer(cch);

    (void)stlsoft_ns_qual_std(fill)(buffer.begin(), buffer.end(), ch);

    return assign(buffer, buffer.size());
}

#if !defined(__STLSOFT_CF_MEMBER_TEMPLATE_RANGE_METHOD_SUPPORT)
template<   ss_typename_param_k C
        ,   ss_typename_param_k T
        ,   ss_typename_param_k A
        >
inline ss_typename_type_k basic_simple_string<C, T, A>::class_type &basic_simple_string<C, T, A>::assign(   ss_typename_type_k basic_simple_string<C, T, A>::const_iterator     first
                                                                                                        ,   ss_typename_type_k basic_simple_string<C, T, A>::const_iterator     last)
{
    // We have to use this strange appearing this, because of Visual C++ .NET's
    // disgusting STL swill. Sigh!
    return assign(&(*first), last - first);
}
#endif /* !__STLSOFT_CF_MEMBER_TEMPLATE_RANGE_METHOD_SUPPORT */

template<   ss_typename_param_k C
        ,   ss_typename_param_k T
        ,   ss_typename_param_k A
        >
inline ss_typename_type_k basic_simple_string<C, T, A>::class_type const &basic_simple_string<C, T, A>::operator =(ss_typename_type_k basic_simple_string<C, T, A>::class_type const &rhs)
{
    return assign(rhs);
}

template<   ss_typename_param_k C
        ,   ss_typename_param_k T
        ,   ss_typename_param_k A
        >
inline ss_typename_type_k basic_simple_string<C, T, A>::class_type const &basic_simple_string<C, T, A>::operator =(ss_typename_type_k basic_simple_string<C, T, A>::char_type const *s)
{
    return assign(s);
}

template<   ss_typename_param_k C
        ,   ss_typename_param_k T
        ,   ss_typename_param_k A
        >
inline ss_typename_type_k basic_simple_string<C, T, A>::class_type const &basic_simple_string<C, T, A>::operator =(ss_typename_type_k basic_simple_string<C, T, A>::char_type ch)
{
    char_type   sz[2] = { ch, traits_type::to_char_type(0) };

    return assign(sz);
}


// Appending
template<   ss_typename_param_k C
        ,   ss_typename_param_k T
        ,   ss_typename_param_k A
        >
inline ss_typename_type_k basic_simple_string<C, T, A>::class_type &basic_simple_string<C, T, A>::append(   ss_typename_type_k basic_simple_string<C, T, A>::char_type const    *s
                                                                                                        ,   ss_typename_type_k basic_simple_string<C, T, A>::size_type          cch)
{
    stlsoft_assert(is_valid_());

    if(NULL == m_buffer)
    {
        assign(s, cch);
    }
    else
    {
        if( NULL == s ||
            0 == cch)
        {
            // Nothing to do
        }
        else
        {
#if 1
            // We're taking a length here, which may have been done already.
            // This should be optimised out in a subsequent release
            size_type len = traits_type::length_max(s, cch);

            if(len < cch)
            {
                cch = len;
            }
#endif /* 0 */

            string_buffer   *old_buffer =   NULL;
            string_buffer   *buffer     =   string_buffer_from_member_pointer_(m_buffer);
            size_type       buf_len     =   buffer->length;

            if(buffer->capacity - buf_len < 1 + cch)
            {
                // Allocate a new buffer of sufficient size
                member_pointer  new_buffer =   alloc_buffer_(buffer->contents, buf_len + cch);

                if(NULL == new_buffer) // Some allocators do not throw on failure!
                {
                    cch = 0;
                }
                else
                {
                    old_buffer = buffer;    // Mark for destruction, but hold around in case appending from self
                    m_buffer = new_buffer;
                    buffer = string_buffer_from_member_pointer_(new_buffer);
                }
            }

            traits_type::copy(buffer->contents + buf_len, s, cch);
            buffer->length += cch;
            buffer->contents[buffer->length] = traits_type::to_char_type(0);

            if(NULL != old_buffer)
            {
                destroy_buffer_(old_buffer);
            }
        }
    }

    stlsoft_assert(is_valid_());
    return *this;
}

template<   ss_typename_param_k C
        ,   ss_typename_param_k T
        ,   ss_typename_param_k A
        >
inline ss_typename_type_k basic_simple_string<C, T, A>::class_type &basic_simple_string<C, T, A>::append(ss_typename_type_k basic_simple_string<C, T, A>::char_type const *s)
{
    return append(s, (NULL == s) ? 0 : traits_type::length(s));
}

template<   ss_typename_param_k C
        ,   ss_typename_param_k T
        ,   ss_typename_param_k A
        >
inline ss_typename_type_k basic_simple_string<C, T, A>::class_type &basic_simple_string<C, T, A>::append(   ss_typename_type_k basic_simple_string<C, T, A>::class_type const   &rhs
                                                                                                        ,   ss_typename_type_k basic_simple_string<C, T, A>::size_type          pos
                                                                                                        ,   ss_typename_type_k basic_simple_string<C, T, A>::size_type          cch)
{
    char_type   *s  =   char_pointer_from_member_pointer_(rhs.m_buffer);
    size_type   len =   rhs.length();

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
        ,   ss_typename_param_k T
        ,   ss_typename_param_k A
        >
inline ss_typename_type_k basic_simple_string<C, T, A>::class_type &basic_simple_string<C, T, A>::append(ss_typename_type_k basic_simple_string<C, T, A>::class_type const &s)
{
    return append(char_pointer_from_member_pointer_(s.m_buffer), s.length());
}

template<   ss_typename_param_k C
        ,   ss_typename_param_k T
        ,   ss_typename_param_k A
        >
inline ss_typename_type_k basic_simple_string<C, T, A>::class_type &basic_simple_string<C, T, A>::append(   ss_typename_type_k basic_simple_string<C, T, A>::size_type  cch
                                                                                                        ,   ss_typename_type_k basic_simple_string<C, T, A>::char_type  ch)
{
    if(NULL == m_buffer)
    {
        assign(cch, ch);
    }
    else
    {
        typedef auto_buffer<char_type, allocator_type>  buffer_t;

        buffer_t    buffer(cch);

        (void)stlsoft_ns_qual_std(fill)(buffer.begin(), buffer.end(), ch);

        append(buffer, buffer.size());
    }

    return *this;
}

#if !defined(__STLSOFT_CF_MEMBER_TEMPLATE_RANGE_METHOD_SUPPORT)
template<   ss_typename_param_k C
        ,   ss_typename_param_k T
        ,   ss_typename_param_k A
        >
inline ss_typename_type_k basic_simple_string<C, T, A>::class_type &basic_simple_string<C, T, A>::append(   ss_typename_type_k basic_simple_string<C, T, A>::const_iterator first
                                                                                                        ,   ss_typename_type_k basic_simple_string<C, T, A>::const_iterator last)
{
    // We have to use this strange appearing this, because of Visual C++ .NET's
    // disgusting STL swill. Sigh!
    return append(&(*first), last - first);
}
#endif /* !__STLSOFT_CF_MEMBER_TEMPLATE_RANGE_METHOD_SUPPORT */

template<   ss_typename_param_k C
        ,   ss_typename_param_k T
        ,   ss_typename_param_k A
        >
inline ss_typename_type_k basic_simple_string<C, T, A>::class_type &basic_simple_string<C, T, A>::operator +=(ss_typename_type_k basic_simple_string<C, T, A>::char_type ch)
{
    return append(1, ch);
}

template<   ss_typename_param_k C
        ,   ss_typename_param_k T
        ,   ss_typename_param_k A
        >
inline ss_typename_type_k basic_simple_string<C, T, A>::class_type &basic_simple_string<C, T, A>::operator +=(const ss_typename_type_k basic_simple_string<C, T, A>::char_type *s)
{
    return append(s);
}

template<   ss_typename_param_k C
        ,   ss_typename_param_k T
        ,   ss_typename_param_k A
        >
inline ss_typename_type_k basic_simple_string<C, T, A>::class_type &basic_simple_string<C, T, A>::operator +=(const ss_typename_type_k basic_simple_string<C, T, A>::class_type &rhs)
{
    return append(rhs);
}

template<   ss_typename_param_k C
        ,   ss_typename_param_k T
        ,   ss_typename_param_k A
        >
inline void basic_simple_string<C, T, A>::push_back(ss_typename_type_k basic_simple_string<C, T, A>::char_type ch)
{
    append(1, ch);
}

// Operations
template<   ss_typename_param_k C
        ,   ss_typename_param_k T
        ,   ss_typename_param_k A
        >
inline void basic_simple_string<C, T, A>::reserve(ss_typename_type_k basic_simple_string<C, T, A>::size_type cch)
{
    if(length() < cch)
    {
        if(NULL == m_buffer)
        {
            m_buffer = alloc_buffer_(NULL, cch, 0);
        }
        else
        {
            // Allocate a new buffer of sufficient size
            member_pointer  new_buffer =   alloc_buffer_(c_str(), cch, length());

            if(NULL != new_buffer) // Some allocators do not throw on failure!
            {
                destroy_buffer_(m_buffer);
                m_buffer = new_buffer;
            }
        }
    }
}

template<   ss_typename_param_k C
        ,   ss_typename_param_k T
        ,   ss_typename_param_k A
        >
inline void basic_simple_string<C, T, A>::swap(ss_typename_type_k basic_simple_string<C, T, A>::class_type &other)
{
    stlsoft_assert(is_valid_());
    stlsoft_assert(other.is_valid_());

    member_pointer  buffer          =   other.m_buffer;
                    other.m_buffer  =   m_buffer;
                    m_buffer        =   buffer;
}

template<   ss_typename_param_k C
        ,   ss_typename_param_k T
        ,   ss_typename_param_k A
        >
inline void basic_simple_string<C, T, A>::resize(   ss_typename_type_k basic_simple_string<C, T, A>::size_type  cch
                                                ,   ss_typename_type_k basic_simple_string<C, T, A>::value_type ch)
{
    stlsoft_assert(is_valid_());

    size_type const len =   length();

    if(len != cch)
    {
        if(len < cch)
        {
            /* Expand the string, using self-assignemt. */
            assign(c_str(), cch);

            traits_type::assign(char_pointer_from_member_pointer_(m_buffer) + len, cch - len, ch);
        }

        string_buffer   *buffer     =   string_buffer_from_member_pointer_(m_buffer);

        buffer->length = cch;
        buffer->contents[buffer->length] = traits_type::to_char_type(0);
    }

    stlsoft_assert(is_valid_());
}

template<   ss_typename_param_k C
        ,   ss_typename_param_k T
        ,   ss_typename_param_k A
        >
inline void basic_simple_string<C, T, A>::clear()
{
    if(NULL != m_buffer)
    {
        string_buffer   *buffer =   string_buffer_from_member_pointer_(m_buffer);

        buffer->length      =   0;
        buffer->contents[0] =   traits_type::to_char_type(0);
    }
}

// Attributes
template<   ss_typename_param_k C
        ,   ss_typename_param_k T
        ,   ss_typename_param_k A
        >
inline ss_typename_type_k basic_simple_string<C, T, A>::size_type basic_simple_string<C, T, A>::size() const
{
    stlsoft_assert(is_valid_());

    return (NULL == m_buffer) ? 0 : string_buffer_from_member_pointer_(m_buffer)->length;
}

template<   ss_typename_param_k C
        ,   ss_typename_param_k T
        ,   ss_typename_param_k A
        >
inline ss_typename_type_k basic_simple_string<C, T, A>::size_type basic_simple_string<C, T, A>::max_size() const
{
    stlsoft_assert(is_valid_());

    return static_cast<size_type>(-1) / sizeof(char_type);
}

template<   ss_typename_param_k C
        ,   ss_typename_param_k T
        ,   ss_typename_param_k A
        >
inline ss_typename_type_k basic_simple_string<C, T, A>::size_type basic_simple_string<C, T, A>::length() const
{
    stlsoft_assert(is_valid_());

    return size();
}

template<   ss_typename_param_k C
        ,   ss_typename_param_k T
        ,   ss_typename_param_k A
        >
inline ss_typename_type_k basic_simple_string<C, T, A>::size_type basic_simple_string<C, T, A>::capacity() const
{
    stlsoft_assert(is_valid_());

    return (NULL == m_buffer) ? 0 : string_buffer_from_member_pointer_(m_buffer)->capacity;
}

template<   ss_typename_param_k C
        ,   ss_typename_param_k T
        ,   ss_typename_param_k A
        >
inline ss_bool_t basic_simple_string<C, T, A>::empty() const
{
    stlsoft_assert(is_valid_());

    return 0 == length();
}

#endif /* !__STLSOFT_DOCUMENTATION_SKIP_SECTION */

/* ////////////////////////////////////////////////////////////////////////// */

/// @} // end of group stlsoft_string_library

/* ////////////////////////////////////////////////////////////////////////// */

#ifndef _STLSOFT_NO_NAMESPACE
} // namespace stlsoft
#endif /* _STLSOFT_NO_NAMESPACE */

/* ////////////////////////////////////////////////////////////////////////// */

#endif /* !STLSOFT_INCL_H_STLSOFT_SIMPLE_STRING */

/* ////////////////////////////////////////////////////////////////////////// */
