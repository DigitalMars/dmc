/* /////////////////////////////////////////////////////////////////////////////
 * File:        stlsoft_fast_string_concatenator.h
 *
 * Purpose:     Fast string concatenator.
 *
 * Created:
 * Updated:     11th September 2004
 *
 * Thanks to:   Sean Kelly for picking up on my gratuitous use of pointers
 *              in the first implementation.
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


/// \file stlsoft_fast_string_concatenator.h
///
/// Fast string concatenator.

#ifndef STLSOFT_INCL_H_STLSOFT_FAST_STRING_CONCATENATOR
#define STLSOFT_INCL_H_STLSOFT_FAST_STRING_CONCATENATOR

#ifndef __STLSOFT_DOCUMENTATION_SKIP_SECTION
# define STLSOFT_VER_H_STLSOFT_FAST_STRING_CONCATENATOR_MAJOR       2
# define STLSOFT_VER_H_STLSOFT_FAST_STRING_CONCATENATOR_MINOR       0
# define STLSOFT_VER_H_STLSOFT_FAST_STRING_CONCATENATOR_REVISION    1
# define STLSOFT_VER_H_STLSOFT_FAST_STRING_CONCATENATOR_EDIT        103
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

#ifndef STLSOFT_INCL_H_STLSOFT
# include "stlsoft.h"                   // Include the STLSoft root header
#endif /* !STLSOFT_INCL_H_STLSOFT */

#if defined(__STLSOFT_COMPILER_IS_MSVC) && \
    _MSC_VER < 1200
# error stlsoft_fast_string_concatenator.h is not compatible with Visual C++ 5.0 or earlier
#endif /* _MSC_VER < 1200 */

#ifndef _STLSOFT_NO_NAMESPACE
namespace stlsoft
{
#endif /* _STLSOFT_NO_NAMESPACE */
template<   ss_typename_param_k S
        ,   ss_typename_param_k C
        ,   ss_typename_param_k T
        >
class fast_string_concatenator;
#ifndef _STLSOFT_NO_NAMESPACE
} // namespace stlsoft
#endif /* _STLSOFT_NO_NAMESPACE */

#ifndef STLSOFT_INCL_H_STLSOFT_CHAR_TRAITS
# include "stlsoft_char_traits.h"      // stlsoft::char_traits
#endif /* !STLSOFT_INCL_H_STLSOFT_CHAR_TRAITS */

/* /////////////////////////////////////////////////////////////////////////////
 * Namespace
 */

#ifndef _STLSOFT_NO_NAMESPACE
namespace stlsoft
{
#endif /* _STLSOFT_NO_NAMESPACE */

/* /////////////////////////////////////////////////////////////////////////////
 * Classes
 */

/// This is used as a seed when namespace issues make the selection of the
/// fast_string_concatenator overloads of operator +() ambiguous.
class fsc_seed
{};

#ifndef __STLSOFT_DOCUMENTATION_SKIP_SECTION

template <ss_typename_param_k S>
class fsc_seed_t
    : public fsc_seed
{
public:
    typedef S   string_type;
};

#endif /* !__STLSOFT_DOCUMENTATION_SKIP_SECTION */

#ifndef __STLSOFT_CF_TEMPLATE_CLASS_DEFAULT_CLASS_ARGUMENT_SUPPORT
# error fast_string_concatenator cannot be used when default template arguments are not supported
#endif /* !__STLSOFT_CF_TEMPLATE_CLASS_DEFAULT_CLASS_ARGUMENT_SUPPORT */

/// Class which provides fast string concatenation
template<   ss_typename_param_k S
        ,   ss_typename_param_k C = ss_typename_type_def_k S::value_type
        ,   ss_typename_param_k T = char_traits<C>
        >
class fast_string_concatenator
{
/// \name Member types
/// @{
public:
    typedef S                                   string_type;
    typedef C                                   char_type;
    typedef T                                   traits_type;
    typedef fast_string_concatenator<S, C, T>   class_type;
    typedef ss_size_t                           size_type;
private:
    typedef ss_typename_type_k S::iterator      string_iterator_type;
/// @}

/// \name Construction
/// @{
public:
    fast_string_concatenator(string_type const &lhs, string_type const &rhs);
    fast_string_concatenator(string_type const &lhs, char_type const *rhs);
    fast_string_concatenator(string_type const &lhs, char_type const rhs);
    fast_string_concatenator(char_type const *lhs, string_type const &rhs);
    fast_string_concatenator(char_type const lhs, string_type const &rhs);

    fast_string_concatenator(class_type const &lhs, string_type const &rhs);
    fast_string_concatenator(class_type const &lhs, char_type const *rhs);
    fast_string_concatenator(class_type const &lhs, char_type const rhs);

    fast_string_concatenator(fsc_seed const &lhs, string_type const &rhs);

    // These constructors are for handling embedded braces in the concatenation sequences, and represent the pathological case
    fast_string_concatenator(class_type const &lhs, class_type const &rhs);
    fast_string_concatenator(string_type const &lhs, class_type const &rhs);
    fast_string_concatenator(char_type const *lhs, class_type const &rhs);
    fast_string_concatenator(char_type const lhs, class_type const &rhs);
/// @}

/// \name Accessors
/// @{
public:
    operator string_type() const;
/// @}

/// \name Implementation
/// @{
private:
    size_type length() const
    {
        return m_lhs.length() + m_rhs.length();
    }
#if defined(STLSOFT_FAST_STRING_CONCATENATION_ASSUME_CONTIGUOUS_STORAGE)
    char_type *write(char_type *s) const
    {
        return m_rhs.write(m_lhs.write(s));
    }
#else /* ? STLSOFT_FAST_STRING_CONCATENATION_ASSUME_CONTIGUOUS_STORAGE */
    string_iterator_type write(string_iterator_type s) const
    {
        return m_rhs.write(m_lhs.write(s));
    }
#endif /* STLSOFT_FAST_STRING_CONCATENATION_ASSUME_CONTIGUOUS_STORAGE */

private:
    struct Data;

    friend struct Data;

    struct Data
    {
        enum DataType
        {
                seed    // Argument was the seed type
            ,   single  // Argument was a single character
            ,   cstring // Argument was a C-string or a string object
            ,   concat  // Argument was another concatenator
        };

        /// Represents a C-style string
        struct CString
        {
            ss_size_t           len;
            char_type const     *s;
        };
        /// Represents a union of the possible concatenation types
        union DataRef
        {
            CString             cstring;
            char_type           ch;
            class_type  const   *concat;
        };
        Data(string_type const &str)
            : type(cstring)
        {
            ref.cstring.len = str.length();
            ref.cstring.s   = str.data();
        }
        Data(char_type const *s)
            : type(cstring)
        {
            ref.cstring.len = traits_type::length(s);
            ref.cstring.s   = s;
        }
        Data(char_type const ch)
            : type(single)
        {
            ref.ch = ch;
        }
        Data(class_type const &fc)
            : type(concat)
        {
            ref.concat = &fc;
        }
        Data(fsc_seed const &)
            : type(seed)
        {}

        size_type length() const
        {
            size_type  len;

            // Note that a default is not used in the switch statement because, even on very high
            // optimisations, it caused a 1-4% hit on most of the compilers
            stlsoft_assert(type == cstring || type == single || type == concat || type == seed);

            switch(type)
            {
                case    seed:
                    len = 0;
                    break;
                case    single:
                    len = 1;
                    break;
                case    cstring:
                    len = ref.cstring.len;
                    break;
                case    concat:
                    len = ref.concat->length();
                    break;
            }

            stlsoft_assert(!(len < 0));

            return len;
        }

#if defined(STLSOFT_FAST_STRING_CONCATENATION_ASSUME_CONTIGUOUS_STORAGE)
        char_type *write(char_type *s) const
#else /* ? STLSOFT_FAST_STRING_CONCATENATION_ASSUME_CONTIGUOUS_STORAGE */
        string_iterator_type write(string_iterator_type s) const
#endif /* STLSOFT_FAST_STRING_CONCATENATION_ASSUME_CONTIGUOUS_STORAGE */
        {
            size_type  len;

            // Note that a default is not used in the switch statement because, even on very high
            // optimisations, it caused a 1-4% hit on most of the compilers
            stlsoft_assert(type == cstring || type == single || type == concat || type == seed);

#if defined(STLSOFT_FAST_STRING_CONCATENATION_ASSUME_CONTIGUOUS_STORAGE)
            // Check that "iterator" is contiguous
            stlsoft_assert(&[1]&*s == s + 1);
#else /* ? STLSOFT_FAST_STRING_CONCATENATION_ASSUME_CONTIGUOUS_STORAGE */
            // Check that iterator is random access
            stlsoft_assert(&s[1] == s + 1);
#endif /* STLSOFT_FAST_STRING_CONCATENATION_ASSUME_CONTIGUOUS_STORAGE */

            switch(type)
            {
                case    seed:
                    break;
                case    single:
                    *(s++) = ref.ch;
                    break;
                case    cstring:
                    len = ref.cstring.len;
#if defined(STLSOFT_FAST_STRING_CONCATENATION_ASSUME_CONTIGUOUS_STORAGE)
                    memcpy(s, ref.cstring.s, sizeof(C) * (len));
#else /* ? STLSOFT_FAST_STRING_CONCATENATION_ASSUME_CONTIGUOUS_STORAGE */
                    std::copy(&ref.cstring.s[0], &ref.cstring.s[len], s);
#endif /* STLSOFT_FAST_STRING_CONCATENATION_ASSUME_CONTIGUOUS_STORAGE */
                    s += len;
                    break;
                case    concat:
                    s = ref.concat->write(s);
                    break;
            }

            return s;
        }

        DataRef         ref;
        DataType const  type;
    };
/// @}

/// \name Construction
/// @{
private:
    Data    m_lhs;
    Data    m_rhs;
/// @}

// Not to be implemented
private:
    fast_string_concatenator &operator =(class_type const &);
};

/* /////////////////////////////////////////////////////////////////////////////
 * Unit-testing
 */

#ifdef STLSOFT_UNITTEST

namespace unittest
{
    ss_bool_t test_stlsoft_fast_string_concatenator(unittest_reporter *r)
    {
        ss_bool_t               bSuccess    =   true;

        unittest_initialiser    init(r, "STLSoft", "fast_string_concatenator", __FILE__);

#if 0
        if(<<TODO>>)
        {
            r->report("<<TODO>> failed ", __LINE__);
            bSuccess = false;
        }
#endif /* 0 */

        return bSuccess;
    }

    unittest_registrar    unittest_stlsoft_fast_string_concatenator(test_stlsoft_fast_string_concatenator);

} // namespace unittest

#endif /* STLSOFT_UNITTEST */

/* /////////////////////////////////////////////////////////////////////////////
 * Implementation
 */

#ifndef __STLSOFT_DOCUMENTATION_SKIP_SECTION

template<   ss_typename_param_k S
        ,   ss_typename_param_k C
        ,   ss_typename_param_k T
        >
inline fast_string_concatenator<S, C, T>::fast_string_concatenator(S const &lhs, S const &rhs)
    : m_lhs(lhs)
    , m_rhs(rhs)
{}

template<   ss_typename_param_k S
        ,   ss_typename_param_k C
        ,   ss_typename_param_k T
        >
inline fast_string_concatenator<S, C, T>::fast_string_concatenator(S const &lhs, C const *rhs)
    : m_lhs(lhs)
    , m_rhs(rhs)
{}

template<   ss_typename_param_k S
        ,   ss_typename_param_k C
        ,   ss_typename_param_k T
        >
inline fast_string_concatenator<S, C, T>::fast_string_concatenator(S const &lhs, C const rhs)
    : m_lhs(lhs)
    , m_rhs(rhs)
{}

template<   ss_typename_param_k S
        ,   ss_typename_param_k C
        ,   ss_typename_param_k T
        >
inline fast_string_concatenator<S, C, T>::fast_string_concatenator(C const *lhs, S const &rhs)
    : m_lhs(lhs)
    , m_rhs(rhs)
{}

template<   ss_typename_param_k S
        ,   ss_typename_param_k C
        ,   ss_typename_param_k T
        >
inline fast_string_concatenator<S, C, T>::fast_string_concatenator(C const lhs, S const &rhs)
    : m_lhs(lhs)
    , m_rhs(rhs)
{}

template<   ss_typename_param_k S
        ,   ss_typename_param_k C
        ,   ss_typename_param_k T
        >
inline fast_string_concatenator<S, C, T>::fast_string_concatenator(fast_string_concatenator const &lhs, S const &rhs)
    : m_lhs(lhs)
    , m_rhs(rhs)
{}

template<   ss_typename_param_k S
        ,   ss_typename_param_k C
        ,   ss_typename_param_k T
        >
inline fast_string_concatenator<S, C, T>::fast_string_concatenator(fast_string_concatenator const &lhs, C const *rhs)
    : m_lhs(lhs)
    , m_rhs(rhs)
{}

template<   ss_typename_param_k S
        ,   ss_typename_param_k C
        ,   ss_typename_param_k T
        >
inline fast_string_concatenator<S, C, T>::fast_string_concatenator(fast_string_concatenator const &lhs, C const rhs)
    : m_lhs(lhs)
    , m_rhs(rhs)
{}

// These constructors are for handling embedded braces in the concatenation sequences, and represent the pathological case
template<   ss_typename_param_k S
        ,   ss_typename_param_k C
        ,   ss_typename_param_k T
        >
inline fast_string_concatenator<S, C, T>::fast_string_concatenator(fast_string_concatenator const &lhs, fast_string_concatenator const &rhs)
    : m_lhs(lhs)
    , m_rhs(rhs)
{}

template<   ss_typename_param_k S
        ,   ss_typename_param_k C
        ,   ss_typename_param_k T
        >
inline fast_string_concatenator<S, C, T>::fast_string_concatenator(S const &lhs, fast_string_concatenator const &rhs)
    : m_lhs(lhs)
    , m_rhs(rhs)
{}

template<   ss_typename_param_k S
        ,   ss_typename_param_k C
        ,   ss_typename_param_k T
        >
inline fast_string_concatenator<S, C, T>::fast_string_concatenator(C const *lhs, fast_string_concatenator const &rhs)
    : m_lhs(lhs)
    , m_rhs(rhs)
{}

template<   ss_typename_param_k S
        ,   ss_typename_param_k C
        ,   ss_typename_param_k T
        >
inline fast_string_concatenator<S, C, T>::fast_string_concatenator(C const lhs, fast_string_concatenator const &rhs)
    : m_lhs(lhs)
    , m_rhs(rhs)
{}

template<   ss_typename_param_k S
        ,   ss_typename_param_k C
        ,   ss_typename_param_k T
        >
inline fast_string_concatenator<S, C, T>::fast_string_concatenator(fsc_seed const &lhs, S const &rhs)
    : m_lhs(lhs)
    , m_rhs(rhs)
{}

template<   ss_typename_param_k S
        ,   ss_typename_param_k C
        ,   ss_typename_param_k T
        >
#if defined(__STLSOFT_COMPILER_IS_GCC)
inline fast_string_concatenator<S, C, T>::operator S() const
#else /* ? __STLSOFT_COMPILER_IS_GCC */
inline fast_string_concatenator<S, C, T>::operator ss_typename_type_k fast_string_concatenator<S, C, T>::string_type() const
#endif /* __STLSOFT_COMPILER_IS_GCC */
{
    size_type   len = length();
    string_type s(len, '~');
#if defined(STLSOFT_FAST_STRING_CONCATENATION_ASSUME_CONTIGUOUS_STORAGE)

    char_type   &c0 = s.operator[](0);

// TODO: Need to fix this up, since assumes that all string types use contiguous storage? Or at least verify that it's got no problem as is

//    *write(&c0) = '\0'; // This assignment may not be necessary
    write(&c0);

    stlsoft_assert(s.length() == traits_type::length(&c0));
#else /* ? STLSOFT_FAST_STRING_CONCATENATION_ASSUME_CONTIGUOUS_STORAGE */
    write(s.begin());
#endif /* STLSOFT_FAST_STRING_CONCATENATION_ASSUME_CONTIGUOUS_STORAGE */

    stlsoft_assert(s.length() == traits_type::length(s.c_str()));

    return s;
}

#endif /* !__STLSOFT_DOCUMENTATION_SKIP_SECTION */

/* /////////////////////////////////////////////////////////////////////////////
 * operator +
 */

template<   ss_typename_param_k S
        ,   ss_typename_param_k C
        ,   ss_typename_param_k T
        >
inline fast_string_concatenator<S, C, T> operator +(fsc_seed const &lhs, S const &rhs)
{
    return fast_string_concatenator<S, C, T>(lhs, rhs);
}

#if 0
template<   ss_typename_param_k S
        ,   ss_typename_param_k C
        ,   ss_typename_param_k T
        >
inline fast_string_concatenator<S, C, T> operator +(S const &lhs, fsc_seed const &rhs)
{
    return fast_string_concatenator<S, C, T>(lhs, rhs);
}
#endif /* 0 */

template<   ss_typename_param_k S
        ,   ss_typename_param_k C
        ,   ss_typename_param_k T
        >
inline fast_string_concatenator<S, C, T> const &operator +(fsc_seed const &lhs, fast_string_concatenator<S, C, T> const &rhs)
{
    return rhs;
}

#if 0
template<   ss_typename_param_k S
        ,   ss_typename_param_k C
        ,   ss_typename_param_k T
        >
inline fast_string_concatenator<S, C, T> const &operator +(fast_string_concatenator<S, C, T> const &lhs, fsc_seed const &rhs)
{
    return rhs;
}
#endif /* 0 */




#if 0 /* These methods are not required, since they must be specifically provided for a given string class anyway */
template<   ss_typename_param_k S
        ,   ss_typename_param_k C
        ,   ss_typename_param_k T
        >
inline fast_string_concatenator<S, C, T> operator +(S const &lhs, S const &rhs)
{
    return fast_string_concatenator<S, C, T>(lhs, rhs);
}

template<   ss_typename_param_k S
        ,   ss_typename_param_k C
        ,   ss_typename_param_k T
        >
inline fast_string_concatenator<S, C, T> operator +(S const &lhs, C const *rhs)
{
    return fast_string_concatenator<S, C, T>(lhs, rhs);
}

template<   ss_typename_param_k S
        ,   ss_typename_param_k C
        ,   ss_typename_param_k T
        >
inline fast_string_concatenator<S, C, T> operator +(S const &lhs, C const rhs)
{
    return fast_string_concatenator<S, C, T>(lhs, rhs);
}

template<   ss_typename_param_k S
        ,   ss_typename_param_k C
        ,   ss_typename_param_k T
        >
inline fast_string_concatenator<S, C, T> operator +(C const *lhs, S const &rhs)
{
    return fast_string_concatenator<S, C, T>(lhs, rhs);
}

template<   ss_typename_param_k S
        ,   ss_typename_param_k C
        ,   ss_typename_param_k T
        >
inline fast_string_concatenator<S, C, T> operator +(C const lhs, S const &rhs)
{
    return fast_string_concatenator<S, C, T>(lhs, rhs);
}
#endif /* 0 */

template<   ss_typename_param_k S
        ,   ss_typename_param_k C
        ,   ss_typename_param_k T
        >
inline fast_string_concatenator<S, C, T> operator +(fast_string_concatenator<S, C, T> const &lhs, S const &rhs)
{
    return fast_string_concatenator<S, C, T>(lhs, rhs);
}

template<   ss_typename_param_k S
        ,   ss_typename_param_k C
        ,   ss_typename_param_k T
        >
inline fast_string_concatenator<S, C, T> operator +(fast_string_concatenator<S, C, T> const &lhs, C const *rhs)
{
    return fast_string_concatenator<S, C, T>(lhs, rhs);
}

template<   ss_typename_param_k S
        ,   ss_typename_param_k C
        ,   ss_typename_param_k T
        >
inline fast_string_concatenator<S, C, T> operator +(fast_string_concatenator<S, C, T> const &lhs, C const rhs)
{
    return fast_string_concatenator<S, C, T>(lhs, rhs);
}

// These operators are for handling embedded braces in the concatenation sequences, and represent the pathological case
template<   ss_typename_param_k S
        ,   ss_typename_param_k C
        ,   ss_typename_param_k T
        >
inline fast_string_concatenator<S, C, T> operator +(fast_string_concatenator<S, C, T> const &lhs, fast_string_concatenator<S, C, T> const &rhs)
{
    return fast_string_concatenator<S, C, T>(lhs, rhs);
}

template<   ss_typename_param_k S
        ,   ss_typename_param_k C
        ,   ss_typename_param_k T
        >
inline fast_string_concatenator<S, C, T> operator +(S const &lhs, fast_string_concatenator<S, C, T> const &rhs)
{
    return fast_string_concatenator<S, C, T>(lhs, rhs);
}

template<   ss_typename_param_k S
        ,   ss_typename_param_k C
        ,   ss_typename_param_k T
        >
inline fast_string_concatenator<S, C, T> operator +(C const *lhs, fast_string_concatenator<S, C, T> const &rhs)
{
    return fast_string_concatenator<S, C, T>(lhs, rhs);
}

template<   ss_typename_param_k S
        ,   ss_typename_param_k C
        ,   ss_typename_param_k T
        >
inline fast_string_concatenator<S, C, T> operator +(C const lhs, fast_string_concatenator<S, C, T> const &rhs)
{
    return fast_string_concatenator<S, C, T>(lhs, rhs);
}

/* ////////////////////////////////////////////////////////////////////////// */

#ifndef _STLSOFT_NO_NAMESPACE
} // namespace stlsoft
#endif /* _STLSOFT_NO_NAMESPACE */

/* ////////////////////////////////////////////////////////////////////////// */

#endif /* !STLSOFT_INCL_H_STLSOFT_FAST_STRING_CONCATENATOR */

/* ////////////////////////////////////////////////////////////////////////// */
