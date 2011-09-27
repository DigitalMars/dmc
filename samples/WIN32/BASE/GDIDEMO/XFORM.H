
/******************************************************************************\
*       This is a part of the Microsoft Source Code Samples. 
*       Copyright (C) 1993 Microsoft Corporation.
*       All rights reserved. 
*       This source code is only intended as a supplement to 
*       Microsoft Development Tools and/or WinHelp documentation.
*       See these sources for detailed information regarding the 
*       Microsoft samples programs.
\******************************************************************************/


#ifdef WIN16
#define APIENTRY FAR PASCAL
typedef WORD WPARAM;
//#else
//typedef DWORD WPARAM;
#endif


#define XFORMCLASS "XFORMDEMO"
#define XFORMTITLE "Transform Demo"

typedef struct _XFORMDATA
{
#ifndef NOT_IMPLEMENTED
    XFORM xForm;
#endif
    POINT pObject[4];
    int   nObject;
} XFORMDATA;
typedef XFORMDATA      *PXFORMDATA;
typedef XFORMDATA NEAR *NPXFORMDATA;
typedef XFORMDATA FAR  *LPXFORMDATA;


/*
** XFORM WINDOW ROUTINES (xform.c)
*/
HWND  FAR      CreateXFormWindow(HWND,int);
LONG  APIENTRY XFormProc(HWND,UINT,WPARAM,LONG);
BOOL           XFormCreateProc(HWND);
VOID           XFormDestroyProc(HWND);
BOOL           XFormCommandProc(HWND,WPARAM,LONG);
VOID           XFormPaintProc(HWND);
