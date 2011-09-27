/* /////////////////////////////////////////////////////////////////////////////
 * File:        winstl_token_information.h
 *
 * Purpose:     Helper for accessing token information.
 *
 * Created:     20th June 2003
 * Updated:     11th September 2004
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


/// \file winstl_token_information.h
///
/// Helper for accessing token information.

#ifndef WINSTL_INCL_H_WINSTL_TOKEN_INFORMATION
#define WINSTL_INCL_H_WINSTL_TOKEN_INFORMATION

#ifndef __STLSOFT_DOCUMENTATION_SKIP_SECTION
# define WINSTL_VER_H_WINSTL_TOKEN_INFORMATION_MAJOR        2
# define WINSTL_VER_H_WINSTL_TOKEN_INFORMATION_MINOR        0
# define WINSTL_VER_H_WINSTL_TOKEN_INFORMATION_REVISION     1
# define WINSTL_VER_H_WINSTL_TOKEN_INFORMATION_EDIT         27
#endif /* !__STLSOFT_DOCUMENTATION_SKIP_SECTION */

/* /////////////////////////////////////////////////////////////////////////////
 * Includes
 */

#ifndef WINSTL_INCL_H_WINSTL
# include "winstl.h"                        // Include the WinSTL root header
#endif /* WINSTL_INCL_H_WINSTL */
#ifndef WINSTL_INCL_H_WINSTL_PROCESSHEAP_ALLOCATOR
# include "winstl_processheap_allocator.h"  // winstl::processheap_allocator
#endif /* !WINSTL_INCL_H_WINSTL_PROCESSHEAP_ALLOCATOR */
#ifndef STLSOFT_INCL_H_STLSOFT_EXCEPTIONS
# include "stlsoft_exceptions.h"            // stlsoft::null_exception_policy
#endif /* !STLSOFT_INCL_H_STLSOFT_EXCEPTIONS */
#ifndef WINSTL_INCL_H_LAST_ERROR_SCOPE
# include "winstl_last_error_scope.h"       // winstl::last_error_scope
#endif /* !WINSTL_INCL_H_LAST_ERROR_SCOPE */

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

/* ////////////////////////////////////////////////////////////////////////// */

/// \weakgroup libraries STLSoft Libraries
/// \brief The individual libraries

/// \weakgroup libraries_security Security Library
/// \ingroup libraries
/// \brief This library provides facilities for manipulating security APIs

/// \defgroup winstl_security_library Security Library (WinSTL)
/// \ingroup WinSTL libraries_security
/// \brief This library provides facilities for manipulating the Win32 security API
/// @{

#if defined(__STLSOFT_COMPILER_IS_BORLAND) || \
    defined(__STLSOFT_COMPILER_IS_COMO) || \
    defined(__STLSOFT_COMPILER_IS_DMC) || \
    defined(__STLSOFT_COMPILER_IS_GCC) || \
    defined(__STLSOFT_COMPILER_IS_MWERKS)
enum
{
        TokenRestrictedSids         =   1 + TokenStatistics
    ,   TokenSessionId
    ,   TokenGroupsAndPrivileges
    ,   TokenSessionReference
    ,   TokenSandBoxInert
    ,   TokenAuditPolicy
};
#endif /* compiler */

/* /////////////////////////////////////////////////////////////////////////////
 * Classes
 */

/// traits type used to determine the data type for a given \c TOKEN_INFORMATION_CLASS
template <TOKEN_INFORMATION_CLASS C>
struct token_information_traits;


#ifndef __STLSOFT_DOCUMENTATION_SKIP_SECTION
template <>
struct token_information_traits<TokenUser>
{
    typedef TOKEN_USER                      data_type;
};

template <>
struct token_information_traits<TokenGroups>
{
    typedef TOKEN_GROUPS                    data_type;
};

template <>
struct token_information_traits<TokenPrivileges>
{
    typedef TOKEN_PRIVILEGES                data_type;
};

template <>
struct token_information_traits<TokenOwner>
{
    typedef TOKEN_OWNER                     data_type;
};

template <>
struct token_information_traits<TokenPrimaryGroup>
{
    typedef TOKEN_PRIMARY_GROUP             data_type;
};

template <>
struct token_information_traits<TokenDefaultDacl>
{
    typedef TOKEN_DEFAULT_DACL              data_type;
};

template <>
struct token_information_traits<TokenSource>
{
    typedef TOKEN_SOURCE                    data_type;
};

template <>
struct token_information_traits<TokenType>
{
    typedef TOKEN_TYPE                      data_type;
};

template <>
struct token_information_traits<TokenImpersonationLevel>
{
    typedef SECURITY_IMPERSONATION_LEVEL    data_type;
};

template <>
struct token_information_traits<TokenStatistics>
{
    typedef TOKEN_STATISTICS                data_type;
};

template <>
struct token_information_traits<static_cast<TOKEN_INFORMATION_CLASS>(TokenRestrictedSids)>
{
    typedef TOKEN_GROUPS                    data_type;
};

template <>
struct token_information_traits<static_cast<TOKEN_INFORMATION_CLASS>(TokenSessionId)>
{
    typedef DWORD                           data_type;
};

# if !defined(__STLSOFT_COMPILER_IS_BORLAND) && \
     !defined(__STLSOFT_COMPILER_IS_COMO) && \
     !defined(__STLSOFT_COMPILER_IS_DMC) && \
     !defined(__STLSOFT_COMPILER_IS_GCC) && \
     !defined(__STLSOFT_COMPILER_IS_MWERKS)
template <>
struct token_information_traits<static_cast<TOKEN_INFORMATION_CLASS>(TokenGroupsAndPrivileges)>
{
    typedef TOKEN_GROUPS_AND_PRIVILEGES     data_type;
};
# endif /* compiler */

template <>
struct token_information_traits<static_cast<TOKEN_INFORMATION_CLASS>(TokenSandBoxInert)>
{
    typedef DWORD                           data_type;
};

#endif /* !__STLSOFT_DOCUMENTATION_SKIP_SECTION */

// token_information
/// Provides typed access to token information.
///
template<   TOKEN_INFORMATION_CLASS C
#ifdef __STLSOFT_CF_TEMPLATE_CLASS_DEFAULT_CLASS_ARGUMENT_SUPPORT
# ifdef __SYNSOFT_DBS_COMPILER_SUPPORTS_PRAGMA_MESSAGE
#  pragma message(_sscomp_fileline_message("Note that we have to have data_type as a parameter, otherwise VC5&6 have a cow"))
# endif /* __SYNSOFT_DBS_COMPILER_SUPPORTS_PRAGMA_MESSAGE */
        ,   ss_typename_param_k     X = stlsoft_ns_qual(null_exception_policy)
        ,   ss_typename_param_k     D = ss_typename_type_def_k token_information_traits<C>::data_type
        ,   ss_typename_param_k     A = processheap_allocator<ss_byte_t>
#else
        ,   ss_typename_param_k     X /* = stlsoft_ns_qual(null_exception_policy) */
        ,   ss_typename_param_k     D /* = token_information_traits<C>::data_type */
        ,   ss_typename_param_k     A /* = processheap_allocator<ss_byte_t> */
#endif /* __STLSOFT_CF_TEMPLATE_CLASS_DEFAULT_CLASS_ARGUMENT_SUPPORT */
        >
class token_information
{
public:
    typedef token_information<C, X, D, A>               class_type;
    typedef token_information_traits<C>                 traits_type;
    typedef X                                           exception_thrower_type;
    typedef D                                           data_type;
    typedef A                                           allocator_type;
//  typedef ss_typename_type_k traits_type::data_type   data_type;

// Construction
public:
    /// \brief Constructs an instance from the given access token
    ///
    ss_explicit_k token_information(HANDLE hToken)
        : m_data(0)
    {
        DWORD   cbRequired;
        DWORD   dwError;

        ::GetTokenInformation(hToken, C, NULL, 0, &cbRequired);
        dwError = ::GetLastError();
        if(ERROR_INSUFFICIENT_BUFFER != dwError)
        {
            // Report error
            exception_thrower_type()(dwError);
        }
        else
        {
            data_type   *data = reinterpret_cast<data_type*>(allocator_type().allocate(cbRequired));

            if(NULL == data)
            {
                // Report error
                exception_thrower_type()(ERROR_NOT_ENOUGH_MEMORY);

                // Set the last error, in case the client code is not using exception reporting
                ::SetLastError(ERROR_NOT_ENOUGH_MEMORY);
            }
            else
            {
                if(!::GetTokenInformation(hToken, C, data, cbRequired, &cbRequired))
                {
                    // Scope the last error, in case the client code is not using exception reporting
                    last_error_scope    scope;

                    allocator_type().deallocate(reinterpret_cast<ss_byte_t*>(data));

                    // Report error
                    exception_thrower_type()(DWORD((scope)));
                }
                else
                {
                    // Success
                    m_data = data;

                    ::SetLastError(ERROR_SUCCESS);
                }
            }
        }
    }
    ~token_information()
    {
        allocator_type().deallocate(reinterpret_cast<ss_byte_t*>(m_data));
    }

// Conversion
public:
    operator data_type *()
    {
        return m_data;
    }
    operator data_type const *() const
    {
        return m_data;
    }

    data_type *operator ->()
    {
        return m_data;
    }
    data_type const *operator ->() const
    {
        return m_data;
    }
/*
    operator ws_bool_t () const
    {
        return 0 != m_data;
    }
*/
    ws_bool_t operator !() const
    {
        return 0 == m_data;
    }

/// Implementation
private:

/// Members
private:
    data_type   *m_data;

/// Not to be implemented
private:
    token_information(token_information const &);
    token_information &operator =(token_information const &);
};

/* ////////////////////////////////////////////////////////////////////////// */

/// @} // end of group winstl_security_library

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

#endif /* WINSTL_INCL_H_WINSTL_TOKEN_INFORMATION */

/* ////////////////////////////////////////////////////////////////////////// */
