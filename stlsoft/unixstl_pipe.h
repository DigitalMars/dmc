/* ////////////////////////////////////////////////////////////////////////////
 * File:        unixstl_pipe.h
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


/// \file unixstl_pipe.h
///
/// pipe class, based on Windows EVENT.

#ifndef UNIXSTL_INCL_H_UNIXSTL_PIPE
#define UNIXSTL_INCL_H_UNIXSTL_PIPE

#ifndef __STLSOFT_DOCUMENTATION_SKIP_SECTION
# define UNIXSTL_VER_H_UNIXSTL_PIPE_MAJOR     2
# define UNIXSTL_VER_H_UNIXSTL_PIPE_MINOR     0
# define UNIXSTL_VER_H_UNIXSTL_PIPE_REVISION  1
# define UNIXSTL_VER_H_UNIXSTL_PIPE_EDIT      7
#endif /* !__STLSOFT_DOCUMENTATION_SKIP_SECTION */

/* /////////////////////////////////////////////////////////////////////////////
 * Includes
 */

#ifndef UNIXSTL_INCL_H_UNIXSTL
# include "unixstl.h"               // Include the UNIXSTL root header
#endif /* !UNIXSTL_INCL_H_UNIXSTL */
#ifndef UNIXSTL_INCL_H_UNIXSTL_EXCEPTIONS
# include "unixstl_exceptions.h"    // unix_exception
#endif /* !UNIXSTL_INCL_H_UNIXSTL_EXCEPTIONS */

/* /////////////////////////////////////////////////////////////////////////////
 * Namespace
 */

#ifndef _UNIXSTL_NO_NAMESPACE
# if defined(_STLSOFT_NO_NAMESPACE) || \
     defined(__STLSOFT_DOCUMENTATION_SKIP_SECTION)
/* There is no stlsoft namespace, so must define ::unixstl */
namespace unixstl
{
# else
/* Define stlsoft::unixstl_project */

namespace stlsoft
{

namespace unixstl_project
{

# endif /* _STLSOFT_NO_NAMESPACE */
#endif /* !_UNIXSTL_NO_NAMESPACE */

/* /////////////////////////////////////////////////////////////////////////////
 * Classes
 */

#ifdef __SYNSOFT_DBS_COMPILER_SUPPORTS_PRAGMA_MESSAGE
# pragma message(_sscomp_fileline_message("This needs to be parameterised with a unixstl::system_resource_policy, which would control whether to throw if MX create fails"))
#endif /* __SYNSOFT_DBS_COMPILER_SUPPORTS_PRAGMA_MESSAGE */

/// Class which wraps the Win32 EVENT synchronisation object
class pipe
{
public:
    /// The class type
    typedef pipe                    class_type;
    /// The exception policy type
    typedef unix_exception_policy   exception_policy_type;

/// \name Construction
/// @{
public:
    pipe()
    {
        if(0 != ::pipe(&m_handles[0]))
        {
            exception_policy_type()(errno);

            m_handles[0] = -1;
            m_handles[1] = -1;
        }
    }

    ~pipe()
    {
        if(-1 != read_handle())
        {
            ::close(m_handles[0]);
        }
        if(NULL != write_handle())
        {
            ::close(m_handles[1]);
        }
    }
/// @}

/// \name Accessors
/// @{
public:
    /// \brief Returns the read handle of the pipe
    int read_handle() const
    {
        return m_handles[0];
    }
    int write_handle() const
    {
        return m_handles[1];
    }
/// @}

/// \name Operations
/// @{
public:
    /// \brief Closes the read handle, if not already closed
    void close_read()
    {
        if(-1 != read_handle())
        {
            close(m_handles[0]);
            m_handles[0] = -1;
        }
    }
    /// \brief Closes the write handle, if not already closed
    void close_write()
    {
        if(-1 != write_handle())
        {
            close(m_handles[1]);
            m_handles[1] = -1;
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
    int m_handles[2];

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
    ss_bool_t test_unixstl_pipe(unittest_reporter *r)
    {
        using stlsoft::unittest::unittest_initialiser;

        ss_bool_t               bSuccess    =   true;

        unittest_initialiser    init(r, "UNIXSTL", "pipe", __FILE__);

#if 0
        if(<<TODO>>)
        {
            r->report("<<TODO>> failed ", __LINE__);
            bSuccess = false;
        }
#endif /* 0 */

        return bSuccess;
    }

    unittest_registrar    unittest_unixstl_pipe(test_unixstl_pipe);

} // namespace unittest

#endif /* STLSOFT_UNITTEST */

/* ////////////////////////////////////////////////////////////////////////// */

#ifndef _UNIXSTL_NO_NAMESPACE
# if defined(_STLSOFT_NO_NAMESPACE) || \
     defined(__STLSOFT_DOCUMENTATION_SKIP_SECTION)
} // namespace unixstl
# else
} // namespace unixstl_project
} // namespace stlsoft
# endif /* _STLSOFT_NO_NAMESPACE */
#endif /* !_UNIXSTL_NO_NAMESPACE */

/* ////////////////////////////////////////////////////////////////////////// */

#endif /* !UNIXSTL_INCL_H_UNIXSTL_PIPE */

/* ////////////////////////////////////////////////////////////////////////// */
