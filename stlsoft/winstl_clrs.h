/* /////////////////////////////////////////////////////////////////////////////
 * File:        winstl_clrs.h (formerly MWWinDef.h, ::SynesisWin)
 *
 * Purpose:     Colour constants.
 *
 * Created:     20th February 1999
 * Updated:     11th September 2004
 *
 * Home:        http://stlsoft.org/
 *
 * Copyright (c) 1999-2004, Matthew Wilson and Synesis Software
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


/// \file winstl_clrs.h
///
/// Colour constants.

#ifndef WINSTL_INCL_H_WINSTL_CLRS
#define WINSTL_INCL_H_WINSTL_CLRS

#ifndef __STLSOFT_DOCUMENTATION_SKIP_SECTION
# define WINSTL_VER_H_WINSTL_CLRS_MAJOR     2
# define WINSTL_VER_H_WINSTL_CLRS_MINOR     0
# define WINSTL_VER_H_WINSTL_CLRS_REVISION  1
# define WINSTL_VER_H_WINSTL_CLRS_EDIT      12
#endif /* !__STLSOFT_DOCUMENTATION_SKIP_SECTION */

/* ////////////////////////////////////////////////////////////////////////////
 * Includes
 */

#ifndef WINSTL_INCL_H_WINSTL
# include "winstl.h"                // Include the WinSTL root header
#endif /* !WINSTL_INCL_H_WINSTL */

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

/* /////////////////////////////////////////////////////////////////////////////
 * Constants
 */

const COLORREF  white           =   RGB(255, 255, 255);
const COLORREF  black           =   RGB(  0,   0,   0);
const COLORREF  darkblue        =   RGB(  0,   0, 128);
const COLORREF  darkgreen       =   RGB(  0, 128,   0);
const COLORREF  bluegreen       =   RGB(  0, 128, 128);
const COLORREF  brown           =   RGB(128,   0,   0);
const COLORREF  olive           =   RGB(128, 128,   0);
const COLORREF  purple          =   RGB(128,   0, 128);
const COLORREF  darkgrey        =   RGB(128, 128, 128);
const COLORREF  lightgrey       =   RGB(192, 192, 192);
const COLORREF  blue            =   RGB(  0,   0, 255);
const COLORREF  green           =   RGB(  0, 255,   0);
const COLORREF  cyan            =   RGB(  0, 255, 255);
const COLORREF  red             =   RGB(255,   0,   0);
const COLORREF  yellow          =   RGB(255, 255,   0);
const COLORREF  magenta         =   RGB(255,   0, 255);

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

#endif /* WINSTL_INCL_H_WINSTL_CLRS */

/* ////////////////////////////////////////////////////////////////////////// */
