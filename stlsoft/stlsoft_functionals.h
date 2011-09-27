/* /////////////////////////////////////////////////////////////////////////////
 * File:        stlsoft_functionals.h (originally MLCpp.h, ::SynesisStd)
 *
 * Purpose:     Basic functionals.
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


/// \file stlsoft_functionals.h
///
/// Basic functionals.

#ifndef STLSOFT_INCL_H_STLSOFT_FUNCTIONALS
#define STLSOFT_INCL_H_STLSOFT_FUNCTIONALS

#ifndef __STLSOFT_DOCUMENTATION_SKIP_SECTION
# define STLSOFT_VER_H_STLSOFT_FUNCTIONALS_MAJOR    2
# define STLSOFT_VER_H_STLSOFT_FUNCTIONALS_MINOR    0
# define STLSOFT_VER_H_STLSOFT_FUNCTIONALS_REVISION 1
# define STLSOFT_VER_H_STLSOFT_FUNCTIONALS_EDIT     27
#endif /* !__STLSOFT_DOCUMENTATION_SKIP_SECTION */

/* /////////////////////////////////////////////////////////////////////////////
 * Includes
 */

#ifndef STLSOFT_INCL_H_STLSOFT
# include "stlsoft.h"   // Include the STLSoft root header
#endif /* !STLSOFT_INCL_H_STLSOFT */
#include <functional>   // std::unary_function

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

// struct noop_function
//
/// This functional performs no operation at all, and is simply a place-holder

template <ss_typename_param_k T>
struct noop_function
    : public stlsoft_ns_qual_std(unary_function)<T const &, void>
{
public:
    /// The function call operator, which does nothing
    void operator ()(T const &/* t */) stlsoft_throw_0()
    {}
};


// struct delete_instance
//
/// This functional deletes an object instance, via scalar delete

template <ss_typename_param_k T>
struct delete_instance
    : public stlsoft_ns_qual_std(unary_function)<T *, void>
{
public:
    /// The function call operator, which deletes the given instance
    ///
    /// \param pt A pointer to an instance of type T to be deleted
    void operator ()(T *pt) stlsoft_throw_0()
    {
        delete pt;
    }
};


// struct delete_array
//
/// This functional deletes an array of objects, via vector delete

template <ss_typename_param_k T>
struct delete_array
    : public stlsoft_ns_qual_std(unary_function)<T [], void>
{
public:
    /// The function call operator, which deletes the given array
    ///
    /// \param t A pointer to an array of type T to be deleted
    void operator ()(T t[]) stlsoft_throw_0()
    {
        delete [] t;
    }
};


// struct select_1st

/// Selects the <b><code>first</code></b> member of an instance and applies the
/// parameterising functional to it. This functional selects the \c first member
/// of an instance (obviously this is usually the \c std::pair type), and
/// applies the parameterising functional to it.
///
/// For example, if you have a std::map and wish to write out the keys
/// with a dump_key functional, you could achieve this with the following:
///
/// &nbsp;&nbsp;<code>std::for_each(m.begin(), m.end(), stlsoft::select_1st<dump_key>());</code>
///
/// \param F The parameterising functional
template<   ss_typename_param_k F
#ifndef __STLSOFT_CF_MEMBER_TEMPLATE_FUNCTION_SUPPORT
        ,   ss_typename_param_k T
#endif // __STLSOFT_CF_MEMBER_TEMPLATE_FUNCTION_SUPPORT
        >
struct select_1st
    : public stlsoft_ns_qual_std(unary_function)<   ss_typename_type_k F::argument_type
                                                ,   ss_typename_type_k F::result_type>
{
public:
    /// Default constructor
    select_1st()
        : m_f()
    {}

    /// Constructs from the given function object, which it will then apply
    /// via operator ()()
    ss_explicit_k select_1st(F f)
        : m_f(f)
    {}

    /// Function call operator, which applies the parameterising function object
    /// to the \c first part of the pair \c t
    ///
    /// \param t An instance of a \c pair like type, to whose \c first member will be applied the function F
    // Regrettably, the current implementation only provides an instantiation
    // returning void
#ifdef __STLSOFT_CF_MEMBER_TEMPLATE_FUNCTION_SUPPORT
    template <ss_typename_param_k T>
#endif // __STLSOFT_CF_MEMBER_TEMPLATE_FUNCTION_SUPPORT
    void operator ()(T &t)
    {
        m_f(t.first);
    }

// Members
private:
    F   m_f;
};

#ifdef __STLSOFT_CF_MEMBER_TEMPLATE_FUNCTION_SUPPORT
template<   ss_typename_param_k F
        >
inline select_1st<F> make_1st_selector(F f)
{
    return select_1st<F>(f);
}
#else /* ? __STLSOFT_CF_MEMBER_TEMPLATE_FUNCTION_SUPPORT */

#endif // __STLSOFT_CF_MEMBER_TEMPLATE_FUNCTION_SUPPORT

// struct select_2nd
//
/// \brief Selects the <b><code>second</code></b> member of an instance and
/// applies the parameterising functional to it.
///
/// This functional selects the "second" member of an instance (obviously
/// this is usually the std::pair type), and applies the parameterising
/// functional to it.
///
/// For example, if you have a std::map and wish to write out the values
/// with a dump_value functional, you could achieve this with the following:
///
/// &nbsp;&nbsp;<code>std::for_each(m.begin(), m.end(), stlsoft::select_2nd<dump_value>());</code>
///
/// \param F The parameterising functional

template<   ss_typename_param_k F
#ifndef __STLSOFT_CF_MEMBER_TEMPLATE_FUNCTION_SUPPORT
        ,   ss_typename_param_k T
#endif // __STLSOFT_CF_MEMBER_TEMPLATE_FUNCTION_SUPPORT
        >
struct select_2nd
    : public stlsoft_ns_qual_std(unary_function)<ss_typename_type_k F::argument_type, ss_typename_type_k F::result_type>
{
public:
    /// Default constructor
    select_2nd()
        : m_f()
    {}

    /// Constructs from the given function object, which it will then apply
    /// via operator ()()
    ss_explicit_k select_2nd(F f)
        : m_f(f)
    {}

    /// Function call operator, which applies the parameterising function object
    /// to the \c second part of the pair \c t
    ///
    /// \param t An instance of a \c pair like type, to whose \c second member will be applied the function F
    // Regrettably, the current implementation only provides an instantiation
    // returning void
#ifdef __STLSOFT_CF_MEMBER_TEMPLATE_FUNCTION_SUPPORT
    template <ss_typename_param_k T>
#endif // __STLSOFT_CF_MEMBER_TEMPLATE_FUNCTION_SUPPORT
    void operator ()(T &t)
    {
        m_f(t.second);
    }

// Members
private:
    F   m_f;
};


// struct select_both
//
/// This functional selects both the \c first and \c second members of an instance
/// (obviously this is usually the std::pair type), and applies the respective
/// parameterising functionals to them.
///
/// For example, if you have a std::map and wish to write out the keys with the
/// dump_key functional and the values with the dump_value functional, you could
/// achieve this with the following:
///
/// &nbsp;&nbsp;<code>std::for_each(m.begin(), m.end(), stlsoft::select_both<dump_key, dump_value>());</code>
///
/// \param F1 The functional to apply to the <b><code>first</code></b> part of the elements
/// \param F2 The functional to apply to the <b><code>second</code></b> part of the elements

template<   ss_typename_param_k F1
        ,   ss_typename_param_k F2
#ifndef __STLSOFT_CF_MEMBER_TEMPLATE_FUNCTION_SUPPORT
        ,   ss_typename_param_k T
#endif // __STLSOFT_CF_MEMBER_TEMPLATE_FUNCTION_SUPPORT
        >
struct select_both
#ifdef __STLSOFT_CF_MEMBER_TEMPLATE_FUNCTION_SUPPORT
    : public stlsoft_ns_qual_std(unary_function)<void, void>
#else
    : public stlsoft_ns_qual_std(unary_function)<T &, void>
#endif // __STLSOFT_CF_MEMBER_TEMPLATE_FUNCTION_SUPPORT
{
public:
    /// Default constructor
    select_both()
        : m_f1()
        , m_f2()
    {}

    /// Constructs from the given function objects, which it will then apply
    /// via operator ()()
    ss_explicit_k select_both(F1 f1, F2 f2)
        : m_f1(f1)
        , m_f2(f2)
    {}

    /// Function call operator, which applies the parameterising function objects
    /// to the \c first and \c second parts of the pair \c t
    ///
    /// \param t An instance of a \c pair like type, to whose \c first and \c second members will be applied the functions F1 and F2
    // Regrettably, the current implementation only provides an instantiation
    // returning void
#ifdef __STLSOFT_CF_MEMBER_TEMPLATE_FUNCTION_SUPPORT
    template <ss_typename_param_k T>
#endif // __STLSOFT_CF_MEMBER_TEMPLATE_FUNCTION_SUPPORT
    void operator ()(T &t)
    {
        m_f1(t.first);
        m_f2(t.second);
    }

// Members
private:
    F1  m_f1;
    F2  m_f2;
};


/* ////////////////////////////////////////////////////////////////////////// */

#ifndef _STLSOFT_NO_NAMESPACE
} // namespace stlsoft
#endif /* _STLSOFT_NO_NAMESPACE */

/* ////////////////////////////////////////////////////////////////////////// */

#endif /* !STLSOFT_INCL_H_STLSOFT_FUNCTIONALS */

/* ////////////////////////////////////////////////////////////////////////// */
