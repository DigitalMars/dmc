/* /////////////////////////////////////////////////////////////////////////////
 * File:        stlsoft_simple_type_converter.h (originally MLTypCnv.h, ::SynesisStd)
 *
 * Purpose:     union_cast - a dangerous weapon in the hands of the unwary.
 *
 * Created:     2nd May 1997
 * Updated:     11th September 2004
 *
 * Home:        http://stlsoft.org/
 *
 * Copyright (c) 1997-2004, Matthew Wilson and Synesis Software
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


/// \file stlsoft_simple_type_converter.h
///
/// union_cast - a dangerous weapon in the hands of the unwary.

#ifndef STLSOFT_INCL_H_STLSOFT_SIMPLE_TYPE_CONVERTER
#define STLSOFT_INCL_H_STLSOFT_SIMPLE_TYPE_CONVERTER

#ifndef __STLSOFT_DOCUMENTATION_SKIP_SECTION
# define STLSOFT_VER_H_STLSOFT_SIMPLE_TYPE_CONVERTER_MAJOR      3
# define STLSOFT_VER_H_STLSOFT_SIMPLE_TYPE_CONVERTER_MINOR      0
# define STLSOFT_VER_H_STLSOFT_SIMPLE_TYPE_CONVERTER_REVISION   1
# define STLSOFT_VER_H_STLSOFT_SIMPLE_TYPE_CONVERTER_EDIT       16
#endif /* !__STLSOFT_DOCUMENTATION_SKIP_SECTION */

/* /////////////////////////////////////////////////////////////////////////////
 * Includes
 */

#ifndef STLSOFT_INCL_H_STLSOFT
# include "stlsoft.h"               // Include the STLSoft root header
#endif /* !STLSOFT_INCL_H_STLSOFT */
#ifndef STLSOFT_INCL_H_STLSOFT_UNION_CAST
# include "stlsoft_union_cast.h"    // union_cast
#endif /* !STLSOFT_INCL_H_STLSOFT_UNION_CAST */

/* /////////////////////////////////////////////////////////////////////////////
 * Namespace
 */

#ifndef _STLSOFT_NO_NAMESPACE
namespace stlsoft
{
#endif /* _STLSOFT_NO_NAMESPACE */

/* ////////////////////////////////////////////////////////////////////////// */

///

/* /////////////////////////////////////////////////////////////////////////////
 * Classes
 */

/// This class (union) effects conversion from one type to another, without
/// the use of casts.
///
/// \note The use of this technique is non-portable, and you use this class at
/// your own risk. Notwithstanding that, the TO and FROM types have to be the
/// same size, so the technique is widely usable.
template<   ss_typename_param_k TO
        ,   ss_typename_param_k FROM
        >
class simple_type_conversion
{
private:
    typedef union_cast<TO, FROM>                cast_type;
    typedef TO                                  to_type;
    typedef FROM                                from_type;
    typedef simple_type_conversion<TO, FROM>    class_type;

// Construction
public:
    /// Conversion constructor
    ss_explicit_k simple_type_conversion(from_type const from)
        : m_cast(from)
    {}

private:
    cast_type   m_cast;

// Not to be implemented
private:
    class_type &operator =(class_type const &);
};

/* ////////////////////////////////////////////////////////////////////////// */

#ifndef _STLSOFT_NO_NAMESPACE
} // namespace stlsoft
#endif /* _STLSOFT_NO_NAMESPACE */

/* ////////////////////////////////////////////////////////////////////////// */

#endif /* !STLSOFT_INCL_H_STLSOFT_SIMPLE_TYPE_CONVERTER */

/* ////////////////////////////////////////////////////////////////////////// */
