/* /////////////////////////////////////////////////////////////////////////////
 * File:        stlsoft_fixed_substring.h
 *
 * Purpose:     basic_fixed_substring class.
 *
 * Created:     1st May 2003
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


/// \file stlsoft_fixed_substring.h
///
/// basic_fixed_substring class.

#ifndef STLSOFT_INCL_H_STLSOFT_FIXED_SUBSTRING
#define STLSOFT_INCL_H_STLSOFT_FIXED_SUBSTRING

#ifndef __STLSOFT_DOCUMENTATION_SKIP_SECTION
# define STLSOFT_VER_H_STLSOFT_FIXED_SUBSTRING_MAJOR        2
# define STLSOFT_VER_H_STLSOFT_FIXED_SUBSTRING_MINOR        0
# define STLSOFT_VER_H_STLSOFT_FIXED_SUBSTRING_REVISION     1
# define STLSOFT_VER_H_STLSOFT_FIXED_SUBSTRING_EDIT         31
#endif /* !__STLSOFT_DOCUMENTATION_SKIP_SECTION */

/* /////////////////////////////////////////////////////////////////////////////
 * Compatibility
 */

/*
[Incompatibilies-start]
__STLSOFT_COMPILER_IS_MWERKS: __MWERKS__<0x3000
[Incompatibilies-end]
 */

/* /////////////////////////////////////////////////////////////////////////////
 * Includes
 */

#ifndef STLSOFT_INCL_H_STLSOFT
# include "stlsoft.h"                   // Include the STLSoft root header
#endif /* !STLSOFT_INCL_H_STLSOFT */

#if defined(__STLSOFT_COMPILER_IS_MWERKS) && \
    __MWERKS__ < 0x3000
# error stlsoft_fixed_substring.h is not compatible with Metrowerks CodeWarrior 8.0 or earlier
#endif /* _MSC_VER < 1200 */

#ifndef STLSOFT_INCL_H_STLSOFT_AUTO_BUFFER
# include "stlsoft_auto_buffer.h"       // stlsoft::auto_buffer
#endif /* !STLSOFT_INCL_H_STLSOFT_AUTO_BUFFER */
#ifndef STLSOFT_INCL_H_STLSOFT_NEW_ALLOCATOR
# include "stlsoft_new_allocator.h"     // stlsoft::new_allocator
#endif /* !STLSOFT_INCL_H_STLSOFT_NEW_ALLOCATOR */
#ifndef STLSOFT_INCL_H_STLSOFT_CHAR_TRAITS
# include "stlsoft_char_traits.h"      // stlsoft::char_traits
#endif /* !STLSOFT_INCL_H_STLSOFT_CHAR_TRAITS */
#ifndef STLSOFT_INCL_H_STLSOFT_ITERATOR
# include "stlsoft_iterator.h"          // stlsoft iterator bases
#endif /* !STLSOFT_INCL_H_STLSOFT_ITERATOR */

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

template<   ss_typename_param_k C
        ,   ss_typename_param_k T
        ,   ss_typename_param_k A
        >
class basic_fixed_substring;


template<   ss_typename_param_k C
        ,   ss_typename_param_k T
        ,   ss_typename_param_k A
        >
class basic_fixed_substring_c_str
{
public:
    /// The value type
    typedef C                                       char_type;
    /// The traits type
    typedef T                                       traits_type;
    /// The allocator type
    typedef A                                       allocator_type;
    /// The current parameterisation of the type
    typedef basic_fixed_substring_c_str<C, T, A>    class_type;

private:
    basic_fixed_substring_c_str(char_type const *first, char_type const *last);

    friend class basic_fixed_substring<C, T, A>;

public:
    operator char_type const *() const
    {
        return m_str.data();
    }

// Members
private:
    typedef auto_buffer<char_type, allocator_type, 32>    buffer_t;

    buffer_t    m_str;

private:
    basic_fixed_substring_c_str(class_type const &);
    basic_fixed_substring_c_str &operator =(class_type const &);
};



// class basic_fixed_substring
/// A string class that holds no internal storage, and merely represents a window into other string storage
///
/// \param C The character type
/// \param T The traits type. On translators that support default template arguments this is defaulted to char_traits<C>
/// \param A The allocator type. On translators that support default template arguments this is defaulted to new_allocator<C>. This is only used by the proxy generated by c_str()
///
/// \note Thanks to Scott Patterson for suggesting the name. My \c window_string and \c proxy_string were carbuncles in comparison
template<   ss_typename_param_k C
#ifdef __STLSOFT_CF_TEMPLATE_CLASS_DEFAULT_CLASS_ARGUMENT_SUPPORT
        ,   ss_typename_param_k T = char_traits<C>
        ,   ss_typename_param_k A = new_allocator<C>
#else
        ,   ss_typename_param_k T /* = char_traits<C> */
        ,   ss_typename_param_k A /* = new_allocator<C> */
#endif /* __STLSOFT_CF_TEMPLATE_CLASS_DEFAULT_CLASS_ARGUMENT_SUPPORT */
        >
class basic_fixed_substring
{
public:
    /// The value type
    typedef C                               value_type;
    /// The traits type
    typedef T                               traits_type;
    /// The allocator type
    typedef A                               allocator_type;
    /// The current parameterisation of the type
    typedef basic_fixed_substring<C, T, A>  class_type;
    /// The character type
    typedef value_type                      char_type;
    /// The pointer type
    typedef value_type const                *pointer;
    /// The non-mutable (const) pointer type
    typedef value_type const                *const_pointer;
    /// The iterator type
    typedef value_type const                *iterator;
    /// The non-mutating (const) iterator type
    typedef value_type const                *const_iterator;
    /// The reference type
    typedef value_type                      &reference;
    /// The non-mutable (const) reference type
    typedef value_type const                &const_reference;
    /// The size type
    typedef ss_size_t                       size_type;
    /// The difference type
    typedef ss_ptrdiff_t                    difference_type;

/// \name Construction
/// @{
public:
    /// Construct from the range [first:last)
    basic_fixed_substring(char_type const *first, char_type const *last);
    /// Construct with \c n characters from the given character string
    basic_fixed_substring(char_type const *p, size_type n);
    /// Construct from a null-terminated character string
    ss_explicit_k basic_fixed_substring(char_type const *p);
#ifdef __STLSOFT_CF_STATIC_ARRAY_SIZE_DETERMINATION_SUPPORT
    /// Constructs from an array
    template <ss_typename_param_k D, ss_size_t N>
    ss_explicit_k basic_fixed_substring(D (&d)[N])
        : m_first(&d[0])
        , m_last(&d[(d[N - 1] == traits_type::to_char_type(0) ? N -1 : N)])
    {}
#endif /* __STLSOFT_CF_STATIC_ARRAY_SIZE_DETERMINATION_SUPPORT */
/// @}

/// \name Operations
//@{
public:
    /// Swaps the contents between \c this and \c other
    void swap(class_type &other);

    /// Empties the string
    void clear();
//@}

/// \name Attributes
//@{
public:
    /// The number of elements in the string
    size_type size() const;
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
    /// Returns non-mutable (const) reference at the given index
    const_reference operator [](size_type index) const;

    /// Returns null-terminated non-mutable (const) pointer to string data
    basic_fixed_substring_c_str<C, T, A> c_str() const;
    /// Returns non-mutable (const) pointer to string data
    value_type const    *data() const;
//@}

/// \name Iteration
//@{
public:
    /// Begins the iteration
    ///
    /// \return A non-mutable (const) iterator representing the start of the sequence
    const_iterator  begin() const;
    /// Ends the iteration
    ///
    /// \return A non-mutable (const) iterator representing the end of the sequence
    const_iterator  end() const;
//@}


/// \name Implementation
//@{
private:
    // Comparison
    static ss_sint_t compare_(char_type const *lhs, size_type lhs_len, char_type const *rhs, size_type rhs_len);
//@}

/// \name Members
//@{
private:
    char_type const *m_first;
    char_type const *m_last;
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
inline ss_bool_t operator ==(basic_fixed_substring<C, T, A> const &lhs, basic_fixed_substring<C, T, A> const &rhs)
{
    return lhs.compare(rhs) == 0;
}

template<   ss_typename_param_k C
        ,   ss_typename_param_k T
        ,   ss_typename_param_k A
        >
#ifdef __STLSOFT_CF_TEMPLATE_OUTOFCLASSFN_QUALIFIED_TYPE_SUPPORT
inline ss_bool_t operator ==(basic_fixed_substring<C, T, A> const &lhs, ss_typename_type_k basic_fixed_substring<C, T, A>::char_type const *rhs)
#else
inline ss_bool_t operator ==(basic_fixed_substring<C, T, A> const &lhs, C const *rhs)
#endif /* __STLSOFT_CF_TEMPLATE_OUTOFCLASSFN_QUALIFIED_TYPE_SUPPORT */
{
    return lhs.compare(rhs) == 0;
}

template<   ss_typename_param_k C
        ,   ss_typename_param_k T
        ,   ss_typename_param_k A
        >
#ifdef __STLSOFT_CF_TEMPLATE_OUTOFCLASSFN_QUALIFIED_TYPE_SUPPORT
inline ss_bool_t operator ==(ss_typename_type_k basic_fixed_substring<C, T, A>::char_type const *lhs, basic_fixed_substring<C, T, A> const &rhs)
#else
inline ss_bool_t operator ==(C *lhs, basic_fixed_substring<C, T, A> const &rhs)
#endif /* __STLSOFT_CF_TEMPLATE_OUTOFCLASSFN_QUALIFIED_TYPE_SUPPORT */
{
    return rhs.compare(lhs) == 0;
}

// operator !=

template<   ss_typename_param_k C
        ,   ss_typename_param_k T
        ,   ss_typename_param_k A
        >
inline ss_bool_t operator !=(basic_fixed_substring<C, T, A> const &lhs, basic_fixed_substring<C, T, A> const &rhs)
{
    return lhs.compare(rhs) != 0;
}
template<   ss_typename_param_k C
        ,   ss_typename_param_k T
        ,   ss_typename_param_k A
        >
#ifdef __STLSOFT_CF_TEMPLATE_OUTOFCLASSFN_QUALIFIED_TYPE_SUPPORT
inline ss_bool_t operator !=(basic_fixed_substring<C, T, A> const &lhs, ss_typename_type_k basic_fixed_substring<C, T, A>::char_type const *rhs)
#else
inline ss_bool_t operator !=(basic_fixed_substring<C, T, A> const &lhs, C const *rhs)
#endif /* __STLSOFT_CF_TEMPLATE_OUTOFCLASSFN_QUALIFIED_TYPE_SUPPORT */
{
    return lhs.compare(rhs) != 0;
}
template<   ss_typename_param_k C
        ,   ss_typename_param_k T
        ,   ss_typename_param_k A
        >
#ifdef __STLSOFT_CF_TEMPLATE_OUTOFCLASSFN_QUALIFIED_TYPE_SUPPORT
inline ss_bool_t operator !=(ss_typename_type_k basic_fixed_substring<C, T, A>::char_type const *lhs, basic_fixed_substring<C, T, A> const &rhs)
#else
inline ss_bool_t operator !=(C const *lhs, basic_fixed_substring<C, T, A> const &rhs)
#endif /* __STLSOFT_CF_TEMPLATE_OUTOFCLASSFN_QUALIFIED_TYPE_SUPPORT */
{
    return rhs.compare(lhs) != 0;
}

// operator <

template<   ss_typename_param_k C
        ,   ss_typename_param_k T
        ,   ss_typename_param_k A
        >
inline ss_bool_t operator <(basic_fixed_substring<C, T, A> const &lhs, basic_fixed_substring<C, T, A> const &rhs)
{
    return lhs.compare(rhs) < 0;
}

template<   ss_typename_param_k C
        ,   ss_typename_param_k T
        ,   ss_typename_param_k A
        >
#ifdef __STLSOFT_CF_TEMPLATE_OUTOFCLASSFN_QUALIFIED_TYPE_SUPPORT
inline ss_bool_t operator <(basic_fixed_substring<C, T, A> const &lhs, ss_typename_type_k basic_fixed_substring<C, T, A>::char_type const *rhs)
#else
inline ss_bool_t operator <(basic_fixed_substring<C, T, A> const &lhs, C const *rhs)
#endif /* __STLSOFT_CF_TEMPLATE_OUTOFCLASSFN_QUALIFIED_TYPE_SUPPORT */
{
    return lhs.compare(rhs) < 0;
}

template<   ss_typename_param_k C
        ,   ss_typename_param_k T
        ,   ss_typename_param_k A
        >
#ifdef __STLSOFT_CF_TEMPLATE_OUTOFCLASSFN_QUALIFIED_TYPE_SUPPORT
inline ss_bool_t operator <(ss_typename_type_k basic_fixed_substring<C, T, A>::char_type const *lhs, basic_fixed_substring<C, T, A> const &rhs)
#else
inline ss_bool_t operator <(C const *lhs, basic_fixed_substring<C, T, A> const &rhs)
#endif /* __STLSOFT_CF_TEMPLATE_OUTOFCLASSFN_QUALIFIED_TYPE_SUPPORT */
{
    return rhs.compare(lhs) > 0;
}

// operator <=

template<   ss_typename_param_k C
        ,   ss_typename_param_k T
        ,   ss_typename_param_k A
        >
inline ss_bool_t operator <=(basic_fixed_substring<C, T, A> const &lhs, basic_fixed_substring<C, T, A> const &rhs)
{
    return lhs.compare(rhs) <= 0;
}
template<   ss_typename_param_k C
        ,   ss_typename_param_k T
        ,   ss_typename_param_k A
        >
#ifdef __STLSOFT_CF_TEMPLATE_OUTOFCLASSFN_QUALIFIED_TYPE_SUPPORT
inline ss_bool_t operator <=(basic_fixed_substring<C, T, A> const &lhs, ss_typename_type_k basic_fixed_substring<C, T, A>::char_type const *rhs)
#else
inline ss_bool_t operator <=(basic_fixed_substring<C, T, A> const &lhs, C const *rhs)
#endif /* __STLSOFT_CF_TEMPLATE_OUTOFCLASSFN_QUALIFIED_TYPE_SUPPORT */
{
    return lhs.compare(rhs) <= 0;
}
template<   ss_typename_param_k C
        ,   ss_typename_param_k T
        ,   ss_typename_param_k A
        >
#ifdef __STLSOFT_CF_TEMPLATE_OUTOFCLASSFN_QUALIFIED_TYPE_SUPPORT
inline ss_bool_t operator <=(ss_typename_type_k basic_fixed_substring<C, T, A>::char_type const *lhs, basic_fixed_substring<C, T, A> const &rhs)
#else
inline ss_bool_t operator <=(C const *lhs, basic_fixed_substring<C, T, A> const &rhs)
#endif /* __STLSOFT_CF_TEMPLATE_OUTOFCLASSFN_QUALIFIED_TYPE_SUPPORT */
{
    return rhs.compare(lhs) >= 0;
}

// operator >

template<   ss_typename_param_k C
        ,   ss_typename_param_k T
        ,   ss_typename_param_k A
        >
inline ss_bool_t operator >(basic_fixed_substring<C, T, A> const &lhs, basic_fixed_substring<C, T, A> const &rhs)
{
    return lhs.compare(rhs) > 0;
}
template<   ss_typename_param_k C
        ,   ss_typename_param_k T
        ,   ss_typename_param_k A
        >
#ifdef __STLSOFT_CF_TEMPLATE_OUTOFCLASSFN_QUALIFIED_TYPE_SUPPORT
inline ss_bool_t operator >(basic_fixed_substring<C, T, A> const &lhs, ss_typename_type_k basic_fixed_substring<C, T, A>::char_type const *rhs)
#else
inline ss_bool_t operator >(basic_fixed_substring<C, T, A> const &lhs, C const *rhs)
#endif /* __STLSOFT_CF_TEMPLATE_OUTOFCLASSFN_QUALIFIED_TYPE_SUPPORT */
{
    return lhs.compare(rhs) > 0;
}
template<   ss_typename_param_k C
        ,   ss_typename_param_k T
        ,   ss_typename_param_k A
        >
#ifdef __STLSOFT_CF_TEMPLATE_OUTOFCLASSFN_QUALIFIED_TYPE_SUPPORT
inline ss_bool_t operator >(ss_typename_type_k basic_fixed_substring<C, T, A>::char_type const *lhs, basic_fixed_substring<C, T, A> const &rhs)
#else
inline ss_bool_t operator >(C const *lhs, basic_fixed_substring<C, T, A> const &rhs)
#endif /* __STLSOFT_CF_TEMPLATE_OUTOFCLASSFN_QUALIFIED_TYPE_SUPPORT */
{
    return rhs.compare(lhs) < 0;
}

// operator >=

template<   ss_typename_param_k C
        ,   ss_typename_param_k T
        ,   ss_typename_param_k A
        >
inline ss_bool_t operator >=(basic_fixed_substring<C, T, A> const &lhs, basic_fixed_substring<C, T, A> const &rhs)
{
    return lhs.compare(rhs) >= 0;
}
template<   ss_typename_param_k C
        ,   ss_typename_param_k T
        ,   ss_typename_param_k A
        >
#ifdef __STLSOFT_CF_TEMPLATE_OUTOFCLASSFN_QUALIFIED_TYPE_SUPPORT
inline ss_bool_t operator >=(basic_fixed_substring<C, T, A> const &lhs, ss_typename_type_k basic_fixed_substring<C, T, A>::char_type const *rhs)
#else
inline ss_bool_t operator >=(basic_fixed_substring<C, T, A> const &lhs, C const *rhs)
#endif /* __STLSOFT_CF_TEMPLATE_OUTOFCLASSFN_QUALIFIED_TYPE_SUPPORT */
{
    return lhs.compare(rhs) >= 0;
}
template<   ss_typename_param_k C
        ,   ss_typename_param_k T
        ,   ss_typename_param_k A
        >
#ifdef __STLSOFT_CF_TEMPLATE_OUTOFCLASSFN_QUALIFIED_TYPE_SUPPORT
inline ss_bool_t operator >=(ss_typename_type_k basic_fixed_substring<C, T, A>::char_type const *lhs, basic_fixed_substring<C, T, A> const &rhs)
#else
inline ss_bool_t operator >=(C const *lhs, basic_fixed_substring<C, T, A> const &rhs)
#endif /* __STLSOFT_CF_TEMPLATE_OUTOFCLASSFN_QUALIFIED_TYPE_SUPPORT */
{
    return rhs.compare(lhs) <= 0;
}


#endif /* !__STLSOFT_DOCUMENTATION_SKIP_SECTION */

/* /////////////////////////////////////////////////////////////////////////////
 * IOStream compatibility
 */

template<   ss_typename_param_k S
        ,   ss_typename_param_k C
        ,   ss_typename_param_k T
        ,   ss_typename_param_k A
        >
inline S &operator <<(S & s, basic_fixed_substring_c_str<C, T, A> const &str)
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
    ss_bool_t test_stlsoft_fixed_substring(unittest_reporter *r)
    {
        ss_bool_t               bSuccess    =   true;

        unittest_initialiser    init(r, "STLSoft", "fixed_substring", __FILE__);

        typedef stlsoft_ns_qual(basic_fixed_substring)< char
                                                    ,   char_traits<char>
                                                    ,   new_allocator<char>
                                                    >                       seg_string_t;

        char const      s[] = "Hello Natty!";

        seg_string_t    ss(s, stlsoft_num_elements(s) - 1);
        seg_string_t    ss1(&s[6], 6);
        seg_string_t    ss2(&s[0], 5);

        if(ss1 <= ss2)
        {
            r->report("<= comparison failed ", __LINE__);
            bSuccess = false;
        }
        if(ss2 > ss1)
        {
            r->report("> comparison failed ", __LINE__);
            bSuccess = false;
        }

        if(0 != seg_string_t("abc", 3).compare("abc"))
        {
            r->report("compare(s) failed ", __LINE__);
            bSuccess = false;
        }

        if(0 != seg_string_t("abcdef", 3).compare(0, 3, "abc"))
        {
            r->report("compare(p, n, s) failed ", __LINE__);
            bSuccess = false;
        }

        if(0 != seg_string_t("Well done Matty!", 16).compare(11, 5, ss, 7, 5))
        {
            r->report("compare(p, n, r, rp, rn) failed ", __LINE__);
            bSuccess = false;
        }

        return bSuccess;
    }

    unittest_registrar    unittest_stlsoft_fixed_substring(test_stlsoft_fixed_substring);

} // namespace unittest

#endif /* STLSOFT_UNITTEST */

/* /////////////////////////////////////////////////////////////////////////////
 * Implementation
 */

#ifndef __STLSOFT_DOCUMENTATION_SKIP_SECTION

template<   ss_typename_param_k C
        ,   ss_typename_param_k T
        ,   ss_typename_param_k A
        >
inline basic_fixed_substring_c_str<C, T, A>::basic_fixed_substring_c_str(ss_typename_type_k basic_fixed_substring_c_str<C, T, A>::char_type const *first, ss_typename_type_k basic_fixed_substring_c_str<C, T, A>::char_type const *last)
    : m_str(1 + (last - first))
{
    traits_type::copy(m_str, first, last - first);
    m_str[last - first] = '\0';
}


template<   ss_typename_param_k C
        ,   ss_typename_param_k T
        ,   ss_typename_param_k A
        >
inline basic_fixed_substring<C, T, A>::basic_fixed_substring(ss_typename_type_k basic_fixed_substring<C, T, A>::char_type const *first, ss_typename_type_k basic_fixed_substring<C, T, A>::char_type const *last)
    : m_first(first)
    , m_last(last)
{}

template<   ss_typename_param_k C
        ,   ss_typename_param_k T
        ,   ss_typename_param_k A
        >
inline basic_fixed_substring<C, T, A>::basic_fixed_substring(ss_typename_type_k basic_fixed_substring<C, T, A>::char_type const *first, ss_typename_type_k basic_fixed_substring<C, T, A>::size_type n)
    : m_first(first)
    , m_last(first + n)
{}

template<   ss_typename_param_k C
        ,   ss_typename_param_k T
        ,   ss_typename_param_k A
        >
inline basic_fixed_substring<C, T, A>::basic_fixed_substring(ss_typename_type_k basic_fixed_substring<C, T, A>::char_type const *first)
    : m_first(first)
    , m_last(first + traits_type::length(first))
{}

// Operations
template<   ss_typename_param_k C
        ,   ss_typename_param_k T
        ,   ss_typename_param_k A
        >
inline void basic_fixed_substring<C, T, A>::swap(ss_typename_type_k basic_fixed_substring<C, T, A>::class_type &other)
{
    char_type const *first  =   m_first;
    char_type const *last   =   m_last;

    m_first = other.m_first;
    m_last  = other.m_last;

    other.m_first   =   first;
    other.m_last    =   last;
}

template<   ss_typename_param_k C
        ,   ss_typename_param_k T
        ,   ss_typename_param_k A
        >
inline void basic_fixed_substring<C, T, A>::clear()
{
    m_first = m_last = NULL;
}

/// Attributes

template<   ss_typename_param_k C
        ,   ss_typename_param_k T
        ,   ss_typename_param_k A
        >
inline ss_typename_type_k basic_fixed_substring<C, T, A>::size_type basic_fixed_substring<C, T, A>::size() const
{
    return m_last - m_first;
}

template<   ss_typename_param_k C
        ,   ss_typename_param_k T
        ,   ss_typename_param_k A
        >
inline ss_typename_type_k basic_fixed_substring<C, T, A>::size_type basic_fixed_substring<C, T, A>::length() const
{
    return size();
}

template<   ss_typename_param_k C
        ,   ss_typename_param_k T
        ,   ss_typename_param_k A
        >
inline ss_typename_type_k basic_fixed_substring<C, T, A>::size_type basic_fixed_substring<C, T, A>::capacity() const
{
    return size();
}

template<   ss_typename_param_k C
        ,   ss_typename_param_k T
        ,   ss_typename_param_k A
        >
inline ss_bool_t basic_fixed_substring<C, T, A>::empty() const
{
    return m_first == m_last;
}

// Comparison

template<   ss_typename_param_k C
        ,   ss_typename_param_k T
        ,   ss_typename_param_k A
        >
inline /* static */ ss_sint_t basic_fixed_substring<C, T, A>::compare_(ss_typename_type_k basic_fixed_substring<C, T, A>::value_type const *lhs, ss_typename_type_k basic_fixed_substring<C, T, A>::size_type lhs_len, ss_typename_type_k basic_fixed_substring<C, T, A>::value_type const *rhs, ss_typename_type_k basic_fixed_substring<C, T, A>::size_type rhs_len)
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
        ,   ss_typename_param_k T
        ,   ss_typename_param_k A
        >
inline ss_sint_t basic_fixed_substring<C, T, A>::compare(ss_typename_type_k basic_fixed_substring<C, T, A>::size_type pos, ss_typename_type_k basic_fixed_substring<C, T, A>::size_type cch, ss_typename_type_k basic_fixed_substring<C, T, A>::value_type const *s, ss_typename_type_k basic_fixed_substring<C, T, A>::size_type cchRhs) const
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

    return compare_(m_first + pos, lhs_len, s, rhs_len);
}

template<   ss_typename_param_k C
        ,   ss_typename_param_k T
        ,   ss_typename_param_k A
        >
inline ss_sint_t basic_fixed_substring<C, T, A>::compare(ss_typename_type_k basic_fixed_substring<C, T, A>::size_type pos, ss_typename_type_k basic_fixed_substring<C, T, A>::size_type cch, ss_typename_type_k basic_fixed_substring<C, T, A>::value_type const *s) const
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

    return compare_(m_first + pos, lhs_len, s, rhs_len);
}

template<   ss_typename_param_k C
        ,   ss_typename_param_k T
        ,   ss_typename_param_k A
        >
inline ss_sint_t basic_fixed_substring<C, T, A>::compare(ss_typename_type_k basic_fixed_substring<C, T, A>::value_type const *s) const
{
    size_type   lhs_len =   length();
    size_type   rhs_len =   (s == 0) ? 0 : traits_type::length(s);

    return compare_(m_first, lhs_len, s, rhs_len);
}

template<   ss_typename_param_k C
        ,   ss_typename_param_k T
        ,   ss_typename_param_k A
        >
inline ss_sint_t basic_fixed_substring<C, T, A>::compare(ss_typename_type_k basic_fixed_substring<C, T, A>::size_type pos, ss_typename_type_k basic_fixed_substring<C, T, A>::size_type cch, ss_typename_type_k basic_fixed_substring<C, T, A>::class_type const &rhs, ss_typename_type_k basic_fixed_substring<C, T, A>::size_type posRhs, ss_typename_type_k basic_fixed_substring<C, T, A>::size_type cchRhs) const
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

    return compare_(m_first + pos, lhs_len, rhs.m_first + posRhs, rhs_len);
}

template<   ss_typename_param_k C
        ,   ss_typename_param_k T
        ,   ss_typename_param_k A
        >
inline ss_sint_t basic_fixed_substring<C, T, A>::compare(ss_typename_type_k basic_fixed_substring<C, T, A>::size_type pos, ss_typename_type_k basic_fixed_substring<C, T, A>::size_type cch, ss_typename_type_k basic_fixed_substring<C, T, A>::class_type const &rhs) const
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

    return compare_(m_first + pos, lhs_len, rhs.m_first, rhs_len);
}

template<   ss_typename_param_k C
        ,   ss_typename_param_k T
        ,   ss_typename_param_k A
        >
inline ss_sint_t basic_fixed_substring<C, T, A>::compare(ss_typename_type_k basic_fixed_substring<C, T, A>::class_type const &rhs) const
{
    size_type   lhs_len =   length();
    size_type   rhs_len =   rhs.length();

    return compare_(m_first, lhs_len, rhs.m_first, rhs_len);
}

// Accessors

template<   ss_typename_param_k C
        ,   ss_typename_param_k T
        ,   ss_typename_param_k A
        >
inline ss_typename_type_k basic_fixed_substring<C, T, A>::const_reference basic_fixed_substring<C, T, A>::operator [](ss_typename_type_k basic_fixed_substring<C, T, A>::size_type index) const
{
    stlsoft_message_assert("Index out of range", !(size() < index));

    return m_first[index];
}

template<   ss_typename_param_k C
        ,   ss_typename_param_k T
        ,   ss_typename_param_k A
        >
inline basic_fixed_substring_c_str<C, T, A> basic_fixed_substring<C, T, A>::c_str() const
{
    return basic_fixed_substring_c_str<C, T, A>(m_first, m_last);
}

template<   ss_typename_param_k C
        ,   ss_typename_param_k T
        ,   ss_typename_param_k A
        >
inline ss_typename_type_k basic_fixed_substring<C, T, A>::value_type const *basic_fixed_substring<C, T, A>::data() const
{
    return m_first;
}

/// Iteration

template<   ss_typename_param_k C
        ,   ss_typename_param_k T
        ,   ss_typename_param_k A
        >
inline ss_typename_type_k basic_fixed_substring<C, T, A>::const_iterator basic_fixed_substring<C, T, A>::begin() const
{
    return m_first;
}

template<   ss_typename_param_k C
        ,   ss_typename_param_k T
        ,   ss_typename_param_k A
        >
inline ss_typename_type_k basic_fixed_substring<C, T, A>::const_iterator basic_fixed_substring<C, T, A>::end() const
{
    return m_last;
}

#endif /* !__STLSOFT_DOCUMENTATION_SKIP_SECTION */

/* /////////////////////////////////////////////////////////////////////////////
 * String access shims
 */

#if 0
/* c_str_ptr_null */

/// \brief Returns the corresponding C-string pointer of \c s, or a null pointer
template<   ss_typename_param_k C
        ,   ss_typename_param_k T
        ,   ss_typename_param_k A
        >
inline C const *c_str_ptr_null(basic_fixed_substring<C, T, A> const &s)
{
    return (s.length() == 0) ? 0 : s.c_str();
}

/* c_str_ptr */

/// \brief Returns the corresponding C-string pointer of \c s
template<   ss_typename_param_k C
        ,   ss_typename_param_k T
        ,   ss_typename_param_k A
        >
inline C const *c_str_ptr(basic_fixed_substring<C, T, A> const &s)
{
    return s.c_str();
}

/// \brief Returns the corresponding C-string pointer of \c s
template<   ss_typename_param_k T
        ,   ss_typename_param_k A
        >
inline ss_char_a_t const *c_str_ptr_a(basic_fixed_substring<ss_char_a_t, T, A> const &s)
{
    return s.c_str();
}

/// \brief Returns the corresponding C-string pointer of \c s
template<   ss_typename_param_k T
        ,   ss_typename_param_k A
        >
inline ss_char_w_t const *c_str_ptr_w(basic_fixed_substring<ss_char_w_t, T, A> const &s)
{
    return s.c_str();
}

/* c_str_ptr_len */

/// \brief Returns the length (in characters) of \c s, <b><i>not</i></b> including the null-terminating character
template<   ss_typename_param_k C
        ,   ss_typename_param_k T
        ,   ss_typename_param_k A
        >
inline ss_size_t c_str_len(basic_fixed_substring<C, T, A> const &s)
{
    return s.length();
}

/* c_str_ptr_size */

/// \brief Returns the size (in bytes) of the contents of \c s, <b><i>not</i></b> including the null-terminating character
template<   ss_typename_param_k C
        ,   ss_typename_param_k T
        ,   ss_typename_param_k A
        >
inline ss_size_t c_str_size(basic_fixed_substring<C, T, A> const &s)
{
    return c_str_len(s) * sizeof(C);
}
#endif /* 0 */

/* ////////////////////////////////////////////////////////////////////////// */

/// @} // end of group stlsoft_string_library

/* ////////////////////////////////////////////////////////////////////////// */

#ifndef _STLSOFT_NO_NAMESPACE
} // namespace stlsoft
#endif /* _STLSOFT_NO_NAMESPACE */

/* ////////////////////////////////////////////////////////////////////////// */

#endif /* !STLSOFT_INCL_H_STLSOFT_FIXED_SUBSTRING */

/* ////////////////////////////////////////////////////////////////////////// */
