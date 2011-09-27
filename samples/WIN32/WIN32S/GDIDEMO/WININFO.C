
/******************************************************************************\
*       This is a part of the Microsoft Source Code Samples. 
*       Copyright (C) 1993 Microsoft Corporation.
*       All rights reserved. 
*       This source code is only intended as a supplement to 
*       Microsoft Development Tools and/or WinHelp documentation.
*       See these sources for detailed information regarding the 
*       Microsoft samples programs.
\******************************************************************************/

/*---------------------------------------------------------------------------*\
| WINDOW INFORMATION MODULE
|   This module contains the routines which deal with obtaining the extra
|   object information associated with a window.  For these to work, the
|   window class must reserve the 0th word of the win-class object to be
|   used to hold global-memory handle.
\*---------------------------------------------------------------------------*/

#include <windows.h>
#include "gdidemo.h"

/*---------------------------------------------------------------------------*\
| ALLOC WINDOW INFO
|   This routine allocates memory out of the application heap for storing
|   extra memory for the window.  It is alway referenced as offset 0.
\*---------------------------------------------------------------------------*/
BOOL FAR AllocWindowInfo(HWND hWnd, WORD wSize)
{
    HANDLE hsd;


    if(hsd = LocalAlloc(LHND,(WORD)wSize))
    {
        SetWindowLong(hWnd,0,(LONG)hsd);
        return(TRUE);
    }
    return(FALSE);
}


/*---------------------------------------------------------------------------*\
| LOCK WINDOW INFO
|   This routine de-references the extra-memory associated with the window.
|   it locks the object and gives the caller a pointer to the memory.
\*---------------------------------------------------------------------------*/
PVOID FAR LockWindowInfo(HWND hWnd)
{
    HANDLE hMem;
    PVOID  pMem;


    pMem = NULL;
    if(hMem = (HANDLE)GetWindowLong(hWnd,0))
        pMem = (PVOID)LocalLock(hMem);

    return(pMem);
}


/*---------------------------------------------------------------------------*\
| UNLOCK WINDOW INFO
|   This routine unlocks the memory the caller has previously locked.
\*---------------------------------------------------------------------------*/
BOOL FAR UnlockWindowInfo(HWND hWnd)
{
    HANDLE hMem;


    if(hMem = (HANDLE)GetWindowLong(hWnd,0))
        if(!LocalUnlock(hMem))
            return(TRUE);

    return(FALSE);
}


/*---------------------------------------------------------------------------*\
| FREE WINDOW INFO
|   This routine frees the object memory associated with the window.
\*---------------------------------------------------------------------------*/
BOOL FAR FreeWindowInfo(HWND hWnd)
{
    LOCALHANDLE hMem;


    if(hMem = (HANDLE)GetWindowLong(hWnd,0))
        LocalFree(hMem);
    return(TRUE);
}
