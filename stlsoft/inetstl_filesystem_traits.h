/* /////////////////////////////////////////////////////////////////////////////
 * File:        inetstl_filesystem_traits.h (formerly MInetEnm.h)
 *
 * Purpose:     Contains the filesystem_traits template class, and ANSI and
 *              Unicode specialisations thereof.
 *
 * Created:     30th April 1999
 * Updated:     11th September 2004
 *
 * Home:        http://stlsoft.org/
 *
 * Copyright (c) 1999-2004, Matthew Wilson and Synesis Software
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


/// \file inetstl_filesystem_traits.h
///
/// Contains the filesystem_traits template class, and ANSI and Unicode specialisations thereof.

#ifndef INETSTL_INCL_H_INETSTL_FILESYSTEM_TRAITS
#define INETSTL_INCL_H_INETSTL_FILESYSTEM_TRAITS

#ifndef __STLSOFT_DOCUMENTATION_SKIP_SECTION
# define INETSTL_VER_H_INETSTL_FILESYSTEM_TRAITS_MAJOR        2
# define INETSTL_VER_H_INETSTL_FILESYSTEM_TRAITS_MINOR        0
# define INETSTL_VER_H_INETSTL_FILESYSTEM_TRAITS_REVISION     1
# define INETSTL_VER_H_INETSTL_FILESYSTEM_TRAITS_EDIT         39
#endif /* !__STLSOFT_DOCUMENTATION_SKIP_SECTION */

/* /////////////////////////////////////////////////////////////////////////////
 * Includes
 */

#ifndef INETSTL_INCL_H_INETSTL
# include "inetstl.h"                        // Include the InetSTL root header
#endif /* !INETSTL_INCL_H_INETSTL */
#ifndef STLSOFT_INCL_H_STLSOFT_SAPCAST
# include "stlsoft_sap_cast.h"               // sap_cast
#endif /* !STLSOFT_INCL_H_STLSOFT_SAPCAST */
#include <string.h>
#include <wchar.h>

# include "stlsoft_any_caster.h"

/* /////////////////////////////////////////////////////////////////////////////
 * Namespace
 */

#ifndef _INETSTL_NO_NAMESPACE
# if defined(_STLSOFT_NO_NAMESPACE) || \
     defined(__STLSOFT_DOCUMENTATION_SKIP_SECTION)
/* There is no stlsoft namespace, so must define ::inetstl */
namespace inetstl
{
# else
/* Define stlsoft::inetstl_project */

namespace stlsoft
{

namespace inetstl_project
{

# endif /* _STLSOFT_NO_NAMESPACE */
#endif /* !_INETSTL_NO_NAMESPACE */

/* ////////////////////////////////////////////////////////////////////////// */

/// \weakgroup libraries STLSoft Libraries
/// \brief The individual libraries

/// \weakgroup libraries_filesystem File-System Library
/// \ingroup libraries
/// \brief This library provides facilities for defining and manipulating file-system objects

/// \weakgroup inetstl_filesystem_library File-System Library (InetSTL)
/// \ingroup InetSTL libraries_filesystem
/// \brief This library provides facilities for defining and manipulating file-system objects for the WinInet API
/// @{

/* /////////////////////////////////////////////////////////////////////////////
 * Classes
 */

#ifdef __STLSOFT_DOCUMENTATION_SKIP_SECTION
/// Traits for accessing the correct file-system functions for a given character type
///
/// filesystem_traits is a traits class for determining the correct file-system
/// structures and functions for a given character type.
///
/// \param C The character type
template <ss_typename_param_k C>
struct filesystem_traits
{
public:
    /// The character type
    typedef C               char_type;
    /// The size type
    typedef is_size_t       size_type;
    /// The difference type
    typedef is_ptrdiff_t    difference_type;
    /// The find data type
    typedef WIN32_FIND_DATA find_data_type;     // Placeholder only

public:
    // General string handling

    /// Copies the contents of \c src to \c dest
    static char_type    *str_copy(char_type *dest, char_type const *src);
    /// Copies the contents of \c src to \c dest, up to cch \c characters
    static char_type    *str_n_copy(char_type *dest, char_type const *src, is_size_t cch);
    /// Appends the contents of \c src to \c dest
    static char_type    *str_cat(char_type *dest, char_type const *src);
    /// Comparies the contents of \c src and \c dest
    static is_int_t     str_compare(char_type const *s1, char_type const *s2);
    /// Comparies the contents of \c src and \c dest in a case-insensitive fashion
    static is_int_t     str_compare_no_case(char_type const *s1, char_type const *s2);
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
    static is_bool_t    has_dir_end(char_type const *dir);

    /// Returns \c true if dir is \c "." or \c ".."
    static is_bool_t    is_dots(char_type const *dir);
    /// Returns \c true if path is rooted
    static is_bool_t    is_path_rooted(char_type const *path);
    /// Returns \c true if path is an absolute path
    static is_bool_t    is_path_absolute(char_type const *path);

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
    /// \note It is '*'
    static char_type const *pattern_all();
    /// Gets the full path name into the given buffer, returning a pointer to the file-part
    static is_dword_t   get_full_path_name(HINTERNET hconn, char_type const *fileName, is_dword_t cchBuffer, char_type *buffer, char_type **ppFile);
    /// Gets the full path name into the given buffer
    static is_dword_t   get_full_path_name(HINTERNET hconn, char_type const *fileName, is_dword_t cchBuffer, char_type *buffer);

    // Internet connectivity

    /// Opens a WinInet session
    static HINTERNET    internet_open(char_type const *agent, is_dword_t accessType, char_type const *proxy, char_type const *proxyBypass, is_dword_t flags);
    /// Makes a connection to a FTP or HTTP site
    static HINTERNET    internet_connect(HINTERNET hsess, char_type const *server, INTERNET_PORT port, char_type const *userName, char_type const *password, is_dword_t service, is_dword_t flags, is_dword_t context);
    /// Closes the connection to the FTP or HTTP site
    static void         close_connection(HINTERNET hconn);

    // FindFile() API

    /// Initiate a file-system search
    static HINTERNET    find_first_file(HINTERNET hconn, char_type const *spec, find_data_type *findData, is_dword_t flags = 0, is_dword_t context = 0);
    /// Advance a given file-system search
    static is_bool_t    find_next_file(HANDLE h, find_data_type *findData);
    /// Closes the file-search
    static void         find_close(HINTERNET hfind);

    // File system state

    /// Sets the current directory to \c dir
    static is_bool_t    set_current_directory(HINTERNET hconn, char_type const *dir);
    /// Retrieves the name of the current directory into \c buffer up to a maximum of \c cchBuffer characters
    static is_uint_t    get_current_directory(HINTERNET hconn, is_size_t &cchBuffer, char_type *buffer);

    /// Returns whether a file exists or not
    static is_bool_t    file_exists(HINTERNET hconn, char_type const *fileName);

    // File system control

    /// Creates a directory
    static is_bool_t    create_directory(HINTERNET hconn, char_type const *dir);
    /// Deletes a directory
    static is_bool_t    remove_directory(HINTERNET hconn, char_type const *dir);

    /// Delete a file
    static is_bool_t    delete_file(HINTERNET hconn, char_type const *file);
    /// Rename a file
    static is_bool_t    rename_file(HINTERNET hconn, char_type const *currentName, char_type const *newName);

/*  BOOL WINAPI FtpGetFile(
HINTERNET hConnect,
LPCTSTR lpszRemoteFile,
LPCTSTR lpszNewFile,
BOOL fFailIfExists,
DWORD dwFlagsAndAttributes,
DWORD dwFlags,
DWORD dwContext);

    BOOL WINAPI FtpOpenFile(
HINTERNET hConnect,
LPCTSTR lpszFileName,
DWORD dwAccess,
DWORD dwFlags,
DWORD dwContext);

    BOOL WINAPI FtpPutFile(
HINTERNET hConnect,
LPCTSTR lpszLocalFile,
LPCTSTR lpszNewRemoteFile,
DWORD dwFlags,
DWORD dwContext);

    BOOL WINAPI InternetReadFile(
IN HINTERNET hFile,
IN LPVOID lpBuffer,
IN DWORD dwNumberOfBytesToRead,
OUT LPDWORD lpdwNumberOfBytesRead);

    BOOL InternetReadFileEx (
HINTERNET hFile,
LPINTERNET_BUFFERS lpBuffersOut,
DWORD dwFlags,
DWORD dwContext);
 */
};

#else

template <ss_typename_param_k C>
struct filesystem_traits;

STLSOFT_TEMPLATE_SPECIALISATION
struct filesystem_traits<is_char_a_t>
{
public:
    typedef is_char_a_t         char_type;
    typedef is_size_t           size_type;
    typedef is_ptrdiff_t        difference_type;
    typedef WIN32_FIND_DATAA    find_data_type;

public:
    // General string handling
    static char_type *str_copy(char_type *dest, char_type const *src)
    {
        return lstrcpyA(dest, src);
    }

    static char_type *str_n_copy(char_type *dest, char_type const *src, is_size_t cch)
    {
        return strncpy(dest, src, cch);
    }

    static char_type *str_cat(char_type *dest, char_type const *src)
    {
        return lstrcatA(dest, src);
    }

    static is_int_t str_compare(char_type const *s1, char_type const *s2)
    {
        return lstrcmpA(s1, s2);
    }

    static is_int_t str_compare_no_case(char_type const *s1, char_type const *s2)
    {
        return lstrcmpiA(s1, s2);
    }

    static size_type str_len(char_type const *src)
    {
        return static_cast<size_type>(lstrlenA(src));
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
        char_type       *end;
        char_type const separator = (NULL == strchr(dir, '/') && NULL != strchr(dir, '\\')) ? '\\' : '/';

        for(end = dir; *end != '\0'; ++end)
        {}

        if( dir < end &&
            *(end - 1) != separator)
        {
            *end        =   separator;
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

    static is_bool_t has_dir_end(char_type const *dir)
    {
        is_size_t len = str_len(dir);

        return (0 < len) && path_name_separator() == dir[len - 1];
    }

    static is_bool_t is_dots(char_type const *dir)
    {
        return  dir != 0 &&
                dir[0] == '.' &&
                (   dir[1] == '\0' ||
                    (    dir[1] == '.' &&
                        dir[2] == '\0'));
    }

    static is_bool_t is_path_rooted(char_type const *path)
    {
        inetstl_assert(NULL != path);

        return '/' == *path;
    }

    static is_bool_t is_path_absolute(char_type const *path)
    {
        return is_path_rooted(path);
    }

    static char_type path_separator()
    {
        return ';';
    }

    static char_type path_name_separator()
    {
        return '/';
    }

    static char_type const *pattern_all()
    {
        return "*";
    }

    static is_dword_t get_full_path_name(HINTERNET hconn, char_type const *fileName, is_dword_t cchBuffer, char_type *buffer, char_type **ppFile)
    {
        inetstl_assert(0 == cchBuffer || NULL != buffer);
        inetstl_assert(NULL == buffer || 0 != cchBuffer);

        // Deduce the separator
        char_type const separator = (NULL == strchr(fileName, '/') && NULL != strchr(fileName, '\\')) ? '\\' : '/';
        char_type       fullPath[1 + _MAX_PATH];

        // If we're not rooted, then get the current directory and concatenate
        if(separator != *fileName)
        {
            is_size_t   cchBuffer   =   stlsoft_num_elements(fullPath);
            int         isDot       =   0 == str_compare(fileName, ".");

            get_current_directory(hconn, cchBuffer, fullPath);
            if(!isDot)
            {
                ensure_dir_end(fullPath);
                str_cat(fullPath, fileName);
            }

            fileName = fullPath;
        }

        size_type       len =   str_len(fileName);

        if(NULL != buffer)
        {
            if(cchBuffer < len)
            {
                len = cchBuffer;
            }

            strncpy(buffer, fileName, cchBuffer);

            if(NULL != ppFile)
            {
                char_type const *pRSlash        =   strrchr(buffer, '/');
                char_type const *pRBackSlash    =   strrchr(buffer, '\\');

                if(pRSlash < pRBackSlash)
                {
                    pRSlash = pRBackSlash;
                }

                if(NULL == pRSlash)
                {
                    *ppFile = NULL;
                }
                else
                {
                    *ppFile = const_cast<char_type*>(pRSlash) + 1;
                }
            }
        }

        return len;
    }

    static is_dword_t get_full_path_name(HINTERNET hconn, char_type const *fileName, is_dword_t cchBuffer, char_type *buffer)
    {
        char_type *pFile;

        return get_full_path_name(hconn, fileName, cchBuffer, buffer, &pFile);
    }

    // Internet connectivity
    static HINTERNET internet_open(char_type const *agent, is_dword_t accessType, char_type const *proxy, char_type const *proxyBypass, is_dword_t flags)
    {
        return ::InternetOpenA(agent, accessType, proxy, proxyBypass, flags);
    }

    static HINTERNET internet_connect(HINTERNET hsess, char_type const *server, INTERNET_PORT port, char_type const *userName, char_type const *password, is_dword_t service, is_dword_t flags, is_dword_t context)
    {
        return ::InternetConnectA(hsess, server, port, userName, password, service, flags, context);
    }

    static void close_connection(HINTERNET hconn)
    {
        inetstl_assert(NULL != hconn);

        ::InternetCloseHandle(hconn);
    }

    // FindFile() API
    static HINTERNET find_first_file(HINTERNET hconn, char_type const *spec, find_data_type *findData, is_dword_t flags = 0, is_dword_t context = 0)
    {
        HINTERNET   hfind   =   ::FtpFindFirstFileA(hconn, spec, any_caster<find_data_type*, LPWIN32_FIND_DATAA, LPWIN32_FIND_DATAW>(findData), flags, context);

#if 0
        if(NULL == hfind)
        {
            findData->cFileName[0] = '\0';
        }

printf("find_first_file(0x%08x, %s => %s)\n", hfind, spec, findData->cFileName);
#endif /* 0 */

        return hfind;
    }

    static is_bool_t find_next_file(HANDLE h, find_data_type *findData)
    {
        return FALSE != ::InternetFindNextFileA(h, findData);
    }

    static void find_close(HINTERNET hfind)
    {
        inetstl_assert(NULL != hfind);

        ::InternetCloseHandle(hfind);
    }

    // File system state
    static is_bool_t set_current_directory(HINTERNET hconn, char_type const *dir)
    {
        return ::FtpSetCurrentDirectoryA(hconn, dir) != FALSE;
    }

    static is_uint_t get_current_directory(HINTERNET hconn, is_size_t &cchBuffer, char_type *buffer)
    {
        return ::FtpGetCurrentDirectoryA(hconn, buffer, sap_cast<unsigned long*>(&cchBuffer));
    }

    static is_bool_t file_exists(HINTERNET hconn, char_type const *fileName)
    {
        find_data_type  data;
        HINTERNET       hfind = find_first_file(hconn, fileName, &data);

        return (NULL == hfind) ? false : (find_close(hfind), true);
    }

    // File system control
    static is_bool_t create_directory(HINTERNET hconn, char_type const *dir)
    {
        return FALSE != ::FtpCreateDirectoryA(hconn, dir);
    }

    static is_bool_t remove_directory(HINTERNET hconn, char_type const *dir)
    {
        return FALSE != ::FtpRemoveDirectoryA(hconn, dir);
    }

    static is_bool_t delete_file(HINTERNET hconn, char_type const *file)
    {
        return FALSE != ::FtpDeleteFileA(hconn, file);
    }

    static is_bool_t rename_file(HINTERNET hconn, char_type const *currentName, char_type const *newName)
    {
        return FALSE != ::FtpRenameFileA(hconn, currentName, newName);
    }
};

STLSOFT_TEMPLATE_SPECIALISATION
struct filesystem_traits<is_char_w_t>
{
public:
    typedef is_char_w_t         char_type;
    typedef is_size_t           size_type;
    typedef is_ptrdiff_t        difference_type;
    typedef WIN32_FIND_DATAW    find_data_type;

public:
    // General string handling
    static char_type *str_copy(char_type *dest, char_type const *src)
    {
        return lstrcpyW(dest, src);
    }

    static char_type *str_n_copy(char_type *dest, char_type const *src, is_size_t cch)
    {
        return wcsncpy(dest, src, cch);
    }

    static char_type *str_cat(char_type *dest, char_type const *src)
    {
        return lstrcatW(dest, src);
    }

    static is_int_t str_compare(char_type const *s1, char_type const *s2)
    {
        return lstrcmpW(s1, s2);
    }

    static is_int_t str_compare_no_case(char_type const *s1, char_type const *s2)
    {
        return lstrcmpiW(s1, s2);
    }

    static size_type str_len(char_type const *src)
    {
        return static_cast<size_type>(lstrlenW(src));
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
        char_type       *end;
        char_type const separator = (NULL == wcschr(dir, L'/') && NULL != wcschr(dir, L'\\')) ? L'\\' : L'/';

        for(end = dir; *end != L'\0'; ++end)
        {}

        if( dir < end &&
            *(end - 1) != separator)
        {
            *end        =   separator;
            *(end + 1)  =   L'\0';
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

    static is_bool_t has_dir_end(char_type const *dir)
    {
        is_size_t len = str_len(dir);

        return (0 < len) && path_name_separator() == dir[len - 1];
    }

    static is_bool_t is_dots(char_type const *dir)
    {
        return  dir != 0 &&
                dir[0] == '.' &&
                (   dir[1] == L'\0' ||
                    (    dir[1] == L'.' &&
                        dir[2] == L'\0'));
    }

    static is_bool_t is_path_rooted(char_type const *path)
    {
        inetstl_assert(NULL != path);

        return L'/' == *path;
    }

    static is_bool_t is_path_absolute(char_type const *path)
    {
        return is_path_rooted(path);
    }

    static char_type path_separator()
    {
        return L';';
    }

    static char_type path_name_separator()
    {
        return L'/';
    }

    static char_type const *pattern_all()
    {
        return L"*";
    }

    static is_dword_t get_full_path_name(HINTERNET hconn, char_type const *fileName, is_dword_t cchBuffer, char_type *buffer, char_type **ppFile)
    {
        inetstl_assert(0 == cchBuffer || NULL != buffer);
        inetstl_assert(NULL == buffer || 0 != cchBuffer);

        // Deduce the separator
        char_type const separator = (NULL == wcschr(fileName, L'/') && NULL != wcschr(fileName, L'\\')) ? L'\\' : L'/';
        char_type       fullPath[1 + _MAX_PATH];

        // If we're not rooted, then get the current directory and concatenate
        if(separator != *fileName)
        {
            is_size_t   cchBuffer   =   stlsoft_num_elements(fullPath);
            int         isDot       =   0 == str_compare(fileName, L".");

            get_current_directory(hconn, cchBuffer, fullPath);
            if(!isDot)
            {
                ensure_dir_end(fullPath);
                str_cat(fullPath, fileName);
            }

            fileName = fullPath;
        }

        size_type       len =   str_len(fileName);

        if(NULL != buffer)
        {
            if(cchBuffer < len)
            {
                len = cchBuffer;
            }

            wcsncpy(buffer, fileName, cchBuffer);

            if(NULL != ppFile)
            {
                char_type const *pRSlash        =   wcsrchr(buffer, L'/');
                char_type const *pRBackSlash    =   wcsrchr(buffer, L'\\');

                if(pRSlash < pRBackSlash)
                {
                    pRSlash = pRBackSlash;
                }

                if(NULL == pRSlash)
                {
                    *ppFile = NULL;
                }
                else
                {
                    *ppFile = const_cast<char_type*>(pRSlash) + 1;
                }
            }
        }

        return len;
    }

    static is_dword_t get_full_path_name(HINTERNET hconn, char_type const *fileName, is_dword_t cchBuffer, char_type *buffer)
    {
        char_type *pFile;

        return get_full_path_name(hconn, fileName, cchBuffer, buffer, &pFile);
    }

    // Internet connectivity
    static HINTERNET internet_open(char_type const *agent, is_dword_t accessType, char_type const *proxy, char_type const *proxyBypass, is_dword_t flags)
    {
        return ::InternetOpenW(agent, accessType, proxy, proxyBypass, flags);
    }

    static HINTERNET internet_connect(HINTERNET hsess, char_type const *server, INTERNET_PORT port, char_type const *userName, char_type const *password, is_dword_t service, is_dword_t flags, is_dword_t context)
    {
        return ::InternetConnectW(hsess, server, port, userName, password, service, flags, context);
    }

    static void close_connection(HINTERNET hconn)
    {
        inetstl_assert(NULL != hconn);

        ::InternetCloseHandle(hconn);
    }

    // FindFile() API
    static HINTERNET find_first_file(HINTERNET hconn, char_type const *spec, find_data_type *findData, is_dword_t flags = 0, is_dword_t context = 0)
    {
        return ::FtpFindFirstFileW(hconn, spec, any_caster<find_data_type*, LPWIN32_FIND_DATAA, LPWIN32_FIND_DATAW>(findData), flags, context);
    }

    static is_bool_t find_next_file(HANDLE h, find_data_type *findData)
    {
        return FALSE != ::InternetFindNextFileW(h, findData);
    }

    static void find_close(HINTERNET hfind)
    {
        inetstl_assert(NULL != hfind);

        ::InternetCloseHandle(hfind);
    }

    // File system state
    static is_bool_t set_current_directory(HINTERNET hconn, char_type const *dir)
    {
        return ::FtpSetCurrentDirectoryW(hconn, dir) != FALSE;
    }

    static is_uint_t get_current_directory(HINTERNET hconn, is_size_t &cchBuffer, char_type *buffer)
    {
        return ::FtpGetCurrentDirectoryW(hconn, buffer, sap_cast<unsigned long*>(&cchBuffer));
    }

    /// Returns whether a file exists or not
    static is_bool_t file_exists(HINTERNET hconn, char_type const *fileName)
    {
        find_data_type  data;
        HINTERNET       hfind = find_first_file(hconn, fileName, &data);

        return (NULL == hfind) ? false : (find_close(hfind), true);
    }

    // File system control
    static is_bool_t create_directory(HINTERNET hconn, char_type const *dir)
    {
        return FALSE != ::FtpCreateDirectoryW(hconn, dir);
    }

    static is_bool_t remove_directory(HINTERNET hconn, char_type const *dir)
    {
        return FALSE != ::FtpRemoveDirectoryW(hconn, dir);
    }

    static is_bool_t delete_file(HINTERNET hconn, char_type const *file)
    {
        return FALSE != ::FtpDeleteFileW(hconn, file);
    }

    static is_bool_t rename_file(HINTERNET hconn, char_type const *currentName, char_type const *newName)
    {
        return FALSE != ::FtpRenameFileW(hconn, currentName, newName);
    }
};

#endif /* __STLSOFT_DOCUMENTATION_SKIP_SECTION */

/* ////////////////////////////////////////////////////////////////////////// */

/// @} // end of group inetstl_filesystem_library

/* ////////////////////////////////////////////////////////////////////////// */

#ifndef _INETSTL_NO_NAMESPACE
# if defined(_STLSOFT_NO_NAMESPACE) || \
     defined(__STLSOFT_DOCUMENTATION_SKIP_SECTION)
} // namespace inetstl
# else
} // namespace inetstl_project
} // namespace stlsoft
# endif /* _STLSOFT_NO_NAMESPACE */
#endif /* !_INETSTL_NO_NAMESPACE */

/* ////////////////////////////////////////////////////////////////////////// */

#endif /* INETSTL_INCL_H_INETSTL_FILESYSTEM_TRAITS */

/* ////////////////////////////////////////////////////////////////////////// */
