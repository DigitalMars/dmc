/* /////////////////////////////////////////////////////////////////////////////
 * File:        comstl_initialisers.h (originally MOInit.h, ::SynesisCom)
 *
 * Purpose:     Contains classes for initialising COM/OLE.
 *
 * Created:     8th February 1999
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


/// \file comstl_initialisers.h
///
/// Contains classes for initialising COM/OLE.

#ifndef COMSTL_INCL_H_COMSTL_INITIALISERS
#define COMSTL_INCL_H_COMSTL_INITIALISERS

#ifndef __STLSOFT_DOCUMENTATION_SKIP_SECTION
# define COMSTL_VER_H_COMSTL_INITIALISERS_MAJOR     2
# define COMSTL_VER_H_COMSTL_INITIALISERS_MINOR     0
# define COMSTL_VER_H_COMSTL_INITIALISERS_REVISION  1
# define COMSTL_VER_H_COMSTL_INITIALISERS_EDIT      44
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

/* /////////////////////////////////////////////////////////////////////////////
 * Classes
 */

/// Initialises the COM libraries
///
/// This class is used to initialise the COM libraries. It can respond to
/// CoInitializeEx argument flags when translated in a DCOM build.
template<   ss_typename_param_k IP  /* Initialisation policy type */
        ,   ss_typename_param_k XP  /* Exception policy type */
        >
class initialiser
{
private:
    typedef initialiser   class_type;
public:
    /// The initialiation policy type
    typedef IP                                                      initialisation_policy_type;
    /// The exception type
    typedef XP                                                      exception_policy_type;
    /// The thrown type
    typedef ss_typename_type_k exception_policy_type::thrown_type   thrown_type;

// Construction
public:
    /// Initialises via CoInitialize()
    initialiser();
#ifdef __COMSTL_CF_DCOM_SUPPORT
    /// Initialises via CoInitializeEx() taking b>COINIT_*</b> flags
    ss_explicit_k initialiser(cs_dword_t dwInit /* = COINIT_APARTMENTTHREADED */);
#endif /* __COMSTL_CF_DCOM_SUPPORT */
    /// Uninitialises via CoUninitialize()
    ~initialiser() comstl_throw_0();

// Attributes
public:
    /// Reflects whether the COM libraries were initialised
    cs_bool_t is_initialised() const;
    /// Reflects whether the COM libraries were not initialised
    cs_bool_t operator !() const;
    /// The result of the call to CoInitialize()/CoInitializeEx()
    HRESULT get_HRESULT() const;

// Members
private:
    HRESULT const   m_hr;

// Not to be implemented
private:
    initialiser(class_type const &rhs);
    class_type const &operator =(class_type const &rhs);
};

////////////////////////////////////////////////////////////////////////////////
// Value policies

#ifndef __STLSOFT_DOCUMENTATION_SKIP_SECTION

struct CoInitialize_policy
{
public:
    static HRESULT init()
    {
        return ::CoInitialize(NULL);
    }
#ifdef __COMSTL_CF_DCOM_SUPPORT
    static HRESULT init(cs_dword_t coInit)
    {
        return ::CoInitializeEx(NULL, coInit);
    }
#endif /* __COMSTL_CF_DCOM_SUPPORT */
    static void uninit()
    {
        ::CoUninitialize();
    }
};

struct OleInitialize_policy
{
public:
    static HRESULT init()
    {
        return ::OleInitialize(NULL);
    }
    static void uninit()
    {
        ::OleUninitialize();
    }
};

#endif /* !__STLSOFT_DOCUMENTATION_SKIP_SECTION */

////////////////////////////////////////////////////////////////////////////////
// Exception policies

/// Exception policy whose action is to do nothing
struct ignore_initialisation_exception
{
public:
    /// The exception type
    struct thrown_type
    {
    };

public:
    /// The function call operator, which does not throw an exception
    ///
    /// \param hr The HRESULT that caused the error
    /// \param riid The REFIID that could not be acquired
    void operator ()(HRESULT hr, REFIID riid) comstl_throw_0()
    {
        STLSOFT_SUPPRESS_UNUSED(hr);
        STLSOFT_SUPPRESS_UNUSED(riid);

        // Do nothing
    }
};


////////////////////////////////////////////////////////////////////////////////
// Typedefs for US-English spellers

typedef initialiser<CoInitialize_policy, ignore_initialisation_exception>   com_initialiser;
typedef initialiser<OleInitialize_policy, ignore_initialisation_exception>  ole_initialiser;

typedef com_initialiser com_initializer;
typedef ole_initialiser ole_initializer;

////////////////////////////////////////////////////////////////////////////////
// Unit-testing

#ifdef STLSOFT_UNITTEST

namespace unittest
{
    ss_bool_t test_comstl_initialisers(unittest_reporter *r)
    {
        using stlsoft::unittest::unittest_initialiser;

        ss_bool_t               bSuccess    =   true;

        unittest_initialiser    init(r, "COMSTL", "initialisers", __FILE__);

        com_initializer coinit;

#if 0
        if(<<TODO>>)
        {
            r->report("<<TODO>> failed ", __LINE__);
            bSuccess = false;
        }
#endif /* 0 */

        return bSuccess;
    }

    unittest_registrar    unittest_comstl_initialisers(test_comstl_initialisers);

} // namespace unittest

#endif /* STLSOFT_UNITTEST */

////////////////////////////////////////////////////////////////////////////////
// Implementation

// initialiser

template<   ss_typename_param_k IP
        ,   ss_typename_param_k XP
        >
inline initialiser<IP, XP>::initialiser()
    : m_hr(initialisation_policy_type::init())
{}

#ifdef __COMSTL_CF_DCOM_SUPPORT
template<   ss_typename_param_k IP
        ,   ss_typename_param_k XP
        >
inline initialiser<IP, XP>::initialiser(cs_dword_t coInit)
    : m_hr(initialisation_policy_type::init(coInit))
{}
#endif // __COMSTL_CF_DCOM_SUPPORT

template<   ss_typename_param_k IP
        ,   ss_typename_param_k XP
        >
inline initialiser<IP, XP>::~initialiser() comstl_throw_0()
{
    if(is_initialised())
    {
        initialisation_policy_type::uninit();
    }
}

template<   ss_typename_param_k IP
        ,   ss_typename_param_k XP
        >
inline cs_bool_t initialiser<IP, XP>::is_initialised() const
{
    return SUCCEEDED(m_hr);
}

template<   ss_typename_param_k IP
        ,   ss_typename_param_k XP
        >
inline cs_bool_t initialiser<IP, XP>::operator !() const
{
    return !is_initialised();
}

template<   ss_typename_param_k IP
        ,   ss_typename_param_k XP
        >
inline HRESULT initialiser<IP, XP>::get_HRESULT() const
{
    return m_hr;
}

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

#endif /* !COMSTL_INCL_H_COMSTL_INITIALISERS */

/* ////////////////////////////////////////////////////////////////////////// */
