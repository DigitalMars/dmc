/* /////////////////////////////////////////////////////////////////////////////
 * File:        comstl_functionals.h
 *
 * Purpose:     Contains COM specific predicates and functionals.
 *
 * Created:
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


/// \file comstl_functionals.h
///
/// Contains COM specific predicates and functionals.

#ifndef COMSTL_INCL_H_COMSTL_FUNCTIONALS
#define COMSTL_INCL_H_COMSTL_FUNCTIONALS

#ifndef __STLSOFT_DOCUMENTATION_SKIP_SECTION
# define COMSTL_VER_H_COMSTL_FUNCTIONALS_MAJOR      2
# define COMSTL_VER_H_COMSTL_FUNCTIONALS_MINOR      0
# define COMSTL_VER_H_COMSTL_FUNCTIONALS_REVISION   1
# define COMSTL_VER_H_COMSTL_FUNCTIONALS_EDIT       37
#endif /* !__STLSOFT_DOCUMENTATION_SKIP_SECTION */

/* ////////////////////////////////////////////////////////////////////////////
 * Includes
 */

#ifndef COMSTL_INCL_H_COMSTL
# include "comstl.h"                    // Include the COMSTL root header
#endif /* !COMSTL_INCL_H_COMSTL */
#ifndef COMSTL_INCL_H_COMSTL_REFCOUNT_FUNCTIONS
# include "comstl_refcount_functions.h" // safe_addref, safe_release
#endif /* !COMSTL_INCL_H_COMSTL_REFCOUNT_FUNCTIONS */
#include <functional>

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

/* /////////////////////////////////////////////////////////////////////////////
 * Functionals
 */

/// \brief Releases COM interfaces
///
/// This function object releases COM interfaces by calling Release()
/// on them. Note that the function object benignly ignores null interface
/// pointers.
///
/// \param I The COM interface on which to parameterise the template
template <ss_typename_param_k I>
struct interface_release
    : public comstl_ns_qual_std(unary_function)<I*, void>
{
public:
    typedef I   interface_type;

    /// \brief Function call operator
    ///
    /// \param pi The interface pointer to release
    void operator ()(interface_type *pi)
    {
        safe_release(pi);
    }
};

/// \brief Adds references to COM interfaces
///
/// This function object adds a reference to COM interfaces by calling AddRef()
/// on them. Note that the function object benignly ignores null interface
/// pointers.
///
/// \param I The COM interface on which to parameterise the template
template <ss_typename_param_k I>
struct interface_addref
    : public comstl_ns_qual_std(unary_function)<I*, void>
{
public:
    typedef I   interface_type;

    /// \brief Function call operator
    ///
    /// \param pi The interface pointer on which to add a reference
    void operator ()(interface_type *pi)
    {
        safe_addref(pi);
    }
};

/// \brief An adaptor for member functions of COM interfaces
///
/// This function object calls a COM STDAPI (__stdcall) function on an object.
///
/// \param T The object class on which the function will be applied
/// \param R The return type of the function to be applied
template< ss_typename_param_k R
        , ss_typename_param_k T
        >
class std_mem_fun_t
    : public comstl_ns_qual_std(unary_function)<T*, R>
{
public:
    typedef R                               return_type;
    typedef T                               operand_class_type;
    typedef return_type (STDAPICALLTYPE T::*method_type)();

public:
    /// \brief Constructor
    ///
    /// \param f The function (pointer to member of T, returning R) which will be applied to the objects
    ss_explicit_k std_mem_fun_t(method_type f)
        : m_f(f)
    {}

    /// \brief Function call operator
    ///
    /// \param pt The object pointer on which to apply the function
    return_type operator ()(operand_class_type *pt) const
    {
        return (pt->*m_f)();
    }

private:
    method_type m_f;
};

/// \brief Helper function for std_mem_fun_t
template <class R, class T>
inline std_mem_fun_t<R, T> std_mem_fun(R (STDAPICALLTYPE T::*f)())
{
    return std_mem_fun_t<R, T>(f);
}

/// \brief An adaptor for member functions of COM interfaces
///
/// This function object calls a COM STDAPI (__stdcall) function on an object.
///
/// \param T The object class on which the function will be applied
/// \param R The return type of the function to be applied
template< ss_typename_param_k R
        , ss_typename_param_k T
        >
class std_mem_fun_ref_t
    : public comstl_ns_qual_std(unary_function)<T&, R>
{
public:
    typedef R                               return_type;
    typedef T                               operand_class_type;
    typedef return_type (STDAPICALLTYPE T::*method_type)();

public:
    /// \brief Constructor
    ///
    /// \param f The function (pointer to member of T, returning R) which will be applied to the objects
    ss_explicit_k std_mem_fun_ref_t(method_type f)
        : m_f(f)
    {}

    /// \brief Function call operator
    ///
    /// \param t The object on which to apply the function
    return_type operator ()(operand_class_type &t) const
    {
        return (t.*m_f)();
    }

private:
    method_type m_f;
};

/// \brief Helper function for std_mem_fun_ref_t
template <class R, class T>
inline std_mem_fun_ref_t<R, T> std_mem_fun_ref(R (STDAPICALLTYPE T::*f)())
{
    return std_mem_fun_ref_t<R, T>(f);
}


/// \brief Closes BSTRs
///
/// This function object closes BSTRs by applying
/// SysFreeString() to them
struct BSTR_close
    : public comstl_ns_qual_std(unary_function)<BSTR, void>
{
    /// \brief Function call operator
    ///
    /// \param s The string to free
   void operator ()(BSTR s) const
    {
        ::SysFreeString(s);
    }
};

/// \brief Close OLE strings
///
/// This function object closes OLE strings (LPOLESTR) by returning
/// their memory back to the COM task allocator
struct olestring_close
    : public comstl_ns_qual_std(unary_function)<LPOLESTR, void>
{
    /// \brief Function call operator
    ///
    /// \param s The string to free
    void operator ()(LPOLESTR s) const
    {
        ::CoTaskMemFree(s);
    }
};

/// \brief Close task memory blocks
///
/// This function object closes task memory blocks by returning
/// their memory back to the COM task allocator
struct task_memory_close
    : public comstl_ns_qual_std(unary_function)<void *, void>
{
    /// \brief Function call operator
    ///
    /// \param p The memory to free
    void operator ()(void *p) const
    {
        ::CoTaskMemFree(p);
    }
};

/// \brief Close VARIANTs
///
/// This function object closes VARIANTs by applying
/// VariantClear() to them
struct VARIANT_close
    : public comstl_ns_qual_std(unary_function)<VARIANT&, void>
{
    /// \brief Function call operator
    ///
    /// \param v The variant to free
    void operator ()(VARIANT &v) const
    {
        ::VariantClear(&v);
    }
};

////////////////////////////////////////////////////////////////////////////////
// Unit-testing

#ifdef STLSOFT_UNITTEST

namespace unittest
{
    ss_bool_t test_comstl_functionals(unittest_reporter *r)
    {
        using stlsoft::unittest::unittest_initialiser;

        ss_bool_t               bSuccess    =   true;

        unittest_initialiser    init(r, "COMSTL", "functionals", __FILE__);

#if 0
        if(<<TODO>>)
        {
            r->report("<<TODO>> failed ", __LINE__);
            bSuccess = false;
        }
#endif /* 0 */

        return bSuccess;
    }

    unittest_registrar    unittest_comstl_functionals(test_comstl_functionals);

} // namespace unittest

#endif /* STLSOFT_UNITTEST */

/* ////////////////////////////////////////////////////////////////////////// */

#ifndef _COMSTL_NO_NAMESPACE
# if defined(_STLSOFT_NO_NAMESPACE) || \
     defined(__STLSOFT_DOCUMENTATION_SKIP_SECTION)
} // namespace comstl
# else
} // namespace comstl_project
} // namespace stlsoft
# endif /* _STLSOFT_NO_NAMESPACE */
#endif /* !_COMSTL_NO_NAMESPACE */

/* ////////////////////////////////////////////////////////////////////////// */

#endif /* !COMSTL_INCL_H_COMSTL_FUNCTIONALS */

/* ////////////////////////////////////////////////////////////////////////// */
