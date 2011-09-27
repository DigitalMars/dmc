/* /////////////////////////////////////////////////////////////////////////////
 * File:        unixstl.h
 *
 * Purpose:     Root header for the UNIXSTL libraries. Performs various compiler
 *              and platform discriminations, and definitions of types.
 *
 * Created:     15th January 2002
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


#ifndef UNIXSTL_INCL_H_UNIXSTL
#define UNIXSTL_INCL_H_UNIXSTL

/* File version */
#ifndef __STLSOFT_DOCUMENTATION_SKIP_SECTION
# define UNIXSTL_VER_H_UNIXSTL_MAJOR    2
# define UNIXSTL_VER_H_UNIXSTL_MINOR    0
# define UNIXSTL_VER_H_UNIXSTL_REVISION 1
# define UNIXSTL_VER_H_UNIXSTL_EDIT     43
#endif /* !__STLSOFT_DOCUMENTATION_SKIP_SECTION */

/** \file unixstl.h The root header for the \ref UNIXSTL project */

/** \weakgroup projects STLSoft Projects
 *
 * \brief The Projects that comprise the STLSoft libraries
 */

/** \defgroup UNIXSTL UNIXSTL
 * \ingroup projects
 *
 * \brief <img src = "unixstl32x32.jpg">&nbsp;&nbsp;&nbsp;&nbsp;<i>Template Software for the UNIX Operating System</i>
 *
 * The philosophy of UNIXSTL (http://unixstl.org/) is essentially the same as that
 * of the STLSoft (http://stlsoft.org/) organisation: providing robust and
 * lightweight software to the UNIX development
 * community. UNIXSTL provides template-based software that builds on that
 * provided by UNIX and STLSoft in order to reduce programmer effort and increase
 * robustness in the use of the UNIX.
 *
 * <b>Namespaces</b>
 *
 * The UNIXSTL namespace <code><b>unixstl</b></code> is actually an alias for the
 * namespace <code><b>stlsoft::unixstl_project</b></code>, and as such all the
 * UNIXSTL project components actually reside within the
 * <code><b>stlsoft</b></code> namespace. However, there is never any need to
 * use the <code><b>stlsoft::unixstl_project</b></code> namespace in your code,
 * and you should always use the alias <code><b>unixstl</b></code>.
 *
 * <b>Dependencies</b>
 *
 * As with <b><i>all</i></b> parts of the STLSoft libraries, there are no
 * dependencies on UNIXSTL binary components and no need to compile UNIXSTL
 * implementation files; UNIXSTL is <b>100%</b> header-only!
 *
 * As with most of the STLSoft sub-projects, UNIXSTL depends only on:
 *
 * - Selected headers from the C standard library, such as  <code><b>wchar.h</b></code>
 * - Selected headers from the C++ standard library, such as <code><b>new</b></code>, <code><b>functional</b></code>
 * - Selected header files of the STLSoft main project
 * - The header files particular to the technology area, in this case the UNIX library headers, such as <code><b>dirent.h</b></code>
 * - The binary (static and dynamic libraries) components particular to the technology area, in this case the UNIX libraries that ship with the operating system and your compiler(s)
 */

/* /////////////////////////////////////////////////////////////////////////////
 * UNIXSTL version
 *
 * The libraries version information is comprised of major, minor and revision
 * components.
 *
 * The major version is denoted by the _UNIXSTL_VER_MAJOR preprocessor symbol.
 * A changes to the major version component implies that a dramatic change has
 * occurred in the libraries, such that considerable changes to source dependent
 * on previous versions would need to be effected.
 *
 * The minor version is denoted by the _UNIXSTL_VER_MINOR preprocessor symbol.
 * Changes to the minor version component imply that a significant change has
 * occurred to the libraries, either in the addition of new functionality or in
 * the destructive change to one or more components such that recomplilation and
 * code change may be necessitated.
 *
 * The revision version is denoted by the _UNIXSTL_VER_REVISIO preprocessor
 * symbol. Changes to the revision version component imply that a bug has been
 * fixed. Dependent code should be recompiled in order to pick up the changes.
 *
 * In addition to the individual version symbols - _UNIXSTL_VER_MAJOR,
 * _UNIXSTL_VER_MINOR and _UNIXSTL_VER_REVISION - a composite symbol _UNIXSTL_VER
 * is defined, where the upper 8 bits are 0, bits 16-23 represent the major
 * component,  bits 8-15 represent the minor component, and bits 0-7 represent
 * the revision component.
 *
 * Each release of the libraries will bear a different version, and that version
 * will also have its own symbol: Version 1.0.1 specifies _UNIXSTL_VER_1_0_1.
 *
 * Thus the symbol _UNIXSTL_VER may be compared meaningfully with a specific
 * version symbol, e.g.# if _UNIXSTL_VER >= _UNIXSTL_VER_1_0_1
 */

/// \def _UNIXSTL_VER_MAJOR
/// The major version number of UNIXSTL

/// \def _UNIXSTL_VER_MINOR
/// The minor version number of UNIXSTL

/// \def _UNIXSTL_VER_REVISION
/// The revision version number of UNIXSTL

/// \def _UNIXSTL_VER
/// The current composite version number of UNIXSTL

#define _UNIXSTL_VER_MAJOR      1
#define _UNIXSTL_VER_MINOR      2
#define _UNIXSTL_VER_REVISION   1
#ifndef __STLSOFT_DOCUMENTATION_SKIP_SECTION
# define _UNIXSTL_VER_0_9_1     0x00000901  /*!< Version 0.9.1 */
# define _UNIXSTL_VER_0_9_2     0x00000902  /*!< Version 0.9.2 */
# define _UNIXSTL_VER_1_0_1     0x00010001  /*!< Version 1.0.1 */
# define _UNIXSTL_VER_1_0_2     0x00010002  /*!< Version 1.0.2 */
# define _UNIXSTL_VER_1_0_3     0x00010003  /*!< Version 1.0.3 */
# define _UNIXSTL_VER_1_1_1     0x00010101  /*!< Version 1.1.1 */
# define _UNIXSTL_VER_1_2_1     0x00010201  /*!< Version 1.2.1 */
#endif /* !__STLSOFT_DOCUMENTATION_SKIP_SECTION */

#define _UNIXSTL_VER            _UNIXSTL_VER_1_2_1

/* /////////////////////////////////////////////////////////////////////////////
 * Includes
 */

#ifndef STLSOFT_INCL_H_STLSOFT
# include "stlsoft.h"   // Include the STLSoft root header
#endif /* !STLSOFT_INCL_H_STLSOFT */

/* /////////////////////////////////////////////////////////////////////////////
 * STLSoft version compatibility
 */

#if !defined(_STLSOFT_VER_1_5_1) || \
    _STLSOFT_VER < _STLSOFT_VER_1_5_1
# error This version of the UNIXSTL libraries requires STLSoft version 1.5.1 or later
#endif /* _STLSOFT_VER < _STLSOFT_VER_1_5_1 */

/* /////////////////////////////////////////////////////////////////////////////
 * Compiler compatibility
 *
 * Currently the only compilers supported by the UNIXSTL libraries are
 *
 * GCC 2.95, 2.96, 3.2
 * Intel C/C++ 6.0 & 7.0
 */

#if defined(__STLSOFT_COMPILER_IS_GCC)
/* GNU C/C++ */
# if __GNUC__ < 2 || \
     (  __GNUC__ == 2 && \
        __GNUC_MINOR__ < 95)
#  error Versions of GNU C/C++ prior to 2.95 are not supported by the UNIXSTL libraries
# endif /* __GNUC__ */

#elif defined(__STLSOFT_COMPILER_IS_INTEL)
/* Intel C++ */
# if (__INTEL_COMPILER < 700)
#  error Versions of Intel C++ prior to 7.0 are not supported by the UNIXSTL libraries
# endif /* __INTEL_COMPILER */

#else
/* No recognised compiler */
# ifdef _STLSOFT_FORCE_ANY_COMPILER
#  define _UNIXSTL_COMPILER_IS_UNKNOWN
#  ifdef _STLSOFT_COMPILE_VERBOSE
#   pragma message("Compiler is unknown to UNIXSTL")
#  endif /* _STLSOFT_COMPILE_VERBOSE */
# else
#  error Currently only GNU C/C++ compiler supported by the UNIXSTL libraries. To use other, possibly untested, compilers, define _STLSOFT_FORCE_ANY_COMPILER
# endif /* _STLSOFT_FORCE_ANY_COMPILER */
#endif /* compiler */

/* /////////////////////////////////////////////////////////////////////////////
 * Debugging
 *
 * The macro unixstl_assert provides standard debug-mode assert functionality.
 */

/// Defines a runtime assertion
///
/// \param expr Must be non-zero, or an assertion will be fired
#define unixstl_assert(expr)                stlsoft_assert(expr)

/// Defines a runtime assertion, with message
///
/// \param expr Must be non-zero, or an assertion will be fired
/// \param msg The literal character string message to be included in the assertion
#define unixstl_message_assert(msg, expr)   stlsoft_message_assert(msg, expr)

/// Defines a compile-time assertion
///
/// \param expr Must be non-zero, or compilation will fail
#define unixstl_static_assert(expr)         stlsoft_static_assert(expr)

/* /////////////////////////////////////////////////////////////////////////////
 * Namespace
 *
 * The UNIXSTL components are contained within the unixstl namespace. This is
 * usually an alias for stlsoft::unixstl_project,
 *
 * When compilers support namespaces they are defined by default. They can be
 * undefined using a cascasing system, as follows:
 *
 * If _STLSOFT_NO_NAMESPACES is defined, then _UNIXSTL_NO_NAMESPACES is defined.
 *
 * If _UNIXSTL_NO_NAMESPACES is defined, then _UNIXSTL_NO_NAMESPACE is defined.
 *
 * If _UNIXSTL_NO_NAMESPACE is defined, then the UNIXSTL constructs are defined
 * in the global scope.
 *
 * If _STLSOFT_NO_NAMESPACES, _UNIXSTL_NO_NAMESPACES and _UNIXSTL_NO_NAMESPACE are
 * all undefined but the symbol _STLSOFT_NO_NAMESPACE is defined (whence the
 * namespace stlsoft does not exist), then the UNIXSTL constructs are defined
 * within the unixstl namespace. The definition matrix is as follows:
 *
 * _STLSOFT_NO_NAMESPACE    _UNIXSTL_NO_NAMESPACE   unixstl definition
 * ---------------------    --------------------    -----------------
 *  not defined              not defined             = stlsoft::unixstl_project
 *  not defined              defined                 not defined
 *  defined                  not defined             unixstl
 *  defined                  defined                 not defined
 *
 *
 *
 * The macro unixstl_ns_qual() macro can be used to refer to elements in the
 * UNIXSTL libraries irrespective of whether they are in the
 * stlsoft::unixstl_project (or unixstl) namespace or in the global namespace.
 *
 * Furthermore, some compilers do not support the standard library in the std
 * namespace, so the unixstl_ns_qual_std() macro can be used to refer to elements
 * in the UNIXSTL libraries irrespective of whether they are in the std namespace
 * or in the global namespace.
 */

/* No STLSoft namespaces means no UNIXSTL namespaces */
#ifdef _STLSOFT_NO_NAMESPACES
# define _UNIXSTL_NO_NAMESPACES
#endif /* _STLSOFT_NO_NAMESPACES */

/* No UNIXSTL namespaces means no unixstl namespace */
#ifdef _UNIXSTL_NO_NAMESPACES
# define _UNIXSTL_NO_NAMESPACE
#endif /* _UNIXSTL_NO_NAMESPACES */

#ifndef _UNIXSTL_NO_NAMESPACE
# if defined(_STLSOFT_NO_NAMESPACE) || \
     defined(__STLSOFT_DOCUMENTATION_SKIP_SECTION)
/* There is no stlsoft namespace, so must define ::unixstl */
/// The UNIXSTL namespace - \c unixstl (aliased to \c stlsoft::unixstl_project) - is
/// the namespace for the UNIXSTL project.
namespace unixstl
{
# else
/* Define stlsoft::unixstl_project */

namespace stlsoft
{

namespace unixstl_project
{

# endif /* _STLSOFT_NO_NAMESPACE */
#else
stlsoft_ns_using(move_lhs_from_rhs)
#endif /* !_UNIXSTL_NO_NAMESPACE */

/// \def unixstl_ns_qual(x)
/// Qualifies with <b>unixstl::</b> if UNIXSTL is using namespaces or, if not, does not qualify

/// \def unixstl_ns_using(x)
/// Declares a using directive (with respect to <b>unixstl</b>) if UNIXSTL is using namespaces or, if not, does nothing

#ifndef _UNIXSTL_NO_NAMESPACE
# define unixstl_ns_qual(x)             ::unixstl::x
# define unixstl_ns_using(x)            using ::unixstl::x;
#else
# define unixstl_ns_qual(x)             x
# define unixstl_ns_using(x)
#endif /* !_UNIXSTL_NO_NAMESPACE */

/// \def unixstl_ns_qual_std(x)
/// Qualifies with <b>std::</b> if UNIXSTL is being translated in the context of the standard library being within the <b>std</b> namespace or, if not, does not qualify

/// \def unixstl_ns_using_std(x)
/// Declares a using directive (with respect to <b>std</b>) if UNIXSTL is being translated in the context of the standard library being within the <b>std</b> namespace or, if not, does nothing

#ifdef __STLSOFT_CF_std_NAMESPACE
# define unixstl_ns_qual_std(x)         ::std::x
# define unixstl_ns_using_std(x)        using ::std::x;
#else
# define unixstl_ns_qual_std(x)         x
# define unixstl_ns_using_std(x)
#endif /* !__STLSOFT_CF_std_NAMESPACE */

/* /////////////////////////////////////////////////////////////////////////////
 * Typedefs
 *
 * The UNIXSTL uses a number of typedefs to aid in compiler-independence in the
 * libraries' main code.
 */

typedef stlsoft_ns_qual(ss_char_a_t)        us_char_a_t;    //!< Ansi char type
typedef stlsoft_ns_qual(ss_char_w_t)        us_char_w_t;    //!< Unicode char type
typedef stlsoft_ns_qual(ss_sint8_t)         us_sint8_t;     //!< 8-bit signed integer
typedef stlsoft_ns_qual(ss_uint8_t)         us_uint8_t;     //!< 8-bit unsigned integer
typedef stlsoft_ns_qual(ss_int16_t)         us_int16_t;     //!< 16-bit integer
typedef stlsoft_ns_qual(ss_sint16_t)        us_sint16_t;    //!< 16-bit signed integer
typedef stlsoft_ns_qual(ss_uint16_t)        us_uint16_t;    //!< 16-bit unsigned integer
typedef stlsoft_ns_qual(ss_int32_t)         us_int32_t;     //!< 32-bit integer
typedef stlsoft_ns_qual(ss_sint32_t)        us_sint32_t;    //!< 32-bit signed integer
typedef stlsoft_ns_qual(ss_uint32_t)        us_uint32_t;    //!< 32-bit unsigned integer
#ifdef STLSOFT_CF_64BIT_INT_SUPPORT
typedef stlsoft_ns_qual(ss_int64_t)         us_int64_t;     //!< 64-bit integer
typedef stlsoft_ns_qual(ss_sint64_t)        us_sint64_t;    //!< 64-bit signed integer
typedef stlsoft_ns_qual(ss_uint64_t)        us_uint64_t;    //!< 64-bit unsigned integer
#endif /* STLSOFT_CF_64BIT_INT_SUPPORT */
typedef stlsoft_ns_qual(ss_int_t)           us_int_t;       //!< integer
typedef stlsoft_ns_qual(ss_sint_t)          us_sint_t;      //!< signed integer
typedef stlsoft_ns_qual(ss_uint_t)          us_uint_t;      //!< unsigned integer
typedef stlsoft_ns_qual(ss_long_t)          us_long_t;      //!< long
typedef stlsoft_ns_qual(ss_bool_t)          us_bool_t;      //!< bool
typedef stlsoft_ns_qual(ss_size_t)          us_size_t;      //!< size
typedef stlsoft_ns_qual(ss_ptrdiff_t)       us_ptrdiff_t;   //!< ptr diff
typedef stlsoft_ns_qual(ss_streampos_t)     us_streampos_t; //!< streampos
typedef stlsoft_ns_qual(ss_streamoff_t)     us_streamoff_t; //!< streamoff

#ifndef _UNIXSTL_NO_NAMESPACE
typedef us_char_a_t         char_a_t;           //!< Ansi char type
typedef us_char_w_t         char_w_t;           //!< Unicode char type
//typedef us_int8_t           int8_t;             //!< 8-bit integer
typedef us_sint8_t          sint8_t;            //!< 8-bit signed integer
typedef us_uint8_t          uint8_t;            //!< 8-bit unsigned integer
typedef us_int16_t          int16_t;            //!< 16-bit integer
typedef us_sint16_t         sint16_t;           //!< 16-bit signed integer
typedef us_uint16_t         uint16_t;           //!< 16-bit unsigned integer
typedef us_int32_t          int32_t;            //!< 32-bit integer
typedef us_sint32_t         sint32_t;           //!< 32-bit signed integer
typedef us_uint32_t         uint32_t;           //!< 32-bit unsigned integer
# ifdef STLSOFT_CF_64BIT_INT_SUPPORT
 typedef us_int64_t         int64_t;            //!< 64-bit integer
 typedef us_sint64_t        sint64_t;           //!< 64-bit signed integer
 typedef us_uint64_t        uint64_t;           //!< 64-bit unsigned integer
# endif /* STLSOFT_CF_64BIT_INT_SUPPORT */
//typedef us_short_t          short_t;            //!< short integer
typedef us_int_t            int_t;              //!< integer
typedef us_sint_t           sint_t;             //!< signed integer
typedef us_uint_t           uint_t;             //!< unsigned integer
typedef us_long_t           long_t;             //!< long integer
//typedef us_byte_t           byte_t;             //!< Byte
typedef us_bool_t           bool_t;             //!< bool
# if !defined(__STLSOFT_COMPILER_IS_DMC)
typedef us_size_t           size_t;             //!< size
typedef us_ptrdiff_t        ptrdiff_t;          //!< ptr diff
typedef us_streampos_t      streampos_t;        //!< streampos
typedef us_streamoff_t      streamoff_t;        //!< streamoff
# endif /* compiler */
#endif /* !_UNIXSTL_NO_NAMESPACE */

#ifndef __STLSOFT_DOCUMENTATION_SKIP_SECTION
/* /////////////////////////////////////////////////////////////////////////////
 * Values
 *
 * Since the boolean type may not be supported natively on all compilers, the
 * values of true and false may also not be provided. Hence the values of
 * us_true_v and us_false_v are defined, and are used in all code.
 */

#define us_true_v       ss_true_v
#define us_false_v      ss_false_v

#endif /* !__STLSOFT_DOCUMENTATION_SKIP_SECTION */
/* /////////////////////////////////////////////////////////////////////////////
 * Code modification macros
 */

#ifndef __STLSOFT_DOCUMENTATION_SKIP_SECTION
/* Exception signatures. */
#define unixstl_throw_0()                               stlsoft_throw_0()
#define unixstl_throw_1(x1)                             stlsoft_throw_1(x1)
#define unixstl_throw_2(x1, x2)                         stlsoft_throw_2(x1, x2)
#define unixstl_throw_3(x1, x2, x3)                     stlsoft_throw_3(x1, x2, x3)
#define unixstl_throw_4(x1, x2, x3, x4)                 stlsoft_throw_4(x1, x2, x3, x4)
#define unixstl_throw_5(x1, x2, x3, x4, x5)             stlsoft_throw_5(x1, x2, x3, x4, x5)
#define unixstl_throw_6(x1, x2, x3, x4, x5, x6)         stlsoft_throw_6(x1, x2, x3, x4, x5, x6)
#define unixstl_throw_7(x1, x2, x3, x4, x5, x6, x7)     stlsoft_throw_7(x1, x2, x3, x4, x5, x6, x7)
#define unixstl_throw_8(x1, x2, x3, x4, x5, x6, x7, x8) stlsoft_throw_8(x1, x2, x3, x4, x5, x6, x7, x8)
#endif /* !__STLSOFT_DOCUMENTATION_SKIP_SECTION */

/// Evaluates, at compile time, to the number of elements within the given vector entity
#define unixstl_num_elements(ar)                        stlsoft_num_elements(ar)

/// Destroys the given instance \c p of the given type (\c t and \c _type)
#define unixstl_destroy_instance(t, _type, p)           stlsoft_destroy_instance(t, _type, p)

/// Generates an opaque type with the name \c _htype
#define unixstl_gen_opaque(_htype)                      stlsoft_gen_opaque(_htype)

/* ////////////////////////////////////////////////////////////////////////// */

#ifndef _UNIXSTL_NO_NAMESPACE
# if defined(_STLSOFT_NO_NAMESPACE) || \
     defined(__STLSOFT_DOCUMENTATION_SKIP_SECTION)
} // namespace unixstl
# else
} // namespace unixstl_project
} // namespace stlsoft
namespace unixstl = ::stlsoft::unixstl_project;
# endif /* _STLSOFT_NO_NAMESPACE */
#endif /* !_UNIXSTL_NO_NAMESPACE */

/* ////////////////////////////////////////////////////////////////////////// */

#endif /* !UNIXSTL_INCL_H_UNIXSTL */

/* ////////////////////////////////////////////////////////////////////////// */
