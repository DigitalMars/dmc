/* ////////////////////////////////////////////////////////////////////////////
 * File:        unixstl_atomic_functions.h (formerly MLAtomic.cpp, ::SynesisStd)
 *
 * Purpose:     UNIXSTL atomic functions.
 *
 * Created:     23rd October 1997
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


/// \file unixstl_atomic_functions.h
///
/// UNIXSTL atomic functions.

#ifndef UNIXSTL_INCL_H_UNIXSTL_ATOMIC_FUNCTIONS
#define UNIXSTL_INCL_H_UNIXSTL_ATOMIC_FUNCTIONS

#ifndef __STLSOFT_DOCUMENTATION_SKIP_SECTION
# define UNIXSTL_VER_H_UNIXSTL_ATOMIC_FUNCTIONS_MAJOR       3
# define UNIXSTL_VER_H_UNIXSTL_ATOMIC_FUNCTIONS_MINOR       0
# define UNIXSTL_VER_H_UNIXSTL_ATOMIC_FUNCTIONS_REVISION    1
# define UNIXSTL_VER_H_UNIXSTL_ATOMIC_FUNCTIONS_EDIT        165
#endif /* !__STLSOFT_DOCUMENTATION_SKIP_SECTION */

/* /////////////////////////////////////////////////////////////////////////////
 * Includes
 */

#if !defined(__i386__) && \
    !defined(_M_IX86)
# error Currently only defined for Intel architectures.
#endif /* !__i386__ && !_M_IX86 */

#ifndef UNIXSTL_INCL_H_UNIXSTL
# include "unixstl.h"   // Include the UNIXSTL root header
#endif /* !UNIXSTL_INCL_H_UNIXSTL */

/* /////////////////////////////////////////////////////////////////////////////
 * Namespace
 */

#ifndef _UNIXSTL_NO_NAMESPACE
# if defined(_STLSOFT_NO_NAMESPACE) || \
     defined(__STLSOFT_DOCUMENTATION_SKIP_SECTION)
/* There is no stlsoft namespace, so must define ::unixstl */
namespace unixstl
{
# else
/* Define stlsoft::unixstl_project */

namespace stlsoft
{

namespace unixstl_project
{

# endif /* _STLSOFT_NO_NAMESPACE */
#endif /* !_UNIXSTL_NO_NAMESPACE */

/* /////////////////////////////////////////////////////////////////////////////
 * Functions
 */

#if 0
#ifdef __SMP__
# define LOCK   "lock ; "
#else
# define LOCK   ""
#endif

/*
 * Make sure gcc doesn't try to be clever and move things around
 * on us. We need to use _exactly_ the address the user gave us,
 * not some alias that contains the same information.
 */
#define __atomic_fool_gcc(x) (*(struct { int a[100]; } *)x)

typedef int atomic_t;

static __inline__ void atomic_add(atomic_t i, atomic_t *v)
{
    __asm__ __volatile__(
        LOCK "addl %1,%0"
        :"=m" (__atomic_fool_gcc(v))
        :"ir" (i), "m" (__atomic_fool_gcc(v)));
}

static __inline__ void atomic_sub(atomic_t i, atomic_t *v)
{
    __asm__ __volatile__(
        LOCK "subl %1,%0"
        :"=m" (__atomic_fool_gcc(v))
        :"ir" (i), "m" (__atomic_fool_gcc(v)));
}

static __inline__ void atomic_inc(atomic_t *v)
{
    __asm__ __volatile__(
        LOCK "incl %0"
        :"=m" (__atomic_fool_gcc(v))
        :"m" (__atomic_fool_gcc(v)));
}

static __inline__ void atomic_dec(atomic_t *v)
{
    __asm__ __volatile__(
        LOCK "decl %0"
        :"=m" (__atomic_fool_gcc(v))
        :"m" (__atomic_fool_gcc(v)));
}

static __inline__ int atomic_dec_and_test(atomic_t *v)
{
    unsigned char c;

    __asm__ __volatile__(
        LOCK "decl %0; sete %1"
        :"=m" (__atomic_fool_gcc(v)), "=qm" (c)
        :"m" (__atomic_fool_gcc(v)));
    return c != 0;
}


atomic_read — read atomic variable
atomic_set — set atomic variable
atomic_add — add integer to atomic variable
atomic_sub — subtract the atomic variable
atomic_sub_and_test — subtract value from variable and test result
atomic_inc — increment atomic variable
atomic_dec — decrement atomic variable
atomic_dec_and_test — decrement and test
atomic_inc_and_test — increment and test
atomic_add_negative — add and test if negative
#else /* ? 0 */

inline us_sint32_t atomic_read(us_sint32_t volatile *pv)
{
    return *pv;
}

inline us_sint32_t atomic_write(us_sint32_t volatile *pv, us_sint32_t n)
{
    us_sint32_t oldval;

    /* Note: the "xchg" instruction does not need a "lock" prefix */
#ifdef __GNUC__
    __asm__ __volatile__(   "xchgl %0, %1" // long (32-bit) xchg, from
                        :   "=r"(oldval),   "=m"(*(pv))
                        :   "0"(n),         "m"(*(pv))
                        :   "memory");
#else /* ? __GNUC__ */
    _asm
    {
        mov ecx, dword ptr [pv]
        mov eax, n
        xchg dword ptr [ecx], eax
        mov oldval, eax
    }
#endif /* __GNUC__ */

    return oldval;
}

#endif /* 0 */

////////////////////////////////////////////////////////////////////////////////
// Unit-testing

#ifdef STLSOFT_UNITTEST

namespace unittest
{
    ss_bool_t test_unixstl_atomic_functions(unittest_reporter *r)
    {
        using stlsoft::unittest::unittest_initialiser;

        ss_bool_t               bSuccess    =   true;

        unittest_initialiser    init(r, "UNIXSTL", "atomic_functions", __FILE__);

#if 0
        if(<<TODO>>)
        {
            r->report("<<TODO>> failed ", __LINE__);
            bSuccess = false;
        }
#endif /* 0 */

        return bSuccess;
    }

    unittest_registrar    unittest_unixstl_atomic_functions(test_unixstl_atomic_functions);

} // namespace unittest

#endif /* STLSOFT_UNITTEST */

/* ////////////////////////////////////////////////////////////////////////// */

#ifndef _UNIXSTL_NO_NAMESPACE
# if defined(_STLSOFT_NO_NAMESPACE) || \
     defined(__STLSOFT_DOCUMENTATION_SKIP_SECTION)
} // namespace unixstl
# else
} // namespace unixstl_project
} // namespace stlsoft
# endif /* _STLSOFT_NO_NAMESPACE */
#endif /* !_UNIXSTL_NO_NAMESPACE */

/* ////////////////////////////////////////////////////////////////////////// */

#endif /* !WINSTL_INCL_H_UNIXSTL_ATOMIC_FUNCTIONS */

/* ////////////////////////////////////////////////////////////////////////// */
