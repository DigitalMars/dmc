/* /////////////////////////////////////////////////////////////////////////////
 * File:        rangelib/cstring_range.hpp
 *
 * Purpose:     Range adaptor for C-strings.
 *
 * Created:     17th May 2004
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


/** \file rangelib/cstring_range.hpp Range adaptor for C-strings */

#ifndef STLSOFT_INCL_RANGELIB_HPP_CSTRING_RANGE
#define STLSOFT_INCL_RANGELIB_HPP_CSTRING_RANGE

#ifndef __STLSOFT_DOCUMENTATION_SKIP_SECTION
# define STLSOFT_VER_RANGELIB_HPP_CSTRING_RANGE_MAJOR     1
# define STLSOFT_VER_RANGELIB_HPP_CSTRING_RANGE_MINOR     2
# define STLSOFT_VER_RANGELIB_HPP_CSTRING_RANGE_REVISION  1
# define STLSOFT_VER_RANGELIB_HPP_CSTRING_RANGE_EDIT      7
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
#ifndef STLSOFT_INCL_H_STLSOFT_ITERATOR
# include <stlsoft_iterator.h> //
#endif /* !STLSOFT_INCL_H_STLSOFT_ITERATOR */
#ifndef STLSOFT_INCL_H_STLSOFT_CONSTRAINTS
# include <stlsoft_constraints.h>       // not_implicitly_comparable
#endif /* !STLSOFT_INCL_H_STLSOFT_CONSTRAINTS */

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

/// This class represents a C-style string as a range
///
/// It is categoried as a Notional Range
///
/// It could be used as follows
/// \htmlonly
/// <code>
/// <pre>
///   // Create a range based on a C-string
///   stlsoft::cstring_range  r("This is a literal string");
///
///   // Count the number of i's in the string
///   size_t  num_Is = stlsoft::r_count(r, 'i');
/// </pre>
/// </code>
/// \endhtmlonly
template <ss_typename_param_k C>
class cstring_range
    : public notional_range_tag
{
public:
    /// The value type
    typedef C                                               value_type;
    /// The current parameterisation of the type
    typedef cstring_range<C>                                class_type;
    /// The non-mutable (const) pointer type
    typedef value_type const                                *const_pointer;
    /// The non-mutable (const) reference type
    typedef value_type const                                &const_reference;

/// \name Construction
/// @{
public:
    /// Constructor
    ///
    /// \param s The C-string for which this instance will act as a range
    cstring_range(value_type const *s)
        : m_s(s)
    {
        stlsoft_message_assert("NULL string passed to cstring_range constructor", NULL != s);
    }
    /// Destructor
    ~cstring_range()
    {
        // This is a constraint to ensure that this template is not used
        // for any non-character types.
        stlsoft_static_assert(0 != is_integral_type<value_type>::value);
        stlsoft_static_assert(0 == is_numeric_type<value_type>::value);
        stlsoft_static_assert(0 == is_bool_type<value_type>::value);
    }
/// @}

/// Range methods
/// @{
private:
    STLSOFT_DEFINE_OPERATOR_BOOL_TYPES_T(class_type, boolean_generator_type, boolean_type);
public:
    /// Indicates whether the range is open
    ss_bool_t is_open() const
    {
        stlsoft_assert(NULL != m_s);

        return '\0' != *m_s;
    }
    /// Returns the current value in the range
    const_reference current() const
    {
        stlsoft_message_assert("Attempting to access the value of a closed range", is_open());

        return *m_s;
    }
    /// Advances the current position in the range
    class_type &advance()
    {
        stlsoft_message_assert("Attempting to advance a closed range", is_open());

        ++m_s;

        return *this;
    }

    /// Indicates whether the range is open
    operator boolean_type() const
    {
        return boolean_generator_type::translate(is_open());
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

// Members
private:
    value_type const    *m_s;
};

////////////////////////////////////////////////////////////////////////////////
// Unit-testing

#ifdef STLSOFT_UNITTEST

namespace unittest
{
    namespace
    {
    } // anonymous namespace

    ss_bool_t test_stlsoft_rangelib_cstring_range(unittest_reporter *r)
    {
        using stlsoft::unittest::unittest_initialiser;

        ss_bool_t               bSuccess    =   true;

        unittest_initialiser    init(r, "RangeLib", "cstring_range", __FILE__);

        typedef cstring_range<char> cstring_range_t;

        cstring_range_t r1("Hello, Natty!");
        size_t          len;

        for(len = 0; r1; ++r1, ++len)
        {}

        if(13 != len)
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

        if(cstring_range_t(""))
        {
            r->report("closed range presents as open", __LINE__);
            bSuccess = false;
        }

        return bSuccess;
    }

    unittest_registrar    unittest_stlsoft_rangelib_cstring_range(test_stlsoft_rangelib_cstring_range);

} // namespace unittest

#endif /* STLSOFT_UNITTEST */

/* ////////////////////////////////////////////////////////////////////////// */

/// @} // end of group rangelib_ranges

/* ////////////////////////////////////////////////////////////////////////// */

#ifndef _STLSOFT_NO_NAMESPACE
} // namespace stlsoft
#endif /* _STLSOFT_NO_NAMESPACE */

/* ////////////////////////////////////////////////////////////////////////// */

#endif /* !STLSOFT_INCL_RANGELIB_HPP_CSTRING_RANGE */

/* ////////////////////////////////////////////////////////////////////////// */
