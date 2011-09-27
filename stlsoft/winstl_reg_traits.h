/* /////////////////////////////////////////////////////////////////////////////
 * File:        winstl_reg_traits.h
 *
 * Purpose:     Contains the reg_traits template class, and ANSI
 *              and Unicode specialisations thereof.
 *
 * Created:     19th January 2002
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


/// \file winstl_reg_traits.h
///
/// Contains the reg_traits template class, and ANSI and Unicode specialisations thereof.

#ifndef WINSTL_INCL_H_WINSTL_REG_TRAITS
#define WINSTL_INCL_H_WINSTL_REG_TRAITS

#ifndef __STLSOFT_DOCUMENTATION_SKIP_SECTION
# define WINSTL_VER_H_WINSTL_REG_TRAITS_MAJOR       2
# define WINSTL_VER_H_WINSTL_REG_TRAITS_MINOR       0
# define WINSTL_VER_H_WINSTL_REG_TRAITS_REVISION    1
# define WINSTL_VER_H_WINSTL_REG_TRAITS_EDIT        39
#endif /* !__STLSOFT_DOCUMENTATION_SKIP_SECTION */

/* /////////////////////////////////////////////////////////////////////////////
 * Includes
 */

#ifndef WINSTL_INCL_H_WINSTL
# include "winstl.h"                        // Include the WinSTL root header
#endif /* !WINSTL_INCL_H_WINSTL */
#ifndef WINSTL_INCL_H_WINSTL_REG_DEFS
# include "winstl_reg_defs.h"               // The WinSTL reg API standard types
#endif /* WINSTL_INCL_H_WINSTL_REG_DEFS */

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

/// \weakgroup winstl_reg_library Registry Library
/// @{

/* /////////////////////////////////////////////////////////////////////////////
 * Classes
 */

#ifdef __STLSOFT_DOCUMENTATION_SKIP_SECTION
/// Traits for accessing the correct registry functions for a given character type
///
/// reg_traits is a traits class for determining the correct registry
/// structures and functions for a given character type.
///
/// \param C The character type
template <ss_typename_param_k C>
struct reg_traits
{
public:
    /// The character type
    typedef C               char_type;
    /// The size type
    typedef ws_size_t       size_type;
    /// The difference type
    typedef ws_ptrdiff_t    difference_type;
    /// The registry key type
    typedef HKEY            hkey_type;
    /// The string type
    typedef reg_string_t    string_type;        // Placeholder only
    /// The time type
    typedef FILETIME        time_type;

public:
    // General string handling

    /// Copies the contents of \c src to \c dest
    static char_type    *str_copy(char_type *dest, char_type const *src);
    /// Appends the contents of \c src to \c dest
    static char_type    *str_cat(char_type *dest, char_type const *src);
    /// Comparies the contents of \c src and \c dest
    static ws_int_t     str_compare(char_type *dest, char_type const *src);
    /// Evaluates the length of \c src
    static size_type    str_len(char_type const *src);

    // Registry names

    /// Appends a registry path separator to \c path if one does not exist
    static char_type    *ensure_subkey_end(char_type *path);

    /// Expands environment strings in \c src into \dest, up to a maximum \c cchDest characters
    static size_type    expand_env_strings(char_type const *src, char_type *dest, size_type cch_dest);

    /// Duplicates a registry key
    static hkey_type    key_dup(        hkey_type       hkey,
                                        REGSAM          samDesired = KEY_ALL_ACCESS);
    /// Opens a registry key
    static ws_long_t    reg_open_key(   hkey_type       hkey,
                                        char_type const *sub_key_name,
                                        hkey_type       *hkey_result,
                                        REGSAM          samDesired = KEY_ALL_ACCESS);
    /// Opens a registry key
    static ws_long_t    reg_create_key( hkey_type       hkey,
                                        char_type const *sub_key_name,
                                        hkey_type       *hkey_result,
                                        REGSAM          samDesired = KEY_ALL_ACCESS);
    static ws_long_t    reg_create_key( hkey_type       hkey,
                                        char_type const *sub_key_name,
                                        hkey_type       *hkey_result,
                                        ws_bool_t       &bCreated,
                                        REGSAM          samDesired = KEY_ALL_ACCESS);
    /// Queries a registry key value
    static ws_long_t    reg_query_value(hkey_type       hkey,
                                        char_type const *value_name,
                                        ws_dword_t      &value_type,
                                        void            *data,
                                        size_type       &cb_data);
    /// Queries a registry key's characteristics
    static ws_long_t    reg_query_info( hkey_type       hkey,
                                        char_type       *key_class,
                                        size_type       *cch_key_class,
                                        ws_uint_t       *c_sub_keys,
                                        size_type       *cch_sub_key_max,
                                        size_type       *cch_key_class_max,
                                        ws_uint_t       *c_values,
                                        size_type       *cch_value_name_max,
                                        size_type       *cb_value_data_max,
                                        size_type       *cb_security_descriptor_max,
                                        time_type       *time_last_write);
    /// Enumerates a registry key's sub-keys
    static ws_long_t    reg_enum_key(   hkey_type       hkey,
                                        ws_dword_t      index,
                                        char_type       *key_name,
                                        size_type       *cch_key_name,
                                        char_type       *key_class,
                                        size_type       *cch_key_class,
                                        time_type       *time_last_write);
    /// Enumerates a registry key's values
    static ws_long_t    reg_enum_value( hkey_type       hkey,
                                        ws_dword_t      index,
                                        char_type       *value_name,
                                        size_type       *cch_value_name,
                                        ws_dword_t      *value_type,
                                        void            *data,
                                        size_type       &cb_data);
}

#else

template <ss_typename_param_k C>
struct reg_traits;

STLSOFT_TEMPLATE_SPECIALISATION
struct reg_traits<ws_char_a_t>
{
public:
    typedef ws_char_a_t         char_type;
    typedef ws_size_t           size_type;
    typedef ws_ptrdiff_t        difference_type;
    typedef HKEY                hkey_type;
    typedef reg_string_a_t      string_type;
    typedef FILETIME            time_type;

public:
    static char_type *str_copy(char_type *dest, char_type const *src)
    {
        return lstrcpyA(dest, src);
    }

    static char_type *str_cat(char_type *dest, char_type const *src)
    {
        return lstrcatA(dest, src);
    }

    static ws_int_t str_compare(char_type const *s1, char_type const *s2)
    {
        return lstrcmpA(s1, s2);
    }

    static size_type str_len(char_type const *src)
    {
        return static_cast<size_type>(lstrlenA(src));
    }

    static char_type *ensure_subkey_end(char_type *path)
    {
        char_type   *end;

        for(end = path; *end != '\0'; ++end)
        {}

        if( path < end &&
            *(end - 1) != '\\')
        {
            *end        =   '\\';
            *(end + 1)  =   '\0';
        }

        return path;
    }

    static size_type expand_env_strings(char_type const *src, char_type *dest, size_type cch_dest)
    {
        return static_cast<size_type>(::ExpandEnvironmentStringsA(src, dest, cch_dest));
    }

    static hkey_type key_dup(hkey_type hkey, REGSAM samDesired = KEY_ALL_ACCESS)
    {
        hkey_type   hkeyDup;
        ws_long_t   res = ::RegOpenKeyExA(hkey, "", 0, samDesired, &hkeyDup);

        if(res != ERROR_SUCCESS)
        {
            hkeyDup = NULL;
        }

        return hkeyDup;
    }

    static ws_long_t reg_open_key(hkey_type hkey, char_type const *sub_key_name, hkey_type *hkey_result, REGSAM samDesired = KEY_ALL_ACCESS)
    {
        return ::RegOpenKeyExA(hkey, sub_key_name, 0, samDesired, hkey_result);
    }

    static ws_long_t reg_create_key(hkey_type hkey, char_type const *sub_key_name, hkey_type *hkey_result, REGSAM samDesired = KEY_ALL_ACCESS)
    {
        return ::RegCreateKeyExA(hkey, sub_key_name, 0, NULL, 0, samDesired, NULL, hkey_result, NULL);
    }

    static ws_long_t reg_create_key(hkey_type hkey, char_type const *sub_key_name, hkey_type *hkey_result, ws_bool_t &bCreated, REGSAM samDesired = KEY_ALL_ACCESS)
    {
        DWORD       disposition;
        ws_long_t   res =   ::RegCreateKeyExA(hkey, sub_key_name, 0, NULL, 0, samDesired, NULL, hkey_result, &disposition);

        bCreated = (ERROR_SUCCESS == res) && (REG_CREATED_NEW_KEY == disposition);

        return res;
    }

    static ws_long_t reg_query_value(hkey_type hkey, char_type const *value_name, ws_dword_t &value_type, void *data, size_type &cb_data)
    {
        return ::RegQueryValueExA(hkey, value_name, NULL, &value_type, static_cast<LPBYTE>(data), reinterpret_cast<LPDWORD>(&cb_data));
    }

    static ws_long_t reg_query_info(    hkey_type       hkey,
                                        char_type       *key_class,
                                        size_type       *cch_key_class,
                                        ws_uint_t       *c_sub_keys,
                                        size_type       *cch_sub_key_max,
                                        size_type       *cch_key_class_max,
                                        ws_uint_t       *c_values,
                                        size_type       *cch_value_name_max,
                                        size_type       *cb_value_data_max,
                                        size_type       *cb_security_descriptor_max,
                                        time_type       *time_last_write)
    {
        return ::RegQueryInfoKeyA(hkey, key_class, reinterpret_cast<LPDWORD>(cch_key_class), NULL, reinterpret_cast<LPDWORD>(c_sub_keys), reinterpret_cast<LPDWORD>(cch_sub_key_max), reinterpret_cast<LPDWORD>(cch_key_class_max), reinterpret_cast<LPDWORD>(c_values), reinterpret_cast<LPDWORD>(cch_value_name_max), reinterpret_cast<LPDWORD>(cb_value_data_max), reinterpret_cast<LPDWORD>(cb_security_descriptor_max), time_last_write);
    }

    static ws_long_t reg_enum_key(      hkey_type       hkey,
                                        ws_dword_t      index,
                                        char_type       *key_name,
                                        size_type       *cch_key_name,
                                        char_type       *key_class,
                                        size_type       *cch_key_class,
                                        time_type       *time_last_write)
    {
        return ::RegEnumKeyExA(hkey, index, key_name, reinterpret_cast<LPDWORD>(cch_key_name), NULL, key_class, reinterpret_cast<LPDWORD>(cch_key_class), time_last_write);
    }

    static ws_long_t reg_enum_value(    hkey_type       hkey,
                                        ws_dword_t      index,
                                        char_type       *value_name,
                                        size_type       *cch_value_name,
                                        ws_dword_t      *value_type,
                                        void            *data,
                                        size_type       &cb_data)
    {
        return ::RegEnumValueA(hkey, index, value_name, reinterpret_cast<LPDWORD>(cch_value_name), NULL, value_type, reinterpret_cast<LPBYTE>(data), reinterpret_cast<LPDWORD>(&cb_data));
    }
};

STLSOFT_TEMPLATE_SPECIALISATION
struct reg_traits<ws_char_w_t>
{
public:
    typedef ws_char_w_t         char_type;
    typedef ws_size_t           size_type;
    typedef ws_ptrdiff_t        difference_type;
    typedef HKEY                hkey_type;
    typedef reg_string_w_t      string_type;
    typedef FILETIME            time_type;

public:
    static char_type *str_copy(char_type *dest, char_type const *src)
    {
        return lstrcpyW(dest, src);
    }

    static char_type *str_cat(char_type *dest, char_type const *src)
    {
        return lstrcatW(dest, src);
    }

    static ws_int_t str_compare(char_type const *s1, char_type const *s2)
    {
        return lstrcmpW(s1, s2);
    }

    static size_type str_len(char_type const *src)
    {
        return static_cast<size_type>(lstrlenW(src));
    }

    static char_type *ensure_subkey_end(char_type *path)
    {
        char_type   *end;

        for(end = path; *end != L'\0'; ++end)
        {}

        if( path < end &&
            *(end - 1) != L'\\')
        {
            *end        =   L'\\';
            *(end + 1)  =   L'\0';
        }

        return path;
    }

    static size_type expand_env_strings(char_type const *src, char_type *dest, size_type cch_dest)
    {
        return static_cast<size_type>(::ExpandEnvironmentStringsW(src, dest, cch_dest));
    }

    static hkey_type key_dup(hkey_type hkey, REGSAM samDesired = KEY_ALL_ACCESS)
    {
        hkey_type   hkeyDup;
        ws_long_t   res = ::RegOpenKeyExW(hkey, L"", 0, samDesired, &hkeyDup);

        if(res != ERROR_SUCCESS)
        {
            hkeyDup = NULL;
        }

        return hkeyDup;
    }

    static ws_long_t reg_open_key(hkey_type hkey, char_type const *sub_key_name, hkey_type *hkey_result, REGSAM samDesired = KEY_ALL_ACCESS)
    {
        return ::RegOpenKeyExW(hkey, sub_key_name, 0, samDesired, hkey_result);
    }

    static ws_long_t reg_create_key(hkey_type hkey, char_type const *sub_key_name, hkey_type *hkey_result, REGSAM samDesired = KEY_ALL_ACCESS)
    {
        return ::RegCreateKeyExW(hkey, sub_key_name, 0, NULL, 0, samDesired, NULL, hkey_result, NULL);
    }

    static ws_long_t reg_create_key(hkey_type hkey, char_type const *sub_key_name, hkey_type *hkey_result, ws_bool_t &bCreated, REGSAM samDesired = KEY_ALL_ACCESS)
    {
        DWORD       disposition;
        ws_long_t   res =   ::RegCreateKeyExW(hkey, sub_key_name, 0, NULL, 0, samDesired, NULL, hkey_result, &disposition);

        bCreated = (ERROR_SUCCESS == res) && (REG_CREATED_NEW_KEY == disposition);

        return res;
    }

    static ws_long_t reg_query_value(hkey_type hkey, char_type const *value_name, ws_dword_t &value_type, void *data, size_type &cb_data)
    {
        return ::RegQueryValueExW(hkey, value_name, NULL, &value_type, static_cast<LPBYTE>(data), reinterpret_cast<LPDWORD>(&cb_data));
    }

    static ws_long_t reg_query_info(    hkey_type       hkey,
                                        char_type       *key_class,
                                        size_type       *cch_key_class,
                                        ws_uint_t       *c_sub_keys,
                                        size_type       *cch_sub_key_max,
                                        size_type       *cch_key_class_max,
                                        ws_uint_t       *c_values,
                                        size_type       *cch_value_name_max,
                                        size_type       *cb_value_data_max,
                                        size_type       *cb_security_descriptor_max,
                                        time_type       *time_last_write)
    {
        return ::RegQueryInfoKeyW(hkey, key_class, reinterpret_cast<LPDWORD>(cch_key_class), NULL, reinterpret_cast<LPDWORD>(c_sub_keys), reinterpret_cast<LPDWORD>(cch_sub_key_max), reinterpret_cast<LPDWORD>(cch_key_class_max), reinterpret_cast<LPDWORD>(c_values), reinterpret_cast<LPDWORD>(cch_value_name_max), reinterpret_cast<LPDWORD>(cb_value_data_max), reinterpret_cast<LPDWORD>(cb_security_descriptor_max), time_last_write);
    }

    static ws_long_t reg_enum_key(      hkey_type       hkey,
                                        ws_dword_t      index,
                                        char_type       *key_name,
                                        size_type       *cch_key_name,
                                        char_type       *key_class,
                                        size_type       *cch_key_class,
                                        time_type       *time_last_write)
    {
        return ::RegEnumKeyExW(hkey, index, key_name, reinterpret_cast<LPDWORD>(cch_key_name), NULL, key_class, reinterpret_cast<LPDWORD>(cch_key_class), time_last_write);
    }

    static ws_long_t reg_enum_value(    hkey_type       hkey,
                                        ws_dword_t      index,
                                        char_type       *value_name,
                                        size_type       *cch_value_name,
                                        ws_dword_t      *value_type,
                                        void            *data,
                                        size_type       &cb_data)
    {
        return ::RegEnumValueW(hkey, index, value_name, reinterpret_cast<LPDWORD>(cch_value_name), NULL, value_type, reinterpret_cast<LPBYTE>(data), reinterpret_cast<LPDWORD>(&cb_data));
    }
};

#endif /* __STLSOFT_DOCUMENTATION_SKIP_SECTION */

/* ////////////////////////////////////////////////////////////////////////// */

/// @} // end of group winstl_reg_library

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

#endif /* WINSTL_INCL_H_WINSTL_REG_TRAITS */

/* ////////////////////////////////////////////////////////////////////////// */
