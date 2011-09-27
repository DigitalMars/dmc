/* /////////////////////////////////////////////////////////////////////////////
 * File:        winstl_file_path_buffer.h
 *
 * Purpose:     Contains the basic_file_path_buffer template class, and ANSI
 *              and Unicode specialisations thereof.
 *
 * Created:     7th February 2002
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


/// \file winstl_file_path_buffer.h
///
/// Contains the basic_file_path_buffer template class, and ANSI and Unicode specialisations thereof.

#ifndef WINSTL_INCL_H_WINSTL_FILE_PATH_BUFFER
#define WINSTL_INCL_H_WINSTL_FILE_PATH_BUFFER

#ifndef __STLSOFT_DOCUMENTATION_SKIP_SECTION
# define WINSTL_VER_H_WINSTL_FILE_PATH_BUFFER_MAJOR     2
# define WINSTL_VER_H_WINSTL_FILE_PATH_BUFFER_MINOR     0
# define WINSTL_VER_H_WINSTL_FILE_PATH_BUFFER_REVISION  1
# define WINSTL_VER_H_WINSTL_FILE_PATH_BUFFER_EDIT      56
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
# include "winstl.h"                        // Include the WinSTL root header
#endif /* !WINSTL_INCL_H_WINSTL */

#if defined(__STLSOFT_COMPILER_IS_MSVC) && \
    _MSC_VER < 1100
# error winstl_file_path_buffer.h is not compatible with Visual C++ 5.0 or earlier
#endif /* _MSC_VER < 1200 */

#ifndef WINSTL_INCL_H_WINSTL_STRING_ACCESS
# include "winstl_string_access.h"          // winstl::string_access
#endif /* WINSTL_INCL_H_WINSTL_STRING_ACCESS */
#ifndef STLSOFT_INCL_H_STLSOFT_AUTO_BUFFER
# include "stlsoft_auto_buffer.h"           // stlsoft::auto_buffer
#endif /* !STLSOFT_INCL_H_STLSOFT_AUTO_BUFFER */
#ifndef WINSTL_INCL_H_WINSTL_PROCESSHEAP_ALLOCATOR
# include "winstl_processheap_allocator.h"  // winstl::processheap_allocator
#endif /* !WINSTL_INCL_H_WINSTL_PROCESSHEAP_ALLOCATOR */

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

#ifndef __STLSOFT_DOCUMENTATION_SKIP_SECTION
template <ss_typename_param_k C>
struct is_ansi
{
    enum { value = 0 };
};

template <>
struct is_ansi<ws_char_a_t>
{
    enum { value = 1 };
};
#endif /* !__STLSOFT_DOCUMENTATION_SKIP_SECTION */

// class basic_file_path_buffer
/// Acts as a buffer with sufficient size for any drive on the host machine
///
/// This class is a non-template class primarily so that separate instantiations
/// are not created for each instantiation of the basic_file_path_buffer.
///
/// This class provides a simple function, which is to provide the maximum path
/// length for the host. This information is then cached due to the static nature
/// of the get_drivesvar_() method, although it can be reset by calling the
/// refresh() method on the buffer class.
///
/// \param C The character type
/// \param A The allocator type
/// \param CCH The size of the internal member path structure. On translators that support default template arguments this default to (\c 1 + \c _MAX_PATH)
template<   ss_typename_param_k C
#ifdef __STLSOFT_CF_TEMPLATE_CLASS_DEFAULT_CLASS_ARGUMENT_SUPPORT
        ,   ss_typename_param_k A = processheap_allocator<C>
#else
        ,   ss_typename_param_k A /* = processheap_allocator<C> */
#endif /* __STLSOFT_CF_TEMPLATE_CLASS_DEFAULT_CLASS_ARGUMENT_SUPPORT */
        >
class basic_file_path_buffer
{
private:
    enum
    {
            CCH_9x = 1 + _MAX_PATH
        ,   CCH_NT = 1 + (is_ansi<C>::value ? _MAX_PATH : 32000)
    };

    typedef stlsoft_ns_qual(auto_buffer)<C, A, 1 + _MAX_PATH>   buffer_type;
public:
    /// The current parameterisation of the type
    typedef basic_file_path_buffer<C, A>                        class_type;
    /// The value type
    typedef ss_typename_type_k buffer_type::value_type          value_type;
    /// The reference type
    typedef value_type                                          &reference;
    /// The non-mutating (const) reference type
    typedef value_type const                                    &const_reference;
    /// The size type
    typedef ss_typename_type_k buffer_type::size_type           size_type;

// Construction
public:
    /// Default constructor
    basic_file_path_buffer()
        : m_buffer((::GetVersion() & 0x80000000) ? CCH_9x : CCH_NT)
    {}

    basic_file_path_buffer(class_type const &rhs)
        : m_buffer(rhs.size())
    {
        stlsoft_ns_qual(pod_copy_n)(m_buffer.data(), rhs.m_buffer.data(), m_buffer.size());
    }

    const basic_file_path_buffer &operator =(class_type const &rhs)
    {
        m_buffer.resize(rhs.size());
        stlsoft_ns_qual(pod_copy_n)(m_buffer.data(), rhs.m_buffer.data(), m_buffer.size());

        return *this;
    }

// Operations
public:
    void swap(class_type &rhs)
    {
        m_buffer.swap(rhs.m_buffer);
    }

// Accessors
public:
    /// Returns a pointer to a nul-terminated string
    value_type const *c_str() const
    {
        return m_buffer.data();
    }

    /// Returns a mutable (non-const) pointer to the internal buffer
    reference operator [](size_t index)
    {
        buffer_type   &this_  =   m_buffer;

        return this_[index];
    }
#if !defined(__STLSOFT_COMPILER_IS_COMO) && \
    !defined(__STLSOFT_COMPILER_IS_MWERKSx)
    /// Returns a non-mutable (const) pointer to the internal buffer
    const_reference operator [](size_t index) const
    {
        winstl_message_assert("Index out of range", !(size() < index));

        return m_buffer.data()[index];
    }
#else /* ? compiler */
#endif /* compiler */

    /// Returns the size of the internal buffer
    size_type size() const
    {
        return m_buffer.size();
    }

// Operations
public:
    /// Causes the drives to be examined again for the next instance.
    ///
    /// \deprecated
    static void refresh() winstl_throw_0()
    {}

// Members
private:
    buffer_type m_buffer;
};

/* Typedefs to commonly encountered types. */
/// Instantiation of the basic_file_path_buffer template for the ANSI character type \c char
typedef basic_file_path_buffer<ws_char_a_t, processheap_allocator<ws_char_a_t> >    file_path_buffer_a;
/// Instantiation of the basic_file_path_buffer template for the Unicode character type \c wchar_t
typedef basic_file_path_buffer<ws_char_w_t, processheap_allocator<ws_char_w_t> >    file_path_buffer_w;

/* /////////////////////////////////////////////////////////////////////////////
 * Shims
 */

template<   ss_typename_param_k C
        ,   ss_typename_param_k A
        >
inline C const *c_str_ptr_null(basic_file_path_buffer<C, A> const &b)
{
    return stlsoft_ns_qual(c_str_ptr_null)(b.c_str());
}

template<   ss_typename_param_k C
        ,   ss_typename_param_k A
        >
inline C const *c_str_ptr(basic_file_path_buffer<C, A> const &b)
{
    return stlsoft_ns_qual(c_str_ptr)(b.c_str());
}

template<   ss_typename_param_k C
        ,   ss_typename_param_k A
        >
inline ws_size_t c_str_len(basic_file_path_buffer<C, A> const &b)
{
    return stlsoft_ns_qual(c_str_len)(b.c_str());
}

template<   ss_typename_param_k C
        ,   ss_typename_param_k A
        >
inline ws_size_t c_str_size(basic_file_path_buffer<C, A> const &b)
{
    return stlsoft_ns_qual(c_str_size)(b.c_str());
}

template<   ss_typename_param_k S
        ,   ss_typename_param_k C
        ,   ss_typename_param_k A
        >
inline S &operator <<(S & s, basic_file_path_buffer<C, A> const &b)
{
    s << b.c_str();

    return s;
}

/* /////////////////////////////////////////////////////////////////////////////
 * Unit-testing
 */

#ifdef STLSOFT_UNITTEST

namespace unittest
{
    ss_bool_t test_winstl_file_path_buffer(unittest_reporter *r)
    {
        using stlsoft::unittest::unittest_initialiser;

        ss_bool_t               bSuccess    =   true;

        unittest_initialiser    init(r, "WinSTL", "file_path_buffer", __FILE__);

        file_path_buffer_a      buf_a;
        file_path_buffer_a      buf_a_2(buf_a);
        file_path_buffer_a      buf_a_3;

        buf_a_3 = buf_a;

        if(1 + _MAX_PATH != buf_a.size())
        {
            r->report("ANSI buffer size incorrect", __LINE__);
            bSuccess = false;
        }

        file_path_buffer_w      buf_w;

        if(::GetVersion() & 0x80000000)
        {
            if(1 + _MAX_PATH != buf_w.size())
            {
                r->report("Unicode (Win9x) buffer size incorrect", __LINE__);
                bSuccess = false;
            }
        }
        else
        {
            if(1 + 32000 != buf_w.size())
            {
                r->report("Unicode (Win NT) buffer size incorrect", __LINE__);
                bSuccess = false;
            }
        }

        return bSuccess;
    }

    unittest_registrar    unittest_winstl_file_path_buffer(test_winstl_file_path_buffer);

} // namespace unittest

#endif /* STLSOFT_UNITTEST */

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

using ::winstl::c_str_ptr_null;

using ::winstl::c_str_ptr;

using ::winstl::c_str_len;

using ::winstl::c_str_size;

# if !defined(_STLSOFT_NO_NAMESPACE) && \
     !defined(__STLSOFT_DOCUMENTATION_SKIP_SECTION)
} // namespace stlsoft
# else /* ? _STLSOFT_NO_NAMESPACE */
/* There is no stlsoft namespace, so must define in the global namespace */
# endif /* !_STLSOFT_NO_NAMESPACE */
#endif /* !_WINSTL_NO_NAMESPACE */

/* ////////////////////////////////////////////////////////////////////////// */

#endif /* WINSTL_INCL_H_WINSTL_FILE_PATH_BUFFER */

/* ////////////////////////////////////////////////////////////////////////// */
