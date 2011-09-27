/* /////////////////////////////////////////////////////////////////////////////
 * File:        rangelib/sequence_range.hpp
 *
 * Purpose:     Sequence container range adaptor.
 *
 * Created:     4th November 2003
 * Updated:     12th September 2004
 *
 * Home:        http://stlsoft.org/
 *
 * Copyright (c) 2003-2004, Matthew Wilson and Synesis Software
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


/** \file rangelib/sequence_range.hpp Sequence container range adaptor */

#ifndef STLSOFT_INCL_RANGELIB_HPP_SEQUENCE_RANGE
#define STLSOFT_INCL_RANGELIB_HPP_SEQUENCE_RANGE

#ifndef __STLSOFT_DOCUMENTATION_SKIP_SECTION
# define STLSOFT_VER_RANGELIB_HPP_SEQUENCE_RANGE_MAJOR    1
# define STLSOFT_VER_RANGELIB_HPP_SEQUENCE_RANGE_MINOR    3
# define STLSOFT_VER_RANGELIB_HPP_SEQUENCE_RANGE_REVISION 2
# define STLSOFT_VER_RANGELIB_HPP_SEQUENCE_RANGE_EDIT     15
#endif /* !__STLSOFT_DOCUMENTATION_SKIP_SECTION */

/* /////////////////////////////////////////////////////////////////////////////
 * Includes
 */

#ifndef STLSOFT_INCL_H_STLSOFT
# include <stlsoft.h>                           // Include the STLSoft root header
#endif /* !STLSOFT_INCL_H_STLSOFT */
#ifndef STLSOFT_INCL_RANGELIB_HPP_RANGE_CATEGORIES
# include <rangelib/range_categories.hpp>
#endif /* !STLSOFT_INCL_RANGELIB_HPP_RANGE_CATEGORIES */
#ifndef STLSOFT_INCL_H_STLSOFT_OPERATOR_BOOL
# include <stlsoft_operator_bool.h> //
#endif /* !STLSOFT_INCL_H_STLSOFT_OPERATOR_BOOL */
#ifndef STLSOFT_INCL_H_STLSOFT_META
# include <stlsoft_meta.h>  //
#endif /* !STLSOFT_INCL_H_STLSOFT_META */

#ifdef STLSOFT_UNITTEST
# include <algorithm>
# include <deque>
# include <list>
# include <numeric>
# include <vector>
#endif /* STLSOFT_UNITTEST */

/* /////////////////////////////////////////////////////////////////////////////
 * Namespace
 */

#ifndef _STLSOFT_NO_NAMESPACE
namespace stlsoft
{
#endif /* _STLSOFT_NO_NAMESPACE */

/* ////////////////////////////////////////////////////////////////////////// */

/// \defgroup rangelib_adaptors Range Adaptors
/// \ingroup RangeLib
/// \brief Range adaptor templates
/// @{

/* /////////////////////////////////////////////////////////////////////////////
 * Classes
 */

#ifdef __STLSOFT_CF_TEMPLATE_PARTIAL_SPECIALISATION_SUPPORT


/// Traits class for determining the attributes of range-adapted sequence container types
template<   ss_typename_param_k S
        ,   bool                B_CONST
        >
struct sequence_range_traits
{
public:
    /// The sequence type
    typedef S                                                       sequence_type;
    /// The sequence reference type
    typedef S                                                       &sequence_reference_type;
    /// The value type
    typedef ss_typename_type_k sequence_type::value_type            value_type;
    /// The mutating (non-const) iterator type
    typedef ss_typename_type_k select_first_type<   ss_typename_type_k sequence_type::const_iterator
                                                ,   ss_typename_type_k sequence_type::iterator
                                                ,   B_CONST
                                                >::type             iterator;
    /// The non-mutating (const) iterator type
    typedef ss_typename_type_k sequence_type::const_iterator        const_iterator;
    /// The mutating (non-const) reference type
    typedef ss_typename_type_k select_first_type<   ss_typename_type_k sequence_type::const_reference
                                                ,   ss_typename_type_k sequence_type::reference
                                                ,   B_CONST
                                                >::type             reference;
    /// The non-mutating (const) reference type
    typedef ss_typename_type_k sequence_type::const_reference       const_reference;
// TODO: Stick in the member-finder stuff here, so can assume ptrdiff_t if none found
    /// The difference type
    typedef ss_typename_type_k sequence_type::difference_type       difference_type;
    /// The size type
    typedef ss_typename_type_k sequence_type::size_type             size_type;
};

template<   ss_typename_param_k S
        >
struct sequence_range_traits<S, true>
{
public:
    typedef S                                                       sequence_type;
    typedef S                                                       &sequence_reference_type;
    typedef ss_typename_type_k sequence_type::value_type            value_type;
    typedef ss_typename_type_k sequence_type::const_iterator        iterator;
    typedef ss_typename_type_k sequence_type::const_iterator        const_iterator;
    typedef ss_typename_type_k sequence_type::const_reference       reference;
    typedef ss_typename_type_k sequence_type::const_reference       const_reference;
// TODO: Stick in the member-finder stuff here, so can assume ptrdiff_t if none found
    typedef ss_typename_type_k sequence_type::difference_type       difference_type;
    typedef ss_typename_type_k sequence_type::size_type             size_type;
};
#else /* ? __STLSOFT_CF_TEMPLATE_PARTIAL_SPECIALISATION_SUPPORT */
template<   ss_typename_param_k S
        >
struct sequence_range_traits
{
public:
    typedef S                                                       sequence_type;
    typedef S                                                       &sequence_reference_type;
    typedef ss_typename_type_k sequence_type::value_type            value_type;
    typedef ss_typename_type_k sequence_type::iterator              iterator;
    typedef ss_typename_type_k sequence_type::const_iterator        const_iterator;
    typedef ss_typename_type_k sequence_type::reference             reference;
    typedef ss_typename_type_k sequence_type::const_reference       const_reference;
// TODO: Stick in the member-finder stuff here, so can assume ptrdiff_t if none found
    typedef ss_typename_type_k sequence_type::difference_type       difference_type;
    typedef ss_typename_type_k sequence_type::size_type             size_type;
};

template<   ss_typename_param_k S
        >
struct const_sequence_range_traits
{
public:
    typedef S                                                       sequence_type;
    typedef S const                                                 &sequence_reference_type;
    typedef ss_typename_type_k sequence_type::value_type            value_type;
    typedef ss_typename_type_k sequence_type::const_iterator        iterator;
    typedef ss_typename_type_k sequence_type::const_iterator        const_iterator;
    typedef ss_typename_type_k sequence_type::const_reference       reference;
    typedef ss_typename_type_k sequence_type::const_reference       const_reference;
    typedef ss_typename_type_k sequence_type::difference_type       difference_type;
    typedef ss_typename_type_k sequence_type::size_type             size_type;
};
#endif /* __STLSOFT_CF_TEMPLATE_PARTIAL_SPECIALISATION_SUPPORT */

/// \brief This class adapts an STL sequence instance into a Range
///
/// \param S The sequence class
/// \param T The sequence range traits, used to deduce the Range's iterator, const_iterator, reference, const_reference and value_type
///
/// It is categoried as an Iterable Range
///
/// It could be used as follows
/// \htmlonly
/// <code>
/// <pre>
/// void dump_elements(std::vector&lt;int&gt; const &numbers)
/// {
///   for(sequence_range&lt;std::vector&lt;int&gt; &gt; r(numbers.begin(), numbers.end()); r; ++r)
///   {
///     std::cout &lt;&lt; &r; // Dump the current value to stdout
///   }
/// }
/// </pre>
/// </code>
/// \endhtmlonly
template<   ss_typename_param_k S
#ifdef __STLSOFT_CF_TEMPLATE_PARTIAL_SPECIALISATION_SUPPORT
        ,   ss_typename_param_k T = sequence_range_traits<S, is_const<S>::value>    // Determines whether the sequence is const
#else /* ? __STLSOFT_CF_TEMPLATE_PARTIAL_SPECIALISATION_SUPPORT */
        ,   ss_typename_param_k T = const_sequence_range_traits<S>                  // Determines whether the sequence is const
#endif /* __STLSOFT_CF_TEMPLATE_PARTIAL_SPECIALISATION_SUPPORT */
        >
class sequence_range
    : public iterable_range_tag
{
public:
    /// The sequence type
    typedef S                                                       sequence_type;
    /// The traits type
    typedef T                                                       traits_type;
    /// The range category tag type
    typedef iterable_range_tag                                      range_tag_type;
    /// The current instantiation of the type
    typedef sequence_range<S, T>                                    class_type;
    /// The sequence reference type
    typedef ss_typename_type_k traits_type::sequence_reference_type sequence_reference_type;
    /// The value type
    typedef ss_typename_type_k traits_type::value_type              value_type;
    /// The mutating (non-const) iterator type
    typedef ss_typename_type_k traits_type::iterator                iterator;
    /// The non-mutating (const) iterator type
    typedef ss_typename_type_k traits_type::const_iterator          const_iterator;
    /// The mutating (non-const) reference type
    typedef ss_typename_type_k traits_type::reference               reference;
    /// The non-mutating (const) reference type
    typedef ss_typename_type_k traits_type::const_reference         const_reference;
    /// The difference type
    typedef ss_typename_type_k traits_type::difference_type         difference_type;
    /// The size type
    typedef ss_typename_type_k traits_type::size_type               size_type;

public:
    sequence_range(sequence_reference_type seq) // The constness of this will require some thinking about. Maybe need sequence_range and const_sequence_range??
        : m_position(seq.begin())
        , m_last(seq.end())
    {}
    template <ss_typename_param_k I>
    sequence_range(I first, I last)
        : m_position(first)
        , m_last(last)
    {}
#ifdef __STLSOFT_CF_STATIC_ARRAY_SIZE_DETERMINATION_SUPPORT
//  template <ss_typename_param_k T2, ss_size_t N>
//    sequence_range(T2 (&ar)[N])
    template <ss_size_t N>
    sequence_range(S (&ar)[N])
        : m_position(&ar[0])
        , m_last(&ar[N])
    {}
#endif /* __STLSOFT_CF_STATIC_ARRAY_SIZE_DETERMINATION_SUPPORT */

/// \name Notional Range methods
/// @{
private:
    STLSOFT_DEFINE_OPERATOR_BOOL_TYPES_T(class_type, boolean_generator_type, boolean_type);
public:
    /// Indicates whether the range is open
    ss_bool_t is_open() const
    {
        return m_position != m_last;
    }
    /// Returns the current value in the range
    reference current()
    {
        stlsoft_assert(is_open());

//      m_value = *m_position;

        return *m_position;
    }
    /// Returns the current value in the range
    const_reference current() const
    {
        stlsoft_assert(is_open());

        return *m_position;
    }
    /// Advances the current position in the range
    class_type &advance()
    {
        stlsoft_message_assert("Attempting to increment the range past its end point", is_open());

        ++m_position;

        return *this;
    }

    /// Indicates whether the range is open
    operator boolean_type() const
    {
        return boolean_generator_type::translate(is_open());
    }
    /// Returns the current value in the range
    reference operator *()
    {
        return current();
    }
    /// Returns the current value in the range
    const_reference operator *() const
    {
        return current();
    }
    /// Advances the current position in the range
    class_type &operator ++()
    {
        return advance();
    }
    /// Advances the current position in the range, returning a copy of the
    /// range prior to its being advanced
    class_type operator ++(int)
    {
        class_type  ret(*this);

        operator ++();

        return ret;
    }
/// @}

/// \name Iterable Range methods
/// @{
public:
    /// Returns an iterator to the current position of the range
    iterator begin()
    {
        return m_position;
    }
    /// Returns an iterator to the end of the range
    iterator end()
    {
        return m_last;
    }

    /// Returns an iterator to the current position of the range
    const_iterator begin() const
    {
        return m_position;
    }
    /// Returns an iterator to the end of the range
    const_iterator end() const
    {
        return m_last;
    }
/// @}

// Members
private:
    iterator    m_position;
    iterator    m_last;
    value_type  m_value;
};

////////////////////////////////////////////////////////////////////////////////
// Unit-testing

#ifdef STLSOFT_UNITTEST

namespace unittest
{
    namespace sequence_range_util
    {
        template <typename T>
        ss_bool_t test_container()
        {
            T           cont;
            int         total0  =   0;
            int         total1  =   0;

            for(int i = 0; i < 100; i += 5)
            {
#if defined(__STLSOFT_COMPILER_IS_DMC)
                typedef ss_typename_type_k T::value_type    value_t;

                cont.push_back(value_t(i));
#else /* ? compiler */
                cont.push_back(ss_typename_type_k T::value_type(i));
#endif /* compiler */
            }

            for(sequence_range<T> range(cont.begin(), cont.end()); range; ++range)
            {
                total0 += *range;
            }

#if defined(__STLSOFT_COMPILER_IS_DMC)
            typedef ss_typename_type_k T::value_type    value_t;

            total1 = std::accumulate(cont.begin(), cont.end(), value_t(0));
#else /* ? compiler */
            total1 = std::accumulate(cont.begin(), cont.end(), ss_typename_type_k T::value_type(0));
#endif /* compiler */

            return total0 == total1;
        }
    } // namespace sequence_range_util

    ss_bool_t test_rangelib_sequence_range(unittest_reporter *r)
    {
        using stlsoft::unittest::unittest_initialiser;

        ss_bool_t               bSuccess    =   true;

        unittest_initialiser    init(r, "RangeLib", "sequence_range", __FILE__);

        if(!sequence_range_util::test_container<std::list<int> >())
        {
            r->report("summation over list<int> failed", __LINE__);
            bSuccess = false;
        }

        if(!sequence_range_util::test_container<std::vector<int> >())
        {
            r->report("summation over vector<int> failed", __LINE__);
            bSuccess = false;
        }

        if(!sequence_range_util::test_container<std::deque<short> >())
        {
            r->report("summation over deque<short> failed", __LINE__);
            bSuccess = false;
        }

        return bSuccess;
    }

    unittest_registrar    unittest_rangelib_sequence_range(test_rangelib_sequence_range);

} // namespace unittest

#endif /* STLSOFT_UNITTEST */

/* ////////////////////////////////////////////////////////////////////////// */

/// @} // end of group rangelib_adaptors

/* ////////////////////////////////////////////////////////////////////////// */

#ifndef _STLSOFT_NO_NAMESPACE
} // namespace stlsoft
#endif /* _STLSOFT_NO_NAMESPACE */

/* ////////////////////////////////////////////////////////////////////////// */

#endif /* !STLSOFT_INCL_RANGELIB_HPP_SEQUENCE_RANGE */

/* ////////////////////////////////////////////////////////////////////////// */
