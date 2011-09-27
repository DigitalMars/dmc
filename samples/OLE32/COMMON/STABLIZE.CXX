//+---------------------------------------------------------------------------
//
//  Microsoft Windows
//  Copyright (C) Microsoft Corporation, 1992 - 1995.
//
//  File:       stablize.cxx
//
//  Contents:   Stabilization Classes used to stabilize objects during
//              re-entrant calls.
//
//  Classes:    CSafeRefCount
//              CStabilize
//
//  History:    8-26-94   stevebl   Modified from code written by AlexGo
//
//----------------------------------------------------------------------------

#include <windows.h>
#include <ole2.h>
#include <stablize.h>
#include <assert.h>

//+-------------------------------------------------------------------------
//
//  Member:     CSafeRefCount::CSafeRefCount
//
//  Synopsis:   constructor for the safe ref count class
//
//  History:    dd-mmm-yy Author    Comment
//              28-Jul-94 alexgo    author
//              8-26-94   stevebl   modified for use by SDK samples
//
//--------------------------------------------------------------------------

CSafeRefCount::CSafeRefCount()
{
        m_cRefs = 0;
        m_cNest = 0;
        m_fInDelete = FALSE;
}

//+-------------------------------------------------------------------------
//
//  Member:     CSafeRefCount::~CSafeRefCount (virtual)
//
//  History:    dd-mmm-yy Author    Comment
//              28-Jul-94 alexgo    author
//              8-26-94   stevebl   modified for use by SDK samples
//
//--------------------------------------------------------------------------

CSafeRefCount::~CSafeRefCount()
{
        assert(m_cRefs == 0 && m_cNest == 0 && m_fInDelete == TRUE);
}

//+-------------------------------------------------------------------------
//
//  Member:     CSafeRefCount::SafeAddRef
//
//  Synopsis:   increments the reference count on the object
//
//  Returns:    ULONG -- the reference count after the increment
//
//  History:    dd-mmm-yy Author    Comment
//              28-Jul-94 alexgo    author
//              8-26-94   stevebl   modified for use by SDK samples
//
//--------------------------------------------------------------------------

ULONG CSafeRefCount::SafeAddRef()
{
        m_cRefs++;
        return m_cRefs;
}

//+-------------------------------------------------------------------------
//
//  Member:     CSafeRefCount::SafeRelease
//
//  Synopsis:   decrements the reference count on the object
//
//  Effects:    May delete the object!
//
//  Returns:    ULONG -- the reference count after decrement
//
//  Algorithm:  decrements the reference count.  If the reference count
//              is zero AND the nest count is zero AND we are not currently
//              trying to delete our object, then it is safe to delete.
//
//  History:    dd-mmm-yy Author    Comment
//              28-Jul-94 alexgo    author
//              8-26-94   stevebl   modified for use by SDK samples
//
//--------------------------------------------------------------------------

ULONG CSafeRefCount::SafeRelease()
{
        ULONG   cRefs;

        if( m_cRefs > 0 )
        {
                cRefs = --m_cRefs;

                if( m_cRefs == 0 && m_cNest == 0 && m_fInDelete == FALSE )
                {
                        m_fInDelete = TRUE;
                        delete this;
                }
        }
        else
        {
                // somebody is releasing a non-addrefed pointer!!
                assert(NULL == "Release called on a non-addref'ed pointer!\n");
                cRefs = 0;
        }
        return cRefs;
}

//+-------------------------------------------------------------------------
//
//  Member:     CSafeRefCount::IncrementNestCount
//
//  Synopsis:   increments the nesting count of the object
//
//  Arguments:  none
//
//  Returns:    ULONG; the nesting count after increment
//
//  History:    dd-mmm-yy Author    Comment
//              28-Jul-94 alexgo    author
//              8-26-94   stevebl   modified for use by SDK samples
//
//  Notes:      The nesting count is the count of how many times an
//              an object has been re-entered.  For example, suppose
//              somebody calls pFoo->Bar1(), which makes some calls that
//              eventually call pFoo->Bar2();.  On entrace to Bar2, the
//              nest count of the object should be 2 (since the invocation
//              of Bar1 is still on the stack above us).
//
//              It is important to keep track of the nest count so we do
//              not accidentally delete ourselves during a nested invocation.
//              If we did, then when the stack unwinds to the original
//              top level call, it could try to access a non-existent member
//              variable and crash.
//
//--------------------------------------------------------------------------

ULONG CSafeRefCount::IncrementNestCount()
{
        m_cNest++;
        return m_cNest;
}

//+-------------------------------------------------------------------------
//
//  Member:     CSafeRefCount::DecrementNestCount
//
//  Synopsis:   decrements the nesting count and deletes the object
//              (if necessary)
//
//  Effects:    may delete 'this' object!
//
//  Arguments:  none
//
//  Returns:    ULONG, the nesting count after decrement
//
//  Algorithm:  decrements the nesting count.  If the nesting count is zero
//              AND the reference count is zero AND we are not currently
//              trying to delete ourselves, then delete 'this' object
//
//  History:    dd-mmm-yy Author    Comment
//              28-Jul-94 alexgo    author
//              8-26-94   stevebl   modified for use by SDK samples
//
//  Notes:
//
//--------------------------------------------------------------------------

ULONG CSafeRefCount::DecrementNestCount()
{
        ULONG   cNest;

        if( m_cNest > 0 )
        {
                cNest = --m_cNest;

                if( m_cRefs == 0 && m_cNest == 0 && m_fInDelete == FALSE )
                {
                        m_fInDelete = TRUE;
                        delete this;
                }
        }
        else
        {
                // somebody forget to increment the nest count!!
                assert(NULL == "Unbalanced nest count!!");
                cNest = 0;
        }
        return cNest;
}

