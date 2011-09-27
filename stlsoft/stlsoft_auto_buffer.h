/* /////////////////////////////////////////////////////////////////////////////
 * File:        stlsoft_auto_buffer.h (originally MTLocBfr.h, ::SynesisStl)
 *
 * Purpose:     Contains the auto_buffer template class.
 *
 * Created:     19th January 2002
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


/// \file stlsoft_auto_buffer.h
///
/// Contains the auto_buffer template class.

#ifndef STLSOFT_INCL_H_STLSOFT_AUTO_BUFFER
#define STLSOFT_INCL_H_STLSOFT_AUTO_BUFFER

#ifndef __STLSOFT_DOCUMENTATION_SKIP_SECTION
# define STLSOFT_VER_H_STLSOFT_AUTO_BUFFER_MAJOR     2
# define STLSOFT_VER_H_STLSOFT_AUTO_BUFFER_MINOR     0
# define STLSOFT_VER_H_STLSOFT_AUTO_BUFFER_REVISION  1
# define STLSOFT_VER_H_STLSOFT_AUTO_BUFFER_EDIT      86
#endif /* !__STLSOFT_DOCUMENTATION_SKIP_SECTION */

/* /////////////////////////////////////////////////////////////////////////////
 * Includes
 */

#ifndef STLSOFT_INCL_H_STLSOFT
# include "stlsoft.h"                   // Include the STLSoft root header
#endif /* !STLSOFT_INCL_H_STLSOFT */
#ifdef _STLSOFT_AUTO_BUFFER_ALLOW_UDT
# define _STLSOFT_AUTO_BUFFER_ALLOW_NON_POD
# ifdef _STLSOFT_COMPILE_VERBOSE
#  pragma message("_STLSOFT_AUTO_BUFFER_ALLOW_UDT is deprecated. Use _STLSOFT_AUTO_BUFFER_ALLOW_NON_POD instead")
# endif /* _STLSOFT_COMPILE_VERBOSE */
#endif /* _STLSOFT_AUTO_BUFFER_ALLOW_UDT */
#if defined(__STLSOFT_CF_BIDIRECTIONAL_ITERATOR_SUPPORT)
# ifndef STLSOFT_INCL_H_STLSOFT_ITERATOR
#  include "stlsoft_iterator.h"         // reverse_iterator_base
# endif /* !STLSOFT_INCL_H_STLSOFT_ITERATOR */
#endif /* __STLSOFT_CF_BIDIRECTIONAL_ITERATOR_SUPPORT */
#ifndef _STLSOFT_AUTO_BUFFER_ALLOW_NON_POD
# ifndef STLSOFT_INCL_H_CONSTRAINTS
#  include "stlsoft_constraints.h"      // Include the STLSoft constraints header
# endif /* !STLSOFT_INCL_H_CONSTRAINTS */
#endif /* _STLSOFT_AUTO_BUFFER_ALLOW_NON_POD */
#ifndef STLSOFT_INCL_H_STLSOFT_SIMPLE_ALGORITHMS
# include "stlsoft_simple_algorithms.h" //
#endif /* !STLSOFT_INCL_H_STLSOFT_SIMPLE_ALGORITHMS */
#ifdef STLSOFT_UNITTEST
# ifndef STLSOFT_INCL_H_STLSOFT_NEW_ALLOCATOR
#  include "stlsoft_new_allocator.h"    // new_allocator
# endif /* !STLSOFT_INCL_H_STLSOFT_NEW_ALLOCATOR */
# include <numeric>                     // accumulate
#endif /* STLSOFT_UNITTEST */

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

// class auto_buffer
//
/// \brief This class provides an efficient variable automatic buffer
///
/// This class provides an efficient replacement for dynamic memory block
/// allocation when the block size generally falls under a certain limit. In
/// such cases, significant performance benefits can be achieved by using an
/// instance of a parameterisation of auto_buffer, whose size parameter SPACE
/// is set to a level to cater for most of the requested sizes. Only where the
/// size of the buffer needs to be larger than this limit does an allocation
/// occur from the heap/free-store via the given allocator.
///
/// \note The buffer works correctly whether the given allocator throws an exception on allocation failure, or returns NULL.
///
/// \param T The type of the elements in the array
/// \param A The allocator type
/// \param SPACE The number of elements in the array. With translators that support default template arguments, this is defaulted to 256

template<   ss_typename_param_k T
        ,   ss_typename_param_k A
#ifdef __STLSOFT_CF_TEMPLATE_CLASS_DEFAULT_FUNDAMENTAL_ARGUMENT_SUPPORT
# if !defined(__STLSOFT_COMPILER_IS_BORLAND) || \
     __BORLANDC__ < 0x0551
        ,   ss_size_t           SPACE = 256
# else /* ? compiler */
        ,   ss_size_t           space = 256
# endif /* compiler */
#else
# if !defined(__STLSOFT_COMPILER_IS_BORLAND) || \
     __BORLANDC__ < 0x0551
        ,   ss_size_t           SPACE /* = 256 */
# else /* ? compiler */
        ,   ss_size_t           space /* = 256 */
# endif /* compiler */
#endif /* __STLSOFT_CF_TEMPLATE_CLASS_DEFAULT_FUNDAMENTAL_ARGUMENT_SUPPORT */
        >
class auto_buffer
#if !defined(STLSOFT_CF_ALLOCATOR_BASE_EXPENSIVE)
    : protected A
#endif /* !STLSOFT_CF_ALLOCATOR_BASE_EXPENSIVE */
{
/// \name Types
//@{
public:
    /// The value type
    typedef T                       value_type;
    /// The allocator type
    typedef A                       allocator_type;
#if !defined(__STLSOFT_COMPILER_IS_BORLAND) || \
    __BORLANDC__ < 0x0551
    enum
    {
        /// The number of items in the internal buffer
        space = SPACE
    };
#endif /* compiler */
    /// The type of the current parameterisation
    typedef auto_buffer<T, A, space>                            class_type;
    /// The reference type
    typedef ss_typename_type_k allocator_type::reference        reference;
    /// The non-mutable (const) reference type
    typedef ss_typename_type_k allocator_type::const_reference  const_reference;
    /// The pointer type
    typedef ss_typename_type_k allocator_type::pointer          pointer;
    /// The non-mutable (const) pointer type
    typedef ss_typename_type_k allocator_type::const_pointer    const_pointer;
    /// The size type
    typedef ss_size_t                                           size_type;
    /// The difference type
    typedef ss_ptrdiff_t                                        difference_type;
#if !defined(__STLSOFT_CF_BIDIRECTIONAL_ITERATOR_SUPPORT)
    /// The iterator type
    typedef value_type                                          *iterator;
    /// The non-mutable (const) iterator type
    typedef value_type const                                    *const_iterator;
#else /* ? !__STLSOFT_CF_BIDIRECTIONAL_ITERATOR_SUPPORT */
    /// The iterator type
    typedef
# if !defined(__STLSOFT_COMPILER_IS_BORLAND)
           ss_typename_type_k
# endif /* compiler */
                       pointer_iterator <   value_type
                                        ,   pointer
                                        ,   reference
                                        >::iterator_type    iterator;
    /// The non-mutating (const) iterator type
    typedef
# if !defined(__STLSOFT_COMPILER_IS_BORLAND)
         ss_typename_type_k
# endif /* __STLSOFT_COMPILER_IS_BORLAND */
                       pointer_iterator <   value_type const
                                        ,   const_pointer
                                        ,   const_reference
                                        >::iterator_type    const_iterator;

    /// The mutating (non-const) reverse iterator type
    typedef reverse_iterator_base       <   iterator
                                        ,   value_type
                                        ,   reference
                                        ,   pointer
                                        ,   difference_type
                                        >                   reverse_iterator;

    /// The non-mutating (const) reverse iterator type
    typedef const_reverse_iterator_base <   const_iterator
                                        ,   value_type const
                                        ,   const_reference
                                        ,   const_pointer
                                        ,   difference_type
                                        >                   const_reverse_iterator;
#endif /* !__STLSOFT_CF_BIDIRECTIONAL_ITERATOR_SUPPORT */
//@}

/// \name Implementation
//@{
private:
    pointer allocate_(size_type cItems, void *hint)
    {
#ifdef __STLSOFT_CF_ALLOCATOR_ALLOCATE_HAS_HINT
        return get_allocator().allocate(cItems, hint);
#else
        STLSOFT_SUPPRESS_UNUSED(hint);

        return get_allocator().allocate(cItems);
#endif /* __STLSOFT_CF_ALLOCATOR_ALLOCATE_HAS_HINT */
    }

    pointer allocate_(size_type cItems)
    {
#ifdef __STLSOFT_CF_ALLOCATOR_ALLOCATE_HAS_HINT
        return get_allocator().allocate(cItems, NULL);
#else
        return get_allocator().allocate(cItems);
#endif /* __STLSOFT_CF_ALLOCATOR_ALLOCATE_HAS_HINT */
    }

    void deallocate_(pointer p, size_type cItems)
    {
        stlsoft_assert(NULL != p);

#ifdef __STLSOFT_CF_ALLOCATOR_DEALLOCATE_HAS_OBJECTCOUNT
        get_allocator().deallocate(p, cItems);
#else
        STLSOFT_SUPPRESS_UNUSED(cItems);

        get_allocator().deallocate(p);
#endif /* __STLSOFT_CF_ALLOCATOR_DEALLOCATE_HAS_OBJECTCOUNT */
    }

    pointer reallocate_(pointer p, size_type cItems, size_type cNewItems)
    {
        pointer new_p   =   allocate_(cNewItems, p);

        // This test is needed, since some allocators may not throw
        // bad_alloc
        if(NULL != new_p)
        {
            block_copy(new_p, p, cItems);

            deallocate_(p, cItems);
        }

        return new_p;
    }
protected:
    static void block_copy(pointer dest, const_pointer src, size_type cItems)
    {
        pod_copy_n(dest, src, cItems);
    }
    static void block_set(pointer dest, size_type cItems, const_reference value)
    {
        pod_fill_n(dest, cItems, value);
    }
//@}

/// \name Construction
//@{
public:
    /// \brief Constructs an auto_buffer with the given number of elements
    ///
    /// Constructs an auto_buffer with the given number of elements. If the
    /// allocation fails by throwing an exception, that exception is passed
    /// through to the caller. If allocation fails by returning a null
    /// pointer the auto_buffer instance is correctly constructed, and the
    /// \link #size size() \endlink method returns 0.
    ///
    /// \see \link #size size() \endlink
    /// \param cItems The number of items in the constructed instance
    ss_explicit_k auto_buffer(size_type cItems)
        : m_buffer((space < cItems) ? allocate_(cItems) : const_cast<pointer>(&m_internal[0]))
        , m_cItems((NULL != m_buffer) ? cItems : 0)
    {
        // Can't create one with an empty buffer. Though such is not legal
        // it is supported by some compilers, so we must ensure it cannot be
        // so
        stlsoft_static_assert(0 != space);

        // These assertions ensure that the member ordering is not
        // changed, invalidating the initialisation logic of m_buffer and
        // m_cItems. The runtime assert is included for those compilers that
        // do not implement compile-time asserts.
        stlsoft_static_assert(stlsoft_raw_offsetof(class_type, m_buffer) < stlsoft_raw_offsetof(class_type, m_cItems));
#if defined(__STLSOFT_COMPILER_IS_WATCOM)
        stlsoft_assert(stlsoft_raw_offsetof(class_type, m_buffer) < stlsoft_raw_offsetof(class_type, m_cItems));
#else
        stlsoft_message_assert("m_buffer must be before m_cItems in the auto_buffer definition", stlsoft_raw_offsetof(class_type, m_buffer) < stlsoft_raw_offsetof(class_type, m_cItems));
#endif /* !__STLSOFT_COMPILER_IS_WATCOM */

#ifndef _STLSOFT_AUTO_BUFFER_ALLOW_NON_POD
        // Use the must_be_pod constraint to ensure that
        // no type is managed in auto_buffer which would result in
        // dangerous mismanagement of the lifetime of its instances.
        //
        // Preprocessor specification of _STLSOFT_AUTO_BUFFER_ALLOW_NON_POD
        // prevents this, but the natural rules of the language will
        // still prevent non POD types being placed in m_internal[].
        stlsoft_constraint_must_be_pod(value_type);
#endif /* _STLSOFT_AUTO_BUFFER_ALLOW_NON_POD */
    }
    /// \brief Releases the allocated element array
    ///
    /// Releases any allocated memory. If the internal memory buffer was
    /// used, then nothing is done, otherwise the allocated memory is
    /// returned to the allocator.
#if defined(__STLSOFT_CF_EXCEPTION_SIGNATURE_SUPPORT)
    ~auto_buffer()
#else /* ? compiler */
    ~auto_buffer() stlsoft_throw_0()
#endif /* compiler */
    {
        if(space < m_cItems)
        {
            stlsoft_assert(NULL != m_buffer);

            deallocate_(m_buffer, m_cItems);
        }
    }
//@}

/// \name Operations
//@{
public:
    /// \brief Expands or contracts the number of items in the buffer
    ///
    /// \param cItems The number of items to change in the buffer
    /// \return Returns \c true if successful. Function failure occurs when sufficient storage for the
    /// requested items cannot be allocated. In that case, std::bad_alloc will be throw for allocators
    /// that support it, otherwise the function will return \c false. In either case, the original
    /// storage and contents of the buffer will remain unchanged.
    ///
    /// \note When reducing the number of elements, the implementation favours speed above memory
    /// consumption. If the new item size is still larger than the internal storage size
    /// (\c internal_size()) then the heap allocated block will not be changed (i.e. it will not be
    /// exchanged for a smaller block).
    ss_bool_t resize(size_type cItems)
    {
        // There are four changes possible:
        //
        // 1. Expansion within the internal buffer
        // 2. Contraction within the internal buffer
        // 3. Expansion from the internal buffer to an allocated buffer
        // 4. Contraction from an allocated buffer to the internal buffer
        // 5. Expansion from the allocated buffer to another allocated buffer
        // 6. Contraction from the allocated buffer to another allocated buffer

        if(m_cItems < cItems)
        {
            // Expansion; cases 1, 3 & 5

            if(space < m_cItems)
            {
                // Current buffer is allocated: case 5
                pointer new_buffer  =   reallocate_(m_buffer, m_cItems, cItems);

                // Still test for NULL here, since some allocators will
                // not throw bad_alloc.
                if(NULL == new_buffer)
                {
                    return false;
                }

                // Now repoint to the new buffer
                m_buffer = new_buffer;
            }
            else
            {
                // Expanding from internal buffer; cases 1 & 3

                if(space < cItems)
                {
                    // Expanding to allocated buffer; case 3

                    pointer new_buffer = allocate_(cItems);

                    // Still test for NULL here, since some allocators will
                    // not throw bad_alloc.
                    if(NULL == new_buffer)
                    {
                        return false;
                    }

                    if(0 != m_cItems)
                    {
                        block_copy(new_buffer, m_buffer, m_cItems);
                    }
                    m_buffer = new_buffer;
                }
                else
                {
                    // Expanding to internal buffer; case 1

                    // Nothing to do
                    stlsoft_assert(!(space < cItems));
                }
            }
        }
        else
        {
            // Contraction; cases 2, 4 & 6

            if(space < m_cItems)
            {
                // Current buffer is allocated: cases 4 & 6

                if(space < cItems)
                {
                    // Contracting within allocated buffer; case 6

                    // Nothing to do
                    stlsoft_assert(space < cItems);
                }
                else
                {
                    // Contracting back to internal; case 4

                    if(0 != cItems)
                    {
                        block_copy(const_cast<pointer>(&m_internal[0]), m_buffer, cItems);
                    }

                    deallocate_(m_buffer, m_cItems);

                    m_buffer = const_cast<pointer>(&m_internal[0]);
                }
            }
            else
            {
                // Current buffer is internal; case 2

                // Nothing to do
                stlsoft_assert(!(space < cItems));
            }
        }

        m_cItems = cItems;

        return true;
    }

    /// \brief Swaps contents with the given buffer
    ///
    /// \note This method is only constant time when the memory for two buffers
    /// has been acquired via the allocator. Otherwise, it will depend on the
    /// costs of exchanging the memory
    void swap(class_type &rhs)
    {
        if( space < m_cItems &&
            space < rhs.m_cItems)
        {
            // Both are allocated, so just swap them
            swap_(m_buffer, rhs.m_buffer);
        }
        else if(space < m_cItems)
        {
            // *this is allocated on the heap, rhs is using m_internal

            // 1. Copy the contents of rhs.m_internal to this->m_internal
            block_copy(&m_internal[0], &rhs.m_internal[0], rhs.m_cItems);

            // 2. Move m_buffer from *this to rhs
            rhs.m_buffer = m_buffer;

            // 3. Tell *this to use its internal buffer
            m_buffer = &m_internal[0];
        }
        else if(space < rhs.m_cItems)
        {
            // This is a lazy cheat.
            rhs.swap(*this);

            return;
        }
        else
        {
            // Both are using internal buffers, so we exchange the contents
            value_type  t[space];

            block_copy(&t[0], &rhs.m_internal[0], rhs.m_cItems);
            block_copy(&rhs.m_internal[0], &m_internal[0], m_cItems);
            block_copy(&m_internal[0], &t[0], rhs.m_cItems);
        }

        swap_(m_cItems, rhs.m_cItems);
    }
//@}

/// \name Operators
//@{
public:
    // Note: The following two const and non-const implicit conversion
    // operators are correctly implemented. However, GCC will pedantically
    // give a verbose warning describing its having selected one over the
    // other, and this is, in current versions of the compiler, not
    // suppressable. The warnings must, alas, simply be ignored.

    /// \brief An implicit conversion to a pointer to the start of the element array
    operator pointer ()
    {
        return m_buffer;
    }

#ifdef _STLSOFT_AUTO_BUFFER_ALLOW_CONST_CONVERSION_OPERATOR
    /// \brief An implicit conversion to a pointer-to-const to the start of the element array
    operator const_pointer () const
    {
        return m_buffer;
    }
#endif /* _STLSOFT_AUTO_BUFFER_ALLOW_CONST_CONVERSION_OPERATOR */
//@}

/// \name Accessors
/// @{
public:
    /// \brief Returns a pointer to the element array
    pointer data()
    {
        return m_buffer;
    }
    /// \brief Returns a pointer-to-const to the element array
    const_pointer data() const
    {
        return m_buffer;
    }
//@}

/// \name Iteration
//@{
public:
    /// \brief Returns a non-mutating iterator representing the start of the sequence
    const_iterator begin() const
    {
        return m_buffer;
    }
    /// \brief Returns a non-mutating iterator representing the end of the sequence
    ///
    /// \note In the case where memory allocation has failed in the context
    /// where exceptions are not thrown for allocation failure, this method will
    /// return the same value as begin(). Hence, operations on the <i>empty</i>
    /// auto_buffer<> instance will be safe if made in respect of the range
    /// defined by [begin(), end()).
    const_iterator end() const
    {
        return m_buffer + m_cItems;
    }

    /// \brief Returns a mutable iterator representing the start of the sequence
    iterator begin()
    {
        return m_buffer;
    }
    /// \brief Returns a mutable iterator representing the end of the sequence
    ///
    /// \note In the case where memory allocation has failed in the context
    /// where exceptions are not thrown for allocation failure, this method will
    /// return the same value as begin(). Hence, operations on the <i>empty</i>
    /// auto_buffer<> instance will be safe if made in respect of the range
    /// defined by [begin(), end()).
    iterator end()
    {
        return m_buffer + m_cItems;
    }

#if defined(__STLSOFT_CF_BIDIRECTIONAL_ITERATOR_SUPPORT)
    /// Begins the reverse iteration
    ///
    /// \return An iterator representing the start of the reverse sequence
    const_reverse_iterator rbegin() const
    {
        return const_reverse_iterator(end());
    }
    /// Ends the reverse iteration
    ///
    /// \return An iterator representing the end of the reverse sequence
    const_reverse_iterator rend() const
    {
        return const_reverse_iterator(begin());
    }
    /// Begins the reverse iteration
    ///
    /// \return An iterator representing the start of the reverse sequence
    reverse_iterator  rbegin()
    {
        return reverse_iterator(end());
    }
    /// Ends the reverse iteration
    ///
    /// \return An iterator representing the end of the reverse sequence
    reverse_iterator  rend()
    {
        return reverse_iterator(begin());
    }
#endif /* __STLSOFT_CF_BIDIRECTIONAL_ITERATOR_SUPPORT */

//@}

/// \name Attributes
//@{
public:
    /// \brief Returns the number of elements in the auto_buffer
    ///
    /// \note In the case where memory allocation has failed in the context
    /// where exceptions are not thrown for allocation failure in the
    /// constructor, this method will return 0. Hence, operations on the
    /// <i>empty</i> auto_buffer<> instance will be safe if made in respect of
    /// the value returned by this method.
    size_type size() const
    {
        return m_cItems;
    }

    /// \brief Returns the number of elements in the auto_buffer's internal buffer
    static size_type internal_size()
    {
        return space;
    }

    /// \brief Indicates whether the buffer has any contents
    ///
    /// \note This will only ever be true when an allocation above the number
    /// of elements in the internal array has been requested, and failed.
    ss_bool_t empty() const
    {
        return 0 == m_cItems;
    }

#if defined(STLSOFT_CF_ALLOCATOR_BASE_EXPENSIVE)
    static allocator_type &get_allocator()
    {
        static allocator_type   s_allocator;

        return s_allocator;
    }
#else /* ? STLSOFT_CF_ALLOCATOR_BASE_EXPENSIVE */
    allocator_type get_allocator() const
    {
        return *this;
    }
#endif /* STLSOFT_CF_ALLOCATOR_BASE_EXPENSIVE */

//@}

/// \name Members
//@{
private:
    pointer     m_buffer;           // Pointer to used buffer
    size_type   m_cItems;           // Number of items in buffer
    value_type  m_internal[space];  // Internal storage
//@}

// Not to be implemented
private:
    auto_buffer(class_type const &rhs);
    auto_buffer const &operator =(class_type const &rhs);
};

/* /////////////////////////////////////////////////////////////////////////////
 * Shims
 */

#ifndef __STLSOFT_CF_TEMPLATE_SHIMS_NOT_SUPPORTED

template<   ss_typename_param_k T
        ,   ss_typename_param_k A
        ,   ss_size_t           SPACE
        >
inline ss_bool_t is_empty(auto_buffer<T, A, SPACE> const &b)
{
    return b.empty();
}

#endif /* !__STLSOFT_CF_TEMPLATE_SHIMS_NOT_SUPPORTED */

////////////////////////////////////////////////////////////////////////////////
// Unit-testing

#ifdef STLSOFT_UNITTEST

namespace unittest
{
    ss_bool_t test_stlsoft_auto_buffer(unittest_reporter *r)
    {
        ss_bool_t               bSuccess    =   true;

        unittest_initialiser    init(r, "STLSoft", "auto_buffer", __FILE__);

        typedef auto_buffer<int, new_allocator<int>, 16>    buffer_t;

        buffer_t    buffer1(10);
        buffer_t    buffer2(100);

        /* Check that resizing within the internal_size() does not
         * change the pointer.
         */
        int *pi = buffer1.data();

        for(ss_size_t c = 1; bSuccess && c < buffer1.internal_size(); ++c)
        {
            buffer1.resize(c);

            if(pi != buffer1.data())
            {
                r->report("auto_buffer resize not allocated from internal buffer: failed ", __LINE__);
                bSuccess = false;
            }
        }

#if defined(__STLSOFT_CF_BIDIRECTIONAL_ITERATOR_SUPPORT)
        /* Check that forward and backward iteration provide the same result. */
        int total_forward   =   stlsoft_ns_qual_std(accumulate)(buffer2.begin(), buffer2.end(), 0);
        int total_backward  =   stlsoft_ns_qual_std(accumulate)(buffer2.rbegin(), buffer2.rend(), 0);

        if(total_forward != total_backward)
        {
            r->report("Forward and backward enumeration equivalence failed ", __LINE__);
            bSuccess = false;
        }
#endif /* __STLSOFT_CF_BIDIRECTIONAL_ITERATOR_SUPPORT */

        return bSuccess;
    }

    unittest_registrar    unittest_stlsoft_auto_buffer(test_stlsoft_auto_buffer);

} // namespace unittest

#endif /* STLSOFT_UNITTEST */

/* ////////////////////////////////////////////////////////////////////////// */

#ifndef _STLSOFT_NO_NAMESPACE
} // namespace stlsoft
#endif /* _STLSOFT_NO_NAMESPACE */

/* ////////////////////////////////////////////////////////////////////////// */

#endif /* !STLSOFT_INCL_H_STLSOFT_AUTO_BUFFER */

/* ////////////////////////////////////////////////////////////////////////// */
