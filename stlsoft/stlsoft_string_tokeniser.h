/* /////////////////////////////////////////////////////////////////////////////
 * File:        stlsoft_string_tokeniser.h (originally MTToken.h, ::SynesisStl)
 *
 * Purpose:     Simple token parsing class.
 *
 * Created:     6th January 2001
 * Updated:     11th September 2004
 *
 * Home:        http://stlsoft.org/
 *
 * Copyright (c) 2001-2004, Matthew Wilson and Synesis Software
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


/// \file stlsoft_string_tokeniser.h
///
/// Simple token parsing class.

#ifndef STLSOFT_INCL_H_STLSOFT_STRING_TOKENISER
#define STLSOFT_INCL_H_STLSOFT_STRING_TOKENISER

#ifndef __STLSOFT_DOCUMENTATION_SKIP_SECTION
# define STLSOFT_VER_H_STLSOFT_STRING_TOKENISER_MAJOR       3
# define STLSOFT_VER_H_STLSOFT_STRING_TOKENISER_MINOR       0
# define STLSOFT_VER_H_STLSOFT_STRING_TOKENISER_REVISION    1
# define STLSOFT_VER_H_STLSOFT_STRING_TOKENISER_EDIT        144
#endif /* !__STLSOFT_DOCUMENTATION_SKIP_SECTION */

/* /////////////////////////////////////////////////////////////////////////////
 * Compatibility
 */

/*
[Incompatibilies-start]
__STLSOFT_COMPILER_IS_DMC: __DMC__<0x0839
__STLSOFT_COMPILER_IS_MSVC: _MSC_VER<1100
[Incompatibilies-end]
 */

/* /////////////////////////////////////////////////////////////////////////////
 * Includes
 */

#ifndef STLSOFT_INCL_H_STLSOFT
# include "stlsoft.h"           // Include the STLSoft root header
#endif /* !STLSOFT_INCL_H_STLSOFT */

#if defined(__STLSOFT_COMPILER_IS_DMC) && \
    __DMC__ < 0x0839
# error stlsoft_string_tokeniser.h is not compatible with Digital Mars C/C++ 3.38 or earlier
#endif /* _MSC_VER < 1200 */
#if defined(__STLSOFT_COMPILER_IS_MSVC) && \
    _MSC_VER < 1100
# error stlsoft_string_tokeniser.h is not compatible with Visual C++ 5.0 or earlier
#endif /* _MSC_VER < 1200 */

#ifndef STLSOFT_INCL_H_STLSOFT_ITERATOR
# include "stlsoft_iterator.h"          // iterator_base
#endif /* !STLSOFT_INCL_H_STLSOFT_ITERATOR */
#ifdef STLSOFT_UNITTEST
# ifndef STLSOFT_INCL_H_STLSOFT_SIMPLE_STRING
#  include "stlsoft_simple_string.h"    // basic_simple_string
# endif /* !STLSOFT_INCL_H_STLSOFT_SIMPLE_STRING */
# include <string>                      // std::basic_string
#endif /* STLSOFT_UNITTEST */
#ifndef STLSOFT_INCL_H_STLSOFT_STRING_ACCESS
# include "stlsoft_string_access.h"     // c_str_ptr
#endif /* !STLSOFT_INCL_H_STLSOFT_STRING_ACCESS */
#include <iterator>                     // std::distance

/* /////////////////////////////////////////////////////////////////////////////
 * Namespace
 */

#ifndef _STLSOFT_NO_NAMESPACE
namespace stlsoft
{
#endif /* _STLSOFT_NO_NAMESPACE */

/* ////////////////////////////////////////////////////////////////////////// */

/// \weakgroup libraries STLSoft Libraries
/// \brief The individual libraries

/// \weakgroup libraries_string String Library
/// \ingroup libraries
/// \brief This library provides facilities for defining and manipulating strings

/// \weakgroup stlsoft_string_library String Library (STLSoft)
/// \ingroup STLSoft libraries_string
/// \brief This library provides facilities for defining and manipulating strings
/// @{

/* ////////////////////////////////////////////////////////////////////////////
 * Classes
 */

// string_tokeniser_ignore_blanks

/// Ignore blanks type
///
/// \param B A boolean as to whether blanks should be ignored (\c true) or preserved (\c false)
template <ss_bool_t B>
struct string_tokeniser_ignore_blanks
{
    enum { value = B };
};

// string_tokeniser_type_traits
/// Traits class for the string tokeniser string type
///
/// \param S The string tokeniser string type
/// \param V The string tokeniser value type
template<   ss_typename_param_k S
        ,   ss_typename_param_k V
        >
struct string_tokeniser_type_traits
{
private:
    /// The string type
    typedef S                                       string_type;
    /// The tokeniser value type
    typedef V                                       tokeniser_value_type;

public:
    /// The value type
    typedef ss_typename_type_k S::value_type        value_type;
    /// The iterator type
    typedef ss_typename_type_k S::iterator          iterator_type;
    /// The non-mutable (const) iterator type
    typedef ss_typename_type_k S::const_iterator    const_iterator_type;
    /// The size type
    typedef ss_typename_type_k S::size_type         size_type;
    /// The difference type
    typedef ss_typename_type_k S::difference_type   difference_type;

    /// Returns the start of the contained sequence of the given string
    static const_iterator_type begin(S const &s)
    {
        return s.begin();
    }

    /// Returns the end of the contained sequence of the given string
    static const_iterator_type end(S const &s)
    {
        return s.end();
    }

    /// Creates an instance of the string from the given range [f:t)
    static tokeniser_value_type create(const_iterator_type f, const_iterator_type t)
    {
        /* There's a bug in the Metrowerks 3.0 standard library string
         * implementation, such that constructing from a range leaves
         * a corrupted sequence due to an optimistic allocation
         * requirement calculation result being stored as the length
         */
#if defined(__STLSOFT_COMPILER_IS_MWERKS) || \
    (   defined(__STLSOFT_COMPILER_IS_MSVC) && \
        _MSC_VER >= 1300)
        return tokeniser_value_type(&*f, static_cast<size_type>(t - f));
#else
        return tokeniser_value_type(f, t);
#endif /* __STLSOFT_COMPILER_IS_MWERKS */
    }
};

#ifndef __STLSOFT_DOCUMENTATION_SKIP_SECTION

STLSOFT_TEMPLATE_SPECIALISATION
struct string_tokeniser_type_traits<ss_char_a_t, ss_char_a_t>
{
    typedef ss_char_a_t                             value_type;
    typedef ss_char_a_t                             *iterator_type;
    typedef ss_char_a_t const                       *const_iterator_type;
    typedef ss_size_t                               size_type;
};

STLSOFT_TEMPLATE_SPECIALISATION
struct string_tokeniser_type_traits<ss_char_w_t, ss_char_w_t>
{
    typedef ss_char_w_t                             value_type;
    typedef ss_char_w_t                             *iterator_type;
    typedef ss_char_w_t const                       *const_iterator_type;
    typedef ss_size_t                               size_type;
};

#endif /* !__STLSOFT_DOCUMENTATION_SKIP_SECTION */

// string_tokeniser_comparator
/// String tokeniser comparator
///
/// \param D The delimiter type
/// \param S The string type
/// \param T The traits type
template<   ss_typename_param_k D
        ,   ss_typename_param_k S
        ,   ss_typename_param_k T
        >
struct string_tokeniser_comparator
{
public:
    /// The delimiter type
    typedef D                                                   delimiter_type;
    /// The string type
    typedef S                                                   string_type;
    /// The traits type
    typedef T                                                   traits_type;
    /// The non-mutating (const) iterator type
    typedef ss_typename_type_k traits_type::const_iterator_type const_iterator;

private:
#ifdef __STLSOFT_CF_MEMBER_TEMPLATE_FUNCTION_SUPPORT
    /// Evaluates whether the contents of the two sequences are equivalent to the given extent
    template <ss_typename_param_k I1, ss_typename_param_k I2>
    static ss_bool_t is_equal(I1 p1, I2 p2, ss_size_t c)
    {
        for(; c-- > 0; ++p1, ++p2)
        {
            if(*p1 != *p2)
            {
                return false;
            }
        }

        return true;
    }

    /// Evaluates whether the delimiter and the sequence are equivalent to the extent of the delimiter
    template <ss_typename_param_k D_, ss_typename_param_k I_>
    static ss_bool_t is_equal(D_ const &delimiter, I_ &p2)
    {
        return is_equal(delimiter.begin(), p2, delimiter.length());
    }

    /// Returns the length of the delimiter
    template <ss_typename_param_k D_>
    static ss_size_t get_length(D_ const &delimiter)
    {
        return delimiter.length();
    }
#else /* ? __STLSOFT_CF_MEMBER_TEMPLATE_FUNCTION_SUPPORT */
    /// Evaluates whether the contents of the two sequences are equivalent to the given extent
    static ss_bool_t is_equal(string_type const &lhs, ss_typename_type_k string_type::value_type const *rhs)
    {
        return lhs.compare(rhs) == 0;
    }

    /// Returns the length of the delimiter
    static ss_size_t get_length(string_type const &s)
    {
        return s.length();
    }
#endif // __STLSOFT_CF_MEMBER_TEMPLATE_FUNCTION_SUPPORT

    /// Evaluates whether the delimiter and the sequence are equivalent to the extent of the delimiter
    static ss_bool_t is_equal(ss_char_a_t const delimiter, const_iterator &it)
    {
        return delimiter == *it;
    }
    /// Evaluates whether the delimiter and the sequence are equivalent to the extent of the delimiter
    static ss_bool_t is_equal(ss_char_w_t const delimiter, const_iterator &it)
    {
        return delimiter == *it;
    }
    /// Returns the length of the delimiter
    static ss_size_t get_length(ss_char_a_t const /* delimiter */)
    {
        return 1;
    }
    /// Returns the length of the delimiter
    static ss_size_t get_length(ss_char_w_t const /* delimiter */)
    {
        return 1;
    }

public:
    /// Evaluates whether the delimiter and the sequence are equivalent to the extent of the delimiter
    static ss_bool_t equal(delimiter_type const &delimiter, const_iterator &it)
    {
        return is_equal(delimiter, it);
    }

    /// Evaluates whether the delimiter and the sequence are not equivalent to the extent of the delimiter
    static ss_bool_t not_equal(delimiter_type const &delimiter, const_iterator &it)
    {
        return !is_equal(delimiter, it);
    }

    /// Returns the length of the delimiter
    static ss_size_t length(delimiter_type const &delimiter)
    {
        return get_length(delimiter);
    }
};

/// String tokeniser
///
/// This class takes a string, and a delimiter, and fashions a sequence from
/// the given string, with each element determined with respect to the delimiter
///
/// \param S The string type
/// \param D The delimiter type (can be a string type or a character type)
/// \param B The ignore-blanks type
/// \param T The string type traits type
/// \param P The tokeniser comparator type
/// \param V The value type (the string type that will be used for the values)
template<   ss_typename_param_k S
        ,   ss_typename_param_k D
#ifdef __STLSOFT_CF_TEMPLATE_CLASS_DEFAULT_CLASS_ARGUMENT_SUPPORT
        ,   ss_typename_param_k B = string_tokeniser_ignore_blanks<true>
        ,   ss_typename_param_k V = S
        ,   ss_typename_param_k T = string_tokeniser_type_traits<S, V>
        ,   ss_typename_param_k P = string_tokeniser_comparator<D, S, T>
#else /* ? __STLSOFT_CF_TEMPLATE_CLASS_DEFAULT_CLASS_ARGUMENT_SUPPORT */
        ,   ss_typename_param_k B
        ,   ss_typename_param_k V
        ,   ss_typename_param_k T
        ,   ss_typename_param_k P
#endif /* __STLSOFT_CF_TEMPLATE_CLASS_DEFAULT_CLASS_ARGUMENT_SUPPORT */
        >
class string_tokeniser
{
public:
    /// The current parameterisation of the type
    typedef string_tokeniser<S, D, B, V, T, P>              class_type;
    /// This tokeniser parameterisation
    typedef string_tokeniser<S, D, B, V, T, P>              tokeniser_type;
    /// The sequence string type
    typedef S                                               string_type;
    /// The delimiter type
    typedef D                                               delimiter_type;
    /// The ignore-blanks type
    typedef B                                               ignore_blanks_type;
    /// The value type
    typedef V                                               value_type;
    /// The traits type
    typedef T                                               traits_type;
    /// The tokeniser comparator type
    typedef P                                               comparator_type;
    /// The char type
    typedef ss_typename_type_k traits_type::value_type      char_type;
    /// The size type
    typedef ss_typename_type_k traits_type::size_type       size_type;
    /// The difference type
    typedef ss_typename_type_k traits_type::difference_type difference_type;

    /// The const_reference type
    typedef const value_type                                const_reference;

    class   const_iterator;

// Construction
public:
    /// Tokenise the given string with the given delimiter
    string_tokeniser(string_type const &str, delimiter_type const &delimiter)
        : m_str(str)
        , m_delimiter(delimiter)
    {
        stlsoft_message_assert("Delimiter of zero-length", comparator_type::length(m_delimiter) > 0);
    }

    /// Tokenise the given string with the given delimiter
    string_tokeniser(char_type const *psz, delimiter_type const &delimiter)
        : m_str(psz)
        , m_delimiter(delimiter)
    {
        stlsoft_message_assert("Delimiter of zero-length", comparator_type::length(m_delimiter) > 0);
    }

#if defined(__STLSOFT_CF_MEMBER_TEMPLATE_FUNCTION_SUPPORT) && \
    (   !defined(__STLSOFT_COMPILER_IS_MSVC) || \
        _MSC_VER > 1100)
    /// Commence a search according to the given search pattern and flags, relative to \c directory
    template <ss_typename_param_k S1>
    string_tokeniser(S1 const &s, delimiter_type const &delimiter)
        : m_str(c_str_ptr(s))
        , m_delimiter(delimiter)
    {
        stlsoft_message_assert("Delimiter of zero-length", comparator_type::length(m_delimiter) > 0);
    }
#endif /* __STLSOFT_CF_MEMBER_TEMPLATE_FUNCTION_SUPPORT */

    /// Tokenise the specified length of the given string with the given delimiter
    string_tokeniser(char_type const *psz, size_type cch, delimiter_type const &delimiter)
        : m_str(psz, cch)
        , m_delimiter(delimiter)
    {
        stlsoft_message_assert("Delimiter of zero-length", comparator_type::length(m_delimiter) > 0);
    }

    /// \brief Tokenise the given range with the given delimiter
    ///
    /// \param from The start of the asymmetric range to tokenise
    /// \param to The start of the asymmetric range to tokenise
    /// \param delimiter The delimiter to use
    string_tokeniser(char_type const *from, char_type const *to, delimiter_type const &delimiter)
        : m_str(from, to)
        , m_delimiter(delimiter)
    {
        stlsoft_message_assert("Delimiter of zero-length", comparator_type::length(m_delimiter) > 0);
    }

#if defined(__STLSOFT_CF_MEMBER_TEMPLATE_CTOR_SUPPORT) && \
    (   !defined(__STLSOFT_COMPILER_IS_MSVC) || \
        _MSC_VER >= 1200)
    /// Tokenise the given range with the given delimiter
    ///
    /// \param from The start of the asymmetric range to tokenise
    /// \param to The start of the asymmetric range to tokenise
    /// \param delimiter The delimiter to use
    template <ss_typename_param_k I>
    string_tokeniser(I from, I to, delimiter_type const &delimiter)
        : m_str(from, to)
        , m_delimiter(delimiter)
    {
        stlsoft_message_assert("Delimiter of zero-length", comparator_type::length(m_delimiter) > 0);
    }
#endif /* __STLSOFT_CF_MEMBER_TEMPLATE_CTOR_SUPPORT */

// Iteration
public:
    /// Iterator for string_tokeniser, supporting the Forward Iterator concept
    class const_iterator
        : public iterator_base<stlsoft_ns_qual_std(input_iterator_tag), S, ss_ptrdiff_t, void *, S>
    {
    public:
        /// The type
        typedef const_iterator                                      class_type;
#if defined(__STLSOFT_COMPILER_IS_DMC) || \
    defined(__STLSOFT_COMPILER_IS_MSVC)
        /// The delimiter type
        typedef delimiter_type                                      delimiter_type;
        /// The value type
        typedef value_type                                          value_type;
        /// The traits type
        typedef traits_type                                         traits_type;
#else
        /// The delimiter type
        typedef ss_typename_type_k tokeniser_type::delimiter_type   delimiter_type;
        /// The value type
        typedef ss_typename_type_k tokeniser_type::value_type       value_type;
        /// The traits type
        typedef ss_typename_type_k tokeniser_type::traits_type      traits_type;
#endif /* __SYNSOFT_DBS_DEBUG */

    // Construction
    public:
        /// Default constructor
        const_iterator()
            : m_find0(NULL)
            , m_find1(NULL)
            , m_next(NULL)
            , m_end(NULL)
            , m_delimiter(delimiter_type())
            , m_cchDelimiter(0)
        {}

        /// Copy constructor
        const_iterator(const_iterator const &rhs)
            : m_find0(rhs.m_find0)
            , m_find1(rhs.m_find1)
            , m_next(rhs.m_next)
            , m_end(rhs.m_end)
            , m_delimiter(rhs.m_delimiter)
            , m_cchDelimiter(comparator_type::length(rhs.m_delimiter))
        {}

    // Operators
    public:
        /// Dereference operator
#if 0 && \
    !defined(__STLSOFT_DOCUMENTATION_SKIP_SECTION) && \
    (   defined(__STLSOFT_COMPILER_IS_INTEL) || \
        defined(__STLSOFT_COMPILER_IS_MSVC)) && \
    _MSC_VER > 1100
// This is just a bit of an experiment, so not documented

template <bool B>
struct dereference_operator
{
};

template <>
struct dereference_operator<true>
{
    value_type operator ()(
        ss_typename_type_k traits_type::const_iterator_type _find0,
        ss_typename_type_k traits_type::const_iterator_type _find1,
        delimiter_type const                                &/* _delimiter */,
        ss_size_t                                           /* _cchDelimiter */
        ) const
    {
        return traits_type::create(_find0, _find1);
    }
};

template <>
struct dereference_operator<false>
{
    value_type operator ()(
        ss_typename_type_k traits_type::const_iterator_type _find0,
        ss_typename_type_k traits_type::const_iterator_type _find1,
        delimiter_type const                                &_delimiter,
        ss_size_t                                           _cchDelimiter
        ) const
    {
        ss_typename_type_k traits_type::const_iterator_type find1 = _find1;

        if(find1 != _find0)
        {
            ss_typename_type_k traits_type::const_iterator_type findp = find1;

            findp -= _cchDelimiter;

            if(comparator_type::equal(_delimiter, findp))
            {
                find1 = findp;
            }
        }

        return traits_type::create(_find0, find1);
    }
};


        const_reference operator *() const
        {
            return dereference_operator<ignore_blanks_type::value>()(m_find0, m_find1, m_delimiter, m_cchDelimiter);
        }
#else
        // This has to be V, rather than value_type, because Visual C++ thinks that S is the value_type!!
        V operator *() const
        {
            if(ignore_blanks_type::value)
            {
                return traits_type::create(m_find0, m_find1);
            }
            else
            {
                ss_typename_type_k traits_type::const_iterator_type find1 = m_find1;

                if(find1 != m_find0)
                {
                    ss_typename_type_k traits_type::const_iterator_type findp = find1;

                    findp -= m_cchDelimiter;

                    if(comparator_type::equal(m_delimiter, findp))
                    {
                        find1 = findp;
                    }
                }

                return traits_type::create(m_find0, find1);
            }
        }
#endif /* !__STLSOFT_COMPILER_IS_DMC */

        /// Pre-increment operator
        const_iterator &operator ++()
        {
            increment_();

            return *this;
        }

        /// Post-increment operator
        const const_iterator operator ++(int)
        {
            class_type  ret(*this);

            operator ++();

            return ret;
        }

        /// Evaluates whether \c this and \c rhs are equivalent
        ss_bool_t operator == (const_iterator const &rhs) const
        {
            stlsoft_message_assert("Comparing iterators from different tokenisers", m_end == rhs.m_end);

            return m_find0 == rhs.m_find0;
        }

        /// Evaluates whether \c this and \c rhs are not equivalent
        ss_bool_t operator != (const_iterator const &rhs) const
        {
            stlsoft_message_assert("Comparing iterators from different tokenisers", m_end == rhs.m_end);

            return m_find0 != rhs.m_find0;
        }

    // Implementation
    private:
        friend class    string_tokeniser<S, D, B, V, T, P>;

        void increment_()
        {
            // This is a three phase algorithm:
            //
            // (i) Firstly, skip all the empty items if that is what
            //     is required
            // (ii) Firstly move m_find0 to the start of the next item

            if(m_find0 != m_end)
            {
                if(ignore_blanks_type::value)
                {
                    // (i) Skip blanks until at start of next item
                    for(m_find0 = m_find1; m_find0 != m_end; )
                    {
                        if(comparator_type::not_equal(m_delimiter, m_find0))
                        {
                            break;
                        }
                        else
                        {
                            m_find0 +=  m_cchDelimiter;
                        }
                    }
                }
                else
                {
                    m_find0 = m_find1;
                }

                // (ii) Move find1 to end of current item (starting from m_find0)
                for(m_find1 = m_find0; ; )
                {
                    if(m_find1 == m_end)
                    {
                        break;
                    }
                    else if(comparator_type::not_equal(m_delimiter, m_find1))
                    {
                        ++m_find1;
                    }
                    else
                    {
                        m_next = m_find1 + m_cchDelimiter;

                        if(!ignore_blanks_type::value)
                        {
                            m_find1 = m_next;
                        }

                        break;
                    }
                }
            }
        }

        const_iterator(ss_typename_type_k traits_type::const_iterator_type first, ss_typename_type_k traits_type::const_iterator_type last, delimiter_type const &delimiter)
            : m_find0(first)
            , m_find1(first)
            , m_next(first)
            , m_end(last)
            , m_delimiter(delimiter)
            , m_cchDelimiter(comparator_type::length(delimiter))
        {
            increment_();
        }

    // Members
    private:
        ss_typename_type_k traits_type::const_iterator_type         m_find0;        // the start of the current item
        ss_typename_type_k traits_type::const_iterator_type         m_find1;        // the end of the current item
        ss_typename_type_k traits_type::const_iterator_type         m_next;         // the start of the next valid (non-null) item
        ss_typename_type_k traits_type::const_iterator_type const   m_end;          // end point of controlled sequence
        delimiter_type const                                        m_delimiter;
        ss_size_t                                                   m_cchDelimiter;

    // Not to be implemented
#if !defined(__STLSOFT_COMPILER_IS_COMO)
    private:
#else /* ? __STLSOFT_COMPILER_IS_COMO */
    public:
#endif /* __STLSOFT_COMPILER_IS_COMO */
        class_type const &operator =(class_type const &);
    };

// Iteration
public:
    /// Begins the iteration
    ///
    /// \return An iterator representing the start of the sequence
    const_iterator begin() const
    {
        return const_iterator(traits_type::begin(m_str), traits_type::end(m_str), m_delimiter);
    }
    /// Ends the iteration
    ///
    /// \return An iterator representing the end of the sequence
    const_iterator end() const
    {
        return const_iterator(traits_type::end(m_str), traits_type::end(m_str), m_delimiter);
    }

// Attributes
public:
    /// Returns the number of elements in the sequence
    size_type   size() const
    {
        // Since the expense incurred in iterating through a range appears to
        // be largely due to the creation of the strings, it is assumed that
        // there is little point in providing a more bare-bones implementation
        // than the following.
#if defined(__STLSOFT_COMPILER_IS_DMC)

        const_iterator  b   =   begin();
        const_iterator  e   =   end();
        size_t          n   =   0;

        for(n = 0; b != e; ++n, ++b)
        {}

        return n;
#else /* ? compiler */
        return stlsoft_ns_qual_std(distance)(begin(), end());
#endif /* compiler */
    }

    /// Indicates whether the search sequence is empty
    ss_bool_t   empty() const
    {
        return begin() == end();
    }

/// \name Invariant
private:
    ss_bool_t is_valid_() const
    {
        return true;
    }

// Members
private:
    string_type const       m_str;
    delimiter_type const    m_delimiter;

// Not to be implemented
private:
    class_type const &operator =(class_type const &);
};

/* /////////////////////////////////////////////////////////////////////////////
 * Unit-testing
 */

#ifdef STLSOFT_UNITTEST

namespace unittest
{
    ss_bool_t test_stlsoft_string_tokeniser(unittest_reporter *r)
    {
        typedef basic_simple_string<char>                           string_t;
        typedef basic_simple_string<char>                           str_delim_t;
        typedef string_tokeniser_type_traits<string_t, string_t>    tt_traits_t;

        typedef string_tokeniser<   string_t
                                ,   char
                                ,   string_tokeniser_ignore_blanks<true>
                                ,   string_t
                                ,   tt_traits_t
                                ,   string_tokeniser_comparator<char, string_t, tt_traits_t >
                                >       tokeniser_char_T;
        typedef string_tokeniser<   string_t
                                ,   char
                                ,   string_tokeniser_ignore_blanks<false>
                                ,   string_t
                                ,   tt_traits_t
                                ,   string_tokeniser_comparator<char, string_t, tt_traits_t >
                                >       tokeniser_char_F;
#if !defined(__STLSOFT_COMPILER_IS_MSVC) || \
    _MSC_VER >= 1200
        typedef string_tokeniser<   string_t
                                ,   str_delim_t
                                ,   string_tokeniser_ignore_blanks<true>
                                ,   string_t
                                ,   tt_traits_t
                                ,   string_tokeniser_comparator<str_delim_t, string_t, tt_traits_t >
                                >       tokeniser_string_T;
        typedef string_tokeniser<   string_t
                                ,   str_delim_t
                                ,   string_tokeniser_ignore_blanks<false>
                                ,   string_t
                                ,   tt_traits_t
                                ,   string_tokeniser_comparator<str_delim_t, string_t, tt_traits_t >
                                >   tokeniser_string_F;
#endif /* compiler */

        struct test_sequence
        {
            char const  *sequence;
            ss_size_t   count_char_T;
            ss_size_t   count_char_F;
            ss_size_t   count_string_T;
            ss_size_t   count_string_F;
        };
        char const      delim_c             =   ';';
        char const      *delim_s            =   "<>";
        test_sequence   test_sequences[]    =
        {
                {   ";x;"                                           ,   1,  2,  1,  1   }
            ,   {   "<>x<>"                                         ,   1,  1,  1,  2   }
            ,   {   ""                                              ,   0,  0,  0,  0   }
            ,   {   ";"                                             ,   0,  1,  1,  1   }
            ,   {   ";;;;;;;;;;"                                    ,   0,  10, 1,  1   }
            ,   {   ";a;b"                                          ,   2,  3,  1,  1   }
            ,   {   "a;b;"                                          ,   2,  2,  1,  1   }
            ,   {   "a;b"                                           ,   2,  2,  1,  1   }
            ,   {   ";a;b;"                                         ,   2,  3,  1,  1   }
            ,   {   "x"                                             ,   1,  1,  1,  1   }
            ,   {   ";x"                                            ,   1,  2,  1,  1   }
            ,   {   "x;"                                            ,   1,  1,  1,  1   }
            ,   {   ";x;"                                           ,   1,  2,  1,  1   }
            ,   {   "x;;"                                           ,   1,  2,  1,  1   }
            ,   {   ";;x"                                           ,   1,  3,  1,  1   }
            ,   {   ";;x;;"                                         ,   1,  4,  1,  1   }
            ,   {   ";%BIN%;b2;;;WBEM;blah;;;bfg"                   ,   5,  10, 1,  1   }
            ,   {   "<<><>abc<sdfsdf<>sdfs><><><><>sdfsdf<>s<><><>" ,   1,  1,  5,  11  }
            ,   {   "<>"                                            ,   1,  1,  0,  1   }
            ,   {   "<><>"                                          ,   1,  1,  0,  2   }
            ,   {   "x<><>"                                         ,   1,  1,  1,  2   }
            ,   {   "<>x<>"                                         ,   1,  1,  1,  2   }
            ,   {   "<><>x"                                         ,   1,  1,  1,  3   }
            ,   {   "x;;;;;"                                        ,   1,  5,  1,  1   }
        };

        ss_bool_t               bSuccess    =   true;

        unittest_initialiser    init(r, "STLSoft", "basic_simple_string", __FILE__);

        test_sequence   *b  =   &test_sequences[0];
        test_sequence   *e  =   &test_sequences[sizeof(test_sequences) / sizeof(test_sequences[0])];

        for(; b != e; ++b)
        {
            ss_size_t   n;

            n = tokeniser_char_T((*b).sequence, delim_c).size();
            if((*b).count_char_T != n)
            {
                r->report("tokeniser<, char, <true>, ...>  failed ", __LINE__);
                bSuccess = false;
            }

            n = tokeniser_char_F((*b).sequence, delim_c).size();
            if((*b).count_char_F != n)
            {
                r->report("tokeniser<, char, <false>, ...>  failed ", __LINE__);
                bSuccess = false;
            }

#if !defined(__STLSOFT_COMPILER_IS_MSVC) || \
    _MSC_VER >= 1300
            n = tokeniser_string_T((*b).sequence, delim_s).size();
            if((*b).count_string_T != n)
            {
                r->report("tokeniser<, string, <true>, ...>  failed ", __LINE__);
                bSuccess = false;
            }

            n = tokeniser_string_F((*b).sequence, delim_s).size();
            if((*b).count_string_F != n)
            {
                r->report("tokeniser<, string, <false>, ...>  failed ", __LINE__);
                bSuccess = false;
            }
#endif /* compiler */
        }

        return bSuccess;
    }

    unittest_registrar    unittest_stlsoft_string_tokeniser(test_stlsoft_string_tokeniser);

} // namespace winstl_test

#endif /* STLSOFT_UNITTEST */

/* ////////////////////////////////////////////////////////////////////////// */

#if defined(__STLSOFT_COMPILER_IS_DMC) && \
    !defined(_STLPORT_VERSION)
template<   ss_typename_param_k S
        ,   ss_typename_param_k D
        ,   ss_typename_param_k B
        ,   ss_typename_param_k V
        ,   ss_typename_param_k T
        ,   ss_typename_param_k P
        >
inline forward_iterator_tag iterator_category(string_tokeniser<S, D, B, V, T, P>::const_iterator const &)
{
    return forward_iterator_tag();
}

template<   ss_typename_param_k S
        ,   ss_typename_param_k D
        ,   ss_typename_param_k B
        ,   ss_typename_param_k V
        ,   ss_typename_param_k T
        ,   ss_typename_param_k P
        >
inline ptrdiff_t* distance_type(string_tokeniser<S, D, B, V, T, P>::const_iterator const &)
{
    return static_cast<ptrdiff_t*>(0);
}
#endif /* __STLSOFT_COMPILER_IS_DMC */

/* ////////////////////////////////////////////////////////////////////////// */

/// @} // end of group stlsoft_string_library

/* ////////////////////////////////////////////////////////////////////////// */

#ifndef _STLSOFT_NO_NAMESPACE
} // namespace stlsoft
#endif /* _STLSOFT_NO_NAMESPACE */

/* ////////////////////////////////////////////////////////////////////////// */

#endif /* !STLSOFT_INCL_H_STLSOFT_STRING_TOKENISER */

/* ////////////////////////////////////////////////////////////////////////// */
