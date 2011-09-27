/* /////////////////////////////////////////////////////////////////////////////
 * File:        stlsoft_any_caster.h
 *
 * Purpose:     A dangerous, but sometimes necessary, tool for handling bad
 *              libraries.
 *
 * Created:     12th May 2004
 * Updated:     11th September 2004
 *
 * Home:        http://stlsoft.org/
 *
 * Copyright (c) 2004, Matthew Wilson and Synesis Software
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


/// \file stlsoft_any_caster.h
///
/// A dangerous, but sometimes necessary, tool for handling bad libraries.

#ifndef STLSOFT_INCL_H_STLSOFT_ANY_CASTER
#define STLSOFT_INCL_H_STLSOFT_ANY_CASTER

#ifndef __STLSOFT_DOCUMENTATION_SKIP_SECTION
# define STLSOFT_VER_H_STLSOFT_ANY_CASTER_MAJOR     2
# define STLSOFT_VER_H_STLSOFT_ANY_CASTER_MINOR     0
# define STLSOFT_VER_H_STLSOFT_ANY_CASTER_REVISION  1
# define STLSOFT_VER_H_STLSOFT_ANY_CASTER_EDIT      14
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

/// \brief a
template <ss_size_t N>
struct any_caster_invalid_type
{
    typedef any_caster_invalid_type<N>  class_type;

#if defined(__STLSOFT_COMPILER_IS_GCC)
protected:
#else /* ? compiler */
private:
#endif /* compiler */
    any_caster_invalid_type();
    any_caster_invalid_type(class_type const &);
};

/// \brief This class helps overcome problems when external libraries have errors in their
/// function parameter declarations, resulting in compilation-time type errors with some
/// versions of the libraries, and not with others.
///
/// \note See \link inetstl_filesystem_traits.h \endlink for an example of its use.
template<   typename    T
        ,   typename    T1
        ,   typename    T2
        ,   typename    T3  =   any_caster_invalid_type<3>*
        ,   typename    T4  =   any_caster_invalid_type<4>*
        ,   typename    T5  =   any_caster_invalid_type<5>*
        ,   typename    T6  =   any_caster_invalid_type<6>*
        ,   typename    T7  =   any_caster_invalid_type<7>*
        ,   typename    T8  =   any_caster_invalid_type<8>*
        >
union any_caster
{
    typedef any_caster<T, T1, T2, T3, T4, T5, T6, T7, T8>   class_type;

/// \name Construction
/// @{
public:
    /// Constructs an instance from the source type
    ss_explicit_k any_caster(T t)
    {
        this->t = t;
    }
/// @}

/// \name Conversion
/// @{
public:
    operator T1 () const
    {
        return t1;
    }
    operator T2 () const
    {
        return t2;
    }
    operator T3 () const
    {
        return t3;
    }
    operator T4 () const
    {
        return t4;
    }
    operator T5 () const
    {
        return t5;
    }
    operator T6 () const
    {
        return t6;
    }
    operator T7 () const
    {
        return t7;
    }
    operator T8 () const
    {
        return t8;
    }
/// @}

private:
    T   t;
    T1  t1;
    T2  t2;
    T3  t3;
    T4  t4;
    T5  t5;
    T6  t6;
    T7  t7;
    T8  t8;

// Not to be implemented
private:
    any_caster(class_type const &rhs);
    class_type &operator =(class_type const &rhs);
};

/* ////////////////////////////////////////////////////////////////////////// */

#ifndef _STLSOFT_NO_NAMESPACE
} // namespace stlsoft
#endif /* _STLSOFT_NO_NAMESPACE */

/* ////////////////////////////////////////////////////////////////////////// */

#endif /* !STLSOFT_INCL_H_STLSOFT_ANY_CASTER */

/* ////////////////////////////////////////////////////////////////////////// */
