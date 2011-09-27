/* /////////////////////////////////////////////////////////////////////////////
 * File:        rangelib/filtered_range.hpp
 *
 * Purpose:     Range filter adaptor.
 *
 * Created:     9th July 2004
 * Updated:     12th September 2004
 *
 * Home:        http://stlsoft.org/
 *
 * Copyright (c) 2004, Matthew Wilson and Synesis Software
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


/** \file rangelib/filtered_range.hpp Range filter adaptor */

#ifndef STLSOFT_INCL_RANGELIB_HPP_FILTERED_RANGE
#define STLSOFT_INCL_RANGELIB_HPP_FILTERED_RANGE

#ifndef __STLSOFT_DOCUMENTATION_SKIP_SECTION
# define STLSOFT_VER_RANGELIB_HPP_FILTERED_RANGE_MAJOR    1
# define STLSOFT_VER_RANGELIB_HPP_FILTERED_RANGE_MINOR    0
# define STLSOFT_VER_RANGELIB_HPP_FILTERED_RANGE_REVISION 2
# define STLSOFT_VER_RANGELIB_HPP_FILTERED_RANGE_EDIT     3
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
# include <stlsoft_operator_bool.h>             // stlsoft::operator_bool_generator
#endif /* !STLSOFT_INCL_H_STLSOFT_OPERATOR_BOOL */
#ifndef STLSOFT_INCL_H_STLSOFT_META
# include <stlsoft_meta.h>                      // TMP stuff
#endif /* !STLSOFT_INCL_H_STLSOFT_META */
#ifndef STLSOFT_INCL_H_STLSOFT_FILTER_ITERATOR
# include <stlsoft_filter_iterator.h>           // stlsoft::filtered_iterator
#endif /* !STLSOFT_INCL_H_STLSOFT_FILTER_ITERATOR */

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

/// \brief This class adapts an STL sequence instance into a Range
///
/// \param R The range class
/// \param P The filter predicate
/// \param RT The range category tag type
template<   ss_typename_param_k R
        ,   ss_typename_param_k P
        ,   ss_typename_param_k RT = ss_typename_type_k R::range_tag_type
        >
class filtered_range
    : public RT
{
public:
    typedef R                                                                   filtered_range_type;
    typedef P                                                                   filter_predicate_type;
    typedef RT                                                                  range_tag_type;
    typedef filtered_range<R, P, RT>                                            class_type;
private:
    typedef ss_typename_type_k filtered_range_type::iterator                    iterator_base_type;
    typedef ss_typename_type_k filtered_range_type::const_iterator              const_iterator_base_type;
public:
    typedef filtered_iterator<iterator_base_type, filter_predicate_type>        iterator;
    typedef filtered_iterator<const_iterator_base_type, filter_predicate_type>  const_iterator;
    typedef ss_typename_type_k filtered_range_type::reference                   reference;
    typedef ss_typename_type_k filtered_range_type::const_reference             const_reference;
    typedef ss_typename_type_k filtered_range_type::value_type                  value_type;

public:
    /// Constructs from a range and a predicate
    ///
    /// \param r The range whose values will be filtered
    /// \param pr The predicate which will be used to filter the values of the range \c r
    filtered_range(filtered_range_type r, filter_predicate_type pr)
        : m_range(r)
        , m_predicate(pr)
    {
        for(; m_range; ++m_range)
        {
            if(m_predicate(*m_range))
            {
                break;
            }
        }
    }

/// \name Notional Range methods
/// @{
private:
    STLSOFT_DEFINE_OPERATOR_BOOL_TYPES_T(class_type, boolean_generator_type, boolean_type);
public:
    /// Indicates whether the range is open
    ss_bool_t is_open() const
    {
        return m_range.is_open();
    }
    /// Returns the current value in the range
    reference current()
    {
        stlsoft_assert(is_open());

        return m_range.current();
    }
    /// Returns the current value in the range
    const_reference current() const
    {
        stlsoft_assert(is_open());

        return m_range.current();
    }
    /// Advances the current position in the range
    class_type &advance()
    {
        stlsoft_message_assert("Attempting to increment the range past its end point", is_open());

        for(++m_range; m_range; ++m_range)
        {
            if(m_predicate(*m_range))
            {
                break;
            }
        }

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
        return iterator(m_range.begin(), m_range.end(), m_predicate);
    }
    /// Returns an iterator to the end of the range
    iterator end()
    {
        return iterator(m_range.end(), m_range.end(), m_predicate);
    }

    /// Returns an iterator to the current position of the range
    const_iterator begin() const
    {
        return const_iterator(m_range.begin(), m_range.end(), m_predicate);
    }
    /// Returns an iterator to the end of the range
    const_iterator end() const
    {
        return const_iterator(m_range.end(), m_range.end(), m_predicate);
    }
/// @}

// Members
private:
    filtered_range_type     m_range;
    filter_predicate_type   m_predicate;
};

template<   ss_typename_param_k R
        ,   ss_typename_param_k P
        >
inline filtered_range<R, P> filter_range(R r, P p)
{
    return filtered_range<R, P>(r, p);
}

////////////////////////////////////////////////////////////////////////////////
// Unit-testing

#ifdef STLSOFT_UNITTEST

namespace unittest
{
    ss_bool_t test_rangelib_filtered_range(unittest_reporter *r)
    {
        using stlsoft::unittest::unittest_initialiser;

        ss_bool_t               bSuccess    =   true;

        unittest_initialiser    init(r, "RangeLib", "filtered_range", __FILE__);

#if 0
        if(NULL != pi1)
        {
            ator1.construct(pi1, 1968);

            if(1968 != *pi1)
            {
                r->report("construct() failed ", __LINE__);
                bSuccess = false;
            }
        }
#endif /* 0 */

        return bSuccess;
    }

    unittest_registrar    unittest_rangelib_filtered_range(test_rangelib_filtered_range);

} // namespace unittest

#endif /* STLSOFT_UNITTEST */

/* ////////////////////////////////////////////////////////////////////////// */

/// @} // end of group rangelib_adaptors

/* ////////////////////////////////////////////////////////////////////////// */

#ifndef _STLSOFT_NO_NAMESPACE
} // namespace stlsoft
#endif /* _STLSOFT_NO_NAMESPACE */

/* ////////////////////////////////////////////////////////////////////////// */

#endif /* !STLSOFT_INCL_RANGELIB_HPP_FILTERED_RANGE */

/* ////////////////////////////////////////////////////////////////////////// */
