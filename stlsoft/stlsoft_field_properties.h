/* ////////////////////////////////////////////////////////////////////////////
 * File:        stlsoft_field_properties.h
 *
 * Purpose:     Field-based properties.
 *
 * Created:     6th October 2003
 * Updated:     11th September 2004
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


/// \file stlsoft_field_properties.h
///
/// Field-based properties.

#ifndef STLSOFT_INCL_H_STLSOFT_FIELD_PROPERTIES
#define STLSOFT_INCL_H_STLSOFT_FIELD_PROPERTIES

#ifndef __STLSOFT_DOCUMENTATION_SKIP_SECTION
# define STLSOFT_VER_H_STLSOFT_FIELD_PROPERTIES_MAJOR       2
# define STLSOFT_VER_H_STLSOFT_FIELD_PROPERTIES_MINOR       0
# define STLSOFT_VER_H_STLSOFT_FIELD_PROPERTIES_REVISION    1
# define STLSOFT_VER_H_STLSOFT_FIELD_PROPERTIES_EDIT        10
#endif /* !__STLSOFT_DOCUMENTATION_SKIP_SECTION */

/* ////////////////////////////////////////////////////////////////////////////
 * Includes
 */

#ifndef STLSOFT_INCL_H_STLSOFT
# include "stlsoft.h"   // Include the STLSoft root header
#endif /* !STLSOFT_INCL_H_STLSOFT */

/* ////////////////////////////////////////////////////////////////////////////
 * Namespace
 */

#ifndef _STLSOFT_NO_NAMESPACE
namespace stlsoft
{
#endif /* _STLSOFT_NO_NAMESPACE */

/* ////////////////////////////////////////////////////////////////////////// */

/// \weakgroup libraries STLSoft Libraries
/// \brief The individual libraries

/// \weakgroup libraries_properties Properties Library
/// \ingroup libraries
/// \brief This library provides portable <code><b>Properties</b></code> templates

/// \weakgroup field_properties_library Field Properties
/// \ingroup STLSoft libraries_properties
/// \brief This library provides portable <code><b>Field Properties</b></code> templates
/// @{

/* ////////////////////////////////////////////////////////////////////////////
 * Classes
 */

/** \weakgroup internal_field_properties_library Field Properties
 ** \ingroup STLSoft field_properties_library libraries_properties
 ** \brief These field properties are backed up by internal member variables
 ** @{
 */

template<   ss_typename_param_k V   /* The actual property value type */
        ,   ss_typename_param_k R   /* The reference type */
        ,   ss_typename_param_k C   /* The enclosing class */
        >
class field_property_get
{
public:
    typedef V                           value_type;
    typedef R                           reference_type;
    typedef C                           container_type;
    typedef field_property_get<V, R, C> class_type;

# if defined(__STLSOFT_COMPILER_IS_DMC)
public:
# else
private:
# endif /* __STLSOFT_COMPILER_IS_DMC */
    field_property_get()
    {}
private:
    ss_explicit_k field_property_get(reference_type value)
        : m_value(value)
    {}

    class_type &operator =(reference_type value)
    {
        m_value = value;

        return *this;
    }

    STLSOFT_DECLARE_TEMPLATE_PARAM_AS_FRIEND(C);

public:
    /// Provides read-only access to the property
    operator reference_type () const
    {
        return m_value;
    }

// Members
private:
    value_type  m_value;

// Not to be implemented
private:
    field_property_get(class_type const &);
    class_type &operator =(class_type const &);
};


template<   ss_typename_param_k V   /* The actual property value type */
        ,   ss_typename_param_k R   /* The reference type */
        ,   ss_typename_param_k C   /* The enclosing class */
        >
class field_property_set
{
public:
    typedef V                           value_type;
    typedef R                           reference_type;
    typedef C                           container_type;
    typedef field_property_set<V, R, C> class_type;

# if defined(__STLSOFT_COMPILER_IS_DMC)
public:
# else
private:
# endif /* __STLSOFT_COMPILER_IS_DMC */
    field_property_set()
    {}
private:
    ss_explicit_k field_property_set(reference_type value)
        : m_value(value)
    {}

    operator reference_type () const
    {
        return m_value;
    }

    STLSOFT_DECLARE_TEMPLATE_PARAM_AS_FRIEND(C);

public:
    /// Provides write-only access to the property
    class_type &operator =(reference_type value)
    {
        m_value = value;

        return *this;
    }

private:
    value_type  m_value;

// Not to be implemented
private:
    field_property_set(class_type const &);
    class_type &operator =(class_type const &);
};

/** @}
 */



/** \weakgroup external_field_properties_library Field Properties
 ** \ingroup STLSoft field_properties_library libraries_properties
 ** \brief These field properties are backed up by external member variables
 ** @{
 */

#if 1
template<   ss_typename_param_k V   /* The actual property value type */
        ,   ss_typename_param_k R   /* The reference type */
        >
class field_property_get_external
{
public:
    typedef V                                   value_type;
    typedef R                                   reference_type;
    typedef field_property_get_external<V, R>   class_type;

public:
    field_property_get_external(value_type &value)
        : m_value(value)
    {}

// Accessors
public:
    /// Provides read-only access to the property
    operator reference_type() const
    {
        return m_value;
    }

// Members
private:
    value_type  &m_value;
};


template<   ss_typename_param_k V   /* The actual property value type */
        ,   ss_typename_param_k R   /* The reference type */
        >
class field_property_set_external
{
public:
    typedef V                                   value_type;
    typedef R                                   reference_type;
    typedef field_property_get_external<V, R>   class_type;

public:
    field_property_set_external(value_type &value)
        : m_value(value)
    {}

// Accessors
public:
    /// Provides write-only access to the property
    field_property_set_external &operator =(reference_type value)
    {
        m_value = value;

        return *this;
    }

// Members
private:
    value_type  &m_value;
};
#else

template<   ss_typename_param_k V   /* The actual property value type */
        ,   ss_typename_param_k R   /* The reference type */
        ,   ptrdiff_t   (*PFnOff)() /* Pointer to function providing offset from value to property */
        >
class field_property_get_external
{
public:
    typedef V                                           value_type;
    typedef R                                           reference_type;
    typedef field_property_get_external<V, R, PFnOff>   class_type;

// Accessors
public:
    /// Provides read-only access to the property
    operator reference_type() const
    {
        ptrdiff_t   offset  =   (*PFnOff)();
        value_type  *pV     =   (value_type*)((ss_byte_t*)this - offset);

        return *pV;
    }
};


template<   ss_typename_param_k V   /* The actual property value type */
        ,   ss_typename_param_k R   /* The reference type */
        ,   ptrdiff_t   (*PFnOff)() /* Pointer to function providing offset from value to property */
        >
class field_property_set_external
{
public:
    typedef V                                           value_type;
    typedef R                                           reference_type;
    typedef field_property_get_external<V, R, PFnOff>   class_type;

// Accessors
public:
    /// Provides write-only access to the property
    field_property_set_external &operator =(reference_type value)
    {
        ptrdiff_t   offset  =   (*PFnOff)();
        value_type  *pV     =   (value_type*)((ss_byte_t*)this - offset);

        *pV = value;

        return *this;
    }
};
#endif /* 0 */

/** @}
 */

/* ////////////////////////////////////////////////////////////////////////// */

/// @} // end of group field_properties_library

/* ////////////////////////////////////////////////////////////////////////// */

#ifndef _STLSOFT_NO_NAMESPACE
} // namespace stlsoft
#endif /* _STLSOFT_NO_NAMESPACE */

/* ////////////////////////////////////////////////////////////////////////// */

#endif /* !STLSOFT_INCL_H_STLSOFT_FIELD_PROPERTIES */

/* ////////////////////////////////////////////////////////////////////////// */
