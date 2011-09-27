// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
// PARTICULAR PURPOSE.
//
// Copyright (C) 1992-1995  Microsoft Corporation.  All Rights Reserved.
//
// MODULE: thrdutil.h
//
// PURPOSE:
//    Contains declarations thrdutil functions
//

#ifndef _THRDUTIL_H
#define _THRDUTIL_H

extern CRITICAL_SECTION g_csOutstanding;

//-------------------------------------------------------------
// thrdutil prototypes

VOID  WaitCloseOutstanding();
VOID  RefreshOutstanding();
DWORD MarkOutstanding(HANDLE *lpHandle);

#endif
