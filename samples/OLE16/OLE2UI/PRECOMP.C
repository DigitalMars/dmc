/*
 * PRECOMP.C
 *
 * This file is used to precompile the OLE2UI.H header file
 *
 * Copyright (c)1992 Microsoft Corporation, All Right Reserved
 */

#define STRICT  1
#include "ole2ui.h"

#if defined( _DEBUG ) && !defined( DLL_VER )
// This dummy variable is needed in order for the static link version
// of this library to work correctly.  When we include PRECOMP.OBJ
// in our library (.LIB file), it will only get linked into our
// application IF at least one symbol in precomp.c is referenced from
// either our EXE or LIB.  Therefore, we will define a variable in
// PRECOMP.C and reference it in OLE2UI.C which includes the function
// OleUIInitialize which MUST be called by the static lib user.

int near g_nOleUIStaticLibDummy = 0;
#endif
