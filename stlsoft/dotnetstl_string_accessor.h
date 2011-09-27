/* /////////////////////////////////////////////////////////////////////////////
 * File:        dotnetstl_string_accessor.h
 *
 * Purpose:     A useful tool for accessing a String object's content as a c-string.
 *
 * Created:     24th June 2003
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


/// \file dotnetstl_string_accessor.h
///
/// A useful tool for accessing a String object's content as a c-string.

#ifndef DOTNETSTL_INCL_H_DOTNETSTL_STRING_ACCESSOR
#define DOTNETSTL_INCL_H_DOTNETSTL_STRING_ACCESSOR

#ifndef __STLSOFT_DOCUMENTATION_SKIP_SECTION
# define DOTNETSTL_VER_H_DOTNETSTL_STRING_ACCESSOR_MAJOR    2
# define DOTNETSTL_VER_H_DOTNETSTL_STRING_ACCESSOR_MINOR    0
# define DOTNETSTL_VER_H_DOTNETSTL_STRING_ACCESSOR_REVISION 1
# define DOTNETSTL_VER_H_DOTNETSTL_STRING_ACCESSOR_EDIT     20
#endif /* !__STLSOFT_DOCUMENTATION_SKIP_SECTION */

/* /////////////////////////////////////////////////////////////////////////////
 * Includes
 */

#ifndef DOTNETSTL_INCL_H_DOTNETSTL
# include "dotnetstl.h"                     // Include the .netSTL root header
#endif /* !DOTNETSTL_INCL_H_DOTNETSTL */
#ifndef STLSOFT_INCL_H_STLSOFT_SAP_CAST
# include "stlsoft_sap_cast.h"
#endif /* !STLSOFT_INCL_H_STLSOFT_SAP_CAST */
#ifdef STLSOFT_UNITTEST
# include <wchar.h>
#endif /* STLSOFT_UNITTEST */

/* ////////////////////////////////////////////////////////////////////////// */

#ifdef _STLSOFT_NO_NAMESPACE
/* There is no stlsoft namespace, so must define ::dotnetstl */
namespace dotnetstl
{
#else
/* Define stlsoft::dotnet_project */

namespace stlsoft
{

namespace dotnetstl_project
{

#endif /* _STLSOFT_NO_NAMESPACE */

/* /////////////////////////////////////////////////////////////////////////////
 * Classes
 */

/// Template providing scoped C-string access to a String
template <ss_typename_param_k C>
class c_string_accessor
{
public:
    typedef C           char_type;
    typedef C           *pointer;
    typedef C const     *const_pointer;

// Construction
public:
    /// Construct from a System::String
    ///
    /// \param s The String for which C-string access is to be provided. Must not be NULL
    ss_explicit_k c_string_accessor(System::String *s)
        : m_s(get_cstring_(s))
    {}
    /// Release resources
    ~c_string_accessor()
    {
        System::Runtime::InteropServices::Marshal::FreeHGlobal(m_s);
    }

// Accessors
public:
    /// Implicit conversion operator to a C-string
    operator const_pointer() const
    {
        return m_s;
    }

// Implementation
private:
    pointer get_cstring_(System::String *s);

// Members
private:
    pointer m_s;

// Not to be implemented
private:
    c_string_accessor(c_string_accessor const &);
    c_string_accessor &operator =(c_string_accessor const &);
};

////////////////////////////////////////////////////////////////////////////////
// Implementation

#ifndef __STLSOFT_DOCUMENTATION_SKIP_SECTION

template <>
inline c_string_accessor<ds_char_a_t>::pointer c_string_accessor<ds_char_a_t>::get_cstring_(System::String *s)
{
    return sap_cast<ds_char_a_t*>(System::Runtime::InteropServices::Marshal::StringToHGlobalAnsi(s).ToPointer());
}

template <>
inline c_string_accessor<ds_char_w_t>::pointer c_string_accessor<ds_char_w_t>::get_cstring_(System::String *s)
{
    return sap_cast<ds_char_w_t*>(System::Runtime::InteropServices::Marshal::StringToHGlobalUni(s).ToPointer());
}

#endif /* !__STLSOFT_DOCUMENTATION_SKIP_SECTION */

////////////////////////////////////////////////////////////////////////////////
// Unit-testing

#ifdef STLSOFT_UNITTEST

namespace unittest
{
    ss_bool_t test_dotnetstl_string_accessor(unittest_reporter *r)
    {
        using stlsoft::unittest::unittest_initialiser;

        ss_bool_t               bSuccess    =   true;

        unittest_initialiser    init(r, ".netSTL", "string_accessor", __FILE__);

        System::String  *s1 =   new System::String("String #1");
        System::String  *s2 =   new System::String("String #2");

        if(0 != strcmp(c_string_accessor<char>(s1), "String #1"))
        {
            r->report("c_string_accessor<char>() failed ", __LINE__);
            bSuccess = false;
        }

        if(0 != wcscmp(c_string_accessor<wchar_t>(s2), L"String #2"))
        {
            r->report("c_string_accessor<char>() failed ", __LINE__);
            bSuccess = false;
        }

        return bSuccess;
    }

    unittest_registrar    unittest_dotnetstl_string_accessor(test_dotnetstl_string_accessor);

} // namespace unittest

#endif /* STLSOFT_UNITTEST */

/* ////////////////////////////////////////////////////////////////////////// */

#ifdef _STLSOFT_NO_NAMESPACE
} // namespace dotnetstl
#else
} // namespace dotnetstl_project
} // namespace stlsoft
#endif /* _STLSOFT_NO_NAMESPACE */

/* ////////////////////////////////////////////////////////////////////////// */

#endif /* DOTNETSTL_INCL_H_DOTNETSTL_STRING_ACCESSOR */

/* ////////////////////////////////////////////////////////////////////////// */
