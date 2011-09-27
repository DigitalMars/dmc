/* /////////////////////////////////////////////////////////////////////////////
 * File:        stlsoft_associative_container_veneer.h
 *
 * Purpose:     RRID (veneer for associative containers)
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


/// \file stlsoft_associative_container_veneer.h
///
/// RRID (veneer for associative containers)

#ifndef STLSOFT_INCL_H_STLSOFT_ASSOCIATIVE_CONTAINER_VENEER
#define STLSOFT_INCL_H_STLSOFT_ASSOCIATIVE_CONTAINER_VENEER

#ifndef __STLSOFT_DOCUMENTATION_SKIP_SECTION
# define STLSOFT_VER_H_STLSOFT_ASSOCIATIVE_CONTAINER_VENEER_MAJOR       2
# define STLSOFT_VER_H_STLSOFT_ASSOCIATIVE_CONTAINER_VENEER_MINOR       0
# define STLSOFT_VER_H_STLSOFT_ASSOCIATIVE_CONTAINER_VENEER_REVISION    1
# define STLSOFT_VER_H_STLSOFT_ASSOCIATIVE_CONTAINER_VENEER_EDIT        19
#endif /* !__STLSOFT_DOCUMENTATION_SKIP_SECTION */

/* /////////////////////////////////////////////////////////////////////////////
 * Compatibility
 */

/*
[Incompatibilies-start]
__STLSOFT_COMPILER_IS_BORLAND:
[Incompatibilies-end]
 */

/* /////////////////////////////////////////////////////////////////////////////
 * Includes
 */

#ifndef STLSOFT_INCL_H_STLSOFT
# include "stlsoft.h"               // Include the STLSoft root header
#endif /* !STLSOFT_INCL_H_STLSOFT */

#if defined(__STLSOFT_COMPILER_IS_BORLAND)
# error stlsoft_associative_container_veneer.h is incompatible with the Borland C/C++ compiler
#endif /* compiler */

#ifndef STLSOFT_INCL_H_STLSOFT_FUNCTIONALS
# include "stlsoft_functionals.h"   // stlsoft::noop_function
#endif /* !STLSOFT_INCL_H_STLSOFT_FUNCTIONALS */

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

/// Applies per-item functions to an associative container's items at its destruction
///
/// \param T The sequence container type
/// \param FV The function object type applied to the values of the container
/// \param FK The function object type applied to the keys of the container. For translators that support default template arguments this defaults to noop_function<T::key_type>
template<   ss_typename_param_k T
        ,   ss_typename_param_k FV
#ifdef __STLSOFT_CF_TEMPLATE_CLASS_DEFAULT_CLASS_ARGUMENT_SUPPORT
        ,   ss_typename_param_k FK = noop_function<ss_typename_type_def_k T::key_type>
#else
        ,   ss_typename_param_k FK /* = noop_function<T> */
#endif /* __STLSOFT_CF_TEMPLATE_CLASS_DEFAULT_CLASS_ARGUMENT_SUPPORT */
        >
class associative_container_veneer
    : public T
{
public:
    /// The container type
    typedef T                                                       container_type;
    /// The function object type applied to the container's values
    typedef FV                                                      value_destruction_function_type;
    /// The function object type applied to the container's keys
    typedef FK                                                      key_destruction_function_type;
    /// The current parameterisation of the type
    typedef associative_container_veneer<T, FV, FK>                 class_type;
    /// The parent class type
private:
    typedef T                                                       parent_class_type;
public:
    /// The container's \c key_compare type
    typedef ss_typename_type_k parent_class_type::key_compare       key_compare;
    /// The container's \c allocator_type type
    typedef ss_typename_type_k parent_class_type::allocator_type    allocator_type;
    /// The container's \c size_type type
    typedef ss_typename_type_k parent_class_type::size_type         size_type;

// Construction
public:
#ifdef __STLSOFT_CF_MEMBER_TEMPLATE_FUNCTION_SUPPORT
    /// Default constructor
    associative_container_veneer()
    {}

    /// Copy constructor
    associative_container_veneer(class_type const &rhs)
        : parent_class_type(rhs)
    {}

#if !defined(__STLSOFT_COMPILER_IS_DMC)
    /// Constructs with the given predicate
    ss_explicit_k associative_container_veneer(const key_compare& comp)
        : parent_class_type(comp)
    {}
    /// Constructs with the given predicate and allocator
    associative_container_veneer(const key_compare& comp, allocator_type const &a)
        : parent_class_type(comp, a)
    {}
    /// Constructs from a range
    template <ss_typename_param_k I>
    associative_container_veneer(I i1, I i2)
        : parent_class_type(i1, i2)
    {}
    /// Constructs from a range, with the given predicate
    template <ss_typename_param_k I>
    associative_container_veneer(I i1, I i2, key_compare const &comp)
        : parent_class_type(i1, i2, comp)
    {}
    /// Constructs from a range, with the given predicate and allocator
    template <ss_typename_param_k I>
    associative_container_veneer(I i1, I i2, key_compare const &comp, allocator_type const &a)
        : parent_class_type(i1, i2, comp, a)
    {}
#else
    template <ss_typename_param_k N1>
    ss_explicit_k associative_container_veneer(N1 n1)
        : parent_class_type(n1)
    {}
    template<   ss_typename_param_k N1
            ,   ss_typename_param_k N2
            >
    associative_container_veneer(N1 n1, N2 n2)
        : parent_class_type(n1, n2)
    {}
    template<   ss_typename_param_k N1
            ,   ss_typename_param_k N2
            ,   ss_typename_param_k N3
            >
    associative_container_veneer(N1 n1, N2 n2, N3 n3)
        : parent_class_type(n1, n2, n3)
    {}
    template<   ss_typename_param_k N1
            ,   ss_typename_param_k N2
            ,   ss_typename_param_k N3
            ,   ss_typename_param_k N4
            >
    associative_container_veneer(N1 n1, N2 n2, N3 n3, N4 n4)
        : parent_class_type(n1, n2, n3, n4)
    {}
    template<   ss_typename_param_k N1
            ,   ss_typename_param_k N2
            ,   ss_typename_param_k N3
            ,   ss_typename_param_k N4
            ,   ss_typename_param_k N5
            >
    associative_container_veneer(N1 n1, N2 n2, N3 n3, N4 n4, N5 n5)
        : parent_class_type(n1, n2, n3, n4, n5)
    {}
#endif /* !__STLSOFT_COMPILER_IS_DMC */
#endif /* !__STLSOFT_CF_MEMBER_TEMPLATE_FUNCTION_SUPPORT */

    /// Destructor, within which all remaining entries are subject to the
    /// key and value destruction functions
    ~associative_container_veneer() stlsoft_throw_0()
    {
        // Determine the destruction function type, allowing for
        // compilers that do not work with member-template functions
        typedef select_both <   key_destruction_function_type
                            ,   value_destruction_function_type
#ifndef __STLSOFT_CF_MEMBER_TEMPLATE_FUNCTION_SUPPORT
                            ,   ss_typename_type_k container_type::iterator::value_type
#endif // __STLSOFT_CF_MEMBER_TEMPLATE_FUNCTION_SUPPORT
                            >   destruction_function_t;

        // Simply iterate through the sequence contents and call
        // the destruction functions on each item in turn.

        ss_typename_type_k container_type::iterator b   =   container_type::begin();
        ss_typename_type_k container_type::iterator e   =   container_type::end();

        for(destruction_function_t fn; b != e; ++b)
        {
            fn(*b);
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

/* ////////////////////////////////////////////////////////////////////////// */

#ifndef _STLSOFT_NO_NAMESPACE
} // namespace stlsoft
#endif /* _STLSOFT_NO_NAMESPACE */

/* ////////////////////////////////////////////////////////////////////////// */

#endif /* !STLSOFT_INCL_H_STLSOFT_ASSOCIATIVE_CONTAINER_VENEER */

/* ////////////////////////////////////////////////////////////////////////// */
