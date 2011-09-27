/* ////////////////////////////////////////////////////////////////////////////
 * File:        comstl_thread_marshal.h
 *
 * Purpose:     Thread marshalling functions.
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


/// \file comstl_thread_marshal.h
///
/// Thread marshalling functions.

#ifndef COMSTL_INCL_H_COMSTL_THREAD_MARSHAL
#define COMSTL_INCL_H_COMSTL_THREAD_MARSHAL

#ifndef __STLSOFT_DOCUMENTATION_SKIP_SECTION
# define COMSTL_VER_H_COMSTL_THREAD_MARSHAL_MAJOR       2
# define COMSTL_VER_H_COMSTL_THREAD_MARSHAL_MINOR       0
# define COMSTL_VER_H_COMSTL_THREAD_MARSHAL_REVISION    1
# define COMSTL_VER_H_COMSTL_THREAD_MARSHAL_EDIT        33
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

/// \defgroup thread_marshal_functions Thread Marshal Functions
/// \ingroup COMSTL
/// \brief These functions assist in the marshaling of components between threads and processes
/// @{

/* ////////////////////////////////////////////////////////////////////////////
 * Functions
 */

/// \brief Marshal a interface to a stream
///
/// This function marshals the given interface into a new stream instance, which
/// is returned to the caller. The stream may then be passed directly to another
/// thread in the process, from which GetInterfaceAndReleaseStream() may be
/// called.
///
/// \param pitf The interface pointer to marshal
/// \param ppstm A pointer to an IStream pointer to receive the stream
/// \return An HRESULT indicating success or failure
/// \retval E_OUTOFMEMORY Sufficient memory could not be acquired
/// \retval S_OK The operation completed successfully
template <ss_typename_param_k I>
inline HRESULT MarshalInterThreadInterfaceInStream(I *pitf, LPSTREAM *ppstm)
{
    return ::CoMarshalInterThreadInterfaceInStream(IID_traits<I>().iid(), pitf, ppstm);
}

/// \brief Retrieve a marshaled interface pointer from a stream
///
/// This function loads a serialised marshalled interface pointer from the given
/// stream, queries for the interface of the given pointer, and returns the
/// pointer if successful, and an error code if not. The stream is always
/// released, irrespective of the success status of the function as a whole.
///
/// \param pstm An IStream pointer from which object is to be unmarshaled
/// \param ppitf A pointer to the interface pointer to be unmarshaled
/// \return An HRESULT indicating success or failure
/// \retval E_INVALIDARG The argument was invalid
/// \retval S_OK The operation completed successfully

template <ss_typename_param_k I>
inline HRESULT GetInterfaceAndReleaseStream(LPSTREAM pstm, I **ppitf)
{
    return ::CoGetInterfaceAndReleaseStream(pstm, IID_traits<I>().iid(), reinterpret_cast<void**>(ppitf));
}

////////////////////////////////////////////////////////////////////////////////
// Unit-testing

#ifdef STLSOFT_UNITTEST

namespace unittest
{
    ss_bool_t test_comstl_thread_marshal(unittest_reporter *r)
    {
        using stlsoft::unittest::unittest_initialiser;

        ss_bool_t               bSuccess    =   true;

        unittest_initialiser    init(r, "COMSTL", "thread_marshal", __FILE__);

#if 0
        if(<<TODO>>)
        {
            r->report("<<TODO>> failed ", __LINE__);
            bSuccess = false;
        }
#endif /* 0 */

        return bSuccess;
    }

    unittest_registrar    unittest_comstl_thread_marshal(test_comstl_thread_marshal);

} // namespace unittest

#endif /* STLSOFT_UNITTEST */

/* ////////////////////////////////////////////////////////////////////////// */

/// @} // end of group thread_marshal_functions

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

#endif /* !COMSTL_INCL_H_COMSTL_THREAD_MARSHAL */

/* ////////////////////////////////////////////////////////////////////////// */
