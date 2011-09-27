/* ////////////////////////////////////////////////////////////////////////////
 * File:        stlsoft_null_allocator.h (originally MRAtors.h; ::SynesisCrt)
 *
 * Purpose:     stlsoft_null_allocator class - doesn't allocate.
 *
 * Created:     2nd January 2001
 * Updated:     11th September 2004
 *
 * Home:        http://stlsoft.org/
 *
 * Copyright (c) 2001-2004, Matthew Wilson and Synesis Software
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


/// \file stlsoft_null_allocator.h
///
/// stlsoft_null_allocator class - doesn't allocate.

#ifndef STLSOFT_INCL_H_STLSOFT_NULL_ALLOCATOR
#define STLSOFT_INCL_H_STLSOFT_NULL_ALLOCATOR

#ifndef __STLSOFT_DOCUMENTATION_SKIP_SECTION
# define STLSOFT_VER_H_STLSOFT_NULL_ALLOCATOR_MAJOR       2
# define STLSOFT_VER_H_STLSOFT_NULL_ALLOCATOR_MINOR       0
# define STLSOFT_VER_H_STLSOFT_NULL_ALLOCATOR_REVISION    1
# define STLSOFT_VER_H_STLSOFT_NULL_ALLOCATOR_EDIT        47
#endif /* !__STLSOFT_DOCUMENTATION_SKIP_SECTION */

/* ////////////////////////////////////////////////////////////////////////////
 * Includes
 */

#ifndef STLSOFT_INCL_H_STLSOFT
# include "stlsoft.h"                   // Include the STLSoft root header
#endif /* !STLSOFT_INCL_H_STLSOFT */
#ifndef STLSOFT_INCL_H_STLSOFT_ALLOCATOR_BASE
# include "stlsoft_allocator_base.h"    // Include the STLSoft root header
#endif /* !STLSOFT_INCL_H_STLSOFT_ALLOCATOR_BASE */
#ifndef STLSOFT_INCL_H_STLSOFT_SAP_CAST
# include "stlsoft_sap_cast.h"
#endif /* !STLSOFT_INCL_H_STLSOFT_SAP_CAST */

/* ////////////////////////////////////////////////////////////////////////////
 * Namespace
 */

#ifndef _STLSOFT_NO_NAMESPACE
namespace stlsoft
{
#endif /* _STLSOFT_NO_NAMESPACE */

/* ////////////////////////////////////////////////////////////////////////// */

/// \weakgroup libraries STLSoft Libraries
/// \brief The individual libraries

/// \weakgroup libraries_allocator Allocator Library
/// \ingroup libraries
/// \brief This library provides STL-compatible <code><b>allocator</b></code> types

/// \weakgroup stlsoft_allocator_library Allocator Library (STLSoft)
/// \ingroup STLSoft libraries_allocator
/// \brief This library provides STL-compatible <code><b>allocator</b></code> types
/// @{

/* /////////////////////////////////////////////////////////////////////////////
 * Classes
 */

#ifndef __STLSOFT_DOCUMENTATION_SKIP_SECTION

template <ss_typename_param_k T>
class null_allocator;

// Specialisation for void
STLSOFT_TEMPLATE_SPECIALISATION
class null_allocator<void>
{
public:
    typedef void                                value_type;
    typedef null_allocator<void>                class_type;
    typedef void                                *pointer;
    typedef void const                          *const_pointer;
    typedef ptrdiff_t                           difference_type;
    typedef ss_size_t                           size_type;

#ifdef STLSOFT_CF_ALLOCATOR_REBIND_SUPPORT
    /// The allocator <b><code>rebind</code></b> structure
    template <ss_typename_param_k U>
    struct rebind
    {
        typedef null_allocator<U>             other;
    };
#endif /* STLSOFT_CF_ALLOCATOR_REBIND_SUPPORT */
};

#endif /* !__STLSOFT_DOCUMENTATION_SKIP_SECTION */

/// STL Allocator which fails all allocation requests
///
/// \param T The value_type of the allocator
template <ss_typename_param_k T>
class null_allocator
{
public:
    /// The value type
    typedef T                                   value_type;
    /// The type of the current parameterisation
    typedef null_allocator<value_type>          class_type;
    /// The pointer type
    typedef value_type                          *pointer;
    /// The non-mutating (const) pointer type
    typedef value_type const                    *const_pointer;
    /// The reference type
    typedef value_type                          &reference;
    /// The non-mutating (const) reference type
    typedef value_type const                    &const_reference;
    /// The difference type
    typedef ptrdiff_t                           difference_type;
    /// The size type
    typedef ss_size_t                           size_type;
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
        typedef null_allocator<U>             other;
    };
#endif /* STLSOFT_CF_ALLOCATOR_REBIND_SUPPORT */

// Construction
public:
    /// Default constructor
    null_allocator() stlsoft_throw_0()
    {}
    /// Copy constructor
#ifndef __STLSOFT_CF_MEMBER_TEMPLATE_FUNCTION_SUPPORT
    null_allocator(const null_allocator &/* rhs */) stlsoft_throw_0()
    {}
#else
    template <ss_typename_param_k U>
    null_allocator(const null_allocator<U> &/* rhs */) stlsoft_throw_0()
    {}
#endif /* !__STLSOFT_CF_MEMBER_TEMPLATE_FUNCTION_SUPPORT */
    /// Destructor
    ~null_allocator() stlsoft_throw_0()
    {}

// Attributes
public:
    /// Returns the maximum number of allocatable entities
    size_type max_size() const stlsoft_throw_0()
    {
        return static_cast<size_type>(-1) / sizeof(value_type);
    }

// Conversion
public:
    /// Returns the address corresponding to the given reference
    ///
    /// \param x A reference to a \c value_type instance whose address will be calculated
    pointer address(reference x) const stlsoft_throw_0()
    {
        return &x;
    }
    /// Returns the address corresponding to the given non-mutable (const) reference
    ///
    /// \param x A non-mutable (const) reference to a \c value_type instance whose address will be calculated
    const_pointer address(const_reference x) const stlsoft_throw_0()
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
    pointer allocate(size_type n, null_allocator<void>::const_pointer pv = NULL)
    {
        STLSOFT_SUPPRESS_UNUSED(n);
        STLSOFT_SUPPRESS_UNUSED(pv);

        null_allocator<void>::pointer p   =   NULL;

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
        return sap_cast<char*>(allocate(n, NULL));
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
        STLSOFT_SUPPRESS_UNUSED(p);
        STLSOFT_SUPPRESS_UNUSED(n);
    }

    /// Deallocates a pointer
    ///
    /// \param p The memory block to deallocate
    void deallocate(pointer p)
    {
        STLSOFT_SUPPRESS_UNUSED(p);
    }

// Operations
public:
    /// In-place constructs an instance of the \c value_type, with the given value
    ///
    /// \param p The location in which to construct the instance
    /// \param x The value with which to copy construct the instance
    void construct(pointer p, value_type const &x)
    {
        stlsoft_assert(p != NULL);

        new(p) value_type(x);
    }

    /// In-place constructs an instance of the \c value_type
    ///
    /// \param p The location in which to construct the instance
    void construct(pointer p)
    {
        stlsoft_assert(p != NULL);

        new(p) value_type();
    }

    /// In-place destroys an instance
    ///
    /// \param p The instance whose destructor is to be called
    void destroy(pointer p) stlsoft_throw_0()
    {
        stlsoft_assert(p != NULL);

        stlsoft_destroy_instance(T, value_type, p);
    }

// Not to be implemented
private:
    class_type const &operator =(class_type const &rhs);
};

#ifndef __STLSOFT_DOCUMENTATION_SKIP_SECTION

template <ss_typename_param_k T>
inline ss_bool_t operator ==(const null_allocator<T> &/* lhs */, const null_allocator<T> &/* rhs */)
{
    return ss_true_v;
}

template <ss_typename_param_k T>
inline ss_bool_t operator !=(const null_allocator<T> &/* lhs */, const null_allocator<T> &/* rhs */)
{
    return ss_false_v;
}

#endif /* !__STLSOFT_DOCUMENTATION_SKIP_SECTION */

////////////////////////////////////////////////////////////////////////////////
// Unit-testing

#ifdef STLSOFT_UNITTEST

namespace unittest
{
    ss_bool_t test_stlsoft_null_allocator(unittest_reporter *r)
    {
        ss_bool_t               bSuccess    =   true;

        unittest_initialiser    init(r, "STLSoft", "null_allocator", __FILE__);

        typedef null_allocator<int>  int_allocator_t;

        int             i;
        int_allocator_t ator1;

        ator1.construct(&i, 1968);

        if(1968 != i)
        {
            r->report("failed to construct ", __LINE__);
            bSuccess = false;
        }

#ifdef __STLSOFT_CF_THROW_BAD_ALLOC
        try
        {
            ator1.allocate(1);

            r->report("null_allocator provided a non-NULL allocation ", __LINE__);
            bSuccess = false;
        }
        catch(std::bad_alloc &)
        {
        }
#else /* ? __STLSOFT_CF_THROW_BAD_ALLOC */
        if(ator1.allocate(1) != NULL)
        {
            r->report("null_allocator provided a non-NULL allocation ", __LINE__);
            bSuccess = false;
        }
#endif /* __STLSOFT_CF_THROW_BAD_ALLOC */

        return bSuccess;
    }

    unittest_registrar    unittest_stlsoft_null_allocator(test_stlsoft_null_allocator);

} // namespace unittest

#endif /* STLSOFT_UNITTEST */

/* ////////////////////////////////////////////////////////////////////////// */

/// @} // end of group stlsoft_allocator_library

/* ////////////////////////////////////////////////////////////////////////// */

#ifndef _STLSOFT_NO_NAMESPACE
} // namespace stlsoft
#endif /* _STLSOFT_NO_NAMESPACE */

/* ////////////////////////////////////////////////////////////////////////// */

#endif /* !STLSOFT_INCL_H_STLSOFT_NULL_ALLOCATOR */

/* ////////////////////////////////////////////////////////////////////////// */
