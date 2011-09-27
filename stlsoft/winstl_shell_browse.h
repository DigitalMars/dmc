/* ////////////////////////////////////////////////////////////////////////////
 * File:        winstl_shell_browse.h
 *
 * Purpose:     shell_browsing class.
 *
 * Created:     2nd March 2002
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


/// \file winstl_shell_browse.h
///
/// shell_browsing class.

#ifndef WINSTL_INCL_H_WINSTL_SHELL_BROWSE
#define WINSTL_INCL_H_WINSTL_SHELL_BROWSE

#ifndef __STLSOFT_DOCUMENTATION_SKIP_SECTION
# define WINSTL_VER_H_WINSTL_SHELL_BROWSE_MAJOR       2
# define WINSTL_VER_H_WINSTL_SHELL_BROWSE_MINOR       0
# define WINSTL_VER_H_WINSTL_SHELL_BROWSE_REVISION    1
# define WINSTL_VER_H_WINSTL_SHELL_BROWSE_EDIT        31
#endif /* !__STLSOFT_DOCUMENTATION_SKIP_SECTION */

/* ////////////////////////////////////////////////////////////////////////////
 * Includes.
 */

#ifndef WINSTL_INCL_H_WINSTL
# include "winstl.h"                    // Include the WinSTL root header
#endif /* !WINSTL_INCL_H_WINSTL */
#ifndef WINSTL_INCL_H_WINSTL_SHELL_ALLOCATOR
# include "winstl_shell_allocator.h"    // shell_allocator
#endif /* !WINSTL_INCL_H_WINSTL_SHELL_ALLOCATOR */
#ifndef STLSOFT_INCL_H_STLSOFT_STRING_ACCESS
# include "stlsoft_string_access.h"     // stlsoft::c_str_ptr
#endif /* !STLSOFT_INCL_H_STLSOFT_STRING_ACCESS */
#ifndef WINSTL_INCL_H_WINSTL_STRING_ACCESS
# include "winstl_string_access.h"      // stlsoft::c_str_ptr
#endif /* !WINSTL_INCL_H_WINSTL_STRING_ACCESS */

/* ////////////////////////////////////////////////////////////////////////////
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

#if !defined(__STLSOFT_COMPILER_IS_MWERKS)
stlsoft_ns_using(c_str_ptr)
#endif /* compiler */

/* /////////////////////////////////////////////////////////////////////////////
 * Classes
 */

// struct shell_browse_traits

#ifdef __STLSOFT_DOCUMENTATION_SKIP_SECTION
/// Traits for accessing the correct browse information functions for a given character type
///
/// shell_browse_traits is a traits class for determining the correct browse
/// information structures and functions for a given character type.
///
/// \param C The character type
template <ss_typename_param_k C>
struct shell_browse_traits
{
    /// The browse-info type
    typedef BROWSEINFO     browseinfo_t;

    /// Browses for the folder according to the given information
    static LPITEMIDLIST browseforfolder(browseinfo_t *bi);
    /// Translates am ITEMIDLIST pointer to a path.
    ///
    /// \param pidl The item identifier list from which to elicit the path
    /// \param pszPath A non-null pointer to a buffer of at least _MAX_PATH length
    static BOOL getpathfromidlist(LPCITEMIDLIST pidl, ws_char_a_t *pszPath);
};

#else

// class shell_browse_traits
template <ss_typename_param_k C>
struct shell_browse_traits;

STLSOFT_TEMPLATE_SPECIALISATION
struct shell_browse_traits<ws_char_a_t>
{
public:
    typedef BROWSEINFOA     browseinfo_t;

    static LPITEMIDLIST browseforfolder(browseinfo_t *bi)
    {
        return SHBrowseForFolderA(bi);
    }

    static BOOL getpathfromidlist(LPCITEMIDLIST pidl, ws_char_a_t *pszPath)
    {
        return SHGetPathFromIDListA(pidl, pszPath);
    }
};

STLSOFT_TEMPLATE_SPECIALISATION
struct shell_browse_traits<ws_char_w_t>
{
public:
    typedef BROWSEINFOW     browseinfo_t;

    static LPITEMIDLIST browseforfolder(browseinfo_t *bi)
    {
        return SHBrowseForFolderW(bi);
    }

    static BOOL getpathfromidlist(LPCITEMIDLIST pidl, ws_char_w_t *pszPath)
    {
        return SHGetPathFromIDListW(pidl, pszPath);
    }
};

#endif /* !__STLSOFT_DOCUMENTATION_SKIP_SECTION */

/* /////////////////////////////////////////////////////////////////////////////
 * Functions
 */

// function shell_browse
/// Browses the shell namespace according to the given parameters
///
/// \param title The title for the browse dialog
/// \param displayName Buffer to receive the display name
/// \param flags Combination of the <b>BIF_*</b> flags for the Win32 \c SHBrowseForFolder() function
/// \param hwndOwner The parent of the browse dialog. May be null
/// \param pidlRoot Pointer to an ITEMIDLIST structure (PIDL) specifying the location of the root folder from which to start browsing. May be null
template<   ss_typename_param_k S
        ,   ss_typename_param_k C
        >
inline ws_bool_t shell_browse(const S &title, C *displayName, UINT flags, HWND hwndOwner, LPCITEMIDLIST pidlRoot)
{
    typedef shell_browse_traits<C>                  traits_type;
    ss_typename_type_k traits_type::browseinfo_t    browseinfo;
    LPITEMIDLIST                                    lpiidl;
    ws_bool_t                                       bRet    =   false;

    browseinfo.hwndOwner        =   hwndOwner;
    browseinfo.pidlRoot         =   pidlRoot;
    browseinfo.pszDisplayName   =   displayName;
    browseinfo.lpszTitle        =   c_str_ptr(title);
    browseinfo.ulFlags          =   flags;
    browseinfo.lpfn             =   0;
    browseinfo.lParam           =   0;

    lpiidl                      =   traits_type::browseforfolder(&browseinfo);

    if(lpiidl != 0)
    {
        if(traits_type::getpathfromidlist(lpiidl, displayName))
        {
            bRet = true;
        }

        shell_allocator<ITEMIDLIST>().deallocate(lpiidl);
    }

    if(!bRet)
    {
        displayName[0] = '\0';
    }

    return bRet;
}

/// Browses the shell namespace according to the given parameters
///
/// \param title The title for the browse dialog
/// \param displayName Buffer to receive the display name
/// \param flags Combination of the <b>BIF_*</b> flags for the Win32 \c SHBrowseForFolder() function
/// \param hwndOwner The parent of the browse dialog. May be null
template<   ss_typename_param_k S
        ,   ss_typename_param_k C
        >
inline ws_bool_t shell_browse(const S &title, C *displayName, UINT flags, HWND hwndOwner)
{
    return shell_browse(title, displayName, flags, hwndOwner, static_cast<LPCITEMIDLIST>(0));
}

/// Browses the shell namespace according to the given parameters
///
/// \param title The title for the browse dialog
/// \param displayName Buffer to receive the display name
/// \param flags Combination of the <b>BIF_*</b> flags for the Win32 \c SHBrowseForFolder() function
/// \param pidlRoot Pointer to an ITEMIDLIST structure (PIDL) specifying the location of the root folder from which to start browsing. May be null
template<   ss_typename_param_k S
        ,   ss_typename_param_k C
        >
inline ws_bool_t shell_browse(const S &title, C *displayName, UINT flags, LPCITEMIDLIST pidlRoot)
{
    return shell_browse(title, displayName, flags, 0, pidlRoot);
}

/// Browses the shell namespace according to the given parameters
///
/// \param title The title for the browse dialog
/// \param displayName Buffer to receive the display name
/// \param flags Combination of the <b>BIF_*</b> flags for the Win32 \c SHBrowseForFolder() function
template<   ss_typename_param_k S
        ,   ss_typename_param_k C
        >
inline ws_bool_t shell_browse(const S &title, C *displayName, UINT flags)
{
    return shell_browse(title, displayName, flags, 0, 0);
}

/// Browses the shell namespace according to the given parameters
///
/// \param title The title for the browse dialog
/// \param displayName Buffer to receive the display name
template<   ss_typename_param_k S
        ,   ss_typename_param_k C
        >
inline ws_bool_t shell_browse(const S &title, C *displayName)
{
    return shell_browse(title, displayName, 0, 0, 0);
}

////////////////////////////////////////////////////////////////////////////////
// Unit-testing

#ifdef STLSOFT_UNITTEST

namespace unittest
{
    ss_bool_t test_winstl_shell_browse(unittest_reporter *r)
    {
        using stlsoft::unittest::unittest_initialiser;

        ss_bool_t               bSuccess    =   true;

        unittest_initialiser    init(r, "WinSTL", "shell_browse", __FILE__);

#if 0
        if(<<TODO>>)
        {
            r->report("<<TODO>> failed ", __LINE__);
            bSuccess = false;
        }
#endif /* 0 */

        return bSuccess;
    }

    unittest_registrar    unittest_winstl_shell_browse(test_winstl_shell_browse);

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

#endif /* WINSTL_INCL_H_WINSTL_SHELL_BROWSE */

/* ////////////////////////////////////////////////////////////////////////// */
