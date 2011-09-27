
/******************************************************************************\
*       This is a part of the Microsoft Source Code Samples. 
*		  Copyright (C) 1994 Microsoft Corporation.
*       All rights reserved. 
*       This source code is only intended as a supplement to 
*       Microsoft Development Tools and/or WinHelp documentation.
*       See these sources for detailed information regarding the 
*       Microsoft samples programs.
\******************************************************************************/

/*++

Copyright (c) 1994  Microsoft Corporation

Module Name:

    tlist.c

Abstract:

    This module implements a task list application.

--*/

#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "common.h"


#define MAX_TASKS           256

#define PrintTask(idx) \
        printf( "%4d %-16s", tlist[idx].dwProcessId, tlist[idx].ProcessName ); \
        if (tlist[idx].hwnd) { \
            printf( "  %s", tlist[idx].WindowTitle ); \
        } \
        printf( "\n" );


DWORD numTasks;
TASK_LIST tlist[MAX_TASKS];

VOID Usage(VOID);


int _cdecl
main(
    int argc,
    char *argv[]
    )

/*++

Routine Description:

    Main entrypoint for the TLIST application.  This app prints
    a task list to stdout.  The task list include the process id,
    task name, ant the window title.

Arguments:

    argc             - argument count
    argv             - array of pointers to arguments

Return Value:

    0                - success

--*/

{
    DWORD          i;
    TASK_LIST_ENUM te;
    BOOL fTree;

    if (argc > 1 && (argv[1][0] == '-' || argv[1][0] == '/') && argv[1][1] == '?') {
        Usage();
    }

    fTree = FALSE;

    //
    // Obtain the ability to manipulate other processes
    //
    EnableDebugPriv();

    //
    // get the task list for the system
    //
    numTasks = GetTaskList( tlist, MAX_TASKS );

    //
    // enumerate all windows and try to get the window
    // titles for each task
    //
    te.tlist = tlist;
    te.numtasks = numTasks;
    GetWindowTitles( &te );

    //
    // print the task list
    //
    for (i=0; i<numTasks; i++) {
	PrintTask( i );
    }

    //
    // end of program
    //
    return 0;
}

VOID
Usage(
    VOID
    )

/*++

Routine Description:

    Prints usage text for this tool.

Arguments:

    None.

Return Value:

    None.

--*/

{
    fprintf( stderr, "Microsoft (R) Windows NT (TM) Version 3.5 TLIST\n" );
    fprintf( stderr, "Copyright (C) 1994 Microsoft Corp. All rights reserved\n\n" );
    fprintf( stderr, "usage: TLIST\n" );
    ExitProcess(0);
}
