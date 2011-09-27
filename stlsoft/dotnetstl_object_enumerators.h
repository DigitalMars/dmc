/* /////////////////////////////////////////////////////////////////////////////
 * File:        dotnetstl_object_enumerators.h
 *
 * Purpose:     A simple object enumerator, based on ArrayList.
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


/// \file dotnetstl_object_enumerators.h
///
/// A simple object enumerator, based on ArrayList.

#ifndef DOTNETSTL_INCL_H_DOTNETSTL_OBJECT_ENUMERATORS
#define DOTNETSTL_INCL_H_DOTNETSTL_OBJECT_ENUMERATORS

#ifndef __STLSOFT_DOCUMENTATION_SKIP_SECTION
# define DOTNETSTL_VER_H_DOTNETSTL_OBJECT_ENUMERATORS_MAJOR     2
# define DOTNETSTL_VER_H_DOTNETSTL_OBJECT_ENUMERATORS_MINOR     0
# define DOTNETSTL_VER_H_DOTNETSTL_OBJECT_ENUMERATORS_REVISION  1
# define DOTNETSTL_VER_H_DOTNETSTL_OBJECT_ENUMERATORS_EDIT      12
#endif /* !__STLSOFT_DOCUMENTATION_SKIP_SECTION */

/* /////////////////////////////////////////////////////////////////////////////
 * Includes
 */

#ifndef DOTNETSTL_INCL_H_DOTNETSTL
# include "dotnetstl.h"                     // Include the .netSTL root header
#endif /* !DOTNETSTL_INCL_H_DOTNETSTL */

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

/// Provides an IEnumerator implementation on an ArrayList
public __gc class ArrayListEnumerator
    : public System::Collections::IEnumerator
{
//    typedef System::Collections::ArrayList  ArrayList;

public:
    /// Creates an enumerator for the given list.
    ArrayListEnumerator(System::Collections::ArrayList *objects)
        : m_objects(objects)
        , m_index(-1)
    {}

public: // VC++ 7.1 requires that these be public
    /// Returns the current object
    Object* get_Current()
    {
        if( m_index < 0 ||
            !(m_index < m_objects->get_Count()))
        {
            throw new System::InvalidOperationException();
        }

        return m_objects->get_Item(m_index);
    }

    /// Advances the search by one position
    ///
    /// \retval true An object is available at the new position
    /// \retval false The enumeration is complete
    bool MoveNext()
    {
        if(m_index < m_objects->get_Count() - 1)
        {
            ++m_index;

            return true;
        }
        else
        {
            return false;
        }
    }

    /// Resets the enumeration
    void Reset()
    {
        m_index = -1;
    }

// Members
private:
    System::Collections::ArrayList  *m_objects;
    int                             m_index;

// Not to be implemented
private:
    ArrayListEnumerator(ArrayListEnumerator const &)
    {}
//  ArrayListEnumerator &operator =(ArrayListEnumerator const &);
};

/* ////////////////////////////////////////////////////////////////////////// */

#ifdef _STLSOFT_NO_NAMESPACE
} // namespace dotnetstl
#else
} // namespace dotnetstl_project
} // namespace stlsoft
#endif /* _STLSOFT_NO_NAMESPACE */

/* ////////////////////////////////////////////////////////////////////////// */

#endif /* DOTNETSTL_INCL_H_DOTNETSTL_OBJECT_ENUMERATORS */

/* ////////////////////////////////////////////////////////////////////////// */
