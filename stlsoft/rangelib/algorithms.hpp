/* /////////////////////////////////////////////////////////////////////////////
 * File:        rangelib/algorithms.hpp
 *
 * Purpose:     Range algorithms.
 *
 * Created:     4th November 2003
 * Updated:     12th September 2004
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


/** \file rangelib/algorithms.hpp Range algorithms */

#ifndef STLSOFT_INCL_RANGELIB_HPP_ALGORITHMS
#define STLSOFT_INCL_RANGELIB_HPP_ALGORITHMS

#ifndef __STLSOFT_DOCUMENTATION_SKIP_SECTION
# define STLSOFT_VER_RANGELIB_HPP_ALGORITHMS_MAJOR    1
# define STLSOFT_VER_RANGELIB_HPP_ALGORITHMS_MINOR    7
# define STLSOFT_VER_RANGELIB_HPP_ALGORITHMS_REVISION 3
# define STLSOFT_VER_RANGELIB_HPP_ALGORITHMS_EDIT     14
#endif /* !__STLSOFT_DOCUMENTATION_SKIP_SECTION */

/* /////////////////////////////////////////////////////////////////////////////
 * Includes
 */

#ifndef STLSOFT_INCL_H_STLSOFT
# include <stlsoft.h>                   // Include the STLSoft root header
#endif /* !STLSOFT_INCL_H_STLSOFT */
#ifndef STLSOFT_INCL_RANGELIB_HPP_RANGE_CATEGORIES
# include <rangelib/range_categories.hpp>
#endif /* !STLSOFT_INCL_RANGELIB_HPP_RANGE_CATEGORIES */
#ifndef STLSOFT_INCL_RANGELIB_HPP_BASIC_INDIRECT_RANGE_ADAPTOR
# include <rangelib/basic_indirect_range_adaptor.hpp>
#endif /* !STLSOFT_INCL_RANGELIB_HPP_BASIC_INDIRECT_RANGE_ADAPTOR */
#include <algorithm>
#include <numeric>

/* /////////////////////////////////////////////////////////////////////////////
 * Namespace
 */

#ifndef _STLSOFT_NO_NAMESPACE
namespace stlsoft
{
#endif /* _STLSOFT_NO_NAMESPACE */

/* ////////////////////////////////////////////////////////////////////////// */

/// \defgroup rangelib_algorithms Range Algorithms
/// \ingroup RangeLib
/// \brief Range algorithms
/// @{

/* /////////////////////////////////////////////////////////////////////////////
 * Functions
 */

/* *********************************************************
 * accumulate (2)
 */

template<   ss_typename_param_k R
        ,   ss_typename_param_k T
        >
inline T r_accumulate_2_impl(R r, T val, notional_range_tag const &)
{
    for(; r; ++r)
    {
        val = val + *r;
    }

    return val;
}

template<   ss_typename_param_k R
        ,   ss_typename_param_k T
        >
inline T r_accumulate_2_impl(R r, T val, iterable_range_tag const &)
{
    return std::accumulate(r.begin(), r.end(), val);
}

template<   ss_typename_param_k R
        ,   ss_typename_param_k T
        >
inline T r_accumulate_2_impl(R r, T val, basic_indirect_range_tag const &)
{
    return indirect_range_adaptor<R>(r).accumulate(val);
}

template<   ss_typename_param_k R
        ,   ss_typename_param_k T
        >
inline T r_accumulate_2_impl(R r, T val, indirect_range_tag const &)
{
    return r.accumulate(val);
}

/// accumulate() for ranges
///
/// \param r The range
/// \param val The initial value
/// \retval The sum of the accumulate items and the initial value
///
/// \note: Supports Notional, Iterable and Indirect Range types
template<   ss_typename_param_k R
        ,   ss_typename_param_k T
        >
inline T r_accumulate(R r, T val)
{
    return r_accumulate_2_impl(r, val, r);
}

/* *********************************************************
 * accumulate (3)
 */

template<   ss_typename_param_k R
        ,   ss_typename_param_k T
        ,   ss_typename_param_k P
        >
inline T r_accumulate_3_impl(R r, T val, P pr, notional_range_tag const &)
{
    for(; r; ++r)
    {
        val = pr(val, *r);
    }

    return val;
}

template<   ss_typename_param_k R
        ,   ss_typename_param_k T
        ,   ss_typename_param_k P
        >
inline T r_accumulate_3_impl(R r, T val, P pr, iterable_range_tag const &)
{
    return std::accumulate(r.begin(), r.end(), val, pr);
}

template<   ss_typename_param_k R
        ,   ss_typename_param_k T
        ,   ss_typename_param_k P
        >
inline T r_accumulate_2_impl(R r, T val, P pr, basic_indirect_range_tag const &)
{
    return indirect_range_adaptor<R>(r).accumulate(val, pr);
}

template<   ss_typename_param_k R
        ,   ss_typename_param_k T
        ,   ss_typename_param_k P
        >
inline T r_accumulate_3_impl(R r, T val, P pr, indirect_range_tag const &)
{
    return r.accumulate(val, pr);
}

/// accumulate() for ranges
///
/// \param r The range
/// \param val The initial value
/// \param pr The predicate applied to each entry
/// \retval The sum of the accumulate items and the initial value
///
/// \note: Supports Notional, Iterable and Indirect Range types
template<   ss_typename_param_k R
        ,   ss_typename_param_k T
        ,   ss_typename_param_k P
        >
inline T r_accumulate(R r, T val, P pr)
{
    return r_accumulate_3_impl(r, val, pr, r);
}


/* *********************************************************
 * copy
 */

template<   ss_typename_param_k R
        ,   ss_typename_param_k O
        >
inline O r_copy_impl(R r, O o, notional_range_tag const &)
{
    for(; r; ++r, ++o)
    {
        *o = *r;
    }

    return o;
}

template<   ss_typename_param_k R
        ,   ss_typename_param_k O
        >
inline O r_copy_impl(R r, O o, iterable_range_tag const &)
{
    return std::copy(r.begin(), r.end(), o);
}

template<   ss_typename_param_k R
        ,   ss_typename_param_k O
        >
inline O r_copy_impl(R r, O o, indirect_range_tag const &)
{
    return r.copy(o);
}

template<   ss_typename_param_k R
        ,   ss_typename_param_k O
        >
inline O r_copy_impl(R r, O o, basic_indirect_range_tag const &)
{
    return indirect_range_adaptor<R>(r).copy(o);
}

/// Copies the contents of the range to the output iterator
///
/// \param r The range whose elements are to be copied
/// \param o The output iterator to receive the elements
///
/// \note: Supports Notional, Iterable and Indirect Range types
template<   ss_typename_param_k R
        ,   ss_typename_param_k O
        >
inline O r_copy(R r, O o)
{
    return r_copy_impl(r, o, r);
}


/* *********************************************************
 * count
 */

template<   ss_typename_param_k R
        ,   ss_typename_param_k T
        >
inline ss_size_t r_count_impl(R r, const T &val, notional_range_tag const &)
{
    ss_size_t n;

    for(n = 0; r; ++r)
    {
        if(val == *r)
        {
            ++n;
        }
    }

    return n;
}

template<   ss_typename_param_k R
        ,   ss_typename_param_k T
        >
inline ss_size_t r_count_impl(R r, const T &val, iterable_range_tag const &)
{
    return std::count(r.begin(), r.end(), val);
}

template<   ss_typename_param_k R
        ,   ss_typename_param_k T
        >
inline ss_size_t r_count_impl(R r, const T &val, basic_indirect_range_tag const &)
{
    return indirect_range_adaptor<R>(r).count(val);
}

template<   ss_typename_param_k R
        ,   ss_typename_param_k T
        >
inline ss_size_t r_count_impl(R r, const T &val, indirect_range_tag const &)
{
    return r.count(val);
}

/// Counts the number of instances of a given value in the range
///
/// \param r The range
/// \param val The value to search for
/// \retval The number of elements in the range matching \c val
///
/// \note: Supports Notional, Iterable and Indirect Range types
template<   ss_typename_param_k R
        ,   ss_typename_param_k T
        >
inline ss_size_t r_count(R r, const T &val)
{
    return r_count_impl(r, val, r);
}


/* *********************************************************
 * count_if
 */

template<   ss_typename_param_k R
        ,   ss_typename_param_k P
        >
inline ss_size_t r_count_if_impl(R r, P pr, notional_range_tag const &)
{
    ss_size_t n;

    for(n = 0; r; ++r)
    {
        if(pr(*r))
        {
            ++n;
        }
    }

    return n;
}

template<   ss_typename_param_k R
        ,   ss_typename_param_k P
        >
inline ss_size_t r_count_if_impl(R r, P pr, iterable_range_tag const &)
{
    return std::count_if(r.begin(), r.end(), pr);
}

template<   ss_typename_param_k R
        ,   ss_typename_param_k P
        >
inline ss_size_t r_count_if_impl(R r, P pr, basic_indirect_range_tag const &)
{
    return indirect_range_adaptor<R>(r).count_if(pr);
}

template<   ss_typename_param_k R
        ,   ss_typename_param_k P
        >
inline ss_size_t r_count_if_impl(R r, P pr, indirect_range_tag const &)
{
    return r.count_if(pr);
}

/// Counts the number of instances matching the given predicate in the range
///
/// \param r The range
/// \param pr The predicate applied to each entry
/// \retval The number of elements in the range matching \c val
///
/// \note: Supports Notional, Iterable and Indirect Range types
template<   ss_typename_param_k R
        ,   ss_typename_param_k P
        >
inline ss_size_t r_count_if(R r, P pr)
{
    return r_count_if_impl(r, pr, r);
}


/* *********************************************************
 * distance
 */

template <ss_typename_param_k R>
inline ss_ptrdiff_t r_distance_1_impl(R r, notional_range_tag const &)
{
    ss_ptrdiff_t    d = 0;

    for(; r; ++r, ++d)
    {}

    return d;
}

template <ss_typename_param_k R>
inline ss_ptrdiff_t r_distance_1_impl(R r, iterable_range_tag const &)
{
    return std::distance(r.begin(), r.end());
}

template <ss_typename_param_k R>
inline ss_ptrdiff_t r_distance_1_impl(R r, indirect_range_tag const &)
{
    return r.distance();
}

template <ss_typename_param_k R>
inline ss_ptrdiff_t r_distance_1_impl(R r, basic_indirect_range_tag const &)
{
    return indirect_range_adaptor<R>(r).distance();
}

/// Counts the number of instances in the range
///
/// \param r The range
/// \retval The number of elements in the range
///
/// \note: Supports Notional, Iterable and Indirect Range types
template <ss_typename_param_k R>
inline ss_ptrdiff_t r_distance(R r)
{
    return r_distance_1_impl(r, r);
}

/* *********************************************************
 * equal (2)
 */

template<   ss_typename_param_k R1
        ,   ss_typename_param_k R2
        >
inline ss_bool_t r_equal_1_impl(R1 r1, R2 r2, notional_range_tag const &, notional_range_tag const &)
{
    for(; r1 && r2; ++r1, ++r2)
    {
        if(*r1 != *r2)
        {
            return false;
        }
    }

    return true;
}

template<   ss_typename_param_k R1
        ,   ss_typename_param_k R2
        >
inline ss_bool_t r_equal_1_impl(R1 r1, R2 r2, iterable_range_tag const &, iterable_range_tag const &)
{
    return std::equal(r1.begin(), r1.end(), r2.begin());
}

/// Determines whether two ranges are equal
///
/// \param r1 The first range to compare
/// \param r2 The second range to compare
/// \retval true if the first N elements in the second range match the N elements in the first range.
///
/// \note: Supports Notional and Iterable Range types
template<   ss_typename_param_k R1
        ,   ss_typename_param_k R2
        >
inline ss_bool_t r_equal(R1 r1, R2 r2)
{
    stlsoft_assert(r_distance(r1) <= r_distance(r2));

    return r_equal_1_impl(r1, r2, r1, r2);
}

/* *********************************************************
 * equal (3)
 */

template<   ss_typename_param_k R1
        ,   ss_typename_param_k R2
        ,   ss_typename_param_k P
        >
inline ss_bool_t r_equal_1_impl(R1 r1, R2 r2, P pr, notional_range_tag const &, notional_range_tag const &)
{
    for(; r1 && r2; ++r1, ++r2)
    {
        if(!pr(*r1, *r2))
        {
            return false;
        }
    }

    return true;
}

template<   ss_typename_param_k R1
        ,   ss_typename_param_k R2
        ,   ss_typename_param_k P
        >
inline ss_bool_t r_equal_1_impl(R1 r1, R2 r2, P pr, iterable_range_tag const &, iterable_range_tag const &)
{
    return std::equal(r1.begin(), r1.end(), r2.begin(), pr);
}

/// Determines whether two ranges are equal, as defined by a predicate
///
/// \param r1 The first range to compare
/// \param r2 The second range to compare
/// \param pr The predicate which evaluates matches between elements of the two ranges
/// \retval true if the first N elements in the second range match the N elements in the first range.
///
/// \note: Supports Notional and Iterable Range types
template<   ss_typename_param_k R1
        ,   ss_typename_param_k R2
        ,   ss_typename_param_k P
        >
inline ss_bool_t r_equal(R1 r1, R2 r2, P pr)
{
    stlsoft_assert(r_distance(r1) <= r_distance(r2));

    return r_equal_1_impl(r1, r2, pr, r1, r2);
}

/* *********************************************************
 * exists
 */

template<   ss_typename_param_k R
        ,   ss_typename_param_k T
        >
inline ss_bool_t r_exists_impl(R r, T const &val, notional_range_tag const &)
{
    for(; r; ++r)
    {
        if(val == *r)
        {
            return true;
        }
    }

    return false;
}

template<   ss_typename_param_k R
        ,   ss_typename_param_k T
        >
inline ss_bool_t r_exists_impl(R r, T const &val, iterable_range_tag const &)
{
    return std::find(r.begin(), r.end(), val) != r.end();
}

template<   ss_typename_param_k R
        ,   ss_typename_param_k T
        >
inline ss_bool_t r_exists_impl(R r, T const &val, basic_indirect_range_tag const &)
{
    return indirect_range_adaptor<R>(r).exists(val);
}

template<   ss_typename_param_k R
        ,   ss_typename_param_k T
        >
inline ss_bool_t r_exists_impl(R r, T const &val, indirect_range_tag const &)
{
    return r.exists(val);
}

/// Determines whether the given value exists in the range
///
/// \param r The range
/// \param val The value to search for
///
/// \note: Supports Notional, Iterable and Indirect Range types
template<   ss_typename_param_k R
        ,   ss_typename_param_k T
        >
inline R r_exists(R r, T const &val)
{
    return r_exists_impl(r, val, r);
}

/* *********************************************************
 * exists_if (1)
 */

template<   ss_typename_param_k R
        ,   ss_typename_param_k P
        >
inline ss_bool_t r_exists_if_1_impl(R r, P pr, notional_range_tag const &)
{
    for(; r; ++r)
    {
        if(pr(*r))
        {
            return true;
        }
    }

    return false;
}

template<   ss_typename_param_k R
        ,   ss_typename_param_k P
        >
inline ss_bool_t r_exists_if_1_impl(R r, P pr, iterable_range_tag const &)
{
    return std::find(r.begin(), r.end(), pr) != r.end();
}

template<   ss_typename_param_k R
        ,   ss_typename_param_k P
        >
inline ss_bool_t r_exists_if_1_impl(R r, P pr, basic_indirect_range_tag const &)
{
    return indirect_range_adaptor<R>(r).exists_if(pr);
}

template<   ss_typename_param_k R
        ,   ss_typename_param_k P
        >
inline ss_bool_t r_exists_if_1_impl(R r, P pr, indirect_range_tag const &)
{
    return r.exists_if(pr);
}

/// Determines whether a value matching the given predicate exists in the range
///
/// \param r The range
/// \param pr The predicate used to match the items
///
/// \note: Supports Notional, Iterable and Indirect Range types
template<   ss_typename_param_k R
        ,   ss_typename_param_k P
        >
inline R r_exists_if(R r, P pr)
{
    return r_exists_if_1_impl(r, pr, r);
}

/* *********************************************************
 * exists_if (2)
 */

template<   ss_typename_param_k R
        ,   ss_typename_param_k P
        ,   ss_typename_param_k T
        >
inline ss_bool_t r_exists_if_2_impl(R r, P pr, T &result, notional_range_tag const &)
{
    for(; r; ++r)
    {
        if(pr(*r))
        {
            result = *r;

            return true;
        }
    }

    return false;
}

template<   ss_typename_param_k I
        ,   ss_typename_param_k V
        >
inline ss_bool_t r_exists_if_2_impl_helper_(I from, I to, V &val)
{
    if(from == to)
    {
        return false;
    }
    else
    {
        val = *from;

        return true;
    }
}

template<   ss_typename_param_k R
        ,   ss_typename_param_k P
        ,   ss_typename_param_k T
        >
inline ss_bool_t r_exists_if_2_impl(R r, P pr, T &result, iterable_range_tag const &)
{
    return r_exists_if_2_impl_helper_(std::find(r.begin(), r.end(), pr), r.end());
}

template<   ss_typename_param_k R
        ,   ss_typename_param_k P
        ,   ss_typename_param_k T
        >
inline ss_bool_t r_exists_if_2_impl(R r, P pr, T &result, basic_indirect_range_tag const &)
{
    return indirect_range_adaptor<R>(r).exists_if(pr, result);
}

template<   ss_typename_param_k R
        ,   ss_typename_param_k P
        ,   ss_typename_param_k T
        >
inline ss_bool_t r_exists_if_2_impl(R r, P pr, T &result, indirect_range_tag const &)
{
    return r.exists_if(pr, result);
}

/// Determines whether a value matching the given predicate exists in the range
///
/// \param r The range
/// \param pr The predicate used to match the items
///
/// \note: Supports Notional, Iterable and Indirect Range types
template<   ss_typename_param_k R
        ,   ss_typename_param_k P
        ,   ss_typename_param_k T
        >
inline R r_exists_if(R r, P pr, T &result)
{
    return r_exists_if_2_impl(r, pr, result, r);
}

/* *********************************************************
 * fill
 */

template<   ss_typename_param_k R
        ,   ss_typename_param_k T
        >
inline void r_fill_impl(R r, T const &val, iterable_range_tag const &)
{
    std::fill(r.begin(), r.end(), val);
}

/// Sets the elements in the range to the given value
///
/// \param r The range
/// \param val The value to assign to all elements in the range
///
/// \note: Supports Iterable Range type
template<   ss_typename_param_k R
        ,   ss_typename_param_k T
        >
inline void r_fill(R r, const T &val)
{
    r_fill_impl(r, val, r);
}

/* *********************************************************
 * fill_n
 */

template<   ss_typename_param_k R
        ,   ss_typename_param_k S
        ,   ss_typename_param_k T
        >
inline void r_fill_n_impl(R r, S n, T const &val, iterable_range_tag const &)
{
    std::fill(r.begin(), n, val);
}

/// Sets the first \c n elements in the range to the given value
///
/// \param r The range
/// \param n The number of elements to set. This must be <font class = "code"><= r_distance(r)</font>
/// \param val The value to assign to all elements in the range
///
/// \note: Supports Iterable Range type
template<   ss_typename_param_k R
        ,   ss_typename_param_k S
        ,   ss_typename_param_k T
        >
inline void r_fill_n(R r, S n, T const &val)
{
    stlsoft_assert(n <= r_distance(r));

    r_fill_1_impl(r, n, val, r);
}

/* *********************************************************
 * find
 */

template<   ss_typename_param_k R
        ,   ss_typename_param_k T
        >
inline R r_find_impl(R r, T const &val, notional_range_tag const &)
{
    for(; r; ++r)
    {
        if(val == *r)
        {
            break;
        }
    }

    return r;
}

template<   ss_typename_param_k R
        ,   ss_typename_param_k T
        >
inline R r_find_impl(R r, T const &val, iterable_range_tag const &)
{
    return R(std::find(r.begin(), r.end(), val), r.end());
}

/// Finds the first instance of the given value in the range
///
/// \param r The range
/// \param val The value to find
///
/// \note: Supports Notional and Iterable Range types
template<   ss_typename_param_k R
        ,   ss_typename_param_k T
        >
inline R r_find(R r, T const &val)
{
    return r_find_impl(r, val, r);
}

/* *********************************************************
 * find_if
 */

// find_if

template<   ss_typename_param_k R
        ,   ss_typename_param_k P
        >
inline R r_find_if_impl(R r, P pr, notional_range_tag const &)
{
    for(; r; ++r)
    {
        if(pr(*r))
        {
            break;
        }
    }

    return r;
}

template<   ss_typename_param_k R
        ,   ss_typename_param_k P
        >
inline R r_find_if_impl(R r, P pr, iterable_range_tag const &)
{
    return R(std::find(r.begin(), r.end(), pr), r.end());
}

/// Finds the first instance of a value in the range matching the given predicate
///
/// \param r The range
/// \param pr The value to find
///
/// \note: Supports Notional and Iterable Range types
template<   ss_typename_param_k R
        ,   ss_typename_param_k P
        >
inline R r_find(R r, P pr)
{
    return r_find_if_impl(r, pr, r);
}

/* *********************************************************
 * for_each
 */

template<   ss_typename_param_k R
        ,   ss_typename_param_k F
        >
inline F r_for_each_impl(R r, F f, notional_range_tag const &)
{
    for(; r; ++r)
    {
        f(*r);
    }

    return f;
}

template<   ss_typename_param_k R
        ,   ss_typename_param_k F
        >
inline F r_for_each_impl(R r, F f, iterable_range_tag const &)
{
    return std::for_each(r.begin(), r.end(), f);
}

template<   ss_typename_param_k R
        ,   ss_typename_param_k F
        >
inline F r_for_each_impl(R r, F f, basic_indirect_range_tag const &)
{
    return indirect_range_adaptor<R>(r).for_each(f);
}

template<   ss_typename_param_k R
        ,   ss_typename_param_k F
        >
inline F r_for_each_impl(R r, F f, indirect_range_tag const &)
{
    return r.for_each(f);
}

/// Applies the given function to every element in the range
///
/// \param r The range
/// \param f The function to apply
///
/// \note: Supports Notional, Iterable and Indirect Range types
template<   ss_typename_param_k R
        ,   ss_typename_param_k F
        >
inline F r_for_each(R r, F f)
{
    return r_for_each_impl(r, f, r);
}

/* *********************************************************
 * generate
 */

template<   ss_typename_param_k R
        ,   ss_typename_param_k F
        >
inline void r_generate_impl(R r, F f, iterable_range_tag const &)
{
    std::generate(r.begin(), r.end(), f);
}

/// Sets each element in the range to the result of the given function
///
/// \param r The range
/// \param f The generator function
///
/// \note: Supports Iterable Range type
template<   ss_typename_param_k R
        ,   ss_typename_param_k F
        >
inline void r_generate(R r, F f)
{
    r_generate_impl(r, f, r);
}

/* *********************************************************
 * max_element (1)
 */

template <ss_typename_param_k R>
inline ss_typename_type_k R::value_type r_max_element_1_impl(R r, notional_range_tag const &)
{
    typedef ss_typename_type_k R::value_type    value_type_t;

    value_type_t    max_    =   value_type_t();

    for(; r; ++r)
    {
        if(max_ < *r)
        {
            max_ = *r;
        }
    }

    return max_;
}

template <ss_typename_param_k R>
inline ss_typename_type_k R::value_type r_max_element_1_impl(R r, iterable_range_tag const &)
{
    return *std::max_element(r.begin(), r.end());
}

template <ss_typename_param_k R>
inline ss_typename_type_k R::value_type r_max_element_1_impl(R r, basic_indirect_range_tag const &)
{
    return indirect_range_adaptor<R>(r).max_element();
}

template <ss_typename_param_k R>
inline ss_typename_type_k R::value_type r_max_element_1_impl(R r, indirect_range_tag const &)
{
    return r.max_element();
}

/// Evaluates the maximum element in the range
///
/// \param r The range. Cannot be closed
///
/// \note: Supports Notional, Iterable and Indirect Range types
/// \note: The behaviour is undefined if the range is closed
template <ss_typename_param_k R>
inline ss_typename_type_k R::value_type r_max_element(R r)
{
    stlsoft_assert(r_distance(r) > 0);

    return r_max_element_1_impl(r, r);
}

/* *********************************************************
 * max_element (2)
 */

template<   ss_typename_param_k R
        ,   ss_typename_param_k F
        >
inline ss_typename_type_k R::value_type r_max_element_2_impl(R r, F f, iterable_range_tag const &)
{
    return *std::max_element(r.begin(), r.end(), f);
}

template<   ss_typename_param_k R
        ,   ss_typename_param_k F
        >
inline ss_typename_type_k R::value_type r_max_element_2_impl(R r, F f, notional_range_tag const &)
{
    typedef ss_typename_type_k R::value_type    value_type_t;

    value_type_t    max_    =   value_type_t();

    for(; r; ++r)
    {
        if(f(max_, *r))
        {
            max_ = *r;
        }
    }

    return max_;
}

template<   ss_typename_param_k R
        ,   ss_typename_param_k F
        >
inline ss_typename_type_k R::value_type r_max_element_2_impl(R r, F f, basic_indirect_range_tag const &)
{
    return indirect_range_adaptor<R>(r).max_element(f);
}

template<   ss_typename_param_k R
        ,   ss_typename_param_k F
        >
inline ss_typename_type_k R::value_type r_max_element_2_impl(R r, F f, indirect_range_tag const &)
{
    return r.max_element(f);
}

/// Evaluates the maximum element in the range evaluated according to the given function
///
/// \param r The range. Cannot be closed
/// \param f The function used to evaluate the ordering
///
/// \note: Supports Notional, Iterable and Indirect Range types
/// \note: The behaviour is undefined if the range is closed
template<   ss_typename_param_k R
        ,   ss_typename_param_k F
        >
inline ss_typename_type_k R::value_type r_max_element(R r, F f)
{
    stlsoft_assert(r_distance(r) > 0);

    return r_max_element_2_impl(r, f, r);
}

/* *********************************************************
 * min_element (1)
 */

template <ss_typename_param_k R>
inline ss_typename_type_k R::value_type r_min_element_1_impl(R r, iterable_range_tag const &)
{
    return *std::min_element(r.begin(), r.end());
}

template <ss_typename_param_k R>
inline ss_typename_type_k R::value_type r_min_element_1_impl(R r, notional_range_tag const &)
{
    typedef ss_typename_type_k R::value_type    value_type_t;

    if(!r)
    {
        return value_type_t();
    }
    else
    {
        value_type_t    min_    =   *r;

        for(; ++r; )
        {
            if(*r < min_)
            {
                min_ = *r;
            }
        }

        return min_;
    }
}

template <ss_typename_param_k R>
inline ss_typename_type_k R::value_type r_min_element_1_impl(R r, basic_indirect_range_tag const &)
{
    return indirect_range_adaptor<R>(r).min_element();
}

template <ss_typename_param_k R>
inline ss_typename_type_k R::value_type r_min_element_1_impl(R r, indirect_range_tag const &)
{
    return r.min_element();
}

/// Evaluates the minimum element in the range
///
/// \param r The range. Cannot be closed
///
/// \note: Supports Notional, Iterable and Indirect Range types
/// \note: The behaviour is undefined if the range is closed
template <ss_typename_param_k R>
inline ss_typename_type_k R::value_type r_min_element(R r)
{
    stlsoft_assert(r_distance(r) > 0);

    return r_min_element_1_impl(r, r);
}

/* *********************************************************
 * min_element (2)
 */

template<   ss_typename_param_k R
        ,   ss_typename_param_k F
        >
inline ss_typename_type_k R::value_type r_min_element_2_impl(R r, F f, iterable_range_tag const &)
{
    return *std::min_element(r.begin(), r.end(), f);
}

template<   ss_typename_param_k R
        ,   ss_typename_param_k F
        >
inline ss_typename_type_k R::value_type r_min_element_2_impl(R r, F f, notional_range_tag const &)
{
    typedef ss_typename_type_k R::value_type    value_type_t;

    value_type_t    min_    =   value_type_t();

    for(; r; ++r)
    {
        if(f(min_, *r))
        {
            min_ = *r;
        }
    }

    return min_;
}

template<   ss_typename_param_k R
        ,   ss_typename_param_k F
        >
inline ss_typename_type_k R::value_type r_min_element_2_impl(R r, F f, basic_indirect_range_tag const &)
{
    return indirect_range_adaptor<R>(r).min_element(f);
}

template<   ss_typename_param_k R
        ,   ss_typename_param_k F
        >
inline ss_typename_type_k R::value_type r_min_element_2_impl(R r, F f, indirect_range_tag const &)
{
    return r.min_element(f);
}

/// Evaluates the minimum element in the range evaluated according to the given function
///
/// \param r The range. Cannot be closed
/// \param f The function used to evaluate the ordering
///
/// \note: Supports Notional, Iterable and Indirect Range types
/// \note: The behaviour is undefined if the range is closed
template<   ss_typename_param_k R
        ,   ss_typename_param_k F
        >
inline ss_typename_type_k R::value_type r_min_element(R r, F f)
{
    stlsoft_assert(r_distance(r) > 0);

    return r_min_element_2_impl(r, f, r);
}

/* *********************************************************
 * replace
 */

template<   ss_typename_param_k R
        ,   ss_typename_param_k T
        >
inline void r_replace_impl(R r, T oldVal, T newVal, iterable_range_tag const &)
{
    std::replace(r.begin(), r.end(), oldVal, newVal);
}

template<   ss_typename_param_k R
        ,   ss_typename_param_k T
        >
inline void r_replace_impl(R r, T oldVal, T newVal, indirect_range_tag const &)
{
    r.replace(r, oldVal, newVal);
}


/// Replaces all elements of the given old value with the new value
///
/// \param r The range
/// \param oldVal The value to search for
/// \param newVal The value to replace any elements with \c oldVal
///
/// \note: Supports Iterable and Indirect Range types
template<   ss_typename_param_k R
        ,   ss_typename_param_k T
        >
inline void r_replace(R r, T oldVal, T newVal)
{
    r_replace_impl(r, oldVal, newVal, r);
}


/* *********************************************************
 * replace_copy
 */

#if 0
template<   ss_typename_param_k RI
        ,   ss_typename_param_k RO
        ,   ss_typename_param_k T
        >
inline void r_replace_copy_impl(RI ri, RO ro, T oldVal, T newVal, iterable_range_tag const &, iterable_range_tag const &)
{
    std::replace_copy(ri.begin(), ri.end(), ro.begin(), oldVal, newVal);
}

template<   ss_typename_param_k RI
        ,   ss_typename_param_k RO
        ,   ss_typename_param_k T
        >
inline void r_replace_copy_impl(RI ri, RO ro, T oldVal, T newVal, indirect_range_tag const &, indirect_range_tag const &)
{
    ri.replace_copy(ro, oldVal, newVal);
}

/// Replaces all elements of the given old value with the new value
///
/// \param r The range
/// \param oldVal The value to search for
/// \param newVal The value to replace any elements with \c oldVal
///
/// \note: Supports Iterable and Indirect Range types
template<   ss_typename_param_k RI
        ,   ss_typename_param_k RO
        ,   ss_typename_param_k T
        >
inline void r_replace_copy(RI ri, RO ro, T oldVal, T newVal)
{
    r_replace_copy_impl(ri, ro, oldVal, newVal, ri, ro);
}
#endif /* 0 */

/* *********************************************************
 * replace_if
 */

template<   ss_typename_param_k R
        ,   ss_typename_param_k P
        ,   ss_typename_param_k T
        >
inline void r_replace_if_impl(R r, P pr, T newVal, iterable_range_tag const &)
{
    std::replace_if(r.begin(), r.end(), pr, newVal);
}

template<   ss_typename_param_k R
        ,   ss_typename_param_k P
        ,   ss_typename_param_k T
        >
inline void r_replace_if_impl(R r, P pr, T newVal, indirect_range_tag const &)
{
    r.replace_if(r, pr, newVal);
}

/// Replaces all elements matching the given predicate with the new value
///
/// \param r The range
/// \param pr The predicate for matching the old values to replace
/// \param newVal The value to replace any elements which match the given predicate
///
/// \note: Supports Iterable and Indirect Range types
template<   ss_typename_param_k R
        ,   ss_typename_param_k P
        ,   ss_typename_param_k T
        >
inline void r_replace_if(R r, P pr, T newVal)
{
    r_replace_if_impl(r, pr, newVal, r);
}


/* *********************************************************
 * replace_copy_if
 */

#if 0
template<   ss_typename_param_k RI
        ,   ss_typename_param_k RO
        ,   ss_typename_param_k P
        ,   ss_typename_param_k T
        >
inline void r_replace_copy_if_impl(RI ri, RO ro, P pr, T newVal, iterable_range_tag const &, iterable_range_tag const &)
{
    std::replace_copy_if(ri.begin(), ri.end(), ro.begin(), pr, newVal);
}

template<   ss_typename_param_k RI
        ,   ss_typename_param_k RO
        ,   ss_typename_param_k P
        ,   ss_typename_param_k T
        >
inline void r_replace_copy_if_impl(RI ri, RO ro, P pr, T newVal, notional_range_tag const &, notional_range_tag const &)
{
    for(; ri; ++ri, ++ro)
    {
        stlsoft_assert(!(!ro));

        if(pr(*ri))
        {
            *ro = newVal;
        }
    }
}

template<   ss_typename_param_k RI
        ,   ss_typename_param_k RO
        ,   ss_typename_param_k P
        ,   ss_typename_param_k T
        >
inline void r_replace_copy_if_impl(RI ri, RO ro, P pr, T newVal, indirect_range_tag const &, indirect_range_tag const &)
{
    ri.replace_copy_if(ro, pr, newVal);
}

template<   ss_typename_param_k RI
        ,   ss_typename_param_k RO
        ,   ss_typename_param_k P
        ,   ss_typename_param_k T
        >
inline void r_replace_copy_if(RI ri, RO ro, P pr, T newVal)
{
    r_replace_copy_if_impl(ri, ro, pe, newVal, ri, ro);
}
#endif /* 0 */

////////////////////////////////////////////////////////////////////////////////
// Unit-testing

#ifdef STLSOFT_UNITTEST

namespace unittest
{
    ss_bool_t test_rangelib_algorithms(unittest_reporter *r)
    {
        using stlsoft::unittest::unittest_initialiser;

        ss_bool_t               bSuccess    =   true;

        unittest_initialiser    init(r, "RangeLib", "algorithms", __FILE__);

#if 0
        if(NULL != pi1)
        {
            ator1.construct(pi1, 1968);

            if(1968 != *pi1)
            {
                r->report("construct() failed ", __LINE__);
                bSuccess = false;
            }
        }
#endif /* 0 */

        return bSuccess;
    }

    unittest_registrar    unittest_rangelib_algorithms(test_rangelib_algorithms);

} // namespace unittest

#endif /* STLSOFT_UNITTEST */

/* ////////////////////////////////////////////////////////////////////////// */

/// @} // end of group rangelib_algorithms

/* ////////////////////////////////////////////////////////////////////////// */

#ifndef _STLSOFT_NO_NAMESPACE
} // namespace stlsoft
#endif /* _STLSOFT_NO_NAMESPACE */

/* ////////////////////////////////////////////////////////////////////////// */

#endif /* !STLSOFT_INCL_RANGELIB_HPP_ALGORITHMS */

/* ////////////////////////////////////////////////////////////////////////// */
