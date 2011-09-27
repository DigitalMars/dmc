/* /////////////////////////////////////////////////////////////////////////////
 * File:        inetstl.h
 *
 * Purpose:     Root header for the InetSTL libraries. Performs various compiler
 *              and platform discriminations, and definitions of types.
 *
 * Created:     24th April 2004
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


#ifndef INETSTL_INCL_H_INETSTL
#define INETSTL_INCL_H_INETSTL

/* File version */
#ifndef __STLSOFT_DOCUMENTATION_SKIP_SECTION
# define INETSTL_VER_H_INETSTL_MAJOR      2
# define INETSTL_VER_H_INETSTL_MINOR      0
# define INETSTL_VER_H_INETSTL_REVISION   1
# define INETSTL_VER_H_INETSTL_EDIT       10
#endif /* !__STLSOFT_DOCUMENTATION_SKIP_SECTION */

/** \file inetstl.h The root header for the \ref InetSTL project */

/** \weakgroup projects STLSoft Projects
 *
 * \brief The Projects that comprise the STLSoft libraries
 */

/** \defgroup InetSTL InetSTL
 * \ingroup projects
 *
 * \brief <img src = "inetstl32x32.jpg">&nbsp;&nbsp;&nbsp;&nbsp;<i>Where the Standard Template Library meets the WinInet API</i>
 *
 * The philosophy of InetSTL (http://inetstl.org/) is essentially the same as that
 * of the STLSoft (http://stlsoft.org/) organisation: providing robust and
 * lightweight software to the WinInet API development
 * community. InetSTL provides template-based software that builds on that
 * provided by Win and STLSoft in order to reduce programmer effort and increase
 * robustness in the use of the Win.
 *
 * <b>Namespaces</b>
 *
 * The InetSTL namespace <code><b>inetstl</b></code> is actually an alias for the
 * namespace <code><b>stlsoft::inetstl_project</b></code>, and as such all the
 * InetSTL project components actually reside within the
 * <code><b>stlsoft</b></code> namespace. However, there is never any need to
 * use the <code><b>stlsoft::inetstl_project</b></code> namespace in your code,
 * and you should always use the alias <code><b>inetstl</b></code>.
 *
 * <b>Dependencies</b>
 *
 * As with <b><i>all</i></b> parts of the STLSoft libraries, there are no
 * dependencies on InetSTL binary components and no need to compile InetSTL
 * implementation files; InetSTL is <b>100%</b> header-only!
 *
 * As with most of the STLSoft sub-projects, InetSTL depends only on:
 *
 * - Selected headers from the C standard library, such as  <code><b>wchar.h</b></code>
 * - Selected headers from the C++ standard library, such as <code><b>new</b></code>, <code><b>functional</b></code>
 * - Selected header files of the STLSoft main project
 * - The header files particular to the technology area, in this case the WinInet API library headers, such as <code><b>objbase.h</b></code>
 * - The binary (static and dynamic libraries) components particular to the technology area, in this case the WinInet API libraries that ship with the operating system and your compiler(s)
 *
 * In addition, some parts of the libraries exhibit different behaviour when
 * translated in different contexts, such as the value of <code><b>_WIN32_WINNT</b></code>,
 * or with <code><b>ntsecapi.h</b></code> include. In <b><i>all</i></b>
 * cases the libraries function correctly in whatever context they are compiled.
 */

/* /////////////////////////////////////////////////////////////////////////////
 * InetSTL version
 *
 * The libraries version information is comprised of major, minor and revision
 * components.
 *
 * The major version is denoted by the _INETSTL_VER_MAJOR preprocessor symbol.
 * A changes to the major version component implies that a dramatic change has
 * occurred in the libraries, such that considerable changes to source dependent
 * on previous versions would need to be effected.
 *
 * The minor version is denoted by the _INETSTL_VER_MINOR preprocessor symbol.
 * Changes to the minor version component imply that a significant change has
 * occurred to the libraries, either in the addition of new functionality or in
 * the destructive change to one or more components such that recomplilation and
 * code change may be necessitated.
 *
 * The revision version is denoted by the _INETSTL_VER_REVISIO preprocessor
 * symbol. Changes to the revision version component imply that a bug has been
 * fixed. Dependent code should be recompiled in order to pick up the changes.
 *
 * In addition to the individual version symbols - _INETSTL_VER_MAJOR,
 * _INETSTL_VER_MINOR and _INETSTL_VER_REVISION - a composite symbol _INETSTL_VER
 * is defined, where the upper 8 bits are 0, bits 16-23 represent the major
 * component,  bits 8-15 represent the minor component, and bits 0-7 represent
 * the revision component.
 *
 * Each release of the libraries will bear a different version, and that version
 * will also have its own symbol: Version 1.0.1 specifies _INETSTL_VER_1_0_1.
 *
 * Thus the symbol _INETSTL_VER may be compared meaningfully with a specific
 * version symbol, e.g. #if _INETSTL_VER >= _INETSTL_VER_1_0_1
 */

/// \def _INETSTL_VER_MAJOR
/// The major version number of InetSTL

/// \def _INETSTL_VER_MINOR
/// The minor version number of InetSTL

/// \def _INETSTL_VER_REVISION
/// The revision version number of InetSTL

/// \def _INETSTL_VER
/// The current composite version number of InetSTL

#define _INETSTL_VER_MAJOR       1
#define _INETSTL_VER_MINOR       0
#define _INETSTL_VER_REVISION    2
#ifndef __STLSOFT_DOCUMENTATION_SKIP_SECTION
# define _INETSTL_VER_1_0_1      0x00010001  /*!< Version 1.0.1 */
# define _INETSTL_VER_1_0_2      0x00010002  /*!< Version 1.0.2 */
#endif /* !__STLSOFT_DOCUMENTATION_SKIP_SECTION */

#define _INETSTL_VER             _INETSTL_VER_1_0_2

/* /////////////////////////////////////////////////////////////////////////////
 * Includes
 */

/* Strict */
#ifndef __STLSOFT_DOCUMENTATION_SKIP_SECTION
# ifndef STRICT
#  if defined(_INETSTL_STRICT) || \
      (   !defined(_INETSTL_NO_STRICT) && \
          !defined(NO_STRICT))
#   define STRICT 1
#  endif /* !NO_STRICT && !_INETSTL_NO_STRICT */
# endif /* STRICT */
#endif /* !__STLSOFT_DOCUMENTATION_SKIP_SECTION */

#ifndef STLSOFT_INCL_H_STLSOFT
# include "stlsoft.h"   // STLSoft root header
#endif /* !STLSOFT_INCL_H_STLSOFT */
//#if defined(__STLSOFT_COMPILER_IS_BORLAND)
# include <windows.h>
//#endif /* compiler */
#include <wininet.h>    // Windows base header

/* /////////////////////////////////////////////////////////////////////////////
 * STLSoft version compatibility
 */

#if !defined(_STLSOFT_VER_1_7_1) || \
    _STLSOFT_VER < _STLSOFT_VER_1_7_1
# error This version of the InetSTL libraries requires STLSoft version 1.7.1 or later
#endif /* _STLSOFT_VER < _STLSOFT_VER_1_5_1 */

/* /////////////////////////////////////////////////////////////////////////////
 * Sanity checks
 *
 * Win32    -   must be compiled in context of Win32 API
 * MBCS     -   none of the libraries' code is written to support MBCS
 */

/* Must be Win32 api. */
#if !defined(WIN32) && \
    !defined(_WIN32)
# error The InetSTL libraries is currently only compatible with the Win32 API
#endif /* !WIN32 && !_WIN32 */

/* Should not be MBCS. */
#ifdef _MBCS
# ifdef _INETSTL_STRICT
#  error The InetSTL libraries are not compatible with variable length character representation schemes such as MBCS
# else
#  ifdef _STLSOFT_COMPILE_VERBOSE
#   pragma message("The InetSTL libraries are not compatible with variable length character representation schemes such as MBCS")
#  endif /* _STLSOFT_COMPILE_VERBOSE */
# endif /* _INETSTL_STRICT */
#endif /* _MBCS */

/* /////////////////////////////////////////////////////////////////////////////
 * Compiler compatibility
 *
 * Currently the only compilers supported by the InetSTL libraries are
 *
 * Borland C++ 5.5, 5.51, 5.6
 * Digital Mars C/C++ 8.26 - 8.32
 * Metrowerks 2.4 & 3.0 (CodeWarrior 7.0 & 8.0)
 * Intel C/C++ 6.0 & 7.0
 * Visual C++ 4.2, 5.0, 6.0, 7.0
 * Watcom C/C++ 11.0
 */

#if defined(__STLSOFT_COMPILER_IS_BORLAND)
/* Borland C++ */
# if __BORLANDC__ < 0x0550
#  error Versions of Borland C++ prior to 5.5 are not supported by the InetSTL libraries
# endif /* __BORLANDC__ */

#elif defined(__STLSOFT_COMPILER_IS_COMO)
/* Comeau C++ */
# if __COMO_VERSION__ < 4300
#  error Versions of Comeau C++ prior to 4.3 are not supported by the InetSTL libraries
# endif /* __COMO_VERSION__ */

#elif defined(__STLSOFT_COMPILER_IS_DMC)
/* Digital Mars C/C++ */
# if __DMC__ < 0x0826
#  error Versions of Digital Mars C/C++ prior to 8.26 are not supported by the InetSTL libraries
# endif /* __DMC__ */

#elif defined(__STLSOFT_COMPILER_IS_GCC)
/* GNU C/C++ */
# if __GNUC__ < 2 || \
     (  __GNUC__ == 2 && \
        __GNUC_MINOR__ < 95)
#  error Versions of GNU C/C++ prior to 2.95 are not supported by the InetSTL libraries
# endif /* __GNUC__ */

#elif defined(__STLSOFT_COMPILER_IS_INTEL)
/* Intel C++ */
# if (__INTEL_COMPILER < 600)
#  error Versions of Intel C++ prior to 6.0 are not supported by the InetSTL libraries
# endif /* __INTEL_COMPILER */

#elif defined(__STLSOFT_COMPILER_IS_MWERKS)
/* Metrowerks C++ */
# if (__MWERKS__ & 0xFF00) < 0x2400
#  error Versions of Metrowerks CodeWarrior C++ prior to 7.0 are not supported by the InetSTL libraries
# endif /* __MWERKS__ */

#elif defined(__STLSOFT_COMPILER_IS_MSVC)
/* Visual C++ */
# if _MSC_VER < 1020
#  error Versions of Visual C++ prior to 4.2 are not supported by the InetSTL libraries
# endif /* _MSC_VER */

#elif defined(__STLSOFT_COMPILER_IS_VECTORC)
/* VectorC C/C++ */

#elif defined(__STLSOFT_COMPILER_IS_WATCOM)
/* Watcom C/C++ */
# if (__WATCOMC__ < 1200)
#  error Versions of Watcom C/C++ prior to 12.0 are not supported by the InetSTL libraries
# endif /* __WATCOMC__ */

#else
/* No recognised compiler */
# ifdef _STLSOFT_FORCE_ANY_COMPILER
#  define _INETSTL_COMPILER_IS_UNKNOWN
#  ifdef _STLSOFT_COMPILE_VERBOSE
#   pragma message("Compiler is unknown to InetSTL")
#  endif /* _STLSOFT_COMPILE_VERBOSE */
# else
#  error Currently only Borland C++, Digital Mars C/C++, Intel C/C++, Metrowerks CodeWarrior and Visual C++ compilers are supported by the InetSTL libraries
# endif /* _STLSOFT_FORCE_ANY_COMPILER */
#endif /* compiler */

/* /////////////////////////////////////////////////////////////////////////////
 * Debugging
 *
 * The macro inetstl_assert provides standard debug-mode assert functionality.
 */

/// Defines a runtime assertion
///
/// \param expr Must be non-zero, or an assertion will be fired
#define inetstl_assert(expr)                stlsoft_assert(expr)

/// Defines a runtime assertion, with message
///
/// \param expr Must be non-zero, or an assertion will be fired
/// \param msg The literal character string message to be included in the assertion
#define inetstl_message_assert(msg, expr)   stlsoft_message_assert(msg, expr)

/// Defines a compile-time assertion
///
/// \param expr Must be non-zero, or compilation will fail
#define inetstl_static_assert(expr)         stlsoft_static_assert(expr)

/* /////////////////////////////////////////////////////////////////////////////
 * Namespace
 *
 * The InetSTL components are contained within the inetstl namespace. This is
 * usually an alias for stlsoft::inetstl_project,
 *
 * When compilers support namespaces they are defined by default. They can be
 * undefined using a cascasing system, as follows:
 *
 * If _STLSOFT_NO_NAMESPACES is defined, then _INETSTL_NO_NAMESPACES is defined.
 *
 * If _INETSTL_NO_NAMESPACES is defined, then _INETSTL_NO_NAMESPACE is defined.
 *
 * If _INETSTL_NO_NAMESPACE is defined, then the InetSTL constructs are defined
 * in the global scope.
 *
 * If _STLSOFT_NO_NAMESPACES, _INETSTL_NO_NAMESPACES and _INETSTL_NO_NAMESPACE are
 * all undefined but the symbol _STLSOFT_NO_NAMESPACE is defined (whence the
 * namespace stlsoft does not exist), then the InetSTL constructs are defined
 * within the inetstl namespace. The definition matrix is as follows:
 *
 * _STLSOFT_NO_NAMESPACE    _INETSTL_NO_NAMESPACE    inetstl definition
 * ---------------------    --------------------    -----------------
 *  not defined              not defined             = stlsoft::inetstl_project
 *  not defined              defined                 not defined
 *  defined                  not defined             inetstl
 *  defined                  defined                 not defined
 *
 *
 *
 * The macro inetstl_ns_qual() macro can be used to refer to elements in the
 * InetSTL libraries irrespective of whether they are in the
 * stlsoft::inetstl_project (or inetstl) namespace or in the global namespace.
 *
 * Furthermore, some compilers do not support the standard library in the std
 * namespace, so the inetstl_ns_qual_std() macro can be used to refer to elements
 * in the InetSTL libraries irrespective of whether they are in the std namespace
 * or in the global namespace.
 */

/* No STLSoft namespaces means no InetSTL namespaces */
#ifdef _STLSOFT_NO_NAMESPACES
# define _INETSTL_NO_NAMESPACES
#endif /* _STLSOFT_NO_NAMESPACES */

/* No InetSTL namespaces means no inetstl namespace */
#ifdef _INETSTL_NO_NAMESPACES
# define _INETSTL_NO_NAMESPACE
#endif /* _INETSTL_NO_NAMESPACES */

#ifndef _INETSTL_NO_NAMESPACE
# if defined(_STLSOFT_NO_NAMESPACE) || \
     defined(__STLSOFT_DOCUMENTATION_SKIP_SECTION)
/* There is no stlsoft namespace, so must define ::inetstl */
/// The InetSTL namespace - \c inetstl (aliased to \c stlsoft::inetstl_project) - is
/// the namespace for the InetSTL project.
namespace inetstl
{
# else
/* Define stlsoft::inetstl_project */

namespace stlsoft
{

namespace inetstl_project
{

# endif /* _STLSOFT_NO_NAMESPACE */
#else
stlsoft_ns_using(move_lhs_from_rhs)
#endif /* !_INETSTL_NO_NAMESPACE */

/// \def inetstl_ns_qual(x)
/// Qualifies with <b>inetstl::</b> if InetSTL is using namespaces or, if not, does not qualify

/// \def inetstl_ns_using(x)
/// Declares a using directive (with respect to <b>inetstl</b>) if InetSTL is using namespaces or, if not, does nothing

#ifndef _INETSTL_NO_NAMESPACE
# define inetstl_ns_qual(x)          ::inetstl::x
# define inetstl_ns_using(x)         using ::inetstl::x;
#else
# define inetstl_ns_qual(x)          x
# define inetstl_ns_using(x)
#endif /* !_INETSTL_NO_NAMESPACE */

/// \def inetstl_ns_qual_std(x)
/// Qualifies with <b>std::</b> if InetSTL is being translated in the context of the standard library being within the <b>std</b> namespace or, if not, does not qualify

/// \def inetstl_ns_using_std(x)
/// Declares a using directive (with respect to <b>std</b>) if InetSTL is being translated in the context of the standard library being within the <b>std</b> namespace or, if not, does nothing

#ifdef __STLSOFT_CF_std_NAMESPACE
# define inetstl_ns_qual_std(x)      ::std::x
# define inetstl_ns_using_std(x)     using ::std::x;
#else
# define inetstl_ns_qual_std(x)      x
# define inetstl_ns_using_std(x)
#endif /* !__STLSOFT_CF_std_NAMESPACE */

/* /////////////////////////////////////////////////////////////////////////////
 * Typedefs
 *
 * The InetSTL uses a number of typedefs to aid in compiler-independence in the
 * libraries' main code.
 */

typedef stlsoft_ns_qual(ss_char_a_t)        is_char_a_t;    //!< Ansi char type
typedef stlsoft_ns_qual(ss_char_w_t)        is_char_w_t;    //!< Unicode char type
typedef stlsoft_ns_qual(ss_sint8_t)         is_sint8_t;     //!< 8-bit signed integer
typedef stlsoft_ns_qual(ss_uint8_t)         is_uint8_t;     //!< 8-bit unsigned integer
typedef stlsoft_ns_qual(ss_int16_t)         is_int16_t;     //!< 16-bit integer
typedef stlsoft_ns_qual(ss_sint16_t)        is_sint16_t;    //!< 16-bit signed integer
typedef stlsoft_ns_qual(ss_uint16_t)        is_uint16_t;    //!< 16-bit unsigned integer
typedef stlsoft_ns_qual(ss_int32_t)         is_int32_t;     //!< 32-bit integer
typedef stlsoft_ns_qual(ss_sint32_t)        is_sint32_t;    //!< 32-bit signed integer
typedef stlsoft_ns_qual(ss_uint32_t)        is_uint32_t;    //!< 32-bit unsigned integer
#ifdef STLSOFT_CF_64BIT_INT_SUPPORT
 typedef stlsoft_ns_qual(ss_int64_t)        is_int64_t;     //!< 64-bit integer
 typedef stlsoft_ns_qual(ss_sint64_t)       is_sint64_t;    //!< 64-bit signed integer
 typedef stlsoft_ns_qual(ss_uint64_t)       is_uint64_t;    //!< 64-bit unsigned integer
#endif /* STLSOFT_CF_64BIT_INT_SUPPORT */
typedef stlsoft_ns_qual(ss_int_t)           is_int_t;       //!< integer
typedef stlsoft_ns_qual(ss_sint_t)          is_sint_t;      //!< signed integer
typedef stlsoft_ns_qual(ss_uint_t)          is_uint_t;      //!< unsigned integer
typedef stlsoft_ns_qual(ss_long_t)          is_long_t;      //!< long
typedef stlsoft_ns_qual(ss_byte_t)          is_byte_t;      //!< Byte
typedef stlsoft_ns_qual(ss_bool_t)          is_bool_t;      //!< bool
typedef DWORD                               is_dword_t;     //!< dword
typedef stlsoft_ns_qual(ss_size_t)          is_size_t;      //!< size
typedef stlsoft_ns_qual(ss_ptrdiff_t)       is_ptrdiff_t;   //!< ptr diff
typedef stlsoft_ns_qual(ss_streampos_t)     is_streampos_t; //!< streampos
typedef stlsoft_ns_qual(ss_streamoff_t)     is_streamoff_t; //!< streamoff

#ifndef _INETSTL_NO_NAMESPACE
typedef is_char_a_t         char_a_t;           //!< Ansi char type
typedef is_char_w_t         char_w_t;           //!< Unicode char type
//typedef is_int8_t           int8_t;             //!< 8-bit integer
typedef is_sint8_t          sint8_t;            //!< 8-bit signed integer
typedef is_uint8_t          uint8_t;            //!< 8-bit unsigned integer
typedef is_int16_t          int16_t;            //!< 16-bit integer
typedef is_sint16_t         sint16_t;           //!< 16-bit signed integer
typedef is_uint16_t         uint16_t;           //!< 16-bit unsigned integer
typedef is_int32_t          int32_t;            //!< 32-bit integer
typedef is_sint32_t         sint32_t;           //!< 32-bit signed integer
typedef is_uint32_t         uint32_t;           //!< 32-bit unsigned integer
# ifdef STLSOFT_CF_64BIT_INT_SUPPORT
 typedef is_int64_t         int64_t;            //!< 64-bit integer
 typedef is_sint64_t        sint64_t;           //!< 64-bit signed integer
 typedef is_uint64_t        uint64_t;           //!< 64-bit unsigned integer
# endif /* STLSOFT_CF_64BIT_INT_SUPPORT */
//typedef is_short_t          short_t;            //!< short integer
typedef is_int_t            int_t;              //!< integer
typedef is_sint_t           sint_t;             //!< signed integer
typedef is_uint_t           uint_t;             //!< unsigned integer
typedef is_long_t           long_t;             //!< long integer
typedef is_byte_t           byte_t;             //!< Byte
typedef is_bool_t           bool_t;             //!< bool
typedef is_dword_t          dword_t;            //!< dword
# if !defined(__STLSOFT_COMPILER_IS_DMC)
typedef is_size_t           size_t;             //!< size
typedef is_ptrdiff_t        ptrdiff_t;          //!< ptr diff
typedef is_streampos_t      streampos_t;        //!< streampos
typedef is_streamoff_t      streamoff_t;        //!< streamoff
# endif /* compiler */
#endif /* !_INETSTL_NO_NAMESPACE */

#ifndef __STLSOFT_DOCUMENTATION_SKIP_SECTION
/* /////////////////////////////////////////////////////////////////////////////
 * Values
 *
 * Since the boolean type may not be supported natively on all compilers, the
 * values of true and false may also not be provided. Hence the values of
 * is_true_v and is_false_v are defined, and are used in all code.
 */

#define is_true_v       ss_true_v
#define is_false_v      ss_false_v

#endif /* !__STLSOFT_DOCUMENTATION_SKIP_SECTION */
/* /////////////////////////////////////////////////////////////////////////////
 * Code modification macros
 */

#ifndef __STLSOFT_DOCUMENTATION_SKIP_SECTION
/* Exception signatures. */
# define inetstl_throw_0()                               stlsoft_throw_0()
# define inetstl_throw_1(x1)                             stlsoft_throw_1(x1)
# define inetstl_throw_2(x1, x2)                         stlsoft_throw_2(x1, x2)
# define inetstl_throw_3(x1, x2, x3)                     stlsoft_throw_3(x1, x2, x3)
# define inetstl_throw_4(x1, x2, x3, x4)                 stlsoft_throw_4(x1, x2, x3, x4)
# define inetstl_throw_5(x1, x2, x3, x4, x5)             stlsoft_throw_5(x1, x2, x3, x4, x5)
# define inetstl_throw_6(x1, x2, x3, x4, x5, x6)         stlsoft_throw_6(x1, x2, x3, x4, x5, x6)
# define inetstl_throw_7(x1, x2, x3, x4, x5, x6, x7)     stlsoft_throw_7(x1, x2, x3, x4, x5, x6, x7)
# define inetstl_throw_8(x1, x2, x3, x4, x5, x6, x7, x8) stlsoft_throw_8(x1, x2, x3, x4, x5, x6, x7, x8)
#endif /* !__STLSOFT_DOCUMENTATION_SKIP_SECTION */

/// Evaluates, at compile time, to the number of elements within the given vector entity
#define inetstl_num_elements(ar)                         stlsoft_num_elements(ar)

/// Destroys the given instance \c p of the given type (\c t and \c _type)
#define inetstl_destroy_instance(t, _type, p)            stlsoft_destroy_instance(t, _type, p)

/// Generates an opaque type with the name \c _htype
#define inetstl_gen_opaque(_htype)                       stlsoft_gen_opaque(_htype)

/* ////////////////////////////////////////////////////////////////////////// */

#ifndef _INETSTL_NO_NAMESPACE
# if defined(_STLSOFT_NO_NAMESPACE) || \
     defined(__STLSOFT_DOCUMENTATION_SKIP_SECTION)
} // namespace inetstl
# else
} // namespace inetstl_project
} // namespace stlsoft
namespace inetstl = ::stlsoft::inetstl_project;
# endif /* _STLSOFT_NO_NAMESPACE */
#endif /* !_INETSTL_NO_NAMESPACE */

/* ////////////////////////////////////////////////////////////////////////// */

#endif /* INETSTL_INCL_H_INETSTL */

/* ////////////////////////////////////////////////////////////////////////// */
