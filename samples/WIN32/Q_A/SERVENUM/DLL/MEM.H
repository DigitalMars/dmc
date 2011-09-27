// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
// PARTICULAR PURPOSE.
//
// Copyright (C) 1992-1995  Microsoft Corporation.  All Rights Reserved.
//
// MODULE: mem.h
//
// PURPOSE:
//    Contains declarations memory allocation functions
//

#ifndef _MEM_H
#define _MEM_H

//-------------------------------------------------------------
// mem prototypes

BOOL MallocMem( LPVOID *lplpv, DWORD dwSize );
BOOL ReallocMem( LPVOID *lplpv, DWORD dwSize );
VOID FreeMem( LPVOID *lplpv );

#endif
