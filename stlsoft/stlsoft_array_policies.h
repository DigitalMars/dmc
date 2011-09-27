/* /////////////////////////////////////////////////////////////////////////////
 * File:        stlsoft_array_policies.h
 *
 * Purpose:     Contains the construction policies for the array (fixed and
 *              frame) classes.
 *
 * Created:     1st September 2002
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


/// \file stlsoft_array_policies.h
///
/// Contains the construction policies for the array (fixed and frame) classes.

#ifndef STLSOFT_INCL_H_STLSOFT_ARRAY_POLICIES
#define STLSOFT_INCL_H_STLSOFT_ARRAY_POLICIES

#ifndef __STLSOFT_DOCUMENTATION_SKIP_SECTION
# define STLSOFT_VER_H_STLSOFT_ARRAY_POLICIES_MAJOR     2
# define STLSOFT_VER_H_STLSOFT_ARRAY_POLICIES_MINOR     0
# define STLSOFT_VER_H_STLSOFT_ARRAY_POLICIES_REVISION  1
# define STLSOFT_VER_H_STLSOFT_ARRAY_POLICIES_EDIT      107
#endif /* !__STLSOFT_DOCUMENTATION_SKIP_SECTION */

/* /////////////////////////////////////////////////////////////////////////////
 * Includes
 */

#ifndef STLSOFT_INCL_H_STLSOFT
# include "stlsoft.h"   // Include the STLSoft root header
#endif /* !STLSOFT_INCL_H_STLSOFT */

/* /////////////////////////////////////////////////////////////////////////////
 * Namespace
 */

#ifndef _STLSOFT_NO_NAMESPACE
namespace stlsoft
{
#endif /* _STLSOFT_NO_NAMESPACE */

/* /////////////////////////////////////////////////////////////////////////////
 * Construction policy classes
 */

/// Traits class that controls whether elements in STLSoft arrays are in-place constructed and destroyed
///
/// Defaults to true, indicating that construction and destruction will be
/// performed, but is false for all supported integral and boolean types
///
/// \param T The type
template <ss_typename_param_k T>
struct do_construction
{
    enum
    {
        value = true //!< If \c true the array elements are constructed, otherwise they are not
    };
};

#ifndef __STLSOFT_DOCUMENTATION_SKIP_SECTION

STLSOFT_TEMPLATE_SPECIALISATION
struct do_construction<ss_sint8_t>  { enum { value = false }; };

STLSOFT_TEMPLATE_SPECIALISATION
struct do_construction<ss_uint8_t>  { enum { value = false }; };

STLSOFT_TEMPLATE_SPECIALISATION
struct do_construction<ss_sint16_t> { enum { value = false }; };

STLSOFT_TEMPLATE_SPECIALISATION
struct do_construction<ss_uint16_t> { enum { value = false }; };

STLSOFT_TEMPLATE_SPECIALISATION
struct do_construction<ss_sint32_t> { enum { value = false }; };

STLSOFT_TEMPLATE_SPECIALISATION
struct do_construction<ss_uint32_t> { enum { value = false }; };

#ifdef STLSOFT_CF_64BIT_INT_SUPPORT
STLSOFT_TEMPLATE_SPECIALISATION
struct do_construction<ss_sint64_t> { enum { value = false }; };

STLSOFT_TEMPLATE_SPECIALISATION
struct do_construction<ss_uint64_t> { enum { value = false }; };
#endif /* STLSOFT_CF_64BIT_INT_SUPPORT */

STLSOFT_TEMPLATE_SPECIALISATION
struct do_construction<float>       { enum { value = false }; };

STLSOFT_TEMPLATE_SPECIALISATION
struct do_construction<double>      { enum { value = false }; };

STLSOFT_TEMPLATE_SPECIALISATION
struct do_construction<long double> { enum { value = false }; };

#ifdef __STLSOFT_CF_NATIVE_BOOL_SUPPORT
STLSOFT_TEMPLATE_SPECIALISATION
struct do_construction<ss_bool_t>   { enum { value = false }; };
#endif /* __STLSOFT_CF_NATIVE_BOOL_SUPPORT */

#endif /* !__STLSOFT_DOCUMENTATION_SKIP_SECTION */


/// Stipulates that array elements are always constructed
struct do_construction_always
{
    enum { value = true };
};

/// Stipulates that array elements are never constructed
struct do_construction_never
{
    enum { value = false };
};

/*
template<   ss_typename_param_k T
        ,   ss_typename_param_k P
struct inplace_range_construct
{
public:
    void
}
 */

#ifndef __STLSOFT_DOCUMENTATION_SKIP_SECTION

template<   ss_typename_param_k T
        ,   ss_typename_param_k A
        ,   ss_typename_param_k P
        >
struct array_range_initialiser
{
    typedef T   value_type;
    typedef T   *pointer;
    typedef A   allocator_type;
    typedef P   initialisation_policy_type;

public:
    static void construct(allocator_type &ator, pointer p, ss_size_t n)
    {
        if(initialisation_policy_type::value)
        {
            construct(ator, p, n, value_type());
        }
        else
        {
#if 1
            memset(p, 0, n * sizeof(value_type));
#else /* ? 0 */
            fill_n(p, n, 0);
#endif /* 0 */
        }
    }

    static void construct(allocator_type &ator, pointer p, ss_size_t n, value_type const &value)
    {
        for(pointer e = p + n; p != e; ++p)
        {
            ator.construct(p, value);
        }
    }

    static void destroy(allocator_type &ator, pointer p, ss_size_t n)
    {
        if(initialisation_policy_type::value)
        {
            for(pointer e = p + n; p != e; ++p)
            {
                ator.destroy(p);
            }
        }
    }
};

#endif /* !__STLSOFT_DOCUMENTATION_SKIP_SECTION */

/* ////////////////////////////////////////////////////////////////////////// */

#ifndef _STLSOFT_NO_NAMESPACE
} // namespace stlsoft
#endif /* _STLSOFT_NO_NAMESPACE */

/* ////////////////////////////////////////////////////////////////////////// */

#endif /* !STLSOFT_INCL_H_STLSOFT_ARRAY_POLICIES */

/* ////////////////////////////////////////////////////////////////////////// */
