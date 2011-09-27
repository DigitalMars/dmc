/* /////////////////////////////////////////////////////////////////////////////
 * File:        Delayed_delete_allocator_adaptor_test.cpp
 *
 * Purpose:     Implementation file for the delayed_delete_allocator_adaptor_test project.
 *
 * Created:     2nd May 2003
 * Updated:     30th May 2004
 *
 * Status:      Wizard-generated
 *
 * License:     (Licensed under the Synesis Software Open License)
 *
 *              Copyright (C) 1999-2003, Synesis Software Pty Ltd.
 *              All rights reserved.
 *
 *              www:        http://www.synesis.com.au/software
 *
 *              email:      software@synesis.com.au
 *
 *              This source code is placed into the public domain 2003
 *              by Synesis Software Pty Ltd. There are no restrictions
 *              whatsoever to your use of the software.
 *
 *              This source code is provided by Synesis Software Pty Ltd "as is"
 *              and any warranties, whether expressed or implied, including, but
 *              not limited to, the implied warranties of merchantability and
 *              fitness for a particular purpose are disclaimed. In no event
 *              shall the Synesis Software Pty Ltd be liable for any direct,
 *              indirect, incidental, special, exemplary, or consequential
 *              damages (including, but not limited to, procurement of
 *              substitute goods or services; loss of use, data, or profits; or
 *              business interruption) however caused and on any theory of
 *              liability, whether in contract, strict liability, or tort
 *              (including negligence or otherwise) arising in any way out of
 *              the use of this software, even if advised of the possibility of
 *              such damage.
 *
 *              Neither the name of Synesis Software Pty Ltd nor the names of
 *              any subdivisions, employees or agents of Synesis Software Pty
 *              Ltd, nor the names of any other contributors to this software
 *              may be used to endorse or promote products derived from this
 *              software without specific prior written permission.
 *
 * ////////////////////////////////////////////////////////////////////////// */

#include <stdio.h>

#include <stlsoft.h>
#include <stlsoft_new_allocator.h>
#include <stlsoft_malloc_allocator.h>

/* ////////////////////////////////////////////////////////////////////////// */

using namespace stlsoft;

#if 0
#ifndef __STLSOFT_DOCUMENTATION_SKIP_SECTION

template<	ss_typename_param_k A
		,	ss_sint_t			QUANTA
		>
class delayed_delete_allocator_adaptor;

// Specialisation for void
#ifdef __STLSOFT_CF_TEMPLATE_SPECIALISATION_SYNTAX
template <>
#endif /* __STLSOFT_CF_TEMPLATE_SPECIALISATION_SYNTAX */
class delayed_delete_allocator_adaptor<void>
{
public:
    typedef void                                value_type;
    typedef delayed_delete_allocator_adaptor<void>              class_type;
    typedef void                                *pointer;
    typedef void const                          *const_pointer;
    typedef ptrdiff_t                           difference_type;
    typedef ss_size_t                           size_type;

#ifdef STLSOFT_CF_ALLOCATOR_REBIND_SUPPORT
    /// The allocator <b><code>rebind</code></b> structure
    template <ss_typename_param_k U>
    struct rebind
    {
        typedef delayed_delete_allocator_adaptor<U>             other;
    };
#endif /* STLSOFT_CF_ALLOCATOR_REBIND_SUPPORT */
};

#endif /* !__STLSOFT_DOCUMENTATION_SKIP_SECTION */
#endif /* 0 */


template<	ss_typename_param_k A
		,	ss_sint_t			QUANTA
		>
class delayed_delete_allocator_adaptor
{
public:
	/// The allocator type
	typedef	A													allocator_type;
	/// The allocation delay quanta
	enum { quanta = QUANTA };
	/// The current parameterisation of the type
	typedef delayed_delete_allocator_adaptor<A, QUANTA>			class_type;
    /// The value type
    typedef ss_typename_type_k allocator_type::value_type		value_type;
    /// The pointer type
    typedef ss_typename_type_k allocator_type::pointer			pointer;
    /// The non-mutating (const) pointer type
    typedef ss_typename_type_k allocator_type::const_pointer	const_pointer;
    /// The reference type
    typedef ss_typename_type_k allocator_type::reference		reference;
    /// The non-mutating (const) reference type
    typedef ss_typename_type_k allocator_type::const_reference	const_reference;
    /// The difference type
    typedef ss_typename_type_k allocator_type::difference_type	difference_type;
    /// The size type
    typedef ss_typename_type_k allocator_type::size_type		size_type;

#ifdef STLSOFT_CF_ALLOCATOR_REBIND_SUPPORT
    /// The allocator <b><code>rebind</code></b> structure
    template<	ss_typename_param_k U
			,	ss_sint_t			Q = quanta
			>
    struct rebind
    {
        typedef delayed_delete_allocator_adaptor<U, Q>			other;
    };
#endif /* STLSOFT_CF_ALLOCATOR_REBIND_SUPPORT */

/// Construction
/// @{
public:
	delayed_delete_allocator_adaptor() stlsoft_throw_0()
		: m_count(0)
	{}
#ifndef __STLSOFT_CF_MEMBER_TEMPLATE_FUNCTION_SUPPORT
    /// Copy constructor
    delayed_delete_allocator_adaptor(const delayed_delete_allocator_adaptor &rhs) stlsoft_throw_0()
    {}
#else
    /// Copy constructor
    template<	ss_typename_param_k U
			,	ss_sint_t			Q
			>
    delayed_delete_allocator_adaptor(const delayed_delete_allocator_adaptor<U, Q> &rhs) stlsoft_throw_0()
    {}
#endif /* !__STLSOFT_CF_MEMBER_TEMPLATE_FUNCTION_SUPPORT */
    /// Destructor
    ~delayed_delete_allocator_adaptor() stlsoft_throw_0()
    {}

// Attributes
public:
    /// Returns the maximum number of allocatable entities
    size_type max_size() const stlsoft_throw_0()
    {
        return static_cast<size_type>(-1) / sizeof(value_type);
    }

// Conversion
public:
    /// Returns the address corresponding to the given reference
    ///
    /// \param x A reference to a \c value_type instance whose address will be calculated
    pointer address(reference x) const stlsoft_throw_0()
    {
        return &x;
    }
    /// Returns the address corresponding to the given non-mutable (const) reference
    ///
    /// \param x A non-mutable (const) reference to a \c value_type instance whose address will be calculated
    const_pointer address(const_reference x) const stlsoft_throw_0()
    {
        return &x;
    }

// Allocation
public:
    /// Allocates a block of memory sufficient to store \c n elements of type \c value_type
    ///
    /// \param n The number of elements to allocate
    /// \param pv A hint to enhance localisation (not used in this class)
    /// \return The allocated block, or the null pointer (if the allocation fails and the translation does not support throwing exceptions upon memory exhaustion)
    pointer allocate(size_type n, allocator_type::rebind<void>::other::const_pointer = NULL)
    {
        allocator_type::rebind<void>::other::pointer p   =   malloc(n * sizeof(value_type));

#ifdef __STLSOFT_CF_THROW_BAD_ALLOC
        if(p == NULL)
        {
            throw stlsoft_ns_qual_std(bad_alloc)();
        }
#endif /* __STLSOFT_CF_THROW_BAD_ALLOC */

        return static_cast<pointer>(p);
    }

    pointer reallocate(pointer p, size_type n, allocator_type::rebind<void>::other::const_pointer = NULL)
    {
        allocator_type::rebind<void>::other::pointer pNew   =   realloc(p, n * sizeof(value_type));

#ifdef __STLSOFT_CF_THROW_BAD_ALLOC
        if(pNew == NULL)
        {
            throw stlsoft_ns_qual_std(bad_alloc)();
        }
#endif /* __STLSOFT_CF_THROW_BAD_ALLOC */

        return static_cast<pointer>(pNew);
    }

    /// Deallocates a pointer
    ///
    /// \param p The memory block to deallocate
    /// \param n The number of blocks to deallocate
    void deallocate(pointer p, size_type)
    {
        free(p);
    }

    /// Deallocates a pointer
    ///
    /// \param p The memory block to deallocate
    void deallocate(pointer p)
    {
        free(p);
    }

// Operations
public:
    /// In-place constructs an instance of the \c value_type, with the given value
    ///
    /// \param p The location in which to construct the instance
    /// \param x The value with which to copy construct the instance
    void construct(pointer p, value_type const &x)
    {
        stlsoft_assert(p != NULL);

        new(p) value_type(x);
    }

    /// In-place constructs an instance of the \c value_type
    ///
    /// \param p The location in which to construct the instance
    void construct(pointer p)
    {
        stlsoft_assert(p != NULL);

        new(p) value_type();
    }

    /// In-place destroys an instance
    ///
    /// \param p The instance whose destructor is to be called
    void destroy(pointer p) stlsoft_throw_0()
    {
        stlsoft_assert(p != NULL);

        stlsoft_destroy_instance(T, value_type, p);
    }
/// @}

//m_ator

/// Members
private:
	allocator_type	m_ator;
	ss_sint_t		m_count;
	pointer			m_allocs[quanta];

// Not to be implemented
private:
    class_type const &operator =(class_type const &rhs);
};

#ifndef __STLSOFT_DOCUMENTATION_SKIP_SECTION

template<	ss_typename_param_k A
		,	ss_sint_t			QUANTA
		>
inline ss_bool_t operator ==(const delayed_delete_allocator_adaptor<A, QUANTA> &/* lhs */, const delayed_delete_allocator_adaptor<A, QUANTA> &/* rhs */)
{
    return ss_true_v;
}

template<	ss_typename_param_k A
		,	ss_sint_t			QUANTA
		>
inline ss_bool_t operator !=(const delayed_delete_allocator_adaptor<A, QUANTA> &/* lhs */, const delayed_delete_allocator_adaptor<A, QUANTA> &/* rhs */)
{
    return ss_false_v;
}

#endif /* !__STLSOFT_DOCUMENTATION_SKIP_SECTION */

/* ////////////////////////////////////////////////////////////////////////// */

/// @} // end of group stlsoft_allocator_library

/* ////////////////////////////////////////////////////////////////////////// */

template <typename A>
void test_allocator(A &a)
{
	for(int i = 0; i < 100; ++i)
	{
		ss_typename_type_k A::pointer p = a.allocate(i, 0);

		a.deallocate(p, i);
	}
}

struct S
{
	int		i;
	float	f;
	double	d;
};

int main(int /* argc */, char ** /*argv*/)
{
	puts("delayed_delete_allocator_adaptor_test: " __STLSOFT_COMPILER_LABEL_STRING);

	/* . */
	malloc_allocator<S>											ma;
	new_allocator<S>											na;
	delayed_delete_allocator_adaptor<malloc_allocator<S>, 10>	dma10;
	delayed_delete_allocator_adaptor<new_allocator<S>, 10>		dna10;
	delayed_delete_allocator_adaptor<malloc_allocator<S>, 50>	dma50;
	delayed_delete_allocator_adaptor<new_allocator<S>, 50>		dna50;

	test_allocator(ma);
	test_allocator(na);
	test_allocator(dma10);
	test_allocator(dna10);
	test_allocator(dma50);
	test_allocator(dna50);

    return 0;
}

/* ////////////////////////////////////////////////////////////////////////// */
