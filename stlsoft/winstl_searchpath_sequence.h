/* /////////////////////////////////////////////////////////////////////////////
 * File:        winstl_searchpath_sequence.h
 *
 * Purpose:     Contains the basic_searchpath_sequence template class, and ANSI
 *              and Unicode specialisations thereof.
 *
 * Created:     12th July 2002
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


/// \file winstl_searchpath_sequence.h
///
/// Contains the basic_searchpath_sequence template class, and ANSI and Unicode specialisations thereof.

#ifndef WINSTL_INCL_H_WINSTL_SEARCHPATH_SEQUENCE
#define WINSTL_INCL_H_WINSTL_SEARCHPATH_SEQUENCE

#ifndef __STLSOFT_DOCUMENTATION_SKIP_SECTION
# define WINSTL_VER_H_WINSTL_SEARCHPATH_SEQUENCE_MAJOR        2
# define WINSTL_VER_H_WINSTL_SEARCHPATH_SEQUENCE_MINOR        0
# define WINSTL_VER_H_WINSTL_SEARCHPATH_SEQUENCE_REVISION     1
# define WINSTL_VER_H_WINSTL_SEARCHPATH_SEQUENCE_EDIT         46
#endif /* !__STLSOFT_DOCUMENTATION_SKIP_SECTION */

/* /////////////////////////////////////////////////////////////////////////////
 * Includes
 */

#ifndef WINSTL_INCL_H_WINSTL
# include "winstl.h"                        // Include the WinSTL root header
#endif /* !WINSTL_INCL_H_WINSTL */
#ifndef WINSTL_INCL_H_WINSTL_FILESYSTEM_TRAITS
# include "winstl_filesystem_traits.h"  // filesystem_traits
#endif /* !WINSTL_INCL_H_WINSTL_FILESYSTEM_TRAITS */
#ifndef WINSTL_INCL_H_WINSTL_PROCESSHEAP_ALLOCATOR
# include "winstl_processheap_allocator.h"  // winstl::processheap_allocator
#endif /* !WINSTL_INCL_H_WINSTL_PROCESSHEAP_ALLOCATOR */
#ifndef WINSTL_INCL_H_WINSTL_SYSTEM_VERSION
# include "winstl_system_version.h"         // winstl::system_version
#endif /* !WINSTL_INCL_H_WINSTL_SYSTEM_VERSION */
#ifndef STLSOFT_INCL_H_STLSOFT_AUTO_BUFFER
# include "stlsoft_auto_buffer.h"           // stlsoft::auto_buffer
#endif /* !STLSOFT_INCL_H_STLSOFT_AUTO_BUFFER */
#ifndef STLSOFT_INCL_H_STLSOFT_ITERATOR
# include "stlsoft_iterator.h"          // stlsoft::iterator, stlsoft::reverse_iterator
#endif /* STLSOFT_INCL_H_STLSOFT_ITERATOR */

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

/// \weakgroup libraries_filesystem File-System Library
/// \ingroup libraries
/// \brief This library provides facilities for defining and manipulating file-system objects

/// \weakgroup winstl_filesystem_library File-System Library (WinSTL)
/// \ingroup WinSTL libraries_filesystem
/// \brief This library provides facilities for defining and manipulating file-system objects for the Win32 API
/// @{

/* /////////////////////////////////////////////////////////////////////////////
 * Classes
 */

// class basic_searchpath_sequence
/// Presents an STL-like sequence interface to the search path sequence for the current process
///
/// \param C The character type
/// \param T The traits type. On translators that support default template arguments this defaults to filesystem_traits<C>
template<   ss_typename_param_k C
#ifdef __STLSOFT_CF_TEMPLATE_CLASS_DEFAULT_CLASS_ARGUMENT_SUPPORT
        ,   ss_typename_param_k T = filesystem_traits<C>
#else
        ,   ss_typename_param_k T /* = filesystem_traits<C> */
#endif /* __STLSOFT_CF_TEMPLATE_CLASS_DEFAULT_CLASS_ARGUMENT_SUPPORT */
        >
class basic_searchpath_sequence
{
public:
    /// The character type
    typedef C                                                                       char_type;
    /// The traits type
    typedef T                                                                       traits_type;
    /// The current parameterisation of the type
    typedef basic_searchpath_sequence<C, T>                                         class_type;
    /// The value type
    typedef char_type const                                                         *value_type;
    /// The pointer type
    typedef value_type                                                              *pointer;
    /// The non-mutable (const) pointer type
    typedef const value_type                                                        *const_pointer;
    /// The reference type
    typedef value_type                                                              &reference;
    /// The non-mutable (const) reference type
    typedef const value_type                                                        &const_reference;
    /// The size type
    typedef ws_size_t                                                               size_type;
    /// The difference type
    typedef ws_ptrdiff_t                                                            difference_type;
    /// The non-mutating (const) iterator type
#if defined(__STLSOFT_COMPILER_IS_BORLAND)
    typedef                   stlsoft_ns_qual(pointer_iterator)<   value_type
#else
    typedef ss_typename_type_k stlsoft_ns_qual(pointer_iterator)<   value_type
#endif /* __STLSOFT_COMPILER_IS_BORLAND */
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
                                                        >                           const_reverse_iterator;
#endif /* __STLSOFT_CF_BIDIRECTIONAL_ITERATOR_SUPPORT */

// Construction
public:
    /// Determines the search path sequence for the calling process
    ///
    /// \note The process directory appears before the current directory
    basic_searchpath_sequence();
    /// Determines the search path sequence for the calling process
    ///
    /// \param bIncludeApplicationDirectory If this is \c true the application directory is included in the search path sequence
    /// \param bIncludeCurrentDirectory If this is \c true the current directory is included in the search path sequence
    /// \param bApplicationDirectoryFirst If this is \c true the process directory is placed before the current directory in the search (the normal loading sequence). If not, then the current directory comes first.
    basic_searchpath_sequence(ws_bool_t bIncludeApplicationDirectory, ws_bool_t bIncludeCurrentDirectory, ws_bool_t bApplicationDirectoryFirst = true);
    /// Releases any resources
    ~basic_searchpath_sequence() winstl_throw_0();

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

// State
public:
    /// Returns the number of items in the sequence
    size_type size() const;
    /// Indicates whether the sequence is empty
    ws_bool_t empty() const;
    /// Returns the maximum number of items in the sequence
    static size_type max_size();

// Accessors
public:
    /// Returns the item at the given index
    value_type operator [](size_type index) const;

// Members
private:
    // 1. Application directory - GetModuleFileName(NULL, ...);
    // 2. Current directory - GetCurrentDirectory
    // 3. System directory
    // 4. NT-only: 16-bit system directory
    // 5. Windows directory
    // 6 - n. Path directories

    typedef processheap_allocator<char_type>                                    main_allocator_type;
    typedef processheap_allocator<value_type>                                   value_allocator_type;
    typedef stlsoft_ns_qual(auto_buffer)<char_type, main_allocator_type, 1024>  main_buffer_type;
    typedef stlsoft_ns_qual(auto_buffer)<value_type, value_allocator_type, 24>  value_buffer_type;

    main_buffer_type    m_buffer;
    value_buffer_type   m_values;
    const_iterator      m_end;

// Implementation
private:
    void _construct(ws_bool_t bIncludeApplicationDirectory, ws_bool_t bIncludeCurrentDirectory, ws_bool_t bApplicationDirectoryFirst);

    /* WSCB: Borland has an internal compiler error if use ws_bool_t */
#ifdef __STLSOFT_COMPILER_IS_BORLAND
    typedef ws_int_t        init_type;
#else
    typedef ws_bool_t       init_type;
#endif /* __STLSOFT_COMPILER_IS_BORLAND */

    static char_type const  *get_application_directory()
    {
        static char_type    s_application_directory[_MAX_PATH + 1];
        static init_type    s_init = ws_false_v;

        if(!s_init)
        {
            char_type   dummy[_MAX_PATH + 1];
            char_type   *file_part;

            traits_type::get_module_filename(NULL, s_application_directory, winstl_num_elements(s_application_directory));
            traits_type::get_full_path_name(s_application_directory, winstl_num_elements(dummy), dummy, &file_part);
            s_application_directory[file_part - &dummy[0]] = '\0';
        }

        return s_application_directory;
    }

    static char_type const  *get_system_directory()
    {
        static char_type    s_system_directory[_MAX_PATH + 1];
        static init_type    s_init = (traits_type::get_system_directory(s_system_directory, winstl_num_elements(s_system_directory)), ws_true_v);

        return s_system_directory;
    }

    static char_type const  *get_windows_directory()
    {
        static char_type    s_windows_directory[_MAX_PATH + 1];
        static init_type    s_init = (traits_type::get_windows_directory(s_windows_directory, winstl_num_elements(s_windows_directory)), ws_true_v);

        return s_windows_directory;
    }

    static char_type const  *get_system16_directory()
    {
        static char_type    s_system16_directory[_MAX_PATH + 1];
        static init_type    s_init = ws_false_v;

        if(!s_init)
        {
            if(system_version::winnt())
            {
                char_type   *file_part;

                traits_type::get_full_path_name(get_system_directory(), winstl_num_elements(s_system16_directory), s_system16_directory, &file_part);
                traits_type::str_copy(file_part, _disgusting_hack("SYSTEM", L"SYSTEM"));
            }
            else
            {
                s_system16_directory[0] = '\0';
            }

            s_init = ws_true_v;
        }

        return s_system16_directory;
    }

    static ws_size_t    directories_total()
    {
        ws_size_t   cch =   0;

        cch += 1 + traits_type::str_len(get_application_directory());       // Application directory
        cch += 1 + traits_type::get_current_directory(0, NULL);             // Current directory
        cch += 1 + traits_type::str_len(get_system_directory());            // System directory
        cch += 1 + traits_type::str_len(get_system16_directory());          // 16-bit System directory
        cch += 1 + traits_type::str_len(get_windows_directory());           // Windows directory
        cch += 1 + traits_type::get_environment_variable(_disgusting_hack("PATH", L"PATH"), NULL, 0);  // PATH

        return cch;
    }

    static ws_size_t    num_paths()
    {
        ws_size_t           cPaths  =   0;
        ws_size_t           cch     =   traits_type::get_environment_variable(_disgusting_hack("PATH", L"PATH"), NULL, 0);
        main_buffer_type    buffer(1 + cch);
        char_type const     *begin  =   buffer;
        char_type const     *end    =   begin + cch;
        char_type const     *last;

        traits_type::get_environment_variable(_disgusting_hack("PATH", L"PATH"), buffer, buffer.size());

        for(; begin != end; ++begin)
        {
            if(*begin != ';')
            {
                break;
            }
        }

        for(last = begin; begin != end; ++begin)
        {
            if(*begin == ';')
            {
                if(1 < begin - last)
                {
                    ++cPaths;
                }

                last = begin + 1;
            }
        }

        if(1 < begin - last)
        {
            ++cPaths;
        }

        return cPaths;
    }

    // One to be ashamed of. This will be replaced in the next version of the libraries
    static char_type const *_disgusting_hack(ws_char_a_t *literal_a, ws_char_w_t *literal_w)
    {
        return static_cast<char_type *>((sizeof(char_type) == sizeof(ws_char_w_t)) ? static_cast<void*>(literal_w) : static_cast<void*>(literal_a));
    }

// Not to be implemented
private:
    basic_searchpath_sequence(class_type const &);
    const basic_searchpath_sequence &operator =(class_type const &);
};

/* /////////////////////////////////////////////////////////////////////////////
 * Typedefs for commonly encountered types
 */

/// Instantiation of the basic_searchpath_sequence template for the ANSI character type \c char
typedef basic_searchpath_sequence<ws_char_a_t, filesystem_traits<ws_char_a_t> > searchpath_sequence_a;
/// Instantiation of the basic_searchpath_sequence template for the Unicode character type \c wchar_t
typedef basic_searchpath_sequence<ws_char_w_t, filesystem_traits<ws_char_w_t> > searchpath_sequence_w;
/// Instantiation of the basic_searchpath_sequence template for the Win32 character type \c TCHAR
typedef basic_searchpath_sequence<TCHAR, filesystem_traits<TCHAR> >             searchpath_sequence;

///////////////////////////////////////////////////////////////////////////////
// Unit-testing

#ifdef STLSOFT_UNITTEST

namespace unittest
{
    ss_bool_t test_winstl_searchpath_sequence(unittest_reporter *r)
    {
        using stlsoft::unittest::unittest_initialiser;

        ss_bool_t               bSuccess    =   true;

        unittest_initialiser    init(r, "WinSTL", "searchpath_sequence", __FILE__);

#if 0
        if(<<TODO>>)
        {
            r->report("<<TODO>> failed ", __LINE__);
            bSuccess = false;
        }
#endif /* 0 */

        return bSuccess;
    }

    unittest_registrar    unittest_winstl_searchpath_sequence(test_winstl_searchpath_sequence);

} // namespace unittest

#endif /* STLSOFT_UNITTEST */

////////////////////////////////////////////////////////////////////////////////
// Implementation

#ifndef __STLSOFT_DOCUMENTATION_SKIP_SECTION

// basic_searchpath_sequence

template<   ss_typename_param_k C
        ,   ss_typename_param_k T
        >
inline basic_searchpath_sequence<C, T>::basic_searchpath_sequence()
    : m_buffer(directories_total())
    , m_values(num_paths() + (system_version::winnt() ? 5 : 4))
{
    _construct(true, false, true);
}

template<   ss_typename_param_k C
        ,   ss_typename_param_k T
        >
inline basic_searchpath_sequence<C, T>::basic_searchpath_sequence(ws_bool_t bIncludeApplicationDirectory, ws_bool_t bIncludeCurrentDirectory, ws_bool_t bApplicationDirectoryFirst /* = true */)

    : m_buffer(directories_total())
    , m_values(num_paths() + (system_version::winnt() ? 5 : 4))
{
    _construct(bIncludeApplicationDirectory, bIncludeCurrentDirectory, bApplicationDirectoryFirst);
}

template<   ss_typename_param_k C
        ,   ss_typename_param_k T
        >
inline void basic_searchpath_sequence<C, T>::_construct(ws_bool_t bIncludeApplicationDirectory, ws_bool_t bIncludeCurrentDirectory, ws_bool_t bApplicationDirectoryFirst)
{
    // 6 - n. Path directories

    char_type       *psz    =   m_buffer;
    char_type const **it    =   m_values.begin();

    psz[0] = '\0';

    { for(int i = 0; i < 2; ++i) {

        if((i & 1) != static_cast<int>(bApplicationDirectoryFirst))
        {
            if(bIncludeApplicationDirectory)
            {
                *it++ = psz;

                // 1. Application directory - GetModuleFileName(NULL, ...);
                traits_type::str_copy(psz, get_application_directory());
                psz += traits_type::str_len(psz);
            }
        }
        else
        {
            if(bIncludeCurrentDirectory)
            {
                *it++ = psz;

                // 2. Current directory - GetCurrentDirectory
                psz += traits_type::get_current_directory(_MAX_PATH + 1, psz);
            }
        }

        ++psz;
    }}

    // 3. System directory
    *it++ = psz;
    traits_type::str_copy(psz, get_system_directory());
    psz += traits_type::str_len(psz);
    ++psz;

    // 4. NT-only: 16-bit system directory
    if(system_version::winnt())
    {
        *it++ = psz;
        traits_type::str_copy(psz, get_system16_directory());
        psz += traits_type::str_len(psz);
        ++psz;
    }

    // 5. Windows directory
    *it++ = psz;
    traits_type::str_copy(psz, get_windows_directory());
    psz += traits_type::str_len(psz);
    ++psz;

    // 6. Paths
    char_type const         *begin  =   psz;
    char_type const * const end     =   m_buffer.end();
    char_type const         *last;

    traits_type::get_environment_variable(_disgusting_hack("PATH", L"PATH"), psz, static_cast<DWORD>(end - psz));

    // Move along to the first valid item
    for(; begin != end; ++begin)
    {
        if(*begin != ';')
        {
            break;
        }
    }

    for(last = begin; begin != end; ++begin)
    {
        if(*begin == ';')
        {
            if(1 < begin - last)
            {
                *it++ = last;
            }

            *const_cast<char_type*>(begin) = '\0';

            last = begin + 1;
        }
    }

    if(1 < begin - last)
    {
        *it++ = last;
    }

    m_end   =   it;
}

template<   ss_typename_param_k C
        ,   ss_typename_param_k T
        >
inline basic_searchpath_sequence<C, T>::~basic_searchpath_sequence() winstl_throw_0()
{}

template<   ss_typename_param_k C
        ,   ss_typename_param_k T
        >
inline ss_typename_type_k basic_searchpath_sequence<C, T>::const_iterator basic_searchpath_sequence<C, T>::begin() const
{
    return m_values.begin();
}

template<   ss_typename_param_k C
        ,   ss_typename_param_k T
        >
inline ss_typename_type_k basic_searchpath_sequence<C, T>::const_iterator basic_searchpath_sequence<C, T>::end() const
{
    return m_end;
}

#if defined(__STLSOFT_CF_BIDIRECTIONAL_ITERATOR_SUPPORT)
template<   ss_typename_param_k C
        ,   ss_typename_param_k T
        >
inline ss_typename_type_k basic_searchpath_sequence<C, T>::const_reverse_iterator basic_searchpath_sequence<C, T>::rbegin() const
{
    return const_reverse_iterator(end());
}

template<   ss_typename_param_k C
        ,   ss_typename_param_k T
        >
inline ss_typename_type_k basic_searchpath_sequence<C, T>::const_reverse_iterator basic_searchpath_sequence<C, T>::rend() const
{
    return const_reverse_iterator(begin());
}
#endif /* __STLSOFT_CF_BIDIRECTIONAL_ITERATOR_SUPPORT */

template<   ss_typename_param_k C
        ,   ss_typename_param_k T
        >
inline ss_typename_type_k basic_searchpath_sequence<C, T>::size_type basic_searchpath_sequence<C, T>::size() const
{
    return static_cast<size_type>(end() - begin());
}

template<   ss_typename_param_k C
        ,   ss_typename_param_k T
        >
inline ws_bool_t basic_searchpath_sequence<C, T>::empty() const
{
    return begin() == end();
}

template<   ss_typename_param_k C
        ,   ss_typename_param_k T
        >
inline /* static */ ss_typename_type_k basic_searchpath_sequence<C, T>::size_type basic_searchpath_sequence<C, T>::max_size()
{
    // Kind of kludgy, sigh.
    return static_cast<size_type>(-1) / _MAX_PATH;
}

template<   ss_typename_param_k C
        ,   ss_typename_param_k T
        >
inline ss_typename_type_k basic_searchpath_sequence<C, T>::value_type basic_searchpath_sequence<C, T>::operator [](ss_typename_type_k basic_searchpath_sequence<C, T>::size_type index) const
{
    winstl_message_assert("Invalid index in search path sequence", !(size() < index));

    return begin()[index];
}

#endif /* !__STLSOFT_DOCUMENTATION_SKIP_SECTION */

/* ////////////////////////////////////////////////////////////////////////// */

/// @} // end of group winstl_filesystem_library

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

#endif /* WINSTL_INCL_H_WINSTL_SEARCHPATH_SEQUENCE */

/* ////////////////////////////////////////////////////////////////////////// */
