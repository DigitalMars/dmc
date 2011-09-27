/* /////////////////////////////////////////////////////////////////////////////
 * File:        stlsoft_unused_return_value_monitor.h
 *
 * Purpose:     Basic functionals.
 *
 * Created:     8th June 2002
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


/// \file stlsoft_unused_return_value_monitor.h
///
/// Basic functionals.

#ifndef STLSOFT_INCL_H_STLSOFT_UNUSED_RETURN_VALUE_MONITOR
#define STLSOFT_INCL_H_STLSOFT_UNUSED_RETURN_VALUE_MONITOR

#ifndef __STLSOFT_DOCUMENTATION_SKIP_SECTION
# define STLSOFT_VER_H_STLSOFT_UNUSED_RETURN_VALUE_MONITOR_MAJOR        2
# define STLSOFT_VER_H_STLSOFT_UNUSED_RETURN_VALUE_MONITOR_MINOR        0
# define STLSOFT_VER_H_STLSOFT_UNUSED_RETURN_VALUE_MONITOR_REVISION     1
# define STLSOFT_VER_H_STLSOFT_UNUSED_RETURN_VALUE_MONITOR_EDIT         26
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

// class unused_return_value_monitor
/// Return value adaptor for monitoring whether return values are used.
///
/// \param V The value type. This is the type that is to be returned by the function whose return type is to be monitored
/// \param M The monitor function type. If the return value is not used, the monitor function instance will be called.
/// \param R The reference type. This is the reference type, used in the constructor of the monitor, and the type of the member variable storing the value
///
/// If the value type is a simple type, you can just allow the reference type R to be defaulted, as in:
///
/// \htmlonly
/// <code>
/// struct int_monitor
/// <br>
/// {
/// <br>
/// public:
/// <br>
///   void operator ()(void const *instance, int value) const
/// <br>
///   {
/// <br>
///     printf("Unused return value %d from object instance %p\n", value, instance);
/// <br>
///   }
/// <br>
/// };
/// <br>
///
/// <br>
/// class X
/// <br>
/// {
/// <br>
/// public:
/// <br>
/// &nbsp;&nbsp;unused_return_value_monitor&lt;int, int_monitor&gt;   fn()
/// <br>
/// &nbsp;&nbsp;{
/// <br>
/// &nbsp;&nbsp;&nbsp;&nbsp;return 10; // A copy is taken, but copies of ints usually cost the same as references to ints
/// <br>
/// &nbsp;&nbsp;}
/// <br>
/// };
/// <br>
/// <br>
/// <br>
/// </code>
/// \endhtmlonly
///
///
/// Where the return type is a complex type, you can make efficiency savings by using a (C++) reference for the reference type. However,
/// this must only be done when the returned value will persist for the duration of the calling expression.
///
/// \htmlonly
/// <code>
/// struct string_monitor
/// <br>
/// {
/// <br>
/// public:
/// <br>
/// &nbsp;&nbsp;void operator ()(void const *instance, string const &value) const
/// <br>
/// &nbsp;&nbsp;{
/// <br>
/// &nbsp;&nbsp;&nbsp;&nbsp;printf("Unused return value %s from object instance %p\n", value.c_str(), instance);
/// <br>
/// &nbsp;&nbsp;}
/// <br>
/// };
/// <br>
///
/// <br>
/// class Y1
/// <br>
/// {
/// <br>
/// public:
/// <br>
/// &nbsp;&nbsp;Y2()
/// <br>
/// &nbsp;&nbsp;&nbsp;&nbsp;: m_str("What's new, Pussycat?")
/// <br>
/// &nbsp;&nbsp;{}
/// <br>
///
/// <br>
/// public:
/// <br>
/// &nbsp;&nbsp;// Must store return value by value, since the returned value will not persist for the duration of the calling expression
/// <br>
/// &nbsp;&nbsp;unused_return_value_monitor&lt;string, string_monitor&gt; fn_by_value()
/// <br>
/// &nbsp;&nbsp;{
/// <br>
/// &nbsp;&nbsp;&nbsp;&nbsp;return "Hello sailor";
/// <br>
/// &nbsp;&nbsp;}
/// <br>
///
/// <br>
/// &nbsp;&nbsp;// Must store return value by (const) reference, since the returned value will persist for the duration of the calling expression
/// <br>
/// &nbsp;&nbsp;unused_return_value_monitor&lt;string, string_monitor, string const &&gt; fn_by_ref()
/// <br>
/// &nbsp;&nbsp;{
/// <br>
/// &nbsp;&nbsp;&nbsp;&nbsp;return m_str;
/// <br>
/// &nbsp;&nbsp;}
/// <br>
///
/// <br>
/// private:
/// <br>
/// &nbsp;&nbsp;string const&nbsp;&nbsp;m_str;
/// <br>
/// };
/// <br>
/// </code>
/// \endhtmlonly
///

template<   ss_typename_param_k V
        ,   ss_typename_param_k M
        ,   ss_typename_param_k R = V // Could be V const &, if you know the value is to persist
        >
class unused_return_value_monitor
{
public:
    typedef V                                       value_type;
    typedef M                                       monitor_function;
    typedef R                                       reference_type;
    typedef unused_return_value_monitor<V, M, R>    class_type;

/// \name Construction
/// @{
public:
    /* ss_explicit_k */ unused_return_value_monitor(reference_type value)
        : m_value(value)
        , m_monitorFn()
        , m_bUsed(false)
    {}
    unused_return_value_monitor(reference_type value, monitor_function monitorFn)
        : m_value(value)
        , m_monitorFn(monitorFn)
        , m_bUsed(false)
    {}
    unused_return_value_monitor(class_type const &rhs)
        : m_value(rhs.m_value)
        , m_monitorFn(rhs.m_monitorFn)
        , m_bUsed(rhs.m_bUsed)
    {
        rhs.m_bUsed = false;
    }
    ~unused_return_value_monitor()
    {
        if(!m_bUsed)
        {
            m_monitorFn(this, m_value);
        }
    }
/// @}

/// \name Conversion
/// @{
public:
    operator value_type() const
    {
        const_cast<class_type*>(this)->m_bUsed = true;

        return m_value;
    }
/// @}

/// Members
private:
    reference_type      m_value;
    monitor_function    m_monitorFn;
    ss_bool_t           m_bUsed;

// Not to be implemented
private:
    unused_return_value_monitor &operator =(class_type const &rhs);
};

/* ////////////////////////////////////////////////////////////////////////// */

#ifndef _STLSOFT_NO_NAMESPACE
} // namespace stlsoft
#endif /* _STLSOFT_NO_NAMESPACE */

/* ////////////////////////////////////////////////////////////////////////// */

#endif /* !STLSOFT_INCL_H_STLSOFT_UNUSED_RETURN_VALUE_MONITOR */

/* ////////////////////////////////////////////////////////////////////////// */
