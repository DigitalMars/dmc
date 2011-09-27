/* /////////////////////////////////////////////////////////////////////////////
 * File:        winstl_listview_sequence.h
 *
 * Purpose:     Contains the listview_sequence class.
 *
 * Created:     8th May 2003
 * Updated:     11th September 2004
 *
 * Home:        http://stlsoft.org/
 *
 * Copyright (c) 2003-2004, Matthew Wilson and Synesis Software
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


/// \file winstl_listview_sequence.h
///
/// Contains the listview_sequence class.

#ifndef WINSTL_INCL_H_WINSTL_LISTVIEW_SEQUENCE
#define WINSTL_INCL_H_WINSTL_LISTVIEW_SEQUENCE

#ifndef __STLSOFT_DOCUMENTATION_SKIP_SECTION
# define WINSTL_VER_H_WINSTL_LISTVIEW_SEQUENCE_MAJOR        2
# define WINSTL_VER_H_WINSTL_LISTVIEW_SEQUENCE_MINOR        0
# define WINSTL_VER_H_WINSTL_LISTVIEW_SEQUENCE_REVISION     1
# define WINSTL_VER_H_WINSTL_LISTVIEW_SEQUENCE_EDIT         23
#endif /* !__STLSOFT_DOCUMENTATION_SKIP_SECTION */

/* /////////////////////////////////////////////////////////////////////////////
 * Includes
 */

#ifndef WINSTL_INCL_H_WINSTL
# include "winstl.h"                        // Include the WinSTL root header
#endif /* !WINSTL_INCL_H_WINSTL */
#ifndef STLSOFT_INCL_H_STLSOFT_AUTO_DESTRUCTOR
# include "stlsoft_auto_buffer.h"           // Include the STLSoft auto_buffer
#endif /* STLSOFT_INCL_H_STLSOFT_AUTO_DESTRUCTOR */
#ifndef WINSTL_INCL_H_WINSTL_PROCESSHEAP_ALLOCATOR
# include "winstl_processheap_allocator.h"  // processheap_allocator
#endif /* WINSTL_INCL_H_WINSTL_PROCESSHEAP_ALLOCATOR */
#ifndef STLSOFT_INCL_H_STLSOFT_ITERATOR
# include "stlsoft_iterator.h"
#endif /* !STLSOFT_INCL_H_STLSOFT_ITERATOR */
#ifndef WINSTL_INCL_H_WINSTL_COMMCTRL_FUNCTIONS
# include "winstl_commctrl_functions.h"     //
#endif /* !WINSTL_INCL_H_WINSTL_COMMCTRL_FUNCTIONS */

#include <stlsoft_simple_string.h>
#ifdef __STLSOFT_CF_TEMPLATE_CLASS_DEFAULT_CLASS_ARGUMENT_SUPPORT
typedef stlsoft_ns_qual(basic_simple_string)<TCHAR> lvs_string_t;
#else
typedef stlsoft_ns_qual(basic_simple_string)<   TCHAR
                                            ,   stlsoft_ns_qual(stlsoft_char_traits)<TCHAR>
                                            ,   winstl_ns_qual(processheap_allocator)<TCHAR>
                                            >       lvs_string_t;
#endif /* __STLSOFT_CF_TEMPLATE_CLASS_DEFAULT_CLASS_ARGUMENT_SUPPORT */


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

/// \weakgroup libraries_control Control Library
/// \ingroup libraries
/// \brief This library provides facilities for defining and manipulating GUI controls

/// \defgroup winstl_control_library Control Library (WinSTL)
/// \ingroup WinSTL libraries_control
/// \brief This library provides facilities for defining and manipulating Win32 GUI controls
/// @{

/* /////////////////////////////////////////////////////////////////////////////
 * Classes
 */

/// Item class used by the listview_sequence class
class listview_sequence_item
{
    typedef stlsoft_ns_qual(auto_buffer)<TCHAR, processheap_allocator<TCHAR>, 256>   buffer_t;

public:
    listview_sequence_item(HWND hwndListView, int iIndex)
        : m_hwndListView(hwndListView)
        , m_index(iIndex)
    {}

public:
    lvs_string_t text(int iSubItem = 0) const
    {
        ws_size_t   cb  =   buffer_t::internal_size();
        LV_ITEM     item;

        item.mask       =   LVIF_TEXT;
        item.iItem      =   m_index;
        item.iSubItem   =   iSubItem;

        for(;; cb += buffer_t::internal_size())
        {
            buffer_t    buffer(cb);

            item.cchTextMax =   cb;
            item.pszText    =   buffer;

            if(!ListView_GetItem(m_hwndListView, &item))
            {
                item.cchTextMax = 0;
                break;
            }
            else
            {
                ss_size_t len =   static_cast<ss_size_t>(lstrlen(item.pszText));

                if(len + 1 < cb)
                {
                    return lvs_string_t(item.pszText, len);
                }
            }
        }

        return lvs_string_t();
    }

    int             index() const
    {
        return m_index;
    }

    int             image() const;
    int             selected_image() const;
    ws_dword_t      data() const
    {
        LV_ITEM item;

        item.mask       =   LVIF_PARAM;
        item.iItem      =   m_index;
        item.iSubItem   =   0;

        return ListView_GetItem(m_hwndListView, &item) ? item.lParam : 0;
    }

private:
    HWND    m_hwndListView;
    int     m_index;
};


/// Provides an STL-like sequence over the contents of a Windows List-view (<code>"SysListView32"</code>)
class listview_sequence
{
public:
    typedef listview_sequence_item  value_type;

public:
    ss_explicit_k listview_sequence(HWND hwndListView)
        : m_hwndListView(hwndListView)
    {}

public:
    /// const_iterator for the listview_sequence
    class const_iterator
    {
        typedef const_iterator  class_type;

    public:
        const_iterator()
            : m_hwndListView(NULL)
            , m_index(-1)
        {}
        const_iterator(HWND hwndListView, int iIndex)
            : m_hwndListView(hwndListView)
            , m_index(iIndex)
        {}
        const_iterator(class_type const &rhs)
            : m_hwndListView(rhs.m_hwndListView)
            , m_index(rhs.m_index)
        {}

        class_type &operator =(class_type const &rhs)
        {
            m_hwndListView  =   rhs.m_hwndListView;
            m_index         =   rhs.m_index;

            return *this;
        }

    public:
        listview_sequence_item operator *() const
        {
            return listview_sequence_item(m_hwndListView, m_index);
        }

        bool operator ==(class_type const &rhs) const
        {
            stlsoft_message_assert("Comparing iterators from different listview_sequence instances!", m_hwndListView == rhs.m_hwndListView);

            return m_index == rhs.m_index;
        }

        bool operator !=(class_type const &rhs) const
        {
            stlsoft_message_assert("Comparing iterators from different listview_sequence instances!", m_hwndListView == rhs.m_hwndListView);

            return m_index != rhs.m_index;
        }

        class_type &operator ++()
        {
            int cItems  =   ListView_GetItemCount(m_hwndListView);

            if(m_index < cItems)
            {
                ++m_index;
            }

            return *this;
        }

    private:
        HWND    m_hwndListView;
        int     m_index;
    };

public:
    const_iterator  begin() const
    {
        return const_iterator(m_hwndListView, 0);
    }
    const_iterator  end() const
    {
        return const_iterator(m_hwndListView, ListView_GetItemCount(m_hwndListView));
    }

private:
    HWND    m_hwndListView;
};

/* ////////////////////////////////////////////////////////////////////////// */

/// @} // end of group winstl_control_library

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

#endif /* WINSTL_INCL_H_WINSTL_LISTVIEW_SEQUENCE */

/* ////////////////////////////////////////////////////////////////////////// */
