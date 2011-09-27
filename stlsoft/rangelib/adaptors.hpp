/* /////////////////////////////////////////////////////////////////////////////
 * File:        rangelib/adaptors.hpp
 *
 * Purpose:     Adaptors.
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


#error This file is not ready for release.

/// \file rangelib/adaptors.h

#ifndef STLSOFT_INCL_RANGELIB_HPP_ADAPTORS
#define STLSOFT_INCL_RANGELIB_HPP_ADAPTORS

#ifndef __STLSOFT_DOCUMENTATION_SKIP_SECTION
# define STLSOFT_VER_RANGELIB_HPP_ADAPTORS_MAJOR      1
# define STLSOFT_VER_RANGELIB_HPP_ADAPTORS_MINOR      0
# define STLSOFT_VER_RANGELIB_HPP_ADAPTORS_REVISION   4
# define STLSOFT_VER_RANGELIB_HPP_ADAPTORS_EDIT       6
#endif /* !__STLSOFT_DOCUMENTATION_SKIP_SECTION */

/* /////////////////////////////////////////////////////////////////////////////
 * Includes
 */

#ifndef STLSOFT_INCL_H_STLSOFT
# include <stlsoft.h>                   // Include the STLSoft root header
#endif /* !STLSOFT_INCL_H_STLSOFT */
#include <iterator>

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

/// Adapts a range for back-insertion
template <class R>
class r_back_insert_range
{
public:
    /// The range type
    typedef R                                   range_type;
    /// The iterator type
    typedef ss_typename_type_k R::iterator      iterator;
    /// The value type
    typedef ss_typename_type_k R::value_type    value_type;
    /// The type of this instantiation
    typedef r_back_insert_range<R>              class_type;

public:
    /// Constructor
    r_back_insert_range(R &r);

//  operator =(

    /// Indicates whether the range is open
    operator bool() const;
    /// Returns the current value in the range
    value_type operator *();
    /// Advances the current position in the range
    class_type &operator ++();
    /// Advances the current position in the range, returning a copy of the
    /// range prior to its being advanced
    class_type operator ++(int)
    {
        class_type  ret(*this);

        operator ++();

        return ret;
    }

    /// Returns an iterator to the current position of the range
    iterator begin();
    /// Returns an iterator to the end of the range
    iterator end();

// Members
private:
};

/// Returns an instance of \c r_back_insert_range<R>
template <class R>
inline r_back_insert_range<R> r_back_inserter(R &r)
{
    return r_back_insert_range<R>(r);
}

////////////////////////////////////////////////////////////////////////////////
// Unit-testing

#ifdef STLSOFT_UNITTEST

namespace unittest
{
    ss_bool_t test_rangelib_adaptors(unittest_reporter *r)
    {
        using stlsoft::unittest::unittest_initialiser;

        ss_bool_t               bSuccess    =   true;

        unittest_initialiser    init(r, "RangeLib", "adaptors", __FILE__);

        adaptors            vi1("USER32");

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

    unittest_registrar    unittest_rangelib_adaptors(test_rangelib_adaptors);

} // namespace unittest

#endif /* STLSOFT_UNITTEST */

/* ////////////////////////////////////////////////////////////////////////// */

/// @} // end of group rangelib_adaptors

/* ////////////////////////////////////////////////////////////////////////// */

#ifndef _STLSOFT_NO_NAMESPACE
} // namespace stlsoft
#endif /* _STLSOFT_NO_NAMESPACE */

/* ////////////////////////////////////////////////////////////////////////// */

#endif /* !STLSOFT_INCL_RANGELIB_HPP_ADAPTORS */

/* ////////////////////////////////////////////////////////////////////////// */
