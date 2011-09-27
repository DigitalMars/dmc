// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
// PARTICULAR PURPOSE.
//
// Copyright (C) 1993-1995  Microsoft Corporation.  All Rights Reserved.
//
//  MODULE:   exports.c
//
//  PURPOSE:   Contains the DLL's exported functions.
//
//  FUNCTIONS:
//    DLLFunction1  -  Displays a message passed in by an application
//    DLLFunction2  -  Squares a number and displays the number & its square
//
//  COMMENTS:
//    These functions are simple and are meant to demonstrate different
//    calling conventions and parameters.
//
//    Since DLLs cannot control what types of applications call them, they
//    should be able to work properly with multithreaded applications.
//    This means using critical sections and mutexs as needed.  Do not
//    expect that an application will call DLL functions from within a
//    critical section or mutex!
//


#include <windows.h>
#include <stdio.h>
#include "dllglob.h"


// Global variable definitions
CRITICAL_SECTION gCriticalSection;   // Initialized in DllMain().

float gRoot = 0.0F;


//
//  FUNCTION: DLLFunction1(HWND, LPSTR)
//
//  PURPOSE:
//    To display a message passed in by an application.
//
//  PARAMETERS:
//    hWnd       -  Handle of the window that is to be parent of message box
//    lpszMsg    -  Character string containing a message to display.
//
//  RETURN VALUE:
//    Always returns TRUE
//

DLLEXPORT BOOL WINAPI DLLFunction1(HWND hWnd, LPSTR lpszMsg)
{
    MessageBox(hWnd, lpszMsg, "DLLFunction1()", MB_OK);
    return TRUE;
}


//
//  FUNCTION: DLLFunction2(HWND)
//
//  PURPOSE:
//    Computes the square of a number and displays the number and its square
//    in a messag box.
//
//  PARAMETERS:
//    hWnd       -  Handle of the window that is to be parent of message box
//
//  RETURN VALUE:
//    The square of root.
//
//  COMMENTS:
//    This function uses a critical section to prevent simultaneous
//    updates of the gRoot global variable by multiple threads.  This
//    is important because the value is changed and after it is changed
//    it is formatted in a string.
//
//    This function minimizes the time in the critical section by only
//    copying the global variable into a local stack-based variable.
//    The stack-based variable is then used in calculations.  Since each
//    thread gets its own stack, stack-based variables are not shared
//    by multiple threads and thus don't need to be put into critical
//    sections.
//

DLLEXPORT float _stdcall DLLFunction2(HWND hWnd)
{
    char  szMsg[80];
    float square, localRoot;

    // Minimize time in the critical section by copying global data
    // into a stack-based variable, which can't be modified by other
    // threads.

    EnterCriticalSection(&gCriticalSection);
    localRoot = gRoot += 1.0F;
    LeaveCriticalSection(&gCriticalSection);

    square = localRoot * localRoot;

    sprintf(szMsg, "The square of %f is %f.", localRoot, square);
    MessageBox(hWnd, szMsg, "DLLFunction2()", MB_OK);

    return square;
}

