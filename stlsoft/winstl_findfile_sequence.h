/* /////////////////////////////////////////////////////////////////////////////
 * File:        winstl_findfile_sequence.h
 *
 * Purpose:     Contains the basic_findfile_sequence template class, and ANSI
 *              and Unicode specialisations thereof.
 *
 * Notes:       1. The original implementation of the class had the const_iterator
 *              and value_type as nested classes. Unfortunately, Visual C++ 5 &
 *              6 both had either compilation or linking problems so these are
 *              regretably now implemented as independent classes.
 *
 *              2. This class was described in detail in the article
 *              "Adapting Windows Enumeration Models to STL Iterator Concepts"
 *              (http://www.windevnet.com/documents/win0303a/), in the March
 *              2003 issue of Windows Developer Network (http://windevnet.com).
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


/// \file winstl_findfile_sequence.h
///
/// Contains the basic_findfile_sequence template class, and ANSI and Unicode specialisations thereof.

#ifndef WINSTL_INCL_H_WINSTL_FINDFILE_SEQUENCE
#define WINSTL_INCL_H_WINSTL_FINDFILE_SEQUENCE

#ifndef __STLSOFT_DOCUMENTATION_SKIP_SECTION
# define WINSTL_VER_H_WINSTL_FINDFILE_SEQUENCE_MAJOR        2
# define WINSTL_VER_H_WINSTL_FINDFILE_SEQUENCE_MINOR        0
# define WINSTL_VER_H_WINSTL_FINDFILE_SEQUENCE_REVISION     1
# define WINSTL_VER_H_WINSTL_FINDFILE_SEQUENCE_EDIT         110
#endif /* !__STLSOFT_DOCUMENTATION_SKIP_SECTION */

/* /////////////////////////////////////////////////////////////////////////////
 * Compatibility
 */

/*
[Incompatibilies-start]
__STLSOFT_COMPILER_IS_MSVC: _MSC_VER<1200
[Incompatibilies-end]
 */

/* /////////////////////////////////////////////////////////////////////////////
 * Includes
 */

#ifndef WINSTL_INCL_H_WINSTL
# include "winstl.h"                    // Include the WinSTL root header
#endif /* !WINSTL_INCL_H_WINSTL */
#ifndef WINSTL_INCL_H_WINSTL_FILESYSTEM_TRAITS

#if defined(__STLSOFT_COMPILER_IS_MSVC) && \
    _MSC_VER < 1200
# error winstl_file_path_buffer.h is not compatible with Visual C++ 5.0 or earlier
#endif /* _MSC_VER < 1200 */

# include "winstl_filesystem_traits.h"  // filesystem_traits
#endif /* !WINSTL_INCL_H_WINSTL_FILESYSTEM_TRAITS */
#ifndef WINSTL_INCL_H_WINSTL_FILE_PATH_BUFFER
# include "winstl_file_path_buffer.h"   // basic_file_path_buffer
#endif /* !WINSTL_INCL_H_WINSTL_FILE_PATH_BUFFER */
#ifndef WINSTL_INCL_H_WINSTL_SYSTEM_VERSION
# include "winstl_system_version.h"     // winnt(), major()
#endif /* !WINSTL_INCL_H_WINSTL_SYSTEM_VERSION */
#ifndef WINSTL_INCL_H_WINSTL_STRING_ACCESS
# include "winstl_string_access.h"                  // winstl::string_access
#endif /* WINSTL_INCL_H_WINSTL_STRING_ACCESS */
#ifndef __STLSOFT_COMPILER_IS_WATCOM
# ifndef STLSOFT_INCL_H_STLSOFT_ITERATOR
#  include "stlsoft_iterator.h"          // iterator_base
# endif /* !STLSOFT_INCL_H_STLSOFT_ITERATOR */
#endif /* __STLSOFT_COMPILER_IS_WATCOM */

/* /////////////////////////////////////////////////////////////////////////////
 * Pre-processor
 *
 * Definition of the
 */

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
 * Forward declarations
 */

#ifndef __STLSOFT_DOCUMENTATION_SKIP_SECTION

template <ss_typename_param_k C, ss_typename_param_k T>
class basic_findfile_sequence_value_type;

template <ss_typename_param_k C, ss_typename_param_k T, ss_typename_param_k V>
class basic_findfile_sequence_const_input_iterator;

#endif /* !__STLSOFT_DOCUMENTATION_SKIP_SECTION */

/* /////////////////////////////////////////////////////////////////////////////
 * Classes
 */

// class basic_findfile_sequence
/// Presents an STL-like sequence interface over the items on the file-system
///
/// \param C The character type
/// \param T The traits type. On translators that support default template arguments this defaults to filesystem_traits<C>
///
/// \note  This class was described in detail in the article
/// "Adapting Windows Enumeration Models to STL Iterator Concepts"
/// (http://www.windevnet.com/documents/win0303a/), in the March 2003 issue of
/// Windows Developer Network (http://windevnet.com).
template<   ss_typename_param_k C
#ifdef __STLSOFT_CF_TEMPLATE_CLASS_DEFAULT_CLASS_ARGUMENT_SUPPORT
        ,   ss_typename_param_k T = filesystem_traits<C>
#else
        ,   ss_typename_param_k T /* = filesystem_traits<C> */
#endif /* __STLSOFT_CF_TEMPLATE_CLASS_DEFAULT_CLASS_ARGUMENT_SUPPORT */
        >
class basic_findfile_sequence
{
public:
    /// The character type
    typedef C                                                                   char_type;
    /// The traits type
    typedef T                                                                   traits_type;
    /// The current parameterisation of the type
    typedef basic_findfile_sequence<C, T>                                       class_type;
    /// The value type
    typedef basic_findfile_sequence_value_type<C, T>                            value_type;
    /// The non-mutating (const) iterator type supporting the Input Iterator concept
    typedef basic_findfile_sequence_const_input_iterator<C, T, value_type>      const_input_iterator;
    /// The non-mutating (const) iterator type
    typedef const_input_iterator                                                const_iterator;
    /// The reference type
    typedef value_type const                                                    reference;
    /// The non-mutable (const) reference type
    typedef value_type const                                                    const_reference;
    /// The find-data type
    typedef ss_typename_type_k traits_type::find_data_type                      find_data_type;
    /// The difference type
    typedef ws_ptrdiff_t                                                        difference_type;
    /// The size type
    typedef ws_size_t                                                           size_type;

    enum
    {
            includeDots =   0x0008          //!< Causes the search to include the "." and ".." directories, which are elided by default
        ,   directories =   0x0010          //!< Causes the search to include directories
        ,   files       =   0x0020          //!< Causes the search to include files
#ifndef __STLSOFT_DOCUMENTATION_SKIP_SECTION
        ,   noSort      =   0 /* 0x0100 */  //!<
#endif /* !__STLSOFT_DOCUMENTATION_SKIP_SECTION */
        ,
    };

// Construction
public:
    /// Commence a search according to the given search pattern and flags
    ss_explicit_k basic_findfile_sequence(char_type const *searchSpec, ws_int_t flags = directories | files);
    /// Commence a search according to the given search pattern and flags, relative to \c directory
    basic_findfile_sequence(char_type const *directory, char_type const *searchSpec, ws_int_t flags = directories | files);
    /// Destructor
    ~basic_findfile_sequence() winstl_throw_0();

// Iteration
public:
    /// Begins the iteration
    ///
    /// \return An iterator representing the start of the sequence
    const_iterator      begin() const;
    /// Ends the iteration
    ///
    /// \return An iterator representing the end of the sequence
    const_iterator      end() const;

// Attributes
public:
    /// Returns the directory of the search
    ///
    /// \note Will be the empty string for instances created with the first constructor
    char_type const     *get_directory() const;

// State
public:
#ifdef STLSOFT_OBSOLETE
    /// Returns the number of items in the sequence
    ///
    /// \note This is a potentially very expensive operation
    /// \deprecated
    size_type           size() const;
#endif /* STLSOFT_OBSOLETE */
    /// Indicates whether the sequence is empty
    ws_bool_t           empty() const;
    /// Returns the maximum number of items in the sequence
    static size_type    max_size();

// Members
private:
    friend class basic_findfile_sequence_value_type<C, T>;
    friend class basic_findfile_sequence_const_input_iterator<C, T, value_type>;

    typedef basic_file_path_buffer<char_type>   file_path_buffer_type;

    file_path_buffer_type   m_directory;
    file_path_buffer_type   m_subpath;
    file_path_buffer_type   m_search;
    ws_int_t const          m_flags;

// Implementation
private:
    static ws_int_t validate_flags_(ws_int_t flags);
    static void     extract_subpath_(file_path_buffer_type &dest, char_type const *searchSpec);

    static  HANDLE  find_first_file_(char_type const *spec, ws_int_t flags, find_data_type *findData);
    HANDLE          begin_(find_data_type &findData) const;

// Not to be implemented
private:
    class_type &operator =(class_type const&);
};

/* /////////////////////////////////////////////////////////////////////////////
 * Typedefs for commonly encountered types
 */

/// Instantiation of the basic_findfile_sequence template for the ANSI character type \c char
typedef basic_findfile_sequence<ws_char_a_t, filesystem_traits<ws_char_a_t> >     findfile_sequence_a;
/// Instantiation of the basic_findfile_sequence template for the Unicode character type \c wchar_t
typedef basic_findfile_sequence<ws_char_w_t, filesystem_traits<ws_char_w_t> >     findfile_sequence_w;
/// Instantiation of the basic_findfile_sequence template for the Win32 character type \c TCHAR
typedef basic_findfile_sequence<TCHAR, filesystem_traits<TCHAR> >                 findfile_sequence;

/* ////////////////////////////////////////////////////////////////////////// */

// class basic_findfile_sequence_value_type
/// Value type for the basic_findfile_sequence
template<   ss_typename_param_k C
        ,   ss_typename_param_k T
        >
class basic_findfile_sequence_value_type
{
public:
    /// The character type
    typedef C                                               char_type;
    /// The traits type
    typedef T                                               traits_type;
    /// The current parameterisation of the type
    typedef basic_findfile_sequence_value_type<C, T>        class_type;
    /// The find-data type
    typedef ss_typename_type_k traits_type::find_data_type  find_data_type;

public:
    /// Default constructor
    basic_findfile_sequence_value_type();
private:
    basic_findfile_sequence_value_type(find_data_type const &data, char_type const *path)
        : m_data(data)
    {
        traits_type::str_copy(&m_path[0], path);
        traits_type::ensure_dir_end(&m_path[0]);
        traits_type::str_cat(&m_path[0], data.cFileName);
    }
public:
#if 0
    /// Copy constructor
    basic_findfile_sequence_value_type(class_type const &rhs);
#endif /* 0 */

    /// Destructor
    ~basic_findfile_sequence_value_type() winstl_throw_0();

    /// Copy assignment operator
    class_type &operator =(class_type const &rhs);

    /// Returns a non-mutating reference to find-data
    find_data_type const    &get_find_data() const;
    /// Returns a non-mutating reference to find-data
    ///
    /// \deprecated This method may be removed in a future release. get_find_data() should be used instead
    find_data_type const    &GetFindData() const;   // Deprecated

    /// Returns the filename part of the item
    char_type const         *get_filename() const;
    /// Returns the short form of the filename part of the item
    char_type const         *get_short_filename() const;
    /// Returns the full path of the item
    char_type const         *get_path() const;

    /// Implicit conversion to a pointer-to-const of the full path
    operator char_type const * () const;

// Members
private:
    friend class basic_findfile_sequence_const_input_iterator<C, T, class_type>;

    typedef basic_file_path_buffer<char_type>   file_path_buffer_type;

    find_data_type          m_data;
    file_path_buffer_type   m_path;
};

// class basic_findfile_sequence_const_input_iterator
/// Iterator type for the basic_findfile_sequence supporting the Input Iterator concept
template <ss_typename_param_k C, ss_typename_param_k T, ss_typename_param_k V>
class basic_findfile_sequence_const_input_iterator
#ifndef __STLSOFT_COMPILER_IS_WATCOM
    : public stlsoft_ns_qual(iterator_base)<winstl_ns_qual_std(input_iterator_tag), V, ws_ptrdiff_t, void, V>
#endif /* !__STLSOFT_COMPILER_IS_WATCOM */
{
public:
    /// The character type
    typedef C                                                       char_type;
    /// The traits type
    typedef T                                                       traits_type;
    /// The value type
    typedef V                                                       value_type;
    /// The current parameterisation of the type
    typedef basic_findfile_sequence_const_input_iterator<C, T, V>   class_type;
    /// The find-data type
    typedef ss_typename_type_k traits_type::find_data_type          find_data_type;
private:
    typedef basic_findfile_sequence<C, T>                           sequence_type;

/// \name Utility classes
/// @{
private:
#ifndef __STLSOFT_DOCUMENTATION_SKIP_SECTION
    struct shared_handle
    {
    public:
        typedef shared_handle       class_type;

    // Members
    public:
        HANDLE      hSrch;
    private:
        ss_sint32_t cRefs;

    /// Construction
    public:
        ss_explicit_k shared_handle(HANDLE h)
            : hSrch(h)
            , cRefs(1)
        {}
# if defined(__STLSOFT_COMPILER_IS_GCC)
    protected:
# else /* ? __STLSOFT_COMPILER_IS_GCC */
    private:
# endif /* __STLSOFT_COMPILER_IS_GCC */
        ~shared_handle()
        {
            winstl_message_assert("Shared search handle being destroyed with outstanding references!", 0 == cRefs);

            if(hSrch != INVALID_HANDLE_VALUE)
            {
                traits_type::find_file_close(hSrch);
            }
        }

    /// Operations
    public:
        void AddRef()
        {
            ++cRefs;
        }
        void Release()
        {
            if(0 == --cRefs)
            {
                delete this;
            }
        }

    // Not to be implemented
    private:
        shared_handle(class_type const &);
        class_type &operator =(class_type const &);
    };
    #endif /* !__STLSOFT_DOCUMENTATION_SKIP_SECTION */
/// @}

/// \name Construction
/// @{
private:
    basic_findfile_sequence_const_input_iterator(sequence_type const &l, HANDLE hSrch, find_data_type const &data)
        : m_list(&l)
        , m_handle(new shared_handle(hSrch))
        , m_data(data)
    {
        winstl_assert(INVALID_HANDLE_VALUE != hSrch);
    }
    basic_findfile_sequence_const_input_iterator(sequence_type const &l);
public:
    /// Default constructor
    basic_findfile_sequence_const_input_iterator();
    /// <a href = "http://synesis.com.au/resources/articles/cpp/movectors.pdf">Move constructor</a>
    basic_findfile_sequence_const_input_iterator(class_type const &rhs);
    /// Destructor
    ~basic_findfile_sequence_const_input_iterator() winstl_throw_0();

    // Copy assignment operator
    basic_findfile_sequence_const_input_iterator &operator =(class_type const &rhs);
/// @}

public:
    /// Pre-increment operator
    class_type &operator ++();
    /// Post-increment operator
    class_type operator ++(int);
    /// Dereference to return the value at the current position
    const value_type operator *() const;
    /// Evaluates whether \c this and \c rhs are equivalent
    ws_bool_t operator ==(class_type const &rhs) const;
    /// Evaluates whether \c this and \c rhs are not equivalent
    ws_bool_t operator !=(class_type const &rhs) const;

// Members
private:
    friend class basic_findfile_sequence<C, T>;

    sequence_type const * const                     m_list;
    shared_handle                                   *m_handle;
    ss_typename_type_k traits_type::find_data_type  m_data;
};

///////////////////////////////////////////////////////////////////////////////
// Shims

template <ss_typename_param_k C, ss_typename_param_k T>
inline C const *c_str_ptr_null(basic_findfile_sequence_value_type<C, T> const &v)
{
    return stlsoft_ns_qual(c_str_ptr_null(v.get_path()));
}

template <ss_typename_param_k C, ss_typename_param_k T>
inline C const *c_str_ptr(basic_findfile_sequence_value_type<C, T> const &v)
{
    return stlsoft_ns_qual(c_str_ptr(v.get_path()));
}

template <ss_typename_param_k C, ss_typename_param_k T>
inline ws_size_t c_str_len(basic_findfile_sequence_value_type<C, T> const &v)
{
    return stlsoft_ns_qual(c_str_len(v.get_path()));
}

template <ss_typename_param_k C, ss_typename_param_k T>
inline ws_size_t c_str_size(basic_findfile_sequence_value_type<C, T> const &v)
{
    return stlsoft_ns_qual(c_str_size(v.get_path()));
}

template <ss_typename_param_k C, ss_typename_param_k T>
inline ws_bool_t is_empty(basic_findfile_sequence<C, T> const &s)
{
    return s.empty();
}

////////////////////////////////////////////////////////////////////////////////
// Unit-testing

#ifdef STLSOFT_UNITTEST

namespace unittest
{
    ss_bool_t test_winstl_findfile_sequence(unittest_reporter *r)
    {
        using stlsoft::unittest::unittest_initialiser;

        ss_bool_t               bSuccess    =   true;

        unittest_initialiser    init(r, "WinSTL", "findfile_sequence", __FILE__);

#if 0
        if(<<TODO>>)
        {
            r->report("<<TODO>> failed ", __LINE__);
            bSuccess = false;
        }
#endif /* 0 */

        return bSuccess;
    }

    unittest_registrar    unittest_winstl_findfile_sequence(test_winstl_findfile_sequence);

} // namespace unittest

#endif /* STLSOFT_UNITTEST */

///////////////////////////////////////////////////////////////////////////////
// Implementation

#ifndef __STLSOFT_DOCUMENTATION_SKIP_SECTION

// basic_findfile_sequence

template <ss_typename_param_k C, ss_typename_param_k T>
inline /* static */ HANDLE basic_findfile_sequence<C, T>::find_first_file_(ss_typename_type_k basic_findfile_sequence<C, T>::char_type const *spec, ws_int_t flags, ss_typename_type_k basic_findfile_sequence<C, T>::find_data_type *findData)
{
    HANDLE  hSrch;

#if _WIN32_WINNT >= 0x0400
    if( (directories == (flags & (directories | files))) &&
        system_version::winnt() &&
        system_version::major() >= 4)
    {
        hSrch = traits_type::find_first_file_ex(spec, FindExSearchLimitToDirectories , findData);
    }
    else
#else
    STLSOFT_SUPPRESS_UNUSED(flags);
#endif /* _WIN32_WINNT >= 0x0400 */
    {
        hSrch = traits_type::find_first_file(spec, findData);
    }

    return hSrch;
}

template <ss_typename_param_k C, ss_typename_param_k T>
inline HANDLE basic_findfile_sequence<C, T>::begin_(ss_typename_type_k basic_findfile_sequence<C, T>::find_data_type &findData) const
{
    HANDLE  hSrch   =   find_first_file_(stlsoft_ns_qual(c_str_ptr)(m_search), m_flags, &findData);

    if(hSrch != INVALID_HANDLE_VALUE)
    {
        // Now need to validate against the flags

        for(; hSrch != INVALID_HANDLE_VALUE; )
        {
            if((findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) == 0)
            {
                // A file, and files requested, so break
                if(m_flags & files)
                {
                    break;
                }
            }
            else
            {
                if(traits_type::is_dots(findData.cFileName))
                {
                    if(m_flags & includeDots)
                    {
                        // A dots file, and dots are requested
                        break;
                    }
                }
                else if(m_flags & directories)
                {
                    // A directory, and directories requested
                    break;
                }
            }

            if(!traits_type::find_next_file(hSrch, &findData))
            {
                ::FindClose(hSrch);

                hSrch = INVALID_HANDLE_VALUE;

                break;
            }
        }
    }

    return hSrch;
}

template <ss_typename_param_k C, ss_typename_param_k T>
inline /* static */ ws_int_t basic_findfile_sequence<C, T>::validate_flags_(ws_int_t flags)
{
    return (flags & (directories | files)) == 0 ? (flags | (directories | files)) : flags;
}

template <ss_typename_param_k C, ss_typename_param_k T>
inline /* static */ void basic_findfile_sequence<C, T>::extract_subpath_(ss_typename_param_k basic_findfile_sequence<C, T>::file_path_buffer_type &dest, ss_typename_param_k basic_findfile_sequence<C, T>::char_type const *searchSpec)
{
    char_type   *pFile;

    traits_type::get_full_path_name(searchSpec, dest.size(), &dest[0], &pFile);

    if(pFile != 0)
    {
        *pFile = '\0';
    }
}

// Construction
template <ss_typename_param_k C, ss_typename_param_k T>
inline basic_findfile_sequence<C, T>::basic_findfile_sequence(char_type const *searchSpec, ws_int_t flags /* = directories | files */)
    : m_flags(validate_flags_(flags))
{
    m_directory[0] = '\0';

    traits_type::str_copy(&m_search[0], searchSpec);

    extract_subpath_(m_subpath, searchSpec);
}

template <ss_typename_param_k C, ss_typename_param_k T>
inline basic_findfile_sequence<C, T>::basic_findfile_sequence(char_type const *directory, char_type const *searchSpec, ws_int_t flags /* = directories | files */)
    : m_flags(validate_flags_(flags))
{
    traits_type::str_copy(&m_directory[0], directory);

    traits_type::str_copy(&m_search[0], directory);
    if( searchSpec[0] == '.' &&
        searchSpec[1] == '\0')
    {
        // This is needed to prevent a searchSpec of "." from stipulating the name twice
        traits_type::remove_dir_end(&m_search[0]);
    }
    else
    {
        traits_type::ensure_dir_end(&m_search[0]);
        traits_type::str_cat(&m_search[0], searchSpec);
    }

    extract_subpath_(m_subpath, &m_search[0]);
}

template <ss_typename_param_k C, ss_typename_param_k T>
inline basic_findfile_sequence<C, T>::~basic_findfile_sequence() winstl_throw_0()
{
#ifdef _DEBUG
    m_directory[0]  =   '\0';
    m_subpath[0]    =   '\0';
    m_search[0]     =   '\0';
#endif /* _DEBUG */
}

// Iteration
template <ss_typename_param_k C, ss_typename_param_k T>
inline ss_typename_type_k basic_findfile_sequence<C, T>::const_iterator basic_findfile_sequence<C, T>::begin() const
{
    ss_typename_type_k traits_type::find_data_type  findData;
    HANDLE                                          hSrch   =   begin_(findData);

    if(hSrch == INVALID_HANDLE_VALUE)
    {
        return const_input_iterator(*this);
    }
    else
    {
        return const_input_iterator(*this, hSrch, findData);
    }
}

template <ss_typename_param_k C, ss_typename_param_k T>
inline ss_typename_type_k basic_findfile_sequence<C, T>::const_iterator basic_findfile_sequence<C, T>::end() const
{
    return const_input_iterator(*this);
}

// Attributes
template <ss_typename_param_k C, ss_typename_param_k T>
ss_typename_type_k basic_findfile_sequence<C, T>::char_type const *basic_findfile_sequence<C, T>::get_directory() const
{
    return stlsoft_ns_qual(c_str_ptr)(m_directory);
}

// State
#ifdef STLSOFT_OBSOLETE
template <ss_typename_param_k C, ss_typename_param_k T>
inline ss_typename_type_k basic_findfile_sequence<C, T>::size_type basic_findfile_sequence<C, T>::size() const
{
    return stlsoft_ns_qual_std(distance)(begin(), end());
}
#endif /* STLSOFT_OBSOLETE */

template <ss_typename_param_k C, ss_typename_param_k T>
inline ws_bool_t basic_findfile_sequence<C, T>::empty() const
{
    return begin() == end();
}

template <ss_typename_param_k C, ss_typename_param_k T>
inline /* static */ ss_typename_type_k basic_findfile_sequence<C, T>::size_type basic_findfile_sequence<C, T>::max_size()
{
    return static_cast<size_type>(-1);
}

// basic_findfile_sequence_value_type

template <ss_typename_param_k C, ss_typename_param_k T>
inline basic_findfile_sequence_value_type<C, T>::basic_findfile_sequence_value_type()
{
    m_data.dwFileAttributes         =   0xFFFFFFFF;
    m_data.cFileName[0]             =   '\0';
    m_data.cAlternateFileName[0]    =   '\0';
    m_path[0]                       =   '\0';
}

#if 0
template <ss_typename_param_k C, ss_typename_param_k T>
inline basic_findfile_sequence_value_type<C, T>::basic_findfile_sequence_value_type(ss_typename_type_k basic_findfile_sequence_value_type<C, T>::class_type const &rhs)
    : m_data(rhs.m_data)
    , m_path(rhs.m_path)
{}
#endif /* 0 */

template <ss_typename_param_k C, ss_typename_param_k T>
inline basic_findfile_sequence_value_type<C, T>::~basic_findfile_sequence_value_type() winstl_throw_0()
{
#ifdef _DEBUG
    m_data.cFileName[0] =   '\0';
    m_path[0]           =   '\0';
#endif /* _DEBUG */
}

#if 0
template <ss_typename_param_k C, ss_typename_param_k T>
#ifdef __STLSOFT_CF_FUNCTION_SIGNATURE_FULL_ARG_QUALIFICATION_REQUIRED
inline basic_findfile_sequence_value_type<C, T>::operator basic_findfile_sequence_value_type<C, T>::char_type const *() const
#else
inline basic_findfile_sequence_value_type<C, T>::operator char_type const *() const
#endif /* __STLSOFT_CF_FUNCTION_SIGNATURE_FULL_ARG_QUALIFICATION_REQUIRED */
{
    return m_data.cFileName;
}
#endif /* 0 */

template <ss_typename_param_k C, ss_typename_param_k T>
inline ss_typename_type_k basic_findfile_sequence_value_type<C, T>::find_data_type const &basic_findfile_sequence_value_type<C, T>::get_find_data() const
{
    return m_data;
}

template <ss_typename_param_k C, ss_typename_param_k T>
inline ss_typename_type_k basic_findfile_sequence_value_type<C, T>::find_data_type const &basic_findfile_sequence_value_type<C, T>::GetFindData() const
{
    return get_find_data();
}

template <ss_typename_param_k C, ss_typename_param_k T>
inline ss_typename_type_k basic_findfile_sequence_value_type<C, T>::char_type const *basic_findfile_sequence_value_type<C, T>::get_filename() const
{
    return m_data.cFileName;
}

template <ss_typename_param_k C, ss_typename_param_k T>
inline ss_typename_type_k basic_findfile_sequence_value_type<C, T>::char_type const *basic_findfile_sequence_value_type<C, T>::get_short_filename() const
{
    return m_data.cAlternateFileName[0] != '\0' ? m_data.cAlternateFileName : m_data.cFileName;
}

template <ss_typename_param_k C, ss_typename_param_k T>
inline ss_typename_type_k basic_findfile_sequence_value_type<C, T>::char_type const *basic_findfile_sequence_value_type<C, T>::get_path() const
{
    return stlsoft_ns_qual(c_str_ptr)(m_path);
}

template <ss_typename_param_k C, ss_typename_param_k T>
#if defined(__STLSOFT_COMPILER_IS_GCC) || \
    (   defined(__STLSOFT_COMPILER_IS_MSVC) && \
        _MSC_VER < 1100)
inline basic_findfile_sequence_value_type<C, T>::operator C const * () const
#else
inline basic_findfile_sequence_value_type<C, T>::operator ss_typename_type_k basic_findfile_sequence_value_type<C, T>::char_type const * () const
#endif /* !__GNUC__ */
{
    return get_path();
}

// operator == ()
template <ss_typename_param_k C, ss_typename_param_k T>
inline ws_bool_t operator == (basic_findfile_sequence_value_type<C, T> const &lhs, basic_findfile_sequence_value_type<C, T> const &rhs)
{
    return 0 == basic_findfile_sequence_value_type<C, T>::traits_type::str_compare(lhs.get_path(), rhs.get_path());
}

template <ss_typename_param_k C, ss_typename_param_k T>
inline ws_bool_t operator == (basic_findfile_sequence_value_type<C, T> const &lhs, C const *rhs)
{
    return 0 == basic_findfile_sequence_value_type<C, T>::traits_type::str_compare(lhs.get_path(), rhs);
}

template <ss_typename_param_k C, ss_typename_param_k T>
inline ws_bool_t operator == (C const *lhs, basic_findfile_sequence_value_type<C, T> const &rhs)
{
    return 0 == basic_findfile_sequence_value_type<C, T>::traits_type::str_compare(lhs, rhs.get_path());
}

// basic_findfile_sequence_const_input_iterator

template <ss_typename_param_k C, ss_typename_param_k T, ss_typename_param_k V>
inline basic_findfile_sequence_const_input_iterator<C, T, V>::basic_findfile_sequence_const_input_iterator()
    : m_list(NULL)
    , m_handle(NULL)
{}

template <ss_typename_param_k C, ss_typename_param_k T, ss_typename_param_k V>
inline basic_findfile_sequence_const_input_iterator<C, T, V>::basic_findfile_sequence_const_input_iterator(sequence_type const &l)
    : m_list(&l)
    , m_handle(NULL)
{}

template <ss_typename_param_k C, ss_typename_param_k T, ss_typename_param_k V>
inline basic_findfile_sequence_const_input_iterator<C, T, V>::basic_findfile_sequence_const_input_iterator(class_type const &rhs)
    : m_list(rhs.m_list)
    , m_handle(rhs.m_handle)
    , m_data(rhs.m_data)
{
    if(NULL != m_handle)
    {
        m_handle->AddRef();
    }
}

template <ss_typename_param_k C, ss_typename_param_k T, ss_typename_param_k V>
#ifndef __STLSOFT_COMPILER_IS_WATCOM
inline ss_typename_type_k basic_findfile_sequence_const_input_iterator<C, T, V>::class_type &basic_findfile_sequence_const_input_iterator<C, T, V>::operator =(ss_typename_param_k basic_findfile_sequence_const_input_iterator<C, T, V>::class_type const &rhs)
#else /* ? __STLSOFT_COMPILER_IS_WATCOM */
inline basic_findfile_sequence_const_input_iterator<C, T, V> &basic_findfile_sequence_const_input_iterator<C, T, V>::operator =(basic_findfile_sequence_const_input_iterator<C, T, V> const &rhs)
#endif /* __STLSOFT_COMPILER_IS_WATCOM */
{
    winstl_message_assert("Assigning iterators from separate sequences", m_list == rhs.m_list);    // Should only be comparing iterators from same container

    m_handle    =   rhs.m_handle;
    m_data      =   rhs.m_data;

    if(NULL != m_handle)
    {
        m_handle->AddRef();
    }

    return *this;
}

template <ss_typename_param_k C, ss_typename_param_k T, ss_typename_param_k V>
inline basic_findfile_sequence_const_input_iterator<C, T, V>::~basic_findfile_sequence_const_input_iterator() winstl_throw_0()
{
    if(NULL != m_handle)
    {
        m_handle->Release();
    }
}

template <ss_typename_param_k C, ss_typename_param_k T, ss_typename_param_k V>
inline ss_typename_type_k basic_findfile_sequence_const_input_iterator<C, T, V>::class_type &basic_findfile_sequence_const_input_iterator<C, T, V>::operator ++()
{
    ws_int_t    flags   =   m_list->m_flags;

    winstl_message_assert("Attempting to increment an invalid iterator!", NULL != m_handle);

    for(; m_handle->hSrch != INVALID_HANDLE_VALUE; )
    {
        if(!traits_type::find_next_file(m_handle->hSrch, &m_data))
        {
            m_handle->Release();

            m_handle = NULL;

            break;
        }
        else
        {
            if((m_data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) == 0)
            {
                // A file, and files requested, so break
                if(flags & sequence_type::files)
                {
                    break;
                }
            }
            else
            {
                if(traits_type::is_dots(m_data.cFileName))
                {
                    if(flags & sequence_type::includeDots)
                    {
                        // A dots file, and dots are requested
                        break;
                    }
                }
                else if(flags & sequence_type::directories)
                {
                    // A directory, and directories requested
                    break;
                }
            }
        }
    }

    return *this;
}

template <ss_typename_param_k C, ss_typename_param_k T, ss_typename_param_k V>
inline ss_typename_type_k basic_findfile_sequence_const_input_iterator<C, T, V>::class_type basic_findfile_sequence_const_input_iterator<C, T, V>::operator ++(int)
{
    class_type  ret(*this);

    operator ++();

    return ret;
}

template <ss_typename_param_k C, ss_typename_param_k T, ss_typename_param_k V>
inline const ss_typename_type_k basic_findfile_sequence_const_input_iterator<C, T, V>::value_type basic_findfile_sequence_const_input_iterator<C, T, V>::operator *() const
{
    if(NULL != m_handle)
    {
        return value_type(m_data, stlsoft_ns_qual(c_str_ptr)(m_list->m_subpath));
    }
    else
    {
        winstl_message_assert("Dereferencing end()-valued iterator", 0);

        return value_type();
    }
}

template <ss_typename_param_k C, ss_typename_param_k T, ss_typename_param_k V>
inline ws_bool_t basic_findfile_sequence_const_input_iterator<C, T, V>::operator ==(class_type const &rhs) const
{
    ws_bool_t    eq;

    // Should only be comparing iterators from same container
    winstl_message_assert("Comparing iterators from separate sequences", m_list == rhs.m_list);

    // Not equal if one but not both handles is the INVALID_HANDLE_VALUE
    // or if the data is not equal.
    if( (NULL == m_handle) != (NULL == rhs.m_handle) ||
        (   NULL != m_handle &&
            traits_type::str_compare(m_data.cFileName, rhs.m_data.cFileName) != 0))
    {
        eq = ws_false_v;
    }
    else
    {
        eq = ws_true_v;
    }

    return eq;
}

template <ss_typename_param_k C, ss_typename_param_k T, ss_typename_param_k V>
inline ws_bool_t basic_findfile_sequence_const_input_iterator<C, T, V>::operator !=(class_type const &rhs) const
{
    return ! operator ==(rhs);
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

/* /////////////////////////////////////////////////////////////////////////////
 * Namespace
 *
 * The string access shims exist either in the stlsoft namespace, or in the
 * global namespace. This is required by the lookup rules.
 *
 */

#ifndef _WINSTL_NO_NAMESPACE
# if !defined(_STLSOFT_NO_NAMESPACE) && \
     !defined(__STLSOFT_DOCUMENTATION_SKIP_SECTION)
namespace stlsoft
{
# else /* ? _STLSOFT_NO_NAMESPACE */
/* There is no stlsoft namespace, so must define in the global namespace */
# endif /* !_STLSOFT_NO_NAMESPACE */

//using ::winstl::c_str_ptr_null;

using ::winstl::c_str_ptr;

//using ::winstl::c_str_len;

//using ::winstl::c_str_size;

using ::winstl::is_empty;

# if !defined(_STLSOFT_NO_NAMESPACE) && \
     !defined(__STLSOFT_DOCUMENTATION_SKIP_SECTION)
} // namespace stlsoft
# else /* ? _STLSOFT_NO_NAMESPACE */
/* There is no stlsoft namespace, so must define in the global namespace */
# endif /* !_STLSOFT_NO_NAMESPACE */
#endif /* !_WINSTL_NO_NAMESPACE */

/* ////////////////////////////////////////////////////////////////////////// */

#endif /* WINSTL_INCL_H_WINSTL_FINDFILE_SEQUENCE */

/* ////////////////////////////////////////////////////////////////////////// */
