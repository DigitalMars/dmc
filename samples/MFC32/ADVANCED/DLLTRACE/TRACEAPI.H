// traceapi.h: Defines TRACER.DLL application interface
// This interface can be included by C or C++ code
//
// This is a part of the Microsoft Foundation Classes C++ library.
// Copyright (C) 1992-1995 Microsoft Corporation
// All rights reserved.
//
// This source code is only intended as a supplement to the
// Microsoft Foundation Classes Reference and related
// electronic documentation provided with the library.
// See these sources for detailed information regarding the
// Microsoft Foundation Classes product.

#ifdef __cplusplus
extern "C" {
#endif  /* __cplusplus */

struct TracerData
{
	BOOL    bEnabled;
	UINT    flags;
};

BOOL FAR PASCAL EXPORT PromptTraceFlags(HWND hWndParent, TracerData FAR* lpData);

BOOL FAR PASCAL EXPORT FilterDllMsg(LPMSG lpMsg);
void FAR PASCAL EXPORT ProcessDllIdle();

#ifdef __cplusplus
}
#endif
