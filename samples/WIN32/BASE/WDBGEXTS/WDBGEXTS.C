/**********************************************************************\
*                                                                      *
* Copyright (c) 1992, 1993  Microsoft Corporation                      *
*                                                                      *
* Module Name:                                                         *
*                                                                      *
*    wdbgexts.c                                                        *
*                                                                      *
* Abstract:                                                            *
*                                                                      *
* This file is a ported version of ntsdexts.c, supporting remote       *
* operations in WinDbg command window.				       *
*                                                                      *
* By including wdbgexts.h, an extension has 4 more apis available,     *
* beyond those available to ntsd extensions:			       *
*                                                                      *
*    PWINDBG_READ_PROCESS_MEMORY_ROUTINE lpReadProcessMemoryRoutine;   *
*    PWINDBG_WRITE_PROCESS_MEMORY_ROUTINE lpWriteProcessMemoryRoutine; *
*    PWINDBG_GET_THREAD_CONTEXT_ROUTINE lpGetThreadContextRoutine;     *
*    PWINDBG_SET_THREAD_CONTEXT_ROUTINE lpSetThreadContextRoutine;     *
*								       *
* The apis available to NTSD and WINDBG extensions are: 	       *
*								       *
*    PNTSD_OUTPUT_ROUTINE lpOutputRoutine;			       *
*    PNTSD_GET_EXPRESSION lpGetExpressionRoutine;		       *
*    PNTSD_GET_SYMBOL lpGetSymbolRoutine;			       *
*    PNTSD_DISASM lpDisasmRoutine;				       *
*    PNTSD_CHECK_CONTROL_C lpCheckControlCRoutine;		       *
*								       *
* Author:							       *
*                                                                      *
*    Peter Sun							       *
*                                                                      *
\**********************************************************************/

#include <windows.h>
#include <string.h>
#include "windbgkd.h"
#include <ntsdexts.h>
#include <wdbgexts.h>

#define MAXLEN 80

CHAR  igrepLastPattern[256];
DWORD igrepSearchStartAddress;
DWORD igrepLastPc;


/**********************************************************************\
*                                                                      *
* igrep( )                                                             *
*                                                                      *
* Description:                                                         *
*                                                                      *
*    This function is called as a WINDBG extension to grep the         *
*    instruction stream for a particular pattern.                      *
*                                                                      *
* Arguments:                                                           *
*                                                                      *
*    hCurrentProcess - Supplies a handle to the current process (at    *
*        the time the extension was called).                           *
*                                                                      *
*    hCurrentThread - Supplies a handle to the current thread (at      *
*        the time the extension was called).                           *
*                                                                      *
*    CurrentPc - Supplies the current pc at the time the extension is  *
*        called.                                                       *
*                                                                      *
*    lpExtensionApis - Supplies the address of the functions callable  *
*        by this extension.                                            *
*                                                                      *
*    lpArgumentString   Supplies the pattern and expression for this   *
*        command.                                                      *
*                                                                      *
* Return Value:                                                        *
*                                                                      *
*    None.                                                             *
\**********************************************************************/

VOID igrep (
    HANDLE hCurrentProcess,
    HANDLE hCurrentThread,
    DWORD dwCurrentPc,
    PWINDBG_EXTENSION_APIS lpExtensionApis,
    LPSTR lpArgumentString
    )

{
    DWORD dwNextGrepAddr;
    DWORD dwCurrGrepAddr;
    CHAR SourceLine[256];
    BOOL NewPc;
    DWORD d;
    PNTSD_OUTPUT_ROUTINE lpOutputRoutine;
    PNTSD_GET_EXPRESSION lpGetExpressionRoutine;
    PNTSD_GET_SYMBOL lpGetSymbolRoutine;
    PNTSD_DISASM lpDisasmRoutine;
    PNTSD_CHECK_CONTROL_C lpCheckControlCRoutine;
    LPSTR pc;
    LPSTR Pattern;
    LPSTR Expression;
    CHAR Symbol[64];
    DWORD Displacement;

    UNREFERENCED_PARAMETER( hCurrentProcess );
    UNREFERENCED_PARAMETER( hCurrentThread );

    lpOutputRoutine = lpExtensionApis->lpOutputRoutine;
    lpGetExpressionRoutine = lpExtensionApis->lpGetExpressionRoutine;
    lpGetSymbolRoutine = lpExtensionApis->lpGetSymbolRoutine;
    lpDisasmRoutine = (PNTSD_DISASM) lpExtensionApis->lpDisasmRoutine;
    lpCheckControlCRoutine = lpExtensionApis->lpCheckControlCRoutine;

    if( igrepLastPc && igrepLastPc == dwCurrentPc ) {
        NewPc = FALSE;
    }
    else {
        igrepLastPc = dwCurrentPc;
        NewPc = TRUE;
    }

    //
    // check for pattern.
    //

    pc = lpArgumentString;
    Pattern = NULL;
    Expression = NULL;
    if( *pc ) {
        Pattern = pc;
        while( *pc > ' ' ) {
            pc++;
    }

        //
        // check for an expression
        //

        if( *pc != '\0' ) {
            *pc = '\0';
            pc++;
            if( *pc <= ' ' ) {
                while (*pc <= ' ') {
                    pc++;
                }
            }
            if( *pc ) {
                Expression = pc;
            }
        }
    }

    if( Pattern ) {
        strcpy(igrepLastPattern,Pattern);

        if( Expression ) {
            igrepSearchStartAddress = (lpGetExpressionRoutine)(Expression);
            if( !igrepSearchStartAddress ) {
                igrepSearchStartAddress = igrepLastPc;
                return;
            }
        }
        else {
            igrepSearchStartAddress = igrepLastPc;
        }
    }

    dwNextGrepAddr = igrepSearchStartAddress;
    dwCurrGrepAddr = dwNextGrepAddr;
    d = (lpDisasmRoutine)(&dwNextGrepAddr,SourceLine,FALSE);
    while( d ) {
        if( strstr(SourceLine,igrepLastPattern) ) {
            igrepSearchStartAddress = dwNextGrepAddr;
	    (lpGetSymbolRoutine)((LPVOID)dwCurrGrepAddr,(PUCHAR)Symbol,&Displacement);
            (lpOutputRoutine)("%s",SourceLine);
            return;
        }
        if( (lpCheckControlCRoutine)() ) {
            return;
        }
        dwCurrGrepAddr = dwNextGrepAddr;
        d = (lpDisasmRoutine)(&dwNextGrepAddr,SourceLine,FALSE);
    }
}


/**********************************************************************\
*                                                                      *
* str( )							       *
*                                                                      *
* Routine Description:                                                 *
*                                                                      *
*    This function is called as a WINDBG extension to format and dump  *
*    a counted ansi string.                                            *
*                                                                      *
* Arguments:                                                           *
*                                                                      *
*    hCurrentProcess - Supplies a handle to the current process (at    *
*        the time the extension was called).                           *
*                                                                      *
*    hCurrentThread - Supplies a handle to the current thread (at the  *
*        time the extension was called).                               *
*                                                                      *
*    CurrentPc - Supplies the current pc at the time the extension is  *
*        called.                                                       *
*                                                                      *
*    lpExtensionApis - Supplies the address of the functions callable  *
*        by this extension.                                            *
*                                                                      *
*    lpArgumentString - Supplies the asciiz string that describes the  *
*        ansi string to be dumped.                                     *
*                                                                      *
* Return Value:                                                        *
*                                                                      *
*    None.                                                             *
*                                                                      *
\**********************************************************************/

VOID str (
    HANDLE hCurrentProcess,
    HANDLE hCurrentThread,
    DWORD dwCurrentPc,
    PWINDBG_EXTENSION_APIS lpExtensionApis,
    LPSTR lpArgumentString
    )

{
    CHAR String[MAXLEN];
    size_t Length;
    DWORD dwAddrString;
    CHAR Symbol[64];
    LPSTR StringData;
    DWORD Displacement;
    BOOL b;
    PNTSD_OUTPUT_ROUTINE lpOutputRoutine;
    PNTSD_GET_EXPRESSION lpGetExpressionRoutine;
    PNTSD_GET_SYMBOL lpGetSymbolRoutine;

    UNREFERENCED_PARAMETER( hCurrentProcess );
    UNREFERENCED_PARAMETER( hCurrentThread );
    UNREFERENCED_PARAMETER( dwCurrentPc );

    lpOutputRoutine = lpExtensionApis->lpOutputRoutine;
    lpGetExpressionRoutine = lpExtensionApis->lpGetExpressionRoutine;
    lpGetSymbolRoutine = lpExtensionApis->lpGetSymbolRoutine;

    //
    // Evaluate the argument string to get the address of
    // the string to dump.
    //

    dwAddrString = (lpGetExpressionRoutine)(lpArgumentString);
    if (!dwAddrString) {
	(lpOutputRoutine)( "Invalid Expression." );
	return;
    }

    //
    // Get the symbolic name of the string
    //

    (lpGetSymbolRoutine)((LPVOID)dwAddrString,(PUCHAR)Symbol,&Displacement);

    //
    // Read current process memory and handle remote read as well
    //

    b = (lpExtensionApis->lpReadProcessMemoryRoutine)(
                                                     dwAddrString,
						     String,
						     MAXLEN,
                                                     NULL
                                                     );

    if (!b) {
	(lpOutputRoutine)( "ReadProcessMemory failed." );
	return;
    }

    Length = strlen( String );

    StringData = (LPSTR)LocalAlloc(LMEM_ZEROINIT,Length+1);

    if (!StringData) {
	(lpOutputRoutine)( "LocalAlloc failed. Error = %x", GetLastError());
        return;
    }

    (lpOutputRoutine)(
	"String: %s ; %d bytes at %lx\n",
	String,
	Length,
	dwAddrString
        );

    LocalFree( StringData );
}
