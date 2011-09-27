/* /////////////////////////////////////////////////////////////////////////////
 * File:        winstl_commctrl_functionals.h
 *
 * Purpose:     Functionals for application to common controls.
 *
 * Created:     8th October 2002
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


/// \file winstl_commctrl_functionals.h
///
/// Functionals for application to common controls.

#ifndef WINSTL_INCL_H_WINSTL_COMMCTRL_FUNCTIONALS
#define WINSTL_INCL_H_WINSTL_COMMCTRL_FUNCTIONALS

#ifndef __STLSOFT_DOCUMENTATION_SKIP_SECTION
# define WINSTL_VER_H_WINSTL_COMMCTRL_FUNCTIONALS_MAJOR     2
# define WINSTL_VER_H_WINSTL_COMMCTRL_FUNCTIONALS_MINOR     0
# define WINSTL_VER_H_WINSTL_COMMCTRL_FUNCTIONALS_REVISION  1
# define WINSTL_VER_H_WINSTL_COMMCTRL_FUNCTIONALS_EDIT      36
#endif /* !__STLSOFT_DOCUMENTATION_SKIP_SECTION */

/* /////////////////////////////////////////////////////////////////////////////
 * Compatibility
 */

/*
[Incompatibilies-start]
__STLSOFT_COMPILER_IS_GCC: __GNUC__<3
[Incompatibilies-end]
 */

/* /////////////////////////////////////////////////////////////////////////////
 * Includes
 */

#ifndef WINSTL_INCL_H_WINSTL
# include "winstl.h"                // Include the WinSTL root header
#endif /* !WINSTL_INCL_H_WINSTL */

#if defined(__STLSOFT_COMPILER_IS_GCC) && \
    __GNUC__ < 3
# error winstl_commctrl_functionals.h is not compatible with GNU C++ prior to 3.0
#endif /* compiler */

#ifndef STLSOFT_INCL_H_STLSOFT_STRING_ACCESS
# include "stlsoft_string_access.h" // stlsoft::c_str_ptr
#endif /* !STLSOFT_INCL_H_STLSOFT_STRING_ACCESS */
#ifndef WINSTL_INCL_H_WINSTL_STRING_ACCESS
# include "winstl_string_access.h"  // winstl::c_str_ptr
#endif /* !WINSTL_INCL_H_WINSTL_STRING_ACCESS */
#include <commctrl.h>

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

#if !defined(__STLSOFT_COMPILER_IS_MWERKS)
stlsoft_ns_using(c_str_ptr)
#endif /* compiler */

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

// Common Controls



/// Function object used to insert items into list views
template <ws_bool_t BACK = true>
struct listview_inserter
{
    enum
    {
#ifdef I_IMAGENONE
        image_none  =   I_IMAGENONE
#else
        image_none  =   -2
#endif /* I_IMAGENONE */
    };

public:
    /// Construct with the target list-view window
    ss_explicit_k listview_inserter(HWND hwndListview, ws_int_t iImage = image_none)
        : m_hwndListview(hwndListview)
        , m_iImage(iImage)
    {}

#ifdef __STLSOFT_CF_MEMBER_TEMPLATE_FUNCTION_SUPPORT
    /// Function call operator taking the item string
    template <ss_typename_param_k S>
    ws_int_t operator ()(S const &s)
    {
        return insert_item(c_str_ptr(s));
    }
#endif // __STLSOFT_CF_MEMBER_TEMPLATE_FUNCTION_SUPPORT
    /// Function call operator taking the item string
    ws_int_t operator ()(const ws_char_a_t *s)
    {
        return insert_item(s);
    }
    /// Function call operator taking the item string
    ws_int_t operator ()(const ws_char_w_t *s)
    {
        return insert_item(s);
    }

private:
    ws_int_t    insert_index() const;

    ws_int_t insert_item(ws_char_a_t const *s)
    {
        LV_ITEMA    item;

        item.mask       =   LVIF_TEXT;
        item.pszText    =   const_cast<ws_char_a_t *>(s);
        item.iItem      =   insert_index();
        item.iSubItem   =   0;

        if(image_none != m_iImage)
        {
            item.mask |= LVIF_IMAGE;
            item.iImage = m_iImage;
        }

        return (int)::SendMessage(m_hwndListview, LVM_INSERTITEMA, 0, reinterpret_cast<LPARAM>(&item));
    }

    ws_int_t insert_item(ws_char_w_t const *s)
    {
        LV_ITEMW    item;

        item.mask       =   LVIF_TEXT;
        item.pszText    =   const_cast<ws_char_w_t *>(s);
        item.iItem      =   insert_index();
        item.iSubItem   =   0;

        if(image_none != m_iImage)
        {
            item.mask |= LVIF_IMAGE;
            item.iImage = m_iImage;
        }

        return (int)::SendMessage(m_hwndListview, LVM_INSERTITEMW, 0, reinterpret_cast<LPARAM>(&item));
    }

private:
    HWND        m_hwndListview;
    ws_int_t    m_iImage;
};

template <>
inline ws_int_t listview_inserter<false>::insert_index() const
{
    return 0;   // Front insertion
}

template <>
inline ws_int_t listview_inserter<true>::insert_index() const
{
    return static_cast<ws_int_t>(::SendMessage(m_hwndListview, LVM_GETITEMCOUNT, 0, 0L));   // Back insertion
}

/// Function object used to insert items at the front of list-views
typedef listview_inserter<false>    listview_front_inserter;
/// Function object used to insert items at the back of list-views
typedef listview_inserter<true>     listview_back_inserter;


/// Function object used to insert items into tree views
struct treeview_inserter
{
    enum
    {
#ifdef I_IMAGENONE
        image_none  =   I_IMAGENONE
#else
        image_none  =   -2
#endif /* I_IMAGENONE */
    };

public:
    typedef void        argument_type;
    typedef HTREEITEM   result_type;

public:
    ss_explicit_k treeview_inserter(HWND hwndTree, HTREEITEM hParent)
        : m_hwndTree(hwndTree)
        , m_hParent(hParent)
        , m_hInsertAfter(TVI_LAST)
        , m_iImage(image_none)
    {}
    ss_explicit_k treeview_inserter(HWND hwndTree, HTREEITEM hParent, HTREEITEM hInsertAfter)
        : m_hwndTree(hwndTree)
        , m_hParent(hParent)
        , m_hInsertAfter(hInsertAfter)
        , m_iImage(image_none)
    {}
    ss_explicit_k treeview_inserter(HWND hwndTree, HTREEITEM hParent, int iImage)
        : m_hwndTree(hwndTree)
        , m_hParent(hParent)
        , m_hInsertAfter(TVI_LAST)
        , m_iImage(iImage)
    {}

public:
    template <typename S>
    HTREEITEM operator () (S const &s)
    {
        return insert_item(stlsoft_ns_qual(c_str_ptr)(s));
    }

    HTREEITEM operator () (char const *s)
    {
        return insert_item(stlsoft_ns_qual(c_str_ptr)(s));
    }

    HTREEITEM operator () (wchar_t const *s)
    {
        return insert_item(stlsoft_ns_qual(c_str_ptr)(s));
    }

private:
    HTREEITEM insert_item(char const *s)
    {
        TV_INSERTSTRUCTA    tvis;

        tvis.hParent        =   m_hParent;
        tvis.hInsertAfter   =   m_hInsertAfter;

        tvis.item.mask      =   TVIF_TEXT;
        tvis.item.pszText   =   const_cast<char*>(s);

        if(image_none != m_iImage)
        {
            tvis.item.mask |= TVIF_IMAGE;
            tvis.item.iImage = m_iImage;
        }

        return (HTREEITEM)::SendMessage(m_hwndTree, TVM_INSERTITEMA, 0, (LPARAM)&tvis);
    }

    HTREEITEM insert_item(wchar_t const *s)
    {
        TV_INSERTSTRUCTW    tvis;

        tvis.hParent        =   m_hParent;
        tvis.hInsertAfter   =   m_hInsertAfter;

        tvis.item.mask      =   TVIF_TEXT;
        tvis.item.pszText   =   const_cast<wchar_t*>(s);

        if(image_none != m_iImage)
        {
            tvis.item.mask |= TVIF_IMAGE;
            tvis.item.iImage = m_iImage;
        }

        return (HTREEITEM)::SendMessage(m_hwndTree, TVM_INSERTITEMW, 0, (LPARAM)&tvis);
    }

private:
    HWND        m_hwndTree;
    HTREEITEM   m_hParent;
    HTREEITEM   m_hInsertAfter;
    int         m_iImage;
};

////////////////////////////////////////////////////////////////////////////////
// Unit-testing

#ifdef STLSOFT_UNITTEST

namespace unittest
{
    ss_bool_t test_winstl_commctrl_functionals(unittest_reporter *r)
    {
        using stlsoft::unittest::unittest_initialiser;

        ss_bool_t               bSuccess    =   true;

        unittest_initialiser    init(r, "WinSTL", "commctrl_functionals", __FILE__);

#if 0
        if(<<TODO>>)
        {
            r->report("<<TODO>> failed ", __LINE__);
            bSuccess = false;
        }
#endif /* 0 */

        return bSuccess;
    }

    unittest_registrar    unittest_winstl_commctrl_functionals(test_winstl_commctrl_functionals);

} // namespace unittest

#endif /* STLSOFT_UNITTEST */

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

#endif /* WINSTL_INCL_H_WINSTL_COMMCTRL_FUNCTIONALS */

/* ////////////////////////////////////////////////////////////////////////// */
