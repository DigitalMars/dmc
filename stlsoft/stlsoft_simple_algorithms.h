/* /////////////////////////////////////////////////////////////////////////////
 * File:        stlsoft_simple_algorithms.h
 *
 * Purpose:     Contains various simple self-contained algorithms.
 *
 * Created:     17th January 2002
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


/// \file stlsoft_simple_algorithms.h
///
/// Contains various simple self-contained algorithms.

#ifndef STLSOFT_INCL_H_STLSOFT_SIMPLE_ALGORITHMS
#define STLSOFT_INCL_H_STLSOFT_SIMPLE_ALGORITHMS

#ifndef __STLSOFT_DOCUMENTATION_SKIP_SECTION
# define STLSOFT_VER_H_STLSOFT_SIMPLE_ALGORITHMS_MAJOR        2
# define STLSOFT_VER_H_STLSOFT_SIMPLE_ALGORITHMS_MINOR        0
# define STLSOFT_VER_H_STLSOFT_SIMPLE_ALGORITHMS_REVISION     1
# define STLSOFT_VER_H_STLSOFT_SIMPLE_ALGORITHMS_EDIT         47
#endif /* !__STLSOFT_DOCUMENTATION_SKIP_SECTION */

/* /////////////////////////////////////////////////////////////////////////////
 * Includes
 */

#ifndef STLSOFT_INCL_H_STLSOFT
# include "stlsoft.h"               // Include the STLSoft root header
#endif /* !STLSOFT_INCL_H_STLSOFT */
#ifndef STLSOFT_INCL_H_STLSOFT_CONSTRAINTS
# include "stlsoft_constraints.h"  // Include the STLSoft root header
#endif /* !STLSOFT_INCL_H_STLSOFT_CONSTRAINTS */
#ifndef STLSOFT_INCL_H_TYPE_TRAITS
# include "stlsoft_type_traits.h"   //
#endif /* !STLSOFT_INCL_H_TYPE_TRAITS */
#include <algorithm>    // std::advance, std::for_each, etc.
#if defined(__STLSOFT_COMPILER_IS_BORLAND) || \
    defined(__STLSOFT_COMPILER_IS_INTEL) || \
    defined(__STLSOFT_COMPILER_IS_MSVC)
# include <memory.h>                    // memcpy
#else
# include <string.h>                    // memcpy
#endif /* __STLSOFT_COMPILER_IS_BORLAND */

/* /////////////////////////////////////////////////////////////////////////////
 * Namespace
 */

#ifndef _STLSOFT_NO_NAMESPACE
namespace stlsoft
{
#endif /* _STLSOFT_NO_NAMESPACE */

/* /////////////////////////////////////////////////////////////////////////////
 * Forwarding functions
 *
 * These are specified to keep the use of the filthy stlsoft_ns_qual_std()
 * macro to a minimum
 */

#ifndef __STLSOFT_DOCUMENTATION_SKIP_SECTION
# ifdef STLSOFT_PROVIDE_OWN_ALGORITHMS

template<   ss_typename_param_k T
        >
inline void swap_(T &lhs, T &rhs)
{
    T   t   =   rhs;
        rhs =   lhs;
        lhs =   t;
}

# else /* ? STLSOFT_PROVIDE_OWN_ALGORITHMS */

template<   ss_typename_param_k T
        >
inline void swap_(T &lhs, T &rhs)
{
    stlsoft_ns_qual_std(swap)(lhs, rhs);
}

# endif /* STLSOFT_PROVIDE_OWN_ALGORITHMS */
#endif /* !__STLSOFT_DOCUMENTATION_SKIP_SECTION */


/* /////////////////////////////////////////////////////////////////////////////
 * Algorithms
 */

// function for_each_if

/// Applies the function to all items in the series for which the predicate is
/// true.
///
/// \param I The iterator type
/// \param F The function type
/// \param P The predicate type
template<   ss_typename_param_k I
        ,   ss_typename_param_k F
        ,   ss_typename_param_k P
        >
inline F for_each_if(I first, I last, F function, P pred)
{
    for(; first != last; ++first)
    {
        if(pred(*first))
        {
            function(*first);
        }
    }

    return function;
}

// function for_each_count_success

/// Counts the number of items in the sequence which the predicate is true.
///
/// \param I The iterator type
/// \param P The predicate type
template<   ss_typename_param_k I
        ,   ss_typename_param_k P
        >
inline ss_size_t for_each_count_success(I first, I to, P pred)
{
    ss_size_t r;

    for (r = 0; first != to; ++first)
    {
        if(pred(*first))
        {
            ++r;
        }
    }

    return r;
}


// function for_each_set_value

/// Sets the value of all items in the sequence.
///
/// \param I The iterator type
/// \param V The value type
/// \return Returns the value
/// \note This is identical to
template<   ss_typename_param_k I
        ,   ss_typename_param_k V
        >
inline V const &for_each_set_value(I begin, I end, V const &v)
{
    for(; begin != end; ++begin)
    {
        *begin = v;
    }

    return v;
}


// function for_each_set_value_if

/// Sets the value of all items in the sequence.
///
/// \param I The iterator type
/// \param V The value type
/// \param P The predicate type
template<   ss_typename_param_k I
        ,   ss_typename_param_k V
        ,   ss_typename_param_k P
        >
inline V const &for_each_set_value_if(I begin, I end, V const &v, P pred)
{
    for(; begin != end; ++begin)
    {
        if(pred(*begin))
        {
            *begin = v;
        }
    }

    return v;
}



// function remove_duplicates_from_unordered_sequence

/// This algorithm removes duplicate entries from unordered sequences. It
/// necessarily runs in O(n) time, since it must do a bubble-like double
/// pass on the sequence (in order to work with unordered sequences).
///
/// \param C The container type
/// \param P The predicate type
template<   ss_typename_param_k C
        ,   ss_typename_param_k P
        >
inline void remove_duplicates_from_unordered_sequence(C &container, P pred)
{
    typedef ss_typename_type_k C::iterator  iterator_type;

#if 0
    for(iterator_type begin = container.begin(); begin != container.end(); ++begin)
    {
        iterator_type it = begin;

        for(++it; it != container.end(); ++it)
        {
            if(pred(*begin, *it))
            {
                //
                container.erase(it);
                it = begin;
            }
        }
    }
#endif /* 0 */

    ss_size_t       last;
    ss_size_t       index;
    iterator_type   begin;

    for(index = 0, last = 0, begin = container.begin(); begin != container.end(); )
    {
        iterator_type   it  =   begin;
        iterator_type   end =   container.end();

        if(++it == end)
        {
            ++begin;
        }
        else
        {
            for(;;)
            {
                if(pred(*begin, *it))
                {
                    // Erase the element. We now assume that all iterators
                    // are invalidated
                    container.erase(it);
                    // Remember the last erasure point
                    last = index;
                    // Set begin back to the start of the sequence
                    begin = container.begin();
                    // Move begin to the point where it was at the last erasure
                    stlsoft_ns_qual_std(advance)(begin, last);
                    break;
                }
                else
                {
                    if(++it == end)
                    {
                        ++begin;
                        ++index;
                        break;
                    }
                }
            }
        }
    }

    // Borland seems to think that 'last' is not used, when it clearly is.
    STLSOFT_SUPPRESS_UNUSED(last);
}




// function fill_all
/// Carries out for_each on the range of items in a container
///
/// \param C The container type
/// \param V The value type
template<   ss_typename_param_k C
        ,   ss_typename_param_k V
        >
inline void fill_all(C &c, V const &v)
{
    fill(c.begin(), c.end(), v);
}

#ifdef __STLSOFT_CF_STATIC_ARRAY_SIZE_DETERMINATION_SUPPORT
// function fill_all
/// Carries out for_each on the range of items in an array
///
/// \param T The array value type
/// \param N The dimension of the array
/// \param V The value type
template<   ss_typename_param_k T
        ,   ss_size_t           N
        ,   ss_typename_param_k V
        >
inline void fill_all(T (&ar)[N], V const &v)
{
    fill(&ar[0], &ar[N], v);
}
#endif /* __STLSOFT_CF_STATIC_ARRAY_SIZE_DETERMINATION_SUPPORT */



// function for_all
/// Carries out for_each on the range of items in a container
///
/// \param C The container type
/// \param F The function type
template<   ss_typename_param_k C
        ,   ss_typename_param_k F
        >
inline F for_all(C &c, F f)
{
    return stlsoft_ns_qual_std(for_each)(c.begin(), c.end(), f);
}

#ifdef __STLSOFT_CF_STATIC_ARRAY_SIZE_DETERMINATION_SUPPORT
// function for_all
/// Carries out for_each on the range of items in an array
///
/// \param T The array value type
/// \param N The dimension of the array
/// \param F The function type
template<   ss_typename_param_k T
        ,   ss_size_t           N
        ,   ss_typename_param_k F
        >
inline F for_all(T (&ar)[N], F f)
{
    return stlsoft_ns_qual_std(for_each)(&ar[0], &ar[N], f);
}
#endif /* __STLSOFT_CF_STATIC_ARRAY_SIZE_DETERMINATION_SUPPORT */



// function for_all_r
/// Carries out for_each on the reverse range of items in a container
///
/// \param C The container type
/// \param F The function type
template<   ss_typename_param_k C
        ,   ss_typename_param_k F
        >
inline F for_all_r(C &c, F f)
{
    return stlsoft_ns_qual_std(for_each)(c.rbegin(), c.rend(), f);
}



// function copy_all
/// Carries out copy on all the items in a container
///
/// \param C The container type
/// \param O The output iterator type
template<   ss_typename_param_k C
        ,   ss_typename_param_k O
        >
inline O copy_all(C &c, O o)
{
    return stlsoft_ns_qual_std(copy)(c.begin(), c.end(), o);
}

#ifdef __STLSOFT_CF_STATIC_ARRAY_SIZE_DETERMINATION_SUPPORT
// function for_all
/// Carries out copy on the range of items in an array
///
/// \param T The array value type
/// \param N The dimension of the array
/// \param O The output iterator type
template<   ss_typename_param_k T
        ,   ss_size_t           N
        ,   ss_typename_param_k O
        >
inline O copy_all(T (&ar)[N], O o)
{
    return stlsoft_ns_qual_std(copy)(&ar[0], &ar[N], o);
}
#endif /* __STLSOFT_CF_STATIC_ARRAY_SIZE_DETERMINATION_SUPPORT */



// function for_each_preinc
/// Carries out for_each on the range, using pre-increment on the iterator
///
/// \param I The iterator type
/// \param F The function type
template<   ss_typename_param_k I
        ,   ss_typename_param_k F
        >
inline F for_each_preinc(I first, I last, F f)
{
    for(; first != last; ++first)
    {
        f(*first);
    }

    return f;
}


// function for_each_postinc
/// Carries out for_each on the range, using post-increment on the iterator
///
/// \param I The iterator type
/// \param F The function type
template<   ss_typename_param_k I
        ,   ss_typename_param_k F
        >
inline F for_each_postinc(I first, I last, F f)
{
    for(; first != last; first++)
    {
        f(*first);
    }

    return f;
}

/* ////////////////////////////////////////////////////////////////////////// */

template<   ss_typename_param_k I
        ,   ss_typename_param_k O
        >
inline void pod_copy(I first, I last, O dest)
{
#if defined(__STLSOFT_COMPILER_IS_BORLAND) || \
    defined(__STLSOFT_COMPILER_IS_MWERKS)
    stlsoft_ns_qual_std(copy)(&first[0], &last[0], &dest[0]);
#else /* ? compiler */
    stlsoft_static_assert(sizeof(*dest) == sizeof(*first));
# if defined(__STLSOFT_CF_TEMPLATE_PARTIAL_SPECIALISATION_SUPPORT)
        // Both must be non-pointers, or must point to POD types
    typedef ss_typename_type_k base_type_traits<I>::base_type   i_base_type;
    typedef ss_typename_type_k base_type_traits<O>::base_type   o_base_type;

    stlsoft_constraint_must_be_pod(i_base_type);
    stlsoft_constraint_must_be_pod(o_base_type);
# endif /* __STLSOFT_CF_TEMPLATE_PARTIAL_SPECIALISATION_SUPPORT */

    ss_size_t n = last - first;

    if(0 != n)
    {
        memcpy(&dest[0], &first[0], n * sizeof(*first));
    }
#endif /* compiler */
}

template<   ss_typename_param_k I
        ,   ss_typename_param_k O
        >
inline void pod_copy_n(O dest, I src, ss_size_t n)
{
#if defined(__STLSOFT_COMPILER_IS_BORLAND) || \
    defined(__STLSOFT_COMPILER_IS_MWERKS)
    stlsoft_ns_qual_std(copy)(&src[0], &src[n], &dest[0]);
#else /* ? compiler */
    stlsoft_static_assert(sizeof(*dest) == sizeof(*src));
# if defined(__STLSOFT_CF_TEMPLATE_PARTIAL_SPECIALISATION_SUPPORT)
        // Both must be non-pointers, or must point to POD types
    typedef ss_typename_type_k base_type_traits<I>::base_type   i_base_type;
    typedef ss_typename_type_k base_type_traits<O>::base_type   o_base_type;

    stlsoft_constraint_must_be_pod(i_base_type);
    stlsoft_constraint_must_be_pod(o_base_type);
# endif /* __STLSOFT_CF_TEMPLATE_PARTIAL_SPECIALISATION_SUPPORT */

    if(0 != n)
    {
        memcpy(&dest[0], &src[0], n * sizeof(*dest));
    }
#endif /* compiler */
}

template<   ss_typename_param_k I
        ,   ss_typename_param_k O
        >
inline void pod_move(I first, I last, O dest)
{
#if defined(__STLSOFT_COMPILER_IS_BORLAND) || \
    defined(__STLSOFT_COMPILER_IS_MWERKS)
    stlsoft_ns_qual_std(copy)(&first[0], &last[0], &dest[0]);
#else /* ? compiler */
    stlsoft_static_assert(sizeof(*first) == sizeof(*dest));
# if defined(__STLSOFT_CF_TEMPLATE_PARTIAL_SPECIALISATION_SUPPORT)
        // Both must be non-pointers, or must point to POD types
    typedef ss_typename_type_k base_type_traits<I>::base_type   i_base_type;
    typedef ss_typename_type_k base_type_traits<O>::base_type   o_base_type;

    stlsoft_constraint_must_be_pod(i_base_type);
    stlsoft_constraint_must_be_pod(o_base_type);
# endif /* __STLSOFT_CF_TEMPLATE_PARTIAL_SPECIALISATION_SUPPORT */

    ss_size_t n = last - first;

    if(0 != n)
    {
        memmove(dest, first, n * sizeof(*first));
    }
#endif /* compiler */
}

template<   ss_typename_param_k I
        ,   ss_typename_param_k O
        >
inline void pod_move_n(O dest, I src, ss_size_t n)
{
#if defined(__STLSOFT_COMPILER_IS_BORLAND) || \
    defined(__STLSOFT_COMPILER_IS_MWERKS)
    stlsoft_ns_qual_std(copy)(&src[0], &src[n], &dest[0]);
#else /* ? compiler */
    stlsoft_static_assert(sizeof(*dest) == sizeof(*src));
# if defined(__STLSOFT_CF_TEMPLATE_PARTIAL_SPECIALISATION_SUPPORT)
        // Both must be non-pointers, or must point to POD types
    typedef ss_typename_type_k base_type_traits<I>::base_type   i_base_type;
    typedef ss_typename_type_k base_type_traits<O>::base_type   o_base_type;

    stlsoft_constraint_must_be_pod(i_base_type);
    stlsoft_constraint_must_be_pod(o_base_type);
# endif /* __STLSOFT_CF_TEMPLATE_PARTIAL_SPECIALISATION_SUPPORT */

    if(0 != n)
    {
        memmove(&dest[0], &src[0], n * sizeof(*dest));
    }
#endif /* compiler */
}


template<   ss_typename_param_k O
        ,   ss_typename_param_k V
        >
inline void pod_fill_n(O dest, ss_size_t n, V const &v)
{
    stlsoft_ns_qual_std(fill_n)(dest, n, v);
}

inline void pod_fill_n(char *dest, ss_size_t n, int v)
{
    memset(dest, v, n);
}
inline void pod_fill_n(signed char *dest, ss_size_t n, int v)
{
    memset(dest, v, n);
}
inline void pod_fill_n(unsigned char *dest, ss_size_t n, int v)
{
    memset(dest, v, n);
}

////////////////////////////////////////////////////////////////////////////////
// Unit-testing

#ifdef STLSOFT_UNITTEST

namespace unittest
{
    ss_bool_t test_stlsoft_simple_algorithms(unittest_reporter *r)
    {
        ss_bool_t               bSuccess    =   true;

        unittest_initialiser    init(r, "STLSoft", "simple_algorithms", __FILE__);

#if 0
        if(<<TODO>>)
        {
            r->report("<<TODO>> failed ", __LINE__);
            bSuccess = false;
        }
#endif /* 0 */

        return bSuccess;
    }

    unittest_registrar    unittest_stlsoft_simple_algorithms(test_stlsoft_simple_algorithms);

} // namespace unittest

#endif /* STLSOFT_UNITTEST */

/* ////////////////////////////////////////////////////////////////////////// */

#ifndef _STLSOFT_NO_NAMESPACE
} // namespace stlsoft
#endif /* _STLSOFT_NO_NAMESPACE */

/* ////////////////////////////////////////////////////////////////////////// */

#endif /* !STLSOFT_INCL_H_STLSOFT_SIMPLE_ALGORITHMS */

/* ////////////////////////////////////////////////////////////////////////// */
