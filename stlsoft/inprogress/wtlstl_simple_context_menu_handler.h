/* /////////////////////////////////////////////////////////////////////////////
 * File:        wtlstl_simple_context_menu_handler.h (formerly MECtxHnd.h; ::SynesisAtl)
 *
 * Purpose:     Contains the SimpleContextMenuHandler template class.
 *
 * Created:     15th January 2000
 * Updated:     9th September 2004
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


/// \file wtlstl_simple_context_menu_handler.h
///
/// Contains the SimpleContextMenuHandler template class.

#ifndef WTLSTL_INCL_H_WTLSTL_SIMPLE_CONTEXT_MENU_HANDLER
#define WTLSTL_INCL_H_WTLSTL_SIMPLE_CONTEXT_MENU_HANDLER

#ifndef __STLSOFT_DOCUMENTATION_SKIP_SECTION
# define WTLSTL_VER_H_WTLSTL_SIMPLE_CONTEXT_MENU_HANDLER_MAJOR      1
# define WTLSTL_VER_H_WTLSTL_SIMPLE_CONTEXT_MENU_HANDLER_MINOR      3
# define WTLSTL_VER_H_WTLSTL_SIMPLE_CONTEXT_MENU_HANDLER_REVISION   6
# define WTLSTL_VER_H_WTLSTL_SIMPLE_CONTEXT_MENU_HANDLER_EDIT       52
#endif /* !__STLSOFT_DOCUMENTATION_SKIP_SECTION */

/* /////////////////////////////////////////////////////////////////////////////
 * Includes
 */

#ifndef WTLSTL_INCL_H_WTLSTL
# include "wtlstl.h"                // Include the WTLSTL root header
#endif /* !WTLSTL_INCL_H_WTLSTL */

/* /////////////////////////////////////////////////////////////////////////////
 * Namespace
 */

#ifndef _WTLSTL_NO_NAMESPACE
# if defined(_STLSOFT_NO_NAMESPACE) || \
     defined(__STLSOFT_DOCUMENTATION_SKIP_SECTION)
/* There is no stlsoft namespace, so must define ::wtlstl */
namespace wtlstl
{
# else
/* Define stlsoft::wtlstl_project */

namespace stlsoft
{

namespace wtlstl_project
{

# endif /* _STLSOFT_NO_NAMESPACE */
#endif /* !_WTLSTL_NO_NAMESPACE */

/* /////////////////////////////////////////////////////////////////////////////
 * Macros
 */

template <ss_typename_param_k C>
struct SimpleContextMenuItem_
{
    typedef void (C::*PFN)();

    UINT    idsMenuItem;
    UINT    idsCommandString;
    PFN     pfn;
};

#define BEGIN_SIMPLE_CONTEXT_MENU_MAP() \
private: \
    typedef wtlstl_ns_qual(SimpleContextMenuItem_)<class_type> ContextMenuItem; \
public: /* Alas this has to be the case, but at least ContextMenuItem is not */ \
    static ContextMenuItem const *WINAPI _GetContextMenuEntries(ws_size_t *psize = NULL) \
    { \
        static ContextMenuItem const _entries[] = \
        {   \

#define SIMPLE_CONTEXT_MENU_ENTRY(idsm, idscs, pfn) \
            { idsm, idscs, pfn },

#define END_SIMPLE_CONTEXT_MENU_MAP() \
            { 0, 0, NULL } \
        }; \
        if(NULL != psize) \
        { \
            *psize = stlsoft_num_elements(_entries) - 1; \
        } \
        return _entries; \
    }


/* /////////////////////////////////////////////////////////////////////////////
 * Classes
 */

template <ss_typename_param_k C>
class SimpleContextMenuHandler
    : public IContextMenu
{
protected:
    typedef C                               boltee_type;
    typedef SimpleContextMenuHandler<C>     context_menu_handler_type;
private:
    typedef SimpleContextMenuItem_<C>       ContextMenuItem;

// IContextMenu
private:
    STDMETHOD(QueryContextMenu)(HMENU hmenu, UINT indexMenu, UINT idCmdFirst, UINT idCmdLast, UINT uFlags)
    {
        HRESULT hr;

        if(uFlags & CMF_DEFAULTONLY)
        {
            hr = S_OK;
        }
        else
        {
            ws_size_t               cItems;
            ContextMenuItem const   *items  =   boltee_type::_GetContextMenuEntries(&cItems);

            // Ensure we have enough room for all the menu items we want to present
            if(idCmdLast < idCmdFirst + cItems + 1)
            {
                hr = E_FAIL;
            }
            else
            {
                ws_size_t  index;

                // For each item, load the string and add to the menu.
                for(hr = S_OK, index = 0; index < cItems; ++index)
                {
                    TCHAR                   szMenuItem[256];
                    ContextMenuItem const   &item   = items[index];

                    if( 0 == ::LoadString(_Module.GetResourceInstance(), item.idsMenuItem, szMenuItem, stlsoft_num_elements(szMenuItem)) ||
                        !::InsertMenu(hmenu, indexMenu++, MF_STRING | MF_BYPOSITION, idCmdFirst++, szMenuItem))
                    {
                        hr = HRESULT_FROM_WIN32(::GetLastError());

                        break;
                    }
                    else
                    {
                        ATLTRACE(_T("Menu item: %d => %d, %d\n"), index, item.idsMenuItem, item.idsCommandString);
                    }
                }

                if(!FAILED(hr))
                {
                    // Note: return S_OK + # of items
                    hr = MAKE_HRESULT(SEVERITY_SUCCESS, FACILITY_NULL, index);
                }
            }
        }

        return hr;
    }

    STDMETHOD(InvokeCommand)(LPCMINVOKECOMMANDINFO lpici)
    {
        HRESULT hr;

        if(0 == HIWORD(lpici->lpVerb))
        {
            UINT                    cmdOffset   =   LOWORD(lpici->lpVerb);
            ws_size_t               cItems;
            ContextMenuItem const   *items      =   boltee_type::_GetContextMenuEntries(&cItems);

            if(cmdOffset < cItems)
            {
                // Run the handler for the item
                (static_cast<boltee_type*>(this)->*items[cmdOffset].pfn)();

                hr = S_OK;
            }
            else
            {
                _ASSERTE(!"Unexpected menu item invoked");
                hr = E_UNEXPECTED;
            }
        }
        else
        {
            // No need to support string based command.
            hr = E_INVALIDARG;
        }

        return hr;
    }

    STDMETHOD(GetCommandString)(UINT cmdOffset, UINT uType, UINT *pwReserved, LPSTR pszName, UINT cchMax)
    {
        HRESULT hr;

        if(pszName != NULL)
        {
            *pszName = 0;
        }

        typedef int (WINAPI *PfnLoadString)(HINSTANCE , UINT , LPVOID , int);

        PfnLoadString   fns[2] =
        {
                (PfnLoadString)LoadStringA
            ,   (PfnLoadString)LoadStringW
        };

        switch(uType)
        {
            default:
                _ASSERTE(!"Unrecognised GetCommandString() type");
            case    GCS_VERBA:
            case    GCS_VERBW:
                // Verbs are not supported currently
                hr = E_NOTIMPL;
                break;
            case    GCS_VALIDATEA:
            case    GCS_VALIDATEW:
                _ASSERTE(!"GCS_VALIDATE never expected by menu handlers");
                hr = E_UNEXPECTED;
                break;
            case    GCS_HELPTEXTA:
            case    GCS_HELPTEXTW:
                {
                    ws_size_t               cItems;
                    ContextMenuItem const   *items  =   boltee_type::_GetContextMenuEntries(&cItems);

                    if(cmdOffset < cItems)
                    {
                        ContextMenuItem const &item = items[cmdOffset];

                        if(fns[(uType & GCS_UNICODE) != 0]( _Module.GetResourceInstance(),
                                                            item.idsCommandString,
                                                            pszName,
                                                            cchMax) > 0)
                        {
                            ATLTRACE(_T("Menu item: %d => %d, %d\n"), cmdOffset, item.idsMenuItem, item.idsCommandString);

                            hr = S_OK;
                        }
                        else
                        {
                            hr = E_INVALIDARG;
                        }
                    }
                    else
                    {
                        hr = E_UNEXPECTED;
                    }
                }
                break;
        }

        return hr;
    }
};

/* ////////////////////////////////////////////////////////////////////////// */

#ifndef _WTLSTL_NO_NAMESPACE
# if defined(_STLSOFT_NO_NAMESPACE) || \
     defined(__STLSOFT_DOCUMENTATION_SKIP_SECTION)
} // namespace wtlstl
# else
} // namespace wtlstl_project
} // namespace stlsoft
# endif /* _STLSOFT_NO_NAMESPACE */
#endif /* !_WTLSTL_NO_NAMESPACE */

/* ////////////////////////////////////////////////////////////////////////// */

#endif /* WTLSTL_INCL_H_WTLSTL_SIMPLE_CONTEXT_MENU_HANDLER */

/* ////////////////////////////////////////////////////////////////////////// */
