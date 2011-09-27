/*** 
*spoly.h - Application-wide definitions
*
*  This is a part of the Microsoft Source Code Samples.
*
*  Copyright (C) 1992-1995 Microsoft Corporation. All rights reserved.
*
*  This source code is only intended as a supplement to Microsoft Development
*  Tools and/or WinHelp documentation.  See these sources for detailed
*  information regarding the Microsoft samples programs.
*
*Purpose:
*
*Implementation Notes:
*
*****************************************************************************/

#include "hostenv.h"
#include "resource.h"
#include "clsid.h"

#if defined(_MAC)
# define STRSTR strstr
#elif defined(WIN32)
# include "statbar.h"
# define STRSTR strstr
#else /* WIN16 */
# include "statbar.h"
# define STRSTR _fstrstr
#endif

#ifdef _MAC
# define UNUSED(X) ((void)(void*)&(X))
#else
# define UNUSED(X) (X)
#endif

#define DIM(X) (sizeof(X) / sizeof(X[0]))

extern "C" void Assert(int, char FAR*, int, char FAR*);
#define ASSERT(X) Assert(X, __FILE__, __LINE__, NULL)
#define ASSERTSZ(X, MSG) Assert(X, __FILE__, __LINE__, MSG)

#ifndef EXPORT
# if defined(WIN32)
#  define EXPORT
# elif defined(_MAC)
#  define EXPORT
# else
#  define EXPORT __export
# endif
#endif

#ifndef NEAR
# if defined(WIN32)
#  define NEAR
# elif defined(_MAC)
#  define NEAR
# else
#  define NEAR __near
# endif
#endif

#if defined(WIN32)
# define CC_CALL        CC_STDCALL
# define METHODCALLTYPE __stdcall
#elif defined(_MAC)
# define CC_CALL        CC_CDECL
# define METHODCALLTYPE
#else
# define CC_CALL        CC_PASCAL
# define METHODCALLTYPE __pascal
#endif

STDAPI InitOle();
STDAPI UninitOle();
STDAPI Revoke();

void IncObjectCount();
void DecObjectCount();
