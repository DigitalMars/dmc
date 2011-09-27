/* /////////////////////////////////////////////////////////////////////////////
 * File:        stlsoft_union_cast.h (originally MLTypCnv.h, ::SynesisStd)
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


/// \file stlsoft_union_cast.h
///
/// union_cast - a dangerous weapon in the hands of the unwary.

#ifndef STLSOFT_INCL_H_STLSOFT_UNION_CAST
#define STLSOFT_INCL_H_STLSOFT_UNION_CAST

#ifndef __STLSOFT_DOCUMENTATION_SKIP_SECTION
# define STLSOFT_VER_H_STLSOFT_UNION_CAST_MAJOR      3
# define STLSOFT_VER_H_STLSOFT_UNION_CAST_MINOR      0
# define STLSOFT_VER_H_STLSOFT_UNION_CAST_REVISION   1
# define STLSOFT_VER_H_STLSOFT_UNION_CAST_EDIT       24
#endif /* !__STLSOFT_DOCUMENTATION_SKIP_SECTION */

/* /////////////////////////////////////////////////////////////////////////////
 * Includes
 */

#ifndef STLSOFT_INCL_H_STLSOFT
# include "stlsoft.h"               // Include the STLSoft root header
#endif /* !STLSOFT_INCL_H_STLSOFT */
#ifndef STLSOFT_INCL_H_CONSTRAINTS
# include "stlsoft_constraints.h"   // stlsoft_constraint_must_be_pod
#endif /* !STLSOFT_INCL_H_CONSTRAINTS */
#ifndef STLSOFT_INCL_H_TYPE_TRAITS
# include "stlsoft_type_traits.h"   //
#endif /* !STLSOFT_INCL_H_TYPE_TRAITS */

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

/// This class (union) effects conversion from one type to another, without
/// the use of casts.
///
/// \note The use of this technique is non-portable, and you use this class at
/// your own risk. Notwithstanding that, the TO and FROM types have to be the
/// same size, so the technique is widely usable.
template<   ss_typename_param_k TO
        ,   ss_typename_param_k FROM
        >
union union_cast
{
public:
    typedef TO                      to_type;
    typedef FROM                    from_type;
    typedef union_cast<TO, FROM>    class_type;

// Construction
public:
    /// Conversion constructor
    ss_explicit_k union_cast(from_type const from)
        : m_from(from)
    {
#if !defined(__STLSOFT_COMPILER_IS_DMC) || \
    __DMC__ >= 0x0833
        // (i) Sizes must be the same
        stlsoft_static_assert(sizeof(from_type) == sizeof(to_type));
        // (ii) Both must be of POD type
        stlsoft_constraint_must_be_pod(from_type);
        stlsoft_constraint_must_be_pod(to_type);
# if defined(__STLSOFT_CF_TEMPLATE_PARTIAL_SPECIALISATION_SUPPORT)
        // (iii) There must be either a change of const/volatile,
        /// or a change of type, but not both.
//        stlsoft_static_assert(  (1 == is_same_type<from_type, to_type>::value) ||
//                                (   base_type_traits<from_type>::is_const == base_type_traits<to_type>::is_const &&
//                                    base_type_traits<from_type>::is_volatile == base_type_traits<to_type>::is_volatile));

        // (iv) Both must be non-pointers, or must point to POD types
        typedef ss_typename_type_k base_type_traits<from_type>::base_type   from_base_type;
        typedef ss_typename_type_k base_type_traits<to_type>::base_type     to_base_type;

        stlsoft_constraint_must_be_pod_or_void(from_base_type);
        stlsoft_constraint_must_be_pod_or_void(to_base_type);

        if( !base_type_traits<from_type>::is_pointer &&
            base_type_traits<to_type>::is_pointer &&
            0 != from)
        {
            ss_size_t       to_size =   size_of<to_base_type>::value;
            union
            {
                ss_size_t   size_;
                from_type   from_;
            } u; // Can't be anonymous, otherwise GCC has an ICE!
            ss_size_t       from_value  =   (u.from_ = from, u.size_);

            // Need to add to_size, since Metrowerks warns of constant division by zero
            stlsoft_message_assert( "Misalignment in conversion from non-pointer to pointer", (0 == ((from_value + to_size) % to_size)));
        }
# endif /* __STLSOFT_CF_TEMPLATE_PARTIAL_SPECIALISATION_SUPPORT */
#else
        // Sizes must be the same
        stlsoft_assert(sizeof(from_type) == sizeof(to_type));
#endif /* !__STLSOFT_COMPILER_IS_DMC || __DMC__ >= 0x0830 */
    }

// Conversion
public:
    /// Implicit conversion operator
    operator to_type () const
    {
        return m_to;
    }

// Members
private:
    from_type const  m_from;
    to_type          m_to;

// Not to be implemented
private:
    class_type &operator =(class_type const &);
};


template<   ss_typename_param_k TO
        ,   ss_typename_param_k FROM
        >
inline union_cast<TO, FROM> make_union_cast(FROM const from)
{
    return union_cast<TO, FROM>(from);
}


template<   ss_typename_param_k TO
        ,   ss_typename_param_k FROM
        >
inline ss_bool_t operator < (union_cast<TO, FROM> const &lhs, TO const &rhs)
{
    TO const    lhs_    =   lhs;

    return lhs_ < rhs;
}

template<   ss_typename_param_k TO
        ,   ss_typename_param_k FROM
        >
inline ss_bool_t operator < (TO const &lhs, union_cast<TO, FROM> const &rhs)
{
    TO const    rhs_    =   rhs;

    return lhs < rhs_;
}

template<   ss_typename_param_k TO
        ,   ss_typename_param_k FROM
        >
inline ss_bool_t operator < (union_cast<TO, FROM> const &lhs, union_cast<TO, FROM> const &rhs)
{
    TO const    lhs_    =   lhs;
    TO const    rhs_    =   rhs;

    return lhs_ < rhs_;
}

////////////////////////////////////////////////////////////////////////////////
// Unit-testing

#ifdef STLSOFT_UNITTEST

namespace unittest
{
    ss_bool_t test_stlsoft_union_cast(unittest_reporter *r)
    {
        ss_bool_t               bSuccess    =   true;

        unittest_initialiser    init(r, "STLSoft", "union_cast", __FILE__);

#if 0
        if(<<TODO>>)
        {
            r->report("<<TODO>> failed ", __LINE__);
            bSuccess = false;
        }
#endif /* 0 */

        return bSuccess;
    }

    unittest_registrar    unittest_stlsoft_union_cast(test_stlsoft_union_cast);

} // namespace unittest

#endif /* STLSOFT_UNITTEST */

/* ////////////////////////////////////////////////////////////////////////// */

#ifndef _STLSOFT_NO_NAMESPACE
} // namespace stlsoft
#endif /* _STLSOFT_NO_NAMESPACE */

/* ////////////////////////////////////////////////////////////////////////// */

#endif /* !STLSOFT_INCL_H_STLSOFT_UNION_CAST */

/* ////////////////////////////////////////////////////////////////////////// */
