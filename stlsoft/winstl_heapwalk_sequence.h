/* /////////////////////////////////////////////////////////////////////////////
 * File:        winstl_heapwalk_sequence.h (formerly MWHeapWk.h; ::SynesisWin)
 *
 * Purpose:     Contains the heapwalk_sequence template class, and ANSI
 *              and Unicode specialisations thereof.
 *
 * Notes:       The original implementation of the class had the const_iterator
 *              and value_type as nested classes. Unfortunately, Visual C++ 5 &
 *              6 both had either compilation or linking problems so these are
 *              regretably now implemented as independent classes.
 *
 * Created:     15th January 2002
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


/// \file winstl_heapwalk_sequence.h
///
/// Contains the heapwalk_sequence template class, and ANSI and Unicode specialisations thereof.

#ifndef WINSTL_INCL_H_WINSTL_HEAPWALK_SEQUENCE
#define WINSTL_INCL_H_WINSTL_HEAPWALK_SEQUENCE

#ifndef __STLSOFT_DOCUMENTATION_SKIP_SECTION
# define WINSTL_VER_H_WINSTL_HEAPWALK_SEQUENCE_MAJOR        2
# define WINSTL_VER_H_WINSTL_HEAPWALK_SEQUENCE_MINOR        0
# define WINSTL_VER_H_WINSTL_HEAPWALK_SEQUENCE_REVISION     1
# define WINSTL_VER_H_WINSTL_HEAPWALK_SEQUENCE_EDIT         30
#endif /* !__STLSOFT_DOCUMENTATION_SKIP_SECTION */

/* /////////////////////////////////////////////////////////////////////////////
 * Includes
 */

#ifndef WINSTL_INCL_H_WINSTL
# include "winstl.h"                // Include the WinSTL root header
#endif /* !WINSTL_INCL_H_WINSTL */
#ifndef STLSOFT_INCL_H_STLSOFT_ITERATOR
# include "stlsoft_iterator.h"      // stlsoft::iterator_base
#endif /* !STLSOFT_INCL_H_STLSOFT_ITERATOR */

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
 * Forward declarations
 */

class heapwalk_sequence_const_iterator;

/* /////////////////////////////////////////////////////////////////////////////
 * Classes
 */

// class heapwalk_sequence
/// An STL-like sequence for walking heap allocated blocks
///
class heapwalk_sequence
{
    typedef PROCESS_HEAP_ENTRY                  entry_type;
public:
    /// The class
    typedef heapwalk_sequence                   class_type;
    /// The value type
    typedef entry_type const                    &value_type;
    /// The non-mutating (const) iterator type
    typedef heapwalk_sequence_const_iterator    const_iterator;
    /// The non-mutating (const) reference type
    typedef entry_type const                    &const_reference;

// Construction
public:
    /// Creates an instance that walks the default process heap
    heapwalk_sequence();
    /// Creates an instance that walks the given heap
    ///
    /// \note The heapwalk_sequence does <i>not</i> take ownership of the given heap handle
    ss_explicit_k heapwalk_sequence(HANDLE hHeap);
    /// Destructor
    ~heapwalk_sequence() winstl_throw_0();

// Iteration
public:
    /// Begins the iteration
    ///
    /// \return An iterator representing the start of the sequence
    const_iterator  begin() const;
    /// Ends the iteration
    ///
    /// \return An iterator representing the end of the sequence
    const_iterator  end() const;

private:
    friend class heapwalk_sequence_const_iterator;

    HANDLE  m_hHeap;

// Not to be implemented
private:
    heapwalk_sequence(class_type const &);
    const heapwalk_sequence &operator =(class_type const &);
};

// class heapwalk_sequence_const_iterator
/// Iterator for the heapwalk_sequence class
///
class heapwalk_sequence_const_iterator
    : public stlsoft_ns_qual(iterator_base) <   winstl_ns_qual_std(forward_iterator_tag)
                                            ,   PROCESS_HEAP_ENTRY const
                                            ,   ws_ptrdiff_t
                                            ,   PROCESS_HEAP_ENTRY const *
                                            ,   PROCESS_HEAP_ENTRY const &>
{
    typedef PROCESS_HEAP_ENTRY                  entry_type;
public:
    /// The value type
    typedef entry_type const                    value_type;
    /// The class
    typedef heapwalk_sequence_const_iterator    class_type;

private:
    heapwalk_sequence_const_iterator(const heapwalk_sequence &l, entry_type const &value)
        : m_list(&l)
        , m_value(value)
    {}
    heapwalk_sequence_const_iterator(const heapwalk_sequence &l);
public:
    /// Default constructor
    heapwalk_sequence_const_iterator();
    /// Copy constructor
    heapwalk_sequence_const_iterator(class_type const &rhs);
    /// Destructor
    ~heapwalk_sequence_const_iterator() winstl_throw_0();

    /// Copy asignment operator
    heapwalk_sequence_const_iterator &operator =(class_type const &rhs);

public:
    /// Pre-increment operator
    class_type &operator ++();
    /// Post-increment operator
    class_type const operator ++(int);
    /// Dereferences and returns a reference to the \c PROCESS_HEAP_ENTRY structure for the current position
    value_type &operator *() const;
    /// Evaluates whether \c this and \c rhs are equivalent
    ws_bool_t operator ==(class_type const &rhs) const;
    /// Evaluates whether \c this and \c rhs are not equivalent
    ws_bool_t operator !=(class_type const &rhs) const;

// Implementation
private:
    friend class heapwalk_sequence;

// Members
private:
    heapwalk_sequence const * const m_list;
    entry_type                      m_value;
};

///////////////////////////////////////////////////////////////////////////////

// heapwalk_sequence

inline heapwalk_sequence::heapwalk_sequence()
    : m_hHeap(::GetProcessHeap())
{}

inline heapwalk_sequence::heapwalk_sequence(HANDLE hHeap)
    : m_hHeap(hHeap)
{}

inline heapwalk_sequence::~heapwalk_sequence() winstl_throw_0()
{}

inline heapwalk_sequence::const_iterator heapwalk_sequence::begin() const
{
    entry_type  value;

    value.lpData    =   NULL;   // Want start of iteration

    if(::HeapWalk(m_hHeap, &value))
    {
        return const_iterator(*this, value);
    }
    else
    {
        return const_iterator(*this);
    }
}

inline heapwalk_sequence::const_iterator heapwalk_sequence::end() const
{
    return const_iterator(*this);
}

// heapwalk_sequence_const_iterator

inline heapwalk_sequence_const_iterator::heapwalk_sequence_const_iterator()
    : m_list(NULL)
{
    m_value.lpData  =   reinterpret_cast<LPVOID>(-1);
}

inline heapwalk_sequence_const_iterator::heapwalk_sequence_const_iterator(const heapwalk_sequence &l)
    : m_list(&l)
{
    m_value.lpData  =   reinterpret_cast<LPVOID>(-1);
}

inline heapwalk_sequence_const_iterator::heapwalk_sequence_const_iterator(class_type const &rhs)
    : m_list(rhs.m_list)
    , m_value(rhs.m_value)
{}

inline heapwalk_sequence_const_iterator::class_type &heapwalk_sequence_const_iterator::operator =(const heapwalk_sequence_const_iterator::class_type &rhs)
{
    winstl_assert(m_list == rhs.m_list);

    m_value =   rhs.m_value;

    return *this;
}

inline heapwalk_sequence_const_iterator::~heapwalk_sequence_const_iterator() winstl_throw_0()
{}

inline heapwalk_sequence_const_iterator::class_type &heapwalk_sequence_const_iterator::operator ++()
{
    if( NULL != m_list &&
        !::HeapWalk(m_list->m_hHeap, &m_value))
    {
        m_value.lpData  =   reinterpret_cast<LPVOID>(-1);
    }

    return *this;
}

inline heapwalk_sequence_const_iterator::class_type const heapwalk_sequence_const_iterator::operator ++(int)
{
    class_type  it(*this);

    operator ++();

    return it;
}

inline heapwalk_sequence_const_iterator::value_type &heapwalk_sequence_const_iterator::operator *() const
{
    return m_value;
}

inline ws_bool_t heapwalk_sequence_const_iterator::operator ==(class_type const &rhs) const
{
    ws_bool_t    eq;

    winstl_assert(m_list == rhs.m_list);    // Should only be comparing iterators from same container

    // Not equal if one but not both handles are not equal.
    if(m_value.lpData != rhs.m_value.lpData)
    {
        eq = ws_false_v;
    }
    else
    {
        eq = ws_true_v;
    }

    return eq;
}

inline ws_bool_t heapwalk_sequence_const_iterator::operator !=(class_type const &rhs) const
{
    return ! operator ==(rhs);
}

////////////////////////////////////////////////////////////////////////////////
// Unit-testing

#ifdef STLSOFT_UNITTEST

namespace unittest
{
    ss_bool_t test_winstl_heapwalk_sequence(unittest_reporter *r)
    {
        using stlsoft::unittest::unittest_initialiser;

        ss_bool_t               bSuccess    =   true;

        unittest_initialiser    init(r, "WinSTL", "heapwalk_sequence", __FILE__);

#if 0
        if(<<TODO>>)
        {
            r->report("<<TODO>> failed ", __LINE__);
            bSuccess = false;
        }
#endif /* 0 */

        return bSuccess;
    }

    unittest_registrar    unittest_winstl_heapwalk_sequence(test_winstl_heapwalk_sequence);

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

#endif /* WINSTL_INCL_H_WINSTL_HEAPWALK_SEQUENCE */

/* ////////////////////////////////////////////////////////////////////////// */
