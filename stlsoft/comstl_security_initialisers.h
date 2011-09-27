/* /////////////////////////////////////////////////////////////////////////////
 * File:        comstl_security_initialisers.h (originally MOSecFns.h, ::SynesisCom)
 *
 * Purpose:     Contains classes for initialising COM/OLE.
 *
 * Created:     1st February 1998
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


/// \file comstl_security_initialisers.h
///
/// Contains classes for initialising COM/OLE.

#ifndef COMSTL_INCL_H_COMSTL_SECURITY_INITIALISERS
#define COMSTL_INCL_H_COMSTL_SECURITY_INITIALISERS

#ifndef __STLSOFT_DOCUMENTATION_SKIP_SECTION
# define COMSTL_VER_H_COMSTL_SECURITY_INITIALISERS_MAJOR        3
# define COMSTL_VER_H_COMSTL_SECURITY_INITIALISERS_MINOR        0
# define COMSTL_VER_H_COMSTL_SECURITY_INITIALISERS_REVISION     1
# define COMSTL_VER_H_COMSTL_SECURITY_INITIALISERS_EDIT         21
#endif /* !__STLSOFT_DOCUMENTATION_SKIP_SECTION */

/* ////////////////////////////////////////////////////////////////////////////
 * Includes
 */

#ifndef COMSTL_INCL_H_COMSTL
# include "comstl.h"                // Include the COMSTL root header
#endif /* !COMSTL_INCL_H_COMSTL */

#if (   !defined(_WIN32_WINNT) || \
        (_WIN32_WINNT < 0x0400)) && \
    !defined(_WIN32_DCOM)
# error This file can only be used in the context of DCOM compilations.
#endif /* _WIN32_WINNT < 0x0400 && !_WIN32_DCOM */

#if _MSC_VER >= 1200
# include <iaccess.h>
#endif /* _MSC_VER >= 1200 */

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

/// \defgroup comstl_security_intialisers Security Initialiser Functions
/// \ingroup COMSTL
/// \brief These functions initialise the COM security libraries
/// @{

/* /////////////////////////////////////////////////////////////////////////////
 * Constants & definitions.
 */

#ifndef __STLSOFT_DOCUMENTATION_SKIP_SECTION
const DWORD _EOAC_SEC_DESC          =   0x0;
const DWORD _EOAC_ACCESS_CONTROL    =   0x4;
const DWORD _EOAC_APPID             =   0x8;
const DWORD _EOAC_SECURITY_MASK     =   (   _EOAC_SEC_DESC |
                                            _EOAC_APPID |
                                            _EOAC_ACCESS_CONTROL);
#endif /* !__STLSOFT_DOCUMENTATION_SKIP_SECTION */

/* /////////////////////////////////////////////////////////////////////////////
 * Functions
 */

// function CoInitSecurity
//
// The three overloaded CoInitSecurity functions
// provide a type-safe interface to the CoInitializeSecurity
// API, discriminating between IAccessControl, AppID, and
// SECURITY_DESCRIPTOR security information parameters.

/// Initialises the COM security libraries with an IAccessControl instance
///
#ifdef __IAccessControl_INTERFACE_DEFINED__
inline HRESULT CoInitSecurity(  LPUNKNOWN                   punkAccessControl,
                                DWORD                       cAuthSvc,
                                SOLE_AUTHENTICATION_SERVICE *asAuthSvc,
                                DWORD                       dwAuthnLevel,
                                DWORD                       dwImpLevel,
                                RPC_AUTH_IDENTITY_HANDLE    pAuthInfo,
                                DWORD                       dwCapabilities)
{
    HRESULT         hr;
    IAccessControl  *pac;

    /* Ensure correct flag. */
    dwCapabilities &= ~_EOAC_SECURITY_MASK;
    dwCapabilities |= _EOAC_ACCESS_CONTROL;

    hr = punkAccessControl->QueryInterface(IID_IAccessControl, (void**)&pac);

    if(SUCCEEDED(hr))
    {
        hr = ::CoInitializeSecurity(pac, cAuthSvc, asAuthSvc, NULL, dwAuthnLevel, dwImpLevel, pAuthInfo, dwCapabilities, NULL);

        pac->Release();
    }

    return hr;
}
#endif // __IAccessControl_INTERFACE_DEFINED__

/// Initialises the COM security libraries with an APPID
///
inline HRESULT CoInitSecurity(  const GUID                  &appid,
                                DWORD                       cAuthSvc,
                                SOLE_AUTHENTICATION_SERVICE *asAuthSvc,
                                DWORD                       dwAuthnLevel,
                                DWORD                       dwImpLevel,
                                RPC_AUTH_IDENTITY_HANDLE    pAuthInfo,
                                DWORD                       dwCapabilities)
{
    /* Ensure correct flag. */
    dwCapabilities &= ~_EOAC_SECURITY_MASK;
    dwCapabilities |= _EOAC_APPID;

    return ::CoInitializeSecurity((LPVOID)&appid, cAuthSvc, asAuthSvc, NULL, dwAuthnLevel, dwImpLevel, pAuthInfo, dwCapabilities, NULL);
}

/// Initialises the COM security libraries with a security descriptor
///
inline HRESULT CoInitSecurity(  SECURITY_DESCRIPTOR         *psd,
                                DWORD                       cAuthSvc,
                                SOLE_AUTHENTICATION_SERVICE *asAuthSvc,
                                DWORD                       dwAuthnLevel,
                                DWORD                       dwImpLevel,
                                RPC_AUTH_IDENTITY_HANDLE    pAuthInfo,
                                DWORD                       dwCapabilities)
{
    /* Ensure correct flag. */
    dwCapabilities &= ~_EOAC_SECURITY_MASK;
    dwCapabilities |= _EOAC_SEC_DESC;

    return ::CoInitializeSecurity(psd, cAuthSvc, asAuthSvc, NULL, dwAuthnLevel, dwImpLevel, pAuthInfo, dwCapabilities, NULL);
}

////////////////////////////////////////////////////////////////////////////////
// Unit-testing

#ifdef STLSOFT_UNITTEST

namespace unittest
{
    ss_bool_t test_comstl_security_initialisers(unittest_reporter *r)
    {
        using stlsoft::unittest::unittest_initialiser;

        ss_bool_t               bSuccess    =   true;

        unittest_initialiser    init(r, "COMSTL", "security_initialisers", __FILE__);

#if 0
        if(<<TODO>>)
        {
            r->report("<<TODO>> failed ", __LINE__);
            bSuccess = false;
        }
#endif /* 0 */

        return bSuccess;
    }

    unittest_registrar    unittest_comstl_security_initialisers(test_comstl_security_initialisers);

} // namespace unittest

#endif /* STLSOFT_UNITTEST */

/* ////////////////////////////////////////////////////////////////////////// */

/// @} // end of group comstl_security_intialisers

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

#endif /* !COMSTL_INCL_H_COMSTL_SECURITY_INITIALISERS */

/* ////////////////////////////////////////////////////////////////////////// */
