/* /////////////////////////////////////////////////////////////////////////////
 * File:        mfcstl_list_adaptor.h
 *
 * Purpose:     Contains the definition of the list_adaptor template, and the
 *              specialisations.
 *
 * Created:     1st December 2002
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


/// \file mfcstl_list_adaptor.h
///
/// Contains the definition of the list_adaptor template, and the specialisations.

#ifndef MFCSTL_INCL_H_MFCSTL_LIST_ADAPTOR
#define MFCSTL_INCL_H_MFCSTL_LIST_ADAPTOR

#ifndef __STLSOFT_DOCUMENTATION_SKIP_SECTION
# define MFCSTL_VER_H_MFCSTL_LIST_ADAPTOR_MAJOR     2
# define MFCSTL_VER_H_MFCSTL_LIST_ADAPTOR_MINOR     0
# define MFCSTL_VER_H_MFCSTL_LIST_ADAPTOR_REVISION  1
# define MFCSTL_VER_H_MFCSTL_LIST_ADAPTOR_EDIT      29
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
 * Pre-processor options
 *
 * Because the CObList, CPtrList, CStringList and CList<,> implementations all
 * internally represent their logical position indicators (of type POSTION) as
 * pointers to the nodes within the lists, it is workable to be able to copy
 * these position variables.
 *
 * However, nothing in the MFC documentation stipulates this to be a reliable
 * and documented part of the classes' interfaces, so this is a potentially
 * unsafe assumption.
 *
 * Therefore, the iterator model for the list_adaptor class is Input Iterator.
 * If you wish to use forward iterators, you may specify the pre-processor
 * symbol _MFCSTL_LIST_ADAPTOR_ENABLE_FWD_ITERATOR, in which case the iterator
 * classes will implement copy semantics, rather than the default move
 * semantics.
 */

/* /////////////////////////////////////////////////////////////////////////////
 * Classes
 */

// struct list_adaptor_traits
//
// Regrettably, since MFC's template classes do not define any member types,
// it is not possible to generalise the traits, so we must just use
// specialisations. Sigh!

template <class C>
struct list_adaptor_traits;

#ifndef __STLSOFT_DOCUMENTATION_SKIP_SECTION
// For CObList

STLSOFT_TEMPLATE_SPECIALISATION
struct list_adaptor_traits<CObList>
{
    typedef CObject         *value_type;
    typedef CObject         *arg_type;
};

// For CPtrList

STLSOFT_TEMPLATE_SPECIALISATION
struct list_adaptor_traits<CPtrList>
{
    typedef void            *value_type;
    typedef void            *arg_type;
};

// For CStringList

STLSOFT_TEMPLATE_SPECIALISATION
struct list_adaptor_traits<CStringList>
{
    typedef CString         value_type;
    typedef const CString   &arg_type;
};

// For CList<, >

#ifdef __AFXTEMPL_H__
# ifdef __STLSOFT_CF_TEMPLATE_PARTIAL_SPECIALISATION_SUPPORT
/* If your translator supports partial template specialisation, then
 * you should be fine with the following specialisation ...
 */

template <class V, class A>
struct list_adaptor_traits<CList<V, A> >
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

/// \brief Adapts MFC List class instances to the STL container concept
///
/// This class takes a container type, e.g. CStringList, as its primary (and
/// only non-default) template argument, C. The second template parameter, T,
/// defaults to list_adaptor_traits<C>, for which specialisations are provided
/// for the following types
///
///  - CObList       (stores pointers to CObject, or derived, instances)
///  - CPtrList      (stores raw pointers, ie. void*)
///  - CStringList   (stores strings, in the form of CString)
///
/// and for compilers that support partial specialisation
///
///  - CList<V, A>   (stores instances of V, with argument type A)
///
/// \param C The container type
/// \param T The traits type. With translators that support default template arguments, this is defaulted to list_adaptor_traits<C>

template<   class C                             // The container type
        ,   class T = list_adaptor_traits<C>    // The traits type
        >
class list_adaptor
{
public:
    typedef list_adaptor<C, T>                          class_type;
    typedef class_type                                  adapted_container_type;
private:
    typedef C                                           container_type;
    typedef T                                           traits_type;
public:
    // If you get a warning about a list_adaptor_traits<CList<,> > parameterisation not
    // having a value_type on the next line, you need to include afxtempl.h _before_
    // this header file
    typedef ss_typename_type_k traits_type::value_type  value_type;
    typedef ms_size_t                                   size_type;

public:
    /// Non-mutating (const) iterator for the list_adaptor class
    ///
    /// \note This currently supports the Input Iterator concept only
    class const_iterator
        : public stlsoft_ns_qual(iterator_base)<mfcstl_ns_qual_std(input_iterator_tag), value_type, ms_ptrdiff_t, void, value_type>
    {
        friend class list_adaptor<C, T>;

        typedef const_iterator                                      class_type;
        // NOTE: If you get a compiler error on the next line, referring to
        // undefined 'value_type' then you need to provide a traits type
        // with the member type 'value_type' defined.
# ifdef __STLSOFT_CF_TEMPLATE_PARTIAL_SPECIALISATION_SUPPORT
        typedef ss_typename_type_k list_adaptor<C, T>::value_type   value_type;
# else
# endif /* !__STLSOFT_CF_TEMPLATE_PARTIAL_SPECIALISATION_SUPPORT */
# ifndef _MFCSTL_LIST_ADAPTOR_ENABLE_FWD_ITERATOR
        typedef stlsoft_define_move_rhs_type(class_type)            rhs_type;
# endif /* !_MFCSTL_LIST_ADAPTOR_ENABLE_FWD_ITERATOR */

    // Construction
    private:
        /// Constructor
        const_iterator(container_type *c, POSITION pos)
            : m_c(c)
            , m_pos(pos)
        {
            operator ++();
        }
    public:
        /// Default constructor
        const_iterator()
            : m_c(0)
            , m_pos(NULL)
        {}
# ifdef _MFCSTL_LIST_ADAPTOR_ENABLE_FWD_ITERATOR
        // The copy constructor and copy assignment operator are not defined,
        // which allows the class to support copy-semantics. See the
        // description of _MFCSTL_LIST_ADAPTOR_ENABLE_FWD_ITERATOR given for
        // a discussion of the ramifications of this choice.

# else
        /// <a href = "http://synesis.com.au/resources/articles/cpp/movectors.pdf">Move constructor</a>
        const_iterator(rhs_type rhs)
            : m_c(rhs.m_c)
            , m_pos(rhs.m_pos)
        {
            move_lhs_from_rhs(rhs).m_pos = NULL;
        }

        /// <a href = "http://synesis.com.au/resources/articles/cpp/movectors.pdf">Move assignment</a> operator
        const_iterator const &operator =(rhs_type rhs)
        {
            m_c     =   rhs.m_c;
            m_pos   =   rhs.m_pos;

            move_lhs_from_rhs(rhs).m_pos = NULL;

            return *this;
        }
# endif /* _MFCSTL_LIST_ADAPTOR_ENABLE_FWD_ITERATOR */

    // Operators
    public:
        /// Dereference operator
        value_type operator *() const
        {
            mfcstl_message_assert("", m_c != 0);

            return m_value;
        }

        /// \brief Pre-increment operator
        const_iterator &operator ++()
        {
            if(m_pos == NULL)
            {
                mfcstl_message_assert("operator ++() called on invalid iterator", m_c != 0);

                m_c = 0;
            }
            else
            {
                m_value = m_c->GetNext(m_pos);
            }

            return *this;
        }

        /// \brief Post-increment operator
# ifdef _MFCSTL_LIST_ADAPTOR_ENABLE_FWD_ITERATOR
        const_iterator operator ++(int)
# else
        void operator ++(int)
# endif /* _MFCSTL_LIST_ADAPTOR_ENABLE_FWD_ITERATOR */
        {
# ifdef _MFCSTL_LIST_ADAPTOR_ENABLE_FWD_ITERATOR
            class_type  ret(*this);
# endif /* _MFCSTL_LIST_ADAPTOR_ENABLE_FWD_ITERATOR */

            operator ++();

# ifdef _MFCSTL_LIST_ADAPTOR_ENABLE_FWD_ITERATOR
            return ret;
# endif /* _MFCSTL_LIST_ADAPTOR_ENABLE_FWD_ITERATOR */
        }

        /// Evaluates whether \c this is equivalent to \c rhs
        ///
        /// \param rhs The instance from which to copy construct
        /// \retval true The two iterators refer to the same position in the same container
        /// \retval false The two iterators do not refer to the same position in the same container
        ms_bool_t operator ==(const_iterator const &rhs) const
        {
            // Because the C<Type><Container> containers, e.g. CStringList
            // work on the basis of get-and-advance, m_pos alone cannot be
            // the sentinel for an ended sequence. Hence, combining the
            // implementation of op++ to set m_c to 0 when m_pos is NULL, we
            // can test both members, which results in the after-the-fact
            // equality evaluating correctly.

            mfcstl_message_assert("invalid comparison between iterators from different ranges", m_c == 0 || rhs.m_c == 0 || m_c == rhs.m_c);

            return m_pos == rhs.m_pos && m_c == rhs.m_c;
        }
        /// Evaluates whether \c this is not equivalent to \c rhs
        ///
        /// \param rhs The instance from which to copy construct
        /// \retval true The two iterators do not refer to the same position in the same container
        /// \retval false The two iterators refer to the same position in the same container
        ms_bool_t operator !=(const_iterator const &rhs) const
        {
            return !operator ==(rhs);
        }

    // Members
    private:
        container_type  *m_c;
        POSITION        m_pos;
        value_type      m_value;
    };

// Construction
public:
    /// Constructs an instance of the list adaptor
    ///
    /// \param c An instance of the list class
    ss_explicit_k list_adaptor(C &c)
        : m_c(c)
    {}

// State
public:
    /// Returns the number of elements in the sequence
    size_type size() const
    {
        return static_cast<size_type>(m_c.GetCount());
    }
    /// Indicates whether the search sequence is empty
    ms_bool_t empty() const
    {
        return m_c.GetCount() == 0;
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
    const_iterator  begin() const
    {
        return const_iterator(&m_c, m_c.GetHeadPosition());
    }
    /// Ends the iteration
    ///
    /// \return An iterator representing the end of the sequence
    const_iterator  end() const
    {
        return const_iterator();
    }

// Members
private:
    container_type  &m_c;
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

#endif /* !MFCSTL_INCL_H_MFCSTL_LIST_ADAPTOR */

/* ////////////////////////////////////////////////////////////////////////// */
