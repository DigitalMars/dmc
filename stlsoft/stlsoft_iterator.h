/* ////////////////////////////////////////////////////////////////////////////
 * File:        stlsoft_iterator.h (originally MTIter.h, ::SynesisStl)
 *
 * Purpose:     iterator classes.
 *
 * Created:     2nd January 2000
 * Updated:     11th September 2004
 *
 * Home:        http://stlsoft.org/
 *
 * Copyright (c) 2000-2004, Matthew Wilson and Synesis Software
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


/// \file stlsoft_iterator.h
///
/// iterator classes.

#ifndef STLSOFT_INCL_H_STLSOFT_ITERATOR
#define STLSOFT_INCL_H_STLSOFT_ITERATOR

#ifndef __STLSOFT_DOCUMENTATION_SKIP_SECTION
# define STLSOFT_VER_H_STLSOFT_ITERATOR_MAJOR       2
# define STLSOFT_VER_H_STLSOFT_ITERATOR_MINOR       0
# define STLSOFT_VER_H_STLSOFT_ITERATOR_REVISION    1
# define STLSOFT_VER_H_STLSOFT_ITERATOR_EDIT        64
#endif /* !__STLSOFT_DOCUMENTATION_SKIP_SECTION */

/* ////////////////////////////////////////////////////////////////////////////
 * Includes
 */

#ifndef STLSOFT_INCL_H_STLSOFT
# include "stlsoft.h"  // Include the STLSoft root header
#endif /* !STLSOFT_INCL_H_STLSOFT */
#ifndef STLSOFT_INCL_H_STLSOFT_META
# include "stlsoft_meta.h"
#endif /* !STLSOFT_INCL_H_STLSOFT_META */
#include <iterator>    // std::iterator, std::reverse_iterator, std::reverse_bidirectional_iterator

/* /////////////////////////////////////////////////////////////////////////////
 * Warnings
 */

/* This is here temporarily, until a better solution can be found. */
#ifdef __STLSOFT_COMPILER_IS_MSVC
# pragma warning(disable : 4097)    // suppresses: typedef-name 'identifier1' used as synonym for class-name 'identifier2'
#endif /* __STLSOFT_COMPILER_IS_MSVC */

/* /////////////////////////////////////////////////////////////////////////////
 * Namespace
 */

#ifndef _STLSOFT_NO_NAMESPACE
namespace stlsoft
{
#endif /* _STLSOFT_NO_NAMESPACE */

/* /////////////////////////////////////////////////////////////////////////////
 * Standard library identification
 *
 * Currently recognised libraries are:          (identifying header include guards)
 *
 * 1. Dinkumware (shipping with Visual C++)     _ITERATOR_, _UTILITY_, _XSTDDEF_
 * 2. Metrowerks' MSL                           _ITERATOR, _MSLCONFIG (and __MSL_CPP__; well done Metrowerks!)
 * 3. STLport                                   _STLP_INTERNAL_ITERATOR_H (and _STLPORT_VERSION)
 * 4. HP/SGI, including HP/SGI/Comeau           __SGI_STL_INTERNAL_ITERATOR_H
 * 5. Gnu FSF's HP/SGI derivative               __GLIBCPP_INTERNAL_ITERATOR_H, _GLIBCXX_ITERATOR
 * 6. HP/RW                                     __RW_ITERATOR_H. __STD_RW_ITERATOR__
 */

/* The inclusion of <iterator> results in the following inclusions when using one
 * of the dinkumware libraries:
 *
 * VC++ 4.2: use_ansi.h, utility { use_ansi.h, iosfwd { use_ansi.h, cstdio { stdio.h }, cstring { string.h }, cwchar { wchar.h }, xstddef { yvals.h, cstddef { stddef.h } } } }
 * VC++ 5.0: utility { iosfwd { cstdio, cstring, cwchar, xstddef { yvals.h, cstddef } } }
 * VC++ 6.0: utility { iosfwd { cstdio, cstring, cwchar, xstddef { yvals.h, cstddef } } }
 * VC++ 7.0: xutility { climits, utility { iosfwd { cstdio, cstring, cwchar, xstddef { yvals.h, cstddef } } }
 * VC++ 7.1: xutility { climits, utility { iosfwd { cstdio, cstring, cwchar, xstddef { yvals.h, cstddef } } }
 *
 */

#ifdef STLSOFT_CF_STD_LIBRARY_IS_DINKUMWARE_VC
# undef STLSOFT_CF_STD_LIBRARY_IS_DINKUMWARE_VC
#endif /* STLSOFT_CF_STD_LIBRARY_IS_DINKUMWARE_VC */

/* Includes: iterator { mslconfig, cstddef, iosfwd, msl_utility } */
#ifdef STLSOFT_CF_STD_LIBRARY_IS_MSL
# undef STLSOFT_CF_STD_LIBRARY_IS_MSL
#endif /* STLSOFT_CF_STD_LIBRARY_IS_MSL */

#ifdef STLSOFT_CF_STD_LIBRARY_IS_STLPORT
# undef STLSOFT_CF_STD_LIBRARY_IS_STLPORT
#endif /* STLSOFT_CF_STD_LIBRARY_IS_STLPORT */

#ifdef STLSOFT_CF_STD_LIBRARY_IS_HP_SGI
# undef STLSOFT_CF_STD_LIBRARY_IS_HP_SGI
#endif /* STLSOFT_CF_STD_LIBRARY_IS_HP_SGI */

#ifdef STLSOFT_CF_STD_LIBRARY_IS_HP_SGI_GNU
# undef STLSOFT_CF_STD_LIBRARY_IS_HP_SGI_GNU
#endif /* STLSOFT_CF_STD_LIBRARY_IS_HP_SGI_GNU */

#ifdef STLSOFT_CF_STD_LIBRARY_IS_HP_RW
# undef STLSOFT_CF_STD_LIBRARY_IS_HP_RW
#endif /* STLSOFT_CF_STD_LIBRARY_IS_HP_RW */

#ifdef STLSOFT_CF_STD_LIBRARY_IS_WATCOM_PATCH
# undef STLSOFT_CF_STD_LIBRARY_IS_WATCOM_PATCH
#endif /* STLSOFT_CF_STD_LIBRARY_IS_WATCOM_PATCH */

#if defined(_STLPORT_VERSION) && \
      defined(_STLP_INTERNAL_ITERATOR_H)
# ifdef _STLSOFT_COMPILE_VERBOSE
#  pragma message("Standard library is STLport")
# endif /* STLSOFT_CF_PRAGMA_MESSAGE_SUPPORT && _STLSOFT_COMPILE_VERBOSE */
# define STLSOFT_CF_STD_LIBRARY_IS_STLPORT
#elif defined(_ITERATOR_) && \
      defined(_UTILITY_) && \
      defined(_XSTDDEF_)
# ifdef _STLSOFT_COMPILE_VERBOSE
#  pragma message("Standard library is Dinkumware (VC++)")
# endif /* STLSOFT_CF_PRAGMA_MESSAGE_SUPPORT && _STLSOFT_COMPILE_VERBOSE */
# define STLSOFT_CF_STD_LIBRARY_IS_DINKUMWARE_VC
#elif defined(_ITERATOR) && \
      defined(_MSLCONFIG) && \
      defined(__MSL_CPP__)
# ifdef _STLSOFT_COMPILE_VERBOSE
#  pragma message("Standard library is MSL")
# endif /* STLSOFT_CF_PRAGMA_MESSAGE_SUPPORT && _STLSOFT_COMPILE_VERBOSE */
# define STLSOFT_CF_STD_LIBRARY_IS_MSL
#elif defined(__SGI_STL_INTERNAL_ITERATOR_H)
# ifdef _STLSOFT_COMPILE_VERBOSE
#  pragma message("Standard library is HP/SGI")
# endif /* STLSOFT_CF_PRAGMA_MESSAGE_SUPPORT && _STLSOFT_COMPILE_VERBOSE */
# define STLSOFT_CF_STD_LIBRARY_IS_HP_SGI
#elif defined(__GLIBCPP_INTERNAL_ITERATOR_H) || \
      defined(_GLIBCXX_ITERATOR)
# ifdef _STLSOFT_COMPILE_VERBOSE
#  pragma message("Standard library is HP/SGI/GnuFSF")
# endif /* STLSOFT_CF_PRAGMA_MESSAGE_SUPPORT && _STLSOFT_COMPILE_VERBOSE */
# define STLSOFT_CF_STD_LIBRARY_IS_HP_SGI_GNU
#elif defined(__RW_ITERATOR_H) && \
      defined(__STD_RW_ITERATOR__)
# ifdef _STLSOFT_COMPILE_VERBOSE
#  pragma message("Standard library is HP/RW")
# endif /* STLSOFT_CF_PRAGMA_MESSAGE_SUPPORT && _STLSOFT_COMPILE_VERBOSE */
# define STLSOFT_CF_STD_LIBRARY_IS_HP_RW
#elif defined(STLSOFT_OW12_INCL_ITERATOR)
# define STLSOFT_CF_STD_LIBRARY_IS_WATCOM_PATCH
#else
# error Standard library implementation not recognised
#endif /* various "unique" macros */

/* Detecting presence of Dinkumware is easy. Now comes the fun when we need to differentiate
 * between versions of the library, because there is no version information contained in any
 * of the headers, which is helpful.
 *
 * _STCONS is in 5.0, 6.0, 7.0, 7.1, but is not in 4.2
 * _TEMPLATE_MEMBER is in 7.0, but not in 4.2, 5.0, 6.0, 7.1
 * _TEMPLATE is in 7.0, but not in 4.2, 5.0, 6.0, 7.1
 * _MESG is in 7.0 and 7.1, but not in 4.2, 5.0, 5.6
 *
 * _XTREE_ is in 6.0, 7.0, 7.1, but not in 4.2, 5.0. Of course, this means including it, which is a PITA!
 *
 * _INC_ASSERT (from assert.h) is in 6.0, but not in 4.2, 5.0, 7.0, 7.1. This one is even
 * dodgier than the rest, since if _STLSOFT_NO_ASSERT is specified, asserts will not even
 * be included.
 */

#ifdef STLSOFT_CF_STD_LIBRARY_DINKUMWARE_VC_VERSION
# undef STLSOFT_CF_STD_LIBRARY_DINKUMWARE_VC_VERSION
#endif /* STLSOFT_CF_STD_LIBRARY_DINKUMWARE_VC_VERSION */

#define STLSOFT_CF_DINKUMWARE_VC_VERSION_UNKNOWN        (0x0000)
#define STLSOFT_CF_DINKUMWARE_VC_VERSION_4_2            (0x0402)
#define STLSOFT_CF_DINKUMWARE_VC_VERSION_5_0            (0x0500)
#define STLSOFT_CF_DINKUMWARE_VC_VERSION_6_0            (0x0600)
#define STLSOFT_CF_DINKUMWARE_VC_VERSION_7_0            (0x0700)
#define STLSOFT_CF_DINKUMWARE_VC_VERSION_7_1            (0x0701)

#ifdef STLSOFT_CF_STD_LIBRARY_IS_DINKUMWARE_VC
# if defined(__STLSOFT_COMPILER_IS_INTEL) && \
     defined(unix)
  /* Ok to have Intel + Dinkumware without VC++ on Linux */
# elif !defined(_MSC_VER)
#  error When the Dinkumware-VC library is used, STLSoft requires that Visual C++ or a compatible compiler (e.g. DMC++, Comeau, CodeWarrior, Intel) is used
# endif /* !_MSC_VER */

# if !defined(_STCONS)
  /* Version 4.2 */
#  define STLSOFT_CF_STD_LIBRARY_DINKUMWARE_VC_VERSION      STLSOFT_CF_DINKUMWARE_VC_VERSION_4_2
# elif defined(_TEMPLATE_MEMBER) && \
       defined(_TEMPLATE)
  /* Version 7.0 */
#  define STLSOFT_CF_STD_LIBRARY_DINKUMWARE_VC_VERSION      STLSOFT_CF_DINKUMWARE_VC_VERSION_7_0
# elif defined(_MESG)
  /* Version 7.1 */
#  define STLSOFT_CF_STD_LIBRARY_DINKUMWARE_VC_VERSION      STLSOFT_CF_DINKUMWARE_VC_VERSION_7_1
# elif defined(_STCONS)
  /* Version 5.0 or 6.0
   *
   * Need to skip out of the STLSoft namespace, then #include <xtree>, skip back in and then
   * test for
   */

#  ifndef _STLSOFT_NO_NAMESPACE
} // namespace stlsoft
#  endif /* _STLSOFT_NO_NAMESPACE */
#  include <xtree>
#  if defined(_XTREE_)
  /* Version 6.0 */
#   define STLSOFT_CF_STD_LIBRARY_DINKUMWARE_VC_VERSION     STLSOFT_CF_DINKUMWARE_VC_VERSION_6_0
#  elif defined(_TREE_)
  /* Version 5.0 */
#   define STLSOFT_CF_STD_LIBRARY_DINKUMWARE_VC_VERSION     STLSOFT_CF_DINKUMWARE_VC_VERSION_5_0
#  else
#   error Does not appear to be either the VC5 or VC6 Dinkumware library
#  endif /* _XTREE_ */
#  ifndef _STLSOFT_NO_NAMESPACE
namespace stlsoft
{
#  endif /* _STLSOFT_NO_NAMESPACE */
# else
#  define STLSOFT_CF_STD_LIBRARY_DINKUMWARE_VC_VERSION      STLSOFT_CF_DINKUMWARE_VC_VERSION_UNKNOWN
# endif /* VC++ version */
#endif /* STLSOFT_CF_STD_LIBRARY_IS_DINKUMWARE_VC */


/* /////////////////////////////////////////////////////////////////////////////
 * Tested compatibilities
 */

#if !defined(STLSOFT_OVERRIDE_COMPILER_STD_LIBRARY_CHECK)

#if 0
    /* Now we Must be either Dinkumware or STLport if compiling with Intel or Visual C++
     */

#    if (   defined(__STLSOFT_COMPILER_IS_INTEL) || \
            (   defined(__STLSOFT_COMPILER_IS_MSVC) && \
                _MSC_VER >= 1200 && \
                _MSC_VER < 1310)) && \
        (   !defined(STLSOFT_CF_STD_LIBRARY_IS_DINKUMWARE_VC) && \
            !defined(STLSOFT_CF_STD_LIBRARY_IS_STLPORT))
#     error When compiling with Intel C/C++ or Microsoft Visual C++, only the Dinkumware or STLport STL implementations are currently supported.
#     error  Please contact STLSoft (admin@stlsoft.org) if you need to support a different STL implementation with these compilers.
#    endif /* (Intel || MSVC) && !DinkumWare && !STLport */
#endif /* 0 */


#endif /* !STLSOFT_OVERRIDE_COMPILER_STD_LIBRARY_CHECK */

/* /////////////////////////////////////////////////////////////////////////////
 * Iterator macros
 */

/* iterator
 *
 * There are
 *
 * 1. Form 1. This is the standard (C++-98: 24.2) form, and looks like the following
 *
 *    template< typename C
 *            , typename V
 *            , typename D = ptrdiff_t
 *            , typename P = V*
 *            , typename R = V&
 *            >
 *    struct iterator
 *    {
 *      typedef C   iterator_category;
 *      typedef V   value_type;
 *      typedef D  difference_type;
 *      typedef P  pointer;
 *      typedef R  reference;
 *    };
 *
 * 2. Form 2. This is found with Dinkumware / Visual C++ (versions 4.2, 5.0, 6.0)
 *
 *    template< typename C
 *            , typename V
 *            , typename D = ptrdiff_t
 *            >
 *    struct iterator
 *    {
 *      typedef C iterator_category;
 *      typedef V value_type;
 *      typedef D distance_type;
 *    };
 *
 * 3. Form 3. This is found in quite old versions of the STL, and in fact does not define
 *            an iterator template at all. Each container has its own iterator type
 */


#ifdef STLSOFT_ITERATOR_ITERATOR_FORM1_SUPPORT
# undef STLSOFT_ITERATOR_ITERATOR_FORM1_SUPPORT
#endif /* !STLSOFT_ITERATOR_ITERATOR_FORM1_SUPPORT */

#ifdef STLSOFT_ITERATOR_ITERATOR_FORM2_SUPPORT
# undef STLSOFT_ITERATOR_ITERATOR_FORM2_SUPPORT
#endif /* !STLSOFT_ITERATOR_ITERATOR_FORM2_SUPPORT */

#ifdef STLSOFT_ITERATOR_ITERATOR_FORM3_SUPPORT
# undef STLSOFT_ITERATOR_ITERATOR_FORM3_SUPPORT
#endif /* !STLSOFT_ITERATOR_ITERATOR_FORM3_SUPPORT */


#if defined(__STLSOFT_COMPILER_IS_GCC) && \
    __GNUC__ < 3
 /* GCC 2.95 */
# define STLSOFT_ITERATOR_ITERATOR_FORM3_SUPPORT
#elif defined(STLSOFT_CF_STD_LIBRARY_IS_DINKUMWARE_VC)
# if  STLSOFT_CF_STD_LIBRARY_DINKUMWARE_VC_VERSION == STLSOFT_CF_DINKUMWARE_VC_VERSION_4_2 || \
      STLSOFT_CF_STD_LIBRARY_DINKUMWARE_VC_VERSION == STLSOFT_CF_DINKUMWARE_VC_VERSION_5_0 || \
      STLSOFT_CF_STD_LIBRARY_DINKUMWARE_VC_VERSION == STLSOFT_CF_DINKUMWARE_VC_VERSION_6_0
 /* Visual C++ + Dinkumware (pre v7.0) */
#  define STLSOFT_ITERATOR_ITERATOR_FORM2_SUPPORT
# else
#  define STLSOFT_ITERATOR_ITERATOR_FORM1_SUPPORT
# endif /* STLSOFT_CF_STD_LIBRARY_IS_DINKUMWARE_VC */
#elif defined(STLSOFT_CF_STD_LIBRARY_IS_MSL)
# define STLSOFT_ITERATOR_ITERATOR_FORM1_SUPPORT
#elif defined(STLSOFT_CF_STD_LIBRARY_IS_STLPORT)
# define STLSOFT_ITERATOR_ITERATOR_FORM1_SUPPORT
#elif defined(STLSOFT_CF_STD_LIBRARY_IS_HP_SGI)
# if defined(__STL_USE_NAMESPACES)
#  define STLSOFT_ITERATOR_ITERATOR_FORM1_SUPPORT
# else /* ? __STL_USE_NAMESPACES */
#  define STLSOFT_ITERATOR_ITERATOR_FORM3_SUPPORT
# endif /* __STL_USE_NAMESPACES */
#elif defined(STLSOFT_CF_STD_LIBRARY_IS_HP_SGI_GNU)
# define STLSOFT_ITERATOR_ITERATOR_FORM1_SUPPORT
#elif defined(STLSOFT_CF_STD_LIBRARY_IS_HP_RW)
# define STLSOFT_ITERATOR_ITERATOR_FORM1_SUPPORT
#elif defined(STLSOFT_CF_STD_LIBRARY_IS_WATCOM_PATCH)
# define STLSOFT_ITERATOR_ITERATOR_FORM1_SUPPORT
#else
# error Library version not recognised
#endif /* library */


/* reverse_iterator
 *
 * There are four known forms for reverse_iterators:
 *
 * 1. Form 1. This is the standard (C++-98: 24.4.1.1) form, and looks like the following
 *
 *    template <typename I>
 *      : public iterator<  typename iterator_traits<I>::iterator_category,
 *                          typename iterator_traits<I>::value_type,
 *                          typename iterator_traits<I>::difference_type,
 *                          typename iterator_traits<I>::pointer,
 *                          typename iterator_traits<I>::reference>
 *    {
 *      . . .
 *      typedef I                                               iterator_type;
 *      typedef typename iterator_traits<I>::difference_type    difference_type;
 *      typedef typename iterator_traits<I>::reference          reference;
 *      typedef typename iterator_traits<I>::pointer            pointer;
 *      . . .
 *    };
 *
 *
 * 2. Form 2. This is effectively standard, but does not derive from std::iterator. It looks
 *            like the following
 *
 *    template <typename I>
 *    {
 *      typedef typename iterator_traits<I>::iterator_category  iterator_category;
 *      typedef typename iterator_traits<I>::value_type         value_type;
 *      typedef typename iterator_traits<I>::difference_type    difference_type;
 *      typedef typename iterator_traits<I>::pointer            pointer;
 *      typedef typename iterator_traits<I>::reference          reference;
 *      . . .
 *    };
 *
 * 3. Form 3.
 *
 *    template< typename I
 *            , typename V
 *            , typename R = V&
 *            , typename P = V*
 *            , typename D = ptrdiff_t
 *            >
 *      : public _Ranit<V, D>
 *    {
 *      . . .
 *      typedef _Rt reference_type;
 *      typedef _Pt pointer_type;
 *      . . .
 *    };
 *
 *
 * 4. Form 4.
 *
 *    template< typename I
 *            , typename IC
 *            , typename V
 *            , typename R = V&
 *            , typename P = V*
 *            , typename D = ptrdiff_t
 *            >
 *    class reverse_iterator
 *      : public iterator<IC, V, D, P, R>
 *    {
 *      typedef D difference_type;
 *      typedef V value_type;
 *      typedef R reference;
 *      typedef P pointer;
 *      . . .
 *    };
 *
 *
 * 5. Form 5.
 *
 *    template< typename I
 *            , typename V
 *            , typename R = V&
 *            , typename D = ptrdiff_t
 *            >
 *    class reverse_iterator
 *      : public iterator<I, V, R, D>
 *    {
 *      typedef random_access_iterator_tag  iterator_category;
 *      typedef V                           value_type;
 *      typedef D                           difference_type;
 *      typedef V                           *pointer;
 *      typedef R                           reference;
 *      . . .
 *    };
 *
 */

#ifdef STLSOFT_ITERATOR_REVERSE_ITERATOR_FORM1_SUPPORT
# undef STLSOFT_ITERATOR_REVERSE_ITERATOR_FORM1_SUPPORT
#endif /* !STLSOFT_ITERATOR_REVERSE_ITERATOR_FORM1_SUPPORT */

#ifdef STLSOFT_ITERATOR_REVERSE_ITERATOR_FORM2_SUPPORT
# undef STLSOFT_ITERATOR_REVERSE_ITERATOR_FORM2_SUPPORT
#endif /* !STLSOFT_ITERATOR_REVERSE_ITERATOR_FORM2_SUPPORT */

#ifdef STLSOFT_ITERATOR_REVERSE_ITERATOR_FORM3_SUPPORT
# undef STLSOFT_ITERATOR_REVERSE_ITERATOR_FORM3_SUPPORT
#endif /* !STLSOFT_ITERATOR_REVERSE_ITERATOR_FORM3_SUPPORT */

#ifdef STLSOFT_ITERATOR_REVERSE_ITERATOR_FORM4_SUPPORT
# undef STLSOFT_ITERATOR_REVERSE_ITERATOR_FORM4_SUPPORT
#endif /* !STLSOFT_ITERATOR_REVERSE_ITERATOR_FORM4_SUPPORT */

#ifdef STLSOFT_ITERATOR_REVERSE_ITERATOR_FORM5_SUPPORT
# undef STLSOFT_ITERATOR_REVERSE_ITERATOR_FORM5_SUPPORT
#endif /* !STLSOFT_ITERATOR_REVERSE_ITERATOR_FORM5_SUPPORT */


/* Form 1 */

#if defined(__STLSOFT_CF_BIDIRECTIONAL_ITERATOR_SUPPORT)

# if defined(STLSOFT_CF_STD_LIBRARY_IS_STLPORT)
 /* STLport is form 1 when not using the old form */
#  if /* defined(__STL_CLASS_PARTIAL_SPECIALIZATION) || \
       */defined(_STLP_CLASS_PARTIAL_SPECIALIZATION)
#   define STLSOFT_ITERATOR_REVERSE_ITERATOR_FORM1_SUPPORT
#  else /* ?_STLP_CLASS_PARTIAL_SPECIALIZATION */
#   define STLSOFT_ITERATOR_REVERSE_ITERATOR_FORM5_SUPPORT
#  endif /* _STLP_CLASS_PARTIAL_SPECIALIZATION */
# elif defined(STLSOFT_CF_STD_LIBRARY_IS_HP_RW)
 /* Borland + HP/RogueWave standard library */
#  define STLSOFT_ITERATOR_REVERSE_ITERATOR_FORM1_SUPPORT
# elif defined(STLSOFT_CF_STD_LIBRARY_IS_HP_SGI_GNU)
 /* GCC + Gnu/FSF standard library */
#  define STLSOFT_ITERATOR_REVERSE_ITERATOR_FORM1_SUPPORT
# elif defined(STLSOFT_CF_STD_LIBRARY_IS_MSL)
 /* CodeWarrior + MSL */
#  define STLSOFT_ITERATOR_REVERSE_ITERATOR_FORM1_SUPPORT
# elif defined(STLSOFT_CF_STD_LIBRARY_IS_DINKUMWARE_VC) && \
       (   STLSOFT_CF_STD_LIBRARY_DINKUMWARE_VC_VERSION == STLSOFT_CF_DINKUMWARE_VC_VERSION_7_0 || \
           STLSOFT_CF_STD_LIBRARY_DINKUMWARE_VC_VERSION == STLSOFT_CF_DINKUMWARE_VC_VERSION_7_1)
 /* Visual C++ + Dinkumware */
#  define STLSOFT_ITERATOR_REVERSE_ITERATOR_FORM1_SUPPORT
# endif /* compiler / library */

/* Form 2 / Form 5 */
# if defined(STLSOFT_CF_STD_LIBRARY_IS_HP_SGI)
 /* HP/SGI or HP/SGI/Comeau */
#  if defined(__STL_CLASS_PARTIAL_SPECIALIZATION)
#   define STLSOFT_ITERATOR_REVERSE_ITERATOR_FORM2_SUPPORT
#  else /* ?__STL_CLASS_PARTIAL_SPECIALIZATION */
#   define STLSOFT_ITERATOR_REVERSE_ITERATOR_FORM5_SUPPORT
#  endif /* __STL_CLASS_PARTIAL_SPECIALIZATION */
# endif /* compiler / library */

/* Form 3 */
# if defined(STLSOFT_CF_STD_LIBRARY_IS_DINKUMWARE_VC) && \
     (   STLSOFT_CF_STD_LIBRARY_DINKUMWARE_VC_VERSION == STLSOFT_CF_DINKUMWARE_VC_VERSION_4_2 || \
         STLSOFT_CF_STD_LIBRARY_DINKUMWARE_VC_VERSION == STLSOFT_CF_DINKUMWARE_VC_VERSION_5_0 || \
         STLSOFT_CF_STD_LIBRARY_DINKUMWARE_VC_VERSION == STLSOFT_CF_DINKUMWARE_VC_VERSION_6_0)
 /* Visual C++ + Dinkumware */
#  define STLSOFT_ITERATOR_REVERSE_ITERATOR_FORM3_SUPPORT
# endif /* compiler / library */

/* Form 4 */


/* stlsoft_reverse_iterator() */
# if defined(STLSOFT_ITERATOR_REVERSE_ITERATOR_FORM1_SUPPORT) || \
     defined(STLSOFT_ITERATOR_REVERSE_ITERATOR_FORM2_SUPPORT)
#  define stlsoft_reverse_iterator(I, V, R, P, D)        stlsoft_ns_qual_std(reverse_iterator)<I>
# elif defined(STLSOFT_ITERATOR_REVERSE_ITERATOR_FORM3_SUPPORT)
#   define stlsoft_reverse_iterator(I, V, R, P, D)       stlsoft_ns_qual_std(reverse_iterator)<I, V, R, P, D>
//# elif defined(STLSOFT_ITERATOR_REVERSE_ITERATOR_FORM4_SUPPORT)
//#   define stlsoft_reverse_iterator(I, V, R, P, D)       stlsoft_ns_qual_std(reverse_iterator)<I, V, R, P, D>
# elif defined(STLSOFT_ITERATOR_REVERSE_ITERATOR_FORM5_SUPPORT)
#  define stlsoft_reverse_iterator(I, V, R, P, D)       stlsoft_ns_qual_std(reverse_iterator)<I, V, R, D>
# else
#  error reverse_iterator form not recognised
# endif /* compiler */

/* stlsoft_reverse_bidirectional_iterator() */

# if defined(STLSOFT_CF_STD_LIBRARY_IS_DINKUMWARE_VC) && \
     (   STLSOFT_CF_STD_LIBRARY_DINKUMWARE_VC_VERSION == STLSOFT_CF_DINKUMWARE_VC_VERSION_4_2 || \
         STLSOFT_CF_STD_LIBRARY_DINKUMWARE_VC_VERSION == STLSOFT_CF_DINKUMWARE_VC_VERSION_5_0 || \
         STLSOFT_CF_STD_LIBRARY_DINKUMWARE_VC_VERSION == STLSOFT_CF_DINKUMWARE_VC_VERSION_6_0)
#  define stlsoft_reverse_bidirectional_iterator(I, V, R, P, D)      stlsoft_ns_qual_std(reverse_bidirectional_iterator)<I, V, R, P, D>
# elif defined(STLSOFT_CF_STD_LIBRARY_IS_STLPORT) && \
       !defined(_STLP_CLASS_PARTIAL_SPECIALIZATION)
#  define stlsoft_reverse_bidirectional_iterator(I, V, R, P, D)      stlsoft_ns_qual_std(reverse_bidirectional_iterator)<I, V, R, P, D>
# else
#  define stlsoft_reverse_bidirectional_iterator(I, V, R, P, D)      stlsoft_reverse_iterator(I, V, R, P, D)
# endif /*  */

#endif /* __STLSOFT_CF_BIDIRECTIONAL_ITERATOR_SUPPORT */

/* /////////////////////////////////////////////////////////////////////////////
 * Helper classes
 */

#ifdef STLSOFT_CF_HAS_MEMBER_TYPE_SUPPORTED
# include "stlsoft_typefixer_pointer.h"
# include "stlsoft_typefixer_reference.h"
#endif /* STLSOFT_CF_HAS_MEMBER_TYPE_SUPPORTED */

/* /////////////////////////////////////////////////////////////////////////////
 * Iterators
 */

// class iterator_base
/// Base type for <b><code>iterator</code></b> types
//
/// This class abstract std::iterator functionality for deriving classes, hiding
/// the inconsistencies and incompatibilities of the various compilers and/or
/// libraries supported by the STLSoft libraries.
///
/// \param C The iterator category
/// \param V The value type
/// \param D The distance type
/// \param P The pointer type
/// \param R The reference type
template<   ss_typename_param_k C   /* iterator category */
        ,   ss_typename_param_k V   /* value type */
        ,   ss_typename_param_k D   /* distance type */
        ,   ss_typename_param_k P   /* pointer */
        ,   ss_typename_param_k R   /* reference */
        >
struct iterator_base
#if defined(STLSOFT_ITERATOR_ITERATOR_FORM1_SUPPORT)
    : public stlsoft_ns_qual_std(iterator)<C, V, D, P, R>
#elif defined(STLSOFT_ITERATOR_ITERATOR_FORM2_SUPPORT)
    : public stlsoft_ns_qual_std(iterator)<C, V, D>
#elif defined(STLSOFT_ITERATOR_ITERATOR_FORM3_SUPPORT)
    /* Form 3 does not provide an iterator from which we can derive */
#else
# error Further iterator form discrimination required
#endif /* STLSOFT_ITERATOR_ITERATOR_FORM?_SUPPORT */
{
private:
#if defined(STLSOFT_ITERATOR_ITERATOR_FORM1_SUPPORT)
    typedef stlsoft_ns_qual_std(iterator)<C, V, D, P, R>    parent_class_type;
#elif defined(STLSOFT_ITERATOR_ITERATOR_FORM2_SUPPORT)
    typedef stlsoft_ns_qual_std(iterator)<C, V, D>          parent_class_type;
#elif defined(STLSOFT_ITERATOR_ITERATOR_FORM3_SUPPORT)
    /* Form 3 does not provide an iterator from which we can derive */
    typedef void                                            parent_class_type;
#else
# error Further iterator form discrimination required
#endif /* STLSOFT_ITERATOR_ITERATOR_FORM?_SUPPORT */


# ifdef STLSOFT_CF_HAS_MEMBER_TYPE_SUPPORTED_XXX
private:
    // iterator_category
    enum { has_member_iterator_category     =   0 != has_iterator_category<parent_class_type>::value    };

    // value_type
    enum { has_member_value_type            =   0 != has_value_type<parent_class_type>::value           };

    // distance_type
    enum { has_member_distance_type         =   0 != has_distance_type<parent_class_type>::value        };

    // pointer
    enum { has_member_pointer               =   0 != has_pointer<parent_class_type>::value              };
    enum { has_member_pointer_type          =   0 != has_pointer_type<parent_class_type>::value         };

    // reference
    enum { has_member_reference             =   0 != has_reference<parent_class_type>::value            };
    enum { has_member_reference_type        =   0 != has_reference_type<parent_class_type>::value       };

    typedef C               iterator_category_candidate;
    typedef V               value_type_candidate;
    typedef D               distance_candidate;
    typedef P               pointer_candidate;
    typedef R               reference_candidate;

public:
    /// The pointer type
    typedef ss_typename_type_k select_first_type<   ss_typename_type_k fixer_pointer_type<parent_class_type, has_member_pointer_type>::pointer
                                                ,   ss_typename_type_k fixer_pointer<parent_class_type, has_member_pointer>::pointer
                                                ,   has_member_pointer_type
                                                >::type             pointer;
    /// The reference type
    typedef ss_typename_type_k select_first_type<   ss_typename_type_k fixer_reference_type<parent_class_type, has_member_reference_type>::reference
                                                ,   ss_typename_type_k fixer_reference<parent_class_type, has_member_reference>::reference
                                                ,   has_member_reference_type
                                                >::type             reference;

# else /* ? STLSOFT_CF_HAS_MEMBER_TYPE_SUPPORTED */

public:
#if defined(STLSOFT_ITERATOR_ITERATOR_FORM3_SUPPORT)
    typedef C                                                       iterator_category;
    typedef V                                                       value_type;
    typedef D                                                       difference_type;
    typedef P                                                       pointer;
    typedef R                                                       reference;
#else
  /* Forms 1 or 2 */
    typedef ss_typename_type_k parent_class_type::iterator_category iterator_category;
    typedef ss_typename_type_k parent_class_type::value_type        value_type;
# if defined(STLSOFT_ITERATOR_ITERATOR_FORM1_SUPPORT)
    typedef ss_typename_type_k parent_class_type::difference_type   difference_type;
    typedef ss_typename_type_k parent_class_type::pointer           pointer;
    typedef ss_typename_type_k parent_class_type::reference         reference;
# elif defined(STLSOFT_ITERATOR_ITERATOR_FORM2_SUPPORT)
    typedef ss_typename_type_k parent_class_type::distance_type     difference_type;
    typedef P                                                       pointer;
    typedef R                                                       reference;
# else
#  error Further iterator form discrimination required
# endif /* STLSOFT_ITERATOR_ITERATOR_FORM?_SUPPORT */
#endif /* STLSOFT_ITERATOR_ITERATOR_FORM3_SUPPORT */

# endif /* STLSOFT_CF_HAS_MEMBER_TYPE_SUPPORTED */

    /* These two are for compatibility with older non-standard implementations, and
     * will be benignly ignored by anything not requiring them.
     */
    typedef pointer                                                 pointer_type;
    typedef reference                                               reference_type;
};


#ifdef __STLSOFT_CF_BIDIRECTIONAL_ITERATOR_SUPPORT

// reverse_iterator_base, const_reverse_iterator_base,
// reverse_bidirectional_iterator_base and const_reverse_bidirectional_iterator_base
//
// These classes act as the base for reverse iterators, insulating deriving
// classes from the inconsistencies and incompatibilities of the various
// compilers and/or libraries supported by the STLSoft libraries.

// class reverse_iterator_base
/// Base type for <b><code>reverse_iterator</code></b> types
//
/// This class acts as the base for reverse iterators, insulating deriving
/// classes from the inconsistencies and incompatibilities of the various
/// compilers and/or libraries supported by the STLSoft libraries.
///
/// \param I The iterator type
/// \param V The value type
/// \param R The reference type
/// \param P The pointer type
/// \param D The distance type
template<   ss_typename_param_k I   /* iterator */
        ,   ss_typename_param_k V   /* value type */
        ,   ss_typename_param_k R   /* reference */
        ,   ss_typename_param_k P   /* pointer */
        ,   ss_typename_param_k D   /* distance type */
        >
struct reverse_iterator_base
    : public stlsoft_reverse_iterator(I, V, R, P, D)
{
public:
    typedef stlsoft_reverse_iterator(I, V, R, P, D)                 parent_class_type;

    typedef ss_typename_type_k parent_class_type::iterator_category iterator_category;
    typedef ss_typename_type_k parent_class_type::value_type        value_type;
# if defined(STLSOFT_ITERATOR_REVERSE_ITERATOR_FORM3_SUPPORT)
    typedef ss_typename_type_k parent_class_type::distance_type     difference_type;
    typedef ss_typename_type_k parent_class_type::pointer_type      pointer;
    typedef ss_typename_type_k parent_class_type::reference_type    reference;
# else /* ? STLSOFT_ITERATOR_REVERSE_ITERATOR_FORM3_SUPPORT */
    typedef ss_typename_type_k parent_class_type::difference_type   difference_type;
    typedef ss_typename_type_k parent_class_type::pointer           pointer;
    typedef ss_typename_type_k parent_class_type::reference         reference;
# endif /* STLSOFT_ITERATOR_REVERSE_ITERATOR_FORM3_SUPPORT */

    /* These two are for compatibility with older non-standard implementations, and
     * will be benignly ignored by anything not requiring them.
     */
    typedef pointer                                                 pointer_type;
    typedef reference                                               reference_type;

// Construction
public:
    /// Constructor
    ss_explicit_k reverse_iterator_base(I i)
        : parent_class_type(i)
    {}
};

// class const_reverse_iterator_base
/// Base type for <b><code>const_reverse_iterator</code></b> types
//
/// This class acts as the base for const reverse iterators, insulating deriving
/// classes from the inconsistencies and incompatibilities of the various
/// compilers and/or libraries supported by the STLSoft libraries.
///
/// \param I The iterator type
/// \param V The value type
/// \param R The reference type
/// \param P The pointer type
/// \param D The distance type
template<   ss_typename_param_k I   /* iterator */
        ,   ss_typename_param_k V   /* value type */
        ,   ss_typename_param_k R   /* reference */
        ,   ss_typename_param_k P   /* pointer */
        ,   ss_typename_param_k D   /* distance type */
        >
struct const_reverse_iterator_base                  // For all current compilers/libraries, ...
    : public reverse_iterator_base<I, V, R, P, D>   // ... this is the same as reverse_iterator_base
{
private:
    typedef reverse_iterator_base<I, V, R, P, D>                    parent_class_type;

public:
    /// The iterator category type
    typedef ss_typename_type_k parent_class_type::iterator_category iterator_category;
    /// The value type
    typedef ss_typename_type_k parent_class_type::value_type        value_type;
    /// The difference type
    typedef ss_typename_type_k parent_class_type::difference_type   difference_type;
    /// The pointer type
    typedef ss_typename_type_k parent_class_type::pointer           pointer;
    /// The reference type
    typedef ss_typename_type_k parent_class_type::reference         reference;

    /* These two are for compatibility with older non-standard implementations, and
     * will be benignly ignored by anything not requiring them.
     */
    /// The alternate pointer type (for backwards compatibility)
    typedef ss_typename_type_k parent_class_type::pointer_type      pointer_type;
    /// The alternate reference type (for backwards compatibility)
    typedef ss_typename_type_k parent_class_type::reference_type    reference_type;

// Construction
public:
    /// Constructor
    ss_explicit_k const_reverse_iterator_base(I i)
        : parent_class_type(i)
    {}
};

// class reverse_bidirectional_iterator_base
/// Base type for <b><code>reverse_bidirectional_iterator</code></b> types
//
/// This class acts as the base for reverse bidirectional iterators,
/// insulating deriving classes from the inconsistencies and incompatibilities
/// of the various compilers and/or libraries supported by the STLSoft libraries.
///
/// \param I The iterator type
/// \param V The value type
/// \param R The reference type
/// \param P The pointer type
/// \param D The distance type
template<   ss_typename_param_k I   /* iterator */
        ,   ss_typename_param_k V   /* value type */
        ,   ss_typename_param_k R   /* reference */
        ,   ss_typename_param_k P   /* pointer */
        ,   ss_typename_param_k D   /* distance type */
        >
struct reverse_bidirectional_iterator_base
    : public stlsoft_reverse_bidirectional_iterator(I, V, R, P, D)
{
public:
    typedef stlsoft_reverse_bidirectional_iterator(I, V, R, P, D)               parent_class_type;

    typedef ss_typename_type_k parent_class_type::iterator_category             iterator_category;
    typedef ss_typename_type_k parent_class_type::value_type                    value_type;

# if defined(STLSOFT_ITERATOR_REVERSE_ITERATOR_FORM3_SUPPORT)
    typedef ss_typename_type_k parent_class_type::distance_type                 difference_type;
    typedef ss_typename_type_k parent_class_type::pointer_type                  pointer;
    typedef ss_typename_type_k parent_class_type::reference_type                reference;
#else /* ? STLSOFT_ITERATOR_REVERSE_ITERATOR_FORM3_SUPPORT */
    typedef ss_typename_type_k parent_class_type::difference_type               difference_type;
    typedef ss_typename_type_k parent_class_type::pointer                       pointer;
    typedef ss_typename_type_k parent_class_type::reference                     reference;
#endif /* STLSOFT_ITERATOR_REVERSE_ITERATOR_FORM3_SUPPORT */

    /* These two are for compatibility with older non-standard implementations, and
     * will be benignly ignored by anything not requiring them.
     */
    typedef pointer                                                             pointer_type;
    typedef reference                                                           reference_type;

// Construction
public:
    /// Constructor
    ss_explicit_k reverse_bidirectional_iterator_base(I i)
        : parent_class_type(i)
    {}
};

// class const_reverse_bidirectional_iterator_base
/// Base type for <b><code>const_reverse_bidirectional_iterator</code></b> types
//
/// This class acts as the base for const reverse bidirectional iterators,
/// insulating deriving classes from the inconsistencies and incompatibilities
/// of the various compilers and/or libraries supported by the STLSoft libraries.
///
/// \param I The iterator type
/// \param V The value type
/// \param R The reference type
/// \param P The pointer type
/// \param D The distance type
template<   ss_typename_param_k I   /* iterator */
        ,   ss_typename_param_k V   /* value type */
        ,   ss_typename_param_k R   /* reference */
        ,   ss_typename_param_k P   /* pointer */
        ,   ss_typename_param_k D   /* distance type */
        >
struct const_reverse_bidirectional_iterator_base                // For all current compilers/libraries, ...
    : public reverse_bidirectional_iterator_base<I, V, R, P, D> // ... this is the same as reverse_iterator_base
{
public:
    typedef reverse_bidirectional_iterator_base<I, V, R, P, D>                  parent_class_type;

    typedef ss_typename_type_k parent_class_type::iterator_category             iterator_category;
    typedef ss_typename_type_k parent_class_type::value_type                    value_type;
    typedef ss_typename_type_k parent_class_type::difference_type               difference_type;
    typedef ss_typename_type_k parent_class_type::pointer_type                  pointer;
    typedef ss_typename_type_k parent_class_type::reference_type                reference;

    /* These two are for compatibility with older non-standard implementations, and
     * will be benignly ignored by anything not requiring them.
     */
    typedef ss_typename_type_k parent_class_type::pointer                       pointer_type;
    typedef ss_typename_type_k parent_class_type::reference                     reference_type;

// Construction
public:
    /// Constructor
    ss_explicit_k const_reverse_bidirectional_iterator_base(I i)
        : parent_class_type(i)
    {}
};

#endif /* __STLSOFT_CF_BIDIRECTIONAL_ITERATOR_SUPPORT */

/* /////////////////////////////////////////////////////////////////////////////
 * Random access iterator support
 */

// This is all some hideous kludge caused by Dinkumware's standard library's
// failure to leave behind any definitive discriminatable vestige of its
// presence.

#ifdef __STLSOFT_CF_MIGHT_BE_DINKUMWARE_MS_NAUGHTIES
# undef __STLSOFT_CF_MIGHT_BE_DINKUMWARE_MS_NAUGHTIES
#endif /* !__STLSOFT_CF_MIGHT_BE_DINKUMWARE_MS_NAUGHTIES */

#ifdef __STLSOFT_CF_MIGHT_BE_DINKUMWARE_MS_NAUGHTIES_1300
# undef __STLSOFT_CF_MIGHT_BE_DINKUMWARE_MS_NAUGHTIES_1300
#endif /* !__STLSOFT_CF_MIGHT_BE_DINKUMWARE_MS_NAUGHTIES_1300 */

/* Detect whether Dinkumware "may" be present
 *
 * Discriminated symbol is __STLSOFT_CF_MIGHT_BE_DINKUMWARE_MS_NAUGHTIES
 */
#if defined(STLSOFT_CF_STD_LIBRARY_IS_DINKUMWARE_VC) && \
    (   STLSOFT_CF_STD_LIBRARY_DINKUMWARE_VC_VERSION == STLSOFT_CF_DINKUMWARE_VC_VERSION_6_0 || \
        STLSOFT_CF_STD_LIBRARY_DINKUMWARE_VC_VERSION == STLSOFT_CF_DINKUMWARE_VC_VERSION_7_0)
# define __STLSOFT_CF_MIGHT_BE_DINKUMWARE_MS_NAUGHTIES
#endif /* _MSC_VER && _MSC_VER == 1300 */

#if defined(__STLSOFT_CF_MIGHT_BE_DINKUMWARE_MS_NAUGHTIES) && \
    defined(_DEPRECATED) && \
    defined(_HAS_TEMPLATE_PARTIAL_ORDERING) && \
    defined(_CPPLIB_VER)
# define __STLSOFT_CF_MIGHT_BE_DINKUMWARE_MS_NAUGHTIES_1300
#endif /*  */


#ifdef __STLSOFT_CF_MIGHT_BE_DINKUMWARE_MS_NAUGHTIES

#ifndef _STLSOFT_NO_NAMESPACE
} // namespace stlsoft
#endif /* _STLSOFT_NO_NAMESPACE */

template<   ss_typename_param_k _Ty
        ,   ss_typename_param_k _Diff
        ,   ss_typename_param_k _Pointer
        ,   ss_typename_param_k _Reference
        ,   ss_typename_param_k _Pointer2
        ,   ss_typename_param_k _Reference2
        >
class _Ptrit
{
public:
    typedef _Pointer    iterator_type;

private:
    char    x[1024];
};

namespace std
{
    namespace test_dinkumware
    {
        template<   ss_typename_param_k T1
                ,   ss_typename_param_k T2
                ,   bool S
                >
        struct select_type
        {
            typedef T1  selected_type;
        };

#ifdef __STLSOFT_CF_TEMPLATE_PARTIAL_SPECIALISATION_SUPPORT
        template<   ss_typename_param_k T1
                ,   ss_typename_param_k T2
                >
        struct select_type<T1, T2, false>
        {
            typedef T2  selected_type;
        };
#endif //# ifdef __STLSOFT_CF_TEMPLATE_PARTIAL_SPECIALISATION_SUPPORT

        template<   class V
                ,   class P
                ,   class R
                >
        class _Ptrit_tdkw
        {
            typedef _Ptrit<V, ptrdiff_t, P, R, P, R>    _Ptrit_type;

        public:
            typedef ss_typename_type_k select_type<_Ptrit_type, P, sizeof(_Ptrit_type) < 1024>::selected_type  iterator_type;
        };

    }
}

#ifndef _STLSOFT_NO_NAMESPACE
namespace stlsoft
{
#endif /* _STLSOFT_NO_NAMESPACE */

#endif /* !__STLSOFT_CF_MIGHT_BE_DINKUMWARE_MS_NAUGHTIES */

/// Pointer iterator type
///
/// \param V The value type
/// \param P The pointer type
/// \param R The reference type
template<   ss_typename_param_k V
        ,   ss_typename_param_k P
        ,   ss_typename_param_k R
        >
struct pointer_iterator
{
#if defined(__STLSOFT_CF_MIGHT_BE_DINKUMWARE_MS_NAUGHTIES) && \
    !defined(STLSOFT_CF_STD_LIBRARY_IS_STLPORT)
# if defined(__STLSOFT_CF_MIGHT_BE_DINKUMWARE_MS_NAUGHTIES_1300)
    typedef std::test_dinkumware::_Ptrit_tdkw<V, P, R>::iterator_type   iterator_type;
# else
    typedef P                                                           iterator_type;
# endif /* __STLSOFT_CF_MIGHT_BE_DINKUMWARE_MS_NAUGHTIES_1300 */
#elif defined(__STLSOFT_COMPILER_IS_MSVC) && \
      !defined(STLSOFT_CF_STD_LIBRARY_IS_STLPORT) && \
      defined(_XUTILITY_) && \
      _MSC_VER == 1300
    typedef std::_Ptrit<V, ptrdiff_t, P, R, P, R>                       iterator_type;
#else
    typedef P                                                           iterator_type;
#endif /* !__STLSOFT_CF_MIGHT_BE_DINKUMWARE_MS_NAUGHTIES */
};

/* ////////////////////////////////////////////////////////////////////////// */

#if defined(__STLSOFT_COMPILER_IS_DMC) && \
    !defined(STLSOFT_CF_STD_LIBRARY_IS_STLPORT)
template<   ss_typename_param_k V
        ,   ss_typename_param_k P
        ,   ss_typename_param_k R
        >
inline random_access_iterator_tag iterator_category(pointer_iterator<V, P, R>::iterator_type const &)
{
    return random_access_iterator_tag();
}

template<   ss_typename_param_k V
        ,   ss_typename_param_k P
        ,   ss_typename_param_k R
        >
inline ptrdiff_t* distance_type(pointer_iterator<V, P, R>::iterator_type const &)
{
    return static_cast<ptrdiff_t*>(0);
}
#endif /* __STLSOFT_COMPILER_IS_DMC  && !STLSOFT_CF_STD_LIBRARY_IS_STLPORT */

/* ////////////////////////////////////////////////////////////////////////// */


/// Iterator category obtainer
///
/// \param I The iterator type
/// \param i The iterator instance



#if defined(STLSOFT_CF_STD_LIBRARY_IS_DINKUMWARE_VC)
# define stlsoft_iterator_query_category(I, i)     (stlsoft_ns_qual_std(_Iter_cat)(i))

//#elif defined(STLSOFT_CF_STD_LIBRARY_IS_STLPORT)
//# define stlsoft_iterator_query_category(I, i)     (*static_cast<std::iterator_traits<I>::iterator_category*>(0))

//#elif defined(__STLSOFT_COMPILER_IS_BORLAND) // Change this to STLSOFT_CF_STD_LIBRARY_IS_SGI_RW
//# define stlsoft_iterator_query_category(I, i)     (*static_cast<std::iterator_traits<I>::iterator_category*>(0))

#else
//#if defined(STLSOFT_CF_STD_LIBRARY_IS_STLPORT)
# define stlsoft_iterator_query_category(I, i)     (*static_cast<ss_typename_type_k std::iterator_traits<I>::iterator_category*>(0))
//#else
//#  define stlsoft_iterator_query_category(I, i)     (stlsoft_ns_qual_std(iterator_category)(i))
#endif /* library / compiler */

#if 0
#    if defined(__STLSOFT_COMPILER_IS_DMC)
#     if defined(STLSOFT_CF_STD_LIBRARY_IS_STLPORT)
#      define stlsoft_iterator_query_category(I, i)     (stlsoft_ns_qual_std(iterator_traits)<I>::iterator_category())
    //#  error Digital Mars with STLport not yet supported
#     else
#      define stlsoft_iterator_query_category(I, i)     (stlsoft_ns_qual_std(iterator_category)(i))
#     endif /*  */
#    elif defined(__STLSOFT_COMPILER_IS_COMO) || \
          defined(__STLSOFT_COMPILER_IS_INTEL)
#     if defined(STLSOFT_CF_STD_LIBRARY_IS_STLPORT)
#      define stlsoft_iterator_query_category(I, i)     (stlsoft_ns_qual_std(iterator_traits)<I>::iterator_category())
#     elif defined(__STLSOFT_CF_MIGHT_BE_DINKUMWARE_MS_NAUGHTIES)
#      define stlsoft_iterator_query_category(I, i)     (stlsoft_ns_qual_std(_Iter_cat)(i))
#     else
#      error
#     endif /*  */
#    elif defined(__STLSOFT_COMPILER_IS_MSVC)
#     if defined(STLSOFT_CF_STD_LIBRARY_IS_STLPORT)
#      if _MSC_VER < 1300
#       define stlsoft_iterator_query_category(I, i)    (stlsoft_ns_qual_std(iterator_category)(i))
#      else
#       define stlsoft_iterator_query_category(I, i)    (stlsoft_ns_qual_std(iterator_category)(i))
#      endif /* _MSC_VER < 1300 */
#     elif defined(__STLSOFT_CF_MIGHT_BE_DINKUMWARE_MS_NAUGHTIES)
#      define stlsoft_iterator_query_category(I, i)     (stlsoft_ns_qual_std(_Iter_cat)(i))
#     elif(_MSC_VER >= 1310)
#      define stlsoft_iterator_query_category(I, i)     (stlsoft_ns_qual_std(iterator_traits)<I>::iterator_category())
#     elif(_MSC_VER >= 1200)
#      error
#     endif /*  */
#    else
#     define stlsoft_iterator_query_category(I, i)      (stlsoft_ns_qual_std(iterator_traits)<I>::iterator_category())
#    endif /* __STLSOFT_CF_MIGHT_BE_DINKUMWARE_MS_NAUGHTIES && !STLSOFT_CF_STD_LIBRARY_IS_STLPORT */
#endif /* 0 */

#if 0
template <typename T>
struct queried_iterator_category
{
};

template <typename T>
query_iterator_category
#endif /* 0 */

////////////////////////////////////////////////////////////////////////////////
// Unit-testing

#ifdef STLSOFT_UNITTEST

namespace unittest
{
    ss_bool_t test_stlsoft_iterator(unittest_reporter *r)
    {
        ss_bool_t               bSuccess    =   true;

        unittest_initialiser    init(r, "STLSoft", "iterator", __FILE__);

#if 0
        if(<<TODO>>)
        {
            r->report("<<TODO>> failed ", __LINE__);
            bSuccess = false;
        }
#endif /* 0 */

        return bSuccess;
    }

    unittest_registrar    unittest_stlsoft_iterator(test_stlsoft_iterator);

} // namespace unittest

#endif /* STLSOFT_UNITTEST */

/* ////////////////////////////////////////////////////////////////////////// */

#ifndef _STLSOFT_NO_NAMESPACE
} // namespace stlsoft
#endif /* _STLSOFT_NO_NAMESPACE */

/* ////////////////////////////////////////////////////////////////////////// */

#endif /* STLSOFT_INCL_H_STLSOFT_ITERATOR */

/* ////////////////////////////////////////////////////////////////////////// */
