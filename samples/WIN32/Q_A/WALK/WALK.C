
/******************************************************************************\
*       This is a part of the Microsoft Source Code Samples. 
*       Copyright (C) 1992-1995 Microsoft Corporation.
*       All rights reserved. 
*       This source code is only intended as a supplement to 
*       Microsoft Development Tools and/or WinHelp documentation.
*       See these sources for detailed information regarding the 
*       Microsoft samples programs.
\******************************************************************************/

/****************************************************************************
*
*    PROGRAM: WALK.C
*
*    PURPOSE: Uses recursion to walk through all subdirectories in the
*             current working directory
*
*    FUNCTIONS:
*
*        FindFirstDirectory() - finds the first directory in the current
*                 working directory
*        Walk() - finds the subdirectories in the current working directory,
*                 changes the current working directory to this subdirectory,
*                 recusively calls itself until there are no more
*                 subdirectories
*
*    COMMENTS:
*
****************************************************************************/

#include <windows.h>
#include <stdio.h>
#include <string.h>
#include <process.h>
#include "walk.h"

/****************************************************************************
*
*    FUNCTION: FindFirstDirectory(LPTSTR, LPWIN32_FIND_DATA)
*
*    PURPOSE: finds the first directory in the current working directory
*
*    COMMENTS:
*
*       This function is called by Walk() each time a new subdirectory is
*       entered.  Since only directory entries are of interest, this call
*       provides a simple means to bypass non-directory filenames.
*
*    INPUT: lpszSearchFile -> "*"
*           lpffd - pointer to the file find data structure of type
*                   WIN32_FIND_DATA
*
*    OUTPUT: Returns a file handle if a directory is found
*            Returns a -1 if no directory is found        
*
****************************************************************************/

HANDLE FindFirstDirectory(LPTSTR lpszSearchFile, LPWIN32_FIND_DATA lpffd)
{
  BOOL     bRC;
  DWORD    dwRC;
  HANDLE   hSearch;
  int      iRC;

  hSearch=FindFirstFile(lpszSearchFile,lpffd);
  if (hSearch == (HANDLE) -1)
     return (hSearch);
  for(;;)
     {
     dwRC=GetFileAttributes(lpffd->cFileName);
     if (dwRC & FILE_ATTRIBUTE_DIRECTORY)
        {
        iRC=strcmp(lpffd->cFileName,".");
        if (iRC)
           {
           iRC=strcmp(lpffd->cFileName,"..");
           if (iRC)
              return (hSearch);
           }
        }
     bRC=FindNextFile(hSearch,lpffd);
     if (bRC == FALSE)
        {
        FindClose(hSearch);
        return ((HANDLE) -1);
        }
     }
}

/****************************************************************************
*
*    FUNCTION: FindNextDirectory(LPTSTR, LPWIN32_FIND_DATA)
*
*    PURPOSE: finds the next directory in the current working directory
*
*    COMMENTS:
*
*       This function is called by Walk() each time a new subdirectory is
*       entered.  Since only directory entries are of interest, this call
*       provides a simple means to bypass non-directory filenames.
*
*    INPUT: lpszSearchFile -> "*"
*           lpffd - pointer to the file find data structure of type
*                   WIN32_FIND_DATA
*
*    OUTPUT: Returns a file handle if a directory is found
*            Returns a -1 if no directory is found        
*
****************************************************************************/

BOOL FindNextDirectory(HANDLE hSearch, LPWIN32_FIND_DATA lpffd)
{
  BOOL     bRC;
  DWORD    dwRC;

  for(;;)
     {
     bRC=FindNextFile(hSearch,lpffd);
     if (bRC == FALSE)
        return (FALSE);
     dwRC=GetFileAttributes(lpffd->cFileName);
     if (dwRC & FILE_ATTRIBUTE_DIRECTORY)
        return (TRUE);
     }
}

/****************************************************************************
*
*    FUNCTION: Walk(WORD)
*
*    PURPOSE: finds a subdirectory in the current working directory,
*             changes the current working directory to this subdirectory,
*             and recusively calls itself until there are no more
*             subdirectories
*
*    COMMENTS:When a new directory is entered from above, a handle for
*             the new directory is obtained using FindFirstDirectory.  Once
*             the first directory is found, the current working directory
*             is changed to this first directory and Walk() is recursively
*             called again.  At this point, the next available directory
*             is searched for using FindNextFile, entered and a recursive
*             call is made to Walk().  When each directory has been searched,
*             until no more directories exist, the current working directory
*             is changed to the parent directory (..).  This continues until
*             the current working directory is equal to the original
*             directory.
*
*    INPUT: wLevel - bookmark, when wLevel is greater than 0, then the
*             current working directory is a subdirectory of the original
*             directory.  If wLevel is equal to 0, then the directory is the
*             original directory and the recursive calls stop
*
*    OUTPUT: 
*
****************************************************************************/

VOID Walk(WORD wLevel)
{
  BOOL              bRC=TRUE;
  DWORD             dwRC;
  HANDLE            hSearch;
  WIN32_FIND_DATA   w32FindBuf;

  dwRC=GetCurrentDirectory(BUFSIZE,chPathName);
  if (dwRC >= (BUFSIZE-1))
     {
     printf("Buffer too small for path name.  Exiting...\n");
     exit(1);
     }
  printf("%s\n",chPathName);
  hSearch=FindFirstDirectory("*",&w32FindBuf);
  if (hSearch == (HANDLE) -1)
     {
     if (wLevel)
        SetCurrentDirectory("..");
     return;
     }

  for (;;)
     {
     SetCurrentDirectory(w32FindBuf.cFileName);
     Walk(++wLevel);
     bRC=FindNextDirectory(hSearch,&w32FindBuf);
     if (bRC == FALSE)
        {
        SetCurrentDirectory("..");
        FindClose(hSearch);
        return;
        }
     }
}

VOID main(VOID)
{
    // check if Win32s, if so, display notice and terminate
	  if( GetVersion() & 0x80000000 && (GetVersion() & 0xFF) ==3)
      {
        MessageBox( NULL,
           "This application cannot run on Windows 3.1.\n"
           "This application will now terminate.",
           "Walk",
           MB_OK | MB_ICONSTOP | MB_SETFOREGROUND );
        return;
      }

  Walk(0);
}
