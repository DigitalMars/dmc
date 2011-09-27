/* /////////////////////////////////////////////////////////////////////////////
 * File:        stlsoft_cc_warnings.h
 *
 * Purpose:     Included by the root header for the STLSoft libraries. Adjusts
 *              warning settings for the supported compilers.
 *
 * Created:     15th January 2002
 * Updated:     2nd July 2003
 *
 * Author:      Matthew Wilson, Synesis Software Pty Ltd.
 *
 * License:     (Licensed under the Synesis Software Standard Source License)
 *
 *              Copyright (C) 2002-2003, Synesis Software Pty Ltd.
 *
 *              All rights reserved.
 *
 *              www:        http://www.synesis.com.au/stlsoft
 *                          http://www.stlsoft.org/
 *
 *              email:      submissions@stlsoft.org  for submissions
 *                          admin@stlsoft.org        for other enquiries
 *
 *              Redistribution and use in source and binary forms, with or
 *              without modification, are permitted provided that the following
 *              conditions are met:
 *
 *              (i) Redistributions of source code must retain the above
 *              copyright notice and contact information, this list of
 *              conditions and the following disclaimer.
 *
 *              (ii) Any derived versions of this software (howsoever modified)
 *              remain the sole property of Synesis Software.
 *
 *              (iii) Any derived versions of this software (howsoever modified)
 *              remain subject to all these conditions.
 *
 *              (iv) Neither the name of Synesis Software nor the names of any
 *              subdivisions, employees or agents of Synesis Software, nor the
 *              names of any other contributors to this software may be used to
 *              endorse or promote products derived from this software without
 *              specific prior written permission.
 *
 *              This source code is provided by Synesis Software "as is" and any
 *              warranties, whether expressed or implied, including, but not
 *              limited to, the implied warranties of merchantability and
 *              fitness for a particular purpose are disclaimed. In no event
 *              shall the Synesis Software be liable for any direct, indirect,
 *              incidental, special, exemplary, or consequential damages
 *              (including, but not limited to, procurement of substitute goods
 *              or services; loss of use, data, or profits; or business
 *              interruption) however caused and on any theory of liability,
 *              whether in contract, strict liability, or tort (including
 *              negligence or otherwise) arising in any way out of the use of
 *              this software, even if advised of the possibility of such
 *              damage.
 *
 * ////////////////////////////////////////////////////////////////////////// */

#error This file is now obsolete. As from STLSoft v1.5.2 compiler warnings are handled in the compiler capability files, e.g. stlsoft_cccap_mwerks.h

#ifndef _STLSOFT_INCL_H_STLSOFT
 #error stlsoft_cc_warnings.h can only be included from within stlsoft.h, and should not be included directly
#endif /* !_STLSOFT_INCL_H_STLSOFT */

/* File version */
#define _STLSOFT_VER_H_STLSOFT_CC_WARNING_MAJOR     1
#define _STLSOFT_VER_H_STLSOFT_CC_WARNING_MINOR     2
#define _STLSOFT_VER_H_STLSOFT_CC_WARNING_REVISION  1
#define _STLSOFT_VER_H_STLSOFT_CC_WARNING_EDIT      9

/* /////////////////////////////////////////////////////////////////////////////
 * Warnings
 *
 * Certain compiler warnings are not useful, and are disabled here.
 */

/* Borland C/C++ warnings
 */
#ifdef __STLSOFT_COMPILER_IS_BORLAND

 #if __BORLANDC__ >= 0x0550

  /* Borland: "Functions with exception specifications are not expanded inline" */
  #pragma warn -8026

  /* Borland: "Functions containing reserved words are not expanded inline" */
  #pragma warn -8027

 #endif /* __BORLANDC__ >= 0x0550 */

#endif /* __STLSOFT_COMPILER_IS_BORLAND */

/* Visual C++ warnings
 */
#ifdef __STLSOFT_COMPILER_IS_MSVC

 /* MSVC: "identifier was truncated to '255' characters in the browser information" */
 #pragma warning(disable : 4786)

 /* Suppresses: "unreferenced inline function has been removed" */
 #pragma warning(disable : 4514)

 /* Suppresses: "function not expanded" */
 #pragma warning(disable : 4710)

 /* Suppresses: "conditional expression is constant" */
 #pragma warning(disable : 4127)

 /* Suppresses: "nonstandard extension used : nameless struct/union" */
 #pragma warning(disable : 4201)

 /* Suppresses: "C++ language change: to explicitly specialize class template 'X' use the following syntax: template<> struct X<Y>" */
 #pragma warning(disable : 4663)

#endif /* __STLSOFT_COMPILER_IS_MSVC */

/* ////////////////////////////////////////////////////////////////////////// */
