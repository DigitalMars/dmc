/* /////////////////////////////////////////////////////////////////////////////
 * File:        stlsoft_sequence_container_veneer.h
 *
 * Purpose:     RRID (veneer for sequence containers)
 *
 * Created:     2nd October 2002
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


/// \file stlsoft_sequence_container_veneer.h
///
/// RRID (veneer for sequence containers)

#ifndef STLSOFT_INCL_H_STLSOFT_SEQUENCE_CONTAINER_VENEER
#define STLSOFT_INCL_H_STLSOFT_SEQUENCE_CONTAINER_VENEER

#ifndef __STLSOFT_DOCUMENTATION_SKIP_SECTION
# define STLSOFT_VER_H_STLSOFT_SEQUENCE_CONTAINER_VENEER_MAJOR      2
# define STLSOFT_VER_H_STLSOFT_SEQUENCE_CONTAINER_VENEER_MINOR      0
# define STLSOFT_VER_H_STLSOFT_SEQUENCE_CONTAINER_VENEER_REVISION   1
# define STLSOFT_VER_H_STLSOFT_SEQUENCE_CONTAINER_VENEER_EDIT       23
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
# include "stlsoft.h"   // Include the STLSoft root header
#endif /* !STLSOFT_INCL_H_STLSOFT */

#if defined(__STLSOFT_COMPILER_IS_MSVC) && \
    _MSC_VER < 1200
# error stlsoft_sequence_container_veneer.h is not compatible with Visual C++ 5.0 or earlier
#endif /* _MSC_VER < 1200 */

#ifdef STLSOFT_UNITTEST
# include <algorithm>
# include <list>
# include <vector>
#endif /* STLSOFT_UNITTEST */

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

/// Applies a per-item function to a sequence container's items at its destruction
///
/// \param T The sequence container type
/// \param F The function object type
template<   ss_typename_param_k T
        ,   ss_typename_param_k F
        >
class sequence_container_veneer
    : public T
{
public:
    /// The container type
    typedef T                                                       container_type;
    /// The function object type applied to the container's items
    typedef F                                                       element_destruction_function_type;
    /// The current parameterisation of the type
    typedef sequence_container_veneer<T, F>                         class_type;
private:
    typedef T                                                       parent_class_type;
public:
    /// The container's \c allocator_type type
    typedef ss_typename_type_k parent_class_type::allocator_type    allocator_type;
    /// The container's \c size_type type
    typedef ss_typename_type_k parent_class_type::size_type         size_type;

// Construction
public:
#ifdef __STLSOFT_CF_MEMBER_TEMPLATE_FUNCTION_SUPPORT
    /// Default constructor
    sequence_container_veneer()
    {}

    /// Copy constructor
    sequence_container_veneer(class_type const &rhs)
        : parent_class_type(rhs)
    {}

#if !defined(__STLSOFT_COMPILER_IS_DMC)
    /// Constructs with the given allocator
    ss_explicit_k sequence_container_veneer(allocator_type const &a)
        : parent_class_type(a)
    {}
    /// Constructs from a range
    template <ss_typename_param_k I>
    sequence_container_veneer(I i1, I i2)
        : parent_class_type(i1, i2)
    {}
    /// Constructs from a range with the given allocator
    template <ss_typename_param_k I>
    sequence_container_veneer(I i1, I i2, allocator_type const &a)
        : parent_class_type(i1, i2, a)
    {}
    /// Constructs with the given number of elements (initialised with the given value)
    template <ss_typename_param_k V>
    sequence_container_veneer(size_type n, V v)
        : parent_class_type(n, v)
    {}
    /// Constructs with the given number of elements (initialised with the given value) with the given allocator
    template <ss_typename_param_k V>
    sequence_container_veneer(size_type n, V v, allocator_type const &a)
        : parent_class_type(n, v, a)
    {}
#else
    template <ss_typename_param_k N1>
    ss_explicit_k sequence_container_veneer(N1 n1)
        : parent_class_type(n1)
    {}
    template<   ss_typename_param_k N1
            ,   ss_typename_param_k N2
            >
    sequence_container_veneer(N1 n1, N2 n2)
        : parent_class_type(n1, n2)
    {}
    template<   ss_typename_param_k N1
            ,   ss_typename_param_k N2
            ,   ss_typename_param_k N3
            >
    sequence_container_veneer(N1 n1, N2 n2, N3 n3)
        : parent_class_type(n1, n2, n3)
    {}
    template<   ss_typename_param_k N1
            ,   ss_typename_param_k N2
            ,   ss_typename_param_k N3
            ,   ss_typename_param_k N4
            >
    sequence_container_veneer(N1 n1, N2 n2, N3 n3, N4 n4)
        : parent_class_type(n1, n2, n3, n4)
    {}
    template<   ss_typename_param_k N1
            ,   ss_typename_param_k N2
            ,   ss_typename_param_k N3
            ,   ss_typename_param_k N4
            ,   ss_typename_param_k N5
            >
    sequence_container_veneer(N1 n1, N2 n2, N3 n3, N4 n4, N5 n5)
        : parent_class_type(n1, n2, n3, n4, n5)
    {}
#endif /* !__STLSOFT_COMPILER_IS_DMC */
#endif /* !__STLSOFT_CF_MEMBER_TEMPLATE_FUNCTION_SUPPORT */

    /// Destructor, within which all remaining entries are subject to the
    /// destruction function
    ~sequence_container_veneer() stlsoft_throw_0()
    {
        // Simply iterate through the sequence contents and call
        // the destruction function on each item in turn.
        parent_class_type                           *this_  =   this;

        ss_typename_type_k container_type::iterator b       =   this_->begin();
        ss_typename_type_k container_type::iterator e       =   this_->end();

        for(; b != e; ++b)
        {
            element_destruction_function_type()(*b);
        }
    }

    /// Copy assignment operator
    ///
    /// This method is provided in case the parameterising class provides the
    /// operator. If it does not, then the compiler will ignore it unless a
    /// call is made to it, in which case an error would have been reported
    /// anyway
    class_type &operator =(class_type const &rhs)
    {
        parent_class_type::operator =(rhs);

        return *this;
    }

protected:
    /// Hidden in accordance with the constraints of the
    /// <a href = "http://synesis.com.au/resources/articles/cpp/veneers.pdf">veneer</a> concept
    void *operator new(size_t )
    {
        return 0;
    }
    /// Hidden in accordance with the constraints of the
    /// <a href = "http://synesis.com.au/resources/articles/cpp/veneers.pdf">veneer</a> concept
    void operator delete(void *)
    {}
};

////////////////////////////////////////////////////////////////////////////////
// Unit-testing

#ifdef STLSOFT_UNITTEST

namespace unittest
{
# if !defined(__STLSOFT_COMPILER_IS_MSVC) || \
     _MSC_VER >= 1200
    namespace
    {
#endif /* compiler */
        struct null_int_functor
        {
        public:
            void operator ()(int const &)
            {}
        };
# if !defined(__STLSOFT_COMPILER_IS_MSVC) || \
     _MSC_VER >= 1200
    } // namespace
#endif /* compiler */

    ss_bool_t test_stlsoft_sequence_container_veneer(unittest_reporter *r)
    {
        ss_bool_t               bSuccess    =   true;

        unittest_initialiser    init(r, "STLSoft", "sequence_container_veneer", __FILE__);

        typedef stlsoft_ns_qual_std(list)<int>      list_t;
        typedef stlsoft_ns_qual_std(vector)<int>    vector_t;

        sequence_container_veneer<list_t, null_int_functor>     l;
        sequence_container_veneer<vector_t, null_int_functor>   v;
        int                                                     i;
        int                                                     total;

        for(i = 0, total = 0; i < 10; ++i)
        {
            l.push_front(i);
            v.push_back(i);

            total += i;
        }

        if(stlsoft_ns_qual_std(accumulate)(l.begin(), l.end(), static_cast<long>(0)) != total)
        {
            r->report("sequence_container_veneer<list> contents (forward iteration) test failed ", __LINE__);
            bSuccess = false;
        }

#if defined(__STLSOFT_CF_BIDIRECTIONAL_ITERATOR_SUPPORT)
        if(stlsoft_ns_qual_std(accumulate)(l.rbegin(), l.rend(), static_cast<long>(0)) != total)
        {
            r->report("sequence_container_veneer<list> contents (forward iteration) test failed ", __LINE__);
            bSuccess = false;
        }
#endif /* __STLSOFT_CF_BIDIRECTIONAL_ITERATOR_SUPPORT */

        if(stlsoft_ns_qual_std(accumulate)(v.begin(), v.end(), static_cast<long>(0)) != total)
        {
            r->report("sequence_container_veneer<vector> contents (forward iteration) test failed ", __LINE__);
            bSuccess = false;
        }

#if defined(__STLSOFT_CF_BIDIRECTIONAL_ITERATOR_SUPPORT)
        if(stlsoft_ns_qual_std(accumulate)(v.rbegin(), v.rend(), static_cast<long>(0)) != total)
        {
            r->report("sequence_container_veneer<vector> contents (forward iteration) test failed ", __LINE__);
            bSuccess = false;
        }
#endif /* __STLSOFT_CF_BIDIRECTIONAL_ITERATOR_SUPPORT */

        return bSuccess;
    }

    unittest_registrar    unittest_stlsoft_sequence_container_veneer(test_stlsoft_sequence_container_veneer);

} // namespace unittest

#endif /* STLSOFT_UNITTEST */

/* ////////////////////////////////////////////////////////////////////////// */

#ifndef _STLSOFT_NO_NAMESPACE
} // namespace stlsoft
#endif /* _STLSOFT_NO_NAMESPACE */

/* ////////////////////////////////////////////////////////////////////////// */

#endif /* !STLSOFT_INCL_H_STLSOFT_SEQUENCE_CONTAINER_VENEER */

/* ////////////////////////////////////////////////////////////////////////// */
