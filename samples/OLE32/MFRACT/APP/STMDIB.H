//+---------------------------------------------------------------------------
//
//  Microsoft Windows
//  Copyright (C) Microsoft Corporation, 1992 - 1995.
//
//  File:       stmdib.h
//
//  Contents:   Functions for saving and loading DIBs to OLE streams
//
//  Classes:
//
//  Functions:  WriteDibToStream
//              ReadDibFromStream
//              ReadDibBitmapInfoFromStream
//
//  History:    4-22-94   stevebl   Created
//
//----------------------------------------------------------------------------

#ifndef __STMDIB_H__
#define __STMDIB_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <windows.h>
#include <ole2.h>

HRESULT WriteDibToStream (IStream * pstm, HANDLE hdib);
HRESULT ReadDibFromStream (IStream * pstm, HANDLE * phDib);
HRESULT ReadDibBitmapInfoFromStream(IStream * pstm, HANDLE * phbi);

#ifdef __cplusplus
}
#endif

#endif //__STMDIB_H__
