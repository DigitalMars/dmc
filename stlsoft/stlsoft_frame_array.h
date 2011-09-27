/* /////////////////////////////////////////////////////////////////////////////
 * File:        stlsoft_frame_array.h (originally MTArray.h, ::SynesisStl)
 *
 * Purpose:     Contains the static_array template class.
 *
 * Created:     4th August 1998
 * Updated:     11th September 2004
 *
 * Home:        http://stlsoft.org/
 *
 * Copyright (c) 1998-2004, Matthew Wilson and Synesis Software
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


#ifndef STLSOFT_INCL_H_STLSOFT_FRAME_ARRAY
#define STLSOFT_INCL_H_STLSOFT_FRAME_ARRAY

#ifndef __STLSOFT_DOCUMENTATION_SKIP_SECTION
# define STLSOFT_VER_H_STLSOFT_FRAME_ARRAY_MAJOR        2
# define STLSOFT_VER_H_STLSOFT_FRAME_ARRAY_MINOR        0
# define STLSOFT_VER_H_STLSOFT_FRAME_ARRAY_REVISION     1
# define STLSOFT_VER_H_STLSOFT_FRAME_ARRAY_EDIT         120
#endif /* !__STLSOFT_DOCUMENTATION_SKIP_SECTION */

/* /////////////////////////////////////////////////////////////////////////////
 * Includes
 */

/// \file stlsoft_frame_array.h
///
/// This file is obsolete, and will be removed in a future version. You should include stlsoft_static_array.h

#include "stlsoft.h"                   // Include the STLSoft root header
#ifdef _STLSOFT_STRICT
# error This file is obsolete, and will be removed in a forthcoming release. The frame_array_* classes are renamed as static_array_* and are now located in stlsoft_static_array.h
#else
# pragma message("This file is obsolete, and will be removed in a forthcoming release. The frame_array_* classes are renamed as static_array_* and are now located in stlsoft_static_array.h. Including now")
# include "stlsoft_static_array.h"
#endif /* _STLSOFT_STRICT */

/* ////////////////////////////////////////////////////////////////////////// */

#endif /* !STLSOFT_INCL_H_STLSOFT_FRAME_ARRAY */

/* ////////////////////////////////////////////////////////////////////////// */
