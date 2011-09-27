/* /////////////////////////////////////////////////////////////////////////////
 * File:        stlsoft_pod_vector.h
 *
 * Purpose:     Contains the pod_vector class.
 *
 * Created:     23rd December 2003
 * Updated:     11th September 2004
 *
 * Thanks to:   Chris Newcombe for requesting sufficient enhancements to
 *              auto_buffer such that pod_vector was born.
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


/// \file stlsoft_pod_vector.h
///
/// Contains the pod_vector class.

#ifndef STLSOFT_INCL_H_STLSOFT_POD_VECTOR
#define STLSOFT_INCL_H_STLSOFT_POD_VECTOR

#ifndef __STLSOFT_DOCUMENTATION_SKIP_SECTION
# define STLSOFT_VER_H_STLSOFT_POD_VECTOR_MAJOR       2
# define STLSOFT_VER_H_STLSOFT_POD_VECTOR_MINOR       0
# define STLSOFT_VER_H_STLSOFT_POD_VECTOR_REVISION    1
# define STLSOFT_VER_H_STLSOFT_POD_VECTOR_EDIT        28
#endif /* !__STLSOFT_DOCUMENTATION_SKIP_SECTION */

/* /////////////////////////////////////////////////////////////////////////////
 * Compatibility
 */

/*
[Incompatibilies-start]
__STLSOFT_COMPILER_IS_MSVC: _MSC_VER<1200
__STLSOFT_COMPILER_IS_MSVC: _MSC_VER==1310
[Incompatibilies-end]
 */

/* /////////////////////////////////////////////////////////////////////////////
 * Includes
 */

#ifndef STLSOFT_INCL_H_STLSOFT
# include "stlsoft.h"                   // Include the STLSoft root header
#endif /* !STLSOFT_INCL_H_STLSOFT */

#if defined(__STLSOFT_COMPILER_IS_MSVC) && \
    _MSC_VER < 1200
# error stlsoft_pod_vector.h is not compatible with Visual C++ 5.0 or earlier
#endif /* _MSC_VER < 1200 */
#if defined(__STLSOFT_COMPILER_IS_MSVC) && \
    _MSC_VER == 1310
# error stlsoft_pod_vector.h is not compatible with Visual C++ 7.1
#endif /* _MSC_VER < 1200 */

#ifndef STLSOFT_INCL_H_STLSOFT_AUTO_BUFFER
# include "stlsoft_auto_buffer.h"       // auto_buffer
#endif /* !STLSOFT_INCL_H_STLSOFT_AUTO_BUFFER */
#ifndef STLSOFT_INCL_H_STLSOFT_MALLOC_ALLOCATOR
# include "stlsoft_malloc_allocator.h"  // malloc_allocator
#endif /* !STLSOFT_INCL_H_STLSOFT_MALLOC_ALLOCATOR */
#ifndef STLSOFT_INCL_H_STLSOFT_SIMPLE_ALGORITHMS
# include "stlsoft_simple_algorithms.h" //
#endif /* !STLSOFT_INCL_H_STLSOFT_SIMPLE_ALGORITHMS */
#include <stdexcept>                    // std::out_of_range

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

/// Efficient vector class for use with POD types only
template<   ss_typename_param_k T
#if defined(__STLSOFT_CF_TEMPLATE_CLASS_DEFAULT_CLASS_ARGUMENT_SUPPORT) && \
    defined(__STLSOFT_CF_TEMPLATE_CLASS_DEFAULT_FUNDAMENTAL_ARGUMENT_SUPPORT)
        ,   ss_typename_param_k A       =   malloc_allocator<T>
        ,   ss_size_t           SPACE   =   64
#else /* ? __STLSOFT_CF_TEMPLATE_CLASS_DEFAULT_CLASS_ARGUMENT_SUPPORT && __STLSOFT_CF_TEMPLATE_CLASS_DEFAULT_FUNDAMENTAL_ARGUMENT_SUPPORT */
        ,   ss_typename_param_k A       /* =   malloc_allocator<T> */
        ,   ss_size_t           SPACE   /* =    64 */
#endif /* __STLSOFT_CF_TEMPLATE_CLASS_DEFAULT_CLASS_ARGUMENT_SUPPORT && __STLSOFT_CF_TEMPLATE_CLASS_DEFAULT_FUNDAMENTAL_ARGUMENT_SUPPORT */
        >
class pod_vector
{
/// \name Typedefs
/// @{
private:
    typedef auto_buffer<T, A, SPACE>                                buffer_type;
public:
    /// The value type
    typedef ss_typename_type_k buffer_type::value_type              value_type;
    /// The allocator type
    typedef ss_typename_type_k buffer_type::allocator_type          allocator_type;
    /// The type of the current parameterisation
    typedef pod_vector<T, A, SPACE>                                 class_type;
    /// The reference type
    typedef ss_typename_type_k buffer_type::reference               reference;
    /// The non-mutable (const) reference type
    typedef ss_typename_type_k buffer_type::const_reference         const_reference;
    /// The pointer type
    typedef ss_typename_type_k buffer_type::pointer                 pointer;
    /// The non-mutable (const) pointer type
    typedef ss_typename_type_k buffer_type::const_pointer           const_pointer;
    /// The iterator type
    typedef ss_typename_type_k buffer_type::iterator                iterator;
    /// The non-mutable (const) iterator type
    typedef ss_typename_type_k buffer_type::const_iterator          const_iterator;
#if defined(__STLSOFT_CF_BIDIRECTIONAL_ITERATOR_SUPPORT)
    /// The type of the non-const (mutating) reverse iterator
    typedef ss_typename_type_k buffer_type::reverse_iterator        reverse_iterator;
    /// The type of the const (non-mutating) reverse iterator
    typedef ss_typename_type_k buffer_type::const_reverse_iterator  const_reverse_iterator;
#endif /* __STLSOFT_CF_BIDIRECTIONAL_ITERATOR_SUPPORT */
    /// The size type
    typedef ss_typename_type_k buffer_type::size_type               size_type;
    /// The difference type
    typedef ss_typename_type_k buffer_type::difference_type         difference_type;

/// @}

/// \name Construction
/// @{
public:
    ss_explicit_k pod_vector(size_type cItems = 0);
    pod_vector(size_type cItems, value_type const &value);
    pod_vector(class_type const &rhs);
    pod_vector(const_iterator first, const_iterator last);

    pod_vector &operator =(class_type const &rhs);
/// @}

/// \name Iteration
/// @{
public:
    iterator                begin();
    const_iterator          begin() const;
    iterator                end();
    const_iterator          end() const;
#if defined(__STLSOFT_CF_BIDIRECTIONAL_ITERATOR_SUPPORT)
    reverse_iterator        rbegin();
    const_reverse_iterator  rbegin() const;
    reverse_iterator        rend();
    const_reverse_iterator  rend() const;
#endif /* __STLSOFT_CF_BIDIRECTIONAL_ITERATOR_SUPPORT */
/// @}

/// \name Attributes
/// @{
public:
    size_type       size() const;
    size_type       capacity() const;
    size_type       max_size() const;
    ss_bool_t       empty() const;
    allocator_type  get_allocator() const;
/// @}

/// \name Accessors
/// @{
public:
    reference       at(size_type index);
    const_reference at(size_type index) const;
    reference       operator[](size_type index);
    const_reference operator[](size_type index) const;
    reference       front();
    const_reference front() const;
    reference       back();
    const_reference back() const;
/// @}

/// \name Operations
/// @{
public:
    void        clear();
    void        swap(class_type &rhs);
    void        reserve(size_type cItems) /* stlsoft_throw_1(stlsoft_ns_qual_std(bad_alloc) ) */;
// Note: resize() is split into two, so the one-param version can be very quick
    void        resize(size_type cItems) /* stlsoft_throw_1(stlsoft_ns_qual_std(bad_alloc) ) */;
    void        resize(size_type cItems, value_type const &value) /* stlsoft_throw_1(stlsoft_ns_qual_std(bad_alloc) ) */;
    void        push_back(value_type const &value);
    void        pop_back();

    void        assign(const_iterator first, const_iterator last);
    void        assign(size_type cItems, value_type const &value = value_type());
    iterator    insert(iterator it, value_type const &value = value_type());
    void        insert(iterator it, size_type cItems, value_type const &value);
    void        insert(iterator it, const_iterator first, const_iterator last);
    iterator    erase(iterator it);
    iterator    erase(iterator first, iterator last);
/// @}

/// \name Implementation
/// @{
private:
    void        range_check_(size_type index) const /* stlsoft_throw_1(stlsoft_ns_qual_std(out_of_range) ) */;

    ss_bool_t   resize_(size_type cItems) /* stlsoft_throw_1(stlsoft_ns_qual_std(bad_alloc) ) */;

    ss_bool_t   is_valid_() const;
/// @}

/// \name Members
/// @{
private:
    size_type   m_cItems;   // A size member is used, rather than m_end (iterator), because some of the state
                            // is maintained in the parent class. Doing it this way makes swap() and other methods
                            // very simple.
    buffer_type m_buffer;   // The auto_buffer
/// @}
};

/* /////////////////////////////////////////////////////////////////////////////
 * Implementation
 */

template<   ss_typename_param_k T
        ,   ss_typename_param_k A
        ,   ss_size_t           SPACE
        >
inline ss_bool_t operator ==(pod_vector<T, A, SPACE> const &lhs, pod_vector<T, A, SPACE> const &rhs)
{
    if(lhs.size() != rhs.size())
    {
        return false;
    }
    else
    {
#if 0
        for(ss_typename_type_k pod_vector<T, A, SPACE>::size_type i = 0, size = lhs.size(); i < size; ++i)
        {
            if(lhs[i] != rhs[i])
            {
                return false;
            }
        }

        return true;
#else /* ? 0 */
        return 0 == memcmp(&lhs[0], &rhs[0], sizeof(ss_typename_type_k pod_vector<T, A, SPACE>::size_type) * lhs.size());
#endif /* 0 */
    }
}

template<   ss_typename_param_k T
        ,   ss_typename_param_k A
        ,   ss_size_t           SPACE
        >
inline ss_bool_t operator !=(pod_vector<T, A, SPACE> const &lhs, pod_vector<T, A, SPACE> const &rhs)
{
    return !operator ==(lhs, rhs);
}

/* /////////////////////////////////////////////////////////////////////////////
 * Unit-testing
 */

#ifdef STLSOFT_UNITTEST

namespace unittest
{
    ss_bool_t test_stlsoft_pod_vector(unittest_reporter *r)
    {
        ss_bool_t               bSuccess    =   true;

        unittest_initialiser    init(r, "STLSoft", "pod_vector", __FILE__);

        typedef pod_vector< int
                        ,   malloc_allocator<int>
                        ,   64
                        >           iv_t;

        // Set up some vectors
        iv_t        v1(10);
        iv_t        v2(32, 1);
        iv_t const  v3(100, -1);
        ss_size_t   i;

        for(i = 0; i < v1.size(); ++i)
        {
            v1[i] = static_cast<int>(i);
        }
        for(i = 0; i < v2.size(); ++i)
        {
            v2[i] = static_cast<int>(i * 100);
        }

        // (i) Test copy construction
        iv_t        v4(v1);

        if(v4 != v1)
        {
            r->report("copy construction failed", __LINE__);
            bSuccess = false;
        }

        // (ii) Test iterator construction
        iv_t        v5(const_cast<iv_t const&>(v2).begin(), const_cast<iv_t const&>(v2).end());

        if(v5 != v2)
        {
            r->report("iterator construction failed", __LINE__);
            bSuccess = false;
        }

        v1.begin();
        v2.begin();
        v1.end();
        v2.end();
#if defined(__STLSOFT_CF_BIDIRECTIONAL_ITERATOR_SUPPORT)
        v1.rbegin();
        v2.rbegin();
        v1.rend();
        v2.rend();
#endif /* __STLSOFT_CF_BIDIRECTIONAL_ITERATOR_SUPPORT */

        v1.size();
        v2.capacity();
        v3.max_size();
        v4.empty();
        v1.get_allocator();

        STLSOFT_SUPPRESS_UNUSED(v1.at(0));
        STLSOFT_SUPPRESS_UNUSED(v2.at(0));
        STLSOFT_SUPPRESS_UNUSED(v1[0]);
        STLSOFT_SUPPRESS_UNUSED(v2[0]);
        STLSOFT_SUPPRESS_UNUSED(v1.front());
        STLSOFT_SUPPRESS_UNUSED(v2.front());
        STLSOFT_SUPPRESS_UNUSED(v1.back());
        STLSOFT_SUPPRESS_UNUSED(v2.back());

        v1.clear();
        v1.swap(v4);
        v1.reserve(v1.size() * 2);
        v1.resize(v1.capacity());
        v4.resize(1000, 127);
        v1.push_back(101);
        v1.pop_back();

        v1.assign(const_cast<iv_t const&>(v4).begin(), const_cast<iv_t const&>(v4).end());
#if 0
        v3.assign(100, 65535);
#endif /* 0 */

#if 0
        iterator    insert(iterator it, value_type const &value = value_type());
        void        insert(iterator it, size_type cItems, value_type const &value);
        void        insert(iterator it, const_iterator first, const_iterator last);
        iterator    erase(iterator it);
        iterator    erase(iterator first, iterator last);
#endif /* 0 */

#if 0
        if(<<TODO>>)
        {
            r->report("<<TODO>> failed ", __LINE__);
            bSuccess = false;
        }
#endif /* 0 */

        return bSuccess;
    }

    unittest_registrar    unittest_stlsoft_pod_vector(test_stlsoft_pod_vector);

} // namespace unittest

#endif /* STLSOFT_UNITTEST */

/* /////////////////////////////////////////////////////////////////////////////
 * Implementation
 */

#ifndef __STLSOFT_DOCUMENTATION_SKIP_SECTION

template<   ss_typename_param_k T
        ,   ss_typename_param_k A
        ,   ss_size_t           SPACE
        >
#ifdef __STLSOFT_CF_FUNCTION_SIGNATURE_FULL_ARG_QUALIFICATION_REQUIRED
inline void pod_vector<T, A, SPACE>::range_check_(ss_typename_type_k pod_vector<T, A, SPACE>::size_type index) const /* stlsoft_throw_1(stlsoft_ns_qual_std(out_of_range) ) */
#else /* ? __STLSOFT_CF_FUNCTION_SIGNATURE_FULL_ARG_QUALIFICATION_REQUIRED */
inline void pod_vector<T, A, SPACE>::range_check_(size_type index) const /* stlsoft_throw_1(stlsoft_ns_qual_std(out_of_range) ) */
#endif /* __STLSOFT_CF_FUNCTION_SIGNATURE_FULL_ARG_QUALIFICATION_REQUIRED */
{
#ifdef __STLSOFT_CF_EXCEPTION_SUPPORT
    if(!(index < size()))
    {
        stlsoft_ns_qual_std(out_of_range)   x("pod vector index out of range");

#ifdef __STLSOFT_COMPILER_IS_MSVC
        if(1) // This is needed to avoid Visual C++ warning 4702: 'unreachable code'
#endif /* __STLSOFT_COMPILER_IS_MSVC */
        {
            throw x;
        }
    }
#else
    stlsoft_message_assert("w index out of range", index < size());
#endif /* __STLSOFT_CF_EXCEPTION_SUPPORT */
}

template<   ss_typename_param_k T
        ,   ss_typename_param_k A
        ,   ss_size_t           SPACE
        >
#ifdef __STLSOFT_CF_FUNCTION_SIGNATURE_FULL_ARG_QUALIFICATION_REQUIRED
inline ss_bool_t pod_vector<T, A, SPACE>::resize_(ss_typename_type_k pod_vector<T, A, SPACE>::size_type cItems) /* stlsoft_throw_1(stlsoft_ns_qual_std(bad_alloc) ) */
#else /* ? __STLSOFT_CF_FUNCTION_SIGNATURE_FULL_ARG_QUALIFICATION_REQUIRED */
inline ss_bool_t pod_vector<T, A, SPACE>::resize_(size_type cItems) /* stlsoft_throw_1(stlsoft_ns_qual_std(bad_alloc) ) */
#endif /* __STLSOFT_CF_FUNCTION_SIGNATURE_FULL_ARG_QUALIFICATION_REQUIRED */
{
    // This method is only called for insertions, so we can make some assumptions.

    size_type   curr_capacity   =   capacity();

    // We only resize the internal buffer if it is not large enough
    if(cItems > curr_capacity)
    {
        size_type   capacity = m_buffer.internal_size() + cItems;

        capacity -= capacity % m_buffer.internal_size();

        if(!m_buffer.resize(capacity))
        {
            return false;
        }
    }

    m_cItems = cItems;

    return true;
}


template<   ss_typename_param_k T
        ,   ss_typename_param_k A
        ,   ss_size_t           SPACE
        >
inline ss_bool_t pod_vector<T, A, SPACE>::is_valid_() const
{
    if(m_buffer.size() < m_cItems)
    {
        return false;
    }

    return true;
}


// Construction

template<   ss_typename_param_k T
        ,   ss_typename_param_k A
        ,   ss_size_t           SPACE
        >
#ifdef __STLSOFT_CF_FUNCTION_SIGNATURE_FULL_ARG_QUALIFICATION_REQUIRED
inline /* ss_explicit_k */ pod_vector<T, A, SPACE>::pod_vector(ss_typename_type_k pod_vector<T, A, SPACE>::size_type cItems /* = 0 */)
#else /* ? __STLSOFT_CF_FUNCTION_SIGNATURE_FULL_ARG_QUALIFICATION_REQUIRED */
inline /* ss_explicit_k */ pod_vector<T, A, SPACE>::pod_vector(size_type cItems /* = 0 */)
#endif /* __STLSOFT_CF_FUNCTION_SIGNATURE_FULL_ARG_QUALIFICATION_REQUIRED */
    : m_buffer(cItems)
{
    m_cItems = m_buffer.size(); // This is done here, since it comes before m_buffer in
                                // the object layout for efficiency (caching) reasons

    stlsoft_assert(is_valid_());
}

template<   ss_typename_param_k T
        ,   ss_typename_param_k A
        ,   ss_size_t           SPACE
        >
#ifdef __STLSOFT_CF_FUNCTION_SIGNATURE_FULL_ARG_QUALIFICATION_REQUIRED
inline pod_vector<T, A, SPACE>::pod_vector(ss_typename_type_k pod_vector<T, A, SPACE>::size_type cItems, ss_typename_type_k pod_vector<T, A, SPACE>::value_type const &value)
#else /* ? __STLSOFT_CF_FUNCTION_SIGNATURE_FULL_ARG_QUALIFICATION_REQUIRED */
inline pod_vector<T, A, SPACE>::pod_vector(size_type cItems, value_type const &value)
#endif /* __STLSOFT_CF_FUNCTION_SIGNATURE_FULL_ARG_QUALIFICATION_REQUIRED */
    : m_buffer(cItems)
{
    m_cItems = m_buffer.size(); // This is done here, since it comes before m_buffer in
                                // the object layout for efficiency (caching) reasons

    pod_fill_n(begin(), size(), value);

    stlsoft_assert(is_valid_());
}

template<   ss_typename_param_k T
        ,   ss_typename_param_k A
        ,   ss_size_t           SPACE
        >
#ifdef __STLSOFT_CF_FUNCTION_SIGNATURE_FULL_ARG_QUALIFICATION_REQUIRED
inline pod_vector<T, A, SPACE>::pod_vector(ss_typename_type_k pod_vector<T, A, SPACE>::class_type const &rhs)
#else /* ? __STLSOFT_CF_FUNCTION_SIGNATURE_FULL_ARG_QUALIFICATION_REQUIRED */
inline pod_vector<T, A, SPACE>::pod_vector(class_type const &rhs)
#endif /* __STLSOFT_CF_FUNCTION_SIGNATURE_FULL_ARG_QUALIFICATION_REQUIRED */
    : m_buffer(rhs.size())
{
    m_cItems = m_buffer.size(); // This is done here, since it comes before m_buffer in
                                // the object layout for efficiency (caching) reasons

    pod_copy_n(begin(), &rhs[0], size());

    stlsoft_assert(is_valid_());
}

template<   ss_typename_param_k T
        ,   ss_typename_param_k A
        ,   ss_size_t           SPACE
        >
#ifdef __STLSOFT_CF_FUNCTION_SIGNATURE_FULL_ARG_QUALIFICATION_REQUIRED
inline pod_vector<T, A, SPACE>::pod_vector(ss_typename_type_k pod_vector<T, A, SPACE>::const_iterator first, ss_typename_type_k pod_vector<T, A, SPACE>::const_iterator last)
#else /* ? __STLSOFT_CF_FUNCTION_SIGNATURE_FULL_ARG_QUALIFICATION_REQUIRED */
inline pod_vector<T, A, SPACE>::pod_vector(const_iterator first, const_iterator last)
#endif /* __STLSOFT_CF_FUNCTION_SIGNATURE_FULL_ARG_QUALIFICATION_REQUIRED */
    : m_buffer(last - first)
{
    m_cItems = m_buffer.size(); // This is done here, since it comes before m_buffer in
                                // the object layout for efficiency (caching) reasons

    if(0 != size()) // It will either be the full size requested, or 0
    {
        pod_copy(first, last, begin());
    }

    stlsoft_assert(is_valid_());
}

template<   ss_typename_param_k T
        ,   ss_typename_param_k A
        ,   ss_size_t           SPACE
        >
#ifdef __STLSOFT_CF_FUNCTION_SIGNATURE_FULL_ARG_QUALIFICATION_REQUIRED
inline pod_vector<T, A, SPACE> &pod_vector<T, A, SPACE>::operator =(ss_typename_type_k pod_vector<T, A, SPACE>::class_type const &rhs)
#else /* ? __STLSOFT_CF_FUNCTION_SIGNATURE_FULL_ARG_QUALIFICATION_REQUIRED */
inline pod_vector<T, A, SPACE> &pod_vector<T, A, SPACE>::operator =(class_type const &rhs)
#endif /* __STLSOFT_CF_FUNCTION_SIGNATURE_FULL_ARG_QUALIFICATION_REQUIRED */
{
// NOTE: This will be optimised by testing the contents of this and the rhs

    stlsoft_assert(is_valid_());

    class_type  temp(rhs);

    temp.swap(*this);

    stlsoft_assert(is_valid_());

    return *this;
}

// Iteration


template<   ss_typename_param_k T
        ,   ss_typename_param_k A
        ,   ss_size_t           SPACE
        >
#ifdef __STLSOFT_CF_FUNCTION_SIGNATURE_FULL_ARG_QUALIFICATION_REQUIRED
inline ss_typename_type_k pod_vector<T, A, SPACE>::iterator pod_vector<T, A, SPACE>::begin()
#else /* ? __STLSOFT_CF_FUNCTION_SIGNATURE_FULL_ARG_QUALIFICATION_REQUIRED */
inline ss_typename_type_k pod_vector<T, A, SPACE>::iterator pod_vector<T, A, SPACE>::begin()
#endif /* __STLSOFT_CF_FUNCTION_SIGNATURE_FULL_ARG_QUALIFICATION_REQUIRED */
{
    stlsoft_assert(is_valid_());

    return m_buffer.begin();
}

template<   ss_typename_param_k T
        ,   ss_typename_param_k A
        ,   ss_size_t           SPACE
        >
#ifdef __STLSOFT_CF_FUNCTION_SIGNATURE_FULL_ARG_QUALIFICATION_REQUIRED
inline ss_typename_type_k pod_vector<T, A, SPACE>::const_iterator pod_vector<T, A, SPACE>::begin() const
#else /* ? __STLSOFT_CF_FUNCTION_SIGNATURE_FULL_ARG_QUALIFICATION_REQUIRED */
inline ss_typename_type_k pod_vector<T, A, SPACE>::const_iterator pod_vector<T, A, SPACE>::begin() const
#endif /* __STLSOFT_CF_FUNCTION_SIGNATURE_FULL_ARG_QUALIFICATION_REQUIRED */
{
    stlsoft_assert(is_valid_());

    return m_buffer.begin();
}

template<   ss_typename_param_k T
        ,   ss_typename_param_k A
        ,   ss_size_t           SPACE
        >
#ifdef __STLSOFT_CF_FUNCTION_SIGNATURE_FULL_ARG_QUALIFICATION_REQUIRED
inline ss_typename_type_k pod_vector<T, A, SPACE>::iterator pod_vector<T, A, SPACE>::end()
#else /* ? __STLSOFT_CF_FUNCTION_SIGNATURE_FULL_ARG_QUALIFICATION_REQUIRED */
inline ss_typename_type_k pod_vector<T, A, SPACE>::iterator pod_vector<T, A, SPACE>::end()
#endif /* __STLSOFT_CF_FUNCTION_SIGNATURE_FULL_ARG_QUALIFICATION_REQUIRED */
{
    stlsoft_assert(is_valid_());

    return &m_buffer.data()[size()];
}

template<   ss_typename_param_k T
        ,   ss_typename_param_k A
        ,   ss_size_t           SPACE
        >
#ifdef __STLSOFT_CF_FUNCTION_SIGNATURE_FULL_ARG_QUALIFICATION_REQUIRED
inline ss_typename_type_k pod_vector<T, A, SPACE>::const_iterator pod_vector<T, A, SPACE>::end() const
#else /* ? __STLSOFT_CF_FUNCTION_SIGNATURE_FULL_ARG_QUALIFICATION_REQUIRED */
inline ss_typename_type_k pod_vector<T, A, SPACE>::const_iterator pod_vector<T, A, SPACE>::end() const
#endif /* __STLSOFT_CF_FUNCTION_SIGNATURE_FULL_ARG_QUALIFICATION_REQUIRED */
{
    stlsoft_assert(is_valid_());

    return &m_buffer.data()[size()];
}

#if defined(__STLSOFT_CF_BIDIRECTIONAL_ITERATOR_SUPPORT)
template<   ss_typename_param_k T
        ,   ss_typename_param_k A
        ,   ss_size_t           SPACE
        >
#ifdef __STLSOFT_CF_FUNCTION_SIGNATURE_FULL_ARG_QUALIFICATION_REQUIRED
inline ss_typename_type_k pod_vector<T, A, SPACE>::reverse_iterator pod_vector<T, A, SPACE>::rbegin()
#else /* ? __STLSOFT_CF_FUNCTION_SIGNATURE_FULL_ARG_QUALIFICATION_REQUIRED */
inline ss_typename_type_k pod_vector<T, A, SPACE>::reverse_iterator pod_vector<T, A, SPACE>::rbegin()
#endif /* __STLSOFT_CF_FUNCTION_SIGNATURE_FULL_ARG_QUALIFICATION_REQUIRED */
{
    stlsoft_assert(is_valid_());

    return reverse_iterator(end());
}

template<   ss_typename_param_k T
        ,   ss_typename_param_k A
        ,   ss_size_t           SPACE
        >
#ifdef __STLSOFT_CF_FUNCTION_SIGNATURE_FULL_ARG_QUALIFICATION_REQUIRED
inline ss_typename_type_k pod_vector<T, A, SPACE>::const_reverse_iterator pod_vector<T, A, SPACE>::rbegin() const
#else /* ? __STLSOFT_CF_FUNCTION_SIGNATURE_FULL_ARG_QUALIFICATION_REQUIRED */
inline ss_typename_type_k pod_vector<T, A, SPACE>::const_reverse_iterator pod_vector<T, A, SPACE>::rbegin() const
#endif /* __STLSOFT_CF_FUNCTION_SIGNATURE_FULL_ARG_QUALIFICATION_REQUIRED */
{
    stlsoft_assert(is_valid_());

    return const_reverse_iterator(end());
}

template<   ss_typename_param_k T
        ,   ss_typename_param_k A
        ,   ss_size_t           SPACE
        >
#ifdef __STLSOFT_CF_FUNCTION_SIGNATURE_FULL_ARG_QUALIFICATION_REQUIRED
inline ss_typename_type_k pod_vector<T, A, SPACE>::reverse_iterator pod_vector<T, A, SPACE>::rend()
#else /* ? __STLSOFT_CF_FUNCTION_SIGNATURE_FULL_ARG_QUALIFICATION_REQUIRED */
inline ss_typename_type_k pod_vector<T, A, SPACE>::reverse_iterator pod_vector<T, A, SPACE>::rend()
#endif /* __STLSOFT_CF_FUNCTION_SIGNATURE_FULL_ARG_QUALIFICATION_REQUIRED */
{
    stlsoft_assert(is_valid_());

    return reverse_iterator(begin());
}

template<   ss_typename_param_k T
        ,   ss_typename_param_k A
        ,   ss_size_t           SPACE
        >
#ifdef __STLSOFT_CF_FUNCTION_SIGNATURE_FULL_ARG_QUALIFICATION_REQUIRED
inline ss_typename_type_k pod_vector<T, A, SPACE>::const_reverse_iterator pod_vector<T, A, SPACE>::rend() const
#else /* ? __STLSOFT_CF_FUNCTION_SIGNATURE_FULL_ARG_QUALIFICATION_REQUIRED */
inline ss_typename_type_k pod_vector<T, A, SPACE>::const_reverse_iterator pod_vector<T, A, SPACE>::rend() const
#endif /* __STLSOFT_CF_FUNCTION_SIGNATURE_FULL_ARG_QUALIFICATION_REQUIRED */
{
    stlsoft_assert(is_valid_());

    return const_reverse_iterator(begin());
}
#endif /* __STLSOFT_CF_BIDIRECTIONAL_ITERATOR_SUPPORT */

// Attributes

template<   ss_typename_param_k T
        ,   ss_typename_param_k A
        ,   ss_size_t           SPACE
        >
#ifdef __STLSOFT_CF_FUNCTION_SIGNATURE_FULL_ARG_QUALIFICATION_REQUIRED
inline ss_typename_type_k pod_vector<T, A, SPACE>::size_type pod_vector<T, A, SPACE>::size() const
#else /* ? __STLSOFT_CF_FUNCTION_SIGNATURE_FULL_ARG_QUALIFICATION_REQUIRED */
inline ss_size_t pod_vector<T, A, SPACE>::size() const
#endif /* __STLSOFT_CF_FUNCTION_SIGNATURE_FULL_ARG_QUALIFICATION_REQUIRED */
{
    stlsoft_assert(is_valid_());

    return m_cItems;
}

template<   ss_typename_param_k T
        ,   ss_typename_param_k A
        ,   ss_size_t           SPACE
        >
#ifdef __STLSOFT_CF_FUNCTION_SIGNATURE_FULL_ARG_QUALIFICATION_REQUIRED
inline ss_typename_type_k pod_vector<T, A, SPACE>::size_type pod_vector<T, A, SPACE>::capacity() const
#else /* ? __STLSOFT_CF_FUNCTION_SIGNATURE_FULL_ARG_QUALIFICATION_REQUIRED */
inline ss_size_t pod_vector<T, A, SPACE>::capacity() const
#endif /* __STLSOFT_CF_FUNCTION_SIGNATURE_FULL_ARG_QUALIFICATION_REQUIRED */
{
    stlsoft_assert(is_valid_());

    return m_buffer.size();
}

template<   ss_typename_param_k T
        ,   ss_typename_param_k A
        ,   ss_size_t           SPACE
        >
#ifdef __STLSOFT_CF_FUNCTION_SIGNATURE_FULL_ARG_QUALIFICATION_REQUIRED
inline ss_typename_type_k pod_vector<T, A, SPACE>::size_type pod_vector<T, A, SPACE>::max_size() const
#else /* ? __STLSOFT_CF_FUNCTION_SIGNATURE_FULL_ARG_QUALIFICATION_REQUIRED */
inline ss_size_t pod_vector<T, A, SPACE>::max_size() const
#endif /* __STLSOFT_CF_FUNCTION_SIGNATURE_FULL_ARG_QUALIFICATION_REQUIRED */
{
    stlsoft_assert(is_valid_());

    return static_cast<size_type>(-1) / sizeof(value_type);
}

template<   ss_typename_param_k T
        ,   ss_typename_param_k A
        ,   ss_size_t           SPACE
        >
inline ss_bool_t pod_vector<T, A, SPACE>::empty() const
{
    stlsoft_assert(is_valid_());

    return 0 == size();
}

template<   ss_typename_param_k T
        ,   ss_typename_param_k A
        ,   ss_size_t           SPACE
        >
#ifdef __STLSOFT_CF_FUNCTION_SIGNATURE_FULL_ARG_QUALIFICATION_REQUIRED
inline ss_typename_type_k pod_vector<T, A, SPACE>::allocator_type pod_vector<T, A, SPACE>::get_allocator() const
#else /* ? __STLSOFT_CF_FUNCTION_SIGNATURE_FULL_ARG_QUALIFICATION_REQUIRED */
inline ss_typename_type_k pod_vector<T, A, SPACE>::allocator_type pod_vector<T, A, SPACE>::get_allocator() const
#endif /* __STLSOFT_CF_FUNCTION_SIGNATURE_FULL_ARG_QUALIFICATION_REQUIRED */
{
    stlsoft_assert(is_valid_());

    return m_buffer.get_allocator();
}

// Accessors
template<   ss_typename_param_k T
        ,   ss_typename_param_k A
        ,   ss_size_t           SPACE
        >
#ifdef __STLSOFT_CF_FUNCTION_SIGNATURE_FULL_ARG_QUALIFICATION_REQUIRED
inline ss_typename_type_k pod_vector<T, A, SPACE>::reference pod_vector<T, A, SPACE>::at(ss_typename_type_k pod_vector<T, A, SPACE>::size_type index)
#else /* ? __STLSOFT_CF_FUNCTION_SIGNATURE_FULL_ARG_QUALIFICATION_REQUIRED */
inline T &pod_vector<T, A, SPACE>::at(size_type index)
#endif /* __STLSOFT_CF_FUNCTION_SIGNATURE_FULL_ARG_QUALIFICATION_REQUIRED */
{
    stlsoft_assert(is_valid_());

    range_check_(index);

    stlsoft_assert(is_valid_());

    return m_buffer.data()[index];
}

template<   ss_typename_param_k T
        ,   ss_typename_param_k A
        ,   ss_size_t           SPACE
        >
#ifdef __STLSOFT_CF_FUNCTION_SIGNATURE_FULL_ARG_QUALIFICATION_REQUIRED
inline ss_typename_type_k pod_vector<T, A, SPACE>::const_reference pod_vector<T, A, SPACE>::at(ss_typename_type_k pod_vector<T, A, SPACE>::size_type index) const
#else /* ? __STLSOFT_CF_FUNCTION_SIGNATURE_FULL_ARG_QUALIFICATION_REQUIRED */
inline T const &pod_vector<T, A, SPACE>::at(size_type index) const
#endif /* __STLSOFT_CF_FUNCTION_SIGNATURE_FULL_ARG_QUALIFICATION_REQUIRED */
{
    stlsoft_assert(is_valid_());

    range_check_(index);

    stlsoft_assert(is_valid_());

    return m_buffer.data()[index];
}

template<   ss_typename_param_k T
        ,   ss_typename_param_k A
        ,   ss_size_t           SPACE
        >
#ifdef __STLSOFT_CF_FUNCTION_SIGNATURE_FULL_ARG_QUALIFICATION_REQUIRED
inline ss_typename_type_k pod_vector<T, A, SPACE>::reference pod_vector<T, A, SPACE>::operator [](ss_typename_type_k pod_vector<T, A, SPACE>::size_type index)
#else /* ? __STLSOFT_CF_FUNCTION_SIGNATURE_FULL_ARG_QUALIFICATION_REQUIRED */
inline T &pod_vector<T, A, SPACE>::operator [](size_type index)
#endif /* __STLSOFT_CF_FUNCTION_SIGNATURE_FULL_ARG_QUALIFICATION_REQUIRED */
{
    stlsoft_assert(is_valid_());

    // The index must be <= the size(). It is allowed to be equal to the size because
    // we must facilitate the taking of the end() element in order to specify ranges.
    stlsoft_message_assert("Requested index is out of range", !(size() < index));

    return m_buffer.data()[index];
}

template<   ss_typename_param_k T
        ,   ss_typename_param_k A
        ,   ss_size_t           SPACE
        >
#ifdef __STLSOFT_CF_FUNCTION_SIGNATURE_FULL_ARG_QUALIFICATION_REQUIRED
inline ss_typename_type_k pod_vector<T, A, SPACE>::const_reference pod_vector<T, A, SPACE>::operator [](ss_typename_type_k pod_vector<T, A, SPACE>::size_type index) const
#else /* ? __STLSOFT_CF_FUNCTION_SIGNATURE_FULL_ARG_QUALIFICATION_REQUIRED */
inline T const &pod_vector<T, A, SPACE>::operator [](size_type index) const
#endif /* __STLSOFT_CF_FUNCTION_SIGNATURE_FULL_ARG_QUALIFICATION_REQUIRED */
{
    stlsoft_assert(is_valid_());

    // The index must be <= the size(). It is allowed to be equal to the size because
    // we must facilitate the taking of the end() element in order to specify ranges.
    stlsoft_message_assert("Requested index is out of range", !(size() < index));

    return m_buffer.data()[index];
}

template<   ss_typename_param_k T
        ,   ss_typename_param_k A
        ,   ss_size_t           SPACE
        >
#ifdef __STLSOFT_CF_FUNCTION_SIGNATURE_FULL_ARG_QUALIFICATION_REQUIRED
inline ss_typename_type_k pod_vector<T, A, SPACE>::reference pod_vector<T, A, SPACE>::front()
#else /* ? __STLSOFT_CF_FUNCTION_SIGNATURE_FULL_ARG_QUALIFICATION_REQUIRED */
inline T &pod_vector<T, A, SPACE>::front()
#endif /* __STLSOFT_CF_FUNCTION_SIGNATURE_FULL_ARG_QUALIFICATION_REQUIRED */
{
    stlsoft_assert(is_valid_());

    stlsoft_message_assert("Range is empty!", 0 != size());

    return m_buffer.data()[0];
}

template<   ss_typename_param_k T
        ,   ss_typename_param_k A
        ,   ss_size_t           SPACE
        >
#ifdef __STLSOFT_CF_FUNCTION_SIGNATURE_FULL_ARG_QUALIFICATION_REQUIRED
inline ss_typename_type_k pod_vector<T, A, SPACE>::const_reference pod_vector<T, A, SPACE>::front() const
#else /* ? __STLSOFT_CF_FUNCTION_SIGNATURE_FULL_ARG_QUALIFICATION_REQUIRED */
inline T const &pod_vector<T, A, SPACE>::front() const
#endif /* __STLSOFT_CF_FUNCTION_SIGNATURE_FULL_ARG_QUALIFICATION_REQUIRED */
{
    stlsoft_assert(is_valid_());

    stlsoft_message_assert("Range is empty!", 0 != size());

    return m_buffer.data()[0];
}

template<   ss_typename_param_k T
        ,   ss_typename_param_k A
        ,   ss_size_t           SPACE
        >
#ifdef __STLSOFT_CF_FUNCTION_SIGNATURE_FULL_ARG_QUALIFICATION_REQUIRED
inline ss_typename_type_k pod_vector<T, A, SPACE>::reference pod_vector<T, A, SPACE>::back()
#else /* ? __STLSOFT_CF_FUNCTION_SIGNATURE_FULL_ARG_QUALIFICATION_REQUIRED */
inline T &pod_vector<T, A, SPACE>::back()
#endif /* __STLSOFT_CF_FUNCTION_SIGNATURE_FULL_ARG_QUALIFICATION_REQUIRED */
{
    stlsoft_assert(is_valid_());

    stlsoft_message_assert("Range is empty!", 0 != size());

    return m_buffer.data()[size() - 1];
}


template<   ss_typename_param_k T
        ,   ss_typename_param_k A
        ,   ss_size_t           SPACE
        >
#ifdef __STLSOFT_CF_FUNCTION_SIGNATURE_FULL_ARG_QUALIFICATION_REQUIRED
inline ss_typename_type_k pod_vector<T, A, SPACE>::const_reference pod_vector<T, A, SPACE>::back() const
#else /* ? __STLSOFT_CF_FUNCTION_SIGNATURE_FULL_ARG_QUALIFICATION_REQUIRED */
inline T const &pod_vector<T, A, SPACE>::back() const
#endif /* __STLSOFT_CF_FUNCTION_SIGNATURE_FULL_ARG_QUALIFICATION_REQUIRED */
{
    stlsoft_assert(is_valid_());

    stlsoft_message_assert("Range is empty!", 0 != size());

    return m_buffer.data()[size() - 1];
}

// Operations

template<   ss_typename_param_k T
        ,   ss_typename_param_k A
        ,   ss_size_t           SPACE
        >
inline void pod_vector<T, A, SPACE>::clear()
{
    stlsoft_assert(is_valid_());

    if(m_buffer.resize(0))
    {
        m_cItems = 0;
    }

    stlsoft_assert(is_valid_());
}

template<   ss_typename_param_k T
        ,   ss_typename_param_k A
        ,   ss_size_t           SPACE
        >
inline void pod_vector<T, A, SPACE>::swap(pod_vector<T, A, SPACE> &rhs)
{
    stlsoft_assert(is_valid_());

    m_buffer.swap(rhs.m_buffer);
    swap_(m_cItems, rhs.m_cItems);

    stlsoft_assert(is_valid_());
}

template<   ss_typename_param_k T
        ,   ss_typename_param_k A
        ,   ss_size_t           SPACE
        >
#ifdef __STLSOFT_CF_FUNCTION_SIGNATURE_FULL_ARG_QUALIFICATION_REQUIRED
inline void pod_vector<T, A, SPACE>::reserve(ss_typename_type_k pod_vector<T, A, SPACE>::size_type cItems) /* stlsoft_throw_1(stlsoft_ns_qual_std(bad_alloc) ) */
#else /* ? __STLSOFT_CF_FUNCTION_SIGNATURE_FULL_ARG_QUALIFICATION_REQUIRED */
inline void pod_vector<T, A, SPACE>::reserve(size_type cItems) /* stlsoft_throw_1(stlsoft_ns_qual_std(bad_alloc) ) */
#endif /* __STLSOFT_CF_FUNCTION_SIGNATURE_FULL_ARG_QUALIFICATION_REQUIRED */
{
    stlsoft_assert(is_valid_());

    // We do not follow vector's much maligned example and refuse to truncate, although
    // we only do so if the requested size is 0.
    if( 0 == cItems ||
        cItems > size())
    {
        m_buffer.resize(cItems);
    }

    stlsoft_assert(is_valid_());
}

template<   ss_typename_param_k T
        ,   ss_typename_param_k A
        ,   ss_size_t           SPACE
        >
#ifdef __STLSOFT_CF_FUNCTION_SIGNATURE_FULL_ARG_QUALIFICATION_REQUIRED
inline void pod_vector<T, A, SPACE>::resize(ss_typename_type_k pod_vector<T, A, SPACE>::size_type cItems) /* stlsoft_throw_1(stlsoft_ns_qual_std(bad_alloc) ) */
#else /* ? __STLSOFT_CF_FUNCTION_SIGNATURE_FULL_ARG_QUALIFICATION_REQUIRED */
inline void pod_vector<T, A, SPACE>::resize(size_type cItems) /* stlsoft_throw_1(stlsoft_ns_qual_std(bad_alloc) ) */
#endif /* __STLSOFT_CF_FUNCTION_SIGNATURE_FULL_ARG_QUALIFICATION_REQUIRED */
{
    stlsoft_assert(is_valid_());

    // We only resize if we're getting bigger, or the requested size is 0
    if( (0 != cItems &&
        cItems < size()) ||
        m_buffer.resize(cItems))
    {
        m_cItems = cItems;
    }

    stlsoft_assert(is_valid_());
}

template<   ss_typename_param_k T
        ,   ss_typename_param_k A
        ,   ss_size_t           SPACE
        >
#ifdef __STLSOFT_CF_FUNCTION_SIGNATURE_FULL_ARG_QUALIFICATION_REQUIRED
inline void pod_vector<T, A, SPACE>::resize(ss_typename_type_k pod_vector<T, A, SPACE>::size_type cItems, ss_typename_type_k pod_vector<T, A, SPACE>::value_type const &value) /* stlsoft_throw_1(stlsoft_ns_qual_std(bad_alloc) ) */
#else /* ? __STLSOFT_CF_FUNCTION_SIGNATURE_FULL_ARG_QUALIFICATION_REQUIRED */
inline void pod_vector<T, A, SPACE>::resize(size_type cItems, ss_typename_type_k pod_vector<T, A, SPACE>::value_type const &value) /* stlsoft_throw_1(stlsoft_ns_qual_std(bad_alloc) ) */
#endif /* __STLSOFT_CF_FUNCTION_SIGNATURE_FULL_ARG_QUALIFICATION_REQUIRED */
{
    stlsoft_assert(is_valid_());

    // We only resize if we're getting bigger, or the requested size is 0
    if( 0 != cItems &&
        cItems < m_cItems)
    {
        m_cItems = cItems;
    }
    else if(m_cItems < cItems)
    {
        if(m_buffer.resize(cItems))
        {
            pod_fill_n(end(), cItems - m_cItems, value);
            m_cItems = cItems;
        }
    }

    stlsoft_assert(is_valid_());
}

template<   ss_typename_param_k T
        ,   ss_typename_param_k A
        ,   ss_size_t           SPACE
        >
#ifdef __STLSOFT_CF_FUNCTION_SIGNATURE_FULL_ARG_QUALIFICATION_REQUIRED
inline void pod_vector<T, A, SPACE>::push_back(ss_typename_type_k pod_vector<T, A, SPACE>::value_type const &value)
#else /* ? __STLSOFT_CF_FUNCTION_SIGNATURE_FULL_ARG_QUALIFICATION_REQUIRED */
inline void pod_vector<T, A, SPACE>::push_back(ss_typename_type_k pod_vector<T, A, SPACE>::value_type const &value)
#endif /* __STLSOFT_CF_FUNCTION_SIGNATURE_FULL_ARG_QUALIFICATION_REQUIRED */
{
    stlsoft_assert(is_valid_());

    insert(end(), value);

    stlsoft_assert(is_valid_());
}

template<   ss_typename_param_k T
        ,   ss_typename_param_k A
        ,   ss_size_t           SPACE
        >
inline void pod_vector<T, A, SPACE>::pop_back()
{
    stlsoft_assert(is_valid_());

    stlsoft_message_assert("No elements to pop", size() > 0);

    if(0 == --m_cItems)
    {
        m_buffer.resize(0);
    }

    stlsoft_assert(is_valid_());
}

template<   ss_typename_param_k T
        ,   ss_typename_param_k A
        ,   ss_size_t           SPACE
        >
#ifdef __STLSOFT_CF_FUNCTION_SIGNATURE_FULL_ARG_QUALIFICATION_REQUIRED
inline void pod_vector<T, A, SPACE>::assign(ss_typename_type_k pod_vector<T, A, SPACE>::const_iterator first, ss_typename_type_k pod_vector<T, A, SPACE>::const_iterator last)
#else /* ? __STLSOFT_CF_FUNCTION_SIGNATURE_FULL_ARG_QUALIFICATION_REQUIRED */
inline void pod_vector<T, A, SPACE>::assign(const_iterator first, const_iterator last)
#endif /* __STLSOFT_CF_FUNCTION_SIGNATURE_FULL_ARG_QUALIFICATION_REQUIRED */
{
    stlsoft_assert(is_valid_());

// NOTE: This will be optimised by testing the contents of this and the rhs

    class_type  temp(first, last);

    temp.swap(*this);

    stlsoft_assert(is_valid_());
}

template<   ss_typename_param_k T
        ,   ss_typename_param_k A
        ,   ss_size_t           SPACE
        >
#ifdef __STLSOFT_CF_FUNCTION_SIGNATURE_FULL_ARG_QUALIFICATION_REQUIRED
inline void pod_vector<T, A, SPACE>::assign(ss_typename_type_k pod_vector<T, A, SPACE>::size_type cItems, ss_typename_type_k pod_vector<T, A, SPACE>::value_type const &value /* = value_type() */)
#else /* ? __STLSOFT_CF_FUNCTION_SIGNATURE_FULL_ARG_QUALIFICATION_REQUIRED */
inline void pod_vector<T, A, SPACE>::assign(size_type cItems, value_type const &value /* = value_type() */)
#endif /* __STLSOFT_CF_FUNCTION_SIGNATURE_FULL_ARG_QUALIFICATION_REQUIRED */
{
    stlsoft_assert(is_valid_());

// NOTE: This will be optimised by testing the contents of this and the rhs

    class_type  temp(cItems, value);

    temp.swap(*this);

    stlsoft_assert(is_valid_());
}

template<   ss_typename_param_k T
        ,   ss_typename_param_k A
        ,   ss_size_t           SPACE
        >
#ifdef __STLSOFT_CF_FUNCTION_SIGNATURE_FULL_ARG_QUALIFICATION_REQUIRED
inline ss_typename_type_k pod_vector<T, A, SPACE>::iterator pod_vector<T, A, SPACE>::insert(ss_typename_type_k pod_vector<T, A, SPACE>::iterator it, ss_typename_type_k pod_vector<T, A, SPACE>::value_type const &value /* = value_type() */)
#else /* ? __STLSOFT_CF_FUNCTION_SIGNATURE_FULL_ARG_QUALIFICATION_REQUIRED */
inline ss_typename_type_k pod_vector<T, A, SPACE>::iterator pod_vector<T, A, SPACE>::insert(iterator it, value_type const &value /* = value_type() */)
#endif /* __STLSOFT_CF_FUNCTION_SIGNATURE_FULL_ARG_QUALIFICATION_REQUIRED */
{
    stlsoft_assert(is_valid_());

    stlsoft_assert(!(end() < it));
    stlsoft_assert(!(it < begin()));

    size_type   index = it - begin();

    insert(it, 1, value);

    stlsoft_assert(is_valid_());

    return begin() + index;
}

template<   ss_typename_param_k T
        ,   ss_typename_param_k A
        ,   ss_size_t           SPACE
        >
#ifdef __STLSOFT_CF_FUNCTION_SIGNATURE_FULL_ARG_QUALIFICATION_REQUIRED
inline void pod_vector<T, A, SPACE>::insert(ss_typename_type_k pod_vector<T, A, SPACE>::iterator it, ss_typename_type_k pod_vector<T, A, SPACE>::size_type cItems, ss_typename_type_k pod_vector<T, A, SPACE>::value_type const &value)
#else /* ? __STLSOFT_CF_FUNCTION_SIGNATURE_FULL_ARG_QUALIFICATION_REQUIRED */
inline void pod_vector<T, A, SPACE>::insert(iterator it, size_type cItems, value_type const &value)
#endif /* __STLSOFT_CF_FUNCTION_SIGNATURE_FULL_ARG_QUALIFICATION_REQUIRED */
{
    stlsoft_assert(is_valid_());

    stlsoft_assert(!(end() < it));
    stlsoft_assert(!(it < begin()));

    size_type   curr_size   =   size();
    size_type   index       =   it - begin();

    if(resize_(size() + cItems))
    {
        size_type cMove = curr_size - index;

        // The resize_ may have invalidated the iterator(s)!!
        it = begin() + index;

        // Move the existing ones up out of the way
        pod_move_n(&it[cItems], &it[0], cMove);

        // And insert the new ones
        pod_fill_n(m_buffer.data() + index, cItems, value);
    }

    stlsoft_assert(is_valid_());
}

template<   ss_typename_param_k T
        ,   ss_typename_param_k A
        ,   ss_size_t           SPACE
        >
#ifdef __STLSOFT_CF_FUNCTION_SIGNATURE_FULL_ARG_QUALIFICATION_REQUIRED
inline void pod_vector<T, A, SPACE>::insert(ss_typename_type_k pod_vector<T, A, SPACE>::iterator it, ss_typename_type_k pod_vector<T, A, SPACE>::const_iterator first, ss_typename_type_k pod_vector<T, A, SPACE>::const_iterator last)
#else /* ? __STLSOFT_CF_FUNCTION_SIGNATURE_FULL_ARG_QUALIFICATION_REQUIRED */
inline void pod_vector<T, A, SPACE>::insert(iterator it, const_iterator first, const_iterator last)
#endif /* __STLSOFT_CF_FUNCTION_SIGNATURE_FULL_ARG_QUALIFICATION_REQUIRED */
{
    stlsoft_assert(is_valid_());

    stlsoft_assert(!(end() < it));
    stlsoft_assert(!(it < begin()));

    size_type   curr_size   =   size();
    size_type   index       =   it - begin();
    size_type   cItems      =   last - first;

    if(resize_(size() + cItems))
    {
        size_type cMove = curr_size - index;

        // The resize_ may have invalidated the iterator(s)!!
        it = begin() + index;

        // Move the existing ones up out of the way
        pod_move_n(&it[cItems], &it[0], cMove);

        // And insert the new ones
        pod_copy_n(m_buffer.data(), first, cItems);
    }

    stlsoft_assert(is_valid_());
}

template<   ss_typename_param_k T
        ,   ss_typename_param_k A
        ,   ss_size_t           SPACE
        >
#ifdef __STLSOFT_CF_FUNCTION_SIGNATURE_FULL_ARG_QUALIFICATION_REQUIRED
inline ss_typename_type_k pod_vector<T, A, SPACE>::iterator pod_vector<T, A, SPACE>::erase(ss_typename_type_k pod_vector<T, A, SPACE>::iterator it)
#else /* ? __STLSOFT_CF_FUNCTION_SIGNATURE_FULL_ARG_QUALIFICATION_REQUIRED */
inline ss_typename_type_k pod_vector<T, A, SPACE>::iterator pod_vector<T, A, SPACE>::erase(iterator it)
#endif /* __STLSOFT_CF_FUNCTION_SIGNATURE_FULL_ARG_QUALIFICATION_REQUIRED */
{
    stlsoft_assert(is_valid_());

    stlsoft_assert(it < end());
    stlsoft_assert(!(it < begin()));

    if(it != end() - 1)
    {
        pod_move_n(&it[0], &it[1], 1);
    }

    if(0 == --m_cItems)
    {
        m_buffer.resize(0);
    }

    stlsoft_assert(is_valid_());

    return it;
}

template<   ss_typename_param_k T
        ,   ss_typename_param_k A
        ,   ss_size_t           SPACE
        >
#ifdef __STLSOFT_CF_FUNCTION_SIGNATURE_FULL_ARG_QUALIFICATION_REQUIRED
inline ss_typename_type_k pod_vector<T, A, SPACE>::iterator pod_vector<T, A, SPACE>::erase(ss_typename_type_k pod_vector<T, A, SPACE>::iterator first, ss_typename_type_k pod_vector<T, A, SPACE>::iterator last)
#else /* ? __STLSOFT_CF_FUNCTION_SIGNATURE_FULL_ARG_QUALIFICATION_REQUIRED */
inline ss_typename_type_k pod_vector<T, A, SPACE>::iterator pod_vector<T, A, SPACE>::erase(iterator first, iterator last)
#endif /* __STLSOFT_CF_FUNCTION_SIGNATURE_FULL_ARG_QUALIFICATION_REQUIRED */
{
    stlsoft_assert(is_valid_());

    stlsoft_assert(first < end());
    stlsoft_assert(!(first < begin()));
    stlsoft_assert(!(end() < last));
    stlsoft_assert(!(last < begin()));

    size_type   curr_size   =   size();
    size_type   index_first =   first - begin();
    size_type   index_last  =   last - begin();
    size_type   cItems      =   last - first;
    size_type   cMove       =   curr_size - index_last;

    // Move the remaining ones down
    pod_move_n(&first[0], &last[0], cMove);

    resize(curr_size - cItems);

    stlsoft_assert(is_valid_());

    return begin() + index_first;
}

#endif /* !__STLSOFT_DOCUMENTATION_SKIP_SECTION */

/* ////////////////////////////////////////////////////////////////////////// */

#ifndef _STLSOFT_NO_NAMESPACE
} // namespace stlsoft
#endif /* _STLSOFT_NO_NAMESPACE */

/* ////////////////////////////////////////////////////////////////////////// */

#endif /* !STLSOFT_INCL_H_STLSOFT_POD_VECTOR */

/* ////////////////////////////////////////////////////////////////////////// */
