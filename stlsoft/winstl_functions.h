/* /////////////////////////////////////////////////////////////////////////////
 * File:        winstl_functions.h (formerly MWBase.h, ::SynesisWin)
 *
 * Purpose:     A number of useful functions .
 *
 * Created:     7th May 2000
 * Updated:     11th September 2004
 *
 * Home:        http://stlsoft.org/
 *
 * Copyright (c) 2000-2004, Matthew Wilson and Synesis Software
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


/// \file winstl_functions.h
///
/// A number of useful functions .

#ifndef WINSTL_INCL_H_WINSTL_FUNCTIONS
#define WINSTL_INCL_H_WINSTL_FUNCTIONS

#ifndef __STLSOFT_DOCUMENTATION_SKIP_SECTION
# define WINSTL_VER_H_WINSTL_FUNCTIONS_MAJOR        2
# define WINSTL_VER_H_WINSTL_FUNCTIONS_MINOR        0
# define WINSTL_VER_H_WINSTL_FUNCTIONS_REVISION     1
# define WINSTL_VER_H_WINSTL_FUNCTIONS_EDIT         24
#endif /* !__STLSOFT_DOCUMENTATION_SKIP_SECTION */

/* /////////////////////////////////////////////////////////////////////////////
 * Includes
 */

#ifndef WINSTL_INCL_H_WINSTL
# include "winstl.h"                // Include the WinSTL root header
#endif /* !WINSTL_INCL_H_WINSTL */

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

/* /////////////////////////////////////////////////////////////////////////////
 * Functions
 */

#ifdef __cplusplus
inline bool BOOL2bool(BOOL b)
{
    return b != FALSE;
}
inline BOOL bool2BOOL(bool b)
{
    return b != false;
}
#else
# define BOOL2bool(b)               SyCastStatic(bool, ((b) != FALSE))
# define bool2BOOL(b)               SyCastStatic(BOOL, ((b) != false))
#endif /* __cplusplus */


/** CloseHandleSetNull
 *
 * Closes a handle, and sets its value to NULL
 */
inline ws_bool_t CloseHandleSetNull(HANDLE &h)
{
    return ::CloseHandle(h) ? (h = NULL, true) : false;
}

/** IsWaitObjectSignalled
 *
 * Indicates whether the given handle is signalled.
 *
 * \note There is no error return
 */
inline ws_bool_t IsWaitObjectSignalled(HANDLE h)
{
    return ::WaitForSingleObject(h,0) == WAIT_OBJECT_0;
}


/** HasOverlappedIoCompleted
 *
 * Indicates whether the I/O operation has completed
 *
 * \note There is no error return
 */
#undef HasOverlappedIoCompleted
inline BOOL HasOverlappedIoCompleted(LPOVERLAPPED lpOverlapped)
{
    return STATUS_PENDING != lpOverlapped->Internal;
}
#define HasOverlappedIoCompleted    HasOverlappedIoCompleted

/** DuplicateLocalHandle()
 *
 * Duplicates a handle. Both the handle and the duplicate are within the context
 * of the current process.
 */
inline ws_bool_t DuplicateLocalHandle(HANDLE h, HANDLE &hDup)
{
    return BOOL2bool(::DuplicateHandle(::GetCurrentProcess(), h, ::GetCurrentProcess(), &hDup, 0, false, DUPLICATE_SAME_ACCESS));
}

/** FormatMessage()
 *
 */

template <ss_typename_param_k C>
inline C *fmt_msg_find_last_good__(C *first, C* last)
{
    for(;first != last; )
    {
        switch(*(last - 1))
        {
            case    ' ':
            case    '\t':
            case    '\r':
            case    '\n':
            case    '.':
                *(last - 1) = '\0';
                --last;
                break;
            default:
                first = last;
                break;
        }
    }

    return last;
}

inline ws_dword_t FormatMessage(DWORD error, HINSTANCE hinst, ws_char_a_t *buffer, ws_uint_t cchBuffer)
{
    ws_dword_t  flags   =   FORMAT_MESSAGE_MAX_WIDTH_MASK | (NULL == hinst ? FORMAT_MESSAGE_FROM_SYSTEM : FORMAT_MESSAGE_FROM_HMODULE);
    ws_dword_t  res     = ::FormatMessageA(flags, static_cast<void*>(hinst), error, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), buffer, cchBuffer, NULL);

    if(res != 0)
    {
        // Now trim the trailing space
        ws_char_a_t *last_good = fmt_msg_find_last_good__(buffer, buffer + res);

        *last_good = 0;
        res = static_cast<ws_dword_t>(last_good - buffer);

        winstl_assert(res >= 0);
    }

    return res;
}

inline ws_dword_t FormatMessage(DWORD error, ws_char_a_t *buffer, ws_uint_t cchBuffer)
{
    return FormatMessage(error, NULL, buffer, cchBuffer);
}

inline ws_dword_t FormatMessage(DWORD error, ws_char_w_t *buffer, ws_uint_t cchBuffer)
{
    ws_dword_t res = ::FormatMessageW(  FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_MAX_WIDTH_MASK,
                                        NULL,
                                        error,
                                        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
                                        buffer,
                                        cchBuffer,
                                        NULL);

    if(res != 0)
    {
        // Now trim the trailing space
        ws_char_w_t *last_good = fmt_msg_find_last_good__(buffer, buffer + res);

        *last_good = 0;
        res = static_cast<ws_dword_t>(last_good - buffer);

        winstl_assert(res >= 0);
    }

    return res;
}

inline ws_dword_t FormatMessage(DWORD error, HINSTANCE hinstSource, ws_char_a_t **buffer)
{
    ws_dword_t  flags   =   FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_MAX_WIDTH_MASK | (NULL == hinstSource ? FORMAT_MESSAGE_FROM_SYSTEM : FORMAT_MESSAGE_FROM_HMODULE);
    ws_dword_t  res     =   ::FormatMessageA(flags, (void*)hinstSource, error, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPSTR)buffer, 0, NULL);

    if(res != 0)
    {
        // Now trim the trailing space
        ws_char_a_t *last_good = fmt_msg_find_last_good__(*buffer, *buffer + res);

        *last_good = 0;
        res = static_cast<ws_dword_t>(last_good - *buffer);

        winstl_assert(res >= 0);
    }

    return res;
}

inline ws_dword_t FormatMessage(DWORD error, HINSTANCE hinstSource, ws_char_w_t **buffer)
{
    ws_dword_t  flags   =   FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_MAX_WIDTH_MASK | (NULL == hinstSource ? FORMAT_MESSAGE_FROM_SYSTEM : FORMAT_MESSAGE_FROM_HMODULE);
    ws_dword_t  res     =   ::FormatMessageW(flags, (void*)hinstSource, error, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPWSTR)buffer, 0, NULL);

    if(res != 0)
    {
        // Now trim the trailing space
        ws_char_w_t *last_good = fmt_msg_find_last_good__(*buffer, *buffer + res);

        *last_good = 0;
        res = static_cast<ws_dword_t>(last_good - *buffer);

        winstl_assert(res >= 0);
    }

    return res;
}



/** Elicits asynchronous key state
 */
inline ws_bool_t IsKeyPressedAsync(int k)
{
    return (::GetAsyncKeyState(k) & 0x8000) != 0;
}

////////////////////////////////////////////////////////////////////////////////
// Unit-testing

#ifdef STLSOFT_UNITTEST

namespace unittest
{
    ss_bool_t test_winstl_functions(unittest_reporter *r)
    {
        using stlsoft::unittest::unittest_initialiser;

        ss_bool_t               bSuccess    =   true;

        unittest_initialiser    init(r, "WinSTL", "functions", __FILE__);

#if 0
        if(<<TODO>>)
        {
            r->report("<<TODO>> failed ", __LINE__);
            bSuccess = false;
        }
#endif /* 0 */

        return bSuccess;
    }

    unittest_registrar    unittest_winstl_functions(test_winstl_functions);

} // namespace unittest

#endif /* STLSOFT_UNITTEST */

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

#endif /* WINSTL_INCL_H_WINSTL_FUNCTIONS */

/* ////////////////////////////////////////////////////////////////////////// */
