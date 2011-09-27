/* ////////////////////////////////////////////////////////////////////////////
 * File:        comstl_task_allocator.h
 *
 * Purpose:     task_allocator class.
 *
 * Created:     19th January 2002
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


/// \file comstl_task_allocator.h
///
/// task_allocator class.

#ifndef COMSTL_INCL_H_COMSTL_TASK_ALLOCATOR
#define COMSTL_INCL_H_COMSTL_TASK_ALLOCATOR

#ifndef __STLSOFT_DOCUMENTATION_SKIP_SECTION
# define COMSTL_VER_H_COMSTL_TASK_ALLOCATOR_MAJOR       2
# define COMSTL_VER_H_COMSTL_TASK_ALLOCATOR_MINOR       0
# define COMSTL_VER_H_COMSTL_TASK_ALLOCATOR_REVISION    1
# define COMSTL_VER_H_COMSTL_TASK_ALLOCATOR_EDIT        50
#endif /* !__STLSOFT_DOCUMENTATION_SKIP_SECTION */

/* ////////////////////////////////////////////////////////////////////////////
 * Includes
 */

#ifndef COMSTL_INCL_H_COMSTL
# include "comstl.h"                    // Include the COMSTL root header
#endif /* !COMSTL_INCL_H_COMSTL */
#ifndef STLSOFT_INCL_H_STLSOFT_ALLOCATOR_BASE
# include "stlsoft_allocator_base.h"    // Include the STLSoft root header
#endif /* !STLSOFT_INCL_H_STLSOFT_ALLOCATOR_BASE */

/* /////////////////////////////////////////////////////////////////////////////
 * Namespace
 *
 * The COMSTL components are contained within the comstl namespace. This is
 * actually an alias for stlsoft::comstl_project,
 *
 * The definition matrix is as follows:
 *
 * _STLSOFT_NO_NAMESPACE    _COMSTL_NO_NAMESPACE    comstl definition
 * ---------------------    --------------------    -----------------
 *  not defined              not defined             = stlsoft::comstl_project
 *  not defined              defined                 not defined
 *  defined                  not defined             comstl
 *  defined                  defined                 not defined
 *
 */

#ifndef _COMSTL_NO_NAMESPACE
# if defined(_STLSOFT_NO_NAMESPACE) || \
     defined(__STLSOFT_DOCUMENTATION_SKIP_SECTION)
/* There is no stlsoft namespace, so must define ::comstl */
namespace comstl
{
# else
/* Define stlsoft::comstl_project */

namespace stlsoft
{

namespace comstl_project
{

# endif /* _STLSOFT_NO_NAMESPACE */
#endif /* !_COMSTL_NO_NAMESPACE */

/* ////////////////////////////////////////////////////////////////////////// */

/// \weakgroup libraries STLSoft Libraries
/// \brief The individual libraries

/// \weakgroup libraries_allocator Allocator Library
/// \ingroup libraries
/// \brief This library provides STL-compatible <code><b>allocator</b></code> types

/// \weakgroup comstl_allocator_library Allocator Library (COMSTL)
/// \ingroup COMSTL libraries_allocator
/// \brief This library provides STL-compatible <code><b>allocator</b></code> types for COM
/// @{

/* /////////////////////////////////////////////////////////////////////////////
 * Classes
 */

#ifndef __STLSOFT_DOCUMENTATION_SKIP_SECTION

template <ss_typename_param_k T>
class task_allocator;

// Specialisation for void
STLSOFT_TEMPLATE_SPECIALISATION
class task_allocator<void>
{
public:
    typedef void                            value_type;
    typedef task_allocator<void>            class_type;
    typedef void                            *pointer;
    typedef void const                      *const_pointer;
    typedef ptrdiff_t                       difference_type;
    typedef cs_size_t                       size_type;

#ifdef STLSOFT_CF_ALLOCATOR_REBIND_SUPPORT
    /// The allocator <b><code>rebind</code></b> structure
    template <ss_typename_param_k U>
    struct rebind
    {
        typedef task_allocator<U>           other;
    };
#endif /* STLSOFT_CF_ALLOCATOR_REBIND_SUPPORT */
};

#endif /* !__STLSOFT_DOCUMENTATION_SKIP_SECTION */

/// STL Allocator based on the COM task allocator
///
/// \param T The value_type of the allocator
template <ss_typename_param_k T>
class task_allocator
{
public:
    /// The value type
    typedef T                               value_type;
    /// The type of the current parameterisation
    typedef task_allocator<value_type>      class_type;
    /// The pointer type
    typedef value_type                      *pointer;
    /// The non-mutating (const) pointer type
    typedef value_type const                *const_pointer;
    /// The reference type
    typedef value_type                      &reference;
    /// The non-mutating (const) reference type
    typedef value_type const                &const_reference;
    /// The difference type
    typedef ptrdiff_t                       difference_type;
    /// The size type
    typedef cs_size_t                       size_type;
    /// The type used in deallocate()
#ifdef __STLSOFT_CF_ALLOCATOR_TYPED_DEALLOCATE_POINTER
    typedef pointer                             deallocate_pointer;
#else
    typedef void                                *deallocate_pointer;
#endif /* __STLSOFT_COMPILER_IS_MSVC && _MSC_VER == 1200 */

#ifdef STLSOFT_CF_ALLOCATOR_REBIND_SUPPORT
    /// The allocator <b><code>rebind</code></b> structure
    template <ss_typename_param_k U>
    struct rebind
    {
        typedef task_allocator<U>           other;
    };
#endif /* STLSOFT_CF_ALLOCATOR_REBIND_SUPPORT */

// Construction
public:
    /// Default constructor
    task_allocator() comstl_throw_0()
    {}
    /// Copy constructor
#ifndef __STLSOFT_CF_MEMBER_TEMPLATE_FUNCTION_SUPPORT
    task_allocator(const task_allocator &/* rhs */) comstl_throw_0()
    {}
#else
    template <ss_typename_param_k U>
    task_allocator(const task_allocator<U> &/* rhs */) comstl_throw_0()
    {}
#endif /* !__STLSOFT_CF_MEMBER_TEMPLATE_FUNCTION_SUPPORT */
    /// Destructor
    ~task_allocator() comstl_throw_0()
    {}

// Attributes
public:
    /// Returns the maximum number of allocatable entities
    size_type max_size() const comstl_throw_0()
    {
        return static_cast<size_type>(-1) / sizeof(value_type);
    }

// Conversion
public:
    /// Returns the address corresponding to the given reference
    ///
    /// \param x A reference to a \c value_type instance whose address will be calculated
    pointer address(reference x) const comstl_throw_0()
    {
        return &x;
    }
    /// Returns the address corresponding to the given non-mutable (const) reference
    ///
    /// \param x A non-mutable (const) reference to a \c value_type instance whose address will be calculated
    const_pointer address(const_reference x) const comstl_throw_0()
    {
        return &x;
    }

// Allocation
public:
    /// Allocates a block of memory sufficient to store \c n elements of type \c value_type
    ///
    /// \param n The number of elements to allocate
    /// \param pv A hint to enhance localisation (not used in this class)
    /// \return The allocated block, or the null pointer (if the allocation fails and the translation does not support throwing exceptions upon memory exhaustion)
#ifdef __STLSOFT_CF_ALLOCATOR_STATIC_ALLOCATE_METHODS
    static
#endif /* __STLSOFT_CF_ALLOCATOR_STATIC_ALLOCATE_METHODS */
    pointer allocate(size_type n, task_allocator<void>::const_pointer pv = NULL)
    {
        STLSOFT_SUPPRESS_UNUSED(pv);

        task_allocator<void>::pointer   p   =   static_cast<task_allocator<void>::pointer>(::CoTaskMemAlloc(n * sizeof(value_type)));

#ifdef __STLSOFT_CF_THROW_BAD_ALLOC
        if(p == NULL)
        {
            throw stlsoft_ns_qual_std(bad_alloc)();
        }
#endif /* __STLSOFT_CF_THROW_BAD_ALLOC */

        return static_cast<pointer>(p);
    }

#ifdef __STLSOFT_CF_ALLOCATOR_CHARALLOC_METHOD
    /// The type used in deallocate
    char *_Charalloc(size_type n)
    {
        return reinterpret_cast<char*>(allocate(n, NULL));
    }
#endif /* __STLSOFT_CF_ALLOCATOR_CHARALLOC_METHOD */

    /// Deallocates a pointer
    ///
    /// \param p The memory block to deallocate
    /// \param n The number of blocks to deallocate
#ifdef __STLSOFT_CF_ALLOCATOR_STATIC_ALLOCATE_METHODS
    static
#endif /* __STLSOFT_CF_ALLOCATOR_STATIC_ALLOCATE_METHODS */
    void deallocate(pointer p, size_type n)
    {
        STLSOFT_SUPPRESS_UNUSED(n);

        ::CoTaskMemFree(static_cast<HGLOBAL>(p));
    }

    /// Deallocates a pointer
    ///
    /// \param p The memory block to deallocate
#ifdef __STLSOFT_CF_ALLOCATOR_STATIC_ALLOCATE_METHODS
    static
#endif /* __STLSOFT_CF_ALLOCATOR_STATIC_ALLOCATE_METHODS */
    void deallocate(pointer p)
    {
        ::CoTaskMemFree(static_cast<HGLOBAL>(p));
    }

// Operations
public:
    /// In-place constructs an instance of the \c value_type, with the given value
    ///
    /// \param p The location in which to construct the instance
    /// \param x The value with which to copy construct the instance
    void construct(pointer p, value_type const &x)
    {
        new(p) value_type(x);
    }

    /// In-place constructs an instance of the \c value_type
    ///
    /// \param p The location in which to construct the instance
    void construct(pointer p)
    {
        new(p) value_type();
    }

    /// In-place destroys an instance
    ///
    /// \param p The instance whose destructor is to be called
    void destroy(pointer p) comstl_throw_0()
    {
        comstl_destroy_instance(T, value_type, p);
    }

// Not to be implemented
private:
    class_type const &operator =(class_type const &rhs);
};

#ifndef __STLSOFT_DOCUMENTATION_SKIP_SECTION

template <ss_typename_param_k T>
inline cs_bool_t operator ==(const task_allocator<T> &/* lhs */, const task_allocator<T> &/* rhs */)
{
    return cs_true_v;
}

template <ss_typename_param_k T>
inline cs_bool_t operator !=(const task_allocator<T> &/* lhs */, const task_allocator<T> &/* rhs */)
{
    return cs_false_v;
}

#endif /* !__STLSOFT_DOCUMENTATION_SKIP_SECTION */

////////////////////////////////////////////////////////////////////////////////
// Unit-testing

#ifdef STLSOFT_UNITTEST

namespace unittest
{
    ss_bool_t test_comstl_task_allocator(unittest_reporter *r)
    {
        using stlsoft::unittest::unittest_initialiser;

        ss_bool_t               bSuccess    =   true;

        unittest_initialiser    init(r, "COMSTL", "task_allocator", __FILE__);

        typedef task_allocator<int>  int_allocator_t;

        int_allocator_t ator1;

        int     *pi1    =   ator1.allocate(100);

        if(NULL != pi1)
        {
            ator1.construct(pi1, 1968);

            if(1968 != *pi1)
            {
                r->report("new_allocator failed ", __LINE__);
                bSuccess = false;
            }
        }

        ator1.deallocate(pi1);

        return bSuccess;
    }

    unittest_registrar    unittest_comstl_task_allocator(test_comstl_task_allocator);

} // namespace unittest

#endif /* STLSOFT_UNITTEST */

/* ////////////////////////////////////////////////////////////////////////// */

/// @} // end of group comstl_allocator_library

/* ////////////////////////////////////////////////////////////////////////// */

#ifndef _COMSTL_NO_NAMESPACE
# if defined(_STLSOFT_NO_NAMESPACE) || \
     defined(__STLSOFT_DOCUMENTATION_SKIP_SECTION)
} // namespace comstl
# else
} // namespace comstl_project
} // namespace stlsoft
# endif /* _STLSOFT_NO_NAMESPACE */
#endif /* !_COMSTL_NO_NAMESPACE */

/* ////////////////////////////////////////////////////////////////////////// */

#endif /* !COMSTL_INCL_H_COMSTL_TASK_ALLOCATOR */

/* ////////////////////////////////////////////////////////////////////////// */
