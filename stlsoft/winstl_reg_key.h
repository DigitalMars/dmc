/* /////////////////////////////////////////////////////////////////////////////
 * File:        winstl_reg_key.h
 *
 * Purpose:     Contains the basic_reg_key template class, and ANSI
 *              and Unicode specialisations thereof.
 *
 * Created:     19th January 2002
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


/// \file winstl_reg_key.h
///
/// Contains the basic_reg_key template class, and ANSI and Unicode specialisations thereof.

#ifndef WINSTL_INCL_H_WINSTL_REG_KEY
#define WINSTL_INCL_H_WINSTL_REG_KEY

#ifndef __STLSOFT_DOCUMENTATION_SKIP_SECTION
# define WINSTL_VER_H_WINSTL_REG_KEY_MAJOR      2
# define WINSTL_VER_H_WINSTL_REG_KEY_MINOR      0
# define WINSTL_VER_H_WINSTL_REG_KEY_REVISION   1
# define WINSTL_VER_H_WINSTL_REG_KEY_EDIT       61
#endif /* !__STLSOFT_DOCUMENTATION_SKIP_SECTION */

/* /////////////////////////////////////////////////////////////////////////////
 * Includes
 */

#ifndef WINSTL_INCL_H_WINSTL
# include "winstl.h"                        // Include the WinSTL root header
#endif /* !WINSTL_INCL_H_WINSTL */
#ifndef WINSTL_INCL_H_WINSTL_REG_DEFS
# include "winstl_reg_defs.h"               // The WinSTL reg API standard types
#endif /* WINSTL_INCL_H_WINSTL_REG_DEFS */
#ifndef WINSTL_INCL_H_WINSTL_REG_TRAITS
# include "winstl_reg_traits.h"             // The WinSTL reg API reg_traits class
#endif /* WINSTL_INCL_H_WINSTL_REG_TRAITS */
#ifndef STLSOFT_INCL_H_STLSOFT_AUTO_DESTRUCTOR
# include "stlsoft_auto_buffer.h"           // Include the STLSoft auto_buffer
#endif /* STLSOFT_INCL_H_STLSOFT_AUTO_DESTRUCTOR */
#ifndef WINSTL_INCL_H_WINSTL_AUTO_DESTRUCTOR
# include "winstl_processheap_allocator.h"  // winstl::processheap_allocator
#endif /* WINSTL_INCL_H_WINSTL_AUTO_DESTRUCTOR */
#ifndef STLSOFT_INCL_H_STLSOFT_STRING_ACCESS
# include "stlsoft_string_access.h"         // stlsoft::c_str_ptr
#endif /* !STLSOFT_INCL_H_STLSOFT_STRING_ACCESS */

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

#if !defined(__STLSOFT_COMPILER_IS_MWERKS)
stlsoft_ns_using(c_str_ptr)
#endif /* compiler */

/* ////////////////////////////////////////////////////////////////////////// */

/// \weakgroup winstl_reg_library Registry Library
/// @{

/* /////////////////////////////////////////////////////////////////////////////
 * Forward declarations
 */

#ifndef __STLSOFT_DOCUMENTATION_SKIP_SECTION

template <ss_typename_param_k C, ss_typename_param_k T, ss_typename_param_k A>
class basic_reg_value_sequence;

template <ss_typename_param_k C, ss_typename_param_k T, ss_typename_param_k A>
class basic_reg_key_sequence;

#endif /* !__STLSOFT_DOCUMENTATION_SKIP_SECTION */

/* /////////////////////////////////////////////////////////////////////////////
 * Classes
 */

/// Represents a registry key
///
/// This class acts as the value type of classes that manipulate registry keys
/// and encapsulates the concept of a registry key.
///
/// \param C The character type
/// \param T The traits type. On translators that support default template arguments this defaults to reg_traits<C>
/// \param A The allocator type. On translators that support default template arguments this defaults to processheap_allocator<C>
template<   ss_typename_param_k C
#ifdef __STLSOFT_CF_TEMPLATE_CLASS_DEFAULT_CLASS_ARGUMENT_SUPPORT
        ,   ss_typename_param_k T = reg_traits<C>
        ,   ss_typename_param_k A = processheap_allocator<C>
#else
        ,   ss_typename_param_k T /* = reg_traits<C> */
        ,   ss_typename_param_k A /* = processheap_allocator<C> */
#endif /* __STLSOFT_CF_TEMPLATE_CLASS_DEFAULT_CLASS_ARGUMENT_SUPPORT */
        >
class basic_reg_key
{
public:
    /// The character type
    typedef C                                           char_type;
    /// The traits type
    typedef T                                           traits_type;
    /// The allocator type
    typedef A                                           allocator_type;
    /// The current parameterisation of the type
    typedef basic_reg_key<C, T, A>                      class_type;
    /// The size type
    typedef ss_typename_type_k traits_type::size_type   size_type;
    /// The string type
    typedef ss_typename_type_k traits_type::string_type string_type;
    /// The key type
#if defined(__STLSOFT_COMPILER_IS_MSVC) && \
    _MSC_VER == 1100
    /* WSCB: VC5 has an internal compiler error if use traits_type::hkey_type */
    typedef HKEY                                        hkey_type;
#else
    typedef ss_typename_type_k traits_type::hkey_type   hkey_type;
#endif /* 0 */

// Construction
public:
    /// Default constructor
    basic_reg_key();
    /// Construct from the named sub-key of the given parent
    basic_reg_key(hkey_type hkeyParent, string_type const &key_name, REGSAM samDesired = KEY_READ)
        : m_hkey(open_key_(hkeyParent, c_str_ptr(key_name), samDesired))
        , m_name(key_name)
    {} // Implementation is within class, otherwise VC5 will not link
    /// Construct from the named sub-key of the given parent
    basic_reg_key(class_type const &keyParent, string_type const &key_name, REGSAM samDesired = KEY_READ)
        : m_hkey(open_key_(keyParent.get_key_handle(), c_str_ptr(key_name), samDesired))
        , m_name(key_name)
    {} // Implementation is within class, otherwise VC5 will not link

    /// Copy constructor
    basic_reg_key(class_type const &rhs);
    /// Copy constructor
    ///
    /// \param rhs The key from which to copy
    /// \param samDesired a change of permissions
    basic_reg_key(class_type const &rhs, REGSAM samDesired);

protected:
    basic_reg_key(hkey_type *hkey, string_type const &key_name);
public:
    /// Destructor
    ~basic_reg_key() winstl_throw_0();

    /// Copy assignment operator
    class_type &operator =(class_type const &rhs);

// Attributes
public:
    /// The name of the key
    string_type const   &name() const;
    /// The registry class of the key
    string_type         reg_class() const;
    /// The number of sub-keys
    ws_uint_t           num_sub_keys() const;
    /// The number of values
    ws_uint_t           num_values() const;

    /// The key handle
    hkey_type           get_key_handle() const;

// Operations
public:
    /// Opens the named sub-key of this key
    class_type  open_sub_key(char_type const *sub_key_name, REGSAM samDesired = KEY_READ);
#ifdef __STLSOFT_CF_MEMBER_TEMPLATE_FUNCTION_SUPPORT
    template <ss_typename_param_k S>
    class_type  open_sub_key(S const &sub_key_name, REGSAM samDesired = KEY_READ)
    {
        return open_sub_key(c_str_ptr(sub_key_name), samDesired);
    }
#endif /* __STLSOFT_CF_MEMBER_TEMPLATE_FUNCTION_SUPPORT */

    /// Creates a named sub-key of this key
    class_type  create_sub_key(char_type const *sub_key_name, REGSAM samDesired = KEY_READ);
#ifdef __STLSOFT_CF_MEMBER_TEMPLATE_FUNCTION_SUPPORT
    template <ss_typename_param_k S>
    class_type  create_sub_key(S const &sub_key_name, REGSAM samDesired = KEY_READ)
    {
        return create_sub_key(c_str_ptr(sub_key_name), samDesired);
    }
#endif /* __STLSOFT_CF_MEMBER_TEMPLATE_FUNCTION_SUPPORT */

    /* The handle returned from this method MUST be closed with RegCloseKey() */
    hkey_type   dup_key_handle(REGSAM samDesired = KEY_ALL_ACCESS);

// Implementation
private:
    static hkey_type    open_key_(hkey_type hkeyParent, char_type const *key_name, REGSAM samDesired);
    void                swap_(class_type &rhs) winstl_throw_0();

// Members
private:
    friend  class basic_reg_value_sequence<C, T, A>;
    friend  class basic_reg_key_sequence<C, T, A>;

    hkey_type   m_hkey; // The parent key
    string_type m_name; // The name
};

/* Typedefs to commonly encountered types. */
/// Instantiation of the basic_reg_key template for the ANSI character type \c char
typedef basic_reg_key<ws_char_a_t, reg_traits<ws_char_a_t>, processheap_allocator<ws_char_a_t> >    reg_key_a;
/// Instantiation of the basic_reg_key template for the Unicode character type \c wchar_t
typedef basic_reg_key<ws_char_w_t, reg_traits<ws_char_w_t>, processheap_allocator<ws_char_w_t> >    reg_key_w;
/// Instantiation of the basic_reg_key template for the Win32 character type \c TCHAR
typedef basic_reg_key<TCHAR, reg_traits<TCHAR>, processheap_allocator<TCHAR> >                      reg_key;

/* ////////////////////////////////////////////////////////////////////////////
 * Handle access shims
 */

/* get_handle */

/// \brief Returns the corresponding registry handle of \c k
template<   ss_typename_param_k C
        ,   ss_typename_param_k T
        ,   ss_typename_param_k A
        >
inline HKEY get_handle(basic_reg_key<C, T, A> const &key)
{
    return key.get_key_handle();
}

////////////////////////////////////////////////////////////////////////////////
// Unit-testing

#ifdef STLSOFT_UNITTEST

namespace unittest
{
    ss_bool_t test_winstl_reg_key(unittest_reporter *r)
    {
        using stlsoft::unittest::unittest_initialiser;

        ss_bool_t               bSuccess    =   true;

        unittest_initialiser    init(r, "WinSTL", "reg_key", __FILE__);

#if 0
        if(<<TODO>>)
        {
            r->report("<<TODO>> failed ", __LINE__);
            bSuccess = false;
        }
#endif /* 0 */

        return bSuccess;
    }

    unittest_registrar    unittest_winstl_reg_key(test_winstl_reg_key);

} // namespace unittest

#endif /* STLSOFT_UNITTEST */

/* /////////////////////////////////////////////////////////////////////////////
 * Implementation
 */

#ifndef __STLSOFT_DOCUMENTATION_SKIP_SECTION

template<   ss_typename_param_k C
        ,   ss_typename_param_k T
        ,   ss_typename_param_k A
        >
#if (   defined(__STLSOFT_COMPILER_IS_MSVC) && \
        _MSC_VER < 1100) || \
    defined(__STLSOFT_COMPILER_IS_VECTORC)
inline /* static */ ss_typename_type_k basic_reg_key<C, T, A>::hkey_type basic_reg_key<C, T, A>::open_key_(hkey_type hkeyParent, char_type const *key_name, REGSAM samDesired)
#else
inline /* static */ ss_typename_type_k basic_reg_key<C, T, A>::hkey_type basic_reg_key<C, T, A>::open_key_(ss_typename_param_k basic_reg_key<C, T, A>::hkey_type hkeyParent, ss_typename_param_k basic_reg_key<C, T, A>::char_type const *key_name, REGSAM samDesired)
#endif /* __STLSOFT_COMPILER_IS_MSVC && _MSC_VER < 1100 */
{
    hkey_type   hkey;

    return (0 == traits_type::reg_open_key(hkeyParent, key_name, &hkey, samDesired)) ? hkey : NULL;
}

// Construction
template <ss_typename_param_k C, ss_typename_param_k T, ss_typename_param_k A>
inline basic_reg_key<C, T, A>::basic_reg_key()
    : m_hkey(NULL)
{}

template <ss_typename_param_k C, ss_typename_param_k T, ss_typename_param_k A>
inline basic_reg_key<C, T, A>::basic_reg_key(ss_typename_type_k basic_reg_key<C, T, A>::hkey_type *hkey, ss_typename_type_k basic_reg_key<C, T, A>::string_type const &key_name)
    : m_hkey(*hkey)
    , m_name(key_name)
{}

#if 0 /* These methods are inline, as VC5 will not link otherwise. (Both 4.2 & 6 will!) */
template <ss_typename_param_k C, ss_typename_param_k T, ss_typename_param_k A>
inline basic_reg_key<C, T, A>::basic_reg_key(hkey_type hkeyParent, basic_reg_key<C, T, A>::string_type const &key_name)
    : m_hkey(open_key_(hkeyParent, c_str_ptr(key_name), samDesired))
    , m_name(key_name)
{}

template <ss_typename_param_k C, ss_typename_param_k T, ss_typename_param_k A>
inline basic_reg_key<C, T, A>::basic_reg_key(ss_typename_param_k basic_reg_key<C, T, A>::class_type const &keyParent, ss_typename_param_k basic_reg_key<C, T, A>::string_type const &key_name, REGSAM samDesired /* = KEY_READ */)
    : m_hkey(open_key_(keyParent.get_key_handle(), c_str_ptr(key_name), samDesired))
    , m_name(key_name)
{}
#endif /* 0 */

template <ss_typename_param_k C, ss_typename_param_k T, ss_typename_param_k A>
inline basic_reg_key<C, T, A>::basic_reg_key(class_type const &rhs)
    : m_hkey(traits_type::key_dup(rhs.m_hkey))
    , m_name(rhs.m_name)
{}

template <ss_typename_param_k C, ss_typename_param_k T, ss_typename_param_k A>
inline basic_reg_key<C, T, A>::basic_reg_key(class_type const &rhs, REGSAM samDesired)
    : m_hkey(traits_type::key_dup(rhs.m_hkey, samDesired))
    , m_name(rhs.m_name)
{}

template <ss_typename_param_k C, ss_typename_param_k T, ss_typename_param_k A>
inline basic_reg_key<C, T, A>::~basic_reg_key() winstl_throw_0()
{
    if(m_hkey != NULL)
    {
        ::RegCloseKey(m_hkey);
    }
}

template <ss_typename_param_k C, ss_typename_param_k T, ss_typename_param_k A>
inline void basic_reg_key<C, T, A>::swap_(ss_typename_type_k basic_reg_key<C, T, A>::class_type &rhs) winstl_throw_0()
{
    m_name.swap(rhs.m_name);

    hkey_type   _hkey       =   m_hkey;
                m_hkey      =   rhs.m_hkey;
                rhs.m_hkey  =   _hkey;
}

template <ss_typename_param_k C, ss_typename_param_k T, ss_typename_param_k A>
inline ss_typename_type_k basic_reg_key<C, T, A>::class_type &basic_reg_key<C, T, A>::operator =(ss_typename_type_k basic_reg_key<C, T, A>::class_type const &rhs)
{
    class_type  _this(rhs);

    swap_(_this);

    return *this;
}

// Attributes
template <ss_typename_param_k C, ss_typename_param_k T, ss_typename_param_k A>
inline ss_typename_type_k basic_reg_key<C, T, A>::string_type const &basic_reg_key<C, T, A>::name() const
{
    return m_name;
}

template <ss_typename_param_k C, ss_typename_param_k T, ss_typename_param_k A>
inline ss_typename_type_k basic_reg_key<C, T, A>::string_type basic_reg_key<C, T, A>::reg_class() const
{
    size_type   cch_key_class   =   0;
    ws_long_t   res             =   traits_type::reg_query_info(m_hkey, NULL, &cch_key_class, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);

    if(res == 0)
    {
        stlsoft_ns_qual(auto_buffer)<char_type, allocator_type, CCH_REG_API_AUTO_BUFFER>  p(++cch_key_class);

        res = traits_type::reg_query_info(m_hkey, p, &cch_key_class, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);

        if(res == 0)
        {
            return string_type(p, cch_key_class);
        }
    }

    return string_type();
}

template <ss_typename_param_k C, ss_typename_param_k T, ss_typename_param_k A>
inline ws_uint_t basic_reg_key<C, T, A>::num_sub_keys() const
{
    ws_uint_t   c_sub_keys;
    ws_long_t   res         =   traits_type::reg_query_info(m_hkey, NULL, NULL, &c_sub_keys, NULL, NULL, NULL, NULL, NULL, NULL, NULL);

    if(res != 0)
    {
        c_sub_keys = 0;
    }

    return c_sub_keys;
}

template <ss_typename_param_k C, ss_typename_param_k T, ss_typename_param_k A>
inline ws_uint_t basic_reg_key<C, T, A>::num_values() const
{
    ws_uint_t   c_values;
    ws_long_t   res         =   traits_type::reg_query_info(m_hkey, NULL, NULL, NULL, NULL, NULL, &c_values, NULL, NULL, NULL, NULL);

    if(res != 0)
    {
        c_values = 0;
    }

    return c_values;
}

template <ss_typename_param_k C, ss_typename_param_k T, ss_typename_param_k A>
inline ss_typename_type_k basic_reg_key<C, T, A>::hkey_type basic_reg_key<C, T, A>::get_key_handle() const
{
    return m_hkey;
}

// Operations
template <ss_typename_param_k C, ss_typename_param_k T, ss_typename_param_k A>
inline ss_typename_type_k basic_reg_key<C, T, A>::class_type basic_reg_key<C, T, A>::open_sub_key(char_type const *sub_key_name, REGSAM samDesired /* = KEY_READ */)
{
    return class_type(m_hkey, sub_key_name);
}

template <ss_typename_param_k C, ss_typename_param_k T, ss_typename_param_k A>
inline ss_typename_type_k basic_reg_key<C, T, A>::class_type basic_reg_key<C, T, A>::create_sub_key(char_type const *sub_key_name, REGSAM samDesired /* = KEY_READ */)
{
    hkey_type hkey;

    return (ERROR_SUCCESS != traits_type::reg_create_key(m_hkey, sub_key_name, &hkey, samDesired)) ? class_type() : class_type(&hkey, sub_key_name);
}

/* The handle returned from this method MUST be closed with RegCloseKey() */
template <ss_typename_param_k C, ss_typename_param_k T, ss_typename_param_k A>
inline ss_typename_type_k basic_reg_key<C, T, A>::hkey_type basic_reg_key<C, T, A>::dup_key_handle(REGSAM samDesired /* = KEY_ALL_ACCESS */)
{
    return traits_type::key_dup(m_hkey);
}

#endif /* !__STLSOFT_DOCUMENTATION_SKIP_SECTION */

/* ////////////////////////////////////////////////////////////////////////// */

/// @} // end of group winstl_reg_library

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

#endif /* WINSTL_INCL_H_WINSTL_REG_KEY */

/* ////////////////////////////////////////////////////////////////////////// */
