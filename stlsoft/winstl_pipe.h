/* ////////////////////////////////////////////////////////////////////////////
 * File:        winstl_pipe.h
 *
 * Purpose:     pipe class, based on Windows anonymous pipe.
 *
 * Date:        19th June 2004
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


/// \file winstl_pipe.h
///
/// pipe class, based on Windows EVENT.

#ifndef WINSTL_INCL_H_WINSTL_PIPE
#define WINSTL_INCL_H_WINSTL_PIPE

#ifndef __STLSOFT_DOCUMENTATION_SKIP_SECTION
# define WINSTL_VER_H_WINSTL_PIPE_MAJOR     2
# define WINSTL_VER_H_WINSTL_PIPE_MINOR     0
# define WINSTL_VER_H_WINSTL_PIPE_REVISION  1
# define WINSTL_VER_H_WINSTL_PIPE_EDIT      9
#endif /* !__STLSOFT_DOCUMENTATION_SKIP_SECTION */

/* /////////////////////////////////////////////////////////////////////////////
 * Includes
 */

#ifndef WINSTL_INCL_H_WINSTL
# include "winstl.h"            // Include the WinSTL root header
#endif /* !WINSTL_INCL_H_WINSTL */
#ifndef WINSTL_INCL_H_WINSTL_EXCEPTIONS
# include "winstl_exceptions.h" // windows_exception
#endif /* !WINSTL_INCL_H_WINSTL_EXCEPTIONS */
#ifndef WINSTL_INCL_H_WINSTL_FUNCTIONS
# include "winstl_functions.h" // CloseHandleSetNull
#endif /* !WINSTL_INCL_H_WINSTL_FUNCTIONS */

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
 * Classes
 */

#ifdef __SYNSOFT_DBS_COMPILER_SUPPORTS_PRAGMA_MESSAGE
# pragma message(_sscomp_fileline_message("This needs to be parameterised with a winstl::system_resource_policy, which would control whether to throw if MX create fails"))
#endif /* __SYNSOFT_DBS_COMPILER_SUPPORTS_PRAGMA_MESSAGE */

/// Class which wraps the Win32 EVENT synchronisation object
class pipe
{
public:
    /// The class type
    typedef pipe                        class_type;
    /// The exception policy type
    typedef windows_exception_policy    exception_policy_type;

/// \name Construction
/// @{
public:
    ss_explicit_k pipe(ws_uint32_t size = 0, ws_bool_t bInheritHandles = true)
        : m_hReadHandle(NULL)
        , m_hWriteHandle(NULL)
    {
        SECURITY_ATTRIBUTES sa;

        sa.nLength              =   sizeof(sa);
        sa.lpSecurityDescriptor =   NULL;
        sa.bInheritHandle       =   bInheritHandles;

        if(!::CreatePipe(&m_hReadHandle, &m_hWriteHandle, &sa, size))
        {
            exception_policy_type()(::GetLastError());
        }
    }

    ~pipe()
    {
        if(NULL != m_hReadHandle)
        {
            ::CloseHandle(m_hReadHandle);
        }
        if(NULL != m_hWriteHandle)
        {
            ::CloseHandle(m_hWriteHandle);
        }
    }
/// @}

/// \name Accessors
/// @{
public:
    /// \brief Returns the read handle of the pipe
    HANDLE read_handle() const
    {
        return m_hReadHandle;
    }
    HANDLE write_handle() const
    {
        return m_hWriteHandle;
    }
/// @}

/// \name Operations
/// @{
public:
    /// \brief Closes the read handle, if not already closed
    void close_read()
    {
        if(NULL != m_hReadHandle)
        {
            CloseHandleSetNull(m_hReadHandle);
        }
    }
    /// \brief Closes the write handle, if not already closed
    void close_write()
    {
        if(NULL != m_hWriteHandle)
        {
            CloseHandleSetNull(m_hWriteHandle);
        }
    }
    /// \brief Closes the read and write handles, if not already closed
    void close()
    {
        close_read();
        close_write();
    }
/// @}

private:
    HANDLE  m_hReadHandle;
    HANDLE  m_hWriteHandle;

// Not to be implemented
private:
    pipe(class_type const &);
    class_type &operator =(class_type const &);
};

////////////////////////////////////////////////////////////////////////////////
// Unit-testing

#ifdef STLSOFT_UNITTEST

namespace unittest
{
    ss_bool_t test_winstl_pipe(unittest_reporter *r)
    {
        using stlsoft::unittest::unittest_initialiser;

        ss_bool_t               bSuccess    =   true;

        unittest_initialiser    init(r, "WinSTL", "pipe", __FILE__);

#if 0
        if(<<TODO>>)
        {
            r->report("<<TODO>> failed ", __LINE__);
            bSuccess = false;
        }
#endif /* 0 */

        return bSuccess;
    }

    unittest_registrar    unittest_winstl_pipe(test_winstl_pipe);

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

#endif /* !WINSTL_INCL_H_WINSTL_PIPE */

/* ////////////////////////////////////////////////////////////////////////// */
