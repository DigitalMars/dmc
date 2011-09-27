/* ////////////////////////////////////////////////////////////////////////////
 * File:        winstl_environment_sequence.h
 *
 * Purpose:     basic_environment_sequence class.
 *
 * Created:     31st December 2002
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


/// \file winstl_environment_sequence.h
///
/// basic_environment_sequence class.

#ifndef WINSTL_INCL_H_WINSTL_ENVIRONMENT_SEQUENCE
#define WINSTL_INCL_H_WINSTL_ENVIRONMENT_SEQUENCE

#ifndef __STLSOFT_DOCUMENTATION_SKIP_SECTION
# define WINSTL_VER_H_WINSTL_ENVIRONMENT_SEQUENCE_MAJOR     2
# define WINSTL_VER_H_WINSTL_ENVIRONMENT_SEQUENCE_MINOR     0
# define WINSTL_VER_H_WINSTL_ENVIRONMENT_SEQUENCE_REVISION  1
# define WINSTL_VER_H_WINSTL_ENVIRONMENT_SEQUENCE_EDIT      28
#endif /* !__STLSOFT_DOCUMENTATION_SKIP_SECTION */

/* ////////////////////////////////////////////////////////////////////////////
 * Includes
 */

#ifndef WINSTL_INCL_H_WINSTL
# include "winstl.h"                        // Include the WinSTL root header
#endif /* !WINSTL_INCL_H_WINSTL */
#ifndef STLSOFT_INCL_H_STLSOFT_ITERATOR
# include "stlsoft_iterator.h"              // const_reverse_iterator_base
#endif /* !STLSOFT_INCL_H_STLSOFT_ITERATOR */
#ifndef STLSOFT_INCL_H_STLSOFT_AUTO_BUFFER
# include "stlsoft_auto_buffer.h"           // stlsoft::auto_buffer
#endif /* !STLSOFT_INCL_H_STLSOFT_AUTO_BUFFER */
#ifndef WINSTL_INCL_H_WINSTL_PROCESSHEAP_ALLOCATOR
# include "winstl_processheap_allocator.h"  // processheap_allocator
#endif /* WINSTL_INCL_H_WINSTL_PROCESSHEAP_ALLOCATOR */

#include <algorithm>

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

/* ////////////////////////////////////////////////////////////////////////// */

/// \weakgroup libraries STLSoft Libraries
/// \brief The individual libraries

/// \weakgroup libraries_system System Library
/// \ingroup libraries
/// \brief This library provides facilities for accessing system attributes

/// \defgroup winstl_system_library System Library (WinSTL)
/// \ingroup WinSTL libraries_system
/// \brief This library provides facilities for accessing Win32 system attributes
/// @{

/* /////////////////////////////////////////////////////////////////////////////
 * Classes
 */

/// STL-like sequence for the system environment variables
///
/// \param C The character type
template <ss_typename_param_k C>
class basic_environment_sequence
{
public:
    /// The character type
    typedef C                                                   char_type;
    /// Structure representing the items
    struct symbol
    {
        /// The symbol name
        char_type const *name;
        /// The symbol value
        char_type const *value;
    };
    /// The value type
    typedef symbol                                              value_type;
    /// The current parameterisation of the type
    typedef basic_environment_sequence<C>                       class_type;
    /// The non-mutable (const) pointer type
    typedef const value_type                                    *const_pointer;
    /// The non-mutable (const) reference type
    typedef const value_type                                    &const_reference;
    /// The size type
    typedef ws_size_t                                           size_type;
    /// The difference type
    typedef ws_ptrdiff_t                                        difference_type;
    /// The non-mutating (const) iterator type
    typedef
#if !defined(__STLSOFT_COMPILER_IS_BORLAND)
      ss_typename_type_k
#endif /* __STLSOFT_COMPILER_IS_BORLAND */
        stlsoft_ns_qual(pointer_iterator)   <   value_type
                                            ,   const_pointer
                                            ,   const_reference
                                            >::iterator_type    const_iterator;
#if defined(__STLSOFT_CF_BIDIRECTIONAL_ITERATOR_SUPPORT)
    /// The non-mutating (const) reverse iterator type
    typedef stlsoft_ns_qual(const_reverse_iterator_base)<   const_iterator
                                                        ,   value_type
                                                        ,   const_reference
                                                        ,   const_pointer
                                                        ,   difference_type
                                                        >       const_reverse_iterator;
#endif /* __STLSOFT_CF_BIDIRECTIONAL_ITERATOR_SUPPORT */

// Construction
public:
    /// Default constructor
    basic_environment_sequence();
    /// Destructor, which releases any resources acquired
    ~basic_environment_sequence() winstl_throw_0();

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

#if defined(__STLSOFT_CF_BIDIRECTIONAL_ITERATOR_SUPPORT)
    /// Begins the reverse iteration
    ///
    /// \return An iterator representing the start of the reverse sequence
    const_reverse_iterator  rbegin() const;
    /// Ends the reverse iteration
    ///
    /// \return An iterator representing the end of the reverse sequence
    const_reverse_iterator  rend() const;
#endif /* __STLSOFT_CF_BIDIRECTIONAL_ITERATOR_SUPPORT */

// Iteration
public:
    /// Returns the number of elements in the enviroment sequence
    size_type size() const;
    /// Indicates whether the enviroment sequence is empty
    ws_bool_t empty() const;

// Accessors
public:
    /// Returns the name-value item for the given index
    value_type operator [](size_type index) const;

// Implementation
private:
    typedef auto_buffer<char_type, processheap_allocator<char_type> >   environment_buffer_t;
    typedef auto_buffer<symbol, processheap_allocator<symbol> >         symbols_buffer_t;

    static ws_size_t    calc_items_(char_type const *p, char_type const **q);
    static void         prepare_items_(symbols_buffer_t &symbols, environment_buffer_t &environment, char_type *p, char_type *q);
private:
    static int      compare_symbol_(symbol const &lhs, symbol const &rhs);

public:
    /// Function object that compares environment symbols for the basic_environment_sequence class
    struct compare_symbol
    {
        /// Function call operator, which returns \c true if \c lhs is lexicographically less than \c rhs
        ws_bool_t operator ()(symbol const &lhs, symbol const &rhs)
        {
            return compare_symbol_(lhs, rhs) < 0;
        }
    };

#if defined(__STLSOFT_COMPILER_IS_DMC) || \
    defined(__STLSOFT_COMPILER_IS_MSVC)
    friend struct compare_symbol;
#endif /* __STLSOFT_COMPILER_IS_DMC || __STLSOFT_COMPILER_IS_MSVC */

private:
    static char_type const *get_environment_strings();
    static void free_environment_strings(char_type *);

private:
    C const                 *m_p;
    C const                 *m_q;
    symbols_buffer_t        m_symbols;
    environment_buffer_t    m_environment;

// Not to be implemented
private:
    basic_environment_sequence(class_type const &);
    class_type const &operator =(class_type const &);
};

/* /////////////////////////////////////////////////////////////////////////////
 * Typedefs for commonly encountered types
 */

/// Instantiation of the basic_environment_sequence template for the ANSI character type \c char
typedef basic_environment_sequence<ws_char_a_t>     environment_sequence_a;
/// Instantiation of the basic_environment_sequence template for the Unicode character type \c wchar_t
typedef basic_environment_sequence<ws_char_w_t>     environment_sequence_w;
/// Instantiation of the basic_environment_sequence template for the Win32 character type \c TCHAR
typedef basic_environment_sequence<TCHAR>           environment_sequence;

////////////////////////////////////////////////////////////////////////////////
// Implementation

#ifndef __STLSOFT_DOCUMENTATION_SKIP_SECTION

template <ss_typename_param_k C>
inline /* static */ ss_typename_type_k basic_environment_sequence<C>::size_type basic_environment_sequence<C>::calc_items_(ss_typename_type_k basic_environment_sequence<C>::char_type const *p, ss_typename_type_k basic_environment_sequence<C>::char_type const **q)
{
    size_type   c;

    for(c = 0;; ++p)
    {
        if(*p == 0)
        {
            ++c;

            if(*(p + 1) == 0)
            {
                *q = p + 1;
                break;
            }
        }
    }

    return c;
}


template <ss_typename_param_k C>
inline /* static */ void basic_environment_sequence<C>::prepare_items_(ss_typename_type_k basic_environment_sequence<C>::symbols_buffer_t &buffer, ss_typename_type_k basic_environment_sequence<C>::environment_buffer_t &environment, ss_typename_type_k basic_environment_sequence<C>::char_type *p, ss_typename_type_k basic_environment_sequence<C>::char_type *q)
{
    ss_typename_type_k environment_buffer_t::iterator   env_begin   =   environment.begin();
    ss_typename_type_k symbols_buffer_t::iterator       sym_begin   =   buffer.begin();
    char_type                                           *begin      =   p;
    char_type *const                                    end         =   q;
    char_type                                           *last_src   =   begin;
    char_type                                           *last_dest  =   env_begin;

    // This loop does two things in one go (for efficiency).
    //
    // Firstly, it copies the source (which is read-only) to the m_environment
    // buffer.
    //
    // Secondly, it processes the source, and adjusts the m_symbols and
    // m_environment contents accordingly.
    for(; begin != end;)
    {
        *env_begin = *begin;

        if(*begin == 0)
        {
            sym_begin->name  =   last_dest;
            for(; last_src != begin; ++last_src, ++last_dest)
            {
                if(*last_src == '=')
                {
                    *last_dest = '\0';
                    ++last_src;
                    ++last_dest;
                    break;
                }
            }
            sym_begin->value    =   last_dest;
            last_src            =   ++begin;
            last_dest           =   ++env_begin;

            ++sym_begin;
        }
        else
        {
            ++begin;
            ++env_begin;
        }
    }

    winstl_ns_qual_std(sort)(buffer.begin(), buffer.end(), compare_symbol());
}

template <>
inline /* static */ basic_environment_sequence<ws_char_a_t>::char_type const *basic_environment_sequence<ws_char_a_t>::get_environment_strings()
{
    return static_cast<ws_char_a_t const *>(::GetEnvironmentStringsA());
}

template <>
inline /* static */ basic_environment_sequence<ws_char_w_t>::char_type const *basic_environment_sequence<ws_char_w_t>::get_environment_strings()
{
    return static_cast<ws_char_w_t const *>(::GetEnvironmentStringsW());
}

template <>
inline /* static */ void basic_environment_sequence<ws_char_a_t>::free_environment_strings(basic_environment_sequence<ws_char_a_t>::char_type *s)
{
    ::FreeEnvironmentStringsA(s);
}

template <>
inline /* static */ void basic_environment_sequence<ws_char_w_t>::free_environment_strings(basic_environment_sequence<ws_char_w_t>::char_type *s)
{
    ::FreeEnvironmentStringsW(s);
}

template <>
inline /* static */ ws_int_t basic_environment_sequence<ws_char_a_t>::compare_symbol_(basic_environment_sequence<ws_char_a_t>::symbol const &lhs, basic_environment_sequence<ws_char_a_t>::symbol const &rhs)
{
    return lstrcmpiA(lhs.name, rhs.name);
}

template <>
inline /* static */ ws_int_t basic_environment_sequence<ws_char_w_t>::compare_symbol_(basic_environment_sequence<ws_char_w_t>::symbol const &lhs, basic_environment_sequence<ws_char_w_t>::symbol const &rhs)
{
    return lstrcmpiW(lhs.name, rhs.name);
}


template <ss_typename_param_k C>
inline basic_environment_sequence<C>::basic_environment_sequence()
    : m_p(get_environment_strings())
    , m_symbols(calc_items_(m_p, &m_q))
    , m_environment(m_q - m_p)
{
    prepare_items_(m_symbols, m_environment, const_cast<char_type*>(m_p), const_cast<char_type*>(m_q));
}

template <ss_typename_param_k C>
inline basic_environment_sequence<C>::~basic_environment_sequence() winstl_throw_0()
{
    // The documentation for FreeEnvironmentStrings does not explicitly state
    // that it is legal to free a null string, so we must do the test.
    if(0 != m_p)
    {
        free_environment_strings(const_cast<char_type*>(m_p));
    }
}

template <ss_typename_param_k C>
inline ss_typename_type_k basic_environment_sequence<C>::const_iterator basic_environment_sequence<C>::begin() const
{
    return m_symbols.begin();
}

template <ss_typename_param_k C>
inline ss_typename_type_k basic_environment_sequence<C>::const_iterator basic_environment_sequence<C>::end() const
{
    return m_symbols.end();
}

#if defined(__STLSOFT_CF_BIDIRECTIONAL_ITERATOR_SUPPORT)
template <ss_typename_param_k C>
inline ss_typename_type_k basic_environment_sequence<C>::const_reverse_iterator basic_environment_sequence<C>::rbegin() const
{
    return const_reverse_iterator(end());
}

template <ss_typename_param_k C>
inline ss_typename_type_k basic_environment_sequence<C>::const_reverse_iterator basic_environment_sequence<C>::rend() const
{
    return const_reverse_iterator(begin());
}
#endif /* __STLSOFT_CF_BIDIRECTIONAL_ITERATOR_SUPPORT */


template <ss_typename_param_k C>
inline ss_typename_type_k basic_environment_sequence<C>::size_type basic_environment_sequence<C>::size() const
{
    return m_symbols.size();
}

template <ss_typename_param_k C>
inline ws_bool_t basic_environment_sequence<C>::empty() const
{
    return size() == 0;
}

template <ss_typename_param_k C>
inline ss_typename_type_k basic_environment_sequence<C>::value_type basic_environment_sequence<C>::operator [](ss_typename_type_k basic_environment_sequence<C>::size_type index) const
{
    winstl_message_assert("index access out of range in basic_environment_sequence", index < size() + 1);   // Has to be +1, since legitimate to take address of one-past-the-end

    return m_symbols.data()[index];
}

#endif /* !__STLSOFT_DOCUMENTATION_SKIP_SECTION */

////////////////////////////////////////////////////////////////////////////////
// Unit-testing

#ifdef STLSOFT_UNITTEST

namespace unittest
{
    ss_bool_t test_winstl_environment_sequence(unittest_reporter *r)
    {
        using stlsoft::unittest::unittest_initialiser;

        ss_bool_t               bSuccess    =   true;

        unittest_initialiser    init(r, "WinSTL", "environment_sequence", __FILE__);

#if 0
        if(<<TODO>>)
        {
            r->report("<<TODO>> failed ", __LINE__);
            bSuccess = false;
        }
#endif /* 0 */

        return bSuccess;
    }

    unittest_registrar    unittest_winstl_environment_sequence(test_winstl_environment_sequence);

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

#endif /* WINSTL_INCL_H_WINSTL_ENVIRONMENT_SEQUENCE */

/* ////////////////////////////////////////////////////////////////////////// */
