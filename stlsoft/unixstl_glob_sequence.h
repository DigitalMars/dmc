/* /////////////////////////////////////////////////////////////////////////////
 * File:        unixstl_glob_sequence.h (formerly unixstl_findfile_sequence.h)
 *
 * Purpose:     glob_sequence class.
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


/// \file unixstl_glob_sequence.h
///
/// glob_sequence class.

#ifndef UNIXSTL_INCL_H_UNIXSTL_GLOB_SEQUENCE
#define UNIXSTL_INCL_H_UNIXSTL_GLOB_SEQUENCE

#ifndef __STLSOFT_DOCUMENTATION_SKIP_SECTION
# define UNIXSTL_VER_H_UNIXSTL_GLOB_SEQUENCE_MAJOR      3
# define UNIXSTL_VER_H_UNIXSTL_GLOB_SEQUENCE_MINOR      0
# define UNIXSTL_VER_H_UNIXSTL_GLOB_SEQUENCE_REVISION   1
# define UNIXSTL_VER_H_UNIXSTL_GLOB_SEQUENCE_EDIT       77
#endif /* !__STLSOFT_DOCUMENTATION_SKIP_SECTION */

/* /////////////////////////////////////////////////////////////////////////////
 * Includes
 */

#ifndef UNIXSTL_INCL_H_UNIXSTL
# include "unixstl.h"                   // Include the UNIXSTL root header
#endif /* !UNIXSTL_INCL_H_UNIXSTL */
#ifndef UNIXSTL_INCL_H_UNIXSTL_FILESYSTEM_TRAITS
# include "unixstl_filesystem_traits.h" // filesystem_traits
#endif /* !UNIXSTL_INCL_H_UNIXSTL_FILESYSTEM_TRAITS */
#ifndef UNIXSTL_INCL_H_UNIXSTL_FILE_PATH_BUFFER
# include "unixstl_file_path_buffer.h"  // basic_file_path_buffer
#endif /* !UNIXSTL_INCL_H_UNIXSTL_FILE_PATH_BUFFER */
#ifndef STLSOFT_INCL_H_STLSOFT_ITERATOR
# include "stlsoft_iterator.h"          // pointer_iterator, reverse_iterator_base
#endif /* !STLSOFT_INCL_H_STLSOFT_ITERATOR */
#ifndef STLSOFT_INCL_H_STLSOFT_AUTO_BUFFER
# include "stlsoft_auto_buffer.h"       // auto_buffer
#endif /* !STLSOFT_INCL_H_STLSOFT_AUTO_BUFFER */
#ifndef STLSOFT_INCL_H_STLSOFT_MALLOC_ALLOCATOR
# include "stlsoft_malloc_allocator.h"  // malloc_allocator
#endif /* !STLSOFT_INCL_H_STLSOFT_MALLOC_ALLOCATOR */
#include <sys/types.h>
#include <sys/stat.h>                   // stat()
#include <errno.h>
#include <glob.h>                       // glob(), globfree()
#include <algorithm>                    // std::sort
#include <utility>                      // std::swap

/* /////////////////////////////////////////////////////////////////////////////
 * Namespace
 *
 * The UNIXSTL components are contained within the unixstl namespace. This is
 * actually an alias for stlsoft::unixstl_project,
 *
 * The definition matrix is as follows:
 *
 * _STLSOFT_NO_NAMESPACE    _UNIXSTL_NO_NAMESPACE   unixstl definition
 * ---------------------    ---------------------   -----------------
 *  not defined              not defined             = stlsoft::unixstl_project
 *  not defined              defined                 not defined
 *  defined                  not defined             unixstl
 *  defined                  defined                 not defined
 *
 */

/* No STLSoft namespaces means no UNIXSTL namespaces */
#ifdef _STLSOFT_NO_NAMESPACES
# define _UNIXSTL_NO_NAMESPACES
#endif /* _STLSOFT_NO_NAMESPACES */

/* No UNIXSTL namespaces means no unixstl namespace */
#ifdef _UNIXSTL_NO_NAMESPACES
# define _UNIXSTL_NO_NAMESPACE
#endif /* _UNIXSTL_NO_NAMESPACES */

#ifndef _UNIXSTL_NO_NAMESPACE
# if defined(_STLSOFT_NO_NAMESPACE) || \
     defined(__STLSOFT_DOCUMENTATION_SKIP_SECTION)
/* There is no stlsoft namespace, so must define ::unixstl */
namespace unixstl
{
# else
/* Define stlsoft::unixstl_project */

namespace stlsoft
{

namespace unixstl_project
{

# endif /* _STLSOFT_NO_NAMESPACE */
#endif /* !_UNIXSTL_NO_NAMESPACE */

/* ////////////////////////////////////////////////////////////////////////// */

/// \weakgroup libraries STLSoft Libraries
/// \brief The individual libraries

/// \weakgroup libraries_filesystem File-System Library
/// \ingroup libraries
/// \brief This library provides facilities for defining and manipulating file-system objects

/// \defgroup unixstl_filesystem_library File-System Library (UNIXSTL)
/// \ingroup UNIXSTL libraries_filesystem
/// \brief This library provides facilities for defining and manipulating UNIX file-system objects
/// @{

/* /////////////////////////////////////////////////////////////////////////////
 * Classes
 */

#ifdef __STLSOFT_CF_EXCEPTION_SUPPORT
/// \brief The exception-type thrown by the glob_sequence
///
///
class glob_sequence_exception
#if defined(__STLSOFT_COMPILER_IS_DMC)
    : public std::exception
#else /* ? compiler */
    : public unixstl_ns_qual_std(exception)
#endif /* compiler */
{
/// \name Types
/// @{
public:
#if defined(__STLSOFT_COMPILER_IS_DMC)
    typedef std::exception                  parent_class_type;
#else /* ? compiler */
    typedef unixstl_ns_qual_std(exception)  parent_class_type;
#endif /* compiler */
    typedef glob_sequence_exception         class_type;
/// @}

/// \name Construction
/// @{
public:
    ss_explicit_k glob_sequence_exception(us_int_t globStatus, us_int_t errno_) unixstl_throw_0()
        : m_globStatus(globStatus)
        , m_errno(errno_)
    {}
/// @}

/// \name Accessors
/// @{
public:
#if defined(__STLSOFT_COMPILER_IS_DMC)
    char const  *what() const throw()
#else /* ? compiler */
    char const  *what() const unixstl_throw_0()
#endif /* compiler */
    {
        return "glob_sequence failure";
    }

    us_int_t get_globstatus() const unixstl_throw_0()
    {
        return m_globStatus;
    }
    us_int_t get_errno() const unixstl_throw_0()
    {
        return m_errno;
    }
/// @}

// Members
private:
    us_int_t const  m_globStatus;
    us_int_t const  m_errno;

// Not to be implemented
private:
    class_type &operator =(class_type const &);
};

#endif /* __STLSOFT_CF_EXCEPTION_SUPPORT */

/// \brief STL-like readonly sequence based on the results of file-system wildcard matches
///
/// This class presents and STL-like readonly sequence interface to allow the
/// iteration over the results of file-system wildcard matches.

class glob_sequence
{
/// \name Types
/// @{
public:
    // This class
    typedef glob_sequence                                                   class_type;
    /// The char type
    typedef us_char_a_t                                                     char_type;
    // The traits type
    typedef filesystem_traits<char_type>                                    traits_type;
    /// The value type
    typedef char_type const                                                 *value_type;
    /// The non-mutating (const) reference type
    typedef value_type const                                                &const_reference;
    /// The non-mutating (const) pointer type
    typedef value_type const                                                *const_pointer;
    /// The size type
    typedef us_size_t                                                       size_type;
    /// The difference type
    typedef us_ptrdiff_t                                                    difference_type;

    /// The non-mutating (const) iterator type
    typedef stlsoft_ns_qual(pointer_iterator)<   value_type const
                                             ,   const_pointer
                                             ,   const_reference
                                             >::iterator_type               const_iterator;

#ifdef __STLSOFT_CF_BIDIRECTIONAL_ITERATOR_SUPPORT
    /// The type of the const (non-mutating) reverse iterator
    typedef stlsoft_ns_qual(reverse_iterator_base)  <   const_iterator
                                                    ,   value_type
                                                    ,   const_reference
                                                    ,   const_pointer
                                                    ,   difference_type
                                                    >                       const_reverse_iterator;
#endif /* __STLSOFT_CF_BIDIRECTIONAL_ITERATOR_SUPPORT */
/// @}

/// \name Flags
/// @{
public:
    enum
    {
            includeDots     =   0x0008  /*!< Requests that dots directories be included in the returned sequence */
        ,   directories     =   0x0010  /*!< Causes the search to include directories */
        ,   files           =   0x0020  /*!< Causes the search to include files */
        ,   noSort          =   0x0100  /*!< Does not sort entries */
        ,   markDirs        =   0x0200  /*!< Mark directories with a trailing path name separator */
        ,   absolutePath    =   0x0400  /*!< Return all entries in absolute format */
        ,
    };
/// @}

/// \name Construction
/// @{
public:
    /// \brief Constructs a sequence according to the given criteria
    ///
    /// The constructor initialises a glob_sequence instance on the given
    /// pattern with the given flags.
    ///
    /// \param pattern The pattern against which to match the file-system contents
    /// \param flags Flags to alter the behaviour of the search
    ///
    /// \note If exceptions are supported, then this will throw a glob_sequence_exception
    /// on failure of any underlying functions
    ss_explicit_k glob_sequence(char_type const *pattern, us_uint_t flags = noSort);

    /// \brief Constructs a sequence according to the given criteria
    ///
    /// The constructor initialises a glob_sequence instance on the given
    /// pattern with the given flags.
    ///
    /// \param directory The directory in which the pattern is located
    /// \param pattern The pattern against which to match the file-system contents
    /// \param flags Flags to alter the behaviour of the search
    ///
    /// \note If exceptions are supported, then this will throw a glob_sequence_exception
    /// on failure of any underlying functions
    glob_sequence(char_type const *directory, char_type const *pattern, us_uint_t flags = noSort);

    // Releases any acquired resources
    ~glob_sequence() unixstl_throw_0();
/// @}

/// \name Attributes
/// @{
public:
    /// Returns the number of elements in the sequence
    us_size_t size() const;

    /// \brief Indicates whether the search sequence is empty
    us_bool_t empty() const;

    /// \brief Returns the value corresponding to the given index
    ///
    /// \note In debug-mode a runtime assert is applied to enforce that the index is valid. There is <b>no</b> release-time checking on the index validity!
    const value_type operator [](size_type index) const;
/// @}

/// \name Iteration
/// @{
public:
    /// Begins the iteration
    ///
    /// \return An iterator representing the start of the sequence
    const_iterator  begin() const;
    /// Ends the iteration
    ///
    /// \return An iterator representing the end of the sequence
    const_iterator  end() const;

#ifdef __STLSOFT_CF_BIDIRECTIONAL_ITERATOR_SUPPORT
    /// Begins the reverse iteration
    ///
    /// \return An iterator representing the start of the reverse sequence
    const_reverse_iterator  rbegin() const;
    /// Ends the reverse iteration
    ///
    /// \return An iterator representing the end of the reverse sequence
    const_reverse_iterator  rend() const;
#endif /* __STLSOFT_CF_BIDIRECTIONAL_ITERATOR_SUPPORT */
/// @}

// Implementation
private:
    static us_uint_t validate_flags_(us_uint_t flags);

    // Returns true if pch == "" or "/" (or "\\"), false otherwise
    static us_bool_t is_end_of_path_elements_(char_type const *pch, difference_type index);

    static us_bool_t is_dots_maybe_slashed_(char_type const *s, us_bool_t &bTwoDots);

    us_size_t init_glob_(char_type const *directory, char_type const *pattern);

// Members
private:
    typedef stlsoft_ns_qual(auto_buffer)<char_type*, stlsoft_ns_qual(malloc_allocator)<char_type*>, 32> buffer_type;

    us_uint_t const m_flags;
    char_type const **m_base;
    us_size_t       m_cItems;
    buffer_type     m_buffer;
    glob_t          m_glob;

// Not to be implemented
private:
    glob_sequence(class_type const &);
    class_type const &operator =(class_type const &);
};

////////////////////////////////////////////////////////////////////////////////
// Unit-testing

#ifdef STLSOFT_UNITTEST

namespace unittest
{
    ss_bool_t test_unixstl_glob_sequence(unittest_reporter *r)
    {
        using stlsoft::unittest::unittest_initialiser;

        ss_bool_t               bSuccess    =   true;

        unittest_initialiser    init(r, "UNIXSTL", "glob_sequence", __FILE__);

#if defined(WIN32) || \
    defined(_WIN32)
        glob_sequence   seq("*.*");
#else /* ? WIN32 */
        glob_sequence   seq("*");
#endif /* WIN32 */

        if(seq.empty() != (0 == seq.size()))
        {
            r->report("glob_sequence empty() and size() contradict ", __LINE__);
            bSuccess = false;
        }

        us_size_t   total_forward   =   0;
        us_size_t   total_backward  =   0;

        glob_sequence::const_iterator           b_f =   seq.begin();
        for(; b_f != seq.end(); ++b_f)
        {
            total_forward += strlen(*b_f);
        }

        glob_sequence::const_reverse_iterator   b_b =   seq.rbegin();
        for(; b_b != seq.rend(); ++b_b)
        {
            total_backward += strlen(*b_b);
        }

        if(total_forward != total_backward)
        {
            r->report("glob_sequence forward and backward ranges contradict ", __LINE__);
            bSuccess = false;
        }

        return bSuccess;
    }

    unittest_registrar    unittest_unixstl_glob_sequence(test_unixstl_glob_sequence);

} // namespace unittest

#endif /* STLSOFT_UNITTEST */

/* /////////////////////////////////////////////////////////////////////////////
 * Implementation
 */

inline /* ss_explicit_k */ glob_sequence::glob_sequence(glob_sequence::char_type const *pattern, us_uint_t flags /* = noSort */)
    : m_flags(validate_flags_(flags))
    , m_buffer(1)
{
    m_cItems = init_glob_(NULL, pattern);

    unixstl_assert((0 == m_cItems) || (NULL != m_base));
}

inline glob_sequence::glob_sequence(glob_sequence::char_type const *directory, glob_sequence::char_type const *pattern, us_uint_t flags /* = noSort */)
    : m_flags(validate_flags_(flags))
    , m_buffer(1)
{
    m_cItems = init_glob_(directory, pattern);

    unixstl_assert((0 == m_cItems) || (NULL != m_base));
}

inline glob_sequence::~glob_sequence() unixstl_throw_0()
{
    unixstl_assert((0 == m_cItems) || (NULL != m_base));

    if(NULL != m_base)
    {
        globfree(&m_glob);
    }
}

inline us_size_t glob_sequence::size() const
{
    return m_cItems;
}

inline us_bool_t glob_sequence::empty() const
{
    return 0 == size();
}

inline glob_sequence::value_type const glob_sequence::operator [](glob_sequence::size_type index) const
{
    unixstl_message_assert("index access out of range in glob_sequence", index < 1 + size());   // Has to be +1, since legitimate to take address of one-past-the-end

    return m_base[index];
}

inline glob_sequence::const_iterator glob_sequence::begin() const
{
    return m_base;
}

inline glob_sequence::const_iterator glob_sequence::end() const
{
    return m_base + m_cItems;
}

#ifdef __STLSOFT_CF_BIDIRECTIONAL_ITERATOR_SUPPORT
inline glob_sequence::const_reverse_iterator glob_sequence::rbegin() const
{
    return const_reverse_iterator(end());
}

inline glob_sequence::const_reverse_iterator glob_sequence::rend() const
{
    return const_reverse_iterator(begin());
}
#endif /* __STLSOFT_CF_BIDIRECTIONAL_ITERATOR_SUPPORT */

inline /* static */ us_uint_t glob_sequence::validate_flags_(us_uint_t flags)
{
    if(0 == (flags & (directories | files)))
    {
        flags |= (directories | files);
    }

    if(0 == (flags & directories))
    {
#ifndef UNIXSTL_GLOB_SEQUENCE_ULTRA_CAUTIOUS
        // It's more efficient to not bother doing a separate dots check if all
        // directories are being elided.
        flags |= includeDots;

        // Since we're not going to be returning directories to the caller, and
        // it's more efficient to believe the glob() directory marking rather
        // than calling stat, we add the markDirs flag here.
        flags |= markDirs;
#endif /* UNIXSTL_GLOB_SEQUENCE_ULTRA_CAUTIOUS */
    }

    return flags;
}

inline /* static */ us_bool_t glob_sequence::is_end_of_path_elements_(glob_sequence::char_type const *pch, glob_sequence::difference_type index)
{
    return  pch[index] == '\0' ||
            (   pch[index + 1] == '\0' &&
                (
#if defined(_UNIXSTL_COMPILER_IS_UNKNOWN) && \
    !defined(_UNIXSTL_GLOB_SEQUENCE_NO_BACK_SLASH_TERMINATOR)
                    pch[index] == '\\' || /* Allow for testing on Win32 systems */
#endif /* _UNIXSTL_COMPILER_IS_UNKNOWN && !_UNIXSTL_GLOB_SEQUENCE_NO_BACK_SLASH_TERMINATOR */
                    pch[index] == '/'));
}

inline /* static */ us_bool_t glob_sequence::is_dots_maybe_slashed_(glob_sequence::char_type const *s, us_bool_t &bTwoDots)
{
    unixstl_assert(NULL != s);

    return  s[0] == '.' &&
            (   (bTwoDots = false, is_end_of_path_elements_(s, 1)) ||
                (bTwoDots = true, ( s[1] == '.' &&
                                    is_end_of_path_elements_(s, 2))));
}

inline us_size_t glob_sequence::init_glob_(glob_sequence::char_type const *directory, glob_sequence::char_type const *pattern)
{
    unixstl_message_assert("Null pattern given to glob_sequence", NULL != pattern);

    us_uint_t                           glob_flags  =   0;
    basic_file_path_buffer<char_type>   scratch_;   // Scratch buffer for directory / pattern

#ifndef __STLSOFT_CF_EXCEPTION_SUPPORT
    if(0 == scratch_.size())
    {
        m_base = NULL;

        return 0;
    }
#endif /* !__STLSOFT_CF_EXCEPTION_SUPPORT */

    // If a directory is given, then ...
    if( NULL != directory &&
        '\0' != *directory)
    {
        // ... optionally turn it into an absolute directory, ...
        if(absolutePath == (m_flags & absolutePath))
        {
            traits_type::get_full_path_name(directory, scratch_.size(), &scratch_[0]);
        }
        else
        {
            traits_type::str_copy(&scratch_[0], directory);
        }

        // ... ensure that it has a trailing path name-separator, and ...
        traits_type::ensure_dir_end(&scratch_[0]);

        // ... prefix directory onto pattern.
        traits_type::str_cat(&scratch_[0], pattern);

        pattern = c_str_ptr(scratch_);
    }

    if(m_flags & noSort)
    {
        // Don't bother sorting
        glob_flags |= GLOB_NOSORT;
    }

    if(m_flags & markDirs)
    {
        // Ask for trailing slashes on directories
        glob_flags |= GLOB_MARK;
    }

    if(directories == (m_flags & (directories | files)))
    {
        // Ask for only directories
        glob_flags |= GLOB_ONLYDIR;
    }

    int gr = glob(pattern, glob_flags, NULL, &m_glob);

    if(0 != gr)
    {
#ifdef __STLSOFT_CF_EXCEPTION_SUPPORT
        if(GLOB_NOMATCH != gr)
        {
            throw glob_sequence_exception(gr, 0);
        }
#endif /* __STLSOFT_CF_EXCEPTION_SUPPORT */

        m_base = NULL;

        return 0;
    }
    else
    {
        char_type   **base  =   m_glob.gl_pathv;
        us_size_t   cItems  =   static_cast<us_size_t>(m_glob.gl_pathc);

        // If we are eliding dots, or trimming out directories, then
        // we'll copy the pointers into the buffer, and process them
        // there
        if( 0 == (m_flags & includeDots) ||
#ifdef UNIXSTL_GLOB_SEQUENCE_ULTRA_CAUTIOUS
            (m_flags & (directories | files)) != (directories | files))
#else /* ? UNIXSTL_GLOB_SEQUENCE_ULTRA_CAUTIOUS */
            (m_flags & (directories | files)) == files)
#endif /* UNIXSTL_GLOB_SEQUENCE_ULTRA_CAUTIOUS */
        {
#ifdef __STLSOFT_CF_EXCEPTION_SUPPORT
            try
            {
                m_buffer.resize(cItems);
            }
            catch(...)
            {
                globfree(&m_glob);

                throw;
            }
#else /* __STLSOFT_CF_EXCEPTION_SUPPORT */
            if(!m_buffer.resize(cItems))
            {
                globfree(&m_glob);

                m_base = NULL;

                return 0;
            }
#endif /* __STLSOFT_CF_EXCEPTION_SUPPORT */

            unixstl_assert(m_buffer.size() == cItems);

            base = static_cast<char_type**>(memcpy(&m_buffer[0], base, m_buffer.size() * sizeof(char_type*)));
        }

        if(0 == (m_flags & includeDots))
        {
            // Now remove the dots. If located at the start of
            // the gl buffer, then simply increment m_base to
            // be above that. If not then rearrange the base
            // two pointers such that they are there.

            us_bool_t   foundDot1   =   false;
            us_bool_t   foundDot2   =   false;
            char_type   **begin     =   base;
            char_type   **end       =   begin + cItems;

            for(; begin != end; ++begin)
            {
                us_bool_t   bTwoDots = false;

                if(is_dots_maybe_slashed_(*begin, bTwoDots))
                {
                    // Swap with whatever is at base[0]
                    if(begin != base)
                    {
                        unixstl_ns_qual_std(swap)(*begin, *base);
                    }
                    ++base;
                    --cItems;

#ifndef UNIXSTL_GLOB_SEQUENCE_ULTRA_CAUTIOUS
                     // We're only going to get one "." and one ".."
                    (bTwoDots ? foundDot2 : foundDot1) = true;

                    if( foundDot1 &&
                        foundDot2)
                    {
                        break;
                    }
#endif /* !UNIXSTL_GLOB_SEQUENCE_ULTRA_CAUTIOUS */
                }
            }
        }

        // We should be able to trust glob() to return only directories when
        // asked, so we assume the following only needs to be done when
        // have asked for files alone
#ifdef UNIXSTL_GLOB_SEQUENCE_ULTRA_CAUTIOUS
        if((m_flags & (directories | files)) != (directories | files))
#else /* ? UNIXSTL_GLOB_SEQUENCE_ULTRA_CAUTIOUS */
        if((m_flags & (directories | files)) == files)
#endif /* UNIXSTL_GLOB_SEQUENCE_ULTRA_CAUTIOUS */
        {
            basic_file_path_buffer<char_type>   buffer;
            char_type                           **begin =   base;
            char_type                           **end   =   begin + cItems;

#ifndef __STLSOFT_CF_EXCEPTION_SUPPORT
            if(0 == buffer.size())
            {
                globfree(&m_glob);

                m_base = NULL;

                return 0;
            }
#endif /* !__STLSOFT_CF_EXCEPTION_SUPPORT */

            for(; begin != end; ++begin)
            {
                // Now need to process the file, by using stat
                struct stat     st;
                int             res;
                char_type const *entry  =   *begin;

// TODO: If not ultra-cautious, and asked for markDirs, then trust
#ifndef UNIXSTL_GLOB_SEQUENCE_ULTRA_CAUTIOUS
                unixstl_assert(files == (m_flags & (directories | files)));

                if( 0 == (m_flags & markDirs) ||
                    !traits_type::has_dir_end(entry))
#endif /* !UNIXSTL_GLOB_SEQUENCE_ULTRA_CAUTIOUS */
                {
                    if(markDirs == (m_flags & markDirs))
                    {
                        traits_type::str_copy(&buffer[0], entry);
                        traits_type::remove_dir_end(&buffer[0]);
#if defined(__STLSOFT_COMPILER_IS_GCC) && \
    __GNUC__ < 3
                        entry = buffer.c_str();
#else /* ? __GNUC__ < 3 */
                        entry = stlsoft_ns_qual(c_str_ptr)(buffer);
#endif /* ? __GNUC__ < 3 */
                    }
                    res = stat(entry, &st);

                    if(0 != res)
                    {
                        // We could throw an exception here, but it might just be
                        // the case that a file has been deleted subsequent to its
                        // having been included in the glob list. As such, it makes
                        // more sense to just kick it from the list

#if 0   // Can't decide on unequivocal list of error codes to exempt (exacerbated
        // by the fact that there are different ones of different systems) so
        // just kick all erroneous entries
#ifdef __STLSOFT_CF_EXCEPTION_SUPPORT
                        switch(errno)
                        {
                            case    ENOENT:
                            case    ENOTDIR:
# if defined(EACCESS)
                            case    EACCESS:
# endif /* EACCESS */
                                break;
                            default:
                                globfree(&m_glob);
                                throw glob_sequence_exception(0, errno);
                                break;
                        }
#endif /* __STLSOFT_CF_EXCEPTION_SUPPORT */
#endif /* 0 */
                    }
                    else
                    {
#ifdef UNIXSTL_GLOB_SEQUENCE_ULTRA_CAUTIOUS
                        if(m_flags & directories) // Want directories
                        {
                            if(S_IFDIR == (st.st_mode & S_IFDIR))
                            {
                                continue; // A directory, so accept it
                            }
                        }
#endif /* UNIXSTL_GLOB_SEQUENCE_ULTRA_CAUTIOUS */
                        if(m_flags & files) // Want files
                        {
                            if(S_IFREG == (st.st_mode & S_IFREG))
                            {
                                continue; // A file, so accept it
                            }
                        }
                    }
                }

                // Note that there is no test here to determine whether or not
                // begin == base. It is assumed that most cases of file elision
                // will involve several files - how many directories have just
                // one file in them? - so the test would actually be a
                // pessimisation

                // Swap with whatever is at base[0]
                unixstl_ns_qual_std(swap)(*begin, *base);
                ++base;
                --cItems;
            }
        }

        // Ensure we've not corrupted the sort order
        if( 0 == (m_flags & noSort) &&
            cItems != static_cast<us_size_t>(m_glob.gl_pathc))
        {
            unixstl_ns_qual_std(sort)(base, base + cItems);
        }

        // Set m_base and m_cItems to the correct values, with
        // or without dots. m_base is cast here to remove the
        // need for const-casting throughout the rest of the
        // class
        m_base = const_cast<char_type const **>(base);

        return cItems;
    }
}

/* ////////////////////////////////////////////////////////////////////////// */

/// @} // end of group unixstl_filesystem_library

/* ////////////////////////////////////////////////////////////////////////// */

#ifndef _UNIXSTL_NO_NAMESPACE
# if defined(_STLSOFT_NO_NAMESPACE) || \
     defined(__STLSOFT_DOCUMENTATION_SKIP_SECTION)
} // namespace unixstl
# else
} // namespace unixstl_project
} // namespace stlsoft
# endif /* _STLSOFT_NO_NAMESPACE */
#endif /* !_UNIXSTL_NO_NAMESPACE */

/* ////////////////////////////////////////////////////////////////////////// */

#endif /* !UNIXSTL_INCL_H_UNIXSTL_GLOB_SEQUENCE */

/* ////////////////////////////////////////////////////////////////////////// */
