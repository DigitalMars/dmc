/* /////////////////////////////////////////////////////////////////////////////
 * File:        inetstl_findfile_sequence.h  (formerly MInetEnm.h)
 *
 * Purpose:     Contains the basic_findfile_sequence template class, and ANSI
 *              and Unicode specialisations thereof.
 *
 * Created:     30th April 1999
 * Updated:     11th September 2004
 *
 * Home:        http://stlsoft.org/
 *
 * Copyright (c) 1999-2004, Matthew Wilson and Synesis Software
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


/// \file inetstl_findfile_sequence.h
///
/// Contains the basic_findfile_sequence template class, and ANSI and Unicode specialisations thereof.

#ifndef INETSTL_INCL_H_INETSTL_FINDFILE_SEQUENCE
#define INETSTL_INCL_H_INETSTL_FINDFILE_SEQUENCE

#ifndef __STLSOFT_DOCUMENTATION_SKIP_SECTION
# define INETSTL_VER_H_INETSTL_FINDFILE_SEQUENCE_MAJOR        2
# define INETSTL_VER_H_INETSTL_FINDFILE_SEQUENCE_MINOR        0
# define INETSTL_VER_H_INETSTL_FINDFILE_SEQUENCE_REVISION     1
# define INETSTL_VER_H_INETSTL_FINDFILE_SEQUENCE_EDIT         60
#endif /* !__STLSOFT_DOCUMENTATION_SKIP_SECTION */

/* /////////////////////////////////////////////////////////////////////////////
 * Includes
 */

#ifndef INETSTL_INCL_H_INETSTL
# include "inetstl.h"                   // Include the InetSTL root header
#endif /* !INETSTL_INCL_H_INETSTL */
#ifndef INETSTL_INCL_H_INETSTL_FILESYSTEM_TRAITS
# include "inetstl_filesystem_traits.h" // filesystem_traits
#endif /* !INETSTL_INCL_H_INETSTL_FILESYSTEM_TRAITS */
#ifndef STLSOFT_INCL_H_STLSOFT_SIMPLE_STRING
//# include "stlsoft_simple_string.h"       // stlsoft::basic_simple_string
#endif /* !STLSOFT_INCL_H_STLSOFT_SIMPLE_STRING */
#ifndef __STLSOFT_COMPILER_IS_WATCOM
# ifndef STLSOFT_INCL_H_STLSOFT_ITERATOR
#  include "stlsoft_iterator.h"         // iterator_base
# endif /* !STLSOFT_INCL_H_STLSOFT_ITERATOR */
#endif /* __STLSOFT_COMPILER_IS_WATCOM */

/* /////////////////////////////////////////////////////////////////////////////
 * Namespace
 */

#ifndef _INETSTL_NO_NAMESPACE
# if defined(_STLSOFT_NO_NAMESPACE) || \
     defined(__STLSOFT_DOCUMENTATION_SKIP_SECTION)
/* There is no stlsoft namespace, so must define ::inetstl */
namespace inetstl
{
# else
/* Define stlsoft::inetstl_project */

namespace stlsoft
{

namespace inetstl_project
{

# endif /* _STLSOFT_NO_NAMESPACE */
#endif /* !_INETSTL_NO_NAMESPACE */

/* ////////////////////////////////////////////////////////////////////////// */

/// \weakgroup libraries STLSoft Libraries
/// \brief The individual libraries

/// \weakgroup libraries_filesystem File-System Library
/// \ingroup libraries
/// \brief This library provides facilities for defining and manipulating file-system objects

/// \weakgroup inetstl_filesystem_library File-System Library (InetSTL)
/// \ingroup InetSTL libraries_filesystem
/// \brief This library provides facilities for defining and manipulating file-system objects for the WinInet API
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
    typedef value_type                                                          &reference;
    /// The non-mutable (const) reference type
    typedef value_type const                                                    &const_reference;
    /// The find-data type
    typedef ss_typename_type_k traits_type::find_data_type                      find_data_type;
    /// The difference type
    typedef is_ptrdiff_t                                                        difference_type;
    /// The size type
    typedef is_size_t                                                           size_type;

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
    basic_findfile_sequence(HINTERNET hconn, char_type const *searchSpec, is_int_t flags = directories | files);
    /// Commence a search according to the given search pattern and flags, relative to \c directory
    basic_findfile_sequence(HINTERNET hconn, char_type const *directory, char_type const *searchSpec, is_int_t flags = directories | files);
    /// Destructor
    ~basic_findfile_sequence() inetstl_throw_0();

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
    is_bool_t           empty() const;
    /// Returns the maximum number of items in the sequence
    static size_type    max_size();

// Members
private:
    friend class basic_findfile_sequence_value_type<C, T>;
    friend class basic_findfile_sequence_const_input_iterator<C, T, value_type>;

    HINTERNET const m_hconn;
    char_type       m_directory[_MAX_DIR + 1];
    char_type       m_subpath[_MAX_PATH + 1];
    char_type       m_search[_MAX_PATH + 1];
    is_int_t        m_flags;

// Implementation
private:
    static is_int_t     validate_flags_(is_int_t flags);
    static void         extract_subpath_(HINTERNET hconn, char_type *dest, char_type const *searchSpec);

    static  HINTERNET   find_first_file_(HINTERNET hconn, char_type const *spec, is_int_t flags, find_data_type *findData);
    HINTERNET           begin_(HINTERNET hconn, find_data_type &findData) const;

// Not to be implemented
private:
    basic_findfile_sequence(class_type const &);
    basic_findfile_sequence const &operator =(class_type const &);
};

/* /////////////////////////////////////////////////////////////////////////////
 * Typedefs for commonly encountered types
 */

/// Instantiation of the basic_findfile_sequence template for the ANSI character type \c char
typedef basic_findfile_sequence<is_char_a_t, filesystem_traits<is_char_a_t> >     findfile_sequence_a;
/// Instantiation of the basic_findfile_sequence template for the Unicode character type \c wchar_t
typedef basic_findfile_sequence<is_char_w_t, filesystem_traits<is_char_w_t> >     findfile_sequence_w;
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
private:
//  typedef stlsoft_ns_qual(basic_simple_string)<char_type> string_type;

/// \name Construction
// @{
public:
    /// Default constructor
    basic_findfile_sequence_value_type();
private:
    basic_findfile_sequence_value_type(find_data_type const &data, char_type const *path)
        : m_data(data)
    {
        if('/' != data.cFileName[0])
        {
            traits_type::str_copy(m_path, path);
            traits_type::ensure_dir_end(m_path);
        }
        else
        {
            m_path[0] = '\0';
        }
        traits_type::str_cat(m_path, data.cFileName);
    }
public:
    /// Destructor
    ~basic_findfile_sequence_value_type() inetstl_throw_0();

    /// Copy assignment operator
    class_type &operator =(class_type const &rhs);
// @}

/// Accessors
// @{
public:
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
// @}

// Members
private:
    friend class basic_findfile_sequence_const_input_iterator<C, T, class_type>;

    find_data_type  m_data;
    char_type       m_path[1 + _MAX_PATH];
};

// class basic_findfile_sequence_const_input_iterator
/// Iterator type for the basic_findfile_sequence supporting the Input Iterator concept
template <ss_typename_param_k C, ss_typename_param_k T, ss_typename_param_k V>
class basic_findfile_sequence_const_input_iterator
#ifndef __STLSOFT_COMPILER_IS_WATCOM
    : public stlsoft_ns_qual(iterator_base)<inetstl_ns_qual_std(input_iterator_tag), V, is_ptrdiff_t, void, V>
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
        typedef shared_handle   class_type;

    // Members
    public:
        HINTERNET   hSrch;
    private:
        ss_sint32_t cRefs;

    // Construction
    public:
        ss_explicit_k shared_handle(HINTERNET h)
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
            inetstl_message_assert("Shared search handle being destroyed with outstanding references!", 0 == cRefs);

            if(NULL != hSrch)
            {
                traits_type::find_close(hSrch);
            }
        }

    // Operations
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
    basic_findfile_sequence_const_input_iterator(sequence_type const &l, HINTERNET hSrch, find_data_type const &data)
        : m_list(&l)
        , m_handle(new shared_handle(hSrch))
        , m_data(data)
    {
        inetstl_assert(NULL != hSrch);
    }
    basic_findfile_sequence_const_input_iterator(sequence_type const &l);
public:
    /// Default constructor
    basic_findfile_sequence_const_input_iterator();
    /// <a href = "http://synesis.com.au/resources/articles/cpp/movectors.pdf">Move constructor</a>
    basic_findfile_sequence_const_input_iterator(class_type const &rhs);
    /// Destructor
    ~basic_findfile_sequence_const_input_iterator() inetstl_throw_0();

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
    is_bool_t operator ==(class_type const &rhs) const;
    /// Evaluates whether \c this and \c rhs are not equivalent
    is_bool_t operator !=(class_type const &rhs) const;

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
inline is_bool_t is_empty(basic_findfile_sequence<C, T> const &s)
{
    return s.empty();
}

////////////////////////////////////////////////////////////////////////////////
// Unit-testing

#ifdef STLSOFT_UNITTEST

namespace unittest
{
    ss_bool_t test_inetstl_findfile_sequence(unittest_reporter *r)
    {
        using stlsoft::unittest::unittest_initialiser;

        ss_bool_t               bSuccess    =   true;

        unittest_initialiser    init(r, "InetSTL", "findfile_sequence", __FILE__);

#if 0
        if(<<TODO>>)
        {
            r->report("<<TODO>> failed ", __LINE__);
            bSuccess = false;
        }
#endif /* 0 */

        return bSuccess;
    }

    unittest_registrar    unittest_inetstl_findfile_sequence(test_inetstl_findfile_sequence);

} // namespace unittest

#endif /* STLSOFT_UNITTEST */

///////////////////////////////////////////////////////////////////////////////
// Implementation

#ifndef __STLSOFT_DOCUMENTATION_SKIP_SECTION

// basic_findfile_sequence

template <ss_typename_param_k C, ss_typename_param_k T>
inline /* static */ HINTERNET basic_findfile_sequence<C, T>::find_first_file_(HINTERNET hconn, ss_typename_type_k basic_findfile_sequence<C, T>::char_type const *spec, is_int_t /* flags */, ss_typename_type_k basic_findfile_sequence<C, T>::find_data_type *findData)
{
    HINTERNET   hSrch   =   traits_type::find_first_file(hconn, spec, findData);

#ifdef _DEBUG
    if(NULL == hSrch)
    {
        ::GetLastError();
    }
#endif /* _DEBUG */

    return hSrch;
}

template <ss_typename_param_k C, ss_typename_param_k T>
inline HINTERNET basic_findfile_sequence<C, T>::begin_(HINTERNET hconn, ss_typename_type_k basic_findfile_sequence<C, T>::find_data_type &findData) const
{
    HINTERNET   hSrch = find_first_file_(hconn, m_search, m_flags, &findData);

    if(hSrch != NULL)
    {
        // Now need to validate against the flags

        for(; hSrch != NULL; )
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
                traits_type::find_close(hSrch);

                hSrch = NULL;

                break;
            }
        }
    }

    return hSrch;
}

template <ss_typename_param_k C, ss_typename_param_k T>
inline /* static */ is_int_t basic_findfile_sequence<C, T>::validate_flags_(is_int_t flags)
{
    return (flags & (directories | files)) == 0 ? (flags | (directories | files)) : flags;
}

template <ss_typename_param_k C, ss_typename_param_k T>
inline /* static */ void basic_findfile_sequence<C, T>::extract_subpath_(HINTERNET hconn, char_type *dest, char_type const *searchSpec)
{
    char_type   *pFile;

    traits_type::get_full_path_name(hconn, searchSpec, _MAX_PATH, dest, &pFile);

    if(pFile != 0)
    {
        *pFile = '\0';
    }
}

// Construction
template <ss_typename_param_k C, ss_typename_param_k T>
inline basic_findfile_sequence<C, T>::basic_findfile_sequence(HINTERNET hconn, char_type const *searchSpec, is_int_t flags /* = directories | files */)
    : m_hconn(hconn)
    , m_flags(validate_flags_(flags))
{
    is_size_t   cchBuffer   =   stlsoft_num_elements(m_directory);

    traits_type::get_current_directory(m_hconn, cchBuffer, m_directory);

    traits_type::str_copy(m_search, m_directory);
    traits_type::ensure_dir_end(m_search);
    traits_type::str_cat(m_search, searchSpec);

    extract_subpath_(m_hconn, m_subpath, searchSpec);
}

template <ss_typename_param_k C, ss_typename_param_k T>
inline basic_findfile_sequence<C, T>::basic_findfile_sequence(HINTERNET hconn, char_type const *directory, char_type const * searchSpec, is_int_t flags /* = directories | files */)
    : m_hconn(hconn)
    , m_flags(validate_flags_(flags))
{
    traits_type::str_copy(m_directory, directory);

    traits_type::str_copy(m_search, directory);
    if( searchSpec[0] == '.' &&
        searchSpec[1] == '\0')
    {
        char_type   slashdot[3] = { '/', '.', '\0' };

        // This is needed to prevent a searchSpec of "." from stipulating the name twice
        traits_type::remove_dir_end(m_search);
        traits_type::str_cat(m_search, slashdot);
    }
    else
    {
        traits_type::ensure_dir_end(m_search);
        traits_type::str_cat(m_search, searchSpec);
    }

    extract_subpath_(m_hconn, m_subpath, m_search);
}

template <ss_typename_param_k C, ss_typename_param_k T>
inline basic_findfile_sequence<C, T>::~basic_findfile_sequence() inetstl_throw_0()
{}

// Iteration
template <ss_typename_param_k C, ss_typename_param_k T>
inline ss_typename_type_k basic_findfile_sequence<C, T>::const_iterator basic_findfile_sequence<C, T>::begin() const
{
    ss_typename_type_k traits_type::find_data_type  findData;
    HINTERNET                                       hSrch   =   begin_(m_hconn, findData);

    if(hSrch == NULL)
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
    return m_directory;
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
inline is_bool_t basic_findfile_sequence<C, T>::empty() const
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


template <ss_typename_param_k C, ss_typename_param_k T>
inline basic_findfile_sequence_value_type<C, T>::~basic_findfile_sequence_value_type() inetstl_throw_0()
{}

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
    return m_path;
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
inline is_bool_t operator == (basic_findfile_sequence_value_type<C, T> const &lhs, basic_findfile_sequence_value_type<C, T> const &rhs)
{
    return 0 == basic_findfile_sequence_value_type<C, T>::traits_type::str_compare(lhs.get_path(), rhs.get_path());
}

template <ss_typename_param_k C, ss_typename_param_k T>
inline is_bool_t operator == (basic_findfile_sequence_value_type<C, T> const &lhs, C const *rhs)
{
    return 0 == basic_findfile_sequence_value_type<C, T>::traits_type::str_compare(lhs.get_path(), rhs);
}

template <ss_typename_param_k C, ss_typename_param_k T>
inline is_bool_t operator == (C const *lhs, basic_findfile_sequence_value_type<C, T> const &rhs)
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
    inetstl_message_assert("Assigning iterators from separate sequences", m_list == rhs.m_list);    // Should only be comparing iterators from same container

    m_handle    =   rhs.m_handle;
    m_data      =   rhs.m_data;

    if(NULL != m_handle)
    {
        m_handle->AddRef();
    }

    return *this;
}

template <ss_typename_param_k C, ss_typename_param_k T, ss_typename_param_k V>
inline basic_findfile_sequence_const_input_iterator<C, T, V>::~basic_findfile_sequence_const_input_iterator() inetstl_throw_0()
{
    if(NULL != m_handle)
    {
        m_handle->Release();
    }
}

template <ss_typename_param_k C, ss_typename_param_k T, ss_typename_param_k V>
inline ss_typename_type_k basic_findfile_sequence_const_input_iterator<C, T, V>::class_type &basic_findfile_sequence_const_input_iterator<C, T, V>::operator ++()
{
    is_int_t    flags   =   m_list->m_flags;

    inetstl_message_assert("Attempting to increment an invalid iterator!", NULL != m_handle);

    for(; m_handle->hSrch != NULL; )
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
        return value_type(m_data, m_list->m_subpath);
    }
    else
    {
        inetstl_message_assert("Dereferencing end()-valued iterator", 0);

        return value_type();
    }
}

template <ss_typename_param_k C, ss_typename_param_k T, ss_typename_param_k V>
inline is_bool_t basic_findfile_sequence_const_input_iterator<C, T, V>::operator ==(class_type const &rhs) const
{
    is_bool_t    eq;

    // Should only be comparing iterators from same container
    inetstl_message_assert("Comparing iterators from separate sequences", m_list == rhs.m_list);

    // Not equal if one but not both handles is the NULL
    // or if the data is not equal.
    if( (NULL == m_handle) != (NULL == rhs.m_handle) ||
        (   NULL != m_handle &&
            traits_type::str_compare(m_data.cFileName, rhs.m_data.cFileName) != 0))
    {
        eq = is_false_v;
    }
    else
    {
        eq = is_true_v;
    }

    return eq;
}

template <ss_typename_param_k C, ss_typename_param_k T, ss_typename_param_k V>
inline is_bool_t basic_findfile_sequence_const_input_iterator<C, T, V>::operator !=(class_type const &rhs) const
{
    return ! operator ==(rhs);
}

#endif /* !__STLSOFT_DOCUMENTATION_SKIP_SECTION */

/* ////////////////////////////////////////////////////////////////////////// */

/// @} // end of group inetstl_filesystem_library

/* ////////////////////////////////////////////////////////////////////////// */

#ifndef _INETSTL_NO_NAMESPACE
# if defined(_STLSOFT_NO_NAMESPACE) || \
     defined(__STLSOFT_DOCUMENTATION_SKIP_SECTION)
} // namespace inetstl
# else
} // namespace inetstl_project
} // namespace stlsoft
# endif /* _STLSOFT_NO_NAMESPACE */
#endif /* !_INETSTL_NO_NAMESPACE */

/* ////////////////////////////////////////////////////////////////////////// */

#endif /* INETSTL_INCL_H_INETSTL_FINDFILE_SEQUENCE */

/* ////////////////////////////////////////////////////////////////////////// */
