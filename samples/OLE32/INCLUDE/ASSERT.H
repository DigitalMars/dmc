//+---------------------------------------------------------------------------
//
//  Microsoft Windows
//  Copyright (C) Microsoft Corporation, 1992 - 1995.
//
//  File:       assert.h
//
//  Contents:   private definition of assert, used by NT SDK OLE2 samples
//
//  History:    8-19-94   stevebl   Created
//
//----------------------------------------------------------------------------

/*
 * Conditional macro definition for function calling type and variable type
 * qualifiers.
 */
#if   ( (_MSC_VER >= 800) && (_M_IX86 >= 300) )

/*
 * Definitions for MS C8-32 (386/486) compiler
 */
#define _CRTAPI1 __cdecl
#define _CRTAPI2 __cdecl

#else

/*
 * Other compilers (e.g., MIPS)
 */
#define _CRTAPI1
#define _CRTAPI2

#endif

#undef  assert

#ifdef NDEBUG

#define assert(exp)     ((void)0)

#else

#ifdef __cplusplus
extern "C" {
#endif
void _CRTAPI1
PopUpAssert(
    void * szFile,
    int iLine,
    void * szMessage);
#ifdef __cplusplus
}
#endif


#define assert(exp) (void)( (exp) || (PopUpAssert(__FILE__, __LINE__, #exp), 0) )
#define _assert(exp, file, line) PopUpAssert(file, line, exp)


#endif  /* NDEBUG */
