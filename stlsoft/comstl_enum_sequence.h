/* /////////////////////////////////////////////////////////////////////////////
 * File:        comstl_enum_sequence.h (originally MOEnSeq.h, ::SynesisCom)
 *
 * Purpose:     STL sequence for IEnumXXXX enumerator interfaces.
 *
 * Created:     17th September 1998
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


/// \file comstl_enum_sequence.h
///
/// STL sequence for IEnumXXXX enumerator interfaces.

#ifndef COMSTL_INCL_H_COMSTL_ENUM_SEQUENCE
#define COMSTL_INCL_H_COMSTL_ENUM_SEQUENCE

#ifndef __STLSOFT_DOCUMENTATION_SKIP_SECTION
# define COMSTL_VER_H_COMSTL_ENUM_SEQUENCE_MAJOR        3
# define COMSTL_VER_H_COMSTL_ENUM_SEQUENCE_MINOR        0
# define COMSTL_VER_H_COMSTL_ENUM_SEQUENCE_REVISION     1
# define COMSTL_VER_H_COMSTL_ENUM_SEQUENCE_EDIT         164
#endif /* !__STLSOFT_DOCUMENTATION_SKIP_SECTION */

/* ////////////////////////////////////////////////////////////////////////////
 * Includes
 */

#ifndef COMSTL_INCL_H_COMSTL
# include "comstl.h"                    // Include the COMSTL root header
#endif /* !COMSTL_INCL_H_COMSTL */
#ifndef COMSTL_INCL_H_COMSTL_REFCOUNT_FUNCTIONS
# include "comstl_refcount_functions.h" // safe_release, release_set_null
#endif /* !COMSTL_INCL_H_COMSTL_REFCOUNT_FUNCTIONS */
#ifndef COMSTL_INCL_H_COMSTL_ENUMERATOR_POLICIES
# include "comstl_enumerator_policies.h" // input_cloning_policy
#endif /* !COMSTL_INCL_H_COMSTL_ENUMERATOR_POLICIES */
#ifndef STLSOFT_INCL_H_STLSOFT_ITERATOR
# include "stlsoft_iterator.h"
#endif /* !STLSOFT_INCL_H_STLSOFT_ITERATOR */
#include <algorithm>

/* /////////////////////////////////////////////////////////////////////////////
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

/// \weakgroup comstl_itf_library Interface Library
/// \ingroup COMSTL libraries
/// \brief This library provides facilities for manipulating COM interfaces
/// @{

/* /////////////////////////////////////////////////////////////////////////////
 * Classes
 */

/// A template for adapting COM enumeration to STL-compatible sequence iteration
///
/// \param I Interface
/// \param V Value type
/// \param VP Value policy type
/// \param R Reference type
/// \param CP Cloning policy type
/// \param Q Quanta
///
/// The various parameterising types are used to stipulate the interface and the
/// value type, and how they are to be handled.
///
/// For example, the following parameterisation defines a sequence operating
/// over an <b>IEnumGUID</b> enumerator instance.
///
/// \htmlonly
/// <table border = 0 cellspacing = 0 cellpadding = 0 >
///  <tr><td>&nbsp;&nbsp;</td><td><code>typedef enumerator_sequence</code></td><td><code>&lt;&nbsp;IEnumGUID</code></td></tr>
///  <tr><td>&nbsp;&nbsp;</td><td><code>&nbsp;</code></td><td><code>,&nbsp;GUID</code></td></tr>
///  <tr><td>&nbsp;&nbsp;</td><td><code>&nbsp;</code></td><td><code>,&nbsp;GUID_policy</code></td></tr>
///  <tr><td>&nbsp;&nbsp;</td><td><code>&nbsp;</code></td><td><code>,&nbsp;GUID const &</code></td></tr>
///  <tr><td>&nbsp;&nbsp;</td><td><code>&nbsp;</code></td><td><code>,&nbsp;forward_cloning_policy&lt;IEnumGUID&gt;</code></td></tr>
///  <tr><td>&nbsp;&nbsp;</td><td><code>&nbsp;</code></td><td><code>,&nbsp;5</code></td></tr>
///  <tr><td>&nbsp;&nbsp;</td><td><code>&nbsp;</code></td><td><code>&gt;&nbsp;&nbsp;&nbsp;enum_sequence_t;</code></td></tr>
/// </table>
/// \endhtmlonly
///
/// The value type is <b>GUID</b> and it is returned as a reference, as
/// the <b>GUID const &</b> in fact.
///
/// The \ref COMSTL type <b>GUID_policy</b> controls how the <b>GUID</b>
/// instances are initialised, copied and destroyed.
///
/// The \ref COMSTL type forward_cloning_policy allows the sequence to provide
/// <a href = "http://sgi.com/tech/stl/ForwardIterator.html">Forward Iterator</a>
/// semantics.
///
/// And the <b>5</b> indicates that the sequence should grab 5 values at a time,
/// to save round trips to the enumerator.
///
/// So this would be used like the following:
///
/// \htmlonly
/// <code>
/// &nbsp;&nbsp;void dump_GUID(GUID const &);
/// <br>
///
/// <br>
/// &nbsp;&nbsp;LPENUMGUID&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;*penGUIDs = . . .;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;// Create an instance from wherever
/// <br>
/// &nbsp;&nbsp;enum_sequence_t&nbsp;&nbsp;&nbsp;guids(penGUIDs, false); // Eat the reference
/// <br>
///
/// <br>
/// &nbsp;&nbsp;std::for_each(guids.begin(), guids.end(), dump_GUID);
/// <br>
/// </code>
/// \endhtmlonly
///
template<   ss_typename_param_k I                                       /* Interface */
        ,   ss_typename_param_k V                                       /* Value type */
        ,   ss_typename_param_k VP                                      /* Value policy type */
        ,   ss_typename_param_k R   =   V const &                       /* Reference type */
        ,   ss_typename_param_k CP  =   degenerate_cloning_policy<I>    /* Cloning policy type */
        ,   cs_size_t           Q   =   8                               /* Quanta */
        >
class enumerator_sequence
{
public:
    /// Interface type
    typedef I                                                           interface_type;
    /// Value type
    typedef V                                                           value_type;
    /// Value policy type
    typedef policy_adaptor<VP>                                          value_policy_type;
    /// Reference type
    typedef R                                                           reference_type;
    /// Cloning policy type
    typedef CP                                                          cloning_policy_type;
    /// Iterator tag type
    typedef ss_typename_type_k cloning_policy_type::iterator_tag_type   iterator_tag_type;
    /// Quanta
    enum                                                                { quanta = Q };
    /// Type of the current parameterisation
    typedef enumerator_sequence<I, V, VP, R, CP, Q>                     class_type;

public:
    /// Constructor
    ///
    /// \param i The enumeration interface pointer to adapt
    /// \param bAddRef Causes a reference to be added if \c true, otherwise the sequence is deemed to <i>sink</i>, or consume, the interface pointer
    /// \param quant The actual quanta required for this instance. Must be <= Q
    enumerator_sequence(interface_type *i, cs_bool_t bAddRef, cs_size_t quant = 0)
        : m_i(i)
        , m_quanta(validate_quanta_(quant))
    {
        if(bAddRef)
        {
            m_i->AddRef();
        }
    }
    /// Releases the adapted interface pointer
    ~enumerator_sequence()
    {
        m_i->Release();
    }

public:
    /// Iterator for the enumerator_sequence class
    class iterator
        : public stlsoft_ns_qual(iterator_base)<iterator_tag_type, value_type, cs_ptrdiff_t, void, value_type>
    {
        typedef iterator    class_type;
    public:
        /// Constructor
        iterator(interface_type *i, cs_size_t quant)
            : m_i(i)
            , m_acquired(0)
            , m_current(0)
            , m_quanta(quant)
        {
            // Default initialise all elements
            comstl_ns_qual_std(for_each)(&m_values[0], &m_values[m_quanta], value_policy_type::_init());

            m_i->AddRef();

            operator ++();
        }
        /// Default constructor
        iterator()
            : m_i(0)
            , m_acquired(0)
            , m_current(0)
            , m_quanta(0)
        {
            // Default initialise all elements
            comstl_ns_qual_std(for_each)(&m_values[0], &m_values[quanta], value_policy_type::_init());
        }
        /// <a href = "http://synesis.com.au/resources/articles/cpp/movectors.pdf">Move constructor</a>
        iterator(class_type const &rhs)
            : m_i(cloning_policy_type::clone(const_cast<interface_type *&>(rhs.m_i)))
            , m_acquired(rhs.m_acquired)
            , m_current(rhs.m_current)
            , m_quanta(rhs.m_quanta)
        {
            value_type          *begin      =   &m_values[0];
            value_type          *end        =   &m_values[m_quanta];
            value_type const    *src_begin  =   &rhs.m_values[0];
            value_type const    *src_end    =   &rhs.m_values[rhs.m_acquired];

            for(; src_begin != src_end; ++begin, ++src_begin)
            {
                value_policy_type::init(begin);
                value_policy_type::copy(begin, src_begin);
            }

            for(; begin != end; ++begin)
            {
                value_policy_type::init(begin);
            }
        }
        /// Releases any internal storage
        ~iterator()
        {
            // Clear all elements
            comstl_ns_qual_std(for_each)(&m_values[0], &m_values[m_acquired], value_policy_type::_clear());

            safe_release(m_i);
        }

    public:
        /// Pre-increment operator
        iterator &operator ++()
        {
            if(++m_current < m_acquired)
            {
                // Do nothing
            }
            else if(NULL == m_i)
            {
                comstl_message_assert("Attempting to increment an invalid iterator", 0 < m_acquired && m_acquired < m_quanta && m_current == m_acquired);

                m_acquired = 0;
            }
            else
            {
                comstl_message_assert("Attempting to increment an invalid iterator", NULL != m_i);

                // Clear all elements
                comstl_ns_qual_std(for_each)(&m_values[0], &m_values[m_acquired], value_policy_type::_clear());

                // Reset enumeration
                m_current = 0;

                ULONG   cFetched    =   0;
                HRESULT hr          =   m_i->Next(m_quanta, &m_values[0], &cFetched);

                // We no longer checked for a FAILED(hr), since some iterators
                // return invalid results. We rely on cFetched, which is the
                // only reliable guide when marshalling anyway

                m_acquired = cFetched;

                if( hr == S_FALSE ||
                    m_acquired == 0)
                {
                    release_set_null(m_i);
                }
            }

            return *this;
        }

        void operator ++(int)
        {
            operator ++();
        }

    public:
        /// Returns the value represented by the current iteration position
        reference_type operator *()
        {
            return m_values[m_current];
        }

    public:
        /// Evaluates whether \c this and \c rhs are equivalent
        cs_bool_t operator == (iterator const &rhs) const
        {
            // The only valid comparison is when they both represent the end
            // values.

            return m_acquired == 0 && m_i == 0 && rhs.m_i == 0;
        }
        /// Evaluates whether \c this and \c rhs are not equivalent
        cs_bool_t operator != (iterator const &rhs) const
        {
            return !operator == (rhs);
        }

    private:
        interface_type  *m_i;
        cs_size_t       m_acquired;
        cs_size_t       m_current;
        cs_size_t const m_quanta;
        value_type      m_values[quanta];

    // Not to be implemented
    private:
        class_type &operator =(class_type const&);
    };

public:
    /// Begins the iteration
    ///
    /// \return An iterator representing the start of the sequence
    iterator begin() const
    {
        if(NULL != m_i)
        {
            m_i->Reset();
        }

        return iterator(m_i, m_quanta);
    }
    /// Ends the iteration
    ///
    /// \return An iterator representing the end of the sequence
    iterator end() const
    {
        return iterator();
    }

// Implementation
private:
    static cs_size_t validate_quanta_(cs_size_t quant)
    {
        comstl_assert(quant <= quanta); // Could have named these things better!

        if( 0 == quant ||
            quant > quanta)
        {
            quant = quanta;
        }

        return quant;
    }

// Members
private:
    interface_type  *m_i;
    cs_size_t const m_quanta;

// Implementation
private:
    enumerator_sequence(class_type const &);
    enumerator_sequence const &operator =(class_type const &);
};


/// \brief Deprecated forwarding template for enumerator_sequence, providing backwards compatibility
///
/// \deprecated
template<   ss_typename_param_k I   /* Interface */
        ,   ss_typename_param_k V   /* Value type */
        ,   ss_typename_param_k VP  /* Value policy type */
        ,   ss_typename_param_k R   /* Reference type */
        ,   ss_typename_param_k CP  /* Cloning policy type */
        ,   cs_size_t           Q   /* Quanta */
        >
class enum_simple_sequence
    : public enumerator_sequence<I, V, VP, R, CP, Q>
{
private:
    typedef enumerator_sequence<I, V, VP, R, CP, Q>     parent_class_type;
public:
    /// Interface type
    typedef ss_typename_type_k parent_class_type::interface_type        interface_type;
    /// Value type
    typedef ss_typename_type_k parent_class_type::value_type            value_type;
    /// Value policy type
    typedef ss_typename_type_k parent_class_type::value_policy_type     value_policy_type;
    /// Reference type
    typedef ss_typename_type_k parent_class_type::reference_type        reference_type;
    /// Cloning policy type
    typedef ss_typename_type_k parent_class_type::cloning_policy_type   cloning_policy_type;
    /// Iterator tag type
    typedef ss_typename_type_k parent_class_type::iterator_tag_type     iterator_tag_type;
    /// Type of the current parameterisation
    typedef enum_simple_sequence<I, V, VP, R, CP, Q>                    class_type;

public:
    /// Constructor
    ///
    /// \param i The enumeration interface pointer to adapt
    /// \param bAddRef Causes a reference to be added if \c true, otherwise the sequence is deemed to <i>sink</i>, or consume, the interface pointer
    enum_simple_sequence(interface_type *i, cs_bool_t bAddRef)
        : parent_class_type(i, bAddRef)
    {}
};


////////////////////////////////////////////////////////////////////////////////
// Unit-testing

#ifdef STLSOFT_UNITTEST

namespace unittest
{
    ss_bool_t test_comstl_enum_sequence(unittest_reporter *r)
    {
        using stlsoft::unittest::unittest_initialiser;

        ss_bool_t               bSuccess    =   true;

        unittest_initialiser    init(r, "COMSTL", "enum_sequence", __FILE__);

#if 0
        if(<<TODO>>)
        {
            r->report("<<TODO>> failed ", __LINE__);
            bSuccess = false;
        }
#endif /* 0 */

        return bSuccess;
    }

    unittest_registrar    unittest_comstl_enum_sequence(test_comstl_enum_sequence);

} // namespace unittest

#endif /* STLSOFT_UNITTEST */

/* ////////////////////////////////////////////////////////////////////////// */

/// @} // end of group comstl_itf_library

/* ////////////////////////////////////////////////////////////////////////// */

#ifndef _COMSTL_NO_NAMESPACE
# if defined(_STLSOFT_NO_NAMESPACE) || \
     defined(__STLSOFT_DOCUMENTATION_SKIP_SECTION)
} // namespace comstl
# else
} // namespace stlsoft::comstl_project
} // namespace stlsoft
# endif /* _STLSOFT_NO_NAMESPACE */
#endif /* !_COMSTL_NO_NAMESPACE */

/* ////////////////////////////////////////////////////////////////////////// */

#endif /* !COMSTL_INCL_H_COMSTL_ENUM_SEQUENCE */

/* ////////////////////////////////////////////////////////////////////////// */
