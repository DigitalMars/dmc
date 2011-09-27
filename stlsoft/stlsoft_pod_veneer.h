/* /////////////////////////////////////////////////////////////////////////////
 * File:        stlsoft_pod_veneer.h
 *
 * Purpose:     Contains the pod_veneer template class.
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


/// \file stlsoft_pod_veneer.h
///
/// Contains the pod_veneer template class.

#ifndef STLSOFT_INCL_H_STLSOFT_POD_VENEER
#define STLSOFT_INCL_H_STLSOFT_POD_VENEER

#ifndef __STLSOFT_DOCUMENTATION_SKIP_SECTION
# define STLSOFT_VER_H_STLSOFT_POD_VENEER_MAJOR       2
# define STLSOFT_VER_H_STLSOFT_POD_VENEER_MINOR       0
# define STLSOFT_VER_H_STLSOFT_POD_VENEER_REVISION    1
# define STLSOFT_VER_H_STLSOFT_POD_VENEER_EDIT        30
#endif /* !__STLSOFT_DOCUMENTATION_SKIP_SECTION */

/* /////////////////////////////////////////////////////////////////////////////
 * Includes
 */

#ifndef STLSOFT_INCL_H_STLSOFT
# include "stlsoft.h"                // Include the STLSoft root header
#endif /* !STLSOFT_INCL_H_STLSOFT */
#ifndef STLSOFT_INCL_H_STLSOFT_CONSTRAINTS
# include "stlsoft_constraints.h"    // constraints
#endif /* !STLSOFT_INCL_H_STLSOFT_CONSTRAINTS */

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

// class pod_veneer

/// Bolts construction and/or destruction to Plain Old Data types
///
/// The parameterisation of this <a href = "http://synesis.com.au/resources/articles/cpp/veneers.pdf">veneer</a>
/// can be used to apply RRID or RAII semantics to a plain old data type. For
/// example,
///
/// &nbsp;&nbsp;<code>struct Simple</code><br>
/// &nbsp;&nbsp;<code>{</code><br>
/// &nbsp;&nbsp;<code>public:</code><br>
/// &nbsp;&nbsp;<code>&nbsp;&nbsp;char&nbsp;*buffer;</code><br>
/// &nbsp;&nbsp;<code>};</code><br>
/// &nbsp;&nbsp;<code></code><br>
/// &nbsp;&nbsp;<code>void Init_Simple(Simple *simple);</code><br>
/// &nbsp;&nbsp;<code>void Uninit_Simple(Simple *simple);</code><br>
/// &nbsp;&nbsp;<code></code><br>
/// &nbsp;&nbsp;<code>struct Simple_Init { void operator ()(Simple *simple) { Init_Simple(simple); } };</code><br>
/// &nbsp;&nbsp;<code>struct Simple_Uninit { void operator ()(Simple *simple) { Uninit_Simple(simple); } };</code><br>
/// &nbsp;&nbsp;<code></code><br>
/// &nbsp;&nbsp;<code>void fn()</code><br>
/// &nbsp;&nbsp;<code>{</code><br>
/// &nbsp;&nbsp;<code>&nbsp;&nbsp;typedef&nbsp;&nbsp;&nbsp;pod_veneer<Simple, Simple_Init, Simple_Uninit>&nbsp;&nbsp;Simple_raii_t;</code><br>
/// &nbsp;&nbsp;<code></code><br>
/// &nbsp;&nbsp;<code>&nbsp;&nbsp;Simple_raii_t&nbsp;simple;&nbsp;// Init_Simple() called here</code><br>
/// &nbsp;&nbsp;<code></code><br>
/// &nbsp;&nbsp;<code>&nbsp;&nbsp;. . .</code><br>
/// &nbsp;&nbsp;<code></code><br>
/// &nbsp;&nbsp;<code>} // Uninit_Simple() called here</code><br>
///
/// \param T The POD type
/// \param CF The function object type applied during construction
/// \param DF The function object type applied during destruction

template<   ss_typename_param_k T
        ,   ss_typename_param_k CF
        ,   ss_typename_param_k DF
        >
class pod_veneer
    : public T
{
public:
    /// The value type
    typedef T                       value_type;
    /// The Plain Old Data type
    typedef T                       pod_type;
    /// The constructor function object type
    typedef CF                      constructor_function_type;
    /// The destructor function object type
    typedef DF                      destructor_function_type;
    /// The type of the current parameterisation
    typedef pod_veneer<T, CF, DF>   class_type;

public:
    /// The constructor, within which CF is applied to \c this
    pod_veneer()
    {
        // It is entirely appropriate to constrain instantiations of this class
        // to default construction only, since the whole raison d'etre of this
        // template is to bring RAII semantics to POD types.

        // Construct the pod
        constructor_function_type()(static_cast<pod_type *>(this));
    }

    /// The destructor, within which DF is applied to \c this
    ~pod_veneer() stlsoft_throw_0()
    {
        // There are four important points to note about the destructor
        //
        // 1. It does not throw an exception. Therefore, the destroying type
        // must not throw an exception in its function call operator
        // 2. It uses the must_be_pod constraint to ensure that the pod_type is
        // a POD type
        // 3. It uses the must_be_same_size constraint to ensure that the
        // generated class is the same size as the pod_type, so that arrays of
        // the generated class can be used where arrays of the pod_type could be
        // used. The runtime assert is included for those compilers that do not
        // implement compile-time asserts.
        // 4. It is non-virtual, since it should not introduce a vtable (which
        // would change the size of the generated class)

        // 2. Must be POD
        stlsoft_constraint_must_be_pod(pod_type);

        // 3. Must use EDO
        stlsoft_constraint_must_be_same_size(class_type, pod_type);
#if defined(__STLSOFT_COMPILER_IS_WATCOM)
        stlsoft_assert(sizeof(class_type) == sizeof(pod_type));
#else
        stlsoft_message_assert("pod_veneer used for inappropriate type", sizeof(class_type) == sizeof(pod_type));
#endif /* !__STLSOFT_COMPILER_IS_WATCOM */

        // Destroy the pod
        destructor_function_type()(static_cast<pod_type *>(this));
    }
};

/* ////////////////////////////////////////////////////////////////////////// */

#ifndef _STLSOFT_NO_NAMESPACE
} // namespace stlsoft
#endif /* _STLSOFT_NO_NAMESPACE */

/* ////////////////////////////////////////////////////////////////////////// */

#endif /* !STLSOFT_INCL_H_STLSOFT_POD_VENEER */

/* ////////////////////////////////////////////////////////////////////////// */
