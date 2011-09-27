//+---------------------------------------------------------------------------
//
//  Microsoft Windows
//  Copyright (C) Microsoft Corporation, 1992 - 1995.
//
//  File:       stablize.h
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

#ifndef __STABLIZE__
#define __STABLIZE__

//+-------------------------------------------------------------------------
//
//  Class:      CSafeRefCount
//
//  Purpose:    A concrete class for objects to inherit from.
//              CSafeRefCount will keep track of reference counts,
//              nesting counts, and zombie states, allowing objects
//              to easily manage the liveness of their memory images.
//
//  Interface:
//
//  History:    dd-mmm-yy Author    Comment
//              01-Aug-94 alexgo    author
//
//  Notes:      inherits CPrivAlloc
//
//--------------------------------------------------------------------------

class CSafeRefCount
{
public:
        ULONG   SafeAddRef();
        ULONG   SafeRelease();
        ULONG   IncrementNestCount();
        ULONG   DecrementNestCount();
                CSafeRefCount();
        virtual ~CSafeRefCount();

private:

        ULONG   m_cRefs;
        ULONG   m_cNest;
        BOOL    m_fInDelete;
};

//+-------------------------------------------------------------------------
//
//  Class:      CStabilize
//
//  Purpose:    An instance of this class should be allocated on the
//              stack of every object method that makes an outgoing call.
//              The contstructor takes a pointer to the object's base
//              CSafeRefCount class.
//
//  Interface:
//
//  History:    dd-mmm-yy Author    Comment
//              01-Aug-94 alexgo    author
//
//  Notes:      The constructor will increment the nest count of the
//              object while the destructor will decrement it.
//
//--------------------------------------------------------------------------

class CStabilize
{
public:
        inline CStabilize( CSafeRefCount *pObjSafeRefCount );
        inline ~CStabilize();

private:
        CSafeRefCount * m_pObjSafeRefCount;
};

inline CStabilize::CStabilize( CSafeRefCount *pObjSafeRefCount )
{
        pObjSafeRefCount->IncrementNestCount();
        m_pObjSafeRefCount = pObjSafeRefCount;
}

inline CStabilize::~CStabilize()
{
        m_pObjSafeRefCount->DecrementNestCount();
}

#endif  // __STABLIZE__
