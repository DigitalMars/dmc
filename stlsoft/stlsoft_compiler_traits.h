/* /////////////////////////////////////////////////////////////////////////////
 * File:        stlsoft_compiler_traits.h
 *
 * Purpose:     compiler_traits classes.
 *
 * Created:     15th November 2003
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


/// \file stlsoft_compiler_traits.h
///
/// compiler_traits classes.

#ifndef STLSOFT_INCL_H_STLSOFT_COMPILER_TRAITS
#define STLSOFT_INCL_H_STLSOFT_COMPILER_TRAITS

#ifndef __STLSOFT_DOCUMENTATION_SKIP_SECTION
# define STLSOFT_VER_H_STLSOFT_COMPILER_TRAITS_MAJOR        2
# define STLSOFT_VER_H_STLSOFT_COMPILER_TRAITS_MINOR        0
# define STLSOFT_VER_H_STLSOFT_COMPILER_TRAITS_REVISION     1
# define STLSOFT_VER_H_STLSOFT_COMPILER_TRAITS_EDIT         8
#endif /* !__STLSOFT_DOCUMENTATION_SKIP_SECTION */

/* /////////////////////////////////////////////////////////////////////////////
 * Includes
 */

#ifndef STLSOFT_INCL_H_STLSOFT
# include "stlsoft.h"               // Include the STLSoft root header
#endif /* !STLSOFT_INCL_H_STLSOFT */
#ifndef STLSOFT_INCL_H_STLSOFT_SIZE_TRAITS
# include "stlsoft_size_traits.h"   // int_size_traits
#endif /* !STLSOFT_INCL_H_STLSOFT_SIZE_TRAITS */

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

#ifndef __STLSOFT_DOCUMENTATION_SKIP_SECTION
/// The base class, and the comparand for empty cases
struct compiler_traits_ebo_parent
{};

/* EBO - Empty Base Optimisation
 */

/// The result of Case 1, and the intermediate parent for Cases 3 & 4.
///
///       compiler_traits_ebo_parent
///                |
///                V
///     compiler_traits_ebo_thin_child
struct compiler_traits_ebo_thin_child
    : compiler_traits_ebo_parent
{};

/// The result of Case 2
///
///       compiler_traits_ebo_parent
///                |
///                V
///     compiler_traits_ebo_thick_child
struct compiler_traits_ebo_thick_child
    : compiler_traits_ebo_parent
{
    int i;
};

/// Used in Cases 2, 4, 6 & 8
struct compiler_traits_ebo_thick_peer
{
    int i;
};

/// The result of Case 3
///
///       compiler_traits_ebo_parent
///                |
///                V
///     compiler_traits_ebo_thin_child
///                |
///                V
///   compiler_traits_ebo_thin_grandchild
struct compiler_traits_ebo_thin_grandchild
    : compiler_traits_ebo_thin_child
{};

/// The result of Case 4
///
///       compiler_traits_ebo_parent
///                |
///                V
///     compiler_traits_ebo_thin_child
///                |
///                V
///   compiler_traits_ebo_thick_grandchild
struct compiler_traits_ebo_thick_grandchild
    : compiler_traits_ebo_thin_child
{
    int i;
};

/// Used in Case 5
struct compiler_traits_ebo_parent2
{};

/// The result of Case 5
///
///   compiler_traits_ebo_parent      compiler_traits_ebo_parent2
///                |                        |
///                V                        V
///            compiler_traits_ebo_mi_thin_child
struct compiler_traits_ebo_mi_thin_child
    : compiler_traits_ebo_parent, compiler_traits_ebo_parent2
{};

/// The result of Case 6
///
///   compiler_traits_ebo_parent      compiler_traits_ebo_parent2
///                |                        |
///                V                        V
///            compiler_traits_ebo_mi_thick_child
struct compiler_traits_ebo_mi_thick_child
    : compiler_traits_ebo_parent, compiler_traits_ebo_parent2
{
    int i;
};

/// The result of Case 7
///
///   compiler_traits_ebo_parent      compiler_traits_ebo_thick_peer
///                |                        |
///                V                        V
///            compiler_traits_ebo_mi_mixin_child
struct compiler_traits_ebo_mi_mixin_child
    : compiler_traits_ebo_parent, compiler_traits_ebo_thick_peer
{};


/* EDO - Empty Derived Optimisation
 */

/// Used in Case 1
struct compiler_traits_edo_thin_base
{};

/// Used in Case 2
struct compiler_traits_edo_thick_base
{
    int i;
};

/// Used in Case 3
struct compiler_traits_edo_thin_base2
{};

/// Used in Cases 3 & 4
template <ss_typename_param_k T>
struct compiler_traits_edo_child
    : T
{};

/// Result of case 3
///
///       compiler_traits_edo_thick_base
///                    |
///                    V
///    compiler_traits_edo_child_of_thick_base
struct compiler_traits_edo_child_of_thick_base
    : compiler_traits_edo_thick_base
{};


/// Used in Cases 7 & 8
template<   ss_typename_param_k T1
        ,   ss_typename_param_k T2
        >
struct compiler_traits_edo_mi_child
    : T1, T2
{};

#endif /* !__STLSOFT_DOCUMENTATION_SKIP_SECTION */

/// traits class for (current) compiler
struct compiler_traits
{
public:
    // Empty Base Optimisation (EBO)
    //
    /// 1. With empty base and empty child
    enum { supportsEBO1 = sizeof(compiler_traits_ebo_thin_child) == sizeof(compiler_traits_ebo_parent) };
    /// 2. With empty base and non-empty child
    enum { supportsEBO2 = sizeof(compiler_traits_ebo_thick_child) == sizeof(compiler_traits_ebo_thick_peer) };
    /// 3. With empty base, empty intermediate, and empty child
    enum { supportsEBO3 = sizeof(compiler_traits_ebo_thin_grandchild) == sizeof(compiler_traits_ebo_parent) };
    /// 4. With empty base, empty intermediate, and non-empty child
    enum { supportsEBO4 = sizeof(compiler_traits_ebo_thick_grandchild) == sizeof(compiler_traits_ebo_thick_peer) };
    /// 5. With two empty bases and empty child
    enum { supportsEBO5 = sizeof(compiler_traits_ebo_mi_thin_child) == sizeof(compiler_traits_ebo_parent) };
    /// 6. With two empty bases and non-empty child
    enum { supportsEBO6 = sizeof(compiler_traits_ebo_mi_thick_child) == sizeof(compiler_traits_ebo_thick_peer) };
    /// 7. With one empty base (the mixin), one non-empty base and empty child
    enum { supportsEBO7 = sizeof(compiler_traits_ebo_mi_mixin_child) == sizeof(compiler_traits_ebo_thick_peer) };

    enum { supportsEBO = supportsEBO1 & supportsEBO2 & supportsEBO3 & supportsEBO4 };
    enum { supportsMIEBO = supportsEBO5 & supportsEBO6 & supportsEBO7 /* & supportsEBO8 */ };
    enum { supportsExtendedEBO = supportsEBO & supportsMIEBO };

    // Empty Derived Optimisation (EDO)
    //
    /// 1. With empty base (same as EBO1)
    enum { supportsEDO1 = supportsEBO1 };
    /// 2. With empty base (same as EBO1); child is template
    enum { supportsEDO2 = sizeof(compiler_traits_edo_child<compiler_traits_edo_thin_base>) == sizeof(compiler_traits_edo_thin_base) };
    /// 3. With non-empty base
    enum { supportsEDO3 = sizeof(compiler_traits_edo_child_of_thick_base) == sizeof(compiler_traits_edo_thick_base) };
    /// 4. With non-empty base; child is template
    enum { supportsEDO4 = sizeof(compiler_traits_edo_child<compiler_traits_edo_thick_base>) == sizeof(compiler_traits_edo_thick_base) };

    /// 5. With two empty bases
    enum { supportsEDO5 = supportsEBO5 };
    /// 6. With two empty bases; child is template
    enum { supportsEDO6 = sizeof(compiler_traits_edo_mi_child<compiler_traits_edo_thin_base, compiler_traits_edo_thin_base2>) == sizeof(compiler_traits_edo_thin_base) };
    /// 7. With one empty base (the mixin), one non-empty base
    enum { supportsEDO7 = supportsEBO7 };
    /// 8. With one empty base (the mixin), one non-empty base; child is template
    enum { supportsEDO8 = sizeof(compiler_traits_edo_mi_child<compiler_traits_edo_thick_base, compiler_traits_edo_thin_base>) == sizeof(compiler_traits_edo_thick_base) };

    enum { supportsEDO = supportsEDO1 & supportsEDO2 & supportsEDO3 & supportsEDO4 };
    enum { supportsMIEDO = supportsEDO5 & supportsEDO6 & supportsEDO7 & supportsEDO8 };
    enum { supportsExtendedEDO = supportsEDO & supportsMIEDO };
};


/* ////////////////////////////////////////////////////////////////////////// */

#ifndef _STLSOFT_NO_NAMESPACE
} // namespace stlsoft
#endif /* _STLSOFT_NO_NAMESPACE */

/* ////////////////////////////////////////////////////////////////////////// */

#endif /* !STLSOFT_INCL_H_STLSOFT_COMPILER_TRAITS */

/* ////////////////////////////////////////////////////////////////////////// */
