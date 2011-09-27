/* /////////////////////////////////////////////////////////////////////////////
 * File:        mfcstl_array_veneer.h
 *
 * Purpose:     Contains the definition of the array_veneer template.
 *
 * Created:     28th January 2003
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


/// \file mfcstl_array_veneer.h
///
/// Contains the definition of the array_veneer template.

#ifndef MFCSTL_INCL_H_MFCSTL_ARRAY_VENEER
#define MFCSTL_INCL_H_MFCSTL_ARRAY_VENEER

#ifndef __STLSOFT_DOCUMENTATION_SKIP_SECTION
# define MFCSTL_VER_H_MFCSTL_ARRAY_VENEER_MAJOR     2
# define MFCSTL_VER_H_MFCSTL_ARRAY_VENEER_MINOR     0
# define MFCSTL_VER_H_MFCSTL_ARRAY_VENEER_REVISION  1
# define MFCSTL_VER_H_MFCSTL_ARRAY_VENEER_EDIT      27
#endif /* !__STLSOFT_DOCUMENTATION_SKIP_SECTION */

/* /////////////////////////////////////////////////////////////////////////////
 * Includes
 */

#ifndef MFCSTL_INCL_H_MFCSTL
# include "mfcstl.h"    // Include the MFCSTL root header
#endif /* !MFCSTL_INCL_H_MFCSTL */
#ifndef STLSOFT_INCL_H_STLSOFT_ITERATOR
# include "stlsoft_iterator.h"
#endif /* !STLSOFT_INCL_H_STLSOFT_ITERATOR */
#ifndef STLSOFT_INCL_H_STLSOFT_CONSTRAINTS
# include "stlsoft_constraints.h"
#endif /* !STLSOFT_INCL_H_STLSOFT_CONSTRAINTS */

/* /////////////////////////////////////////////////////////////////////////////
 * Namespace
 *
 * The MFCSTL components are contained within the mfcstl namespace. This is
 * actually an alias for stlsoft::mfcstl_project,
 *
 * The definition matrix is as follows:
 *
 * _STLSOFT_NO_NAMESPACE    _MFCSTL_NO_NAMESPACE    mfcstl definition
 * ---------------------    --------------------    -----------------
 *  not defined              not defined             = stlsoft::mfcstl_project
 *  not defined              defined                 not defined
 *  defined                  not defined             mfcstl
 *  defined                  defined                 not defined
 *
 */

#ifndef _MFCSTL_NO_NAMESPACE
# if defined(_STLSOFT_NO_NAMESPACE) || \
     defined(__STLSOFT_DOCUMENTATION_SKIP_SECTION)
/* There is no stlsoft namespace, so must define ::mfcstl */
namespace mfcstl
{
# else
/* Define stlsoft::mfcstl_project */

namespace stlsoft
{

namespace mfcstl_project
{

# endif /* _STLSOFT_NO_NAMESPACE */
#endif /* !_MFCSTL_NO_NAMESPACE */

/* ////////////////////////////////////////////////////////////////////////// */

/// \weakgroup libraries STLSoft Libraries
/// \brief The individual libraries

/// \weakgroup libraries_container Container Library
/// \ingroup libraries
/// \brief This library provides facilities for defining and manipulation containers

/// \defgroup mfcstl_container_library Container Library (MFCSTL)
/// \ingroup MFCSTL libraries_container
/// \brief This library provides facilities for defining and manipulation containers for MFC types
/// @{

/* /////////////////////////////////////////////////////////////////////////////
 * Classes
 */

// struct array_veneer_traits
//
// Regrettably, since MFC's template classes do not define any member types,
// it is not possible to generalise the traits, so we must just use
// specialisations. Sigh!

template <class C>
struct array_veneer_traits;

#ifndef __STLSOFT_DOCUMENTATION_SKIP_SECTION
// For CByteArray

STLSOFT_TEMPLATE_SPECIALISATION
struct array_veneer_traits<CByteArray>
{
    typedef BYTE            value_type;
    typedef BYTE            arg_type;
};

// For CDWordArray

STLSOFT_TEMPLATE_SPECIALISATION
struct array_veneer_traits<CDWordArray>
{
    typedef DWORD           value_type;
    typedef DWORD           arg_type;
};

// For CObArray

STLSOFT_TEMPLATE_SPECIALISATION
struct array_veneer_traits<CObArray>
{
    typedef CObject         *value_type;
    typedef CObject         *arg_type;
};

// For CPtrArray

STLSOFT_TEMPLATE_SPECIALISATION
struct array_veneer_traits<CPtrArray>
{
    typedef void            *value_type;
    typedef void            *arg_type;
};

// For CStringArray

STLSOFT_TEMPLATE_SPECIALISATION
struct array_veneer_traits<CStringArray>
{
    typedef CString         value_type;
    typedef const CString   &arg_type;
};

// For CUIntArray

STLSOFT_TEMPLATE_SPECIALISATION
struct array_veneer_traits<CUIntArray>
{
    typedef UINT            value_type;
    typedef UINT            arg_type;
};

// For CWordArray

STLSOFT_TEMPLATE_SPECIALISATION
struct array_veneer_traits<CWordArray>
{
    typedef WORD            value_type;
    typedef WORD            arg_type;
};


// For CArray<, >

#ifdef __AFXTEMPL_H__
# ifdef __STLSOFT_CF_TEMPLATE_PARTIAL_SPECIALISATION_SUPPORT
/* If your translator supports partial template specialisation, then
 * you should be fine with the following specialisation ...
 */

template <class V, class A>
struct array_veneer_traits<CArray<V, A> >
{
    typedef V               value_type;
    typedef A               arg_type;
};

# else

/* ... otherwise you will need to provide your own traits class, e.g
 *
 *  struct my_traits_type
 *  {
 *    typedef MyValType       value_type;
 *    typedef MyValType const &arg_type;
 *  };
 */

# endif // __STLSOFT_CF_TEMPLATE_PARTIAL_SPECIALISATION_SUPPORT
#endif // __AFXTEMPL_H__
#endif /* !__STLSOFT_DOCUMENTATION_SKIP_SECTION */

/// \brief Adapts MFC Array classes to the STL container concept
///
/// This class takes a container type, e.g. CStringArray, as its primary (and
/// only non-default) template argument, C. The second template parameter, T,
/// defaults to array_veneer_traits<C>, for which specialisations are provided
/// for the following types
///
///  - CObArray      (stores pointers to CObject, or derived, instances)
///  - CPtrArray     (stores raw pointers, ie. void*)
///  - CStringArray  (stores strings, in the form of CString)
///
/// and for compilers that support partial specialisation
///
///  - CArray<V, A>  (stores instances of V, with argument type A)
///
/// \param C The container type
/// \param T The traits type. With translators that support default template arguments, this is defaulted to array_veneer_traits<C>

template<   class C                             // The container type
        ,   class T = array_veneer_traits<C>    // The traits type
        >
class array_veneer
    : public C
{
public:
    /// The type of the instantiated template
    typedef array_veneer<C, T>                          class_type;
    /// The type of the adapted container
    typedef C                                           container_type;
private:
    typedef T                                           traits_type;
    typedef class_type                                  adapted_container_type;
public:
    // If you get a warning about a array_veneer_traits<CArray<,> > parameterisation not
    // having a value_type on the next line, you need to include afxtempl.h _before_
    // this header file
    typedef ss_typename_type_k traits_type::value_type  value_type;
    /// The size type
    typedef ms_size_t                                   size_type;
    /// The difference type
    typedef ms_ptrdiff_t                                difference_type;

public:
    /// Non-mutating (const) iterator for the list_veneer class
    ///
    /// \note This supports the Random-Access Iterator concept
    class const_iterator
        : public stlsoft_ns_qual(iterator_base)<mfcstl_ns_qual_std(random_access_iterator_tag), value_type, ms_ptrdiff_t, void, value_type>
    {
    protected:
        friend class array_veneer<C, T>;

        typedef const_iterator                                      class_type;
        // NOTE: If you get a compiler error on the next line, referring to
        // undefined 'value_type' then you need to provide a traits type
        // with the member type 'value_type' defined.
    public:
        typedef int                                                 index_type;
#ifdef __STLSOFT_CF_TEMPLATE_PARTIAL_SPECIALISATION_SUPPORT
        typedef ss_typename_type_k array_veneer<C, T>::value_type   value_type;
#else
#endif /* !__STLSOFT_CF_TEMPLATE_PARTIAL_SPECIALISATION_SUPPORT */

    // Construction
    protected:
        const_iterator(container_type const *const c, index_type index)
            : m_c(c)
            , m_index(index)
        {}
    public:
        /// Default constructor
        const_iterator()
            : m_c(0)
            , m_index(0)
        {}
        /// Copy constructor
        ///
        /// \param rhs The instance from which to copy construct
        const_iterator(class_type const &rhs)
            : m_c(rhs.m_c)
            , m_index(rhs.m_index)
        {}

        /// Copy assignment operator
        ///
        /// \param rhs The instance from which to copy assign
        const_iterator const &operator =(class_type const &rhs)
        {
            mfcstl_message_assert("Attempting to assign iterator from another container!", m_c == NULL || rhs.m_c == NULL || m_c == rhs.m_c);

            m_c     =   rhs.m_c;
            m_index =   rhs.m_index;

            return *this;
        }

    // Operators
    public:
        ss_typename_type_k traits_type::value_type operator *() const
        {
            mfcstl_message_assert("", m_c != 0);

            return m_c->GetAt(m_index);
        }

        /// Pre-increment operator
        class_type &operator ++()
        {
            ++m_index;

            return *this;
        }

        /// Post-increment operator
        class_type operator ++(int)
        {
            class_type  ret(*this);

            operator ++();

            return ret;
        }

        /// Compares \c this with \c rhs
        ///
        /// \param rhs The instance against which to test
        /// \retval 0 if the two are equal
        /// \retval <0 if \c this is before \c rhs in the sequence
        /// \retval >0 if \c this is after \c rhs in the sequence
        difference_type compare(class_type const &rhs) const
        {
            // Because the C<Type><Container> containers, e.g. CStringArray
            // work on the basis of get-and-advance, m_pos alone cannot be
            // the sentinel for an ended sequence. Hence, combining the
            // implementation of op++ to set m_c to 0 when m_pos is NULL, we
            // can test both members, which results in the after-the-fact
            // equality evaluating correctly.

            mfcstl_message_assert("invalid comparison between iterators from different ranges", m_c == 0 || rhs.m_c == 0 || m_c == rhs.m_c);

            return m_index - rhs.m_index;
        }

        /// Evaluates whether \c this and \c rhs are equivalent
        ///
        /// \param rhs The instance against which to compare
        /// \retval true If \c this and \c rhs are equivalent
        /// \retval false If \c this and \c rhs are not equivalent
        ms_bool_t operator ==(class_type const &rhs) const
        {
            return compare(rhs) == 0;
        }

        /// Evaluates whether \c this and \c rhs are not equivalent
        ///
        /// \param rhs The instance against which to compare
        /// \retval true If \c this and \c rhs are not equivalent
        /// \retval false If \c this and \c rhs are equivalent
        ms_bool_t operator !=(class_type const &rhs) const
        {
            return compare(rhs) != 0;
        }

        // Bidirectional iterator operations

        /// Pre-decrement operator
        class_type &operator --()
        {
            --m_index;

            return *this;
        }

        /// Post-decrement operator
        class_type operator --(int)
        {
            class_type  ret(*this);

            operator --();

            return ret;
        }

        // Random access operations

        class_type &operator +=(difference_type index)
        {
            m_index += index;

            return *this;
        }

        class_type &operator -=(difference_type index)
        {
            m_index -= index;

            return *this;
        }

        value_type operator [](difference_type index) const
        {
            return m_c->GetAt(m_index + index);
        }

        difference_type distance(class_type const &rhs) const
        {
            return m_index - rhs.m_index;
        }

        class_type operator -(difference_type n)
        {
            return class_type(*this) -= n;
        }

        class_type operator +(difference_type n)
        {
            return class_type(*this) += n;
        }

        difference_type operator -(class_type const &rhs)
        {
            return distance(rhs);
        }

        ms_bool_t operator <(class_type const &rhs)
        {
            return compare(rhs) < 0;
        }

        ms_bool_t operator >(class_type const &rhs)
        {
            return compare(rhs) > 0;
        }

        ms_bool_t operator <=(class_type const &rhs)
        {
            return compare(rhs) <= 0;
        }

        ms_bool_t operator >=(class_type const &rhs)
        {
            return compare(rhs) >= 0;
        }


    // Members
    protected:
        container_type const    *m_c;
        size_type               m_index;
    };

    /// Iterator class for the list_veneer class
    class iterator
        : public const_iterator
    {
        friend class array_veneer<C, T>;

    private:
        typedef const_iterator                                      parent_class_type;
        typedef iterator                                            class_type;
        typedef ss_typename_type_k parent_class_type::index_type    index_type;

    private:
        iterator(container_type *const c, index_type index)
            : parent_class_type(c, index)
        {}
    public:
        /// Default constructor
        iterator()
        {}
        /// Copy constructor
        ///
        /// \param rhs The instance from which to copy construct
        iterator(class_type const &rhs)
            : parent_class_type(rhs)
        {}

        /// Copy assignment operator
        ///
        /// \param rhs The instance from which to copy assign
        iterator const &operator =(class_type const &rhs)
        {
            mfcstl_message_assert("Attempting to assign iterator from another container!", m_c == NULL || rhs.m_c == NULL || m_c == rhs.m_c);

            parent_class_type::operator =(rhs);

            return *this;
        }

        // Random access operations

        value_type &operator [](difference_type index)
        {
            return const_cast<container_type *>(m_c)->ElementAt(m_index + index);
        }

        value_type operator [](difference_type index) const
        {
            return parent_class_type::operator [](index);
        }
    };

/// The reverse non-mutating (const) iterator type
#if defined(__STLSOFT_CF_BIDIRECTIONAL_ITERATOR_SUPPORT)
    typedef stlsoft_ns_qual(const_reverse_bidirectional_iterator_base)< const_iterator,
                                                                        value_type,
                                                                        value_type, // Return by value!
                                                                        void*,
                                                                        difference_type>    const_reverse_iterator;
#endif /* __STLSOFT_CF_BIDIRECTIONAL_ITERATOR_SUPPORT */


// Construction
public:
    /// Constructs an instance of the array adaptor
    ss_explicit_k array_veneer()
    {
        stlsoft_constraint_must_be_same_size(C, class_type);
    }

// Operations
public:
    void push_back(value_type const &v)
    {
        Add(v);
    }

// State
public:
    /// Returns the number of elements in the sequence
    size_type size() const
    {
        return static_cast<size_type>(GetSize());
    }
    /// Indicates whether the search sequence is empty
    ms_bool_t empty() const
    {
        return GetSize() == 0;
    }
    /// Returns the maximum number of elements in the sequence
    static size_type max_size()
    {
        return static_cast<size_type>(-1) / sizeof(value_type);
    }

// Iteration
public:
    /// Begins the iteration
    ///
    /// \return An iterator representing the start of the sequence
    const_iterator begin() const
    {
        return const_iterator(this, 0);
    }
    /// Ends the iteration
    ///
    /// \return An iterator representing the end of the sequence
    const_iterator end() const
    {
        return const_iterator(this, size());
    }

    /// Begins the iteration
    ///
    /// \return An iterator representing the start of the sequence
    iterator begin()
    {
        return iterator(this, 0);
    }
    /// Ends the iteration
    ///
    /// \return An iterator representing the end of the sequence
    iterator end()
    {
        return iterator(this, size());
    }

//  const_reverse_iterator
#if defined(__STLSOFT_CF_BIDIRECTIONAL_ITERATOR_SUPPORT)
    /// Begins the reverse iteration
    ///
    /// \return An iterator representing the start of the reverse sequence
    const_reverse_iterator  rbegin() const
    {
        return const_reverse_iterator(end());
    }
    /// Ends the reverse iteration
    ///
    /// \return An iterator representing the end of the reverse sequence
    const_reverse_iterator  rend() const
    {
        return const_reverse_iterator(begin());
    }
#endif /* __STLSOFT_CF_BIDIRECTIONAL_ITERATOR_SUPPORT */

// Accessors
public:
    /// Returns a mutable reference to the element at the given \c index
    value_type &operator [](difference_type index)
    {
        return container_type::operator [](index);
    }

    /// Returns a copy of the element at the given \c index
    value_type operator [](difference_type index) const
    {
        return GetAt(index);
    }
};

/* ////////////////////////////////////////////////////////////////////////// */

/// @} // end of group mfcstl_container_library

/* ////////////////////////////////////////////////////////////////////////// */

#ifndef _MFCSTL_NO_NAMESPACE
# if defined(_STLSOFT_NO_NAMESPACE) || \
     defined(__STLSOFT_DOCUMENTATION_SKIP_SECTION)
} // namespace mfcstl
# else
} // namespace mfcstl_project
} // namespace stlsoft
# endif /* _STLSOFT_NO_NAMESPACE */
#endif /* !_MFCSTL_NO_NAMESPACE */

/* ////////////////////////////////////////////////////////////////////////// */

#endif /* !MFCSTL_INCL_H_MFCSTL_ARRAY_VENEER */

/* ////////////////////////////////////////////////////////////////////////// */
