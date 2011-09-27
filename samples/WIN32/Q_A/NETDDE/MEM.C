// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
// PARTICULAR PURPOSE.
//
// Copyright (C) 1992-1995  Microsoft Corporation.  All Rights Reserved.
//
//  MODULE:   mem.c
//
//  PURPOSE:  Contains memory management routines
//
//  FUNCTIONS:
//    MallocMem - allocates memory
//    ReallocMem - reallocates memory
//    FreeMem - frees memory
//
//  AUTHOR: Craig Link - Microsoft Developer Support
//

#include <windows.h>
#include <windowsx.h>
#include <stdlib.h>
#include <malloc.h>
#include "mem.h"

#ifdef _DEBUG
DWORD dwAlloc = 0;
#endif

//
//  FUNCTION: MallocMem( LPVOID *lplpv, DWORD dwSize )
//
//  PURPOSE:  Dynamically allocates memory
//
//  PARAMETERS:
//    lplpv - address of pointer to newly allocated memory
//    dwSize - number of bytes to allocate
//
//  RETURN VALUE:
//    ERROR_SUCCESS
//    ERROR_NOT_ENOUGH_MEMORY
//
//
DWORD MallocMem( LPVOID *lplpv, DWORD dwSize )
{
    LPVOID lpTemp;

    if ( NULL == ( lpTemp = _fmalloc( (size_t)dwSize ) ) )
        return ERROR_NOT_ENOUGH_MEMORY;

    *lplpv = lpTemp;

#ifdef _DEBUG
#ifdef WIN16
    dwAlloc++;
#else
    InterlockedIncrement(&dwAlloc);
#endif
#endif

    return ERROR_SUCCESS;

}


//
//  FUNCTION: ReallocMem( LPVOID *lplpv, DWORD dwSize )
//
//  PURPOSE:  Dynamically reallocates memory
//
//  PARAMETERS:
//    lplpv - address of pointer to newly reallocated memory
//    dwSize - number of bytes to allocate
//
//  RETURN VALUE:
//    ERROR_SUCCESS
//    ERROR_NOT_ENOUGH_MEMORY
//
//
DWORD ReallocMem( LPVOID *lplpv, DWORD dwSize )
{
    LPVOID lpTemp;

    if ( NULL == ( lpTemp = _frealloc( *lplpv, (size_t)dwSize ) ) )
        return ERROR_NOT_ENOUGH_MEMORY;

#ifdef _DEBUG
    if ( dwSize == 0 )
#ifdef WIN16
        dwAlloc--;
#else
        InterlockedDecrement(&dwAlloc);
#endif
    else if ( *lplpv == NULL )
#ifdef WIN16
        dwAlloc++;
#else
        InterlockedIncrement(&dwAlloc);
#endif
#endif

    *lplpv = lpTemp;

    return ERROR_SUCCESS;
}

//
//  FUNCTION: FreeMem( LPVOID *lplpv )
//
//  PURPOSE:  Frees dynamically allocated memory
//
//  PARAMETERS:
//    lplpv - address of pointer to newly allocated memory 
//            Set to NULL on return
//
//  RETURN VALUE:
//    none
//
VOID FreeMem( LPVOID *lplpv )
{
    _ffree( *lplpv );
    *lplpv = NULL;
#ifdef _DEBUG
#ifdef WIN16
    dwAlloc--;
#else
    InterlockedDecrement(&dwAlloc);
#endif
#endif

}
