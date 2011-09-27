
// Copyright (C) 1995-2001 by Digital Mars
// All Rights Reserved
// www.digitalmars.com

#if __SC__ || __RCC__ || __DMC__
#pragma once
#endif

#ifndef __TRACE_H
#define __TRACE_H 1

#if __cplusplus
extern "C" {
#endif

#if _DEBUG_TRACE && _WIN32
int  __cdecl trace_setlogfilename(const char *name);
int  __cdecl trace_setdeffilename(const char *name);
void __cdecl trace_term(void);
#else
#define trace_setlogfilename(n)	(void)0
#define trace_setdeffilename(n)	(void)0
#define trace_term()		(void)0
#endif

#if __cplusplus
}
#endif

#endif

