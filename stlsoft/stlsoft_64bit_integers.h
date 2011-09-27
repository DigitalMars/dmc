/* ////////////////////////////////////////////////////////////////////////////
 * File:        stlsoft_64bit_integers.h (originally MWLrgInt.h, ::SynesisWin)
 *
 * Purpose:     Byte-order independent large integer (aggregate) classes.
 *
 * Created:     15th October 2000
 * Updated:     11th September 2004
 *
 * Home:        http://stlsoft.org/
 *
 * Copyright (c) 2000-2004, Matthew Wilson and Synesis Software
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


/// \file stlsoft_64bit_integers.h
///
/// Byte-order independent large integer (aggregate) classes.

#ifndef STLSOFT_INCL_H_STLSOFT_64BIT_INTEGERS
#define STLSOFT_INCL_H_STLSOFT_64BIT_INTEGERS

#ifndef __STLSOFT_DOCUMENTATION_SKIP_SECTION
# define STLSOFT_VER_H_STLSOFT_64BIT_INTEGERS_MAJOR     3
# define STLSOFT_VER_H_STLSOFT_64BIT_INTEGERS_MINOR     0
# define STLSOFT_VER_H_STLSOFT_64BIT_INTEGERS_REVISION  1
# define STLSOFT_VER_H_STLSOFT_64BIT_INTEGERS_EDIT      103
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

/// Signed 64-bit integer class
class sinteger64
{
public:
    typedef sinteger64  class_type;

public:
    sinteger64();
    sinteger64(ss_sint8_t i);
    sinteger64(ss_sint16_t i);
    sinteger64(ss_sint32_t i);
    sinteger64(ss_sint32_t high, ss_sint32_t low);
#ifdef __STLSOFT_CF_INT_DISTINCT_TYPE
    sinteger64(int i);
#endif /* __STLSOFT_CF_INT_DISTINCT_TYPE */

    class_type &operator =(class_type const &rhs);
    class_type &operator =(ss_sint8_t i);
    class_type &operator =(ss_sint16_t i);
    class_type &operator =(ss_sint32_t i);
#ifdef __STLSOFT_CF_INT_DISTINCT_TYPE
    class_type &operator =(int i);
#endif /* __STLSOFT_CF_INT_DISTINCT_TYPE */

    class_type &operator +=(class_type const &rhs);
    class_type &operator +=(ss_sint8_t i);
    class_type &operator +=(ss_sint16_t i);
    class_type &operator +=(ss_sint32_t i);
#ifdef __STLSOFT_CF_INT_DISTINCT_TYPE
    class_type &operator +=(int i);
#endif /* __STLSOFT_CF_INT_DISTINCT_TYPE */

    class_type &operator -=(class_type const &rhs);
    class_type &operator -=(ss_sint8_t i);
    class_type &operator -=(ss_sint16_t i);
    class_type &operator -=(ss_sint32_t i);
#ifdef __STLSOFT_CF_INT_DISTINCT_TYPE
    class_type &operator -=(int i);
#endif /* __STLSOFT_CF_INT_DISTINCT_TYPE */

    class_type &operator *=(class_type const &rhs);
    class_type &operator *=(ss_sint8_t i);
    class_type &operator *=(ss_sint16_t i);
    class_type &operator *=(ss_sint32_t i);
#ifdef __STLSOFT_CF_INT_DISTINCT_TYPE
    class_type &operator *=(int i);
#endif /* __STLSOFT_CF_INT_DISTINCT_TYPE */

    class_type &operator /=(class_type const &rhs);
    class_type &operator /=(ss_sint8_t i);
    class_type &operator /=(ss_sint16_t i);
    class_type &operator /=(ss_sint32_t i);
#ifdef __STLSOFT_CF_INT_DISTINCT_TYPE
    class_type &operator /=(int i);
#endif /* __STLSOFT_CF_INT_DISTINCT_TYPE */

public:
    ss_sint_t compare(class_type const &rhs) const;
    ss_sint_t compare(ss_sint8_t i) const;
    ss_sint_t compare(ss_sint16_t i) const;
    ss_sint_t compare(ss_sint32_t i) const;
#ifdef __STLSOFT_CF_INT_DISTINCT_TYPE
    ss_sint_t compare(int i) const;
#endif /* __STLSOFT_CF_INT_DISTINCT_TYPE */

// Members
private:
    ss_sint32_t m_low;
    ss_sint32_t m_high;
};

/* /////////////////////////////////////////////////////////////////////////////
 * Operators
 */

// operator ==

template <ss_typename_param_k T>
inline ss_bool_t operator ==(sinteger64 const &lhs, T const &rhs)
{
    return 0 == lhs.compare(rhs);
}

template <ss_typename_param_k T>
inline ss_bool_t operator ==(T const &lhs, sinteger64 const &rhs)
{
    return 0 == rhs.compare(lhs);
}

// operator !=

template <ss_typename_param_k T>
inline ss_bool_t operator !=(sinteger64 const &lhs, T const &rhs)
{
    return 0 != lhs.compare(rhs);
}

template <ss_typename_param_k T>
inline ss_bool_t operator !=(T const &lhs, sinteger64 const &rhs)
{
    return 0 != rhs.compare(lhs);
}


// operator <

template <ss_typename_param_k T>
inline ss_bool_t operator <(sinteger64 const &lhs, T const &rhs)
{
    return lhs.compare(rhs) < 0;
}


// operator +

template <ss_typename_param_k T>
inline sinteger64 operator +(sinteger64 const &lhs, T const &rhs)
{
    sinteger64  result(lhs);

    result += rhs;

    return result;
}

// operator -

template <ss_typename_param_k T>
inline sinteger64 operator -(sinteger64 const &lhs, T const &rhs)
{
    sinteger64  result(lhs);

    result -= rhs;

    return result;
}

// operator *

template <ss_typename_param_k T>
inline sinteger64 operator *(sinteger64 const &lhs, T const &rhs)
{
    sinteger64  result(lhs);

    result *= rhs;

    return result;
}

// operator /

template <ss_typename_param_k T>
inline sinteger64 operator /(sinteger64 const &lhs, T const &rhs)
{
    sinteger64  result(lhs);

    result /= rhs;

    return result;
}


/* /////////////////////////////////////////////////////////////////////////////
 * Unit-testing
 */

#ifdef STLSOFT_UNITTEST

namespace unittest
{
    ss_bool_t test_stlsoft_64bit_integers(unittest_reporter *r)
    {
        ss_bool_t               bSuccess    =   true;

        unittest_initialiser    init(r, "STLSoft", "64bit_integers", __FILE__);

#if 0
        if(<<TODO>>)
        {
            r->report("<<TODO>> failed ", __LINE__);
            bSuccess = false;
        }
#endif /* 0 */

        return bSuccess;
    }

    unittest_registrar    unittest_stlsoft_64bit_integers(test_stlsoft_64bit_integers);

} // namespace unittest

#endif /* STLSOFT_UNITTEST */

/* /////////////////////////////////////////////////////////////////////////////
 * Implementation
 */

inline sinteger64::sinteger64()
{}

inline sinteger64::sinteger64(ss_sint8_t i)
    : m_high(0)
    , m_low(i)
{}

inline sinteger64::sinteger64(ss_sint16_t i)
    : m_high(0)
    , m_low(i)
{}

inline sinteger64::sinteger64(ss_sint32_t i)
    : m_high(0)
    , m_low(i)
{}

inline sinteger64::sinteger64(ss_sint32_t high, ss_sint32_t low)
    : m_high(high)
    , m_low(low)
{}

#ifdef __STLSOFT_CF_INT_DISTINCT_TYPE
inline sinteger64::sinteger64(int i)
{}
#endif /* __STLSOFT_CF_INT_DISTINCT_TYPE */

inline sinteger64::class_type &sinteger64::operator =(class_type const &rhs)
{
    m_high  =   rhs.m_high;
    m_low   =   rhs.m_low;

    return *this;
}

inline sinteger64::class_type &sinteger64::operator =(ss_sint8_t i)
{
    m_high  =   0;
    m_low   =   i;

    return *this;
}

inline sinteger64::class_type &sinteger64::operator =(ss_sint16_t i)
{
    m_high  =   0;
    m_low   =   i;

    return *this;
}

inline sinteger64::class_type &sinteger64::operator =(ss_sint32_t i)
{
    m_high  =   0;
    m_low   =   i;

    return *this;
}

#ifdef __STLSOFT_CF_INT_DISTINCT_TYPE
inline sinteger64::class_type &sinteger64::operator =(int i)
{
    m_high  =   0;
    m_low   =   i;

    return *this;
}
#endif /* __STLSOFT_CF_INT_DISTINCT_TYPE */

inline sinteger64::class_type &sinteger64::operator +=(class_type const &rhs)
{
    m_low   +=  rhs.m_low;
    m_high  +=  rhs.m_high;

    if( rhs.m_low > 0 &&
        m_low < rhs.m_low)
    {
        ++m_high;
    }
    else if(rhs.m_low < 0 &&
            m_low > rhs.m_low)
    {
        --m_high;
    }

    return *this;
}

inline sinteger64::class_type &sinteger64::operator +=(ss_sint8_t i)
{
    return operator +=(class_type(i));
}

inline sinteger64::class_type &sinteger64::operator +=(ss_sint16_t i)
{
    return operator +=(class_type(i));
}

inline sinteger64::class_type &sinteger64::operator +=(ss_sint32_t i)
{
    return operator +=(class_type(i));
}

#ifdef __STLSOFT_CF_INT_DISTINCT_TYPE
inline sinteger64::class_type &sinteger64::operator +=(int i)
{
    return operator +=(class_type(i));
}
#endif /* __STLSOFT_CF_INT_DISTINCT_TYPE */

inline sinteger64::class_type &sinteger64::operator -=(class_type const &rhs)
{
    m_low   -=  rhs.m_low;
    m_high  -=  rhs.m_high;

    if( rhs.m_low > 0 &&
        m_low < rhs.m_low)
    {
        ++m_high;
    }
    else if(rhs.m_low < 0 &&
            m_low > rhs.m_low)
    {
        --m_high;
    }

    return *this;
}

inline sinteger64::class_type &sinteger64::operator -=(ss_sint8_t i)
{
    return operator -=(class_type(i));
}

inline sinteger64::class_type &sinteger64::operator -=(ss_sint16_t i)
{
    return operator -=(class_type(i));
}

inline sinteger64::class_type &sinteger64::operator -=(ss_sint32_t i)
{
    return operator -=(class_type(i));
}

#ifdef __STLSOFT_CF_INT_DISTINCT_TYPE
inline sinteger64::class_type &sinteger64::operator -=(int i)
{
    return operator -=(class_type(i));
}
#endif /* __STLSOFT_CF_INT_DISTINCT_TYPE */

inline sinteger64::class_type &sinteger64::operator *=(class_type const &rhs)
{
    m_low   *=  rhs.m_low;
    m_high  *=  rhs.m_high;

    if( rhs.m_low > 0 &&
        m_low < rhs.m_low)
    {
        ++m_high;
    }
    else if(rhs.m_low < 0 &&
            m_low > rhs.m_low)
    {
        --m_high;
    }

    return *this;
}

inline sinteger64::class_type &sinteger64::operator *=(ss_sint8_t i)
{
    return operator *=(class_type(i));
}

inline sinteger64::class_type &sinteger64::operator *=(ss_sint16_t i)
{
    return operator *=(class_type(i));
}

inline sinteger64::class_type &sinteger64::operator *=(ss_sint32_t i)
{
    return operator *=(class_type(i));
}

#ifdef __STLSOFT_CF_INT_DISTINCT_TYPE
inline sinteger64::class_type &sinteger64::operator *=(int i)
{
    return operator *=(class_type(i));
}
#endif /* __STLSOFT_CF_INT_DISTINCT_TYPE */

inline ss_sint_t sinteger64::compare(class_type const &rhs) const
{
    ss_sint_t   res =   m_high - rhs.m_high;

    if(0 == res)
    {
        res = m_low - rhs.m_low;
    }

    return res;
}

inline ss_sint_t sinteger64::compare(ss_sint8_t i) const
{
    return compare(class_type(0, i));
}

inline ss_sint_t sinteger64::compare(ss_sint16_t i) const
{
    return compare(class_type(0, i));
}

inline ss_sint_t sinteger64::compare(ss_sint32_t i) const
{
    return compare(class_type(0, i));
}

#ifdef __STLSOFT_CF_INT_DISTINCT_TYPE
inline ss_sint_t sinteger64::compare(int i) const
{
    return compare(class_type(0, i));
}
#endif /* __STLSOFT_CF_INT_DISTINCT_TYPE */


/* ////////////////////////////////////////////////////////////////////////// */

#ifndef _STLSOFT_NO_NAMESPACE
} // namespace stlsoft
#endif /* _STLSOFT_NO_NAMESPACE */

/* ////////////////////////////////////////////////////////////////////////// */

#endif /* !STLSOFT_INCL_H_STLSOFT_64BIT_INTEGERS */

/* ////////////////////////////////////////////////////////////////////////// */
