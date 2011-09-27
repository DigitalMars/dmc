/* /////////////////////////////////////////////////////////////////////////////
 * File:        comstl_value_policies.h (originally MOEnSeq.h, ::SynesisCom)
 *
 * Purpose:     Value policies that work with STL sequence types for IEnumXXXX
 *              enumerator interfaces.
 *
 * Created:     17th September 1998
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


/// \file comstl_value_policies.h
///
/// Value policies that work with STL sequence types for IEnumXXXX enumerator interfaces.

#ifndef COMSTL_INCL_H_COMSTL_VALUE_POLICIES
#define COMSTL_INCL_H_COMSTL_VALUE_POLICIES

#ifndef __STLSOFT_DOCUMENTATION_SKIP_SECTION
# define COMSTL_VER_H_COMSTL_VALUE_POLICIES_MAJOR       3
# define COMSTL_VER_H_COMSTL_VALUE_POLICIES_MINOR       0
# define COMSTL_VER_H_COMSTL_VALUE_POLICIES_REVISION    1
# define COMSTL_VER_H_COMSTL_VALUE_POLICIES_EDIT        143
#endif /* !__STLSOFT_DOCUMENTATION_SKIP_SECTION */

/* ////////////////////////////////////////////////////////////////////////////
 * Includes
 */

#ifndef COMSTL_INCL_H_COMSTL
# include "comstl.h"                       // Include the COMSTL root header
#endif /* !COMSTL_INCL_H_COMSTL */
#ifndef COMSTL_INCL_H_COMSTL_REFCOUNT_FUNCTIONS
# include "comstl_refcount_functions.h"    // safe_release, etc.
#endif /* !COMSTL_INCL_H_COMSTL_REFCOUNT_FUNCTIONS */
#ifndef COMSTL_INCL_H_COMSTL_OLESTRING_FUNCTIONS
# include "comstl_olestring_functions.h"   // olestring_dup, etc.
#endif /* !COMSTL_INCL_H_COMSTL_OLESTRING_FUNCTIONS */
#ifndef STLSOFT_INCL_H_STLSOFT_SAP_CAST
# include "stlsoft_sap_cast.h"
#endif /* !STLSOFT_INCL_H_STLSOFT_SAP_CAST */

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

/// \defgroup comstl_value_policies Value Policies
/// \ingroup COMSTL
/// \brief These functions objects define the policies for manipulating various COM types
/// @{

/* /////////////////////////////////////////////////////////////////////////////
 * Classes
 */

/// Value policy for GUID
struct GUID_policy
{
public:
    typedef GUID        value_type;

public:
    /// Initialises an instance
    static void init(value_type *)
    {}
    /// Initialises an instance from another
    static void copy(value_type *dest, value_type const *src)
    {
        *dest = *src;
    }
    /// Releases an instance
    static void clear(value_type *)
    {}
};


/// Value policy for BSTR
struct BSTR_policy
{
public:
    typedef BSTR    value_type;

public:
    /// Initialises an instance
    static void init(value_type *p)
    {
        *p = NULL;
    }
    /// Initialises an instance from another
    static void copy(value_type *dest, value_type const *src)
    {
        *dest = ::SysAllocString(*src);
    }
    /// Releases an instance
    static void clear(value_type *p)
    {
        ::SysFreeString(*p);
    }
};


/// Value policy for LPOLESTR
struct LPOLESTR_policy
{
public:
    typedef LPOLESTR    value_type;

public:
    /// Initialises an instance
    static void init(value_type *p)
    {
        *p = NULL;
    }
    /// Initialises an instance from another
    static void copy(value_type *dest, value_type const *src)
    {
        *dest = olestring_dup(*src);
    }
    /// Releases an instance
    static void clear(value_type *p)
    {
        olestring_destroy(*p);
    }
};


/// Value policy for VARIANT
struct VARIANT_policy
{
public:
    typedef VARIANT value_type;

public:
    /// Initialises an instance
    static void init(value_type *p)
    {
        ::VariantInit(p);
    }
    /// Initialises an instance from another
    static void copy(value_type *dest, value_type const *src)
    {
        ::VariantCopy(dest, const_cast<VARIANT*>(src));
    }
    /// Releases an instance
    static void clear(value_type *p)
    {
        ::VariantClear(p);
    }
};


/// Value policy for interface
///
/// \param I The interface
template <ss_typename_param_k I>
struct interface_policy
{
public:
    typedef I               interface_type;
    typedef interface_type  *value_type;

public:
    /// Initialises an instance
    static void init(value_type *p)
    {
        *p = NULL;
    }
    /// Initialises an instance from another
    static void copy(value_type *dest, value_type const *src)
    {
        *dest = *src;
        safe_addref(*dest);
    }
    /// Releases an instance
    static void clear(value_type *p)
    {
        release_set_null(*p);
    }
};


/// Value policy for LPUNKNOWN
struct LPUNKNOWN_policy
{
public:
    typedef LPUNKNOWN   value_type;

public:
    /// Initialises an instance
    static void init(value_type *p)
    {
        *p = NULL;
    }
    /// Initialises an instance from another
    static void copy(value_type *dest, value_type const *src)
    {
        *dest = *src;
        safe_addref(*dest);
    }
    /// Releases an instance
    static void clear(value_type *p)
    {
        release_set_null(*p);
    }
};


/// Value policy for STATSTG
struct STATSTG_policy
{
public:
    typedef STATSTG value_type;

public:
    /// Initialises an instance
    static void init(value_type *p)
    {
        p->pwcsName = NULL;
    }
    /// Initialises an instance from another
    static void copy(value_type *dest, value_type const *src)
    {
        *dest = *src;
        if(src->pwcsName != NULL)
        {
            dest->pwcsName = olestring_dup(src->pwcsName);
        };
    }
    /// Releases an instance
    static void clear(value_type *p)
    {
        olestring_destroy(p->pwcsName);
    }
};


/// Value policy for FORMATETC
struct FORMATETC_policy
{
public:
    typedef FORMATETC   value_type;

public:
    /// Initialises an instance
    static void init(value_type *p)
    {
        p->ptd  =   NULL;
    }
    /// Initialises an instance from another
    static void copy(value_type *dest, value_type const *src)
    {
        *dest = *src;
        if(dest->ptd != NULL)
        {
            dest->ptd = static_cast<DVTARGETDEVICE*>(::CoTaskMemAlloc(src->ptd->tdSize));

            if(dest->ptd != NULL)
            {
                BYTE const  *src_begin  =   stlsoft_ns_qual(sap_cast)<BYTE const*>(&src->ptd);
                BYTE const  *src_end    =   src_begin + src->ptd->tdSize;
                BYTE        *dest_begin =   stlsoft_ns_qual(sap_cast)<BYTE*>(&dest->ptd);

                for(; src_begin != src_end; ++src_begin, ++dest_begin)
                {
                    *dest_begin = *src_begin;
                }
            }
        }
    }
    /// Releases an instance
    static void clear(value_type *p)
    {
        ::CoTaskMemFree(p->ptd);
    }
};

/* ////////////////////////////////////////////////////////////////////////// */

/// @} // end of group comstl_value_policies

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

#endif /* !COMSTL_INCL_H_COMSTL_VALUE_POLICIES */

/* ////////////////////////////////////////////////////////////////////////// */
