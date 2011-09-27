/* /////////////////////////////////////////////////////////////////////////////
 * File:        unixstl_readdir_sequence.h
 *
 * Purpose:     readdir_sequence class.
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


/// \file unixstl_readdir_sequence.h
///
/// readdir_sequence class.

#ifndef UNIXSTL_INCL_H_UNIXSTL_READDIR_SEQUENCE
#define UNIXSTL_INCL_H_UNIXSTL_READDIR_SEQUENCE

#ifndef __STLSOFT_DOCUMENTATION_SKIP_SECTION
# define UNIXSTL_VER_H_UNIXSTL_READDIR_SEQUENCE_MAJOR       2
# define UNIXSTL_VER_H_UNIXSTL_READDIR_SEQUENCE_MINOR       0
# define UNIXSTL_VER_H_UNIXSTL_READDIR_SEQUENCE_REVISION    1
# define UNIXSTL_VER_H_UNIXSTL_READDIR_SEQUENCE_EDIT        63
#endif /* !__STLSOFT_DOCUMENTATION_SKIP_SECTION */

/* /////////////////////////////////////////////////////////////////////////////
 * Includes
 */

#ifndef UNIXSTL_INCL_H_UNIXSTL
# include "unixstl.h"               // Include the UNIXSTL root header
#endif /* !UNIXSTL_INCL_H_UNIXSTL */
#ifndef UNIXSTL_INCL_H_UNIXSTL_FILESYSTEM_TRAITS
# include "unixstl_filesystem_traits.h" // filesystem_traits
#endif /* !UNIXSTL_INCL_H_UNIXSTL_FILESYSTEM_TRAITS */
#include <unistd.h>
#if defined(PATH_MAX)
# ifndef STLSOFT_INCL_H_STLSOFT_STATIC_STRING
#  include "stlsoft_static_string.h"         // stlsoft::basic_static_string
# endif /* !STLSOFT_INCL_H_STLSOFT_STATIC_STRING */
#else /* ? PATH_MAX */
# ifndef STLSOFT_INCL_H_STLSOFT_SIMPLE_STRING
#  include "stlsoft_simple_string.h"         // stlsoft::basic_simple_string
# endif /* !STLSOFT_INCL_H_STLSOFT_SIMPLE_STRING */
#endif /* !PATH_MAX */
#ifndef STLSOFT_INCL_H_STLSOFT_ITERATOR
# include "stlsoft_iterator.h"
#endif /* !STLSOFT_INCL_H_STLSOFT_ITERATOR */
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>

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

/// \brief STL-like readonly sequence based on directory contents
///
/// This class presents and STL-like readonly sequence interface to allow the
/// iteration over the contents of a directory.

class readdir_sequence
{
public:
    // This class
    typedef readdir_sequence                                    class_type;
private:
    typedef filesystem_traits<us_char_a_t>                      traits_type;
public:
    // The size type
    typedef us_size_t                                           size_type;
    // The non-mutating (const) iterator type
    class                                                       const_iterator;
    // The value type
    typedef struct dirent const                                 *value_type;

//    typedef const_iterator::pointer            pointer;
//    typedef const_iterator::pointer const      const_pointer;
//    typedef const_iterator::reference          reference;
//    typedef const_iterator::reference const    const_reference;

public:
#if defined(PATH_MAX)
    typedef stlsoft_ns_qual(basic_static_string)<   us_char_a_t
                                                ,   PATH_MAX
                                                >               string_type;
#else /* ? PATH_MAX */
    typedef stlsoft_ns_qual(basic_simple_string)<   us_char_a_t
                                                >               string_type;
#endif /* !PATH_MAX */

public:
    enum
    {
            includeDots =   0x0008  /*!< Requests that dots directories be included in the returned sequence */
        ,   directories =   0x0010  /*!< Causes the search to include directories */
        ,   files       =   0x0020  /*!< Causes the search to include files */
#ifndef __STLSOFT_DOCUMENTATION_SKIP_SECTION
        ,   noSort      =   0 /* 0x0100 */  //!<
#endif /* !__STLSOFT_DOCUMENTATION_SKIP_SECTION */
        ,
    };

// Construction
public:
    /// \brief Constructs a sequence according to the given criteria
    ///
    /// The constructor initialises a readdir_sequence instance on the given
    /// directory with the given flags.
    ///
    /// \param directory The directory whose contents are to be searched
    /// \param flags Flags to alter the behaviour of the search
    ///
    /// \note The \c flags parameter defaults to <code>directories | files</code> because
    /// this reflects the default behaviour of \c readdir(), and also because it is the
    /// most efficient.
    readdir_sequence(us_char_a_t const *directory, us_uint_t flags = directories | files);

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

// Attributes
public:
#if 0
    /// Returns the number of elements in the sequence
    ///
    /// \note Nor currently implemented
    size_type   size() const
    {
        return 0;
    }
#endif /* 0 */

    /// \brief Indicates whether the search sequence is empty
    us_bool_t empty() const;

    /// \brief The search directory
    ///
    /// \note The value returned by this method always has a trailing path name separator, so
    /// you can safely concatenate this with the value returned by the iterator's operator *()
    /// with minimal fuss.
    string_type const &get_directory() const;

// Implementation
private:
    /// \brief Ensures that the flags are correct
    static us_uint_t validate_flags_(us_uint_t flags);

// Members
private:
    string_type m_directory;
    us_uint_t   m_flags;
};

/// \brief Iterator for readdir_sequence class
///
/// This class performs as a non-mutating iterator (aka const iterator) for the
/// readdir_sequence class.

class readdir_sequence::const_iterator
    : public stlsoft_ns_qual(iterator_base) <   unixstl_ns_qual_std(input_iterator_tag)
                                            ,   readdir_sequence::value_type
                                            ,   us_ptrdiff_t
                                            ,   readdir_sequence::value_type*
                                            ,   readdir_sequence::value_type&
                                            >
{
private:
    typedef readdir_sequence::string_type           string_type;
public:
    /// The class type
    typedef const_iterator                          class_type;
    /// The value type
    typedef readdir_sequence::value_type            value_type;
//    typedef value_type                            *pointer;
//    typedef value_type                            &reference;

// Construction
private:
    friend class readdir_sequence;

    /// Construct an instance and begin a sequence iteration on the given dir.
    const_iterator(DIR *dir, string_type const &directory, us_uint_t flags);
public:
    /// Default constructor
    const_iterator();
    /// Copy constructor
    const_iterator(class_type const &rhs);
    /// Release the search handle
    ~const_iterator() unixstl_throw_0();

    /// Copy assignment operator
    class_type const &operator =(class_type const &rhs);

// Accessors
public:
    /// Returns the value representative
    value_type operator *() const;

    /// Moves the iteration on to the next point in the sequence, or end() if
    /// the sequence is exhausted
    class_type &operator ++();

    /// Post-increment form of operator ++().
    ///
    /// \note Because this version uses a temporary on which to call the
    /// pre-increment form it is thereby less efficient, and should not be used
    /// except where post-increment semantics are required.
    class_type operator ++(int);

    /// Compares \c this for equality with \c rhs
    ///
    /// \param rhs The instance against which to test
    /// \retval true if the iterators are equivalent
    /// \retval false if the iterators are not equivalent
    bool operator ==(class_type const &rhs) const;
    /// Compares \c this for inequality with \c rhs
    ///
    /// \param rhs The instance against which to test
    /// \retval false if the iterators are equivalent
    /// \retval true if the iterators are not equivalent
    bool operator !=(class_type const &rhs) const;

// Members
private:
    struct rds_shared_handle;

    rds_shared_handle   *m_handle;  // The DIR handle, shared with other iterator instances
    struct dirent       *m_entry;   // The current entry
    us_uint_t           m_flags;    // flags. (Only non-const, to allow copy assignment)
    string_type         m_scratch;  // Holds the directory, and is a scratch area
    size_type           m_dirLen;   // The length of the directory
};

#ifndef __STLSOFT_DOCUMENTATION_SKIP_SECTION
struct readdir_sequence::const_iterator::rds_shared_handle
{
public:
    typedef rds_shared_handle   class_type;

// Members
public:
    DIR         *dir;
private:
    ss_sint32_t cRefs;

public:
    ss_explicit_k rds_shared_handle(DIR *d)
        : dir(d)
        , cRefs(1)
    {}
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
#if defined(__STLSOFT_COMPILER_IS_GCC)
protected:
#else /* ? __STLSOFT_COMPILER_IS_GCC */
private:
#endif /* __STLSOFT_COMPILER_IS_GCC */
    ~rds_shared_handle()
    {
        unixstl_message_assert("Shared search handle being destroyed with outstanding references!", 0 == cRefs);

        if(NULL != dir)
        {
            closedir(dir);
        }
    }

private:
    rds_shared_handle(class_type const &);
    class_type &operator =(class_type const &);
};
#endif /* !__STLSOFT_DOCUMENTATION_SKIP_SECTION */

////////////////////////////////////////////////////////////////////////////////
// Unit-testing

#ifdef STLSOFT_UNITTEST

namespace unittest
{
    ss_bool_t test_unixstl_readdir_sequence(unittest_reporter *r)
    {
        using stlsoft::unittest::unittest_initialiser;

        ss_bool_t               bSuccess    =   true;

        unittest_initialiser    init(r, "UNIXSTL", "readdir_sequence", __FILE__);

        readdir_sequence        files(".", readdir_sequence::files);
        int                     bFilesTestedEmpty = true;

        { for(readdir_sequence::const_iterator b = files.begin(); b != files.end(); ++b)
        {
            bFilesTestedEmpty = false;
        }}

        if(!bFilesTestedEmpty != !!files.empty())
        {
            r->report("readdir_sequence empty() contradiction when enumerating files", __LINE__);
            bSuccess = false;
        }

        readdir_sequence        directories(".", readdir_sequence::directories);
        int                     bDirectoriesTestedEmpty = true;

        { for(readdir_sequence::const_iterator b = directories.begin(); b != directories.end(); ++b)
        {
            bDirectoriesTestedEmpty = false;
        }}

        if(!bDirectoriesTestedEmpty != !!directories.empty())
        {
            r->report("readdir_sequence empty() contradiction when enumerating files", __LINE__);
            bSuccess = false;
        }

#if 0
        if(seq.empty() == (0 == seq.size()))
        {
            r->report("readdir_sequence empty() and size() contradict ", __LINE__);
            bSuccess = false;
        }
#endif /* 0 */

#if 0
        readdir_sequence::size_type   total_forward   =   0;
        readdir_sequence::size_type   total_backward  =   0;

        readdir_sequence::const_iterator            b_f =   seq.begin();
        for(; b_f != seq.end(); ++b_f)
        {
            total_forward += strlen((*b_f)->d_name);
        }

        readdir_sequence::const_reverse_iterator    b_b =   seq.rbegin();
        for(; b_b != seq.rend(); ++b_b)
        {
            total_backward += strlen((*b_b)->d_name);
        }

        if(total_forward != total_backward)
        {
            r->report("readdir_sequence forward and backward ranges contradict ", __LINE__);
            bSuccess = false;
        }
#endif /* 0 */

        return bSuccess;
    }

    unittest_registrar    unittest_unixstl_readdir_sequence(test_unixstl_readdir_sequence);

} // namespace unittest

#endif /* STLSOFT_UNITTEST */

/* /////////////////////////////////////////////////////////////////////////////
 * Implementation
 */

// readdir_sequence

inline /* static */ us_uint_t readdir_sequence::validate_flags_(us_uint_t flags)
{
    return (0 == (flags & (directories | files))) ? (flags | (directories | files)) : flags;
}

inline readdir_sequence::readdir_sequence(us_char_a_t const *directory, us_uint_t flags /* = directories | files */)
    : m_directory(directory)
    , m_flags(validate_flags_(flags))
{
    if(!traits_type::has_dir_end(directory))
    {
        m_directory += traits_type::path_name_separator();
    }
}

inline readdir_sequence::const_iterator readdir_sequence::begin() const
{
    DIR *dir = opendir(m_directory.c_str());

    return const_iterator(dir, m_directory, m_flags);
}

inline readdir_sequence::const_iterator readdir_sequence::end() const
{
    return const_iterator();
}

inline us_bool_t readdir_sequence::empty() const
{
    return begin() != end();
}

inline readdir_sequence::string_type const &readdir_sequence::get_directory() const
{
    return m_directory;
}

// readdir_sequence::const_iterator;

inline readdir_sequence::const_iterator::const_iterator(DIR *dir, readdir_sequence::string_type const &directory, us_uint_t flags)
    : m_handle(new rds_shared_handle(dir))
    , m_entry(NULL)
    , m_flags(flags)
    , m_scratch(directory)
    , m_dirLen(directory.length())
{
    unixstl_assert(0 != traits_type::has_dir_end(stlsoft_ns_qual(c_str_ptr)(m_scratch)));

    // It's more efficient to not bother doing a separate dots check if all
    // directories are being elided.
    if(0 == (m_flags & directories))
    {
        m_flags |= includeDots;
    }

    if(NULL != m_handle)
    {
        operator ++();
    }
}

inline readdir_sequence::const_iterator::const_iterator()
    : m_handle(NULL)
    , m_entry(NULL)
    , m_dirLen(0)
{}

inline readdir_sequence::const_iterator::const_iterator(class_type const &rhs)
    : m_handle(rhs.m_handle)
    , m_entry(rhs.m_entry)
    , m_scratch(rhs.m_scratch)
    , m_dirLen(rhs.m_dirLen)
{
    if(NULL != m_handle)
    {
        m_handle->AddRef();
    }
}

inline readdir_sequence::const_iterator::~const_iterator() unixstl_throw_0()
{
    if(NULL != m_handle)
    {
        m_handle->Release();
    }
}

inline readdir_sequence::const_iterator::class_type const &readdir_sequence::const_iterator::operator =(readdir_sequence::const_iterator::class_type const &rhs)
{
    if(NULL != m_handle)
    {
        m_handle->Release();
    }

    m_handle  =   rhs.m_handle;
    m_entry   =   rhs.m_entry;
    m_flags   =   rhs.m_flags;
    m_scratch =   rhs.m_scratch;
    m_dirLen  =   rhs.m_dirLen;

    if(NULL != m_handle)
    {
        m_handle->AddRef();
    }

    return *this;
}

inline readdir_sequence::const_iterator::value_type readdir_sequence::const_iterator::operator *() const
{
    unixstl_message_assert( "Dereferencing invalid iterator", NULL != m_entry);

    return m_entry;
}

/// Moves the iteration on to the next point in the sequence, or end() if
/// the sequence is exhausted
inline readdir_sequence::const_iterator::class_type &readdir_sequence::const_iterator::operator ++()
{
    unixstl_message_assert( "Incrementing invalid iterator", NULL != m_handle);

    for(;;)
    {
        m_entry = readdir(m_handle->dir);

        if(NULL != m_entry)
        {
            unixstl_assert(NULL != m_entry->d_name);

            if(0 == (m_flags & includeDots))
            {
                if(traits_type::is_dots(m_entry->d_name))
                {
                    continue; // Don't want dots; skip it
                }
            }

            if((m_flags & (directories | files)) != (directories | files))
            {
                // Now need to process the file, by using stat
                struct stat st;

                // Truncate the scratch to the directory path, ...
                m_scratch.resize(m_dirLen);
                // ... and add the file
                m_scratch += m_entry->d_name;

                if(0 != stat(stlsoft_ns_qual(c_str_ptr)(m_scratch), &st))
                {
                    // Failed to get info from entry. Must assume it is
                    // dead, so skip it
                    continue;
                }
                else
                {
                    if(m_flags & directories) // Want directories
                    {
                        if(S_IFDIR == (st.st_mode & S_IFDIR))
                        {
                            // It is a directory, so accept it
                            break;
                        }
                    }
                    if(m_flags & files) // Want files
                    {
                        if(S_IFREG == (st.st_mode & S_IFREG))
                        {
                            // It is a file, so accept it
                            break;
                        }
                    }

                    continue; // Not a match, so skip this entry
                }
            }
        }

        break;
    }

    if(NULL == m_entry)
    {
        unixstl_assert(NULL != m_handle);

        m_handle->Release();

        m_handle = NULL;
    }

    return *this;
}
/// Post-increment form of operator ++().
///
/// \note Because this version uses a temporary on which to call the
/// pre-increment form it is thereby less efficient, and should not be used
/// except where post-increment semantics are required.
inline readdir_sequence::const_iterator::class_type readdir_sequence::const_iterator::operator ++(int)
{
    class_type  ret(*this);

    operator ++();

    return ret;
}

/// Compares \c this for equality with \c rhs
///
/// \param rhs The instance against which to test
/// \retval true if the iterators are equivalent
/// \retval false if the iterators are not equivalent
inline bool readdir_sequence::const_iterator::operator ==(readdir_sequence::const_iterator::class_type const &rhs) const
{
    unixstl_assert(NULL == m_handle || NULL == rhs.m_handle || m_handle->dir == rhs.m_handle->dir);

    return m_entry == rhs.m_entry;
}
/// Compares \c this for inequality with \c rhs
///
/// \param rhs The instance against which to test
/// \retval false if the iterators are equivalent
/// \retval true if the iterators are not equivalent
inline bool readdir_sequence::const_iterator::operator !=(readdir_sequence::const_iterator::class_type const &rhs) const
{
    return !operator ==(rhs);
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

#endif /* !UNIXSTL_INCL_H_UNIXSTL_READDIR_SEQUENCE */

/* ////////////////////////////////////////////////////////////////////////// */
