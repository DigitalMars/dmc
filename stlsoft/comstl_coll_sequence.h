/* /////////////////////////////////////////////////////////////////////////////
 * File:        comstl_collection_sequence.h (originally MOEnSeq.h, ::SynesisCom)
 *
 * Purpose:     STL sequence for COM collection interfaces.
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


/// \file comstl_coll_sequence.h
///
/// STL sequence for COM collection interfaces.

#ifndef COMSTL_INCL_H_COMSTL_COLL_SEQUENCE
#define COMSTL_INCL_H_COMSTL_COLL_SEQUENCE

#ifndef __STLSOFT_DOCUMENTATION_SKIP_SECTION
# define COMSTL_VER_H_COMSTL_COLL_SEQUENCE_MAJOR        3
# define COMSTL_VER_H_COMSTL_COLL_SEQUENCE_MINOR        0
# define COMSTL_VER_H_COMSTL_COLL_SEQUENCE_REVISION     1
# define COMSTL_VER_H_COMSTL_COLL_SEQUENCE_EDIT         39
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
#ifndef COMSTL_INCL_H_COMSTL_INTERFACE_TRAITS
# include "comstl_interface_traits.h"
#endif /* !COMSTL_INCL_H_COMSTL_INTERFACE_TRAITS */
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
/// over a notional <b>IGUIDCollection</b> collection instance.
///
/// \htmlonly
/// <table border = 0 cellspacing = 0 cellpadding = 0 >
///  <tr><td>&nbsp;&nbsp;</td><td><code>typedef collection_sequence</code></td><td><code>&lt;&nbsp;IGUIDCollection</code></td></tr>
///  <tr><td>&nbsp;&nbsp;</td><td><code>&nbsp;</code></td><td><code>,&nbsp;IEnumGUID</code></td></tr>
///  <tr><td>&nbsp;&nbsp;</td><td><code>&nbsp;</code></td><td><code>,&nbsp;GUID</code></td></tr>
///  <tr><td>&nbsp;&nbsp;</td><td><code>&nbsp;</code></td><td><code>,&nbsp;GUID_policy</code></td></tr>
///  <tr><td>&nbsp;&nbsp;</td><td><code>&nbsp;</code></td><td><code>,&nbsp;GUID const &</code></td></tr>
///  <tr><td>&nbsp;&nbsp;</td><td><code>&nbsp;</code></td><td><code>,&nbsp;forward_cloning_policy&lt;IEnumGUID&gt;</code></td></tr>
///  <tr><td>&nbsp;&nbsp;</td><td><code>&nbsp;</code></td><td><code>,&nbsp;5</code></td></tr>
///  <tr><td>&nbsp;&nbsp;</td><td><code>&nbsp;</code></td><td><code>&gt;&nbsp;&nbsp;&nbsp;collection_sequence_t;</code></td></tr>
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
/// &nbsp;&nbsp;IGUIDCollection&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;*penGUIDs = . . .;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;// Create an instance from wherever
/// <br>
/// &nbsp;&nbsp;collection_sequence_t&nbsp;&nbsp;&nbsp;guids(penGUIDs, false); // Eat the reference
/// <br>
///
/// <br>
/// &nbsp;&nbsp;std::for_each(guids.begin(), guids.end(), dump_GUID);
/// <br>
/// </code>
/// \endhtmlonly
///
template<   ss_typename_param_k CI                                      /* Collection interface */
        ,   ss_typename_param_k EI                                      /* Enumerator interface */
        ,   ss_typename_param_k V                                       /* Value type */
        ,   ss_typename_param_k VP                                      /* Value policy type */
        ,   ss_typename_param_k R   =   V const &                       /* Reference type */
        ,   ss_typename_param_k CP  =   degenerate_cloning_policy<EI>   /* Cloning policy type */
        ,   cs_size_t           Q   =   8                               /* Quanta */
        >
class collection_sequence
{
public:
    /// Colleciton interface type
    typedef CI                                                          collection_interface_type;
    /// Enumerator interface type
    typedef EI                                                          enumerator_interface_type;
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
    typedef collection_sequence<CI, EI, V, VP, R, CP, Q>                class_type;
    /// Type of the current parameterisation
    typedef class_type                                                  sequence_type;
    /// The size type
    typedef cs_size_t                                                   size_type;

public:
    /// Constructor
    ///
    /// \param i The enumeration interface pointer to adapt
    /// \param bAddRef Causes a reference to be added if \c true, otherwise the sequence is deemed to <i>sink</i>, or consume, the interface pointer
    /// \param quant The actual quanta required for this instance. Must be <= Q
    collection_sequence(collection_interface_type *i, cs_bool_t bAddRef, cs_size_t quant = 0)
        : m_i(i)
        , m_quanta(validate_quanta_(quant))
    {
        if(bAddRef)
        {
            m_i->AddRef();
        }
    }
    /// Releases the adapted interface pointer
    ~collection_sequence()
    {
        m_i->Release();
    }

public:
    /// Iterator for the collection_sequence class
    class iterator
        : public stlsoft_ns_qual(iterator_base)<iterator_tag_type, value_type, ss_ptrdiff_t, void, value_type>
    {
        typedef iterator    class_type;
    private:
        friend class collection_sequence<CI, EI, V, VP, R, CP, Q>;

        /// Value constructor
        iterator(enumerator_interface_type *i, cs_size_t quant)
            : m_i(i)
            , m_acquired(0)
            , m_current(0)
            , m_quanta(quant)
        {
            // Default initialise all elements
            comstl_ns_qual_std(for_each)(&m_values[0], &m_values[m_quanta], value_policy_type::_init());

//            m_i->AddRef();

            operator ++();
        }
    public:
        /// Default constructor
        iterator()
            : m_i(0)
            , m_acquired(0)
            , m_current(0)
            , m_quanta(0)
        {
            // Default initialise all elements
            comstl_ns_qual_std(for_each)(&m_values[0], &m_values[m_quanta], value_policy_type::_init());
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

#if 1
            for(; begin != end; ++begin)
            {
                value_policy_type::init(begin);
            }
#else /* ? 0 */
            std::for_each(begin, end, s
#endif /* 0 */
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

    // Members
    private:
        enumerator_interface_type   *m_i;
        cs_size_t                   m_acquired;
        cs_size_t                   m_current;
        cs_size_t                   m_quanta;
        value_type                  m_values[quanta];

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
        LPUNKNOWN   punkEnum;
        HRESULT     hr  =   m_i->get__NewEnum(&punkEnum);

        if(SUCCEEDED(hr))
        {
            enumerator_interface_type   *ei;

            hr = punkEnum->QueryInterface(IID_traits<enumerator_interface_type>::iid(), reinterpret_cast<void**>(&ei));

            punkEnum->Release();

            if(SUCCEEDED(hr))
            {
                return iterator(ei, m_quanta);
            }
        }

        return iterator();
    }
    /// Ends the iteration
    ///
    /// \return An iterator representing the end of the sequence
    iterator end() const
    {
        return iterator();
    }

    /// Returns the number of items in the collection
    ///
    /// \note This method will not compile for collection interfaces
    /// that do not contain the get_Count method
    size_type size() const
    {
        ULONG   count;
        HRESULT hr  =   m_i->get_Count(&count);

        return SUCCEEDED(hr) ? count : 0;
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
    collection_interface_type   *m_i;
    cs_size_t const             m_quanta;

// Implementation
private:
    collection_sequence(class_type const &);
    collection_sequence const &operator =(class_type const &);
};

////////////////////////////////////////////////////////////////////////////////
// Unit-testing

#ifdef STLSOFT_UNITTEST

namespace unittest
{
    ss_bool_t test_comstl_coll_sequence(unittest_reporter *r)
    {
        using stlsoft::unittest::unittest_initialiser;

        ss_bool_t               bSuccess    =   true;

        unittest_initialiser    init(r, "COMSTL", "coll_sequence", __FILE__);

#if 0
        if(<<TODO>>)
        {
            r->report("<<TODO>> failed ", __LINE__);
            bSuccess = false;
        }
#endif /* 0 */

        return bSuccess;
    }

    unittest_registrar    unittest_comstl_coll_sequence(test_comstl_coll_sequence);

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

#endif /* !COMSTL_INCL_H_COMSTL_COLL_SEQUENCE */

/* ////////////////////////////////////////////////////////////////////////// */
