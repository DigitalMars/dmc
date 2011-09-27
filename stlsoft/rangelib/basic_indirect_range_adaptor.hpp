/* /////////////////////////////////////////////////////////////////////////////
 * File:        rangelib/basic_indirect_range_adaptor.hpp
 *
 * Purpose:     basic_indirect_range_adaptor.
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


/** \file rangelib/basic_indirect_range_adaptor.hpp basic_indirect_range_adaptor */

#ifndef STLSOFT_INCL_RANGELIB_HPP_BASIC_INDIRECT_RANGE_ADAPTOR
#define STLSOFT_INCL_RANGELIB_HPP_BASIC_INDIRECT_RANGE_ADAPTOR

#ifndef __STLSOFT_DOCUMENTATION_SKIP_SECTION
# define STLSOFT_VER_RANGELIB_HPP_BASIC_INDIRECT_RANGE_ADAPTOR_MAJOR      1
# define STLSOFT_VER_RANGELIB_HPP_BASIC_INDIRECT_RANGE_ADAPTOR_MINOR      0
# define STLSOFT_VER_RANGELIB_HPP_BASIC_INDIRECT_RANGE_ADAPTOR_REVISION   8
# define STLSOFT_VER_RANGELIB_HPP_BASIC_INDIRECT_RANGE_ADAPTOR_EDIT       10
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
#include <iterator>
#include <functional>

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
 * Functions
 */

#ifndef __STLSOFT_DOCUMENTATION_SKIP_SECTION
namespace
{
    template <ss_typename_param_k T>
    struct accumulate_1_function
    {
    public:
        accumulate_1_function(T &total)
            : m_total(total)
        {}

    public:
        template <ss_typename_param_k T2>
        ss_bool_t operator ()(T2 val)
        {
            m_total = m_total + val;

            return true;
        }

    private:
        T   &m_total;
    };

    template<   ss_typename_param_k T
            ,   ss_typename_param_k P
            >
    struct accumulate_2_function
    {
    public:
        accumulate_2_function(T &total, P pr)
            : m_total(total)
            , m_pr(pr)
        {}

    public:
        template <ss_typename_param_k T2>
        ss_bool_t operator ()(T2 val)
        {
            m_total = m_pr(m_total, val);

            return true;
        }

    private:
        T   &m_total;
        P   m_pr;
    };

    template <ss_typename_param_k O>
    struct copy_function
    {
    public:
        copy_function(O o)
            : m_o(o)
        {}

    public:
        template <ss_typename_param_k T2>
        ss_bool_t operator ()(T2 val)
        {
            *m_o++ = val;

            return true;
        }

    public:
        operator O()
        {
            return m_o;
        }

    private:
        O   m_o;
    };

    template <ss_typename_param_k T>
    struct count_function
    {
    public:
        count_function(size_t &n, T const &val)
            : m_n(n)
            , m_val(val)
        {}

    public:
        template <ss_typename_param_k T2>
        ss_bool_t operator ()(T2 val) const
        {
            if(m_val == val)
            {
                ++m_n;
            }

            return true;
        }
    private:
        size_t  &m_n;
        T const &m_val;
    };

    template <ss_typename_param_k P>
    struct count_if_function
    {
    public:
        count_if_function(size_t &n, P pr)
            : m_n(n)
            , m_pr(pr)
        {}

    public:
        template <ss_typename_param_k T2>
        ss_bool_t operator ()(T2 val) const
        {
            if(m_pr(val))
            {
                ++m_n;
            }

            return true;
        }
    private:
        size_t  &m_n;
        P       m_pr;
    };

    struct distance_function
    {
    public:
        distance_function(ptrdiff_t &count)
            : m_count(count)
        {}
    public:
        template <ss_typename_param_k T>
        ss_bool_t operator ()(T /* v */) const
        {
            return (++m_count, true);
        }
    private:
        ptrdiff_t   &m_count;

    private:
        distance_function &operator =(distance_function const &);
    };

    template <ss_typename_param_k T>
    struct exists_function
    {
    public:
        exists_function(ss_bool_t &b, T const &val)
            : m_b(b)
            , m_val(val)
        {
            stlsoft_assert(!b);
        }

    public:
        template <ss_typename_param_k T2>
        ss_bool_t operator ()(T2 val) const
        {
            if(m_val == val)
            {
                m_b = true;

                return false; // break out of loop
            }

            return true;
        }
    private:
        ss_bool_t   &m_b;
        T const     &m_val;
    };

    template <ss_typename_param_k P>
    struct exists_if_1_function
    {
    public:
        exists_if_1_function(ss_bool_t &b, P pr)
            : m_b(b)
            , m_pr(pr)
        {
            stlsoft_assert(!b);
        }

    public:
        template <ss_typename_param_k T2>
        ss_bool_t operator ()(T2 val) const
        {
            if(pf(val))
            {
                m_b = true;

                return false; // break out of loop
            }

            return true;
        }
    private:
        ss_bool_t   &m_b;
        P           m_pr;
    };

    template<   ss_typename_param_k P
            ,   ss_typename_param_k V
            >
    struct exists_if_2_function
    {
    public:
        exists_if_2_function(ss_bool_t &b, P pr, V &result)
            : m_b(b)
            , m_pr(pr)
            , m_result(result)
        {
            stlsoft_assert(!b);
        }

    public:
        template <ss_typename_param_k T2>
        ss_bool_t operator ()(T2 val) const
        {
            if(pf(val))
            {
                m_b         =   true;
                m_result    =   val;

                return false; // break out of loop
            }

            return true;
        }
    private:
        ss_bool_t   &m_b;
        P           m_pr;
        V           &m_result;
    };

    template <ss_typename_param_k F>
    struct for_each_function
    {
    public:
        for_each_function(F f)
            : m_f(f)
        {}
    public:
        template <ss_typename_param_k T>
        ss_bool_t operator ()(T v) const
        {
            return (m_f(v), true);
        }
    private:
        F   m_f;
    };

    template<   ss_typename_param_k V
            ,   ss_typename_param_k P
            >
    struct minmax_element_2_function
    {
    public:
        minmax_element_2_function(ss_bool_t &bRetrieved, P pr, V &result)
            : m_bRetrieved(bRetrieved)
            , m_pr(pr)
            , m_result(result)
        {}
    public:
        template <ss_typename_param_k T>
        ss_bool_t operator ()(T val) const
        {
            if(!m_bRetrieved)
            {
                m_result = val;

                m_bRetrieved = true;
            }
            else
            {
                if(m_pr(m_result, val))
                {
                    m_result = val;
                }
            }

            return true;
        }
    private:
        ss_bool_t   &m_bRetrieved;
        P           m_pr;
        V           &m_result;
    };

} // anonymous namespace

#endif /* !__STLSOFT_DOCUMENTATION_SKIP_SECTION */

/* /////////////////////////////////////////////////////////////////////////////
 * Classes
 */

/// Runtime adaptor that adapts a Basic Indirect range to an Indirect range.
template <ss_typename_param_k R>
class indirect_range_adaptor
    : public indirect_range_tag
{
public:
    typedef R                                   range_type;
    typedef ss_typename_type_k R::value_type    value_type;

public:
    /// Constructor
    ///
    /// \param r The Basic Indirect range instance to be adapted
    indirect_range_adaptor(range_type r)
        : m_r(r)
    {}

    /// Returns the sum of \c val and the value of each element in the range
    template <ss_typename_param_k T>
    T accumulate(T val) const
    {
        T   total   =   val;

        m_r.for_each_cancelable(accumulate_1_function<T>(total));

        return total;
    }

    /// Returns the sum of \c val and the value of pr applied to each element in the range
    template<   ss_typename_param_k T
            ,   ss_typename_param_k P
            >
    T accumulate(T val, P pr) const
    {
        T   total   =   val;

        m_r.for_each_cancelable(accumulate_2_function<T, P>(total, pr));

        return total;
    }

    /// Copies each element in the range to the output iterator \c o
    template <ss_typename_param_k O>
    O copy(O o) const
    {
        copy_function<O>    cf(o);

        m_r.for_each_cancelable(cf);

        return cf;
    }

    /// Returns the number of elements in the range that evaluate equal to \c val
    template <ss_typename_param_k T>
    size_t count(T const &val) const
    {
        size_t  n   =   0;

        m_r.for_each_cancelable(count_function<T>(n, val));

        return n;
    }

    /// Returns the number of elements in the range matching the predicate \c pr
    template <ss_typename_param_k P>
    size_t count_if(P pr) const
    {
        size_t  n   =   0;

        m_r.for_each_cancelable(count_if_function<P>(n, pr));

        return n;
    }

    /// Returns the number of elements in the range
    ptrdiff_t distance() const
    {
        ptrdiff_t   n = 0;

        m_r.for_each_cancelable(distance_function(n));

        return n;
    }

    /// Applied the functor \c f to each element in the array
    template <ss_typename_param_k F>
    F for_each(F f)
    {
        m_r.for_each_cancelable(for_each_function<F>(f));

        return f;
    }

    /// Returns true if the element \c val exists in the range
    template <ss_typename_param_k T>
    ss_bool_t exists(T const &val) const
    {
        ss_bool_t   bExists = false;

        m_r.for_each_cancelable(exists_function<T>(bExists, val));

        return bExists;
    }

    /// Returns true if any element in given predicate \c pr evaluates true
    /// any element in the range
    template <ss_typename_param_k P>
    ss_bool_t exists_if(P pr) const
    {
        ss_bool_t   bExists = false;

        m_r.for_each_cancelable(exists_if_1_function<P>(bExists, pr));

        return bExists;
    }

    /// Returns true if any element in given predicate \c pr evaluates true 
    /// any element in the range, and places the matching element in \c result
    template<   ss_typename_param_k P
            ,   ss_typename_param_k T>
    ss_bool_t exists_if(P pr, T &result) const
    {
        ss_bool_t   bExists = false;

        m_r.for_each_cancelable(exists_if_2_function<P, T>(bExists, pr, result));

        return bExists;
    }

    /// Returns the value of the maximum elements in the range
    value_type max_element() const
    {
        return minmax_element(std::less<value_type>());
    }

    /// Returns the value of the maximum elements in the range, as determined by
    /// the comparand predicate \c pr
    template <ss_typename_param_k P>
    value_type max_element(P pr) const
    {
        return minmax_element(pr);
    }

    /// Returns the value of the minimum elements in the range
    value_type min_element() const
    {
        return minmax_element(std::greater<value_type>());
    }

    /// Returns the value of the minimum elements in the range, as determined by
    /// the comparand predicate \c pr
    template <ss_typename_param_k P>
    value_type min_element(P pr) const
    {
        return minmax_element(std::not2(pr));
    }

private:
    template <ss_typename_param_k P>
    value_type minmax_element(P pr) const
    {
        ss_bool_t   bRetrieved;
        value_type  result;

        m_r.for_each_cancelable(minmax_element_2_function<value_type, P>(bRetrieved, pr, result));

        stlsoft_assert(bRetrieved);

        return result;
    }

private:
    range_type  m_r;
};

////////////////////////////////////////////////////////////////////////////////
// Unit-testing

#ifdef STLSOFT_UNITTEST

namespace unittest
{
    ss_bool_t test_rangelib_indirect_range_adaptors(unittest_reporter *r)
    {
        using stlsoft::unittest::unittest_initialiser;

        ss_bool_t               bSuccess    =   true;

        unittest_initialiser    init(r, "RangeLib", "indirect_range_adaptors", __FILE__);

#if 0
        if(
        {
            r->report("construct() failed ", __LINE__);
            bSuccess = false;
        }
#endif /* 0 */

        return bSuccess;
    }

    unittest_registrar    unittest_rangelib_indirect_range_adaptors(test_rangelib_indirect_range_adaptors);

} // namespace unittest

#endif /* STLSOFT_UNITTEST */

/* ////////////////////////////////////////////////////////////////////////// */

/// @} // end of group rangelib_adaptors

/* ////////////////////////////////////////////////////////////////////////// */

#ifndef _STLSOFT_NO_NAMESPACE
} // namespace stlsoft
#endif /* _STLSOFT_NO_NAMESPACE */

/* ////////////////////////////////////////////////////////////////////////// */

#endif /* !STLSOFT_INCL_RANGELIB_HPP_BASIC_INDIRECT_RANGE_ADAPTOR */

/* ////////////////////////////////////////////////////////////////////////// */
