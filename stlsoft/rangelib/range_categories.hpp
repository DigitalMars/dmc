/* /////////////////////////////////////////////////////////////////////////////
 * File:        rangelib/range_categories.h
 *
 * Purpose:     Range categories.
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


/** \file rangelib/range_categories.hpp Range categories */

#ifndef STLSOFT_INCL_RANGELIB_HPP_RANGE_CATEGORIES
#define STLSOFT_INCL_RANGELIB_HPP_RANGE_CATEGORIES

#ifndef __STLSOFT_DOCUMENTATION_SKIP_SECTION
# define STLSOFT_VER_RANGELIB_HPP_RANGE_CATEGORIES_MAJOR      1
# define STLSOFT_VER_RANGELIB_HPP_RANGE_CATEGORIES_MINOR      4
# define STLSOFT_VER_RANGELIB_HPP_RANGE_CATEGORIES_REVISION   1
# define STLSOFT_VER_RANGELIB_HPP_RANGE_CATEGORIES_EDIT       7
#endif /* !__STLSOFT_DOCUMENTATION_SKIP_SECTION */

/* /////////////////////////////////////////////////////////////////////////////
 * Includes
 */

#ifndef STLSOFT_INCL_H_STLSOFT
# include <stlsoft.h>                           // Include the STLSoft root header
#endif /* !STLSOFT_INCL_H_STLSOFT */

/* /////////////////////////////////////////////////////////////////////////////
 * Namespace
 */

#ifndef _STLSOFT_NO_NAMESPACE
namespace stlsoft
{
#endif /* _STLSOFT_NO_NAMESPACE */

/* ////////////////////////////////////////////////////////////////////////// */

/// \defgroup rangelib_categories Range Categories
/// \ingroup RangeLib
/// \brief Range Categories
/// @{

/* /////////////////////////////////////////////////////////////////////////////
 * Classes
 */

/// Denotes a deriving class as being a Notional Range
struct notional_range_tag
{};

/// Denotes a deriving class as being an Iterable Range
struct iterable_range_tag
    : public notional_range_tag
{};

/// Denotes a deriving class as being an Indirect Range
struct basic_indirect_range_tag
{};

/// Denotes a deriving class as being a Basic Indirect Range
struct indirect_range_tag
    : public basic_indirect_range_tag
{};

////////////////////////////////////////////////////////////////////////////////
// Unit-testing

#ifdef STLSOFT_UNITTEST

namespace unittest
{
    ss_bool_t test_rangelib_range_categories(unittest_reporter *r)
    {
        using stlsoft::unittest::unittest_initialiser;

        ss_bool_t               bSuccess    =   true;

        unittest_initialiser    init(r, "RangeLib", "range_categories", __FILE__);

        // Just check the relationship between the indirect ranges
        const indirect_range_tag        irt     =   indirect_range_tag();
        basic_indirect_range_tag const  &birt   =   irt;

        STLSOFT_SUPPRESS_UNUSED(birt);

        return bSuccess;
    }

    unittest_registrar    unittest_rangelib_range_categories(test_rangelib_range_categories);

} // namespace unittest

#endif /* STLSOFT_UNITTEST */

/* ////////////////////////////////////////////////////////////////////////// */

/// @} // end of group rangelib_categories

/* ////////////////////////////////////////////////////////////////////////// */

#ifndef _STLSOFT_NO_NAMESPACE
} // namespace stlsoft
#endif /* _STLSOFT_NO_NAMESPACE */

/* ////////////////////////////////////////////////////////////////////////// */

#endif /* !STLSOFT_INCL_RANGELIB_HPP_RANGE_CATEGORIES */

/* ////////////////////////////////////////////////////////////////////////// */
