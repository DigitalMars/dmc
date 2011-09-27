/* ////////////////////////////////////////////////////////////////////////////
 * File:        comstl_errorinfo_functions.h
 *
 * Purpose:     Error info functions.
 *
 * Created:
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


/// \file comstl_errorinfo_functions.h
///
/// Error info functions.

#ifndef COMSTL_INCL_H_COMSTL_ERRORINFO_FUNCTIONS
#define COMSTL_INCL_H_COMSTL_ERRORINFO_FUNCTIONS

#ifndef __STLSOFT_DOCUMENTATION_SKIP_SECTION
# define COMSTL_VER_H_COMSTL_ERRORINFO_FUNCTIONS_MAJOR      2
# define COMSTL_VER_H_COMSTL_ERRORINFO_FUNCTIONS_MINOR      0
# define COMSTL_VER_H_COMSTL_ERRORINFO_FUNCTIONS_REVISION   1
# define COMSTL_VER_H_COMSTL_ERRORINFO_FUNCTIONS_EDIT       11
#endif /* !__STLSOFT_DOCUMENTATION_SKIP_SECTION */

/* ////////////////////////////////////////////////////////////////////////////
 * Includes
 */

#ifndef COMSTL_INCL_H_COMSTL
# include "comstl.h"    // Include the COMSTL root header
#endif /* !COMSTL_INCL_H_COMSTL */
#ifdef STLSOFT_UNITTEST
# include <wchar.h>
#endif /* STLSOFT_UNITTEST */

/* ////////////////////////////////////////////////////////////////////////////
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

/// \weakgroup comstl_error_library Error Library
/// \ingroup COMSTL libraries
/// \brief This library provides facilities for manipulating COM error information
/// @{

/* ////////////////////////////////////////////////////////////////////////////
 * Functions
 */

inline HRESULT set_error_info(LPCOLESTR desc)
{
    ICreateErrorInfo    *pCEI;
    HRESULT             hr = ::CreateErrorInfo(&pCEI);

    if(SUCCEEDED(hr))
    {
        IErrorInfo  *pEI;

        // Ask for IErrorInfo
        hr = pCEI->QueryInterface(IID_IErrorInfo, (void**)&pEI);

        if(SUCCEEDED(hr))
        {
            hr = pCEI->SetDescription(const_cast<LPOLESTR>(desc));

            ::SetErrorInfo(0, pEI);

            pEI->Release();
        }

        pCEI->Release();
    }

    return hr;
}

////////////////////////////////////////////////////////////////////////////////
// Unit-testing

#ifdef STLSOFT_UNITTEST

namespace unittest
{
    ss_bool_t test_comstl_errorinfo_functions(unittest_reporter *r)
    {
        using stlsoft::unittest::unittest_initialiser;

        ss_bool_t               bSuccess    =   true;

        unittest_initialiser    init(r, "COMSTL", "errorinfo_functions", __FILE__);

        set_error_info(L"Test info #1");

        IErrorInfo  *pei;
        HRESULT     hr  =   ::GetErrorInfo(0, &pei);

        if(SUCCEEDED(hr))
        {
            BSTR    bstrDesc;

            hr = pei->GetDescription(&bstrDesc);

            if(SUCCEEDED(hr))
            {
                if(0 != wcscmp(bstrDesc, L"Test info #1"))
                {
                    r->report("Error information not as expected ", __LINE__);
                    bSuccess = false;
                }

                ::SysFreeString(bstrDesc);
            }

            pei->Release();
        }

        return bSuccess;
    }

    unittest_registrar    unittest_comstl_errorinfo_functions(test_comstl_errorinfo_functions);

} // namespace unittest

#endif /* STLSOFT_UNITTEST */

/* ////////////////////////////////////////////////////////////////////////// */

/// @} // end of group comstl_error_library

/* ////////////////////////////////////////////////////////////////////////// */

#ifndef _COMSTL_NO_NAMESPACE
# if defined(_STLSOFT_NO_NAMESPACE) || \
     defined(__STLSOFT_DOCUMENTATION_SKIP_SECTION)
} // namespace comstl
# else
} // namespace comstl_project
} // namespace stlsoft
# endif /* _STLSOFT_NO_NAMESPACE */
#endif /* !_COMSTL_NO_NAMESPACE */

/* ////////////////////////////////////////////////////////////////////////// */

#endif /* !COMSTL_INCL_H_COMSTL_ERRORINFO_FUNCTIONS */

/* ////////////////////////////////////////////////////////////////////////// */
