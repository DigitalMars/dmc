//+---------------------------------------------------------------------------
//
//  Microsoft Windows
//  Copyright (C) Microsoft Corporation, 1992 - 1995.
//
//  File:       engnlist.h
//
//  Contents:   definition of the Engine List class
//
//  Classes:    CEngineList
//
//  Functions:
//
//  History:    4-14-94   stevebl   Created
//
//----------------------------------------------------------------------------

#ifndef __ENGNLIST_H__
#define __ENGNLIST_H__

#include <windows.h>
#include "fractal.h"

#ifdef __cplusplus

//+---------------------------------------------------------------------------
//
//  Class:      CEngineList
//
//  Purpose:    implements a simple list of fractal engines
//
//  Interface:  CEngineList  -- constructor
//              ~CEngineList -- destructor
//              Read         -- reads the list from the registry and attaches
//                              the Choose Fractal Type popup menu to the
//                              main menu
//              FindEntry    -- looks up the index of a fractal engine
//                              by its class ID
//              GetEntry     -- gets the class ID of a fractal engine
//
//  History:    4-14-94   stevebl   Created
//
//----------------------------------------------------------------------------

class CEngineList
{
public:
    CEngineList(HMENU hmenuOuter);
    ~CEngineList();
    HRESULT Read();
    int FindEntry(CLSID cid);
    CLSID GetEntry(unsigned uIndex);
private:
    CLSID * _rgCID;
    TCHAR ** _rgszName;
    HMENU _hMenu;
    HMENU _hmenuOuter;
    unsigned _nEntries;
};

#endif // __cplusplus

#endif // __ENGNLIST_H__
