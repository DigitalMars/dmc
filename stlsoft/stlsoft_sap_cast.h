/* /////////////////////////////////////////////////////////////////////////////
 * File:        stlsoft_sap_cast.h
 *
 * Purpose:     sap_cast - a dangerous weapon in the hands of the unwary.
 *
 * Created:     25th February 2004
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


/// \file stlsoft_sap_cast.h
///
/// sap_cast - a dangerous weapon in the hands of the unwary.

#ifndef STLSOFT_INCL_H_STLSOFT_SAP_CAST
#define STLSOFT_INCL_H_STLSOFT_SAP_CAST

#ifndef __STLSOFT_DOCUMENTATION_SKIP_SECTION
# define STLSOFT_VER_H_STLSOFT_SAP_CAST_MAJOR       2
# define STLSOFT_VER_H_STLSOFT_SAP_CAST_MINOR       0
# define STLSOFT_VER_H_STLSOFT_SAP_CAST_REVISION    1
# define STLSOFT_VER_H_STLSOFT_SAP_CAST_EDIT        11
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
 * Functions
 */

template<   ss_typename_param_k TO
        ,   ss_typename_param_k FROM
        >
inline TO sap_cast(FROM from)
{
#if defined(__STLSOFT_CF_TEMPLATE_PARTIAL_SPECIALISATION_SUPPORT) && \
    (   !defined(__STLSOFT_COMPILER_IS_BORLAND) || \
        __BORLANDC__ >= 0x0564)
    // Both types must be pointer types
    stlsoft_static_assert(0 != base_type_traits<FROM>::is_pointer);
    stlsoft_static_assert(0 != base_type_traits<TO>::is_pointer);

    typedef ss_typename_type_k base_type_traits<FROM>::base_type    from_base_type;
    typedef ss_typename_type_k base_type_traits<TO>::base_type      to_base_type;

    // The intermediate type might be void *, void const *, void volatile * or
    // void const volatile *
    typedef ss_typename_type_k select_first_type<   void const *
                                                ,   void *
                                                ,   base_type_traits<FROM>::is_const
                                                >::type     non_volatile_type;
    typedef ss_typename_type_k select_first_type<   void const volatile *
                                                ,   void volatile *
                                                ,   base_type_traits<FROM>::is_const
                                                >::type     volatile_type;
    typedef ss_typename_type_k select_first_type<   volatile_type
                                                ,   non_volatile_type
                                                ,   base_type_traits<FROM>::is_volatile
                                                >::type     pointer_type;

    // "static_cast" to void (const) (volatile) *
    pointer_type        pv  =   from;
#else /* __STLSOFT_CF_TEMPLATE_PARTIAL_SPECIALISATION_SUPPORT */
    void const volatile *p1 =   from;
    void                *pv =   const_cast<void*>(p1);
#endif /* __STLSOFT_CF_TEMPLATE_PARTIAL_SPECIALISATION_SUPPORT */

    // static_cast to destination type
    return static_cast<TO>(pv);
}

////////////////////////////////////////////////////////////////////////////////
// Unit-testing

#ifdef STLSOFT_UNITTEST

namespace unittest
{
    ss_bool_t test_stlsoft_sap_cast(unittest_reporter *r)
    {
        ss_bool_t               bSuccess    =   true;

        unittest_initialiser    init(r, "STLSoft", "sap_cast", __FILE__);

        int     i;
        void    *pv =   &i;
        short   *ps =   reinterpret_cast<short*>(&i);

        if(sap_cast<short*>(pv) != ps)
        {
            r->report("sap_cast failed ", __LINE__);
            bSuccess = false;
        }

        return bSuccess;
    }

    unittest_registrar    unittest_stlsoft_sap_cast(test_stlsoft_sap_cast);

} // namespace unittest

#endif /* STLSOFT_UNITTEST */

/* ////////////////////////////////////////////////////////////////////////// */

#ifndef _STLSOFT_NO_NAMESPACE
} // namespace stlsoft
#endif /* _STLSOFT_NO_NAMESPACE */

/* ////////////////////////////////////////////////////////////////////////// */

#endif /* !STLSOFT_INCL_H_STLSOFT_SAP_CAST */

/* ////////////////////////////////////////////////////////////////////////// */
