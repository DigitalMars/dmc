/* ////////////////////////////////////////////////////////////////////////////
 * File:        comstl_interface_traits_std.h
 *
 * Purpose:     Standard interface traits.
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


/// \file comstl_interface_traits_std.h
///
/// Standard interface traits.

#ifndef COMSTL_INCL_H_COMSTL_INTERFACE_TRAITS_STD
#define COMSTL_INCL_H_COMSTL_INTERFACE_TRAITS_STD

#ifndef __STLSOFT_DOCUMENTATION_SKIP_SECTION
# define COMSTL_VER_H_COMSTL_INTERFACE_TRAITS_STD_MAJOR     2
# define COMSTL_VER_H_COMSTL_INTERFACE_TRAITS_STD_MINOR     0
# define COMSTL_VER_H_COMSTL_INTERFACE_TRAITS_STD_REVISION  1
# define COMSTL_VER_H_COMSTL_INTERFACE_TRAITS_STD_EDIT      25
#endif /* !__STLSOFT_DOCUMENTATION_SKIP_SECTION */

/* ////////////////////////////////////////////////////////////////////////////
 * Includes
 */

#ifndef COMSTL_INCL_H_COMSTL
# include "comstl.h"                    // Include the COMSTL root header
#endif /* !COMSTL_INCL_H_COMSTL */
#ifndef COMSTL_INCL_H_COMSTL_INTERFACE_TRAITS
# include "comstl_interface_traits.h"   // Include IID_traits
#endif /* !COMSTL_INCL_H_COMSTL_INTERFACE_TRAITS */

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

/// \weakgroup comstl_itf_library Interface Library
/// \ingroup COMSTL libraries
/// \brief This library provides facilities for manipulating COM interfaces
/// @{

/* ////////////////////////////////////////////////////////////////////////////
 * Classes
 */

/* unknwn.idl */

#if defined(__STLSOFT_COMPILER_IS_GCC) && \
    defined(_UNKNWN_H)
# define __IUnknown_FWD_DEFINED__
# define __IClassFactory_FWD_DEFINED__
#endif /* __STLSOFT_COMPILER_IS_GCC && _UNKNWN_H */

#ifdef __IUnknown_FWD_DEFINED__
COMSTL_IID_TRAITS_DEFINE(IUnknown)
#endif  /* __IUnknown_FWD_DEFINED__ */
#ifdef __IClassFactory_FWD_DEFINED__
COMSTL_IID_TRAITS_DEFINE(IClassFactory)
#endif  /* __IClassFactory_FWD_DEFINED__ */

/* objidl.idl */

#if defined(__STLSOFT_COMPILER_IS_GCC) && \
    defined(_UNKNWN_H)
# define __IMarshal_FWD_DEFINED__
# define __IMalloc_FWD_DEFINED__
# define __IMallocSpy_FWD_DEFINED__
# define __IStdMarshalInfo_FWD_DEFINED__
# define __IExternalConnection_FWD_DEFINED__
//# define __IMultiQI_FWD_DEFINED__
# define __IEnumUnknown_FWD_DEFINED__
# define __IBindCtx_FWD_DEFINED__
# define __IEnumMoniker_FWD_DEFINED__
# define __IRunnableObject_FWD_DEFINED__
# define __IRunningObjectTable_FWD_DEFINED__
# define __IPersist_FWD_DEFINED__
# define __IPersistStream_FWD_DEFINED__
# define __IMoniker_FWD_DEFINED__
# define __IROTData_FWD_DEFINED__
# define __IEnumString_FWD_DEFINED__
# define __ISequentialStream_FWD_DEFINED__
# define __IStream_FWD_DEFINED__
# define __IEnumSTATSTG_FWD_DEFINED__
# define __IStorage_FWD_DEFINED__
# define __IPersistFile_FWD_DEFINED__
# define __IPersistStorage_FWD_DEFINED__
# define __ILockBytes_FWD_DEFINED__
# define __IEnumFORMATETC_FWD_DEFINED__
# define __IEnumSTATDATA_FWD_DEFINED__
# define __IRootStorage_FWD_DEFINED__
# define __IAdviseSink_FWD_DEFINED__
# define __IAdviseSink2_FWD_DEFINED__
# define __IDataObject_FWD_DEFINED__
# define __IDataAdviseHolder_FWD_DEFINED__
# define __IMessageFilter_FWD_DEFINED__
# define __IRpcChannelBuffer_FWD_DEFINED__
# define __IRpcProxyBuffer_FWD_DEFINED__
# define __IRpcStubBuffer_FWD_DEFINED__
# define __IPSFactoryBuffer_FWD_DEFINED__
# define __IChannelHook_FWD_DEFINED__
# define __IPropertyStorage_FWD_DEFINED__
# define __IPropertySetStorage_FWD_DEFINED__
# define __IEnumSTATPROPSTG_FWD_DEFINED__
# define __IEnumSTATPROPSETSTG_FWD_DEFINED__
# define __IClientSecurity_FWD_DEFINED__
# define __IServerSecurity_FWD_DEFINED__
# define __IClassActivator_FWD_DEFINED__
# define __IFillLockBytes_FWD_DEFINED__
# define __IProgressNotify_FWD_DEFINED__
# define __ILayoutStorage_FWD_DEFINED__
#endif /* __STLSOFT_COMPILER_IS_GCC && _UNKNWN_H */

#ifdef __IMarshal_FWD_DEFINED__
COMSTL_IID_TRAITS_DEFINE(IMarshal)
#endif  /* __IMarshal_FWD_DEFINED__ */
#ifdef __IMalloc_FWD_DEFINED__
COMSTL_IID_TRAITS_DEFINE(IMalloc)
#endif  /* __IMalloc_FWD_DEFINED__ */
#ifdef __IMallocSpy_FWD_DEFINED__
COMSTL_IID_TRAITS_DEFINE(IMallocSpy)
#endif  /* __IMallocSpy_FWD_DEFINED__ */
#ifdef __IStdMarshalInfo_FWD_DEFINED__
COMSTL_IID_TRAITS_DEFINE(IStdMarshalInfo)
#endif  /* __IStdMarshalInfo_FWD_DEFINED__ */
#ifdef __IExternalConnection_FWD_DEFINED__
COMSTL_IID_TRAITS_DEFINE(IExternalConnection)
#endif  /* __IExternalConnection_FWD_DEFINED__ */
#ifdef __IMultiQI_FWD_DEFINED__
COMSTL_IID_TRAITS_DEFINE(IMultiQI)
#endif  /* __IMultiQI_FWD_DEFINED__ */
#ifdef __IEnumUnknown_FWD_DEFINED__
COMSTL_IID_TRAITS_DEFINE(IEnumUnknown)
#endif  /* __IEnumUnknown_FWD_DEFINED__ */
#ifdef __IBindCtx_FWD_DEFINED__
COMSTL_IID_TRAITS_DEFINE(IBindCtx)
#endif  /* __IBindCtx_FWD_DEFINED__ */
#ifdef __IEnumMoniker_FWD_DEFINED__
COMSTL_IID_TRAITS_DEFINE(IEnumMoniker)
#endif  /* __IEnumMoniker_FWD_DEFINED__ */
#ifdef __IRunnableObject_FWD_DEFINED__
COMSTL_IID_TRAITS_DEFINE(IRunnableObject)
#endif  /* __IRunnableObject_FWD_DEFINED__ */
#ifdef __IRunningObjectTable_FWD_DEFINED__
COMSTL_IID_TRAITS_DEFINE(IRunningObjectTable)
#endif  /* __IRunningObjectTable_FWD_DEFINED__ */
#ifdef __IPersist_FWD_DEFINED__
COMSTL_IID_TRAITS_DEFINE(IPersist)
#endif  /* __IPersist_FWD_DEFINED__ */
#ifdef __IPersistStream_FWD_DEFINED__
COMSTL_IID_TRAITS_DEFINE(IPersistStream)
#endif  /* __IPersistStream_FWD_DEFINED__ */
#ifdef __IMoniker_FWD_DEFINED__
COMSTL_IID_TRAITS_DEFINE(IMoniker)
#endif  /* __IMoniker_FWD_DEFINED__ */
#ifdef __IROTData_FWD_DEFINED__
COMSTL_IID_TRAITS_DEFINE(IROTData)
#endif  /* __IROTData_FWD_DEFINED__ */
#ifdef __IEnumString_FWD_DEFINED__
COMSTL_IID_TRAITS_DEFINE(IEnumString)
#endif  /* __IEnumString_FWD_DEFINED__ */
#ifdef __ISequentialStream_FWD_DEFINED__
COMSTL_IID_TRAITS_DEFINE(ISequentialStream)
#endif  /* __ISequentialStream_FWD_DEFINED__ */
#ifdef __IStream_FWD_DEFINED__
COMSTL_IID_TRAITS_DEFINE(IStream)
#endif  /* __IStream_FWD_DEFINED__ */
#ifdef __IEnumSTATSTG_FWD_DEFINED__
COMSTL_IID_TRAITS_DEFINE(IEnumSTATSTG)
#endif  /* __IEnumSTATSTG_FWD_DEFINED__ */
#ifdef __IStorage_FWD_DEFINED__
COMSTL_IID_TRAITS_DEFINE(IStorage)
#endif  /* __IStorage_FWD_DEFINED__ */
#ifdef __IPersistFile_FWD_DEFINED__
COMSTL_IID_TRAITS_DEFINE(IPersistFile)
#endif  /* __IPersistFile_FWD_DEFINED__ */
#ifdef __IPersistStorage_FWD_DEFINED__
COMSTL_IID_TRAITS_DEFINE(IPersistStorage)
#endif  /* __IPersistStorage_FWD_DEFINED__ */
#ifdef __ILockBytes_FWD_DEFINED__
COMSTL_IID_TRAITS_DEFINE(ILockBytes)
#endif  /* __ILockBytes_FWD_DEFINED__ */
#ifdef __IEnumFORMATETC_FWD_DEFINED__
COMSTL_IID_TRAITS_DEFINE(IEnumFORMATETC)
#endif  /* __IEnumFORMATETC_FWD_DEFINED__ */
#ifdef __IEnumSTATDATA_FWD_DEFINED__
COMSTL_IID_TRAITS_DEFINE(IEnumSTATDATA)
#endif  /* __IEnumSTATDATA_FWD_DEFINED__ */
#ifdef __IRootStorage_FWD_DEFINED__
COMSTL_IID_TRAITS_DEFINE(IRootStorage)
#endif  /* __IRootStorage_FWD_DEFINED__ */
#ifdef __IAdviseSink_FWD_DEFINED__
COMSTL_IID_TRAITS_DEFINE(IAdviseSink)
#endif  /* __IAdviseSink_FWD_DEFINED__ */
#ifdef __IAdviseSink2_FWD_DEFINED__
COMSTL_IID_TRAITS_DEFINE(IAdviseSink2)
#endif  /* __IAdviseSink2_FWD_DEFINED__ */
#ifdef __IDataObject_FWD_DEFINED__
COMSTL_IID_TRAITS_DEFINE(IDataObject)
#endif  /* __IDataObject_FWD_DEFINED__ */
#ifdef __IDataAdviseHolder_FWD_DEFINED__
COMSTL_IID_TRAITS_DEFINE(IDataAdviseHolder)
#endif  /* __IDataAdviseHolder_FWD_DEFINED__ */
#ifdef __IMessageFilter_FWD_DEFINED__
COMSTL_IID_TRAITS_DEFINE(IMessageFilter)
#endif  /* __IMessageFilter_FWD_DEFINED__ */
#ifdef __IRpcChannelBuffer_FWD_DEFINED__
COMSTL_IID_TRAITS_DEFINE(IRpcChannelBuffer)
#endif  /* __IRpcChannelBuffer_FWD_DEFINED__ */
#ifdef __IRpcProxyBuffer_FWD_DEFINED__
COMSTL_IID_TRAITS_DEFINE(IRpcProxyBuffer)
#endif  /* __IRpcProxyBuffer_FWD_DEFINED__ */
#ifdef __IRpcStubBuffer_FWD_DEFINED__
COMSTL_IID_TRAITS_DEFINE(IRpcStubBuffer)
#endif  /* __IRpcStubBuffer_FWD_DEFINED__ */
#ifdef __IPSFactoryBuffer_FWD_DEFINED__
COMSTL_IID_TRAITS_DEFINE(IPSFactoryBuffer)
#endif  /* __IPSFactoryBuffer_FWD_DEFINED__ */
#ifdef __IChannelHook_FWD_DEFINED__
COMSTL_IID_TRAITS_DEFINE(IChannelHook)
#endif  /* __IChannelHook_FWD_DEFINED__ */
#ifdef __IPropertyStorage_FWD_DEFINED__
COMSTL_IID_TRAITS_DEFINE(IPropertyStorage)
#endif  /* __IPropertyStorage_FWD_DEFINED__ */
#ifdef __IPropertySetStorage_FWD_DEFINED__
COMSTL_IID_TRAITS_DEFINE(IPropertySetStorage)
#endif  /* __IPropertySetStorage_FWD_DEFINED__ */
#ifdef __IEnumSTATPROPSTG_FWD_DEFINED__
COMSTL_IID_TRAITS_DEFINE(IEnumSTATPROPSTG)
#endif  /* __IEnumSTATPROPSTG_FWD_DEFINED__ */
#ifdef __IEnumSTATPROPSETSTG_FWD_DEFINED__
COMSTL_IID_TRAITS_DEFINE(IEnumSTATPROPSETSTG)
#endif  /* __IEnumSTATPROPSETSTG_FWD_DEFINED__ */
#ifdef __IClientSecurity_FWD_DEFINED__
COMSTL_IID_TRAITS_DEFINE(IClientSecurity)
#endif  /* __IClientSecurity_FWD_DEFINED__ */
#ifdef __IServerSecurity_FWD_DEFINED__
COMSTL_IID_TRAITS_DEFINE(IServerSecurity)
#endif  /* __IServerSecurity_FWD_DEFINED__ */
#ifdef __IClassActivator_FWD_DEFINED__
COMSTL_IID_TRAITS_DEFINE(IClassActivator)
#endif  /* __IClassActivator_FWD_DEFINED__ */
#ifdef __IFillLockBytes_FWD_DEFINED__
COMSTL_IID_TRAITS_DEFINE(IFillLockBytes)
#endif  /* __IFillLockBytes_FWD_DEFINED__ */
#ifdef __IProgressNotify_FWD_DEFINED__
COMSTL_IID_TRAITS_DEFINE(IProgressNotify)
#endif  /* __IProgressNotify_FWD_DEFINED__ */
#ifdef __ILayoutStorage_FWD_DEFINED__
COMSTL_IID_TRAITS_DEFINE(ILayoutStorage)
#endif  /* __ILayoutStorage_FWD_DEFINED__ */

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

#endif /* !COMSTL_INCL_H_COMSTL_INTERFACE_TRAITS_STD */

/* ////////////////////////////////////////////////////////////////////////// */
