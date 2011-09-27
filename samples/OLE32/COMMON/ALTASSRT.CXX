//+---------------------------------------------------------------------------
//
//  Microsoft Windows
//  Copyright (C) Microsoft Corporation, 1992 - 1995.
//
//  File:       altassrt.cxx
//
//  Contents:   Assertion routines for the SDK
//
//  Functions:  NTSDKAssert
//              PopUpError
//
//----------------------------------------------------------------------------

#include <stdarg.h>
#include <stdio.h>

// ensure that this module always compiles with DEBUG settings.
#undef  NDEBUG

#include "assert.h"

int
PopUpError(
    char const *szMsg,
    int iLine,
    char const *szFile);

extern "C"
{

# ifdef WIN32
#  undef FAR
#  undef NEAR
# else
#  define MessageBoxA MessageBox
# endif

# include <windows.h>
}

//+---------------------------------------------------------------------------
//
//  Function:   PopUpAssert
//
//  Synopsis:   Display assertion information
//
//  Effects:    Called when an assertion is hit.
//
//  History:    8-19-94   stevebl   Created
//
//----------------------------------------------------------------------------

void _CRTAPI1
PopUpAssert(
    void * szFile,
    int iLine,
    void * szMessage)
{
    int id = PopUpError((char const *)szMessage,iLine, (char const *) szFile);

    if (id == IDCANCEL)
    {
#ifdef WIN32
        DebugBreak();
#else
        _asm int 3;
#endif
    }
}

//+------------------------------------------------------------
// Function:    PopUpError
//
// Synopsis:    Displays a dialog box using provided text,
//              and presents the user with the option to
//              continue or cancel.
//
// Arguments:
//      szMsg --        The string to display in main body of dialog
//      iLine --        Line number of file in error
//      szFile --       Filename of file in error
//
// Returns:
//      IDCANCEL --     User selected the CANCEL button
//      IDOK     --     User selected the OK button
//-------------------------------------------------------------

int
PopUpError(
    char const *szMsg,
    int iLine,
    char const *szFile)
{

    int id;
    static char szAssertCaption[128];
    static char szModuleName[128];

    DWORD tid = GetCurrentThreadId();
    DWORD pid = GetCurrentProcessId();
    char * pszModuleName;

    if (GetModuleFileNameA(NULL, szModuleName, 128))
    {
        pszModuleName = strrchr(szModuleName, '\\');
        if (!pszModuleName)
        {
            pszModuleName = szModuleName;
        }
        else
        {
            pszModuleName++;
        }
    }
    else
    {
        pszModuleName = "Unknown";
    }

    sprintf(szAssertCaption,"Process: %s File: %s line %u, thread id %d.%d",
        pszModuleName, szFile, iLine, pid, tid);


    DWORD dwMessageFlags = MB_SETFOREGROUND | MB_TASKMODAL |
                           MB_ICONEXCLAMATION | MB_OKCANCEL;

    id = MessageBoxA(NULL,
                     (char *) szMsg,
                     (LPSTR) szAssertCaption,
                     dwMessageFlags);

    // If id == 0, then an error occurred.  There are two possibilities
    // that can cause the error:  Access Denied, which means that this
    // process does not have access to the default desktop, and everything
    // else (usually out of memory).  Oh well.

    return id;
}

