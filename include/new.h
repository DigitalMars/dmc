/* Copyright (C) 1986-2001 by Digital Mars. $Revision: 1.1.1.1 $ */
#if __SC__ || __RCC__
#pragma once
#endif

#ifndef __NEW_H
#define __NEW_H 1

/* Define _CRTAPI1 (for compatibility with the NT SDK) */
#ifndef _CRTAPI1
#define _CRTAPI1 __cdecl
#endif

/* Define _CRTAPI2 (for compatibility with the NT SDK) */
#ifndef _CRTAPI2
#define _CRTAPI2 __cdecl
#endif

/* Define CRTIMP */
#ifndef _CRTIMP
#if defined(_WIN32) && defined(_DLL)
#define _CRTIMP  __declspec(dllimport)
#else
#define _CRTIMP
#endif
#endif

extern "C++"
{

typedef int (__cdecl * _PNH)( unsigned );	//for Microsoft compatibility
typedef void (* PFVV)();			//as defined in ARM

_PNH __cdecl _set_new_handler(_PNH);		//for Microsoft compatibility
PFVV set_new_handler(PFVV);			//as defined in ARM


#if !defined(__NEW_DEFINED)
#define __NEW_DEFINED 1
void *operator new (unsigned size, void* ptr) throw();
#if _ENABLE_ARRAYNEW
void *operator new[](unsigned size, void* ptr) throw();
#endif
#endif


#if _WIN32

#include <exception>
namespace std
{
    struct bad_alloc : exception
    {
	bad_alloc() throw() { }
	bad_alloc(const bad_alloc&) throw() { }
	bad_alloc& operator=(const bad_alloc&) throw() { return *this; }
	virtual ~bad_alloc() throw();
	virtual const char* what() const throw();
    };

    struct nothrow_t {};
    extern const nothrow_t __cdecl nothrow;
    typedef void (__cdecl *new_handler)();
    new_handler __cdecl set_new_handler(new_handler new_p) throw();
}

// Implicitly declared ones
//void* operator new(unsigned size) throw(std::bad_alloc);
//void* operator new[](unsigned size) throw(std::bad_alloc);
//void operator delete(void* ptr) throw();
//void operator delete[](void* ptr) throw();



void* operator new(unsigned size, const std::nothrow_t&) throw();
void operator delete (void* ptr, void*) throw();
void operator delete(void* ptr, const std::nothrow_t&) throw();


#if _ENABLE_ARRAYNEW
void* operator new[](unsigned size, const std::nothrow_t&) throw();
void operator delete[](void* ptr, void*) throw();
void operator delete[](void* ptr, const std::nothrow_t&) throw();
#endif

#else

#endif

}

#endif
