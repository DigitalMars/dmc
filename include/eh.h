
/* Copyright (C) 1986-2003 by Digital Mars.
 * www.digitalmars.com
 * All Rights Reserved.
 */

#if __DMC__ || __RCC__
#pragma once
#endif

#ifndef __EXCEPT_H
#define __EXCEPT_H 1

#if _WIN32
// eh.h is obsolete, use <exception> instead
#include <exception>

// Backwards compatibility
using std::set_terminate;
using std::set_unexpected;
using std::terminate;
using std::unexpected;

#else

typedef void (__cdecl *__eh_fp)();

__eh_fp __cdecl set_terminate(__eh_fp);
__eh_fp __cdecl set_unexpected(__eh_fp);

void __cdecl terminate();
void __cdecl unexpected();

#endif

#endif

