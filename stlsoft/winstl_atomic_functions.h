/* /////////////////////////////////////////////////////////////////////////////
 * File:        winstl_atomic_functions.h (formerly MLAtomic.cpp, ::SynesisStd)
 *
 * Purpose:     WinSTL atomic functions.
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


/// \file winstl_atomic_functions.h
///
/// WinSTL atomic functions.

#ifndef WINSTL_INCL_H_WINSTL_ATOMIC_FUNCTIONS
#define WINSTL_INCL_H_WINSTL_ATOMIC_FUNCTIONS

#ifndef __STLSOFT_DOCUMENTATION_SKIP_SECTION
# define WINSTL_VER_H_WINSTL_ATOMIC_FUNCTIONS_MAJOR     3
# define WINSTL_VER_H_WINSTL_ATOMIC_FUNCTIONS_MINOR     0
# define WINSTL_VER_H_WINSTL_ATOMIC_FUNCTIONS_REVISION  1
# define WINSTL_VER_H_WINSTL_ATOMIC_FUNCTIONS_EDIT      170
#endif /* !__STLSOFT_DOCUMENTATION_SKIP_SECTION */

/* /////////////////////////////////////////////////////////////////////////////
 * Includes
 */

#ifndef WINSTL_INCL_H_WINSTL
# include "winstl.h"                        // Include the WinSTL root header
#endif /* !WINSTL_INCL_H_WINSTL */
#ifndef WINSTL_INCL_H_SPINMUTEXT
# include "winstl_spin_mutex.h"
#endif /* WINSTL_INCL_H_SPINMUTEXT */

/* /////////////////////////////////////////////////////////////////////////////
 * Compatibility
 */

#if !defined(_M_IX86)
# error Not valid for processors other than Intel
#endif /* _M_IX86 */

#ifdef STLSOFT_ATOMIC_CALLCONV
# undef STLSOFT_ATOMIC_CALLCONV
#endif /* STLSOFT_ATOMIC_CALLCONV */
#ifdef WINSTL_ATOMIC_FNS_CALLCONV_IS_FASTCALL
# undef WINSTL_ATOMIC_FNS_CALLCONV_IS_FASTCALL
#endif /* WINSTL_ATOMIC_FNS_CALLCONV_IS_FASTCALL */
#ifdef WINSTL_ATOMIC_FNS_CALLCONV_IS_STDCALL
# undef WINSTL_ATOMIC_FNS_CALLCONV_IS_STDCALL
#endif /* WINSTL_ATOMIC_FNS_CALLCONV_IS_STDCALL */

#ifndef STLSOFT_NO_FASTCALL
# if defined(__STLSOFT_COMPILER_IS_BORLAND) || \
     defined(__STLSOFT_COMPILER_IS_DMC) || \
     defined(__STLSOFT_COMPILER_IS_WATCOM)
#  define STLSOFT_NO_FASTCALL
# endif /* compiler */
#endif /* STLSOFT_NO_FASTCALL */

#if defined(STLSOFT_CF_FASTCALL_SUPPORTED) && \
    !defined(STLSOFT_NO_FASTCALL)
# define WINSTL_ATOMIC_FNS_CALLCONV_IS_FASTCALL
# define WINSTL_ATOMIC_FNS_CALLCONV         __fastcall
#elif defined(STLSOFT_CF_STDCALL_SUPPORTED)
# define WINSTL_ATOMIC_FNS_CALLCONV_IS_STDCALL
# define WINSTL_ATOMIC_FNS_CALLCONV         __stdcall
#else
# error Need to define calling convention
#endif /* call-conv */

/* /////////////////////////////////////////////////////////////////////////////
 * Namespace
 */

#ifndef _WINSTL_NO_NAMESPACE
# if defined(_STLSOFT_NO_NAMESPACE) || \
     defined(__STLSOFT_DOCUMENTATION_SKIP_SECTION)
/* There is no stlsoft namespace, so must define ::winstl */
namespace winstl
{
# else
/* Define stlsoft::winstl_project */

namespace stlsoft
{

namespace winstl_project
{

# endif /* _STLSOFT_NO_NAMESPACE */
#endif /* !_WINSTL_NO_NAMESPACE */

/* ////////////////////////////////////////////////////////////////////////// */

/// \weakgroup libraries STLSoft Libraries
/// \brief The individual libraries

/// \weakgroup libraries_threading Threading Library
/// \ingroup libraries
/// \brief This library provides synchronisation and threading facilities

/// \weakgroup winstl_threading_library Threading Library (WinSTL)
/// \ingroup WinSTL libraries_threading
/// \brief This library provides synchronisation and threading facilities for the Win32 API
/// @{

/* /////////////////////////////////////////////////////////////////////////////
 * Implementation options
 *
 * Because some compilers can make the code actually faster when it the naked
 * functions are not inline, we provide for that here. If you want to out-of-line
 * the functions, then you just need to define WINSTL_ATOMIC_FNS_DECLARATION_ONLY
 * in the code that uses it, and define WINSTL_ATOMIC_FNS_DEFINITION in one
 * implementation file.
 */

#ifdef WINSTL_ATOMIC_FNS_DECL_
# undef WINSTL_ATOMIC_FNS_DECL_
#endif /* WINSTL_ATOMIC_FNS_DECL_ */

#ifdef WINSTL_ATOMIC_FNS_IMPL_
# undef WINSTL_ATOMIC_FNS_IMPL_
#endif /* WINSTL_ATOMIC_FNS_IMPL_ */

#if defined(WINSTL_ATOMIC_FNS_DECLARATION_ONLY)
/* Only the function declarations are included */
# define WINSTL_ATOMIC_FNS_DECL_(type)              type WINSTL_ATOMIC_FNS_CALLCONV
#elif defined(WINSTL_ATOMIC_FNS_DEFINITION)
/* Only the function definitions are included */
# ifdef STSLSOFT_INLINE_ASM_SUPPORTED
#  define WINSTL_ATOMIC_FNS_IMPL_(type)             __declspec(naked) type WINSTL_ATOMIC_FNS_CALLCONV
# else /* ? STSLSOFT_INLINE_ASM_SUPPORTED */
#  define WINSTL_ATOMIC_FNS_IMPL_(type)             type WINSTL_ATOMIC_FNS_CALLCONV
# endif /* STSLSOFT_INLINE_ASM_SUPPORTED */
#else /* ? declaration / definition */
# if defined(__STLSOFT_COMPILER_IS_MWERKS) && \
     (__MWERKS__ & 0xFF00) < 0x3000
#  error CodeWarrior 7 and earlier does not generate correct code when inline naked functions are used
# endif /* __MWERKS__ & 0xFF00) < 0x3000 */

# ifdef STSLSOFT_INLINE_ASM_SUPPORTED
  /* The default is to define them inline */
#  ifdef STSLSOFT_ASM_IN_INLINE_SUPPORTED
#   define WINSTL_ATOMIC_FNS_DECL_(type)             inline type WINSTL_ATOMIC_FNS_CALLCONV
#   define WINSTL_ATOMIC_FNS_IMPL_(type)             inline __declspec(naked) type WINSTL_ATOMIC_FNS_CALLCONV
#  else /* ? STSLSOFT_ASM_IN_INLINE_SUPPORTED */
#   define WINSTL_ATOMIC_FNS_DECL_(type)             type WINSTL_ATOMIC_FNS_CALLCONV
#   define WINSTL_ATOMIC_FNS_IMPL_(type)             static __declspec(naked) type WINSTL_ATOMIC_FNS_CALLCONV
#  endif /* STSLSOFT_ASM_IN_INLINE_SUPPORTED */
# else /* ? STSLSOFT_INLINE_ASM_SUPPORTED */
  /* ASM not supported, so we're using the Win32 functions */
#   define WINSTL_ATOMIC_FNS_DECL_(type)             inline type WINSTL_ATOMIC_FNS_CALLCONV
#   define WINSTL_ATOMIC_FNS_IMPL_(type)             inline type WINSTL_ATOMIC_FNS_CALLCONV
# endif /* STSLSOFT_INLINE_ASM_SUPPORTED */
#endif /* declaration / definition */

/* /////////////////////////////////////////////////////////////////////////////
 * Atomic function declarations
 */

#ifndef WINSTL_ATOMIC_FNS_DEFINITION

/* Uni-processor variants */
WINSTL_ATOMIC_FNS_DECL_(ws_sint32_t) atomic_preincrement_up(ws_sint32_t volatile *pl);
WINSTL_ATOMIC_FNS_DECL_(ws_sint32_t) atomic_predecrement_up(ws_sint32_t volatile *pl);
WINSTL_ATOMIC_FNS_DECL_(ws_sint32_t) atomic_postincrement_up(ws_sint32_t volatile *pl);
WINSTL_ATOMIC_FNS_DECL_(ws_sint32_t) atomic_postdecrement_up(ws_sint32_t volatile *pl);
WINSTL_ATOMIC_FNS_DECL_(void) atomic_increment_up(ws_sint32_t volatile *pl);
WINSTL_ATOMIC_FNS_DECL_(void) atomic_decrement_up(ws_sint32_t volatile *pl);

WINSTL_ATOMIC_FNS_DECL_(ws_sint32_t) atomic_write_up(ws_sint32_t volatile *pl, ws_sint32_t n);
WINSTL_ATOMIC_FNS_DECL_(ws_sint32_t) atomic_read_up(ws_sint32_t volatile const *pl);

WINSTL_ATOMIC_FNS_DECL_(ws_sint32_t) atomic_postadd_up(ws_sint32_t volatile *pl, ws_sint32_t n);
inline ws_sint32_t atomic_preadd_up(ws_sint32_t volatile *pl, ws_sint32_t n);



/* SMP variants */
WINSTL_ATOMIC_FNS_DECL_(ws_sint32_t) atomic_preincrement_smp(ws_sint32_t volatile *pl);
WINSTL_ATOMIC_FNS_DECL_(ws_sint32_t) atomic_predecrement_smp(ws_sint32_t volatile *pl);
WINSTL_ATOMIC_FNS_DECL_(ws_sint32_t) atomic_postincrement_smp(ws_sint32_t volatile *pl);
WINSTL_ATOMIC_FNS_DECL_(ws_sint32_t) atomic_postdecrement_smp(ws_sint32_t volatile *pl);
inline void atomic_increment_smp(ws_sint32_t volatile *pl);
inline void atomic_decrement_smp(ws_sint32_t volatile *pl);

WINSTL_ATOMIC_FNS_DECL_(ws_sint32_t) atomic_write_smp(ws_sint32_t volatile *pl, ws_sint32_t n);
WINSTL_ATOMIC_FNS_DECL_(ws_sint32_t) atomic_read_smp(ws_sint32_t volatile const *pl);

WINSTL_ATOMIC_FNS_DECL_(ws_sint32_t) atomic_postadd_smp(ws_sint32_t volatile *pl, ws_sint32_t n);
inline ws_sint32_t atomic_preadd_smp(ws_sint32_t volatile *pl, ws_sint32_t n);



/* Multi-processor detection variants */
WINSTL_ATOMIC_FNS_DECL_(ws_sint32_t) atomic_preincrement(ws_sint32_t volatile *pl);
WINSTL_ATOMIC_FNS_DECL_(ws_sint32_t) atomic_predecrement(ws_sint32_t volatile *pl);
WINSTL_ATOMIC_FNS_DECL_(ws_sint32_t) atomic_postincrement(ws_sint32_t volatile *pl);
WINSTL_ATOMIC_FNS_DECL_(ws_sint32_t) atomic_postdecrement(ws_sint32_t volatile *pl);
WINSTL_ATOMIC_FNS_DECL_(void) atomic_increment(ws_sint32_t volatile *pl);
WINSTL_ATOMIC_FNS_DECL_(void) atomic_decrement(ws_sint32_t volatile *pl);

WINSTL_ATOMIC_FNS_DECL_(ws_sint32_t) atomic_write(ws_sint32_t volatile *pl, ws_sint32_t n);
WINSTL_ATOMIC_FNS_DECL_(ws_sint32_t) atomic_read(ws_sint32_t volatile const *pl);

WINSTL_ATOMIC_FNS_DECL_(ws_sint32_t) atomic_postadd(ws_sint32_t volatile *pl, ws_sint32_t n);
inline ws_sint32_t atomic_preadd(ws_sint32_t volatile *pl, ws_sint32_t n);


#endif /* !WINSTL_ATOMIC_FNS_DEFINITION */

/* /////////////////////////////////////////////////////////////////////////////
 * Atomic function definitions
 */

#ifndef __STLSOFT_DOCUMENTATION_SKIP_SECTION

# if !defined(WINSTL_ATOMIC_FNS_DECLARATION_ONLY)

#  ifdef STSLSOFT_INLINE_ASM_SUPPORTED
/* Inline assembler versions */

#ifdef __STLSOFT_COMPILER_IS_BORLAND
# pragma warn -8002     /* Suppresses: "Restarting compile using assembly" */
# pragma warn -8070     /* Suppresses: "Function should return a value" */
#endif /* __STLSOFT_COMPILER_IS_BORLAND */

// Uni-processor

WINSTL_ATOMIC_FNS_IMPL_(ws_sint32_t) atomic_preincrement_up(ws_sint32_t volatile * /* pl */)
{
    _asm
    {
        // pop 1 into eax, which can then be atomically added into *pl (held
        // in ecx). Since it's an xadd it exchanges the previous value into eax
        mov eax, 1

#if defined(WINSTL_ATOMIC_FNS_CALLCONV_IS_FASTCALL)
        // __fastcall: ecx is pl
#elif defined(WINSTL_ATOMIC_FNS_CALLCONV_IS_STDCALL)
        // __stdcall: arguments are on the stack

        mov ecx, dword ptr [esp + 4]
#else
# error Need to define calling convention
#endif /* call-conv */

        xadd dword ptr [ecx], eax

        // Since this is pre-increment, we need to inc eax to catch up with the
        // real value
        inc eax

#if defined(WINSTL_ATOMIC_FNS_CALLCONV_IS_FASTCALL)
        ret
#elif defined(WINSTL_ATOMIC_FNS_CALLCONV_IS_STDCALL)
        ret 4
#endif /* call-conv */
    }
}

WINSTL_ATOMIC_FNS_IMPL_(ws_sint32_t) atomic_predecrement_up(ws_sint32_t volatile * /* pl */)
{
    _asm
    {
        // pop 1 into eax, which can then be atomically added into *pl (held
        // in ecx). Since it's an xadd it exchanges the previous value into eax
        mov eax, -1

#if defined(WINSTL_ATOMIC_FNS_CALLCONV_IS_FASTCALL)
        // __fastcall: ecx is pl
#elif defined(WINSTL_ATOMIC_FNS_CALLCONV_IS_STDCALL)
        // __stdcall: arguments are on the stack

        mov ecx, dword ptr [esp + 4]
#else
# error Need to define calling convention
#endif /* call-conv */

        xadd dword ptr [ecx], eax

        // Since this is pre-decrement, we need to inc eax to catch up with the
        // real value
        dec eax

#if defined(WINSTL_ATOMIC_FNS_CALLCONV_IS_FASTCALL)
        ret
#elif defined(WINSTL_ATOMIC_FNS_CALLCONV_IS_STDCALL)
        ret 4
#endif /* call-conv */
    }
}

WINSTL_ATOMIC_FNS_IMPL_(ws_sint32_t) atomic_postincrement_up(ws_sint32_t volatile * /* pl */)
{
    _asm
    {
        // pop 1 into eax, which can then be atomically added into *pl (held
        // in ecx). Since it's an xadd it exchanges the previous value into eax
        mov eax, 1

#if defined(WINSTL_ATOMIC_FNS_CALLCONV_IS_FASTCALL)
        // __fastcall: ecx is pl
#elif defined(WINSTL_ATOMIC_FNS_CALLCONV_IS_STDCALL)
        // __stdcall: arguments are on the stack

        mov ecx, dword ptr [esp + 4]
#else
# error Need to define calling convention
#endif /* call-conv */

        xadd dword ptr [ecx], eax

        // Since this is post-increment, we need do nothing, since the previous
        // value is in eax

#if defined(WINSTL_ATOMIC_FNS_CALLCONV_IS_FASTCALL)
        ret
#elif defined(WINSTL_ATOMIC_FNS_CALLCONV_IS_STDCALL)
        ret 4
#endif /* call-conv */
    }
}

WINSTL_ATOMIC_FNS_IMPL_(ws_sint32_t) atomic_postdecrement_up(ws_sint32_t volatile * /* pl */)
{
    _asm
    {
        // pop 1 into eax, which can then be atomically added into *pl (held
        // in ecx). Since it's an xadd it exchanges the previous value into eax
        mov eax, -1

#if defined(WINSTL_ATOMIC_FNS_CALLCONV_IS_FASTCALL)
        // __fastcall: ecx is pl
#elif defined(WINSTL_ATOMIC_FNS_CALLCONV_IS_STDCALL)
        // __stdcall: arguments are on the stack

        mov ecx, dword ptr [esp + 4]
#else
# error Need to define calling convention
#endif /* call-conv */

        xadd dword ptr [ecx], eax

        // Since this is post-decrement, we need do nothing, since the previous
        // value is in eax

#if defined(WINSTL_ATOMIC_FNS_CALLCONV_IS_FASTCALL)
        ret
#elif defined(WINSTL_ATOMIC_FNS_CALLCONV_IS_STDCALL)
        ret 4
#endif /* call-conv */
    }
}

WINSTL_ATOMIC_FNS_IMPL_(void) atomic_increment_up(ws_sint32_t volatile * /* pl */)
{
    _asm
    {
#if defined(WINSTL_ATOMIC_FNS_CALLCONV_IS_FASTCALL)
        // __fastcall: ecx is pl
#elif defined(WINSTL_ATOMIC_FNS_CALLCONV_IS_STDCALL)
        // __stdcall: arguments are on the stack

        mov ecx, dword ptr [esp + 4]
#else
# error Need to define calling convention
#endif /* call-conv */

        add dword ptr [ecx], 1

#if defined(WINSTL_ATOMIC_FNS_CALLCONV_IS_FASTCALL)
        ret
#elif defined(WINSTL_ATOMIC_FNS_CALLCONV_IS_STDCALL)
        ret 4
#endif /* call-conv */
    }
}

WINSTL_ATOMIC_FNS_IMPL_(void) atomic_decrement_up(ws_sint32_t volatile * /* pl */)
{
    _asm
    {
#if defined(WINSTL_ATOMIC_FNS_CALLCONV_IS_FASTCALL)
        // __fastcall: ecx is pl
#elif defined(WINSTL_ATOMIC_FNS_CALLCONV_IS_STDCALL)
        // __stdcall: arguments are on the stack

        mov ecx, dword ptr [esp + 4]
#else
# error Need to define calling convention
#endif /* call-conv */

        sub dword ptr [ecx], 1

#if defined(WINSTL_ATOMIC_FNS_CALLCONV_IS_FASTCALL)
        ret
#elif defined(WINSTL_ATOMIC_FNS_CALLCONV_IS_STDCALL)
        ret 4
#endif /* call-conv */
    }
}

WINSTL_ATOMIC_FNS_IMPL_(ws_sint32_t) atomic_read_up(ws_sint32_t volatile const * /* pl */)
{
    _asm
    {
        mov eax, 0
#if defined(WINSTL_ATOMIC_FNS_CALLCONV_IS_FASTCALL)
        // __fastcall: ecx is pl

#elif defined(WINSTL_ATOMIC_FNS_CALLCONV_IS_STDCALL)
        // __stdcall: arguments are on the stack
        mov ecx, dword ptr [esp + 4]
#else
# error Need to define calling convention
#endif /* call-conv */

        // pop 0 into eax, which can then be atomically added into *pl (held
        // in ecx), leaving the value unchanged.
        xadd dword ptr [ecx], eax

        // Since it's an xadd it exchanges the previous value into eax, which
        // is exactly what's required

#if defined(WINSTL_ATOMIC_FNS_CALLCONV_IS_FASTCALL)
        ret
#elif defined(WINSTL_ATOMIC_FNS_CALLCONV_IS_STDCALL)
        ret 4
#endif /* call-conv */
    }
}

WINSTL_ATOMIC_FNS_IMPL_(ws_sint32_t) atomic_write_up(ws_sint32_t volatile * /* pl */, ws_sint32_t /* n */)
{
    _asm
    {
#if defined(WINSTL_ATOMIC_FNS_CALLCONV_IS_FASTCALL)
        // __fastcall: ecx is pl, edx is n

        // Just exchange *pl and n
        xchg dword ptr [ecx], edx

        // The previous value goes into edx, so me move it into eax for return
        mov eax, edx

        ret
#elif defined(WINSTL_ATOMIC_FNS_CALLCONV_IS_STDCALL)
        // __stdcall: arguments are on the stack: pl in esp+4, pl in esp+8
        mov ecx, dword ptr [esp + 4]    // Load the address of pl into ecx
        mov eax, dword ptr [esp + 8]    // Load the value into eax, so the return value will be there waiting

        xchg dword ptr [ecx], eax

        ret 8
#else
# error Need to define calling convention
#endif /* call-conv */
    }
}


WINSTL_ATOMIC_FNS_IMPL_(ws_sint32_t) atomic_postadd_up(ws_sint32_t volatile * /* pl */, ws_sint32_t /* n */)
{
    // Thanks to Eugene Gershnik for the fast-call implementation
    __asm
    {
#if defined(WINSTL_ATOMIC_FNS_CALLCONV_IS_FASTCALL)
        // __fastcall: ecx is pl, edx is n

        // Simply atomically add them, which will leave the previous value
        // in edx
        xadd dword ptr [ecx], edx

        // Just need to move adx into eax to return it
        mov eax, edx

        ret
#elif defined(WINSTL_ATOMIC_FNS_CALLCONV_IS_STDCALL)
        // __stdcall: arguments are on the stack: pl in esp+4, pl in esp+8

        // Simply atomically add them, which will leave the previous value
        // in edx
        mov ecx, dword ptr [esp + 4]    // Load the address of pl into ecx
        mov eax, dword ptr [esp + 8]    // Load the value into eax, so the return value will be there waiting

        xadd dword ptr [ecx], eax

        // Just need to move adx into eax to return it

        ret 8
#else
# error Need to define calling convention
#endif /* call-conv */
    }
}

// Symmetric multi-processor

WINSTL_ATOMIC_FNS_IMPL_(ws_sint32_t) atomic_preincrement_smp(ws_sint32_t volatile * /* pl */)
{
    _asm
    {
        // pop 1 into eax, which can then be atomically added into *pl (held
        // in ecx). Since it's an xadd it exchanges the previous value into eax
        mov eax, 1

#if defined(WINSTL_ATOMIC_FNS_CALLCONV_IS_FASTCALL)
        // __fastcall: ecx is pl
#elif defined(WINSTL_ATOMIC_FNS_CALLCONV_IS_STDCALL)
        // __stdcall: arguments are on the stack

        mov ecx, dword ptr [esp + 4]
#else
# error Need to define calling convention
#endif /* call-conv */

        lock xadd dword ptr [ecx], eax

        // Since this is pre-increment, we need to inc eax to catch up with the
        // real value
        inc eax

#if defined(WINSTL_ATOMIC_FNS_CALLCONV_IS_FASTCALL)
        ret
#elif defined(WINSTL_ATOMIC_FNS_CALLCONV_IS_STDCALL)
        ret 4
#endif /* call-conv */
    }
}

WINSTL_ATOMIC_FNS_IMPL_(ws_sint32_t) atomic_predecrement_smp(ws_sint32_t volatile * /* pl */)
{
    _asm
    {
        // pop 1 into eax, which can then be atomically added into *pl (held
        // in ecx). Since it's an xadd it exchanges the previous value into eax
        mov eax, -1

#if defined(WINSTL_ATOMIC_FNS_CALLCONV_IS_FASTCALL)
        // __fastcall: ecx is pl
#elif defined(WINSTL_ATOMIC_FNS_CALLCONV_IS_STDCALL)
        // __stdcall: arguments are on the stack

        mov ecx, dword ptr [esp + 4]
#else
# error Need to define calling convention
#endif /* call-conv */

        lock xadd dword ptr [ecx], eax

        // Since this is pre-decrement, we need to inc eax to catch up with the
        // real value
        dec eax

#if defined(WINSTL_ATOMIC_FNS_CALLCONV_IS_FASTCALL)
        ret
#elif defined(WINSTL_ATOMIC_FNS_CALLCONV_IS_STDCALL)
        ret 4
#endif /* call-conv */
    }
}

WINSTL_ATOMIC_FNS_IMPL_(ws_sint32_t) atomic_postincrement_smp(ws_sint32_t volatile * /* pl */)
{
    _asm
    {
        // pop 1 into eax, which can then be atomically added into *pl (held
        // in ecx). Since it's an xadd it exchanges the previous value into eax
        mov eax, 1

#if defined(WINSTL_ATOMIC_FNS_CALLCONV_IS_FASTCALL)
        // __fastcall: ecx is pl
#elif defined(WINSTL_ATOMIC_FNS_CALLCONV_IS_STDCALL)
        // __stdcall: arguments are on the stack

        mov ecx, dword ptr [esp + 4]
#else
# error Need to define calling convention
#endif /* call-conv */

        lock xadd dword ptr [ecx], eax

        // Since this is post-increment, we need do nothing, since the previous
        // value is in eax

#if defined(WINSTL_ATOMIC_FNS_CALLCONV_IS_FASTCALL)
        ret
#elif defined(WINSTL_ATOMIC_FNS_CALLCONV_IS_STDCALL)
        ret 4
#endif /* call-conv */
    }
}

WINSTL_ATOMIC_FNS_IMPL_(ws_sint32_t) atomic_postdecrement_smp(ws_sint32_t volatile * /* pl */)
{
    _asm
    {
        // pop 1 into eax, which can then be atomically added into *pl (held
        // in ecx). Since it's an xadd it exchanges the previous value into eax
        mov eax, -1

#if defined(WINSTL_ATOMIC_FNS_CALLCONV_IS_FASTCALL)
        // __fastcall: ecx is pl
#elif defined(WINSTL_ATOMIC_FNS_CALLCONV_IS_STDCALL)
        // __stdcall: arguments are on the stack

        mov ecx, dword ptr [esp + 4]
#else
# error Need to define calling convention
#endif /* call-conv */

        lock xadd dword ptr [ecx], eax

        // Since this is post-decrement, we need do nothing, since the previous
        // value is in eax

#if defined(WINSTL_ATOMIC_FNS_CALLCONV_IS_FASTCALL)
        ret
#elif defined(WINSTL_ATOMIC_FNS_CALLCONV_IS_STDCALL)
        ret 4
#endif /* call-conv */
    }
}

WINSTL_ATOMIC_FNS_IMPL_(ws_sint32_t) atomic_read_smp(ws_sint32_t volatile const * /* pl */)
{
    _asm
    {
        mov eax, 0
#if defined(WINSTL_ATOMIC_FNS_CALLCONV_IS_FASTCALL)
        // __fastcall: ecx is pl

#elif defined(WINSTL_ATOMIC_FNS_CALLCONV_IS_STDCALL)
        // __stdcall: arguments are on the stack
        mov ecx, dword ptr [esp + 4]
#else
# error Need to define calling convention
#endif /* call-conv */

        // pop 0 into eax, which can then be atomically added into *pl (held
        // in ecx), leaving the value unchanged.
        lock xadd dword ptr [ecx], eax

        // Since it's an xadd it exchanges the previous value into eax, which
        // is exactly what's required

#if defined(WINSTL_ATOMIC_FNS_CALLCONV_IS_FASTCALL)
        ret
#elif defined(WINSTL_ATOMIC_FNS_CALLCONV_IS_STDCALL)
        ret 4
#endif /* call-conv */
    }
}

WINSTL_ATOMIC_FNS_IMPL_(ws_sint32_t) atomic_write_smp(ws_sint32_t volatile * /* pl */, ws_sint32_t /* n */)
{
    _asm
    {
#if defined(WINSTL_ATOMIC_FNS_CALLCONV_IS_FASTCALL)
        // __fastcall: ecx is pl, edx is n

        // Just exchange *pl and n
        /* lock */ xchg dword ptr [ecx], edx

        // The previous value goes into edx, so me move it into eax for return
        mov eax, edx

        ret
#elif defined(WINSTL_ATOMIC_FNS_CALLCONV_IS_STDCALL)
        // __stdcall: arguments are on the stack: pl in esp+4, pl in esp+8
        mov ecx, dword ptr [esp + 4]    // Load the address of pl into ecx
        mov eax, dword ptr [esp + 8]    // Load the value into eax, so the return value will be there waiting

        /* lock */ xchg dword ptr [ecx], eax

        ret 8
#else
# error Need to define calling convention
#endif /* call-conv */
    }
}


WINSTL_ATOMIC_FNS_IMPL_(ws_sint32_t) atomic_postadd_smp(ws_sint32_t volatile * /* pl */, ws_sint32_t /* n */)
{
    // Thanks to Eugene Gershnik for the fast-call implementation
    __asm
    {
#if defined(WINSTL_ATOMIC_FNS_CALLCONV_IS_FASTCALL)
        // __fastcall: ecx is pl, edx is n

        // Simply atomically add them, which will leave the previous value
        // in edx
        lock xadd dword ptr [ecx], edx

        // Just need to move adx into eax to return it
        mov eax, edx

        ret
#elif defined(WINSTL_ATOMIC_FNS_CALLCONV_IS_STDCALL)
        // __stdcall: arguments are on the stack: pl in esp+4, pl in esp+8

        // Simply atomically add them, which will leave the previous value
        // in edx
        mov ecx, dword ptr [esp + 4]    // Load the address of pl into ecx
        mov eax, dword ptr [esp + 8]    // Load the value into eax, so the return value will be there waiting

        lock xadd dword ptr [ecx], eax

        // Just need to move adx into eax to return it

        ret 8
#else
# error Need to define calling convention
#endif /* call-conv */
    }
}

// Processor detecting

namespace
{
    ws_bool_t is_host_up()
    {
        // All these statics are guaranteed to be zero as a result of the module/process loading
        static ws_sint32_t  s_spin; // The spin variable
        static ws_bool_t    s_init; // This is guaranteed to be zero
        static ws_bool_t    s_up;   // This is the flag variably, also guaranteed to be zero

        // Simple spin lock
        if(!s_init) // Low cost pre-test. In the unlikely event that another thread does come in and
        {           // also see this as false, the dual initialisation of all three statics is benign
            spin_mutex  smx(&s_spin);

            smx.lock();
            if(!s_init)
            {
                SYSTEM_INFO sys_info;

                ::GetSystemInfo(&sys_info);

                s_init = true;

                s_up = 1 == sys_info.dwNumberOfProcessors;
            }
            smx.unlock();
        }

        return s_up;
    }

    // s_up is guaranteed to be zero at load time.
    //
    // There is a race condition with all static variables, since multiple threads
    // can come in and one can have set the hidden flag variable without prior to
    // setting the static variable itself, just at the time that an arbitrary number
    // of other threads pick up the pre-initialised value.
    //
    // However, because the test here is whether to skip the lock, the pathological
    // case is benign. The only cost in the very rare case where it happens is that
    // the thread(s) will use bus locking until such time as the static is fully
    // initialised.
    static ws_bool_t    s_up = is_host_up();
}

WINSTL_ATOMIC_FNS_IMPL_(ws_sint32_t) atomic_preincrement(ws_sint32_t volatile * /* pl */)
{
    if(s_up)
    {
        _asm
        {
            // pop 1 into eax, which can then be atomically added into *pl (held
            // in ecx). Since it's an xadd it exchanges the previous value into eax
            mov eax, 1

#if defined(WINSTL_ATOMIC_FNS_CALLCONV_IS_FASTCALL)
            // __fastcall: ecx is pl
#elif defined(WINSTL_ATOMIC_FNS_CALLCONV_IS_STDCALL)
            // __stdcall: arguments are on the stack

            mov ecx, dword ptr [esp + 4]
#else
# error Need to define calling convention
#endif /* call-conv */

            xadd dword ptr [ecx], eax

            // Since this is pre-increment, we need to inc eax to catch up with the
            // real value
            inc eax

#if defined(WINSTL_ATOMIC_FNS_CALLCONV_IS_FASTCALL)
            ret
#elif defined(WINSTL_ATOMIC_FNS_CALLCONV_IS_STDCALL)
            ret 4
#endif /* call-conv */
        }
    }
    else
    {
        _asm
        {
            // pop 1 into eax, which can then be atomically added into *pl (held
            // in ecx). Since it's an xadd it exchanges the previous value into eax
            mov eax, 1

#if defined(WINSTL_ATOMIC_FNS_CALLCONV_IS_FASTCALL)
            // __fastcall: ecx is pl
#elif defined(WINSTL_ATOMIC_FNS_CALLCONV_IS_STDCALL)
            // __stdcall: arguments are on the stack

            mov ecx, dword ptr [esp + 4]
#else
# error Need to define calling convention
#endif /* call-conv */

            lock xadd dword ptr [ecx], eax

            // Since this is pre-increment, we need to inc eax to catch up with the
            // real value
            inc eax

#if defined(WINSTL_ATOMIC_FNS_CALLCONV_IS_FASTCALL)
            ret
#elif defined(WINSTL_ATOMIC_FNS_CALLCONV_IS_STDCALL)
            ret 4
#endif /* call-conv */
        }
    }
}

WINSTL_ATOMIC_FNS_IMPL_(ws_sint32_t) atomic_predecrement(ws_sint32_t volatile * /* pl */)
{
    if(s_up)
    {
        _asm
        {
            // pop 1 into eax, which can then be atomically added into *pl (held
            // in ecx). Since it's an xadd it exchanges the previous value into eax
            mov eax, -1

#if defined(WINSTL_ATOMIC_FNS_CALLCONV_IS_FASTCALL)
            // __fastcall: ecx is pl
#elif defined(WINSTL_ATOMIC_FNS_CALLCONV_IS_STDCALL)
            // __stdcall: arguments are on the stack

            mov ecx, dword ptr [esp + 4]
#else
# error Need to define calling convention
#endif /* call-conv */

            xadd dword ptr [ecx], eax

            // Since this is pre-decrement, we need to inc eax to catch up with the
            // real value
            dec eax

#if defined(WINSTL_ATOMIC_FNS_CALLCONV_IS_FASTCALL)
            ret
#elif defined(WINSTL_ATOMIC_FNS_CALLCONV_IS_STDCALL)
            ret 4
#endif /* call-conv */
        }
    }
    else
    {
        _asm
        {
            // pop 1 into eax, which can then be atomically added into *pl (held
            // in ecx). Since it's an xadd it exchanges the previous value into eax
            mov eax, -1

#if defined(WINSTL_ATOMIC_FNS_CALLCONV_IS_FASTCALL)
            // __fastcall: ecx is pl
#elif defined(WINSTL_ATOMIC_FNS_CALLCONV_IS_STDCALL)
            // __stdcall: arguments are on the stack

            mov ecx, dword ptr [esp + 4]
#else
# error Need to define calling convention
#endif /* call-conv */

            lock xadd dword ptr [ecx], eax

            // Since this is pre-decrement, we need to inc eax to catch up with the
            // real value
            dec eax

#if defined(WINSTL_ATOMIC_FNS_CALLCONV_IS_FASTCALL)
            ret
#elif defined(WINSTL_ATOMIC_FNS_CALLCONV_IS_STDCALL)
            ret 4
#endif /* call-conv */
        }
    }
}

WINSTL_ATOMIC_FNS_IMPL_(ws_sint32_t) atomic_postincrement(ws_sint32_t volatile * /* pl */)
{
    if(s_up)
    {
        _asm
        {
            // pop 1 into eax, which can then be atomically added into *pl (held
            // in ecx). Since it's an xadd it exchanges the previous value into eax
            mov eax, 1

#if defined(WINSTL_ATOMIC_FNS_CALLCONV_IS_FASTCALL)
            // __fastcall: ecx is pl
#elif defined(WINSTL_ATOMIC_FNS_CALLCONV_IS_STDCALL)
            // __stdcall: arguments are on the stack

            mov ecx, dword ptr [esp + 4]
#else
# error Need to define calling convention
#endif /* call-conv */

            xadd dword ptr [ecx], eax

            // Since this is post-increment, we need do nothing, since the previous
            // value is in eax

#if defined(WINSTL_ATOMIC_FNS_CALLCONV_IS_FASTCALL)
            ret
#elif defined(WINSTL_ATOMIC_FNS_CALLCONV_IS_STDCALL)
            ret 4
#endif /* call-conv */
        }
    }
    else
    {
        _asm
        {
            // pop 1 into eax, which can then be atomically added into *pl (held
            // in ecx). Since it's an xadd it exchanges the previous value into eax
            mov eax, 1

#if defined(WINSTL_ATOMIC_FNS_CALLCONV_IS_FASTCALL)
            // __fastcall: ecx is pl
#elif defined(WINSTL_ATOMIC_FNS_CALLCONV_IS_STDCALL)
            // __stdcall: arguments are on the stack

            mov ecx, dword ptr [esp + 4]
#else
# error Need to define calling convention
#endif /* call-conv */

            lock xadd dword ptr [ecx], eax

            // Since this is post-increment, we need do nothing, since the previous
            // value is in eax

#if defined(WINSTL_ATOMIC_FNS_CALLCONV_IS_FASTCALL)
            ret
#elif defined(WINSTL_ATOMIC_FNS_CALLCONV_IS_STDCALL)
            ret 4
#endif /* call-conv */
        }
    }
}

WINSTL_ATOMIC_FNS_IMPL_(ws_sint32_t) atomic_postdecrement(ws_sint32_t volatile * /* pl */)
{
    if(s_up)
    {
        _asm
        {
            // pop 1 into eax, which can then be atomically added into *pl (held
            // in ecx). Since it's an xadd it exchanges the previous value into eax
            mov eax, -1

#if defined(WINSTL_ATOMIC_FNS_CALLCONV_IS_FASTCALL)
        // __fastcall: ecx is pl
#elif defined(WINSTL_ATOMIC_FNS_CALLCONV_IS_STDCALL)
            // __stdcall: arguments are on the stack

            mov ecx, dword ptr [esp + 4]
#else
# error Need to define calling convention
#endif /* call-conv */

            xadd dword ptr [ecx], eax

            // Since this is post-decrement, we need do nothing, since the previous
            // value is in eax

#if defined(WINSTL_ATOMIC_FNS_CALLCONV_IS_FASTCALL)
            ret
#elif defined(WINSTL_ATOMIC_FNS_CALLCONV_IS_STDCALL)
            ret 4
#endif /* call-conv */
        }
    }
    else
    {
        _asm
        {
            // pop 1 into eax, which can then be atomically added into *pl (held
            // in ecx). Since it's an xadd it exchanges the previous value into eax
            mov eax, -1

#if defined(WINSTL_ATOMIC_FNS_CALLCONV_IS_FASTCALL)
        // __fastcall: ecx is pl
#elif defined(WINSTL_ATOMIC_FNS_CALLCONV_IS_STDCALL)
            // __stdcall: arguments are on the stack

            mov ecx, dword ptr [esp + 4]
#else
# error Need to define calling convention
#endif /* call-conv */

            lock xadd dword ptr [ecx], eax

            // Since this is post-decrement, we need do nothing, since the previous
            // value is in eax

#if defined(WINSTL_ATOMIC_FNS_CALLCONV_IS_FASTCALL)
            ret
#elif defined(WINSTL_ATOMIC_FNS_CALLCONV_IS_STDCALL)
            ret 4
#endif /* call-conv */
        }
    }
}

WINSTL_ATOMIC_FNS_IMPL_(void) atomic_increment(ws_sint32_t volatile * /* pl */)
{
    if(s_up)
    {
        _asm
        {
#if defined(WINSTL_ATOMIC_FNS_CALLCONV_IS_FASTCALL)
            // __fastcall: ecx is pl
#elif defined(WINSTL_ATOMIC_FNS_CALLCONV_IS_STDCALL)
            // __stdcall: arguments are on the stack

            mov ecx, dword ptr [esp + 4]
#else
# error Need to define calling convention
#endif /* call-conv */

            add dword ptr [ecx], 1

#if defined(WINSTL_ATOMIC_FNS_CALLCONV_IS_FASTCALL)
            ret
#elif defined(WINSTL_ATOMIC_FNS_CALLCONV_IS_STDCALL)
            ret 4
#endif /* call-conv */
        }
    }
    else
    {
        _asm
        {
#if defined(WINSTL_ATOMIC_FNS_CALLCONV_IS_FASTCALL)
            // __fastcall: ecx is pl
#elif defined(WINSTL_ATOMIC_FNS_CALLCONV_IS_STDCALL)
            // __stdcall: arguments are on the stack

            mov ecx, dword ptr [esp + 4]
#else
# error Need to define calling convention
#endif /* call-conv */

            // The IA-32 Intel Architecture Software Developer's Manual, volume 2
            // states that a LOCK can be prefixed to ADD, but CodePlay VectorC
            // has a problem with it.
#if defined(__STLSOFT_COMPILER_IS_VECTORC)
            mov eax, 1
            lock xadd dword ptr [ecx], eax
#else /* ? compiler */
            lock add dword ptr [ecx], 1
#endif /* compiler */


#if defined(WINSTL_ATOMIC_FNS_CALLCONV_IS_FASTCALL)
            ret
#elif defined(WINSTL_ATOMIC_FNS_CALLCONV_IS_STDCALL)
            ret 4
#endif /* call-conv */
        }
    }
}

WINSTL_ATOMIC_FNS_IMPL_(void) atomic_decrement(ws_sint32_t volatile * /* pl */)
{
    if(s_up)
    {
        _asm
        {
#if defined(WINSTL_ATOMIC_FNS_CALLCONV_IS_FASTCALL)
            // __fastcall: ecx is pl
#elif defined(WINSTL_ATOMIC_FNS_CALLCONV_IS_STDCALL)
            // __stdcall: arguments are on the stack

            mov ecx, dword ptr [esp + 4]
#else
# error Need to define calling convention
#endif /* call-conv */

            add dword ptr [ecx], -1

#if defined(WINSTL_ATOMIC_FNS_CALLCONV_IS_FASTCALL)
            ret
#elif defined(WINSTL_ATOMIC_FNS_CALLCONV_IS_STDCALL)
            ret 4
#endif /* call-conv */
        }
    }
    else
    {
        _asm
        {
#if defined(WINSTL_ATOMIC_FNS_CALLCONV_IS_FASTCALL)
            // __fastcall: ecx is pl
#elif defined(WINSTL_ATOMIC_FNS_CALLCONV_IS_STDCALL)
            // __stdcall: arguments are on the stack

            mov ecx, dword ptr [esp + 4]
#else
# error Need to define calling convention
#endif /* call-conv */

#if defined(__STLSOFT_COMPILER_IS_VECTORC)
            mov eax, -1
            lock xadd dword ptr [ecx], eax
#else /* ? compiler */
            // This might be wrong
            lock sub dword ptr [ecx], 1
#endif /* compiler */

#if defined(WINSTL_ATOMIC_FNS_CALLCONV_IS_FASTCALL)
            ret
#elif defined(WINSTL_ATOMIC_FNS_CALLCONV_IS_STDCALL)
            ret 4
#endif /* call-conv */
        }
    }
}

WINSTL_ATOMIC_FNS_IMPL_(ws_sint32_t) atomic_read(ws_sint32_t volatile const * /* pl */)
{
    if(s_up)
    {
        _asm
        {
            mov eax, 0
#if defined(WINSTL_ATOMIC_FNS_CALLCONV_IS_FASTCALL)
            // __fastcall: ecx is pl

#elif defined(WINSTL_ATOMIC_FNS_CALLCONV_IS_STDCALL)
            // __stdcall: arguments are on the stack
            mov ecx, dword ptr [esp + 4]
#else
# error Need to define calling convention
#endif /* call-conv */

            // pop 0 into eax, which can then be atomically added into *pl (held
            // in ecx), leaving the value unchanged.
            xadd dword ptr [ecx], eax

            // Since it's an xadd it exchanges the previous value into eax, which
            // is exactly what's required

#if defined(WINSTL_ATOMIC_FNS_CALLCONV_IS_FASTCALL)
            ret
#elif defined(WINSTL_ATOMIC_FNS_CALLCONV_IS_STDCALL)
            ret 4
#endif /* call-conv */
        }
    }
    else
    {
        _asm
        {
            mov eax, 0
#if defined(WINSTL_ATOMIC_FNS_CALLCONV_IS_FASTCALL)
            // __fastcall: ecx is pl

#elif defined(WINSTL_ATOMIC_FNS_CALLCONV_IS_STDCALL)
            // __stdcall: arguments are on the stack
            mov ecx, dword ptr [esp + 4]
#else
# error Need to define calling convention
#endif /* call-conv */

            // pop 0 into eax, which can then be atomically added into *pl (held
            // in ecx), leaving the value unchanged.
            lock xadd dword ptr [ecx], eax

            // Since it's an xadd it exchanges the previous value into eax, which
            // is exactly what's required

#if defined(WINSTL_ATOMIC_FNS_CALLCONV_IS_FASTCALL)
            ret
#elif defined(WINSTL_ATOMIC_FNS_CALLCONV_IS_STDCALL)
            ret 4
#endif /* call-conv */
        }
    }
}

WINSTL_ATOMIC_FNS_IMPL_(ws_sint32_t) atomic_write(ws_sint32_t volatile * /* pl */, ws_sint32_t /* n */)
{
    _asm
    {
#if defined(WINSTL_ATOMIC_FNS_CALLCONV_IS_FASTCALL)
        // __fastcall: ecx is pl, edx is n

        // Just exchange *pl and n
        lock xchg dword ptr [ecx], edx

        // The previous value goes into edx, so me move it into eax for return
        mov eax, edx

        ret
#elif defined(WINSTL_ATOMIC_FNS_CALLCONV_IS_STDCALL)
        // __stdcall: arguments are on the stack: pl in esp+4, pl in esp+8
        mov ecx, dword ptr [esp + 4]    // Load the address of pl into ecx
        mov eax, dword ptr [esp + 8]    // Load the value into eax, so the return value will be there waiting

        xchg dword ptr [ecx], eax

        ret 8
#else
# error Need to define calling convention
#endif /* call-conv */
    }
}


WINSTL_ATOMIC_FNS_IMPL_(ws_sint32_t) atomic_postadd(ws_sint32_t volatile * /* pl */, ws_sint32_t /* n */)
{
    // Thanks to Eugene Gershnik for the fast-call implementation
    if(s_up)
    {
        __asm
        {
#if defined(WINSTL_ATOMIC_FNS_CALLCONV_IS_FASTCALL)
            // __fastcall: ecx is pl, edx is n

            // Simply atomically add them, which will leave the previous value
            // in edx
            xadd dword ptr [ecx], edx

            // Just need to move adx into eax to return it
            mov eax, edx

            ret
#elif defined(WINSTL_ATOMIC_FNS_CALLCONV_IS_STDCALL)
            // __stdcall: arguments are on the stack: pl in esp+4, pl in esp+8

            // Simply atomically add them, which will leave the previous value
            // in edx
            mov ecx, dword ptr [esp + 4]    // Load the address of pl into ecx
            mov eax, dword ptr [esp + 8]    // Load the value into eax, so the return value will be there waiting

            xadd dword ptr [ecx], eax

            // Just need to move adx into eax to return it

            ret 8
#else
# error Need to define calling convention
#endif /* call-conv */
        }
    }
    else
    {
        __asm
        {
#if defined(WINSTL_ATOMIC_FNS_CALLCONV_IS_FASTCALL)
            // __fastcall: ecx is pl, edx is n

            // Simply atomically add them, which will leave the previous value
            // in edx
            lock xadd dword ptr [ecx], edx

            // Just need to move adx into eax to return it
            mov eax, edx

            ret
#elif defined(WINSTL_ATOMIC_FNS_CALLCONV_IS_STDCALL)
            // __stdcall: arguments are on the stack: pl in esp+4, pl in esp+8

            // Simply atomically add them, which will leave the previous value
            // in edx
            mov ecx, dword ptr [esp + 4]    // Load the address of pl into ecx
            mov eax, dword ptr [esp + 8]    // Load the value into eax, so the return value will be there waiting

            lock xadd dword ptr [ecx], eax

            // Just need to move adx into eax to return it

            ret 8
#else
# error Need to define calling convention
#endif /* call-conv */
        }
    }
}

#ifdef __STLSOFT_COMPILER_IS_BORLAND
# pragma warn .8070     /* Suppresses: "Function should return a value" */
# pragma warn .8002     /* Suppresses: "Restarting compile using assembly" */
#endif /* __STLSOFT_COMPILER_IS_BORLAND */

#  else /* STSLSOFT_INLINE_ASM_SUPPORTED */
/* Non-assembler versions
 *
 * These use the Win32 Interlocked functions. These are not guaranteed to give
 * precise answers on Windows 95.
 */


/* Multi-processor detection variants */
WINSTL_ATOMIC_FNS_IMPL_(ws_sint32_t) atomic_preincrement(ws_sint32_t volatile *pl)
{
    return ::InterlockedIncrement((LPLONG)pl);
}

WINSTL_ATOMIC_FNS_IMPL_(ws_sint32_t) atomic_predecrement(ws_sint32_t volatile *pl)
{
    return ::InterlockedDecrement((LPLONG)pl);
}

WINSTL_ATOMIC_FNS_IMPL_(ws_sint32_t) atomic_postincrement(ws_sint32_t volatile *pl)
{
    ws_sint32_t pre = *pl;

    ::InterlockedIncrement((LPLONG)pl);

    return pre;
}

WINSTL_ATOMIC_FNS_IMPL_(ws_sint32_t) atomic_postdecrement(ws_sint32_t volatile *pl)
{
    ws_sint32_t pre = *pl;

    ::InterlockedDecrement((LPLONG)pl);

    return pre;
}

WINSTL_ATOMIC_FNS_IMPL_(void) atomic_increment(ws_sint32_t volatile *pl)
{
    ::InterlockedIncrement((LPLONG)pl);
}

WINSTL_ATOMIC_FNS_IMPL_(void) atomic_decrement(ws_sint32_t volatile *pl)
{
    ::InterlockedDecrement((LPLONG)pl);
}

WINSTL_ATOMIC_FNS_IMPL_(ws_sint32_t) atomic_write(ws_sint32_t volatile *pl, ws_sint32_t n)
{
    return ::InterlockedExchange((LPLONG)pl, n);
}

WINSTL_ATOMIC_FNS_IMPL_(ws_sint32_t) atomic_read(ws_sint32_t volatile const *pl)
{
    return *pl;
}

WINSTL_ATOMIC_FNS_IMPL_(ws_sint32_t) atomic_postadd(ws_sint32_t volatile *pl, ws_sint32_t n)
{
    return (ws_sint32_t)::InterlockedExchangeAdd((LPLONG)pl, n);
}


/* Uni-processor variants */

WINSTL_ATOMIC_FNS_IMPL_(ws_sint32_t) atomic_preincrement_up(ws_sint32_t volatile *pl)
{
    return atomic_preincrement(pl);
}

WINSTL_ATOMIC_FNS_IMPL_(ws_sint32_t) atomic_predecrement_up(ws_sint32_t volatile *pl)
{
    return atomic_predecrement(pl);
}

WINSTL_ATOMIC_FNS_IMPL_(ws_sint32_t) atomic_postincrement_up(ws_sint32_t volatile *pl)
{
    return atomic_postincrement(pl);
}

WINSTL_ATOMIC_FNS_IMPL_(ws_sint32_t) atomic_postdecrement_up(ws_sint32_t volatile *pl)
{
    return atomic_postdecrement(pl);
}

WINSTL_ATOMIC_FNS_IMPL_(void) atomic_increment_up(ws_sint32_t volatile *pl)
{
    atomic_increment(pl);
}

WINSTL_ATOMIC_FNS_IMPL_(void) atomic_decrement_up(ws_sint32_t volatile *pl)
{
    atomic_decrement(pl);
}

WINSTL_ATOMIC_FNS_IMPL_(ws_sint32_t) atomic_write_up(ws_sint32_t volatile *pl, ws_sint32_t n)
{
    return atomic_write(pl, n);
}

WINSTL_ATOMIC_FNS_IMPL_(ws_sint32_t) atomic_read_up(ws_sint32_t volatile const *pl)
{
    return *pl;
}

WINSTL_ATOMIC_FNS_IMPL_(ws_sint32_t) atomic_postadd_up(ws_sint32_t volatile *pl, ws_sint32_t n)
{
    return (ws_sint32_t)::InterlockedExchangeAdd((LPLONG)pl, n);
}

/* SMP variants */

WINSTL_ATOMIC_FNS_IMPL_(ws_sint32_t) atomic_preincrement_smp(ws_sint32_t volatile *pl)
{
    return atomic_preincrement(pl);
}

WINSTL_ATOMIC_FNS_IMPL_(ws_sint32_t) atomic_predecrement_smp(ws_sint32_t volatile *pl)
{
    return atomic_predecrement(pl);
}

WINSTL_ATOMIC_FNS_IMPL_(ws_sint32_t) atomic_postincrement_smp(ws_sint32_t volatile *pl)
{
    return atomic_postincrement(pl);
}

WINSTL_ATOMIC_FNS_IMPL_(ws_sint32_t) atomic_postdecrement_smp(ws_sint32_t volatile *pl)
{
    return atomic_postdecrement(pl);
}

WINSTL_ATOMIC_FNS_IMPL_(ws_sint32_t) atomic_write_smp(ws_sint32_t volatile *pl, ws_sint32_t n)
{
    return atomic_write(pl, n);
}

WINSTL_ATOMIC_FNS_IMPL_(ws_sint32_t) atomic_read_smp(ws_sint32_t volatile const *pl)
{
    return *pl;
}

WINSTL_ATOMIC_FNS_IMPL_(ws_sint32_t) atomic_postadd_smp(ws_sint32_t volatile *pl, ws_sint32_t n)
{
    return (ws_sint32_t)::InterlockedExchangeAdd((LPLONG)pl, n);
}

#  endif /* STSLSOFT_INLINE_ASM_SUPPORTED */

# endif /* !WINSTL_ATOMIC_FNS_DECLARATION_ONLY */

/* /////////////////////////////////////////////////////////////////////////////
 * Other inline atomic function
 */

inline ws_sint32_t atomic_preadd_up(ws_sint32_t volatile *pl, ws_sint32_t n)
{
    return n + atomic_postadd_up(pl, n);
}

inline void atomic_increment_smp(ws_sint32_t volatile *pl)
{
    atomic_postincrement_smp(pl);
}

inline void atomic_decrement_smp(ws_sint32_t volatile *pl)
{
    atomic_postdecrement_smp(pl);
}

inline ws_sint32_t atomic_preadd_smp(ws_sint32_t volatile *pl, ws_sint32_t n)
{
    return n + atomic_postadd_smp(pl, n);
}

inline ws_sint32_t atomic_preadd(ws_sint32_t volatile *pl, ws_sint32_t n)
{
    return n + atomic_postadd(pl, n);
}

#endif /* !__STLSOFT_DOCUMENTATION_SKIP_SECTION */

/* /////////////////////////////////////////////////////////////////////////////
 * Unit-testing
 */

#ifdef STLSOFT_UNITTEST

namespace unittest
{
    ss_bool_t test_winstl_atomic_functions(unittest_reporter *r)
    {
        using stlsoft::unittest::unittest_initialiser;

        ss_bool_t               bSuccess    =   true;

        unittest_initialiser    init(r, "WinSTL", "atomic_functions", __FILE__);

        ws_sint32_t     i   =   0;

        if(1 != atomic_preincrement_up(&i))
        {
            r->report("atomic_preincrement_up() failed ", __LINE__);
            bSuccess = false;
        }
        if(0 != atomic_predecrement_up(&i))
        {
            r->report("atomic_predecrement_up() failed ", __LINE__);
            bSuccess = false;
        }
        if(0 != atomic_postincrement_up(&i))
        {
            r->report("atomic_postincrement_up() failed ", __LINE__);
            bSuccess = false;
        }
        if(1 != atomic_postdecrement_up(&i))
        {
            r->report("atomic_postdecrement_up() failed ", __LINE__);
            bSuccess = false;
        }
        atomic_increment_up(&i);
        if(1 != i)
        {
            r->report("atomic_increment_up() failed ", __LINE__);
            bSuccess = false;
        }
        atomic_decrement_up(&i);
        if(0 != i)
        {
            r->report("atomic_decrement_up() failed ", __LINE__);
            bSuccess = false;
        }
        if(0 != atomic_write_up(&i, 1968))
        {
            r->report("atomic_write_up() failed ", __LINE__);
            bSuccess = false;
        }
        if(1968 != atomic_read_up(&i))
        {
            r->report("atomic_read_up() failed ", __LINE__);
            bSuccess = false;
        }
        if(1968 != atomic_postadd_up(&i, 35))
        {
            r->report("atomic_postadd_up() failed ", __LINE__);
            bSuccess = false;
        }
        if(1968 != atomic_preadd_up(&i, -35))
        {
            r->report("atomic_preadd_up() failed ", __LINE__);
            bSuccess = false;
        }

        ws_sint32_t     j   =   0;

        if(1 != atomic_preincrement_smp(&j))
        {
            r->report("atomic_preincrement_smp() failed ", __LINE__);
            bSuccess = false;
        }
        if(0 != atomic_predecrement_smp(&j))
        {
            r->report("atomic_predecrement_smp() failed ", __LINE__);
            bSuccess = false;
        }
        if(0 != atomic_postincrement_smp(&j))
        {
            r->report("atomic_postincrement_smp() failed ", __LINE__);
            bSuccess = false;
        }
        if(1 != atomic_postdecrement_smp(&j))
        {
            r->report("atomic_postdecrement_smp() failed ", __LINE__);
            bSuccess = false;
        }
        atomic_increment_smp(&j);
        if(1 != j)
        {
            r->report("atomic_increment_smp() failed ", __LINE__);
            bSuccess = false;
        }
        atomic_decrement_smp(&j);
        if(0 != j)
        {
            r->report("atomic_decrement_smp() failed ", __LINE__);
            bSuccess = false;
        }
        if(0 != atomic_write_smp(&j, 1968))
        {
            r->report("atomic_write_smp() failed ", __LINE__);
            bSuccess = false;
        }
        if(1968 != atomic_read_smp(&j))
        {
            r->report("atomic_read_smp() failed ", __LINE__);
            bSuccess = false;
        }
        if(1968 != atomic_postadd_smp(&j, 35))
        {
            r->report("atomic_postadd_smp() failed ", __LINE__);
            bSuccess = false;
        }
        if(1968 != atomic_preadd_smp(&j, -35))
        {
            r->report("atomic_preadd_smp() failed ", __LINE__);
            bSuccess = false;
        }

        ws_sint32_t     k   =   0;

        if(1 != atomic_preincrement(&k))
        {
            r->report("atomic_preincrement() failed ", __LINE__);
            bSuccess = false;
        }
        if(0 != atomic_predecrement(&k))
        {
            r->report("atomic_predecrement() failed ", __LINE__);
            bSuccess = false;
        }
        if(0 != atomic_postincrement(&k))
        {
            r->report("atomic_postincrement() failed ", __LINE__);
            bSuccess = false;
        }
        if(1 != atomic_postdecrement(&k))
        {
            r->report("atomic_postdecrement() failed ", __LINE__);
            bSuccess = false;
        }
        atomic_increment(&k);
        if(1 != k)
        {
            r->report("atomic_increment() failed ", __LINE__);
            bSuccess = false;
        }
        atomic_decrement(&k);
        if(0 != k)
        {
            r->report("atomic_decrement() failed ", __LINE__);
            bSuccess = false;
        }
        if(0 != atomic_write(&k, 1968))
        {
            r->report("atomic_write() failed ", __LINE__);
            bSuccess = false;
        }
        if(1968 != atomic_read(&k))
        {
            r->report("atomic_read() failed ", __LINE__);
            bSuccess = false;
        }
        if(1968 != atomic_postadd(&k, 35))
        {
            r->report("atomic_postadd() failed ", __LINE__);
            bSuccess = false;
        }
        if(1968 != atomic_preadd(&k, -35))
        {
            r->report("atomic_preadd() failed ", __LINE__);
            bSuccess = false;
        }

        return bSuccess;
    }

    unittest_registrar    unittest_winstl_atomic_functions(test_winstl_atomic_functions);

} // namespace unittest

#endif /* STLSOFT_UNITTEST */

/* ////////////////////////////////////////////////////////////////////////// */

/// @} // end of group winstl_threading_library

/* ////////////////////////////////////////////////////////////////////////// */

#ifndef _WINSTL_NO_NAMESPACE
# if defined(_STLSOFT_NO_NAMESPACE) || \
     defined(__STLSOFT_DOCUMENTATION_SKIP_SECTION)
} // namespace winstl
# else
} // namespace winstl_project
} // namespace stlsoft
# endif /* _STLSOFT_NO_NAMESPACE */
#endif /* !_WINSTL_NO_NAMESPACE */

/* ////////////////////////////////////////////////////////////////////////// */

#endif /* !WINSTL_INCL_H_WINSTL_ATOMIC_FUNCTIONS */

/* ////////////////////////////////////////////////////////////////////////// */
