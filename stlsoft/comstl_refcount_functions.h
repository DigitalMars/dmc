/* ////////////////////////////////////////////////////////////////////////////
 * File:        comstl_refcount_functions.h
 *
 * Purpose:     Reference-counting helper functions.
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


/// \file comstl_refcount_functions.h
///
/// Reference-counting helper functions.

#ifndef COMSTL_INCL_H_COMSTL_REFCOUNT_FUNCTIONS
#define COMSTL_INCL_H_COMSTL_REFCOUNT_FUNCTIONS

#ifndef __STLSOFT_DOCUMENTATION_SKIP_SECTION
# define COMSTL_VER_H_COMSTL_REFCOUNT_FUNCTIONS_MAJOR       2
# define COMSTL_VER_H_COMSTL_REFCOUNT_FUNCTIONS_MINOR       0
# define COMSTL_VER_H_COMSTL_REFCOUNT_FUNCTIONS_REVISION    1
# define COMSTL_VER_H_COMSTL_REFCOUNT_FUNCTIONS_EDIT        30
#endif /* !__STLSOFT_DOCUMENTATION_SKIP_SECTION */

/* ////////////////////////////////////////////////////////////////////////////
 * Includes
 */

#ifndef COMSTL_INCL_H_COMSTL
# include "comstl.h"    // Include the COMSTL root header
#endif /* !COMSTL_INCL_H_COMSTL */

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

/// \defgroup comstl_refcount_functions OLE Reference Management Functions
/// \ingroup COMSTL
/// \brief These functions assist in the management of object reference counts
/// @{

/* ////////////////////////////////////////////////////////////////////////////
 * Functions
 */

/// Adds a reference on the interface pointer
///
/// \param punk The interface pointer on which to add the reference. Cannot be null
inline void addref(LPUNKNOWN punk)
{
    punk->AddRef();
}

/// Releases a reference on the interface pointer
///
/// \param punk The interface pointer on which to release the reference. Cannot be null
inline void release(LPUNKNOWN punk)
{
    punk->Release();
}

/// Adds a reference on the interface pointer
///
/// \param punk The interface pointer on which to add the reference. Can be null
inline void safe_addref(LPUNKNOWN punk)
{
    if(0 != punk)
    {
        addref(punk);
    }
}

/// Releases a reference on the interface pointer
///
/// \param punk The interface pointer on which to release the reference. Can be null
inline void safe_release(LPUNKNOWN punk)
{
    if(0 != punk)
    {
        release(punk);
    }
}

/// Releases a reference on the interface pointer, and resets the pointer
///
/// \param pt The interface pointer on which to release the reference. Can be null
template <ss_typename_param_k T>
inline void release_set_null(T *&pt)
{
    if(0 != pt)
    {
        release(pt);
        pt = 0;
    }
}

////////////////////////////////////////////////////////////////////////////////
// Unit-testing

#ifdef STLSOFT_UNITTEST

namespace unittest
{
    ss_bool_t test_comstl_refcount_functions(unittest_reporter *r)
    {
        using stlsoft::unittest::unittest_initialiser;

        ss_bool_t               bSuccess    =   true;

        unittest_initialiser    init(r, "COMSTL", "refcount_functions", __FILE__);

#if 0
        if(<<TODO>>)
        {
            r->report("<<TODO>> failed ", __LINE__);
            bSuccess = false;
        }
#endif /* 0 */

        return bSuccess;
    }

    unittest_registrar    unittest_comstl_refcount_functions(test_comstl_refcount_functions);

} // namespace unittest

#endif /* STLSOFT_UNITTEST */

/* ////////////////////////////////////////////////////////////////////////// */

/// @} // end of group comstl_refcount_functions

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

#endif /* !COMSTL_INCL_H_COMSTL_REFCOUNT_FUNCTIONS */

/* ////////////////////////////////////////////////////////////////////////// */
