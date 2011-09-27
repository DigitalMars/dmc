// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
// PARTICULAR PURPOSE.
//
// Copyright (C) 1993-1995  Microsoft Corporation.  All Rights Reserved.
//
//  MODULE:   dllskel.h
//
//  PURPOSE:
//    Contains declarations for use by applications and other DLLs that
//    link to this DLL.  It is not used to build the DLL itself.
//


#ifndef INC_DLLSKEL_H
#define INC_DLLSKEL_H

#include <windows.h>

#define DLLIMPORT __declspec(dllimport)


DLLIMPORT BOOL WINAPI DLLFunction1 (HWND, LPSTR);
DLLIMPORT float _stdcall DLLFunction2 (HWND);


#endif  // INC_DLLSKEL_H
