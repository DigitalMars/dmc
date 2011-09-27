/* /////////////////////////////////////////////////////////////////////////////
 * File:        wtlstl.h
 *
 * Purpose:     Root header for the WTLSTL libraries. Performs various compiler
 *              and platform discriminations, and definitions of types.
 *
 * Created:     12th May 2003
 * Updated:     9th September 2004
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


#ifndef WTLSTL_INCL_H_WTLSTL
#define WTLSTL_INCL_H_WTLSTL

/* File version */
#ifndef __STLSOFT_DOCUMENTATION_SKIP_SECTION
#define _WTLSTL_VER_H_WTLSTL_MAJOR      1
#define _WTLSTL_VER_H_WTLSTL_MINOR      1
#define _WTLSTL_VER_H_WTLSTL_REVISION   3
#define _WTLSTL_VER_H_WTLSTL_EDIT       10
#endif /* !__STLSOFT_DOCUMENTATION_SKIP_SECTION */

/** \file wtlstl.h The root header for the \ref WTLSTL project */

/** \weakgroup projects STLSoft Projects
 *
 * \brief The Projects that comprise the STLSoft libraries
 */

/** \defgroup WTLSTL WTLSTL
 * \ingroup projects
 *
 * \brief <img src = "wtlstl32x32.jpg">&nbsp;&nbsp;&nbsp;&nbsp;<i>Template Software for the Window Template Library</i>
 *
 * The philosophy of WTLSTL (http://wtlstl.org/) is essentially the same as that
 * of the STLSoft (http://stlsoft.org/) organisation: providing robust and
 * lightweight software to the Window Template Library (WTL) development
 * community. WTLSTL provides template-based software that builds on that
 * provided by ATL and STLSoft in order to reduce programmer effort and increase
 * robustness in the use of the ATL.
 *
 * <b>Namespaces</b>
 *
 * The WTLSTL namespace <code><b>wtlstl</b></code> is actually an alias for the
 * namespace <code><b>stlsoft::wtlstl_project</b></code>, and as such all the
 * WTLSTL project components actually reside within the
 * <code><b>stlsoft</b></code> namespace. However, there is never any need to
 * use the <code><b>stlsoft::wtlstl_project</b></code> namespace in your code,
 * and you should always use the alias <code><b>wtlstl</b></code>.
 *
 * <b>Dependencies</b>
 *
 * As with <b><i>all</i></b> parts of the STLSoft libraries, there are no
 * dependencies on WTLSTL binary components and no need to compile WTLSTL
 * implementation files; WTLSTL is <b>100%</b> header-only!
 *
 * As with most of the STLSoft sub-projects, WTLSTL depends only on:
 *
 * - Selected headers from the C standard library, such as  <code><b>wchar.h</b></code>
 * - Selected headers from the C++ standard library, such as <code><b>new</b></code>, <code><b>functional</b></code>
 * - Selected header files of the STLSoft main project
 * - The header files particular to the technology area, in this case the ATL library headers, such as <code><b>atlbase.h</b></code>
 * - The binary (static and dynamic libraries) components particular to the technology area, in this case the ATL libraries that ship with the operating system and your compiler(s)
 */

/* /////////////////////////////////////////////////////////////////////////////
 * WTLSTL version
 *
 * The libraries version information is comprised of major, minor and revision
 * components.
 *
 * The major version is denoted by the _WTLSTL_VER_MAJOR preprocessor symbol.
 * A changes to the major version component implies that a dramatic change has
 * occurred in the libraries, such that considerable changes to source dependent
 * on previous versions would need to be effected.
 *
 * The minor version is denoted by the _WTLSTL_VER_MINOR preprocessor symbol.
 * Changes to the minor version component imply that a significant change has
 * occurred to the libraries, either in the addition of new functionality or in
 * the destructive change to one or more components such that recomplilation and
 * code change may be necessitated.
 *
 * The revision version is denoted by the _WTLSTL_VER_REVISIO preprocessor
 * symbol. Changes to the revision version component imply that a bug has been
 * fixed. Dependent code should be recompiled in order to pick up the changes.
 *
 * In addition to the individual version symbols - _WTLSTL_VER_MAJOR,
 * _WTLSTL_VER_MINOR and _WTLSTL_VER_REVISION - a composite symbol _WTLSTL_VER
 * is defined, where the upper 8 bits are 0, bits 16-23 represent the major
 * component,  bits 8-15 represent the minor component, and bits 0-7 represent
 * the revision component.
 *
 * Each release of the libraries will bear a different version, and that version
 * will also have its own symbol: Version 1.0.1 specifies _WTLSTL_VER_1_0_1.
 *
 * Thus the symbol _WTLSTL_VER may be compared meaningfully with a specific
 * version symbol, e.g. #if _WTLSTL_VER >= _WTLSTL_VER_1_0_1
 */

/// \def _WTLSTL_VER_MAJOR
/// The major version number of WTLSTL

/// \def _WTLSTL_VER_MINOR
/// The minor version number of WTLSTL

/// \def _WTLSTL_VER_REVISION
/// The revision version number of WTLSTL

/// \def _WTLSTL_VER
/// The current composite version number of WTLSTL

#define _WTLSTL_VER_MAJOR       1
#define _WTLSTL_VER_MINOR       1
#define _WTLSTL_VER_REVISION    1
#define _WTLSTL_VER_1_0_1       0x00010001  /*!< Version 1.0.1 */
#define _WTLSTL_VER_1_1_1       0x00010101  /*!< Version 1.1.1 */

#define _WTLSTL_VER             _WTLSTL_VER_1_1_1

/* /////////////////////////////////////////////////////////////////////////////
 * Includes
 */

#ifndef STLSOFT_INCL_H_STLSOFT
# include "stlsoft.h"   // STLSoft root header
#endif /* !STLSOFT_INCL_H_STLSOFT */

/* In order to be compatible with Visual C++ 5.0, we need to define a few
 * things before we include the WTL headers to convince WTL 3.1 that we are
 * valid.
 */

#if defined(__STLSOFT_COMPILER_IS_MSVC) && \
    _MSC_VER < 1200

# ifndef MIDL_INTERFACE
#  define MIDL_INTERFACE(x)         struct
# endif /* !MIDL_INTERFACE */

# ifndef LONG_PTR
#  define LONG_PTR                  LONG
# endif /* !LONG_PTR */

# ifndef UINT_PTR
#  define UINT_PTR                  unsigned int
# endif /* !UINT_PTR */

# ifndef _ATL_NO_MSIMG
#  define _ATL_NO_MSIMG
# endif /* !_ATL_NO_MSIMG */

# ifndef ATLASSERT
#  define ATLASSERT                 _ASSERTE
# endif /* !ATLASSERT */

# ifndef _WIN32_IE
#  define _WIN32_IE                 0x0300
# endif /* !_WIN32_IE */

#endif /* _MSC_VER < 1200 */

/* Now we include AtlApp.h here */

#include <atlapp.h>    // WTL base header

#if !defined(_WTL_VER) || \
    _WTL_VER < 0x0310
# error WTLSTL supports WTL version 3.1 and above only
#endif /* _WTL_VER < 0x0310 */

/* /////////////////////////////////////////////////////////////////////////////
 * STLSoft version compatibility
 */

#if !defined(_STLSOFT_VER_1_6_3) || \
    _STLSOFT_VER < _STLSOFT_VER_1_6_3
# error This version of the WTLSTL libraries requires STLSoft version 1.6.3 or later
#endif /* _STLSOFT_VER < _STLSOFT_VER_1_6_3 */

/* /////////////////////////////////////////////////////////////////////////////
 * Compiler compatibility
 *
 * Currently the only compilers supported by the WTLSTL libraries are
 *
 * Borland C++ 5.5, 5.51, 5.6
 * Digital Mars C/C++ 8.26 - 8.32
 * Metrowerks 2.4 & 3.0 (CodeWarrior 7.0 & 8.0)
 * Intel C/C++ 6.0
 * Visual C++ 5.0, 6.0, 7.0
 */

#if defined(__STLSOFT_COMPILER_IS_BORLAND)
/* Borland C++ */
# if __BORLANDC__ < 0x0550
#  error Versions of Borland C++ prior to 5.5 are not supported by the WTLSTL libraries
# endif /* __BORLANDC__ */

#elif defined(__STLSOFT_COMPILER_IS_DMC)
/* Digital Mars C/C++ */
# if __DMC__ < 0x0826
#  error Versions of Digital Mars C/C++ prior to 8.26 are not supported by the WTLSTL libraries
# endif /* __DMC__ */

#elif defined(__STLSOFT_COMPILER_IS_INTEL)
/* Intel C++ */
# if (__INTEL_COMPILER < 600)
#  error Versions of Intel C++ prior to 6.0 are not supported by the WTLSTL libraries
# endif /* __INTEL_COMPILER */

#elif defined(__STLSOFT_COMPILER_IS_MWERKS)
/* Metrowerks C++ */
# if (__MWERKS__ & 0xFF00) < 0x2400
#  error Versions of Metrowerks CodeWarrior C++ prior to 7.0 are not supported by the WTLSTL libraries
# endif /* __MWERKS__ */

#elif defined(__STLSOFT_COMPILER_IS_MSVC)
/* Visual C++ */
# if _MSC_VER < 1100
#  error Versions of Visual C++ prior to 5.0 are not supported by the WTLSTL libraries
# endif /* _MSC_VER */

#else
/* No recognised compiler */
# ifdef _STLSOFT_FORCE_ANY_COMPILER
#  define _WTLSTL_COMPILER_IS_UNKNOWN
#  ifdef _STLSOFT_COMPILE_VERBOSE
#   pragma message("Compiler is unknown to WTLSTL")
#  endif /* _STLSOFT_COMPILE_VERBOSE */
# else
#  error Currently only Borland C++, Digital Mars C/C++, Intel C/C++, Metrowerks CodeWarrior and Visual C++ compilers are supported by the WTLSTL libraries
# endif /* _STLSOFT_FORCE_ANY_COMPILER */
#endif /* compiler */

/* /////////////////////////////////////////////////////////////////////////////
 * Debugging
 *
 * The macro wtlstl_assert provides standard debug-mode assert functionality.
 */

/// Defines a runtime assertion
///
/// \param _x Must be non-zero, or an assertion will be fired
#define wtlstl_assert(_x)               stlsoft_assert(_x)

/// Defines a runtime assertion, with message
///
/// \param _x Must be non-zero, or an assertion will be fired
/// \param _m The literal character string message to be included in the assertion
#define wtlstl_message_assert(_m, _x)   stlsoft_message_assert(_m, _x)

/// Defines a compile-time assertion
///
/// \param _x Must be non-zero, or compilation will fail
#define wtlstl_static_assert(_x)        stlsoft_static_assert(_x)

/* /////////////////////////////////////////////////////////////////////////////
 * Namespace
 *
 * The WTLSTL components are contained within the wtlstl namespace. This is
 * usually an alias for stlsoft::wtlstl_project,
 *
 * When compilers support namespaces they are defined by default. They can be
 * undefined using a cascasing system, as follows:
 *
 * If _STLSOFT_NO_NAMESPACES is defined, then _WTLSTL_NO_NAMESPACES is defined.
 *
 * If _WTLSTL_NO_NAMESPACES is defined, then _WTLSTL_NO_NAMESPACE is defined.
 *
 * If _WTLSTL_NO_NAMESPACE is defined, then the WTLSTL constructs are defined
 * in the global scope.
 *
 * If _STLSOFT_NO_NAMESPACES, _WTLSTL_NO_NAMESPACES and _WTLSTL_NO_NAMESPACE are
 * all undefined but the symbol _STLSOFT_NO_NAMESPACE is defined (whence the
 * namespace stlsoft does not exist), then the WTLSTL constructs are defined
 * within the wtlstl namespace. The definition matrix is as follows:
 *
 * _STLSOFT_NO_NAMESPACE    _WTLSTL_NO_NAMESPACE    wtlstl definition
 * ---------------------    --------------------    -----------------
 *  not defined              not defined             = stlsoft::wtlstl_project
 *  not defined              defined                 not defined
 *  defined                  not defined             wtlstl
 *  defined                  defined                 not defined
 *
 *
 *
 * The macro wtlstl_ns_qual() macro can be used to refer to elements in the
 * WTLSTL libraries irrespective of whether they are in the
 * stlsoft::wtlstl_project (or wtlstl) namespace or in the global namespace.
 *
 * Furthermore, some compilers do not support the standard library in the std
 * namespace, so the wtlstl_ns_qual_std() macro can be used to refer to elements
 * in the WTLSTL libraries irrespective of whether they are in the std namespace
 * or in the global namespace.
 *
 *
 * The ATL libraries themselves may or may not be defined within the ATL
 * namespace, so the wtlstl_ns_qual_stl() macro can be used to refer to
 * elements in the WTLSTL libraries irrespective of whether they are in the std
 * namespace or in the global namespace.
 */

/* No STLSoft namespaces means no WTLSTL namespaces */
#ifdef _STLSOFT_NO_NAMESPACES
# define _WTLSTL_NO_NAMESPACES
#endif /* _STLSOFT_NO_NAMESPACES */

/* No WTLSTL namespaces means no wtlstl namespace */
#ifdef _WTLSTL_NO_NAMESPACES
# define _WTLSTL_NO_NAMESPACE
#endif /* _WTLSTL_NO_NAMESPACES */

#ifndef _WTLSTL_NO_NAMESPACE
# if defined(_STLSOFT_NO_NAMESPACE) || \
     defined(__STLSOFT_DOCUMENTATION_SKIP_SECTION)
/* There is no stlsoft namespace, so must define ::wtlstl */
namespace wtlstl
{
# else
/* Define stlsoft::wtlstl_project */

namespace stlsoft
{

/// The WTLSTL namespace - \c wtlstl (aliased to \c stlsoft::wtlstl_project) - is
/// the namespace for the WTLSTL project.
namespace wtlstl_project
{

# endif /* _STLSOFT_NO_NAMESPACE */
#else
stlsoft_ns_using(move_lhs_from_rhs)
#endif /* !_WTLSTL_NO_NAMESPACE */

/// \def wtlstl_ns_qual(x)
/// Qualifies with <b>wtlstl::</b> if WTLSTL is using namespaces or, if not, does not qualify

/// \def wtlstl_ns_using(x)
/// Declares a using directive (with respect to <b>wtlstl</b>) if WTLSTL is using namespaces or, if not, does nothing

#ifndef _WTLSTL_NO_NAMESPACE
# define wtlstl_ns_qual(x)          ::wtlstl::x
# define wtlstl_ns_using(x)         using ::wtlstl::x;
#else
# define wtlstl_ns_qual(x)          x
# define wtlstl_ns_using(x)
#endif /* !_WTLSTL_NO_NAMESPACE */

/// \def wtlstl_ns_qual_std(x)
/// Qualifies with <b>std::</b> if WTLSTL is being translated in the context of the standard library being within the <b>std</b> namespace or, if not, does not qualify

/// \def wtlstl_ns_using_std(x)
/// Declares a using directive (with respect to <b>std</b>) if WTLSTL is being translated in the context of the standard library being within the <b>std</b> namespace or, if not, does nothing

#ifdef __STLSOFT_CF_std_NAMESPACE
# define wtlstl_ns_qual_std(x)      ::std::x
# define wtlstl_ns_using_std(x)     using ::std::x;
#else
# define wtlstl_ns_qual_std(x)      x
# define wtlstl_ns_using_std(x)
#endif /* !__STLSOFT_CF_std_NAMESPACE */

/// \def wtlstl_ns_qual_atl(x)
/// Qualifies with <b>ATL::</b> if WTLSTL is being translated in the context of ATL being within the <b>ATL</b> namespace or, if not, does not qualify

/// \def wtlstl_ns_using_atl(x)
/// Declares a using directive (with respect to <b>ATL</b>) if WTLSTL is being translated in the context of ATL being within the <b>ATL</b> namespace or, if not, does nothing

#ifndef ATL_NO_NAMESPACE
# define wtlstl_ns_qual_atl(x)       ::ATL::x
# define wtlstl_ns_using_atl(x)      using ::ATL::x;
#else
# define wtlstl_ns_qual_atl(x)       x
# define wtlstl_ns_using_atl(x)
#endif /* !ATL_NO_NAMESPACE */

/* /////////////////////////////////////////////////////////////////////////////
 * Typedefs
 *
 * The WTLSTL uses a number of typedefs to aid in compiler-independence in the
 * libraries' main code.
 */

typedef stlsoft_ns_qual(ss_char_a_t)        ws_char_a_t;    //!< Ansi char type
typedef stlsoft_ns_qual(ss_char_w_t)        ws_char_w_t;    //!< Unicode char type
typedef stlsoft_ns_qual(ss_sint8_t)         ws_sint8_t;     //!< 8-bit signed integer
typedef stlsoft_ns_qual(ss_uint8_t)         ws_uint8_t;     //!< 8-bit unsigned integer
typedef stlsoft_ns_qual(ss_int16_t)         ws_int16_t;     //!< 16-bit integer
typedef stlsoft_ns_qual(ss_sint16_t)        ws_sint16_t;    //!< 16-bit signed integer
typedef stlsoft_ns_qual(ss_uint16_t)        ws_uint16_t;    //!< 16-bit unsigned integer
typedef stlsoft_ns_qual(ss_int32_t)         ws_int32_t;     //!< 32-bit integer
typedef stlsoft_ns_qual(ss_sint32_t)        ws_sint32_t;    //!< 32-bit signed integer
typedef stlsoft_ns_qual(ss_uint32_t)        ws_uint32_t;    //!< 32-bit unsigned integer
#ifdef STLSOFT_CF_64BIT_INT_SUPPORT
typedef stlsoft_ns_qual(ss_int64_t)         ws_int64_t;     //!< 64-bit integer
typedef stlsoft_ns_qual(ss_sint64_t)        ws_sint64_t;    //!< 64-bit signed integer
typedef stlsoft_ns_qual(ss_uint64_t)        ws_uint64_t;    //!< 64-bit unsigned integer
#endif /* STLSOFT_CF_64BIT_INT_SUPPORT */
typedef stlsoft_ns_qual(ss_int_t)           ws_int_t;       //!< integer
typedef stlsoft_ns_qual(ss_sint_t)          ws_sint_t;      //!< signed integer
typedef stlsoft_ns_qual(ss_uint_t)          ws_uint_t;      //!< unsigned integer
typedef stlsoft_ns_qual(ss_long_t)          ws_long_t;      //!< long
typedef stlsoft_ns_qual(ss_bool_t)          ws_bool_t;      //!< bool
typedef DWORD                               ws_dword_t;     //!< dword
typedef stlsoft_ns_qual(ss_size_t)          ws_size_t;      //!< size
typedef stlsoft_ns_qual(ss_ptrdiff_t)       ws_ptrdiff_t;   //!< ptr diff
typedef stlsoft_ns_qual(ss_streampos_t)     ws_streampos_t; //!< streampos
typedef stlsoft_ns_qual(ss_streamoff_t)     ws_streamoff_t; //!< streamoff

#ifndef __STLSOFT_DOCUMENTATION_SKIP_SECTION
/* /////////////////////////////////////////////////////////////////////////////
 * Values
 *
 * Since the boolean type may not be supported natively on all compilers, the
 * values of true and false may also not be provided. Hence the values of
 * ws_true_v and ws_false_v are defined, and are used in all code.
 */

#define ws_true_v       ss_true_v
#define ws_false_v      ss_false_v

#endif /* !__STLSOFT_DOCUMENTATION_SKIP_SECTION */
/* /////////////////////////////////////////////////////////////////////////////
 * Code modification macros
 */

#ifndef __STLSOFT_DOCUMENTATION_SKIP_SECTION
/* Exception signatures. */
#define wtlstl_throw_0()                                stlsoft_throw_0()
#define wtlstl_throw_1(x1)                              stlsoft_throw_1(x1)
#define wtlstl_throw_2(x1, x2)                          stlsoft_throw_2(x1, x2)
#define wtlstl_throw_3(x1, x2, x3)                      stlsoft_throw_3(x1, x2, x3)
#define wtlstl_throw_4(x1, x2, x3, x4)                  stlsoft_throw_4(x1, x2, x3, x4)
#define wtlstl_throw_5(x1, x2, x3, x4, x5)              stlsoft_throw_5(x1, x2, x3, x4, x5)
#define wtlstl_throw_6(x1, x2, x3, x4, x5, x6)          stlsoft_throw_6(x1, x2, x3, x4, x5, x6)
#define wtlstl_throw_7(x1, x2, x3, x4, x5, x6, x7)      stlsoft_throw_7(x1, x2, x3, x4, x5, x6, x7)
#define wtlstl_throw_8(x1, x2, x3, x4, x5, x6, x7, x8)  stlsoft_throw_8(x1, x2, x3, x4, x5, x6, x7, x8)
#endif /* !__STLSOFT_DOCUMENTATION_SKIP_SECTION */

/// Evaluates, at compile time, to the number of elements within the given vector entity
#define wtlstl_num_elements(_x)                         stlsoft_num_elements(_x)

/// Destroys the given instance \c p of the given type (\c t and \c _type)
#define wtlstl_destroy_instance(t, _type, p)            stlsoft_destroy_instance(t, _type, p)

/// Generates an opaque type with the name \c _htype
#define wtlstl_gen_opaque(_htype)                       stlsoft_gen_opaque(_htype)

/* ////////////////////////////////////////////////////////////////////////// */

#ifndef _WTLSTL_NO_NAMESPACE
# if defined(_STLSOFT_NO_NAMESPACE) || \
     defined(__STLSOFT_DOCUMENTATION_SKIP_SECTION)
} // namespace wtlstl
# else
} // namespace wtlstl_project
} // namespace stlsoft
namespace wtlstl = ::stlsoft::wtlstl_project;
# endif /* _STLSOFT_NO_NAMESPACE */
#endif /* !_WTLSTL_NO_NAMESPACE */

/* ////////////////////////////////////////////////////////////////////////// */

#endif /* WTLSTL_INCL_H_WTLSTL */

/* ////////////////////////////////////////////////////////////////////////// */
