/* /////////////////////////////////////////////////////////////////////////////
 * File:        winstl_treeview_sequence.h
 *
 * Purpose:     Contains the treeview sequence classes.
 *
 * Created:     1st December 2002
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


/// \file winstl_treeview_sequence.h
///
/// Contains the treeview sequence classes.

#ifndef WINSTL_INCL_H_WINSTL_TREEVIEW_SEQUENCE
#define WINSTL_INCL_H_WINSTL_TREEVIEW_SEQUENCE

#ifndef __STLSOFT_DOCUMENTATION_SKIP_SECTION
# define WINSTL_VER_H_WINSTL_TREEVIEW_SEQUENCE_MAJOR        2
# define WINSTL_VER_H_WINSTL_TREEVIEW_SEQUENCE_MINOR        0
# define WINSTL_VER_H_WINSTL_TREEVIEW_SEQUENCE_REVISION     1
# define WINSTL_VER_H_WINSTL_TREEVIEW_SEQUENCE_EDIT         33
#endif /* !__STLSOFT_DOCUMENTATION_SKIP_SECTION */

/* /////////////////////////////////////////////////////////////////////////////
 * Includes
 */

#ifndef MFCSTL_INCL_H_MFCSTL
# include "winstl.h"                        // Include the WinSTL root header
#endif /* !MFCSTL_INCL_H_MFCSTL */
#ifndef STLSOFT_INCL_H_STLSOFT_ITERATOR
# include "stlsoft_iterator.h"              // stlsoft::iterator_base
#endif /* !STLSOFT_INCL_H_STLSOFT_ITERATOR */
#ifndef WINSTL_INCL_H_WINSTL_COMMCTRL_FUNCTIONS
# include "winstl_commctrl_functions.h"     // treeview_getnext, etc.
#endif /* !WINSTL_INCL_H_WINSTL_COMMCTRL_FUNCTIONS */

/* ////////////////////////////////////////////////////////////////////////// */

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

/* ////////////////////////////////////////////////////////////////////////// */

/// Iterator for the treeview_child_sequence, treeview_peer_sequence and treeview_visible_sequence classes
///
/// \param N The windows message that is used to access the next element in the iteration sequence
/// \param P The windows message that is used to access the previous element in the iteration sequence
template <UINT N, UINT P>
class treeview_sequence_const_iterator
    : public stlsoft_ns_qual(iterator_base) <   winstl_ns_qual_std(bidirectional_iterator_tag)
                                            ,   HTREEITEM
                                            ,   ws_ptrdiff_t
                                            ,   void
                                            ,   HTREEITEM
                                            >
{
public:
    /// The current parameterisation of the type
    typedef treeview_sequence_const_iterator<N, P>  class_type;
    /// The value type
    typedef HTREEITEM                               value_type;
    /// The difference type
    typedef ws_ptrdiff_t                            difference_type;

private:
    friend class treeview_sequence_base;
    friend class treeview_visible_sequence;

    treeview_sequence_const_iterator(HWND hwndTree, HTREEITEM hitem);
public:
    /// Default constructor
    treeview_sequence_const_iterator();
    /// Copy constructor
    treeview_sequence_const_iterator(class_type const &rhs);

    /// Copy assignment operator
    treeview_sequence_const_iterator &operator =(class_type const &rhs);

public:
    /// Derefences and returns the current item
    HTREEITEM operator *() const;
    /// Pre-increment
    class_type &operator ++();
    /// Post-increment
    class_type operator ++(int);

    /// Evaluates whether \c this and \c rhs are equivalent
    ws_bool_t operator ==(class_type const &rhs) const;
    /// Evaluates whether \c this and \c rhs are not equivalent
    ws_bool_t operator !=(class_type const &rhs) const;

#if 0
    // Bidirectional support not yet available
    class_type &operator --();
#endif /* 0 */

// Members
private:
    HWND        m_hwnd;
    HTREEITEM   m_hitem;
};


/// Base class for the treeview_child_sequence and treeview_peer_sequence classes
class treeview_sequence_base
{
#if defined(__STLSOFT_COMPILER_IS_DMC) &&  \
    __DMC__ < 0x0840
public:
#else /* ? __DMC__ < 0x0840 */
protected:
#endif /* __DMC__ < 0x0840 */
    /// This class
    typedef treeview_sequence_base                                      class_type;
    /// The non-mutating (const) iterator type
    typedef treeview_sequence_const_iterator<TVGN_NEXT, TVGN_PREVIOUS>  const_iterator;
    /// The value type
    typedef const_iterator::value_type                                  value_type;
    /// The difference type
    typedef const_iterator::difference_type                             difference_type;

#if 0
    // Bidirectional support not yet available
#if defined(__STLSOFT_CF_BIDIRECTIONAL_ITERATOR_SUPPORT)
    /// The non-mutating (const) reverse iterator type
    typedef stlsoft_ns_qual(const_reverse_bidirectional_iterator_base)  <   const_iterator
                                                                        ,   value_type
                                                                        ,   value_type // Return by value!
                                                                        ,   value_type*
                                                                        ,   difference_type
                                                                        >    const_reverse_iterator;
#endif /* __STLSOFT_CF_BIDIRECTIONAL_ITERATOR_SUPPORT */
#endif /* 0 */

// Construction
protected:
    /// Constructs from the given tree and item
    treeview_sequence_base(HWND hwndTree, HTREEITEM hitem);

// Iteration
public:
    /// Begins the iteration
    ///
    /// \return An iterator representing the start of the sequence
    const_iterator begin() const;
    /// Ends the iteration
    ///
    /// \return An iterator representing the end of the sequence
    const_iterator end() const;

#if 0
    // Bidirectional support not yet available
#if defined(__STLSOFT_CF_BIDIRECTIONAL_ITERATOR_SUPPORT)
    /// Begins the reverse iteration
    ///
    /// \return An iterator representing the start of the reverse sequence
    const_reverse_iterator rbegin() const;
    /// Ends the reverse iteration
    ///
    /// \return An iterator representing the end of the reverse sequence
    const_reverse_iterator rend() const;
#endif /* __STLSOFT_CF_BIDIRECTIONAL_ITERATOR_SUPPORT */
#endif /* 0 */

// Members
private:
    HWND        m_hwnd;
    HTREEITEM   m_hitem;
};

// class treeview_child_sequence
/// Presents an STL-like sequence interface to the children of a given node in a tree-view
class treeview_child_sequence
    : public treeview_sequence_base
{
    typedef treeview_sequence_base                                      base_class_type;
public:
    /// This class
    typedef treeview_child_sequence                                     class_type;
    /// The non-mutating (const) iterator type
    typedef base_class_type::const_iterator                             const_iterator;
    /// The value type
    typedef base_class_type::value_type                                 value_type;
    /// The difference type
    typedef base_class_type::difference_type                            difference_type;
#if 0
    // Bidirectional support not yet available
#if defined(__STLSOFT_CF_BIDIRECTIONAL_ITERATOR_SUPPORT)
    typedef base_class_type::const_reverse_iterator                     const_reverse_iterator;
#endif /* __STLSOFT_CF_BIDIRECTIONAL_ITERATOR_SUPPORT */
#endif /* 0 */

// Construction
public:
    /// Create sequence of the children of \c hitem in the given tree
    treeview_child_sequence(HWND hwndTree, HTREEITEM hitem);
    /// Create sequence of the children of the root in the given tree
    ss_explicit_k treeview_child_sequence(HWND hwndTree);
};

// class treeview_peer_sequence
/// Presents an STL-like sequence interface to the peers of a given node in a tree-view
class treeview_peer_sequence
    : public treeview_sequence_base
{
    typedef treeview_sequence_base                                      base_class_type;
public:
    /// This class
    typedef treeview_child_sequence                                     class_type;
    /// The non-mutating (const) iterator type
    typedef base_class_type::const_iterator                             const_iterator;
    /// The value type
    typedef base_class_type::value_type                                 value_type;
    /// The difference type
    typedef base_class_type::difference_type                            difference_type;
#if 0
    // Bidirectional support not yet available
#if defined(__STLSOFT_CF_BIDIRECTIONAL_ITERATOR_SUPPORT)
    typedef base_class_type::const_reverse_iterator                     const_reverse_iterator;
#endif /* __STLSOFT_CF_BIDIRECTIONAL_ITERATOR_SUPPORT */
#endif /* 0 */

// Construction
public:
    /// Create sequence of the peers of \c hitem in the given tree
    treeview_peer_sequence(HWND hwndTree, HTREEITEM hitem);
};

// class treeview_visible_sequence
/// Presents an STL-like sequence interface to the visible items in a tree-view
class treeview_visible_sequence
{
public:
    /// This class
    typedef treeview_visible_sequence                                   class_type;
    /// The non-mutating (const) iterator type
    typedef treeview_sequence_const_iterator<   TVGN_NEXTVISIBLE
                                            ,   TVGN_PREVIOUSVISIBLE
                                            >                           const_iterator;
    /// The value type
    typedef const_iterator::value_type                                  value_type;
    /// The difference type
    typedef const_iterator::difference_type                             difference_type;

#if 0
    // Bidirectional support not yet available
#if defined(__STLSOFT_CF_BIDIRECTIONAL_ITERATOR_SUPPORT)
    /// The non-mutating (const) reverse iterator type
    typedef stlsoft_ns_qual(const_reverse_bidirectional_iterator_base)  <   const_iterator
                                                                        ,   value_type
                                                                        ,   value_type // Return by value!
                                                                        ,   value_type*
                                                                        ,   difference_type
                                                                        >    const_reverse_iterator;
#endif /* __STLSOFT_CF_BIDIRECTIONAL_ITERATOR_SUPPORT */
#endif /* 0 */

// Construction
public:
    /// Create sequence of the visible items in the given tree
    ss_explicit_k treeview_visible_sequence(HWND hwndTree);

// Iteration
public:
    /// Begins the iteration
    ///
    /// \return An iterator representing the start of the sequence
    const_iterator begin() const;
    /// Ends the iteration
    ///
    /// \return An iterator representing the end of the sequence
    const_iterator end() const;

#if 0
    // Bidirectional support not yet available
#if defined(__STLSOFT_CF_BIDIRECTIONAL_ITERATOR_SUPPORT)
    /// Begins the reverse iteration
    ///
    /// \return An iterator representing the start of the reverse sequence
    const_reverse_iterator rbegin() const;
    /// Ends the reverse iteration
    ///
    /// \return An iterator representing the end of the reverse sequence
    const_reverse_iterator rend() const;
#endif /* __STLSOFT_CF_BIDIRECTIONAL_ITERATOR_SUPPORT */
#endif /* 0 */

// Members
private:
    HWND        m_hwnd;
    HTREEITEM   m_hitem;
};

/* /////////////////////////////////////////////////////////////////////////////
 * Unit-testing
 */

#ifdef STLSOFT_UNITTEST

namespace unittest
{
    ss_bool_t test_winstl_treeview_sequence(unittest_reporter *r)
    {
        using stlsoft::unittest::unittest_initialiser;

        ss_bool_t               bSuccess    =   true;

        unittest_initialiser    init(r, "WinSTL", "treeview_sequence", __FILE__);

#if 0
        if(<<TODO>>)
        {
            r->report("<<TODO>> failed ", __LINE__);
            bSuccess = false;
        }
#endif /* 0 */

        return bSuccess;
    }

    unittest_registrar    unittest_winstl_treeview_sequence(test_winstl_treeview_sequence);

} // namespace unittest

#endif /* STLSOFT_UNITTEST */

/* /////////////////////////////////////////////////////////////////////////////
 * Implementation
 */

#ifndef __STLSOFT_DOCUMENTATION_SKIP_SECTION

// treeview_sequence_const_iterator

template <UINT N, UINT P>
inline treeview_sequence_const_iterator<N, P>::treeview_sequence_const_iterator(HWND hwndTree, HTREEITEM hitem)
    : m_hwnd(hwndTree)
    , m_hitem(hitem)
{}

template <UINT N, UINT P>
inline treeview_sequence_const_iterator<N, P>::treeview_sequence_const_iterator()
    : m_hwnd(NULL)
    , m_hitem(NULL)
{}

template <UINT N, UINT P>
inline treeview_sequence_const_iterator<N, P>::treeview_sequence_const_iterator(treeview_sequence_const_iterator<N, P> const &rhs)
    : m_hwnd(rhs.m_hwnd)
    , m_hitem(rhs.m_hitem)
{}

template <UINT N, UINT P>
inline treeview_sequence_const_iterator<N, P> &treeview_sequence_const_iterator<N, P>::operator =(treeview_sequence_const_iterator<N, P> const &rhs)
{
    m_hwnd = rhs.m_hwnd;
    m_hitem = rhs.m_hitem;

    return *this;
}

template <UINT N, UINT P>
inline HTREEITEM treeview_sequence_const_iterator<N, P>::operator *() const
{
    return m_hitem;
}

template <UINT N, UINT P>
inline ss_typename_type_k treeview_sequence_const_iterator<N, P>::class_type &treeview_sequence_const_iterator<N, P>::operator ++()
{
    if(m_hitem != NULL)
    {
        m_hitem = treeview_getnextitem(m_hwnd, m_hitem, N);
    }

    return *this;
}

template <UINT N, UINT P>
inline ss_typename_type_k treeview_sequence_const_iterator<N, P>::class_type treeview_sequence_const_iterator<N, P>::operator ++(int)
{
    class_type  ret(*this);

    operator ++();

    return ret;
}

template <UINT N, UINT P>
inline ws_bool_t treeview_sequence_const_iterator<N, P>::operator ==(treeview_sequence_const_iterator<N, P> const &rhs) const
{
    return m_hitem == rhs.m_hitem;
}

template <UINT N, UINT P>
inline ws_bool_t treeview_sequence_const_iterator<N, P>::operator !=(treeview_sequence_const_iterator<N, P> const &rhs) const
{
    return !operator ==(rhs);
}

// treeview_sequence_base

inline treeview_sequence_base::treeview_sequence_base(HWND hwndTree, HTREEITEM hitem)
    : m_hwnd(hwndTree)
    , m_hitem(hitem)
{}

inline treeview_sequence_base::const_iterator treeview_sequence_base::begin() const
{
    return const_iterator(m_hwnd, m_hitem);
}

inline treeview_sequence_base::const_iterator treeview_sequence_base::end() const
{
    return const_iterator();
}

// treeview_child_sequence

inline treeview_child_sequence::treeview_child_sequence(HWND hwndTree, HTREEITEM hitem)
    : base_class_type(hwndTree, treeview_getchilditem(hwndTree, hitem))
{}

inline treeview_child_sequence::treeview_child_sequence(HWND hwndTree)
    : base_class_type(hwndTree, treeview_getchilditem(hwndTree, treeview_getrootitem(hwndTree)))
{}

// treeview_peer_sequence

inline treeview_peer_sequence::treeview_peer_sequence(HWND hwndTree, HTREEITEM hitem)
    : base_class_type(hwndTree, hitem)
{}

// treeview_visible_sequence

inline treeview_visible_sequence::treeview_visible_sequence(HWND hwndTree)
    : m_hwnd(hwndTree)
    , m_hitem((HTREEITEM)::SendMessage(hwndTree, TVM_GETNEXTITEM, TVGN_FIRSTVISIBLE, 0))
{}

inline treeview_visible_sequence::const_iterator treeview_visible_sequence::begin() const
{
    return const_iterator(m_hwnd, m_hitem);
}

inline treeview_visible_sequence::const_iterator treeview_visible_sequence::end() const
{
    return const_iterator();
}

#endif /* !__STLSOFT_DOCUMENTATION_SKIP_SECTION */

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

#endif /* WINSTL_INCL_H_WINSTL_TREEVIEW_SEQUENCE */

/* ////////////////////////////////////////////////////////////////////////// */
