/* /////////////////////////////////////////////////////////////////////////////
 * File:        atlstl_support_error_info.h (formerly MAErInfo.h, ::SynesisAtl)
 *
 * Purpose:     SupportErrorInfoImpl class.
 *
 * Created:     17th April 1999
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


/// \file atlstl_support_error_info.h
///
/// SupportErrorInfoImpl class.

#ifndef ATLSTL_INCL_H_ATLSTL_SUPPORT_ERROR_INFO
#define ATLSTL_INCL_H_ATLSTL_SUPPORT_ERROR_INFO

#ifndef __STLSOFT_DOCUMENTATION_SKIP_SECTION
# define ATLSTL_VER_H_ATLSTL_SUPPORT_ERROR_INFO_MAJOR      3
# define ATLSTL_VER_H_ATLSTL_SUPPORT_ERROR_INFO_MINOR      0
# define ATLSTL_VER_H_ATLSTL_SUPPORT_ERROR_INFO_REVISION   1
# define ATLSTL_VER_H_ATLSTL_SUPPORT_ERROR_INFO_EDIT       49
#endif /* !__STLSOFT_DOCUMENTATION_SKIP_SECTION */

/* /////////////////////////////////////////////////////////////////////////////
 * Includes
 */

#ifndef ATLSTL_INCL_H_ATLSTL
# include "atlstl.h"    // Include the ATLSTL root header
#endif /* !ATLSTL_INCL_H_ATLSTL */

/* /////////////////////////////////////////////////////////////////////////////
 * Namespace
 *
 * The ATLSTL components are contained within the atlstl namespace. This is
 * actually an alias for stlsoft::atlstl_project,
 *
 * The definition matrix is as follows:
 *
 * _STLSOFT_NO_NAMESPACE    _ATLSTL_NO_NAMESPACE    atlstl definition
 * ---------------------    --------------------    -----------------
 *  not defined              not defined             = stlsoft::atlstl_project
 *  not defined              defined                 not defined
 *  defined                  not defined             atlstl
 *  defined                  defined                 not defined
 *
 */

#ifndef _ATLSTL_NO_NAMESPACE
# if defined(_STLSOFT_NO_NAMESPACE) || \
     defined(__STLSOFT_DOCUMENTATION_SKIP_SECTION)
/* There is no stlsoft namespace, so must define ::atlstl */
namespace atlstl
{
# else
/* Define stlsoft::atlstl_project */

namespace stlsoft
{

namespace atlstl_project
{

# endif /* _STLSOFT_NO_NAMESPACE */
#endif /* !_ATLSTL_NO_NAMESPACE */

/* ////////////////////////////////////////////////////////////////////////// */

/// \defgroup atlstl_interface_classes Interface Classes
/// \ingroup ATLSTL
/// \brief Interface (template) classes
/// @{

/* /////////////////////////////////////////////////////////////////////////////
 * Classes
 */

/// Provides implementation of ISupportErrorInfo for support for errors on one
/// interface
template <const IID *piid>
class ATL_NO_VTABLE SupportErrorInfoImpl
    : public ISupportErrorInfo
{
public:
    typedef SupportErrorInfoImpl<piid>  Class;

// ISupportErrorInfo
public:
    STDMETHOD(InterfaceSupportsErrorInfo)(REFIID riid)
    {
        return (InlineIsEqualGUID(riid, *piid))
                    ? S_OK
                    : S_FALSE;
    }
};


/// Provides implementation of ISupportErrorInfo for support for errors on two
/// interfaces
template <const IID *piid1, const IID *piid2>
class ATL_NO_VTABLE SupportErrorInfoImpl2
    : public ISupportErrorInfo
{
public:
    typedef SupportErrorInfoImpl2<piid1, piid2> Class;

// ISupportErrorInfo
public:
    STDMETHOD(InterfaceSupportsErrorInfo)(REFIID riid)
    {
        return (InlineIsEqualGUID(riid, *piid1) ||
                InlineIsEqualGUID(riid, *piid2))
                    ? S_OK
                    : S_FALSE;
    }
};


/// Provides implementation of ISupportErrorInfo for support for errors on three
/// interfaces
template <const IID *piid1, const IID *piid2, const IID *piid3>
class ATL_NO_VTABLE SupportErrorInfoImpl3
    : public ISupportErrorInfo
{
public:
    typedef SupportErrorInfoImpl3<piid1, piid2, piid3>  Class;

// ISupportErrorInfo
public:
    STDMETHOD(InterfaceSupportsErrorInfo)(REFIID riid)
    {
        return (InlineIsEqualGUID(riid, *piid1) ||
                InlineIsEqualGUID(riid, *piid2) ||
                InlineIsEqualGUID(riid, *piid3))
                    ? S_OK
                    : S_FALSE;
    }
};

/* ////////////////////////////////////////////////////////////////////////// */

/// @} // end of group atlstl_interface_classes

/* ////////////////////////////////////////////////////////////////////////// */

#ifndef _ATLSTL_NO_NAMESPACE
# if defined(_STLSOFT_NO_NAMESPACE) || \
     defined(__STLSOFT_DOCUMENTATION_SKIP_SECTION)
} // namespace atlstl
# else
} // namespace atlstl_project
} // namespace stlsoft
# endif /* _STLSOFT_NO_NAMESPACE */
#endif /* !_ATLSTL_NO_NAMESPACE */

/* ////////////////////////////////////////////////////////////////////////// */

#endif /* !ATLSTL_INCL_H_ATLSTL_SUPPORT_ERROR_INFO */

/* ////////////////////////////////////////////////////////////////////////// */
