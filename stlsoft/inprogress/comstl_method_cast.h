/* /////////////////////////////////////////////////////////////////////////////
 * File:        comstl_method_cast.h
 *
 * Purpose:     COM memory functions.
 *
 * Created:     20th December 2003
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


/// \file comstl_method_cast.h
///
/// COM memory functions.

#ifndef COMSTL_INCL_H_COMSTL_METHOD_CAST
#define COMSTL_INCL_H_COMSTL_METHOD_CAST

#ifndef __STLSOFT_DOCUMENTATION_SKIP_SECTION
# define COMSTL_VER_H_COMSTL_METHOD_CAST_MAJOR      1
# define COMSTL_VER_H_COMSTL_METHOD_CAST_MINOR      0
# define COMSTL_VER_H_COMSTL_METHOD_CAST_REVISION   6
# define COMSTL_VER_H_COMSTL_METHOD_CAST_EDIT       7
#endif /* !__STLSOFT_DOCUMENTATION_SKIP_SECTION */

/* ////////////////////////////////////////////////////////////////////////////
 * Includes
 */

#ifndef COMSTL_INCL_H_COMSTL
# include "comstl.h"                // Include the COMSTL root header
#endif /* !COMSTL_INCL_H_COMSTL */

/* /////////////////////////////////////////////////////////////////////////////
 * Namespace
 *
 * The COMSTL components are contained within the comstl namespace. This is
 * actually an alias for stlsoft::comstl_project,
 *
 * The definition matrix is as follows:
 *
 * _STLSOFT_NO_NAMESPACE    _COMSTL_NO_NAMESPACE    comstl definition
 * ---------------------    --------------------    -----------------
 *  not defined              not defined             = stlsoft::comstl_project
 *  not defined              defined                 not defined
 *  defined                  not defined             comstl
 *  defined                  defined                 not defined
 *
 */

#ifndef _COMSTL_NO_NAMESPACE
# if defined(_STLSOFT_NO_NAMESPACE) || \
     defined(__STLSOFT_DOCUMENTATION_SKIP_SECTION)
/* There is no stlsoft namespace, so must define ::comstl */
namespace comstl
{
# else
/* Define stlsoft::comstl_project */

namespace stlsoft
{

namespace comstl_project
{

# endif /* _STLSOFT_NO_NAMESPACE */
#endif /* !_COMSTL_NO_NAMESPACE */

/* ////////////////////////////////////////////////////////////////////////// */

/// \defgroup comstl_method_cast COM Memory Functions
/// \ingroup COMSTL
/// \brief These functions expand on the COM API's <code>CoTaskMem*()</code> suite of functions
/// @{

/* /////////////////////////////////////////////////////////////////////////////
 * Functions
 */

#if 0
template<   typename R
        ,   typename V
        >
inline R &transfer_resource(R &r, V v)
{
//  comstl_message_assert("Resource destination not empty!", 0 == r);
    r = v;

    return r;
}
#endif /* 0 */

inline ss_sint8_t &transfer_resource(ss_sint8_t &r, ss_sint8_t v)               { return (r = v); }
inline ss_uint8_t &transfer_resource(ss_uint8_t &r, ss_uint8_t v)               { return (r = v); }
inline ss_sint16_t &transfer_resource(ss_sint16_t &r, ss_sint16_t v)            { return (r = v); }
inline ss_uint16_t &transfer_resource(ss_uint16_t &r, ss_uint16_t v)            { return (r = v); }
inline ss_sint32_t &transfer_resource(ss_sint32_t &r, ss_sint32_t v)            { return (r = v); }
inline ss_uint32_t &transfer_resource(ss_uint32_t &r, ss_uint32_t v)            { return (r = v); }
#ifdef STLSOFT_CF_64BIT_INT_SUPPORT
inline ss_sint64_t &transfer_resource(ss_sint64_t &r, ss_sint64_t v)            { return (r = v); }
inline ss_uint64_t &transfer_resource(ss_uint64_t &r, ss_uint64_t v)            { return (r = v); }
#endif /* !STLSOFT_CF_64BIT_INT_SUPPORT */
#if (   defined(__STLSOFT_COMPILER_IS_INTEL) || \
        defined(__STLSOFT_COMPILER_IS_MSVC)) && \
    _MSC_VER == 1200
inline signed char &transfer_resource(signed char &r, signed char v)            { return (r = v); }
inline unsigned char &transfer_resource(unsigned char &r, unsigned char v)      { return (r = v); }
inline signed short &transfer_resource(signed short &r, signed short v)         { return (r = v); }
inline unsigned short &transfer_resource(unsigned short &r, unsigned short v)   { return (r = v); }
inline signed int &transfer_resource(signed int &r, signed int v)               { return (r = v); }
inline unsigned int &transfer_resource(unsigned int &r, unsigned int v)         { return (r = v); }
#endif /* _MSC_VER == 1200 */
#if (   defined(__STLSOFT_COMPILER_IS_INTEL) || \
        defined(__STLSOFT_COMPILER_IS_MSVC)) && \
    (   _MSC_VER == 1200 || \
        _MSC_VER == 1300 || \
        _MSC_VER == 1310)
inline signed long &transfer_resource(signed long &r, signed long v)            { return (r = v); }
inline unsigned long &transfer_resource(unsigned long &r, unsigned long v)      { return (r = v); }
#endif /* _MSC_VER == 1310 */
inline float &transfer_resource(float &r, float v)                              { return (r = v); }
inline double &transfer_resource(double &r, double v)                           { return (r = v); }
inline long double &transfer_resource(long double &r, long double v)            { return (r = v); }

template<   typename R
        ,   typename V
        >
inline R *&transfer_resource(R *&r, V *v)
{
    comstl_message_assert("Resource destination not empty!", 0 == r);

    return (r = v);
}


template<   typename R
        ,   typename C
        ,   typename V
        >
inline R method_cast(C &c, HRESULT (C::*pfn)(V *))
{
    R       r;
    V       v;
    HRESULT hr = (c.*pfn)(&v);

    if(SUCCEEDED(hr))
    {
        return transfer_resource(r, v);
    }

    return r;
}

template<   typename R
        ,   typename C
        ,   typename V
        >
inline R method_cast(C *c, HRESULT (STDAPICALLTYPE C::*pfn)(V *))
{
    R       r = R();
    V       v;
    HRESULT hr = (c->*pfn)(&v);

    if(SUCCEEDED(hr))
    {
        return transfer_resource(r, v);
    }

    return r;
}

/* ////////////////////////////////////////////////////////////////////////// */

/// @} // end of group comstl_method_cast

/* ////////////////////////////////////////////////////////////////////////// */

#ifndef _COMSTL_NO_NAMESPACE
# if defined(_STLSOFT_NO_NAMESPACE) || \
     defined(__STLSOFT_DOCUMENTATION_SKIP_SECTION)
} // namespace comstl
# else
} // namespace stlsoft::comstl_project
} // namespace stlsoft
# endif /* _STLSOFT_NO_NAMESPACE */
#endif /* !_COMSTL_NO_NAMESPACE */

/* ////////////////////////////////////////////////////////////////////////// */

#endif /* !COMSTL_INCL_H_COMSTL_METHOD_CAST */

/* ////////////////////////////////////////////////////////////////////////// */
