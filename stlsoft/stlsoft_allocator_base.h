/* ////////////////////////////////////////////////////////////////////////////
 * File:        stlsoft_allocator_base.h
 *
 * Purpose:     Allocator commmon features.
 *
 * Created:     20th August 2003
 * Updated:     11th September 2004
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


/// \file stlsoft_allocator_base.h
///
/// Allocator commmon features.

#ifndef STLSOFT_INCL_H_STLSOFT_ALLOCATOR_BASE
#define STLSOFT_INCL_H_STLSOFT_ALLOCATOR_BASE

#ifndef __STLSOFT_DOCUMENTATION_SKIP_SECTION
# define STLSOFT_VER_H_STLSOFT_ALLOCATOR_BASE_MAJOR     2
# define STLSOFT_VER_H_STLSOFT_ALLOCATOR_BASE_MINOR     0
# define STLSOFT_VER_H_STLSOFT_ALLOCATOR_BASE_REVISION  1
# define STLSOFT_VER_H_STLSOFT_ALLOCATOR_BASE_EDIT      8
#endif /* !__STLSOFT_DOCUMENTATION_SKIP_SECTION */

/* ////////////////////////////////////////////////////////////////////////////
 * Includes
 */

#ifndef STLSOFT_INCL_H_STLSOFT
# include "stlsoft.h"   // Include the STLSoft root header
#endif /* !STLSOFT_INCL_H_STLSOFT */
#if defined(__STLSOFT_CF_THROW_BAD_ALLOC) || \
    (   defined(__STLSOFT_COMPILER_IS_MSVC) && \
        _MSC_VER < 1100)
# include <new>         // placement new, std::bad_alloc
#endif /* __STLSOFT_CF_THROW_BAD_ALLOC || _MSC_VER < 1100) */

/* ////////////////////////////////////////////////////////////////////////////
 * Allocator compatibilities
 */

/* stl-like allocator classes need allocate and deallocate as static
 *
 * Discriminated symbol is __STLSOFT_CF_ALLOCATOR_STATIC_ALLOCATE_METHODS
 *
 * Note: this should be resolving on the library, not the compiler
 */
#ifdef __STLSOFT_CF_ALLOCATOR_STATIC_ALLOCATE_METHODS
# undef __STLSOFT_CF_ALLOCATOR_STATIC_ALLOCATE_METHODS
#endif /* __STLSOFT_CF_ALLOCATOR_STATIC_ALLOCATE_METHODS */

/* stl-like allocator classes need deallocate to use void *
 *
 * Discriminated symbol is __STLSOFT_CF_ALLOCATOR_TYPED_DEALLOCATE_POINTER
 *
 * Note: this should be resolving on the library, not the compiler
 */
#define __STLSOFT_CF_ALLOCATOR_TYPED_DEALLOCATE_POINTER /* This is standard behaviour */

/* stl-like allocator classes need a _CharAlloc method
 *
 * Discriminated symbol is __STLSOFT_CF_ALLOCATOR_CHARALLOC_METHOD
 *
 * Note: this should be resolving on the library, not the compiler
 */
#ifdef __STLSOFT_CF_ALLOCATOR_CHARALLOC_METHOD
# undef __STLSOFT_CF_ALLOCATOR_CHARALLOC_METHOD
#endif /* __STLSOFT_CF_ALLOCATOR_CHARALLOC_METHOD */


#if defined(__STLSOFT_COMPILER_IS_DMC)
# define __STLSOFT_CF_ALLOCATOR_STATIC_ALLOCATE_METHODS
# undef __STLSOFT_CF_ALLOCATOR_TYPED_DEALLOCATE_POINTER
#elif defined(__STLSOFT_COMPILER_IS_MWERKS)
# undef __STLSOFT_CF_ALLOCATOR_TYPED_DEALLOCATE_POINTER
#elif ( defined(__STLSOFT_COMPILER_IS_INTEL) || \
        defined(__STLSOFT_COMPILER_IS_MSVC)) && \
    _MSC_VER < 1300
# undef __STLSOFT_CF_ALLOCATOR_TYPED_DEALLOCATE_POINTER
# define __STLSOFT_CF_ALLOCATOR_CHARALLOC_METHOD
#endif /* compiler */

#if defined(__STLSOFT_DOCUMENTATION_SKIP_SECTION)
# ifdef __STLSOFT_CF_ALLOCATOR_CHARALLOC_METHOD
#  undef __STLSOFT_CF_ALLOCATOR_CHARALLOC_METHOD
# endif /* __STLSOFT_CF_ALLOCATOR_CHARALLOC_METHOD */
#endif /* !__STLSOFT_DOCUMENTATION_SKIP_SECTION */


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


/* ////////////////////////////////////////////////////////////////////////// */

/// @} // end of group stlsoft_allocator_library

/* ////////////////////////////////////////////////////////////////////////// */

#ifndef _STLSOFT_NO_NAMESPACE
} // namespace stlsoft
#endif /* _STLSOFT_NO_NAMESPACE */

/* ////////////////////////////////////////////////////////////////////////// */

#endif /* !STLSOFT_INCL_H_STLSOFT_ALLOCATOR_BASE */

/* ////////////////////////////////////////////////////////////////////////// */
