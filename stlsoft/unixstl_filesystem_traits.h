/* /////////////////////////////////////////////////////////////////////////////
 * File:        unixstl_filesystem_traits.h
 *
 * Purpose:     Contains the filesystem_traits template class, and ANSI and
 *              Unicode specialisations thereof.
 *
 * Created:     15th November 2002
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


/// \file unixstl_filesystem_traits.h
///
/// Contains the filesystem_traits template class, and ANSI and Unicode specialisations thereof.

#ifndef UNIXSTL_INCL_H_UNIXSTL_FILESYSTEM_TRAITS
#define UNIXSTL_INCL_H_UNIXSTL_FILESYSTEM_TRAITS

#ifndef __STLSOFT_DOCUMENTATION_SKIP_SECTION
# define UNIXSTL_VER_H_UNIXSTL_FILESYSTEM_TRAITS_MAJOR      2
# define UNIXSTL_VER_H_UNIXSTL_FILESYSTEM_TRAITS_MINOR      0
# define UNIXSTL_VER_H_UNIXSTL_FILESYSTEM_TRAITS_REVISION   1
# define UNIXSTL_VER_H_UNIXSTL_FILESYSTEM_TRAITS_EDIT       49
#endif /* !__STLSOFT_DOCUMENTATION_SKIP_SECTION */

/* /////////////////////////////////////////////////////////////////////////////
 * Includes
 */

#ifndef UNIXSTL_INCL_H_WINSTL
# include "unixstl.h"                   // Include the UNIXSTL root header
#endif /* !UNIXSTL_INCL_H_WINSTL */
#include <errno.h>
#include <limits.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <wchar.h>
#include <sys/types.h>
#include <sys/stat.h>

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

/* ////////////////////////////////////////////////////////////////////////// */

/// \weakgroup libraries STLSoft Libraries
/// \brief The individual libraries

/// \weakgroup libraries_filesystem File-System Library
/// \ingroup libraries
/// \brief This library provides facilities for defining and manipulating file-system objects

/// \defgroup unixstl_filesystem_library File-System Library (UNIXSTL)
/// \ingroup UNIXSTL libraries_filesystem
/// \brief This library provides facilities for defining and manipulating UNIX file-system objects
/// @{

/* /////////////////////////////////////////////////////////////////////////////
 * Classes
 *
 * filesystem_traits                -   a traits template, along with
 * filesystem_traits<us_char_a_t>       and
 * filesystem_traits<us_char_w_t>
 */

/// \brief Traits class for file-system operations
///
/// \param C The character type (e.g. \c char, \c wchar_t)
template <ss_typename_param_k C>
#ifdef __STLSOFT_DOCUMENTATION_SKIP_SECTION
struct filesystem_traits
{
public:
    typedef C           char_type;  /*!< The character type */
    typedef us_size_t   size_type;  /*!< The size type */

#ifdef PATH_MAX
    enum
    {
        maxPathLength   =   1 + PATH_MAX    //!< The maximum length of a path for the current file system
    };
#endif /* PATH_MAX */

public:
    // General string handling

    /// Copies the contents of \c src to \c dest
    static char_type    *str_copy(char_type *dest, char_type const *src);
    /// Copies the contents of \c src to \c dest, up to cch \c characters
    static char_type    *str_n_copy(char_type *dest, char_type const *src, us_size_t cch);
    /// Appends the contents of \c src to \c dest
    static char_type    *str_cat(char_type *dest, char_type const *src);
    /// Comparies the contents of \c src and \c dest
    static us_int_t     str_compare(char_type const *s1, char_type const *s2);
    /// Comparies the contents of \c src and \c dest in a case-insensitive fashion
    static us_int_t     str_compare_no_case(char_type const *s1, char_type const *s2);
    /// Evaluates the length of \c src
    static size_type    str_len(char_type const *src);
    /// Finds the given character \c ch in \c s
    static char_type    *str_chr(char_type const *s, char_type ch);
    /// Finds the rightmost instance \c ch in \c s
    static char_type    *str_rchr(char_type const *s, char_type ch);
    /// Finds the given substring \c sub in \c s
    static char_type    *str_str(char_type const *s, char_type const *sub);

    // File-system entry names

    /// Appends a path name separator to \c dir if one does not exist
    ///
    /// \see \link #path_name_separator path_name_separator() \endlink
    static char_type    *ensure_dir_end(char_type *dir);
    /// Removes the path name separator from the end of \c dir, if it has it
    ///
    /// \see \link #path_name_separator path_name_separator() \endlink
    static char_type    *remove_dir_end(char_type *dir);
    /// Returns \c true if \c dir has trailing path name separator
    ///
    /// \see \link #path_name_separator path_name_separator() \endlink
    static us_bool_t    has_dir_end(char_type const *dir);

    /// Returns \c true if dir is \c "." or \c ".."
    static us_bool_t    is_dots(char_type const *dir);
    /// Returns \c true if path is rooted
    static us_bool_t    is_path_rooted(char_type const *path);
    /// Returns \c true if path is an absolute path
    static us_bool_t    is_path_absolute(char_type const *path);

    /// Returns the path separator
    ///
    /// This is the separator that is used to separate multiple paths on the operating system. On UNIX it is ':'
    static char_type    path_separator();
    /// Returns the path name separator
    ///
    /// This is the separator that is used to separate parts of a path on the operating system. On UNIX it is '/'
    static char_type    path_name_separator();
    /// Returns the wildcard pattern that represents all possible matches
    ///
    /// \note On UNIX it is '*'
    static char_type const *pattern_all();
    /// The maximum length of a path on the file-system
    ///
    /// \note Because not all systems support fixed maximum path lengths, the value of this function is notionally dynamic
    static us_size_t    path_max();
    /// Gets the full path name into the given buffer, returning a pointer to the file-part
    static us_size_t    get_full_path_name(char_type const *fileName, us_size_t cchBuffer, char_type *buffer, char_type **ppFile);
    /// Gets the full path name into the given buffer
    static us_size_t    get_full_path_name(char_type const *fileName, us_size_t cchBuffer, char_type *buffer);

    // File system state

    /// Sets the current directory to \c dir
    static us_bool_t    set_current_directory(char_type const *dir);
    /// Retrieves the name of the current directory into \c buffer up to a maximum of \c cchBuffer characters
    static us_uint_t    get_current_directory(us_uint_t cchBuffer, char_type *buffer);

    /// Returns whether a file exists or not
    static us_bool_t    file_exists(char_type const *fileName);
    /// Returns whether the given path represents a file
    static us_bool_t    is_file(char_type const *path);
    /// Returns whether the given path represents a directory
    static us_bool_t    is_directory(char_type const *path);

    // Environment

    /// Gets an environment variable into the given buffer
    ///
    /// \param name The name of the variable to find
    /// \param buffer The buffer in which to write the variable. If this is NULL, then the required length is returned
    /// \param cchBuffer The size of the buffer, in characters
    static us_uint_t    get_environment_variable(char_type const *name, char_type *buffer, us_uint_t cchBuffer);
    /// Expands environment strings in \c src into \dest, up to a maximum \c cchDest characters
    static us_uint_t    expand_environment_strings(char_type const *src, char_type *buffer, us_uint_t cchBuffer);
};
#else
struct filesystem_traits;

STLSOFT_TEMPLATE_SPECIALISATION
struct filesystem_traits<us_char_a_t>
{
public:
    typedef us_char_a_t char_type;
    typedef us_size_t   size_type;

#ifdef PATH_MAX
    enum
    {
        maxPathLength   =   1 + PATH_MAX    //!< The maximum length of a path for the current file system
    };
#endif /* PATH_MAX */

public:
    // General string handling
    static char_type *str_copy(char_type *dest, char_type const *src)
    {
        return strcpy(dest, src);
    }

    static char_type *str_n_copy(char_type *dest, char_type const *src, us_size_t cch)
    {
        return strncpy(dest, src, cch);
    }

    static char_type *str_cat(char_type *dest, char_type const *src)
    {
        return strcat(dest, src);
    }

    static us_int_t str_compare(char_type const *s1, char_type const *s2)
    {
        return strcmp(s1, s2);
    }

    static us_int_t str_compare_no_case(char_type const *s1, char_type const *s2);

    static size_type str_len(char_type const *src)
    {
        return static_cast<size_type>(strlen(src));
    }

    static char_type *str_chr(char_type const *s, char_type ch)
    {
        return const_cast<char_type*>(strchr(s, ch));
    }

    static char_type *str_rchr(char_type const *s, char_type ch)
    {
        return const_cast<char_type*>(strrchr(s, ch));
    }

    static char_type *str_str(char_type const *s, char_type const *sub)
    {
        return const_cast<char_type*>(strstr(s, sub));
    }

    // File-system entry names
    static char_type *ensure_dir_end(char_type *dir)
    {
        char_type   *end;

        for(end = dir; *end != '\0'; ++end)
        {}

        if( dir < end &&
            *(end - 1) != path_name_separator())
        {
            *end        =   path_name_separator();
            *(end + 1)  =   '\0';
        }

        return dir;
    }

    static char_type *remove_dir_end(char_type *dir)
    {
        char_type   *end;

        for(end = dir; *end != '\0'; ++end)
        {}

        if( dir < end &&
            *(end - 1) == path_name_separator())
        {
            *(end - 1)  =   '\0';
        }

        return dir;
    }

    static us_bool_t has_dir_end(char_type const *dir)
    {
        unixstl_assert(NULL != dir);

        us_size_t len = str_len(dir);

        return (0 < len) && path_name_separator() == dir[len - 1];
    }

    static us_bool_t is_dots(char_type const *dir)
    {
        return  dir != 0 &&
                dir[0] == '.' &&
                (   dir[1] == '\0' ||
                    (    dir[1] == '.' &&
                        dir[2] == '\0'));
    }

    static us_bool_t is_path_rooted(char_type const *path)
    {
        unixstl_assert(NULL != path);

        return '/' == *path;
    }

    static us_bool_t is_path_absolute(char_type const *path)
    {
        return is_path_rooted(path);
    }

    static char_type path_separator()
    {
        return ':';
    }

    static char_type path_name_separator()
    {
        return '/';
    }

    static char_type const *pattern_all()
    {
        return "*";
    }

    static us_size_t path_max()
    {
#if defined(PATH_MAX)
        return 1 + PATH_MAX;
#else /* ? PATH_MAX */
        return 1 + pathconf("/", _PC_PATH_MAX);
#endif /* PATH_MAX */
    }

    static us_size_t get_full_path_name(char_type const *fileName, us_size_t cchBuffer, char_type *buffer, char_type **ppFile)
    {
        unixstl_assert(NULL != ppFile);

        us_size_t   cchRequired =   get_full_path_name(fileName, 0, NULL);

        if(!(cchBuffer < cchRequired))
        {
            get_full_path_name(fileName, cchBuffer, buffer);

            // Now search for the file separator
            char_type *pFile    =   str_rchr(buffer, path_name_separator());
#if defined(WIN32)
            char_type *pFile2   =   str_rchr(buffer, '\\');

            if(NULL == pFile)
            {
                pFile = pFile2;
            }
            else if(NULL != pFile2)
            {
                if(pFile2 > pFile)
                {
                    pFile = pFile2;
                }
            }
#endif /* WIN32 */

            if(NULL != (*ppFile = pFile))
            {
                (*ppFile)++;
            }
        }

        return cchRequired;
    }

    static us_size_t get_full_path_name(char_type const *fileName, us_size_t cchBuffer, char_type *buffer)
    {
        unixstl_assert(NULL != fileName);
        unixstl_assert(0 == cchBuffer || NULL != buffer);

        us_size_t   len = str_len(fileName);

        // The next thing to so is determine whether the path is absolute, in
        // which case we'll just copy it into the buffer
#if defined(WIN32)
        // Since UNIXSTL is partially tested on Win32, this is to allow it to work with Win32 paths
        if( fileName[0] == path_name_separator() ||
            fileName[0] == '\\' ||
            (   strlen(fileName) > 1 &&
                fileName[1] == ':'))
#else /* ? WIN32 */
        if(fileName[0] == path_name_separator())
#endif /* WIN32 */
        {
            // Given path is absolute, so simply copy into buffer

            if(!(cchBuffer < len))
            {
                // Given buffer is large enough, so copy
                str_copy(buffer, fileName);
            }
        }
        else
        {
            // Given path is relative, so get the current directory, and then concatenate
#ifdef PATH_MAX
            char_type   directory[1 + PATH_MAX];
#else /* ? PATH_MAX */
            char_type   directory[1 + 4096];    // A guess at a realistic maximum. Probably a good one, but you never know!
#endif /* PATH_MAX */

            if(0 == get_current_directory(stlsoft_num_elements(directory), directory))
            {
                // The call failed, so indicate that to caller
                len = 0;
            }
            else
            {
                // All that remains is to

                us_size_t   lenDir      =   str_len(remove_dir_end(directory));
                int         isDot       =   0 == str_compare(fileName, ".");

                len = isDot ? lenDir : (len + 1 + lenDir);

                if(!(cchBuffer < len))
                {
                    str_copy(buffer, directory);

                    if(0 != str_compare(fileName, "."))
                    {
                        ensure_dir_end(buffer);
                        str_cat(buffer, fileName);
                    }
                }
            }
        }

        return len;
    }

    // File system state
    static us_bool_t set_current_directory(char_type const *dir)
    {
        return chdir(dir) == 0;
    }

    static us_uint_t get_current_directory(us_uint_t cchBuffer, char_type *buffer)
    {
        return getcwd(buffer, cchBuffer) != 0;
    }

    static us_bool_t file_exists(char_type const *fileName)
    {
        struct stat st;

        return 0 == stat(fileName, &st) || errno != ENOENT;
    }

    static us_bool_t is_file(char_type const *path)
    {
        struct stat st;

        return 0 == stat(path, &st) && S_IFREG == (st.st_mode & S_IFMT);
    }

    static us_bool_t is_directory(char_type const *path)
    {
        struct stat st;

        return 0 == stat(path, &st) && S_IFDIR == (st.st_mode & S_IFMT);
    }

    // Environment

    static us_uint_t get_environment_variable(char_type const *name, char_type *buffer, us_uint_t cchBuffer)
    {
        char    *var = getenv(name);

        if(NULL == var)
        {
            return 0;
        }
        else
        {
            ss_size_t   var_len = strlen(var);

            if(NULL == buffer)
            {
                return var_len;
            }
            else
            {
                strncpy(buffer, var, cchBuffer);

                return (var_len < cchBuffer) ? var_len : cchBuffer;
            }
        }
    }

    static us_uint_t expand_environment_strings(char_type const *src, char_type *buffer, us_uint_t cchBuffer);
};

#if 0
STLSOFT_TEMPLATE_SPECIALISATION
struct filesystem_traits<us_char_w_t>
{
public:
    typedef us_char_w_t char_type;
    typedef us_size_t   size_type;

#ifdef PATH_MAX
    enum
    {
        maxPathLength   =   1 + PATH_MAX    //!< The maximum length of a path for the current file system
    };
#endif /* PATH_MAX */

public:
    // General string handling
    static char_type *str_copy(char_type *dest, char_type const *src)
    {
        return wcscpy(dest, src);
    }

    static char_type *str_n_copy(char_type *dest, char_type const *src, ws_size_t cch)
    {
        return wcsncpy(dest, src, cch);
    }

    static char_type *str_cat(char_type *dest, char_type const *src)
    {
        return wcscat(dest, src);
    }

    static us_int_t str_compare(char_type const *s1, char_type const *s2)
    {
        return wcscmp(s1, s2);
    }

    static is_int_t str_compare_no_case(char_type const *s1, char_type const *s2);

    static size_type str_len(char_type const *src)
    {
        return static_cast<size_type>(wcslen(src));
    }

    static char_type *str_chr(char_type const *s, char_type ch)
    {
        return const_cast<char_type*>(wcschr(s, ch));
    }

    static char_type *str_rchr(char_type const *s, char_type ch)
    {
        return const_cast<char_type*>(wcsrchr(s, ch));
    }

    static char_type *str_str(char_type const *s, char_type const *sub)
    {
        return const_cast<char_type*>(wcsstr(s, sub));
    }

    // File-system entry names
    static char_type *ensure_dir_end(char_type *dir)
    {
        char_type   *end;

        for(end = dir; *end != L'\0'; ++end)
        {}

        if( dir < end &&
            *(end - 1) != path_name_separator())
        {
            *end        =   path_name_separator();
            *(end + 1)  =   L'\0';
        }

        return dir;
    }

    static char_type *remove_dir_end(char_type *dir)
    {
        char_type   *end;

        for(end = dir; *end != L'\0'; ++end)
        {}

        if( dir < end &&
            *(end - 1) == path_name_separator())
        {
            *(end - 1)  =   L'\0';
        }

        return dir;
    }

    static us_bool_t has_dir_end(char_type const *dir)
    {
        us_size_t len = str_len(dir);

        return (0 < len) && path_name_separator() == dir[len - 1];
    }

    static us_bool_t is_dots(char_type const *dir)
    {
        return  dir != 0 &&
                dir[0] == '.' &&
                (   dir[1] == L'\0' ||
                    (    dir[1] == L'.' &&
                        dir[2] == L'\0'));
    }

    static us_bool_t is_path_rooted(char_type const *path)
    {
        unixstl_assert(NULL != path);

        return L'/' == *path;
    }

    static us_bool_t is_path_absolute(char_type const *path)
    {
        return is_path_rooted(path);
    }

    static char_type path_separator()
    {
        return L':';
    }

    static char_type path_name_separator()
    {
        return L'/';
    }

    static char_type const *pattern_all()
    {
        return L"*";
    }

    static us_size_t path_max()
    {
#if defined(PATH_MAX)
        return 1 + PATH_MAX;
#else /* ? PATH_MAX */
        return 1 + pathconf("/", _PC_PATH_MAX);
#endif /* PATH_MAX */
    }

    static us_size_t get_full_path_name(char_type const *fileName, us_size_t cchBuffer, char_type *buffer, char_type **ppFile);

    static us_size_t get_full_path_name(char_type const *fileName, us_size_t cchBuffer, char_type *buffer)
    {
        char_type *pFile;

        return get_full_path_name(fileName, cchBuffer, buffer, &pFile);
    }

    // File system state
    static us_bool_t set_current_directory(char_type const *dir);

    static us_uint_t get_current_directory(us_uint_t cchBuffer, char_type *buffer);
};
#endif /* 0 */

#endif /* !__STLSOFT_DOCUMENTATION_SKIP_SECTION */

/* /////////////////////////////////////////////////////////////////////////////
 * Unit-testing
 */

#ifdef STLSOFT_UNITTEST

namespace unittest
{
    namespace
    {
        typedef filesystem_traits<char>     traits_a_t;
        typedef filesystem_traits<wchar_t>  traits_w_t;
    }

    ss_bool_t test_get_full_path_name(unittest_reporter *r)
    {
        ss_bool_t   bSuccess    =   true;

        char        sz1[1 + 4096];
        char        sz2[1 + 4096];
        char        cwd[1 + 4096];

        if(NULL == getcwd(cwd, stlsoft_num_elements(cwd)))
        {
            r->report("Failed to get current directory", __LINE__);
            bSuccess = false;
        }
        else
        {
            char        *pFile1;
            char        *pFile2;
            ss_size_t   cch1    =   traits_a_t::get_full_path_name(".", stlsoft_num_elements(sz1), sz1, &pFile1);
            ss_size_t   cch2    =   traits_a_t::get_full_path_name(cwd, stlsoft_num_elements(sz2), sz2, &pFile2);

            STLSOFT_SUPPRESS_UNUSED(cch1);
            STLSOFT_SUPPRESS_UNUSED(cch2);

            if( 0 != traits_a_t::str_compare(sz1, sz2) ||
                0 != traits_a_t::str_compare(pFile1, pFile2))
            {
                r->report("Directory equivalence failed", __LINE__);
                bSuccess = false;
            }

            traits_a_t::str_cat(traits_a_t::ensure_dir_end(cwd), "../..");

            cch1    =   traits_a_t::get_full_path_name("../..", stlsoft_num_elements(sz1), sz1, &pFile1);
            cch2    =   traits_a_t::get_full_path_name(cwd, stlsoft_num_elements(sz2), sz2, &pFile2);

            STLSOFT_SUPPRESS_UNUSED(cch1);
            STLSOFT_SUPPRESS_UNUSED(cch2);

            if( 0 != traits_a_t::str_compare(sz1, sz2) ||
                0 != traits_a_t::str_compare(pFile1, pFile2))
            {
                r->report("Directory equivalence failed", __LINE__);
                bSuccess = false;
            }
        }

        return bSuccess;
    }


    ss_bool_t test_unixstl_filesystem_traits(unittest_reporter *r)
    {
        using stlsoft::unittest::unittest_initialiser;

        ss_bool_t               bSuccess    =   true;

        unittest_initialiser    init(r, "UNIXSTL", "filesystem_traits", __FILE__);

        // (i) get_full_path_name()
        if(bSuccess)
        {
            bSuccess = test_get_full_path_name(r);
        }

        return bSuccess;
    }

    unittest_registrar    unittest_unixstl_filesystem_traits(test_unixstl_filesystem_traits);

} // namespace unittest

#endif /* STLSOFT_UNITTEST */

/* ////////////////////////////////////////////////////////////////////////// */

/// @} // end of group unixstl_filesystem_library

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

#endif /* UNIXSTL_INCL_H_UNIXSTL_FILESYSTEM_TRAITS */

/* ////////////////////////////////////////////////////////////////////////// */
