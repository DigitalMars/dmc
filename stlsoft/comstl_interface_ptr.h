/* ////////////////////////////////////////////////////////////////////////////
 * File:        comstl_interface_ptr.h (formerly MLRelItf.h; ::SynesisCom)
 *
 * Purpose:     Interface management helper classes.
 *
 * Created:     2nd November 1994
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


/// \file comstl_interface_ptr.h
///
/// Interface management helper classes.

#ifndef COMSTL_INCL_H_COMSTL_INTERFACE_PTR
#define COMSTL_INCL_H_COMSTL_INTERFACE_PTR

#ifndef __STLSOFT_DOCUMENTATION_SKIP_SECTION
# define COMSTL_VER_H_COMSTL_INTERFACE_PTR_MAJOR        4
# define COMSTL_VER_H_COMSTL_INTERFACE_PTR_MINOR        0
# define COMSTL_VER_H_COMSTL_INTERFACE_PTR_REVISION     1
# define COMSTL_VER_H_COMSTL_INTERFACE_PTR_EDIT         453
#endif /* !__STLSOFT_DOCUMENTATION_SKIP_SECTION */

/* ////////////////////////////////////////////////////////////////////////////
 * Includes
 */

#ifndef COMSTL_INCL_H_COMSTL
# include "comstl.h"                    // Include the COMSTL root header
#endif /* !COMSTL_INCL_H_COMSTL */
#ifndef COMSTL_INCL_H_COMSTL_REFCOUNT_FUNCTIONS
# include "comstl_refcount_functions.h" // addref, release, etc.
#endif /* !COMSTL_INCL_H_COMSTL_REFCOUNT_FUNCTIONS */
#ifndef STLSOFT_INCL_H_STLSOFT_OPERATOR_BOOL
# include "stlsoft_operator_bool.h"     // operator_bool_generator
#endif /* !STLSOFT_INCL_H_STLSOFT_OPERATOR_BOOL */
#include <algorithm>

/* ////////////////////////////////////////////////////////////////////////////
 * Namespace
 *
 * The COMSTL components are contained within the comstl namespace. This is
 * actually an alias for stlsoft::comstl_project,
 *
 * The definition matrix is as follows:
 *
 * _STLSOFT_NO_NAMESPACE    _COMSTL_NO_NAMESPACE    comstl definition
 * ---------------------    --------------------    -----------------
 *  not defined              not defined             = stlsoft::comstl_project
 *  not defined              defined                 not defined
 *  defined                  not defined             comstl
 *  defined                  defined                 not defined
 *
 */

#ifndef _COMSTL_NO_NAMESPACE
# if defined(_STLSOFT_NO_NAMESPACE) || \
     defined(__STLSOFT_DOCUMENTATION_SKIP_SECTION)
/* There is no stlsoft namespace, so must define ::comstl */
namespace comstl
{
# else
/* Define stlsoft::comstl_project */

namespace stlsoft
{

namespace comstl_project
{

# endif /* _STLSOFT_NO_NAMESPACE */
#endif /* !_COMSTL_NO_NAMESPACE */

/* ////////////////////////////////////////////////////////////////////////// */

/// \defgroup comstl_refcount_functions COM Reference Management Functions
/// \ingroup COMSTL
/// \brief These functions assist in the management of object reference counts
/// @{

/* ////////////////////////////////////////////////////////////////////////////
 * Classes
 */

/// Manages a COM interface pointer, ensuring that it is released
template <ss_typename_param_k T>
class interface_ptr
{
/// \name Types
/// @{
public:
    typedef interface_ptr<T>        class_type;

    typedef T                       value_type;
    typedef value_type              *pointer;
    typedef value_type const        *const_pointer;
    typedef value_type              &reference;
    typedef value_type const        &const_reference;
/// @}

/// \name Construction
/// @{
public:
     interface_ptr();
     interface_ptr(pointer p, cs_bool_t bAddRef);
     interface_ptr(reference r, cs_bool_t bAddRef);
     interface_ptr(class_type const &rhs);
    ~interface_ptr() comstl_throw_0();

    class_type &operator =(class_type const &rhs);
/// @}

/// \name Operations
/// @{
public:
    void    set(pointer p, cs_bool_t bAddRef);
    void    set(reference r, cs_bool_t bAddRef);
    void    release();
    void    release(cs_bool_t bDecRef /* = true */);
    pointer detach();
    void    swap(class_type &rhs);
/// @}

/// \name Comparison
/// @{
public:
    cs_bool_t operator ==(class_type const &rhs) const;
    cs_bool_t operator !=(class_type const &rhs) const;
/// @}

/// \name State
/// @{
private:
    STLSOFT_DEFINE_OPERATOR_BOOL_TYPES_T(class_type, operator_bool_generator_type, operator_bool_type);
public:
    operator operator_bool_type() const;

//    cs_bool_t operator !() const;
/// @}

/// \name Accessors
/// @{
public:
    const_pointer   operator ->() const;
    pointer         operator ->();
    const_reference operator *() const;
    reference       operator *();

    const_pointer   get_interface_ptr() const;
    pointer         get_interface_ptr();
/// @}

private:
    pointer m_p;
};

////////////////////////////////////////////////////////////////////////////////
// Shims

template <ss_typename_param_k T>
inline cs_bool_t is_empty(interface_ptr<T> const &p)
{
    return NULL == p.get_interface_ptr();
}

template <ss_typename_param_k T>
inline T const *get_ptr(interface_ptr<T> const &p)
{
    return p.get_interface_ptr();
}

template <ss_typename_param_k T>
inline T *get_ptr(interface_ptr<T> &p)
{
    return p.get_interface_ptr();
}

/* /////////////////////////////////////////////////////////////////////////////
 * Unit-testing
 */

#ifdef STLSOFT_UNITTEST

namespace unittest
{
    ss_bool_t test_comstl_interface_ptr(unittest_reporter *r)
    {
        using stlsoft::unittest::unittest_initialiser;

        ss_bool_t               bSuccess    =   true;

        unittest_initialiser    init(r, "COMSTL", "interface_ptr", __FILE__);

#if 0
        if(<<TODO>>)
        {
            r->report("<<TODO>> failed ", __LINE__);
            bSuccess = false;
        }
#endif /* 0 */

        return bSuccess;
    }

    unittest_registrar    unittest_comstl_interface_ptr(test_comstl_interface_ptr);

} // namespace unittest

#endif /* STLSOFT_UNITTEST */

/* /////////////////////////////////////////////////////////////////////////////
 * Implementation
 */

// interface_ptr

#ifndef __STLSOFT_DOCUMENTATION_SKIP_SECTION

template <ss_typename_param_k T>
inline void interface_ptr<T>::swap(interface_ptr<T> &rhs)
{
    comstl_ns_qual_std(swap)(m_p, rhs.m_p);
}

template <ss_typename_param_k T>
inline interface_ptr<T>::interface_ptr()
    : m_p(NULL)
{}

template <ss_typename_param_k T>
inline interface_ptr<T>::interface_ptr(ss_typename_type_k interface_ptr<T>::pointer p, cs_bool_t bAddRef)
    : m_p(p)
{
    if( bAddRef &&
        NULL != m_p)
    {
        addref(m_p);
    }
}

template <ss_typename_param_k T>
inline interface_ptr<T>::interface_ptr(ss_typename_type_k interface_ptr<T>::reference r, cs_bool_t bAddRef)
    : m_p(&r)
{
    stlsoft_message_assert("Attempting to dereference a null pointer", NULL != m_p);

    if(bAddRef)
    {
        addref(m_p);
    }
}

template <ss_typename_param_k T>
inline interface_ptr<T>::interface_ptr(ss_typename_type_k interface_ptr<T>::class_type const &rhs)
    : m_p(rhs.m_p)
{
    safe_addref(m_p);
}

template <ss_typename_param_k T>
inline void interface_ptr<T>::release()
{
    release_set_null(m_p);
}

template <ss_typename_param_k T>
inline void interface_ptr<T>::release(cs_bool_t bDecRef /* = true */)
{
    if(NULL != m_p)
    {
        if(bDecRef)
        {
            release(m_p);
        }

        m_p = NULL;
    }
}

template <ss_typename_param_k T>
inline interface_ptr<T>::~interface_ptr() comstl_throw_0()
{
    release();
}

template <ss_typename_param_k T>
inline ss_typename_type_k interface_ptr<T>::class_type &interface_ptr<T>::operator =(ss_typename_type_k interface_ptr<T>::class_type const &rhs)
{
    class_type   t(SyCastConst(class_type &, rhs).m_p);

    swap(t);

    return *this;
}

template <ss_typename_param_k T>
inline void interface_ptr<T>::set(ss_typename_type_k interface_ptr<T>::pointer p, cs_bool_t bAddRef)
{
    class_type   t(p, bAddRef);

    swap(t);
}

template <ss_typename_param_k T>
inline void interface_ptr<T>::set(ss_typename_type_k interface_ptr<T>::reference r, cs_bool_t bAddRef)
{
    stlsoft_message_assert("Attempting to dereference a null pointer", &r != NULL);

    class_type   t(p, bAddRef);

    swap(t);
}

template <ss_typename_param_k T>
inline ss_typename_type_k interface_ptr<T>::pointer interface_ptr<T>::detach()
{
    pointer p = m_p;

    m_p = NULL;

    return p;
}

template <ss_typename_param_k T>
inline cs_bool_t interface_ptr<T>::operator ==(ss_typename_type_k interface_ptr<T>::class_type const &rhs) const
{
    return m_p == rhs.m_p;
}

template <ss_typename_param_k T>
inline cs_bool_t interface_ptr<T>::operator !=(ss_typename_type_k interface_ptr<T>::class_type const &rhs) const
{
    return m_p != rhs.m_p;
}

template <ss_typename_param_k T>
inline interface_ptr<T>::operator ss_typename_type_k interface_ptr<T>::operator_bool_type() const
{
    return operator_bool_generator_type::translate(NULL != m_p);
}

template <ss_typename_param_k T>
inline ss_typename_type_k interface_ptr<T>::pointer interface_ptr<T>::get_interface_ptr()
{
    return m_p;
}

template <ss_typename_param_k T>
inline ss_typename_type_k interface_ptr<T>::const_pointer interface_ptr<T>::get_interface_ptr() const
{
    return m_p;
}

#if 0
template <ss_typename_param_k T>
inline cs_bool_t interface_ptr<T>::operator !() const
{
    return NULL == m_p;
}
#endif /* 0 */

template <ss_typename_param_k T>
inline ss_typename_type_k interface_ptr<T>::const_pointer interface_ptr<T>::operator ->() const
{
    stlsoft_message_assert("Attempting to dereference a null pointer", NULL != m_p);

    return m_p;
}

template <ss_typename_param_k T>
inline ss_typename_type_k interface_ptr<T>::pointer interface_ptr<T>::operator ->()
{
    stlsoft_message_assert("Attempting to dereference a null pointer", NULL != m_p);

    return m_p;
}

template <ss_typename_param_k T>
inline ss_typename_type_k interface_ptr<T>::const_reference interface_ptr<T>::operator *() const
{
    stlsoft_message_assert("Attempting to dereference a null pointer", NULL != m_p);

    return *m_p;
}

template <ss_typename_param_k T>
inline ss_typename_type_k interface_ptr<T>::reference interface_ptr<T>::operator *()
{
    stlsoft_message_assert("Attempting to dereference a null pointer", NULL != m_p);

    return *m_p;
}

#endif /* !__STLSOFT_DOCUMENTATION_SKIP_SECTION */

/* ////////////////////////////////////////////////////////////////////////// */

/// @} // end of group comstl_refcount_functions

/* ////////////////////////////////////////////////////////////////////////// */

#ifndef _COMSTL_NO_NAMESPACE
# if defined(_STLSOFT_NO_NAMESPACE) || \
     defined(__STLSOFT_DOCUMENTATION_SKIP_SECTION)
} // namespace comstl
# else
} // namespace comstl_project
} // namespace stlsoft
# endif /* _STLSOFT_NO_NAMESPACE */
#endif /* !_COMSTL_NO_NAMESPACE */

/* ////////////////////////////////////////////////////////////////////////// */

#endif /* !COMSTL_INCL_H_COMSTL_INTERFACE_PTR */

/* ////////////////////////////////////////////////////////////////////////// */
