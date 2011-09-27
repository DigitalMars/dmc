/* ////////////////////////////////////////////////////////////////////////////
 * File:        stlsoft_lock_scope.h (originally MLLock.h, ::SynesisStd)
 *
 * Purpose:     Synchronisation object lock scoping class.
 *
 * Created:     1st October 1994
 * Updated:     11th September 2004
 *
 * Home:        http://stlsoft.org/
 *
 * Copyright (c) 1994-2004, Matthew Wilson and Synesis Software
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


/// \file stlsoft_lock_scope.h
///
/// Synchronisation object lock scoping class.

#ifndef STLSOFT_INCL_H_STLSOFT_LOCK_SCOPE
#define STLSOFT_INCL_H_STLSOFT_LOCK_SCOPE

#ifndef __STLSOFT_DOCUMENTATION_SKIP_SECTION
# define STLSOFT_VER_H_STLSOFT_LOCK_SCOPE_MAJOR     4
# define STLSOFT_VER_H_STLSOFT_LOCK_SCOPE_MINOR     0
# define STLSOFT_VER_H_STLSOFT_LOCK_SCOPE_REVISION  1
# define STLSOFT_VER_H_STLSOFT_LOCK_SCOPE_EDIT      95
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
 * Classes
 */

// class lock_traits

/// Traits class for lockable objects
///
/// \param L The lockable class
template<ss_typename_param_k L>
struct lock_traits
{
public:
    /// The lockable type
    typedef L               lock_type;
    /// The current parameterisation of this type
    typedef lock_traits<L>  class_type;

// Operations
public:
    /// Locks the given lockable instance
    static void lock(lock_type &c)
    {
        lock_instance(c);
    }

    /// Unlocks the given lockable instance
    static void unlock(lock_type &c)
    {
        unlock_instance(c);
    }
};

// class lock_invert_traits

/// Traits class for inverting the lock status of lockable objects
///
/// \param L The lockable class
template<ss_typename_param_k L>
struct lock_invert_traits
{
public:
    /// The lockable type
    typedef L                       lock_type;
    /// The current parameterisation of this type
    typedef lock_invert_traits<L>   class_type;

// Operations
public:
    /// Unlocks the given lockable instance
    static void lock(lock_type &c)
    {
        unlock_instance(c);
    }

    /// Locks the given lockable instance
    static void unlock(lock_type &c)
    {
        lock_instance(c);
    }
};

// class lock_traits_inverter

/// Traits inverter class for inverting the lock behaviour of lockable traits types
///
/// \param L The traits class
template<ss_typename_param_k T>
struct lock_traits_inverter
{
public:
    /// The traits type
    typedef T                                           traits_type;
    /// The lockable type
    typedef ss_typename_type_k traits_type::lock_type   lock_type;
    /// The current parameterisation of this type
    typedef lock_traits_inverter<T>                     class_type;

// Operations
public:
    /// Unlocks the given lockable instance
    static void lock(lock_type &c)
    {
        traits_type::unlock(c);
    }

    /// Locks the given lockable instance
    static void unlock(lock_type &c)
    {
        traits_type::lock(c);
    }
};

// class lock_scope

/// This class scopes the lock status of a lockable type
///
/// \param L The lockable type, e.g. stlsoft::null_mutex
/// \param T The lock traits. On translators that support default template arguments this defaults to lock_traits<L>
template<   ss_typename_param_k L
#ifdef __STLSOFT_CF_TEMPLATE_CLASS_DEFAULT_CLASS_ARGUMENT_SUPPORT
        ,   ss_typename_param_k T = lock_traits<L>
#else
        ,   ss_typename_param_k T
#endif /* __STLSOFT_CF_TEMPLATE_CLASS_DEFAULT_CLASS_ARGUMENT_SUPPORT */
        >
class lock_scope
{
public:
    /// The lockable type
    typedef L                       lock_type;
    /// The traits type
    typedef T                       traits_type;
    /// The current parameterisation of this type
    typedef lock_scope<L, T>        class_type;

// Construction
public:
    /// Locks the lockable instance
    lock_scope(lock_type &l)
        : m_l(l)
    {
        traits_type::lock(m_l);
    }
    /// Unlocks the lockable instance
    ~lock_scope()
    {
        traits_type::unlock(m_l);
    }

// Members
private:
    lock_type   &m_l;

// Not to be implemented
private:
    lock_scope(class_type const &rhs);
    lock_scope &operator =(class_type const &rhs);
};

/* ////////////////////////////////////////////////////////////////////////// */

#ifndef _STLSOFT_NO_NAMESPACE
} // namespace stlsoft
#endif /* _STLSOFT_NO_NAMESPACE */

/* ////////////////////////////////////////////////////////////////////////// */

#endif /* !STLSOFT_INCL_H_STLSOFT_LOCK_SCOPE */

/* ////////////////////////////////////////////////////////////////////////// */
