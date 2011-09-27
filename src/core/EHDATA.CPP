//- ehdata.cpp
// RTL Support for Exceptions, shared data
// $Revision: 1.1.1.1 $

#include "ehsup.h"

/* NOTE: do not put __tiX in the code segment for VCM */
#if (__LARGE__ || __COMPACT__)
#define _FAR_TIX __cs
#else
#define _FAR_TIX
#endif

extern "C" char __cdecl _FAR_TIX __tiX[] = { 0,'v','o','i','d',0 }; // 'void' rtti

// These are defined in except.cpp
extern void *__eh_new(unsigned);
extern void __eh_delete(void *);

// What to do about these globals in a multithreaded environment?
// What to do if the object is allocated in a DLL, but free'd in another?
void * (__cdecl * __eh_newp)(unsigned) = __eh_new;
void (__cdecl * __eh_delp)(void *) = __eh_delete;
