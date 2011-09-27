/* /////////////////////////////////////////////////////////////////////////////
 * File:        comstl_memory_functions.h (originally MOMemory.h/.c, ::SynesisCom)
 *
 * Purpose:     COM memory functions.
 *
 * Created:     2nd March 1996
 * Updated:     11th September 2004
 *
 * Home:        http://stlsoft.org/
 *
 * Copyright (c) 1996-2004, Matthew Wilson and Synesis Software
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


/// \file comstl_memory_functions.h
///
/// COM memory functions.

#ifndef COMSTL_INCL_H_COMSTL_MEMORY_FUNCTIONS
#define COMSTL_INCL_H_COMSTL_MEMORY_FUNCTIONS

#ifndef __STLSOFT_DOCUMENTATION_SKIP_SECTION
# define COMSTL_VER_H_COMSTL_MEMORY_FUNCTIONS_MAJOR     2
# define COMSTL_VER_H_COMSTL_MEMORY_FUNCTIONS_MINOR     0
# define COMSTL_VER_H_COMSTL_MEMORY_FUNCTIONS_REVISION  1
# define COMSTL_VER_H_COMSTL_MEMORY_FUNCTIONS_EDIT      24
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

/// \defgroup comstl_memory_functions COM Memory Functions
/// \ingroup COMSTL
/// \brief These functions expand on the COM API's <code>CoTaskMem*()</code> suite of functions
/// @{

/* /////////////////////////////////////////////////////////////////////////////
 * Functions
 */

/* CoTaskMemGetSize, CoTaskMemDidAlloc & CoTaskMemHeapMinimise
 */

/// \brief Gives the size of a memory block
///
/// This function returns the size of a memory block relative to the COM task
/// alloctor, as per IMalloc::GetSize()
///
/// \param pv Pointer to the memory block
/// \return The size of the memory block (in bytes)
inline ULONG CoTaskMemGetSize(void *pv)
{
    LPMALLOC    lpmalloc;
    ULONG       ulRet;
    HRESULT     hr  =   ::CoGetMalloc(MEMCTX_TASK, &lpmalloc);

    if(SUCCEEDED(hr))
    {
        ulRet = lpmalloc->GetSize(pv);
        lpmalloc->Release();
    }
    else
    {
        ulRet = 0;
    }

    return ulRet;
}

/// \brief Determines allocation ownership of a memory block
///
/// This function returns a value indicating whether a memory block was allocated
/// by the COM task allocator, as per IMalloc::DidAlloc()
///
/// \param pv Pointer to the memory block
/// \return Result indicating ownership
/// \retval 1 The memory block was allocated by the task allocator
/// \retval 0 The memory block was <i>not</i> allocated by the task allocator
/// \retval -1 CoTaskMemDidAlloc() cannot determine whether the memory block was allocated by the task allocator
inline cs_sint_t CoTaskMemDidAlloc(void *pv)
{
    LPMALLOC    lpmalloc;
    cs_sint_t   iRet;
    HRESULT     hr  =   ::CoGetMalloc(MEMCTX_TASK, &lpmalloc);

    if(SUCCEEDED(hr))
    {
        iRet = lpmalloc->DidAlloc(pv);
        lpmalloc->Release();
    }
    else
    {
        iRet = -1;
    }

    return iRet;
}

/// \brief Minimises the heap
///
/// This function minimises the heap as much as possible by releasing unused
/// memory to the operating system, coalescing adjacent free blocks and
/// committing free pages, as as per IMalloc::HeapMinimize().
inline void CoTaskMemHeapMinimise()
{
    LPMALLOC    lpmalloc;

    if(SUCCEEDED(::CoGetMalloc(MEMCTX_TASK, &lpmalloc)))
    {
        lpmalloc->HeapMinimize();
        lpmalloc->Release();
    }
}

/// \brief Minimises the heap
///
/// This function minimises the heap as much as possible by releasing unused
/// memory to the operating system, coalescing adjacent free blocks and
/// committing free pages, as as per IMalloc::HeapMinimize().
inline void CoTaskMemHeapMinimize()
{
    CoTaskMemHeapMinimise();
}

////////////////////////////////////////////////////////////////////////////////
// Unit-testing

#ifdef STLSOFT_UNITTEST

namespace unittest
{
    ss_bool_t test_comstl_memory_functions(unittest_reporter *r)
    {
        using stlsoft::unittest::unittest_initialiser;

        ss_bool_t               bSuccess    =   true;

        unittest_initialiser    init(r, "COMSTL", "memory_functions", __FILE__);

        void    *pv =   ::CoTaskMemAlloc(100);

        if(CoTaskMemGetSize(pv) < 100)
        {
            r->report("Memory block has invalid size ", __LINE__);
            bSuccess = false;
        }

        if(0 == CoTaskMemDidAlloc(pv))
        {
            r->report("Memory block was not recognised by task allocator ", __LINE__);
            bSuccess = false;
        }

        return bSuccess;
    }

    unittest_registrar    unittest_comstl_memory_functions(test_comstl_memory_functions);

} // namespace unittest

#endif /* STLSOFT_UNITTEST */

/* ////////////////////////////////////////////////////////////////////////// */

/// @} // end of group comstl_memory_functions

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

#endif /* !COMSTL_INCL_H_COMSTL_MEMORY_FUNCTIONS */

/* ////////////////////////////////////////////////////////////////////////// */
