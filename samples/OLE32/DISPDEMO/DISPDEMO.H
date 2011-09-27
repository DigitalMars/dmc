/*** 
*dispdemo.h
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
*  IDispatch Demo App definitions.
*
*Implementation Notes:
*
*****************************************************************************/

#include "hostenv.h"
#include "resource.h"
#include "clsid.h"

#ifdef _MAC
# define UNUSED(X) ((void)(void*)&(X))
#else
# define UNUSED(X) (X)
#endif

#ifndef NEAR
# define NEAR
#endif

#define DIM(X) (sizeof(X) / sizeof(X[0]))

#define ASSERT(X) Assert(X, __FILE__, __LINE__)


#ifdef __cplusplus
extern "C" {
#endif

STDAPI InitOle(void);
STDAPI UninitOle(void);

STDAPI DoPoly(CLSID);

void   Assert(int, char FAR*, int); 

#ifdef _MAC
void DbPrintf(char*, ...);
#endif

#ifdef __cplusplus
}
#endif

