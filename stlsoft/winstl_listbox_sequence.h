/* /////////////////////////////////////////////////////////////////////////////
 * File:        winstl_listbox_sequence.h
 *
 * Purpose:     Contains the listbox_sequence class.
 *
 * Created:     10th November 2002
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


/// \file winstl_listbox_sequence.h
///
/// Contains the listbox_sequence class.

#ifndef WINSTL_INCL_H_WINSTL_LISTBOX_SEQUENCE
#define WINSTL_INCL_H_WINSTL_LISTBOX_SEQUENCE

#ifndef __STLSOFT_DOCUMENTATION_SKIP_SECTION
# define WINSTL_VER_H_WINSTL_LISTBOX_SEQUENCE_MAJOR     2
# define WINSTL_VER_H_WINSTL_LISTBOX_SEQUENCE_MINOR     0
# define WINSTL_VER_H_WINSTL_LISTBOX_SEQUENCE_REVISION  1
# define WINSTL_VER_H_WINSTL_LISTBOX_SEQUENCE_EDIT      26
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
#ifndef WINSTL_INCL_H_WINSTL_CONTROL_FUNCTIONS
# include "winstl_control_functions.h"      // listbox_addstring, etc.
#endif /* !WINSTL_INCL_H_WINSTL_CONTROL_FUNCTIONS */

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

/// Iterator for listbox_sequence class
///
/// This class acts as the iterator for the listbox_sequence class, and implements
/// the Random Access Iterator concept
///
/// \param S The string type
template <ss_typename_param_k S>
class listbox_sequence_const_iterator
    : public stlsoft_ns_qual(iterator_base) <   winstl_ns_qual_std(random_access_iterator_tag)
                                            ,   S
                                            ,   ws_ptrdiff_t
                                            ,   void * // Placate Borland C++ (to test, replace with void)
                                            ,   S
                                            >
{
public:
    /// The string type
    typedef S                                           value_type;
    /// The current parameterisation of the type
    typedef listbox_sequence_const_iterator<S>          class_type;
    /// The character type
    typedef TCHAR                                       char_type;
    /// The size type
    typedef ws_size_t                                   size_type;
    /// The difference type
    typedef ws_ptrdiff_t                                difference_type;
private:
    typedef processheap_allocator<char_type>            allocator_type;
    typedef auto_buffer<char_type, allocator_type, 256> buffer_type;

public:
    /// Construct an instance from the list-box \c hwndListBox at the given \c index
    listbox_sequence_const_iterator(HWND hwndListBox, int index)
        : m_hwnd(hwndListBox)
        , m_index(index)
    {}

public:
    /// Dereferences the iterator and returns the current value
    value_type const &operator *() const
    {
        int         len =   listbox_gettextlen(m_hwnd, m_index);
        buffer_type buffer(1 + len);

        listbox_gettext(m_hwnd, m_index, buffer);

        mutable_access(m_value).assign(buffer, buffer.size());

        return m_value;
    }

public:
    /// Pre-increment operator
    class_type &operator ++()
    {
        ++m_index;

        return *this;
    }

    /// Pre-increment operator
    class_type operator ++(int)
    {
        class_type  ret(*this);

        operator ++();

        return ret;
    }

    /// Compares \c this and \c rhs for equivalence
    difference_type compare(class_type const &rhs) const
    {
        winstl_message_assert("invalid comparison between iterators from different ranges", m_hwnd == rhs.m_hwnd);

        return m_index - rhs.m_index;
    }

    /// Indicates whether \c this and \c rhs are equivalent
    bool operator == (class_type const &rhs) const
    {
        return compare(rhs) == 0;
    }

    /// Indicates whether \c this and \c rhs are not equivalent
    bool operator != (class_type const &rhs) const
    {
        return compare(rhs) != 0;
    }

    // Bidirectional iterator operations

    /// Pre-decrement operator
    class_type &operator --()
    {
        --m_index;

        return *this;
    }

    /// Post-decrement operator
    class_type operator --(int)
    {
        class_type  ret(*this);

        operator --();

        return ret;
    }

    // Random access operations

    /// Offset
    class_type &operator +=(difference_type index)
    {
        m_index += index;

        return *this;
    }

    /// Offset
    class_type &operator -=(difference_type index)
    {
        m_index -= index;

        return *this;
    }

    /// Indexing operator
    value_type operator [](difference_type index) const
    {
        return get_value_at_(m_hwnd, index);
    }

    /// Calculate the distance between \c this and \c rhs
    difference_type distance(class_type const &rhs) const
    {
        return m_index - rhs.m_index;
    }

    /// Pointer subtraction
    class_type operator -(difference_type n) const
    {
        return class_type(*this) -= n;
    }

    /// Pointer addition
    class_type operator +(difference_type n) const
    {
        return class_type(*this) += n;
    }

    /// Pointer difference
    difference_type operator -(class_type const &rhs) const
    {
        return distance(rhs);
    }

    /// Indicates whether \c this is less than \c rhs
    ws_bool_t operator <(class_type const &rhs) const
    {
        return compare(rhs) < 0;
    }

    /// Indicates whether \c this is greater than \c rhs
    ws_bool_t operator >(class_type const &rhs) const
    {
        return compare(rhs) > 0;
    }

    /// Indicates whether \c this is less than or equal \c rhs
    ws_bool_t operator <=(class_type const &rhs) const
    {
        return compare(rhs) <= 0;
    }

    /// Indicates whether \c this is greater than or equal \c rhs
    ws_bool_t operator >=(class_type const &rhs) const
    {
        return compare(rhs) >= 0;
    }

// Implementation
private:
    static value_type get_value_at_(HWND hwnd, difference_type index)
    {
        int         len =   listbox_gettextlen(hwnd, index);
        buffer_type buffer(1 + len);

        listbox_gettext(hwnd, index, buffer);

        return value_type(buffer, buffer.size());
    }

// Members
private:
    HWND                    m_hwnd;
    int                     m_index;
    ss_mutable_k value_type m_value;
};

/// STL-like sequence for list-box contents
///
/// This class presents an STL-like sequence interface to a list-box
///
/// \param S The string type
template <ss_typename_param_k S>
class listbox_sequence
{
public:
    /// The current parameterisation of the type
    typedef listbox_sequence<S>                         class_type;
    /// The non-mutating (const) iterator type
    typedef listbox_sequence_const_iterator<S>          const_iterator;
    /// The character type
    typedef TCHAR                                       char_type;
    /// The value type
    typedef S                                           value_type;
    /// The size type
    typedef ws_size_t                                   size_type;
    /// The difference type
    typedef ws_ptrdiff_t                                difference_type;
#if defined(__STLSOFT_CF_BIDIRECTIONAL_ITERATOR_SUPPORT)
    /// The non-mutating (const) reverse iterator type
    typedef stlsoft_ns_qual(const_reverse_iterator_base)<   const_iterator
                                                        ,   value_type
                                                        ,   value_type // Return by value!
                                                        ,   void*
                                                        ,   difference_type
                                                        >   const_reverse_iterator;
#endif /* __STLSOFT_CF_BIDIRECTIONAL_ITERATOR_SUPPORT */

private:
    typedef processheap_allocator<char_type>            allocator_type;
    typedef auto_buffer<char_type, allocator_type, 256> buffer_type;

// Construction
public:
    /// Constructs from the given list-box
    ss_explicit_k listbox_sequence(HWND hwndListBox)
        : m_hwnd(hwndListBox)
    {}

// State
public:
    /// Returns the number of elements in the list-box
    size_type size() const
    {
        return static_cast<size_type>(listbox_getcount(m_hwnd));
    }
    /// Indicates whether the list-box is empty
    ws_bool_t empty() const
    {
        return size() == 0;
    }
    /// Returns the maximum number of items that the list-box can contain
    static size_type max_size()
    {
        return static_cast<size_type>(-1) / sizeof(LPCTSTR);
    }

// Iteration
public:
    /// Begins the iteration
    ///
    /// \return An iterator representing the start of the sequence
    const_iterator  begin() const
    {
        return const_iterator(m_hwnd, 0);
    }
    /// Ends the iteration
    ///
    /// \return An iterator representing the end of the sequence
    const_iterator  end() const
    {
        return const_iterator(m_hwnd, static_cast<int>(listbox_getcount(m_hwnd)));
    }

#if defined(__STLSOFT_CF_BIDIRECTIONAL_ITERATOR_SUPPORT)
    /// Begins the reverse iteration
    ///
    /// \return An iterator representing the start of the reverse sequence
    const_reverse_iterator rbegin() const
    {
        return const_reverse_iterator(end());
    }
    /// Ends the reverse iteration
    ///
    /// \return An iterator representing the end of the reverse sequence
    const_reverse_iterator rend() const
    {
        return const_reverse_iterator(begin());
    }
#endif /* __STLSOFT_CF_BIDIRECTIONAL_ITERATOR_SUPPORT */

// Accessors
public:
    /// Returns the item at the given index
    value_type operator [](difference_type index) const
    {
        int         len =   listbox_gettextlen(m_hwnd, index);
        buffer_type buffer(1 + len);

        listbox_gettext(m_hwnd, index, buffer);

        return value_type(buffer, buffer.size());
    }

// Members
private:
    HWND    m_hwnd;
};

////////////////////////////////////////////////////////////////////////////////
// Unit-testing

#ifdef STLSOFT_UNITTEST

namespace unittest
{
    ss_bool_t test_winstl_listbox_sequence(unittest_reporter *r)
    {
        using stlsoft::unittest::unittest_initialiser;

        ss_bool_t               bSuccess    =   true;

        unittest_initialiser    init(r, "WinSTL", "listbox_sequence", __FILE__);

#if 0
        if(<<TODO>>)
        {
            r->report("<<TODO>> failed ", __LINE__);
            bSuccess = false;
        }
#endif /* 0 */

        return bSuccess;
    }

    unittest_registrar    unittest_winstl_listbox_sequence(test_winstl_listbox_sequence);

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

#endif /* WINSTL_INCL_H_WINSTL_LISTBOX_SEQUENCE */

/* ////////////////////////////////////////////////////////////////////////// */
