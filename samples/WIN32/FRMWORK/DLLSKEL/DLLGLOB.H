// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
// PARTICULAR PURPOSE.
//
// Copyright (C) 1993-1995  Microsoft Corporation.  All Rights Reserved.
//
//  MODULE:   dllglob.h
//
//  PURPOSE:
//    Contains declarations for all globally scoped names in the DLL.
//

#ifndef INC_DLLGLOB_H
#define INC_DLLGLOB_H


//-------------------------------------------------------------------------
// Constant definitions

#define DLLEXPORT __declspec(dllexport)


//-------------------------------------------------------------------------
// Exported function declarations

DLLEXPORT BOOL WINAPI DLLFunction1 (HWND, LPSTR);
DLLEXPORT float _stdcall DLLFunction2 (HWND);


//-------------------------------------------------------------------------
// Global variables

extern CRITICAL_SECTION gCriticalSection;
extern float            gRoot;


//-------------------------------------------------------------------------
// Product identifier string defines

#define SZAPPNAME     "DLLSkel"
#define SZDESCRIPTION "Win32 DLL Skeleton"
#define SZVERSION     "Version 4.0"


//-------------------------------------------------------------------------
// Version string definitions

#define SZRCOMPANYNAME "CompanyName"
#define SZRDESCRIPTION "FileDescription"
#define SZRVERSION     "FileVersion"
#define SZRAPPNAME     "InternalName"
#define SZRCOPYRIGHT   "LegalCopyright"
#define SZRTRADEMARK   "LegalTrademarks"
#define SZRPRODNAME    "ProductName"
#define SZRPRODVER     "ProuctVersion"


#endif // INC_DLLGLOB_H
