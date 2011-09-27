/* /////////////////////////////////////////////////////////////////////////////
 * File:        stlsoft_size_traits.h
 *
 * Purpose:     size_traits classes.
 *
 * Created:     24th August 2003
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


/// \file stlsoft_size_traits.h
///
/// size_traits classes.

#ifndef STLSOFT_INCL_H_STLSOFT_SIZE_TRAITS
#define STLSOFT_INCL_H_STLSOFT_SIZE_TRAITS

#ifndef __STLSOFT_DOCUMENTATION_SKIP_SECTION
# define STLSOFT_VER_H_STLSOFT_SIZE_TRAITS_MAJOR        2
# define STLSOFT_VER_H_STLSOFT_SIZE_TRAITS_MINOR        0
# define STLSOFT_VER_H_STLSOFT_SIZE_TRAITS_REVISION     1
# define STLSOFT_VER_H_STLSOFT_SIZE_TRAITS_EDIT         9
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
 * Classes
 */

/// traits type for determining the signed and unsigned forms of a given integral
/// type
template <ss_size_t BYTES>
struct int_size_traits
{
    typedef void    signed_type;
    typedef void    unsigned_type;
};

#ifndef __STLSOFT_DOCUMENTATION_SKIP_SECTION

STLSOFT_TEMPLATE_SPECIALISATION
struct int_size_traits<1>
{
    typedef ss_sint8_t  signed_type;
    typedef ss_uint8_t  unsigned_type;
};

STLSOFT_TEMPLATE_SPECIALISATION
struct int_size_traits<2>
{
    typedef ss_sint16_t signed_type;
    typedef ss_uint16_t unsigned_type;
};

STLSOFT_TEMPLATE_SPECIALISATION
struct int_size_traits<4>
{
    typedef ss_sint32_t signed_type;
    typedef ss_uint32_t unsigned_type;
};

#ifdef STLSOFT_CF_64BIT_INT_SUPPORT

STLSOFT_TEMPLATE_SPECIALISATION
struct int_size_traits<8>
{
    typedef ss_sint64_t signed_type;
    typedef ss_uint64_t unsigned_type;
};

#endif /* STLSOFT_CF_64BIT_INT_SUPPORT */

#endif /* !__STLSOFT_DOCUMENTATION_SKIP_SECTION */

/* ////////////////////////////////////////////////////////////////////////// */

#ifndef _STLSOFT_NO_NAMESPACE
} // namespace stlsoft
#endif /* _STLSOFT_NO_NAMESPACE */

/* ////////////////////////////////////////////////////////////////////////// */

#endif /* !STLSOFT_INCL_H_STLSOFT_SIZE_TRAITS */

/* ////////////////////////////////////////////////////////////////////////// */
