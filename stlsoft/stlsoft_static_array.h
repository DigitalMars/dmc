/* /////////////////////////////////////////////////////////////////////////////
 * File:        stlsoft_static_array.h (originally MTArray.h, ::SynesisStl)
 *
 * Purpose:     Contains the static_array template class.
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


/// \file stlsoft_static_array.h
///
/// Contains the static_array template class.

#ifndef STLSOFT_INCL_H_STLSOFT_STATIC_ARRAY
#define STLSOFT_INCL_H_STLSOFT_STATIC_ARRAY

#ifndef __STLSOFT_DOCUMENTATION_SKIP_SECTION
# define STLSOFT_VER_H_STLSOFT_STATIC_ARRAY_MAJOR       2
# define STLSOFT_VER_H_STLSOFT_STATIC_ARRAY_MINOR       0
# define STLSOFT_VER_H_STLSOFT_STATIC_ARRAY_REVISION    1
# define STLSOFT_VER_H_STLSOFT_STATIC_ARRAY_EDIT        128
#endif /* !__STLSOFT_DOCUMENTATION_SKIP_SECTION */

/* /////////////////////////////////////////////////////////////////////////////
 * Compatibility
 */

/*
[Incompatibilies-start]
__STLSOFT_COMPILER_IS_DMC: __DMC__<0x0839
[Incompatibilies-end]
 */

/* /////////////////////////////////////////////////////////////////////////////
 * Includes
 */

#ifndef STLSOFT_INCL_H_STLSOFT
# include "stlsoft.h"                   // Include the STLSoft root header
#endif /* !STLSOFT_INCL_H_STLSOFT */

#if defined(__STLSOFT_COMPILER_IS_DMC) && \
    __DMC__ < 0x0839
# error stlsoft_static_array.h is not compatible with Digital Mars C/C++ 3.38 or earlier
#endif /* _MSC_VER < 1200 */

#ifndef STLSOFT_INCL_H_STLSOFT_NULL_ALLOCATOR
# include "stlsoft_null_allocator.h"    // stlsoft::null_allocator
#endif /* !STLSOFT_INCL_H_STLSOFT_NULL_ALLOCATOR */
#ifndef STLSOFT_INCL_H_STLSOFT_ITERATOR
# include "stlsoft_iterator.h"          // stlsoft::iterator, stlsoft::reverse_iterator
#endif /* STLSOFT_INCL_H_STLSOFT_ITERATOR */
#ifndef STLSOFT_INCL_H_STLSOFT_ARRAY_POLICIES
# include "stlsoft_array_policies.h"    // stlsoft::do_construction
#endif /* !STLSOFT_INCL_H_STLSOFT_ARRAY_POLICIES */
#ifndef STLSOFT_INCL_H_STLSOFT_META
# include "stlsoft_meta.h"  // is_pointer
#endif /* !STLSOFT_INCL_H_STLSOFT_META */
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
        ,   ss_size_t N0
        ,   ss_typename_param_k P
        ,   ss_typename_param_k M
        >
class static_array_1d;

template<   ss_typename_param_k T
        ,   ss_size_t N0
        ,   ss_size_t N1
        ,   ss_typename_param_k P
        ,   ss_typename_param_k M
        >
class static_array_2d;

template<   ss_typename_param_k T
        ,   ss_size_t N0
        ,   ss_size_t N1
        ,   ss_size_t N2
        ,   ss_typename_param_k P
        ,   ss_typename_param_k M
        >
class static_array_3d;

template<   ss_typename_param_k T
        ,   ss_size_t N0
        ,   ss_size_t N1
        ,   ss_size_t N2
        ,   ss_size_t N3
        ,   ss_typename_param_k P
        ,   ss_typename_param_k M
        >
class static_array_4d;

template<   ss_typename_param_k T
        ,   ss_size_t N0
        ,   ss_size_t N1
        ,   ss_size_t N2
        ,   ss_size_t N3
        ,   ss_size_t N4
        ,   ss_typename_param_k P
        ,   ss_typename_param_k M
        >
class static_array_5d;

#endif /* !__STLSOFT_DOCUMENTATION_SKIP_SECTION */

////////////////////////////////////////////////////////////////////////////////
// Classes

// class static_array_1d
/// 1 dimensional static array
///
/// \param T The value type
/// \param N0 The first dimension extent
/// \param P The construction policy type
template<   ss_typename_param_k T
        ,   ss_size_t N0
#ifdef __STLSOFT_CF_TEMPLATE_CLASS_DEFAULT_CLASS_ARGUMENT_SUPPORT
        ,   ss_typename_param_k P   =   do_construction<T>
        ,   ss_typename_param_k M   =   T[N0]
#else
        ,   ss_typename_param_k P
        ,   ss_typename_param_k M
#endif /* __STLSOFT_CF_TEMPLATE_CLASS_DEFAULT_CLASS_ARGUMENT_SUPPORT */
        >
class static_array_1d
    : public null_allocator<T>
{
public:
    typedef static_array_1d<T, N0, P, M>                    class_type;
    typedef T                                               dimension_type;
    typedef null_allocator<T>                               allocator_type;
    typedef P                                               policy_type;

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

#ifdef __STLSOFT_CF_BIDIRECTIONAL_ITERATOR_SUPPORT
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
//protected:
    static_array_1d(T *data/* [N0] */);
public:
    static_array_1d();
    static_array_1d(value_type const &t);
    ~static_array_1d();

// Access
public:
    reference               at(index_type i0);
    const_reference         at(index_type i0) const;

    reference               at_unchecked(index_type i0);
    const_reference         at_unchecked(index_type i0) const;

    reference               operator [](index_type i0);
    const_reference         operator [](index_type i0) const;

    reference               front();
    reference               back();
    const_reference         front() const;
    const_reference         back() const;

// State
public:
    static index_type       dimension0();
    static index_type       size();
    static ss_bool_t        empty();
    static size_type        max_size();

// Iteration
public:
    iterator                begin();
    iterator                end();
    const_iterator          begin() const;
    const_iterator          end() const;

#ifdef __STLSOFT_CF_BIDIRECTIONAL_ITERATOR_SUPPORT
    reverse_iterator        rbegin();
    reverse_iterator        rend();
    const_reverse_iterator  rbegin() const;
    const_reverse_iterator  rend() const;
#endif /* __STLSOFT_CF_BIDIRECTIONAL_ITERATOR_SUPPORT */

// Access
public:
    value_type const        *data() const;

// Implementation
protected:
    pointer     data_();
    index_type  calc_index_(index_type i0) const;
    void        range_check_(index_type i0) const stlsoft_throw_1(stlsoft_ns_qual_std(out_of_range) );

// Members
protected:
    M   m_data;

// Not to be implemented
private:
#if defined(__STLSOFT_COMPILER_IS_BORLAND) || \
    defined(__STLSOFT_COMPILER_IS_GCC) || \
    defined(__STLSOFT_COMPILER_IS_MSVC)
public:
#else
private:
#endif /* compiler */
    static_array_1d(class_type const &rhs);
private:
    class_type const &operator =(class_type const &rhs);
};

// class static_array_2d
/// 2 dimensional static array
///
/// \param T The value type
/// \param N0 The first dimension extent
/// \param N1 The second dimension extent
/// \param P The construction policy type
template<   ss_typename_param_k T
        ,   ss_size_t N0
        ,   ss_size_t N1
#ifdef __STLSOFT_CF_TEMPLATE_CLASS_DEFAULT_CLASS_ARGUMENT_SUPPORT
        ,   ss_typename_param_k P   =   do_construction<T>
        ,   ss_typename_param_k M   =   T[N0 * N1]
#else
        ,   ss_typename_param_k P
        ,   ss_typename_param_k M
#endif /* __STLSOFT_CF_TEMPLATE_CLASS_DEFAULT_CLASS_ARGUMENT_SUPPORT */
        >
class static_array_2d
    : public null_allocator<T>
{
public:
    typedef static_array_2d<T, N0, N1, P, M>                class_type;
    typedef static_array_1d<T, N1, P, T*>                   dimension_type;
    typedef null_allocator<T>                               allocator_type;
    typedef P                                               policy_type;

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

#ifdef __STLSOFT_CF_BIDIRECTIONAL_ITERATOR_SUPPORT
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
//protected:
//    static_array_2d(T data[N0][N1]);
    static_array_2d(T *data);
public:
    static_array_2d();
    static_array_2d(value_type const &t);
    ~static_array_2d();

// Operations
public:
    reference               at(index_type i0, index_type i1);
    const_reference         at(index_type i0, index_type i1) const;

    reference               at_unchecked(index_type i0, index_type i1);
    const_reference         at_unchecked(index_type i0, index_type i1) const;

    dimension_type          at(index_type i0);
    const dimension_type    at(index_type i0) const;

    dimension_type          at_unchecked(index_type i0);
    const dimension_type    at_unchecked(index_type i0) const;

    dimension_type          operator [](index_type i0);
    const dimension_type    operator [](index_type i0) const;

    reference               front();
    reference               back();
    const_reference         front() const;
    const_reference         back() const;

// State
public:
    static index_type       dimension0();
    static index_type       dimension1();
    static index_type       size();
    static ss_bool_t        empty();
    static size_type        max_size();

// Iteration
public:
    iterator                begin();
    iterator                end();
    const_iterator          begin() const;
    const_iterator          end() const;

#ifdef __STLSOFT_CF_BIDIRECTIONAL_ITERATOR_SUPPORT
    reverse_iterator        rbegin();
    reverse_iterator        rend();
    const_reverse_iterator  rbegin() const;
    const_reverse_iterator  rend() const;
#endif /* __STLSOFT_CF_BIDIRECTIONAL_ITERATOR_SUPPORT */

// Access
public:
    value_type const        *data() const;

// Implementation
protected:
    pointer     data_();
    index_type  calc_index_(index_type i0, index_type i1) const;
    void        range_check_(index_type i0, index_type i1) const stlsoft_throw_1(stlsoft_ns_qual_std(out_of_range) );
    void        range_check_(index_type i0) const stlsoft_throw_1(stlsoft_ns_qual_std(out_of_range) );

// Members
protected:
    M   m_data;

// Not to be implemented
private:
#if defined(__STLSOFT_COMPILER_IS_BORLAND) || \
    defined(__STLSOFT_COMPILER_IS_GCC) || \
    defined(__STLSOFT_COMPILER_IS_MSVC)
public:
#else
private:
#endif /* compiler */
    static_array_2d(class_type const &rhs);
private:
    class_type const &operator =(class_type const &rhs);
};

// class static_array_3d
/// 3 dimensional static array
///
/// \param T The value type
/// \param N0 The first dimension extent
/// \param N1 The second dimension extent
/// \param N2 The third dimension extent
/// \param P The construction policy type
template<   ss_typename_param_k T
        ,   ss_size_t N0
        ,   ss_size_t N1
        ,   ss_size_t N2
#ifdef __STLSOFT_CF_TEMPLATE_CLASS_DEFAULT_CLASS_ARGUMENT_SUPPORT
        ,   ss_typename_param_k P   =   do_construction<T>
        ,   ss_typename_param_k M   =   T[N0 * N1 * N2]
#else
        ,   ss_typename_param_k P
        ,   ss_typename_param_k M
#endif /* __STLSOFT_CF_TEMPLATE_CLASS_DEFAULT_CLASS_ARGUMENT_SUPPORT */
        >
class static_array_3d
    : public null_allocator<T>
{
public:
    typedef static_array_3d<T, N0, N1, N2, P, M>            class_type;
    typedef static_array_2d<T, N1, N2, P, T*>               dimension_type;
    typedef null_allocator<T>                               allocator_type;
    typedef P                                               policy_type;

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

#ifdef __STLSOFT_CF_BIDIRECTIONAL_ITERATOR_SUPPORT
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
//protected:
//    static_array_3d(T data[N0][N1][N2]);
    static_array_3d(T *data);
public:
    static_array_3d();
    static_array_3d(value_type const &t);
    ~static_array_3d();

// Operations
public:
    reference               at(index_type i0, index_type i1, index_type i2);
    const_reference         at(index_type i0, index_type i1, index_type i2) const;

    reference               at_unchecked(index_type i0, index_type i1, index_type i2);
    const_reference         at_unchecked(index_type i0, index_type i1, index_type i2) const;

    dimension_type          at(index_type i0);
    const dimension_type    at(index_type i0) const;

    dimension_type          at_unchecked(index_type i0);
    const dimension_type    at_unchecked(index_type i0) const;

    dimension_type          operator [](index_type i0);
    const dimension_type    operator [](index_type i0) const;

    reference               front();
    reference               back();
    const_reference         front() const;
    const_reference         back() const;

// State
public:
    static index_type       dimension0();
    static index_type       dimension1();
    static index_type       dimension2();
    static index_type       size();
    static ss_bool_t        empty();
    static size_type        max_size();

// Iteration
public:
    iterator                begin();
    iterator                end();
    const_iterator          begin() const;
    const_iterator          end() const;

#ifdef __STLSOFT_CF_BIDIRECTIONAL_ITERATOR_SUPPORT
    reverse_iterator        rbegin();
    reverse_iterator        rend();
    const_reverse_iterator  rbegin() const;
    const_reverse_iterator  rend() const;
#endif /* __STLSOFT_CF_BIDIRECTIONAL_ITERATOR_SUPPORT */

// Access
public:
    value_type const        *data() const;

// Implementation
protected:
    pointer     data_();
    index_type  calc_index_(index_type i0, index_type i1, index_type i2) const;
    void        range_check_(index_type i0, index_type i1, index_type i2) const stlsoft_throw_1(stlsoft_ns_qual_std(out_of_range) );
    void        range_check_(index_type i0) const stlsoft_throw_1(stlsoft_ns_qual_std(out_of_range) );

// Members
protected:
    M   m_data;

// Not to be implemented
private:
#if defined(__STLSOFT_COMPILER_IS_BORLAND) || \
    defined(__STLSOFT_COMPILER_IS_GCC) || \
    defined(__STLSOFT_COMPILER_IS_MSVC)
public:
#else
private:
#endif /* compiler */
    static_array_3d(class_type const &rhs);
private:
    class_type const &operator =(class_type const &rhs);
};


// class static_array_4d
/// 4 dimensional static array
///
/// \param T The value type
/// \param N0 The first dimension extent
/// \param N1 The second dimension extent
/// \param N2 The third dimension extent
/// \param P The construction policy type
template<   ss_typename_param_k T
        ,   ss_size_t N0
        ,   ss_size_t N1
        ,   ss_size_t N2
        ,   ss_size_t N3
#ifdef __STLSOFT_CF_TEMPLATE_CLASS_DEFAULT_CLASS_ARGUMENT_SUPPORT
        ,   ss_typename_param_k P   =   do_construction<T>
        ,   ss_typename_param_k M   =   T[N0 * N1 * N2 * N3]
#else
        ,   ss_typename_param_k P
        ,   ss_typename_param_k M
#endif /* __STLSOFT_CF_TEMPLATE_CLASS_DEFAULT_CLASS_ARGUMENT_SUPPORT */
        >
class static_array_4d
    : public null_allocator<T>
{
public:
    typedef static_array_4d<T, N0, N1, N2, N3, P, M>        class_type;
    typedef static_array_3d<T, N1, N2, N3, P, T*>           dimension_type;
    typedef null_allocator<T>                               allocator_type;
    typedef P                                               policy_type;

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

#ifdef __STLSOFT_CF_BIDIRECTIONAL_ITERATOR_SUPPORT
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
//protected:
//    static_array_4d(T data[N0][N1][N2][N3]);
    static_array_4d(T *data);
public:
    static_array_4d();
    static_array_4d(value_type const &t);
    ~static_array_4d();

// Operations
public:
    reference               at(index_type i0, index_type i1, index_type i2, index_type i3);
    const_reference         at(index_type i0, index_type i1, index_type i2, index_type i3) const;

    reference               at_unchecked(index_type i0, index_type i1, index_type i2, index_type i3);
    const_reference         at_unchecked(index_type i0, index_type i1, index_type i2, index_type i3) const;

    dimension_type          at(index_type i0);
    const dimension_type    at(index_type i0) const;

    dimension_type          at_unchecked(index_type i0);
    const dimension_type    at_unchecked(index_type i0) const;

    dimension_type          operator [](index_type i0);
    const dimension_type    operator [](index_type i0) const;

    reference               front();
    reference               back();
    const_reference         front() const;
    const_reference         back() const;

// State
public:
    static index_type       dimension0();
    static index_type       dimension1();
    static index_type       dimension2();
    static index_type       dimension3();
    static index_type       size();
    static ss_bool_t        empty();
    static size_type        max_size();

// Iteration
public:
    iterator                begin();
    iterator                end();
    const_iterator          begin() const;
    const_iterator          end() const;

#ifdef __STLSOFT_CF_BIDIRECTIONAL_ITERATOR_SUPPORT
    reverse_iterator        rbegin();
    reverse_iterator        rend();
    const_reverse_iterator  rbegin() const;
    const_reverse_iterator  rend() const;
#endif /* __STLSOFT_CF_BIDIRECTIONAL_ITERATOR_SUPPORT */

// Access
public:
    value_type const        *data() const;

// Implementation
protected:
    pointer     data_();
    index_type  calc_index_(index_type i0, index_type i1, index_type i2, index_type i3) const;
    void        range_check_(index_type i0, index_type i1, index_type i2, index_type i3) const stlsoft_throw_1(stlsoft_ns_qual_std(out_of_range) );
    void        range_check_(index_type i0, index_type i1, index_type i2) const stlsoft_throw_1(stlsoft_ns_qual_std(out_of_range) );
    void        range_check_(index_type i0) const stlsoft_throw_1(stlsoft_ns_qual_std(out_of_range) );

// Members
protected:
    M   m_data;

// Not to be implemented
private:
#if defined(__STLSOFT_COMPILER_IS_BORLAND) || \
    defined(__STLSOFT_COMPILER_IS_GCC) || \
    defined(__STLSOFT_COMPILER_IS_MSVC)
public:
#else
private:
#endif /* compiler */
    static_array_4d(class_type const &rhs);
private:
    class_type const &operator =(class_type const &rhs);
};

/* /////////////////////////////////////////////////////////////////////////////
 * Implementation
 */

#ifndef __STLSOFT_DOCUMENTATION_SKIP_SECTION

// static_array_1d

template <ss_typename_param_k T, ss_size_t N0, ss_typename_param_k P, ss_typename_param_k M>
inline ss_typename_type_k static_array_1d<T, N0, P, M>::pointer static_array_1d<T, N0, P, M>::data_()
{
    return &m_data[0];
}

template <ss_typename_param_k T, ss_size_t N0, ss_typename_param_k P, ss_typename_param_k M>
inline ss_typename_type_k static_array_1d<T, N0, P, M>::index_type static_array_1d<T, N0, P, M>::calc_index_(ss_typename_type_k static_array_1d<T, N0, P, M>::index_type i0) const
{
    return i0;
}

template <ss_typename_param_k T, ss_size_t N0, ss_typename_param_k P, ss_typename_param_k M>
inline void static_array_1d<T, N0, P, M>::range_check_(ss_typename_type_k static_array_1d<T, N0, P, M>::index_type i0) const stlsoft_throw_1(stlsoft_ns_qual_std(out_of_range) )
{
    STLSOFT_SUPPRESS_UNUSED(i0); STLSOFT_SUPPRESS_UNUSED(N0);

#ifdef __STLSOFT_CF_EXCEPTION_SUPPORT
    if(!(i0 < N0))
    {
        stlsoft_ns_qual_std(out_of_range)   x("static array index out of range");

#ifdef __STLSOFT_COMPILER_IS_MSVC
        if(1) // This is needed to avoid Visual C++ warning 4702: 'unreachable code'
#endif /* __STLSOFT_COMPILER_IS_MSVC */
        {
            throw x;
        }
    }
#else
    stlsoft_message_assert("static array index out of range", i0 < N0);
#endif /* __STLSOFT_CF_EXCEPTION_SUPPORT */
}

template <ss_typename_param_k T, ss_size_t N0, ss_typename_param_k P, ss_typename_param_k M>
inline static_array_1d<T, N0, P, M>::static_array_1d(T *data/* [N0] */)
    : m_data(data)
{}

template <ss_typename_param_k T, ss_size_t N0, ss_typename_param_k P, ss_typename_param_k M>
inline static_array_1d<T, N0, P, M>::static_array_1d()
{
    array_range_initialiser<T, allocator_type, P>::construct(*this, data_(), size());
}

template <ss_typename_param_k T, ss_size_t N0, ss_typename_param_k P, ss_typename_param_k M>
inline static_array_1d<T, N0, P, M>::static_array_1d(value_type const &t)
{
    array_range_initialiser<T, allocator_type, P>::construct(*this, data_(), size(), t);
}

template <ss_typename_param_k T, ss_size_t N0, ss_typename_param_k P, ss_typename_param_k M>
inline static_array_1d<T, N0, P, M>::~static_array_1d()
{
    if(!is_pointer_type<M>::value)
    {
        array_range_initialiser<T, allocator_type, P>::destroy(*this, data_(), size());
    }
}

template <ss_typename_param_k T, ss_size_t N0, ss_typename_param_k P, ss_typename_param_k M>
inline ss_typename_type_k static_array_1d<T, N0, P, M>::value_type &static_array_1d<T, N0, P, M>::at(ss_typename_type_k static_array_1d<T, N0, P, M>::index_type i0)
{
    range_check_(i0);

    return m_data[i0];
}

template <ss_typename_param_k T, ss_size_t N0, ss_typename_param_k P, ss_typename_param_k M>
inline ss_typename_type_k static_array_1d<T, N0, P, M>::value_type const &static_array_1d<T, N0, P, M>::at(ss_typename_type_k static_array_1d<T, N0, P, M>::index_type i0) const
{
    range_check_(i0);

    return m_data[i0];
}

template <ss_typename_param_k T, ss_size_t N0, ss_typename_param_k P, ss_typename_param_k M>
inline ss_typename_type_k static_array_1d<T, N0, P, M>::value_type &static_array_1d<T, N0, P, M>::at_unchecked(ss_typename_type_k static_array_1d<T, N0, P, M>::index_type i0)
{
    return m_data[i0];
}

template <ss_typename_param_k T, ss_size_t N0, ss_typename_param_k P, ss_typename_param_k M>
inline ss_typename_type_k static_array_1d<T, N0, P, M>::value_type const &static_array_1d<T, N0, P, M>::at_unchecked(ss_typename_type_k static_array_1d<T, N0, P, M>::index_type i0) const
{
    return m_data[i0];
}

template <ss_typename_param_k T, ss_size_t N0, ss_typename_param_k P, ss_typename_param_k M>
inline ss_typename_type_k static_array_1d<T, N0, P, M>::value_type &static_array_1d<T, N0, P, M>::operator [](ss_typename_type_k static_array_1d<T, N0, P, M>::index_type i0)
{
    return m_data[i0];
}

template <ss_typename_param_k T, ss_size_t N0, ss_typename_param_k P, ss_typename_param_k M>
inline ss_typename_type_k static_array_1d<T, N0, P, M>::value_type const &static_array_1d<T, N0, P, M>::operator [](ss_typename_type_k static_array_1d<T, N0, P, M>::index_type i0) const
{
    return m_data[i0];
}

template <ss_typename_param_k T, ss_size_t N0, ss_typename_param_k P, ss_typename_param_k M>
inline ss_typename_type_k static_array_1d<T, N0, P, M>::reference static_array_1d<T, N0, P, M>::front()
{
    return at(0);
}

template <ss_typename_param_k T, ss_size_t N0, ss_typename_param_k P, ss_typename_param_k M>
inline ss_typename_type_k static_array_1d<T, N0, P, M>::reference static_array_1d<T, N0, P, M>::back()
{
    return at(N0 - 1);
}

template <ss_typename_param_k T, ss_size_t N0, ss_typename_param_k P, ss_typename_param_k M>
inline ss_typename_type_k static_array_1d<T, N0, P, M>::const_reference static_array_1d<T, N0, P, M>::front() const
{
    return at(0);
}

template <ss_typename_param_k T, ss_size_t N0, ss_typename_param_k P, ss_typename_param_k M>
inline ss_typename_type_k static_array_1d<T, N0, P, M>::const_reference static_array_1d<T, N0, P, M>::back() const
{
    return at(N0 - 1);
}

template <ss_typename_param_k T, ss_size_t N0, ss_typename_param_k P, ss_typename_param_k M>
inline /* static */ ss_typename_type_k static_array_1d<T, N0, P, M>::index_type static_array_1d<T, N0, P, M>::dimension0()
{
    return N0;
}

template <ss_typename_param_k T, ss_size_t N0, ss_typename_param_k P, ss_typename_param_k M>
inline /* static */ ss_typename_type_k static_array_1d<T, N0, P, M>::index_type static_array_1d<T, N0, P, M>::size()
{
    return N0;
}

template <ss_typename_param_k T, ss_size_t N0, ss_typename_param_k P, ss_typename_param_k M>
inline /* static */ ss_bool_t static_array_1d<T, N0, P, M>::empty()
{
    return false;
}

template <ss_typename_param_k T, ss_size_t N0, ss_typename_param_k P, ss_typename_param_k M>
inline /* static */ ss_typename_type_k static_array_1d<T, N0, P, M>::index_type static_array_1d<T, N0, P, M>::max_size()
{
    return size();
}

template <ss_typename_param_k T, ss_size_t N0, ss_typename_param_k P, ss_typename_param_k M>
inline ss_typename_type_k static_array_1d<T, N0, P, M>::iterator static_array_1d<T, N0, P, M>::begin()
{
    return m_data;
}

template <ss_typename_param_k T, ss_size_t N0, ss_typename_param_k P, ss_typename_param_k M>
inline ss_typename_type_k static_array_1d<T, N0, P, M>::iterator static_array_1d<T, N0, P, M>::end()
{
    return m_data + size();
}

template <ss_typename_param_k T, ss_size_t N0, ss_typename_param_k P, ss_typename_param_k M>
inline ss_typename_type_k static_array_1d<T, N0, P, M>::const_iterator static_array_1d<T, N0, P, M>::begin() const
{
    return m_data;
}

template <ss_typename_param_k T, ss_size_t N0, ss_typename_param_k P, ss_typename_param_k M>
inline ss_typename_type_k static_array_1d<T, N0, P, M>::const_iterator static_array_1d<T, N0, P, M>::end() const
{
    return m_data + size();
}

#ifdef __STLSOFT_CF_BIDIRECTIONAL_ITERATOR_SUPPORT
template <ss_typename_param_k T, ss_size_t N0, ss_typename_param_k P, ss_typename_param_k M>
inline ss_typename_type_k static_array_1d<T, N0, P, M>::reverse_iterator static_array_1d<T, N0, P, M>::rbegin()
{
    return reverse_iterator(end());
}

template <ss_typename_param_k T, ss_size_t N0, ss_typename_param_k P, ss_typename_param_k M>
inline ss_typename_type_k static_array_1d<T, N0, P, M>::reverse_iterator static_array_1d<T, N0, P, M>::rend()
{
    return reverse_iterator(begin());
}

template <ss_typename_param_k T, ss_size_t N0, ss_typename_param_k P, ss_typename_param_k M>
inline ss_typename_type_k static_array_1d<T, N0, P, M>::const_reverse_iterator static_array_1d<T, N0, P, M>::rbegin() const
{
    return const_reverse_iterator(end());
}

template <ss_typename_param_k T, ss_size_t N0, ss_typename_param_k P, ss_typename_param_k M>
inline ss_typename_type_k static_array_1d<T, N0, P, M>::const_reverse_iterator static_array_1d<T, N0, P, M>::rend() const
{
    return const_reverse_iterator(begin());
}
#endif /* __STLSOFT_CF_BIDIRECTIONAL_ITERATOR_SUPPORT */

template <ss_typename_param_k T, ss_size_t N0, ss_typename_param_k P, ss_typename_param_k M>
inline ss_typename_type_k static_array_1d<T, N0, P, M>::value_type const *static_array_1d<T, N0, P, M>::data() const
{
    return m_data;
}


// static_array_2d

template <ss_typename_param_k T, ss_size_t N0, ss_size_t N1, ss_typename_param_k P, ss_typename_param_k M>
inline ss_typename_type_k static_array_2d<T, N0, N1, P, M>::pointer static_array_2d<T, N0, N1, P, M>::data_()
{
    return &m_data[0];
}

template <ss_typename_param_k T, ss_size_t N0, ss_size_t N1, ss_typename_param_k P, ss_typename_param_k M>
inline ss_typename_type_k static_array_2d<T, N0, N1, P, M>::index_type static_array_2d<T, N0, N1, P, M>::calc_index_(ss_typename_type_k static_array_2d<T, N0, N1, P, M>::index_type i0, ss_typename_type_k static_array_2d<T, N0, N1, P, M>::index_type i1) const
{
    return (i0 * N1) + i1;
}

template <ss_typename_param_k T, ss_size_t N0, ss_size_t N1, ss_typename_param_k P, ss_typename_param_k M>
inline void static_array_2d<T, N0, N1, P, M>::range_check_(ss_typename_type_k static_array_2d<T, N0, N1, P, M>::index_type i0, ss_typename_type_k static_array_2d<T, N0, N1, P, M>::index_type i1) const stlsoft_throw_1(stlsoft_ns_qual_std(out_of_range) )
{
    STLSOFT_SUPPRESS_UNUSED(i0); STLSOFT_SUPPRESS_UNUSED(N0); STLSOFT_SUPPRESS_UNUSED(i1); STLSOFT_SUPPRESS_UNUSED(N1);

#ifdef __STLSOFT_CF_EXCEPTION_SUPPORT
    if( !(i0 < N0) ||
        !(i1 < N1))
    {
        stlsoft_ns_qual_std(out_of_range)   x("static array index out of range");

#ifdef __STLSOFT_COMPILER_IS_MSVC
        if(1) // This is needed to avoid Visual C++ warning 4702: 'unreachable code'
#endif /* __STLSOFT_COMPILER_IS_MSVC */
        {
            throw x;
        }
    }
#else
    stlsoft_message_assert("static array index out of range", i0 < N0 && i1 < N1);
#endif /* __STLSOFT_CF_EXCEPTION_SUPPORT */
}

template <ss_typename_param_k T, ss_size_t N0, ss_size_t N1, ss_typename_param_k P, ss_typename_param_k M>
inline void static_array_2d<T, N0, N1, P, M>::range_check_(ss_typename_type_k static_array_2d<T, N0, N1, P, M>::index_type i0) const stlsoft_throw_1(stlsoft_ns_qual_std(out_of_range) )
{
    STLSOFT_SUPPRESS_UNUSED(i0); STLSOFT_SUPPRESS_UNUSED(N0);

#ifdef __STLSOFT_CF_EXCEPTION_SUPPORT
    if(!(i0 < N0))
    {
        stlsoft_ns_qual_std(out_of_range)   x("static array index out of range");

#ifdef __STLSOFT_COMPILER_IS_MSVC
        if(1) // This is needed to avoid Visual C++ warning 4702: 'unreachable code'
#endif /* __STLSOFT_COMPILER_IS_MSVC */
        {
            throw x;
        }
    }
#else
    stlsoft_message_assert("static array index out of range", i0 < N0);
#endif /* __STLSOFT_CF_EXCEPTION_SUPPORT */
}

template <ss_typename_param_k T, ss_size_t N0, ss_size_t N1, ss_typename_param_k P, ss_typename_param_k M>
inline static_array_2d<T, N0, N1, P, M>::static_array_2d(T *data)
    : m_data(data)
{}

template <ss_typename_param_k T, ss_size_t N0, ss_size_t N1, ss_typename_param_k P, ss_typename_param_k M>
inline static_array_2d<T, N0, N1, P, M>::static_array_2d()
{
    array_range_initialiser<T, allocator_type, P>::construct(*this, data_(), size());
}

template <ss_typename_param_k T, ss_size_t N0, ss_size_t N1, ss_typename_param_k P, ss_typename_param_k M>
inline static_array_2d<T, N0, N1, P, M>::static_array_2d(value_type const &t)
{
    array_range_initialiser<T, allocator_type, P>::construct(*this, data_(), size(), t);
}

template <ss_typename_param_k T, ss_size_t N0, ss_size_t N1, ss_typename_param_k P, ss_typename_param_k M>
inline static_array_2d<T, N0, N1, P, M>::~static_array_2d()
{
    if(!is_pointer_type<M>::value)
    {
        array_range_initialiser<T, allocator_type, P>::destroy(*this, data_(), size());
    }
}

template <ss_typename_param_k T, ss_size_t N0, ss_size_t N1, ss_typename_param_k P, ss_typename_param_k M>
inline ss_typename_type_k static_array_2d<T, N0, N1, P, M>::reference static_array_2d<T, N0, N1, P, M>::at(ss_typename_type_k static_array_2d<T, N0, N1, P, M>::index_type i0, ss_typename_type_k static_array_2d<T, N0, N1, P, M>::index_type i1)
{
    range_check_(i0, i1);

    return *(m_data + calc_index_(i0, i1));
}

template <ss_typename_param_k T, ss_size_t N0, ss_size_t N1, ss_typename_param_k P, ss_typename_param_k M>
inline ss_typename_type_k static_array_2d<T, N0, N1, P, M>::const_reference static_array_2d<T, N0, N1, P, M>::at(ss_typename_type_k static_array_2d<T, N0, N1, P, M>::index_type i0, ss_typename_type_k static_array_2d<T, N0, N1, P, M>::index_type i1) const
{
    range_check_(i0, i1);

    return *(m_data + calc_index_(i0, i1));
}

template <ss_typename_param_k T, ss_size_t N0, ss_size_t N1, ss_typename_param_k P, ss_typename_param_k M>
inline ss_typename_type_k static_array_2d<T, N0, N1, P, M>::reference static_array_2d<T, N0, N1, P, M>::at_unchecked(ss_typename_type_k static_array_2d<T, N0, N1, P, M>::index_type i0, ss_typename_type_k static_array_2d<T, N0, N1, P, M>::index_type i1)
{
    stlsoft_message_assert("static array index out of range", i0 < N0 && i1 < N1);

    return *(m_data + calc_index_(i0, i1));
}

template <ss_typename_param_k T, ss_size_t N0, ss_size_t N1, ss_typename_param_k P, ss_typename_param_k M>
inline ss_typename_type_k static_array_2d<T, N0, N1, P, M>::const_reference static_array_2d<T, N0, N1, P, M>::at_unchecked(ss_typename_type_k static_array_2d<T, N0, N1, P, M>::index_type i0, ss_typename_type_k static_array_2d<T, N0, N1, P, M>::index_type i1) const
{
    stlsoft_message_assert("static array index out of range", i0 < N0 && i1 < N1);

    return *(m_data + calc_index_(i0, i1));
}

template <ss_typename_param_k T, ss_size_t N0, ss_size_t N1, ss_typename_param_k P, ss_typename_param_k M>
inline ss_typename_type_k static_array_2d<T, N0, N1, P, M>::dimension_type static_array_2d<T, N0, N1, P, M>::at(ss_typename_type_k static_array_2d<T, N0, N1, P, M>::index_type i0)
{
    range_check_(i0);

    return dimension_type(m_data + i0 * N1);
}

template <ss_typename_param_k T, ss_size_t N0, ss_size_t N1, ss_typename_param_k P, ss_typename_param_k M>
inline ss_typename_type_k static_array_2d<T, N0, N1, P, M>::dimension_type const static_array_2d<T, N0, N1, P, M>::at(ss_typename_type_k static_array_2d<T, N0, N1, P, M>::index_type i0) const
{
    range_check_(i0);

    return dimension_type(m_data + i0 * N1);
}

template <ss_typename_param_k T, ss_size_t N0, ss_size_t N1, ss_typename_param_k P, ss_typename_param_k M>
inline ss_typename_type_k static_array_2d<T, N0, N1, P, M>::dimension_type static_array_2d<T, N0, N1, P, M>::at_unchecked(ss_typename_type_k static_array_2d<T, N0, N1, P, M>::index_type i0)
{
    return dimension_type(m_data + i0 * N1);
}

template <ss_typename_param_k T, ss_size_t N0, ss_size_t N1, ss_typename_param_k P, ss_typename_param_k M>
inline ss_typename_type_k static_array_2d<T, N0, N1, P, M>::dimension_type const static_array_2d<T, N0, N1, P, M>::at_unchecked(ss_typename_type_k static_array_2d<T, N0, N1, P, M>::index_type i0) const
{
    return dimension_type(m_data + i0 * N1);
}

template <ss_typename_param_k T, ss_size_t N0, ss_size_t N1, ss_typename_param_k P, ss_typename_param_k M>
inline ss_typename_type_k static_array_2d<T, N0, N1, P, M>::dimension_type static_array_2d<T, N0, N1, P, M>::operator [](ss_typename_type_k static_array_2d<T, N0, N1, P, M>::index_type i0)
{
    return dimension_type(m_data + i0 * N1);
}

template <ss_typename_param_k T, ss_size_t N0, ss_size_t N1, ss_typename_param_k P, ss_typename_param_k M>
inline const ss_typename_type_k static_array_2d<T, N0, N1, P, M>::dimension_type static_array_2d<T, N0, N1, P, M>::operator [](ss_typename_type_k static_array_2d<T, N0, N1, P, M>::index_type i0) const
{
    return dimension_type(m_data + i0 * N1);
}

template <ss_typename_param_k T, ss_size_t N0, ss_size_t N1, ss_typename_param_k P, ss_typename_param_k M>
inline ss_typename_type_k static_array_2d<T, N0, N1, P, M>::reference static_array_2d<T, N0, N1, P, M>::front()
{
    return at(0, 0);
}

template <ss_typename_param_k T, ss_size_t N0, ss_size_t N1, ss_typename_param_k P, ss_typename_param_k M>
inline ss_typename_type_k static_array_2d<T, N0, N1, P, M>::reference static_array_2d<T, N0, N1, P, M>::back()
{
    return at(N0 - 1, N1 - 1);
}

template <ss_typename_param_k T, ss_size_t N0, ss_size_t N1, ss_typename_param_k P, ss_typename_param_k M>
inline ss_typename_type_k static_array_2d<T, N0, N1, P, M>::const_reference static_array_2d<T, N0, N1, P, M>::front() const
{
    return at(0, 0);
}

template <ss_typename_param_k T, ss_size_t N0, ss_size_t N1, ss_typename_param_k P, ss_typename_param_k M>
inline ss_typename_type_k static_array_2d<T, N0, N1, P, M>::const_reference static_array_2d<T, N0, N1, P, M>::back() const
{
    return at(N0 - 1, N1 - 1);
}

template <ss_typename_param_k T, ss_size_t N0, ss_size_t N1, ss_typename_param_k P, ss_typename_param_k M>
inline /* static */ ss_typename_type_k static_array_2d<T, N0, N1, P, M>::index_type static_array_2d<T, N0, N1, P, M>::dimension0()
{
    return N0;
}

template <ss_typename_param_k T, ss_size_t N0, ss_size_t N1, ss_typename_param_k P, ss_typename_param_k M>
inline /* static */ ss_typename_type_k static_array_2d<T, N0, N1, P, M>::index_type static_array_2d<T, N0, N1, P, M>::dimension1()
{
    return N1;
}

template <ss_typename_param_k T, ss_size_t N0, ss_size_t N1, ss_typename_param_k P, ss_typename_param_k M>
inline /* static */ ss_typename_type_k static_array_2d<T, N0, N1, P, M>::index_type static_array_2d<T, N0, N1, P, M>::size()
{
    return N0 * N1;
}

template <ss_typename_param_k T, ss_size_t N0, ss_size_t N1, ss_typename_param_k P, ss_typename_param_k M>
inline /* static */ ss_bool_t static_array_2d<T, N0, N1, P, M>::empty()
{
    return false;
}

template <ss_typename_param_k T, ss_size_t N0, ss_size_t N1, ss_typename_param_k P, ss_typename_param_k M>
inline /* static */ ss_typename_type_k static_array_2d<T, N0, N1, P, M>::index_type static_array_2d<T, N0, N1, P, M>::max_size()
{
    return size();
}

template <ss_typename_param_k T, ss_size_t N0, ss_size_t N1, ss_typename_param_k P, ss_typename_param_k M>
inline ss_typename_type_k static_array_2d<T, N0, N1, P, M>::iterator static_array_2d<T, N0, N1, P, M>::begin()
{
    return m_data;
}

template <ss_typename_param_k T, ss_size_t N0, ss_size_t N1, ss_typename_param_k P, ss_typename_param_k M>
inline ss_typename_type_k static_array_2d<T, N0, N1, P, M>::iterator static_array_2d<T, N0, N1, P, M>::end()
{
    return m_data + size();
}

template <ss_typename_param_k T, ss_size_t N0, ss_size_t N1, ss_typename_param_k P, ss_typename_param_k M>
inline ss_typename_type_k static_array_2d<T, N0, N1, P, M>::const_iterator static_array_2d<T, N0, N1, P, M>::begin() const
{
    return m_data;
}

template <ss_typename_param_k T, ss_size_t N0, ss_size_t N1, ss_typename_param_k P, ss_typename_param_k M>
inline ss_typename_type_k static_array_2d<T, N0, N1, P, M>::const_iterator static_array_2d<T, N0, N1, P, M>::end() const
{
    return m_data + size();
}

#ifdef __STLSOFT_CF_BIDIRECTIONAL_ITERATOR_SUPPORT
template <ss_typename_param_k T, ss_size_t N0, ss_size_t N1, ss_typename_param_k P, ss_typename_param_k M>
inline ss_typename_type_k static_array_2d<T, N0, N1, P, M>::reverse_iterator static_array_2d<T, N0, N1, P, M>::rbegin()
{
    return reverse_iterator(end());
}

template <ss_typename_param_k T, ss_size_t N0, ss_size_t N1, ss_typename_param_k P, ss_typename_param_k M>
inline ss_typename_type_k static_array_2d<T, N0, N1, P, M>::reverse_iterator static_array_2d<T, N0, N1, P, M>::rend()
{
    return reverse_iterator(begin());
}

template <ss_typename_param_k T, ss_size_t N0, ss_size_t N1, ss_typename_param_k P, ss_typename_param_k M>
inline ss_typename_type_k static_array_2d<T, N0, N1, P, M>::const_reverse_iterator static_array_2d<T, N0, N1, P, M>::rbegin() const
{
    return const_reverse_iterator(end());
}

template <ss_typename_param_k T, ss_size_t N0, ss_size_t N1, ss_typename_param_k P, ss_typename_param_k M>
inline ss_typename_type_k static_array_2d<T, N0, N1, P, M>::const_reverse_iterator static_array_2d<T, N0, N1, P, M>::rend() const
{
    return const_reverse_iterator(begin());
}
#endif /* __STLSOFT_CF_BIDIRECTIONAL_ITERATOR_SUPPORT */

template <ss_typename_param_k T, ss_size_t N0, ss_size_t N1, ss_typename_param_k P, ss_typename_param_k M>
inline ss_typename_type_k static_array_2d<T, N0, N1, P, M>::value_type const *static_array_2d<T, N0, N1, P, M>::data() const
{
    return m_data;
}

// static_array_3d

template <ss_typename_param_k T, ss_size_t N0, ss_size_t N1, ss_size_t N2, ss_typename_param_k P, ss_typename_param_k M>
inline ss_typename_type_k static_array_3d<T, N0, N1, N2, P, M>::pointer static_array_3d<T, N0, N1, N2, P, M>::data_()
{
    return &m_data[0];
}

template <ss_typename_param_k T, ss_size_t N0, ss_size_t N1, ss_size_t N2, ss_typename_param_k P, ss_typename_param_k M>
inline ss_typename_type_k static_array_3d<T, N0, N1, N2, P, M>::index_type static_array_3d<T, N0, N1, N2, P, M>::calc_index_(ss_typename_type_k static_array_3d<T, N0, N1, N2, P, M>::index_type i0, ss_typename_type_k static_array_3d<T, N0, N1, N2, P, M>::index_type i1, ss_typename_type_k static_array_3d<T, N0, N1, N2, P, M>::index_type i2) const
{
    return ((i0 * N1) + i1) * N2 + i2;
}

template <ss_typename_param_k T, ss_size_t N0, ss_size_t N1, ss_size_t N2, ss_typename_param_k P, ss_typename_param_k M>
inline void static_array_3d<T, N0, N1, N2, P, M>::range_check_(ss_typename_type_k static_array_3d<T, N0, N1, N2, P, M>::index_type i0, ss_typename_type_k static_array_3d<T, N0, N1, N2, P, M>::index_type i1, ss_typename_type_k static_array_3d<T, N0, N1, N2, P, M>::index_type i2) const stlsoft_throw_1(stlsoft_ns_qual_std(out_of_range) )
{
    STLSOFT_SUPPRESS_UNUSED(i0); STLSOFT_SUPPRESS_UNUSED(N0); STLSOFT_SUPPRESS_UNUSED(i1); STLSOFT_SUPPRESS_UNUSED(N1); STLSOFT_SUPPRESS_UNUSED(i2); STLSOFT_SUPPRESS_UNUSED(N2);

#ifdef __STLSOFT_CF_EXCEPTION_SUPPORT
    if( !(i0 < N0) ||
        !(i1 < N1) ||
        !(i2 < N2))
    {
        stlsoft_ns_qual_std(out_of_range)   x("static array index out of range");

#ifdef __STLSOFT_COMPILER_IS_MSVC
        if(1) // This is needed to avoid Visual C++ warning 4702: 'unreachable code'
#endif /* __STLSOFT_COMPILER_IS_MSVC */
        {
            throw x;
        }
    }
#else
    stlsoft_message_assert("static array index out of range", i0 < N0 && i1 < N1 && i2 < N2);
#endif /* __STLSOFT_CF_EXCEPTION_SUPPORT */
}

template <ss_typename_param_k T, ss_size_t N0, ss_size_t N1, ss_size_t N2, ss_typename_param_k P, ss_typename_param_k M>
inline void static_array_3d<T, N0, N1, N2, P, M>::range_check_(ss_typename_type_k static_array_3d<T, N0, N1, N2, P, M>::index_type i0) const stlsoft_throw_1(stlsoft_ns_qual_std(out_of_range) )
{
    STLSOFT_SUPPRESS_UNUSED(i0); STLSOFT_SUPPRESS_UNUSED(N0);

#ifdef __STLSOFT_CF_EXCEPTION_SUPPORT
    if(!(i0 < N0))
    {
        stlsoft_ns_qual_std(out_of_range)   x("static array index out of range");

#ifdef __STLSOFT_COMPILER_IS_MSVC
        if(1) // This is needed to avoid Visual C++ warning 4702: 'unreachable code'
#endif /* __STLSOFT_COMPILER_IS_MSVC */
        {
            throw x;
        }
    }
#else
    stlsoft_message_assert("static array index out of range", i0 < N0);
#endif /* __STLSOFT_CF_EXCEPTION_SUPPORT */
}

template <ss_typename_param_k T, ss_size_t N0, ss_size_t N1, ss_size_t N2, ss_typename_param_k P, ss_typename_param_k M>
inline static_array_3d<T, N0, N1, N2, P, M>::static_array_3d(T *data)
    : m_data(data)
{}

template <ss_typename_param_k T, ss_size_t N0, ss_size_t N1, ss_size_t N2, ss_typename_param_k P, ss_typename_param_k M>
inline static_array_3d<T, N0, N1, N2, P, M>::static_array_3d()
{
    array_range_initialiser<T, allocator_type, P>::construct(*this, data_(), size());
}

template <ss_typename_param_k T, ss_size_t N0, ss_size_t N1, ss_size_t N2, ss_typename_param_k P, ss_typename_param_k M>
inline static_array_3d<T, N0, N1, N2, P, M>::static_array_3d(value_type const &t)
{
    array_range_initialiser<T, allocator_type, P>::construct(*this, data_(), size(), t);
}

template <ss_typename_param_k T, ss_size_t N0, ss_size_t N1, ss_size_t N2, ss_typename_param_k P, ss_typename_param_k M>
inline static_array_3d<T, N0, N1, N2, P, M>::~static_array_3d()
{
    if(!is_pointer_type<M>::value)
    {
        array_range_initialiser<T, allocator_type, P>::destroy(*this, data_(), size());
    }
}

template <ss_typename_param_k T, ss_size_t N0, ss_size_t N1, ss_size_t N2, ss_typename_param_k P, ss_typename_param_k M>
inline ss_typename_type_k static_array_3d<T, N0, N1, N2, P, M>::value_type &static_array_3d<T, N0, N1, N2, P, M>::at(ss_typename_type_k static_array_3d<T, N0, N1, N2, P, M>::index_type i0, ss_typename_type_k static_array_3d<T, N0, N1, N2, P, M>::index_type i1, ss_typename_type_k static_array_3d<T, N0, N1, N2, P, M>::index_type i2)
{
    range_check_(i0, i1, i2);

    return *(m_data + calc_index_(i0, i1, i2));
}

template <ss_typename_param_k T, ss_size_t N0, ss_size_t N1, ss_size_t N2, ss_typename_param_k P, ss_typename_param_k M>
inline ss_typename_type_k static_array_3d<T, N0, N1, N2, P, M>::value_type const &static_array_3d<T, N0, N1, N2, P, M>::at(ss_typename_type_k static_array_3d<T, N0, N1, N2, P, M>::index_type i0, ss_typename_type_k static_array_3d<T, N0, N1, N2, P, M>::index_type i1, ss_typename_type_k static_array_3d<T, N0, N1, N2, P, M>::index_type i2) const
{
    range_check_(i0, i1, i2);

    return *(m_data + calc_index_(i0, i1, i2));
}

template <ss_typename_param_k T, ss_size_t N0, ss_size_t N1, ss_size_t N2, ss_typename_param_k P, ss_typename_param_k M>
inline ss_typename_type_k static_array_3d<T, N0, N1, N2, P, M>::value_type &static_array_3d<T, N0, N1, N2, P, M>::at_unchecked(ss_typename_type_k static_array_3d<T, N0, N1, N2, P, M>::index_type i0, ss_typename_type_k static_array_3d<T, N0, N1, N2, P, M>::index_type i1, ss_typename_type_k static_array_3d<T, N0, N1, N2, P, M>::index_type i2)
{
    stlsoft_message_assert("static array index out of range", i0 < N0 && i1 < N1 && i2 < N2);

    return *(m_data + calc_index_(i0, i1, i2));
}

template <ss_typename_param_k T, ss_size_t N0, ss_size_t N1, ss_size_t N2, ss_typename_param_k P, ss_typename_param_k M>
inline ss_typename_type_k static_array_3d<T, N0, N1, N2, P, M>::value_type const &static_array_3d<T, N0, N1, N2, P, M>::at_unchecked(ss_typename_type_k static_array_3d<T, N0, N1, N2, P, M>::index_type i0, ss_typename_type_k static_array_3d<T, N0, N1, N2, P, M>::index_type i1, ss_typename_type_k static_array_3d<T, N0, N1, N2, P, M>::index_type i2) const
{
    stlsoft_message_assert("static array index out of range", i0 < N0 && i1 < N1 && i2 < N2);

    return *(m_data + calc_index_(i0, i1, i2));
}

template <ss_typename_param_k T, ss_size_t N0, ss_size_t N1, ss_size_t N2, ss_typename_param_k P, ss_typename_param_k M>
inline ss_typename_type_k static_array_3d<T, N0, N1, N2, P, M>::dimension_type static_array_3d<T, N0, N1, N2, P, M>::at(ss_typename_type_k static_array_3d<T, N0, N1, N2, P, M>::index_type i0)
{
    range_check_(i0);

    return dimension_type(m_data + i0 * N1 * N2);
}

template <ss_typename_param_k T, ss_size_t N0, ss_size_t N1, ss_size_t N2, ss_typename_param_k P, ss_typename_param_k M>
inline const ss_typename_type_k static_array_3d<T, N0, N1, N2, P, M>::dimension_type static_array_3d<T, N0, N1, N2, P, M>::at(ss_typename_type_k static_array_3d<T, N0, N1, N2, P, M>::index_type i0) const
{
    range_check_(i0);

    return dimension_type(m_data + i0 * N1 * N2);
}

template <ss_typename_param_k T, ss_size_t N0, ss_size_t N1, ss_size_t N2, ss_typename_param_k P, ss_typename_param_k M>
inline ss_typename_type_k static_array_3d<T, N0, N1, N2, P, M>::dimension_type static_array_3d<T, N0, N1, N2, P, M>::at_unchecked(ss_typename_type_k static_array_3d<T, N0, N1, N2, P, M>::index_type i0)
{
    return dimension_type(m_data + i0 * N1 * N2);
}

template <ss_typename_param_k T, ss_size_t N0, ss_size_t N1, ss_size_t N2, ss_typename_param_k P, ss_typename_param_k M>
inline const ss_typename_type_k static_array_3d<T, N0, N1, N2, P, M>::dimension_type static_array_3d<T, N0, N1, N2, P, M>::at_unchecked(ss_typename_type_k static_array_3d<T, N0, N1, N2, P, M>::index_type i0) const
{
    return dimension_type(m_data + i0 * N1 * N2);
}

template <ss_typename_param_k T, ss_size_t N0, ss_size_t N1, ss_size_t N2, ss_typename_param_k P, ss_typename_param_k M>
inline ss_typename_type_k static_array_3d<T, N0, N1, N2, P, M>::dimension_type static_array_3d<T, N0, N1, N2, P, M>::operator [](ss_typename_type_k static_array_3d<T, N0, N1, N2, P, M>::index_type i0)
{
    return dimension_type(m_data + i0 * N1 * N2);
}

template <ss_typename_param_k T, ss_size_t N0, ss_size_t N1, ss_size_t N2, ss_typename_param_k P, ss_typename_param_k M>
inline const ss_typename_type_k static_array_3d<T, N0, N1, N2, P, M>::dimension_type static_array_3d<T, N0, N1, N2, P, M>::operator [](ss_typename_type_k static_array_3d<T, N0, N1, N2, P, M>::index_type i0) const
{
    return dimension_type(m_data + i0 * N1 * N2);
}

template <ss_typename_param_k T, ss_size_t N0, ss_size_t N1, ss_size_t N2, ss_typename_param_k P, ss_typename_param_k M>
inline ss_typename_type_k static_array_3d<T, N0, N1, N2, P, M>::reference static_array_3d<T, N0, N1, N2, P, M>::front()
{
    return at(0, 0, 0);
}

template <ss_typename_param_k T, ss_size_t N0, ss_size_t N1, ss_size_t N2, ss_typename_param_k P, ss_typename_param_k M>
inline ss_typename_type_k static_array_3d<T, N0, N1, N2, P, M>::reference static_array_3d<T, N0, N1, N2, P, M>::back()
{
    return at(N0 - 1, N1 - 1, N2 - 1);
}

template <ss_typename_param_k T, ss_size_t N0, ss_size_t N1, ss_size_t N2, ss_typename_param_k P, ss_typename_param_k M>
inline ss_typename_type_k static_array_3d<T, N0, N1, N2, P, M>::const_reference static_array_3d<T, N0, N1, N2, P, M>::front() const
{
    return at(0, 0, 0);
}

template <ss_typename_param_k T, ss_size_t N0, ss_size_t N1, ss_size_t N2, ss_typename_param_k P, ss_typename_param_k M>
inline ss_typename_type_k static_array_3d<T, N0, N1, N2, P, M>::const_reference static_array_3d<T, N0, N1, N2, P, M>::back() const
{
    return at(N0 - 1, N1 - 1, N2 - 1);
}

template <ss_typename_param_k T, ss_size_t N0, ss_size_t N1, ss_size_t N2, ss_typename_param_k P, ss_typename_param_k M>
inline /* static */ ss_typename_type_k static_array_3d<T, N0, N1, N2, P, M>::index_type static_array_3d<T, N0, N1, N2, P, M>::dimension0()
{
    return N0;
}

template <ss_typename_param_k T, ss_size_t N0, ss_size_t N1, ss_size_t N2, ss_typename_param_k P, ss_typename_param_k M>
inline /* static */ ss_typename_type_k static_array_3d<T, N0, N1, N2, P, M>::index_type static_array_3d<T, N0, N1, N2, P, M>::dimension1()
{
    return N1;
}

template <ss_typename_param_k T, ss_size_t N0, ss_size_t N1, ss_size_t N2, ss_typename_param_k P, ss_typename_param_k M>
inline /* static */ ss_typename_type_k static_array_3d<T, N0, N1, N2, P, M>::index_type static_array_3d<T, N0, N1, N2, P, M>::dimension2()
{
    return N2;
}

template <ss_typename_param_k T, ss_size_t N0, ss_size_t N1, ss_size_t N2, ss_typename_param_k P, ss_typename_param_k M>
inline /* static */ ss_typename_type_k static_array_3d<T, N0, N1, N2, P, M>::index_type static_array_3d<T, N0, N1, N2, P, M>::size()
{
    return N0 * N1 * N2;
}

template <ss_typename_param_k T, ss_size_t N0, ss_size_t N1, ss_size_t N2, ss_typename_param_k P, ss_typename_param_k M>
inline /* static */ ss_bool_t static_array_3d<T, N0, N1, N2, P, M>::empty()
{
    return false;
}

template <ss_typename_param_k T, ss_size_t N0, ss_size_t N1, ss_size_t N2, ss_typename_param_k P, ss_typename_param_k M>
inline /* static */ ss_typename_type_k static_array_3d<T, N0, N1, N2, P, M>::index_type static_array_3d<T, N0, N1, N2, P, M>::max_size()
{
    return size();
}

template <ss_typename_param_k T, ss_size_t N0, ss_size_t N1, ss_size_t N2, ss_typename_param_k P, ss_typename_param_k M>
inline ss_typename_type_k static_array_3d<T, N0, N1, N2, P, M>::iterator static_array_3d<T, N0, N1, N2, P, M>::begin()
{
    return m_data;
}

template <ss_typename_param_k T, ss_size_t N0, ss_size_t N1, ss_size_t N2, ss_typename_param_k P, ss_typename_param_k M>
inline ss_typename_type_k static_array_3d<T, N0, N1, N2, P, M>::iterator static_array_3d<T, N0, N1, N2, P, M>::end()
{
    return m_data + size();
}

template <ss_typename_param_k T, ss_size_t N0, ss_size_t N1, ss_size_t N2, ss_typename_param_k P, ss_typename_param_k M>
inline ss_typename_type_k static_array_3d<T, N0, N1, N2, P, M>::const_iterator static_array_3d<T, N0, N1, N2, P, M>::begin() const
{
    return m_data;
}

template <ss_typename_param_k T, ss_size_t N0, ss_size_t N1, ss_size_t N2, ss_typename_param_k P, ss_typename_param_k M>
inline ss_typename_type_k static_array_3d<T, N0, N1, N2, P, M>::const_iterator static_array_3d<T, N0, N1, N2, P, M>::end() const
{
    return m_data + size();
}

#ifdef __STLSOFT_CF_BIDIRECTIONAL_ITERATOR_SUPPORT
template <ss_typename_param_k T, ss_size_t N0, ss_size_t N1, ss_size_t N2, ss_typename_param_k P, ss_typename_param_k M>
inline ss_typename_type_k static_array_3d<T, N0, N1, N2, P, M>::reverse_iterator static_array_3d<T, N0, N1, N2, P, M>::rbegin()
{
    return reverse_iterator(end());
}

template <ss_typename_param_k T, ss_size_t N0, ss_size_t N1, ss_size_t N2, ss_typename_param_k P, ss_typename_param_k M>
inline ss_typename_type_k static_array_3d<T, N0, N1, N2, P, M>::reverse_iterator static_array_3d<T, N0, N1, N2, P, M>::rend()
{
    return reverse_iterator(begin());
}

template <ss_typename_param_k T, ss_size_t N0, ss_size_t N1, ss_size_t N2, ss_typename_param_k P, ss_typename_param_k M>
inline ss_typename_type_k static_array_3d<T, N0, N1, N2, P, M>::const_reverse_iterator static_array_3d<T, N0, N1, N2, P, M>::rbegin() const
{
    return const_reverse_iterator(end());
}

template <ss_typename_param_k T, ss_size_t N0, ss_size_t N1, ss_size_t N2, ss_typename_param_k P, ss_typename_param_k M>
inline ss_typename_type_k static_array_3d<T, N0, N1, N2, P, M>::const_reverse_iterator static_array_3d<T, N0, N1, N2, P, M>::rend() const
{
    return const_reverse_iterator(begin());
}
#endif /* __STLSOFT_CF_BIDIRECTIONAL_ITERATOR_SUPPORT */

template <ss_typename_param_k T, ss_size_t N0, ss_size_t N1, ss_size_t N2, ss_typename_param_k P, ss_typename_param_k M>
inline ss_typename_type_k static_array_3d<T, N0, N1, N2, P, M>::value_type const *static_array_3d<T, N0, N1, N2, P, M>::data() const
{
    return m_data;
}

// static_array_4d

template <ss_typename_param_k T, ss_size_t N0, ss_size_t N1, ss_size_t N2, ss_size_t N3, ss_typename_param_k P, ss_typename_param_k M>
inline ss_typename_type_k static_array_4d<T, N0, N1, N2, N3, P, M>::pointer static_array_4d<T, N0, N1, N2, N3, P, M>::data_()
{
    return &m_data[0];
}

template <ss_typename_param_k T, ss_size_t N0, ss_size_t N1, ss_size_t N2, ss_size_t N3, ss_typename_param_k P, ss_typename_param_k M>
inline ss_typename_type_k static_array_4d<T, N0, N1, N2, N3, P, M>::index_type static_array_4d<T, N0, N1, N2, N3, P, M>::calc_index_(ss_typename_type_k static_array_4d<T, N0, N1, N2, N3, P, M>::index_type i0, ss_typename_type_k static_array_4d<T, N0, N1, N2, N3, P, M>::index_type i1, ss_typename_type_k static_array_4d<T, N0, N1, N2, N3, P, M>::index_type i2, ss_typename_type_k static_array_4d<T, N0, N1, N2, N3, P, M>::index_type i3) const
{
    return (((i0 * N1) + i1) * N2 + i2) * N3 + i3;
}

template <ss_typename_param_k T, ss_size_t N0, ss_size_t N1, ss_size_t N2, ss_size_t N3, ss_typename_param_k P, ss_typename_param_k M>
inline void static_array_4d<T, N0, N1, N2, N3, P, M>::range_check_(ss_typename_type_k static_array_4d<T, N0, N1, N2, N3, P, M>::index_type i0, ss_typename_type_k static_array_4d<T, N0, N1, N2, N3, P, M>::index_type i1, ss_typename_type_k static_array_4d<T, N0, N1, N2, N3, P, M>::index_type i2, ss_typename_type_k static_array_4d<T, N0, N1, N2, N3, P, M>::index_type i3) const stlsoft_throw_1(stlsoft_ns_qual_std(out_of_range) )
{
    STLSOFT_SUPPRESS_UNUSED(i0); STLSOFT_SUPPRESS_UNUSED(i1); STLSOFT_SUPPRESS_UNUSED(i2); STLSOFT_SUPPRESS_UNUSED(i3);

#ifdef __STLSOFT_CF_EXCEPTION_SUPPORT
    if( !(i0 < N0) ||
        !(i1 < N1) ||
        !(i2 < N2) ||
        !(i3 < N3))
    {
        stlsoft_ns_qual_std(out_of_range)   x("static array index out of range");

#ifdef __STLSOFT_COMPILER_IS_MSVC
        if(1) // This is needed to avoid Visual C++ warning 4702: 'unreachable code'
#endif /* __STLSOFT_COMPILER_IS_MSVC */
        {
            throw x;
        }
    }
#else
    stlsoft_message_assert("static array index out of range", i0 < N0 && i1 < N1 && i2 < N2 && i3 < N3);
#endif /* __STLSOFT_CF_EXCEPTION_SUPPORT */
}

template <ss_typename_param_k T, ss_size_t N0, ss_size_t N1, ss_size_t N2, ss_size_t N3, ss_typename_param_k P, ss_typename_param_k M>
inline void static_array_4d<T, N0, N1, N2, N3, P, M>::range_check_(ss_typename_type_k static_array_4d<T, N0, N1, N2, N3, P, M>::index_type i0) const stlsoft_throw_1(stlsoft_ns_qual_std(out_of_range) )
{
    STLSOFT_SUPPRESS_UNUSED(i0); STLSOFT_SUPPRESS_UNUSED(N0);

#ifdef __STLSOFT_CF_EXCEPTION_SUPPORT
    if(!(i0 < N0))
    {
        stlsoft_ns_qual_std(out_of_range)   x("static array index out of range");

#ifdef __STLSOFT_COMPILER_IS_MSVC
        if(1) // This is needed to avoid Visual C++ warning 4702: 'unreachable code'
#endif /* __STLSOFT_COMPILER_IS_MSVC */
        {
            throw x;
        }
    }
#else
    stlsoft_message_assert("static array index out of range", i0 < N0);
#endif /* __STLSOFT_CF_EXCEPTION_SUPPORT */
}

template <ss_typename_param_k T, ss_size_t N0, ss_size_t N1, ss_size_t N2, ss_size_t N3, ss_typename_param_k P, ss_typename_param_k M>
inline static_array_4d<T, N0, N1, N2, N3, P, M>::static_array_4d()
{
    array_range_initialiser<T, allocator_type, P>::construct(*this, data_(), size());
}

template <ss_typename_param_k T, ss_size_t N0, ss_size_t N1, ss_size_t N2, ss_size_t N3, ss_typename_param_k P, ss_typename_param_k M>
inline static_array_4d<T, N0, N1, N2, N3, P, M>::static_array_4d(value_type const &t)
{
    array_range_initialiser<T, allocator_type, P>::construct(*this, data_(), size(), t);
}

template <ss_typename_param_k T, ss_size_t N0, ss_size_t N1, ss_size_t N2, ss_size_t N3, ss_typename_param_k P, ss_typename_param_k M>
inline static_array_4d<T, N0, N1, N2, N3, P, M>::~static_array_4d()
{
    if(!is_pointer_type<M>::value)
    {
        array_range_initialiser<T, allocator_type, P>::destroy(*this, data_(), size());
    }
}

template <ss_typename_param_k T, ss_size_t N0, ss_size_t N1, ss_size_t N2, ss_size_t N3, ss_typename_param_k P, ss_typename_param_k M>
inline ss_typename_type_k static_array_4d<T, N0, N1, N2, N3, P, M>::value_type &static_array_4d<T, N0, N1, N2, N3, P, M>::at(ss_typename_type_k static_array_4d<T, N0, N1, N2, N3, P, M>::index_type i0, ss_typename_type_k static_array_4d<T, N0, N1, N2, N3, P, M>::index_type i1, ss_typename_type_k static_array_4d<T, N0, N1, N2, N3, P, M>::index_type i2, ss_typename_type_k static_array_4d<T, N0, N1, N2, N3, P, M>::index_type i3)
{
    range_check_(i0, i1, i2, i3);

    return *(m_data + calc_index_(i0, i1, i2, i3));
}

template <ss_typename_param_k T, ss_size_t N0, ss_size_t N1, ss_size_t N2, ss_size_t N3, ss_typename_param_k P, ss_typename_param_k M>
inline ss_typename_type_k static_array_4d<T, N0, N1, N2, N3, P, M>::value_type const &static_array_4d<T, N0, N1, N2, N3, P, M>::at(ss_typename_type_k static_array_4d<T, N0, N1, N2, N3, P, M>::index_type i0, ss_typename_type_k static_array_4d<T, N0, N1, N2, N3, P, M>::index_type i1, ss_typename_type_k static_array_4d<T, N0, N1, N2, N3, P, M>::index_type i2, ss_typename_type_k static_array_4d<T, N0, N1, N2, N3, P, M>::index_type i3) const
{
    range_check_(i0, i1, i2, i3);

    return *(m_data + calc_index_(i0, i1, i2, i3));
}

template <ss_typename_param_k T, ss_size_t N0, ss_size_t N1, ss_size_t N2, ss_size_t N3, ss_typename_param_k P, ss_typename_param_k M>
inline ss_typename_type_k static_array_4d<T, N0, N1, N2, N3, P, M>::value_type &static_array_4d<T, N0, N1, N2, N3, P, M>::at_unchecked(ss_typename_type_k static_array_4d<T, N0, N1, N2, N3, P, M>::index_type i0, ss_typename_type_k static_array_4d<T, N0, N1, N2, N3, P, M>::index_type i1, ss_typename_type_k static_array_4d<T, N0, N1, N2, N3, P, M>::index_type i2, ss_typename_type_k static_array_4d<T, N0, N1, N2, N3, P, M>::index_type i3)
{
    stlsoft_message_assert("static array index out of range", i0 < N0 && i1 < N1 && i2 < N2 && i3 < N3);

    return *(m_data + calc_index_(i0, i1, i2, i3));
}

template <ss_typename_param_k T, ss_size_t N0, ss_size_t N1, ss_size_t N2, ss_size_t N3, ss_typename_param_k P, ss_typename_param_k M>
inline ss_typename_type_k static_array_4d<T, N0, N1, N2, N3, P, M>::value_type const &static_array_4d<T, N0, N1, N2, N3, P, M>::at_unchecked(ss_typename_type_k static_array_4d<T, N0, N1, N2, N3, P, M>::index_type i0, ss_typename_type_k static_array_4d<T, N0, N1, N2, N3, P, M>::index_type i1, ss_typename_type_k static_array_4d<T, N0, N1, N2, N3, P, M>::index_type i2, ss_typename_type_k static_array_4d<T, N0, N1, N2, N3, P, M>::index_type i3) const
{
    stlsoft_message_assert("static array index out of range", i0 < N0 && i1 < N1 && i2 < N2 && i3 < N3);

    return *(m_data + calc_index_(i0, i1, i2, i3));
}

template <ss_typename_param_k T, ss_size_t N0, ss_size_t N1, ss_size_t N2, ss_size_t N3, ss_typename_param_k P, ss_typename_param_k M>
inline ss_typename_type_k static_array_4d<T, N0, N1, N2, N3, P, M>::dimension_type static_array_4d<T, N0, N1, N2, N3, P, M>::at(ss_typename_type_k static_array_4d<T, N0, N1, N2, N3, P, M>::index_type i0)
{
    range_check_(i0);

    return dimension_type(m_data + i0 * N1 * N2 * N3);
}

template <ss_typename_param_k T, ss_size_t N0, ss_size_t N1, ss_size_t N2, ss_size_t N3, ss_typename_param_k P, ss_typename_param_k M>
inline const ss_typename_type_k static_array_4d<T, N0, N1, N2, N3, P, M>::dimension_type static_array_4d<T, N0, N1, N2, N3, P, M>::at(ss_typename_type_k static_array_4d<T, N0, N1, N2, N3, P, M>::index_type i0) const
{
    range_check_(i0);

    return dimension_type(m_data + i0 * N1 * N2 * N3);
}

template <ss_typename_param_k T, ss_size_t N0, ss_size_t N1, ss_size_t N2, ss_size_t N3, ss_typename_param_k P, ss_typename_param_k M>
inline ss_typename_type_k static_array_4d<T, N0, N1, N2, N3, P, M>::dimension_type static_array_4d<T, N0, N1, N2, N3, P, M>::at_unchecked(ss_typename_type_k static_array_4d<T, N0, N1, N2, N3, P, M>::index_type i0)
{
    stlsoft_message_assert("static array index out of range", i0 < N0);

    return dimension_type(m_data + i0 * N1 * N2 * N3);
}

template <ss_typename_param_k T, ss_size_t N0, ss_size_t N1, ss_size_t N2, ss_size_t N3, ss_typename_param_k P, ss_typename_param_k M>
inline const ss_typename_type_k static_array_4d<T, N0, N1, N2, N3, P, M>::dimension_type static_array_4d<T, N0, N1, N2, N3, P, M>::at_unchecked(ss_typename_type_k static_array_4d<T, N0, N1, N2, N3, P, M>::index_type i0) const
{
    stlsoft_message_assert("static array index out of range", i0 < N0);

    return dimension_type(m_data + i0 * N1 * N2 * N3);
}

template <ss_typename_param_k T, ss_size_t N0, ss_size_t N1, ss_size_t N2, ss_size_t N3, ss_typename_param_k P, ss_typename_param_k M>
inline ss_typename_type_k static_array_4d<T, N0, N1, N2, N3, P, M>::dimension_type static_array_4d<T, N0, N1, N2, N3, P, M>::operator [](ss_typename_type_k static_array_4d<T, N0, N1, N2, N3, P, M>::index_type i0)
{
    stlsoft_message_assert("static array index out of range", i0 < N0);

    return dimension_type(m_data + i0 * N1 * N2 * N3);
}

template <ss_typename_param_k T, ss_size_t N0, ss_size_t N1, ss_size_t N2, ss_size_t N3, ss_typename_param_k P, ss_typename_param_k M>
inline const ss_typename_type_k static_array_4d<T, N0, N1, N2, N3, P, M>::dimension_type static_array_4d<T, N0, N1, N2, N3, P, M>::operator [](ss_typename_type_k static_array_4d<T, N0, N1, N2, N3, P, M>::index_type i0) const
{
    stlsoft_message_assert("static array index out of range", i0 < N0);

    return dimension_type(m_data + i0 * N1 * N2 * N3);
}

template <ss_typename_param_k T, ss_size_t N0, ss_size_t N1, ss_size_t N2, ss_size_t N3, ss_typename_param_k P, ss_typename_param_k M>
inline ss_typename_type_k static_array_4d<T, N0, N1, N2, N3, P, M>::reference static_array_4d<T, N0, N1, N2, N3, P, M>::front()
{
    return at(0, 0, 0, 0);
}

template <ss_typename_param_k T, ss_size_t N0, ss_size_t N1, ss_size_t N2, ss_size_t N3, ss_typename_param_k P, ss_typename_param_k M>
inline ss_typename_type_k static_array_4d<T, N0, N1, N2, N3, P, M>::reference static_array_4d<T, N0, N1, N2, N3, P, M>::back()
{
    return at(N0 - 1, N1 - 1, N2 - 1, N3 - 1);
}

template <ss_typename_param_k T, ss_size_t N0, ss_size_t N1, ss_size_t N2, ss_size_t N3, ss_typename_param_k P, ss_typename_param_k M>
inline ss_typename_type_k static_array_4d<T, N0, N1, N2, N3, P, M>::const_reference static_array_4d<T, N0, N1, N2, N3, P, M>::front() const
{
    return at(0, 0, 0, 0);
}

template <ss_typename_param_k T, ss_size_t N0, ss_size_t N1, ss_size_t N2, ss_size_t N3, ss_typename_param_k P, ss_typename_param_k M>
inline ss_typename_type_k static_array_4d<T, N0, N1, N2, N3, P, M>::const_reference static_array_4d<T, N0, N1, N2, N3, P, M>::back() const
{
    return at(N0 - 1, N1 - 1, N2 - 1, N3 - 1);
}

template <ss_typename_param_k T, ss_size_t N0, ss_size_t N1, ss_size_t N2, ss_size_t N3, ss_typename_param_k P, ss_typename_param_k M>
inline /* static */ ss_typename_type_k static_array_4d<T, N0, N1, N2, N3, P, M>::index_type static_array_4d<T, N0, N1, N2, N3, P, M>::dimension0()
{
    return N0;
}

template <ss_typename_param_k T, ss_size_t N0, ss_size_t N1, ss_size_t N2, ss_size_t N3, ss_typename_param_k P, ss_typename_param_k M>
inline /* static */ ss_typename_type_k static_array_4d<T, N0, N1, N2, N3, P, M>::index_type static_array_4d<T, N0, N1, N2, N3, P, M>::dimension1()
{
    return N1;
}

template <ss_typename_param_k T, ss_size_t N0, ss_size_t N1, ss_size_t N2, ss_size_t N3, ss_typename_param_k P, ss_typename_param_k M>
inline /* static */ ss_typename_type_k static_array_4d<T, N0, N1, N2, N3, P, M>::index_type static_array_4d<T, N0, N1, N2, N3, P, M>::dimension2()
{
    return N2;
}

template <ss_typename_param_k T, ss_size_t N0, ss_size_t N1, ss_size_t N2, ss_size_t N3, ss_typename_param_k P, ss_typename_param_k M>
inline /* static */ ss_typename_type_k static_array_4d<T, N0, N1, N2, N3, P, M>::index_type static_array_4d<T, N0, N1, N2, N3, P, M>::dimension3()
{
    return N3;
}

template <ss_typename_param_k T, ss_size_t N0, ss_size_t N1, ss_size_t N2, ss_size_t N3, ss_typename_param_k P, ss_typename_param_k M>
inline /* static */ ss_typename_type_k static_array_4d<T, N0, N1, N2, N3, P, M>::index_type static_array_4d<T, N0, N1, N2, N3, P, M>::size()
{
    return N0 * N1 * N2 * N3;
}

template <ss_typename_param_k T, ss_size_t N0, ss_size_t N1, ss_size_t N2, ss_size_t N3, ss_typename_param_k P, ss_typename_param_k M>
inline /* static */ ss_bool_t static_array_4d<T, N0, N1, N2, N3, P, M>::empty()
{
    return false;
}

template <ss_typename_param_k T, ss_size_t N0, ss_size_t N1, ss_size_t N2, ss_size_t N3, ss_typename_param_k P, ss_typename_param_k M>
inline /* static */ ss_typename_type_k static_array_4d<T, N0, N1, N2, N3, P, M>::index_type static_array_4d<T, N0, N1, N2, N3, P, M>::max_size()
{
    return size();
}

template <ss_typename_param_k T, ss_size_t N0, ss_size_t N1, ss_size_t N2, ss_size_t N3, ss_typename_param_k P, ss_typename_param_k M>
inline ss_typename_type_k static_array_4d<T, N0, N1, N2, N3, P, M>::iterator static_array_4d<T, N0, N1, N2, N3, P, M>::begin()
{
    return m_data;
}

template <ss_typename_param_k T, ss_size_t N0, ss_size_t N1, ss_size_t N2, ss_size_t N3, ss_typename_param_k P, ss_typename_param_k M>
inline ss_typename_type_k static_array_4d<T, N0, N1, N2, N3, P, M>::iterator static_array_4d<T, N0, N1, N2, N3, P, M>::end()
{
    return m_data + size();
}

template <ss_typename_param_k T, ss_size_t N0, ss_size_t N1, ss_size_t N2, ss_size_t N3, ss_typename_param_k P, ss_typename_param_k M>
inline ss_typename_type_k static_array_4d<T, N0, N1, N2, N3, P, M>::const_iterator static_array_4d<T, N0, N1, N2, N3, P, M>::begin() const
{
    return m_data;
}

template <ss_typename_param_k T, ss_size_t N0, ss_size_t N1, ss_size_t N2, ss_size_t N3, ss_typename_param_k P, ss_typename_param_k M>
inline ss_typename_type_k static_array_4d<T, N0, N1, N2, N3, P, M>::const_iterator static_array_4d<T, N0, N1, N2, N3, P, M>::end() const
{
    return m_data + size();
}

#ifdef __STLSOFT_CF_BIDIRECTIONAL_ITERATOR_SUPPORT
template <ss_typename_param_k T, ss_size_t N0, ss_size_t N1, ss_size_t N2, ss_size_t N3, ss_typename_param_k P, ss_typename_param_k M>
inline ss_typename_type_k static_array_4d<T, N0, N1, N2, N3, P, M>::reverse_iterator static_array_4d<T, N0, N1, N2, N3, P, M>::rbegin()
{
    return reverse_iterator(end());
}

template <ss_typename_param_k T, ss_size_t N0, ss_size_t N1, ss_size_t N2, ss_size_t N3, ss_typename_param_k P, ss_typename_param_k M>
inline ss_typename_type_k static_array_4d<T, N0, N1, N2, N3, P, M>::reverse_iterator static_array_4d<T, N0, N1, N2, N3, P, M>::rend()
{
    return reverse_iterator(begin());
}

template <ss_typename_param_k T, ss_size_t N0, ss_size_t N1, ss_size_t N2, ss_size_t N3, ss_typename_param_k P, ss_typename_param_k M>
inline ss_typename_type_k static_array_4d<T, N0, N1, N2, N3, P, M>::const_reverse_iterator static_array_4d<T, N0, N1, N2, N3, P, M>::rbegin() const
{
    return const_reverse_iterator(end());
}

template <ss_typename_param_k T, ss_size_t N0, ss_size_t N1, ss_size_t N2, ss_size_t N3, ss_typename_param_k P, ss_typename_param_k M>
inline ss_typename_type_k static_array_4d<T, N0, N1, N2, N3, P, M>::const_reverse_iterator static_array_4d<T, N0, N1, N2, N3, P, M>::rend() const
{
    return const_reverse_iterator(begin());
}
#endif /* __STLSOFT_CF_BIDIRECTIONAL_ITERATOR_SUPPORT */

template <ss_typename_param_k T, ss_size_t N0, ss_size_t N1, ss_size_t N2, ss_size_t N3, ss_typename_param_k P, ss_typename_param_k M>
inline ss_typename_type_k static_array_4d<T, N0, N1, N2, N3, P, M>::value_type const *static_array_4d<T, N0, N1, N2, N3, P, M>::data() const
{
    return m_data;
}

#endif /* !__STLSOFT_DOCUMENTATION_SKIP_SECTION */

/* /////////////////////////////////////////////////////////////////////////////
 * Shims
 */

template<   ss_typename_param_k T
        ,   ss_size_t N0
        ,   ss_typename_param_k P
        ,   ss_typename_param_k M
        >
inline ss_size_t array_size(static_array_1d<T, N0, P, M> const &ar)
{
    return ar.size();
}

template<   ss_typename_param_k T
        ,   ss_size_t N0
        ,   ss_size_t N1
        ,   ss_typename_param_k P
        ,   ss_typename_param_k M
        >
inline ss_size_t array_size(static_array_2d<T, N0, N1, P, M> const &ar)
{
    return ar.size();
}

template<   ss_typename_param_k T
        ,   ss_size_t N0
        ,   ss_size_t N1
        ,   ss_size_t N2
        ,   ss_typename_param_k P
        ,   ss_typename_param_k M
        >
inline ss_size_t array_size(static_array_3d<T, N0, N1, N2, P, M> const &ar)
{
    return ar.size();
}

template<   ss_typename_param_k T
        ,   ss_size_t N0
        ,   ss_size_t N1
        ,   ss_size_t N2
        ,   ss_size_t N3
        ,   ss_typename_param_k P
        ,   ss_typename_param_k M
        >
inline ss_size_t array_size(static_array_4d<T, N0, N1, N2, N3, P, M> const &ar)
{
    return ar.size();
}

#if 0
template<   ss_typename_param_k T
        ,   ss_size_t N0
        ,   ss_size_t N1
        ,   ss_size_t N2
        ,   ss_size_t N3
        ,   ss_size_t N4
        ,   ss_typename_param_k P
        ,   ss_typename_param_k M
        >
inline ss_size_t array_size(static_array_5d<T, N0, N1, N2, N3, N4, P, M> const &ar)
{
    return ar.size();
}
#endif /* 0 */

////////////////////////////////////////////////////////////////////////////////
// Unit-testing

#ifdef STLSOFT_UNITTEST

namespace unittest
{
    ss_bool_t test_stlsoft_static_array(unittest_reporter *r)
    {
        ss_bool_t               bSuccess    =   true;

        unittest_initialiser    init(r, "STLSoft", "static_array", __FILE__);

#if 0
        if(<<TODO>>)
        {
            r->report("<<TODO>> failed ", __LINE__);
            bSuccess = false;
        }
#endif /* 0 */

        return bSuccess;
    }

    unittest_registrar    unittest_stlsoft_static_array(test_stlsoft_static_array);

} // namespace unittest

#endif /* STLSOFT_UNITTEST */

/* ////////////////////////////////////////////////////////////////////////// */

#ifndef _STLSOFT_NO_NAMESPACE
} // namespace stlsoft
#endif /* _STLSOFT_NO_NAMESPACE */

/* ////////////////////////////////////////////////////////////////////////// */

#endif /* !STLSOFT_INCL_H_STLSOFT_STATIC_ARRAY */

/* ////////////////////////////////////////////////////////////////////////// */
