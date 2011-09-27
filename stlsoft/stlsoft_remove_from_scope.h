/* /////////////////////////////////////////////////////////////////////////////
 * File:        stlsoft_remove_from_scope.h
 *
 * Purpose:     Contains the auto_destructor and auto_array_destructor template
 *              classes.
 *
 * Created:     10th June 2002
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


/// \file stlsoft_remove_from_scope.h
///
/// Contains the auto_destructor and auto_array_destructor template classes.

#ifndef STLSOFT_INCL_H_STLSOFT_REMOVE_FROM_SCOPE
#define STLSOFT_INCL_H_STLSOFT_REMOVE_FROM_SCOPE

#ifndef __STLSOFT_DOCUMENTATION_SKIP_SECTION
# define STLSOFT_VER_H_STLSOFT_REMOVE_FROM_SCOPE_MAJOR      2
# define STLSOFT_VER_H_STLSOFT_REMOVE_FROM_SCOPE_MINOR      0
# define STLSOFT_VER_H_STLSOFT_REMOVE_FROM_SCOPE_REVISION   1
# define STLSOFT_VER_H_STLSOFT_REMOVE_FROM_SCOPE_EDIT       22
#endif /* !__STLSOFT_DOCUMENTATION_SKIP_SECTION */

/* /////////////////////////////////////////////////////////////////////////////
 * Includes
 */

#ifndef STLSOFT_INCL_H_STLSOFT
# include "stlsoft.h"                // Include the STLSoft root header
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

// class remove_from_scope

/// This class is almost completely unusable, and is used as a placeholder or as
/// a name remover
class remove_from_scope
{
// Construction
public:
    /// Default constructor
    remove_from_scope()
    {}
    /// Destructor
    ~remove_from_scope() stlsoft_throw_0()
    {}

// Not to be implemented
private:
    remove_from_scope(remove_from_scope const &rhs);
    remove_from_scope const &operator =(remove_from_scope const &rhs);

    // This function exists merely to prevent GCC from warning about
    // op-new returning null rather than throwing an exception
    static void *op_new_(size_t )
    {
        return 0;
    }

    void *operator new(size_t si)
    {
        return op_new_(si);
    }
    void operator delete(void *)
    {}

    int operator !() const;
#ifdef __STLSOFT_CF_NATIVE_BOOL_SUPPORT
    operator bool() const;
#endif /* __STLSOFT_CF_NATIVE_BOOL_SUPPORT */
    operator void *() const;
    void *operator &() const;
#if defined(__STLSOFT_COMPILER_IS_GCC) || \
    defined(__STLSOFT_COMPILER_IS_INTEL) || \
    defined(__STLSOFT_COMPILER_IS_WATCOM)
    struct _{};
    _ *operator ->() const;
#else
    struct {} *operator ->() const;
#endif /* __SYNSOFT_DBS_DEBUG */
#ifdef __STLSOFT_CF_MEMBER_TEMPLATE_FUNCTION_SUPPORT
    template <ss_typename_param_k T>
    operator T &();
    template <ss_typename_param_k T>
    operator T const &() const;
#endif // __STLSOFT_CF_MEMBER_TEMPLATE_FUNCTION_SUPPORT
};

/* ////////////////////////////////////////////////////////////////////////// */

#ifndef _STLSOFT_NO_NAMESPACE
} // namespace stlsoft
#endif /* _STLSOFT_NO_NAMESPACE */

/* ////////////////////////////////////////////////////////////////////////// */

#endif /* !STLSOFT_INCL_H_STLSOFT_REMOVE_FROM_SCOPE */

/* ////////////////////////////////////////////////////////////////////////// */
