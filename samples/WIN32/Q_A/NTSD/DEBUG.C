/******************************Module*Header*******************************\
* Module Name: debug.c
*
* Debugger extension for dumping the INFO structure in NTSD
*
* Created: 24-Feb-1992 20:47:03
* Author: Petrus Wong
*
* Copyright (c) 1990-1995 Microsoft Corporation
*
* 1. Place the DLL in the correct path
* 2. Run NTSD -d mdi
* 3. Upon NTSD prompt, do
*       !debug.dinfo address
*       where address is the address of Info
*
* Dependencies:
*
*   (#defines)
*   (#includes)
*
\**************************************************************************/
#include <stdarg.h>
#include <stdio.h>

#include <stddef.h>
#include <windows.h>
#include <winerror.h>

#include <excpt.h>

typedef LARGE_INTEGER PHYSICAL_ADDRESS, *PPHYSICAL_ADDRESS;

#include <ntsdexts.h>
#include "mditypes.h"

//
// don't use try/except here.  Otherwise, mip2coff -c chokes with the
// following fatal error on MIPS:
//      tried to get local variable absolute addr with no procedure
//
// NTSD will catch the fault anyway
//
// Alternatively, we can get rid of the -c switch on mip2coff in the
// module that use try/except, but we won't have any locals for the module
//
// Petrus Wong 05-Oct-1992
//
// We use mcl now which eliminates the need for mip2coff so we can use
// try/except again
//
// Petrus Wong 10-Feb-1993
//
#define move(dst, src)\
try {\
    ReadProcessMemory(hCurrentProcess, (LPVOID)(src), &(dst), sizeof(dst), NULL);\
} except (EXCEPTION_EXECUTE_HANDLER) {\
    return;\
}

/******************************Public*Routine******************************\
*
* DllMain
*
* Effects:   DllMain is called by Windows when
*            the DLL is initialized, Thread Attached, and other times.
*
* Warnings:  The DllMain function should perform additional initialization
*            tasks required by the DLL.  In this example, no initialization
*            tasks are required.  DllMain should return a value of 1 if
*            the initialization is successful.
*
* History:
*  24-Feb-1992 -by- Petrus Wong
* Wrote it.
\**************************************************************************/

BOOL  WINAPI DllMain(HANDLE hInst, DWORD ul_reason_being_called, LPVOID lpReserved)
{
    return 1;
	UNREFERENCED_PARAMETER(hInst);
	UNREFERENCED_PARAMETER(ul_reason_being_called);
	UNREFERENCED_PARAMETER(lpReserved);
}


/******************************Public*Routine******************************\
* dinfo
*
*
* Effects: Debugger extension to dump the INFO structure
*
* To invoke the routine in NTSD, do the following:
*
*   1. place the debug.dll in the path
*   2. on NTSD prompt, type
*
*      !debug.dinfo <address of the INFO structure>
*
* Warnings:
*
* History:
*  24-Feb-1992 -by- Petrus Wong
* Wrote it.
\**************************************************************************/

void dinfo (
    HANDLE hCurrentProcess,
    HANDLE hCurrentThread,
    HANDLE dwCurrentPc,
    PNTSD_EXTENSION_APIS lpExtensionApis,
    LPSTR lpArgumentString)
{
    PNTSD_OUTPUT_ROUTINE Print;
    PNTSD_GET_EXPRESSION EvalExpression;
    PNTSD_GET_SYMBOL GetSymbol;

    PINFO   pInfo;
    INFO    info;

    Print = lpExtensionApis->lpOutputRoutine;
    EvalExpression = lpExtensionApis->lpGetExpressionRoutine;
    GetSymbol = lpExtensionApis->lpGetSymbolRoutine;

    pInfo = (PINFO)EvalExpression(lpArgumentString);
    move(info, pInfo);
    Print(info.CaptionBarText);
    Print("\n");
    Print("hParent             = %x\n", info.hParent         );
    Print("hTextWnd            = %x\n", info.hTextWnd        );
    Print("rcClient(l,t,r,b)   = (%d,%d,%d,%d)\n",
                                 info.rcClient.left,  info.rcClient.top,
                                 info.rcClient.right, info.rcClient.bottom );
    return;
}

