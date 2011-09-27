/* /////////////////////////////////////////////////////////////////////////////
 * File:        stlsoft_typefixer_reference.h
 *
 * Purpose:     Definition of the fixer_reference and fixer_reference_type types.
 *
 * Created:     9th February 2004
 * Updated:     11th September 2004
 *
 * Home:        http://stlsoft.org/
 *
 * Copyright (c) 2004, Matthew Wilson and Synesis Software
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


#ifndef STLSOFT_INCL_H_STLSOFT_ITERATOR
# error This file must not be included independently of stlsoft_iterator.h
#endif /* !STLSOFT_INCL_H_STLSOFT_ITERATOR */

#ifndef __STLSOFT_CF_TEMPLATE_PARTIAL_SPECIALISATION_SUPPORT
# error This file cannot be included by compilers that do not support partial template specialisation
#endif // !__STLSOFT_CF_TEMPLATE_PARTIAL_SPECIALISATION_SUPPORT

/* ////////////////////////////////////////////////////////////////////////// */

#ifndef __STLSOFT_DOCUMENTATION_SKIP_SECTION
# define STLSOFT_VER_H_STLSOFT_TYPEFIXER_REFERENCE_MAJOR        2
# define STLSOFT_VER_H_STLSOFT_TYPEFIXER_REFERENCE_MINOR        0
# define STLSOFT_VER_H_STLSOFT_TYPEFIXER_REFERENCE_REVISION     1
# define STLSOFT_VER_H_STLSOFT_TYPEFIXER_REFERENCE_EDIT         7
#endif /* !__STLSOFT_DOCUMENTATION_SKIP_SECTION */

/* /////////////////////////////////////////////////////////////////////////////
 * Auto-generation and compatibility
 */

/*
[<[STLSOFT-AUTO:NO-DOCFILELABEL]>]
[<[STLSOFT-AUTO:NO-UNITTEST]>]
*/

/* /////////////////////////////////////////////////////////////////////////////
 * Types
 */

// Uses the "reference" member type, if present
template <ss_typename_param_k T, ss_bool_t>
struct fixer_reference
{
    typedef ss_typename_type_k T::value_type    value_type;
    typedef ss_typename_type_k T::reference       reference;
};

#ifndef __STLSOFT_DOCUMENTATION_SKIP_SECTION

// When not present it uses void as a placeholder
template <ss_typename_param_k T>
struct fixer_reference<T, false>
{
    typedef void                                value_type;
    typedef void                                reference;
};

#endif /* !__STLSOFT_DOCUMENTATION_SKIP_SECTION */


// Uses the "reference_type" member type, if present
template <ss_typename_param_k T, ss_bool_t>
struct fixer_reference_type
{
    typedef ss_typename_type_k T::value_type    value_type;
    typedef ss_typename_type_k T::reference_type  reference;
};

#ifndef __STLSOFT_DOCUMENTATION_SKIP_SECTION

// When not present it uses void as a placeholder
template <ss_typename_param_k T>
struct fixer_reference_type<T, false>
{
    typedef void                                value_type;
    typedef void                                reference;
};

#endif /* !__STLSOFT_DOCUMENTATION_SKIP_SECTION */

/* ////////////////////////////////////////////////////////////////////////// */
