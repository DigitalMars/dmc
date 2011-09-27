/* /////////////////////////////////////////////////////////////////////////////
 * File:        atlstl_property_method_helpers.h
 *
 * Purpose:     Contains functions for assisting in the implementation of
 *              property methods of ATL COM server classes.
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


/// \file atlstl_property_method_helpers.h
///
/// Contains functions for assisting in the implementation of property methods of ATL COM server classes.

#ifndef ATLSTL_INCL_H_ATLSTL_PROPERTY_METHOD_HELPERS
#define ATLSTL_INCL_H_ATLSTL_PROPERTY_METHOD_HELPERS

#ifndef __STLSOFT_DOCUMENTATION_SKIP_SECTION
# define ATLSTL_VER_H_ATLSTL_PROPERTY_METHOD_HELPERS_MAJOR      2
# define ATLSTL_VER_H_ATLSTL_PROPERTY_METHOD_HELPERS_MINOR      0
# define ATLSTL_VER_H_ATLSTL_PROPERTY_METHOD_HELPERS_REVISION   1
# define ATLSTL_VER_H_ATLSTL_PROPERTY_METHOD_HELPERS_EDIT       41
#endif /* !__STLSOFT_DOCUMENTATION_SKIP_SECTION */

/* /////////////////////////////////////////////////////////////////////////////
 * Compatibility
 */

/*
[Incompatibilies-start]
__STLSOFT_COMPILER_IS_BORLAND: __BORLANDC__=*
[Incompatibilies-end]
 */

/* /////////////////////////////////////////////////////////////////////////////
 * Includes
 */

#ifndef ATLSTL_INCL_H_ATLSTL
# include "atlstl.h"    // Include the ATLSTL root header
#endif /* !ATLSTL_INCL_H_ATLSTL */

#if defined(__STLSOFT_COMPILER_IS_BORLAND)
# error No recognised Borland compiler generates correct code when used with these functions
#endif /* __STLSOFT_COMPILER_IS_BORLAND */

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

/// \defgroup atlstl_property_helpers Property Helpers
/// \ingroup ATLSTL
/// \brief This set of <b>get_</b>/<b>put_</b> accessor template functions simplify the writing of COM object accessor methods
/// @{

/* /////////////////////////////////////////////////////////////////////////////
 * get_MemberValue
 *
 * This can be used to get a member variable value. The function has the
 * general usage:
 *
 *   HRESULT Class::get_Member(MemberType *ret)
 *   {
 *       return get_MemberValue(this, ret, &Class::m_memberVariable);
 *   }
 */

/// \brief Inline retrieval of member variable value
///
/// This function takes a pointer to a class, a pointer to a return value
/// and a pointer to member of the given class, and retrieves the value
/// of the member into the return value.
///
/// \note This generic version should only be used for built-in types, or those which have value semantics.
///
/// \param cls Pointer to the class instance
/// \param ret Pointer to the return value
/// \param mem Pointer to the member variable
/// \return An HRESULT code indicating whether the access succeeded or failed
/// \retval E_POINTER ret was a null pointer
/// \retval S_OK The value was retrieved successfully
template <ss_typename_param_k C, ss_typename_param_k T>
inline HRESULT get_MemberValue(C *const cls, T *ret, T C::*mem)
{
    return (ret == 0) ? E_POINTER : (*ret = cls->*mem, S_OK);
}

/// \brief Ghost overload to prevent use of get_MemberValue with pointer types
///
/// This version is overloaded to deal with pointer types, and is not
/// implemented so as to prevent the generic version being used with such types.
///
/// Although this is inconvenient, there is no other way to prevent the use of
/// free functions. In such circumstances, the shorthand of get_memberValue()
/// must be eschewed for full and congnisant implementation.
///
/// \note This is deemed worth the inconvenience since using the generic version would like lead to code that violated COM's memory rules
///
template <ss_typename_param_k C, class T>
inline HRESULT get_MemberValue(C *const cls, T **ret, T *C::*mem);

/// \brief Inline retrieval of a CComBSTR member variable value
///
/// This function takes a pointer to a class, a pointer to a return value
/// and a pointer to a CComBSTR member of the given class, and retrieves the
/// value of the member into the BSTR return value.
///
/// \param cls Pointer to the class instance
/// \param ret Pointer to the BSTR return value
/// \param mem Pointer to the CComBSTR member variable
/// \return An HRESULT code indicating whether the access succeeded or failed
/// \retval E_POINTER ret was a null pointer
/// \retval E_OUTOFMEMORY Not enough memory to create a copy for the returned value
/// \retval S_OK The value was retrieved successfully
template <ss_typename_param_k C>
inline HRESULT get_MemberValue(C *const cls, BSTR *ret, CComBSTR C::*mem)
{
    return (ret == 0) ? E_POINTER : (*ret = (cls->*mem).Copy(), (*ret != 0 ? S_OK :  E_OUTOFMEMORY));
}

/// \brief Inline retrieval of a VARIANT member variable value
///
/// This function takes a pointer to a class, a pointer to a return value
/// and a pointer to a VARIANT member of the given class, and retrieves the
/// value of the member into the VARIANT return value.
///
/// \param cls Pointer to the class instance
/// \param ret Pointer to the VARIANT return value
/// \param mem Pointer to the VARIANT member variable
/// \return An HRESULT code indicating whether the access succeeded or failed
/// \retval E_POINTER ret was a null pointer
/// \retval DISP_E_ARRAYISLOCKED The variant contains an array that is locked.
/// \retval DISP_E_BADVARTYPE The source and destination have an invalid variant type (usually uninitialized).
/// \retval E_OUTOFMEMORY Memory could not be allocated for the copy.
/// \retval E_INVALIDARG One of the arguments is invalid.
/// \retval S_OK The value was retrieved successfully
template <ss_typename_param_k C>
inline HRESULT get_MemberValue(C *const cls, VARIANT *ret, CComVariant C::*mem)
{
    return (ret == 0) ? E_POINTER : ::VariantCopy(ret, &(cls->*mem));
}

/* /////////////////////////////////////////////////////////////////////////////
 * put_MemberValue
 *
 * This can be used to put a member variable value. The function has the
 * general usage:
 *
 *   HRESULT Class::put_Member(MemberType newValue)
 *   {
 *       return put_MemberValue(this, newValue, &Class::m_memberVariable);
 *   }
 */

/// \brief Inline assignment of a member variable value
///
/// This function takes a pointer to a class, a new value and a pointer to
/// member of the given class, and assigns the new value to the member.
///
/// \note This generic version should only be used for built-in types, or those which have value semantics.
///
/// \param cls Pointer to the class instance
/// \param newVal The new value
/// \param mem Pointer to the member variable
/// \retval S_OK The value was assigned successfully
template <ss_typename_param_k C, ss_typename_param_k T>
inline HRESULT put_MemberValue(C *const cls, T const &newVal, T C::*mem)
{
    return (cls->*mem = newVal, S_OK);
}

/// \brief Inline assignment of a CComBSTR member variable value
///
/// This function takes a pointer to a class, a new BSTR value and a pointer to
/// CComBSTR  member of the given class, and assigns the new value to the
/// member.
///
/// \param cls Pointer to the class instance
/// \param newVal The new BSTR value
/// \param mem Pointer to the CComBSTR member variable
/// \retval S_OK The value was assigned successfully
template <ss_typename_param_k C>
inline HRESULT put_MemberValue(C *const cls, BSTR newVal, CComBSTR C::*mem)
{
    return (cls->*mem = newVal, S_OK);
}

/// \brief Inline assignment of a CComBSTR member variable value
///
/// This function takes a pointer to a class, a new CComBSTR value and a pointer
/// to CComBSTR  member of the given class, and assigns the new value to the
/// member.
///
/// \param cls Pointer to the class instance
/// \param newVal The new CComBSTR  value
/// \param mem Pointer to the CComBSTR member variable
/// \retval S_OK The value was assigned successfully
template <ss_typename_param_k C>
inline HRESULT put_MemberValue(C *const cls, CComBSTR const &newVal, CComBSTR C::*mem)
{
    return put_MemberValue(cls, (BSTR)newVal, mem);
}

/// \brief Inline assignment of a CComVariant member variable value
///
/// This function takes a pointer to a class, a new CComVariant value and a
/// pointer to CComVariant member of the given class, and assigns the new value
/// to the member.
///
/// \param cls Pointer to the class instance
/// \param newVal The new CComVariant value
/// \param mem Pointer to the CComVariant member variable
/// \retval S_OK The value was assigned successfully
template <ss_typename_param_k C>
inline HRESULT put_MemberValue(C *const cls, CComVariant const &newVal, CComVariant C::*mem)
{
    return put_MemberValue(cls, (VARIANT const &)newVal, mem);
}

////////////////////////////////////////////////////////////////////////////////
// Unit-testing

#ifdef STLSOFT_UNITTEST

namespace unittest
{
    class SimpleServer
    {
    public:
        SimpleServer()
            : m_bstrUserName("[No username]")
            , m_logonKey(-1)
        {}

    // Property accessors
    public:
        STDMETHODIMP SimpleServer::get_UserName(/* [retval][out] */ BSTR *pVal)
        {
            return get_MemberValue(this, pVal, &SimpleServer::m_bstrUserName);
        }

        STDMETHODIMP SimpleServer::put_UserName(/* [in] */ BSTR newVal)
        {
            return put_MemberValue(this, newVal, &SimpleServer::m_bstrUserName);
        }

        STDMETHODIMP SimpleServer::get_LogonKey(/* [retval][out] */ short *pVal)
        {
            return get_MemberValue(this, pVal, &SimpleServer::m_logonKey);
        }

        STDMETHODIMP SimpleServer::put_LogonKey(/* [in] */ short newVal)
        {
            return put_MemberValue(this, newVal, &SimpleServer::m_logonKey);
        }

    // Members
    protected:
        friend ss_bool_t test_atlstl_property_method_helpers(unittest_reporter *r);

        CComBSTR    m_bstrUserName;
        short       m_logonKey;
    };

    ss_bool_t test_atlstl_property_method_helpers(unittest_reporter *r)
    {
        using stlsoft::unittest::unittest_initialiser;

        ss_bool_t               bSuccess    =   true;
        HRESULT                 hr;

        unittest_initialiser    init(r, "ATLSTL", "property_method_helpers", __FILE__);

        SimpleServer    simpleserver;
        short           logonKey(-1);
        CComBSTR        bstrUserName;

        hr = simpleserver.put_LogonKey(12);
        if(FAILED(hr))
        {
            r->report("call to put_LogonKey() failed ", __LINE__);
            bSuccess = false;
        }

        hr = simpleserver.get_LogonKey(&logonKey);
        if(FAILED(hr))
        {
            r->report("call to get_LogonKey() failed ", __LINE__);
            bSuccess = false;
        }
        else
        {
            if(12 != logonKey)
            {
                r->report("put/get_MemberValue failed ", __LINE__);
                bSuccess = false;
            }
        }

        simpleserver.put_UserName(L"Matty");
        hr = simpleserver.get_UserName(&bstrUserName);
        if(SUCCEEDED(hr))
        {
            if(!(bstrUserName == L"Matty"))
            {
                r->report("put/get_MemberValue failed ", __LINE__);
                bSuccess = false;
            }
        }

        return bSuccess;
    }

    unittest_registrar    unittest_atlstl_property_method_helpers(test_atlstl_property_method_helpers);

} // namespace unittest

#endif /* STLSOFT_UNITTEST */

/* ////////////////////////////////////////////////////////////////////////// */

/// @} // end of group atlstl_property_helpers

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

#endif /* !ATLSTL_INCL_H_ATLSTL_PROPERTY_METHOD_HELPERS */

/* ////////////////////////////////////////////////////////////////////////// */
