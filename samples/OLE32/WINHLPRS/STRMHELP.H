//+---------------------------------------------------------------------------
//
//  Microsoft Windows
//  Copyright (C) Microsoft Corporation, 1992 - 1995.
//
//  File:       strmhelp.h
//
//  Contents:   Routines for writting variables to a stream in an
//              architecture independent manner.
//
//  Classes:
//
//  Functions:  WriteDouble
//              ReadDouble
//              SizeDouble
//
//  History:    4-26-94   stevebl   Created
//
//----------------------------------------------------------------------------

#ifndef __STRMHELP_H__
#define __STRMHELP_H__

#ifdef __cplusplus
extern "C" {
#endif

DWORD SizeDouble(void);

HRESULT WriteDouble(IStream * pstm, double d, ULONG FAR * pcbWritten);

HRESULT ReadDouble(IStream * pstm, double * pd, ULONG FAR * pcbRead);

#ifdef __cplusplus
}
#endif

#endif //__STRMHELP_H__
