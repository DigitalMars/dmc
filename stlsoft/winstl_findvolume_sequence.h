/* /////////////////////////////////////////////////////////////////////////////
 * File:        winstl_findvolume_sequence.h
 *
 * Purpose:     Contains the basic_findvolume_sequence template class, and ANSI
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


/// \file winstl_findvolume_sequence.h
///
/// Contains the basic_findvolume_sequence template class, and ANSI and Unicode specialisations thereof.

#ifndef WINSTL_INCL_H_WINSTL_FINDVOLUME_SEQUENCE
#define WINSTL_INCL_H_WINSTL_FINDVOLUME_SEQUENCE

#ifndef __STLSOFT_DOCUMENTATION_SKIP_SECTION
# define WINSTL_VER_H_WINSTL_FINDVOLUME_SEQUENCE_MAJOR        2
# define WINSTL_VER_H_WINSTL_FINDVOLUME_SEQUENCE_MINOR        0
# define WINSTL_VER_H_WINSTL_FINDVOLUME_SEQUENCE_REVISION     1
# define WINSTL_VER_H_WINSTL_FINDVOLUME_SEQUENCE_EDIT         62
#endif /* !__STLSOFT_DOCUMENTATION_SKIP_SECTION */

/* /////////////////////////////////////////////////////////////////////////////
 * Includes
 */

#ifndef WINSTL_INCL_H_WINSTL
# include "winstl.h"                    // Include the WinSTL root header
#endif /* !WINSTL_INCL_H_WINSTL */
#ifndef WINSTL_INCL_H_WINSTL_FILESYSTEM_TRAITS
# include "winstl_filesystem_traits.h"  // filesystem_traits
#endif /* !WINSTL_INCL_H_WINSTL_FILESYSTEM_TRAITS */
#ifndef STLSOFT_INCL_H_STLSOFT_ITERATOR
# include "stlsoft_iterator.h"
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
 * Enumerations
 */

// The FindNextVolume API is not well documented so assume _MAX_PATH
// is sufficient for volume names
enum
{
    MAX_VOL_NAME    =   _MAX_PATH   //!< The maximum number of characters in a volume name
};

/* /////////////////////////////////////////////////////////////////////////////
 * Forward declarations
 */

#ifndef __STLSOFT_DOCUMENTATION_SKIP_SECTION

template <ss_typename_param_k C, ss_typename_param_k T>
class basic_findvolume_sequence_value_type;

template <ss_typename_param_k C, ss_typename_param_k T, ss_typename_param_k V>
class basic_findvolume_sequence_const_iterator;

#endif /* !__STLSOFT_DOCUMENTATION_SKIP_SECTION */

/* /////////////////////////////////////////////////////////////////////////////
 * Classes
 */

// class basic_findvolume_sequence
/// Presents an STL-like sequence interface over the volumes on a system
///
/// \note This class functions only on Windows 2000 and later
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
class basic_findvolume_sequence
{
public:
    /// The character type
    typedef C                                                           char_type;
    /// The traits type
    typedef T                                                           traits_type;
    /// The current parameterisation of the type
    typedef basic_findvolume_sequence<C, T>                             class_type;
    /// The value type
    typedef basic_findvolume_sequence_value_type<C, T>                  value_type;
    /// The non-mutating (const) iterator type
    typedef basic_findvolume_sequence_const_iterator<C, T, value_type>  const_iterator;
    /// The reference type
    typedef value_type                                                  &reference;
    /// The non-mutable (const) reference type
    typedef const value_type                                            &const_reference;

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

// State
public:
    /// Evalulates whether the sequence is empty
    ws_bool_t       empty() const;
};

/* /////////////////////////////////////////////////////////////////////////////
 * Typedefs for commonly encountered types
 */

/// Instantiation of the basic_findvolume_sequence template for the ANSI character type \c char
typedef basic_findvolume_sequence<ws_char_a_t, filesystem_traits<ws_char_a_t> >     findvolume_sequence_a;
/// Instantiation of the basic_findvolume_sequence template for the Unicode character type \c wchar_t
typedef basic_findvolume_sequence<ws_char_w_t, filesystem_traits<ws_char_w_t> >     findvolume_sequence_w;
/// Instantiation of the basic_findvolume_sequence template for the Win32 character type \c TCHAR
typedef basic_findvolume_sequence<TCHAR, filesystem_traits<TCHAR> >                 findvolume_sequence;

/* ////////////////////////////////////////////////////////////////////////// */

// class basic_findvolume_sequence_value_type
/// Value type for the basic_findvolume_sequence
template<   ss_typename_param_k C
        ,   ss_typename_param_k T
        >
class basic_findvolume_sequence_value_type
{
public:
    /// The character type
    typedef C                                           char_type;
    /// The traits type
    typedef T                                           traits_type;
    /// The current parameterisation of the type
    typedef basic_findvolume_sequence_value_type<C, T>  class_type;

public:
    /// Default constructor
    basic_findvolume_sequence_value_type();
    /// Copy constructor
    basic_findvolume_sequence_value_type(const class_type &rhs);
private:
    basic_findvolume_sequence_value_type(char_type const *vol_name);
public:

    /// Copy assignment operator
    basic_findvolume_sequence_value_type &operator =(const class_type &rhs);

// Attributes
public:
    /// Implicit conversion to non-mutable pointer-to-const of the volume name
    operator char_type const * () const
    {
        return m_name;
    }

private:
    friend class basic_findvolume_sequence_const_iterator<C, T, class_type>;

    char_type   m_name[MAX_VOL_NAME + 1];
};

// class basic_findvolume_sequence_const_iterator
/// Iterator type for the basic_findvolume_sequence, supporting the Input Iterator concept
template<   ss_typename_param_k C
        ,   ss_typename_param_k T
        ,   ss_typename_param_k V
        >
class basic_findvolume_sequence_const_iterator
    : public stlsoft_ns_qual(iterator_base)<winstl_ns_qual_std(input_iterator_tag), V, ws_ptrdiff_t, void, V>
{
public:
    /// The character type
    typedef C                                                   char_type;
    /// The traits type
    typedef T                                                   traits_type;
    /// The traits type
    typedef V                                                   value_type;
    /// The current parameterisation of the type
    typedef basic_findvolume_sequence_const_iterator<C, T, V>   class_type;

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

    public:
        ss_explicit_k shared_handle(HANDLE h)
            : hSrch(h)
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
        ~shared_handle()
        {
            winstl_message_assert("Shared search handle being destroyed with outstanding references!", 0 == cRefs);

            if(hSrch != INVALID_HANDLE_VALUE)
            {
                traits_type::find_volume_close(hSrch);
            }
        }

    private:
        shared_handle(class_type const &);
        class_type &operator =(class_type const &);
    };
#endif /* !__STLSOFT_DOCUMENTATION_SKIP_SECTION */
/// @}

/// \name Construction
/// @{
private:
    basic_findvolume_sequence_const_iterator(basic_findvolume_sequence<C, T> const &l, HANDLE hSrch, char_type const *vol_name)
        : m_list(&l)
        , m_handle(new shared_handle(hSrch))
    {
        traits_type::str_copy(m_name, vol_name);

        winstl_assert(INVALID_HANDLE_VALUE != hSrch);
    }
    basic_findvolume_sequence_const_iterator(basic_findvolume_sequence<C, T> const &l);
public:
    /// Default constructor
    basic_findvolume_sequence_const_iterator();
    /// Copy constructor
    basic_findvolume_sequence_const_iterator(const class_type &rhs);
    /// Destructor
    ~basic_findvolume_sequence_const_iterator() winstl_throw_0();

    /// Copy assignment operator
    basic_findvolume_sequence_const_iterator &operator =(const class_type &rhs);

public:
    /// Pre-increment operator
    class_type &operator ++();
    /// Post-increment operator
    class_type operator ++(int);
    /// Dereference to access the value at the current position
    const value_type operator *() const;
    /// Evaluates whether \c this and \c rhs are equivalent
    ws_bool_t operator ==(const class_type &rhs) const;
    /// Evaluates whether \c this and \c rhs are not equivalent
    ws_bool_t operator !=(const class_type &rhs) const;

// Members
private:
    friend class basic_findvolume_sequence<C, T>;

    typedef basic_findvolume_sequence<C, T>     list_type;

    list_type const *m_list;
    shared_handle   *m_handle;
    char_type       m_name[MAX_VOL_NAME + 1];
};

////////////////////////////////////////////////////////////////////////////////
// Unit-testing

#ifdef STLSOFT_UNITTEST

namespace unittest
{
    ss_bool_t test_winstl_findvolume_sequence(unittest_reporter *r)
    {
        using stlsoft::unittest::unittest_initialiser;

        ss_bool_t               bSuccess    =   true;

        unittest_initialiser    init(r, "WinSTL", "findvolume_sequence", __FILE__);

        findvolume_sequence_a   fvsa;
        findvolume_sequence_w   fvsw;

        if(fvsa.empty() != fvsw.empty())
        {
            r->report("findvolume_sequence(ANSI)::empty() != findvolume_sequence(Unicode)::empty()", __LINE__);
            bSuccess = false;
        }

        return bSuccess;
    }

    unittest_registrar    unittest_winstl_findvolume_sequence(test_winstl_findvolume_sequence);

} // namespace unittest

#endif /* STLSOFT_UNITTEST */

////////////////////////////////////////////////////////////////////////////////
// Implementation

#ifndef __STLSOFT_DOCUMENTATION_SKIP_SECTION

// basic_findvolume_sequence

template <ss_typename_param_k C, ss_typename_param_k T>
inline ss_typename_type_k basic_findvolume_sequence<C, T>::const_iterator basic_findvolume_sequence<C, T>::begin() const
{
    char_type   vol_name[MAX_VOL_NAME + 1];
    HANDLE      hSrch   =   traits_type::find_first_volume(vol_name, winstl_num_elements(vol_name));

    if(hSrch == INVALID_HANDLE_VALUE)
    {
        return const_iterator(*this);
    }
    else
    {
        return const_iterator(*this, hSrch, vol_name);
    }
}

template <ss_typename_param_k C, ss_typename_param_k T>
inline ss_typename_type_k basic_findvolume_sequence<C, T>::const_iterator basic_findvolume_sequence<C, T>::end() const
{
    return const_iterator(*this);
}

template <ss_typename_param_k C, ss_typename_param_k T>
inline ws_bool_t basic_findvolume_sequence<C, T>::empty() const
{
    return begin() == end();
}

// basic_findvolume_sequence_value_type

template <ss_typename_param_k C, ss_typename_param_k T>
inline basic_findvolume_sequence_value_type<C, T>::basic_findvolume_sequence_value_type()
{
    m_name[0] = '\0';
}

template <ss_typename_param_k C, ss_typename_param_k T>
inline basic_findvolume_sequence_value_type<C, T>::basic_findvolume_sequence_value_type(const class_type &rhs)
{
    traits_type::str_copy(m_name, rhs.m_name);
}

template <ss_typename_param_k C, ss_typename_param_k T>
inline basic_findvolume_sequence_value_type<C, T>::basic_findvolume_sequence_value_type(char_type const *vol_name)
{
    traits_type::str_copy(m_name, vol_name);
}

template <ss_typename_param_k C, ss_typename_param_k T>
inline ss_typename_type_k basic_findvolume_sequence_value_type<C, T>::class_type &basic_findvolume_sequence_value_type<C, T>::operator =(const class_type &rhs)
{
    traits_type::str_copy(m_name, rhs.m_name);

    return *this;
}


#if 0
template <ss_typename_param_k C, ss_typename_param_k T>
#ifdef __STLSOFT_CF_FUNCTION_SIGNATURE_FULL_ARG_QUALIFICATION_REQUIRED
inline basic_findvolume_sequence_value_type<C, T>::operator basic_findvolume_sequence<C, T>::char_type const *() const
#else
inline basic_findvolume_sequence_value_type<C, T>::operator char_type const *() const
#endif /* __STLSOFT_CF_FUNCTION_SIGNATURE_FULL_ARG_QUALIFICATION_REQUIRED */
{
    return m_name;
}
#endif /* 0 */


// basic_findvolume_sequence_const_iterator

template <ss_typename_param_k C, ss_typename_param_k T, ss_typename_param_k V>
inline basic_findvolume_sequence_const_iterator<C, T, V>::basic_findvolume_sequence_const_iterator()
    : m_list(NULL)
    , m_handle(NULL)
{
    m_name[0] = '\0';
}

template <ss_typename_param_k C, ss_typename_param_k T, ss_typename_param_k V>
inline basic_findvolume_sequence_const_iterator<C, T, V>::basic_findvolume_sequence_const_iterator(basic_findvolume_sequence<C, T> const &l)
    : m_list(&l)
    , m_handle(NULL)
{
    m_name[0] = '\0';
}

template <ss_typename_param_k C, ss_typename_param_k T, ss_typename_param_k V>
inline basic_findvolume_sequence_const_iterator<C, T, V>::basic_findvolume_sequence_const_iterator(const class_type &rhs)
    : m_list(rhs.m_list)
    , m_handle(rhs.m_handle)
{
    if(NULL != m_handle)
    {
        m_handle->AddRef();
    }
}

#ifndef __STLSOFT_DOCUMENTATION_SKIP_SECTION

template <ss_typename_param_k C, ss_typename_param_k T, ss_typename_param_k V>
inline ss_typename_type_k basic_findvolume_sequence_const_iterator<C, T, V>::class_type &basic_findvolume_sequence_const_iterator<C, T, V>::operator =(ss_typename_type_k basic_findvolume_sequence_const_iterator<C, T, V>::class_type const &rhs)
{
    if(NULL != m_handle)
    {
        m_handle->Release();
    }

    m_list      =   rhs.m_list;
    m_handle    =   rhs.m_handle;

    if(NULL != m_handle)
    {
        m_handle->AddRef();
    }

    return *this;
}

#endif /* !__STLSOFT_DOCUMENTATION_SKIP_SECTION */

template <ss_typename_param_k C, ss_typename_param_k T, ss_typename_param_k V>
inline basic_findvolume_sequence_const_iterator<C, T, V>::~basic_findvolume_sequence_const_iterator() winstl_throw_0()
{
    if(NULL != m_handle)
    {
        m_handle->Release();
    }
}

template <ss_typename_param_k C, ss_typename_param_k T, ss_typename_param_k V>
inline ss_typename_type_k basic_findvolume_sequence_const_iterator<C, T, V>::class_type &basic_findvolume_sequence_const_iterator<C, T, V>::operator ++()
{
    winstl_message_assert("Attempting to increment an invalid iterator!", NULL != m_handle);

    if(!traits_type::find_next_volume(m_handle->hSrch, m_name, winstl_num_elements(m_name)))
    {
        m_handle->Release();

        m_handle = NULL;
    }

    return *this;
}

template <ss_typename_param_k C, ss_typename_param_k T, ss_typename_param_k V>
inline ss_typename_type_k basic_findvolume_sequence_const_iterator<C, T, V>::class_type basic_findvolume_sequence_const_iterator<C, T, V>::operator ++(int)
{
    class_type  ret(*this);

    operator ++();

    return ret;
}

template <ss_typename_param_k C, ss_typename_param_k T, ss_typename_param_k V>
inline const ss_typename_type_k basic_findvolume_sequence_const_iterator<C, T, V>::value_type basic_findvolume_sequence_const_iterator<C, T, V>::operator *() const
{
    if(NULL != m_handle)
    {
        return value_type(m_name);
    }
    else
    {
        return value_type();
    }
}

template <ss_typename_param_k C, ss_typename_param_k T, ss_typename_param_k V>
inline ws_bool_t basic_findvolume_sequence_const_iterator<C, T, V>::operator ==(class_type const &rhs) const
{
    ws_bool_t    eq;

    winstl_assert(m_list == rhs.m_list);    // Should only be comparing iterators from same container

    // Not equal if different lists, or if one but not both handles is the INVALID_HANDLE_VALUE
    // or if the data is not equal.
    if( m_list != rhs.m_list ||
        (NULL == m_handle) != (NULL == m_handle) ||
        (   NULL != m_handle &&
            traits_type::str_compare(m_name, rhs.m_name) != 0))
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
inline ws_bool_t basic_findvolume_sequence_const_iterator<C, T, V>::operator !=(class_type const &rhs) const
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

/* ////////////////////////////////////////////////////////////////////////// */

#endif /* WINSTL_INCL_H_WINSTL_FINDVOLUME_SEQUENCE */

/* ////////////////////////////////////////////////////////////////////////// */
