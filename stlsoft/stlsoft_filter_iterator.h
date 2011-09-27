/* /////////////////////////////////////////////////////////////////////////////
 * File:        stlsoft_filter_iterator.h
 *
 * Purpose:     An iterator adaptor that uses a predicate to filter desired
 *              values from the iterator's underlying sequence.
 *
 * Created:     9th July 2004
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


/// \file stlsoft_filter_iterator.h
///
/// An iterator adaptor that uses a predicate to filter desired values from the iterator's underlying sequence.

#ifndef STLSOFT_INCL_H_STLSOFT_FILTER_ITERATOR
#define STLSOFT_INCL_H_STLSOFT_FILTER_ITERATOR

#ifndef __STLSOFT_DOCUMENTATION_SKIP_SECTION
# define STLSOFT_VER_H_STLSOFT_FILTER_ITERATOR_MAJOR    2
# define STLSOFT_VER_H_STLSOFT_FILTER_ITERATOR_MINOR    0
# define STLSOFT_VER_H_STLSOFT_FILTER_ITERATOR_REVISION 2
# define STLSOFT_VER_H_STLSOFT_FILTER_ITERATOR_EDIT     4
#endif /* !__STLSOFT_DOCUMENTATION_SKIP_SECTION */

/* /////////////////////////////////////////////////////////////////////////////
 * Includes
 */

#ifndef STLSOFT_INCL_H_STLSOFT
# include "stlsoft.h"               // Include the STLSoft root header
#endif /* !STLSOFT_INCL_H_STLSOFT */
#ifndef STLSOFT_INCL_H_STLSOFT_ITERATOR
# include "stlsoft_iterator.h"      // stlsoft::iterator, and macros
#endif /* !STLSOFT_INCL_H_STLSOFT_ITERATOR */
#ifndef STLSOFT_INCL_H_STLSOFT_META
# include "stlsoft_meta.h"          // TMP stuff
#endif /* !STLSOFT_INCL_H_STLSOFT_META */
#ifndef STLSOFT_INCL_H_STLSOFT_TYPE_TRAITS
# include "stlsoft_type_traits.h"   // base_type_traits
#endif /* !STLSOFT_INCL_H_STLSOFT_TYPE_TRAITS */

/* /////////////////////////////////////////////////////////////////////////////
 * Namespace
 */

#ifndef _STLSOFT_NO_NAMESPACE
namespace stlsoft
{
#endif /* _STLSOFT_NO_NAMESPACE */

/* /////////////////////////////////////////////////////////////////////////////
 * Classes
 */

template <ss_typename_param_k I>
struct filtered_iterator_traits
{
public:
    typedef ss_typename_type_k I::iterator_category                                     iterator_category;
    typedef ss_typename_type_k I::value_type                                            value_type;
    typedef ss_typename_type_k I::difference_type                                       difference_type;
private:
#ifdef STLSOFT_ITERATOR_ITERATOR_FORM2_SUPPORT
    // Use the iterator itself to determine whether the iterator is const
    enum { IS_CONST = base_type_traits<value_type>::is_const };
#else /* ? STLSOFT_ITERATOR_ITERATOR_FORM2_SUPPORT */
    // 1. Use iterator_traits ...
    typedef std::iterator_traits<I>                                                     traits_type_;
    // ... to get the pointer type ...
    typedef ss_typename_type_k traits_type_::pointer                                    pointer_type_;
    // ... to determine whether the iterator is const
    enum { IS_CONST = base_type_traits<pointer_type_>::is_const };
#endif /* !STLSOFT_ITERATOR_ITERATOR_FORM2_SUPPORT */
    // 2. Use Determine the base type of the value_type, which may then be used to create a reference of the appropriate type
    typedef ss_typename_type_k base_type_traits<value_type>::base_type                  base_value_type;
//  enum { IS_CONST = is_const<pointer>::value };
public:
    // TODO: Need to discriminate pointer and, hence, const_pointer types
//    typedef value_type volatile const                       *pointer;
//    typedef value_type volatile const                       *const_pointer;
    typedef I                                                                           iterator;
    typedef ss_typename_type_k base_type_traits<iterator>::base_type const              &const_iterator;
    typedef ss_typename_type_k select_first_type<   base_value_type const &
                                                ,   base_value_type &, IS_CONST>::type  reference;
    typedef base_value_type const                                                       &const_reference;
};

#ifndef __STLSOFT_DOCUMENTATION_SKIP_SECTION

template <ss_typename_param_k T>
struct filtered_iterator_traits<T*>
{
    typedef stlsoft_ns_qual_std(random_access_iterator_tag) iterator_category;
    typedef T                                               value_type;
    typedef ptrdiff_t                                       difference_type;
    typedef value_type                                      *pointer;
    typedef value_type const                                *const_pointer;
    typedef value_type                                      *iterator;
    typedef value_type const                                *const_iterator;
    typedef value_type                                      &reference;
    typedef value_type const                                &const_reference;
};

template <ss_typename_param_k T>
struct filtered_iterator_traits<T const*>
{
    typedef stlsoft_ns_qual_std(random_access_iterator_tag) iterator_category;
    typedef T                                               value_type;
    typedef ptrdiff_t                                       difference_type;
    typedef value_type const                                *pointer;
    typedef value_type const                                *const_pointer;
    typedef value_type const                                *iterator;
    typedef value_type const                                *const_iterator;
    typedef value_type const                                &reference;
    typedef value_type const                                &const_reference;
};

template <ss_typename_param_k T>
struct filtered_iterator_traits<T volatile*>
{
    typedef stlsoft_ns_qual_std(random_access_iterator_tag) iterator_category;
    typedef T                                               value_type;
    typedef ptrdiff_t                                       difference_type;
    typedef value_type volatile                             *pointer;
    typedef value_type volatile const                       *const_pointer;
    typedef value_type volatile                             *iterator;
    typedef value_type volatile const                       *const_iterator;
    typedef value_type volatile                             &reference;
    typedef value_type volatile const                       &const_reference;
};

template <ss_typename_param_k T>
struct filtered_iterator_traits<T const volatile*>
{
    typedef stlsoft_ns_qual_std(random_access_iterator_tag) iterator_category;
    typedef T                                               value_type;
    typedef ptrdiff_t                                       difference_type;
    typedef value_type volatile const                       *pointer;
    typedef value_type volatile const                       *const_pointer;
    typedef value_type volatile const                       *iterator;
    typedef value_type volatile const                       *const_iterator;
    typedef value_type volatile const                       &reference;
    typedef value_type volatile const                       &const_reference;
};

#endif /* !__STLSOFT_DOCUMENTATION_SKIP_SECTION */


/// Filters the values of an iterable range
template<   ss_typename_param_k I
        ,   ss_typename_param_k P
        ,   ss_typename_param_k T = filtered_iterator_traits<I>
        >
class filtered_iterator
{
public:
    typedef I                                                   filtered_iterator_type;
    typedef P                                                   filter_predicate_type;
    typedef T                                                   traits_type;
    typedef filtered_iterator<I, P, T>                          class_type;
    typedef ss_typename_type_k traits_type::iterator_category   iterator_category;
    typedef ss_typename_type_k traits_type::value_type          value_type;
    typedef ss_typename_type_k traits_type::difference_type     difference_type;
    typedef ss_typename_type_k traits_type::pointer             pointer;
    typedef ss_typename_type_k traits_type::const_pointer       const_pointer;
    typedef ss_typename_type_k traits_type::reference           reference;
    typedef ss_typename_type_k traits_type::const_reference     const_reference;

public:
    filtered_iterator(filtered_iterator_type begin, filtered_iterator_type end, filter_predicate_type pr)
        : m_begin(begin)
        , m_end(end)
        , m_predicate(pr)
    {
        for(; m_begin != m_end; ++m_begin)
        {
            if(m_predicate(*m_begin))
            {
                break;
            }
        }
    }

/// \name Accessors
/// @{
public:
    reference operator *()
    {
        return *m_begin;
    }
    const_reference operator *() const
    {
        return *m_begin;
    }
    pointer operator ->()
    {
        return &*m_begin;
    }
    const_pointer operator ->() const
    {
        return &*m_begin;
    }
/// @}

/// \name Forward Iterator methods
/// @{
public:
    class_type &operator ++()
    {
        for(++m_begin; m_begin != m_end; ++m_begin)
        {
            if(m_predicate(*m_begin))
            {
                break;
            }
        }
    }
    class_type &operator ++(int)
    {
        class_type  ret(*this);

        operator ++();

        return ret;
    }
/// @}

/// \name Bidirectional Iterator methods
/// @{
public:
    class_type &operator --();
#if 0
    {
        // TODO: Do filtering here
    }
#endif /* 0 */
    class_type &operator --(int)
    {
        class_type  ret(*this);

        operator --();

        return ret;
    }
/// @}

/// \name Random Access Iterator methods
/// @{
public:
    class_type &operator +=(difference_type d)
    {
        m_begin += d;

        return *this;
    }
    class_type &operator -=(difference_type d)
    {
        m_begin -= d;

        return *this;
    }

    reference operator [](difference_type index)
    {
        stlsoft_message_assert("Requested index is out of range", index <= distance());

        return m_begin[index];
    }
    const_reference operator [](difference_type index) const
    {
        stlsoft_message_assert("Requested index is out of range", index <= distance());

        return m_begin[index];
    }
/// @}

/// \name Comparison
/// @{
public:
    ss_bool_t equals(class_type const &rhs) const
    {
        stlsoft_message_assert("Comparing iterators from different sequences", m_end == rhs.m_end);

        return m_begin == rhs.m_begin;
    }
    ss_sint_t compare(class_type const &rhs) const
    {
        stlsoft_message_assert("Comparing iterators from different sequences", m_end == rhs.m_end);

        return m_begin - rhs.m_begin;
    }
    difference_type difference(class_type const &rhs) const
    {
        stlsoft_message_assert("Comparing iterators from different sequences", m_end == rhs.m_end);

        return m_begin - rhs.m_begin;
    }
/// @}

/// \name Attributes
/// @{
public:
    difference_type distance() const
    {
        return m_end - m_begin;
    }
/// @}


// Members
private:
    filtered_iterator_type  m_begin;
    filtered_iterator_type  m_end;
    filter_predicate_type   m_predicate;
};

/* /////////////////////////////////////////////////////////////////////////////
 * Operators
 */

// operator +

template<   ss_typename_param_k I
        ,   ss_typename_param_k P
        ,   ss_typename_param_k T
        >
inline filtered_iterator<I, P, T> operator +(filtered_iterator<I, P, T> const &lhs, ss_typename_type_k filtered_iterator<I, P, T>::difference_type rhs)
{
    return filtered_iterator<I, P, T>(lhs) += rhs;
}

// operator -

template<   ss_typename_param_k I
        ,   ss_typename_param_k P
        ,   ss_typename_param_k T
        >
inline filtered_iterator<I, P, T> operator -(filtered_iterator<I, P, T> const &lhs, ss_typename_type_k filtered_iterator<I, P, T>::difference_type rhs)
{
    return filtered_iterator<I, P, T>(lhs) -= rhs;
}

template<   ss_typename_param_k I
        ,   ss_typename_param_k P
        ,   ss_typename_param_k T
        >
inline ss_typename_type_k filtered_iterator<I, P, T>::difference_type operator -(filtered_iterator<I, P, T> const &lhs, filtered_iterator<I, P, T> const &rhs)
{
    return lhs.difference(rhs);
}

// operator ==

template<   ss_typename_param_k I
        ,   ss_typename_param_k P
        ,   ss_typename_param_k T
        >
inline ss_bool_t operator ==(filtered_iterator<I, P, T> const &lhs, filtered_iterator<I, P, T> const &rhs)
{
    return lhs.equals(rhs);
}

// operator !=

template<   ss_typename_param_k I
        ,   ss_typename_param_k P
        ,   ss_typename_param_k T
        >
inline ss_bool_t operator !=(filtered_iterator<I, P, T> const &lhs, filtered_iterator<I, P, T> const &rhs)
{
    return !lhs.equals(rhs);
}

// operator <

template<   ss_typename_param_k I
        ,   ss_typename_param_k P
        ,   ss_typename_param_k T
        >
inline ss_bool_t operator <(filtered_iterator<I, P, T> const &lhs, filtered_iterator<I, P, T> const &rhs)
{
    return lhs.compare(rhs) < 0;
}

// operator <=

template<   ss_typename_param_k I
        ,   ss_typename_param_k P
        ,   ss_typename_param_k T
        >
inline ss_bool_t operator <=(filtered_iterator<I, P, T> const &lhs, filtered_iterator<I, P, T> const &rhs)
{
    return lhs.compare(rhs) <= 0;
}

// operator >

template<   ss_typename_param_k I
        ,   ss_typename_param_k P
        ,   ss_typename_param_k T
        >
inline ss_bool_t operator >(filtered_iterator<I, P, T> const &lhs, filtered_iterator<I, P, T> const &rhs)
{
    return lhs.compare(rhs) > 0;
}

// operator >=

template<   ss_typename_param_k I
        ,   ss_typename_param_k P
        ,   ss_typename_param_k T
        >
inline ss_bool_t operator >=(filtered_iterator<I, P, T> const &lhs, filtered_iterator<I, P, T> const &rhs)
{
    return lhs.compare(rhs) >= 0;
}

/* ////////////////////////////////////////////////////////////////////////// */

#ifndef _STLSOFT_NO_NAMESPACE
} // namespace stlsoft
#endif /* _STLSOFT_NO_NAMESPACE */

/* ////////////////////////////////////////////////////////////////////////// */

#endif /* !STLSOFT_INCL_H_STLSOFT_FILTER_ITERATOR */

/* ////////////////////////////////////////////////////////////////////////// */
