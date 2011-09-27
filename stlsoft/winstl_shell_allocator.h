/* ////////////////////////////////////////////////////////////////////////////
 * File:        winstl_shell_allocator.h
 *
 * Purpose:     shell_allocator class.
 *
 * Created:     2nd March 2002
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


/// \file winstl_shell_allocator.h
///
/// shell_allocator class.

#ifndef WINSTL_INCL_H_WINSTL_SHELL_ALLOCATOR
#define WINSTL_INCL_H_WINSTL_SHELL_ALLOCATOR

#ifndef __STLSOFT_DOCUMENTATION_SKIP_SECTION
# define WINSTL_VER_H_WINSTL_SHELL_ALLOCATOR_MAJOR       2
# define WINSTL_VER_H_WINSTL_SHELL_ALLOCATOR_MINOR       0
# define WINSTL_VER_H_WINSTL_SHELL_ALLOCATOR_REVISION    1
# define WINSTL_VER_H_WINSTL_SHELL_ALLOCATOR_EDIT        44
#endif /* !__STLSOFT_DOCUMENTATION_SKIP_SECTION */

/* ////////////////////////////////////////////////////////////////////////////
 * Includes
 */

#ifndef WINSTL_INCL_H_WINSTL
# include "winstl.h"                    // Include the WinSTL root header
#endif /* !WINSTL_INCL_H_WINSTL */
#ifndef STLSOFT_INCL_H_STLSOFT_ALLOCATOR_BASE
# include "stlsoft_allocator_base.h"    // Include the STLSoft root header
#endif /* !STLSOFT_INCL_H_STLSOFT_ALLOCATOR_BASE */
#ifndef STLSOFT_INCL_H_STLSOFT_SAP_CAST
# include "stlsoft_sap_cast.h"
#endif /* !STLSOFT_INCL_H_STLSOFT_SAP_CAST */
#include <stdexcept>                    // runtime_error
#include <shlobj.h>                     // SHGetMalloc

/* ////////////////////////////////////////////////////////////////////////////
 * Namespace
 */

#ifndef _WINSTL_NO_NAMESPACE
# if defined(_STLSOFT_NO_NAMESPACE) || \
     defined(__STLSOFT_DOCUMENTATION_SKIP_SECTION)
/* There is no stlsoft namespace, so must define ::winstl */
namespace winstl
{
# else
/* Define stlsoft::winstl_project */

namespace stlsoft
{

namespace winstl_project
{

# endif /* _STLSOFT_NO_NAMESPACE */
#endif /* !_WINSTL_NO_NAMESPACE */

/* ////////////////////////////////////////////////////////////////////////// */

/// \weakgroup libraries STLSoft Libraries
/// \brief The individual libraries

/// \weakgroup libraries_allocator Allocator Library
/// \ingroup libraries
/// \brief This library provides STL-compatible <code><b>allocator</b></code> types

/// \weakgroup winstl_allocator_library Allocator Library (WinSTL)
/// \ingroup WinSTL libraries_allocator
/// \brief This library provides STL-compatible <code><b>allocator</b></code> types for the Win32 API
/// @{

/* /////////////////////////////////////////////////////////////////////////////
 * Classes
 */

#ifndef __STLSOFT_DOCUMENTATION_SKIP_SECTION

template <ss_typename_param_k T>
class shell_allocator;

// Specialisation for void
STLSOFT_TEMPLATE_SPECIALISATION
class shell_allocator<void>
{
public:
    typedef void                            value_type;
    typedef shell_allocator<void>           class_type;
    typedef void                            *pointer;
    typedef void const                      *const_pointer;
    typedef ptrdiff_t                       difference_type;
    typedef ws_size_t                       size_type;

#ifdef STLSOFT_CF_ALLOCATOR_REBIND_SUPPORT
    /// The allocator <b><code>rebind</code></b> structure
    template <ss_typename_param_k U>
    struct rebind
    {
        typedef shell_allocator<U>          other;
    };
#endif /* STLSOFT_CF_ALLOCATOR_REBIND_SUPPORT */
};

#endif /* !__STLSOFT_DOCUMENTATION_SKIP_SECTION */

/// STL Allocator based on the Win32 shell allocator
///
/// \param T The value_type of the allocator
template <ss_typename_param_k T>
class shell_allocator
{
public:
    /// The value type
    typedef T                               value_type;
    /// The type of the current parameterisation
    typedef shell_allocator<value_type>     class_type;
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
    typedef ws_size_t                       size_type;
    /// The type used in deallocate()
#ifdef __STLSOFT_CF_ALLOCATOR_TYPED_DEALLOCATE_POINTER
    typedef pointer                         deallocate_pointer;
#else
    typedef void                            *deallocate_pointer;
#endif /* __STLSOFT_COMPILER_IS_MSVC && _MSC_VER == 1200 */

#ifdef STLSOFT_CF_ALLOCATOR_REBIND_SUPPORT
    /// The allocator <b><code>rebind</code></b> structure
    template <ss_typename_param_k U>
    struct rebind
    {
        typedef shell_allocator<U>          other;
    };
#endif /* STLSOFT_CF_ALLOCATOR_REBIND_SUPPORT */

// Construction
public:
    /// Default constructor
    shell_allocator() winstl_throw_1(stlsoft_ns_qual_std(runtime_error) )
        : m_malloc(NULL)
    {
        _get_malloc();
    }
    /// Copy constructor
#ifndef __STLSOFT_CF_MEMBER_TEMPLATE_FUNCTION_SUPPORT
    shell_allocator(const shell_allocator &/* rhs */) //winstl_throw_1(stlsoft_ns_qual_std(runtime_error) )
    {
        _get_malloc();
    }
#else
    template <ss_typename_param_k U>
    shell_allocator(const shell_allocator<U> &/* rhs */) //winstl_throw_1(stlsoft_ns_qual_std(runtime_error) )
    {
        _get_malloc();
    }
#endif /* !__STLSOFT_CF_MEMBER_TEMPLATE_FUNCTION_SUPPORT */
    /// Destructor
    ~shell_allocator() //winstl_throw_0()
    {
        if(m_malloc != NULL)
        {
            m_malloc->Release();
        }
    }

// Attributes
public:
    /// Returns the maximum number of allocatable entities
    size_type max_size() const winstl_throw_0()
    {
        return static_cast<size_type>(-1) / sizeof(value_type);
    }

// Conversion
public:
    /// Returns the address corresponding to the given reference
    ///
    /// \param x A reference to a \c value_type instance whose address will be calculated
    pointer address(reference x) const winstl_throw_0()
    {
        return &x;
    }
    /// Returns the address corresponding to the given non-mutable (const) reference
    ///
    /// \param x A non-mutable (const) reference to a \c value_type instance whose address will be calculated
    const_pointer address(const_reference x) const winstl_throw_0()
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
    pointer allocate(size_type n, shell_allocator<void>::const_pointer pv = NULL)
    {
        STLSOFT_SUPPRESS_UNUSED(pv);

        shell_allocator<void>::pointer   p   =   static_cast<shell_allocator<void>::pointer>(m_malloc->Alloc(n * sizeof(value_type)));

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
    void deallocate(pointer p, size_type n)
    {
        STLSOFT_SUPPRESS_UNUSED(n);

        m_malloc->Free(static_cast<HGLOBAL>(p));
    }

    /// Deallocates a pointer
    ///
    /// \param p The memory block to deallocate
    void deallocate(pointer p)
    {
        m_malloc->Free(static_cast<HGLOBAL>(p));
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
    void destroy(pointer p) winstl_throw_0()
    {
        winstl_destroy_instance(T, value_type, p);
    }

// Implementation
protected:
    void _get_malloc() winstl_throw_1(stlsoft_ns_qual_std(runtime_error) )
    {
        HRESULT hr  =   ::SHGetMalloc(&m_malloc);

#ifdef __STLSOFT_CF_THROW_BAD_ALLOC
        if(FAILED(hr))
        {
            throw stlsoft_ns_qual_std(runtime_error)("failed to retrieve the shell allocator");
        }
#else
        STLSOFT_SUPPRESS_UNUSED(hr);
#endif /* __STLSOFT_CF_THROW_BAD_ALLOC */
    }

// Members
protected:
    LPMALLOC    m_malloc;

// Not to be implemented
private:
    class_type const &operator =(class_type const &rhs);
};

#ifndef __STLSOFT_DOCUMENTATION_SKIP_SECTION

template <ss_typename_param_k T>
inline ws_bool_t operator ==(const shell_allocator<T> &/* lhs */, const shell_allocator<T> &/* rhs */)
{
    return ws_true_v;
}

template <ss_typename_param_k T>
inline ws_bool_t operator !=(const shell_allocator<T> &/* lhs */, const shell_allocator<T> &/* rhs */)
{
    return ws_false_v;
}

#endif /* !__STLSOFT_DOCUMENTATION_SKIP_SECTION */

////////////////////////////////////////////////////////////////////////////////
// Unit-testing

#ifdef STLSOFT_UNITTEST

namespace unittest
{
    ss_bool_t test_winstl_shell_allocator(unittest_reporter *r)
    {
        using stlsoft::unittest::unittest_initialiser;

        ss_bool_t               bSuccess    =   true;

        unittest_initialiser    init(r, "WinSTL", "shell_allocator", __FILE__);

        typedef shell_allocator<int>  int_allocator_t;

        int_allocator_t ator1;

        int     *pi1    =   ator1.allocate(100);

        if(NULL != pi1)
        {
            ator1.construct(pi1, 1968);

            if(1968 != *pi1)
            {
                r->report("construct() failed ", __LINE__);
                bSuccess = false;
            }
        }

        return bSuccess;
    }

    unittest_registrar    unittest_winstl_shell_allocator(test_winstl_shell_allocator);

} // namespace unittest

#endif /* STLSOFT_UNITTEST */

/* ////////////////////////////////////////////////////////////////////////// */

/// @} // end of group winstl_allocator_library

/* ////////////////////////////////////////////////////////////////////////// */

#ifndef _WINSTL_NO_NAMESPACE
# if defined(_STLSOFT_NO_NAMESPACE) || \
     defined(__STLSOFT_DOCUMENTATION_SKIP_SECTION)
} // namespace winstl
# else
} // namespace winstl_project
} // namespace stlsoft
# endif /* _STLSOFT_NO_NAMESPACE */
#endif /* !_WINSTL_NO_NAMESPACE */

/* ////////////////////////////////////////////////////////////////////////// */

#endif /* WINSTL_INCL_H_WINSTL_SHELL_ALLOCATOR */

/* ////////////////////////////////////////////////////////////////////////// */
