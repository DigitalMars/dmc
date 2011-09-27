/* /////////////////////////////////////////////////////////////////////////////
 * File:        stlsoft_static_initialisers.h (formerly MLClsCtr.h, ::SynesisStd)
 *
 * Purpose:     Initialiser classes for the STLSoft libraries.
 *
 * Created:     17th February 1997
 * Updated:     11th September 2004
 *
 * Home:        http://stlsoft.org/
 *
 * Copyright (c) 1997-2004, Matthew Wilson and Synesis Software
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


/// \file stlsoft_static_initialisers.h
///
/// Initialiser classes for the STLSoft libraries.

#ifndef STLSOFT_INCL_H_STLSOFT_STATIC_INITIALISERS
#define STLSOFT_INCL_H_STLSOFT_STATIC_INITIALISERS

#ifndef __STLSOFT_DOCUMENTATION_SKIP_SECTION
# define STLSOFT_VER_H_STLSOFT_STATIC_INITIALISERS_MAJOR     2
# define STLSOFT_VER_H_STLSOFT_STATIC_INITIALISERS_MINOR     0
# define STLSOFT_VER_H_STLSOFT_STATIC_INITIALISERS_REVISION  1
# define STLSOFT_VER_H_STLSOFT_STATIC_INITIALISERS_EDIT      196
#endif /* !__STLSOFT_DOCUMENTATION_SKIP_SECTION */

/* /////////////////////////////////////////////////////////////////////////////
 * Includes
 */

#ifndef STLSOFT_INCL_H_STLSOFT
# include "stlsoft.h"   // Include the STLSoft root header
#endif /* !STLSOFT_INCL_H_STLSOFT */

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

#if 0
class method_constructor
{
public:
    template<typename T>
    method_constructor(T const &t, void (T::*const fn)())
    {
        (t.*fn)();
    }
    template<typename T, typename R>
    method_constructor(T const &t, R (T::*const fn)())
    {
        (t.*fn)();
    }
};
#endif /* 0 */

/// static_initialiser
///
/// Initialises any non-class function or type
class static_initialiser
{
public:
    typedef static_initialiser  class_type;

/// \name Constructors
//@{
public:
#ifdef __STLSOFT_CF_MEMBER_TEMPLATE_CTOR_SUPPORT
    template <typename T>
    static_initialiser(T const &/* t */)
    {}
    template <typename T>
    static_initialiser(T const * /* pt */)
    {}
#else
    static_initialiser(int /* t */)
    {}
    static_initialiser(void const * /* pt */)
    {}
#endif // __STLSOFT_CF_MEMBER_TEMPLATE_CTOR_SUPPORT
//@}

/// \name Not to be implemented
//@{
private:
    static_initialiser(class_type const &);
    static_initialiser &operator =(class_type const &);

#ifdef __STLSOFT_COMPILER_IS_COMO
    void *operator new(ss_size_t) stlsoft_throw_0()
    {
        return 0;
    }
#else /* ? __STLSOFT_COMPILER_IS_COMO */
    void *operator new(ss_size_t) stlsoft_throw_0();
#endif /* __STLSOFT_COMPILER_IS_COMO */
    void operator delete(void *)
    {}
//@}
};

/// Used to initialise APIs
class api_constructor
{
/// \name Constructors
//@{
public:
    api_constructor(void (*pfnInit)(), void (*pfnUninit)())
        : m_pfnUninit(pfnUninit)
    {
        if(NULL != pfnInit)
        {
            (*pfnInit)();
        }
    }
    ~api_constructor()
    {
        if(NULL != m_pfnUninit)
        {
            (*m_pfnUninit)();
        }
    }
//@}

/// \name Members
//@{
private:
    void (*m_pfnUninit)(void);
//@}

/// \name Not to be implemented
//@{
private:
    api_constructor(api_constructor const &);
    api_constructor &operator =(api_constructor const &);

#ifdef __STLSOFT_COMPILER_IS_COMO
    void *operator new(ss_size_t) stlsoft_throw_0()
    {
        return 0;
    }
#else /* ? __STLSOFT_COMPILER_IS_COMO */
    void *operator new(ss_size_t) stlsoft_throw_0();
#endif /* __STLSOFT_COMPILER_IS_COMO */
    void operator delete(void *)
    {}
//@}
};

/// Used to initialise classes
template <ss_typename_param_k T>
class class_constructor
    : protected api_constructor
{
/// \name Member types
//@{
public:
    typedef void (*class_init_fn_t)();
    typedef void (*class_uninit_fn_t)();

//@}

/// \name Constructors
//@{
public:
    ss_explicit_k class_constructor()
        : api_constructor(&T::class_init, &T::class_uninit)
    {}

    ss_explicit_k class_constructor(    class_init_fn_t     pfnInit
                                    ,   class_uninit_fn_t   pfnUninit)
        : api_constructor(pfnInit, pfnUninit)
    {}
//@}

/// \name Not to be implemented
//@{
private:
    class_constructor(class_constructor const &);
    class_constructor &operator =(class_constructor const &);

#ifdef __STLSOFT_COMPILER_IS_COMO
    void *operator new(ss_size_t) stlsoft_throw_0()
    {
        return 0;
    }
#else /* ? __STLSOFT_COMPILER_IS_COMO */
    void *operator new(ss_size_t) stlsoft_throw_0();
#endif /* __STLSOFT_COMPILER_IS_COMO */
    void operator delete(void *)
    {}
//@}
};

/* ////////////////////////////////////////////////////////////////////////// */

#ifndef _STLSOFT_NO_NAMESPACE
} // namespace stlsoft
#endif /* _STLSOFT_NO_NAMESPACE */

/* ////////////////////////////////////////////////////////////////////////// */

#endif /* !STLSOFT_INCL_H_STLSOFT_STATIC_INITIALISERS */

/* ////////////////////////////////////////////////////////////////////////// */
