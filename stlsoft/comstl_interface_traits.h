/* ////////////////////////////////////////////////////////////////////////////
 * File:        comstl_interface_traits.h
 *
 * Purpose:     Interface traits.
 *
 * Created:     25th May 2002
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


/// \file comstl_interface_traits.h
///
/// Interface traits.

#ifndef COMSTL_INCL_H_COMSTL_INTERFACE_TRAITS
#define COMSTL_INCL_H_COMSTL_INTERFACE_TRAITS

#ifndef __STLSOFT_DOCUMENTATION_SKIP_SECTION
# define COMSTL_VER_H_COMSTL_INTERFACE_TRAITS_MAJOR     2
# define COMSTL_VER_H_COMSTL_INTERFACE_TRAITS_MINOR     0
# define COMSTL_VER_H_COMSTL_INTERFACE_TRAITS_REVISION  1
# define COMSTL_VER_H_COMSTL_INTERFACE_TRAITS_EDIT      35
#endif /* !__STLSOFT_DOCUMENTATION_SKIP_SECTION */

/* ////////////////////////////////////////////////////////////////////////////
 * Includes
 */

#ifndef COMSTL_INCL_H_COMSTL
# include "comstl.h"    // Include the COMSTL root header
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

/// \defgroup comstl_itf_library Interface Library
/// \ingroup COMSTL libraries
/// \brief This library provides facilities for manipulating COM interfaces
/// @{

/* ////////////////////////////////////////////////////////////////////////////
 * Macros
 */

#ifndef __STLSOFT_DOCUMENTATION_SKIP_SECTION

#define COMSTL_IID_TRAITS_DEFINE__(I, T) \
  STLSOFT_TEMPLATE_SPECIALISATION \
struct IID_traits<T> \
{ \
public: \
    static REFIID   iid() { return IID_##I; } \
};

#define _COMSTL_IID_TRAITS_DEFINE(I)        COMSTL_IID_TRAITS_DEFINE__(I, I)

#endif /* !__STLSOFT_DOCUMENTATION_SKIP_SECTION */

/* ////////////////////////////////////////////////////////////////////////////
 * Classes
 */

#ifdef __STLSOFT_DOCUMENTATION_SKIP_SECTION

/// Interface identifier traits
///
/// This traits class takes an interface type and provides a specialisation that
/// has a statid iid() method which returns the interface identifier (IID) for
/// that type.
///
/// To use, simply have the expression IID_traits<I>::iid() where I is your
/// interface type.
///
/// On compilers that support the __uuidof pseudo-operator this is used,
/// otherwise you must specify specialisations - using the
/// COMSTL_IID_TRAITS_DEFINE macro - for your interfaces, as in
///
///   COMSTL_IID_TRAITS_DEFINE(IMyInterface)
///
/// All the interfaces currently defined in unknown.idl and objidl.idl are
/// so defined in the file comstl_interface_traits_std.h, which is included in
/// for non-__uuidof compilations.
///
/// \param I The interface
template <class I>
struct IID_traits
{
public:
    /// Returns a reference to the IID for the parameterising interface
    static REFIID   iid();
};
#else

#if !defined(_COMSTL_NO_UUIDOF) && \
    (   defined(__STLSOFT_COMPILER_IS_BORLAND) ||  \
        defined(__STLSOFT_COMPILER_IS_INTEL) ||  \
        defined(__STLSOFT_COMPILER_IS_MWERKS) ||  \
        (   defined(__STLSOFT_COMPILER_IS_MSVC) && \
            _MSC_VER >= 1200))
template <class I>
struct IID_traits
{
public:
    static REFIID   iid() { return __uuidof(I); }
};

#define COMSTL_IID_TRAITS_DEFINE(I)

#else
template <class I>
struct IID_traits;

#define COMSTL_IID_TRAITS_DEFINE(I)     EXTERN_C const IID IID_##I; \
                                        _COMSTL_IID_TRAITS_DEFINE(I) \
                                        COMSTL_IID_TRAITS_DEFINE__(I, I*)

/* For backwards compatibility */
#define comstl_IID_traits_define(I)     COMSTL_IID_TRAITS_DEFINE(I)

#endif /* __STLSOFT_DOCUMENTATION_SKIP_SECTION */

#ifndef _COMSTL_NO_NAMESPACE
# if defined(_STLSOFT_NO_NAMESPACE) || \
     defined(__STLSOFT_DOCUMENTATION_SKIP_SECTION)
} // namespace comstl
# else
} // namespace comstl_project
} // namespace stlsoft
# endif /* _STLSOFT_NO_NAMESPACE */
#endif /* !_COMSTL_NO_NAMESPACE */

#include "comstl_interface_traits_std.h"

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

#endif /* __SYNSOFT_DVS_COMPILER_VER_MSVC <
__SYNSOFT_GEN_COMPILER_VERSION(5, 0) */

/* ////////////////////////////////////////////////////////////////////////// */

/// @} // end of group comstl_itf_library

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

#endif /* !COMSTL_INCL_H_COMSTL_INTERFACE_TRAITS */

/* ////////////////////////////////////////////////////////////////////////// */
