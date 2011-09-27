/* /////////////////////////////////////////////////////////////////////////////
 * File:        stlsoft_auto_destructor.h (originally MLAutDtr.h, ::SynesisStd)
 *
 * Purpose:     Contains the auto_destructor and auto_array_destructor template
 *              classes.
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


/// \file stlsoft_auto_destructor.h
///
/// Contains the auto_destructor and auto_array_destructor template classes.

#ifndef STLSOFT_INCL_H_STLSOFT_AUTO_DESTRUCTOR
#define STLSOFT_INCL_H_STLSOFT_AUTO_DESTRUCTOR

#ifndef __STLSOFT_DOCUMENTATION_SKIP_SECTION
# define STLSOFT_VER_H_STLSOFT_AUTO_DESTRUCTOR_MAJOR      2
# define STLSOFT_VER_H_STLSOFT_AUTO_DESTRUCTOR_MINOR      0
# define STLSOFT_VER_H_STLSOFT_AUTO_DESTRUCTOR_REVISION   1
# define STLSOFT_VER_H_STLSOFT_AUTO_DESTRUCTOR_EDIT       43
#endif /* !__STLSOFT_DOCUMENTATION_SKIP_SECTION */

/* /////////////////////////////////////////////////////////////////////////////
 * Includes
 */

#ifndef STLSOFT_INCL_H_STLSOFT
# include "stlsoft.h"   // Include the STLSoft root header
#endif /* !STLSOFT_INCL_H_STLSOFT */

/* /////////////////////////////////////////////////////////////////////////////
 * Namespace
 */

#ifndef _STLSOFT_NO_NAMESPACE
namespace stlsoft
{
#endif /* _STLSOFT_NO_NAMESPACE */

/* /////////////////////////////////////////////////////////////////////////////
 * Forward declarations
 */

#ifndef __STLSOFT_DOCUMENTATION_SKIP_SECTION

template <ss_typename_param_k T>
class auto_destructor;

template <ss_typename_param_k T>
class return_value_destructor;

template <ss_typename_param_k T>
class auto_array_destructor;

template <ss_typename_param_k T>
class return_value_array_destructor;

#endif /* !__STLSOFT_DOCUMENTATION_SKIP_SECTION */

/* /////////////////////////////////////////////////////////////////////////////
 * Classes
 */

// class auto_destructor
/// This class acts as an automatic frame scope variable that manages
/// heap-allocated object instances.
///
/// \param T The value type
template <ss_typename_param_k T>
class auto_destructor
{
public:
    /// The value type
    typedef T                                               value_type;
    /// The current parameterisation of the type
    typedef auto_destructor<T>                              class_type;
    /// The return value type
    typedef return_value_destructor<T>                      return_value_type;
    /// The <a href = "http://synesis.com.au/resources/articles/cpp/movectors.pdf">move construction</a> type
    typedef stlsoft_define_move_rhs_type(return_value_type) return_value_rhs_type;

// Construction
public:
    /// Constructs from the pointer to an instance whose lifetime will be managedd
    ss_explicit_k auto_destructor(value_type *t)
        : m_value(t)
    {}
    /// Constructs from the return_value_type, transferring ownership from it
    auto_destructor(return_value_rhs_type rhs) // Note: not explicit
        : m_value(move_lhs_from_rhs(rhs).detach())
    {}
    /// Destroys the managed instance
    ~auto_destructor() stlsoft_throw_0()
    {
        delete m_value;
    }

// Operations
public:
    /// Detaches the managed instance from the auto_destructor and returns a pointer to it to the caller
    ///
    /// \note The caller becomes responsible for destroying the instance.
    value_type *detach()
    {
        value_type  *t = m_value;

        m_value = 0;

        return t;
    }

// Conversion
public:
    /// Implicit conversion to pointer to the managed instance
    operator value_type *()
    {
        return m_value;
    }
    /// Implicit conversion to pointer-to-const to the managed instance
    operator const value_type *() const
    {
        return m_value;
    }

// Members
private:
    value_type  *m_value;

// Not to be implemented
private:
    auto_destructor(class_type const &rhs);
    auto_destructor const &operator =(class_type const &rhs);
};

// class auto_array_destructor
/// This class acts as an automatic frame scope variable that manages
/// heap-allocated object arrays.
///
/// \param T The value type
template <ss_typename_param_k T>
class auto_array_destructor
{
public:
    /// The value type
    typedef T                                               value_type;
    /// The current parameterisation of the type
    typedef auto_array_destructor<T>                        class_type;
    /// The return value type
    typedef return_value_array_destructor<T>                return_value_type;
    /// The <a href = "http://synesis.com.au/resources/articles/cpp/movectors.pdf">move construction</a> type
    typedef stlsoft_define_move_rhs_type(return_value_type) return_value_rhs_type;

// Construction
public:
    /// Constructs from the pointer to the array whose lifetimes will be managedd
    ss_explicit_k auto_array_destructor(value_type t[])
        : m_value(t)
    {}
    /// Construct from an return_value_array_destructor<T>, transferring the managed instance from it
    auto_array_destructor(return_value_rhs_type rhs) // Note: not explicit
        : m_value(move_lhs_from_rhs(rhs).detach())
    {}
    /// Destroys the managed array
    ~auto_array_destructor() stlsoft_throw_0()
    {
        delete [] m_value;
    }

// Operations
public:
    /// Detaches the managed instance from the auto_array_destructor and returns a pointer to it to the caller
    ///
    /// \note The caller becomes responsible for destroying the instance.
    value_type *detach()
    {
        value_type  *t = m_value;

        m_value = 0;

        return t;
    }

// Conversion
public:
    /// Implicit conversion to pointer to the managed instance
    operator value_type *()
    {
        return m_value;
    }
    /// Implicit conversion to pointer-to-const to the managed instance
    operator const value_type *() const
    {
        return m_value;
    }

// Members
private:
    value_type  *m_value;

// Not to be implemented
private:
    auto_array_destructor(class_type const &rhs);
    auto_array_destructor const &operator =(class_type const &rhs);
};


// class return_value_destructor
/// This class acts as a return-value scope variable that manages
/// heap-allocated object instances.
///
/// \param T The value type
template <ss_typename_param_k T>
class return_value_destructor
{
public:
    /// The value type
    typedef T                                           value_type;
    /// The current parameterisation of the type
    typedef return_value_destructor<T>                  class_type;
    /// The auto type
    typedef auto_destructor<T>                          auto_type;
    /// The <a href = "http://synesis.com.au/resources/articles/cpp/movectors.pdf">move construction</a> type
    typedef stlsoft_define_move_rhs_type(class_type)    rhs_type;

// Construction
public:
#ifdef _STLSOFT_RETURN_VALUE_DESTRUCTOR_ENABLE_DIRECT_CTOR
    /// Constructs from the pointer to an instance whose lifetime will be managedd
    ss_explicit_k return_value_destructor(value_type *pt)
        : m_value(pt)
    {}
#endif /* _STLSOFT_RETURN_VALUE_DESTRUCTOR_ENABLE_DIRECT_CTOR */
    /// Construct from an auto_destructor<T>, transferring the managed instance from it
    return_value_destructor(auto_type &rhs) // Note: not explicit
        : m_value(rhs.detach())
    {}
    /// <a href = "http://synesis.com.au/resources/articles/cpp/movectors.pdf">Move constructor</a>
    return_value_destructor(rhs_type rhs) // Note: Move-constructor
        : m_value(move_lhs_from_rhs(rhs).detach())
    {}
    /// Destroys the managed instance
    ~return_value_destructor() stlsoft_throw_0()
    {
        // This fires a report if value is non-zero, which indicates that
        // the return value had not been used. This is arguably ok since this
        // only ever happens in debug builds (what would be the point in
        // including in a release build?), so its violation of the rules on
        // no-throw destructors can be forgiven.

#ifndef _STLSOFT_RETURN_VALUE_DESTRUCTOR_DISABLE_UNUSED_ASSERT
#if defined(__STLSOFT_COMPILER_IS_WATCOM)
        stlsoft_assert(m_value == 0);
#else
        stlsoft_message_assert("This return value was not used", m_value == 0);
#endif /* __STLSOFT_COMPILER_IS_WATCOM */
#endif /* !_STLSOFT_RETURN_VALUE_DESTRUCTOR_DISABLE_UNUSED_ASSERT */

        delete m_value;
    }

// Operations
private:
    friend class auto_destructor<T>;

    /// Detaches the managed instance from the return_value_destructor and returns a pointer to it to the caller
    ///
    /// \note The caller becomes responsible for destroying the instance.
    value_type *detach()
    {
        value_type  *t = m_value;

        m_value = 0;

        return t;
    }

// Members
private:
    value_type  *m_value;

// Not to be implemented
private:
    return_value_destructor const &operator = (class_type const &rhs);
};

// class return_value_array_destructor
/// This class acts as a return-value scope variable that manages
/// heap-allocated object arrays.
///
/// \param T The value type
template <ss_typename_param_k T>
class return_value_array_destructor
{
public:
    /// The value type
    typedef T                                           value_type;
    /// The current parameterisation of the type
    typedef return_value_array_destructor<T>            class_type;
    /// The auto type
    typedef auto_array_destructor<T>                    auto_type;
    /// The <a href = "http://synesis.com.au/resources/articles/cpp/movectors.pdf">move construction</a> type
    typedef stlsoft_define_move_rhs_type(class_type)    rhs_type;

// Construction
public:
#ifdef _STLSOFT_RETURN_VALUE_DESTRUCTOR_ENABLE_DIRECT_CTOR
    /// Constructs from the pointer to the array whose lifetimes will be managedd
    ss_explicit_k return_value_array_destructor(value_type t[])
        : m_value(pt)
    {}
#endif /* _STLSOFT_RETURN_VALUE_DESTRUCTOR_ENABLE_DIRECT_CTOR */
    /// Constructs from an auto_array_destructor<T> instance, transferring the managed array from it
    return_value_array_destructor(auto_type &rhs) // Note: not explicit
        : m_value(rhs.detach())
    {}
    /// <a href = "http://synesis.com.au/resources/articles/cpp/movectors.pdf">Move constructor</a>
    return_value_array_destructor(rhs_type rhs) // Note: Move-constructor
        : m_value(move_lhs_from_rhs(rhs).detach())
    {}
    /// Destroys the managed array
    ~return_value_array_destructor() stlsoft_throw_0()
    {
        // This fires a report if value is non-zero, which indicates that
        // the return value had not been used. This is arguably ok since this
        // only ever happens in debug builds (what would be the point in
        // including in a release build?), so its violation of the rules on
        // no-throw destructors can be forgiven.

#ifndef _STLSOFT_RETURN_VALUE_DESTRUCTOR_DISABLE_UNUSED_ASSERT
#if defined(__STLSOFT_COMPILER_IS_WATCOM)
        stlsoft_assert(m_value == 0);
#else
        stlsoft_message_assert("This return value was not used", m_value == 0);
#endif /* __STLSOFT_COMPILER_IS_WATCOM */
#endif /* !_STLSOFT_RETURN_VALUE_DESTRUCTOR_DISABLE_UNUSED_ASSERT */

        delete [] m_value;
    }

// Operations
private:
    friend class auto_array_destructor<T>;

    /// Detaches the managed instance from the return_value_array_destructor and returns a pointer to it to the caller
    ///
    /// \note The caller becomes responsible for destroying the instance.
    value_type *detach()
    {
        value_type  *t = m_value;

        m_value = 0;

        return t;
    }

// Members
private:
    value_type  *m_value;

// Not to be implemented
private:
    return_value_array_destructor const &operator = (class_type const &rhs);
};


/* ////////////////////////////////////////////////////////////////////////// */

#ifndef _STLSOFT_NO_NAMESPACE
} // namespace stlsoft
#endif /* _STLSOFT_NO_NAMESPACE */

/* ////////////////////////////////////////////////////////////////////////// */

#endif /* !STLSOFT_INCL_H_STLSOFT_AUTO_DESTRUCTOR */

/* ////////////////////////////////////////////////////////////////////////// */
