
/******************************************************************************\
*       This is a part of the Microsoft Source Code Samples. 
*       Copyright (C) 1993-1995 Microsoft Corporation.
*       All rights reserved. 
*       This source code is only intended as a supplement to 
*       Microsoft Development Tools and/or WinHelp documentation.
*       See these sources for detailed information regarding the 
*       Microsoft samples programs.
\******************************************************************************/

/****************************** Module Header *******************************
* Module Name: GUTILS.C
*
* Entry point for GUTILS.DLL
*
* Functions:
*
* DllMain()
*
****************************************************************************/

#include <windows.h>

#include "gutils.h"
#include "gutilsrc.h"

/* dll global data */
HANDLE hLibInst;
extern void gtab_init(void);
extern BOOL StatusInit(HANDLE);

BOOL APIENTRY DllMain(HANDLE hInstance, DWORD dwReason, LPVOID reserved)
{
        if (dwReason == DLL_PROCESS_ATTACH) {
                hLibInst = hInstance;
                gtab_init();
                StatusInit(hLibInst);
        }
        return(TRUE);
}

