/******************************Module*Header*******************************\
* Module Name: glscrnsv.h
*
* Include for modified scrnsave.c
*
* Copyright (c) 1996 Microsoft Corporation
*
\**************************************************************************/

#ifndef __glscrnsv_h__
#define __glscrnsv_h__

#include "sscommon.hxx"
#include "sswproc.hxx"

#ifdef __cplusplus
extern "C" {
#endif

extern int DoScreenSave( HWND hwndParent );
extern int DoWindowedScreenSave( LPCTSTR szArgs );
extern int DoConfigBox( HWND hwndParent );

#ifdef __cplusplus
}
#endif

#endif // __glscrnsv_h__
