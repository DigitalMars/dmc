/* /////////////////////////////////////////////////////////////////////////////
 * File:        rangelib/random_range.hpp
 *
 * Purpose:     Random number range class.
 *
 * Created:     31st May 2004
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


/** \file rangelib/random_range.hpp Random number range class */

#ifndef STLSOFT_INCL_RANGELIB_HPP_RANDOM_RANGE
#define STLSOFT_INCL_RANGELIB_HPP_RANDOM_RANGE

#ifndef __STLSOFT_DOCUMENTATION_SKIP_SECTION
# define STLSOFT_VER_RANGELIB_HPP_RANDOM_RANGE_MAJOR      1
# define STLSOFT_VER_RANGELIB_HPP_RANDOM_RANGE_MINOR      1
# define STLSOFT_VER_RANGELIB_HPP_RANDOM_RANGE_REVISION   1
# define STLSOFT_VER_RANGELIB_HPP_RANDOM_RANGE_EDIT       5
#endif /* !__STLSOFT_DOCUMENTATION_SKIP_SECTION */

/* /////////////////////////////////////////////////////////////////////////////
 * Includes
 */

#ifndef STLSOFT_INCL_H_STLSOFT
# include <stlsoft.h>                   // Include the STLSoft root header
#endif /* !STLSOFT_INCL_H_STLSOFT */
#ifndef STLSOFT_INCL_RANGELIB_HPP_RANGE_CATEGORIES
# include <rangelib/range_categories.hpp>
#endif /* !STLSOFT_INCL_RANGELIB_HPP_RANGE_CATEGORIES */
#ifndef STLSOFT_INCL_H_STLSOFT_OPERATOR_BOOL
# include <stlsoft_operator_bool.h>     // operator_bool_generator
#endif /* !STLSOFT_INCL_H_STLSOFT_OPERATOR_BOOL */
#include <stdlib.h>                     // rand(), srand()

/* /////////////////////////////////////////////////////////////////////////////
 * Namespace
 */

#ifndef _STLSOFT_NO_NAMESPACE
namespace stlsoft
{
#endif /* _STLSOFT_NO_NAMESPACE */

/* ////////////////////////////////////////////////////////////////////////// */

/// \defgroup rangelib_ranges Range Classes
/// \ingroup RangeLib
/// \brief Range Classes
/// @{

/* /////////////////////////////////////////////////////////////////////////////
 * Classes
 */

/// This range class represents a range of random numbers.
///
/// It is categoried as a Notional Range
///
/// It could be used as follows
/// \htmlonly
/// <code>
/// <pre>
///   // Create a range of 10 values, whose value are between -100 and +100
///   stlsoft::random_range   r(10, -100, +100);
///
///   // Dump them to stdout
///   stlsoft::r_copy(r, std::ostream_iterator&lt;int>(" "));
/// </pre>
/// </code>
/// \endhtmlonly
class random_range
    : public notional_range_tag
{
/// \name Types
/// @{
public:
    typedef int                 value_type;
    typedef notional_range_tag  range_tag_type;
    typedef random_range        class_type;
/// @}

/// \name Construction
/// @{
public:
    /// Constructs a random range
    ///
    /// \param numValues The number of values in the range
    /// \param minValue The lower limit of the random number range 
    /// \param maxValue The upper limit of the random number range
    random_range(ss_size_t numValues, value_type minValue = 0, value_type maxValue = RAND_MAX)
        : m_numValues(numValues)
        , m_minValue(minValue)
        , m_maxValue(maxValue)
        , m_position(0)
        , m_value(next_value_(minValue, maxValue))
    {
        stlsoft_assert(minValue <= maxValue);
        stlsoft_assert((maxValue - minValue) <= RAND_MAX);
    }
/// @}

/// \name Notional Range methods
/// @{
private:
    STLSOFT_DEFINE_OPERATOR_BOOL_TYPES(class_type, operator_bool_generator_type, operator_bool_type);
public:
    /// Indicates whether the range is open
    ss_bool_t is_open() const
    {
        return m_position != m_numValues;
    }
    /// Returns the current value in the range
    value_type current() const
    {
        stlsoft_message_assert("Attempting to access the value of a closed range", is_open());

        return m_value;
    }
    /// Advances the current position in the range
    class_type &advance()
    {
        stlsoft_message_assert("Attempting to advance a closed range", is_open());

        ++m_position;
        m_value = next_value_(m_minValue, m_maxValue);

        return *this;
    }

    /// Indicates whether the range is open
    operator operator_bool_type() const
    {
        return operator_bool_generator_type::translate(is_open());
    }
    /// Returns the current value in the range
    value_type operator *() const
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

/// \name Attributes
/// @{
public:
    /// The lower limit of the random number range 
    value_type minimum() const
    {
        return m_minValue;
    }
    /// The upper limit of the random number range
    value_type maximum() const
    {
        return m_maxValue;
    }
/// @}

/// \name Comparison
/// @{
public:
    /// Evaluates whether two ranges are equal
    bool operator ==(class_type const &/* rhs */) const
    {
        return false;
    }
    /// Evaluates whether two ranges are unequal
    bool operator !=(class_type const &rhs) const
    {
        return ! operator ==(rhs);
    }
/// @}

/// \name Implementation
private:
    static value_type next_value_(value_type minValue, value_type maxValue)
    {
        return (maxValue - minValue) ? minValue + (rand() % (maxValue - minValue)) : 0;
    }
/// @}

// Members
private:
    int const   m_numValues;
    int const   m_minValue;
    int const   m_maxValue;
    int         m_position;
    int         m_value;

// Not to be implemented
private:
    class_type &operator =(class_type const&);
};

////////////////////////////////////////////////////////////////////////////////
// Unit-testing

#ifdef STLSOFT_UNITTEST

namespace unittest
{
    namespace
    {
    } // anonymous namespace

    ss_bool_t test_stlsoft_rangelib_random_range(unittest_reporter *r)
    {
        using stlsoft::unittest::unittest_initialiser;

        ss_bool_t               bSuccess    =   true;

        unittest_initialiser    init(r, "RangeLib", "random_range", __FILE__);

        random_range    r1(10, -10, 10);
        int             sum;

        stlsoft_assert(-10 == r1.minimum());
        stlsoft_assert(+10 == r1.maximum());

        for(sum = 0; r1; ++r1)
        {
            stlsoft_assert(*r1 >= r1.minimum());
            stlsoft_assert(*r1 <= r1.maximum());

            sum += *r1;
        }

        if( sum < 10 * r1.minimum() ||
            sum >= 10 * r1.maximum())
        {
            r->report("manual enumeration failed", __LINE__);
            bSuccess = false;
        }

        if(r1.is_open())
        {
            r->report("closed range presents as open (is_open() method)", __LINE__);
            bSuccess = false;
        }

        if(r1)
        {
            r->report("closed range presents as open (operator \"bool\"())", __LINE__);
            bSuccess = false;
        }

        if(random_range(0, 0, 0))
        {
            r->report("closed range presents as open", __LINE__);
            bSuccess = false;
        }

        return bSuccess;
    }

    unittest_registrar    unittest_stlsoft_rangelib_random_range(test_stlsoft_rangelib_random_range);

} // namespace unittest

#endif /* STLSOFT_UNITTEST */

/* ////////////////////////////////////////////////////////////////////////// */

/// @} // end of group rangelib_classes

/* ////////////////////////////////////////////////////////////////////////// */

#ifndef _STLSOFT_NO_NAMESPACE
} // namespace stlsoft
#endif /* _STLSOFT_NO_NAMESPACE */

/* ////////////////////////////////////////////////////////////////////////// */

#endif /* !STLSOFT_INCL_RANGELIB_HPP_RANDOM_RANGE */

/* ////////////////////////////////////////////////////////////////////////// */
