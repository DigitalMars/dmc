/* /////////////////////////////////////////////////////////////////////////////
 * File:        winstl_windows_ident.h
 *
 * Purpose:     Windows identification.
 *
 * Created:     11th March 2004
 * Updated:     11th September 2004
 *
 * Home:        http://stlsoft.org/
 *
 * Copyright (c) 2004, Matthew Wilson and Synesis Software
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


/// \file winstl_windows_ident.h
///
/// Windows identification.

#ifndef WINSTL_INCL_H_WINSTL_WINDOWS_IDENT
#define WINSTL_INCL_H_WINSTL_WINDOWS_IDENT

#ifndef __STLSOFT_DOCUMENTATION_SKIP_SECTION
# define WINSTL_VER_H_WINSTL_WINDOWS_IDENT_MAJOR        2
# define WINSTL_VER_H_WINSTL_WINDOWS_IDENT_MINOR        0
# define WINSTL_VER_H_WINSTL_WINDOWS_IDENT_REVISION     1
# define WINSTL_VER_H_WINSTL_WINDOWS_IDENT_EDIT         16
#endif /* !__STLSOFT_DOCUMENTATION_SKIP_SECTION */

/* /////////////////////////////////////////////////////////////////////////////
 * Includes
 */

#ifndef WINSTL_INCL_H_WINSTL
# include "winstl.h"                        // Include the WinSTL root header
#endif /* !WINSTL_INCL_H_WINSTL */
#ifndef WINSTL_INCL_H_WINSTL_FUNCTIONS
//# include "winstl_functions.h"            //
#endif /* !WINSTL_INCL_H_WINSTL_FUNCTIONS */
#ifndef STLSOFT_INCL_H_STLSOFT_AUTO_BUFFER
# include "stlsoft_auto_buffer.h"           // auto_buffer
#endif /* !STLSOFT_INCL_H_STLSOFT_AUTO_BUFFER */
#ifndef WINSTL_INCL_H_PROCESSHEAP_ALLOCATOR
# include "winstl_processheap_allocator.h"  //
#endif /* !WINSTL_INCL_H_PROCESSHEAP_ALLOCATOR */

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
 * Enumerations
 */

enum WindowIdent
{
        Generic
    ,   ListBox         =   11
    ,   ComboBox        =   12
    ,   ListView        =   13
};

/* /////////////////////////////////////////////////////////////////////////////
 * Enumerations
 */

inline WindowIdent GetWindowIdent(HWND hwnd)
{
    typedef auto_buffer<char, processheap_allocator<char>, 256> buffer_t;

    struct Ident
    {
        WindowIdent ident;
        char const  *name;
    };

    WindowIdent         ident   =   Generic;
    buffer_t            buffer(buffer_t::internal_size() / 2);
    static const Ident  s_idents[] =
    {
            {   ListBox,    "LISTBOX"       }
        ,   {   ComboBox,   "COMBOBOX"      }
        ,   {   ListView,   "SysListView32" }
    };

    for(; buffer.resize(2 * buffer.size()); )
    {
        ws_size_t cch = static_cast<ws_size_t>(::GetClassNameA(hwnd, buffer, static_cast<int>(buffer.size())));

        if(cch < buffer.size())
        {
            break;
        }
    }

    for(ws_size_t index = 0; index < sizeof(s_idents) / sizeof(s_idents[0]); ++index)
    {
        if(0 == lstrcmpiA(s_idents[index].name, buffer))
        {
            ident = s_idents[index].ident;
            break;
        }
    }

    return ident;
}


////////////////////////////////////////////////////////////////////////////////
// Unit-testing

#ifdef STLSOFT_UNITTEST

namespace unittest
{
    ss_bool_t test_winstl_windows_ident(unittest_reporter *r)
    {
        using stlsoft::unittest::unittest_initialiser;

        ss_bool_t               bSuccess    =   true;

        unittest_initialiser    init(r, "WinSTL", "windows_ident", __FILE__);

        HWND    hwndListBox     =   ::CreateWindowA("LISTBOX", "", 0, 0, 0, 0, 0, NULL, NULL, NULL, NULL);
        HWND    hwndComboBox    =   ::CreateWindowA("COMBOBOX", "", 0, 0, 0, 0, 0, NULL, NULL, NULL, NULL);
        HWND    hwndListView    =   ::CreateWindowA("SysListView32", "", 0, 0, 0, 0, 0, NULL, NULL, NULL, NULL);

        if( NULL != hwndListBox &&
            GetWindowIdent(hwndListBox) != ListBox)
        {
            r->report("Identification of listbox failed ", __LINE__);
            bSuccess = false;
        }

        if( NULL != hwndComboBox &&
            GetWindowIdent(hwndComboBox) != ComboBox)
        {
            r->report("Identification of combobox failed ", __LINE__);
            bSuccess = false;
        }

        if( NULL != hwndListView &&
            GetWindowIdent(hwndListView) != ListView)
        {
            r->report("Identification of listview failed ", __LINE__);
            bSuccess = false;
        }

        return bSuccess;
    }

    unittest_registrar    unittest_winstl_windows_ident(test_winstl_windows_ident);

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

#endif /* WINSTL_INCL_H_WINSTL_WINDOWS_IDENT */

/* ////////////////////////////////////////////////////////////////////////// */
