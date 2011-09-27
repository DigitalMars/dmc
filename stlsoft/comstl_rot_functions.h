/* /////////////////////////////////////////////////////////////////////////////
 * File:        comstl_rot_functions.h (originally MORotFns.h, ::SynesisCom)
 *
 * Purpose:     COM ROT (Running Object Table) functions.
 *
 * Created:     21st October 1998
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


/// \file comstl_rot_functions.h
///
/// COM ROT (Running Object Table) functions.

#ifndef COMSTL_INCL_H_COMSTL_ROT_FUNCTIONS
#define COMSTL_INCL_H_COMSTL_ROT_FUNCTIONS

#ifndef __STLSOFT_DOCUMENTATION_SKIP_SECTION
# define COMSTL_VER_H_COMSTL_ROT_FUNCTIONS_MAJOR    3
# define COMSTL_VER_H_COMSTL_ROT_FUNCTIONS_MINOR    0
# define COMSTL_VER_H_COMSTL_ROT_FUNCTIONS_REVISION 1
# define COMSTL_VER_H_COMSTL_ROT_FUNCTIONS_EDIT     39
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

/// \defgroup comstl_rot_functions Running Object Table (ROT) Functions
/// \ingroup COMSTL
/// \brief These functions create, manipulate and destroy instances of COM's Running Object Table (ROT)
/// @{

/* /////////////////////////////////////////////////////////////////////////////
 * Functions
 */

/// Registers an object in the Running Object Table
///
/// Registers an object and its identifying moniker in the Running Object Table (ROT).
///
/// \param grfFlags Registration options
/// \param punkObject Pointer to the object being registered
/// \param pmkObjectName Pointer to the moniker of the object being registered
/// \param pdwRegister Pointer to the value identifying the registration
/// \return An HRESULT indicating success or failure
inline HRESULT Rot_Register(DWORD       grfFlags,
                            LPUNKNOWN   punkObject,
                            LPMONIKER   pmkObjectName,
                            DWORD       *pdwRegister)
{
    LPRUNNINGOBJECTTABLE    prot;
    HRESULT                 hr = ::GetRunningObjectTable(0, &prot);

    if(SUCCEEDED(hr))
    {
        hr = prot->Register(grfFlags, punkObject, pmkObjectName, pdwRegister);

        prot->Release();
    }

    return hr;
}

/// Removes an object from the Running Object Table
///
/// Removes from the Running Object Table (ROT) an entry that was previously
/// registered by a call to Rot_Register().
///
/// \param dwRegister Value identifying registration to be revoked
/// \return An HRESULT indicating success or failure
inline HRESULT Rot_Revoke(DWORD dwRegister)
{
    LPRUNNINGOBJECTTABLE    prot;
    HRESULT                 hr = ::GetRunningObjectTable(0, &prot);

    if(SUCCEEDED(hr))
    {
        hr = prot->Revoke(dwRegister);

        prot->Release();
    }

    return hr;
}

/// Determines if object current in the Running Object Table
///
/// Determines whether the object identified by the specified moniker is
/// currently running. This method looks for the moniker in the Running Object
/// Table (ROT).
///
/// \param pmkObjectName Pointer to the moniker of the object whose status is desired
/// \return An HRESULT indicating success or failure
inline HRESULT Rot_IsRunning(LPMONIKER pmkObjectName)
{
    LPRUNNINGOBJECTTABLE    prot;
    HRESULT                 hr = ::GetRunningObjectTable(0, &prot);

    if(SUCCEEDED(hr))
    {
        hr = prot->IsRunning(pmkObjectName);

        prot->Release();
    }

    return hr;
}

/// Retrieves the object from the Running Object Table
///
/// Determines whether the object identified by the specified moniker is running,
/// and if it is, retrieves a pointer to that object. This method looks for the
/// moniker in the Running Object Table (ROT), and retrieves the pointer
/// registered there.
///
/// \param pmkObjectName Pointer to the moniker of the object
/// \param ppunkObject Address of output variable that receives the IUnknown interface pointer
/// \return An HRESULT indicating success or failure
inline HRESULT Rot_GetObject(   LPMONIKER   pmkObjectName,
                                LPUNKNOWN   *ppunkObject)
{
    LPRUNNINGOBJECTTABLE    prot;
    HRESULT                 hr = ::GetRunningObjectTable(0, &prot);

    if(SUCCEEDED(hr))
    {
        hr = prot->GetObject(pmkObjectName, ppunkObject);

        prot->Release();
    }

    return hr;
}

/// Retrieves the last modification time of a running object in the Running Object Table
///
/// Records the time that a running object was last modified. The object must
/// have previously been registered with the Running Object Table (ROT). This
/// method stores the time of last change in the ROT.
///
/// \param dwRegister Value identifying registration being updated
/// \param lpfiletime Pointer to structure containing object's last change time
/// \return An HRESULT indicating success or failure
inline HRESULT Rot_NoteChangeTime(  DWORD       dwRegister,
                                    FILETIME    *lpfiletime)
{
    LPRUNNINGOBJECTTABLE    prot;
    HRESULT                 hr = ::GetRunningObjectTable(0, &prot);

    if(SUCCEEDED(hr))
    {
        hr = prot->NoteChangeTime(dwRegister, lpfiletime);

        prot->Release();
    }

    return hr;
}

/// Retrieves the last modification time of an object in the Running Object Table
///
/// Returns the time that an object was last modified. The object must have
/// previously been registered with the Running Object Table (ROT). This method
/// looks for the last change time recorded in the ROT.
///
/// \param pmkObjectName Pointer to moniker on the object whose status is desired
/// \param lpfiletime Pointer to structure containing object's last change time
/// \return An HRESULT indicating success or failure
inline HRESULT Rot_GetTimeOfLastChange( LPMONIKER   pmkObjectName,
                                        FILETIME    *lpfiletime)
{
    LPRUNNINGOBJECTTABLE    prot;
    HRESULT                 hr = ::GetRunningObjectTable(0, &prot);

    if(SUCCEEDED(hr))
    {
        hr = prot->GetTimeOfLastChange(pmkObjectName, lpfiletime);

        prot->Release();
    }

    return hr;
}

/// Queries the current set of objects in the Running Object Table
///
/// Creates and returns a pointer to an enumerator that can list the monikers of
/// all the objects currently registered in the Running Object Table (ROT).
///
/// \param ppenumMoniker Address of output variable that receives the IEnumMoniker interface pointer
/// \return An HRESULT indicating success or failure
inline HRESULT Rot_EnumRunning(IEnumMoniker **ppenumMoniker)
{
    LPRUNNINGOBJECTTABLE    prot;
    HRESULT                 hr = ::GetRunningObjectTable(0, &prot);

    if(SUCCEEDED(hr))
    {
        hr = prot->EnumRunning(ppenumMoniker);

        prot->Release();
    }

    return hr;
}

/// @} // end of group comstl_rot_functions

////////////////////////////////////////////////////////////////////////////////
// Unit-testing

#ifdef STLSOFT_UNITTEST

namespace unittest
{
    ss_bool_t test_comstl_rot_functions(unittest_reporter *r)
    {
        using stlsoft::unittest::unittest_initialiser;

        ss_bool_t               bSuccess    =   true;

        unittest_initialiser    init(r, "COMSTL", "rot_functions", __FILE__);

#if 0
        if(<<TODO>>)
        {
            r->report("<<TODO>> failed ", __LINE__);
            bSuccess = false;
        }
#endif /* 0 */

        return bSuccess;
    }

    unittest_registrar    unittest_comstl_rot_functions(test_comstl_rot_functions);

} // namespace unittest

#endif /* STLSOFT_UNITTEST */

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

#endif /* !COMSTL_INCL_H_COMSTL_ROT_FUNCTIONS */

/* ////////////////////////////////////////////////////////////////////////// */
