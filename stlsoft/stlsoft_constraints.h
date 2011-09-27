/* /////////////////////////////////////////////////////////////////////////////
 * File:        stlsoft_constraints.h (originally MTAlgo.h, ::SynesisStl)
 *
 * Purpose:     Compile-time template constraints templates.
 *
 * Created:     19th November 1998
 * Updated:     11th September 2004
 *
 * Home:        http://stlsoft.org/
 *
 * Copyright (c) 1998-2004, Matthew Wilson and Synesis Software
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


/// \file stlsoft_constraints.h
///
/// Compile-time template constraints templates.

#ifndef STLSOFT_INCL_H_STLSOFT_CONSTRAINTS
#define STLSOFT_INCL_H_STLSOFT_CONSTRAINTS

#ifndef __STLSOFT_DOCUMENTATION_SKIP_SECTION
# define STLSOFT_VER_H_STLSOFT_CONSTRAINTS_MAJOR    3
# define STLSOFT_VER_H_STLSOFT_CONSTRAINTS_MINOR    0
# define STLSOFT_VER_H_STLSOFT_CONSTRAINTS_REVISION 1
# define STLSOFT_VER_H_STLSOFT_CONSTRAINTS_EDIT     65
#endif /* !__STLSOFT_DOCUMENTATION_SKIP_SECTION */

/* /////////////////////////////////////////////////////////////////////////////
 * Includes
 */

#ifndef STLSOFT_INCL_H_STLSOFT
# include "stlsoft.h"       // Include the STLSoft root header
#endif /* !STLSOFT_INCL_H_STLSOFT */
#ifndef STLSOFT_INCL_H_STLSOFT_META
# include "stlsoft_meta.h"  // is_***
#endif /* !STLSOFT_INCL_H_STLSOFT_META */

/* /////////////////////////////////////////////////////////////////////////////
 * Namespace
 */

#ifndef _STLSOFT_NO_NAMESPACE
namespace stlsoft
{
#endif /* _STLSOFT_NO_NAMESPACE */

/* /////////////////////////////////////////////////////////////////////////////
 * Macros
 */

#if defined(__STLSOFT_COMPILER_IS_BORLAND) || \
    defined(__STLSOFT_COMPILER_IS_INTEL) || \
    defined(__STLSOFT_COMPILER_IS_MWERKS)
# define stlsoft_constraint_must_be_pod(T)              do { stlsoft_ns_qual(must_be_pod)<T>::func_ptr_type const pfn = stlsoft_ns_qual(must_be_pod)<T>::constraint(); STLSOFT_SUPPRESS_UNUSED(pfn); } while(0)
# define stlsoft_constraint_must_be_pod_or_void(T)      do { stlsoft_ns_qual(must_be_pod_or_void)<T>::func_ptr_type const pfn = stlsoft_ns_qual(must_be_pod_or_void)<T>::constraint(); STLSOFT_SUPPRESS_UNUSED(pfn); } while(0)
#elif defined(__STLSOFT_COMPILER_IS_DMC)
# define stlsoft_constraint_must_be_pod(T)          do { int i = sizeof(stlsoft_ns_qual(must_be_pod)<T>::constraint()); } while(0)
# define stlsoft_constraint_must_be_pod_or_void(T)  do { int i = sizeof(stlsoft_ns_qual(must_be_pod_or_void)<T>::constraint()); } while(0)
#else
# define stlsoft_constraint_must_be_pod(T)          stlsoft_static_assert(sizeof(stlsoft_ns_qual(must_be_pod)<T>::constraint()) != 0)
# define stlsoft_constraint_must_be_pod_or_void(T)  stlsoft_static_assert(sizeof(stlsoft_ns_qual(must_be_pod_or_void)<T>::constraint()) != 0)
#endif /* __STLSOFT_COMPILER_IS_DMC */

# define stlsoft_constraint_must_be_same_size(T1, T2)   stlsoft_ns_qual(must_be_same_size)<T1, T2>()
# define stlsoft_constraint_must_be_subscriptable(T)    stlsoft_ns_qual(must_be_subscriptable)<T>()
# define stlsoft_constraint_must_have_base(D, B)        stlsoft_ns_qual(must_have_base)<D, B>()

/* /////////////////////////////////////////////////////////////////////////////
 * Constraints
 */

// must_have_base
//
/// Constraint to ensure that the one type is convertible to another via inheritance
///
/// \param D The derived type
/// \param B The base type
///
/// \note This is borrowed from
template<   ss_typename_param_k D
        ,   ss_typename_param_k B
        >
struct must_have_base
{
public:
    ~must_have_base()
    {
        void(*p)(D*, B*) = constraints;

        STLSOFT_SUPPRESS_UNUSED(p);
    }

private:
    static void constraints(D* pd, B* pb)
    {
        pb = pd;

        STLSOFT_SUPPRESS_UNUSED(pb);
    }
};

// must_be_same_size
//
/// \param T1 The parameterising type of the veneer
/// \param T2 The veneer type
template<   ss_typename_param_k T1
        ,   ss_typename_param_k T2
        >
struct must_be_same_size
{
    ~must_be_same_size()
    {
        void    (*pfn)(void) = constraints;

        STLSOFT_SUPPRESS_UNUSED(pfn);
    }
private:
    static void constraints()
    {
        // The compiler will bring you here if T1 and T2 are not the same
        // size.
        struct must_be_same_size_
        {
            int T1_must_be_same_size_as_T2 : (static_cast<int>(size_of<T1>::value) == static_cast<int>(size_of<T2>::value));
        };

        const int   T1_must_be_same_size_as_T2    = (static_cast<int>(size_of<T1>::value) == static_cast<int>(size_of<T2>::value));
        int         i[T1_must_be_same_size_as_T2];

        STLSOFT_SUPPRESS_UNUSED(i);
    }
};


/// Constraint to enforce that a given type is an array, or pointer, or user defined type
/// which is amenable to subsripting (i.e. defines <code>operator[]</code> or <code>operator X*()</code>)
template <ss_typename_param_k T>
struct must_be_subscriptable
{
public:
    ~must_be_subscriptable()
    {
        int (*pfn)(T const &) = constraints;

        STLSOFT_SUPPRESS_UNUSED(pfn);
    }
private:
    static int constraints(T const &T_is_not_subscriptable)
    {
        // The compiler will bring you here if T is not subscriptable
        return sizeof(T_is_not_subscriptable[0]);
    }
};

/// Constraint to enforce that a given type is an actual array or pointer, rather than
/// a user-defined type with
template <ss_typename_param_k T>
struct must_subscript_as_decayable_pointer
{
public:
    ~must_subscript_as_decayable_pointer()
    {
        ss_size_t   (*pfn)(T const &) = constraints;

        STLSOFT_SUPPRESS_UNUSED(pfn);
    }
private:
    static ss_size_t constraints(T const &T_is_not_decay_subscriptable)
    {
        // The compiler will bring you here if T has a user-defined
        // subscript operator.
        return sizeof(0[T_is_not_decay_subscriptable]);
    }
};



// must_be_pod
//
/// Constraint to ensure that a type is a built-in or trivial type.
///
/// \param T The type
// This class can be used to constrain a type to be of either built-in, e.g.
// int, or of a trivial type, i.e. aggregate types or types with publicly
// accessible default contructors and assignment operators
template <ss_typename_param_k T>
union must_be_pod
{
private:
    typedef must_be_pod     class_type;

public:
    T   t;
    int i;

    typedef int (*func_ptr_type)();

    static func_ptr_type constraint()
    {
        return constraints;
    }

    // Required by CodeWarrior
    ~must_be_pod()
    {
        int   (*pfn)(void) = constraints;

        STLSOFT_SUPPRESS_UNUSED(pfn);
    }

private:
    static int constraints()
    {
#if defined(__STLSOFT_COMPILER_IS_MWERKS)
# if ((__MWERKS__ & 0xFF00) < 0x3000)
        class_type  u;
# else /* ? __MWERKS__ */
        class_type  u;

        u = *static_cast<class_type*>(0);
# endif /* __MWERKS__ */
#else /* ? compiler */
        class_type  u = class_type();
#endif /* compiler */

        STLSOFT_SUPPRESS_UNUSED(u);

        return sizeof(u);
    }
};


// must_be_pod_or_void
//
/// Constraint to ensure that a type is a built-in or trivial type, or is void.
///
/// \param T The type
// This class can be used to constrain a type to be of either built-in, e.g.
// int, or of a trivial type, i.e. aggregate types or types with publicly
// accessible default contructors and assignment operators
template <ss_typename_param_k T>
union must_be_pod_or_void
{
    T   t;
    int i;

    typedef int (*func_ptr_type)();

    static func_ptr_type constraint()
    {
        return constraints;
    }

#if defined(__STLSOFT_COMPILER_IS_GCC)
protected:
#else /* ? __STLSOFT_COMPILER_IS_GCC */
private:
#endif /* __STLSOFT_COMPILER_IS_GCC */
    static int constraints()
    {
        must_be_pod_or_void<T>  u;

        STLSOFT_SUPPRESS_UNUSED(u);

        return sizeof(u);
    }
};

#ifndef __STLSOFT_DOCUMENTATION_SKIP_SECTION

STLSOFT_TEMPLATE_SPECIALISATION
union must_be_pod_or_void<void>
{
    typedef int (*func_ptr_type)();

    static func_ptr_type constraint()
    {
        return NULL;
    }
};

#endif /* !__STLSOFT_DOCUMENTATION_SKIP_SECTION */

/* ////////////////////////////////////////////////////////////////////////// */

#if 0

/// This type is used as a tag parent to hide implicit comparison of types that
/// provide implicit converion operators.

#ifdef NIC_TEMPLATE_VERSION
//struct nic_null_
//{};

typedef void    nic_null_;

template <ss_typename_param_k T = nic_null_>
struct not_implicitly_comparable
    : public T
{
public:
    typedef not_implicitly_comparable<T>    class_type;

#if defined(__STLSOFT_COMPILER_IS_GCC)
protected:
#else /* ? __STLSOFT_COMPILER_IS_GCC */
private:
#endif /* __STLSOFT_COMPILER_IS_GCC */
#if 0
    template <ss_typename_param_k T2>
    ss_bool_t operator ==(T2 const &) const;
    template <ss_typename_param_k T2>
    ss_bool_t operator !=(T2 const &) const;
#endif /* 0 */
};

/// This specialisation allows it to
template <>
struct not_implicitly_comparable<nic_null_>
{
public:
    typedef not_implicitly_comparable<nic_null_>    class_type;

#if defined(__STLSOFT_COMPILER_IS_GCC)
protected:
#else /* ? __STLSOFT_COMPILER_IS_GCC */
private:
#endif /* __STLSOFT_COMPILER_IS_GCC */
#if 0
    template <ss_typename_param_k T2>
    ss_bool_t operator ==(T2 const &) const;
    template <ss_typename_param_k T2>
    ss_bool_t operator !=(T2 const &) const;
#endif /* 0 */
};

template<   ss_typename_param_k T
        ,   ss_typename_param_k T2
        >
inline ss_bool_t operator ==(not_implicitly_comparable<T> const &lhs, T2 const &rhs)
{
//  return lhs.operator ==(rhs);

  lhs.this_type_does_not_support_comparisons();

  return false; // Placate the eager beavers
}

template<   ss_typename_param_k T
        ,   ss_typename_param_k T2
        >
inline ss_bool_t operator ==(T2 const &lhs, not_implicitly_comparable<T> const &rhs)
{
//  return rhs.operator ==(lhs);

  rhs.this_type_does_not_support_comparisons();

  return false; // Placate the eager beavers
}

#if 0
template<   ss_typename_param_k T1
        ,   ss_typename_param_k T2
        >
inline ss_bool_t operator ==(not_implicitly_comparable<T1> const &lhs, not_implicitly_comparable<T2> const &rhs)
{
//  return rhs.operator ==(lhs);

  rhs.this_type_does_not_support_comparisons();

  return false; // Placate the eager beavers
}
#endif /* 0 */

template<   ss_typename_param_k T
        ,   ss_typename_param_k T2
        >
inline ss_bool_t operator !=(T2 const &lhs, not_implicitly_comparable<T> const &rhs)
{
//  return rhs.operator !=(lhs);

  rhs.this_type_does_not_support_comparisons();

  return false; // Placate the eager beavers
}

template<   ss_typename_param_k T
        ,   ss_typename_param_k T2
        >
inline ss_bool_t operator !=(not_implicitly_comparable<T> const &lhs, T2 const &rhs)
{
//  return lhs.operator !=(rhs);

  lhs.this_type_does_not_support_comparisons();

  return false; // Placate the eager beavers
}

template<   ss_typename_param_k T1
        ,   ss_typename_param_k T2
        >
inline ss_bool_t operator !=(not_implicitly_comparable<T1> const &lhs, not_implicitly_comparable<T2> const &rhs)
{
//  return lhs.operator !=(rhs);

  lhs.this_type_does_not_support_comparisons();

  return false; // Placate the eager beavers
}

#else /* ? 0 */

struct not_implicitly_comparable
{
public:
    typedef not_implicitly_comparable   class_type;

#if defined(__STLSOFT_COMPILER_IS_GCC)
protected:
#else /* ? __STLSOFT_COMPILER_IS_GCC */
private:
#endif /* __STLSOFT_COMPILER_IS_GCC */

#ifndef NIC_EXTERNAL_OPERATORS
    ss_bool_t operator ==(class_type const &) const;
    ss_bool_t operator !=(class_type const &) const;

    template <ss_typename_param_k T2>
    ss_bool_t operator ==(T2 const &) const;
    template <ss_typename_param_k T2>
    ss_bool_t operator !=(T2 const &) const;
#endif /* !NIC_EXTERNAL_OPERATORS */
};


#ifdef NIC_EXTERNAL_OPERATORS

template<   ss_typename_param_k T2
        >
inline ss_bool_t operator ==(not_implicitly_comparable const &lhs, T2 const &rhs)
{
//  return lhs.operator ==(rhs);

  lhs.this_type_does_not_support_comparisons();

  return false; // Placate the eager beavers
}

template<   ss_typename_param_k T2
        >
inline ss_bool_t operator ==(T2 const &lhs, not_implicitly_comparable const &rhs)
{
//  return rhs.operator ==(lhs);

  rhs.this_type_does_not_support_comparisons();

  return false; // Placate the eager beavers
}
#endif /* NIC_EXTERNAL_OPERATORS */

#endif /* 0 */
#endif /* 0 */

/* ////////////////////////////////////////////////////////////////////////// */

#ifndef _STLSOFT_NO_NAMESPACE
} // namespace stlsoft
#endif /* _STLSOFT_NO_NAMESPACE */

/* ////////////////////////////////////////////////////////////////////////// */

#endif /* !STLSOFT_INCL_H_STLSOFT_CONSTRAINTS */

/* ////////////////////////////////////////////////////////////////////////// */
