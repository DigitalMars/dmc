/* /////////////////////////////////////////////////////////////////////////////
 * File:        stlsoft_proxy_iterator.h (Formerly part of stlsoft_proxy_sequence.h)
 *
 * Purpose:     proxy_sequence template class.
 *
 * Created:     28th June 2004
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


/// \file stlsoft_proxy_sequence.h
///
/// proxy_sequence template class.

#ifndef STLSOFT_INCL_H_STLSOFT_PROXY_ITERATOR
#define STLSOFT_INCL_H_STLSOFT_PROXY_ITERATOR

#ifndef __STLSOFT_DOCUMENTATION_SKIP_SECTION
# define STLSOFT_VER_H_STLSOFT_PROXY_ITERATOR_MAJOR       2
# define STLSOFT_VER_H_STLSOFT_PROXY_ITERATOR_MINOR       0
# define STLSOFT_VER_H_STLSOFT_PROXY_ITERATOR_REVISION    1
# define STLSOFT_VER_H_STLSOFT_PROXY_ITERATOR_EDIT        20
#endif /* !__STLSOFT_DOCUMENTATION_SKIP_SECTION */

/* /////////////////////////////////////////////////////////////////////////////
 * Includes
 */

#ifndef STLSOFT_INCL_H_STLSOFT
# include "stlsoft.h"                       // Include the STLSoft root header
#endif /* !STLSOFT_INCL_H_STLSOFT */
#ifndef STLSOFT_INCL_H_STLSOFT_ITERATOR
# include "stlsoft_iterator.h"              // iterator_base
#endif /* !STLSOFT_INCL_H_STLSOFT_ITERATOR */
#ifndef STLSOFT_INCL_H_STLSOFT_ADDRESS_HIDING_VENEER
# include "stlsoft_address_hiding_veneer.h" // address_hiding_veneer
#endif /* !STLSOFT_INCL_H_STLSOFT_ADDRESS_HIDING_VENEER */
#ifndef STLSOFT_INCL_H_STLSOFT_OUTER_INITIALISER
# include "stlsoft_outer_initialiser.h"     // outer_initialiser
#endif /* !STLSOFT_INCL_H_STLSOFT_OUTER_INITIALISER */

/* /////////////////////////////////////////////////////////////////////////////
 * Namespace
 */

#ifndef _STLSOFT_NO_NAMESPACE
namespace stlsoft
{
#endif /* _STLSOFT_NO_NAMESPACE */

/* /////////////////////////////////////////////////////////////////////////////
 * Warnings
 */

#if defined(__STLSOFT_COMPILER_IS_MSVC)
# if _MSC_VER >= 1200
#  pragma warning(push)
# endif /* _MSC_VER */
# pragma warning(disable : 4355)
#endif /* __STLSOFT_COMPILER_IS_MSVC */

/* /////////////////////////////////////////////////////////////////////////////
 * Classes
 */

/** \brief Provides translation between the elements in a range and a different value type
 *
 * \param R The element type. This type is the element in the underlying sequence
 * \param V The value type. This is the type to which the element type is translated
 * \param T The traits_type. This type provides a static method \c make_value(), which
 * converts the element type to the value type
 */
/// Iterator for proxy_sequence
template<   ss_typename_param_k E
        ,   ss_typename_param_k V
        ,   ss_typename_param_k T
        ,   ss_typename_param_k C
        >
class proxy_iterator
    : public iterator_base<C, V, ss_ptrdiff_t, V *, V&>
{
/// \name Types
/// @{
private:
    typedef iterator_base<C, V, ss_ptrdiff_t, V *, V&>              parent_class_type;
    typedef ss_typename_type_k parent_class_type::value_type        raw_value_type;
public:
    typedef E                                                       element_type;
//    typedef address_hiding_veneer<raw_value_type>                   value_type;
    typedef raw_value_type                                          value_type;
    typedef value_type                                              &reference;
    typedef value_type const                                        &const_reference;
    typedef T                                                       traits_type;
    typedef ss_typename_type_k parent_class_type::iterator_category iterator_category;
    typedef ss_typename_type_k parent_class_type::pointer           pointer;
    typedef proxy_iterator<E, V, T, C>                              class_type;
    typedef ss_size_t                                               size_type;
    typedef ss_typename_type_k parent_class_type::difference_type   difference_type;
/// @}

/// \name Construction
/// @{
public:
    proxy_iterator()
        : m_begin(NULL)
        , m_end(NULL)
        , m_value(value_type())
        , m_initialiser(this, &class_type::init_value_)
    {}
    proxy_iterator(element_type *first, element_type *last)
        : m_begin(first)
        , m_end(last)
        , m_value(value_type())
        , m_initialiser(this, &class_type::init_value_)
    {}
    proxy_iterator(element_type *first, size_type n)
        : m_begin(first)
        , m_end(first + n)
        , m_value(value_type())
        , m_initialiser(this, &class_type::init_value_)
    {}
    proxy_iterator(class_type const &rhs)
        : m_begin(rhs.m_begin)
        , m_end(rhs.m_end)
        , m_value(value_type())
        , m_initialiser(this, &class_type::init_value_)
    {}
#if !defined(__STLSOFT_COMPILER_IS_MSVC) || \
    _MSC_VER > 1200
    /// Copy from
    template <ss_typename_param_k I>
    proxy_iterator(I const &i)
        : m_begin(i.begin())
        , m_end(i.end())
        , m_value(value_type())
        , m_initialiser(this, &class_type::init_value_)
    {}
#endif /* !__STLSOFT_COMPILER_IS_MSVC || _MSC_VER > 1200 */

    class_type &operator =(class_type const &rhs)
    {
        m_begin         =   rhs.m_begin;
        m_end           =   rhs.m_end;

        init_value_();

        return *this;
    }
#if !defined(__STLSOFT_COMPILER_IS_MSVC) || \
    _MSC_VER > 1200
    template <ss_typename_param_k I>
    class_type &operator =(I const &rhs)
    {
        return operator =(class_type(rhs.begin(), rhs.end()));
    }
#endif /* !__STLSOFT_COMPILER_IS_MSVC || _MSC_VER > 1200 */
/// @}

/// \name Helper functions
/// @{
public:
    /// The proxy iterator begin-point
    ///
    /// \return A pointer to the current position of the proxy iterator
    element_type    *begin()
    {
        return m_begin;
    }
    /// The proxy iterator end-point
    ///
    /// \return A pointer to the end point of the proxy iterator
    element_type    *end()
    {
        return m_end;
    }
    /// The proxy iterator begin-point
    ///
    /// \return A pointer to the current position of the proxy iterator
    element_type const  *begin() const
    {
        return m_begin;
    }
    /// The proxy iterator end-point
    ///
    /// \return A pointer to the end point of the proxy iterator
    element_type const  *end() const
    {
        return m_end;
    }
/// @}

/// \name Iterator methods
/// @{
public:
    /// Pre-increment operator
    class_type &operator ++()
    {
        stlsoft_message_assert("Incrementing invalid iterator", m_begin != m_end);

        ++m_begin;

        init_value_();

        return *this;
    }
    /// Post-increment operator
    class_type operator ++(int)
    {
        class_type  r(*this);

        operator ++();

        return r;
    }
    /// Dereference to return a value at the current position of type V
    const_reference operator *() const
    {
        stlsoft_message_assert("Attempting to dereference a null pointer", begin() != end());

        return m_value;
    }

    /// Evaluates whether \c this and \c rhs are equivalent
    bool operator ==(class_type const &rhs) const
    {
        if(m_end == rhs.m_end)
        {
            return m_begin == rhs.m_begin;
        }
        else
        {
            return (m_begin == m_end) == (rhs.m_begin == rhs.m_end);
        }
    }
    /// Evaluates whether \c this and \c rhs are not equivalent
    bool operator !=(class_type const &rhs) const
    {
        return !operator ==(rhs);
    }
/// @}

// Implementation
private:
    void init_value_()
    {
        if(m_begin != m_end)
        {
            // TODO: Fix this nasty hack

            remove_const(m_value) = value_type(traits_type::make_value(*m_begin));
        }
    }

// Members
private:
    element_type                    *m_begin;
    element_type                    *m_end;
    value_type                      m_value;
    outer_initialiser<class_type>   m_initialiser;
};

/* /////////////////////////////////////////////////////////////////////////////
 * Operators
 */

template<   ss_typename_param_k E
        ,   ss_typename_param_k V
        ,   ss_typename_param_k T
        ,   ss_typename_param_k C
        >
inline proxy_iterator<E, V, T, C> operator +(proxy_iterator<E, V, T, C> const &lhs, ss_ptrdiff_t d)
{
    return proxy_iterator<E, V, T, C>(lhs.m_begin + d, lhs.m_end);
}

template<   ss_typename_param_k E
        ,   ss_typename_param_k V
        ,   ss_typename_param_k T
        ,   ss_typename_param_k C
        >
inline proxy_iterator<E, V, T, C> operator -(proxy_iterator<E, V, T, C> const &lhs, ss_ptrdiff_t d)
{
    return proxy_iterator<E, V, T, C>(lhs.m_begin - d, lhs.m_end);
}

template<   ss_typename_param_k E
        ,   ss_typename_param_k V
        ,   ss_typename_param_k T
        ,   ss_typename_param_k C
        >
inline ss_ptrdiff_t operator -(proxy_iterator<E, V, T, C> const &lhs, proxy_iterator<E, V, T, C> const &rhs)
{
    return rhs.m_begin - lhs.m_begin;
}


/* /////////////////////////////////////////////////////////////////////////////
 * Warnings
 */

#if defined(__STLSOFT_COMPILER_IS_MSVC)
# if _MSC_VER >= 1200
#  pragma warning(pop)
# else /* ? _MSC_VER */
#  pragma warning(default: 4355)
# endif /* _MSC_VER */
#endif /* __STLSOFT_COMPILER_IS_MSVC */

/* ////////////////////////////////////////////////////////////////////////// */

#ifndef _STLSOFT_NO_NAMESPACE
} // namespace stlsoft
#endif /* _STLSOFT_NO_NAMESPACE */

/* ////////////////////////////////////////////////////////////////////////// */

#endif /* !STLSOFT_INCL_H_STLSOFT_PROXY_ITERATOR */

/* ////////////////////////////////////////////////////////////////////////// */
