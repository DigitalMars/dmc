/* ////////////////////////////////////////////////////////////////////////////
 * File:        unixstl_process_mutex.h
 *
 * Purpose:     Intra-process mutext, based on PTHREADS.
 *
 * Date:        15th May 2002
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


/// \file unixstl_process_mutex.h
///
/// Intra-process mutext, based on PTHREADS.

#ifndef UNIXSTL_INCL_H_UNIXSTL_PROCESS_MUTEX
#define UNIXSTL_INCL_H_UNIXSTL_PROCESS_MUTEX

#ifndef __STLSOFT_DOCUMENTATION_SKIP_SECTION
# define UNIXSTL_VER_H_UNIXSTL_PROCESS_MUTEX_MAJOR      2
# define UNIXSTL_VER_H_UNIXSTL_PROCESS_MUTEX_MINOR      0
# define UNIXSTL_VER_H_UNIXSTL_PROCESS_MUTEX_REVISION   1
# define UNIXSTL_VER_H_UNIXSTL_PROCESS_MUTEX_EDIT       26
#endif /* !__STLSOFT_DOCUMENTATION_SKIP_SECTION */

/* /////////////////////////////////////////////////////////////////////////////
 * Includes
 */

#ifndef UNIXSTL_INCL_H_UNIXSTL
# include "unixstl.h"    // Include the UNIXSTL root header
#endif /* !UNIXSTL_INCL_H_UNIXSTL */
#if !defined(_REENTRANT) && \
    !defined(_POSIX_THREADS)
# error unixstl_process_mutex.h must be compiled in the context of PTHREADS
#endif /* !_REENTRANT && !_POSIX_THREADS */
#include <pthread.h>

/* /////////////////////////////////////////////////////////////////////////////
 * Namespace
 */

#ifndef _UNIXSTL_NO_NAMESPACE
# if defined(_STLSOFT_NO_NAMESPACE) || \
     defined(__STLSOFT_DOCUMENTATION_SKIP_SECTION)
/* There is no stlsoft namespace, so must define ::unixstl */
namespace unixstl
{
# else
/* Define stlsoft::unixstl_project */

namespace stlsoft
{

namespace unixstl_project
{

# endif /* _STLSOFT_NO_NAMESPACE */
#endif /* !_UNIXSTL_NO_NAMESPACE */

/* /////////////////////////////////////////////////////////////////////////////
 * Classes
 */

// class process_mutex
/// This class provides an implementation of the mutex model based on the Win32 CRITICAL_SECTION
class process_mutex
{
public:
    typedef process_mutex class_type;

// Construction
public:
    /// Creates an instance of the mutex
    ss_explicit_k process_mutex(us_bool_t bRecursive) unixstl_throw_0()
        : m_init(create_(&m_mx, 0, bRecursive))
    {}
#if defined(_POSIX_THREAD_PROCESS_SHARED)
    /// Creates an instance of the mutex
    process_mutex(int pshared, us_bool_t bRecursive) unixstl_throw_0()
        : m_init(create_(&m_mx, pshared, bRecursive))
    {}
#endif /* _POSIX_THREAD_PROCESS_SHARED */
    /// Destroys an instance of the mutex
    ~process_mutex() unixstl_throw_0()
    {
        if(m_init)
        {
            ::pthread_mutex_destroy(&m_mx);
        }
    }

// Operations
public:
    /// Acquires a lock on the mutex, pending the thread until the lock is aquired
    void lock() unixstl_throw_0()
    {
        pthread_mutex_lock(&m_mx);
    }
    /// Attempts to lock the mutex
    ///
    /// \return <b>true</b> if the mutex was aquired, or <b>false</b> if not
    /// \note Only available with Windows NT 4 and later
    bool try_lock()
    {
        return pthread_mutex_trylock(&m_mx) == 0;
    }
    /// Releases an aquired lock on the mutex
    void unlock() unixstl_throw_0()
    {
        pthread_mutex_unlock(&m_mx);
    }

// Implementation
private:
    static us_bool_t create_(pthread_mutex_t *mx, int pshared, us_bool_t bRecursive)
    {
        us_bool_t           bSuccess    =   false;
        pthread_mutexattr_t attr;

        if(0 == ::pthread_mutexattr_init(&attr))
        {
            if( !bRecursive ||
                0 == ::pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE))
            {
#if defined(_POSIX_THREAD_PROCESS_SHARED)
                if(0 == ::pthread_mutexattr_setpshared(&attr, pshared))
#else
                STLSOFT_SUPPRESS_UNUSED(pshared);
#endif /* _POSIX_THREAD_PROCESS_SHARED */
                {
                    if(0 == ::pthread_mutex_init(mx, &attr))
                    {
                        bSuccess = true;
                    }
                }
            }

            ::pthread_mutexattr_destroy(&attr);
        }

        return bSuccess;
    }

// Members
private:
    pthread_mutex_t m_mx;   // mx
    us_bool_t       m_init;

// Not to be implemented
private:
    process_mutex(class_type const &rhs);
    process_mutex &operator =(class_type const &rhs);
};

/* /////////////////////////////////////////////////////////////////////////////
 * Control shims
 */

#ifndef _UNIXSTL_NO_NAMESPACE
# if defined(_STLSOFT_NO_NAMESPACE) || \
     defined(__STLSOFT_DOCUMENTATION_SKIP_SECTION)
} // namespace unixstl
# else
} // namespace unixstl_project
# endif /* _STLSOFT_NO_NAMESPACE */
#endif /* !_UNIXSTL_NO_NAMESPACE */

/// \weakgroup concepts STLSoft Concepts

/// \weakgroup concepts_shims Shims
/// \ingroup concepts

/// \weakgroup concepts_shims_sync_control Synchronisation Control Shims
/// \ingroup concepts_shims
/// \brief These \ref concepts_shims "shims" control the behaviour of synchronisation objects

/// \defgroup unixstl_sync_control_shims Synchronisation Control Shims (UNIXSTL)
/// \ingroup UNIXSTL concepts_shims_sync_control
/// \brief These \ref concepts_shims "shims" control the behaviour of Win32 synchronisation objects
/// @{

/// This control ref concepts_shims "shim" aquires a lock on the given mutex
///
/// \param mx The mutex on which to aquire the lock
inline void lock_instance(unixstl_ns_qual(process_mutex) &mx)
{
    mx.lock();
}

/// This control ref concepts_shims "shim" releases a lock on the given mutex
///
/// \param mx The mutex on which to release the lock
inline void unlock_instance(unixstl_ns_qual(process_mutex) &mx)
{
    mx.unlock();
}

/// @} // end of group unixstl_sync_control_shims

#ifndef _UNIXSTL_NO_NAMESPACE
# if defined(_STLSOFT_NO_NAMESPACE) || \
     defined(__STLSOFT_DOCUMENTATION_SKIP_SECTION)
namespace unixstl
{
# else /* ? _STLSOFT_NO_NAMESPACE */
namespace unixstl_project
{
# endif /* _STLSOFT_NO_NAMESPACE */
#endif /* !_UNIXSTL_NO_NAMESPACE */

/* /////////////////////////////////////////////////////////////////////////////
 * lock_traits (for the compilers that do not support Koenig Lookup)
 */

// class lock_traits
/// Traits for the process_mutex class (for compilers that do not support Koenig Lookup)
struct process_mutex_lock_traits
{
public:
    /// The lockable type
    typedef process_mutex                lock_type;
    typedef process_mutex_lock_traits    class_type;

// Operations
public:
    /// Lock the given process_mutex instance
    static void lock(process_mutex &c)
    {
#if defined(__STLSOFT_COMPILER_IS_BORLAND)
        // Borland requires that we explicitly qualify the shim functions, even
        // though they're defined in the enclosing namespace of this one.
        stlsoft_ns_qual(lock_instance)(c);
#else /* ? compiler */
        lock_instance(c);
#endif /* compiler */
    }

    /// Unlock the given process_mutex instance
    static void unlock(process_mutex &c)
    {
#if defined(__STLSOFT_COMPILER_IS_BORLAND)
        // Borland requires that we explicitly qualify the shim functions, even
        // though they're defined in the enclosing namespace of this one.
        stlsoft_ns_qual(unlock_instance)(c);
#else /* ? compiler */
        unlock_instance(c);
#endif /* compiler */
    }
};

////////////////////////////////////////////////////////////////////////////////
// Unit-testing

#ifdef STLSOFT_UNITTEST

namespace unittest
{
    ss_bool_t test_unixstl_process_mutex(unittest_reporter *r)
    {
        using stlsoft::unittest::unittest_initialiser;

        ss_bool_t               bSuccess    =   true;

        unittest_initialiser    init(r, "UNIXSTL", "process_mutex", __FILE__);

        process_mutex   mx_r(true);

        mx_r.lock();
        if(!mx_r.try_lock())
        {
            r->report("process_mutex (recursive) could not lock recursively ", __LINE__);
            bSuccess = false;
        }
        else
        {
            mx_r.unlock();
        }
        mx_r.unlock();

        process_mutex   mx_nr(false);

        mx_nr.lock();
        if(mx_nr.try_lock())
        {
            r->report("process_mutex (non-recursive) could lock recursively ", __LINE__);
            mx_nr.unlock();
            bSuccess = false;
        }
        mx_nr.unlock();

        return bSuccess;
    }

    unittest_registrar    unittest_unixstl_process_mutex(test_unixstl_process_mutex);

} // namespace unittest

#endif /* STLSOFT_UNITTEST */

/* ////////////////////////////////////////////////////////////////////////// */

#ifndef _UNIXSTL_NO_NAMESPACE
# if defined(_STLSOFT_NO_NAMESPACE) || \
     defined(__STLSOFT_DOCUMENTATION_SKIP_SECTION)
} // namespace unixstl
# else
} // namespace unixstl_project
} // namespace stlsoft
# endif /* _STLSOFT_NO_NAMESPACE */
#endif /* !_UNIXSTL_NO_NAMESPACE */

/* ////////////////////////////////////////////////////////////////////////// */

#endif /* !UNIXSTL_INCL_H_UNIXSTL_PROCESS_MUTEX */

/* ////////////////////////////////////////////////////////////////////////// */
