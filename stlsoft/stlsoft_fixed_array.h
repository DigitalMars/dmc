/* /////////////////////////////////////////////////////////////////////////////
 * File:        stlsoft_fixed_array.h (originally MTArray.h, ::SynesisStl)
 *
 * Purpose:     Contains the array1D, array2D template classes.
 *
 * Created:     4th August 1998
 * Updated:     11th September 2004
 *
 * Home:        http://stlsoft.org/
 *
 * Copyright (c) 1998-2004, Matthew Wilson and Synesis Software
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


/// \file stlsoft_fixed_array.h
///
/// Contains the array1D, array2D template classes.

#ifndef STLSOFT_INCL_H_STLSOFT_FIXED_ARRAY
#define STLSOFT_INCL_H_STLSOFT_FIXED_ARRAY

#ifndef __STLSOFT_DOCUMENTATION_SKIP_SECTION
# define STLSOFT_VER_H_STLSOFT_FIXED_ARRAY_MAJOR     2
# define STLSOFT_VER_H_STLSOFT_FIXED_ARRAY_MINOR     0
# define STLSOFT_VER_H_STLSOFT_FIXED_ARRAY_REVISION  1
# define STLSOFT_VER_H_STLSOFT_FIXED_ARRAY_EDIT      132
#endif /* !__STLSOFT_DOCUMENTATION_SKIP_SECTION */

/* /////////////////////////////////////////////////////////////////////////////
 * Compatibility
 */

/*
[Incompatibilies-start]
__STLSOFT_COMPILER_IS_MSVC: _MSC_VER<1200
[Incompatibilies-end]
 */

/* /////////////////////////////////////////////////////////////////////////////
 * Includes
 */

#ifndef STLSOFT_INCL_H_STLSOFT
# include "stlsoft.h"                   // Include the STLSoft root header
#endif /* !STLSOFT_INCL_H_STLSOFT */

#if defined(__STLSOFT_COMPILER_IS_MSVC) && \
    _MSC_VER < 1200
# error stlsoft_fixed_array.h is not compatible with Visual C++ 5.0 or earlier
#endif /* _MSC_VER < 1200 */

#ifndef STLSOFT_INCL_H_STLSOFT_NEW_ALLOCATOR
# include "stlsoft_new_allocator.h"     // new_allocator
#endif /* !STLSOFT_INCL_H_STLSOFT_NEW_ALLOCATOR */
#ifndef STLSOFT_INCL_H_STLSOFT_ITERATOR
# include "stlsoft_iterator.h"          // stlsoft::iterator, stlsoft::reverse_iterator
#endif /* STLSOFT_INCL_H_STLSOFT_ITERATOR */
#ifndef STLSOFT_INCL_H_STLSOFT_ARRAY_POLICIES
# include "stlsoft_array_policies.h"    // stlsoft::do_construction
#endif /* !STLSOFT_INCL_H_STLSOFT_ARRAY_POLICIES */
#ifndef STLSOFT_INCL_H_STLSOFT_META
# include "stlsoft_meta.h"  // is_pointer
#endif /* !STLSOFT_INCL_H_STLSOFT_META */

#if 0
#    define STLSOFT_FIXED_ARRAY_USES_CONTAINER_BASE
#    ifndef STLSOFT_INCL_H_STLSOFT_CONTAINER_BASE
#     include "stlsoft_container_base.h"
#    endif /* !STLSOFT_INCL_H_STLSOFT_CONTAINER_BASE */
#endif /* 0 */

#if defined(__STLSOFT_COMPILER_IS_MSVC) && \
    defined(__STLSOFT_CF_STL_IS_STLPORT)
# include <string>                      // std::string - sigh!
#endif /* __STLSOFT_COMPILER_IS_MSVC && __STLSOFT_CF_STL_IS_STLPORT */
#include <stdexcept>                    // std::out_of_range

/* /////////////////////////////////////////////////////////////////////////////
 * Namespace
 */

#ifndef _STLSOFT_NO_NAMESPACE
namespace stlsoft
{
#endif /* _STLSOFT_NO_NAMESPACE */

/* /////////////////////////////////////////////////////////////////////////////
 * Forward declarations
 */

#ifndef __STLSOFT_DOCUMENTATION_SKIP_SECTION

template<   ss_typename_param_k T
        ,   ss_typename_param_k A
        ,   ss_typename_param_k P
        ,   ss_bool_t           R
        >
class fixed_array_1d;

template<   ss_typename_param_k T
        ,   ss_typename_param_k A
        ,   ss_typename_param_k P
        ,   ss_bool_t           R
        >
class fixed_array_2d;

template<   ss_typename_param_k T
        ,   ss_typename_param_k A
        ,   ss_typename_param_k P
        ,   ss_bool_t           R
        >
class fixed_array_3d;

template<   ss_typename_param_k T
        ,   ss_typename_param_k A
        ,   ss_typename_param_k P
        ,   ss_bool_t           R
        >
class fixed_array_4d;

template<   ss_typename_param_k T
        ,   ss_typename_param_k A
        ,   ss_typename_param_k P
        ,   ss_bool_t           R
        >
class fixed_array_5d;

#endif /* !__STLSOFT_DOCUMENTATION_SKIP_SECTION */

/* /////////////////////////////////////////////////////////////////////////////
 * Classes
 */

// class fixed_array_1d
/// 1 dimensional fixed array
///
/// \param T The value type
/// \param A The allocator type
/// \param P The construction policy type
template<   ss_typename_param_k T
#ifdef __STLSOFT_CF_TEMPLATE_CLASS_DEFAULT_CLASS_ARGUMENT_SUPPORT
        ,   ss_typename_param_k A = new_allocator<T>
        ,   ss_typename_param_k P = do_construction<T>
        ,   ss_bool_t           R = true
#else
        ,   ss_typename_param_k A
        ,   ss_typename_param_k P
        ,   ss_typename_param_k R
#endif /* __STLSOFT_CF_TEMPLATE_CLASS_DEFAULT_CLASS_ARGUMENT_SUPPORT */
        >
class fixed_array_1d
#ifdef STLSOFT_FIXED_ARRAY_USES_CONTAINER_BASE
    : public container_base<A>
#else /* ? STLSOFT_FIXED_ARRAY_USES_CONTAINER_BASE */
    : protected A
#endif /* STLSOFT_FIXED_ARRAY_USES_CONTAINER_BASE */
{
public:
    typedef fixed_array_1d<T, A, P, R>                              class_type;
    typedef T                                                       dimension_element_type;
#ifdef STLSOFT_FIXED_ARRAY_USES_CONTAINER_BASE
    typedef ss_typename_type_k container_base<A>::allocator_type    allocator_type;
    typedef P                                                       initialisation_policy_type;
#else /* ? STLSOFT_FIXED_ARRAY_USES_CONTAINER_BASE */
    typedef A                                                       allocator_type;
#endif /* STLSOFT_FIXED_ARRAY_USES_CONTAINER_BASE */

//private:
//  typedef inplace_range_construct<T>::
public:
    typedef T                                               value_type;
    typedef value_type                                      &reference;
    typedef value_type const                                &const_reference;
    typedef value_type                                      *pointer;
    typedef value_type const                                *const_pointer;
    typedef ss_size_t                                       size_type;
    typedef ss_size_t                                       index_type;
    typedef ss_ptrdiff_t                                    difference_type;

    typedef
#if !defined(__STLSOFT_COMPILER_IS_BORLAND)
           ss_typename_type_k
#endif /* __STLSOFT_COMPILER_IS_BORLAND */
                       pointer_iterator <   value_type
                                        ,   pointer
                                        ,   reference
                                        >::iterator_type    iterator;
    typedef
#if !defined(__STLSOFT_COMPILER_IS_BORLAND)
         ss_typename_type_k
#endif /* __STLSOFT_COMPILER_IS_BORLAND */
                       pointer_iterator <   value_type const
                                        ,   const_pointer
                                        ,   const_reference
                                        >::iterator_type    const_iterator;

#if defined(__STLSOFT_CF_BIDIRECTIONAL_ITERATOR_SUPPORT)
    typedef reverse_iterator_base       <   iterator
                                        ,   value_type
                                        ,   reference
                                        ,   pointer
                                        ,   difference_type
                                        >                   reverse_iterator;

    typedef const_reverse_iterator_base <   const_iterator
                                        ,   value_type const
                                        ,   const_reference
                                        ,   const_pointer
                                        ,   difference_type
                                        >                   const_reverse_iterator;
#endif /* __STLSOFT_CF_BIDIRECTIONAL_ITERATOR_SUPPORT */

// Construction
private:
                  fixed_array_1d(T *data, index_type d0);
public:
    ss_explicit_k fixed_array_1d(index_type d0);
                  fixed_array_1d(index_type d0, value_type const &t);
                 ~fixed_array_1d() stlsoft_throw_0();

// Access
public:
    reference               at(index_type i0);
    const_reference         at(index_type i0) const;

    reference               at_unchecked(index_type i0);
    const_reference         at_unchecked(index_type i0) const;

    reference               operator [](index_type i0);
    const_reference         operator [](index_type i0) const;

    /// Providing the evil operator, in order to support &ar[0]
//    pointer                 operator &();
    /// Providing the evil operator, in order to support &ar[0]
//    const_pointer           operator &() const;

    reference               front();
    reference               back();
    const_reference         front() const;
    const_reference         back() const;

// State
public:
    index_type              dimension0() const;
    index_type              size() const;
    ss_bool_t               empty() const;
    static size_type        max_size();

// Iteration
public:
    iterator                begin();
    iterator                end();
    const_iterator          begin() const;
    const_iterator          end() const;

#if defined(__STLSOFT_CF_BIDIRECTIONAL_ITERATOR_SUPPORT)
    reverse_iterator        rbegin();
    reverse_iterator        rend();
    const_reverse_iterator  rbegin() const;
    const_reverse_iterator  rend() const;
#endif /* __STLSOFT_CF_BIDIRECTIONAL_ITERATOR_SUPPORT */

// Access
public:
    value_type const        *data() const;

// Implementation
private:
    pointer     allocate_(size_type n);
    void        deallocate_(pointer p, size_type n);

    pointer     data_();
    index_type  calc_index_(index_type i0) const;
    void        range_check_(index_type i0) const stlsoft_throw_1(stlsoft_ns_qual_std(out_of_range) );

// Members
private:
    T * const           m_data;
    const index_type    m_d0;

    friend class fixed_array_2d<T, A, P, true>;
    friend class fixed_array_2d<T, A, P, false>;

// Not to be implemented
private:
    fixed_array_1d(class_type const &rhs);
    class_type const &operator =(class_type const &rhs);
};

// class fixed_array_2d
/// 2 dimensional fixed array
///
/// \param T The value type
/// \param A The allocator type
/// \param P The construction policy type
template<   ss_typename_param_k T
#ifdef __STLSOFT_CF_TEMPLATE_CLASS_DEFAULT_CLASS_ARGUMENT_SUPPORT
        ,   ss_typename_param_k A = new_allocator<T>
        ,   ss_typename_param_k P = do_construction<T>
        ,   ss_bool_t           R = true
#else
        ,   ss_typename_param_k A
        ,   ss_typename_param_k P
        ,   ss_bool_t           R
#endif /* __STLSOFT_CF_TEMPLATE_CLASS_DEFAULT_CLASS_ARGUMENT_SUPPORT */
        >
class fixed_array_2d
#ifdef STLSOFT_FIXED_ARRAY_USES_CONTAINER_BASE
    : public container_base<A>
#else /* ? STLSOFT_FIXED_ARRAY_USES_CONTAINER_BASE */
    : protected A
#endif /* STLSOFT_FIXED_ARRAY_USES_CONTAINER_BASE */
{
public:
    typedef fixed_array_2d<T, A, P, R>                      class_type;
    typedef fixed_array_1d<T, A, P, false>                  dimension_element_type;
#ifdef STLSOFT_FIXED_ARRAY_USES_CONTAINER_BASE
    typedef ss_typename_type_k container_base<A>::allocator_type    allocator_type;
    typedef P                                               initialisation_policy_type;
#else /* ? STLSOFT_FIXED_ARRAY_USES_CONTAINER_BASE */
    typedef A                                               allocator_type;
#endif /* STLSOFT_FIXED_ARRAY_USES_CONTAINER_BASE */

public:
    typedef T                                               value_type;
    typedef value_type                                      &reference;
    typedef value_type const                                &const_reference;
    typedef value_type                                      *pointer;
    typedef value_type const                                *const_pointer;
    typedef ss_size_t                                       size_type;
    typedef ss_size_t                                       index_type;
    typedef ss_ptrdiff_t                                    difference_type;

    typedef
#if !defined(__STLSOFT_COMPILER_IS_BORLAND)
           ss_typename_type_k
#endif /* __STLSOFT_COMPILER_IS_BORLAND */
                       pointer_iterator <   value_type
                                        ,   pointer
                                        ,   reference
                                        >::iterator_type    iterator;
    typedef
#if !defined(__STLSOFT_COMPILER_IS_BORLAND)
         ss_typename_type_k
#endif /* __STLSOFT_COMPILER_IS_BORLAND */
                       pointer_iterator <   value_type const
                                        ,   const_pointer
                                        ,   const_reference
                                        >::iterator_type    const_iterator;

#if defined(__STLSOFT_CF_BIDIRECTIONAL_ITERATOR_SUPPORT)
    typedef reverse_iterator_base       <   iterator
                                        ,   value_type
                                        ,   reference
                                        ,   pointer
                                        ,   difference_type
                                        >                   reverse_iterator;

    typedef const_reverse_iterator_base <   const_iterator
                                        ,   value_type const
                                        ,   const_reference
                                        ,   const_pointer
                                        ,   difference_type
                                        >                   const_reverse_iterator;
#endif /* __STLSOFT_CF_BIDIRECTIONAL_ITERATOR_SUPPORT */

// Construction
private:
    fixed_array_2d(T *data, index_type d0, index_type d1);
public:
    fixed_array_2d(index_type d0, index_type d1);
    fixed_array_2d(index_type d0, index_type d1, value_type const &t);
    ~fixed_array_2d() stlsoft_throw_0();

// Access
public:
    reference               at(index_type i0, index_type i1);
    const_reference         at(index_type i0, index_type i1) const;

    reference               at_unchecked(index_type i0, index_type i1);
    const_reference         at_unchecked(index_type i0, index_type i1) const;

    dimension_element_type          at(index_type i0);
    const dimension_element_type    at(index_type i0) const;

    dimension_element_type          at_unchecked(index_type i0);
    const dimension_element_type    at_unchecked(index_type i0) const;

    dimension_element_type          operator [](index_type i0);
    const dimension_element_type    operator [](index_type i0) const;

    /// A reference to the first element in the array
    reference               front();
    reference               back();
    const_reference         front() const;
    const_reference         back() const;

// State
public:
    index_type              dimension0() const;
    index_type              dimension1() const;
    index_type              size() const;
    ss_bool_t               empty() const;
    static size_type        max_size();

// Iteration
public:
    iterator                begin();
    iterator                end();
    const_iterator          begin() const;
    const_iterator          end() const;

#if defined(__STLSOFT_CF_BIDIRECTIONAL_ITERATOR_SUPPORT)
    reverse_iterator        rbegin();
    reverse_iterator        rend();
    const_reverse_iterator  rbegin() const;
    const_reverse_iterator  rend() const;
#endif /* __STLSOFT_CF_BIDIRECTIONAL_ITERATOR_SUPPORT */

// Access
public:
    value_type const        *data() const;

// Implementation
private:
    pointer     allocate_(size_type n);
    void        deallocate_(pointer p, size_type n);

    pointer     data_();
    index_type  calc_index_(index_type i0, index_type i1) const;
    void        range_check_(index_type i0, index_type i1) const stlsoft_throw_1(stlsoft_ns_qual_std(out_of_range) );
    void        range_check_(index_type i0) const stlsoft_throw_1(stlsoft_ns_qual_std(out_of_range) );

// Members
private:
    T * const           m_data;
    const index_type    m_d0;
    const index_type    m_d1;
    const size_type     m_size;

    friend class fixed_array_3d<T, A, P, true>;
    friend class fixed_array_3d<T, A, P, false>;

// Not to be implemented
private:
    fixed_array_2d(class_type const &rhs);
    class_type const &operator =(class_type const &rhs);
};

// class fixed_array_3d
/// 3 dimensional fixed array
///
/// \param T The value type
/// \param A The allocator type
/// \param P The construction policy type
template<   ss_typename_param_k T
#ifdef __STLSOFT_CF_TEMPLATE_CLASS_DEFAULT_CLASS_ARGUMENT_SUPPORT
        ,   ss_typename_param_k A = new_allocator<T>
        ,   ss_typename_param_k P = do_construction<T>
        ,   ss_bool_t           R = true
#else
        ,   ss_typename_param_k A
        ,   ss_typename_param_k P
        ,   ss_bool_t           R
#endif /* __STLSOFT_CF_TEMPLATE_CLASS_DEFAULT_CLASS_ARGUMENT_SUPPORT */
        >
class fixed_array_3d
#ifdef STLSOFT_FIXED_ARRAY_USES_CONTAINER_BASE
    : public container_base<A>
#else /* ? STLSOFT_FIXED_ARRAY_USES_CONTAINER_BASE */
    : protected A
#endif /* STLSOFT_FIXED_ARRAY_USES_CONTAINER_BASE */
{
public:
    typedef fixed_array_3d<T, A, P, R>                      class_type;
    typedef fixed_array_2d<T, A, P, false>                  dimension_element_type;
#ifdef STLSOFT_FIXED_ARRAY_USES_CONTAINER_BASE
    typedef ss_typename_type_k container_base<A>::allocator_type    allocator_type;
    typedef P                                               initialisation_policy_type;
#else /* ? STLSOFT_FIXED_ARRAY_USES_CONTAINER_BASE */
    typedef A                                               allocator_type;
#endif /* STLSOFT_FIXED_ARRAY_USES_CONTAINER_BASE */

public:
    typedef T                                               value_type;
    typedef value_type                                      &reference;
    typedef value_type const                                &const_reference;
    typedef value_type                                      *pointer;
    typedef value_type const                                *const_pointer;
    typedef ss_size_t                                       size_type;
    typedef ss_size_t                                       index_type;
    typedef ss_ptrdiff_t                                    difference_type;

    typedef
#if !defined(__STLSOFT_COMPILER_IS_BORLAND)
           ss_typename_type_k
#endif /* __STLSOFT_COMPILER_IS_BORLAND */
                       pointer_iterator <   value_type
                                        ,   pointer
                                        ,   reference
                                        >::iterator_type    iterator;
    typedef
#if !defined(__STLSOFT_COMPILER_IS_BORLAND)
         ss_typename_type_k
#endif /* __STLSOFT_COMPILER_IS_BORLAND */
                       pointer_iterator <   value_type const
                                        ,   const_pointer
                                        ,   const_reference
                                        >::iterator_type    const_iterator;

#if defined(__STLSOFT_CF_BIDIRECTIONAL_ITERATOR_SUPPORT)
    typedef reverse_iterator_base       <   iterator
                                        ,   value_type
                                        ,   reference
                                        ,   pointer
                                        ,   difference_type
                                        >                   reverse_iterator;

    typedef const_reverse_iterator_base <   const_iterator
                                        ,   value_type const
                                        ,   const_reference
                                        ,   const_pointer
                                        ,   difference_type
                                        >                   const_reverse_iterator;
#endif /* __STLSOFT_CF_BIDIRECTIONAL_ITERATOR_SUPPORT */

// Construction
private:
    fixed_array_3d(pointer data, index_type d0, index_type d1, index_type d2);
public:
    fixed_array_3d(index_type d0, index_type d1, index_type d2);
    fixed_array_3d(index_type d0, index_type d1, index_type d2, value_type const &t);
    ~fixed_array_3d() stlsoft_throw_0();

// Access
public:
    reference               at(index_type i0, index_type i1, index_type i2);
    const_reference         at(index_type i0, index_type i1, index_type i3) const;

    reference               at_unchecked(index_type i0, index_type i1, index_type i2);
    const_reference         at_unchecked(index_type i0, index_type i1, index_type i2) const;

    dimension_element_type          at(index_type i0);
    const dimension_element_type    at(index_type i0) const;

    dimension_element_type          at_unchecked(index_type i0);
    const dimension_element_type    at_unchecked(index_type i0) const;

    dimension_element_type          operator [](index_type i0);
    const dimension_element_type    operator [](index_type i0) const;

    reference               front();
    reference               back();
    const_reference         front() const;
    const_reference         back() const;

// State
public:
    index_type              dimension0() const;
    index_type              dimension1() const;
    index_type              dimension2() const;
    index_type              size() const;
    ss_bool_t               empty() const;
    static size_type        max_size();

// Iteration
public:
    iterator                begin();
    iterator                end();
    const_iterator          begin() const;
    const_iterator          end() const;

#if defined(__STLSOFT_CF_BIDIRECTIONAL_ITERATOR_SUPPORT)
    reverse_iterator        rbegin();
    reverse_iterator        rend();
    const_reverse_iterator  rbegin() const;
    const_reverse_iterator  rend() const;
#endif /* __STLSOFT_CF_BIDIRECTIONAL_ITERATOR_SUPPORT */

// Access
public:
    value_type const        *data() const;

// Implementation
private:
    pointer     allocate_(size_type n);
    void        deallocate_(pointer p, size_type n);

    pointer     data_();
    index_type  calc_index_(index_type i0, index_type i1, index_type i2) const;
    void        range_check_(index_type i0, index_type i1, index_type i2) const stlsoft_throw_1(stlsoft_ns_qual_std(out_of_range) );
    void        range_check_(index_type i0) const stlsoft_throw_1(stlsoft_ns_qual_std(out_of_range) );

// Members
private:
    T * const           m_data;
    const index_type    m_d0;
    const index_type    m_d1;
    const index_type    m_d2;

    friend class fixed_array_4d<T, A, P, true>;
    friend class fixed_array_4d<T, A, P, false>;

// Not to be implemented
private:
    fixed_array_3d(class_type const &rhs);
    class_type const &operator =(class_type const &rhs);
};


// class fixed_array_4d
/// 4 dimensional fixed array
///
/// \param T The value type
/// \param A The allocator type
/// \param P The construction policy type
template<   ss_typename_param_k T
#ifdef __STLSOFT_CF_TEMPLATE_CLASS_DEFAULT_CLASS_ARGUMENT_SUPPORT
        ,   ss_typename_param_k A = new_allocator<T>
        ,   ss_typename_param_k P = do_construction<T>
        ,   ss_bool_t           R = true
#else
        ,   ss_typename_param_k A
        ,   ss_typename_param_k P
        ,   ss_bool_t           R
#endif /* __STLSOFT_CF_TEMPLATE_CLASS_DEFAULT_CLASS_ARGUMENT_SUPPORT */
        >
class fixed_array_4d
#ifdef STLSOFT_FIXED_ARRAY_USES_CONTAINER_BASE
    : public container_base<A>
#else /* ? STLSOFT_FIXED_ARRAY_USES_CONTAINER_BASE */
    : protected A
#endif /* STLSOFT_FIXED_ARRAY_USES_CONTAINER_BASE */
{
public:
    typedef fixed_array_4d<T, A, P, R>                      class_type;
    typedef fixed_array_3d<T, A, P, false>                  dimension_element_type;
#ifdef STLSOFT_FIXED_ARRAY_USES_CONTAINER_BASE
    typedef ss_typename_type_k container_base<A>::allocator_type    allocator_type;
    typedef P                                               initialisation_policy_type;
#else /* ? STLSOFT_FIXED_ARRAY_USES_CONTAINER_BASE */
    typedef A                                               allocator_type;
#endif /* STLSOFT_FIXED_ARRAY_USES_CONTAINER_BASE */

public:
    typedef T                                               value_type;
    typedef value_type                                      &reference;
    typedef value_type const                                &const_reference;
    typedef value_type                                      *pointer;
    typedef value_type const                                *const_pointer;
    typedef ss_size_t                                       size_type;
    typedef ss_size_t                                       index_type;
    typedef ss_ptrdiff_t                                    difference_type;

    typedef
#if !defined(__STLSOFT_COMPILER_IS_BORLAND)
           ss_typename_type_k
#endif /* __STLSOFT_COMPILER_IS_BORLAND */
                       pointer_iterator <   value_type
                                        ,   pointer
                                        ,   reference
                                        >::iterator_type    iterator;
    typedef
#if !defined(__STLSOFT_COMPILER_IS_BORLAND)
         ss_typename_type_k
#endif /* __STLSOFT_COMPILER_IS_BORLAND */
                       pointer_iterator <   value_type const
                                        ,   const_pointer
                                        ,   const_reference
                                        >::iterator_type    const_iterator;

#if defined(__STLSOFT_CF_BIDIRECTIONAL_ITERATOR_SUPPORT)
    typedef reverse_iterator_base       <   iterator
                                        ,   value_type
                                        ,   reference
                                        ,   pointer
                                        ,   difference_type
                                        >                   reverse_iterator;

    typedef const_reverse_iterator_base <   const_iterator
                                        ,   value_type const
                                        ,   const_reference
                                        ,   const_pointer
                                        ,   difference_type
                                        >                   const_reverse_iterator;
#endif /* __STLSOFT_CF_BIDIRECTIONAL_ITERATOR_SUPPORT */

// Construction
private:
    fixed_array_4d(T *data, index_type d0, index_type d1, index_type d2, index_type d3);
public:
    fixed_array_4d(index_type d0, index_type d1, index_type d2, index_type d3);
    fixed_array_4d(index_type d0, index_type d1, index_type d2, index_type d3, value_type const &t);
    ~fixed_array_4d() stlsoft_throw_0();

// Access
public:
    reference               at(index_type i0, index_type i1, index_type i2, index_type i3);
    const_reference         at(index_type i0, index_type i1, index_type i2, index_type i3) const;

    reference               at_unchecked(index_type i0, index_type i1, index_type i2, index_type i3);
    const_reference         at_unchecked(index_type i0, index_type i1, index_type i2, index_type i3) const;

    dimension_element_type          at(index_type i0);
    const dimension_element_type    at(index_type i0) const;

    dimension_element_type          at_unchecked(index_type i0);
    const dimension_element_type    at_unchecked(index_type i0) const;

    dimension_element_type          operator [](index_type i0);
    const dimension_element_type    operator [](index_type i0) const;

    reference               front();
    reference               back();
    const_reference         front() const;
    const_reference         back() const;

// State
public:
    index_type              dimension0() const;
    index_type              dimension1() const;
    index_type              dimension2() const;
    index_type              dimension3() const;
    index_type              size() const;
    ss_bool_t               empty() const;
    static size_type        max_size();

// Iteration
public:
    iterator                begin();
    iterator                end();
    const_iterator          begin() const;
    const_iterator          end() const;

#if defined(__STLSOFT_CF_BIDIRECTIONAL_ITERATOR_SUPPORT)
    reverse_iterator        rbegin();
    reverse_iterator        rend();
    const_reverse_iterator  rbegin() const;
    const_reverse_iterator  rend() const;
#endif /* __STLSOFT_CF_BIDIRECTIONAL_ITERATOR_SUPPORT */

// Access
public:
    value_type const        *data() const;

// Implementation
private:
    pointer     allocate_(size_type n);
    void        deallocate_(pointer p, size_type n);

    pointer     data_();
    index_type  calc_index_(index_type i0, index_type i1, index_type i2, index_type i3) const;
    void        range_check_(index_type i0, index_type i1, index_type i2, index_type i3) const stlsoft_throw_1(stlsoft_ns_qual_std(out_of_range) );
    void        range_check_(index_type i0) const stlsoft_throw_1(stlsoft_ns_qual_std(out_of_range) );

// Members
private:
    T * const           m_data;
    const index_type    m_d0;
    const index_type    m_d1;
    const index_type    m_d2;
    const index_type    m_d3;

    friend class fixed_array_5d<T, A, P, true>;
    friend class fixed_array_5d<T, A, P, false>;

// Not to be implemented
private:
    fixed_array_4d(class_type const &rhs);
    class_type const &operator =(class_type const &rhs);
};

////////////////////////////////////////////////////////////////////////////////
// Unit-testing

#ifdef STLSOFT_UNITTEST

namespace unittest
{
    ss_bool_t test_stlsoft_fixed_array(unittest_reporter *r)
    {
        ss_bool_t               bSuccess    =   true;

        unittest_initialiser    init(r, "STLSoft", "fixed_array", __FILE__);

#if 0
        if(<<TODO>>)
        {
            r->report("<<TODO>> failed ", __LINE__);
            bSuccess = false;
        }
#endif /* 0 */

        return bSuccess;
    }

    unittest_registrar    unittest_stlsoft_fixed_array(test_stlsoft_fixed_array);

} // namespace unittest

#endif /* STLSOFT_UNITTEST */

/* /////////////////////////////////////////////////////////////////////////////
 * Implementation
 */

#ifndef __STLSOFT_DOCUMENTATION_SKIP_SECTION

// fixed_array_1d

template <ss_typename_param_k T, ss_typename_param_k A, ss_typename_param_k P, ss_bool_t R>
inline ss_typename_type_k fixed_array_1d<T, A, P, R>::pointer fixed_array_1d<T, A, P, R>::allocate_(ss_typename_type_k fixed_array_1d<T, A, P, R>::size_type n)
{
    allocator_type  &ator = *this;

    return ator.allocate(n, NULL);
}

template <ss_typename_param_k T, ss_typename_param_k A, ss_typename_param_k P, ss_bool_t R>
inline void fixed_array_1d<T, A, P, R>::deallocate_(ss_typename_type_k fixed_array_1d<T, A, P, R>::pointer p, ss_typename_type_k fixed_array_1d<T, A, P, R>::size_type n)
{
    allocator_type  &ator = *this;

    ator.deallocate(p, n);
}

template <ss_typename_param_k T, ss_typename_param_k A, ss_typename_param_k P, ss_bool_t R>
inline ss_typename_type_k fixed_array_1d<T, A, P, R>::pointer fixed_array_1d<T, A, P, R>::data_()
{
    return m_data;
}

template <ss_typename_param_k T, ss_typename_param_k A, ss_typename_param_k P, ss_bool_t R>
inline ss_typename_type_k fixed_array_1d<T, A, P, R>::index_type fixed_array_1d<T, A, P, R>::calc_index_(ss_typename_type_k fixed_array_1d<T, A, P, R>::index_type i0) const
{
    return i0;
}

template <ss_typename_param_k T, ss_typename_param_k A, ss_typename_param_k P, ss_bool_t R>
inline void fixed_array_1d<T, A, P, R>::range_check_(ss_typename_type_k fixed_array_1d<T, A, P, R>::index_type i0) const stlsoft_throw_1(stlsoft_ns_qual_std(out_of_range) )
{
#ifdef __STLSOFT_CF_EXCEPTION_SUPPORT
    if(!(i0 < m_d0))
    {
        stlsoft_ns_qual_std(out_of_range)   x("fixed array index out of range");

#ifdef __STLSOFT_COMPILER_IS_MSVC
        if(1) // This is needed to avoid Visual C++ warning 4702: 'unreachable code'
#endif /* __STLSOFT_COMPILER_IS_MSVC */
        {
            throw x;
        }
    }
#else
    stlsoft_message_assert("fixed array index out of range", i0 < m_d0);
#endif /* __STLSOFT_CF_EXCEPTION_SUPPORT */
}

template <ss_typename_param_k T, ss_typename_param_k A, ss_typename_param_k P, ss_bool_t R>
inline fixed_array_1d<T, A, P, R>::fixed_array_1d(T *data, ss_typename_type_k fixed_array_1d<T, A, P, R>::index_type d0)
    : m_data(data)
    , m_d0(d0)
{
    stlsoft_static_assert(!R);
}

template <ss_typename_param_k T, ss_typename_param_k A, ss_typename_param_k P, ss_bool_t R>
inline fixed_array_1d<T, A, P, R>::fixed_array_1d(ss_typename_type_k fixed_array_1d<T, A, P, R>::index_type d0)
    : m_data(allocate_(d0))
    , m_d0(d0)
{
    stlsoft_static_assert(R);
    array_range_initialiser<T, A, P>::construct(*this, data_(), size());
}

template <ss_typename_param_k T, ss_typename_param_k A, ss_typename_param_k P, ss_bool_t R>
inline fixed_array_1d<T, A, P, R>::fixed_array_1d(ss_typename_type_k fixed_array_1d<T, A, P, R>::index_type d0, value_type const &t)
    : m_data(allocate_(d0))
    , m_d0(d0)
{
    stlsoft_static_assert(R);
    array_range_initialiser<T, A, P>::construct(*this, data_(), size(), t);
}

template <ss_typename_param_k T, ss_typename_param_k A, ss_typename_param_k P, ss_bool_t R>
inline fixed_array_1d<T, A, P, R>::~fixed_array_1d() stlsoft_throw_0()
{
    if(R)
    {
        array_range_initialiser<T, A, P>::destroy(*this, data_(), size());
        deallocate_(m_data, size());
    }
}

template <ss_typename_param_k T, ss_typename_param_k A, ss_typename_param_k P, ss_bool_t R>
inline ss_typename_type_k fixed_array_1d<T, A, P, R>::reference fixed_array_1d<T, A, P, R>::at(ss_typename_type_k fixed_array_1d<T, A, P, R>::index_type i0)
{
    range_check_(i0);

    return m_data[i0];
}

template <ss_typename_param_k T, ss_typename_param_k A, ss_typename_param_k P, ss_bool_t R>
inline ss_typename_type_k fixed_array_1d<T, A, P, R>::const_reference fixed_array_1d<T, A, P, R>::at(ss_typename_type_k fixed_array_1d<T, A, P, R>::index_type i0) const
{
    range_check_(i0);

    return m_data[i0];
}

template <ss_typename_param_k T, ss_typename_param_k A, ss_typename_param_k P, ss_bool_t R>
inline ss_typename_type_k fixed_array_1d<T, A, P, R>::reference fixed_array_1d<T, A, P, R>::at_unchecked(ss_typename_type_k fixed_array_1d<T, A, P, R>::index_type i0)
{
    stlsoft_message_assert("fixed array index out of range", i0 < m_d0);

    return m_data[i0];
}

template <ss_typename_param_k T, ss_typename_param_k A, ss_typename_param_k P, ss_bool_t R>
inline ss_typename_type_k fixed_array_1d<T, A, P, R>::const_reference fixed_array_1d<T, A, P, R>::at_unchecked(ss_typename_type_k fixed_array_1d<T, A, P, R>::index_type i0) const
{
    stlsoft_message_assert("fixed array index out of range", i0 < m_d0);

    return m_data[i0];
}

template <ss_typename_param_k T, ss_typename_param_k A, ss_typename_param_k P, ss_bool_t R>
inline ss_typename_type_k fixed_array_1d<T, A, P, R>::reference fixed_array_1d<T, A, P, R>::operator [](ss_typename_type_k fixed_array_1d<T, A, P, R>::index_type i0)
{
    stlsoft_message_assert("fixed array index out of range", i0 < m_d0);

    return m_data[i0];
}

template <ss_typename_param_k T, ss_typename_param_k A, ss_typename_param_k P, ss_bool_t R>
inline ss_typename_type_k fixed_array_1d<T, A, P, R>::const_reference fixed_array_1d<T, A, P, R>::operator [](ss_typename_type_k fixed_array_1d<T, A, P, R>::index_type i0) const
{
    stlsoft_message_assert("fixed array index out of range", i0 < m_d0);

    return m_data[i0];
}

#if 0
template <ss_typename_param_k T, ss_typename_param_k A, ss_typename_param_k P, ss_bool_t R>
inline ss_typename_type_k fixed_array_1d<T, A, P, R>::pointer fixed_array_1d<T, A, P, R>::operator &()
{
    return &m_data[0];
}

template <ss_typename_param_k T, ss_typename_param_k A, ss_typename_param_k P, ss_bool_t R>
inline ss_typename_type_k fixed_array_1d<T, A, P, R>::const_pointer fixed_array_1d<T, A, P, R>::operator &() const
{
    return &m_data[0];
}
#endif /* 0 */

template <ss_typename_param_k T, ss_typename_param_k A, ss_typename_param_k P, ss_bool_t R>
inline ss_typename_type_k fixed_array_1d<T, A, P, R>::reference fixed_array_1d<T, A, P, R>::front()
{
    return at(0);
}

template <ss_typename_param_k T, ss_typename_param_k A, ss_typename_param_k P, ss_bool_t R>
inline ss_typename_type_k fixed_array_1d<T, A, P, R>::reference fixed_array_1d<T, A, P, R>::back()
{
    return at(m_d0 - 1);
}

template <ss_typename_param_k T, ss_typename_param_k A, ss_typename_param_k P, ss_bool_t R>
inline ss_typename_type_k fixed_array_1d<T, A, P, R>::const_reference fixed_array_1d<T, A, P, R>::front() const
{
    return at(0);
}

template <ss_typename_param_k T, ss_typename_param_k A, ss_typename_param_k P, ss_bool_t R>
inline ss_typename_type_k fixed_array_1d<T, A, P, R>::const_reference fixed_array_1d<T, A, P, R>::back() const
{
    return at(m_d0 - 1);
}

template <ss_typename_param_k T, ss_typename_param_k A, ss_typename_param_k P, ss_bool_t R>
inline ss_typename_type_k fixed_array_1d<T, A, P, R>::index_type fixed_array_1d<T, A, P, R>::dimension0() const
{
    return m_d0;
}

template <ss_typename_param_k T, ss_typename_param_k A, ss_typename_param_k P, ss_bool_t R>
inline ss_typename_type_k fixed_array_1d<T, A, P, R>::index_type fixed_array_1d<T, A, P, R>::size() const
{
    return m_d0;
}

template <ss_typename_param_k T, ss_typename_param_k A, ss_typename_param_k P, ss_bool_t R>
inline ss_bool_t fixed_array_1d<T, A, P, R>::empty() const
{
    return false;
}

template <ss_typename_param_k T, ss_typename_param_k A, ss_typename_param_k P, ss_bool_t R>
inline /* static */ ss_typename_type_k fixed_array_1d<T, A, P, R>::size_type fixed_array_1d<T, A, P, R>::max_size()
{
    return static_cast<size_type>(-1) / sizeof(T);
}

template <ss_typename_param_k T, ss_typename_param_k A, ss_typename_param_k P, ss_bool_t R>
inline ss_typename_type_k fixed_array_1d<T, A, P, R>::iterator fixed_array_1d<T, A, P, R>::begin()
{
    return m_data;
}

template <ss_typename_param_k T, ss_typename_param_k A, ss_typename_param_k P, ss_bool_t R>
inline ss_typename_type_k fixed_array_1d<T, A, P, R>::iterator fixed_array_1d<T, A, P, R>::end()
{
    return m_data + size();
}

template <ss_typename_param_k T, ss_typename_param_k A, ss_typename_param_k P, ss_bool_t R>
inline ss_typename_type_k fixed_array_1d<T, A, P, R>::const_iterator fixed_array_1d<T, A, P, R>::begin() const
{
    return m_data;
}

template <ss_typename_param_k T, ss_typename_param_k A, ss_typename_param_k P, ss_bool_t R>
inline ss_typename_type_k fixed_array_1d<T, A, P, R>::const_iterator fixed_array_1d<T, A, P, R>::end() const
{
    return m_data + size();
}

#if defined(__STLSOFT_CF_BIDIRECTIONAL_ITERATOR_SUPPORT)

template <ss_typename_param_k T, ss_typename_param_k A, ss_typename_param_k P, ss_bool_t R>
inline ss_typename_type_k fixed_array_1d<T, A, P, R>::reverse_iterator fixed_array_1d<T, A, P, R>::rbegin()
{
    return reverse_iterator(end());
}

template <ss_typename_param_k T, ss_typename_param_k A, ss_typename_param_k P, ss_bool_t R>
inline ss_typename_type_k fixed_array_1d<T, A, P, R>::reverse_iterator fixed_array_1d<T, A, P, R>::rend()
{
    return reverse_iterator(begin());
}

template <ss_typename_param_k T, ss_typename_param_k A, ss_typename_param_k P, ss_bool_t R>
inline ss_typename_type_k fixed_array_1d<T, A, P, R>::const_reverse_iterator fixed_array_1d<T, A, P, R>::rbegin() const
{
    return const_reverse_iterator(end());
}

template <ss_typename_param_k T, ss_typename_param_k A, ss_typename_param_k P, ss_bool_t R>
inline ss_typename_type_k fixed_array_1d<T, A, P, R>::const_reverse_iterator fixed_array_1d<T, A, P, R>::rend() const
{
    return const_reverse_iterator(begin());
}

#endif /* __STLSOFT_CF_BIDIRECTIONAL_ITERATOR_SUPPORT */

template <ss_typename_param_k T, ss_typename_param_k A, ss_typename_param_k P, ss_bool_t R>
inline ss_typename_type_k fixed_array_1d<T, A, P, R>::value_type const *fixed_array_1d<T, A, P, R>::data() const
{
    return m_data;
}


// fixed_array_2d

template <ss_typename_param_k T, ss_typename_param_k A, ss_typename_param_k P, ss_bool_t R>
inline ss_typename_type_k fixed_array_2d<T, A, P, R>::pointer fixed_array_2d<T, A, P, R>::allocate_(ss_typename_type_k fixed_array_2d<T, A, P, R>::size_type n)
{
    allocator_type  &ator = *this;

    return ator.allocate(n, NULL);
}

template <ss_typename_param_k T, ss_typename_param_k A, ss_typename_param_k P, ss_bool_t R>
inline void fixed_array_2d<T, A, P, R>::deallocate_(ss_typename_type_k fixed_array_2d<T, A, P, R>::pointer p, ss_typename_type_k fixed_array_2d<T, A, P, R>::size_type n)
{
    allocator_type  &ator = *this;

    ator.deallocate(p, n);
}

template <ss_typename_param_k T, ss_typename_param_k A, ss_typename_param_k P, ss_bool_t R>
inline ss_typename_type_k fixed_array_2d<T, A, P, R>::pointer fixed_array_2d<T, A, P, R>::data_()
{
    return m_data;
}

template <ss_typename_param_k T, ss_typename_param_k A, ss_typename_param_k P, ss_bool_t R>
inline ss_typename_type_k fixed_array_2d<T, A, P, R>::index_type fixed_array_2d<T, A, P, R>::calc_index_(ss_typename_type_k fixed_array_2d<T, A, P, R>::index_type i0, ss_typename_type_k fixed_array_2d<T, A, P, R>::index_type i1) const
{
    return (i0 * m_d1) + i1;
}

template <ss_typename_param_k T, ss_typename_param_k A, ss_typename_param_k P, ss_bool_t R>
inline void fixed_array_2d<T, A, P, R>::range_check_(ss_typename_type_k fixed_array_2d<T, A, P, R>::index_type i0, ss_typename_type_k fixed_array_2d<T, A, P, R>::index_type i1) const stlsoft_throw_1(stlsoft_ns_qual_std(out_of_range) )
{
#ifdef __STLSOFT_CF_EXCEPTION_SUPPORT
    if( !(i0 < m_d0) ||
        !(i1 < m_d1))
    {
        stlsoft_ns_qual_std(out_of_range)   x("fixed array index out of range");

#ifdef __STLSOFT_COMPILER_IS_MSVC
        if(1) // This is needed to avoid Visual C++ warning 4702: 'unreachable code'
#endif /* __STLSOFT_COMPILER_IS_MSVC */
        {
            throw x;
        }
    }
#else
    stlsoft_message_assert("fixed array index out of range", i0 < m_d0 && i1 < m_d1);
#endif /* __STLSOFT_CF_EXCEPTION_SUPPORT */
}

template <ss_typename_param_k T, ss_typename_param_k A, ss_typename_param_k P, ss_bool_t R>
inline void fixed_array_2d<T, A, P, R>::range_check_(ss_typename_type_k fixed_array_2d<T, A, P, R>::index_type i0) const stlsoft_throw_1(stlsoft_ns_qual_std(out_of_range) )
{
#ifdef __STLSOFT_CF_EXCEPTION_SUPPORT
    if(!(i0 < m_d0))
    {
        stlsoft_ns_qual_std(out_of_range)   x("fixed array index out of range");

#ifdef __STLSOFT_COMPILER_IS_MSVC
        if(1) // This is needed to avoid Visual C++ warning 4702: 'unreachable code'
#endif /* __STLSOFT_COMPILER_IS_MSVC */
        {
            throw x;
        }
    }
#else
    stlsoft_message_assert("fixed array index out of range", i0 < m_d0);
#endif /* __STLSOFT_CF_EXCEPTION_SUPPORT */
}

template <ss_typename_param_k T, ss_typename_param_k A, ss_typename_param_k P, ss_bool_t R>
inline fixed_array_2d<T, A, P, R>::fixed_array_2d(T *data, ss_typename_type_k fixed_array_2d<T, A, P, R>::index_type d0, ss_typename_type_k fixed_array_2d<T, A, P, R>::index_type d1)
    : m_data(data)
    , m_d0(d0)
    , m_d1(d1)
    , m_size(d0 * d1)
{
    stlsoft_static_assert(!R);
}

template <ss_typename_param_k T, ss_typename_param_k A, ss_typename_param_k P, ss_bool_t R>
inline fixed_array_2d<T, A, P, R>::fixed_array_2d(ss_typename_type_k fixed_array_2d<T, A, P, R>::index_type d0, ss_typename_type_k fixed_array_2d<T, A, P, R>::index_type d1)
    : m_data(allocate_(d0 * d1))
    , m_d0(d0)
    , m_d1(d1)
    , m_size(d0 * d1)
{
    stlsoft_static_assert(R);
    array_range_initialiser<T, A, P>::construct(*this, data_(), size());
}

template <ss_typename_param_k T, ss_typename_param_k A, ss_typename_param_k P, ss_bool_t R>
inline fixed_array_2d<T, A, P, R>::fixed_array_2d(ss_typename_type_k fixed_array_2d<T, A, P, R>::index_type d0, ss_typename_type_k fixed_array_2d<T, A, P, R>::index_type d1, value_type const &t)
    : m_data(allocate_(d0 * d1))
    , m_d0(d0)
    , m_d1(d1)
    , m_size(d0 * d1)
{
    stlsoft_static_assert(R);
    array_range_initialiser<T, A, P>::construct(*this, data_(), size(), t);
}

template <ss_typename_param_k T, ss_typename_param_k A, ss_typename_param_k P, ss_bool_t R>
inline fixed_array_2d<T, A, P, R>::~fixed_array_2d() stlsoft_throw_0()
{
    if(R)
    {
        array_range_initialiser<T, A, P>::destroy(*this, data_(), size());
        deallocate_(m_data, size());
    }
}

template <ss_typename_param_k T, ss_typename_param_k A, ss_typename_param_k P, ss_bool_t R>
inline ss_typename_type_k fixed_array_2d<T, A, P, R>::reference fixed_array_2d<T, A, P, R>::at(ss_typename_type_k fixed_array_2d<T, A, P, R>::index_type i0, ss_typename_type_k fixed_array_2d<T, A, P, R>::index_type i1)
{
    range_check_(i0, i1);

    return *(m_data + calc_index_(i0, i1));
}

template <ss_typename_param_k T, ss_typename_param_k A, ss_typename_param_k P, ss_bool_t R>
inline ss_typename_type_k fixed_array_2d<T, A, P, R>::const_reference fixed_array_2d<T, A, P, R>::at(ss_typename_type_k fixed_array_2d<T, A, P, R>::index_type i0, ss_typename_type_k fixed_array_2d<T, A, P, R>::index_type i1) const
{
    range_check_(i0, i1);

    return *(m_data + calc_index_(i0, i1));
}

template <ss_typename_param_k T, ss_typename_param_k A, ss_typename_param_k P, ss_bool_t R>
inline ss_typename_type_k fixed_array_2d<T, A, P, R>::reference fixed_array_2d<T, A, P, R>::at_unchecked(ss_typename_type_k fixed_array_2d<T, A, P, R>::index_type i0, ss_typename_type_k fixed_array_2d<T, A, P, R>::index_type i1)
{
    stlsoft_message_assert("fixed array index out of range", i0 < m_d0 && i1 < m_d1);

    return *(m_data + calc_index_(i0, i1));
}

template <ss_typename_param_k T, ss_typename_param_k A, ss_typename_param_k P, ss_bool_t R>
inline ss_typename_type_k fixed_array_2d<T, A, P, R>::const_reference fixed_array_2d<T, A, P, R>::at_unchecked(ss_typename_type_k fixed_array_2d<T, A, P, R>::index_type i0, ss_typename_type_k fixed_array_2d<T, A, P, R>::index_type i1) const
{
    stlsoft_message_assert("fixed array index out of range", i0 < m_d0 && i1 < m_d1);

    return *(m_data + calc_index_(i0, i1));
}

template <ss_typename_param_k T, ss_typename_param_k A, ss_typename_param_k P, ss_bool_t R>
inline ss_typename_type_k fixed_array_2d<T, A, P, R>::dimension_element_type fixed_array_2d<T, A, P, R>::at(ss_typename_type_k fixed_array_2d<T, A, P, R>::index_type i0)
{
    range_check_(i0);

    return dimension_element_type(m_data + i0 * m_d1, m_d1);
}

template <ss_typename_param_k T, ss_typename_param_k A, ss_typename_param_k P, ss_bool_t R>
inline ss_typename_type_k fixed_array_2d<T, A, P, R>::dimension_element_type const fixed_array_2d<T, A, P, R>::at(ss_typename_type_k fixed_array_2d<T, A, P, R>::index_type i0) const
{
    range_check_(i0);

    return dimension_element_type(m_data + i0 * m_d1, m_d1);
}

template <ss_typename_param_k T, ss_typename_param_k A, ss_typename_param_k P, ss_bool_t R>
inline ss_typename_type_k fixed_array_2d<T, A, P, R>::dimension_element_type fixed_array_2d<T, A, P, R>::at_unchecked(ss_typename_type_k fixed_array_2d<T, A, P, R>::index_type i0)
{
    stlsoft_message_assert("fixed array index out of range", i0 < m_d0);

    return dimension_element_type(m_data + i0 * m_d1, m_d1);
}

template <ss_typename_param_k T, ss_typename_param_k A, ss_typename_param_k P, ss_bool_t R>
inline ss_typename_type_k fixed_array_2d<T, A, P, R>::dimension_element_type const fixed_array_2d<T, A, P, R>::at_unchecked(ss_typename_type_k fixed_array_2d<T, A, P, R>::index_type i0) const
{
    stlsoft_message_assert("fixed array index out of range", i0 < m_d0);

    return dimension_element_type(m_data + i0 * m_d1, m_d1);
}

template <ss_typename_param_k T, ss_typename_param_k A, ss_typename_param_k P, ss_bool_t R>
inline ss_typename_type_k fixed_array_2d<T, A, P, R>::dimension_element_type fixed_array_2d<T, A, P, R>::operator [](ss_typename_type_k fixed_array_2d<T, A, P, R>::index_type i0)
{
    stlsoft_message_assert("fixed array index out of range", i0 < m_d0);

    return dimension_element_type(m_data + i0 * m_d1, m_d1);
}

template <ss_typename_param_k T, ss_typename_param_k A, ss_typename_param_k P, ss_bool_t R>
inline ss_typename_type_k fixed_array_2d<T, A, P, R>::dimension_element_type const fixed_array_2d<T, A, P, R>::operator [](ss_typename_type_k fixed_array_2d<T, A, P, R>::index_type i0) const
{
    stlsoft_message_assert("fixed array index out of range", i0 < m_d0);

    return dimension_element_type(m_data + i0 * m_d1, m_d1);
}

template <ss_typename_param_k T, ss_typename_param_k A, ss_typename_param_k P, ss_bool_t R>
inline ss_typename_type_k fixed_array_2d<T, A, P, R>::reference fixed_array_2d<T, A, P, R>::front()
{
    return at(0, 0);
}

template <ss_typename_param_k T, ss_typename_param_k A, ss_typename_param_k P, ss_bool_t R>
inline ss_typename_type_k fixed_array_2d<T, A, P, R>::reference fixed_array_2d<T, A, P, R>::back()
{
    return at(m_d0 - 1, m_d1 - 1);
}

template <ss_typename_param_k T, ss_typename_param_k A, ss_typename_param_k P, ss_bool_t R>
inline ss_typename_type_k fixed_array_2d<T, A, P, R>::const_reference fixed_array_2d<T, A, P, R>::front() const
{
    return at(0, 0);
}

template <ss_typename_param_k T, ss_typename_param_k A, ss_typename_param_k P, ss_bool_t R>
inline ss_typename_type_k fixed_array_2d<T, A, P, R>::const_reference fixed_array_2d<T, A, P, R>::back() const
{
    return at(m_d0 - 1, m_d1 - 1);
}

template <ss_typename_param_k T, ss_typename_param_k A, ss_typename_param_k P, ss_bool_t R>
inline ss_typename_type_k fixed_array_2d<T, A, P, R>::index_type fixed_array_2d<T, A, P, R>::dimension0() const
{
    return m_d0;
}

template <ss_typename_param_k T, ss_typename_param_k A, ss_typename_param_k P, ss_bool_t R>
inline ss_typename_type_k fixed_array_2d<T, A, P, R>::index_type fixed_array_2d<T, A, P, R>::dimension1() const
{
    return m_d1;
}

template <ss_typename_param_k T, ss_typename_param_k A, ss_typename_param_k P, ss_bool_t R>
inline ss_typename_type_k fixed_array_2d<T, A, P, R>::index_type fixed_array_2d<T, A, P, R>::size() const
{
    stlsoft_assert(m_size == m_d0 * m_d1);

    return m_size;
}

template <ss_typename_param_k T, ss_typename_param_k A, ss_typename_param_k P, ss_bool_t R>
inline ss_bool_t fixed_array_2d<T, A, P, R>::empty() const
{
    return false;
}

template <ss_typename_param_k T, ss_typename_param_k A, ss_typename_param_k P, ss_bool_t R>
inline /* static */ ss_typename_type_k fixed_array_2d<T, A, P, R>::size_type fixed_array_2d<T, A, P, R>::max_size()
{
    return static_cast<size_type>(-1) / sizeof(T);
}

template <ss_typename_param_k T, ss_typename_param_k A, ss_typename_param_k P, ss_bool_t R>
inline ss_typename_type_k fixed_array_2d<T, A, P, R>::iterator fixed_array_2d<T, A, P, R>::begin()
{
    return m_data;
}

template <ss_typename_param_k T, ss_typename_param_k A, ss_typename_param_k P, ss_bool_t R>
inline ss_typename_type_k fixed_array_2d<T, A, P, R>::iterator fixed_array_2d<T, A, P, R>::end()
{
    return m_data + size();
}

template <ss_typename_param_k T, ss_typename_param_k A, ss_typename_param_k P, ss_bool_t R>
inline ss_typename_type_k fixed_array_2d<T, A, P, R>::const_iterator fixed_array_2d<T, A, P, R>::begin() const
{
    return m_data;
}

template <ss_typename_param_k T, ss_typename_param_k A, ss_typename_param_k P, ss_bool_t R>
inline ss_typename_type_k fixed_array_2d<T, A, P, R>::const_iterator fixed_array_2d<T, A, P, R>::end() const
{
    return m_data + size();
}

#if defined(__STLSOFT_CF_BIDIRECTIONAL_ITERATOR_SUPPORT)

template <ss_typename_param_k T, ss_typename_param_k A, ss_typename_param_k P, ss_bool_t R>
inline ss_typename_type_k fixed_array_2d<T, A, P, R>::reverse_iterator fixed_array_2d<T, A, P, R>::rbegin()
{
    return reverse_iterator(end());
}

template <ss_typename_param_k T, ss_typename_param_k A, ss_typename_param_k P, ss_bool_t R>
inline ss_typename_type_k fixed_array_2d<T, A, P, R>::reverse_iterator fixed_array_2d<T, A, P, R>::rend()
{
    return reverse_iterator(begin());
}

template <ss_typename_param_k T, ss_typename_param_k A, ss_typename_param_k P, ss_bool_t R>
inline ss_typename_type_k fixed_array_2d<T, A, P, R>::const_reverse_iterator fixed_array_2d<T, A, P, R>::rbegin() const
{
    return const_reverse_iterator(end());
}

template <ss_typename_param_k T, ss_typename_param_k A, ss_typename_param_k P, ss_bool_t R>
inline ss_typename_type_k fixed_array_2d<T, A, P, R>::const_reverse_iterator fixed_array_2d<T, A, P, R>::rend() const
{
    return const_reverse_iterator(begin());
}

#endif /* __STLSOFT_CF_BIDIRECTIONAL_ITERATOR_SUPPORT */

template <ss_typename_param_k T, ss_typename_param_k A, ss_typename_param_k P, ss_bool_t R>
inline ss_typename_type_k fixed_array_2d<T, A, P, R>::value_type const *fixed_array_2d<T, A, P, R>::data() const
{
    return m_data;
}

// fixed_array_3d

template <ss_typename_param_k T, ss_typename_param_k A, ss_typename_param_k P, ss_bool_t R>
inline ss_typename_type_k fixed_array_3d<T, A, P, R>::pointer fixed_array_3d<T, A, P, R>::allocate_(ss_typename_type_k fixed_array_3d<T, A, P, R>::size_type n)
{
    allocator_type  &ator = *this;

    return ator.allocate(n, NULL);
}

template <ss_typename_param_k T, ss_typename_param_k A, ss_typename_param_k P, ss_bool_t R>
inline void fixed_array_3d<T, A, P, R>::deallocate_(ss_typename_type_k fixed_array_3d<T, A, P, R>::pointer p, ss_typename_type_k fixed_array_3d<T, A, P, R>::size_type n)
{
    allocator_type  &ator = *this;

    ator.deallocate(p, n);
}

template <ss_typename_param_k T, ss_typename_param_k A, ss_typename_param_k P, ss_bool_t R>
inline ss_typename_type_k fixed_array_3d<T, A, P, R>::pointer fixed_array_3d<T, A, P, R>::data_()
{
    return m_data;
}

template <ss_typename_param_k T, ss_typename_param_k A, ss_typename_param_k P, ss_bool_t R>
inline ss_typename_type_k fixed_array_3d<T, A, P, R>::index_type fixed_array_3d<T, A, P, R>::calc_index_(ss_typename_type_k fixed_array_3d<T, A, P, R>::index_type i0, ss_typename_type_k fixed_array_3d<T, A, P, R>::index_type i1, ss_typename_type_k fixed_array_3d<T, A, P, R>::index_type i2) const
{
    return ((i0 * m_d1) + i1) * m_d2 + i2;
}

template <ss_typename_param_k T, ss_typename_param_k A, ss_typename_param_k P, ss_bool_t R>
inline void fixed_array_3d<T, A, P, R>::range_check_(ss_typename_type_k fixed_array_3d<T, A, P, R>::index_type i0, ss_typename_type_k fixed_array_3d<T, A, P, R>::index_type i1, ss_typename_type_k fixed_array_3d<T, A, P, R>::index_type i2) const stlsoft_throw_1(stlsoft_ns_qual_std(out_of_range) )
{
#ifdef __STLSOFT_CF_EXCEPTION_SUPPORT
    if( !(i0 < m_d0) ||
        !(i1 < m_d1) ||
        !(i2 < m_d2))
    {
        stlsoft_ns_qual_std(out_of_range)   x("fixed array index out of range");

#ifdef __STLSOFT_COMPILER_IS_MSVC
        if(1) // This is needed to avoid Visual C++ warning 4702: 'unreachable code'
#endif /* __STLSOFT_COMPILER_IS_MSVC */
        {
            throw x;
        }
    }
#else
    stlsoft_message_assert("fixed array index out of range", i0 < m_d0 && i1 < m_d1 && i2 < m_d2);
#endif /* __STLSOFT_CF_EXCEPTION_SUPPORT */
}

template <ss_typename_param_k T, ss_typename_param_k A, ss_typename_param_k P, ss_bool_t R>
inline void fixed_array_3d<T, A, P, R>::range_check_(ss_typename_type_k fixed_array_3d<T, A, P, R>::index_type i0) const stlsoft_throw_1(stlsoft_ns_qual_std(out_of_range) )
{
#ifdef __STLSOFT_CF_EXCEPTION_SUPPORT
    if(!(i0 < m_d0))
    {
        stlsoft_ns_qual_std(out_of_range)   x("fixed array index out of range");

#ifdef __STLSOFT_COMPILER_IS_MSVC
        if(1) // This is needed to avoid Visual C++ warning 4702: 'unreachable code'
#endif /* __STLSOFT_COMPILER_IS_MSVC */
        {
            throw x;
        }
    }
#else
    stlsoft_message_assert("fixed array index out of range", i0 < m_d0);
#endif /* __STLSOFT_CF_EXCEPTION_SUPPORT */
}

template <ss_typename_param_k T, ss_typename_param_k A, ss_typename_param_k P, ss_bool_t R>
inline fixed_array_3d<T, A, P, R>::fixed_array_3d(ss_typename_type_k fixed_array_3d<T, A, P, R>::pointer data, ss_typename_type_k fixed_array_3d<T, A, P, R>::index_type d0, ss_typename_type_k fixed_array_3d<T, A, P, R>::index_type d1, ss_typename_type_k fixed_array_3d<T, A, P, R>::index_type d2)
    : m_data(data)
    , m_d0(d0)
    , m_d1(d1)
    , m_d2(d2)
{
    stlsoft_static_assert(!R);
}

template <ss_typename_param_k T, ss_typename_param_k A, ss_typename_param_k P, ss_bool_t R>
inline fixed_array_3d<T, A, P, R>::fixed_array_3d(ss_typename_type_k fixed_array_3d<T, A, P, R>::index_type d0, ss_typename_type_k fixed_array_3d<T, A, P, R>::index_type d1, ss_typename_type_k fixed_array_3d<T, A, P, R>::index_type d2)
    : m_data(allocate_(d0 * d1 * d2))
    , m_d0(d0)
    , m_d1(d1)
    , m_d2(d2)
{
    stlsoft_static_assert(R);
    array_range_initialiser<T, A, P>::construct(*this, data_(), size());
}

template <ss_typename_param_k T, ss_typename_param_k A, ss_typename_param_k P, ss_bool_t R>
inline fixed_array_3d<T, A, P, R>::fixed_array_3d(ss_typename_type_k fixed_array_3d<T, A, P, R>::index_type d0, ss_typename_type_k fixed_array_3d<T, A, P, R>::index_type d1, ss_typename_type_k fixed_array_3d<T, A, P, R>::index_type d2, ss_typename_type_k fixed_array_3d<T, A, P, R>::value_type const &t)
    : m_data(allocate_(d0 * d1 * d2))
    , m_d0(d0)
    , m_d1(d1)
    , m_d2(d2)
{
    stlsoft_static_assert(R);
    array_range_initialiser<T, A, P>::construct(*this, data_(), size(), t);
}

template <ss_typename_param_k T, ss_typename_param_k A, ss_typename_param_k P, ss_bool_t R>
inline fixed_array_3d<T, A, P, R>::~fixed_array_3d() stlsoft_throw_0()
{
    if(R)
    {
        array_range_initialiser<T, A, P>::destroy(*this, data_(), size());
        deallocate_(m_data, size());
    }
}

template <ss_typename_param_k T, ss_typename_param_k A, ss_typename_param_k P, ss_bool_t R>
inline ss_typename_type_k fixed_array_3d<T, A, P, R>::reference fixed_array_3d<T, A, P, R>::at(ss_typename_type_k fixed_array_3d<T, A, P, R>::index_type i0, ss_typename_type_k fixed_array_3d<T, A, P, R>::index_type i1, ss_typename_type_k fixed_array_3d<T, A, P, R>::index_type i2)
{
    range_check_(i0, i1, i2);

    return *(m_data + calc_index_(i0, i1, i2));
}

template <ss_typename_param_k T, ss_typename_param_k A, ss_typename_param_k P, ss_bool_t R>
inline ss_typename_type_k fixed_array_3d<T, A, P, R>::const_reference fixed_array_3d<T, A, P, R>::at(ss_typename_type_k fixed_array_3d<T, A, P, R>::index_type i0, ss_typename_type_k fixed_array_3d<T, A, P, R>::index_type i1, ss_typename_type_k fixed_array_3d<T, A, P, R>::index_type i2) const
{
    range_check_(i0, i1, i2);

    return *(m_data + calc_index_(i0, i1, i2));
}

template <ss_typename_param_k T, ss_typename_param_k A, ss_typename_param_k P, ss_bool_t R>
inline ss_typename_type_k fixed_array_3d<T, A, P, R>::reference fixed_array_3d<T, A, P, R>::at_unchecked(ss_typename_type_k fixed_array_3d<T, A, P, R>::index_type i0, ss_typename_type_k fixed_array_3d<T, A, P, R>::index_type i1, ss_typename_type_k fixed_array_3d<T, A, P, R>::index_type i2)
{
    stlsoft_message_assert("fixed array index out of range", i0 < m_d0 && i1 < m_d1 && i2 < m_d2);

    return *(m_data + calc_index_(i0, i1, i2));
}

template <ss_typename_param_k T, ss_typename_param_k A, ss_typename_param_k P, ss_bool_t R>
inline ss_typename_type_k fixed_array_3d<T, A, P, R>::const_reference fixed_array_3d<T, A, P, R>::at_unchecked(ss_typename_type_k fixed_array_3d<T, A, P, R>::index_type i0, ss_typename_type_k fixed_array_3d<T, A, P, R>::index_type i1, ss_typename_type_k fixed_array_3d<T, A, P, R>::index_type i2) const
{
    stlsoft_message_assert("fixed array index out of range", i0 < m_d0 && i1 < m_d1 && i2 < m_d2);

    return *(m_data + calc_index_(i0, i1, i2));
}

template <ss_typename_param_k T, ss_typename_param_k A, ss_typename_param_k P, ss_bool_t R>
inline ss_typename_type_k fixed_array_3d<T, A, P, R>::dimension_element_type fixed_array_3d<T, A, P, R>::at(ss_typename_type_k fixed_array_3d<T, A, P, R>::index_type i0)
{
    range_check_(i0);

    return dimension_element_type(m_data + i0 * m_d1 * m_d2, m_d1, m_d2);
}

template <ss_typename_param_k T, ss_typename_param_k A, ss_typename_param_k P, ss_bool_t R>
inline ss_typename_type_k fixed_array_3d<T, A, P, R>::dimension_element_type const fixed_array_3d<T, A, P, R>::at(ss_typename_type_k fixed_array_3d<T, A, P, R>::index_type i0) const
{
    range_check_(i0);

    return dimension_element_type(m_data + i0 * m_d1 * m_d2, m_d1, m_d2);
}

template <ss_typename_param_k T, ss_typename_param_k A, ss_typename_param_k P, ss_bool_t R>
inline ss_typename_type_k fixed_array_3d<T, A, P, R>::dimension_element_type fixed_array_3d<T, A, P, R>::at_unchecked(ss_typename_type_k fixed_array_3d<T, A, P, R>::index_type i0)
{
    stlsoft_message_assert("fixed array index out of range", i0 < m_d0);

    return dimension_element_type(m_data + i0 * m_d1 * m_d2, m_d1, m_d2);
}

template <ss_typename_param_k T, ss_typename_param_k A, ss_typename_param_k P, ss_bool_t R>
inline ss_typename_type_k fixed_array_3d<T, A, P, R>::dimension_element_type const fixed_array_3d<T, A, P, R>::at_unchecked(ss_typename_type_k fixed_array_3d<T, A, P, R>::index_type i0) const
{
    stlsoft_message_assert("fixed array index out of range", i0 < m_d0);

    return dimension_element_type(m_data + i0 * m_d1 * m_d2, m_d1, m_d2);
}

template <ss_typename_param_k T, ss_typename_param_k A, ss_typename_param_k P, ss_bool_t R>
inline ss_typename_type_k fixed_array_3d<T, A, P, R>::dimension_element_type fixed_array_3d<T, A, P, R>::operator [](ss_typename_type_k fixed_array_3d<T, A, P, R>::index_type i0)
{
    stlsoft_message_assert("fixed array index out of range", i0 < m_d0);

    return dimension_element_type(m_data + i0 * m_d1 * m_d2, m_d1, m_d2);
}

template <ss_typename_param_k T, ss_typename_param_k A, ss_typename_param_k P, ss_bool_t R>
inline ss_typename_type_k fixed_array_3d<T, A, P, R>::dimension_element_type const fixed_array_3d<T, A, P, R>::operator [](ss_typename_type_k fixed_array_3d<T, A, P, R>::index_type i0) const
{
    stlsoft_message_assert("fixed array index out of range", i0 < m_d0);

    return dimension_element_type(m_data + i0 * m_d1 * m_d2, m_d1, m_d2);
}

template <ss_typename_param_k T, ss_typename_param_k A, ss_typename_param_k P, ss_bool_t R>
inline ss_typename_type_k fixed_array_3d<T, A, P, R>::reference fixed_array_3d<T, A, P, R>::front()
{
    return at(0, 0, 0);
}

template <ss_typename_param_k T, ss_typename_param_k A, ss_typename_param_k P, ss_bool_t R>
inline ss_typename_type_k fixed_array_3d<T, A, P, R>::reference fixed_array_3d<T, A, P, R>::back()
{
    return at(m_d0 - 1, m_d1 - 1, m_d2 - 1);
}

template <ss_typename_param_k T, ss_typename_param_k A, ss_typename_param_k P, ss_bool_t R>
inline ss_typename_type_k fixed_array_3d<T, A, P, R>::const_reference fixed_array_3d<T, A, P, R>::front() const
{
    return at(0, 0, 0);
}

template <ss_typename_param_k T, ss_typename_param_k A, ss_typename_param_k P, ss_bool_t R>
inline ss_typename_type_k fixed_array_3d<T, A, P, R>::const_reference fixed_array_3d<T, A, P, R>::back() const
{
    return at(m_d0 - 1, m_d1 - 1, m_d2 - 1);
}

template <ss_typename_param_k T, ss_typename_param_k A, ss_typename_param_k P, ss_bool_t R>
inline ss_typename_type_k fixed_array_3d<T, A, P, R>::index_type fixed_array_3d<T, A, P, R>::dimension0() const
{
    return m_d0;
}

template <ss_typename_param_k T, ss_typename_param_k A, ss_typename_param_k P, ss_bool_t R>
inline ss_typename_type_k fixed_array_3d<T, A, P, R>::index_type fixed_array_3d<T, A, P, R>::dimension1() const
{
    return m_d1;
}

template <ss_typename_param_k T, ss_typename_param_k A, ss_typename_param_k P, ss_bool_t R>
inline ss_typename_type_k fixed_array_3d<T, A, P, R>::index_type fixed_array_3d<T, A, P, R>::dimension2() const
{
    return m_d2;
}

template <ss_typename_param_k T, ss_typename_param_k A, ss_typename_param_k P, ss_bool_t R>
inline ss_typename_type_k fixed_array_3d<T, A, P, R>::index_type fixed_array_3d<T, A, P, R>::size() const
{
    return m_d0 * m_d1 * m_d2;
}

template <ss_typename_param_k T, ss_typename_param_k A, ss_typename_param_k P, ss_bool_t R>
inline ss_bool_t fixed_array_3d<T, A, P, R>::empty() const
{
    return false;
}

template <ss_typename_param_k T, ss_typename_param_k A, ss_typename_param_k P, ss_bool_t R>
inline /* static */ ss_typename_type_k fixed_array_3d<T, A, P, R>::size_type fixed_array_3d<T, A, P, R>::max_size()
{
    return static_cast<size_type>(-1) / sizeof(T);
}

template <ss_typename_param_k T, ss_typename_param_k A, ss_typename_param_k P, ss_bool_t R>
inline ss_typename_type_k fixed_array_3d<T, A, P, R>::iterator fixed_array_3d<T, A, P, R>::begin()
{
    return m_data;
}

template <ss_typename_param_k T, ss_typename_param_k A, ss_typename_param_k P, ss_bool_t R>
inline ss_typename_type_k fixed_array_3d<T, A, P, R>::iterator fixed_array_3d<T, A, P, R>::end()
{
    return m_data + size();
}

template <ss_typename_param_k T, ss_typename_param_k A, ss_typename_param_k P, ss_bool_t R>
inline ss_typename_type_k fixed_array_3d<T, A, P, R>::const_iterator fixed_array_3d<T, A, P, R>::begin() const
{
    return m_data;
}

template <ss_typename_param_k T, ss_typename_param_k A, ss_typename_param_k P, ss_bool_t R>
inline ss_typename_type_k fixed_array_3d<T, A, P, R>::const_iterator fixed_array_3d<T, A, P, R>::end() const
{
    return m_data + size();
}

#if defined(__STLSOFT_CF_BIDIRECTIONAL_ITERATOR_SUPPORT)

template <ss_typename_param_k T, ss_typename_param_k A, ss_typename_param_k P, ss_bool_t R>
inline ss_typename_type_k fixed_array_3d<T, A, P, R>::reverse_iterator fixed_array_3d<T, A, P, R>::rbegin()
{
    return reverse_iterator(end());
}

template <ss_typename_param_k T, ss_typename_param_k A, ss_typename_param_k P, ss_bool_t R>
inline ss_typename_type_k fixed_array_3d<T, A, P, R>::reverse_iterator fixed_array_3d<T, A, P, R>::rend()
{
    return reverse_iterator(begin());
}

template <ss_typename_param_k T, ss_typename_param_k A, ss_typename_param_k P, ss_bool_t R>
inline ss_typename_type_k fixed_array_3d<T, A, P, R>::const_reverse_iterator fixed_array_3d<T, A, P, R>::rbegin() const
{
    return const_reverse_iterator(end());
}

template <ss_typename_param_k T, ss_typename_param_k A, ss_typename_param_k P, ss_bool_t R>
inline ss_typename_type_k fixed_array_3d<T, A, P, R>::const_reverse_iterator fixed_array_3d<T, A, P, R>::rend() const
{
    return const_reverse_iterator(begin());
}

#endif /* __STLSOFT_CF_BIDIRECTIONAL_ITERATOR_SUPPORT */

template <ss_typename_param_k T, ss_typename_param_k A, ss_typename_param_k P, ss_bool_t R>
inline ss_typename_type_k fixed_array_3d<T, A, P, R>::value_type const *fixed_array_3d<T, A, P, R>::data() const
{
    return m_data;
}

// fixed_array_4d

template <ss_typename_param_k T, ss_typename_param_k A, ss_typename_param_k P, ss_bool_t R>
inline ss_typename_type_k fixed_array_4d<T, A, P, R>::pointer fixed_array_4d<T, A, P, R>::allocate_(ss_typename_type_k fixed_array_4d<T, A, P, R>::size_type n)
{
    allocator_type  &ator = *this;

    return ator.allocate(n, NULL);
}

template <ss_typename_param_k T, ss_typename_param_k A, ss_typename_param_k P, ss_bool_t R>
inline void fixed_array_4d<T, A, P, R>::deallocate_(ss_typename_type_k fixed_array_4d<T, A, P, R>::pointer p, ss_typename_type_k fixed_array_4d<T, A, P, R>::size_type n)
{
    allocator_type  &ator = *this;

    ator.deallocate(p, n);
}

template <ss_typename_param_k T, ss_typename_param_k A, ss_typename_param_k P, ss_bool_t R>
inline ss_typename_type_k fixed_array_4d<T, A, P, R>::pointer fixed_array_4d<T, A, P, R>::data_()
{
    return m_data;
}

template <ss_typename_param_k T, ss_typename_param_k A, ss_typename_param_k P, ss_bool_t R>
inline ss_typename_type_k fixed_array_4d<T, A, P, R>::index_type fixed_array_4d<T, A, P, R>::calc_index_(ss_typename_type_k fixed_array_4d<T, A, P, R>::index_type i0, index_type i1, index_type i2, index_type i3) const
{
    return (((i0 * m_d1) + i1) * m_d2 + i2) * m_d3 + i3;
}

template <ss_typename_param_k T, ss_typename_param_k A, ss_typename_param_k P, ss_bool_t R>
inline void fixed_array_4d<T, A, P, R>::range_check_(ss_typename_param_k fixed_array_4d<T, A, P, R>::index_type i0, ss_typename_param_k fixed_array_4d<T, A, P, R>::index_type i1, ss_typename_param_k fixed_array_4d<T, A, P, R>::index_type i2, ss_typename_param_k fixed_array_4d<T, A, P, R>::index_type i3) const stlsoft_throw_1(stlsoft_ns_qual_std(out_of_range) )
{
#ifdef __STLSOFT_CF_EXCEPTION_SUPPORT
    if( !(i0 < m_d0) ||
        !(i1 < m_d1) ||
        !(i2 < m_d2) ||
        !(i3 < m_d3))
    {
        stlsoft_ns_qual_std(out_of_range)   x("fixed array index out of range");

#ifdef __STLSOFT_COMPILER_IS_MSVC
        if(1) // This is needed to avoid Visual C++ warning 4702: 'unreachable code'
#endif /* __STLSOFT_COMPILER_IS_MSVC */
        {
            throw x;
        }
    }
#else
    stlsoft_message_assert("fixed array index out of range", i0 < m_d0 && i1 < m_d1 && i2 < m_d2 && i3 < m_d3);
#endif /* __STLSOFT_CF_EXCEPTION_SUPPORT */
}

template <ss_typename_param_k T, ss_typename_param_k A, ss_typename_param_k P, ss_bool_t R>
inline void fixed_array_4d<T, A, P, R>::range_check_(ss_typename_param_k fixed_array_4d<T, A, P, R>::index_type i0) const stlsoft_throw_1(stlsoft_ns_qual_std(out_of_range) )
{
#ifdef __STLSOFT_CF_EXCEPTION_SUPPORT
    if(!(i0 < m_d0))
    {
        stlsoft_ns_qual_std(out_of_range)   x("fixed array index out of range");

#ifdef __STLSOFT_COMPILER_IS_MSVC
        if(1) // This is needed to avoid Visual C++ warning 4702: 'unreachable code'
#endif /* __STLSOFT_COMPILER_IS_MSVC */
        {
            throw x;
        }
    }
#else
    stlsoft_message_assert("fixed array index out of range", i0 < m_d0);
#endif /* __STLSOFT_CF_EXCEPTION_SUPPORT */
}

template <ss_typename_param_k T, ss_typename_param_k A, ss_typename_param_k P, ss_bool_t R>
inline fixed_array_4d<T, A, P, R>::fixed_array_4d(T *data, ss_typename_param_k fixed_array_4d<T, A, P, R>::index_type d0, ss_typename_param_k fixed_array_4d<T, A, P, R>::index_type d1, ss_typename_param_k fixed_array_4d<T, A, P, R>::index_type d2, ss_typename_param_k fixed_array_4d<T, A, P, R>::index_type d3)
    : m_data(data)
    , m_d0(d0)
    , m_d1(d1)
    , m_d2(d2)
    , m_d3(d3)
{
    stlsoft_static_assert(!R);
}

template <ss_typename_param_k T, ss_typename_param_k A, ss_typename_param_k P, ss_bool_t R>
inline fixed_array_4d<T, A, P, R>::fixed_array_4d(ss_typename_param_k fixed_array_4d<T, A, P, R>::index_type d0, ss_typename_param_k fixed_array_4d<T, A, P, R>::index_type d1, ss_typename_param_k fixed_array_4d<T, A, P, R>::index_type d2, ss_typename_param_k fixed_array_4d<T, A, P, R>::index_type d3)
    : m_data(allocate_(d0 * d1 * d2 * d3))
    , m_d0(d0)
    , m_d1(d1)
    , m_d2(d2)
    , m_d3(d3)
{
    stlsoft_static_assert(R);
    array_range_initialiser<T, A, P>::construct(*this, data_(), size());
}

template <ss_typename_param_k T, ss_typename_param_k A, ss_typename_param_k P, ss_bool_t R>
inline fixed_array_4d<T, A, P, R>::fixed_array_4d(ss_typename_param_k fixed_array_4d<T, A, P, R>::index_type d0, ss_typename_param_k fixed_array_4d<T, A, P, R>::index_type d1, ss_typename_param_k fixed_array_4d<T, A, P, R>::index_type d2, ss_typename_param_k fixed_array_4d<T, A, P, R>::index_type d3, ss_typename_param_k fixed_array_4d<T, A, P, R>::value_type const &t)
    : m_data(allocate_(d0 * d1 * d2 * d3))
    , m_d0(d0)
    , m_d1(d1)
    , m_d2(d2)
    , m_d3(d3)
{
    stlsoft_static_assert(R);
    array_range_initialiser<T, A, P>::construct(*this, data_(), size(), t);
}

template <ss_typename_param_k T, ss_typename_param_k A, ss_typename_param_k P, ss_bool_t R>
inline fixed_array_4d<T, A, P, R>::~fixed_array_4d() stlsoft_throw_0()
{
    if(R)
    {
        array_range_initialiser<T, A, P>::destroy(*this, data_(), size());
        deallocate_(m_data, size());
    }
}

template <ss_typename_param_k T, ss_typename_param_k A, ss_typename_param_k P, ss_bool_t R>
inline ss_typename_type_k fixed_array_4d<T, A, P, R>::reference fixed_array_4d<T, A, P, R>::at(ss_typename_param_k fixed_array_4d<T, A, P, R>::index_type i0, ss_typename_param_k fixed_array_4d<T, A, P, R>::index_type i1, ss_typename_param_k fixed_array_4d<T, A, P, R>::index_type i2, ss_typename_param_k fixed_array_4d<T, A, P, R>::index_type i3)
{
    range_check_(i0, i1, i2, i3);

    return *(m_data + calc_index_(i0, i1, i2, i3));
}

template <ss_typename_param_k T, ss_typename_param_k A, ss_typename_param_k P, ss_bool_t R>
inline ss_typename_type_k fixed_array_4d<T, A, P, R>::const_reference fixed_array_4d<T, A, P, R>::at(ss_typename_param_k fixed_array_4d<T, A, P, R>::index_type i0, ss_typename_param_k fixed_array_4d<T, A, P, R>::index_type i1, ss_typename_param_k fixed_array_4d<T, A, P, R>::index_type i2, ss_typename_param_k fixed_array_4d<T, A, P, R>::index_type i3) const
{
    range_check_(i0, i1, i2, i3);

    return *(m_data + calc_index_(i0, i1, i2, i3));
}

template <ss_typename_param_k T, ss_typename_param_k A, ss_typename_param_k P, ss_bool_t R>
inline ss_typename_type_k fixed_array_4d<T, A, P, R>::reference fixed_array_4d<T, A, P, R>::at_unchecked(ss_typename_param_k fixed_array_4d<T, A, P, R>::index_type i0, ss_typename_param_k fixed_array_4d<T, A, P, R>::index_type i1, ss_typename_param_k fixed_array_4d<T, A, P, R>::index_type i2, ss_typename_param_k fixed_array_4d<T, A, P, R>::index_type i3)
{
    stlsoft_message_assert("fixed array index out of range", i0 < m_d0 && i1 < m_d1 && i2 < m_d2 && i3 < m_d3);

    return *(m_data + calc_index_(i0, i1, i2, i3));
}

template <ss_typename_param_k T, ss_typename_param_k A, ss_typename_param_k P, ss_bool_t R>
inline ss_typename_type_k fixed_array_4d<T, A, P, R>::const_reference fixed_array_4d<T, A, P, R>::at_unchecked(ss_typename_param_k fixed_array_4d<T, A, P, R>::index_type i0, ss_typename_param_k fixed_array_4d<T, A, P, R>::index_type i1, ss_typename_param_k fixed_array_4d<T, A, P, R>::index_type i2, ss_typename_param_k fixed_array_4d<T, A, P, R>::index_type i3) const
{
    stlsoft_message_assert("fixed array index out of range", i0 < m_d0 && i1 < m_d1 && i2 < m_d2 && i3 < m_d3);

    return *(m_data + calc_index_(i0, i1, i2, i3));
}

template <ss_typename_param_k T, ss_typename_param_k A, ss_typename_param_k P, ss_bool_t R>
inline ss_typename_type_k fixed_array_4d<T, A, P, R>::dimension_element_type fixed_array_4d<T, A, P, R>::at(ss_typename_param_k fixed_array_4d<T, A, P, R>::index_type i0)
{
    range_check_(i0);

    return dimension_element_type(m_data + i0 * m_d1 * m_d2 * m_d3, m_d1, m_d2, m_d3);
}

template <ss_typename_param_k T, ss_typename_param_k A, ss_typename_param_k P, ss_bool_t R>
inline const ss_typename_type_k fixed_array_4d<T, A, P, R>::dimension_element_type fixed_array_4d<T, A, P, R>::at(ss_typename_param_k fixed_array_4d<T, A, P, R>::index_type i0) const
{
    range_check_(i0);

    return dimension_element_type(m_data + i0 * m_d1 * m_d2 * m_d3, m_d1, m_d2, m_d3);
}

template <ss_typename_param_k T, ss_typename_param_k A, ss_typename_param_k P, ss_bool_t R>
inline ss_typename_type_k fixed_array_4d<T, A, P, R>::dimension_element_type fixed_array_4d<T, A, P, R>::at_unchecked(ss_typename_param_k fixed_array_4d<T, A, P, R>::index_type i0)
{
    stlsoft_message_assert("fixed array index out of range", i0 < m_d0);

    return dimension_element_type(m_data + i0 * m_d1 * m_d2 * m_d3, m_d1, m_d2, m_d3);
}

template <ss_typename_param_k T, ss_typename_param_k A, ss_typename_param_k P, ss_bool_t R>
inline ss_typename_type_k fixed_array_4d<T, A, P, R>::dimension_element_type const fixed_array_4d<T, A, P, R>::at_unchecked(ss_typename_param_k fixed_array_4d<T, A, P, R>::index_type i0) const
{
    stlsoft_message_assert("fixed array index out of range", i0 < m_d0);

    return dimension_element_type(m_data + i0 * m_d1 * m_d2 * m_d3, m_d1, m_d2, m_d3);
}

template <ss_typename_param_k T, ss_typename_param_k A, ss_typename_param_k P, ss_bool_t R>
inline ss_typename_type_k fixed_array_4d<T, A, P, R>::dimension_element_type fixed_array_4d<T, A, P, R>::operator [](ss_typename_param_k fixed_array_4d<T, A, P, R>::index_type i0)
{
    stlsoft_message_assert("fixed array index out of range", i0 < m_d0);

    return dimension_element_type(m_data + i0 * m_d1 * m_d2 * m_d3, m_d1, m_d2, m_d3);
}

template <ss_typename_param_k T, ss_typename_param_k A, ss_typename_param_k P, ss_bool_t R>
inline ss_typename_type_k fixed_array_4d<T, A, P, R>::dimension_element_type const fixed_array_4d<T, A, P, R>::operator [](ss_typename_param_k fixed_array_4d<T, A, P, R>::index_type i0) const
{
    stlsoft_message_assert("fixed array index out of range", i0 < m_d0);

    return dimension_element_type(m_data + i0 * m_d1 * m_d2 * m_d3, m_d1, m_d2, m_d3);
}

template <ss_typename_param_k T, ss_typename_param_k A, ss_typename_param_k P, ss_bool_t R>
inline ss_typename_type_k fixed_array_4d<T, A, P, R>::reference fixed_array_4d<T, A, P, R>::front()
{
    return at(0, 0, 0, 0);
}

template <ss_typename_param_k T, ss_typename_param_k A, ss_typename_param_k P, ss_bool_t R>
inline ss_typename_type_k fixed_array_4d<T, A, P, R>::reference fixed_array_4d<T, A, P, R>::back()
{
    return at(m_d0 - 1, m_d1 - 1, m_d2 - 1, m_d3 - 1);
}

template <ss_typename_param_k T, ss_typename_param_k A, ss_typename_param_k P, ss_bool_t R>
inline ss_typename_type_k fixed_array_4d<T, A, P, R>::const_reference fixed_array_4d<T, A, P, R>::front() const
{
    return at(0, 0, 0, 0);
}

template <ss_typename_param_k T, ss_typename_param_k A, ss_typename_param_k P, ss_bool_t R>
inline ss_typename_type_k fixed_array_4d<T, A, P, R>::const_reference fixed_array_4d<T, A, P, R>::back() const
{
    return at(m_d0 - 1, m_d1 - 1, m_d2 - 1, m_d3 - 1);
}

template <ss_typename_param_k T, ss_typename_param_k A, ss_typename_param_k P, ss_bool_t R>
inline ss_typename_type_k fixed_array_4d<T, A, P, R>::index_type fixed_array_4d<T, A, P, R>::dimension0() const
{
    return m_d0;
}

template <ss_typename_param_k T, ss_typename_param_k A, ss_typename_param_k P, ss_bool_t R>
inline ss_typename_type_k fixed_array_4d<T, A, P, R>::index_type fixed_array_4d<T, A, P, R>::dimension1() const
{
    return m_d1;
}

template <ss_typename_param_k T, ss_typename_param_k A, ss_typename_param_k P, ss_bool_t R>
inline ss_typename_type_k fixed_array_4d<T, A, P, R>::index_type fixed_array_4d<T, A, P, R>::dimension2() const
{
    return m_d2;
}

template <ss_typename_param_k T, ss_typename_param_k A, ss_typename_param_k P, ss_bool_t R>
inline ss_typename_type_k fixed_array_4d<T, A, P, R>::index_type fixed_array_4d<T, A, P, R>::dimension3() const
{
    return m_d3;
}

template <ss_typename_param_k T, ss_typename_param_k A, ss_typename_param_k P, ss_bool_t R>
inline ss_typename_type_k fixed_array_4d<T, A, P, R>::index_type fixed_array_4d<T, A, P, R>::size() const
{
    return m_d0 * m_d1 * m_d2 * m_d3;
}

template <ss_typename_param_k T, ss_typename_param_k A, ss_typename_param_k P, ss_bool_t R>
inline ss_bool_t fixed_array_4d<T, A, P, R>::empty() const
{
    return false;
}

template <ss_typename_param_k T, ss_typename_param_k A, ss_typename_param_k P, ss_bool_t R>
inline /* static */ ss_typename_type_k fixed_array_4d<T, A, P, R>::size_type fixed_array_4d<T, A, P, R>::max_size()
{
    return static_cast<size_type>(-1) / sizeof(T);
}

template <ss_typename_param_k T, ss_typename_param_k A, ss_typename_param_k P, ss_bool_t R>
inline ss_typename_type_k fixed_array_4d<T, A, P, R>::iterator fixed_array_4d<T, A, P, R>::begin()
{
    return m_data;
}

template <ss_typename_param_k T, ss_typename_param_k A, ss_typename_param_k P, ss_bool_t R>
inline ss_typename_type_k fixed_array_4d<T, A, P, R>::iterator fixed_array_4d<T, A, P, R>::end()
{
    return m_data + size();
}

template <ss_typename_param_k T, ss_typename_param_k A, ss_typename_param_k P, ss_bool_t R>
inline ss_typename_type_k fixed_array_4d<T, A, P, R>::const_iterator fixed_array_4d<T, A, P, R>::begin() const
{
    return m_data;
}

template <ss_typename_param_k T, ss_typename_param_k A, ss_typename_param_k P, ss_bool_t R>
inline ss_typename_type_k fixed_array_4d<T, A, P, R>::const_iterator fixed_array_4d<T, A, P, R>::end() const
{
    return m_data + size();
}

#if defined(__STLSOFT_CF_BIDIRECTIONAL_ITERATOR_SUPPORT)

template <ss_typename_param_k T, ss_typename_param_k A, ss_typename_param_k P, ss_bool_t R>
inline ss_typename_type_k fixed_array_4d<T, A, P, R>::reverse_iterator fixed_array_4d<T, A, P, R>::rbegin()
{
    return reverse_iterator(end());
}

template <ss_typename_param_k T, ss_typename_param_k A, ss_typename_param_k P, ss_bool_t R>
inline ss_typename_type_k fixed_array_4d<T, A, P, R>::reverse_iterator fixed_array_4d<T, A, P, R>::rend()
{
    return reverse_iterator(begin());
}

template <ss_typename_param_k T, ss_typename_param_k A, ss_typename_param_k P, ss_bool_t R>
inline ss_typename_type_k fixed_array_4d<T, A, P, R>::const_reverse_iterator fixed_array_4d<T, A, P, R>::rbegin() const
{
    return const_reverse_iterator(end());
}

template <ss_typename_param_k T, ss_typename_param_k A, ss_typename_param_k P, ss_bool_t R>
inline ss_typename_type_k fixed_array_4d<T, A, P, R>::const_reverse_iterator fixed_array_4d<T, A, P, R>::rend() const
{
    return const_reverse_iterator(begin());
}

#endif /* __STLSOFT_CF_BIDIRECTIONAL_ITERATOR_SUPPORT */

template <ss_typename_param_k T, ss_typename_param_k A, ss_typename_param_k P, ss_bool_t R>
inline ss_typename_type_k fixed_array_4d<T, A, P, R>::value_type const *fixed_array_4d<T, A, P, R>::data() const
{
    return m_data;
}

#endif /* !__STLSOFT_DOCUMENTATION_SKIP_SECTION */

/* /////////////////////////////////////////////////////////////////////////////
 * Shims
 */

template<   ss_typename_param_k T
        ,   ss_typename_param_k A
        ,   ss_typename_param_k P
        ,   ss_bool_t           R
        >
inline ss_size_t array_size(fixed_array_1d<T, A, P, R> const &ar)
{
    return ar.size();
}

template<   ss_typename_param_k T
        ,   ss_typename_param_k A
        ,   ss_typename_param_k P
        ,   ss_bool_t           R
        >
inline ss_size_t array_size(fixed_array_2d<T, A, P, R> const &ar)
{
    return ar.size();
}

template<   ss_typename_param_k T
        ,   ss_typename_param_k A
        ,   ss_typename_param_k P
        ,   ss_bool_t           R
        >
inline ss_size_t array_size(fixed_array_3d<T, A, P, R> const &ar)
{
    return ar.size();
}

template<   ss_typename_param_k T
        ,   ss_typename_param_k A
        ,   ss_typename_param_k P
        ,   ss_bool_t           R
        >
inline ss_size_t array_size(fixed_array_4d<T, A, P, R> const &ar)
{
    return ar.size();
}

#if 0
template<   ss_typename_param_k T
        ,   ss_typename_param_k A
        ,   ss_typename_param_k P
        ,   ss_bool_t           R
        >
inline ss_size_t array_size(fixed_array_5d<T, A, P, R> const &ar)
{
    return ar.size();
}
#endif /* 0 */

/* ////////////////////////////////////////////////////////////////////////// */

#ifndef _STLSOFT_NO_NAMESPACE
} // namespace stlsoft
#endif /* _STLSOFT_NO_NAMESPACE */

/* ////////////////////////////////////////////////////////////////////////// */

#endif /* !STLSOFT_INCL_H_STLSOFT_FIXED_ARRAY */

/* ////////////////////////////////////////////////////////////////////////// */
