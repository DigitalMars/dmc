/* /////////////////////////////////////////////////////////////////////////////
 * File:        winstl_filesystem_functions.h (Formerly MLFlMan.h, ::SynesisStd)
 *
 * Purpose:     Contains some Win32 file-system functions.
 *
 * Created:     7th February 2002
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


/// \file winstl_filesystem_functions.h
///
/// Contains the basic_file_path_buffer template class, and ANSI and Unicode specialisations thereof.

#ifndef WINSTL_INCL_H_WINSTL_FILESYSTEM_FUNCTIONS
#define WINSTL_INCL_H_WINSTL_FILESYSTEM_FUNCTIONS

#ifndef __STLSOFT_DOCUMENTATION_SKIP_SECTION
# define WINSTL_VER_H_WINSTL_FILESYSTEM_FUNCTIONS_MAJOR     2
# define WINSTL_VER_H_WINSTL_FILESYSTEM_FUNCTIONS_MINOR     0
# define WINSTL_VER_H_WINSTL_FILESYSTEM_FUNCTIONS_REVISION  1
# define WINSTL_VER_H_WINSTL_FILESYSTEM_FUNCTIONS_EDIT      4
#endif /* !__STLSOFT_DOCUMENTATION_SKIP_SECTION */

/* /////////////////////////////////////////////////////////////////////////////
 * Compatibility
 */

/*
[Incompatibilies-start]
__STLSOFT_COMPILER_IS_MSVC: _MSC_VER<1200
[Incompatibilies-end]
 */

/* /////////////////////////////////////////////////////////////////////////////
 * Includes
 */

#ifndef WINSTL_INCL_H_WINSTL
# include "winstl.h"                        // Include the WinSTL root header
#endif /* !WINSTL_INCL_H_WINSTL */
#ifndef WINSTL_INCL_H_WINSTL_FILESYSTEM_TRAITS
# include "winstl_filesystem_traits.h"      // filesystem_traits
#endif /* WINSTL_INCL_H_WINSTL_FILESYSTEM_TRAITS */
#ifndef WINSTL_INCL_H_WINSTL_FILE_PATH_BUFFER
# include "winstl_file_path_buffer.h"       // file_path_buffer
#endif /* WINSTL_INCL_H_WINSTL_FILE_PATH_BUFFER */

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

/// \weakgroup libraries_filesystem File-System Library
/// \ingroup libraries
/// \brief This library provides facilities for defining and manipulating file-system objects

/// \weakgroup winstl_filesystem_library File-System Library (WinSTL)
/// \ingroup WinSTL libraries_filesystem
/// \brief This library provides facilities for defining and manipulating file-system objects for the Win32 API
/// @{

/* /////////////////////////////////////////////////////////////////////////////
 * Functions
 */

template <ss_typename_param_k C>
inline ws_bool_t create_directory_recurse_impl(C const *dir, LPSECURITY_ATTRIBUTES lpsa)
{
    typedef C                       char_type;
    typedef filesystem_traits<C>    traits_type;

    ws_bool_t    bRet;

    if( NULL == dir ||
        '\0' == *dir)
    {
        SetLastError(ERROR_DIRECTORY);

        bRet = false;
    }
    else
    {
        if(traits_type::file_exists(dir))
        {
            if(traits_type::is_directory(dir))
            {
                SetLastError(ERROR_FILE_EXISTS);

                bRet = true;
            }
            else
            {
                SetLastError(ERROR_ALREADY_EXISTS);

                bRet = false;
            }
        }
        else
        {
            basic_file_path_buffer<char_type>   sz;
            basic_file_path_buffer<char_type>   szParent;
            char_type                           *pszSlash;

            // May be being compiled absent exception support, so need to check the
            // file path buffers

            if( 0 == sz.size() ||
                0 == szParent.size())
            {
                bRet = false;
            }
            else
            {
                traits_type::str_copy(&sz[0], dir);
                traits_type::ensure_dir_end(&sz[0]);

                if(traits_type::create_directory(sz.c_str(), lpsa))
                {
                    SetLastError(ERROR_SUCCESS);

                    bRet = true;
                }
                else
                {
                    // Trim previous directory
                    traits_type::str_copy(&szParent[0], sz.c_str());
                    pszSlash = traits_type::str_rchr(szParent.c_str(), '\\');
                    if(pszSlash == NULL)
                    {
                        SetLastError(ERROR_INVALID_NAME);

                        bRet = false;
                    }
                    else
                    {
                        *pszSlash = '\0';                   // Will always have enough room for two bytes
                        pszSlash = traits_type::str_rchr(szParent.c_str(), '\\');
                        if(pszSlash == NULL)
                        {
                            SetLastError(ERROR_INVALID_NAME);

                            bRet = false;
                        }
                        else
                        {
                            *pszSlash = '\0';                   // Will always have enough room for two bytes

                            // If second character is ':', and total lengths is less than four,
                            // or the recurse create fails, then return false;
                            if( (   szParent[1] == ':' &&
                                    (SetLastError(ERROR_CANNOT_MAKE), traits_type::str_len(szParent.c_str()) < 4)) ||
                                !create_directory_recurse(szParent, lpsa))
                            {
                                bRet = false;
                            }
                            else
                            {
    //                          SetLastError(ERROR_SUCCESS);

                                bRet = traits_type::create_directory(sz.c_str(), lpsa);
                            }
                        }
                    }
                }
            }
        }
    }

    return bRet;
}

inline ws_bool_t create_directory_recurse(ws_char_a_t const *dir, LPSECURITY_ATTRIBUTES lpsa = NULL)
{
    return create_directory_recurse_impl(dir, lpsa);
}

inline ws_bool_t create_directory_recurse(ws_char_w_t const *dir, LPSECURITY_ATTRIBUTES lpsa = NULL)
{
    return create_directory_recurse_impl(dir, lpsa);
}

template <ss_typename_param_k S>
inline ws_bool_t create_directory_recurse(S const &dir, LPSECURITY_ATTRIBUTES lpsa = NULL)
{
    return create_directory_recurse(stlsoft_ns_qual(c_str_ptr)(dir), lpsa);
}

/* /////////////////////////////////////////////////////////////////////////////
 * Unit-testing
 */

#ifdef STLSOFT_UNITTEST

namespace unittest
{
    ss_bool_t test_winstl_file_path_buffer(unittest_reporter *r)
    {
        using stlsoft::unittest::unittest_initialiser;

        ss_bool_t               bSuccess    =   true;

        unittest_initialiser    init(r, "WinSTL", "filesystem_functions", __FILE__);

        file_path_buffer_a      buf_a;
        file_path_buffer_a      buf_a_2(buf_a);
        file_path_buffer_a      buf_a_3;

        buf_a_3 = buf_a;

        if(1 + _MAX_PATH != buf_a.size())
        {
            r->report("ANSI buffer size incorrect", __LINE__);
            bSuccess = false;
        }

        file_path_buffer_w      buf_w;

        if(::GetVersion() & 0x80000000)
        {
            if(1 + _MAX_PATH != buf_w.size())
            {
                r->report("Unicode (Win9x) buffer size incorrect", __LINE__);
                bSuccess = false;
            }
        }
        else
        {
            if(1 + 32000 != buf_w.size())
            {
                r->report("Unicode (Win NT) buffer size incorrect", __LINE__);
                bSuccess = false;
            }
        }

        return bSuccess;
    }

    unittest_registrar    unittest_winstl_file_path_buffer(test_winstl_file_path_buffer);

} // namespace unittest

#endif /* STLSOFT_UNITTEST */

/* ////////////////////////////////////////////////////////////////////////// */

/// @} // end of group winstl_filesystem_library

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

#endif /* WINSTL_INCL_H_WINSTL_FILESYSTEM_FUNCTIONS */

/* ////////////////////////////////////////////////////////////////////////// */
