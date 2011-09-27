// This is a part of the Microsoft Foundation Classes C++ library.
// Copyright (C) 1992-1993 Microsoft Corporation
// All rights reserved.
//
// This source code is only intended as a supplement to the
// Microsoft Foundation Classes Reference and Microsoft
// QuickHelp and/or WinHelp documentation provided with the library.
// See these sources for detailed information regarding the
// Microsoft Foundation Classes product.

// STDAFX.H is the header that includes the standard includes that are used
//  for most of the project.  These are compiled into a pre-compiled header

#ifdef _AFXDLL
// See comment in ..\include\afxv_dll.h on special AFXAPI_DATA macros
#ifdef _AFXCTL
// MFC core and OLE in one OC16.DLL
#define AFXAPI_DATA     
#define AFXAPIEX_DATA   __far
#else
#ifndef _AFXDLLEX
// Defining SMFC25.DLL itself.
#define AFXAPI_DATA    
#else
// Defining one of the _AFXDLL extensions (OLE or DB)
#define AFXAPIEX_DATA  __far
#endif
#endif
#endif  // _AFXDLL

#include "afx.h"
#include "plex.h"
#include "afxcoll.h"

#ifdef _WINDOWS
// public headers
#include "afxwin.h"
#include "afxdlgs.h"
#include "afxext.h"
#include "afxole.h"
#include "afxdb.h"
#ifdef _AFXCTL
#include "ole2ui.h"
#else
#include "afxodlgs.h"
#include "afxsock.h"
#endif
// private headers as well
#include "afxpriv.h"
#include "auxdata.h"
#include "auxvars.h"
#include "oleimpl.h"
#endif

#ifdef _AFXDLL
// For the MFCDLL build, non-header data is always in _DATA
#undef AFXAPP_DATA
#define AFXAPP_DATA    AFXAPI_DATA
#endif


// The MFC library MUST be built with WINVER >= 0x030A (the default)
// even when Windows 3.0 is the target. There are no compatability
// issues, rather this is done for source code maintainability.
#ifdef _WINDOWS
#if (WINVER < 0x030a)
	#error The MFC library MUST be built with WINVER >= 0x030A
#endif
#endif
